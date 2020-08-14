/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: fscanf_s.c
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
 *    fscanf_s
 *
 * <SYNOPSIS>
 *    int fscanf_s(FILE *stream, const char *format, ...)
 *
 * <FUNCTION DESCRIPTION>
 *    The fscanf function reads data from the current position of stream into 
 *    the locations given by argument (if any). Each argument must be a pointer 
 *    to a variable of a type that corresponds to a type specifier in format.
 *    format controls the interpretation of the input fields and has the same 
 *    form and function as the format argument for scanf; see scanf for a 
 *    description of format.
 *
 * <INPUT PARAMETERS>
 *    stream              Pointer to FILE structure.
 *    format              Format control string, see Format Specifications.
 *    ...                 Optional arguments.
 *
 * <OUTPUT PARAMETERS>
 *    ...                 The convered value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Each of these functions returns the number of fields successfully converted
 *    and assigned; the return value does not include fields that were read but 
 *    not assigned. A return value of 0 indicates that no fields were assigned. 
 *    If an error occurs, or if the end of the file stream is reached before the 
 *    first conversion, the return value is EOF for fscanf and fwscanf.
 *******************************************************************************
*/

int fscanf_s(FILE* stream, const char* format, ...)
{
    int ret = 0;
    va_list arglist;

    va_start(arglist, format);
    ret = vfscanf_s(stream, format, arglist);
    va_end(arglist);

    return ret;
}


