/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : hi_jpeg_hdec_test.c
Version            : Initial Draft
Author            : y00181162
Created            : 2014/06/20
Description        : the test need some function
                  CNcomment: 测试用例需要的一些对外函数 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162            Created file
******************************************************************************/

/*********************************add include here******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "jpeglib.h"

#include "jpeg_hdec_rwreg.h"
#include "jpeg_hdec_api.h"
#include "hi_jpeg_hdec_test.h"
#include "hi_jpeg_reg.h"


#if defined(CONFIG_JPEG_TEST_SAVE_BMP_PIC) || defined(CONFIG_JPEG_TEST_SAVE_YUVSP_DATA)
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/unistd.h>
#endif

#if 0
#include <utils/Log.h>
#define LOG_TAG           "libjpeg"
#endif

/***************************** Macro Definition ******************************/
#ifdef CONFIG_JPEG_FPGA_TEST_SET_DIFFERENT_OUTSTANDING_VALUE
static HI_S32 gs_s32OutstandingValue = 0xffffff3f;
static HI_BOOL gs_bOutstandingFlag   = HI_FALSE;
#endif

/******************** to see which include file we want to use***************/



/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/




/******************************* API forward declarations *******************/

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
 HI_S32 HI_JPEG_SetTestInfo(const struct jpeg_decompress_struct *cinfo,JPEG_TEST_INFO_S stTestInfo)
{
    JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    if(NULL == pJpegHandle){
        return HI_FAILURE;
    }
    pJpegHandle->stTestInfo.bExitBeforeOpenDev = stTestInfo.bExitBeforeOpenDev;
    pJpegHandle->stTestInfo.bExitAfterOpenDev  = stTestInfo.bExitAfterOpenDev;
    pJpegHandle->stTestInfo.bExitBeforeGetDev  = stTestInfo.bExitBeforeGetDev;
    pJpegHandle->stTestInfo.bExitAfterGetDev   = stTestInfo.bExitAfterGetDev;
    pJpegHandle->stTestInfo.bExitContinueDec   = stTestInfo.bExitContinueDec;
    pJpegHandle->stTestInfo.bDiscardClose      = stTestInfo.bDiscardClose;
    return HI_SUCCESS;
}
#endif


#ifdef CONFIG_JPEG_FPGA_TEST_SAVE_SCEN_ENABLE

/*****************************************************************************
* func            : HI_JPEG_OpenScenFile
* description    : open the scen file
                  CNcomment: 打开导现场的文件 CNend\n
* param[in]        : cinfo.       CNcomment:解码对象     CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_S32 HI_JPEG_OpenScenFile(const struct jpeg_decompress_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     char pFileName[256];
     char pNum[20];
     static HI_S32 s32Cnt = 0;

     if (NULL != pJpegHandle->pScenFile){
        return HI_SUCCESS;
     }

     snprintf(pNum,sizeof(pNum), "%d", s32Cnt);
     strncpy(pFileName, SCEN_FILE_NAME,strlen(SCEN_FILE_NAME));
     pFileName[strlen(SCEN_FILE_NAME)] = '\0';
     strncat(pFileName,pNum,strlen(pNum));

     pJpegHandle->pScenFile = fopen(pFileName,"wb+");
     if (NULL == pJpegHandle->pScenFile){
         JPEG_TRACE("open scen file failure\n");
        return HI_FAILURE;
     }
     s32Cnt++;

     SCEN_PRINT(pJpegHandle->pScenFile,"=====================================================\n");
     SCEN_PRINT(pJpegHandle->pScenFile,"the dec file name is %s\n",pJpegHandle->pScenFileName);
     SCEN_PRINT(pJpegHandle->pScenFile,"=====================================================\n");

     return HI_SUCCESS;

}

/*****************************************************************************
* func            : HI_JPEG_CloseScenFile
* description    : close the scen file
                  CNcomment: 关闭导现场的文件 CNend\n
* param[in]        : cinfo.       CNcomment:解码对象     CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID HI_JPEG_CloseScenFile(const struct jpeg_decompress_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (NULL != pJpegHandle->pScenFile){
         fclose(pJpegHandle->pScenFile);
     }
     pJpegHandle->pScenFile = NULL;
}


/*****************************************************************************
* func            : HI_JPEG_GetScenData
* description    : get scen data.
                  CNcomment: 获取现场数据 CNend\n
* param[in]        : cinfo.       CNcomment:解码对象     CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
static HI_VOID HI_JPEG_GetScenData(const struct jpeg_decompress_struct *cinfo)
{

        HI_S32 offset      = 0;
        HI_S32 cnt         = 0;

        JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

        /* 寄存器(含表) */
        for (offset = 0xc; offset <= 0x6bc; offset+=4){

            if(JPGD_REG_STADD == offset){
                cnt++;
            }else if(JPGD_REG_ENDADD == offset){
                cnt++;
            }else{
                pJpegHandle->s32RegData[cnt] = JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,offset);
                cnt++;
            }
        }

}

