#include "drv_disp_ext.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"

#include "hi_drv_pdm.h"
#include "drv_com_logo.h"
#include "hi_drv_pdm.h"
#include "hi_drv_module.h"

#include "vdp_software_selfdefine.h"
#include "vdp_hal_intf.h"


#ifndef HI_MINIBOOT_SUPPORT
#include <linux/string.h>
#else
#include <string.h>
#endif

#if (!defined(HI_MINIBOOT_SUPPORT) &&  !defined(__DISP_PLATFORM_BOOT__))
#include "drv_pdm_ext.h"
#include <linux/delay.h>

#include <linux/kthread.h>
#include "hi_kernel_adapt.h"

HI_DECLARE_MUTEX(g_LogoFreeLock);
HI_U32 g_u32LogoFreeFlag = HI_FALSE;

#endif

#define HI_VIDEO_LAYER_MAXCOUNT       6
#define VDP_LOGO_WBC_MEM_SIZE 0x12FC00/*720*576*3*/
#define WBC_VP_STRIDE         720/*720*576*3*/


#if (defined(HI_MINIBOOT_SUPPORT) ||  defined(__DISP_PLATFORM_BOOT__))


static HI_VIDEO_DISPLAY_Infor_S gs_stDisplayInfo[HI_VIDEO_LAYER_MAXCOUNT];

static HI_VOID TransvertPixelFmt2CBB(HI_DRV_PIX_FORMAT_E eInputPixelFmt,CBB_PIX_FORMAT_E  *peOutPixelFmt)
{
	switch(eInputPixelFmt)
	{
		case HI_DRV_PIX_FMT_NV12:
		case HI_DRV_PIX_FMT_NV21:		
		case HI_DRV_PIX_FMT_NV21_CMP:
		case HI_DRV_PIX_FMT_NV12_CMP:
			*peOutPixelFmt = CBB_VID_IFMT_SP_420;
			break;
		case HI_DRV_PIX_FMT_NV16_2X1:
		case HI_DRV_PIX_FMT_NV61_2X1:
			*peOutPixelFmt = CBB_VID_IFMT_SP_422;
			break;
		case HI_DRV_PIX_FMT_YUYV:
			*peOutPixelFmt = CBB_VID_IFMT_PKG_YUYV;
			break;
		case HI_DRV_PIX_FMT_YVYU:
			*peOutPixelFmt = CBB_VID_IFMT_PKG_YVYU;			
			break;
		case HI_DRV_PIX_FMT_UYVY:
			*peOutPixelFmt = CBB_VID_IFMT_PKG_UYVY;
			break;
		case HI_DRV_PIX_FMT_NV21_TILE:			
		case HI_DRV_PIX_FMT_NV21_TILE_CMP:
			*peOutPixelFmt = CBB_VID_IFMT_SP_TILE_64;
			break;
		default:
			HI_ERR_DISP("Invalid pixel fmt.\n");
			break;
	}

	return ;
}

static HI_U32 Coordinate_ParaAlign(HI_U32 x, HI_U32 a)
{
    if(!a){
        return x;
    }else{
        return ( (( x + (a-1) ) / a ) * a);
    }
}

