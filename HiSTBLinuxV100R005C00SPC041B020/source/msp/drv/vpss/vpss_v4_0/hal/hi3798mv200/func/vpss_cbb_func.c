#include "vpss_cbb_reg.h"
#include "vpss_cbb_func.h"



#define HI_PQ_VPSS_PORT0_LAYER_ZME 0


HI_VOID VPSS_FuncCfg_VC1(CBB_FUNC_VC1_S *pstFuncVC1Cfg, CBB_FRAME_CHANNEL_E enChan, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	switch ( enChan )
	{
	    case CBB_FRAME_READ_REF:
			VPSS_DRV_Set_VPSS_REF_VC1_CTRL0(pstVpssNode, pstFuncVC1Cfg->rangedfrm, pstFuncVC1Cfg->profile,
															pstFuncVC1Cfg->mapyflg, pstFuncVC1Cfg->mapcflg,
															pstFuncVC1Cfg->mapy, pstFuncVC1Cfg->mapc);
	        VPSS_DRV_Set_ref_vc1_en(pstVpssNode, pstFuncVC1Cfg->bEnable);
	        break;
	    case CBB_FRAME_READ_CUR:
			VPSS_DRV_Set_VPSS_CUR_VC1_CTRL0(pstVpssNode, pstFuncVC1Cfg->rangedfrm, pstFuncVC1Cfg->profile,
															pstFuncVC1Cfg->mapyflg, pstFuncVC1Cfg->mapcflg,
															pstFuncVC1Cfg->mapy, pstFuncVC1Cfg->mapc);
	        VPSS_DRV_Set_cur_vc1_en(pstVpssNode, pstFuncVC1Cfg->bEnable);
	        break;
	    case CBB_FRAME_READ_NX1:
  			VPSS_DRV_Set_VPSS_NX1_VC1_CTRL0(pstVpssNode, pstFuncVC1Cfg->rangedfrm, pstFuncVC1Cfg->profile,
															pstFuncVC1Cfg->mapyflg, pstFuncVC1Cfg->mapcflg,
															pstFuncVC1Cfg->mapy, pstFuncVC1Cfg->mapc);
	        VPSS_DRV_Set_nx1_vc1_en(pstVpssNode, pstFuncVC1Cfg->bEnable);
	        break;
	    case CBB_FRAME_READ_NX2:
			VPSS_DRV_Set_VPSS_NX2_VC1_CTRL0(pstVpssNode, pstFuncVC1Cfg->rangedfrm, pstFuncVC1Cfg->profile,
															pstFuncVC1Cfg->mapyflg, pstFuncVC1Cfg->mapcflg,
															pstFuncVC1Cfg->mapy, pstFuncVC1Cfg->mapc);
	        VPSS_DRV_Set_nx2_vc1_en(pstVpssNode, pstFuncVC1Cfg->bEnable);
	        break;
	    default:

	        break;
	}

}

HI_VOID VPSS_FuncCfg_VPZme(CBB_FUNC_VPZME_S *pstFuncVPZmeCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址

	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_VPZME_CFG0(pstVpssNode, pstFuncVPZmeCfg->eFirMode);
	VPSS_DRV_Set_vhd0_pre_vfir_en(pstVpssNode, pstFuncVPZmeCfg->bEnable);
}

HI_VOID VPSS_FuncCfg_Zme(CBB_FUNC_ZME_S *pstFuncZmeCfg, CBB_REG_ADDR_S stRegAddr)
{
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_zme_vhd0_en(pstVpssNode, pstFuncZmeCfg->bEnable);
	VPSS_DRV_Set_VPSS_VHD0_HSP(pstVpssNode, HI_TRUE, HI_TRUE, HI_TRUE,
												HI_TRUE, HI_TRUE, HI_TRUE,
													0x0, 0x0);
	VPSS_DRV_Set_VPSS_ZME_ADDR(pstVpssNode, pstFuncZmeCfg->u32ParAddr);

	DRV_PQ_SetVpssZme(HI_PQ_VPSS_PORT0_LAYER_ZME,
						  pstVpssNode,
						  	&(pstFuncZmeCfg->stZmeDrvPara),
						  		HI_TRUE);
}

