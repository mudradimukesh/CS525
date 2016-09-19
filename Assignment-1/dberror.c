#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dberror.h"

char *RC_message;

void printError (RC RC_Code)
{
  if (RC_Code != RC_OK)
    printf("\nError_Number:(%i) Eror_Message -> \n", RC_Code);
  else
    printf("\nRC_Code:(%i)\n", RC_Code);
}

