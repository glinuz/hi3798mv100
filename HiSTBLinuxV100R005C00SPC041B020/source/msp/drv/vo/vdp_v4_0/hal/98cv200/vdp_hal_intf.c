#include <linux/string.h>

#include "hi_type.h"
#include "vdp_drv_vid.h"
#include "vdp_drv_wbc.h"
#include "vdp_drv_comm.h"
#include "vdp_hal_intf.h"
#include "vdp_hal_chn.h"

#define  SYS_CONTROL_VDP_LOCATION_SEL_  0xf8a20910
#define  REG_IOREMAP_SIZE               0x1000

extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;
HI_U32 *gu32TnrDnrLocationSel = HI_NULL;

static HI_U32 *Internal_Get_TnrLocationSel(HI_VOID)
{
	return gu32TnrDnrLocationSel;
}

static HI_S32  Internal_SelectTnrLocation(HI_PQ_ALGIP_SEL_E   enAlgIPSel)
{
	HI_U32 *pu32TnrLocationSelAddr = HI_NULL;
	
	pu32TnrLocationSelAddr = Internal_Get_TnrLocationSel();
	if (!pu32TnrLocationSelAddr)
	{
		return HI_FAILURE;
	}
	
	if (HI_PQ_ALGIP_SEL_VDP == enAlgIPSel)
	{
		/*only set bit[4] to switch alg to vdp*/
		*pu32TnrLocationSelAddr |= 0x1;
	}
	else if (HI_PQ_ALGIP_SEL_VPSS == enAlgIPSel)
	{		
		*pu32TnrLocationSelAddr &= 0xfffffffe;
	}
	
	return HI_SUCCESS;
}

static HI_VOID Internal_GenateTnrDbDmInfor(HI_PQ_VDP_WBC_INFO_S *pstVdpLayerAlgInfor,
	                                                  CBB_LAYER_CFG_INFO_S *pLayerCfgBasePara)
{
	pstVdpLayerAlgInfor->bCropBeforeDB = pLayerCfgBasePara->stAlgWbcInfor.bCropBeforeDB;
    pstVdpLayerAlgInfor->u32BitDepth = pLayerCfgBasePara->stAlgWbcInfor.u32BitDepth;
    pstVdpLayerAlgInfor->stOriCoordinate = pLayerCfgBasePara->stAlgWbcInfor.stOriCoordinate;
    pstVdpLayerAlgInfor->stCropCoordinate = pLayerCfgBasePara->stAlgWbcInfor.stCropCoordinate;
    pstVdpLayerAlgInfor->enAlgIPSel = pLayerCfgBasePara->stAlgWbcInfor.enAlgIPSel;
    
    pstVdpLayerAlgInfor->pstVdpWbcReg = pLayerCfgBasePara->stAlgWbcInfor.pstVdpWbcRegCpuAddr;
    pstVdpLayerAlgInfor->u32VdpWbcRegPhyAddr = pLayerCfgBasePara->stAlgWbcInfor.u32VdpWbcRegPhyAddr;
    
    pstVdpLayerAlgInfor->u32SingleModeFlag = pLayerCfgBasePara->stAlgWbcInfor.u32SingleModeFlag;			
    pstVdpLayerAlgInfor->u32ImageAvgQP = pLayerCfgBasePara->stAlgWbcInfor.u32ImageAvgQP;
    pstVdpLayerAlgInfor->u32VdhInfoAddr = pLayerCfgBasePara->stAlgWbcInfor.u32VdhInfoAddr;
    pstVdpLayerAlgInfor->u32VdhInfoStride = pLayerCfgBasePara->stAlgWbcInfor.u32VdhInfoStride;
    pstVdpLayerAlgInfor->u32ImageStruct = pLayerCfgBasePara->stAlgWbcInfor.u32ImageStruct;
    pstVdpLayerAlgInfor->enVdecProtocol = pLayerCfgBasePara->stAlgWbcInfor.enVdecProtocol;	
	pstVdpLayerAlgInfor->enLayerId	   =  pLayerCfgBasePara->u32LayerNO;
	
	return;
}

static HI_VOID Internal_TransferWbcInfor(HI_PQ_ZME_STRATEGY_IN_U *puWbcZmeIn,
	                                              CBB_WBC_CFG_INFO_S *pstWbcCfgBasePara)
{

	puWbcZmeIn->stWbcStrategy.stLayerInfo[0].u32FmtIn = 1;	
    puWbcZmeIn->stWbcStrategy.stLayerInfo[0].u32FrmWIn = pstWbcCfgBasePara->stHdWinInputSize.s32Width;  
    puWbcZmeIn->stWbcStrategy.stLayerInfo[0].u32FrmHIn = pstWbcCfgBasePara->stHdWinInputSize.s32Height; 
    puWbcZmeIn->stWbcStrategy.stLayerInfo[0].u32FrmWOut = pstWbcCfgBasePara->stHdWinOutputSize.s32Width;
    puWbcZmeIn->stWbcStrategy.stLayerInfo[0].u32FrmHOut = pstWbcCfgBasePara->stHdWinOutputSize.s32Height;
    
    
    puWbcZmeIn->stWbcStrategy.stLayerInfo[1].u32FmtIn = 0;
    puWbcZmeIn->stWbcStrategy.stLayerInfo[1].u32FrmWIn = pstWbcCfgBasePara->stHdWinInputSize.s32Width;  
    puWbcZmeIn->stWbcStrategy.stLayerInfo[1].u32FrmHIn = pstWbcCfgBasePara->stHdWinInputSize.s32Height; 
    puWbcZmeIn->stWbcStrategy.stLayerInfo[1].u32FrmWOut = pstWbcCfgBasePara->stSdWinOutputSize.s32Width;
    puWbcZmeIn->stWbcStrategy.stLayerInfo[1].u32FrmHOut = pstWbcCfgBasePara->stSdWinOutputSize.s32Height;
	
	puWbcZmeIn->stWbcStrategy.bForceWbcPoint2Vp = pstWbcCfgBasePara->bForceWbcPoint2Vp;
	puWbcZmeIn->stWbcStrategy.bForceWbcFieldMode = pstWbcCfgBasePara->bForceWbcFieldMode;
	
	return;
}


