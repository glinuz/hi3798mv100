/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_api.h
Version            : Initial Draft
Author            :
Created            : 2014/06/20
Description        : this include file is cover all api of jpeg dec
                  CNcomment: 内部的解码实现都包含在这个文件中 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162              Created file
******************************************************************************/
#ifndef __JPEG_HDEC_API_H__
#define __JPEG_HDEC_API_H__


/*********************************add include here******************************/
#include  "jpeglib.h"

#include  "hi_type.h"
#include  "hi_jpeg_config.h"
#include  "hi_drv_jpeg.h"
#include  "jpeg_hdec_rwreg.h"
#include  "hi_jpeg_api.h"

#ifdef CONFIG_JPEG_FPGA_TEST_SAVE_SCEN_ENABLE
#include "hi_jpeg_reg.h"
#endif

#ifdef CONFIG_JPEG_TEST_SUPPORT
#include "hi_jpeg_hdec_test.h"
#endif

/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



    /***************************** Macro Definition ******************************/
     /** \addtogroup      JPEG HDEC MACRO */
     /** @{ */  /** <!-- 【JPEG HDEC MACRO】 */

     /** jpeg device */
     /** CNcomment:jpeg 设备 */
     #define JPG_DEV                           "/dev/jpeg"
     #define MMZ_DEV                          "/dev/mmz_userdev"
     #define CSC_DEV                          "/dev/hi_tde"

     #define CLIENT_DATA_MARK                        0x00FFFFFF
     #define CONFIG_JPEG_SCALE_MULTIPLE_8            3


     /** @} */    /*! <!-- Macro Definition end */


     /*************************** Enum Definition ****************************/
    /****************************************************************************/
    /*                               jpeg enum                                    */
    /****************************************************************************/

    /** \addtogroup      JPEG HDEC ENUM */
    /** @{ */  /** <!-- 【JPEG HDEC ENUM】 */

    /** enum of the dec scale */
    /** CNcomment:解码缩放比例 */
    typedef enum tagJPEG_HDEC_SCALE_E
    {
        JPEG_SCALEDOWN_1 = 0,      /**< scanle down 1 *//**<CNcomment:不缩放   */
        JPEG_SCALEDOWN_2 = 1,      /**< scanle down 2 *//**<CNcomment:缩小2倍  */
        JPEG_SCALEDOWN_4 = 2,      /**< scanle down 4 *//**<CNcomment:缩小4倍  */
        JPEG_SCALEDOWN_8 = 3,      /**< scanle down 8 *//**<CNcomment:缩小8倍  */
        JPEG_SCALEDOWN_BUTT
    }JPEG_HDEC_SCALE_E;

    /** @} */  /*! <!-- enum Definition end */

    /*************************** Structure Definition ****************************/
    /****************************************************************************/
    /*                               jpeg api structure                            */
    /****************************************************************************/

    /** \addtogroup      JPEG HDEC STRUCTURE */
    /** @{ */  /** <!-- 【JPEG HDEC STRUCTURE】 */


     /** Structure of stream message，this memory from user alloc */
     /** CNcomment:码流buffer信息，内存来源于用户分配的
                   要是码流内存连续的，有底下几个优点
                   优点一:不需要cpy码流，省非常多的时间
                   优点二:不需要再分配码流buffer，这样省内存
                   优点三:直接全部送给硬件解码，不需要续码流，这样就省了解码中断的时间 */

     /** deal with the data in hard decode */
     /** CNcomment:进入硬件解码之后的码流处理
                   将码流直接读到码流buffer中，而不需要进行拷贝 */
     typedef struct tagJPEG_HDEC_DATA_S
     {
         HI_U32   u32PhyBuf;            /**< use mem data,save these        *//**<CNcomment:码流来源于内存，这个buffer用来保存这些数据         */
         HI_CHAR* pDataVirBuf;          /**< use mem data,save these        *//**<CNcomment:码流来源于内存，这个buffer用来保存这些数据         */
         HI_U32   u32SaveStreamPhyBuf;  /**< save stream physics buffer     *//**<CNcomment:硬件解码要存放的码流物理内存                       */
         HI_CHAR* pSaveStreamVirBuf;    /**< save stream virtual buffer     *//**<CNcomment:硬件解码要存放的码流虚拟内存                       */
         /** add para that deal with the stream buffer return **/
         HI_U32   u32StartBufPhy;        /**< the buffer start phy address  *//**<CNcomment: 码流buffer的起始物理地址                          */
         HI_CHAR* pDataVirBufReturn;    /**< the stream buffer return      *//**<CNcomment: 码流回绕的起始虚拟地址                            */
         HI_S32   s32BufLen;             /**< the buffer len                *//**<CNcomment: 码流buffer大小                                    */
         HI_S32   s32StreamReturnLen;   /**< the stream buffer return len  *//**<CNcomment: 存码流回绕的码流大小                              */
         HI_BOOL  bNeedStreamReturn;    /**< if has stream return          *//**<CNcomment: 是否有码流回绕                                     */
         HI_BOOL  bUserPhyMem;           /**< use user physics memory        *//**<CNcomment:用户码流物理内存                                   */
         HI_BOOL  bUseFileData;          /**< use user file stream           *//**<CNcomment:用户文件码流                                         */
         HI_BOOL  bUseInsideData;        /**< use inside deal withe the data *//**<CNcomment:使用内部处理码流方式                               */
         HI_BOOL  bReadToDataBuf;        /**< if read data to stream buffer  *//**<CNcomment:是否要直接读到该码流buffer中，这个是针对文件来说的 */
         HI_U32   u32ReadDataSize;       /**< read data size one time        *//**<CNcomment:每次读取的码流大小,非用户连续内存                  */
         HI_U64   u64DataSize;           /**< save user stream size           *//**<CNcomment:用户送进行的整帧码流大小                           */
         HI_U32   u32ConsumeDataSize;   /**< the consume stream size before hard decode  *//**<CNcomment:硬件解码之前消耗了多少码流，这个是针对内存码流来说的 */
     }JPEG_HDEC_DATA_S;


    /** when hard decode failure, callback to soft decode need message */
    /** CNcomment:硬件解码不成功回退到软件解码需要的信息 */
    typedef struct tagJPEG_HDEC_TO_SDEC_S
    {
         HI_CHAR* pLeaveBuf;     /**< remain stream buffer        *//**<CNcomment:剩余码流           */
         HI_U32   u32FilePos;    /**< the current file position   *//**<CNcomment:当前读到的文件位置 */
         HI_U32   u32StreamPos;  /**< the current stream position *//**<CNcomment:当前码流的位置     */
         HI_U32   u32LeaveByte;  /**< remain stream size          *//**<CNcomment:剩余的码流数       */
    }JPEG_HDEC_TO_SDEC_S;


    /** the jpeg sof information，the hard decode is only use dec para */
    /** CNcomment:jpeg大小相关信息，硬件解码只有解码分辨率的概念 */
    typedef struct tagJPEG_HDEC_SOFINFO_S
    {
         HI_BOOL bCalcSize;        /**< the jpeg size has  calculated *//**<CNcomment:jpeg大小已经计算完了，不需要重新计算了  */
         HI_U32  u32YWidth;        /**< the lu width             *//**<CNcomment:亮度宽宽度        */
         HI_U32  u32YHeight;       /**< the lu height            *//**<CNcomment:亮度高度          */
         HI_U32  u32YMcuHeight;    /**< the lu height,mcu align  *//**<CNcomment:亮度高度，MCU对齐 */
         HI_U32  u32YSize;         /**< the lu size              *//**<CNcomment:亮度大小          */
         HI_U32  u32CWidth;        /**< the ch width             *//**<CNcomment:色度宽度          */
         HI_U32  u32CHeight;       /**< the ch height            *//**<CNcomment:色度高度          */
         HI_U32  u32CMcuHeight;    /**< the ch height,mcu align  *//**<CNcomment:色度高度，MCU对齐 */
         HI_U32  u32CSize;         /**< the ch size              *//**<CNcomment:色度大小          */
         HI_U32  u32YStride;       /**< the lu stride            *//**<CNcomment:亮度行间距        */
         HI_U32  u32CbCrStride;    /**< the ch stride            *//**<CNcomment:色度行间距        */
         HI_U32  u32DisplayW;      /**< the display width        *//**<CNcomment:显示宽度          */
         HI_U32  u32DisplayH;      /**< the display height       *//**<CNcomment:显示高度          */
         HI_U32  u32DisplayStride; /**< the display stride        *//**<CNcomment:显示行间距        */
         HI_U32  u32DecW;          /**< the dec width            *//**<CNcomment:解码宽度          */
         HI_U32  u32DecH;          /**< the dec height           *//**<CNcomment:解码高度          */
         HI_U32  u32DecStride;     /**< the dec stride            *//**<CNcomment:解码 行间距        */
         HI_U32  u32InWandH;       /**< set to register size     *//**<CNcomment:配给硬件的分辨率  */
         #ifdef CONFIG_JPEG_HARDDEC2ARGB
         HI_U32  u32McuWidth;      /**< the mcu width        */
         HI_U32  u32MINSize;       /**< the min size         */
         HI_U32  u32MIN1Size;      /**< the min1 size        */
         HI_U32  u32RGBSizeReg;    /**< set to register size *//**<CNcomment:配给硬件寄存器的值  */
         #endif
    }JPEG_HDEC_SOFINFO_S;

    /** the jpeg hard decode output middle buffer and csc output middle buffer */
    /** CNcomment:硬件解码输出中间buffer和颜色空间转换输出中间buffer */
    typedef struct tagJPEG_MIDDLE_SURFACE_S
    {
         HI_U32   u32MiddlePhy[MAX_PIXEL_COMPONENT_NUM]; /**< jpeg hard dec output physics buffer *//**<CNcomment:jpeg 硬件解码输出物理地址  */
         HI_CHAR* pMiddleVir[MAX_PIXEL_COMPONENT_NUM];   /**< jpeg hard dec output vitual  buffer *//**<CNcomment:jpeg 硬件解码输出虚拟地址  */
         HI_U32   u32OutPhy;                             /**<  csc output physics buffer          *//**<CNcomment:颜色空间转换输出物理地址   */
         HI_CHAR* pOutVir;                               /**<  csc output virtual buffer          *//**<CNcomment:颜色空间转换输出虚拟地址   */
    }JPEG_MIDDLE_SURFACE_S;

    /** the private structure of hard decode */
    /** CNcomment:硬件解码增加的私有结构体 */
    typedef struct tagJPEG_HDEC_HANDLE_S
    {

          volatile HI_CHAR *pJpegRegVirAddr;  /**< mmap the jpeg virtual address                *//**<CNcomment:jpeg映射上来的虚拟地址         */
          HI_BOOL  bHdecEnd;                  /**< hard decode success                          *//**<CNcomment:硬件解码成功                   */
          HI_BOOL  bCSCEnd;                   /**< color space convert success                  *//**<CNcomment:颜色空间转换成功               */
          HI_BOOL  bOutYCbCrSP;               /**< if output yuvsp                              *//**<CNcomment:是否解码输出yuv semi_planer    */
          HI_BOOL  bFillInput;                  /**< the fill input whether release read stream   *//**<CNcomment: fill input码流填充回调接口是否实现码流填充 */
          HI_U32   u32StrideAlign;            /**< how many bytes the output stride align       *//**<CNcomment: 输出stride几个字节对齐        */
          HI_BOOL  bDecOutColorSpaecXRGB;     /**< the jpeg support dec to argb                 *//**<CNcomment:jpeg硬件支持解码输出ARGB        */
          HI_U32   u32MinPhyBuf;              /**< dec to argb need row buffer                  *//**<CNcomment:解码输出ARGB需要的行buffer      */
          #ifdef CONFIG_JPEG_FPGA_TEST_SAVE_SCEN_ENABLE
          HI_BOOL bSaveScen;                  /**< if you want to save scen, set it             *//**<CNcomment:如果想保存解码现场，设置这个标记 */
          HI_S32  s32RegData[JPGD_REG_LENGTH];
          FILE*   pScenFile;
          HI_CHAR* pScenFileName;
          #endif
          HI_S32  s32LuDcLen[6];              /**< huffman lu dc codelenght                     *//**<CNcomment:亮度DC的最长CodeLength          */
          #ifdef CONFIG_JPEG_OUTPUT_YUV420SP
          HI_BOOL  bOutYUV420SP;              /**< all dec output yuv420sp                      *//**<CNcomment:统一输出yuv420sp格式        */
          #endif
          #ifdef CONFIG_JPEG_OUTPUT_LUPIXSUM
          HI_BOOL  bLuPixSum;                 /**< count the lu pixle sum value                 *//**<CNcomment:统计亮度和信息              */
          HI_U64   u64LuPixValue;             /**< the lu pixle value                           *//**<CNcomment:亮度值和                    */
          #endif
          #ifdef CONFIG_JPEG_GETDECTIME
          HI_U32   u32DecTime;                 /**< calc the decode time                        *//**<CNcomment:计算解码时间                */
          HI_U32   u32CurTime;                 /**< calc the current time                       *//**<CNcomment:获取当前的时间              */
          #endif
          #ifdef CONFIG_JPEG_LOW_DELAY_SUPPORT
          HI_BOOL  bLowDelayEn;
          HI_U32   u32PhyLineBuf;
          HI_CHAR* pVirLineBuf;
          #endif
          HI_S32   s32QuantTab[DCTSIZE2];
          HI_U32   u32HuffDcTab[12];
          HI_U32   u32HufAcMinTab[8];
          HI_U32   u32HufAcBaseTab[8];
          HI_U32   u32HufAcSymbolTab[256];
          HI_S32   s32ClientData;              /**< the client pointer of private structure     *//**<CNcomment:私有结构体指针              */
          HI_S32   s32JpegDev;                   /**< the jpeg device                                *//**<CNcomment:jpeg设备                    */
          HI_U32   u32ScalRation;              /**< the dec scale                               *//**<CNcomment:缩放比例                    */
          HI_U32   u32Inflexion;               /**< the soft and hard dec flexiont              *//**<CNcomment:软解和硬件解码的拐点大小    */
          HI_U32   u32Alpha;                   /**< set alpha value of out ARGB8888 ABGR8888    *//**<CNcomment:设置alpha的值               */
          HI_BOOL  bReleaseRes;                /**< the resouce has release                     *//**<CNcomment:资源已经释放掉了            */
          HI_BOOL  bInHardDec;                 /**< hard dec support and dev open success,now into hard decode                              */
          #ifdef CONFIG_GFX_MEM_ION
          HI_S32   s32MMZDev;                    /**< the mmz device                                *//**<CNcomment:MMZ设备                      */
            HI_VOID *pSaveStreamMemHandle;
          HI_VOID *pMiddleMemHandle;
          HI_VOID *pMinMemHandle;
          HI_VOID *pOutMemHandle;
          #endif
          HI_S32  s32ContinueSize;                 /** 这个参数只有在码流回调的情况下才有用   **/
          HI_BOOL bFirstContinueStream;            /** 这个参数只有在码流回调的情况下才有用   **/
          HI_U32 u32ResByteConsum;                 /**< consume byte after suspend              *//**<CNcomment:待机消耗的码流数   */
          HI_U32 u32NeedMemSize;                   /**< the need memsize                        *//**<CNcomment:需要系统内存大小   */
          HI_U32 u32LeaveMemSize;                   /**< the leave memsize                       *//**<CNcomment:剩余内存大小       */
          HI_U8 u8Fac[MAX_PIXEL_COMPONENT_NUM][2]; /**< save the sample factor                  *//**<CNcomment:保存采样因子大小   */
          #ifdef CONFIG_JPEG_PROC_ENABLE
          HI_JPEG_DEC_STATE_E eDecState;           /**< the decode state                        *//**<CNcomment:解码状态          */
          #endif
          JPEG_HDEC_SOFINFO_S            stJpegSofInfo;     /**< the jpeg sof information               *//**<CNcomment:jpeg图片大小信息           */
          JPEG_HDEC_DATA_S               stHDecDataBuf;     /**< the hard decode data buffer message    *//**<CNcomment:?布饴胄枰穆肓餍畔?     */
          HI_JPEG_FMT_E                     enImageFmt;        /**< jpeg picture type                      *//**<CNcomment:jpeg图片格式               */
          JPEG_HDEC_TO_SDEC_S            stJpegHtoSInfo;    /**< hard dec failure callback to soft dec  *//**<CNcomment:硬解不成功回退到软解的过程 */
          HI_JPEG_DEC_COEF_S             stDecCoef;         /**< hard dec out argb set coef             *//**<CNcomment:解码输出ARGB时候配置的参数 */
          HI_JPEG_SURFACE_DESCRIPTION_S  stOutDesc;         /**< the user output message description    *//**<CNcomment:用户设置的输出信息描述     */
          JPEG_MIDDLE_SURFACE_S          stMiddleSurface;
          #ifdef CONFIG_JPEG_TEST_SUPPORT
          JPEG_TEST_INFO_S               stTestInfo;
          #endif
    }JPEG_HDEC_HANDLE_S,*JPEG_HDEC_HANDLE_S_PTR;


    #ifndef CONFIG_JPEG_CSC_DISABLE
      typedef struct tagJPEG_DECOMPRESS_RES
      {
          HI_S32     s32CscDev;
          HI_CHAR*   pStreamPhyBuf;
          HI_CHAR*   pStreamVirBuf;
      }JPEG_DECOMPRESS_RES;
    #endif

    /** @} */  /*! <!-- Structure Definition end */


    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/

    /** \addtogroup      JPEG HDEC API */
    /** @{ */  /** <!-- 【JPEG HDEC API】 */


    /*****************************************************************************
    * func            : JPEG_HDEC_Init
    * description    : init the private structure para
                      CNcomment: 初始化私有结构体变量    CNend\n
    * param[in]     : cinfo       CNcomment: 解码对象    CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功  CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败  CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_Init(j_common_ptr cinfo);

    HI_VOID JPEG_HDEC_Destroy(const struct jpeg_common_struct *cinfo);

    HI_VOID JPEG_HDEC_Abort(const struct jpeg_common_struct *cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_CloseDev
    * description    : closxe some device that decode need
                      CNcomment: 关闭解码打开的相关设备        CNend\n
    * param[in]     : cinfo         CNcomment: 解码对象    CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_CloseDev(const struct jpeg_common_struct *cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_Crc
    * description    : crc较验，解决解码花瓶的问题
    * param[in]     : pDataVir       CNcomment: 解码对象 CNend\n
    * param[in]     : s32DataSize    CNcomment: 解码对象 CNend\n
    * param[in]     : pbFlag         CNcomment: 解码对象 CNend\n
    * retval        : HI_SUCCESS     CNcomment: 成功     CNend\n
    * retval        : HI_FAILURE     CNcomment: 失败     CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_Crc(HI_CHAR* pDataVir,HI_S32 s32DataSize,HI_BOOL* pbFlag);

    /*****************************************************************************
    * func            : JPEG_HDEC_CheckStreamMemType
    * description    : check the stream buffer type, if user no call the function
                      of set stream buffer type,call this function
                      CNcomment: 查询码流buffer类型，是连续的物理内存，还是虚拟
                      内存，要是用户没有调用设置码流buffer类型，调用该接口
    * param[in]     : cinfo       CNcomment: 解码对象  CNend\n
    * param[in]     : pVirBuf     CNcomment: 虚拟内存  CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功      CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败      CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_CheckStreamMemType(const struct jpeg_decompress_struct *cinfo,HI_UCHAR* pVirBuf);

    /*****************************************************************************
     * func          : JPEG_HDEC_Start
     * description   : start jpeg hard decode
                       CNcomment: 开始硬件解码
     * param[in]     : cinfo       CNcomment: 解码对象   CNend\n
     * retval        : HI_TRUE     CNcomment: 成功       CNend\n
     * retval        : HI_FALSE    CNcomment: 失败       CNend\n
     * others:       : NA
     *****************************************************************************/
     HI_BOOL JPEG_HDEC_Start(j_decompress_ptr cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_HardCSC
    * description    : use hard csc
                      CNcomment: 使用硬件进行颜色空间转换 CNend\n
    * param[in]     : cinfo      CNcomment: 解码对象 CNend\n
    * retval        : HI_SUCCESS CNcomment: 成功     CNend\n
    * retval        : HI_FAILURE CNcomment: 失败     CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_HardCSC(j_decompress_ptr cinfo);


    #ifdef CONFIG_JPEG_SOFTCSC_ENABLE
    /*****************************************************************************
    * func            : JPEG_HDEC_SoftCSC
    * description    : use soft csc, is use to debug the hard csc
                      CNcomment: 使用软件进行颜色空间转换,用来定位硬件转换的问题 CNend\n
    * param[in]     : cinfo      CNcomment: 解码对象  CNend\n
    * retval        : HI_SUCCESS CNcomment: 成功      CNend\n
    * retval        : HI_FAILURE CNcomment: 失败      CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_SoftCSC(j_decompress_ptr cinfo);
    #endif


    /*****************************************************************************
    * func            : JPEG_HDEC_OutUserBuf
    * description    : output the scanlines buffer
                      CNcomment:  输出到用户行buffer中 CNend\n
    * param[in]     : cinfo      CNcomment:  解码对象  CNend\n
    * param[out]    : max_lines  CNcomment:  解码行数  CNend\n
    * param[out]    : scanlines  CNcomment:  行buffer  CNend\n
    * retval        : HI_SUCCESS CNcomment:  成功      CNend\n
    * retval        : HI_FAILURE CNcomment:  失败      CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_OutUserBuf(j_decompress_ptr cinfo,JDIMENSION max_lines, HI_CHAR *scanlines);


     /*****************************************************************************
    * func            : JPEG_HDEC_GetMemFromMemInfo
    * description    : get leave memory from cat /proc/meminfo
                      CNcomment: 从内存信息中获取剩余内存大小 CNend\n
    * param[out]    : pu64MemSize  CNcomment:  内存大小  CNend\n
    * retval        : HI_SUCCESS   CNcomment:  成功      CNend\n
    * retval        : HI_FAILURE   CNcomment:  失败      CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_GetMemFromMemInfo(HI_U64 *pu64MemSize);


    HI_VOID JPEG_HDEC_SetLowDelayBufferDate(j_decompress_ptr cinfo);

    /** @} */  /*! <!-- API declaration end */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __JPEG_HDEC_API_H__*/
