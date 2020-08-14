/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: snprintf.c
* History:   
*     1. Date:
*         Author:    
*         Modification:
********************************************************************************
*/

#include "securec.h"
#include "securecutil.h"
#include <stdarg.h>

/*******************************************************************************
 * <NAME>
 *    sprintf_s
 *
 * <SYNOPSIS>
 *    int sprintf_s(char* strDest, size_t destMax, const char* format, ...);
 *
 * <FUNCTION DESCRIPTION>
 *    The sprintf_s function formats and stores a series of characters and values
 *    in strDest. Each argument (if any) is converted and output according to 
 *    the corresponding format specification in format. The format consists of
 *    ordinary characters and has the same form and function as the format argument
 *    for printf. A null character is appended after the last character written.
 *    If copying occurs between strings that overlap, the behavior is undefined.
 *
 * <INPUT PARAMETERS>
 *    strDest                Storage location for output.
 *    destMax                Maximum number of characters to store.
 *    format                 Format-control string.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                is updated
 *
 * <RETURN VALUE>
 *    sprintf_s returns the number of bytes stored in strDest, not counting the
 *    terminating null character.
 *    The number of characters written, or -1 if an error occurred. If strDest 
 *    or format is a null pointer, sprintf_s returns -1.
 *******************************************************************************
*/

int sprintf_s (char* strDest, size_t destMax, const char* format, ...)
{
    int ret = 0;
    va_list arglist;

    va_start(arglist, format);
    ret = vsprintf_s(strDest, destMax, format, arglist);
    va_end(arglist);

    return ret;
}


