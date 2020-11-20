#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#include "hashtable.h"
CALLABLE int hash_table_new(hash_table_t * table, int init_size, hash_func_ptr_t func_ptr){
    hash_entry_t * p = NULL;
    int real_size = init_size * sizeof(hash_entry_t);

    assert(real_size > 0);

    p = (hash_entry_t *) malloc( real_size );

    if(NULL == p){
        return -1;
    }

    memset(p, real_size, 0);

    table->entries = p;
    table->hash_func_ptr = func_ptr;
    table->size = init_size;
    table->used = 0;

    return 0;
}


CALLABLE void hash_table_free(hash_table_t * table){   
    hash_entry_t * entries = table->entries;

    if( NULL != entries ){
        free(entries);
    }

    table->entries = NULL;
    table->hash_func_ptr = NULL;
    table->size = 0;
    table->used = 0;   
}

CALLABLE int  hash_table_add_entry(hash_table_t * table, hash_key_t key, hash_data_t data){
    hashcode_t hashcode = 0;
    hash_func_ptr_t hash_func_ptr = table->hash_func_ptr;

    if(-1 == hash_func_ptr(&hashcode, key)){
        return -1;
    }

    /*
    * 
    * find the entry and set data.
    */

    return 0;
}
