#include "hashmap.h"

size_t hash(char *str, size_t size){
	size_t hcode = 0;

	char ch = *str;

	while((ch = *(str++))){
		hcode += (int) ch;
	}

	hcode = hcode % size;

	return hcode;
}

void create_hm(hashmap *map, size_t cap){
	map->buckets = (bucket_node**) malloc(sizeof(bucket_node*)*cap);
	map->capacity = cap;

	size_t i;

	for(i=0;i<cap;i++){
		map->buckets[i] = NULL;
	}
}

void free_bucket(bucket_node *n){
	bucket_node *ptr = n;

	while(ptr){
		free(ptr);
		ptr = ptr->next;
	}
}

void free_hm(hashmap *map){
	int i=0;

	while(i < map->capacity){
		free_bucket(map->buckets[i]);
	}
}

void insert(hashmap *map, char *key, void *val){
	size_t loc = hash(key, map->capacity);

	bucket_node *n = (bucket_node*) malloc(sizeof(bucket_node));
	n->key = key;
	n->val = val;
	n->next = map->buckets[loc];

	map->buckets[loc] = n;
}

bucket_node* get_bucket(hashmap *map, char *key){
	size_t loc = hash(key, map->capacity);

	return map->buckets[loc];
}

void* get(hashmap *map, char *key){
	size_t loc = hash(key, map->capacity);

	bucket_node *ptr = map->buckets[loc];

	while(ptr){
		if(!strcmp(ptr->key, key))
			return ptr->val;

		ptr = ptr->next;
	}

	return NULL;
}

int has_key(hashmap *map, char *key){
	return get(map, key) == NULL ? 0 : 1;
}
