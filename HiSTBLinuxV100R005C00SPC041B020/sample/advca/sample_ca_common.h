/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_common.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef CA_COMMON_H
#define CA_COMMON_H

#ifdef  CONFIG_SUPPORT_CA_RELEASE
#define HI_DEBUG_ADVCA(format...)
#else
#define HI_DEBUG_ADVCA(format...)    printf(format)
#endif

#endif

