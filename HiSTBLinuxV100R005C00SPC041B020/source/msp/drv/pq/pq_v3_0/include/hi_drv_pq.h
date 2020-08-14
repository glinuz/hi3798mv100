/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : hi_drv_pq.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/04/01
  Description   :
******************************************************************************/

#ifndef __HI_DRV_PQ_V3_H__
#define __HI_DRV_PQ_V3_H__

#include <linux/ioctl.h>
#include "hi_module.h"

#include "hi_type.h"
#include "hi_debug.h"
#include "drv_pq_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define HI_FATAL_PQ(fmt...) HI_TRACE(HI_LOG_LEVEL_FATAL, HI_ID_PQ, fmt)
#define HI_ERR_PQ(fmt...)   HI_TRACE(HI_LOG_LEVEL_ERROR, HI_ID_PQ, fmt)
#define HI_WARN_PQ(fmt...)  HI_TRACE(HI_LOG_LEVEL_WARNING, HI_ID_PQ, fmt)
#define HI_INFO_PQ(fmt...)  HI_TRACE(HI_LOG_LEVEL_INFO, HI_ID_PQ, fmt)
#define HI_DEBUG_PQ(fmt...) HI_TRACE(HI_LOG_LEVEL_DBG, HI_ID_PQ, fmt)
#else
#define HI_FATAL_PQ(fmt...)
#define HI_ERR_PQ(fmt...)
#define HI_WARN_PQ(fmt...)
#define HI_INFO_PQ(fmt...)
#define HI_DEBUG_PQ(fmt...)
#endif


/* PQ模块 */
typedef enum hiHIPQ_MODULE_E
{
    HI_PQ_MODULE_FMD = 0      ,
    HI_PQ_MODULE_TNR          ,
    HI_PQ_MODULE_SNR          ,
    HI_PQ_MODULE_DB           ,
    HI_PQ_MODULE_DR           ,
    HI_PQ_MODULE_DM           ,
    HI_PQ_MODULE_DBM          ,
    HI_PQ_MODULE_SHARPNESS    ,
    HI_PQ_MODULE_DCI          ,
    HI_PQ_MODULE_COLOR        ,
    HI_PQ_MODULE_SR           ,
    HI_PQ_MODULE_CSC          ,
    HI_PQ_MODULE_ZME          ,
    HI_PQ_MODULE_DEI          ,
    HI_PQ_MODULE_DS           ,
    HI_PQ_MODULE_ARTDS        ,
    HI_PQ_MODULE_GFXCSC       ,
    HI_PQ_MODULE_GFXZME       ,
    HI_PQ_MODULE_ALL          ,

    HI_PQ_MODULE_BUTT
}  HI_PQ_MODULE_E;

/*亮度/对比度/色调/饱和度设定*/
typedef struct hiPQ_IMAGE_PARAM_S
{
    HI_U16 u16Brightness;
    HI_U16 u16Contrast;
    HI_U16 u16Hue;
    HI_U16 u16Saturation;

} HI_PQ_IMAGE_PARAM_S;

/*
 * 模块开关属性
 */
typedef struct hiPQ_MODULE_S
{
    HI_PQ_MODULE_E enModule;
    HI_U32 u32OnOff; /*开关*/
} HI_PQ_MODULE_S;

/*
 * 卖场模式属性
 */
typedef struct hiPQ_DEMO_S
{
    HI_PQ_MODULE_E enModule;
    HI_BOOL      bOnOff; /*开关*/
} HI_PQ_DEMO_S;

/* 卖场模式 显示方式*/
typedef enum hiPQ_DEMO_MODE_E
{
    HI_PQ_DEMO_MODE_FIXED_R = 0,
    HI_PQ_DEMO_MODE_FIXED_L,
    HI_PQ_DEMO_MODE_SCROLL_R,
    HI_PQ_DEMO_MODE_SCROLL_L,

    HI_PQ_DEMO_MODE_BUTT
} HI_PQ_DEMO_MODE_E;

/*
 * 寄存器属性
 */
typedef struct hiPQ_REGISTER_S
{
    HI_U32 u32RegAddr;     /* register addr */
    HI_U8  u8Lsb;          /* register lsb */
    HI_U8  u8Msb;          /* register msb */
    HI_U8  u8SourceMode;   /* video source */
    HI_U8  u8OutputMode;   /* output mode */
    HI_U32 u32Module;      /* module */
    HI_U32 u32Value;       /* register value */
} HI_PQ_REGISTER_S;

