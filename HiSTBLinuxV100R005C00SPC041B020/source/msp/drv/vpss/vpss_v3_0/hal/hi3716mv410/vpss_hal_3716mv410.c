
#include "vpss_hal_3716mv410.h"
#include "linux/kthread.h"
#include "vpss_common.h"

#include <asm/barrier.h>
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

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

    if (HI_FALSE == abPortUsed[VPSS_REG_SD])
    {
         abPortUsed[VPSS_REG_SD] = HI_TRUE;
         return VPSS_REG_SD;
    }
#endif

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
    
	if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_Field)
	{
		pstZmeDrvPara->u32ZmeFrmWIn = pstHalPort->stOutCropRect.s32Width;
		pstZmeDrvPara->u32ZmeFrmHIn = (pstHalPort->stOutCropRect.s32Height/2)&0xfffffffc;
	}
  	else if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI)
  	{
	    pstZmeDrvPara->u32ZmeFrmWIn = pstHalPort->stOutCropRect.s32Width;
	    
	    pstZmeDrvPara->u32ZmeFrmHIn = pstHalPort->stOutCropRect.s32Height*2;

	    VPSS_DBG_INFO("step2 zme in: W H %d %d\n", pstZmeDrvPara->u32ZmeFrmWIn, pstZmeDrvPara->u32ZmeFrmHIn);
  	}
	else
	{
	    pstZmeDrvPara->u32ZmeFrmWIn = pstHalPort->stOutCropRect.s32Width;
	    
	    pstZmeDrvPara->u32ZmeFrmHIn = pstHalPort->stOutCropRect.s32Height;

	    VPSS_DBG_INFO("step1 zme in: W H %d %d\n", pstZmeDrvPara->u32ZmeFrmWIn, pstZmeDrvPara->u32ZmeFrmHIn);
	}


#if 0
    if (pstInInfo->bProgressive == HI_TRUE)
    {
        if (pstInInfo->enFieldMode != HI_DRV_FIELD_ALL)
        {
            pstZmeDrvPara->u32ZmeFrmHIn = pstZmeDrvPara->u32ZmeFrmHIn*2;
        }
    }
