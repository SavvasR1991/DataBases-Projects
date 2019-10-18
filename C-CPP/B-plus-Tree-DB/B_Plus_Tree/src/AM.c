#include "AM.h"
#include "Operations.h"

/********************************|INIT|************************************/
void AM_Init( void )
{
    int i;
    BF_Init();
    for(i=0;i<MAX_FILES;i++){                   //arxikopoihsh pinaka filr
        File_Information_Array[i].file_descriptor =-1;//anagnwristiko arxeiou
        File_Information_Array[i].length_key1 = 0;    //mikos prwtou kleidioy
        File_Information_Array[i].length_key2 = 0;    //mikos deuterou kleidiou
        File_Information_Array[i].rec_in_block = 0;   //eggrafes sto block
        File_Information_Array[i].index_in_block = 0;//index sto block
        File_Information_Array[i].total_rec = 0;    //sunolikes eggrafes
        File_Information_Array[i].root = -1;        //riza
        File_Information_Array[i].block_num = 0;    //sunolika block sto arxeio
    }
    for(i=0;i<MAX_SCANS;i++){                        //arxikopoihsh pinaka scan
        File_Scan_Array[i].scan_descriptor=-1;      //anagnwristiko scan
        File_Scan_Array[i].file_descriptor=-1;      //anagnwristiko arxeiou
    }
    AM_errno = -1;                                  //arxikopoihsh lathwn                    
    index_metadata = sizeof("Ind")+3*sizeof(int);   //dedomena index
    index_counter = sizeof("Ind");                  //metritis index
    index_parent = sizeof("Ind")+sizeof(int);       //goneas index
    index_next = sizeof("Ind")+2*sizeof(int);       //aderfos index
    record_metadata =sizeof("Rec")+3*sizeof(int);   //dedomena eggrafwn
    record_counter =sizeof("Rec");                  //metritis eggrafwn
    record_parent = sizeof("Rec")+sizeof(int);      //pateras eggrafwn
    record_next = sizeof("Rec")+2*sizeof(int);      //aderfos eggrafwn
    pro_printing=1;                                 //epiprostheta sxolia
};
/**************************************************************************/

/****************************************|CREATE INDEX|********************************************************/
int AM_CreateIndex(char *fileName,char attrType1,int attrLength1,char attrType2,int attrLength2 )
{
    int hp_file_check,hp_file_initialation,i,check_full_file_array = 0,res;//apotelesma reumatos apo anoigma BF arxeiou
    char *B_plus_Three_index="B+_Tree",str_length[5];           //anagnwristiko arxeioy 
    void *block;
    AM_errno = 0;
    printf("->Creating B+ Tree file with name :\"%s\" ,types :[%c] [%c]\n",fileName,attrType1,attrType2);   //metablhth gia to block
    for(i=0;i<MAX_FILES;i++){                           //elegxo adeias theshs sto pinaka
        if(File_Information_Array[i].file_descriptor==-1){
            check_full_file_array = 1;
            break;
        }
    }
    if(check_full_file_array = 0){      //o pinakas arxeiwn einai gematos
        sprintf(errStr,"Error overflow occured,reached max number of files"); //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return 0;
    }//elegxos gia thn orthotita twn tupwn twn metablhtwn
    if(attrType1!='c'&&attrType1!='i'&&attrType1!='f'){//tupos prwtos
        sprintf(errStr,"->Error wrong types used in File \"%s\" \n",fileName);   //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return -1;
    }
    if(attrType2!='c'&&attrType2!='i'&&attrType2!='f'){//typos deuteros
        sprintf(errStr,"->Error wrong types used in File \"%s\" \n",fileName);         //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return -1;
    }//elegxos gia ta mikoi twn metablhtwn
    if(attrType1=='c'&&attrLength1>256){//sumboloseira oxi panw apo 256
        sprintf(errStr,"->Error wrong types used in File \"%s\" \n",fileName);          //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return -1;
    }
    if(attrType1=='i'&&attrLength1!=4){//akeraioi
        sprintf(errStr,"->Error wrong types used in File \"%s\" \n",fileName);         //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return -1;
    }
    if(attrType1=='f'&&attrLength1!=4){//pragmatikoi
        sprintf(errStr,"->Error wrong types used in File \"%s\" \n",fileName);          //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return -1;
    }
    if(attrType2=='c'&&attrLength2>256){//deuteros typos
        sprintf(errStr,"->Error wrong types used in File \"%s\" \n",fileName);          //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return -1;
    }
    if(attrType2=='i'&&attrLength2!=4){
        sprintf(errStr,"->Error wrong types used in File \"%s\" \n",fileName);           //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return -1;
    }
    if(attrType2=='f'&&attrLength2!=4){
        sprintf(errStr,"->Error wrong types used in File \"%s\" \n",fileName);           //metablhth gia to block
        AM_PrintError(errStr);
        printf("->B+ Tree file with name :\"%s\" not created\n->\n",fileName);
        return -1;
    }
    hp_file_check=BF_CreateFile(fileName);                      //Dhmiourgia BF arxeiou filename
    if(hp_file_initialation<0)                                  //Elegxos sfalmatos
    {
        sprintf(errStr,"->Error creating File \"%s\" \n",fileName);     //metablhth gia to block
        AM_PrintError(errStr);
        BF_PrintError(errStr);
        return -1;                                            //epistrofh sthn main logo sfalmatos
    }
    hp_file_initialation=BF_OpenFile(fileName);                 //anoigma BF arxeiou filename
    if(hp_file_initialation<0)                                  //Elegxos sfalmatos
    {
        printf(errStr,"->Error Opening  File \"%s\" \n",fileName);        //metablhth gia to block
        AM_PrintError(errStr);
        BF_PrintError(errStr);
        return -1;                                            //epistrofh sthn main logo sfalmatos
        
    }
    hp_file_check=BF_AllocateBlock(hp_file_initialation);       //desmeush arxikou block (0)
    if(hp_file_check<0){
        printf(errStr,"->Error Allocate blocl\n");        //metablhth gia to block
        AM_PrintError(errStr);
        BF_PrintError(errStr);
        return -1;                                            //epistrofh sthn main logo sfalmatos
    }
    hp_file_check=BF_ReadBlock(hp_file_initialation,0,&block);  //diabasma tou block [0]
    if(hp_file_check<0){                                         //Elegxos gia sfalma
        printf(errStr,"->Error Reading block\n");        //metablhth gia to block
        AM_PrintError(errStr);
        BF_PrintError(errStr);       
        return -1;                                            //Epistrofh sthn main logo sfalmatos
    }
    memcpy(block,B_plus_Three_index,sizeof("B+_Tree"));//Grapse sthn arxh tou block[0] thn leksh "B+_Tree" gia anagnwristiko swrou
    if(attrType1=='c'){//egrafh sto block[0] to c gia sumboloseira
        memcpy(block+sizeof("B+_Tree"),"c",sizeof("c"));
    }
    if(attrType1=='i'){//egrafh sto block[0] to i gia akeraio
        memcpy(block+sizeof("B+_Tree"),"i",sizeof("i"));
    }
    if(attrType1=='f'){//egrafh sto block[0] to f gia pragmatiko
        memcpy(block+sizeof("B+_Tree"),"f",sizeof("f"));
    }
    memcpy(block+sizeof("B+_Tree")+2*sizeof("c"),&attrLength1,sizeof(attrLength1));//eggrafh sto block[0]
    if(attrType2=='c')
    {
        memcpy(block+sizeof("B+_Tree")+sizeof("c"),"c",sizeof("c"));
    }                   //Grapse sthn arxh tou block[0] thn leksh "heap"gia anagnwristiko swrou
    if(attrType2=='i'){//deuterou typou gia block [0]
        memcpy(block+sizeof("B+_Tree")+sizeof("c"),"i",sizeof("i"));
    }
    if(attrType2=='f'){
        memcpy(block+sizeof("B+Tree")+sizeof("c"),"f",sizeof("f"));
    }
    memcpy(block+sizeof("B+_Tree")+2*sizeof("c")+sizeof(int),&attrLength2,sizeof(attrLength2));
    hp_file_check=BF_WriteBlock(hp_file_initialation,0);        //Grapse to block[0] sthn mhnmh
    if(hp_file_check<0){                                         //Elegxos gia sfalma
        sprintf(errStr,"->Error Writing block\n");        //metablhth gia to block
        AM_PrintError(errStr);
        BF_PrintError(errStr);               return -1;                     //epistrofh sthn main logo sfalmatos
    }
    printf("->B+ Tree file with name :\"%s\" created\n->\n",fileName);
    return AME_OK;//ola ok 
};
/**************************************************************************************************************/

