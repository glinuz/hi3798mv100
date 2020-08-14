/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name             :   hi_mpi_venc.h
  Version               :   Initial Draft
  Author                :   Hisilicon multimedia software group
  Created               :   2010/03/31
  Last Modified         :
  Description           :
  Function List         :
  History               :
  1.Date                :   2010/03/31
    Author              :   j00131665
Modification            :   Created file
******************************************************************************/

#ifndef  __HI_MPI_VENC_H__
#define  __HI_MPI_VENC_H__

#include "hi_error_mpi.h"
#include "hi_unf_venc.h"
#include "drv_venc_ext.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/********************************Macro Definition********************************/
/** \addtogroup      H_2_2_11 */
/** @{ */  /** <!-- 【VENC】 */

/**<Maximum encoding width/height*/
/**<CNcomment: 最大编码宽度/高度*/
#define HI_VENC_MAX_WIDTH       (1920)
#define HI_VENC_MAX_HEIGTH      (1088)
/**<Minimum encoding width/height*/
/**<CNcomment: 最小编码宽度/高度*/
#ifdef HI_VENC_HEVC_SUPPORT
#define HI_VENC_MIN_WIDTH       (256)
#define HI_VENC_MIN_HEIGTH      (128)
#else
#define HI_VENC_MIN_WIDTH       (176)
#define HI_VENC_MIN_HEIGTH      (144)
#endif			
/**<Picture size alignment (in bytes)*/		  
/**<CNcomment: 图像大小对齐字节*/
#define HI_VENC_PIC_SZIE_ALIGN  (4)	
/**<Maximum buffer (in bytes)*/		  
/**<CNcomment: 最大缓存字节*/
#define HI_VENC_MAX_BUF_SIZE    (20*1024*1024)	
/**<Minimum buffer (in bytes)*/
/**<CNcomment: 最小缓存字节*/ 
#define HI_VENC_MIN_BUF_SIZE    (256*1024)	
/**<Minimum group of picture (GOP)*/
/**<CNcomment: 最小GOP*/
#define HI_VENC_MIN_GOP         (1)		
/**<Maximum output bit rate*/		  
/**<CNcomment: 最大输出码率*/
#define HI_VENC_MAX_bps         (50*1024*1024)
/**<Minimum output bit rate*/	
/**<CNcomment: 最小输出码率*/
#define HI_VENC_MIN_bps         (32*1024)	

/**<Maximum split size (in bytes)*/	
/**<CNcomment: 最大Split字节*/
#define HI_VENC_MAX_SPLIT_BYTE_SIZE  (0xFFFF)	
/**<Minimum split size (in bytes)*/	
/**<CNcomment: 最小Split字节*/
#define HI_VENC_MIN_SPLIT_BYTE_SIZE  (512)	
#if 0
/**<Maximum split size (in bytes)*/	
/**<CNcomment: 最大Split字节*/
#define HI_VENC_MAX_SPLIT_MB_LINE  (511)	
#endif
/**Maximum frame rate*/			  
/**<CNcomment: 最大帧率*/
#define HI_VENC_MAX_fps         (60)
/**Minimum frame rate*/		
/**<CNcomment: 最小帧率*/	  
#define HI_VENC_MIN_fps         (1)		
/**Maximum channel priority*/		
/**<CNcomment: 最大通道优先级*/	  
#define HI_VENC_MAX_PRIORITY    (8)					  
/**<Maximum Quantization Parameter*/		 
/**<CNcomment: 最大量化参数值*/
#define HI_VENC_MAX_QP          (51)
/**<Size of reserved bytes for the bit rate*/		 
/**<CNcomment: 码率保留字节大小*/
#define HI_VENC_STREAM_RESERV_SIZE  (48)
/**<Maximum quantization level*/		  
/**<CNcomment: 最大量化级别*/
#define HI_VENC_MAX_Q_VALUE     (99)	
/**<Minimum quantization level*/ 	 
/**<CNcomment: 最小量化级别*/ 
#define HI_VENC_MIN_Q_VALUE     (1)	
/**<Maximum threshold of  bitrate fluctuation*/		  
/**<CNcomment: 最大编码码率波动阈值*/
#define HI_VENC_MAX_RcThr        (100)



/** @} */  /** <!-- ==== Macro Definition end ==== */


HI_S32 HI_MPI_VENC_Init(HI_VOID);
HI_S32 HI_MPI_VENC_DeInit(HI_VOID);
HI_S32 HI_MPI_VENC_SetAttr(HI_HANDLE hVencChn, const HI_UNF_VENC_CHN_ATTR_S *pstAttr);
HI_S32 HI_MPI_VENC_GetAttr(HI_HANDLE hVencChn, HI_UNF_VENC_CHN_ATTR_S *pstAttr);
HI_S32 HI_MPI_VENC_Create(HI_HANDLE *phVencChn, const HI_UNF_VENC_CHN_ATTR_S *pstAttr);
HI_S32 HI_MPI_VENC_Destroy(HI_HANDLE hVencChn);
HI_S32 HI_MPI_VENC_AttachInput(HI_HANDLE hVencChn, HI_HANDLE hSrc);
HI_S32 HI_MPI_VENC_DetachInput(HI_HANDLE hVencChn);
HI_S32 HI_MPI_VENC_AcquireStream(HI_HANDLE hVencChn, HI_UNF_VENC_STREAM_S *pstStream, HI_U32 u32BlockFlag);
HI_S32 HI_MPI_VENC_ReleaseStream(HI_HANDLE hVencChn, const HI_UNF_VENC_STREAM_S *pstStream);
HI_S32 HI_MPI_VENC_Start(HI_HANDLE hVencChn);
HI_S32 HI_MPI_VENC_Stop(HI_HANDLE hVencChn);

HI_S32 HI_MPI_VENC_QueueFrame(HI_HANDLE hVencChn,HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo);
HI_S32 HI_MPI_VENC_DequeueFrame(HI_HANDLE hVencChn,HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo);
//HI_S32 HI_MPI_VENC_GetFrame(HI_HANDLE hVencChn, HI_UNF_VI_BUF_S *pstFrame);
//HI_S32 HI_MPI_VENC_PutFrame(HI_HANDLE hVencChn, const HI_UNF_VI_BUF_S *pstFrame);
HI_S32 HI_MPI_VENC_RequestIFrame(HI_HANDLE hVencChn);
HI_S32 HI_MPI_VENC_SetSource(HI_HANDLE hVencChn, HI_DRV_VENC_SRC_HANDLE_S* pstSrc);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif //__HI_MPI_VENC_H__
