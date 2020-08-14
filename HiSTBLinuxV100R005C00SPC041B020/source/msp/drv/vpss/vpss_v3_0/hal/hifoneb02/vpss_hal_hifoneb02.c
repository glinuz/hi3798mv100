
#include "vpss_hal_hifoneb02.h"
#include "linux/kthread.h"
#include "vpss_common.h"

#include <asm/barrier.h>
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#if 1
VPSS_HAL_CTX_S  stHalCtx[VPSS_IP_BUTT] =
{
    {
        .bInit = HI_FALSE,
        .bClockEn = HI_FALSE,
        .u32LogicVersion = HAL_VERSION_3798M,
        .u32BaseRegPhy   = VPSS0_BASE_ADDR,
        .u32BaseRegVir   = 0,
    },

    {
        .bInit = HI_FALSE,
        .bClockEn = HI_FALSE,
        .u32LogicVersion = HAL_VERSION_3798M,
        .u32BaseRegPhy   = VPSS1_BASE_ADDR,
        .u32BaseRegVir   = 0,
    }
};
#endif

VPSS_REG_PORT_E VPSS_HAL_AllocPortId(VPSS_HAL_PORT_INFO_S *pstHalPort,
    HI_BOOL abPortUsed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER])
{
    if (HI_FALSE == abPortUsed[VPSS_REG_HD])
    {			
         abPortUsed[VPSS_REG_HD] = HI_TRUE;
         return VPSS_REG_HD;
    }

#if 0
    if (HI_FALSE == abPortUsed[VPSS_REG_STR])
    {			
         abPortUsed[VPSS_REG_STR] = HI_TRUE;
         return VPSS_REG_STR;
    }
#endif
    if (HI_FALSE == abPortUsed[VPSS_REG_SD])
    {			
         abPortUsed[VPSS_REG_SD] = HI_TRUE;
         return VPSS_REG_SD;
    }

    return VPSS_REG_BUTT;
}

ZME_FORMAT_E VPSS_HAL_GetZmeFmt(HI_DRV_PIX_FORMAT_E enFormat)
{
    ZME_FORMAT_E enZmeFmt = HI_PQ_ALG_PIX_FORMAT_SP420;

    switch(enFormat)
    {
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            enZmeFmt = HI_PQ_ALG_PIX_FORMAT_SP420;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1_CMP:
        case HI_DRV_PIX_FMT_YUYV:
        case HI_DRV_PIX_FMT_YVYU:
        case HI_DRV_PIX_FMT_UYVY:
        //case HI_DRV_PIX_FMT_ARGB8888:   //sp420->sp422->csc->rgb
        //case HI_DRV_PIX_FMT_ABGR8888:
        //case HI_DRV_PIX_FMT_KBGR8888:
            enZmeFmt = HI_PQ_ALG_PIX_FORMAT_SP422;
            break;
        default:
            VPSS_FATAL("REG ERROR format %d\n",enFormat);
    }

    return enZmeFmt;
}



HI_S32 VPSS_HAL_SetDnrCfg(VPSS_IP_E enIP,HI_U32 u32AppVir,
                            VPSS_HAL_INFO_S *pstHalInfo)
{
    VPSS_HAL_FRAME_S *pstInInfo;
    HI_BOOL bdrEn;
    HI_BOOL bdbEn;
	HI_BOOL bdnrEn;
	HI_BOOL bEnDnrDither;

    pstInInfo = &(pstHalInfo->stInInfo);

    if(pstInInfo->u32Height <= 576 && pstInInfo->u32Width <= 720
		&&(pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
			)
		&&pstInInfo->u32Width >= 128
		&&pstInInfo->u32Height >= 64
		&&((pstInInfo->u32Width % 16) == 0) 
		&&((pstInInfo->u32Height % 16) == 0) 
		)

    {
		bdnrEn = 1;
		bEnDnrDither = 1;
    }
    else
    {
		bdnrEn = 0;
		bEnDnrDither = 0;
    }

    if(pstInInfo->u32Height <= 1080 && pstInInfo->u32Width <= 1920)
    {
		bdbEn = 1;
		bdrEn = 1;
    }
    else
    {
		bdbEn = 0;
		bdrEn = 0;
    }

    VPSS_REG_SetDREn(u32AppVir, bdrEn);
    VPSS_REG_SetDBEn(u32AppVir,bdbEn);
	VPSS_REG_SetDNREn(u32AppVir,bdnrEn);

	VPSS_REG_SetDnrDitherEn(u32AppVir, bEnDnrDither);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetHSCLCfg(VPSS_IP_E enIP,HI_U32 u32AppVir,
                            VPSS_HAL_INFO_S *pstHalInfo)
{
    HI_PQ_ZME_PARA_IN_S stZmeDrvPara;
    HI_PQ_ZME_PARA_IN_S *pstZmeDrvPara;
    VPSS_HAL_FRAME_S *pstInInfo;

    pstInInfo = &(pstHalInfo->stInInfo);

    pstZmeDrvPara  = &(stZmeDrvPara);

    memset(pstZmeDrvPara,0,sizeof(HI_PQ_ZME_PARA_IN_S));

    if (pstInInfo->bProgressive != HI_TRUE)
    {
        pstZmeDrvPara->bZmeFrmFmtIn = 1;
    }
    else
    {
        if (pstInInfo->enFieldMode == HI_DRV_FIELD_ALL)
        {
            pstZmeDrvPara->bZmeFrmFmtIn = 1;
        }
        else
        {
            pstZmeDrvPara->bZmeFrmFmtIn = 0;
        }
    }
    pstZmeDrvPara->bZmeFrmFmtOut = 1;

    if (pstInInfo->enFieldMode == HI_DRV_FIELD_TOP)
    {
        pstZmeDrvPara->bZmeBFIn = 0;
    }
    else if (pstInInfo->enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstZmeDrvPara->bZmeBFIn = 1;
    }
    else
    {
        pstZmeDrvPara->bZmeBFIn = 0;
    }
    pstZmeDrvPara->bZmeBFOut = 0;

    if (pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV61
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV16
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV422_1X2
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV420p
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p
        )
    {
        pstZmeDrvPara->u8ZmeYCFmtIn = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
    }
    else
    {
        pstZmeDrvPara->u8ZmeYCFmtIn = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
    }

    pstZmeDrvPara->u8ZmeYCFmtOut = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;

    pstZmeDrvPara->u32ZmeFrmHIn = pstInInfo->u32Height;
    pstZmeDrvPara->u32ZmeFrmWIn = pstInInfo->u32Width;

    pstZmeDrvPara->u32ZmeFrmHOut = pstInInfo->u32Height;

    if (pstInInfo->u32Width % 4 != 0)
    {
        VPSS_ERROR("Invalid image width %d\n",pstInInfo->u32Width);
    }

    //pstZmeDrvPara->u32ZmeFrmWOut = pstInInfo->u32Width/2;
    pstZmeDrvPara->u32ZmeFrmWOut = 1920;

    pstZmeDrvPara->stOriRect.s32X = 0;
    pstZmeDrvPara->stOriRect.s32Y = 0;
    pstZmeDrvPara->stOriRect.s32Width = pstInInfo->u32Width;
    pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height;
    pstZmeDrvPara->u32InRate = 25000;
    pstZmeDrvPara->u32OutRate = 25000;
    pstZmeDrvPara->bDispProgressive = HI_TRUE;
    pstZmeDrvPara->u32Fidelity = 0;

    DRV_PQ_SetVpssZme(HI_PQ_VPSS_HSCL_LAYER_ZME,
                      (VPSS_REG_S*)u32AppVir,
                      pstZmeDrvPara,
                      HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetZmeCfg(VPSS_IP_E enIP,HI_U32 u32AppVir,
                            VPSS_HAL_INFO_S *pstHalInfo,
                            VPSS_REG_PORT_E enPort,
                            HI_U32 u32PortCnt)
{
    VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;
    HI_PQ_ZME_PARA_IN_S stZmeDrvPara;
    HI_PQ_ZME_PARA_IN_S *pstZmeDrvPara;
    VPSS_HAL_FRAME_S *pstInInfo;
    HI_PQ_VPSS_ZME_LAYER_E enPqPort = HI_PQ_VPSS_LAYER_ZME_BUTT;

	if (VPSS_REG_BUTT <= enPort)
	{
		VPSS_ERROR("Invalid enPort %d\n",enPort);	
		return HI_FAILURE;
	}
    
    pstHalPort = &pstHalInfo->astPortInfo[u32PortCnt];

    pstInInfo = &(pstHalInfo->stInInfo);

    pstZmeDrvPara  = &(stZmeDrvPara);

    memset(pstZmeDrvPara,0,sizeof(HI_PQ_ZME_PARA_IN_S));

#if 0
    if (pstInInfo->bProgressive != HI_TRUE)
    {
        pstZmeDrvPara->bZmeFrmFmtIn = 1;
    }
    else
    {
        if (pstInInfo->enFieldMode == HI_DRV_FIELD_ALL)
        {
            pstZmeDrvPara->bZmeFrmFmtIn = 1;
        }
        else
        {
            pstZmeDrvPara->bZmeFrmFmtIn = 0;
        }
    }
#else
	pstZmeDrvPara->bZmeFrmFmtIn = 1;
#endif
    pstZmeDrvPara->bZmeFrmFmtOut = 1;

#if 0
    if (pstInInfo->enFieldMode == HI_DRV_FIELD_TOP)
    {
        pstZmeDrvPara->bZmeBFIn = 0;
    }
    else if (pstInInfo->enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstZmeDrvPara->bZmeBFIn = 1;
    }
    else
    {
        pstZmeDrvPara->bZmeBFIn = 0;
    }
#else
	pstZmeDrvPara->bZmeBFIn = 0;
#endif
    pstZmeDrvPara->bZmeBFOut = 0;

    if (pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV61
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV16
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV422_1X2
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV420p
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p
        )
    {
        pstZmeDrvPara->u8ZmeYCFmtIn = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
    }
    else
    {
        pstZmeDrvPara->u8ZmeYCFmtIn = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
    }

    if (pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21
        || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12
        || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP
        || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP)
    {
        pstZmeDrvPara->u8ZmeYCFmtOut = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
    }
    else
    {
        pstZmeDrvPara->u8ZmeYCFmtOut = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
    }

    pstZmeDrvPara->u32ZmeFrmHIn = pstInInfo->u32Height;
    pstZmeDrvPara->u32ZmeFrmWIn = pstInInfo->u32Width;

#if 0
    if (pstInInfo->bProgressive == HI_TRUE)
    {
        if (pstInInfo->enFieldMode != HI_DRV_FIELD_ALL)
        {
            pstZmeDrvPara->u32ZmeFrmHIn = pstZmeDrvPara->u32ZmeFrmHIn*2;
        }
    }
#endif

    pstZmeDrvPara->u32ZmeFrmHOut = pstHalPort->stVideoRect.s32Height;
    pstZmeDrvPara->u32ZmeFrmWOut = pstHalPort->stVideoRect.s32Width;

	/*
	for SD port, ZME out should not exceed 1920x1088
	*/
	if ( VPSS_REG_SD == enPort)
	{
		if ( pstZmeDrvPara->u32ZmeFrmHOut >= 1088 || 
			 pstZmeDrvPara->u32ZmeFrmWOut >=1920)
		{
			pstZmeDrvPara->u32ZmeFrmHOut = 1080;
			pstZmeDrvPara->u32ZmeFrmWOut = 1920;
		}
	}

	#if 0
    VPSS_ERROR("IN W %d H %d  W %d H %d\n",
        pstZmeDrvPara->u32ZmeFrmWIn,
        pstZmeDrvPara->u32ZmeFrmHIn,
        pstZmeDrvPara->u32ZmeFrmWOut,
        pstZmeDrvPara->u32ZmeFrmHOut);
	#endif

    pstZmeDrvPara->stOriRect.s32X = 0;
    pstZmeDrvPara->stOriRect.s32Y = 0;
    pstZmeDrvPara->stOriRect.s32Width = pstInInfo->u32Width;
    pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height;
    pstZmeDrvPara->u32InRate = 25000;
    pstZmeDrvPara->u32OutRate = 25000;
    pstZmeDrvPara->bDispProgressive = HI_TRUE;
    pstZmeDrvPara->u32Fidelity = 0;

    switch(enPort)
    {
        case VPSS_REG_HD:
            enPqPort = HI_PQ_VPSS_PORT0_LAYER_ZME;
            break;
        case VPSS_REG_SD:
            enPqPort = HI_PQ_VPSS_PORT1_LAYER_ZME;
            break;
//        case VPSS_REG_STR:
//            enPqPort = HI_PQ_VPSS_PORT2_LAYER_ZME;
            break;
        default:
            break;
    }

	if ( enPqPort == HI_PQ_VPSS_LAYER_ZME_BUTT)
	{
		VPSS_ERROR("func=%s,enPqPort is %d\n",__func__,enPqPort);
		return HI_FAILURE;
	}
	
    DRV_PQ_SetVpssZme(enPqPort,
                      (VPSS_REG_S*)u32AppVir,
                      pstZmeDrvPara,
                      HI_TRUE);

    return HI_SUCCESS;
}
HI_BOOL VPSS_HAL_CheckNeedCmp(VPSS_HAL_FRAME_S *pstOutFrm)
{

    if((HI_DRV_PIX_FMT_NV12_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV21_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV16_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV61_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV16_2X1_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV61_2X1_CMP == pstOutFrm->enFormat)
        )
    {

        return HI_TRUE;
    }

    return HI_FALSE;
}
/******************************************************************************
brief      : CNcomment:  Config Port Compress info
attention  :
param[in]  :
retval     : HI_S32
see        :
author	   :f00241306
******************************************************************************/

HI_S32 VPSS_HAL_SetPortCmpCfg(HI_U32 u32AppVir,VPSS_REG_PORT_E ePort, VPSS_HAL_PORT_INFO_S *pstHalPort)
{
    HI_BOOL bIs_lossless;
    HI_BOOL bCmp_Mode;
    HI_U32 u32Dw_Mode;
    HI_U32 u32CmpRatioY;
    HI_U32 u32CmpRatioC;
    HI_U32 u32YMaxQp;
    HI_U32 u32CMaxQp;
    HI_DRV_PIXEL_BITWIDTH_E  enBitWidth;
    ZME_FORMAT_E enOutFormat;
    REG_CMP_DATA_S stYcmpData;
    REG_CMP_DATA_S stCcmpData;
    VPSS_HAL_FRAME_S *pstOutFrm;

    if(VPSS_REG_HD != ePort)
    {
        VPSS_FATAL("Vpss cmp error,only VHD support!!\n");
        return HI_FAILURE;
    }

    pstOutFrm =  &(pstHalPort->stOutInfo);
    enBitWidth = pstHalPort->stOutInfo.enBitWidth;

	if( ! ((HI_DRV_PIXEL_BITWIDTH_8BIT == enBitWidth) || (HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth)) )
    {
        VPSS_FATAL("Vpss cmp error,only 8BIT and 10BIT support!!\n");
        return HI_FAILURE;
    }

    u32CmpRatioY = ( HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 140 : 130;
    u32CmpRatioC = (HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 140 : 130;
    u32YMaxQp  = ( HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 4 : 2;
    u32CMaxQp  = ( HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 4 : 2;

    if(HI_DRV_PIXEL_BITWIDTH_8BIT == enBitWidth)
    {
        u32YMaxQp = 2;
		u32CMaxQp = 2;
    }
    else
    {
		u32YMaxQp = 4;
		u32CMaxQp = 4;
    }

    bIs_lossless = !(pstHalPort->bCmpLoss);//vdp will control

    bCmp_Mode = HI_FALSE;//cmp mode
    u32Dw_Mode = (HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 0 : 1;

    if(pstOutFrm->u32Width < 128)
    {
        bIs_lossless = HI_TRUE;
    }

	if ( pstOutFrm->u32Width < 32 || pstOutFrm->u32Width > 8192 || 
		 pstOutFrm->u32Height < 32 || pstOutFrm->u32Height > 8192)
	{
		VPSS_FATAL("Vpss cmp error, input IMG Width or Height not support!!\n");
        return HI_FAILURE;
	}

    enOutFormat = VPSS_HAL_GetZmeFmt(pstOutFrm->enFormat);
//calc y cmp config
    stYcmpData.u32Width = pstOutFrm->u32Width ;
    stYcmpData.u32Height = pstOutFrm->u32Height ;
    stYcmpData.u32Cfg0_Mb_bits = 32 * 100 * ((enBitWidth == 0) ? 10 : 8) / u32CmpRatioY;
    stYcmpData.u32Cfg0_Ri_bits = (enBitWidth == 1) ? 3 : 3;
    stYcmpData.u32Cfg0_Max_rg_comp_bits = (enBitWidth == 1) ? 12 : 10;
    stYcmpData.u32Cfg1_Max_qp = u32YMaxQp;
    stYcmpData.u32Cfg1_Rc_smth_ngain = (enBitWidth == 1) ? 1 : 1;
    stYcmpData.u32Cfg1_Sad_bits_ngain = (enBitWidth == 1) ? 1 : 1;
    stYcmpData.u32Cfg2_Pix_diff_thr = (enBitWidth == 1) ? 200 : 50;
    stYcmpData.u32Cfg2_Smth_pix_num_thr = (enBitWidth == 1) ? 4 : 4;
    stYcmpData.u32Cfg2_Smth_thr = (enBitWidth == 1) ? 12 : 3;
    stYcmpData.u32Cfg3_Adj_sad_bit_thr = (enBitWidth == 1) ? 25 : 20;
    stYcmpData.u32Cfg3_Adj_sad_thr = (enBitWidth == 1) ? 1280 : 320;
    stYcmpData.u32Cfg4_Qp_chg1_bits_thr = (enBitWidth == 1) ? 35 : 28;
    stYcmpData.u32Cfg4_Qp_chg2_bits_thr = (enBitWidth == 1) ? 75 : 60;
    stYcmpData.u32Cfg5_Smth_lftbits_thr0 = (enBitWidth == 1) ? 12 : 10;
    stYcmpData.u32Cfg5_Smth_lftbits_thr1 = (enBitWidth == 1) ? 25 : 20;
    stYcmpData.u32Cfg5_Smth_qp0 = (enBitWidth == 1) ? 1 : 1;
    stYcmpData.u32Cfg5_Smth_qp1 = (enBitWidth == 1) ? 2 : 2;
//calc c cmp config
    stCcmpData.u32Width = pstOutFrm->u32Width / 2 ;
    stCcmpData.u32Height = (HI_PQ_ALG_PIX_FORMAT_SP422 == enOutFormat) ? pstOutFrm->u32Height : pstOutFrm->u32Height / 2;
    stCcmpData.u32Cfg0_Mb_bits = 64 * 100 * ((enBitWidth == 0) ? 10 : 8) / u32CmpRatioC;
    stCcmpData.u32Cfg0_Ri_bits = (enBitWidth == 1) ? 7 : 6;
    stCcmpData.u32Cfg0_Max_rg_comp_bits = (enBitWidth == 1) ? 25 : 20;
    stCcmpData.u32Cfg1_Max_qp = u32CMaxQp;
    stCcmpData.u32Cfg1_Rc_smth_ngain = (enBitWidth == 1) ? 0 : 0;
    stCcmpData.u32Cfg1_Sad_bits_ngain = (enBitWidth == 1) ? 1 : 1;
    stCcmpData.u32Cfg2_Pix_diff_thr = (enBitWidth == 1) ? 200 : 50;
    stCcmpData.u32Cfg2_Smth_pix_num_thr = (enBitWidth == 1) ? 4 : 4;
    stCcmpData.u32Cfg2_Smth_thr = (enBitWidth == 1) ? 12 : 3;
    stCcmpData.u32Cfg3_Adj_sad_bit_thr = (enBitWidth == 1) ? 50 : 40;
    stCcmpData.u32Cfg3_Adj_sad_thr = (enBitWidth == 1) ? 1920 : 480;
    stCcmpData.u32Cfg4_Qp_chg1_bits_thr = (enBitWidth == 1) ? 70 : 56;
    stCcmpData.u32Cfg4_Qp_chg2_bits_thr = (enBitWidth == 1) ? 150 : 120;
    stCcmpData.u32Cfg5_Smth_lftbits_thr0 = (enBitWidth == 1) ? 12 : 10;
    stCcmpData.u32Cfg5_Smth_lftbits_thr1 = (enBitWidth == 1) ? 25 : 20;
    stCcmpData.u32Cfg5_Smth_qp0 = (enBitWidth == 1) ? 1 : 1;
    stCcmpData.u32Cfg5_Smth_qp1 = (enBitWidth == 1) ? 2 : 2;

    VPSS_REG_SetPortLCmpEn(u32AppVir, ePort, HI_TRUE);

    //:Y
	VPSS_REG_SetLossLess_Y(u32AppVir,bIs_lossless);
	VPSS_REG_SetVhd0CmpMode_Y(u32AppVir,bCmp_Mode);
	VPSS_REG_SetVhd0DwMode_Y(u32AppVir,u32Dw_Mode);

	VPSS_REG_SetVhd0LcmpSize_Y(u32AppVir, stYcmpData.u32Width, stYcmpData.u32Height);

	VPSS_REG_SetVhd0LcmpMbBit_Y(u32AppVir,stYcmpData.u32Cfg0_Mb_bits);
	VPSS_REG_SetVhd0LcmpRiBit_Y(u32AppVir, stYcmpData.u32Cfg0_Ri_bits);
	VPSS_REG_SetVhd0LcmpMaxRg_Y(u32AppVir, stYcmpData.u32Cfg0_Max_rg_comp_bits);

	VPSS_REG_SetVhd0LcmpMaxQp_Y(u32AppVir, stYcmpData.u32Cfg1_Max_qp);
	VPSS_REG_SetVhd0LcmpSadBit_Y(u32AppVir, stYcmpData.u32Cfg1_Sad_bits_ngain);
	VPSS_REG_SetVhd0LcmpRcSmth_Y(u32AppVir, stYcmpData.u32Cfg1_Rc_smth_ngain);

	VPSS_REG_SetVhd0LcmpSmthThr_Y(u32AppVir, stYcmpData.u32Cfg2_Smth_thr);
	VPSS_REG_SetVhd0LcmpSmthPixNum_Y(u32AppVir, stYcmpData.u32Cfg2_Smth_pix_num_thr);
	VPSS_REG_SetVhd0LcmpPixDiff_Y(u32AppVir, stYcmpData.u32Cfg2_Pix_diff_thr);

	VPSS_REG_SetVhd0LcmpAdjSad_Y(u32AppVir, stYcmpData.u32Cfg3_Adj_sad_thr);
	VPSS_REG_SetVhd0LcmpAdjBit_Y(u32AppVir, stYcmpData.u32Cfg3_Adj_sad_bit_thr);

	VPSS_REG_SetVhd0LcmpQpChg1_Y(u32AppVir, stYcmpData.u32Cfg4_Qp_chg1_bits_thr);
	VPSS_REG_SetVhd0LcmpQpChg2_Y(u32AppVir, stYcmpData.u32Cfg4_Qp_chg2_bits_thr);

	VPSS_REG_SetVhd0LcmpSmthQp0_Y(u32AppVir, stYcmpData.u32Cfg5_Smth_qp0);
	VPSS_REG_SetVhd0LcmpSmthQp1_Y(u32AppVir, stYcmpData.u32Cfg5_Smth_qp1);
	VPSS_REG_SetVhd0LcmpSmthThr0_Y(u32AppVir, stYcmpData.u32Cfg5_Smth_lftbits_thr0);
	VPSS_REG_SetVhd0LcmpSmthThr1_Y(u32AppVir, stYcmpData.u32Cfg5_Smth_lftbits_thr1);

	//:C
	VPSS_REG_SetLossLess_C(u32AppVir,bIs_lossless);
	VPSS_REG_SetVhd0CmpMode_C(u32AppVir,bCmp_Mode);
	VPSS_REG_SetVhd0DwMode_C(u32AppVir,u32Dw_Mode);

	VPSS_REG_SetVhd0LcmpSize_C(u32AppVir, stCcmpData.u32Width, stCcmpData.u32Height);

	VPSS_REG_SetVhd0LcmpMbBit_C(u32AppVir,stCcmpData.u32Cfg0_Mb_bits);
	VPSS_REG_SetVhd0LcmpRiBit_C(u32AppVir, stCcmpData.u32Cfg0_Ri_bits);
	VPSS_REG_SetVhd0LcmpMaxRg_C(u32AppVir, stCcmpData.u32Cfg0_Max_rg_comp_bits);

	VPSS_REG_SetVhd0LcmpMaxQp_C(u32AppVir, stCcmpData.u32Cfg1_Max_qp);
	VPSS_REG_SetVhd0LcmpSadBit_C(u32AppVir, stCcmpData.u32Cfg1_Sad_bits_ngain);
	VPSS_REG_SetVhd0LcmpRcSmth_C(u32AppVir, stCcmpData.u32Cfg1_Rc_smth_ngain);

	VPSS_REG_SetVhd0LcmpSmthThr_C(u32AppVir, stCcmpData.u32Cfg2_Smth_thr);
	VPSS_REG_SetVhd0LcmpSmthPixNum_C(u32AppVir, stCcmpData.u32Cfg2_Smth_pix_num_thr);
	VPSS_REG_SetVhd0LcmpPixDiff_C(u32AppVir, stCcmpData.u32Cfg2_Pix_diff_thr);

	VPSS_REG_SetVhd0LcmpAdjSad_C(u32AppVir, stCcmpData.u32Cfg3_Adj_sad_thr);
	VPSS_REG_SetVhd0LcmpAdjBit_C(u32AppVir, stCcmpData.u32Cfg3_Adj_sad_bit_thr);

	VPSS_REG_SetVhd0LcmpQpChg1_C(u32AppVir, stCcmpData.u32Cfg4_Qp_chg1_bits_thr);
	VPSS_REG_SetVhd0LcmpQpChg2_C(u32AppVir, stCcmpData.u32Cfg4_Qp_chg2_bits_thr);

	VPSS_REG_SetVhd0LcmpSmthQp0_C(u32AppVir, stCcmpData.u32Cfg5_Smth_qp0);
	VPSS_REG_SetVhd0LcmpSmthQp1_C(u32AppVir, stCcmpData.u32Cfg5_Smth_qp1);
	VPSS_REG_SetVhd0LcmpSmthThr0_C(u32AppVir, stCcmpData.u32Cfg5_Smth_lftbits_thr0);
	VPSS_REG_SetVhd0LcmpSmthThr1_C(u32AppVir, stCcmpData.u32Cfg5_Smth_lftbits_thr1);

    return HI_SUCCESS;
}
/*
*  VPSS_HAL_SetPortCfg函数应该只需要传 VPSS_HAL_PORT_INFO_S数组信息即可，
*  但由于缩放需要源格式 所以传递了VPSS_HAL_INFO_S信息，也同时为后期预缩放场景提供扩展
*/
HI_S32 VPSS_HAL_SetPortCfg(VPSS_IP_E enIP,HI_U32 u32AppVir, HI_U32 u32AppPhy,
    VPSS_HAL_INFO_S *pstHalInfo)
{
    HI_U32 u32Count;
	HI_U32 u32PortAvailable = DEF_VPSS_HAL_PORT_NUM;
    HI_BOOL abPortUsed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER]
        = {HI_FALSE, HI_FALSE, HI_FALSE};
    HI_U32 VPSS_VHD0_LCMP_ADDR;

    /* 先默认关闭三个硬件输出通道 */
    VPSS_REG_EnPort(u32AppVir, VPSS_REG_HD, HI_FALSE);
    //VPSS_REG_EnPort(u32AppVir, VPSS_REG_STR, HI_FALSE);
    //VPSS_REG_EnPort(u32AppVir, VPSS_REG_SD,  HI_FALSE);
    //VPSS_REG_SetFidelity(u32AppVir, HI_FALSE); //tqy ,hifoneb02 no this register bit.
	
    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER && u32PortAvailable > 0; u32Count ++)
    {
        VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;

        pstHalPort = &pstHalInfo->astPortInfo[u32Count];
        if (pstHalPort->bEnable && pstHalPort->bConfig == HI_FALSE)
        {
            VPSS_REG_PORT_E enPort = VPSS_REG_BUTT;
            VPSS_HAL_FRAME_S *pstOutFrm = HI_NULL;
            HI_DRV_VID_FRAME_ADDR_S *pstOutAddr = HI_NULL;

            enPort = VPSS_HAL_AllocPortId(pstHalPort, abPortUsed);
            if (enPort == VPSS_REG_BUTT)
            {
                HI_ASSERT(0);
            }
			else
			{
				u32PortAvailable--;
			}

            pstOutFrm = &pstHalPort->stOutInfo;
            pstOutAddr = &pstOutFrm->stAddr;

            /* Flip&Mirro */
            VPSS_REG_SetPortMirrorEn(u32AppVir, enPort,pstHalPort->bNeedMirror);  //暂时未考虑旋转
            VPSS_REG_SetPortFlipEn(u32AppVir, enPort,pstHalPort->bNeedFlip);    //暂时未考虑旋转

            /* UV反转 */
			VPSS_REG_SetPortUVInvert(u32AppVir, enPort, pstOutFrm->bUVInver);

            /* PreZme */
            //VPSS_REG_SetFrmPreZmeEn(u32AppVir, enPort, HI_FALSE, HI_FALSE);

            /*ZME*/
            VPSS_HAL_SetZmeCfg(enIP,u32AppVir, pstHalInfo,enPort,u32Count);
			VPSS_REG_SetAddr(u32AppVir,REG_VPSS_ZME_ADDR,VPSS_ZME_ADDR_GET(u32AppVir));
			VPSS_REG_SetZmeEn(u32AppVir, enPort, HI_TRUE);

            /* LBX */
            VPSS_REG_SetLBABg(u32AppVir, enPort, 0x02080200, 0x7f);
            VPSS_REG_SetLBAVidPos(u32AppVir, enPort,
                        (HI_U32)pstHalPort->stVideoRect.s32X,
                        (HI_U32)pstHalPort->stVideoRect.s32Y,
                        pstHalPort->stVideoRect.s32Height,
                        pstHalPort->stVideoRect.s32Width);
            VPSS_REG_SetLBADispPos(u32AppVir, enPort, 0, 0,
                pstOutFrm->u32Height, pstOutFrm->u32Width);
            VPSS_REG_SetLBAEn(u32AppVir, enPort, HI_TRUE);


            /* 输出格式 */
            VPSS_REG_SetFrmSize(u32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
            VPSS_REG_SetFrmAddr(u32AppVir,enPort,pstOutAddr->u32PhyAddr_Y, pstOutAddr->u32PhyAddr_C);
            VPSS_REG_SetFrmStride(u32AppVir,enPort,pstOutAddr->u32Stride_Y,pstOutAddr->u32Stride_C);
            VPSS_REG_SetFrmFormat(u32AppVir,enPort,pstOutFrm->enFormat);
            //VPSS_REG_SetFrmBitWidth(u32AppVir,enPort,pstOutFrm->enBitWidth);

			/* Set Output BitWidth   */
			VPSS_REG_SetPortPixBitW(u32AppVir,enPort, pstOutFrm->enBitWidth);

            /*if bitwidth < 10, dither is neccesary.*/
            if (pstOutFrm->enBitWidth > HI_DRV_PIXEL_BITWIDTH_8BIT)
            {
				VPSS_REG_SetPortDitherEn(u32AppVir,enPort, HI_FALSE);
            }
            else
            {
                VPSS_REG_SetPortDitherEn(u32AppVir,enPort, HI_TRUE);
            }

			VPSS_REG_SetPortDitherMode(u32AppVir,enPort, REG_DITHER_MODE_ROUND);
          
#if 1   //@f00241306 for CMP
            if(enPort == VPSS_REG_HD &&  VPSS_HAL_CheckNeedCmp(pstOutFrm))
            {
                //压缩配置
				VPSS_VHD0_LCMP_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_VHD0_LCMP_YINFO) - sizeof(HI_U32);

				VPSS_REG_SetAddr(u32AppVir, REG_VPSS_VHD0_LCMP_ADDR, VPSS_VHD0_LCMP_ADDR);

                VPSS_HAL_SetPortCmpCfg(u32AppVir, enPort, pstHalPort);
            }
#endif
			pstHalPort->bConfig = HI_TRUE;

            VPSS_REG_EnPort(u32AppVir, enPort, HI_TRUE);
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetAllAlgCfgAddr(HI_U32 u32AppVir, HI_U32 u32AppPhy)
{
    #if 0
    VPSS_REG_SetVc1StrCfgAddr(u32AppVir, u32AppPhy+0x2400);
    VPSS_REG_SetZmeCfgAddr(u32AppVir, u32AppPhy+0x2000);
    VPSS_REG_SetHspCfgAddr(u32AppVir, u32AppPhy+0x2100);
    VPSS_REG_SetSnrCfgAddr(u32AppVir, u32AppPhy+0x3000);
    VPSS_REG_SetDbCfgAddr(u32AppVir, u32AppPhy+0x2200);
    VPSS_REG_SetDrCfgAddr(u32AppVir, u32AppPhy+0x2300);
    VPSS_REG_SetDeiCfgAddr(u32AppVir, u32AppPhy+0x1000);
    VPSS_REG_SetTnrCfgAddr(u32AppVir, u32AppPhy+0x3800);
    VPSS_REG_SetTnrClutCfgAddr(u32AppVir, u32AppPhy+0x3b00);
    VPSS_REG_SetEsCfgAddr(u32AppVir, u32AppPhy+0x2500);
    #endif
    return HI_SUCCESS;
}
HI_S32 VPSS_HAL_SetRwzbCfg(HI_U32 u32AppAddr,VPSS_RWZB_INFO_S *pstRwzbInfo)
{
    HI_U32 u32Count;

    if( pstRwzbInfo->u32EnRwzb == 0x1)
    {
        for(u32Count = 0; u32Count < 6; u32Count++)
        {
            VPSS_REG_SetDetBlk(u32AppAddr,u32Count,&(pstRwzbInfo->u32Addr[u32Count][0]));
                #if 1
                VPSS_INFO("adddr X %d Y %d\n",pstRwzbInfo->u32Addr[u32Count][0],
                                pstRwzbInfo->u32Addr[u32Count][1]);
                                #endif
        }

    }
    VPSS_REG_SetDetEn(u32AppAddr,pstRwzbInfo->u32EnRwzb);
    VPSS_REG_SetDetMode(u32AppAddr,pstRwzbInfo->u32Mode);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetDeiCfg(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir,HI_U32 u32AppPhy)

{
    HI_U32  VPSS_REFYADDR;
    HI_U32  VPSS_REFCADDR;
    HI_U32  VPSS_REFCRADDR;

    HI_U32  VPSS_NEXT1YADDR;
    HI_U32  VPSS_NEXT1CADDR;
    HI_U32  VPSS_NEXT1CRADDR;

    HI_U32  VPSS_NEXT3YADDR;
    HI_U32  VPSS_NEXT3CADDR;
    HI_U32  VPSS_NEXT3CRADDR;

    HI_U32  VPSS_DEI_ADDR;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

    /*dei*/
    VPSS_REG_SetImgReadMod(u32AppVir, HI_TRUE);
    VPSS_REG_EnDei(u32AppVir, HI_TRUE);
    VPSS_REG_SetDeiTopFirst(u32AppVir, pstHalInfo->stInInfo.bTopFirst);

    switch(pstHalInfo->stInInfo.enFieldMode)
    {
        case HI_DRV_FIELD_BOTTOM:
            VPSS_REG_SetDeiFieldMode(u32AppVir, HI_TRUE);
            break;
        case HI_DRV_FIELD_TOP:
            VPSS_REG_SetDeiFieldMode(u32AppVir, HI_FALSE);
            break;
        default:
            VPSS_FATAL("No spt field Type:%d!\n", pstHalInfo->stInInfo.enFieldMode);
            return HI_FAILURE;
    }
    //VPSS_REG_SetModeEn(u32AppVir,REG_DIE_MODE_CHROME,HI_TRUE);
    //VPSS_REG_SetModeEn(u32AppVir,REG_DIE_MODE_LUMA,HI_TRUE);
    //VPSS_REG_SetOutSel(u32AppVir,REG_DIE_MODE_ALL,HI_FALSE);
//VPSS_REG_SetMorFlt(u32AppVir,1,0,0);
    #if 1
    VPSS_REG_SetStWrAddr(u32AppVir,pstHalInfo->stHisAddr.u32WPhyAddr);

    VPSS_REG_SetStRdAddr(u32AppVir,
                        pstHalInfo->stHisAddr.u32RPhyAddr);
    VPSS_REG_SetStStride(u32AppVir,
                        pstHalInfo->stHisAddr.u32Stride);

    //HI_S32 VPSS_REG_SetDeiParaAddr(HI_U32 u32ParaAddr);
    VPSS_DEI_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DIECTRL)-sizeof(HI_U32);
    VPSS_REG_SetDeiParaAddr(u32AppVir,
                        VPSS_DEI_ADDR);
    #endif
    VPSS_REG_SetMode(u32AppVir, REG_DIE_MODE_ALL, 1);

    VPSS_REFYADDR = pstHalInfo->stFieldAddr[0].u32PhyAddr_Y;
    VPSS_REFCADDR = pstHalInfo->stFieldAddr[0].u32PhyAddr_C;
    VPSS_REFCRADDR = pstHalInfo->stFieldAddr[0].u32PhyAddr_Cr;

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,LAST_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,LAST_FIELD,HI_FALSE);
    }
    VPSS_REG_SetDeiAddr(u32AppVir,LAST_FIELD,VPSS_REFYADDR,VPSS_REFCADDR, VPSS_REFCRADDR);
    VPSS_REG_SetDeiStride(u32AppVir,LAST_FIELD,
                            pstHalInfo->stFieldAddr[0].u32Stride_Y,
                            pstHalInfo->stFieldAddr[0].u32Stride_C);

    /*********/

    /*********/
    VPSS_NEXT1YADDR = pstHalInfo->stFieldAddr[3].u32PhyAddr_Y;
    VPSS_NEXT1CADDR = pstHalInfo->stFieldAddr[3].u32PhyAddr_C;
    VPSS_NEXT1CRADDR = pstHalInfo->stFieldAddr[3].u32PhyAddr_Cr;
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,NEXT1_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,NEXT1_FIELD,HI_FALSE);
    }
    VPSS_REG_SetDeiAddr(u32AppVir,NEXT1_FIELD,VPSS_NEXT1YADDR,VPSS_NEXT1CADDR, VPSS_NEXT1CRADDR);
    VPSS_REG_SetDeiStride(u32AppVir,NEXT1_FIELD,
                            pstHalInfo->stFieldAddr[3].u32Stride_Y,
                            pstHalInfo->stFieldAddr[3].u32Stride_C);

    /*********/

    /*********/
    VPSS_NEXT3YADDR = pstHalInfo->stFieldAddr[4].u32PhyAddr_Y;
    VPSS_NEXT3CADDR = pstHalInfo->stFieldAddr[4].u32PhyAddr_C;
    VPSS_NEXT3CRADDR = pstHalInfo->stFieldAddr[4].u32PhyAddr_Cr;
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,NEXT2_FIELD,HI_FALSE);
    }
    VPSS_REG_SetDeiAddr(u32AppVir,NEXT2_FIELD,VPSS_NEXT3YADDR,VPSS_NEXT3CADDR, VPSS_NEXT3CRADDR);
    VPSS_REG_SetDeiStride(u32AppVir,NEXT2_FIELD,
                            pstHalInfo->stFieldAddr[4].u32Stride_Y,
                            pstHalInfo->stFieldAddr[4].u32Stride_C);

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetFieldNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_U32 u32AppPhy)
{
	HI_BOOL bPreZme = HI_FALSE;

    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
    VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);

    #if 1
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(u32AppVir, &(pstHalInfo->stRwzbInfo));
    #endif

    /* 输入源信息 */
    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
	VPSS_REG_SetPixBitW(u32AppVir, pstHalInfo->stInInfo.enBitWidth);
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
    }

    VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);


    pstCur = &pstHalInfo->stInInfo.stAddr;
	#if 0
    if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP
        || pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstHalInfo->stInInfo.u32Height =
                    pstHalInfo->stInInfo.u32Height/2;
        pstCur->u32Stride_Y = pstCur->u32Stride_Y*2;
        pstCur->u32Stride_C = pstCur->u32Stride_C*2;
        if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            pstCur->u32PhyAddr_Y = pstCur->u32PhyAddr_Y + pstCur->u32Stride_Y;
            pstCur->u32PhyAddr_C = pstCur->u32PhyAddr_C+ pstCur->u32Stride_C;
        }
    }

    #endif
    VPSS_REG_SetImgSize(u32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 帧信息配置 */

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C,
                        0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

    if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM
			|| pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP)
	{
		if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
		{
			VPSS_REG_SetPreZme(u32AppVir,VPSS_REG_HD,PREZME_DISABLE,PREZME_VFIELD);
			VPSS_REG_SetPreZme(u32AppVir,VPSS_REG_SD,PREZME_DISABLE,PREZME_VFIELD);
		}
		else
		{
			VPSS_REG_SetPreZme(u32AppVir,VPSS_REG_HD,PREZME_DISABLE,PREZME_2X);
			VPSS_REG_SetPreZme(u32AppVir,VPSS_REG_SD,PREZME_DISABLE,PREZME_2X);
		}
		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height/2;
		bPreZme = HI_TRUE;
	}

	VPSS_REG_SetGlobalMotionEn(u32AppVir,HI_FALSE);

	VPSS_REG_SetIfmdEn(u32AppVir,HI_FALSE);
    /*db/dr/dnr*/
    VPSS_HAL_SetDnrCfg(enIP,u32AppVir,pstHalInfo);

    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy,pstHalInfo);

	if (bPreZme)
	{
		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height*2;
	}
		
	//t00273561
	VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(u32AppVir,u32AppPhy);

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetFrameNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither

    VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(u32AppVir, CUR_CHANEL, HI_TRUE);
        VPSS_REG_SetCurTunlAddr(u32AppVir, CUR_FRAME, pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(u32AppVir, CUR_CHANEL, HI_FALSE);
    }

