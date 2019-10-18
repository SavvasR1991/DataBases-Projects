#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "column_store.h"

int main(int argc, char** argv) {
	//Add your test code here
	int i,j,file_descr,blkCnt,n,counter;                  //metritis
	int error=0,read,fpb,total_records = 0,end_of_prog=0,overflow=0;                             //flag uperxilishs
	char filename [50],file_in[50],input[50];   //stdin user
	char *pch,*pch1,*pch2,*line = NULL,**record_name_array,cwd[1024];
    void* block; 
    struct stat st = {0};
                               //block
    size_t len = 0;
	Record record_entry;                        //eggrafh
	FILE *insert_file;                          //arxeio
    HeaderInfo Headerinfo;
    Lengths lengthinfo;
    CS_Init();


    Headerinfo.column_fds = malloc((NUM_REC_FIELD)*sizeof(ColumnFds));
    lengthinfo.length = malloc((NUM_REC_FIELD)*sizeof(long unsigned));
    record_name_array = malloc(NUM_REC_FIELD*sizeof(char*));
    for(i=0;i<NUM_REC_FIELD;i++){
        *(record_name_array+i) = malloc(MAX_CHAR*sizeof(char));
        if(*(record_name_array+i)==NULL){
            printf("->Error allocating memory with malloc \n");
            printf("->Programm terminates...\n");
            return 0;
        }
    }
    getcwd(cwd, sizeof(cwd));                           //entolh gia to monopati
    strcat(cwd,"/File Directory");                      //onoma tou fakelou

    if (stat(cwd, &st) == -1) {                         //dhmiourgia fakelou
        mkdir(cwd ,0700);
    }
    strcpy(record_name_array[0], "id");                 //gemisma tou pinaka ,1 id
    lengthinfo.length[0] = sizeof(record_entry.id);     //1 megethos id
    
    strcpy(record_name_array[1], "name");               //2 name
    lengthinfo.length[1] = sizeof(record_entry.name);   //2 megethos name
    
    strcpy(record_name_array[2], "surname");            //3 surname
    lengthinfo.length[2] = sizeof(record_entry.surname);//3 megethos surname
    
    strcpy(record_name_array[3], "status");             //4 status
    lengthinfo.length[3] = sizeof(record_entry.status); //4 megethos status
    
    strcpy(record_name_array[4], "dateOfBirth");        //5 date
    lengthinfo.length[4] = sizeof(record_entry.dateOfBirth);//5 megethos
    
    strcpy(record_name_array[5], "salary");             //6 salary
    lengthinfo.length[5] = sizeof(record_entry.salary); //6 megethos
    
    error = CS_CreateFiles(record_name_array,cwd);      //h dhmiourgia ton arxeiwn sto fakelo
    if(error == -1)                                     //periptwsh lathous
    {
        printf("->Error due creation of databases...\n");
        printf("->Programm terminates...\n");
        return 0;
    }
    getcwd(cwd, sizeof(cwd));                           //lhpsh ksana monopatiou
    strcat(cwd,"/File Directory/");
    error = CS_OpenFile(&Headerinfo,cwd);               //anoigma ton arxeiwn
    if(error == -1)                                     //periptwsh lathous
    {
        printf("->Error due opening the databases...\n");
        printf("->Programm terminates...\n");
        return 0;
    }
    printf("->--------------------INFORMATION------------------------|\n");
    for(i=0;i<NUM_REC_FIELD;i++){
        printf("->  [%d] *: File Name - \"%s\"\n",i+1,Headerinfo.column_fds[i].columnName);
        printf("->      *: File FD   - {%d}\n",Headerinfo.column_fds[i].fd);
    }
    
    printf("->-------------------------------------------------------|\n");

    printf("\n|------------------------->COLUMN STORE FILE PROGRAMM<--------------------------|\n");
    printf("->|*Current storing file directory: %s *|\n", cwd);
    printf("->                   _________________\n");
    printf("->                  |PROGRAMM COMMANDS|\n");
    printf("->-------------------------------------------------------|\n");
    printf("->To load a file of records: [load] Filename             |\n");
    printf("->To insert a record       : [insert] Id Name Surname    |\n");
    printf("->                         : Status DateOfBirth Salary   |\n");
    printf("->To get info of records   : [get] field value           |\n");
    printf("->To see file status       : [status]                    |\n");
    printf("->To see Database records  : [print] filename            |\n");
    printf("->To see Database status   : [datastatus] filename       |\n");
    printf("->To exit programm press   : [exit]                      |\n");
    printf("->-------------------------------------------------------|\n");
    while(1)                                    //bronxos eisagwghs entolwn user
    {
        if(end_of_prog==1)                      //telos programmatos
        {
            break;
        }
        printf("->Insert your command...\n->");
        fgets(input,50,stdin);                  //stdin command
        pch = strtok (input," ,.-\n");          //analuse tis entoles
        while (pch != NULL)
        {                                       //entolh load
            if(strcmp(pch,"load")==0)
            {
                pch = strtok (NULL,"\n");
                if(pch==NULL)
                {
                    printf("->Error No file selected...\n");
                    break;
                }
                printf("->Loading file : \"%s\"\n",pch);
                insert_file=fopen(pch,"r");     //onoma arxeiou
                if (insert_file == NULL)
                {
                    printf("->Error cannot open file :\"%s\"\n",pch);
                    break;
                }                               //diabase kathe eggrafh apo arxeio
                counter=0;
                while ((read = getline(&line, &len, insert_file)) != -1)
                {
                    counter ++;
                    pch1 = strtok (line, " \",-\n");
                    record_entry.id=atoi(pch1);
                    pch1 = strtok (NULL, " \",-\n");
                    strcpy(record_entry.name,pch1);
                    pch1 = strtok (NULL, " \",-\n");
                    strcpy(record_entry.surname,pch1);
                    pch1 = strtok (NULL, "\",-\n");
                    strcpy(record_entry.status,pch1);
                    pch1 = strtok (NULL, "\",\n");
                    strcpy(record_entry.dateOfBirth,pch1);
                    pch1 = strtok (NULL, "\",\n");
                    record_entry.salary=atoi(pch1);
                    pch1 = strtok (NULL, " \"\n");
                    printf("->[%3d] Insert %3d %15s %15s %3s %10s %5d...\n",counter,record_entry.id,record_entry.name,record_entry.surname,record_entry.status,record_entry.dateOfBirth,record_entry.salary);
                    error=CS_InsertEntry(&Headerinfo,record_entry);
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
                fclose(insert_file);            //kleisimo arxeiou
                break;
            }                                   //entolh insert
            else if(strcmp(input,"insert")==0&&overflow==0)
            {
                printf("->Inserting new record...\n");
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Id Name Surname Status Date Salary selected...\n");
                    break;
                }
                printf("->Id        :%s\n",pch);
                record_entry.id=atoi(pch);
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Name Surname Status Date Salary selected...\n");
                    break;
                }
                printf("->Name      :%s\n",pch);
                strcpy(record_entry.name,pch);
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Surname Status Date Salary selected...\n");
                    break;
                }
                printf("->Surname   :%s\n",pch);
                strcpy(record_entry.surname,pch);
                pch = strtok (NULL," \n");
                if(pch==NULL)
                {
                    printf("->No Status Date Salary selected...\n");
                    break;
                }
                printf("->Status    :%s\n",pch);
                strcpy(record_entry.status,pch);
                pch = strtok (NULL," \n");
                if(pch==NULL)
                {
                    printf("->No Date Salary selected...\n");
                    break;
                }
                printf("->Date Birth:%s\n",pch);
                strcpy(record_entry.dateOfBirth,pch);
                pch = strtok (NULL," \n");
                if(pch==NULL)
                {
                    printf("->No Salary selected...\n");
                    break;
                }
                printf("->Salary    :%s\n",pch);
                record_entry.salary=atoi(pch);
                error=CS_InsertEntry(&Headerinfo,record_entry);
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
                break;
            }                                   //entolh exit
            else if(strcmp(input,"exit")==0)
            {
                end_of_prog=1;
                break;
            }                                   //entolh status
            else if(strcmp(input,"status")==0)
            {
                CS_PrintStatus(&Headerinfo,cwd);
            }                                   //entolh get
            else if(strcmp(input,"get")==0)
            {
                n = 0;
                printf("->Finding Customers...\n");
                pch1 = strtok (NULL," \n");
                if(pch1==NULL)
                {
                    printf("->No field selected...\n");
                    break;
                }
                if(strcmp(pch1,"all")==0){
                    CS_GetAllEntries(&Headerinfo,NULL,NULL,record_name_array,n);
                    break;
                }
                pch2 = strtok (NULL," \n");
                if(pch2==NULL)
                {
                    printf("->No value selected...\n");
                    break;
                }
                while (pch != NULL)
                {
                    pch = strtok (NULL," \n");
                    if(pch==NULL)
                    {
                        break;
                    }
                    if(n<6){
                        strcpy(record_name_array[n],pch);
                    }
                    n++;
                }
                if(n<1||n>6){
                    printf("->Wrong input for get command...\n");
                }
                else{
                    CS_GetAllEntries(&Headerinfo,pch1,pch2,record_name_array,n); 
                }                
            }           //entolh print
            else if(strcmp(input,"print")==0){
                pch = strtok (NULL, ",-\n");
                if(pch==NULL)
                {
                    printf("->No file selected...\n");
                    break;
                }
                CS_PrintDataBase(pch,&Headerinfo,&lengthinfo);
                break;
            }
            else if(strcmp(input,"datastatus")==0){ //entolh datastatus
                pch = strtok (NULL, ",-\n");
                if(pch==NULL)
                {
                    printf("->No file selected...\n");
                    break;
                }
                CS_DataStatus(pch,&Headerinfo,&lengthinfo);
                break;
            }
            else            //lathos entolh,epanelabe
            {
                printf("->Wrong input from command line...\n");
                printf("->                   _________________\n");
                printf("->                  |PROGRAMM COMMANDS|\n");
                printf("->-------------------------------------------------------|\n");
                printf("->To load a file of records: [load] Filename             |\n");
                printf("->To insert a record       : [insert] Id Name Surname    |\n");
                printf("->                         : Status DateOfBirth Salary   |\n");
                printf("->To get info of records   : [get] field value           |\n");
                printf("->To see files status      : [status]                    |\n");
                printf("->To see Database records  : [print] filename            |\n");
                printf("->To see Database status   : [datastatus] filename       |\n");
                printf("->To exit programm press   : [exit]                      |\n");
                printf("->-------------------------------------------------------|\n");
                break;
            }
            pch = strtok (NULL," ");
         }
    }  
    for(i=0;i<NUM_REC_FIELD;i++){//eleftherwsh xwrou
        free(*(record_name_array+i));
    }
    free(record_name_array);
    error = CS_CloseFile(&Headerinfo);//kleisimo arxeiou
    if(error<0)
    {
        printf("->Failure losing files,Exiting...\n");
        printf("|---------------------------->END OF PROGRAMM<--------------------------|\n");
        return 0;
    }
    printf("|---------------------------->END OF PROGRAMM<--------------------------|\n");
    return 0;
}
