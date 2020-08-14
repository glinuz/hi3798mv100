/******************************************************************************
  Copyright (C), 2001-2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hdr_transfer.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2016/06/15
Last Modified :
Description   : this file use to adapt hal's structures.
Function List :
History       :
******************************************************************************/

#include "hdr_transfer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

#ifndef __DISP_PLATFORM_BOOT__

HI_VOID TransferToHalHdrMode(DRV_DISP_HISI_HDR_CFG_MODE_E enDrvHdrMode,HIHDR_SCENE_MODE_E *penHalHdrMode)
{
    switch(enDrvHdrMode)
    {
        case DRV_HISI_HDR10_IN_HDR10_OUT:
            *penHalHdrMode = HIHDR_HDR10_IN_HDR10_OUT;
            break;

        case DRV_HISI_HDR10_IN_SDR_OUT:
            *penHalHdrMode = HIHDR_HDR10_IN_SDR_OUT;
            break;

        case DRV_HISI_HDR10_IN_HLG_OUT:
            *penHalHdrMode = HIHDR_HDR10_IN_HLG_OUT;
            break;

        case DRV_HISI_HLG_IN_HDR10_OUT:
            *penHalHdrMode = HIHDR_HLG_IN_HDR10_OUT;
            break;

        case DRV_HISI_HLG_IN_SDR_OUT:
            *penHalHdrMode = HIHDR_HLG_IN_SDR_OUT;
            break;

        case DRV_HISI_HLG_IN_HLG_OUT:
            *penHalHdrMode = HIHDR_HLG_IN_HLG_OUT;
            break;

        case DRV_HISI_SLF_IN_HDR10_OUT:
            *penHalHdrMode = HIHDR_AVS_IN_HDR10_OUT;
            break;

        case DRV_HISI_SLF_IN_SDR_OUT:
            *penHalHdrMode = HIHDR_AVS_IN_SDR_OUT;
            break;

        case DRV_HISI_TECHNI_IN_HDR10_OUT:
            *penHalHdrMode = HIHDR_TECHNI_IN_HDR10_OUT;
            break;

        case DRV_HISI_TECHNI_IN_SDR_OUT:
            *penHalHdrMode = HIHDR_TECHNI_IN_SDR_OUT;
            break;

        default:
            break;

    }
    return;

}

HI_VOID TransferToHalReso(HI_RECT_S *pstDrvReso,VDP_RECT_S *pstCbbReso)
{
    pstCbbReso->u32X    = pstDrvReso->s32X;
    pstCbbReso->u32Y    = pstDrvReso->s32Y;
    pstCbbReso->u32Wth  = pstDrvReso->s32Width;
    pstCbbReso->u32Hgt  = pstDrvReso->s32Height;
    return ;
}

HI_VOID TransferToHalAddr(HI_DRV_DISP_HDR_FRAME_INFO_S *pstFrmInfo,
                                VDP_ADDR_S                *pstCbbAddr,
                                HI_BOOL                    bDcmp)
{
    /* put the 8bit addr in stAddr[VDP_ADDR_DATA] */
    pstCbbAddr[VDP_ADDR_DATA].u32LumAddr = pstFrmInfo->stAddress[0].u32PhyAddr_Y;
    pstCbbAddr[VDP_ADDR_DATA].u32LumStr  = pstFrmInfo->stAddress[0].u32Stride_Y;
    pstCbbAddr[VDP_ADDR_DATA].u32ChmAddr = pstFrmInfo->stAddress[0].u32PhyAddr_C;
    pstCbbAddr[VDP_ADDR_DATA].u32ChmStr  = pstFrmInfo->stAddress[0].u32Stride_C;

    if (bDcmp)
    {
        pstCbbAddr[VDP_ADDR_HEAD].u32LumAddr = pstFrmInfo->stAddress[0].u32PhyAddr_YHead;
        pstCbbAddr[VDP_ADDR_HEAD].u32ChmAddr = pstFrmInfo->stAddress[0].u32PhyAddr_CHead;
        pstCbbAddr[VDP_ADDR_HEAD].u32LumStr  = pstFrmInfo->stAddress[0].u32Head_Stride;
        pstCbbAddr[VDP_ADDR_HEAD].u32DcmpHeadSize = pstFrmInfo->stAddress[0].u32Head_Size;
    }

    pstCbbAddr[VDP_ADDR_TILE].u32LumAddr = pstFrmInfo->stAddress_lb[0].u32PhyAddr_Y;
    pstCbbAddr[VDP_ADDR_TILE].u32LumStr  = pstFrmInfo->stAddress_lb[0].u32Stride_Y;
    pstCbbAddr[VDP_ADDR_TILE].u32ChmAddr = pstFrmInfo->stAddress_lb[0].u32PhyAddr_C;
    pstCbbAddr[VDP_ADDR_TILE].u32ChmStr  = pstFrmInfo->stAddress_lb[0].u32Stride_C;


    return ;
}

