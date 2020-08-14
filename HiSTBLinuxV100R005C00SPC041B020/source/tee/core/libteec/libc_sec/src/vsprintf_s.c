/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: vsprintf.c
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
 *    vsprintf_s
 *
 * <SYNOPSIS>
 *    int vsprintf_s(char* strDest, size_t destMax, const char* format, valist ap);
 *
 * <FUNCTION DESCRIPTION>
 *    The vsprintf_s function takes a pointer to an argument list, and then formats
 *    and writes the given data to the memory pointed to by strDest.
 *    The function differ from the non-secure versions only in that the secure
 *    versions support positional parameters.
 *
 * <INPUT PARAMETERS>
 *    strDest                Storage location for the output.
 *    destMax                Size of strDest
 *    format                 Format specification.
 *    ap                     pointer to list of arguments
 *
 * <OUTPUT PARAMETERS>
 *    strDest                is updated
 *
 * <RETURN VALUE>
 *    vsprintf_s returns the number of characters written, not including the 
 *    terminating null character, or a negative value if an output error occurs.
 *    If strDest or format is a null pointer, if count is zero, or if the format
 *    string contains invalid formatting characters, the function return -1.
 *******************************************************************************
*/

int vsprintf_s (char* strDest, size_t destMax, const char* format, va_list arglist)
{
    int retvalue = -1;

    if (format == NULL || strDest == NULL || destMax == 0 || destMax > SECUREC_STRING_MAX_LEN)
    {
        if (strDest != NULL && destMax > 0)
        {
            strDest[0] = '\0';
        }
        SECUREC_ERROR_INVALID_PARAMTER("vsprintf_s");
        return -1;
    }

    retvalue = vsnprintf_helper(strDest, destMax, format, arglist);

    if (retvalue < 0)
    {
        strDest[0] = '\0';
        if (retvalue == -2)
        {
            /*Buffer is too small*/
            SECUREC_ERROR_INVALID_RANGE("vsprintf_s");
        }
        SECUREC_ERROR_INVALID_PARAMTER("vsprintf_s");
        return -1;
    }

    return retvalue;
}


