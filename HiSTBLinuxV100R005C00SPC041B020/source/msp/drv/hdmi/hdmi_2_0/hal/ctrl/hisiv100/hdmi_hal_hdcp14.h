/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_hdcp14.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_HDCP14_H__
#define __HDMI_HAL_HDCP14_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "drv_hdmi_common.h"
#include "hdmi_hal_ddc.h"


#define HDCP14_KSV_SIZE_5BYTES          5
#define HDCP14_AN_SIZE_8BYTES           8
#define HDCP14_Km_SIZE_7BYTES           7
#define HDCP14_M0_SIZE_8BYTES           8
#define HDCP14_Ri_SIZE_2BYTES           2
#define HDCP14_Bstatus_SIZE_2BYTES      2
#define HDCP14_Vi_SIZE_20BYTES          20

#define HDCP14_MAX_PART_I_CNT           3
#define HDCP14_MAX_PART_II_CNT          3
#define HDCP14_MAX_PART_III_CNT         1
#define HDCP14_MAX_PART_III_RECORD_CNT  5
#define HDCP14_MAX_RI_ERR               3

#define HDCP14_MAX_AUTHEN_NUM           3





typedef enum
{
    HDCP14_PROCESS_UNDO,
    HDCP14_PROCESS_OK,
    HDCP14_PROCESS_FAIL,
    
}HDCP14_PROCESS_E;


/* Bcaps ,Rd,OFFSET 0x40,see <HDCP1.4> table 2-2 */
typedef union
{
    struct {
        HI_U8 bzFastReAuthentication    : 1 ;
        HI_U8 bzFeatures1p1             : 1 ;
        HI_U8 bzRsvd                    : 2 ;
        HI_U8 bzFast                    : 1 ;
        HI_U8 bzKsvFifoReady            : 1 ;
        HI_U8 bzRepeater                : 1 ;
        HI_U8 bzHdmiRsvd                : 1 ;
    }u8;

    HI_U8 u8Byte;
}HDCP14_BCAPS_U;


/* Bstatus ,Rd,OFFSET 0x41,see <HDCP1.4> table 2-4 */
typedef union
{
    struct{
        HI_U16 bzDeviceCnt              : 7 ;
        HI_U16 bzMaxDevsExceeded        : 1 ;
        HI_U16 bzDepth                  : 3 ;
        HI_U16 bzMaxCasCadeExceeded     : 1 ;
        HI_U16 bzHdmiMode               : 1 ;
        HI_U16 bzHdmiRsvd               : 1 ;
        HI_U16 bzRsvd                   : 2 ;
    }u16;

    HI_U16      u16Word;
    
}HDCP14_BSTATUS_U;

typedef struct
{
    HI_BOOL     bHdcp14Mode;
    HI_BOOL     bEncryption;
    HI_BOOL     bAvmute;
    HI_BOOL     bRepeaterEnalbe;
    HI_BOOL     bShaEnable;
    
}HDCP14_REAL_HW_S;

typedef struct
{
    HDCP14_PROCESS_E    enLoadKey;
    HDCP14_PROCESS_E    enCrcCheck;

    HDCP14_PROCESS_E    enBksvRead;
    HDCP14_PROCESS_E    enBCapsRead;
    HDCP14_PROCESS_E    enAnGenerateSent;
    HDCP14_PROCESS_E    enAksvSent;
    HDCP14_PROCESS_E    enAR0BR0Cmp;
    HDCP14_PROCESS_E    enEncrypEnable;

    /* A device */
    HI_U8               au8Aksv[HDCP14_KSV_SIZE_5BYTES];
    HI_U8               au8An[HDCP14_AN_SIZE_8BYTES];
    //HI_U8               u8AKm[HDCP14_Km_SIZE_7BYTES];
    //HI_U8               au8AM0[HDCP14_M0_SIZE_8BYTES];
    HI_U8               au8AR0[HDCP14_Ri_SIZE_2BYTES];

    /* B device */
    HDCP14_BCAPS_U      unBCaps;                /* bRepeater */
    HI_U8               au8Bksv[HDCP14_KSV_SIZE_5BYTES];
    HI_U8               au8BR0[HDCP14_Ri_SIZE_2BYTES];
}HDCP14_PART_I_S;

typedef struct
{
    HDCP14_PROCESS_E    enBlistReady;

    HDCP14_PROCESS_E    enBksvListChk;
    HDCP14_PROCESS_E    enHashDone;
    HDCP14_PROCESS_E    enViReadCmp;

    HDCP14_BCAPS_U      unBcaps;
    HDCP14_BSTATUS_U    unBstatus;
    HI_U8               *pu8BksvListData;
    HI_U32              u32BksvListSize;
    HI_U8               au8AVi[HDCP14_Vi_SIZE_20BYTES];
    HI_U8               au8BVi[HDCP14_Vi_SIZE_20BYTES];
}HDCP14_PART_II_S;

