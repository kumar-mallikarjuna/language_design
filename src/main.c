#include "grammar.h"
#include "tokenStream.h"
#include "parser.h"
#include "traverse.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	printf("First argument: <grammar file location>\n");
	printf("Second argument: <dead source code location>\n\n");

	const char *g_loc = argv[1];
	const char *s_loc = argv[2];

	grammar G;

	printf(">> Reading grammar (%s)...\n", g_loc);
	readGrammar(g_loc, &G);
	printf(">> Reading successful\n");

	printf(">> Reading source (%s)...\n\n", s_loc);
	tokenStream* s = NULL;
	tokeniseSourcecode(s_loc, &s);
	printf(">> Reading successful\n");	

	while(1){
		node *p_tree = NULL;
		typeExpressionTable *T = NULL;

		int op;
		printf("\nOperations \n");
		printf("----------\n");
		printf("0: Exit\n");
		printf("1: Create parse tree\n");
		printf("2: Traverse parse tree\n");
		printf("3: Print parse tree\n");
		printf("4: Print typeExpressionTable\n");

		printf("\nInput: ");
		scanf("%d", &op);

		switch(op){
			case 0:
				printf("\nExiting...\nGoodbye!\n\n");
				return 0;
			case 1:
				free(p_tree);

				createParseTree(&p_tree, s, G);
				break;
			case 2:
				if(T)
					free(T);

				T = (typeExpressionTable*) malloc(sizeof(typeExpressionTable));
				if(!p_tree)
					createParseTree(&p_tree, s, G);

				traverseParseTree(p_tree, T);
				break;
			case 3:
				if(!p_tree)
					createParseTree(&p_tree, s, G);
				if(!T){
					T = (typeExpressionTable*) malloc(sizeof(typeExpressionTable));
					traverseParseTree(p_tree, T);
				}

				printParseTree(p_tree);
				break;
			case 4:
				if(!p_tree)
					createParseTree(&p_tree, s, G);
				if(!T){
					T = (typeExpressionTable*) malloc(sizeof(typeExpressionTable));
					traverseParseTree(p_tree, T);
				}

				printTypeExpressionTable(T);
				break;
		}
	}	

	return 0;
}
