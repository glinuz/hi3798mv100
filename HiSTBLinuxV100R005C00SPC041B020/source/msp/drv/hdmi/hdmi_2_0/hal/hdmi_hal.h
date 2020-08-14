//#include "hdmi_drv_common.h"

#ifndef __HDMI_HAL_H__
#define __HDMI_HAL_H__
/**
* @brief KSV data object
*/

#ifdef HDMI_CEC_SUPPORT
#include "hal_cec.h"
#endif
#include "drv_hdmi_common.h"
#include "hdmi_emi.h"

typedef HI_VOID* HDMI_HANDLE;

typedef enum{
    HDMI_DEBUG_CMD_COLOR_BAR,
    HDMI_DEBUG_CMD_SW_RESET,
    HDMI_DEBUG_CMD_RGB2YUV,
    HDMI_DEBUG_CMD_YUV2RGB,
    HDMI_DEBUG_CMD_DITHER,
    HDMI_DEBUG_CMD_BYPASS,
    HDMI_DEBUG_CMD_DDC_FREQ,
    HDMI_DEBUG_CMD_PHY_DEFAULT_GET,
    HDMI_DEBUG_CMD_PHY_PARA_SET,
    HDMI_DEBUG_CMD_DUMP,
#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
    HDMI_DEBUG_CMD_PROC_MACH,
    HDMI_DEBUG_CMD_PROC_SCDC,
    HDMI_DEBUG_CMD_PROC_HDCP14,
    HDMI_DEBUG_CMD_PROC_HDCP22,
    HDMI_DEBUG_CMD_PROC_DDC,
    HDMI_DEBUG_CMD_PROC_CECTX,
    HDMI_DEBUG_CMD_PROC_CECRX,

    HDMI_DEBUG_CMD_DBG_VIDEO_GET,
    HDMI_DEBUG_CMD_DBG_VIDEO_SET,
    HDMI_DEBUG_CMD_SSC,

#endif
}HDMI_HAL_DEBUG_CMD_E;

typedef struct 
{
    HI_U32  u32RegAddr;
    HI_U32  u32RegVaule;
}HDMI_HAL_REG_S;

typedef struct{

    HI_U8   u8DataSwing;        //data-swing
    HI_U8   u8ClkSwing;         //CLK-swing
    HI_U8   u8SrcTermination;   //source termination
    HI_U8   u8DrvVnbValue;      //Tx driver vnb value setting	
    HI_U8   u8ClkSrcFineAdjust; // Fine adjust clock source termination
    HI_U8   u8TxRegZone;        // tx_reg_zone
    HI_U8   u8RiseTime;         //rise time
    HI_U8   u8FallTime;         // fall time

}HDMI_HAL_PHY_PARA_S;
typedef struct
{
    HDMI_HANDLE          hHdmiHw;
    HDMI_HANDLE          hHdmiDev;
    HI_U32               u32HdmiID;
    //HDMI_TX_CAPABILITY_E enTxCapability;
    HDMI_TX_CAPABILITY_S stTxCapability;
    HMDI_CALLBACK        pCallback;
    HDMI_VIDEO_CONFIG_S  stVideoCfg;
    HDMI_AUDIO_CONFIG_S  stAudioCfg;
#ifdef HDMI_CEC_SUPPORT
    HDMI_HAL_CEC_S*      pstCec;
#endif
}HDMI_HAL_CONTEXT_S;

typedef struct 
{
    HI_U32              u32DispFmt;
    HI_U32              u32PixClk;
    HDMI_COLORSPACE_E   enColorSpace;
    HDMI_DEEP_COLOR_E   enDeepColor;

}HDMI_HAL_BASE_PARAM_S;

