#include"tokenStream.h"
#include<stdio.h>
#include<string.h>

int checknum(char* ip){
    char* next;
    long temp = strtol(ip, &next,10 );
    if((*next != '\0') ||(next == ip))   return 0; //not valid number
    else return 1; //valid number 
}

term gettok(char* ip){ //return enum term
    if(strcmp(ip,"program"))
        return PROGRAM;
    else  if(strcmp(ip,"declare"))  return DECLARE;
    else if(strcmp(ip,"list"))  return LIST;
    else if(strcmp(ip,"of"))    return OF;
    else if(strcmp(ip,"variables")) return VARIABLES;
    else if(strcmp(ip,"array")) return ARRAY;
    else if(strcmp(ip,"size"))  return SIZE;
    else if(strcmp(ip,"values"))    return VALUES;
    else if(strcmp(ip,"jagged"))    return JAGGED;
    else if(strcmp(ip,"of"))    return OF;
    else if(strcmp(ip,"integer"))   return INTEGER_T;
    else if(strcmp(ip,"real"))  return REAL_T;
    else if(strcmp(ip,"boolean")) return BOOLEAN_T; 
    else if(strcmp(ip,"+")) return ADDOP;
    else if(strcmp(ip,"-")) return ADDOP;
    else if(strcmp(ip,"*")) return MULOP;
    else if(strcmp(ip,"/")) return MULOP;
    else if(strcmp(ip,":")) return COLON;
    else if(strcmp(ip,";")) return SEMI_C;
    else if(strcmp(ip,"(")) return PAR_OP;
    else if(strcmp(ip,")")) return PAR_CL;
    else if(strcmp(ip,"{")) return CURL_OP;
    else if(strcmp(ip,"}")) return CURL_CL;
    else if(strcmp(ip,"[")) return SQ_OP;
    else if(strcmp(ip,"]")) return SQ_CL;
    else if(strcmp(ip,"=")) return ASSIGNOP;
    else if(strcmp(ip,"..")) return ELL;
    else if(strcmp(ip,"R1")) return R1_T;
    else if(strcmp(ip,"|||")) return B_OR;
    else if(strcmp(ip,"&&&")) return B_AND;
    else if(checknum(ip)== 1) return NUM;
    else return ID;
}

void push(tokenStream** head, char tname[50],char lex[50], int line_no){
    tokenStream* newNode = (tokenStream*)malloc(sizeof(tokenStream));
    strcpy(newNode->lex, lex);
    newNode->token = gettok(tname);
    newNode->line_no = line_no;
    newNode->next = *head;
    *head = newNode;
}

void tokeniseSourcecode( char *s_loc, tokenStream *s){ // s_loc denotes “sourcecode.txt” as the input 
    //FILE *fptr = fopen(s_loc, "r");
    FILE *fptr = fopen("t1.txt", "r");
    char c, *lin = NULL, buff[1000] = "";
    int lcount =0;
    int rline; //read lines with fscanf
    if(fptr == NULL)   printf("File not found\n");
    else{
        //tokenStream* head = NULL;
	
	size_t lin_sz;
	char ch;
        while(getline(&lin, &lin_sz, fptr) != -1){
        // line number
        lcount++;
	char *token = strtok(lin, " \t");
	while(token){
        	//push(&head, buff, buff , lcount);

		int skip = 1;
		char *ptr = token;
		while(*ptr){
			if(*ptr != ' ' && *ptr != '\t' && *ptr != '\n'){
				skip = 0;
				break;
			}

			ptr++;
		}

		if(skip == 0){
        		strcat(buff,token);
        		push(&s, buff, buff , lcount);
			printf("%d%s\n", lcount, token);
		}

		token = strtok(NULL, " ");
	}
    }
    }
}

void printstream( tokenStream *s){
    while(s->next != NULL){
        printf("%d %s %d",s->token,s->lex,s->line_no);
        s= s->next;
    }
}