static HI_BOOL Internal_GetVidZmeOutFormatBasedOnP2IState(HI_U32 u32LayerNO, HI_BOOL bP2IOpen)
{
    if (HI_FALSE == bP2IOpen)
    {
        return (VDP_DISP_GetDispMode(0)==1)? HI_TRUE : HI_FALSE;
    }
    
    if (u32LayerNO == VDP_LAYER_VID0 || u32LayerNO == VDP_LAYER_VID1)
    {
        return HI_TRUE; /* Progressive */
    }
    else
    {
        return HI_FALSE; /* Interlace */
    }
}

HI_S32 CBB_Video_SetLayerParam(CBB_LAYER_CFG_INFO_S *pLayerCfgBasePara,CBB_CFG_EXT_S *pVDPCfgExtPara)
{
	VDP_LAYER_VID_INFO_S stVidInfo;
	VDP_DATA_RMODE_E eReadDataMode;
	HI_PQ_VDP_WBC_INFO_S stVdpLayerAlgInfor;

	
#ifdef  VDP_PQ_STRATEGY
	HI_PQ_ZME_STRATEGY_IN_U  pstZmeIn ;
	HI_PQ_ZME_STRATEGY_OUT_U pstZmeOut;
	
	HI_S32   s32Ret = HI_SUCCESS;
	HI_BOOL bBtm = HI_FALSE;
	HI_U32  u32Vcnt = 0;
#endif

	memset((HI_VOID  *)&stVidInfo,0,sizeof(VDP_LAYER_VID_INFO_S));
	if ((pLayerCfgBasePara->u32LayerNO >= VDP_LAYER_VID4)
		|| (pLayerCfgBasePara->u32RegionNO >= 16)
		)
		
	{
		return HI_FAILURE;
	}

	memset((HI_VOID*)&stVidInfo, 0, sizeof(VDP_LAYER_VID_INFO_S));
	stVidInfo.u32RegionNO        	  =  pLayerCfgBasePara->u32RegionNO;
	stVidInfo.u32LayerZorder          =  pLayerCfgBasePara->u32LayerZorder;
	stVidInfo.bRegionEn         	  =  pLayerCfgBasePara->bRegionEn;	
	stVidInfo.bRegionModeEn 		  =  pLayerCfgBasePara->bRegionModeEn;
	
#ifdef  VDP_PQ_STRATEGY
	if (!pLayerCfgBasePara->bMuteEn && pLayerCfgBasePara->bRegionEn)
	{
        pstZmeIn.stLayerStrategy.u32FmtIn   = pLayerCfgBasePara->stFrame.eSrcFmt;
        pstZmeIn.stLayerStrategy.u32FrmWIn  = pLayerCfgBasePara->stCropRect.s32Width;	
        pstZmeIn.stLayerStrategy.u32FrmHIn  = pLayerCfgBasePara->stCropRect.s32Height; 

		if(((pLayerCfgBasePara->stDispInfo.bInterlace	== 1)&&(pLayerCfgBasePara->stDispInfo.stFmtResolution.s32Width == 720 )&&(pLayerCfgBasePara->stDispInfo.stFmtResolution.s32Height	== 576))
			||((pLayerCfgBasePara->stDispInfo.bInterlace  == 1)&&(pLayerCfgBasePara->stDispInfo.stFmtResolution.s32Width == 720 )&&(pLayerCfgBasePara->stDispInfo.stFmtResolution.s32Height  == 480)))
		{
	        pstZmeIn.stLayerStrategy.u32FrmWOut = pLayerCfgBasePara->stVideoRect.s32Width*2;
		}
		else
		{
	        pstZmeIn.stLayerStrategy.u32FrmWOut = pLayerCfgBasePara->stVideoRect.s32Width;
		}
		
        pstZmeIn.stLayerStrategy.u32FrmHOut = pLayerCfgBasePara->stVideoRect.s32Height;

		if ((pLayerCfgBasePara->u32LayerNO == VDP_LAYER_VID0)
		   || (pLayerCfgBasePara->u32LayerNO == VDP_LAYER_VID1))
		{

            if (HI_SUCCESS != (s32Ret = DRV_PQ_GetVdpZmeStrategy ((HI_PQ_ZME_LAYER_E)pLayerCfgBasePara->u32LayerNO, &pstZmeIn, &pstZmeOut)))
            {
        		return s32Ret;
            }

     		stVidInfo.enDrawMode = (HI_U32)pstZmeOut.stStrategyLayerId.eVScalerMode;
	#if 0  	
			VDP_PRINT("---1 zme:%d,%d,%d,%d!\n", pstZmeOut.stStrategyLayerId.stZmeFmt[0].u32FrmInWidth,
                            				   pstZmeOut.stStrategyLayerId.stZmeFmt[0].u32FrmInHeight,
                            				   pstZmeOut.stStrategyLayerId.stZmeFmt[0].u32FrmOutWidth,
                            				   pstZmeOut.stStrategyLayerId.stZmeFmt[0].u32FrmOutHeight);

			
			VDP_PRINT("---2 zme:%d,%d,%d,%d!\n", pstZmeOut.stStrategyLayerId.stZmeFmt[1].u32FrmInWidth,
                            				   pstZmeOut.stStrategyLayerId.stZmeFmt[1].u32FrmInHeight,
                            				   pstZmeOut.stStrategyLayerId.stZmeFmt[1].u32FrmOutWidth,
                            				   pstZmeOut.stStrategyLayerId.stZmeFmt[1].u32FrmOutHeight);
	#endif

			/*only layer who has 2 scalers should set zme2. */
			if (pstZmeOut.stStrategyLayerId.u32ZmeNum == 2)
			{
	    		stVidInfo.stZme2InRect.u32Wth  = 	pstZmeOut.stStrategyLayerId.stZmeFmt[1].u32FrmInWidth;
	    		stVidInfo.stZme2InRect.u32Hgt  = 	pstZmeOut.stStrategyLayerId.stZmeFmt[1].u32FrmInHeight;
			}
			else
			{
				stVidInfo.stZme2InRect.u32Wth  = 	0;
	    		stVidInfo.stZme2InRect.u32Hgt  = 	0;
			}
						
			Internal_GenateTnrDbDmInfor(&stVdpLayerAlgInfor, pLayerCfgBasePara);
			if (Internal_SelectTnrLocation(pLayerCfgBasePara->stAlgWbcInfor.enAlgIPSel))
			{
				return HI_FAILURE;
			}	
			
			DRV_PQ_SetVdpAlgCalcCfg(&stVdpLayerAlgInfor);			
		}
		else
		{
			stVidInfo.enDrawMode = VDP_DRAW_MODE_1;
    		stVidInfo.stZme2InRect.u32Wth  = 	0;
    		stVidInfo.stZme2InRect.u32Hgt  = 	0;
		}
		
        /*pass zme strategy to zme.*/
		stVidInfo.pstZmeDrvPara.bZmeFirHL	= pstZmeOut.stStrategyLayerId.bZmeFirHL;
		stVidInfo.pstZmeDrvPara.bZmeFirHC	= pstZmeOut.stStrategyLayerId.bZmeFirHC;
		stVidInfo.pstZmeDrvPara.bZmeFirVL	= pstZmeOut.stStrategyLayerId.bZmeFirVL;
		stVidInfo.pstZmeDrvPara.bZmeFirVC	= pstZmeOut.stStrategyLayerId.bZmeFirVC;
		stVidInfo.pstZmeDrvPara.bZmeMedH	= pstZmeOut.stStrategyLayerId.bZmeMedH;
		stVidInfo.pstZmeDrvPara.bZmeMedV	= pstZmeOut.stStrategyLayerId.bZmeMedV;
		stVidInfo.pstZmeDrvPara.bZmeDefault	= pstZmeOut.stStrategyLayerId.bZmeDefault;

		stVidInfo.pstZmeDrvPara.bZmeFrmFmtIn  = pstZmeOut.stStrategyLayerId.bReadFmtIn;
        stVidInfo.pstZmeDrvPara.bZmeFrmFmtOut = Internal_GetVidZmeOutFormatBasedOnP2IState(pLayerCfgBasePara->u32LayerNO, HI_TRUE);

		/*set the layer's read  t/b mode, when dhd0 is progressive, the t/b config 
		 *is  useless. If dhd0 is interleave, the output mode is fixed to dhd0's opposite.
		 * And the input mode forced to set to the output. since if input is p, the input mode is useless
		 * and if input is interleave ,the input mode is the same to the output mode.
		 */
	    if ((pLayerCfgBasePara->u32LayerNO ==  VDP_LAYER_VID0)
		   || (pLayerCfgBasePara->u32LayerNO ==  VDP_LAYER_VID1))
	    {
		    /*get current t/b filed mode.*/
			VDP_DISP_GetVactState(0,&bBtm,&u32Vcnt);
			
			/*config next period's effective t/b field mode. */
			stVidInfo.pstZmeDrvPara.bZmeBFOut 	    = !bBtm;
		    stVidInfo.pstZmeDrvPara.bZmeBFIn		= !bBtm;
	    }
		else
		{
			stVidInfo.pstZmeDrvPara.bZmeBFOut 	    = HI_FALSE;
		    stVidInfo.pstZmeDrvPara.bZmeBFIn		= pstZmeOut.stStrategyLayerId.bReadBFIn;
		}		
		
		/* frame is interlace, this is for V3.*/
		if(pLayerCfgBasePara->stFrame.bFrameInterlace == HI_TRUE)
		{
			if(pLayerCfgBasePara->stFrame.eFieldMode == CBB_FIELD_TOP)
			{
				eReadDataMode           = VDP_RMODE_TOP;
			}
			else
			{
				eReadDataMode           = VDP_RMODE_BOTTOM;
			}
		}
		else
		{
			/*read frame.*/
			if (pstZmeOut.stStrategyLayerId.bReadFmtIn == HI_TRUE)
			{
				eReadDataMode  =  VDP_RMODE_PROGRESSIVE;
			}
			else
			{
				eReadDataMode  =  VDP_RMODE_INTERFACE;
			}
		}
	}
	else	
	{
		stVidInfo.pstZmeDrvPara.bZmeFrmFmtIn	 = HI_TRUE;
	    stVidInfo.pstZmeDrvPara.bZmeFrmFmtOut   = HI_TRUE;
	    stVidInfo.pstZmeDrvPara.bZmeBFOut	     = HI_FALSE;
	    stVidInfo.pstZmeDrvPara.bZmeBFIn		 = HI_FALSE;
		stVidInfo.stZme2InRect.u32Wth  = 	0;
   		stVidInfo.stZme2InRect.u32Hgt  = 	0;

	    stVidInfo.enDrawMode = VDP_DRAW_MODE_1;

        if(pLayerCfgBasePara->stFrame.bFrameInterlace == HI_TRUE)
        {
        	if(pLayerCfgBasePara->stFrame.eFieldMode == CBB_FIELD_TOP)
        	{
        		eReadDataMode           = VDP_RMODE_TOP;
        	}
        	else
        	{
        		eReadDataMode           = VDP_RMODE_BOTTOM;
        	}
        }
        else
        {	
			if (pLayerCfgBasePara->stDispInfo.bInterlace == 1)
			{
				eReadDataMode  =  VDP_RMODE_INTERLACE;		
			}
			else
			{
				eReadDataMode  =  VDP_RMODE_PROGRESSIVE;
			}
   		}
	//	printk("mute-layer%d-r-%d---F--%d\n",pLayerCfgBasePara->u32LayerNO,pstZmeOut.stStrategyLayerId.bReadFmtIn,pLayerCfgBasePara->stFrame.bFrameInterlace);
		
	}	
#else
	{
		// CBB  process  add by hyx
		stVidInfo.pstZmeDrvPara.bZmeFrmFmtIn	 = HI_TRUE;
		stVidInfo.pstZmeDrvPara.bZmeFrmFmtOut   = HI_TRUE;
		stVidInfo.pstZmeDrvPara.bZmeBFOut	     = HI_FALSE;
		stVidInfo.pstZmeDrvPara.bZmeBFIn		 = HI_FALSE;
		stVidInfo.stZme2InRect.u32Wth  = 	0;
   		stVidInfo.stZme2InRect.u32Hgt  = 	0;

		stVidInfo.enDrawMode = VDP_DRAW_MODE_1;

		if(pLayerCfgBasePara->stFrame.bFrameInterlace == HI_TRUE)
		{
			if(pLayerCfgBasePara->stFrame.eFieldMode == CBB_FIELD_TOP)
			{
				eReadDataMode           = VDP_RMODE_TOP;
			}
			else
			{
				eReadDataMode           = VDP_RMODE_BOTTOM;
			}
		}
		else
		{			
			 eReadDataMode  =  VDP_RMODE_PROGRESSIVE;			
		}

	}
#endif

	stVidInfo.enReadMode = eReadDataMode;
		
	if (stVidInfo.bRegionEn == HI_TRUE)
	{
		stVidInfo.bFlipEn              = pLayerCfgBasePara->bFlipEn;

		stVidInfo.stCropRect.u32X      = (HI_U32)(pLayerCfgBasePara->stCropRect.s32X);
		stVidInfo.stCropRect.u32Y      = (HI_U32)(pLayerCfgBasePara->stCropRect.s32Y);
		stVidInfo.stCropRect.u32Wth    = pLayerCfgBasePara->stCropRect.s32Width;
		stVidInfo.stCropRect.u32Hgt    = pLayerCfgBasePara->stCropRect.s32Height;

		stVidInfo.stVideoRect.u32X     = (HI_U32)(pLayerCfgBasePara->stVideoRect.s32X);
		stVidInfo.stVideoRect.u32Y     = (HI_U32)(pLayerCfgBasePara->stVideoRect.s32Y);
		stVidInfo.stVideoRect.u32Wth   = pLayerCfgBasePara->stVideoRect.s32Width;
		stVidInfo.stVideoRect.u32Hgt   = pLayerCfgBasePara->stVideoRect.s32Height;

		stVidInfo.stDispRect.u32X      = (HI_U32)(pLayerCfgBasePara->stDispRect.s32X);
		stVidInfo.stDispRect.u32Y      = (HI_U32)(pLayerCfgBasePara->stDispRect.s32Y);
		stVidInfo.stDispRect.u32Wth    = pLayerCfgBasePara->stDispRect.s32Width;
		stVidInfo.stDispRect.u32Hgt    = pLayerCfgBasePara->stDispRect.s32Height;

		switch(pLayerCfgBasePara->enOutColorSpace)
		{
			case CBB_CSC_BT601_YUV_LIMITED:
			{
				stVidInfo.enOutClrSpace = VDP_CLR_SPACE_YUV_601_L;
				break;
			}
			case CBB_CSC_BT601_YUV_FULL:
			{
				stVidInfo.enOutClrSpace = VDP_CLR_SPACE_YUV_601;
				break;
			}
			case CBB_CSC_BT709_YUV_LIMITED:
			{
				stVidInfo.enOutClrSpace = VDP_CLR_SPACE_YUV_709_L;
				break;
			}
			case CBB_CSC_BT709_YUV_FULL:
			{
				stVidInfo.enOutClrSpace = VDP_CLR_SPACE_YUV_709; 
				break;
			}
			case CBB_CSC_BT709_RGB_FULL:
			{
				stVidInfo.enOutClrSpace = VDP_CLR_SPACE_RGB_709; 
				break;
			}
			case CBB_CSC_BT2020_YUV_LIMITED:
			{
				stVidInfo.enOutClrSpace = VDP_CLR_SPACE_YUV_2020_L;
				break;
			}
			case CBB_CSC_BT2020_RGB_FULL:
			{
				stVidInfo.enOutClrSpace = VDP_CLR_SPACE_RGB_2020;
				break;
			}
			default:
			{
				//error
				break;
			}
		}


		
		switch(pLayerCfgBasePara->stFrame.enSrcColorSpace)
		{
			case CBB_CSC_BT601_YUV_LIMITED:
			{
				stVidInfo.enInClrSpace = VDP_CLR_SPACE_YUV_601_L;
				break;
			}
			case CBB_CSC_BT601_YUV_FULL:
			{
				stVidInfo.enInClrSpace = VDP_CLR_SPACE_YUV_601;
				break;
			}
			case CBB_CSC_BT709_YUV_LIMITED:
			{
				stVidInfo.enInClrSpace = VDP_CLR_SPACE_YUV_709_L;
				break;
			}
			case CBB_CSC_BT709_YUV_FULL:
			{
				stVidInfo.enInClrSpace = VDP_CLR_SPACE_YUV_709; 
				break;
			}
			case CBB_CSC_BT709_RGB_FULL:
			{
				stVidInfo.enInClrSpace = VDP_CLR_SPACE_RGB_709; 
				break;
			}
			case CBB_CSC_BT2020_YUV_LIMITED:
			{
				stVidInfo.enInClrSpace = VDP_CLR_SPACE_YUV_2020_L;
				break;
			}
			case CBB_CSC_BT2020_RGB_FULL:
			{
				stVidInfo.enInClrSpace = VDP_CLR_SPACE_RGB_2020;
				break;
			}
			default:
			{
				//error
				break;
			}
		}
		


		if(pLayerCfgBasePara->stFrame.en3dSrcFormat == CBB_FT_SBS)
		{
			stVidInfo.stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_SBS; 
		}
		else if(pLayerCfgBasePara->stFrame.en3dSrcFormat == CBB_FT_TAB)
		{
			stVidInfo.stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_TAB; 
		}
		else if(pLayerCfgBasePara->stFrame.en3dSrcFormat == CBB_FT_FPK)
		{
			stVidInfo.stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_FP; 
		}
		else//??CBB_FT_TILE
		{
			stVidInfo.stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_2D; 
		}

		
		if(pLayerCfgBasePara->stFrame.en3dSrcFormat == CBB_FT_SBS)
		{
			stVidInfo.stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_SBS; 
		}
		else if(pLayerCfgBasePara->stFrame.en3dSrcFormat == CBB_FT_TAB)
		{
			stVidInfo.stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_TAB; 
		}
		else if(pLayerCfgBasePara->stFrame.en3dSrcFormat == CBB_FT_FPK)
		{
			stVidInfo.stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_FP; 
		}
		else//??CBB_FT_TILE
		{
			stVidInfo.stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_2D; 
		}

		switch(pLayerCfgBasePara->stDispInfo.eDispMode)
		{
			case CBB_DISP_STEREO_NONE:
				stVidInfo.enDispMode = VDP_DISP_MODE_2D;
				break;
				
			case CBB_DISP_STEREO_FPK:			
				stVidInfo.enDispMode = VDP_DISP_MODE_FP;
				break;
				
			 case CBB_DISP_STEREO_SBS_HALF:
				stVidInfo.enDispMode = VDP_DISP_MODE_SBS;
				break;
				
			case CBB_DISP_STEREO_TAB:
				stVidInfo.enDispMode = VDP_DISP_MODE_TAB;
				break;
				
			case CBB_DISP_STEREO_SBS_FULL:			
				stVidInfo.enDispMode = VDP_DISP_MODE_FS;
				break;
		
			 default:
				stVidInfo.enDispMode = VDP_DISP_MODE_2D;
				break;
		}
		
		stVidInfo.stVdpDispInfo.enDispMode = stVidInfo.enDispMode;
		stVidInfo.stVdpDispInfo.bInterlace  = pLayerCfgBasePara->stDispInfo.bInterlace;
		stVidInfo.stVdpDispInfo.stFmtResolution.u32X  = pLayerCfgBasePara->stDispInfo.stFmtResolution.s32X;
		stVidInfo.stVdpDispInfo.stFmtResolution.u32Y  = pLayerCfgBasePara->stDispInfo.stFmtResolution.s32Y;
		stVidInfo.stVdpDispInfo.stFmtResolution.u32Wth  = pLayerCfgBasePara->stDispInfo.stFmtResolution.s32Width;
		stVidInfo.stVdpDispInfo.stFmtResolution.u32Hgt  = pLayerCfgBasePara->stDispInfo.stFmtResolution.s32Height;
		stVidInfo.stVdpDispInfo.stFmtResolution.u32SX = 0;


		stVidInfo.u32DepthLevel        = pLayerCfgBasePara->stFrame.u32DepthLevel;

		stVidInfo.stSrcInfo.bSecureEn             = pLayerCfgBasePara->stFrame.bSecure; 
		stVidInfo.stSrcInfo.bSmmuEn               = pLayerCfgBasePara->stFrame.bSmmu; 

		stVidInfo.stSrcInfo.bUvOrder              = pLayerCfgBasePara->stFrame.bUvorder; 
		stVidInfo.stSrcInfo.bDcmpEn               = pLayerCfgBasePara->stFrame.bCmpFmt; 
		stVidInfo.stSrcInfo.bDcmpLost             = pLayerCfgBasePara->stFrame.bLostCmp; 

		switch(pLayerCfgBasePara->stFrame.eSrcFmt)
		{
			case CBB_VID_IFMT_SP_400:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_SP_400; 
				break;
			}
			case CBB_VID_IFMT_SP_420:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_SP_420; 
				break;
			}
			case CBB_VID_IFMT_SP_422:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_SP_422; 
				break;
			}
			case CBB_VID_IFMT_SP_444:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_SP_444; 
				break;
			}
			case CBB_VID_IFMT_SP_TILE:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_SP_TILE; 
				break;
			}
			case CBB_VID_IFMT_SP_TILE_64:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_SP_TILE_64; 
				break;
			}
			case CBB_VID_IFMT_PKG_UYVY:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_PKG_UYVY; 
				break;
			}
			case CBB_VID_IFMT_PKG_YUYV:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_PKG_YUYV; 
				break;
			}
			case CBB_VID_IFMT_PKG_YVYU:
			{
				stVidInfo.stSrcInfo.enInFmt               = VDP_VID_IFMT_PKG_YVYU; 
				break;
			}
			default:
			{
				//error
				break;
			}
		}

		if(pLayerCfgBasePara->stFrame.enSrcBitWidth == CBB_PIXEL_BITWIDTH_8BIT)
		{
			stVidInfo.stSrcInfo.enDataWidth           = VDP_DATA_WTH_8; 
		}
		else if(pLayerCfgBasePara->stFrame.enSrcBitWidth == CBB_PIXEL_BITWIDTH_10BIT)
		{
			stVidInfo.stSrcInfo.enDataWidth           = VDP_DATA_WTH_10; 
		}
		else
		{
			//error
			stVidInfo.stSrcInfo.enDataWidth           = VDP_DATA_WTH_10; 
		}


		stVidInfo.stSrcInfo.stSrcRect.u32X      = (HI_U32)(pLayerCfgBasePara->stFrame.stSrcRect.s32X);
		stVidInfo.stSrcInfo.stSrcRect.u32Y      = (HI_U32)(pLayerCfgBasePara->stFrame.stSrcRect.s32Y);
		stVidInfo.stSrcInfo.stSrcRect.u32Wth    = pLayerCfgBasePara->stFrame.stSrcRect.s32Width;
		stVidInfo.stSrcInfo.stSrcRect.u32Hgt    = pLayerCfgBasePara->stFrame.stSrcRect.s32Height;

		stVidInfo.stSrcInfo.bSrcIsInterlace       = pLayerCfgBasePara->stFrame.bFrameInterlace; 

		//For V1


		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_DATA].u32LumAddr = pLayerCfgBasePara->stFrame.stAddress[0].u32PhyAddr_Y; 
		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_DATA].u32ChmAddr = pLayerCfgBasePara->stFrame.stAddress[0].u32PhyAddr_C; 
		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_DATA].u32LumStr  = pLayerCfgBasePara->stFrame.stAddress[0].u32Stride_Y; 
		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_DATA].u32ChmStr  = pLayerCfgBasePara->stFrame.stAddress[0].u32Stride_C; 
		

        if (stVidInfo.stSrcInfo.enSrc3dMode != VDP_DISP_MODE_2D)
        {
            stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32LumAddr = pLayerCfgBasePara->stFrame.stAddress[1].u32PhyAddr_Y; 
    		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmAddr = pLayerCfgBasePara->stFrame.stAddress[1].u32PhyAddr_C; 
		    stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32LumStr  = pLayerCfgBasePara->stFrame.stAddress[1].u32Stride_Y; 
	    	stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmStr  = pLayerCfgBasePara->stFrame.stAddress[1].u32Stride_C; 
        }
        else
        {
			if ( pLayerCfgBasePara->stDispInfo.eDispMode == CBB_DISP_STEREO_NONE )
			{
	            stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32LumAddr = 0; 
	    		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmAddr = 0; 
			    stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32LumStr  = 0; 
		    	stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmStr  = 0; 
			}
			else
			{
				stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32LumAddr = pLayerCfgBasePara->stFrame.stAddress[0].u32PhyAddr_Y; 
	    		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmAddr = pLayerCfgBasePara->stFrame.stAddress[0].u32PhyAddr_C; 
			    stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32LumStr  = pLayerCfgBasePara->stFrame.stAddress[0].u32Stride_Y; 
		    	stVidInfo.stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmStr  = pLayerCfgBasePara->stFrame.stAddress[0].u32Stride_C; 
			}
        }
		
		if (stVidInfo.stSrcInfo.bDcmpEn)
		{
			stVidInfo.stSrcInfo.stAddr[VDP_ADDR_HEAD].u32LumAddr = pLayerCfgBasePara->stFrame.stAddress[0].u32PhyAddr_YHead; 
			stVidInfo.stSrcInfo.stAddr[VDP_ADDR_HEAD].u32ChmAddr = pLayerCfgBasePara->stFrame.stAddress[0].u32PhyAddr_CHead; 

			/*head stride*/
			stVidInfo.stSrcInfo.stAddr[VDP_ADDR_HEAD].u32LumStr  = pLayerCfgBasePara->stFrame.stAddress[0].u32Head_Stride; 
			/*head size*/
			stVidInfo.stSrcInfo.stAddr[VDP_ADDR_HEAD].u32DcmpHeadSize  = pLayerCfgBasePara->stFrame.stAddress[0].u32Head_Size; 
		}
		
		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_TILE].u32LumAddr = pLayerCfgBasePara->stFrame.stAddress_lb[0].u32PhyAddr_Y; 
		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_TILE].u32ChmAddr = pLayerCfgBasePara->stFrame.stAddress_lb[0].u32PhyAddr_C; 
		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_TILE].u32LumStr  = pLayerCfgBasePara->stFrame.stAddress_lb[0].u32Stride_Y; 
		stVidInfo.stSrcInfo.stAddr[VDP_ADDR_TILE].u32ChmStr  = pLayerCfgBasePara->stFrame.stAddress_lb[0].u32Stride_C; 
		
		stVidInfo.stVideoRect   =  stVidInfo.stVideoRect;		
	}
		stVidInfo.bRegionModeEn = pLayerCfgBasePara->bRegionModeEn;
		stVidInfo.u32RegionNO   = pLayerCfgBasePara->u32RegionNO  ;
		stVidInfo.bRegionEn     = pLayerCfgBasePara->bRegionEn    ;
		stVidInfo.stSrcInfo.enSrcClrSpace         = stVidInfo.enInClrSpace ;//to do 		
		stVidInfo.bMuteEn              = pLayerCfgBasePara->bMuteEn;
		stVidInfo.stMuteColor.u32BkgY  = 0x40; 
		stVidInfo.stMuteColor.u32BkgU  = 0x200; 
		stVidInfo.stMuteColor.u32BkgV  = 0x200; 
		stVidInfo.stMuteColor.u32BkgA  = 0xff; 
		stVidInfo.stLetterBoxColor     = stVidInfo.stMuteColor;
		stVidInfo.stMixvColor          = stVidInfo.stMuteColor; 

	VDP_DRV_SetVidLayer(pLayerCfgBasePara->u32LayerNO, &stVidInfo);


	return HI_SUCCESS;
}

HI_S32 CBB_Video_SetWbcParam(CBB_WBC_CFG_INFO_S *pLayerCfgBasePara,CBB_CFG_EXT_S *pVDPCfgExtPara)
{
	VDP_LAYER_WBC_INFO_S stWbcInfo = {0};
	
#ifdef  VDP_PQ_STRATEGY
    HI_PQ_ZME_STRATEGY_IN_U  unionZmeIn;
    HI_PQ_ZME_STRATEGY_OUT_U unionZmeOut;
	HI_S32   s32Ret = HI_SUCCESS;
	HI_BOOL bBtm = 0;
	HI_U32  u32Vcnt = 0;

	memset((HI_VOID*)&unionZmeIn, 0, sizeof(HI_PQ_ZME_STRATEGY_IN_U));
	memset((HI_VOID*)&unionZmeOut, 0, sizeof(HI_PQ_ZME_STRATEGY_OUT_U));	

	if (pLayerCfgBasePara->enWBC == DRV_VDP_WBC_WIN)
	{
		if (pLayerCfgBasePara->bWbcEnable == HI_TRUE)
    	{
			
		   Internal_TransferWbcInfor(&unionZmeIn, pLayerCfgBasePara);
		   
            if (HI_SUCCESS != (s32Ret = DRV_PQ_GetVdpZmeStrategy (HI_PQ_DISP_WBC0_LAYER_ZME, &unionZmeIn, &unionZmeOut)))
            {
        		return s32Ret;
            }
			
            stWbcInfo.bZmeFirHL   = unionZmeOut.stStrategyWbcVp.bZmeFirHL;
			stWbcInfo.bZmeFirHC   = unionZmeOut.stStrategyWbcVp.bZmeFirHC;
			
            stWbcInfo.bZmeFirVL   = unionZmeOut.stStrategyWbcVp.bZmeFirVL;
			stWbcInfo.bZmeFirVC   = unionZmeOut.stStrategyWbcVp.bZmeFirVC;
			
            stWbcInfo.bZmeMedH   = unionZmeOut.stStrategyWbcVp.bZmeMedH; 
            stWbcInfo.bZmeMedV   = unionZmeOut.stStrategyWbcVp.bZmeMedV;
			stWbcInfo.bZmeDefault   = unionZmeOut.stStrategyWbcVp.bZmeDefault;

            /*we must get sd channel's top/bottom field, because hd may be progressive.*/
			VDP_DISP_GetVactState(1,&bBtm,&u32Vcnt);
			stWbcInfo.bZmeFrmFmtIn  = unionZmeOut.stStrategyWbcVp.stZme.bInFmt;
			stWbcInfo.bZmeFrmFmtOut = unionZmeOut.stStrategyWbcVp.stZme.bOutFmt;			
			stWbcInfo.bZmeBFIn      = 1 - bBtm;
			stWbcInfo.bZmeBFOut     = 1 - bBtm;
			
            stWbcInfo.enWbcVpConnPoint   = (unionZmeOut.stStrategyWbcVp.enVpPoint == HI_PQ_WBC_POINT_VP) ? VDP_LAYER_CONN_VPACSC : VDP_LAYER_CONN_VIDBZME; 
		    stWbcInfo.enInFmt =  (unionZmeOut.stStrategyWbcVp.stZme.u32PixInFmt == 0) ? VDP_VID_IFMT_SP_422 : VDP_VID_IFMT_SP_420;	
            stWbcInfo.bP2iEn   = unionZmeOut.stStrategyWbcVp.bOpenP2I;			
			
            stWbcInfo.stInRect.u32Wth = unionZmeOut.stStrategyWbcVp.stZme.u32FrmInWidth;
            stWbcInfo.stInRect.u32Hgt = unionZmeOut.stStrategyWbcVp.stZme.u32FrmInHeight;
            
            stWbcInfo.stOutRect.u32Wth = unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutWidth;
            stWbcInfo.stOutRect.u32Hgt = unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutHeight;
#if 0            
            VDP_PRINT("----Wbc Config:%d,%d,%d,%d,%d!\n", unionZmeOut.stStrategyWbcVp.stZme.u32FrmInWidth,
            	                              unionZmeOut.stStrategyWbcVp.stZme.u32FrmInHeight,
            	                              unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutWidth,
            	                              unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutHeight,
            	                              unionZmeOut.stStrategyWbcVp.enVpPoint);
											  
			 VDP_PRINT("Wbc Strategy:%d,%d,%d,%d!\n", unionZmeOut.stStrategyWbcVp.bReadFmtIn,
                 unionZmeOut.stStrategyWbcVp.stZme.bOutFmt,
                 bBtm,
                 stWbcInfo.enWbcVpConnPoint);

#endif
            
            stWbcInfo.enOutFmt  = (unionZmeOut.stStrategyWbcVp.stZme.u32PixOutFmt == 0) ? VDP_WBC_OFMT_SP422 : VDP_WBC_OFMT_SP420;
            stWbcInfo.enWbcVpConnPoint = (unionZmeOut.stStrategyWbcVp.enVpPoint == HI_PQ_WBC_POINT_VP) ? VDP_LAYER_CONN_VPACSC:VDP_LAYER_CONN_VIDBZME;
    
    		
        	stWbcInfo.stAddr[VDP_ADDR_WBCVP0].u32LumStr = pLayerCfgBasePara->stAddr.u32Stride_Y;
        	stWbcInfo.stAddr[VDP_ADDR_WBCVP0].u32ChmStr = pLayerCfgBasePara->stAddr.u32Stride_C;
        	
        	stWbcInfo.stAddr[VDP_ADDR_WBCVP0].u32LumAddr = pLayerCfgBasePara->stAddr.u32PhyAddr_Y;
        	stWbcInfo.stAddr[VDP_ADDR_WBCVP0].u32ChmAddr = pLayerCfgBasePara->stAddr.u32PhyAddr_C;

			switch(pLayerCfgBasePara->eFieldMode)
			{
				case CBB_FIELD_TOP:
					stWbcInfo.u32OutIop = VDP_RMODE_TOP;
					break;
				case CBB_FIELD_BOTTOM:
					stWbcInfo.u32OutIop = VDP_RMODE_BOTTOM;
					break;
				case CBB_FIELD_ALL:
					stWbcInfo.u32OutIop = VDP_RMODE_PROGRESSIVE;
					break;
				default:
					break;
			}			
			
    		stWbcInfo.bSmmu       = pLayerCfgBasePara->bSmmu;
    		stWbcInfo.bSecure     = pLayerCfgBasePara->bSecure;	
    	}
    		
    
    	stWbcInfo.bWbcLayerEn = pLayerCfgBasePara->bWbcEnable ; 
    	VDP_DRV_SetWbcLayer(VDP_LAYER_WBC_VP0, &stWbcInfo);
	}

	return s32Ret;
	
#else
    if (pLayerCfgBasePara->enWBC  == DRV_VDP_WBC_WIN)
    {
    	stWbcInfo.stInRect.u32Wth = pLayerCfgBasePara->stHdWinInputSize.s32Width;
    	stWbcInfo.stInRect.u32Hgt = pLayerCfgBasePara->stHdWinInputSize.s32Height;
		
    	stWbcInfo.stOutRect.u32Wth = pLayerCfgBasePara->stSdWinOutputSize.s32Width;
    	stWbcInfo.stOutRect.u32Hgt = pLayerCfgBasePara->stSdWinOutputSize.s32Height;
		
    	stWbcInfo.enOutFmt  = (pLayerCfgBasePara->eOutPutPixelFmt == CBB_VID_IFMT_SP_422) ? VDP_WBC_OFMT_SP422 : VDP_WBC_OFMT_SP422;
    	
    	stWbcInfo.stAddr[VDP_ADDR_WBCVP0].u32LumStr = pLayerCfgBasePara->stAddr.u32Stride_Y;
    	stWbcInfo.stAddr[VDP_ADDR_WBCVP0].u32ChmStr = pLayerCfgBasePara->stAddr.u32Stride_C;
    	
    	stWbcInfo.stAddr[VDP_ADDR_WBCVP0].u32LumAddr = pLayerCfgBasePara->stAddr.u32PhyAddr_Y;
    	stWbcInfo.stAddr[VDP_ADDR_WBCVP0].u32ChmAddr = pLayerCfgBasePara->stAddr.u32PhyAddr_C;
    	stWbcInfo.u32OutIop = (HI_U32)pLayerCfgBasePara->eFieldMode;
    	stWbcInfo.enWbcVpConnPoint = VDP_LAYER_CONN_VIDBZME;
    	stWbcInfo.bWbcLayerEn = pLayerCfgBasePara->bWbcEnable ; 
		stWbcInfo.bSmmu       = pLayerCfgBasePara->bSmmu;
		stWbcInfo.bSecure     = pLayerCfgBasePara->bSecure;	
    
    	VDP_DRV_SetWbcLayer(VDP_LAYER_WBC_VP0, &stWbcInfo);    	
    }

	return s32Ret;
#endif

return HI_SUCCESS;

}

