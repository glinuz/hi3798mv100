/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: secureprintoutput_w.c
* Decription: 
*             by defining corresponding macro for UNICODE string and including
*             "output.inl", this file generates real underlying function used by
*             printf family API.
* History:
*     1. Date: 2014/6/8
*         Author: LiShunda
*         Modification: move the functions of __putwc_nolock, write_char_w, write_multi_char_w 
*                       and write_string_w from securecutil.c to this file. In ANSI mode build,
*                       these functions are NOT needed, which can reduce code generation size
*                       and let securecutil.c NOT include <wchar.h>.
*     2. Date: 2014/7/1
*         Author:  LiShunda
*         Modification: remove "extern const UINT8T securec__lookuptable_s[]".
*                     securec__lookuptable_s is a global variable which will cause 
*                     redefinition when multiple secure c library are integrated.
********************************************************************************
*/

#include "securec.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#ifndef WEOF
/*if some platforms don't have wchar.h, dont't include it*/
#if !(defined(SECUREC_VXWORKS_PLATFORM))
#include <wchar.h>  /*lint !e322 !e7*/
#endif

#ifndef WEOF
    #define WEOF ((wchar_t)-1)
#endif
#endif  /* WEOF */

#include "secureprintoutput.h"

/***************************/
#define _XXXUNICODE
#define TCHAR wchar_t
#define _T(x) L ## x
#define write_char write_char_w
#define write_multi_char write_multi_char_w
#define write_string write_string_w
/***************************/

/*extern const UINT8T securec__lookuptable_s[];*/

/*put a wchar to output stream */
/*LSD change "unsigned short" to wchar_t*/

static wchar_t __putwc_nolock (wchar_t _c, SECUREC_XPRINTF_STREAM* _stream)
{
    wchar_t wcRet = 0;
    if (((_stream)->_cnt -= (int)WCHAR_SIZE ) >= 0 )
    {
        *(wchar_t*)(_stream->_ptr)  = (wchar_t)_c; /*lint !e826*/
        _stream->_ptr += sizeof (wchar_t);
        /* remove LSD wcRet =  (unsigned short) (0xffff & (wchar_t)_c); */
        wcRet = _c;
    }
    else
    {
        wcRet = (wchar_t)WEOF;
    }
    return wcRet;
}

static void write_char_w(wchar_t ch, SECUREC_XPRINTF_STREAM* f, int* pnumwritten)
{
    if (__putwc_nolock(ch, f) == (wchar_t)WEOF)
    {
        *pnumwritten = -1;
    }
    else
    {
        ++(*pnumwritten);
    }
}

static void write_multi_char_w(wchar_t ch, int num, SECUREC_XPRINTF_STREAM* f, int* pnumwritten)
{
    while (num-- > 0)
    {
        write_char_w(ch, f, pnumwritten);
        if (*pnumwritten == -1)
        {
            break;
        }
    }
}

static void write_string_w (wchar_t* string, int len, SECUREC_XPRINTF_STREAM* f, int* pnumwritten)
{
    while (len-- > 0)
    {
        write_char_w(*string++, f, pnumwritten);
        if (*pnumwritten == -1)
        {
            /*
            if (errno == EILSEQ)
                write_char(_T('?'), f, pnumwritten);
            else
                break;
            */
            break;
        }
    }
}

#include "output.inl"


