/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: vswprintf.c
* History:   
*     1. Date: 2014/4/10
*        Author: LiShunda
*        Modification: move vswprintf_helper() function from  vswprintf.c to this file,
*                      which will facilitate the UNICODE string API building.
********************************************************************************
*/

#include "securec.h"
#include "securecutil.h"
#include <stdarg.h>
#include "secureprintoutput.h"

/*******************************************************************************
 * <NAME>
 *    vswprintf_s
 *
 * <SYNOPSIS>
 *    int vswprintf_s(wchar_t* strDest, size_t destMax, const wchar_t* format, valist ap);
 *
 * <FUNCTION DESCRIPTION>
 *    The vswprintf_s function is a wide-character version of vsprintf_s.
 *    The function takes a pointer to an argument list, and then formats and
 *    writes the given data to the memory pointed to by strDest.
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
 *    vswprintf_s returns the number of characters written, not including the
 *    terminating null character, or a negative value if an output error occurs.
 *    If strDest or format is a null pointer, if count is zero, or if the format
 *    string contains invalid formatting characters, the function return -1.
 *******************************************************************************
*/
extern int putWcharStrEndingZero(SECUREC_XPRINTF_STREAM* str, int zerosNum);

int vswprintf_helper (wchar_t* string, size_t sizeInWchar, const wchar_t* format, va_list arglist)
{
    SECUREC_XPRINTF_STREAM str;
    int retval;


    str._ptr = (char*) string;
    str._cnt = (int)(sizeInWchar * WCHAR_SIZE);

    retval = securec_woutput_s(&str, format, arglist );
    //lint -esym(526, putWcharStrEndingZero*)
    if ((retval >= 0) && putWcharStrEndingZero(&str, WCHAR_SIZE) )
    {
        return (retval);
    }


    if (string != NULL)
    {
        string[sizeInWchar - 1] = 0;
    }
    if (str._cnt < 0)
    {
        /* the buffer was too small; we return -2 to indicate truncation */
        return -2;
    }
    return -1;

}

int vswprintf_s (wchar_t* strDest, size_t destMax, const wchar_t* format, va_list ap)
{
    int retvalue = -1;

    if (format == NULL || strDest == NULL || destMax == 0 || destMax > (SECUREC_WCHAR_STRING_MAX_LEN))
    {
        if (strDest != NULL && destMax > 0)
        {
            strDest[0] = '\0';
        }
        SECUREC_ERROR_INVALID_PARAMTER("vswprintf_s");
        return -1;
    }

    retvalue = vswprintf_helper(strDest, destMax, format, ap);

    if (retvalue < 0)
    {
        strDest[0] = '\0';
        if (retvalue == -2)
        {
            /*Buffer too small*/
            SECUREC_ERROR_INVALID_RANGE("vswprintf_s");
        }
        SECUREC_ERROR_INVALID_PARAMTER("vswprintf_s");
        return -1;
    }

    return retvalue;
}


