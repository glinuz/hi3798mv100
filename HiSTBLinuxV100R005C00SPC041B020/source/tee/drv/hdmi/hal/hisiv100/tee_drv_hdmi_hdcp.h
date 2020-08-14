#ifndef __TEE_DRV_HDMI_HDCP_H__
#define __TEE_DRV_HDMI_HDCP_H__

#include "tee_drv_hdmi_common.h"

HI_S32 Hdcp22ProgramLoad();
HI_S32 Hdcp22ProgramVerify();
HI_BOOL Hdcp22DevIdReady();
HI_BOOL Hdcp22DevIdListReady();
HI_VOID Hdcp_DevIdRead(HDMI_HDCP_DEVID_S *pstDevId);
HI_VOID Hdcp22AuthStop(HI_VOID);
HI_VOID Hdcp_DevIdListRead(HDMI_HDCP_REPEAT_IDLIST_S *pstRepeatDevId);
HI_VOID Hdcp22ClrDevIdListReady();
HI_VOID HdcpMuteSet(HI_BOOL bMute);
HI_VOID HdcpMuteStatusGet(HDMI_HDCP_MUTE_STATUS_S *pstHdcpMuteStatus);
HI_VOID HdcpStrategySet(HI_DRV_HDMI_HDCP_LEVEL_E enHdcpLevel);
HI_VOID HdcpStrategyGet(HI_DRV_HDMI_HDCP_LEVEL_E *penHdcpLevel);

HI_VOID HAL_HDMI_HdcpSecEnable(HI_BOOL bEnable);




#endif

