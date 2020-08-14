/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : hi_jpeg_hdec_test.h
Version            : Initial Draft
Author            :
Created            : 2014/06/20
Description        : The user will use this api to realize some function
                  这个是专门用来测试使用的
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162              Created file
******************************************************************************/

#ifndef __HI_JPEG_HDEC_TEST_H__
#define __HI_JPEG_HDEC_TEST_H__


/*********************************add include here******************************/

#include "jpeglib.h"
#include "hi_type.h"

#include "hi_jpeg_config.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



     /***************************** Macro Definition ******************************/
     /** \addtogroup      JPEG */
     /** @{ */  /** <!-- 【JPEG】 */

#ifdef CONFIG_JPEG_FPGA_TEST_SAVE_SCEN_ENABLE
     #define  CV200_JPEG_BASE    0xf8c40000
     #define  PRN_SCENE(pFile,addr,data)  fprintf(pFile, "W 0x%08x 0x%08x word single\n",addr,data)
    /**
     **android可以放置的路径为 cd /data/ mkdir register_scen
     **则 /data/register_scen/.scen
     **/
     #define  SCEN_FILE_NAME                "./test/res/test_data_dec_log_file/scen_file/output_file.scen"
     #define  SCEN_PRINT                     fprintf
#endif


#ifdef CONFIG_JPEG_TEST_SAVE_BMP_PIC
    #ifndef CONFIG_JPEG_ANDROID_SAVE
        #define BMP_DATA_FILE_DIR            "./../res/test_save_data_bmp"
    #else/** Android **/
        #define BMP_DATA_FILE_DIR            "/mnt/sdcard/Movies/test_save_data_bmp"
    #endif
#endif

     /** @} */    /*! <!-- Macro Definition end */


     /*************************** Enum Definition ****************************/
    /****************************************************************************/
    /*                               jpeg enum                                    */
    /****************************************************************************/

    /** \addtogroup      JPEG */
    /** @{ */  /** <!-- 【JPEG】 */

    /** @} */  /*! <!-- enum Definition end */

    /*************************** Structure Definition ****************************/
    /****************************************************************************/
    /*                               jpeg api structure                            */
    /****************************************************************************/

    /** \addtogroup      JPEG */
    /** @{ */  /** <!-- 【JPEG】 */

#ifdef CONFIG_JPEG_TEST_SUPPORT
    /** support jpeg auto test */
    typedef struct tagJPEG_TEST_INFO_S
    {
        HI_BOOL bExitBeforeOpenDev;        /**< exit before open device to soft decode       */
        HI_BOOL bExitAfterOpenDev;         /**< exit after open device to soft decode        */
        HI_BOOL bExitBeforeGetDev;         /**< exit before get device to soft decode        */
        HI_BOOL bExitAfterGetDev;          /**< exit after  get device to soft decode        */
        HI_BOOL bExitContinueDec;          /**< exit when continue interrupt to soft decode  */
        HI_BOOL bDiscardClose;             /**< discard close, can decode next frame img     */
    }JPEG_TEST_INFO_S;
#endif

    /** @} */  /*! <!-- Structure Definition end */


    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/


     /** \addtogroup      JPEG */
     /** @{ */  /** <!-- 【JPEG】 */

#ifdef CONFIG_JPEG_TEST_SUPPORT
     /*****************************************************************************
     * func          : HI_JPEG_SetTestInfo
     * description     : set test decode information
                       CNcomment: 设置要测试的解码器信息 CNend\n
     * param[in]     : cinfo.            CNcomment:解码对象           CNend\n
     * param[in]     : stTestInfo.        CNcomment:要调试的信息       CNend\n
     * retval         : NA
     * others:         : NA
     *****************************************************************************/
     HI_S32 HI_JPEG_SetTestInfo(const struct jpeg_decompress_struct *cinfo,JPEG_TEST_INFO_S stTestInfo);
