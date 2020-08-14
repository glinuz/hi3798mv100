#include "vpss_alg_ratio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ALG_ASP_CHECK_W_H_ZERO(w, h) do {\
    if ((0 == (w)) || (0 == (h))) \
    { \
        VPSS_FATAL("Invalid para, divisor can't be 0. %s=%d, %s=%d\n", #w, w, #h, h); \
        return HI_VPSS_RET_DIVZERO; \
    }\
} while (0);


static HI_S32 ALG_ASP_Process(ALG_RATIO_DRV_PARA_S *pstDrvPara, ALG_RATIO_OUT_PARA_S *pstOutPara);
static HI_VOID ALG_ASP_GetImageAspectRatio(ALG_RATIO_DRV_PARA_S *pstDrvPara, HI_S32 *ps32SrcImg_asp_w, HI_S32 *ps32SrcImg_asp_h);
static HI_VOID ALG_ASP_GetScreenAspectRatio(ALG_RATIO_DRV_PARA_S *pstDrvPara, HI_S32 *ps32Screen_asp_w, HI_S32 *ps32Screen_asp_h);
static HI_VOID ALG_ASP_Correction(HI_U32 *pw, HI_U32 *ph);
static HI_VOID ALG_ASP_LetterBox(ALG_RATIO_DRV_PARA_S *pstDrvPara, ALG_RATIO_OUT_PARA_S *pstOutPara,
                                 ALG_ASP_PARA_S *pstAspPara);
static HI_VOID ALG_ASP_Panscan(ALG_RATIO_DRV_PARA_S *pstDrvPara, ALG_RATIO_OUT_PARA_S *pstOutPara,
                               ALG_ASP_PARA_S *pstAspPara);
static HI_VOID ALG_ASP_Combined(ALG_RATIO_DRV_PARA_S *pstDrvPara, ALG_RATIO_OUT_PARA_S *pstOutPara,
                                ALG_ASP_PARA_S *pstAspPara);