static HI_VOID Win_ReviseOutRect(const HI_RECT_S *tmp_virtscreen,
                          const HI_DRV_DISP_OFFSET_S *stOffsetInfo,
                          const HI_RECT_S *stFmtResolution,
                          const HI_RECT_S *stPixelFmtResolution,
                          HI_RECT_S *stToBeRevisedRect,
                          HI_RECT_S *stRevisedRect)
{    
    HI_U32 width_ratio   = 0;
    HI_U32 height_ratio  = 0;
    HI_U32 zmeDestWidth  = 0;
    HI_U32 zmeDestHeight = 0;
    HI_DRV_DISP_OFFSET_S tmp_offsetInfo;

    tmp_offsetInfo = *stOffsetInfo;

    if ( (stFmtResolution->s32Width * 2) == stPixelFmtResolution->s32Width){
        tmp_offsetInfo.u32Left  *= 2;
        tmp_offsetInfo.u32Right *= 2;        
    }

    zmeDestWidth = (stPixelFmtResolution->s32Width - tmp_offsetInfo.u32Left - tmp_offsetInfo.u32Right);
    zmeDestHeight = (stPixelFmtResolution->s32Height - tmp_offsetInfo.u32Top - tmp_offsetInfo.u32Bottom);

    width_ratio  = zmeDestWidth  * 100 /(tmp_virtscreen->s32Width);
    height_ratio = zmeDestHeight * 100 /(tmp_virtscreen->s32Height);    

    if (tmp_virtscreen->s32Width != stToBeRevisedRect->s32Width){
        stRevisedRect->s32Width = (stToBeRevisedRect->s32Width * width_ratio) / 100;         
    }else{
        stRevisedRect->s32Width = zmeDestWidth;
    }
    
    if (tmp_virtscreen->s32Height != stToBeRevisedRect->s32Height){
        stRevisedRect->s32Height = (stToBeRevisedRect->s32Height * height_ratio) / 100;      
    }else{
        stRevisedRect->s32Height = zmeDestHeight;
    }
    
        
    stRevisedRect->s32X = (stToBeRevisedRect->s32X * width_ratio) /100 + tmp_offsetInfo.u32Left;
    stRevisedRect->s32Y = (stToBeRevisedRect->s32Y  * height_ratio) /100 + tmp_offsetInfo.u32Top;
    
    stRevisedRect->s32X      = Coordinate_ParaAlign(stRevisedRect->s32X, 2);
    stRevisedRect->s32Y      = Coordinate_ParaAlign(stRevisedRect->s32Y , 2);
    stRevisedRect->s32Width  = Coordinate_ParaAlign(stRevisedRect->s32Width, 2);
    stRevisedRect->s32Height = Coordinate_ParaAlign(stRevisedRect->s32Height, 2);
    
    return;
}


static HI_S32 Logo_TransferChannelId2VideoLayerId(HI_UNF_DISP_E eDispChannel)
{
	switch(eDispChannel)
	{
		case HI_UNF_DISPLAY1:
			return 0;
		case HI_UNF_DISPLAY0:
			return 3;
		default:
			return 0;
	}
	
	return 0;
}
static HI_S32 Get_LogoDisplayInfo(HI_VIDEO_LOGO_INFO *pstLogoInfor,
	                                   HI_VIDEO_DISPLAY_Infor_S *pDisplayInfo)
{
    HI_DISP_DISPLAY_INFO_S stInfo = {0};
	
    if(NULL == pstLogoInfor){
		return HI_FAILURE;
    }	

	HI_DISP_GetDisplayInfo(pstLogoInfor->eDispChannelId, &stInfo);
	
	memset(pDisplayInfo, 0, sizeof(HI_VIDEO_DISPLAY_Infor_S));
	
	
    pDisplayInfo->stInRect.s32Width          = pstLogoInfor->u32LogoWidth;
    pDisplayInfo->stInRect.s32Height         = pstLogoInfor->u32LogoHeight;

	
	pDisplayInfo->eColorSpace                = stInfo.eColorSpace;
	pDisplayInfo->stFmtResolution            = stInfo.stFmtResolution;
	
	pDisplayInfo->u32LogoYStride    		 = pstLogoInfor->u32LogoYStride;
	pDisplayInfo->u32LogoCbStride    		 = pstLogoInfor->u32LogoCbStride;
	pDisplayInfo->u32LogoCrStride    		 = pstLogoInfor->u32LogoCrStride;
	
	pDisplayInfo->u32LogoYAddr    			= pstLogoInfor->u32LogoYAddr;
	pDisplayInfo->u32LogoCbAddr    			= pstLogoInfor->u32LogoCbAddr;
	pDisplayInfo->u32LogoCrAddr    			= pstLogoInfor->u32LogoCrAddr;

	pDisplayInfo->bWbcIso                   = pstLogoInfor->bWbcIso;
	
	Win_ReviseOutRect(&stInfo.stVirtaulScreen,      \
    	              &stInfo.stOffsetInfo,         \
                      &stInfo.stFmtResolution,      \
                      &stInfo.stPixelFmtResolution, \
                      &stInfo.stVirtaulScreen,      \
                      (HI_RECT_S*)(&(pDisplayInfo->stOutRect)));
	
    return HI_SUCCESS;
}

