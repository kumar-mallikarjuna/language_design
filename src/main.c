#include<stdio.h>

int main(int argc, char *argv[]){

	const char *g_loc = argv[1];
	const char *s_loc = argv[2];

	printf("Reading grammar (%s)...\n", g_loc);
	// Parse grammar
	printf("Reading source (%s)...\n", s_loc);
	// Parse source

	return 0;
}
