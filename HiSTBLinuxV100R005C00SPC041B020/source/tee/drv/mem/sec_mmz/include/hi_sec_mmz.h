#ifndef __HI_SEC_MMZ_H__
#define __HI_SEC_MMZ_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "hi_type.h"

#ifdef WITH_HISI_SEC_MMZ
/**
\brief Allocate memory block.
CNcomment:\brief
\attention \n
N/A
\param[in]  zone_name   The name of the memory pool to allocate from.
\param[in]  buf_name        The name of the memory block.
\param[in]  size        The size of the memory block to be allocated.
\retval ::   Return the phisical address of the block if succeed, else return 0.
\see \n
N/A
*/
HI_ULONG DRV_TEE_MMZ_New(const char *zone_name, const char *buf_name, int size);

/**
\brief Delete memory block.
CNcomment:\brief
\attention \n
N/A
\param[in]  phys_addr        The address of the memory block to be deleted.
\see \n
N/A
*/
HI_VOID DRV_TEE_MMZ_Delete(HI_ULONG phys_addr);

/**
\brief Remap memory block.
CNcomment:\brief
\attention \n
N/A
\param[in]  phys_addr        The phys address of the memory block to be remaped.
\param[in]  cached	     The cache attr of the buffer.
\retval ::Return the virtual address of the block.
\see \n
N/A
*/
HI_VOID *DRV_TEE_MMZ_Map(HI_ULONG phys_addr, HI_BOOL cached);

/**
\brief Unmap memory block.
CNcomment:\brief
\attention \n
N/A
\param[in]  virt_addr        The virtual address of the memory block to be unmaped.
\retval ::0        Success.
\retval ::-1       Calling this API fails.
\see \n
N/A
*/
HI_S32 DRV_TEE_MMZ_Unmap(HI_VOID *virt_addr);

/**
\brief flush mmb data cache
CNcomment:
\attention \n
\param[in]  phys_addr        The phys address of the memory block to be remaped.
N/A
*/
HI_S32 DRV_TEE_MMZ_Flush(HI_ULONG phys_addr);

/**
\brief sec mmz map to sec smmu
CNcomment:\brief
\attention \n
N/A
\param[in]  phys_addr        The phys address of the memory block.
\param[in]  size	     The size of the memory block.
\retval ::sec smmu	Success.
\retval ::0	Calling this API failed.
\see \n
N/A
*/
HI_ULONG DRV_TEE_MMZ_MAP_TO_SECSMMU(HI_ULONG phys_addr, HI_ULONG size);

/**
\brief sec mmz unmap from sec smmu
CNcomment:\brief
\attention \n
N/A
\param[in]  sec_smmu        The sec smmu address of the memory block.
\retval ::0	Success.
\retval ::-1	Calling this API failed.
\see \n
N/A
*/
HI_S32 DRV_TEE_MMZ_UNMAP_FROM_SECSMMU(HI_ULONG sec_smmu);


/**
\brief judge the mem whther in sec mmz zone or not
CNcomment:\brief
\attention \n
N/A
\param[in]  phys_addr       The phys address of the memory block.
\retval ::0	not in sec mmz zone.
\retval ::1	in sec mmz zone.
\see \n
N/A
*/
HI_S32 DRV_TEE_MMZ_IsSecMMZ(HI_ULONG phys_addr);
#else

static inline HI_ULONG DRV_TEE_MMZ_New(const char *zone_name, const char *buf_name, int size)
{
	return 0;
}

static inline HI_VOID DRV_TEE_MMZ_Delete(HI_ULONG phys_addr)
{
	return;
}

static inline HI_VOID *DRV_TEE_MMZ_Map(HI_ULONG phys_addr, HI_BOOL cached)
{
	return NULL;
}

static inline HI_S32 DRV_TEE_MMZ_Unmap(HI_VOID *virt_addr)
{
	return 0;
}

HI_S32 DRV_TEE_MMZ_Flush(HI_ULONG phys_addr)
{
	return 0;
}

static inline HI_ULONG DRV_TEE_MMZ_MAP_TO_SECSMMU(HI_ULONG phys_addr, HI_ULONG size)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_UNMAP_FROM_SECSMMU(HI_ULONG sec_smmu)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_IsSecMMZ(HI_ULONG phys_addr)
{
	return 0;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
