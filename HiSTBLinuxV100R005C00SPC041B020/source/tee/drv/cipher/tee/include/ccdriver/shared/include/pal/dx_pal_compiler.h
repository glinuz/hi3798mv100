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

#ifndef __DX_PAL_COMPILER_H__
#define __DX_PAL_COMPILER_H__

/************************************/
/* Compiler directives wrappers     */
/************************************/

#ifdef __GNUC__

/* associate a symbol with a link section */
#define DX_PAL_COMPILER_SECTION(sectionName)  __attribute__((section(sectionName)))

/* Mark symbol as used, i.e., prevent garbage collector from dropping it */
#define DX_PAL_COMPILER_KEEP_SYMBOL __attribute__((used))

/* Make given data item aligned (alignment in bytes) */
#define DX_PAL_COMPILER_ALIGN(alignement)  __attribute__((aligned(alignement)))

/* Mark function that never returns */
#define DX_PAL_COMPILER_FUNC_NEVER_RETURNS __attribute__((noreturn))

/* Prevent function from being inlined */
#define DX_PAL_COMPILER_FUNC_DONT_INLINE __attribute__((noinline))

/* Given data type may cast (alias) another data type pointer */
/* (this is used for "superclass" struct casting)             */
#define DX_PAL_COMPILER_TYPE_MAY_ALIAS __attribute__((__may_alias__))

/* Get sizeof for a structure type member */
#define DX_PAL_COMPILER_SIZEOF_STRUCT_MEMBER(type_name, member_name) \
	sizeof(((type_name *)0)->member_name)

/* Compile time assertion (we do not have static_assert support) */
#define DX_ASSERT_CONCAT_(a, b) a##b
#define DX_ASSERT_CONCAT(a, b) DX_ASSERT_CONCAT_(a, b)
#define DX_PAL_COMPILER_ASSERT(cond, message) \
	enum { DX_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(cond)) }

#elif defined(__ARM_DSM__)
#define inline 
/* associate a symbol with a link section */
#define DX_PAL_COMPILER_SECTION(sectionName)  __attribute__((section(sectionName)))

/* Mark symbol as used, i.e., prevent garbage collector from dropping it */
#define DX_PAL_COMPILER_KEEP_SYMBOL __attribute__((used))

/* Make given data item aligned (alignment in bytes) */
#define DX_PAL_COMPILER_ALIGN(alignement)  __attribute__((aligned(alignement)))

/* Mark function that never returns */
#define DX_PAL_COMPILER_FUNC_NEVER_RETURNS __attribute__((noreturn))

/* Prevent function from being inlined */
#define DX_PAL_COMPILER_FUNC_DONT_INLINE __attribute__((noinline))

/* Given data type may cast (alias) another data type pointer */
/* (this is used for "superclass" struct casting)             */
#define DX_PAL_COMPILER_TYPE_MAY_ALIAS __attribute__((__may_alias__))

/* Get sizeof for a structure type member */
#define DX_PAL_COMPILER_SIZEOF_STRUCT_MEMBER(type_name, member_name) \
	sizeof(((type_name *)0)->member_name)

/* Compile time assertion (we do not have static_assert support) */
#define DX_ASSERT_CONCAT_(a, b) a##b
#define DX_ASSERT_CONCAT(a, b) DX_ASSERT_CONCAT_(a, b)
#define DX_PAL_COMPILER_ASSERT(cond, message) \
	enum { DX_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(cond)) }
#else
#error Unsupported compiler.
#endif

#endif /*__DX_PAL_COMPILER_H__*/