#if 0
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(u32AppVir, &(pstHalInfo->stRwzbInfo));
#endif

    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
    VPSS_REG_SetPixBitW(u32AppVir, pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);
	
    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither
    
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
    }

#if 1
    if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP
        || pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstHalInfo->stInInfo.u32Height =
                    pstHalInfo->stInInfo.u32Height/2;
        pstCur->u32Stride_Y = pstCur->u32Stride_Y*2;
        pstCur->u32Stride_C = pstCur->u32Stride_C*2;
        if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            pstCur->u32PhyAddr_Y = pstCur->u32PhyAddr_Y + pstCur->u32Stride_Y;
            pstCur->u32PhyAddr_C = pstCur->u32PhyAddr_C+ pstCur->u32Stride_C;
        }
    }
#endif
	
    VPSS_REG_SetImgSize(u32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C,
                        0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(u32AppVir, CUR_FIELD, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C, 0);
		VPSS_REG_SetImgStrideLB(u32AppVir, CUR_FIELD, pstCur_LB->u32Stride_Y, pstCur_LB->u32Stride_C);
	}

    /*db/dr/dnr*/
    VPSS_HAL_SetDnrCfg(enIP,u32AppVir,pstHalInfo);

	VPSS_REG_SetGlobalMotionEn(u32AppVir,HI_FALSE);
	
	VPSS_REG_SetIfmdEn(u32AppVir,HI_FALSE);
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy, pstHalInfo);
	//t00273561
	VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(u32AppVir,u32AppPhy);
    return HI_SUCCESS;
}



