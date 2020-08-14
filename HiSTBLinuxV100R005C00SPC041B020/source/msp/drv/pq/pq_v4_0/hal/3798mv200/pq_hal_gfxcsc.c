/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_gfxcsc.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2015/10/10
  Description   :

******************************************************************************/

#include "pq_hal_comm.h"
#include "pq_hal_gfxcsc.h"

#ifndef HI_BUILD_IN_BOOT
#include <linux/kernel.h>
#include <linux/string.h>
#else
#include "hi_type.h"
#include "uboot.h"
#endif


static S_VDP_REGS_TYPE* pVdpReg;



/***************************************************************************************
* func          : PQ_HAL_SetGfxCscEnable
* description   : CNcomment: 设置GP CSC使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID PQ_HAL_SetGfxCscEnable(HI_U32 u32Data, HI_BOOL bCscEn)
{
    volatile U_GP0_CSC_IDC   GP0_CSC_IDC;
    volatile U_GP0_CSC_ODC   GP0_CSC_ODC;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_GP_SetCscEnable() Select Wrong GPeo Layer ID\n");
        return;
    }

    if (u32Data != 0) /** bound with Wbc; but set GP1 **/
    {
        GP0_CSC_IDC.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_IDC.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_IDC.bits.csc_en = bCscEn;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_IDC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_IDC.u32);

        /* set csc sign mode; 0:10bit unsigned; 1:8bits signed; */
        GP0_CSC_ODC.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_ODC.u32)) + u32Data * PQ_GP_OFFSET));
        /* real precision only 8bit signed; can not set mode "1" */
        GP0_CSC_ODC.bits.csc_sign_mode = 0;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_ODC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_ODC.u32);
    }
    return;
}


