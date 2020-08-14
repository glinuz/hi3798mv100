/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: scanf_s.c
* History:   
*     1. Date:
*         Author:    
*         Modification:
********************************************************************************
*/

#include "securec.h"
#include "secinput.h"

/*******************************************************************************
 * <NAME>
 *    scanf_s
 *
 * <SYNOPSIS>
 *    int scanf_s (const char* format, ...);
 *
 * <FUNCTION DESCRIPTION>
 *    The scanf_s function reads data from the standard input stream stdin and
 *    writes the data into the location that's given by argument. Each argument
 *    must be a pointer to a variable of a type that corresponds to a type specifier
 *    in format. If copying occurs between strings that overlap, the behavior is
 *    undefined.
 *
 * <INPUT PARAMETERS>
 *    format                  Format control string.
 *    ...                     Optional arguments.
 *
 * <OUTPUT PARAMETERS>
 *    ...                     The converted value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Returns the number of fields successfully converted and assigned;
 *    the return value does not include fields that were read but not assigned.
 *    A return value of 0 indicates that no fields were assigned.
 *    The return value is EOF for an error, or if the end-of-file character
 *    or the end-of-string character is encountered in the first attempt to read
 *    a character.
 *    If format is a NULL pointer, the invalid parameter handler is invoked, If
 *    execution is allowed to continue, scanf_s and wscanf_s return SCANF_EINVAL.
 *******************************************************************************
*/

int scanf_s (const char* format, ...)
{
    int ret = 0;
    va_list arglist;

    va_start(arglist, format);
    ret =  vscanf_s( format, arglist);
    va_end(arglist);

    return ret;
}


