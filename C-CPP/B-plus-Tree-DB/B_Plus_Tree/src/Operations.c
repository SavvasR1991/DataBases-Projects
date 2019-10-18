#include "Operations.h"
#include "AM.h"

/***************************************EXTRA FUNCTIONS************************************************************/
int AM_Check_File_Info_Capacity(int filedis,char *fileName){//eisagwgh timwn sto open pinaka
    int i, hp_file_check,length1,length2,res;
    char length_str[5];
    void *block;
    if(File_Information_Array[filedis].file_descriptor==-1){//apeuthias an einai adeio
        File_Information_Array[filedis].file_descriptor=filedis;
        File_Information_Array[filedis].filename = strdup(fileName);
        hp_file_check=BF_ReadBlock(filedis,0,&block);      //diabasma block[0]
        if(hp_file_check<0){                                     //elegxos sfalmatos
                printf("->Error Reading block...\n");
                return -1;}                 //eidagwgh timwn ston pinaka apo to block[0]
        memcpy(&length1,block+sizeof("B+_Tree")+2*sizeof("c"),sizeof(int));
        File_Information_Array[filedis].length_key1=length1;
        memcpy(&length2,block+sizeof("B+_Tree")+2*sizeof("c")+sizeof(int),sizeof(int));
        File_Information_Array[filedis].length_key2=length2;
        memcpy(&File_Information_Array[filedis].type_key1,block+sizeof("B+_Tree"),sizeof("c"));
        memcpy(&File_Information_Array[filedis].type_key2,block+sizeof("B+_Tree")+sizeof("c"),sizeof("c"));
        res = (BLOCK_SIZE-3*sizeof(int)-sizeof("Rec")-(length1+length2))/(length1+length2);
        File_Information_Array[filedis].rec_in_block =res;
        res = (BLOCK_SIZE-(6*sizeof(int)+sizeof("Ind"))-2*length1)/(length1+sizeof(int));
        File_Information_Array[filedis].index_in_block =res;
        File_Information_Array[filedis].index_pointers_in_block = res+1;
        return AME_OK;}
    for(i=0;i<MAX_FILES;i++){           //oxi apethias bres adeia thesh
        if(File_Information_Array[i].file_descriptor==-1){
            File_Information_Array[i].file_descriptor=filedis;
            File_Information_Array[i].filename = strdup(fileName);
            hp_file_check=BF_ReadBlock(filedis,0,&block);      //diabasma block[0]
            if(hp_file_check<0){
                    printf("->Error Reading block...\n");
                    return -1;}
            memcpy(&length1,block+sizeof("B+_Tree")+2*sizeof("c"),sizeof(int));
            File_Information_Array[i].length_key1=length1;
            memcpy(&length2,block+sizeof("B+_Tree")+2*sizeof("c")+sizeof(int),sizeof(int));
            File_Information_Array[i].length_key2=length2;
            memcpy(&File_Information_Array[i].type_key1,block+sizeof("B+_Tree"),sizeof("c"));
            memcpy(&File_Information_Array[i].type_key2,block+sizeof("B+_Tree")+sizeof("c"),sizeof("c"));
            res = (BLOCK_SIZE-3*sizeof(int)-sizeof("Rec"))/(length1+length2);
            File_Information_Array[i].rec_in_block =res;
            res = (BLOCK_SIZE-(4*sizeof(int)+sizeof("Ind")))/(length1+sizeof(int));
            File_Information_Array[i].index_in_block =res-1;
            File_Information_Array[i].index_pointers_in_block = res;
            return AME_OK;}
    }
    return -1;
};
void AM_File_Info_Status(){ //ektupwsh tou file pinaka
    int i;
    printf("->|-------------------FILE INFORMATION ARRAY--------------------|\n");
    for(i=0;i<MAX_FILES;i++){
        if(File_Information_Array[i].file_descriptor!=-1){
            printf("->[%2d] : >>--------------------------------------------------<<\n",i);
            printf("->     : >>Rec Types {[%c],[%c]} ,Rec number in block [%d]\n->     : >>Index number in block [%d],Total Records [%d],Total indexes pointers [%d]\n->     : >>Discriptor (%d),FileName \"%s\"\n->     : >>Root Block (%d),Total Blocks (%d)\n",
            File_Information_Array[i].type_key1,File_Information_Array[i].type_key2,File_Information_Array[i].rec_in_block,
            File_Information_Array[i].index_in_block,File_Information_Array[i].total_rec,File_Information_Array[i].index_pointers_in_block,
            File_Information_Array[i].file_descriptor,File_Information_Array[i].filename,File_Information_Array[i].root,File_Information_Array[i].block_num);
            printf("->     : >>--------------------------------------------------<<\n");
        }
        else{
            printf("->[%2d] : Empty\n",i);
        }
    }
    printf("->|-------------------------------------------------------------|\n");
}
void AM_File_Scan_Status(){     //ektupwsh tou scan pinaka
    int i;
    printf("->|-------------------SCAN INFORMATION ARRAY--------------------|\n");
    for(i=0;i<MAX_FILES;i++){
        if(File_Scan_Array[i].scan_descriptor!=-1){
            printf("->[%2d] : >>--------------------------------------------------<<\n",i);
            printf("->     : >>     : >>Scan Discriptor (%d),File Discriptor (%d)\n",
            File_Scan_Array[i].scan_descriptor,File_Scan_Array[i].file_descriptor);
            printf("->     : >>     : >>Block for scanning (%d)\n",
            File_Scan_Array[i].block);
            printf("->     : >>     : >>Operation (%d),Possition in block (%d)\n",
            File_Scan_Array[i].op,File_Scan_Array[i].curr_rec);
            printf("->     : >>     : >>Record possition A (%d),Record possition B (%d),Total scan records (%d)\n",
            File_Scan_Array[i].possition_rec_A,File_Scan_Array[i].possition_rec_B,File_Scan_Array[i].total_scan_rec);
            printf("->     : >>     : >>Type1 (%c),Type2 (%c),Type1 length (%d),Type2 length (%d)\n",
            File_Scan_Array[i].type1,File_Scan_Array[i].type2,File_Scan_Array[i].length1,File_Scan_Array[i].length2);
            printf("->     : >>     : >>Current scan blocks (%d)\n",
            File_Scan_Array[i].current_scan);
            printf("->     : >>--------------------------------------------------<<\n");
        }
        else{
            printf("->[%2d] : Empty\n",i);
        }
    }
    printf("->|-------------------------------------------------------------|\n");
}

