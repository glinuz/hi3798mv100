#include "pq_hal_comm.h"
#include "pq_hal_dering.h"
#include "drv_pq_table.h"


HI_S32 PQ_HAL_EnableDeringDemo(HI_U32 u32HandleNo, HI_BOOL DeringDemoEn)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DR))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_CTRL.bits.dbm_demo_en = DeringDemoEn;
    }
    else
    {
        pstVDPReg = PQ_HAL_GetVdpReg();
        pstVDPReg->V0_DB_CTRL.bits.dbm_demo_en = DeringDemoEn;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDeringDemoMode(HI_U32 u32HandleNo, DR_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DR))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_CTRL.bits.dbm_demo_mode = enMode;
    }
    else
    {
        pstVDPReg = PQ_HAL_GetVdpReg();
        pstVDPReg->V0_DB_CTRL.bits.dbm_demo_mode = enMode;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableDering(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;
    S_VDP_REGS_TYPE* pstVdpReg  = HI_NULL;
    U_V0_DB_CTRL     V0_DB_CTRL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_DB_CTRL.bits.dr_en = bOnOff;
    }
    else
    {
        pstVdpReg = PQ_HAL_GetVdpReg();
        V0_DB_CTRL.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&pstVdpReg->V0_DB_CTRL.u32)));
        V0_DB_CTRL.bits.dr_en = bOnOff;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pstVdpReg->V0_DB_CTRL.u32))), V0_DB_CTRL.u32);
    }

    return HI_SUCCESS;
}

