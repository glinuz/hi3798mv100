/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: strcpy_s.c
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
 *    strcpy_s
 *
 * <SYNOPSIS>
 *    errno_t strcpy_s(char* strDest, size_t destMax, const char* strSrc)
 *
 * <FUNCTION DESCRIPTION>
 *    The strcpy_s function copies the contents in the address of strSrc,
 *    including the terminating null character, to the location specified by strDest.
 *    The destination string must be large enough to hold the source string,
 *    including the terminating null character. strcpy_s will return EOVERLAP_AND_RESET
 *    if the source and destination strings overlap.
 *
 * <INPUT PARAMETERS>
 *    strDest                  Location of destination string buffer
 *    destMax                  Size of the destination string buffer.
 *    strSrc                   Null-terminated source string buffer.
 *
 * <OUTPUT PARAMETERS>
 *    strDest                  is updated.
 *
 * <RETURN VALUE>
 *    0                        success
 *    EINVAL                   strDest == NULL or strSrc == NULL
 *    ERANGE                   destination buffer is NOT enough,  or size of
 *                             buffer is zero or greater than SECUREC_STRING_MAX_LEN
 *    EOVERLAP_AND_RESET       dest buffer and source buffer are overlapped
 *
 *    If there is a runtime-constraint violation, then if strDest is not a null
 *    pointer and destMax is greater than zero and not greater than
 *    SECUREC_STRING_MAX_LEN, then strcpy_s sets strDest[0] to the null character.
 *******************************************************************************
*/
#ifndef SECURE_STRCOPY_SIZE
#define SECURE_STRCOPY_SIZE (32UL)
#endif

#define SC_ADDR_ALIGNED(addr) !((size_t)addr & 7)

#define SECURE_STRCPY_BY_STRUCT(num) case num: *(MY_STR##num *)strDest=*(MY_STR##num *) strSrc; break;

#define SECURE_STRCPY_BY_BYTE(caseNum) case caseNum: *pcDest++ = *pcSrc++;

#define SMALL_STR_COPY \
    if (SC_ADDR_ALIGNED(strDest) && SC_ADDR_ALIGNED(strSrc) ) \
    { \
        /*use struct assignment*/ \
        switch(srcStrLen)\
        { \
        SECURE_STRCPY_BY_STRUCT(1)\
        SECURE_STRCPY_BY_STRUCT(2)\
        SECURE_STRCPY_BY_STRUCT(3)\
        SECURE_STRCPY_BY_STRUCT(4)\
        SECURE_STRCPY_BY_STRUCT(5)\
        SECURE_STRCPY_BY_STRUCT(6)\
        SECURE_STRCPY_BY_STRUCT(7)\
        SECURE_STRCPY_BY_STRUCT(8)\
        SECURE_STRCPY_BY_STRUCT(9)\
        SECURE_STRCPY_BY_STRUCT(10)\
        SECURE_STRCPY_BY_STRUCT(11)\
        SECURE_STRCPY_BY_STRUCT(12)\
        SECURE_STRCPY_BY_STRUCT(13)\
        SECURE_STRCPY_BY_STRUCT(14)\
        SECURE_STRCPY_BY_STRUCT(15)\
        SECURE_STRCPY_BY_STRUCT(16)\
        SECURE_STRCPY_BY_STRUCT(17)\
        SECURE_STRCPY_BY_STRUCT(18)\
        SECURE_STRCPY_BY_STRUCT(19)\
        SECURE_STRCPY_BY_STRUCT(20)\
        SECURE_STRCPY_BY_STRUCT(21)\
        SECURE_STRCPY_BY_STRUCT(22)\
        SECURE_STRCPY_BY_STRUCT(23)\
        SECURE_STRCPY_BY_STRUCT(24)\
        SECURE_STRCPY_BY_STRUCT(25)\
        SECURE_STRCPY_BY_STRUCT(26)\
        SECURE_STRCPY_BY_STRUCT(27)\
        SECURE_STRCPY_BY_STRUCT(28)\
        SECURE_STRCPY_BY_STRUCT(29)\
        SECURE_STRCPY_BY_STRUCT(30)\
        SECURE_STRCPY_BY_STRUCT(31)\
        SECURE_STRCPY_BY_STRUCT(32)\
        }/*END switch*/ \
    } \
    else \
    { \
        char* pcDest = (char*)strDest; \
        char* pcSrc =  (char*)strSrc; \
        switch(srcStrLen) \
        { /*lint -save -e616*/\
        SECURE_STRCPY_BY_BYTE(32)\
        SECURE_STRCPY_BY_BYTE(31)\
        SECURE_STRCPY_BY_BYTE(30)\
        SECURE_STRCPY_BY_BYTE(29)\
        SECURE_STRCPY_BY_BYTE(28)\
        SECURE_STRCPY_BY_BYTE(27)\
        SECURE_STRCPY_BY_BYTE(26)\
        SECURE_STRCPY_BY_BYTE(25)\
        SECURE_STRCPY_BY_BYTE(24)\
        SECURE_STRCPY_BY_BYTE(23)\
        SECURE_STRCPY_BY_BYTE(22)\
        SECURE_STRCPY_BY_BYTE(21)\
        SECURE_STRCPY_BY_BYTE(20)\
        SECURE_STRCPY_BY_BYTE(19)\
        SECURE_STRCPY_BY_BYTE(18)\
        SECURE_STRCPY_BY_BYTE(17)\
        SECURE_STRCPY_BY_BYTE(16)\
        SECURE_STRCPY_BY_BYTE(15)\
        SECURE_STRCPY_BY_BYTE(14)\
        SECURE_STRCPY_BY_BYTE(13)\
        SECURE_STRCPY_BY_BYTE(12)\
        SECURE_STRCPY_BY_BYTE(11)\
        SECURE_STRCPY_BY_BYTE(10)\
        SECURE_STRCPY_BY_BYTE(9)\
        SECURE_STRCPY_BY_BYTE(8)\
        SECURE_STRCPY_BY_BYTE(7)\
        SECURE_STRCPY_BY_BYTE(6)\
        SECURE_STRCPY_BY_BYTE(5)\
        SECURE_STRCPY_BY_BYTE(4)\
        SECURE_STRCPY_BY_BYTE(3)\
        SECURE_STRCPY_BY_BYTE(2)\
        SECURE_STRCPY_BY_BYTE(1)\
        } /*lint -restore */\
    } \
    return EOK;

