/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_hdcp22.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/03/02
  Description   :
  History       :
  Date          : 2016/03/02
  Author        : l00232354
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_HDCP22_H__
#define __HDMI_HAL_HDCP22_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "drv_hdmi_common.h"
#include "hdmi_hal_ddc.h"

/******************************************************************************/
/* Defined                                                                    */
/******************************************************************************/
#define HDCP22_RECVID_SIZE      (5)
#define HDCP22_DEVICE_MAX_NUM   (31)
#define HDCP22_DEPTH_MAX_SIZE   (4)

/******************************************************************************/
/* Union define                                                               */
/******************************************************************************/
typedef union
{
    struct
    {
        HI_U8    bAkeInitDone     :1; // [0]
        HI_U8    bSendCertDone    :1; // [1]
        HI_U8    bKmDone          :1; // [2]
        HI_U8    bSendHDone       :1; // [3]
        HI_U8    bPairingDone     :1; // [4]
        HI_U8    bLcInitDone      :1; // [5]
        HI_U8    bSendLDone       :1; // [6]
        HI_U8    bSendSksDone     :1; // [7]
    } bits;
    HI_U8    u8;

}HDCP22_AUTH_STATE_RECV_U;


typedef union
{
    struct
    {
        HI_U8    bIdListDone      :1; // [0]
        HI_U8    bSendAckDone     :1; // [1]
        HI_U8    bStrmManageDone  :1; // [2]
        HI_U8    bStrmRdyDone     :1; // [3]
        HI_U8    bReserved        :4; // [4~7]
    } bits;
    HI_U8    u8;
}HDCP22_AUTH_STATE_RPT_U;


typedef union
{
    struct
    {
        HI_U8    bRxstatusDone       :1; // [0]
        HI_U8    bCalcCertSha256Done :1; // [1]
        HI_U8    bCheckSignatureFail :1; // [2]
        HI_U8    bCheckSignatureOk   :1; // [3]
        HI_U8    bKmIsStored         :1; // [4]
        HI_U8    bKmIsNoStored       :1; // [5]
        HI_U8    bCalcEmDone         :1; // [6]
        HI_U8    bCalcEkpubkmDone    :1; // [7]
    } bits;
    HI_U8    u8;
}HDCP22_AUTH_STATE0_U;


typedef union
{
    struct
    {
        HI_U8    bAkeNoStoredKmDone  :1; // [8]
        HI_U8    bAkeStoredKmDone    :1; // [9]
        HI_U8    bCalcKdDone         :1; // [10]
        HI_U8    bCalcHDone          :1; // [11]
        HI_U8    bCmpHDone           :1; // [12]
        HI_U8    bLcInitCntExc       :1; // [13]
        HI_U8    bCalcLDone          :1; // [14]
        HI_U8    bCmpLDone           :1; // [15]
    } bits;
    HI_U8    u8;

}HDCP22_AUTH_STATE1_U;


typedef union
{
    struct
    {
        HI_U8    bCalcDkey2Done      :1; // [16]
        HI_U8    bCalcEdkeyKsDone    :1; // [17]
        HI_U8    bCalcVDone          :1; // [18]
        HI_U8    bCmpVDone           :1; // [19]
        HI_U8    bCalcSha256KdDone   :1; // [20]
        HI_U8    bCalcMDone          :1; // [21]
        HI_U8    bCmpMDone           :1; // [22]
        HI_U8    bEnableEncTopDone   :1; // [23]
    } bits;
    HI_U8    u8;

}HDCP22_AUTH_STATE2_U;


typedef union
{
    struct
    {
        HI_U8    bAuthState24     :1; // [24]
        HI_U8    bAuthState25     :1; // [25]
        HI_U8    bAuthState26     :1; // [26]
        HI_U8    bAuthState27     :1; // [27]
        HI_U8    bAuthState28     :1; // [28]
        HI_U8    bAuthState29     :1; // [29]
        HI_U8    bAuthState30     :1; // [30]
        HI_U8    bAuthState31     :1; // [31]
    } bits;
    HI_U8    u8;
}HDCP22_AUTH_STATE3_U;