HI_VOID TransferToDolbyHalAddr(DRV_DISP_HDR_FRAME_ADDR_S *pstDrvAddr,
                                VDP_ADDR_S                *pstCbbAddr,
                                HI_BOOL                    bHeaderAddr,
                                HI_BOOL                    bSecure,
                                HI_BOOL                    bSmmu)
{
    if (HI_FALSE == bHeaderAddr)
    {
        pstCbbAddr->u32LumAddr = pstDrvAddr->u32PhyAddr_Y;
        pstCbbAddr->u32LumStr  = pstDrvAddr->u32Stride_Y;
        pstCbbAddr->u32ChmAddr = pstDrvAddr->u32PhyAddr_C;
        pstCbbAddr->u32ChmStr  = pstDrvAddr->u32Stride_C;
    }
    else
    {
        pstCbbAddr->u32LumAddr = pstDrvAddr->u32PhyAddr_YHead;
        pstCbbAddr->u32ChmAddr = pstDrvAddr->u32PhyAddr_CHead;
        pstCbbAddr->u32LumStr = pstDrvAddr->u32Head_Stride;
        pstCbbAddr->u32DcmpHeadSize = pstDrvAddr->u32Head_Size;

    }
    pstCbbAddr->bSecure        = bSecure;
    pstCbbAddr->bSmmu          = bSmmu;

    return ;
}


HI_VOID TransferToHalPixelFmt(HI_DRV_PIX_FORMAT_E enInputPixelFmt,VDP_VID_IFMT_E *peOutPixelFmt)
{

    switch(enInputPixelFmt)
    {

        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_CMP:
            *peOutPixelFmt = VDP_VID_IFMT_SP_420;
            break;
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1:
            *peOutPixelFmt = VDP_VID_IFMT_SP_422;
            break;
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            *peOutPixelFmt = VDP_VID_IFMT_SP_TILE_64;
            break;
        case HI_DRV_PIX_FMT_UYVY:
            *peOutPixelFmt = VDP_VID_IFMT_PKG_UYVY;
            break;
        case HI_DRV_PIX_FMT_YUYV:
            *peOutPixelFmt = VDP_VID_IFMT_PKG_YUYV;
            break;
        case HI_DRV_PIX_FMT_YVYU:
            *peOutPixelFmt = VDP_VID_IFMT_PKG_YVYU;
            break;
        default:
            //error
            break;
    }
}

