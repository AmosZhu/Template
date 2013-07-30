#ifndef _AMOS_TYPE_H
#define _AMOS_TYPE_H

#include <stdio.h>
#include <stdlib.h>

#define BOOL int
#define FALSE (0)
#define TRUE (1)


typedef enum ERROR_TYPE
{
    RETURN_SUCCESS=0,
    OPERATOR_FAILED=1,
    INVALIDE_PARAMET=2,
}
Err_t;


typedef unsigned long AM_U32;
typedef long AM_S32;
typedef unsigned long long AM_U64;
typedef long long AM_S64;
typedef unsigned short AM_U16;
typedef short AM_S16;
typedef unsigned char AM_U8;
typedef char AM_S8;

#endif
