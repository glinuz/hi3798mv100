/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: wmemcpy_s.c
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
 *    wmemcpy_s
 *
 * <SYNOPSIS>
 *    errno_t wmemcpy_s(wchar_t* dest, size_t destMax, const wchar_t* src, size_t count);
 *
 * <FUNCTION DESCRIPTION>
 *    wmemcpy_s copies count wide character(two bytes) from src to dest.
 *
 * <INPUT PARAMETERS>
 *    dest                  new buffer.
 *    destMax               Size of the destination buffer.
 *    src                   Buffer to copy from.
 *    count                 Number of characters to copy.
 *
 * <OUTPUT PARAMETERS>
 *    dest buffer           is uptdated.
 *
 * <RETURN VALUE>
 *    0                     Success
 *    EINVAL                dest == NULL or src == NULL
 *    ERANGE                count > destMax or destMax > SECUREC_WCHAR_MEM_MAX_LEN
 *                          or destMax == 0
 *    EOVERLAP_AND_RESET    dest buffer and source buffer are overlapped
 *
 *    if an error occured, dest will be filled with 0.
 *    If the source and destination overlap, the behavior of wmemcpy_s is undefined.
 *    Use wmemmove_s to handle overlapping regions.
 *******************************************************************************
*/

errno_t wmemcpy_s(wchar_t* dest, size_t destMax, const wchar_t* src, size_t count)
{
    if (destMax == 0 || destMax > SECUREC_WCHAR_MEM_MAX_LEN )
    {
        SECUREC_ERROR_INVALID_PARAMTER("wmemcpy_s");
        return ERANGE;
    }
    if (count > destMax)
    {
        SECUREC_ERROR_INVALID_PARAMTER("wmemcpy_s");
        if (dest != NULL)
        {
            (void)memset(dest, 0, destMax * WCHAR_SIZE);
            return ERANGE_AND_RESET;
        }
        return ERANGE;
    }
    return memcpy_s(dest, destMax * WCHAR_SIZE, src, count * WCHAR_SIZE);
}