/***************************************************************************************
* func          : PQ_HAL_SetGfxCscCoef
* description   : CNcomment: 设置GP CSC系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID PQ_HAL_SetGfxCscCoef(HI_U32 u32Data, PQ_GFX_CSC_COEF_S* pstGfxCscCoef)
{
    volatile U_GP0_CSC_P0 GP0_CSC_P0;
    volatile U_GP0_CSC_P1 GP0_CSC_P1;
    volatile U_GP0_CSC_P2 GP0_CSC_P2;
    volatile U_GP0_CSC_P3 GP0_CSC_P3;
    volatile U_GP0_CSC_P4 GP0_CSC_P4;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_GP_SetCscCoef Select Wrong GPeo ID\n");
        return;
    }

    if (u32Data != 0) /** bound with Wbc; but set GP1 **/
    {
        GP0_CSC_P0.u32 = PQ_HAL_RegRead((HI_U32*)(((unsigned long) & (pVdpReg->GP0_CSC_P0.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P0.bits.cscp00 = pstGfxCscCoef->csc_coef00;
        GP0_CSC_P0.bits.cscp01 = pstGfxCscCoef->csc_coef01;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_P0.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P0.u32);

        GP0_CSC_P1.u32 = PQ_HAL_RegRead((HI_U32*)(((unsigned long) & (pVdpReg->GP0_CSC_P1.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P1.bits.cscp02 = pstGfxCscCoef->csc_coef02;
        GP0_CSC_P1.bits.cscp10 = pstGfxCscCoef->csc_coef10;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_P1.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P1.u32);

        GP0_CSC_P2.u32 = PQ_HAL_RegRead((HI_U32*)(((unsigned long) & (pVdpReg->GP0_CSC_P2.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P2.bits.cscp11 = pstGfxCscCoef->csc_coef11;
        GP0_CSC_P2.bits.cscp12 = pstGfxCscCoef->csc_coef12;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_P2.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P2.u32);

        GP0_CSC_P3.u32 = PQ_HAL_RegRead((HI_U32*)(((unsigned long) & (pVdpReg->GP0_CSC_P3.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P3.bits.cscp20 = pstGfxCscCoef->csc_coef20;
        GP0_CSC_P3.bits.cscp21 = pstGfxCscCoef->csc_coef21;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_P3.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P3.u32);

        GP0_CSC_P4.u32 = PQ_HAL_RegRead((HI_U32*)(((unsigned long) & (pVdpReg->GP0_CSC_P4.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P4.bits.cscp22 = pstGfxCscCoef->csc_coef22;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_P4.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P4.u32);
    }
    return;
}

/***************************************************************************************
* func          : PQ_HAL_SetGfxCscDcCoef
* description   : CNcomment: 设置GP CSC DC系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID PQ_HAL_SetGfxCscDcCoef(HI_U32 u32Data, PQ_GFX_CSC_DCCOEF_S* pstGfxCscDcCoef)
{
    volatile U_GP0_CSC_IDC  GP0_CSC_IDC;
    volatile U_GP0_CSC_ODC  GP0_CSC_ODC;
    volatile U_GP0_CSC_IODC GP0_CSC_IODC;

    pVdpReg = PQ_HAL_GetVdpReg();

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("PQ_HAL_GP_SetCscDcCoef() Select Wrong GPeo Layer ID\n");
        return;
    }

    if (u32Data != 0) /** bound with Wbc; but set GP1 **/
    {
        /* dc coef of GP1 is correct order, do not need tran negation */
        GP0_CSC_IDC.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_IDC.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_IDC.bits.cscidc1  = pstGfxCscDcCoef->csc_in_dc1;
        GP0_CSC_IDC.bits.cscidc0  = pstGfxCscDcCoef->csc_in_dc0;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_IDC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_IDC.u32);

        GP0_CSC_ODC.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_ODC.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_ODC.bits.cscodc1 = pstGfxCscDcCoef->csc_out_dc1;
        GP0_CSC_ODC.bits.cscodc0 = pstGfxCscDcCoef->csc_out_dc0;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_ODC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_ODC.u32);

        GP0_CSC_IODC.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_IODC.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_IODC.bits.cscodc2 = pstGfxCscDcCoef->csc_out_dc2;
        GP0_CSC_IODC.bits.cscidc2 = pstGfxCscDcCoef->csc_in_dc2;
        PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GP0_CSC_IODC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_IODC.u32);
    }

    return;
}

HI_VOID PQ_HAL_SetGdmGdm00Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_00_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF0 GDM_RGB2YUV_COEF0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF0.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF0.bits.gdm_00_rgb2yuv = gdm_00_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF0.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdm01Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_01_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF1 GDM_RGB2YUV_COEF1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF1.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF1.bits.gdm_01_rgb2yuv = gdm_01_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF1.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdm02Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_02_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF2 GDM_RGB2YUV_COEF2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF2.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF2.bits.gdm_02_rgb2yuv = gdm_02_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF2.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdm10Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_10_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF3 GDM_RGB2YUV_COEF3;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF3.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF3.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF3.bits.gdm_10_rgb2yuv = gdm_10_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF3.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF3.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdm11Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_11_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF4 GDM_RGB2YUV_COEF4;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF4.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF4.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF4.bits.gdm_11_rgb2yuv = gdm_11_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF4.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF4.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdm12Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_12_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF5 GDM_RGB2YUV_COEF5;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF5.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF5.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF5.bits.gdm_12_rgb2yuv = gdm_12_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF5.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF5.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdm20Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_20_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF6 GDM_RGB2YUV_COEF6;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF6.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF6.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF6.bits.gdm_20_rgb2yuv = gdm_20_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF6.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF6.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdm21Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_21_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF7 GDM_RGB2YUV_COEF7;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF7.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF7.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF7.bits.gdm_21_rgb2yuv = gdm_21_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF7.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF7.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdm22Rgb2yuv(HI_U32 u32Data, HI_U32 gdm_22_rgb2yuv)
{
    U_GDM_RGB2YUV_COEF8 GDM_RGB2YUV_COEF8;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_COEF8.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF8.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_COEF8.bits.gdm_22_rgb2yuv = gdm_22_rgb2yuv;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_COEF8.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_COEF8.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuvscale2p(HI_U32 u32Data, HI_U32 gdm_rgb2yuvscale2p)
{
    U_GDM_RGB2YUV_SCALE2P GDM_RGB2YUV_SCALE2P;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_SCALE2P.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_SCALE2P.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_SCALE2P.bits.gdm_rgb2yuvscale2p = gdm_rgb2yuvscale2p;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_SCALE2P.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_SCALE2P.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuv0InDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_0_in_dc)
{
    U_GDM_RGB2YUV_IN_DC0 GDM_RGB2YUV_IN_DC0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_IN_DC0.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_IN_DC0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_IN_DC0.bits.gdm_rgb2yuv_0_in_dc = gdm_rgb2yuv_0_in_dc;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_IN_DC0.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_IN_DC0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuv1InDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_1_in_dc)
{
    U_GDM_RGB2YUV_IN_DC1 GDM_RGB2YUV_IN_DC1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_IN_DC1.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_IN_DC1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_IN_DC1.bits.gdm_rgb2yuv_1_in_dc = gdm_rgb2yuv_1_in_dc;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_IN_DC1.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_IN_DC1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuv2InDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_2_in_dc)
{
    U_GDM_RGB2YUV_IN_DC2 GDM_RGB2YUV_IN_DC2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_IN_DC2.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_IN_DC2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_IN_DC2.bits.gdm_rgb2yuv_2_in_dc = gdm_rgb2yuv_2_in_dc;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_IN_DC2.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_IN_DC2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuv0OutDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_0_out_dc)
{
    U_GDM_RGB2YUV_OUT_DC0 GDM_RGB2YUV_OUT_DC0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_OUT_DC0.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_OUT_DC0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_OUT_DC0.bits.gdm_rgb2yuv_0_out_dc = gdm_rgb2yuv_0_out_dc;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_OUT_DC0.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_OUT_DC0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuv1OutDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_1_out_dc)
{
    U_GDM_RGB2YUV_OUT_DC1 GDM_RGB2YUV_OUT_DC1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_OUT_DC1.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_OUT_DC1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_OUT_DC1.bits.gdm_rgb2yuv_1_out_dc = gdm_rgb2yuv_1_out_dc;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_OUT_DC1.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_OUT_DC1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuv2OutDc(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_2_out_dc)
{
    U_GDM_RGB2YUV_OUT_DC2 GDM_RGB2YUV_OUT_DC2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_OUT_DC2.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_OUT_DC2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_OUT_DC2.bits.gdm_rgb2yuv_2_out_dc = gdm_rgb2yuv_2_out_dc;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_OUT_DC2.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_OUT_DC2.u32);

    return;
}

HI_VOID PQ_HAL_SetGdmGdmRgb2yuvClipMin(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_clip_min)
{
    U_GDM_RGB2YUV_MIN GDM_RGB2YUV_MIN;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_MIN.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_MIN.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_MIN.bits.gdm_rgb2yuv_clip_min = gdm_rgb2yuv_clip_min;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_MIN.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_MIN.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuvClipMax(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_clip_max)
{
    U_GDM_RGB2YUV_MAX GDM_RGB2YUV_MAX;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_MAX.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_MAX.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_MAX.bits.gdm_rgb2yuv_clip_max = gdm_rgb2yuv_clip_max;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_MAX.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_MAX.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmRgb2yuvEn(HI_U32 u32Data, HI_U32 gdm_rgb2yuv_en)
{
    U_GDM_RGB2YUV_CTRL GDM_RGB2YUV_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_RGB2YUV_CTRL.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_RGB2YUV_CTRL.bits.gdm_rgb2yuv_en = gdm_rgb2yuv_en;
    PQ_HAL_RegWrite((HI_U32*)((unsigned long)(&(pVdpReg->GDM_RGB2YUV_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_RGB2YUV_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_SetGdmCscCoef(HI_U32 enLayer, PQ_GFX_CSC_COEF_S* pstCscCoef)
{
    PQ_HAL_SetGdmGdm00Rgb2yuv(enLayer, pstCscCoef->csc_coef00);
    PQ_HAL_SetGdmGdm01Rgb2yuv(enLayer, pstCscCoef->csc_coef01);
    PQ_HAL_SetGdmGdm02Rgb2yuv(enLayer, pstCscCoef->csc_coef02);
    PQ_HAL_SetGdmGdm10Rgb2yuv(enLayer, pstCscCoef->csc_coef10);
    PQ_HAL_SetGdmGdm11Rgb2yuv(enLayer, pstCscCoef->csc_coef11);
    PQ_HAL_SetGdmGdm12Rgb2yuv(enLayer, pstCscCoef->csc_coef12);
    PQ_HAL_SetGdmGdm20Rgb2yuv(enLayer, pstCscCoef->csc_coef20);
    PQ_HAL_SetGdmGdm21Rgb2yuv(enLayer, pstCscCoef->csc_coef21);
    PQ_HAL_SetGdmGdm22Rgb2yuv(enLayer, pstCscCoef->csc_coef22);
}

HI_VOID PQ_HAL_SetGdmCscDc(HI_U32 enLayer, PQ_GFX_CSC_DCCOEF_S* pstCscDcCoef)
{

    PQ_HAL_SetGdmGdmRgb2yuv0InDc (enLayer, pstCscDcCoef->csc_in_dc0);
    PQ_HAL_SetGdmGdmRgb2yuv1InDc (enLayer, pstCscDcCoef->csc_in_dc1);
    PQ_HAL_SetGdmGdmRgb2yuv2InDc (enLayer, pstCscDcCoef->csc_in_dc2);
    PQ_HAL_SetGdmGdmRgb2yuv0OutDc(enLayer, pstCscDcCoef->csc_out_dc0);
    PQ_HAL_SetGdmGdmRgb2yuv1OutDc(enLayer, pstCscDcCoef->csc_out_dc1);
    PQ_HAL_SetGdmGdmRgb2yuv2OutDc(enLayer, pstCscDcCoef->csc_out_dc2);

    return;
}

/* GP1 */
HI_S32 PQ_HAL_SetGfxCsc(HI_U32 u32Data, HI_BOOL bCscEn, PQ_GFX_CSC_COEF_S* pstGfxCscCoef, PQ_GFX_CSC_DCCOEF_S* pstGfxCscDcCoef)
{
    PQ_HAL_SetGfxCscEnable(u32Data, bCscEn);
    PQ_HAL_SetGfxCscCoef  (u32Data, pstGfxCscCoef);
    PQ_HAL_SetGfxCscDcCoef(u32Data, pstGfxCscDcCoef);

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_TransGfxCscPrecision(PQ_GFX_CSC_PRE_S* pstCscPre, PQ_GFX_CSC_COEF_S* pstCscOriCoef, PQ_GFX_CSC_DCCOEF_S* pstCscOriDcCoef,
    PQ_GFX_CSC_COEF_S* pstCscTrueCoef, PQ_GFX_CSC_DCCOEF_S* pstCscTrueDcCoef)
{
    HI_U32 CscCoefPre;      /* Csc 系数精度 */
    HI_U32 CscInDcCoefPre;  /* Csc 直流分量精度 */
    HI_U32 CscOutDcCoefPre; /* Csc 直流分量精度 */

    if(pstCscPre->CscCoefPre > 10)
    {
        CscCoefPre = 1 << (pstCscPre->CscCoefPre - 10);
    }
    else
    {
        CscCoefPre = 1;
    }

    if(pstCscPre->CscInDcCoefPre > 10)
    {
        CscInDcCoefPre = 1 << (pstCscPre->CscInDcCoefPre - 10);
    }
    else
    {
        CscInDcCoefPre = 1;
    }

    if(pstCscPre->CscOutDcCoefPre > 10)
    {
        CscOutDcCoefPre = 1 << (pstCscPre->CscOutDcCoefPre - 10);
    }
    else
    {
        CscOutDcCoefPre = 1;
    }

    pstCscTrueCoef->csc_coef00    = (HI_S32)(pstCscOriCoef->csc_coef00 * CscCoefPre);
    pstCscTrueCoef->csc_coef01    = (HI_S32)(pstCscOriCoef->csc_coef01 * CscCoefPre);
    pstCscTrueCoef->csc_coef02    = (HI_S32)(pstCscOriCoef->csc_coef02 * CscCoefPre);
    pstCscTrueCoef->csc_coef10    = (HI_S32)(pstCscOriCoef->csc_coef10 * CscCoefPre);
    pstCscTrueCoef->csc_coef11    = (HI_S32)(pstCscOriCoef->csc_coef11 * CscCoefPre);
    pstCscTrueCoef->csc_coef12    = (HI_S32)(pstCscOriCoef->csc_coef12 * CscCoefPre);
    pstCscTrueCoef->csc_coef20    = (HI_S32)(pstCscOriCoef->csc_coef20 * CscCoefPre);
    pstCscTrueCoef->csc_coef21    = (HI_S32)(pstCscOriCoef->csc_coef21 * CscCoefPre);
    pstCscTrueCoef->csc_coef22    = (HI_S32)(pstCscOriCoef->csc_coef22 * CscCoefPre);

    pstCscTrueDcCoef->csc_in_dc0  = (HI_S32)(pstCscOriDcCoef->csc_in_dc0  * CscInDcCoefPre);
    pstCscTrueDcCoef->csc_in_dc1  = (HI_S32)(pstCscOriDcCoef->csc_in_dc1  * CscInDcCoefPre);
    pstCscTrueDcCoef->csc_in_dc2  = (HI_S32)(pstCscOriDcCoef->csc_in_dc2  * CscInDcCoefPre);
    pstCscTrueDcCoef->csc_out_dc0 = (HI_S32)(pstCscOriDcCoef->csc_out_dc0 * CscOutDcCoefPre);
    pstCscTrueDcCoef->csc_out_dc1 = (HI_S32)(pstCscOriDcCoef->csc_out_dc1 * CscOutDcCoefPre);
    pstCscTrueDcCoef->csc_out_dc2 = (HI_S32)(pstCscOriDcCoef->csc_out_dc2 * CscOutDcCoefPre);

    return;
}



HI_S32 PQ_HAL_SetGfxGdmCoef(PQ_GFX_CSC_COEF_S* pstCscCoef, PQ_GFX_CSC_DCCOEF_S* pstCscDcCoef)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetGfxCvmCscCoef(PQ_GFX_CSC_COEF_S* pstCscCoef, PQ_GFX_CSC_DCCOEF_S* pstCscDcCoef)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetGdmCtrl(HI_U32 bOnOff)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetGfxGdmCsc(HI_U32 enLayer, HI_BOOL bCSCEn, PQ_GFX_CSC_COEF_S* pstCscCoef, PQ_GFX_CSC_DCCOEF_S* pstCscDcCoef)
{
    HI_U16 gdm_rgb2yuvscale2p   = 0;
    HI_U16 gdm_rgb2yuv_clip_min = 0;
    HI_U16 gdm_rgb2yuv_clip_max = 0;
    PQ_GFX_CSC_PRE_S    stCscPre;
    PQ_GFX_CSC_COEF_S   stCscTrueCoef;
    PQ_GFX_CSC_DCCOEF_S stCscTrueDcCoef;

    gdm_rgb2yuvscale2p   = 14;
    gdm_rgb2yuv_clip_min = 0;
    gdm_rgb2yuv_clip_max = 1023;

    stCscPre.CscCoefPre      = 14;
    stCscPre.CscInDcCoefPre  = 10;
    stCscPre.CscOutDcCoefPre = 10;

    PQ_HAL_TransGfxCscPrecision(&stCscPre, pstCscCoef, pstCscDcCoef, &stCscTrueCoef, &stCscTrueDcCoef);

    PQ_HAL_SetGdmGdmRgb2yuvEn(enLayer, bCSCEn);
    PQ_HAL_SetGdmCscCoef     (enLayer, &stCscTrueCoef);
    PQ_HAL_SetGdmCscDc       (enLayer, &stCscTrueDcCoef);

    PQ_HAL_SetGdmGdmRgb2yuvscale2p(enLayer, gdm_rgb2yuvscale2p);
    PQ_HAL_SetGdmGdmRgb2yuvClipMin(enLayer, gdm_rgb2yuv_clip_min);
    PQ_HAL_SetGdmGdmRgb2yuvClipMax(enLayer, gdm_rgb2yuv_clip_max);

    return HI_SUCCESS;
}




