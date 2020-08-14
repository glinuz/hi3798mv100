/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : hi_vdp_smmu.h
Version         : version 1.0
Author          :
Created         : 2015/08/25
Description     : Describes adp file.
Function List   :
History         :
Date                Author              Modification
2015/08/25          w00217574
******************************************************************************/


#ifndef  _HI_VDP_SMMU_H_
#define  _HI_VDP_SMMU_H_


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */


#include "hi_type.h"
#include "hi_drv_mmz.h"
#include "hi_drv_struct.h"


#include "vdp_hal_mmu.h"
#include "drv_disp_com.h"

/***************************** Macro Definition ******************************/


/******************************* API declaration *****************************/

HI_S32 VDP_Hal_SetSmmuScbTtbr(HI_S32 u32ScbTtbr)
{

#ifdef CFG_VDP_MMU_SUPPORT
    VDP_SMMU_SetScbTtbr(u32ScbTtbr);
#endif

    return HI_SUCCESS;
}
HI_S32 VDP_Hal_SetSmmuCbTtbr(HI_S32 u32CbTtbr)
{

#ifdef CFG_VDP_MMU_SUPPORT
    VDP_SMMU_SetCbTtbr(u32CbTtbr);
#endif

    return HI_SUCCESS;
}
HI_S32 VDP_Hal_SetSmmuErrRdAddr(HI_S32 u32ErrRdAddr)
{
#ifdef CFG_VDP_MMU_SUPPORT
    VDP_SMMU_SetErrRdAddr(u32ErrRdAddr);
#endif
    return HI_SUCCESS;

}
HI_S32 VDP_Hal_SetSmmuErrWrAddr(HI_S32 u32ErrWrAddr)
{

#ifdef CFG_VDP_MMU_SUPPORT
    VDP_SMMU_SetErrWrAddr(u32ErrWrAddr);
#endif

    return HI_SUCCESS;
}


#if (defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a))

HI_VOID VDP_Hal_ClearSMMUInt(HI_S32 u32State)
{

#ifdef CFG_VDP_MMU_SUPPORT
    VDP_SMMU_SetIntnsPtwTransClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntnsTlbinvalidRdClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntnsTlbinvalidWrClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntnsTlbmissClr((HI_U32)HI_TRUE);

    VDP_SMMU_SetIntsPtwTransClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntsTlbinvalidRdClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntsTlbinvalidWrClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntsTlbmissClr((HI_U32)HI_TRUE);

#endif
}


#elif defined(CHIP_TYPE_hi3798cv200)
HI_VOID VDP_Hal_ClearSMMUInt(HI_S32 u32State)
{

#ifdef CFG_VDP_MMU_SUPPORT
    VDP_SMMU_SetIntnsPtwTransClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntnsTlbinvalidClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntnsTlbmissClr((HI_U32)HI_TRUE);

    VDP_SMMU_SetIntsPtwTransClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntsTlbinvalidClr((HI_U32)HI_TRUE);
    VDP_SMMU_SetIntsTlbmissClr((HI_U32)HI_TRUE);

#endif
}
#endif



#if (defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a))

HI_VOID VDP_Hal_SetSMMUIntEnable(HI_BOOL bEnable)
{
#ifdef CFG_VDP_MMU_SUPPORT
    if (0)
    {
        VDP_SMMU_SetIntEn(HI_TRUE);

        VDP_SMMU_SetIntsTlbinvalidRdMsk(HI_FALSE);
        VDP_SMMU_SetIntsTlbinvalidWrMsk(HI_FALSE);

        VDP_SMMU_SetIntsPtwTransMsk(HI_FALSE);
        VDP_SMMU_SetIntsTlbmissMsk(HI_FALSE);

        VDP_SMMU_SetIntnsTlbinvalidRdMsk(HI_FALSE);
        VDP_SMMU_SetIntnsTlbinvalidWrMsk(HI_FALSE);

        VDP_SMMU_SetIntnsPtwTransMsk(HI_FALSE);
        VDP_SMMU_SetIntnsTlbmissMsk(HI_FALSE);

    }
    else
    {
        VDP_SMMU_SetIntEn(HI_FALSE);

        VDP_SMMU_SetIntsTlbinvalidRdMsk(HI_TRUE);
        VDP_SMMU_SetIntsTlbinvalidWrMsk(HI_TRUE);
        VDP_SMMU_SetIntsPtwTransMsk(HI_TRUE);
        VDP_SMMU_SetIntsTlbmissMsk(HI_TRUE);


        VDP_SMMU_SetIntnsTlbinvalidRdMsk(HI_TRUE);
        VDP_SMMU_SetIntnsTlbinvalidWrMsk(HI_TRUE);

        VDP_SMMU_SetIntnsPtwTransMsk(HI_TRUE);
        VDP_SMMU_SetIntnsTlbmissMsk(HI_TRUE);
    }
#endif

}


