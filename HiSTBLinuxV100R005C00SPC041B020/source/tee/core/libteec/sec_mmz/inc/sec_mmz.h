
#ifndef __SEC_MMZ_H
#define __SEC_MMZ_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/******************************* API Declaration *****************************/
/** \addtogroup      SecureMMZ*/
/** @{*/  /** <!-- -SecureMMZ=*/
/**
\brief Initialize Secure MMZ context.
\attention \n
N/A
\retval ::0   Success
\retval ::non-zero   Fail
\see \n
N/A
*/
int HI_SEC_MMZ_Init(void);

/**
\brief Close Secure MMZ context
\attention \n
N/A
\retval ::0   Success
\retval ::non-zero   Fail
\see \n
N/A
*/
void HI_SEC_MMZ_DeInit(void);

/**
\brief Allocate secure memory block.
\attention \n
N/A
\param[in]  size        The size of the memory block to be allocated.
\param[in]  mmz_name    The name of the memory pool for allocation.
\param[in]  mmb_name    The name of the memory block.
\retval ::non-zero   Success, return the physical address of the block.
\retval ::NULL       Fail to allocate secure memory.
\see \n
N/A
*/
void *HI_SEC_MMZ_New(unsigned long size , char *mmz_name, char *mmb_name);

/**
\brief Free the secure memory block.
\attention \n
N/A
\param[in]  phys_addr    The physical address of the secure memory block.
\retval ::0   Success
\retval ::non-zero Fail
\see \n
N/A
*/
int HI_SEC_MMZ_Delete(unsigned long phys_addr);

/**
\brief Copy data from REE mem to TEE mem.
\attention \n
N/A
\param[in]  CAphyaddr    The physical address of REE.
\param[in]  TAphyaddr    The physical address of the TEE.
\param[in]  CopySize     The size to copy.
\retval ::0   Success
\retval ::non-zero Fail
\see \n
N/A
*/
int HI_SEC_MMZ_CA2TA(unsigned long CAphyaddr, unsigned long TAphyaddr, unsigned long CopySize);

/**
\brief Copy data from TEE mem1 to TEE mem2.
\attention \n
N/A
\param[in]  phyaddr1    The physical address of secure mem1.
\param[in]  phyaddr2    The physical address of secure mem2.
\param[in]  CopySize     The size to copy.
\retval ::0   Success
\retval ::non-zero Fail
\see \n
N/A
*/
int HI_SEC_MMZ_TA2TA(unsigned long phyaddr1, unsigned long phyaddr2, unsigned long CopySize);

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif