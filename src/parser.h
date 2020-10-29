#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "grammar.h"
#include "tokenStream.h"
//#include "typeexp.h"

#ifndef __PARSER_H__
#define __PARSER_H__

typedef struct leaf_val{
	term T;
	char lexeme[50];
	int line_num;
}leaf_val;


typedef struct internal_val{
	non_term V;
	//type_exp type;
	char g_rule[100];
} internal_val;


typedef struct node{
	char sym_name[50];  //remove later
	int depth;
	int t;  //terminal check
	union {
		internal_val internal;
		leaf_val leaf;
	} u;
	struct node* child;
	struct node* sibling;
}node;


typedef struct stack{

	entity rev[50];
	int count;

}stack;

typedef struct PDA_element{

	entity ent;
	node* parent;

}PDA_element;


typedef struct PDA_stack{

	PDA_element p_element[300];
	int count;
}PDA_stack;

void createParseTree(node **p_tree, tokenStream* tks, grammar G);	


#endif
