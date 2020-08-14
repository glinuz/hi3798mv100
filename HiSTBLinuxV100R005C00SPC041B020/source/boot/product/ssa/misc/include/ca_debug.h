/******************************************************************************

Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_debug.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __CA_DEBUG_H__
#define __CA_DEBUG_H__


#ifdef HI_ADVCA_FUNCTION_RELEASE
#define HI_DEBUG_CA(format, arg...)
#else
#define HI_DEBUG_CA(format, arg...)     printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#endif

#ifdef  HI_ADVCA_FUNCTION_RELEASE
#define HI_ERR_CA(format, arg...)    
#define HI_INFO_CA(format, arg...)   
#define HI_SIMPLEINFO_CA(format, arg...)   
#else
#define HI_ERR_CA(format, arg...)       printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)   
#define HI_INFO_CA(format, arg...)      printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg) 
#define HI_SIMPLEINFO_CA(format, arg...)   printf(format, ## arg)
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
        ret = api; \
        if (ret != HI_SUCCESS) \
        { \
            HI_ERR_CA("run %s failed, ERRNO:%#x.\n", #api, ret); \
            return ret;\
        } \
        else\
        {\
        /*printf("sample %s: run %s ok.\n", __FUNCTION__, #api);}*/   \
        }\
    }while(0)

#endif/*__CA_DEBUG_H__*/
