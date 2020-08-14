/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hal_aiao_func.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : aiao
Function List :
History       :
* main\1    2012-09-22   z40717     init.
******************************************************************************/
#ifndef __HI_AIAO_FUNC_H__
#define __HI_AIAO_FUNC_H__

#include "hi_type.h"
#include "hal_aiao_common.h"
#include "hal_aiao_priv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

/* global function */
HI_S32                  iHAL_AIAO_Init(HI_VOID);
HI_VOID                 iHAL_AIAO_DeInit(HI_VOID);
HI_VOID                 iHAL_AIAO_GetHwCapability(HI_U32* pu32Capability);
HI_VOID                 iHAL_AIAO_GetHwVersion(HI_U32* pu32Version);
HI_VOID                 iHAL_AIAO_DBG_RWReg(AIAO_Dbg_Reg_S* pstReg);
HI_VOID                 iHAL_AIAO_SetTopInt(HI_U32 u32Multibit);
HI_U32                  iHAL_AIAO_GetTopIntRawStatus(HI_VOID);
HI_U32                  iHAL_AIAO_GetTopIntStatus(HI_VOID);

/*****************************************************************************
 Description  : AIAO TX/RX Port DSP Control HAL API
*****************************************************************************/
HI_VOID                 iHAL_AIAO_P_SetInt(AIAO_PORT_ID_E enPortID, HI_U32 u32Multibit);
HI_VOID                 iHAL_AIAO_P_ClrInt(AIAO_PORT_ID_E enPortID, HI_U32 u32Multibit);
HI_U32                  iHAL_AIAO_P_GetIntStatusRaw(AIAO_PORT_ID_E enPortID);
HI_U32                  iHAL_AIAO_P_GetIntStatus(AIAO_PORT_ID_E enPortID);

/* global port function */
HI_S32                  iHAL_AIAO_P_Open(const AIAO_PORT_ID_E enPortID,
                                         const AIAO_PORT_USER_CFG_S* pstConfig,
                                         AIAO_PORT_S** ppstPort,
                                         AIAO_IsrFunc** pIsr);

HI_VOID                 iHAL_AIAO_P_Close(AIAO_PORT_S* pstPort);
HI_S32                  iHAL_AIAO_P_Start(AIAO_PORT_S* pstPort);
HI_S32                  iHAL_AIAO_P_Stop(AIAO_PORT_S* pstPort, AIAO_PORT_STOPMODE_E enStopMode);
HI_S32                  iHAL_AIAO_P_Mute(AIAO_PORT_S* pstPort, HI_BOOL bMute);
HI_S32                  iHAL_AIAO_P_MuteFade(AIAO_PORT_S* pstPort, HI_BOOL bMuteFade);
HI_S32                  iHAL_AIAO_P_SetVolume(AIAO_PORT_S* pstPort, HI_U32 u32VolumedB);
HI_S32                  iHAL_AIAO_P_SetSpdifCategoryCode(AIAO_PORT_S* pstPort, AIAO_SPDIF_CATEGORYCODE_E eCategoryCode);
HI_S32                  iHAL_AIAO_P_SetSpdifSCMSMode(AIAO_PORT_S* pstPort, AIAO_SPDIF_SCMS_MODE_E eSCMSMode);
HI_S32                  iHAL_AIAO_P_SetTrackMode(AIAO_PORT_S* pstPort, AIAO_TRACK_MODE_E enTrackMode);
HI_S32                  iHAL_AIAO_P_SetBypass(AIAO_PORT_S* pstPort, HI_BOOL bByBass);
HI_S32                  iHAL_AIAO_P_SetMode(AIAO_PORT_S* pstPort, HI_BOOL bMode);
HI_S32                  iHAL_AIAO_P_GetUserCongfig(AIAO_PORT_S* pstPort, AIAO_PORT_USER_CFG_S* pstUserConfig);
HI_S32                  iHAL_AIAO_P_GetStatus(AIAO_PORT_S* pstPort, AIAO_PORT_STAUTS_S* pstProcInfo);
HI_S32                  iHAL_AIAO_P_SelectSpdifSource(AIAO_PORT_S* pstPort, AIAO_SPDIFPORT_SOURCE_E eSrcChnId);
HI_S32                  iHAL_AIAO_P_SetSpdifOutPort(AIAO_PORT_S* pstPort, HI_S32 bEn);
HI_S32                  iHAL_AIAO_P_SetI2SSdSelect(AIAO_PORT_S* pstPort, AIAO_I2SDataSel_S*  pstSdSel);
HI_S32                  iHAL_AIAO_P_SetAttr(AIAO_PORT_S* pstPort, AIAO_PORT_ATTR_S* pstAttr);
HI_S32                  iHAL_AIAO_P_GetAttr(AIAO_PORT_S* pstPort, AIAO_PORT_ATTR_S* pstAttr);
HI_VOID                 iHAL_AIAO_P_ProcStatistics(AIAO_PORT_S* pstPort, HI_U32 u32IntStatus);
HI_S32                  iHAL_AIAO_P_SetI2SMasterClk(AIAO_PORT_ID_E enPortID, AIAO_IfAttr_S* pstIfAttr);
HI_S32                  iHAL_AIAO_P_SetI2SlaveClk(AIAO_PORT_ID_E enPortID, AIAO_IfAttr_S* pstIfAttr);
HI_VOID                 iHAL_AIAO_P_SetSPDIFPortEn(AIAO_PORT_ID_E enPortID, HI_S32 bEn);


