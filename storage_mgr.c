#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "storage_mgr.h"
#include "dberror.h"

extern int errno ;

/*********************** Storage Manager Initialisation  **************************/
void initStorageManager (void)
{
    printf("\n****** Storage Manager Initialised ****** \n");
}

/***********************  Page Functions  **************************/

RC createPageFile (char *file_name)
{

    FILE *file = NULL;  
    char *page_info;

    printf("\nIn createPageFile Function");

    file = fopen(file_name, "w");  /* opening file in write mode */ 
    page_info = (char *) calloc(PAGE_SIZE, sizeof(char));  /* reserving 1st page's space for storing total number of pages information */
     
 
    strcat(page_info,"1\n");
    printf("\nTotal Pages =%s", page_info);

    fwrite(page_info, sizeof(char), PAGE_SIZE, file);

    free(page_info);


    fclose(file);

    printf("\nEnd createPageFile Function");

    return RC_OK;
}

/*------------------------------------------------------------------------*/

RC openPageFile (char *file_name, SM_FileHandle *fHandle)
{
    FILE *file = NULL;
    int error;

    printf("\nIn openPageFile Function"); 

    file = fopen(file_name, "r+"); /* opening file in read mode */ 
    
    if (file != NULL)
    {
        char *page_info;
            

        page_info = (char *) calloc(PAGE_SIZE, sizeof(char));
        fgets(page_info, PAGE_SIZE, file);

        page_info = strtok(page_info, "\n"); /* remove trailing newline char */

        fHandle->fileName = file_name; /* assign values to SM_FileHandle structure */
        fHandle->totalNumPages = atoi(page_info);
        fHandle->curPagePos = 0;
        fHandle->mgmtInfo = file;

        printf("\nTotal Pages =%d", atoi(page_info));

        free(page_info);
        printf("\nEnd openPageFile Function");
        return RC_OK;
    }
    else
    {
     error = errno;
     fprintf(stderr, "Error opening page file: %s\n", strerror( error ));
     printf("\nEnd openPageFile Function");
     return RC_FILE_NOT_FOUND;
    } 
  
}

/*------------------------------------------------------------------------*/


RC closePageFile (SM_FileHandle *fHandle)
{

  int closingCheck = -1, error;

  printf("\nIn closePageFile Function");
  closingCheck = fclose(fHandle->mgmtInfo); /* close open file descriptor at fHandle->mgmtInfo */

 /* on success closingCheck ==0 */

  if(closingCheck == 0)
  {
    printf("\nEnd closePageFile Function");
    return RC_OK;
  }
  else
  {
   error = errno;
   fprintf(stderr, "Error opening page file: %s\n", strerror( error ));
   printf("\nEnd closePageFile Function");
   return RC_FILE_NOT_FOUND;
  }
}

/*------------------------------------------------------------------------*/

RC destroyPageFile (char *file_name)
{
  int destroyCheck = -1, error;

  printf("\nIn destroyPageFile Function");

  destroyCheck = remove(file_name);
  /* on success destroyCheck ==0 */
  if (destroyCheck == 0)
  {
    printf("\nEnd destroyPageFile Function");
    return RC_OK;
  }
  else
 { 
  error = errno;
  fprintf(stderr, "Error opening page file: %s\n", strerror( error ));
  printf("\nEnd destroyPageFile Function");
  return RC_FILE_NOT_FOUND;
 }
}

/*------------------------------------------------------------------------*/

