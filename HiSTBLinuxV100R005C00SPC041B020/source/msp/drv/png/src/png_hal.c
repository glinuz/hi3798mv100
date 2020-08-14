/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name    : png_hal.h
Version        : Initial Draft
Author        : z00141204
Created        : 2010/10/12
Description    : PNG Hal layer code    CNcomment:PNG hal≤„ µœ÷
Function List     :

History           :
Date                Author                Modification
2010/10/12        z00141204        Created file
******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif    /* __cplusplus */
#endif    /* __cplusplus */

#include <asm/io.h>
#include <linux/delay.h>

#include "png_hal.h"
#include "hi_png_errcode.h"
#include "hi_gfx_comm_k.h"
/** get chip version **/
#include "hi_png_config.h"


#ifdef CONFIG_PNG_USE_SDK_CRG_ENABLE
#include "hi_reg_common.h"
/** get chip version **/
#include "hi_drv_sys.h"
#endif

/* ptr to register structure*/
static volatile PNG_HAL_REGISTER_S *g_pstPngReg = HI_NULL;

#ifndef CONFIG_PNG_USE_SDK_CRG_ENABLE
/* vitural address to reset registet base*/
static volatile HI_U32 *g_pu32RegReset = HI_NULL;
#endif
#include "png_define.h"
static HI_U32 g_u32PngIrqNum = 0;
static HI_U32 g_u32PngRegAddr = 0;

#define DECLARE_PNG_REG \
{\
    g_u32PngRegAddr = PNG_REG_BASEADDR;\
    g_u32PngIrqNum  = PNG_INTNUM;    \
}
/********************************************************************************************
* func:    Hal init
* in:    none
* out:    none
* ret:    HI_SUCCESS    open success
*        HI_ERR_PNG_SYS    systerm error , such as map failed
* others:
*********************************************************************************************/
HI_S32 PngHalInit(HI_VOID)
{
    DECLARE_PNG_REG
    /* register map*/
    g_pstPngReg = (volatile PNG_HAL_REGISTER_S *)HI_GFX_REG_MAP(g_u32PngRegAddr, PNG_REG_SIZE);

    if (NULL == g_pstPngReg)
    {
        return HI_ERR_PNG_SYS;
    }

    #ifndef CONFIG_PNG_USE_SDK_CRG_ENABLE
    g_pu32RegReset = (volatile HI_U32 *)HI_GFX_REG_MAP(PNG_REG_RESET, 4);
    if (NULL == g_pu32RegReset)
    {
        HI_GFX_REG_UNMAP(g_pstPngReg);
        return HI_ERR_PNG_SYS;
    }
    #endif
    PngHalSetClock(HI_FALSE);

    HI_GFX_MapSmmuReg((HI_U32)(PNG_REG_BASEADDR + 0xf000));
    HI_GFX_InitSmmu((HI_U32)(PNG_REG_BASEADDR + 0xf000));


    return HI_SUCCESS;

}

extern HI_U32 g_u32RdcBufPhyaddr;
HI_S32 PngHalOpen(HI_VOID)
{
    /* register reset*/

    PngHalSetClock(HI_TRUE);
    PngHalReset();

    PngHalSetRdcAddr(g_u32RdcBufPhyaddr);
    /* config AXI */
    g_pstPngReg->u32AXIConfig = 0x20441;

    /* config port time out*/
    g_pstPngReg->u32TimeOut = 0x80008;

    /* open all the interupt*/
    g_pstPngReg->uIntMask.u32All = 0xffffffff;

    /* set error strategy */
    g_pstPngReg->uErrMode.u32All = 0xffff0000;

    HI_GFX_InitSmmu((HI_U32)(PNG_REG_BASEADDR + 0xf000));

    return HI_SUCCESS;
}

