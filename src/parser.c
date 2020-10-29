#include "parser.h"

void init_PDA(PDA_stack *p){

	p->count = 0;
	return;

}


void init_stack(stack *p){
	p->count = 0;
	return;
}


void push_stack(stack *p, entity* et){

	p->rev[p->count].tag = et->tag;
	if(et->tag == 0){
		p->rev[p->count].e.V = et->e.V;
	}
	else{
		p->rev[p->count].e.T = et->e.T;
	}
	p->count++;
	return;
}


void push_PDA(PDA_stack *p, entity *A, node* parent){

	(p->p_element[p->count]).ent.tag = A->tag;
	(p->p_element[p->count]).parent = parent;
	if(A->tag == 0){	
		(p->p_element[p->count]).ent.e.V = (A->e).V;

	}
	else{
		(p->p_element[p->count]).ent.e.T = (A->e).T;
	}
	p->count++;
	return;
}


entity pop_stack(stack *p){
	
	entity temp;

	if(p->count == 0){
		printf("Aux stack is empty!\n");
		exit(0);
		return temp;
	}
	else{
		p->count--;
		temp.tag = p->rev[p->count].tag;
		if(temp.tag == 0){
			temp.e.V = p->rev[p->count].e.V;
		}
		else{
			temp.e.T = p->rev[p->count].e.T;
		}
		return temp;
	}
}

entity pop_PDA(PDA_stack *p){
	
	entity temp;	
	if(p->count == 0){
		printf("PDA stack is empty");
		exit(0);
		return temp;
	}
	else{
		p->count--;
		temp.tag = (p->p_element[p->count]).ent.tag;

		if(temp.tag == 0){
			temp.e.V = (p->p_element[p->count]).ent.e.V;
		}
		else{
			temp.e.T = (p->p_element[p->count]).ent.e.T;
		}
		return temp; 
	}
}