/****************************************|DESTROY INDEX|*******************************************************/
int AM_DestroyIndex(char *fileName){
    int status,i,check_no_open = 0;
    AM_errno = 1;
    for(i=0;i<MAX_FILES;i++){   //elegxos gia anoixta arxeio
        if(File_Information_Array[i].file_descriptor!=-1){
            if(strcmp(File_Information_Array[i].filename,fileName)==0){
                check_no_open = 1;//an nai den diagrafete to arxeio
                break;
            }
        }
    }
    if(check_no_open == 1){//oxi katastrofh
        sprintf(errStr,"->Error Destoying File \"%s\" it is open \n",fileName);     //metablhth gia to block
        AM_PrintError(errStr);
        return -1;
    }
    else{//alliws katastrofh
        status = remove(fileName);
        if( status == 0 ){//elegxos remove
            printf("->File \"%s\" is deleted successfully.\n",fileName);
            return AME_OK;
        }
        else{//sfalma 
            sprintf(errStr,"->Error Destoying File ,revome fault ,file :\"%s\" \n",fileName);     //metablhth gia to block
            AM_PrintError(errStr);
            return -1;
        }
    }
};
/**************************************************************************************************************/

/****************************************|OPEN INDEX|*****************************************/
int AM_OpenIndex (char *fileName)
{
    int hp_file_open, hp_file_check;                          //Elegxos sfalmatos
    char index[8];                                          //Leksh gia thn tautopoihsh tou arxeiou
    void *block;
    AM_errno = 2;
    printf("->Opening B+ Tree file :\"%s\"\n",fileName);
    hp_file_open=BF_OpenFile(fileName);                     //anoigma arxeiou 
    if(hp_file_open<0){                                     //elegxos sfalmatos
            sprintf(errStr,"->Error Opening Block\n");             //metablhth gia to block
            AM_PrintError(errStr);
            BF_PrintError(errStr);
            return -1;                                        //sfalma
    }
    hp_file_check=BF_ReadBlock(hp_file_open,0,&block);      //diabasma block[0]
    if(hp_file_check<0){                                     //elegxos sfalmatos
            sprintf(errStr,"->Error Reading Block\n");             //metablhth gia to block
            AM_PrintError(errStr);
            BF_PrintError(errStr);
            return -1;                                        //sfalma
    }
    memcpy(index,block,sizeof("B+_Tree"));                     //anagnwsh anagnwristikou swrou gia tautopoihsh
    if(strcmp(index,"B+_Tree")!=0){                             //elegxos anagnwristikou
            printf("->File \"%s\" is not a B+ Tree file...\n->\n",fileName);
            return -1;                                        //sfalma
    }
    printf("->File \"%s\" is open...\n->\n",fileName);  //ola ok 
    return hp_file_open;
};
/*********************************************************************************************/

/******************************************|CLOSE_INDEX|*************************************************************/
int AM_CloseIndex (int fileDesc)
{
    int hp_file_close, i,check_scan_file = 0;
    AM_errno = 3;
    for(i=0;i<MAX_SCANS;i++){       //elegxos gia anoixta scan
        if(File_Scan_Array[i].file_descriptor==fileDesc){
            check_scan_file = 1;
            break;
        }
    }
    if(check_scan_file == 1){//oxi kleisimo 
        AM_errno2 = 1;
        AM_PrintError("Error in closing index ,has opened scans\n");
        return -1;
    }
    for(i=0;i<MAX_FILES;i++){
        if(File_Information_Array[i].file_descriptor==fileDesc){//psakse to arxeio
            hp_file_close=BF_CloseFile(fileDesc);          //Kleise to arxeio BF
            if(hp_file_close<0) {                           //elegxos sfalmatos
                printf("->Error closing B+ Three file...\n");
                sprintf(errStr,"->->Error closing B+ Three file...\n");      //metablhth gia to block
                AM_PrintError(errStr);
                BF_PrintError(errStr);
                return -1;                               //sfalma
            }
            else{                                          
                    printf("->Closing the B+ Tree file :\"%s\"...\n",File_Information_Array[i].filename);
                    File_Information_Array[i].file_descriptor = -1;//midenismos metablhtwn
                    File_Information_Array[i].total_rec = 0;
                    File_Information_Array[i].index_in_block = 0;
                    File_Information_Array[i].rec_in_block = 0;
                    File_Information_Array[i].length_key1 = 0;
                    File_Information_Array[i].length_key2 = 0;
                    free(File_Information_Array[i].filename);
                    return AME_OK;
            }
        }
    }
    printf("->Error closing B+ Three file it doesn't exist...\n");
    return -1;//sfalma
};
/********************************************************************************************************************/

