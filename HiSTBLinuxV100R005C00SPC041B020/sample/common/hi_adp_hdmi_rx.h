/******************************************************************************

  Copyright (C), 2001-2050, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/01/27
  Description   :
  History       :
  1.Date        : 2014/06/9
    Author      : l00214567
    Modification: Created file

******************************************************************************/
#ifndef __SAMPLE_HDMI_RX_COMMON_H__
#define __SAMPLE_HDMI_RX_COMMON_H__
    

#ifdef HI_HDMI_RX_INSIDE 
#include "hi_unf_hdmirx.h"
#else
#include "hi_drv_hdmirx.h"
#endif


#ifdef HI_HDMI_RX_INSIDE 
HI_S32 HI_ADP_HDMIRX_Init(HI_UNF_HDMIRX_TIMING_INFO_S *pstTiming);
#else
HI_S32 HI_ADP_HDMIRX_Init(HI_HDMIRX_TIMING_INFO_S *pstTiming);
#endif
HI_S32 HI_ADP_HDMIRX_Init_GetViAttr(HI_UNF_VI_ATTR_S *pstViAttr);
HI_VOID HI_ADP_HDMIRX_DeInit(void);

#endif
