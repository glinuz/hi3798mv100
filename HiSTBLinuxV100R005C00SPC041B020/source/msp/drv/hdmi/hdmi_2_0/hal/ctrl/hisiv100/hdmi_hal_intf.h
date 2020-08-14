
/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_intf.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_INTF_H__
#define __HDMI_HAL_INTF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "drv_hdmi_common.h"
#include "hdmi_hal_ctrl.h"
#include "hdmi_hal_ddc.h"
#include "hdmi_hal_scdc.h"
#include "hdmi_hal_debug.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_compatibility.h"
#include "hdmi_hal_hdcp14.h"
#include "hdmi_hal_hdcp22.h"
#include "hdmi_hal_cec.h"
#endif



typedef struct
{
    HI_BOOL                 bInit;
   
    HDMI_TX_CAPABILITY_S    stTxCapability;
    HDMI_HDCP_MODE_E        enHdcpMode;
    HI_U32                  u32TmdsClk;
    //HDMI_HAL_BASE_PARAM_S   stBaseParm;
    
}HDMI_HAL_CFG_S;


typedef struct 
{
    HMDI_CALLBACK           pfnEventCallback;
    HI_VOID                 *pstEventHandle;
}HDMI_HAL_EVENT_S;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_INTF_H__ */   

