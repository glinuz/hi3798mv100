
#ifndef __VDP_ALG_RATIO_H__
#define __VDP_ALG_RATIO_H__
//#include"vpss_common.h"
#include"hi_drv_video.h"

#define VDP_HEIGHT_ALIGN 0xfffffffc
#define VDP_WIDTH_ALIGN 0xfffffffe
#define ALG_RATIO_PIX_RATIO1 1024
typedef struct tagVDP_ALG_RATIO_USR_ASPECTRATIO_S
{
	HI_BOOL bUserDefAspectRatio;
	HI_U32 u32UserAspectWidth;
	HI_U32 u32UserAspectHeight;
}VDP_ALG_RATIO_USR_ASPECTRATIO_S;


typedef struct{
    HI_U32 AspectWidth;
    HI_U32 AspectHeight;
    HI_U32 DeviceWidth;
    HI_U32 DeviceHeight;
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    VDP_ALG_RATIO_USR_ASPECTRATIO_S stUsrAsp;
    HI_RECT_S stInWnd;
	HI_RECT_S stSrcRect;
    HI_RECT_S stOutWnd;
    HI_RECT_S stScreen;
}VDP_ALG_RATIO_DRV_PARA_S;


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
}VDP_ALG_RATIO_OUT_PARA_S;


HI_S32 VDP_ALG_RATIO_RatioProcess(VDP_ALG_RATIO_DRV_PARA_S *pstDrvPara,VDP_ALG_RATIO_OUT_PARA_S *pstOutPara);

HI_VOID VDP_ALG_RATIO_LetterBox(HI_U32 AspectRatioW, HI_U32 AspectRatioH,HI_RECT_S *pOutWnd,HI_S32 pixr1_out);
HI_VOID VDP_ALG_RATIO_CropedAspect(HI_RECT_S *pInWnd,HI_RECT_S *pCropedWnd,HI_U32 *AspectRatioW, HI_U32 *AspectRatioH);
HI_VOID VDP_ALG_RATIO_CorrectAspectRatioW_H(HI_U32 *pw, HI_U32 *ph);

HI_S32 VDP_ALG_GetAspCfg(VDP_ALG_RATIO_DRV_PARA_S *pstAspDrvPara,
                        HI_DRV_ASP_RAT_MODE_E eAspMode,HI_RECT_S *pstScreen,
                        VDP_ALG_RATIO_OUT_PARA_S *pstAspCfg);

#endif /*__ALG_ASP_H__*/