/*ACM 寄存器控制参数结构*/
typedef struct hiPQ_COLOR_CTRL_S
{
    HI_U32 u32En;
    HI_U32 u32DbgEn;
    HI_U32 u32Stretch;
    HI_U32 u32Cliprange;
    HI_U32 u32Cliporwrap;
    HI_U32 u32Cbcrthr;
} HI_PQ_COLOR_CTRL_S;

/*ACM table结构*/
typedef struct hiPQ_DRV_ACM_LUT_S
{
    HI_U32 u32LutType;           /*ACM 查找表曲线类型*/
    HI_S16 as16Lut[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];   /*ACM 查找表*/
} HI_PQ_ACM_LUT_S;

/*DCI曲线配置表*/
typedef struct hiPQ_DCI_WGT_S
{
    HI_S16 s16WgtCoef0[33];
    HI_S16 s16WgtCoef1[33];
    HI_S16 s16WgtCoef2[33];
    HI_U16 u16Gain0;
    HI_U16 u16Gain1;
    HI_U16 u16Gain2;
} HI_PQ_DCI_WGT_S;

/*DCI Black Stretch Lut*/
typedef struct hiPQ_DCI_BS_LUT_S
{
    HI_S16 s16BSDelta[320];
} HI_PQ_DCI_BS_LUT_S;

/*DCI直方图结构*/
typedef struct hiPQ_DCI_HISTGRAM_S
{
    HI_S32 s32HistGram[32];
} HI_PQ_DCI_HISTGRAM_S;

typedef struct
{
    HI_U16  u16GammCoef[1024];
} HI_PQ_CSC_GAMM_PARA_S;

/*TNR通用结构*/
typedef struct hiPQ_TNR_S
{
    HI_S32 s32MappingMax;
    HI_S32 s32MappingMin;
    HI_S32 s32MappingR[5];
    HI_S32 s32MappingT[6];
    HI_S32 s32MappingK[4];
} HI_PQ_TNR_S;

/* YFmotion 的映射曲线  */
typedef struct hiPQ_TNR_FMOTION_S
{
    HI_S16 s16YFmotion[32];
    HI_S16 s16CFmotion[32];
} HI_PQ_TNR_FMOTION_S;

/*SNR的pixmean-ratio结构*/
typedef struct hiPQ_SNR_PIXMEAN_2_RATIO_S
{
    HI_U8  u8EdgeMaxRatio;
    HI_U8  u8EdgeMinRatio;
    HI_U8  u8EdgeOriRatio;
    HI_U8  u8Reserve;
    HI_U16 u16EdgeMeanTh[8];
    HI_U8  u8EdgeMeanK[8];
    HI_U16 u16PixMeanRatio[8];
} HI_PQ_SNR_PIXMEAN_2_RATIO_S;

/*SNR 的pixdiff-edgestr结构*/
typedef struct hiPQ_SNR_PIXDIFF_2_EDGESTR_S
{
    HI_U8  u8EdgeOriStrength;
    HI_U8  u8EdgeMaxStrength;
    HI_U8  u8EdgeMinStrength;
    HI_U8  u8Reserve1;
    HI_U16 u16EdgeStrTh[3];
    HI_U16 u16Reserve2;
    HI_U8  u8EdgeStrK[3];
    HI_U8  u8Reserve3;
    HI_U8  u8EdgeStr[3];
    HI_U8  u8Reserve4;
} HI_PQ_SNR_PIXDIFF_2_EDGESTR_S;

typedef struct  hiPQ_SNR_MEAN_RATIO_S
{
    HI_S32 s32MappingMax;    /*meanedgeratio max映射表最大值 0~127 */
    HI_S32 s32MappingMin;    /*meanedgeratio min映射表最小值 0~127 */
    HI_S32 s32MappingX[6];   /*meanedgeratio X*/
    HI_S32 s32MappingY[5];   /*meanedgeratio Y*/
    HI_S32 s32MappingK[4];   /*meanedgeratio K*/

} HI_PQ_SNR_MEAN_RATIO_S;

typedef struct  hiPQ_SNR_EDGE_STR_S
{
    HI_S32 s32MappingMax;    /*edgestrmapping max映射表最大值 0~63*/
    HI_S32 s32MappingMin;    /*edgestrmapping min映射表最小值 0~63*/
    HI_S32 s32MappingX[8];   /*edgestrmapping X*/
    HI_S32 s32MappingY[7];   /*edgestrmapping Y*/
    HI_S32 s32MappingK[6];   /*edgestrmapping K*/

} HI_PQ_SNR_EDGE_STR_S;


/*SR演示模式*/
typedef enum hiPQ_SR_DEMO_E
{
    HI_PQ_SR_DISABLE  = 0,/* 关SR,只ZME */
    HI_PQ_SR_ENABLE_R,    /* 右边SR */
    HI_PQ_SR_ENABLE_L,    /* 左边SR */
    HI_PQ_SR_ENABLE_A,    /* 全屏 */

    HI_PQ_SR_DEMO_BUTT
} HI_PQ_SR_DEMO_E;

typedef struct hiPQ_SIX_BASE_COLOR_S
{
    HI_U32  u32Red;
    HI_U32  u32Green;
    HI_U32  u32Blue;
    HI_U32  u32Cyan;
    HI_U32  u32Magenta;
    HI_U32  u32Yellow;
} HI_PQ_SIX_BASE_COLOR_S;

/*ACM GAIN 消息结构*/
typedef struct hiPQ_COLOR_GAIN_S
{
    HI_U32 u32GainMode;   /* 0:SD;1:HD;2:UHD */
    HI_U32 u32Gainluma;   /* 表示对Hue的增益，Range:0-1023 */
    HI_U32 u32Gainhue;    /* 表示对Hue的增益，Range:0-1023 */
    HI_U32 u32Gainsat;    /* 表示对Luma的增益 */
} HI_PQ_COLOR_GAIN_S;

/*颜色增强类型*/
typedef enum hiPQ_COLOR_ENHANCE_E
{
    HI_PQ_COLOR_ENHANCE_FLESHTONE = 0,    /* 肤色增强 */
    HI_PQ_COLOR_ENHANCE_SIX_BASE,         /* 六基色增强,自定义颜色的增强 */
    HI_PQ_COLOR_ENHANCE_SPEC_COLOR_MODE,  /* 固定模式的颜色增强模式 */
    HI_PQ_COLOR_ENHANCE_BUTT
} HI_PQ_COLOR_ENHANCE_E;

/*六基色增强参数*/
typedef struct  hiPQ_SIX_BASE_S
{
    HI_U32  u32Red;       /* Range:0~100 */
    HI_U32  u32Green;     /* Range:0~100 */
    HI_U32  u32Blue;      /* Range:0~100 */

    HI_U32  u32Cyan;      /* Range:0~100 */
    HI_U32  u32Magenta;   /* Range:0~100 */
    HI_U32  u32Yellow;    /* Range:0~100 */
} HI_PQ_SIX_BASE_S;


/*肤色增益参数*/
typedef enum hiPQ_FLESHTONE_E
{
    HI_PQ_FLESHTONE_GAIN_OFF = 0,
    HI_PQ_FLESHTONE_GAIN_LOW,
    HI_PQ_FLESHTONE_GAIN_MID,
    HI_PQ_FLESHTONE_GAIN_HIGH,

    HI_PQ_FLESHTONE_GAIN_BUTT
}  HI_PQ_FLESHTONE_E;


/*颜色增强类型*/
typedef enum hiPQ_COLOR_SPEC_MODE_E
{
    HI_PQ_COLOR_MODE_RECOMMEND = 0, /* 推荐的颜色增强模式 */
    HI_PQ_COLOR_MODE_BLUE,          /* 固定的蓝色增强模式 */
    HI_PQ_COLOR_MODE_GREEN,         /* 固定的绿色增强模式 */
    HI_PQ_COLOR_MODE_BG,            /* 固定的蓝绿色增强模式 */
    HI_PQ_COLOR_MODE_ORIGINAL,      /* 原始颜色模式 */
    HI_PQ_COLOR_MODE_BUTT
} HI_PQ_COLOR_SPEC_MODE_E;


/*颜色增强参数*/
typedef struct  hiPQ_COLOR_ENHANCE_S
{
    HI_PQ_COLOR_ENHANCE_E    enColorEnhanceType; /* 色彩增强类型 */
    union
    {
        HI_PQ_FLESHTONE_E    enFleshtone;        /* 肤色增强参数 */
        HI_PQ_SIX_BASE_S     stSixBase;          /* 六基色增强参数 */
        HI_PQ_COLOR_SPEC_MODE_E   enColorMode;   /* 固定的颜色增强模式 */

    } unColorGain;
} HI_PQ_COLOR_ENHANCE_S;

typedef struct hiPQ_HDR_TM_LUT_S
{
    HI_S16 as16TMLutI[512];
	HI_S16 as16TMLutS[512];
	HI_S16 as16SMLutI[512];
} HI_PQ_HDR_TM_LUT_S;

typedef enum hiPQ_IMAGE_MODE_E
{
    HI_PQ_IMAGE_MODE_NORMAL = 0,
    HI_PQ_IMAGE_MODE_VIDEOPHONE,
    HI_PQ_IMAGE_MODE_BUTT,
} HI_PQ_IMAGE_MODE_E;


