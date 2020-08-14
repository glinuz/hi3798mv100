/******************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_csc.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/10/15
  Description   :

******************************************************************************/

#include "pq_hal_csc.h"
#include "pq_hal_comm.h"


/**
 \brief 打开CSC模块
 \attention \n
无

 \param[in] enLayer
 \param[in] bOnOff

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_HAL_EnableCSC(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_CSC0_IDC VP0_CSC0_IDC;
    U_VP0_CSC1_IDC VP0_CSC1_IDC;
    U_V0_CSC_IDC V0_CSC_IDC;

    if (enLayer >= HAL_DISP_LAYER_BUTT)
    {
        HI_ERR_PQ("Error, Select Wrong VP Layer ID\n");
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();

    if (enLayer == HAL_DISP_LAYER_V0)
    {
        VP0_CSC0_IDC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_IDC.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_IDC.bits.csc_en = bCscEn;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_IDC.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_IDC.u32);
    }
    else if (enLayer == HAL_DISP_LAYER_V1)
    {
        VP0_CSC1_IDC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_IDC.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_IDC.bits.csc_en = bCscEn;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_IDC.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_IDC.u32);
    }
    else if ((enLayer >= HAL_DISP_LAYER_V2) && (enLayer < HAL_DISP_LAYER_BUTT))
    {
        V0_CSC_IDC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_IDC.u32) + enLayer * VDP_VID_OFFSET);
        V0_CSC_IDC.bits.csc_en = bCscEn;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_IDC.u32) + enLayer * VDP_VID_OFFSET), V0_CSC_IDC.u32);
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetCscCoef(HAL_DISP_LAYER_E enChan, CSC_COEF_S* pstCscCoef)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_CSC0_P0 VP0_CSC0_P0;
    U_VP0_CSC0_P1 VP0_CSC0_P1;
    U_VP0_CSC0_P2 VP0_CSC0_P2;
    U_VP0_CSC0_P3 VP0_CSC0_P3;
    U_VP0_CSC0_P4 VP0_CSC0_P4;
    U_VP0_CSC1_P0 VP0_CSC1_P0;
    U_VP0_CSC1_P1 VP0_CSC1_P1;
    U_VP0_CSC1_P2 VP0_CSC1_P2;
    U_VP0_CSC1_P3 VP0_CSC1_P3;
    U_VP0_CSC1_P4 VP0_CSC1_P4;
    U_V0_CSC_P0 V0_CSC_P0;
    U_V0_CSC_P1 V0_CSC_P1;
    U_V0_CSC_P2 V0_CSC_P2;
    U_V0_CSC_P3 V0_CSC_P3;
    U_V0_CSC_P4 V0_CSC_P4;

    if (enChan >= HAL_DISP_LAYER_BUTT)
    {
        HI_ERR_PQ("Error,Select Wrong VP layer!\n");
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    if (enChan == HAL_DISP_LAYER_V0)
    {
        VP0_CSC0_P0.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_P0.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_P0.bits.cscp00 = pstCscCoef->csc_coef00;
        VP0_CSC0_P0.bits.cscp01 = pstCscCoef->csc_coef01;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_P0.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_P0.u32);

        VP0_CSC0_P1.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_P1.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_P1.bits.cscp02 = pstCscCoef->csc_coef02;
        VP0_CSC0_P1.bits.cscp10 = pstCscCoef->csc_coef10;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_P1.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_P1.u32);

        VP0_CSC0_P2.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_P2.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_P2.bits.cscp11 = pstCscCoef->csc_coef11;
        VP0_CSC0_P2.bits.cscp12 = pstCscCoef->csc_coef12;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_P2.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_P2.u32);

        VP0_CSC0_P3.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_P3.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_P3.bits.cscp20 = pstCscCoef->csc_coef20;
        VP0_CSC0_P3.bits.cscp21 = pstCscCoef->csc_coef21;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_P3.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_P3.u32);

        VP0_CSC0_P4.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_P4.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_P4.bits.cscp22 = pstCscCoef->csc_coef22;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_P4.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_P4.u32);
    }
    else if (enChan == HAL_DISP_LAYER_V1)
    {
        VP0_CSC1_P0.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_P0.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_P0.bits.cscp00 = pstCscCoef->csc_coef00;
        VP0_CSC1_P0.bits.cscp01 = pstCscCoef->csc_coef01;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_P0.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_P0.u32);
        VP0_CSC1_P1.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_P1.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_P1.bits.cscp02 = pstCscCoef->csc_coef02;
        VP0_CSC1_P1.bits.cscp10 = pstCscCoef->csc_coef10;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_P1.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_P1.u32);
        VP0_CSC1_P2.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_P2.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_P2.bits.cscp11 = pstCscCoef->csc_coef11;
        VP0_CSC1_P2.bits.cscp12 = pstCscCoef->csc_coef12;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_P2.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_P2.u32);
        VP0_CSC1_P3.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_P3.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_P3.bits.cscp20 = pstCscCoef->csc_coef20;
        VP0_CSC1_P3.bits.cscp21 = pstCscCoef->csc_coef21;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_P3.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_P3.u32);
        VP0_CSC1_P4.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_P4.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_P4.bits.cscp22 = pstCscCoef->csc_coef22;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_P4.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_P4.u32);
    }
    else if ((enChan >= HAL_DISP_LAYER_V2) && (enChan < HAL_DISP_LAYER_BUTT))
    {
        V0_CSC_P0.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_P0.u32) + enChan * VDP_VID_OFFSET);
        V0_CSC_P0.bits.cscp00 = pstCscCoef->csc_coef00;
        V0_CSC_P0.bits.cscp01 = pstCscCoef->csc_coef01;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_P0.u32) + enChan * VDP_VID_OFFSET), V0_CSC_P0.u32);

        V0_CSC_P1.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_P1.u32) + enChan  * VDP_VID_OFFSET);
        V0_CSC_P1.bits.cscp02 = pstCscCoef->csc_coef02;
        V0_CSC_P1.bits.cscp10 = pstCscCoef->csc_coef10;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_P1.u32) + enChan * VDP_VID_OFFSET), V0_CSC_P1.u32);

        V0_CSC_P2.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_P2.u32) + enChan  * VDP_VID_OFFSET);
        V0_CSC_P2.bits.cscp11 = pstCscCoef->csc_coef11;
        V0_CSC_P2.bits.cscp12 = pstCscCoef->csc_coef12;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_P2.u32) + enChan * VDP_VID_OFFSET), V0_CSC_P2.u32);

        V0_CSC_P3.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_P3.u32) + enChan  * VDP_VID_OFFSET);
        V0_CSC_P3.bits.cscp20 = pstCscCoef->csc_coef20;
        V0_CSC_P3.bits.cscp21 = pstCscCoef->csc_coef21;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_P3.u32) + enChan * VDP_VID_OFFSET), V0_CSC_P3.u32);

        V0_CSC_P4.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_P4.u32) + enChan * VDP_VID_OFFSET);
        V0_CSC_P4.bits.cscp22 = pstCscCoef->csc_coef22;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_P4.u32) + enChan * VDP_VID_OFFSET), V0_CSC_P4.u32);
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetCscDcCoef(HAL_DISP_LAYER_E enChan, CSC_DCCOEF_S* pstCscCoef)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_VP0_CSC0_IDC  VP0_CSC0_IDC;
    U_VP0_CSC0_ODC  VP0_CSC0_ODC;
    U_VP0_CSC0_IODC VP0_CSC0_IODC;

    U_VP0_CSC1_IDC  VP0_CSC1_IDC;
    U_VP0_CSC1_ODC  VP0_CSC1_ODC;
    U_VP0_CSC1_IODC VP0_CSC1_IODC;

    U_V0_CSC_IDC  V0_CSC_IDC;
    U_V0_CSC_ODC  V0_CSC_ODC;
    U_V0_CSC_IODC V0_CSC_IODC;

    if (enChan >= HAL_DISP_LAYER_BUTT)
    {
        HI_ERR_PQ("Error,Select Wrong VP layer!\n");
        return HI_FAILURE;
    }

    pstVDPReg = PQ_HAL_GetVdpReg();
    if (enChan == HAL_DISP_LAYER_V0)
    {
        VP0_CSC0_IDC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_IDC.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_IDC.bits.cscidc1  = pstCscCoef->csc_in_dc1;
        VP0_CSC0_IDC.bits.cscidc0  = pstCscCoef->csc_in_dc0;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_IDC.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_IDC.u32);

        VP0_CSC0_ODC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_ODC.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_ODC.bits.cscodc1 = pstCscCoef->csc_out_dc1;
        VP0_CSC0_ODC.bits.cscodc0 = pstCscCoef->csc_out_dc0;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_ODC.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_ODC.u32);

        VP0_CSC0_IODC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC0_IODC.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC0_IODC.bits.cscodc2 = pstCscCoef->csc_out_dc2;
        VP0_CSC0_IODC.bits.cscidc2 = pstCscCoef->csc_in_dc2;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC0_IODC.u32) + 0 * VDP_VID_OFFSET), VP0_CSC0_IODC.u32);
    }
    else if (enChan == HAL_DISP_LAYER_V1)
    {
        VP0_CSC1_IDC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_IDC.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_IDC.bits.cscidc1  = pstCscCoef->csc_in_dc1;
        VP0_CSC1_IDC.bits.cscidc0  = pstCscCoef->csc_in_dc0;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_IDC.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_IDC.u32);

        VP0_CSC1_ODC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_ODC.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_ODC.bits.cscodc1 = pstCscCoef->csc_out_dc1;
        VP0_CSC1_ODC.bits.cscodc0 = pstCscCoef->csc_out_dc0;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_ODC.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_ODC.u32);

        VP0_CSC1_IODC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->VP0_CSC1_IODC.u32) + 0 * VDP_VID_OFFSET);
        VP0_CSC1_IODC.bits.cscodc2 = pstCscCoef->csc_out_dc2;
        VP0_CSC1_IODC.bits.cscidc2 = pstCscCoef->csc_in_dc2;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->VP0_CSC1_IODC.u32) + 0 * VDP_VID_OFFSET), VP0_CSC1_IODC.u32);
    }
    else if ((enChan >= HAL_DISP_LAYER_V2) && (enChan < HAL_DISP_LAYER_BUTT))
    {
        V0_CSC_IDC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_IDC.u32) + enChan  * VDP_VID_OFFSET);
        V0_CSC_IDC.bits.cscidc1  = pstCscCoef->csc_in_dc1;
        V0_CSC_IDC.bits.cscidc0  = pstCscCoef->csc_in_dc0;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_IDC.u32) + enChan * VDP_VID_OFFSET), V0_CSC_IDC.u32);

        V0_CSC_ODC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_ODC.u32) + enChan * VDP_VID_OFFSET);
        V0_CSC_ODC.bits.cscodc1 = pstCscCoef->csc_out_dc1;
        V0_CSC_ODC.bits.cscodc0 = pstCscCoef->csc_out_dc0;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_ODC.u32) + enChan * VDP_VID_OFFSET), V0_CSC_ODC.u32);

        V0_CSC_IODC.u32 = PQ_HAL_RegRead((HI_U32) & (pstVDPReg->V0_CSC_IODC.u32) + enChan * VDP_VID_OFFSET);
        V0_CSC_IODC.bits.cscodc2 = pstCscCoef->csc_out_dc2;
        V0_CSC_IODC.bits.cscidc2 = pstCscCoef->csc_in_dc2;
        PQ_HAL_RegWrite(((HI_U32) & (pstVDPReg->V0_CSC_IODC.u32) + enChan * VDP_VID_OFFSET), V0_CSC_IODC.u32);
    }

    return HI_SUCCESS;
}

