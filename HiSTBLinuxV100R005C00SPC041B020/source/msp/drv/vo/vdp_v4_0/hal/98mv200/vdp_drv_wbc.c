#include "hi_type.h"
#include "vdp_drv_comm.h"
#include "vdp_drv_wbc.h"
#include "vdp_drv_vid.h"
//#include "vdp_hal_hdr.h"
#include "vdp_drv_pq_csc.h"
#include "vdp_drv_pq_zme.h"
#include "vdp_drv_vid.h"
#include "vdp_hal_mmu.h"

#if (VDP_CBB_FPGA &&  !defined(__DISP_PLATFORM_BOOT__))

#include <linux/string.h>
#include <linux/kernel.h>
#endif

extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;

HI_S32 VDP_DRV_GetWbcZmeStrategy(VDP_DRV_WBC_ZME_INFO_S *pstZmeInfo,
	                                            HI_PQ_ZME_COEF_RATIO_E  *penHiPqZmeCoefRatio,
	                                            HI_PQ_ZME_COEF_TYPE_E enPqZmeCoefRatio,
	                                            VDP_PROC_FMT_E  enInZmeFmt,
	                                            VDP_PROC_FMT_E  enOutZmeFmt,
	                                            HI_U32 u32Ration)
{

	HI_S32 s32RatioH=100;
	HI_S32 s32RatioVL=100;
	HI_S32 s32RatioVC=100;

	HI_S32 s32Ratio1d0=1*100;
	HI_S32 s32Ratio0d75=3*100/4;
	HI_S32 s32Ratio0d5=1*100/2;
	HI_S32 s32Ratio0d25=1*100/4;
	HI_S32 s32Ratio0d33=1*100/3;
	HI_S32 s32Ratio0d2=1*100/5;



    if (enInZmeFmt == VDP_PROC_FMT_SP_420)
    {
        if(enOutZmeFmt == VDP_PROC_FMT_SP_420)
        {
			s32RatioH	=(pstZmeInfo->stOutRect.u32Wth*100/u32Ration)/pstZmeInfo->stInRect.u32Wth;
			s32RatioVL	=pstZmeInfo->stOutRect.u32Hgt*100/pstZmeInfo->stInRect.u32Hgt;
			s32RatioVC	=s32RatioVL;
		}
	    else if (enOutZmeFmt == VDP_PROC_FMT_SP_422)
	    {
			s32RatioH	=(pstZmeInfo->stOutRect.u32Wth*100/u32Ration)/pstZmeInfo->stInRect.u32Wth;
			s32RatioVL	=pstZmeInfo->stOutRect.u32Hgt*100/pstZmeInfo->stInRect.u32Hgt;
			s32RatioVC	=s32RatioVL*2;
		}
	}
	else if (enInZmeFmt == VDP_PROC_FMT_SP_422)
	{
        if(enOutZmeFmt == VDP_PROC_FMT_SP_420)
        {
			s32RatioH	=(pstZmeInfo->stOutRect.u32Wth*100/u32Ration)/pstZmeInfo->stInRect.u32Wth;
			s32RatioVL	=pstZmeInfo->stOutRect.u32Hgt*100/pstZmeInfo->stInRect.u32Hgt;
			s32RatioVC	=s32RatioVL/2;

		}
	    else if (enOutZmeFmt == VDP_PROC_FMT_SP_422)
	    {
			s32RatioH	=(pstZmeInfo->stOutRect.u32Wth*100/u32Ration)/pstZmeInfo->stInRect.u32Wth;
			s32RatioVL	=pstZmeInfo->stOutRect.u32Hgt*100/pstZmeInfo->stInRect.u32Hgt;
			s32RatioVC	=s32RatioVL;
		}
	}


	
	switch(enPqZmeCoefRatio)
	{
	case HI_PQ_ZME_COEF_TYPE_LH:
	case HI_PQ_ZME_COEF_TYPE_CH:
		if(s32RatioH == s32Ratio1d0)
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		}
		else if((s32RatioH >= s32Ratio0d75)&&(s32RatioH < s32Ratio1d0))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_075;
		}
		else if((s32RatioH >= s32Ratio0d5)&&(s32RatioH < s32Ratio0d75))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_05;
		}
		else if((s32RatioH >=s32Ratio0d33)&&(s32RatioH <s32Ratio0d5))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_033;
		}
		else if((s32RatioH >= s32Ratio0d25)&&(s32RatioH < s32Ratio0d33))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_025;
		}
		else if((s32RatioH >= s32Ratio0d2)&&(s32RatioH < s32Ratio0d25))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_0;
		}
		else
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		}
		break;
	case HI_PQ_ZME_COEF_TYPE_LV:
		if(s32RatioVL == s32Ratio1d0)
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		}
		else if((s32RatioVL >= s32Ratio0d75)&&(s32RatioVL <s32Ratio1d0))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_075;
		}
		else if((s32RatioVL >= s32Ratio0d5)&&(s32RatioVL < s32Ratio0d75))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_05;
		}
		else if((s32RatioVL >= s32Ratio0d33)&&(s32RatioVL < s32Ratio0d5))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_033;
		}
		else if((s32RatioVL >= s32Ratio0d25)&&(s32RatioVL < s32Ratio0d33))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_025;
		}
		else if((s32RatioVL >= s32Ratio0d2)&&(s32RatioVL < s32Ratio0d25))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_0;
		}
		else
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		}
		break;
	case HI_PQ_ZME_COEF_TYPE_CV:
		if(s32RatioVC == s32Ratio1d0)
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		}
		else if((s32RatioVC >= s32Ratio0d75)&&(s32RatioVC < s32Ratio1d0))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_075;
		}
		else if((s32RatioVC >= s32Ratio0d5)&&(s32RatioVC < s32Ratio0d75))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_05;
		}
		else if((s32RatioVC >= s32Ratio0d33)&&(s32RatioVC < s32Ratio0d5))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_033;
		}
		else if((s32RatioVC >=s32Ratio0d25)&&(s32RatioVC < s32Ratio0d33))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_025;
		}
		else if((s32RatioVC >= s32Ratio0d2)&&(s32RatioVC < s32Ratio0d25))
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_0;
		}
		else
		{
			*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		}
		break;
	default:
		*penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		break;
	}
	return  HI_SUCCESS;


}

HI_S32 VDP_DRV_SetWbcZme(HI_U32 u32LayerId, VDP_DRV_WBC_ZME_INFO_S *pstZmeInfo)
{
	HI_U32 u32Ration   = 1;
	
#ifdef VDP_PQ_STRATEGY
	VDP_PROC_FMT_E  enInZmeFmt  = VDP_PROC_FMT_SP_422;
	VDP_PROC_FMT_E  enOutZmeFmt = VDP_PROC_FMT_SP_422;
#endif

	HI_PQ_ZME_COEF_RATIO_E enRationHL =HI_PQ_ZME_COEF_1;
	HI_PQ_ZME_COEF_RATIO_E enRationHC =HI_PQ_ZME_COEF_1;
	HI_PQ_ZME_COEF_RATIO_E enRationVL =HI_PQ_ZME_COEF_1;
	HI_PQ_ZME_COEF_RATIO_E enRationVC =HI_PQ_ZME_COEF_1;
	//HI_PQ_ZME_COEF_RATIO_E   pstZmeRtlPara = {0};
	VDP_VZME_RTL_PARA_S      pstZmeRtlPara= {0};
	HI_U32 u32InFmtTmp =0;
	//--------------------------------------------
	//WBC zme setting
	//--------------------------------------------
	VDP_WBC_SetZmeEnable		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_HOR, HI_TRUE);
	VDP_WBC_SetZmeEnable		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VER, HI_TRUE);


	VDP_WBC_SetFirEnable		   (u32LayerId, VDP_ZME_MODE_HORL, pstZmeInfo->pstZmeDrvPara.bZmeFirHL);
	VDP_WBC_SetFirEnable		   (u32LayerId, VDP_ZME_MODE_HORC, pstZmeInfo->pstZmeDrvPara.bZmeFirHC);
	
	VDP_WBC_SetFirEnable		   (u32LayerId, VDP_ZME_MODE_VERL, pstZmeInfo->pstZmeDrvPara.bZmeFirVL);
	VDP_WBC_SetFirEnable		   (u32LayerId, VDP_ZME_MODE_VERC, pstZmeInfo->pstZmeDrvPara.bZmeFirVC);
	
	VDP_WBC_SetMidEnable		   (u32LayerId, VDP_ZME_MODE_HOR,  pstZmeInfo->pstZmeDrvPara.bZmeMedH);
	VDP_WBC_SetMidEnable		   (u32LayerId, VDP_ZME_MODE_VER,  pstZmeInfo->pstZmeDrvPara.bZmeMedV);

	if(pstZmeInfo->stInRect.u32Wth >= W_2160P)
	{
		VDP_WBC_SetHorZmeEnable 	   ((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
		VDP_WBC_SetHorZmeMidEnable	   ((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
		VDP_WBC_SetHorZmeFirEnable	   ((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);		
		u32Ration = 2;
	}
	else
	{
		VDP_WBC_SetHorZmeEnable 	   ((VDP_LAYER_WBC_E)u32LayerId, HI_FALSE);
	}


	if((pstZmeInfo->enWbcVpConnPoint == VDP_LAYER_CONN_VIDBZME) && ((VDP_LAYER_WBC_E)u32LayerId == VDP_LAYER_WBC_VP0))
	{
		if(pstZmeInfo->enInFmt ==	VDP_VID_IFMT_SP_420)
		{
			VDP_WBC_SetZmeInFmt 		   (u32LayerId, VDP_PROC_FMT_SP_420);
			#ifdef VDP_PQ_STRATEGY
			enInZmeFmt = VDP_PROC_FMT_SP_420;
			#endif
		}
		else
		{
			VDP_WBC_SetZmeInFmt 		   (u32LayerId, VDP_PROC_FMT_SP_422);
			#ifdef VDP_PQ_STRATEGY
			enInZmeFmt = VDP_PROC_FMT_SP_422;
			#endif
			
		}
	}
	else if(u32LayerId == VDP_LAYER_WBC_VP0)
	{
		VDP_WBC_SetZmeInFmt 		   (u32LayerId, VDP_PROC_FMT_SP_422);

		#ifdef VDP_PQ_STRATEGY
	    enInZmeFmt = VDP_PROC_FMT_SP_422;
		#endif
	}

	if(u32LayerId == VDP_LAYER_WBC_HD0)
	{
		VDP_WBC_SetZmeInFmt 		   (u32LayerId, VDP_PROC_FMT_SP_422);
		#ifdef VDP_PQ_STRATEGY
		enInZmeFmt = VDP_PROC_FMT_SP_422;
		#endif
	}
	
	if(pstZmeInfo->enOutFmt == VDP_WBC_OFMT_SP420)
	{
		VDP_WBC_SetZmeOutFmt		   (u32LayerId, VDP_PROC_FMT_SP_420);
		#ifdef VDP_PQ_STRATEGY
	    enOutZmeFmt = VDP_PROC_FMT_SP_420;
		#endif
	}
	else
	{
		VDP_WBC_SetZmeOutFmt		   (u32LayerId, VDP_PROC_FMT_SP_422);
		#ifdef VDP_PQ_STRATEGY
	    enOutZmeFmt = VDP_PROC_FMT_SP_422;
		#endif
	}


    pstZmeInfo->pstZmeDrvPara.u32ZmeFrmWIn	= (pstZmeInfo->stInRect.u32Wth/u32Ration);
    pstZmeInfo->pstZmeDrvPara.u32ZmeFrmWOut = pstZmeInfo->stOutRect.u32Wth;
    pstZmeInfo->pstZmeDrvPara.u32ZmeFrmHIn	= pstZmeInfo->stInRect.u32Hgt;
    pstZmeInfo->pstZmeDrvPara.u32ZmeFrmHOut = pstZmeInfo->stOutRect.u32Hgt;
	
    if((pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_420) & (pstZmeInfo->enWbcVpConnPoint == VDP_LAYER_CONN_VIDBZME))
    {
    	u32InFmtTmp = 1;
    }
	
	pstZmeInfo->pstZmeDrvPara.u8ZmeYCFmtIn	= u32InFmtTmp;
	pstZmeInfo->pstZmeDrvPara.u8ZmeYCFmtOut = 0;
	
	VdpZmeComnSet(&pstZmeInfo->pstZmeDrvPara, &pstZmeRtlPara);
	
	if (HI_TRUE ==  pstZmeInfo->pstZmeDrvPara.bZmeDefault)
	{
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_HOR, 0);	
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VERL, 0);
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VERC, 0);	
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VERBL, 0);
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VERBC, 0);		
	}
	else
	{
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_HOR, 0);	
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VERL, pstZmeRtlPara.s32ZmeOffsetVL);
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VERC, pstZmeRtlPara.s32ZmeOffsetVC);	
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VERBL, pstZmeRtlPara.s32ZmeOffsetVLBtm);
		VDP_WBC_SetZmePhase 		   ((VDP_LAYER_WBC_E)u32LayerId, VDP_ZME_MODE_VERBC, pstZmeRtlPara.s32ZmeOffsetVCBtm);		
	}
	
	VDP_WBC_SetZmeHorRatio		   ((VDP_LAYER_WBC_E)u32LayerId, ((pstZmeInfo->stInRect.u32Wth/u32Ration)*ZME_HPREC)/pstZmeInfo->stOutRect.u32Wth);
	VDP_WBC_SetZmeVerRatio		   ((VDP_LAYER_WBC_E)u32LayerId, (pstZmeInfo->stInRect.u32Hgt*ZME_VPREC)/pstZmeInfo->stOutRect.u32Hgt);

