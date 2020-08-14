/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : drv_hifb_hal.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      :
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/


/***************************** add include here*********************************/

#include "hi_debug.h"

#include "drv_hifb_adp.h"
#include "drv_hifb_hal.h"

#ifdef HI_BUILD_IN_BOOT
#include "hifb_debug.h"
#include "hi_common.h"
#endif

/**
 **ZME水平方向行和列配置
 **/
#include "drv_hifb_config.h"

#include "hi_reg_common.h"


/***************************** Macro Definition ********************************/

/** hifone已经将这个规格裁剪了 **/

#define STRIDE_ALIGN 16


#define OPTM_HAL_CHECK_NULL_POINT(p) do\
{\
 if (HI_NULL == p)\
    {\
        HIFB_ERROR("can't handle null point.\n");\
        return;\
    }\
}while(0)

#define OPTM_HAL_CHECK_LAYER_VALID(u32Data) do\
 {\
  if (u32Data >= OPTM_GFX_MAX)\
     {\
         HIFB_ERROR("Select Wrong Graph Layer ID\n");\
         return;\
     }\
 }while(0)


/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/

volatile S_VOU_CV200_REGS_TYPE* pOptmVdpReg = NULL;

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
volatile OPTM_MDDRC_REGS_S *pMddrcReg       = NULL;
#endif



/***************************** API forward declarations ************************/
static inline HI_VOID OPTM_VDP_GP_SetHorizontalZmeFirEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
static inline HI_VOID OPTM_VDP_GP_SetVerticalZmeFirEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
static inline HI_VOID OPTM_VDP_GP_SetHorizontalZmeMidEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
static inline HI_VOID OPTM_VDP_GP_SetVerticalZmeMidEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable);
static inline HI_VOID OPTM_VDP_GP_SetChn0IpOrder(HI_U32 u32Data, OPTM_VDP_GP_ORDER_E enIpOrder);
static inline HI_VOID OPTM_VDP_GP_SetChn1IpOrder(HI_U32 u32Data, OPTM_VDP_GP_ORDER_E enIpOrder);




/***************************** API realization *********************************/


/***************************************************************************************
* func          : OPTM_VDP_DRIVER_Initial
* description : CNcomment: hal层保存寄存器相关信息 CNend\n
* param[in]      : HI_VOID
* retval          : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_DRIVER_Initial(volatile HI_U32* virAddr)
{
    pOptmVdpReg = (S_VOU_CV200_REGS_TYPE*)virAddr;
}

#ifndef HI_BUILD_IN_BOOT
HI_VOID  OPTM_MDDRC_DRIVER_Initial(HI_U32 virAddr)
{
    #ifdef CFG_HIFB_COMPRESSION_SUPPORT
        pMddrcReg = (OPTM_MDDRC_REGS_S*)virAddr;
    #endif
}
#endif

HI_U32 OPTM_VDP_RegRead(HI_U32* a)
{
   return (*a);
}

HI_VOID OPTM_VDP_RegWrite(HI_U32* a, HI_U32 b)
{
    *a = b;
}

//--------------------------------------------------------------------
// 这两个地方在98C上是屏蔽的，但是有支持挂接所以这个地方的代码
// 和前面有矛盾
//--------------------------------------------------------------------
/***************************************************************************************
* func          : OPTM_VDP_SetLayerConnect
* description   : CNcomment: 设置图形链接到高清通道还是标清通道 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_SetLayerConnect(HI_U32 u32Data)
{
#ifdef CONFIG_HIFB_GFX3_TO_GFX5
    volatile U_VOCTRL VOCTRL;
    VOCTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->VOCTRL.u32)));
    VOCTRL.bits.g3_dhd1_sel = u32Data;
    OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->VOCTRL.u32)), VOCTRL.u32);
#endif
}

/***************************************************************************************
* func          : OPTM_VDP_OpenGFX3
* description   : CNcomment: 打开G3 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_OpenGFX3(HI_BOOL bOpen)
{
#ifdef CONFIG_HIFB_GFX3_TO_GFX5
    volatile U_CBM_ATTR CBM_ATTR;
    CBM_ATTR.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CBM_ATTR.u32)));
    if (bOpen)
    {
        CBM_ATTR.bits.sur_attr5 = 1;  /*g3 to mixer2*/
        CBM_ATTR.bits.sur_attr4 = 0;  /*v4 to mixer1*/
    }
    else
    {
        CBM_ATTR.bits.sur_attr5 = 0;  /*g3 to mixer1*/
        CBM_ATTR.bits.sur_attr4 = 1;  /*v4 to mixer2*/
    }
    OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CBM_ATTR.u32)), CBM_ATTR.u32);
#endif

}

/***************************************************************************************
* func          : OPTM_VDP_CBM_SetMixerBkg
* description   : CNcomment: 设置MIXG0叠加背景色寄存器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_CBM_SetMixerBkg(OPTM_VDP_CBM_MIX_E u32mixer_id, OPTM_VDP_BKG_S stBkg)
{

    volatile U_MIXG0_BKG MIXG0_BKG;
    volatile U_MIXG0_BKALPHA MIXG0_BKALPHA;

    if(u32mixer_id == VDP_CBM_MIXG0)
    {
        /* G0 mixer link */
        MIXG0_BKG.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_BKG.u32)));;
        MIXG0_BKG.bits.mixer_bkgy  = stBkg.u32BkgY;
        MIXG0_BKG.bits.mixer_bkgcb = stBkg.u32BkgU;
        MIXG0_BKG.bits.mixer_bkgcr = stBkg.u32BkgV;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_BKG.u32)), MIXG0_BKG.u32);
        /**
         **MIXG0采用SRC OVER算法时背景色的像素ALPHA寄存器。
         **/
        MIXG0_BKALPHA.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_BKALPHA.u32)));;
        MIXG0_BKALPHA.bits.mixer_alpha  = stBkg.u32BkgA;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_BKALPHA.u32)), MIXG0_BKALPHA.u32);
    }
    else if(u32mixer_id == VDP_CBM_MIXG1)
    {
        return ;
    }
    else
    {
        HIFB_ERROR("Select Wrong mixer ID\n");
    }
    return ;
}

HI_VOID OPTM_VDP_CBM_GetMixerPrio(OPTM_VDP_CBM_MIX_E u32mixer_id, HI_U32 *pu32prio)
{
    OPTM_HAL_CHECK_NULL_POINT(pu32prio);

    if (u32mixer_id == VDP_CBM_MIXG0)
    {
        *pu32prio = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)));
    }
    else
    {
        *pu32prio= 0;
    }
}

/***************************************************************************************
* func          : OPTM_VDP_SetMixgPrio
* description   : CNcomment: MIXG0优先级配置寄存器。在vsync处更新有效。
                             mixer_prio_x表示第x个优先级所配置的层。
                             该寄存器为即时寄存器。CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_SetMixgPrio(OPTM_VDP_CBM_MIX_E u32mixer_id,HI_U32 u32prio)
{
    volatile U_MIXG0_MIX MIXG0_MIX;

    if (u32mixer_id == VDP_CBM_MIXG0)
    {
        MIXG0_MIX.u32 = u32prio;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
    }
}

/***************************************************************************************
* func          : OPTM_VDP_CBM_SetMixerPrio
* description   : CNcomment: 设置图形层叠加优先级，Z序 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_CBM_SetMixerPrio(OPTM_VDP_CBM_MIX_E u32mixer_id,HI_U32 u32layer_id,HI_U32 u32prio)
{
    volatile U_MIXG0_MIX MIXG0_MIX;

    if (u32mixer_id == VDP_CBM_MIXG0)
    {
        switch(u32prio)
        {
            case 0:
            {
                MIXG0_MIX.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio0 = u32layer_id+1;
                OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 1:
            {
                MIXG0_MIX.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio1 = u32layer_id+1;
                OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 2:
            {
                MIXG0_MIX.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio2 = u32layer_id+1;
                OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            case 3:
            {
                MIXG0_MIX.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)));
                MIXG0_MIX.bits.mixer_prio3 = u32layer_id+1;
                OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->MIXG0_MIX.u32)), MIXG0_MIX.u32);
                break;
            }
            default:
            {
                HIFB_ERROR("select wrong layer ID\n");
                return ;
            }
        }
    }
}


//-------------------------------------------------------------------
//GFX_BEGIN
//-------------------------------------------------------------------


/***************************************************************************************
* func          : OPTM_VDP_GFX_GetLayerEnable
* description   : CNcomment: 从寄存器中获取图层是否使能数据 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#ifndef HI_BUILD_IN_BOOT
HI_VOID  OPTM_VDP_GFX_GetLayerEnable(HI_U32 u32Data, HI_U32 *pu32Enable )
{
    volatile U_G0_CTRL GFX_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    OPTM_HAL_CHECK_NULL_POINT(pu32Enable);

    GFX_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));

    *pu32Enable  = GFX_CTRL.bits.surface_en;
    return ;
}
#endif


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetLayerEnable
* description   : CNcomment: 设置图层使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetLayerEnable(HI_U32 u32Data, HI_U32 u32bEnable )
{
    volatile U_G0_CTRL G0_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CTRL.bits.surface_en = u32bEnable;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CTRL.u32);

    return ;
}


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetNoSecFlag
* description   : CNcomment: 设置DDR模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetNoSecFlag    (HI_U32 u32Data, HI_U32 u32Enable )
{
    /** 要是安全DDR，这里就要设置成安全的了，和高安有关系 **/
    volatile U_G0_CTRL  G0_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CTRL.bits.nosec_flag= u32Enable ;/** question 为什么是非安全得 **/
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CTRL.u32);

    return ;
}



/***************************************************************************************
* func          : OPTM_VDP_GFX_SetDcmpEnable
* description   : CNcomment: 配置解压寄存器，和压缩有关 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetDcmpEnable(HI_U32 u32Data, HI_U32 u32bEnable)
{
#ifdef CFG_HI_FB_DECOMPRESS_SUPPORT
    volatile U_G0_CTRL G0_CTRL;
    OPTM_HAL_CHECK_LAYER_VALID(u32Data);
    G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CTRL.bits.dcmp_en = u32bEnable;
    G0_CTRL.bits.lossless   = 1; /** 数据无损压缩 **/
    G0_CTRL.bits.lossless_a = 1; /** a无损压缩    **/
    G0_CTRL.bits.cmp_mode   = 0; /** RAM模式      **/
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CTRL.u32);
#endif
    return;
}


#ifndef HI_BUILD_IN_BOOT
HI_VOID OPTM_VDP_GFX_SetDcmpAddr(HI_U32 u32Data, HI_U32 u32LAddr)
{
    volatile U_G0_DCMP_ADDR G0_DCMP_ADDR;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_DCMP_ADDR.u32 = u32LAddr;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_ADDR.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_DCMP_ADDR.u32);
}

HI_VOID OPTM_VDP_GFX_SetDcmpNAddr(HI_U32 u32Data, HI_U32 u32NAddr)
{
    volatile U_G0_DCMP_NADDR G0_DCMP_NADDR;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_DCMP_NADDR.u32 = u32NAddr;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_NADDR.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_DCMP_NADDR.u32);
}
#endif

HI_VOID OPTM_VDP_GFX_SetDcmpOffset(HI_U32 u32Data, HI_U32 u32Offset)
{
    volatile U_G0_DCMP_OFFSET G0_DCMP_OFFSET;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_DCMP_OFFSET.u32 = u32Offset;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_OFFSET.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_DCMP_OFFSET.u32);
}


