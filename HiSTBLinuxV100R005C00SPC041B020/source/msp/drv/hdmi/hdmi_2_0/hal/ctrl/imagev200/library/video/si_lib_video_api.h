/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_lib_video.h
*
* @brief supporting library functions for TPG
*
*****************************************************************************/
#ifndef __SI_LIB_VIDEO_H__
#define __SI_LIB_VIDEO_H__

/***** #include statements ***************************************************/
#include "si_datatypes.h"

/***** public macro definitions **********************************************/

/*****************************************************************************/
/** @defgroup SII_LIB_VIDEO_MOD3D 3D Modes
* @brief 3D Modes through SiiLibVideoMod3D_t
*
* @see SiiLibVideoMod3D_t
*
******************************************************************************/
/* @{ */
#define SII_LIB_VIDEO_MOD3D__NONE             ((SiiLibVideoMod3D_t)0x0000) //!< 2D mode
#define SII_LIB_VIDEO_MOD3D__FP               ((SiiLibVideoMod3D_t)0x0001) //!< Frame Packing.      P/I Vertical ribbon: prog: L, R (one frame); interl: L_odd, R_odd, L_even, R_even (one frame)
#define SII_LIB_VIDEO_MOD3D__FA               ((SiiLibVideoMod3D_t)0x0002) //!< Field Alternative.  -/I Vertical ribbon: L_odd, R_odd, L_even, R_even (two frames)
#define SII_LIB_VIDEO_MOD3D__TB               ((SiiLibVideoMod3D_t)0x0004) //!< Top and Bottom.     P/I Vertical ribbon: L_odd, R_odd, L_even, R_even (two frames)
#define SII_LIB_VIDEO_MOD3D__SBSF             ((SiiLibVideoMod3D_t)0x0008) //!< Side By Side Full.  P/I Horizontal ribbon: L, R (one frame)
#define SII_LIB_VIDEO_MOD3D__LA               ((SiiLibVideoMod3D_t)0x0010) //!< Line Alternative.   P/- Line intermittent: 1L, 1R, 2L, 2R, ... (one frame)
#define SII_LIB_VIDEO_MOD3D__LD               ((SiiLibVideoMod3D_t)0x0020) //!< Left Plus Depth.    P/- Vertical ribbon: L, depth (one frame)
#define SII_LIB_VIDEO_MOD3D__LDG              ((SiiLibVideoMod3D_t)0x0040) //!< Left Plus Graphics. P/- Vertical ribbon: L, depth, Graphics, Graphics Minus Depth (one frame)
#define SII_LIB_VIDEO_MOD3D__SBSH_HORIZ_OL_OR ((SiiLibVideoMod3D_t)0x0100) //!< Side By Side Half.  P/I Horizontal sub-sampling: Odd L, Odd R
#define SII_LIB_VIDEO_MOD3D__SBSH_HORIZ_OL_ER ((SiiLibVideoMod3D_t)0x0200) //!< Side By Side Half.  P/I Horizontal sub-sampling: Odd L, Even R
#define SII_LIB_VIDEO_MOD3D__SBSH_HORIZ_EL_OR ((SiiLibVideoMod3D_t)0x0400) //!< Side By Side Half.  P/I Horizontal sub-sampling: Even L, Odd R
#define SII_LIB_VIDEO_MOD3D__SBSH_HORIZ_EL_ER ((SiiLibVideoMod3D_t)0x0800) //!< Side By Side Half.  P/I Horizontal sub-sampling: Even L, Even R
#define SII_LIB_VIDEO_MOD3D__SBSH_QUINX_OL_OR ((SiiLibVideoMod3D_t)0x1000) //!< Side By Side Half.  P/I Quincunx matrix: Odd L, Odd R
#define SII_LIB_VIDEO_MOD3D__SBSH_QUINX_OL_ER ((SiiLibVideoMod3D_t)0x2000) //!< Side By Side Half.  P/I Quincunx matrix: Odd L, Even R
#define SII_LIB_VIDEO_MOD3D__SBSH_QUINX_EL_OR ((SiiLibVideoMod3D_t)0x4000) //!< Side By Side Half.  P/I Quincunx matrix: Even L, Odd R
#define SII_LIB_VIDEO_MOD3D__SBSH_QUINX_EL_ER ((SiiLibVideoMod3D_t)0x8000) //!< Side By Side Half.  P/I Quincunx matrix: Even L, Even R
/* @} */

