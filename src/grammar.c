typedef struct alpha_t {
	char *VT;

	struct alpha_t *next;
} alpha;

typedef struct rule_t {
	char *LHS;

	struct alpha *RHS;
} rule;

typedef rule* grammar;

void readGrammar(char *g_loc, grammar G){
}
