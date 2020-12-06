#include <stdio.h>
#include <stdlib.h>
#include "grammar.h"
#include "traverse.h"
#include "parser.h" //UNCOMMENT AFTER PUTTING IN RIGHT FOLDER
#include "typeexp.h"



void print_err_a(int line_num, non_term op, int depth, char *message){
	printf("\n ERROR>> %12d %20s %20s %20s %20s %20s %20s %12d %30s \n", line_num, "Assignment", "***", "***", "***", "***", "***", depth, message);
}

//FOR ADDOP after calling this we do for assignop, Pass <p_assignment> as tree pointer here
void traverseParseTreeAssignment(node *tree, typeExpressionTable T ){

	if(tree == NULL){
		printf("inside null condition \n");
		return;
	}

	if((tree-> child->t ==1) && (tree->child->u.leaf.T == EPSILON)){
		printf("inside epsilon condition \n");
		//Change tree typeE to ANY
        tree->typeExp = ANY;
		//tree->child typeE to ANY
		
        tree->child->typeExp = ANY;
	}

	if(tree->t ==1 && tree->u.leaf.T == ID){
		//Do expression table search
		printf("inside ID condition \n");
		TypeE treeTypeExp = (TypeE)get(&(T.expr), tree->u.leaf.lexeme);
		tree->typeExp = treeTypeExp;
		tree->par->typeExp = treeTypeExp;
	}

	if((tree->t ==1) && (tree->u.leaf.T == ADDOP || tree->u.leaf.T == MULOP || tree->u.leaf.T == B_AND|| tree->u.leaf.T == B_OR)){
		node* operand2 = tree->sibling;
		node* operand1;
		node* parent = tree->par;
		operand1 = parent->par->child;
		printf("inside addop mulop condition \n");
		/*
		while(operand1->sibling != t){
			operand1 = operand1->sibling;
		}
		if(operand1->sym_name == "SQ_CL"){
			operand1 = parent->child;
			node * temp1 = operand1;
			node* temp2 = operand2;	
			int termCount = 1;
			while(operand1->sibling != t){
				if(operand1->t == 0){
					termCount++;
				}
				operand1 = operand1->sibling;
			}
			
			operand1 = parent->child;
			while(true){
				if(operand1->t == 0){
					termCount--;
					if(termCount == 0){
						break;
					}
				}
				operand1 = operand1->sibling;
			}
		}
		*/



		if(operand1->child->t == 0){
			traverseParseTreeAssignment(operand1->child, T);
			//TypeE of operand1->child = opernad1
            operand1->child->typeExp = operand1->typeExp ;
		}
		else{
			//Initialize type of operand1->child using table and make it as operand1's type

			TypeE opType;
			printf("inside before primitive condition \n");
			type_exp *t_exp = (type_exp*)get(&(T.expr), operand1->u.leaf.lexeme);
			if(t_exp->texp_type == PRIMITIVE){
				if(t_exp->prim_exp.basic_el_type == INTEGER_T){
					opType = INTEGER_TYPE;
				}
				else if(t_exp->prim_exp.basic_el_type == BOOLEAN_T){
					opType = BOOLEAN_TYPE;
				}	
				else if(t_exp->prim_exp.basic_el_type == REAL_T){
					opType = REAL_TYPE;
				}
			}
			operand1->child->typeExp = opType;
		}

		if(operand2->child->t == 0){
			traverseParseTreeAssignment(operand2->child, T);
			//TypeE of operand2->child = opernad1
            operand2->child->typeExp = operand1->typeExp;
		}
		else{
			//Initialize type of operand2->child using table and make it as operand2's type
			//TypeE opType = (TypeE)get(&(T->expr), operand2->u.leaf.lexeme); 

			TypeE opType;

			type_exp *t_exp = (type_exp*)get(&(T.expr), operand2->u.leaf.lexeme);
			if(t_exp->texp_type == PRIMITIVE){
				if(t_exp->prim_exp.basic_el_type == INTEGER_T){
					opType = INTEGER_TYPE;
				}
				else if(t_exp->prim_exp.basic_el_type == BOOLEAN_T){
					opType = BOOLEAN_TYPE;
				}	
				else if(t_exp->prim_exp.basic_el_type == REAL_T){
					opType = REAL_TYPE;
				}
			}
			operand2->child->typeExp = opType;
		}
		//Now check if typeE of operand1==operand2, if it is the no error, make typeE = ADDOP ka typeE, else error.
        if(operand1->typeExp == operand2->typeExp){
            
        	tree->typeExp = operand1->typeExp;
        }
        else{
            //error
			printf("inside error else cond\n");
            tree->typeExp = ERROR;
            print_err_a(operand1->u.leaf.line_num, tree->u.internal.V, tree->depth, "Type Mismatch"); //fill
        }
	}
	traverseParseTreeAssignment(tree->child,T);
	traverseParseTreeAssignment(tree->sibling,T);
}



