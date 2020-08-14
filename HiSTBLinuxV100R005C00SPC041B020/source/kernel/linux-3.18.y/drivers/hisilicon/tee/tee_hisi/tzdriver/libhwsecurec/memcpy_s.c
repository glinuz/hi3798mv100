/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: memcpy_s.c
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
*    memcpy_s
*
* <SYNOPSIS>
*    errno_t memcpy_s(void *dest, size_t destMax, const void *src, size_t count);
*
* <FUNCTION DESCRIPTION>
*    memcpy_s copies count bytes from src to dest
*
* <INPUT PARAMETERS>
*    dest                       new buffer.
*    destMax                    Size of the destination buffer.
*    src                        Buffer to copy from.
*    count                      Number of characters to copy
*
* <OUTPUT PARAMETERS>
*    dest buffer                is updated.
*
* <RETURN VALUE>
*    EOK                        Success
*    EINVAL                     dest == NULL or strSrc == NULL
*    ERANGE                     count > destMax or destMax >
*                               SECUREC_MEM_MAX_LEN or destMax == 0
*    EOVERLAP_AND_RESET         dest buffer and source buffer are overlapped
*
*    if an error occured, dest will be filled with 0.
*    If the source and destination overlap, the behavior of memcpy_s is undefined.
*    Use memmove_s to handle overlapping regions.
*******************************************************************************
*/

/* assembly language memcpy for X86 or MIPS */
extern void *memcpy_opt(void *dest, const void *src, size_t n);

/*
if USE_ASM macro is enabled, it will call assembly language function to improve performance.
*/
#ifndef SECURE_MEMCOPY_THRESHOLD_SIZE
#define SECURE_MEMCOPY_THRESHOLD_SIZE (64UL)
#endif

#define SC_ADDR_ALIGNED(addr) !((size_t)addr & 7)

#define SECURE_COPY_STRUCT(num) case num:*(MY_STR##num *)dest=*(MY_STR##num *) src;break;

#define SECURE_COPY_BY_BYTE(caseNum) case caseNum: *pcDest++ = *pcSrc++;