#endif
    
	if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE)
  	{
	    pstZmeDrvPara->u32ZmeFrmWOut = pstZmeDrvPara->u32ZmeFrmWIn;	    
	    pstZmeDrvPara->u32ZmeFrmHOut = pstZmeDrvPara->u32ZmeFrmHIn;

		VPSS_DBG_INFO("step1 zme out: W H %d %d\n", pstZmeDrvPara->u32ZmeFrmWOut, pstZmeDrvPara->u32ZmeFrmHOut);
  	}
  	else
  	{      
        pstZmeDrvPara->u32ZmeFrmHOut = pstHalPort->stVideoRect.s32Height;
        pstZmeDrvPara->u32ZmeFrmWOut = pstHalPort->stVideoRect.s32Width;

		if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI)
		{
        	VPSS_DBG_INFO("step2 zme out: W H %d %d\n", pstZmeDrvPara->u32ZmeFrmWOut, pstZmeDrvPara->u32ZmeFrmHOut);
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

	if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI)
	{
		pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height * 2;
		VPSS_DBG_INFO("step2 zme ori: W H %d %d\n", pstZmeDrvPara->stOriRect.s32Width, pstZmeDrvPara->stOriRect.s32Height);
	}
	else //if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE)
	{
		pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height;
		VPSS_DBG_INFO("step1 zme ori: W H %d %d\n", pstZmeDrvPara->stOriRect.s32Width, pstZmeDrvPara->stOriRect.s32Height);
	}


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
        case VPSS_REG_STR:
            enPqPort = HI_PQ_VPSS_PORT2_LAYER_ZME;
            break;
        default:
            break;
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
brief      : CNcomment:  Config Port Compression info
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

    u32CmpRatioY = (enBitWidth == 1) ? 140 : 130;
    u32CmpRatioC = (enBitWidth == 1) ? 140 : 130;
    u32YMaxQp  = (enBitWidth == 1) ? 4 : 2;
    u32CMaxQp  = (enBitWidth == 1) ? 4 : 2;

    if(0 == enBitWidth)
    {
        if(u32CmpRatioY < 135)
        {
            u32YMaxQp = 2;
        }
        else if((u32CmpRatioY >= 135) && (u32CmpRatioY < 160))
        {
            u32YMaxQp = 3;
        }
        else
        {
            u32YMaxQp = 4;
        }

        if(u32CmpRatioC < 135)
        {
            u32CMaxQp = 2;
        }
        else if((u32CmpRatioC >= 135) && (u32CmpRatioC < 160))
        {
            u32CMaxQp = 3;
        }
        else
        {
            u32CMaxQp = 4;
        }

    }
    else
    {
        if(u32CmpRatioY < 135)
        {
            u32YMaxQp = 3;
        }
        else if((u32CmpRatioY >= 135) && (u32CmpRatioY < 160))
        {
            u32YMaxQp = 4;
        }
        else
        {
            u32YMaxQp = 5;
        }

        if(u32CmpRatioC < 135)
        {
            u32CMaxQp = 3;
        }
        else if((u32CmpRatioC >= 135) && (u32CmpRatioC < 160))
        {
            u32CMaxQp = 4;
        }
        else
        {
            u32CMaxQp = 5;
        }
    }

    bIs_lossless = !(pstHalPort->bCmpLoss);//vdp will control

    bCmp_Mode = HI_FALSE;//cmp mode
    u32Dw_Mode = (enBitWidth == 1) ? 0 : 1;

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

    VPSS_REG_SetLcmpEn(u32AppVir, HI_TRUE);

    //:Y
	VPSS_REG_Set_Y_Is_Lossless(u32AppVir,bIs_lossless);
	VPSS_REG_Set_Y_Cmp_Mode(u32AppVir,bCmp_Mode);
	VPSS_REG_Set_Y_Dw_Mode(u32AppVir,u32Dw_Mode);

	VPSS_REG_Set_Y_Frame_Width(u32AppVir, stYcmpData.u32Width);
	VPSS_REG_Set_Y_Frame_Height(u32AppVir, stYcmpData.u32Height);

	VPSS_REG_Set_Y_MbBits(u32AppVir,stYcmpData.u32Cfg0_Mb_bits);
	VPSS_REG_Set_Y_RiBits(u32AppVir, stYcmpData.u32Cfg0_Ri_bits);
	VPSS_REG_Set_Y_MaxRgCompBits(u32AppVir, stYcmpData.u32Cfg0_Max_rg_comp_bits);

	VPSS_REG_Set_Y_MaxQp(u32AppVir, stYcmpData.u32Cfg1_Max_qp);
	VPSS_REG_Set_Y_SadNgain(u32AppVir, stYcmpData.u32Cfg1_Sad_bits_ngain);
	VPSS_REG_Set_Y_RcSmthNgain(u32AppVir, stYcmpData.u32Cfg1_Rc_smth_ngain);

	VPSS_REG_Set_Y_SmthThr(u32AppVir, stYcmpData.u32Cfg2_Smth_thr);
	VPSS_REG_Set_Y_SmthPixNumThr(u32AppVir, stYcmpData.u32Cfg2_Smth_pix_num_thr);
	VPSS_REG_Set_Y_PixDiffThr(u32AppVir, stYcmpData.u32Cfg2_Pix_diff_thr);

	VPSS_REG_Set_Y_AdjSadThr(u32AppVir, stYcmpData.u32Cfg3_Adj_sad_thr);
	VPSS_REG_Set_Y_AdjSadBitThr(u32AppVir, stYcmpData.u32Cfg3_Adj_sad_bit_thr);

	VPSS_REG_Set_Y_QpChg1BitsThr(u32AppVir, stYcmpData.u32Cfg4_Qp_chg1_bits_thr);
	VPSS_REG_Set_Y_QpChg2BitsThr(u32AppVir, stYcmpData.u32Cfg4_Qp_chg2_bits_thr);

	VPSS_REG_Set_Y_SmthQp0(u32AppVir, stYcmpData.u32Cfg5_Smth_qp0);
	VPSS_REG_Set_Y_SmthQp1(u32AppVir, stYcmpData.u32Cfg5_Smth_qp1);
	VPSS_REG_Set_Y_SmthLftBThr0(u32AppVir, stYcmpData.u32Cfg5_Smth_lftbits_thr0);
	VPSS_REG_Set_Y_SmthLftBThr1(u32AppVir, stYcmpData.u32Cfg5_Smth_lftbits_thr1);

	//:C
	VPSS_REG_Set_C_Is_Lossless(u32AppVir,bIs_lossless);
	VPSS_REG_Set_C_Cmp_Mode(u32AppVir,bCmp_Mode);
	VPSS_REG_Set_C_Dw_Mode(u32AppVir,u32Dw_Mode);

	VPSS_REG_Set_C_Frame_Width(u32AppVir, stCcmpData.u32Width);
	VPSS_REG_Set_C_Frame_Height(u32AppVir, stCcmpData.u32Height);

	VPSS_REG_Set_C_MbBits(u32AppVir,stCcmpData.u32Cfg0_Mb_bits);
	VPSS_REG_Set_C_RiBits(u32AppVir, stCcmpData.u32Cfg0_Ri_bits);
	VPSS_REG_Set_C_MaxRgCompBits(u32AppVir, stCcmpData.u32Cfg0_Max_rg_comp_bits);

	VPSS_REG_Set_C_Max_Qp(u32AppVir, stCcmpData.u32Cfg1_Max_qp);
	VPSS_REG_Set_C_SadNgain(u32AppVir, stCcmpData.u32Cfg1_Sad_bits_ngain);
	VPSS_REG_Set_C_RcSmthNgain(u32AppVir, stCcmpData.u32Cfg1_Rc_smth_ngain);

	VPSS_REG_Set_C_SmthThr(u32AppVir, stCcmpData.u32Cfg2_Smth_thr);
	VPSS_REG_Set_C_SmthPixNumThr(u32AppVir, stCcmpData.u32Cfg2_Smth_pix_num_thr);
	VPSS_REG_Set_C_PixDiffThr(u32AppVir, stCcmpData.u32Cfg2_Pix_diff_thr);

	VPSS_REG_Set_C_AdjSadThr(u32AppVir, stCcmpData.u32Cfg3_Adj_sad_thr);
	VPSS_REG_Set_C_AdjSadBitThr(u32AppVir, stCcmpData.u32Cfg3_Adj_sad_bit_thr);

	VPSS_REG_Set_C_QpChg1BitsThr(u32AppVir, stCcmpData.u32Cfg4_Qp_chg1_bits_thr);
	VPSS_REG_Set_C_QpChg2BitsThr(u32AppVir, stCcmpData.u32Cfg4_Qp_chg2_bits_thr);

	VPSS_REG_Set_C_SmthQp0(u32AppVir, stCcmpData.u32Cfg5_Smth_qp0);
	VPSS_REG_Set_C_SmthQp1(u32AppVir, stCcmpData.u32Cfg5_Smth_qp1);
	VPSS_REG_Set_C_SmthLftBThr0(u32AppVir, stCcmpData.u32Cfg5_Smth_lftbits_thr0);
	VPSS_REG_Set_C_SmthLftBThr1(u32AppVir, stCcmpData.u32Cfg5_Smth_lftbits_thr1);

    return HI_SUCCESS;
}
HI_S32 VPSS_HAL_GetInCropPortID(VPSS_IP_E enIP,
		HI_U32 u32AppVir, 
		VPSS_HAL_INFO_S *pstHalInfo,
		HI_U32 *pu32ProcessID,
		HI_U32 *pu32AvailablePortCnt)
{
    HI_U32 u32Count;
    HI_U32 u32ProcessPortCnt = 0;
	HI_BOOL bProcessPort[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
	HI_RECT_S stInRect[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
	HI_RECT_S *pstOriRect;
	HI_RECT_S *pstCmpRect;


	*pu32ProcessID = 0xffffffff;
	*pu32AvailablePortCnt = DEF_VPSS_HAL_PORT_NUM;

    for (u32Count = 0; 
			u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;
			u32Count ++)
    {
        VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;

        pstHalPort = &pstHalInfo->astPortInfo[u32Count];

        if (pstHalPort->bEnable && pstHalPort->bConfig == HI_FALSE)
        {
			stInRect[u32Count] = pstHalPort->stInCropRect;
			bProcessPort[u32Count] = HI_TRUE;
			u32ProcessPortCnt ++;
		}
		else
		{
			memset(&stInRect[u32Count],0,sizeof(HI_RECT_S));
			bProcessPort[u32Count] = HI_FALSE;
		}
	}

/*
单路播放 CROP该路
两路同源 一样/不一样
两路同源+虚拟屏幕 判断前两路
*/
	switch(u32ProcessPortCnt)
	{
		case 1:
			for (u32Count = 0; 
					u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;
					u32Count ++)
			{
				if (bProcessPort[u32Count] == HI_TRUE)
				{
					*pu32ProcessID = u32Count;
					*pu32AvailablePortCnt = DEF_VPSS_HAL_PORT_NUM;
					break;	
				}
			}
			break;
		case 2:
		case 3:
			pstOriRect = HI_NULL;
			pstCmpRect = HI_NULL;
			for (u32Count = 0; 
					u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;
					u32Count ++)
			{
				if (bProcessPort[u32Count] == HI_TRUE)
				{
					if (pstOriRect == HI_NULL)	
					{
						pstOriRect = &stInRect[u32Count];
						*pu32ProcessID = u32Count;
					}
					else if (pstCmpRect == HI_NULL)
					{
						pstCmpRect = &stInRect[u32Count];
						break;
					}
					else
					{
						VPSS_ERROR("Invalid branch\n");
					}
				}
			}

			if (pstOriRect->s32X == pstCmpRect->s32X 
					&& pstOriRect->s32Y == pstCmpRect->s32Y
					&& pstOriRect->s32Width == pstCmpRect->s32Width
					&& pstOriRect->s32Height == pstCmpRect->s32Height)
			{
				*pu32AvailablePortCnt = DEF_VPSS_HAL_PORT_NUM;
			}
			else
			{
				*pu32AvailablePortCnt = 1;
				VPSS_ERROR("Can't support different crop config\n");
			}
			break;
		default:
			VPSS_ERROR("Invalid u32ProcessPortCnt %d\n",u32ProcessPortCnt);
			break;
	}

	if (*pu32ProcessID == 0xffffffff)
	{
		VPSS_ERROR("Can't get Crop process port ID\n");
		return HI_FAILURE;	
	}
	else
	{
		return HI_SUCCESS;	
	}
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
	HI_U32 VPSS_VHD0_LCMP_ADDR;

    HI_BOOL abPortUsed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER]
        = {HI_FALSE, HI_FALSE, HI_FALSE};
    
    /* 先默认关闭HD硬件输出通道 */
    VPSS_REG_EnPort(u32AppVir, VPSS_REG_HD, HI_FALSE);
#if 0
	VPSS_HAL_GetInCropPortID(enIP,
		u32AppVir, 
		pstHalInfo,
		&u32ProcessPortCnt,
		&u32PortAvailable);

	VPSS_HAL_SetCropCfg(enIP,
			u32AppVir, 
            pstHalInfo,
            u32ProcessPortCnt);
#endif
    for (u32Count = 0; (u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER) && (u32PortAvailable > 0); u32Count ++)
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
            VPSS_REG_SetPortMirrorEn(u32AppVir, enPort,pstHalPort->bNeedMirror);  
            VPSS_REG_SetPortFlipEn(u32AppVir, enPort,pstHalPort->bNeedFlip);    
            
            /* UV反转 */
			VPSS_REG_SetVHD0_UVINVERT(u32AppVir, pstOutFrm->bUVInver);

            /* PreZme */
            //VPSS_REG_SetFrmPreZmeEn(u32AppVir, enPort, HI_FALSE, HI_FALSE);
            
			/*crop*/
			{
				HI_U32 u32CropX;	
				HI_U32 u32CropY;	
				HI_U32 u32CropWidth;	
				HI_U32 u32CropHeight;	

				u32CropX = pstHalPort->stOutCropRect.s32X;
				u32CropY = pstHalPort->stOutCropRect.s32Y;
				u32CropWidth = pstHalPort->stOutCropRect.s32Width;
				u32CropHeight = pstHalPort->stOutCropRect.s32Height;
				if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_Field)
				{
					u32CropHeight = u32CropHeight/2;
				}
				else if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI)
				{
				    u32CropHeight = u32CropHeight*2;			    

				    VPSS_DBG_INFO("step2 Crop  X %d Y %d W %d H %d\n",u32CropX,u32CropY,u32CropWidth,u32CropHeight);
				}

				VPSS_REG_SetPortCropPos(u32AppVir,enPort,
						u32CropY,
						u32CropX);
				VPSS_REG_SetPortCropSize(u32AppVir,enPort,
						u32CropHeight,
						u32CropWidth); 
			}

			VPSS_REG_SetPortCropEn(u32AppVir,enPort,HI_TRUE);

            /*ZME*/
            VPSS_HAL_SetZmeCfg(enIP,u32AppVir, pstHalInfo,enPort,u32Count);
         
            /* LBX */
            VPSS_REG_SetLBABg(u32AppVir, enPort, 0x108080, 0x7f);
            VPSS_REG_SetLBAVidPos(u32AppVir, enPort, 
                                    (HI_U32)pstHalPort->stVideoRect.s32X,
                                    (HI_U32)pstHalPort->stVideoRect.s32Y,
                                    pstHalPort->stVideoRect.s32Height,
                                    pstHalPort->stVideoRect.s32Width);
            VPSS_REG_SetLBADispPos(u32AppVir, enPort, 0, 0, 
                                   pstOutFrm->u32Height, pstOutFrm->u32Width);
            VPSS_REG_SetLBAEn(u32AppVir, enPort, HI_TRUE);

			VPSS_DBG_INFO("step2: lbx disp W H %d %d\n", pstOutFrm->u32Width, pstOutFrm->u32Height);
			VPSS_DBG_INFO("step2: lbx valid X Y H W %d %d %d %d\n",(HI_U32)pstHalPort->stVideoRect.s32X,
									(HI_U32)pstHalPort->stVideoRect.s32Y,
									pstHalPort->stVideoRect.s32Height,
									pstHalPort->stVideoRect.s32Width);

            /* 输出格式 */
            VPSS_REG_SetFrmSize(u32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
            VPSS_REG_SetFrmAddr(u32AppVir,enPort,pstOutAddr->u32PhyAddr_Y, pstOutAddr->u32PhyAddr_C);
            VPSS_REG_SetFrmStride(u32AppVir,enPort,pstOutAddr->u32Stride_Y,pstOutAddr->u32Stride_C);
            VPSS_REG_SetFrmFormat(u32AppVir,enPort,pstOutFrm->enFormat);
			VPSS_DBG_INFO("step2: set W H YS CS %d %d %d %d\n", pstOutFrm->u32Width,pstOutFrm->u32Height, 
													pstOutAddr->u32Stride_Y, pstOutAddr->u32Stride_C);

            if(enPort == VPSS_REG_HD &&  VPSS_HAL_CheckNeedCmp(pstOutFrm))
            {
                //压缩配置
				VPSS_VHD0_LCMP_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_VHD0_LCMP_YINFO) - sizeof(HI_U32);
				VPSS_REG_SetAddr(u32AppVir, REG_VPSS_VHD0_LCMP_ADDR, VPSS_VHD0_LCMP_ADDR);

                VPSS_HAL_SetPortCmpCfg(u32AppVir, enPort, pstHalPort);
                //VPSS_REG_SetPortMirrorEn(u32AppVir, enPort,HI_FALSE);//压缩输出可以支持水平镜像               
            }
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
#if 0
			VPSS_INFO("adddr X %d Y %d\n",pstRwzbInfo->u32Addr[u32Count][0],
					pstRwzbInfo->u32Addr[u32Count][1]);
