/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: vwscanf_s.c
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
 *    vwscanf_s
 *
 * <SYNOPSIS>
 *    int vwscanf_s(const wchar_t* format, va_list arglist)
 *
 * <FUNCTION DESCRIPTION>
 *    The vwscanf_s function is the wide-character version of vscanf_s. The 
 *    function reads data from the standard input stream stdin and writes the
 *    data into the location that's given by argument. Each argument  must be a
 *    pointer to a variable of a type that corresponds to a type specifier in
 *    format. If copying occurs between strings that overlap, the behavior is
 *    undefined.
 *
 * <INPUT PARAMETERS>
 *    format                 Format control string.
 *    arglist                pointer to list of arguments
 *
 * <OUTPUT PARAMETERS>
 *    arglist                the converted value stored in user assigned address
 *
 * <RETURN VALUE>
 *    Returns the number of fields successfully converted and assigned;
 *    the return value does not include fields that were read but not assigned.
 *    A return value of 0 indicates that no fields were assigned.
 *    The return value is EOF for an error,
 *    or if the end-of-file character or the end-of-string character is encountered
 *    in the first attempt to read a character.
 *    If format is a NULL pointer, the invalid parameter handler is invoked,
 *    as described in Parameter Validation. If execution is allowed to continue,
 *    scanf_s and wscanf_s return SCANF_EINVAL(-1).
 *******************************************************************************
*/

int vwscanf_s(const wchar_t* format, va_list arglist)
{
    int retval = -1;
    SEC_FILE_STREAM fStr = INIT_SEC_FILE_STREAM;

    if (format == NULL)
    {
        SECUREC_ERROR_INVALID_PARAMTER("vwscanf_s");
        return SCANF_EINVAL;
    }
    
    fStr.pf = stdin;
    fStr._flag = FROM_STDIN_FLAG;

    SECUREC_LOCK_STDIN(0, stdin);

    retval = securec_winput_s(&fStr, format, arglist);

    SECUREC_UNLOCK_STDIN(0, stdin);

    if (retval < 0)
    {
        /* coverity[leaked_storage] */
        SECUREC_ERROR_INVALID_PARAMTER("vwscanf_s");
        return SCANF_EINVAL;
    }

    return retval;
}


