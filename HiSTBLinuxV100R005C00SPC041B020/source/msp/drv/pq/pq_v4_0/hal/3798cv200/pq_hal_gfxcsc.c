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
        GP0_CSC_IDC.u32 = PQ_HAL_RegRead((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_IDC.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_IDC.bits.csc_en = bCscEn;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_IDC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_IDC.u32);

        /* set csc sign mode; 0:10bit unsigned; 1:8bits signed; */
        GP0_CSC_ODC.u32 = PQ_HAL_RegRead((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_ODC.u32)) + u32Data * PQ_GP_OFFSET));
        /* real precision only 8bit signed; can not set mode "1" */
        GP0_CSC_ODC.bits.csc_sign_mode = 0;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_ODC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_ODC.u32);
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
        GP0_CSC_P0.u32 = PQ_HAL_RegRead((HI_U32*)(((HI_ULONG) & (pVdpReg->GP0_CSC_P0.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P0.bits.cscp00 = pstGfxCscCoef->csc_coef00;
        GP0_CSC_P0.bits.cscp01 = pstGfxCscCoef->csc_coef01;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_P0.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P0.u32);

        GP0_CSC_P1.u32 = PQ_HAL_RegRead((HI_U32*)(((HI_ULONG) & (pVdpReg->GP0_CSC_P1.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P1.bits.cscp02 = pstGfxCscCoef->csc_coef02;
        GP0_CSC_P1.bits.cscp10 = pstGfxCscCoef->csc_coef10;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_P1.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P1.u32);

        GP0_CSC_P2.u32 = PQ_HAL_RegRead((HI_U32*)(((HI_ULONG) & (pVdpReg->GP0_CSC_P2.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P2.bits.cscp11 = pstGfxCscCoef->csc_coef11;
        GP0_CSC_P2.bits.cscp12 = pstGfxCscCoef->csc_coef12;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_P2.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P2.u32);

        GP0_CSC_P3.u32 = PQ_HAL_RegRead((HI_U32*)(((HI_ULONG) & (pVdpReg->GP0_CSC_P3.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P3.bits.cscp20 = pstGfxCscCoef->csc_coef20;
        GP0_CSC_P3.bits.cscp21 = pstGfxCscCoef->csc_coef21;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_P3.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P3.u32);

        GP0_CSC_P4.u32 = PQ_HAL_RegRead((HI_U32*)(((HI_ULONG) & (pVdpReg->GP0_CSC_P4.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_P4.bits.cscp22 = pstGfxCscCoef->csc_coef22;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_P4.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_P4.u32);
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
        GP0_CSC_IDC.u32 = PQ_HAL_RegRead((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_IDC.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_IDC.bits.cscidc1  = pstGfxCscDcCoef->csc_in_dc1;
        GP0_CSC_IDC.bits.cscidc0  = pstGfxCscDcCoef->csc_in_dc0;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_IDC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_IDC.u32);

        GP0_CSC_ODC.u32 = PQ_HAL_RegRead( (HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_ODC.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_ODC.bits.cscodc1 = pstGfxCscDcCoef->csc_out_dc1;
        GP0_CSC_ODC.bits.cscodc0 = pstGfxCscDcCoef->csc_out_dc0;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_ODC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_ODC.u32);

        GP0_CSC_IODC.u32 = PQ_HAL_RegRead((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_IODC.u32)) + u32Data * PQ_GP_OFFSET));
        GP0_CSC_IODC.bits.cscodc2 = pstGfxCscDcCoef->csc_out_dc2;
        GP0_CSC_IODC.bits.cscidc2 = pstGfxCscDcCoef->csc_in_dc2;
        PQ_HAL_RegWrite((HI_U32*)((HI_ULONG)(&(pVdpReg->GP0_CSC_IODC.u32)) + u32Data * PQ_GP_OFFSET), GP0_CSC_IODC.u32);
    }

    return;
}

HI_S32 PQ_HAL_SetGfxCsc(HI_U32 u32Data, HI_BOOL bCscEn, PQ_GFX_CSC_COEF_S* pstGfxCscCoef, PQ_GFX_CSC_DCCOEF_S* pstGfxCscDcCoef)
{
    PQ_HAL_SetGfxCscEnable(u32Data, bCscEn);
    PQ_HAL_SetGfxCscCoef  (u32Data, pstGfxCscCoef);
    PQ_HAL_SetGfxCscDcCoef(u32Data, pstGfxCscDcCoef);

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_HDR_SetCvmSel(HI_U32 cvm_sel)
{
    U_CVM_CTRL CVM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();
    CVM_CTRL.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM_CTRL.u32) ));
    CVM_CTRL.bits.cvm_sel = cvm_sel;
    PQ_HAL_RegWrite((&(pVdpReg->CVM_CTRL.u32) ), CVM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetCvmEn(HI_U32 cvm_en)
{
    U_CVM_CTRL CVM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();
    CVM_CTRL.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM_CTRL.u32) ));
    CVM_CTRL.bits.cvm_en = cvm_en;
    PQ_HAL_RegWrite((&(pVdpReg->CVM_CTRL.u32) ), CVM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGCvmOmapRgb2yuvOutDc(HI_S32 g_omap_rgb2yuv_out_dc[3])
{
    U_CVM2_RGB2YUV_OUT_DC0 CVM2_RGB2YUV_OUT_DC0;
    U_CVM2_RGB2YUV_OUT_DC1 CVM2_RGB2YUV_OUT_DC1;
    U_CVM2_RGB2YUV_OUT_DC2 CVM2_RGB2YUV_OUT_DC2;

    pVdpReg = PQ_HAL_GetVdpReg();

    /* dc coef of GP1 is correct order, do not need tran negation */
    CVM2_RGB2YUV_OUT_DC0.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_OUT_DC0.u32) ));
    CVM2_RGB2YUV_OUT_DC0.bits.g_omap_rgb2yuv_0_out_dc = g_omap_rgb2yuv_out_dc[0];
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_OUT_DC0.u32) ), CVM2_RGB2YUV_OUT_DC0.u32);

    CVM2_RGB2YUV_OUT_DC1.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_OUT_DC1.u32) ));
    CVM2_RGB2YUV_OUT_DC1.bits.g_omap_rgb2yuv_1_out_dc = g_omap_rgb2yuv_out_dc[1];
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_OUT_DC1.u32) ), CVM2_RGB2YUV_OUT_DC1.u32);

    CVM2_RGB2YUV_OUT_DC2.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_OUT_DC2.u32) ));
    CVM2_RGB2YUV_OUT_DC2.bits.g_omap_rgb2yuv_2_out_dc = g_omap_rgb2yuv_out_dc[2];
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_OUT_DC2.u32) ), CVM2_RGB2YUV_OUT_DC2.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGCvmRgb2yuvEn(HI_U32 g_rgb2yuv_en)
{
    U_CVM2_RGB2YUV_CTRL CVM2_RGB2YUV_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();
    CVM2_RGB2YUV_CTRL.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_CTRL.u32) ));
    CVM2_RGB2YUV_CTRL.bits.cvm2_rgb2yuv_en = g_rgb2yuv_en;
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_CTRL.u32) ), CVM2_RGB2YUV_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGCvmOmapRgb2yuv(HI_S32 g_omap_rgb2yuv[3][3])
{
    U_CVM2_RGB2YUV_COEF0 CVM2_RGB2YUV_COEF0;
    U_CVM2_RGB2YUV_COEF1 CVM2_RGB2YUV_COEF1;
    U_CVM2_RGB2YUV_COEF2 CVM2_RGB2YUV_COEF2;
    U_CVM2_RGB2YUV_COEF3 CVM2_RGB2YUV_COEF3;
    U_CVM2_RGB2YUV_COEF4 CVM2_RGB2YUV_COEF4;
    U_CVM2_RGB2YUV_COEF5 CVM2_RGB2YUV_COEF5;
    U_CVM2_RGB2YUV_COEF6 CVM2_RGB2YUV_COEF6;
    U_CVM2_RGB2YUV_COEF7 CVM2_RGB2YUV_COEF7;
    U_CVM2_RGB2YUV_COEF8 CVM2_RGB2YUV_COEF8;

    pVdpReg = PQ_HAL_GetVdpReg();
    CVM2_RGB2YUV_COEF0.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF0.u32) ));
    CVM2_RGB2YUV_COEF1.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF1.u32) ));
    CVM2_RGB2YUV_COEF2.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF2.u32) ));
    CVM2_RGB2YUV_COEF3.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF3.u32) ));
    CVM2_RGB2YUV_COEF4.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF4.u32) ));
    CVM2_RGB2YUV_COEF5.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF5.u32) ));
    CVM2_RGB2YUV_COEF6.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF6.u32) ));
    CVM2_RGB2YUV_COEF7.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF7.u32) ));
    CVM2_RGB2YUV_COEF8.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_COEF8.u32) ));
    CVM2_RGB2YUV_COEF0.bits.g_omap_00_rgb2yuv = g_omap_rgb2yuv[0][0];
    CVM2_RGB2YUV_COEF1.bits.g_omap_01_rgb2yuv = g_omap_rgb2yuv[0][1];
    CVM2_RGB2YUV_COEF2.bits.g_omap_02_rgb2yuv = g_omap_rgb2yuv[0][2];
    CVM2_RGB2YUV_COEF3.bits.g_omap_10_rgb2yuv = g_omap_rgb2yuv[1][0];
    CVM2_RGB2YUV_COEF4.bits.g_omap_11_rgb2yuv = g_omap_rgb2yuv[1][1];
    CVM2_RGB2YUV_COEF5.bits.g_omap_12_rgb2yuv = g_omap_rgb2yuv[1][2];
    CVM2_RGB2YUV_COEF6.bits.g_omap_20_rgb2yuv = g_omap_rgb2yuv[2][0];
    CVM2_RGB2YUV_COEF7.bits.g_omap_21_rgb2yuv = g_omap_rgb2yuv[2][1];
    CVM2_RGB2YUV_COEF8.bits.g_omap_22_rgb2yuv = g_omap_rgb2yuv[2][2];
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF0.u32) ), CVM2_RGB2YUV_COEF0.u32);
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF1.u32) ), CVM2_RGB2YUV_COEF1.u32);
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF2.u32) ), CVM2_RGB2YUV_COEF2.u32);
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF3.u32) ), CVM2_RGB2YUV_COEF3.u32);
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF4.u32) ), CVM2_RGB2YUV_COEF4.u32);
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF5.u32) ), CVM2_RGB2YUV_COEF5.u32);
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF6.u32) ), CVM2_RGB2YUV_COEF6.u32);
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF7.u32) ), CVM2_RGB2YUV_COEF7.u32);
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_COEF8.u32) ), CVM2_RGB2YUV_COEF8.u32);

    return;
}

