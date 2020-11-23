# hashtable
A hashtable c implementation.

# how it works

## what's the key?

The key object is an union which supports char, long and void * data:
```c
typedef union {
    char char_t;
    long long_t;
    void * ptr_t;
} hash_key_t;
```

You can add more structures if you want. This object acts as the KEY in the hash table.

As I'm no sure the real type of keys, so you have to specify your own hash function, turn the key object into a hash code that will be very important for later HASH MAPPING.

## what's the data?
almost same as the key object, you can attach any pointer to hash_data_t->ptr_t for much rich and personalized data.

## why you should specify your own hash function and key compare function?
Just because they are unions, I've no idea about how to get the hash code and how to compare them. 

You can offer your hash function and key compare function in the init call:
```c
CALLABLE int  hash_table_new(
        hash_table_t * table, 
        int init_size, 
        hash_func_ptr_t func_ptr, 
        hash_key_cmp_func_ptr_t hash_key_cmp_func_ptr);
```

# Core algorithm
## auto resize(only enlarge currently)

See comments [here](https://github.com/python/cpython/blob/master/Objects/dictobject.c)!

The visit table strategy indices via this recurrence:
```text
    j = ((5*j) + 1) mod 2**i
```

There are always 2\*\*i entries in the hash table, 8 for example. The visite order will be:
```text
0 -> 1 -> 6 -> 7 -> 4 -> 5 -> 2 -> 3 -> 0 [and here it's repeating]
```

# More

see [hashtable.h](https://github.com/alvendarthy/hashtable/blob/main/hashtable.h) for details.