errno_t strcpy_error(char *strDest, size_t destMax, const char *strSrc)
{
	if (destMax == 0 || destMax > SECUREC_STRING_MAX_LEN) {
		SECUREC_ERROR_INVALID_RANGE("strcpy_s");
		return ERANGE;
	} else if (strDest == NULL || strSrc == NULL) {
		SECUREC_ERROR_INVALID_PARAMTER("strcpy_s");
		if (strDest != NULL) {
			strDest[0] = '\0';
			return EINVAL_AND_RESET;
		}
		return EINVAL;
	} else if (strlen(strSrc) + 1 > destMax) {
		strDest[0] = '\0';
		SECUREC_ERROR_INVALID_RANGE("strcpy_s");
		return ERANGE_AND_RESET;
	} else {
		return EOK;
	}
}

errno_t strcpy_s(char *strDest, size_t destMax, const char *strSrc)
{
	if ((destMax > 0 && destMax <= SECUREC_STRING_MAX_LEN && strDest != NULL && strSrc != NULL && strDest != strSrc)) {
		const char *endPos = strSrc;
		size_t srcStrLen = destMax;  /* use it to store the maxi length limit */
		while (*(endPos++) && srcStrLen-- > 0);   /* use srcStrLen as boundary checker */

		srcStrLen = endPos - strSrc ; /*with ending terminator*/
		if (srcStrLen <= destMax) {
			if (strDest < strSrc) {
				if (strDest + srcStrLen <= strSrc) {
					if (srcStrLen > SECURE_STRCOPY_SIZE) {
						(void)memcpy(strDest, strSrc, srcStrLen);
					} else {
						SMALL_STR_COPY
					}
					return EOK;
				} else {
					strDest[0] = '\0';
					SECUREC_ERROR_BUFFER_OVERLAP("strcpy_s");
					return EOVERLAP_AND_RESET;
				}
			} else {
				if (strSrc + srcStrLen <= strDest) {
					if (srcStrLen > SECURE_STRCOPY_SIZE) {
						(void)memcpy(strDest, strSrc, srcStrLen);
					} else {
						SMALL_STR_COPY
					}
					return EOK;
				} else {
					strDest[0] = '\0';
					SECUREC_ERROR_BUFFER_OVERLAP("strcpy_s");
					return EOVERLAP_AND_RESET;
				}
			}
		}
	}
	return strcpy_error(strDest, destMax, strSrc);
}

