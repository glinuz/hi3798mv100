/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_advca.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#ifndef __HI_DRV_ADVCA_H__
#define __HI_DRV_ADVCA_H__

#include "drv_advca_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_FATAL_CA(fmt...)    HI_FATAL_PRINT(HI_ID_CA, fmt)
#define HI_ERR_CA(fmt...)      HI_ERR_PRINT(HI_ID_CA, fmt)
#define HI_WARN_CA(fmt...)     HI_WARN_PRINT(HI_ID_CA, fmt)
#define HI_INFO_CA(fmt...)     HI_INFO_PRINT(HI_ID_CA, fmt)

HI_S32 HI_DRV_ADVCA_Crypto(DRV_ADVCA_EXTFUNC_PARAM_S stParam);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* End of #ifndef __HI_DRV_ADVCA_H__ */