#endif
		}

	}

	VPSS_REG_SetDetEn(u32AppAddr,pstRwzbInfo->u32EnRwzb);
	VPSS_REG_SetDetMode(u32AppAddr,pstRwzbInfo->u32Mode);

	return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetDeTileDeiCfg(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir,HI_U32 u32AppPhy)

{
    HI_U32  VPSS_DEI_ADDR;

    HI_DRV_VID_FRAME_ADDR_S *pstRef  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt1 = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt2 = HI_NULL;
	HI_U32 u32Head = 0;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

    u32Head = pstHalInfo->pstRefList->u32RefListHead;

    //需要考虑前4场时，每个缓冲BUFFER的有效性
	if (!pstHalInfo->pstRefList->abRefNodeValid[0])
	{
		VPSS_FATAL("!!pstHalInfo->pstRefList->abRefNodeValid[0]!");
	}

	if (!pstHalInfo->pstRefList->abRefNodeValid[1])
	{
		pstRef	= &pstHalInfo->pstRefList->stRefListAddr[0];
		pstCur	= &pstHalInfo->pstRefList->stRefListAddr[0];
		pstNxt1 = &pstHalInfo->pstRefList->stRefListAddr[0];
		pstNxt2 = &pstHalInfo->pstRefList->stRefListAddr[0];
	}
	else if (!pstHalInfo->pstRefList->abRefNodeValid[2])
	{
		pstRef	= &pstHalInfo->pstRefList->stRefListAddr[0];
		pstCur	= &pstHalInfo->pstRefList->stRefListAddr[0];
		pstNxt1 = &pstHalInfo->pstRefList->stRefListAddr[0];
		pstNxt2 = &pstHalInfo->pstRefList->stRefListAddr[1];
	}
	else if (!pstHalInfo->pstRefList->abRefNodeValid[3])
	{
		pstRef	= &pstHalInfo->pstRefList->stRefListAddr[0];
		pstCur	= &pstHalInfo->pstRefList->stRefListAddr[0];
		pstNxt1 = &pstHalInfo->pstRefList->stRefListAddr[1];
		pstNxt2 = &pstHalInfo->pstRefList->stRefListAddr[2];
	}
	else
	{
		pstRef	= &pstHalInfo->pstRefList->stRefListAddr[(u32Head+1)%DEF_VPSS_HAL_REF_LIST_NUM];
		pstCur	= &pstHalInfo->pstRefList->stRefListAddr[(u32Head+2)%DEF_VPSS_HAL_REF_LIST_NUM];
		pstNxt1 = &pstHalInfo->pstRefList->stRefListAddr[(u32Head+3)%DEF_VPSS_HAL_REF_LIST_NUM];
		pstNxt2 = &pstHalInfo->pstRefList->stRefListAddr[(u32Head)];
	}

	VPSS_DBG_INFO("Ref Cur Nx1 Nx2 %x %x %x %x\n", pstRef->u32PhyAddr_Y, 
								pstCur->u32PhyAddr_Y, pstNxt1->u32PhyAddr_Y, pstNxt2->u32PhyAddr_Y);

    /*dei*/
    VPSS_REG_SetImgReadMod(u32AppVir, HI_TRUE);
    VPSS_REG_EnDei(u32AppVir, HI_TRUE);
    VPSS_REG_SetDeiTopFirst(u32AppVir, pstHalInfo->stInInfo.bTopFirst);
#if 1
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
#endif
    
    VPSS_REG_SetModeEn(u32AppVir,REG_DIE_MODE_CHROME,HI_TRUE);
    VPSS_REG_SetModeEn(u32AppVir,REG_DIE_MODE_LUMA,HI_TRUE);

    VPSS_DEI_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DIECTRL)-sizeof(HI_U32);
    VPSS_REG_SetDeiParaAddr(u32AppVir,VPSS_DEI_ADDR);

    VPSS_REG_SetMode(u32AppVir, REG_DIE_MODE_ALL, 1);//0为5场模式、1为4场模式、2为3场模式

    /*********************************ref**************************************/
    VPSS_REG_SetImgTile(u32AppVir, LAST_FIELD, HI_FALSE);
    VPSS_REG_SetImgAddr(u32AppVir, LAST_FIELD, pstRef->u32PhyAddr_Y,
						pstRef->u32PhyAddr_C, pstRef->u32PhyAddr_Cr);
    VPSS_REG_SetImgStride(u32AppVir,LAST_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);
	
    /************************* Cur **********************************/
    VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
	VPSS_REG_SetRefSize(u32AppVir,pstHalInfo->stInRefInfo[1].u32Width,pstHalInfo->stInRefInfo[1].u32Height*2);
    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,pstCur->u32PhyAddr_Y,pstCur->u32PhyAddr_C,0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);
	
    /*******************************next1**************************************/
    VPSS_REG_SetImgTile(u32AppVir, NEXT1_FIELD, HI_FALSE);
    VPSS_REG_SetImgAddr(u32AppVir, NEXT1_FIELD, pstNxt1->u32PhyAddr_Y,
						pstNxt1->u32PhyAddr_C, pstNxt1->u32PhyAddr_Cr);
    VPSS_REG_SetImgStride(u32AppVir, NEXT1_FIELD, pstNxt1->u32Stride_Y, pstNxt1->u32Stride_C);

    /*****************************next2****************************************/
    VPSS_REG_SetImgTile(u32AppVir, NEXT2_FIELD,HI_FALSE);

    VPSS_REG_SetImgAddr(u32AppVir, NEXT2_FIELD, pstNxt2->u32PhyAddr_Y,
								pstNxt2->u32PhyAddr_C, pstNxt2->u32PhyAddr_Cr);
    VPSS_REG_SetImgStride(u32AppVir, NEXT2_FIELD, pstNxt2->u32Stride_Y, pstNxt2->u32Stride_C);	

    return HI_SUCCESS;
}
HI_S32 VPSS_HAL_SetDeiCfg(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir,HI_U32 u32AppPhy)

