
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
//#include "hi_common.h"
#include "hi_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

typedef enum hiDRV_PIX_FORMAT_E
{
    /* RGB formats */
    HI_DRV_PIX_FMT_RGB332 = 0, /*  8  RGB-3-3-2     */
    HI_DRV_PIX_FMT_RGB444  ,   /* 16  xxxxrrrr ggggbbbb */
    HI_DRV_PIX_FMT_RGB555  ,   /* 16  RGB-5-5-5     */
    HI_DRV_PIX_FMT_RGB565  ,   /* 16  RGB-5-6-5     */
    HI_DRV_PIX_FMT_RGB555X ,   /* 16  RGB-5-5-5 BE  */
    HI_DRV_PIX_FMT_RGB565X ,   /* 16  RGB-5-6-5 BE  */
    HI_DRV_PIX_FMT_BGR666  ,   /* 18  BGR-6-6-6	  */
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
    HI_U8 u32ARw;
    HI_U8 u32ARh;
}HI_DRV_ASPECT_RATIO_S;

/* video frame buffer physical address */
typedef struct hiDRV_VID_FRAME_ADDR_S
{
    /* Y address*/
    HI_U32  u32PhyAddr_YHead; /* only for compress format */
    HI_U32  u32PhyAddr_Y;
    HI_U32  u32Stride_Y;

    /* C OR Cb address*/
    HI_U32  u32PhyAddr_CHead;
    HI_U32  u32PhyAddr_C;
    HI_U32  u32Stride_C;

    /* Cr address*/
    HI_U32  u32PhyAddr_CrHead;
    HI_U32  u32PhyAddr_Cr;
    HI_U32  u32Stride_Cr;
}HI_DRV_VID_FRAME_ADDR_S;

typedef enum hiDRV_BUF_ADDR_E
{
    HI_DRV_BUF_ADDR_LEFT  = 0,
    HI_DRV_BUF_ADDR_RIGHT = 1,  /* only for right eye frame of 3D video */
    HI_DRV_BUF_ADDR_MAX
}HI_DRV_BUF_ADDR_E;

#define DEF_HI_DRV_FRAME_INFO_SIZE 128
#if 0
typedef struct hiDRV_VIDEO_PRIV_INFO_S
{
    HI_U32 u32FrameIndex;  //有递增要求
    HI_U32 u32BufferID;

    //HI_U32 u32Is1D;
    //HI_U32 u32IsCompress;

    HI_U32 u32SpecStrmFlag;
    HI_U32 u32Playtime;

    HI_DRV_COLOR_SPACE_E eColorSpace; /* 'HI_DRV_CS_UNKNOWN' means unknown */
    HI_DRV_FIELD_MODE_E  eOriginField;
    HI_RECT_S stOriginImageRect;  /* orgin image rectagle without letterbox */

    HI_U32 u32VC1RangeInfo[16];
    HI_U32 u32PrivInfo[16];
}DRV_VIDEO_PRIV_INFO_S;

/* video frame info */
typedef struct hiDRV_VIDEO_FRAME_S
{
    //HI_DRV_VID_INFO_S  stFrmInfo;
    HI_DRV_FRAME_TYPE_E  eFrmType;
    HI_DRV_PIX_FORMAT_E  ePixFormat;
   
    HI_BOOL bInterlaced;
    HI_BOOL bTopFirst;

    HI_U32  u32Width;
    HI_U32  u32Height;

    //display region in rectangle (x,y,w,h)
    HI_RECT_S stDispRect;

    //maybe use
/*
    HI_DRV_ROT_ANGLE_E eRotAngle;
    HI_BOOL bToFlip_H;
    HI_BOOL bToFlip_V;
*/
    HI_DRV_ASPECT_RATIO_S stDispAR;
    //HI_DRV_ASPECT_RATIO_S stSampAR;  /* reserved, must be 0 */

    HI_U32  u32FrameRate;     /* in 1/100 Hz, 0 means unknown */
    HI_DRV_COLOR_SPACE_E eColorSpace; /* 'HI_DRV_CS_UNKNOWN' means unknown */

    /* these member may be changed per frame */
    HI_U32 u32FrmCnt;
    HI_U32 u32SrcPts;  /* 0xffffffff means unknown */
    HI_U32 u32Pts;     /* 0xffffffff means unknown */

    HI_U32  u32PlayTime;  /* 0 means ignore, do not process */
    HI_BOOL bToRelease; /* frame should be released when displayed */

    /* stBufAddr[1] is right eye for stereo video */
    HI_DRV_VID_FRAME_ADDR_S stBufAddr[HI_DRV_BUF_ADDR_MAX];  

    HI_DRV_FIELD_MODE_E  eField;

    HI_DRV_FIELD_MODE_E  eOriginField;
    HI_RECT_S stOriginImageRect;  /* orgin image rectagle without letterbox */

    HI_U32 u32ErrorLevel;
    
    HI_U32 u32Priv[DEF_HI_DRV_FRAME_INFO_SIZE];  /* must be 0 */
}HI_DRV_VIDEO_FRAME_S;
#endif