/*****************************************************************************
* func            : HI_JPEG_PrintScenData
* description    : export the scen data.
                  CNcomment: 导出现场数据 CNend\n
* param[in]        : cinfo.                 CNcomment:解码对象         CNend\n
* param[in]        : pStreamStartBuf.       CNcomment:码流起始地址     CNend\n
* param[in]        : pStreamEndBuf.         CNcomment:码流结束地址     CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
static HI_VOID HI_JPEG_PrintScenData(const struct jpeg_decompress_struct *cinfo,HI_CHAR* pStreamStartBuf,HI_CHAR* pStreamEndBuf)
{

        HI_S32 offset      = 0;
        HI_S32 cnt        = 0;

        JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

        if(NULL != pJpegHandle->pScenFile){
            SCEN_PRINT(pJpegHandle->pScenFile,"=====================================================\n");
            SCEN_PRINT(pJpegHandle->pScenFile,"   the register data \n");
            SCEN_PRINT(pJpegHandle->pScenFile,"=====================================================\n");
            /* 寄存器(含表)，从VAHB_STRIDE开始 */
            for (offset = 0xc; offset <= 0x6bc; offset+=4){

                if(JPGD_REG_STADD == offset){
                    PRN_SCENE(pJpegHandle->pScenFile,CV200_JPEG_BASE + offset, (HI_U32)pStreamStartBuf);
                    cnt++;
                }else if(JPGD_REG_ENDADD == offset){
                  /**
                  **这个是码流buffer,一直在发生改变，所以这个数据不准确
                  **/
                    PRN_SCENE(pJpegHandle->pScenFile,CV200_JPEG_BASE + offset, (HI_U32)pStreamEndBuf);
                    cnt++;
                }else{
                    PRN_SCENE(pJpegHandle->pScenFile,CV200_JPEG_BASE + offset, pJpegHandle->s32RegData[cnt]);
                    cnt++;
                }
            }
        }


}

/*****************************************************************************
* func            : HI_JPEG_OutScenData
* description    : get the scen data
                  CNcomment: 获取现场数据 CNend\n
* param[in]        : cinfo.                 CNcomment:解码对象         CNend\n
* param[in]        : pStreamStartBuf.       CNcomment:码流起始地址     CNend\n
* param[in]        : pStreamEndBuf.         CNcomment:码流结束地址     CNend\n
* param[in]        : pData.                 CNcomment:数据地址         CNend\n
* param[in]        : s32DataSize.           CNcomment:数据大小         CNend\n
* param[in]     : bStartFirst            CNcomment:第一次启动解码   CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID HI_JPEG_OutScenData(const struct jpeg_decompress_struct *cinfo, \
                                         HI_CHAR* pStreamStartBuf,                    \
                                         HI_CHAR* pStreamEndBuf,                      \
                                         HI_CHAR* pData,                              \
                                         HI_U64  u64DataSize,                         \
                                         HI_BOOL bStartFirst)
{

         HI_U32 data        = 0;
         HI_S32 offset       = 0;

         JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

         /**
          ** output the scen message to file
          ** CNcomment:导解码现场 CNend\n
          **/
          HI_JPEG_GetScenData(cinfo);
          HI_JPEG_PrintScenData(cinfo,pStreamStartBuf,pStreamEndBuf);
          SCEN_PRINT(pJpegHandle->pScenFile,"=====================================================\n");
          SCEN_PRINT(pJpegHandle->pScenFile,"    the dec data \n");
          SCEN_PRINT(pJpegHandle->pScenFile,"=====================================================\n");
          SCEN_PRINT(pJpegHandle->pScenFile,"======================= data begin ==================\n\n");
          for(offset = 0; offset < u64DataSize; offset+=4){
               data =    (pData[offset+3] << 24)
                      | (pData[offset+2] << 16)
                      | (pData[offset+1] << 8)
                      | (pData[offset]);

               PRN_SCENE(pJpegHandle->pScenFile,(HI_U32)(pStreamStartBuf+offset),data);
               /**
                **现场从左往右读，这里可能比实际多1或2或3个字节
                **/

          }
          if(HI_TRUE == bStartFirst){
              PRN_SCENE(pJpegHandle->pScenFile,CV200_JPEG_BASE,0x00000001);
          }else{
              PRN_SCENE(pJpegHandle->pScenFile,CV200_JPEG_BASE+4,0x00000001);
          }
          SCEN_PRINT(pJpegHandle->pScenFile,"\n======================= data end ==================\n");


}