/*用户接口*/
//#define HIIOC_PQ_S_COLORTEMP            _IOW(HI_ID_PQ, 1, HI_PQ_COLOR_TEMP_S)         /* 设置色温参数*/
//#define HIIOC_PQ_G_COLORTEMP            _IOR(HI_ID_PQ, 2, HI_PQ_COLOR_TEMP_S)         /* 获取色温参数*/

#define HIIOC_PQ_S_SD_BRIGHTNESS        _IOW(HI_ID_PQ, 3, HI_U32)                     /* 设置标清BRIGHTNESS level*/
#define HIIOC_PQ_G_SD_BRIGHTNESS        _IOR(HI_ID_PQ, 4, HI_U32)                     /* 获取标清BRIGHTNESS level*/

#define HIIOC_PQ_S_SD_CONTRAST          _IOW(HI_ID_PQ, 5, HI_U32)                     /* 设置标清CONTRAST level*/
#define HIIOC_PQ_G_SD_CONTRAST          _IOR(HI_ID_PQ, 6, HI_U32)                     /* 获取标清CONTRAST level*/

#define HIIOC_PQ_S_SD_SATURATION        _IOW(HI_ID_PQ, 7, HI_U32)                     /* 设置标清SATURATION level*/
#define HIIOC_PQ_G_SD_SATURATION        _IOR(HI_ID_PQ, 8, HI_U32)                     /* 获取标清SATURATION level*/

#define HIIOC_PQ_S_SD_HUE               _IOW(HI_ID_PQ, 9, HI_U32)                     /* 设置标清HUE level*/
#define HIIOC_PQ_G_SD_HUE               _IOR(HI_ID_PQ, 10, HI_U32)                    /* 获取标清HUE level*/

#define HIIOC_PQ_S_TNR                  _IOW(HI_ID_PQ, 11, HI_U32)                    /* 设置TNR level*/
#define HIIOC_PQ_G_TNR                  _IOR(HI_ID_PQ, 12, HI_U32)                    /* 获取TNR level*/

#define HIIOC_PQ_S_SHARPNESS            _IOW(HI_ID_PQ, 13, HI_U32)                    /* 设置SHARPNESS level*/
#define HIIOC_PQ_G_SHARPNESS            _IOR(HI_ID_PQ, 14, HI_U32)                    /* 获取SHARPNESS level*/

#define HIIOC_PQ_S_DB                   _IOW(HI_ID_PQ, 15, HI_U32)                    /* 设置De-blocking level*/
#define HIIOC_PQ_G_DB                   _IOR(HI_ID_PQ, 16, HI_U32)                    /* 获取De-blocking level*/

#define HIIOC_PQ_S_DR                   _IOW(HI_ID_PQ, 17, HI_U32)                    /* 设置De-Ring level*/
#define HIIOC_PQ_G_DR                   _IOR(HI_ID_PQ, 18, HI_U32)                    /* 获取De-Ring level*/

#define HIIOC_PQ_S_COLORGAIN            _IOW(HI_ID_PQ, 19, HI_U32)                    /* 设置Color gain level*/
#define HIIOC_PQ_G_COLORGAIN            _IOR(HI_ID_PQ, 20, HI_U32)                    /* 获取Color gain level*/

#define HIIOC_PQ_S_FLESHTONE            _IOW(HI_ID_PQ, 21, HI_PQ_FLESHTONE_E)         /* 设置FleshTone level*/
#define HIIOC_PQ_G_FLESHTONE            _IOR(HI_ID_PQ, 22, HI_PQ_FLESHTONE_E)         /* 获取FleshTone level*/

#define HIIOC_PQ_S_BACKLIGHT            _IOW(HI_ID_PQ, 23, HI_U32)                    /* 设置BackLight level*/
#define HIIOC_PQ_G_BACKLIGHT            _IOR(HI_ID_PQ, 24, HI_U32)                    /* 获取BackLight level*/

#define HIIOC_PQ_S_GAMMAIDX             _IOW(HI_ID_PQ, 25, HI_S32)                    /* 设置GAMMA index*/
#define HIIOC_PQ_G_GAMMANUM             _IOR(HI_ID_PQ, 26, HI_S32)                    /* 获取GAMMA num*/

#define HIIOC_PQ_S_MODULE               _IOW(HI_ID_PQ, 27, HI_PQ_MODULE_S)            /* 设置各算法模块开关*/
#define HIIOC_PQ_G_MODULE               _IOWR(HI_ID_PQ, 28, HI_PQ_MODULE_S)           /* 获取各算法模块开关*/

#define HIIOC_PQ_S_DEMO                 _IOW(HI_ID_PQ, 29, HI_PQ_DEMO_S)              /* 设置各算法模块卖场模式开关*/