#ifndef VDP_PQ_STRATEGY
    {
         static HI_PQ_ZME_COEF_RATIO_E enHiPqZmeCoefRatio=HI_PQ_ZME_COEF_1;
         enRationHL  =enHiPqZmeCoefRatio;
         enRationHC  =enHiPqZmeCoefRatio;
         enRationVL  =enHiPqZmeCoefRatio;
         enRationVC  =enHiPqZmeCoefRatio;
    }
#else

	VDP_DRV_GetWbcZmeStrategy(pstZmeInfo,&enRationHL,HI_PQ_ZME_COEF_TYPE_LH,enInZmeFmt,enOutZmeFmt,u32Ration);
	VDP_DRV_GetWbcZmeStrategy(pstZmeInfo,&enRationHC,HI_PQ_ZME_COEF_TYPE_CH,enInZmeFmt,enOutZmeFmt,u32Ration);
	VDP_DRV_GetWbcZmeStrategy(pstZmeInfo,&enRationVL,HI_PQ_ZME_COEF_TYPE_LV,enInZmeFmt,enOutZmeFmt,u32Ration);
	VDP_DRV_GetWbcZmeStrategy(pstZmeInfo,&enRationVC,HI_PQ_ZME_COEF_TYPE_CV,enInZmeFmt,enOutZmeFmt,u32Ration);
