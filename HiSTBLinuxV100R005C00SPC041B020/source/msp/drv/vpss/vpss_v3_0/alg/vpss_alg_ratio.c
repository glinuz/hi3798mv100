#include "vpss_alg_ratio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
HI_VOID ALG_RATIO_CorrectAspectRatioW_H(HI_U32 *pw, HI_U32 *ph)
{
    HI_U32 w, h;
    HI_U32 ratioRange = 1;
    
    w = *pw;
    h = *ph;

    //we only support to 256:1 or 1:256
    if(w != 0 && h!= 0)
    {
        ratioRange = ((w * ALG_RATIO_PIX_RATIO1)/ h);
        if(ratioRange > 256 * ALG_RATIO_PIX_RATIO1)
        {
            w = 256;
            h = 1;
        }

        if(ratioRange < ALG_RATIO_PIX_RATIO1 / 256)
        {
            w = 1;
            h = 256;
        }
    }

    while((w > 256) || (h > 256))
    {
        w >>= 1;
        h >>= 1;
    }

    if (!w) w = 1;
    if (!h) h =1;

    *pw = w;
    *ph  = h;
}
HI_VOID ALG_RATIO_CropedAspect(HI_RECT_S *pInWnd,HI_RECT_S *pCropedWnd,HI_U32 *AspectRatioW, HI_U32 *AspectRatioH)
{
    HI_U32 u32pixr1_In;
    HI_U32 a_w, a_h;
    
    a_w = *AspectRatioW;
    a_h = *AspectRatioH;
    
    u32pixr1_In = (a_w * pInWnd->s32Height * ALG_RATIO_PIX_RATIO1) / (a_h * pInWnd->s32Width);
    
    a_w = u32pixr1_In * pCropedWnd->s32Width;
    a_h = ALG_RATIO_PIX_RATIO1 * pCropedWnd->s32Height;
    
    ALG_RATIO_CorrectAspectRatioW_H(&a_w,&a_h);
    
    *AspectRatioW = a_w;
    *AspectRatioH = a_h;
}
HI_VOID ALG_RATIO_LetterBox(HI_U32 AspectRatioW, HI_U32 AspectRatioH,HI_RECT_S *pOutWnd,HI_S32 pixr1_out)
{
    HI_U32 v_w, v_h;
    HI_U32 ar_w = AspectRatioW, ar_h = AspectRatioH;
    
    // (v_w / v_h) * u32pixr1_out = ar_w / ar_h
    v_w = (ar_w * pOutWnd->s32Height * ALG_RATIO_PIX_RATIO1) / (ar_h * pixr1_out);
    
    if (v_w <= pOutWnd->s32Width)
    {
        v_w = v_w & 0xfffffffcul;
        pOutWnd->s32X = (pOutWnd->s32X + (pOutWnd->s32Width - v_w)/2) & 0xfffffffeul;
        pOutWnd->s32Width = v_w;
    }
    else
    {
        v_h = pOutWnd->s32Height * pOutWnd->s32Width / v_w;
        v_h = v_h & 0xfffffffcul;

        pOutWnd->s32Y = (pOutWnd->s32Y + (pOutWnd->s32Height - v_h)/2) & 0xfffffffcul;
        pOutWnd->s32Height = v_h;
    }
}
HI_S32 ALG_RATIO_RatioProcess(ALG_RATIO_DRV_PARA_S *pstDrvPara,ALG_RATIO_OUT_PARA_S *pstOutPara)
{
    // screen ratio w h, aspect ratio w h, output format w h
    HI_S32 sr_w, sr_h, ar_w = 0, ar_h = 0, f_w, f_h;
    // output video w h
    //u32pixr1_out:tv pixel aspect ratio concluded from  tv resolution and  output aspect ratio.   pixel width / pixel height
    HI_S32 u32pixr1_out;

    /*In Customer mode cvrs==Ignore will not full the window */
    if ((HI_DRV_ASP_RAT_MODE_BUTT == pstDrvPara->eAspMode))
    {
        return HI_SUCCESS;
    }

    //In customer mode ,process pOutInfo first
    if (HI_TRUE == pstDrvPara->stUsrAsp.bUserDefAspectRatio)
    {
        ar_w = pstDrvPara->stUsrAsp.u32UserAspectWidth;
        ar_h = pstDrvPara->stUsrAsp.u32UserAspectHeight;
    }
    
    if ((0 == ar_w) || (0 == ar_h))
    {
        // using ar_w and ar_h replace video output window ratio
        ar_w = pstDrvPara->AspectWidth;
        ar_h = pstDrvPara->AspectHeight;
        //HI_UNF_DISP_ASPECT_RATIO_AUTO&Square 0/0 
        if ((0 == ar_w) || (0 == ar_h))
        {
            ar_w = pstDrvPara->stInWnd.s32Width;
            ar_h = pstDrvPara->stInWnd.s32Height;            
        }
    }
    ALG_RATIO_CorrectAspectRatioW_H(&ar_w, &ar_h);
        
    // using sr_w and sr_h replace screen ratio
    sr_w = pstDrvPara->DeviceWidth;
    sr_h = pstDrvPara->DeviceHeight;
    if ((0 == sr_w) || (0 == sr_h))
    {
        sr_w = pstDrvPara->stScreen.s32Width;
        sr_h = pstDrvPara->stScreen.s32Height;
    }
    ALG_RATIO_CorrectAspectRatioW_H(&sr_w, &sr_h);
    
    // get output format w h
    f_w = pstDrvPara->stScreen.s32Width;
    f_h = pstDrvPara->stScreen.s32Height;

    //In customer mode use logic pixel,too
    u32pixr1_out = (sr_w * f_h * ALG_RATIO_PIX_RATIO1) / (sr_h * f_w);
    
    if (HI_DRV_ASP_RAT_MODE_LETTERBOX == pstDrvPara->eAspMode)
    {
        ALG_RATIO_LetterBox(ar_w, ar_h, &(pstDrvPara->stOutWnd), u32pixr1_out);
    }
    else //  HI_UNF_VO_ASPECT_CVRS_PAN_SCAN == enCvrsMode  ||  HI_UNF_VO_ASPECT_CVRS_COMBINED == enCvrsMode
    {
        HI_U32 u32ZmeH;
        HI_U32 u32ZmeW;
        HI_U32 u32ExpH;
        HI_U32 u32ExpW;

        u32ExpH = pstDrvPara->stOutWnd.s32Width * (u32pixr1_out)*ar_h /ar_w;
		u32ExpW = ar_w * pstDrvPara->stOutWnd.s32Height * ALG_RATIO_PIX_RATIO1 / u32pixr1_out / ar_h;

		u32ExpW = (u32ExpW + 0x3)&VPSS_WIDTH_ALIGN;
		u32ExpH = (u32ExpH + 0x3)&VPSS_HEIGHT_ALIGN;


        if (u32ExpH > pstDrvPara->stOutWnd.s32Height * ALG_RATIO_PIX_RATIO1 )
        {
            u32ZmeW = pstDrvPara->stOutWnd.s32Width;
            u32ZmeH = u32ExpH /ALG_RATIO_PIX_RATIO1;
			
			u32ZmeH = (u32ZmeH + 0x3)&VPSS_HEIGHT_ALIGN;

            if (HI_DRV_ASP_RAT_MODE_COMBINED == pstDrvPara->eAspMode)
            {
                u32ZmeH = pstDrvPara->stOutWnd.s32Height 
                            + (u32ZmeH - pstDrvPara->stOutWnd.s32Height)/2;
                u32ZmeW = ar_w * u32ZmeH * ALG_RATIO_PIX_RATIO1 / u32pixr1_out / ar_h;
				
				u32ZmeW = (u32ZmeW+0x3)&VPSS_HEIGHT_ALIGN;
            }
        }
        else
        {
            u32ZmeW = u32ExpW ;
            u32ZmeH = pstDrvPara->stOutWnd.s32Height;
            if (HI_DRV_ASP_RAT_MODE_COMBINED == pstDrvPara->eAspMode)
            {
                u32ZmeW = pstDrvPara->stOutWnd.s32Width
                            + (u32ZmeW - pstDrvPara->stOutWnd.s32Width)/2;
							
                u32ZmeH = u32ZmeW * (u32pixr1_out)*ar_h /ar_w / ALG_RATIO_PIX_RATIO1;

				u32ZmeH = (u32ZmeH + 0x3)&VPSS_HEIGHT_ALIGN;
            }
        }
        u32ZmeW = (u32ZmeW & VPSS_HEIGHT_ALIGN) < 4092?(u32ZmeW & VPSS_HEIGHT_ALIGN):4092;
        u32ZmeH = (u32ZmeH & VPSS_HEIGHT_ALIGN) < 4092?(u32ZmeH & VPSS_HEIGHT_ALIGN):4092;
        
        pstOutPara->u32ZmeH = u32ZmeH;  
        pstOutPara->u32ZmeW = u32ZmeW;
		
        if (pstOutPara->u32ZmeH > pstDrvPara->stOutWnd.s32Height
				&& pstOutPara->u32ZmeW <= pstDrvPara->stOutWnd.s32Width)
        {
            pstOutPara->stCropWnd.s32X = 0;
            pstOutPara->stCropWnd.s32Y = ((u32ZmeH-pstDrvPara->stOutWnd.s32Height)/2) & VPSS_HEIGHT_ALIGN;
            pstOutPara->stCropWnd.s32Height = pstDrvPara->stOutWnd.s32Height;
            pstOutPara->stCropWnd.s32Width = u32ZmeW;
        }
        else if (pstOutPara->u32ZmeW > pstDrvPara->stOutWnd.s32Width
				&& pstOutPara->u32ZmeH <= pstDrvPara->stOutWnd.s32Height)
        {
            pstOutPara->stCropWnd.s32X = ((u32ZmeW-pstDrvPara->stOutWnd.s32Width)/2) & VPSS_WIDTH_ALIGN;
            pstOutPara->stCropWnd.s32Y = 0;
            pstOutPara->stCropWnd.s32Height = u32ZmeH;
            pstOutPara->stCropWnd.s32Width = pstDrvPara->stOutWnd.s32Width;
        }
		else 
		{
            pstOutPara->stCropWnd.s32X = 0;
            pstOutPara->stCropWnd.s32Y = 0;
            pstOutPara->stCropWnd.s32Height = pstDrvPara->stOutWnd.s32Height;
            pstOutPara->stCropWnd.s32Width = pstDrvPara->stOutWnd.s32Width;
        }
        
        pstOutPara->bEnCrop = HI_TRUE;
        
        pstOutPara->stOutWnd.s32Height = pstOutPara->stCropWnd.s32Height;
        pstOutPara->stOutWnd.s32Width = pstOutPara->stCropWnd.s32Width;
        pstOutPara->stOutWnd.s32X = (pstDrvPara->stOutWnd.s32Width-pstOutPara->stCropWnd.s32Width)/2;
        pstOutPara->stOutWnd.s32Y = (pstDrvPara->stOutWnd.s32Height-pstOutPara->stCropWnd.s32Height)/2;
        pstOutPara->stOutWnd.s32X = pstOutPara->stOutWnd.s32X & VPSS_WIDTH_ALIGN;
        pstOutPara->stOutWnd.s32Y = pstOutPara->stOutWnd.s32Y & VPSS_HEIGHT_ALIGN;
        
        pstOutPara->stOutScreen.s32X = 0;
        pstOutPara->stOutScreen.s32Y = 0;
        pstOutPara->stOutScreen.s32Height = pstDrvPara->stScreen.s32Height;
        pstOutPara->stOutScreen.s32Width = pstDrvPara->stScreen.s32Width;
       
    } 
    return HI_SUCCESS;
}

