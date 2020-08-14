/** 
 * \file
 * \brief Describes the information about HiSilicion SecureMMZ.
*/


#ifndef __HI_TEE_MMZ_H__
#define __HI_TEE_MMZ_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************************** Macro Definition ****************************/
/** \addtogroup     SecureMMZ */
/** @{ */  /** <!-- [SecureMMZ] */

/** @} */  /*! <!-- Macro Definition End */

/******************************* API Declaration *****************************/
/** \addtogroup      SecureMMZ*/
/** @{*/  /** <!-- -SecureMMZ=*/

/**
\brief Allocate memory block.
\attention \n
N/A
\param[in]  size        The size of the memory block to be allocated.
\param[in]  mmz_name    The name of the memory pool for allocation, should be "SEC-MMZ" currently.
\param[in]  mmb_name    The name of the memory block.
\retval ::non-zero   Success, return the physical address of the block.
\retval ::zero       Fail to allocate memory block.
\see \n
N/A
*/
HI_ULONG HI_TEE_MMZ_New(HI_U32 size, HI_CHAR *mmz_name, HI_CHAR *buf_name);

/**
\brief Free the memory block.
\attention \n
N/A
\param[in]  phys_addr    The physical address of the memory block.
\retval ::0   Success.
\retval ::non-zero Fail to free the memory block.
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_Delete(HI_ULONG phys_addr);

/**
\brief Map the memory block.
\attention \n
N/A
\param[in]  phys_addr    The physical address of the memory block.                       
\param[in]  cached       Indicating whether the memory is cached or non-cacheable, 1 - cached, 0 - non-cacheable      
\retval ::The virtual address   Success                                           
\retval ::NULL   Fail to map the memory block                                   
\see \n
N/A
*/
HI_VOID *HI_TEE_MMZ_Map(HI_ULONG phys_addr, HI_BOOL cached);

/**
\brief Unmap the memory block.
\attention \n
N/A
\param[in]  virt_addr    The virtual address of the memory block.          
\retval ::0   Success                                                     
\retval ::non-zero  Fail to unmap the memory block.                          
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_Unmap(HI_VOID *virt_addr);

/**
\brief Cache flush.
\attention \n
N/A
\param[in]  phys_addr    The physical address of the memory block.     
\retval ::0   Success                                                    
\retval ::non-zero  Fail to flush the memory block.                         
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_Flush(HI_ULONG phys_addr);

/**
\brief Map to sec smmu.
\attention \n
N/A
\param[in]  phys_addr    The physical address of the memory block.
\param[in]  size    The size of the memory block.    
\retval ::sec_smmu address   Success                                                    
\retval ::0  Fail.                        
\see \n
N/A
*/
HI_ULONG HI_TEE_MMZ_MapToSecSmmu(HI_ULONG phys_addr, HI_ULONG size);

/**
\brief Unmap from sec smmu.
\attention \n
N/A
\param[in]  sec_smmu    The sec smmu address of the memory block.     
\retval ::0   Success                                                    
\retval ::non-zero  Failed.                         
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_UnmapFromSecSmmu(HI_ULONG sec_smmu);

/**
\brief judge if the mem is in sec mmz zone.
\attention \n
N/A
\param[in]  phys_addr    The phys address of the memory block.     
\retval ::0   the mem is not in sec mmz zone                                                    
\retval ::1   the mem is in sec mmz zone.                         
\see \n
N/A
*/
HI_S32 HI_TEE_MMZ_IsSecMMZ(HI_ULONG phys_addr);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __HI_TEE_MEM_H__ */