#endif

	GetVdpZmeCoef(enRationHL,HI_PQ_ZME_COEF_TYPE_LH,HI_PQ_ZME_TAP_8T32P, (HI_S16*)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));

	DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_8LH,HI_PQ_ZME_COEF_TYPE_LH,(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_WBC_ZMEH]),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));	

	

	
	GetVdpZmeCoef(enRationHC,HI_PQ_ZME_COEF_TYPE_CH,HI_PQ_ZME_TAP_4T32P, (HI_S16*)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
	DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_4CH,HI_PQ_ZME_COEF_TYPE_CH,(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_WBC_ZMEH]+256),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));	

	
	GetVdpZmeCoef(enRationVL,HI_PQ_ZME_COEF_TYPE_LV,HI_PQ_ZME_TAP_4T32P, (HI_S16*)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
	DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_4CV,HI_PQ_ZME_COEF_TYPE_LV,(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_WBC_ZMEV]),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));	
	GetVdpZmeCoef(enRationVC,HI_PQ_ZME_COEF_TYPE_CV,HI_PQ_ZME_TAP_4T32P, (HI_S16*)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
	DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_4CV,HI_PQ_ZME_COEF_TYPE_CV,(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_WBC_ZMEV]+256),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));	

	VDP_WBC_SetZmeCoefAddr((VDP_LAYER_WBC_E)u32LayerId, VDP_WBC_PARA_ZME_HOR , (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_WBC_ZMEH]));
	VDP_WBC_SetZmeCoefAddr((VDP_LAYER_WBC_E)u32LayerId, VDP_WBC_PARA_ZME_VER , (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_WBC_ZMEV]));
	VDP_WBC_SetParaUpd((VDP_LAYER_WBC_E)u32LayerId,VDP_WBC_PARA_ZME_HOR);
	VDP_WBC_SetParaUpd((VDP_LAYER_WBC_E)u32LayerId,VDP_WBC_PARA_ZME_VER);

	return  HI_SUCCESS;
	

}

