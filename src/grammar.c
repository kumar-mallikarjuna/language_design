#include "grammar.h"
#include "global_map.h"

#include <stdio.h>
#include <string.h>

void readGrammar(char *g_loc, grammar *G){
	size_t curr_sz = 256;

	G->capacity = 256;
	G->rules = (alpha**) malloc(sizeof(alpha*)*(G->capacity));

	int i;

	for(i=0;i<G->capacity;i++)
		G->rules[i] = NULL;

	FILE *fp = fopen(g_loc, "r");
	char *buff = NULL;
	ssize_t read_sz;
	size_t buff_sz = 0;

	while((read_sz = getline(&buff, &buff_sz, fp)) != -1){
		char *token = strtok(buff, " ");

		non_term *LHS = (non_term*) get(&non_terminals, token);

		token = strtok(NULL, " ");

		alpha *RHS = (alpha*) malloc(sizeof(alpha)*(G->capacity));
		RHS->head = NULL;
		RHS->next = G->rules[*LHS];
		G->rules[*LHS] = RHS;

		entity_ll *head, *ptr;
		head = ptr = NULL;

		while(token){
			token = strtok(NULL, " ");
			entity_ll *n = (entity_ll*) malloc(
					sizeof(entity_ll));

			if(has_key(&non_terminals, token) == 0){
				n->val.tag = 0;
				n->val.e.V = *((non_term*) get(
							&non_terminals,
							token));
			}else if(has_key(&terminals, token) == 0){
				n->val.tag = 1;
				n->val.e.T = *((term*) get(
							&terminals,
							token));
			}

			if(!head){
				head = ptr = n;
			}else{
				ptr->next = n;
				ptr = ptr->next;
			}
		}
	}
}
