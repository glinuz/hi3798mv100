
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_drv_video.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/11/17
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __HI_DRV_VIDEO_H__
#define __HI_DRV_VIDEO_H__

#include "hi_type.h"
#include "hi_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

typedef enum hiDRV_COLOR_SYS_E
{
    HI_DRV_COLOR_SYS_AUTO = 0,
    HI_DRV_COLOR_SYS_PAL,
    HI_DRV_COLOR_SYS_NTSC,
    HI_DRV_COLOR_SYS_SECAM,
    HI_DRV_COLOR_SYS_PAL_M,
    HI_DRV_COLOR_SYS_PAL_N,
    HI_DRV_COLOR_SYS_PAL_60,
    HI_DRV_COLOR_SYS_NTSC443,
    HI_DRV_COLOR_SYS_NTSC_50,

    HI_DRV_COLOR_SYS_BUTT
} HI_DRV_COLOR_SYS_E;

/**Defines the oversample mode of the current input source.*/
/**CNcomment: 定义当前输入源的过采样模式*/
typedef enum hiDRV_OVERSAMPLE_MODE_E
{
    HI_DRV_OVERSAMPLE_1X = 0,
    HI_DRV_OVERSAMPLE_2X,
    HI_DRV_OVERSAMPLE_4X,
    HI_DRV_OVERSAMPLE_BUTT,
} HI_DRV_OVERSAMPLE_MODE_E;

