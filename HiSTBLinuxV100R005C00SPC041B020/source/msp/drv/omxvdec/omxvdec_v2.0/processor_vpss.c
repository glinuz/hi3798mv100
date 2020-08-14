/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    processor_vpss.c
 *
 * Purpose: omxvdec processor vpss functions
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#include <asm/div64.h>
#include "hi_drv_module.h"
#include "drv_vpss_ext.h"
#include "drv_vdec_ext.h"
#include "decoder.h"
#include "processor.h"
#include "row_map_table.h"

#include "hi_math.h"
#include "vfmw_ext.h"

#include "vfmw.h"  //add by l00228308 for vpss bypass

/*================ EXTERN VALUE ================*/
extern HI_BOOL  g_SaveYuvEnable;
extern HI_BOOL  g_SaveSrcYuvEnable;
extern HI_U32   g_SaveSrcYuvChanNum;
extern HI_U32   g_SaveYuvChanNum;
extern HI_CHAR  g_SavePath[];
extern HI_CHAR  g_SaveName[];
extern HI_U32   g_SaveNum;
extern struct file *g_SaveSrcYuvFile;
extern OMXVDEC_FUNC g_stOmxFunc;
extern OMXVDEC_ENTRY *g_OmxVdec;
extern VFMW_EXPORT_FUNC_S* pVfmwFunc;
extern HI_BOOL  g_FastOutputMode;
extern HI_U32 g_CompressEnable;

/*=================== MACRO ====================*/
#define  REALID(id)           (id%100+2)

//HDR
#define SMPTE_ST_2084 (16)
#define BT2020 (9)
#define BT2020_NON_CONSTANT_LUMA_SYS (9)
#define BT2020_CONSTANT_LUMA_SYS (10)
#define XVYCC_709 (1)

/*================ GLOBAL VALUE ================*/
HI_BOOL  g_FrameRateLimit        = HI_FALSE;//HI_TRUE;

#ifdef HI_TVOS_SUPPORT
HI_BOOL  g_DeInterlaceEnable     = HI_TRUE;
#else
HI_BOOL  g_DeInterlaceEnable     = HI_FALSE;
#endif

HI_BOOL  g_BypassVpss            = HI_FALSE;
MMZ_BUFFER_S  g_YUV_ExchangeBuffer;


/*================ STATIC VALUE ================*/
static VPSS_EXPORT_FUNC_S* pVpssFunc = HI_NULL;


/*============== INTERNAL FUNCTION =============*/
extern HI_S32 decoder_find_special2Normal_Index(OMXVDEC_CHAN_CTX *pchan,HI_U32 u32Phyaddr, HI_U32 *pIndex);

static HI_U32 processor_get_stride(HI_U32 protocol, HI_U32 width, HI_BOOL is_native_output);

static HI_DRV_PIX_FORMAT_E processor_color_omx_to_hal(OMX_PIX_FORMAT_E format)
{
    HI_DRV_PIX_FORMAT_E ret_color;
    switch (format)
    {
       case OMX_PIX_FMT_NV12:
           ret_color =  HI_DRV_PIX_FMT_NV12;
           break;

       case OMX_PIX_FMT_NV21:
           ret_color = HI_DRV_PIX_FMT_NV21;
           break;

       case OMX_PIX_FMT_YUV420Planar:
           ret_color = HI_DRV_PIX_FMT_YUV420M;
           break;

       default:
           OmxPrint(OMX_ERR, "vpss not support format %d, return default NV12\n", format);
           ret_color = HI_DRV_PIX_FMT_NV12;
           break;
    }

    return ret_color;
}


static HI_VOID OMXVDEC_GetColourInfo(IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    pstFrame->stColourInfo.u8FullRangeFlag   = pstImage->ColourInfo.FullRangeFlag;
    pstFrame->stColourInfo.u8ColourPrimaries = pstImage->ColourInfo.ColourPrimaries;
    pstFrame->stColourInfo.u8MatrixCoeffs    = pstImage->ColourInfo.MatrixCoeffs;
}

#if 0 //when AVS2.0 support, open this function
HI_DRV_VIDEO_FRAME_TYPE_E OMXVDEC_CheckAVS2HDRType(UINT32 TransferCharacteristics)
{
    HI_DRV_VIDEO_FRAME_TYPE_E   enSrcFrameType;

    switch (TransferCharacteristics)
    {
        case 12:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HDR10;
            break;

        case 13:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SLF;
            break;

        case 14:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HLG;
            break;

        default:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;
            break;
    }

    return enSrcFrameType;
}

static HI_VOID OMXVDEC_GetAVS2HDRInfo(IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    OMXVDEC_GetColourInfo(pstImage, pstFrame);
    pstFrame->unHDRInfo.stSLFInfo.u8TransferCharacteristics = pstImage->TransferCharacteristics;

    if (pstImage->MasteringDisplayColourVolume.IsAvailable == 1)
    {
        pstFrame->unHDRInfo.stHDR10Info.bMasteringAvailable = pstImage->MasteringAvailable;
        memcpy(&(pstFrame->unHDRInfo.stSLFInfo.stMasteringInfo),
               &(pstImage->MasteringDisplayColourVolume),
               sizeof(MASTERING_DISPLAY_COLOUR_VOLUME_S));
    }

    if (pstImage->ContentLightLevel.IsAvailable == 1)
    {
        pstFrame->unHDRInfo.stHDR10Info.bContentAvailable = pstImage->bContentAvailable;
        memcpy(&(pstFrame->unHDRInfo.stSLFInfo.stContentInfo),
               &(pstImage->ContentLightLevel),
               sizeof(CONTENT_LIGHT_LEVEL_INFO_S));
    }

    return;
}
#endif

HI_DRV_VIDEO_FRAME_TYPE_E OMXVDEC_CheckHEVCHDRType(IMAGE* pstImage, HI_U8 *pCompatible)
{
    HI_DRV_VIDEO_FRAME_TYPE_E   enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;

    if (pstImage->TechnicolorInfo.IsValid == 1)
    {
        enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_TECHNICOLOR;

        return enSrcFrameType;
    }

    switch (pstImage->TransferCharacteristics)
    {
        case 14:
            if (pstImage->ComapitibilityInfo.HDRTransferCharacteristicIdc == 18)
            {
                enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HLG;
                *pCompatible = 1;
            }
            else
            {
                enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;
            }

            break;

        case 16:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HDR10;
            break;

        case 18:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HLG;
            *pCompatible = 0;
            break;

        default:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;
            break;
    }

    return enSrcFrameType;
}

static HI_VOID OMXVDEC_GetHEVCHDRInfo(IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    if (pstFrame->enSrcFrameType == HI_DRV_VIDEO_FRAME_TYPE_HDR10)
    {
        OMXVDEC_GetColourInfo(pstImage, pstFrame);
        pstFrame->unHDRInfo.stHDR10Info.u8TransferCharacteristics = pstImage->TransferCharacteristics;

        if (pstImage->MasteringAvailable == 1)
        {
            pstFrame->unHDRInfo.stHDR10Info.u8MasteringAvailable = pstImage->MasteringAvailable;
            memcpy(&(pstFrame->unHDRInfo.stHDR10Info.stMasteringInfo),
                   &(pstImage->MasteringDisplayColourVolume),
                   sizeof(MASTERING_DISPLAY_COLOUR_VOLUME_S));
        }

        if (pstImage->ContentAvailable == 1)
        {
            pstFrame->unHDRInfo.stHDR10Info.u8ContentAvailable = pstImage->ContentAvailable;
            memcpy(&(pstFrame->unHDRInfo.stHDR10Info.stContentInfo),
                   &(pstImage->ContentLightLevel),
                   sizeof(CONTENT_LIGHT_LEVEL_INFO_S));
        }
    }

    else if (pstFrame->enSrcFrameType == HI_DRV_VIDEO_FRAME_TYPE_HLG)
    {
        OMXVDEC_GetColourInfo(pstImage, pstFrame);
    }

    else if (pstFrame->enSrcFrameType == HI_DRV_VIDEO_FRAME_TYPE_TECHNICOLOR)
    {
        if (pstImage->TechnicolorInfo.IsValid == 1)
        {
            memcpy(&(pstFrame->unHDRInfo.stTechnicolorInfo.stHDR_StaticParms),
                   &(pstImage->TechnicolorInfo.TCH_HDR_StaticParms),
                   sizeof(TECHNICOLOR_HDR_SEQ_PARAMS));

            memcpy(&(pstFrame->unHDRInfo.stTechnicolorInfo.stHDR_DanamicParms),
                   &(pstImage->TechnicolorInfo.TCH_HDR_DanamicParms),
                   sizeof(TECHNICOLOR_HDR_FRM_PARAMS));
        }
    }

    return;
}

static HI_VOID OMXVDEC_SetHDRInfo(HI_U32 protocol, IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_U8 IsBackwardsCompatible = 0;

    switch (protocol)
    {
#if 0 //when AVS2.0 support, open this function
        case VFMW_AVS2:
            pstFrame->enCodecType = HI_DRV_STD_AVS2;
            pstFrame->enSrcFrameType = OMXVDEC_CheckAVS2HDRType(pstImage->TransferCharacteristics);
            OMXVDEC_GetAVS2HDRInfo(pstImage, pstFrame);
            break;
#endif

        case STD_VP9:
            pstFrame->enCodecType = HI_DRV_STD_VP9;
            //todo
            break;

        case STD_HEVC:
            pstFrame->enCodecType = HI_DRV_STD_HEVC;
            pstFrame->enSrcFrameType = OMXVDEC_CheckHEVCHDRType(pstImage, &IsBackwardsCompatible);
            pstFrame->unHDRInfo.stHLGInfo.IsBackwardsCompatible = IsBackwardsCompatible;
            OMXVDEC_GetHEVCHDRInfo(pstImage, pstFrame);
            break;

        default :
            pstFrame->enCodecType = HI_DRV_STD_START_RESERVED;
            //do nothing
            break;
    }

    return ;
}

