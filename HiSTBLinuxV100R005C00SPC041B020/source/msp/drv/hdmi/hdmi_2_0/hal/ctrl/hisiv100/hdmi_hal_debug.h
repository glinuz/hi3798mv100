/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_debug.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_DEBUG_H__
#define __HDMI_HAL_DEBUG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct
{
    HI_U32  u32HdmiId;
    HI_BOOL bCecTx;
}DEBUG_DATA_CEC_S;



HI_VOID HAL_HDMI_DebugMachPrint(HI_VOID *pvData);

HI_VOID HAL_HDMI_DebugDdcPrint(HI_VOID *pvData);

HI_VOID HAL_HDMI_DebugHdcp14Print(HI_VOID *pvData);

HI_VOID HAL_HDMI_DebugCecPrint(HI_VOID *pvData);

HI_VOID HAL_HDMI_DebugScdcPrint(HI_VOID *pvData);





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_DEBUG_H__ */   












