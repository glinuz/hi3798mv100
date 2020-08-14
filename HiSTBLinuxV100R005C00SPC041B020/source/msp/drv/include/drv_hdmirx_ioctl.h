/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  drv_hdmirx_ioctl.h
* Description:
*
***********************************************************************************/

#ifndef __DRV_HDMIRX_IOCTL_H__
#define __DRV_HDMIRX_IOCTL_H__

#include "hi_debug.h"
#include "hi_unf_vi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

typedef enum hi_SIG_STATUS_E
{
    HI_SIG_SUPPORT = 0,
    HI_SIG_NO_SIGNAL,
    HI_SIG_NOT_SUPPORT,
    HI_SIG_UNSTABLE,
    HI_SIG_BUTT
} HI_SIG_STATUS_E;

typedef enum hiCOLOR_SPACE_E
{
    HI_COLOR_SPACE_YCBCR_601 = 0,
    HI_COLOR_SPACE_YCBCR_709,
    HI_COLOR_SPACE_RGB,
    HI_COLOR_SPACE_BUTT,
} HI_COLOR_SPACE_E;

typedef enum hiPIXEL_FORMAT_E
{
    HI_PIXEL_FORMAT_RGB_444 = 0,
    HI_PIXEL_FORMAT_YUV_420,
    HI_PIXEL_FORMAT_YUV_422,
    HI_PIXEL_FORMAT_YUV_444,
    HI_PIXEL_FORMAT_BUTT,
} HI_PIXEL_FORMAT_E;

typedef enum hiPIXEL_BITWIDTH_E
{
    HI_PIXEL_BITWIDTH_8BIT = 0,
    HI_PIXEL_BITWIDTH_10BIT,
    HI_PIXEL_BITWIDTH_12BIT,
    HI_PIXEL_BITWIDTH_BUTT,
} HI_PIXEL_BITWIDTH_E;

typedef enum hiOVERSAMPLE_MODE_E
{
    HI_OVERSAMPLE_1X = 0,
    HI_OVERSAMPLE_2X,
    HI_OVERSAMPLE_4X,
    HI_OVERSAMPLE_BUTT,
} HI_OVERSAMPLE_MODE_E;

typedef enum hiVIDEO_3D_FORMAT_E
{
    HI_VIDEO_3D_FORMAT_2D = 0,     /* 2D */
    HI_VIDEO_3D_FORMAT_SBS,        /* side-by-side half */
    HI_VIDEO_3D_FORMAT_TB,         /* top and bottom */
    HI_VIDEO_3D_FORMAT_FP,         /* frame packing */
    HI_VIDEO_3D_FORMAT_BUTT,
} HI_VIDEO_3D_FORMAT_E;

typedef struct hiHDMIRX_TIMING_INFO_S
{
    HI_U32               u32Width;
    HI_U32               u32Height;
    HI_U32               u32FrameRate;              /* P: input frame rate, I: input field rate */
    HI_COLOR_SPACE_E     enColorSpace;
    HI_PIXEL_FORMAT_E    enPixelFmt;
    HI_PIXEL_BITWIDTH_E  enBitWidth;
    HI_BOOL              bInterlace;
    HI_OVERSAMPLE_MODE_E enOverSampleMode;
    HI_VIDEO_3D_FORMAT_E en3dFmt;
    HI_BOOL              bDviMode;
    HI_U32               u32Vblank;                 /* vblank in 3D frame packing mode */
} HI_HDMIRX_TIMING_INFO_S;

typedef struct hiHDMIRX_SET_GPIO_PIN_S
{
    HI_U32 u32I2cNo;
    HI_U32 u32GpioScl;
    HI_U32 u32GpioSda;
    HI_U32 u32GpioRst;
    HI_U32 u32GpioInt;
    HI_BOOL bSigLowCi2ca;
} HI_HDMIRX_SET_GPIO_PIN_S;

typedef enum hiIOC_HDMIRX_E
{
    IOC_HDMIRX_CONNECT = 0,
    IOC_HDMIRX_DIS_CONNECT,
    IOC_HDMIRX_SET_GPIO_PIN,
    IOC_HDMIRX_GET_STATUS,
    IOC_HDMIRX_GET_ATTR,
    IOC_HDMIRX_BUTT,
} IOC_HDMIRX_E;

#ifdef HI_HDMI_RX_INSIDE

#define HIIOC_HDMIRX_CONNECT                _IOW    ('H', 0,    HI_UNF_HDMIRX_PORT_E)           /* connect hdmirx and select hdmirx port */
#define HIIOC_HDMIRX_DISCONNECT             _IO     ('H', 1)                                    /* disconnect hdmirx */
#define HIIOC_HDMIRX_G_STATUS               _IOR    ('H', 2,    HI_UNF_SIG_STATUS_E)            /* Get the signal status */
#define HIIOC_HDMIRX_G_TIMING_INFO          _IOR    ('H', 3,    HI_UNF_HDMIRX_TIMING_INFO_S)    /* get the timing info like hactive/vactive/colorspace etc. */
#define HIIOC_HDMIRX_G_HDCP_STATUS          _IOR    ('H', 4,    HI_BOOL)                        /* get the timing info like hactive/vactive/colorspace etc. */
#define HIIOC_HDMIRX_S_UPDATE_EDID          _IOW    ('H', 5,    HI_UNF_HDMIRX_EDID_S)           /* update the edid */
#define HIIOC_HDMIRX_S_LOAD_HDCP            _IOW    ('H', 6,    HI_UNF_HDMIRX_HDCP_S)           /* Load HDCP */
#define HIIOC_HDMIRX_EN_MONITOR             _IOW    ('H', 7,    HI_BOOL)                        /* enable/disable the main thread */
#define HIIOC_HDMIRX_G_OFF_LINE_DET_STA     _IOWR   ('H', 8,    HI_UNF_HDMIRX_OFF_LINE_DET_S)   /* detect other port connect to tx or not */
#define HIIOC_HDMIRX_G_AUDIO_INFO           _IOR    ('H', 9,    HI_UNF_AI_HDMI_ATTR_S)
#define HIIOC_HDMIRX_G_AudioSTATUS          _IOR    ('H', 10,   HI_UNF_SIG_STATUS_E)

#else

#define CMD_HDMIRX_CONNECT _IO(HI_ID_SIL9293, IOC_HDMIRX_CONNECT)
#define CMD_HDMIRX_DIS_CONNECT _IO(HI_ID_SIL9293, IOC_HDMIRX_DIS_CONNECT)
#define CMD_HDMIRX_SET_GPIO_PIN _IOWR(HI_ID_SIL9293, IOC_HDMIRX_SET_GPIO_PIN, HI_HDMIRX_SET_GPIO_PIN_S)
#define CMD_HDMIRX_GET_STATUS _IOWR(HI_ID_SIL9293, IOC_HDMIRX_GET_STATUS, HI_SIG_STATUS_E)
#define CMD_HDMIRX_GET_ATTR _IOWR(HI_ID_SIL9293, IOC_HDMIRX_GET_ATTR, HI_HDMIRX_TIMING_INFO_S)

#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif //__DRV_HDMIRX_IOCTL_H__

