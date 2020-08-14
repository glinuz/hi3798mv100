#include "vpss_cbb_frame.h"
#include "vpss_cbb_func.h"
#include "vpss_cbb_component.h"
#include "vpss_cbb_debug.h"
#include "vpss_cbb_reg.h"
#include "vpss_cbb_intf.h"
#include "vpss_reg_struct.h"

HI_S32 CBB_CfgFrm_ReadCur(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr)
{
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
	CBB_FUNC_VC1_S		stFuncVC1Cfg;
	CBB_FRAME_CHANNEL_E enChan;
	HI_BOOL				bEnTile;
	HI_U32				u32BitWth;
	
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

    if(pstFrmCfg->enBitWidth==CBB_BITWIDTH_8BIT)
    {
        u32BitWth = 0;
    }
	else if (pstFrmCfg->enBitWidth==CBB_BITWIDTH_10BIT)
	{
		u32BitWth = 1;
	}
	else
	{
		CBB_ERRMSG("invalid enPixelFmt\n");
		return HI_FAILURE;
	}
	
	switch (pstFrmCfg->enPixelFmt)
	{
		case HI_DRV_PIX_FMT_NV12_TILE: /* 12 tile  */
		case HI_DRV_PIX_FMT_NV21_TILE: /* 21 tile  */
    	case HI_DRV_PIX_FMT_YUV400_TILE: /* 21 tile  */
    	case HI_DRV_PIX_FMT_NV12_TILE_CMP:
    	case HI_DRV_PIX_FMT_NV21_TILE_CMP:
			bEnTile = HI_TRUE;
    		break;
    	default:
			bEnTile = HI_FALSE;
    		break;
	}
	if (pstFrmCfg->bSmmu)
	{
		//mmu 每个通道独立配置
	}

	VPSS_DRV_Set_VPSS_IMGSIZE(pstVpssNode, pstFrmCfg->u32Height, pstFrmCfg->u32Width);

	stFuncVC1Cfg.bEnable = HI_FALSE;
	enChan = CBB_FRAME_READ_CUR;
	VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VC1, (HI_VOID *)&stFuncVC1Cfg, (HI_VOID*)&enChan);

	VPSS_DRV_Set_VPSS_CUR_CTRL(pstVpssNode, pstFrmCfg->stOffset.s32Y, pstFrmCfg->stOffset.s32X, bEnTile);
	VPSS_DRV_Set_VPSS_CURYADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y);
	VPSS_DRV_Set_VPSS_CURCADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C);
	VPSS_DRV_Set_VPSS_CURSTRIDE(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y);

	return HI_SUCCESS;
}

HI_S32 CBB_CfgFrm_ReadNx1(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr)
{
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
	CBB_FUNC_VC1_S		stFuncVC1Cfg;
	CBB_FRAME_CHANNEL_E enChan;
	HI_BOOL				bEnTile;
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;
    
	switch (pstFrmCfg->enPixelFmt)
	{
		case HI_DRV_PIX_FMT_NV12_TILE: /* 12 tile  */
		case HI_DRV_PIX_FMT_NV21_TILE: /* 21 tile  */
    	case HI_DRV_PIX_FMT_YUV400_TILE: /* 21 tile  */
    	case HI_DRV_PIX_FMT_NV12_TILE_CMP:
    	case HI_DRV_PIX_FMT_NV21_TILE_CMP:

			bEnTile = HI_TRUE;
    		break;
    	default:
			bEnTile = HI_FALSE;
    		break;
	}
	if (pstFrmCfg->bSmmu)
	{
		//mmu 每个通道独立配置
	}

	stFuncVC1Cfg.bEnable = HI_FALSE;
	enChan = CBB_FRAME_READ_NX1;
	VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VC1, (HI_VOID *)&stFuncVC1Cfg, (HI_VOID*)&enChan);
	VPSS_DRV_Set_VPSS_NXT1_CTRL(pstVpssNode, pstFrmCfg->stOffset.s32Y, pstFrmCfg->stOffset.s32X,bEnTile);
	VPSS_DRV_Set_VPSS_NXT1YADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y);
	VPSS_DRV_Set_VPSS_NXT1CADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C);
	VPSS_DRV_Set_VPSS_NXT1STRIDE(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y);

	return HI_SUCCESS;
}

