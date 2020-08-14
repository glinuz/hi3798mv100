
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_bufcore.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2014/04/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "drv_win_policy.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define _SR_OPEN_


#ifndef _VDP_CHIP_ALG_DEAL_CONSISTENCE
static int caculate_sr_size(HI_RECT_S *pstFrameRect,
               HI_RECT_S *pstOutRect,
               HI_RECT_S *pstSrRect,
               HI_U32    u32WinNum,
               HI_BOOL *pbHorSrEnable,
               HI_BOOL *pbVerSrEnable,
               HI_RECT_S *pstFmtResolution,
               HI_BOOL bExistScaleDown,
               HI_DRV_DISP_STEREO_E enStereo)
{
    HI_CHIP_TYPE_E   enChipType;
    HI_CHIP_VERSION_E enChipVersion;

#ifndef _SR_OPEN_
    return HI_FALSE;
#endif

    /*no matter 98m or 98c, when winnum >2, sr should not opened.*/
    if ((u32WinNum >= 2) || (bExistScaleDown) || (enStereo != DISP_STEREO_NONE))
    {
        return HI_FALSE;
    }

    /*currently,only 4k support sr.*/
    if (!((pstFmtResolution->s32Width == 3840)
           &&(pstFmtResolution->s32Height == 2160))
       )
    {
        return HI_FALSE;
    }

    if ((pstFrameRect->s32Width*2) <= pstOutRect->s32Width)
    {
        *pbHorSrEnable = HI_TRUE;
        pstSrRect->s32Width =  pstOutRect->s32Width/2;
    }

    if ((pstFrameRect->s32Height*2) <= pstOutRect->s32Height)
    {
        *pbVerSrEnable = HI_TRUE;
        pstSrRect->s32Height =  pstOutRect->s32Height/2;
    }

    /*the limit of sr is: max input size is 2k*2k,
     * if larger than this, and make a zme even in vertical direction, error happens.
     * so we should turn sr off, the recover the value.
     */
    if ((*pbVerSrEnable) || (*pbHorSrEnable))
    {
        if ((pstSrRect->s32Width > 1920)
            || (pstSrRect->s32Height > 2160)
            || !(*pbVerSrEnable && *pbHorSrEnable)
           )
        {
            *pbHorSrEnable = HI_FALSE;
            *pbVerSrEnable = HI_FALSE;
            pstSrRect->s32Width =  pstOutRect->s32Width;
            pstSrRect->s32Height =  pstOutRect->s32Height;
        }
    }

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

    /*when 3798m added, this branch should be opened.*/
    if (0)
    {
        if ((pstFrameRect->s32Width  > 1920) && (pstOutRect->s32Width > 1920))
        {
            *pbHorSrEnable = HI_TRUE;
            pstSrRect->s32Width =  pstOutRect->s32Width/2;
        }
    }

    return HI_SUCCESS;
}
#else

#endif

static  HI_VOID generate_FrameRect(WINDOW_S *pstWin,
                                   HI_RECT_S *pstSourceFrameRect,
                                   HI_BOOL *pbExistScaleDown,
                                   HI_U32 u32LayerSupportWidthMax,
                                   HI_U32 u32LayerSupportHeightMax)
{
    HI_DRV_VIDEO_FRAME_S *pstDispFrame = HI_NULL;
    HI_DRV_VIDEO_PRIVATE_S *pstPrivInfo = HI_NULL;

    *pbExistScaleDown = HI_FALSE;

    if (!pstWin->stBuffer.stWinBP.pstDisplay)
        goto __error;

    pstDispFrame = (HI_DRV_VIDEO_FRAME_S *)pstWin->stBuffer.stWinBP.pstDisplay->u32Data;
    if (!pstDispFrame)
        goto __error;

    pstPrivInfo = (HI_DRV_VIDEO_PRIVATE_S *)(pstDispFrame->u32Priv);
    if (!pstPrivInfo)
        goto __error;

    pstSourceFrameRect->s32X = 0;
    pstSourceFrameRect->s32Y= 0;
    if (pstDispFrame)
    {
        pstSourceFrameRect->s32Width  = pstPrivInfo->stVideoOriginalInfo.u32Width;
        pstSourceFrameRect->s32Height = pstPrivInfo->stVideoOriginalInfo.u32Height;
    }

    if ((pstDispFrame->stLbxInfo.s32Width  < pstSourceFrameRect->s32Width)
        || (pstDispFrame->stLbxInfo.s32Height  < pstSourceFrameRect->s32Height))
    {
        *pbExistScaleDown = HI_TRUE;
    }

    pstWin->stWinInfoForDeveloper.stOringinFrameSize = *pstSourceFrameRect;
    pstWin->stWinInfoForDeveloper.bExistScaleDown_WhenRatioRevise = *pbExistScaleDown;

    return;
__error:

    pstSourceFrameRect->s32Width  = u32LayerSupportWidthMax;
    pstSourceFrameRect->s32Height = u32LayerSupportHeightMax;
    return;
}


