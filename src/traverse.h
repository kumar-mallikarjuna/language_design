#include "hashmap.h"

typedef struct id_list_t {
	char id[32];
	struct id_list_t *next;
} id_list;

typedef struct {
	hashmap expr;
	id_list *ids;
	size_t num_ids;
} typeExpressionTable;