HI_U32 CBB_FrameFmtTrans( HI_DRV_PIX_FORMAT_E	enPixelFmt )
{
	HI_U32 u32Fmt = 0;

	switch(enPixelFmt)
	{
		case HI_DRV_PIX_FMT_NV21:
		case HI_DRV_PIX_FMT_NV12:
		case HI_DRV_PIX_FMT_NV12_CMP:
		case HI_DRV_PIX_FMT_NV21_CMP:
		case HI_DRV_PIX_FMT_NV12_TILE:
		case HI_DRV_PIX_FMT_NV21_TILE:
		case HI_DRV_PIX_FMT_NV21_TILE_CMP:
		case HI_DRV_PIX_FMT_NV12_TILE_CMP:
			u32Fmt = 0x0;
			break;
		case HI_DRV_PIX_FMT_NV61_2X1_CMP:
		case HI_DRV_PIX_FMT_NV16_2X1_CMP:
		case HI_DRV_PIX_FMT_NV61_CMP:
		case HI_DRV_PIX_FMT_NV16_CMP:
		case HI_DRV_PIX_FMT_NV16_2X1:
		case HI_DRV_PIX_FMT_NV61_2X1:
		case HI_DRV_PIX_FMT_NV16:
		case HI_DRV_PIX_FMT_NV61:
			u32Fmt = 0x1;
			break;
		case HI_DRV_PIX_FMT_YUV400:
			u32Fmt = 0x4;
			break; 
		case HI_DRV_PIX_FMT_YUV_444:
			u32Fmt = 0x5;
			break;	
		case HI_DRV_PIX_FMT_YUV422_2X1:
			u32Fmt = 0x6;
			break;
		case HI_DRV_PIX_FMT_YUV420p:
			u32Fmt = 0x8;
			break;	
		case HI_DRV_PIX_FMT_YUV411:
			u32Fmt = 0x9;
			break;	
		case HI_DRV_PIX_FMT_YUV410p:
			u32Fmt = 0xa;
			break;	
		case HI_DRV_PIX_FMT_YUYV:
			u32Fmt = 0xb;
			break;	
		case HI_DRV_PIX_FMT_YVYU:
			u32Fmt = 0xc;
			break;	
		case HI_DRV_PIX_FMT_UYVY:
			u32Fmt = 0xd;
			break; 
		default:
			CBB_ERRMSG("REG ERROR");
	}

	return u32Fmt;
}

HI_S32 CBB_CfgFrm_ReadNx2(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
	HI_U32 u32BitWth=0;
	HI_U32 u32Format=0;
	HI_BOOL bEnTile = HI_FALSE;
	HI_BOOL bDeCmp;
	HI_U32 u32Field;
	HI_U32 u32FieldFirst;
	CBB_FUNC_VC1_S		stFuncVC1Cfg;
	CBB_FRAME_CHANNEL_E enChan;
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;
	

    if(pstFrmCfg->enBitWidth==CBB_BITWIDTH_8BIT)
    {
        u32BitWth = 0;
    }
	else if (pstFrmCfg->enBitWidth==CBB_BITWIDTH_10BIT)
	{
		u32BitWth = 1;
	}
	else
	{
		CBB_ERRMSG("invalid enPixelFmt\n");
		return HI_FAILURE;
	}

	switch (pstFrmCfg->enPixelFmt)
	{
		case HI_DRV_PIX_FMT_NV12_TILE: /* 12 tile  */
		case HI_DRV_PIX_FMT_NV21_TILE: /* 21 tile  */
    	case HI_DRV_PIX_FMT_YUV400_TILE: /* 21 tile  */
    	case HI_DRV_PIX_FMT_NV12_TILE_CMP:
    	case HI_DRV_PIX_FMT_NV21_TILE_CMP:

			bEnTile = HI_TRUE;
    		break;
    	default:
			bEnTile = HI_FALSE;
    		break;
	}

	u32Format = CBB_FrameFmtTrans(pstFrmCfg->enPixelFmt);

	if (pstFrmCfg->bProgressive)
	{
		VPSS_DRV_Set_bfield(pstVpssNode, HI_FALSE);
	}
	else
	{
		VPSS_DRV_Set_bfield(pstVpssNode, HI_TRUE);
	}
	
	if (pstFrmCfg->bTop)
	{
		u32Field = 0x0;
	}
	else
	{
		u32Field = 0x1;
	}

	if (pstFrmCfg->bTopFirst)
	{
		u32FieldFirst = 0x0;
	}
	else
	{
		u32FieldFirst = 0x1;
	}

	VPSS_MEM_CLEAR(stFuncVC1Cfg);

	// TODO: 增加CTS 的设置


	VPSS_DRV_Set_bfield_mode(pstVpssNode, u32Field);
	VPSS_DRV_Set_bfield_first(pstVpssNode, u32FieldFirst);

	stFuncVC1Cfg.bEnable = HI_FALSE;
	enChan = CBB_FRAME_READ_NX2;
	VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VC1, (HI_VOID *)&stFuncVC1Cfg, (HI_VOID*)&enChan);
	VPSS_DRV_Set_prot(pstVpssNode, pstFrmCfg->bSecure);
	
	if (pstFrmCfg->bCompress)
	{
		// 98mv200 does not have cmp&interlace scene, only configs top-header-addr 
		VPSS_DRV_Set_VPSS_NX2Y_HEAD_ADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead);
		VPSS_DRV_Set_VPSS_NX2C_HEAD_ADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead);
		VPSS_DRV_Set_VPSS_NX2_HEAD_STRIDE(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride);
		bDeCmp = HI_TRUE;
	}
	else
	{
		bDeCmp = HI_FALSE;
	}

	VPSS_DRV_Set_VPSS_IN_CTRL_nx2(pstVpssNode, u32BitWth, u32Format);
	VPSS_DRV_Set_VPSS_IMGSIZE(pstVpssNode, pstFrmCfg->u32Height, pstFrmCfg->u32Width);

	VPSS_DRV_Set_VPSS_NXT2_CTRL(pstVpssNode, bDeCmp, pstFrmCfg->stOffset.s32Y, pstFrmCfg->stOffset.s32X, bEnTile);
	VPSS_DRV_Set_VPSS_NXT2YADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y);
	VPSS_DRV_Set_VPSS_NXT2CADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C);
	VPSS_DRV_Set_VPSS_NXT2STRIDE(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y);

	if( CBB_BITWIDTH_10BIT == pstFrmCfg->enBitWidth )
	{
		VPSS_DRV_Set_VPSS_NX2Y_LB_ADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y);
		VPSS_DRV_Set_VPSS_NX2C_LB_ADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C);
		VPSS_DRV_Set_VPSS_NX2_LB_STRIDE(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y);
	}

	if (pstFrmCfg->bSmmu)
	{
		//mmu 每个通道独立配置
	}

	return HI_SUCCESS;
}