static int pre_ScalerDistribute(HI_RECT_S *pstVpssZmeSize,
                                 HI_RECT_S *pstFinalZmeSize,
                                 HI_U32  u32WinNum,
                                 HI_BOOL *pbHorSrEnable,
                                 HI_BOOL *pbVerSrEnable,
                                 HI_BOOL  bZmeCapability,
                                 HI_BOOL bZmePriorCapability,
                                 HI_BOOL  bSrCapability,
                                 HI_U32   u32LayerMAXWidth,
                                 HI_U32   u32LayerMAXHeight,
                                 HI_RECT_S *pstSourceFrameRect,
                                 HI_RECT_S *pstFmtResolution,
                                 HI_BOOL bExistScaleDown,
                                 HI_DRV_DISP_STEREO_E enStereo)
{    
    HI_BOOL bHorSrEnable = HI_FALSE;
    HI_BOOL bVerSrEnable = HI_FALSE;
	
	
    /*if no zme exist, the size must be limited to the video capability.*/
    if (!bZmeCapability)
    {
        if (pstFinalZmeSize->s32Width > u32LayerMAXWidth)
        {
            pstVpssZmeSize->s32Width  = u32LayerMAXWidth;
        }
		
        if (pstFinalZmeSize->s32Height > u32LayerMAXHeight)
        {
            pstVpssZmeSize->s32Height = u32LayerMAXHeight;
        }

    }
    else
    {		
        if (!bSrCapability)
        {
			HI_RECT_S stExpectVpssOutputSize;
			
			if (bZmePriorCapability  == HI_TRUE)
			{
				stExpectVpssOutputSize = *pstSourceFrameRect;
			}
			else
			{
				stExpectVpssOutputSize = *pstFinalZmeSize;
			}
			
            /*if no sr, such as cv200,  should consider the video capability,
                    * and  the aspect ratio should be kept.
                    */
            if((stExpectVpssOutputSize.s32Width > u32LayerMAXWidth)||(stExpectVpssOutputSize.s32Height > u32LayerMAXHeight))
            {
                if(u32LayerMAXWidth * stExpectVpssOutputSize.s32Height > u32LayerMAXHeight * stExpectVpssOutputSize.s32Width)
                {
                    pstVpssZmeSize->s32Width = (stExpectVpssOutputSize.s32Width*u32LayerMAXHeight/stExpectVpssOutputSize.s32Height)&HI_WIN_OUT_RECT_WIDTH_ALIGN;
                    pstVpssZmeSize->s32Height = u32LayerMAXHeight;
                }
                else
                {
                    pstVpssZmeSize->s32Height = (stExpectVpssOutputSize.s32Height*u32LayerMAXWidth/stExpectVpssOutputSize.s32Width)&HI_WIN_OUT_RECT_HEIGHT_ALIGN;
                    pstVpssZmeSize->s32Width = u32LayerMAXWidth;
                }
            }
			else
			{
				pstVpssZmeSize->s32Height = stExpectVpssOutputSize.s32Height;
                pstVpssZmeSize->s32Width  = stExpectVpssOutputSize.s32Width;
			}			
        }
        else
        {
			HI_RECT_S stVpssZmeSize;
			stVpssZmeSize = *pstFinalZmeSize;
			
            /* if SR supported, to judge to  open SR or not.
			  * if SR enabled, the size should be divided  into  2 parts bettween VPSS and vdp.
                     */
            caculate_sr_size(pstSourceFrameRect,
                            pstFinalZmeSize,
                            &stVpssZmeSize,
                            u32WinNum,
                            &bHorSrEnable,
                            &bVerSrEnable,
                            pstFmtResolution,
                            bExistScaleDown,
                            enStereo);

            /*when sr enable ,but the 1/2 size is larger than video layer cap, do a scale down.*/
            if((stVpssZmeSize.s32Width > u32LayerMAXWidth)||(stVpssZmeSize.s32Height > u32LayerMAXHeight))
            {
                /*when sr enable,but 1/2 size  larger than max, the vpss zme size should be calculated again.*/
                if(u32LayerMAXWidth * pstFinalZmeSize->s32Height > u32LayerMAXHeight * pstFinalZmeSize->s32Width)
                {
                    pstVpssZmeSize->s32Width = (pstFinalZmeSize->s32Width*u32LayerMAXHeight/pstFinalZmeSize->s32Height)&HI_WIN_OUT_RECT_WIDTH_ALIGN;
                    pstVpssZmeSize->s32Height = u32LayerMAXHeight;
                }
                else
                {
                    pstVpssZmeSize->s32Height = (pstFinalZmeSize->s32Height*u32LayerMAXWidth/pstFinalZmeSize->s32Width)&HI_WIN_OUT_RECT_HEIGHT_ALIGN;
                    pstVpssZmeSize->s32Width = u32LayerMAXWidth;
                }

                /*no matter how, sr should be disabled.*/
                bHorSrEnable = HI_FALSE;
                bVerSrEnable = HI_FALSE;
            }
            else
            {
                pstVpssZmeSize->s32Width = stVpssZmeSize.s32Width;
                pstVpssZmeSize->s32Height = stVpssZmeSize.s32Height;
            }
        }		
    }

    if ((!pstVpssZmeSize->s32Height) || (!pstVpssZmeSize->s32Width))
    {
        pstVpssZmeSize->s32Width =   VIDEO_LAYER_SUPPORT_MIN_WIDTH;
        pstVpssZmeSize->s32Height =  VIDEO_LAYER_SUPPORT_MIN_HEIGHT;
    }

    pstVpssZmeSize->s32Width  &=  HI_WIN_OUT_RECT_WIDTH_ALIGN;
    pstVpssZmeSize->s32Height &=  HI_WIN_OUT_RECT_HEIGHT_ALIGN;

    *pbHorSrEnable  = bHorSrEnable;
    *pbVerSrEnable  = bVerSrEnable;

    return HI_SUCCESS;
}


/*principly, we only use vpss to zme out or in.  but video layer
 * only support a limit input size.  so in large scale zme, we should
 * do a 2-class zme: vpss and vdp both do scaler.
 */
int Win_Pre_ScalerDistribute(WINDOW_S *pstWin,
                             HI_RECT_S *pstVpssZmeSize,
                             HI_RECT_S *pstFinalZmeSize,
                             HI_U32 u32WinNum,
                             HI_BOOL *pbHorSrEnable,
                             HI_BOOL *pbVerSrEnable,
                             HI_RECT_S *pstFmtResolution,
                             HI_DRV_DISP_STEREO_E enStereo)
{
    unsigned int u32LayerSupportWidthMax = 0, u32LayerSupportHeightMax = 0;
    VIDEO_LAYER_CAPABILITY_S  stVideoLayerCap;
    HI_RECT_S stSourceFrameRect;

    HI_BOOL bZmeCapability			 = HI_FALSE;
	HI_BOOL bZmePriorCapability      = HI_FALSE;
    HI_BOOL bSrCapability			 = HI_FALSE;
	
    HI_S32 ret = 0;
    HI_BOOL bExistScaleDown = HI_FALSE;
	HI_RECT_S stVDP_FinalZmeSize = *pstFinalZmeSize;

    if ((ret = pstWin->stVLayerFunc.PF_GetCapability(pstWin->u32VideoLayer,&stVideoLayerCap)))
    {
        return ret;
    }

    bZmeCapability = stVideoLayerCap.bZme;
	bZmePriorCapability      = stVideoLayerCap.bZmePrior;
    bSrCapability = stVideoLayerCap.bSR;

    u32LayerSupportWidthMax = stVideoLayerCap.u32LayerWidthMax;
    u32LayerSupportHeightMax = stVideoLayerCap.u32LayerHeightMax;
	
	/*get the source frame.*/
    memset((void*)&stSourceFrameRect, 0, sizeof(HI_RECT_S));
    generate_FrameRect(pstWin, 
					   &stSourceFrameRect,
					   &bExistScaleDown,
					   u32LayerSupportWidthMax,
					   u32LayerSupportHeightMax);

	pre_ScalerDistribute(pstVpssZmeSize,
                    &stVDP_FinalZmeSize,
                    u32WinNum,
                    pbHorSrEnable,
                    pbVerSrEnable,
                    bZmeCapability,
                    bZmePriorCapability,
                    bSrCapability,
                    u32LayerSupportWidthMax,
                    u32LayerSupportHeightMax,
                    &stSourceFrameRect,
                    pstFmtResolution,
                    bExistScaleDown,
                    enStereo);

	
    return HI_SUCCESS;
}


/*if  outrect/inrect > 2, the primary condition of sr's openning is satified.
 *but vpss does not give a size vdp want, so vdp should give a second scaler.
 */