/* CNcomment: 设置图形层RGB2YUV精度 CNend */
HI_VOID PQ_HAL_HDR_SetGCvmOmapRgb2yuvscale2p(HI_U32 g_omap_rgb2yuvscale2p)
{
    U_CVM2_RGB2YUV_SCALE2P CVM2_RGB2YUV_SCALE2P;

    pVdpReg = PQ_HAL_GetVdpReg();
    CVM2_RGB2YUV_SCALE2P.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_SCALE2P.u32) ));
    CVM2_RGB2YUV_SCALE2P.bits.g_omap_rgb2yuvscale2p = g_omap_rgb2yuvscale2p;
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_SCALE2P.u32) ), CVM2_RGB2YUV_SCALE2P.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetRegUp(HI_VOID)
{
    U_HDR_UPD HDR_UPD;

    pVdpReg = PQ_HAL_GetVdpReg();
    HDR_UPD.u32 = PQ_HAL_RegRead((&(pVdpReg->HDR_UPD.u32)));
    HDR_UPD.bits.regup = 1;
    PQ_HAL_RegWrite((&(pVdpReg->HDR_UPD.u32)), HDR_UPD.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmEn(HI_U32 g_gdm_en)
{
    U_GDM_CTRL GDM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();
    GDM_CTRL.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_CTRL.u32) ));
    GDM_CTRL.bits.g_gdm_en = g_gdm_en;
    PQ_HAL_RegWrite((&(pVdpReg->GDM_CTRL.u32) ), GDM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmDegammaEn(HI_U32 g_degamma_en)
{
    U_GDM_CTRL GDM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();
    GDM_CTRL.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_CTRL.u32) ));
    GDM_CTRL.bits.g_degamma_en = g_degamma_en;
    PQ_HAL_RegWrite((&(pVdpReg->GDM_CTRL.u32) ), GDM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmRgb2lmsEn(HI_U32 g_rgb2lms_en)
{
    U_GDM_RGB2LMS_CTRL GDM_RGB2LMS_CTRL;


    GDM_RGB2LMS_CTRL.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_CTRL.u32) ));
    GDM_RGB2LMS_CTRL.bits.gdm_rgb2lms_en = g_rgb2lms_en;
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_CTRL.u32) ), GDM_RGB2LMS_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmImapRgb2lms(HI_S16 g_imap_rgb2lms[3][3])
{
    U_GDM_RGB2LMS_COEF0 GDM_RGB2LMS_COEF0;
    U_GDM_RGB2LMS_COEF1 GDM_RGB2LMS_COEF1;
    U_GDM_RGB2LMS_COEF2 GDM_RGB2LMS_COEF2;
    U_GDM_RGB2LMS_COEF3 GDM_RGB2LMS_COEF3;
    U_GDM_RGB2LMS_COEF4 GDM_RGB2LMS_COEF4;
    U_GDM_RGB2LMS_COEF5 GDM_RGB2LMS_COEF5;
    U_GDM_RGB2LMS_COEF6 GDM_RGB2LMS_COEF6;
    U_GDM_RGB2LMS_COEF7 GDM_RGB2LMS_COEF7;
    U_GDM_RGB2LMS_COEF8 GDM_RGB2LMS_COEF8;

    pVdpReg = PQ_HAL_GetVdpReg();
    GDM_RGB2LMS_COEF0.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF0.u32) ));
    GDM_RGB2LMS_COEF1.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF1.u32) ));
    GDM_RGB2LMS_COEF2.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF2.u32) ));
    GDM_RGB2LMS_COEF3.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF3.u32) ));
    GDM_RGB2LMS_COEF4.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF4.u32) ));
    GDM_RGB2LMS_COEF5.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF5.u32) ));
    GDM_RGB2LMS_COEF6.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF6.u32) ));
    GDM_RGB2LMS_COEF7.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF7.u32) ));
    GDM_RGB2LMS_COEF8.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_COEF8.u32) ));
    GDM_RGB2LMS_COEF0.bits.g_imap_00_rgb2lms = g_imap_rgb2lms[0][0];
    GDM_RGB2LMS_COEF1.bits.g_imap_01_rgb2lms = g_imap_rgb2lms[0][1];
    GDM_RGB2LMS_COEF2.bits.g_imap_02_rgb2lms = g_imap_rgb2lms[0][2];
    GDM_RGB2LMS_COEF3.bits.g_imap_10_rgb2lms = g_imap_rgb2lms[1][0];
    GDM_RGB2LMS_COEF4.bits.g_imap_11_rgb2lms = g_imap_rgb2lms[1][1];
    GDM_RGB2LMS_COEF5.bits.g_imap_12_rgb2lms = g_imap_rgb2lms[1][2];
    GDM_RGB2LMS_COEF6.bits.g_imap_20_rgb2lms = g_imap_rgb2lms[2][0];
    GDM_RGB2LMS_COEF7.bits.g_imap_21_rgb2lms = g_imap_rgb2lms[2][1];
    GDM_RGB2LMS_COEF8.bits.g_imap_22_rgb2lms = g_imap_rgb2lms[2][2];
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF0.u32) ), GDM_RGB2LMS_COEF0.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF1.u32) ), GDM_RGB2LMS_COEF1.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF2.u32) ), GDM_RGB2LMS_COEF2.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF3.u32) ), GDM_RGB2LMS_COEF3.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF4.u32) ), GDM_RGB2LMS_COEF4.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF5.u32) ), GDM_RGB2LMS_COEF5.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF6.u32) ), GDM_RGB2LMS_COEF6.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF7.u32) ), GDM_RGB2LMS_COEF7.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_COEF8.u32) ), GDM_RGB2LMS_COEF8.u32);

    return;
}