HI_VOID processor_save_yuv_NV21(struct file *pfile, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_U8 *Y_Addr, HI_U8 *C_Addr)
{
    HI_S32 ret;
    HI_U32 i, j;
    HI_S32 len = 0;
    mm_segment_t oldfs;
    HI_S32 chroma_width = 0;
    HI_S32 chroma_height = 0;
    HI_S32 chroma_len = 0;
    HI_U32 need_size = 0;
    HI_U8 *UArray = NULL;
    HI_U8 *VArray = NULL;

    OmxPrint(OMX_ALWS, "Saving src NV21 yuv data...\n");

    need_size = pstFrame->u32Width * pstFrame->u32Height / 2;
    if (g_YUV_ExchangeBuffer.u32Size < need_size)
    {
        if (g_YUV_ExchangeBuffer.u32Size > 0)
        {
            HI_DRV_OMXVDEC_UnmapAndRelease(&g_YUV_ExchangeBuffer);
        }

        ret = HI_DRV_OMXVDEC_AllocAndMap("YUV_NV21_EXG", HI_NULL, need_size, 0, &g_YUV_ExchangeBuffer);
        if (ret != HI_SUCCESS)
        {
            OmxPrint(OMX_ERR, "%s alloc extra buffer mem failed\n", __func__);
            return;
        }
    }

    UArray = g_YUV_ExchangeBuffer.pu8StartVirAddr;
    VArray = UArray + pstFrame->u32Width * pstFrame->u32Height / 4;

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    /* Y */
    for (i = 0; i < pstFrame->u32Height; i++)
    {
        len = pfile->f_op->write(pfile, Y_Addr, pstFrame->u32Width, &pfile->f_pos);

        if (len < pstFrame->u32Width)
        {
            OmxPrint(OMX_ERR, "%s write y failed.\n", __func__);
        }

        Y_Addr += pstFrame->stBufAddr[0].u32Stride_Y;
    }

    /* UV */
    chroma_width = pstFrame->u32Width / 2;
    chroma_height = pstFrame->u32Height / 2;
    chroma_len = chroma_width * chroma_height;

    for (i = 0; i < chroma_height; i++)
    {
        for (j = 0; j < chroma_width; j++)
        {
            VArray[i * chroma_width + j] = C_Addr[2 * j];
            UArray[i * chroma_width + j] = C_Addr[2 * j + 1];
        }

        C_Addr += pstFrame->stBufAddr[0].u32Stride_C;
    }

    len = pfile->f_op->write(pfile, UArray, chroma_len, &pfile->f_pos);
    if (len != chroma_len)
    {
        OmxPrint(OMX_ERR, "%s write u failed.\n", __func__);
    }

    len = pfile->f_op->write(pfile, VArray, chroma_len, &pfile->f_pos);
    if (len != chroma_len)
    {
        OmxPrint(OMX_ERR, "%s write v failed.\n", __func__);
    }

    set_fs(oldfs);

    return;
}

HI_VOID processor_save_yuv_NV21_Tile(struct file *pfile, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_U8 *Y_Addr, HI_U8 *C_Addr)
{
    HI_S32 ret;
    HI_U32 i, j;
    mm_segment_t oldfs;
    HI_S32 stride;
    HI_S32 chroma_width, chroma_height, chroma_len;
    HI_U32 len, need_size;
    HI_U8 *dst, *src, *tmp;
    HI_U32 srcIdx, dstIdx;
    HI_U32 tileX, tileY;
    HI_U8 *YUVArray = NULL;
    HI_U8 *UArray = NULL;
    HI_U8 *VArray = NULL;

    OmxPrint(OMX_ALWS, "Saving src NV21 Tile yuv data...\n");

    need_size = pstFrame->u32Width * pstFrame->u32Height * 3 / 2;
    if (g_YUV_ExchangeBuffer.u32Size < need_size)
    {
        if (g_YUV_ExchangeBuffer.u32Size > 0)
        {
            HI_DRV_OMXVDEC_UnmapAndRelease(&g_YUV_ExchangeBuffer);
        }

        ret = HI_DRV_OMXVDEC_AllocAndMap("YUV_NV21_TILE_EXG", HI_NULL, need_size, 0, &g_YUV_ExchangeBuffer);
        if (ret != HI_SUCCESS)
        {
            OmxPrint(OMX_ERR, "%s alloc extra buffer mem failed\n", __func__);
            return;
        }
    }

    YUVArray = g_YUV_ExchangeBuffer.pu8StartVirAddr;
    UArray = YUVArray + pstFrame->u32Width * pstFrame->u32Height;
    VArray = UArray + pstFrame->u32Width * pstFrame->u32Height / 4;

    stride      = pstFrame->stBufAddr[0].u32Stride_Y * 16;

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    /* Y */
    for (i = 0; i < pstFrame->u32Height; i++)
    {
        for (j = 0; j < pstFrame->u32Width; j += 64)
        {
            dst  = (unsigned char *)(YUVArray + pstFrame->u32Width * i + j);
            tileY = (i / 16) % 2;
            tileX = (j / 64) % 4;
            srcIdx = i % 16;
            dstIdx = g_RowMapTable_Y[tileY][tileX][srcIdx];
            src =  Y_Addr + stride * (i / 16) + dstIdx * 64 + (j / 64) * 64 * 16;
            memcpy(dst, src, 64);
        }
    }

    len = pfile->f_op->write(pfile, YUVArray, pstFrame->u32Width * pstFrame->u32Height, &pfile->f_pos);
    if (len != pstFrame->u32Width * pstFrame->u32Height)
    {
        OmxPrint(OMX_ERR, "%s write y failed.\n", __func__);
    }

    /* C */
    for (i = 0; i < pstFrame->u32Height / 2; i++)
    {
        for (j = 0; j < pstFrame->u32Width; j += 64)
        {
            dst  = (unsigned char *)(YUVArray + pstFrame->u32Width * i + j);
            tileY = (i / 8) % 2;
            tileX = (j / 64) % 4;
            srcIdx = i % 8;
            dstIdx = g_RowMapTable_UV[tileY][tileX][srcIdx];
            src =  C_Addr + (stride / 2) * (i / 8) + dstIdx * 64 + (j / 64) * 64 * 8;
            memcpy(dst, src, 64);
        }
    }

    chroma_width = pstFrame->u32Width / 2;
    chroma_height = pstFrame->u32Height / 2;
    chroma_len = chroma_width * chroma_height;

    tmp = YUVArray;
    for (i = 0; i < chroma_height; i++)
    {
        for (j = 0; j < chroma_width; j++)
        {
            VArray[i * chroma_width + j] = tmp[2 * j];
            UArray[i * chroma_width + j] = tmp[2 * j + 1];
        }

        tmp += pstFrame->u32Width;
    }

    len = pfile->f_op->write(pfile, UArray, chroma_len, &pfile->f_pos);
    if (len != chroma_len)
    {
        OmxPrint(OMX_ERR, "%s write u failed.\n", __func__);
    }

    len = pfile->f_op->write(pfile, VArray, chroma_len, &pfile->f_pos);
    if (len != chroma_len)
    {
        OmxPrint(OMX_ERR, "%s write v failed.\n", __func__);
    }

    set_fs(oldfs);

    return;
}


HI_VOID processor_save_src_frame(OMXVDEC_CHAN_CTX *pchan, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    MMZ_BUFFER_S stFrmBuf;
    HI_U8 *Y_Addr = HI_NULL;
    HI_U8 *C_Addr = HI_NULL;
    HI_DRV_VIDEO_PRIVATE_S *pstVideoPriv = (HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv);

    if (g_SaveSrcYuvFile == HI_NULL || g_SaveSrcYuvChanNum != pchan->channel_id)
    {
        return;
    }

    if (DEF_HI_DRV_VPSS_LAST_ERROR_FLAG == pstVideoPriv->u32LastFlag)
    {
        OmxPrint(OMX_ERR, "%s Met DEF_HI_DRV_VPSS_LAST_ERROR_FLAG frame!\n", __func__);
        return;
    }

    memset(&stFrmBuf, 0 , sizeof(MMZ_BUFFER_S));
    if (STD_H263 == pchan->protocol || STD_SORENSON == pchan->protocol)
    {
        Y_Addr = (HI_S8 *)(HI_SIZE_T)(pstFrame->stBufAddr[0].vir_addr_y);
        C_Addr = (HI_S8 *)(HI_SIZE_T)(pstFrame->stBufAddr[0].vir_addr_c);
    }
    else
    {
        stFrmBuf.u32StartPhyAddr = pstFrame->stBufAddr[0].u32PhyAddr_YHead;
        stFrmBuf.u32Size = (pstFrame->stBufAddr[0].u32PhyAddr_CHead - pstFrame->stBufAddr[0].u32PhyAddr_YHead) * 3 / 2;
        if (HI_SUCCESS != HI_DRV_OMXVDEC_Map(&stFrmBuf))
        {
            OmxPrint(OMX_ERR, "%s map virtual address failed!\n", __func__);
            return;
        }

        Y_Addr = stFrmBuf.pu8StartVirAddr;
        C_Addr = Y_Addr + (pstFrame->stBufAddr[0].u32PhyAddr_CHead - pstFrame->stBufAddr[0].u32PhyAddr_YHead);
    }

    if ((Y_Addr == 0) || (C_Addr == 0))
    {
        OmxPrint(OMX_ERR, "%s invalid: Y_Addr = %p, C_Addr = %p!\n", __func__, Y_Addr, C_Addr);
        goto EXIT;
    }

    if (HI_DRV_PIX_FMT_NV21 == pstFrame->ePixFormat)
    {
        processor_save_yuv_NV21(g_SaveSrcYuvFile, pstFrame, Y_Addr, C_Addr);
    }
    else if (HI_DRV_PIX_FMT_NV21_TILE == pstFrame->ePixFormat)
    {
        processor_save_yuv_NV21_Tile(g_SaveSrcYuvFile, pstFrame, Y_Addr, C_Addr);
    }

EXIT:
    if (stFrmBuf.pu8StartVirAddr != HI_NULL)
    {
        HI_DRV_OMXVDEC_Unmap(&stFrmBuf);
    }

    return;
}


HI_S32 processor_inform_img_ready(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s pchan is NULL!\n", __func__);
        return HI_FAILURE;
    }
    s32Ret = (pVpssFunc->pfnVpssSendCommand)(pchan->processor_id, HI_DRV_VPSS_USER_COMMAND_IMAGEREADY, NULL);
    if (s32Ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s CALL HI_DRV_VPSS_USER_COMMAND_IMAGEREADY error,ret = %d\n", __func__, s32Ret);
    }
    return s32Ret;
}

