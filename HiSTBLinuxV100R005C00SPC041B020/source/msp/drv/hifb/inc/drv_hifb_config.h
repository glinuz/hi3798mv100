/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hifb_config.h
Version             : Initial Draft
Author              :
Created             : 2015/09/09
Description         : 所有信息的变化都在这一个文件中体现，其它的保持不动
Function List       :
History             :
Date                       Author                   Modification
2014/09/09                 y00181162                Created file
******************************************************************************/

#ifndef __HIFB_CONFIG_H__
#define __HIFB_CONFIG_H__


/*********************************add include here******************************/

#include "hi_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif /* __cplusplus */



/***************************** Macro Definition ******************************/
#define CONFIG_HIFB_LAYER_0BUFFER    0
#define CONFIG_HIFB_LAYER_1BUFFER    1
#define CONFIG_HIFB_LAYER_2BUFFER    2
#define CONFIG_HIFB_LAYER_3BUFFER    3

#define CONFIG_HIFB_STEREO_BUFFER_MAX_NUM       2
#ifdef CFG_HIFB_ANDROID_SUPPORT
    #define CONFIG_HIFB_LAYER_BUFFER_MAX_NUM    3
#else
    #define CONFIG_HIFB_LAYER_BUFFER_MAX_NUM    2
#endif


/**
 ** 每个寄存器的偏移地址
 **/
#define CONFIG_HIFB_CHN_OFFSET          0x400
#define CONFIG_HIFB_WBC_OFFSET          0x400

#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) \
|| defined(CHIP_TYPE_hi3798cv200)    || defined(CHIP_TYPE_hi3798mv200)   \
|| defined(CHIP_TYPE_hi3798mv200_a)  || defined(CHIP_TYPE_hi3796mv200)
    #define CONFIG_HIFB_GFX_OFFSET     0x200
    #define CONFIG_HIFB_GP_OFFSET      0x800
#else
    #define CONFIG_HIFB_GFX_OFFSET     0x800             /** 图层   **/
    #define CONFIG_HIFB_GP_OFFSET      0x800              /** 图层GP **/
#endif

#define CONFIG_VDP_SIZE                0xffff


/**
 ** VDP寄存器基地址
 **/
#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) \
 || defined(CHIP_TYPE_hi3798cv200)   || defined(CHIP_TYPE_hi3798mv200)   \
 || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    /** 0XB208图形和视屏叠加寄存器 **/
    #define CONFIG_VDP_REG_BASEADDR                         (0xf8cc0000)
    #define CONFIG_HIFB_HD0_REG_BASEADDR                (0xf8cc7000)
    /** sd0 -> fb4 **/
    #define CONFIG_HIFB_SD_LOGO_REG_BASEADDR            (0xf8cc7800)
    #define CONFIG_HIFB_WBC_SLAYER_REG_BASEADDR         (0xf8cc7800)
    /** WBC_GP0同源回写 0xf8cc9400**/
    #define CONFIG_HIFB_WBC_GP0_REG_BASEADDR            (0xf8cc9400)

#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_VDP_REG_BASEADDR                         (0xf8cc0000)
    #define CONFIG_HIFB_HD0_REG_BASEADDR                (0xf8cc6000)
    #define CONFIG_HIFB_SD_LOGO_REG_BASEADDR            (0xf8cc8000)
    #define CONFIG_HIFB_WBC_SLAYER_REG_BASEADDR         (0xf8cc8000)
    #define CONFIG_HIFB_WBC_GP0_REG_BASEADDR            (0xf8cca800)
#else
    #define CONFIG_VDP_REG_BASEADDR                         (0xf8cc0000)
    #define CONFIG_HIFB_HD0_REG_BASEADDR                (0xf8cc6000)
    /**
     ** (CONFIG_HIFB_HD0_REG_BASEADDR + 3 * OPTM_GFX_OFFSET) 标清层LOGO SD1(双显功能G3->G5)
     **/
    #define CONFIG_HIFB_SD_LOGO_REG_BASEADDR            (0xf8cc7800)
    /**
     ** (CONFIG_HIFB_SD0_REG_BASEADDR + 4*OPTM_GFX_OFFSET) 同源回写使用标清层G4
     **/
    #define CONFIG_HIFB_WBC_SLAYER_REG_BASEADDR         (0xf8cc8000)
    /** WBC_GP0同源回写 0xf8cca800**/
    #define CONFIG_HIFB_WBC_GP0_REG_BASEADDR            (0xf8cca800)
#endif

/** logo use hd0 **/
#define CONFIG_HIFB_HD_LOGO_REG_BASEADDR                CONFIG_HIFB_HD0_REG_BASEADDR