typedef enum hiDRV_PIXEL_BITWIDTH_E
{
    HI_DRV_PIXEL_BITWIDTH_8BIT = 0,
    HI_DRV_PIXEL_BITWIDTH_10BIT,
    HI_DRV_PIXEL_BITWIDTH_12BIT,
    HI_DRV_PIXEL_BITWIDTH_10BIT_CTS,
    HI_DRV_PIXEL_BITWIDTH_BUTT,
} HI_DRV_PIXEL_BITWIDTH_E;
typedef enum hiDRV_PIX_FORMAT_E
{
    /* RGB formats */
    HI_DRV_PIX_FMT_RGB332 = 0, /*  8  RGB-3-3-2     */
    HI_DRV_PIX_FMT_RGB444  ,   /* 16  xxxxrrrr ggggbbbb */
    HI_DRV_PIX_FMT_RGB555  ,   /* 16  RGB-5-5-5     */
    HI_DRV_PIX_FMT_RGB565  ,   /* 16  RGB-5-6-5     */
    HI_DRV_PIX_FMT_BGR565  ,   /* 16  RGB-5-6-5     */
    HI_DRV_PIX_FMT_RGB555X ,   /* 16  RGB-5-5-5 BE  */
    HI_DRV_PIX_FMT_RGB565X ,   /* 16  RGB-5-6-5 BE  */
    HI_DRV_PIX_FMT_BGR666  ,   /* 18  BGR-6-6-6   */
    HI_DRV_PIX_FMT_BGR24   ,   /* 24  BGR-8-8-8     */
    HI_DRV_PIX_FMT_RGB24   ,   /* 24  RGB-8-8-8     */
    HI_DRV_PIX_FMT_BGR32   ,   /* 32  BGR-8-8-8-8   */
    HI_DRV_PIX_FMT_RGB32   ,   /* 32  RGB-8-8-8-8   */

    /****** HISI CLUT formats ******/
    HI_DRV_PIX_FMT_CLUT_1BPP,
    HI_DRV_PIX_FMT_CLUT_2BPP,
    HI_DRV_PIX_FMT_CLUT_4BPP,
    HI_DRV_PIX_FMT_CLUT_8BPP,
    HI_DRV_PIX_FMT_ACLUT_44,
    HI_DRV_PIX_FMT_ACLUT_88,

    /****** HISI 16bit RGB formats ******/
    HI_DRV_PIX_FMT_ARGB4444,
    HI_DRV_PIX_FMT_ABGR4444,
    HI_DRV_PIX_FMT_RGBA4444,

    HI_DRV_PIX_FMT_ARGB1555,
    HI_DRV_PIX_FMT_ABGR1555,
    HI_DRV_PIX_FMT_RGBA5551,

    /****** HISI 24bit RGB formats ******/
    HI_DRV_PIX_FMT_ARGB8565,
    HI_DRV_PIX_FMT_ABGR8565,
    HI_DRV_PIX_FMT_RGBA5658,

    HI_DRV_PIX_FMT_ARGB6666,
    HI_DRV_PIX_FMT_RGBA6666,

    /****** HISI 32bit RGB formats ******/
    HI_DRV_PIX_FMT_ARGB8888,
    HI_DRV_PIX_FMT_ABGR8888,
    HI_DRV_PIX_FMT_RGBA8888,

    HI_DRV_PIX_FMT_AYUV8888,
    HI_DRV_PIX_FMT_YUVA8888,

    /* Grey formats */
    HI_DRV_PIX_FMT_GREY    ,    /*  8  Greyscale     */
    HI_DRV_PIX_FMT_Y4      ,    /*  4  Greyscale     */
    HI_DRV_PIX_FMT_Y6      ,    /*  6  Greyscale     */
    HI_DRV_PIX_FMT_Y10     ,    /* 10  Greyscale     */
    HI_DRV_PIX_FMT_Y12     ,    /* 12  Greyscale     */
    HI_DRV_PIX_FMT_Y16     ,    /* 16  Greyscale     */

    /* Grey bit-packed formats */
    HI_DRV_PIX_FMT_Y10BPACK    ,   /* 10  Greyscale bit-packed */

    /* Palette formats */
    HI_DRV_PIX_FMT_PAL8    ,    /*  8  8-bit palette */

    /* Luminance+Chrominance formats */
    HI_DRV_PIX_FMT_YVU410  ,    /*  9  YVU 4:1:0     */
    HI_DRV_PIX_FMT_YVU420  ,    /* 12  YVU 4:2:0     */
    HI_DRV_PIX_FMT_YUYV    ,    /* 16  YUV 4:2:2     */
    HI_DRV_PIX_FMT_YYUV    ,    /* 16  YUV 4:2:2     */
    HI_DRV_PIX_FMT_YVYU    ,    /* 16  YVU 4:2:2 */
    HI_DRV_PIX_FMT_UYVY    ,    /* 16  YUV 4:2:2     */
    HI_DRV_PIX_FMT_VYUY    ,    /* 16  YUV 4:2:2     */
    HI_DRV_PIX_FMT_YUV422P ,    /* 16  YVU422 planar */
    HI_DRV_PIX_FMT_YUV411P ,    /* 16  YVU411 planar */
    HI_DRV_PIX_FMT_Y41P    ,    /* 12  YUV 4:1:1     */
    HI_DRV_PIX_FMT_YUV444  ,    /* 16  xxxxyyyy uuuuvvvv */
    HI_DRV_PIX_FMT_YUV555  ,    /* 16  YUV-5-5-5     */
    HI_DRV_PIX_FMT_YUV565  ,    /* 16  YUV-5-6-5     */
    HI_DRV_PIX_FMT_YUV32   ,    /* 32  YUV-8-8-8-8   */
    HI_DRV_PIX_FMT_YUV410  ,    /*  9  YUV 4:1:0     */
    HI_DRV_PIX_FMT_YUV420  ,    /* 12  YUV 4:2:0     */
    HI_DRV_PIX_FMT_HI240   ,    /*  8  8-bit color   */
    HI_DRV_PIX_FMT_HM12    ,    /*  8  YUV 4:2:0 16x16 macroblocks */
    HI_DRV_PIX_FMT_M420    ,    /* 12  YUV 4:2:0 2 lines y,
                                       1 line uv interleaved */

    /* two planes -- one Y, one Cr + Cb interleaved  */
    HI_DRV_PIX_FMT_NV08    ,     /*08  Y/CbCr 4:0:0 @*/
    HI_DRV_PIX_FMT_NV80    ,     /*08  Y/CrCb 4:0:0 @*/
    HI_DRV_PIX_FMT_NV12    ,    /* 12  Y/CbCr 4:2:0  */
    HI_DRV_PIX_FMT_NV21    ,    /* 12  Y/CrCb 4:2:0  */
    HI_DRV_PIX_FMT_NV12_411,    /* 12  Y/CbCr 4:1:1  @*/
    HI_DRV_PIX_FMT_NV16    ,    /* 16  Y/CbCr 4:2:2  */
    HI_DRV_PIX_FMT_NV61    ,    /* 16  Y/CrCb 4:2:2  */
    HI_DRV_PIX_FMT_NV16_2X1,    /* 16  Y/CbCr 4:2:2 2X1 @*/
    HI_DRV_PIX_FMT_NV61_2X1,    /* 16  Y/CrCb 4:2:2  2X1 @*/
    HI_DRV_PIX_FMT_NV24    ,    /* 24  Y/CbCr 4:4:4  */
    HI_DRV_PIX_FMT_NV42    ,    /* 24  Y/CrCb 4:4:4  */
    HI_DRV_PIX_FMT_NV42_RGB,    /* 24  G/RB 4:4:4  */

    /* two non contiguous planes - one Y, one Cr + Cb interleaved  */
    HI_DRV__PIX_FMT_NV12M  ,    /* 12  Y/CbCr 4:2:0  */
    HI_DRV__PIX_FMT_NV12MT ,    /* 12  Y/CbCr 4:2:0 64x32 macroblocks */

    /* three non contiguous planes - Y, Cb, Cr */
    HI_DRV_PIX_FMT_YUV420M ,    /* 12  YUV420 planar */

    /* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
    HI_DRV_PIX_FMT_SBGGR8  ,    /*  8  BGBG.. GRGR.. */
    HI_DRV_PIX_FMT_SGBRG8  ,    /*  8  GBGB.. RGRG.. */
    HI_DRV_PIX_FMT_SGRBG8  ,    /*  8  GRGR.. BGBG.. */
    HI_DRV_PIX_FMT_SRGGB8  ,    /*  8  RGRG.. GBGB.. */
    HI_DRV_PIX_FMT_SBGGR10 ,    /* 10  BGBG.. GRGR.. */
    HI_DRV_PIX_FMT_SGBRG10 ,    /* 10  GBGB.. RGRG.. */
    HI_DRV_PIX_FMT_SGRBG10 ,    /* 10  GRGR.. BGBG.. */
    HI_DRV_PIX_FMT_SRGGB10 ,    /* 10  RGRG.. GBGB.. */
    HI_DRV_PIX_FMT_SBGGR12 ,    /* 12  BGBG.. GRGR.. */
    HI_DRV_PIX_FMT_SGBRG12 ,    /* 12  GBGB.. RGRG.. */
    HI_DRV_PIX_FMT_SGRBG12 ,    /* 12  GRGR.. BGBG.. */
    HI_DRV_PIX_FMT_SRGGB12 ,    /* 12  RGRG.. GBGB.. */

    /****** HISI Luminance+Chrominance formats ******/

    /****** HISI  contiguoustwo planes -- one Y, one Cr + Cb interleaved ******/
    HI_DRV_PIX_FMT_NV08_CMP,     /*08   Y/CbCr 4:0:0 compressed @*/
    HI_DRV_PIX_FMT_NV80_CMP,     /*08   Y/CrCb 4:0:0 compressed @*/
    HI_DRV_PIX_FMT_NV12_CMP ,    /* 12  Y/CbCr 4:2:0 compressed */
    HI_DRV_PIX_FMT_NV21_CMP ,    /* 12  Y/CrCb 4:2:0 compressed */
    HI_DRV_PIX_FMT_NV16_CMP ,    /* 16  Y/CbCr 4:2:2 compressed */
    HI_DRV_PIX_FMT_NV61_CMP ,    /* 16  Y/CrCb 4:2:2 compressed */
    HI_DRV_PIX_FMT_NV16_2X1_CMP,    /* 16  Y/CbCr 4:2:2   2X1@*/
    HI_DRV_PIX_FMT_NV61_2X1_CMP,    /* 16  Y/CrCb 4:2:2  2X1@*/
    HI_DRV_PIX_FMT_NV24_CMP ,    /* 24  Y/CbCr 4:4:4 compressed */
    HI_DRV_PIX_FMT_NV42_CMP ,    /* 24  Y/CrCb 4:4:4 compressed */

    HI_DRV_PIX_FMT_NV12_TILE,    /* 12 tile  */
    HI_DRV_PIX_FMT_NV21_TILE,    /* 21 tile  */
    HI_DRV_PIX_FMT_YUV400_TILE,    /* 21 tile  */

    HI_DRV_PIX_FMT_NV12_TILE_CMP,   /* 12 tile compressed */
    HI_DRV_PIX_FMT_NV21_TILE_CMP,   /* 21 tile compressed */

    /****** HISI three non contiguous planes - Y, Cb, Cr ******/
    HI_DRV_PIX_FMT_YUV400  ,   /*08  YUV400 planar @*/
    HI_DRV_PIX_FMT_YUV410p  ,   /*10  YUV410 planar @*/
    HI_DRV_PIX_FMT_YUV420p ,   /*12  YUV420 planar @*/
    HI_DRV_PIX_FMT_YUV411  ,   /*12  YUV411  planar @*/
    HI_DRV_PIX_FMT_YUV422_1X2, /*16  YUV422  planar 1X2 @*/
    HI_DRV_PIX_FMT_YUV422_2X1, /*16  YUV422  planar 2X1@*/
    HI_DRV_PIX_FMT_YUV_444 ,   /*24  YUV444  planar @*/

    /****** HISI three non contiguous planes - Y, Cb, Cr ******/

    HI_DRV_PIX_BUTT


}HI_DRV_PIX_FORMAT_E;

