/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_hdmirx.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/09/27
  Description   :
  History       :
  1.Date        : 2013/09/27
    Author      : t00202585
    Modification: Created file
******************************************************************************/
#ifndef __HI_UNF_HDMIRX_H__
#define __HI_UNF_HDMIRX_H__

/* add include here */
//#include <linux/ioctl.h>
//#include "hi_sdk_comm.h"
//#include "hi_module.h"
#include "hi_unf_common.h"
#include "hi_unf_ai.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      HDMI */
/** @{ */  /** <!-- [HDMI] */

/**the config parameter of HDMI interface*/
/**CNcomment:HDMI 接口参数配置 */
typedef struct hiUNF_HDMIRX_TIMING_INFO_S
{
    HI_U32                  			u32Width;           /**<HDMIRX Hactive*//**<CNcomment:HDMIRX 宽有效值 */
    HI_U32                  			u32Height;          /**<HDMIRX Vactive*//**<CNcomment:HDMIRX 高有效值 */
    HI_U32                  			u32FrameRate;       /**<HDMIRX frequency of vsync*//**<CNcomment:HDMIRX 场频率 */
    HI_UNF_COLOR_SPACE_E    			enColorSpace;       /**<HDMIRX color space*//**<CNcomment:HDMIRX 色度空间 */
    HI_UNF_VIDEO_FORMAT_E   			enPixelFmt;         /**<HDMIRX video formate*//**<CNcomment:HDMIRX 视频格式 */
    HI_UNF_PIXEL_BITWIDTH_E     		enBitWidth;         /**<HDMIRX video BitWidth*//**<CNcomment:HDMIRX 视频位宽 */
    HI_BOOL                 			bInterlace;         /**<HDMIRX video interlace or progressive*//**<CNcomment:HDMIRX HDMIRX视频扫描方式是逐行还是隔行 */
    HI_UNF_OVERSAMPLE_MODE_E    		enOversample;       /**<HDMIRX video Oversample*//**<CNcomment:HDMIRX 视频过采样大小 */
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E	en3dFmt;            /**<HDMIRX video 3D formate*//**<CNcomment:HDMIRX 视频3D格式 */
    HI_BOOL                            	bHdmiMode;          /**<HDMIRX video HDMI or DVI*//**<CNcomment:HDMIRX视频模式是HDMI还是DVI */
    HI_U32                  			u32Vblank;          /**<HDMIRX video Vblank，the value must set when the 3D formate is Frame PACKING*//**<CNcomment:HDMIRX Vblank值，仅在视频格式为3D Frame PACKING时有效 */
    HI_BOOL					            bPcMode;	    /**<HDMIRX PC Timing Mode flag*//**<CNcomment:HDMIRX PC Timing标志位*/
    HI_UNF_RGB_RANGE_E                 	enRGBRange;         /**<HDMIRX RGB Quantization Range*//**<CNcomment:HDMIRX RGB 量化范围*/
    HI_U32                              u32TimingIdx;	    /**<HDMIRX Timing Inedex Table*//**<CNcomment:HDMIRX Timing 对应的Index*/
    HI_BOOL                             bMHL;		    /**<HDMIRX MHL flag*//**<CNcomment:HDMIRX MHL标志位*/
}HI_UNF_HDMIRX_TIMING_INFO_S;

/**HDMI port ID *//**CNcomment:HDMI port ID  */
typedef enum hiUNF_HDMIRX_PORT_E
{
    HI_UNF_HDMIRX_PORT0,					       /**<HDMIRX port 0*/ /**<CNcomment:HDMIRX port0 */
    HI_UNF_HDMIRX_PORT1,					       /**<HDMIRX port 1*/ /**<CNcomment:HDMIRX port1 */
    HI_UNF_HDMIRX_PORT2,					       /**<HDMIRX port 2*/ /**<CNcomment:HDMIRX port2 */
    HI_UNF_HDMIRX_PORT3,					       /**<HDMIRX port 3*/ /**<CNcomment:HDMIRX port3 */
    HI_UNF_HDMIRX_PORT_BUTT
} HI_UNF_HDMIRX_PORT_E;

/**the config parameter of EDID interface*/
/**CNcomment:EDID 接口参数配置 */
typedef struct hiUNF_HDMIRX_EDID_S
{
    HI_U32   					au32Edid[256];	     /**<HDMIRX EDID Value*//**<CNcomment:HDMIRX EDID 值 */
    HI_U32   					au32Addr[4];	     /**<HDMIRX EDID port Address*//**<CNcomment:HDMIRX EDID 对应port口地址*/
}HI_UNF_HDMIRX_EDID_S;

/**the config parameter of HDCP interface*/
/**CNcomment:HDCP 接口参数配置 */
typedef struct hiUNF_HDMIRX_HDCP_S
{
    HI_U8   					au32Hdcp[332];        /**<HDMIRX HDCP Value*//**<CNcomment:HDMIRX HDCP 值 */
}HI_UNF_HDMIRX_HDCP_S;

