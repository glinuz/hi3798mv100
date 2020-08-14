/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

 File Name    : pq_hal_algchange.c
 Version        : Initial Draft
 Author         : p00203646
 Created       : 2015/12/23
 Description   :
******************************************************************************/
#include "hi_type.h"
#include "hi_drv_mem.h"
#include "drv_pq_table.h"
#include "pq_hal_comm.h"
#include "pq_hal_algchange.h"


#define VID_REGION_NUM        16

/*SNR*/
static HI_VOID PQ_HAL_SetVdpSnrCfgAddr(HI_U32 u32Data, HI_U32 u32Addr)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_SNR_CFGAD         V0_SNR_CFGAD;

    if (u32Data >= VID_MAX)
    {
        return ;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_SNR_CFGAD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_SNR_CFGAD.u32)) + u32Data * PQ_VP_OFFSET));
    V0_SNR_CFGAD.bits.coef_addr = u32Addr;

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_SNR_CFGAD.u32)) + u32Data * PQ_VP_OFFSET), V0_SNR_CFGAD.u32);
    return;
}

HI_S32  PQ_HAL_SetVdpSnrParaUpd (HI_U32 u32Data)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_PARAUP V0_PARAUP;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    V0_PARAUP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PARAUP.u32)) + u32Data * PQ_VP_OFFSET));
    V0_PARAUP.bits.snr_upd = 0x1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PARAUP.u32)) + u32Data * PQ_VP_OFFSET), V0_PARAUP.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetVdpSnrEnable(HI_U32 u32Data, HI_U32 u32Data1)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_PRERD V0_PRERD;
    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    V0_PRERD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VP_OFFSET));
    V0_PRERD.bits.snr_en = u32Data1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VP_OFFSET), V0_PRERD.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_InitVdpSnr(HI_VOID)
{
    HI_U32 u32Vid      = 0;
    HI_U32 u32HandleNo = 0;
    HI_U32 u32SNRAddrOffset = 0;
    HI_U32 u32SNRParaSize   = 0;
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);

    u32SNRParaSize   = VDP_PQ_REG_SIZE(V0_SNR_ENABLE, V0_COLOR_BASED_LUT_16d);
    u32SNRAddrOffset = VDP_PQ_REG_SIZE(VOCTRL, V0_SNR_ENABLE) - sizeof(HI_U32);

    PQ_HAL_CopyRegBySize((HI_VOID *)(g_stIPSelAlgBuffer.pu8StartVirAddr + u32SNRAddrOffset),
                         (HI_VOID *)(&pstVirReg->VPSS_SNR_ENABLE), u32SNRParaSize / 4);

    PQ_HAL_SetVdpSnrCfgAddr(u32Vid, g_stIPSelAlgBuffer.u32StartPhyAddr + u32SNRAddrOffset);
    return PQ_HAL_SetVdpSnrParaUpd(u32Vid);
}

