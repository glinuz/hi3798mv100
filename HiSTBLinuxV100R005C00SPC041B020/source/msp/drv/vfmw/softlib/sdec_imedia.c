/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpAGeVq5QmYhjfHq3op0e22GpR3m6sTBTb2tTyFV
p4pUFSdyH/adJlZwEaYad4DUbG2WdJ0iVrq62OJEuGv9QbeAp3k+H0OGGekZ3thYp9QvGJAI
vvgI/x5yRi46o9+AeOhZESay31anZBbf+yqXpQKZUpzODx4OGciV3wURGEQAxQ==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/***********************************************************************
*
* Copyright (c) 2007 HUAWEI - All Rights Reserved
*
* File     : $sh263.c$
* Date     : $2010/09/08$
* Revision : $v1.1$
* Purpose  : H.263 soft-decoder adaptor file.
*
* Change History:
*
* Date                       Author                          Change
* ====                       ======                          ======
* 2010/09/08                 z56361                          Original.
*
* Dependencies:
* Linux OS
*
************************************************************************/

#include "public.h"
#include "scd_drv.h"
#include "syn_cmn.h"
#include "sdec_imedia.h"
#include "syntax.h"
#include "mem_manage.h"
#include "vfmw_ctrl.h"
#include "imedia_util.h"
#include "vfmw_svdec_ext.h"

#include "vfmw_osal.h"

#ifndef ENV_ARMLINUX_KERNEL
#include "math.h"
#endif

SINT32 SVDEC_CapItem[][7] =
{
    /* width, height, pmv, fs(min), fs(mid), fs(max),  scdbuf */
    {176,   144,    4,    10,      11,       12,         512 * 1024},    // CAP_LEVEL_MPEG_QCIF
    {352,   288,    4,    10,      11,       12,         768 * 1024},    // CAP_LEVEL_MPEG_CIF
    {720,   576,    4,    10,      11,       12,         1024 * 1024},   // CAP_LEVEL_MPEG_D1
    {1280,  736,    4,    10,      11,       12,         2 * 1024 * 1024}, // CAP_LEVEL_MPEG_720
    {1920,  1088,   4,    10,      11,       12,         3 * 1024 * 1024}, // CAP_LEVEL_MPEG_FHD
    {176,   144,    15,   19,      22,       24,         512 * 1024},    // CAP_LEVEL_H264_QCIF
    {352,   288,    15,   19,      22,       24,         768 * 1024},    // CAP_LEVEL_H264_CIF
    {720,   576,    9,    15,      16,       17,         1024 * 1024},   // CAP_LEVEL_H264_D1
    {1280,  736,    6,    13,      13,       14,         2 * 1024 * 1024}, // CAP_LEVEL_H264_720
    {1920,  1088,   6,    13,      13,       13,         2 * 1024 * 1024}, // CAP_LEVEL_H264_FHD
    {5632,  4224,   1,    3,       3,        3,          16 * 1024 * 1024}, // CAP_LEVEL_H264_BYDHD

    {1280,   800,      6,    13,      13,       14,     2 * 1024 * 1024}, // CAP_LEVEL_1280x800
    {800,    1280,     6,    13,      13,       14,     2 * 1024 * 1024}, // CAP_LEVEL_800x1280
    {1488,   1280,     6,    13,      13,       14,     3 * 1024 * 1024}, // CAP_LEVEL_1488x1280
    {1280,   1488,     6,    13,      13,       14,     3 * 1024 * 1024}, // CAP_LEVEL_1280x1488
    {2160,   1280,     6,    13,      13,       13,     3 * 1024 * 1024}, // CAP_LEVEL_2160x1280
    {1280,   2176,     6,    13,      13,       13,     3 * 1024 * 1024}, // CAP_LEVEL_1280x2160
    {2160,   2176,     4,    11,      11,       11,     4 * 1024 * 1024}, // CAP_LEVEL_2160x2160
    {4096,   2304,     2,     9,       9,        9,     5 * 1024 * 1024}, // CAP_LEVEL_4096x2160
    {2304,   4096,     2,     9,       9,        9,     5 * 1024 * 1024}, // CAP_LEVEL_2160x4096
    {4096,   4096,     2,     9,       9,        9,     8 * 1024 * 1024}, // CAP_LEVEL_4096x4096
    {8192,   4096,     2,     9,       9,        9,    10 * 1024 * 1024}, // CAP_LEVEL_8192x4096
    {4096,   8192,     2,     9,       9,        9,    10 * 1024 * 1024}, // CAP_LEVEL_4096x8192
    {8192,   8192,     1,     3,       3,        3,    20 * 1024 * 1024}, // CAP_LEVEL_8192x8192

    {1920,  1088,   1,    3,       3,        3,          2 * 1024 * 1024}, // CAP_LEVEL_SINGLE_IFRAME_FHD
    {1920,  1088,   1,    3,       3,        3,          2 * 1024 * 1024}, // CAP_LEVEL_USER_DEFINE_WITH_OPTION

    {1920,  1088,   17,    26,      26,       26,         2 * 1024 * 1024}, // CAP_LEVEL_MVC_FHD

    /* 因为HEVC协议的解码宏块目前逻辑支持到64x64，因此需要把最大支持的宽高进行64对齐，
       否则根据能力级获取的解码内存不足以给逻辑解码。HEVC协议解码宏块最大支持到128x128，
       若逻辑支持到128x128，需要再把宽高1228对齐。c00277632  20141225*/
    {192,   192,    15,   19,      22,       24,         512 * 1024},    // CAP_LEVEL_HEVC_QCIF
    {384,   320,    15,   19,      22,       24,         768 * 1024},    // CAP_LEVEL_HEVC_CIF
    {768,   576,    9,    15,      16,       17,         1024 * 1024},   // CAP_LEVEL_HEVC_D1
    {1280,  768,    6,    13,      13,       14,         2 * 1024 * 1024}, // CAP_LEVEL_HEVC_720
    {1920,  1088,   6,    13,      13,       13,         2 * 1024 * 1024}, // CAP_LEVEL_HEVC_FHD
    {4096,	2304,	4,     4,       4,        4,		 16 * 1024 * 1024},	 // CAP_LEVEL_HEVC_UHD
    /*
    {176,   144,    15,   19,      22,       24,         512*1024},      // CAP_LEVEL_HEVC_QCIF
    {352,   288,    15,   19,      22,       24,         768*1024},      // CAP_LEVEL_HEVC_CIF
    {720,   576,    9,    15,      16,       17,         1024*1024},     // CAP_LEVEL_HEVC_D1
    {1280,  736,    6,    13,      13,       14,         2*1024*1024},   // CAP_LEVEL_HEVC_720
    {1920,  1088,   6,    13,      13,       13,         2*1024*1024},   // CAP_LEVEL_HEVC_FHD
    {4096,	2304,	4,     4,       4,        4,		 16*1024*1024},	 // CAP_LEVEL_HEVC_UHD
    */
};