HI_S32 post_ScalerProcess(HI_RECT_S *pstFinalDisPosition,
                        HI_RECT_S *pstV0DisPosition,
                        HI_BOOL   *pbHorSrEnable,
                        HI_BOOL   *pbVerSrEnable,
                        HI_U32    u32WinNum,
                        HI_RECT_S *pstSourceFrameRect,
                        HI_CHIP_TYPE_E   enChipType,
                        HI_BOOL bSrCapability,
                        const HI_RECT_S *pstFmtResolution,
                        HI_DRV_DISP_STEREO_E enStereo)
{

    *pbHorSrEnable = HI_FALSE;
    *pbVerSrEnable = HI_FALSE;

    if ((u32WinNum  < 2) && (enStereo == DISP_STEREO_NONE))
    {
        if ((pstSourceFrameRect->s32Width > 1920) || (pstSourceFrameRect->s32Height > 2160))
        {
            *pbHorSrEnable = HI_FALSE;
            *pbVerSrEnable = HI_FALSE;
        }
        else if (!((pstFmtResolution->s32Width == 3840)
           &&(pstFmtResolution->s32Height == 2160))
                )
        {
            *pbHorSrEnable = HI_FALSE;
            *pbVerSrEnable = HI_FALSE;
        }
        else
        {
            if (((pstSourceFrameRect->s32Width*2) <= pstFinalDisPosition->s32Width)
                && (bSrCapability))
            {
                *pbHorSrEnable = HI_TRUE;
            }

            if (((pstSourceFrameRect->s32Height*2) <= pstFinalDisPosition->s32Height)
                 && (bSrCapability))
            {
                *pbVerSrEnable = HI_TRUE;
            }
        }

        /*both h and vertical meets sr, then open sr, else close it.*/
        if (!(*pbHorSrEnable && *pbVerSrEnable))
        {
            *pbHorSrEnable = HI_FALSE;
            *pbVerSrEnable = HI_FALSE;
        }
    }

#ifndef _SR_OPEN_
    *pbHorSrEnable = HI_FALSE;
    *pbVerSrEnable = HI_FALSE;
#endif


    /*zme has no x/y,it's just a size , but position has x/y,
     *so we should not put them together.when SR in Vp and enabled, the position which V0 care
     *should  be relocated.
     */
    if ((HI_CHIP_TYPE_HI3798C == enChipType) || (HI_CHIP_TYPE_HI3796C == enChipType))
    {
        *pstV0DisPosition = *pstFinalDisPosition;

        if (*pbHorSrEnable)
        {
            pstV0DisPosition->s32X = pstFinalDisPosition->s32X / 2;
            pstV0DisPosition->s32Width = pstFinalDisPosition->s32Width / 2;
        }

        if (*pbVerSrEnable)
        {
            pstV0DisPosition->s32Y = pstFinalDisPosition->s32Y / 2;
            pstV0DisPosition->s32Height = pstFinalDisPosition->s32Height / 2;
        }
    }
    else
    {
        /*because SR in v0 or no SR, so  v0 positon == final position.*/
        /*98m sr is in v0, so v0 output position remain the same with cv200.*/
        *pstV0DisPosition = *pstFinalDisPosition;
    }

    pstV0DisPosition->s32X &=  HI_WIN_OUT_RECT_X_ALIGN;
    pstV0DisPosition->s32Y &=  HI_WIN_OUT_RECT_Y_ALIGN;
    pstV0DisPosition->s32Width &=  HI_WIN_OUT_RECT_WIDTH_ALIGN;
    pstV0DisPosition->s32Height &=  HI_WIN_OUT_RECT_HEIGHT_ALIGN;

    return HI_SUCCESS;
}

int Win_Post_ScalerProcess( WINDOW_S  *pstWin,
                            HI_RECT_S *pstFinalDisPosition,
                            HI_RECT_S *pstV0DisPosition,
                            HI_BOOL   *pbHorSrEnable,
                            HI_BOOL   *pbVerSrEnable,
                            HI_U32 u32WinNum,
                            HI_RECT_S *pstSourceFrameRect,
                            const HI_RECT_S *pstFmtResolution,
                            HI_DRV_DISP_STEREO_E enStereo)
{
    HI_CHIP_TYPE_E   enChipType;
    HI_CHIP_VERSION_E enChipVersion;
    VIDEO_LAYER_CAPABILITY_S  stVideoLayerCap;
    HI_BOOL bSrCapability  = HI_FALSE;
    HI_S32 ret = 0;

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

    if ((ret = pstWin->stVLayerFunc.PF_GetCapability(pstWin->u32VideoLayer,&stVideoLayerCap)))
    {
        return ret;
    }

    bSrCapability = stVideoLayerCap.bSR;
    post_ScalerProcess(pstFinalDisPosition,
                       pstV0DisPosition,
                       pbHorSrEnable,
                       pbVerSrEnable,
                       u32WinNum,
                       pstSourceFrameRect,
                       enChipType,
                       bSrCapability,
                       pstFmtResolution,
                       enStereo);

    return HI_SUCCESS;
}

static HI_VOID updateDci(WINDOW_S *pstWin,
                      HI_BOOL bDciEnable,
                      HI_PQ_DCI_WIN_S *pstActualDciArea)
{
    if (pstWin->enDisp == HI_DRV_DISPLAY_1)
    {
        /*this flag is not a control of dci,
			 *just if dci is open ,then make a choice.
			 */
        DRV_PQ_UpdateDCIWin(pstActualDciArea, bDciEnable);
    }

    /*save the value for next judgement.*/
    pstWin->stMiscInfor.stWinDciLastConfig = *pstActualDciArea;
    pstWin->stMiscInfor.bWinDciEnableLastConfig = bDciEnable;

    return;
}

HI_S32 dci_enable_policy(HI_RECT_S *pstOriginDciArea,/*vpss's effective area.*/
                         HI_RECT_S *pstV0DisPosition,/*secondary coordinate*/
                         HI_RECT_S *pstVpssGive,/*the original rect give by vpss.*/
                         HI_U32    u32WinNum,
                         HI_BOOL  *pbDciEnable,
                         HI_PQ_DCI_WIN_S *pstActualDciArea)
{
    HI_U32 u32OriginDCI_Width = 0;
    HI_U32 u32OriginDCI_Height = 0;

    HI_U32 u32SecondCoord_DCI_X_Start = 0;
    HI_U32 u32SecondCoord_DCI_X_End = 0;

    HI_U32 u32SecondCoord_DCI_Y_Start = 0;
    HI_U32 u32SecondCoord_DCI_Y_End = 0;

    *pbDciEnable = HI_FALSE;

    /*when win >=2, we only cover the window on V0, 2014-0910,
        *discussed by zk and zlling.*/
#if 1
    if (u32WinNum >= 2)
    {
        memset((void*)pstActualDciArea, 0, sizeof(HI_PQ_DCI_WIN_S));
        return HI_SUCCESS;
    }
#endif

    u32OriginDCI_Width = pstOriginDciArea->s32Width;
    u32OriginDCI_Height = pstOriginDciArea->s32Height;


    /*although 98m different with 98c, but we both use pstV0DisPosition, not final.
     * the final and v0's calculation is done in Win_Post_ScalerProcess*/
    /*secondly, caculate the new dci coordinate in the secondary coordinate.*/
    u32SecondCoord_DCI_X_Start = (pstOriginDciArea->s32X * pstV0DisPosition->s32Width) / pstVpssGive->s32Width;
    u32SecondCoord_DCI_Y_Start = (pstOriginDciArea->s32Y * pstV0DisPosition->s32Height) / pstVpssGive->s32Height;

    u32SecondCoord_DCI_X_End = u32SecondCoord_DCI_X_Start  +
                (u32OriginDCI_Width *  pstV0DisPosition->s32Width) / pstVpssGive->s32Width;

    u32SecondCoord_DCI_Y_End = u32SecondCoord_DCI_Y_Start  +
         (u32OriginDCI_Height *  pstV0DisPosition->s32Height) / pstVpssGive->s32Height;

    /*3rdly, calculate the final dci in middle coordinate system.*/
    u32SecondCoord_DCI_X_Start += pstV0DisPosition->s32X;
    u32SecondCoord_DCI_X_End   += pstV0DisPosition->s32X;

    u32SecondCoord_DCI_Y_Start  += pstV0DisPosition->s32Y;
    u32SecondCoord_DCI_Y_End  += pstV0DisPosition->s32Y;

    pstActualDciArea->u16HStar = u32SecondCoord_DCI_X_Start;
    pstActualDciArea->u16HEnd = u32SecondCoord_DCI_X_End;
    pstActualDciArea->u16VStar = u32SecondCoord_DCI_Y_Start;
    pstActualDciArea->u16VEnd = u32SecondCoord_DCI_Y_End;

    *pbDciEnable = HI_TRUE;
    return HI_SUCCESS;
}