#elif defined(CHIP_TYPE_hi3798cv200)

HI_VOID VDP_Hal_SetSMMUIntEnable(HI_BOOL bEnable)
{
#ifdef CFG_VDP_MMU_SUPPORT
    if (bEnable)
    {
        VDP_SMMU_SetIntEn(HI_TRUE);

        VDP_SMMU_SetIntsTlbinvalidMsk(HI_FALSE);
        VDP_SMMU_SetIntsPtwTransMsk(HI_FALSE);
        VDP_SMMU_SetIntsTlbmissMsk(HI_FALSE);

        VDP_SMMU_SetIntnsTlbinvalidMsk(HI_FALSE);
        VDP_SMMU_SetIntnsPtwTransMsk(HI_FALSE);
        VDP_SMMU_SetIntnsTlbmissMsk(HI_FALSE);

    }
    else
    {
        VDP_SMMU_SetIntEn(HI_FALSE);

        VDP_SMMU_SetIntsTlbinvalidMsk(HI_TRUE);
        VDP_SMMU_SetIntsPtwTransMsk(HI_TRUE);
        VDP_SMMU_SetIntsTlbmissMsk(HI_TRUE);

        VDP_SMMU_SetIntnsTlbinvalidMsk(HI_TRUE);
        VDP_SMMU_SetIntnsPtwTransMsk(HI_TRUE);
        VDP_SMMU_SetIntnsTlbmissMsk(HI_TRUE);
    }
#endif

}

#endif


typedef HI_U32 (*GetSmmuStatus)(HI_VOID);
typedef HI_U32 (*SMMU_GetRdErrAddr)(HI_VOID);


typedef struct SmmuStatusFunc{
    GetSmmuStatus  pfnGetSmmuStatus;
    HI_U8  *pu8State;
    SMMU_GetRdErrAddr pfnGetSmmuFaultAddr;
}VDP_SMMU_ACCESS_ENTRY_S;

#if (defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a))

VDP_SMMU_ACCESS_ENTRY_S pfnSmmuAccessEntry[] = {
        {VDP_SMMU_GetIntnsPtwTransSta, "SMMU_WRITE_ERR MODULE:vo,Non-Secure State, ptwtrans error:", VDP_SMMU_GetRdErrAddr},
        {VDP_SMMU_GetIntnsTlbmissSta,  "SMMU_WRITE_ERR MODULE:vo,Non-Secure State, tlb miss:",VDP_SMMU_GetRdErrAddr},
        {VDP_SMMU_GetIntnsTlbinvalidRdSta, "SMMU_READ_ERR MODULE:vo,Non-Secure State, Read ErrAddr:", VDP_SMMU_GetRdErrAddr},
        {VDP_SMMU_GetIntnsTlbinvalidWrSta, "SMMU_WRITE_ERR MODULE:vo,Non-Secure State, Write ErrAddr:", VDP_SMMU_GetWrErrAddr},

        {VDP_SMMU_GetIntsPtwTransSta, "SMMU_WRITE_ERR MODULE:vo,Secure State, ptwtrans error:", VDP_SMMU_GetSecureRdErrAddr},
        {VDP_SMMU_GetIntsTlbmissSta,  "SMMU_WRITE_ERR MODULE:vo,Secure State, tlb miss:",  VDP_SMMU_GetSecureRdErrAddr},
        {VDP_SMMU_GetIntsTlbinvalidRdSta, "SMMU_READ_ERR MODULE:vo,Secure State, Read ErrAddr:", VDP_SMMU_GetSecureRdErrAddr},
        {VDP_SMMU_GetIntsTlbinvalidWrSta, "SMMU_WRITE_ERR MODULE:vo,Secure State, Write ErrAddr:", VDP_SMMU_GetSecureWrErrAddr}
};