#define CALL_IMEDIA_LIB

extern VFMW_SVDEC_EXPORT_FUNC_S *pVfmwToSvdecFun;

VOID  ConvFormat( UINT32 UVWidth, UINT32 UVHeight, UINT32 UVStride, UINT8 *pY, UINT8 *pU, UINT8 *pV, UINT8 *pLuma, UINT8 *pChrom, UINT32 bReversed);
VOID  SaveYUV( UINT32 Width, UINT32 Height, UINT8 *py, UINT8 *pu, UINT8 *pv, UINT32 stride);
#ifdef MODULE
VOID SetAspectRatio(IMAGE *pImg, VDEC_DAR_E Code);
#endif
void  ImediaMsgOutput(int level, const char *msg)
{
    return;
}

/******************************************************************************
    创建实例
 ******************************************************************************/
SINT32 iMediaSoftDEC_Create(iMediaSDEC_CTX_S  *pCtx, MEM_DESC_S *pstInstMem, VID_STD_E eVidStd)
{
    SINT32 s32Ret, s32MemNeedSize;
    //extern SINT32 CapItem[][7];
    ENUM_IVIDEO_CODEC_TYPE iMediaCodecType;
    STRU_IVIDDEC_MEMORY_INFO stMem = {0};
    STRU_IVIDDEC_PARAMS stParams = {0};
#ifdef ENV_ARMLINUX_KERNEL
    STRU_IMEDIA_SYSINFO stMediaSysInfo;

    stMediaSysInfo.pfnMalloc = (funMalloc *)kmalloc;
    stMediaSysInfo.pfnFree   = (funFree *)kfree;
    stMediaSysInfo.pfnPrintf = ImediaMsgOutput;
    s32Ret = IMedia_SetSysInfo(&stMediaSysInfo);
#endif
    pCtx->eVidStd = eVidStd;

    switch (eVidStd)
    {
        case STD_H263:
            iMediaCodecType = IVIDEO_CODEC_H263;
            break;

        case STD_SORENSON:
            iMediaCodecType = IVIDEO_CODEC_SORENSON;
            break;

        case STD_VP6:
            iMediaCodecType = IVIDEO_CODEC_VP6;
            break;

        case STD_VP6F:
            iMediaCodecType = IVIDEO_CODEC_VP6F;
            break;

        case STD_VP6A:
            iMediaCodecType = IVIDEO_CODEC_VP6A;
            break;

        default:
            return iMediaSDEC_ERR;
    }

    stMem.eColorFormat = IVIDEO_CSP_YUV420;

    if ( (STD_VP6 == pCtx->eVidStd) || (STD_VP6F == pCtx->eVidStd) || (STD_VP6A == pCtx->eVidStd))
    {
        stMem.usBufNum     = 6;
        stMem.usWidth      = 1280;
        stMem.usHeight     = 768;
    }
    else
    {
        //stMem.usBufNum     = 10;
        //stMem.usWidth      = 1920;
        //stMem.usHeight     = 1088;

        stMem.usBufNum     = 5;

        if ((pCtx->eChanCapLevel >= CAP_LEVEL_MPEG_QCIF && pCtx->eChanCapLevel <= CAP_LEVEL_MPEG_FHD)
            || (pCtx->eChanCapLevel >= CAP_LEVEL_H264_QCIF && pCtx->eChanCapLevel <= CAP_LEVEL_H264_FHD))
        {
            stMem.usWidth      = SVDEC_CapItem[pCtx->eChanCapLevel][0];
            stMem.usHeight     = SVDEC_CapItem[pCtx->eChanCapLevel][1];
        }
        else
        {
            stMem.usWidth      = 1920;
            stMem.usHeight     = 1088;
        }
    }
	stMem.usWidth	   = 1920;
	stMem.usHeight	   = 1088;

    if (0 != (s32Ret = IMedia_Viddec_QueryMemSize(iMediaCodecType, &stMem, &s32MemNeedSize)))
    {
        //dprint(PRN_ALWS, "ERROR: IMedia_Viddec_QueryMemSize return %d\n", s32Ret);
        return iMediaSDEC_ERR;
    }

    if (s32MemNeedSize > pstInstMem->Length)
    {
        return iMediaSDEC_ERR;
    }

    pCtx->InstMemPhyAddr    = pstInstMem->PhyAddr;
    pCtx->InstMemLength     = pstInstMem->Length;
#if 0 //#ifdef ENV_ARMLINUX_KERNEL
    klib_munmap(pstInstMem->VirAddr);
    pCtx->pu8InstMemVirAddr = klib_mmap_cached(pstInstMem->PhyAddr, pstInstMem->Length);
    dprint(PRN_ALWS, "###################### 0x%x remap: %p -> %p ########################\n", pstInstMem->PhyAddr, pstInstMem->VirAddr, pCtx->pu8InstMemVirAddr);
    dprint(PRN_ALWS, "MemSize: iMedia need %d, Chan have %d\n", s32MemNeedSize, pstInstMem->Length);
#else
    pCtx->pu8InstMemVirAddr = (UINT8*)(ULONG)pstInstMem->VirAddr;
#endif

    if (pCtx->pu8InstMemVirAddr == NULL)
    {
        return iMediaSDEC_ERR;
    }

    stParams.usMaxWidth          = stMem.usWidth;
    stParams.usMaxHeight         = stMem.usHeight;
    stParams.usMaxRefFrame       = 2;
    stParams.eProfile            = 0;
    stParams.eLevel              = 0;
    stParams.usMaxFrameBufNum    = stMem.usBufNum;
    stParams.iFlags              = 0;
    stParams.bForceOutYUV420Flag = 1;
    stParams.pucBuf              = pCtx->pu8InstMemVirAddr;
    stParams.iBufLength          = pCtx->InstMemLength;

    s32Ret = IMedia_Viddec_Create(iMediaCodecType, &stParams, &pCtx->CodecInstHandle);

    return (s32Ret == 0) ? iMediaSDEC_OK : iMediaSDEC_ERR;
}

