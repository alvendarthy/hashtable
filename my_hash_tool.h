#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "hashtable.h"

hash_code_t integer_hash(const hash_key_t *key);
int integer_key_cmp(const hash_key_t *first, const hash_key_t *second);

#endif
