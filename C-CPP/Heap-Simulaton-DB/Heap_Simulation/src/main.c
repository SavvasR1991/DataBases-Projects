#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#define MAX_FILES 100
#define FILENAME "file"

int main(int argc, char** argv) {
	int bfs [MAX_FILES];                        
	int i;                                      
	int j;
	int file_descr;                             
    int blkCnt;                                 
	int error=0;                                
	int read,fpb;                               
    int total_records = 0;                      
    int end_of_prog=0;                          
    int overflow=0;                             
	char filename [50],file_in[50],input[50];   //stdin user
	char *pch;                                  //strtok
	char *pch1;
	char pch2[8];
    char * line = NULL;
    size_t len = 0;
    void* block;                                //block
	Record record_entry;                        //records
	FILE *insert_file;                          //Input data files

    printf("\n|------------------------->HEAP FILE PROGRAMM<--------------------------|\n");
    printf("->Please insert the heap file name...\n->");
    fgets(filename,50,stdin);                   //heap file
    filename[strlen(filename)-1]='\0';
    printf("->You have insert the name :\"%s\"\n",filename);
	error=HP_CreateFile(filename);              //heap arxeiou creation
	if(error==-1)                               //error check
	{
        printf("->Failure ,Exiting...\n");
        printf("|---------------------------->END OF PROGRAMM<--------------------------|\n");
        return 0;
	}
	file_descr=HP_OpenFile(filename);           //open file
    if(file_descr<0)                            //check if file is open
    {
        printf("->Failure ,Exiting...\n");
        printf("|---------------------------->END OF PROGRAMM<--------------------------|\n");
        return 0;
    }                                           
    printf("->                   _________________\n");
    printf("->                  |PROGRAMM COMMANDS|\n");
    printf("->-------------------------------------------------------|\n");
    printf("->To load a file of records: load Filename               |\n");
    printf("->To insert a record       : insert Id Name Surname City |\n");
    printf("->To get info of records   : get field value             |\n");
    printf("->To see Database status   : status                      |\n");
    printf("->To exit programm press   : exit                        |\n");
    printf("->-------------------------------------------------------|\n");
    while(1)                                    //main loop
    {
        if(end_of_prog==1)                      //end of programm
        {
            break;
        }
        printf("->Insert your command...\n->");
        fgets(input,50,stdin);                  //stdin command
        pch = strtok (input," ,.-\n");          //user commands check
        while (pch != NULL)
        {                                       //load
            if(strcmp(pch,"load")==0&&overflow==0)
            {
                pch = strtok (NULL,"\n");
                if(pch==NULL)
                {
                    printf("->Error No file selected...\n");
                    break;
                }
                printf("->Loading file : \"%s\"\n",pch);
                insert_file=fopen(pch,"r");     
                if (insert_file == NULL)
                {
                    printf("->Error cannot open file :\"%s\"\n",pch);
                    break;
                }                               
                while ((read = getline(&line, &len, insert_file)) != -1)
                {
                    pch1 = strtok (line, " \",-\n");
                    record_entry.id=atoi(pch1);
                    pch1 = strtok (NULL, " \",-\n");
                    strcpy(record_entry.name,pch1);
                    pch1 = strtok (NULL, " \",-\n");
                    strcpy(record_entry.surname,pch1);
                    pch1 = strtok (NULL, "\",-\n");
                    strcpy(record_entry.city,pch1);
                    pch1 = strtok (NULL, " \"\n");
                    error=HP_InsertEntry(file_descr,record_entry);
                    if(error<0)
                    {
                        printf("->Failure ,Exiting...\n");
                        printf("|---------------------------->END OF PROGRAMM<--------------------------|\n");
                        return 0;
                    }
                    if(error==2)
                    {
                        overflow==1;
                        break;
                    }
                }
                printf("->Loading success...\n");
                fclose(insert_file);            
            }                                   //insert
            else if(strcmp(input,"insert")==0&&overflow==0)
            {
                printf("->Inserting new record...\n");
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Id Name Surname City selected...\n");
                    break;
                }
                printf("->Id        :%s\n",pch);
                record_entry.id=atoi(pch);
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Name Surname City selected...\n");
                    break;
                }
                printf("->Name      :%s\n",pch);
                strcpy(record_entry.name,pch);
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Surname City selected...\n");
                    break;
                }
                printf("->Surname   :%s\n",pch);
                strcpy(record_entry.surname,pch);
                pch = strtok (NULL,"\n");
                if(pch==NULL)
                {
                    printf("->No City selected...\n");
                    break;
                }
                printf("->City      :%s\n",pch);
                strcpy(record_entry.city,pch);
                error=HP_InsertEntry(file_descr,record_entry);
                if(error<0)
                {
                    printf("->Failure ,Exiting...\n");
                    printf("|---------------------------->END OF PROGRAMM<--------------------------|\n");
                    return 0;
                }
                else if(error==2)
                {
                    overflow=1;
                }
                else
                {
                    printf("->The record has been inserted...\n");
                }
            }                                   //exit
            else if(strcmp(input,"exit")==0)
            {
                end_of_prog=1;
                break;
            }                                   //status
            else if(strcmp(input,"status")==0)
            {
                HP_PrintDatabase_Status(file_descr);
            }                                   //get
            else if(strcmp(input,"get")==0)
            {
                printf("->Finding Customers...\n");
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Field and Value selected...\n");
                    break;
                }
                strcpy(pch2,pch);
                printf("->Field  :%s\n",pch2);
                if(strcmp(pch2,"all")==0)
                {
                    printf("->Printing all customers...\n");
                    HP_GetAllEntries(file_descr,pch2,NULL);
                }
                else
                {
                    pch = strtok (NULL, ",-\n");
                    if(pch==NULL)
                    {
                        printf("->No Value selected...\n");
                        break;
                    }
                    printf("->Value  :%s\n",pch);
                    HP_GetAllEntries(file_descr,pch2,pch);
                }
            }
            else
            {
                printf("->Wrong input from command line...\n");
                printf("->                  _________________\n");
                printf("->                 |PROGRAMM COMMANDS|\n");
                printf("->-------------------------------------------------------|\n");
                printf("->To load a file of records: load Filename               |\n");
                printf("->To insert a record       : insert Id Name Surname City |\n");
                printf("->To get info of records   : get field value             |\n");
                printf("->To see Database status   : status                      |\n");
                printf("->To exit programm press   : exit                        |\n");
                printf("->-------------------------------------------------------|\n");
                break;
            }
            pch = strtok (NULL," ");
         }
    }                                           //File close
    error=HP_CloseFile(file_descr);
    if(error<0)
    {
        printf("->Failure ,Exiting...\n");
        printf("|---------------------------->END OF PROGRAMM<--------------------------|\n");
        return 0;
    }
    printf("|---------------------------->END OF PROGRAMM<--------------------------|\n");
    return 0;
}
