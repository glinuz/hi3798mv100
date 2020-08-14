/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_hdmirx.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/01/16
  Description   : 
  History       :
  1.Date        : 2014/01/16
    Author      : t00202585
    Modification: Created file
******************************************************************************/
#ifndef __HI_DRV_HDMIRX_H__
#define __HI_DRV_HDMIRX_H__

#include <linux/ioctl.h>
#include "hi_type.h"
#include "hi_module.h"
#include "hi_type.h"
#include "hi_debug.h"
#include "drv_hdmirx_ioctl.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif


#ifdef HI_HDMI_RX_INSIDE 
#define HI_FATAL_HDMIRX(fmt...) HI_FATAL_PRINT(HI_ID_HDMIRX, fmt)
#define HI_ERR_HDMIRX(fmt...)   HI_ERR_PRINT(HI_ID_HDMIRX, fmt)
#define HI_WARN_HDMIRX(fmt...)  HI_WARN_PRINT(HI_ID_HDMIRX, fmt)
#define HI_INFO_HDMIRX(fmt...)  HI_INFO_PRINT(HI_ID_HDMIRX, fmt)
#define HI_DEBUG_HDMIRX(fmt...) HI_DBG_PRINT(HI_ID_HDMIRX, fmt)
#else
#define HI_FATAL_HDMIRX(fmt...) HI_FATAL_PRINT(HI_ID_SIL9293, fmt)
#define HI_ERR_HDMIRX(fmt...) HI_ERR_PRINT(HI_ID_SIL9293, fmt)
#define HI_WARN_HDMIRX(fmt...) HI_WARN_PRINT(HI_ID_SIL9293, fmt)
#define HI_INFO_HDMIRX(fmt...) HI_INFO_PRINT(HI_ID_SIL9293, fmt)
#define HI_DEBUG_HDMIRX(fmt...) HI_DBG_PRINT(HI_ID_SIL9293, fmt)

HI_S32	HI_DRV_SIL9293_Connect(HI_VOID);
HI_VOID HI_DRV_SIL9293_DisConnect(HI_VOID);
HI_S32	HI_DRV_SIL9293_SetGpioPin(HI_HDMIRX_SET_GPIO_PIN_S *pstGpioPin);
HI_S32	HI_DRV_SIL9293_GetStatus(HI_SIG_STATUS_E *pstSigStatus);
HI_S32	HI_DRV_SIL9293_GetTiming(HI_HDMIRX_TIMING_INFO_S *pstTiming);

#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif

