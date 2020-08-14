/***************************************************************
*  Copyright 2014 (c) Discretix Technologies Ltd.              *
*  This software is protected by copyright, international      *
*  treaties and various patents. Any copy, reproduction or     *
*  otherwise use of this software must be authorized in a      *
*  license agreement and include this Copyright Notice and any *
*  other notices specified in the license agreement.           *
*  Any redistribution in binary form must be authorized in the *
*  license agreement and include this Copyright Notice and     *
*  any other notices specified in the license agreement and/or *
*  in materials provided with the binary distribution.         *
****************************************************************/

 
#ifndef DX_PAL_TYPES_PLAT_H
#define DX_PAL_TYPES_PLAT_H
/* Host specific types for standard (ISO-C99) compilant platforms */

#include <stdint.h>
#include <stddef.h>

#define DX_NULL NULL
#define INT32_MAX 2147483647
/*
typedef unsigned int            DxUint_t;
typedef uint8_t                 DxUint8_t;
typedef uint16_t                DxUint16_t;
typedef uint32_t                DxUint32_t;
typedef uint64_t                DxUint64_t;

typedef int                   	DxInt_t;
typedef int8_t                  DxInt8_t;
typedef int16_t                 DxInt16_t;
typedef int32_t                 DxInt32_t;
typedef int64_t	                DxInt64_t;

typedef char                    DxChar_t;
typedef short                   DxWideChar_t;
*/
typedef unsigned int                  DxUint_t;
typedef unsigned char                 DxUint8_t;
typedef unsigned short                DxUint16_t;
typedef unsigned int                   DxUint32_t;
typedef unsigned long long            DxUint64_t;

typedef int                 DxInt_t;
typedef char                DxInt8_t;
typedef short               DxInt16_t;
typedef int                  DxInt32_t;
typedef long long	        DxInt64_t;

typedef char                    DxChar_t;
typedef short                   DxWideChar_t;

/*typedef unsigned int                  uint_t;
typedef unsigned char                 uint8_t;
typedef unsigned short               uint16_t;
typedef unsigned int                 uint32_t;
typedef unsigned long long           uint64_t;


typedef signed char                int8_t;
typedef signed short               int16_t;
typedef signed int                int32_t;
typedef long long	        int64_t;
typedef unsigned int     size_t;

*/

typedef enum {
    DX_FALSE = 0,
    DX_TRUE = 1
} EDxBool;
typedef uintptr_t		DxVirtAddr_t;
typedef uint32_t		DxBool_t;
typedef uint32_t		DxStatus;

#define DxError_t   	DxStatus
#define DX_INFINITE		0xFFFFFFFF

#define CEXPORT_C
#define CIMPORT_C

/* Define macros for host to SeP endianess conversion (for host wrappers) */
#define cpu_to_le16(x) x
#define le16_to_cpu(x) x
#define cpu_to_le32(x) x
#define le32_to_cpu(x) x

#endif /*DX_PAL_TYPES_PLAT_H*/