static HI_S32 processor_get_frame(OMXVDEC_CHAN_CTX *pchan, EXTERNAL_FRAME_STORE_S *frame, HI_U32 expect_length)
{
    unsigned long flags;
    OMXVDEC_BUF_S *pbuf = HI_NULL;
    HI_S32 ret = HI_FAILURE;

    spin_lock_irqsave(&pchan->chan_lock, flags);
    if (pchan->state != CHAN_STATE_WORK)
    {
        spin_unlock_irqrestore(&pchan->chan_lock, flags);
        OmxPrint(OMX_VPSS, "VPSS: pchan->state != CHAN_STATE_WORK\n");
        return HI_FAILURE;
    }
    spin_unlock_irqrestore(&pchan->chan_lock, flags);

    if (pchan->output_flush_pending)
    {
        OmxPrint(OMX_VPSS, "VPSS: output_flush_pending\n");
        return HI_FAILURE;
    }

    spin_lock_irqsave(&pchan->yuv_lock, flags);
    if (list_empty(&pchan->yuv_queue))
    {
        OmxPrint(OMX_VPSS, "VPSS: List is empty!\n");
        goto empty;
    }

    list_for_each_entry(pbuf, &pchan->yuv_queue, list)
    {
        if(BUF_STATE_USING == pbuf->status)
        {
            continue;
        }

        if (expect_length > pbuf->buf_len)
        {
            OmxPrint(OMX_VPSS, "VPSS: expect_length(%d) > buf_len(%d)\n", expect_length, pbuf->buf_len);
            continue;
        }

        pbuf->status   = BUF_STATE_USING;
        frame->PhyAddr = pbuf->phy_addr + pbuf->offset;
        frame->VirAddr = (UINT8 *)(pbuf->kern_vaddr + pbuf->offset);
        frame->Length  = pbuf->buf_len;

        frame->PrivatePhyAddr = pbuf->private_phy_addr;
        frame->PrivateVirAddr = pbuf->private_vaddr;
        frame->PrivateLength  = pbuf->private_len;

        pchan->yuv_use_cnt++;

        ret = HI_SUCCESS;

        OmxPrint(OMX_OUTBUF, "VPSS got frame: phy = 0x%08x vir:0x%p Private Phy = 0x%08x vir = 0x%p\n",\
                 frame->PhyAddr, frame->VirAddr, frame->PrivatePhyAddr, frame->PrivateVirAddr);
        break;
    }

empty:
    spin_unlock_irqrestore(&pchan->yuv_lock, flags);

    return ret;
}

static HI_S32 processor_release_frame(OMXVDEC_CHAN_CTX *pchan, HI_U32 phyaddr)
{
    HI_S32 is_find = 0;
    unsigned long flags;
    OMXVDEC_BUF_S *pbuf = HI_NULL;
    OMXVDEC_BUF_S *ptmp = HI_NULL;
    OMXVDEC_BUF_DESC user_buf;
    memset(&user_buf, 0, sizeof(OMXVDEC_BUF_DESC));

    if (HI_NULL == pchan || 0 == phyaddr)
    {
        OmxPrint(OMX_FATAL, "%s param invalid!\n", __func__);
        return HI_FAILURE;
    }

    /* for we del list during, so use safe means */
    spin_lock_irqsave(&pchan->yuv_lock, flags);
    if (list_empty(&pchan->yuv_queue))
    {
        spin_unlock_irqrestore(&pchan->yuv_lock, flags);
        OmxPrint(OMX_ERR, "%s: list is empty\n", __func__);
        return 0;
    }

    list_for_each_entry_safe(pbuf, ptmp, &pchan->yuv_queue, list)
    {
        if (phyaddr == (pbuf->phy_addr + pbuf->offset))
        {
           if (pbuf->status != BUF_STATE_USING)
           {
               OmxPrint(OMX_ERR, "%s: buffer(0x%08x) flags confused!\n",__func__, phyaddr);
           }

           is_find = 1;
           pbuf->status =  BUF_STATE_IDLE;
           break;
        }
    }
    spin_unlock_irqrestore(&pchan->yuv_lock, flags);

    if (!is_find)
    {
        OmxPrint(OMX_ERR, "%s: buffer(0x%08x) not in queue!\n",__func__,  phyaddr);
        return HI_FAILURE;
    }

    pchan->yuv_use_cnt = (pchan->yuv_use_cnt > 0) ? (pchan->yuv_use_cnt-1) : 0;

    if (pchan->output_flush_pending || pchan->pause_pending)
    {
        spin_lock_irqsave(&pchan->yuv_lock, flags);
        list_del(&pbuf->list);
        spin_unlock_irqrestore(&pchan->yuv_lock, flags);

        user_buf.dir = PORT_DIR_OUTPUT;
        user_buf.bufferaddr =(HI_SIZE_T)(pbuf->user_vaddr);
        user_buf.buffer_len =  pbuf->buf_len;
        user_buf.client_data = (HI_SIZE_T)(pbuf->client_data);
        user_buf.data_len = 0;
        user_buf.timestamp = 0;

        message_queue(pchan->msg_queue, VDEC_MSG_RESP_OUTPUT_DONE, VDEC_S_SUCCESS, (HI_VOID *)&user_buf);
        pchan->omx_chan_statinfo.FBD++;

        if (0 == pchan->yuv_use_cnt)
        {
            if (pchan->output_flush_pending)
            {
                message_queue(pchan->msg_queue, VDEC_MSG_RESP_FLUSH_OUTPUT_DONE, VDEC_S_SUCCESS, HI_NULL);
                pchan->output_flush_pending = 0;
            }

            if (pchan->pause_pending)
            {
                message_queue(pchan->msg_queue, VDEC_MSG_RESP_PAUSE_DONE, VDEC_S_SUCCESS, HI_NULL);
                pchan->pause_pending = 0;
            }
        }

        OmxPrint(OMX_OUTBUF, "VPSS release frame: phy addr = 0x%08x (delete)\n", phyaddr);
    }
    else
    {
        pbuf->status = BUF_STATE_QUEUED;
        OmxPrint(OMX_OUTBUF, "VPSS release frame: phy addr = 0x%08x (requeue)\n", phyaddr);
    }

    return HI_SUCCESS;

}

static HI_S32 processor_report_frame(OMXVDEC_CHAN_CTX *pchan, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_U32 phyaddr = 0;
    unsigned long flags;
    HI_S32 is_find = 0;
    HI_U32 stride = 0;

    OMXVDEC_BUF_S *pbuf = HI_NULL;
    OMXVDEC_BUF_S *ptmp = HI_NULL;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = HI_NULL;
    OMXVDEC_BUF_DESC user_buf;
    memset(&user_buf, 0, sizeof(OMXVDEC_BUF_DESC));

    phyaddr = pstFrame->stBufAddr[0].u32PhyAddr_Y;
    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv);

    if (HI_NULL == pchan || 0 == phyaddr)
    {
        OmxPrint(OMX_FATAL, "%s param invalid!\n", __func__);
        return HI_FAILURE;
    }

    /* for we del list during, so use safe means */
    spin_lock_irqsave(&pchan->yuv_lock, flags);
    if (list_empty(&pchan->yuv_queue))
    {
        spin_unlock_irqrestore(&pchan->yuv_lock, flags);
        OmxPrint(OMX_ERR, "%s: list is empty\n", __func__);
        return HI_FAILURE;
    }

    list_for_each_entry_safe(pbuf, ptmp, &pchan->yuv_queue, list)
    {
       if (phyaddr == (pbuf->phy_addr + pbuf->offset))
       {
           if (pbuf->status != BUF_STATE_USING)
           {
               OmxPrint(OMX_ERR, "%s: buffer(0x%08x) flags confused!\n", __func__, phyaddr);
           }

           is_find = 1;
           pbuf->status = BUF_STATE_IDLE;
           list_del(&pbuf->list);
           break;
       }
    }
    spin_unlock_irqrestore(&pchan->yuv_lock, flags);

    if (!is_find)
    {
       OmxPrint(OMX_ERR, "%s: buffer(0x%08x) not in queue!\n", __func__,  phyaddr);
       return HI_FAILURE;
    }

    /* let out msg to inform application */
    user_buf.dir                            = PORT_DIR_OUTPUT;
    user_buf.bufferaddr                     = (HI_SIZE_T)pbuf->user_vaddr;
    user_buf.buffer_len                     = pbuf->buf_len;
    user_buf.client_data                    = (HI_SIZE_T)pbuf->client_data;
    user_buf.flags                          = VDEC_BUFFERFLAG_ENDOFFRAME;
    user_buf.phyaddr                        = phyaddr;
    user_buf.out_frame.phyaddr_Y            = pstFrame->stBufAddr[0].u32PhyAddr_Y;
    user_buf.out_frame.phyaddr_C            = pstFrame->stBufAddr[0].u32PhyAddr_C;
    user_buf.out_frame.stride               = pstFrame->stBufAddr[0].u32Stride_Y;
    user_buf.out_frame.width                = pstFrame->u32Width;
    user_buf.out_frame.height               = pstFrame->u32Height;
    user_buf.u32FrameIndex                  = pstFrame->u32FrameIndex;
    user_buf.hTunnelSrc                     = pstFrame->hTunnelSrc;
    user_buf.u32FrameRate                   = pstFrame->u32FrameRate;

    pchan->last_frame.u32FrameRate = pstFrame->u32FrameRate;

    if (g_SaveYuvChanNum == pchan->channel_id)
    {
        user_buf.out_frame.save_yuv             = g_SaveYuvEnable;
    }

#if (1 == HI_PROC_SUPPORT)
    if (HI_TRUE == user_buf.out_frame.save_yuv)
    {
        snprintf(user_buf.out_frame.save_path, sizeof(user_buf.out_frame.save_path),
                 "%s/%s_%d_%dx%d.yuv", g_SavePath, g_SaveName, g_SaveNum, pstFrame->u32Width, pstFrame->u32Height);
        user_buf.out_frame.save_path[PATH_LEN-1] = '\0';
    }
#endif

    if (pchan->output_flush_pending)
    {
        OmxPrint(OMX_OUTBUF, "output flush pending, unrelease buffer num: %d\n", pchan->yuv_use_cnt-1);
        user_buf.data_len     = 0;
        user_buf.timestamp    = 0;
    }
    else
    {
        user_buf.timestamp = pstFrame->s64OmxPts;

        stride = processor_get_stride(pchan->protocol, pstFrame->u32Width, pchan->m_use_native_buf);
        user_buf.data_len = DEFAULT_FRAME_SIZE(stride, pstFrame->u32Height);
    }

    pbuf->act_len = user_buf.data_len;
    OmxPrint(OMX_PTS, "Put Time Stamp: %lld\n", user_buf.timestamp);

    if (PROCESSOR_GOT_LAST_FRAME == pchan->last_frame_info[0])
    {
       /* vpss last frame flag */
       if (DEF_HI_DRV_VPSS_LAST_FRAME_FLAG == pstPriv->u32LastFlag)
       {
           user_buf.flags |= VDEC_BUFFERFLAG_EOS;
           pchan->last_frame_info[0] = LAST_FRAME_FLAG_NULL;
           pchan->eof_send_flag++;
           OmxPrint(OMX_INFO, "VPSS report last frame, phyaddr = %x, timestamp = %lld\n", phyaddr, user_buf.timestamp);
       }
    }

    message_queue(pchan->msg_queue, VDEC_MSG_RESP_OUTPUT_DONE, VDEC_S_SUCCESS, &user_buf);
    pchan->omx_chan_statinfo.FBD++;

    pchan->yuv_use_cnt = (pchan->yuv_use_cnt > 0) ? (pchan->yuv_use_cnt-1) : 0;

    if (0 == pchan->yuv_use_cnt)
    {
       if (pchan->output_flush_pending)
       {
           message_queue(pchan->msg_queue, VDEC_MSG_RESP_FLUSH_OUTPUT_DONE, VDEC_S_SUCCESS, HI_NULL);
           pchan->output_flush_pending = 0;
       }

       if (pchan->pause_pending)
       {
           message_queue(pchan->msg_queue, VDEC_MSG_RESP_PAUSE_DONE, VDEC_S_SUCCESS, HI_NULL);
           pchan->pause_pending = 0;
       }
    }

    OmxPrint(OMX_OUTBUF, "VPSS report frame: phy addr = 0x%08x, data_len: %d\n", phyaddr, user_buf.data_len);

    return HI_SUCCESS;
}

static HI_U32 processor_get_stride(HI_U32 protocol, HI_U32 width, HI_BOOL is_native_output)
{
    HI_U32 stride = 0;

    if (is_native_output)
    {
        if ((protocol == STD_HEVC) || (protocol == STD_VP9))
        {
            stride = HI_SYS_GET_STRIDE(HI_OMX_ALIGN_10BIT(width));
        }
        else
        {
            stride = HI_SYS_GET_STRIDE(width);
        }
    }
    else
    {
        stride = HI_OMX_GET_STRIDE(width);
    }

    return stride;
}


static HI_S32 processor_get_frame_buffer(OMXVDEC_CHAN_CTX *pchan, HI_VOID *pstArgs)
{
    HI_S32 ret = HI_FAILURE;
    HI_S32 hPort;
    HI_S32 hVpss;
    HI_U32 ExpectedSize;
    OMXVDEC_IMG_SIZE ImgSize;
    EXTERNAL_FRAME_STORE_S OutFrame;
    HI_DRV_VPSS_FRMBUF_S * pVpssFrm = (HI_DRV_VPSS_FRMBUF_S*)pstArgs;

    hPort = ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->hPort;
    hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
    if (hVpss != pchan->processor_id || hPort != pchan->port_id)
    {
        OmxPrint(OMX_FATAL, "%s hVpss/hPort Not Match!\n", __func__);
        return HI_FAILURE;
    }

    if (pchan->port_enable_flag != 1)
    {
        OmxPrint(OMX_FATAL, "%s state err! port_enable_flag = %d\n", __func__,pchan->port_enable_flag);
        return HI_FAILURE;
    }

    // 检测输出宽高变化，进行上报
    if (pVpssFrm->u32FrmW != pchan->out_width || pVpssFrm->u32FrmH != pchan->out_height)
    {
        OmxPrint(OMX_INFO, "Image size changed: %dx%d stride:%d change to %dx%d stride:%d\n", pchan->out_width, \
                 pchan->out_height, pchan->out_stride, pVpssFrm->u32FrmW, pVpssFrm->u32FrmH, pVpssFrm->u32Stride);

        pchan->out_width  = ImgSize.frame_width  = pVpssFrm->u32FrmW;
        pchan->out_height = ImgSize.frame_height = pVpssFrm->u32FrmH;
        ImgSize.frame_stride = processor_get_stride(pchan->protocol, pchan->out_width, pchan->m_use_native_buf);
        pchan->out_stride = ImgSize.frame_stride;

        pchan->recfg_flag = 1;
        pchan->port_enable_flag = 0;

        channel_report_message(pchan, VDEC_EVT_REPORT_IMG_SIZE_CHG, (HI_VOID *)&ImgSize);

        return HI_FAILURE;
    }

    if (pchan->out_stride >= pchan->out_width)
    {
        pVpssFrm->u32Stride = pchan->out_stride;
    }
    else
    {
        OmxPrint(OMX_FATAL, " %s stride(%d) < width(%d)!\n", __func__, pchan->out_stride, pchan->out_width);

        return HI_FAILURE;
    }

    //ExpectedSize = pVpssFrm->u32Size;
    ExpectedSize = DEFAULT_FRAME_SIZE(pVpssFrm->u32Stride, pVpssFrm->u32FrmH);

    ret = processor_get_frame(pchan, &OutFrame, ExpectedSize);
    if(ret != HI_SUCCESS)
    {
        OmxPrint(OMX_VPSS, "VPSS call vpss_get_frame failed!\n");
        return HI_FAILURE;
    }

    if (OutFrame.PhyAddr != 0)
    {
        pVpssFrm->u32StartPhyAddr = (HI_U32)OutFrame.PhyAddr;
        pVpssFrm->pu8StartVirAddr = OutFrame.VirAddr;

        //PrivatePhyAddr = -1 表示未分配metadata，此时也传递给VPSS，给VPSS作判断用
        pVpssFrm->u32PrivDataPhyAddr = (HI_U32)OutFrame.PrivatePhyAddr;
        pVpssFrm->pu8PrivDataVirAddr = OutFrame.PrivateVirAddr;
        pVpssFrm->u32PrivDataSize    = (HI_U32)OutFrame.PrivateLength;
    }
    else
    {
        OmxPrint(OMX_FATAL, "%s phy address is invalid\n", __func__);
        return HI_FAILURE;
    }

    OmxPrint(OMX_VPSS, "VPSS get frame buffer success! Phyaddr = 0x%x\n",pVpssFrm->u32StartPhyAddr);

    return HI_SUCCESS;

}

static HI_S32 processor_release_frame_buffer(OMXVDEC_CHAN_CTX *pchan, HI_VOID *pstArgs)
{
    HI_S32 ret = HI_FAILURE;
    VPSS_HANDLE hPort;
    VPSS_HANDLE hVpss;
    HI_U32 PhyAddr;
    HI_DRV_VIDEO_FRAME_S *pstFrame = &((HI_DRV_VPSS_FRMINFO_S*)pstArgs)->stFrame;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv);

    hPort = ((HI_DRV_VPSS_FRMINFO_S*)pstArgs)->hPort;
    hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
    if (hVpss != pchan->processor_id || hPort != pchan->port_id)
    {
        OmxPrint(OMX_FATAL, "%s hVpss/hPort Not Match!\n", __func__);
        return HI_FAILURE;
    }

    if (PROCESSOR_GOT_LAST_FRAME == pchan->last_frame_info[0])
    {
      /* vpss last frame flag */
       if (DEF_HI_DRV_VPSS_LAST_FRAME_FLAG == pstPriv->u32LastFlag)
       {
           OmxPrint(OMX_ERR, "VPSS release last frame!!\n");
           pchan->last_frame_info[0] = LAST_FRAME_FLAG_NULL;
       }
    }

    PhyAddr = ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->u32StartPhyAddr;
    ret = processor_release_frame(pchan, PhyAddr);
    if(ret != HI_SUCCESS)
    {
        OmxPrint(OMX_ERR, "%s call vpss_release_frame failed.Phyaddr = 0x%x\n", __func__, PhyAddr);
        return HI_FAILURE;
    }

    OmxPrint(OMX_VPSS, "VPSS release frame buffer success! Phyaddr = 0x%x\n", PhyAddr);

    return HI_SUCCESS;

}

static HI_S32 processor_report_new_frame(OMXVDEC_CHAN_CTX *pchan, HI_VOID *pstArgs)
{
    HI_S32 ret = HI_FAILURE;
    VPSS_HANDLE hPort;
    VPSS_HANDLE hVpss;
    HI_DRV_VIDEO_FRAME_S *pstFrame;
    HI_DRV_VID_FRAME_ADDR_S stBakBufAddr;
    HI_DRV_VIDEO_PRIVATE_S *pstPrivInfo = HI_NULL;
    HI_VDEC_PRIV_FRAMEINFO_S *pstvdecPrivInfo = HI_NULL;

    hPort = ((HI_DRV_VPSS_FRMINFO_S*)pstArgs)->hPort;
    hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
    if (hVpss != pchan->processor_id || hPort != pchan->port_id)
    {
        OmxPrint(OMX_FATAL, "%s hVpss/hPort Not Match!\n", __func__);
        return HI_FAILURE;
    }

    pstFrame = &((HI_DRV_VPSS_FRMINFO_S*)pstArgs)->stFrame;
    //g_SaveYuvEnable = HI_TRUE;
    ret = processor_report_frame(pchan, pstFrame);
    if(ret != HI_SUCCESS)
    {
        OmxPrint(OMX_ERR, "%s report frame failed!\n", __func__);
        return HI_FAILURE;
    }
    if (pstFrame->eFrmType != HI_DRV_FT_NOT_STEREO)
    {
        memcpy(&stBakBufAddr, &pstFrame->stBufAddr[0], sizeof(HI_DRV_VID_FRAME_ADDR_S));
        memcpy(&pstFrame->stBufAddr[0], &pstFrame->stBufAddr[1], sizeof(HI_DRV_VID_FRAME_ADDR_S));
        //g_SaveYuvEnable = HI_FALSE;
        ret = processor_report_frame(pchan, pstFrame);
        memcpy(&pstFrame->stBufAddr[0], &stBakBufAddr, sizeof(HI_DRV_VID_FRAME_ADDR_S));
        if(ret != HI_SUCCESS)
        {
            OmxPrint(OMX_ERR, "%s report right frame failed!\n", __func__);
            return HI_FAILURE;
        }
    }

    if((pchan->bLowdelay || g_FastOutputMode) && (NULL != pchan->omx_vdec_lowdelay_proc_rec))
    {
        pstPrivInfo     = (HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv);
        pstvdecPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S*)(pstPrivInfo->u32Reserve);
        channel_add_lowdelay_tag_time(pchan, pstvdecPrivInfo->u32Usertag, OMX_LOWDELAY_REC_VPSS_RPO_IMG_TIME, OMX_GetTimeInMs());
    }

    pchan->omx_chan_statinfo.ProcessorOut++;

    OmxPrint(OMX_VPSS, "VPSS report new frame! Phyaddr = 0x%x,id = %d\n", pstFrame->stBufAddr[0].u32PhyAddr_Y, pstFrame->u32FrameIndex);

    return HI_SUCCESS;

}

static HI_S32 processor_report_last_frame(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;
    EXTERNAL_FRAME_STORE_S stFrameStore;
    HI_DRV_VIDEO_FRAME_S stLastFrame;
    HI_DRV_VIDEO_PRIVATE_S *pVideoPri = (HI_DRV_VIDEO_PRIVATE_S *)(stLastFrame.u32Priv);
    HI_S32 FailedNum = 0;

    do
    {
        ret = processor_get_frame(pchan, &stFrameStore, 0);
        if (HI_SUCCESS == ret)
        {
            memset(&stLastFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
            pVideoPri->u32LastFlag = DEF_HI_DRV_VPSS_LAST_FRAME_FLAG;
            stLastFrame.stBufAddr[0].u32PhyAddr_Y = stFrameStore.PhyAddr;

            if (HI_SUCCESS == processor_report_frame(pchan, &stLastFrame))
            {
                OmxPrint(OMX_INFO, "VPSS report a fade last frame.\n");
            }
            else
            {
                OmxPrint(OMX_INFO, "VPSS report fade last frame failed!\n");
            }
        }
        else
        {
            FailedNum++;
            OmxPrint(OMX_INFO, "Get last frame buffer failed, retry!\n");
            msleep(5);
        }

        if (FailedNum > 500)
        {
            return HI_FAILURE;
        }

    }while(ret != HI_SUCCESS);

    return HI_SUCCESS;
}

static HI_S32 processor_event_handler(HI_HANDLE ChanId, HI_DRV_VPSS_EVENT_E enEventID, HI_VOID *pstArgs)
{
    HI_S32 ret = HI_FAILURE;
    OMXVDEC_CHAN_CTX *pchan = HI_NULL;

    if (HI_NULL == pstArgs)
    {
        OmxPrint(OMX_FATAL, "%s enEventID = %d, pstArgs = NULL!\n", __func__, enEventID);
        return HI_FAILURE;
    }

    pchan = channel_find_inst_by_channel_id(g_OmxVdec, ChanId);
    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s can't find Chan(%d).\n", __func__, ChanId);
         return HI_FAILURE;
    }

    /* Event handle */
    switch (enEventID)
    {
        case VPSS_EVENT_CHECK_FRAMEBUFFER:
            ret = channel_check_avaible_frame_buffer(pchan);
            if (HI_SUCCESS == ret)
            {
                ((HI_DRV_VPSS_BUFINFO_S*)pstArgs)->bAvailable = HI_TRUE;
            }
            else
            {
                ((HI_DRV_VPSS_BUFINFO_S*)pstArgs)->bAvailable = HI_FALSE;
            }
            break;

        case  VPSS_EVENT_BUFLIST_FULL:
            //VPSS使用外部BUFFER，不应该报这个消息
            OmxPrint(OMX_WARN, "%s, VPSS_EVENT_BUFLIST_FULL, not expected.\n", __func__);
            break;

        case  VPSS_EVENT_GET_FRMBUFFER:
            ret = processor_get_frame_buffer(pchan, pstArgs);
            if(ret != HI_SUCCESS)
            {
                return HI_FAILURE;
            }
            break;

        case  VPSS_EVENT_REL_FRMBUFFER:
            ret = processor_release_frame_buffer(pchan, pstArgs);
            if(ret != HI_SUCCESS)
            {
                return HI_FAILURE;
            }
            break;

        case  VPSS_EVENT_TASK_COMPLETE:
            break;

        case  VPSS_EVENT_NEW_FRAME:
            ret = processor_report_new_frame(pchan, pstArgs);
            if(ret != HI_SUCCESS)
            {
                return HI_FAILURE;
            }
            break;

        default:
            OmxPrint(OMX_ERR, "%s Unknow enEventID: %d\n", __func__, enEventID);
            return HI_FAILURE;
    }

    return HI_SUCCESS;

}

static HI_VOID processor_convert_desc_info_part1(OMXVDEC_CHAN_CTX *pchan, IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_BOOL IsCmpFlag = HI_FALSE;

    pstFrame->bTopFieldFirst = ((pstImage->format & 0x3000) != 0) ? HI_TRUE : HI_FALSE;

    if (pchan->protocol == STD_H263 || pchan->protocol == STD_SORENSON)
    {
        pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21;
    }
    else
    {
        IsCmpFlag = (pchan->is_compress == HI_TRUE) && ((pstImage->format >> 18) & 0x1);

        if ((pstImage->format>>17) & 0x1)
        {
            pstFrame->ePixFormat = (IsCmpFlag == HI_TRUE) ? HI_DRV_PIX_FMT_NV21_CMP : HI_DRV_PIX_FMT_NV21;
        }
        else
        {
            pstFrame->ePixFormat = (IsCmpFlag == HI_TRUE) ? HI_DRV_PIX_FMT_NV21_TILE_CMP : HI_DRV_PIX_FMT_NV21_TILE;
        }
    }

    if (pchan->protocol == STD_VP6 || pchan->protocol == STD_VP6F || pchan->protocol == STD_VP6A)
    {
        pstFrame->u32Circumrotate = (STD_VP6A == pchan->protocol) ? pchan->bReversed&0x1 : !(pchan->bReversed&0x1);
    }
    else
    {
        pstFrame->u32Circumrotate = 0;
    }

    if (pstImage->disp_height <= 288
    || (pstImage->disp_width  >= 3840 && pstImage->disp_height >= 2160))
    {
        pstFrame->bProgressive= HI_TRUE;
    }
    else
    {
        if ((pstImage->format & 0x300) == 0     /* PROGRESSIVE */
         || (pstImage->format & 0x300) == 0x200 /* INFERED PROGRESSIVE */)
        {
            pstFrame->bProgressive = HI_TRUE;
        }
        else
        {
            pstFrame->bProgressive = HI_FALSE;
        }
    }

    pchan->progress = pstFrame->bProgressive;

    return;
}

static HI_VOID processor_convert_desc_info_part2(OMXVDEC_CHAN_CTX *pchan, IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    switch (pstImage->format & 0xC00)
    {
        case 0x400:
            pstFrame->enFieldMode = HI_DRV_FIELD_TOP;
            break;
        case 0x800:
            pstFrame->enFieldMode = HI_DRV_FIELD_BOTTOM;
            break;
        case 0xC00:
            pstFrame->enFieldMode = HI_DRV_FIELD_ALL;
            break;
        default:
            pstFrame->enFieldMode = HI_DRV_FIELD_BUTT;
            break;
    }

    if ((pchan->protocol == STD_HEVC  || pchan->protocol == STD_VP9)
     && (pstImage->bit_depth_luma > 8 || pstImage->bit_depth_chroma > 8))
    {
        pstFrame->enBitWidth = HI_DRV_PIXEL_BITWIDTH_10BIT;
        pchan->is_10bit = HI_TRUE;
    }
    else
    {
        pstFrame->enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
        pchan->is_10bit = HI_FALSE;
    }

    pstFrame->u32AspectWidth  = pstImage->u32AspectWidth;
    pstFrame->u32AspectHeight = pstImage->u32AspectHeight;
    pstFrame->u32Width        = pstImage->disp_width;
    pstFrame->u32Height       = pstImage->disp_height;
    pstFrame->u32ErrorLevel   = pstImage->error_level;
    pstFrame->s64OmxPts       = pstImage->SrcPts;
    pstFrame->u32FrameIndex   = pstImage->seq_img_cnt;
    pstFrame->hTunnelSrc      = (HI_ID_VDEC << 16) | ((HI_U32)pchan->channel_id);

    if (pchan->protocol == STD_MVC)
    {
        switch (pstImage->eFramePackingType)
        {
        case FRAME_PACKING_TYPE_NONE:
            pstFrame->eFrmType = HI_DRV_FT_NOT_STEREO;
            break;
        case FRAME_PACKING_TYPE_SIDE_BY_SIDE:
            pstFrame->eFrmType = HI_DRV_FT_SBS;
            break;
        case FRAME_PACKING_TYPE_TOP_BOTTOM:
            pstFrame->eFrmType = HI_DRV_FT_TAB;
            break;
        case FRAME_PACKING_TYPE_TIME_INTERLACED:
            pstFrame->eFrmType = HI_DRV_FT_FPK;
            break;
        default:
            pstFrame->eFrmType = FRAME_PACKING_TYPE_BUTT;
            break;
        }
    }

#ifdef HI_OMX_TEE_SUPPORT
    pstFrame->bSecure = (pstImage->is_SecureFrame == 1) ? HI_TRUE : HI_FALSE;
#endif

    return;
}

static HI_VOID processor_convert_addr_info(OMXVDEC_CHAN_CTX *pchan, IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    if (pchan->protocol == STD_H263 || pchan->protocol == STD_SORENSON || pchan->protocol == STD_MJPEG)
    {
        pstFrame->stBufAddr[0].u32PhyAddr_YHead    = pstImage->top_luma_phy_addr;
        pstFrame->stBufAddr[0].u32PhyAddr_Y        = pstImage->luma_phy_addr;
        pstFrame->stBufAddr[0].u32Stride_Y         = pstImage->image_stride;
        pstFrame->stBufAddr[0].u32PhyAddr_C        = pstImage->top_chrom_phy_addr;
        pstFrame->stBufAddr[0].u32Stride_C         = pstFrame->stBufAddr[0].u32Stride_Y;
    }
    else
    {
        if ((pchan->is_compress == HI_FALSE) || (((pstImage->format >> 18) & 0x1) == 0))
        {
            pstFrame->stBufAddr[0].u32PhyAddr_YHead    = pstImage->top_luma_phy_addr;//非压缩时，也赋值，保证不会出错
            pstFrame->stBufAddr[0].u32PhyAddr_Y        = pstImage->luma_phy_addr;
            pstFrame->stBufAddr[0].u32Stride_Y         = pstImage->image_stride / 16;

            pstFrame->stBufAddr[0].u32PhyAddr_CHead    = pstImage->top_chrom_phy_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_C        = pstImage->top_chrom_phy_addr;
            pstFrame->stBufAddr[0].u32Stride_C         = pstFrame->stBufAddr[0].u32Stride_Y;
        }
        else
        {
            pstFrame->stBufAddr[0].u32PhyAddr_YHead    = pstImage->top_luma_phy_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_Y        = pstImage->top_luma_phy_addr + pstImage->head_info_size;
            pstFrame->stBufAddr[0].u32Stride_Y         = pstImage->image_stride / 16;
            pstFrame->stBufAddr[0].u32PhyAddr_CHead    = pstImage->top_chrom_phy_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_C        = pstImage->top_chrom_phy_addr + (pstImage->head_info_size / 2);
            pstFrame->stBufAddr[0].u32Stride_C         = pstFrame->stBufAddr[0].u32Stride_Y;
            pstFrame->stBufAddr[0].u32Head_Stride      = pstImage->head_stride;
            pstFrame->stBufAddr[0].u32Head_Size        = pstImage->head_info_size;
        }

        if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
        {
            pstFrame->stBufAddr_LB[0].u32PhyAddr_Y = pstImage->luma_2bit_phy_addr;
            pstFrame->stBufAddr_LB[0].u32PhyAddr_C = pstImage->chrom_2bit_phy_addr;

        #if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
            pstFrame->stBufAddr_LB[0].u32Stride_Y  = pstImage->image_stride_2bit / 16;
            pstFrame->stBufAddr_LB[0].u32Stride_C  = pstImage->image_stride_2bit / 16;
        #else
            pstFrame->stBufAddr_LB[0].u32Stride_Y  = pstImage->image_stride_2bit / 32;
            pstFrame->stBufAddr_LB[0].u32Stride_C  = pstImage->image_stride_2bit / 32;
        #endif
        }
    }

    //FOR MVC DEBUG
    if (pstImage->is_3D)
    {
        pstFrame->stBufAddr[1].u32PhyAddr_Y        = pstImage->top_luma_phy_addr_1;
        pstFrame->stBufAddr[1].u32Stride_Y         = pstImage->image_stride/16;
        pstFrame->stBufAddr[1].u32PhyAddr_C        = pstImage->top_chrom_phy_addr_1;
        pstFrame->stBufAddr[1].u32Stride_C         = pstFrame->stBufAddr[1].u32Stride_Y;
    }

    if (pchan->bLowdelay == HI_TRUE || g_FastOutputMode == HI_TRUE)
    {
        pstFrame->u32TunnelPhyAddr = pstImage->line_num_phy_addr;
    }
    else
    {
        pstFrame->u32TunnelPhyAddr = 0;
    }

    return;
}

static HI_VOID processor_convert_hdr_info(OMXVDEC_CHAN_CTX *pchan, IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    pstFrame->enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;
    memset(&pstFrame->unHDRInfo.stDolbyInfo, 0 , sizeof(HI_DRV_VIDEO_DOLBY_INFO_S));
    memset(&pstFrame->unHDRInfo.stHDR10Info, 0 , sizeof(HI_DRV_VIDEO_HDR10_INFO_S));
    memset(&pstFrame->unHDRInfo, 0 , sizeof(pstFrame->unHDRInfo));

    if ((pstFrame->enSrcFrameType != HI_DRV_VIDEO_FRAME_TYPE_DOLBY_BL) && (pstFrame->enSrcFrameType != HI_DRV_VIDEO_FRAME_TYPE_DOLBY_EL))
    {
        OMXVDEC_SetHDRInfo(pchan->protocol, pstImage, pstFrame);

        pchan->omx_chan_hdr_info.enSrcFrameType = pstFrame->enSrcFrameType;
        pchan->omx_chan_hdr_info.CodecType      = pstFrame->enCodecType;
        memcpy(&pchan->omx_chan_hdr_info.stColourInfo, &pstFrame->stColourInfo, sizeof(pstFrame->stColourInfo));
        memcpy(&pchan->omx_chan_hdr_info.unHDRInfo, &pstFrame->unHDRInfo, sizeof(pstFrame->unHDRInfo));
    }

    return;
}

static HI_VOID processor_convert_pts_info(OMXVDEC_CHAN_CTX *pchan, IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32fpsInteger = 0;
    HI_U32 u32fpsDecimal = 0;
    HI_U32 remainder;
    HI_U32 u32FrameRate = 0;

    remainder = do_div(pstImage->SrcPts, 1000);
    pstImage->SrcPts = (remainder >= 500)? (pstImage->SrcPts + 1):pstImage->SrcPts;

    /* Calculate PTS */
    OMX_PTSREC_CalcStamp(pchan->channel_id, pstImage);

    pstImage->SrcPts= pstImage->SrcPts * 1000 + remainder;
    u32fpsInteger                               = pstImage->frame_rate/1024;
    u32fpsDecimal                               = pstImage->frame_rate%1024;
    pstFrame->u32FrameRate                      = u32fpsInteger*1000 + u32fpsDecimal ;
    pchan->last_frame.u32SrcFrameRate           = pstFrame->u32FrameRate;

    if (pstFrame->u32FrameRate > 30000 && pstFrame->u32FrameRate != pchan->last_frame.u32FrameRate)
    {
        u32FrameRate = pstFrame->u32FrameRate/1000;

        ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_SET_FRAME_RATE, &u32FrameRate);
        if(VDEC_OK != ret)
        {
            OmxPrint(OMX_ERR, "VDEC_CID_SET_FRAME_RATE falled!\n");
        }
    }

    return;
}

static HI_VOID processor_convert_private_info(OMXVDEC_CHAN_CTX *pchan, IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_DRV_VIDEO_PRIVATE_S *pstPrivInfo = HI_NULL;
    HI_VDEC_PRIV_FRAMEINFO_S *pstvdecPrivInfo = HI_NULL;

    pstPrivInfo = (HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv);
    pstPrivInfo->u32BufferID = pstImage->image_id;

    if (pstImage->image_width >= 1280 && pstImage->image_height >= 720)
    {
        pstPrivInfo->eColorSpace = HI_DRV_CS_BT709_YUV_LIMITED;
    }
    else
    {
        pstPrivInfo->eColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
    }

    pstPrivInfo->stVideoOriginalInfo.enSource   = HI_DRV_SOURCE_DTV;
    pstPrivInfo->stVideoOriginalInfo.u32Width   = pstImage->disp_width;
    pstPrivInfo->stVideoOriginalInfo.u32Height  = pstImage->disp_height;
    pstPrivInfo->stVideoOriginalInfo.u32FrmRate = pstFrame->u32FrameRate;
    pstPrivInfo->stVideoOriginalInfo.en3dType   = pstFrame->eFrmType;
    pstPrivInfo->stVideoOriginalInfo.enSrcColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
    pstPrivInfo->stVideoOriginalInfo.enColorSys      = HI_DRV_COLOR_SYS_AUTO;
    pstPrivInfo->stVideoOriginalInfo.bGraphicMode    = HI_FALSE;
    pstPrivInfo->stVideoOriginalInfo.bInterlace      = pstFrame->bProgressive;

    pstvdecPrivInfo                             = (HI_VDEC_PRIV_FRAMEINFO_S*)(pstPrivInfo->u32Reserve);
    pstvdecPrivInfo->image_id                   = pstImage->image_id;
    pstvdecPrivInfo->image_id_1                 = pstImage->image_id_1;
    pstvdecPrivInfo->stBTLInfo.u32BTLImageID    = pstImage->BTLInfo.btl_imageid;
    pstvdecPrivInfo->stBTLInfo.u32Is1D          = pstImage->BTLInfo.u32Is1D;
    pstvdecPrivInfo->u32Usertag                 = pstImage->Usertag;

    return;
}

static HI_S32 processor_convert_image(OMXVDEC_CHAN_CTX *pchan, IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    if (pchan == HI_NULL || pstImage == HI_NULL || pstFrame == HI_NULL)
    {
        OmxPrint(OMX_WARN, "%s param is invalid\n", __func__);

        return HI_FAILURE;
    }

    /* Desc info*/
    processor_convert_desc_info_part1(pchan, pstImage, pstFrame);
    processor_convert_desc_info_part2(pchan, pstImage, pstFrame);

    /* Addr info*/
    processor_convert_addr_info(pchan, pstImage, pstFrame);

    /* HDR info*/
    processor_convert_hdr_info(pchan, pstImage, pstFrame);

    /* PTS info*/
    processor_convert_pts_info(pchan, pstImage, pstFrame);

    /* Private info*/
    processor_convert_private_info(pchan, pstImage, pstFrame);

    return HI_SUCCESS;
}

static HI_S32 processor_get_image(OMXVDEC_CHAN_CTX *pchan, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 ret = HI_FAILURE;
    IMAGE stImage;
#ifdef VFMW_VPSS_BYPASS_EN
    unsigned long   flags;
#endif

    if (pstFrame == HI_NULL)
    {
        OmxPrint(OMX_FATAL, "%s pstFrame = NULL!\n", __func__);
        return HI_FAILURE;
    }

    if (pchan->reset_pending)
    {
        OmxPrint(OMX_WARN, "%s channel reset pending...\n", __func__);
        return HI_FAILURE;
    }

    /* read ready image struct from vfmw. */
    //memset(&stImage, 0, sizeof(IMAGE));
#ifdef VFMW_VPSS_BYPASS_EN
    spin_lock_irqsave(&g_OmxVdec->stRemainFrmList.bypass_lock, flags);
#endif

    ret = pchan->image_ops.read_image(pchan->decoder_id, &stImage);
    if (g_BypassVpss == HI_TRUE && ret == HI_SUCCESS)
    {
        ret = pchan->image_ops.release_image(pchan->decoder_id, &stImage);
        ret = HI_FAILURE;
    }

#ifdef VFMW_VPSS_BYPASS_EN
    spin_unlock_irqrestore(&g_OmxVdec->stRemainFrmList.bypass_lock, flags);
#endif
    if (ret != HI_SUCCESS)
    {
        if (pchan->last_frame_info[0] == DECODER_REPORT_LAST_FRAME)
        {
            /* 最后一帧已经被拿走 / 最后一帧上报失败的情况处理 */
            if((pchan->last_frame_processor_got == pchan->last_frame_image_id)
            || (pchan->last_frame_info[1]) == REPORT_LAST_FRAME_FAIL)
            {
               OmxPrint(OMX_INFO, "VPSS got no frame to handle, report a fake one!\n");
               pchan->last_frame_info[0] = PROCESSOR_GOT_LAST_FRAME;
               ret = processor_report_last_frame(pchan); /* 输出假的最后一帧 */

               if (ret != HI_SUCCESS)
               {
                   OmxPrint(OMX_ERR, "VPSS report fade last frame failed!\n");
               }
            }
        }
        OmxPrint(OMX_VPSS, "VPSS read_image failed!\n");

        return HI_FAILURE;
    }

    /*Change IMAGE to HI_DRV_VIDEO_FRAME_S*/
    processor_convert_image(pchan, &stImage, pstFrame);

    if (1 == stImage.last_frame)
    {
        pchan->last_frame_image_id = REALID(stImage.image_id);
    }

    pchan->last_frame_processor_got = REALID(stImage.image_id);

    OmxPrint(OMX_VPSS, "VPSS read image success!Phyaddr = 0x%x,id = %d\n",pstFrame->stBufAddr[0].u32PhyAddr_Y,pstFrame->u32FrameIndex);

    if((pchan->bLowdelay || g_FastOutputMode) && (NULL != pchan->omx_vdec_lowdelay_proc_rec))
    {
        channel_add_lowdelay_tag_time(pchan, stImage.Usertag, OMX_LOWDELAY_REC_VPSS_RCV_IMG_TIME, OMX_GetTimeInMs());
    }
    pchan->omx_chan_statinfo.ProcessorIn++;

    return HI_SUCCESS;
}

HI_S32 processor_release_image(OMXVDEC_CHAN_CTX *pchan, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 ret = HI_FAILURE;
    IMAGE stImage;
    HI_DRV_VIDEO_PRIVATE_S* pstPrivInfo = HI_NULL;
    HI_VDEC_PRIV_FRAMEINFO_S* pstVdecPrivInfo = HI_NULL;
    unsigned long   flags;
#ifdef VFMW_VPSS_BYPASS_EN
    HI_U32 OccupiedFrmIndex = 0;
#endif
    if (HI_NULL == pstFrame)
    {
        OmxPrint(OMX_FATAL, "%s pstFrame = NULL!\n", __func__);

        return HI_FAILURE;
    }

    pstPrivInfo = (HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv);
    if (DEF_HI_DRV_VPSS_LAST_ERROR_FLAG == pstPrivInfo->u32LastFlag)
    {
        /*this is fake frame*/
        OmxPrint(OMX_VPSS, "%s release fake frame!\n", __func__);
        return HI_SUCCESS;
    }

    pstVdecPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S*)(pstPrivInfo->u32Reserve);

    memset(&stImage, 0, sizeof(IMAGE));
    stImage.image_stride         = pstFrame->stBufAddr[0].u32Stride_Y;
    stImage.disp_height          = pstFrame->u32Height;
    stImage.disp_width           = pstFrame->u32Width;
    stImage.luma_phy_addr        = pstFrame->stBufAddr[0].u32PhyAddr_Y;
    stImage.top_luma_phy_addr    = pstFrame->stBufAddr[0].u32PhyAddr_YHead;

    stImage.image_id             = pstVdecPrivInfo->image_id;
    stImage.image_id_1           = pstVdecPrivInfo->image_id_1;
    stImage.BTLInfo.btl_imageid  = pstVdecPrivInfo->stBTLInfo.u32BTLImageID;
    stImage.BTLInfo.u32Is1D      = pstVdecPrivInfo->stBTLInfo.u32Is1D;

#ifdef VFMW_VPSS_BYPASS_EN
    spin_lock_irqsave(&g_OmxVdec->stRemainFrmList.bypass_lock, flags);
#endif

    ret = pchan->image_ops.release_image(pchan->decoder_id, &stImage);

#ifdef VFMW_VPSS_BYPASS_EN
    spin_unlock_irqrestore(&g_OmxVdec->stRemainFrmList.bypass_lock, flags);
#endif

    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_ERR, "%s call release_image failed!\n", __func__);

    #ifdef VFMW_VPSS_BYPASS_EN
        if ((0 != g_OmxVdec->stRemainFrmList.s32Num) && (channel_IsOccupiedFrm(pstFrame->stBufAddr[0].u32PhyAddr_YHead, &OccupiedFrmIndex)))
        {
            /*this channel have special frame*/
            if (OccupiedFrmIndex >= VFMW_MAX_RESERVE_NUM)
            {
               OmxPrint(OMX_FATAL, "%s release_occupied_frame[id = %d] failed!\n", __func__,OccupiedFrmIndex);

               return HI_FAILURE;
            }

            OmxPrint(OMX_INFO, "processor_release_image PhyAddr= 0x%x\n",pstFrame->stBufAddr[0].u32PhyAddr_YHead);

            spin_lock_irqsave(&g_OmxVdec->stRemainFrmList.bypass_lock,flags);
            g_OmxVdec->stRemainFrmList.stSpecialFrmRec[OccupiedFrmIndex].bCanRls = HI_TRUE;
            spin_unlock_irqrestore(&g_OmxVdec->stRemainFrmList.bypass_lock,flags);

            g_OmxVdec->task.task_state = TASK_STATE_ONCALL;
            wake_up(&g_OmxVdec->task.task_wait);

            OmxPrint(OMX_VPSS, "VPSS release image success! Phyaddr = 0x%x,id = %d\n",pstFrame->stBufAddr[0].u32PhyAddr_YHead, pstFrame->u32FrameIndex);

            return HI_SUCCESS;
        }
    #endif

        return HI_FAILURE;
    }

    OmxPrint(OMX_VPSS, "VPSS release image success! Phyaddr = 0x%x\n",pstFrame->stBufAddr[0].u32PhyAddr_Y);
    return HI_SUCCESS;
}

static HI_S32 processor_get_src_frame(HI_S32 VpssId, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 ret;
    OMXVDEC_CHAN_CTX *pchan = HI_NULL;

    pchan = channel_find_inst_by_processor_id(g_OmxVdec, VpssId);
    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_WARN, "%s can't find %d vpss channel\n", __func__, VpssId);
        return HI_FAILURE;
    }

    ret = processor_get_image(pchan, pstFrame);
    if (ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    if (pchan->is_tvp == HI_FALSE)
    {
        processor_save_src_frame(pchan, pstFrame);
    }

    return HI_SUCCESS;
}

static HI_S32 processor_release_src_frame(HI_S32 VpssId, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 ret;
    OMXVDEC_CHAN_CTX *pchan = HI_NULL;

    pchan = channel_find_inst_by_processor_id(g_OmxVdec, VpssId);
    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_WARN, "%s can't find %d vpss channel\n", __func__, VpssId);
        return HI_FAILURE;
    }

    ret = processor_release_image(pchan, pstFrame);

    return ret;
}

/*============== EXPORT FUNCTION =============*/
HI_S32 processor_init(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    pVpssFunc = HI_NULL;

    /*Get vpss functions*/
    ret = HI_DRV_MODULE_GetFunction(HI_ID_VPSS, (HI_VOID**)&g_stOmxFunc.pProcessorFunc);
    if (HI_SUCCESS != ret || HI_NULL == g_stOmxFunc.pProcessorFunc)
    {
        OmxPrint(OMX_FATAL, "%s get vpss function failed!\n", __func__);
        return HI_FAILURE;
    }

    pVpssFunc = (VPSS_EXPORT_FUNC_S *)(g_stOmxFunc.pProcessorFunc);

    ret = (pVpssFunc->pfnVpssGlobalInit)();
    if (ret != HI_SUCCESS)
    {
        pVpssFunc = g_stOmxFunc.pProcessorFunc = HI_NULL;
        OmxPrint(OMX_FATAL, "%s init vpss failed!\n", __func__);
        return HI_FAILURE;
    }

    memset(&g_YUV_ExchangeBuffer, 0, sizeof(g_YUV_ExchangeBuffer));

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 processor_exit(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (g_YUV_ExchangeBuffer.u32Size > 0)
    {
        HI_DRV_OMXVDEC_UnmapAndRelease(&g_YUV_ExchangeBuffer);
    }

    ret = (pVpssFunc->pfnVpssGlobalDeInit)();
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s exit vpss failed!\n", __func__);
        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 processor_create_inst(OMXVDEC_CHAN_CTX *pchan, OMX_PIX_FORMAT_E color_format)
{
    HI_S32 ret;
    HI_DRV_VPSS_CFG_S stVpssCfg;
    HI_DRV_VPSS_SOURCE_FUNC_S stRegistSrcFunc;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s pchan = NULL!\n", __func__);
        return HI_FAILURE;
    }

    pchan->processor_id  = VPSS_INVALID_HANDLE;
    pchan->port_id       = VPSS_INVALID_HANDLE;

    ret = (pVpssFunc->pfnVpssGetDefaultCfg)(&stVpssCfg);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_GetDefaultCfg failed, ret = %d\n", __func__, ret);
        return HI_FAILURE;
    }

#ifdef HI_OMX_TEE_SUPPORT
    stVpssCfg.bSecure = pchan->is_tvp;
#endif

    stVpssCfg.bProgRevise = g_DeInterlaceEnable; //开关DEI
    ret = (pVpssFunc->pfnVpssCreateVpss)(&stVpssCfg, &pchan->processor_id);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_CreateVpss failed, ret = %d\n", __func__, ret);
        return HI_FAILURE;
    }

    ret = (pVpssFunc->pfnVpssRegistHook)(pchan->processor_id, pchan->channel_id, processor_event_handler);
    if (ret != HI_SUCCESS)
    {
         OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_RegistHook failed, ret = %d\n", __func__, ret);
         goto error;
    }

    stRegistSrcFunc.VPSS_GET_SRCIMAGE = processor_get_src_frame;
    stRegistSrcFunc.VPSS_REL_SRCIMAGE = processor_release_src_frame;
    ret = (pVpssFunc->pfnVpssSetSourceMode)(pchan->processor_id, VPSS_SOURCE_MODE_VPSSACTIVE, &stRegistSrcFunc);
    if (ret != HI_SUCCESS)
    {
         OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_SetSourceMode failed, ret = %d\n", __func__, ret);
         goto error;
    }

    ret = (pVpssFunc->pfnVpssGetDefaultPortCfg)(&stVpssPortCfg);
    if (ret != HI_SUCCESS)
    {
         OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_GetDefaultPortCfg failed, ret = %d\n", __func__, ret);
         goto error;
    }

    pchan->color_format                 = color_format;
    stVpssPortCfg.s32OutputWidth        = 0;    // 宽高设为0表示根据输入自适应配置
    stVpssPortCfg.s32OutputHeight       = 0;
    stVpssPortCfg.eFormat               = processor_color_omx_to_hal(color_format);
    stVpssPortCfg.enOutBitWidth         = HI_DRV_PIXEL_BITWIDTH_10BIT;
    stVpssPortCfg.stBufListCfg.eBufType = HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE;
    stVpssPortCfg.u32MaxFrameRate       = (g_FrameRateLimit == HI_TRUE) ? 30 : 30000;

    if (STD_MVC == pchan->protocol)
    {
        stVpssPortCfg.b3Dsupport = HI_TRUE;
    }

    if ( HI_TRUE == pchan->bLowdelay || HI_TRUE == g_FastOutputMode)
    {
        stVpssPortCfg.bTunnelEnable = HI_TRUE;
        stVpssPortCfg.s32SafeThr = 10;
    }

    ret = (pVpssFunc->pfnVpssCreatePort)(pchan->processor_id, &stVpssPortCfg, &pchan->port_id);
    if (ret != HI_SUCCESS)
    {
         OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_CreatePort failed, ret = %d\n", __func__, ret);
         goto error;
    }

    ret = (pVpssFunc->pfnVpssGetPortCfg)(pchan->port_id, &stVpssPortCfg);
    if (ret != HI_SUCCESS)
    {
         OmxPrint(OMX_FATAL, "%s call pfnVpssGetPortCfg failed, ret = %d\n", __func__, ret);
         goto error;
    }

    switch (pchan->rotation)
    {
        case 0:
            stVpssPortCfg.enRotation = HI_DRV_VPSS_ROTATION_DISABLE;
            break;

        case 90:
            stVpssPortCfg.enRotation = HI_DRV_VPSS_ROTATION_90;
            break;

        case 180:
            stVpssPortCfg.enRotation = HI_DRV_VPSS_ROTATION_180;
            break;

        case 270:
            stVpssPortCfg.enRotation = HI_DRV_VPSS_ROTATION_270;
            break;

        default:
            OmxPrint(OMX_ERR, "%s, value is invalid rotation:%d\n", __func__, pchan->rotation);
            stVpssPortCfg.enRotation = HI_DRV_VPSS_ROTATION_DISABLE;
            break;
    }

    ret = (pVpssFunc->pfnVpssSetPortCfg)(pchan->port_id, &stVpssPortCfg);
    if (ret != HI_SUCCESS)
    {
         OmxPrint(OMX_FATAL, "%s call pfnVpssSetPortCfg failed, ret = %d\n", __func__, ret);
         goto error;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;

error:
    (pVpssFunc->pfnVpssDestroyVpss)(pchan->processor_id);
    pchan->processor_id = VPSS_INVALID_HANDLE;

    return HI_FAILURE;
}

HI_S32 processor_release_inst(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s pchan = NULL!\n", __func__);
        return HI_FAILURE;
    }

    if (VPSS_INVALID_HANDLE == pchan->port_id)
    {
        OmxPrint(OMX_FATAL, "%s port_id = VPSS_INVALID_HANDLE\n", __func__);
        return HI_FAILURE;
    }

    ret = (pVpssFunc->pfnVpssDestroyPort)(pchan->port_id);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_DestroyPort failed, ret = %d\n", __func__, ret);
    }

    pchan->port_id = VPSS_INVALID_HANDLE;

    if (VPSS_INVALID_HANDLE == pchan->processor_id)
    {
        OmxPrint(OMX_FATAL, "%s hVpss = VPSS_INVALID_HANDLE\n", __func__);
        return HI_FAILURE;
    }

    ret = (pVpssFunc->pfnVpssDestroyVpss)(pchan->processor_id);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_DestroyVpss failed, ret = %d\n", __func__, ret);
    }

    pchan->processor_id = VPSS_INVALID_HANDLE;

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 processor_start_inst(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret;
    HI_BOOL bStore = HI_FALSE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s pchan = NULL!\n", __func__);
        return HI_FAILURE;
    }

#ifdef HI_TVOS_SUPPORT
    bStore = (pchan->bVpssBypass == HI_TRUE)? HI_TRUE : HI_FALSE;
#else
    bStore = (pchan->is_overlay == HI_TRUE)? HI_TRUE : HI_FALSE;
#endif

    ret = (pVpssFunc->pfnVpssSendCommand)(pchan->processor_id, HI_DRV_VPSS_USER_COMMAND_STOREPRIV, &bStore);

    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_USER_COMMAND_STOREPRIV failed, ret = %d\n", __func__, ret);
        return HI_FAILURE;
    }

    ret = (pVpssFunc->pfnVpssEnablePort)(pchan->port_id, HI_TRUE);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_EnablePort failed, ret = %d\n", __func__, ret);
        return HI_FAILURE;
    }

    ret = (pVpssFunc->pfnVpssSendCommand)(pchan->processor_id, HI_DRV_VPSS_USER_COMMAND_START, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call start vpss failed, ret = %d\n", __func__, ret);
        return HI_FAILURE;
    }
    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 processor_stop_inst(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s pchan = NULL!\n", __func__);
        return HI_FAILURE;
    }

    ret = (pVpssFunc->pfnVpssEnablePort)(pchan->port_id, HI_FALSE);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call HI_DRV_VPSS_EnablePort failed, ret = %d\n", __func__, ret);
        return HI_FAILURE;
    }

    ret = (pVpssFunc->pfnVpssSendCommand)(pchan->processor_id, HI_DRV_VPSS_USER_COMMAND_STOP, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call stop vpss failed, ret = %d\n", __func__, ret);
        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 processor_reset_inst(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s pchan = NULL!\n", __func__);
        return HI_FAILURE;
    }

    ret = (pVpssFunc->pfnVpssSendCommand)(pchan->processor_id, HI_DRV_VPSS_USER_COMMAND_RESET, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call reset vpss failed, ret = %d\n", __func__, ret);
        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

#ifdef VFMW_VPSS_BYPASS_EN

HI_S32 processor_set_bypass(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;

    memset(&stVpssPortCfg, 0, sizeof(HI_DRV_VPSS_PORT_CFG_S));

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s pchan = NULL!\n", __func__);
        return HI_FAILURE;
    }

    OmxPrint(OMX_INFO, "%s bVpssBypass:%d\n", __func__, pchan->bVpssBypass);

    ret = (pVpssFunc->pfnVpssGetPortCfg)(pchan->port_id, &stVpssPortCfg);
    if (ret == HI_SUCCESS)
    {
        stVpssPortCfg.bPassThrough = pchan->bVpssBypass;
        ret = (pVpssFunc->pfnVpssSetPortCfg)(pchan->port_id, &stVpssPortCfg);
    }
    else
    {
         OmxPrint(OMX_FATAL, "%s call pfnVpssGetPortCfg failed, ret = %d\n", __func__, ret);
    }
    OmxPrint(OMX_TRACE, "%s exit normally!return = %x\n", __func__,ret);
    return ret;
}

HI_S32 processor_get_bypass(OMXVDEC_CHAN_CTX *pchan,PROCESSOR_BYPASSATTR_S *pBypasaAttr)
{
   //此处上报是否bypass
   HI_DRV_VPSS_BYPASSATTR_S stVpssBypassInfo;
   HI_S32 s32Ret = HI_FAILURE;

   memset(&stVpssBypassInfo, 0, sizeof(HI_DRV_VPSS_BYPASSATTR_S));

   if ((HI_NULL == pBypasaAttr) || (HI_NULL == pchan))
   {
       OmxPrint(OMX_FATAL, "%s pchan = NULL!\n", __func__);
       return HI_FAILURE;
   }

   stVpssBypassInfo.u32InputWidth  = pBypasaAttr->u32InputWidth;
   stVpssBypassInfo.u32InputHeight = pBypasaAttr->u32InputHeight;
   stVpssBypassInfo.u32InputFrameRate = pBypasaAttr->u32InputFrameRate;
   stVpssBypassInfo.enInputPixFormat  = pBypasaAttr->enInputPixFormat;

   if (pVpssFunc->pfnVpssSendCommand)
   {
      s32Ret = (pVpssFunc->pfnVpssSendCommand)(pchan->processor_id, HI_DRV_VPSS_USER_COMMAND_CHECKBYPASS, &(stVpssBypassInfo));

      if (HI_SUCCESS != s32Ret)
      {
          OmxPrint(OMX_ALWS,"%s,VPSS Check bypass failed!return 0x%x,pchan->processor_id = %d\n",__func__,s32Ret,pchan->processor_id);
          pchan->bVpssBypass = HI_FALSE;
      }
      else
      {
          pchan->bVpssBypass = stVpssBypassInfo.bVpssBypass;
      }
   }

   OmxPrint(OMX_INFO, "%s bVpssBypass:%d\n", __func__, pchan->bVpssBypass);

   return s32Ret;
}
#endif


