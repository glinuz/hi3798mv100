#ifndef _DRV_TEE_MEM_H
#define _DRV_TEE_MEM_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "hi_type.h"

typedef unsigned int HI_U32;
typedef int	HI_S32;
typedef char    HI_CHAR;

typedef enum
{
	SEC_MMZ = 0,
	NORMAL_MMZ,
}TEE_MMZ_TYEP_E;

typedef struct
{
	void* pu8StartVirAddr;
	HI_U32 u32StartPhyAddr;
	HI_U32 u32Size;
} TEE_MMZ_BUFFER_S;

typedef struct
{
	void* pu8StartVirAddr;
	HI_U32 u32StartSmmuAddr;
	HI_U32 u32Size;
} TEE_SMMU_BUFFER_S;

#ifdef WITH_HISI_SMMU
/* 分配不连续的安全内存，做smmu映射
 * bufname：输入参数，buffer的名字
 * size：输入参数，请求申请的大小
 * align：输入参数，对齐方式
 * pSecSmmuBuf ：输出参数，输出安全smmu首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_SMMU_Alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, TEE_SMMU_BUFFER_S *pSecSmmuBuf);

/* 释放安全内存，解除smmu映射
 * pSecSmmuBuf ：输入参数，输入安全内存的smmu首地址
 * 返回值： 成功返回0，失败返回-1
 **/
HI_S32 DRV_TEE_SMMU_Free(const TEE_SMMU_BUFFER_S *pSecSmmuBuf);


/* 安全smmu内存做安全cpu 映射
 * pSecSmmuBuf ：输入输出参数，输入安全内存的smmu首地址，pSecSmmuBuf->pu8StartVirAddr 输出cpu虚拟地址
 * cached: 输入参数，表示映射时的cache属性，1表示带cache，0表示不带cache
 * 返回值： 成功返回0，失败返回-1
 **/
HI_S32 DRV_TEE_SMMU_MapCpu(TEE_SMMU_BUFFER_S *pSecSmmuBuf, HI_BOOL cached);

/* 安全smmu内存清除安全cpu 映射 
 * pSecSmmuBuf ：输入安全内存的cpu虚拟首地址
 * 返回值： 成功返回0，失败返回-1
 **/
HI_S32 DRV_TEE_SMMU_UnmapCpu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf);

/* 分配连续的安全内存
 * bufname：输入参数，buffer的名字
 * size：输入参数，请求申请的大小
 * align：输入参数，对齐方式
 * memtype: 输入参数, mmz内存的类型
 * pSecMBuf ：输出参数，输出安全连续内存的物理首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_MMZ_Alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align,
			TEE_MMZ_TYEP_E memtype, TEE_MMZ_BUFFER_S *pSecMBuf);

/* 释放安全内存
 * pSecMBuf ：输入参数，输入安全连续内存的物理首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_MMZ_Free(const TEE_MMZ_BUFFER_S *pSecMBuf);

/* mmz内存做安全cpu 映射 （share mem）
 * pSecMBuf ：输入/输出参数，输入安全连续内存的物理首地址，pSecMBuf->pu8StartVirAddr 输出安全cpu虚拟地址
 * cached: 输入参数，映射的cache属性，当内存为 sec mmz时有效
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_MMZ_MapCpu(TEE_MMZ_BUFFER_S *pSecMBuf, HI_BOOL cached);

/* 刷新sec mmz内存, 当内存为 sec mmz时有效 
 * pSecMBuf ：输入参数，输入安全cpu虚拟地址和物理地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_MMZ_FlushSecMMZ(TEE_MMZ_BUFFER_S *pSecMBuf);

/*
 * 按照cpu虚拟地址和size刷新cache
 * virt：输入参数，mem的安全cpu虚拟地址
 * size：输入参数，mem的size
 * 返回值：成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_MEM_FlushCache(HI_VOID *virt, HI_U32 size);

/* mmz内存清除安全cpu 映射 
 * pSecMBuf ：输入参数，输入安全cpu虚拟地址和物理地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_MMZ_UnmapCpu(const TEE_MMZ_BUFFER_S *pSecMBuf);

/* 普通连续物理内存做安全SMMU 映射：如安全mmz/share mem 的安全mmz
 * pSecMBuf : 输入参数，需要输入连续内存的物理首地址和size
 * pSecSmmuBuf ： 输出参数，需要输出smmu首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_MMZ_MapToSmmu(const TEE_MMZ_BUFFER_S *pSecMBuf, TEE_SMMU_BUFFER_S *pSecSmmuBuf);

/* 清除连续物理内存的安全SMMU 映射 
 * pSecSmmuBuf ： 输入参数，需要输入安全smmu首地址
 * phys_addr: 输入参数,安全buffer的物理首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_MMZ_UmapFromSmmu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf, HI_ULONG phys_addr);

/* 非安全侧MMZ内存做安全cpu映射
 * pSecMBuf:输入/输出 参数，输入 pSecMBuf->u32StartPhyAddr非安全的物理地址，输出安全的cpu虚拟地址 pSecMBuf->pu8StartVirAddr
 * cached:输入参数，表示映射时cache属性，1表示带cache，0表示不带cache
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_NSMMZ_MapCpu(TEE_MMZ_BUFFER_S *pSecMBuf, HI_BOOL cached);

/* 解除非安全侧MMZ内存安全cpu映射
 * pSecMBuf ：输入参数，输入安全侧的cpu虚拟地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_NSMMZ_UmapCpu(const TEE_MMZ_BUFFER_S *pSecMBuf);

/* 非安全侧MMZ内存做安全SMMU映射
 * psMBuf: 输入参数，输入非安全侧的内存的物理首地址
 * pSecSmmuBuf ：输出参数，输出安全侧的smmu首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_NSMMZ_MapSmmu(const TEE_MMZ_BUFFER_S *psMBuf, TEE_SMMU_BUFFER_S *pSecSmmuBuf);

/* 解除非安全侧MMZ内存安全SMMU映射
 * pSecSmmuBuf:输入参数，输入安全侧的smmu虚拟首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_NSMMZ_UmapSmmu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf);

/* 非安全侧SMMU内存做安全cpu映射
 * pSmmuBuf:输入/输出 参数，输入 pSmmuBuf->u32StartSmmuAddr非安全的smmu首地址，输出安全的cpu虚拟地址pSmmuBuf->pu8StartVirAddr
 * cached:输入参数，表示映射的cache属性，1表示带cache，0表示不带cache
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_NSSMMU_MapCpu(TEE_SMMU_BUFFER_S *pSmmuBuf, HI_BOOL cached);

/* 解除非安全侧SMMU内存安全cpu映射
 * pSecSmmuBuf ：输入参数，输入安全侧的cpu虚拟地址和size
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_NSSMMU_UmapCpu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf);

/* 非安全侧SMMU内存做安全SMMU映射
 * pSmmuBuf: 输入参数，输入非安全侧的内存的smmu首地址
 * sec_smmu ：输出参数，输出安全侧的smmu首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_NSSMMU_MapSmmu(const TEE_SMMU_BUFFER_S *pSmmuBuf, HI_U32 *sec_smmu);

/* 解除非安全侧SMMU内存安全SMMU映射
 * pSecSmmuBuf:输入参数，输入安全侧的smmu虚拟首地址
 * 返回值： 成功返回0， 失败返回 -1
 */
