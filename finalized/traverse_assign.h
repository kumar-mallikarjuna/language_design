#include<stdlib.h>
#include"traverse.h"
#include "typeexp.h"
#include "grammar.h"
#include <stdio.h>

void print_err_a(int line_num, non_term op, int depth, char *message);
void traverseParseTreeForAssignment(node* tree, typeExpressionTable T);
void traverseParseTreeAssignment(node *tree, typeExpressionTable T );
void recurseTreeForActualTypeChecking(node* tree);
void forAssignOp(node *tree, typeExpressionTable T);