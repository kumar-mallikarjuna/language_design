//line 37 strcomp
#include <stdio.h>
#include <stdlib.h>
#include "grammar.h"
#include "traverse.h"
#include "parser.h" //UNCOMMENT AFTER PUTTING IN RIGHT FOLDER
/*
typedef struct bucket_node_t {
	char key[64];
	void *val;
	struct bucket_node_t *next;
} bucket_node;

typedef struct hashmap_t {
	bucket_node **buckets;
	size_t capacity;
} hashmap;

typedef struct id_list_t {
	char id[32];
	struct id_list_t *next;
} id_list;

typedef struct {
	hashmap expr;
	id_list *ids;
	size_t num_ids;
} typeExpressionTable;

*/
typedef enum{
	ANY,
	INTEGER_TYPE,
	REAL_TYPE,
	BOOLEAN_TYPE,
	ERROR,
	NA
}TypeE;

//FOR ADDOP after calling this we do for assignop, Pass <p_assignment> as tree pointer here
void traverseParseTree(node *tree, typeExpressionTable T ){
	if(tree == NULL){
		return;
	}
	if((tree-> child->t ==1) && (tree->child->u.leaf.T == EPSILON)){
		//Change tree typeE to ANY
        tree->typeExp = ANY;
		//tree->child typeE to ANY
        tree->child->typeExp = ANY;
	}   
	if(tree->t ==1 && tree->u.leaf.T == ID){
		//Do expression table search
		get(&(T.expr), tree->u.leaf.lexeme);
	}
	if((tree->t ==1) && (tree->u.leaf.T == ADDOP || tree->u.leaf.T == MULOP || tree->u.leaf.T == B_AND|| tree->u.leaf.T == B_OR)){
		node* operand2 = tree->sibling;
		node* operand1;
		node* parent = tree->par;
		node* operand1 = parent->par->child;
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
			traverseParseTree(operand1->child, T);
			//TypeE of operand1->child = opernad1
            operand1->child->typeExp = operand1->typeExp ;
		}
		else{
			//Initialize type of operand1->child using table and make it as operand1's type
			TypeE opType = (TypeE)get(&(T.expr), operand1->u.leaf.lexeme); //for operand1 , t = 1
			operand1->child->typeExp = opType;
			
		}

		if(operand2->child->t == 0){
			traverseParseTree(operand2->child, T);
			//TypeE of operand2->child = opernad1
            operand2->child->typeExp = operand1->typeExp;
		}
		else{
			//Initialize type of operand2->child using table and make it as operand2's type
			TypeE opType = (TypeE)get(&(T.expr), operand2->u.leaf.lexeme); 
			operand2->child->typeExp = opType;
		}
		//Now check if typeE of operand1==operand2, if it is the no error, make typeE = ADDOP ka typeE, else error.
        if(operand1->typeExp == operand2->typeExp){
            operand1->typeExp == ANY;
        }
        else{
            //error 
            print_err_a(operand1->u.leaf.line_num, operand1->depth, "Type Mismatch"); // fill
        }
	}
	recurseTree(tree->child);
	recurseTree(tree->sibling);
}
void recurseTreeForActualTypeChecking(node* tree){
	if(tree == NULL){
		return;
	}
	recurseTree(tree->child);
	recurseTree(tree->sibling);

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
	if(tree->t ==1 && tree->u.leaf.T == ASSIGNOP){
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
	else if(tree->sym_name == ASSIGNMENTS){
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
}