HI_VOID VPSS_FuncCfg_Crop(CBB_FUNC_CROP_S *pstFuncCropCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_VHD0CROP_SIZE(pstVpssNode, pstFuncCropCfg->stCropCfg.s32Height,  pstFuncCropCfg->stCropCfg.s32Width);
	VPSS_DRV_Set_VPSS_VHD0CROP_POS(pstVpssNode, pstFuncCropCfg->stCropCfg.s32Y, pstFuncCropCfg->stCropCfg.s32X);
	VPSS_DRV_Set_vhd0_crop_en(pstVpssNode, pstFuncCropCfg->bEnable);

}

HI_VOID VPSS_FuncCfg_LBox(CBB_FUNC_LBOX_S *pstFuncLBoxCfg, CBB_REG_ADDR_S stRegAddr)
{
	HI_U32 vhd0_vbk_cr;
	HI_U32 vhd0_vbk_cb;
	HI_U32 vhd0_vbk_y;

	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	if (pstFuncLBoxCfg->eBGColor >= CBB_COLOR_BUTT)
	{
		return;
	}

	switch (pstFuncLBoxCfg->eBGColor)
	{
	    case CBB_COLOR_BLACK:
            vhd0_vbk_y = 0;
            vhd0_vbk_cb = 128;
            vhd0_vbk_cr = 128;
	        break;
	    case CBB_COLOR_WHITE:
            vhd0_vbk_y = 255;
            vhd0_vbk_cb = 128;
            vhd0_vbk_cr = 128;
            break;
	    default:
            vhd0_vbk_y = 128;
            vhd0_vbk_cb = 128;
            vhd0_vbk_cr = 240;
	    	break;
	}

	VPSS_DRV_Set_VPSS_VHD0LBA_DSIZE(pstVpssNode, pstFuncLBoxCfg->stLBoxRect.s32Height, pstFuncLBoxCfg->stLBoxRect.s32Width);
	VPSS_DRV_Set_VPSS_VHD0LBA_VFPOS(pstVpssNode, pstFuncLBoxCfg->stLBoxRect.s32Y, pstFuncLBoxCfg->stLBoxRect.s32X);
	VPSS_DRV_Set_VPSS_VHD0LBA_BK(pstVpssNode, vhd0_vbk_cr, vhd0_vbk_cb, vhd0_vbk_y);
	VPSS_DRV_Set_vhd0_lba_en(pstVpssNode, pstFuncLBoxCfg->bEnable);

}


HI_VOID VPSS_FuncCfg_Flip(CBB_FUNC_FLIP_S *pstFuncFlipCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

    VPSS_DRV_Set_vhd0_flip(pstVpssNode, pstFuncFlipCfg->bFlipV);
    VPSS_DRV_Set_vhd0_mirror(pstVpssNode, pstFuncFlipCfg->bFlipH);
}

HI_VOID VPSS_FuncCfg_UVRevert(HI_BOOL bUVInvertEnable, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_vhd0_uv_invert(pstVpssNode, bUVInvertEnable);
}

HI_VOID VPSS_FuncCfg_TunneIn(CBB_FUNC_TUNNELIN_S *pstTunnelInCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_NX2_TUNL_ADDR(pstVpssNode, pstTunnelInCfg->u32ReadAddr);
	VPSS_DRV_Set_VPSS_NX2_TUNL_CTRL(pstVpssNode, pstTunnelInCfg->u32ReadInterval, pstTunnelInCfg->bEnable);
}

HI_VOID VPSS_FuncCfg_TunneOut(CBB_FUNC_TUNNELOUT_S *pstTunnelOutCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_VHD0_TUNL_CTRL(pstVpssNode, pstTunnelOutCfg->bEnable, pstTunnelOutCfg->eMode, pstTunnelOutCfg->u32FinishLine);
	VPSS_DRV_Set_VPSS_VHD0_TUNL_ADDR(pstVpssNode, pstTunnelOutCfg->u32WriteAddr);
}

