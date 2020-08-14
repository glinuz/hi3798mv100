#ifndef __TEE_DRV_HDMI_HAL_H__
#define __TEE_DRV_HDMI_HAL_H__

#include "tee_drv_hdmi.h"

typedef struct hiTEE_HDMI_HAL_S
{
    HI_VOID (*TEE_HAL_HDMI_HdcpMuteSet)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal, HI_BOOL bMute);
    HI_VOID (*TEE_HAL_HDMI_HdcpMuteGet)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal, HDMI_HDCP_MUTE_STATUS_S *pstHdcpMuteStatus);
    HI_S32  (*TEE_HAL_HDMI_HdcpMcuCodeLoad)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal);
    HI_S32  (*TEE_HAL_HDMI_HdcpMcuCodeVerify)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal);
    HI_VOID (*TEE_HAL_HDMI_HdcpStrategySet)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal, HI_DRV_HDMI_HDCP_LEVEL_E enHdcpLevel);
    HI_VOID (*TEE_HAL_HDMI_HdcpStrategyGet)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal, HI_DRV_HDMI_HDCP_LEVEL_E *penHdcpLevel);
    HI_VOID (*TEE_HAL_HDMI_Hdcp2DevIdRead)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal, HDMI_HDCP_DEVID_S *pstDevId);
    HI_VOID (*TEE_HAL_HDMI_Hdcp2AuthStart)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal, HI_BOOL bStart);
    HI_BOOL (*TEE_HAL_HDMI_Hdcp2DevIdReady)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal);
    HI_VOID (*TEE_HAL_HDMI_Hdcp2ClrDevIdReady)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal);
    HI_BOOL (*TEE_HAL_HDMI_Hdcp2DevIdListReady)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal);
    HI_VOID (*TEE_HAL_HDMI_Hdcp2ClrDevIdListReady)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal);
    HI_VOID (*TEE_HAL_HDMI_Hdcp2DevIdListRead)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal, HDMI_HDCP_REPEAT_IDLIST_S *pstRepeatDevId);
    HI_VOID (*TEE_HAL_HDMI_HdcpStatusGet)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal, HI_DRV_HDMI_HDCP_STATUS_E *penHdcpStatus);

    HI_S32 (*TEE_HAL_HDMI_HdcpFunModeSet)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal,HI_BOOL bHdcp2xFunSel);
    HI_S32 (*TEE_HAL_HDMI_Hdcp14R0Cmp)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal,HI_U8 *pu8Data);
    HI_S32 (*TEE_HAL_HDMI_Hdcp14BksvSet)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal,HI_U8 *pu8Data);
    HI_S32 (*TEE_HAL_HDMI_Hdcp14BksvListSet)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal,HI_U8 *pu8Data,HI_U32 u32Size);
    HI_S32 (*TEE_HAL_HDMI_Hdcp14EncrpytionEanble)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal,HI_BOOL bEnable);
    HI_S32 (*TEE_HAL_HDMI_Hdcp14RepeaterEnable)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal,HI_BOOL bEnable);
    HI_S32 (*TEE_HAL_HDMI_Hdcp14ViCmp)(struct hiTEE_HDMI_HAL_S *pstTeeHdmiHal,HI_U8 *pu8Data);

}TEE_HDMI_HAL_S;

HI_VOID TEE_HAL_HDMI_Open(HI_U32 u32HdmiId, TEE_HDMI_HAL_S* pstHalHandle);
HI_VOID TEE_HAL_HDMI_Close(HI_U32 u32HdmiId, TEE_HDMI_HAL_S* pstHalHandle);


#endif