#define HIIOC_PQ_S_SIXBASECOLOR         _IOW(HI_ID_PQ, 30, HI_PQ_SIX_BASE_COLOR_S)    /* 设置 六基色类型*/
#define HIIOC_PQ_G_SIXBASECOLOR         _IOR(HI_ID_PQ, 31, HI_PQ_SIX_BASE_COLOR_S)    /* 获取六基色类型*/

#define HIIOC_PQ_S_NR_AUTO              _IOW(HI_ID_PQ, 33, HI_U32)                    /* 设置NR自动模式*/
#define HIIOC_PQ_G_NR_AUTO              _IOR(HI_ID_PQ, 34, HI_U32)                    /* 获取NR自动模式*/

#define HIIOC_PQ_S_SR_DEMO              _IOW(HI_ID_PQ, 35, HI_PQ_SR_DEMO_E)           /* 设置SR演示模式*/
#define HIIOC_PQ_G_SR_DEMO              _IOR(HI_ID_PQ, 36, HI_PQ_SR_DEMO_E)           /* 获取SR演示模式*/

/*调试接口*/
#define HIIOC_PQ_S_3DSHARP              _IOW(HI_ID_PQ, 40, HI_U32)                    /* 设置3D SHARP mode*/
#define HIIOC_PQ_G_3DSHARP              _IOR(HI_ID_PQ, 41, HI_U32)                    /* 获取3D SHARP mode*/

//#define HIIOC_PQ_S_CSCMODE                    _IOW(HI_ID_PQ, 42, HI_PQ_CSC_S)                          /* 设置CSC mode*/
//#define HIIOC_PQ_G_CSCMODE                    _IOR(HI_ID_PQ, 43, HI_PQ_CSC_S)                           /* 获取CSC mode*/

#define HIIOC_PQ_S_REGISTER             _IOW(HI_ID_PQ, 44, HI_PQ_REGISTER_S)          /* 写寄存器*/
#define HIIOC_PQ_G_REGISTER             _IOWR(HI_ID_PQ, 45, HI_PQ_REGISTER_S)         /*读寄存器*/

//#define HIIOC_PQ_S_GAMMA                      _IOW(HI_ID_PQ, 46, HI_PQ_GAMMA_TABLE_S)            /* 写GAMMA曲线*/
//#define HIIOC_PQ_G_GAMMA                      _IOR(HI_ID_PQ, 47, HI_PQ_GAMMA_TABLE_S)             /*读GAMMA曲线*/

#define HIIOC_PQ_S_DCI                 _IOW(HI_ID_PQ, 48, HI_PQ_DCI_WGT_S)            /*写DCI曲线*/
#define HIIOC_PQ_G_DCI                 _IOR(HI_ID_PQ, 49, HI_PQ_DCI_WGT_S)            /*读DCI曲线*/

#define HIIOC_PQ_S_ACM_GAIN            _IOW(HI_ID_PQ, 50, HI_PQ_COLOR_GAIN_S)         /*写COLOR GAIN*/
#define HIIOC_PQ_G_ACM_GAIN            _IOR(HI_ID_PQ, 51, HI_PQ_COLOR_GAIN_S)         /*读COLOR GAIN*/

#define HIIOC_PQ_S_ACM_LUMA            _IOW(HI_ID_PQ, 52, HI_PQ_ACM_LUT_S)            /*写COLOR LUMA曲线*/
#define HIIOC_PQ_G_ACM_LUMA            _IOR(HI_ID_PQ, 53, HI_PQ_ACM_LUT_S)            /*读COLOR LUMA曲线*/

#define HIIOC_PQ_S_ACM_HUE             _IOW(HI_ID_PQ, 54, HI_PQ_ACM_LUT_S)            /*写COLOR HUE曲线*/
#define HIIOC_PQ_G_ACM_HUE             _IOR(HI_ID_PQ, 55, HI_PQ_ACM_LUT_S)            /*读COLOR HUE曲线*/

#define HIIOC_PQ_S_ACM_SAT             _IOW(HI_ID_PQ, 56, HI_PQ_ACM_LUT_S)            /*写COLOR SAT曲线*/
#define HIIOC_PQ_G_ACM_SAT             _IOR(HI_ID_PQ, 57, HI_PQ_ACM_LUT_S)            /*读COLOR SAT曲线*/

#define HIIOC_PQ_G_DCI_HIST            _IOR(HI_ID_PQ, 58, HI_PQ_DCI_HISTGRAM_S)       /*读DCI直方图*/