HI_VOID PQ_HAL_HDR_SetGDmImapRgb2lmsscale2p(HI_U32 g_imap_rgb2lmsscale2p)
{
    U_GDM_RGB2LMS_SCALE2P GDM_RGB2LMS_SCALE2P;

    pVdpReg = PQ_HAL_GetVdpReg();
    GDM_RGB2LMS_SCALE2P.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_SCALE2P.u32) ));
    GDM_RGB2LMS_SCALE2P.bits.g_imap_rgb2lmsscale2p = g_imap_rgb2lmsscale2p;
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_SCALE2P.u32) ), GDM_RGB2LMS_SCALE2P.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmImapRgb2lmsmin(HI_U32 g_imap_rgb2lmsmin)
{
    U_GDM_RGB2LMS_MIN GDM_RGB2LMS_MIN;

    pVdpReg = PQ_HAL_GetVdpReg();
    GDM_RGB2LMS_MIN.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_MIN.u32) ));
    GDM_RGB2LMS_MIN.bits.g_imap_rgb2lmsmin = g_imap_rgb2lmsmin;
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_MIN.u32) ), GDM_RGB2LMS_MIN.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmImapRgb2lmsmax(HI_U32 g_imap_rgb2lmsmax)
{
    U_GDM_RGB2LMS_MAX GDM_RGB2LMS_MAX;

    pVdpReg = PQ_HAL_GetVdpReg();
    GDM_RGB2LMS_MAX.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_MAX.u32) ));
    GDM_RGB2LMS_MAX.bits.g_imap_rgb2lmsmax = g_imap_rgb2lmsmax;
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_MAX.u32) ), GDM_RGB2LMS_MAX.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmImapRgb2lmsOutDc(HI_S32 g_imap_rgb2lms_out_dc[3])
{
    U_GDM_RGB2LMS_OUT_DC0 GDM_RGB2LMS_OUT_DC0;
    U_GDM_RGB2LMS_OUT_DC1 GDM_RGB2LMS_OUT_DC1;
    U_GDM_RGB2LMS_OUT_DC2 GDM_RGB2LMS_OUT_DC2;

    pVdpReg = PQ_HAL_GetVdpReg();
    GDM_RGB2LMS_OUT_DC0.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_OUT_DC0.u32) ));
    GDM_RGB2LMS_OUT_DC1.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_OUT_DC1.u32) ));
    GDM_RGB2LMS_OUT_DC2.u32 = PQ_HAL_RegRead((&(pVdpReg->GDM_RGB2LMS_OUT_DC2.u32) ));
    GDM_RGB2LMS_OUT_DC0.bits.g_imap_rgb2lms_0_out_dc = g_imap_rgb2lms_out_dc[0];
    GDM_RGB2LMS_OUT_DC1.bits.g_imap_rgb2lms_1_out_dc = g_imap_rgb2lms_out_dc[1];
    GDM_RGB2LMS_OUT_DC2.bits.g_imap_rgb2lms_2_out_dc = g_imap_rgb2lms_out_dc[2];
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_OUT_DC0.u32) ), GDM_RGB2LMS_OUT_DC0.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_OUT_DC1.u32) ), GDM_RGB2LMS_OUT_DC1.u32);
    PQ_HAL_RegWrite((&(pVdpReg->GDM_RGB2LMS_OUT_DC2.u32) ), GDM_RGB2LMS_OUT_DC2.u32);

    return;
}

