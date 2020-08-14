
/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    drv_omxvdec.h
 *
 * Purpose: omxvdec export header file
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 * NOTICE !!
 * 该文件同时被component 和driver 引用
 * 如修改，两边均需重新编译!!!
 */

#ifndef __DRV_OMXVDEC_H__
#define __DRV_OMXVDEC_H__

#include <linux/ioctl.h>

#include "hi_type.h"
#include "vfmw.h"
#include "hi_drv_video.h"  //主要是输出颜色空间枚举
/*
base on version 2015102899

*/

#define OMXVDEC_VERSION                     (2016081600)

#define OMXVDEC_NAME                    "hi_omxvdec"
#define DRIVER_PATH                     "/dev/hi_omxvdec"

#define PATH_LEN                        (64)
#define NAME_LEN                        (64)

#define MAX(a, b)                       (((a) > (b)) ?  (a) : (b))
#define MIN(a, b)                       (((a) > (b)) ?  (b) : (a))

#define UHD_FRAME_WIDTH                    (4096)
#define UHD_FRAME_HEIGHT                (2304)
#define HD_FRAME_WIDTH                  (1920)
#define HD_FRAME_HEIGHT                 (1088)
#define SD_FRAME_WIDTH                  (1280)
#define SD_FRAME_HEIGHT                 (768)

#define MAX_SUPPORT_WIDTH               (UHD_FRAME_WIDTH)
#define MAX_SUPPORT_HEIGHT              (UHD_FRAME_HEIGHT)

#define DEFAULT_FRAME_WIDTH             (HD_FRAME_WIDTH)
#define DEFAULT_FRAME_HEIGHT            (HD_FRAME_HEIGHT)

#define DEFAULT_FRAME_SIZE(w, h)        (((w)*(h)*3)/2)
#define HI_OMX_GET_STRIDE(w)            ((w + (16-1))&(~(16-1)))


typedef enum  // omx color space enum
{
    OMX_PIX_FMT_NV12    ,          /* YUV 4:2:0 semiplanar */
    OMX_PIX_FMT_NV21    ,          /* YVU 4:2:0 semiplanar */
    OMX_PIX_FMT_YUV420Planar ,     /* YUV 4:2:0 planar     */
    OMX_PIX_FMT_YUV420Tile ,       /* YUV 4:2:0 tile       */
    OMX_PIX_BUTT
}OMX_PIX_FORMAT_E;

enum {
    VDEC_S_SUCCESS = 0,
    VDEC_S_FAILED  = 1,
};

/* omxvdec msg response types */
#define VDEC_MSG_RESP_BASE              (0x10000)
#define VDEC_MSG_RESP_OPEN              (VDEC_MSG_RESP_BASE + 0x1)
#define VDEC_MSG_RESP_START_DONE        (VDEC_MSG_RESP_BASE + 0x2)
#define VDEC_MSG_RESP_STOP_DONE         (VDEC_MSG_RESP_BASE + 0x3)
#define VDEC_MSG_RESP_PAUSE_DONE        (VDEC_MSG_RESP_BASE + 0x4)
#define VDEC_MSG_RESP_RESUME_DONE       (VDEC_MSG_RESP_BASE + 0x5)
#define VDEC_MSG_RESP_FLUSH_INPUT_DONE  (VDEC_MSG_RESP_BASE + 0x6)
#define VDEC_MSG_RESP_FLUSH_OUTPUT_DONE (VDEC_MSG_RESP_BASE + 0x7)
#define VDEC_MSG_RESP_INPUT_DONE        (VDEC_MSG_RESP_BASE + 0x8)
#define VDEC_MSG_RESP_OUTPUT_DONE       (VDEC_MSG_RESP_BASE + 0x9)
#define VDEC_MSG_RESP_MSG_STOP_DONE     (VDEC_MSG_RESP_BASE + 0xa)

/* omxvdec error code types */
#define VDEC_S_ERR_BASE                 (0x20000)
#define VDEC_ERR_FAIL                   (VDEC_S_ERR_BASE + 0x01)
#define VDEC_ERR_DYNAMIC_ALLOC_FAIL     (VDEC_S_ERR_BASE + 0x02)
#define VDEC_ERR_ILLEGAL_OP             (VDEC_S_ERR_BASE + 0x03)
#define VDEC_ERR_ILLEGAL_PARM           (VDEC_S_ERR_BASE + 0x04)
#define VDEC_ERR_BAD_POINTER            (VDEC_S_ERR_BASE + 0x05)
#define VDEC_ERR_BAD_HANDLE             (VDEC_S_ERR_BASE + 0x06)
#define VDEC_ERR_NOT_SUPPORTED          (VDEC_S_ERR_BASE + 0x07)
#define VDEC_ERR_BAD_STATE              (VDEC_S_ERR_BASE + 0x08)
#define VDEC_ERR_BUSY                   (VDEC_S_ERR_BASE + 0x09)
#define VDEC_ERR_HW_FATAL               (VDEC_S_ERR_BASE + 0x0a)
#define VDEC_ERR_BITSTREAM_ERR          (VDEC_S_ERR_BASE + 0x0b)
#define VDEC_ERR_QEMPTY                 (VDEC_S_ERR_BASE + 0x0c)
#define VDEC_ERR_QFULL                  (VDEC_S_ERR_BASE + 0x0d)
#define VDEC_ERR_INPUT_NOT_PROCESSED    (VDEC_S_ERR_BASE + 0x0e)
#define VDEC_ERR_INDEX_NOMORE           (VDEC_S_ERR_BASE + 0x0f)

#define VDEC_EVT_REPORT_BASE            (0x30000)
#define VDEC_EVT_REPORT_IMG_SIZE_CHG    (VDEC_EVT_REPORT_BASE + 0x1)
#define VDEC_EVT_REPORT_FRAME_RATE_CHG    (VDEC_EVT_REPORT_BASE + 0x2)
#define VDEC_EVT_REPORT_SCAN_TYPE_CHG    (VDEC_EVT_REPORT_BASE + 0x3)
#define VDEC_EVT_REPORT_HW_ERROR        (VDEC_EVT_REPORT_BASE + 0x4)
#define VDEC_EVT_REPORT_CROP_RECT_CHG    (VDEC_EVT_REPORT_BASE + 0x5)
#define VDEC_EVT_REPORT_DEC_SIZE_CHG    (VDEC_EVT_REPORT_BASE + 0x6)


/* ========================================================
 * internal struct enum definition
 * ========================================================*/
typedef enum {
    PORT_DIR_INPUT,
    PORT_DIR_OUTPUT,
    PORT_DIR_BOTH = 0xFFFFFFFF
}ePORT_DIR;

typedef enum {
    OMX_USE_NOSECURE,
    OMX_USE_SECURE,
    OMX_USE_NATIVE,
    OMX_ALLOCATE_NONSECURE,
    OMX_ALLOCATE_SECURE,
    OMX_ALLOCATE_DRIVER,
    OMX_BUTT_TYPE,
}eBUFFER_TYPE;

typedef VID_STD_E  VDEC_CODEC_TYPE;

/* CodecType Relative */
typedef enum {
    STD_WMV = STD_END_RESERVED + 1,
    STD_MJPEG,
    STD_EXT_RESERVED,
}OMXVDEC_CODEC_EXT;

typedef struct {
    HI_BOOL          is_tvp;
    HI_U32           cfg_width;
    HI_U32           cfg_height;
    HI_U32           cfg_stride;
    HI_U32           cfg_inbuf_num;
    HI_U32           cfg_outbuf_num;
    HI_U32           cfg_rotation;
    OMX_PIX_FORMAT_E cfg_color_format;
    HI_BOOL          m_use_native_buf;
    VDEC_CHAN_CFG_S  chan_cfg;
}OMXVDEC_DRV_CFG;

typedef struct {
    HI_S32           video_driver_fd;
    HI_S32           chan_handle;
    OMXVDEC_DRV_CFG  drv_cfg;
    HI_VOID*         yuv_fp;
    HI_U8*           chrom_l;
    HI_U32           chrom_l_size;
}OMXVDEC_DRV_CONTEXT;

/* video frame buffer description */
typedef struct {
    HI_U32   phyaddr_Y;
    HI_U32   phyaddr_C;
    HI_U32   stride;
    HI_U32   width;
    HI_U32   height;
    HI_BOOL  save_yuv;
    HI_CHAR  save_path[PATH_LEN];
}OMXVDEC_FRAME_S;