/* video frame filed in buffer */
typedef enum hiDRV_FIELD_MODE_E
{
    HI_DRV_FIELD_TOP = 0,
    HI_DRV_FIELD_BOTTOM,
    HI_DRV_FIELD_ALL,
    HI_DRV_FIELD_BUTT
}HI_DRV_FIELD_MODE_E;

/* video frame type */
typedef enum hiDRV_FRAME_TYPE_E
{
    HI_DRV_FT_NOT_STEREO = 0,
    HI_DRV_FT_SBS,
    HI_DRV_FT_TAB,
    HI_DRV_FT_FPK,
    HI_DRV_FT_TILE,
    HI_DRV_FT_BUTT
}HI_DRV_FRAME_TYPE_E;

/* source color space */
typedef enum hiDRV_COLOR_SPACE_E
{
    HI_DRV_CS_UNKNOWN = 0,
    HI_DRV_CS_DEFAULT,

    HI_DRV_CS_BT601_YUV_LIMITED,/* BT.601 */
    HI_DRV_CS_BT601_YUV_FULL,
    HI_DRV_CS_BT601_RGB_LIMITED,
    HI_DRV_CS_BT601_RGB_FULL,

    HI_DRV_CS_NTSC1953,

    /* These should be useful.  Assume 601 extents. */
    HI_DRV_CS_BT470_SYSTEM_M,
    HI_DRV_CS_BT470_SYSTEM_BG,

    HI_DRV_CS_BT709_YUV_LIMITED,/* BT.709 */
    HI_DRV_CS_BT709_YUV_FULL,
    HI_DRV_CS_BT709_RGB_LIMITED,
    HI_DRV_CS_BT709_RGB_FULL,
    HI_DRV_CS_BT2020_YUV_LIMITED,/* ::::Current Used:::: BT.2020 */
    HI_DRV_CS_BT2020_YUV_FULL,
    HI_DRV_CS_BT2020_RGB_LIMITED,
    HI_DRV_CS_BT2020_RGB_FULL,   /* ::::Current Used:::: */

    HI_DRV_CS_REC709,      /* HD and modern captures. */

    HI_DRV_CS_SMPT170M, /* ITU-R 601 -- broadcast NTSC/PAL */
    HI_DRV_CS_SMPT240M, /* 1125-Line (US) HDTV */

    HI_DRV_CS_BT878,    /* broken BT878 extents
                           (601, luma range 16-253 instead of 16-235) */

    HI_DRV_CS_XVYCC,

    /* I know there will be cameras that send this.  So, this is
     * unspecified chromaticities and full 0-255 on each of the
     * Y'CbCr components
     */
    HI_DRV_CS_JPEG,
    HI_DRV_CS_BUTT
} HI_DRV_COLOR_SPACE_E;

/* frame rotation angle */
typedef enum hiDRV_ROT_ANGLE_E
{
    HI_DRV_ROT_ANGLE_0 = 0,
    HI_DRV_ROT_ANGLE_90,
    HI_DRV_ROT_ANGLE_180,
    HI_DRV_ROT_ANGLE_270,
    HI_DRV_ROT_ANGLE_BUTT
}HI_DRV_ROT_ANGLE_E;