/* CNcomment: 设置图形层RGB2YUV钳位最小值 CNend */
HI_VOID PQ_HAL_HDR_SetGCvmOmapRgb2yuvmin(HI_U32 g_imap_lms2iptmin)
{
    U_CVM2_RGB2YUV_MIN CVM2_RGB2YUV_MIN;
    pVdpReg = PQ_HAL_GetVdpReg();
    CVM2_RGB2YUV_MIN.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_MIN.u32) ));
    CVM2_RGB2YUV_MIN.bits.g_omap_rgb2yuvmin = g_imap_lms2iptmin;
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_MIN.u32) ), CVM2_RGB2YUV_MIN.u32);

    return;
}

/* CNcomment: 设置图形层RGB2YUV钳位最大值 CNend */
HI_VOID PQ_HAL_HDR_SetGCvmOmapRgb2yuvmax(HI_U32 g_imap_lms2iptmax)
{
    U_CVM2_RGB2YUV_MAX CVM2_RGB2YUV_MAX;
    pVdpReg = PQ_HAL_GetVdpReg();
    CVM2_RGB2YUV_MAX.u32 = PQ_HAL_RegRead((&(pVdpReg->CVM2_RGB2YUV_MAX.u32) ));
    CVM2_RGB2YUV_MAX.bits.g_omap_rgb2yuvmax = g_imap_lms2iptmax;
    PQ_HAL_RegWrite((&(pVdpReg->CVM2_RGB2YUV_MAX.u32) ), CVM2_RGB2YUV_MAX.u32);

    return;
}

