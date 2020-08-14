/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.  
* File name: secureinput_a.c
* Decription: 
*             by defining data type for ANSI string and including "input.inl",
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

#ifdef _UNICODE
#undef _UNICODE
#endif

#ifdef UNICODE
#undef UNICODE
#endif


typedef char _TCHAR;
typedef unsigned char _TUCHAR;
typedef int _TINT;

#include "input.h"