/**the config parameter of OffLineDetect Status interface*/
/**CNcomment:离线检测 接口参数配置 */
typedef struct hiUNF_HDMIRX_OFF_LINE_DET_S
{
    HI_UNF_HDMIRX_PORT_E    			enPort;			/**<HDMIRX port ID */ /**<CNcomment:HDMIRX port ID */
    HI_BOOL 		    			bConnected;		/**<HDMIRX OffLineDetect Status for callback*/ /**<CNcomment:HDMIRX 离线检测状态，用于返回值 */
}HI_UNF_HDMIRX_OFF_LINE_DET_S;

/**
\brief the whole initialization of the hdmirx. CNcomment:HDMIRX接口驱动软件初始化 CNend
\attention  this func should be called before vo_init and after disp_init. CNcomment:必须在DISP驱动Setup之后和VO驱动Setup之前打开 CNend
\param CNcomment:无 CNend
\retval HI_SUCCESS  success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_Init(HI_VOID);

/**
\brief deinit the hdmirx. CNcomment:HDMIRX接口去初始化 CNend
\attention  this must be called after vo exited and before disp  exited . CNcomment:必须在VO驱动Exit之后和DISP驱动Exit之前打开 CNend
\param CNcomment:无 CNend
\retval HI_SUCCESS      success.CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_DeInit(HI_VOID);

/**
\brief create hdmirx device.Select the input port. CNcomment:创建HDMIRX设备,选择输入port口 CNend
\attention \n
\param[in] HI_UNF_HDMIRX_PORT_E  hdmirx device id. CNcomment:要打开的HDMIRX port口 ID CNend
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_Connect(HI_UNF_HDMIRX_PORT_E enPort);

/**
\brief disConnect the handler created by  HI_UNF_HDMIRX_Connect. CNcomment:销毁由HI_UNF_HDMIRX_Connect创建的句柄 CNend
\attention \n
\param CNcomment:无 CNend
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_DisConnect(HI_VOID);

/**
\brief Get the current HDCP authentication Status. CNcomment:查询当前HDCP状态
\attention \n
\param[in] *HI_BOOL  callback function CNcomment:回调函数 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_GetHdcpStatus(HI_BOOL *pbHdcpSta);

/**
\brief Get the current timing information. CNcomment:获取当前Timing信息
\attention \n
\param[in] *HI_UNF_HDMIRX_TIMING_INFO_S  callback function CNcomment:回调函数 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_GetTimingInfo(HI_UNF_HDMIRX_TIMING_INFO_S *pstTimingInfo);

/* Get the Timing hdcp authentication status */
/**
\brief Get the current Audio information. CNcomment:获取当前音频信息
\attention \n
\param[in] *HI_UNF_AI_HDMI_ATTR_S  callback function CNcomment:回调函数 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_GetAudioInfo(HI_UNF_AI_HDMI_ATTR_S *pstAudioInfo);

/**
\brief Get the signal status like no signal/unstable/support . CNcomment:查询当前视频信号状态如无信号、不稳定、稳定
\attention \n
\param[in] *HI_UNF_SIG_STATUS_E  callback function CNcomment:回调函数 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_GetSignalStatus(HI_UNF_SIG_STATUS_E *penStatus);

/**
\brief Get the Audio status like no signal/unstable/support . CNcomment:查询当前音频信号状态如无信号、不稳定、稳定
\attention \n
\param[in] *HI_UNF_SIG_STATUS_E  callback function CNcomment:回调函数 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_GetAudioStatus(HI_UNF_SIG_STATUS_E *penStatus);

/**
\brief Update the EDID in SRAM. CNcomment:更新EDID 至SRAM中 CNend
\attention \n
\param[in] *HI_UNF_HDMIRX_EDID_S  key struct point CNcomment: key的结构指针
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_UpdateEdid(HI_UNF_HDMIRX_EDID_S *pstEdid);

/**
\brief Load HDCP key. CNcomment:导入hdcpkey CNend
\attention \n
\param[in] *HI_UNF_HDMIRX_HDCP_S  key struct point CNcomment: key的结构指针
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_LoadHdcp(HI_UNF_HDMIRX_HDCP_S *pstHdcp);

/**
\brief  Enable the main thread of hdmirx or not CNcomment:HDMIRX 主线程使能 CNend
\attention \n
\param[in] HI_BOOL  Enable  main thread or no CNcomment: HDMIRX 主线程是否使能
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
//HI_S32 HI_UNF_HDMIRX_EnMainThread(HI_BOOL bEn);

/**
\brief  Get the port OffLineDetect Status CNcomment:获取port口离线检测状态 CNend
\attention \n
\param[in] HI_UNF_HDMIRX_PORT_E  The port ID  CNcomment: HDMIRX port口ID
\param[in] *HI_BOOL  callback the port OffLineDetect Status CNcomment:回调函数，返回当前口离线检测状态 CNend
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMIRX_GetOffLineDetStatus(HI_UNF_HDMIRX_PORT_E enPort, HI_BOOL *pbConnected);

/** @} */  /** <!-- ==== API declaration end ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