int AM_Create_Index_Block(int fileDesc,void *value1,int length){//dhmiourgia index block
        int hp_file_check, one=1,i,null = -1,curr_num_blocks;
        void *block;char d[40];
        hp_file_check=BF_AllocateBlock(fileDesc);       //desmeush arxikou block (0)
        if(hp_file_check<0){                          //Sfalma desmeushs mnhmhs gia block
                return -1;                           //epistrofh sthn main logo sfalmatos
        }
        curr_num_blocks = BF_GetBlockCounter(fileDesc);
        hp_file_check=BF_ReadBlock(fileDesc,(curr_num_blocks-1),&block);  //diabasma tou block [0]
        if(hp_file_check<0) {
                return -1;                     //Epistrofh sthn main logo sfalmatos
        }
        memcpy(block,"Ind",sizeof("Ind"));//arxikopoihsh tou block index
        memcpy(block+index_counter,&one,sizeof(int));//me tis analoges times
        memcpy(block+index_parent,&null,sizeof(int));//arxikopoihshs
        memcpy(block+index_next,&null,sizeof(int));
        memcpy(block+index_metadata+(File_Information_Array[fileDesc].index_pointers_in_block)*sizeof(int),value1,length);
        for(i=0;i<File_Information_Array[fileDesc].index_pointers_in_block;i++){
            memcpy(block+index_metadata+(i)*sizeof(int),&null,sizeof(int));
        }
        hp_file_check=BF_WriteBlock(fileDesc,(curr_num_blocks-1));//Grapse to block[0] sthn mhnmh
        if(hp_file_check<0){
                AM_errno = 5;
                AM_PrintError("->Error Writting  block\n");
                return -1;                      //epistrofh sthn main logo sfalmatos
        }
        return AME_OK;
}