#elif defined(CHIP_TYPE_hi3798cv200)

VDP_SMMU_ACCESS_ENTRY_S pfnSmmuAccessEntry[] = {
    {VDP_SMMU_GetIntnsPtwTransSta, "SMMU_WRITE_ERR MODULE:vo,Non-Secure State, ptwtrans error:",VDP_SMMU_GetWrErrAddr},
    {VDP_SMMU_GetIntnsTlbmissSta,  "SMMU_WRITE_ERR MODULE:vo,Non-Secure State, tlb miss:",VDP_SMMU_GetWrErrAddr},
    {VDP_SMMU_GetIntnsTlbinvalidSta, "SMMU_WRITE_ERR MODULE:vo,Non-Secure State, Write ErrAddr:", VDP_SMMU_GetWrErrAddr},

    {VDP_SMMU_GetIntsPtwTransSta, "SMMU_WRITE_ERR MODULE:vo,Secure State, ptwtrans error:", VDP_SMMU_GetSecureWrErrAddr},
    {VDP_SMMU_GetIntsTlbmissSta,  "SMMU_WRITE_ERR MODULE:vo,Secure State, tlb miss:",  VDP_SMMU_GetSecureWrErrAddr},
    {VDP_SMMU_GetIntsTlbinvalidSta, "SMMU_WRITE_ERR MODULE:vo,Secure State, Write ErrAddr:", VDP_SMMU_GetSecureWrErrAddr}
};

#endif

static HI_VOID Local_Printf_SmmuErr(HI_VOID)
{
#ifdef CFG_VDP_MMU_SUPPORT
    HI_U32  i = 0;

   for (i = 0; i < sizeof(pfnSmmuAccessEntry)/sizeof(pfnSmmuAccessEntry[0]); i++)
   {
      if (0 != pfnSmmuAccessEntry[i].pfnGetSmmuStatus())
      {
        HI_PRINT("%s 0x%x \n", pfnSmmuAccessEntry[i].pu8State, pfnSmmuAccessEntry[i].pfnGetSmmuFaultAddr());
      }
   }

 #endif
    return;
}

HI_VOID VDP_Hal_GetSMMUSta(HI_S32 *pu32State)
{

#ifdef CFG_VDP_MMU_SUPPORT
    Local_Printf_SmmuErr();
    if (HI_NULL != pu32State)
    {
        *pu32State = 0xffffffff;
    }
#endif

    return;
}


HI_S32 VDP_Hal_SetSmmuEnable(HI_BOOL bEnable)
{
#ifdef CFG_VDP_MMU_SUPPORT
    HI_U32 u32CbTtbr;
    HI_U32 u32ErrRdAddr;
    HI_U32 u32ErrWrAddr;

    HI_DRV_SMMU_GetPageTableAddr(&u32CbTtbr, &u32ErrRdAddr, &u32ErrWrAddr);
    //printk("SMMU addr: 0x%x---0x%x,0x%x\n",u32CbTtbr,u32ErrRdAddr,u32ErrWrAddr);
    if (bEnable)
    {
        VDP_SMMU_SetGlbBypass(HI_FALSE);
        VDP_VID_SetAllRegionSmmuBypassDisable(0,0);
        VDP_VID_SetAllRegionSmmuBypassDisable(1,0);
        VDP_VID_SetAllRegionSmmuBypassDisable(3,0);

        //VDP_Hal_SetSmmuScbTtbr(u32CbTtbr);
        VDP_Hal_SetSmmuCbTtbr(u32CbTtbr);
        VDP_Hal_SetSmmuErrRdAddr(u32ErrRdAddr);
        VDP_Hal_SetSmmuErrWrAddr(u32ErrWrAddr);
    }
    else
    {
        VDP_SMMU_SetGlbBypass(HI_TRUE);
        VDP_VID_SetAllRegionSmmuBypassDisable(0,~0);
        VDP_VID_SetAllRegionSmmuBypassDisable(1,~0);
        VDP_VID_SetAllRegionSmmuBypassDisable(3,~0);
    }

    VDP_SMMU_SetIntEn(HI_TRUE);
#endif

    return HI_SUCCESS;
}


HI_VOID ISR_VDP_SmmuDebug(HI_VOID)
{
    //VDP_Hal_GetSMMUSta(HI_NULL);
    return;
}

/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif /**_HI_VDP_SMMU_H_         *\*/
