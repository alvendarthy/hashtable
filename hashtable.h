#ifndef HASHMAP_H
#define HASHMAP_H

#define HASH_TABLE_RESIZE_THRETHOLD 3/4

typedef unsigned int uint32;

typedef union {
    char character;
    short short_int;
    int integer;
    long long_int;
    void * ptr;
} hash_key_t;

typedef hash_key_t hash_data_t;

typedef uint32 hashcode_t;

typedef struct {
    hashcode_t hashcode;
    hash_key_t key;
    hash_data_t data;
} hash_entry_t;

typedef hashcode_t (*hash_func_ptr_t)(hashcode_t * hashcode, const hash_key_t key);

typedef struct{
    hash_entry_t * entries;
    hash_func_ptr_t hash_func_ptr;
    size_t size;
    size_t used;
}hash_table_t;
    

#define CALLABLE
/*
 * create a new hash table
 * @ table: the hash table 
 * @ init_size: init size, should be 16, 32, 64 ...
 * @ func_ptr: hash code function, calculate key to hashcode.
*/
CALLABLE int  hash_table_new(hash_table_t * table, int init_size, hash_func_ptr_t func_ptr);

/*
 * free the hash table
 * @ table: hash table
*/
CALLABLE void hash_table_free(hash_table_t * table);


CALLABLE int  hash_table_add_entry(hash_table_t * table, hash_key_t key, hash_data_t data);

CALLABLE hash_entry_t * hash_table_get_by_key(hash_table_t * table, hash_key_t key);

CALLABLE int  hash_table_resize(hash_table_t * old_table, hash_table_t * new_table, int new_size);

#endif 
