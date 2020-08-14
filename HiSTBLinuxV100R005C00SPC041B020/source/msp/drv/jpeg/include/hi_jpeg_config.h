/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : hi_jpeg_config.h
Version            : Initial Draft
Author            :
Created            : 2016/02/02
Description        : this file is through set macro to select different funciton,
                  and select different platform
Function List     :


History           :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/



#ifndef __HI_JPEG_CONFIG_H__
#define __HI_JPEG_CONFIG_H__

/*********************************add include here******************************/
#ifndef HI_BUILD_IN_BOOT
    #ifdef __KERNEL__
     #include "hi_gfx_comm_k.h"
    #else
        #ifdef CONFIG_HI_FPGA_JPEG_VERSION
             #include "hi_gfx_test_comm.h"
        #else
            #include "hi_gfx_comm.h"
        #endif
    #endif
#else
    #include "exports.h"
#endif
/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */


    /***************************** Macro Definition ******************************/

    /** \addtogroup      JPEG CFG */
    /** @{ */  /** <!-- 【JPEG CFG】 */


    /****************************************************************************
     **宏开关都是在Makefile中定义
     ****************************************************************************/
    /** this macro is add google function and use in android version,define in Makefile */
    /** CNcomment:将android的google版本核入jpeg-6b中，在Makefile中控制该宏开关 CNend */
    #if 0
    #define CONFIG_JPEG_ADD_GOOGLEFUNCTION
    #endif

     /****************************************************************************
      ** function marco definition no depend chip platform
      ** CNcomment:和芯片平台无关的宏定义
     ****************************************************************************/
     #ifdef CONFIG_GFX_ANDROID_SDK
        /** use android debug message */
        /** CNcomment:Android版本的调试信息 */
        #define CONFIG_JPEG_ANDROID_DEBUG_ENABLE
     #endif

     /** fpga and eda and advca not use this file **/
     #if (1 == HI_PROC_SUPPORT)
        /** if need proc message */
        /** CNcomment:是否需要proc信息 */
        #define CONFIG_JPEG_PROC_ENABLE
     #endif

    /** if need use soft csc to debug whether the hard csc has problem */
    /** CNcomment:使用软件转换来定位硬件转换是否有问题 */
    //#define CONFIG_JPEG_SOFTCSC_ENABLE

    /** use hard idct method */
    /** CNcomment:使用硬件得IDCT算法 */
    #define CONFIG_JPEG_USEHWIDCT

    #define CONFIG_JPEG_MPG_DEC_ENABLE