HI_VOID  VideoLogo_GenarateLayerConfig(HI_S32 s32DataId,
												CBB_LAYER_CFG_INFO_S *pstLayerCfg,
                                                HI_VIDEO_DISPLAY_Infor_S *pDisplayInfo,
                                                CBB_PIX_FORMAT_E enLayerFmt)
{
	HI_DISP_DISPLAY_INFO_S stInfo;

	memset((HI_VOID*)&stInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));
		
	if(s32DataId < 3){
		HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_1, &stInfo);  
	}else{
		HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_0, &stInfo);
	}    
	
	pstLayerCfg->u32LayerNO = s32DataId;
	pstLayerCfg->u32RegionNO = 0;
	pstLayerCfg->u32LayerZorder = 0;
	
	pstLayerCfg->bRegionModeEn = HI_FALSE;
	pstLayerCfg->bRegionEn = HI_TRUE;
	pstLayerCfg->bMuteEn = HI_FALSE;	
	pstLayerCfg->bFlipEn = HI_FALSE;
	
	pstLayerCfg->stCropRect.s32X            = pDisplayInfo->stInRect.s32X;
	pstLayerCfg->stCropRect.s32Y            = pDisplayInfo->stInRect.s32Y;	
	pstLayerCfg->stCropRect.s32Width            = pDisplayInfo->stInRect.s32Width;	
	pstLayerCfg->stCropRect.s32Height            = pDisplayInfo->stInRect.s32Height;	

	
	pstLayerCfg->stVideoRect.s32X            = pDisplayInfo->stOutRect.s32X;
	pstLayerCfg->stVideoRect.s32Y            = pDisplayInfo->stOutRect.s32Y;	
	pstLayerCfg->stVideoRect.s32Width            = pDisplayInfo->stOutRect.s32Width;	
	pstLayerCfg->stVideoRect.s32Height            = pDisplayInfo->stOutRect.s32Height;	

	pstLayerCfg->stDispRect.s32X            = pDisplayInfo->stFmtResolution.s32X;
	pstLayerCfg->stDispRect.s32Y            = pDisplayInfo->stFmtResolution.s32Y;	
	pstLayerCfg->stDispRect.s32Width            = pDisplayInfo->stFmtResolution.s32Width;	
	pstLayerCfg->stDispRect.s32Height            = pDisplayInfo->stFmtResolution.s32Height;		
	
	pstLayerCfg->enOutColorSpace       = pDisplayInfo->eColorSpace;
	
	pstLayerCfg->stFrame.stSrcRect.s32X        = pDisplayInfo->stInRect.s32X;
	pstLayerCfg->stFrame.stSrcRect.s32Y        = pDisplayInfo->stInRect.s32Y;
	pstLayerCfg->stFrame.stSrcRect.s32Width    = pDisplayInfo->stInRect.s32Width;
	pstLayerCfg->stFrame.stSrcRect.s32Height   = pDisplayInfo->stInRect.s32Height;

	/*sm444/sm422/sm420, decided by what??? */
    if ((enLayerFmt == CBB_VID_IFMT_SP_420) 
		 || (enLayerFmt == CBB_VID_IFMT_SP_422))
    {
		pstLayerCfg->stFrame.stAddress[0].u32PhyAddr_Y = pDisplayInfo->u32LogoYAddr;		
     	pstLayerCfg->stFrame.stAddress[0].u32Stride_Y =  pDisplayInfo->u32LogoYStride;
		
	    pstLayerCfg->stFrame.stAddress[0].u32PhyAddr_C = pDisplayInfo->u32LogoCbAddr; 
	    pstLayerCfg->stFrame.stAddress[0].u32Stride_C = pDisplayInfo->u32LogoCbStride;
    }
	
	pstLayerCfg->stFrame.bSecure = HI_FALSE;
	pstLayerCfg->stFrame.bSmmu = HI_FALSE;
	pstLayerCfg->stFrame.bFrameUseFul = HI_TRUE;
	
	pstLayerCfg->stFrame.bUvorder = HI_FALSE;
	pstLayerCfg->stFrame.bCmpFmt = HI_FALSE;
	pstLayerCfg->stFrame.bLostCmp = HI_FALSE;
	pstLayerCfg->stFrame.bFrameInterlace   = HI_FALSE;
	
	pstLayerCfg->stFrame.eSrcFmt = enLayerFmt;
	pstLayerCfg->stFrame.enSrcColorSpace = CBB_CSC_BT709_YUV_LIMITED;
	pstLayerCfg->stFrame.enSrcBitWidth   = CBB_PIXEL_BITWIDTH_8BIT;
	pstLayerCfg->stFrame.eFieldMode      = CBB_FIELD_ALL;
	pstLayerCfg->stFrame.en3dSrcFormat   = CBB_FT_NOT_STEREO;
	pstLayerCfg->stFrame.u32DepthLevel   = 0;
	pstLayerCfg->stFrame.u32ViewLevel    = 0;

	pstLayerCfg->stDispInfo.bInterlace   =	stInfo.bInterlace;
	pstLayerCfg->stDispInfo.eDispMode   =	CBB_DISP_STEREO_NONE;
	pstLayerCfg->stDispInfo.stFmtResolution.s32X  =	0;
	pstLayerCfg->stDispInfo.stFmtResolution.s32Y  =	0;
	pstLayerCfg->stDispInfo.stFmtResolution.s32Width =	stInfo.stFmtResolution.s32Width;
	pstLayerCfg->stDispInfo.stFmtResolution.s32Height =	stInfo.stFmtResolution.s32Height;
	
}

