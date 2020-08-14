
/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_ncts.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_NCTS_H__
#define __HDMI_HAL_NCTS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "hi_type.h"

typedef struct{
    HI_U32      u32AudioSmpRate;
    HI_U32      u32TmdsClk;
    HI_U32      u32NValue;
    HI_U32      u32CtsValue;
}HDMI_AUDIO_CTS_N_S;


HI_U32 HAL_HDMI_NValueGet(HI_U32 u32SampleRate,HI_U32 u32TmdsClk);

HI_U32 HAL_HDMI_CtsValueGet(HI_U32 u32SampleRate,HI_U32 u32TmdsClk);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_NCTS_H__ */   