/*TNR*/
HI_S32 PQ_HAL_SetTnrEnable(HI_U32 u32Data, HI_U32 u32En)
{
    U_V0_PRERD V0_PRERD;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_PRERD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VID_OFFSET));
    V0_PRERD.bits.tnr_en = u32En;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VID_OFFSET), V0_PRERD.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrWbcEnable(HI_U32 u32Data, HI_U32 u32Enable)
{
    U_WBC_FI_CTRL   WBC_FI_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_FI_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_FI_CTRL.bits.wbc_en = u32Enable;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CTRL.u32)) + u32Data * PQ_VID_OFFSET), WBC_FI_CTRL.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrWbcCEnable(HI_U32 u32Data, HI_U32 u32Enable)
{
    U_WBC_FI_CTRL   WBC_FI_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_FI_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_FI_CTRL.bits.wbc_c_en = u32Enable;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CTRL.u32)) + u32Data * PQ_VID_OFFSET), WBC_FI_CTRL.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrWbcRegUp(HI_U32 u32Data)
{
    U_WBC_FI_UPD   WBC_FI_UPD;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_FI_UPD.bits.regup = 1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_UPD.u32)) + u32Data * PQ_VID_OFFSET), WBC_FI_UPD.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr, HI_U32 u32CAddr)
{
    U_WBC_FI_YADDR   WBC_FI_YADDR;
    U_WBC_FI_CADDR   WBC_FI_CADDR;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_FI_YADDR.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_YADDR.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_FI_CADDR.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CADDR.u32)) + u32Data * PQ_VID_OFFSET));

    WBC_FI_YADDR.bits.wbcaddr = u32YAddr;
    WBC_FI_CADDR.bits.wbcaddr = u32CAddr;

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_YADDR.u32)) + u32Data * PQ_VID_OFFSET), WBC_FI_YADDR.u32);
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CADDR.u32)) + u32Data * PQ_VID_OFFSET), WBC_FI_CADDR.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrWbcStride(HI_U32 u32Data, HI_U32 u32YStride, HI_U32 u32CStride)
{
    U_WBC_FI_STRIDE   WBC_FI_STRIDE;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_FI_STRIDE.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_STRIDE.u32)) + u32Data * PQ_VID_OFFSET));

    WBC_FI_STRIDE.bits.wbclstride = u32YStride;
    WBC_FI_STRIDE.bits.wbccstride = u32CStride;

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_STRIDE.u32)) + u32Data * PQ_VID_OFFSET), WBC_FI_STRIDE.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetMadWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr)
{
    U_WBC_ME_YADDR   WBC_ME_YADDR;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_ME_YADDR.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_YADDR.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_ME_YADDR.bits.wbcaddr = u32YAddr;

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_YADDR.u32)) + u32Data * PQ_VID_OFFSET), WBC_ME_YADDR.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetMadWbcStride(HI_U32 u32Data, HI_U32 u32YStride)
{
    U_WBC_ME_STRIDE   WBC_ME_STRIDE;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_ME_STRIDE.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_STRIDE.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_ME_STRIDE.bits.wbclstride = u32YStride;

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_STRIDE.u32)) + u32Data * PQ_VID_OFFSET), WBC_ME_STRIDE.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrCEnable(HI_U32 u32Data, HI_U32 tnr_c_en)
{
    U_V0_PRERD   V0_PRERD;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_PRERD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VID_OFFSET));

    V0_PRERD.bits.tnr_c_en = tnr_c_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VID_OFFSET), V0_PRERD.u32);

    return HI_SUCCESS;
}

///ref data_width
//
HI_S32 PQ_HAL_SetTnrRefDataWidth(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_VPSSIP_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    V0_VPSSIP_CTRL.bits.data_width = u32Data1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET), V0_VPSSIP_CTRL.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrRefReqCtrl(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_VPSSIP_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    V0_VPSSIP_CTRL.bits.ref_req_ctrl = u32Data1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET), V0_VPSSIP_CTRL.u32);
    return HI_SUCCESS;
}
//mad
HI_S32 PQ_HAL_SetTnrMadDbmReqCtrl(HI_U32 u32Data, HI_U32 u32Data1)
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_VPSSIP_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    V0_VPSSIP_CTRL.bits.mad_dbm_req_ctrl = u32Data1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET), V0_VPSSIP_CTRL.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrMadDataMode(HI_U32 u32Data, HI_U32 mad_data_mode)
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_VPSSIP_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    V0_VPSSIP_CTRL.bits.mad_data_mode = mad_data_mode;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET), V0_VPSSIP_CTRL.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrWbcDataWidth(HI_U32 u32Data, HI_U32 u32DataWidth)
{
    U_WBC_FI_CTRL   WBC_FI_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_FI_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_FI_CTRL.bits.data_width = u32DataWidth;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CTRL.u32)) + u32Data * PQ_VID_OFFSET), WBC_FI_CTRL.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetMadWbcDataMode(HI_U32 u32Data, HI_U32 mad_data_mode)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_WBC_ME_CTRL   WBC_ME_CTRL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_ME_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_ME_CTRL.bits.mad_data_mode = mad_data_mode;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_CTRL.u32)) + u32Data * PQ_VID_OFFSET), WBC_ME_CTRL.u32);

    return HI_SUCCESS;
}

//REF DCMP
HI_S32  PQ_HAL_SetRefDcmpEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_V0_VPSSIP_CTRL V0_VPSSIP_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_VPSSIP_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    V0_VPSSIP_CTRL.bits.dcmp_en = u32bEnable ;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_VPSSIP_CTRL.u32)) + u32Data * PQ_VID_OFFSET), V0_VPSSIP_CTRL.u32);

    return HI_SUCCESS;
}

//WBC REF CMP
HI_S32  PQ_HAL_SetTnrWbcCmpEnable    (HI_U32 u32Data, HI_U32 u32bEnable )
{
    U_WBC_FI_CTRL   WBC_FI_CTRL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_FI_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_FI_CTRL.bits.cmp_en = u32bEnable ;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_FI_CTRL.u32)) + u32Data * PQ_VID_OFFSET), WBC_FI_CTRL.u32);

    return HI_SUCCESS;
}

