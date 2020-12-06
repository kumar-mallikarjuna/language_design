#include "hashmap.h"
#include "parser.h"
#include "grammar.h"

#ifndef __TRAVERSE_H__
#define __TRAVERSE_H__

typedef struct id_list_t {
	char id[32];
	struct id_list_t *next;
} id_list;

typedef struct {
	hashmap expr;
	id_list *ids;
	size_t num_ids;
} typeExpressionTable;

void traverseParseTree(node *t, typeExpressionTable *T);

#endif