HI_S32 VDP_DRV_SetWbcDitherMode(HI_U32 u32LayerId, VDP_WBC_DITHER_MODE_E enDitherMode)
{
	if(enDitherMode == VDP_WBC_DITHER_MODE_8BIT_OUT)
	{
		VDP_WBC_SetDitherEnable((VDP_LAYER_WBC_E)u32LayerId,HI_TRUE);
		VDP_WBC_SetDitherRoundEnable((VDP_LAYER_WBC_E)u32LayerId,HI_TRUE);//must to 1 自动比对必须为1；
		VDP_WBC_SetDitherMode((VDP_LAYER_WBC_E)u32LayerId,(VDP_DITHER_E)1);//must to 1 自动比对必须为1；
	}
	else
	{
		;
	}

    return	HI_SUCCESS;
}

HI_S32 VDP_DRV_SetWbcLayer(HI_U32 u32LayerId, VDP_LAYER_WBC_INFO_S *pstInfo)
{

	HI_S32 s32CdsCoef[4] = {103,335,103,-29};
	VDP_DRV_WBC_ZME_INFO_S  stZmeInfo ;    
    VDP_RECT_S st_crop = pstInfo->stInRect;

    memset(&stZmeInfo,0x0,sizeof(VDP_DRV_WBC_ZME_INFO_S));

	VDP_WBC_SetEnable			   ((VDP_LAYER_WBC_E)u32LayerId, pstInfo->bWbcLayerEn);	
	if (pstInfo->bWbcLayerEn != HI_TRUE)
	{
		VDP_WBC_SetRegUp((VDP_LAYER_WBC_E)u32LayerId);
		return HI_SUCCESS;
	}
	
	//--------------------------------------------
	//3D to 2D setting
	//--------------------------------------------
	if(pstInfo->enDispMode == VDP_DISP_MODE_SBS)
	{
		st_crop.u32X	/= 2;
		st_crop.u32Wth	/= 2;
	}
	else if(pstInfo->enDispMode == VDP_DISP_MODE_TAB)
	{
		st_crop.u32Y	/= 2;
		st_crop.u32Hgt	/= 2;
	}
	else if(pstInfo->enDispMode == VDP_DISP_MODE_LBL)
	{
		st_crop.u32Y	/= 2;
		st_crop.u32Hgt	/= 2;
	}


	if(pstInfo->bP2iEn == HI_TRUE)
	{
		VDP_WBC_SetProToInterEnable((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
	}
	else
	{
		VDP_WBC_SetProToInterEnable((VDP_LAYER_WBC_E)u32LayerId, HI_FALSE);
	}


	//VDP_WBC_SetCscEnable((VDP_LAYER_WBC_E)u32LayerId, pstInfo->bCscEnable);
	//VDP_WBC_SetCscMode ((VDP_LAYER_WBC_E)u32LayerId, pstInfo->enCscMode);

	
	//VDP_WBC_SetOutIntf			 (enLayerId, pstCfg->enReadMode);
	VDP_WBC_SetOutFmt			   ((VDP_LAYER_WBC_E)u32LayerId, pstInfo->enOutFmt);
	if((VDP_LAYER_WBC_E)u32LayerId == VDP_LAYER_WBC_VP0)
	VDP_WBC_SetLayerAddr		   ((VDP_LAYER_WBC_E)u32LayerId, pstInfo->stAddr[VDP_ADDR_WBCVP0].u32LumAddr, pstInfo->stAddr[VDP_ADDR_WBCVP0].u32ChmAddr,pstInfo->stAddr[VDP_ADDR_WBCVP0].u32LumStr, pstInfo->stAddr[VDP_ADDR_WBCVP0].u32ChmStr);
	else if((VDP_LAYER_WBC_E)u32LayerId == VDP_LAYER_WBC_HD0)
	VDP_WBC_SetLayerAddr		   ((VDP_LAYER_WBC_E)u32LayerId, pstInfo->stAddr[VDP_ADDR_WBCDHD0].u32LumAddr, pstInfo->stAddr[VDP_ADDR_WBCDHD0].u32ChmAddr,pstInfo->stAddr[VDP_ADDR_WBCDHD0].u32LumStr, pstInfo->stAddr[VDP_ADDR_WBCDHD0].u32ChmStr);

	VDP_WBC_SetCropPos			   ((VDP_LAYER_WBC_E)u32LayerId, st_crop);
	VDP_WBC_SetOutIntf			   ((VDP_LAYER_WBC_E)u32LayerId, (VDP_DATA_RMODE_E)pstInfo->u32OutIop);
	VDP_WBC_SetOutReso			   ((VDP_LAYER_WBC_E)u32LayerId, pstInfo->stOutRect);

	//VDP_WBC_SetDitherMode 		 (enLayerId, VDP_DITHER_ROUND_8);
	VDP_DRV_SetWbcDitherMode(u32LayerId, pstInfo->enWbcDitherMode);

	VDP_WBC_SetThreeMd			  	   ((VDP_LAYER_WBC_E)u32LayerId, 0);
	VDP_WBC_SetSmmuCBypass((VDP_LAYER_WBC_E)u32LayerId, !pstInfo->bSmmu);
	VDP_WBC_SetSmmuLBypass((VDP_LAYER_WBC_E)u32LayerId, !pstInfo->bSmmu);

	//--------------------------------------------
	//wbc cds setting
	//--------------------------------------------
	if((VDP_LAYER_WBC_E)u32LayerId == VDP_LAYER_WBC_HD0 )
	{
		 VDP_WBC_SetCdsEnable			((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
		 VDP_WBC_SetCdsFirEnable		((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
		 VDP_WBC_SetCdsMidEnable		((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
		 VDP_WBC_SetCdsCoef 			((VDP_LAYER_WBC_E)u32LayerId, s32CdsCoef);
	}
	else if((VDP_LAYER_WBC_E)u32LayerId == VDP_LAYER_WBC_VP0)
	{
		VDP_WBC_SetWbcVpConnection((VDP_LAYER_WBC_E)u32LayerId,pstInfo->enWbcVpConnPoint);


		if(pstInfo->enWbcVpConnPoint == VDP_LAYER_CONN_VPACSC)
		{
			VDP_WBC_SetCdsEnable		   ((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
			VDP_WBC_SetCdsFirEnable 	   ((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
			VDP_WBC_SetCdsMidEnable 	   ((VDP_LAYER_WBC_E)u32LayerId, HI_TRUE);
			VDP_WBC_SetCdsCoef			   ((VDP_LAYER_WBC_E)u32LayerId, s32CdsCoef);
		}
		else
		{
			VDP_WBC_SetCdsEnable		   ((VDP_LAYER_WBC_E)u32LayerId, HI_FALSE);
		}

	}

	//--------------------------------------------
	//wbc zme setting
	//--------------------------------------------

	stZmeInfo.stInRect  = st_crop;
	stZmeInfo.stOutRect = pstInfo->stOutRect;
	stZmeInfo.enWbcVpConnPoint = pstInfo->enWbcVpConnPoint;
	stZmeInfo.enInFmt   = pstInfo->enInFmt;

	/*pass the wbc zme config to zme drv para.*/
	stZmeInfo.pstZmeDrvPara.bZmeFirHL = pstInfo->bZmeFirHL;
	stZmeInfo.pstZmeDrvPara.bZmeFirHC = pstInfo->bZmeFirHC;
	
	stZmeInfo.pstZmeDrvPara.bZmeFirVL = pstInfo->bZmeFirVL;
	stZmeInfo.pstZmeDrvPara.bZmeFirVC = pstInfo->bZmeFirVC;
	
	stZmeInfo.pstZmeDrvPara.bZmeMedH	 = pstInfo->bZmeMedH; 
	stZmeInfo.pstZmeDrvPara.bZmeMedV	 = pstInfo->bZmeMedV;
    stZmeInfo.pstZmeDrvPara.bZmeDefault	 = pstInfo->bZmeDefault;
	
	stZmeInfo.pstZmeDrvPara.bZmeFrmFmtIn	 = pstInfo->bZmeFrmFmtIn;
	stZmeInfo.pstZmeDrvPara.bZmeFrmFmtOut	 = pstInfo->bZmeFrmFmtOut;
	stZmeInfo.pstZmeDrvPara.bZmeBFIn	 = pstInfo->bZmeBFIn;
	stZmeInfo.pstZmeDrvPara.bZmeBFOut	 = pstInfo->bZmeBFOut;
	
	VDP_DRV_SetWbcZme((VDP_LAYER_WBC_E)u32LayerId, &stZmeInfo);	
	VDP_WBC_SetRegUp(u32LayerId);
	
	return  HI_SUCCESS;


}