/* video frame aspect ratio mode */
typedef enum hiDRV_ASP_RAT_MODE_E
{
    HI_DRV_ASP_RAT_MODE_FULL = 0x0,
    HI_DRV_ASP_RAT_MODE_LETTERBOX,
    HI_DRV_ASP_RAT_MODE_PANANDSCAN,
    HI_DRV_ASP_RAT_MODE_COMBINED,
    HI_DRV_ASP_RAT_MODE_FULL_H,
    HI_DRV_ASP_RAT_MODE_FULL_V,
    HI_DRV_ASP_RAT_MODE_CUSTOMER,
    HI_DRV_ASP_RAT_MODE_TV,
    HI_DRV_ASP_RAT_MODE_BUTT
}HI_DRV_ASP_RAT_MODE_E;

typedef struct hiDRV_CROP_RECT_S
{
    HI_U32 u32LeftOffset;
    HI_U32 u32TopOffset;
    HI_U32 u32RightOffset;
    HI_U32 u32BottomOffset;
}HI_DRV_CROP_RECT_S;


/* aspect ratio, for monitor or pixel.
  0<= arw <= 256, 0<=arh<=256, and '1/16  <= arw/arh <= 16'.
  e.g., if aspect ratio is 16:9, you can set arw as 16 and arh as 9,
  OR arw as 160, arh as 90.
  Exceptive :
  0:1 means unknown;
  0:2 means display pixel 1:1
  */
typedef struct hiDRV_ASPECT_RATIO_S
{
    HI_U32 u32ARw;
    HI_U32 u32ARh;
}HI_DRV_ASPECT_RATIO_S;

/* video frame buffer physical address */
typedef struct hiDRV_VID_FRAME_ADDR_S
{
    /* Y address*/
    HI_U32  u32PhyAddr_YHead; /* only for compress format */
    HI_U32  u32PhyAddr_Y;
    HI_U64  vir_addr_y;
    HI_U32  u32Stride_Y;

    /* C OR Cb address*/
    HI_U32  u32PhyAddr_CHead;
    HI_U32  u32PhyAddr_C;
    HI_U64  vir_addr_c;
    HI_U32  u32Stride_C;

    /* Cr address*/
    HI_U32  u32PhyAddr_CrHead;
    HI_U32  u32PhyAddr_Cr;
    HI_U32  u32Stride_Cr;

    HI_U32  u32Head_Stride;
    HI_U32  u32Head_Size;
}HI_DRV_VID_FRAME_ADDR_S;

typedef enum hiDRV_BUF_ADDR_E
{
    HI_DRV_BUF_ADDR_LEFT  = 0,
    HI_DRV_BUF_ADDR_RIGHT = 1,  /* only for right eye frame of 3D video */
    HI_DRV_BUF_ADDR_MAX
}HI_DRV_BUF_ADDR_E;

#define DEF_HI_DRV_FRAME_INFO_SIZE 96

/* define of how to adjust the TB match */
typedef enum hiDRV_VIDEO_TB_ADJUST_E
{
    HI_DRV_VIDEO_TB_PLAY = 0,
    HI_DRV_VIDEO_TB_REPEAT,
    HI_DRV_VIDEO_TB_DISCARD,
    HI_DRV_VIDEO_TB_BUTT
}HI_DRV_VIDEO_TB_ADJUST_E;


typedef enum hiDRV_SOURCE_E
{
    HI_DRV_SOURCE_DTV = 0,
    HI_DRV_SOURCE_USB,

    HI_DRV_SOURCE_ATV,
    HI_DRV_SOURCE_SCART,
    HI_DRV_SOURCE_SVIDEO,
    HI_DRV_SOURCE_CVBS,
    HI_DRV_SOURCE_VGA,
    HI_DRV_SOURCE_YPBPR,
    HI_DRV_SOURCE_HDMI,

    HI_DRV_SOURCE_BUTT
} HI_DRV_SOURCE_E;

/* Source Timing Info */
typedef struct hiDRV_VIDEO_ORIGINAL_INFO_S
{
    HI_DRV_SOURCE_E             enSource;       //DTV Default HI_DRV_SOURCE_DTV
    HI_U32                      u32Width;       //source width
    HI_U32                      u32Height;      //source height
    HI_U32                      u32FrmRate;     //source framerate
    HI_DRV_FRAME_TYPE_E         en3dType;       //source 3D Type
    HI_DRV_COLOR_SPACE_E        enSrcColorSpace;//Only use three Type:BT601_YUV_LIMITED,BT709_YUV_LIMITED,BT709_RGB_FULL
    HI_DRV_COLOR_SYS_E          enColorSys;     //DTV Default HI_DRV_COLOR_SYS_AUTO
    HI_BOOL                     bGraphicMode;   //DTV default HI_FALSE
    HI_BOOL                     bInterlace;     //source is Interlace or Progress
}HI_DRV_VIDEO_ORIGINAL_INFO_S;

typedef struct hiDRV_LOWDELAY_STAT_INFO_S
{
    HI_U32  u32OmxReportDoneTime;
    HI_U32  u32OverlayQueueTime;
    HI_U32  u32WinGetFrameTime;
    HI_U32  u32WinConfigTime;
}HI_DRV_LOWDELAY_STAT_INFO_S;


typedef struct hiDRV_VIDEO_DOLBY_METADATA_STREAM_S
{
    HI_U32                              u32Addr;        /**<bit-stream metadata address.*/ /**<CNcomment: metadata???????*/
    HI_U32                              u32Length;      /**<bit-stream metadata length.*/ /**<CNcomment: metadata???????*/
}HI_DRV_VIDEO_DOLBY_METADATA_STREAM_S;

