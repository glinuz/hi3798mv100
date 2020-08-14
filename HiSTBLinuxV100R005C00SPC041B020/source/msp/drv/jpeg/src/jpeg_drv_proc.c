/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_drv_proc.c
Version            : Initial Draft
Author            :
Created            : 2016/02/02
Description        :
Function List     :


History           :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef HI_BUILD_IN_BOOT


/*********************************add include here******************************/
#include "hi_jpeg_config.h"

#ifdef CONFIG_JPEG_PROC_ENABLE

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "jpeg_drv_proc.h"
#include "hi_type.h"


/**
 ** use common proc to deal with
 **/
#include "hi_drv_proc.h"

#include "hi_gfx_comm_k.h"

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/

HI_JPEG_PROC_INFO_S s_stJpeg6bProcInfo = {0};


/** user need proc message */
/** CNcomment:对外的proc信息 */
static HI_BOOL gs_bProcOn  = HI_FALSE;
/** our need debug message */
/** CNcomment:自己需要调试增加的信息 */
static HI_BOOL gs_bTraceOn = HI_FALSE;


/******************************* API forward declarations *******************/


static inline HI_VOID JPEG_PROC_PrintImgPixFmt (struct seq_file *pProcFile, HI_U32 ImgPixFmt);
static inline HI_VOID JPEG_PROC_PrintOutPixFmt (struct seq_file *pProcFile, HI_U32 OutPixFmt);
static inline HI_VOID JPEG_PROC_PrintDecState  (struct seq_file *pProcFile, HI_JPEG_DEC_STATE_E eDecState);
static inline HI_VOID JPEG_PROC_PrintDecType   (struct seq_file *pProcFile, HI_JPEG_DEC_TYPE_E eDecodeType);


/******************************* API realization *****************************/


/*****************************************************************************
* Function     : JPEG_Read_Proc
* Description  :
* param[in]    : *p
* param[in]    : *v
* retval       :HI_SUCCESS
* retval       :HI_FAILURE
*****************************************************************************/
static HI_S32 JPEG_Read_Proc(struct seq_file *p, HI_VOID *v)
{

   DRV_PROC_ITEM_S     *item  = NULL;
   HI_JPEG_PROC_INFO_S *procinfo = NULL;

   if (NULL == p)
   {
       return HI_FAILURE;
   }

   item = (DRV_PROC_ITEM_S *)(p->private);
   if (NULL == item)
   {
       return HI_FAILURE;
   }

   procinfo = (HI_JPEG_PROC_INFO_S *)(item->data);
   if (NULL == procinfo)
   {
       return HI_FAILURE;
   }

   if ( (HI_TRUE == gs_bProcOn) && (HI_TRUE == gs_bTraceOn))
   {
        SEQ_Printf(p, "lu width\t\t:%u\n",       procinfo->u32YWidth);
        SEQ_Printf(p, "lu height\t\t:%u\n",      procinfo->u32YHeight);
        SEQ_Printf(p, "lu stride\t\t:%u\n",      procinfo->u32YStride);
        SEQ_Printf(p, "lu size  \t\t:%u\n",      procinfo->u32YSize);
        SEQ_Printf(p, "ch width\t\t:%u\n",       procinfo->u32CWidth);
        SEQ_Printf(p, "ch height\t\t:%u\n",      procinfo->u32CHeight);
        SEQ_Printf(p, "ch size  \t\t:%u\n",      procinfo->u32CSize);
        SEQ_Printf(p, "ch stride\t\t:%u\n",      procinfo->u32CbCrStride);
        SEQ_Printf(p, "displey width\t\t:%u\n",  procinfo->u32DisplayW);
        SEQ_Printf(p, "display height\t\t:%u\n", procinfo->u32DisplayH);
        SEQ_Printf(p, "display stride\t\t:%u\n", procinfo->u32DisplayStride);
        SEQ_Printf(p, "decode width\t\t:%u\n",   procinfo->u32DecW);
        SEQ_Printf(p, "decode height\t\t:%u\n",  procinfo->u32DecH);
        SEQ_Printf(p, "decode stride\t\t:%u\n",  procinfo->u32DecStride);
    }

    if (HI_TRUE == gs_bProcOn)
    {

       SEQ_Printf(p, "input width\t\t:%u\n",   procinfo->u32InWidth);
       SEQ_Printf(p, "input height\t\t:%u\n",  procinfo->u32InHeight);
       SEQ_Printf(p, "output width\t\t:%u\n",  procinfo->u32OutWidth);
       SEQ_Printf(p, "output height\t\t:%u\n", procinfo->u32OutHeight);
       SEQ_Printf(p, "output stride\t\t:%u\n", procinfo->u32OutStride);

       JPEG_PROC_PrintImgPixFmt(p, procinfo->u32InFmt);

       JPEG_PROC_PrintOutPixFmt(p, procinfo->u32OutFmt);

       SEQ_Printf(p, "outbuf address\t\t:0x%x\n", procinfo->u32OutPhyBuf);
       SEQ_Printf(p, "decode scale\t\t:%u\n",     procinfo->u32Scale);

       JPEG_PROC_PrintDecState(p, procinfo->eDecState);

       JPEG_PROC_PrintDecType(p, procinfo->eDecodeType);

   }

   if ( (HI_TRUE == gs_bProcOn) && (HI_TRUE == gs_bTraceOn))
   {
      SEQ_Printf(p, "buffer start address     \t\t:0x%x\n", procinfo->u32DataStartAddr);
      SEQ_Printf(p, "buffer end address       \t\t:0x%x\n", procinfo->u32DataEndAddr);
      SEQ_Printf(p, "save stream start address\t\t:0x%x\n", procinfo->u32SaveStartAddr);
      SEQ_Printf(p, "save stream end address  \t\t:0x%x\n", procinfo->u32SaveEndAddr);
   }

   return HI_SUCCESS;
}