HI_VOID Win_DciEnable_Policy(WINDOW_S *pstWin,
                         HI_RECT_S *pstOriginDciArea,/*vpss's effective area.*/
                         HI_RECT_S *pstV0DisPosition,/*secondary coordinate*/
                         HI_RECT_S *pstVpssGive,/*the original rect give by vpss.*/
                         HI_U32    u32WinNum)
{
    HI_BOOL   bDciEnable = HI_FALSE;
    HI_BOOL   bDciCapability = HI_FALSE;
    HI_PQ_DCI_WIN_S stActualDciArea;
    HI_RECT_S stActualDciAreaRect;
    VIDEO_LAYER_CAPABILITY_S  stVideoLayerCap;
    HI_S32 ret = 0;

    if ((ret = pstWin->stVLayerFunc.PF_GetCapability(pstWin->u32VideoLayer,&stVideoLayerCap)))
    {
        return ;
    }

    bDciCapability = stVideoLayerCap.bDci;
    if (bDciCapability)
    {
        dci_enable_policy(pstOriginDciArea,/*vpss's effective area.*/
                            pstV0DisPosition,/*secondary coordinate*/
                            pstVpssGive,/*the original rect give by vpss.*/
                            u32WinNum,
                            &bDciEnable,
                            &stActualDciArea);

        updateDci(pstWin, bDciEnable, &stActualDciArea);

        stActualDciAreaRect.s32X = stActualDciArea.u16HStar;
        stActualDciAreaRect.s32Y = stActualDciArea.u16VStar;

        stActualDciAreaRect.s32Width = stActualDciArea.u16HEnd - stActualDciArea.u16HStar;
        stActualDciAreaRect.s32Height =stActualDciArea.u16VEnd - stActualDciArea.u16VStar;

        pstWin->stWinInfoForDeveloper.stOriginDCIPositionInFrame = *pstOriginDciArea;
        pstWin->stWinInfoForDeveloper.stDciFrameSize             = *pstVpssGive;
        pstWin->stWinInfoForDeveloper.bDciOpen                   = bDciEnable;
        pstWin->stWinInfoForDeveloper.stWinFinalPosition         = stActualDciAreaRect;
    }

    return ;
}


/*since we support out of window, so when out of window, the content and size of
 * the window  should be  cropped.
 */
HI_S32 Win_Revise_OutOfScreenWin_OutRect(HI_RECT_S *pstInRect,
                        HI_RECT_S *pstVideoRect,HI_RECT_S *pstDispRect,
                        HI_RECT_S stScreen,
                        HI_DRV_DISP_OFFSET_S stOffsetRect,
                        WIN_HAL_PARA_S *pstLayerPara)
{
    HI_RECT_S stInRect = *pstInRect;
	HI_RECT_S stVideoRect = *pstVideoRect;
    HI_RECT_S stDispRect = *pstDispRect;
	HI_RECT_S stVideoRectOffset ;
    HI_S32 s32VaildWidth,s32VaildHeight;
    HI_S32 ix = 0, iy = 0, iw = 0, ih = 0;
    HI_S32 ox =0, oy = 0, ow = 0, oh = 0;
    HI_BOOL bDispFlag = HI_FALSE;
    HI_CHIP_TYPE_E   enChipType;
    HI_CHIP_VERSION_E enChipVersion;

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);


	stVideoRectOffset.s32X = stDispRect.s32X - stVideoRect.s32X ;
	stVideoRectOffset.s32Y = stDispRect.s32Y - stVideoRect.s32Y;
	stVideoRectOffset.s32Width = stDispRect.s32Width - stVideoRect.s32Width;
	stVideoRectOffset.s32Height = stDispRect.s32Height - stVideoRect.s32Height;

	ix = pstInRect->s32X;
	iy = pstInRect->s32Y;
	iw = pstInRect->s32Width;
	ih = pstInRect->s32Height;
	
    if (pstLayerPara->bSecondHorZmeEnable && pstLayerPara->bSecondVerZmeEnable)
    {
        /*when sr enable ,in 98c ,because v0 output size is half of final.
         * so fmt should be 1/2 size.
         */
        if ((HI_CHIP_TYPE_HI3798C == enChipType) || (HI_CHIP_TYPE_HI3796C == enChipType))
        {
            stScreen.s32Width  >>= 1;
            stScreen.s32Height >>= 1;
        }
    }

    stScreen.s32X = 0;
    stScreen.s32Y = 0;

    stScreen.s32Width -= stOffsetRect.u32Left + stOffsetRect.u32Right;
    stScreen.s32Height -= stOffsetRect.u32Top + stOffsetRect.u32Bottom;
    /*stOutRect considered the offset already, so no need to - left.*/
    stVideoRect.s32X  -= stOffsetRect.u32Left;
    stVideoRect.s32Y  -= stOffsetRect.u32Top;

    if(stVideoRect.s32X < 0)
    {
        s32VaildWidth = stVideoRect.s32Width  + stVideoRect.s32X;

        if(s32VaildWidth < WIN_INRECT_MIN_WIDTH)
        {
            ow = WIN_INRECT_MIN_WIDTH;
            ox = 0;
            iw = stInRect.s32Width * ow/stVideoRect.s32Width;
            ix += stInRect.s32Width - iw;

            bDispFlag = HI_TRUE;
        }
        else
        {   // >screen size
            ow = stScreen.s32Width;
            if(s32VaildWidth < ow) // no full screan
            {
                 ow = s32VaildWidth;
            }

            ox = 0;
            iw = stInRect.s32Width * ow/stVideoRect.s32Width;
            ix += stInRect.s32Width - stInRect.s32Width * s32VaildWidth/stVideoRect.s32Width;
        }
    }
    else if (stVideoRect.s32X < (stScreen.s32Width - WIN_INRECT_MIN_WIDTH))
    {
        s32VaildWidth = stVideoRect.s32Width;

        if(s32VaildWidth < WIN_INRECT_MIN_WIDTH)
        {
            ow = WIN_INRECT_MIN_WIDTH;
            ox = stVideoRect.s32X;
            iw = stInRect.s32Width;
            ix += 0;
            bDispFlag = HI_TRUE;
        }
        else
        {
            ox = stVideoRect.s32X;
            ix += 0;

            if((s32VaildWidth + ox) > stScreen.s32Width)
            {
                ow = stScreen.s32Width - stVideoRect.s32X;
                iw = stInRect.s32Width*ow/stVideoRect.s32Width;
            }
            else
            {
               ow = stVideoRect.s32Width;
               iw = stInRect.s32Width;
            }
        }
    }
    else
    {
        ow = WIN_INRECT_MIN_WIDTH;
        ox = stScreen.s32Width  - ow;
        iw = stInRect.s32Width*ow/stVideoRect.s32Width;
        ix += 0;
        bDispFlag = HI_TRUE;
    }


    if(stVideoRect.s32Y < 0)
    {
        s32VaildHeight = stVideoRect.s32Height + stVideoRect.s32Y;
        if(s32VaildHeight < WIN_INRECT_MIN_HEIGHT)
        {
            oy = 0;
            oh = WIN_INRECT_MIN_HEIGHT;
            ih = stInRect.s32Height*oh/stVideoRect.s32Height;
            iy += stInRect.s32Height - ih;
            bDispFlag = HI_TRUE;
        }
        else
        {
            oy = 0;
            oh = stScreen.s32Height;
            if(s32VaildHeight < oh)
            {
                oh = s32VaildHeight;
            }

            ih = stInRect.s32Height*oh/stVideoRect.s32Height;
            iy += stInRect.s32Height - stInRect.s32Height*s32VaildHeight/stVideoRect.s32Height;
        }
    }
    else if (stVideoRect.s32Y < (stScreen.s32Height - WIN_INRECT_MIN_HEIGHT))
    {
        s32VaildHeight = stVideoRect.s32Height;

        if(s32VaildHeight < WIN_INRECT_MIN_HEIGHT)
        {
            oy = stVideoRect.s32Y;
            oh = WIN_INRECT_MIN_HEIGHT;
            ih = stInRect.s32Height;
            iy += 0;
            bDispFlag = HI_TRUE;
        }
        else
        {
            oy = stVideoRect.s32Y;
            iy += 0;
            if((s32VaildHeight + oy) > stScreen.s32Height)
            {
                oh = stScreen.s32Height - stVideoRect.s32Y;
                ih = stInRect.s32Height * oh/stVideoRect.s32Height;
            }
            else
            {
                oh = stVideoRect.s32Height;
                ih = stInRect.s32Height;
            }
         }
    }
    else
    {
        oh = WIN_INRECT_MIN_HEIGHT;
        oy = stScreen.s32Height - oh;
        ih = stInRect.s32Height * oh/stVideoRect.s32Height;
        iy += 0;
        bDispFlag = HI_TRUE;
    }

    ox  += stOffsetRect.u32Left;
    oy  += stOffsetRect.u32Top;

    /*DTS2013100801678, pstInRect in fact is the output of vpss(zme,dei,crop dnr)
     * it does not confirm to  the align limit of inrect.
     * If we give a wrong use, and make a wrong align,
     * there will be no consistency bettween GFX and VIDEO, and the backgroud color will appears.
     */
    pstInRect->s32X = ix & HI_WIN_OUT_RECT_X_ALIGN;
    pstInRect->s32Y = iy & HI_WIN_OUT_RECT_Y_ALIGN;
    pstInRect->s32Width = iw & HI_WIN_OUT_RECT_WIDTH_ALIGN;
    pstInRect->s32Height = ih & HI_WIN_OUT_RECT_HEIGHT_ALIGN;

    pstVideoRect->s32X = ox & HI_WIN_OUT_RECT_X_ALIGN;
    pstVideoRect->s32Y = oy & HI_WIN_OUT_RECT_Y_ALIGN;
    pstVideoRect->s32Width = ow & HI_WIN_OUT_RECT_WIDTH_ALIGN;
    pstVideoRect->s32Height = oh & HI_WIN_OUT_RECT_HEIGHT_ALIGN;

	pstDispRect->s32X = (ox + stVideoRectOffset.s32X) & HI_WIN_OUT_RECT_X_ALIGN;
    pstDispRect->s32Y = (oy + stVideoRectOffset.s32Y) & HI_WIN_OUT_RECT_Y_ALIGN;
	
	if (pstDispRect->s32X < 0)
		pstDispRect->s32X = 0;
	if (pstDispRect->s32Y < 0)
		pstDispRect->s32Y = 0;
	
    pstDispRect->s32Width = (ow + stVideoRectOffset.s32Width) & HI_WIN_OUT_RECT_WIDTH_ALIGN;
    pstDispRect->s32Height = (oh + stVideoRectOffset.s32Height) & HI_WIN_OUT_RECT_HEIGHT_ALIGN ;
	
    return bDispFlag;
}


