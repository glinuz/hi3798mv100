#include "vpss_cbb_base.h"
#include "vpss_cbb_reg.h"
#include "vpss_cbb_component.h"


HI_VOID VPSS_BaseOpt_CreateListandStart(CBB_START_CFG_S *pstStartCfg, CBB_REG_ADDR_S stRegAddr)
{
    HI_U32 enLastValid = DEF_CBB_MAX_NODE_NUM;
    HI_U32 enId;
    HI_U32 i=0;
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    S_VPSS_REGS_TYPE *pstVpss;      //逻辑虚拟基地址

    for(i=0;i<DEF_CBB_MAX_NODE_NUM;i++)
    {
       enId = DEF_CBB_MAX_NODE_NUM - 1 -i;
       if(HI_TRUE==pstStartCfg->u32NodeValid[enId])
       {
           if(DEF_CBB_MAX_NODE_NUM==enLastValid)
           {
               pstVpssNode = (S_VPSS_REGS_TYPE *)pstStartCfg->pu8NodeVirAddr[enId];
               VPSS_DRV_Set_VPSS_PNEXT(pstVpssNode, 0);
           }
           else
           {
               pstVpssNode = (S_VPSS_REGS_TYPE *)pstStartCfg->pu8NodeVirAddr[enId];
               VPSS_DRV_Set_VPSS_PNEXT(pstVpssNode, (HI_U32 *)((unsigned long)(pstStartCfg->u32NodePhyAddr[enLastValid])));
           }

           enLastValid = enId;
       }
    }
    
	HI_ASSERT(enLastValid != DEF_CBB_MAX_NODE_NUM);

	if (enLastValid == DEF_CBB_MAX_NODE_NUM)
	{
		VPSS_FATAL("No Node Needs Start VPSS !!!\n");
		return;
	}
	else
	{
		pstVpss = (S_VPSS_REGS_TYPE *)stRegAddr.pu8BaseVirAddr;
        VPSS_DRV_Set_VPSS_PNEXT(pstVpss, (HI_U32 *)((unsigned long)(pstStartCfg->u32NodePhyAddr[enLastValid])));
		VPSS_DRV_Set_VPSS_START(pstVpss);
	}

}

HI_VOID VPSS_BaseOpt_GetRawInt(HI_BOOL *pbIsrState, CBB_REG_ADDR_S stRegAddr)
{
    HI_U32 regval=0;
    S_VPSS_REGS_TYPE *pstVpss;  //逻辑虚拟基地址
    pstVpss = (S_VPSS_REGS_TYPE *)stRegAddr.pu8BaseVirAddr;
    regval = VPSS_DRV_GetRawIntStatus(pstVpss);
    pbIsrState[CBB_ISR_NODE_COMPLETE]   = 0x1&(regval>>CBB_ISR_NODE_COMPLETE)   ;
    pbIsrState[CBB_ISR_LIST_COMPLETE]   = 0x1&(regval>>CBB_ISR_LIST_COMPLETE)   ;
    pbIsrState[CBB_ISR_BUS_READ_ERR]    = 0x1&(regval>>CBB_ISR_BUS_READ_ERR)    ;
    pbIsrState[CBB_ISR_BUS_WRITE_ERR]   = 0x1&(regval>>CBB_ISR_BUS_WRITE_ERR)   ;
    pbIsrState[CBB_ISR_DCMP_ERR]        = 0x1&(regval>>CBB_ISR_DCMP_ERR)        ;
    pbIsrState[CBB_ISR_VHD0_TUNNEL]     = 0x1&(regval>>CBB_ISR_VHD0_TUNNEL)     ;
}

