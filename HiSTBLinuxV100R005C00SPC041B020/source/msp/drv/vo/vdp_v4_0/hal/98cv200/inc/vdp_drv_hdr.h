#ifndef __DRV_HDR_H__
#define __DRV_HDR_H__

#include "vdp_drv_vid.h"
#include "KdmTypeFxp.h"

typedef enum tagVDP_HDR_CFG_MODE_E
{
    VDP_HDR_DOVI_BC_IN_DOVI_YUV_OUT  = 0x0 , 
    VDP_HDR_DOVI_NBC_IN_DOVI_YUV_OUT       , 

    //for certificate: 422 10bit output
    VDP_HDR_DOVI_BC_IN_HDR10_OUT_CERT      , 
    VDP_HDR_DOVI_NBC_IN_HDR10_OUT_CERT     , 
    VDP_HDR_HDR10_IN_HDR10_OUT_CERT        , 

	//for STB hdmi output, 444
    VDP_HDR_SDR_IN_SDR_OUT                 , 
    VDP_HDR_DOVI_BC_IN_SDR_OUT             , 
    VDP_HDR_DOVI_NBC_IN_SDR_OUT            , 
    VDP_HDR_HDR10_IN_SDR_OUT               , 

    VDP_HDR_DOVI_BC_IN_DOVI_IPT_OUT        , 
    VDP_HDR_DOVI_NBC_IN_DOVI_IPT_OUT       , 

	//for certificate: 422 uyvy output
    VDP_HDR_SDR_IN_SDR_OUT_CERT            , 
    VDP_HDR_DOVI_BC_IN_SDR_OUT_CERT        , 
    VDP_HDR_DOVI_NBC_IN_SDR_OUT_CERT       , 
    VDP_HDR_HDR10_IN_SDR_OUT_CERT          , 
	VDP_HDR_DOVI_NBC_IN_SDR_IPT_OUT		   ,
	VDP_HDR_DOVI_BC_IN_SDR_IPT_OUT		   ,

	//444 10bit output,ds in HDMI
    VDP_HDR_DOVI_BC_IN_HDR10_OUT       , 
    VDP_HDR_DOVI_NBC_IN_HDR10_OUT      , 
    VDP_HDR_HDR10_IN_HDR10_OUT         , 

    VDP_HDR_CFG_MODE_BUTT        
    
}VDP_HDR_CFG_MODE_E;


typedef struct
{
	VDP_HDR_CFG_MODE_E enHdrMode;
	VDP_DATA_WTH u32DataWidth;//0:8bit; 1:10bit
	VDP_RECT_S stVidReso ; //vid input reso
	VDP_RECT_S stBlReso  ; //vid zme output reso
	VDP_RECT_S stElReso  ; //el input reso
	VDP_RECT_S stGfxReso ; //gfx input reso

	VDP_ADDR_S stBlAddr  ; 
	VDP_ADDR_S stBlAddr_2LowBit  ; 
	VDP_ADDR_S stElAddr  ; 
	VDP_ADDR_S stHeadAddr; //dcmp head data addr

	HI_BOOL bDcmpEn;
	HI_BOOL bMuteEn;
	HI_BOOL bGfxEn;
	HI_BOOL bPreMult;
	
    VDP_VID_IFMT_E  enVdpBlVidIFmt;
    VDP_VID_IFMT_E  enVdpElVidIFmt;

	VDP_GFX_IFMT_E enGfxIfmt;
	
    HI_BOOL           bSmmuEn               ; 
    HI_BOOL           bFlag480I576I        ; 
	HI_BOOL           bSecureEn;
	//HI_U32 u32CoefAddr[5];

} VDP_HDR_CFG_S;


HI_S32 VDP_DRV_SetHdrMode         ( VDP_HDR_CFG_S *stHdrCfg, rpu_ext_config_fixpt_main_t *pstCompCfg, DmKsFxp_t *pstDmKs);
HI_S32 VDP_DRV_SetHdrEdaCfg       ( VDP_HDR_CFG_S *stHdrCfg);
HI_S32 VDP_DRV_SetComposer        ( rpu_ext_config_fixpt_main_t* pstCompCfg);
HI_S32 VDP_DRV_SetVdm             ( DmKsFxp_t* pstDmKs);
HI_S32 VDP_DRV_SetHdrHdmiMetadata ( HI_U8 * pu8HeadData, HI_U32 u32MdLen, HI_U8 * pu8MdBuf);
HI_VOID VDP_DRV_SetGdm(HI_VOID);
HI_VOID VDP_DRV_CleanHdr(HI_BOOL bclear);

HI_VOID VDP_DRV_MaskAndBackupGfx(HI_U32 u32GfxLayerId,HI_U32 *pu32Alpha,HI_U32 *pu32MuteState);
HI_VOID VDP_DRV_UnmaskGfx(HI_U32 u32GfxLayerId,HI_U32 u32Alpha,HI_U32 u32MuteState);
HI_VOID VDP_DRV_SetGfxMixvBypassState(HI_U32 u32GfxLayerId,HI_BOOL bEnable);

#endif
