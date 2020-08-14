/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: input.inl
* Description:
*           used by secureinput_a.c and secureinput_w.c to include. This file
*           provides a template function for ANSI and UNICODE compiling by
*           different type definition. The functions of securec_input_s or
*           securec_winput_s provides internal implementation for scanf family
*           API, such as sscanf_s, fscanf_s.
* History:
*     1. Date: 2014/4/11
*        Author: LiShunda
*        Modification: add "#ifdef va_copy ... #endif" wraper to variable "arglistBeenCopied", which make the meaning
*                      of this variable more clean and can save a a variable space if va_copy macro NOT exist.
*     2. Date: 2014/4/11
*        Author: LiShunda
*        Modification: the code "if (!suppress)" at Line:1435 missing brace, which can cause logic error. Add "{ }" to
*                      wrap the code.
*     3. Date: 2014/4/22
*        Author: LiShunda
*        Modification: Line851, change "memcpy(pointer, tmpbuf, temp)" to 
*                      memcpy_s(pointer, arrayWidth, tmpbuf, temp), which is more secure.
*     4. Date: 2014/6/10
*        Author: LiShunda
*        Modification: add ANDROID macro to detect locale.h exist,  if locale.h not exist,
*                      change (localeconv()->decimal_point) to be "." by macro DECIMAL_POINT_PTR.
*     5. Date: 2014/6/13
*        Author: LiShunda
*        Modification: remove redundant else which has empty body in line 946, 1212, 1420.
*     6. Date: 2014/6/13
*        Author: LiShunda
*        Modification: add judgement on variable dec_point in function _safecrt_fassign to avoid
*                     warning on NOT used variable.
*     7. Date: 2014/6/13
*        Author: LiShunda
*        Modification: change the value of MB_LEN_MAX from 5 to 16.
********************************************************************************
*/

#ifndef __INPUT_INL__5D13A042_DC3F_4ED9_A8D1_882811274C27
#define __INPUT_INL__5D13A042_DC3F_4ED9_A8D1_882811274C27

#ifndef _INTEGRAL_MAX_BITS
#define _INTEGRAL_MAX_BITS 64
#endif  /* _INTEGRAL_MAX_BITS */

#include <stdio.h>
#if !defined(SECUREC_SYSAPI4VXWORKS)
#include <ctype.h>
#endif
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define SECUREC_BUF_EXT_MUL (2)
#define BUFFERED_BLOK_SIZE 1024

#if defined(ANDROID) || defined(SECUREC_SYSAPI4VXWORKS)
#define DECIMAL_POINT_PTR  "."
#else
#include <locale.h> /*if this file NOT exist, you can remove it*/
#define DECIMAL_POINT_PTR (localeconv()->decimal_point)
#endif

#if defined(SECUREC_VXWORKS_PLATFORM) && !defined(va_copy) && !defined(__va_copy)
#define __va_copy(d,s) {\
                            size_t size_of_d = (size_t)sizeof(d);\
                            size_t size_of_s = (size_t)sizeof(s);\
                            if(size_of_d != size_of_s){\
                                (void)memcpy(d,s, sizeof(va_list));\
                            }else{\
                                (void)memcpy(&d, &s, sizeof(va_list));\
                            }\
                         }
#endif

#if (defined(_MSC_VER)) && (_MSC_VER >= 1400)
#define MASK_MSVC_SECURE_CRT_WARNING __pragma(warning(push)) \
    __pragma(warning(disable:4996))

#define END_MASK_MSVC_SECURE_CRT_WARNING  __pragma(warning(pop))
#else
#define MASK_MSVC_SECURE_CRT_WARNING 
#define END_MASK_MSVC_SECURE_CRT_WARNING
#endif

#define MUL10(x) ((((x) << 2) + (x)) << 1)
#define MULTI_BYTE_MAX_LEN (6)

#if !defined(UNALIGNED)
#if defined(_M_IA64) || defined(_M_AMD64)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif
#endif

#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
#define MAX_64BITS_VALUE                18446744073709551615ULL
#define MAX_64BITS_VALUE_DIV_TEN        1844674407370955161ULL
#define MAX_64BITS_VALUE_CUT_LAST_DIGIT 18446744073709551610ULL
#define MIN_64BITS_NEG_VALUE            9223372036854775808ULL
#define MAX_64BITS_POS_VALUE            9223372036854775807ULL
#define MIN_32BITS_NEG_VALUE            2147483648ULL
#define MAX_32BITS_POS_VALUE            2147483647ULL
#define MAX_32BITS_VALUE                4294967295ULL
#define MAX_32BITS_VALUE_INC            4294967296ULL
#define MAX_32BITS_VALUE_DIV_TEN        429496729ULL
#endif

#define _T(x) (x)
#ifdef _UNICODE
#define _TEOF WEOF

#ifdef MB_LEN_MAX
#undef MB_LEN_MAX
#endif
#define MB_LEN_MAX 16                     /* max. # bytes in multibyte char */
#else                                    /*None unicode*/
#define _TEOF EOF
#endif

#if defined (UNICODE)
#define ALLOC_TABLE 1
#else                                    /* defined (UNICODE) */
#define ALLOC_TABLE 0
#endif                                   /* defined (UNICODE) */

#define LEFT_BRACKET ('[' | ('a' - 'A')) /* 'lowercase' version */

static _TINT  _hextodec(_TCHAR);/*lint !e10*/

#define INC() (++charCount, _inc(stream))
#define UN_INC(chr) (--charCount, _un_inc(chr, stream))
#define EAT_WHITE() _whiteout(&charCount, stream)

static _TINT _inc(SEC_FILE_STREAM*);/*lint !e31 !e10*/
static void _un_inc(_TINT, SEC_FILE_STREAM*);/*lint !e528 !e110 !e10*/
static _TINT _whiteout(int*, SEC_FILE_STREAM*);/*lint !e31 !e10*/

#if defined(_IS_DIGIT)
    #undef _IS_DIGIT
#endif
#if defined(_IS_XDIGIT)
    #undef _IS_XDIGIT
#endif

#if defined(UNICODE) || defined(_UNICODE)
    #define _IS_DIGIT(chr) (!((chr) & 0xff00) && isdigit( ((chr) & 0x00ff) ))
    #define _IS_XDIGIT(chr) (!((chr) & 0xff00) && isxdigit( ((chr) & 0x00ff)))
#else   /* ANSI */
    #define _IS_DIGIT(chr) isdigit((UINT8T)(chr))
    #define _IS_XDIGIT(chr) isxdigit((UINT8T)(chr))
#endif  /* _UNICODE */

/* 1 means long long is same as int64, 
*  0 means long long is same as long
*/
#define LONGLONG_IS_INT64 1

static void _safecrt_fassign(int flag, char* argument, char* number, int dec_point)
{

    char* endptr;
    double d;
#if defined(SECUREC_SUPPORT_STRTOLD)
    long double d2;
#endif

    if (dec_point != '.')
    {
        return; /*don't support multi-language decimal point*/
    }
    if ((NULL != argument) && (NULL != number))
    {
#if defined(SECUREC_SUPPORT_STRTOLD)
        if(flag == 2)
        {
            d2 = strtold(number, &endptr);
            *(long double UNALIGNED*)argument = (long double)d2; /*lint !e826*/
            return;
        }
#endif
        d = strtod(number, &endptr);
        if (flag > 0)
        {
            *(double UNALIGNED*)argument = (double)d; /*lint !e826*/
        }
        else
        {
            *(float UNALIGNED*)argument = (float)d; /*lint !e826*/
        }
    }
}

/******************************************************************************
*  int __check_float_string(size_t,size_t *, _TCHAR**, _TCHAR*, int*)
*
*  Purpose:
*       Check if there is enough space insert one more character in the given
*       block, if not then allocate more memory.
*
*  Return:
*       0 if more memory needed and the reallocation failed.
*
*******************************************************************************
*/

