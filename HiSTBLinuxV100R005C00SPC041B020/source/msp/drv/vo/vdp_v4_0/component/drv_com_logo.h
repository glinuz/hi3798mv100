#ifndef  __DRV_COM_LOGO_H_
#define  __DRV_COM_LOGO_H_

#include "hi_type.h"
#include "hi_drv_video.h"

#define  LOGO_WBC_MEM "vdp_logo_mem"
#define  VDP_DDR_NAME_UBOOT "VDP_DDR_CFG"

#if (defined(HI_MINIBOOT_SUPPORT) ||  defined(__DISP_PLATFORM_BOOT__))
#include "hi_unf_disp.h"
typedef struct hiVideoLogoInfoS
{
    HI_BOOL bWbcIso;
    HI_DRV_PIX_FORMAT_E ePixelFmt;
    
    HI_U32 u32LogoWidth;
    HI_U32 u32LogoHeight;
    
    HI_U32 u32LogoYStride;
    HI_U32 u32LogoCbStride;
    HI_U32 u32LogoCrStride;
    
    HI_U32 u32LogoYAddr;
    HI_U32 u32LogoCbAddr;
    HI_U32 u32LogoCrAddr;
    
    HI_UNF_DISP_E eDispChannelId;
}HI_VIDEO_LOGO_INFO;

typedef struct hiVideoDisplayInfo_S
{
	HI_U32 u32LogoYStride;
    HI_U32 u32LogoCbStride;
    HI_U32 u32LogoCrStride;
    
    HI_U32 u32LogoYAddr;
    HI_U32 u32LogoCbAddr;
    HI_U32 u32LogoCrAddr;
	
    HI_RECT_S stInRect;
    HI_RECT_S stOutRect;
	HI_RECT_S stFmtResolution;
	HI_DRV_COLOR_SPACE_E eColorSpace;
	HI_BOOL bWbcIso;
}HI_VIDEO_DISPLAY_Infor_S;

HI_S32  HI_UBOOT_LOGO_CreateLayer(HI_VIDEO_LOGO_INFO *pstLogoInfor);
HI_S32  HI_UBOOT_LOGO_RefreshLayer(HI_VIDEO_LOGO_INFO *pstLogoInfor);
HI_S32  HI_UBOOT_LOGO_DestoryLayer(HI_VIDEO_LOGO_INFO *pstLogoInfor);
#else

HI_VOID HI_DRV_LOGO_FreeLogoMem(HI_VOID);
HI_VOID HI_DRV_LOGO_GetLogoInfor(HI_DRV_DISP_VideoLogoInfo *pstLogoInfor);
#endif

#endif
