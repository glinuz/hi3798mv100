/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_global.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/7/20
  Description   :
  History       :
  Date          : 2013/7/20
  Author        : y00229039
  Modification  :
*******************************************************************************/

#ifndef __DRV_GLOBAL_H__
#define __DRV_GLOBAL_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#include "hi_error_mpi.h"
//#include "mpi_priv_hdmi.h"
#include "hi_drv_hdmi.h"
#include "hi_drv_disp.h"
#include "hi_unf_hdmi.h"
#include "drv_hdmi.h"


#include "drv_hdmi_debug.h"
//#include "test_edid.h"

#define DEF_FILE_NAMELENGTH 32


/*
**HDMI Debug
*/
#ifdef HDMI_DEBUG

#define EDID_INFO(fmt...)           HDMI_DBG_INFO   (HDMI_DEBUG_PRINT_EDID, fmt)
#define EDID_WARN(fmt...)           HDMI_DBG_WARN   (HDMI_DEBUG_PRINT_EDID, fmt)
#define EDID_ERR(fmt...)            HDMI_DBG_ERR    (HDMI_DEBUG_PRINT_EDID, fmt)
#define EDID_FATAL(fmt...)          HDMI_DBG_FATAL  (HDMI_DEBUG_PRINT_EDID, fmt)

#define HDCP_INFO(fmt...)           HDMI_DBG_INFO   (HDMI_DEBUG_PRINT_HDCP, fmt)
#define HDCP_WARN(fmt...)           HDMI_DBG_WARN   (HDMI_DEBUG_PRINT_HDCP, fmt)
#define HDCP_ERR(fmt...)            HDMI_DBG_ERR    (HDMI_DEBUG_PRINT_HDCP, fmt)
#define HDCP_FATAL(fmt...)          HDMI_DBG_FATAL  (HDMI_DEBUG_PRINT_HDCP, fmt)

#define CEC_INFO(fmt...)            HDMI_DBG_INFO   (HDMI_DEBUG_PRINT_CEC, fmt)
#define CEC_WARN(fmt...)            HDMI_DBG_WARN   (HDMI_DEBUG_PRINT_CEC, fmt)
#define CEC_ERR(fmt...)             HDMI_DBG_ERR    (HDMI_DEBUG_PRINT_CEC, fmt)
#define CEC_FATAL(fmt...)           HDMI_DBG_FATAL  (HDMI_DEBUG_PRINT_CEC, fmt)

#define HPD_INFO(fmt...)            HDMI_DBG_INFO   (HDMI_DEBUG_PRINT_HPD, fmt)
#define HPD_WARN(fmt...)            HDMI_DBG_WARN   (HDMI_DEBUG_PRINT_HPD, fmt)
#define HPD_ERR(fmt...)             HDMI_DBG_ERR    (HDMI_DEBUG_PRINT_HPD, fmt)
#define HPD_FATAL(fmt...)           HDMI_DBG_FATAL  (HDMI_DEBUG_PRINT_HPD, fmt)

#define COM_INFO(fmt...)            HDMI_DBG_INFO   (HDMI_DEBUG_PRINT_COM, fmt)
#define COM_WARN(fmt...)            HDMI_DBG_WARN   (HDMI_DEBUG_PRINT_COM, fmt)
#define COM_ERR(fmt...)             HDMI_DBG_ERR    (HDMI_DEBUG_PRINT_COM, fmt)
#define COM_FATAL(fmt...)           HDMI_DBG_FATAL  (HDMI_DEBUG_PRINT_COM, fmt)

#else

#define EDID_INFO(fmt...)           HI_INFO_HDMI    (fmt)
#define EDID_WARN(fmt...)           HI_WARN_HDMI    (fmt)
#define EDID_ERR(fmt...)            HI_ERR_HDMI     (fmt)
#define EDID_FATAL(fmt...)          HI_FATAL_HDMI   (fmt)
                                                         
#define HDCP_INFO(fmt...)           HI_INFO_HDMI    (fmt)
#define HDCP_WARN(fmt...)           HI_WARN_HDMI    (fmt)
#define HDCP_ERR(fmt...)            HI_ERR_HDMI     (fmt)
#define HDCP_FATAL(fmt...)          HI_FATAL_HDMI   (fmt)
                                                         
#define CEC_INFO(fmt...)            HI_INFO_HDMI    (fmt)
#define CEC_WARN(fmt...)            HI_WARN_HDMI    (fmt)
#define CEC_ERR(fmt...)             HI_ERR_HDMI     (fmt)
#define CEC_FATAL(fmt...)           HI_FATAL_HDMI   (fmt)
                                                         
#define HPD_INFO(fmt...)            HI_INFO_HDMI    (fmt)
#define HPD_WARN(fmt...)            HI_WARN_HDMI    (fmt)
#define HPD_ERR(fmt...)             HI_ERR_HDMI     (fmt)
#define HPD_FATAL(fmt...)           HI_FATAL_HDMI   (fmt)
                                                         
#define COM_INFO(fmt...)            HI_INFO_HDMI    (fmt)
#define COM_WARN(fmt...)            HI_WARN_HDMI    (fmt)
#define COM_ERR(fmt...)             HI_ERR_HDMI     (fmt)
#define COM_FATAL(fmt...)           HI_FATAL_HDMI   (fmt)


#endif

//g_stHdmiCommParam

typedef struct
{
    HI_BOOL     bOpenMce2App;        /* smooth change Mce to App mode*/
    HI_BOOL     bOpenedInBoot;             /*boot => mce : we only need set default param to attr(no config) and create thread*/
    struct task_struct  *kThreadTimer;    /*timer thread*//*CNcomment:定时器线程 */
    HI_BOOL     kThreadTimerStop;
    struct task_struct  *kCECRouter;      /*CEC thread*//*CNcomment: CEC线程 */
    HI_UNF_HDMI_VIDEO_MODE_E enVidInMode; /*reservation,please setting VIDEO_MODE_YCBCR422 mode*//*CNcomment:保留，请配置为VIDEO_MODE_YCBCR422 */
}HDMI_COMM_ATTR_S;

/** VSDB Mode */
typedef enum 
{
    VSDB_MODE_NONE = 0x00,      
    VSDB_MODE_3D ,
    VSDB_MODE_4K ,
    VSDB_MODE_BUTT
}VSDB_MODE_E;


HDMI_COMM_ATTR_S *DRV_Get_CommAttr(HI_VOID);




void DRV_PrintCommAttr(HI_VOID);

HI_UNF_HDMI_VIDEO_MODE_E DRV_Get_VIDMode(HI_VOID);
void DRV_Set_VIDMode(HI_UNF_HDMI_VIDEO_MODE_E enVInMode);


HI_S32 DRV_Get_IsMce2App(HI_VOID);
void DRV_Set_Mce2App(HI_BOOL bSmooth);


HI_S32 DRV_Get_IsOpenedInBoot(HI_VOID);
void DRV_Set_OpenedInBoot(HI_BOOL bOpened);

HI_S32 DRV_Get_IsThreadStoped(HI_VOID);
void DRV_Set_ThreadStop(HI_BOOL bStop);
HI_VOID DRV_HDMI_SetSoftResume(HI_BOOL bSoftResume);
HI_BOOL DRV_HDMI_GetSoftResume(HI_VOID);
//g_stHdmiCommParam end

#define MAX_PROCESS_NUM 10
#define PROC_EVENT_NUM 5

typedef struct
{
    HI_U32      bUsed;
    HI_U32      CurEventNo;
    HI_U32      Event[PROC_EVENT_NUM];
    HI_U32      u32ProcHandle;
}HDMI_PROC_EVENT_S;


typedef struct
{
    HI_BOOL      bUnderScanDev;
}HDMI_PRIVATE_EDID_S;


typedef struct
{
    HI_BOOL            bOpen;
    HI_BOOL            bStart;
    HI_BOOL            bValidSinkCap;
    HDMI_PROC_EVENT_S  eventList[MAX_PROCESS_NUM];
    //HI_U32           Event[5];        /*Current Event Array, sequence will be change */
    HDMI_ATTR_S        stHDMIAttr;          /*HDMI implement parameter*//*CNcomment:HDMI 运行参数 */
    HI_BOOL            ForceUpdateFlag;
    HI_BOOL            partUpdateFlag;

    VSDB_MODE_E        enVSDBMode;
    HI_UNF_HDMI_AVI_INFOFRAME_VER2_S   stAVIInfoFrame;
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S   stAUDInfoFrame;

    HI_BOOL                            bCECEnable;
    HI_BOOL                            bCECStart;
    HI_U8                              u8CECCheckCount;
    HI_UNF_HDMI_CEC_STATUS_S           stCECStatus;

    HI_UNF_HDMI_DEFAULT_ACTION_E       enDefaultMode;
}HDMI_CHN_ATTR_S;

typedef struct
{
    HI_U32      bEdidLen;
    HI_U8      *u8Edid;
}HDMI_Test_EDID_S;

typedef struct
{
    HI_U32      overrun;        /*  Audio FIFO Overflow. This interrupt occurs if the audio FIFO overflows
                                 *  when more samples are written into it than are drawn out across the HDMI
                                 *  link. Such a condition can occur from a transient change in the Fs or 
                                 *  pixel clock rate.  */
                                 
    HI_U32      under_run;      /*  Audio FIFO Underflow. Similar to OVER_RUN.  This interrupt occurs when
                                 *  the audio FIFO empties. */     
                                    
    HI_U32      spdif_par;       /*  S/PDIF Parity Error.The S/PDIF stream includes a parity (P) bit at the 
                                 *  end of each sub-frame. An interrupt occurs if the calculated parity does
                                 *  not match the state of this bit. */    
}HDMI_INT_Err_Conut;

HDMI_CHN_ATTR_S *DRV_Get_ChnAttr(HI_VOID);

HI_U32 DRV_HDMI_SetDefaultAttr(HI_VOID);
HDMI_ATTR_S *DRV_Get_HDMIAttr(HI_UNF_HDMI_ID_E enHdmi);
HDMI_APP_ATTR_S   *DRV_Get_AppAttr(HI_UNF_HDMI_ID_E enHdmi);
HDMI_VIDEO_ATTR_S *DRV_Get_VideoAttr(HI_UNF_HDMI_ID_E enHdmi);
HDMI_AUDIO_ATTR_S *DRV_Get_AudioAttr(HI_UNF_HDMI_ID_E enHdmi);
HI_UNF_EDID_BASE_INFO_S *DRV_Get_SinkCap(HI_UNF_HDMI_ID_E enHdmi);

HDMI_PRIVATE_EDID_S *DRV_Get_PriSinkCap(HI_UNF_HDMI_ID_E enHdmi);



HI_BOOL DRV_Get_IsNeedForceUpdate(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_ForceUpdateFlag(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bupdate);

HI_BOOL DRV_Get_IsNeedPartUpdate(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_PartUpdateFlag(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bupdate);

HDMI_PROC_EVENT_S *DRV_Get_EventList(HI_UNF_HDMI_ID_E enHdmi);
HI_UNF_HDMI_CEC_STATUS_S *DRV_Get_CecStatus(HI_UNF_HDMI_ID_E enHdmi);

HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *DRV_Get_AviInfoFrm(HI_UNF_HDMI_ID_E enHdmi);
HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *DRV_Get_AudInfoFrm(HI_UNF_HDMI_ID_E enHdmi);

HI_BOOL DRV_Get_IsChnOpened(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_ChnOpen(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bChnOpen);

HI_BOOL DRV_Get_IsChnStart(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_ChnStart(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bChnStart);

HI_BOOL DRV_Get_IsCECEnable(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_CECEnable(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bCecEnable);

HI_BOOL DRV_Get_IsCECStart(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_CECStart(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bCecStart);

HI_BOOL DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_SinkCapValid(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bSinkValid);

HI_UNF_HDMI_DEFAULT_ACTION_E DRV_Get_DefaultOutputMode(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_DefaultOutputMode(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_HDMI_DEFAULT_ACTION_E enDefaultMode);

HI_DRV_HDMI_AUDIO_CAPABILITY_S *DRV_Get_OldAudioCap(void);

HI_U32 DRV_Get_DDCSpeed(void);
void DRV_Set_DDCSpeed(HI_U32 delayCount);

//
HDMI_EDID_S *DRV_Get_UserEdid(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_UserEdid(HI_UNF_HDMI_ID_E enHdmi,HDMI_EDID_S *pEDID);

//
HI_BOOL DRV_Get_IsUserEdid(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_UserEdidMode(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bUserEdid);

//
HI_U32 DRV_Get_DebugEdidNum(void);
HDMI_Test_EDID_S *DRV_Get_DebugEdid(HI_U32 u32Num);

VSDB_MODE_E DRV_Get_VSDBMode(HI_UNF_HDMI_ID_E enHdmi);
void DRV_Set_VSDBMode(HI_UNF_HDMI_ID_E enHdmi,VSDB_MODE_E enVSDBMode);

HI_BOOL DRV_Get_Is4KFmt(HI_DRV_DISP_FMT_E enFmt);
HI_BOOL DRV_Get_IsLCDFmt(HI_DRV_DISP_FMT_E enFmt);
HI_BOOL DRV_Get_IsPixelRepeatFmt(HI_DRV_DISP_FMT_E enFmt);

HI_BOOL DRV_Get_IsForceOutput(void);
void DRV_Set_ForceOutputMode(HI_BOOL bForce);

HI_BOOL DRV_Get_ForcePowerState(void); 
void DRV_Set_ForcePowerState(HI_BOOL bForce);

HI_U32 DRV_Calculate_TMDSCLK(HI_UNF_ENC_FMT_E fmt, HI_UNF_HDMI_DEEP_COLOR_E enDeepColorMode);
HI_UNF_ENC_FMT_E DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_E SrcFmt);
HI_DRV_DISP_FMT_E DRV_HDMI_Unf2DispFmt(HI_UNF_ENC_FMT_E SrcFmt);

//internal dubug option
//#define DEBUG_EVENTLIST
//#define DEBUG_NOTIFY_COUNT
//#define DEBUG_NEED_RESET

//#define DEBUG_TIMER
//#define DEBUG_PROCID
//#define DEBUG_EDID
//#define ANDROID_SUPPORT
//#define DEBUG_HDCP

#ifdef DEBUG_HDCP
#define HDCP_PRINT HI_PRINT
#else
#define HDCP_PRINT HI_INFO_HDMI
#endif


//#define EDID_FULL_COLOR_RANGE


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /* __DRV_GLOBAL_H__ */

/*------------------------------------END-------------------------------------*/