typedef union
{
    struct
    {
        HI_U8    bRevidReady     :1; // [0]   1 - enable;  0 - disable 
        HI_U8    bRevidListReady :1; // [1]   1 - enable;  0 - disable 
        HI_U8    bReauthRequset  :1; // [2]   1 - enable;  0 - disable 
        HI_U8    bAuthFail       :1; // [3]   1 - enable;  0 - disable 
        HI_U8    bAuthDone       :1; // [4]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr05      :1; // [5]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr06      :1; // [6]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr07      :1; // [7]   1 - enable;  0 - disable 
    } bits;
    HI_U8    u8;

}HDCP22_INTR_B0_U;


typedef union
{
    struct
    {
        HI_U8    bMcuIntr00 :1; // [0]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr01 :1; // [1]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr02 :1; // [2]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr03 :1; // [3]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr04 :1; // [4]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr05 :1; // [5]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr06 :1; // [6]   1 - enable;  0 - disable 
        HI_U8    bMcuIntr07 :1; // [7]   1 - enable;  0 - disable 
    } bits;
    HI_U8    u8;

}HDCP22_INTR_U;


typedef union
{
    struct
    {
        HI_U8    bMcuMask00 :1; // [0]   1 - enable;  0 - disable 
        HI_U8    bMcuMask01 :1; // [1]   1 - enable;  0 - disable 
        HI_U8    bMcuMask02 :1; // [2]   1 - enable;  0 - disable 
        HI_U8    bMcuMask03 :1; // [3]   1 - enable;  0 - disable 
        HI_U8    bMcuMask04 :1; // [4]   1 - enable;  0 - disable 
        HI_U8    bMcuMask05 :1; // [5]   1 - enable;  0 - disable 
        HI_U8    bMcuMask06 :1; // [6]   1 - enable;  0 - disable 
        HI_U8    bMcuMask07 :1; // [7]   1 - enable;  0 - disable 
    } bits;
    HI_U8    u8;

}HDCP22_MASK_U;

/******************************************************************************/
/* Enum define                                                                 */
/******************************************************************************/
typedef enum
{
    HDCP22_MACH_AUTH_EN,
    HDCP22_MACH_WAIT_START,
    HDCP22_MACH_CHK_INTR,
    HDCP22_MACH_BUTT,
    
}HDCP22_MACH_E;


/******************************************************************************/
/* Data structure                                                             */
/******************************************************************************/
typedef struct
{
    HI_BOOL          bInit;
    HI_U32           u32MachId;
    HI_VOID         *pvEventData;
    HMDI_CALLBACK    pfnEventCallBack;
    HDMI_DEVICE_ID_E enHdmiId;
}HDCP22_INIT_INFO_S;


typedef struct
{
    HI_U32 u32HwVersion;
    HI_U32 u32SwVersion;
}HDCP22_VERSION_INFO_S;


typedef struct
{
    HI_BOOL bRepeater;
    HI_U8   u8Version;
}HDCP22_RX_CAPS_S;


typedef struct
{
    HI_U8   u8SeqNumV0;
    HI_U8   u8SeqNumV1;
    HI_U8   u8SeqNumV2;
}HDCP22_SEQ_NUM_V_S;


typedef struct
{
    HI_U8   u8SeqNumM0;
    HI_U8   u8SeqNumM1;
    HI_U8   u8SeqNumM2;
    HI_U8   u8K0;
    HI_U8   u8K1;
    HI_U8   u8StreamID;
    HI_U8   u8StreamType;
}HDCP22_STREAM_MANAGE_S;


typedef struct
{
    HI_U8 u8Depth;
    HI_U8 u8DevCnt;
    HI_BOOL bMaxDevsExceeded;
    HI_BOOL bMaxCascadeExceeded;
    HI_BOOL bMaxHdcp20DownStream;
    HI_BOOL bMaxHdcp1xDownStream;
}HDCP22_RX_INFO_S;


typedef struct
{
    HDCP22_AUTH_STATE_RECV_U uStatusRecv;
    HDCP22_AUTH_STATE_RPT_U  uStatusRpt;
    HDCP22_AUTH_STATE0_U     uStatus0;
    HDCP22_AUTH_STATE1_U     uStatus1;
    HDCP22_AUTH_STATE2_U     uStatus2;
    HDCP22_AUTH_STATE3_U     uStatus3;

}HDCP22_AUTH_FOLLOW_S;


