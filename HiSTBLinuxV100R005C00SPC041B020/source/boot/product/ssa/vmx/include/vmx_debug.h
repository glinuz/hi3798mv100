/******************************************************************************

Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_debug.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __VMX_DEBUG_H__
#define __VMX_DEBUG_H__

#include "hi_debug.h"

#ifdef  HI_ADVCA_FUNCTION_RELEASE
#define HI_DEBUG_CA(fmt...)
#define HI_ERR_CA(fmt...)    
#define HI_INFO_CA(fmt...)   
#else
#define HI_ERR_CA(fmt...)         HI_ERR_PRINT(HI_ID_CA, fmt)
#define HI_INFO_CA(fmt...)        HI_INFO_PRINT(HI_ID_CA, fmt)
#define HI_DEBUG_CA(fmt...)       HI_DEBUG_PRINT(HI_ID_CA, fmt)
#endif

#define CA_CheckPointer(p) \
    do {  \
        if (HI_NULL == p)\
        {\
            HI_ERR_CA("pointer parameter is NULL.\n"); \
            return -1; \
        } \
    } while (0)
    
#define CA_ASSERT(api, ret) \
    do{ \
        int l_ret = api; \
        if (l_ret != HI_SUCCESS) \
        { \
            HI_ERR_CA("run %s failed, ERRNO:%#x.\n", #api, l_ret); \
            return -1;\
        } \
        else\
        {\
        /*printf("sample %s: run %s ok.\n", __FUNCTION__, #api);}*/   \
        }\
        ret = l_ret;\
    }while(0)

#endif/*__VMX_DEBUG_H__*/
