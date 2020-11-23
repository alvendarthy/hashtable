#include "my_hash_tool.h"

hash_code_t integer_hash(const hash_key_t* key){
    return (hash_code_t)key->long_t;
}

int integer_key_cmp(const hash_key_t* first, const hash_key_t* second){
    return first->long_t - second->long_t;
}