HI_S32 ALG_ASP_GetConfig(ALG_RATIO_DRV_PARA_S *pstAspDrvPara,
                      HI_DRV_ASP_RAT_MODE_E eAspMode, HI_RECT_S *pstScreen,
                      ALG_RATIO_OUT_PARA_S *pstAspCfg)
{
    HI_S32 s32Ret;

    if (HI_DRV_ASP_RAT_MODE_BUTT == pstAspDrvPara->eAspMode)
    {
        return HI_VPSS_RET_SUCCESS;
    }

    s32Ret = ALG_ASP_Process(pstAspDrvPara, pstAspCfg);

    if (HI_VPSS_RET_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

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
    else if ((pstAspDrvPara->eAspMode == HI_DRV_ASP_RAT_MODE_PANANDSCAN)
             || (pstAspDrvPara->eAspMode == HI_DRV_ASP_RAT_MODE_COMBINED))
    {
        //do nothing.
    }
    else
    {
        VPSS_FATAL("AspMode invalid!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*
   º¯Êý²ð·Ö, ASSERTÓï¾ä
*/
HI_S32 ALG_ASP_Process(ALG_RATIO_DRV_PARA_S *pstDrvPara, ALG_RATIO_OUT_PARA_S *pstOutPara)
{
    HI_S32 s32Screen_asp_w   = 0;
    HI_S32 s32Screen_asp_h   = 0;
    HI_S32 s32Screen_pixel_w = pstDrvPara->stScreen.s32Width;
    HI_S32 s32Screen_pixel_h = pstDrvPara->stScreen.s32Height;
    HI_S32 s32Screen_skew    = 0;
    HI_S32 s32SrcImg_asp_w   = 0;
    HI_S32 s32SrcImg_asp_h   = 0;
    ALG_ASP_PARA_S stAspPara = {0};


    ALG_ASP_GetImageAspectRatio(pstDrvPara, &s32SrcImg_asp_w, &s32SrcImg_asp_h);
    ALG_ASP_GetScreenAspectRatio(pstDrvPara, &s32Screen_asp_w, &s32Screen_asp_h);

    ALG_ASP_CHECK_W_H_ZERO(s32Screen_pixel_w, s32Screen_pixel_h);
    ALG_ASP_CHECK_W_H_ZERO(s32Screen_asp_w, s32Screen_asp_h);
    ALG_ASP_CHECK_W_H_ZERO(s32SrcImg_asp_w, s32SrcImg_asp_h);    

    /*
        s32Screen_asp_w           s32Screen_pixel_w
        ---------------     =     -----------------  * s32Screen_skew
        s32Screen_asp_h           s32Screen_pixel_h
    */
    s32Screen_skew = (s32Screen_asp_w * s32Screen_pixel_h * ALG_RATIO_PIX_RATIO1) / (s32Screen_asp_h * s32Screen_pixel_w);

    stAspPara.s32Screen_skew = s32Screen_skew;
    stAspPara.s32SrcAsp_W = s32SrcImg_asp_w;
    stAspPara.s32SrcAsp_H = s32SrcImg_asp_h;

    switch (pstDrvPara->eAspMode)
    {
        case HI_DRV_ASP_RAT_MODE_LETTERBOX:
            ALG_ASP_LetterBox(pstDrvPara, pstOutPara, &stAspPara);
            break;

        case HI_DRV_ASP_RAT_MODE_PANANDSCAN:
            ALG_ASP_Panscan(pstDrvPara, pstOutPara, &stAspPara);
            break;

        case HI_DRV_ASP_RAT_MODE_COMBINED:
            ALG_ASP_Combined(pstDrvPara, pstOutPara, &stAspPara);
            break;

        default:
            VPSS_FATAL("Unsupport asp mode:%d\n", pstDrvPara->eAspMode);
            return HI_VPSS_RET_UNSUPPORT;
    }

    return HI_VPSS_RET_SUCCESS;
}

static HI_VOID ALG_ASP_GetImageAspectRatio(ALG_RATIO_DRV_PARA_S *pstDrvPara, 
                                    HI_S32 *ps32SrcImg_asp_w, HI_S32 *ps32SrcImg_asp_h)
{
    HI_RECT_S *pstDrvWin_in  = &pstDrvPara->stInWnd;
    HI_S32 s32SrcImg_asp_w = 0;
    HI_S32 s32SrcImg_asp_h = 0;

    if (HI_TRUE == pstDrvPara->stUsrAsp.bUserDefAspectRatio)
    {
        s32SrcImg_asp_w = pstDrvPara->stUsrAsp.u32UserAspectWidth;
        s32SrcImg_asp_h = pstDrvPara->stUsrAsp.u32UserAspectHeight;
    }

    if ((0 == s32SrcImg_asp_w) || (0 == s32SrcImg_asp_h))
    {
        if ((0 == pstDrvPara->AspectWidth) || (0 == pstDrvPara->AspectHeight))
        {
            s32SrcImg_asp_w = pstDrvWin_in->s32Width;
            s32SrcImg_asp_h = pstDrvWin_in->s32Height;
        }
        else
        {
            s32SrcImg_asp_w = pstDrvPara->AspectWidth;
            s32SrcImg_asp_h = pstDrvPara->AspectHeight;
        }
    }

    ALG_ASP_Correction(&s32SrcImg_asp_w, &s32SrcImg_asp_h);

    *ps32SrcImg_asp_w = s32SrcImg_asp_w;
    *ps32SrcImg_asp_h = s32SrcImg_asp_h;

    return;
}

static HI_VOID ALG_ASP_GetScreenAspectRatio(ALG_RATIO_DRV_PARA_S *pstDrvPara, 
                                    HI_S32 *ps32Screen_asp_w, HI_S32 *ps32Screen_asp_h)
{
    HI_S32 s32Screen_asp_w = pstDrvPara->DeviceWidth;
    HI_S32 s32Screen_asp_h = pstDrvPara->DeviceHeight;

    if ((0 == s32Screen_asp_w) || (0 == s32Screen_asp_h))
    {
        s32Screen_asp_w = pstDrvPara->stScreen.s32Width;
        s32Screen_asp_h = pstDrvPara->stScreen.s32Height;
    }

    ALG_ASP_Correction(&s32Screen_asp_w, &s32Screen_asp_h);

    *ps32Screen_asp_w = s32Screen_asp_w;
    *ps32Screen_asp_h = s32Screen_asp_h;

    return;
}


static HI_VOID ALG_ASP_Correction(HI_U32* pw, HI_U32* ph)
{
    HI_U32 w, h;
    HI_U32 ratioRange = 1;

    w = *pw;
    h = *ph;

    //we only support to 256:1 or 1:256
    if (w != 0 && h != 0)
    {
        ratioRange = ((w * ALG_RATIO_PIX_RATIO1) / h);

        if (ratioRange > 256 * ALG_RATIO_PIX_RATIO1)
        {
            w = 256;
            h = 1;
        }

        if (ratioRange < ALG_RATIO_PIX_RATIO1 / 256)
        {
            w = 1;
            h = 256;
        }
    }

    while ((w > 256) || (h > 256))
    {
        w >>= 1;
        h >>= 1;
    }

    if (!w) { w = 1; }

    if (!h) { h = 1; }

    *pw = w;
    *ph  = h;

    return;
}

static HI_VOID ALG_ASP_LetterBox(ALG_RATIO_DRV_PARA_S *pstDrvPara, ALG_RATIO_OUT_PARA_S *pstOutPara,
                                 ALG_ASP_PARA_S *pstAspPara)
{
    HI_U32 v_w          = 0;
    HI_U32 v_h          = 0;
    HI_U32 ar_w         = pstAspPara->s32SrcAsp_W;
    HI_U32 ar_h         = pstAspPara->s32SrcAsp_H;
    HI_S32 pixr1_out    = pstAspPara->s32Screen_skew;
    HI_RECT_S *pOutWnd  = &pstDrvPara->stOutWnd;

    (HI_VOID)pstOutPara;

    // (v_w / v_h) * u32pixr1_out = ar_w / ar_h
    v_w = (ar_w * pOutWnd->s32Height * ALG_RATIO_PIX_RATIO1) / (ar_h * pixr1_out);

    if (v_w <= pOutWnd->s32Width)
    {
        v_w = v_w & 0xfffffffcul;
        pOutWnd->s32X = (pOutWnd->s32X + (pOutWnd->s32Width - v_w) / 2) & 0xfffffffeul;
        pOutWnd->s32Width = v_w;
    }
    else
    {
        v_h = pOutWnd->s32Height * pOutWnd->s32Width / v_w;
        v_h = v_h & 0xfffffffcul;

        pOutWnd->s32Y = (pOutWnd->s32Y + (pOutWnd->s32Height - v_h) / 2) & 0xfffffffcul;
        pOutWnd->s32Height = v_h;
    }

    return;
}

static HI_VOID ALG_ASP_Panscan(ALG_RATIO_DRV_PARA_S *pstDrvPara, ALG_RATIO_OUT_PARA_S *pstOutPara,
                                 ALG_ASP_PARA_S *pstAspPara)
{
    HI_RECT_S *pstDrvWin_in       = &pstDrvPara->stInWnd;
    HI_RECT_S *pstDrvWin_out      = &pstDrvPara->stOutWnd;
    HI_RECT_S *pstOutWin_out      = &pstOutPara->stOutWnd;
    HI_RECT_S *pstOutWin_crop     = &pstOutPara->stCropWnd;
    HI_S32      s32SrcImg_asp_w   = pstAspPara->s32SrcAsp_W;
    HI_S32      s32SrcImg_asp_h   = pstAspPara->s32SrcAsp_H;
    HI_S32      s32Crop_in_h      = 0;
    HI_S32      s32Crop_in_w      = 0;
    HI_S32      s32Croped_pixel_w = 0;
    HI_S32      s32Croped_pixel_h = 0;
    HI_S32      s32ExpH           = 0;
    HI_S32      s32ExpW           = 0;
    HI_BOOL     bFourPixOneClk    = HI_FALSE;
    HI_S32      s32Screen_skew    = pstAspPara->s32Screen_skew;

    /*
        s32SrcImg_asp_w           s32ExpW
        ---------------     =     --------  * s32Screen_skew
        s32SrcImg_asp_h           s32ExpH
    */
    s32ExpH = (pstDrvWin_out->s32Width * s32Screen_skew * s32SrcImg_asp_h) / s32SrcImg_asp_w / ALG_RATIO_PIX_RATIO1;
    s32ExpW = (s32SrcImg_asp_w * pstDrvWin_out->s32Height * ALG_RATIO_PIX_RATIO1) / s32Screen_skew / s32SrcImg_asp_h;
    VPSS_PIXLE_UPALIGN_W(s32ExpW, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(s32ExpH, bFourPixOneClk);

    if (s32ExpH > pstDrvWin_out->s32Height)
    {
        /* crop pixel in height */
        s32Crop_in_h = s32ExpH;
        s32Crop_in_w = pstDrvWin_out->s32Width;
    }
    else
    {
        /* crop pixel in width */
        s32Crop_in_w = s32ExpW ;
        s32Crop_in_h = pstDrvWin_out->s32Height;
    }

    s32Croped_pixel_w = pstDrvWin_in->s32Width * (s32Crop_in_w - pstDrvWin_out->s32Width) / s32Crop_in_w;
    s32Croped_pixel_h = pstDrvWin_in->s32Height * (s32Crop_in_h - pstDrvWin_out->s32Height) / s32Crop_in_h;
    s32Croped_pixel_w = (s32Croped_pixel_w > 0) ? s32Croped_pixel_w : 0;
    s32Croped_pixel_h = (s32Croped_pixel_h > 0) ? s32Croped_pixel_h : 0;

    VPSS_PIXLE_UPALIGN_W(s32Croped_pixel_w, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(s32Croped_pixel_h, bFourPixOneClk);

    pstOutPara->bEnCrop = HI_TRUE;
    pstOutWin_crop->s32X = s32Croped_pixel_w >> 1;
    pstOutWin_crop->s32Y = s32Croped_pixel_h >> 1;

    VPSS_PIXLE_DOWNALIGN_W(pstOutWin_crop->s32X, bFourPixOneClk);
    VPSS_PIXLE_DOWNALIGN_H(pstOutWin_crop->s32Y, bFourPixOneClk);

    s32Croped_pixel_w = pstOutWin_crop->s32X << 1;
    s32Croped_pixel_h = pstOutWin_crop->s32Y << 1;

    pstOutWin_crop->s32Width =  pstDrvWin_in->s32Width - s32Croped_pixel_w;
    pstOutWin_crop->s32Height = pstDrvWin_in->s32Height - s32Croped_pixel_h;
    VPSS_PIXLE_UPALIGN_W(pstOutWin_crop->s32Width, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(pstOutWin_crop->s32Height, bFourPixOneClk);
    HI_ASSERT(((pstOutWin_crop->s32X << 1) + pstOutWin_crop->s32Width) == pstDrvWin_in->s32Width);
    HI_ASSERT(((pstOutWin_crop->s32Y << 1) + pstOutWin_crop->s32Height) == pstDrvWin_in->s32Height);

    /*ZME*/
    pstOutPara->u32ZmeW = pstDrvWin_out->s32Width;
    pstOutPara->u32ZmeH = pstDrvWin_out->s32Height;
    VPSS_PIXLE_DOWNALIGN_W(pstOutPara->u32ZmeW, bFourPixOneClk);
    VPSS_PIXLE_DOWNALIGN_H(pstOutPara->u32ZmeH, bFourPixOneClk);

    pstOutWin_out->s32X = pstDrvWin_out->s32X;
    pstOutWin_out->s32Y = pstDrvWin_out->s32Y;
    pstOutWin_out->s32Width = pstDrvWin_out->s32Width;
    pstOutWin_out->s32Height = pstDrvWin_out->s32Height;

    VPSS_PIXLE_UPALIGN_W(pstOutWin_out->s32X, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(pstOutWin_out->s32Y, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_W(pstOutWin_out->s32Width, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(pstOutWin_out->s32Height, bFourPixOneClk);

    return;
}

static HI_VOID ALG_ASP_Combined(ALG_RATIO_DRV_PARA_S *pstDrvPara, ALG_RATIO_OUT_PARA_S *pstOutPara,
                                 ALG_ASP_PARA_S *pstAspPara)
{
    HI_RECT_S *pstDrvWin_in       = &pstDrvPara->stInWnd;
    HI_RECT_S *pstDrvWin_out      = &pstDrvPara->stOutWnd;
    HI_RECT_S *pstOutWin_out      = &pstOutPara->stOutWnd;
    HI_RECT_S *pstOutWin_crop     = &pstOutPara->stCropWnd;
    HI_S32      s32SrcImg_asp_w   = pstAspPara->s32SrcAsp_W;
    HI_S32      s32SrcImg_asp_h   = pstAspPara->s32SrcAsp_H;
    HI_S32      s32Crop_in_h      = 0;
    HI_S32      s32Crop_in_w      = 0;
    HI_S32      s32Croped_pixel_w = 0;
    HI_S32      s32Croped_pixel_h = 0;
    HI_S32      s32ExpH           = 0;
    HI_S32      s32ExpW           = 0;
    HI_BOOL     bFourPixOneClk    = HI_FALSE;
    HI_S32      s32Screen_skew    = pstAspPara->s32Screen_skew;



    /*
        s32SrcImg_asp_w           s32ExpW
        ---------------     =     --------  * s32Screen_skew
        s32SrcImg_asp_h           s32ExpH
    */
    s32ExpH = (pstDrvWin_out->s32Width * s32Screen_skew * s32SrcImg_asp_h) / s32SrcImg_asp_w / ALG_RATIO_PIX_RATIO1;
    s32ExpW = (s32SrcImg_asp_w * pstDrvWin_out->s32Height * ALG_RATIO_PIX_RATIO1) / s32Screen_skew /
              s32SrcImg_asp_h;
    VPSS_PIXLE_UPALIGN_W(s32ExpW, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(s32ExpH, bFourPixOneClk);

    if (s32ExpH > pstDrvWin_out->s32Height)
    {
        /* crop pixel in height */
        s32Crop_in_h = s32ExpH;
        s32Crop_in_w = pstDrvWin_out->s32Width;

        s32Crop_in_h = pstDrvWin_out->s32Height + (s32Crop_in_h - pstDrvWin_out->s32Height) / 2;
        s32Crop_in_w = s32SrcImg_asp_w * s32Crop_in_h * ALG_RATIO_PIX_RATIO1 / s32Screen_skew / s32SrcImg_asp_h;
        VPSS_PIXLE_UPALIGN_W(s32Crop_in_w, bFourPixOneClk);
        VPSS_PIXLE_UPALIGN_H(s32Crop_in_h, bFourPixOneClk);
    }
    else
    {
        /* crop pixel in width */
        s32Crop_in_w = s32ExpW ;
        s32Crop_in_h = pstDrvWin_out->s32Height;

        s32Crop_in_w = pstDrvWin_out->s32Width + (s32Crop_in_w - pstDrvWin_out->s32Width) / 2;
        s32Crop_in_h = s32Crop_in_w * s32Screen_skew * s32SrcImg_asp_h / s32SrcImg_asp_w / ALG_RATIO_PIX_RATIO1;
        VPSS_PIXLE_UPALIGN_W(s32Crop_in_w, bFourPixOneClk);
        VPSS_PIXLE_UPALIGN_H(s32Crop_in_h, bFourPixOneClk);
    }

    s32Croped_pixel_w = pstDrvWin_in->s32Width * (s32Crop_in_w - pstDrvWin_out->s32Width) / s32Crop_in_w;
    s32Croped_pixel_h = pstDrvWin_in->s32Height * (s32Crop_in_h - pstDrvWin_out->s32Height) / s32Crop_in_h;
    s32Croped_pixel_w = (s32Croped_pixel_w > 0) ? s32Croped_pixel_w : 0;
    s32Croped_pixel_h = (s32Croped_pixel_h > 0) ? s32Croped_pixel_h : 0;


    VPSS_PIXLE_UPALIGN_W(s32Croped_pixel_w, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(s32Croped_pixel_h, bFourPixOneClk);

    VPSS_DBG_INFO("pixr1_out:%d src_in_w h %d %d s32SrcImg_asp_w h : %d %d\ns32ExpW s32ExpH: %d %d, crop_in_w h:%d %d\n",
                  s32Screen_skew,
                  pstDrvWin_in->s32Width, pstDrvWin_in->s32Height,
                  s32SrcImg_asp_w, s32SrcImg_asp_h,
                  s32ExpW, s32ExpH,
                  s32Croped_pixel_w, s32Croped_pixel_h);

    pstOutPara->bEnCrop = HI_TRUE;
    pstOutWin_crop->s32X = s32Croped_pixel_w >> 1;
    pstOutWin_crop->s32Y = s32Croped_pixel_h >> 1;

    VPSS_PIXLE_DOWNALIGN_W(pstOutWin_crop->s32X, bFourPixOneClk);
    VPSS_PIXLE_DOWNALIGN_H(pstOutWin_crop->s32Y, bFourPixOneClk);

    s32Croped_pixel_w = pstOutWin_crop->s32X << 1;
    s32Croped_pixel_h = pstOutWin_crop->s32Y << 1;

    pstOutWin_crop->s32Width =  pstDrvWin_in->s32Width - s32Croped_pixel_w;
    pstOutWin_crop->s32Height = pstDrvWin_in->s32Height - s32Croped_pixel_h;
    VPSS_PIXLE_UPALIGN_W(pstOutWin_crop->s32Width, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(pstOutWin_crop->s32Height, bFourPixOneClk);
    HI_ASSERT(((pstOutWin_crop->s32X << 1) + pstOutWin_crop->s32Width) == pstDrvWin_in->s32Width);
    HI_ASSERT(((pstOutWin_crop->s32Y << 1) + pstOutWin_crop->s32Height) == pstDrvWin_in->s32Height);

    /*ZME*/
    pstOutPara->u32ZmeW = pstDrvWin_out->s32Width;
    pstOutPara->u32ZmeH = pstDrvWin_out->s32Height;
    VPSS_PIXLE_DOWNALIGN_W(pstOutPara->u32ZmeW, bFourPixOneClk);
    VPSS_PIXLE_DOWNALIGN_H(pstOutPara->u32ZmeH, bFourPixOneClk);

    pstOutWin_out->s32X = pstDrvWin_out->s32X;
    pstOutWin_out->s32Y = pstDrvWin_out->s32Y;
    pstOutWin_out->s32Width = pstDrvWin_out->s32Width;
    pstOutWin_out->s32Height = pstDrvWin_out->s32Height;

    VPSS_PIXLE_UPALIGN_W(pstOutWin_out->s32X, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(pstOutWin_out->s32Y, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_W(pstOutWin_out->s32Width, bFourPixOneClk);
    VPSS_PIXLE_UPALIGN_H(pstOutWin_out->s32Height, bFourPixOneClk);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