/******************************************************************************
    销毁实例
 ******************************************************************************/
SINT32 iMediaSoftDEC_Destroy(iMediaSDEC_CTX_S  *pCtx)
{

    IMedia_Viddec_Delete(pCtx->CodecInstHandle);

#ifdef ENV_ARMLINUX_KERNEL
    //klib_munmap(pCtx->pu8InstMemVirAddr);
#endif

    return iMediaSDEC_OK;
}


/******************************************************************************
    初始化，即复位
 ******************************************************************************/
SINT32 iMediaSoftDEC_Init(iMediaSDEC_CTX_S  *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData)
{
    SINT32 ImgQueRstMagic;
    IMEDIA_CODEC_CTX Handle;
    VID_STD_E eVidStd;

#ifdef CALL_IMEDIA_LIB
    //初始化
    Handle = pCtx->CodecInstHandle;
    eVidStd = pCtx->eVidStd;

    ImgQueRstMagic = pCtx->ImageQue.ResetMagicWord;
    memset(pCtx, 0, sizeof(iMediaSDEC_CTX_S));
    pCtx->ImageQue.ResetMagicWord = ImgQueRstMagic;
    //    ResetVoQueue(&pCtx->ImageQue);
    (pVfmwToSvdecFun->pfnVfmwResetVoQueue)(&pCtx->ImageQue);

    pCtx->pstExtraData = pstExtraData;
    pCtx->ChanID = (pVfmwToSvdecFun->pfnVfmwVctrlGetChanIDByCtx)(pCtx);
    //    pCtx->ChanID = VCTRL_GetChanIDByCtx(pCtx);

    pCtx->CodecInstHandle = Handle;
    pCtx->eVidStd = eVidStd;
    IMedia_Viddec_Control(pCtx->CodecInstHandle, IMEDIA_RESET, NULL, NULL);
#endif

    return iMediaSDEC_OK;
}


