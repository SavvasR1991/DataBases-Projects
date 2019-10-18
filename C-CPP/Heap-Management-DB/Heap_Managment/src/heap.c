#include "BF.h"
#include "heap.h"
int total_records;                                            //global metablith gia arithmo eggrafwn

int HP_CreateFile(char *fileName)
{/**Dhmiourgei to arxeio BF,thn bash dedomenwn**/
  // Add your code here                                       //Metablites
  int hp_file_check,one = 0;                                          //apotelesma apo BF sunarthseis gia lathos
  int hp_file_initialation;                                   //apotelesma reumatos apo anoigma BF arxeiou
  void *block;                                                //metablhth gia to block

  hp_file_check=BF_CreateFile(fileName);                      //Dhmiourgia BF arxeiou filename
  if(hp_file_check<0)                                  //Elegxos sfalmatos
  {
        printf("->Error Creating file \n->   *path:\"%s\"\n",fileName);
        return -1;                                            //epistrofh sthn main logo sfalmatos
  }
  return 1;                                //epituxia
}

int HP_OpenFile(char *fileName)
{/**Anoigei to BF arxeio**/
  // Add your code here                                   //Metablhtes
  int one=0;                                             //Mhdenikh metablhth
  int hp_file_open;                                       //Timh reumatos tou arxeiou
  int hp_file_check,hp_file_initialation;                 //Elegxos sfalmatos
  char *heap_index="Column-store";                            //anagnwristiko arxeioy swrou
  void *block;                                            //metablhth block
  printf("->Opening Column-store file :\"%s\"\n",fileName);
  hp_file_initialation=BF_OpenFile(fileName);                 //anoigma BF arxeiou filename
  if(hp_file_initialation<0)                                  //Elegxos sfalmatos
  {
        printf("->Error Opening file \n->   *path:\"%s\"\n",fileName);
        return -1;                                            //epistrofh sthn main logo sfalmatos
  }
  hp_file_check=BF_AllocateBlock(hp_file_initialation);       //desmeush arxikou block (0)
  if(hp_file_check<0){                                        //Sfalma desmeushs mnhmhs gia block
        printf("->Error Allocating new block\n");
        return -1;                                            //epistrofh sthn main logo sfalmatos
  }
  hp_file_check=BF_ReadBlock(hp_file_initialation,0,&block);  //diabasma tou block [0]
  if(hp_file_check<0)                                         //Elegxos gia sfalma
  {
        printf("->Error Reading new block\n");
        return -1;                                            //Epistrofh sthn main logo sfalmatos
  }
  memcpy(block,heap_index,sizeof("Column-store"));            //Grapse sthn arxh tou block[0] thn leksh "heap"gia anagnwristiko swrou
  hp_file_check=BF_WriteBlock(hp_file_initialation,0);        //Grapse to block[0] sthn mhnmh
  if(hp_file_check<0)                                         //Elegxos gia sfalma
  {
        printf("->Error Writing new block\n");
        return -1;                                            //epistrofh sthn main logo sfalmatos
  }
  hp_file_check=BF_AllocateBlock(hp_file_initialation);       //desmeush arxikou block (0)
  if(hp_file_check<0){                                        //Sfalma desmeushs mnhmhs gia block
        printf("->Error Allocating new block\n");
        return -1;                                            //epistrofh sthn main logo sfalmatos
  }
  hp_file_check=BF_ReadBlock(hp_file_initialation,1,&block);  //diabasma tou block [0]
  if(hp_file_check<0)                                         //Elegxos gia sfalma
  {
        printf("->Error Reading new block\n");
        return -1;                                            //Epistrofh sthn main logo sfalmatos
  }
  memcpy(block,&one,sizeof(int));                             //Grapse sthn arxh tou block[0] thn leksh "heap"gia anagnwristiko swrou
  hp_file_check=BF_WriteBlock(hp_file_initialation,1);        //Grapse to block[0] sthn mhnmh
  if(hp_file_check<0)                                         //Elegxos gia sfalma
  {
        printf("->Error Writing new block\n");
        return -1;                                            //epistrofh sthn main logo sfalmatos
  }
  return hp_file_initialation;                                    //epistrofh to anagnwristikou arxeioy swrou
}

int HP_CloseFile(int fileDesc)
{/**Kleinei to BF arxeiou**/
  // Add your code here
  int hp_file_close;
  hp_file_close=BF_CloseFile(fileDesc);          //Kleise to arxeio BF
  if(hp_file_close<0)                            //elegxos sfalmatos
  {
        printf("->Error closing Column-store file...\n");
        return -1;                               //sfalma
  }
  else                                           //epituxhs klhsimatos
  {
        printf("->Column-store file is closed...\n");
        return 0;
  }
}

