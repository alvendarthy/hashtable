#include <stdio.h>
#include "hashtable.h"
#include "my_hash_tool.h"

void main(){
    hash_table_t hash_table;
    
    printf("hello world!\n");

    // create hash table, specific the initial size, it will auto resize.
    if(-1 == hash_table_new(&hash_table, 4, integer_hash, integer_key_cmp)){
        printf("failed to create hashtable.\n");
    }

    // add k-v pairs
    for(int i = 0; i < 1000000; i ++){
        hash_key_t key = {.long_t = 10 * i};
        hash_data_t data = {.long_t = 20 * i};

        if(-1 == hash_table_add(&hash_table, &key, &data)){
            printf("cannot add key-data\n");
            return;
        }
    }

    // get v by k
    hash_key_t key1 = {.long_t = 889890};
    const hash_data_t * data1 = hash_table_get_by_key(&hash_table, &key1);
    if(NULL == data1){
        printf("cannot find this key.\n");
    } else {
        printf("%d -> %d\n", key1.long_t, data1->long_t);
    }

    key1.long_t = 50;
    data1 = hash_table_get_by_key(&hash_table, &key1);
    if(NULL == data1){
        printf("cannot find this key.\n");
    } else {
        printf("%d -> %d\n", key1.long_t, data1->long_t);
    }

    // delete an record
    if(-1 == hash_table_delete(&hash_table, &key1)){
        printf("cannot del this key.\n");
    }

    // try to get it again
    key1.long_t = 50;
    data1 = hash_table_get_by_key(&hash_table, &key1);
    if(NULL == data1){
        printf("cannot find this key.\n");
    } else {
        printf("%d -> %d\n", key1.long_t, data1->long_t);
    }


    // try to get an invalid key
    key1.long_t = 21;
    data1 = hash_table_get_by_key(&hash_table, &key1);
    if(NULL == data1){
        printf("cannot find this key.\n");
    } else {
        printf("%d -> %d\n", key1.long_t, data1->long_t);
    }


    // free the hash table
    hash_table_free(&hash_table);
}