#define HDCP14_MAX_III_RECORD_NUM            5

typedef struct
{
    HI_U64              u64TimeStamp;
    HI_U32              u32EncrypFrameCnt;
    HI_U8               u8ARi[HDCP14_Ri_SIZE_2BYTES];
    HI_U8               u8BRi[HDCP14_Ri_SIZE_2BYTES];
}HDCP14_PART_III_S;


typedef enum{

    HDCP14_MACH_AUTHEN_READY,

    HDCP14_MACH_PI_START,
    HDCP14_MACH_PI_BCAP,
    HDCP14_MACH_PI_AKSV,
    HDCP14_MACH_PI_BKSV_SET,
    HDCP14_MACH_PI_R0_WAIT,
    HDCP14_MACH_PI_DONE,
    HDCP14_MACH_PI_WAIT_ENCRP,
    
    HDCP14_MACH_PII_START,
    HDCP14_MACH_PII_SHA_WAIT,
    HDCP14_MACH_PII_VI_CMP,

    HDCP14_MACH_PIII_START,
    HDCP14_MACH_PIII_RI_MONITOR,
    
}HDCP14_MACH_E;

typedef struct
{
    HI_BOOL             bEnable;

    HI_U32              u32AuthenCnt;
    HI_U64              u64MachStartStamp;       /* machine status start time stamp */
    HI_U64              u64ReAuthWaitMs;
    HI_U64              u64PIIIRiInterval;            /* default | normal is 2000ms;when Ri error, is 50ms */
    HDCP14_MACH_E       enMachStatus;
    HI_U32              u32PartIIIPtr;
    HI_U32              u32RiErrCnt;
    
}HDCP14_RUN_STAT_S;

typedef struct
{
    HI_BOOL             bInit;
    HI_U32              u32MachId;
    HI_VOID             *pvEventData;
    HMDI_CALLBACK       pfnEventCallBack;
    HI_U32              enHdmiId;
}HDCP14_INIT_STAT_S;

typedef struct
{
    
    HI_U32              u32MachInterval;

    //HI_BOOL             bRandomAn; 
    HI_U32              u32R0ReadyTime;         /* if config [0,100) default 100ms;or,set the config value */

    HI_U32              u32BlistReadyTime;      /* if config [0,5000) default 5000ms;or,set the config value */

    HI_BOOL             bFailStopMach;          /* default HI_FALSE;when debug,set HI_TRUE to proc status */

    HI_U64              u64StartWaitMs;
}HDCP14_ATTR_S;


typedef struct
{
    HDCP14_REAL_HW_S    stRealHw;
    HDCP14_PART_I_S     stPartI;
    HDCP14_PART_II_S    stPartII;
    HDCP14_PART_III_S   stPartIII[HDCP14_MAX_III_RECORD_NUM];

}HDCP14_STAUTS_S;

typedef struct
{
    HDCP14_INIT_STAT_S  stInit;
    HDCP14_RUN_STAT_S   stRun;
    HDCP14_ATTR_S       stAttr;
    HDCP14_STAUTS_S     stStatus;
}HDCP14_INFO_S;


HI_S32  HAL_HDMI_Hdcp14Init(HDMI_DEVICE_ID_E enHdmi,HDMI_HAL_INIT_S *pstHalInit);

HI_S32  HAL_HDMI_Hdcp14DeInit(HDMI_DEVICE_ID_E enHdmi);

HI_S32  HAL_HDMI_Hdcp14Reset(HDMI_DEVICE_ID_E enHdmi);

HI_S32  HAL_HDMI_Hdcp14StatusGet(HDMI_DEVICE_ID_E enHdmi,HDCP14_STAUTS_S *pstStauts);

HI_S32  HAL_HDMI_Hdcp14RunStatusGet(HDMI_DEVICE_ID_E enHdmi,HDCP14_RUN_STAT_S *pstRunStat);

HI_S32  HAL_HDMI_Hdcp14AttrSet(HDMI_DEVICE_ID_E enHdmi,HDCP14_ATTR_S *pstAttr);

HI_S32  HAL_HDMI_Hdcp14AttrGet(HDMI_DEVICE_ID_E enHdmi,HDCP14_ATTR_S *pstAttr);

HI_S32  HAL_HDMI_Hdcp14MachEnable(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEnable);

HI_S32  HAL_HDMI_Hdcp14Support(HDMI_DEVICE_ID_E enHdmiId, HI_BOOL *pbSupport);

HI_S32 HAL_HDMI_Hdcp14LoadKey(HDMI_DEVICE_ID_E enHdmiId) ;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_HDCP14_H__ */   



