#include "hashtable.h"

CALLABLE int hash_table_new(hash_table_t * table, int init_size, hash_func_ptr_t hash_func_ptr, hash_key_cmp_func_ptr_t key_cmp_func_ptr){
    hash_entry_t * p = NULL;

    assert(init_size > 0);

    init_size = get_2_n_size(init_size);

    int real_size = init_size * sizeof(hash_entry_t);


    p = (hash_entry_t *) malloc( real_size );

    if(NULL == p){
        return -1;
    }

    memset(p, 0, real_size);

    table->entries = p;
    table->hash_func_ptr = hash_func_ptr;
    table->hash_key_cmp_func_ptr = key_cmp_func_ptr;
    table->size = init_size;
    table->nbusy = 0;
    table->ndummy = 0;

    return 0;
}

CALLABLE void hash_table_free(hash_table_t * table){   
    hash_entry_t * entries = table->entries;

    if( NULL != entries ){
        free(entries);
    }

    table->entries = NULL;
    table->hash_func_ptr = NULL;
    table->hash_key_cmp_func_ptr = NULL;
    table->size = 0;
    table->nbusy = 0;
    table->ndummy = 0;   
}

INTERNAL int get_2_n_size(size_t size){
    if(IS_POWER_OF_2(size)){
        return size;
    }

    int new_size = 1;
    for(; new_size < size; new_size = new_size << 1);

    return new_size;
}

INTERNAL int hash_entries_next_jump(int nentries, int cur_index){
    assert(IS_POWER_OF_2(nentries));

    return ((5 * cur_index) + 1) % nentries;
}

CALLABLE hash_entry_flag_t hash_table_find_entry(hash_table_t * table, const hash_key_t * key, hash_entry_t ** entry_out){
    hash_func_ptr_t hash_func = table->hash_func_ptr;
    hash_key_cmp_func_ptr_t key_cmp_func = table->hash_key_cmp_func_ptr;
    hash_entry_t * entries = table->entries;
    size_t size = table->size;
    *entry_out = NULL;
    
    hash_code_t code = 0;
    if(key){
        code = hash_func(key);
    }

    int index = code % size;
    hash_entry_flag_t flag = HASH_ENTRY_NOT_FOUND;

    for(int i = 0; i < size; index = hash_entries_next_jump(size, index)){
        flag = entries[index].flag;

        if(NULL == key){
            if(flag == HASH_ENTRY_DUMMY || flag == HASH_ENTRY_FREE){
                *entry_out = &entries[index];
                return entries[index].flag;
            }

            continue;
        } else {
            if(flag == HASH_ENTRY_DUMMY){
                continue;
            }
    
            if(flag == HASH_ENTRY_FREE){
                *entry_out = &entries[index];
                return entries[index].flag;   
            }

            // HASH_ENTRY_BUSY
            if(0 == key_cmp_func(&entries[index].key, key)){
                *entry_out = &entries[index];
                return entries[index].flag;   
            }

            // key do not match
            continue;
        }
    }

    return HASH_ENTRY_NOT_FOUND;
}

CALLABLE int  hash_table_add(hash_table_t * table, const hash_key_t *key, const hash_data_t *data){
    assert(NULL != table);
    assert(NULL != key);
    assert(NULL != data);    

    double size = table->size;
    double used = table->nbusy + table->ndummy;
    double rate = used/size;

    if(rate >= HASH_TABLE_RESIZE_THRETHOLD){
        if(-1 == hash_table_resize(table, size * 4)){
            return -1;
        }
    }


    hash_code_t hash_code = 0;
    hash_func_ptr_t hash_func_ptr = table->hash_func_ptr;

    hash_code = hash_func_ptr(key);
    hash_entry_t *entry = NULL;

    if( HASH_ENTRY_NOT_FOUND == hash_table_find_entry(table, key, &entry)){
        return -1;
    }

    entry->hashcode = table->hash_func_ptr(key);
    entry->key = *key;
    entry->data = *data;

    if(HASH_ENTRY_FREE == entry->flag){
        table->nbusy ++;
    } else if(HASH_ENTRY_DUMMY == entry->flag){
        table->nbusy++;
        table->ndummy--;
    }
    entry->flag = HASH_ENTRY_BUSY;

    return 0;
}

CALLABLE const hash_data_t * hash_table_get_by_key(hash_table_t * table, const hash_key_t * key){
    assert(NULL != table);
    assert(NULL != key);

    hash_entry_t *entry = NULL;

    if(HASH_ENTRY_BUSY != hash_table_find_entry(table, key, &entry)){
        return NULL;
    }

    const hash_data_t * data = & entry->data;

    return data;
}

CALLABLE int hash_table_delete(hash_table_t * table, const hash_key_t * key){
    assert(NULL != table);
    assert(NULL != key);

    hash_entry_t *entry = NULL;

    if( HASH_ENTRY_BUSY != hash_table_find_entry(table, key, &entry)){
        return -1;
    }

    memset(&entry->data, 0, sizeof(hash_data_t));
    memset(&entry->key, 0, sizeof(hash_key_t));
    entry->flag = HASH_ENTRY_DUMMY;
    entry->hashcode = 0;

    table->nbusy--;
    table->ndummy++;

    return 0;
}

CALLABLE int  hash_table_resize(hash_table_t * table, int new_size){
    new_size = get_2_n_size(new_size);
    hash_table_t new_table = {};

    hash_func_ptr_t hash_func = table->hash_func_ptr;
    hash_key_cmp_func_ptr_t key_cmp_func = table->hash_key_cmp_func_ptr;   
    size_t nbusy = table->nbusy;
    hash_entry_t * entries = table->entries;

    if(-1 == hash_table_new(&new_table, new_size, hash_func, key_cmp_func)){
        return -1;
    }

    for(int i = 0; i < table->size; i ++){
        if(entries[i].flag == HASH_ENTRY_BUSY){
            // add to new
            if(-1 == hash_table_add(&new_table, &entries[i].key, &entries[i].data)){
                goto ERROR;
            }
            nbusy --;
            if(0 == nbusy){
                break;
            }
        }
    }

    hash_table_free(table);

    memcpy(table, &new_table, sizeof(hash_table_t));

    return 0;

ERROR:
    hash_table_free(&new_table);

    return -1;          
}