/******************************************|INSERT ENTRY|******************************************************/
int AM_InsertEntry(int fileDesc,void *value1,void *value2)
{
    void *block,*block2,*block3,*block4,*va1[100],*va2[100];
    float float_value;
    char type1[]="c",type2[]="c",*string_value,check_block[3],check[40];
    int block1_id,block2_id,block3_id,index_middle,ount_ind;
    int block_curr,cmp_res,i,j,error,index_block,rec_block,hp_file_check;
    int int_value,temp_help,temp2_help,brother,total_blocks,parent,insert_index;
    int one=1,zero = 0,curr_num_blocks,no_rec_block,length1,length2,totalRec,file_find = 0;
    int k,end_search,indexes,null = -1,split_num,index_pointers_length,temp_bro;
    Value_Array value[3];
    ////////////////////////////////CONTROL B+TREE////////////////////////////////
    hp_file_check=BF_ReadBlock(fileDesc,0,&block);  //diabasma tou block [0]
    if(hp_file_check<0){                                   //Elegxos gia sfalma
        AM_errno = 5;
        AM_PrintError("->Error Reading  block\n");
        return -1;                                            //Epistrofh sthn main logo sfalmatos
    }
    memcpy(type1,block+sizeof("B+_Tree"),sizeof(type1));//lipsi typwn apo block[0]
    memcpy(type2,block+sizeof("B+_Tree")+sizeof(type1),sizeof(type2));//typos 2
    printf("->|----------------Insering Record---------------|  \n");
    if(strcmp(type1,"c")==0){   //sumboloseira ,aksia se void
        string_value = (char*)value1;
        printf("->  <<Record [c]:%s ,",string_value);
    }
    else if(strcmp(type1,"i")==0){//int
        int_value=*((int *)value1);
        printf("->  <<Record [i]%d ,",int_value);
    }
    else if(strcmp(type1,"f")==0){//float
        float_value=*((float *)value1);
        printf("->  <<Record [f]:%f ,",float_value);
    }
    else{       //sfalma
        printf("->Error\n");
        return -1;
    }
    if(strcmp(type2,"c")==0){//typos 2 string
        string_value = (char*)value2;
        printf(" [c]:%s >>\n",string_value);
    }
    else if(strcmp(type2,"i")==0){//int
        int_value=*((int *)value2);
        printf(" [i]:%d >>\n",int_value);
    }
    else if(strcmp(type2,"f")==0){//float
        float_value=*((float *)value2);
        printf(" [f]:%f >>\n",float_value);
    }
    else{   //sfalma
        printf("->Error\n");
        return -1;
    }
    if(File_Information_Array[fileDesc].file_descriptor==fileDesc){//elegxos gia open file
        file_find = 1;//apeuthias
        i=fileDesc;
    }
    else{
        for(i=0;i<MAX_FILES;i++){//alliws sarwse olo to pinaka
            if(File_Information_Array[i].file_descriptor==fileDesc){
                file_find = 1;//brethike
                break;
            }
        }
    }
    if(file_find == 0){//den brethike ,sfalma
        return -1;
    }
    ////////////////////////////////B+TREE INSERT////////////////////////////////
    int melina=0;   //arxikopoihsh metablitwn apo to pinaka arxeiwn
    length1 = File_Information_Array[i].length_key1;
    length2 = File_Information_Array[i].length_key2;
    totalRec = File_Information_Array[i].total_rec;
    index_block = File_Information_Array[i].index_in_block;
    rec_block = File_Information_Array[i].rec_in_block;
    total_blocks = BF_GetBlockCounter(fileDesc);
    index_pointers_length = File_Information_Array[i].index_pointers_in_block;
    if(pro_printing==1){//ekthpwsh twn metablhtwn gia bohthia
        printf("->  <<Informations on B+_Tree ,of file \"%s\">>\n",File_Information_Array[i].filename);
        printf("->         *  File Name         : \"%s\">>\n",File_Information_Array[i].filename);
        printf("->         *  Records in a block: [%d]>>\n",rec_block);
        printf("->         *  Ind pointers block: [%d]>>\n",index_pointers_length);
        printf("->         *  Total indexes     : [%d]>>\n",File_Information_Array[i].index_in_block);
        printf("->         *  Length of value1  : [%d]>>\n",length1);
        printf("->         *  Length of value2  : [%d]>>\n",length2);
        printf("->         *  Total records     : [%d]>>\n",totalRec);
        printf("->         *  Root  block       : [%d] \n",File_Information_Array[i].root);
        printf("->         *  Total blocks      : [%d] \n",total_blocks);
    }
    if(total_blocks==1){//dimiourgia prwtou block,eggrafwn
        error = AM_Create_Record_Block(fileDesc,value1,value2,length1,length2);
        if(error !=AME_OK){//sfalma
            AM_errno = 5;
            AM_PrintError("->Error Creating Record  block\n");
            return -1;
        }
        hp_file_check=BF_ReadBlock(fileDesc,1,&block);  //diabasma tou block [0]
        if(hp_file_check<0){                              //Elegxos gia sfalma
            AM_errno = 5;
            AM_PrintError("->Error Reading  Index  block\n");
            return -1;                                //Epistrofh sthn main logo sfalmatos
        }
        hp_file_check=BF_WriteBlock(fileDesc,1);   //Grapse to block[0] sthn mhnmh
        if(hp_file_check<0){                          //Elegxos gia sfalma
                AM_errno = 5;
                AM_PrintError("->Error Writting  block\n");
                return -1;                             //epistrofh sthn main logo sfalmatos
        }
    }
    else{
        if(total_blocks==2){//to prwto block dhmiourghtike,
            block_curr = 1;//ksekinaei o algorithmos,eggrafh
        }
        else{               //exoume riza
            block_curr = File_Information_Array[i].root;
        }
        end_search = 1; //metabliti gia anazitisi
        insert_index = 0;//metabliti gia eisodo h anazhthsh index
        while(end_search!=0){           //anazitisi sto dentro
            hp_file_check=BF_ReadBlock(fileDesc,block_curr,&block);  //diabasma tou block [0]
            if(hp_file_check<0){                               //Elegxos gia sfalma
                AM_errno = 5;
                AM_PrintError("->Error Reading  Index  block\n");
                return -1;                              //Epistrofh sthn main logo sfalmatos
            }
            memcpy(&check_block,block,sizeof("Ind")); //anagnwristiko block
            if(strcmp(check_block,"Ind")==0){   //eimaste se index
                memcpy(&indexes,block+index_counter,sizeof(int));//arithmos indexes sto block
                if(pro_printing==1)
                {
                    printf("->      ->>Block type of Index :[%d]  %d/%d Indexes\n",block_curr,indexes,index_block);
                }
                if(insert_index == 0){      //anazitisi sto block index
                    for(j=0;j<indexes;j++){ //sarwsh toy block 
                        if(pro_printing==1)
                        {
                            printf("->      ->>     *Index Compare values  :");
                        }               //sugkrisi tis timis me twn indexes
                        cmp_res = AM_Compare_Values(value1,j,type1,block_curr,fileDesc,length1);
                        if(cmp_res==0){         //pame aristera
                            memcpy(&block_curr,block +index_metadata +j*sizeof(int),sizeof(int));
                            if(pro_printing==1)
                                printf("->      ->>     *Going to block :[%d]\n",block_curr);
                            break;
                        }
                        else{       //pame deksia
                           if(j == (indexes -1)){   //ena ligotero logo tou prwtou
                                memcpy(&block_curr,block + index_metadata + indexes * sizeof(int),sizeof(int));
                           }
                           if(pro_printing==1)
                                printf("->      ->>     *Next loop in block :[%d]\n",block_curr);
                        }
                    }
                    if(pro_printing==1){        //mia ektupwsh sxoliwn
                        printf("->      ->>     *Block index has : {");
                        for(k=0;k<indexes;k++){
                            memcpy(&va1,block+index_metadata+index_pointers_length*sizeof(int)+(k)*length1,length1);
                            AM_Void_Printing(va1,i);
                        }
                        printf("} ,[%d]\n",indexes);
                    }
                }
                else{
                    if(pro_printing==1){//sxolia
                        printf("->      ->>Insering index in  block:[%d] ,points block [%d] \n",(block_curr),insert_index);
                    }
                    if(indexes>=index_block-1){     //eisagwgh timis sto index
                        if(pro_printing == 1){      //spliting
                            printf("->      ->>Block type of Index :[%d] , is full ,splitting %d/%d Indexes\n",block_curr,indexes,index_block);
                            
                        }//eisagwgh tis timis
                        memcpy(block+index_metadata+index_pointers_length*sizeof(int)+(indexes)*length1,&value1,length1);
                        hp_file_check = AM_Sorting_Index(fileDesc,block_curr,i,type1,indexes);//taksinomisi tou block
                        memcpy(&va1,block+index_metadata+index_pointers_length*sizeof(int)+(index_middle)*length1,length1);
                        curr_num_blocks = BF_GetBlockCounter(fileDesc);//timi toy epomenouy
                        memcpy(block+index_next,&curr_num_blocks,sizeof(int));  //aderfos
                        memcpy(&curr_num_blocks,block+index_parent,sizeof(int));//pateras
                        if(curr_num_blocks ==-1){       //den exei patera,dhmiourgia rizas
                            curr_num_blocks = BF_GetBlockCounter(fileDesc);
                            hp_file_check =AM_Create_Index_Block(fileDesc,va1,length1);
                            hp_file_check=BF_ReadBlock(fileDesc,curr_num_blocks,&block2);  //diabasma tou block [0]
                            if(hp_file_check<0)                                         //Elegxos gia sfalma
                            {
                                AM_errno = 5;
                                AM_PrintError("->Error Reading  Index  block\n");
                                return -1;                                            //Epistrofh sthn main logo sfalmatos
                            }
                            memcpy(block2+index_metadata+index_pointers_length*sizeof(int),block+index_metadata+index_pointers_length*sizeof(int)+(index_middle+1)*(length1),(index_middle+1)*(length1));
                            memcpy(block+index_counter,&index_middle,sizeof(int));
                            ount_ind = index_middle-1;
                            memcpy(block2+index_counter,&ount_ind,sizeof(int));
                            index_middle = (index_block+1)/2;
                           /* memcpy(block2+index_metadata+index_pointers_length*sizeof(int),block+index_metadata+index_pointers_length*sizeof(int));
                            memcpy(block2+index_metadata+index_pointers_length*sizeof(int),block+index_metadata+index_pointers_length*sizeof(int));*/
                        }
                        else{               //exei patera 
                            hp_file_check =AM_Create_Index_Block(fileDesc,va1,length1);
                            curr_num_blocks = BF_GetBlockCounter(fileDesc);
                        }
                    }
                    else{           //alliws eisagwgh timis sto index
                        memcpy(block+index_metadata+index_pointers_length*sizeof(int)+(indexes)*length1,value1,length1);
                        indexes++;              //ena index parapano
                        memcpy(block+index_metadata+(indexes)*sizeof(int),&insert_index,sizeof(int));
                        hp_file_check = AM_Sorting_Index(fileDesc,block_curr,i,type1,indexes);
                        memcpy(block+index_counter,&indexes,sizeof(int));       //grapse sto disko
                        end_search =0;          //telos anazitisis
                        hp_file_check=BF_WriteBlock(fileDesc,block_curr);    //Grapse to block[0] sthn mhnmh
                        if(hp_file_check<0){                                 //Elegxos gia sfalma
                            AM_errno = 5;
                            AM_PrintError("->Error Writting  block\n");
                            return -1;                           //epistrofh sthn main logo sfalmatos
                        }
                        break;
                    }
                }
            }
            else if(strcmp(check_block,"Rec")==0){      //alliws eimaste se record
                memcpy(&temp_help,block+record_counter,sizeof(int));
                if(pro_printing==1){
                    printf("->      ->>Record block [%d] ,%d/%d \n",block_curr,temp_help,rec_block);
                }
                if((temp_help)<rec_block){      //an uparxei xwros 
                    if(pro_printing==1)
                        printf("->      ->>     *Record block [%d] sorting\n",block_curr);
                    memcpy(block+record_metadata+temp_help*(length1+length2),value1,length1);//antrigrafh timwn
                    memcpy(block+record_metadata+temp_help*(length1+length2)+length1,value2,length2);
                    temp_help++;                        //allo ena record
                    memcpy(block+record_counter,&temp_help,sizeof(int));
                    error = AM_Sorting_Record(fileDesc,block_curr,i,type1,temp_help);//taksinomisi
                    if(error == -1){    //sfalma
                        return -1;
                    }
                    end_search = 0;
                    if(pro_printing==1){
                        printf("->      ->>     *Record block [%d] ,%d/%d Records : {",block_curr,temp_help,rec_block);
                        for(j=0;j<temp_help;j++){
                            memcpy(&va1,block+record_metadata+j*(length2+length1),length1);
                            AM_Void_Printing(va1,i);
                        }
                        printf("} ,[%d] ",temp_help);
                        memcpy(&temp2_help,block+record_next,sizeof(int));
                        printf(",brother [%d] ",temp2_help);
                        memcpy(&temp2_help,block+record_parent,sizeof(int));
                        printf(",parent [%d] \n",temp2_help);
                    }
                    break;
                }
                else{       //an einai gemato
                    if(pro_printing==1){            //splitting
                        printf("->      ->>     *Record block [%d] splitting\n",block_curr);
                    }
                    melina=rec_block/2;         //to mesaio stoixeio
                    if(pro_printing==1)
                        printf("->      ->>     *Record block splitting in %d records\n",melina);
                    memcpy(&va1,block+record_metadata+(melina-1)*(length1+length2),length1);//pare to mesaio
                    cmp_res=1;
                    while(cmp_res==1){      //elegxos gia n-plotypa
                        if(pro_printing==1){
                            printf("->      ->>     *Record Compare values  :");
                        }
                        cmp_res = AM_Compare_Values(va1,melina,type1,block_curr,fileDesc,length1);
                        if(cmp_res==0) //telos 
                            break;
                        melina++;       //thesi gia to split
                    }
                    memcpy(&parent,block+record_parent,sizeof(int));    //lhpsh dedomenwn apo disko
                    curr_num_blocks = BF_GetBlockCounter(fileDesc);
                    memcpy(&temp_bro,block+record_next,sizeof(int));
                    memcpy(block+record_next,&curr_num_blocks,sizeof(int));
                    block2_id = curr_num_blocks;        //dimiourgia neas eggrafhs block
                    hp_file_check = AM_Create_Record_Block(fileDesc,value1,value2,length1,length2);
                    if(pro_printing==1){
                        printf("->      ->>     *Record block [%d] is created from splitting \n",(block2_id));
                    }
                    hp_file_check=BF_ReadBlock(fileDesc,block2_id,&block2);  //diabasma tou block [0]
                    if(hp_file_check<0){                                         //Elegxos gia sfalma
                            return -1;                                            //Epistrofh sthn main logo sfalmatos
                    }
                    if(pro_printing==1){
                        printf("->      ->>     *Record [%d] splits in %d/%d \n",block_curr,melina,rec_block);
                    }       //pragmatopoihsh spliting
                    temp_help = rec_block-melina;       //antigrafh dedomenwn apo block se block
                    memcpy(block2+record_metadata,block+record_metadata+(melina)*(length1+length2),(temp_help)*(length1+length2));
                    memcpy(block2+record_counter,&temp_help,sizeof(int));
                    memcpy(block2+record_next,&temp_bro,sizeof(int));   //rithmish dedomenwn
                    memcpy(block+record_counter,&melina,sizeof(int));
                    if(pro_printing==1){            //epiprostheta sxolia
                        printf("->      ->>     *Record [%d] :  {",block_curr);
                        for(j=0;j<rec_block;j++){
                            memcpy(&va1,block+record_metadata+j*(length2+length1),length1);
                            AM_Void_Printing(va1,i);
                        }
                        printf("} ,[%d]\n",j);
                        printf("->      ->>     *Splitted Record [%d] :  {",block_curr);
                        for(j=0;j<melina;j++){
                            memcpy(&va1,block+record_metadata+j*(length2+length1),length1);
                            AM_Void_Printing(va1,i);
                        }
                        printf("} ,[%d]\n",j);
                        printf("->      ->>     *New Record      [%d] :  {",block2_id);
                        for(j=0;j<temp_help;j++){
                            memcpy(&va1,block2+record_metadata+j*(length2+length1),length1);
                            AM_Void_Printing(va1,i);
                        }
                        printf("} ,[%d]\n",j);
                    }
                    memcpy(&va1,block2+record_metadata,length1);  //prwto stoixeio deuterou block
                    if(parent==-1){                                  //gia na paei sto index
                        File_Information_Array[i].root = 3;         //dhmiourgia prwtou index
                        hp_file_check =AM_Create_Index_Block(fileDesc,va1,length1);//mia fora sto programma
                        curr_num_blocks = 3;                        //tha einai to trito block stadar
                        block3_id = curr_num_blocks;
                        memcpy(block+record_parent,&curr_num_blocks,sizeof(int));
                        memcpy(block2+record_parent,&curr_num_blocks,sizeof(int));
                        hp_file_check=BF_ReadBlock(fileDesc,(3),&block3);  //diabasma tou block [0]
                        if(hp_file_check<0){                            //Elegxos gia sfalma
                                return -1;                   //Epistrofh sthn main logo sfalmatos
                        }
                        if(pro_printing==1){
                            printf("->      ->>Index block Root [%d] created,1/%d indexes\n",(curr_num_blocks),index_block);
                        }
                        memcpy(block3+index_parent,&zero,sizeof(int));          //rithmisi dedomenwn index block
                        memcpy(block3+index_metadata,&block_curr,sizeof(int));
                        memcpy(block3+index_metadata+sizeof(int),&block2_id,sizeof(int));
                        hp_file_check=BF_WriteBlock(fileDesc,block_curr);        //Grapse to block[0] sthn mhnmh
                        if(hp_file_check<0){                                         //Elegxos gia sfalma
                                AM_errno = 5;
                                AM_PrintError("->Error Writting  block\n");
                                return -1;                                            //epistrofh sthn main logo sfalmatos
                        }
                        hp_file_check=BF_WriteBlock(fileDesc,block2_id);        //Grapse to block[0] sthn mhnmh
                        if(hp_file_check<0){                                         //Elegxos gia sfalma
                                AM_errno = 5;
                                AM_PrintError("->Error Writting  block\n");
                                return -1;                                            //epistrofh sthn main logo sfalmatos
                        }
                        hp_file_check=BF_WriteBlock(fileDesc,block3_id);        //Grapse to block[0] sthn mhnmh
                        if(hp_file_check<0){                                         //Elegxos gia sfalma
                                AM_errno = 5;
                                AM_PrintError("->Error Writting  block\n");
                                return -1;                                            //epistrofh sthn main logo sfalmatos
                        }
                        end_search = 0;
                    }
                    else{                                   //uparxei index 
                        memcpy(block+record_parent,&parent,sizeof(int));
                        memcpy(block2+record_parent,&parent,sizeof(int));
                        insert_index = curr_num_blocks;     //rithmisi metablitwn
                        block_curr = parent;
                        memcpy(value1,va1,length1);
                        if(pro_printing==1){
                            printf("->      ->>     *Insering index in  block:[%d] ->[",parent);
                            AM_Void_Printing(va1,i);
                            printf("] \n");
                        }
                        hp_file_check=BF_WriteBlock(fileDesc,block_curr);        //Grapse to block[0] sthn mhnmh
                        if(hp_file_check<0){                                         //Elegxos gia sfalma
                                AM_errno = 5;
                                AM_PrintError("->Error Writting  block\n");
                                return -1;                                            //epistrofh sthn main logo sfalmatos
                        }
                        hp_file_check=BF_WriteBlock(fileDesc,block2_id);        //Grapse to block[0] sthn mhnmh
                        if(hp_file_check<0)                                         //Elegxos gia sfalma
                        {
                                AM_errno = 5;
                                AM_PrintError("->Error Writting  block\n");
                                return -1;                                            //epistrofh sthn main logo sfalmatos
                        }
                    }
                }
            }
            else{       //sfalma
                    printf("->ERROR\n");
                    return -1;
            }
            
        }
    }
    totalRec++;         //teleytaies leptomeries
    printf("->|---------------Record is insert---------------|\n->\n");
    File_Information_Array[i].block_num = BF_GetBlockCounter(fileDesc)-1;
    File_Information_Array[i].total_rec = totalRec;
    return AME_OK;
}
/**************************************************************************************************************/

