#include "vpss_cbb_info.h"
#include "vpss_cbb_reg.h"

HI_VOID VPSS_InfoCfg_Dei_ST(CBB_INFO_S *pstInfoDeiCfg, CBB_REG_ADDR_S stRegAddr)
{
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_STWADDR(pstVpssNode, pstInfoDeiCfg->u32WPhyAddr);
	VPSS_DRV_Set_VPSS_STRADDR(pstVpssNode, pstInfoDeiCfg->u32RPhyAddr);
	VPSS_DRV_Set_VPSS_STSTRIDE(pstVpssNode, pstInfoDeiCfg->u32Stride);
}

HI_VOID VPSS_InfoGet_All(S_STT_REGS_TYPE *pstSttReg, CBB_INFO_S *pstInfoStt)
{
    VPSS_SAFE_MEMCPY(pstSttReg, pstInfoStt->pu8RVirAddr, sizeof(S_STT_REGS_TYPE));
}

HI_VOID VPSS_InfoCfg_Prjv(CBB_INFO_PRJV_S *pstInfoPrjvCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr; 

	VPSS_DRV_Set_VPSS_PRJVNX2ADDR(pstVpssNode, pstInfoPrjvCfg->u32Nx2WriteAddr);
	VPSS_DRV_Set_VPSS_PRJNX2STRIDE_V(pstVpssNode, pstInfoPrjvCfg->u32Stride);
	VPSS_DRV_Set_VPSS_PRJVCURADDR(pstVpssNode, pstInfoPrjvCfg->u32CurReadAddr);
	VPSS_DRV_Set_VPSS_PRJCURSTRIDE_V(pstVpssNode, pstInfoPrjvCfg->u32Stride);
}

HI_VOID VPSS_InfoCfg_Prjh(CBB_INFO_PRJH_S *pstInfoPrjhCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr; 

	VPSS_DRV_Set_VPSS_PRJHNX2ADDR(pstVpssNode, pstInfoPrjhCfg->u32Nx2WriteAddr);
	VPSS_DRV_Set_VPSS_PRJNX2STRIDE_H(pstVpssNode, pstInfoPrjhCfg->u32Stride);
	VPSS_DRV_Set_VPSS_PRJHCURADDR(pstVpssNode, pstInfoPrjhCfg->u32CurReadAddr);
	VPSS_DRV_Set_VPSS_PRJCURSTRIDE_H(pstVpssNode, pstInfoPrjhCfg->u32Stride);
}

HI_VOID VPSS_InfoCfg_Blkmv(CBB_INFO_BLKMV_S *pstInfoBlkmvCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_BLKMVCURADDR(pstVpssNode, pstInfoBlkmvCfg->u32CurReadAddr);
	VPSS_DRV_Set_VPSS_BLKMVREFADDR(pstVpssNode, pstInfoBlkmvCfg->u32RefReadAddr);
	VPSS_DRV_Set_VPSS_BLKMVSTRIDE(pstVpssNode, pstInfoBlkmvCfg->u32Stride, pstInfoBlkmvCfg->u32Stride);
	VPSS_DRV_Set_VPSS_BLKMVNX1ADDR(pstVpssNode, pstInfoBlkmvCfg->u32Nx1ReadAddr);
	VPSS_DRV_Set_VPSS_BLKMVNX2ADDR(pstVpssNode, pstInfoBlkmvCfg->u32Nx2WriteAddr);
	VPSS_DRV_Set_VPSS_BLKMVNX1STRIDE(pstVpssNode, pstInfoBlkmvCfg->u32Stride);
	VPSS_DRV_Set_VPSS_BLKMVNX2STRIDE(pstVpssNode, pstInfoBlkmvCfg->u32Stride);
}

HI_VOID VPSS_InfoCfg_Rgmv(CBB_INFO_RGMV_S *pstInfoRgmvCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_RGMVCURADDR(pstVpssNode, pstInfoRgmvCfg->u32CurReadAddr);
	VPSS_DRV_Set_VPSS_RGMVNX1ADDR(pstVpssNode, pstInfoRgmvCfg->u32Nx1ReadAddr);
	VPSS_DRV_Set_VPSS_RGMVSTRIDE(pstVpssNode, pstInfoRgmvCfg->u32Stride, pstInfoRgmvCfg->u32Stride);
	VPSS_DRV_Set_VPSS_RGMVNX2ADDR(pstVpssNode, pstInfoRgmvCfg->u32Nx2WriteAddr);
	VPSS_DRV_Set_VPSS_RGMVNX2STRIDE(pstVpssNode, pstInfoRgmvCfg->u32Stride);

	VPSS_DRV_Set_rgme_en(pstVpssNode, HI_TRUE);
}

HI_VOID VPSS_InfoCfg_Snr_Mad(CBB_INFO_S *pstInfoSnrCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

    VPSS_DRV_Set_VPSS_SNR_MAD_RADDR(pstVpssNode, pstInfoSnrCfg->u32RPhyAddr);
    VPSS_DRV_Set_VPSS_MADSTRIDE(pstVpssNode, pstInfoSnrCfg->u32Stride);
}

HI_VOID VPSS_InfoCfg_Tnr_Mad(CBB_INFO_S *pstInfoTnrCfg, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_MAD_RADDR(pstVpssNode, pstInfoTnrCfg->u32RPhyAddr);
	VPSS_DRV_Set_VPSS_MAD_WADDR(pstVpssNode, pstInfoTnrCfg->u32WPhyAddr);
	VPSS_DRV_Set_VPSS_MADSTRIDE(pstVpssNode, pstInfoTnrCfg->u32Stride);

}

HI_VOID VPSS_INFOCfg_SttInfo(CBB_INFO_S *pstInfoStt, CBB_REG_ADDR_S stRegAddr)
{
	S_VPSS_REGS_TYPE *pstVpssNode;	//DDR中节点首地址
	pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;

	VPSS_DRV_Set_VPSS_STT_W_ADDR(pstVpssNode, pstInfoStt->u32WPhyAddr);

#ifdef NEVER
	VPSS_DRV_Set_VPSS_STWADDR(pstVpssNode, pstInfoStt->u32WPhyAddr);
	VPSS_DRV_Set_VPSS_STRADDR(pstVpssNode, pstInfoStt->u32RPhyAddr);
	VPSS_DRV_Set_VPSS_STSTRIDE(pstVpssNode, pstInfoStt->u32Stride);
#endif /* NEVER */

}