typedef struct hiDRV_VIDEO_DOLBY_INFO_S
{
    HI_DRV_VIDEO_DOLBY_METADATA_STREAM_S    stMetadata;     /**<metadata in Dolby frame.*/ /**<CNcomment: Dolby??metadata??*/
    HI_BOOL                                 bMetadataValid; /**<whether metadata is valid or not.*/ /**<CNcomment: metadata????*/
    HI_BOOL                                 bCompatible;    /**<whether Dolby frame is compatible or not.*/ /**<CNcomment: Dolby?????*/
}HI_DRV_VIDEO_DOLBY_INFO_S;

typedef enum hiDRV_GAMMA_TYPE_E
{
    HI_DRV_GAMMA_TYPE_1886 = 0,
    HI_DRV_GAMMA_TYPE_2084,

    HI_DRV_GAMMA_TYPE_BUTT
} HI_DRV_GAMMA_TYPE_E;

typedef enum hiDRV_BT2020_LUMINACE_SYS_TYPE_E
{
    HI_DRV_BT2020_NON_CONSTANCE_LUMINACE_SYS = 0,
    HI_DRV_BT2020_CONSTANCE_LUMINACE_SYS,

    HI_DRV_BT2020_LUMINACE_SYS_BUTT
} HI_DRV_BT2020_LUMINACE_SYS_TYPE_E;

typedef struct
{
    HI_U8 u8FullRangeFlag;
    HI_U8 u8ColourPrimaries;
    HI_U8 u8MatrixCoeffs;
} HI_DRV_COLOUR_DESCRIPTION_INFO_S;

typedef struct
{
    HI_U16 u16DisplayPrimaries_x[3];
    HI_U16 u16DisplayPrimaries_y[3];
    HI_U16 u16WhitePoint_x;
    HI_U16 u16WhitePoint_y;
    HI_U32 u32MaxDisplayMasteringLuminance;
    HI_U32 u32MinDisplayMasteringLuminance;
} HI_DRV_MASTERING_DISPLAY_COLOUR_VOLUME_S;

typedef struct
{
    HI_U32 u32MaxContentLightLevel;
    HI_U32 u32MaxPicAverageLightLevel;
} HI_DRV_CONTENT_LIGHT_LEVEL_INFO_S;

typedef struct hiDRV_VIDEO_HDR10_INFO_S
{
    HI_U8  u8MasteringAvailable;
    HI_U8  u8ContentAvailable;
    HI_U8  u8TransferCharacteristics;
    HI_DRV_MASTERING_DISPLAY_COLOUR_VOLUME_S  stMasteringInfo;
    HI_DRV_CONTENT_LIGHT_LEVEL_INFO_S stContentInfo;
} HI_DRV_VIDEO_HDR10_INFO_S;

typedef struct hiDRV_VIDEO_HLG_INFO_S
{
    //HI_DRV_COLOUR_DESCRIPTION_INFO_S stColourInfo;
    HI_U8 IsBackwardsCompatible;
} HI_DRV_VIDEO_HLG_INFO_S;

typedef struct
{
    HI_U8 u8YUVRange;        //Informs on the range of the YUV input signal
    HI_U8 u8HDRContainerCS;  //Gives the color space of the original HDR video container
    HI_U8 u8HDRContentCS;    //Informs on the content HDR YUV color space
    HI_U8 u8LDRContentCS;    //Informs on the content LDR YUV color space
    HI_U32 u32PeakLuma;       //Peak luminance of the sequence, in NITs.
    HI_S32 s32OCT[7];         //Signed array of seven integers, depending on COLOR_CONTAINER parameter
} HI_DRV_TECHNICOLOR_HDR_SEQ_PARAMS; //"static" parameters

typedef struct
{
    HI_U32 u32BA; //Used to compute the content of the P_LUT, once per picture
} HI_DRV_TECHNICOLOR_HDR_FRM_PARAMS; //"danamic" parameters

typedef struct hiDRV_VIDEO_TECHNICOLOR_INFO_S
{
    HI_DRV_TECHNICOLOR_HDR_SEQ_PARAMS stHDR_StaticParms;
    HI_DRV_TECHNICOLOR_HDR_FRM_PARAMS stHDR_DanamicParms;
}HI_DRV_VIDEO_TECHNICOLOR_INFO_S;

typedef struct hiDRV_VIDEO_SLF_INFO_S
{
    HI_U8  u8MasteringAvailable;
    HI_U8  u8ContentAvailable;
    HI_U8  u8TransferCharacteristics;
    HI_DRV_MASTERING_DISPLAY_COLOUR_VOLUME_S  stMasteringInfo;
    HI_DRV_CONTENT_LIGHT_LEVEL_INFO_S stContentInfo;
}HI_DRV_VIDEO_SLF_INFO_S;

typedef enum
{
    HI_DRV_STD_START_RESERVED = 0,
    HI_DRV_STD_HEVC,
    HI_DRV_STD_VP9,
    HI_DRV_STD_AVS2,
    HI_DRV_STD_END_RESERVED
}HI_DRV_VIDEO_STD_E;

typedef enum hiDRV_VIDEO_FRAME_TYPE_E
{
    HI_DRV_VIDEO_FRAME_TYPE_SDR = 0,      /*Normal SDR frame.*/
    HI_DRV_VIDEO_FRAME_TYPE_DOLBY_BL,     /*Dolby BL frame.*/
    HI_DRV_VIDEO_FRAME_TYPE_DOLBY_EL,     /*Dolby EL frame.*/
    HI_DRV_VIDEO_FRAME_TYPE_HDR10,        /*HDR10 frame.*/
    HI_DRV_VIDEO_FRAME_TYPE_HLG,          /*HLG frame.*/
    HI_DRV_VIDEO_FRAME_TYPE_SLF,          /*SLF frame.*/
    HI_DRV_VIDEO_FRAME_TYPE_TECHNICOLOR,  /*TECHNICOLOR frame.*/
    HI_DRV_VIDEO_FRAME_TYPE_BUTT
}HI_DRV_VIDEO_FRAME_TYPE_E;