static inline HI_VOID JPEG_PROC_PrintImgPixFmt(struct seq_file *pProcFile, HI_U32 ImgPixFmt)
{
     char fmtname[50] = {'\0'};
     switch(ImgPixFmt)
     {
         case 0:
             strncpy(fmtname, "yuv400",strlen("yuv400"));
             fmtname[strlen("yuv400")] = '\0';
             break;
         case 1:
             strncpy(fmtname, "yuv420",strlen("yuv420"));
             fmtname[strlen("yuv420")] = '\0';
             break;
         case 2:
             strncpy(fmtname, "yuv422_21",strlen("yuv422_21"));
             fmtname[strlen("yuv422_21")] = '\0';
             break;
         case 3:
             strncpy(fmtname, "yuv422_12",strlen("yuv422_12"));
             fmtname[strlen("yuv422_12")] = '\0';
             break;
         case 4:
             strncpy(fmtname, "yuv444",strlen("yuv444"));
             fmtname[strlen("yuv444")] = '\0';
             break;
          case 5:
             strncpy(fmtname, "ycck",strlen("ycck"));
             fmtname[strlen("ycck")] = '\0';
             break;
          case 6:
             strncpy(fmtname, "cmyk",strlen("cmyk"));
             fmtname[strlen("cmyk")] = '\0';
             break;
         default:
             strncpy(fmtname, "Unknown",strlen("Unknown"));
             fmtname[strlen("Unknown")] = '\0';
             break;

    }

    SEQ_Printf(pProcFile, "image format\t\t:%s\n", fmtname);

    return;
}