#define CONFIG_HIFB_MDDRC_REG_BASEADDR  (0xF8A35000)
#define CONFIG_HIFB_MDDRC_OFFSET         0x8
#define CONFIG_HIFB_MDDRC_MAX_ZONE       32


/**
 **是否支持将高清通道图层3挂载到标清通道变成图层5
 **/
#if defined(CHIP_TYPE_hi3716cv200)
    /** 支持双显功能TC **/
    #define CONFIG_HIFB_GFX3_TO_GFX5
#endif

/**
 ** ZME COEF
 ** 处理水平精度查询
 ** 98MV100/HIFONE-STB: 水平精度查询是按照20bit
 ** 其它: 水平精度查询是按照12bit
 ** 本来是按照12bit计算的，按照这个再进行偏移
 **/
#if    defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_PRECISION_ZMERATIONH_12BITOFFSET          8
#else
    #define CONFIG_HIFB_PRECISION_ZMERATIONH_12BITOFFSET          0
#endif
/**
 ** 水平方向同时具有行和列缩放
 **/
#if    defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_GP0_ZME_HLC_OFFSET_ENABLE
#endif
/**
 **
 **/
#if    defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_ALG_G_HZME_PRECISION      1048576
    #define CONFIG_HIFB_ALG_G_VZME_PRECISION      4096
#else
    #define CONFIG_HIFB_ALG_G_HZME_PRECISION      4096
    #define CONFIG_HIFB_ALG_G_VZME_PRECISION      4096
#endif


/**
 ** GP支持的图形层个数
 **/
#if     defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
     || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_GP0_SUPPORT_GFX_COUNT          2 /** (G0 G1) **/
    #define CONFIG_HIFB_GP1_SUPPORT_GFX_COUNT          1 /** (G4)    **/
#elif   defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) \
     || defined(CHIP_TYPE_hi3798cv200)   || defined(CHIP_TYPE_hi3798mv200)   \
     || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    #define CONFIG_HIFB_GP0_SUPPORT_GFX_COUNT          3 /** (G0 G1 G3) **//** G2不支持 **/
    #define CONFIG_HIFB_GP1_SUPPORT_GFX_COUNT          1 /** (G4) **/
#else
    #define CONFIG_HIFB_GP0_SUPPORT_GFX_COUNT           3 /** (G0 G1 G2)       **/
    #define CONFIG_HIFB_GP1_SUPPORT_GFX_COUNT           2 /** (G3(变G5) G4 G5) **/
#endif


#if defined(CHIP_TYPE_hi3798mv100)  || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
 || defined(CHIP_TYPE_hi3716mv410)  || defined(CHIP_TYPE_hi3716mv420)
    /** 先垂直后水平 **/
    #define CONFIG_HIFB_ZME_ORDER_VH
#elif  defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    /** 先水平后垂直 **/
    #define CONFIG_HIFB_ZME_ORDER_HV
#else
    /** 先水平后垂直 **/
    #define CONFIG_HIFB_ZME_ORDER_HV
#endif



/**
 ** 标清层使用的LOGO图层，这里要用"\"，不然会定义错误
 **/
#if defined(CHIP_TYPE_hi3798mv100)   || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
 || defined(CHIP_TYPE_hi3716mv410)   || defined(CHIP_TYPE_hi3716mv420)   \
 || defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) \
 || defined(CHIP_TYPE_hi3798cv200)   || defined(CHIP_TYPE_hi3798mv200)   \
 || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    /**
     ** 是否需要抽行处理，只和性能有关，和功能没有关系，逻辑缩放都支持
     ** 性能不够就用抽行
     **/
    #define CONFIG_HIFB_EXTRACE_DISABLE
#endif

/**
 ** 使用公共寄存器头文件
 **/
#if    defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3796mv200)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_USE_COMM_REGH
#endif


/**
 **support input color format
 **/
#ifndef HI_BUILD_IN_BOOT
    #if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_HD1_LAYER_16BIT_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_24BIT_FMT            0
    #define CONFIG_HIFB_HD1_LAYER_32BIT_FMT            0
    #define CONFIG_HIFB_HD1_LAYER_CLUT1_FMT            0
    #define CONFIG_HIFB_HD1_LAYER_CLUT2_FMT            0
    #define CONFIG_HIFB_HD1_LAYER_CLUT4_FMT            0
    #define CONFIG_HIFB_HD1_LAYER_CLUT8_FMT            0
    #else
    #define CONFIG_HIFB_HD1_LAYER_16BIT_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_24BIT_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_32BIT_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_CLUT1_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_CLUT2_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_CLUT4_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_CLUT8_FMT            1
    #endif
