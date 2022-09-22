#ifndef _b_tree_h
#define _b_tree_h
#include <stdio.h>
#include <stdbool.h>

typedef struct record record;
typedef enum status status;
typedef struct hash_record hash_record;
typedef struct hash hash;

record *init_record(size_t key, const char *field);
hash_record *init_hash_record(record *rec);
hash *init_hash();
void release_hash(hash *h);

size_t hash_func(size_t i, size_t max_size);
void insert(hash *has, record *rec);
void search_existence_and_record(hash *has, size_t target, bool *found, record **target_rec);

void print_record(record *rec);
void print_hash(hash *h);
void print_search_existence(hash *has, size_t target);

record *cli_record();
#endif