HI_S32 PQ_HAL_SetGfxGdmCoef(PQ_GFX_CSC_COEF_S* pstCscCoef, PQ_GFX_CSC_DCCOEF_S* pstCscDcCoef)
{
    HI_U32 gdm_en                = HI_TRUE;
    HI_U32 g_degamma_en          = HI_FALSE;
    HI_U32 g_rgb2lms_en          = HI_TRUE;
    HI_U32 g_imap_rgb2lmsscale2p = 16; /* precision */
    HI_U32 g_imap_rgb2lmsmin     = 0;
    HI_U32 g_imap_rgb2lmsmax     = 26219552;

    HI_S16 g_imap_rgb2lms[3][3];
    HI_S32 g_imap_rgb2lms_out_dc[3];

    g_imap_rgb2lms[0][0]     = pstCscCoef->csc_coef00;
    g_imap_rgb2lms[0][1]     = pstCscCoef->csc_coef01;
    g_imap_rgb2lms[0][2]     = pstCscCoef->csc_coef02;
    g_imap_rgb2lms[1][0]     = pstCscCoef->csc_coef10;
    g_imap_rgb2lms[1][1]     = pstCscCoef->csc_coef11;
    g_imap_rgb2lms[1][2]     = pstCscCoef->csc_coef12;
    g_imap_rgb2lms[2][0]     = pstCscCoef->csc_coef20;
    g_imap_rgb2lms[2][1]     = pstCscCoef->csc_coef21;
    g_imap_rgb2lms[2][2]     = pstCscCoef->csc_coef22;

    g_imap_rgb2lms_out_dc[0] = pstCscDcCoef->csc_out_dc0;
    g_imap_rgb2lms_out_dc[1] = pstCscDcCoef->csc_out_dc1;
    g_imap_rgb2lms_out_dc[2] = pstCscDcCoef->csc_out_dc2;

    PQ_HAL_HDR_SetGDmEn                ( gdm_en                );
    PQ_HAL_HDR_SetGDmDegammaEn         ( g_degamma_en          );
    PQ_HAL_HDR_SetGDmRgb2lmsEn         ( g_rgb2lms_en          );
    PQ_HAL_HDR_SetGDmImapRgb2lms       ( g_imap_rgb2lms        );
    PQ_HAL_HDR_SetGDmImapRgb2lmsscale2p( g_imap_rgb2lmsscale2p );
    PQ_HAL_HDR_SetGDmImapRgb2lmsmin    ( g_imap_rgb2lmsmin     );
    PQ_HAL_HDR_SetGDmImapRgb2lmsmax    ( g_imap_rgb2lmsmax     );
    PQ_HAL_HDR_SetGDmImapRgb2lmsOutDc  ( g_imap_rgb2lms_out_dc );

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetGfxCvmCscCoef(PQ_GFX_CSC_COEF_S* pstCscCoef, PQ_GFX_CSC_DCCOEF_S* pstCscDcCoef)
{
    HI_BOOL  gcvm_csc_en;
    //HI_U32 u32Pre, u32DcPre, u32Scale;
    HI_U32   u32Yuv2RgbScale2P;
    HI_S32   m33Rgb2Yuv[3][3];
    HI_S32   m33Rgb2YuvOutDc[3];

    PQ_HAL_HDR_SetCvmEn(HI_TRUE);
    PQ_HAL_HDR_SetGCvmRgb2yuvEn(HI_TRUE);
    PQ_HAL_HDR_SetCvmSel(0);

    //u32Pre          = 1 << 10;
    //u32DcPre        = 4 * 32;//1:8bit; 4:10bit
    //u32Scale        = 1 << (u32Yuv2RgbScale2P - 10);
    //u32DcPre        = 4 * 64; /* different to pre */
    gcvm_csc_en       = HI_TRUE;
    u32Yuv2RgbScale2P = 15; /* precision */ //sGetRandEx(5,31);

    m33Rgb2Yuv[0][0] = pstCscCoef->csc_coef00;
    m33Rgb2Yuv[0][1] = pstCscCoef->csc_coef01;
    m33Rgb2Yuv[0][2] = pstCscCoef->csc_coef02;
    m33Rgb2Yuv[1][0] = pstCscCoef->csc_coef10;
    m33Rgb2Yuv[1][1] = pstCscCoef->csc_coef11;
    m33Rgb2Yuv[1][2] = pstCscCoef->csc_coef12;
    m33Rgb2Yuv[2][0] = pstCscCoef->csc_coef20;
    m33Rgb2Yuv[2][1] = pstCscCoef->csc_coef21;
    m33Rgb2Yuv[2][2] = pstCscCoef->csc_coef22;

    /*  do not need tran negation */
    m33Rgb2YuvOutDc[0] = pstCscDcCoef->csc_out_dc0;
    m33Rgb2YuvOutDc[1] = pstCscDcCoef->csc_out_dc1;
    m33Rgb2YuvOutDc[2] = pstCscDcCoef->csc_out_dc2;

    PQ_HAL_HDR_SetGCvmOmapRgb2yuvscale2p(u32Yuv2RgbScale2P | (gcvm_csc_en << 5));
    PQ_HAL_HDR_SetGCvmOmapRgb2yuv       (m33Rgb2Yuv);
    PQ_HAL_HDR_SetGCvmOmapRgb2yuvOutDc  (m33Rgb2YuvOutDc);
    PQ_HAL_HDR_SetGCvmOmapRgb2yuvmin    (0);
    PQ_HAL_HDR_SetGCvmOmapRgb2yuvmax    (0x7FFF);

    /* todo HDR regup; need set at here ?? */
    PQ_HAL_HDR_SetRegUp();

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetGdmCtrl(HI_U32 bOnOff)
{
    PQ_HAL_HDR_SetGDmEn       (bOnOff);
    PQ_HAL_HDR_SetGDmDegammaEn(bOnOff);
    PQ_HAL_HDR_SetGDmRgb2lmsEn(bOnOff);

    return HI_SUCCESS;
}