/***************************************************************************************
* func          : OPTM_VDP_GFX_GetLayerAddr
* description   : CNcomment: 获取surface 帧buffer地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#ifndef HI_BUILD_IN_BOOT
HI_VOID OPTM_VDP_GFX_GetLayerAddr(HI_U32 u32Data, HI_U32 *pu32Addr)
{
    volatile U_G0_ADDR GFX_ADDR;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    OPTM_HAL_CHECK_NULL_POINT(pu32Addr);

    GFX_ADDR.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_ADDR.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));

    *pu32Addr = GFX_ADDR.u32;

    return;
}
#endif


/***************************************************************************************
* func            : OPTM_VDP_GFX_SetLayerAddrEX
* description    : CNcomment: 设置显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetLayerAddrEX(HI_U32 u32Data, HI_U32 u32LAddr)
{
    volatile U_G0_ADDR G0_ADDR;
#ifdef CONFIG_HIFB_MMU_SUPPORT
    volatile U_G0_SMMU_BYPASS0 G0_SMMU_BYPASS0;
#endif

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_ADDR.u32 = u32LAddr;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_ADDR.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_ADDR.u32);

#ifdef CONFIG_HIFB_MMU_SUPPORT
    #ifdef CONFIG_GFX_MMU_SUPPORT
        G0_SMMU_BYPASS0.bits.bypass_2d   = 0;
        G0_SMMU_BYPASS0.bits.r_bypass_3d = 0;
    #else
        G0_SMMU_BYPASS0.bits.bypass_2d   = 1;
        G0_SMMU_BYPASS0.bits.r_bypass_3d = 1;
    #endif
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_SMMU_BYPASS0.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_SMMU_BYPASS0.u32);
#endif
}

/***************************************************************************************
* func          : OPTM_VDP_GFX_GetLayerStride
* description   : CNcomment: 获取stride CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#ifndef HI_BUILD_IN_BOOT
HI_VOID OPTM_VDP_GFX_GetLayerStride(HI_U32 u32Data, HI_U32 *pu32Stride)
{
    volatile U_G0_STRIDE GFX_STRIDE;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    GFX_STRIDE.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_STRIDE.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *pu32Stride = GFX_STRIDE.bits.surface_stride * STRIDE_ALIGN;

    return;
}
#endif


/***************************************************************************************
* func          : OPTM_GfxSetLayerStride
* description   : CNcomment: 设置图层stride CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetLayerStride(HI_U32 u32Data, HI_U32 u32Stride)
{
    volatile U_G0_STRIDE G0_STRIDE;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);
    G0_STRIDE.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_STRIDE.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_STRIDE.bits.surface_stride = u32Stride/STRIDE_ALIGN;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_STRIDE.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_STRIDE.u32);
}


HI_VOID  DRV_HAL_SetDeCmpDdrInfo(HI_U32 u32Data, HI_U32 ARHeadDdr,HI_U32 ARDataDdr,HI_U32 GBHeadDdr,HI_U32 GBDataDdr,HI_U32 DeCmpStride)
{
#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT) && !defined(CONFIG_HIFB_CALCSTRIDE_WITH_ONEPOINTTHREE)
      if (0 == u32Data)
      {
         OPTM_VDP_GFX_SetDcmpAddr     (u32Data,ARHeadDdr);
         OPTM_VDP_GFX_SetLayerAddrEX  (u32Data,ARDataDdr);
         OPTM_VDP_GFX_SetDcmpNAddr    (u32Data,GBHeadDdr);
         OPTM_VDP_GFX_SetLayerNAddr   (u32Data,GBDataDdr);
         OPTM_VDP_GFX_SetLayerStride  (u32Data,DeCmpStride);
      }
#else
      UNUSED(u32Data);
      UNUSED(ARHeadDdr);
      UNUSED(ARDataDdr);
      UNUSED(GBHeadDdr);
      UNUSED(GBDataDdr);
      UNUSED(DeCmpStride);
#endif
}


HI_VOID  DRV_HAL_GetDeCmpStatus(HI_U32 u32Data, DRV_HIFB_DECMPSTATUS_S *pstDeCmpStatus)
{
#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT) && !defined(CONFIG_HIFB_CALCSTRIDE_WITH_ONEPOINTTHREE)
    U_G0_DCMP_INTER     G0_DCMP_INTER;
    U_G0_DCMP_AR_DBG_0  G0_DCMP_AR_DBG_0;
    U_G0_DCMP_AR_DBG_1  G0_DCMP_AR_DBG_1;
    U_G0_DCMP_GB_DBG_0  G0_DCMP_GB_DBG_0;
    U_G0_DCMP_GB_DBG_1  G0_DCMP_GB_DBG_1;

    if (0 != u32Data)
    {
        return;
    }

    G0_DCMP_INTER.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_INTER.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    if (1 == G0_DCMP_INTER.bits.dcmp_ar_errclr)
    {
        pstDeCmpStatus->bDeCmpARDataErr = HI_TRUE;
    }

    if (1 == G0_DCMP_INTER.bits.dcmp_gb_errclr)
    {
        pstDeCmpStatus->bDeCmpGBDataErr = HI_TRUE;
    }

    G0_DCMP_AR_DBG_0.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_AR_DBG_0.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_DCMP_AR_DBG_1.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_AR_DBG_1.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_DCMP_GB_DBG_0.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_GB_DBG_0.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_DCMP_GB_DBG_1.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_GB_DBG_1.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));

    pstDeCmpStatus->DeCmpARDataStatus[0] = G0_DCMP_AR_DBG_0.u32;
    pstDeCmpStatus->DeCmpARDataStatus[1] = G0_DCMP_AR_DBG_1.u32;
    pstDeCmpStatus->DeCmpGBDataStatus[0] = G0_DCMP_GB_DBG_0.u32;
    pstDeCmpStatus->DeCmpGBDataStatus[1] = G0_DCMP_GB_DBG_1.u32;

    G0_DCMP_INTER.bits.dcmp_ar_errclr = 1;
    G0_DCMP_INTER.bits.dcmp_gb_errclr = 1;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DCMP_INTER.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_DCMP_INTER.u32);
#else
    UNUSED(u32Data);
#endif
    return;
}

/***************************************************************************************
* func          : OPTM_VDP_GFX_SetLutAddr
* description   : CNcomment: 设置调色板地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetLutAddr(HI_U32 u32Data, HI_U32 u32LutAddr)
{
    volatile U_G0_PARAADDR G0_PARAADDR;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_PARAADDR.u32 = u32LutAddr;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_PARAADDR.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_PARAADDR.u32);
}

#ifndef HI_BUILD_IN_BOOT
HI_VOID OPTM_VDP_GFX_GetInDataFmt(HI_U32 u32Data, OPTM_VDP_GFX_IFMT_E  *enDataFmt)
{
    volatile U_G0_CTRL G0_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *enDataFmt = G0_CTRL.bits.ifmt ;

    return ;
}
#endif


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetInDataFmt
* description   : CNcomment: 输入数据格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetInDataFmt(HI_U32 u32Data, OPTM_VDP_GFX_IFMT_E  enDataFmt)
{
    /**
     **输入数据格式
     **/
    volatile U_G0_CTRL G0_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CTRL.bits.ifmt = enDataFmt;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CTRL.u32);

    return ;
}


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetBitExtend
* description   : CNcomment: 设置低位扩展模式
                             所谓低位扩展就是比如ARGB1555,R少了3位，这三位如何补齐
                             是用高一位还高几位还是使用0来补齐 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetBitExtend(HI_U32 u32Data, OPTM_VDP_GFX_BITEXTEND_E u32mode)
{
    volatile U_G0_CTRL G0_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CTRL.bits.bitext = u32mode;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CTRL.u32);
}

/***************************************************************************************
* func          : OPTM_VDP_GFX_SetColorKey
* description   : CNcomment:设置color key的值CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetColorKey(HI_U32 u32Data,HI_U32  bkeyEn,OPTM_VDP_GFX_CKEY_S stKey )
{
    volatile U_G0_CKEYMAX G0_CKEYMAX;
    volatile U_G0_CKEYMIN G0_CKEYMIN;
    volatile U_G0_CBMPARA G0_CBMPARA;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CKEYMAX.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMAX.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CKEYMAX.bits.keyr_max = stKey.u32Key_r_max;
    G0_CKEYMAX.bits.keyg_max = stKey.u32Key_g_max;
    G0_CKEYMAX.bits.keyb_max = stKey.u32Key_b_max;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMAX.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CKEYMAX.u32);

    G0_CKEYMIN.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMIN.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CKEYMIN.bits.keyr_min = stKey.u32Key_r_min;
    G0_CKEYMIN.bits.keyg_min = stKey.u32Key_g_min;
    G0_CKEYMIN.bits.keyb_min = stKey.u32Key_b_min;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMIN.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CKEYMIN.u32);

    /**
     ** color key模式
     ** 0：满足Keymin <= Pixel <= Keymax时，处理为关键色；
     ** 1：满足Pixel  <= Keymin 或者 Pixel >= Keymax时处理为关键色 。
     **/
    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CBMPARA.bits.key_en = bkeyEn;
    G0_CBMPARA.bits.key_mode = stKey.bKeyMode;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CBMPARA.u32);

}

#ifndef HI_BUILD_IN_BOOT
HI_VOID OPTM_VDP_GFX_GetColorKey(HI_U32 u32Data, HI_U32 *pbkeyEn,OPTM_VDP_GFX_CKEY_S *pstKey)
{
    volatile U_G0_CKEYMAX G0_CKEYMAX;
    volatile U_G0_CKEYMIN G0_CKEYMIN;
    volatile U_G0_CBMPARA G0_CBMPARA;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CKEYMAX.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMAX.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    pstKey->u32Key_r_max = G0_CKEYMAX.bits.keyr_max;
    pstKey->u32Key_g_max = G0_CKEYMAX.bits.keyg_max;
    pstKey->u32Key_b_max = G0_CKEYMAX.bits.keyb_max;

    G0_CKEYMIN.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMIN.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    pstKey->u32Key_r_min = G0_CKEYMIN.bits.keyr_min;
    pstKey->u32Key_g_min = G0_CKEYMIN.bits.keyg_min;
    pstKey->u32Key_b_min = G0_CKEYMIN.bits.keyb_min;

    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *pbkeyEn = G0_CBMPARA.bits.key_en;
    pstKey->bKeyMode = G0_CBMPARA.bits.key_mode;
}
#endif


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetKeyMask
* description   : CNcomment:设置掩码值，将color key的实际值与掩码值做与操作，这样就可以
                            key掉一个范围值，而不只单个值CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetKeyMask(HI_U32 u32Data, OPTM_VDP_GFX_MASK_S stMsk)
{
    volatile U_G0_CMASK G0_CMASK;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CMASK.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CMASK.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CMASK.bits.kmsk_r = stMsk.u32Mask_r;
    G0_CMASK.bits.kmsk_g = stMsk.u32Mask_g;
    G0_CMASK.bits.kmsk_b = stMsk.u32Mask_b;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CMASK.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CMASK.u32);

}

#ifndef HI_BUILD_IN_BOOT
HI_VOID OPTM_VDP_GFX_GetKeyMask(HI_U32 u32Data, OPTM_VDP_GFX_MASK_S *pstMsk)
{
    volatile U_G0_CMASK G0_CMASK;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CMASK.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CMASK.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    pstMsk->u32Mask_r = G0_CMASK.bits.kmsk_r;
    pstMsk->u32Mask_g = G0_CMASK.bits.kmsk_g;
    pstMsk->u32Mask_b = G0_CMASK.bits.kmsk_b;
}
#endif


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetReadMode
* description   : CNcomment: 设置图层读取数据模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetReadMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    volatile U_G0_CTRL G0_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CTRL.bits.read_mode = u32Mode;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CTRL.u32);


}

HI_VOID  OPTM_VDP_GFX_SetParaUpd  (HI_U32 u32Data, OPTM_VDP_DISP_COEFMODE_E enMode )
{
    volatile U_G0_PARAUP G0_PARAUP;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    if (enMode == VDP_DISP_COEFMODE_LUT || enMode == VDP_DISP_COEFMODE_ALL)
    {
        G0_PARAUP.bits.gdc_paraup = 0x1;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_PARAUP.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_PARAUP.u32);
    }



}

//3D MODE
#ifndef HI_BUILD_IN_BOOT
HI_VOID  OPTM_VDP_GFX_SetThreeDimDofEnable    (HI_U32 u32Data, HI_U32 bEnable)
{
    volatile U_G0_DOF_CTRL  G0_DOF_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_DOF_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DOF_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_DOF_CTRL.bits.dof_en = bEnable;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DOF_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_DOF_CTRL.u32);
}

HI_VOID  OPTM_VDP_GFX_SetThreeDimDofStep(HI_U32 u32Data, HI_S32 s32LStep, HI_S32 s32RStep)
{
    volatile U_G0_DOF_STEP  G0_DOF_STEP;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_DOF_STEP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DOF_STEP.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_DOF_STEP.bits.right_step= s32RStep;
    G0_DOF_STEP.bits.left_step = s32LStep;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DOF_STEP.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_DOF_STEP.u32);
}