HI_S32 VPSS_ALG_GetAspCfg(ALG_RATIO_DRV_PARA_S *pstAspDrvPara,
                        HI_DRV_ASP_RAT_MODE_E eAspMode,HI_RECT_S *pstScreen,
                        ALG_RATIO_OUT_PARA_S *pstAspCfg)
{    
    ALG_RATIO_RatioProcess(pstAspDrvPara,pstAspCfg);
    if (pstAspDrvPara->eAspMode == HI_DRV_ASP_RAT_MODE_LETTERBOX)
    {
        pstAspCfg->bEnCrop = HI_FALSE;
        pstAspCfg->u32ZmeW  = pstAspDrvPara->stOutWnd.s32Width;
        pstAspCfg->u32ZmeH  = pstAspDrvPara->stOutWnd.s32Height;
        
        pstAspCfg->stOutWnd.s32Height = pstAspDrvPara->stOutWnd.s32Height;
        pstAspCfg->stOutWnd.s32Width = pstAspDrvPara->stOutWnd.s32Width;
        pstAspCfg->stOutWnd.s32X = pstAspDrvPara->stOutWnd.s32X;
        pstAspCfg->stOutWnd.s32Y = pstAspDrvPara->stOutWnd.s32Y;
        pstAspCfg->stOutScreen.s32X = 0;
        pstAspCfg->stOutScreen.s32Y = 0;
        pstAspCfg->stOutScreen.s32Height = pstScreen->s32Height;
        pstAspCfg->stOutScreen.s32Width = pstScreen->s32Width;
    }
    else if (pstAspDrvPara->eAspMode == HI_DRV_ASP_RAT_MODE_PANANDSCAN)
    {
        
    }
    else
    {

    }
    
    return HI_SUCCESS;
}     
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */