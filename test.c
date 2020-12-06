
#include<stdio.h>
#include<stdlib.h>
#include<string.h>    
int main()
{
    FILE *fp;
    char ch,line[500],bb[1000] = ""; // initialize bb
    int i=0,ret;
    fp=fopen("tfile.txt","r");
    int lcount =1; //linecount
    if(fp==NULL) printf("File not opened");
    else
    {
        printf("File opened successfully\n");
        while(ret = fscanf(fp," %s ", line))   // use of fscanf
        {
            if(ret == EOF)
            {
                break;
            }
            if(line == "\n") lcount++;
            strcat(bb,line);
            //strcat(bb," ");
        }

        // char *tok=strtok(bb,"\n");
        // int j = 0; 
        // while(tok!=NULL)
        // {
        //     printf("%s ",tok);
        //     tok=strtok(NULL,"\n" );
        //     j++;
        //     printf("%d ", j);   // NULL instead of pointer
        // }
        printf("%s",bb);

       

    }
}