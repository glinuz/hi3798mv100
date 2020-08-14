/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: memset_s.c
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
*    memset_s
*
* <SYNOPSIS>
*    errno_t memset_s(void* dest, size_t destMax, int c, size_t count)
*
* <FUNCTION DESCRIPTION>
*    Sets buffers to a specified character.
*
* <INPUT PARAMETERS>
*    dest                       Pointer to destination.
*    destMax                    The size of the buffer.
*    c                          Character to set.
*    count                      Number of characters.
*
* <OUTPUT PARAMETERS>
*    dest buffer                is uptdated.
*
* <RETURN VALUE>
*    EOK                        Success
*    EINVAL                     dest == NULL
*    ERANGE                     count > destMax or destMax > SECUREC_MEM_MAX_LEN
*                               or destMax == 0
*******************************************************************************
*/

/*assemble language memset*/
extern void *memset_opt(void *d, int c, size_t cnt);

static const MY_STR32 myStr = {{'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'}}; /*static variable always set zero*/ /*lint !e784 */
static const MY_STR32 myStrAllFF = {{'\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF'}}; /*lint !e784 */

#ifndef SECURE_MEMSET_THRESHOLD_SIZE
#define SECURE_MEMSET_THRESHOLD_SIZE (32UL)
#endif

#define SC_ADDR_ALIGNED(addr) !((size_t)addr & 7)