void push_rule(PDA_stack *pd, entity_ll *ent, node* parent){

	stack aux_stack; //to reverse the rules
	init_stack(&aux_stack);
	entity_ll* trav_ent = ent;
	if(ent->val.e.T == EPSILON){
		push_PDA(pd, &(ent->val), parent);
		return;
	}
	else{
		entity* temp;
		entity temp2;
		while(trav_ent != NULL){
			temp = &(trav_ent->val);
			push_stack(&aux_stack, temp);
			trav_ent = trav_ent->next;
			
		}
			
		
		while(aux_stack.count != 0){
			temp2 = pop_stack(&aux_stack);
			push_PDA(pd, &temp2, parent);
		}
		return;
	}
}
/*
//Update!
char* get_sym_name(entity* ent){

	int tag = ent->tag;

	if(tag == 0){
		if(ent->e.V == 0){
			char * A = "START";
			return A;
		}
		else if(ent->e.V == 1){
			char * A = "BODY";
			return A;
		}
		else if(ent->e.V == 2){
			char * A = "DECLARATION";
			return A;
		}
		else if(ent->e.V == 3){
			char * A = "DECLARATIONS";
			return A;
		}
		else if(ent->e.V == 4){
			char * A = "IDS";
			return A;
		}
		else if(ent->e.V == 5){
			char * A = "TYPE";
			return A;
		}
		else if(ent->e.V == 6){
			char * A = "REC_ARRAY";
			return A;
		}
		else if(ent->e.V == 7){
			char * A = "DIMS";
			return A;
		}
		else if(ent->e.V == 8){
			char * A = "DIM";
			return A;
		}
		else if(ent->e.V == 9){
			char * A = "IDX";
			return A;
		}
		else if(ent->e.V == 10){
			char * A = "JAG_ARRAY";
			return A;
		}
		else if(ent->e.V == 11){
			char * A = "JAGG_INIS";
			return A;
		}
		else if(ent->e.V == 12){
			char * A = "JAGG_ARRAY_NUMS";
			return A;
		}
		else if(ent->e.V == 13){
			char * A = "ARRAY_L";
			return A;
		}
		else if(ent->e.V == 14){
			char * A = "OP_DIM";
			return A;
		}
		else if(ent->e.V == 15){
			char * A = "ASSIGNMENTS";
			return A;
		}
		else if(ent->e.V == 16){
			char * A = "ASSIGNMENT";
			return A;
		}
		else if(ent->e.V == 17){
			char * A = "VARIABLE";
			return A;
		}
		else if(ent->e.V == 18){
			char * A = "INT_LIST";
			return A;
		}
		else if(ent->e.V == 19){
			char * A = "A_EXP";
			return A;
		}
		if(ent->e.V == 20){
			char * A = "TERM";
			return A;
		}
		if(ent->e.V == 21){
			char * A = "FACTOR";
			return A;
		}
		if(ent->e.V == 22){
			char * A = "B_EXP";
			return A;
		}
		if(ent->e.V == 23){
			char * A = "B_TERM";
			return A;
		}
	}
	else if(tag == 0){
		if(ent->e.T == 0){
			char * A = "PROGRAM";
			return A;
		}
		if(ent->e.T == 1){
			char * A = "PAR_OP";
			return A;
		}
		if(ent->e.T == 2){
			char * A = "PAR_CL";
			return A;
		}
		if(ent->e.T == 3){
			char * A = "CURL_OP";
			return A;
		}
		if(ent->e.T == 4){
			char * A = "CURL_CL";
			return A;
		}
		if(ent->e.T == 5){
			char * A = "DECLARE";
			return A;
		}
		if(ent->e.T == 6){
			char * A = "ID";
			return A;
		}
		if(ent->e.T == 7){
			char * A = "COLON";
			return A;
		}
		if(ent->e.T == 8){
			char * A = "LIST";
			return A;
		}
		if(ent->e.T == 9){
			char * A = "OF";
			return A;
		}
		if(ent->e.T == 10){
			char * A = "VARIABLES";
			return A;
		}
		if(ent->e.T == 11){
			char * A = "EPSILON";
			return A;
		}
		if(ent->e.T == 12){
			char * A = "SEMI_C";
			return A;
		}
		if(ent->e.T == 13){
			char * A = "ARRAY";
			return A;
		}
		if(ent->e.T == 14){
			char * A = "INTEGER";
			return A;
		}
		if(ent->e.T == 15){
			char * A = "REAL";
			return A;
		}
		if(ent->e.T == 16){
			char * A = "BOOLEAN";
			return A;
		}
		if(ent->e.T == 17){
			char * A = "SQ_OP";
			return A;
		}
		if(ent->e.T == 18){
			char * A = "ELL";
			return A;
		}
		if(ent->e.T == 19){
			char * A = "SQ_CL";
			return A;
		}
		if(ent->e.T == 20){
			char * A = "NUM";
			return A;
		}
		if(ent->e.T == 21){
			char * A = "JAGGED";
			return A;
		}
		if(ent->e.T == 22){
			char * A = "VALUES";
			return A;
		}
		if(ent->e.T == 23){
			char * A = "SIZE";
			return A;
		}
		if(ent->e.T == 24){
			char * A = "R1_T";
			return A;
		}
		if(ent->e.T == 25){
			char * A = "ASSIGNOP";
			return A;
		}
		if(ent->e.T == 26){
			char * A = "ADDOP";
			return A;
		}
		if(ent->e.T == 27){
			char * A = "MULOP";
			return A;
		}
		if(ent->e.T == 28){
			char * A = "B_OR";
			return A;
		}
		if(ent->e.T == 29){
			char * A = "B_AND";
			return A;
		}
	}
}

*/
node* stack_to_tree(node** p_tree, entity* ent, node* parent, tokenStream* tks, entity_ll* rule){

	node* insertion = (node*)malloc(sizeof(node));

	//strcpy(insertion->sym_name, get_sym_name(ent));
	insertion->t = ent->tag;
	if(ent->tag == 0){ //non-terminal
		insertion->u.internal.g_rule = rule;
	}
	else{
		if(ent->e.T == EPSILON){
			strcpy(insertion->u.leaf.lexeme, "EPSILON");
		}
		else{
			strcpy(insertion->u.leaf.lexeme, tks->lex);
			insertion->u.leaf.line_num = tks->line_no;
		}	
	}
	
	insertion->child = NULL;
	insertion->sibling = NULL;

	if(*p_tree == NULL){
		insertion->depth = 0;
		*p_tree = insertion;
	}
	else{
		insertion->depth = parent->depth + 1;
		if(parent->child == NULL){
			parent->child = insertion;
			return insertion;
		}
		else{
			node* find_loc = parent->child;
			while(find_loc->sibling != NULL)
				find_loc = find_loc->sibling;
			find_loc->sibling = insertion;
		}
	}

	return insertion;

}



