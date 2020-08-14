#ifndef __DRV_HIHDR_H__
#define __DRV_HIHDR_H__

#include "vdp_drv_vid.h"


typedef enum tagVDP_HIHDR_CFG_MODE_E
{
    VDP_HIHDR_HDR10_IN_HDR10_OUT  = 0x0 , 
	VDP_HIHDR_HDR10_IN_SDR_OUT   ,
	
    VDP_HIHDR_HLG_IN_HDR10_OUT , 
	VDP_HIHDR_HLG_IN_SDR_OUT   , 
	
    VDP_HIHDR_CFG_MODE_BUTT        
    
}VDP_HIHDR_CFG_MODE_E;


typedef struct
{
	VDP_HIHDR_CFG_MODE_E enHdrMode;
	VDP_DATA_WTH u32DataWidth;//0:8bit; 1:10bit
	VDP_RECT_S stVidInReso ; //vid input reso
	VDP_RECT_S stVidOutReso  ; //vid zme output reso
	//VDP_RECT_S stGfxReso ; //gfx input reso

	VDP_ADDR_S stVidAddr; 
	VDP_ADDR_S stVidAddr_2LowBit  ; 
	VDP_ADDR_S stHeadAddr; //dcmp head data addr

	HI_BOOL bDcmpEn;
	HI_BOOL bMuteEn;
	HI_BOOL bGfxEn;
	HI_BOOL bPreMult;
	
    VDP_VID_IFMT_E  enVidIFmt;

	//VDP_GFX_IFMT_E enGfxIfmt;
	
    HI_BOOL           bSmmuEn; 
    HI_BOOL           bFlag480I576I; 
	HI_BOOL           bSecureEn;
	VDP_CLR_SPACE_E   enInClr;
	VDP_CLR_SPACE_E   enOutClr;

} VDP_HISI_HDR_CFG_S;


#ifndef __DISP_PLATFORM_BOOT__
HI_S32  VDP_DRV_SetHisiHdrMode( VDP_HISI_HDR_CFG_S *stHdrCfg, HI_PQ_HDR_CFG *pstHisiHdr);
#endif

HI_VOID VDP_DRV_HiSiHdrClean(HI_BOOL bclear);
HI_VOID VDP_DRV_HiSiHdrMute(HI_BOOL bMuteEn);


#endif