/*****************************************************************************
* func            : HI_JPEG_SetSaveScen
* description    : if the decode failure, we want to debug should need the decode\n
                  scen,and use eda simulation.
                  CNcomment: 如果解码失败我们希望用EDA仿真进行定位，需要保存解码现\n
                             场用来调试使用 CNend\n
* param[in]        :cinfo.       CNcomment:解码对象     CNend\n
* param[in]        :bSaveScen.   CNcomment:是否保存现场 CNend\n
* param[in]     :pFileName    CNcomment:解码文件     CNend\n
* retval        : HI_SUCCESS  CNcomment: 成功         CNend\n
* retval        : HI_FAILURE  CNcomment: 失败         CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 HI_JPEG_SetSaveScen(const struct jpeg_decompress_struct *cinfo,HI_BOOL bSaveScen,HI_CHAR* pFileName)
{

    JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    pJpegHandle->bSaveScen = bSaveScen;
    pJpegHandle->pScenFileName = pFileName;

    return HI_SUCCESS;

}
#endif

/*****************************************************************************
* func            : HI_JPEG_GetDev
* description    : get the jpeg device.
                  CNcomment: 获取jpeg设备 CNend\n
* param[in]     : s32JpegDev CNcomment: 打开的jpeg节点 CNend\n
* retval        : HI_SUCCESS
* retval        : HI_FAILURE
* others:        : NA
*****************************************************************************/
HI_S32 HI_JPEG_GetDev(HI_S32 s32JpegDev)
{
    return ioctl(s32JpegDev, CMD_JPG_GETDEVICE);
}


/*****************************************************************************
* func            : HI_JPEG_SaveBmp
* description    : save the phy data to bmp picture.
                  CNcomment: 保存解码后的数据到bmp图片
                             RGB565,ARGB4444,ARGB1555,RGB888 and ARGB8888 CNend\n
* param[in]        : u32DataPhy
* param[in]        : u32Width
* param[in]        : u32Height
* param[in]        : u32Stride
* param[in]        : cinfo
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID HI_JPEG_SaveBmp(HI_U32 u32DataPhy,HI_U32 u32Width,HI_U32 u32Height,HI_U32 u32Stride,const struct jpeg_decompress_struct *cinfo)
{
#ifdef CONFIG_JPEG_TEST_SAVE_BMP_PIC
     HI_CHAR exec[256];
     HI_CHAR pColorSpace[256];
     HI_CHAR *pFile = "data";
     HI_CHAR pFileName[256];
     static HI_U32 u32Cnt = 0;

     switch(cinfo->out_color_space){
         case JCS_ARGB_8888:
             strncpy(pColorSpace,"ABGR8888",strlen("ABGR8888"));
            pColorSpace[strlen("ABGR8888")] = '\0';
            break;
         case JCS_ABGR_8888:
         case JCS_RGBA_8888:
             strncpy(pColorSpace,"ARGB8888",strlen("ARGB8888"));
            pColorSpace[strlen("ARGB8888")] = '\0';
            break;
         case JCS_ARGB_1555:
             strncpy(pColorSpace,"ABGR1555",strlen("ABGR1555"));
            pColorSpace[strlen("ABGR1555")] = '\0';
            break;
         case JCS_ABGR_1555:
             strncpy(pColorSpace,"ARGB1555",strlen("ARGB1555"));
            pColorSpace[strlen("ARGB1555")] = '\0';
            break;
          case JCS_RGB:
          case JCS_YCbCr:
              strncpy(pColorSpace,"BGR888",strlen("BGR888"));
            pColorSpace[strlen("BGR888")] = '\0';
              break;
          case JCS_BGR:
          case JCS_CrCbY:
              strncpy(pColorSpace,"RGB888",strlen("RGB888"));
            pColorSpace[strlen("RGB888")] = '\0';
              break;
          case JCS_RGB_565:
              strncpy(pColorSpace,"BGR565",strlen("BGR565"));
            pColorSpace[strlen("BGR565")] = '\0';
              break;
          case JCS_BGR_565:
              strncpy(pColorSpace,"RGB565",strlen("RGB565"));
            pColorSpace[strlen("RGB565")] = '\0';
              break;
          default:
              break;
     }
     /**
      **android可以放置的路径为 cd /data
      **则 /data/./bmpGen, /data/.bmp
      **/
#ifdef CONFIG_JPEG_ANDROID_SAVE
     snprintf(exec, 256, "/data/./bmpGen 0x%x %d %d %d %s  %s/%s%d.bmp",
              u32DataPhy,u32Width,u32Height,u32Stride,pColorSpace,BMP_DATA_FILE_DIR,pFile,u32Cnt);
