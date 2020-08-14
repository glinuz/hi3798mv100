/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: secureinput_w.c
* Decription: 
*             by defining data type for UNICODE string and including "input.inl",
*             this file generates real underlying function used by scanf family
*             API.
* History:   
*     1. Date:
*         Author:    
*         Modification:
********************************************************************************
*/

#include "securec.h"
#include "secinput.h"
#include <stddef.h>

#if !(defined(SECUREC_VXWORKS_PLATFORM))
#include <wchar.h> /*lint !e322 !e7*/
#include <wctype.h>
#endif

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef WEOF
    #define WEOF ((wchar_t)-1)
#endif
typedef wchar_t _TCHAR;
typedef wchar_t _TUCHAR;

#if defined(SECUREC_VXWORKS_PLATFORM) && !defined(__WINT_TYPE__)
typedef wchar_t wint_t;
#endif

typedef wint_t _TINT;

#include "input.h"