#define SII_LIB_VIDEO_ASPRAT__125             ((SiiLibVideoAspRat_t)(1.25))
#define SII_LIB_VIDEO_ASPRAT__133             ((SiiLibVideoAspRat_t)(4.0/3.0))
#define SII_LIB_VIDEO_ASPRAT__155             ((SiiLibVideoAspRat_t)(1.55))        /* 14.0/9.0 */
#define SII_LIB_VIDEO_ASPRAT__166             ((SiiLibVideoAspRat_t)(1.66))        /* 15.0/9.0 */
#define SII_LIB_VIDEO_ASPRAT__178             ((SiiLibVideoAspRat_t)(16.0/9.0))
#define SII_LIB_VIDEO_ASPRAT__185             ((SiiLibVideoAspRat_t)(1.85))
#define SII_LIB_VIDEO_ASPRAT__235             ((SiiLibVideoAspRat_t)(2.35))

/***** public type definitions ***********************************************/

typedef uint16_t SiiLibVideoPix_t;      //!< Pixel or line count
typedef uint32_t SiiLibVideoFrq_t;      //!< Pixel frequency in Hz
typedef uint16_t SiiLibVideoMod3D_t;    //!< 3D video mode (see SII_LIB_VIDEO_MOD3D definitions)
typedef float    SiiLibVideoAspRat_t;