HI_VOID  VideoLogo_GenarateWbcConfig(CBB_WBC_CFG_INFO_S   *pstWbcCfgBasePara,
	                                           HI_VIDEO_DISPLAY_Infor_S *pDisplayInfo)
{
	HI_DISP_DISPLAY_INFO_S stInfo = {0};
	HI_U32 u32PhyAddr = 0;
	
	if(HI_FALSE == pDisplayInfo->bWbcIso)
	{
		return;
	}
	if (HI_SUCCESS != HI_DRV_PDM_AllocReserveMem(LOGO_WBC_MEM, VDP_LOGO_WBC_MEM_SIZE, &u32PhyAddr))
    {
		HI_ERR_DISP("Allocate reserved mem failed!\n");
        return ;
    }
	
	pstWbcCfgBasePara->enWBC = DRV_VDP_WBC_WIN;
	pstWbcCfgBasePara->bWbcEnable = HI_TRUE;
	pstWbcCfgBasePara->eOutPutPixelFmt = CBB_VID_IFMT_SP_422;
	pstWbcCfgBasePara->eFieldMode = CBB_FIELD_ALL;
	pstWbcCfgBasePara->enSrcColorSpace = CBB_CSC_BT709_YUV_LIMITED;
	pstWbcCfgBasePara->enDestColorSpace = CBB_CSC_BT601_YUV_LIMITED;	

	


	pstWbcCfgBasePara->stHdWinOutputSize.s32X = pDisplayInfo->stOutRect.s32X;
	pstWbcCfgBasePara->stHdWinOutputSize.s32Y = pDisplayInfo->stOutRect.s32Y;
	pstWbcCfgBasePara->stHdWinOutputSize.s32Width = pDisplayInfo->stOutRect.s32Width;
	pstWbcCfgBasePara->stHdWinOutputSize.s32Height = pDisplayInfo->stOutRect.s32Height;

	/*get sd's fmt*/
	HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_0, &stInfo);
	pstWbcCfgBasePara->stSdWinOutputSize      = stInfo.stFmtResolution;
	
	pstWbcCfgBasePara->stAddr.u32PhyAddr_Y = u32PhyAddr;
	pstWbcCfgBasePara->stAddr.u32Stride_Y = WBC_VP_STRIDE;
	
	pstWbcCfgBasePara->stAddr.u32PhyAddr_C = u32PhyAddr 
		                                     + WBC_VP_STRIDE * pstWbcCfgBasePara->stSdWinOutputSize.s32Height;
	pstWbcCfgBasePara->stAddr.u32Stride_C = WBC_VP_STRIDE;
	
	pstWbcCfgBasePara->stAddr.u32PhyAddr_Cr = 0;
	pstWbcCfgBasePara->stAddr.u32Stride_Cr = 0;
	
	
	pstWbcCfgBasePara->bSecure = DRV_VDP_WBC_WIN;
	pstWbcCfgBasePara->bSmmu = HI_FALSE;
	pstWbcCfgBasePara->bForceWbcFieldMode = HI_FALSE;
	pstWbcCfgBasePara->bForceWbcPoint2Vp = HI_TRUE;

    if (pstWbcCfgBasePara->bForceWbcPoint2Vp == HI_TRUE)
    {
		HI_DISP_GetDisplayInfo(HI_DRV_DISPLAY_1, &stInfo);
		pstWbcCfgBasePara->stHdWinInputSize = stInfo.stFmtResolution;
    }
	else
	{
		pstWbcCfgBasePara->stHdWinInputSize.s32X = pDisplayInfo->stInRect.s32X;
		pstWbcCfgBasePara->stHdWinInputSize.s32Y = pDisplayInfo->stInRect.s32Y;
		pstWbcCfgBasePara->stHdWinInputSize.s32Width = pDisplayInfo->stInRect.s32Width;
		pstWbcCfgBasePara->stHdWinInputSize.s32Height = pDisplayInfo->stInRect.s32Height;
	}
	
	
	pstWbcCfgBasePara->enHdWinSrcColorSpace  = CBB_CSC_BT709_YUV_LIMITED;
	pstWbcCfgBasePara->enHdWinDestColorSpace = pDisplayInfo->eColorSpace;

	return;
	
}