HI_S32 CBB_Video_GetWbcPolicy(CBB_WBC_CFG_INFO_S *pLayerCfgBasePara,
                						  CBB_CFG_EXT_S *pVDPCfgExtPara,
            							  CBB_WBC_POLICY_S *pWbcPolicy)
{
	HI_PQ_ZME_STRATEGY_IN_U  unionZmeIn;
    HI_PQ_ZME_STRATEGY_OUT_U unionZmeOut;
	HI_S32 s32Ret = 0;
	VDP_LAYER_CONN_E			    enWbcVpConnPoint;
		
	memset((HI_VOID*)&unionZmeIn, 0, sizeof(HI_PQ_ZME_STRATEGY_IN_U));
	memset((HI_VOID*)&unionZmeOut, 0, sizeof(HI_PQ_ZME_STRATEGY_OUT_U));
	
	if (pLayerCfgBasePara->enWBC  == DRV_VDP_WBC_WIN)
	{		
        
		Internal_TransferWbcInfor(&unionZmeIn, pLayerCfgBasePara);
		
        if (HI_SUCCESS != (s32Ret = DRV_PQ_GetVdpZmeStrategy (HI_PQ_DISP_WBC0_LAYER_ZME, &unionZmeIn, &unionZmeOut)))
        {
    		return s32Ret;
        }
		
		pWbcPolicy->bWbcOutputFieldMode = unionZmeOut.stStrategyWbcVp.stZme.bOutFmt ? HI_FALSE:HI_TRUE;

		enWbcVpConnPoint = (unionZmeOut.stStrategyWbcVp.enVpPoint == HI_PQ_WBC_POINT_VP) ? VDP_LAYER_CONN_VPACSC:VDP_LAYER_CONN_VIDBZME;

		if (enWbcVpConnPoint == VDP_LAYER_CONN_VPACSC)
		{
			pWbcPolicy->enSrcColorSpace = pLayerCfgBasePara->enHdWinDestColorSpace;

			/* WBC0 no CSC*/
			pWbcPolicy->enDestColorSpace = pWbcPolicy->enSrcColorSpace;
			
			pWbcPolicy->stSdFinaleDispRect.s32X = 0;
			pWbcPolicy->stSdFinaleDispRect.s32Y = 0;
			pWbcPolicy->stSdFinaleDispRect.s32Width = unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutWidth;
			pWbcPolicy->stSdFinaleDispRect.s32Height = unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutHeight;
		}
		else
		{
			pWbcPolicy->enSrcColorSpace = pLayerCfgBasePara->enHdWinSrcColorSpace;
			/* WBC0 no CSC*/
			pWbcPolicy->enDestColorSpace = pWbcPolicy->enSrcColorSpace;
			
			pWbcPolicy->stSdFinaleDispRect.s32X = pLayerCfgBasePara->stSdWinOutputSize.s32X;
			pWbcPolicy->stSdFinaleDispRect.s32Y = pLayerCfgBasePara->stSdWinOutputSize.s32Y;
			pWbcPolicy->stSdFinaleDispRect.s32Width = unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutWidth;
			pWbcPolicy->stSdFinaleDispRect.s32Height = unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutHeight; 
		}

	}
	
	return HI_SUCCESS;	
}


