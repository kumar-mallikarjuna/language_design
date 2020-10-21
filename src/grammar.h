#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <stdlib.h>

typedef enum {

} term;

typedef enum {

} non_term;

typedef struct entity_t {
	int tag;

	non_term V;
	term a;
} entity;

typedef struct entity_ll_t {
	entity *val;

	struct entity_ll_t *next;
} entity_ll;

typedef struct alpha_t {
	entity_ll_t *str;

	struct alpha_t *next;
} alpha;

typedef struct rule_t {
	non_term *LHS;

	alpha *RHS;
} rule;

typedef struct {
	size_t rules_sz = 0;
	non_term start;
	rule *rules = NULL;
} grammar;

#endif
