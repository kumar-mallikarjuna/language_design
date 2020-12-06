#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdlib.h>
#include <string.h>

typedef struct bucket_node_t {
	char key[64];
	void *val;
	struct bucket_node_t *next;
} bucket_node;

typedef struct hashmap_t {
	bucket_node **buckets;
	size_t capacity;
} hashmap;

size_t hash(char*, size_t);
void create_hm(hashmap*, size_t);
void free_bucket(bucket_node*);
void free_hm(hashmap*);
void insert(hashmap*, char*, void*);
bucket_node* get_bucket(hashmap*, char*);
void* get(hashmap*, char*);
int has_key(hashmap*, char*);

#endif