#define SMALL_MEM_COPY \
    if (SC_ADDR_ALIGNED(dest) && SC_ADDR_ALIGNED(src) ) \
    { \
        /*use struct assignment*/ \
        switch(count)\
        { \
            SECURE_COPY_STRUCT(1)\
            SECURE_COPY_STRUCT(2)\
            SECURE_COPY_STRUCT(3)\
            SECURE_COPY_STRUCT(4)\
            SECURE_COPY_STRUCT(5)\
            SECURE_COPY_STRUCT(6)\
            SECURE_COPY_STRUCT(7)\
            SECURE_COPY_STRUCT(8)\
            SECURE_COPY_STRUCT(9)\
            SECURE_COPY_STRUCT(10)\
            SECURE_COPY_STRUCT(11)\
            SECURE_COPY_STRUCT(12)\
            SECURE_COPY_STRUCT(13)\
            SECURE_COPY_STRUCT(14)\
            SECURE_COPY_STRUCT(15)\
            SECURE_COPY_STRUCT(16)\
            SECURE_COPY_STRUCT(17)\
            SECURE_COPY_STRUCT(18)\
            SECURE_COPY_STRUCT(19)\
            SECURE_COPY_STRUCT(20)\
            SECURE_COPY_STRUCT(21)\
            SECURE_COPY_STRUCT(22)\
            SECURE_COPY_STRUCT(23)\
            SECURE_COPY_STRUCT(24)\
            SECURE_COPY_STRUCT(25)\
            SECURE_COPY_STRUCT(26)\
            SECURE_COPY_STRUCT(27)\
            SECURE_COPY_STRUCT(28)\
            SECURE_COPY_STRUCT(29)\
            SECURE_COPY_STRUCT(30)\
            SECURE_COPY_STRUCT(31)\
            SECURE_COPY_STRUCT(32)\
            SECURE_COPY_STRUCT(33)\
            SECURE_COPY_STRUCT(34)\
            SECURE_COPY_STRUCT(35)\
            SECURE_COPY_STRUCT(36)\
            SECURE_COPY_STRUCT(37)\
            SECURE_COPY_STRUCT(38)\
            SECURE_COPY_STRUCT(39)\
            SECURE_COPY_STRUCT(40)\
            SECURE_COPY_STRUCT(41)\
            SECURE_COPY_STRUCT(42)\
            SECURE_COPY_STRUCT(43)\
            SECURE_COPY_STRUCT(44)\
            SECURE_COPY_STRUCT(45)\
            SECURE_COPY_STRUCT(46)\
            SECURE_COPY_STRUCT(47)\
            SECURE_COPY_STRUCT(48)\
            SECURE_COPY_STRUCT(49)\
            SECURE_COPY_STRUCT(50)\
            SECURE_COPY_STRUCT(51)\
            SECURE_COPY_STRUCT(52)\
            SECURE_COPY_STRUCT(53)\
            SECURE_COPY_STRUCT(54)\
            SECURE_COPY_STRUCT(55)\
            SECURE_COPY_STRUCT(56)\
            SECURE_COPY_STRUCT(57)\
            SECURE_COPY_STRUCT(58)\
            SECURE_COPY_STRUCT(59)\
            SECURE_COPY_STRUCT(60)\
            SECURE_COPY_STRUCT(61)\
            SECURE_COPY_STRUCT(62)\
            SECURE_COPY_STRUCT(63)\
            SECURE_COPY_STRUCT(64)\
        }/*END switch*/ \
    } \
    else \
    { \
        char* pcDest = (char*)dest; \
        char* pcSrc =  (char*)src; \
        switch(count) \
        { /*lint -save -e616*/\
            SECURE_COPY_BY_BYTE(64)\
            SECURE_COPY_BY_BYTE(63)\
            SECURE_COPY_BY_BYTE(62)\
            SECURE_COPY_BY_BYTE(61)\
            SECURE_COPY_BY_BYTE(60)\
            SECURE_COPY_BY_BYTE(59)\
            SECURE_COPY_BY_BYTE(58)\
            SECURE_COPY_BY_BYTE(57)\
            SECURE_COPY_BY_BYTE(56)\
            SECURE_COPY_BY_BYTE(55)\
            SECURE_COPY_BY_BYTE(54)\
            SECURE_COPY_BY_BYTE(53)\
            SECURE_COPY_BY_BYTE(52)\
            SECURE_COPY_BY_BYTE(51)\
            SECURE_COPY_BY_BYTE(50)\
            SECURE_COPY_BY_BYTE(49)\
            SECURE_COPY_BY_BYTE(48)\
            SECURE_COPY_BY_BYTE(47)\
            SECURE_COPY_BY_BYTE(46)\
            SECURE_COPY_BY_BYTE(45)\
            SECURE_COPY_BY_BYTE(44)\
            SECURE_COPY_BY_BYTE(43)\
            SECURE_COPY_BY_BYTE(42)\
            SECURE_COPY_BY_BYTE(41)\
            SECURE_COPY_BY_BYTE(40)\
            SECURE_COPY_BY_BYTE(39)\
            SECURE_COPY_BY_BYTE(38)\
            SECURE_COPY_BY_BYTE(37)\
            SECURE_COPY_BY_BYTE(36)\
            SECURE_COPY_BY_BYTE(35)\
            SECURE_COPY_BY_BYTE(34)\
            SECURE_COPY_BY_BYTE(33)\
            SECURE_COPY_BY_BYTE(32)\
            SECURE_COPY_BY_BYTE(31)\
            SECURE_COPY_BY_BYTE(30)\
            SECURE_COPY_BY_BYTE(29)\
            SECURE_COPY_BY_BYTE(28)\
            SECURE_COPY_BY_BYTE(27)\
            SECURE_COPY_BY_BYTE(26)\
            SECURE_COPY_BY_BYTE(25)\
            SECURE_COPY_BY_BYTE(24)\
            SECURE_COPY_BY_BYTE(23)\
            SECURE_COPY_BY_BYTE(22)\
            SECURE_COPY_BY_BYTE(21)\
            SECURE_COPY_BY_BYTE(20)\
            SECURE_COPY_BY_BYTE(19)\
            SECURE_COPY_BY_BYTE(18)\
            SECURE_COPY_BY_BYTE(17)\
            SECURE_COPY_BY_BYTE(16)\
            SECURE_COPY_BY_BYTE(15)\
            SECURE_COPY_BY_BYTE(14)\
            SECURE_COPY_BY_BYTE(13)\
            SECURE_COPY_BY_BYTE(12)\
            SECURE_COPY_BY_BYTE(11)\
            SECURE_COPY_BY_BYTE(10)\
            SECURE_COPY_BY_BYTE(9)\
            SECURE_COPY_BY_BYTE(8)\
            SECURE_COPY_BY_BYTE(7)\
            SECURE_COPY_BY_BYTE(6)\
            SECURE_COPY_BY_BYTE(5)\
            SECURE_COPY_BY_BYTE(4)\
            SECURE_COPY_BY_BYTE(3)\
            SECURE_COPY_BY_BYTE(2)\
            SECURE_COPY_BY_BYTE(1)\
        } /*lint -restore */\
    }