/******************************************|OPEN SCAN|*****************************************************/
int AM_OpenIndexScan(int fileDesc,int op,void *value)
{
    int i,j,hp_file_check,k, check_file_ex=0,counter,check_scan_full = 0;
    int block_pointer,length2,search = 0,cmp_res,length1,loop_count;
    int founded = 0,temp,temp2,curr_rec,total_scan_rec,AM_errno5;
    char check_block[3],type1[]="l",ty1,ty2,*c,*c2;
    float tempf,tempf2;
    void *block;
    for(i=0;i<MAX_FILES;i++){               //elengxos uparkshs arxeiou 
        if(File_Information_Array[i].file_descriptor == fileDesc){
            check_file_ex = 1;
            break;
        }
    }
    if(check_file_ex == 0){             //arxeio den uparxei
        printf("->Error,File doesn't exist,you cannot scan this file\n");
        return -1;
    }
    printf("->Insert a scan to B+_Tree \"%d\"\n",*((int*)value));//File_Information_Array[i].filename);
    k = i;
    if(AM_Print_Info_Scan_Insert(i,op,value)==AME_OK){  //elenxos ton stoixeiwn tou xrhsth
        for(i=0;i<MAX_SCANS;i++){                   //elenxos an o pinakas scan einai gematos
            if(File_Scan_Array[i].scan_descriptor == -1){
                check_scan_full = 1;
                break;
            }
        }
        if(check_scan_full == 1){           //ola ok
            total_scan_rec = 0;
            block_pointer = File_Information_Array[k].root; //labe dedomena
            length1 = File_Information_Array[k].length_key1;//apo to open pinaka
            length2 = File_Information_Array[k].length_key2;//giati tha xreiastoun
            ty2 = File_Information_Array[k].type_key2;
            ty1 = File_Information_Array[k].type_key1;
            if(pro_printing==1)
                printf("->Searching the tree....\n");
            hp_file_check=BF_ReadBlock(fileDesc,0,&block);  //diabasma tou block [0]
            if(hp_file_check<0)                               //Elegxos gia sfalma
            {
                    AM_PrintError("->Error Reading info block\n");
                    BF_PrintError("->Error Reading info block\n");
                    return -1;                                     //Epistrofh sthn main logo sfalmatos
            }
            memcpy(type1,block+sizeof("B+_Tree"),sizeof(type1)); //labe apo ton disko
            while(search==0)                        //ksekinaei h anazitish 
            {
                hp_file_check=BF_ReadBlock(fileDesc,block_pointer,&block);  //diabasma tou block 
                if(hp_file_check<0)                                         //Elegxos gia sfalma
                {
                    AM_PrintError("->Error Reading  Index  block\n");
                    BF_PrintError("->Error Reading  Index block\n");
                    return -1;                                            //Epistrofh sthn main logo sfalmatos
                }
                memcpy(&check_block,block,sizeof("Ind"));           //na doume ti block einai
                if(strcmp(check_block,"Ind")==0)                    //an einai Index
                {
                    memcpy(&counter,block+index_counter,sizeof(int));//pare ton metrith tou
                    for(j=0;j<counter;j++)                            //sarwse to block
                    {
                        if(pro_printing==1)
                            printf("->      ->>     *Compare values  :"); //sugkrish
                        cmp_res = AM_Compare_Values(value,j,type1,block_pointer,fileDesc,length1);
                        if(cmp_res==0)      //phgene aristera
                        {
                            memcpy(&block_pointer,block +index_metadata +j*sizeof(int),sizeof(int));
                            if(pro_printing==1)
                                printf("->      ->>     *Going to block :[%d]\n",block_pointer);
                            break;
                        }
                        else
                        {
                           if(j == (counter -1))        //phgene deksia
                           {
                                memcpy(&block_pointer,block + index_metadata + counter * sizeof(int),sizeof(int));
                           }
                           if(pro_printing==1)
                                printf("->      ->>     *Next loop in block :[%d]\n",block_pointer);
                        }
                    }
                }
                else
                {                                       //an einai eggrafh
                    memcpy(&counter,block+record_counter,sizeof(int));//metriths
                    if(ty1 == 'i')          //peritwseis
                    {
                        temp2 = (*(int*)value);//int
                        for(j=0;j<counter;j++) //sarwse to block
                        {
                            memcpy(&temp,block+record_metadata+j*(length1+length2),length1);//bres thn isotita
                            if(temp2==temp){
                                if(total_scan_rec==0){
                                    founded = 1;//thesi prwths isotitas
                                    loop_count = j;
                                }
                                total_scan_rec++;
                            }                            
                        }
                        if(total_scan_rec>0) //teliwse h anazitisi brethikan
                        {
                            search = 1;
                        }
                        break;
                    }
                    else if(ty1 == 'f')//omoia gia float
                    {
                        tempf = (*(float*)value);
                        for(j=0;j<counter;j++){
                            memcpy(&tempf2,block+record_metadata+j*(length1+length2),length1);
                            if(tempf==tempf2){
                                if(total_scan_rec==0){
                                    founded = 1;
                                    loop_count = j;
                                }
                                total_scan_rec++;
                            }                            
                        }
                        if(total_scan_rec>0){
                            search = 1;
                        }
                        break;
                    }
                    else{   //omoia gia strings
                        c = ((char*)value);
                        c2 = (char*)malloc(length1*sizeof(char));
                        for(j=0;j<counter;j++){
                            memcpy(c2,block+record_metadata+j*(length1+length2),length1);
                            if(strcmp(c,c2)==0){
                                if(total_scan_rec==0){
                                    founded = 1;
                                    loop_count = j;
                                }
                                total_scan_rec++;
                            }                            
                        }
                        if(total_scan_rec>0){
                            search = 1;
                        }
                        free(c2);
                        break;
                    }
                        
                }
            }
            if(founded==0)//den brethike
            {
                printf("->Record not founded...\n");
                return -1;
            }
            else{//brethike
                printf("->Record founded...\n");
                if(File_Scan_Array[block_pointer].scan_descriptor==-1){ //rithmish pinaka scan,apeuthias
                    File_Scan_Array[block_pointer].scan_descriptor = block_pointer;
                    File_Scan_Array[block_pointer].curr_rec = loop_count;
                    File_Scan_Array[block_pointer].op = op;
                    File_Scan_Array[block_pointer].file_descriptor = fileDesc;
                    File_Scan_Array[block_pointer].possition_rec_A = loop_count;
                    File_Scan_Array[block_pointer].total_scan_rec = total_scan_rec;
                    File_Scan_Array[block_pointer].possition_rec_B =loop_count + total_scan_rec-1;
                    File_Scan_Array[block_pointer].type1 =ty1;
                    File_Scan_Array[block_pointer].length1 =length1;
                    File_Scan_Array[block_pointer].length2 =length2;
                    File_Scan_Array[block_pointer].type2 =ty2;
                    File_Scan_Array[block_pointer].current_scan = 0;
                    File_Scan_Array[block_pointer].block = block_pointer;
                    File_Scan_Array[block_pointer].curr_block = File_Information_Array[k].root;
                    File_Scan_Array[block_pointer].rec_in_block = File_Information_Array[k].rec_in_block;
                    return block_pointer;
                }
                else{
                    for(i=0;i<MAX_SCANS;i++){
                        if(File_Scan_Array[i].scan_descriptor==-1){ //bres kenh thesi
                            File_Scan_Array[i].scan_descriptor = i;
                            File_Scan_Array[i].block = block_pointer;
                            File_Scan_Array[i].curr_rec = loop_count;
                            File_Scan_Array[i].op = op;
                            File_Scan_Array[i].file_descriptor = fileDesc;
                            File_Scan_Array[i].possition_rec_A = loop_count;
                            File_Scan_Array[i].total_scan_rec = total_scan_rec;
                            File_Scan_Array[i].possition_rec_B = loop_count + total_scan_rec-1;
                            File_Scan_Array[i].type1 =ty1;
                            File_Scan_Array[i].type2 =ty2;
                            File_Scan_Array[i].length1 =length1;
                            File_Scan_Array[i].length2 =length2;
                            File_Scan_Array[i].current_scan = 0;
                            File_Scan_Array[i].curr_block = File_Information_Array[k].root;
                            File_Scan_Array[i].rec_in_block = File_Information_Array[k].rec_in_block;
                            return i;
                        }
                    }
                }
            }
        }
        else{                   //pinakas gematos 
            AM_PrintError("->Error,you have reached max scans [20/20]\n");
            return -1;
        }
    }
    else{                       //den uparxei h energeia lathos xrhsth
        AM_PrintError("->Error,This operation does not exist you cannot scan file \n");
        return -1;
    }
};
/**********************************************************************************************************/