#define VDP_ZME_IN_SCALE_VER_THD   5
#define VDP_ZME_IN_SCALE_HOR_THD   5
#define VDP_ZME_OUT_SCALE_THD  10


/*P2i means Progressive to Interleave,  means read one field from frame.
  * This func is fit for  V0 and WBC_VP.
  * in these cases, open p2i, will improve the Picture quanlity.
  */
HI_BOOL Win_Decide_LayerP2iOpenOrNot(HI_RECT_S *pstFrameRectSize,
                                HI_RECT_S *pstOutPutSize,
                                HI_BOOL  bInputFrameMode)
{
    HI_U32   u32ZmeScale = 0;
	if (bInputFrameMode != HI_TRUE)
	{
		return HI_FALSE;
	}
	
	/**************when hd progressive, v0 may not open P2i,
	 * but support draw line, fixed top or fixed bottom********/	 
    if ((pstFrameRectSize->s32Width == pstOutPutSize->s32Width)
        && (pstFrameRectSize->s32Height == pstOutPutSize->s32Height))
    {
        return HI_TRUE;
    }
	
	if (pstFrameRectSize->s32Height  > pstOutPutSize->s32Height)
	{
		/*input height divide output height.*/
		u32ZmeScale = pstFrameRectSize->s32Height/pstOutPutSize->s32Height;
	}
	
    /*: vertical and horizical both, or only one
        *now: we only care the vertical direction.
        */
    if (u32ZmeScale > VDP_ZME_IN_SCALE_VER_THD)		
    {
        return HI_TRUE;
    }

	/*read frame or filed ,by the fmt resolution, or the output rect size.*/
    return HI_FALSE;
    
} 


HI_BOOL Win_Decide_WbcP2iOpenOrNot(HI_RECT_S *pstFrameRectSize,
                                           HI_RECT_S *pstOutPutSize,
                                           HI_BOOL  bInputFrameMode,
                                           HI_DRV_FIELD_MODE_E eWbcInFieldMode_Out)
{
    HI_U32   u32ZmeScale = 0;

	/*if  wbc's  output is progressive, p2i should not opened!\n*/
	if (eWbcInFieldMode_Out == HI_DRV_FIELD_ALL)
	{
		return HI_FALSE;
	}

	/*if wbc's input is not progressive , p2i should not opened!\n*/
	if (bInputFrameMode != HI_TRUE)
	{
		return HI_FALSE;
	}
	
	
	/**************when hd progressive, v0 may not open P2i,
	 * but support draw line, fixed top or fixed bottom********/	 
    if ((pstFrameRectSize->s32Width == pstOutPutSize->s32Width)
        && (pstFrameRectSize->s32Height == pstOutPutSize->s32Height))
    {
        return HI_TRUE;
    }
	
	if (pstFrameRectSize->s32Height  > pstOutPutSize->s32Height)
	{
		/*input height divide output height.*/
		u32ZmeScale = pstFrameRectSize->s32Height/pstOutPutSize->s32Height;
	}
	
    if (u32ZmeScale > VDP_ZME_IN_SCALE_VER_THD)		
    {
        return HI_TRUE;
    }
	/*read frame or filed ,by the fmt resolution, or the output rect size.*/
    return HI_FALSE;
    
} 

/*
  * v0  scale down or not , judged from v0's frame format: frame or field.
  * to decide the wbc point. different wbc point has different.
  * in: bHDZoomIn , means whether zoom in occurs in hd (v0),
  * in: u32HDZoomScale , means the zoom in scale in hd (v0),
  * in: bSdZoomIn ,means whether zoom in occurs in sd(v3),
  * in: u32SDZoomScale ,means the zoom in scale in sd(v3),
  * out: pu32WbcPoint, means the wbc point.
  */
HI_S32 Win_Decide_WbcPoint (HI_BOOL  bHDVertZoomIn,/*whether exist horizical scale down.*/
                                     HI_BOOL  bHDHorZoomIn,                                     
                                     HI_BOOL  bSdVertZoomIn,
                                     HI_BOOL  bSdHorZoomIn,                                     
                                     HI_U32  u32SDVertZoomScale,
									 HI_U32  u32SDHorZoomScale,
                                     HI_U32   u32WinNum,
                                     HI_U32   *pu32WbcPoint)
{
    /*when window size larger than 2, we put the wbc point onto vp.*/
    if (u32WinNum >= 2)
    {
        *pu32WbcPoint = (HI_U32) VDP_LAYER_CONN_VPACSC;
        return HI_SUCCESS;
    }

	if (bHDVertZoomIn)
	{
		 /*if hd zoomin, sd will be zoomin.*/
	    *pu32WbcPoint = (HI_U32) VDP_LAYER_CONN_VPACSC;
	}
	else
	{
		/*if hd zoomin, sd will be zoomin.*/
	    *pu32WbcPoint = (HI_U32) VDP_LAYER_CONN_VIDBZME;
	}
	
    return HI_SUCCESS;
}