static int __check_float_string(size_t nFloatStrUsed,
                                size_t* pnFloatStrSz,
                                _TCHAR** pFloatStr,/*lint !e40 !e601 */
                                _TCHAR* floatstring,/*lint !e40 !e601 */
                                int* pmalloc_FloatStrFlag)
{
    void* tmpPointer;
   
    if (nFloatStrUsed == (*pnFloatStrSz))
    {
        if ((*pFloatStr) == floatstring)
        {
            size_t oriBufSize = (*pnFloatStrSz) * SECUREC_BUF_EXT_MUL * sizeof(_TCHAR);
            if (NULL == ((*pFloatStr ) = (_TCHAR*)SECC_MALLOC(oriBufSize)))/*lint !e586 */
            {
                return 0;
            }
            (void)memset_s((*pFloatStr), oriBufSize, 0, oriBufSize);
            (*pmalloc_FloatStrFlag) = 1;
            (void)memcpy_s((*pFloatStr), oriBufSize, floatstring, (*pnFloatStrSz) * sizeof(_TCHAR));/*lint !e40 */
            (*pnFloatStrSz) *= SECUREC_BUF_EXT_MUL;
        }
        else
        {
            /* LSD 2014.3.6 fix, replace realloc to malloc to avoid heap injection */
            size_t oriBufSize = (*pnFloatStrSz) * sizeof(_TCHAR);/*lint !e40 */
            size_t nextSize = oriBufSize * SECUREC_BUF_EXT_MUL;
            if (nextSize > SECUREC_MEM_MAX_LEN)/*lint !e40 !e574 */
            {
                return 0;
            }
            tmpPointer = (void*)SECC_MALLOC(nextSize);/*lint !e586 */
            if (tmpPointer == NULL)
            {
                return 0;
            }
            (void)memcpy_s(tmpPointer, nextSize, (*pFloatStr), oriBufSize );
            (void)memset_s((*pFloatStr), oriBufSize, 0 ,  oriBufSize);/*lint !e1055*/
            SECC_FREE( (*pFloatStr) );/*lint !e586 */


            (*pFloatStr) = (_TCHAR*)(tmpPointer);/*lint !e40 !e26 !e10 */
            (*pnFloatStrSz) *= SECUREC_BUF_EXT_MUL;
        }
    }
    return 1;
}

#ifndef _UNICODE
/*LSD only multi-bytes string need isleadbyte() function*/
#if !(defined(_WIN32) || defined(_INC_WCTYPE))
/*#define isleadbyte(c) ( (c) & 0x80 )*/

static int isleadbyte(UINT8T c) 
{   /*lint !e401*/
    return (c & 0x80 );
}

#endif
#endif


#define ASCII       (32)           /* # of bytes needed to hold 256 bits */

#ifndef _CVTBUFSIZE
#define _CVTBUFSIZE (309+40)       /* # of digits in max. dp value + slop */
#endif

#ifndef _UNICODE
#define TABLESIZE    ASCII
#else  /* _UNICODE */
#define TABLESIZE    (ASCII * 256)
#endif  /* _UNICODE */

/*******************************************************************************
* int securec_input_s(stream, format, arglist)

* Entry:
*   SEC_FILE_STREAM *stream - struct pointer which contains needed info to read from
*   char *format - format string controlling the data to read
*   arglist - list of pointer to data items
*
* Exit:
*   returns number of items assigned and fills in data items
********************************************************************************
*/

#ifdef _UNICODE
#define _tfgetc fgetwc
#define _tCharMask 0xffff
#else
#define _tfgetc fgetc
#define _tCharMask 0xff
#endif

/* LSD 2014 1 24 add to protect NULL pointer access */
#define CHECK_INPUT_ADDR(p) if (!p) { paraIsNull = 1; goto error_return; }

#ifdef _UNICODE
void clearwDestBuf (const wchar_t* buffer,const wchar_t* cformat, va_list arglist)
#else
void clearDestBuf(const char* buffer,const char* cformat, va_list arglist)
#endif
{    
    const _TUCHAR* fmt = ( _TUCHAR*)cformat;
    void *pDestBuf = NULL;
    va_list arglistsave;                    /*lint !e530*/ /* backup for arglist value, this variable don't need initialized */
    size_t bufSize = 0;
    int spec = 0;  
    signed char wideChar = 0; 
    char doneFlag = 0;
      
    if (fmt != NULL)/*lint !e944*/
    {
        while (*fmt) /*lint !e944*/
        {
            if (_T('%') == *fmt)
            {
                doneFlag = 0;
                wideChar = 0;

                while (!doneFlag)
                {
                    spec = *(++fmt);
                    /* coverity[returned_null] */
                    if (_IS_DIGIT((_TUCHAR)spec)) /*lint !e48 !e409*/
                    {
                        continue;
                    }
                    else if(_T('h') == spec)
                    {
                        --wideChar;
                        continue;
                    }
                    else if(_T('l') == spec || _T('w') == spec)
                    {
                        ++wideChar; 
                        continue;
                    }
                    ++doneFlag;
                }

                /*if no  l or h flag  */
                if (!wideChar)
                {
                    if ((*fmt == _T('S')) || (*fmt == _T('C')))
#if defined(_UNICODE) && (defined(COMPATIBLE_WIN_FORMAT))
                    {
                        --wideChar;
                    }
                    else
                    { 
                        ++wideChar; 
                    }
#else  /* _UNICODE */
                    { 
                        ++wideChar;
                    }
                    else
                    {
                        --wideChar; 
                    }
#endif  /* _UNICODE */
                }

                spec = *fmt | (_T('a') - _T('A'));

                if (!(spec == _T('c') || spec == _T('s') || spec == LEFT_BRACKET))
                {
                    return;/*first argument is not a string type*/
                }
                
                if ((buffer !=NULL) && (*buffer != _T('\0')) && (spec != _T('s')))
                {
            /*when buffer not empty just clear %s.  
            example sscanf(" \n","%s",str,sizeof(str))
            */
                    return;
                }

                if( LEFT_BRACKET == spec)
                {
#if !(defined(COMPATIBLE_WIN_FORMAT))
                    if (_T('{') == *fmt)
                    {
                        return; /*lint !e801*/
                    }
#endif
                    ++fmt;

                    if (_T('^') == *fmt)
                    {
                        ++fmt;
                    }

                    if (_T(']') == *fmt)
                    {
                        ++fmt;
                    }
                    /*2014 3.14 LSD add _T('\0') != *scanptr*/
                    while ((_T('\0') != *fmt) && (_T(']') != *fmt))
                    {
                        ++fmt;
                    }
                    if (!*fmt)
                    {
                        return;      /* trunc'd format string */ /*lint !e801*/
                    }
                }
                
#if defined(va_copy)
                va_copy(arglistsave, arglist); /*lint !e530*/
#elif defined(__va_copy) /*for vxworks*/
                __va_copy(arglistsave, arglist);
#else
                arglistsave = arglist;
#endif
                pDestBuf = (void*)va_arg(arglistsave, void*); /*lint !e826 !e64 !e10*/
                /* Get the next argument - size of the array in characters */
                bufSize = ((size_t)(va_arg(arglistsave, size_t))) & 0xFFFFFFFFUL; ; /*lint !e826 !e48 !e516 !e530 !e78*/
                
                va_end(arglistsave);

                if(0 == bufSize || bufSize >  SECUREC_STRING_MAX_LEN || !pDestBuf)
                {
                    return; /*lint !e801*/
                }

                *(char*)pDestBuf = '\0';

                if (wideChar > 0  && bufSize >= sizeof(wchar_t))
                {
                    *(wchar_t UNALIGNED*)pDestBuf = L'\0';
                }

                return;/*lint !e801*/
            }
            ++fmt;  /* skip to next char */
        }
    }
    return;
}


#ifdef _UNICODE
int securec_winput_s (SEC_FILE_STREAM* stream, const wchar_t* cformat, va_list arglist)
#else
int securec_input_s (SEC_FILE_STREAM* stream, const char* cformat, va_list arglist)/*lint !e40 !e10*/
#endif

