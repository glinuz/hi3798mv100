/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: wcscpy_s.c
* History:   
*     1. Date:
*         Author:    
*         Modification:
********************************************************************************
*/

#include "securec.h"
#include "securecutil.h"

/*******************************************************************************
 * <NAME>
 *    wcscpy_s
 *
 * <SYNOPSIS>
 *    errno_t wcscpy_s(wchar_t* strDest, size_t destMax, const wchar_t* strSrc);
 *
 * <FUNCTION DESCRIPTION>
 *    wcscpy_s is wide-character version of strcpy_s.

 * <INPUT PARAMETERS>
 *    strDest                   Location of destination string buffer
 *    destMax                   Size of the destination string buffer.
 *    strSrc                    Null-terminated source string buffer.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                   is updated.
 *
 * <RETURN VALUE>
 *    0                         success
 *    EINVAL                    strDest == NULL or strSrc == NULL
 *    ERANGE                    destination buffer is NOT enough,  or size of 
 *                              buffer is zero or greater than SECUREC_STRING_MAX_LEN
 *    EOVERLAP_AND_RESET        dest buffer and source buffer are overlapped
 *
 *    If there is a runtime-constraint violation, then if strDest is not a null 
 *    pointer and destMax is greater than zero and not greater than 
 *    SECUREC_WCHAR_STRING_MAX_LEN, then wcscpy_s sets strDest[0] to the null
 *    character.
 *******************************************************************************
*/

errno_t wcscpy_s(wchar_t* strDest, size_t destMax, const wchar_t* strSrc)
{
    wchar_t* pHeader = strDest;
    size_t maxSize = destMax;
    IN_REGISTER const wchar_t* overlapGuard = NULL;

    if (destMax == 0 || destMax > SECUREC_WCHAR_STRING_MAX_LEN)
    {
        SECUREC_ERROR_INVALID_RANGE("wcscpy_s");
        return ERANGE;
    }
    if (strDest == NULL || strSrc == NULL)
    {
        SECUREC_ERROR_INVALID_PARAMTER("wcscpy_s");
        if (strDest != NULL)
        {
            pHeader[0] = '\0';
            return EINVAL_AND_RESET;
        }


        return EINVAL;
    }

    if (strDest == strSrc)
    {
        return EOK;
    }

    if (strDest < strSrc)
    {
        overlapGuard = strSrc;
        while ((*(strDest++) = *(strSrc++)) != '\0'  && --maxSize > 0)
        {
            if ( strDest == overlapGuard)
            {
                pHeader[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("strcpy_s");
                return EOVERLAP_AND_RESET;
            }
        }
    }
    else
    {
        overlapGuard = strDest;
        while ((*(strDest++) = *(strSrc++)) != '\0'  && --maxSize > 0)
        {
            if ( strSrc == overlapGuard)
            {
                pHeader[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("strcpy_s");
                return EOVERLAP_AND_RESET;
            }
        }
    }

    if (maxSize == 0)
    {
        pHeader[0] = '\0';
        SECUREC_ERROR_INVALID_RANGE("wcscpy_s");
        return ERANGE;
    }

    return EOK;
}