/* we calculate the zoom scale according the frame size, hd fmt size, sd fmt size .
  * and winnum.
  *do not care v0 read field or not, it does not  effect the zoom in or out.
 */
HI_S32  Win_Get_WbcPointAndOutSize(HI_RECT_S *pstInputFrameSize,
	    		                                HI_RECT_S *pstOutPutFrameSize,
	    		                                HI_RECT_S *pstHdFmtRect,
	    		                                HI_RECT_S *pstSdFmtRect,
	    		                                HI_RECT_S *pstSdOutRect,
	    										HI_U32    u32WinNum,
	    		                                HI_U32    *pu32WbcPoint,
												HI_BOOL bFieldRead,
												HI_BOOL bFieldOutput)
{   
	HI_RECT_S  stSdOutputRectSize;	
	HI_BOOL  bHdVertZoomIn;
    HI_BOOL  bHdHorZoomIn;                                          
    HI_BOOL  bSdVertZoomIn;
    HI_BOOL  bSdHorZoomIn;
    HI_U32   u32SdVertZoomScale;
    HI_U32   u32SdHorZoomScale;
	HI_U32   u32InputMultiple = bFieldRead ? 2:1;
	HI_U32   u32OutputMultiple = bFieldOutput ? 2:1;
	
	/*do correspoding tranlation according the hd and sd fmt.*/
	stSdOutputRectSize.s32X = (pstOutPutFrameSize->s32X * pstSdFmtRect->s32Width)/pstHdFmtRect->s32Width;
	stSdOutputRectSize.s32Width = (pstOutPutFrameSize->s32Width * pstSdFmtRect->s32Width)/pstHdFmtRect->s32Width;
	
	stSdOutputRectSize.s32Y = (pstOutPutFrameSize->s32Y * pstSdFmtRect->s32Height)/pstHdFmtRect->s32Height;
	stSdOutputRectSize.s32Height = (pstOutPutFrameSize->s32Height * pstSdFmtRect->s32Height)/pstHdFmtRect->s32Height;

	if (stSdOutputRectSize.s32Height < 64)
	{
		//WIN_WARN("Sd OutputSize out of range.\n");
		stSdOutputRectSize.s32Height = 64;
	}
	
	if (stSdOutputRectSize.s32Width < 64)
	{
		//WIN_WARN("Sd OutputSize out of range.\n");
		stSdOutputRectSize.s32Width = 64;
	}
	
	if (stSdOutputRectSize.s32Width > 720)
	{
		WIN_WARN("Sd OutputSize out of range.\n");
		stSdOutputRectSize.s32Width = 720;
	}

	if (stSdOutputRectSize.s32Height > 576)
	{
		WIN_WARN("Sd OutputSize out of range.\n");
		stSdOutputRectSize.s32Height = 576;
	}

	stSdOutputRectSize.s32X &= 0xfffffffe;
	stSdOutputRectSize.s32Width &= 0xfffffffe;
	stSdOutputRectSize.s32Y &= 0xfffffffc;
	stSdOutputRectSize.s32Height &= 0xfffffffc;

	
	
	/*calculate the zoom in result and the zoomscale .*/
	if ((pstInputFrameSize->s32Height / u32InputMultiple) > (pstOutPutFrameSize->s32Height / u32OutputMultiple))
	{
		bHdVertZoomIn = HI_TRUE;
	}
	else
	{
		bHdVertZoomIn = HI_FALSE;
	}

	if (pstInputFrameSize->s32Width > pstOutPutFrameSize->s32Width)
	{
		bHdHorZoomIn = HI_TRUE;
	}
	else
	{
		bHdHorZoomIn = HI_FALSE;
	}

	if (pstInputFrameSize->s32Height > stSdOutputRectSize.s32Height)
	{
		bSdVertZoomIn = HI_TRUE;
		u32SdVertZoomScale = pstInputFrameSize->s32Height/stSdOutputRectSize.s32Height; 
	}
	else
	{
		bSdVertZoomIn = HI_FALSE;
		u32SdVertZoomScale = 0; 
	}

	if (pstInputFrameSize->s32Width > stSdOutputRectSize.s32Width)
	{
		bSdHorZoomIn = HI_TRUE;
		u32SdHorZoomScale = pstInputFrameSize->s32Width/stSdOutputRectSize.s32Width; 
	}
	else
	{
		bSdHorZoomIn = HI_FALSE;
		u32SdHorZoomScale = 0; 
	}
	
	Win_Decide_WbcPoint(bHdVertZoomIn,
						bHdHorZoomIn,                                          
						bSdVertZoomIn,
						bSdHorZoomIn,
						u32SdVertZoomScale,
						u32SdHorZoomScale,
						u32WinNum,
						pu32WbcPoint);
	
	if (*pu32WbcPoint == VDP_LAYER_CONN_VPACSC)
	{
		pstSdOutRect->s32X = 0;
		pstSdOutRect->s32Y = 0;		
		pstSdOutRect->s32Width = pstSdFmtRect->s32Width;
		pstSdOutRect->s32Height = pstSdFmtRect->s32Height;
	}
	else
	{
		*pstSdOutRect = stSdOutputRectSize;
	}
		
    return HI_SUCCESS;
}

#define DEBUG_TEST 1
#if DEBUG_TEST
typedef struct
{
    HI_U32  u32WinNum;
    HI_BOOL bHorSrEnable;
    HI_BOOL bVerSrEnable;

    HI_BOOL  bZmeCapability;
    HI_BOOL  bSrCapability;

    HI_U32   u32LayerMAXWidth;
    HI_U32   u32LayerMAXHeight;
    HI_RECT_S stVpssZmeSize;
    HI_RECT_S stFinalZmeSize;
    HI_RECT_S stSourceFrameRect;
} sr_struct_s;

//WIN_ERROR("expect:%d,%d,%d,%d, actual:%d,%d,%d,%d!\n", a,b,w,h,sr_test.bHorSrEnable,sr_test.bVerSrEnable,sr_test.stVpssZmeSize.s32Width,sr_test.stVpssZmeSize.s32Height);
#define check_return_value(sr_test, a,b,w,h) ({ \
        HI_BOOL bValue;\
        bValue = ((sr_test.bHorSrEnable != a) \
                  || (sr_test.bVerSrEnable != b) \
                  || (sr_test.stVpssZmeSize.s32Width != w) \
                  || (sr_test.stVpssZmeSize.s32Height != h)); \
        bValue; \
    })

#define check_return_value_post(post_test, a,b,x,y,w,h) ({ \
        HI_BOOL bValue;\
        bValue = ((post_test.bHorSrEnable != a) \
                  || (post_test.bVerSrEnable != b) \
                  || (post_test.stV0DisPosition.s32X != x) \
                  || (post_test.stV0DisPosition.s32Y != y) \
                  || (post_test.stV0DisPosition.s32Width != w) \
                  || (post_test.stV0DisPosition.s32Height != h)); \
        bValue; \
    })