HI_VOID TransferToHalFrameInfo(HI_DRV_DISP_HDR_FRAME_INFO_S *pstFrmInfo, VDP_SRC_INFO_S *pstHalFrmInfo)
{
    pstHalFrmInfo->bDcmpEn = pstFrmInfo->bDcmpEn;
    pstHalFrmInfo->bDcmpLost = HI_TRUE;
    pstHalFrmInfo->bSecureEn = pstFrmInfo->bSecurity;
    switch(pstFrmInfo->enBitWidth)
    {
        case HI_DRV_PIXEL_BITWIDTH_8BIT:
            pstHalFrmInfo->enDataWidth = VDP_DATA_WTH_8;
            break;
        case HI_DRV_PIXEL_BITWIDTH_10BIT:
            pstHalFrmInfo->enDataWidth = VDP_DATA_WTH_10;
            break;
        case HI_DRV_PIXEL_BITWIDTH_10BIT_CTS:
            pstHalFrmInfo->enDataWidth = VDP_DATA_WTH_10_CTS;
            break;
        default:
            break;
    }

    TransferToHalPixelFmt(pstFrmInfo->enPixFormat, &pstHalFrmInfo->enInFmt);
    TransferToHalAddr(pstFrmInfo, &pstHalFrmInfo->stAddr[0], pstHalFrmInfo->bDcmpEn);

    return;
}

HI_VOID HAL_TransferToHiHalHdrCfg(HI_DRV_DISP_HISI_HDR_CFG_S *pstDrvPara,VDP_HISI_HDR_CFG_S *pstHalHdrCfg)
{
    //printk("----%s %d-Mode %d---\n",__func__,__LINE__,pstTopPara->enHdrMode);
    TransferToHalHdrMode(pstDrvPara->enHdrMode, &pstHalHdrCfg->eHiHdrMode);
    if(pstHalHdrCfg->eHiHdrMode == HIHDR_HDR10_IN_SDR_OUT
    || pstHalHdrCfg->eHiHdrMode == HIHDR_HLG_IN_SDR_OUT
    || pstHalHdrCfg->eHiHdrMode == HIHDR_AVS_IN_SDR_OUT
    || pstHalHdrCfg->eHiHdrMode == HIHDR_TECHNI_IN_SDR_OUT
    )
    {
        pstHalHdrCfg->enHiHdrGfxMode = GDM_BYPASS_MODE;
    }
    else
    {
        pstHalHdrCfg->enHiHdrGfxMode = GDM_SDR_IN_HDR10_OUT;
    }
    TransferToHalFrameInfo(&pstDrvPara->stFrame, &pstHalHdrCfg->stSrcInfo);
    pstHalHdrCfg->stSrcInfo.bSmmuEn = pstDrvPara->bSmmu;
    TransferToHalReso(&pstDrvPara->stResoIn, &pstHalHdrCfg->stSrcInfo.stSrcRect);
    TransferToHalReso(&pstDrvPara->stResoIn, &pstHalHdrCfg->stCropRect);
    TransferToHalReso(&pstDrvPara->stResoOut, &pstHalHdrCfg->stVideoRect);
    TransferToHalReso(&pstDrvPara->stResoOut, &pstHalHdrCfg->stDispRect);

    pstHalHdrCfg->stHdrVdpCfgExt.bFlipEn = HI_FALSE;
    pstHalHdrCfg->stHdrVdpCfgExt.bHzmeEn = HI_FALSE;
    pstHalHdrCfg->stHdrVdpCfgExt.bRegionEn = HI_TRUE;
    pstHalHdrCfg->stHdrVdpCfgExt.enDispMode = VDP_DISP_MODE_2D;
    pstHalHdrCfg->stHdrVdpCfgExt.enDrawMode = VDP_DRAW_MODE_1;
    pstHalHdrCfg->stHdrVdpCfgExt.enDrawPixelMode = VDP_DRAW_PIXEL_MODE_1;
    pstHalHdrCfg->stHdrVdpCfgExt.enHzmeMode = VDP_HZME_MODE_COPY;
    pstHalHdrCfg->stHdrVdpCfgExt.enReadMode = VDP_RMODE_PROGRESSIVE;
    TransferToHalReso(&pstDrvPara->stResoOut, &pstHalHdrCfg->stHdrVdpCfgExt.stZme2InRect);
    pstHalHdrCfg->stHdrVdpCfgExt.u32LayerZorder = 1;
    pstHalHdrCfg->stHdrVdpCfgExt.bHzmeEn = HI_FALSE;

    pstHalHdrCfg->bMuteEn = pstDrvPara->bMuteEn;

    pstHalHdrCfg->stVdpDispInfo.stFmtResolution.u32X = pstDrvPara->stDispInfo.stFmtResolution.s32X;
    pstHalHdrCfg->stVdpDispInfo.stFmtResolution.u32Y = pstDrvPara->stDispInfo.stFmtResolution.s32Y;
    pstHalHdrCfg->stVdpDispInfo.stFmtResolution.u32Wth = pstDrvPara->stDispInfo.stFmtResolution.s32Width;
    pstHalHdrCfg->stVdpDispInfo.stFmtResolution.u32Hgt = pstDrvPara->stDispInfo.stFmtResolution.s32Height;
    return;
}