typedef struct
{
    HI_U8 u8RecvId[HDCP22_RECVID_SIZE];
    HI_U8 u8RecvIdList[HDCP22_RECVID_SIZE*HDCP22_DEVICE_MAX_NUM];
    HDCP22_VERSION_INFO_S stVerInfo;
    HDCP22_RX_CAPS_S stRxCaps;
    HDCP22_SEQ_NUM_V_S stSeqNumV;
    HDCP22_STREAM_MANAGE_S stStreamManage;
    HDCP22_RX_INFO_S stRxInfo;
    HDCP22_AUTH_FOLLOW_S stAuthFollow;

}HDCP22_AUTH_STATUS_S;


typedef struct
{
    HDCP22_INTR_B0_U uIntrB0;
    HDCP22_INTR_U uIntrB1;
    HDCP22_INTR_U uIntrB2;
    HDCP22_INTR_U uIntrB3;

}HDCP22_INTR_INFO_S;


typedef struct
{
    HDCP22_MASK_U uMaskB0;
    HDCP22_MASK_U uMaskB1;
    HDCP22_MASK_U uMaskB2;
    HDCP22_MASK_U uMaskB3;

}HDCP22_MASK_INFO_S;


typedef struct
{
    HI_BOOL          bEnable;

    HI_BOOL          bMcuLoad;
    HI_U32           u32AuthEnCnt;
    HI_U64           u64MachStartStamp;       /* machine status start time stamp */
    HI_U64           u64ReAuthWaitMs;
    HDCP22_MACH_E    enMachStatus;

}HDCP22_RUN_STATUS_S;


typedef struct
{
    HI_U32              u32MachInterval;
    HI_BOOL             bFailStopMach;        /* default HI_FALSE;when debug,set HI_TRUE to proc status */
    HI_U64              u64StartWaitMs;              
}HDCP22_ATTR_S;


typedef struct
{
    HDCP22_INIT_INFO_S    stInit;
    HDCP22_ATTR_S         stAttr;
    HDCP22_RUN_STATUS_S   stRun;
    HDCP22_AUTH_STATUS_S  stAuth;
    HDCP22_INTR_INFO_S    stIntr;
    HDCP22_MASK_INFO_S    stMask;
}HDCP22_INFO_S;

/******************************************************************************/
/* public interface                                                           */
/******************************************************************************/
HI_S32  HAL_HDMI_Hdcp22Init(HDMI_DEVICE_ID_E enHdmi,HDMI_HAL_INIT_S *pstHalInit);

HI_VOID  HAL_HDMI_Hdcp22DeInit(HDMI_DEVICE_ID_E enHdmi);

HI_VOID  HAL_HDMI_Hdcp22Reset(HDMI_DEVICE_ID_E enHdmi);

HI_S32  HAL_HDMI_Hdcp22McuCodeLoad(HDMI_DEVICE_ID_E enHdmi);

HI_S32  HAL_HDMI_Hdcp22AuthStatusGet(HDMI_DEVICE_ID_E enHdmi, HDCP22_AUTH_STATUS_S *pstStauts);

HI_S32  HAL_HDMI_Hdcp22RunStatusGet(HDMI_DEVICE_ID_E enHdmi, HDCP22_RUN_STATUS_S *pstRunStat);

HI_S32  HAL_HDMI_Hdcp22AttrSet(HDMI_DEVICE_ID_E enHdmi, HDCP22_ATTR_S *pstAttr);

HI_S32  HAL_HDMI_Hdcp22AttrGet(HDMI_DEVICE_ID_E enHdmi, HDCP22_ATTR_S *pstAttr);

HI_S32  HAL_HDMI_Hdcp22MachEnable(HDMI_DEVICE_ID_E enHdmi, HI_BOOL bEnable);

HI_S32  HAL_HDMI_Hdcp22Support(HDMI_DEVICE_ID_E enHdmiId, HI_BOOL *pbSupport);


//HI_S32 HAL_HDMI_Hdcp22Load8051(HDMI_DEVICE_ID_E enHdmiId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_HDCP22_H__ */



