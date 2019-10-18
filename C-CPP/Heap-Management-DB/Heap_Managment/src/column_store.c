#include "BF.h"
#include "heap.h"
#include "column_store.h"

///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////        FUNCTIONS OF COLUMN-STORE      ////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void CS_Init(){
    BF_Init();                //arxikopoihsh pediou block
    printf("->Initialation of CS and BF field\n");
    total_records = 0;        //misenismos records
}

int CS_CreateFiles(char **fieldNames, char *dbDirectory) {
    int error,error2,idfile,i;
    char buffer[1024];
    void *block;
    FILE * fp;
    
    strcpy(buffer,dbDirectory);     //prosorinos buffer gia to path
    strcat(buffer,"/Header_Info");  //to arxeio me ta info 
    fp = fopen (buffer, "w+");      //anoigma kai topothethsh stoixeiwn
    strcat(dbDirectory,"/CSFile_"); //arxiko onoma gia ola ta arxeia
    strcpy(buffer,dbDirectory);     //antigrafh
    for(i=0;i<NUM_REC_FIELD;i++){   //dhmioyrgia 6 arxeiwn
        strcat(buffer,fieldNames[i]);//antigrafh tou teleutaiou stoixeiou tou onomatos
        printf("->Creation of File \"%s\"\n",buffer);
        error = HP_CreateFile(buffer);//dhmiourgia heap file
        if(error == -1){            //elegxos lathous
            printf("->Error in CS_CreateFiles function\n");
            return -1;
        }                           //grapsimo sto info file
        printf("->  *Creation of File, success\n");
        fprintf(fp,"CSFile_%s\n",fieldNames[i]);
        //fprintf(fp,"%d\n",error);
        strcpy(buffer,dbDirectory); //anadiorganonh buffer
    }    
    fclose(fp);                      //kleisimo arxeiou
    return 0;                       //ola ok 
}

int CS_OpenFile(HeaderInfo* header_info, char *dbDirectory) {
  // Add your code here
    int i = 0,error;
    char * line = NULL,buffer[1024],buffer2[1024],oneword[100],c;
    FILE *fd_info_file;
    size_t len = 0;
    ssize_t read;
    
    strcpy(buffer,dbDirectory);   //prosorinos buffer gia to path
    strcat(buffer,"Header_Info"); //to arxeio me ta info 
    strcpy(buffer2,buffer);       //prosorinos buffer gia to path gia meta
    fd_info_file = fopen (buffer, "r+");//anoigma kai topothethsh stoixeiwn
    if (fd_info_file == NULL){    //elegxos
        printf("->Error in CS_OpenFile function %s\n",buffer);
        return -1;                //epistrofh sthn main logo sfalmatos
    }                             //diabasma arxeiou line by line
    while ((read = getline(&line, &len, fd_info_file)) != -1) {
        header_info->column_fds[i].columnName = malloc(strlen(line-1)*sizeof(char));//desmeush
        strcpy(header_info->column_fds[i].columnName,line);     //totpothetish
        header_info->column_fds[i].columnName[strlen(line)-1] = '\0';
        strcpy(buffer,dbDirectory); //prosorinos buffer gia to path
        strcat(buffer,header_info->column_fds[i].columnName);//to arxeio me ta record 
        error = HP_OpenFile(buffer);//anoigma arxeiou
        if(error==-1){              //elegxos
            printf("->Error due opening file \n");
            return -1;
        }                           //eisagwgh sthn domh
        header_info->column_fds[i].fd = error;
        i++;                        //epomeno
    }
    fclose(fd_info_file);           //kleisimo
    fd_info_file = fopen (buffer2, "w+");//anoigma kai topothethsh stoixeiwn
    if (fd_info_file == NULL){      //elegxos
        printf("->Error in CS_OpenFile function %s\n",buffer2);
        return -1;                  //epistrofh sthn main logo sfalmatos
    }
    for(i=0;i<NUM_REC_FIELD;i++){   //grapsimo sto arxeio
        fprintf(fd_info_file,"%s %d\n",header_info->column_fds[i].columnName,header_info->column_fds[i].fd); 
    }
    fclose(fd_info_file);           //kleisimo
    return 0;
}