{/*lint !e10*/
    
    unsigned long number = 0;  
    _TCHAR floatstring[_CVTBUFSIZE + 1];
    _TCHAR* pFloatStr = floatstring;
    const _TUCHAR* format = ( _TUCHAR*)cformat;
    
    size_t arrayWidth = 0;
    size_t nFloatStrUsed = 0;
    size_t nFloatStrSz = sizeof(floatstring) / sizeof(floatstring[0]);
    int malloc_FloatStrFlag = 0;

              
#if ALLOC_TABLE
    char* table = NULL;                    
    int malloc_flag = 0;                    /*flag for whether "table" is allocated on the heap */
#else  /* ALLOC_TABLE */
    char AsciiTable[TABLESIZE];
    char* table = AsciiTable;
#endif  /* ALLOC_TABLE */

#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
    int longbits = sizeof(long) << 3;
#endif
#if _INTEGRAL_MAX_BITS >= 64
    UINT64T num64 = 0;                     
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
    int  beyondMax = 0;
    UINT64T num64as = 0;
#endif
#endif                                     
    void* pointer = NULL;                   /* points to receiving data addr  */
    void* start;                           

    _TUCHAR* scanptr;                       /* for building "table" data */
    _TINT ch = 0;
    int charCount = 0;                      /* total number of chars read */
    int comChr = 0;                         /* holds designator type */
#if defined(COMPATIBLE_LINUX_FORMAT)
    int oricomChr = 0;
#endif
    int count = 0;                         

    int started = 0;                        
    int width = 0;                          /* width of field */
    int widthSet = 0;                       /* user has specified width */

    int errNoMem = 0;
    int formatError = 0;
    /* int is64Bits = sizeof(int*) == 8;    LSD add */
    int paraIsNull = 0;

    int longone;                            /* LSD change to  0 = SHORT, 1 = int, > 1  long or L_DOUBLE */
#if _INTEGRAL_MAX_BITS >= 64
    int integer64 = 0;                          /* 1 for 64-bit integer, 0 otherwise */
#endif                                     

    va_list arglistsave;                    /*lint !e530*/ /* backup for arglist value, this variable don't need initialized */

#if defined(va_copy) || defined(__va_copy)
    int arglistBeenCopied = 0;
#endif


#ifndef _UNICODE
    wchar_t wctemp = L'\0';
#endif  /* _UNICODE */

    
    _TCHAR decimal;
    _TUCHAR rngch;
    _TUCHAR last;
    _TUCHAR prevChar;
    _TCHAR tch;
    
    char fl_wchar_arg;                      /* flags wide char/string argument */
    signed char wideChar;                  
    char reject;                           
    char negative;                         
    char suppress;                         
    char match;   
    char doneFlag;                         /* general purpose loop monitor */
        


    count = charCount = match = 0;
    (void)memset(&arglistsave, 0, sizeof(arglistsave));/*lint !e545*/
    
    while (format != NULL && *format) /*lint !e944*/
    {
        /* coverity[returned_null] */
        if (isspace((_TUCHAR)*format))  /*lint !e48 !e409*/
        {

            UN_INC(EAT_WHITE()); /* put first non-space char back */

            do
            {
                tch = (_TCHAR)*(++format);
            }
            /* coverity[returned_null] */
            while (isspace((_TUCHAR)tch));  /*lint !e48 !e409*/

            continue;

        }

        if (_T('%') == *format)
        {
            number = 0;
            prevChar = 0;
            width = widthSet = started = 0;
            arrayWidth = 0;
            errNoMem = 0;
            fl_wchar_arg = 0;
            doneFlag = 0;
            suppress = 0;
            negative = 0;
            reject = 0;
            wideChar = 0;

            longone = 1;

#if _INTEGRAL_MAX_BITS >= 64
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
            beyondMax = 0;
#endif
            integer64 = 0;
            num64 = 0;
#endif  

            while (!doneFlag)
            {
                comChr = *(++format);
                /* coverity[returned_null] */
                if (_IS_DIGIT((_TUCHAR)comChr)) /*lint !e48 !e409*/
                {
                    ++widthSet;
                    width = (int)MUL10(width) + (comChr - _T('0')); /*lint !e701*/
                }
                else
                {
                    switch (comChr)
                    {
                        case _T('F') :
                        case _T('N') :   
                            break;  /* NEAR is default in small model */
                        case _T('h') :
                            /* set longone to 0 */
                            --longone;
                            --wideChar;         /* set wideChar = -1 */
                            break;

#if _INTEGRAL_MAX_BITS >= 64
                        case _T('I'):
                            if ( (*(format + 1) == _T('6')) &&
                                 (*(format + 2) == _T('4')) )
                            {
                                format += 2;
                                ++integer64;
                                break;
                            }
                            else if ( (*(format + 1) == _T('3')) &&
                                      (*(format + 2) == _T('2')) )
                            {
                                format += 2;
                                break;
                            }
                            else if ( (*(format + 1) == _T('d')) ||
                                      (*(format + 1) == _T('i')) ||
                                      (*(format + 1) == _T('o')) ||
                                      (*(format + 1) == _T('x')) ||
                                      (*(format + 1) == _T('X')) )
                            {
                                /*lint -e506*/
                                if (sizeof(void*) == sizeof(INT64T))   /*lint !e774*/
                                {
                                    ++integer64;
                                }
                                break;
                            }
                            /*lint -e506*/
                            if (sizeof(void*) == sizeof(INT64T))  /*lint !e774*/
                            {
                                ++integer64;
                            }
                            goto DEFAULT_LABEL; /*lint !e801*/
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
#ifdef COMPATIBLE_LINUX_FORMAT
                        case _T('j') :
                            longone += 2;
                            ++integer64;
                            break;
                        case _T('t') :
#endif
                        case _T('z') :
                            ++longone;
#ifdef SECUREC_ON_64BITS
                            ++longone;
                            ++integer64;
#endif
                            break;
                        case _T('L') :                            
                        case _T('q') :
                            longone += 2;
#if LONGLONG_IS_INT64
                            ++integer64;
#endif
                            break;

                        case _T('l') :
                            if (*(format + 1) == _T('l'))
                            {
                                ++format;
#if LONGLONG_IS_INT64   /*LSD change from #ifdef to #if*/
                                ++integer64;
                                longone += 2;
                                break;
#else  /* LONGLONG_IS_INT64 */
                                ++longone;
                                /* NOBREAK */
#endif  /* LONGLONG_IS_INT64 */
                            }
                            else
                            {
                                ++longone;
#ifdef SECUREC_ON_64BITS
#if  !(defined(COMPATIBLE_WIN_FORMAT)) /*on window 64 system sizeof long is 32bit */
                                ++integer64;
#endif
#endif
                                /* NOBREAK */
                            }
                            /*lint -fallthrough*/
                        case _T('w') :
                            ++wideChar;         /* set wideChar = 1 */
                            break;

                        case _T('*') :
                            ++suppress;
                            break;

                        default:
                        DEFAULT_LABEL:
                            ++doneFlag;
                            break;
                    }/* end of switch(comChr) ... */
                }
            }

            if (!suppress)
            {
                /* LSD change, for gcc compile arglistsave = arglist; */
#if defined(va_copy)
                va_copy(arglistsave, arglist); /*lint !e530 !e644*/
#elif defined(__va_copy) /*for vxworks*/
                __va_copy(arglistsave, arglist);
#else
                arglistsave = arglist;
#endif
                pointer = va_arg(arglist, void*); /*lint !e826 !e64 !e10*/
                CHECK_INPUT_ADDR(pointer); /*lint !e801*/
            }
            else
            {
                /*"pointer = NULL" is safe, in supress mode we don't use pointer to store data*/
                pointer = NULL;         /* doesn't matter what value we use here - we're only using it as a flag*/
            }

            doneFlag = 0;

            if (!wideChar)
            {
                /* use case if not explicitly specified */
                if ((*format == _T('S')) || (*format == _T('C')))
#if defined(_UNICODE) && (defined(COMPATIBLE_WIN_FORMAT))
                {    --wideChar;}
                else
                { ++wideChar; }
#else  /* _UNICODE */
                {    ++wideChar;}
                else
                { --wideChar; }
#endif  /* _UNICODE */
            }


            comChr = *format | (_T('a') - _T('A'));

            if (_T('n') != comChr)
            {
                if (_T('c') != comChr && LEFT_BRACKET != comChr)
                {
                    ch = EAT_WHITE();
                }
                else
                {
                    ch = INC();
                }
            }

            if (_T('n') != comChr)
            {
                if (_TEOF == ch)
                {
                    goto error_return; /*lint !e801*/
                }
            }

            if (!widthSet || width)
            {
                if (!suppress && (comChr == _T('c') || comChr == _T('s') || comChr == LEFT_BRACKET))
                {

#if defined(va_copy)
                    va_copy(arglist, arglistsave);
                    va_end(arglistsave);
                    arglistBeenCopied = 1;
#elif defined(__va_copy) /*for vxworks*/
                    __va_copy(arglist, arglistsave);
                    va_end(arglistsave);
                    arglistBeenCopied = 1;
#else
                    arglist = arglistsave;  
#endif
                    pointer = va_arg(arglist, void*); /*lint !e826 !e64 !e10*/
                    CHECK_INPUT_ADDR(pointer);    /*lint !e801*/


#if defined(va_copy)
                    va_copy(arglistsave, arglist);
#elif defined(__va_copy) /*for vxworks*/
                    __va_copy(arglistsave, arglist);
#else
                    arglistsave = arglist;
#endif
                    /* Get the next argument - size of the array in characters */
#ifdef SECUREC_ON_64BITS
                    arrayWidth = ((size_t)(va_arg(arglist, size_t))) & 0xFFFFFFFFUL; 
#else  /* !SECUREC_ON_64BITS */
                    arrayWidth = va_arg(arglist, size_t); /*lint !e826 !e48 !e516 !e530 !e78*/
#endif

                    if (arrayWidth < 1)
                    {

                        if (wideChar > 0)
                        {
                            *(wchar_t UNALIGNED*)pointer = L'\0';
                        }
                        else
                        {
                            *(char*)pointer = '\0';
                        }

                        goto error_return; /*lint !e801*/
                    }

                    /*LSD add string maxi width protection*/
                    if (wideChar > 0 )
                    {
                        if (arrayWidth > SECUREC_WCHAR_STRING_MAX_LEN)
                        {
                            goto error_return; /*lint !e801*/
                        }
                    }
                    else
                    {
                        /*for char* buffer*/
                        if ( arrayWidth > SECUREC_STRING_MAX_LEN)
                        {
                            goto error_return; /*lint !e801*/
                        }
                    }

                }
#if defined(COMPATIBLE_LINUX_FORMAT)
                oricomChr = comChr;
#endif
                switch (comChr)
                {

                    case _T('c'):
                        /*  case _T('C'):  */
                        if (!widthSet)
                        {
                            ++widthSet;
                            ++width;
                        }
                        if (wideChar > 0)
                        {
                            fl_wchar_arg++;
                        }
                        goto scanit; /*lint !e801*/


                    case _T('s'):
                        /*  case _T('S'):  */
                        if (wideChar > 0)
                        {
                            fl_wchar_arg++;
                        }
                        goto scanit; /*lint !e801*/


                    case LEFT_BRACKET :   /* scanset */
#if !(defined(COMPATIBLE_WIN_FORMAT))
                        if (_T('{') == *format)
                        {
                            goto error_return; /*lint !e801*/
                        }
#endif
                        if (wideChar > 0)
                        {
                            fl_wchar_arg++;
                        }
                        scanptr = (_TUCHAR*)(++format);

                        if (_T('^') == *scanptr)
                        {
                            ++scanptr;
                            --reject; /* set reject to 255 */
                        }

                        /* Allocate "table" on first %[] spec */
#if ALLOC_TABLE
                        if (table == NULL)
                        {
                            /*LSD the table will be freed after error_return label of this function */
                            table = (char*)SECC_MALLOC(TABLESIZE);
                            if ( table == NULL)
                            {
                                goto error_return; /*lint !e801*/
                            }
                            malloc_flag = 1;
                        }
#endif  /* ALLOC_TABLE */
                        (void)memset_s(table, TABLESIZE, 0, TABLESIZE);


                        if (LEFT_BRACKET == comChr) /*lint !e774*/
                        {
                            if (_T(']') == *scanptr)
                            {
                                prevChar = _T(']');
                                ++scanptr;

                                table[ _T(']') >> 3] = 1 << (_T(']') & 7);

                            }
                        }

                        /*2014 3.14 LSD add _T('\0') != *scanptr*/
                        while (_T('\0') != *scanptr && _T(']') != *scanptr)
                        {

                            rngch = *scanptr++;

                            if (_T('-') != rngch ||
                                !prevChar ||           /* first char */
                                _T(']') == *scanptr) /* last char */
                            {

                                table[(prevChar = rngch) >> 3] |= 1 << (rngch & 7); /*lint !e734 !e702*/
                            }
                            else
                            {
                                /* handle a-z type set */

                                rngch = *scanptr++; /* get end of range */

                                if (prevChar < rngch)  /* %[a-z] */
                                {
                                    last = rngch;
                                }
                                else
                                {
#if (defined(COMPATIBLE_WIN_FORMAT))
                                    /* %[z-a] */
                                    last = prevChar;
                                    prevChar = rngch;
#else
                                    table[ '-' >> 3] |= 1 << ('-' & 7); /*lint !e734 !e702*/
                                    table[(prevChar = rngch) >> 3] |= 1 << (rngch & 7); /*lint !e734 !e702*/
                                    continue;
#endif
                                }
                                for (rngch = prevChar; rngch <= last; ++rngch)
                                {
                                    table[rngch >> 3] |= 1 << (rngch & 7); /*lint !e734 !e702*/
                                }

                                prevChar = 0;

                            }
                        }


                        if (!*scanptr)
                        {
                            if (arrayWidth >= sizeof(_TCHAR) && pointer) /*LSD add "&& pointer"*/
                            {
                                *(_TCHAR*)pointer = _T('\0');  /*lint !e613*/
                            }
                            goto error_return;      /* trunc'd format string */ /*lint !e801*/
                        }

                        /* scanset completed.  Now read string */

                        if (LEFT_BRACKET == comChr) /*lint !e774*/
                        {
                            format = scanptr;
                        }

                    scanit:
                        start = pointer;

                        UN_INC(ch);

                        /* One element is needed for '\0' for %s & %[ */
                        if (comChr != _T('c'))
                        {
                            --arrayWidth;
                        }
                        while ( !widthSet || width-- )
                        {

                            ch = INC();
                            if (
                                (_TEOF != ch) &&
                                /* char conditions*/
                                ( ( comChr == _T('c')) ||
                                  /* string conditions !isspace()*/
                                  ( ( comChr == _T('s') &&
                                      (!(ch >= _T('\t') && ch <= _T('\r')) &&
                                       ch != _T(' ')))) ||
                                  /* BRACKET conditions*/
                                  ( (comChr == LEFT_BRACKET) && table && /*LSD add && table is NOT NULL condition*/
                                    ((table[(size_t)ch >> 3] ^ reject) & (1 << (ch & 7)))
                                  )
                                )
                            )
                            {
                                if (!suppress)
                                {
                                    if (!arrayWidth)
                                    {
                                        /* We have exhausted the user's buffer */

                                        errNoMem = 1;
                                        break;
                                    }
                                    CHECK_INPUT_ADDR(pointer);  /*lint !e801*/

#ifndef _UNICODE
                                    if (fl_wchar_arg)
                                    {
                                        char temp[MULTI_BYTE_MAX_LEN];
                                        int di = 1;
#if !defined(COMPATIBLE_WIN_FORMAT)
                                        int convRes = 0;
#endif
                                        wctemp = L'?';    /* set default char as ?*/

                                        (void)memset(temp, 0, MULTI_BYTE_MAX_LEN);
                                        temp[0] = (char) ch;

                                        if ( isleadbyte((UINT8T)ch))
                                        {

#if defined(COMPATIBLE_WIN_FORMAT)
                                            temp[1] = (char) INC();

#else
                                            /*in Linux like system, the string is encoded in UTF-8*/
                                            while (di < (int)MB_CUR_MAX)    /*lint !e681*/
                                            {
                                                temp[di++] = (char) INC();
                                                if ( (convRes = mbtowc(&wctemp, temp, (size_t)MB_CUR_MAX)) > 0 ) 
                                                {
                                                    break; /*convert succeed*/
                                                }
                                            }
#endif
                                        }

                                        if (di == 1)
                                        {
                                            /*for windows system*/
                                            if (mbtowc(&wctemp, temp, (size_t)MB_CUR_MAX) <= 0 )    /*no string termination error for Fortify*/
                                            {
                                                wctemp = L'?';
                                            }
                                        }
#if !(defined(COMPATIBLE_WIN_FORMAT))
                                        else /*di > 1*/
                                        {
                                            if (convRes <= 0 )
                                            {
                                                wctemp = L'?';
                                            }
                                        }
#endif
                                        *(wchar_t UNALIGNED*)pointer = wctemp;
                                        /* just copy L'?' if mbtowc fails, errno is set by mbtowc */
                                        pointer = (wchar_t*)pointer + 1;
                                        --arrayWidth;

                                    }
                                    else
#else  /* _UNICODE */
                                    if (fl_wchar_arg)
                                    {
                                        *(wchar_t UNALIGNED*)pointer = (wchar_t)ch;
                                        pointer = (wchar_t*)pointer + 1;
                                        --arrayWidth;
                                    }
                                    else
#endif  /* _UNICODE */
                                    {
#ifndef _UNICODE
                                        *(char*)pointer = (char)ch;
                                        pointer = (char*)pointer + 1;
                                        --arrayWidth;

#else  /* _UNICODE */
                                        int temp = 0;
                                        /* convert wide to multibyte */
                                        if (arrayWidth >= ((size_t)MB_CUR_MAX))
                                        {
MASK_MSVC_SECURE_CRT_WARNING
                                            temp = wctomb((char*)pointer, (wchar_t)ch);
END_MASK_MSVC_SECURE_CRT_WARNING
                                        }
                                        else
                                        {
                                            char tmpbuf[MB_LEN_MAX];
MASK_MSVC_SECURE_CRT_WARNING
                                            temp = wctomb(tmpbuf, (wchar_t)ch);
END_MASK_MSVC_SECURE_CRT_WARNING
                                            if (temp > 0 && ((size_t)temp) > arrayWidth)
                                            {
                                                errNoMem = 1;
                                                break;
                                            }
                                            if (temp > 0)
                                            {
                                                (void)memcpy_s(pointer, arrayWidth, tmpbuf, (size_t)temp); 
                                            }
                                        }
                                        if (temp > 0)
                                        {
                                            /* do nothing if wctomb fails*/
                                            pointer = (char*)pointer + temp;
                                            arrayWidth -= temp; /*lint !e737*/
                                        }
#endif  /* _UNICODE */
                                    }
                                } /* suppress */
                                else
                                {
                                    /* just indicate a match */
                                    
                                    start = (_TCHAR*)start + 1; /*lint !e613 */
                                }
                            }
                            else
                            {
                                UN_INC(ch);
                                break;
                            }
                        }


                        if (errNoMem)
                        {
                            /* In case of error, blank out the input buffer */
                            if (fl_wchar_arg)
                            {
                                if (start )
                                {
                                    *(wchar_t UNALIGNED*)start = 0;
                                }
                            }
                            else
                            {
                                if (start )
                                {
                                    *(char*)start = 0;     /*LSD add if (start )*/
                                }
                            }

                            goto error_return; /*lint !e801*/
                        }

                        if (start != pointer)
                        {
                            if (!suppress)
                            {
                                ++count;
                                CHECK_INPUT_ADDR(pointer);  /*lint !e801*/

                                if ('c' != comChr)
                                {
                                    /* null-terminate strings */
                                    if (fl_wchar_arg)
                                    {
                                        *(wchar_t UNALIGNED*)pointer = L'\0';
                                    }
                                    else
                                    {
                                        *(char*)pointer = '\0';
                                    }
                                }
                            }
                            /*remove blank else*/ /*NULL*/
                        }
                        else
                        {
                            goto error_return; /*lint !e801*/
                        }

                        break;

                    case _T('i') :      /* could be d, o, or x */

                        comChr = _T('d'); /* use as default */
                    /*lint -fallthrough*/
                    case _T('x'):
                        if (_T('-') == ch)
                        {
                            ++negative;

                            goto x_incwidth; /*lint !e801*/

                        }
                        else if (_T('+') == ch)
                        {
                        x_incwidth:
                            if (!--width && widthSet)
                            { ++doneFlag; }
                            else
                            { ch = INC(); }
                        }
                        if (_T('0') == ch)
                        {

                            if (_T('x') == (_TCHAR)(ch = INC()) || _T('X') == (_TCHAR)ch)
                            {
                                ch = INC();
                                if (widthSet)
                                {
                                    width -= 2;
                                    if (width < 1)
                                    { ++doneFlag; }
                                }
                                comChr = _T('x');
                            }
                            else
                            {
                                ++started;
                                if (_T('x') != comChr)
                                {
                                    if (widthSet && !--width)
                                    {
                                        ++doneFlag;
                                    }
                                    comChr = _T('o');
                                }
                                else
                                {
                                    UN_INC(ch);
                                    ch = _T('0');
                                }
                            }
                        }
                        goto getnum; /*lint !e801*/

                        /* NOTREACHED */
                    /*lint -fallthrough*/
                    case _T('p') :
                        /* force %hp to be treated as %p */
                        longone = 1;
#ifdef SECUREC_ON_64BITS
                        /* force %p to be 64 bit in 64bits system */
                        ++integer64;
                        num64 = 0;
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                        beyondMax = 0;
#endif
#endif
                    /*lint -fallthrough*/
                    case _T('o') :
                    case _T('u') :
                    case _T('d') :

                        if (_T('-') == ch)
                        {
                            ++negative;
                            goto d_incwidth;  /*lint !e801*/
                        }
                        else if (_T('+') == ch)
                        {
                        d_incwidth:
                            if (!--width && widthSet)
                            {
                                ++doneFlag;
                            }
                            else
                            {
                                ch = INC();
                            }
                        }

                    getnum:
#if _INTEGRAL_MAX_BITS >= 64
                        if ( integer64 )
                        {

                            while (!doneFlag)
                            {

                                if (_T('x') == comChr || _T('p') == comChr)
                                    /* coverity[returned_null] */
                                    if (_IS_XDIGIT(ch)) /*lint !e48 !e409*/
                                    {
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                                        if((num64 >> 60) > 0)
                                            beyondMax = 1;
                                        num64as = 16;
#endif
                                        num64 <<= 4;
                                        /*coverity[negative_return_fn]*/
                                        /* coverity[negative_returns] */
                                        ch = _hextodec((_TCHAR)ch);
                                    }
                                    else
                                    { ++doneFlag; }
                                /* coverity[returned_null] */
                                else if (_IS_DIGIT(ch)) /*lint !e48 !e409*/

                                    if (_T('o') == comChr)
                                        if (_T('8') > ch)
                                        { 
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                                            if((num64 >> 61) > 0)
                                                beyondMax = 1;
                                            num64as = 8;
#endif
                                            num64 <<= 3; 
                                        }
                                        else
                                        {
                                            ++doneFlag;
                                        }
                                    else /* _T('d') == comChr */
                                    {
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                                        if (num64 > MAX_64BITS_VALUE_DIV_TEN)
                                        {
                                            beyondMax = 1;
                                        }
#endif
                                        num64 = MUL10(num64);
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                                        if (num64 == MAX_64BITS_VALUE_CUT_LAST_DIGIT)
                                        {
                                            num64as = MAX_64BITS_VALUE - num64;
                                            if (num64as < (UINT64T)(ch - _T('0'))) /*lint !e574 !e571*/
                                            {
                                                beyondMax = 1;
                                            }
                                        }
#endif
                                    }

                                else
                                {
                                    ++doneFlag;
                                }

                                if (!doneFlag)
                                {
                                    ++started;
                                    num64 += ch - _T('0'); /*lint !e737*/

                                    if (widthSet && !--width)
                                    {
                                        ++doneFlag;
                                    }
                                    else
                                    {
                                        ch = INC();
                                    }
                                }
                                else
                                {
                                    UN_INC(ch);
                                }

                            } /* end of WHILE loop */

                            if (negative)
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                            {
                                if ((_T('d') == oricomChr) || (_T('i') == oricomChr))
                                {
                                    if (num64 > MIN_64BITS_NEG_VALUE)
                                    {
                                        num64 = MIN_64BITS_NEG_VALUE;
                                    }
                                    else
                                    {
                                        num64 = (UINT64T )(-(INT64T)num64);
                                    }
                                    if(beyondMax)
                                    {
                                        num64 = MIN_64BITS_NEG_VALUE;
                                    }
                                }
                                else /* o, u, x, X, p */
                                {
                                    num64 = (UINT64T )(-(INT64T)num64);
                                    if (beyondMax)
                                    {
                                        num64 = MAX_64BITS_VALUE;
                                    }
                                }
                            }
                            else 
                            {
                                    if ((_T('d') == oricomChr) || (_T('i') == oricomChr))
                                    {
                                        if (num64 > MAX_64BITS_POS_VALUE)
                                        {
                                            num64 = MAX_64BITS_POS_VALUE;
                                        }
                                        if (beyondMax)
                                        {
                                            num64 = MAX_64BITS_POS_VALUE;
                                        }
                                    }
                                    else
                                    {
                                        if (beyondMax)
                                        {
                                            num64 = MAX_64BITS_VALUE;
                                        }
                                    }
                            }
#else
                            {
#if defined(__hpux)
                                if (_T('p') != oricomChr)
#endif
                                num64 = (UINT64T )(-(INT64T)num64);
                            }
#endif
    
                        }
                        else
                        {
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                            unsigned long decimalEdge = MAX_32BITS_VALUE_DIV_TEN;
#endif
                            while (!doneFlag)
                            {

                                if (_T('x') == comChr || _T('p') == comChr)
                                {

                                    if (_IS_XDIGIT(ch)) /*lint !e48 !e409*/
                                    {
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                                        if((number >> (longbits - 4)) > 0)
                                        {
                                            beyondMax = 1;
                                        }
#endif
                                        number = (number << 4);
                                        ch = _hextodec((_TCHAR)ch);
                                    }
                                    else
                                    {
                                        ++doneFlag;
                                    }
                                }

                                else if (_IS_DIGIT(ch)) /*lint !e48 !e409*/
                                {

                                    if (_T('o') == comChr)
                                    {
                                        if (_T('8') > ch)
                                        {
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                                            if((number >> (longbits - 3)) > 0)
                                            {
                                                beyondMax = 1;
                                            }
#endif
                                            number = (number << 3);
                                        }
                                        else
                                        {
                                            ++doneFlag;
                                        }
                                    }
                                    else /* _T('d') == comChr */
                                    {
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
#ifdef SECUREC_ON_64BITS
                                        if (longbits == 64)
                                        {
                                            decimalEdge = (unsigned long)MAX_64BITS_VALUE_DIV_TEN;
                                        }
#else
                                        if (longbits == 32)
                                        {
                                            decimalEdge = MAX_32BITS_VALUE_DIV_TEN;
                                        }
#endif
                                        if (number > decimalEdge)
                                        {
                                            beyondMax = 1;
                                        }
#endif
                                        number = MUL10(number);
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                                        if (number == MUL10(decimalEdge))
                                        {
                                            num64as = (unsigned long)MAX_64BITS_VALUE - number;
                                            if (num64as < (UINT64T)(ch - _T('0'))) /*lint !e574 !e571*/
                                            {
                                                beyondMax = 1;
                                            }
                                        }
#endif
                                    }
                                }

                                else
                                {
                                    ++doneFlag;
                                }

                                if (!doneFlag)
                                {
                                    ++started;
                                    number += ch - _T('0'); /*lint !e737*/

                                    if (widthSet && !--width)
                                    { ++doneFlag; }
                                    else
                                    { ch = INC(); }
                                }
                                else
                                { UN_INC(ch); }

                            } /* end of WHILE loop */

                            if (negative)
#if (defined(COMPATIBLE_LINUX_FORMAT) && !(defined(__UNIX)))
                            {
                                if(1 == longone)
                                {
                                    if ((_T('d') == oricomChr) || (_T('i') == oricomChr))
                                    {
#ifdef SECUREC_ON_64BITS
                                        if (longbits == 64)
                                        {
                                            if ((number > MIN_64BITS_NEG_VALUE))
                                            {
                                                number = 0;
                                            }
                                            else
                                            {
                                                number = (unsigned int)(-(int)number);
                                            }
                                        }
#else
                                        if (longbits == 32)
                                        {
                                            if ((number > MIN_32BITS_NEG_VALUE))
                                            {
                                                number = MIN_32BITS_NEG_VALUE;
                                            }
                                            else
                                            {
                                                number = (unsigned int)(-(int)number);
                                            }
                                        }
#endif
                                        if (beyondMax)
                                        {
#ifdef SECUREC_ON_64BITS
                                            if (64 == longbits)
                                            {
                                                number = 0;
                                            }
#else
                                            if (32 == longbits)
                                            {
                                                number = MIN_32BITS_NEG_VALUE;
                                            }
#endif
                                        }
                                    }
                                    else /* o, u, x, X ,p */
                                    {
#ifdef SECUREC_ON_64BITS
                                        if (number > MAX_32BITS_VALUE_INC)
                                        {
                                            number = MAX_32BITS_VALUE;
                                        }
                                        else
#endif
                                        {
                                            number = (unsigned int)(-(int)number);
                                        }
                                        if (beyondMax) 
                                        {
                                            number |= (unsigned long)0xffffffffffffffffULL;
                                        }
                                    }
                                }
                                else
                                {
                                    if ((_T('d') == oricomChr) || (_T('i') == oricomChr))
                                    {
                                        if (number > (unsigned long)(1ULL << (longbits - 1)))
                                        {
                                            number = (unsigned long)(1ULL << (longbits - 1));
                                        }
                                        else
                                        {
                                            number = (unsigned long)(-(long)number);
                                        }
                                    }
                                    else
                                    {
                                        number = (unsigned long)(-(long)number);
                                        if (beyondMax) 
                                        {
                                            number |= (unsigned long)0xffffffffffffffffULL;
                                        }
                                    }
                                }
                                if ((_T('d') == oricomChr) || (_T('i') == oricomChr))
                                {
                                    if (((beyondMax) && (-1 == longone)) || 
                                       ((beyondMax) && ( 0 == longone)) || 
                                       ((beyondMax) && ( 1 == longone) && (64 == longbits)))
                                    {
                                        number = 0;
                                    }
                                    if ((beyondMax) && (2 == longone))
                                    {
                                        number = ((unsigned long)(1UL << (longbits - 1)));
                                    }
                                }
                                else /* o, u, x, X, p */
                                {
                                    if (beyondMax)
                                    {
                                        number |= (unsigned long)0xffffffffffffffffULL;
                                    }
                                }
                            }
                            else 
                            {
                                if(1 == longone)
                                {
                                    if ((_T('d') == oricomChr) || (_T('i') == oricomChr))
                                    {
#ifdef SECUREC_ON_64BITS
                                        if (longbits == 64)
                                        {
                                            if (number > MAX_64BITS_POS_VALUE)
                                            {
                                                number |= (unsigned long)0xffffffffffffffffULL;
                                            }
                                        }
                                        if ((beyondMax) && (64 == longbits))
                                        {
                                            number |= (unsigned long)0xffffffffffffffffULL;
                                        }
#else
                                        if (longbits == 32)
                                        {
                                            if (number > MAX_32BITS_POS_VALUE)
                                            {
                                                number = MAX_32BITS_POS_VALUE;
                                            }
                                        }
                                        if ((beyondMax) && (32 == longbits))
                                        {
                                            number = MAX_32BITS_POS_VALUE;
                                        }
#endif
                                    }
                                    else /* o,u,x,X,p */
                                    {
                                        if(beyondMax)
                                        {
                                            number = MAX_32BITS_VALUE;
                                        }
                                    }
                                    
                                }
                                else 
                                {
                                    if ((_T('d') == oricomChr) || (_T('i') == oricomChr) )
                                    {
                                        if (number > ((unsigned long)(1UL << (longbits - 1)) - 1))
                                        {
                                            number = ((unsigned long)(1UL << (longbits - 1)) - 1);
                                        }
                                        if(((beyondMax) && (-1 == longone)) || 
                                           ((beyondMax) && ( 0 == longone)) || 
                                           ((beyondMax) && ( 1 == longone) && (64 == longbits)))
                                        {
                                            number |= (unsigned long)0xffffffffffffffffULL;
                                        }
                                        if((beyondMax) && (2 == longone))
                                        {
                                            number = ((unsigned long)(1UL << (longbits - 1)) - 1);
                                        }
                                    }
                                    else
                                    {
                                        if(beyondMax)
                                        {
                                            number |= (unsigned long)0xffffffffffffffffULL;
                                        }
                                    }
                                }
                            }
#else
                            {
#if defined(__hpux)
                                if (_T('p') != oricomChr)
#endif
                                number = (unsigned long)(-(long)number);
                            }
#endif
#if _INTEGRAL_MAX_BITS >= 64
                        }
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
                        if (_T('F') == comChr) /* expected ':' in long pointer */
                        {
                            started = 0;
                        }

                        if (started)
                        {
                            if (!suppress)
                            {

                                ++count;
                            assign_num:
                                CHECK_INPUT_ADDR(pointer);    /*lint !e801*/
#if _INTEGRAL_MAX_BITS >= 64
                                if ( integer64 )
                                {
#if defined(SECUREC_VXWORKS_PLATFORM)
                                    *(INT64T UNALIGNED*)pointer = *(UINT64T*)(&num64); /*lint !e713, take num64 as unsigned number*/
#else
                                    *(INT64T UNALIGNED*)pointer = (UINT64T)num64; /*lint !e713, take num64 as unsigned number*/
#endif
                                }
                                else
#endif  
                                {
                                    if (longone > 1)
                                    {
                                        *(long UNALIGNED*)pointer = (unsigned long)number; /*lint !e713, take number as unsigned number*/
                                    }
                                    else if (longone == 1)
                                    {
                                        *(int UNALIGNED*)pointer = (int)number;
                                    }
                                    else if (longone == 0)
                                    {
                                        *(short UNALIGNED*)pointer = (unsigned short)number; /*lint !e713, take number as unsigned number*/
                                    }
                                    else /* < 0 for hh format modifier*/
                                    {
                                        *(char UNALIGNED*)pointer = (unsigned char)number; /*lint !e713, take number as unsigned number*/
                                    }
                                }
                            }
                            /* remove blank else*/ /*NULL*/
                        }
                        else
                        {
                            goto error_return;   /*lint !e801*/
                        }
                        break;

                    case _T('n') :      /* char count*/
                        number = charCount; /*lint !e732*/
                        if (!suppress)
                        {
                            goto assign_num;   /*lint !e801*/
                        } 
                        break;

                    case _T('e') :
                        /* case _T('E') : */
                    case _T('f') :
                    case _T('g') : /* scan a float */
                        /* case _T('G') : */
                        nFloatStrUsed = 0;

                        if (_T('-') == ch)
                        {
                            pFloatStr[nFloatStrUsed++] = _T('-');
                            goto f_incwidth; /*lint !e801*/

                        }
                        else if (_T('+') == ch)
                        {
                        f_incwidth:
                            --width;
                            ch = INC();
                        }

                        if (!widthSet)              /* must care width */
                        {
                            width = -1;
                        }


                        /* now get integral part */
                        /* coverity[returned_null] */
                        while (_IS_DIGIT(ch) && width--)    /*lint !e48 !e409*/
                        {
                            ++started;
                            pFloatStr[nFloatStrUsed++] = (_TCHAR)ch; /* ch must be '0' - '9'*/
                            if (__check_float_string(nFloatStrUsed,
                                                     &nFloatStrSz,
                                                     &pFloatStr,
                                                     floatstring,
                                                     &malloc_FloatStrFlag
                                                    ) == 0)
                            {
                                goto error_return;  /*lint !e801*/
                            }
                            ch = INC();
                        }

#ifdef _UNICODE
                        /* convert decimal point(.) to wide-char */
                        decimal = L'.';
                        if (mbtowc(&decimal, DECIMAL_POINT_PTR, (size_t)MB_CUR_MAX) <= 0)
                        {
                            decimal = L'.';
                        }
#else  /* _UNICODE */
                        decimal = *DECIMAL_POINT_PTR; /*if locale.h NOT exist, let decimal = '.' */
#endif  /* _UNICODE */

                        /* now check for decimal */
                        if (decimal == (char)ch && width--)
                        {
                            ch = INC();
                            pFloatStr[nFloatStrUsed++] = decimal;
                            if (__check_float_string(nFloatStrUsed,
                                                     &nFloatStrSz,
                                                     &pFloatStr,
                                                     floatstring,
                                                     &malloc_FloatStrFlag
                                                    ) == 0)
                            {
                                goto error_return;  /*lint !e801*/
                            }
                            /* coverity[returned_null] */
                            while (_IS_DIGIT(ch) && width--)    /*lint !e48 !e409*/
                            {
                                ++started;
                                pFloatStr[nFloatStrUsed++] = (_TCHAR)ch;
                                if (__check_float_string(nFloatStrUsed,
                                                         &nFloatStrSz,
                                                         &pFloatStr,
                                                         floatstring,
                                                         &malloc_FloatStrFlag
                                                        ) == 0)
                                {
                                    goto error_return;  /*lint !e801*/
                                }
                                ch = INC();
                            }
                        }

                        /* now check for exponent */

                        if (started && (_T('e') == ch || _T('E') == ch) && width--)
                        {
                            pFloatStr[nFloatStrUsed++] = _T('e');
                            if (__check_float_string(nFloatStrUsed,
                                                     &nFloatStrSz,
                                                     &pFloatStr,
                                                     floatstring,
                                                     &malloc_FloatStrFlag
                                                    ) == 0)
                            {
                                goto error_return;  /*lint !e801*/
                            }

                            if (_T('-') == (ch = INC()))
                            {

                                pFloatStr[nFloatStrUsed++] = _T('-');
                                if (__check_float_string(nFloatStrUsed,
                                                         &nFloatStrSz,
                                                         &pFloatStr,
                                                         floatstring,
                                                         &malloc_FloatStrFlag
                                                        ) == 0)
                                {
                                    goto error_return;  /*lint !e801*/
                                }
                                goto f_incwidth2; /*lint !e801*/

                            }
                            else if (_T('+') == ch)
                            {
                            f_incwidth2:
                                if (!width--)
                                {
                                    ++width;
                                }
                                else
                                {
                                    ch = INC();
                                }
                            }

                            /* coverity[returned_null] */
                            while (_IS_DIGIT(ch) && width--)    /*lint !e48 !e409*/
                            {
                                ++started;
                                pFloatStr[nFloatStrUsed++] = (_TCHAR)ch;
                                if (__check_float_string(nFloatStrUsed,
                                                         &nFloatStrSz,
                                                         &pFloatStr,
                                                         floatstring,
                                                         &malloc_FloatStrFlag
                                                        ) == 0)
                                {
                                    goto error_return; /*lint !e801*/
                                }
                                ch = INC();
                            }

                        }

                        UN_INC(ch);

                        if (started)
                        {
                            if (!suppress)
                            {
                                ++count;
                                pFloatStr[nFloatStrUsed] = _T('\0');
#ifdef _UNICODE
                                {
                                    /* convert float string */
                                    size_t cfslength;
                                    char* cfloatstring;

                                    cfslength = (size_t)(nFloatStrSz + 1) * WCHAR_SIZE;

                                    if ((cfloatstring = (char*)SECC_MALLOC(cfslength)) == NULL)
                                    {
                                        goto error_return;  /*lint !e801*/
                                    }

                                    if (wcstombs (cfloatstring, pFloatStr, cfslength - 1) > 0)
                                    {
                                        /*LSD from wcstombs_s tp wcstombs add >0*/
                                        _safecrt_fassign( longone - 1, (char*)pointer , cfloatstring, (char)decimal);
                                    }
                                    else
                                    {
                                        SECC_FREE(cfloatstring); /*lint !e516*/
                                        goto error_return;  /*lint !e801*/
                                    }
                                    SECC_FREE(cfloatstring); /*lint !e516*/
                                }
#else  /* _UNICODE */
                                _safecrt_fassign( longone - 1, (char*)pointer , pFloatStr, (char)decimal);
#endif  /* _UNICODE */
                            }
                            /* remove blank else */ /*NULL */
                        }
                        else
                        {
                            goto error_return; /*lint !e801*/
                        }

                        break;


                    default:    /* either found '%' or something else */

                        if ((int)*format != (int)ch)
                        {
                            UN_INC(ch);
                            /* error_return ASSERT's if formatError is true */
                            formatError = 1;
                            goto error_return; /*lint !e801*/
                        }
                        else
                        {
                            match--; /* % found, compensate for inc below */
                        }

                        if (!suppress)
                        {
#if defined(va_copy)
                        va_copy(arglist, arglistsave);
                        arglistBeenCopied = 1;
                        va_end(arglistsave);
#elif defined(__va_copy) /*for vxworks*/
                        __va_copy(arglist, arglistsave);
                        arglistBeenCopied = 1;
                        va_end(arglistsave);
#else
                         arglist = arglistsave;
#endif
                        }
                } /* SWITCH */

                match++;        /* matched a format field - set flag */

            } /* WHILE (width) */

            else
            {
                /* zero-width field in format string */
                UN_INC(ch); 
                goto error_return; /*lint !e801*/
            }

            ++format;  /* skip to next char */
        }
        else  /*  ('%' != *format) */
        {

            if ((int)*format++ != (int)(ch = INC()))
            {
                UN_INC(ch);
                goto error_return; /*lint !e801*/
            }
#ifndef _UNICODE
            if (isleadbyte((UINT8T)ch))
            {
                int ch2;
                char temp[MULTI_BYTE_MAX_LEN];

                if ((int)*format++ != (ch2 = INC()))
                {
                    UN_INC(ch2);    /*LSD in console mode, UN_INC twice will cause problem */
                    UN_INC(ch);
                    goto error_return;  /*lint !e801*/
                }
                if (MB_CUR_MAX > 2 && (((UINT8T)ch & 0xE0) == 0xE0) && (((UINT8T)ch2 & 0x80) == 0x80) )
                {
                    /*this char is very likely to be a UTF-8 char*/
                    int ch3 = INC();
                    temp[0] = (char)ch;
                    temp[1] = (char)ch2;
                    temp[2] = (char)ch3;

                    if (mbtowc(&wctemp, temp, (size_t)MB_CUR_MAX) > 0 )
                    {
                        /*succeed*/
                        if ((int)*format++ != (int)ch3)
                        {
                            UN_INC(ch3);
                            goto error_return;  /*lint !e801*/
                        }
                        --charCount;
                    }
                    else
                    {
                        UN_INC(ch3);
                    }
                }
                --charCount; /* only count as one character read */
            }
#endif  /* _UNICODE */
        }

        if ( (_TEOF == ch) && ((*format != _T('%')) || (*(format + 1) != _T('n'))) )
        {
            break;
        }

    }  /*END while (*format) */

error_return:
#if ALLOC_TABLE
    if (malloc_flag == 1)
    {
        SECC_FREE(table);    /*lint !e668 !e516*/
    }
#endif  /* ALLOC_TABLE */

#if  defined(va_copy) || defined(__va_copy)
    if (arglistBeenCopied)
    {
        va_end(arglist);
    }
#endif
    va_end(arglistsave);


    /*LSD 2014.3.6 add, clear the stack data*/
    (void)memset_s(floatstring, (_CVTBUFSIZE + 1) * sizeof(_TCHAR), 0 ,  (_CVTBUFSIZE + 1) * sizeof(_TCHAR) );
    if (malloc_FloatStrFlag == 1)
    {
        /* pFloatStr can be alloced in __check_float_string function, clear and free it*/
        (void)memset_s(pFloatStr, nFloatStrSz * sizeof(_TCHAR), 0 ,  nFloatStrSz * sizeof(_TCHAR) );
        SECC_FREE(pFloatStr); /*lint !e586 !e424 */
    }

    if (stream != NULL && (stream->_flag & FILE_STREAM_FLAG) && stream->_base != NULL)
    {

        if ( (0 == stream->_cnt) && feof(stream->pf))  /*lint !e611*/
        {
            /* file pointer at the end of file, don't need to seek back */
            stream->_base[0] = '\0';
        }
        else
        {
            /*LSD seek to original position, bug fix 2014 1 21*/

            if ( fseek(stream->pf, stream->oriFilePos, SEEK_SET) ) /*lint !e613*/
            {
                /*seek failed, ignore it*/
                stream->oriFilePos = 0;
            }
            else
            {
                if ( stream->fileRealRead > 0  ) /*LSD bug fix. when file reach to EOF, don't seek back*/
                {
#if (defined(COMPATIBLE_WIN_FORMAT))
                    int loops = 0;
                    for( loops = 0; loops < (stream->fileRealRead / BUFFERED_BLOK_SIZE); ++loops )
                    {
                        if (BUFFERED_BLOK_SIZE != fread(stream->_base, 1, BUFFERED_BLOK_SIZE, stream->pf))
                        {
                            break;
                        }
                    }
                    if ( 0 != (stream->fileRealRead % BUFFERED_BLOK_SIZE))
                    {
                        (void)fread(stream->_base, (stream->fileRealRead % BUFFERED_BLOK_SIZE), 1, stream->pf);
                        if (ftell(stream->pf) < stream->oriFilePos + stream->fileRealRead) /*lint !e613*/
                        {
                            (void)fseek(stream->pf, stream->oriFilePos + stream->fileRealRead, SEEK_SET);
                        }
                    }

#else
                    /* in linux like system*/
                    if ( fseek(stream->pf, stream->oriFilePos + stream->fileRealRead, SEEK_SET) ) /*lint !e613*/
                    {
                        /*seek failed, ignore it*/
                        stream->oriFilePos = 0;
                    }
#endif
                }
            }
        }

        SECC_FREE(stream->_base); /*lint !e613 !e586 !e516*/
    }

    if (_TEOF == ch)
    {
        /* If any fields were matched or assigned, return count */
        /* coverity[missing_va_end] */
        return ( (count || match) ? count : EOF);  /*lint !e593 */
    }
    else if (formatError == 1)
    {
        /*Invalid Input Format*/
        /* coverity[missing_va_end] */
        return -2;  /*lint !e593 */
    }
    else if (paraIsNull)
    {
        /* coverity[missing_va_end] */
        return -2;  /*lint !e593 */
    }
    /* coverity[missing_va_end] */
    /* coverity[leaked_storage] */
    return count;  /*lint !e593 */
}

