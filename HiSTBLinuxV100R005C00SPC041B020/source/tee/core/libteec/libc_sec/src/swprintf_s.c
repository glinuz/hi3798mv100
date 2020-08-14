/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: swprintf_s.c
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
 *    swprintf_s
 *
 * <SYNOPSIS>
 *    int swprintf_s(wchar_t* strDest, size_t destMax, const wchar_t* format, ...);
 *
 * <FUNCTION DESCRIPTION>
 *    swprintf_s is a wide-character version of sprintf_s; the pointer arguments
 *    to swprintf_s are wide-character strings. Detection of encoding errors in
 *    swprintf_s may differ from that in sprintf_s.
 *
 * <INPUT PARAMETERS>
 *    strDest                 Storage location for the output.
 *    destMax                 Maximum number of characters to store.
 *    format                  Format-control string.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                 is updated
 *
 * <RETURN VALUE>
 *    swprintf_s returns the number of wide characters stored in strDest, not 
 *    counting the terminating null wide character.
 *    The number of characters written, or -1 if an error occurred. If strDest
 *    or format is a null pointer, swprintf_s return -1.
 *******************************************************************************
*/

int swprintf_s (wchar_t* strDest, size_t destMax, const wchar_t* format, ...)
{
    int ret = 0;
    va_list arglist;

    va_start(arglist, format);
    ret = vswprintf_s(strDest, destMax, format, arglist);
    va_end(arglist);

    return ret;
}