{
    HI_U32  VPSS_DEI_ADDR;

    HI_DRV_VID_FRAME_ADDR_S *pstRef  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt1 = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt2 = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

	/* 帧信息配置 */
    pstRef  = &pstHalInfo->stInRefInfo[0].stAddr;
    pstCur  = &pstHalInfo->stInRefInfo[1].stAddr;
    pstNxt1 = &pstHalInfo->stInRefInfo[2].stAddr;
    pstNxt2 = &pstHalInfo->stInInfo.stAddr;

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
    
    VPSS_REG_SetModeEn(u32AppVir,REG_DIE_MODE_CHROME,HI_TRUE);
    VPSS_REG_SetModeEn(u32AppVir,REG_DIE_MODE_LUMA,HI_TRUE);

    VPSS_DEI_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DIECTRL)-sizeof(HI_U32);
    VPSS_REG_SetDeiParaAddr(u32AppVir,VPSS_DEI_ADDR);

    VPSS_REG_SetMode(u32AppVir, REG_DIE_MODE_ALL, 1);//0为5场模式、1为4场模式、2为3场模式

    /*********************************ref**************************************/
    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir, LAST_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir, LAST_FIELD, HI_FALSE);
    }	
    VPSS_REG_SetImgAddr(u32AppVir, LAST_FIELD, pstRef->u32PhyAddr_Y,
						pstRef->u32PhyAddr_C, pstRef->u32PhyAddr_Cr);
    VPSS_REG_SetImgStride(u32AppVir,LAST_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);
	
    /************************* Cur **********************************/
    if (pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir,CUR_FIELD,HI_FALSE);
    }
	VPSS_REG_SetRefSize(u32AppVir,pstHalInfo->stInRefInfo[1].u32Width,pstHalInfo->stInRefInfo[1].u32Height);
    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,pstCur->u32PhyAddr_Y,pstCur->u32PhyAddr_C,0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);
	
    /*******************************next1**************************************/
    if (pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir, NEXT1_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir, NEXT1_FIELD, HI_FALSE);
    }
    VPSS_REG_SetImgAddr(u32AppVir, NEXT1_FIELD, pstNxt1->u32PhyAddr_Y,
						pstNxt1->u32PhyAddr_C, pstNxt1->u32PhyAddr_Cr);
    VPSS_REG_SetImgStride(u32AppVir, NEXT1_FIELD, pstNxt1->u32Stride_Y, pstNxt1->u32Stride_C);

    /*****************************next2****************************************/
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir, NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir, NEXT2_FIELD,HI_FALSE);
    }

    VPSS_REG_SetImgAddr(u32AppVir, NEXT2_FIELD, pstNxt2->u32PhyAddr_Y,
								pstNxt2->u32PhyAddr_C, pstNxt2->u32PhyAddr_Cr);
    VPSS_REG_SetImgStride(u32AppVir, NEXT2_FIELD, pstNxt2->u32Stride_Y, pstNxt2->u32Stride_C);	

    return HI_SUCCESS;
}



HI_S32 VPSS_HAL_SetFieldNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_U32 u32AppPhy)
{
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
	HI_U32 VPSS_DBM_ADDR;
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstRef = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstRfr = HI_NULL;
    VPSS_NRMADCFG_S *pstNrMad = HI_NULL;
	HI_BOOL bPreZme = HI_FALSE;

    VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);

    /* 需要强制设置的模式和开关 */
    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);

	VPSS_REG_SetRfrEn(u32AppVir, HI_FALSE);/* 默认把回写打开 */
    VPSS_REG_SetTnrEn(u32AppVir, HI_FALSE);/* 默认都把TNR打开 */

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(u32AppVir, HI_TRUE);
        VPSS_REG_SetCurTunlAddr(u32AppVir, CUR_FRAME, pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(u32AppVir, HI_FALSE);
    }
    
    #if 1
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(u32AppVir, &(pstHalInfo->stRwzbInfo));
    #endif
    
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

    /* 输入帧信息 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    VPSS_REG_SetImgSize(u32AppVir, 
				pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
				pstHalInfo->stInInfo.bProgressive);
    VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,pstCur->u32PhyAddr_Y,pstCur->u32PhyAddr_C,0);
    VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);
	
#if 0
	VPSS_ERROR("prog %d field %d addr %d stride %d\n",
			pstHalInfo->stInInfo.bProgressive,
			pstHalInfo->stInInfo.enFieldMode,
			pstCur->u32PhyAddr_Y,
			pstCur->u32Stride_Y
			);
#endif
    /* 参考帧信息 */
	pstRef = &pstHalInfo->stInRefInfo[0].stAddr;
	VPSS_REG_SetRefSize(u32AppVir,pstHalInfo->stInRefInfo[0].u32Width,pstHalInfo->stInRefInfo[0].u32Height);
    VPSS_REG_SetImgAddr(u32AppVir,LAST_FIELD,pstRef->u32PhyAddr_Y,pstRef->u32PhyAddr_C,0);
    VPSS_REG_SetImgStride(u32AppVir,LAST_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);

    /* 回写帧信息 */
	if (pstHalInfo->stNrInfo.bNrEn)
	{
		pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
		VPSS_REG_SetRfrAddr(u32AppVir, pstRfr->u32PhyAddr_Y, pstRfr->u32PhyAddr_C);
		VPSS_REG_SetRfrStride(u32AppVir, pstRfr->u32Stride_Y, pstRfr->u32Stride_C);

		/* 运动信息 */
		pstNrMad = &pstHalInfo->stNrInfo.stNrMadCfg;
		VPSS_REG_SetMadRaddr(u32AppVir, pstNrMad->u32Tnrmad_raddr);
		VPSS_REG_SetMadWaddr(u32AppVir, pstNrMad->u32Tnrmad_waddr);
		VPSS_REG_SetMadStride(u32AppVir, pstNrMad->u32madstride);
	}
    /*DBM*/
	VPSS_DBM_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DB_CTRL) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_DBM_ADDR, VPSS_DBM_ADDR);
	VPSS_REG_SetDbmEn(u32AppVir,HI_FALSE);

	VPSS_REG_SetIglbEn(u32AppVir,HI_FALSE);
	VPSS_REG_SetIfmdEn(u32AppVir,HI_FALSE);

	if (pstHalInfo->stRwzbInfo.u32IsRwzb)
	{
		VPSS_REG_SetDbmEn(u32AppVir,HI_FALSE);
	}

	if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP)
	{
		VPSS_REG_SetPreZme(u32AppVir,PREZME_DISABLE,PREZME_2X);
		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height/2;
		bPreZme = HI_TRUE;
	}
	else
	{
		VPSS_REG_SetPreZme(u32AppVir,PREZME_DISABLE,PREZME_VFIELD);
		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height/2;
		bPreZme = HI_TRUE;
	}
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy, pstHalInfo);

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_ADDR, VPSS_Tnr_ADDR);

	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_CLUT_ADDR, VPSS_Tnr_CLUT_ADDR);

	VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);
	if (bPreZme == HI_TRUE)
	{
		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height*2;
	}
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetFrameNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_U32 u32AppPhy)
{
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
	HI_U32 VPSS_DBM_ADDR;
	HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstRef = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstRfr = HI_NULL;
	VPSS_NRMADCFG_S *pstNrMad = HI_NULL;

	VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);

	/* 需要强制设置的模式和开关 */
	VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);

	VPSS_REG_SetRfrEn(u32AppVir, pstHalInfo->bEnableTNR);/* 默认把回写打开 */

	if (pstHalInfo->stInInfo.u32Width > 1920 
			|| pstHalInfo->stInInfo.u32Height > 1088)
	{
		VPSS_REG_SetTnrEn(u32AppVir, HI_FALSE);
	}
	else
	{
		VPSS_REG_SetTnrEn(u32AppVir, pstHalInfo->bEnableTNR);
	}

	/*tunnel*/
	if (pstHalInfo->stInInfo.u32TunnelAddr)
	{
		VPSS_REG_SetCurTunlEn(u32AppVir, HI_TRUE);
		VPSS_REG_SetCurTunlAddr(u32AppVir, CUR_FRAME, pstHalInfo->stInInfo.u32TunnelAddr);
	}
	else
	{
		VPSS_REG_SetCurTunlEn(u32AppVir, HI_FALSE);
	}

#if 1
	/*rwzb*/
	VPSS_HAL_SetRwzbCfg(u32AppVir, &(pstHalInfo->stRwzbInfo));
#endif

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

	if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_YUYV
			|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_YVYU
			|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_UYVY)
	{
		VPSS_REG_SetCurPackage(u32AppVir,HI_TRUE);
	}
	else
	{
		VPSS_REG_SetCurPackage(u32AppVir,HI_FALSE);
	}

	/* 输入帧信息 */
	pstCur = &pstHalInfo->stInInfo.stAddr;
	VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);
	VPSS_REG_SetImgSize(u32AppVir, 
			pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
			pstHalInfo->stInInfo.bProgressive);
	VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,pstCur->u32PhyAddr_Y,pstCur->u32PhyAddr_C,0);
	VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/* 参考帧信息 */
	pstRef = &pstHalInfo->stInRefInfo[0].stAddr;
	VPSS_REG_SetRefSize(u32AppVir,pstHalInfo->stInRefInfo[0].u32Width,pstHalInfo->stInRefInfo[0].u32Height);
	VPSS_REG_SetImgAddr(u32AppVir,LAST_FIELD,pstRef->u32PhyAddr_Y,pstRef->u32PhyAddr_C,0);
	VPSS_REG_SetImgStride(u32AppVir,LAST_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);

    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(u32AppVir, LAST_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(u32AppVir, LAST_FIELD, HI_FALSE);
    }	

	if (pstHalInfo->stNrInfo.bNrEn)
	{
		/* 回写帧信息 */
		pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
		VPSS_REG_SetRfrAddr(u32AppVir, pstRfr->u32PhyAddr_Y, pstRfr->u32PhyAddr_C);
		VPSS_REG_SetRfrStride(u32AppVir, pstRfr->u32Stride_Y, pstRfr->u32Stride_C);

		/* 运动信息 */
		pstNrMad = &pstHalInfo->stNrInfo.stNrMadCfg;
		VPSS_REG_SetMadRaddr(u32AppVir, pstNrMad->u32Tnrmad_raddr);
		VPSS_REG_SetMadWaddr(u32AppVir, pstNrMad->u32Tnrmad_waddr);
		VPSS_REG_SetMadStride(u32AppVir, pstNrMad->u32madstride);
	}

	/*DBM*/
	VPSS_DBM_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DB_CTRL) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_DBM_ADDR, VPSS_DBM_ADDR);
	VPSS_REG_SetDbmEn(u32AppVir,HI_TRUE);

	VPSS_REG_SetIglbEn(u32AppVir,HI_FALSE);
	VPSS_REG_SetIfmdEn(u32AppVir,HI_FALSE);

	if (pstHalInfo->stRwzbInfo.u32IsRwzb)
	{
		VPSS_REG_SetDbmEn(u32AppVir,HI_FALSE);
	}

	/* 输出Port信息 */
	VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy, pstHalInfo);

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_ADDR, VPSS_Tnr_ADDR);

	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_CLUT_ADDR, VPSS_Tnr_CLUT_ADDR);

	VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);

	return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set5FieldNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo, 
    HI_U32 u32AppVir,HI_U32 u32AppPhy)
{
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
	HI_U32 VPSS_DBM_ADDR;
		
    HI_DRV_VID_FRAME_ADDR_S *pstRfr  = HI_NULL;
//    HI_DRV_VID_FRAME_ADDR_S *pstPr1  = HI_NULL;
//    HI_DRV_VID_FRAME_ADDR_S *pstPr2  = HI_NULL;

    VPSS_DIESTCFG_S *pstDeiSt = HI_NULL;
    VPSS_NRMADCFG_S *pstNrMad = HI_NULL;
  
    VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);
    
    /* 需要强制设置的模式和开关 */
    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
    
	VPSS_REG_SetRfrEn(u32AppVir, pstHalInfo->bEnableTNR);/* 默认把回写打开 */
    VPSS_REG_SetTnrEn(u32AppVir, pstHalInfo->bEnableTNR);/* 默认都把TNR打开 */

    VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);
    VPSS_REG_SetImgSize(u32AppVir, 
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

	//	  pstPr1 = &pstHalInfo->stCCCLInfo.stInRefInfo[0].stAddr;
	//	  pstPr2 = &pstHalInfo->stCCCLInfo.stInRefInfo[1].stAddr;

    /*DEI*/
    VPSS_HAL_SetDeiCfg(enIP, pstHalInfo, u32AppVir, u32AppPhy);

    /* 回写帧信息 */
    pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
	VPSS_REG_SetRfrAddr(u32AppVir, pstRfr->u32PhyAddr_Y, pstRfr->u32PhyAddr_C);
	VPSS_REG_SetRfrStride(u32AppVir, pstRfr->u32Stride_Y, pstRfr->u32Stride_C);

	/* 运动信息  DEI*/
	pstDeiSt = &pstHalInfo->stDieInfo.stDieStCfg;	
	VPSS_REG_SetStRdAddr(u32AppVir, pstDeiSt->u32PPreAddr);
	VPSS_REG_SetStWrAddr(u32AppVir, pstDeiSt->u32PreAddr);
	VPSS_REG_SetStStride(u32AppVir, pstDeiSt->u32Stride);

	/* 运动信息 TNR*/
    pstNrMad = &pstHalInfo->stNrInfo.stNrMadCfg;
	if (pstHalInfo->stNrInfo.bNrEn)
	{
		VPSS_REG_SetMadRaddr(u32AppVir, pstNrMad->u32Tnrmad_raddr);
		VPSS_REG_SetMadWaddr(u32AppVir, pstNrMad->u32Tnrmad_waddr);
		VPSS_REG_SetMadStride(u32AppVir, pstNrMad->u32madstride);
	}
   
    /*DBM*/
	VPSS_DBM_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DB_CTRL) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_DBM_ADDR, VPSS_DBM_ADDR);
	VPSS_REG_SetDbmEn(u32AppVir,HI_TRUE);


	VPSS_REG_SetIglbEn(u32AppVir,HI_TRUE);
	VPSS_REG_SetIfmdEn(u32AppVir,HI_TRUE);

    /*RWZB*/
	#if 1
    VPSS_HAL_SetRwzbCfg(u32AppVir, &(pstHalInfo->stRwzbInfo));
	#endif

	if (pstHalInfo->stRwzbInfo.u32IsRwzb)
	{
		VPSS_REG_SetOutSel(u32AppVir,REG_DIE_MODE_ALL,HI_TRUE);
		VPSS_REG_SetDbmEn(u32AppVir,HI_FALSE);
	}
                                 
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy, pstHalInfo);

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_ADDR, VPSS_Tnr_ADDR);

	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_CLUT_ADDR, VPSS_Tnr_CLUT_ADDR);

	VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);
	
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

