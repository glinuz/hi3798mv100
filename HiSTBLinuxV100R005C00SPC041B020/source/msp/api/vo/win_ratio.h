
#ifndef __WIN_RATIO_H__
#define __WIN_RATIO_H__
#include"hi_drv_video.h"

#define ALG_RATIO_PIX_RATIO1 1024
typedef struct tagALG_RATIO_USR_ASPECTRATIO_S
{
	HI_BOOL bUserDefAspectRatio;
	HI_U32 u32UserAspectWidth;
	HI_U32 u32UserAspectHeight;
}ALG_RATIO_USR_ASPECTRATIO_S;


typedef struct{
	HI_S32 SrcImgH;			//src img w-h
	HI_S32 SrcImgW;			
    HI_U32 AspectWidth;		//src img w-h-aspecratio
    HI_U32 AspectHeight;
    HI_U32 DeviceWidth;		//device w-h-aspecratio
    HI_U32 DeviceHeight;
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    ALG_RATIO_USR_ASPECTRATIO_S stUsrAsp;
    HI_RECT_S stInWnd;		//img w-h after vpss first INCROP.
    HI_RECT_S stOutWnd;		//(1)port w-h ;(2) src img w-h(case (1)invalid).
    HI_RECT_S stScreen;		//device w-h
}ALG_RATIO_IN_PARA_S;


typedef struct{
    HI_RECT_S stCropWnd;
    HI_RECT_S stOutWnd;
    HI_RECT_S stOutScreen;
    HI_U32 u32ZmeH;
    HI_U32 u32ZmeW;
    HI_BOOL bEnAsp;
    HI_BOOL bEnCrop;
    HI_U32 u32BgColor;
    HI_U32 u32BgAlpha;
}ALG_RATIO_OUT_PARA_S;



HI_S32 WIN_RATIO_GetAspCfg(ALG_RATIO_OUT_PARA_S *pstAspCfg,
		ALG_RATIO_IN_PARA_S *pstAspDrvPara);

#endif /*__ALG_CSC_H__*/