HI_S32 VPSS_HAL_Set5FieldNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir,HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither

	HI_U32 u32CurFieldYaddr;
	HI_U32 u32CurFieldCaddr;
    VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(u32AppVir, NEXT2_CHANEL, HI_TRUE);
        VPSS_REG_SetCurTunlAddr(u32AppVir, NEXT2_CHANEL, pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(u32AppVir, NEXT2_CHANEL, HI_FALSE);
    }

#if 1
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(u32AppVir, &(pstHalInfo->stRwzbInfo));
#endif

    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
    VPSS_REG_SetPixBitW(u32AppVir, pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);

    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
    }

#if 1
    if(pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        if (pstCur->u32Stride_Y >= 256
            && (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
                || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
                || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
                || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP))
        {
			u32CurFieldYaddr = pstCur->u32PhyAddr_Y + 256;
			u32CurFieldCaddr = pstCur->u32PhyAddr_C + 256;
        }
        else
        {
			u32CurFieldYaddr = pstCur->u32PhyAddr_Y + pstCur->u32Stride_Y;
			u32CurFieldCaddr = pstCur->u32PhyAddr_C + pstCur->u32Stride_C;
        }
    }
	else
	{
		u32CurFieldYaddr = pstCur->u32PhyAddr_Y;
		u32CurFieldCaddr = pstCur->u32PhyAddr_C;
	}
