#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AM.h"
#include "defn.h"
#include "Operations.h"

File_Information File_Information_Array[MAX_FILES];
File_Scan File_Scan_Array[MAX_SCANS];
char errStr[200];
int AM_errno;
int AM_errno2;
int index_metadata;
int index_counter;
int index_parent;
int index_next;

int record_metadata;
int record_counter;
int record_parent;
int record_next;
int pro_printing;

char empName[40];
char empAge[40];
char empSal[40];
char dnameAge[40];

void insertEntries(int eNentry, int eAentry, int eSentry, int dAentry,
		char* ename, int eage, float esal, char* edname, int recid) {
	char errStr[140];
	if (AM_InsertEntry(eNentry, (void *) ename, &recid) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empName);
	}
        if (AM_InsertEntry(eAentry, (void *) &eage, ename) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empAge);
	}
	if (AM_InsertEntry(eSentry, (void *) &esal, ename) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empSal);
	}
	if (AM_InsertEntry(dAentry, (void *) edname, &eage) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", dnameAge);
	}
}

int main(int argc, char** argv) {
	int scan1;

	int eNentry;
	int eAentry;
	int eSentry;
	int dAentry;
	int i,l1,l2,l3,l4;
    float tem,k1,k2;
	int eage,end =0;
	float esal;
	char edname[40];
	char ename[40],temp[225];
	char fltname[40],t1,t2;
	char* cvalue = NULL;
    char filename [50],file_in[50],input[50],*pch,pch2[225];   //stdin user
	int recordid = 1;


	int* ivalue = NULL;
    strcpy(empName, "EMP-NAME");
	strcpy(empAge, "EMP-AGE");
	strcpy(empSal, "EMP-SAL");
	strcpy(dnameAge, "DNAME-AGE");
	strcpy(fltname, "EMP-FAULT");
    
	/********************************************************************************
	 *  Αρχικοποίηση των εσωτερικών δομών του λογισμικού των ΒΔ                     *
	 ********************************************************************************/
	AM_Init();
    printf("->                            _________________\n");
    printf("->                           |PROGRAMM COMMANDS|\n");
    printf("->-------------------------------------------------------------------------|\n");
    printf("->To load a test examble   : example                                       |\n");
    printf("->To Create File B+_Tree   : create [Name] [Type1] [Size1] [Type2] [Size2] |\n");
    printf("->To Open  File B+_Tree    : open [Filename]                               |\n");
    printf("->To Close open File       : close [Filename id]                           |\n");
    printf("->To Destroy File          : destroy [Filename]                            |\n");
    printf("->To insert a  records     : insert [Filename id] [Value1] [Value2]        |\n");
    printf("->To Open  Scanning        : sopen [Filename id] [Operation] [Value]       |\n");
    printf("->To Search Scanning       : search [Scanning]                             |\n");
    printf("->To Close Scanning        : sclose [Scanning id]                          |\n");
    printf("->To see Scanning status   : status scan                                   |\n");
    printf("->To see Opening  status   : status open                                   |\n");
    printf("->To see/not more options  : pro                                           |\n");
    printf("->To exit programm press   : exit                                          |\n");
    printf("->-------------------------------------------------------------------------|\n");
    while(end == 0)                                    //bronxos eisagwghs entolwn user
    {
        printf("->Insert your command...\n->");
        fgets(input,50,stdin);                  //stdin command
        pch = strtok (input," ,.-\n");          //analuse tis entoles
        while (pch != NULL){  
            if(strcmp(pch,"exit")==0){
                end = 1;
                break;
            }
            else if(strcmp(pch,"create")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Name of File insert...\n");
                    break;
                }
                strcpy(filename,pch);
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Type 1 insert...\n");
                    break;
                }
                if(strcmp(pch,"string")==0){t1 = 'c';}
                else if(strcmp(pch,"int")==0){t1 = 'i';}
                else if(strcmp(pch,"float")==0){t1 = 'f';}
                else{printf("->No valid type 1\n");break;}
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Length 1 insert...\n");
                    break;
                }
                l1 = atoi(pch);
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Type 2 insert...\n");
                    break;
                }
                if(strcmp(pch,"string")==0){t2 = 'c';}
                else if(strcmp(pch,"int")==0){t2 = 'i';}
                else if(strcmp(pch,"float")==0){t2 = 'f';}
                else{printf("->No valid type 2\n");break;}
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No length 2 insert...\n");
                    break;
                }
                l2 = atoi(pch);
                if (AM_CreateIndex(filename, t1,l1, t2,l2) != AME_OK) {
                sprintf(errStr, "Error in AM_CreateIndex called on %s \n", empName);}
                break;
            }
            else if(strcmp(pch,"open")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Filename insert...\n");
                    break;
                }
                if (i= AM_OpenIndex(pch) < 0) {
                sprintf(errStr, "Error in AM_OpenIndex called on %s \n", pch);}
                i=AM_Check_File_Info_Capacity(i,pch);
                break;
            }
            else if(strcmp(pch,"close")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL){
                    printf("->No File id insert...\n");
                    break;
                }
                l1 = atoi(pch);
                if (AM_CloseIndex(l1) != AME_OK) {
                    sprintf(errStr, "Error in AM_CloseIndex called on %s \n", dnameAge);
                    AM_PrintError(errStr);}
                    break;
            }
            else if(strcmp(pch,"destroy")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No File Name insert...\n");
                    break;
                }
                strcpy(filename,pch);
                if (AM_DestroyIndex(filename) != AME_OK) {
                    sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",filename);
                    AM_PrintError(errStr);	}
                break;
            }
            else if(strcmp(pch,"insert")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No File Id insert...\n");
                    break;
                }
                l1 = atoi(pch);
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Operation insert...\n");
                    break;
                }
                l3 = atoi(pch);
                k1 = atof(pch);
                strcpy(pch2,pch);
                pch = strtok (NULL, ",-\n");
                if(pch==NULL)
                {
                    printf("->No Value insert...\n");
                    break;
                }
                l4 = atoi(pch);
                k2 = atof(pch);
                for(i=0;i<MAX_FILES;i++){
                    if(File_Information_Array[i].file_descriptor==l1){
                        if(File_Information_Array[i].type_key1=='c'){
                            if(File_Information_Array[i].type_key2=='c'){
                                if (AM_InsertEntry(l1, (void *)pch2,(void *)pch)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            }
                            else if(File_Information_Array[i].type_key2=='i'){
                                if (AM_InsertEntry(l1, (void *)pch2,(void *)&l4)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            }
                            else{
                                if (AM_InsertEntry(l1, (void *)pch2,(void *)&k2)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            }
                        }
                        if(File_Information_Array[i].type_key1=='i'){
                            if(File_Information_Array[i].type_key2=='c'){
                                if (AM_InsertEntry(l1, (void *)&l3,(void *)pch)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            }
                            else if(File_Information_Array[i].type_key2=='i'){
                                if (AM_InsertEntry(l1, (void *)&l3,(void *)&l4)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            }
                            else{
                                if (AM_InsertEntry(l1, (void *)&l3,(void *)&k2)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            
                        
                            }
                            
                        }
                        if(File_Information_Array[i].type_key1=='f'){
                            if(File_Information_Array[i].type_key2=='c'){
                                if (AM_InsertEntry(l1, (void *)&k1,(void *)pch)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            }
                            else if(File_Information_Array[i].type_key2=='i'){
                                if (AM_InsertEntry(l1, (void *)&k1,(void *)&l4)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            }
                            else{
                                if (AM_InsertEntry(l1, (void *)&k1,(void *)&k2)!= AME_OK) {
                                sprintf(errStr, "Error in AM_InsertEntry called on %s \n", filename);}
                            }
                        }
                        l1 = -10;
                        break;
                    }
                }
                if(l1!=-10)
                printf("->File Open does not exists");
                break;
            }
            else if(strcmp(pch,"sopen")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No File Id insert...\n");
                    break;
                }
                l1 = atoi(pch);
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Operation insert...\n");
                    break;
                }
                l2 = atoi(pch);
                pch = strtok (NULL, ",-\n");
                if(pch==NULL)
                {
                    printf("->No Value insert...\n");
                    break;
                }
                strcpy(temp,pch);
                l3 = atoi(pch);
                tem = atof(pch);
                for(i=0;i<MAX_FILES;i++){
                    if(File_Information_Array[i].file_descriptor==l1){
                        if(File_Information_Array[i].type_key1=='c')
                            if (AM_OpenIndexScan(l1,l2,(void*)temp) != AME_OK) {
                                sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",scan1);
                                AM_PrintError(errStr);
                            }
                        if(File_Information_Array[i].type_key1=='i')
                            if (AM_OpenIndexScan(l1,l2,(void*)&l3) != AME_OK) {
                                sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",scan1);
                                AM_PrintError(errStr);
                            }
                        if(File_Information_Array[i].type_key1=='f')
                            if (AM_OpenIndexScan(l1,l2,(void*)&tem) != AME_OK) {
                                sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",scan1);
                                AM_PrintError(errStr);
                            }
                        l1 = -10;
                        break;
                    }
                }
                if(l1!=-10)
                printf("->Scan does not exists");
                break;


            }
            else if(strcmp(pch,"search")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No File Name insert...\n");
                    break;
                }
                l1 = atoi(pch);
                while(AM_FindNextEntry(l1)!=NULL);
                break;
            }
            else if(strcmp(pch,"sclose")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No Scan insert...\n");
                    break;
                }
                l1 = atoi(pch);
                if (AM_CloseIndexScan(l1) != AME_OK) {
                    sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",scan1);
                AM_PrintError(errStr);
                }
                break;
            }
            else if(strcmp(pch,"pro")==0){
                if(pro_printing == 0)pro_printing=1;
                else pro_printing=0;
                break;
            }
            else if(strcmp(pch,"status")==0){
                pch = strtok (NULL, " ,-\n");
                if(pch==NULL)
                {
                    printf("->No element...\n");
                    break;
                }
                if(strcmp(pch,"scan")==0){
                    AM_File_Scan_Status();
                }else if(strcmp(pch,"open")==0){
                    AM_File_Info_Status();
                }else{
                    printf("->Invalid input\n");
                    break;
                }
                break;
            }
            else if(strcmp(pch,"example")==0){
                if (AM_CreateIndex(empName, STRING, sizeof(empName) - 1, INTEGER,sizeof(int)) != AME_OK) {
                    sprintf(errStr, "Error in AM_CreateIndex called on %s \n", empName);
                    AM_PrintError(errStr);
                }
                if (AM_CreateIndex(empAge, INTEGER, sizeof(int), STRING, 39) != AME_OK) {
                    sprintf(errStr, "Error in AM_CreateIndex called on %s \n", empAge);
                    AM_PrintError(errStr);
                }
                if (AM_CreateIndex(empSal, FLOAT, sizeof(float), STRING, 39) != AME_OK) {
                    sprintf(errStr, "Error in AM_CreateIndex called on %s \n", empSal);
                    AM_PrintError(errStr);
                }
                if (AM_CreateIndex(fltname, FLOAT, 39, STRING, 39) != AME_OK) {
                    sprintf(errStr, "Expected error in AM_CreateIndex called on %s \n",
                            fltname);
                    AM_PrintError(errStr);
                } else {
                    sprintf(errStr, "Creating %s index should have failed \n", fltname);
                    AM_PrintError(errStr);
                }

                if (AM_CreateIndex(dnameAge, STRING, 10, INTEGER, sizeof(int)) != AME_OK) {
                    sprintf(errStr, "Error in AM_CreateIndex called on %s \n", dnameAge);
                    AM_PrintError(errStr);
                }

                /********************************************************************************
                 *  ¶νοιγμα των τεσσάρων ΑΚ (με σκοπό την εισαγωγή εγγραφών)                    *
                 ********************************************************************************/

                if ((eNentry = AM_OpenIndex(empName)) < 0) {
                    sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empName);
                }
                i=AM_Check_File_Info_Capacity(eNentry,empName);
                if ((eAentry = AM_OpenIndex(empAge)) < 0) {
                    sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empAge);	}
                i=AM_Check_File_Info_Capacity(eAentry,empAge);

                if ((eSentry = AM_OpenIndex(empSal)) < 0) {
                    sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empSal);
                }
                i=AM_Check_File_Info_Capacity(eSentry,empSal);

                if ((dAentry = AM_OpenIndex(dnameAge)) < 0) {
                    sprintf(errStr, "Error in AM_OpenIndex called on %s \n", dnameAge);
                }
                i=AM_Check_File_Info_Capacity(dAentry,dnameAge);
                /********************************************************************************
                 *  Εισαγωγή των δυάδων (τιμή1, τιμή2) στα ΑΚ. Υπάρχουν 100 συνολικά εισαγωγές    *
                 *  σε κάθε αρχείο, οι οποίες διαχωρίζονται με παύλες για δική σας ευκολία         *
                 ********************************************************************************/

                /* -----------------------------------------------------------------------------*/
                recordid = 0;

                strcpy(ename, "XRHSTOFORIDHS ANTWNHS");
                eage = 25;
                esal = 155.5;
                strcpy(edname, "GALATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PARAGWGOY MARIA");
                eage = 30;
                esal = 208.9;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "IWAKEIMIDHS ELEYUERIOS");
                eage = 49;
                esal = 255.1;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "SYRIANOS ARHS");
                eage = 59;
                esal = 306.9;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KYRIAKIDHS GRHGORIOS");
                eage = 27;
                esal = 448.1;
                strcpy(edname, "PSWMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "XANTZHKYRIAKOS ANTWNIOS");
                eage = 69;
                esal = 357.5;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "APELPISIADOY IWSHFINA");
                eage = 21;
                esal = 400.0;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PANTAZHS LEYTERHS");
                eage = 19;
                esal = 367.6;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KOYROPETROGLOY KWSTAS");
                eage = 33;
                esal = 315.8;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "TSAXPINH EYLAMPIA");
                eage = 42;
                esal = 267.9;
                strcpy(edname, "SAPOUNIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "TRIGWNIDHS PYQAGORAS");
                eage = 51;
                esal = 218.9;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PISIDOY ANTIGONH");
                eage = 61;
                esal = 169.1;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);
                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ROYBAS APOSTOLOS");
                eage = 25;
                esal = 175.2;
                strcpy(edname, "GLYKA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "APOSBOLHS ANDREAS");
                eage = 36;
                esal = 227.2;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ASTRONOMOGLOY ISAAK");
                eage = 42;
                esal = 278.2;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "MOLOTOFIDHS AYGOYSTHS");
                eage = 53;
                esal = 328.5;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KYTTAROGLOY RODOYLA");
                eage = 65;
                esal = 378.5;
                strcpy(edname, "SAPOUNIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "MPALADOROS PETROS");
                eage = 22;
                esal = 428.1;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "OIKONOMIDHS ALKHS");
                eage = 34;
                esal = 478.8;
                strcpy(edname, "GLYKA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ARTEMIADH PETROYLA");
                eage = 19;
                esal = 436.5;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PANTELIADOY ELENH");
                eage = 44;
                esal = 387.1;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PERISTERIADH ANNA");
                eage = 52;
                esal = 337.1;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "GEWRGAKOPOYLOS EYQYMIOS");
                eage = 64;
                esal = 286.7;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PANAGIWTOPOYLOY QEODWRA");
                eage = 23;
                esal = 238.0;
                strcpy(edname, "LAXANIKA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "LASKARIDHS SWKRATHS");
                eage = 36;
                esal = 189.2;
                strcpy(edname, "PSARIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ZWGRAFOY ALEXANDRA");
                eage = 19;
                esal = 198.2;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "NIKOLAKOPOYLOS IWANNHS");
                eage = 41;
                esal = 248.6;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "STAMOS IASWNAS");
                eage = 56;
                esal = 295.5;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "TERZOPOYLOY ANASTASIA");
                eage = 65;
                esal = 347.1;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "AFENTOYLIDHS AXILLEAS");
                eage = 23;
                esal = 398.1;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ANASTASIADHS ALEXANDROS");
                eage = 36;
                esal = 449.2;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "BELIBASSAKH AIMILIA");
                eage = 44;
                esal = 458.8;
                strcpy(edname, "GLYKA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "XRISTODOYLIDOY ARISTEA");
                eage = 56;
                esal = 406.9;
                strcpy(edname, "PSWMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "MARKOY ANTWNIOS");
                eage = 69;
                esal = 359.9;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "EYAGGELOY LEWNIDAS");
                eage = 70;
                esal = 308.9;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "SPAQARAKH SOFIA");
                eage = 18;
                esal = 257.8;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "STEQANAKHS APOSTOLOS");
                eage = 19;
                esal = 205.6;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PARASKEYOPOYLOS ARGYRHS");
                eage = 18;
                esal = 157.9;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ANTWNOPOYLOY ELISABET");
                eage = 22;
                esal = 169.0;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "GEWRGIADOY MARIA");
                eage = 33;
                esal = 217.0;
                strcpy(edname, "KRASIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "SKLABOY MARQA");
                eage = 45;
                esal = 268.1;
                strcpy(edname, "PSARIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "BASLEIADHS ARISTARXOS");
                eage = 56;
                esal = 317.8;
                strcpy(edname, "PSARIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);
                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "SEREMETHS ANASTASIOS");
                eage = 63;
                esal = 368.8;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "STAMATIADOY ARTEMIS");
                eage = 18;
                esal = 415.1;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KONTOGIANNIDHS XRHSTOS");
                eage = 27;
                esal = 468.1;
                strcpy(edname, "LAXANIKA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "WRAIOPOYLOS KVNSTANTINOS");
                eage = 30;
                esal = 479.5;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ELEYQERIADHS MARKOS");
                eage = 49;
                esal = 428.1;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ADAMOPOYLOS ANTWNIOS");
                eage = 57;
                esal = 377.0;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "EYSTAQIOY XRISTINA");
                eage = 63;
                esal = 328.1;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "MARKOGIANNAKHS EYSTAQIOS");
                eage = 71;
                esal = 279.4;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KOKKINOS ARISTOTELHS");
                eage = 28;
                esal = 228.4;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ZORMPAS KVNSTANTINOS");
                eage = 33;
                esal = 177.1;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "POLYKANDRIWTHS DHMHTRIOS");
                eage = 41;
                esal = 126.9;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "MANWLIKIDHS ZHSHS");
                eage = 58;
                esal = 186.9;
                strcpy(edname, "PSWMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ZENEMPISHS IAKWBOS");
                eage = 69;
                esal = 237.9;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KALOGHROY ALKMHNH");
                eage = 19;
                esal = 288.2;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "QEOTOKAS PARASKEYAS");
                eage = 25;
                esal = 337.9;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KANARH EYQALEIA");
                eage = 38;
                esal = 385.1;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "LYRISTHS EYSTAQIOS");
                eage = 18;
                esal = 438.9;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "TREMPELHS ALKHBIADHS");
                eage = 40;
                esal = 486.2;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ARGYRIADHS ANASTASIOS");
                eage = 52;
                esal = 447.9;
                strcpy(edname, "KRASIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "QWMADAKHS EMMANOYHL");
                eage = 66;
                esal = 398.1;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "SIDHROPYLOS XRYSANQOS");
                eage = 21;
                esal = 349.3;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "STELLAS PETROS");
                eage = 33;
                esal = 298.6;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "DHMHTRIADOY XARALAMPIA");
                eage = 44;
                esal = 245.9;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PALAIOLOGOS QWMAS");
                eage = 50;
                esal = 198.1;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "TSAKIROGLOY ANASTASIOS");
                eage = 61;
                esal = 147.1;
                strcpy(edname, "PSWMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "STAMATIOY DHMHTRIOS");
                eage = 24;
                esal = 159.6;
                strcpy(edname, "XYMOI");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "DEMERTZH AIMILIA");
                eage = 31;
                esal = 207.0;
                strcpy(edname, "SAPOUNIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "EYSTAQIOY MARIA");
                eage = 45;
                esal = 258.0;
                strcpy(edname, "PSWMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "MARKOGIANNAKH PARASKEYH");
                eage = 53;
                esal = 309.2;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "DIAMANTOPOYLOS QEOXARHS");
                eage = 60;
                esal = 356.9;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "LEONTIOS STAMATHS");
                eage = 24;
                esal = 408.7;
                strcpy(edname, "LAXANIKA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PAPPA EYQYMIA");
                eage = 31;
                esal = 457.0;
                strcpy(edname, "PSARIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "LAMPRINOYDHS NIKOLAOS");
                eage = 45;
                esal = 508.1;
                strcpy(edname, "SAPOUNIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "LOYKOPOYLOY BASILIKH");
                eage = 56;
                esal = 469.1;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "SKLABOS APOSTOLOS");
                eage = 61;
                esal = 418.7;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PANAGOPOYLOY EYANQIA");
                eage = 54;
                esal = 369.2;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "MAMALH NIKOLETA");
                eage = 53;
                esal = 315.8;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PANAGIWTOPOYLOS XARALAMPOS");
                eage = 34;
                esal = 268.2;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "XRYSANQAKOPOYLOS DHMHTRIOS");
                eage = 25;
                esal = 218.7;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "XRISTOOYLOS PANAGIWTHS");
                eage = 32;
                esal = 166.0;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "BASILAKAKH STAMATOYLA");
                eage = 43;
                esal = 175.0;
                strcpy(edname, "SAPOUNIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "TSOLAKAS KWNSTANTINOS");
                eage = 52;
                esal = 228.9;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KOLLIA AIKATERINH");
                eage = 65;
                esal = 277.2;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "GKOYRHS NIKOLAOS");
                eage = 56;
                esal = 328.2;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KAZAKOS STEFANOS");
                eage = 41;
                esal = 376.8;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "APOSTOLOPOYLOS KWNSTANTINOS");
                eage = 33;
                esal = 428.6;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PAPAQANASIOY ELENH");
                eage = 27;
                esal = 479.2;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "SOYTSOS IWANNHS");
                eage = 20;
                esal = 439.9;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "MATSARIDHS XARALAMPOS");
                eage = 31;
                esal = 388.9;
                strcpy(edname, "LAXANIKA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "STAYROY FILIPPOS");
                eage = 19;
                esal = 339.1;
                strcpy(edname, "PSWMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "TAGKOPOYLOS DHMHTRIOS");
                eage = 44;
                esal = 287.1;
                strcpy(edname, "KRASIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "REGKLEITHS STEFANOS");
                eage = 51;
                esal = 235.7;
                strcpy(edname, "KRASIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "LASKOS NIKOLAOS");
                eage = 60;
                esal = 188.3;
                strcpy(edname, "SAPOUNIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "ZANAKHS MIXAHL");
                eage = 18;
                esal = 198.1;
                strcpy(edname, "KREATA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "AMORIANOS KWNSTANTINOS");
                eage = 63;
                esal = 247.4;
                strcpy(edname, "SALAMIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "KOYTSOYKOS GEWRGIOS");
                eage = 54;
                esal = 297.9;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "PAPASWTHRIOY KYRIAKOS");
                eage = 46;
                esal = 348.6;
                strcpy(edname, "TYRIA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
                        edname, ++recordid);

                /* -----------------------------------------------------------------------------*/

                strcpy(ename, "LAMPROPOYLOY XRISTINA");
                eage = 31;
                esal = 399.0;
                strcpy(edname, "FROUTA");
                insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,edname, ++recordid);
                break;
            }
            else{
                printf("->Wrong input from command line...\n");
                printf("->                            _________________\n");
                printf("->                           |PROGRAMM COMMANDS|\n");
                printf("->-------------------------------------------------------------------------|\n");
                printf("->To load a test examble   : example                                       |\n");
                printf("->To Create File B+_Tree   : create [Name] [Type1] [Size1] [Type2] [Size2] |\n");
                printf("->To Open  File B+_Tree    : open [Filename]                               |\n");
                printf("->To Close open File       : close [Filename id]                           |\n");
                printf("->To Destroy File          : destroy [Filename]                            |\n");
                printf("->To insert a  records     : insert [Filename id] [Value1] [Value2]        |\n");
                printf("->To Open  Scanning        : sopen [Filename id] [Operation] [Value]       |\n");
                printf("->To Search Scanning       : search [Scanning]                             |\n");
                printf("->To Close Scanning        : sclose [Scanning id]                          |\n");
                printf("->To see Scanning status   : status scan                                   |\n");
                printf("->To see Opening  status   : status open                                   |\n");
                printf("->To exit programm press   : exit                                          |\n");
                printf("->-------------------------------------------------------------------------|\n");
                break;
            }
        }
    }
    if (AM_CloseIndex(eNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eAentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eSentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(dAentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", dnameAge);
		AM_PrintError(errStr);
	}
    if (AM_DestroyIndex(empName) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",empName);
	   AM_PrintError(errStr);
	}

	if (AM_DestroyIndex(empAge) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",empAge);
	   AM_PrintError(errStr);
	}

	if (AM_DestroyIndex(empSal) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",empSal);
	   AM_PrintError(errStr);
	}

	if (AM_DestroyIndex(dnameAge) != AME_OK) {
  	 sprintf(errStr,"Error in AM_DestroyIndex called on %s \n",dnameAge);
   	AM_PrintError(errStr);
	}
    return 0;
}