#define UP_ALIGN_16(dat) (((dat) + 15) & (~15))
/******************************************************************************
    解码一帧
 ******************************************************************************/
SINT32 iMediaSoftDEC_Decode(iMediaSDEC_CTX_S  *pCtx, DEC_STREAM_PACKET_S *pPacket)
{
    UINT32  i;
    SINT32 s32Ret;
    STRU_IVIDDEC_STREAM stStream;
    STRU_IVIDDEC_OUT_ARGS stDecOutArgs;
    STRU_IVIDEO_PICTURE *pstPic;
    IMAGE *pstImg;
    UINT8 *pu8Luma;
    UINT8 *pu8Chrom;
    UINT32 u32AspectRatio = 0;   // aspect_ratio
    UINT32 u32FldFirst    = 0;   // 场序: 0=top first, 1=bottom first
    UINT32 u32SrcFmt      = 0;   // 逐行/隔行: 0=progressive, 1=interlaced
    UINT32 u32VideoFmt    = 1;   // 制式: 1=pal, 2=ntsc
    UINT32 u32CodType     = 0;   // 帧类型: 0=I, 1=P, 2=B
    /*UINT32 time1, time2, time3;
    UINT32 hit0,hit1, req0, req1;;	*/

    static UINT32 last_image_id = -1;

    if (1 == pPacket->StreamPack[0].IsStreamEnd)
    {
        if (last_image_id >= 0 && last_image_id < iMediaSDEC_MAX_IMAGE_NUM)
        {
            (pVfmwToSvdecFun->pfnVfmwLastFrameNotify)(pCtx->ChanID, last_image_id);
            last_image_id = -1;
        }
        else
        {
            (pVfmwToSvdecFun->pfnVfmwLastFrameNotify)(pCtx->ChanID, -1);
        }

        return iMediaSDEC_OK;
    }

    /*	y00226912  双核情况下，不断报警，暂时去掉不影响解码（修正？）
    	if (p1 == 0)
    	{
    	    MEM_RECORD_S MemRec;
    	    (pVfmwToSvdecFun->pfnVfmwMemMapRegisterAddr)(0x16800600, 0x400, &MemRec);  // 查看L2 cache命中率
    //	    MEM_MapRegisterAddr(0x16800600, 0x400, &MemRec);  // 查看L2 cache命中率
    		p1 = (UINT32*)MemRec.VirAddr;
    	}
    */

    /* 解码输入参数 */
    stStream.pucBuf = pPacket->StreamPack[0].VirAddr;
    stStream.uiNumBytes = pPacket->StreamPack[0].LenInByte;

    if (0 == stStream.uiNumBytes)
    {
        return iMediaSDEC_ERR;
    }

    stStream.iPTS = (UINT32)pPacket->StreamPack[0].Pts;

    /* 调用iMedia库解码一帧码流 */
    memset(&stDecOutArgs, 0, sizeof(stDecOutArgs));
    s32Ret = IMedia_Viddec_FrameDecode(pCtx->CodecInstHandle, &stStream, &stDecOutArgs);

    /* 如果iMedia有图像输出，则将此图像读出，转换格式后写入到显示图像空间去，等待VO读取 */
    if (stDecOutArgs.stPicture.apucBuf[0] && stDecOutArgs.stPicture.apucBuf[1] && stDecOutArgs.stPicture.apucBuf[2] && 0 == s32Ret)
    {
        UINT32 bReversed = 0;

        /* 转换色色度格式, 使UV间插存放 */
        pstPic = &stDecOutArgs.stPicture;
        pu8Chrom = pstPic->apucBuf[0] + pstPic->usWidthPitch * (UP_ALIGN_16(pstPic->usHeight) + 16);

        if ( (STD_VP6 == pCtx->eVidStd) || (STD_VP6F == pCtx->eVidStd) || (STD_VP6A == pCtx->eVidStd) )
        {
            bReversed = pCtx->pstExtraData->StdExt.Vp6Ext.bReversed;
        }
        else
        {
            bReversed = 0;
        }

        if (1 == bReversed)
        {
            pu8Luma = pu8Chrom + pstPic->usWidthPitch * (UP_ALIGN_16(pstPic->usHeight) / 2);
        }
        else
        {
            pu8Luma = pstPic->apucBuf[0];
        }

        ConvFormat(pstPic->usWidth / 2, pstPic->usHeight / 2, pstPic->usWidthPitch / 2,
                   pstPic->apucBuf[0], pstPic->apucBuf[1], pstPic->apucBuf[2], pu8Luma, pu8Chrom, bReversed);
#ifdef ENV_ARMLINUX_KERNEL
        (pVfmwToSvdecFun->pfnVfmwKlibFlushCache)((void *)pu8Luma, (pVfmwToSvdecFun->pfnVfmwMemVir2Phy)((signed char *)pu8Luma), pstPic->usWidthPitch * pstPic->usHeight);
        (pVfmwToSvdecFun->pfnVfmwKlibFlushCache)((void *)pu8Chrom, (pVfmwToSvdecFun->pfnVfmwMemVir2Phy)((signed char *)pu8Chrom), pstPic->usWidthPitch * pstPic->usHeight / 2);
        //		klib_flush_cache(pu8Luma, MEM_Vir2Phy(pu8Luma), pstPic->usWidthPitch * pstPic->usHeight);
        //        klib_flush_cache(pu8Chrom, MEM_Vir2Phy(pu8Chrom), pstPic->usWidthPitch * pstPic->usHeight / 2);
#endif

        /* 寻找一个空闲的IMAGE槽位存放刚刚解码的帧信息 */
        for (i = 0; i < iMediaSDEC_MAX_IMAGE_NUM; i++)
        {
            if (0 == pCtx->ImageUsedFlag[i])
            {
                break;
            }
        }

        /* 如果槽位占满: 将所有帧释放给解码库，然后清空IMAGE集 */
        if (i >= iMediaSDEC_MAX_IMAGE_NUM)
        {
            for (i = 0; i < iMediaSDEC_MAX_IMAGE_NUM; i++)
            {
                iMediaSoftDEC_RecycleImage(pCtx, i);
            }

            i = 0;
        }

        pstImg   = &pCtx->stImgs[i];
        pstImg->image_id = i;

        pstImg->PTS = pCtx->pstExtraData->pts;
        pCtx->pstExtraData->pts = -1;

        pstImg->top_luma_phy_addr  = bReversed ?
                                     iMediaSDEC_VIR_2_PHY((signed char *)pu8Luma) :
                                     iMediaSDEC_VIR_2_PHY((signed char *)pstPic->apucBuf[0]);
        pstImg->top_chrom_phy_addr = iMediaSDEC_VIR_2_PHY((signed char *)pu8Chrom);
        pstImg->btm_luma_phy_addr  = pstImg->top_luma_phy_addr + pstPic->usWidthPitch;
        pstImg->btm_chrom_phy_addr = pstImg->top_chrom_phy_addr + pstPic->usWidthPitch;

        pstImg->luma_2d_phy_addr   = pstImg->top_luma_phy_addr;
#ifdef VFMW_4_SUPPORT
		pstImg->luma_2d_vir_addr   = (UINT8*)(ULONG)pstPic->apucBuf[0];
		pstImg->luma_vir_addr	= (UINT8*)(ULONG)pstPic->apucBuf[0];
		pstImg->chrom_2d_vir_addr  = (UINT8*)(ULONG)pu8Chrom;
		pstImg->chrom_vir_addr	= (UINT8*)(ULONG)pu8Chrom;
#else
        pstImg->luma_2d_vir_addr   = (UINT64)(ULONG)pstPic->apucBuf[0];
        pstImg->luma_vir_addr   = (UINT64)(ULONG)pstPic->apucBuf[0];
		pstImg->chrom_2d_vir_addr  = (UINT64)(ULONG)pu8Chrom;
		pstImg->chrom_vir_addr	= (UINT64)(ULONG)pu8Chrom;
#endif
        
        pstImg->luma_phy_addr   = pstImg->top_luma_phy_addr; 
        pstImg->chrom_2d_phy_addr  = pstImg->top_chrom_phy_addr;
        pstImg->chrom_phy_addr  = pstImg->top_chrom_phy_addr;
        
        /* 设置宽高比的两个参数 */
        SetAspectRatio(pstImg, (VDEC_DAR_E) u32AspectRatio);

        pstImg->format = ((u32AspectRatio & 7) << 14)
                         | ((u32FldFirst & 0x3) << 12)
                         | (3 << 10)
                         | ((u32SrcFmt & 3) << 8)
                         | ((u32VideoFmt & 3) << 5)
                         | ((0 & 7) << 2) // yuv420
                         | (u32CodType & 3);

        pstImg->image_width   = pstPic->usWidth;
        pstImg->image_height  = pstPic->usHeight;
        pstImg->disp_width    = pstPic->usWidth;
        pstImg->disp_height   = pstPic->usHeight;
        pstImg->disp_center_x = pstPic->usWidth / 2;
        pstImg->disp_center_y = pstPic->usHeight / 2;
        pstImg->image_stride  = pstPic->usWidthPitch;

        pstImg->frame_rate    = 25 << 10;
        pstImg->error_level = 0;
        pstImg->ImageDnr.video_standard = pCtx->eVidStd;

        /* 将此图像插入输出队列 */
        pCtx->ImageUsedFlag[i] = 1;
        memcpy(&pCtx->stVidPics[i], pstPic, sizeof(STRU_IVIDEO_PICTURE));

        //        if( VF_OK != InsertImgToVoQueue(pCtx->ChanID, pCtx->eVidStd, pCtx, &pCtx->ImageQue, pstImg))
        if (VF_OK != (pVfmwToSvdecFun->pfnVfmwInsertImgToVoQueue)(pCtx->ChanID, pCtx->eVidStd, pCtx, &pCtx->ImageQue, pstImg))
        {
            IMedia_Viddec_Control(pCtx->CodecInstHandle, IMEDIA_PICTURE_RELEASE, pstPic, NULL);
            pCtx->ImageUsedFlag[i] = 0;
        }
        else
        {
            last_image_id = pstImg->image_id;
        }

    }

    if (stDecOutArgs.uiBytesConsumed == 0)
    {
        return iMediaSDEC_NOT_DEC;
    }

    return iMediaSDEC_OK;
}