/**
* @brief Video format (resolution)
*/
typedef enum
{
	SII_LIB_VIDEO_VM__VGA60           , //!<  0 = vm1_640x480p,
	SII_LIB_VIDEO_VM__480P            , //!<  1 = vm2_3_720x480p,
	SII_LIB_VIDEO_VM__720P60          , //!<  2 = vm4_1280x720p,
	SII_LIB_VIDEO_VM__1080I60         , //!<  3 = vm5_1920x1080i,
	SII_LIB_VIDEO_VM__480I2           , //!<  4 = vm6_7_720_1440x480i,
	SII_LIB_VIDEO_VM__240P2A          , //!<  5 = vm8_9_720_1440x240p_1,
	SII_LIB_VIDEO_VM__240P2B          , //!<  6 = vm8_9_720_1440x240p_2,
	SII_LIB_VIDEO_VM__480I4           , //!<  7 = vm10_11_2880x480i,
	SII_LIB_VIDEO_VM__240P4A          , //!<  8 = vm12_13_2880x240p_1,
	SII_LIB_VIDEO_VM__240P4B          , //!<  9 = vm12_13_2880x240p_2,
	SII_LIB_VIDEO_VM__480P2           , //!< 10 = vm14_15_1440x480p,
	SII_LIB_VIDEO_VM__1080P60         , //!< 11 = vm16_1920x1080p,
	SII_LIB_VIDEO_VM__576P            , //!< 12 = vm17_18_720x576p,
	SII_LIB_VIDEO_VM__720P50          , //!< 13 = vm19_1280x720p,
	SII_LIB_VIDEO_VM__1080I50         , //!< 14 = vm20_1920x1080i,
	SII_LIB_VIDEO_VM__576I2           , //!< 15 = vm21_22_720_1440x576i,
	SII_LIB_VIDEO_VM__288P2A          , //!< 16 = vm23_24_720_1440x288p_1,
	SII_LIB_VIDEO_VM__288P2B          , //!< 17 = vm23_24_720_1440x288p_2,
	SII_LIB_VIDEO_VM__288P2C          , //!< 18 = vm23_24_720_1440x288p_3,
	SII_LIB_VIDEO_VM__576I4           , //!< 19 = vm25_26_2880x576i,
	SII_LIB_VIDEO_VM__288P4A          , //!< 20 = vm27_28_2880x288p_1,
	SII_LIB_VIDEO_VM__288P4B          , //!< 21 = vm27_28_2880x288p_2,
	SII_LIB_VIDEO_VM__288P4C          , //!< 22 = vm27_28_2880x288p_3,
	SII_LIB_VIDEO_VM__576P2           , //!< 23 = vm29_30_1440x576p,
	SII_LIB_VIDEO_VM__1080P50         , //!< 24 = vm31_1920x1080p,
	SII_LIB_VIDEO_VM__1080P24         , //!< 25 = vm32_1920x1080p,
	SII_LIB_VIDEO_VM__1080P25         , //!< 26 = vm33_1920x1080p,
	SII_LIB_VIDEO_VM__1080P30         , //!< 27 = vm34_1920x1080p,
	SII_LIB_VIDEO_VM__480P4           , //!< 28 = vm35_36_2880x480p,
	SII_LIB_VIDEO_VM__576P4           , //!< 29 = vm37_38_2880x576p,
	SII_LIB_VIDEO_VM__1080I50A        , //!< 30 = vm39_1920x1080i_1250_total,
	SII_LIB_VIDEO_VM__1080I100        , //!< 31 = vm40_1920x1080i,
	SII_LIB_VIDEO_VM__720P100         , //!< 32 = vm41_1280x720p,
	SII_LIB_VIDEO_VM__576P100         , //!< 33 = vm42_43_720x576p,
	SII_LIB_VIDEO_VM__576I100         , //!< 34 = vm44_45_720_1440x576i,
	SII_LIB_VIDEO_VM__1080I120        , //!< 35 = vm46_1920x1080i,
	SII_LIB_VIDEO_VM__720P120         , //!< 36 = vm47_1280x720p,
	SII_LIB_VIDEO_VM__480P120         , //!< 37 = vm48_49_720x480p,
	SII_LIB_VIDEO_VM__480I120         , //!< 38 = vm50_51_720_1440x480i,
	SII_LIB_VIDEO_VM__576P200         , //!< 39 = vm52_53_720x576p,
	SII_LIB_VIDEO_VM__576I200         , //!< 40 = vm54_55_720_1440x576i,
	SII_LIB_VIDEO_VM__480P240         , //!< 41 = vm56_57_720x480p,
	SII_LIB_VIDEO_VM__480I240         , //!< 42 = vm58_59_720_1440x480i,
	SII_LIB_VIDEO_VM__720P24          , //!< 43 = vm60_1280x720p,
	SII_LIB_VIDEO_VM__720P25          , //!< 44 = vm61_1280x720p,
	SII_LIB_VIDEO_VM__720P30          , //!< 45 = vm62_1280x720p,
	SII_LIB_VIDEO_VM__1080P120        , //!< 46 = vm63_1920x1080p,
	SII_LIB_VIDEO_VM__1080P100        , //!< 47 = vm64_1920x1080p,
	SII_LIB_VIDEO_VM__2160P30         , //!< 48 = vm_3840x2160p,
	SII_LIB_VIDEO_VM__2160P25         , //!< 49 = vm_3840x2160p,
	SII_LIB_VIDEO_VM__2160P24         , //!< 50 = vm_3840x2160p,
	SII_LIB_VIDEO_VM__2160P24S        , //!< 51 = vm_4096x2160p,
	SII_LIB_VIDEO_VM__VGA72           , //!< 52 = VM_PC_VGA72,
	SII_LIB_VIDEO_VM__VGA75           , //!< 53 = VM_PC_VGA75,
	SII_LIB_VIDEO_VM__VGA85_1         , //!< 54 = VM_PC_VGA85_1,
	SII_LIB_VIDEO_VM__VGA85_2         , //!< 55 = VM_PC_VGA85_2,
	SII_LIB_VIDEO_VM__SVGA56          , //!< 56 = VM_PC_SVGA56,
	SII_LIB_VIDEO_VM__SVGA60          , //!< 57 = VM_PC_SVGA60,
	SII_LIB_VIDEO_VM__SVGA72          , //!< 58 = VM_PC_SVGA72,
	SII_LIB_VIDEO_VM__SVGA75          , //!< 59 = VM_PC_SVGA75,
	SII_LIB_VIDEO_VM__SVGA85          , //!< 60 = VM_PC_SVGA85,
	SII_LIB_VIDEO_VM__XGAI87          , //!< 61 = VM_PC_XGAI87,
	SII_LIB_VIDEO_VM__XGA60           , //!< 62 = VM_PC_XGA60,
	SII_LIB_VIDEO_VM__XGA70           , //!< 63 = VM_PC_XGA70,
	SII_LIB_VIDEO_VM__XGA75_1         , //!< 64 = VM_PC_XGA75_1,
	SII_LIB_VIDEO_VM__XGA85           , //!< 65 = VM_PC_XGA85,
	SII_LIB_VIDEO_VM__XGA75_2         , //!< 66 = VM_PC_XGA75_2,
	SII_LIB_VIDEO_VM__WXGA60_1        , //!< 67 = VM_PC_WXGA60_1,
	SII_LIB_VIDEO_VM__WXGA60_2        , //!< 68 = VM_PC_WXGA60_2,
	SII_LIB_VIDEO_VM__WXGA75          , //!< 69 = VM_PC_WXGA75,
	SII_LIB_VIDEO_VM__WXGA60_800      , //!< 70 = VM_PC_WXGA60_800,
	SII_LIB_VIDEO_VM__WXGA60_3        , //!< 71 = VM_PC_WXGA60_3,
	SII_LIB_VIDEO_VM__SXGA60          , //!< 72 = VM_PC_SXGA60,
	SII_LIB_VIDEO_VM__WSXGA60_1       , //!< 73 = VM_PC_WSXGA60_1,
	SII_LIB_VIDEO_VM__WSXGA60_2       , //!< 74 = VM_PC_WSXGA60_2,
	SII_LIB_VIDEO_VM__WXGA85_1        , //!< 75 = VM_PC_WXGA85_1,
	SII_LIB_VIDEO_VM__WXGA85_2        , //!< 76 = VM_PC_WXGA85_2,
	SII_LIB_VIDEO_VM__SXGA75          , //!< 77 = VM_PC_SXGA75,
	SII_LIB_VIDEO_VM__SXGA85          , //!< 78 = VM_PC_SXGA85,
	SII_LIB_VIDEO_VM__HD60            , //!< 79 = VM_PC_HD60,
	SII_LIB_VIDEO_VM__WSXGA60         , //!< 80 = VM_PC_WSXGA60,
	SII_LIB_VIDEO_VM__WSXGA75         , //!< 81 = VM_PC_WSXGA75,
	SII_LIB_VIDEO_VM__WSXGA50         , //!< 82 = VM_PC_WSXGA50,
	SII_LIB_VIDEO_VM__HD50_PLUS       , //!< 83 = VM_PC_HD50_PLUS,
	SII_LIB_VIDEO_VM__UXGA60          , //!< 84 = VM_PC_UXGA60,
	SII_LIB_VIDEO_VM__WSXGA50_PLUS    , //!< 85 = VM_PC_WSXGA50_PLUS,
	SII_LIB_VIDEO_VM__WUXGA50         , //!< 86 = VM_PC_WUXGA50,
	SII_LIB_VIDEO_VM__WUXGA60         , //!< 87 = VM_PC_WUXGA60,
	SII_LIB_VIDEO_VM__480I            , //!< 88 = VM_720X480i,
	SII_LIB_VIDEO_VM__576I            , //!< 89 = VM_720X576i,
	SII_LIB_VIDEO_VM__540X60		  , //!< 90 = VM_960X540,

	SII_LIB_VIDEO_VM__2160P50         , //!< 91 = vm_3840x2160p,
	SII_LIB_VIDEO_VM__2160P60         , //!< 92 = vm_3840x2160p,

	SII_LIB_VIDEO_VM__NONE              //!< No Video
} SiiLibVideoVM_t;