HI_VOID HAL_TransferToDolbyHalHdrCfg(HI_DRV_DISP_DOLBY_HDR_CFG_S * pstDrvPara, VDP_HDR_CFG_S * pstHalHdrCfg)
{

    pstHalHdrCfg->bGfxEn  = pstDrvPara->bGfxEn;
    pstHalHdrCfg->bMuteEn = pstDrvPara->bMuteEn;
    pstHalHdrCfg->bSmmuEn = pstDrvPara->bSmmu;
    pstHalHdrCfg->bPreMult = HI_FALSE;
    //pstHalHdrCfg->bFlag480I576I = pstDrvPara->bSdFmt;
    //pstHalHdrCfg->bSecureEn = pstDrvPara->bSecureEn;

    switch (pstDrvPara->enHdrMode)
    {
        case DRV_DOVI_BC_IN_SDR_OUT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_BC_IN_SDR_OUT;
            break;
        }
        case DRV_DOVI_BC_IN_SDR_OUT_CERT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_BC_IN_SDR_OUT_CERT;
            break;
        }
        case DRV_DOVI_BC_IN_HDR10_OUT:
        {
            /* !!!!NOTECE!!!HAL NEED TO ADD NEW BRANCH TO OUTPUT 444 */
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_BC_IN_HDR10_OUT;
            break;
        }
        case DRV_DOVI_BC_IN_HDR10_OUT_CERT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_BC_IN_HDR10_OUT_CERT;
            break;
        }
        case DRV_DOVI_BC_IN_DOVI_YUV_OUT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_BC_IN_DOVI_YUV_OUT;
            break;
        }
        case DRV_DOVI_BC_IN_DOVI_IPT_OUT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_BC_IN_DOVI_IPT_OUT;
            break;
        }
        case DRV_DOVI_NBC_IN_SDR_OUT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_NBC_IN_SDR_OUT;
            break;
        }
        case DRV_DOVI_NBC_IN_SDR_OUT_CERT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_NBC_IN_SDR_OUT_CERT;
            break;
        }
        case DRV_DOVI_NBC_IN_HDR10_OUT:
        {
            /* !!!!NOTECE!!!HAL NEED TO ADD NEW BRANCH TO OUTPUT 444 */
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_NBC_IN_HDR10_OUT;
            break;
        }
        case DRV_DOVI_NBC_IN_HDR10_OUT_CERT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_NBC_IN_HDR10_OUT_CERT;
            break;
        }
        case DRV_DOVI_NBC_IN_DOVI_YUV_OUT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_NBC_IN_DOVI_YUV_OUT;
            break;
        }
        case DRV_DOVI_NBC_IN_DOVI_IPT_OUT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_NBC_IN_DOVI_IPT_OUT;
            break;
        }
        case DRV_HDR10_IN_SDR_OUT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_HDR10_IN_SDR_OUT;
            break;
        }
        case DRV_HDR10_IN_SDR_OUT_CERT:
        {
            pstHalHdrCfg->enHdrMode = VDP_HDR_HDR10_IN_SDR_OUT_CERT;
            break;
        }
        case DRV_HDR10_IN_HDR10_OUT:
        {
            /* OUTPUT 10BIT 444 */
            pstHalHdrCfg->enHdrMode = VDP_HDR_HDR10_IN_HDR10_OUT ;
            break;
        }
        case DRV_HDR10_IN_HDR10_OUT_CERT:
        {
            /* OUTPUT 10BIT 422 */
            pstHalHdrCfg->enHdrMode = VDP_HDR_HDR10_IN_HDR10_OUT_CERT;
            break;
        }
        default:
            pstHalHdrCfg->enHdrMode = VDP_HDR_DOVI_NBC_IN_SDR_OUT;
            break;
    }

    switch (pstDrvPara->stBLFrame.enBitWidth)
    {
        case HI_DRV_PIXEL_BITWIDTH_8BIT :
        {
            pstHalHdrCfg->u32DataWidth = VDP_DATA_WTH_8;
            break;
        }
        case HI_DRV_PIXEL_BITWIDTH_10BIT :
        {
            pstHalHdrCfg->u32DataWidth = VDP_DATA_WTH_10;
            break;
        }
        default:
            pstHalHdrCfg->u32DataWidth = VDP_DATA_WTH_10;
            break;
    }

    TransferToHalPixelFmt(pstDrvPara->stBLFrame.enPixFormat,
                                    &pstHalHdrCfg->enVdpBlVidIFmt);
    TransferToHalPixelFmt(pstDrvPara->stELFrame.enPixFormat,
                                    &pstHalHdrCfg->enVdpElVidIFmt);

    TransferToHalReso(&pstDrvPara->stBLResoIn,&pstHalHdrCfg->stVidReso);
    TransferToHalReso(&pstDrvPara->stELResoIn,&pstHalHdrCfg->stElReso);
    TransferToHalReso(&pstDrvPara->stBLResoOut,&pstHalHdrCfg->stBlReso);

    TransferToDolbyHalAddr(&pstDrvPara->stBLFrame.stAddress[0],
                            &pstHalHdrCfg->stBlAddr,
                            HI_FALSE,
                            pstDrvPara->stBLFrame.bSecurity,
                            pstDrvPara->bSmmu);
    TransferToDolbyHalAddr(&pstDrvPara->stBLFrame.stAddress_lb[0],
                            &pstHalHdrCfg->stBlAddr_2LowBit,
                            HI_FALSE,
                            pstDrvPara->stBLFrame.bSecurity,
                            pstDrvPara->bSmmu);
    TransferToDolbyHalAddr(&pstDrvPara->stELFrame.stAddress[0],
                            &pstHalHdrCfg->stElAddr,
                            HI_FALSE,
                            pstDrvPara->stELFrame.bSecurity,
                            pstDrvPara->bSmmu);

    if(pstDrvPara->stBLFrame.bDcmpEn)
    {
        TransferToDolbyHalAddr(&pstDrvPara->stBLFrame.stAddress[0],
                                &pstHalHdrCfg->stHeadAddr,
                                HI_TRUE,
                                pstDrvPara->stBLFrame.bSecurity,
                                pstDrvPara->bSmmu);
    }
    pstHalHdrCfg->bDcmpEn = pstDrvPara->stBLFrame.bDcmpEn;
}

#endif


/*----------------------------remember to remove!!!!--------------------------*/

HI_VOID VDP_DRV_MaskAndBackupGfx(HI_U32 u32GfxLayerId, HI_U32* pu32Alpha, HI_U32* pu32MuteState)
{
    return;
}
HI_VOID VDP_DRV_UnmaskGfx(HI_U32 u32GfxLayerId, HI_U32 u32Alpha, HI_U32 u32MuteState)
{
    return;
}
HI_VOID VDP_DRV_SetGfxMixvBypassState(HI_U32 u32GfxLayerId, HI_BOOL bEnable)
{
    return;
}

HI_VOID VDP_DRV_GetGfxState(HI_BOOL *pbNeedProcessGfx)
{
    return;
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

