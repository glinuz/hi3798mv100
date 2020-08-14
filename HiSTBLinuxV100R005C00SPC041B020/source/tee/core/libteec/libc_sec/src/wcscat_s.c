/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: wcscat_s.c
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
 *    wcscat_s
 *
 * <SYNOPSIS>
 *    errno_t wcscat_s(wchar_t* strDest, size_t destMax, const wchar_t* strSrc);
 *
 * <FUNCTION DESCRIPTION>
 *    The wcscat_s function is the wide-character and multibyte-character 
 *    versions of strcat_s.
 *    The arguments and return value of wcscat_s are wide-character strings.
 *
 *    The wcscat_s function appends strSrc to strDest and terminates the resulting
 *    string with a null character. The initial character of strSrc overwrites the
 *    terminating null character of strDest. wcscat_s will return EOVERLAP_AND_RESET if the
 *    source and destination strings overlap.
 *
 *    Note that the second parameter is the total size of the buffer, not the
 *    remaining size.
 *
 * <INPUT PARAMETERS>
 *    strDest              Null-terminated destination string buffer.
 *    destMax              Size of the destination string buffer.
 *    strSrc               Null-terminated source string buffer.
 *
 * <OUTPUT PARAMETERS>
 *    strDest              is updated
 * 
 * <RETURN VALUE>
 *    EOK                  Successful operation
 *    EINVAL               strDest not terminated or NULL, strSrc is NULL
 *    ERANGE               destMax is 0, too big or too small
 *    EOVERLAP_AND_RESET   dest buffer and source buffer are overlapped
 *******************************************************************************
*/

errno_t wcscat_s(wchar_t* strDest, size_t destMax, const wchar_t* strSrc)
{
    wchar_t* pHeader = strDest;
    size_t  availableSize = destMax;
    IN_REGISTER const wchar_t* overlapGuard = NULL;

    if (destMax == 0 || destMax > SECUREC_WCHAR_STRING_MAX_LEN )
    {
        SECUREC_ERROR_INVALID_RANGE("wcscat_s");
        return ERANGE;
    }

    if (strDest == NULL || strSrc == NULL)
    {
        SECUREC_ERROR_INVALID_PARAMTER("wcscat_s");
        if (strDest != NULL )
        {
            pHeader[0] = '\0';
            return EINVAL_AND_RESET;
        }
        return EINVAL;
    }

    if (strDest < strSrc)
    {
        overlapGuard = strSrc;
        while (availableSize > 0 && *strDest != '\0')
        {
            if (strDest == overlapGuard)
            {
                pHeader[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("wcscat_s");
                return EOVERLAP_AND_RESET;
            }
            /*seek to string end*/
            strDest++;
            availableSize--;
        }

        /* strDest unterminated, return error. */
        if (availableSize == 0)
        {
            pHeader[0] = '\0';
            SECUREC_ERROR_INVALID_PARAMTER("wcscat_s");
            return EINVAL_AND_RESET;
        }

        /* if available > 0, then excute the strcat operation */
        while ((*strDest++ = *strSrc++) != '\0' && --availableSize > 0)
        {
            if ( strDest == overlapGuard)
            {
                pHeader[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("wcscat_s");
                return EOVERLAP_AND_RESET;
            }
        }
    }
    else
    {
        overlapGuard = strDest;
        while (availableSize > 0 && *strDest != '\0')
        {
            /*seek to string end, and no need to check overlap*/
            strDest++;
            availableSize--;
        }
        /* strDest unterminated, return error. */
        if (availableSize == 0)
        {
            pHeader[0] = '\0';
            SECUREC_ERROR_INVALID_PARAMTER("wcscat_s");
            return EINVAL_AND_RESET;
        }
        while ((*strDest++ = *strSrc++) != '\0' && --availableSize > 0)
        {
            if ( strSrc == overlapGuard)
            {
                pHeader[0] = '\0';
                SECUREC_ERROR_BUFFER_OVERLAP("wcscat_s");
                return EOVERLAP_AND_RESET;
            }
        }
    }

    /* strDest have not enough space, return error */
    if (availableSize == 0)
    {
        pHeader[0] = '\0';
        SECUREC_ERROR_INVALID_RANGE("wcscat_s");
        return ERANGE_AND_RESET;
    }

    return EOK;
}