//WIN_ERROR("expect:%d,%d,%d,%d,%d, actual:%d,%d,%d,%d,%d!\n", a,x,y,w,h,dci_test.bDciEnable,dci_test.stActualDciArea.u16HStar,dci_test.stActualDciArea.u16VStar,dci_test.stActualDciArea.u16HEnd - dci_test.stActualDciArea.u16HStar, dci_test.stActualDciArea.u16VEnd - dci_test.stActualDciArea.u16VStar);
#define check_return_value_dci(dci_test, a,x,y,w,h) ({ \
        HI_BOOL bValue;\
        bValue = ((dci_test.bDciEnable != a) \
                  || (dci_test.stActualDciArea.u16HStar != x) \
                  || (dci_test.stActualDciArea.u16VStar != y) \
                  || ((dci_test.stActualDciArea.u16HEnd - dci_test.stActualDciArea.u16HStar)  != w) \
                  || ((dci_test.stActualDciArea.u16VEnd - dci_test.stActualDciArea.u16VStar) != h)); \
        bValue; \
    })

HI_VOID  memset_SR(HI_RECT_S* pstRect, HI_U32 u32X, HI_U32 u32Y, HI_U32 u32Width, HI_U32 u32Height)
{
    pstRect->s32Width = u32Width;
    pstRect->s32Height = u32Height;

    pstRect->s32X = u32X;
    pstRect->s32Y = u32Y;

}


HI_S32 call_sr_func(sr_struct_s* sr_test)
{
#if 0
    return pre_ScalerDistribute(&sr_test->stVpssZmeSize,
                                &sr_test->stFinalZmeSize,
                                sr_test->u32WinNum,
                                &sr_test->bHorSrEnable,
                                &sr_test->bVerSrEnable,
                                sr_test->bZmeCapability,
                                sr_test->bSrCapability,
                                sr_test->u32LayerMAXWidth,
                                sr_test->u32LayerMAXHeight,
                                &sr_test->stSourceFrameRect,
                                HI_NULL,
                                HI_FALSE,
                                DISP_STEREO_NONE);
#endif

    if (sr_test)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}


