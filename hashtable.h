#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <assert.h>
#include <memory.h>


#define HASH_TABLE_RESIZE_THRETHOLD (3.0/4.0)

typedef unsigned int uint32;

typedef char byte_t;

typedef union {
    char char_t;
    long long_t;
    void * ptr_t;
} hash_key_t;

typedef hash_key_t hash_data_t;

typedef uint32 hash_code_t;

typedef char hash_entry_flag_t;

typedef struct {
    hash_code_t hashcode;
    hash_entry_flag_t flag;
    hash_key_t key;
    hash_data_t data;
} hash_entry_t;

// entry flags
#define HASH_ENTRY_NOT_FOUND -1
#define HASH_ENTRY_FREE  0
#define HASH_ENTRY_BUSY  1
#define HASH_ENTRY_DUMMY 2 // 1 << 1

#define is_entry_free(entry) ((entry)->flag == HASH_ENTRY_FREE)
#define is_entry_busy(entry) ((entry)->flag == HASH_ENTRY_BUSY)
#define is_entry_dummy(entry) ((entry)->flag == HASH_ENTRY_DUMMY)


typedef hash_code_t (*hash_func_ptr_t)(const hash_key_t *key);
typedef int (*hash_key_cmp_func_ptr_t)(const hash_key_t *first, const hash_key_t *second);

typedef struct{
    hash_entry_t * entries;
    hash_func_ptr_t hash_func_ptr;
    hash_key_cmp_func_ptr_t hash_key_cmp_func_ptr;
    size_t size;
    size_t nbusy;
    size_t ndummy;
}hash_table_t;
 

#define IS_POWER_OF_2(x) (((x) & (x-1)) == 0)   

#define CALLABLE
#define INTERNAL static
/*
 * create a new hash table, DO NOT forget to free it.
 *
 * @ table: the hash table 
 * @ init_size: init size, should be 16, 32, 64 ...
 * @ func_ptr: hash code function, calculate key to hashcode.
 * @ hash_key_cmp_func_ptr: cmp keys
 *   0 : first enquals second
 *   -1 : first little than second
 *   1 : first grater than second
 *
 * returns:
 *  -1: error occurs
 *  0 : OK
*/
CALLABLE int  hash_table_new(hash_table_t * table, int init_size, hash_func_ptr_t func_ptr, hash_key_cmp_func_ptr_t hash_key_cmp_func_ptr);

/* add k-v, it will auto resize to enlarge space if necessary.
 * if the key already has a data, this new data will over write the old data.
 *
 * @ table: the hash table.
 * @ key: the key object, an union, see hashtable.h for more info.
 * @ data: the data object, an union too.
 *
 * returns:
 * -1: error occurs
 *  0: OK
 * */
CALLABLE int  hash_table_add(hash_table_t * table, const hash_key_t * key, const hash_data_t *data);

/* delete k-v pair
 *
 * @table: the hash table.
 * @key: the key object.
 *
 * returns:
 *  -1: error occurs
 *   0: OK 
 * */
CALLABLE int hash_table_delete(hash_table_t * table, const hash_key_t * key);

/* get k-v
 *
 * @table: hash table
 * @key: the key object.
 *
 * returns:
 *  a const data object pointer. You cannot make any change to it.
 * */
CALLABLE const hash_data_t * hash_table_get_by_key(hash_table_t * table, const hash_key_t * key);
/*
 * free the hash table
 * @ table: hash table
 *
 * returns:
 *  VOID
*/
CALLABLE void hash_table_free(hash_table_t * table);




//************ FOR INTERNAL CALL ONLY *********

INTERNAL hash_entry_flag_t hash_table_find_entry(hash_table_t * table, const hash_key_t *key, hash_entry_t ** entry_out);
INTERNAL int  hash_table_resize(hash_table_t * table, int new_size);
INTERNAL int get_2_n_size(size_t size);
INTERNAL int hash_entries_next_jump(int nentries, int index);

#endif 
