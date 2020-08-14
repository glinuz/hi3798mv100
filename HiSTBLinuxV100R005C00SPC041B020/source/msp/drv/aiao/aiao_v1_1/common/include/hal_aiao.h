/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hal_aiao.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2013/03/09
Last Modified :
Description   : aiao
Function List :
History       :
* main\1    2012-03-09  zgjie     init.
******************************************************************************/
#ifndef __HI_HAL_AIAO_H__
#define __HI_HAL_AIAO_H__

#include "hi_type.h"
#include "hal_aiao_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#if      defined(CHIP_TYPE_hi3796cv100)   \
      || defined(CHIP_TYPE_hi3798cv100)

#define PERIOND_NUM 6  /* 4/16 */
#elif    defined (CHIP_TYPE_hi3798mv100)  \
      || defined (CHIP_TYPE_hi3796mv100)  \
      || defined(CHIP_TYPE_hi3798cv200_a) \
      || defined(CHIP_TYPE_hi3798cv200_b) \
      || defined(CHIP_TYPE_hi3798cv200)   \
      || defined(CHIP_TYPE_hi3798mv200)   \
      || defined(CHIP_TYPE_hi3798mv200_a) \
      || defined(CHIP_TYPE_hi3716mv410)   \
      || defined(CHIP_TYPE_hi3716mv420)
#define PERIOND_NUM 4
#else
#define PERIOND_NUM 2
#endif



/***************************** Macro Definition ******************************/
/* global function */
HI_S32                  HAL_AIAO_Suspend(HI_VOID);
HI_S32                  HAL_AIAO_Resume(HI_VOID);
HI_S32                  HAL_AIAO_PowerOn(HI_VOID);
HI_VOID                 HAL_AIAO_PowerOff(HI_VOID);
HI_S32                  HAL_AIAO_RequestIsr(HI_VOID);
HI_VOID                 HAL_AIAO_FreeIsr(HI_VOID);
HI_S32                  HAL_AIAO_Init(HI_VOID);
HI_VOID                 HAL_AIAO_DeInit(HI_VOID);
HI_VOID                 HAL_AIAO_GetHwCapability(HI_U32* pu32Capability);
HI_VOID                 HAL_AIAO_GetHwVersion(HI_U32* pu32Version);
HI_VOID                 HAL_AIAO_DBG_RWReg(AIAO_Dbg_Reg_S* pstReg);
HI_VOID                 HAL_AIAO_SetTopInt(HI_U32 u32Multibit);
HI_U32                  HAL_AIAO_GetTopIntRawStatus(HI_VOID);
HI_U32                  HAL_AIAO_GetTopIntStatus(HI_VOID);

/* port function */
HI_S32                  HAL_AIAO_P_Open(AIAO_PORT_ID_E enPortID, const AIAO_PORT_USER_CFG_S* pstConfig);

