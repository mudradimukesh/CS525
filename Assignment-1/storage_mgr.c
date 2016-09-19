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
    printf("\n---------------- < Storage Manager Initialised > ------------------\n");
}

/*********************** create/open/close Page Functions  **************************/
/* manipulating page files */

RC createPageFile (char *file_name)
{
    int i;
    FILE *file = NULL;  
    char *new_page,*page_info;

  
    file = fopen(file_name, "w");  /* opening file in write mode */ 
    new_page = (char *) calloc(PAGE_SIZE, sizeof(char));  
    page_info = (char *) calloc(FILE_HEADER, sizeof(char));  
    
    for (i=0; i < PAGE_SIZE; i++)
    { 
        new_page[i] == '\0';
        
        if(i < FILE_HEADER)
        {
          page_info[i] == '\0';
        }
   
    }
  
    strcat(page_info,"1\n");
    
    
    fwrite(page_info, sizeof(char), FILE_HEADER, file);
    fwrite(new_page, sizeof(char), PAGE_SIZE, file);

    free(new_page);
    free(page_info);
    fclose(file);

    return RC_OK;
}

/*------------------------------------------------------------------------*/

RC openPageFile (char *file_name, SM_FileHandle *fHandle)
{
    FILE *file = NULL;
    int error;
    char *page_info;
   
    file = fopen(file_name, "r+"); /* opening file in read mode */ 
    
    if (file != NULL)
    {            

        page_info = (char *) calloc(FILE_HEADER, sizeof(char));
        fgets(page_info, FILE_HEADER, file);
        

        fHandle->fileName = file_name; 
        fHandle->totalNumPages = atoi(page_info);
        fHandle->curPagePos = 1;
        fHandle->mgmtInfo = file;

        
        free(page_info);
        //fclose(file);   
        return RC_OK;
    }
    else
    {
     error = errno;
     //fclose(file);
     fprintf(stderr, "Error opening page file: %s\n", strerror( error ));
     return RC_FILE_NOT_FOUND;
    } 
  
}

/*------------------------------------------------------------------------*/


RC closePageFile (SM_FileHandle *fHandle)
{

  int closingCheck = -1, error;

  closingCheck = fclose(fHandle->mgmtInfo); 
  
 /* on success closingCheck ==0 */

  if(closingCheck == 0)
  {
     return RC_OK;
  }
  else
  {
   error = errno;
   fprintf(stderr, "Error opening page file: %s\n", strerror( error ));
    return RC_FILE_NOT_FOUND;
  }
}

/*------------------------------------------------------------------------*/

RC destroyPageFile (char *file_name)
{
  int destroyCheck = -1, error;

 
  destroyCheck = remove(file_name);
  /* on success destroyCheck ==0 */
  if (destroyCheck == 0)
  {
    return RC_OK;
  }
  else
 { 
  error = errno;
  fprintf(stderr, "Error opening page file: %s\n", strerror( error ));
  return RC_FILE_NOT_FOUND;
 }
}

/*------------------------------------------------------------------------*/

/***********************  Read Blocks Functions  **************************/
/* reading blocks from disc */

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    size_t buffer; // check for need to be free or not
    int seek_page;
    //FILE *file;
    //char print_str[PAGE_SIZE];

  /* check if file is present */
    
   if (fHandle->mgmtInfo == NULL)
    {
       printf("\n File Not Found");
       return RC_FILE_NOT_FOUND; 
    }

    /* check for the valid page number */
    if (pageNum > fHandle->totalNumPages || pageNum < 1)
    {
       printf("\n Not Valid Page");
       return RC_READ_NON_EXISTING_PAGE ;                                                                
    }
     
    if(pageNum==1)
    {
     seek_page = fseek(fHandle->mgmtInfo,FILE_HEADER, SEEK_SET); 
     printf("\npage no = %d-> flip pinter shift =%d",pageNum,FILE_HEADER);
  
    }
     
    if(pageNum>1)
    {
     printf("\npage no = %d-> flip pinter shift =%d",pageNum,((pageNum-1)*PAGE_SIZE)+FILE_HEADER);
     seek_page = fseek(fHandle->mgmtInfo, ((pageNum-1)*PAGE_SIZE)+FILE_HEADER, SEEK_SET);
    } 
     
     

    if (seek_page == 0)
    {
        printf("\n ------------Reading page no = %d",pageNum);
        
        buffer = fread(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);
        printf("\n Data in PageBlock =%s",memPage);
        fHandle->curPagePos = pageNum;     //*************      

        return RC_OK;
    }
    else
    {
        printf("\n File Read Error"); 
        return RC_READ_NON_EXISTING_PAGE;
    }

  


}

