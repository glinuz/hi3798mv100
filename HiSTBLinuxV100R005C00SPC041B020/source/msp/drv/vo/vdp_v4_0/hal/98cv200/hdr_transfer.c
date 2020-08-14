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

/************HDR hal start******************/
HI_VOID TransferToHalReso(HI_RECT_S *pstDrvReso,VDP_RECT_S *pstCbbReso)
{
    pstCbbReso->u32X    = pstDrvReso->s32X;
    pstCbbReso->u32Y    = pstDrvReso->s32Y;
    pstCbbReso->u32Wth  = pstDrvReso->s32Width;
    pstCbbReso->u32Hgt  = pstDrvReso->s32Height;
    return ;
}

HI_VOID TransferToHalAddr(DRV_DISP_HDR_FRAME_ADDR_S *pstDrvAddr,
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

HI_VOID TransferToHalPixelFmt(HI_DRV_PIX_FORMAT_E enInputPixelFmt,VDP_VID_IFMT_E  *peOutPixelFmt)
{
    switch(enInputPixelFmt)
    {

        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_CMP:
        {
            *peOutPixelFmt              = VDP_VID_IFMT_SP_420;
            break;
        }
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1:
        {
            *peOutPixelFmt              = VDP_VID_IFMT_SP_422;
            break;
        }
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
        {
            *peOutPixelFmt               = VDP_VID_IFMT_SP_TILE_64;
            break;
        }
        case HI_DRV_PIX_FMT_UYVY:
        {
            *peOutPixelFmt               = VDP_VID_IFMT_PKG_UYVY;
            break;
        }
        case HI_DRV_PIX_FMT_YUYV:
        {
            *peOutPixelFmt               = VDP_VID_IFMT_PKG_YUYV;
            break;
        }
        case HI_DRV_PIX_FMT_YVYU:
        {
            *peOutPixelFmt               = VDP_VID_IFMT_PKG_YVYU;
            break;
        }
        default:
        {
            //error
            break;
        }
    }
}

HI_VOID TransferToHalColorSpace(HI_DRV_COLOR_SPACE_E enDrvCs, VDP_CLR_SPACE_E *penHalCs)
{
    switch(enDrvCs)
    {
        case HI_DRV_CS_BT709_YUV_LIMITED:
            *penHalCs = VDP_CLR_SPACE_YUV_709_L;
            break;
        case HI_DRV_CS_BT709_RGB_LIMITED:
            *penHalCs = VDP_CLR_SPACE_RGB_709;
            break;
        case HI_DRV_CS_BT709_YUV_FULL:
            *penHalCs = VDP_CLR_SPACE_YUV_709;
            break;
        case HI_DRV_CS_BT709_RGB_FULL:
            *penHalCs = VDP_CLR_SPACE_RGB_709;
            break;

        case HI_DRV_CS_BT2020_YUV_LIMITED:
            *penHalCs = VDP_CLR_SPACE_YUV_2020_L;
            break;
        case HI_DRV_CS_BT2020_RGB_LIMITED:
            *penHalCs = VDP_CLR_SPACE_RGB_2020_L;
            break;
        case HI_DRV_CS_BT2020_YUV_FULL:
            *penHalCs = VDP_CLR_SPACE_YUV_2020;
            break;
        case HI_DRV_CS_BT2020_RGB_FULL:
            *penHalCs = VDP_CLR_SPACE_RGB_2020;
            break;
        default:
            break;
        
    }
    return;
}

HI_VOID HAL_TransferToDolbyHalHdrCfg(HI_DRV_DISP_DOLBY_HDR_CFG_S *pstDrvPara,VDP_HDR_CFG_S *pstHalHdrCfg)
{
    pstHalHdrCfg->bGfxEn  = pstDrvPara->bGfxEn;
    pstHalHdrCfg->bMuteEn = pstDrvPara->bMuteEn;
    pstHalHdrCfg->bSmmuEn = pstDrvPara->bSmmu;
    pstHalHdrCfg->bPreMult = HI_FALSE;
    pstHalHdrCfg->bFlag480I576I = pstDrvPara->bSdFmt;
    pstHalHdrCfg->bSecureEn = pstDrvPara->bSecureEn;

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
    
    TransferToHalAddr(&pstDrvPara->stBLFrame.stAddress[0],
                            &pstHalHdrCfg->stBlAddr,
                            HI_FALSE,
                            pstDrvPara->stBLFrame.bSecurity,
                            pstDrvPara->bSmmu);
    TransferToHalAddr(&pstDrvPara->stBLFrame.stAddress_lb[0],
                            &pstHalHdrCfg->stBlAddr_2LowBit,
                            HI_FALSE,
                            pstDrvPara->stBLFrame.bSecurity,
                            pstDrvPara->bSmmu);
    TransferToHalAddr(&pstDrvPara->stELFrame.stAddress[0],
                            &pstHalHdrCfg->stElAddr,
                            HI_FALSE,
                            pstDrvPara->stELFrame.bSecurity,
                            pstDrvPara->bSmmu);   

    if(pstDrvPara->stBLFrame.bDcmpEn)
    {
        TransferToHalAddr(&pstDrvPara->stBLFrame.stAddress[0],
                                &pstHalHdrCfg->stHeadAddr,
                                HI_TRUE,
                                pstDrvPara->stBLFrame.bSecurity,
                                pstDrvPara->bSmmu);
    }
	pstHalHdrCfg->bDcmpEn = pstDrvPara->stBLFrame.bDcmpEn;

    return ;
}

//===========================add Hisi-self solution=============================
HI_VOID HAL_TransferToHiHalHdrCfg(HI_DRV_DISP_HISI_HDR_CFG_S *pstDrvPara,VDP_HISI_HDR_CFG_S *pstHalHdrCfg)
{
    pstHalHdrCfg->bGfxEn  = pstDrvPara->bGfxEn;
    pstHalHdrCfg->bMuteEn = pstDrvPara->bMuteEn;
    pstHalHdrCfg->bSmmuEn = pstDrvPara->bSmmu;
    pstHalHdrCfg->bPreMult = HI_FALSE;
    pstHalHdrCfg->bFlag480I576I = pstDrvPara->bSdFmt;
	pstHalHdrCfg->bSecureEn = pstDrvPara->bSecureEn;

    switch (pstDrvPara->enHdrMode)
    {            
        case DRV_HISI_HDR10_IN_SDR_OUT:
            pstHalHdrCfg->enHdrMode = VDP_HIHDR_HDR10_IN_SDR_OUT;
            break;
        case DRV_HISI_HDR10_IN_HDR10_OUT:
            /* OUTPUT 10BIT 444 */
            pstHalHdrCfg->enHdrMode = VDP_HIHDR_HDR10_IN_HDR10_OUT;
            break;
        case DRV_HISI_HLG_IN_SDR_OUT:
            pstHalHdrCfg->enHdrMode = VDP_HIHDR_HLG_IN_SDR_OUT;
            break;
        case DRV_HISI_HLG_IN_HDR10_OUT:
            pstHalHdrCfg->enHdrMode = VDP_HIHDR_HLG_IN_HDR10_OUT;
            break;
        default:
            pstHalHdrCfg->enHdrMode = VDP_HIHDR_HDR10_IN_SDR_OUT;
            break;
    }

    switch (pstDrvPara->stFrame.enBitWidth)
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

    TransferToHalColorSpace(pstDrvPara->enInColorSpace, &pstHalHdrCfg->enInClr);
    TransferToHalColorSpace(pstDrvPara->enOutColorSpace, &pstHalHdrCfg->enOutClr);
       
    TransferToHalPixelFmt(pstDrvPara->stFrame.enPixFormat, 
                                    &pstHalHdrCfg->enVidIFmt);

    TransferToHalReso(&pstDrvPara->stResoIn,&pstHalHdrCfg->stVidInReso);
    TransferToHalReso(&pstDrvPara->stResoOut,&pstHalHdrCfg->stVidOutReso);
    
    TransferToHalAddr(&pstDrvPara->stFrame.stAddress[0],
                            &pstHalHdrCfg->stVidAddr,
                            HI_FALSE,
                            pstDrvPara->stFrame.bSecurity,
                            pstDrvPara->bSmmu);
    TransferToHalAddr(&pstDrvPara->stFrame.stAddress_lb[0],
                            &pstHalHdrCfg->stVidAddr_2LowBit,
                            HI_FALSE,
                            pstDrvPara->stFrame.bSecurity,
                            pstDrvPara->bSmmu);   

    if(pstDrvPara->stFrame.bDcmpEn)
    {
        TransferToHalAddr(&pstDrvPara->stFrame.stAddress[0],
                                &pstHalHdrCfg->stHeadAddr,
                                HI_TRUE,
                                pstDrvPara->stFrame.bSecurity,
                                pstDrvPara->bSmmu);
    }
	pstHalHdrCfg->bDcmpEn = pstDrvPara->stFrame.bDcmpEn;

    return ;
}



  
#endif 



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

