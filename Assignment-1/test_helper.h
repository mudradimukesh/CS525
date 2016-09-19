#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// varible to store the current test's name
extern char *testName;

#define FUNCT_INFO  __FUNCTION__,__FILE__,__LINE__


#define TEST_RC(RC_Code)\
    do {\
        if(RC_Code == RC_OK)\
        {                   \
            printf("\nFunction_Name:%s  File_Name:%s  Line_Number:L%i,<----RC_Status : %d [OK] ---->\n",FUNCT_INFO,RC_Code); \
        }\
        else \
        { printError ( RC_Code, RC_message);\
            printf("\nFunction_Name:%s  File_Name:%s  Line_Number:L%i,<----RC_Status : %d [Error] ---->\n",FUNCT_INFO,RC_Code); \
        }       \
    } while(0); 

#endif