HI_VOID VPSS_BaseOpt_GetMaskInt(HI_BOOL *pbIsrState, CBB_REG_ADDR_S stRegAddr)
{
    HI_U32 regval=0;
	HI_U32 u32SecureState;
	HI_U32 u32NoSecureState;

    S_VPSS_REGS_TYPE *pstVpss;  //逻辑虚拟基地址
    pstVpss = (S_VPSS_REGS_TYPE *)stRegAddr.pu8BaseVirAddr;
    regval = VPSS_DRV_GetIntStatus(pstVpss);
   
    pbIsrState[CBB_ISR_NODE_COMPLETE]   = 0x1&(regval>>CBB_ISR_NODE_COMPLETE)   ;
	pbIsrState[CBB_ISR_TIMEOUT]			= 0x1&(regval>>CBB_ISR_TIMEOUT)   		;    
    pbIsrState[CBB_ISR_LIST_COMPLETE]   = 0x1&(regval>>CBB_ISR_LIST_COMPLETE)   ;
    pbIsrState[CBB_ISR_BUS_READ_ERR]    = 0x1&(regval>>CBB_ISR_BUS_READ_ERR)    ;
    pbIsrState[CBB_ISR_BUS_WRITE_ERR]   = 0x1&(regval>>CBB_ISR_BUS_WRITE_ERR)   ;
    pbIsrState[CBB_ISR_DCMP_ERR]        = 0x1&(regval>>CBB_ISR_DCMP_ERR)        ;
    pbIsrState[CBB_ISR_VHD0_TUNNEL]     = 0x1&(regval>>CBB_ISR_VHD0_TUNNEL)     ;

	if (pbIsrState[CBB_ISR_TIMEOUT])
	{
		VPSS_FATAL("VPSS_DEBUG0 : 0x%X\n", VPSS_REG_RegRead((volatile HI_U32*)&(pstVpss->VPSS_DEBUG0.u32)));
	}

	VPSS_DRV_Get_SMMU_INTSTAT_S(pstVpss, &u32SecureState, &u32NoSecureState);

	pbIsrState[CBB_ISR_SMMU_S_TLBMISS]		= 0x1 & (u32SecureState >> (CBB_ISR_SMMU_S_TLBMISS - CBB_ISR_SMMU_S_MIN));
	pbIsrState[CBB_ISR_SMMU_S_PTW_TRANS]	= 0x1 & (u32SecureState >> (CBB_ISR_SMMU_S_PTW_TRANS - CBB_ISR_SMMU_S_MIN));
	pbIsrState[CBB_ISR_SMMU_S_TLBINVALID_RD]	= 0x1 & (u32SecureState >> (CBB_ISR_SMMU_S_TLBINVALID_RD - CBB_ISR_SMMU_S_MIN));
	pbIsrState[CBB_ISR_SMMU_S_TLBINVALID_WR]	= 0x1 & (u32SecureState >> (CBB_ISR_SMMU_S_TLBINVALID_WR - CBB_ISR_SMMU_S_MIN));

	pbIsrState[CBB_ISR_SMMU_NS_TLBMISS]		= 0x1 & (u32NoSecureState >> (CBB_ISR_SMMU_NS_TLBMISS - CBB_ISR_SMMU_NS_MIN));
	pbIsrState[CBB_ISR_SMMU_NS_PTW_TRANS]	= 0x1 & (u32NoSecureState >> (CBB_ISR_SMMU_NS_PTW_TRANS - CBB_ISR_SMMU_NS_MIN));
	pbIsrState[CBB_ISR_SMMU_NS_TLBINVALID_RD] = 0x1 & (u32NoSecureState >> (CBB_ISR_SMMU_NS_TLBINVALID_RD - CBB_ISR_SMMU_NS_MIN));
	pbIsrState[CBB_ISR_SMMU_NS_TLBINVALID_WR] = 0x1 & (u32NoSecureState >> (CBB_ISR_SMMU_NS_TLBINVALID_WR - CBB_ISR_SMMU_NS_MIN));

}


HI_VOID VPSS_BaseOpt_ClrInt(HI_BOOL *pbIsrState, CBB_REG_ADDR_S stRegAddr)
{
    HI_U32 u32ClrInt=0;
    S_VPSS_REGS_TYPE *pstVpss; //逻辑虚拟基地址
    pstVpss = (S_VPSS_REGS_TYPE *)stRegAddr.pu8BaseVirAddr;
    u32ClrInt = ( (pbIsrState[CBB_ISR_NODE_COMPLETE] << CBB_ISR_NODE_COMPLETE   )
                 |(pbIsrState[CBB_ISR_LIST_COMPLETE] << CBB_ISR_LIST_COMPLETE   )
                 |(pbIsrState[CBB_ISR_BUS_READ_ERR]  << CBB_ISR_BUS_READ_ERR    )
                 |(pbIsrState[CBB_ISR_BUS_WRITE_ERR] << CBB_ISR_BUS_WRITE_ERR   )
                 |(pbIsrState[CBB_ISR_DCMP_ERR]      << CBB_ISR_DCMP_ERR        )
                 |(pbIsrState[CBB_ISR_VHD0_TUNNEL]   << CBB_ISR_VHD0_TUNNEL     )
                 );

    VPSS_DRV_ClrInt(pstVpss, u32ClrInt);

	VPSS_DRV_Set_SMMU_INTCLR_S(pstVpss, 
								pbIsrState[CBB_ISR_SMMU_S_TLBINVALID_WR], 
								pbIsrState[CBB_ISR_SMMU_S_TLBINVALID_RD], 
								pbIsrState[CBB_ISR_SMMU_S_PTW_TRANS],
								pbIsrState[CBB_ISR_SMMU_S_TLBMISS]);

	VPSS_DRV_Set_SMMU_INTCLR_NS(pstVpss, 
							pbIsrState[CBB_ISR_SMMU_NS_TLBINVALID_WR], 
							pbIsrState[CBB_ISR_SMMU_NS_TLBINVALID_RD], 
							pbIsrState[CBB_ISR_SMMU_NS_PTW_TRANS],
							pbIsrState[CBB_ISR_SMMU_NS_TLBMISS]);

}

HI_VOID VPSS_BaseOpt_SetIntMask(HI_BOOL *pbIsrMask, CBB_REG_ADDR_S stRegAddr)
{
    HI_U32 u32Mask;
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;
    u32Mask   = ( (pbIsrMask[CBB_ISR_NODE_COMPLETE] << CBB_ISR_NODE_COMPLETE   )
                 |(pbIsrMask[CBB_ISR_LIST_COMPLETE] << CBB_ISR_LIST_COMPLETE   )
                 |(pbIsrMask[CBB_ISR_BUS_READ_ERR]  << CBB_ISR_BUS_READ_ERR    )
                 |(pbIsrMask[CBB_ISR_BUS_WRITE_ERR] << CBB_ISR_BUS_WRITE_ERR   )
                 |(pbIsrMask[CBB_ISR_DCMP_ERR]      << CBB_ISR_DCMP_ERR        )
                 |(pbIsrMask[CBB_ISR_VHD0_TUNNEL]   << CBB_ISR_VHD0_TUNNEL     )

                 );
    VPSS_DRV_SetIntMask(pstVpssNode, u32Mask);

	VPSS_DRV_Set_SMMU_INTMASK_S(pstVpssNode, pbIsrMask[CBB_ISR_SMMU_S_TLBINVALID_WR], 
								pbIsrMask[CBB_ISR_SMMU_S_TLBINVALID_RD], 
								pbIsrMask[CBB_ISR_SMMU_S_PTW_TRANS],
								pbIsrMask[CBB_ISR_SMMU_S_TLBMISS]);

	VPSS_DRV_Set_SMMU_INTMASK_NS(pstVpssNode, pbIsrMask[CBB_ISR_SMMU_NS_TLBINVALID_WR], 
							pbIsrMask[CBB_ISR_SMMU_NS_TLBINVALID_RD], 
							pbIsrMask[CBB_ISR_SMMU_NS_PTW_TRANS],
							pbIsrMask[CBB_ISR_SMMU_NS_TLBMISS]);

}

HI_VOID VPSS_BaseOpt_SetOutStd(CBB_OUTSTANDING_S *pOutStd, CBB_REG_ADDR_S stRegAddr)
{
    S_VPSS_REGS_TYPE *pstVpss; //逻辑虚拟基地址
    pstVpss = (S_VPSS_REGS_TYPE *)stRegAddr.pu8BaseVirAddr;
	VPSS_DRV_Set_VPSS_MISCELLANEOUS_OUTSTANDING(pstVpss, pOutStd->u32WOutStd, pOutStd->u32ROutStd);
}

HI_VOID VPSS_BaseOpt_SetBurstLength(HI_U32 u32BurstLength, CBB_REG_ADDR_S stRegAddr)
{
    S_VPSS_REGS_TYPE *pstVpss; //逻辑虚拟基地址
    pstVpss = (S_VPSS_REGS_TYPE *)stRegAddr.pu8BaseVirAddr;
	VPSS_DRV_Set_VPSS_MISCELLANEOUS_BURSTLENGTH(pstVpss, u32BurstLength);
}