HI_S32 Win_TestSR(HI_VOID)
{
    HI_S32  s32Ret = 0;
    sr_struct_s sr_test;

    memset((void*)&sr_test, 0, sizeof(sr_struct_s));
    sr_test.bZmeCapability = HI_TRUE;
    sr_test.bSrCapability = HI_TRUE;
    sr_test.u32LayerMAXWidth = 5000;
    sr_test.u32LayerMAXHeight = 5000;

    /*1. win== 2,even 1/2,sr not opened.*/
    sr_test.u32WinNum = 2;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 180, 144);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_FALSE, HI_FALSE, 360, 288))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*2. win== 2,even less 1/2,sr not opened.*/
    sr_test.u32WinNum = 2;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 176, 140);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_FALSE, HI_FALSE, 360, 288))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*3. win== 2, large 1/2,sr not opened.*/
    sr_test.u32WinNum = 2;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 184, 148);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_FALSE, HI_FALSE, 360, 288))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*4. win== 1, 1/2,sr opened.*/
    sr_test.u32WinNum = 1;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 180, 144);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_TRUE, HI_TRUE, 180, 144))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*5. win== 1, less 1/2,sr opened.*/
    sr_test.u32WinNum = 1;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 176, 140);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_TRUE, HI_TRUE, 180, 144))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*6. win== 1, height less  1/2,sr opened.*/
    sr_test.u32WinNum = 1;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 184, 140);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_FALSE, HI_TRUE, 360, 144))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*7. win== 1, width less  1/2,sr opened.*/
    sr_test.u32WinNum = 1;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 176, 148);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_TRUE, HI_FALSE, 180, 288))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*8. win== 1, both larger then  1/2,sr opened.*/
    sr_test.u32WinNum = 1;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 188, 148);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_FALSE, HI_FALSE, 360, 288))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*9. win== 1, both equal,sr opened.*/
    sr_test.u32WinNum = 1;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 360, 288);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_FALSE, HI_FALSE, 360, 288))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*a. win== 1, both larger than outrect,sr opened.*/
    sr_test.u32WinNum = 1;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 360, 288);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 366, 292);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_FALSE, HI_FALSE, 360, 288))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }


    /*a. win== 1, both larger than outrect,sr opened.*/
    sr_test.u32WinNum = 1;
    memset_SR(&sr_test.stFinalZmeSize, 0, 0, 3840, 2160);
    memset_SR(&sr_test.stSourceFrameRect, 0, 0, 2000, 1070);
    if ((s32Ret = call_sr_func(&sr_test)))
    {
        WIN_ERROR("sr test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value(sr_test, HI_FALSE, HI_FALSE, 3840, 2160))
        {
            WIN_ERROR("sr test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    WIN_ERROR("%s: test success!\n",__func__);
    return HI_SUCCESS;
}

typedef struct
{
    HI_RECT_S stFinalDisPosition;
    HI_RECT_S stV0DisPosition;
    HI_BOOL   bHorSrEnable;
    HI_BOOL   bVerSrEnable;
    HI_U32    u32WinNum;
    HI_RECT_S stSourceFrameRect;
    HI_CHIP_TYPE_E   enChipType;
    HI_BOOL bSrCapability;
} post_test_s;

typedef struct
{
    HI_RECT_S stOriginDciArea;
    HI_RECT_S stV0DisPosition;
    HI_RECT_S stVpssGive;
    HI_U32    u32WinNum;
    HI_BOOL   bDciEnable;
    HI_PQ_DCI_WIN_S stActualDciArea;
} dci_test_s;

HI_S32 call_post_func(post_test_s* post_test)
{
#if 0
    return post_ScalerProcess(&post_test->stFinalDisPosition,
                              &post_test->stV0DisPosition,
                              &post_test->bHorSrEnable,
                              &post_test->bVerSrEnable,
                              post_test->u32WinNum,
                              &post_test->stSourceFrameRect,
                              post_test->enChipType,
                              post_test->bSrCapability,
                              HI_NULL,
                              DISP_STEREO_NONE);
#endif

    if (post_test)
        return HI_SUCCESS;
    else
        return HI_FAILURE;
}

HI_S32 call_dci_func(dci_test_s* dci_test)
{
    return dci_enable_policy(&dci_test->stOriginDciArea,
                             &dci_test->stV0DisPosition,
                             &dci_test->stVpssGive,
                             dci_test->u32WinNum,
                             &dci_test->bDciEnable,
                             &dci_test->stActualDciArea);
}

HI_S32 Win_TestPost(HI_VOID)
{
    post_test_s post_test;
    HI_S32 s32Ret = 0;

    memset((void*)&post_test, 0, sizeof(post_test_s));

    post_test.enChipType = HI_CHIP_TYPE_HI3798C;
    post_test.bSrCapability = HI_TRUE;

    /*1. 2 WIN, less than 1/2.*/
    post_test.u32WinNum = 2;
    memset_SR(&post_test.stFinalDisPosition, 24, 36, 360, 288);
    memset_SR(&post_test.stSourceFrameRect, 0, 0, 170, 136);
    if ((s32Ret = call_post_func(&post_test)))
    {
        WIN_ERROR("post test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_post(post_test, HI_FALSE, HI_FALSE, 24, 36, 360, 288))
        {
            WIN_ERROR("post test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*2. 2 WIN, == 1/2.*/
    post_test.u32WinNum = 2;
    memset_SR(&post_test.stFinalDisPosition, 24, 36, 360, 288);
    memset_SR(&post_test.stSourceFrameRect, 0, 0, 180, 144);
    if ((s32Ret = call_post_func(&post_test)))
    {
        WIN_ERROR("post test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_post(post_test, HI_FALSE, HI_FALSE, 24, 36, 360, 288))
        {
            WIN_ERROR("post test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }


    /*3. 2 WIN, > 1/2.*/
    post_test.u32WinNum = 2;
    memset_SR(&post_test.stFinalDisPosition, 24, 36, 360, 288);
    memset_SR(&post_test.stSourceFrameRect, 0, 0, 190, 160);
    if ((s32Ret = call_post_func(&post_test)))
    {
        WIN_ERROR("post test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_post(post_test, HI_FALSE, HI_FALSE, 24, 36, 360, 288))
        {
            WIN_ERROR("post test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*4. 1 WIN, == 1/2.*/
    post_test.u32WinNum = 1;
    memset_SR(&post_test.stFinalDisPosition, 24, 36, 360, 288);
    memset_SR(&post_test.stSourceFrameRect, 0, 0, 180, 144);
    if ((s32Ret = call_post_func(&post_test)))
    {
        WIN_ERROR("post test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_post(post_test, HI_TRUE, HI_TRUE, 12, 18, 180, 144))
        {
            WIN_ERROR("post test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }


    /*5. 1 WIN, width ==  1/2.*/
    post_test.u32WinNum = 1;
    memset_SR(&post_test.stFinalDisPosition, 24, 36, 360, 288);
    memset_SR(&post_test.stSourceFrameRect, 0, 0, 180, 146);
    if ((s32Ret = call_post_func(&post_test)))
    {
        WIN_ERROR("post test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_post(post_test, HI_TRUE, HI_FALSE, 12, 36, 180, 288))
        {
            WIN_ERROR("post test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*6. 1 WIN, height ==  1/2.*/
    post_test.u32WinNum = 1;
    memset_SR(&post_test.stFinalDisPosition, 24, 36, 360, 288);
    memset_SR(&post_test.stSourceFrameRect, 0, 0, 190, 144);
    if ((s32Ret = call_post_func(&post_test)))
    {
        WIN_ERROR("post test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_post(post_test, HI_FALSE, HI_TRUE, 24, 18, 360, 144))
        {
            WIN_ERROR("post test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }


    /*7. 1 WIN, both <=  1/2.*/
    post_test.u32WinNum = 1;
    memset_SR(&post_test.stFinalDisPosition, 24, 36, 360, 288);
    memset_SR(&post_test.stSourceFrameRect, 0, 0, 148, 140);
    if ((s32Ret = call_post_func(&post_test)))
    {
        WIN_ERROR("post test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_post(post_test, HI_TRUE, HI_TRUE, 12, 18, 180, 144))
        {
            WIN_ERROR("post test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }


    /*8. 1 WIN, both >=  1/2.*/
    post_test.u32WinNum = 1;
    memset_SR(&post_test.stFinalDisPosition, 24, 36, 360, 288);
    memset_SR(&post_test.stSourceFrameRect, 0, 0, 190, 148);
    if ((s32Ret = call_post_func(&post_test)))
    {
        WIN_ERROR("post test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_post(post_test, HI_FALSE, HI_FALSE, 24, 36, 360, 288))
        {
            WIN_ERROR("post test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }


    WIN_ERROR("%s: test success!\n",__func__);
    return HI_SUCCESS;
}


HI_S32 Win_TestDci(HI_VOID)
{
    dci_test_s dci_test;
    HI_S32 s32Ret = 0;

    memset((void*)&dci_test, 0, sizeof(dci_test_s));

    /*1. 2 WIN*/
    dci_test.u32WinNum = 2;
    memset_SR(&dci_test.stOriginDciArea, 24, 36, 360, 288);
    memset_SR(&dci_test.stVpssGive, 0, 0, 500, 400);
    memset_SR(&dci_test.stV0DisPosition, 0, 0, 500, 400);
    if ((s32Ret = call_dci_func(&dci_test)))
    {
        WIN_ERROR("dci test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_dci(dci_test, HI_FALSE, 0, 0, 0, 0))
        {
            WIN_ERROR("dci test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }
    }

    /*2. 2 WIN */
    dci_test.u32WinNum = 3;
    memset_SR(&dci_test.stOriginDciArea, 24, 36, 360, 288);
    memset_SR(&dci_test.stVpssGive, 0, 0, 500, 400);
    memset_SR(&dci_test.stV0DisPosition, 0, 0, 500, 400);
    if ((s32Ret = call_dci_func(&dci_test)))
    {
        WIN_ERROR("dci test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_dci(dci_test, HI_FALSE, 0, 0, 0, 0))
        {
            WIN_ERROR("dci test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }

    }

    /*3. 1 WIN, vpss == vdp0 */
    dci_test.u32WinNum = 1;
    memset_SR(&dci_test.stOriginDciArea, 24, 36, 360, 288);
    memset_SR(&dci_test.stVpssGive, 0, 0, 500, 400);
    memset_SR(&dci_test.stV0DisPosition, 0, 0, 500, 400);
    if ((s32Ret = call_dci_func(&dci_test)))
    {
        WIN_ERROR("dci test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_dci(dci_test, HI_TRUE, 24, 36, 360, 288))
        {
            WIN_ERROR("dci test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }

    }

    /*4. 1 WIN vpss_w == vdp0/2*/
    dci_test.u32WinNum = 1;
    memset_SR(&dci_test.stOriginDciArea, 24, 36, 160, 150);
    memset_SR(&dci_test.stVpssGive, 0, 0, 250, 400);
    memset_SR(&dci_test.stV0DisPosition, 0, 0, 500, 400);
    if ((s32Ret = call_dci_func(&dci_test)))
    {
        WIN_ERROR("dci test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_dci(dci_test, HI_TRUE, 48, 36, 320, 150))
        {
            WIN_ERROR("dci test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }

    }

    /*5. 1 WIN  vpss_h == vdp0/2*/
    dci_test.u32WinNum = 1;
    memset_SR(&dci_test.stOriginDciArea, 24, 36, 160, 150);
    memset_SR(&dci_test.stVpssGive, 0, 0, 500, 200);
    memset_SR(&dci_test.stV0DisPosition, 0, 0, 500, 400);
    if ((s32Ret = call_dci_func(&dci_test)))
    {
        WIN_ERROR("dci test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_dci(dci_test, HI_TRUE, 24, 72, 160, 300))
        {
            WIN_ERROR("dci test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }

    }


    /*6. 1 WIN  vpss_wh == vdp0/2*/
    dci_test.u32WinNum = 1;
    memset_SR(&dci_test.stOriginDciArea, 24, 36, 160, 150);
    memset_SR(&dci_test.stVpssGive, 0, 0, 250, 200);
    memset_SR(&dci_test.stV0DisPosition, 0, 0, 500, 400);
    if ((s32Ret = call_dci_func(&dci_test)))
    {
        WIN_ERROR("dci test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_dci(dci_test, HI_TRUE, 48, 72, 320, 300))
        {
            WIN_ERROR("dci test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }

    }


    /*7. 1 WIN  vpss_wh == vdp0/2*/
    dci_test.u32WinNum = 1;
    memset_SR(&dci_test.stOriginDciArea, 24, 36, 160, 150);
    memset_SR(&dci_test.stVpssGive, 0, 0, 250, 300);
    memset_SR(&dci_test.stV0DisPosition, 32, 40, 500, 600);
    if ((s32Ret = call_dci_func(&dci_test)))
    {
        WIN_ERROR("dci test error:%d!\n", __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (check_return_value_dci(dci_test, HI_TRUE, 80, 112, 320, 300))
        {
            WIN_ERROR("dci test error:%d!\n", __LINE__);
            return HI_FAILURE;
        }

    }

    WIN_ERROR("%s: test success!\n",__func__);
    return HI_SUCCESS;
}


#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */




