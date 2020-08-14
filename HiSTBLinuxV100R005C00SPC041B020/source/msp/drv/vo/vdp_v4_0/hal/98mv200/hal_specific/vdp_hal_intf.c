#include "hi_type.h"
#include "vdp_drv_vid.h"
#include "vdp_drv_wbc.h"
#include "vdp_drv_comm.h"
#include "vdp_hal_intf.h"
#include "vdp_hal_chn.h"
#include "vdp_assert.h"

#define  SYS_CONTROL_VDP_LOCATION_SEL_  0xf8a20910
#define  REG_IOREMAP_SIZE               0x1000

extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;
HI_U32 *gu32TnrDnrLocationSel = HI_NULL;

#ifdef  VDP_PQ_STRATEGY
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
    pstVdpLayerAlgInfor->enLayerId     =  pLayerCfgBasePara->u32LayerNO;

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

#endif

static HI_S32 Internal_GatherLayerVariableConfig(CBB_LAYER_CFG_INFO_S *pLayerCfgBasePara,
                                                              VDP_LAYER_VID_INFO_S *pstVidInfo)
{
#ifdef  VDP_PQ_STRATEGY
    HI_PQ_ZME_STRATEGY_IN_U  pstZmeIn ;
    HI_PQ_ZME_STRATEGY_OUT_U pstZmeOut;

    HI_S32   s32Ret = HI_SUCCESS;
    HI_BOOL bBtm = HI_FALSE;
    HI_U32  u32Vcnt = 0;
    HI_PQ_VDP_WBC_INFO_S stVdpLayerAlgInfor;

    if (!pLayerCfgBasePara->bMuteEn && pLayerCfgBasePara->bRegionEn)
    {

        pstZmeIn.stLayerStrategy.u32FmtIn   = pLayerCfgBasePara->stFrame.eSrcFmt;
        pstZmeIn.stLayerStrategy.u32FrmWIn  = pLayerCfgBasePara->stCropRect.s32Width;
        pstZmeIn.stLayerStrategy.u32FrmHIn  = pLayerCfgBasePara->stCropRect.s32Height;

        // TODO:
        pLayerCfgBasePara->enLayerFrostMode = CBB_FROST_CLOSE;
        switch(pLayerCfgBasePara->enLayerFrostMode)
        {
            case CBB_FROST_LOW:
                pstZmeIn.stLayerStrategy.eFrostLevel = HI_PQ_FROST_LEVEL_LOW;
                break;

            case CBB_FROST_MIDDLE:
                pstZmeIn.stLayerStrategy.eFrostLevel = HI_PQ_FROST_LEVEL_MIDDLE;
                break;

            case CBB_FROST_HIGH:
                pstZmeIn.stLayerStrategy.eFrostLevel = HI_PQ_FROST_LEVEL_HIGH;
                break;
            case CBB_FROST_CLOSE:
            default:
                pstZmeIn.stLayerStrategy.eFrostLevel = HI_PQ_FROST_LEVEL_CLOSE;
                break;
        }

        pstZmeIn.stLayerStrategy.stHDCPAttestFactor.bHDCPScene = pLayerCfgBasePara->bHdcpDegradePQ;
        pstZmeIn.stLayerStrategy.stHDCPAttestFactor.u32IntermedWidth = pLayerCfgBasePara->stHdcpIntermediateRect.s32Width;
        pstZmeIn.stLayerStrategy.stHDCPAttestFactor.u32IntermedHeight = pLayerCfgBasePara->stHdcpIntermediateRect.s32Height;

        pstZmeIn.stLayerStrategy.u32FrmWOut = pLayerCfgBasePara->stVideoRect.s32Width;
        pstZmeIn.stLayerStrategy.u32FrmHOut = pLayerCfgBasePara->stVideoRect.s32Height;

        if ((pLayerCfgBasePara->u32LayerNO == VDP_LAYER_VID0)
           || (pLayerCfgBasePara->u32LayerNO == VDP_LAYER_VID1))
        {
            if (HI_SUCCESS != (s32Ret = DRV_PQ_GetVdpZmeStrategy ((HI_PQ_ZME_LAYER_E)pLayerCfgBasePara->u32LayerNO, &pstZmeIn, &pstZmeOut)))
            {
                return s32Ret;
            }

            pstVidInfo->enDrawMode = (HI_U32)pstZmeOut.stStrategyLayerId.eVScalerMode;

            /*only layer who has 2 scalers should set zme2. */
            if (pstZmeOut.stStrategyLayerId.u32ZmeNum == 2)
            {
                pstVidInfo->stZme2InRect.u32Wth  =  pstZmeOut.stStrategyLayerId.stZmeFmt[1].u32FrmInWidth;
                pstVidInfo->stZme2InRect.u32Hgt  =  pstZmeOut.stStrategyLayerId.stZmeFmt[1].u32FrmInHeight;
            }
            else
            {
                //VDP_PRINT("---line:%d:%d!\n", __LINE__, pstZmeOut.stStrategyLayerId.u32ZmeNum);
                pstVidInfo->stZme2InRect.u32Wth  =  pstZmeOut.stStrategyLayerId.stZmeFmt[0].u32FrmOutWidth;
                pstVidInfo->stZme2InRect.u32Hgt  =  pstZmeOut.stStrategyLayerId.stZmeFmt[0].u32FrmOutHeight;
            }

            Internal_GenateTnrDbDmInfor(&stVdpLayerAlgInfor, pLayerCfgBasePara);
            if (Internal_SelectTnrLocation(pLayerCfgBasePara->stAlgWbcInfor.enAlgIPSel))
            {
                return HI_FAILURE;
            }

            //DRV_PQ_SetVdpAlgCalcCfg(&stVdpLayerAlgInfor);
        }
        else
        {
            memset((HI_VOID*)&pstZmeOut, 0,sizeof(HI_PQ_ZME_STRATEGY_OUT_U));
            pstVidInfo->enDrawMode = VDP_DRAW_MODE_1;
            pstVidInfo->stZme2InRect.u32Wth  =  pLayerCfgBasePara->stVideoRect.s32Width;;
            pstVidInfo->stZme2InRect.u32Hgt  =  pLayerCfgBasePara->stVideoRect.s32Height;;
        }

        /*pass zme strategy to zme.*/
#ifndef VDP_ZME_COPY_MODE
        pstVidInfo->pstZmeDrvPara.bZmeFirHL = pstZmeOut.stStrategyLayerId.bZmeFirHL;
        pstVidInfo->pstZmeDrvPara.bZmeFirHC = pstZmeOut.stStrategyLayerId.bZmeFirHC;
        pstVidInfo->pstZmeDrvPara.bZmeFirVL = pstZmeOut.stStrategyLayerId.bZmeFirVL;
        pstVidInfo->pstZmeDrvPara.bZmeFirVC = pstZmeOut.stStrategyLayerId.bZmeFirVC;

        #if 0
        printk("%d-----:%d,%d,%d,%d!\n",  pLayerCfgBasePara->u32LayerNO,
                                       pstVidInfo->pstZmeDrvPara.bZmeFirHL,
                                       pstVidInfo->pstZmeDrvPara.bZmeFirHC,
                                       pstVidInfo->pstZmeDrvPara.bZmeFirVL,
                                       pstVidInfo->pstZmeDrvPara.bZmeFirVC);
        #endif

        pstVidInfo->pstZmeDrvPara.bZmeMedH  = pstZmeOut.stStrategyLayerId.bZmeMedH;
        pstVidInfo->pstZmeDrvPara.bZmeMedV  = pstZmeOut.stStrategyLayerId.bZmeMedV;
        pstVidInfo->pstZmeDrvPara.bZmeDefault   = pstZmeOut.stStrategyLayerId.bZmeDefault;

#ifdef VDP_V1_ZME_COPY_MODE
        if (VDP_LAYER_VID1 == pLayerCfgBasePara->u32LayerNO)
        {
            pstVidInfo->pstZmeDrvPara.bZmeFirHL     = HI_FALSE;
            pstVidInfo->pstZmeDrvPara.bZmeFirHC     = HI_FALSE;
            pstVidInfo->pstZmeDrvPara.bZmeFirVL     = HI_FALSE;
            pstVidInfo->pstZmeDrvPara.bZmeFirVC     = HI_FALSE;

            pstVidInfo->pstZmeDrvPara.bZmeMedH      = HI_FALSE;
            pstVidInfo->pstZmeDrvPara.bZmeMedV      = HI_FALSE;
            pstVidInfo->pstZmeDrvPara.bZmeDefault   = HI_FALSE;
        }
#endif

#else
        pstVidInfo->pstZmeDrvPara.bZmeFirHL = HI_FALSE;
        pstVidInfo->pstZmeDrvPara.bZmeFirHC = HI_FALSE;
        pstVidInfo->pstZmeDrvPara.bZmeFirVL = HI_FALSE;
        pstVidInfo->pstZmeDrvPara.bZmeFirVC = HI_FALSE;

        pstVidInfo->pstZmeDrvPara.bZmeMedH  = HI_FALSE;
        pstVidInfo->pstZmeDrvPara.bZmeMedV  = HI_FALSE;
        pstVidInfo->pstZmeDrvPara.bZmeDefault   = HI_FALSE;
#endif


        pstVidInfo->bHzmeEn = pstZmeOut.stStrategyLayerId.stPreZmeConfig.bPreZme;
        pstVidInfo->enHzmeMode = pstZmeOut.stStrategyLayerId.stPreZmeConfig.bPreZmeFir ? VDP_HZME_MODE_FIR:VDP_HZME_MODE_COPY;

        switch (pstZmeOut.stStrategyLayerId.eHScalerMode)
        {
            case HI_PQ_PREZME_HOR_2X:
                pstVidInfo->enDrawPixelMode = VDP_DRAW_PIXEL_MODE_2;
                break;
            case HI_PQ_PREZME_HOR_4X:
                pstVidInfo->enDrawPixelMode = VDP_DRAW_PIXEL_MODE_4;
                break;
            case HI_PQ_PREZME_HOR_8X:
                pstVidInfo->enDrawPixelMode = VDP_DRAW_PIXEL_MODE_8;
                break;
            case HI_PQ_PREZME_HOR_DISABLE:
            default:
                pstVidInfo->enDrawPixelMode = VDP_DRAW_PIXEL_MODE_1;
                break;
        }

        pstVidInfo->pstZmeDrvPara.bZmeFrmFmtIn  = pstZmeOut.stStrategyLayerId.bReadFmtIn;
        pstVidInfo->pstZmeDrvPara.bZmeFrmFmtOut   = (VDP_DISP_GetDispMode(0)==1)? 1 : 0;

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
            pstVidInfo->pstZmeDrvPara.bZmeBFOut         = !bBtm;
            pstVidInfo->pstZmeDrvPara.bZmeBFIn      = !bBtm;
        }
        else
        {
            pstVidInfo->pstZmeDrvPara.bZmeBFOut         = HI_FALSE;
            pstVidInfo->pstZmeDrvPara.bZmeBFIn      = pstZmeOut.stStrategyLayerId.bReadBFIn;
        }

        /* frame is interlace, this is for V3.*/
        if(pLayerCfgBasePara->stFrame.bFrameInterlace == HI_TRUE)
        {
            if(pLayerCfgBasePara->stFrame.eFieldMode == CBB_FIELD_TOP)
            {
                pstVidInfo->enReadMode           = VDP_RMODE_TOP;
            }
            else
            {
                pstVidInfo->enReadMode           = VDP_RMODE_BOTTOM;
            }
        }
        else
        {
            /*read frame.*/
            if (pstZmeOut.stStrategyLayerId.bReadFmtIn == HI_TRUE)
            {
                pstVidInfo->enReadMode  =  VDP_RMODE_PROGRESSIVE;
            }
            else
            {
                pstVidInfo->enReadMode  =  VDP_RMODE_INTERFACE;
            }
        }
    }
    else
    {
        pstVidInfo->pstZmeDrvPara.bZmeFrmFmtIn   = HI_TRUE;
        pstVidInfo->pstZmeDrvPara.bZmeFrmFmtOut   = HI_TRUE;
        pstVidInfo->pstZmeDrvPara.bZmeBFOut      = HI_FALSE;
        pstVidInfo->pstZmeDrvPara.bZmeBFIn       = HI_FALSE;

        pstVidInfo->stZme2InRect.u32Wth = pLayerCfgBasePara->stVideoRect.s32Width;
        pstVidInfo->stZme2InRect.u32Hgt  =  pLayerCfgBasePara->stVideoRect.s32Height;

        pstVidInfo->enDrawMode = VDP_DRAW_MODE_1;

        if(pLayerCfgBasePara->stFrame.bFrameInterlace == HI_TRUE)
        {
            if(pLayerCfgBasePara->stFrame.eFieldMode == CBB_FIELD_TOP)
            {
                pstVidInfo->enReadMode           = VDP_RMODE_TOP;
            }
            else
            {
                pstVidInfo->enReadMode           = VDP_RMODE_BOTTOM;
            }
        }
        else
        {
            if (pLayerCfgBasePara->stDispInfo.bInterlace == 1)
            {
                pstVidInfo->enReadMode  =  VDP_RMODE_INTERLACE;
            }
            else
            {
                pstVidInfo->enReadMode  =  VDP_RMODE_PROGRESSIVE;
            }
        }

    }
#else
    {
        // CBB  process  add by hyx
        pstVidInfo->pstZmeDrvPara.bZmeFrmFmtIn   = HI_TRUE;
        pstVidInfo->pstZmeDrvPara.bZmeFrmFmtOut   = HI_TRUE;
        pstVidInfo->pstZmeDrvPara.bZmeBFOut      = HI_FALSE;
        pstVidInfo->pstZmeDrvPara.bZmeBFIn       = HI_FALSE;

        pstVidInfo->enHzmeMode = VDP_HZME_MODE_COPY;
        pstVidInfo->enDrawPixelMode = VDP_DRAW_PIXEL_MODE_1;

        if (VDP_LAYER_VID0 == pLayerCfgBasePara->u32LayerNO)
        {
            pstVidInfo->bHzmeEn = HI_TRUE;
        }
        else
        {
            pstVidInfo->bHzmeEn = HI_FALSE;
        }

        pstVidInfo->stZme2InRect.u32Wth  =  pLayerCfgBasePara->stVideoRect.s32Width;
        pstVidInfo->stZme2InRect.u32Hgt  =  pLayerCfgBasePara->stVideoRect.s32Height;

        pstVidInfo->enDrawMode = VDP_DRAW_MODE_1;

        if(pLayerCfgBasePara->stFrame.bFrameInterlace == HI_TRUE)
        {
            if(pLayerCfgBasePara->stFrame.eFieldMode == CBB_FIELD_TOP)
            {
                pstVidInfo->enReadMode           = VDP_RMODE_TOP;
            }
            else
            {
                pstVidInfo->enReadMode           = VDP_RMODE_BOTTOM;
            }
        }
        else
        {
             pstVidInfo->enReadMode  =  VDP_RMODE_PROGRESSIVE;
        }

    }
#endif
    return HI_SUCCESS;
}


HI_S32 CBB_Video_SetLayerParam(CBB_LAYER_CFG_INFO_S *pLayerCfgBasePara,CBB_CFG_EXT_S *pVDPCfgExtPara)
{
    VDP_LAYER_VID_INFO_S stVidInfo;

    memset((HI_VOID  *)&stVidInfo,0,sizeof(VDP_LAYER_VID_INFO_S));
    if ((pLayerCfgBasePara->u32LayerNO >= VDP_LAYER_VID4) || (pLayerCfgBasePara->u32RegionNO >= 16))
    {
        return HI_FAILURE;
    }

    memset((HI_VOID*)&stVidInfo, 0, sizeof(VDP_LAYER_VID_INFO_S));
    stVidInfo.u32RegionNO             =  pLayerCfgBasePara->u32RegionNO;
    stVidInfo.u32LayerZorder          =  pLayerCfgBasePara->u32LayerZorder;
    stVidInfo.bRegionEn               =  pLayerCfgBasePara->bRegionEn;
    stVidInfo.bRegionModeEn           =  pLayerCfgBasePara->bRegionModeEn;

    if (stVidInfo.bRegionEn == HI_TRUE)
    {
        (HI_VOID)Internal_GatherLayerVariableConfig(pLayerCfgBasePara, &stVidInfo);

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


        stVidInfo.enOutClrSpace = COLOR_SPACE_TRANSFER(pLayerCfgBasePara->enOutColorSpace);
        stVidInfo.enInClrSpace = COLOR_SPACE_TRANSFER(pLayerCfgBasePara->stFrame.enSrcColorSpace);

        stVidInfo.stSrcInfo.enSrcClrSpace         = stVidInfo.enInClrSpace ;//to do
        stVidInfo.bMuteEn              = pLayerCfgBasePara->bMuteEn;
        stVidInfo.stMuteColor.u32BkgY  = 0x40;
        stVidInfo.stMuteColor.u32BkgU  = 0x200;
        stVidInfo.stMuteColor.u32BkgV  = 0x200;
        stVidInfo.stMuteColor.u32BkgA  = 0xff;

        stVidInfo.stLetterBoxColor     = stVidInfo.stMuteColor;
        stVidInfo.stMixvColor          = stVidInfo.stMuteColor;

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


        stVidInfo.stSrcInfo.enInFmt = PIXCEL_FMT_TRANSFER(pLayerCfgBasePara->stFrame.eSrcFmt);

        if(pLayerCfgBasePara->stFrame.enSrcBitWidth == CBB_PIXEL_BITWIDTH_8BIT)
        {
            stVidInfo.stSrcInfo.enDataWidth           = VDP_DATA_WTH_8;
        }
        else if(pLayerCfgBasePara->stFrame.enSrcBitWidth == CBB_PIXEL_BITWIDTH_10BIT)
        {
            stVidInfo.stSrcInfo.enDataWidth           = VDP_DATA_WTH_10;
        }
        else if(pLayerCfgBasePara->stFrame.enSrcBitWidth == CBB_PIXEL_BITWIDTH_10BIT_CTS)
        {
            stVidInfo.stSrcInfo.enDataWidth           = VDP_DATA_WTH_10_CTS;
        }

        stVidInfo.stSrcInfo.stSrcRect.u32X      = (HI_U32)(pLayerCfgBasePara->stFrame.stSrcRect.s32X);
        stVidInfo.stSrcInfo.stSrcRect.u32Y      = (HI_U32)(pLayerCfgBasePara->stFrame.stSrcRect.s32Y);
        stVidInfo.stSrcInfo.stSrcRect.u32Wth    = pLayerCfgBasePara->stFrame.stSrcRect.s32Width;
        stVidInfo.stSrcInfo.stSrcRect.u32Hgt    = pLayerCfgBasePara->stFrame.stSrcRect.s32Height;

        stVidInfo.stSrcInfo.bSrcIsInterlace       = pLayerCfgBasePara->stFrame.bFrameInterlace;

        //For V1
        stVidInfo.bRegionModeEn = pLayerCfgBasePara->bRegionModeEn;
        stVidInfo.u32RegionNO   = pLayerCfgBasePara->u32RegionNO  ;
        stVidInfo.bRegionEn     = pLayerCfgBasePara->bRegionEn    ;


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

    VDP_DRV_SetVidLayer(pLayerCfgBasePara->u32LayerNO, &stVidInfo);

    return HI_SUCCESS;
}


HI_VOID CBB_Video_SetLayerEnable(HI_U32  u32LayerNO,
                                       HI_U32   u32RegionNO,
                                       HI_U32   bEnable)
{
#if 0
    if ((u32LayerNO == VDP_LAYER_VID0) || (u32LayerNO == VDP_LAYER_VID3))
    {
        VDP_VID_SetLayerEnable (u32LayerNO, bEnable);

    }
    else if (u32LayerNO == VDP_LAYER_VID1)
    {
        VDP_VID_SetRegionEnable(u32LayerNO, u32RegionNO, bEnable);

        if (HI_FALSE == VDP_VID_CheckRegionState(u32LayerNO))
        {
            VDP_VID_SetLayerEnable (u32LayerNO, HI_FALSE);
            VDP_VID_SetAllRegionDisable(u32LayerNO);
        }
        else
        {
            VDP_VID_SetLayerEnable (u32LayerNO, HI_TRUE);
        }
    }

    VDP_HDR_SetRegUp();
    VDP_VID_SetRegUp(u32LayerNO);
#endif
    return;
}


HI_S32 CBB_Video_SetWbcParam(CBB_WBC_CFG_INFO_S *pLayerCfgBasePara,CBB_CFG_EXT_S *pVDPCfgExtPara)
{
    VDP_LAYER_WBC_INFO_S stWbcInfo = {0};
    HI_S32   s32Ret = HI_SUCCESS;

#ifdef  VDP_PQ_STRATEGY
    HI_PQ_ZME_STRATEGY_IN_U  unionZmeIn;
    HI_PQ_ZME_STRATEGY_OUT_U unionZmeOut;

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

#ifndef VDP_ZME_COPY_MODE

            stWbcInfo.bZmeFirHL   = unionZmeOut.stStrategyWbcVp.bZmeFirHL;
            stWbcInfo.bZmeFirHC   = unionZmeOut.stStrategyWbcVp.bZmeFirHC;

            stWbcInfo.bZmeFirVL   = unionZmeOut.stStrategyWbcVp.bZmeFirVL;
            stWbcInfo.bZmeFirVC   = unionZmeOut.stStrategyWbcVp.bZmeFirVC;

            stWbcInfo.bZmeMedH   = unionZmeOut.stStrategyWbcVp.bZmeMedH;
            stWbcInfo.bZmeMedV   = unionZmeOut.stStrategyWbcVp.bZmeMedV;
            stWbcInfo.bZmeDefault   = unionZmeOut.stStrategyWbcVp.bZmeDefault;
#else

            stWbcInfo.bZmeFirHL   = HI_FALSE;
            stWbcInfo.bZmeFirHC   = HI_FALSE;

            stWbcInfo.bZmeFirVL   = HI_FALSE;
            stWbcInfo.bZmeFirVC   = HI_FALSE;

            stWbcInfo.bZmeMedH   = HI_FALSE;
            stWbcInfo.bZmeMedV   = HI_FALSE;
            stWbcInfo.bZmeDefault   = HI_FALSE;
#endif

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

        stWbcInfo.enWbcVpConnPoint = VDP_LAYER_CONN_VPACSC;
        stWbcInfo.bWbcLayerEn = pLayerCfgBasePara->bWbcEnable ;
        stWbcInfo.bSmmu       = pLayerCfgBasePara->bSmmu;
        stWbcInfo.bSecure     = pLayerCfgBasePara->bSecure;

        VDP_DRV_SetWbcLayer(VDP_LAYER_WBC_VP0, &stWbcInfo);
    }

    return s32Ret;
#endif

return HI_SUCCESS;

}


