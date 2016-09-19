#ifndef DBERROR_H
#define DBERROR_H

#include "stdio.h"


#define PAGE_SIZE 1024
#define FILE_HEADER 128


#define RC_OK 0
#define RC_FILE_NOT_FOUND 1
#define RC_READ_NON_EXISTING_PAGE 2
#define RC_WRITE_FAILED 3

typedef int RC;
extern char RC_message[]; 

extern void printError(RC error, char *message);


#endif