int CS_CloseFile(HeaderInfo* header_info) {
  // Add your code here
    int i,error,fileDesc;
    char name[100];
    
    for(i=0;i<NUM_REC_FIELD;i++){           //sarwse thn domh anoiktwn
        fileDesc = header_info->column_fds[i].fd;//arxeion gia kleisimo
        strcpy(name,header_info->column_fds[i].columnName);
        printf("->Closing the Column-store file\n->   *File :%s\n->   *Filedescriptor :%d\n",name,fileDesc);
        error = HP_CloseFile(fileDesc);     //kleise to arxeio
        if(error == -1){                    //elegxos sfalmatos
            printf("->Error in CS_CloseFile function\n");
            return -1;                      //sfalma
        }
    }
    return 0;                               //ola ok 
}

int CS_InsertEntry(HeaderInfo* header_info, Record record) {
  // Add your code here
    int error,fileDesc,i=0;
    char element[50];
    fileDesc = header_info->column_fds[0].fd;//metatroph se int
    sprintf(element, "%d", record.id);

    error = HP_InsertEntry(fileDesc,element,0,sizeof(int));//eisagwgh id
    if(error==-1){                                         //lathos
        printf("->Error in CS_InsertEntry function\n");
        return -1;
    }
    
    fileDesc = header_info->column_fds[1].fd;               //eisagwgh name
    error = HP_InsertEntry(fileDesc,record.name,1,sizeof(record.name));
    if(error==-1){                                          //lathos
        printf("->Error in CS_InsertEntry function\n");
        return -1;
    }
    
    fileDesc = header_info->column_fds[2].fd;               //eisagwgh surname
    error = HP_InsertEntry(fileDesc,record.surname,1,sizeof(record.surname));
    if(error==-1){                                          //lathos
        printf("->Error in CS_InsertEntry function\n");
        return -1;
    }
    
    fileDesc = header_info->column_fds[3].fd;               //eisagwgh status
    error = HP_InsertEntry(fileDesc,record.status,1,sizeof(record.status));
    if(error==-1){                                          //lathos
        printf("->Error in CS_InsertEntry function\n");
        return -1;
    }
    
    fileDesc = header_info->column_fds[4].fd;               //eisagwgh date
    error = HP_InsertEntry(fileDesc,record.dateOfBirth,1,sizeof(record.dateOfBirth));
    if(error==-1){                                          //lathos
        printf("->Error in CS_InsertEntry function\n");
        return -1;
    }
    
    sprintf(element, "%d", record.salary);                  //metatroph int
    fileDesc = header_info->column_fds[5].fd;
    error = HP_InsertEntry(fileDesc,element,0,sizeof(int)); //eisagwgh salary
    if(error==-1){                                          //lathos
        printf("->Error in CS_InsertEntry function\n");
        return -1;
    }
    total_records++;                                        //allo ena record mpike
    return 0;
} 