HI_S32  PQ_HAL_SetTnrPreAddr   (HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32CAddr, HI_U32 u32LStr, HI_U32 u32CStr)
{
    U_V0_P1STRIDE          V0_P1STRIDE;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_P1LADDR.u32)) + u32Data * PQ_VID_OFFSET), u32LAddr);
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_P1CADDR.u32)) + u32Data * PQ_VID_OFFSET), u32CAddr);

    V0_P1STRIDE.bits.surface_stride = u32LStr;
    V0_P1STRIDE.bits.surface_cstride = u32CStr;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_P1STRIDE.u32)) + u32Data * PQ_VID_OFFSET), V0_P1STRIDE.u32);

    return HI_SUCCESS;
}

HI_S32  PQ_HAL_SetTnrMadAddr   (HI_U32 u32Data, HI_U32 u32LAddr, HI_U32 u32LStr)
{
    U_V0_P2STRIDE          V0_P2STRIDE;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_P2LADDR.u32)) + u32Data * PQ_VID_OFFSET), u32LAddr);

    V0_P2STRIDE.bits.surface_stride = u32LStr;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_P2STRIDE.u32)) + u32Data * PQ_VID_OFFSET), V0_P2STRIDE.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrCfgAddr(HI_U32 u32Data, HI_U32 u32Addr)
{
    U_V0_TNR_CFGAD         V0_TNR_CFGAD;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();


    V0_TNR_CFGAD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_TNR_CFGAD.u32)) + u32Data * PQ_VID_OFFSET));
    V0_TNR_CFGAD.bits.coef_addr = u32Addr;

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_TNR_CFGAD.u32)) + u32Data * PQ_VID_OFFSET), V0_TNR_CFGAD.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrLutAddr(HI_U32 u32Data, HI_U32 u32Addr)
{
    U_V0_TNR_LUTAD         V0_TNR_LUTAD; /* 0x81c */
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }
    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_TNR_LUTAD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_TNR_LUTAD.u32)) + u32Data * PQ_VID_OFFSET));
    V0_TNR_LUTAD.bits.coef_addr = u32Addr;

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_TNR_LUTAD.u32)) + u32Data * PQ_VID_OFFSET), V0_TNR_LUTAD.u32);
    return HI_SUCCESS;
}

HI_S32  PQ_HAL_SetVdpTnrParaUpd (HI_U32 u32Data)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_PARAUP V0_PARAUP;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    V0_PARAUP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PARAUP.u32)) + u32Data * PQ_VP_OFFSET));
    V0_PARAUP.bits.tnr_upd = 0x1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PARAUP.u32)) + u32Data * PQ_VP_OFFSET), V0_PARAUP.u32);
    return HI_SUCCESS;
}

HI_S32  PQ_HAL_SetVdpTnrLutUpd (HI_U32 u32Data)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_PARAUP V0_PARAUP;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    V0_PARAUP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PARAUP.u32)) + u32Data * PQ_VP_OFFSET));
    V0_PARAUP.bits.tnr_lut_upd = 0x1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PARAUP.u32)) + u32Data * PQ_VP_OFFSET), V0_PARAUP.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_InitVdpTnr(HI_VOID)
{
    HI_U32 u32HandleNo = 0;
    HI_U32 u32TNRAddrOffset = 0;
    HI_U32 u32TNRParaSize   = 0;
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);

    u32TNRParaSize   = VDP_PQ_REG_SIZE(V0_TNR_MODE, V0_TNR_CLUT10);
    u32TNRAddrOffset = VDP_PQ_REG_SIZE(VOCTRL, V0_TNR_MODE) - sizeof(HI_U32);


    PQ_HAL_CopyRegBySize((HI_VOID *)(g_stIPSelAlgBuffer.pu8StartVirAddr + u32TNRAddrOffset),
                         (HI_VOID *)(&pstVirReg->VPSS_TNR_MODE), u32TNRParaSize / 4);
    PQ_HAL_SetTnrCfgAddr(PQ_VDP_LAYER_VID0, g_stIPSelAlgBuffer.u32StartPhyAddr + u32TNRAddrOffset);

    u32TNRParaSize   = VDP_PQ_REG_SIZE(V0_TNR_CLUT10, V0_TNR_CLUT67);
    u32TNRAddrOffset = VDP_PQ_REG_SIZE(VOCTRL, V0_TNR_CLUT10) - sizeof(HI_U32);

    PQ_HAL_CopyRegBySize((HI_VOID *)(g_stIPSelAlgBuffer.pu8StartVirAddr + u32TNRAddrOffset),
                         (HI_VOID *)(&pstVirReg->VPSS_TNR_CLUT10), u32TNRParaSize / 4);
    PQ_HAL_SetTnrLutAddr(PQ_VDP_LAYER_VID0, g_stIPSelAlgBuffer.u32StartPhyAddr + u32TNRAddrOffset);

    PQ_HAL_SetVdpTnrParaUpd(PQ_VDP_LAYER_VID0);
    return PQ_HAL_SetVdpTnrLutUpd(PQ_VDP_LAYER_VID0);
}