typedef struct hiDRV_VIDEO_FRAME_S
{
    HI_DRV_VIDEO_FRAME_TYPE_E   enSrcFrameType;         /**<Source frame type:SDR/Dolby(BL/EL)/HDR10.*/
    HI_U32                      u32FrameIndex;          //frame index

    /* stBufAddr[1] is right eye for stereo video */
    HI_DRV_VID_FRAME_ADDR_S     stBufAddr[HI_DRV_BUF_ADDR_MAX];
    HI_DRV_VID_FRAME_ADDR_S     stBufAddr_LB[HI_DRV_BUF_ADDR_MAX];

    /* tunnel attributes */
	HI_U64                      u64TunnelVirAddr;
    HI_U32                      u32TunnelPhyAddr;
    HI_HANDLE                   hTunnelSrc;

    HI_U32                      u32Width;
    HI_U32                      u32Height;

    HI_U32                      u32SrcPts;  /* original pts from the stream */
    HI_U32                      u32Pts;     /* pts after adjust */
    HI_S64                      s64OmxPts;  /* pts for OMX */

    HI_U32                      u32AspectWidth;
    HI_U32                      u32AspectHeight;

    HI_U32                      u32FrameRate;     /* in 1/100 Hz, 0 means unknown */
    HI_U32                      u32OriFrameRate;

    HI_DRV_PIX_FORMAT_E         ePixFormat;     // pixel format of this frame
    HI_BOOL                     bProgressive;   // p or i
    HI_DRV_FIELD_MODE_E         enFieldMode;
    HI_DRV_FIELD_MODE_E         enBufValidMode;
    HI_BOOL                     bTopFieldFirst;

    HI_BOOL                     bCompressd;
    HI_DRV_PIXEL_BITWIDTH_E     enBitWidth;

    //display region in rectangle (x,y,w,h)
    HI_RECT_S                   stDispRect;

    HI_DRV_FRAME_TYPE_E         eFrmType;   //3D type of this frame

    HI_U32                      u32Circumrotate;

    HI_BOOL                     bToFlip_H;
    HI_BOOL                     bToFlip_V;

    HI_U32                      u32ErrorLevel;

    // store hisilicon private data, the store struct is HI_DRV_VIDEO_PRIVATE_S
    HI_U32                      u32Priv[DEF_HI_DRV_FRAME_INFO_SIZE];

    HI_BOOL                     bIsFirstIFrame;
    HI_BOOL                     bStillFrame;

    HI_BOOL                     bSecure;

    HI_DRV_VIDEO_TB_ADJUST_E    enTBAdjust;
    HI_RECT_S                   stLbxInfo;

    HI_DRV_VIDEO_STD_E          enCodecType;

    HI_DRV_COLOUR_DESCRIPTION_INFO_S stColourInfo;

    union
    {
        HI_DRV_VIDEO_DOLBY_INFO_S     stDolbyInfo;          /*Dolby frame info.*/
        HI_DRV_VIDEO_HDR10_INFO_S     stHDR10Info;          /*HDR10 frame info.*/
        HI_DRV_VIDEO_HLG_INFO_S       stHLGInfo;            /*HLG frame info.*/
        HI_DRV_VIDEO_SLF_INFO_S       stSLFInfo;            /*SLF frame info.*/
        HI_DRV_VIDEO_TECHNICOLOR_INFO_S stTechnicolorInfo;  /*Technicolor frame info.*/
    } unHDRInfo;

    HI_DRV_LOWDELAY_STAT_INFO_S stLowdelayStat;
}HI_DRV_VIDEO_FRAME_S;

/* video frame or field */
typedef enum hiDRV_PICTURE_TYPE_E
{
    HI_DRV_PICTURE_FIELD = 0,
    HI_DRV_PICTURE_FRAME,
    HI_DRV_PICTURE_BUTT
}HI_DRV_PICTURE_TYPE_E;

typedef enum hiDRV_SAMPLE_TYPE_E
{
    HI_DRV_SAMPLE_TYPE_UNKNOWN,                  /**<Unknown*/ /**<CNcomment: 未知采样方式*/
    HI_DRV_SAMPLE_TYPE_PROGRESSIVE,              /**<Progressive*/ /**<CNcomment: 采样方式为逐行*/
    HI_DRV_SAMPLE_TYPE_INTERLACE,                /**<Interlaced*/ /**<CNcomment: 采样方式为隔行*/
    HI_DRV_SAMPLE_TYPE_BUTT
}HI_DRV_SAMPLE_TYPE_E;

