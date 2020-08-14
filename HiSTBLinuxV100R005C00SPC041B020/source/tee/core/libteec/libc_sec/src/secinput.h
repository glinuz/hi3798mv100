/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: secinput.h
* Decription: 
*             define macro, data struct, and declare function prototype,
*             which is used by input.inl, secureinput_a.c and secureinput_w.c.
Note:
*             The macro SECUREC_LOCK_FIL, SECUREC_UNLOCK_FILE, SECUREC_LOCK_STDIN 
*             and SECUREC_UNLOCK_STDIN are NOT implemented, which depends on your dest
*             system. User can add implementation to these macro.
* History:   
*     1. Date:
*         Author:     
*         Modification:
********************************************************************************
*/

#ifndef __SEC_INPUT_H_E950DA2C_902F_4B15_BECD_948E99090D9C
#define __SEC_INPUT_H_E950DA2C_902F_4B15_BECD_948E99090D9C

#include <stdarg.h>
#include <stdio.h>

#define SCANF_EINVAL (-1)
#define NOT_USED_FILE_PTR NULL

/* for internal stream flag */
#define MEM_STR_FLAG 0X01
#define FILE_STREAM_FLAG 0X02
#define FROM_STDIN_FLAG 0X04
#define LOAD_FILE_TO_MEM_FLAG 0X08

#define UNINITIALIZED_FILE_POS (-1)
#define BOM_HEADER_SIZE (2)
#define UTF8_BOM_HEADER_SIZE (3)

typedef struct
{
    int _cnt;                                   /* the size of buffered string in bytes*/
    char* _ptr;                                 /* the pointer to next read position */
    char* _base;                                /*the pointer to the header of buffered string*/
    int _flag;                                  /* mark the properties of input stream*/
    FILE* pf;                                   /*the file pointer*/
    int fileRealRead;
    long oriFilePos;                            /*the original position of file offset when fscanf is called*/
    unsigned int lastChar;                      /*the char code of last input*/
    int fUnget;                                 /*the boolean flag of pushing a char back to read stream*/
} SEC_FILE_STREAM;

#define INIT_SEC_FILE_STREAM {0, NULL, NULL, 0, NULL, 0, 0, 0, 0 }
//lint -esym(526, securec_input_s*)
int  securec_input_s (SEC_FILE_STREAM* stream, const char* format, va_list arglist); /*from const UINT8T* to const char**/
//lint -esym(526, securec_winput_s*)
int  securec_winput_s (SEC_FILE_STREAM* stream, const wchar_t* format, va_list arglist);

//lint -esym(526, clearDestBuf*)
void clearDestBuf(const char* buffer,const char* cformat, va_list arglist);
//lint -esym(526, clearwDestBuf*)
void clearwDestBuf (const wchar_t* buffer,const wchar_t* cformat, va_list arglist);

#define SECUREC_LOCK_FILE(s)
#define SECUREC_UNLOCK_FILE(s)

#define SECUREC_LOCK_STDIN(i,s)
#define SECUREC_UNLOCK_STDIN(i,s)

#define _VALIDATE_STREAM_ANSI_SETRET( stream, errorcode, retval, retexpr)

#endif