static HI_S32 CreateAndEnableLayer(HI_VIDEO_LOGO_INFO *pstLogoInfor, HI_S32 s32DataId)
{
	HI_S32 s32Ret    = HI_SUCCESS;
	
	CBB_LAYER_CFG_INFO_S stLayerCfgBasePara;
	CBB_PIX_FORMAT_E enLayerFmt = CBB_VID_IFMT_BUTT;
	HI_VIDEO_DISPLAY_Infor_S *pDisplayInfo = NULL;
	
    if(HI_NULL == pstLogoInfor){
		return HI_FAILURE;
	}    
	
    pDisplayInfo = gs_stDisplayInfo + s32DataId;	
    s32Ret = Get_LogoDisplayInfo(pstLogoInfor, pDisplayInfo);
    if (s32Ret != HI_SUCCESS){
        return HI_FAILURE;
    }
	
	memset((HI_VOID*)&stLayerCfgBasePara, 0, sizeof(CBB_LAYER_CFG_INFO_S));	
	
	TransvertPixelFmt2CBB(pstLogoInfor->ePixelFmt, &enLayerFmt);
	
	/*step 2:  genarate V0's configure, but not to enable.*/
	VideoLogo_GenarateLayerConfig(s32DataId,
	                              &stLayerCfgBasePara,
	                              pDisplayInfo,
	                              enLayerFmt);
	
	/*step 3: call vdp's hal func, to configure the hardware.*/
    CBB_Video_SetLayerParam(&stLayerCfgBasePara, HI_NULL);
	
    return HI_SUCCESS;    
}

static HI_S32 Local_CreateAndEnableHdChannel(HI_VIDEO_LOGO_INFO *pstLogoInfor, HI_S32 s32DataId)
{	
    return CreateAndEnableLayer(pstLogoInfor, s32DataId);    
}

static HI_S32 Local_CreateAndEnableSdChannel(HI_VIDEO_LOGO_INFO *pstLogoInfor,
	                                                   HI_S32 s32DataId)
{
	CBB_WBC_CFG_INFO_S   stWbcCfgBasePara;
	HI_VIDEO_DISPLAY_Infor_S *pDisplayInfo = NULL;
	HI_VIDEO_LOGO_INFO stSdPictureInfor;
	HI_DISP_DISPLAY_INFO_S stInfo = {0};
	
	memset((HI_VOID*)&stWbcCfgBasePara, 0, sizeof(CBB_WBC_CFG_INFO_S));
	memset((HI_VOID*)&stSdPictureInfor, 0, sizeof(HI_VIDEO_LOGO_INFO));
	
	pDisplayInfo = gs_stDisplayInfo + s32DataId;	

	/*genarate wbc config  and  enable wbc.*/
	VideoLogo_GenarateWbcConfig(&stWbcCfgBasePara, pDisplayInfo);
	CBB_Video_SetWbcParam(&stWbcCfgBasePara, HI_NULL);
	
	stSdPictureInfor.bWbcIso = HI_FALSE;
	stSdPictureInfor.ePixelFmt = HI_DRV_PIX_FMT_NV61_2X1;
	stSdPictureInfor.eDispChannelId = HI_DRV_DISPLAY_0;
	
	HI_DISP_GetDisplayInfo(stSdPictureInfor.eDispChannelId, &stInfo);
	
	stSdPictureInfor.u32LogoWidth = stInfo.stFmtResolution.s32Width;
	stSdPictureInfor.u32LogoHeight = stInfo.stFmtResolution.s32Height;
	stSdPictureInfor.u32LogoYStride = stWbcCfgBasePara.stAddr.u32Stride_Y;
	stSdPictureInfor.u32LogoCbStride = stWbcCfgBasePara.stAddr.u32Stride_C;
	stSdPictureInfor.u32LogoCrStride = stWbcCfgBasePara.stAddr.u32Stride_Cr;

	stSdPictureInfor.u32LogoYAddr    = stWbcCfgBasePara.stAddr.u32PhyAddr_Y;
	stSdPictureInfor.u32LogoCbAddr   = stWbcCfgBasePara.stAddr.u32PhyAddr_C;
	stSdPictureInfor.u32LogoCrAddr   = stWbcCfgBasePara.stAddr.u32PhyAddr_Cr;

#if 0
	printf("v3:%x,%x,%x!\n", stSdPictureInfor.u32LogoYAddr, 
		                        stSdPictureInfor.u32LogoCbAddr,
		                        stSdPictureInfor.u32LogoCrAddr);
#endif

	/*config v3 layer.*/
	CreateAndEnableLayer(&stSdPictureInfor, 3);

	return HI_SUCCESS;
}
HI_S32 HI_UBOOT_LOGO_CreateLayer(HI_VIDEO_LOGO_INFO *pstLogoInfor)
{
	HI_S32 s32DataId = 0;
	
	if(HI_NULL == pstLogoInfor)
	{
		return HI_FAILURE;
	}
	
	if (HI_UNF_DISPLAY1 == pstLogoInfor->eDispChannelId)
	{
		CBB_Video_Init(HI_NULL, HI_NULL);
	}
	
	s32DataId = Logo_TransferChannelId2VideoLayerId(pstLogoInfor->eDispChannelId);
	if (HI_FAILURE == s32DataId)
	{
		return HI_FAILURE;
	}
	
	(HI_VOID)Local_CreateAndEnableHdChannel(pstLogoInfor, s32DataId);	
	if (HI_TRUE == pstLogoInfor->bWbcIso)
	{
		(HI_VOID)Local_CreateAndEnableSdChannel(pstLogoInfor, s32DataId);
	}
	
    return HI_SUCCESS;    
}


/*****************************************************************************
* func            : HI_GFX_ADP_RefreshLayer
* description     : Ë¢ÐÂÍ¼²ã
* in              : Í¼²ã²ÎÊý
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_UBOOT_LOGO_RefreshLayer(HI_VIDEO_LOGO_INFO *pstLogoInfor)
{

	HI_S32 s32DataId = 0;	
	if(NULL == pstLogoInfor){
		HI_ERR_DISP("null logoInfor!\n");
		return HI_FAILURE;
	}
	
    s32DataId = Logo_TransferChannelId2VideoLayerId(pstLogoInfor->eDispChannelId);
	if(HI_FAILURE == s32DataId){
		HI_ERR_DISP("GetInvalid channel.!\n");
		return HI_FAILURE;
	}
	
	CBB_Video_SetLayerEnable(s32DataId, 0, HI_TRUE);	
    return HI_SUCCESS;
}

/*****************************************************************************
* func            : HI_GFX_ADP_DestoryLayer
* description     : Ïú»ÙÍ¼²ã
* in              : Í¼²ã²ÎÊý
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_S32 HI_UBOOT_LOGO_DestoryLayer(HI_VIDEO_LOGO_INFO *pstLogoInfor)
{
#if 0

#endif
    return HI_SUCCESS;
}

#else

static HI_VOID Local_DRV_StopHardwareForSmoothTransition(HI_VOID)
{
	CBB_LAYER_CFG_INFO_S stLayerCfgBasePara;
	CBB_WBC_CFG_INFO_S   stWbcCfgBasePara;

	memset((HI_VOID*)&stLayerCfgBasePara, 0, sizeof(CBB_LAYER_CFG_INFO_S));
	memset((HI_VOID*)&stWbcCfgBasePara, 0, sizeof(CBB_WBC_CFG_INFO_S));	


	CBB_Video_Init(HI_NULL, HI_NULL);
	
	stLayerCfgBasePara.bRegionEn   = HI_FALSE;
	stLayerCfgBasePara.u32LayerNO  = 0;
	stLayerCfgBasePara.u32RegionNO = 0;
	
	CBB_Video_SetLayerParam(&stLayerCfgBasePara, HI_NULL);

	stWbcCfgBasePara.enWBC = DRV_VDP_WBC_WIN;
	stWbcCfgBasePara.bWbcEnable = HI_FALSE;	
	CBB_Video_SetWbcParam(&stWbcCfgBasePara,HI_NULL);
	
	stLayerCfgBasePara.bRegionEn   = HI_FALSE;
	stLayerCfgBasePara.u32LayerNO  = 3;
	stLayerCfgBasePara.u32RegionNO = 0;
	
	CBB_Video_SetLayerParam(&stLayerCfgBasePara, HI_NULL);

	/* msleep 80ms to ensure wbc closed or screen addr switched
         *  max time or each fps is 1000 / 24 = 42ms*/
	msleep(80);
	
	CBB_Video_DeInit();	
	return;
}


HI_VOID HI_DRV_LOGO_FreeLogoMem(HI_VOID)
{
		
    PDM_EXPORT_FUNC_S *ps_PdmExportFuncs = HI_NULL;
	
    if (HI_SUCCESS != HI_DRV_MODULE_GetFunction(HI_ID_PDM, (HI_VOID**)&ps_PdmExportFuncs))
	{
		HI_ERR_DISP("call  HI_DRV_MODULE_GetFunction failed!\n");
		return;
    }

    if(HI_NULL == ps_PdmExportFuncs)
	{
		HI_ERR_DISP("Get ps_PdmExportFuncs null!\n");
        return;
    }
	
	if (HI_SUCCESS != down_interruptible(&g_LogoFreeLock))
	{
		return;
	}
	
	if (HI_TRUE == g_u32LogoFreeFlag)
	{
		up(&g_LogoFreeLock);
		return;
	}
	
    Local_DRV_StopHardwareForSmoothTransition();	
    ps_PdmExportFuncs->pfnPDM_ReleaseReserveMem(VDP_DDR_NAME_UBOOT);
    ps_PdmExportFuncs->pfnPDM_ReleaseReserveMem(LOGO_WBC_MEM);

	g_u32LogoFreeFlag = HI_TRUE;
	up(&g_LogoFreeLock);
	
	return;
}

HI_VOID HI_DRV_LOGO_GetLogoInfor(HI_DRV_DISP_VideoLogoInfo *pstLogoInfor)
{
	HI_RECT_S stRect = {0};	
	
	VDP_VID_GetLayerEnableFlag(0, &pstLogoInfor->bVideoLogoMode);

	if (HI_TRUE != pstLogoInfor->bVideoLogoMode)
	{
		return;
	}
	
	VDP_VID_GetInReso(0,&stRect);
	pstLogoInfor->u32LogoWidth  = stRect.s32Width  + 1;
	pstLogoInfor->u32LogoHeight = stRect.s32Height + 1;
	
	VDP_VID_GetInDataFmt(0, &pstLogoInfor->ePixelFmt);
	VDP_VID_GetLayerAddrAndStride(0, 
		                          &pstLogoInfor->u32LogoYAddr,
		                          &pstLogoInfor->u32LogoCbAddr,
		                          &pstLogoInfor->u32LogoYStride,
		                          &pstLogoInfor->u32LogoCbStride);
	
	pstLogoInfor->u32LogoCrAddr   = 0;
	pstLogoInfor->u32LogoCrStride = 0;
	
	return;
}

#endif

