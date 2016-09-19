#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dberror.h"

char RC_message[1024];

void printError (RC RC_Code, char *message)
{
  if (RC_Code != RC_OK) {
    printf("\nError_Occured -------> Error_Number: (%i) Error_Message: %s\n", RC_Code, message);
  } else {
    printf("\nRC_Code:(%i)\n", RC_Code);
  }

  memset((void *) RC_message, 0, 1024);
}