#define HIIOC_PQ_S_DCI_BS              _IOW(HI_ID_PQ, 60, HI_PQ_DCI_BS_LUT_S)         /*写DCI 黑扩展曲线*/
#define HIIOC_PQ_G_DCI_BS              _IOR(HI_ID_PQ, 61, HI_PQ_DCI_BS_LUT_S)         /*读DCI 黑扩展曲线*/

#define HIIOC_PQ_S_TNR_Y_PIXMEAN_2_RATIO       _IOW(HI_ID_PQ, 62, HI_PQ_TNR_S)        /*写TNR的亮度PixMean-to-Ratio*/
#define HIIOC_PQ_G_TNR_Y_PIXMEAN_2_RATIO       _IOR(HI_ID_PQ, 63, HI_PQ_TNR_S)        /*读TNR的亮度PixMean-to-Ratio*/

#define HIIOC_PQ_S_TNR_C_PIXMEAN_2_RATIO       _IOW(HI_ID_PQ, 64, HI_PQ_TNR_S)        /*写TNR 色度PixMean-to-Ratio映射曲线*/
#define HIIOC_PQ_G_TNR_C_PIXMEAN_2_RATIO       _IOR(HI_ID_PQ, 65, HI_PQ_TNR_S)        /*读TNR 色度PixMean-to-Ratio映射曲线*/

#define HIIOC_PQ_S_TNR_Y_MOTION_MAPPING        _IOW(HI_ID_PQ, 66, HI_PQ_TNR_S)        /*写TNR亮度MotionMapping曲线*/
#define HIIOC_PQ_G_TNR_Y_MOTION_MAPPING        _IOR(HI_ID_PQ, 67, HI_PQ_TNR_S)        /*读TNR亮度MotionMapping曲线*/

#define HIIOC_PQ_S_TNR_C_MOTION_MAPPING        _IOW(HI_ID_PQ, 68, HI_PQ_TNR_S)        /*写TNR色度MotionMapping曲线*/
#define HIIOC_PQ_G_TNR_C_MOTION_MAPPING        _IOR(HI_ID_PQ, 69, HI_PQ_TNR_S)        /*读TNR色度MotionMapping曲线*/

#define HIIOC_PQ_S_TNR_Y_FINAL_MOTION_MAPPING  _IOW(HI_ID_PQ, 70, HI_PQ_TNR_S)        /*写TNR亮度FINAL MotionMapping曲线*/
#define HIIOC_PQ_G_TNR_Y_FINAL_MOTION_MAPPING  _IOR(HI_ID_PQ, 71, HI_PQ_TNR_S)        /*读TNR亮度FINAL MotionMapping曲线*/

#define HIIOC_PQ_S_TNR_C_FINAL_MOTION_MAPPING  _IOW(HI_ID_PQ, 72, HI_PQ_TNR_S)        /*写TNR色度FINAL MotionMapping曲线*/
#define HIIOC_PQ_G_TNR_C_FINAL_MOTION_MAPPING  _IOR(HI_ID_PQ, 73, HI_PQ_TNR_S)        /*读TNR色度FINAL MotionMapping曲线*/

#define HIIOC_PQ_S_SNR_PIXMEAN_2_RATIO         _IOW(HI_ID_PQ, 74, HI_PQ_SNR_PIXMEAN_2_RATIO_S)     /*写SNR 的pixmean-ratio映射曲线*/
#define HIIOC_PQ_G_SNR_PIXMEAN_2_RATIO         _IOR(HI_ID_PQ, 75, HI_PQ_SNR_PIXMEAN_2_RATIO_S)     /*读SNR 的pixmean-ratio映射曲线*/

#define HIIOC_PQ_S_SNR_PIXDIFF_2_EDGESTR       _IOW(HI_ID_PQ, 76, HI_PQ_SNR_PIXDIFF_2_EDGESTR_S)   /*写SNR 的pixdiff-edgestr映射曲线*/
#define HIIOC_PQ_G_SNR_PIXDIFF_2_EDGESTR       _IOR(HI_ID_PQ, 77, HI_PQ_SNR_PIXDIFF_2_EDGESTR_S)   /*读SNR 的pixdiff-edgestr映射曲线*/

#define HIIOC_PQ_S_HD_BRIGHTNESS        _IOW(HI_ID_PQ, 84, HI_U32)                    /* 设置高清BRIGHTNESS level*/
#define HIIOC_PQ_G_HD_BRIGHTNESS        _IOR(HI_ID_PQ, 85, HI_U32)                    /* 获取高清BRIGHTNESS level*/

#define HIIOC_PQ_S_HD_CONTRAST          _IOW(HI_ID_PQ, 86, HI_U32)                    /* 设置高清CONTRAST level*/
#define HIIOC_PQ_G_HD_CONTRAST          _IOR(HI_ID_PQ, 87, HI_U32)                    /* 获取高清CONTRAST level*/