static inline HI_VOID JPEG_PROC_PrintOutPixFmt(struct seq_file *pProcFile, HI_U32 OutPixFmt)
{
    char OutputColorspace[50] = {'\0'};
    switch(OutPixFmt)
    {
        case 2:
            strncpy(OutputColorspace, "JCS_RGB",strlen("JCS_RGB"));
            OutputColorspace[strlen("JCS_RGB")] = '\0';
            break;
        case 3:
            strncpy(OutputColorspace, "JCS_YCbCr",strlen("JCS_YCbCr"));
            OutputColorspace[strlen("JCS_YCbCr")] = '\0';
            break;
        case 4:
            strncpy(OutputColorspace, "JCS_CMYK",strlen("JCS_CMYK"));
            OutputColorspace[strlen("JCS_CMYK")] = '\0';
            break;
         case 5:
            strncpy(OutputColorspace, "JCS_YCCK",strlen("JCS_YCCK"));
            OutputColorspace[strlen("JCS_YCCK")] = '\0';
            break;
         case 6:
            strncpy(OutputColorspace, "JCS_CrCbY",strlen("JCS_CrCbY"));
            OutputColorspace[strlen("JCS_CrCbY")] = '\0';
            break;
         case 7:
             strncpy(OutputColorspace, "JCS_BGR",strlen("JCS_BGR"));
            OutputColorspace[strlen("JCS_BGR")] = '\0';
             break;
          case 8:
             strncpy(OutputColorspace, "JCS_ABGR_8888",strlen("JCS_ABGR_8888"));
            OutputColorspace[strlen("JCS_ABGR_8888")] = '\0';
             break;
          case 9:
             strncpy(OutputColorspace, "JCS_ARGB_8888",strlen("JCS_ARGB_8888"));
            OutputColorspace[strlen("JCS_ARGB_8888")] = '\0';
             break;
          case 10:
             strncpy(OutputColorspace, "JCS_ABGR_1555",strlen("JCS_ABGR_1555"));
            OutputColorspace[strlen("JCS_ABGR_1555")] = '\0';
             break;
          case 11:
             strncpy(OutputColorspace, "JCS_ARGB_1555",strlen("JCS_ARGB_1555"));
            OutputColorspace[strlen("JCS_ARGB_1555")] = '\0';
             break;
          case 12:
             strncpy(OutputColorspace, "JCS_RGB_565",strlen("JCS_RGB_565"));
            OutputColorspace[strlen("JCS_RGB_565")] = '\0';
             break;
          case 13:
             strncpy(OutputColorspace, "JCS_BGR_565",strlen("JCS_BGR_565"));
            OutputColorspace[strlen("JCS_BGR_565")] = '\0';
             break;
          case 14:
             strncpy(OutputColorspace, "JCS_YUV400_SP",strlen("JCS_YUV400_SP"));
            OutputColorspace[strlen("JCS_YUV400_SP")] = '\0';
             break;
          case 15:
             strncpy(OutputColorspace, "JCS_YUV420_SP",strlen( "JCS_YUV420_SP"));
            OutputColorspace[strlen("JCS_YUV420_SP")] = '\0';
             break;
          case 16:
             strncpy(OutputColorspace, "JCS_YUV422_SP_21",strlen("JCS_YUV422_SP_21"));
            OutputColorspace[strlen("JCS_YUV422_SP_21")] = '\0';
             break;
          case 17:
             strncpy(OutputColorspace, "JCS_YUV422_SP_12",strlen("JCS_YUV422_SP_12"));
            OutputColorspace[strlen("JCS_YUV422_SP_12")] = '\0';
             break;
          case 18:
             strncpy(OutputColorspace, "JCS_YUV444_SP",strlen("JCS_YUV444_SP"));
            OutputColorspace[strlen("JCS_YUV444_SP")] = '\0';
             break;
          default:
            strncpy(OutputColorspace, "UNKNOW",strlen("UNKNOW"));
            OutputColorspace[strlen("UNKNOW")] = '\0';
            break;

    }

    SEQ_Printf(pProcFile, "output format\t\t:%s\n",    OutputColorspace);

    return;
}


static inline HI_VOID JPEG_PROC_PrintDecState(struct seq_file *pProcFile, HI_JPEG_DEC_STATE_E eDecState)
{
      char DecodeState[50] = {'\0'};
      switch(eDecState)
      {
         case JPEG_DEC_FINISH_CREATE_DECOMPRESS:
                strncpy(DecodeState, "FinishCreateDecompress",strlen("FinishCreateDecompress"));
                DecodeState[strlen("FinishCreateDecompress")] = '\0';
                break;
          case JPEG_DEC_FINISH_STDIC:
                  strncpy(DecodeState, "FinishStdicFile",strlen("FinishStdicFile"));
                DecodeState[strlen("FinishStdicFile")] = '\0';
                break;
          case JPEG_DEC_FINISH_READ_HEADER:
                strncpy(DecodeState, "FinishReadHead",strlen("FinishReadHead"));
                DecodeState[strlen("FinishReadHead")] = '\0';
                break;
          case JPEG_DEC_FINISH_START_DECOMPRESS:
                strncpy(DecodeState, "FinishStartDecompress",strlen("FinishStartDecompress"));
                DecodeState[strlen("FinishStartDecompress")] = '\0';
                break;
           case JPEG_DEC_FINISH_READ_SCANLINES:
                   strncpy(DecodeState, "FinishReadScanlies",strlen("FinishReadScanlies"));
                DecodeState[strlen("FinishReadScanlies")] = '\0';
                   break;
           case JPEG_DEC_FINISH_FINISH_DECOMPRESS:
                  strncpy(DecodeState, "FinishDecompress",strlen("FinishDecompress"));
                DecodeState[strlen("FinishDecompress")] = '\0';
                   break;
           case JPEG_DEC_FINISH_DESTORY_DECOMPRESS:
                   strncpy(DecodeState, "DestoryDecompress",strlen("DestoryDecompress"));
                DecodeState[strlen("DestoryDecompress")] = '\0';
                   break;
           case JPEG_DEC_SUCCESS:
                   strncpy(DecodeState, "DesSuccess",strlen("DesSuccess"));
                DecodeState[strlen("DesSuccess")] = '\0';
                   break;
          default:
               strncpy(DecodeState, "UnDecode",strlen("UnDecode"));
               DecodeState[strlen("UnDecode")] = '\0';
               break;
      }

      SEQ_Printf(pProcFile, "decode state\t\t:%s\n", DecodeState);

      return;
}