#else
     snprintf(exec, 256, "./bmpGen 0x%x %d %d %d %s  %s/%s%d.bmp",
              u32DataPhy,u32Width,u32Height,u32Stride,pColorSpace,BMP_DATA_FILE_DIR,pFile,u32Cnt);
#endif

     snprintf(pFileName,256,"%s/%s%d.bmp",BMP_DATA_FILE_DIR,pFile,u32Cnt);

     system(exec);

     u32Cnt++;

     #if 0
     JPEG_TRACE("\n==================================================================================================\n");
     JPEG_TRACE("has save the data to bmp picture,please press enter to save next picture!\n");
     JPEG_TRACE("==================================================================================================\n");
     getchar();
     #endif

     #if 0
     JPEG_TRACE("\n==================================================================================================\n");
     JPEG_TRACE("has save the data to bmp picture,please press enter to save next picture!\n");
     JPEG_TRACE("exec = %s\n",exec);
     JPEG_TRACE("==================================================================================================\n");
     #endif

     //mkdir("./../res/test_save_data_bmp/", 0775 )
     //chmod(pFileName, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
     //chown("./../res/test_save_data_bmp/", 0, 0);
#else
     /** deal with warning **/
    if(cinfo->is_decompressor || 0 == u32DataPhy || 0 == u32Width || 0 == u32Height || 0 == u32Stride)
        return;
#endif
}


/*****************************************************************************
* func            : HI_JPEG_GetIfHardDec2ARGB8888
* description    : get if use jpeg hard decode to ARGB888,is not tde csc to ARGB8888
                  CNcomment: 获取是否使用JPEG硬件解码解成ARGB8888，这个时候是用解码分辨率的
                             真正的数据是显示分辨率，所以JPEG解码要裁剪成显示分辨率大小的，
                             这样输出数据就正确了。否则不设置裁剪会有多余数据的 CNend\n
* param[in]     : cinfo                    解码对象
* param[in]     : pbJpegHardDecARGB8888    是否为JPEG硬件解码输出ARGB8888
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID HI_JPEG_GetIfHardDec2ARGB8888(const struct jpeg_decompress_struct *cinfo,HI_BOOL *pbJpegHardDecARGB8888)
{
    /** 是否使用JPEG硬件解码 **/
    #ifdef CONFIG_JPEG_HARDDEC2ARGB
    JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    *pbJpegHardDecARGB8888 = pJpegHandle->bDecOutColorSpaecXRGB;
    #else
        if(cinfo->progressive_mode)/** deal with warning **/
            *pbJpegHardDecARGB8888 = HI_FALSE;
        else
            *pbJpegHardDecARGB8888 = HI_FALSE;
    #endif
}


/*****************************************************************************
* func            : HI_JPEG_IfHardDec
* description    : whether is hard decode or soft decode
                  CNcomment: 是硬件解码还是软件解码 CNend\n
* param[in]        : cinfo       解码对象
* param[in]        : pHardDec    是否为硬件解码
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID HI_JPEG_IfHardDec(const struct jpeg_decompress_struct *cinfo,HI_BOOL *pHardDec)
{

    JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    if(HI_TRUE == pJpegHandle->bHdecEnd){
       *pHardDec  = HI_TRUE;
    }else{
       *pHardDec  = HI_FALSE;
    }

}

/*****************************************************************************
* func            : HI_JPEG_GetRegTime
* description    : get the jpeg hard register time value
                  CNcomment: 获取硬件解码寄存器的性能值 CNend\n
* param[in]        : cinfo         解码对象
* param[in]        : u32DecTime    寄存器解码时间
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID HI_JPEG_GetRegTime(const struct jpeg_decompress_struct *cinfo,HI_U32 *u32DecTime)
{
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    *u32DecTime  = JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr, JPGD_REG_TIME);
}

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
HI_VOID HI_JPEG_SetOutstandingValue(HI_S32 s32OutstandingValue,HI_BOOL bSetFlag)
{
    gs_s32OutstandingValue = s32OutstandingValue;
    gs_bOutstandingFlag    = bSetFlag;
}
/*****************************************************************************
* func            : HI_JPEG_GetOutstandingValue
* description    : get the outstanding value
                  CNcomment: 获取outstanding 的值大小 CNend\n
* param[in]     : *ps32OutstandingValue         要获取的值
* param[in]     : *pbSetFlag                    要是设置的标志
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID HI_JPEG_GetOutstandingValue(HI_S32 *ps32OutstandingValue,HI_BOOL *pbSetFlag)

{
    *ps32OutstandingValue = gs_s32OutstandingValue;
    *pbSetFlag = gs_bOutstandingFlag;
}
#endif
