/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: strncat_s.c
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
 *    strncat_s
 *
 * <SYNOPSIS>
 *    errno_t
 *    strncat_s(char* strDest, size_t destMax, const char* strSrc, size_t count);
 *
 * <FUNCTION DESCRIPTION>
 *    The strncat_s function try to append the first D characters of strSrc to
 *    the end of strDest, where D is the lesser of count and the length of strSrc.
 *    If appending those D characters will fit within strDest (whose size is given
 *    as destMax) and still leave room for a null terminator, then those characters
 *    are appended, starting at the original terminating null of strDest, and a
 *    new terminating null is appended; otherwise, strDest[0] is set to the null
 *    character.
 *
 * <INPUT PARAMETERS>
 *    strDest                  Null-terminated destination string.
 *    destMax                  Size of the destination buffer.
 *    strSrc                   Null-terminated source string.
 *    count                    Number of character to append, or truncate.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                  is updated
 *
 * <RETURN VALUE>
 *    EOK                      Successful operation
 *    EINVAL                   strDest not terminated or NULL, or strSrc is NULL
 *    ERANGE                   destMax is 0, too big or too small, count >
 *                             SECUREC_STRING_MAX_LEN
 *    EOVERLAP_AND_RESET       dest buffer and source buffer are overlapped
 ********************************************************************************
*/
errno_t strncat_s(char *strDest, size_t destMax, const char *strSrc, size_t count)
{
	char *pHeader = strDest;
	size_t availableSize = destMax;
	IN_REGISTER const char *overlapGuard = NULL;

	if (destMax == 0 || destMax > SECUREC_STRING_MAX_LEN) {
		SECUREC_ERROR_INVALID_RANGE("strncat_s");
		return ERANGE;
	}

	if (strDest == NULL || strSrc == NULL) {
		SECUREC_ERROR_INVALID_PARAMTER("strncat_s");
		if (strDest != NULL) {
			pHeader[0] = '\0';
			return EINVAL_AND_RESET;
		}
		return EINVAL;
	}
#ifdef  COMPATIBLE_WIN_FORMAT
	if (count > SECUREC_STRING_MAX_LEN && count != (size_t) - 1)
#else
	if (count > SECUREC_STRING_MAX_LEN)
#endif
	{
		pHeader[0] = '\0';
		SECUREC_ERROR_INVALID_RANGE("strncat_s");
		return ERANGE_AND_RESET;
	}

	if (strDest < strSrc) {
		overlapGuard = strSrc;
		while (availableSize > 0 && *strDest != '\0') {
			if (strDest == overlapGuard) {
				pHeader[0] = '\0';
				SECUREC_ERROR_BUFFER_OVERLAP("strncat_s");
				return EOVERLAP_AND_RESET;
			}
			strDest++;
			availableSize--;
		}

		/* strDestination unterminated, return error. */
		if (availableSize == 0) {
			pHeader[0] = '\0';
			SECUREC_ERROR_INVALID_PARAMTER("strncat_s");
			return EINVAL_AND_RESET;
		}
		while (count > 0 && (*strDest++ = *strSrc++) != '\0' && --availableSize > 0) {
			if (strDest == overlapGuard) {
				pHeader[0] = '\0';
				SECUREC_ERROR_BUFFER_OVERLAP("strncat_s");
				return EOVERLAP_AND_RESET;
			}
			count--;
		}
	} else {
		overlapGuard = strDest;
		while (availableSize > 0 && *strDest != '\0') {
			/*seek to string end, and no need to check overlap*/
			strDest++;
			availableSize--;
		}

		/* strDest unterminated, return error. */
		if (availableSize == 0) {
			pHeader[0] = '\0';
			SECUREC_ERROR_INVALID_PARAMTER("strncat_s");
			return EINVAL_AND_RESET;
		}
		while (count > 0 && (*strDest++ = *strSrc++) != '\0' && --availableSize > 0) {
			if (strSrc == overlapGuard) {
				pHeader[0] = '\0';
				SECUREC_ERROR_BUFFER_OVERLAP("strncat_s");
				return EOVERLAP_AND_RESET;
			}
			count--;
		}
	}
	if (count == 0) {
		*strDest = 0; /* add terminator to strDest*/
	}

	/* strDest have not enough space,return error */
	if (availableSize == 0) {
		pHeader[0] = '\0';
		SECUREC_ERROR_INVALID_RANGE("strncat_s");
		return ERANGE_AND_RESET;
	}
	return EOK;
}