HI_S32 CBB_Video_GetLayersCapability(CBB_VIDEO_LAYER_CAPABILITY_S *pstLayerCapability)
{    
	if(pstLayerCapability->eId == VDP_LAYER_VID0)
	{
		pstLayerCapability->bZme = HI_TRUE;
		pstLayerCapability->bZmePrior = HI_TRUE;
		pstLayerCapability->bTnrSnrDbDMDrDs = HI_TRUE;
		
		pstLayerCapability->bDcmp = HI_TRUE;	
		pstLayerCapability->stVideoMaxReadSize.s32Width = 4096;
		pstLayerCapability->stVideoMaxReadSize.s32Height = 2304;

		pstLayerCapability->u32BitWidth = 10;
	}
	
    if(pstLayerCapability->eId == VDP_LAYER_VID1)
	{
		pstLayerCapability->bZme = HI_FALSE;
		pstLayerCapability->bZmePrior = HI_FALSE;
		pstLayerCapability->bTnrSnrDbDMDrDs = HI_FALSE;
		
		pstLayerCapability->bDcmp = HI_FALSE;	
		pstLayerCapability->stVideoMaxReadSize.s32Width = 4096;
		pstLayerCapability->stVideoMaxReadSize.s32Height = 2304;

		/*in most conditions, v1 will be set to multiarea mode.*/
		pstLayerCapability->u32BitWidth = 8;
	}
	
	if(pstLayerCapability->eId == VDP_LAYER_VID3)
	{
		pstLayerCapability->bZme = HI_FALSE;
		pstLayerCapability->bZmePrior = HI_FALSE;
		pstLayerCapability->bTnrSnrDbDMDrDs = HI_FALSE;
		
		pstLayerCapability->bDcmp = HI_FALSE;	
		pstLayerCapability->stVideoMaxReadSize.s32Width = 720;
		pstLayerCapability->stVideoMaxReadSize.s32Height = 576;
		pstLayerCapability->u32BitWidth = 8;
	}
	
	if(pstLayerCapability->eId == VDP_LAYER_VID_BUTT)
	{
		pstLayerCapability->bZme = HI_TRUE;
		pstLayerCapability->bZmePrior = HI_TRUE;
		pstLayerCapability->bTnrSnrDbDMDrDs = HI_TRUE;
		
		pstLayerCapability->bDcmp = HI_TRUE;	
		pstLayerCapability->stVideoMaxReadSize.s32Width = 4096;
		pstLayerCapability->stVideoMaxReadSize.s32Height = 2304;

		pstLayerCapability->u32BitWidth = 10;
	}
	
	return HI_SUCCESS;
}