#endif

     /*****************************************************************************
     * func          : HI_JPEG_OpenScenFile
     * description     : open the scen file
                       CNcomment: 打开导现场的文件 CNend\n
     * param[in]     : cinfo.        CNcomment:解码对象       CNend\n
     * retval         : NA
     * others:         : NA
     *****************************************************************************/
     HI_S32 HI_JPEG_OpenScenFile(const struct jpeg_decompress_struct *cinfo);


    /*****************************************************************************
    * func            : HI_JPEG_CloseScenFile
    * description    : close the scen file
                      CNcomment: 关闭导现场的文件 CNend\n
    * param[in]     : cinfo.       CNcomment:解码对象      CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_CloseScenFile(const struct jpeg_decompress_struct *cinfo);

    /*****************************************************************************
    * func            : HI_JPEG_OutScenData
    * description    : get the scen data
                      CNcomment: 获取现场数据 CNend\n
    * param[in]     : cinfo.                 CNcomment:解码对象         CNend\n
    * param[in]     : pStreamStartBuf.         CNcomment:码流起始地址     CNend\n
    * param[in]     : pStreamEndBuf.         CNcomment:码流结束地址     CNend\n
    * param[in]     : pData.                 CNcomment:数据地址         CNend\n
    * param[in]     : u64DataSize.             CNcomment:数据大小         CNend\n
    * param[in]     : bStartFirst             CNcomment:第一次启动解码    CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_OutScenData(const struct jpeg_decompress_struct *cinfo, \
                                             HI_CHAR* pStreamStartBuf,                      \
                                             HI_CHAR* pStreamEndBuf,                      \
                                             HI_CHAR* pData,                              \
                                             HI_U64  u64DataSize,                          \
                                             HI_BOOL bStartFirst);


    /*****************************************************************************
    * func            : HI_JPEG_SetSaveScen
    * description    : if the decode failure, we want to debug should need the decode\n
                      scen,and use eda simulation.
                      CNcomment: 如果解码失败我们希望用EDA仿真进行定位，需要保存解码现\n
                                 场用来调试使用 CNend\n
    * param[in]     :cinfo.       CNcomment:解码对象     CNend\n
    * param[in]     :bSaveScen.   CNcomment:是否保存现场 CNend\n
    * param[in]     :pFileName    CNcomment:解码文件     CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功         CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败         CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 HI_JPEG_SetSaveScen(const struct jpeg_decompress_struct *cinfo,HI_BOOL bSaveScen,HI_CHAR* pFileName);

    /*****************************************************************************
    * func            : HI_JPEG_GetDev
    * description    : get the jpeg device.
                      CNcomment: 获取jpeg设备 CNend\n
    * param[in]     : s32JpegDev CNcomment: 打开的jpeg节点 CNend\n
    * retval        : HI_SUCCESS
    * retval        : HI_FAILURE
    * others:        : NA
    *****************************************************************************/
    HI_S32 HI_JPEG_GetDev(HI_S32 s32JpegDev);

    /*****************************************************************************
    * func            : HI_JPEG_Suspend
    * description    : test suspend.
                      CNcomment: 待机处理 CNend\n
    * param[in]        : s32JpegDev
    * param[ou]        : *bSendSuspendSignal
    * retval        : HI_SUCCESS
    * retval        : HI_FAILURE
    * others:        : NA
    *****************************************************************************/
    HI_S32 HI_JPEG_Suspend(HI_S32 s32JpegDev,HI_BOOL *bSendSuspendSignal);

    /*****************************************************************************
    * func            : HI_JPEG_Resume
    * description    : test resume.
                      CNcomment: 待机唤醒处理 CNend\n
    * param[in]     : s32JpegDev
    * param[ou]        : *bSendSuspendSignal
    * retval        : HI_SUCCESS
    * retval        : HI_FAILURE
    * others:        : NA
    *****************************************************************************/
    HI_S32 HI_JPEG_Resume(HI_S32 s32JpegDev,HI_BOOL *bSendSuspendSignal);


    /*****************************************************************************
    * func            : HI_JPEG_SaveBmp
    * description    : save the phy data to bmp picture.
                      CNcomment: 保存解码后的数据到bmp图片 CNend\n
    * param[in]        : pDataPhy
    * param[in]        : u32Width
    * param[in]        : u32Height
    * param[in]        : u32Stride
    * param[in]        : cinfo
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_SaveBmp(HI_U32 u32DataPhy,HI_U32 u32Width,HI_U32 u32Height,HI_U32 u32Stride,const struct jpeg_decompress_struct *cinfo);


    /*****************************************************************************
    * func            : HI_JPEG_GetIfHardDec2ARGB8888
    * description    : get if use jpeg hard decode to ARGB888,is not tde csc to ARGB8888
                      CNcomment: 获取是否使用JPEG硬件解码解成ARGB8888，这个时候是用解码分辨率的
                                 真正的数据是显示分辨率，所以JPEG解码要裁剪成显示分辨率大小的，
                                 这样输出数据就正确了。否则不设置裁剪会有多余数据的 CNend\n
    * param[in]        : cinfo                    解码对象
    * param[in]        : pbJpegHardDecARGB8888    是否为JPEG硬件解码输出ARGB8888
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_GetIfHardDec2ARGB8888(const struct jpeg_decompress_struct *cinfo,HI_BOOL *pbJpegHardDecARGB8888);



    /*****************************************************************************
    * func            : HI_JPEG_IfHardDec
    * description    : whether is hard decode or soft decode
                      CNcomment: 是硬件解码还是软件解码 CNend\n
    * param[in]        : cinfo       解码对象
    * param[in]        : pHardDec    是否为硬件解码
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_IfHardDec(const struct jpeg_decompress_struct *cinfo,HI_BOOL *pHardDec);


#ifdef CONFIG_JPEG_TEST_CHIP_RANDOM_RESET

   /*****************************************************************************
    * func            : HI_JPEG_HardDecNow
    * description    : whether has hard decode now
                      CNcomment: 此刻是否启动了硬件解码 CNend\n
    * retval        : HI_SUCCESS;
    * retval        : HI_FAILURE;
    * others:        : NA
    *****************************************************************************/
    HI_S32 HI_JPEG_HardDecNow();

   /*****************************************************************************
    * func            : HI_JPEG_RandomReset
    * description    : reset the hard decode register
                      CNcomment: 复位硬件解码寄存器 CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_RandomReset();

   /*****************************************************************************
    * func            : HI_JPEG_SetDecState
    * description    : set hard decode state
                      CNcomment: 设置解码状态 CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_SetDecState(HI_BOOL bDecState);

    /*****************************************************************************
    * func            : HI_JPEG_SetJpegDev
    * description    : set Jpeg Dev
                      CNcomment: 设置解码设备值 CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_SetJpegDev(HI_S32 s32JpegDev);

    /*****************************************************************************
    * func            : HI_JPEG_SetJpegVir
    * description    : set Jpeg map virtual value
                      CNcomment: 设置映射虚拟地址 CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_SetJpegVir(volatile HI_CHAR *pRegisterVir);

    /*****************************************************************************
    * func            : HI_JPEG_RandomResetInit
    * description    : init the random reset valure
                      CNcomment: 初始化随机软复位测试的相关参数值 CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_RandomResetInit();

#endif


    /*****************************************************************************
    * func            : HI_JPEG_GetRegTime
    * description    : get the jpeg hard register time value
                      CNcomment: 获取硬件解码寄存器的性能值 CNend\n
    * param[in]     : cinfo         解码对象
    * param[in]     : u32DecTime    寄存器解码时间
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_GetRegTime(const struct jpeg_decompress_struct *cinfo,HI_U32 *u32DecTime);



#ifdef CONFIG_JPEG_FPGA_TEST_CONTINUE_STREAM_DDR_CHANGE
    /*****************************************************************************
    * func            : HI_JPEG_SetConStreamDDRChangeFlag
    * description    : if the continue stream ddr change
                      CNcomment: 续码流地址是否发生变化 CNend\n
    * param[in]     : bConDDRChange         设置发生变化标志
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_SetConStreamDDRChangeFlag(HI_BOOL bConDDRChange);
    /*****************************************************************************
    * func            : HI_JPEG_GetConStreamDDRChangeFlag
    * description    : get the whether the continue stream ddr change
                       CNcomment: 获取续码流地址是否发生变化标志 CNend\n
    * param[in]     : *pbConDDRChange         获取标记状态
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_GetConStreamDDRChangeFlag(HI_BOOL *pbConDDRChange);

#endif

#ifdef CONFIG_JPEG_FPGA_TEST_SET_DIFFERENT_OUTSTANDING_VALUE
    /*****************************************************************************
    * func            : HI_JPEG_SetOutstandingValue
    * description    : set the outstanding value
                      CNcomment: 设置outstanding的值 CNend\n
    * param[in]     : s32OutstandingValue    要设置的值大小
    * param[in]     : bSetFlag                要设置标志
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_SetOutstandingValue(HI_S32 s32OutstandingValue,HI_BOOL bSetFlag);

    /*****************************************************************************
    * func            : HI_JPEG_GetOutstandingValue
    * description    : get the outstanding value
                      CNcomment: 获取outstanding 的值大小 CNend\n
    * param[in]     : *ps32OutstandingValue         要获取的值
    * param[in]     : *pbSetFlag                    要是设置的标志
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID HI_JPEG_GetOutstandingValue(HI_S32 *ps32OutstandingValue,HI_BOOL *pbSetFlag);

#endif

    /** @} */  /*! <!-- API declaration end */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __HI_JPEG_HDEC_TEST_H__*/
