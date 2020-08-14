/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_gfxzme.c
  Version       : Initial Draft
  Author        : pengjunwei 00203646
  Created       : 2015/10/15
  Description   :

******************************************************************************/

#include "pq_hal_gfxzme.h"


static S_VDP_REGS_TYPE *pVdpReg;


HI_VOID PQ_HAL_SetGZmeEnable  (HI_U32 u32Data, PQ_GZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP GP0_ZME_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_SetGZmeEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /*GP zoom enable */
    if ((enMode == PQ_GZME_MODE_HOR) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_HSP.bits.hsc_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if ((enMode == PQ_GZME_MODE_VER) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_VSP.bits.vsc_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}


HI_VOID PQ_HAL_SetGZmePhase   (HI_U32 u32Data, PQ_GZME_MODE_E enMode, HI_S32 s32Phase)
{
    volatile U_GP0_ZME_HLOFFSET  GP0_ZME_HLOFFSET;
    volatile U_GP0_ZME_HCOFFSET  GP0_ZME_HCOFFSET;
    volatile U_GP0_ZME_VOFFSET   GP0_ZME_VOFFSET;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_SetGZmePhase() Select Wrong Video Layer ID\n");
        return ;
    }

    if ((enMode == PQ_GZME_MODE_HORL) || (enMode == PQ_GZME_MODE_HOR) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_HLOFFSET.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HLOFFSET.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_HLOFFSET.bits.hor_loffset = s32Phase;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HLOFFSET.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HLOFFSET.u32);
    }

    if ((enMode == PQ_GZME_MODE_HORC) || (enMode == PQ_GZME_MODE_HOR) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_HCOFFSET.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HCOFFSET.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_HCOFFSET.bits.hor_coffset = s32Phase;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HCOFFSET.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HCOFFSET.u32);
    }

    if ((enMode == PQ_GZME_MODE_VERL) || (enMode == PQ_GZME_MODE_VER) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VOFFSET.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_VOFFSET.bits.vbtm_offset = s32Phase;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VOFFSET.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VOFFSET.u32);
    }

    if ((enMode == PQ_GZME_MODE_VERC) || (enMode == PQ_GZME_MODE_VER) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_VOFFSET.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VOFFSET.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_VOFFSET.bits.vtp_offset = s32Phase;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VOFFSET.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VOFFSET.u32);
    }

    return ;
}

HI_VOID PQ_HAL_SetGZmeFirEnable   (HI_U32 u32Data, PQ_GZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP        GP0_ZME_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_SetGZmeFirEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    if ((enMode == PQ_GZME_MODE_ALPHA) || (enMode == PQ_GZME_MODE_HOR) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_HSP.bits.hafir_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if ((enMode == PQ_GZME_MODE_HOR) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_HSP.bits.hfir_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if ((enMode == PQ_GZME_MODE_ALPHAV) || (enMode == PQ_GZME_MODE_VER) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_VSP.bits.vafir_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    if ((enMode == PQ_GZME_MODE_VER) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_VSP.bits.vfir_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}

HI_VOID PQ_HAL_SetGZmeMidEnable   (HI_U32 u32Data, PQ_GZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP        GP0_ZME_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_SetGZmeMidEnable() Select Wrong Video Layer ID\n");
        return ;
    }

    /* VHD layer zoom enable */
    if ((enMode == PQ_GZME_MODE_ALPHA) || (enMode == PQ_GZME_MODE_HOR) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_HSP.bits.hamid_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HSP.u32);
    }
    if ((enMode == PQ_GZME_MODE_HORL) || (enMode == PQ_GZME_MODE_HOR) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_HSP.bits.hlmid_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if ((enMode == PQ_GZME_MODE_HORC) || (enMode == PQ_GZME_MODE_HOR) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_HSP.bits.hchmid_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if ((enMode == PQ_GZME_MODE_ALPHAV) || (enMode == PQ_GZME_MODE_VER) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_VSP.bits.vamid_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VSP.u32);
    }
    if ((enMode == PQ_GZME_MODE_VERL) || (enMode == PQ_GZME_MODE_VER) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_VSP.bits.vlmid_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    if ((enMode == PQ_GZME_MODE_VERC) || (enMode == PQ_GZME_MODE_VER) || (enMode == PQ_GZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_ZME_VSP.bits.vchmid_en = u32bEnable;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}


HI_VOID PQ_HAL_SetGZmeHorRatio  (HI_U32 u32Data, HI_U32 u32Ratio)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error,PQ_HAL_SetGZmeHorRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET));
    GP0_ZME_HSP.bits.hratio = u32Ratio;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HSP.u32);

    return ;
}

HI_VOID PQ_HAL_SetGZmeVerRatio  (HI_U32 u32Data, HI_U32 u32Ratio)
{
    volatile U_GP0_ZME_VSR        GP0_ZME_VSR;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_SetGZmeVerRatio() Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_VSR.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSR.u32)) + u32Data * PQ_GP_OFFSET));
    GP0_ZME_VSR.bits.vratio = u32Ratio;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSR.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VSR.u32);

    return ;
}



HI_VOID PQ_HAL_SetGZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        return ;
    }

    GP0_ZME_HSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET));
    GP0_ZME_HSP.bits.hfir_order = u32HfirOrder;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_HSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_HSP.u32);

    return ;
}

