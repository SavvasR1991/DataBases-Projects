#include "BF.h"
#include "heap.h"
int total_records;                                            //Total records number

int HP_CreateFile(char *fileName)                              
{ //CREATE HEAP FILE             
  int hp_file_check;                                          
  int hp_file_initialation;                                   
  char *heap_index="heap";                                    
  void *block;                                                

  printf("->Initialition of block field...\n");
  BF_Init();                                                  //Initialize block
  printf("->Creating Heap file with name :\"%s\"\n",fileName);
  hp_file_check=BF_CreateFile(fileName);                      //BF file creation
  if(hp_file_initialation<0)                                  
  {
        printf("->Error Creating file :\"%s\"\n",fileName);
        return -1;                                            
  }
  hp_file_initialation=BF_OpenFile(fileName);                 //Open BF file
  if(hp_file_initialation<0)                                  
  {
        printf("->Error Opening file :\"%s\"\n",fileName);
        return -1;                                            
  }
  hp_file_check=BF_AllocateBlock(hp_file_initialation);       //Allocate block (0)
  if(hp_file_check<0){                                        
        printf("->Error Allocating new block\n");
        return -1;                                            
  }
  hp_file_check=BF_ReadBlock(hp_file_initialation,0,&block);  //Read block [0]
  if(hp_file_check<0)                                         
  {
        printf("->Error Reading new block\n");
        return -1;                                            
  }
  memcpy(block,heap_index,sizeof("heap"));                    //Write "heap" at block start
  hp_file_check=BF_WriteBlock(hp_file_initialation,0);        //Write block[0] to memory
  if(hp_file_check<0)                                         
  {
        printf("->Error Writing new block\n");
        return -1;                                            
  }
  printf("->Heap file \"%s\" is created succesfuly..\n",fileName);
  return 0;                                                   
}

int HP_OpenFile(char *fileName)
{/**OPEN HEAP FILE**/
  int zero=0;                                             
  int hp_file_open;                                       
  int hp_file_check;                                      
  char index[5];                                          
  void *block;                                            
  printf("->Opening heap file :\"%s\"\n",fileName);
  hp_file_open=BF_OpenFile(fileName);                     //Open heap file
  if(hp_file_open<0)                                      
  {
        printf("->Error Opening file :\"%s\"\n",fileName);
        return -1;                                        
  }
  hp_file_check=BF_ReadBlock(hp_file_open,0,&block);      //Read block[0]
  if(hp_file_check<0)                                     
  {
        printf("->Error Reading block...\n");
        return -1;                                        
  }
  memcpy(index,block,sizeof("heap"));                     //Check for "index" in file
  memcpy((char*)block+sizeof("heap"),&zero,sizeof(int));  //record counter block[0] to zero
  if(strcmp(index,"heap")!=0)                             //Check if index is "heap"
  {
        printf("->File \"%s\" is not a heap file...\n",fileName);
        return -1;                                        //no heap file
  }
  return hp_file_open;                                    //return index
}

int HP_CloseFile(int fileDesc)
{/**CLOSE BF file**/
  int hp_file_close;
  printf("->Closing the heap file...\n");
  hp_file_close=BF_CloseFile(fileDesc);          //close file BF
  if(hp_file_close<0)                           
  {
        printf("->Error closing heap file...\n");
        return -1;                               //error
  }
  else                                           
  {
        printf("->Heap file is closed...\n");
        return 0;
  }
}