static _TINT  _hextodec ( _TCHAR chr)/*lint !e129 !e10 */
{
    /* coverity[returned_null] */
    return _IS_DIGIT(chr) ? chr : ((chr & ~(_T('a') - _T('A'))) - _T('A')) + 10 + _T('0');/*lint !e40 !e10 !e48 !e409*/
}

#ifdef _UNICODE
#define _gettc_nolock   getWch____
#define _UN_T_INC     _un_w_inc
#else
#define _gettc_nolock   getCh____
#define _UN_T_INC     _un_c_inc
#endif




#ifdef _UNICODE
static _TINT getWch____(SEC_FILE_STREAM* str)/*lint !e1075 !e129 !e10 */
#else
static _TINT getCh____(SEC_FILE_STREAM* str)/*lint !e1075 !e129 !e10 */
#endif
{

    _TINT ch = 0;
    int firstReadOnFile = 0;
    do
    {
        if ((str->_flag & FROM_STDIN_FLAG) > 0 )
        {

            if (str->fUnget)
            {
                ch = (_TINT)str->lastChar;
                str->fUnget = 0;

            }
            else
            {
                ch = _tfgetc(stdin);
                str->lastChar = (unsigned int)ch; /*lint !e732*/

            }
            break;
        }
        if ((str->_flag & FILE_STREAM_FLAG) > 0 && str->_cnt == 0 )
        {
            /*load file to buffer*/
            if (!str->_base )
            {
                str->_base = (char*)SECC_MALLOC(BUFFERED_BLOK_SIZE + 1);/*lint !e586*/
                if (!str->_base )
                {
                    ch = _TEOF;
                    break;
                }
                str->_base[BUFFERED_BLOK_SIZE] = '\0';/* for fortify Warning STRING_NULL*/
            }
            /*LSD add 2014.3.21*/
            if (UNINITIALIZED_FILE_POS == str->oriFilePos)
            {
                str->oriFilePos = ftell(str->pf);    /* save original file read position*/
                firstReadOnFile = 1;
            }
            str->_cnt = (int)fread(str->_base,  1, BUFFERED_BLOK_SIZE, str->pf);
            str->_base[BUFFERED_BLOK_SIZE] = '\0'; /* for fortify  Warning STRING_NULL*/
            if (0 == str->_cnt || str->_cnt > BUFFERED_BLOK_SIZE)
            {
                ch = _TEOF;
                break;
            }
            str->_ptr = str->_base;
            str->_flag |= LOAD_FILE_TO_MEM_FLAG;
            if (firstReadOnFile)
            {
#ifdef _UNICODE
                /*LSD fix 2014.3.24, add (UINT8T) to (str->_base[0])*/
                if ( str->_cnt > 1 \
                    && (((UINT8T)(str->_base[0]) == 0xFFU && (UINT8T)(str->_base[1]) == 0xFEU) \
                           || ((UINT8T)(str->_base[0]) == 0xFEU && (UINT8T)(str->_base[1]) == 0xFFU)))
                {
                    /*it's BOM header, UNICODE little endian*/
                    str->_cnt -= BOM_HEADER_SIZE;
                    if(0 != memmove_s(str->_base,(size_t)BUFFERED_BLOK_SIZE,str->_base + BOM_HEADER_SIZE,(size_t)str->_cnt))
                    {
                        ch = _TEOF;
                        break;
                    }
                    
                    if(str->_cnt % (int)sizeof(_TCHAR))
                    {
                        /*the str->_cnt must be a  multiple of  sizeof(_TCHAR),otherwise this function will return _TEOF when read the last character */
                        int ret = 0;
                        ret = (int)fread(str->_base + str->_cnt,  1, BOM_HEADER_SIZE, str->pf);
                        if (ret > 0 && ret <= BUFFERED_BLOK_SIZE)
                        {
                              str->_cnt += ret;
                        }
                    }
                }

#else
                if ( str->_cnt > 2 && (UINT8T)(str->_base[0]) == 0xEFU && (UINT8T)(str->_base[1]) == 0xBBU && (UINT8T)(str->_base[2]) == 0xBFU)
                {
                    /*it's BOM header,  little endian*/
                    str->_cnt -= UTF8_BOM_HEADER_SIZE;
                    str->_ptr += UTF8_BOM_HEADER_SIZE;
                }
#endif
            }
        }


        if ((str->_flag & MEM_STR_FLAG) > 0 || (str->_flag & LOAD_FILE_TO_MEM_FLAG) > 0  )
        {
            ch = (_TINT)((str->_cnt -= (int)sizeof(_TCHAR)) >= 0 ? (_tCharMask & * ((_TCHAR*)str->_ptr)) : _TEOF); /*lint !e826*/
            str->_ptr += sizeof(_TCHAR);
        }
    }
    while (0); /*lint !e717, use break in do-while to skip some code*/

    if (_TEOF != ch && (str->_flag & FILE_STREAM_FLAG) > 0 && str->_base)
    {
        str->fileRealRead  += (int)sizeof(_TCHAR);  
    }
    return ch;

}