/*------------------------------------------------------------------------*/

int getBlockPos (SM_FileHandle *fHandle)
{
   return fHandle->curPagePos;
}
/*------------------------------------------------------------------------*/

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
     return readBlock(1, fHandle, memPage);
}
/*------------------------------------------------------------------------*/

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
   return readBlock(fHandle->curPagePos-1, fHandle, memPage); 
}

/*------------------------------------------------------------------------*/

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(fHandle->curPagePos, fHandle, memPage);
}

/*------------------------------------------------------------------------*/

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 
   return readBlock(fHandle->curPagePos+1, fHandle, memPage);
}
/*------------------------------------------------------------------------*/

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
     return readBlock(fHandle->totalNumPages, fHandle, memPage);
}



/***********************  Write Blocks Functions  **************************/
/* writing blocks to a page file */
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int seek_page;
    size_t buffer;
    //char print_str[PAGE_SIZE];

     if (pageNum > (fHandle->totalNumPages) || (pageNum < 1))
    {
        printf("\nNot valid Page"); 
    
        return RC_WRITE_FAILED;
    }
    
    
    if(pageNum==1)
    {
     seek_page = fseek(fHandle->mgmtInfo, FILE_HEADER, SEEK_SET); 
     printf("\npage no = %d-> flip pointer shift =%d",pageNum,FILE_HEADER);
  
    }
     
    if(pageNum>1)
    {
     printf("\npage no = %d-> flip pointer shift =%d",pageNum,((pageNum-1)*PAGE_SIZE)+FILE_HEADER);
     seek_page = fseek(fHandle->mgmtInfo, ((pageNum-1)*PAGE_SIZE)+FILE_HEADER, SEEK_SET); 
    } 
     

    if (seek_page == 0)
    {
      
        buffer = fwrite(memPage,sizeof(char), PAGE_SIZE, fHandle->mgmtInfo); 
        fHandle->curPagePos = pageNum;    //*************    
       
        rewind(fHandle->mgmtInfo);
	fprintf(fHandle->mgmtInfo, "%d\n" , fHandle->totalNumPages);

        return RC_OK;
    }
    else
    {

        printf("\nseek_page failed "); 
        return RC_WRITE_FAILED;
    }

}
/*------------------------------------------------------------------------*/

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
  int pagePosi; 
  // aditional check introduce if want
  pagePosi=fHandle->curPagePos;
  return writeBlock (pagePosi, fHandle, memPage);

}
/*------------------------------------------------------------------------*/

RC appendEmptyBlock (SM_FileHandle *fHandle)
{
    int i,seek_page,total_pages;
    size_t buffer; 
    char *empty_block;
 
    empty_block = (char *) calloc(PAGE_SIZE, sizeof(char)); 
  
    for (i=0; i < PAGE_SIZE; i++)
    { 
        empty_block[i] = '\0';
    }
    
    //empty_block[PAGE_SIZE] == '\0';
  

    total_pages=fHandle->totalNumPages;
    seek_page = fseek(fHandle->mgmtInfo,0L, SEEK_END); 

    if (seek_page == 0)
    {
    
        buffer = fwrite(empty_block,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo); 
        fHandle->totalNumPages = fHandle->totalNumPages + 1;
        fHandle->curPagePos = fHandle->totalNumPages; //*************   
	

	rewind(fHandle->mgmtInfo);
	fprintf(fHandle->mgmtInfo, "%d\n" , fHandle->totalNumPages); 

        free(empty_block);
        
           
        return RC_OK;
    }
    else
    {
        free(empty_block);
	return RC_WRITE_FAILED;
    }

}
/*------------------------------------------------------------------------*/

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
   int i,requiredPages;
   
    if (fHandle->totalNumPages < numberOfPages)
    {
        requiredPages = numberOfPages - fHandle->totalNumPages; 
        
        for (i=1; i <= requiredPages; i++)
        {
	   appendEmptyBlock(fHandle); 
	}
    }
    
    return RC_OK;

}
/*------------------------------------------------------------------------*/