static inline HI_VOID JPEG_PROC_PrintDecType(struct seq_file *pProcFile, HI_JPEG_DEC_TYPE_E eDecodeType)
{
      char DecodeType[50]  = {'\0'};
      switch(eDecodeType)
      {

         case JPEG_DEC_HW:
             strncpy(DecodeType, "HardDec",strlen("HardDec"));
             DecodeType[strlen("HardDec")] = '\0';
             break;

         case JPEG_DEC_SW:
             strncpy(DecodeType, "SoftDec",strlen("SoftDec"));
             DecodeType[strlen("SoftDec")] = '\0';
             break;

         default:
             strncpy(DecodeType, "UnDecode",strlen("UnDecode"));
             DecodeType[strlen("UnDecode")] = '\0';
             break;

     }

     SEQ_Printf(pProcFile, "decode type\t\t:%s\n", DecodeType);

     return;
}

/*****************************************************************************
* Function     : JPEG_Write_Proc
* Description  :
* param[in]    : file
* param[in]    : pBuf
* param[in]    : count
* param[in]    : *ppos
* retval       : HI_FAILURE
* retval       : HI_SUCCESS
*****************************************************************************/
static HI_S32 JPEG_Write_Proc(struct file * file,const char __user * pBuf,size_t count,loff_t *ppos)
{
     HI_CHAR buf[128] = {'\0'};

     if (NULL == pBuf)
     {
        return 0;
     }

     if (count > sizeof(buf))
     {
        return 0;
     }

     if (copy_from_user(buf, pBuf, count))
     {
         return 0;
     }
     buf[sizeof(buf) - 1] = '\0';

     if(strstr(buf, "proc on"))
     {
         gs_bProcOn = HI_TRUE;
     }
     else if(strstr(buf, "proc off"))
     {
         gs_bProcOn = HI_FALSE;
     }
     else if(strstr(buf, "trace on"))
     {
         gs_bTraceOn = HI_TRUE;
     }
     else if(strstr(buf, "trace off"))
     {
         gs_bTraceOn = HI_FALSE;
     }

     return count;
}


/*****************************************************************************
* Function     : JPEG_Proc_GetStruct
* Description  :
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:       :nothing
*****************************************************************************/

HI_VOID JPEG_Proc_GetStruct(HI_JPEG_PROC_INFO_S **ppstProcInfo)
{
    *ppstProcInfo = &s_stJpeg6bProcInfo;
    return;
}

/*****************************************************************************
* Function     : JPEG_Proc_init
* Description  :
* param[in]    : NA
* retval       : NA
*****************************************************************************/
HI_VOID JPEG_Proc_init(HI_VOID)
{
     GFX_PROC_ITEM_S pProcItem;
     HI_CHAR *pEntry_name = PROC_JPEG_ENTRY_NAME;

     pProcItem.fnRead   = JPEG_Read_Proc;
     pProcItem.fnWrite  = JPEG_Write_Proc;
     pProcItem.fnIoctl  = NULL;

     HI_GFX_PROC_AddModule(pEntry_name,&pProcItem,(HI_VOID *)&s_stJpeg6bProcInfo);

     return;
}
/*****************************************************************************
* Function     : JPEG_Proc_Cleanup
* Description  :
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:       :nothing
*****************************************************************************/

HI_VOID JPEG_Proc_Cleanup(HI_VOID)
{
     HI_CHAR *pEntry_name =  PROC_JPEG_ENTRY_NAME;
     HI_GFX_PROC_RemoveModule(pEntry_name);

     return;
}


/*****************************************************************************
* Function     : JPEG_Proc_IsOpen
* Description  :
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:       :nothing
*****************************************************************************/
HI_BOOL JPEG_Proc_IsOpen(HI_VOID)
{
    return gs_bProcOn;
}
/*****************************************************************************
* Function     : JPEG_Get_Proc_Status
* Description  : get the proc status
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:       :nothing
*****************************************************************************/

HI_VOID JPEG_Get_Proc_Status(HI_BOOL* pbProcStatus)
{
     *pbProcStatus = gs_bProcOn;
}

#endif/**CONFIG_JPEG_PROC_ENABLE**/



#endif/**HI_BUILD_IN_BOOT**/