void recurseTreeForActualTypeChecking(node* tree){
	if(tree == NULL){
		return;
	}
	recurseTreeForActualTypeChecking(tree->child);
	recurseTreeForActualTypeChecking(tree->sibling);

	if(tree->child == NULL){
		return;
	}

	node* child = tree->child;
	TypeE typeE = child->typeExp;
	int error = 0;

	while(child->sibling != NULL){
		if(child->typeExp == NA){
			return;
		}

		if(typeE == child->typeExp || child->typeExp == ANY){
			child = child->sibling;
		}
		else{
			tree->typeExp = ERROR;
			error = 1;
			break;
		}
	}
	if(error == 0){
		tree->typeExp = typeE;
	}
}

//Give at p_assignment, calculate for p_assignment, body, start and others above it using it's children in another fucntion, similarly.
void forAssignOp(node *tree, typeExpressionTable T){
	
	if(tree == NULL){
		return;
	}

	//if(tree->t == 1 && tree->u.leaf.T == ASSIGNOP){
	if(tree->u.internal.V == ASSIGNMENT){
		node* op1 = tree->child;
		node* op2 = op1;
         
		while(op2->u.leaf.T != ASSIGNOP){
			op2 = op2->sibling;
		}
		TypeE lhs = INTEGER_TYPE;//(INT IS EX HERE)//TypeE for LHS using op1->child in table.
		op2 = op2->sibling;
		TypeE rhs = op2->typeExp;
		int error = 0;
		while(op2->sibling->u.leaf.T != SEMI_C){
			if(op2->typeExp == rhs || op2->typeExp == ANY){
				op2 = op2->sibling;
			}
			else{
				tree->typeExp = ERROR;
				error = 1;
				break;
			}
		}
		if(error == 0){
			tree->typeExp = lhs;
		}
	}
	else if(tree->u.internal.V == ASSIGNMENTS){

		if(tree->t==1 && tree->child->u.leaf.T == EPSILON){
			return;
		}

		node* op1 = tree->child;
		node* op2 = op1;

		while(op2->u.leaf.T != ASSIGNOP){
			op2 = op2->sibling;
		}
		TypeE lhs = INTEGER_TYPE;//(INT IS EX HERE)//TypeE for LHS using op1->child in table.
		op2 = op2->sibling;
		TypeE rhs = op2->typeExp;
		int error = 0;
		while(op2->sibling->u.leaf.T != SEMI_C){
			if(op2->typeExp == rhs || op2->typeExp == ANY){
				op2 = op2->sibling;
			}
			else{
				tree->typeExp = ERROR;
				error = 1;
				break;
			}
		}
		if(error == 0){
			tree->typeExp = lhs;
		}

	}

	node * new = tree->child;
	while(new->sibling != NULL){
		new = new->sibling;
	}
	
	forAssignOp(new,T);

}

void traverseParseTreeForAssignment(node* tree, typeExpressionTable T){
	traverseParseTreeAssignment(tree->child->sibling->sibling->sibling->sibling->child->sibling, T); //Basically passing p_assignemnt.
	recurseTreeForActualTypeChecking(tree);
	forAssignOp(tree->child->sibling->sibling->sibling->sibling->child->sibling, T);
	
}