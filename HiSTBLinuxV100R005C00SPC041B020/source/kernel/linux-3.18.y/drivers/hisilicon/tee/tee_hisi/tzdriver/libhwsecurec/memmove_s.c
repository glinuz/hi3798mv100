/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: memmove_s.c
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
 *    memmove_s
 *
 * <SYNOPSIS>
 *    errno_t memmove_s(void *dest, size_t destMax, const void *src, size_t count);
 *
 * <FUNCTION DESCRIPTION>
 *    Copies count bytes of characters from src to dest.
 *
 * <INPUT PARAMETERS>
 *    dest                       Destination object.
 *    destMax                    Size of the destination buffer.
 *    src                        Source object.
 *    count                      Number of characters to copy.
 *
 * <OUTPUT PARAMETERS>
 *    dest buffer                is uptdated.
 *
 * <RETURN VALUE>
 *    EOK                        Success
 *    EINVAL                     dest == NULL or strSrc == NULL
 *    ERANGE                     count > destMax or destMax > SECUREC_MEM_MAX_LEN
 *                               or destMax == 0
 *
 *    If an error occured, dest will NOT be filled with 0.
 *    If some regions of the source area and the destination overlap, memmove_s
 *    ensures that the original source bytes in the overlapping region are copied
 *    before being overwritten.
 *******************************************************************************
*/

errno_t memmove_s(void *dest, size_t destMax, const void *src, size_t count)
{
	if (destMax == 0 || destMax > SECUREC_MEM_MAX_LEN) {
		SECUREC_ERROR_INVALID_RANGE("memmove_s");
		return ERANGE;
	}
	if (dest == NULL || src == NULL) {
		SECUREC_ERROR_INVALID_PARAMTER("memmove_s");
		if (dest != NULL) {
			(void)memset(dest, 0, destMax);
			return EINVAL_AND_RESET;
		}
		return EINVAL;
	}
	if (count > destMax) {
		(void)memset(dest, 0, destMax);
		SECUREC_ERROR_INVALID_RANGE("memmove_s");
		return ERANGE_AND_RESET;
	}
	if (dest == src) {
		return EOK;
	}

	if (count > 0) {
#ifdef CALL_LIBC_COR_API
		/*use underlying memmove for performance consideration*/
		(void)memmove(dest, src, count);
#else

		util_memmove(dest, src, count);
#endif
	}
	return EOK;
}