/* define of how to adjust the TB match */
typedef enum hiDRV_VIDEO_TB_ADJUST_E
{
    HI_DRV_VIDEO_TB_PLAY = 0,
    HI_DRV_VIDEO_TB_REPEAT,
    HI_DRV_VIDEO_TB_DISCARD,
    HI_DRV_VIDEO_TB_BUTT
}HI_DRV_VIDEO_TB_ADJUST_E;


typedef struct hiDRV_VIDEO_FRAME_S
{    
    HI_U32 u32FrameIndex; //????????????,????????
	/* stBufAddr[1] is right eye for stereo video */
    HI_DRV_VID_FRAME_ADDR_S stBufAddr[HI_DRV_BUF_ADDR_MAX];  
	
    HI_U32 u32Width;
    HI_U32 u32Height;
		
    HI_U32 u32SrcPts;  /* 0xffffffff means unknown */
    HI_U32 u32Pts;     /* 0xffffffff means unknown */
		 
    HI_U32 u32AspectWidth;    
    HI_U32 u32AspectHeight; 
	
    HI_U32  u32FrameRate;     /* in 1/100 Hz, 0 means unknown */
	
    HI_DRV_PIX_FORMAT_E  ePixFormat;          //??????,???????
    HI_BOOL bProgressive;                     //?????
	HI_DRV_FIELD_MODE_E  enFieldMode;         //???????????,????????????????????
    HI_BOOL bTopFieldFirst;

    //display region in rectangle (x,y,w,h)
    HI_RECT_S stDispRect;					//????

    HI_DRV_FRAME_TYPE_E  eFrmType;          //2D or 3D ???
	
    HI_U32 u32Circumrotate;    //???
	
    HI_BOOL bToFlip_H;						//??????
    HI_BOOL bToFlip_V;						//??????
	/*
    HI_DRV_ROT_ANGLE_E eRotAngle;
	*/
    HI_U32 u32ErrorLevel;                   //???????
	HI_U32 u32Priv[DEF_HI_DRV_FRAME_INFO_SIZE];  /* must be 0 */ //??????????,????
		/***********above as unf***************/

    HI_BOOL bIsFirstIFrame;

    HI_DRV_VIDEO_TB_ADJUST_E    enTBAdjust;

		
		/***********below should be deleted************/
    //maybe use

	//HI_DRV_ASPECT_RATIO_S stDispAR;
	//HI_BOOL bInterlaced;

    //HI_DRV_COLOR_SPACE_E eColorSpace; /* 'HI_DRV_CS_UNKNOWN' means unknown */

    /* these member may be changed per frame */
    //HI_U32 u32FrmCnt;

    //HI_U32  u32PlayTime;  /* 0 means ignore, do not process */
    //HI_BOOL bToRelease; /* frame should be released when displayed */


    //HI_DRV_FIELD_MODE_E  eOriginField;
    //HI_RECT_S stOriginImageRect;  /* orgin image rectagle without letterbox */
}HI_DRV_VIDEO_FRAME_S;


/*private struct*/
typedef struct hiDRV_VIDEO_PRIVATE_S
{
	HI_BOOL bValid;
	HI_U32 u32LastFlag;                  /*最后一帧标记*/
	HI_DRV_COLOR_SPACE_E eColorSpace;    //?????????
	HI_U32 u32BufferID;                    //?????????????ID
	HI_U32 u32FrmCnt;
	HI_U32 u32PlayTime;                /* ????*/
	HI_U32 u32Fidelity;
	HI_DRV_FIELD_MODE_E  eOriginField;   /*?DEI???,????????????*/
    HI_RECT_S stOriginImageRect;         /* ?LBOX???,??LBOX?????? */
	HI_U32 u32Reserve[32]; 				//??????????????????,?VC1????
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
    
    /*Display MaxRate*/
    HI_U32 u32MaxRate;     /* in 1/100 Hz', if 0, full rate */
}HI_DRV_WIN_PRIV_INFO_S;



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_VIDEO_H__ */



