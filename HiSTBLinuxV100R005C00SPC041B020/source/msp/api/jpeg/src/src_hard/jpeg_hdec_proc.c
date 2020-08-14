/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_proc.c
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
#include "jpegint.h"

#include "hi_jpeg_config.h"
#include "jpeg_hdec_api.h"
#include "hi_jpeg_reg.h"

/***************************** Macro Definition ********************************/

/***************************** Structure Definition ****************************/



/***************************** Global Variable declaration *********************/

/***************************** API forward declarations ************************/

/***************************** API realization *********************************/

HI_VOID JPEG_HDEC_SetProcInfo(const struct jpeg_decompress_struct *cinfo)
{
 #ifdef CONFIG_JPEG_PROC_ENABLE
     HI_S32 s32Ret = HI_SUCCESS;
     HI_JPEG_PROC_INFO_S stProcInfo;

     const HI_U32 u32InFmt[10]  = {0,1,2,3,4,5,6,7,8,9};
     const HI_U32 u32OutFmt[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (NULL == pJpegHandle->pJpegRegVirAddr){
        return;
     }
     stProcInfo.u32YWidth        = pJpegHandle->stJpegSofInfo.u32YWidth;
     stProcInfo.u32YHeight       = pJpegHandle->stJpegSofInfo.u32YHeight;
     stProcInfo.u32YSize         = pJpegHandle->stJpegSofInfo.u32YSize;
     stProcInfo.u32CWidth        = pJpegHandle->stJpegSofInfo.u32CWidth;
     stProcInfo.u32CHeight       = pJpegHandle->stJpegSofInfo.u32CHeight;
     stProcInfo.u32CSize         = pJpegHandle->stJpegSofInfo.u32CSize;
     stProcInfo.u32YStride       = pJpegHandle->stJpegSofInfo.u32YStride;
     stProcInfo.u32CbCrStride    = pJpegHandle->stJpegSofInfo.u32CbCrStride;
     stProcInfo.u32DisplayW      = pJpegHandle->stJpegSofInfo.u32DisplayW;
     stProcInfo.u32DisplayH      = pJpegHandle->stJpegSofInfo.u32DisplayH;
     stProcInfo.u32DisplayStride = pJpegHandle->stJpegSofInfo.u32DisplayStride;
     stProcInfo.u32DecW          = pJpegHandle->stJpegSofInfo.u32DecW;
     stProcInfo.u32DecH          = pJpegHandle->stJpegSofInfo.u32DecH;
     stProcInfo.u32DecStride     = pJpegHandle->stJpegSofInfo.u32DecStride;
     stProcInfo.u32DataStartAddr = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADDR);
     stProcInfo.u32DataEndAddr   = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADDR);
     stProcInfo.u32SaveStartAddr = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD);
     stProcInfo.u32SaveEndAddr   = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD);


     stProcInfo.u32InWidth       = cinfo->image_width;
     stProcInfo.u32InHeight      = cinfo->image_height;
     stProcInfo.u32OutWidth      = cinfo->output_width;
     stProcInfo.u32OutHeight     = cinfo->output_height;
     stProcInfo.u32OutStride     = pJpegHandle->stOutDesc.stOutSurface.u32OutStride[0];
     stProcInfo.u32InFmt         = u32InFmt[pJpegHandle->enImageFmt];
     stProcInfo.u32OutFmt        = u32OutFmt[cinfo->out_color_space];
     stProcInfo.u32OutPhyBuf     = pJpegHandle->stOutDesc.stOutSurface.u32OutPhy[0];
     if (0 == pJpegHandle->u32ScalRation){
         stProcInfo.u32Scale     = 1;
     }else if (1 == pJpegHandle->u32ScalRation){
         stProcInfo.u32Scale     = 2;
     }else if (2 == pJpegHandle->u32ScalRation){
         stProcInfo.u32Scale     = 4;
     }else{
          stProcInfo.u32Scale     = 8;
     }

     if (DSTATE_START == cinfo->global_state){
         /**
         **create decompress
         **/
         pJpegHandle->eDecState = JPEG_DEC_FINISH_CREATE_DECOMPRESS;
     }else if (DSTATE_INHEADER == cinfo->global_state){
        /**
         **read header ready
         **/
         pJpegHandle->eDecState = JPEG_DEC_FINISH_READ_HEADER;
     }else if (DSTATE_SCANNING == cinfo->global_state){
        /**
         **start decompress ready
         **/
         pJpegHandle->eDecState = JPEG_DEC_FINISH_START_DECOMPRESS;
     }else if (DSTATE_SCANNING == cinfo->global_state){
        /**
         **read scanlines ready
         **/
         pJpegHandle->eDecState = JPEG_DEC_FINISH_READ_SCANLINES;
     }else if (DSTATE_STOPPING == cinfo->global_state){
        /**
         **finish decompress
         **/
         pJpegHandle->eDecState = JPEG_DEC_FINISH_FINISH_DECOMPRESS;
     }else if (0 == cinfo->global_state){
        /**
         **destory decompress
         **/
         pJpegHandle->eDecState = JPEG_DEC_FINISH_DESTORY_DECOMPRESS;
     }

     stProcInfo.eDecState        = pJpegHandle->eDecState;

     if (HI_TRUE == pJpegHandle->bHdecEnd){
        stProcInfo.eDecodeType      = JPEG_DEC_HW;
     }else{
        stProcInfo.eDecodeType      = JPEG_DEC_SW;
     }

     s32Ret = ioctl(pJpegHandle->s32JpegDev, CMD_JPG_READPROC, &stProcInfo);
     if (HI_SUCCESS != s32Ret){
        return;
     }
#endif
     return;
}