/***************************************************************************************
* func          : OPTM_GfxSetTriDimEnable
* description   : CNcomment: 设置3D使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetThreeDimEnable(HI_U32 u32Data,HI_U32 bTrue)
{

}


/***************************************************************************************
* func          : OPTM_VDP_GFX_GetLayerInRect
* description   : CNcomment: 获取图层的输入数据 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_GetLayerInRect(HI_U32 u32Data, OPTM_VDP_DISP_RECT_S *stInRect)
{
    U_G0_SFPOS G0_SFPOS;
    U_G0_VFPOS G0_VFPOS;
    U_G0_VLPOS G0_VLPOS;
    U_G0_DFPOS G0_DFPOS;
    U_G0_DLPOS G0_DLPOS;
    U_G0_IRESO G0_IRESO;

    G0_SFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_SFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    stInRect->u32SX = G0_SFPOS.bits.src_xfpos;

    G0_VFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_VFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    stInRect->u32VX = G0_VFPOS.bits.video_xfpos;
    stInRect->u32VY = G0_VFPOS.bits.video_yfpos;

    G0_DFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    stInRect->u32DXS = G0_DFPOS.bits.disp_xfpos;
    stInRect->u32DYS = G0_DFPOS.bits.disp_yfpos;

    G0_DLPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DLPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    stInRect->u32DXL = G0_DLPOS.bits.disp_xlpos + 1;
    stInRect->u32DYL = G0_DLPOS.bits.disp_ylpos + 1;

    G0_VLPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_VLPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    stInRect->u32VXL = G0_VLPOS.bits.video_xlpos + 1;
    stInRect->u32VYL = G0_VLPOS.bits.video_ylpos + 1;

    G0_IRESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_IRESO.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    stInRect->u32IWth = G0_IRESO.bits.iw + 1;
    stInRect->u32IHgt = G0_IRESO.bits.ih + 1;
}
#endif


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetLayerReso
* description   : CNcomment: 设置图层分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetLayerReso     (HI_U32 u32Data, OPTM_VDP_DISP_RECT_S  stRect)
{
    volatile U_G0_SFPOS G0_SFPOS;
    volatile U_G0_VFPOS G0_VFPOS;
    volatile U_G0_VLPOS G0_VLPOS;
    volatile U_G0_DFPOS G0_DFPOS;
    volatile U_G0_DLPOS G0_DLPOS;
    volatile U_G0_IRESO G0_IRESO;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    /**
     **源起始X坐标值，0为一行第一个像素。
     **S:Source
     **/
    G0_SFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_SFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_SFPOS.bits.src_xfpos = stRect.u32SX;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_SFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_SFPOS.u32);

    /**
     **Surface真实内容在显示窗口的起始位置
     **视频内容列起始坐标。
     **视频内容行起始坐标。
     **F:First
     **/
    G0_VFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_VFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_VFPOS.bits.video_xfpos = stRect.u32VX;
    G0_VFPOS.bits.video_yfpos = stRect.u32VY;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_VFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_VFPOS.u32);

    /**
     **Surface真实内容在显示窗口的结束位置
     **视频内容列结束坐标。
     **视频内容行结束坐标。
     **L:Last
     **O:Out
     **V:Video
     **/
    G0_VLPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_VLPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth;
    if (G0_VLPOS.bits.video_xlpos)
    {
        G0_VLPOS.bits.video_xlpos -= 1;
    }
    G0_VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt;
    if (G0_VLPOS.bits.video_ylpos)
    {
        G0_VLPOS.bits.video_ylpos -= 1;
    }
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_VLPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_VLPOS.u32);

    /**
     **输出，Surface在显示窗口的起始位置
     **列起始坐标。
     **行起始坐标。
     **D:Display
     **/
    G0_DFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_DFPOS.bits.disp_xfpos = stRect.u32DXS;
    G0_DFPOS.bits.disp_yfpos = stRect.u32DYS;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DFPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_DFPOS.u32);

    /**
     **输出，Surface在显示窗口的结束位置
     **列结束坐标。
     **行结束坐标。
     **D:Display
     **/
    G0_DLPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DLPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_DLPOS.bits.disp_xlpos = stRect.u32DXL;
    if (G0_DLPOS.bits.disp_xlpos)
    {
        G0_DLPOS.bits.disp_xlpos -= 1;
    }
    G0_DLPOS.bits.disp_ylpos = stRect.u32DYL;
    if (G0_DLPOS.bits.disp_ylpos)
    {
        G0_DLPOS.bits.disp_ylpos -= 1;
    }
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_DLPOS.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_DLPOS.u32);


    /**
     **输入分辨率寄存器
     **/
    G0_IRESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_IRESO.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_IRESO.bits.iw = stRect.u32IWth;
    if (G0_IRESO.bits.iw)
    {
        G0_IRESO.bits.iw -= 1;
    }
    G0_IRESO.bits.ih = stRect.u32IHgt;
    if (G0_IRESO.bits.ih)
    {
        G0_IRESO.bits.ih -= 1;
    }
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_IRESO.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_IRESO.u32);

    return ;

}

/***************************************************************************************
* func          : OPTM_VDP_GFX_SetRegUp
* description   : CNcomment: 设置图层更新使能寄存器，更新完之后硬件自动清0 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetRegUp (HI_U32 u32Data)
{
    volatile U_G0_UPD G0_UPD;

    /* GO layer register update */
    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_UPD.bits.regup = 0x1;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_UPD.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_UPD.u32);

    return ;
}


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetLayerBkg
* description   : CNcomment: 设置图层背景色 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetLayerBkg(HI_U32 u32Data, OPTM_VDP_BKG_S stBkg)
{
    volatile U_G0_BK    G0_BK;
    volatile U_G0_ALPHA G0_ALPHA;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_BK.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_BK.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_BK.bits.vbk_y  = stBkg.u32BkgY ;
    G0_BK.bits.vbk_cb = stBkg.u32BkgU;
    G0_BK.bits.vbk_cr = stBkg.u32BkgV;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_BK.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_BK.u32);

    G0_ALPHA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_ALPHA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_ALPHA.bits.vbk_alpha = stBkg.u32BkgA;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_ALPHA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_ALPHA.u32);

    return ;
}


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetLayerGalpha
* description   : CNcomment: 设置图层全局alpha CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha0)
{
    volatile U_G0_CBMPARA G0_CBMPARA;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CBMPARA.bits.galpha = u32Alpha0;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CBMPARA.u32);

    return ;
}

#ifndef HI_BUILD_IN_BOOT
HI_VOID  OPTM_VDP_GFX_GetLayerGalpha(HI_U32 u32Data, HI_U8 *pu8Alpha0)
{
    volatile U_G0_CBMPARA G0_CBMPARA;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *pu8Alpha0 = G0_CBMPARA.bits.galpha;

    return ;
}
#endif


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetPalpha
* description   : CNcomment: 设置pixle alpha值 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetPalpha(HI_U32 u32Data, HI_U32 bAlphaEn,HI_U32 bArange,HI_U32 u32Alpha0,HI_U32 u32Alpha1)
{


    volatile U_G0_CBMPARA G0_CBMPARA;
    volatile U_G0_CKEYMIN G0_CKEYMIN;
    volatile U_G0_CKEYMAX G0_CKEYMAX;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    /**
     **叠加寄存器像素alpha使能。
     **/
    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CBMPARA.bits.palpha_en = bAlphaEn;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CBMPARA.u32);

    /**
     **color key最小值，为非即时寄存器。
     **/
    G0_CKEYMIN.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMIN.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CKEYMIN.bits.va1 = u32Alpha1;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMIN.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CKEYMIN.u32);

    /**
     **color key最大值，为非即时寄存器。
     **/
    G0_CKEYMAX.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMAX.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CKEYMAX.bits.va0 = u32Alpha0;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMAX.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CKEYMAX.u32);

    /**
     **叠加寄存器像素的alpha范围选择。
     **0：表示象素的alpha范围为0~128；
     **1：表示象素的alpha范围为0~255。
     **/
    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CBMPARA.bits.palpha_range = bArange;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CBMPARA.u32);

}

#ifndef HI_BUILD_IN_BOOT
HI_VOID  OPTM_VDP_GFX_GetPalpha(HI_U32 u32Data, HI_U32 *pbAlphaEn,HI_U32 *pbArange,HI_U8 *pu8Alpha0,HI_U8 *pu8Alpha1)
{
    volatile U_G0_CBMPARA G0_CBMPARA;
    volatile U_G0_CKEYMIN G0_CKEYMIN;
    volatile U_G0_CKEYMAX G0_CKEYMAX;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *pbAlphaEn = G0_CBMPARA.bits.palpha_en;

    G0_CKEYMIN.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMIN.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *pu8Alpha1 = G0_CKEYMIN.bits.va1;

    G0_CKEYMAX.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CKEYMAX.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *pu8Alpha0 = G0_CKEYMAX.bits.va0;

    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *pbArange = G0_CBMPARA.bits.palpha_range;

}



/***************************************************************************************
* func            : OPTM_VDP_GFX_SetLayerNAddr
* description    : CNcomment: 设置3D显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetLayerNAddr(HI_U32 u32Data, HI_U32 u32NAddr)
{
    volatile U_G0_NADDR G0_NADDR;
#ifdef CONFIG_HIFB_MMU_SUPPORT
    volatile U_G0_SMMU_BYPASS0 G0_SMMU_BYPASS0;
#endif

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_NADDR.u32 = u32NAddr;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_NADDR.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_NADDR.u32);

#ifdef CONFIG_HIFB_MMU_SUPPORT
    #ifdef CONFIG_GFX_MMU_SUPPORT
        G0_SMMU_BYPASS0.bits.bypass_2d   = 0;
        G0_SMMU_BYPASS0.bits.r_bypass_3d = 0;
    #else
        G0_SMMU_BYPASS0.bits.bypass_2d   = 1;
        G0_SMMU_BYPASS0.bits.r_bypass_3d = 1;
    #endif
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_SMMU_BYPASS0.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_SMMU_BYPASS0.u32);
#endif
}
#endif


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetPreMultEnable
* description   : CNcomment: 叠加寄存器预乘使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GFX_SetPreMultEnable(HI_U32 u32Data, HI_U32 bEnable)
{
    /**
     **叠加寄存器输入位图为预乘图。
     **0：非预乘图；
     **1：预乘图。
     **/
    volatile U_G0_CBMPARA G0_CBMPARA;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CBMPARA.bits.premult_en = bEnable;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CBMPARA.u32);
}

#ifndef HI_BUILD_IN_BOOT
HI_VOID OPTM_VDP_GFX_GetPreMultEnable(HI_U32 u32Data, HI_U32 *pbEnable)
{
    volatile U_G0_CBMPARA G0_CBMPARA;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_CBMPARA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CBMPARA.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    *pbEnable = G0_CBMPARA.bits.premult_en;
}
#endif


/***************************************************************************************
* func          : OPTM_VDP_GFX_SetUpdMode
* description   : CNcomment: 设置图层更新数据模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GFX_SetUpdMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    volatile U_G0_CTRL G0_CTRL;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    /* G0 layer register update mode */
    G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_CTRL.bits.upd_mode = u32Mode;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_CTRL.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_CTRL.u32);
}


//-------------------------------------------------------------------
//GP_BEGIN
//-------------------------------------------------------------------
//
HI_VOID  OPTM_VDP_GP_SetParaUpd       (HI_U32 u32Data, OPTM_VDP_GP_PARA_E enMode)
{
    volatile U_GP0_PARAUP GP0_PARAUP;
    GP0_PARAUP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_PARAUP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("select wrong GP layer id\n");
        return ;
    }
    if(enMode == VDP_GP_PARA_ZME_HOR)
    {
        GP0_PARAUP.bits.gp0_hcoef_upd = 0x1;
    }
    else if(enMode == VDP_GP_PARA_ZME_VER)
    {
        GP0_PARAUP.bits.gp0_vcoef_upd = 0x1;
    }
    else
    {
        HIFB_ERROR("select wrong mode!\n");
    }

    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_PARAUP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_PARAUP.u32);
    return ;
}


