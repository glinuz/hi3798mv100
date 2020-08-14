#ifndef __TYPES_H__
#define __TYPES_H__


typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

typedef unsigned int size_t;
typedef int ptrdiff_t;

#define BITS_PER_LONG 32

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif


typedef s8  INT8;
typedef s16 INT16;
typedef s32 INT32;

typedef u8  UINT8;
typedef u16 UINT16;
typedef u32 UINT32;

typedef int BOOL;
typedef int STATUS;

#define TRUE    1
#define FALSE   0

#define ERROR   -1
#define OK      0

#define FOREVER         while(1)
#define IN
#define OUT

#define PRIVATE static

#endif