#else
    /**
     **开机logo比较难出现低带宽，所以都支持，方便平滑过渡
     **单图层方案改了之后这个地方可以去掉，方便代码统一
     **/
    #define CONFIG_HIFB_HD1_LAYER_16BIT_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_24BIT_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_32BIT_FMT            1
    #define CONFIG_HIFB_HD1_LAYER_CLUT1_FMT            0
    #define CONFIG_HIFB_HD1_LAYER_CLUT2_FMT            0
    #define CONFIG_HIFB_HD1_LAYER_CLUT4_FMT            0
    #define CONFIG_HIFB_HD1_LAYER_CLUT8_FMT            0
#endif


/**
 **判断图层是否支持
 **/
#if    defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_HD0_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_HD1_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_HD2_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_HD3_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_SD0_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_SD2_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_SD3_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD0_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD1_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD2_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD3_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_CURSOR_LAYER_SUPPORT                HI_FALSE
#elif  defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    #define CONFIG_HIFB_HD0_LAYER_SUPPORT                  HI_TRUE/** G0 **/
    #define CONFIG_HIFB_HD1_LAYER_SUPPORT                  HI_TRUE/** G1 **/
    #define CONFIG_HIFB_HD2_LAYER_SUPPORT                  HI_TRUE/** G3 **/
    #define CONFIG_HIFB_HD3_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_SD0_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_SD2_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_SD3_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD0_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD1_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD2_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD3_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_CURSOR_LAYER_SUPPORT                HI_FALSE
#else
    #define CONFIG_HIFB_HD0_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_HD1_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_HD2_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_HD3_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_SD0_LAYER_SUPPORT                  HI_TRUE
    #define CONFIG_HIFB_SD2_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_SD3_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD0_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD1_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD2_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_AD3_LAYER_SUPPORT                  HI_FALSE
    #define CONFIG_HIFB_CURSOR_LAYER_SUPPORT                HI_FALSE
#endif

#if defined(CHIP_TYPE_hi3719mv100)  || defined(CHIP_TYPE_hi3718mv100)   \
 || defined(CHIP_TYPE_hi3798mv100)  || defined(CHIP_TYPE_hi3796mv100)   \
 || defined(CHIP_TYPE_hi3716dv100)  || defined(CHIP_TYPE_hi3796mv200)   \
 || defined(CHIP_TYPE_hi3798cv200_a)|| defined(CHIP_TYPE_hi3798cv200_b) \
 || defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200)   \
 || defined(CHIP_TYPE_hi3798mv200_a)                                    \
 || defined(CHIP_TYPE_hi3716mv410)  || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_SD1_LAYER_SUPPORT                  HI_FALSE
#else
    #ifndef HI_BUILD_IN_BOOT
        #ifdef CFG_HIFB_CURSOR_SUPPORT
            #define CONFIG_HIFB_SD1_LAYER_SUPPORT          HI_TRUE
        #else
               #define CONFIG_HIFB_SD1_LAYER_SUPPORT          HI_FALSE
        #endif
    #else
            #define CONFIG_HIFB_SD1_LAYER_SUPPORT          HI_TRUE
    #endif
#endif
/**
 **判断压缩解压是否支持
 **/
#if    defined(CHIP_TYPE_hi3798mv100)    \
    || defined(CHIP_TYPE_hi3796mv100)    \
    || defined(CHIP_TYPE_hi3716dv100)    \
    || defined(CHIP_TYPE_hi3798cv200_a)  \
    || defined(CHIP_TYPE_hi3798cv200_b)  \
    || defined(CHIP_TYPE_hi3798cv200)    \
    || defined(CHIP_TYPE_hi3798mv200)    \
    || defined(CHIP_TYPE_hi3798mv200_a)  \
    || defined(CHIP_TYPE_hi3796mv200)    \
    || defined(CHIP_TYPE_hi3716mv410)    \
    || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_HD0_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_HD1_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_HD2_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_HD3_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_SD0_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_SD1_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_SD2_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_SD3_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_AD0_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_AD1_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_AD2_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_AD3_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_CURSOR_COMPRESSION_SUPPORT         HI_FALSE
#else
    #define CONFIG_HIFB_HD0_COMPRESSION_SUPPORT            HI_TRUE
    #define CONFIG_HIFB_HD1_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_HD2_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_HD3_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_SD0_COMPRESSION_SUPPORT            HI_TRUE
    #define CONFIG_HIFB_SD1_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_SD2_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_SD3_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_AD0_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_AD1_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_AD2_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_AD3_COMPRESSION_SUPPORT            HI_FALSE
    #define CONFIG_HIFB_CURSOR_COMPRESSION_SUPPORT         HI_FALSE
