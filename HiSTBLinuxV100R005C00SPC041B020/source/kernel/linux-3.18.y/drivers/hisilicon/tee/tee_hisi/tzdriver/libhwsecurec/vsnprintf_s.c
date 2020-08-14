/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: vsnprintf.c
* History:
*     1. Date: 2014/5/20
*         Author: LiShunda
*         Modification: change #include "securectype.h" to #include "securec.h", which
*                      will cause link error under C++ compiler.
********************************************************************************
*/

#include "securec.h"
#include "securecutil.h"
#include <stdarg.h>

/*******************************************************************************
 * <NAME>
 *    vsnprintf_s
 *
 * <SYNOPSIS>
 *    int vsnprintf_s(char* strDest, size_t destMax, size_t count, const char* format, valist ap);
 *
 * <FUNCTION DESCRIPTION>
 *    The vsnprintf_s function takes a pointer to an argument list, then formats
 *    and writes up to count characters of the given data to the memory pointed
 *    to by strDest and appends a terminating null.
 *
 * <INPUT PARAMETERS>
 *    strDest                Storage location for the output.
 *    destMax                The size of the strDest for output.
 *    count                  Maximum number of character to write(not including
 *                           the terminating NULL)
 *    format                 Format-control string.
 *    ap                     pointer to list of arguments.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                is updated
 *
 * <RETURN VALUE>
 *    vsnprintf_s returns the number of characters written, not including the
 *    terminating null, or a negative value if an output error occurs. vsnprintf_s
 *    is included for compliance to the ANSI standard.
 *    If the storage required to store the data and a terminating null exceeds
 *    destMax, the function set strDest to an empty strDest, and return -1.
 *    If strDest or format is a NULL pointer, or if count is less than or equal
 *    to zero, the function return -1.
 *
 *    ERROR CONDITIONS:
 *    Condition                       Return
 *    strDest is NULL                 -1
 *    format is NULL                  -1
 *    count <= 0                      -1
 *    destMax too small               -1(and strDest set to an empty string)
 *******************************************************************************
*/


int vsnprintf_s(char *strDest, size_t destMax, size_t count, const char *format, va_list arglist)
{
	int retvalue = -1;

	if (format == NULL || strDest == NULL || destMax == 0 || destMax > SECUREC_STRING_MAX_LEN || (count > (SECUREC_STRING_MAX_LEN - 1) &&  count != (size_t) - 1)) {
		if (strDest != NULL && destMax > 0) {
			strDest[0] = '\0';
		}
		SECUREC_ERROR_INVALID_PARAMTER("vsnprintf_s");
		return -1;
	}

	if (destMax > count) {
		retvalue = vsnprintf_helper(strDest, count + 1, format, arglist);
		if (retvalue == -2) {  /* lsd add to keep dest buffer not destroyed 2014.2.18 */
			/* the string has been truncated, return  -1 */
			return -1;     /* to skip error handler,  return strlen(strDest) or -1 */
		}
	} else { /* destMax <= count */
		retvalue = vsnprintf_helper(strDest, destMax, format, arglist);
		strDest[destMax - 1] = 0;
#ifdef COMPATIBLE_WIN_FORMAT
		if (retvalue == -2 && count == (size_t) - 1) {
			return -1;
		}
#endif
	}

	if (retvalue < 0) {
		strDest[0] = '\0';    /* empty the dest strDest */

		if (retvalue == -2) {
			/* Buffer too small */
			SECUREC_ERROR_INVALID_RANGE("vsnprintf_s");
		}

		SECUREC_ERROR_INVALID_PARAMTER("vsnprintf_s");
		return -1;
	}

	return retvalue;
}