/****************************************************************************
 ** function marco definition depend chip platform
 ** CNcomment:和芯片平台相关的宏定义
 ****************************************************************************/
    /** if support crop */
    /** CNcomment:是否支持裁剪功能 */
    #define CONFIG_JPEG_OUTPUT_CROP
    /** if support suspend */
    /** CNcomment:是否支持待机功能,在解码过程中待机的时候处理时钟等有点问题，先关闭该功能 */
    //#define CONFIG_JPEG_SUSPEND
    /** get jpeg dec time */
    /** CNcomment:获取jpeg解码的时间 */
    #define CONFIG_JPEG_GETDECTIME
    /** all jpeg dec output yuv420sp */
    /** CNcomment:统一解码输出yuv420sp,硬件和软件都不支持yuv400 -> yuv420
                　要是需要支持填充0x80 */
    #define CONFIG_JPEG_OUTPUT_YUV420SP
    /** dec jpeg file output lu pixel value sum */
    /** CNcomment:统计亮度值 */
    #define CONFIG_JPEG_OUTPUT_LUPIXSUM
    /** CNcomment:这里由于DDR无法处理跨4K内存的问题，所以JPGD逻辑
     ** 要对跨4K内存进行处理，否则会有一段段小黑条
     ** 0x0 - 0x1000 - 0x2000 - 0x3000 内存起始地址+大小要在区间内的问题 */

    /** HSCP201405300013 HSCP201405290010 DTS2014061006717**/
    /** CNcomment:hifone之后修改的按照采样因子来解码 **/
    #define CONFIG_JPEG_SET_SAMPLEFACTOR

    /** support omx dec             */
    /** CNcomment:实现OMX的方案设计 */
    #define CONFIG_JPEG_OMX_FUNCTION

    /** HSCP201502100030 **/
    /** CNcomment:在dri不符合标准协议得情况下解码花屏 **/
    #define CONFIG_JPEG_DRI_SUPPORT

    #define CONFIG_JPEG_TEST_SUPPORT

    /****************************************************************************
     ** 底下根据不同芯片平台将不支持的功能宏开关打开
     ****************************************************************************/
    #ifdef CONFIG_GFX_BVT_SDK
        #undef CONFIG_JPEG_SUSPEND
        #undef CONFIG_JPEG_GETDECTIME
        #undef CONFIG_JPEG_PROC_ENABLE
        #define CONFIG_JPEG_CSC_DISABLE
        /** 裁剪非yuvsp输出的代码 **/
        #define CONFIG_JPEG_CODE_CUT_RGB
    #endif

    #if   defined(HI_CHIP_TYPE_hi3531) || defined(HI_CHIP_TYPE_hi3532)   \
       || defined(HI_CHIP_TYPE_hi3521) || defined(HI_CHIP_TYPE_hi3520A)  \
       || defined(HI_CHIP_TYPE_hi3520D)
        #undef CONFIG_JPEG_OUTPUT_YUV420SP
        #undef CONFIG_JPEG_OUTPUT_LUPIXSUM
    #endif

    #if    defined(CHIP_TYPE_hi3716cv200)   \
        || defined(CHIP_TYPE_hi3716mv400)   \
        || defined(CHIP_TYPE_hi3718cv100)   \
        || defined(CHIP_TYPE_hi3718mv100)   \
        || defined(CHIP_TYPE_hi3719cv100)   \
        || defined(CHIP_TYPE_hi3719mv100)   \
        || defined(CHIP_TYPE_hi3719mv100_a)
        #undef CONFIG_JPEG_OUTPUT_YUV420SP
        #undef CONFIG_JPEG_OUTPUT_LUPIXSUM
        /** the save stream buffer whether 4 byte align */
        /** CNcomment:存储码流buffer是否需要4字节对齐,这个bug在3535/CV200(EC)之后的芯片解决了**/
        #define CONFIG_JPEG_STREAMBUF_4ALIGN
    #endif

    /**CV200 EC之后已经修改，有需要再打开 **/
    #ifdef CONFIG_JPEG_OUTPUT_CROP
        #undef CONFIG_JPEG_OUTPUT_CROP
    #endif

    /** 底下几颗芯片需要配置采样因子 **/
    #if   !defined(CHIP_TYPE_hi3798cv200_a)   \
       && !defined(CHIP_TYPE_hi3798cv200_b)   \
       && !defined(CHIP_TYPE_hi3798cv200)     \
       && !defined(CHIP_TYPE_hi3798mv200)     \
       && !defined(CHIP_TYPE_hi3798mv200_a)   \
       && !defined(CHIP_TYPE_hi3796mv200)     \
       && !defined(CHIP_TYPE_hi3798hv100)     \
       && !defined(CHIP_TYPE_hi3716mv410)     \
       && !defined(CHIP_TYPE_hi3716mv420)     \
       && !defined(CHIP_TYPE_hi3751av500)
        #undef CONFIG_JPEG_SET_SAMPLEFACTOR
    #endif

    /** 底下几颗芯片需要配置dri，否则花屏 **/
    #if   !defined(CHIP_TYPE_hi3716mv410)     \
       && !defined(CHIP_TYPE_hi3716mv420)     \
       && !defined(CHIP_TYPE_hi3798cv200)     \
       && !defined(CHIP_TYPE_hi3798mv200)     \
       && !defined(CHIP_TYPE_hi3798mv200_a)   \
       && !defined(CHIP_TYPE_hi3796mv200)     \
       && !defined(CHIP_TYPE_hi3798hv100)     \
       && !defined(CHIP_TYPE_hi3531a)
        #undef CONFIG_JPEG_DRI_SUPPORT
    #endif

    #if    defined(CHIP_TYPE_hi3716mv310) \
        || defined(CHIP_TYPE_hi3110ev500) \
        || defined(CHIP_TYPE_hi3716mv320) \
        || defined(CHIP_TYPE_hi3716mv330) \
        || defined(CHIP_TYPE_hi3712)      \
        || defined(CHIP_TYPE_hi3716c)     \
        || defined(CHIP_TYPE_hi3716m)     \
        || defined(CHIP_TYPE_hi3716h)
        #undef CONFIG_JPEG_OUTPUT_YUV420SP
        #undef CONFIG_JPEG_OUTPUT_LUPIXSUM
        #ifndef HI_BUILD_IN_BOOT
            #undef CONFIG_JPEG_OMX_FUNCTION
        #endif
    #endif

    #if   defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv320) \
       || defined(CHIP_TYPE_hi3716mv330) || defined(CHIP_TYPE_hi3110ev500)
        /** 深度裁剪方案 **/
        #define CONFIG_JPEG_CODE_DEPTH_CUT
        #ifdef UPSAMPLE_MERGING_SUPPORTED
          /** 不走这条通道的YUV输出 **/
          #undef UPSAMPLE_MERGING_SUPPORTED
        #endif
    #endif


    #if    !defined(CHIP_TYPE_hi3716mv310) && !defined(CHIP_TYPE_hi3110ev500)  \
        && !defined(CHIP_TYPE_hi3716mv320) && !defined(CHIP_TYPE_hi3716mv330)
        /** 只支持mv310 x5v500 mv320的开机动画，其它平台都用ts流做瞬播 **/
        #ifdef HI_MCE_SUPPORT
            #undef HI_MCE_SUPPORT
        #endif
    #endif


    #if    defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200)   \
        || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200) \
        && !defined(HI_BUILD_IN_BOOT)
         #define CONFIG_JPEG_HARDDEC2ARGB
        /** should read register, but may be **/
    #endif

    #if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
        /** 低延迟规格，视频通话mjpg解码 **/
        #define CONFIG_JPEG_LOW_DELAY_SUPPORT
    #endif

    /****************************************************************************
     ** function marco definition use to fpga test and deal with bug by eda
     ** CNcomment:用来硬件逻辑测试及定位bug使用的宏开关
     ****************************************************************************/
    /** output the need output os memory size */
    /** CNcomment:输出OS需要的内存大小 */
    //#define CONFIG_JPEG_OUTPUT_NEED_OS_MEMSIZE

    /** test press */
    /** CNcomment:测试反压 */
    //#define CONFIG_JPEG_TEST_CHIP_PRESS

    /**
     ** 默认支持跨4K，FPGA测试要测试到不支持跨4K得情况(会有异常)
     ** 这里的跨4K只的是取地址的时候会有跨4K的情况。打开CBB不支持
     ** 跨4K
     **/
    //#define CONFIG_JPEG_4KDDR_DISABLE
    //#define CONFIG_JPEG_4KDDR_DISABLE_SHOWMSG /** 显示信息 **/

    /** save the scen information,use it to eda simulation */
    /** CNcomment:导现场使能，用来调试用的，现场给逻辑做EDA仿真 */
    //#define CONFIG_JPEG_FPGA_TEST_SAVE_SCEN_ENABLE
    /** save the data to bmp picture */
    /** CNcomment:保存bmp图片 */
    //#define CONFIG_JPEG_TEST_SAVE_BMP_PIC
    /** test hard decode capa */
    /** CNcomment:测试硬件解码性能 */
    //#define CONFIG_JPEG_TEST_HARD_DEC_CAPA
    /** test set the different outstanding value */
    /** CNcomment:测试配置不同的outstanding值,自动比较宏开关打开 */
    //#define CONFIG_JPEG_FPGA_TEST_SET_DIFFERENT_OUTSTANDING_VALUE
    //#define CONFIG_JPEG_SAVE_INPUTSTREAM_DATA

    #ifdef CONFIG_HI_FPGA_JPEG_VERSION
        #define CONFIG_JPEG_CSC_DISABLE
    #endif

    #ifdef CONFIG_GFX_ADVCA_RELEASE
        /** this is not has warning message */
        /** CNcomment:只有带上参数才不会有告警信息 */
        #define JPEG_TRACE( fmt,args...)
    #else
        #ifndef HI_BUILD_IN_BOOT
            #ifdef __KERNEL__
            #define JPEG_TRACE               GFX_Printk
            #else
             #define JPEG_TRACE               GFX_Printf
            #endif
        #else
            #define JPEG_TRACE               printf
        #endif
    #endif

    /** @} */  /*! <!-- Macro Definition end */

    /*************************** Structure Definition ****************************/

    /***************************  The enum of Jpeg image format  ******************/


    /********************** Global Variable declaration **************************/


    /******************************* API declaration *****************************/


    #ifdef __cplusplus

        #if __cplusplus



}

        #endif

   #endif /* __cplusplus */

#endif /* __HI_JPEG_CONFIG_H__*/