#endif


#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
    #define CONFIG_HIFB_HD_SUPPORT_4KUI
    #define CONFIG_HIFB_HD_LAYER_MAXWIDTH         3840
    #define CONFIG_HIFB_HD_LAYER_MAXHEIGHT        2160
    #define CONFIG_HIFB_SD_LAYER_MAXWIDTH         2560
    #define CONFIG_HIFB_SD_LAYER_MAXHEIGHT        1600
    #define CONFIG_HIFB_AD_LAYER_MAXWIDTH         1920
    #define CONFIG_HIFB_AD_LAYER_MAXHEIGHT        1080
    #define CONFIG_HIFB_CURSOR_LAYER_MAXWIDTH     1920
    #define CONFIG_HIFB_CURSOR_LAYER_MAXHEIGHT    1080
#elif defined(CHIP_TYPE_hi3798cv200)   || defined(CHIP_TYPE_hi3798mv200) \
   || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    #define CONFIG_HIFB_HD_LAYER_MAXWIDTH         3840
    #define CONFIG_HIFB_HD_LAYER_MAXHEIGHT        2160
    #define CONFIG_HIFB_SD_LAYER_MAXWIDTH         720
    #define CONFIG_HIFB_SD_LAYER_MAXHEIGHT        576
    #define CONFIG_HIFB_AD_LAYER_MAXWIDTH         1920
    #define CONFIG_HIFB_AD_LAYER_MAXHEIGHT        1080
    #define CONFIG_HIFB_CURSOR_LAYER_MAXWIDTH     1920
    #define CONFIG_HIFB_CURSOR_LAYER_MAXHEIGHT    1080
#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    #define CONFIG_HIFB_HD_LAYER_MAXWIDTH         1280
    #define CONFIG_HIFB_HD_LAYER_MAXHEIGHT        720
    #define CONFIG_HIFB_SD_LAYER_MAXWIDTH         720
    #define CONFIG_HIFB_SD_LAYER_MAXHEIGHT        576
    #define CONFIG_HIFB_AD_LAYER_MAXWIDTH         1280
    #define CONFIG_HIFB_AD_LAYER_MAXHEIGHT        720
    #define CONFIG_HIFB_CURSOR_LAYER_MAXWIDTH     1280
    #define CONFIG_HIFB_CURSOR_LAYER_MAXHEIGHT    720
#else
    #define CONFIG_HIFB_HD_LAYER_MAXWIDTH         2560
    #define CONFIG_HIFB_HD_LAYER_MAXHEIGHT        1600
    #define CONFIG_HIFB_SD_LAYER_MAXWIDTH         2560
    #define CONFIG_HIFB_SD_LAYER_MAXHEIGHT        1600
    #define CONFIG_HIFB_AD_LAYER_MAXWIDTH         1920
    #define CONFIG_HIFB_AD_LAYER_MAXHEIGHT        1080
    #define CONFIG_HIFB_CURSOR_LAYER_MAXWIDTH     1920
    #define CONFIG_HIFB_CURSOR_LAYER_MAXHEIGHT    1080
#endif

#if   defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) \
   || defined(CHIP_TYPE_hi3798cv200)   || defined(CHIP_TYPE_hi3798mv200)   \
   || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    /** 压缩解码规格已经删除 **/
    #ifdef CFG_HIFB_COMPRESSION_SUPPORT
        #undef CFG_HIFB_COMPRESSION_SUPPORT
    #endif
    #define GFX_CONFIG_GP0_MZE_VSP_2TAB_EN
#endif

#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
    /** the pre zme enable **/
    #define GFX_CONFIG_WBC_GP0_PRE_ZME_EN
#endif


#define CONFIG_HIFB_STRIDE_16ALIGN          16


#if   defined(CHIP_TYPE_hi3798cv200)   || defined(CHIP_TYPE_hi3798mv200)  \
   || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
      #define CONFIG_HIFB_MMU_SUPPORT
      #define CONFIG_HIFB_GP0_GALPHA_SUM_SUPPORT
#endif


#if defined(CHIP_TYPE_hi3798cv200)
#define CONFIG_HIFB_GP0CSC_USE_HDRMOD   /** GP0 CSC和HDR绑定 **/
#define CONFIG_HIFB_BI2020_SUPPORT
#endif


#if defined(CHIP_TYPE_hi3798cv200)
#define  CONFIG_HIFB_CALCSTRIDE_WITH_ONEPOINTTHREE
#endif

//#define CONFIG_HIFB_TRANSIT_LOGO_BY_MASK

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/

#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __HIFB_CONFIG_H__ */