int AM_Create_Record_Block(int fileDesc,void* value1,void* value2,int length1,int length2){
        int hp_file_check, curr_num_blocks,one = 1,null=-1; //dhmiourgia tou record block
        void *block;
        hp_file_check=BF_AllocateBlock(fileDesc);       //desmeush arxikou block (0)
        if(hp_file_check<0){                                        //Sfalma desmeushs mnhmhs gia block
                printf("->Error Allocating new block\n");
                return -1;                                            //epistrofh sthn main logo sfalmatos
        }
        curr_num_blocks = BF_GetBlockCounter(fileDesc);
        curr_num_blocks--;
        hp_file_check=BF_ReadBlock(fileDesc,curr_num_blocks,&block);  //diabasma tou block [0]
        if(hp_file_check<0){
                return -1;                                            //Epistrofh sthn main logo sfalmatos
        }
        memcpy(block,"Rec",sizeof("Rec"));              //arxikopoihsh tou block record
        memcpy(block+record_counter,&one,sizeof(int));  //me tis analoges times
        memcpy(block+record_parent,&null,sizeof(int));  //arxikopoihshs
        memcpy(block+record_next,&null,sizeof(int));
        memcpy(block+record_metadata,value1,length1);
        memcpy(block+record_metadata+length1,value2,length2);
        hp_file_check=BF_WriteBlock(fileDesc,curr_num_blocks);   //Grapse to block[0] sthn mhnmh
        if(hp_file_check<0){
                AM_errno = 5;
                AM_PrintError("->Error Writting  block\n");
                return -1;                 //epistrofh sthn main logo sfalmatos
        }
        return AME_OK;
}

int AM_Compare_Values(void* value1,int j,char *type,int block_num,int fileDesc,int length){
    int a1,a2,hp_file_check,temp_meta,temp_length;  //sugkrish timwn 
    float b1,b2;
    char* c1, c2[40], check[3];
    void *block;
    hp_file_check=BF_ReadBlock(fileDesc,block_num,&block);  //diabasma tou block [0]
    if(hp_file_check<0){
        AM_errno = 5;
        AM_PrintError("->Error Reading  Index  block\n");
        return -1;                                            //Epistrofh sthn main logo sfalmatos
    }
    memcpy(check,block,sizeof("Ind"));  //diaxwrismos index record
    if(strcmp(check,"Ind")==0){          //index
        temp_meta=index_metadata+File_Information_Array[fileDesc].index_pointers_in_block*sizeof(int);
        temp_length = length;
    }
    else{                               //record
        temp_meta=record_metadata;
        temp_length = length + File_Information_Array[fileDesc].length_key2;
    }
    if(strcmp(type,"c")==0){        //an eimaste string
        c1=(char*)value1;
        memcpy(c2,block+temp_meta+j*(length),length);
        if(strcmp(c1,c2)>0){
            if(pro_printing==1){
                printf("[%s]<=[%s]\n",c1,c2);
                printf("->      ->>     *Going Left  of Key Value :\"%s\"\n",c2);
            }
            return 0;           //mikroterh timh
        }
        else     {
            if(pro_printing==1){
                printf("[%s]>[%s] \n",c1,c2);
                printf("->      ->>     *Going Right of Key Value :\"%s\"",c2);
            }
            return 1;   //mikroterh timh
        }
    }
    else if(strcmp(type,"i")==0){ //exoume int 
        a1=*((int *)value1);
        memcpy(&a2,block+temp_meta+j*(temp_length),length);
        if(a1>=a2){
            if(pro_printing==1){
                printf("[%d]>[%d] ,'i'\n",a1,a2);
                printf("->      ->>     *Going Right of Key Value :\"%d\"\n",a2);
            }
            return 1;   //megalitero
        }
        else{
            if(pro_printing==1){
                printf("[%d]<=[%d] ,'i'\n",a1,a2);
                printf("->      ->>     *Going Left  of Key Value :\"%d\"\n",a2);
            }
            return 0;   //mikrotero
        }
    }
    else{       //exoume float
        b1=*((float *)value1);
        memcpy(&b2,block+temp_meta+j*(length),length);
        if(b1>=b2){
            if(pro_printing==1)
            {
                printf("[%f]>[%f] ,'f'\n",b1,b2);
                printf("->      ->>     *Going Rigth of Key Value :\"%f\"\n",b2);
            }
            return 1;   //megalutero
        }
        else{
            if(pro_printing==1){
                printf("[%f]<=[%f] ,'f'\n",b1,b2);
                printf("->      ->>     *Going Left of Key Value :\"%f\"\n",b2);
            }
            return 0;   //mikrotero
        }
    }
}