#define COPY_STRUCT_4_ZERO(caseNum)\
        case caseNum:*(MY_STR##caseNum *)dest=*(MY_STR##caseNum *)(&myStr);break;

#define COPY_STRUCT_4_FF(caseNum)\
        case caseNum:*(MY_STR##caseNum *)dest=*(MY_STR##caseNum *)(&myStrAllFF);break;

#define SECURE_MEMSET_CONST(caseNum,addr,c) case caseNum: *addr++ = (char)c;

#define SC_ALIGNED_COPY \
    switch(c) { \
        case 0: \
            switch(count)\
            { \
                COPY_STRUCT_4_ZERO(1)\
                COPY_STRUCT_4_ZERO(2)\
                COPY_STRUCT_4_ZERO(3)\
                COPY_STRUCT_4_ZERO(4)\
                COPY_STRUCT_4_ZERO(5)\
                COPY_STRUCT_4_ZERO(6)\
                COPY_STRUCT_4_ZERO(7)\
                COPY_STRUCT_4_ZERO(8)\
                COPY_STRUCT_4_ZERO(9)\
                COPY_STRUCT_4_ZERO(10)\
                COPY_STRUCT_4_ZERO(11)\
                COPY_STRUCT_4_ZERO(12)\
                COPY_STRUCT_4_ZERO(13)\
                COPY_STRUCT_4_ZERO(14)\
                COPY_STRUCT_4_ZERO(15)\
                COPY_STRUCT_4_ZERO(16)\
                COPY_STRUCT_4_ZERO(17)\
                COPY_STRUCT_4_ZERO(18)\
                COPY_STRUCT_4_ZERO(19)\
                COPY_STRUCT_4_ZERO(20)\
                COPY_STRUCT_4_ZERO(21)\
                COPY_STRUCT_4_ZERO(22)\
                COPY_STRUCT_4_ZERO(23)\
                COPY_STRUCT_4_ZERO(24)\
                COPY_STRUCT_4_ZERO(25)\
                COPY_STRUCT_4_ZERO(26)\
                COPY_STRUCT_4_ZERO(27)\
                COPY_STRUCT_4_ZERO(28)\
                COPY_STRUCT_4_ZERO(29)\
                COPY_STRUCT_4_ZERO(30)\
                COPY_STRUCT_4_ZERO(31)\
                COPY_STRUCT_4_ZERO(32)\
            } \
            return EOK; \
        case 0xFF: \
            switch(count)\
            { \
                COPY_STRUCT_4_FF(1)\
                COPY_STRUCT_4_FF(2)\
                COPY_STRUCT_4_FF(3)\
                COPY_STRUCT_4_FF(4)\
                COPY_STRUCT_4_FF(5)\
                COPY_STRUCT_4_FF(6)\
                COPY_STRUCT_4_FF(7)\
                COPY_STRUCT_4_FF(8)\
                COPY_STRUCT_4_FF(9)\
                COPY_STRUCT_4_FF(10)\
                COPY_STRUCT_4_FF(11)\
                COPY_STRUCT_4_FF(12)\
                COPY_STRUCT_4_FF(13)\
                COPY_STRUCT_4_FF(14)\
                COPY_STRUCT_4_FF(15)\
                COPY_STRUCT_4_FF(16)\
                COPY_STRUCT_4_FF(17)\
                COPY_STRUCT_4_FF(18)\
                COPY_STRUCT_4_FF(19)\
                COPY_STRUCT_4_FF(20)\
                COPY_STRUCT_4_FF(21)\
                COPY_STRUCT_4_FF(22)\
                COPY_STRUCT_4_FF(23)\
                COPY_STRUCT_4_FF(24)\
                COPY_STRUCT_4_FF(25)\
                COPY_STRUCT_4_FF(26)\
                COPY_STRUCT_4_FF(27)\
                COPY_STRUCT_4_FF(28)\
                COPY_STRUCT_4_FF(29)\
                COPY_STRUCT_4_FF(30)\
                COPY_STRUCT_4_FF(31)\
                COPY_STRUCT_4_FF(32)\
            } \
            return EOK; \
        }/*END switch*/

#define SC_ALIGNED_TAIL_COPY \
    char* pcDest = (char*)dest; \
    switch(count) \
    { \
    SECURE_MEMSET_CONST(32,pcDest,c)\
    SECURE_MEMSET_CONST(31,pcDest,c)\
    SECURE_MEMSET_CONST(30,pcDest,c)\
    SECURE_MEMSET_CONST(29,pcDest,c)\
    SECURE_MEMSET_CONST(28,pcDest,c)\
    SECURE_MEMSET_CONST(27,pcDest,c)\
    SECURE_MEMSET_CONST(26,pcDest,c)\
    SECURE_MEMSET_CONST(25,pcDest,c)\
    SECURE_MEMSET_CONST(24,pcDest,c)\
    SECURE_MEMSET_CONST(23,pcDest,c)\
    SECURE_MEMSET_CONST(22,pcDest,c)\
    SECURE_MEMSET_CONST(21,pcDest,c)\
    SECURE_MEMSET_CONST(20,pcDest,c)\
    SECURE_MEMSET_CONST(19,pcDest,c)\
    SECURE_MEMSET_CONST(18,pcDest,c)\
    SECURE_MEMSET_CONST(17,pcDest,c)\
    SECURE_MEMSET_CONST(16,pcDest,c)\
    SECURE_MEMSET_CONST(15,pcDest,c)\
    SECURE_MEMSET_CONST(14,pcDest,c)\
    SECURE_MEMSET_CONST(13,pcDest,c)\
    SECURE_MEMSET_CONST(12,pcDest,c)\
    SECURE_MEMSET_CONST(11,pcDest,c)\
    SECURE_MEMSET_CONST(10,pcDest,c)\
    SECURE_MEMSET_CONST(9,pcDest,c)\
    SECURE_MEMSET_CONST(8,pcDest,c)\
    SECURE_MEMSET_CONST(7,pcDest,c)\
    SECURE_MEMSET_CONST(6,pcDest,c)\
    SECURE_MEMSET_CONST(5,pcDest,c)\
    SECURE_MEMSET_CONST(4,pcDest,c)\
    SECURE_MEMSET_CONST(3,pcDest,c)\
    SECURE_MEMSET_CONST(2,pcDest,c)\
    SECURE_MEMSET_CONST(1,pcDest,c)\
    }

#define SECC_RET_MEMSET_ECODE   \
    if (destMax == 0 || destMax > SECUREC_MEM_MAX_LEN ) \
    { \
        SECUREC_ERROR_INVALID_RANGE("memset_s"); \
        return ERANGE; \
    } \
    if (dest == NULL) \
    { \
        SECUREC_ERROR_INVALID_PARAMTER("memset_s"); \
        return EINVAL; \
    } \
    if (count > destMax) \
    { \
        (void)memset(dest, c, destMax);   /*set entire buffer to value c*/ \
        SECUREC_ERROR_INVALID_RANGE("memset_s"); \
        return ERANGE_AND_RESET; \
    } \
    return EOK;

errno_t memset_s(void *dest, size_t destMax, int c, size_t count)
{
	if (LIKELY(count <= destMax  && dest  && destMax <= SECUREC_MEM_MAX_LEN)) {
		if (count > SECURE_MEMSET_THRESHOLD_SIZE) {
#ifdef USE_ASM
			(void)memset_opt(dest, c, count);
#else
			(void)memset(dest, c, count);
#endif
			return EOK;
		} else {
			if (SC_ADDR_ALIGNED(dest)) {
				/*use struct assignment*/
				SC_ALIGNED_COPY
			}

			{
				SC_ALIGNED_TAIL_COPY    /*lint !e616 */
			}
			return EOK;
		}
	} else {
		/* meet some runtime violation, return error code */
		SECC_RET_MEMSET_ECODE
	}
}

#if defined(WITH_PERFORMANCE_ADDONS)

errno_t memset_sOptTc(void *dest, size_t destMax, int c, size_t count)
{
	if (LIKELY(count <= destMax  && dest)) {
		if (count > SECURE_MEMSET_THRESHOLD_SIZE) {
#ifdef USE_ASM
			(void)memset_opt(dest, c, count);
#else
			(void)memset(dest, c, count);
#endif
			return EOK;
		} else {
			if (SC_ADDR_ALIGNED(dest)) {
				/*use struct assignment*/
				SC_ALIGNED_COPY
			}

			{
				SC_ALIGNED_TAIL_COPY    /*lint !e616 */
			}
			return EOK;
		}
	} else {
		/* meet some runtime violation, return error code */
		SECC_RET_MEMSET_ECODE
	}
}
#endif /* WITH_PERFORMANCE_ADDONS */