/***************************************************************************************
* func          : OPTM_VDP_GP_SetIpOrder
* description   : CNcomment: 设置GP CTRL CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GP_SetIpOrder (HI_U32 u32Data, HI_U32 u32Chn, OPTM_VDP_GP_ORDER_E enIpOrder)
{
    if (u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong GP Layer ID\n");
        return;
    }

    if (u32Chn > 1)
    {
        HIFB_ERROR("Select Wrong GP Channel\n");
        return;
    }

    if (u32Chn == 0)//channel A
    {
       OPTM_VDP_GP_SetChn0IpOrder(u32Data,enIpOrder);
    }

    if (u32Chn == 1)//channel B
    {
        OPTM_VDP_GP_SetChn1IpOrder(u32Data,enIpOrder);
    }

    return;
}

static inline HI_VOID OPTM_VDP_GP_SetChn0IpOrder(HI_U32 u32Data, OPTM_VDP_GP_ORDER_E enIpOrder)
{
     volatile U_GP0_CTRL GP0_CTRL ;

     GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));

     if (u32Data == OPTM_VDP_LAYER_GP1)
     {
         GP0_CTRL.bits.ffc_sel = 0;
     }
     switch(enIpOrder)
     {
        #if 0/** deal with codecc **/
         case VDP_GP_ORDER_NULL:
         {
             GP0_CTRL.bits.mux1_sel = 2;
             GP0_CTRL.bits.mux2_sel = 3;
             GP0_CTRL.bits.aout_sel = 0;
             GP0_CTRL.bits.bout_sel = 1;

             break;
         }
        #endif
         case VDP_GP_ORDER_CSC:
         {
             GP0_CTRL.bits.mux1_sel = 0;
             GP0_CTRL.bits.mux2_sel = 3;
             GP0_CTRL.bits.aout_sel = 2;
             GP0_CTRL.bits.bout_sel = 1;

             break;
         }
         case VDP_GP_ORDER_ZME:
         {
             GP0_CTRL.bits.mux1_sel = 2;
             GP0_CTRL.bits.mux2_sel = 0;
             GP0_CTRL.bits.aout_sel = 3;
             GP0_CTRL.bits.bout_sel = 1;

             break;
         }
         case VDP_GP_ORDER_CSC_ZME:
         {
             GP0_CTRL.bits.mux1_sel = 0;
             GP0_CTRL.bits.mux2_sel = 2;
             GP0_CTRL.bits.aout_sel = 3;
             GP0_CTRL.bits.bout_sel = 1;

             break;
         }
         case VDP_GP_ORDER_ZME_CSC:
         {
             GP0_CTRL.bits.mux1_sel = 3;
             GP0_CTRL.bits.mux2_sel = 0;
             GP0_CTRL.bits.aout_sel = 2;
             GP0_CTRL.bits.bout_sel = 1;

             break;
         }
         default://null
         {
             GP0_CTRL.bits.mux1_sel = 2;
             GP0_CTRL.bits.mux2_sel = 3;
             GP0_CTRL.bits.aout_sel = 0;
             GP0_CTRL.bits.bout_sel = 1;

             break;
         }
     }

     OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_CTRL.u32);

     return;
}

static inline HI_VOID OPTM_VDP_GP_SetChn1IpOrder(HI_U32 u32Data, OPTM_VDP_GP_ORDER_E enIpOrder)
{
    volatile U_GP0_CTRL GP0_CTRL ;

    GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));

    if (u32Data == OPTM_VDP_LAYER_GP1)
    {
        GP0_CTRL.bits.ffc_sel = 1;
    }

    switch(enIpOrder)
    {
       #if 0/** deal with codecc **/
        case VDP_GP_ORDER_NULL:
        {
            GP0_CTRL.bits.mux1_sel = 1;
            GP0_CTRL.bits.mux2_sel = 0;
            GP0_CTRL.bits.aout_sel = 3;
            GP0_CTRL.bits.bout_sel = 2;

            break;
        }
       #endif
        case VDP_GP_ORDER_CSC:
        {
            GP0_CTRL.bits.mux1_sel = 1;
            GP0_CTRL.bits.mux2_sel = 3;
            GP0_CTRL.bits.aout_sel = 0;
            GP0_CTRL.bits.bout_sel = 2;

            break;
        }
        case VDP_GP_ORDER_ZME:
        {
            GP0_CTRL.bits.mux1_sel = 2;
            GP0_CTRL.bits.mux2_sel = 1;
            GP0_CTRL.bits.aout_sel = 0;
            GP0_CTRL.bits.bout_sel = 3;

            break;
        }
        case VDP_GP_ORDER_CSC_ZME:
        {
            GP0_CTRL.bits.mux1_sel = 1;
            GP0_CTRL.bits.mux2_sel = 2;
            GP0_CTRL.bits.aout_sel = 0;
            GP0_CTRL.bits.bout_sel = 3;

            break;
        }
        case VDP_GP_ORDER_ZME_CSC:
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

    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_CTRL.u32);

    return;
}


/***************************************************************************************
* func          : OPTM_VDP_GP_SetReadMode
* description   : CNcomment: 设置GP读取数据模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GP_SetReadMode(HI_U32 u32Data, HI_U32 u32Mode)
{
    volatile U_GP0_CTRL GP0_CTRL;
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }

    GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_CTRL.bits.read_mode = u32Mode;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_CTRL.u32);

}

/***************************************************************************************
* func          : OPTM_VDP_GP_GetRect
* description   : CNcomment: 获取GP大小 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#ifndef HI_BUILD_IN_BOOT
HI_VOID  OPTM_VDP_GP_GetRect  (HI_U32 u32Data, OPTM_VDP_DISP_RECT_S  *pstRect)
{
    volatile U_GP0_IRESO GP0_IRESO;
    volatile U_GP0_ORESO GP0_ORESO;

    GP0_IRESO.u32    = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_IRESO.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    pstRect->u32IWth = GP0_IRESO.bits.iw + 1;
    pstRect->u32IHgt = GP0_IRESO.bits.ih + 1;

    GP0_ORESO.u32    = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ORESO.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    pstRect->u32OWth = GP0_ORESO.bits.ow + 1;
    pstRect->u32OHgt = GP0_ORESO.bits.oh + 1;

}
#endif

/***************************************************************************************
* func          : OPTM_VDP_GP_SetLayerReso
* description   : CNcomment: 设置GP输入输出分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GP_SetLayerReso (HI_U32 u32Data, OPTM_VDP_DISP_RECT_S  stRect)
{
    volatile U_GP0_VFPOS GP0_VFPOS;
    volatile U_GP0_VLPOS GP0_VLPOS;
    volatile U_GP0_DFPOS GP0_DFPOS;
    volatile U_GP0_DLPOS GP0_DLPOS;
    volatile U_GP0_IRESO GP0_IRESO;
    volatile U_GP0_ORESO GP0_ORESO;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong GP Layer ID\n");
        return ;
    }

    /* video position */
    GP0_VFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_VFPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_VFPOS.bits.video_xfpos = stRect.u32VX;
    GP0_VFPOS.bits.video_yfpos = stRect.u32VY;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_VFPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_VFPOS.u32);

    GP0_VLPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_VLPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_VLPOS.bits.video_xlpos = stRect.u32VX + stRect.u32OWth - 1;
    GP0_VLPOS.bits.video_ylpos = stRect.u32VY + stRect.u32OHgt - 1;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_VLPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_VLPOS.u32);

    GP0_DFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_DFPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_DFPOS.bits.disp_xfpos = stRect.u32DXS;
    GP0_DFPOS.bits.disp_yfpos = stRect.u32DYS;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_DFPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_DFPOS.u32);

    GP0_DLPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_DLPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_DLPOS.bits.disp_xlpos = stRect.u32DXL-1;
    GP0_DLPOS.bits.disp_ylpos = stRect.u32DYL-1;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_DLPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_DLPOS.u32);

    GP0_IRESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_IRESO.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_IRESO.bits.iw = stRect.u32IWth - 1;
    GP0_IRESO.bits.ih = stRect.u32IHgt - 1;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_IRESO.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_IRESO.u32);

    GP0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ORESO.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_ORESO.bits.ow = stRect.u32OWth - 1;
    GP0_ORESO.bits.oh = stRect.u32OHgt - 1;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ORESO.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ORESO.u32);

    return ;
}



/***************************************************************************************
* func          : OPTM_VDP_GP_GetLayerReso
* description   : CNcomment: 获取GP输入输出分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GP_GetLayerReso (HI_U32 u32Data, OPTM_VDP_DISP_RECT_S *pstRect)
{
    volatile U_GP0_IRESO GP0_IRESO;
    volatile U_GP0_VFPOS GP0_VFPOS;

    if (u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong GP Layer ID\n");
        return ;
    }
    GP0_VFPOS.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_VFPOS.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_IRESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_IRESO.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    pstRect->u32IWth = GP0_IRESO.bits.iw + 1;
    pstRect->u32IHgt = GP0_IRESO.bits.ih + 1;
    pstRect->u32VX   = GP0_VFPOS.bits.video_xfpos;
    pstRect->u32VY   = GP0_VFPOS.bits.video_yfpos;
    return ;
}

/***************************************************************************************
* func          : OPTM_VDP_GP_SetLayerGalpha
* description   : CNcomment: 设置全局alpha CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GP_SetLayerGalpha (HI_U32 u32Data, HI_U32 u32Alpha)
{
    volatile U_GP0_GALPHA GP0_GALPHA;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong gp Layer ID\n");
        return ;
    }
    GP0_GALPHA.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_GALPHA.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_GALPHA.bits.galpha = u32Alpha;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_GALPHA.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_GALPHA.u32);

    return ;
}


/***************************************************************************************
* func          : OPTM_VDP_GP_GetLayerGalphaSum
* description   : CNcomment: 获取设置全局alpha为0的和 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GP_GetLayerGalphaSum (HI_U32 u32Data, HI_U32* pu32AlphaSum)
{

#ifdef CONFIG_HIFB_GP0_GALPHA_SUM_SUPPORT
    volatile U_GP0_GALPHA_SUM GP0_GALPHA_SUM;
    if(0 == u32Data)
    {
        GP0_GALPHA_SUM.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->GP0_GALPHA_SUM.u32)));
        *pu32AlphaSum = GP0_GALPHA_SUM.bits.galpha_sum;
        return ;
    }
 #endif

    *pu32AlphaSum = 0;

    return ;
}


/***************************************************************************************
* func          : OPTM_VDP_GP_SetRegUp
* description   : CNcomment: 更新GP寄存器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GP_SetRegUp  (HI_U32 u32Data)
{
    volatile U_GP0_UPD GP0_UPD;

    /** GP layer register update **/
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong GP Layer ID\n");
        return ;
    }

    GP0_UPD.bits.regup = 0x1;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_UPD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_UPD.u32);

    return ;
}

//-------------------------------------------------------------------
// GP.CSC begin
//-------------------------------------------------------------------

/***************************************************************************************
* func          : OPTM_VDP_GP_SetCscDcCoef
* description   : CNcomment: 设置GP CSC DC系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GP_SetCscDcCoef   (HI_U32 u32Data, OPTM_VDP_CSC_DC_COEF_S pstCscCoef)
{

#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    volatile U_CVM2_RGB2YUV_OUT_DC0 CVM2_RGB2YUV_OUT_DC0;
    volatile U_CVM2_RGB2YUV_OUT_DC1 CVM2_RGB2YUV_OUT_DC1;
    volatile U_CVM2_RGB2YUV_OUT_DC2 CVM2_RGB2YUV_OUT_DC2;
#endif
    volatile U_GP0_CSC_IDC  GP0_CSC_IDC;
    volatile U_GP0_CSC_ODC  GP0_CSC_ODC;
    volatile U_GP0_CSC_IODC GP0_CSC_IODC;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong GPeo Layer ID\n");
        return ;
    }

#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    if(0 == u32Data){/** GP0 **/

        CVM2_RGB2YUV_OUT_DC0.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_OUT_DC0.u32)));
        CVM2_RGB2YUV_OUT_DC0.bits.g_omap_rgb2yuv_0_out_dc = pstCscCoef.rgb2yuv_out_dc0;
        OPTM_VDP_RegWrite((HI_U32*)(&(pOptmVdpReg->CVM2_RGB2YUV_OUT_DC0.u32)),CVM2_RGB2YUV_OUT_DC0.u32);

        CVM2_RGB2YUV_OUT_DC1.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_OUT_DC1.u32)));
        CVM2_RGB2YUV_OUT_DC1.bits.g_omap_rgb2yuv_1_out_dc = pstCscCoef.rgb2yuv_out_dc1;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_OUT_DC1.u32)),CVM2_RGB2YUV_OUT_DC1.u32);

        CVM2_RGB2YUV_OUT_DC2.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_OUT_DC2.u32)));
        CVM2_RGB2YUV_OUT_DC2.bits.g_omap_rgb2yuv_2_out_dc = pstCscCoef.rgb2yuv_out_dc2;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_OUT_DC2.u32)),CVM2_RGB2YUV_OUT_DC2.u32);

        return;
    }