HI_S32 VPSS_HAL_SetRotateNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 u32AppVir, HI_BOOL bRotateY, HI_U32 u32PortId)
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
    //VPSS_REG_SetImgBitWidth(u32AppVir, pstHalInfo->stInInfo.enBitWidth);
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
    #endif
    return HI_SUCCESS;
}

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

HI_S32 VPSS_HAL_RefListDeInit(VPSS_HAL_RefList_S *pstRefList)
{
	HI_S32 s32Idx;	

	if (pstRefList->bRefListValid)
	{
		VPSS_DBG_INFO("VPSS free %x\n", pstRefList->stRefListBuf_0.u32StartPhyAddr);

		(HI_VOID)VPSS_OSAL_FreeMem(&pstRefList->stRefListBuf_0);
		memset(&pstRefList->stRefListBuf_0, 0, sizeof(pstRefList->stRefListBuf_0));

		for (s32Idx = 0; s32Idx < DEF_VPSS_HAL_REF_LIST_NUM; s32Idx++)
		{
			memset(&(pstRefList->stRefListAddr[s32Idx]), 0x0, sizeof(pstRefList->stRefListAddr[s32Idx]));
			pstRefList->abRefNodeValid[s32Idx] = HI_FALSE;
		}
	}

	pstRefList->bRefListValid = HI_FALSE;
	pstRefList->u32RefListHead = 0;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_RefListInit(VPSS_HAL_RefList_S *pstRefList, HI_S32 s32Width, HI_S32 s32Height, HI_DRV_PIX_FORMAT_E 
enFormat, HI_BOOL bSecure)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32StartAddr;
    HI_U32 u32YSize;
    HI_U32 u32Frame1Offset;
    //HI_U32 u32Frame2Offset;
	HI_U32 u32FieldIdx;
	HI_U8  u8SecFlag;

	if (pstRefList->bRefListValid)
	{
		VPSS_DBG_INFO("VPSS_HAL_RefListInit has been called!\n");

		VPSS_HAL_RefListDeInit(pstRefList);
		pstRefList->bRefListValid = HI_FALSE;
	}

	if ((HI_DRV_PIX_FMT_NV12 != enFormat)
			&& (HI_DRV_PIX_FMT_NV21 != enFormat)
			&& (HI_DRV_PIX_FMT_NV12_CMP != enFormat)
			&& (HI_DRV_PIX_FMT_NV21_CMP != enFormat)
			&& (HI_DRV_PIX_FMT_NV12_TILE_CMP != enFormat)
			&& (HI_DRV_PIX_FMT_NV21_TILE_CMP != enFormat))
	{
		VPSS_FATAL("Unsupport pixel format:%d\n", enFormat);
		return HI_FAILURE;
	}

    u8SecFlag = (bSecure) ? VPSS_MEM_FLAG_SECURE : VPSS_MEM_FLAG_NORMAL;
    s32Ret = VPSS_OSAL_AllocateMem(u8SecFlag, s32Width*s32Height * 3 / 2 * DEF_VPSS_HAL_REF_LIST_NUM,
                                                "VPSS_H265RefBuf",
                                                "VPSS",
                                                &pstRefList->stRefListBuf_0);
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc VPSS_H265RefBuf Failed\n");
        return HI_FAILURE;
    }

	VPSS_DBG_INFO("VPSS malloc W H adr %d %d %x\n", s32Width, s32Height, pstRefList->stRefListBuf_0.u32StartPhyAddr);

	for (u32FieldIdx = 0; u32FieldIdx < DEF_VPSS_HAL_REF_LIST_NUM; u32FieldIdx++)
	{
		pstRefList->abRefNodeValid[u32FieldIdx] = HI_FALSE;
	}

    pstRefList->u32RefListHead = 0;

    /*************************************************************************
    ------------ frame0 start ------------
    ytop ytop ytop ...... ytop ytop ytop   <--- stRefListAddr[0].u32PhyAddr_Y
    ybot ybot ybot ...... ybot ybot ybot   <--- stRefListAddr[1].u32PhyAddr_Y
    ytop ytop ytop ...... ytop ytop ytop
    ...
    ------------ Y end ------------
    ctop ctop ctop ...... ctop ctop ctop   <--- stRefListAddr[0].u32PhyAddr_C
    cbot cbot cbot ...... cbot cbot cbot   <--- stRefListAddr[1].u32PhyAddr_C
    ...
    ------------ C end ------------
    ------------ frame0 end ------------


    <<<---------------- u32Frame1Offset

    ------------ frame1 start ------------
    ...
    ------------ frame1 end ------------


    <<<---------------- u32Frame2Offset
    
    ------------ frame2 start ------------
    ...
    ------------ frame2 end ------------
    *************************************************************************/

    u32StartAddr = pstRefList->stRefListBuf_0.u32StartPhyAddr;
    u32YSize = s32Width*s32Height*2;

    pstRefList->stRefListAddr[0].u32PhyAddr_Y = u32StartAddr;
    pstRefList->stRefListAddr[0].u32PhyAddr_C = u32StartAddr + u32YSize;
    pstRefList->stRefListAddr[0].u32Stride_Y = s32Width;
    pstRefList->stRefListAddr[0].u32Stride_C = s32Width;

    pstRefList->stRefListAddr[1].u32PhyAddr_Y = u32StartAddr + s32Width;
    pstRefList->stRefListAddr[1].u32PhyAddr_C = u32StartAddr + u32YSize + s32Width;
    pstRefList->stRefListAddr[1].u32Stride_Y = s32Width;
    pstRefList->stRefListAddr[1].u32Stride_C = s32Width;

	u32Frame1Offset = s32Width*s32Height*3;

    pstRefList->stRefListAddr[2].u32PhyAddr_Y = u32StartAddr + u32Frame1Offset;
    pstRefList->stRefListAddr[2].u32PhyAddr_C = u32StartAddr + u32Frame1Offset + u32YSize;
    pstRefList->stRefListAddr[2].u32Stride_Y = s32Width;
    pstRefList->stRefListAddr[2].u32Stride_C = s32Width;

    pstRefList->stRefListAddr[3].u32PhyAddr_Y = u32StartAddr + u32Frame1Offset + s32Width;
    pstRefList->stRefListAddr[3].u32PhyAddr_C = u32StartAddr + u32Frame1Offset + u32YSize + s32Width;
    pstRefList->stRefListAddr[3].u32Stride_Y = s32Width;
    pstRefList->stRefListAddr[3].u32Stride_C = s32Width;

	pstRefList->bRefListValid = HI_TRUE;

    return HI_SUCCESS;
}



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
        
        pstHalCtx->au32AppVir[i] = (HI_U32)(pstHalCtx->stRegBuf.pu8StartVirAddr
                                 + VPSS_REG_SIZE*i);
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
        VPSS_REG_SetClockEn(enIP, HI_TRUE);

        VPSS_REG_ResetAppReg(pstHalCtx->u32BaseRegVir, HI_NULL);
        VPSS_REG_SetTimeOut(pstHalCtx->u32BaseRegVir, DEF_LOGIC_TIMEOUT);
        VPSS_REG_SetIntMask(pstHalCtx->u32BaseRegVir, 0xfe);
    }
    else
    {
        VPSS_REG_SetClockEn(enIP, HI_FALSE);
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
HI_S32 VPSS_HAL_SetDetileNode_STEP1(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32  u32AppVir,HI_U32 u32AppPhy)
{
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
	HI_U32 VPSS_DBM_ADDR;
	HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
    VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

	VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);

	VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
	VPSS_REG_SetRfrEn(u32AppVir, HI_FALSE);
	VPSS_REG_SetTnrEn(u32AppVir, HI_FALSE);
	VPSS_REG_SetCurTunlEn(u32AppVir, HI_FALSE);

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

	if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_YUYV
			|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_YVYU
			|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_UYVY)
	{
		VPSS_REG_SetCurPackage(u32AppVir,HI_TRUE);
	}
	else
	{
		VPSS_REG_SetCurPackage(u32AppVir,HI_FALSE);
	}

	/* 输入帧信息 */
	pstCur = &pstHalInfo->stInInfo.stAddr;
	VPSS_REG_SetImgFormat(u32AppVir, pstHalInfo->stInInfo.enFormat);
	VPSS_REG_SetImgSize(u32AppVir, 
			pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
			pstHalInfo->stInInfo.bProgressive);


	VPSS_DBG_INFO("STEP1: pstHalInfo->stInInfo.bProgressive:%d\n", pstHalInfo->stInInfo.bProgressive);
	VPSS_DBG_INFO("STEP1: StridY StridC %d %d\n", pstCur->u32Stride_Y, pstCur->u32Stride_C);


	VPSS_REG_SetImgAddr(u32AppVir,CUR_FIELD,pstCur->u32PhyAddr_Y,pstCur->u32PhyAddr_C,0);
	VPSS_REG_SetImgStride(u32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	
	/*DBM*/
	VPSS_DBM_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DB_CTRL) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_DBM_ADDR, VPSS_DBM_ADDR);
	VPSS_REG_SetDbmEn(u32AppVir,HI_FALSE);

	VPSS_REG_SetIglbEn(u32AppVir,HI_FALSE);
	VPSS_REG_SetIfmdEn(u32AppVir,HI_FALSE);

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_ADDR, VPSS_Tnr_ADDR);

	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_CLUT_ADDR, VPSS_Tnr_CLUT_ADDR);

	VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);


    pstHalPort = &pstHalInfo->astPortInfo[0];

    if (pstHalPort->bEnable && pstHalPort->bConfig == HI_FALSE)
    {
        VPSS_REG_PORT_E enPort = VPSS_REG_BUTT;
        VPSS_HAL_FRAME_S *pstOutFrm = HI_NULL;
        HI_DRV_VID_FRAME_ADDR_S *pstOutAddr = HI_NULL;
        
        enPort = VPSS_REG_HD;
        
        pstOutFrm = &pstHalInfo->stInInfo;
        pstOutAddr = &(pstHalInfo->pstRefList->stRefListAddr[pstHalInfo->pstRefList->u32RefListHead]);
		pstHalInfo->pstRefList->abRefNodeValid[pstHalInfo->pstRefList->u32RefListHead] = HI_TRUE;

        if (0)
        {
			HI_U8 chFile[20] = "vpss_detile.yuv";
			HI_DRV_VIDEO_FRAME_S stFrame;
			stFrame.u32Width = pstHalInfo->stInInfo.u32Width;
			stFrame.u32Height = pstHalInfo->stInInfo.u32Height;
			stFrame.ePixFormat = HI_DRV_PIX_FMT_NV21;
			stFrame.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
			stFrame.stBufAddr[0] = *pstOutAddr;
			stFrame.stBufAddr[0].u32Stride_Y =1920*2;
			stFrame.stBufAddr[0].u32Stride_C =1920*2;
			VPSS_DBG();
			VPSS_OSAL_WRITEYUV(&stFrame, chFile);
			VPSS_DBG();
        }
        /* Flip&Mirro */
        VPSS_REG_SetPortMirrorEn(u32AppVir, enPort,HI_FALSE);  
        VPSS_REG_SetPortFlipEn(u32AppVir, enPort,HI_FALSE);    
        
        /* UV反转 */
		VPSS_REG_SetVHD0_UVINVERT(u32AppVir, HI_FALSE);
        
		VPSS_REG_SetPortCropEn(u32AppVir,enPort,HI_FALSE);

        /*ZME*/
        VPSS_HAL_SetZmeCfg(enIP,u32AppVir, pstHalInfo,enPort,0);
     
        /* LBX */
        VPSS_REG_SetLBABg(u32AppVir, enPort, 0x108080, 0x7f);
        VPSS_REG_SetLBAVidPos(u32AppVir, enPort, 
                                (HI_U32)pstHalPort->stVideoRect.s32X,
                                (HI_U32)pstHalPort->stVideoRect.s32Y,
                                pstHalPort->stVideoRect.s32Height,
                                pstHalPort->stVideoRect.s32Width);
#if 0                                
		VPSS_DBG_INFO("step1: lbx valid X Y H W %d %d %d %d\n",(HI_U32)pstHalPort->stVideoRect.s32X,
                                (HI_U32)pstHalPort->stVideoRect.s32Y,
                                pstHalPort->stVideoRect.s32Height,
                                pstHalPort->stVideoRect.s32Width);
#endif
        VPSS_REG_SetLBADispPos(u32AppVir, enPort, 0, 0, 
                               pstOutFrm->u32Height, pstOutFrm->u32Width);
        VPSS_REG_SetLBAEn(u32AppVir, enPort, HI_FALSE);
                   
        /* 输出格式 */
        VPSS_REG_SetFrmSize(u32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
        VPSS_REG_SetFrmAddr(u32AppVir,enPort,pstOutAddr->u32PhyAddr_Y, pstOutAddr->u32PhyAddr_C);
        VPSS_REG_SetFrmStride(u32AppVir,enPort,pstOutAddr->u32Stride_Y*2,pstOutAddr->u32Stride_C*2);
        VPSS_REG_SetFrmFormat(u32AppVir,enPort,HI_DRV_PIX_FMT_NV21);

		VPSS_DBG_INFO("step1: set H W YS CS %d %d %d %d\n", pstOutFrm->u32Height, pstOutFrm->u32Width,
												pstOutAddr->u32Stride_Y*2, pstOutAddr->u32Stride_C*2);


        VPSS_REG_EnPort(u32AppVir, enPort, HI_TRUE);
    }    

    
	return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetFirst3FieldNode_STEP2(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32  u32AppVir,HI_U32 u32AppPhy)
{
#if 1
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
	HI_U32 VPSS_DBM_ADDR;

    VPSS_DIESTCFG_S *pstDeiSt = HI_NULL;
    VPSS_HAL_CHECK_IP_VAILD(enIP);

  
    VPSS_REG_ResetAppReg(u32AppVir, pstHalInfo->pstPqCfg);
    
    /* 需要强制设置的模式和开关 */
    VPSS_REG_SetInCropEn(u32AppVir, HI_FALSE);
    
	VPSS_REG_SetRfrEn(u32AppVir, HI_FALSE);
    VPSS_REG_SetTnrEn(u32AppVir, HI_FALSE);

    VPSS_REG_SetImgFormat(u32AppVir, HI_DRV_PIX_FMT_NV21);
    VPSS_REG_SetImgSize(u32AppVir, 
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height*2,
        pstHalInfo->stInInfo.bProgressive);

    /*DEI*/
    VPSS_HAL_SetDeTileDeiCfg(enIP, pstHalInfo, u32AppVir, u32AppPhy);

	/* 运动信息  DEI*/
	pstDeiSt = &pstHalInfo->stDieInfo.stDieStCfg;	
	VPSS_REG_SetStRdAddr(u32AppVir, pstDeiSt->u32PPreAddr);
	VPSS_REG_SetStWrAddr(u32AppVir, pstDeiSt->u32PreAddr);
	VPSS_REG_SetStStride(u32AppVir, pstDeiSt->u32Stride);
   
    /*DBM*/
	VPSS_DBM_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DB_CTRL) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_DBM_ADDR, VPSS_DBM_ADDR);
	VPSS_REG_SetDbmEn(u32AppVir,HI_TRUE);

	VPSS_REG_SetIglbEn(u32AppVir,HI_TRUE);
	VPSS_REG_SetIfmdEn(u32AppVir,HI_TRUE);

    /*RWZB*/
	#if 1
    VPSS_HAL_SetRwzbCfg(u32AppVir, &(pstHalInfo->stRwzbInfo));
	#endif

	if (pstHalInfo->stRwzbInfo.u32IsRwzb)
	{
		VPSS_REG_SetOutSel(u32AppVir,REG_DIE_MODE_ALL,HI_TRUE);
		VPSS_REG_SetDbmEn(u32AppVir,HI_FALSE);
	}
                                 
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, u32AppVir, u32AppPhy, pstHalInfo);

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_ADDR, VPSS_Tnr_ADDR);

	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetAddr(u32AppVir, REG_VPSS_TNR_CLUT_ADDR, VPSS_Tnr_CLUT_ADDR);

	VPSS_REG_SetSttWrAddr(u32AppVir, pstHalInfo->u32stt_w_phy_addr);