#define HIIOC_PQ_S_HD_SATURATION        _IOW(HI_ID_PQ, 88, HI_U32)                    /* 设置高清SATURATION level*/
#define HIIOC_PQ_G_HD_SATURATION        _IOR(HI_ID_PQ, 89, HI_U32)                    /* 获取高清SATURATION level*/

#define HIIOC_PQ_S_HD_HUE               _IOW(HI_ID_PQ, 90, HI_U32)                    /* 设置高清HUE level*/
#define HIIOC_PQ_G_HD_HUE               _IOR(HI_ID_PQ, 91, HI_U32)                    /* 获取高清HUE level*/

#define HIIOC_PQ_S_DCI_LEVEL            _IOW(HI_ID_PQ, 92, HI_U32)                    /* 设置DCI level*/
#define HIIOC_PQ_G_DCI_LEVEL            _IOR(HI_ID_PQ, 93, HI_U32)                    /* 获取DCI level*/

#define HIIOC_PQ_G_BIN_ADDR             _IOR(HI_ID_PQ, 94, HI_U32)                    /* 获取PQBin的物理地址*/

#define HIIOC_PQ_S_COLOR_ENHANCE_MODE   _IOW(HI_ID_PQ, 95, HI_PQ_COLOR_SPEC_MODE_E)   /* 设置颜色增强模式*/
#define HIIOC_PQ_G_COLOR_ENHANCE_MODE   _IOR(HI_ID_PQ, 96, HI_PQ_COLOR_SPEC_MODE_E)   /* 获取颜色增强模式*/

#define HIIOC_PQ_S_ACM_CTRL             _IOW(HI_ID_PQ, 97, HI_PQ_COLOR_CTRL_S)        /* 设置ACM 控制寄存器*/
#define HIIOC_PQ_G_ACM_CTRL             _IOR(HI_ID_PQ, 98, HI_PQ_COLOR_CTRL_S)        /* 获取ACM 控制寄存器*/

#define HIIOC_PQ_S_DEMO_MODE            _IOW(HI_ID_PQ, 99, HI_PQ_DEMO_MODE_E)         /* 设置各算法模块卖场模式显示方式*/
#define HIIOC_PQ_G_DEMO_MODE            _IOR(HI_ID_PQ, 100, HI_PQ_DEMO_MODE_E)        /* 获取各算法模块卖场模式显示方式*/

#define HIIOC_PQ_S_GRAPH_SD_PARAM       _IOW(HI_ID_PQ, 101, HI_PQ_IMAGE_PARAM_S)      /* 设置图形标清参数*/
#define HIIOC_PQ_G_GRAPH_SD_PARAM       _IOR(HI_ID_PQ, 102, HI_PQ_IMAGE_PARAM_S)      /* 获取图形标清参数*/

#define HIIOC_PQ_S_GRAPH_HD_PARAM       _IOW(HI_ID_PQ, 103, HI_PQ_IMAGE_PARAM_S)      /* 设置图形高清参数*/
#define HIIOC_PQ_G_GRAPH_HD_PARAM       _IOR(HI_ID_PQ, 104, HI_PQ_IMAGE_PARAM_S)      /* 获取图形高清参数*/

#define HIIOC_PQ_S_VIDEO_SD_PARAM       _IOW(HI_ID_PQ, 105, HI_PQ_IMAGE_PARAM_S)      /* 设置视频标清参数*/
#define HIIOC_PQ_G_VIDEO_SD_PARAM       _IOR(HI_ID_PQ, 106, HI_PQ_IMAGE_PARAM_S)      /* 获取视频标清参数*/

#define HIIOC_PQ_S_VIDEO_HD_PARAM       _IOW(HI_ID_PQ, 107, HI_PQ_IMAGE_PARAM_S)      /* 设置视频高清参数*/
#define HIIOC_PQ_G_VIDEO_HD_PARAM       _IOR(HI_ID_PQ, 108, HI_PQ_IMAGE_PARAM_S)      /* 获取视频高清参数*/

#define HIIOC_PQ_S_TNR_FMOTION_MAPPING  _IOW(HI_ID_PQ, 109, HI_PQ_TNR_FMOTION_S)      /*写TNR FMotionMapping 曲线*/
#define HIIOC_PQ_G_TNR_FMOTION_MAPPING  _IOR(HI_ID_PQ, 110, HI_PQ_TNR_FMOTION_S)      /*读TNR FMotionMapping 曲线*/

#define HIIOC_PQ_S_DEFAULT_PARAM        _IOW(HI_ID_PQ, 111, HI_BOOL)                  /*设置PQ 的默认值*/