#endif
	
    VPSS_REG_SetImgSize(u32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);
	
    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,
                        u32CurFieldYaddr,
                        u32CurFieldCaddr,
                        0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(u32AppVir, CUR_FIELD, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C, 0);
		VPSS_REG_SetImgStrideLB(u32AppVir, CUR_FIELD, pstCur_LB->u32Stride_Y, pstCur_LB->u32Stride_C);
	}

	/*DEI*/
	VPSS_HAL_SetDeiCfg(enIP, pstHalInfo, u32AppVir, u32AppPhy);

	/*db/dr*/
#if 1
    VPSS_HAL_SetDnrCfg(enIP,u32AppVir,pstHalInfo);
#endif

	VPSS_REG_SetGlobalMotionEn(u32AppVir,HI_TRUE);

	VPSS_REG_SetIfmdEn(u32AppVir,HI_TRUE);

    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy, pstHalInfo);

	//t00273561
    VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(u32AppVir,u32AppPhy);

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_Set3DFrameNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_U32 u32AppPhy)
{
    HI_U32 u32W, u32H;


    (HI_VOID)VPSS_HAL_SetFrameNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);

    /* 根据输入格式设置偏移 */
    switch(pstHalInfo->stInInfo.eFrmType)
    {
        case HI_DRV_FT_SBS:
            u32W = pstHalInfo->stInInfo.u32Width;
            VPSS_REG_SetImgOffset(u32AppVir,CUR_FIELD,u32W,0);

            break;

        case HI_DRV_FT_TAB:
            u32H = pstHalInfo->stInInfo.u32Height;
            VPSS_REG_SetImgOffset(u32AppVir,CUR_FIELD,0,u32H);
            break;

        case HI_DRV_FT_FPK:
            break;
        default:
            VPSS_FATAL("eFrmType %d invaild\n", pstHalInfo->stInInfo.eFrmType);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetUHDNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither

    VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    VPSS_REG_SetCurTunlEn(u32AppVir, CUR_CHANEL, HI_FALSE);

    /* 输入源信息 */
    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
    VPSS_REG_SetPixBitW(u32AppVir, pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);
    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
    }


    pstCur = &pstHalInfo->stInInfo.stAddr;
    VPSS_REG_SetImgSize(u32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C,
                        0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(u32AppVir, CUR_FIELD, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C, 0);
		VPSS_REG_SetImgStrideLB(u32AppVir, CUR_FIELD, pstCur_LB->u32Stride_Y, pstCur_LB->u32Stride_C);
	}

    /*db/dr/dnr*/
    VPSS_HAL_SetDnrCfg(enIP,u32AppVir,pstHalInfo);
	VPSS_REG_SetGlobalMotionEn(u32AppVir,HI_FALSE);
	VPSS_REG_SetIfmdEn(u32AppVir,HI_FALSE);
	
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy, pstHalInfo);
    //t00273561
    VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);

    VPSS_HAL_SetAlgParaAddr(u32AppVir,u32AppPhy);
    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetUHDHighSpeedNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither

    VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    VPSS_REG_SetCurTunlEn(u32AppVir, CUR_CHANEL, HI_FALSE);

    /* 输入源信息 */
    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
    VPSS_REG_SetPixBitW(u32AppVir, pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);

	/*set 4K@P60 Two pix one cycle*/
	VPSS_REG_SetTwoPix(u32AppVir, HI_TRUE);
	
    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
    }


    pstCur = &pstHalInfo->stInInfo.stAddr;
    VPSS_REG_SetImgSize(u32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C,
                        0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(u32AppVir, CUR_FIELD, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C, 0);
		VPSS_REG_SetImgStrideLB(u32AppVir, CUR_FIELD, pstCur_LB->u32Stride_Y, pstCur_LB->u32Stride_C);
	}
	
    /*db/dr/dnr*/
    VPSS_HAL_SetDnrCfg(enIP,u32AppVir,pstHalInfo);
    VPSS_REG_SetGlobalMotionEn(u32AppVir,HI_FALSE);
    VPSS_REG_SetIfmdEn(u32AppVir,HI_FALSE);
	   
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy, pstHalInfo);
    //t00273561
    VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);

    VPSS_HAL_SetAlgParaAddr(u32AppVir,u32AppPhy);
    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetRotateNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_U32 u32AppPhy, HI_BOOL bRotateY, HI_U32 u32PortId)
{
    #if 1
    HI_DRV_VPSS_ROTATION_E enRotation;
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
    VPSS_REG_PORT_E enPort = VPSS_REG_HD;
    VPSS_HAL_FRAME_S *pstOutFrm = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstOutAddr = HI_NULL;
    HI_U32 u32Angle;

    VPSS_REG_ResetAppReg(u32AppVir, NULL);

     /* 输入源信息 */
    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
	VPSS_REG_SetPixBitW(u32AppVir, pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);
    VPSS_REG_SetImgSize(u32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;

    /* 输入帧信息 */
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
    }

    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C,
                        0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

    /* 输出配置 */
    pstOutFrm = &pstHalInfo->astPortInfo[u32PortId].stOutInfo;
    pstOutAddr = &pstOutFrm->stAddr;
    VPSS_REG_SetFrmSize(u32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
    VPSS_REG_SetFrmAddr(u32AppVir,enPort,pstOutAddr->u32PhyAddr_Y, pstOutAddr->u32PhyAddr_C);
    VPSS_REG_SetFrmStride(u32AppVir,enPort,pstOutAddr->u32Stride_Y,pstOutAddr->u32Stride_C);
    VPSS_REG_SetFrmFormat(u32AppVir,enPort,pstOutFrm->enFormat);
	
	/* Set Output BitWidth   */
	VPSS_REG_SetPortPixBitW(u32AppVir,enPort, pstOutFrm->enBitWidth);

    VPSS_REG_EnPort(u32AppVir, enPort, HI_TRUE);

    /* 旋转配置 */
    enRotation = pstHalInfo->astPortInfo[u32PortId].enRotation;
	
    switch(enRotation)
    {
        case HI_DRV_VPSS_ROTATION_90:
            u32Angle = 0x0;
            break;
        case HI_DRV_VPSS_ROTATION_270:
            u32Angle = 0x1;
            break;
        default:
            u32Angle = 0x1;
            VPSS_FATAL("Ro Error  %d\n",u32Angle);
            break;
    }

    if (bRotateY)
    {
        VPSS_REG_SetRotation(u32AppVir,u32Angle,1);
    }
    else
    {
        VPSS_REG_SetRotation(u32AppVir,u32Angle,2);
    }
	//t00273561
    VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);
    #endif
    return HI_SUCCESS;
}