HI_VOID                 HAL_AIAO_P_Close(AIAO_PORT_ID_E enPortID);
HI_S32                  HAL_AIAO_P_Start(AIAO_PORT_ID_E enPortID);
HI_S32                  HAL_AIAO_P_Stop(AIAO_PORT_ID_E enPortID, AIAO_PORT_STOPMODE_E enStopMode);
HI_S32                  HAL_AIAO_P_Mute(AIAO_PORT_ID_E enPortID, HI_BOOL bMute);
HI_S32                  HAL_AIAO_P_SetSampleRate(AIAO_PORT_ID_E enPortID, HI_UNF_SAMPLE_RATE_E enSampleRate);
HI_S32                  HAL_AIAO_P_SetVolume(AIAO_PORT_ID_E enPortID, HI_U32 u32VolumedB);
HI_S32                  HAL_AIAO_P_SetSpdifCategoryCode(AIAO_PORT_ID_E enPortID, AIAO_SPDIF_CATEGORYCODE_E eCategoryCode);
HI_S32                  HAL_AIAO_P_SetSpdifSCMSMode(AIAO_PORT_ID_E enPortID, AIAO_SPDIF_SCMS_MODE_E eSCMSMode);
HI_S32                  HAL_AIAO_P_SetTrackMode(AIAO_PORT_ID_E enPortID, AIAO_TRACK_MODE_E enTrackMode);
HI_S32                  HAL_AIAO_P_SetBypass(AIAO_PORT_ID_E enPortID, HI_BOOL bByBass);
HI_S32                  HAL_AIAO_P_SetMode(AIAO_PORT_ID_E enPortID, HI_BOOL bMode);
HI_S32                  HAL_AIAO_P_GetUserCongfig(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pstUserConfig);
HI_S32                  HAL_AIAO_P_GetStatus(AIAO_PORT_ID_E enPortID, AIAO_PORT_STAUTS_S* pstProcInfo);
HI_S32                  HAL_AIAO_P_SelectSpdifSource(AIAO_PORT_ID_E enPortID, AIAO_SPDIFPORT_SOURCE_E eSrcChnId);
HI_S32                  HAL_AIAO_P_SetSpdifOutPort(AIAO_PORT_ID_E enPortID, HI_S32 bEn);
HI_S32                  HAL_AIAO_P_SetI2SSdSelect(AIAO_PORT_ID_E enPortID, AIAO_I2SDataSel_S*  pstSdSel);
HI_S32                  HAL_AIAO_P_SetAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_ATTR_S* pstAttr);
HI_S32                  HAL_AIAO_P_GetAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_ATTR_S* pstAttr);

HI_VOID                 HAL_AIAO_P_GetTxI2SDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr);
HI_VOID                 HAL_AIAO_P_SetTxI2SDfAttr(AIAO_PORT_ID_E enPortID, AIAO_IsrFunc*      pIsrFunc);//i2s only card set proc func HI_ALSA_I2S_ONLY_SUPPORT
HI_VOID                 HAL_AIAO_P_GetHdmiHbrDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr);
HI_VOID                 HAL_AIAO_P_GetHdmiI2SDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr);
HI_VOID                 HAL_AIAO_P_GetTxSpdDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr);
HI_VOID                 HAL_AIAO_P_GetRxAdcDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr);
HI_VOID                 HAL_AIAO_P_GetRxSifDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr);
HI_VOID                 HAL_AIAO_P_GetRxHdmiDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S* pAttr);

HI_VOID                 HAL_AIAO_P_SetSPDIFPortEn(AIAO_PORT_ID_E enPortID, HI_S32 bEn);

/* port function for verification*/
HI_S32                  HAL_AIAO_P_Open_Veri(AIAO_PORT_ID_E enPortID, const AIAO_PORT_USER_CFG_S* pstConfig);
HI_VOID                 HAL_AIAO_P_Close_Veri(AIAO_PORT_ID_E enPortID);

/*
    todo : alsa + unf isr
    AIAO_IsrFunc alsa_unf_isr()
    {
        ... call alsa_isr_func
        ... HAL_AIAO_P_ProcStatistics
    }
*/
#ifdef HI_ALSA_AI_SUPPORT
HI_U32                  HAL_AIAO_P_ALSA_QueryReadPos(AIAO_PORT_ID_E enPortID);
HI_U32                  HAL_AIAO_P_ALSA_QueryWritePos(AIAO_PORT_ID_E enPortID);
HI_U32                  HAL_AIAO_P_ALSA_FLASH(AIAO_PORT_ID_E enPortID);
HI_U32                  HAL_AIAO_P_ALSA_UpdateRptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize);
HI_U32                  HAL_AIAO_P_ALSA_UpdateWptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize);
#endif
HI_VOID HAL_AIAO_P_ProcStatistics(AIAO_PORT_ID_E enPortID, HI_U32 u32IntStatus, void* pst);

