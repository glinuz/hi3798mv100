/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: wmemmove_s.c
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
 *    wmemmove_s
 *
 * <SYNOPSIS>
 *    errno_t wmemmove_s(wchar_t* dest, size_t destMax, const wchar_t* src, size_t count);
 *
 * <FUNCTION DESCRIPTION>
 *    wmemmove_s copies count wide character(two bytes) from src to dest
 *
 * <INPUT PARAMETERS>
 *    dest                     destination object.
 *    destMax                  Size of the destination buffer.
 *    src                      Source object.
 *    count                    Number of bytes or character to copy.
 *
 * <OUTPUT PARAMETERS>
 *    dest                  is updated.
 *
 * <RETURN VALUE>
 *     EOK                     success
 *     EINVAL                  dest == NULL or src == NULL
 *     ERANGE                  count > destMax or destMax > SECUREC_WCHAR_MEM_MAX_LEN
 *                             or destMax == 0
 *
 *     If an error occured, dest will NOT be filled with 0.
 *     If some regions of the source area and the destination overlap, wmemmove_s
 *     ensures that the original source bytes in the overlapping region are copied
 *     before being overwritten
 ********************************************************************************
*/

errno_t wmemmove_s(wchar_t* dest, size_t destMax, const wchar_t* src, size_t count)
{
    if (destMax == 0 || destMax > SECUREC_WCHAR_MEM_MAX_LEN )
    {
        SECUREC_ERROR_INVALID_PARAMTER("wmemmove_s");
        return ERANGE;
    }
    if ( count > destMax)
    {
        SECUREC_ERROR_INVALID_PARAMTER("wmemmove_s");
        if (dest != NULL)
        {
            (void)memset(dest, 0, destMax * WCHAR_SIZE);
            return ERANGE_AND_RESET;
        }
        return ERANGE;
    }
    return memmove_s(dest, destMax * WCHAR_SIZE, src, count * WCHAR_SIZE);
}


