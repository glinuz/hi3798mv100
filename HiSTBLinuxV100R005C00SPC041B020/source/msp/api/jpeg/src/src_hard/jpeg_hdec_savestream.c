/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_savestream.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      :
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/

/***************************** add include here*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpeglib.h"
#include "jpeg_hdec_api.h"

/***************************** Macro Definition ********************************/
//#define CONFIG_JPEG_SAVE_YUVSP_DATA
//#define CONFIG_JPEG_SAVE_YUVSP_ANDROID_DIR
//#define CONFIG_JPEG_SAVE_STREAM_ANDROID_DIR


#ifdef CONFIG_JPEG_SAVE_YUVSP_DATA
   #define DADA_WRITE( file,fmt, args... )\
         do { \
                 fprintf(file,fmt, ##args );\
         } while (0)
    #ifdef CONFIG_JPEG_SAVE_YUVSP_ANDROID_DIR
        #define CAPTURE_YUVSP_START             "/mnt/sdcard/Movies/capture_yuvsp_start"
        #define YUVSP_DATA_FILE_DIR             "/mnt/sdcard/Movies"
    #else
        #define CAPTURE_YUVSP_START             "./../res/test_save_data_yuvsp/capture_yuvsp_start"
        #define YUVSP_DATA_FILE_DIR             "./../res/test_save_data_yuvsp"
    #endif
#endif



#ifdef CONFIG_JPEG_SAVE_INPUTSTREAM_DATA
    #ifdef CONFIG_JPEG_SAVE_STREAM_ANDROID_DIR
        #define CAPTURE_INPUTSTREAM_START       "/mnt/sdcard/Movies/capture_inputstream_start"
        #define INPUTSTREAM_DATA_FILE_DIR       "/mnt/sdcard/Movies"
    #else
        #define CAPTURE_INPUTSTREAM_START       "./../res/capture_inputstream_start"
        #define INPUTSTREAM_DATA_FILE_DIR       "./../res/jpg"
    #endif
#endif



#if defined(CONFIG_JPEG_SAVE_STREAM_ANDROID_DIR) || defined(CONFIG_JPEG_SAVE_YUVSP_ANDROID_DIR)
#include "utils/Log.h"
#define LOG_TAG        "libjpeg"
#define LOG_PRINT      ALOGE
#else
#define LOG_PRINT      JPEG_TRACE
#endif

/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/

/***************************** API forward declarations ************************/

/***************************** API realization *********************************/

HI_VOID JPEG_HDEC_PrintInputStream(HI_UCHAR* InputStreamVirBuf, HI_ULONG InputStreamSize)
{
#ifdef CONFIG_JPEG_SAVE_INPUTSTREAM_DATA
     if ((NULL == InputStreamVirBuf) || (0 == InputStreamSize)){
         LOG_PRINT("=====InputStreamVirBuf = 0x%lx,InputStreamSize = %ld\n",(unsigned long)InputStreamVirBuf,InputStreamSize);
         return;
     }
     LOG_PRINT("\n===============================================================================\n");
     LOG_PRINT("start[0x%x 0x%x 0x%x 0x%x]  end [0x%x 0x%x 0x%x 0x%x]\n",
                                                      InputStreamVirBuf[0],
                                                      InputStreamVirBuf[1],
                                                      InputStreamVirBuf[2],
                                                      InputStreamVirBuf[3],
                                                      InputStreamVirBuf[InputStreamSize - 4],
                                                      InputStreamVirBuf[InputStreamSize - 3],
                                                      InputStreamVirBuf[InputStreamSize - 2],
                                                      InputStreamVirBuf[InputStreamSize - 1]);
     LOG_PRINT("===============================================================================\n");
#else
     UNUSED(InputStreamVirBuf);
     UNUSED(InputStreamSize);
#endif
     return;
}


HI_VOID JPEG_HDEC_SaveInputStream(HI_UCHAR* InputStreamVirBuf, HI_ULONG InputStreamSize)
{
#ifdef CONFIG_JPEG_SAVE_INPUTSTREAM_DATA
     HI_CHAR OutFileName[256];
     HI_CHAR *pFile = "input_stream_";
     FILE* OutFile  = NULL;
     static HI_U32 SameFileCnt = 0;
     //HI_U32 u32TotalSize = (HI_U32)InputStreamSize;

     if ((NULL == InputStreamVirBuf) || (0 == InputStreamSize)){
         LOG_PRINT("=====InputStreamVirBuf = 0x%lx,InputStreamSize = %ld\n",(unsigned long)InputStreamVirBuf,InputStreamSize);
         return;
     }

     if (0 != access(CAPTURE_INPUTSTREAM_START, 0)){
         LOG_PRINT("\n===============================================================================\n");
         LOG_PRINT("please create %s file first\n",CAPTURE_INPUTSTREAM_START);
         LOG_PRINT("for example input: touch %s\n",CAPTURE_INPUTSTREAM_START);
         LOG_PRINT("===============================================================================\n");
         return;
     }

     snprintf(OutFileName,256,"%s/%s%d.jpg",INPUTSTREAM_DATA_FILE_DIR,pFile,SameFileCnt);

     OutFile = fopen(OutFileName,"wb+");
     if (NULL == OutFile){
         LOG_PRINT("errno = %d\n",errno);
         LOG_PRINT("open %s failure\n",OutFileName);
         return;
     }

     //fwrite((void*)&u32TotalSize,4,1,OutFile);
     fwrite((void*)InputStreamVirBuf, InputStreamSize, 1, OutFile);

     fclose(OutFile);

     SameFileCnt++;

     LOG_PRINT("\n===============================================================================\n");
     LOG_PRINT("success save input stream file %s\n",OutFileName);
     LOG_PRINT("start[0x%x 0x%x]  end [0x%x 0x%x]\n", InputStreamVirBuf[0],
                                                      InputStreamVirBuf[1],
                                                      InputStreamVirBuf[InputStreamSize - 2],
                                                      InputStreamVirBuf[InputStreamSize - 1]);
     LOG_PRINT("===============================================================================\n");
#else
     UNUSED(InputStreamVirBuf);
     UNUSED(InputStreamSize);
#endif
     return;
}

HI_VOID JPEG_HDEC_TEST_SaveYUVSP(const struct jpeg_decompress_struct *cinfo)
{
#ifdef CONFIG_JPEG_SAVE_YUVSP_DATA
     HI_CHAR pFileName[256];
     HI_CHAR pHdrName[256];
     HI_CHAR *pFile = "yuvsp";
     static HI_U32 u32Count = 0;

     FILE* pOutFile = NULL;
     FILE* pHdrFile = NULL;
     HI_S32 s32Cnt  = 0;
     HI_S32 s32Cnt1 = 0;
     HI_CHAR *pTmp  = NULL;

     HI_CHAR pTmpCbCr[10000];

     HI_U32 u32YWidth  = 0;
     HI_U32 u32YHeight = 0;
     HI_U32 u32YStride = 0;
     HI_U32 u32CWidth  = 0;
     HI_U32 u32CHeight = 0;
     HI_U32 u32CStride = 0;
     HI_CHAR* pYAddr   = NULL;
     HI_CHAR* pUVAddr  = NULL;
     HI_JPEG_FMT_E eFmt = JPEG_FMT_BUTT;

     JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (0 != access(CAPTURE_YUVSP_START, 0)){
         LOG_PRINT("\n===============================================================================\n");
         LOG_PRINT("请先创建 %s 文件\n",CAPTURE_YUVSP_START);
         LOG_PRINT("touch %s\n",CAPTURE_YUVSP_START);
         LOG_PRINT("===============================================================================\n");
         return;
     }

     snprintf(pFileName,256,"%s/%s%d.yuv",YUVSP_DATA_FILE_DIR,pFile,u32Count);
     snprintf(pHdrName,256,"%s/%s%d.hdr",YUVSP_DATA_FILE_DIR,pFile,u32Count);

     pOutFile = fopen(pFileName,"wb+");
     if (NULL == pOutFile){
         LOG_PRINT("open %s failure\n",pFileName);
     }

     u32YWidth  = pJpegHandle->stJpegSofInfo.u32YWidth;
     u32YHeight = pJpegHandle->stJpegSofInfo.u32YHeight;
     u32YStride = pJpegHandle->stJpegSofInfo.u32YStride;
     u32CWidth  = pJpegHandle->stJpegSofInfo.u32CWidth;
     u32CHeight = pJpegHandle->stJpegSofInfo.u32CHeight;
     u32CStride = pJpegHandle->stJpegSofInfo.u32CbCrStride;
     pYAddr     = pJpegHandle->stMiddleSurface.pMiddleVir[0];
     pUVAddr    = pJpegHandle->stMiddleSurface.pMiddleVir[1];
     eFmt       = pJpegHandle->enImageFmt;

     if (0 != u32YWidth){
         pTmp = pYAddr;
         for (s32Cnt = 0; s32Cnt < (HI_S32)u32YHeight; s32Cnt++){
             fwrite(pTmp, u32YWidth, 1, pOutFile);
             pTmp += u32YStride;
         }
     }

     if (0 != u32CWidth){

         pTmp = pUVAddr;
         for (s32Cnt = 0; s32Cnt < (HI_S32)u32CHeight; s32Cnt ++ ){
             for (s32Cnt1=0; s32Cnt1 < (HI_S32)u32CWidth; s32Cnt1++ ){
                 pTmpCbCr[s32Cnt1] = pTmp[2*s32Cnt1 + 1];
             }
             fwrite(pTmpCbCr, u32CWidth, 1, pOutFile );
             pTmp += u32CStride;
         }

         pTmp = pUVAddr;
         for (s32Cnt = 0; s32Cnt < (HI_S32)u32CHeight; s32Cnt ++ ){
             for (s32Cnt1 = 0; s32Cnt1 < (HI_S32)u32CWidth; s32Cnt1++ ){
                 pTmpCbCr[s32Cnt1] = pTmp[2*s32Cnt1 + 0];
             }
             fwrite(pTmpCbCr, u32CWidth, 1, pOutFile);
             pTmp += u32CStride;
         }


     }

     fclose(pOutFile);


     pHdrFile = fopen(pHdrName,"wb+");
     if (NULL == pHdrFile){
         LOG_PRINT("open %s failure\n",pHdrName);
     }
     switch(eFmt){
         case JPEG_FMT_YUV400:
             DADA_WRITE(pHdrFile, "format = \"Y\" ;\n");
             break;
         case JPEG_FMT_YUV420:
             DADA_WRITE(pHdrFile, "format = \"YUV420\" ;\n");
             break;
         case JPEG_FMT_YUV422_12:
             DADA_WRITE(pHdrFile, "format = \"YUV422\" ;\n");
             break;
         case JPEG_FMT_YUV422_21:
             DADA_WRITE(pHdrFile, "format = \"YUV422\" ;\n");
             break;
         case JPEG_FMT_YUV444:
             DADA_WRITE(pHdrFile, "format = \"YUV444\" ;\n");
             break;
         default:
             LOG_PRINT("format is unknow ;\n");
             break;
     }

     DADA_WRITE(pHdrFile, "offset = 0 ;\n");
     DADA_WRITE(pHdrFile, "width = %d ;\n", u32YWidth);
     DADA_WRITE(pHdrFile, "height = %d ;\n", u32YHeight);
     DADA_WRITE(pHdrFile, "framerate = 50 ;\n");
     DADA_WRITE(pHdrFile, "loop = 1 ;\n");

     fclose(pHdrFile);

     u32Count++;

     LOG_PRINT("\n===============================================================================\n");
     LOG_PRINT("save yuvsp data success\n");
     LOG_PRINT("the yuvsp file name is %s\n",pFileName);
     LOG_PRINT("the hdr file name is %s\n",pHdrName);
     LOG_PRINT("y width   = %d\n",   u32YWidth);
     LOG_PRINT("y height  = %d\n",   u32YHeight);
     LOG_PRINT("y stride  = %d\n",   u32YStride);
     LOG_PRINT("uv width  = %d\n",   u32CWidth);
     LOG_PRINT("uv height = %d\n",   u32CHeight);
     LOG_PRINT("uv stride = %d\n",   u32CStride);
     LOG_PRINT("y ddr     = 0x%lx\n",(unsigned long)pYAddr);
     LOG_PRINT("uv ddr    = 0x%lx\n",(unsigned long)pUVAddr);
     LOG_PRINT("img fmt   = %d\n",   eFmt);
     LOG_PRINT("===============================================================================\n");
#else
     UNUSED(cinfo);
#endif
     return;
}
