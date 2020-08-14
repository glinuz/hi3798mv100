/*========================================================================
Open MAX   Component: Video Decoder
This module contains the class definition for openMAX decoder component.
========================================================================== */
#ifndef __VENC_DRV_CTX_H__
#define __VENC_DRV_CTX_H__

#include "hi_drv_venc.h"
#include "OMX_Types.h"

#define VENC_NAME	"/dev/hi_venc"   //"/dev/hisi_venc"
#define VENC_MAX_CHN  (8)
#define HI_INVALID_HANDLE (0xffffffff)

/********************************Macro Definition********************************/
/** \addtogroup      H_2_2_11 */
/** @{ */  /** <!-- 【VENC】 */

/**<Maximum encoding width*/
/**<CNcomment: 最大编码宽度*/
#define HI_VENC_MAX_WIDTH       (1920)
/**<Minimum encoding width*/
/**<CNcomment: 最小编码宽度*/
#define HI_VENC_MIN_WIDTH       (176)
/**<Maximum encoding height*/
/**<CNcomment: 最大编码高度*/
#define HI_VENC_MAX_HEIGTH      (1088)
/**<Minimum encoding height*/
/**<CNcomment: 最小编码高度*/
#define HI_VENC_MIN_HEIGTH      (144)
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
#define HI_VENC_MAX_SPLIT_SIZE  (0xFFFF)
/**<Minimum split size (in bytes)*/
/**<CNcomment: 最小Split字节*/
#define HI_VENC_MIN_SPLIT_SIZE  (512)
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
/**<Maximum threshold of  bitrate fluctuation*/		  
/**<CNcomment: 最大编码码率波动阈值*/
#define HI_VENC_MAX_RcThr        (100)
/** @} */  /** <!-- ==== Macro Definition end ==== */


typedef struct OPTM_venc_channel_S
{
    OMX_U32 chan_handle;
    OMX_S32 state;                /* 0: not create yet ; 1: start; 2:stop */
    venc_chan_cfg chan_cfg;
} venc_channel_S;

typedef struct OPTM_venc_stream_S
{
    //OMX_S32        chan_handle;
    void*      pStrmBufVirAddr;
    OMX_U32   u32StrmBufPhyAddr;
    OMX_U32   u32BufSize;
} venc_stream_buf_S;

typedef struct
{
    HI_U32 ProfileIDC;
    HI_U32 FrameWidthInMb;
    HI_U32 FrameHeightInMb;
    HI_U8  FrameCropLeft;
    HI_U8  FrameCropRight;
    HI_U8  FrameCropTop;
    HI_U8  FrameCropBottom;
} OMXVenc_H264e_SPS_S;

typedef struct
{
    HI_U32  ConstIntra;
    HI_S32  ChrQpOffset;
    HI_U32  H264HpEn;
    HI_U32  H264CabacEn;
    //HI_S32 *pScale8x8;
    HI_S32  Scale8x8[128];
} OMXVenc_H264e_PPS_S;

typedef struct
{
    /* header parameter */
    HI_U8  SpsStream[16];
    HI_U8  PpsStream[320];
    HI_U32 SpsBits;
    HI_U32 PpsBits;

    /*sps & pps info*/
    OMXVenc_H264e_SPS_S stSPSData;
    OMXVenc_H264e_PPS_S stPPSData;
} omx_sps_pps_data_S;

typedef struct venc_driver_context
{
    OMX_S32 video_driver_fd;                                //设备文件标识符，当video_driver_fd>0时，设备已经被打开(通常在初始化的时候打开)

    venc_channel_S venc_chan_attr;
    venc_stream_buf_S venc_stream_addr;
    omx_sps_pps_data_S stStrmHeadData;
} venc_drv_context;

OMX_S32 venc_init_drv_context(venc_drv_context* drv_ctx);

void venc_deinit_drv_context(venc_drv_context*  drv_ctx);

OMX_S32 channel_create(venc_drv_context* drv_ctx);

OMX_S32 channel_destroy(venc_drv_context* drv_ctx);

OMX_S32 channel_start(venc_drv_context* drv_ctx);

OMX_S32 channel_stop(venc_drv_context* drv_ctx);

OMX_S32 channel_request_IFrame(venc_drv_context* drv_ctx);

//OMX_S32 channel_pause(venc_drv_context *drv_ctx);

//OMX_S32 channel_resume(venc_drv_context *drv_ctx);

OMX_S32 channel_flush_port(venc_drv_context* drv_ctx,
                           OMX_U32 flush_dir);

OMX_S32 channel_get_msg(venc_drv_context* drv_ctx,
                        venc_msginfo* msginfo);

OMX_S32 channel_queue_Image(venc_drv_context* drv_ctx,
                            venc_user_buf* puser_buf);

OMX_S32 channel_queue_stream(venc_drv_context* drv_ctx, venc_user_buf* puser_buf);

#if 0
OMX_S32 channel_bind_buffer(venc_drv_context* drv_ctx,
                            venc_user_buf* puser_buf);

OMX_S32 channel_unbind_buffer(venc_drv_context* drv_ctx,
                              venc_user_buf* puser_buf);
#endif

void venc_get_default_attr(venc_drv_context* drv_ctx);

OMX_S32 venc_mmap_kerel_vir(venc_drv_context* drv_ctx, venc_user_buf* puser_buf);
OMX_S32 venc_unmmap_kerel_vir(venc_drv_context* drv_ctx, venc_user_buf* puser_buf);
OMX_S32 channel_get_sps_pps(venc_drv_context *drv_ctx,  DRV_VIDEO_PPS_SPS_DATA *pParameterSet);
OMX_S32 channel_set_attr(venc_drv_context* drv_ctx);
OMX_S32 channel_get_attr(venc_drv_context* drv_ctx);

OMX_U32 channel_get_current_level(OMX_U32 EncWidth, OMX_U32 EncHeight);
#endif