/********************************************************************************************
* func:    Hal deinit
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalDeinit(HI_VOID)
{
    HI_GFX_DeinitSmmu();
    HI_GFX_UnMapSmmuReg();
    HI_GFX_REG_UNMAP(g_pstPngReg);
    g_pstPngReg = HI_NULL;

    return;
}

/********************************************************************************************
* func:    register reset
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalReset(HI_VOID)
{
    HI_U32 i = 0;
    HI_U32 u32AXIConfig = 0;
    HI_U32 u32TimeOut = 0;
    HI_U32 u32RdcPhyaddr = 0;
    HI_U32 u32ErrMode = 0;
    HI_U32 u32IntMask = 0;
#ifdef CONFIG_PNG_USE_SDK_CRG_ENABLE
    U_PERI_CRG33 unTempValue;
#endif
    /* reset the value of register before reseting */
    u32AXIConfig = g_pstPngReg->u32AXIConfig;
    u32TimeOut = g_pstPngReg->u32TimeOut;
    u32RdcPhyaddr = g_pstPngReg->u32RdcStAddr;
    u32ErrMode = g_pstPngReg->uErrMode.u32All;
    u32IntMask = g_pstPngReg->uIntMask.u32All;

#ifdef CONFIG_PNG_USE_SDK_CRG_ENABLE
    unTempValue.u32 = g_pstRegCrg->PERI_CRG33.u32;
    /*reset*/
    unTempValue.bits.pgd_srst_req = 0x1;

    g_pstRegCrg->PERI_CRG33.u32 = unTempValue.u32;
#else
    *g_pu32RegReset |= 0x10;
#endif

    /*make sure reset is effective*/
    for (i = 0; i < 100; i++)
    {
    }


#ifdef CONFIG_PNG_USE_SDK_CRG_ENABLE
    unTempValue.u32 = g_pstRegCrg->PERI_CRG33.u32;
    /*cancel reset*/
    unTempValue.bits.pgd_srst_req = 0x0;
    g_pstRegCrg->PERI_CRG33.u32 = unTempValue.u32;
#else
    *g_pu32RegReset &= ~0x10;
#endif
    /* recover value */
    g_pstPngReg->u32AXIConfig = u32AXIConfig;
    g_pstPngReg->u32TimeOut = u32TimeOut;
    g_pstPngReg->u32RdcStAddr = u32RdcPhyaddr;
    g_pstPngReg->uErrMode.u32All = u32ErrMode;
    g_pstPngReg->uIntMask.u32All = u32IntMask;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
        g_pstPngReg->uMmuBypass.stBits.u32RdcRdMmuBypass = 0x0;
        g_pstPngReg->uMmuBypass.stBits.u32RdcWtMmuBypass = 0x0;
#else
        g_pstPngReg->uMmuBypass.stBits.u32RdcRdMmuBypass = 0x1;
        g_pstPngReg->uMmuBypass.stBits.u32RdcWtMmuBypass = 0x1;
#endif
#endif

    return;
}

HI_VOID PngHalSetSmmuClock(HI_BOOL bEnable)
{
#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_S32 s32Cnt = 0;
    U_PERI_CRG210 unTempValue;

    if (bEnable)
    {
        unTempValue.u32 = g_pstRegCrg->PERI_CRG210.u32;
        unTempValue.bits.pgd_smmu_cken = 0x1;
        g_pstRegCrg->PERI_CRG210.u32 = unTempValue.u32;
        mb();

        do
        {
            s32Cnt++;
            unTempValue.u32 = g_pstRegCrg->PERI_CRG210.u32;
            if (0x1 == unTempValue.bits.pgd_smmu_cken)
            {
                break;
            }
        }while(s32Cnt < 10);

        unTempValue.u32 = g_pstRegCrg->PERI_CRG210.u32;
        unTempValue.bits.pgd_smmu_srst_req = 0x0;
        g_pstRegCrg->PERI_CRG210.u32 = unTempValue.u32;
        mb();
    }
    else
    {
        HI_GFX_DeinitSmmu();
        udelay(1);

        unTempValue.u32 = g_pstRegCrg->PERI_CRG210.u32;
        unTempValue.bits.pgd_smmu_srst_req = 0x1;
        g_pstRegCrg->PERI_CRG210.u32 = unTempValue.u32;
        mb();

        unTempValue.u32 = g_pstRegCrg->PERI_CRG210.u32;
        unTempValue.bits.pgd_smmu_cken = 0x0;
        mb();
    }
#endif

    return;
}

