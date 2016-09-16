#ifndef DBERROR_H
#define DBERROR_H

#include "stdio.h"

/* module wide constants */
#define PAGE_SIZE 2048

/* return code definitions */
typedef int RC;

#define RC_OK 0
#define RC_FILE_NOT_FOUND -1

/*For printing a Eror Message  */
extern void printError_Msg (RC error);
extern char *error_Msg (RC error);

#endif