#endif

    
    pstHalInfo->pstRefList->u32RefListHead = (pstHalInfo->pstRefList->u32RefListHead+1)%DEF_VPSS_HAL_REF_LIST_NUM;
   
    return HI_SUCCESS;
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

    switch(pstHalInfo->enNodeType)
    {

        case VPSS_HAL_NODE_2D_FRAME:
            s32Ret = VPSS_HAL_SetFrameNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_2D_5Field:
            s32Ret = VPSS_HAL_Set5FieldNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_2D_Field:
            s32Ret = VPSS_HAL_SetFieldNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_3D_FRAME_R:
            s32Ret = VPSS_HAL_Set3DFrameNode(enIP, pstHalInfo, u32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE:
			VPSS_HAL_SetDetileNode_STEP1(enIP, pstHalInfo,  u32AppVir, u32AppPhy);
			break;
		case VPSS_HAL_NODE_2D_H265_STEP2_DEI:
			VPSS_HAL_SetFirst3FieldNode_STEP2(enIP, pstHalInfo,  u32AppVir, u32AppPhy);
			break;
        case VPSS_HAL_NODE_PZME:
            //VPSS_HAL_SetFrameNode(pstHalInfo, u32AppVir);
            break;
        
        case VPSS_HAL_NODE_3DDET:
            //VPSS_HAL_SetFrameNode(pstHalInfo, u32AppVir);
            break;

        case VPSS_HAL_NODE_ROTATION_Y:
            u32PortId = (enTaskNodeId - VPSS_HAL_TASK_NODE_P0_RO_Y) / 2;//:TODO:优化
            s32Ret = VPSS_HAL_SetRotateNode(enIP, pstHalInfo, u32AppVir, HI_TRUE,u32PortId);
            break;
        case VPSS_HAL_NODE_ROTATION_C:
            u32PortId = (enTaskNodeId - VPSS_HAL_TASK_NODE_P0_RO_Y) / 2;            
            s32Ret = VPSS_HAL_SetRotateNode(enIP, pstHalInfo, u32AppVir, HI_FALSE,u32PortId);
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

HI_S32 VPSS_HAL_SetCropCfg(VPSS_IP_E enIP,
							HI_U32 u32AppVir, 
                            VPSS_HAL_INFO_S *pstHalInfo,
                            HI_U32 u32PortCnt)
{
	VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;
	VPSS_HAL_FRAME_S *pstInInfo; 
	HI_RECT_S *pstInCropRect;
	HI_U32 u32CropX;
	HI_U32 u32CropY;
	HI_U32 u32CropWidth;
	HI_U32 u32CropHeight;

	pstHalPort = &pstHalInfo->astPortInfo[u32PortCnt];

	pstInInfo = &(pstHalInfo->stInInfo);

	pstInCropRect = &(pstHalPort->stInCropRect);

	if (pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD) 
	{
		u32CropX = (pstInCropRect->s32X/2)&0xfffffffe;
		u32CropY = pstInCropRect->s32Y; 
		u32CropWidth = (pstInCropRect->s32Width/2)&0xfffffffe;
		u32CropHeight = pstInCropRect->s32Height;
	}
	else
	{
		if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_Field)
		{
			u32CropX = pstInCropRect->s32X;
			u32CropY = pstInCropRect->s32Y;
			u32CropWidth = pstInCropRect->s32Width;
			u32CropHeight = pstInCropRect->s32Height;

//			pstInCropRect->s32Y = (pstInCropRect->s32Y/2)&0xfffffffc;
//			pstInCropRect->s32Height = (pstInCropRect->s32Height/2)&0xfffffffc;
		}
		else
		{
			u32CropX = pstInCropRect->s32X;
			u32CropY = pstInCropRect->s32Y;
			u32CropWidth = pstInCropRect->s32Width;
			u32CropHeight = pstInCropRect->s32Height;
		}
	}

	if (pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_L
		|| pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_R) 
	{
		VPSS_REG_SetInCropEn(u32AppVir,HI_FALSE);
	}
	else
	{
		VPSS_REG_SetInCropPos(u32AppVir,u32CropY,u32CropX);
		VPSS_REG_SetInCropSize(u32AppVir,u32CropHeight,u32CropWidth);

		VPSS_REG_SetInCropEn(u32AppVir,HI_TRUE);
		VPSS_REG_SetInCropMode(u32AppVir,HI_FALSE);
	}

	return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif  /* __cplusplus */
