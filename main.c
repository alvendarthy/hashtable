#include <stdio.h>
#include "hashtable.h"

void main(){
    hash_table_t hash_table;
    
    printf("hello world!\n");

    if(-1 == hash_table_new(&hash_table, 16, NULL)){
        printf("failed to create hashtable.\n");
    }

    hash_table_free(&hash_table);
}