/* DNR frame info */
typedef struct hiDRV_DNR_INFO_S
{
    HI_BOOL bValid;                      /*DNR??????*/
    HI_U32  u32ImageAvgQP;               /* ???? QP */
    HI_U32  u32VdhInfoAddr;              /* dbinfo ?? */
    HI_U32  u32Size;                     /* dbinfo ?? */
    HI_U32  u32VdhInfoStride;            /* ??stride, ?????(????+7)/8 */
}HI_DRV_DNR_INFO_S;
/*private struct*/
typedef struct hiDRV_VIDEO_PRIVATE_S
{
    HI_DRV_VIDEO_ORIGINAL_INFO_S  stVideoOriginalInfo;  //Video original info
    HI_BOOL                 bValid;
    HI_U32                  u32LastFlag;        /*最后一帧标记*/
    HI_DRV_COLOR_SPACE_E    eColorSpace;        //Current Frame ColorSpace, when DTV VPSS will set this, when Atv Vicap will set this
    HI_U32                  u32BufferID;        //Buffer ID
    HI_U32                  u32FrmCnt;
    HI_U32                  u32PlayTime;
    HI_U32                  u32Fidelity;
    HI_DRV_FIELD_MODE_E     eOriginField;       /*?DEI???,????????????*/
    HI_RECT_S stOriginImageRect;
    HI_U32                  u32PrivDispTime;    //This displaytime is for pvr smooth tplay
    HI_DRV_PICTURE_TYPE_E      ePictureMode;
    HI_DRV_SAMPLE_TYPE_E       eSampleType;
    HI_U32                  u32PrivBufPhyAddr;
    HI_U32                  u32MetaDataFd;
    HI_U32                  u32MetaDataOffset;
    HI_U32                  u32MetaDataSize;
    HI_BOOL                 bByPassVpss;
    HI_BOOL                 bAspectProcessed;
    HI_RECT_S               stWBC2SDDispRect;
    HI_BOOL                 bForFenceUse;
    HI_DRV_DNR_INFO_S       stDNRInfo;
    HI_U32                  u32Reserve[32];     /* Reserve Bytes *
                                                 * !!NOTICE!! this struct is over (64*4)bytes,
                                                 * pay attention when deliver it to user space.
                                                 */
}HI_DRV_VIDEO_PRIVATE_S;

#define DEF_HI_DRV_FRAME_PACKAGE_MAX_FRAME_NUMBER 3
//add by l00225186
/*vdec frame include port handle*/
typedef struct hiDRV_VDEC_FRAME_S
{
    HI_HANDLE hport;
    HI_DRV_VIDEO_FRAME_S stFrameVideo;
}HI_DRV_VDEC_FRAME_S;
/* video frame package */
typedef struct hiDRV_VIDEO_FRAME_PACKAGE_S
{
    HI_U32 u32FrmNum;
    HI_DRV_VDEC_FRAME_S stFrame[DEF_HI_DRV_FRAME_PACKAGE_MAX_FRAME_NUMBER];
}HI_DRV_VIDEO_FRAME_PACKAGE_S;

#define DEF_HI_DRV_VIDEO_BUFFER_MAX_NUMBER 16

/* extern frame buffer setting for virtual window,
   user alloc memory and set this setting to virtual window.*/
typedef struct hiDRV_VIDEO_BUFFER_POOL_S
{
    HI_S32 s32BufNum;
    HI_S32 s32BufStride;
    HI_S32 s32BufHeight;
    HI_U32 u32PhyAddr[DEF_HI_DRV_VIDEO_BUFFER_MAX_NUMBER];
}HI_DRV_VIDEO_BUFFER_POOL_S;

/* window private information, it could be calllback to win-sourece */
typedef struct hiDRV_WIN_PRIV_INFO_S
{
    HI_DRV_PIX_FORMAT_E ePixFmt;

    HI_BOOL   bUseCropRect;
    HI_RECT_S stInRect;  /* (0,0,0,0) means full imgae, not clip */
    HI_DRV_CROP_RECT_S stCropRect;
    HI_RECT_S stOutRect;

    /* may change when window lives */
    HI_DRV_ASPECT_RATIO_S stCustmAR;
    HI_DRV_ASP_RAT_MODE_E enARCvrs;

    /* external buffer config */
    HI_BOOL bUseExtBuf;
    HI_DRV_VIDEO_BUFFER_POOL_S stExtBufPool;

    /* Display Info */
    HI_BOOL   bInterlaced;
    HI_RECT_S stScreen;
    HI_DRV_ASPECT_RATIO_S stScreenAR;
    HI_BOOL  bIn3DMode;
    HI_BOOL bTunnelSupport;
    HI_DRV_ROT_ANGLE_E enRotation;
    HI_BOOL bVertFlip;
    HI_BOOL bHoriFlip;
    HI_BOOL bPassThrough;

    /*Display MaxRate*/
    HI_U32 u32MaxRate;     /* in 1/100 Hz', if 0, full rate */
    HI_BOOL bCompressFlag;     /* in 1/100 Hz', if 0, full rate */
    HI_DRV_PIXEL_BITWIDTH_E  enBitWidth;
}HI_DRV_WIN_PRIV_INFO_S;

/******************************************************************************
|avoid  including each other between hi_drv_hdmi.h and hi_drv_disp.h,
|move HI_DRV_DISP_FMT_E and HI_DRV_DISP_STEREO_E here.ONLY support HDMI2.0 Currently.
*******************************************************************************/

