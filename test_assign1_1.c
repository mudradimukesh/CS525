#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
//#include <assert.h>
//#include "test_helper.h"

#define file_name "test_file.txt"


/* prototype functions */
static void testPage(void);


/*---------------------------------------------------------*/

int main(void)
{

  //printf("\nEnter the file name :%s",file_name);

  initStorageManager();

  testPage();


  return 0;
}

/*---------------------------------------------------------*/
void testPage(void)
{
  SM_FileHandle fh;

  createPageFile (file_name);
  
  openPageFile (file_name, &fh);

  printf("\n**** File Handler Info ****"); 
  printf("\nFile handler->file name = %s",fh.fileName);  
  printf("\nFile fHandle->totalNumPages =%d", fh.totalNumPages);
  printf("\nFile fHandle->curPagePos =%d", fh.curPagePos);
  //printf("\nFile fHandle->mgmtInfo =%s", char*(fh.mgmtInfo));


  if(strcmp(fh.fileName, file_name)==0)
   {
      printf("\n File is Good");
   } 

  closePageFile (&fh);
  destroyPageFile (file_name);

}