#ifdef ZME_2L_TEST //@f00241306 
HI_S32 VPSS_HAL_SetZME2LNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,HI_U32 u32AppVir, HI_U32 u32AppPhy,  HI_U32 u32PortId)
{
	HI_DRV_VID_FRAME_ADDR_S *pstCur		= HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB	= HI_NULL; //@f00241306 for dither

	HI_BOOL abPortUsed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER] = {HI_FALSE, HI_FALSE, HI_FALSE};
	VPSS_HAL_PORT_INFO_S	*pstHalPort = HI_NULL;
	VPSS_HAL_FRAME_S		*pstOutFrm = HI_NULL;
	VPSS_REG_PORT_E			enPort = VPSS_REG_BUTT;

	pstHalPort = &pstHalInfo->astPortInfo[u32PortId];
	if (pstHalPort->bEnable)
	{
		enPort = VPSS_HAL_AllocPortId(pstHalPort, abPortUsed);
		if (enPort == VPSS_REG_BUTT)
		{
			HI_ASSERT(0);
		}
	}

    pstOutFrm = &pstHalPort->stOutInfo;
			
    VPSS_REG_ResetAppReg(u32AppVir, HI_NULL);

	/* 输入帧信息配置 */
    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
    VPSS_REG_SetPixBitW(u32AppVir, pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);
	
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(u32AppVir, CUR_FIELD, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C, 0);
		VPSS_REG_SetImgStrideLB(u32AppVir, CUR_FIELD, pstCur_LB->u32Stride_Y, pstCur_LB->u32Stride_C);
	}
    
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
    }

    VPSS_REG_SetImgSize(u32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C,
                        0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/* 输出帧信息配置 */
		/*ZME*/
    VPSS_HAL_SetZmeCfg(enIP,u32AppVir, pstHalInfo,enPort,u32PortId);
	VPSS_REG_SetAddr(u32AppVir,REG_VPSS_ZME_ADDR,VPSS_ZME_ADDR_GET(u32AppVir));
	VPSS_REG_SetZmeEn(u32AppVir, enPort, HI_TRUE);

    /* 输出格式 */
    VPSS_REG_SetFrmSize(u32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
    VPSS_REG_SetFrmAddr(u32AppVir,enPort,pstOutFrm->stAddr.u32PhyAddr_Y, pstOutFrm->stAddr.u32PhyAddr_C);
    VPSS_REG_SetFrmStride(u32AppVir,enPort,pstOutFrm->stAddr.u32Stride_Y,pstOutFrm->stAddr.u32Stride_C);
    VPSS_REG_SetFrmFormat(u32AppVir,enPort,pstOutFrm->enFormat);

	/* Set Output BitWidth   */
	VPSS_REG_SetPortPixBitW(u32AppVir,enPort, pstOutFrm->enBitWidth);

	VPSS_REG_EnPort(u32AppVir, enPort, HI_TRUE);
		

    /*db/dr/dnr*/
//    VPSS_HAL_SetDnrCfg(enIP,u32AppVir,pstHalInfo);

//	VPSS_REG_SetGlobalMotionEn(u32AppVir,HI_FALSE);
	
//	VPSS_REG_SetIfmdEn(u32AppVir,HI_FALSE);			
	VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(u32AppVir,u32AppPhy);
    return HI_SUCCESS;
}
#endif
HI_VOID VPSS_HAL_DumpReg(VPSS_IP_E enIP, VPSS_HAL_TASK_NODE_E enTaskNodeId)
{
    HI_U32 i;
    HI_U32 *pu32Reg;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    pstHalCtx = &stHalCtx[enIP];

    pu32Reg = (HI_U32 *)(pstHalCtx->au32AppVir[enTaskNodeId]);

    VPSS_INFO("\n\nPhyAddr %8x   enTaskNodeId = %d\n\n",
        pstHalCtx->au32AppPhy[enTaskNodeId], enTaskNodeId);

    for(i=0; i<(64*4); i++)
    {
        if(i%(64) == 0)
        {
            VPSS_INFO("\nBASE %x", i*4);
        }

        if(i%4 == 0)
        {
            VPSS_INFO("\n%x0:", (i%64)/4);
        }

        VPSS_INFO("%.8x  ", *(pu32Reg+i));
    }

    VPSS_INFO("\n\n");

    for(i=1024; i<1024+64; i++)
    {
        if(i%(64) == 0)
        {
            VPSS_INFO("\nBASE %x", i*4);
        }

        if(i%4 == 0)
        {
            VPSS_INFO("\n%x0:", (i%64)/4);
        }

        VPSS_INFO("%.8x  ", *(pu32Reg+i));
    }

    VPSS_INFO("\n\n");

    for(i=2048; i<2048+64; i++)
    {
        if(i%(64) == 0)
        {
            VPSS_INFO("\nBASE %x", i*4);
        }

        if(i%4 == 0)
        {
            VPSS_INFO("\n%x0:", (i%64)/4);
        }

        VPSS_INFO("%.8x  ", *(pu32Reg+i));
    }

    VPSS_INFO("\n\n");

}

#if 0
/* 以上为内部调用函数 */
/* 以下为外部调用函数 */
#endif

HI_S32 VPSS_HAL_Init(VPSS_IP_E enIP)
{
	HI_U32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];

    if (pstHalCtx->bInit)
    {
        VPSS_INFO("VPSS IP%d, Already Init\n", enIP);
        return HI_SUCCESS;
    }

    /* 申请和分配NODE节点 */
    s32Ret = HI_DRV_MMZ_AllocAndMap("VPSS_RegBuf", HI_NULL,
        VPSS_REG_SIZE*VPSS_HAL_TASK_NODE_BUTT, 0, &pstHalCtx->stRegBuf);
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc VPSS_RegBuf Failed\n");
        return HI_FAILURE;
    }

    for (i=0; i<VPSS_HAL_TASK_NODE_BUTT; i++)
    {
        pstHalCtx->au32AppPhy[i] = pstHalCtx->stRegBuf.u32StartPhyAddr
                                 + VPSS_REG_SIZE*i;

        pstHalCtx->au32AppVir[i] = pstHalCtx->stRegBuf.u32StartVirAddr
                                 + VPSS_REG_SIZE*i;
    }


    /* 映射寄存器地址 */
    pstHalCtx->u32BaseRegVir
        = (HI_U32)IO_ADDRESS(pstHalCtx->u32BaseRegPhy);
    if (0 == pstHalCtx->u32BaseRegVir)
    {
        VPSS_FATAL("io_address VPSS_REG(%#x) Failed\n", pstHalCtx->u32BaseRegPhy);
        HI_DRV_MMZ_UnmapAndRelease(&pstHalCtx->stRegBuf);
        return HI_FAILURE;
    }

    pstHalCtx->bInit = HI_TRUE;

    VPSS_HAL_SetClockEn(enIP, HI_FALSE);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_DelInit(VPSS_IP_E enIP)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];

    if (!pstHalCtx->bInit)
    {
        VPSS_INFO("VPSS IP%d, Already DeInit\n", enIP);
        return HI_SUCCESS;
    }

    VPSS_HAL_SetClockEn(enIP, HI_FALSE);

    HI_ASSERT(pstHalCtx->u32BaseRegVir != 0);
    if (pstHalCtx->u32BaseRegVir != 0)
    {
        pstHalCtx->u32BaseRegVir = 0;
    }

    HI_DRV_MMZ_UnmapAndRelease(&pstHalCtx->stRegBuf);
    memset(&pstHalCtx->stRegBuf, 0, sizeof(MMZ_BUFFER_S));

    pstHalCtx->bInit = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetClockEn(VPSS_IP_E enIP, HI_BOOL bClockEn)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    if(pstHalCtx->bClockEn == bClockEn)
    {
        return HI_SUCCESS;
    }

    if (bClockEn)
    {
        VPSS_REG_SetClockEn(HI_TRUE);

        VPSS_REG_ResetAppReg(pstHalCtx->u32BaseRegVir, HI_NULL);
        VPSS_REG_SetTimeOut(pstHalCtx->u32BaseRegVir, DEF_LOGIC_TIMEOUT);
        VPSS_REG_SetIntMask(pstHalCtx->u32BaseRegVir, 0xfe);
    }
    else
    {
        VPSS_REG_SetClockEn(HI_FALSE);
    }

    pstHalCtx->bClockEn = bClockEn;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_GetClockEn(VPSS_IP_E enIP, HI_BOOL *pbClockEn)
{
    //:TODO:后续加入CRG和时钟控制
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_GetIntState(VPSS_IP_E enIP, HI_U32* pu32IntState)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pu32IntState);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    return VPSS_REG_GetIntState(pstHalCtx->u32BaseRegVir, pu32IntState);
}

HI_S32 VPSS_HAL_ClearIntState(VPSS_IP_E enIP, HI_U32 u32IntState)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    return VPSS_REG_ClearIntState(pstHalCtx->u32BaseRegVir, u32IntState);
}

HI_S32 VPSS_HAL_SetNodeInfo(VPSS_IP_E enIP,
     VPSS_HAL_INFO_S *pstHalInfo,  VPSS_HAL_TASK_NODE_E enTaskNodeId)
{
    HI_U32 u32PortId = 0;
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;
    HI_U32 u32AppVir, u32AppPhy;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);
    VPSS_HAL_CHECK_NODE_ID_VAILD(enTaskNodeId);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    u32AppVir = pstHalCtx->au32AppVir[enTaskNodeId];
    u32AppPhy = pstHalCtx->au32AppPhy[enTaskNodeId];

	//pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_FRAME;

	//VPSS_INFO("pstHalInfo->enNodeType=%d\n",pstHalInfo->enNodeType);
	switch(pstHalInfo->enNodeType)
    {
        case VPSS_HAL_NODE_2D_Field:
            s32Ret = VPSS_HAL_SetFieldNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_2D_FRAME:
            s32Ret = VPSS_HAL_SetFrameNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_2D_5Field:
            s32Ret = VPSS_HAL_Set5FieldNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_3D_FRAME_R:
            s32Ret = VPSS_HAL_Set3DFrameNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_UHD:
        case VPSS_HAL_NODE_UHD_SPLIT_L:
        case VPSS_HAL_NODE_UHD_SPLIT_R:
            s32Ret = VPSS_HAL_SetUHDNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_UHD_HIGH_SPEED:
            s32Ret = VPSS_HAL_SetUHDHighSpeedNode(enIP, pstHalInfo, u32AppVir,u32AppPhy);
            break; 
#ifdef ZME_2L_TEST
        case VPSS_HAL_NODE_ZME_2L:
			u32PortId = enTaskNodeId - VPSS_HAL_TASK_NODE_P0_ZME_L2;
            VPSS_HAL_SetZME2LNode(enIP,pstHalInfo, u32AppVir,u32AppPhy,u32PortId);
            break;
#endif
#if 0
        case VPSS_HAL_NODE_2D_3Field:
            //VPSS_HAL_SetFrameNode(pstHalInfo, u32AppVir);
            break;
        case VPSS_HAL_NODE_PZME:
            //VPSS_HAL_SetFrameNode(pstHalInfo, u32AppVir);
            break;

        case VPSS_HAL_NODE_3DDET:
            //VPSS_HAL_SetFrameNode(pstHalInfo, u32AppVir);
            break;
        case VPSS_HAL_NODE_ZME_2L:
			u32PortId = enTaskNodeId - VPSS_HAL_TASK_NODE_P0_ZME_L2;
            VPSS_HAL_SetZME2LNode(enIP,pstHalInfo, u32AppVir,u32AppPhy,u32PortId);
            break;
        case VPSS_HAL_NODE_ROTATION_Y:
            //VPSS_HAL_SetFrameNode(pstHalInfo, u32AppVir);
            break;
        case VPSS_HAL_NODE_ROTATION_C:
            //VPSS_HAL_SetFrameNode(pstHalInfo, u32AppVir);
            break;
#endif
        case VPSS_HAL_NODE_ROTATION_Y:
            u32PortId = (enTaskNodeId - VPSS_HAL_TASK_NODE_P0_RO_Y) / 2;//:TODO:优化
            s32Ret = VPSS_HAL_SetRotateNode(enIP, pstHalInfo, u32AppVir, u32AppPhy, HI_TRUE,u32PortId);
            break;
        case VPSS_HAL_NODE_ROTATION_C:
            u32PortId = (enTaskNodeId - VPSS_HAL_TASK_NODE_P0_RO_Y) / 2;
            s32Ret = VPSS_HAL_SetRotateNode(enIP, pstHalInfo, u32AppVir, u32AppPhy, HI_FALSE,u32PortId);
            break;
        default:
            VPSS_FATAL("No this Node Type:%d!\n", pstHalInfo->enNodeType);
            return HI_FAILURE;

    }

    (HI_VOID)VPSS_HAL_SetAllAlgCfgAddr(u32AppVir, u32AppPhy);

    //VPSS_FATAL("enTaskNodeId = %d, this Node Type:%d!\n", enTaskNodeId, pstHalInfo->enNodeType);
    //VPSS_HAL_DumpReg(enIP,enTaskNodeId);

    return s32Ret;
}