void rule_expansion(node** p_tree, grammar* G, PDA_stack* pd, tokenStream* tk_pos){
	
	alpha* buck = G->rules[(pd->p_element[pd->count-1].ent.e.V)];
	alpha* within_buck = buck;

	entity temp;
       	node* parent;	
	
	while(within_buck != NULL){
		//only one rule present or derivable first non-terminal matches or epsilon occurs or last rule
		//In case of multiple rules, epsilon rule is always the last rule
		if(within_buck->next == NULL || within_buck->head->val.e.T == tk_pos->token){
			
			parent = stack_to_tree(p_tree ,&(pd->p_element[pd->count-1].ent),
					pd->p_element[pd->count-1].parent, tk_pos, within_buck->head);
			
			temp = pop_PDA(pd);
			
			push_rule(pd, within_buck->head, parent);
			break;
		
		}
		else{
			within_buck = within_buck->next;
		}
	}
}

void recurseTree(node* tree){

	if(tree == NULL){
		return;
	}
	char* na = "NA";
	char * todo = "TODO";
	printf("%s\t%d\t\t%d\t",todo, tree->t, tree->depth);
	//To print type expression.
	if(tree->t == 1){
		printf("%s\t%s\t%d\t%s\n",na, tree->u.leaf.lexeme, tree->u.leaf.line_num, na);
	}
	else{
		printf("\n");
		//printf("TODO\tNA\tNA\tTODO\n");
		entity_ll* rule = tree->u.internal.g_rule;
		/*while(rule != NULL){
			if(rule->val.tag == 0){
				printf(" %s ",get_sym_name(rule->val.e.V));
			}
			else{
				printf(" %s ",get_sym_name(rule->val.e.T));
			}
			
			rule = rule->next;
		}*/
	}

	recurseTree(tree->child);

	recurseTree(tree->sibling);

}


void printParseTree(node* tree){

	if(tree == NULL){
		printf("Empty Tree");
		return;
	}
	printf("\nSymName\tTerminal?\tDepth\tType\tExpression\tLexName\tLineNum\tGrammar Rule\n");

	recurseTree(tree);

	return;
}

void createParseTree(node **p_tree, tokenStream* tks, grammar G){

		
	PDA_stack pd_st;
	init_PDA(&pd_st);
	node *par = NULL;
	entity start;
	start.tag = 0;
	start.e.V = START;

	push_PDA(&pd_st, &start, par); //push the start symbol entity to the stack. MODIFY and intialize the parent = NULL

	tokenStream* trav_tks = tks; 

	if(trav_tks == NULL){
		printf("Token Stream is empty");
		return;
	}

	entity temp;
	entity_ll* empty = NULL; //terminals cannot be expanded

	while(trav_tks != NULL){ //Read until the end of the token stream
	
		//if non-terminal then expand it with a rule
		if(pd_st.p_element[pd_st.count-1].ent.tag == 0){
			//printf("non-term at the top of the stack\n");
			rule_expansion(p_tree, &G, &pd_st, trav_tks);
		}
		else{
			while(pd_st.p_element[pd_st.count-1].ent.tag == 1 && // pop until top element is terminal and matches with the tks ele
					(pd_st.p_element[pd_st.count-1].ent.e.T == trav_tks->token || 
					pd_st.p_element[pd_st.count-1].ent.e.T == EPSILON)){

				node * p = stack_to_tree(p_tree,&(pd_st.p_element[pd_st.count-1].ent), 
						pd_st.p_element[pd_st.count-1].parent, trav_tks, NULL); 
				temp = pop_PDA(&pd_st); //Pop the non-terminal
				//printf("\n lexeme popped is %s\n", trav_tks->lex);


				if(temp.e.T != EPSILON){
				       	trav_tks = trav_tks->next;
				}
			}

			if(pd_st.p_element[pd_st.count-1].ent.tag == 1){ //If top element is a terminal means that it doesn't match
				printf("\nSyntax Error! The TokenStream cannot be parsed with the grammar.\n");
				printf("T at top is %d and token is %d and lexeme '%s' \n",
					       	pd_st.p_element[pd_st.count-1].ent.e.V, trav_tks->token, trav_tks->lex);
				int i = pd_st.count;
				while(i>0){
				printf("Flag is %d and token is %d\n", pd_st.p_element[i-1].ent.tag, 
						pd_st.p_element[i-1].ent.e.T);
				i--;
				}
				exit(0);
			}
		}
	}
	printf("Parse tree is created successfully.\n");
	printf("\n************************");
	printParseTree(*p_tree);
	return;
}
