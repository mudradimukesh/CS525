#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
//#include <assert.h>
#include "test_helper.h"

#define file_name "test_file.txt"

#define TOTAL_TEST_CASES 7

/* prototype functions */

void testFileHandleInfo(SM_FileHandle );
void readFile(char *);


/*---------------------------------------------------------*/

int main(void)
{
  int Test_Case=1;
  int i;
  SM_FileHandle File_ph,Mem_ph;
  char *Mem_ph_pointer,buffer[PAGE_SIZE];

 //Mem_ph_pointer = (char *) calloc(PAGE_SIZE, sizeof(char)); // free last
 
 printf("\n------------- < Running Test Cases > ----------------\n");
 
  do{
    
  
     switch(Test_Case) 
     {
     
      case 1: 
              printf("\n------------- < Running Test Case :%d Intialise Storage Manager > ----------------\n",Test_Case); 
              initStorageManager();
              break;
      case 2:
              printf("\n------------- < Running Test Case :%d Open Page > ----------------\n",Test_Case); 
              /*---------------------------------------------------------------------*/
              TEST_RC(createPageFile (file_name));   
              TEST_RC(openPageFile (file_name, &File_ph));
              //TEST_RC(readFirstBlock(&File_ph, Mem_ph_pointer) );
 
               /*---------------------------------------------------------------------*/
              break;
      case 3:
              printf("\n------------- < Running Test Case :%d Write Block> ----------------\n",Test_Case);
              /*---------------------------------------------------------*/
 
  		for (i=0;i<PAGE_SIZE;i++)
  		{
     			 buffer[i]='S';
 		 } 
 
  		//buffer[PAGE_SIZE] ='\0';
       
  		Mem_ph_pointer=buffer;
  		TEST_RC(writeBlock (1, &File_ph, Mem_ph_pointer));  		
     		//TEST_RC(readFirstBlock(&File_ph, Mem_ph_pointer));
  
               /*---------------------------------------------------------------------*/

              break;
      case 4: 
    	       printf("\n------------- < Running Test Case :%d Append Block> ----------------\n",Test_Case);
              /*---------------------------------------------------------*/
                 appendEmptyBlock(&File_ph);
                //TEST_RC(readFirstBlock(&File_ph, Mem_ph_pointer));
  		//TEST_RC(readNextBlock(&File_ph, Mem_ph_pointer));
  		//TEST_RC(readNextBlock(&File_ph, Mem_ph_pointer)); //testing
            
               break;
              
              
      case 5:          
               /*---------------------------------------------------------------------*/
              printf("\n------------- < Running Test Case :%d Over Writing Page Block> ----------------\n",Test_Case);
         
   
 	      for (i=0;i<PAGE_SIZE;i++)
  		{
  		    buffer[i]='A';
 		 } 
  
  		//buffer[PAGE_SIZE] ='\0';
    
 		 Mem_ph_pointer=buffer;
 		 TEST_RC(writeBlock (2, &File_ph, Mem_ph_pointer));
  		 //TEST_RC(readFirstBlock(&File_ph, Mem_ph_pointer) );
  		 //TEST_RC(readNextBlock(&File_ph, Mem_ph_pointer) );
  


               /*---------------------------------------------------------------------*/
              break;
              
              
     case 6: 
              /*---------------------------------------------------------------------*/
               printf("\n------------- < Running Test Case :%d Writing on non-existing Page Block> ----------------\n",Test_Case);
               TEST_RC(writeBlock (3, &File_ph, Mem_ph_pointer));
               break;
               
     case 7:
               /*---------------------------------------------------------------------*/
               printf("\n------------- < Running Test Case :%d Add this case test> ----------------\n",Test_Case);
              
               break;
       
    default :
        printf("\n------------- < All Test Cases Ran Successfully > ----------------\n");
               break;
     }
     
      
      if(Test_Case>1)
      {
        testFileHandleInfo(File_ph); //testing
        readFile(File_ph.fileName); //testing
      }
      
      //free(Mem_ph_pointer);
      Test_Case ++ ;
     
   }while(Test_Case<=TOTAL_TEST_CASES);
  
     //free(Mem_ph_pointer);
     closePageFile (&File_ph);
     destroyPageFile (file_name);
  
     printf("\n------------- < Working Fine > ----------------\n");
 
  return 0;
}


/*---------------------------------------------------------------------*/

void testFileHandleInfo(SM_FileHandle File_ph)
{

  printf("\n-----------------< File Handler Info >-------------------\n"); 
  printf("\nFile handler->file name = %s",File_ph.fileName);  
  printf("\nFile fHandle->totalNumPages =%d", File_ph.totalNumPages);
  printf("\nFile fHandle->curPagePos =%d", File_ph.curPagePos);

}

/*---------------------------------------------------------*/
void readFile(char *file)
{
   int file_size=0;
   FILE *fp ;
   char c;
 
  fp=fopen(file, "r+");
  printf("\n  ------------------------ < Read File > ---------------------------  \n");
  printf("\n------------------------Testing File Content------------------------\n");  
  
  fseek(fp, 0L, SEEK_SET);
    
       while((c=fgetc(fp))!=EOF)
       {
         printf("%c",c);
       }
       

  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  
  printf("\n----------------------> < File Size =%d  > <-------------",file_size);
  printf("\n-----------------------------------------------------------------\n");


  fclose(fp);

}