HI_S32 CBB_CfgFrm_ReadRef(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
	HI_U32 u32BitWth=0;
	CBB_FUNC_VC1_S		stFuncVC1Cfg;
	CBB_FRAME_CHANNEL_E enChan;
	HI_BOOL 			bEnTile;
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;
	

    if(pstFrmCfg->enBitWidth==CBB_BITWIDTH_8BIT)
    {
        u32BitWth = 0;
    }
	else if (pstFrmCfg->enBitWidth==CBB_BITWIDTH_10BIT)
	{
		u32BitWth = 1;
	}
	else
	{
		CBB_ERRMSG("invalid enPixelFmt\n");
		return HI_FAILURE;
	}

	switch (pstFrmCfg->enPixelFmt)
	{
		case HI_DRV_PIX_FMT_NV12_TILE: /* 12 tile  */
		case HI_DRV_PIX_FMT_NV21_TILE: /* 21 tile  */
		case HI_DRV_PIX_FMT_YUV400_TILE: /* 21 tile  */
			bEnTile = HI_TRUE;
			break;
		default:
			bEnTile = HI_FALSE;
			break;
	}

	stFuncVC1Cfg.bEnable = HI_FALSE;
	enChan = CBB_FRAME_READ_REF;
	VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VC1, (HI_VOID *)&stFuncVC1Cfg, (HI_VOID*)&enChan);
	VPSS_DRV_Set_VPSS_REFSIZE(pstVpssNode, pstFrmCfg->u32Height, pstFrmCfg->u32Width);
	VPSS_DRV_Set_VPSS_REF_CTRL(pstVpssNode, pstFrmCfg->stOffset.s32Y, pstFrmCfg->stOffset.s32X, bEnTile);
	VPSS_DRV_Set_VPSS_IN_CTRL_ref(pstVpssNode, u32BitWth);

	VPSS_DRV_Set_VPSS_REFYADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y);
	VPSS_DRV_Set_VPSS_REFCADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C);
	VPSS_DRV_Set_VPSS_REFSTRIDE(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C,
													pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y);


	if (pstFrmCfg->bSmmu)
	{
		//mmu 每个通道独立配置
	}

	return HI_SUCCESS;
}

HI_S32 CBB_CfgFrm_ReadRee(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	CBB_FRAME_ADDR_S	*pstAddr;
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	pstAddr = &(pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT]);
	VPSS_DRV_Set_VPSS_REEYADDR(pstVpssNode, pstAddr->u32PhyAddr_Y);
	VPSS_DRV_Set_VPSS_REECADDR(pstVpssNode, pstAddr->u32PhyAddr_C);
	VPSS_DRV_Set_VPSS_REESTRIDE(pstVpssNode, pstAddr->u32Stride_C, pstAddr->u32Stride_Y);

	if (pstFrmCfg->bSmmu)
	{
		//mmu 每个通道独立配置
	}

	return HI_SUCCESS;
}