void CS_GetAllEntries(HeaderInfo* header_info, 
                      char *fieldName, 
                      void *value,
                      char **fieldNames,
                      int n) {
  // Add your code here
    Record record_entry;
    char *value_pointer,cvalue[1024];
    void *block;
    int date_counter,date_block,status_counter,status_block,salary_counter,salary_block;
    int id_counter,id_block,name_counter,name_block,surname_counter,surname_block;
    int int_value,records = 0,check=0,counter;
    int i,j,curr_rec=0,num_block,rec_in_block,fileDesc,fileDesc2,check_error,total_rec=0,id;
    if(fieldName==NULL){                        //ektupwsh olwn twn eggrafwn
        printf("->Printing all customers...\n");
        strcpy(fieldNames[0],"id");             //arxikopoihsh tou pinaka 
        strcpy(fieldNames[1],"name");           //me ta pedia
        strcpy(fieldNames[2],"surname");
        strcpy(fieldNames[3],"status");
        strcpy(fieldNames[4],"dateofbirth");
        strcpy(fieldNames[5],"salary");
        id_counter = 0;                         //arxikopoihsh metritwn
        id_block = 1;                           //kai ariumwn block
        name_counter = 0;                       //gia kathe pedio
        name_block = 1;
        surname_counter = 0;
        surname_block = 1;
        date_counter = 0;
        date_block = 1;
        status_counter = 0;
        status_block = 1;
        salary_counter = 0;
        salary_block = 1;
        printf("-> ____________________________________________________________________________________________________\n");
        printf("->||  ID ||         NAME         ||        SURNAME       ||   STATUS   ||  DATE OF BIRTH  ||  SALARY  ||\n");
        printf("->||--------------------------------------------------------------------------------------------------||\n");
        for(i=0;i<total_records;i++){           //sarwse oles tis eggrafes
            for(j=0;j<NUM_REC_FIELD;j++){       //gia kathe pedio
                if(strcmp(fieldNames[j],"id")==0){  //id
                    rec_in_block = ((BLOCK_SIZE-4)/sizeof(int));//rec ana block
                    if(id_counter>=rec_in_block){               //an kseperasoume to block
                        id_block++;                             //epomeno
                        id_counter = 0;
                    }
                    check_error=BF_ReadBlock(header_info->column_fds[0].fd,id_block,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }                                   //pare thn timh
                    memcpy(&int_value,block+sizeof(int)+id_counter*sizeof(int),sizeof(int));
                    printf("->|| %3d |",int_value);     //ektupwsh
                    id_counter++;                   //aukshse metrith 
                    total_rec++;                    //auksise sunolikes egrafes
                
                }
                else if(strcmp(fieldNames[j],"name")==0){//omoia gia pedio name
                    rec_in_block = ((BLOCK_SIZE-4)/sizeof(record_entry.name));
                    if(name_counter>=rec_in_block){
                        name_block++;
                        name_counter = 0;
                    }
                    check_error=BF_ReadBlock(header_info->column_fds[1].fd,name_block,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(cvalue,block+sizeof(int)+name_counter*sizeof(record_entry.name),sizeof(record_entry.name));
                    printf("| %20s |",cvalue);
                    name_counter++;
                }
                else if(strcmp(fieldNames[j],"surname")==0){//omoia surname
                    rec_in_block = ((BLOCK_SIZE-4)/sizeof(record_entry.surname)); 
                    if(surname_counter>=rec_in_block){
                        surname_block++;
                        surname_counter = 0;
                    }
                    check_error=BF_ReadBlock(header_info->column_fds[2].fd,surname_block,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(cvalue,block+sizeof(int)+surname_counter*sizeof(record_entry.surname),sizeof(record_entry.surname));
                    printf("| %20s |",cvalue);
                    surname_counter++;
                }
                else if(strcmp(fieldNames[j],"status")==0){//omoia status
                    rec_in_block = ((BLOCK_SIZE-4)/sizeof(record_entry.status));
                    if(status_counter>=rec_in_block){
                        status_block++;
                        status_counter = 0;
                    }
                    check_error=BF_ReadBlock(header_info->column_fds[3].fd,status_block,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(cvalue,block+sizeof(int)+status_counter*sizeof(record_entry.status),sizeof(record_entry.status));
                    printf("| %10s |",cvalue);
                    status_counter++;
                }
                else if(strcmp(fieldNames[j],"dateofbirth")==0){//omoia date
                    rec_in_block = ((BLOCK_SIZE-4)/sizeof(record_entry.dateOfBirth));
                    if(date_counter>=rec_in_block){
                        date_block++;
                        date_counter = 0;
                    }
                    check_error=BF_ReadBlock(header_info->column_fds[4].fd,date_block,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(cvalue,block+sizeof(int)+date_counter*sizeof(record_entry.dateOfBirth),sizeof(record_entry.dateOfBirth));
                    printf("| %15s |",cvalue);
                    date_counter++;
                }
                else{                                           //omoia salary
                    rec_in_block = ((BLOCK_SIZE-4)/sizeof(int));
                    if(salary_counter>=rec_in_block){
                        salary_block++;
                        salary_counter = 0;
                    }
                    check_error=BF_ReadBlock(header_info->column_fds[5].fd,salary_block,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(&int_value,block+sizeof(int)+salary_counter*sizeof(int),sizeof(int));
                    printf("| %8d ||\n",int_value);
                    salary_counter++;

                }
            }
        }
        printf("->||--------------------------------------------------------------------------------------------------||\n");
    }
    else{           //entolh get 
        value_pointer = (char*)value;   //ektupwsh
        printf("->Field of search : [%s]\n",fieldName);
        printf("->Value of search : [%s]\n",value_pointer);
        printf("->Field for print : \n");
        for(i=0;i<n;i++){               //ektupwsh pediwn
            printf("->         * [%s]\n",fieldNames[i]);
        }
        for(i=0;i<n;i++){           //elegxos gia orthi topothetish pediwn
            check = check + CS_Check_Fields(fieldNames[i]);
        }
        if(check!=(n)){             //sfalma
            printf("->Error in field input,does not exist...\n");
        }
        else{
            if(strcmp(fieldName,"id")==0){  //pedio id
                for(i=1;i<BF_GetBlockCounter(header_info->column_fds[0].fd);i++){//sarwsh block
                    check_error=BF_ReadBlock(header_info->column_fds[0].fd,i,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(&counter,block,sizeof(int));     //metrhth eggrafwn
                    for(j=0;j<counter;j++){                 //sarwsh eggrafwn
                        memcpy(&int_value,block+sizeof(int)+j*sizeof(int),sizeof(int));//labe timh
                        if(int_value == atoi(value_pointer)){       //elenkse isothta
                            total_rec ++;                       //auksise metrith
                            printf("->[%3d] Costumer with id \"%d\" ",total_rec,int_value);
                            CS_Print_Fields(header_info,curr_rec,fieldNames,n);//bres timh gia ta dotheda pedia
                            printf(", founded...\n");
                        }
                        curr_rec++;
                    }
                }
            }
            else if(strcmp(fieldName,"name")==0){           //omoia name
                for(i=1;i<BF_GetBlockCounter(header_info->column_fds[1].fd);i++){
                    check_error=BF_ReadBlock(header_info->column_fds[1].fd,i,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(&counter,block,sizeof(int));
                    for(j=0;j<counter;j++){
                        memcpy(cvalue,block+sizeof(int)+j*sizeof(record_entry.name),sizeof(record_entry.name));
                        if(strcmp(cvalue,value_pointer)==0){
                            total_rec ++;
                            printf("->[%3d] Costumer with Name \"%s\" ",total_rec,cvalue);
                            CS_Print_Fields(header_info,curr_rec,fieldNames,n);
                            printf(", founded...\n");
                        }
                        curr_rec++;
                    }
                }
            }
            else if(strcmp(fieldName,"surname")==0){        //omoia surname
                for(i=1;i<BF_GetBlockCounter(header_info->column_fds[2].fd);i++){
                    check_error=BF_ReadBlock(header_info->column_fds[2].fd,i,&block);  //diabasma tou block 
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(&counter,block,sizeof(int));
                    for(j=0;j<counter;j++){
                        memcpy(cvalue,block+sizeof(int)+j*sizeof(record_entry.surname),sizeof(record_entry.surname));
                        if(strcmp(cvalue,value_pointer)==0){
                            total_rec ++;
                            printf("->[%3d] Costumer with Surname \"%s\" ",total_rec,cvalue);
                            CS_Print_Fields(header_info,curr_rec,fieldNames,n);
                            printf(", founded...\n");
                        }
                        curr_rec++;
                    }
                }
            }
            else if(strcmp(fieldName,"status")==0){         //omoia status
                for(i=1;i<BF_GetBlockCounter(header_info->column_fds[3].fd);i++){
                    check_error=BF_ReadBlock(header_info->column_fds[3].fd,i,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(&counter,block,sizeof(int));
                    for(j=0;j<counter;j++){
                        memcpy(cvalue,block+sizeof(int)+j*sizeof(record_entry.status),sizeof(record_entry.status));
                        if(strcmp(cvalue,value_pointer)==0){
                            total_rec ++;
                            printf("->[%3d] Costumer with Status \"%s\" ",total_rec,cvalue);
                            CS_Print_Fields(header_info,curr_rec,fieldNames,n);
                            printf(", founded...\n");
                        }
                        curr_rec++;
                    }
                }
            }
            else if(strcmp(fieldName,"dateofbirth")==0){     //omoia date
                for(i=1;i<BF_GetBlockCounter(header_info->column_fds[4].fd);i++){
                    check_error=BF_ReadBlock(header_info->column_fds[4].fd,i,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(&counter,block,sizeof(int));
                    for(j=0;j<counter;j++){
                        memcpy(cvalue,block+sizeof(int)+j*sizeof(record_entry.dateOfBirth),sizeof(record_entry.dateOfBirth));
                        if(strcmp(cvalue,value_pointer)==0){
                            total_rec ++;
                            printf("->[%3d] Costumer with Date of birth \"%s\" ",total_rec,cvalue);
                            CS_Print_Fields(header_info,curr_rec,fieldNames,n);
                            printf(", founded...\n");
                        }
                        curr_rec++;
                    }
                }
            }
            else if(strcmp(fieldName,"salary")==0){     //omoia salary
                for(i=1;i<BF_GetBlockCounter(header_info->column_fds[5].fd);i++){
                    check_error=BF_ReadBlock(header_info->column_fds[5].fd,i,&block);  //diabasma tou block [0]
                    if(check_error<0)                //Elegxos gia sfalma
                    {
                        printf("->Error Reading new block\n");
                    }
                    memcpy(&counter,block,sizeof(int));
                    for(j=0;j<counter;j++){
                        memcpy(&int_value,block+sizeof(int)+j*sizeof(int),sizeof(int));
                        if(int_value == atoi(value_pointer)){
                            total_rec ++;
                            printf("->[%3d] Costumer with Salary \"%d\" ",total_rec,int_value);
                            CS_Print_Fields(header_info,curr_rec,fieldNames,n);
                            printf(", founded...\n");
                        }
                        curr_rec++;
                    }
                }
            }
            else {              //sfalmenh eisodos
                printf("->This field does not exist...\n");
            }
        }
    }
    if(total_rec==0){           //den brethike
        printf("->No costumers founded...\n");
    }
    else{                       //brethike
        printf("->Costumers found : [%d] costumers\n",total_rec);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////        EXTRA FUNCTIONS OF COLUMN-STORE      ////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

/*******************ektupwsh bash*********************/
void CS_PrintDataBase(char *file,HeaderInfo* header_info,Lengths* len){
    int i,fileDesc,nu_block,j,k,counter,check_error,intrec,a=0;
    void *block;
    long unsigned int length;
    char value[1024];
    for(i=0;i<NUM_REC_FIELD;i++){
        if(strcmp(file,header_info->column_fds[i].columnName)==0){
            fileDesc = header_info->column_fds[i].fd;//pare fd
            length = len->length[i];                  //pare megethos
            nu_block = BF_GetBlockCounter(fileDesc)-1;
            
            for(j=1;j<=nu_block;j++){
                check_error=BF_ReadBlock(fileDesc,j,&block);//diabase to block
                if(check_error<0)                              //elenxos sfalmatwn
                {
                    BF_PrintError("Error Reading block\n");
                }
                memcpy(&counter,block,sizeof(int)); //pare metrith block
                printf("-> ---------Block number [%d],Total Recs [%d]----------  \n",j,counter);
                for(k=0;k<counter;k++){         //ektupwse eggrafes
                    if(length == 4){            //gia int
                        memcpy(&intrec,block+sizeof(int)+k*length,length);
                        printf("->[%3d] %d\n",a+1,intrec);
                        
                    }
                    else{               //alliws analoga megethos
                        memcpy(value,block+sizeof(int)+k*length,length);
                        printf("->[%3d] %s\n",a+1,value);
                    }
                    a++;
                }
            }
        }
    }
    if(a==0){       //sfalma
        printf("->This file does not exist...\n");
    }
    else{           //ola ok ektupwse 
        printf("->Database   :%s\n",file);
        printf("->Filedesc   :%d\n",fileDesc);
        printf("->Length     :%lu\n",length);
        printf("->Blocks     :%d\n",nu_block+1);
        printf("->Total recs :%d\n",a);
    }
}

/***************ektupwsh stoixeiwn arxeiwn***************/
void CS_PrintStatus(HeaderInfo* header,char *pwd){
    int i;
    printf("->                   ______________\n");
    printf("->                  |    STATUS   |\n");    //ektupwsh stoixeiwn
    printf("->-------------------------------------------------------|\n");
    printf("->[1] Total numbers of files      :%d files\n",NUM_REC_FIELD);
    printf("->[2] Total numbers of files info :1 file\n");
    printf("->[3] Total records               :%d Records\n",total_records);
    printf("->[4] Path name                   :%s \n",pwd);
    printf("->[5] Databases name files        :\n");
    for(i=0;i<NUM_REC_FIELD;i++){
        printf("->      *%s ",header->column_fds[i].columnName);
        printf(" %d\n",header->column_fds[i].fd);
    }
    printf("->-------------------------------------------------------|\n");
}

/***************ektupwsh stoixeiwn bashs***************/
void CS_DataStatus(char* file,HeaderInfo* header,Lengths* len){
    int i ,check =0,fileDesc;
    for(i=0;i<NUM_REC_FIELD;i++){//gia kathe bash 
        if(strcmp(file,header->column_fds[i].columnName)==0){
            fileDesc = header->column_fds[i].fd;
            check = 1;              //ektupwse bash
            HP_PrintDatabase_Status(fileDesc,len->length[i]);
            break;
        }
    }
    if(check==0){                   //sfalma
        printf("->This file does not exist...\n");
    }
}
/************elengos gia thn eisodo tis get***********/
int CS_Check_Fields(char* fieldNames){
    int check = 0;
    if(strcmp(fieldNames,"id")==0){     //an id
        check++;
    }
    if(strcmp(fieldNames,"name")==0){       //an name
        check++;
    }
    if(strcmp(fieldNames,"surname")==0){    //an surname
        check++;
    }
    if(strcmp(fieldNames,"status")==0){     //an status
        check++;
    }
    if(strcmp(fieldNames,"dateofbirth")==0){//an date
        check++;
    }
    if(strcmp(fieldNames,"salary")==0){     //an salary
        check++;
    }
    return check;                           //sunoliko athroisma epilogwn
}

/***************euresh thesews timwn gia ta pedia***************/
void CS_Print_Fields(HeaderInfo* header_info,int rec_possition,char** fields,int n){
    int i,check_error,block_num,posittion,int_value,total_rec;
    char c_value[1024];
    void *block;
    Record rec;

    for(i=0;i<n;i++){
        if(strcmp(fields[i],"id")==0){          //an id
            total_rec = ((BLOCK_SIZE-4)/sizeof(int)); //bres eggrafes ana block
            block_num = rec_possition/total_rec+1;      //bres block 
            check_error=BF_ReadBlock(header_info->column_fds[0].fd,block_num,&block);  //diabasma tou block 
            if(check_error<0)                //Elegxos gia sfalma
            {
                printf("->Error Reading new block\n");
            }                                   //bres thesh
            posittion = rec_possition - (block_num-1)*((BLOCK_SIZE-4)/sizeof(int));
            memcpy(&int_value,block + sizeof(int) + posittion*sizeof(int),sizeof(int)); //pare timh
            printf(",Id \"%3d\"",int_value);        //ektupwse
        }
        if(strcmp(fields[i],"name")==0){                //omoia gia name
            total_rec = ((BLOCK_SIZE-4)/sizeof(rec.name)) ;
            block_num = rec_possition/total_rec+1;
            check_error=BF_ReadBlock(header_info->column_fds[1].fd,block_num,&block);  //diabasma tou block 
            if(check_error<0)                //Elegxos gia sfalma
            {
                printf("->Error Reading new block\n");
            }
            posittion = rec_possition - (block_num-1)*(total_rec);
            memcpy(c_value,block + sizeof(int) + posittion*sizeof(rec.name),sizeof(rec.name)); 
            printf(",Name \"%15s\"",c_value);
        }
        if(strcmp(fields[i],"surname")==0){     //omoia gia surname
            total_rec = ((BLOCK_SIZE-4)/sizeof(rec.surname)) ;
            block_num = rec_possition/total_rec+1;
            check_error=BF_ReadBlock(header_info->column_fds[2].fd,block_num,&block);  //diabasma tou block 
            if(check_error<0)                //Elegxos gia sfalma
            {
                printf("->Error Reading new block\n");
            }
            posittion = rec_possition - (block_num-1)*(total_rec);
            memcpy(c_value,block + sizeof(int) + posittion*sizeof(rec.surname),sizeof(rec.surname)); 
            printf(",Surname \"%15s\"",c_value);
        }
        if(strcmp(fields[i],"status")==0){      //omoia gia status
            total_rec = ((BLOCK_SIZE-4)/sizeof(rec.status)) ;
            block_num = rec_possition/total_rec+1;
            check_error=BF_ReadBlock(header_info->column_fds[3].fd,block_num,&block);  //diabasma tou block 
            if(check_error<0)                //Elegxos gia sfalma
            {
                printf("->Error Reading new block\n");
            }
            posittion = rec_possition - (block_num-1)*(total_rec);          
            memcpy(c_value,block + sizeof(int) + posittion*sizeof(rec.status),sizeof(rec.status)); 
            printf(",Status \"%2s\"",c_value);
        }
        if(strcmp(fields[i],"dateofbirth")==0){     //omoia gia date
            total_rec = ((BLOCK_SIZE-4)/sizeof(rec.dateOfBirth)) ;
            block_num = rec_possition/total_rec+1;
            check_error=BF_ReadBlock(header_info->column_fds[4].fd,block_num,&block);  //diabasma tou block 
            if(check_error<0)                //Elegxos gia sfalma
            {
                printf("->Error Reading new block\n");
            }
            posittion = rec_possition - (block_num-1)*(total_rec);
            memcpy(c_value,block + sizeof(int) + posittion*sizeof(rec.dateOfBirth),sizeof(rec.dateOfBirth)); 
            printf(",Date \"%10s\"",c_value);
        }
        if(strcmp(fields[i],"salary")==0){          //omoia gia salary
            total_rec = ((BLOCK_SIZE-4)/sizeof(int)) ;
            block_num = rec_possition/total_rec+1;
            check_error=BF_ReadBlock(header_info->column_fds[5].fd,block_num,&block);  //diabasma tou block 
            if(check_error<0)                //Elegxos gia sfalma
            {
                printf("->Error Reading new block\n");
            }
            posittion = rec_possition - (block_num-1)*(total_rec);           
            memcpy(&int_value,block + sizeof(int) + posittion*sizeof(int),sizeof(int)); 
            printf(",Salary \"%5d\"",int_value);
        }
    }
}