/**
* @brief Picture Aspect ratio
*/
typedef enum
{
	SII_LIB_VIDEO_PICTAR__NONE,         //!< Unknown or default
	SII_LIB_VIDEO_PICTAR__4X3,          //!< 4 x 3
	SII_LIB_VIDEO_PICTAR__16X9,         //!< 16 x 9
	SII_LIB_VIDEO_PICTAR__FUTURE,       //!< Reserved
	SII_LIB_VIDEO_PICTAR__SQP,          //!< Square Pixel (number of pixels per line x number of lines)

} SiiLibVideoPictAR_t;

/**
* @brief Video Timing Description
*/
typedef struct
{
	SiiLibVideoPix_t     vTotal;        //!< Total number of lines per video frame
	SiiLibVideoPix_t     vActive;       //!< Number of active lines per video frame
	SiiLibVideoPix_t     vSync;         //!< VSync width in lines
	SiiLibVideoPix_t     vFrontPorch;   //!< V front porch width in lines
	SiiLibVideoPix_t     vBackPorch;    //!< V back porch width in lines

	SiiLibVideoPix_t     hTotal;        //!< Total number of pixels per video line
	SiiLibVideoPix_t     hActive;       //!< Number of active pixels per video line
	SiiLibVideoPix_t     hSync;         //!< HSync width in pixels
	SiiLibVideoPix_t     hFrontPorch;   //!< H front porch width in pixels
	SiiLibVideoPix_t     hBackPorch;    //!< H back porch width in pixels

	SiiLibVideoFrq_t     pixelFreq;     //!< Pixel frequency
	SiiLibVideoFrq_t     lineFreq;      //!< Line (horizontal) frequency
	SiiLibVideoFrq_t     vFreq;         //!< Frame (vertical) frequency
	SiiLibVideoPictAR_t  pictAr;        //!< Picture aspect ratio
	SiiLibVideoMod3D_t   mode3D;        //!< 3d mode

	bit_fld_t            bInterlaced;   //!< true for interlaced format, false for progressive
	bit_fld_t            bVpol;         //!< true for negative V polarity, false for positive
	bit_fld_t            bHpol;         //!< true for negative H polarity, false for positive
} SiiLibVideoTiming_t;

