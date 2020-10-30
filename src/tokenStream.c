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
    if(strcmp(ip,"program")==0)
        return PROGRAM;
    else  if(strcmp(ip,"declare")==0)  return DECLARE;
    else if(strcmp(ip,"list")==0)  return LIST;
    else if(strcmp(ip,"of")==0)    return OF;
    else if(strcmp(ip,"variables")==0) return VARIABLES;
    else if(strcmp(ip,"array")==0) return ARRAY;
    else if(strcmp(ip,"size")==0)  return SIZE;
    else if(strcmp(ip,"values")==0)    return VALUES;
    else if(strcmp(ip,"jagged")==0)    return JAGGED;
    else if(strcmp(ip,"of")==0)    return OF;
    else if(strcmp(ip,"integer")==0)   return INTEGER;
    else if(strcmp(ip,"real")==0)  return REAL;
    else if(strcmp(ip,"boolean")==0) return BOOLEAN; 
    else if(strcmp(ip,"+")==0) return ADDOP;
    else if(strcmp(ip,"-")==0) return ADDOP;
    else if(strcmp(ip,"*")==0) return MULOP;
    else if(strcmp(ip,"/")==0) return MULOP;
    else if(strcmp(ip,":")==0) return COLON;
    else if(strcmp(ip,";")==0) return SEMI_C;
    else if(strcmp(ip,"(")==0) return PAR_OP;
    else if(strcmp(ip,")")==0) return PAR_CL;
    else if(strcmp(ip,"{")==0) return CURL_OP;
    else if(strcmp(ip,"}")==0) return CURL_CL;
    else if(strcmp(ip,"[")==0) return SQ_OP;
    else if(strcmp(ip,"]")==0) return SQ_CL;
    else if(strcmp(ip,"=")==0) return ASSIGNOP;
    else if(strcmp(ip,"..")==0) return ELL;
    else if(strcmp(ip,"R1")==0) return R1_T;
    else if(strcmp(ip,"|||")==0) return B_OR;
    else if(strcmp(ip,"&&&")==0) return B_AND;
    else if(checknum(ip)== 1) return NUM;
    else return ID;
}

void push(tokenStream** head, char tname[50],char lexeme[50], int line_num){
    if(*head == NULL){
	tokenStream* newNode = (tokenStream*)malloc(sizeof(tokenStream));
	strcpy(newNode->lex, lexeme);
	newNode->token = gettok(tname);
	newNode->line_no = line_num;
	*head = newNode;
	return;
	}
	tokenStream* newNode = (tokenStream*)malloc(sizeof(tokenStream));
	strcpy(newNode->lex, lexeme);
	newNode->token = gettok(tname);
	newNode->line_no = line_num;
	tokenStream * trav_tks = *head;
	while(trav_tks->next != NULL) trav_tks = trav_tks->next;
	trav_tks->next = newNode;
	
}

void printstream( tokenStream *s){
 
    while(s != NULL){
   
        printf("%s %d %d\n",s->lex,s->line_no, s->token);
        s= s->next;
    }
}

void tokeniseSourcecode(const char *s_loc, tokenStream **s){ // s_loc denotes “sourcecode.txt” as the input 
    *s = NULL;
    FILE *fptr = fopen(s_loc, "r");
    //FILE *fptr = fopen("tfile.txt", "r");
    char c, *lin, buff[1000] = "";
    int lcount = 0;
    int rline; //read lines with fscanf
    if(fptr == NULL)   printf("File not found\n");
    else{
        
	ssize_t read_sz;
	size_t lin_sz;
	char ch;
        while((read_sz = getline(&lin, &lin_sz, fptr)) != -1){
	if(lin[read_sz-1] == '\n'){
		lin[read_sz-1] = '\0';
		lin_sz--;
	}
        // line number
        lcount++;

	// Tokenizing on POSIX whitespaces
	char *token = strtok(lin, " \t\r\n\f\v");
	while(token){
        	//push(&head, buff, buff , lcount);

		int skip = 1;
		char *ptr = token;
		while(*ptr){
			if(*ptr != ' ' && *ptr != '\t' && *ptr != '\r' && *ptr != '\n' && *ptr != '\f' && *ptr != '\v'){
				skip = 0;
				break;
			}

			ptr++;
		}

		if(skip == 0){
        		strcat(buff,token);
        		push(s, buff, buff , lcount);
			strcpy(buff, "");
		}

		token = strtok(NULL, " \t\r\n\f\v");
	}
    }
    }
//	printstream(s);
        return;
}
