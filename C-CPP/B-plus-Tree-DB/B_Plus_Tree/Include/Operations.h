#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#define MAX_FILES 20
#define MAX_SCANS 20
/*--------------domh open file ----------------*/
typedef struct File_Information
{
    int file_descriptor;
    char *filename;
    char type_key1;
    char type_key2;
    int length_key1;
    int length_key2;
    int rec_in_block;
    int index_in_block;
    int index_pointers_in_block;
    int total_rec;
    int root;
    int block_num;
}File_Information;

extern File_Information File_Information_Array[MAX_FILES];

/*--------------domh scan file ----------------*/
typedef struct File_Scan
{
    int file_descriptor;
    int scan_descriptor;
    int op;
    int block;
    int curr_block;
    int curr_rec;
    int current_scan;
    int total_scan_rec;
    int possition_rec_A;
    int possition_rec_B;
    int length1;
    int length2;
    char type1;
    char type2;
    char *filename;
    int rec_in_block;

}File_Scan;

extern File_Scan File_Scan_Array[MAX_SCANS];

typedef struct Value_Array
{

	int integer;
	float floating;
	char *nstring;

}Value_Array;
//eisagwgh ston pinaka 
int AM_Check_File_Info_Capacity(int filedis,char *fileName);
//ektupwsh
void AM_File_Scan_Status(void);

void AM_File_Info_Status(void);
//dimiourgia blocks
int AM_Create_Index_Block(int,void*,int);

int AM_Create_Record_Block(int,void*,void*,int,int);
//sugkrish timwn
int AM_Compare_Values(void*,int,char*,int,int,int);
//eisagwgh sto scan pinaka
int AM_Print_Info_Scan_Insert(int,int,void*);
//taksinomish
int AM_Sorting_Record(int,int,int,char*,int);

int AM_Sorting_Index(int,int,int,char*,int);
//ektupwsh timwn
void AM_Void_Printing(void *,int);

void AM_Void_Printing2(void *,int);

#endif