HI_VOID PQ_HAL_SetSmmuLBypass(PQ_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_WBC_DHD0_SMMU_BYPASS  WBC_DHD0_SMMU_BYPASS ;

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_DHD0_SMMU_BYPASS.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_DHD0_SMMU_BYPASS.u32)) + enLayer * PQ_WBC_OFFSET));
    WBC_DHD0_SMMU_BYPASS.bits.l_bypass = u32Enable;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_DHD0_SMMU_BYPASS.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_SMMU_BYPASS.u32);
}

HI_VOID PQ_HAL_SetSmmuCBypass(PQ_LAYER_WBC_E enLayer, HI_U32 u32Enable)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_WBC_DHD0_SMMU_BYPASS  WBC_DHD0_SMMU_BYPASS ;

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_DHD0_SMMU_BYPASS.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_DHD0_SMMU_BYPASS.u32)) + enLayer * PQ_WBC_OFFSET));
    WBC_DHD0_SMMU_BYPASS.bits.c_bypass = u32Enable;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_DHD0_SMMU_BYPASS.u32)) + enLayer * PQ_WBC_OFFSET), WBC_DHD0_SMMU_BYPASS.u32);
}

HI_VOID PQ_HAL_Set16RegionSmmuLumBypass (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32Bypass)
{
    U_V0_SMMU_BYPASS0 V0_SMMU_BYPASS0;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return ;
    }

    if (u32Num >= VID_REGION_NUM)
    {
        return ;
    }

    u32Num = u32Num * 2;

    pstVDPReg = PQ_HAL_GetVdpReg();
    V0_SMMU_BYPASS0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_SMMU_BYPASS0.u32)) + u32Data * PQ_VID_OFFSET));
    if (u32Bypass)
    {
        V0_SMMU_BYPASS0.u32 = (1 << u32Num) | V0_SMMU_BYPASS0.u32;
    }
    else
    {
        V0_SMMU_BYPASS0.u32 = (~(1 << u32Num)) & V0_SMMU_BYPASS0.u32;
    }

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_SMMU_BYPASS0.u32)) + u32Data * PQ_VID_OFFSET), V0_SMMU_BYPASS0.u32);

    return ;
}

HI_VOID PQ_HAL_Set16RegionSmmuChromBypass (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32Bypass)
{
    U_V0_SMMU_BYPASS0 V0_SMMU_BYPASS0;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (u32Data >= VID_MAX)
    {
        return ;
    }

    if (u32Num >= VID_REGION_NUM)
    {
        return ;
    }

    u32Num = u32Num * 2 + 1;

    pstVDPReg = PQ_HAL_GetVdpReg();
    V0_SMMU_BYPASS0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_SMMU_BYPASS0.u32)) + u32Data * PQ_VID_OFFSET));
    if (u32Bypass)
    {
        V0_SMMU_BYPASS0.u32 = (1 << u32Num) | V0_SMMU_BYPASS0.u32;
    }
    else
    {
        V0_SMMU_BYPASS0.u32 = (~(1 << u32Num)) & V0_SMMU_BYPASS0.u32;
    }

    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_SMMU_BYPASS0.u32)) + u32Data * PQ_VID_OFFSET), V0_SMMU_BYPASS0.u32);

    return ;
}

/*DB*/
HI_S32  PQ_HAL_SetVdpDbmParaUpd (HI_U32 u32Data)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_PARAUP V0_PARAUP;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    V0_PARAUP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PARAUP.u32)) + u32Data * PQ_VP_OFFSET));
    V0_PARAUP.bits.dbm_upd = 0x1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PARAUP.u32)) + u32Data * PQ_VP_OFFSET), V0_PARAUP.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetVdpDbmEnable(HI_U32 u32Data, HI_BOOL bEn)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_PRERD V0_PRERD;
    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    V0_PRERD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VP_OFFSET));
    V0_PRERD.bits.dbm_en = bEn;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VP_OFFSET), V0_PRERD.u32);
    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_SetVdpDbmCfgAddr(HI_U32 u32Data, HI_U32 u32Addr)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_DBM_CFGAD         V0_DBM_CFGAD;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_DBM_CFGAD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_DBM_CFGAD.u32)) + u32Data * PQ_VP_OFFSET));
    V0_DBM_CFGAD.bits.coef_addr = u32Addr;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_DBM_CFGAD.u32)) + u32Data * PQ_VP_OFFSET), V0_DBM_CFGAD.u32);
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_InitVdpDbm(HI_VOID)
{
    HI_U32 u32Vid      = 0;
    HI_U32 u32HandleNo = 0;
    HI_U32 u32DbAddrOffset = 0;
    HI_U32 u32DbParaSize   = 0;
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);

    u32DbParaSize   = VDP_PQ_REG_SIZE(V0_DB_CTRL, V0_DB_BORD_FLAG);
    u32DbAddrOffset = VDP_PQ_REG_SIZE(VOCTRL, V0_DB_CTRL) - sizeof(HI_U32);

    PQ_HAL_CopyRegBySize((HI_VOID *)(g_stIPSelAlgBuffer.pu8StartVirAddr + u32DbAddrOffset),
                         (HI_VOID *)(&pstVirReg->VPSS_DB_CTRL), (u32DbParaSize + 128 * 4) / 4);

    return PQ_HAL_SetVdpDbmCfgAddr(u32Vid, g_stIPSelAlgBuffer.u32StartPhyAddr + u32DbAddrOffset);
}


HI_S32 PQ_HAL_SetSttWbcAddr(HI_U32 u32Data, HI_U32 u32YAddr)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_WBC_ME_CADDR WBC_ME_CADDR;
    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    WBC_ME_CADDR.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_CADDR.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_ME_CADDR.bits.wbccaddr = u32YAddr;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_CADDR.u32)) + u32Data * PQ_VID_OFFSET), WBC_ME_CADDR.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetMadWbcRegUp(HI_U32 u32Data)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_WBC_ME_UPD   WBC_ME_UPD;
    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_ME_UPD.bits.regup = 1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_UPD.u32)) + u32Data * PQ_VID_OFFSET), WBC_ME_UPD.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetMadWbcEnable(HI_U32 u32Data, HI_U32 u32Enable)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_WBC_ME_CTRL WBC_ME_CTRL;
    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_ME_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_CTRL.u32)) + u32Data * PQ_VID_OFFSET));
    WBC_ME_CTRL.bits.wbc_en = u32Enable;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->WBC_ME_CTRL.u32)) + u32Data * PQ_VID_OFFSET), WBC_ME_CTRL.u32);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDbmDetEnable(HI_U32 u32Data, HI_U32 u32Data1)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_PRERD V0_PRERD;
    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_PRERD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VID_OFFSET));
    V0_PRERD.bits.dbm_det_en = u32Data1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VID_OFFSET), V0_PRERD.u32);
    return HI_SUCCESS;
}

HI_S32  PQ_HAL_SetDbmInfoReso(HI_U32 u32Data, HI_U32 u32InfoWidth, HI_U32 u32InfoHeight)
{
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_DBM_RESO    V0_DBM_RESO;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_DBM_RESO.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_DBM_RESO.u32)) + u32Data * PQ_VID_OFFSET));
    V0_DBM_RESO.bits.info_width   = u32InfoWidth - 1;
    V0_DBM_RESO.bits.info_height  = u32InfoHeight - 1;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_DBM_RESO.u32)) + u32Data * PQ_VID_OFFSET), V0_DBM_RESO.u32);

    return HI_SUCCESS;
}

HI_S32  PQ_HAL_SetDbmInfoEnable(HI_U32 u32Data, HI_U32  dbm_info_en)
{
    S_VDP_REGS_TYPE  *pstVDPReg = HI_NULL;
    U_V0_PRERD        V0_PRERD;

    if (u32Data >= VID_MAX)
    {
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    V0_PRERD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VID_OFFSET));
    V0_PRERD.bits.dbm_info_en   = dbm_info_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pstVDPReg->V0_PRERD.u32)) + u32Data * PQ_VID_OFFSET), V0_PRERD.u32);

    return HI_SUCCESS;
}

