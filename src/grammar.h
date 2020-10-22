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
	LIST,
	VARIABLES,
	EPSILON,
	SEMI_C,
	ARRAY,
	OF,
	INTEGER,
	REAL,
	BOOLEAN,
	SQ_OP,
	ELL,
	SQ_CL,
	NUM,
	JAGGED,
	VALUES,
	COLON,
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
	DECLARATION,
	DECLARATIONS,
	IDS,
	TYPE,
	REC_ARRAY,
	DIMS,
	DIM,
	IDX,
	JAG_ARRAY,
	JAGG_INIS,
	JAGG_ARRAY_NUMS,
	ARRAY_L,
	OP_DIM,
	ASSIGNMENTS,
	ASSIGNMENT,
	VARIABLE,
	INT_LIST,
	A_EXP,
	TERM,
	FACTOR,
	B_EXP,
	B_TERM
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