#endif

    GP0_CSC_IDC.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_IDC.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_IDC.bits.cscidc1  = pstCscCoef.csc_in_dc1;
    GP0_CSC_IDC.bits.cscidc0  = pstCscCoef.csc_in_dc0;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_IDC.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_CSC_IDC.u32);

    GP0_CSC_ODC.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_ODC.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_ODC.bits.cscodc1 = pstCscCoef.csc_out_dc1;
    GP0_CSC_ODC.bits.cscodc0 = pstCscCoef.csc_out_dc0;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_ODC.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_CSC_ODC.u32);

    GP0_CSC_IODC.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_IODC.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_IODC.bits.cscodc2 = pstCscCoef.csc_out_dc2;
    GP0_CSC_IODC.bits.cscidc2 = pstCscCoef.csc_in_dc2;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_IODC.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_CSC_IODC.u32);

    return ;

}

/***************************************************************************************
* func          : OPTM_VDP_GP_SetCscCoef
* description   : CNcomment: 设置GP CSC系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID   OPTM_VDP_GP_SetCscCoef(HI_U32 u32Data, OPTM_VDP_CSC_COEF_S stCscCoef)
{
#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    volatile U_CVM2_RGB2YUV_COEF0 CVM2_RGB2YUV_COEF0;
    volatile U_CVM2_RGB2YUV_COEF1 CVM2_RGB2YUV_COEF1;
    volatile U_CVM2_RGB2YUV_COEF2 CVM2_RGB2YUV_COEF2;
    volatile U_CVM2_RGB2YUV_COEF3 CVM2_RGB2YUV_COEF3;
    volatile U_CVM2_RGB2YUV_COEF4 CVM2_RGB2YUV_COEF4;
    volatile U_CVM2_RGB2YUV_COEF5 CVM2_RGB2YUV_COEF5;
    volatile U_CVM2_RGB2YUV_COEF6 CVM2_RGB2YUV_COEF6;
    volatile U_CVM2_RGB2YUV_COEF7 CVM2_RGB2YUV_COEF7;
    volatile U_CVM2_RGB2YUV_COEF8 CVM2_RGB2YUV_COEF8;
#endif

    volatile U_GP0_CSC_P0 GP0_CSC_P0;
    volatile U_GP0_CSC_P1 GP0_CSC_P1;
    volatile U_GP0_CSC_P2 GP0_CSC_P2;
    volatile U_GP0_CSC_P3 GP0_CSC_P3;
    volatile U_GP0_CSC_P4 GP0_CSC_P4;
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong GPeo ID\n");
        return ;
    }

#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    if(0 == u32Data){/** GP0 **/

        CVM2_RGB2YUV_COEF0.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF0.u32)));
        CVM2_RGB2YUV_COEF1.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF1.u32)));
        CVM2_RGB2YUV_COEF2.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF2.u32)));
        CVM2_RGB2YUV_COEF3.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF3.u32)));
        CVM2_RGB2YUV_COEF4.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF4.u32)));
        CVM2_RGB2YUV_COEF5.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF5.u32)));
        CVM2_RGB2YUV_COEF6.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF6.u32)));
        CVM2_RGB2YUV_COEF7.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF7.u32)));
        CVM2_RGB2YUV_COEF8.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF8.u32)));
        CVM2_RGB2YUV_COEF0.bits.g_omap_00_rgb2yuv = stCscCoef.rgb2yuv_coef00;
        CVM2_RGB2YUV_COEF1.bits.g_omap_01_rgb2yuv = stCscCoef.rgb2yuv_coef01;
        CVM2_RGB2YUV_COEF2.bits.g_omap_02_rgb2yuv = stCscCoef.rgb2yuv_coef02;
        CVM2_RGB2YUV_COEF3.bits.g_omap_10_rgb2yuv = stCscCoef.rgb2yuv_coef10;
        CVM2_RGB2YUV_COEF4.bits.g_omap_11_rgb2yuv = stCscCoef.rgb2yuv_coef11;
        CVM2_RGB2YUV_COEF5.bits.g_omap_12_rgb2yuv = stCscCoef.rgb2yuv_coef12;
        CVM2_RGB2YUV_COEF6.bits.g_omap_20_rgb2yuv = stCscCoef.rgb2yuv_coef20;
        CVM2_RGB2YUV_COEF7.bits.g_omap_21_rgb2yuv = stCscCoef.rgb2yuv_coef21;
        CVM2_RGB2YUV_COEF8.bits.g_omap_22_rgb2yuv = stCscCoef.rgb2yuv_coef22;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF0.u32)),CVM2_RGB2YUV_COEF0.u32);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF1.u32)),CVM2_RGB2YUV_COEF1.u32);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF2.u32)),CVM2_RGB2YUV_COEF2.u32);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF3.u32)),CVM2_RGB2YUV_COEF3.u32);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF4.u32)),CVM2_RGB2YUV_COEF4.u32);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF5.u32)),CVM2_RGB2YUV_COEF5.u32);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF6.u32)),CVM2_RGB2YUV_COEF6.u32);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF7.u32)),CVM2_RGB2YUV_COEF7.u32);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_COEF8.u32)),CVM2_RGB2YUV_COEF8.u32);

        return;
    }

#endif

    GP0_CSC_P0.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P0.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_P0.bits.cscp00 = stCscCoef.csc_coef00;
    GP0_CSC_P0.bits.cscp01 = stCscCoef.csc_coef01;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P0.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET), GP0_CSC_P0.u32);

    GP0_CSC_P1.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P1.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_P1.bits.cscp02 = stCscCoef.csc_coef02;
    GP0_CSC_P1.bits.cscp10 = stCscCoef.csc_coef10;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P1.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET), GP0_CSC_P1.u32);

    GP0_CSC_P2.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P2.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_P2.bits.cscp11 = stCscCoef.csc_coef11;
    GP0_CSC_P2.bits.cscp12 = stCscCoef.csc_coef12;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P2.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET), GP0_CSC_P2.u32);

    GP0_CSC_P3.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P3.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_P3.bits.cscp20 = stCscCoef.csc_coef20;
    GP0_CSC_P3.bits.cscp21 = stCscCoef.csc_coef21;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P3.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET), GP0_CSC_P3.u32);

    GP0_CSC_P4.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P4.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_P4.bits.cscp22 = stCscCoef.csc_coef22;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_P4.u32)) + u32Data*CONFIG_HIFB_GP_OFFSET), GP0_CSC_P4.u32);

}


/***************************************************************************************
* func          : OPTM_VDP_GP_SetCscEnable
* description   : CNcomment: 设置GP CSC使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GP_SetCscEnable   (HI_U32 u32Data, HI_U32 u32bCscEn)
{
#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    volatile U_CVM_CTRL CVM_CTRL;
    volatile U_CVM2_RGB2YUV_CTRL CVM2_RGB2YUV_CTRL;
#endif
    volatile U_GP0_CSC_IDC GP0_CSC_IDC;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong GPeo Layer ID\n");
        return ;
    }

#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    if(0 == u32Data){/** GP0 **/
        CVM_CTRL.u32  = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM_CTRL.u32)));
        CVM_CTRL.bits.cvm_en = u32bCscEn;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM_CTRL.u32)),CVM_CTRL.u32);

        CVM2_RGB2YUV_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_CTRL.u32)));
        CVM2_RGB2YUV_CTRL.bits.cvm2_rgb2yuv_en = u32bCscEn;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_CTRL.u32)),CVM2_RGB2YUV_CTRL.u32);
        return;
    }
#endif
    GP0_CSC_IDC.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_IDC.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_CSC_IDC.bits.csc_en = u32bCscEn;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_CSC_IDC.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_CSC_IDC.u32);

    return ;

}

/***************************************************************************************
* func          : OPTM_VDP_GP_SetRgb2YuvScale2p
* description   : CNcomment: 设置图形层RGB2YUV精度 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_GP_SetRgb2YuvScale2p(HI_U32 u32Data, HI_U32 u32RGB2YUVScale2p)
{
#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    volatile U_CVM2_RGB2YUV_SCALE2P CVM2_RGB2YUV_SCALE2P;

    if(0 == u32Data){/** GP0 **/
        CVM2_RGB2YUV_SCALE2P.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_SCALE2P.u32) ));
        CVM2_RGB2YUV_SCALE2P.bits.g_omap_rgb2yuvscale2p = u32RGB2YUVScale2p;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_SCALE2P.u32)),CVM2_RGB2YUV_SCALE2P.u32);
    }
#endif
    return ;
}

/***************************************************************************************
* func          : OPTM_VDP_GP_SetRgb2YuvMin
* description   : CNcomment: 设置图形层RGB2YUV钳位最小值 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GP_SetRgb2YuvMin(HI_U32 u32Data, HI_U32 u32Rgb2YuvMin)
{

#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    volatile U_CVM2_RGB2YUV_MIN CVM2_RGB2YUV_MIN;

    if(0 == u32Data){/** GP0 **/
        CVM2_RGB2YUV_MIN.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_MIN.u32) ));
        CVM2_RGB2YUV_MIN.bits.g_omap_rgb2yuvmin = u32Rgb2YuvMin;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_MIN.u32) ),CVM2_RGB2YUV_MIN.u32);
    }
#endif
    return ;
}

/***************************************************************************************
* func          : OPTM_VDP_GP_SetRgb2YuvMax
* description   : CNcomment: 设置图形层RGB2YUV钳位最大值 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GP_SetRgb2YuvMax(HI_U32 u32Data, HI_U32 u32Rgb2YuvMax)
{
#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD

    volatile U_CVM2_RGB2YUV_MAX CVM2_RGB2YUV_MAX;

    if(0 == u32Data){/** GP0 **/
        CVM2_RGB2YUV_MAX.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_MAX.u32) ));
        CVM2_RGB2YUV_MAX.bits.g_omap_rgb2yuvmax = u32Rgb2YuvMax;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->CVM2_RGB2YUV_MAX.u32) ),CVM2_RGB2YUV_MAX.u32);
    }
#endif
    return ;
}


//-------------------------------------------------------------------
// GP.ZME begin
//-------------------------------------------------------------------

HI_VOID OPTM_VDP_GP_SetZmeEnable  (HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP GP0_ZME_HSP;
    volatile U_GP0_ZME_VSP GP0_ZME_VSP;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Video Layer ID\n");
        return ;
    }

    /*GP zoom enable */
    if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HSP.bits.hsc_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_VSP.bits.vsc_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}


HI_VOID OPTM_VDP_GP_SetZmePhase   (HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_S32 s32Phase)
{
 #ifdef CONFIG_GP0_ZME_HLC_OFFSET_ENABLE
    volatile U_GP0_ZME_HLOFFSET  GP0_ZME_HLOFFSET;
    volatile U_GP0_ZME_HCOFFSET  GP0_ZME_HCOFFSET;
 #else
    volatile U_GP0_ZME_HOFFSET   GP0_ZME_HOFFSET;
 #endif
    volatile U_GP0_ZME_VOFFSET   GP0_ZME_VOFFSET;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Video Layer ID\n");
        return ;
    }

 #ifdef CONFIG_GP0_ZME_HLC_OFFSET_ENABLE    /* GP zoom enable */
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HLOFFSET.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HLOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HLOFFSET.bits.hor_loffset = s32Phase;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HLOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HLOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HCOFFSET.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HCOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HCOFFSET.bits.hor_coffset = s32Phase;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HCOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HCOFFSET.u32);
    }

 #else
    if((enMode == VDP_ZME_MODE_HORL) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HOFFSET.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HOFFSET.bits.hor_loffset = s32Phase;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_HORC) || (enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HOFFSET.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HOFFSET.bits.hor_coffset = s32Phase;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HOFFSET.u32);
    }
 #endif

    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VOFFSET.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_VOFFSET.bits.vbtm_offset = s32Phase;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VOFFSET.u32);
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VOFFSET.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_VOFFSET.bits.vtp_offset = s32Phase;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VOFFSET.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VOFFSET.u32);
    }

    return ;
}

HI_VOID OPTM_VDP_GP_SetZmeFirEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Video Layer ID\n");
        return ;
    }

    OPTM_VDP_GP_SetHorizontalZmeFirEnable(u32Data,enMode,u32bEnable);
    OPTM_VDP_GP_SetVerticalZmeFirEnable(u32Data,enMode,u32bEnable);

    return ;
}


static inline HI_VOID OPTM_VDP_GP_SetHorizontalZmeFirEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;

    if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HSP.bits.hafir_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HSP.bits.hfir_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    return ;
}


static inline HI_VOID OPTM_VDP_GP_SetVerticalZmeFirEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_VSP        GP0_ZME_VSP;

    if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_VSP.bits.vafir_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_VSP.bits.vfir_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}



HI_VOID OPTM_VDP_GP_SetZmeMidEnable   (HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Video Layer ID\n");
        return ;
    }

    OPTM_VDP_GP_SetHorizontalZmeMidEnable(u32Data,enMode,u32bEnable);
    OPTM_VDP_GP_SetVerticalZmeMidEnable(u32Data,enMode,u32bEnable);

    return ;
}

static inline HI_VOID OPTM_VDP_GP_SetHorizontalZmeMidEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;

    if((enMode == VDP_ZME_MODE_ALPHA)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HSP.bits.hamid_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HSP.u32);
    }
    if((enMode == VDP_ZME_MODE_HORL)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HSP.bits.hlmid_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    if((enMode == VDP_ZME_MODE_HORC)||(enMode == VDP_ZME_MODE_HOR)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_HSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_HSP.bits.hchmid_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HSP.u32);
    }

    return ;
}



static inline HI_VOID OPTM_VDP_GP_SetVerticalZmeMidEnable(HI_U32 u32Data, OPTM_VDP_ZME_MODE_E enMode, HI_U32 u32bEnable)
{
    volatile U_GP0_ZME_VSP        GP0_ZME_VSP;

    if((enMode == VDP_ZME_MODE_ALPHAV)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_VSP.bits.vamid_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VSP.u32);
    }
    if((enMode == VDP_ZME_MODE_VERL)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_VSP.bits.vlmid_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    if((enMode == VDP_ZME_MODE_VERC)||(enMode == VDP_ZME_MODE_VER)||(enMode == VDP_ZME_MODE_ALL))
    {
        GP0_ZME_VSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_VSP.bits.vchmid_en = u32bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VSP.u32);
    }

    return ;
}


HI_VOID OPTM_VDP_GP_SetZmeHorRatio  (HI_U32 u32Data, HI_U32 u32Ratio)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_HSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_ZME_HSP.bits.hratio = u32Ratio;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HSP.u32);

    return ;
}

HI_VOID OPTM_VDP_GP_SetZmeVerRatio  (HI_U32 u32Data, HI_U32 u32Ratio)
{
    volatile U_GP0_ZME_VSR        GP0_ZME_VSR;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Video Layer ID\n");
        return ;
    }

    GP0_ZME_VSR.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSR.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_ZME_VSR.bits.vratio = u32Ratio;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_VSR.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_VSR.u32);

    return ;
}



HI_VOID OPTM_VDP_GP_SetZmeHfirOrder        (HI_U32 u32Data, HI_U32 u32HfirOrder)
{
    volatile U_GP0_ZME_HSP        GP0_ZME_HSP;

    if(u32Data >= OPTM_GP_MAX)
    {
        return ;
    }

    GP0_ZME_HSP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
    GP0_ZME_HSP.bits.hfir_order = u32HfirOrder;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_HSP.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_HSP.u32);

    return ;
}

HI_VOID OPTM_VDP_GP_SetZmeCoefAddr  (HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr)
{
    volatile U_GP0_HCOEFAD    GP0_HCOEFAD;
    volatile U_GP0_VCOEFAD    GP0_VCOEFAD;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Video Layer ID\n");
        return ;
    }

    if(u32Mode == VDP_GP_PARA_ZME_HOR)
    {
        GP0_HCOEFAD.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_HCOEFAD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_HCOEFAD.bits.coef_addr = u32Addr;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_HCOEFAD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_HCOEFAD.u32);
    }
    else if(u32Mode == VDP_GP_PARA_ZME_VER)
    {
        GP0_VCOEFAD.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_VCOEFAD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_VCOEFAD.bits.coef_addr = u32Addr;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_VCOEFAD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_VCOEFAD.u32);
    }
    else
    {
        HIFB_ERROR("Select a Wrong Mode!\n");
    }

    return ;
}

/***************************************************************************************
* func          : OPTM_VDP_GP_SetZmeTabV
* description   : CNcomment: 设置ZME阶数，用来做微缩使用 HIFONE B02新增 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_GP_SetZmeTabV(HI_U32 u32Data, HI_U32 u32ZmeTabV)
{
    return ;
}
//-------------------------------------------------------------------
// GP.ZME.GTI(LTI/CTI) begin
//-------------------------------------------------------------------

HI_VOID  OPTM_VDP_GP_SetTiEnable(HI_U32 u32Data, HI_U32 u32Md,HI_U32 u32Data1)
{
    volatile U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    volatile U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }
    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LTICTRL.bits.lti_en = u32Data1;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LTICTRL.u32);

    }
    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_CTICTRL.bits.cti_en = u32Data1;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_CTICTRL.u32);
    }

}

HI_VOID  OPTM_VDP_GP_SetTiGainRatio(HI_U32 u32Data, HI_U32 u32Md, HI_S32 s32Data)

{
    volatile U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    volatile U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LTICTRL.bits.lgain_ratio = s32Data;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LTICTRL.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_CTICTRL.bits.cgain_ratio = s32Data;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_CTICTRL.u32);
    }

}

HI_VOID  OPTM_VDP_GP_SetTiMixRatio(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32mixing_ratio)

{
    volatile U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    volatile U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }


    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LTICTRL.bits.lmixing_ratio = u32mixing_ratio;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LTICTRL.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_CTICTRL.bits.cmixing_ratio = u32mixing_ratio;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_CTICTRL.u32);
    }

}

HI_VOID  OPTM_VDP_GP_SetTiHfThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32TiHfThd)
{
    volatile U_GP0_ZME_LHFREQTHD GP0_ZME_LHFREQTHD;
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LHFREQTHD.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LHFREQTHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LHFREQTHD.bits.lhfreq_thd0 = u32TiHfThd[0];
        GP0_ZME_LHFREQTHD.bits.lhfreq_thd1 = u32TiHfThd[1];
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LHFREQTHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LHFREQTHD.u32);
    }
}

HI_VOID  OPTM_VDP_GP_SetTiHpCoef(HI_U32 u32Data, HI_U32 u32Md, HI_S32 * s32Data)
{
    volatile U_GP0_ZME_LTICTRL GP0_ZME_LTICTRL;
    volatile U_GP0_ZME_CTICTRL GP0_ZME_CTICTRL;
    volatile U_GP0_ZME_LHPASSCOEF GP0_ZME_LHPASSCOEF;
    volatile U_GP0_ZME_CHPASSCOEF GP0_ZME_CHPASSCOEF;
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTICTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LHPASSCOEF.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LHPASSCOEF.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef0 = s32Data[0];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef1 = s32Data[1];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef2 = s32Data[2];
        GP0_ZME_LHPASSCOEF.bits.lhpass_coef3 = s32Data[3];
        GP0_ZME_LTICTRL.bits.lhpass_coef4    = s32Data[4];
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LTICTRL.u32);
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LHPASSCOEF.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LHPASSCOEF.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTICTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_CHPASSCOEF.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CHPASSCOEF.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_CHPASSCOEF.bits.chpass_coef0 = s32Data[0];
        GP0_ZME_CHPASSCOEF.bits.chpass_coef1 = s32Data[1];
        GP0_ZME_CHPASSCOEF.bits.chpass_coef2 = s32Data[2];
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTICTRL.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_CTICTRL.u32);
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CHPASSCOEF.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_CHPASSCOEF.u32);
    }
}

HI_VOID  OPTM_VDP_GP_SetTiCoringThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd)
{
    volatile U_GP0_ZME_LTITHD GP0_ZME_LTITHD;
    volatile U_GP0_ZME_CTITHD GP0_ZME_CTITHD;
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTITHD.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTITHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LTITHD.bits.lcoring_thd = u32thd;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTITHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LTITHD.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTITHD.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTITHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_CTITHD.bits.ccoring_thd = u32thd;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTITHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_CTITHD.u32);
    }

}

//***********************have problem !!!*****************************************************
HI_VOID  OPTM_VDP_GP_SetTiSwingThd(HI_U32 u32Data, HI_U32 u32Md, HI_U32 u32thd, HI_U32 u32thd1)//have problem
{
    volatile U_GP0_ZME_LTITHD GP0_ZME_LTITHD;
    volatile U_GP0_ZME_CTITHD GP0_ZME_CTITHD;
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LTITHD.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTITHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LTITHD.bits.lover_swing  = u32thd;
        GP0_ZME_LTITHD.bits.lunder_swing = u32thd1;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LTITHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LTITHD.u32);
    }

    if((u32Md == VDP_TI_MODE_CHM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_CTITHD.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTITHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_CTITHD.bits.cover_swing  = u32thd;
        GP0_ZME_CTITHD.bits.cunder_swing = u32thd1;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_CTITHD.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_CTITHD.u32);
    }

}

//***********************************************************************************************
HI_VOID  OPTM_VDP_GP_SetTiGainCoef(HI_U32 u32Data, HI_U32 u32Md, HI_U32 * u32coef)
{
    volatile U_GP0_ZME_LGAINCOEF GP0_ZME_LGAINCOEF;
    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }

    if((u32Md == VDP_TI_MODE_LUM)||(u32Md == VDP_TI_MODE_ALL))
    {
        GP0_ZME_LGAINCOEF.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LGAINCOEF.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET));
        GP0_ZME_LGAINCOEF.bits.lgain_coef0 = u32coef[0];
        GP0_ZME_LGAINCOEF.bits.lgain_coef1 = u32coef[1];
        GP0_ZME_LGAINCOEF.bits.lgain_coef2 = u32coef[2];
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->GP0_ZME_LGAINCOEF.u32)) + u32Data * CONFIG_HIFB_GP_OFFSET), GP0_ZME_LGAINCOEF.u32);
    }
}

HI_VOID  OPTM_VDP_GP_SetTiDefThd(HI_U32 u32Data, HI_U32 u32Md)
{
    HI_U32 u32HfThd[2]    = {20,512};
    HI_U32 u32GainCoef[3] = {152,118,108};
//    HI_S32  s32LtiHpCoef[5] = {-19,-8,-2,-1,-1};
//    HI_S32  s32CtiHpCoef[5] = {-21,-21,-11,0,0};

    HI_S32  s32CtiHpCoef[5] = {-21,-21,-11,0,0};
    HI_S32  s32LtiHpCoef[5] = {-21,-21,-11,0,0};

    if(u32Data >= OPTM_GP_MAX)
    {
        HIFB_ERROR("Select Wrong Graph Layer ID\n");
        return ;
    }

    if(u32Md == VDP_TI_MODE_LUM)
    {
        OPTM_VDP_GP_SetTiGainRatio(u32Data, u32Md, 255);
        OPTM_VDP_GP_SetTiMixRatio (u32Data, u32Md, 127);
        OPTM_VDP_GP_SetTiCoringThd(u32Data, u32Md, 0);
        OPTM_VDP_GP_SetTiSwingThd (u32Data, u32Md, 6, 6);
        OPTM_VDP_GP_SetTiHfThd    (u32Data, u32Md, u32HfThd);
        OPTM_VDP_GP_SetTiGainCoef (u32Data, u32Md, u32GainCoef);
        OPTM_VDP_GP_SetTiHpCoef   (u32Data, u32Md, s32LtiHpCoef);
    }

    if(u32Md == VDP_TI_MODE_CHM)
    {
        OPTM_VDP_GP_SetTiGainRatio(u32Data, u32Md, 255);
        OPTM_VDP_GP_SetTiMixRatio (u32Data, u32Md, 127);
        OPTM_VDP_GP_SetTiCoringThd(u32Data, u32Md, 4);
        OPTM_VDP_GP_SetTiSwingThd (u32Data, u32Md, 6, 6);
        OPTM_VDP_GP_SetTiHfThd    (u32Data, u32Md, u32HfThd);
        OPTM_VDP_GP_SetTiGainCoef (u32Data, u32Md, u32GainCoef);
        OPTM_VDP_GP_SetTiHpCoef   (u32Data, u32Md, s32CtiHpCoef);
    }
}


//-------------------------------------------------------------------
//WBC_DHD0_BEGIN
//-------------------------------------------------------------------
HI_VOID OPTM_VDP_WBC_SetAutoSt(OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    volatile U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    //volatile U_WBC_DHD0_CTRL  WBC_DHD0_CTRL;
    volatile U_WBC_G0_CTRL   WBC_G0_CTRL;

    if(enLayer == OPTM_VDP_LAYER_WBC_GP0 )
    {
        WBC_GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.auto_stop_en=  bEnable;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)));
        WBC_G0_CTRL.bits.auto_stop_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)), WBC_G0_CTRL.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_CTRL.bits.auto_stop_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_CTRL.u32);

    }
}


/***************************************************************************************
* func          : OPTM_VDP_WBC_SetThreeMd
* description   : CNcomment:  设置回写模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_WBC_SetThreeMd(OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 bMode)
{
    /**
     **回写模式选择（在Frame Packing格式时有效）。
     **00：双眼回写(2D回写时需配成00)；
     **10：左眼回写；
     **11：右眼回写。
     **/
    volatile    U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    if(enLayer == OPTM_VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.three_d_mode =  bMode;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
}


