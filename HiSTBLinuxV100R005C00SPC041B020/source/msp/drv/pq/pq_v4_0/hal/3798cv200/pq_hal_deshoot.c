#include "pq_hal_comm.h"
#include "pq_hal_deshoot.h"
#include "pq_mng_deshoot.h"
#include "drv_pq_table.h"


HI_S32 PQ_HAL_EnableDeshootDemo(HI_U32 u32HandleNo, HI_BOOL DSDemoEn)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DS))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_CTRL.bits.dbm_demo_en = DSDemoEn;
    }
    else
    {
        pstVDPReg = PQ_HAL_GetVdpReg();
        pstVDPReg->V0_DB_CTRL.bits.dbm_demo_en = DSDemoEn;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDeshootDemoMode(HI_U32 u32HandleNo, DS_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DS))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_CTRL.bits.dbm_demo_mode = enMode;
    }
    else
    {
        pstVDPReg = PQ_HAL_GetVdpReg();
        pstVDPReg->V0_DB_CTRL.bits.dbm_demo_en = enMode;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDeshootThr(HI_U32 u32HandleNo, HI_U32 u32Width)
{
    HI_U32 data1 = 0, data3 = 0;
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DS))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        data3 = (u32Width < pstVpssVirReg->VPSS_DS_THR13.bits.ds_picresofst) ? 0 : u32Width - pstVpssVirReg->VPSS_DS_THR13.bits.ds_picresofst ;

        //data0 = (u32Width < pstVpssVirReg->VPSS_DS_THR13.bits.ds_picresofst) ? 1 :
        //        (((pstVpssVirReg->VPSS_DS_THR12.bits.ds_picresscl2 * data3 + 2048) >> 12) + 1);
        data1 = (u32Width < pstVpssVirReg->VPSS_DS_THR13.bits.ds_picresofst) ? 0 : \
                ((pstVpssVirReg->VPSS_DS_THR12.bits.ds_picresscl0 * data3 + 2048) >> 12);

        pstVpssVirReg->VPSS_DS_THR16.bits.ds_sclctrst = (pstVpssVirReg->VPSS_DS_THR16.bits.ds_resadpten == 1) ? data1 : 1;
        pstVpssVirReg->VPSS_DS_THR16.bits.ds_patcond6ofst = (pstVpssVirReg->VPSS_DS_THR16.bits.ds_resadpten == 1) ? \
                (1664 + ((pstVpssVirReg->VPSS_DS_THR14.bits.ds_patcond6normofst * data3) >> 12)) : 1664;
        pstVpssVirReg->VPSS_DS_THR18.bits.ds_cormmthr1 = (pstVpssVirReg->VPSS_DS_THR16.bits.ds_resadpten == 1) ? data1 : 1;
        pstVpssVirReg->VPSS_DS_THR18.bits.ds_cormmthr0 = (pstVpssVirReg->VPSS_DS_THR16.bits.ds_resadpten == 1) ? data1 : 1;
        pstVpssVirReg->VPSS_DS_THR19.bits.ds_patcond6ofst5  = (pstVpssVirReg->VPSS_DS_THR15.bits.ds_patcond6norm5 * data3) >> 12;
        pstVpssVirReg->VPSS_DS_THR19.bits.ds_patcond6ofst6  = (pstVpssVirReg->VPSS_DS_THR15.bits.ds_patcond6norm6 * data3) >> 12;
        pstVpssVirReg->VPSS_DS_THR19.bits.ds_patcond6ofst61 = (pstVpssVirReg->VPSS_DS_THR14.bits.ds_patcond6norm61 * data3) >> 12;
        pstVpssVirReg->VPSS_DS_THR20.bits.ds_patcond6ofst62 = (pstVpssVirReg->VPSS_DS_THR14.bits.ds_patcond6norm62 * data3) >> 12;
        pstVpssVirReg->VPSS_DS_THR20.bits.ds_patcond6ofst63 = (pstVpssVirReg->VPSS_DS_THR14.bits.ds_patcond6norm63 * data3) >> 12;
    }
    else
    {
        pstVDPReg = PQ_HAL_GetVdpReg();
        data3 = (u32Width < pstVDPReg->V0_DS_THR13.bits.ds_picresofst) ? 0 : u32Width - pstVDPReg->V0_DS_THR13.bits.ds_picresofst ;

        //data0 = (u32Width < pstVDPReg->V0_DS_THR13.bits.ds_picresofst) ? 1 :
        //        (((pstVDPReg->V0_DS_THR12.bits.ds_picresscl2 * data3 + 2048) >> 12) + 1);
        data1 = (u32Width < pstVDPReg->V0_DS_THR13.bits.ds_picresofst) ? 0 : \
                ((pstVDPReg->V0_DS_THR12.bits.ds_picresscl0 * data3 + 2048) >> 12);

        pstVDPReg->V0_DS_THR16.bits.ds_sclctrst = (pstVDPReg->V0_DS_THR16.bits.ds_resadpten == 1) ? data1 : 1;
        pstVDPReg->V0_DS_THR16.bits.ds_patcond6ofst = (pstVDPReg->V0_DS_THR16.bits.ds_resadpten == 1) ? \
                (1664 + ((pstVDPReg->V0_DS_THR14.bits.ds_patcond6normofst * data3) >> 12)) : 1664;
        pstVDPReg->V0_DS_THR18.bits.ds_cormmthr1 = (pstVDPReg->V0_DS_THR16.bits.ds_resadpten == 1) ? data1 : 1;
        pstVDPReg->V0_DS_THR18.bits.ds_cormmthr0 = (pstVDPReg->V0_DS_THR16.bits.ds_resadpten == 1) ? data1 : 1;
        pstVDPReg->V0_DS_THR19.bits.ds_patcond6ofst5  = (pstVDPReg->V0_DS_THR15.bits.ds_patcond6norm5 * data3) >> 12;
        pstVDPReg->V0_DS_THR19.bits.ds_patcond6ofst6  = (pstVDPReg->V0_DS_THR15.bits.ds_patcond6norm6 * data3) >> 12;
        pstVDPReg->V0_DS_THR19.bits.ds_patcond6ofst61 = (pstVDPReg->V0_DS_THR14.bits.ds_patcond6norm61 * data3) >> 12;
        pstVDPReg->V0_DS_THR20.bits.ds_patcond6ofst62 = (pstVDPReg->V0_DS_THR14.bits.ds_patcond6norm62 * data3) >> 12;
        pstVDPReg->V0_DS_THR20.bits.ds_patcond6ofst63 = (pstVDPReg->V0_DS_THR14.bits.ds_patcond6norm63 * data3) >> 12;
    }

    return HI_SUCCESS;

}

HI_S32 PQ_HAL_EnableDeshoot(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;
    S_VDP_REGS_TYPE *pstVdpReg  = HI_NULL;
    U_V0_DB_CTRL     V0_DB_CTRL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_DB_CTRL.bits.ds_en = bOnOff;
    }
    else
    {
        pstVdpReg = PQ_HAL_GetVdpReg();
        V0_DB_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&pstVdpReg->V0_DB_CTRL.u32)));
        V0_DB_CTRL.bits.ds_en = bOnOff;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pstVdpReg->V0_DB_CTRL.u32))), V0_DB_CTRL.u32);
    }

    return HI_SUCCESS;
}