/******************************************|FIND SCAN|*****************************************************/
void *AM_FindNextEntry(int scanDesc)
{
    int i,op,blockn,length1,length2,t,total,hp_file_check,rec_in_block,curr_block;
    int check_scan_full = 0,curr_total_rec=0,A,B,curr,fileDesc,search=0;
    char t1,t2,meta[3];
    void *block,*va1[100],*va2[100];
    for(i=0;i<MAX_SCANS;i++){// bres to scan ston pinaka
        if(File_Scan_Array[i].scan_descriptor == scanDesc){
            check_scan_full = 1;
            break;
        }
    }
    if(check_scan_full==0){//den uparxei tetoio scan
        AM_PrintError("->Error no scan with ths descriptor exists\n");
        return NULL;
    }
    else{
        if(File_Scan_Array[scanDesc].scan_descriptor==scanDesc){
            scanDesc=scanDesc; //brethike kateuthian
        }
        else{
            for(i=0;i<MAX_SCANS;i++){
                if(File_Scan_Array[i].scan_descriptor==scanDesc){
                    scanDesc=i; //sarwse ton pinaka kai bres to scan
                    break;
                }
            }
        }
        blockn = File_Scan_Array[scanDesc].block ; // pare times apo 
        length1 = File_Scan_Array[scanDesc].length1 ;// pinaka
        length2 = File_Scan_Array[scanDesc].length2 ;//gia bohthia 
        A = File_Scan_Array[scanDesc].possition_rec_A ;//sthn euresh
        B = File_Scan_Array[scanDesc].possition_rec_B ;//scan
        op = File_Scan_Array[scanDesc].op;
        t1 = File_Scan_Array[scanDesc].type1;
        t2 = File_Scan_Array[scanDesc].type2;
        curr = File_Scan_Array[scanDesc].current_scan;
        total = File_Scan_Array[scanDesc].total_scan_rec;
        curr_block = File_Scan_Array[scanDesc].curr_block;
        fileDesc = File_Scan_Array[scanDesc].file_descriptor;
        rec_in_block = File_Scan_Array[scanDesc].rec_in_block;
        if(op==1){  //energeia isotitas
            printf("->---- EQUAL ----\n");
            if(pro_printing==1)
                printf("->Searching %ds from %d record ,EQUAL to value \n",curr+1,total+1);
            hp_file_check=BF_ReadBlock(fileDesc,blockn,&block);  //diabasma tou block [0]
            if(hp_file_check<0){                      //Elegxos gia sfalma{
                    AM_PrintError("->Error Reading info block\n");
                    BF_PrintError("->Error Reading info block\n");
                    return NULL;
            }
            if(pro_printing==1)
                printf("->Searching from block number [%d] \n",blockn);
            if(File_Scan_Array[scanDesc].current_scan<total){ //oso eimaste sto clock
               File_Scan_Array[scanDesc].current_scan = File_Scan_Array[scanDesc].current_scan +1; 
               memcpy(&va1,block+record_metadata+curr*(length1+length2)+length1,length2);
               printf("->");                                //ektupwse record
               AM_Void_Printing2(va1,scanDesc);printf("\n");
            }
            else {//alliws teleiwse to block kai den 
                return NULL;//exoume kati allo na kanoume 
            }
        }
        else if(op==2){//oxi isotita
            printf("->---- NOT EQUAL ----\n");
            if(pro_printing==1)
                printf("->Searching record ,NOT EQUAL to value \n");
            if(curr<1){ //pigene sto pio aristero block
                while(search == 0){//anazitisi
                    hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
                    if(hp_file_check<0){                                         //Elegxos gia sfalma
                            AM_PrintError("->Error Reading info block\n");
                            BF_PrintError("->Error Reading info block\n");
                            return NULL;
                    }
                    memcpy(&meta,block,sizeof("Ind"));
                    if(strcmp(meta,"Ind")==0){ //eimast se index
                        memcpy(&curr_block,block +index_metadata ,sizeof(int));
                    }                           //pigene sto epomeno block
                    else{
                        search = 1;//eimaste se record ,telos anazitisis
                        File_Scan_Array[scanDesc].curr_block = curr_block;
                    }
                }
            }
            hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
            if(hp_file_check<0){                         //Elegxos gia sfalma
                    AM_PrintError("->Error Reading info block\n");
                    BF_PrintError("->Error Reading info block\n");
                    return NULL;
            } //pernoume tis gia thn sugkrisi meta
            memcpy(&va1,block+record_metadata+(curr)*(length1+length2)+length1,length2);
            memcpy(&rec_in_block,block+record_counter,sizeof(int));
            if(curr_block!=blockn){ //allo block apo isotita
                printf("->");       //ektupwsh records
                AM_Void_Printing2(va1,scanDesc);printf(" ,block %d\n",curr_block);
            }
            else{       //eimaste sto block me isotita,elegxos gia isotita
                memcpy(&va2,block+record_metadata+(A-1)*(length1+length2)+length1,length2);
                if(t1 == 'c'){          //eimaste se string
                    if(strcmp((char*)va1,(char*)va2)==0){
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);                  
                    }
                }
                else if(t1 =='i'){      //eimaste se int
                    if((*(int*)va1)!=(*(int*)va2)){
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);                  
                    }
                }
                else{                   //eimaste se float
                    if((*(float*)va1)!=(*(float*)va2)){
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);                  
                    }
                }
            }
            if(File_Scan_Array[scanDesc].current_scan>=rec_in_block){//an xwrame sto block
                memcpy(&curr_block,block+record_next,sizeof(int));
                if(curr_block == -1){ //sarwsame ola ta block
                    return NULL;
                }
                File_Scan_Array[scanDesc].curr_block = curr_block;
                File_Scan_Array[scanDesc].current_scan = 1; //epomeno block
            }
            else
                File_Scan_Array[scanDesc].current_scan = curr +1; //idio block
        }
        else if(op==3){ //energeia mikroterou
            printf("->---- LESS THAN ----\n");
            if(pro_printing==1)
                printf("->Searching record ,LESS THAN to value \n");
            if(curr<1){
                while(search == 0){         //anazitisi 
                    hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
                    if(hp_file_check<0){                                         //Elegxos gia sfalma
                            AM_PrintError("->Error Reading info block\n");
                            BF_PrintError("->Error Reading info block\n");  
                            return NULL;
                    }
                    memcpy(&meta,block,sizeof("Ind"));
                    if(strcmp(meta,"Ind")==0){      //eimaste se index
                        memcpy(&curr_block,block +index_metadata ,sizeof(int));
                    }
                    else{
                        search = 1;     //eimaste se record
                        File_Scan_Array[scanDesc].curr_block = curr_block;
                    }
                }
            }
            hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
            if(hp_file_check<0){                                         //Elegxos gia sfalma
                    AM_PrintError("->Error Reading info block\n");
                    BF_PrintError("->Error Reading info block\n");
                    return NULL;
            }//times gia na broume thn isotita na stamatisoume
            memcpy(&va1,block+record_metadata+(curr)*(length1+length2)+length1,length2);
            memcpy(&rec_in_block,block+record_counter,sizeof(int));
            if(curr_block!=blockn){ //oxi block me isotita
                printf("->");       //ektupwse record
                AM_Void_Printing2(va1,scanDesc);printf(" ,block %d\n",curr_block);
            }
            else{   //isotita,pare thn timh isotitas
                memcpy(&va2,block+record_metadata+(A-1)*(length1+length2)+length1,length2);//printf("VVVVV %s\n",((char*)va2));
                if(t1 == 'c'){  //eimaste se string
                    if(strcmp((char*)va1,(char*)va2)==0){
                        return NULL;                  
                    }
                    else{
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);
                    }
                }
                else if(t1 =='i'){//eimaste se int
                    if((*(int*)va1)==(*(int*)va2)){
                        return NULL;                  
                    }
                    else{
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);
                    }
                }
                else{       //eimaste se float
                     if((*(float*)va1)==(*(float*)va2)){
                        return NULL;                  
                    }
                    else{
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);
                    }
                }
            }
            if(File_Scan_Array[scanDesc].current_scan>=rec_in_block){//den xwrame se block
                memcpy(&curr_block,block+record_next,sizeof(int));
                File_Scan_Array[scanDesc].curr_block = curr_block;
                File_Scan_Array[scanDesc].current_scan = 1;     //epomeno block
            }
            else        
                File_Scan_Array[scanDesc].current_scan = curr +1; //xwrame 
        }
        else if(op==4){ //energeia megalitero
            printf("->---- GREATER THAN ----\n");getchar();
            curr_block = File_Scan_Array[scanDesc].block ;//arxikopoihsh
            if(curr_block == -1){ //den exoume record
                return NULL;
            }
            hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
            if(hp_file_check<0)                                         //Elegxos gia sfalma
            {
                    AM_PrintError("->Error Reading info block\n");
                    BF_PrintError("->Error Reading info block\n");
                    return NULL;
            }
            if(curr<1){     //teleutaia thesi isotitas
                curr = B;
            }
            memcpy(&rec_in_block,block+record_counter,sizeof(int));//records in block
            if(File_Scan_Array[scanDesc].current_scan<rec_in_block){//an xwrame 
                memcpy(&va1,block+record_metadata+(curr)*(length1+length2)+length1,length2);
                printf("->");           //ektupwse
                AM_Void_Printing2(va1,scanDesc);
                printf(" ,block %d\n",curr_block);
                File_Scan_Array[scanDesc].current_scan = curr +1; 
            }
            else{   //den xwrame,epomeno block
                memcpy(&curr_block,block+record_next,sizeof(int));
                File_Scan_Array[scanDesc].block = curr_block;           
                File_Scan_Array[scanDesc].current_scan = 1; 
            }
        }
        else if(op==5){//energeia mikrotero h iso
            printf("->---- LESS THAN OR EQUAL ----\n");
            if(pro_printing==1)
                printf("->Searching record ,LESS THAN to value \n");
            if(curr<1){//omoio me mikrotero apo prin,diafora katw
                while(search == 0){
                    hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
                    if(hp_file_check<0){                                         //Elegxos gia sfalma
                            AM_PrintError("->Error Reading info block\n");
                            BF_PrintError("->Error Reading info block\n");
                            return NULL;
                    }
                    memcpy(&meta,block,sizeof("Ind"));
                    if(strcmp(meta,"Ind")==0){
                        memcpy(&curr_block,block +index_metadata ,sizeof(int));
                    }
                    else{
                        search = 1;
                        File_Scan_Array[scanDesc].curr_block = curr_block;
                    }
                }
            }
            hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
            if(hp_file_check<0){                                         //Elegxos gia sfalma
                AM_PrintError("->Error Reading info block\n");
                BF_PrintError("->Error Reading info block\n");
                return NULL;
            }
            memcpy(&va1,block+record_metadata+(curr)*(length1+length2)+length1,length2);
            memcpy(&rec_in_block,block+record_counter,sizeof(int));
            if(curr_block!=blockn){
                printf("->");
                AM_Void_Printing2(va1,scanDesc);
                printf(" ,block %d\n",curr_block);
                
            }
            else{
                memcpy(&va2,block+record_metadata+(A-1)*(length1+length2)+length1,length2);//printf("VVVVV %s\n",((char*)va2));
                if(t1 == 'c'){
                    if(strcmp((char*)va1,(char*)va2)==0){ //kai thn isotita ektupwnoume
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);
                    }
                }
                else if(t1 =='i'){
                    if((*(int*)va1)<=(*(int*)va2)){ //kai thn isotita ektupwnoume
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);
                    }
                    else{
                        return NULL;                  
                    }
                }
                else{
                    if((*(float*)va1)<=(*(float*)va2)){ //kai thn isotita ektupwnoume
                        printf("->");
                        AM_Void_Printing2(va1,scanDesc);
                        printf(" ,block %d\n",curr_block);
                    }
                }
            }
            if(File_Scan_Array[scanDesc].current_scan>=rec_in_block){
                memcpy(&curr_block,block+record_next,sizeof(int));
                File_Scan_Array[scanDesc].curr_block = curr_block;
                File_Scan_Array[scanDesc].current_scan = 1; 
            }
            else
                File_Scan_Array[scanDesc].current_scan = curr +1; 
        }
        else{   //energeia megalutero h iso
        getchar();
            printf("->---- GREATER THAN OR EQUAL ----\n");
            curr_block = File_Scan_Array[scanDesc].block ;
            if(curr_block == -1){
                return NULL;
            }   //omoio me megalutero,diafora katw
            hp_file_check=BF_ReadBlock(fileDesc,curr_block,&block);  //diabasma tou block [0]
            if(hp_file_check<0)                                         //Elegxos gia sfalma
            {
                AM_PrintError("->Error Reading info block\n");
                BF_PrintError("->Error Reading info block\n");
                return NULL;
            }
            if(curr<1){ //pernoume thn thesi tou prwtou isou
                curr = A; //gia na ektupwsoume ta iso
            }
            memcpy(&rec_in_block,block+record_counter,sizeof(int));
            if(File_Scan_Array[scanDesc].current_scan<rec_in_block){
                memcpy(&va1,block+record_metadata+(curr)*(length1+length2)+length1,length2);
                printf("->");
                AM_Void_Printing2(va1,scanDesc);
                printf(" ,block %d\n",curr_block);
                File_Scan_Array[scanDesc].current_scan = curr +1; 
            }
            else{
                memcpy(&curr_block,block+record_next,sizeof(int));
                File_Scan_Array[scanDesc].block = curr_block;           
                File_Scan_Array[scanDesc].current_scan = 1; 
            }
        }
    }
};
/**********************************************************************************************************/

