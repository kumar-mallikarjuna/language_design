#include<stdlib.h>
#include"grammar.h"
#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

typedef struct tokenstream_node{
    term token; 
    char lex[50];
    int line_no;
    struct tokenstream_node *next; 
} tokenStream;

void tokeniseSourcecode(const char *s_loc, tokenStream **s);
void push(tokenStream** head, char tname[50],char lex[50], int line_no);
term gettok(char* ip);
int checknum(char* ip);
void printstream( tokenStream *s);
#endif

