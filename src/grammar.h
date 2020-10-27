#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <stdlib.h>

typedef enum {
	PROGRAM,
	PAR_OP,
	PAR_CL,
	CURL_OP,
	CURL_CL,
	DECLARE,
	ID,
	COLON,
	LIST,
	OF,
	VARIABLES,
	EPSILON,
	SEMI_C,
	ARRAY,
	INTEGER,
	REAL,
	BOOLEAN,
	SQ_OP,
	ELL,
	SQ_CL,
	NUM,
	JAGGED,
	VALUES,
	SIZE,
	R1_T,
	ASSIGNOP,
	ADDOP,
	MULOP,
	B_OR,
	B_AND
} term;

typedef enum {
	START,
	BODY,
	P_DECLARATION,
	P_ASSIGNMENT,
	DECLARATION,
	S_L_DECLARE,
	DECLARATIONS,
	IDS,
	TYPE,
	DIMS,
	IDX,
	JAGG_INIS,
	S_OR_M_JAGG,
	JAGG_ARRAY_NUMS,
	ARRAY_L,
	OP_DIM,
	ASSIGNMENT,
	ASSIGNMENTS,
	IS_ARRAY,
	INT_LIST,
	ELE,
	EXP,
	EXP_2,
	TERM,
	TERM_2,
	FACTOR
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
	entity_ll *head;

	struct alpha_t *next;
} alpha;

/*
typedef struct rule_t {
	non_term *LHS;

	alpha *RHS;
} rule;
*/

typedef struct grammar_t {
	size_t capacity;
	non_term start;
	alpha **rules;
	// rule *rules = NULL;
} grammar;

void readGrammar(const char *g_loc, grammar *G);

#endif