static _TINT  _inc(SEC_FILE_STREAM* fileptr)/*lint !e1075 !e129 !e10 */
{
    return (_gettc_nolock(fileptr));
}


/*FIXIT _un_inc w version*/
#ifndef TRUE
#define TRUE 1
#endif

#ifdef _UNICODE
static void  _un_w_inc(_TINT chr, SEC_FILE_STREAM* str)
#else
static void  _un_c_inc(_TINT chr, SEC_FILE_STREAM* str)/*lint !e1075 !e129 !e10 */
#endif
{
    if (_TEOF != chr)
    {
        if ((str->_flag & FROM_STDIN_FLAG) > 0)
        {
            str->lastChar = (unsigned int)chr;
            str->fUnget = TRUE;
        }
        else if ( (str->_flag & MEM_STR_FLAG) || (str->_flag & LOAD_FILE_TO_MEM_FLAG) > 0)
        {
            if (str->_ptr > str->_base)   /*lint !e946*/
            {
                str->_ptr -= sizeof(_TCHAR);
                str->_cnt += (int)sizeof(_TCHAR);
            }
        }

        if ( (str->_flag & FILE_STREAM_FLAG) > 0 && str->_base)
        {
            str->fileRealRead -= (int)sizeof(_TCHAR);    /*LSD fix, change from -- str->fileRealRead to str->fileRealRead -= sizeof(_TCHAR). 2014.2.21*/
        }

    }
}

static void  _un_inc(_TINT chr, SEC_FILE_STREAM* str)/*lint !e1075 !e129 !e10 */
{
    _UN_T_INC(chr, str);
}

static _TINT  _whiteout(int* counter, SEC_FILE_STREAM* fileptr)/*lint !e1075 !e129 !e601 !e40 !e10 */
{/*lint !e578 */
    _TINT ch;/*lint !e1075 !e522 !e10 */

    do
    {
        ++*counter;
        ch = _inc(fileptr);/*lint !e40 !e10 !e63 !e530 */

        if (ch == _TEOF)/*lint !e40 */
        {
            break;
        }
    }
#ifdef _UNICODE
    while (iswspace((wint_t)ch));
#else
    /* coverity[returned_null] */
    while (isspace((_TUCHAR)ch));/*lint !e40 !e26 !e10 !e48 !e409*/
#endif
    return ch;/*lint !e40 */
}

#endif /*__INPUT_INL__5D13A042_DC3F_4ED9_A8D1_882811274C27*/