int HP_InsertEntry(int fileDesc, Record record)
{/**INSERT NEW RECORD**/

	int num_blocks;                                     
	int check_error;                                    
	int one=1;                                          
	int recs_in_a_block;                                
	int current_num_rec;                                
	void *block;                                        
	recs_in_a_block=(BLOCK_SIZE)/(sizeof(record));      
	num_blocks=BF_GetBlockCounter(fileDesc)-1;          
	if(num_blocks<0)                                    // No block allocated, error
	{
        printf("->Error no block to insert entry...\n");
        return -1;                                      
	}
	if(num_blocks==0)                                   //block[0]
	{
        check_error=BF_ReadBlock(fileDesc,0,&block);    //read block
        if(check_error<0)                               
        {
            printf("->Error Reading block...\n");
            return -1;                                  
        }
        memcpy(&current_num_rec,block+sizeof("heap"),sizeof(int));//read total #records
        if(current_num_rec<recs_in_a_block)             
        {                                               
            memcpy(block+sizeof(int)+sizeof("heap")+current_num_rec*sizeof(Record),&record,sizeof(Record));
            current_num_rec++;                          //total #records plus one
            memcpy(block+sizeof("heap"),&current_num_rec,sizeof(int));
            check_error=BF_WriteBlock(fileDesc,0);      //Write block to memory
            if(check_error<0)                           
            {
                printf("->Error Writting block...\n");
                return -1;                              
            }
        }
        else                                            //block is full
        {
            check_error=BF_AllocateBlock(fileDesc);     //block new block
			if(check_error<0)                           
            {
                BF_PrintError("Error Allocating new block\n");
                return -1;                              
            }
            check_error=BF_ReadBlock(fileDesc,1,&block);//read block
            if(check_error<0)                           
            {
                BF_PrintError("Error Reading block\n");
                return -1;                             
            }
            memcpy(block,&one,sizeof(int));            
            memcpy(block+sizeof(int),&record,sizeof(Record));
            check_error=BF_WriteBlock(fileDesc,1);      //write block to memory
            if(check_error<0)
            {
                BF_PrintError("Error Writting block\n");
                return -1;                              
            }
        }
        total_records++;                                //++ total blocks
        return 0;                                       
	}
	else                                                
	{
        check_error=BF_ReadBlock(fileDesc,num_blocks,&block);//read next block
        if(check_error<0)                               
        {
            BF_PrintError("Error Reading block\n");
            return -1;                                  
        }
        memcpy(&current_num_rec,block,sizeof(int));     //read record counter
        if(current_num_rec<recs_in_a_block)             //if record fits block
        {                                               //write record
            memcpy(block+sizeof(int)+current_num_rec*sizeof(Record),&record,sizeof(Record));
            check_error=BF_WriteBlock(fileDesc,num_blocks);      
            if(check_error<0)                           
            {
                BF_PrintError("Error Writting block\n");
                return -1;                              
            }
            current_num_rec++;                              //new record
            memcpy(block,&current_num_rec,sizeof(int));
        }
        else
        {
            check_error=BF_AllocateBlock(fileDesc);       //allocate block
			if(check_error<0)                             
            {
                printf("->Error Allocating new block..Database is full\n->Danger of Overflow...\n");
                return 2;                                 
            }
            check_error=BF_ReadBlock(fileDesc,BF_GetBlockCounter(fileDesc)-1,&block);
            if(check_error<0)                              
            {
                BF_PrintError("Error Reading block\n");
                return -1;                                  
            }
            current_num_rec=1;
            memcpy(block,&current_num_rec,sizeof(int));     
            memcpy(block+sizeof(int),&record,sizeof(Record));
            check_error=BF_WriteBlock(fileDesc,BF_GetBlockCounter(fileDesc)-1);
            if(check_error<0)                                
            {
                BF_PrintError("Error Writting block\n");
                return -1;                                  
            }
        }
        total_records++;
        return 0;                                           
	}
}

