/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: securecutil.h
* History:
*     1. Date: 2014/5/20
*         Author:  LiShunda
*         Modification: remove extern "C" modifier which will cause g++ link error.
********************************************************************************
*/

#ifndef __SECURECUTIL_H__46C86578_F8FF_4E49_8E64_9B175241761F
#define __SECURECUTIL_H__46C86578_F8FF_4E49_8E64_9B175241761F

#include <stdarg.h>

#ifdef CALL_LIBC_COR_API
/*#include <memory.h>  if memory.h don't exist, use "string.h" instead.*/
//#include <string.h>
#endif

#define DIRECT_ASSIGNMENT_THRESHOLD (12UL)

/*struct for performance*/
typedef struct {
	char buf[1];
} MY_STR1;
typedef struct {
	char buf[2];
} MY_STR2;
typedef struct {
	char buf[3];
} MY_STR3;
typedef struct {
	char buf[4];
} MY_STR4;
typedef struct {
	char buf[5];
} MY_STR5;
typedef struct {
	char buf[6];
} MY_STR6;
typedef struct {
	char buf[7];
} MY_STR7;
typedef struct {
	char buf[8];
} MY_STR8;
typedef struct {
	char buf[9];
} MY_STR9;
typedef struct {
	char buf[10];
} MY_STR10;
typedef struct {
	char buf[11];
} MY_STR11;
typedef struct {
	char buf[12];
} MY_STR12;
typedef struct {
	char buf[13];
} MY_STR13;
typedef struct {
	char buf[14];
} MY_STR14;
typedef struct {
	char buf[15];
} MY_STR15;
typedef struct {
	char buf[16];
} MY_STR16;
typedef struct {
	char buf[17];
} MY_STR17;
typedef struct {
	char buf[18];
} MY_STR18;
typedef struct {
	char buf[19];
} MY_STR19;
typedef struct {
	char buf[20];
} MY_STR20;
typedef struct {
	char buf[21];
} MY_STR21;
typedef struct {
	char buf[22];
} MY_STR22;
typedef struct {
	char buf[23];
} MY_STR23;
typedef struct {
	char buf[24];
} MY_STR24;
typedef struct {
	char buf[25];
} MY_STR25;
typedef struct {
	char buf[26];
} MY_STR26;
typedef struct {
	char buf[27];
} MY_STR27;
typedef struct {
	char buf[28];
} MY_STR28;
typedef struct {
	char buf[29];
} MY_STR29;
typedef struct {
	char buf[30];
} MY_STR30;
typedef struct {
	char buf[31];
} MY_STR31;
typedef struct {
	char buf[32];
} MY_STR32;
typedef struct {
	char buf[33];
} MY_STR33;
typedef struct {
	char buf[34];
} MY_STR34;
typedef struct {
	char buf[35];
} MY_STR35;
typedef struct {
	char buf[36];
} MY_STR36;
typedef struct {
	char buf[37];
} MY_STR37;
typedef struct {
	char buf[38];
} MY_STR38;
typedef struct {
	char buf[39];
} MY_STR39;
typedef struct {
	char buf[40];
} MY_STR40;
typedef struct {
	char buf[41];
} MY_STR41;
typedef struct {
	char buf[42];
} MY_STR42;
typedef struct {
	char buf[43];
} MY_STR43;
typedef struct {
	char buf[44];
} MY_STR44;
typedef struct {
	char buf[45];
} MY_STR45;
typedef struct {
	char buf[46];
} MY_STR46;
typedef struct {
	char buf[47];
} MY_STR47;
typedef struct {
	char buf[48];
} MY_STR48;
typedef struct {
	char buf[49];
} MY_STR49;
typedef struct {
	char buf[50];
} MY_STR50;
typedef struct {
	char buf[51];
} MY_STR51;
typedef struct {
	char buf[52];
} MY_STR52;
typedef struct {
	char buf[53];
} MY_STR53;
typedef struct {
	char buf[54];
} MY_STR54;
typedef struct {
	char buf[55];
} MY_STR55;
typedef struct {
	char buf[56];
} MY_STR56;
typedef struct {
	char buf[57];
} MY_STR57;
typedef struct {
	char buf[58];
} MY_STR58;
typedef struct {
	char buf[59];
} MY_STR59;
typedef struct {
	char buf[60];
} MY_STR60;
typedef struct {
	char buf[61];
} MY_STR61;
typedef struct {
	char buf[62];
} MY_STR62;
typedef struct {
	char buf[63];
} MY_STR63;
typedef struct {
	char buf[64];
} MY_STR64;

/*#define USE_ASM*/

#define _CHECK_BUFFER_OVERLAP /*lint !e946*/
#define ERROR_HANDLER_BY_PRINTF

/*
#define ERROR_HANDLER_BY_ASSERT

#define ERROR_HANDLER_BY_FILE_LOG
*/
/* User can change the error handler by modify the following definition,
* such as logging the detail error in file.
*/
#if defined(_DEBUG) || defined(DEBUG)
#if defined(ERROR_HANDLER_BY_ASSERT)
#define SECUREC_ERROR_INVALID_PARAMTER(msg) assert( msg "invalid argument" == NULL)
#define SECUREC_ERROR_INVALID_RANGE(msg)    assert( msg "invalid dest buffer size" == NULL)
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   assert( msg "buffer overlap" == NULL)

#elif defined(ERROR_HANDLER_BY_PRINTF)
#define SECUREC_ERROR_INVALID_PARAMTER(msg) printf( "%s invalid argument\n",msg)
#define SECUREC_ERROR_INVALID_RANGE(msg)    printf( "%s invalid dest buffer size\n", msg)
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   printf( "%s buffer overlap\n",msg)

#else
#define SECUREC_ERROR_INVALID_PARAMTER(msg) ((void)0)
#define SECUREC_ERROR_INVALID_RANGE(msg)    ((void)0)
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   ((void)0)
#endif

#if defined(ERROR_HANDLER_BY_FILE_LOG)
#define SECUREC_ERROR_INVALID_PARAMTER(msg) logSecureCRuntimeError(msg " EINVAL\n")
#define SECUREC_ERROR_INVALID_RANGE(msg)    logSecureCRuntimeError(msg " ERANGE\n")
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   logSecureCRuntimeError(msg " EOVERLAP\n")
#endif
#else
#define SECUREC_ERROR_INVALID_PARAMTER(msg) ((void)0)
#define SECUREC_ERROR_INVALID_RANGE(msg)    ((void)0)
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   ((void)0)
#endif

void util_memmove(void *dst, const void *src, size_t count);
//lint -esym(526, vsnprintf_helper*)
int vsnprintf_helper(char *string, size_t count, const char *format, va_list ap);

#ifdef __cplusplus
extern "C"
{
#endif
void logSecureCRuntimeError(const char *errDetail);
#ifdef __cplusplus
}
#endif  /* __cplusplus */



#endif