typedef struct {
    HI_U32 frame_width;
    HI_U32 frame_height;
    HI_U32 frame_stride;
}OMXVDEC_IMG_SIZE;

typedef struct {
    HI_U32 frame_width;
    HI_U32 frame_height;
    HI_U32 frame_stride;
    HI_U32 frame_size;
    HI_U32 max_frame_num;
    HI_U32 min_frame_num;
}OMXVDEC_DEC_SIZE;

typedef struct {
    HI_U32           phyaddr;
    HI_U32           buffer_len;
    HI_U32           align;
    HI_U32           data_offset;
    HI_U32           data_len;
    HI_U32           private_phyaddr;//VPSS 假4K输出需要额外信息
    HI_U32           private_len;
    HI_U32           flags;
    HI_S64           timestamp;
    eBUFFER_TYPE     buffer_type;
    ePORT_DIR        dir;
    OMXVDEC_FRAME_S  out_frame;
    HI_VOID         *bufferaddr;
    HI_VOID         *client_data;
    HI_U32           u32FrameIndex;
    HI_HANDLE        hTunnelSrc;
    HI_U32           u32FrameRate;
    HI_DRV_FIELD_MODE_E eOriginField;
    HI_BOOL          is_sec;
}OMXVDEC_BUF_DESC;

typedef struct {
    HI_U32           status_code;
    HI_U32            msgcode;
    HI_U32            msgdatasize;

    union {
    OMXVDEC_IMG_SIZE img_size;
    OMXVDEC_DEC_SIZE dec_size;
    OMXVDEC_BUF_DESC buf;
    } msgdata;

}OMXVDEC_MSG_INFO;

/* ========================================================
 * IOCTL for interaction with omx components
 * ========================================================*/
typedef struct {
    HI_S32    chan_num;
    HI_VOID  *in;
    HI_VOID  *out;
}OMXVDEC_IOCTL_MSG;

#define VDEC_IOCTL_MAGIC 'v'

#define VDEC_IOCTL_CHAN_CREATE   \
    _IO(VDEC_IOCTL_MAGIC, 1)

#define VDEC_IOCTL_CHAN_RELEASE  \
    _IO(VDEC_IOCTL_MAGIC, 2)

#define VDEC_IOCTL_SET_EXTRADATA \
    _IO(VDEC_IOCTL_MAGIC, 3)

#define VDEC_IOCTL_GET_EXTRADATA \
    _IO(VDEC_IOCTL_MAGIC, 4)

#define VDEC_IOCTL_FLUSH_PORT \
    _IO(VDEC_IOCTL_MAGIC, 5)

#define VDEC_IOCTL_CHAN_BIND_BUFFER \
    _IO(VDEC_IOCTL_MAGIC, 6)

#define VDEC_IOCTL_CHAN_UNBIND_BUFFER \
    _IO(VDEC_IOCTL_MAGIC, 7)

#define VDEC_IOCTL_FILL_OUTPUT_FRAME \
    _IO(VDEC_IOCTL_MAGIC, 8)

#define VDEC_IOCTL_EMPTY_INPUT_STREAM \
    _IO(VDEC_IOCTL_MAGIC, 9)

#define VDEC_IOCTL_CHAN_START \
    _IO(VDEC_IOCTL_MAGIC, 10)

#define VDEC_IOCTL_CHAN_STOP  \
    _IO(VDEC_IOCTL_MAGIC, 11)

#define VDEC_IOCTL_CHAN_PAUSE \
    _IO(VDEC_IOCTL_MAGIC, 12)

#define VDEC_IOCTL_CHAN_RESUME \
    _IO(VDEC_IOCTL_MAGIC, 13)

#define VDEC_IOCTL_CHAN_GET_MSG \
    _IO(VDEC_IOCTL_MAGIC, 14)

#define VDEC_IOCTL_CHAN_STOP_MSG \
    _IO(VDEC_IOCTL_MAGIC, 15)

#define VDEC_IOCTL_CHAN_ALLOC_BUF    \
    _IO(VDEC_IOCTL_MAGIC, 16)

#define VDEC_IOCTL_CHAN_RELEASE_BUF \
    _IO(VDEC_IOCTL_MAGIC, 17)

#define VDEC_IOCTL_CHAN_PORT_ENABLE \
    _IO(VDEC_IOCTL_MAGIC, 22)
#endif

