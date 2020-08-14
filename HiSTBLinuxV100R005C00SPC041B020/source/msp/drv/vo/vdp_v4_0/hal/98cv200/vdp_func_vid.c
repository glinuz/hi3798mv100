#include "hi_type.h"
#include "vdp_drv_vid.h"
#include "vdp_func_vid.h"

HI_S32 VDP_FUNC_InitVidInfo(VDP_LAYER_VID_E u32LayerId, VDP_LAYER_VID_INFO_S *pstInfo, HI_U32 w, HI_U32 h)
{
	//rect to do...
	pstInfo->u32RegionNO          = 1;
	pstInfo->bRegionEn            = HI_FALSE;
	pstInfo->bEnable              = HI_TRUE;

	pstInfo->bFlipEn              = HI_FALSE;

	pstInfo->enReadMode           = VDP_RMODE_PROGRESSIVE;
	pstInfo->stCropRect.u32X      = 0;
	pstInfo->stCropRect.u32Y      = 0;
	pstInfo->stCropRect.u32Wth    = w;
	pstInfo->stCropRect.u32Hgt    = h;

	pstInfo->stVideoRect          = pstInfo->stCropRect;
	pstInfo->stDispRect           = pstInfo->stCropRect;
	pstInfo->enOutClrSpace        = VDP_CLR_SPACE_RGB_709;

	pstInfo->bMuteEn              = HI_FALSE;
	pstInfo->stMuteColor.u32BkgY  = 0x40; 
	pstInfo->stMuteColor.u32BkgU  = 0x200; 
	pstInfo->stMuteColor.u32BkgV  = 0x200; 
	pstInfo->stMuteColor.u32BkgA  = 0xff; 

	pstInfo->stLetterBoxColor     = pstInfo->stMuteColor;
	pstInfo->stMixvColor          = pstInfo->stMuteColor; 

	pstInfo->enDispMode           = VDP_DISP_MODE_2D;
	pstInfo->u32DepthLevel        = 0;

	pstInfo->stSrcInfo.bSecureEn             = HI_FALSE; 
	pstInfo->stSrcInfo.bSmmuEn               = HI_FALSE; 

	pstInfo->stSrcInfo.bUvOrder              = HI_FALSE; 
	pstInfo->stSrcInfo.bDcmpEn               = HI_FALSE; 
	pstInfo->stSrcInfo.bDcmpLost             = HI_FALSE; 

	pstInfo->stSrcInfo.enInFmt               = VDP_VID_IFMT_SP_420; 
	pstInfo->stSrcInfo.enDataWidth           = VDP_DATA_WTH_10; 
	pstInfo->stSrcInfo.enSrcClrSpace         = VDP_CLR_SPACE_YUV_709; 

	pstInfo->stSrcInfo.stSrcRect             = pstInfo->stCropRect; 

	pstInfo->stSrcInfo.bSrcIsInterlace       = HI_FALSE; 
	pstInfo->stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_2D; 

	pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32LumAddr = 0xf1000000; 
	pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32ChmAddr = 0xf2000000; 
	pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32LumStr  = 0x1200; 
	pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32ChmStr  = 0x1200; 

	//pstInfo->stSrcInfo.stAddr[VDP_ADDR_3D].u32LumAddr = 0xf3000000; 
	//pstInfo->stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmAddr = 0xf4000000; 
	//pstInfo->stSrcInfo.stAddr[VDP_ADDR_3D].u32LumStr  = 0x1200; 
	//pstInfo->stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmStr  = 0x1200; 

	return HI_SUCCESS;
}

HI_S32 VDP_FUNC_GetVidInfo(VDP_LAYER_VID_E u32LayerId, VDP_LAYER_VID_INFO_S *pstInfo, VDP_VID_CFG_MODE_E enCfgMode)
{
	if(enCfgMode == VDP_VID_CFG_TYP)
	{
		pstInfo->u32RegionNO          = 1;
		pstInfo->bRegionEn            = HI_FALSE;
		pstInfo->bEnable              = HI_TRUE;

		pstInfo->bFlipEn              = HI_FALSE;

		pstInfo->enReadMode           = VDP_RMODE_PROGRESSIVE;
		pstInfo->stCropRect.u32X      = 0;
		pstInfo->stCropRect.u32Y      = 0;
		pstInfo->stCropRect.u32Wth    = 1920;//w;
		pstInfo->stCropRect.u32Hgt    = 1080;//h;

		pstInfo->stVideoRect          = pstInfo->stCropRect;
		pstInfo->stDispRect           = pstInfo->stCropRect;
		pstInfo->enOutClrSpace        = VDP_CLR_SPACE_RGB_709;

		pstInfo->bMuteEn              = HI_FALSE;
		pstInfo->stMuteColor.u32BkgY  = 0x40; 
		pstInfo->stMuteColor.u32BkgU  = 0x200; 
		pstInfo->stMuteColor.u32BkgV  = 0x200; 
		pstInfo->stMuteColor.u32BkgA  = 0xff; 

		pstInfo->stLetterBoxColor     = pstInfo->stMuteColor;
		pstInfo->stMixvColor          = pstInfo->stMuteColor; 

		pstInfo->enDispMode           = VDP_DISP_MODE_2D;
		pstInfo->u32DepthLevel        = 0;

		pstInfo->stSrcInfo.bSecureEn             = HI_FALSE; 
		pstInfo->stSrcInfo.bSmmuEn               = HI_FALSE; 

		pstInfo->stSrcInfo.bUvOrder              = HI_FALSE; 
		pstInfo->stSrcInfo.bDcmpEn               = HI_FALSE; 
		pstInfo->stSrcInfo.bDcmpLost             = HI_FALSE; 

		pstInfo->stSrcInfo.enInFmt               = VDP_VID_IFMT_SP_420; 
		pstInfo->stSrcInfo.enDataWidth           = VDP_DATA_WTH_10; 
		pstInfo->stSrcInfo.enSrcClrSpace         = VDP_CLR_SPACE_YUV_709; 

		pstInfo->stSrcInfo.stSrcRect             = pstInfo->stCropRect; 

		pstInfo->stSrcInfo.bSrcIsInterlace       = HI_FALSE; 
		pstInfo->stSrcInfo.enSrc3dMode           = VDP_DISP_MODE_2D; 

		pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32LumAddr = 0xf1000000; 
		pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32ChmAddr = 0xf2000000; 
		pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32LumStr  = 0x1200; 
		pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32ChmStr  = 0x1200; 

		pstInfo->stSrcInfo.stAddr[VDP_ADDR_3D].u32LumAddr = 0xf3000000; 
		pstInfo->stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmAddr = 0xf4000000; 
		pstInfo->stSrcInfo.stAddr[VDP_ADDR_3D].u32LumStr  = 0x1200; 
		pstInfo->stSrcInfo.stAddr[VDP_ADDR_3D].u32ChmStr  = 0x1200; 
	}

	return HI_SUCCESS;
}