/* port buffer function */
HI_U32                  iHAL_AIAO_P_ReadData_NotUpRptr(AIAO_PORT_S* pstPort, HI_U8* pu32Dest, HI_U32 u32DestSize, HI_U32* pu32Rptr, HI_U32* pu32Wptr);
HI_U32                  iHAL_AIAO_P_ReadData(AIAO_PORT_S* pstPort, HI_U8* pu32Dest, HI_U32 u32DestSize);
HI_U32                  iHAL_AIAO_P_WriteData(AIAO_PORT_S* pstPort, HI_U8* pu32Src, HI_U32 u3SrcLen);
HI_U32                  iHAL_AIAO_P_PrepareData(AIAO_PORT_S* pstPort, HI_U8* pu32Src, HI_U32 u3SrcLen);
HI_U32                  iHAL_AIAO_P_QueryBufData_ProvideRptr(AIAO_PORT_S* pstPort, HI_U32* pu32Rptr);
HI_U32                  iHAL_AIAO_P_QueryBufData(AIAO_PORT_S* pstPort);
HI_U32                  iHAL_AIAO_P_QueryBufFree(AIAO_PORT_S* pstPort);
HI_U32                  iHAL_AIAO_P_UpdateRptr(AIAO_PORT_S* pstPort, HI_U8* pu32Dest, HI_U32 u32DestSize);
HI_U32                  iHAL_AIAO_P_UpdateWptr(AIAO_PORT_S* pstPort, HI_U8* pu32Src, HI_U32 u3SrcLen);
HI_S32                  iHAL_AIAO_P_GetRbfAttr(AIAO_PORT_S* pstPort, AIAO_RBUF_ATTR_S* pstRbfAttr);
HI_VOID                 iHAL_AIAO_P_GetDelayMs(AIAO_PORT_S* pstPort, HI_U32* pu32Delayms);
#ifdef HI_ALSA_AI_SUPPORT
HI_U32                  iHAL_AIAO_P_ALSA_UpdateRptr(AIAO_PORT_S* pstPort, HI_U8* pu32Dest, HI_U32 u32DestSize);
HI_U32                  iHAL_AIAO_P_ALSA_QueryWritePos (AIAO_PORT_S* pstPort);
HI_U32                  iHAL_AIAO_P_ALSA_QueryReadPos (AIAO_PORT_S* pstPort);

HI_U32                  iHAL_AIAO_P_ALSA_UpdateWptr(AIAO_PORT_S* pstPort, HI_U8* pu32Dest, HI_U32 u32DestSize);
HI_U32                  iHAL_AIAO_P_ALSA_FLASH(AIAO_PORT_S* pstPort);
#endif

HI_S32                  iHAL_AIAO_P_SetOpType(AIAO_PORT_ID_E enPortID, AIAO_OP_TYPE_E enOpType);

#ifdef HI_AIAO_TIMER_SUPPORT
HI_S32                  iHAL_AIAO_T_Create(AIAO_TIMER_ID_E enTimerID, const AIAO_Timer_Create_S* pstParam, AIAO_TIMER_S* ppstPort, AIAO_TimerIsrFunc** pIsr);
HI_VOID                 iHAL_AIAO_T_Destroy(AIAO_TIMER_S pstPort);
HI_S32                  iHAL_AIAO_T_SetTimerAttr(AIAO_TIMER_S pstPort, const AIAO_TIMER_Attr_S* pstAttrParam);
HI_S32                  iHAL_AIAO_T_SetTimerEnalbe(AIAO_TIMER_S pstPort, HI_BOOL bEnalbe);
HI_S32                  iHAL_AIAO_T_GetStatus(AIAO_TIMER_S pstPort, AIAO_TIMER_Status_S* pstStatus);
HI_VOID                 iHAL_AIAO_T_TIMERProcess(AIAO_TIMER_S pstPort);
HI_VOID                 iHAL_TIMER_ClearTimer(AIAO_TIMER_ID_E enTimerID);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // __HI_AIAO_FUNC_H__
