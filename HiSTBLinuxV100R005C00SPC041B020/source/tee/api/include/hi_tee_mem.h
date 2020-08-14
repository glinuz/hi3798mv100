/******************************************************************************

  Copyright (C), 2015-2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_tee_mem.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/01/010
  Description   :
  History       :
  1.Date        : 2016/01/10
    Author      : y00241285
    Modification: Created file

*******************************************************************************/
/**
 * \file
 * \brief Describes the API about the smmu mem interface.
 */
#ifndef _HI_TEE_MEM_H
#define _HI_TEE_MEM_H

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      SMMU_MEM */
/** @{ */  /** <!-- [SMMU_MEM] */


/** the type of continuous mem */
typedef enum hiTEE_MMZ_TYEP_E
{
	HI_TEE_SEC_MMZ = 0,  /**< mem in trustedzone  */
	HI_TEE_NORMAL_MMZ,   /**< mem in share mem area */
}HI_TEE_MMZ_TYEP_E;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      SMMU_MEM */
/** @{ */  /** <!-- [SMMU_MEM] */


/**
\brief alloc sec mem in share mem, system mem.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] pBufName The name of the buf which will be allocated.
\param[in] ulSize   The size of buf.
\param[in] ulAlign  Reserved.
\retval ::sec smmu address  Success.
\retval ::0                 Failure.
\see \n
N/A
*/
HI_ULONG HI_TEE_SMMU_Alloc(const HI_CHAR *pBufName, HI_ULONG ulSize, HI_ULONG ulAlign);

/**
\brief free sec mem in share mem, system mem.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulSecSmmu    The sec smmu address of the mem which will be freed.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_SMMU_Free(HI_ULONG ulSecSmmu);

/**
\brief map the sec smmu mem to cpu virt.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulSecSmmu    The sec smmu address of the mem.
\param[in] bCached      The cache attr when map.
\retval ::sec cpu address   Success.
\retval ::NULL              Failure.
\see \n
N/A
*/
HI_VOID *HI_TEE_SMMU_MapToCpu(HI_ULONG ulSecSmmu, HI_BOOL bCached);

/**
\brief unmap the sec smmu mem from cpu virt.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] pSecVirt     The sec cpu address of the mem.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_SMMU_UnmapFromCpu(HI_VOID *pSecVirt);

/**
\brief alloc sec mem, continuous phys mem.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] pBufName The name of the buf which will be allocated.
\param[in] ulSize   The size of buf.
\param[in] ulAlign  Reserved.
\param[in] eMemType The mem type:sec mmz zone or share cma mem.
\retval ::sec phys address  Success.
\retval ::0                 Failure.
\see \n
N/A
*/
HI_ULONG HI_TEE_MMZ_Alloc(const HI_CHAR *pBufName, HI_ULONG ulSize, HI_ULONG ulAlign, HI_TEE_MMZ_TYEP_E eMemType);

/**
\brief free sec mem, continuous phy mem.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulPhysAddr   The phys addr of the buf which will be freed.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_Free(HI_ULONG ulPhysAddr);

/**
\brief map the sec mem to cpu virt.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulPhysAddr   The sec phys address of the mem.
\param[in] bCached      The cache attr when map.
\retval ::sec cpu address   Success.
\retval ::NULL              Failure.
\see \n
N/A
*/
HI_VOID *HI_TEE_MMZ_MapToCpu(HI_ULONG ulPhysAddr, HI_BOOL bCached);

/**
\brief unmap the sec mem from cpu virt.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] pSecVirt     The sec cpu address of the mem.
\param[in] ulPhysAddr   The phys address of the mem.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_UnmapFromCpu(HI_VOID *pSecVirt, HI_ULONG ulPhysAddr);

/**
\brief flush the mem cache via phys_addr, only for sec mmz.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulPhysAddr   The phys address of the mem, must be the first address.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_FlushMem(HI_ULONG ulPhysAddr);

/**
\brief flush the mem cache via cpu virt and size.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] pVirt        The cpu virt address of the mem.
\param[in] ulSize       The size of the mem.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_FlushMem(HI_VOID *pVirt, HI_ULONG ulSize);

/**
\brief phys mem map to sec smmu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulPhysAddr   The phys_addr address of the mem.
\param[in] ulSize       The size of the mem.
\retval ::sec smmu address  Success.
\retval ::0                 Failure.
\see \n
N/A
*/
HI_ULONG HI_TEE_MMZ_MapToSmmu(HI_ULONG ulPhysAddr, HI_ULONG ulSize);

/**
\brief phys mem unmap from sec smmu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulSecSmmu        The sec smmu address of the mem.
\param[in] ulPhysAddr       The phys addr of the mem.
\retval ::HI_SUCCESS        Success.
\retval ::HI_FAILURE        Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_UnmapFromSmmu(HI_ULONG ulSecSmmu, HI_ULONG ulPhysAddr);

/**
\brief normal smmu mem map to cpu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulNormalSmmu     The normal smmu address of the mem.
\param[in] bCached          The cache attr of the mem.
\retval ::cpu virt address  Success.
\retval ::NULL              Failure.
\see \n
N/A
*/
HI_VOID *HI_TEE_SMMU_NonsecSmmu_MapToCpu(HI_ULONG ulNormalSmmu, HI_BOOL bCached);

/**
\brief normal smmu mem unmap from cpu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] pSecVirt     The cpu address of the mem.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_SMMU_NonsecSmmu_UnmapFromCpu(HI_VOID *pSecVirt);

/**
\brief normal smmu mem map to sec smmu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulNormalSmmu     The normal smmu address of the mem.
\retval ::sec smmu address  Success.
\retval ::0                 Failure.
\see \n
N/A
*/
HI_ULONG HI_TEE_SMMU_NonsecSmmu_MapToSecSmmu(HI_ULONG ulNormalSmmu);

/**
\brief normal smmu mem unmap from sec smmu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulSecSmmu    The sec smmu address of the mem.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_SMMU_NonsecSmmu_UnmapFromSecSmmu(HI_ULONG ulSecSmmu);

/**
\brief normal cma mem map to cpu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulNormalPhys     The normal cma phys address of the mem.
\param[in] bCached          The cache attr of the mem.
\retval ::cpu virt address  Success.
\retval ::NULL              Failure.
\see \n
N/A
*/
HI_VOID *HI_TEE_SMMU_NonsecMMZ_MapToCpu(HI_ULONG ulNormalPhys, HI_BOOL bCached);

/**
\brief normal cma mem unmap from cpu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] pSecVirt     The cpu address of the mem.
\retval ::HI_SUCCESS    Success.
\retval ::HI_FAILURE    Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_SMMU_NonsecMMZ_UnmapFromCpu(HI_VOID *pSecVirt);

/**
\brief normal cma mem map to sec smmu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulNormalPhys     The normal phys address of the mem.
\retval ::sec smmu address  Success.
\retval ::0                 Failure.
\see \n
N/A
*/
HI_ULONG HI_TEE_SMMU_NonsecMMZ_MapToSecSmmu(HI_ULONG ulNormalPhys);

/**
\brief normal cma mem unmap from sec smmu address.
\attention \n
The hi_mmz.ko need to insmod in REE first.
\param[in] ulSecSmmu        The sec smmu address of the mem.
\retval ::HI_SUCCESS        Success.
\retval ::HI_FAILURE        Failure.
\see \n
N/A
*/
HI_S32 HI_TEE_SMMU_NonsecMMZ_UnmapFromSecSmmu(HI_ULONG ulSecSmmu);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