int AM_Print_Info_Scan_Insert(int i,int op,void *value){    //elegxos gia thn open scan
    printf("->Opening a scan to file \"%s\"\n",File_Information_Array[i].filename);
    if(op == 1){    //periptwsh equal
        if(pro_printing==1){
            printf("->Operation  :EQUAL\n");
            printf("->           *Key field type :[%c]\n",File_Information_Array[i].type_key1);
            if(File_Information_Array[i].type_key1 == 'c'){
                printf("->           *Records EQUAL to [%s]\n",(char*)value);
            }
            else if(File_Information_Array[i].type_key1 == 'i'){
                printf("->           *Records EQUAL to [%d]\n",(*(int*)value));
            }
            else{
                printf("->           *Records EQUAL to [%f]\n",(*(float*)value));
            }
        }
        return AME_OK;
    }
    else if(op == 2){   //periptwsh not equal
        if(pro_printing==1){
            printf("->Operation  :NOT EQUAL\n");
            printf("->           *Key field type :[%c]\n",File_Information_Array[i].type_key1);
            if(File_Information_Array[i].type_key1 == 'c') {
                printf("->           *Records NOT EQUAL to [%s]\n",(char*)value);
            }
            else if(File_Information_Array[i].type_key1 == 'i') {
                printf("->           *Records NOT EQUAL to [%d]\n",(*(int*)value));
            }
            else {
                printf("->           *Records NOT EQUAL to [%f]\n",(*(float*)value));
            }
        }
        return AME_OK;
    }
    else if(op == 3){   //periptwsj less than
        if(pro_printing==1){
            printf("->Operation  :LESS THAN\n");
            printf("->           *Key field type :[%c]\n",File_Information_Array[i].type_key1);
            if(File_Information_Array[i].type_key1 == 'c') {
                printf("->           *Records LESS THAN [%s]\n",(char*)value);
            }
            else if(File_Information_Array[i].type_key1 == 'i'){
                printf("->           *Records LESS THAN [%d]\n",(*(int*)value));
            }
            else{
                printf("->           *Records LESS THAN [%f]\n",(*(float*)value));
            }
        }
        return AME_OK;
    }
    else if(op == 4){   //periptwsh greates than
        if(pro_printing==1){
            printf("->Operation  :GREATER THAN\n");
            printf("->           *Key field type :[%c]\n",File_Information_Array[i].type_key1);
            if(File_Information_Array[i].type_key1 == 'c')  {
                printf("->           *Records GREATER THAN [%s]\n",(char*)value);
            }
            else if(File_Information_Array[i].type_key1 == 'i'){
                printf("->           *Records GREATER THAN [%d]\n",(*(int*)value));
            }
            else {
                printf("->           *Records GREATER THAN [%f]\n",(*(float*)value));
            }
        }
        return AME_OK;
    }
    else if(op == 5){   //periptwsh less or equal
        if(pro_printing==1){
            printf("->Operation  :LESS THAN OR EQUAL\n");
            printf("->           *Key field type :[%c]\n",File_Information_Array[i].type_key1);
            if(File_Information_Array[i].type_key1 == 'c'){
                printf("->           *Records LESS THAN OR EQUAL [%s]\n",(char*)value);
            }
            else if(File_Information_Array[i].type_key1 == 'i'){
                printf("->           *Records LESS THAN OR EQUAL [%d]\n",(*(int*)value));
            }
            else{
                printf("->           *Records LESS THAN OR EQUAL [%f]\n",(*(float*)value));
            }
        }
        return AME_OK;
    }
    else if(op == 6){   //periptwsh greater or equal
        if(pro_printing==1)
        {
            printf("->Operation  :GREATER THAN OR EQUAL\n");
            printf("->           *Key field type :[%c]\n",File_Information_Array[i].type_key1);
            if(File_Information_Array[i].type_key1 == 'c') {
                printf("->           *Records GREATER THAN OR EQUAL [%s]\n",(char*)value);
            }
            else if(File_Information_Array[i].type_key1 == 'i'){
                printf("->           *Records GREATER THAN OR EQUAL [%d]\n",(*(int*)value));
            }
            else{
                printf("->           *Records GREATER THAN OR EQUAL [%f]\n",(*(float*)value));
            }
        }
        return AME_OK;
    }
    else{
        return -1;
    }
}


int AM_Sorting_Record(int fileDesc,int curr_block,int i,char* type1,int rec){
    int hp_file_check,j,k,a,a1,a2,d1,d2,length1,length2,temp;   //taksinomish tou record
    float b,b1,b2,tempf,f1,f2;
    char *c,*c1,*c2,*tempc,type2,*s1,*s2;
    void *block,*v1,*v2;
    length1 = File_Information_Array[i].length_key1;    //lhpsh dedomenwn
    length2 = File_Information_Array[i].length_key2;
    type2   = File_Information_Array[i].type_key2;
    hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
    if(hp_file_check<0)                                         //Elegxos gia sfalma
    {
        return -1;
    }
    if(strcmp(type1,"c")==0){ //typos 1 string
        if(type2=='c'){     //typos 2 string
            c = (char*)malloc(length1*sizeof(char));
            c2 = (char*)malloc(length1*sizeof(char));
            tempc = (char*)malloc(length1*sizeof(char));
            s1 = (char*)malloc(length1*sizeof(char));
            s2 = (char*)malloc(length1*sizeof(char));
            for (j = 0; j < rec; j++) {     //fusallida
                for (k = 0; k < (rec-j-1); k++) {
                    memcpy(c,block+record_metadata+k*(length1+length2),length1);
                    memcpy(c2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(s1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(s2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                    if(strcmp(c,c2)>0){
                        strcpy(tempc,c);
                        strcpy(c,c2);
                        strcpy(c2,tempc);
                        memcpy(block+record_metadata+k*(length1+length2),c,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),c2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,s2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,s1,length2);
                    }
                }
            }
            free(c);
            free(c2);
            free(tempc);
            free(s1);
            free(s2);
        }
        else if(type2=='i') {   //tupos 2 int
            c = (char*)malloc(length1*sizeof(char));
            c2 = (char*)malloc(length1*sizeof(char));
            tempc = (char*)malloc(length1*sizeof(char));
            for (j = 0; j < rec; j++){      //taksinomhsh
                for (k = 0; k < (rec-j-1); k++) {
                    memcpy(c,block+record_metadata+k*(length1+length2),length1);
                    memcpy(c2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(&a1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(&a2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                   
                    if(strcmp(c,c2)>0)
                    {                        
                        strcpy(tempc,c);
                        strcpy(c,c2);
                        strcpy(c2,tempc);
                        memcpy(block+record_metadata+k*(length1+length2),c,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),c2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,&a2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,&a1,length2);
                    }
                }
            }
            free(c);
            free(c2);
            free(tempc);
        }
        else {      //typos float
            c = (char*)malloc(length1*sizeof(char));
            c2 = (char*)malloc(length1*sizeof(char));
            tempc = (char*)malloc(length1*sizeof(char));
            for (j = 0; j < rec; j++) {     //taksinomhsh
                for (k = 0; k < (rec-j-1); k++) {
                    memcpy(c,block+record_metadata+k*(length1+length2),length1);
                    memcpy(c2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(&b1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(&b2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                    if(strcmp(c,c2)>0){
                        strcpy(tempc,c);
                        strcpy(c,c2);
                        strcpy(c2,tempc);
                        memcpy(block+record_metadata+k*(length1+length2),c,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),c2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,&b2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,&b1,length2);
                    }
                }
            }
            free(c);
            free(c2);
            free(tempc);
        }
    }       //OMOIA KAI GIA INT KAI FLOAT GIA TYPE 1 ,TYPE 2
    else if(strcmp(type1,"i")==0){
        if(type2=='c'){
            c1 = (char*)malloc(length2*sizeof(char));
            c2 = (char*)malloc(length2*sizeof(char));
            for (j = 0; j < rec; j++){
                for (k = 0; k < (rec-j-1); k++){
                    memcpy(&a,block+record_metadata+k*(length1+length2),length1);
                    memcpy(&a2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(c1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(c2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                    if (a > a2){
                        temp = a;
                        a = a2;
                        a2 = temp;
                        memcpy(block+record_metadata+k*(length1+length2),&a,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),&a2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,c2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,c1,length2);
                    }
                }
            }
        }
        else if(type2=='i'){
            for (j = 0; j < rec; j++){
                for (k = 0; k < (rec-j-1); k++){
                    memcpy(&a,block+record_metadata+k*(length1+length2),length1);
                    memcpy(&a2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(&d1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(&d2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                    if (a > a2){
                        temp = a;
                        a = a2;
                        a2 = temp;
                        memcpy(block+record_metadata+k*(length1+length2),&a,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),&a2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,&d2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,&d1,length2);
                    }
                }
            }
        }
        else{
            for (j = 0; j < rec; j++){
                for (k = 0; k < (rec-j-1); k++){
                    memcpy(&a,block+record_metadata+k*(length1+length2),length1);
                    memcpy(&a2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(&b1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(&b2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                    if (a > a2){
                        temp = a;
                        a = a2;
                        a2 = temp;
                        memcpy(block+record_metadata+k*(length1+length2),&a,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),&a2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,&b2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,&b1,length2);
                    }
                }
            }
        }
    }
    else{
        if(type2=='c'){
            c1 = (char*)malloc(length2*sizeof(char));
            c2 = (char*)malloc(length2*sizeof(char));
            for (j = 0; j < rec; j++){
                for (k = 0; k < (rec-j-1); k++){
                    memcpy(&b,block+record_metadata+k*(length1+length2),length1);
                    memcpy(&b2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(c1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(c2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                    if (b > b2){
                        tempf = b;
                        b = b2;
                        b2 = tempf;
                        memcpy(block+record_metadata+k*(length1+length2),&b,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),&b2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,c2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,c1,length2);
                    
                    }
                }
            }
            free(c1);
            free(c2);
        }
        else if(type2=='i'){
            for (j = 0; j < rec; j++){
                for (k = 0; k < (rec-j-1); k++){
                    memcpy(&b,block+record_metadata+k*(length1+length2),length1);
                    memcpy(&b2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(&d1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(&d2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                    if (b > b2){
                        tempf = b;
                        b = b2;
                        b2 = tempf;
                        memcpy(block+record_metadata+k*(length1+length2),&b,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),&b2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,&d2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,&d1,length2);
                    }
                }
            }
        }
        else{
            for (j = 0; j < rec; j++){
                for (k = 0; k < (rec-j-1); k++){
                    memcpy(&b,block+record_metadata+k*(length1+length2),length1);
                    memcpy(&b2,block+record_metadata+(k+1)*(length1+length2),length1);
                    memcpy(&f1,block+record_metadata+k*(length1+length2)+length1,length2);
                    memcpy(&f2,block+record_metadata+(k+1)*(length1+length2)+length1,length2);
                    if (b > b2){
                        tempf = b;
                        b = b2;
                        b2 = tempf;
                        memcpy(block+record_metadata+k*(length1+length2),&b,length1);
                        memcpy(block+record_metadata+(k+1)*(length1+length2),&b2,length1);
                        memcpy(block+record_metadata+k*(length1+length2)+length1,&f2,length2);
                        memcpy(block+record_metadata+(k+1)*(length1+length2)+length1,&f1,length2);
                    }
                }
            }
        }
    }
    hp_file_check=BF_WriteBlock(fileDesc,curr_block);        //Grapse to block[0] sthn mhnmh
    if(hp_file_check<0)                                         //Elegxos gia sfalma
    {
        AM_errno = 5;
        AM_PrintError("->Error Writting  block\n");
        return -1;                                            //epistrofh sthn main logo sfalmatos
    }
    return AME_OK;
}



int AM_Sorting_Index(int fileDesc,int curr_block,int i,char* type1,int rec)
{
    int hp_file_check,j,k,a,a1,a2,a3,a4,length1,length2,temp,temp2,point;
    int b3,b4,tempf2;
    float b,b1,b2,tempf;
    int c3,c4,tempc2;
    char *c,*c1,*c2,*tempc;
    void *block,*v1,*v2;
    int parent;
    length1 = File_Information_Array[i].length_key1;
    point = File_Information_Array[i].index_pointers_in_block;
    hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
    if(hp_file_check<0)                                         //Elegxos gia sfalma
    {
        return -1;
    }
    if(strcmp(type1,"c")==0){       //OMOIA GIA TA INDEX TAKSINOMISH
        c1 = (char*)malloc(length1*sizeof(char));
        c2 = (char*)malloc(length1*sizeof(char));
        tempc = (char*)malloc(length1*sizeof(char));
        for (j = 0; j < rec; j++){
            for (k = 0; k < (rec-j-1); k++){
                memcpy(c1,block+index_metadata+point*sizeof(int)+(k)*(length1),length1);
                memcpy(c2,block+index_metadata+point*sizeof(int)+(k+1)*(length1),length1);
                memcpy(&b3,block+index_metadata+(k+1)*(sizeof(int)),sizeof(int));
                memcpy(&b4,block+index_metadata+(k+2)*(sizeof(int)),sizeof(int));
                if (strcmp(c1,c2)>0){
                    strcpy(tempc,c1);
                    tempf2 = b3;
                    strcpy(c1,c2);
                    b3 = b4;
                    strcpy(c2,tempc);
                    b4 = tempf2;
                    memcpy(block+index_metadata+point*sizeof(int)+k*(length1),c1,length1);
                    memcpy(block+index_metadata+point*sizeof(int)+(k+1)*(length1),c2,length1);
                    memcpy(block+index_metadata+(k+1)*(sizeof(int)),&b3,sizeof(int));
                    memcpy(block+index_metadata+(k+2)*(sizeof(int)),&b4,sizeof(int));
                }
            }
        }
        free(c1);
        free(c2);
        free(tempc);
    }
    else if(strcmp(type1,"i")==0){
        for (j = 0; j < rec; j++){
            for (k = 0; k < (rec-j-1); k++){
                memcpy(&a,block+index_metadata+point*sizeof(int)+k*(length1),length1);
                memcpy(&a2,block+index_metadata+point*sizeof(int)+(k+1)*(length1),length1);
                memcpy(&a3,block+index_metadata+(k+1)*(sizeof(int)),sizeof(int));
                memcpy(&a4,block+index_metadata+(k+2)*(sizeof(int)),sizeof(int));
                if (a > a2){
                    temp = a;
                    temp2 = a3;
                    a = a2;
                    a3 = a4;
                    a2 = temp;
                    a4 = temp2;
                    memcpy(block+index_metadata+point*sizeof(int)+k*(length1),&a,length1);
                    memcpy(block+index_metadata+point*sizeof(int)+(k+1)*(length1),&a2,length1);
                    memcpy(block+index_metadata+(k+1)*(sizeof(int)),&a3,sizeof(int));
                    memcpy(block+index_metadata+(k+2)*(sizeof(int)),&a4,sizeof(int));
                }
            }
        }
    }
    else{
        for (j = 0; j < rec; j++){
            for (k = 0; k < (rec-j-1); k++){
                memcpy(&b,block+index_metadata+point*sizeof(int)+k*(length1),length1);
                memcpy(&b2,block+index_metadata+point*sizeof(int)+(k+1)*(length1),length1);
                memcpy(&b3,block+index_metadata+(k+1)*(sizeof(int)),sizeof(int));
                memcpy(&b4,block+index_metadata+(k+2)*(sizeof(int)),sizeof(int));
                if (b > b2){
                    tempf = b;
                    tempf2 = b3;
                    b = b2;
                    b3 = b4;
                    b2 = tempf;
                    b4 = tempf2;
                    memcpy(block+index_metadata+point*sizeof(int)+k*(length1),&b,length1);
                    memcpy(block+index_metadata+point*sizeof(int)+(k+1)*(length1),&b2,length1);
                    memcpy(block+index_metadata+(k+1)*(sizeof(int)),&b3,sizeof(int));
                    memcpy(block+index_metadata+(k+2)*(sizeof(int)),&b4,sizeof(int));
                }
            }
        }
    }
    hp_file_check=BF_WriteBlock(fileDesc,curr_block);        //Grapse to block[0] sthn mhnmh
    if(hp_file_check<0)                                         //Elegxos gia sfalma
    {
        AM_errno = 5;
        AM_PrintError("->Error Writting  block\n");
        return -1;                                            //epistrofh sthn main logo sfalmatos
    }
    return AME_OK;
}
//ektupwsh timwn void 
void AM_Void_Printing(void *value,int i)
{
    if(File_Information_Array[i].type_key1=='c')
    {
        printf("(%s) ",(char*)value);
    }
    else if(File_Information_Array[i].type_key1=='i')
    {
        printf("%d ",*((int*)value));
    }
    else
    {
        printf("%f ",*((float*)value));
    }
}
//ektupwsh timwn void 
void AM_Void_Printing2(void *value,int i)
{
    if(File_Scan_Array[i].type2=='c')
    {
        printf("(%s) ",(char*)value);
    }
    else if(File_Scan_Array[i].type2=='i')
    {
        printf("%d ",*((int*)value));
    }
    else
    {
        printf("%f ",*((float*)value));
    }
}