HI_VOID PngHalSetClock(HI_BOOL bEnable)
{
#ifdef CONFIG_PNG_USE_SDK_CRG_ENABLE
    HI_S32 s32Cnt = 0;
    HI_U32 u32StartTimeMs = 0;
    HI_U32 u32EndTimeMs   = 0;
    HI_U32 u32TotalTime   = 0;
    U_PERI_CRG33 unTempValue;

    PngHalSetSmmuClock(bEnable);

    if (bEnable)
    {
        unTempValue.u32 = g_pstRegCrg->PERI_CRG33.u32;
        unTempValue.bits.pgd_cken = 0x1;
        g_pstRegCrg->PERI_CRG33.u32 = unTempValue.u32;
        mb();

        do
        {
            s32Cnt++;
            unTempValue.u32 = g_pstRegCrg->PERI_CRG33.u32;
            if (0x1 == unTempValue.bits.pgd_cken)
            {
                break;
            }
        }while(s32Cnt < 10);

        unTempValue.u32 = g_pstRegCrg->PERI_CRG33.u32;
        unTempValue.bits.pgd_srst_req = 0x1;
        g_pstRegCrg->PERI_CRG33.u32 = unTempValue.u32;
        mb();

        HI_GFX_GetTimeStamp(&u32StartTimeMs,NULL);
        while (g_pstPngReg->u32RstBusy & 0x1)
        {
            HI_GFX_GetTimeStamp(&u32EndTimeMs,NULL);
            u32TotalTime = u32EndTimeMs - u32StartTimeMs;
            if (u32TotalTime >= 20)
            {
              break;
            }
        }

        unTempValue.u32 = g_pstRegCrg->PERI_CRG33.u32;
        unTempValue.bits.pgd_srst_req = 0x0;
        g_pstRegCrg->PERI_CRG33.u32 = unTempValue.u32;
        mb();

        udelay(1);
        HI_GFX_InitSmmu((HI_U32)(PNG_REG_BASEADDR + 0xf000));
    }
    else
    {
        unTempValue.u32 = g_pstRegCrg->PERI_CRG33.u32;
        unTempValue.bits.pgd_srst_req = 0x1;
        g_pstRegCrg->PERI_CRG33.u32 = unTempValue.u32;
        mb();

        if (0 == unTempValue.bits.pgd_cken)
        {
            return;
        }
        HI_GFX_GetTimeStamp(&u32StartTimeMs,NULL);
        while (g_pstPngReg->u32RstBusy & 0x1)
        {
            HI_GFX_GetTimeStamp(&u32EndTimeMs,NULL);
            u32TotalTime = u32EndTimeMs - u32StartTimeMs;
            if (u32TotalTime >= 20)
            {
               break;
            }
        }

        unTempValue.u32 = g_pstRegCrg->PERI_CRG33.u32;
        unTempValue.bits.pgd_cken = 0x0;
        //g_pstRegCrg->PERI_CRG33.u32 = unTempValue.u32;
        mb();
    }

#else
    /* open clock*/
    *g_pu32RegReset |= 0x100;
    /* cancel reset*/
    *g_pu32RegReset &= 0xffffffef;
#endif
    return;
}