HI_S32 DRV_TEE_NSSMMU_UmapSmmu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf);

/*
 * 获取安全侧smmu的页表基址和smmu逻辑的读写垃圾箱的地址
 * smmu_e_raddr: smmu逻辑读垃圾箱地址
 * smmu_e_waddr: smmu逻辑写垃圾箱地址
 * smmu_pgtbl: 安全侧smmu安全页表基址
 * 返回值：null
 */
HI_VOID DRV_TEE_SMMU_GetPgtbladdr(HI_ULONG *smmu_e_raddr, HI_ULONG *smmu_e_waddr,
						HI_ULONG *smmu_pgtbl);

#else
static inline HI_S32 DRV_TEE_SMMU_Alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, TEE_SMMU_BUFFER_S *pSecSmmuBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_SMMU_Free(const TEE_SMMU_BUFFER_S *pSecSmmuBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_SMMU_MapCpu(TEE_SMMU_BUFFER_S *pSecSmmuBuf, HI_BOOL cached)
{
	return 0;
}

static inline HI_S32 DRV_TEE_SMMU_UnmapCpu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_Alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align,
			TEE_MMZ_TYEP_E memtype, TEE_MMZ_BUFFER_S *pSecMBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_Free(const TEE_MMZ_BUFFER_S *pSecMBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_MapCpu(TEE_MMZ_BUFFER_S *pSecMBuf, HI_BOOL cached)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_FlushSecMMZ(TEE_MMZ_BUFFER_S *pSecMBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_UnmapCpu(const TEE_MMZ_BUFFER_S *pSecMBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_MapToSmmu(const TEE_MMZ_BUFFER_S *pSecMBuf, TEE_SMMU_BUFFER_S *pSecSmmuBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_MMZ_UmapFromSmmu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf, HI_ULONG phys_addr)
{
	return 0;
}

static inline HI_S32 DRV_TEE_NSMMZ_MapCpu(TEE_MMZ_BUFFER_S *pSecMBuf, HI_BOOL cached)
{
	return 0;
}

static inline HI_S32 DRV_TEE_NSMMZ_UmapCpu(const TEE_MMZ_BUFFER_S *pSecMBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_NSMMZ_MapSmmu(const TEE_MMZ_BUFFER_S *psMBuf, TEE_SMMU_BUFFER_S *pSecSmmuBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_NSMMZ_UmapSmmu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_NSSMMU_MapCpu(TEE_SMMU_BUFFER_S *pSmmuBuf, HI_BOOL cached)
{
	return 0;
}

static inline HI_S32 DRV_TEE_NSSMMU_UmapCpu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf)
{
	return 0;
}

static inline HI_S32 DRV_TEE_NSSMMU_MapSmmu(const TEE_SMMU_BUFFER_S *pSmmuBuf, HI_U32 *sec_smmu)
{
	return 0;
}

static inline HI_S32 DRV_TEE_NSSMMU_UmapSmmu(const TEE_SMMU_BUFFER_S *pSecSmmuBuf)
{
	return 0;
}

static inline HI_VOID DRV_TEE_SMMU_GetPgtbladdr(HI_ULONG *smmu_e_raddr, HI_ULONG *smmu_e_waddr,
						HI_ULONG *smmu_pgtbl)
{
	return 0;
}


static inline HI_S32 DRV_TEE_MEM_FlushCache(HI_VOID *virt, HI_U32 size)
{
	return 0;
}
#endif

#ifdef __cplusplus           
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
