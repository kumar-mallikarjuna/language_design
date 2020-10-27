#include "grammar.h"
#include "global_map.h"

#include <stdio.h>
#include <string.h>

hashmap non_terminals;
hashmap terminals;

void global_init(){
	// Terminals
	create_hm(&terminals, 64);

	term *x = (term*) malloc(sizeof(term));
	*x = PROGRAM;
	insert(&terminals, "PROGRAM", x);

	x = (term*) malloc(sizeof(term));
	*x = PAR_OP;
	insert(&terminals, "PAR_OP", x);

	x = (term*) malloc(sizeof(term));
	*x = PAR_CL;
	insert(&terminals, "PAR_CL", x);

	x = (term*) malloc(sizeof(term));
	*x = CURL_OP;
	insert(&terminals, "CURL_OP", x);

	x = (term*) malloc(sizeof(term));
	*x = CURL_CL;
	insert(&terminals, "CURL_CL", x);

	x = (term*) malloc(sizeof(term));
	*x = DECLARE;
	insert(&terminals, "DECLARE", x);

	x = (term*) malloc(sizeof(term));
	*x = ID;
	insert(&terminals, "ID", x);

	x = (term*) malloc(sizeof(term));
	*x = COLON;
	insert(&terminals, "COLON", x);

	x = (term*) malloc(sizeof(term));
	*x = LIST;
	insert(&terminals, "LIST", x);

	x = (term*) malloc(sizeof(term));
	*x = OF;
	insert(&terminals, "OF", x);

	x = (term*) malloc(sizeof(term));
	*x = VARIABLES;
	insert(&terminals, "VARIABLES", x);

	x = (term*) malloc(sizeof(term));
	*x = EPSILON;
	insert(&terminals, "EPSILON", x);

	x = (term*) malloc(sizeof(term));
	*x = SEMI_C;
	insert(&terminals, "SEMI_C", x);

	x = (term*) malloc(sizeof(term));
	*x = ARRAY;
	insert(&terminals, "ARRAY", x);

	x = (term*) malloc(sizeof(term));
	*x = INTEGER;
	insert(&terminals, "INTEGER", x);

	x = (term*) malloc(sizeof(term));
	*x = REAL;
	insert(&terminals, "REAL", x);

	x = (term*) malloc(sizeof(term));
	*x = BOOLEAN;
	insert(&terminals, "BOOLEAN", x);

	x = (term*) malloc(sizeof(term));
	*x = SQ_OP;
	insert(&terminals, "SQ_OP", x);

	x = (term*) malloc(sizeof(term));
	*x = ELL;
	insert(&terminals, "ELL", x);

	x = (term*) malloc(sizeof(term));
	*x = SQ_CL;
	insert(&terminals, "SQ_CL", x);

	x = (term*) malloc(sizeof(term));
	*x = NUM;
	insert(&terminals, "NUM", x);

	x = (term*) malloc(sizeof(term));
	*x = JAGGED;
	insert(&terminals, "JAGGED", x);

	x = (term*) malloc(sizeof(term));
	*x = VALUES;
	insert(&terminals, "VALUES", x);

	x = (term*) malloc(sizeof(term));
	*x = SIZE;
	insert(&terminals, "SIZE", x);

	x = (term*) malloc(sizeof(term));
	*x = R1_T;
	insert(&terminals, "R1_T", x);

	x = (term*) malloc(sizeof(term));
	*x = ASSIGNOP;
	insert(&terminals, "ASSIGNOP", x);

	x = (term*) malloc(sizeof(term));
	*x = ADDOP;
	insert(&terminals, "ADDOP", x);

	x = (term*) malloc(sizeof(term));
	*x = MULOP;
	insert(&terminals, "MULOP", x);

	x = (term*) malloc(sizeof(term));
	*x = B_OR;
	insert(&terminals, "B_OR", x);

	x = (term*) malloc(sizeof(term));
	*x = B_AND;
	insert(&terminals, "B_AND", x);

	// Non-terminals
	create_hm(&non_terminals, 64);

	non_term *y = (non_term*) malloc(sizeof(non_term));
	*y = START;
	insert(&non_terminals, "<start>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = BODY;
	insert(&non_terminals, "<body>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = P_DECLARATION;
	insert(&non_terminals, "<p_declaration>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = P_ASSIGNMENT;
	insert(&non_terminals, "<p_assignment>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = DECLARATION;
	insert(&non_terminals, "<declaration>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = S_L_DECLARE;
	insert(&non_terminals, "<s_l_declare>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = DECLARATIONS;
	insert(&non_terminals, "<declarations>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = IDS;
	insert(&non_terminals, "<ids>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = TYPE;
	insert(&non_terminals, "<type>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = DIMS;
	insert(&non_terminals, "<dims>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = IDX;
	insert(&non_terminals, "<idx>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = JAGG_INIS;
	insert(&non_terminals, "<jagg_inis>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = S_OR_M_JAGG;
	insert(&non_terminals, "<s_or_m_jagg>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = JAGG_ARRAY_NUMS;
	insert(&non_terminals, "<jagg_array_nums>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = ARRAY_L;
	insert(&non_terminals, "<array_l>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = OP_DIM;
	insert(&non_terminals, "<op_dim>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = ASSIGNMENT;
	insert(&non_terminals, "<assignment>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = ASSIGNMENTS;
	insert(&non_terminals, "<assignments>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = IS_ARRAY;
	insert(&non_terminals, "<is_array>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = INT_LIST;
	insert(&non_terminals, "<int_list>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = ELE;
	insert(&non_terminals, "<ele>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = EXP;
	insert(&non_terminals, "<exp>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = EXP_2;
	insert(&non_terminals, "<exp_2>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = TERM;
	insert(&non_terminals, "<term>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = TERM_2;
	insert(&non_terminals, "<term_2>", y);

	y = (non_term*) malloc(sizeof(non_term));
	*y = FACTOR;
	insert(&non_terminals, "<factor>", y);
}

void readGrammar(const char *g_loc, grammar *G){
	global_init();

	size_t curr_sz = 256;

	G->capacity = 256;
	G->rules = (alpha**) malloc(sizeof(alpha*)*(G->capacity));
	G->start = START;

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
			entity_ll *n = (entity_ll*) malloc(
					sizeof(entity_ll));

			if(has_key(&non_terminals, token) == 1){
				n->val.tag = 0;
				n->val.e.V = *((non_term*) get(
							&non_terminals,
							token));
			}else if(has_key(&terminals, token) == 1){
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

			token = strtok(NULL, " ");
		}
	}
}