/******************************************************************************
    回收图像
 ******************************************************************************/
SINT32 iMediaSoftDEC_RecycleImage(iMediaSDEC_CTX_S  *pCtx, UINT32 img_id)
{

    if (1 == pCtx->ImageUsedFlag[img_id])
    {
        IMedia_Viddec_Control(pCtx->CodecInstHandle, IMEDIA_PICTURE_RELEASE, &pCtx->stVidPics[img_id], NULL);
        pCtx->ImageUsedFlag[img_id] = 0;
        return iMediaSDEC_OK;
    }

    return iMediaSDEC_ERR;
}


/******************************************************************************
    如下需要实现否?
 ******************************************************************************/
SINT32 iMediaSoftDEC_GetRemainImg(iMediaSDEC_CTX_S  *pCtx)
{
    return iMediaSDEC_OK;
}

SINT32 iMediaSoftDEC_GetImageBuffer(iMediaSDEC_CTX_S  *pCtx)
{
    return iMediaSDEC_OK;
}


/******************************************************************************
    转换格式: 把planar转换为semi-planar.
              具体地，把pu/pv所指向的两片独立色度空间合并起来，存放到pChrom所
              指的空间中去. 合并的方法是间插，即每行对应的u,v间插，形成vuvu....
 ******************************************************************************/
