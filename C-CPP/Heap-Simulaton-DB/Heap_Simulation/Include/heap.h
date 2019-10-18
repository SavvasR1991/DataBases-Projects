#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int total_records;

typedef struct Record {
	int id;
	char name[15];
	char surname[20];
	char city[20];
} Record;

int HP_CreateFile(char *fileName);

int HP_OpenFile(char *fileName);

int HP_CloseFile(int fileDesc);

int HP_InsertEntry(int fileDesc,Record record);

void HP_GetAllEntries(int fileDesc,char* fieldName,void *value);

void HP_PrintDatabase_Status();
