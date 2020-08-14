/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: vscanf_s.c
* History:   
*     1. Date: 
*         Author: 
*         Modification: 
********************************************************************************
*/

#include "securec.h"
#include "secinput.h"
#include "securecutil.h"

/*******************************************************************************
 * <NAME>
 *    vscanf_s
 *
 * <SYNOPSIS>
 *    int vscanf_s(const char* format, va_list arglist)
 *
 * <FUNCTION DESCRIPTION>
 *    The vscanf_s function reads data from the standard input stream stdin and
 *    writes the data into the location that's given by argument. Each argument
 *    must be a pointer to a variable of a type that corresponds to a type specifier
 *    in format. If copying occurs between strings that overlap, the behavior is
 *    undefined.
 *
 * <INPUT PARAMETERS>
 *    format                Format control string.
 *    arglist               pointer to list of arguments
 *
 * <OUTPUT PARAMETERS>
 *    arglist               the converted value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Returns the number of fields successfully converted and assigned;
 *    the return value does not include fields that were read but not assigned.
 *    A return value of 0 indicates that no fields were assigned.
 *    The return value is EOF for an error,
 *    or if the end-of-file character or the end-of-string character is encountered
 *    in the first attempt to read a character.
 *    If format is a NULL pointer or retrun value is less than zero, the invalid
 *    parameter handler is invoked, as described in Parameter Validation. If
 *    execution is allowed to continue, scanf_s and wscanf_s return SCANF_EINVAL(-1).
 *******************************************************************************
*/
int vscanf_s(const char* format, va_list arglist)
{
    int retval = -1;
    SEC_FILE_STREAM fStr = INIT_SEC_FILE_STREAM;
/*
"va_list" has different definition on different platform, so we can't use arglist == NULL
to determine it's invalid. If you has fixed platform, you can check some fields to validate it,
such as "arglist == NULL" or arglist.xxx != NULL or *(size_t*)&arglist != 0.
*/
    if (format == NULL)
    {
        SECUREC_ERROR_INVALID_PARAMTER("vscanf_s");
        return SCANF_EINVAL;
    }

    fStr.pf = stdin;
    fStr._flag = FROM_STDIN_FLAG;

    SECUREC_LOCK_STDIN(0, stdin);

    retval = securec_input_s(&fStr, format, arglist);

    SECUREC_UNLOCK_STDIN(0, stdin);
    if (retval < 0)
    {
        /*MaskCoverityID31812*/ /*coverity[RESOURCE_LEAK]*/
        SECUREC_ERROR_INVALID_PARAMTER("vscanf_s");
        /* coverity[leaked_storage] */
        return SCANF_EINVAL;
    }
    return retval;
}