HI_S32 VPSS_HAL_StartLogic(VPSS_IP_E enIP,
    HI_BOOL abNodeVaild[VPSS_HAL_TASK_NODE_BUTT])
{
    HI_U32 i = 0;
    VPSS_HAL_TASK_NODE_E enId, enLastVaild = VPSS_HAL_TASK_NODE_BUTT;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(abNodeVaild);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    /* 从后向前链接所有节点 */
    enLastVaild = VPSS_HAL_TASK_NODE_BUTT;
    for(i=0;i<VPSS_HAL_TASK_NODE_BUTT;i++)
    {
        enId = VPSS_HAL_TASK_NODE_BUTT - 1 - i;

        if(HI_TRUE == abNodeVaild[enId])
        {
            if(VPSS_HAL_TASK_NODE_BUTT == enLastVaild)
            {
                VPSS_REG_StartLogic(0, pstHalCtx->au32AppVir[enId]);
            }
            else
            {
                VPSS_REG_StartLogic(pstHalCtx->au32AppPhy[enLastVaild],
                    pstHalCtx->au32AppVir[enId]);
            }

            enLastVaild = enId;
        }
    }

    HI_ASSERT(enLastVaild != VPSS_HAL_TASK_NODE_BUTT);
    if (enLastVaild == VPSS_HAL_TASK_NODE_BUTT)
    {
        VPSS_FATAL("No Node Needs Start\n");
        return HI_FAILURE;
    }
    else
    {
        /* 启动硬件 */
        return VPSS_REG_StartLogic(pstHalCtx->au32AppPhy[enLastVaild],
                            pstHalCtx->u32BaseRegVir);
    }
}




HI_S32 VPSS_HAL_GetSCDInfo(HI_U32 u32AppAddr,HI_S32 s32SCDInfo[32])
{
	//return VPSS_REG_GetSCDInfo(u32AppAddr,s32SCDInfo);
	return HI_SUCCESS;
}


HI_VOID VPSS_HAL_GetDetPixel(HI_U32 u32AppAddr,HI_U32 BlkNum, HI_U8* pstData)
{
    VPSS_REG_GetDetPixel(u32AppAddr,BlkNum,pstData);
}

HI_S32 VPSS_HAL_GetBaseRegAddr(VPSS_IP_E enIP,
                                 HI_U32 *pu32PhyAddr,
                                 HI_U32 *pu32VirAddr)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    pstHalCtx = &stHalCtx[enIP];

    *pu32PhyAddr = pstHalCtx->u32BaseRegPhy;
    *pu32VirAddr = pstHalCtx->u32BaseRegVir;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetAlgParaAddr(HI_U32 u32AppVir,HI_U32 u32AppPhy)
{
	HI_U32 u32Vc1ParaAddr;
	HI_U32 u32ZmeParaAddr;
	HI_U32 u32HspParaAddr;
	HI_U32 u32DbParaAddr;
	HI_U32 u32DrParaAddr;

	u32Vc1ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_VC1_CTRL0)-sizeof(HI_U32);

	VPSS_REG_SetVc1ParaAddr(u32AppVir,u32Vc1ParaAddr);

	u32ZmeParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_VHD0_HSP)-sizeof(HI_U32);
	VPSS_REG_SetZmeParaAddr(u32AppVir,u32ZmeParaAddr);

	u32HspParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_HSPCFG1)-sizeof(HI_U32);
	VPSS_REG_SetHspParaAddr(u32AppVir,u32HspParaAddr);

	u32DrParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DR_CTRL)-sizeof(HI_U32);
	VPSS_REG_SetDrParaAddr(u32AppVir,u32DrParaAddr);

	u32DbParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DB_CTRL)-sizeof(HI_U32);
	VPSS_REG_SetDbParaAddr(u32AppVir,u32DbParaAddr);

	return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif  /* __cplusplus */