/******************************************|CLOSE SCAN|****************************************************/
int AM_CloseIndexScan(int scanDesc)
{
    int hp_file_close;
    int i,AM_errno=7,check_scan_file = 0;
    for(i=0;i<MAX_SCANS;i++){       //elenxos gia euresh scan
        if(File_Scan_Array[i].scan_descriptor==scanDesc){//brethike
            check_scan_file = 1;
            break;
        }
    }
    if(check_scan_file == 0){ //den uparxei to scan
        AM_PrintError("->Error closing scanning on B+ Three file ,it doesn't exist scan ...\n");
        return -1;
    }
    printf("->Closing scan [%d] on B+ Tree file ...\n",scanDesc);
    File_Scan_Array[i].scan_descriptor = -1;//midenismos stoixeiwn
    File_Scan_Array[i].file_descriptor = -1;
    free(File_Scan_Array[i].filename);
    return AME_OK;
};
/**********************************************************************************************************/

/******************************************|ERROR PRINTING|****************************************************/
void AM_PrintError(char *errString)
{
    printf("->\n->!!!-------------AN ERROR HAS OCCURED-------------!!!\n");
    switch (AM_errno){      //ektupwsh sfalmatwn ana sunarthsh
        case 0 :
                printf("->  **ERROR IN CREATING FILES!!\n");    //create index
                printf("->      -%s",errString);
                break;
        case 1 :
                printf("->  **ERROR IN DELETING FILES!!\n");    //destroy index
                printf("      -%s",errString);
                break;
        case 2 :
                printf("->  **ERROR IN OPENING FILES!!\n");     //open index
                printf("      -%s",errString);
                break;
        case 3 :
                printf("->  **ERROR IN CLOSING FILES!!\n");     //close index
                printf("     -%s",errString);
                  break;
        case 4 :
                printf("->  **ERROR IN INSERING RECORD\n");
                printf("%s",errString);
                break;
        case 5 :
                printf("->  **ERROR IN OPENNING SCANNING\n");   //open scan
                printf("%s",errString);
                break;
        case 6 :
                printf("->  **ERROR IN FIND NEXT RECORD \n");
                printf("%s",errString);
                break;
        case 7 :
                printf("->  **ERROR IN CLOSING SCANNING \n");   //close scan
                printf("%s",errString);
                break;
        default:
                printf("->No error occured,everything is fine...\n");
    }
    printf("->!!!----------------------------------------------!!!\n->\n");
};
/**************************************************************************************************************/
