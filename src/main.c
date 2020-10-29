#include "grammar.h"
#include "tokenStream.h"
#include "parser.h"
#include "traverse.h"

#include <stdio.h>

int main(int argc, char *argv[]){

	const char *g_loc = argv[1];
	const char *s_loc = argv[2];

	grammar G;

	printf("Reading grammar (%s)...\n", g_loc);
	readGrammar(g_loc, &G);
	//printf(" %d\n ", G.rules[ASSIGNMENT]->head->val.e.T);
	// Parse grammar
	readGrammar(g_loc, &G);
	printf("Reading source (%s)...\n", s_loc);
	// Parse source
	tokenStream* s = NULL;
	tokeniseSourcecode(s_loc, &s);
	printstream(s);
	printf("Reading successful");
	node* p_tree = NULL;
	createParseTree(&p_tree, s, G);
	printf("Parse tree created!");
	typeExpressionTable T;
	traverseParseTree(p_tree, &T);

	return 0;
}