VOID  ConvFormat( UINT32 UVWidth, UINT32 UVHeight, UINT32 UVStride, UINT8 *pY, UINT8 *pU, UINT8 *pV, UINT8 *pLuma, UINT8 *pChrom, UINT32 bReversed)
{
    SINT32 i, j;
    UINT32 InterlacedChromStride;
    UINT8  *pu8;

    InterlacedChromStride = UVStride * 2;

    if (0 == bReversed)
    {
        for (i = 0; i < UVHeight; i++)
        {
            pu8 = pChrom + (i * InterlacedChromStride);

            for (j = 0; j < UVWidth; j++)
            {
                *pu8++ = pV[j];
                *pu8++ = pU[j];
            }

            pU += UVStride;
            pV += UVStride;
        }
    }
    else
    {
        for (i = 0; i < UVHeight; i++)
        {
            pu8 = pChrom + ((UVHeight - 1 - i) * InterlacedChromStride);

            for (j = 0; j < UVWidth; j++)
            {
                *pu8++ = pV[j];
                *pu8++ = pU[j];
            }

            pU += UVStride;
            pV += UVStride;
        }

        for (i = 0; i < UVHeight * 2; i++)
        {
            pu8 = pLuma + ( (UVHeight * 2 - 1 - i) * InterlacedChromStride);

            memmove(pu8, pY, UVWidth * 2);

            pY += UVStride * 2;
        }

    }

    return;
}

