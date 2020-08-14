/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: secureprintoutput_a.c
* Decription:
*             by defining corresponding macro for ANSI string and including
*             "output.inl", this file generates real underlying function used by
*             printf family API.
* History:
*     1. Date: 2014/6/13
*         Author:  LiShunda
*         Modification: change "#define TCHAR char" to "typedef char TCHAR"
*     2. Date: 2014/7/1
*         Author:  LiShunda
*         Modification: remove "extern const UINT8T securec__lookuptable_s[]".
*                     securec__lookuptable_s is a global variable which will cause
*                     redefinition when multiple secure c library are integrated.
********************************************************************************
*/
#include "securec.h"
#include <stdarg.h>
//#include <string.h>
#include "secureprintoutput.h"
//#include <stdio.h>

/***************************/
/*remove this def style #define TCHAR char*/
typedef char TCHAR;
#define _T(x) x
#define write_char write_char_a
#define write_multi_char write_multi_char_a
#define write_string write_string_a
/***************************/

/*extern const UINT8T securec__lookuptable_s[];*/

//#include "output.h"


