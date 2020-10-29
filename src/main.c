#include "grammar.h"
#include "tokenStream.h"

#include <stdio.h>

int main(int argc, char *argv[]){

	const char *g_loc = argv[1];
	const char *s_loc = argv[2];

	grammar G;

	printf("Reading grammar (%s)...\n", g_loc);
	readGrammar(g_loc, &G);
	printf("Reading source (%s)...\n", s_loc);
	// Parse source
	
	tokenStream s;
	tokeniseSourcecode(NULL, &s);

	return 0;
}
