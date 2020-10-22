#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <stdlib.h>

typedef enum {

} term;

typedef enum {
	START,
} non_term;

typedef struct entity_t {
	int tag;

	union {
		non_term V;
		term T;
	} e;
} entity;

typedef struct entity_ll_t {
	entity val;

	struct entity_ll_t *next;
} entity_ll;

typedef struct alpha_t {
	entity_ll_t *head;

	struct alpha_t *next;
} alpha;

/*
typedef struct rule_t {
	non_term *LHS;

	alpha *RHS;
} rule;
*/

typedef struct {
	size_t capacity = 0;
	non_term start = START;
	alpha **rules = NULL;
	// rule *rules = NULL;
} grammar;

#endif