/* port buffer function */
HI_U32                  HAL_AIAO_P_ReadData_NotUpRptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize, HI_U32* pu32Rptr, HI_U32* pu32Wptr);
HI_U32                  HAL_AIAO_P_ReadData(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize);
HI_U32                  HAL_AIAO_P_WriteData(AIAO_PORT_ID_E enPortID, HI_U8* pu32Src, HI_U32 u3SrcLen);
HI_U32                  HAL_AIAO_P_PrepareData(AIAO_PORT_ID_E enPortID, HI_U8* pu32Src, HI_U32 u3SrcLen);
HI_U32                  HAL_AIAO_P_QueryBufData_ProvideRptr(AIAO_PORT_ID_E enPortID, HI_U32* pu32Rptr);
HI_U32                  HAL_AIAO_P_QueryBufData(AIAO_PORT_ID_E enPortID);
HI_U32                  HAL_AIAO_P_QueryBufFree(AIAO_PORT_ID_E enPortID);
HI_U32                  HAL_AIAO_P_UpdateRptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Dest, HI_U32 u32DestSize);
HI_U32                  HAL_AIAO_P_UpdateWptr(AIAO_PORT_ID_E enPortID, HI_U8* pu32Src, HI_U32 u3SrcLen);
HI_VOID                 HAL_AIAO_P_GetDelayMs(AIAO_PORT_ID_E enPortID, HI_U32* pu32Delayms);  //todo
HI_S32                  HAL_AIAO_P_GetRbfAttr(AIAO_PORT_ID_E enPortID, AIAO_RBUF_ATTR_S* pstRbfAttr);
HI_S32                  HAL_AIAO_P_SetOpType(AIAO_PORT_ID_E enPortID, AIAO_OP_TYPE_E enOpType);


#if defined (HI_I2S0_SUPPORT) || defined (HI_I2S1_SUPPORT)
HI_VOID                 HAL_AIAO_P_GetBoardTxI2SDfAttr(HI_U32 u32BoardI2sNum, HI_UNF_I2S_ATTR_S*  pstI2sAttr, AIAO_PORT_ID_E* penPortID, AIAO_PORT_USER_CFG_S* pAttr);
HI_VOID                 HAL_AIAO_P_GetBoardRxI2SDfAttr(HI_U32 u32BoardI2sNum, AIAO_PORT_ID_E* penPortID, AIAO_PORT_USER_CFG_S* pAttr);
HI_S32                  HAL_AIAO_P_CheckBoardI2SOpenAttr(AIAO_PORT_ID_E enPortID, const AIAO_IfAttr_S* pstNewIfAttr);
HI_VOID                 HAL_AIAO_P_SetBoardI2SOpenAttr(AIAO_PORT_ID_E enPortID, const AIAO_IfAttr_S* pstNewIfAttr);
HI_VOID                 HAL_AIAO_P_DestroyBoardI2SOpenAttr(AIAO_PORT_ID_E enPortID);
#endif


#ifdef HI_AIAO_TIMER_SUPPORT
HI_S32                  HAL_AIAO_T_Create(AIAO_TIMER_ID_E enTimerID, const AIAO_Timer_Create_S* pstParam);
HI_VOID                 HAL_AIAO_T_Destroy(AIAO_TIMER_ID_E enTimerID);
HI_S32                  HAL_AIAO_T_SetTimerAttr(AIAO_TIMER_ID_E enTimerID, const AIAO_TIMER_Attr_S* pstAttrParam);
HI_S32                  HAL_AIAO_T_SetTimerEnable(AIAO_TIMER_ID_E enTimerID, HI_BOOL bEnalbe);
HI_S32                  HAL_AIAO_T_GetStatus(AIAO_TIMER_ID_E enTimerID, AIAO_TIMER_Status_S* Param);
HI_VOID                 HAL_AIAO_T_TIMERProcess(AIAO_TIMER_ID_E enTimerID, HI_VOID* pst);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // __HI_HAL_AIAO_H__