typedef struct
{
	float fR;
	float fG;
	float fB;
} SiiLibVideoRGB_t;

typedef struct
{
	float fY;
	float fCb;
	float fCr;
} SiiLibVideoYUV_t;

/**
* @brief Color Space and Colorimetry
*/
typedef enum
{
	SII_LIB_VIDEO_CLRSPC__NONE,   //!< Unknown
	SII_LIB_VIDEO_CLRSPC__601,    //!< YCbCr ITU-601
	SII_LIB_VIDEO_CLRSPC__709,    //!< YCbCr ITU-709
	SII_LIB_VIDEO_CLRSPC__XV601,  //!< xvYCC ITU-601
	SII_LIB_VIDEO_CLRSPC__XV709,  //!< xvYCC ITU-709
	SII_LIB_VIDEO_CLRSPC__RGB     //!< RGB
} SiiLibVideoClrSpc_t;

/**
* @brief Color Space and Colorimetry
*/
typedef enum
{
	SII_LIB_VIDEO_CHROMASMPL__444, //!< 4:4:4
	SII_LIB_VIDEO_CHROMASMPL__422, //!< 4:2:2
	SII_LIB_VIDEO_CHROMASMPL__420  //!< 4:2:0
} SiiLibVideoChromaSmpl_t;

/***** external functions ****************************************************/

/***** public functions ******************************************************/
void SiiLibVideoFullRgbToLimitedRgb( SiiLibVideoRGB_t* pRgbO, const SiiLibVideoRGB_t* pRgbI );
void SiiLibVideoRgbToYc709( SiiLibVideoYUV_t* pYuvO, const SiiLibVideoRGB_t* pRgbI );
void SiiLibVideoQuantizeYc( SiiLibVideoYUV_t* pYuv, uint8_t bits );
void SiiLibVideoRgbToYc601( SiiLibVideoYUV_t* pYuvO, const SiiLibVideoRGB_t* pRgbI );
void SiiLibVideoRgbToRGB( SiiLibVideoYUV_t* pYuvO, const SiiLibVideoRGB_t* pRgbI );
void SiiLibVideoQuantizeRgb( SiiLibVideoRGB_t* pRgb, uint8_t bits );
void SiiLibVideoTimingDetailGet(SiiLibVideoVM_t vM, SiiLibVideoMod3D_t mode3D, bool_t perField, SiiLibVideoTiming_t* pTiming);

#endif // __SI_LIB_VIDEO_H__

/***** end of file ***********************************************************/