/***************************************************************************************
* func          : OPTM_VDP_WBC_SetEnable
* description   : CNcomment: 配置回写使能寄存器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_WBC_SetEnable( OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{

    volatile U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    volatile U_WBC_G0_CTRL   WBC_G0_CTRL;

    if(enLayer == OPTM_VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.wbc_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)));
        WBC_G0_CTRL.bits.wbc_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)), WBC_G0_CTRL.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_CTRL.bits.wbc_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_CTRL.u32);

    }

}

#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_VDP_WBC_GetEnable
* description   : CNcomment: 回写是否使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_WBC_GetEnable( OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 *pU32Enable)
{
    volatile U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    if(enLayer == OPTM_VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)));
        *pU32Enable = WBC_GP0_CTRL.bits.wbc_en;
    }

}
#endif

/***************************************************************************************
* func          : OPTM_VDP_WBC_SetOutIntf
* description   : CNcomment:WBC输出模式。 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_WBC_SetOutIntf (OPTM_VDP_LAYER_WBC_E enLayer, OPTM_VDP_DATA_RMODE_E u32RdMode)
{
    /**
     **WBC输出模式。
     **00：根据接口读取方式自动选择(逐行显示时逐行回写，隔行显示时隔行回写，并自动适配顶底场)；
     **01：逐行输出；
     **10：顶场输出；
     **11：底场输出。
     **/
    volatile U_WBC_GP0_CTRL  WBC_GP0_CTRL;
    if( enLayer == OPTM_VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.mode_out = u32RdMode;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
    else
    {
        HIFB_ERROR("enRdMode error!\n");
    }
}


/***************************************************************************************
* func          : OPTM_VDP_WBC_SetRegUp
* description   : CNcomment:更新回写寄存器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_WBC_SetRegUp (OPTM_VDP_LAYER_WBC_E enLayer)
{

    volatile U_WBC_GP0_UPD WBC_GP0_UPD;
    volatile U_WBC_G0_UPD  WBC_G0_UPD;

    pOptmVdpReg->WBC_GP0_YADDR.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_YADDR.u32)));
    if(0 == pOptmVdpReg->WBC_GP0_YADDR.u32 || 0xffffffff == pOptmVdpReg->WBC_GP0_YADDR.u32){
    /** 防止回写地址空指针，启动回写挂死 **/
        HIFB_ERROR("the wbc addr is null!\n");
        return;
    }
    if(enLayer == OPTM_VDP_LAYER_WBC_GP0){
        WBC_GP0_UPD.bits.regup = 0x1;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_UPD.u32)), WBC_GP0_UPD.u32);
    }else if(enLayer == OPTM_VDP_LAYER_WBC_G0){
        WBC_G0_UPD.bits.regup = 0x1;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_UPD.u32)), WBC_G0_UPD.u32);
    }else if(enLayer == OPTM_VDP_LAYER_WBC_G4){
        WBC_G0_UPD.bits.regup = 0x1;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_UPD.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_UPD.u32);
    }

    return;

}

/***************************************************************************************
* func          : OPTM_VDP_WBC_SetOutFmt
* description   : CNcomment:WBC的输出数据格式。 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_WBC_SetOutFmt(OPTM_VDP_LAYER_WBC_E enLayer, OPTM_VDP_WBC_OFMT_E stIntfFmt)

{
    /**
     **WBC的输出数据格式。
     **0011：ARGB8888/AYUV444；
     **其它：保留。
     **/
    volatile U_WBC_GP0_CTRL   WBC_GP0_CTRL;
    volatile U_WBC_G0_CTRL    WBC_G0_CTRL;
    if(enLayer == OPTM_VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)));
        WBC_GP0_CTRL.bits.format_out = stIntfFmt;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CTRL.u32)), WBC_GP0_CTRL.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)));
        WBC_G0_CTRL.bits.format_out = stIntfFmt;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)), WBC_G0_CTRL.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_CTRL.bits.format_out = stIntfFmt;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CTRL.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_CTRL.u32);
    }


}
/***************************************************************************************
* func          : OPTM_VDP_WBC_SetLayerAddr
* description   : CNcomment: 设置WBC回写地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_WBC_SetLayerAddr   (OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 u32LAddr,HI_U32 u32CAddr,HI_U32 u32LStr, HI_U32 u32CStr)
{
    volatile U_WBC_GP0_STRIDE WBC_GP0_STRIDE;
    volatile U_WBC_G0_STRIDE  WBC_G0_STRIDE;
#ifdef CONFIG_HIFB_MMU_SUPPORT
    volatile U_WBC_GP0_SMMU_BYPASS  WBC_GP0_SMMU_BYPASS;
#endif

    if(OPTM_VDP_LAYER_WBC_GP0 == enLayer){
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_YADDR.u32)), u32LAddr);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_CADDR.u32)), u32CAddr);
        WBC_GP0_STRIDE.bits.wbclstride = u32LStr;
        WBC_GP0_STRIDE.bits.wbccstride = u32CStr;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_STRIDE.u32)), WBC_GP0_STRIDE.u32);
    }else if (OPTM_VDP_LAYER_WBC_G0 == enLayer){
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_AR_ADDR.u32)), u32LAddr);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_GB_ADDR.u32)), u32CAddr);
        WBC_G0_STRIDE.bits.wbcstride = u32LStr;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_STRIDE.u32)), WBC_G0_STRIDE.u32);
    }else if (OPTM_VDP_LAYER_WBC_G4 == enLayer){
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_AR_ADDR.u32)) + CONFIG_HIFB_WBC_OFFSET), u32LAddr);
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_GB_ADDR.u32)) + CONFIG_HIFB_WBC_OFFSET), u32CAddr);
        WBC_G0_STRIDE.bits.wbcstride = u32LStr;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_STRIDE.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_STRIDE.u32);
    }

#ifdef CONFIG_HIFB_MMU_SUPPORT
    if(OPTM_VDP_LAYER_WBC_GP0 == enLayer){
       #ifdef CONFIG_GFX_MMU_SUPPORT
         WBC_GP0_SMMU_BYPASS.bits.bypass = 0;
       #else
         WBC_GP0_SMMU_BYPASS.bits.bypass = 1;
       #endif
       OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_SMMU_BYPASS.u32)), WBC_GP0_SMMU_BYPASS.u32);
    }
#endif

    return;

}


/***************************************************************************************
* func          : OPTM_VDP_WBC_SetLayerReso
* description   : CNcomment: 设置回写标清图层分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_WBC_SetLayerReso     (OPTM_VDP_LAYER_WBC_E enLayer, OPTM_VDP_DISP_RECT_S  stRect)
{
    volatile U_WBC_GP0_ORESO WBC_GP0_ORESO;
    volatile U_WBC_G0_ORESO  WBC_G0_ORESO;

    if(enLayer == OPTM_VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_ORESO.u32)));
        WBC_GP0_ORESO.bits.ow = stRect.u32OWth;
        if (WBC_GP0_ORESO.bits.ow)
        {
            WBC_GP0_ORESO.bits.ow -= 1;
        }
        WBC_GP0_ORESO.bits.oh = stRect.u32OHgt;
        if (WBC_GP0_ORESO.bits.oh)
        {
            WBC_GP0_ORESO.bits.oh -= 1;
        }
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_ORESO.u32)), WBC_GP0_ORESO.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)));
        WBC_G0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)));
        WBC_G0_ORESO.bits.ow = stRect.u32OWth;
        if (WBC_G0_ORESO.bits.ow)
        {
            WBC_G0_ORESO.bits.ow -= 1;
        }
        WBC_G0_ORESO.bits.oh = stRect.u32OHgt;
        if (WBC_G0_ORESO.bits.oh)
        {
            WBC_G0_ORESO.bits.oh -= 1;
        }
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)), WBC_G0_ORESO.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_ORESO.bits.ow = stRect.u32OWth;
        if (WBC_G0_ORESO.bits.ow)
        {
            WBC_G0_ORESO.bits.ow -= 1;
        }
        WBC_G0_ORESO.bits.oh = stRect.u32OHgt;
        if (WBC_G0_ORESO.bits.oh)
        {
            WBC_G0_ORESO.bits.oh -= 1;
        }
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32))+ CONFIG_HIFB_WBC_OFFSET), WBC_G0_ORESO.u32);
    }
}

/***************************************************************************************
* func          : OPTM_VDP_WBC_SetDitherMode
* description   : CNcomment: Dither输出模式选择。 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_WBC_SetDitherMode  (OPTM_VDP_LAYER_WBC_E enLayer, OPTM_VDP_DITHER_E enDitherMode)

{

    volatile U_WBC_GP0_DITHER_CTRL WBC_GP0_DITHER_CTRL;

    if(enLayer == OPTM_VDP_LAYER_WBC_GP0)
    {
        /**
         **Dither输出模式选择。
         **0：10bits 输出；
         **1：8bits输出。
         **/
        WBC_GP0_DITHER_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_DITHER_CTRL.u32)));
        WBC_GP0_DITHER_CTRL.bits.dither_md = enDitherMode;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_DITHER_CTRL.u32)), WBC_GP0_DITHER_CTRL.u32);

    }

}

