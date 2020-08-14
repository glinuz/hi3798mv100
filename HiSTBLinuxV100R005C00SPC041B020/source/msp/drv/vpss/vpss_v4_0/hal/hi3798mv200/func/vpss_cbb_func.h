#ifndef __VPSS_CBB_FUNC_H__
#define __VPSS_CBB_FUNC_H__

#include "vpss_cbb_common.h"
#include "vpss_cbb_component.h"

//CBB_FUNC_VC1
typedef struct
{
	HI_BOOL bEnable;
	HI_U32 rangedfrm;
	HI_U32 profile;
    HI_U32 mapyflg;
    HI_U32 mapcflg;
    HI_U32 mapy;
	HI_U32 mapc;
}CBB_FUNC_VC1_S;

HI_VOID VPSS_FuncCfg_VC1(CBB_FUNC_VC1_S *pstFuncVC1Cfg, CBB_FRAME_CHANNEL_E enChan, CBB_REG_ADDR_S stRegAddr);


//CBB_FUNC_VHD0_VPZME
typedef struct
{
	HI_BOOL bEnable;
	CBB_FIR_MODE_E	eFirMode;
	HI_RECT_S stInRect;
    HI_RECT_S stOutRect;
}CBB_FUNC_VPZME_S;

HI_VOID VPSS_FuncCfg_VPZme(CBB_FUNC_VPZME_S *pstFuncVPZmeCfg, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_VHD0_ZME
typedef struct
{
	HI_BOOL				bEnable;
	HI_U32				u32ParAddr;
	CBB_ZME_MODE_E		eMode;
	HI_PQ_ZME_PARA_IN_S stZmeDrvPara;
    HI_RECT_S			stInRect;
    HI_RECT_S			stOutRect;
}CBB_FUNC_ZME_S;

HI_VOID VPSS_FuncCfg_Zme(CBB_FUNC_ZME_S *pstFuncZmeCfg, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_VHD0_CROP
typedef struct
{
    HI_BOOL bEnable;
    HI_RECT_S stInRect;
    HI_RECT_S stOutRect;
    HI_RECT_S stCropCfg;
}CBB_FUNC_CROP_S;

HI_VOID VPSS_FuncCfg_Crop(CBB_FUNC_CROP_S *pstFuncCropCfg, CBB_REG_ADDR_S stRegAddr);


//CBB_FUNC_VHD0_LBOX
typedef struct
{
    HI_BOOL		bEnable;
    HI_RECT_S 	stInRect;
    HI_RECT_S 	stOutRect;
    HI_RECT_S	stLBoxRect;
	CBB_COLOR_E eBGColor;
}CBB_FUNC_LBOX_S;

HI_VOID VPSS_FuncCfg_LBox(CBB_FUNC_LBOX_S *pstFuncLBoxCfg, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_VHD0_MIRROR
typedef struct
{
    HI_BOOL		bFlipV;
	HI_BOOL     bFlipH; //Mirror
}CBB_FUNC_FLIP_S;

HI_VOID VPSS_FuncCfg_Flip(CBB_FUNC_FLIP_S *pstFuncFlipCfg, CBB_REG_ADDR_S stRegAddr);

HI_VOID VPSS_FuncCfg_UVRevert(HI_BOOL bUVInvertEnable, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_TUNNEL_IN
typedef struct
{
    HI_BOOL		bEnable;
	HI_U32		u32ReadInterval;
	HI_U32		u32ReadAddr;
}CBB_FUNC_TUNNELIN_S;

HI_VOID VPSS_FuncCfg_TunneIn(CBB_FUNC_TUNNELIN_S *pstTunnelInCfg, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_TUNNEL_OUT
// 
typedef struct
{
    HI_BOOL		bEnable;
	CBB_TUNOUT_MODE_E eMode;
	HI_U32		u32FinishLine;
	HI_U32		u32WriteAddr;
}CBB_FUNC_TUNNELOUT_S;

HI_VOID VPSS_FuncCfg_TunneOut(CBB_FUNC_TUNNELOUT_S *pstTunnelOutCfg, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_TRANS
typedef struct
{
    HI_BOOL		bEnable;
	HI_U32		u32SrcAddr;
	HI_U32		u32DstAddr;
	HI_U32		u32Size;
	HI_BOOL		bSMMU;
	HI_BOOL		bSecure;
}CBB_FUNC_TRANS_S;

HI_VOID VPSS_FuncCfg_Trans(CBB_FUNC_TRANS_S *pstTransCfg, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_SMMU
typedef struct
{
    HI_BOOL		bGlobalBypass; //global bypass
    HI_BOOL		bIntEnable;
	HI_U32		u32Ptw_pf;
    
    HI_U32		u32BypassFlag_RCH;
    HI_U32		u32BypassFlag_WCH;
	HI_U32		u32SCB_TTBR; //SMMU_SCB_TTBR
	HI_U32		u32CB_TTBR; //SMMU_CB_TTBR
	HI_U32		u32Err_s_rd_addr;
	HI_U32		u32Err_s_wr_addr;
	HI_U32		u32Err_ns_rd_addr;
	HI_U32		u32Err_ns_wr_addr;
}CBB_FUNC_SMMU_S;

HI_VOID VPSS_FuncCfg_SMMU(CBB_FUNC_SMMU_S *pstSMMUCfg, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_ROTATION_Y, CBB_FUNC_ROTATION_C
typedef struct
{
    HI_BOOL		bEnable; //rotate_en
	HI_U32		u32Angle; //rotate_angle, 0:90°1:270°
	HI_U32		u32ImgProcessMode; //img_pro_mode, 0:all, 1:Y, 2:C
}CBB_FUNC_ROTATION_YC_S;

HI_VOID VPSS_FuncCfg_ROTATIONYC(CBB_FUNC_ROTATION_YC_S *pstRotaCfg, CBB_REG_ADDR_S stRegAddr);

//CBB_FUNC_SPEED_UP_4PIXEL
typedef struct
{
    HI_BOOL		bEnable; //four_pix_en
}CBB_FUNC_4PIXEL_S;

HI_VOID VPSS_FuncCfg_4PIXEL(CBB_FUNC_4PIXEL_S *pst4PixelCfg, CBB_REG_ADDR_S stRegAddr);


//CBB_FUNC_CTS10BIT
typedef struct
{
    HI_BOOL		bEnable; //cts_en
    HI_BOOL		bBitSel_LOW10; //cts_bit_sel, CTS像素填充选择信号：0:填充高比特[15:6]; 1:填充低比特[9:0]
}CBB_FUNC_CTS10BIT_S;

HI_VOID VPSS_FuncCfg_CTS10BIT(CBB_FUNC_CTS10BIT_S *pstCts10BitCfg, CBB_REG_ADDR_S stRegAddr);



//CBB_FUNC_RWZB, 软件算法


#endif /* __VPSS_CBB_FUNC_H__ */