typedef struct hiHDMI_HAL_S
{
    HDMI_HAL_CONTEXT_S stHalCtx;
    HI_VOID (*HAL_HDMI_HardwareInit)(struct hiHDMI_HAL_S *pstHdmiHal);
    HI_VOID (*HAL_HDMI_TmdsModeSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TMDS_MODE_E enTmdsMode);
    HI_VOID (*HAL_HDMI_AvMuteSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bAvMute);
    HI_VOID (*HAL_HDMI_InfoframeSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 u8InBuffer[]);
    HI_VOID (*HAL_HDMI_InfoframeEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable);
    HI_S32  (*HAL_HDMI_VideoPathSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_VIDEO_CONFIG_S* pstVideoCfg);
    HI_VOID (*HAL_HDMI_PhyOutputEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable);
    HI_VOID (*HAL_HDMI_PhyPowerEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable);
    HI_VOID (*HAL_HDMI_TxCapabilityGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TX_CAPABILITY_S *pstTxCapability); 
    HDMI_VIDEO_TIMING_E (*HAL_HDMI_DispFmt2HdmiTiming)(struct hiHDMI_HAL_S *pstHdmiHal, HI_U32 u32DispFmt);
    HI_VOID (*HAL_HDMI_EmiSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_EMI_CONFIG_S* pstEmiCfg);
    HI_VOID (*HAL_HDMI_EmiStatusGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_EMI_STATUS_S *pstEmiStatus);
    HI_VOID (*HAL_HDMI_CscParamSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_VIDEO_CONFIG_S* pstVideoCfg);

#ifndef HDMI_BUILD_IN_BOOT
    HI_S32  (*HAL_HDMI_DispFmtGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HAL_BASE_PARAM_S *pstParam);
    HI_VOID (*HAL_HDMI_SequencerHandlerProcess)(struct hiHDMI_HAL_S *pstHdmiHal);
    HI_VOID (*HAL_HDMI_HardwareStatusGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HARDWARE_STATUS_S* pstHwStatus);
    HI_VOID (*HAL_HDMI_HotPlugStatusGet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL* bHotPlug);
    HI_VOID (*HAL_HDMI_AudioMuteSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bAudioMute);
    HI_S32 (*HAL_HDMI_AudioPathSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_AUDIO_CONFIG_S* pstAudioCfg);
    HI_VOID (*HAL_HDMI_AudioPathEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable);
    HI_U32 (*HAL_HDMI_EdidRawDataRead)(struct hiHDMI_HAL_S *pstHdmiHal, HI_U32 u32Size, HI_U8 u8OutBuffer[]);
    HI_VOID (*HAL_HDMI_PhyOutputEnableGet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL* bEnable);
    HI_VOID (*HAL_HDMI_IrqEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_INT_TYPE_E enIntType, HI_BOOL bEnable);
    HI_VOID (*HAL_HDMI_VideoMuteSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bVideoMute);
    HI_VOID (*HAL_HDMI_BlackDataSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_BLACK_FRAME_INFO_S *pstBlackPram);
    HI_VOID (*HAL_HDMI_Debug)(struct hiHDMI_HAL_S *pstHdmiHal,HDMI_HAL_DEBUG_CMD_E enDebugCmd ,HI_VOID *pData);
    HI_U32  *(*HAL_HDMI_BaseAddrGet)(struct hiHDMI_HAL_S *pstHdmiHal);
#ifdef HDMI_HDR_SUPPORT  	
    HI_VOID (*HAL_HDMI_HdrTimerSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_TIMER_CONFIG_S *pstTimerCfg);
#endif
#endif

#ifdef HDMI_HDCP_SUPPORT
    HI_VOID (*HAL_HDMI_HdcpEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEnable);
    HI_S32 (*HAL_HDMI_HdcpModeSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HDCP_MODE_E enHdcpMode);
    HI_S32 (*HAL_HDMI_HdcpMcuCodeLoad)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HDCP_MODE_E enHdcpMode);
    HI_S32 (*HAL_HDMI_HdcpKsvListGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HDCP_KSVLIST_S* pstKsvList);
    HI_VOID (*HAL_HDMI_HdcpSinkCapabilityGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_HDCP_CAPABILITY_S *pstHdcpSupport);
#endif

#ifdef HDMI_CEC_SUPPORT
    HI_S32 (*HAL_HDMI_CecEnableSet)(struct hiHDMI_HAL_S *pstHdmiHal, HI_BOOL bEanble);
    HI_S32 (*HAL_HDMI_CecNetPing)(struct hiHDMI_HAL_S *pstHdmiHal, HI_U8 *pu8PhyAddr);
    HI_S32 (*HAL_HDMI_CecStatusGet)(struct hiHDMI_HAL_S *pstHdmiHal,HDMI_CEC_INFO_S *pstCecInfo);
    HI_S32 (*HAL_HDMI_CecMsgSend)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_CEC_CMD_S* pstMsg);
    HI_S32 (*HAL_HDMI_CecMsgReceive)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_CEC_CMD_S* pstCecCmd);
#endif
#ifdef HDMI_SCDC_SUPPORT
    HI_VOID (*HAL_HDMI_ScdcConfig)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_CONFIG_S* pstScdcConfig);
    HI_VOID (*HAL_HDMI_ScdcStatusGet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_STATUS_S* pstScdcStatus);
    HI_VOID (*HAL_HDMI_ScdcStatusSet)(struct hiHDMI_HAL_S *pstHdmiHal, HDMI_SCDC_STATUS_S* pstScdcStatus);
#endif
}HDMI_HAL_S;


/*******************************define_start**********************************************************
Function:      
HI_S32 HAL_HDMI_Open(HDMI_HAL_INIT_S *pstHalInit, HDMI_HAL_S** pstHalHandle)
* Description:  打开HDMI HAL模块
* Input:        pstHalInit:初始化配置       
* Output:       **pstHalHandle: hal层全局结构体指针
* Return:        成功 / 错误码
* Others:        无
************************************end****************************************************************/
HI_S32 HAL_HDMI_Open(HDMI_HAL_INIT_S *pstHalInit, HDMI_HAL_S** pstHalHandle);

/*******************************define_start**********************************************************
Function:      
void HAL_HDMI_Close(HDMI_HAL_S* pstHdmiHal)
* Description:  释放HDMI HAL模块
* Input:        *pstHdmiHal:hal层全局结构体指针         
* Output:       无
* Return:        无
* Others:        无
************************************end****************************************************************/
void HAL_HDMI_Close(HDMI_HAL_S* pstHdmiHal);
#endif