/***************************************************************************************
* func          : OPTM_VDP_WBC_SetPreZmeEnable
* description   : CNcomment: 设置预缩放使能 CNend\n
* param[in]     : enLayer  : 回写点
* param[in]     : bEnable  : 是否开预缩放使能
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_VDP_WBC_SetPreZmeEnable(OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
}



/***************************************************************************************
* func          : OPTM_VDP_WBC_SetCropReso
* description   : CNcomment: 设置回写标清图层裁剪分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_VDP_WBC_SetCropReso (OPTM_VDP_LAYER_WBC_E enLayer, OPTM_VDP_DISP_RECT_S stRect)
{
    volatile U_WBC_GP0_FCROP WBC_GP0_FCROP;
    volatile U_WBC_GP0_LCROP WBC_GP0_LCROP;

    volatile U_WBC_G0_FCROP WBC_G0_FCROP;
    volatile U_WBC_G0_LCROP WBC_G0_LCROP;

    if(enLayer == OPTM_VDP_LAYER_WBC_GP0)
    {
        WBC_GP0_FCROP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_FCROP.u32)));
        WBC_GP0_FCROP.bits.wfcrop = stRect.u32DXS;
        WBC_GP0_FCROP.bits.hfcrop = stRect.u32DYS;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_FCROP.u32)), WBC_GP0_FCROP.u32);

        WBC_GP0_LCROP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_LCROP.u32)));
        WBC_GP0_LCROP.bits.wlcrop = stRect.u32DXL-1;
        WBC_GP0_LCROP.bits.hlcrop = stRect.u32DYL-1;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_GP0_LCROP.u32)), WBC_GP0_LCROP.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_FCROP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_FCROP.u32)));
        WBC_G0_FCROP.bits.wfcrop = stRect.u32DXS;
        WBC_G0_FCROP.bits.hfcrop = stRect.u32DYS;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_FCROP.u32)), WBC_G0_FCROP.u32);

        WBC_G0_LCROP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_LCROP.u32)));
        WBC_G0_LCROP.bits.wlcrop = stRect.u32DXL-1;
        WBC_G0_LCROP.bits.hlcrop = stRect.u32DYL-1;
        OPTM_VDP_RegWrite((HI_U32*)(&(pOptmVdpReg->WBC_G0_LCROP.u32)), WBC_G0_LCROP.u32);


    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_FCROP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_FCROP.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_FCROP.bits.wfcrop = stRect.u32DXS;
        WBC_G0_FCROP.bits.hfcrop = stRect.u32DYS;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_FCROP.u32)  + CONFIG_HIFB_WBC_OFFSET)), WBC_G0_FCROP.u32);

        WBC_G0_LCROP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_LCROP.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_LCROP.bits.wlcrop = stRect.u32DXL-1;
        WBC_G0_LCROP.bits.hlcrop = stRect.u32DYL-1;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_LCROP.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_LCROP.u32);
    }

    return ;
}

//-------------------------------------------------------------------
//WBC_GFX_BEGIN
//-------------------------------------------------------------------
//
#ifndef HI_BUILD_IN_BOOT
HI_VOID OPTM_VDP_WBC_SetCmpEnable( OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    volatile U_WBC_G0_CMP WBC_G0_CMP;
    if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_CMP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)));
        WBC_G0_CMP.bits.cmp_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)), WBC_G0_CMP.u32);

    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_CMP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_CMP.u32);

    }

}
HI_VOID OPTM_VDP_WBC_SetCmpDrr( OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 u32CmpDrr)
{
    volatile U_WBC_G0_CMP WBC_G0_CMP;
    if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_CMP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)));
        WBC_G0_CMP.bits.cmp_drr = u32CmpDrr;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)), WBC_G0_CMP.u32);

    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_CMP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_drr = u32CmpDrr;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_CMP.u32);

    }

}
HI_VOID OPTM_VDP_WBC_SetLossy( OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 bEnable)
{
    volatile U_WBC_G0_CMP WBC_G0_CMP;
    if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_CMP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)));
        WBC_G0_CMP.bits.cmp_lossy_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)), WBC_G0_CMP.u32);

    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_CMP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_CMP.bits.cmp_lossy_en = bEnable;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_CMP.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_CMP.u32);

    }
}

HI_VOID OPTM_VDP_WBC_SetOffSet(OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 u32Offset)
{
    volatile U_WBC_G0_OFFSET WBC_G0_OFFSET;

    if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_OFFSET.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_OFFSET.u32)));
        WBC_G0_OFFSET.bits.wbcoffset = u32Offset;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_OFFSET.u32)), WBC_G0_OFFSET.u32);

    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_OFFSET.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_OFFSET.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_OFFSET.bits.wbcoffset = u32Offset;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_OFFSET.u32))+ CONFIG_HIFB_WBC_OFFSET), WBC_G0_OFFSET.u32);

    }
}

HI_VOID  OPTM_VDP_SetCMPAddr(OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 u32ARddr,HI_U32 u32GBddr)
{
    if (enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_AR_ADDR.u32)), u32ARddr);
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_GB_ADDR.u32)), u32GBddr);
        return ;
    }
    else if (enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_AR_ADDR.u32)) + CONFIG_HIFB_WBC_OFFSET), u32ARddr);
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_GB_ADDR.u32)) + CONFIG_HIFB_WBC_OFFSET), u32GBddr);
        return ;
    }
}

HI_VOID  OPTM_VDP_SetDCMPAddr(HI_U32 u32Data, HI_U32 u32ARddr,HI_U32 u32GBddr)
{
    volatile U_G0_ADDR  G0_ADDR;
    volatile U_G0_NADDR G0_NADDR;
#ifdef CONFIG_HIFB_MMU_SUPPORT
    volatile U_G0_SMMU_BYPASS0 G0_SMMU_BYPASS0;
#endif
    OPTM_HAL_CHECK_LAYER_VALID(u32Data);

    G0_ADDR.u32  = u32ARddr;
    G0_NADDR.u32 = u32GBddr;

    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_ADDR.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_ADDR.u32);
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_NADDR.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_NADDR.u32);

#ifdef CONFIG_HIFB_MMU_SUPPORT
    #ifdef CONFIG_GFX_MMU_SUPPORT
        G0_SMMU_BYPASS0.bits.bypass_2d   = 0;
        G0_SMMU_BYPASS0.bits.r_bypass_3d = 0;
    #else
        G0_SMMU_BYPASS0.bits.bypass_2d   = 1;
        G0_SMMU_BYPASS0.bits.r_bypass_3d = 1;
    #endif
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_SMMU_BYPASS0.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_SMMU_BYPASS0.u32);
#endif
}

HI_VOID  OPTM_VDP_SetCMPStride(OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 u32Stride)
{
    volatile U_WBC_G0_STRIDE  WBC_G0_STRIDE;

    if (enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_STRIDE.bits.wbcstride = u32Stride;

        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_STRIDE.u32)), WBC_G0_STRIDE.u32);

        return ;
    }
    else if (enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_STRIDE.bits.wbcstride = u32Stride;

        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_STRIDE.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_STRIDE.u32);

        return ;
    }
}

HI_VOID  OPTM_VDP_SetDCMPStride(HI_U32 u32Data, HI_U32 u32Stride)
{
    volatile U_G0_STRIDE G0_STRIDE;

    OPTM_HAL_CHECK_LAYER_VALID(u32Data);
    G0_STRIDE.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_STRIDE.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET));
    G0_STRIDE.bits.surface_stride = u32Stride/STRIDE_ALIGN;
    OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->G0_STRIDE.u32)) + u32Data * CONFIG_HIFB_GFX_OFFSET), G0_STRIDE.u32);
}

HI_VOID  OPTM_VDP_SetCMPInRect (OPTM_VDP_LAYER_WBC_E enLayer, OPTM_VDP_RECT_S *pstRect)
{
    volatile U_WBC_G0_FCROP WBC_G0_FCROP;
    volatile U_WBC_G0_LCROP WBC_G0_LCROP;

       if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_FCROP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_FCROP.u32)));
        WBC_G0_FCROP.bits.wfcrop = pstRect->u32X;
        WBC_G0_FCROP.bits.hfcrop = pstRect->u32Y;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_FCROP.u32)), WBC_G0_FCROP.u32);

        WBC_G0_LCROP.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_LCROP.u32)));
        WBC_G0_LCROP.bits.wlcrop = pstRect->u32X + pstRect->u32Wth-1;
        WBC_G0_LCROP.bits.hlcrop = pstRect->u32Y + pstRect->u32Hgt-1;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_LCROP.u32)), WBC_G0_LCROP.u32);


    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_FCROP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_FCROP.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_FCROP.bits.wfcrop = pstRect->u32X;
        WBC_G0_FCROP.bits.hfcrop = pstRect->u32Y;

        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_FCROP.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_FCROP.u32);

        WBC_G0_LCROP.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_LCROP.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_LCROP.bits.wlcrop = pstRect->u32X + pstRect->u32Wth;
        if (WBC_G0_LCROP.bits.wlcrop)
        {
            WBC_G0_LCROP.bits.wlcrop -= 1;
        }
        WBC_G0_LCROP.bits.hlcrop = pstRect->u32Y + pstRect->u32Hgt;
        if (WBC_G0_LCROP.bits.hlcrop)
        {
            WBC_G0_LCROP.bits.hlcrop -= 1;
        }
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_LCROP.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_LCROP.u32);
    }

    return ;
}

HI_VOID  OPTM_VDP_WBC_SetCMPOutSize(OPTM_VDP_LAYER_WBC_E enLayer, HI_U32 u32Width, HI_U32 u32Height)
{
    volatile U_WBC_G0_ORESO  WBC_G0_ORESO;

    if(enLayer == OPTM_VDP_LAYER_WBC_G0)
    {
        WBC_G0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)));
        WBC_G0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)));
        WBC_G0_ORESO.bits.ow = u32Width - 1;
        WBC_G0_ORESO.bits.oh = u32Height - 1;
        OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)), WBC_G0_ORESO.u32);
    }
    else if(enLayer == OPTM_VDP_LAYER_WBC_G4)
    {
        WBC_G0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32))+ CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_ORESO.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)) + CONFIG_HIFB_WBC_OFFSET));
        WBC_G0_ORESO.bits.ow = u32Width - 1;
        WBC_G0_ORESO.bits.oh = u32Height - 1;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)(&(pOptmVdpReg->WBC_G0_ORESO.u32)) + CONFIG_HIFB_WBC_OFFSET), WBC_G0_ORESO.u32);
    }
}

/*配置内存检测区域首尾地址*/
HI_VOID OPTM_MDDRC_SetZoneAddr(HI_U32 u32Section, HI_U32 u32StartAddr, HI_U32 u32EndAddr)
{

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    volatile U_AWADDR_SRVLNC_START awaddr_srvlnc_start;
    volatile U_AWADDR_SRVLNC_END awaddr_srvlnc_end;
    if(u32Section <= (CONFIG_HIFB_MDDRC_MAX_ZONE-1))
    {
        awaddr_srvlnc_start.u32 = u32StartAddr;
        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)&(pMddrcReg->awaddr_srvlnc_start.u32)) + CONFIG_HIFB_MDDRC_OFFSET*u32Section)
                            , awaddr_srvlnc_start.u32);

        awaddr_srvlnc_end.u32 = u32EndAddr;

        OPTM_VDP_RegWrite((HI_U32*)((unsigned long)&(pMddrcReg->awaddr_srvlnc_end.u32)) + CONFIG_HIFB_MDDRC_OFFSET*u32Section)
                            , awaddr_srvlnc_end.u32);
    }
    else
    {
        return;
    }
#endif
    return;

}

HI_VOID OPTM_MDDRC_GetStatus(HI_U32 *u32Status)
{
#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    *u32Status = OPTM_VDP_RegRead((HI_U32*)&(pMddrcReg->awaddr_srvlnc_status));
#endif
}

OPTM_VDP_DISP_MODE_E OPTM_DISP_GetDispMode  (HI_U32 u32Data)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    if(u32Data >= OPTM_CHN_MAX)
    {
        HIFB_ERROR("Select Wrong CHANNEL ID\n");
        return VDP_DISP_MODE_2D;
    }

    DHD0_CTRL.u32 = OPTM_VDP_RegRead((HI_U32*)((unsigned long)(&(pOptmVdpReg->DHD0_CTRL.u32)) + u32Data * CONFIG_HIFB_CHN_OFFSET));
    return (OPTM_VDP_DISP_MODE_E)DHD0_CTRL.bits.disp_mode;
}


/***************************************************************************************
* func          : OPTM_VDP_DISP_GetIntSignal
* description   : CNcomment: 获取中断状态 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_U32 OPTM_VDP_DISP_GetIntSignal(HI_U32 u32intmask)
{
    volatile U_VOINTSTA VOINTSTA;
    /**
     ** read interrupt status
     ** 读取完成中断状态，入回写完成
     **/
    VOINTSTA.u32 = OPTM_VDP_RegRead((HI_U32*)(unsigned long)(&(pOptmVdpReg->VOINTSTA.u32)));

    return (VOINTSTA.u32 & u32intmask);
}


/***************************************************************************************
* func          : OPTM_ClearIntSignal
* description   : CNcomment: 清除中断状态 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_ClearIntSignal(HI_U32 u32intmask)
{
    OPTM_VDP_RegWrite((HI_U32*)(unsigned long)(&(pOptmVdpReg->VOMSKINTSTA.u32)), u32intmask);
}
#endif

//-------------------------------------------------------------------
//WBC_GFX_END
//-------------------------------------------------------------------