#define HIIOC_PQ_S_SNR                  _IOW(HI_ID_PQ, 112, HI_U32)                   /* set SNR level*/
#define HIIOC_PQ_G_SNR                  _IOR(HI_ID_PQ, 113, HI_U32)                   /* get SNR level*/

#define HIIOC_PQ_S_SNR_MEAN_RATIO       _IOW(HI_ID_PQ, 114, HI_PQ_SNR_MEAN_RATIO_S)   /*写SNR 的MEAN RATIO映射曲线*/
#define HIIOC_PQ_G_SNR_MEAN_RATIO       _IOR(HI_ID_PQ, 115, HI_PQ_SNR_MEAN_RATIO_S)   /*读SNR 的MEAN RATIO映射曲线*/

#define HIIOC_PQ_S_SNR_EDGE_STR         _IOW(HI_ID_PQ, 116, HI_PQ_SNR_EDGE_STR_S)     /*写SNR 的EDGE STR映射曲线*/
#define HIIOC_PQ_G_SNR_EDGE_STR         _IOR(HI_ID_PQ, 117, HI_PQ_SNR_EDGE_STR_S)     /*读SNR 的EDGE STR映射曲线*/

#define HIIOC_PQ_S_GAMM_PARA            _IOW(HI_ID_PQ, 118, HI_PQ_CSC_GAMM_PARA_S)
#define HIIOC_PQ_G_GAMM_PARA            _IOR(HI_ID_PQ, 119, HI_PQ_CSC_GAMM_PARA_S)

#define HIIOC_PQ_S_DEGAMM_PARA          _IOW(HI_ID_PQ, 120, HI_PQ_CSC_GAMM_PARA_S)
#define HIIOC_PQ_G_DEGAMM_PARA          _IOR(HI_ID_PQ, 121, HI_PQ_CSC_GAMM_PARA_S)

#define HIIOC_PQ_S_GFX_GAMM_PARA        _IOW(HI_ID_PQ, 122, HI_PQ_CSC_GAMM_PARA_S)
#define HIIOC_PQ_G_GFX_GAMM_PARA        _IOR(HI_ID_PQ, 123, HI_PQ_CSC_GAMM_PARA_S)

#define HIIOC_PQ_S_GFX_DEGAMM_PARA      _IOW(HI_ID_PQ, 124, HI_PQ_CSC_GAMM_PARA_S)
#define HIIOC_PQ_G_GFX_DEGAMM_PARA      _IOR(HI_ID_PQ, 125, HI_PQ_CSC_GAMM_PARA_S)

#define HIIOC_PQ_S_DEMO_COORDINATE      _IOW(HI_ID_PQ, 126, HI_U32)                   /* set demo coordinate*/
#define HIIOC_PQ_G_DEMO_COORDINATE      _IOR(HI_ID_PQ, 127, HI_U32)                   /* get demo coordinate*/

#define HIIOC_PQ_S_DM                   _IOW(HI_ID_PQ, 128, HI_U32)                   /* set dm level*/
#define HIIOC_PQ_G_DM                   _IOR(HI_ID_PQ, 129, HI_U32)                   /* get dm level*/

#define HIIOC_PQ_S_HDR_SATURATION       _IOW(HI_ID_PQ, 130, HI_U32)                   /* set HDR SATURATION level*/
#define HIIOC_PQ_G_HDR_SATURATION       _IOR(HI_ID_PQ, 131, HI_U32)                   /* get HDR SATURATION level*/

#define HIIOC_PQ_S_HDR_TMLUT            _IOW(HI_ID_PQ, 132, HI_PQ_HDR_TM_LUT_S)       /* set HDR TM Lut*/
#define HIIOC_PQ_G_HDR_TMLUT            _IOR(HI_ID_PQ, 133, HI_PQ_HDR_TM_LUT_S)       /* get HDR TM Lut*/

#define HIIOC_PQ_S_IMAGE_MODE           _IOW(HI_ID_PQ, 134, HI_PQ_IMAGE_MODE_E)       /* set VideoPhone mode*/
#define HIIOC_PQ_G_IMAGE_MODE           _IOR(HI_ID_PQ, 135, HI_PQ_IMAGE_MODE_E)       /* get VideoPhone mode*/

#define HIIOC_PQ_S_ACC_LUT              _IOW(HI_ID_PQ, 136, PQ_ACC_LUT_S)             /* set ACC Lut*/
#define HIIOC_PQ_G_ACC_LUT              _IOR(HI_ID_PQ, 137, PQ_ACC_LUT_S)             /* get ACC Lut*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __HI_DRV_PQ_H__ */