HI_BOOL VDP_CBB_Video_HalCheck(HI_U32 u32LayerNum)
{
	//to do...
	// use vdp_config_check()
	return HI_TRUE;
}

HI_S32 CBB_Video_Init(MemAllocateFunType mem_allocate_func_ptr, MemFreeFunType mem_free_func_ptr)
{
	if (VDP_DRV_AllocateMem(VDP_ALL_COEF_SIZE, &gstVdpCoefBufAddr.stBufBaseAddr))
	{
		return HI_FAILURE;
	}
	
	if (HI_FAILURE == VDP_DRV_IoRemap(SYS_CONTROL_VDP_LOCATION_SEL_, REG_IOREMAP_SIZE, &gu32TnrDnrLocationSel))
	{
		VDP_DRV_DeleteMem(&gstVdpCoefBufAddr.stBufBaseAddr);
		return HI_FAILURE;
	}
	
	VDP_DRV_CoefBufAddrDistribute(&gstVdpCoefBufAddr);
	return HI_SUCCESS;
}

HI_VOID CBB_Video_DeInit(HI_VOID)
{
	VDP_DRV_IoUnmap(&gu32TnrDnrLocationSel);
	gu32TnrDnrLocationSel = HI_NULL;
	
    VDP_DRV_DeleteMem(&gstVdpCoefBufAddr.stBufBaseAddr);
	return ;
}

HI_S32  CBB_Video_Set_SysInfo(CBB_SYS_INFO_S *pstSysInfo)
{
	return HI_SUCCESS;
}

HI_S32  CBB_Disp_Set_SysInfo(CBB_DISP_INFO_S *pstSysInfo)
{
	return HI_SUCCESS;
}

HI_VOID CBB_Video_SetLayerInit(HI_U32 u32LayerNum)
{
	return;
}

/*check whether the layer is the  initiated*/
HI_BOOL CBB_Video_GetLayerStatus(HI_U32 u32LayerNum)
{
	return HI_TRUE;
}

HI_VOID  CBB_Video_CheckRegConfig(HI_CHAR *pcheckItem)
{
	#ifndef __DISP_PLATFORM_BOOT__
	#endif
	
	return;
}

HI_VOID  CBB_Video_HalTestPatternInputPiture(CBB_TEST_PATTERN_S *pstTestPattern)
{
	return;
}