HI_S32 CBB_CfgFrm_WriteVhd0(CBB_FRAME_S *pstFrmCfg, HI_VOID* pArgs, CBB_REG_ADDR_S stRegAddr)
{
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    HI_U32 u32BitWth=0;
    HI_U32 u32Format=0;
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

    if(pstFrmCfg->enBitWidth==CBB_BITWIDTH_8BIT)
    {
        u32BitWth = 0;
    }
	else if ((pstFrmCfg->enBitWidth==CBB_BITWIDTH_10BIT) || (pstFrmCfg->enBitWidth==CBB_BITWIDTH_10BIT_CTS))
	{
		u32BitWth = 1;
	}
	else
	{
		CBB_ERRMSG("invalid enPixelFmt\n");
		return HI_FAILURE;
	}

    switch(pstFrmCfg->enPixelFmt)
    {
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
            u32Format = 0x0;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV16:
        case HI_DRV_PIX_FMT_NV61:
            u32Format = 0x1;
            break;
        default:            
            return HI_FAILURE;
    }


	VPSS_DRV_Set_VPSS_VHD0SIZE(pstVpssNode, pstFrmCfg->u32Height,  pstFrmCfg->u32Width);
	VPSS_DRV_Set_VPSS_VHD0CTRL_bitw(pstVpssNode, u32BitWth);
	VPSS_DRV_Set_VPSS_VHD0CTRL_format(pstVpssNode, u32Format);

	if (pstFrmCfg->enBitWidth > CBB_BITWIDTH_8BIT)
	{
		VPSS_DRV_Set_VPSS_VHD0CTRL_dither(pstVpssNode, REG_DITHER_MODE_DITHER,HI_FALSE);
	}
	else
	{
		VPSS_DRV_Set_VPSS_VHD0CTRL_dither(pstVpssNode, REG_DITHER_MODE_DITHER,HI_TRUE);
	}


	VPSS_DRV_Set_VPSS_VHD0YADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y);
	VPSS_DRV_Set_VPSS_VHD0CADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C);	
	VPSS_DRV_Set_VPSS_VHD0STRIDE(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C, 
													pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y);

	if (pstFrmCfg->bSmmu)
	{
		//mmu 每个通道独立配置
	}

	VPSS_DRV_Set_vhd0_en(pstVpssNode, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 CBB_CfgFrm_WriteRfr(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr, HI_BOOL bEnable)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
	HI_U32 u32BitWth=0;
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

    if(pstFrmCfg->enBitWidth==CBB_BITWIDTH_8BIT)
    {
        u32BitWth = 0;
    }
	else if (pstFrmCfg->enBitWidth==CBB_BITWIDTH_10BIT)
	{
		u32BitWth = 1;
	}
	else
	{
		CBB_ERRMSG("invalid enPixelFmt\n");
		return HI_FAILURE;
	}

	VPSS_DRV_Set_VPSS_REFSIZE(pstVpssNode, pstFrmCfg->u32Height, pstFrmCfg->u32Width);
	VPSS_DRV_Set_VPSS_IN_CTRL_rfr_bitw(pstVpssNode, u32BitWth);
	VPSS_DRV_Set_VPSS_RFRYADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y);
	VPSS_DRV_Set_VPSS_RFRCADDR(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C);
	VPSS_DRV_Set_VPSS_RFRSTRIDE(pstVpssNode, pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C,
												pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y);
	if (pstFrmCfg->enBitWidth > CBB_BITWIDTH_8BIT)
	{
		VPSS_DRV_Set_VPSS_RFRCTRL(pstVpssNode, REG_DITHER_MODE_DITHER, HI_FALSE);
	}
	else
	{
		VPSS_DRV_Set_VPSS_RFRCTRL(pstVpssNode, REG_DITHER_MODE_DITHER, HI_TRUE);
	}

	VPSS_DRV_Set_rfr_en(pstVpssNode, bEnable);

	return HI_SUCCESS;
}

HI_S32 CBB_CfgFrm_WriteCue(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	CBB_FRAME_ADDR_S	*pstAddr;
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	pstAddr = &(pstFrmCfg->stAddr[CBB_FREAM_BUF_8BIT]);
	VPSS_DRV_Set_VPSS_CUEYADDR(pstVpssNode, pstAddr->u32PhyAddr_Y);
	VPSS_DRV_Set_VPSS_CUECADDR(pstVpssNode, pstAddr->u32PhyAddr_C);
	VPSS_DRV_Set_VPSS_CUESTRIDE(pstVpssNode, pstAddr->u32Stride_C, pstAddr->u32Stride_Y);

	if (pstFrmCfg->bSmmu)
	{
		//mmu 每个通道独立配置
	}

	return HI_SUCCESS;
}

