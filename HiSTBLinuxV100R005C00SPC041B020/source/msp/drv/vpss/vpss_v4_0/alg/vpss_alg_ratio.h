#ifndef __VPSS_ALG_RATIO_H__
#define __VPSS_ALG_RATIO_H__
#include"vpss_common.h"
#include"hi_drv_video.h"

#define ALG_RATIO_PIX_RATIO1 864
typedef struct tagALG_RATIO_USR_ASPECTRATIO_S
{
	HI_BOOL bUserDefAspectRatio;
	HI_U32 u32UserAspectWidth;
	HI_U32 u32UserAspectHeight;
}ALG_RATIO_USR_ASPECTRATIO_S;


typedef struct{
    HI_U32 AspectWidth;
    HI_U32 AspectHeight;
    HI_U32 DeviceWidth;
    HI_U32 DeviceHeight;
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    ALG_RATIO_USR_ASPECTRATIO_S stUsrAsp;
    HI_RECT_S stInWnd;
    HI_RECT_S stOutWnd;
    HI_RECT_S stScreen;
}ALG_RATIO_DRV_PARA_S;


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


typedef struct{
    HI_S32 s32Screen_skew;
    HI_S32 s32SrcAsp_W;
    HI_S32 s32SrcAsp_H;
}ALG_ASP_PARA_S;



HI_S32 ALG_ASP_GetConfig(ALG_RATIO_DRV_PARA_S *pstAspDrvPara,
                        HI_DRV_ASP_RAT_MODE_E eAspMode,HI_RECT_S *pstScreen,
                        ALG_RATIO_OUT_PARA_S *pstAspCfg);

#endif /*__ALG_CSC_H__*/