VOID  SaveYUV( UINT32 Width, UINT32 Height, UINT8 *py, UINT8 *pu, UINT8 *pv, UINT32 stride)
{
#ifndef ENV_ARMLINUX_KERNEL
    SINT32 i;
    static FILE *fp = NULL;

    if (fp == NULL)
    {
        fp = fopen("sdec_save_yuv.yuv", "wb");
    }

    if (fp != NULL)
    {
        for (i = 0; i < Height; i++)
        {
            fwrite(py, 1, Width, fp);
            py += stride;
        }

        for (i = 0; i < Height / 2; i++)
        {
            fwrite(pu, 1, Width / 2, fp);
            pu += (stride / 2);
        }

        for (i = 0; i < Height / 2; i++)
        {
            fwrite(pv, 1, Width / 2, fp);
            pv += (stride / 2);
        }

        fflush(fp);
    }

#endif

}

#ifdef MODULE
VOID SetAspectRatio(IMAGE *pImg, VDEC_DAR_E Code)
{
    switch (Code)
    {
        case DAR_UNKNOWN:
            pImg->u32AspectWidth = 0;
            pImg->u32AspectHeight = 1;
            break;

        case DAR_4_3:
            pImg->u32AspectWidth = 4;
            pImg->u32AspectHeight = 3;
            break;

        case DAR_16_9:
            pImg->u32AspectWidth = 16;
            pImg->u32AspectHeight = 9;
            break;

        case DAR_221_100:
            pImg->u32AspectWidth = 221;
            pImg->u32AspectHeight = 100;
            break;

        case DAR_235_100:
            pImg->u32AspectWidth = 235;
            pImg->u32AspectHeight = 100;
            break;

        case DAR_IMG_SIZE:
            pImg->u32AspectWidth = 1;
            pImg->u32AspectHeight = 1;
            break;

        default:
            pImg->u32AspectWidth = 0;
            pImg->u32AspectHeight = 1;
            break;
    }
}
#endif