typedef enum hiDRV_DISP_FMT_E
{
    HI_DRV_DISP_FMT_1080P_60 = 0,         /**<1080p 60 Hz*/
    HI_DRV_DISP_FMT_1080P_50,         /**<1080p 50 Hz*/
    HI_DRV_DISP_FMT_1080P_30,         /**<1080p 30 Hz*/
    HI_DRV_DISP_FMT_1080P_25,         /**<1080p 25 Hz*/
    HI_DRV_DISP_FMT_1080P_24,         /**<1080p 24 Hz*/

    HI_DRV_DISP_FMT_1080i_60,         /**<1080i 60 Hz*/
    HI_DRV_DISP_FMT_1080i_50,         /**<1080i 60 Hz*/

    HI_DRV_DISP_FMT_720P_60,          /**<720p 60 Hz*/
    HI_DRV_DISP_FMT_720P_50,          /**<720p 50 Hz */

    HI_DRV_DISP_FMT_576P_50,          /**<576p 50 Hz*/
    HI_DRV_DISP_FMT_480P_60,          /**<480p 60 Hz*/

    HI_DRV_DISP_FMT_PAL,              /* B D G H I PAL */
    HI_DRV_DISP_FMT_PAL_B,            /* B PAL， Australia */
    HI_DRV_DISP_FMT_PAL_B1,           /* B1 PAL, Hungary */
    HI_DRV_DISP_FMT_PAL_D,            /* D PAL, China */
    HI_DRV_DISP_FMT_PAL_D1,           /* D1 PAL， Poland */
    HI_DRV_DISP_FMT_PAL_G,            /* G PAL， Europe */
    HI_DRV_DISP_FMT_PAL_H,            /* H PAL， Europe */
    HI_DRV_DISP_FMT_PAL_K,            /* K PAL， Europe */
    HI_DRV_DISP_FMT_PAL_I,            /* I PAL，U.K. */
    HI_DRV_DISP_FMT_PAL_N,            /* N PAL, Jamaica/Uruguay */
    HI_DRV_DISP_FMT_PAL_Nc,           /* Nc PAL, Argentina:21 */

    HI_DRV_DISP_FMT_PAL_M,            /* M PAL, 525 lines */
    HI_DRV_DISP_FMT_PAL_60,           /* 60 PAL */
    HI_DRV_DISP_FMT_NTSC,             /* (M)NTSC       */
    HI_DRV_DISP_FMT_NTSC_J,           /* NTSC-J        */
    HI_DRV_DISP_FMT_NTSC_443,          /* (M)PAL        */

    HI_DRV_DISP_FMT_SECAM_SIN,      /**< SECAM_SIN*/
    HI_DRV_DISP_FMT_SECAM_COS,      /**< SECAM_COS*/
    HI_DRV_DISP_FMT_SECAM_L,        /**< France*/
    HI_DRV_DISP_FMT_SECAM_B,        /**< Middle East*/
    HI_DRV_DISP_FMT_SECAM_G,        /**< Middle East*/
    HI_DRV_DISP_FMT_SECAM_D,        /**< Eastern Europe*/
    HI_DRV_DISP_FMT_SECAM_K,        /**< Eastern Europe*/
    HI_DRV_DISP_FMT_SECAM_H,        /**< Line SECAM:34*/

    HI_DRV_DISP_FMT_1440x576i_50,
    HI_DRV_DISP_FMT_1440x480i_60, /*sequnce:36*/

    HI_DRV_DISP_FMT_1080P_24_FP,
    HI_DRV_DISP_FMT_720P_60_FP,
    HI_DRV_DISP_FMT_720P_50_FP,

    HI_DRV_DISP_FMT_861D_640X480_60,
    HI_DRV_DISP_FMT_VESA_800X600_60,
    HI_DRV_DISP_FMT_VESA_1024X768_60,
    HI_DRV_DISP_FMT_VESA_1280X720_60,
    HI_DRV_DISP_FMT_VESA_1280X800_60,
    HI_DRV_DISP_FMT_VESA_1280X1024_60,
    HI_DRV_DISP_FMT_VESA_1360X768_60,         //Rowe
    HI_DRV_DISP_FMT_VESA_1366X768_60,
    HI_DRV_DISP_FMT_VESA_1400X1050_60,        //Rowe
    HI_DRV_DISP_FMT_VESA_1440X900_60,
    HI_DRV_DISP_FMT_VESA_1440X900_60_RB,
    HI_DRV_DISP_FMT_VESA_1600X900_60_RB,
    HI_DRV_DISP_FMT_VESA_1600X1200_60,
    HI_DRV_DISP_FMT_VESA_1680X1050_60,       //Rowe
    HI_DRV_DISP_FMT_VESA_1680X1050_60_RB,       //Rowe
    HI_DRV_DISP_FMT_VESA_1920X1080_60,
    HI_DRV_DISP_FMT_VESA_1920X1200_60,
    HI_DRV_DISP_FMT_VESA_1920X1440_60,
    HI_DRV_DISP_FMT_VESA_2048X1152_60,
    HI_DRV_DISP_FMT_VESA_2560X1440_60_RB,
    HI_DRV_DISP_FMT_VESA_2560X1600_60_RB, /*sequence:60*/

    HI_DRV_DISP_FMT_3840X2160_24,/*reserve for extention*/
    HI_DRV_DISP_FMT_3840X2160_25,
    HI_DRV_DISP_FMT_3840X2160_30,
    HI_DRV_DISP_FMT_3840X2160_50,
    HI_DRV_DISP_FMT_3840X2160_60,

    HI_DRV_DISP_FMT_4096X2160_24,/*reserve for extention*/
    HI_DRV_DISP_FMT_4096X2160_25,
    HI_DRV_DISP_FMT_4096X2160_30,
    HI_DRV_DISP_FMT_4096X2160_50,
    HI_DRV_DISP_FMT_4096X2160_60,

    HI_DRV_DISP_FMT_3840X2160_23_976,
    HI_DRV_DISP_FMT_3840X2160_29_97,
    HI_DRV_DISP_FMT_720P_59_94,
    HI_DRV_DISP_FMT_1080P_59_94,
    HI_DRV_DISP_FMT_1080P_29_97,
    HI_DRV_DISP_FMT_1080P_23_976,
    HI_DRV_DISP_FMT_1080i_59_94,

    //ADD
    HI_DRV_DISP_FMT_CUSTOM,
    HI_DRV_DISP_FMT_BUTT
} HI_DRV_DISP_FMT_E;

typedef enum hiDISP_STEREO_E
{
    DISP_STEREO_NONE = 0,
    DISP_STEREO_FPK,
    DISP_STEREO_SBS_HALF,
    DISP_STEREO_TAB,
    DISP_STEREO_FIELD_ALTE,
    DISP_STEREO_LINE_ALTE,
    DISP_STEREO_SBS_FULL,
    DISP_STEREO_L_DEPT,
    DISP_STEREO_L_DEPT_G_DEPT,
    DISP_STEREO_BUTT
} HI_DRV_DISP_STEREO_E;


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_VIDEO_H__ */



