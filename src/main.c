#include <stdio.h>

#include "grammar.h"

int main(int argc, char *argv[]){

	const char *g_loc = argv[1];
	const char *s_loc = argv[2];

	grammar G;

	printf("Reading grammar (%s)...\n", g_loc);
	readGrammar(g_loc, &G);
	printf("Reading source (%s)...\n", s_loc);
	// Parse source
	printf("HERE%d\n", G.rules[START]->head->val.e.T);

	return 0;
}
