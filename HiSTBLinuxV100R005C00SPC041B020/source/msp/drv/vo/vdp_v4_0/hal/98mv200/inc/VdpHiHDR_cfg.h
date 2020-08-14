#ifndef __HIHDR_COEF_H__
#define __HIHDR_COEF_H__


/******************************************************************************

  Copyright (C), 2001-2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : HiHDR_cfg_98M.h
  Version       : HiHDR_V100_R1(For 98MV200)
  Author        : w00357637
  Created       : 2016/05/15
  Last Modified : 2016/06/07
  Description   : HiHDR≈‰÷√Ω·ππÃÂ
  Function List :
******************************************************************************/
#include "hi_type.h"
#include "vdp_ip_define.h"
//#include "vdp_drv_hihdr.h"
#include "vdp_drv_vid.h"

#include "vdp_drv_ip_gdm.h"



// Lut Size
#define DEGMM_LUT_SIZE		64
#define TMAP_LUT_SIZE		64
#define GMM_LUT_SIZE		64

typedef struct VDP_HIHDR_EXTERN
{
	HI_BOOL 			   bRegionEn;
	HI_U32				   u32LayerZorder;

	VDP_DATA_RMODE_E	   enReadMode;
	HI_BOOL 			   bFlipEn;

	VDP_DRAW_MODE_E 	   enDrawMode;
	VDP_DRAW_PIXEL_MODE_E  enDrawPixelMode;
	HI_BOOL 			   bHzmeEn;
	VDP_HZME_MODE_E  enHzmeMode;//add by hyx t28
	VDP_RECT_S 			   stZme2InRect;	
	VDP_DISP_MODE_E 	   enDispMode;
	
}VDP_HIHDR_EXTERN_S;

typedef enum 
{
	HIHDR_HDR10_IN_SDR_OUT = 0,
	HIHDR_HDR10_IN_HDR10_OUT , 
	HIHDR_HDR10_IN_HLG_OUT,
	
	HIHDR_HLG_IN_SDR_OUT ,
	HIHDR_HLG_IN_HDR10_OUT ,
	HIHDR_HLG_IN_HLG_OUT ,
	
	HIHDR_AVS_IN_SDR_OUT , //new add
	HIHDR_AVS_IN_HDR10_OUT , //new add
	
	HIHDR_TECHNI_IN_SDR_OUT,
	HIHDR_TECHNI_IN_HDR10_OUT,
	
	HIHDR_BT2020_IN_RGB_709_OUT,
	HIHDR_BT2020_IN_709_OUT,//not use
	HIHDR_SDR_IN_HDR10_OUT ,//not use
	HIHDR_SDR_IN_HLG_OUT ,//not use
	HIHDR_709_IN_BT2020_OUT,//not use
	HIHDR_BYPASS_MODE ,//not use
	HIHDR_BUTT
}HIHDR_SCENE_MODE_E;


typedef struct VDP_HISI_HDR_CFG_S
{
	HIHDR_SCENE_MODE_E eHiHdrMode;
	GDM_SCENE_MODE_E   enHiHdrGfxMode;
    HI_BOOL            bMuteEn;
	VDP_SRC_INFO_S		   stSrcInfo;
	VDP_RECT_S			   stCropRect;
	VDP_RECT_S			   stVideoRect;
	VDP_RECT_S			   stDispRect;
	//VDP_RECT_S			   stZme1OutRect;
	VDP_DISP_INF_S			stVdpDispInfo;

	VDP_HIHDR_EXTERN_S	   stHdrVdpCfgExt;
	
	VDP_VZME_DRV_PARA_S pstZmeDrvPara;//add by hyx

}VDP_HISI_HDR_CFG_S;



#endif