/********************************************************************************************
* func:    set basic info of picture
* in:    stImgInfo structure of picture
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetImgInfo(HI_PNG_INFO_S stImgInfo)
{
    PNG_SIZE_U uSize;
    PNG_TYPE_U uType;

    uSize.u32All = g_pstPngReg->uSize.u32All;
    uType.u32All = g_pstPngReg->uType.u32All;

    uSize.stBits.u32Width = stImgInfo.u32Width;
    uSize.stBits.u32Height = stImgInfo.u32Height;
    uType.stBits.u32BitDepth = stImgInfo.u8BitDepth;
    uType.stBits.u32ColorFmt = stImgInfo.eColorFmt;
    uType.stBits.u32InterlaceType = 0x0;//stImgInfo.u8InterlaceType;

    g_pstPngReg->uSize.u32All = uSize.u32All;
    g_pstPngReg->uType.u32All = uType.u32All;

    return;
}

/********************************************************************************************
* func:    set data convert
* in:    stTransform structrue of data convert
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetTransform(HI_PNG_TRANSFORM_S stTransform)
{
    PNG_TRANSFORM_U uTransform;
    PNG_TRANS_COLOR0_U uTransColor0;
    PNG_TRANS_COLOR0_U uTransColor1;

    uTransform.u32All = g_pstPngReg->uTransform.u32All;
    uTransColor0.u32All = g_pstPngReg->uTransColor1.u32All;
    uTransColor1.u32All = g_pstPngReg->uTransColor1.u32All;

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_ADJUSTPIXELORDER)
    {
        uTransform.stBits.u32PixelOrderEn = HI_TRUE;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_GRAY124TO8)
    {
        uTransform.stBits.u32PackByteEn = HI_TRUE;
        uTransform.stBits.u32PackBypeMode = stTransform.e124To8Mode;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_GRAY2RGB)
    {
        uTransform.stBits.u32Gray2BGREn = HI_TRUE;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_ADDALPHA)
    {
        uTransform.stBits.u32AddAlphaEn = HI_TRUE;
        uTransColor1.u32All = ((HI_U32)stTransform.u16Filler << 16) | stTransform.sTrnsInfo.u16Blue;
        uTransColor0.u32All = ((HI_U32)stTransform.sTrnsInfo.u16Red << 16) | stTransform.sTrnsInfo.u16Green;
        uTransform.stBits.u32TransInfoEn = stTransform.eAddAlphaMode;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_STRIPALPHA)
    {
        uTransform.stBits.u32StripAlphaEn = HI_TRUE;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_BGR2RGB)
    {
        uTransform.stBits.u32BGR2RGBEn = HI_TRUE;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_SWAPALPHA)
    {
        uTransform.stBits.u32SwapAlphaEn = HI_TRUE;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_16TO8)
    {
        uTransform.stBits.u32Strip16En = HI_TRUE;
        uTransform.stBits.u32Streip16Mode = stTransform.e16To8Mode;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_8TO4)
    {
        uTransform.stBits.u32Strip4En = HI_TRUE;
        uTransform.stBits.u32Strip4Fmt = stTransform.eOutFmt;
    }

    if (stTransform.u32Transform & HI_PNG_TRANSFORM_PREMULTI_ALPHA)
    {
        #ifdef CONFIG_PNG_PREMULTIALPHA_ENABLE
            uTransform.stBits.u32PreMultiAlphaEn = HI_TRUE;
            uTransform.stBits.u32PreMultiRoundMode = 0x1;
         #endif
    }

    g_pstPngReg->uTransform.u32All = uTransform.u32All;
    g_pstPngReg->uTransColor0.u32All = uTransColor0.u32All;
    g_pstPngReg->uTransColor1.u32All = uTransColor1.u32All;

    return;
}

/********************************************************************************************
* func:    set rle window physics address
* in:    u32Phyaddr: physical address
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetRdcAddr(HI_U32 u32Phyaddr)
{
    g_pstPngReg->u32RdcStAddr = u32Phyaddr;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
        g_pstPngReg->uMmuBypass.stBits.u32RdcRdMmuBypass = 0x0;
        g_pstPngReg->uMmuBypass.stBits.u32RdcWtMmuBypass = 0x0;
#else
        g_pstPngReg->uMmuBypass.stBits.u32RdcRdMmuBypass = 0x1;
        g_pstPngReg->uMmuBypass.stBits.u32RdcWtMmuBypass = 0x1;
#endif
#endif

    return;
}

/********************************************************************************************
* func:    get rle window physics address
* in:    u32Phyaddr: physical address
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_U32 PngHalGetRdcAddr(HI_VOID)
{
    return g_pstPngReg->u32RdcStAddr;
}


/********************************************************************************************
* func:    set filter buffer starting address and ending address
* in:    u32Phyaddr: physical address
* in:    u32Size: buf size
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetFltAddr(HI_U32 u32Phyaddr, HI_U32 u32Size)
{
    g_pstPngReg->u32FltStAddr = u32Phyaddr;
    g_pstPngReg->u32FltEndAddr = u32Phyaddr + u32Size - 1;

#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
        g_pstPngReg->uMmuBypass.stBits.u32FilterRdMmuBypass = 0x0;
        g_pstPngReg->uMmuBypass.stBits.u32FilterWtMmuBypass = 0x0;
#else
        g_pstPngReg->uMmuBypass.stBits.u32FilterRdMmuBypass = 0x1;
        g_pstPngReg->uMmuBypass.stBits.u32FilterWtMmuBypass = 0x1;
#endif
#endif
    return;
}

/********************************************************************************************
* func:    set target buffer
* in:    u32Phyaddr physical address
* in:    u32Stride: target buffer line step
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetTgt(HI_U32 u32Phyaddr, HI_U32 u32Stride)
{
    g_pstPngReg->u32FinalStAddr = u32Phyaddr;
    g_pstPngReg->u32Stride = u32Stride;

#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
        g_pstPngReg->uMmuBypass.stBits.u32OutMmuBypass = 0x0;
#else
        g_pstPngReg->uMmuBypass.stBits.u32OutMmuBypass = 0x1;
#endif
#endif
    return;
}

/********************************************************************************************
* func:    set stream buffer address
* in:    u32Phyaddr physical address
* in:    u32Size buf size
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetStreamBuf(HI_U32 u32Phyaddr, HI_U32 u32Size)
{
    g_pstPngReg->u32BitBufStAddr = u32Phyaddr;
    g_pstPngReg->u32BitBufEndAddr = u32Phyaddr + u32Size - 1;

#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
        g_pstPngReg->uMmuBypass.stBits.u32StreamMmuBypass = 0x0;
#else
        g_pstPngReg->uMmuBypass.stBits.u32StreamMmuBypass = 0x1;
#endif
#endif

    return;
}

/********************************************************************************************
* func:    set stream address
* in:    u32Phyaddr physical address
* in:    u32Size stream size
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetStreamAddr(HI_U32 u32Phyaddr, HI_U32 u32Size)
{
    g_pstPngReg->u32BitStreamStAddr = u32Phyaddr;
    g_pstPngReg->u32BitStreamEndAddr = u32Phyaddr + u32Size - 1;
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
#ifdef CONFIG_GFX_MMU_SUPPORT
        g_pstPngReg->uMmuBypass.stBits.u32StreamMmuBypass = 0x0;
#else
        g_pstPngReg->uMmuBypass.stBits.u32StreamMmuBypass = 0x1;
#endif
#endif

    return;
}

/********************************************************************************************
* func:    start decode
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalStartDecode(HI_VOID)
{
    PNG_TYPE_U uType;
    uType.u32All = g_pstPngReg->uType.u32All;
    uType.stBits.u32FunSel = 0x0;
    g_pstPngReg->uType.u32All = uType.u32All;
    g_pstPngReg->u32DecStart = 0x1;

    return;
}

/********************************************************************************************
* func:    send stream resuing decoder
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalResumeDecode(HI_VOID)
{
    g_pstPngReg->u32ResumeStart = 0x1;

    return;
}

/********************************************************************************************
* func:    read interupt state and clear interupt
* in:    none
* out:    interupt value
* ret:    none
* others:
*********************************************************************************************/
HI_U32 PngHalGetIntStatus(HI_VOID)
{
    HI_U32 u32Int;
    HI_S32 s32Ret = HI_SUCCESS;

    u32Int = g_pstPngReg->uInt.u32All;

    /* clear interupt*/
    g_pstPngReg->uInt.u32All &= u32Int;

    s32Ret = HI_GFX_SmmuIsr("HI_MOD_PNG");
    if(HI_SUCCESS != s32Ret){
        u32Int = PNG_INT_ERR_MASK;
    }

    return u32Int;
}

/********************************************************************************************
* func:    set AXI port and timeout
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetAxiAndTimeout(HI_VOID)
{
    /* config AXI */
    g_pstPngReg->u32AXIConfig = 0x20441;

    /* config port and timeout*/
    g_pstPngReg->u32TimeOut = 0x80008;

    return;
}

/********************************************************************************************
* func:    set interrupt mask
* in:    u32Value mask value
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetIntmask(HI_U32 u32Value)
{
    g_pstPngReg->uIntMask.u32All = u32Value;

    return;
}

/********************************************************************************************
* func:    set error strategy
* in:    u32Value mode of error strategy
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetErrmode(HI_U32 u32Value)
{
    g_pstPngReg->uErrMode.u32All = u32Value;

    return;
}

HI_U32 PngHalGetIrqNum()
{
    return g_u32PngIrqNum;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cplusplus */
#endif    /* __cplusplus */
