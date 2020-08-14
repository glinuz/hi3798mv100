/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: strncpy_s.c
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
 *    strncpy_s
 *
 * <SYNOPSIS>
 *    errno_t strncpy_s( char* strDest, size_t destMax, const char* strSrc, size_t count);
 *
 * <FUNCTION DESCRIPTION>
 *    Copy the contents from strSrc, including the terminating null character,
 *    to the location specified by strDest.
 *
 * <INPUT PARAMETERS>
 *    strDest                     Destination string.
 *    destMax                     The size of the destination string, in characters.
 *    strSrc                      Source string.
 *    count                       Number of characters to be copied.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                     is updated
 *
 * <RETURN VALUE>
 *    EOK(0)                      success
 *    EINVAL                      strDest == NULL or strSrc == NULL
 *    ERANGE                      destMax is zero or greater than SECUREC_STRING_MAX_LEN,
 *                                or count > SECUREC_STRING_MAX_LEN, or destMax is too small
 *    EOVERLAP_AND_RESET          buffer and source buffer are overlapped
 *
 *    If there is a runtime-constraint violation, then if strDest is not a null
 *    pointer and destMax is greater than zero and not greater than SECUREC_STRING_MAX_LEN,
 *    then strncpy_s sets strDest[0] to the null character.
 *******************************************************************************
*/

errno_t strncpy_error(char *strDest, size_t destMax, const char *strSrc, size_t count)
{
	if (destMax == 0 || destMax > SECUREC_STRING_MAX_LEN) {
		SECUREC_ERROR_INVALID_RANGE("strncpy_s");
		return ERANGE;
	} else  if (strDest == NULL || strSrc == NULL) {
		SECUREC_ERROR_INVALID_PARAMTER("strncpy_s");
		if (strDest != NULL) {
			strDest[0] = '\0';
			return EINVAL_AND_RESET;
		}
		return EINVAL;
	} else if (count > SECUREC_STRING_MAX_LEN) {
		strDest[0] = '\0'; /*clear dest string*/
		SECUREC_ERROR_INVALID_RANGE("strncpy_s");
		return ERANGE_AND_RESET;
	} else if (count == 0) {
		strDest[0] = '\0';
		return EOK;
	} else {
		size_t srcStrLen = strlen(strSrc);
		if ((size_t)(TWO_MIN(srcStrLen, count) + 1) > destMax) {
			strDest[0] = '\0';
			SECUREC_ERROR_INVALID_RANGE("strncpy_s");
			return ERANGE_AND_RESET;
		} else {
			return EOK;
		}
	}
}

errno_t strncpy_s(char *strDest, size_t destMax, const char *strSrc, size_t count)
{
#ifdef  COMPATIBLE_WIN_FORMAT
	if ((destMax > 0 && destMax <= SECUREC_STRING_MAX_LEN && strDest != NULL && strSrc != NULL && (count <= SECUREC_STRING_MAX_LEN || count == (size_t) - 1) && count > 0)) {
#else
	if ((destMax > 0 && destMax <= SECUREC_STRING_MAX_LEN && strDest != NULL && strSrc != NULL && count <= SECUREC_STRING_MAX_LEN && count > 0)) {
#endif

		const char *endPos = strSrc;
		int fillEndingZero = 0;   /* flag for setting ending terminator for dest string */
		size_t destMaxBk = destMax, minCpLen = count; /* use it to store the maxi length limit */
		while (*(endPos++) && --destMaxBk > 0 && --minCpLen > 0);  /* use srcStrLen and minCpLen as boundary checker */

		if (0 == destMaxBk) {
			strDest[0] = '\0';
			SECUREC_ERROR_BUFFER_OVERLAP("strncpy_s");
			return ERANGE_AND_RESET;
		}
		if (0 == minCpLen) {
			minCpLen = count + 1; /* add ending terminator */
			fillEndingZero = 1;
		} else {
			minCpLen = endPos - strSrc;  /* with ending terminator */
		}


		if ((strDest < strSrc && strDest + minCpLen <= strSrc)
		    || (strSrc < strDest && strSrc + minCpLen <= strDest)
		    || strDest == strSrc) {
			/*Not overlap*/
			if (fillEndingZero) {
				(void)memcpy(strDest, strSrc, minCpLen - 1); /* copy string by count bytes */
				strDest[minCpLen - 1] = '\0';
			} else {
				(void)memcpy(strDest, strSrc, minCpLen);   /* copy string with terminator */
			}
			return EOK;
		} else {
			strDest[0] = '\0';
			SECUREC_ERROR_BUFFER_OVERLAP("strncpy_s");
			return EOVERLAP_AND_RESET;
		}
	}
	return strncpy_error(strDest, destMax, strSrc, count);
}