int HP_InsertEntry(int fileDesc,char* record,int type,long unsigned int length)
{//Eisagwgh neas eggrafhs sthn bash
	// Add your code here                               //Metablhtes
	int num_blocks;                                     //arithmos twn blocks sthn bash
	int check_error;                                    //elegxos lathous
	int one=1;                                          //monada
	int recs_in_a_block;                                //eggrafes sto block
	int current_num_rec,int_rec;                        //trexouses eggrafes sto block
	void *block;                                        //metablhth block
	recs_in_a_block=(BLOCK_SIZE-4)/(length);      //upologismos eggrafes ana block
	num_blocks=BF_GetBlockCounter(fileDesc)-1;          //Blocks se bash
	if(num_blocks<0)                                    //elegxos sfalmatos
	{
        printf("->Error no block to insert entry...\n");
        return -1;                                      //sfalma
	}
    check_error=BF_ReadBlock(fileDesc,num_blocks,&block);//diabaze to epomeno block
    if(check_error<0)                               //elenxos sfalmatwn
    {
        BF_PrintError("Error Reading block\n");
        return -1;                                  //sfalma
    }
    memcpy(&current_num_rec,block,sizeof(int));     //diabase metrhth eggrafwn
    if(current_num_rec<recs_in_a_block)             //an xwrane sto block
    {                                               //grapse thn eggrafh
        if(type==0){
            int_rec = atoi(record);
            memcpy(block+sizeof(int)+current_num_rec*length,&int_rec,sizeof(int));
        }
        else{
            memcpy(block+sizeof(int)+current_num_rec*length,record,length);

        }
        check_error=BF_WriteBlock(fileDesc,num_blocks);      //grapse sto disko to block
        if(check_error<0)                           //elegxos
        {
            BF_PrintError("Error Writting block\n");
            return -1;                              //sfalma
        }
        current_num_rec++;                              //gia nea eggrafh
        memcpy(block,&current_num_rec,sizeof(int));
    }
    else
    {
        check_error=BF_AllocateBlock(fileDesc);       //desmeuse mnhmh gia to block
        if(check_error<0)                             //elenxos sfalmatwn
        {
            printf("->Error Allocating new block..Database is full\n->Danger of Overflow...\n");
            return 2;                                 //sfalma
        }
        check_error=BF_ReadBlock(fileDesc,BF_GetBlockCounter(fileDesc)-1,&block);//diabase to block
        if(check_error<0)                              //elenxos sfalmatwn
        {
            BF_PrintError("Error Reading block\n");
            return -1;                                  //sfalma
        }
        current_num_rec=1;
        memcpy(block,&current_num_rec,sizeof(int));     //grapse sto disko
        if(type==0){
            int_rec = atoi(record);
            memcpy(block+sizeof(int),&int_rec,sizeof(int));
        }
        else{
            memcpy(block+sizeof(int),record,length);
        }
        check_error=BF_WriteBlock(fileDesc,BF_GetBlockCounter(fileDesc)-1);
        if(check_error<0)                               //elenxos sfalmatwn
        {
            BF_PrintError("Error Writting block\n");
            return -1;                                  //sfalma
        }
    }
    return 0;                                           //epituxia
}

void HP_PrintDatabase_Status(int fileDesc,int len)
{//Ektupwnei thn katastash ths basehs
    int num_block=BF_GetBlockCounter(fileDesc)-1;     //arithmos block sthn bash
    int check_error;                                  //elegxos sfalmatos
    int curr_recs;                                    //arithmos eggrafwn sto block
    int r;                                         //Record
    int rec_cap = BLOCK_SIZE/(len);        //eggrafh ana block
    void *block;                                      //block
    check_error=BF_ReadBlock(fileDesc,num_block,&block);//diabaze to block
    if(check_error<0)                                 //elenxos sfalmatwn//
    {
        BF_PrintError("Error Reading block\n");
    }

    memcpy(&curr_recs,block,sizeof(int));//ektupwsh dedomenwn
    printf("->                   _________________\n");
    printf("->                  | DATABASE STATUS |\n");
    printf("->-------------------------------------------------------|\n");
    printf("->[1] Total numbers of blocks  :%d Blocks\n",num_block+1);
    printf("->[2] Records in block saved   :%d Records\n",curr_recs);
    printf("->[3] Current block free space :%d %%\n",((rec_cap-curr_recs)*100)/rec_cap);
    printf("->[4] Total records in Database:%d Records\n",total_records);
    printf("->[5] Block size               :%d Bytes\n",BLOCK_SIZE);
    printf("->[6] Records in a block total :%d Records\n",BLOCK_SIZE/len);
    printf("->[7] Database size            :%lu Bytes\n",sizeof("Column-store")+total_records*sizeof(int)+total_records*len);
    printf("->-------------------------------------------------------|\n");
}