void HP_GetAllEntries(int fileDesc, char* fieldName, void *value)
{ // GET ALL ENTRIES  
                                 
  int i;                                                  
  int j;
  int counter=0;                                          
  int nu_blocks;                                          
  int curr_recs;                                          
  int check_error;                                        
  char *value_pointer;                                    
  void *block;                                             
  Record rec;                                             
  if(value!=NULL)
  {
    value_pointer=(char*)value;                           
  }
  nu_blocks=BF_GetBlockCounter(fileDesc);                 //total blocks
  for(i=0;i<nu_blocks;i++)                                //check blocks
  {
        check_error=BF_ReadBlock(fileDesc,i,&block);      
        if(check_error<0)                                 
        {
            BF_PrintError("Error Reading block\n");
        }
        if(i==0)                                          //block[0]
        {
            memcpy(&curr_recs,block+sizeof("heap"),sizeof(int));
        }
        else                                              //other blocks
        {
            memcpy(&curr_recs,block,sizeof(int));

        }
        for(j=0;j<curr_recs;j++)                          //check all records
        {
            if(i==0)                              //block[0]
            {
                memcpy(&rec,block+sizeof("heap")+sizeof(int)+j*sizeof(Record),sizeof(Record));
            }
            else                                          //other blocks
            {
                memcpy(&rec,block+sizeof(int)+j*sizeof(Record),sizeof(Record));
            }
            if(strcmp(fieldName,"id")==0)                 //id
            {
                if(rec.id==atoi(value_pointer))           
                {
                    if(counter==0)                        
                    {
                        printf("->          Customers with Id :%d founded..\n",rec.id);
                    }
                    counter++;                            
                    printf("->[%3d] %s ",counter,rec.name);
                    printf("%s from ",rec.surname);
                    printf("%s ",rec.city);
                    printf("with id:");
                    printf(" %d\n",rec.id);
                }
            }
            if(strcmp(fieldName,"name")==0)               //name
            {
                if(strcmp(rec.name,value_pointer)==0)     
                {
                    if(counter==0)
                    {
                        printf("->          Customers with Name :%s founded..\n",rec.name);
                    }
                    counter++;
                    printf("->[%3d] %s ",counter,rec.name);
                    printf("%s from ",rec.surname);
                    printf("%s ",rec.city);
                    printf("with id:");
                    printf(" %d\n",rec.id);
                }
            }
            if(strcmp(fieldName,"surname")==0)            //surname
            {
                if(strcmp(rec.surname,value_pointer)==0)  
                {
                    if(counter==0)
                    {
                        printf("->          Customers with Surname :%s founded..\n",rec.surname);
                    }
                    counter++;
                    printf("->[%3d] %s ",counter,rec.name);
                    printf("%s from ",rec.surname);
                    printf("%s ",rec.city);
                    printf("with id:");
                    printf(" %d\n",rec.id);
                }
            }
            if(strcmp(fieldName,"city")==0)               //city
            {
                if(strcmp(rec.city,value_pointer)==0)     
                {
                    if(counter==0)
                    {
                        printf("->          Customers from City :%s founded..\n",rec.city);
                    }
                    counter++;
                    printf("->[%3d] %s ",counter,rec.name);
                    printf("%s from ",rec.surname);
                    printf("%s ",rec.city);
                    printf("with id:");
                    printf(" %d\n",rec.id);
                }
            }
            if(strcmp(fieldName,"all")==0)
            {
                counter++;
                printf("->[%3d] %s ",counter,rec.name);
                printf("%s from ",rec.surname);
                printf("%s ",rec.city);
                printf("with id:");
                printf(" %d\n",rec.id);
            }
        }
  }
  if(counter==0)                                             //Record not found
  {
        printf("->No Customer founded...\n");
  }
  else
  {
        printf("-> %d Customers founded...\n",counter);
  }
}


void HP_PrintDatabase_Status(int fileDesc)
{/**PRINT DATABASE STATUS**/
    int num_block=BF_GetBlockCounter(fileDesc)-1;    
    int check_error;                                 
    int curr_recs;                                    
    Record r;                                         
    int rec_cap = BLOCK_SIZE/(sizeof(Record));        
    void *block;                                      
   
    check_error=BF_ReadBlock(fileDesc,num_block,&block);//read  block
    if(check_error<0)                                 
    {
        BF_PrintError("Error Reading block\n");
    }
    if(num_block==0)                                  //block[0]
    {
        memcpy(&curr_recs,block+sizeof("heap"),sizeof(int));
    }
    else                                               //other block
    {
        memcpy(&curr_recs,block,sizeof(int));
    }                                                  
    printf("->                   _________________\n");
    printf("->                  | DATABASE STATUS |\n");
    printf("->-------------------------------------------------------|\n");
    printf("->[1] Total numbers of blocks  :%d Blocks\n",num_block+1);
    printf("->[2] Records in block saved   :%d Records\n",curr_recs);
    printf("->[3] Current block free space :%d %%\n",((rec_cap-curr_recs)*100)/rec_cap);
    printf("->[4] Total records in Database:%d Records\n",total_records);
    printf("->[5] Block size               :%d Bytes\n",BLOCK_SIZE);
    printf("->[6] Records in a block total :%lu Records\n",BLOCK_SIZE/sizeof(Record));
    printf("->[7] Database size            :%lu Bytes\n",sizeof("heap")+total_records*sizeof(Record)+total_records*sizeof(int));
    printf("->-------------------------------------------------------|\n");
}
