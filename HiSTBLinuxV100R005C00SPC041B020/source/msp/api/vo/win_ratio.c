/*-----------------------------------------------------------------------*/
/*!!Warning: Hisilicon key information asset. No spread without permission. */
/*CODEMARK:EG4uRhTwMmgcVFBsBnYHCDadN5jJKSuVyxmmaCmKFU6eJEbB2fyHF9weu4/jer/hxLHb+S1e
E0zVg4C3NiZh4b+GnwjAHj8JYHgZh/mRmQnQz8Q3/q2vMRDmpFjnoIR0GIRpa1Ns7mfriMjl
Aaa27GhR6HoW0OdWixwHoorKQReYQYkEPXNzJfYt7uQ3e90SQQIlCXFjIw4s7M9b4L8g85NS
sQJIsQkQWNULNHxzhn7e7pa/yv8qkCmj1lmJcm7j9LI4n4W0cgFZoQMGkbJMhA==#*/
/*--!!Warning: Deleting or modifying the preceding information is prohibited.--*/
#include "win_ratio.h"
#include "hi_drv_win.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define RATIO_WIDTH_ALIGN 0xfffffffe
#define RATIO_HEIGHT_ALIGN 0xfffffffc
#define RATIO_WIDTH_MAX 4096
static HI_VOID ALG_RATIO_AspecRatioCorrect(HI_S32 *pw, HI_S32 *ph)
{
    HI_S32 w, h;
    HI_S32 ratioRange = 1;
    
    w = *pw;
    h = *ph;

    /*we only support to 256:1 or 1:256*/
    if(w != 0 && h!= 0)
    {
        ratioRange = ((w * ALG_RATIO_PIX_RATIO1) / h);
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

	return ;
}

static HI_VOID ALG_RATIO_DisAspRatioGet(HI_S32 *Dasp_w,\
													HI_S32 *Dasp_h,\
													ALG_RATIO_IN_PARA_S *pstDrvPara)
{
	/* expected-disp wh-aspectratio*/
	HI_S32 s32DisAsp_w = 0;
	HI_S32 s32DisAsp_h = 0;	
	
	/* priorty :User cfg / src-frame(keep un-transmutative)*/
    if (HI_TRUE == pstDrvPara->stUsrAsp.bUserDefAspectRatio)
    {
        s32DisAsp_w = pstDrvPara->stUsrAsp.u32UserAspectWidth;
        s32DisAsp_h = pstDrvPara->stUsrAsp.u32UserAspectHeight;
    } 
    else if ((0 != pstDrvPara->AspectWidth) && (0 != pstDrvPara->AspectHeight)
		&& (0 != pstDrvPara->SrcImgW) && (0 != pstDrvPara->SrcImgH) )
    {

        s32DisAsp_w = 256 * pstDrvPara->stInWnd.s32Width * pstDrvPara->AspectWidth / pstDrvPara->SrcImgW;
        s32DisAsp_h = 256 * pstDrvPara->stInWnd.s32Height * pstDrvPara->AspectHeight / pstDrvPara->SrcImgH;
    }
	else
	{
		//do nothting
	}
	
	if (s32DisAsp_w==0 || s32DisAsp_h==0)
	{
		s32DisAsp_w = pstDrvPara->stInWnd.s32Width;
        s32DisAsp_h = pstDrvPara->stInWnd.s32Height;
	}

	/*after ALG_RATIO_AspecRatioCorrect,w!=0 & h!=0 */
    ALG_RATIO_AspecRatioCorrect(&s32DisAsp_w, &s32DisAsp_h);

	*Dasp_h = s32DisAsp_h;
	*Dasp_w = s32DisAsp_w;

	return ;
}


static HI_VOID ALG_RATIO_PixOutAspGet(HI_S32 *pDevPix,\
									ALG_RATIO_IN_PARA_S *pstDrvPara)
{
	HI_S32 Dev_w, Dev_h;		
    HI_S32 DevAsp_w, DevAsp_h;	
    
	/* get device width height aspectratio. */
    DevAsp_w = pstDrvPara->DeviceWidth; 
    DevAsp_h = pstDrvPara->DeviceHeight;
	
    if ((0 == DevAsp_w) || (0 == DevAsp_h))
    {
        DevAsp_w = pstDrvPara->stScreen.s32Width;
        DevAsp_h = pstDrvPara->stScreen.s32Height;
    }
	
    /* after ALG_RATIO_AspecRatioCorrect,w!=0 & h!=0 */
    ALG_RATIO_AspecRatioCorrect(&DevAsp_w , &DevAsp_h );
    
    Dev_w = pstDrvPara->stScreen.s32Width;
    Dev_h = pstDrvPara->stScreen.s32Height;
	if (Dev_h==0 || Dev_w==0)
	{
		/*invalid device w-h , pixout = 1:1*/
		*pDevPix = ALG_RATIO_PIX_RATIO1;	
		return ;
	}

    /* device pix aspectratio ,NOTE:Dev_h limit? */
    *pDevPix= (DevAsp_w * Dev_h * ALG_RATIO_PIX_RATIO1) / (DevAsp_h * Dev_w);

	return ;

}




static HI_VOID ALG_RATIO_LetterBox(ALG_RATIO_OUT_PARA_S *pstOutPara,\
									ALG_RATIO_IN_PARA_S *pstDrvPara)
{	
	HI_S32 s32ExpW;
	HI_S32 s32ExpH;
	HI_S32 s32DisAsp_w = 0;
	HI_S32 s32DisAsp_h = 0;
	HI_S32 s32PixOut;
	
    HI_RECT_S *pOutWnd ;
	pOutWnd = &pstDrvPara->stOutWnd;

	/*decide the display width&height aspectratio*/
	ALG_RATIO_DisAspRatioGet(&s32DisAsp_w, &s32DisAsp_h, pstDrvPara);

	/*calculate the device(output) pixel-wh-aspectratio  */
    ALG_RATIO_PixOutAspGet(&s32PixOut, pstDrvPara);
	
    /* (s32ExpW / s32ExpH) * s32PixOut = s32DisAsp_w / s32DisAsp_h*/
    s32ExpW = (s32DisAsp_w * pOutWnd->s32Height * ALG_RATIO_PIX_RATIO1) / s32DisAsp_h / s32PixOut;

	/*decide the output-win width & height */
    if (s32ExpW <= pOutWnd->s32Width)
    {
        s32ExpW = s32ExpW & RATIO_WIDTH_ALIGN;
        pOutWnd->s32X = (pOutWnd->s32X + (pOutWnd->s32Width - s32ExpW)/2) & RATIO_WIDTH_ALIGN;
        pOutWnd->s32Width = s32ExpW;
    }
    else
    {
		s32ExpH = pOutWnd->s32Width * s32PixOut * s32DisAsp_h / s32DisAsp_w / ALG_RATIO_PIX_RATIO1;
        s32ExpH = s32ExpH & RATIO_HEIGHT_ALIGN;
        pOutWnd->s32Y = (pOutWnd->s32Y + (pOutWnd->s32Height - s32ExpH)/2) & RATIO_HEIGHT_ALIGN;
        pOutWnd->s32Height = s32ExpH;
    }

	/*pstOutPara Crop cfg*/
	pstOutPara->bEnCrop = HI_TRUE;
	pstOutPara->stCropWnd.s32X = 0;
	pstOutPara->stCropWnd.s32Y = 0;
	/*NOTE:use w-h after vpss first logic INCROP.*/
	pstOutPara->stCropWnd.s32Height = pstDrvPara->stInWnd.s32Height & RATIO_WIDTH_ALIGN;
	pstOutPara->stCropWnd.s32Width =  pstDrvPara->stInWnd.s32Width & RATIO_HEIGHT_ALIGN;


	/*pstOutPara output-win cfg*/
	pstOutPara->stOutWnd.s32Height = pstDrvPara->stOutWnd.s32Height;
    pstOutPara->stOutWnd.s32Width = pstDrvPara->stOutWnd.s32Width;
	pstOutPara->stOutWnd.s32X = pstDrvPara->stOutWnd.s32X;
	pstOutPara->stOutWnd.s32Y = pstDrvPara->stOutWnd.s32Y;
	
	return ;
}

static HI_VOID ALG_RATIO_PanAndScan(ALG_RATIO_OUT_PARA_S *pstOutPara,\
									ALG_RATIO_IN_PARA_S *pstDrvPara)
{
	HI_S32 s32ZmeH;
    HI_S32 s32ZmeW;
	HI_S32 s32CropH;
	HI_S32 s32CropW;
    HI_S32 s32ExpH;
	HI_S32 s32ExpW;
	HI_S32 s32DisAsp_w = 0;
	HI_S32 s32DisAsp_h = 0;
    HI_S32 s32PixOut;

	/*decide the display width&height aspectratio*/
	ALG_RATIO_DisAspRatioGet(&s32DisAsp_w, &s32DisAsp_h, pstDrvPara);

	/*calculate the device(output) pixel-wh-aspectratio  */
    ALG_RATIO_PixOutAspGet(&s32PixOut, pstDrvPara);

	/* (s32ExpW / s32ExpH) * s32PixOut = s32DisAsp_w / s32DisAsp_h*/
	s32ExpH = pstDrvPara->stOutWnd.s32Width * s32PixOut * s32DisAsp_h / s32DisAsp_w / ALG_RATIO_PIX_RATIO1;

	if (s32ExpH > pstDrvPara->stOutWnd.s32Height)
	{
		/*indicate win-out's height will be croped*/
		s32ZmeH = s32ExpH;
		s32ZmeW = pstDrvPara->stOutWnd.s32Width;
	}
	else
	{
		/*indicate win-out's width will be croped*/
		s32ExpW = (s32DisAsp_w * pstDrvPara->stOutWnd.s32Height * ALG_RATIO_PIX_RATIO1) / s32DisAsp_h / s32PixOut;
		s32ZmeW = s32ExpW ;
		s32ZmeH = pstDrvPara->stOutWnd.s32Height;
	}
	
	s32CropW = pstDrvPara->stInWnd.s32Width *(s32ZmeW - pstDrvPara->stOutWnd.s32Width) / s32ZmeW;
	s32CropH = pstDrvPara->stInWnd.s32Height *(s32ZmeH - pstDrvPara->stOutWnd.s32Height) / s32ZmeH;

	/*pstOutPara Crop cfg*/
	pstOutPara->bEnCrop = HI_TRUE;
	pstOutPara->stCropWnd.s32X = (s32CropW>>1) & RATIO_WIDTH_ALIGN;
	pstOutPara->stCropWnd.s32Y = (s32CropH>>1) & RATIO_HEIGHT_ALIGN;
	pstOutPara->stCropWnd.s32Height = (pstDrvPara->stInWnd.s32Height- s32CropH) & RATIO_WIDTH_ALIGN;
	pstOutPara->stCropWnd.s32Width =  (pstDrvPara->stInWnd.s32Width - s32CropW)  & RATIO_HEIGHT_ALIGN;

    /*pstOutPara output-win cfg*/    
	pstOutPara->stOutWnd.s32X = (pstDrvPara->stOutWnd.s32X + 0) & RATIO_WIDTH_ALIGN;
	pstOutPara->stOutWnd.s32Y = (pstDrvPara->stOutWnd.s32Y + 0) & RATIO_HEIGHT_ALIGN;
	pstOutPara->stOutWnd.s32Height = pstDrvPara->stOutWnd.s32Height & RATIO_HEIGHT_ALIGN;
	pstOutPara->stOutWnd.s32Width = pstDrvPara->stOutWnd.s32Width & RATIO_WIDTH_ALIGN;
		
	return ;
}

static HI_VOID ALG_RATIO_Combine(ALG_RATIO_OUT_PARA_S *pstOutPara,\
									ALG_RATIO_IN_PARA_S *pstDrvPara)
{

	HI_S32 s32ZmeH;
    HI_S32 s32ZmeW;
    HI_S32 s32ExpH = 0;
	HI_S32 s32ExpW = 0;
	HI_S32 s32DisAsp_w = 0;
	HI_S32 s32DisAsp_h = 0;
    HI_S32 s32PixOut;
	HI_S32 s32CropH;
	HI_S32 s32CropW;
	HI_RECT_S *pOutWnd = HI_NULL;


	/*decide the display width&height aspectratio*/
	ALG_RATIO_DisAspRatioGet(&s32DisAsp_w, &s32DisAsp_h, pstDrvPara);

	/*calculate the device(output) pixel-wh-aspectratio  */
    ALG_RATIO_PixOutAspGet(&s32PixOut, pstDrvPara);

	/* (s32ExpW / s32ExpH) * s32PixOut = s32DisAsp_w / s32DisAsp_h*/
	s32ExpH = pstDrvPara->stOutWnd.s32Width * s32PixOut * s32DisAsp_h / s32DisAsp_w / ALG_RATIO_PIX_RATIO1;
	
	if (s32ExpH > pstDrvPara->stOutWnd.s32Height)
	{
		/*indicate win-out's height will be croped*/
		s32ZmeH = s32ExpH;
		s32ZmeW = pstDrvPara->stOutWnd.s32Width;
	}
	else
	{
		/*indicate win-out's width will be croped*/
		s32ExpW = (s32DisAsp_w * pstDrvPara->stOutWnd.s32Height * ALG_RATIO_PIX_RATIO1) / s32DisAsp_h / s32PixOut;

		s32ZmeW = s32ExpW ;
		s32ZmeH = pstDrvPara->stOutWnd.s32Height;
	}

	s32CropW = pstDrvPara->stInWnd.s32Width *(s32ZmeW - pstDrvPara->stOutWnd.s32Width) / s32ZmeW / 2 ;
	s32CropH = pstDrvPara->stInWnd.s32Height *(s32ZmeH - pstDrvPara->stOutWnd.s32Height) / s32ZmeH / 2;

	/*pstOutPara Crop cfg*/
	pstOutPara->bEnCrop = HI_TRUE;
	pstOutPara->stCropWnd.s32X = (s32CropW >> 1) & RATIO_WIDTH_ALIGN;
	pstOutPara->stCropWnd.s32Y = (s32CropH >> 1) & RATIO_HEIGHT_ALIGN;
	pstOutPara->stCropWnd.s32Height = (pstDrvPara->stInWnd.s32Height - s32CropH) & RATIO_HEIGHT_ALIGN;
	pstOutPara->stCropWnd.s32Width =  (pstDrvPara->stInWnd.s32Width - s32CropW)  & RATIO_WIDTH_ALIGN;

	pOutWnd = &pstDrvPara->stOutWnd;
	/*re-caculate the expective display w-h-aspectratio.*/
	pstDrvPara->stInWnd.s32Width = pstOutPara->stCropWnd.s32Width;
    pstDrvPara->stInWnd.s32Height = pstOutPara->stCropWnd.s32Height;
	ALG_RATIO_DisAspRatioGet(&s32DisAsp_w, &s32DisAsp_h, pstDrvPara);

	/* (s32ExpW / s32ExpH) * s32PixOut = s32DisAsp_w / s32DisAsp_h*/
    s32ExpW = (s32DisAsp_w * pOutWnd->s32Height * ALG_RATIO_PIX_RATIO1) / (s32DisAsp_h * s32PixOut);

	/*decide the output-win width & height */
    if (s32ExpW <= pOutWnd->s32Width)
    {
        s32ExpW = s32ExpW & RATIO_WIDTH_ALIGN;
        pOutWnd->s32X = (pOutWnd->s32X + (pOutWnd->s32Width - s32ExpW)/2) & RATIO_WIDTH_ALIGN;
        pOutWnd->s32Width = s32ExpW;
    }
    else
    {
		s32ExpH = pOutWnd->s32Width * s32PixOut * s32DisAsp_h / s32DisAsp_w / ALG_RATIO_PIX_RATIO1;
        s32ExpH = s32ExpH & RATIO_HEIGHT_ALIGN;

        pOutWnd->s32Y = (pOutWnd->s32Y + (pOutWnd->s32Height - s32ExpH)/2) & RATIO_HEIGHT_ALIGN;
        pOutWnd->s32Height = s32ExpH ;
    }


	pstOutPara->stOutWnd.s32Height = pstDrvPara->stOutWnd.s32Height;
    pstOutPara->stOutWnd.s32Width = pstDrvPara->stOutWnd.s32Width;
	pstOutPara->stOutWnd.s32X = pstDrvPara->stOutWnd.s32X;
	pstOutPara->stOutWnd.s32Y = pstDrvPara->stOutWnd.s32Y;
	
	return ;
}


HI_S32 WIN_RATIO_GetAspCfg(ALG_RATIO_OUT_PARA_S *pstOutPara,\
								ALG_RATIO_IN_PARA_S *pstDrvPara)
{  
	if (HI_NULL==pstDrvPara || HI_NULL==pstOutPara)
	{
		HI_FATAL_WIN("param invalid!\n");
		return HI_FAILURE;
	}

	if ( pstDrvPara->stOutWnd.s32Width==0 || pstDrvPara->stOutWnd.s32Width > RATIO_WIDTH_MAX 
		|| pstDrvPara->stOutWnd.s32Height==0 || pstDrvPara->stOutWnd.s32Height > RATIO_WIDTH_MAX 
		|| pstDrvPara->stInWnd.s32Height == 0 || pstDrvPara->stInWnd.s32Height > RATIO_WIDTH_MAX 
		|| pstDrvPara->stInWnd.s32Width == 0 || pstDrvPara->stInWnd.s32Width > RATIO_WIDTH_MAX 
		|| pstDrvPara->SrcImgW == 0 || pstDrvPara->SrcImgW > RATIO_WIDTH_MAX 
		|| pstDrvPara->SrcImgH == 0 || pstDrvPara->SrcImgH > RATIO_WIDTH_MAX  
		|| pstDrvPara->stScreen.s32Width > RATIO_WIDTH_MAX || pstDrvPara->stScreen.s32Height > RATIO_WIDTH_MAX)
	{
		HI_FATAL_WIN("pstDrvPara Param invalid!\n");
		return HI_FAILURE;
	}

	switch(pstDrvPara->eAspMode)
	{
		case HI_DRV_ASP_RAT_MODE_LETTERBOX :
			ALG_RATIO_LetterBox(pstOutPara,pstDrvPara);  
			break;
			
		case HI_DRV_ASP_RAT_MODE_PANANDSCAN :
			ALG_RATIO_PanAndScan(pstOutPara,pstDrvPara);
			break;

		case HI_DRV_ASP_RAT_MODE_COMBINED :
			ALG_RATIO_Combine(pstOutPara,pstDrvPara);
			break;

		default:
			HI_FATAL_WIN("AspMode invalid!\n");
			return HI_FAILURE;
			break;
	} 
	
    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