HI_VOID VPSS_FuncCfg_Trans(CBB_FUNC_TRANS_S *pstTransCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	// TODO:  mmu.  secure


	VPSS_DRV_Set_VPSS_TRANS_SRC_ADDR(pstVpssNode, pstTransCfg->u32SrcAddr);
	VPSS_DRV_Set_VPSS_TRANS_DES_ADDR(pstVpssNode, pstTransCfg->u32DstAddr);
	VPSS_DRV_Set_VPSS_TRANS_INFO	(pstVpssNode, pstTransCfg->bEnable, pstTransCfg->u32Size);
}

HI_VOID VPSS_FuncCfg_SMMU(CBB_FUNC_SMMU_S *pstSMMUCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssGloablAddr; //全局地址
	S_VPSS_REGS_TYPE *pstVpssNode; //DDR中节点首地址
	pstVpssGloablAddr = (S_VPSS_REGS_TYPE *)stRegAddr.pu8BaseVirAddr;
	pstVpssNode       = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;


#if 0 //common module cfg these security regs...
	VPSS_DRV_Set_SMMU_SCB_TTBR(pstVpssGloablAddr, pstSMMUCfg->u32SCB_TTBR);
	VPSS_DRV_Set_SMMU_ERR_RDADDR_S(pstVpssGloablAddr, pstSMMUCfg->u32Err_s_rd_addr);
	VPSS_DRV_Set_SMMU_ERR_WRADDR_S(pstVpssGloablAddr, pstSMMUCfg->u32Err_s_wr_addr);
#endif

	VPSS_DRV_Set_SMMU_CB_TTBR (pstVpssGloablAddr, pstSMMUCfg->u32CB_TTBR);
	VPSS_DRV_Set_SMMU_ERR_RDADDR_NS(pstVpssGloablAddr, pstSMMUCfg->u32Err_ns_rd_addr);
	VPSS_DRV_Set_SMMU_ERR_WRADDR_NS(pstVpssGloablAddr, pstSMMUCfg->u32Err_ns_wr_addr);
	VPSS_DRV_Set_SMMU_SCR(pstVpssGloablAddr, pstSMMUCfg->u32Ptw_pf, pstSMMUCfg->bIntEnable, pstSMMUCfg->bGlobalBypass);

	VPSS_DRV_Set_VPSS_RCH_BYPASS_FLAG(pstVpssNode, pstSMMUCfg->u32BypassFlag_RCH);
	VPSS_DRV_Set_VPSS_WCH_BYPASS_FLAG(pstVpssNode, pstSMMUCfg->u32BypassFlag_WCH);

}

HI_VOID VPSS_FuncCfg_ROTATIONYC(CBB_FUNC_ROTATION_YC_S *pstRotaCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	//VPSS_FATAL("ROTATIONYC %d %d %d\n", pstRotaCfg->u32Angle, pstRotaCfg->u32ImgProcessMode, pstRotaCfg->bEnable);

	VPSS_DRV_Set_VPSS_RTT_CFG(pstVpssNode, pstRotaCfg->u32Angle);
	VPSS_DRV_Set_img_pro_mode(pstVpssNode, pstRotaCfg->u32ImgProcessMode);
	VPSS_DRV_Set_rotate_en(pstVpssNode, pstRotaCfg->bEnable);
}

HI_VOID VPSS_FuncCfg_4PIXEL(CBB_FUNC_4PIXEL_S *pst4PixelCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_four_pix_en(pstVpssNode, pst4PixelCfg->bEnable);
}

HI_VOID VPSS_FuncCfg_CTS10BIT(CBB_FUNC_CTS10BIT_S *pstCts10BitCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_IN_CTRL_CTS(pstVpssNode, pstCts10BitCfg->bBitSel_LOW10, pstCts10BitCfg->bEnable);

}