#define SECC_RET_MEMCPY_ECODE  \
    if (destMax == 0 || destMax > SECUREC_MEM_MAX_LEN ) \
{ \
    SECUREC_ERROR_INVALID_RANGE("memcpy_s"); \
    return ERANGE; \
    } \
    if (dest == NULL || src == NULL) \
{ \
    SECUREC_ERROR_INVALID_PARAMTER("memcpy_s"); \
    if (dest != NULL ) \
{ \
    (void)memset(dest, 0, destMax); \
    return EINVAL_AND_RESET; \
    } \
    return EINVAL; \
    } \
    if (count > destMax) \
{ \
    (void)memset(dest, 0, destMax); \
    SECUREC_ERROR_INVALID_RANGE("memcpy_s"); \
    return ERANGE_AND_RESET; \
    } \
    if (dest == src) \
{ \
    return EOK; \
    } \
    if ((dest > src && dest < (void*)((UINT8T*)src + count)) || \
    (src > dest && src < (void*)((UINT8T*)dest + count)) ) \
{ \
    (void)memset(dest, 0, destMax); \
    SECUREC_ERROR_BUFFER_OVERLAP("memcpy_s"); \
    return EOVERLAP_AND_RESET; \
    } \
    return EOK;



errno_t memcpy_s(void *dest, size_t destMax, const void *src, size_t count)
{
#if defined(COMPATIBLE_WIN_FORMAT)
	/* fread API in windows will call memcpy_s and pass 0xffffffff to destMax. To avoid the failure of fread, we don't check desMax limit. */
	if (LIKELY(count <= destMax && dest && src    /*&& dest != src*/
		   && count > 0
		   && ((dest > src  && (void *)((UINT8T *)src  + count) <= dest) ||
		       (src  > dest && (void *)((UINT8T *)dest + count) <= src))
		  ))
#else
	if (LIKELY(count <= destMax && dest && src    /*&& dest != src*/
		   && destMax <= SECUREC_MEM_MAX_LEN
		   && count > 0
		   && ((dest > src  && (void *)((UINT8T *)src  + count) <= dest) ||
		       (src  > dest && (void *)((UINT8T *)dest + count) <= src))
		  ))
#endif
	{
		if (count > SECURE_MEMCOPY_THRESHOLD_SIZE) {
			/*large enough, let system API do it*/
#ifdef USE_ASM
			memcpy_opt(dest, src, count);
#else
			(void)memcpy(dest, src, count);
#endif
			return EOK;
		} else {
			SMALL_MEM_COPY;
			return EOK;
		}
	} else {
		/* meet some runtime violation, return error code */
		SECC_RET_MEMCPY_ECODE
	}
}


#if defined(WITH_PERFORMANCE_ADDONS)

/*trim judgement on "destMax <= SECUREC_MEM_MAX_LEN"  */
errno_t memcpy_sOptTc(void *dest, size_t destMax, const void *src, size_t count)
{
	if (LIKELY(count <= destMax && dest && src    /*&& dest != src*/
		   && count > 0
		   && ((dest > src  && (void *)((UINT8T *)src  + count) <= dest) ||
		       (src  > dest && (void *)((UINT8T *)dest + count) <= src))
		  )) {
		if (count > SECURE_MEMCOPY_THRESHOLD_SIZE)  {
			/*large enough, let system API do it*/
#ifdef USE_ASM
			memcpy_opt(dest, src, count);
#else
			(void)memcpy(dest, src, count);
#endif
			return EOK;
		} else {
			SMALL_MEM_COPY;
			return EOK;
		}
	} else {
		/* meet some runtime violation, return error code */
		SECC_RET_MEMCPY_ECODE
	}
}
#endif /* WITH_PERFORMANCE_ADDONS */