HI_VOID PQ_HAL_SetGZmeCoefAddr  (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr)
{
    volatile U_GP0_HCOEFAD    GP0_HCOEFAD;
    volatile U_GP0_VCOEFAD    GP0_VCOEFAD;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_SetGZmeCoefAddr() Select Wrong Video Layer ID\n");
        return ;
    }

    if (u32Mode == PQ_GP_PARA_ZME_HOR)
    {
        GP0_HCOEFAD.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_HCOEFAD.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_HCOEFAD.bits.coef_addr = u32Addr;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_HCOEFAD.u32)) + u32Data * PQ_GP_OFFSET), GP0_HCOEFAD.u32);
    }
    else if (u32Mode == PQ_GP_PARA_ZME_VER)
    {
        GP0_VCOEFAD.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_VCOEFAD.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_VCOEFAD.bits.coef_addr = u32Addr;
        PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_VCOEFAD.u32)) + u32Data * PQ_GP_OFFSET), GP0_VCOEFAD.u32);
    }
    else
    {
        HI_ERR_PQ("PQ_HAL_SetGZmeCoefAddr() Select a Wrong Mode!\n");
    }

    return ;
}

/***************************************************************************************
* func          : PQ_HAL_SetGZmeTabV
* description   : CNcomment: 设置ZME阶数，用来做微缩使用 HIFONE B02新增 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID PQ_HAL_SetGZmeTabV(HI_U32 u32Data, HI_U32 u32ZmeTabV)
{
    volatile U_GP0_ZME_VSP   GP0_ZME_VSP;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        return ;
    }
    GP0_ZME_VSP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET));
    GP0_ZME_VSP.bits.vfir_2tap_en = u32ZmeTabV;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_ZME_VSP.u32)) + u32Data * PQ_GP_OFFSET), GP0_ZME_VSP.u32);

    return ;
}

/***************************************************************************************
* func          : OPTM_VDP_GP_SetIpOrder
* description   : CNcomment: 设置GP CTRL CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID PQ_HAL_SetGZmeIpOrder (HI_U32 u32Data, HI_BOOL bSlvGp, PQ_GP_ORDER_E enIpOrder)
{
    volatile U_GP0_CTRL GP0_CTRL ;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_SetGZmeIpOrder() Select Wrong GP Layer ID\n");
        return ;
    }

    GP0_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_CTRL.u32)) + u32Data * PQ_GP_OFFSET));

    if (HI_FALSE == bSlvGp) //channel A
    {
        if (u32Data == PQ_GZME_GP_1)
        {
            //for GP0
            GP0_CTRL.bits.ffc_sel = 0;
        }
        switch (enIpOrder)
        {
#if 0/** deal with codecc **/
            case PQ_GP_ORDER_NULL:
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }

            case PQ_GP_ORDER_CSC:
            {
                GP0_CTRL.bits.mux1_sel = 0;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 2;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            case PQ_GP_ORDER_ZME:
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 0;
                GP0_CTRL.bits.aout_sel = 3;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            case PQ_GP_ORDER_CSC_ZME:
            {
                GP0_CTRL.bits.mux1_sel = 0;
                GP0_CTRL.bits.mux2_sel = 2;
                GP0_CTRL.bits.aout_sel = 3;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
#endif
            case PQ_GP_ORDER_ZME_CSC:
            {
                GP0_CTRL.bits.mux1_sel = 3;
                GP0_CTRL.bits.mux2_sel = 0;
                GP0_CTRL.bits.aout_sel = 2;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
            default:
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 1;

                break;
            }
        }
    }
    else if (bSlvGp == HI_TRUE) //channel B
    {
        if (u32Data == PQ_GZME_GP_1)
        {
            /* for WBC_GP0 */
            GP0_CTRL.bits.ffc_sel = 1;
        }
        switch (enIpOrder)
        {
#if 0/** deal with codecc **/
            case PQ_GP_ORDER_NULL:
            {
                GP0_CTRL.bits.mux1_sel = 1;
                GP0_CTRL.bits.mux2_sel = 0;
                GP0_CTRL.bits.aout_sel = 3;
                GP0_CTRL.bits.bout_sel = 2;

                break;
            }

            case PQ_GP_ORDER_CSC:
            {
                GP0_CTRL.bits.mux1_sel = 1;
                GP0_CTRL.bits.mux2_sel = 3;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 2;

                break;
            }
            case PQ_GP_ORDER_ZME:
            {
                GP0_CTRL.bits.mux1_sel = 2;
                GP0_CTRL.bits.mux2_sel = 1;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 3;

                break;
            }
            case PQ_GP_ORDER_CSC_ZME:
            {
                GP0_CTRL.bits.mux1_sel = 1;
                GP0_CTRL.bits.mux2_sel = 2;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 3;

                break;
            }
#endif
            case PQ_GP_ORDER_ZME_CSC:
            {
                GP0_CTRL.bits.mux1_sel = 3;
                GP0_CTRL.bits.mux2_sel = 1;
                GP0_CTRL.bits.aout_sel = 0;
                GP0_CTRL.bits.bout_sel = 2;

                break;
            }
            default://null
            {
                GP0_CTRL.bits.mux1_sel = 1;
                GP0_CTRL.bits.mux2_sel = 0;
                GP0_CTRL.bits.aout_sel = 3;
                GP0_CTRL.bits.bout_sel = 2;

                break;
            }
        }
    }
    else
    {
        HI_ERR_PQ("PQ_HAL_SetGZmeIpOrder() Select Wrong GP Channel\n");
    }

    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GP0_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GP0_CTRL.u32);
}