#ifndef __DISP_PLATFORM_BOOT__
HI_S32 CBB_Video_GetWbcPolicy(CBB_WBC_CFG_INFO_S *pLayerCfgBasePara,
                                          CBB_CFG_EXT_S *pVDPCfgExtPara,
                                          CBB_WBC_POLICY_S *pWbcPolicy)
{
    HI_PQ_ZME_STRATEGY_IN_U  unionZmeIn;
    HI_PQ_ZME_STRATEGY_OUT_U unionZmeOut;
    HI_S32 s32Ret = 0;
    VDP_LAYER_CONN_E                enWbcVpConnPoint;

    memset((HI_VOID*)&unionZmeIn, 0, sizeof(HI_PQ_ZME_STRATEGY_IN_U));
    memset((HI_VOID*)&unionZmeOut, 0, sizeof(HI_PQ_ZME_STRATEGY_OUT_U));

    if (pLayerCfgBasePara->enWBC  == DRV_VDP_WBC_WIN)
    {

        Internal_TransferWbcInfor(&unionZmeIn, pLayerCfgBasePara);

        if (HI_SUCCESS != (s32Ret = DRV_PQ_GetVdpZmeStrategy (HI_PQ_DISP_WBC0_LAYER_ZME, &unionZmeIn, &unionZmeOut)))
        {
            return s32Ret;
        }

        //printk("zme-%d - wh(%d,%d)\n",unionZmeOut.stStrategyWbcVp.enVpPoint,unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutWidth,unionZmeOut.stStrategyWbcVp.stZme.u32FrmOutHeight);
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

#endif

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

#ifdef  VDP_PQ_STRATEGY
    if (HI_FAILURE == VDP_DRV_IoRemap(SYS_CONTROL_VDP_LOCATION_SEL_, REG_IOREMAP_SIZE, &gu32TnrDnrLocationSel))
    {
        VDP_DRV_DeleteMem(&gstVdpCoefBufAddr.stBufBaseAddr);
        return HI_FAILURE;
    }
#endif

    VDP_DRV_CoefBufAddrDistribute(&gstVdpCoefBufAddr);
    return HI_SUCCESS;
}

HI_VOID CBB_Video_DeInit(HI_VOID)
{
    #ifdef  VDP_PQ_STRATEGY
    VDP_DRV_IoUnmap(&gu32TnrDnrLocationSel);
    gu32TnrDnrLocationSel = HI_NULL;
    #endif

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
    VDP_ConfigCheck(HI_TRUE);
    #endif
    return;
}

HI_VOID  CBB_Video_HalTestPatternInputPiture(CBB_TEST_PATTERN_S *pstTestPattern)
{
    return;
}






