#ifndef __HI_DRV_MMZ_H__
#define __HI_DRV_MMZ_H__

/* add include here */
#include <linux/version.h>

#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/seq_file.h>
#include <linux/list.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include "hi_type.h"

#include <linux/scatterlist.h>

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/** @addtogroup H_MMZ */
/** @{ */

/***************************** Macro Definition ******************************/
#define MMZ_OTHERS      NULL
#define MMZ_SMMU        "iommu"
/*************************** Structure Definition ****************************/

typedef struct
{
	HI_U8* pu8StartVirAddr;
	HI_U32 u32StartPhyAddr;
	HI_U32 u32Size;
} MMZ_BUFFER_S;

typedef struct
{
	HI_U8* pu8StartVirAddr;
	HI_U32 u32StartSmmuAddr;
	HI_U32 u32Size;
} SMMU_BUFFER_S;

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/*alloc mmz memory, get physic address and map kernel-state address*/
/*CNcomment:申请mmz内存，得到物理地址，并做内核态地址的映射*/
HI_S32  HI_DRV_MMZ_AllocAndMap(const char *name, char *mmzzonename, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf);

/*unmap kernel-state address, release mmz memory*/
/*CNcomment:解除内核态地址的映射，并释放mmz内存*/
HI_VOID HI_DRV_MMZ_UnmapAndRelease(MMZ_BUFFER_S *psMBuf);

/*Only alloc mmz memory, return physic address, but not map kernel-state address*/
/*CNcomment:只申请mmz内存，返回物理地址，不做内核态地址的映射*/
HI_S32  HI_DRV_MMZ_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf);

/*map kernel-state address after alloc mmz memory for cache, and flushing cache with HI_DRV_MMZ_Flush*/
/*CNcomment:申请mmz可Cache内存后，进行内核态地址的映射, 并使用HI_DRV_MMZ_Flush进行cache同步 */
HI_S32 HI_DRV_MMZ_MapCache(MMZ_BUFFER_S *psMBuf);

/*flush cache data to memory, needed to call when map memory with HI_DRV_MMZ_MapCache*/
/*CNcomment:使用HI_DRV_MMZ_MapCache时，需主动调用HI_DRV_MMZ_Flush进行cache数据同步 */
HI_S32 HI_DRV_MMZ_Flush(MMZ_BUFFER_S *psMBuf);

/*alloc mmz memory, and map kernel-state address*/
/*CNcomment:申请mmz内存后，进行内核态地址的映射*/
HI_S32  HI_DRV_MMZ_Map(MMZ_BUFFER_S *psMBuf);

/*unmap kernel-state address*/
/*CNcomment:解除内核态地址的映射*/
HI_VOID HI_DRV_MMZ_Unmap(MMZ_BUFFER_S *psMBuf);

/*release unmapped mmz memory */
/*CNcomment:解除映射后，或没有进行内核态映射的mmz内存进行释放*/
HI_VOID HI_DRV_MMZ_Release(MMZ_BUFFER_S *psMBuf);

// SMMU API
HI_S32  HI_DRV_SMMU_AllocAndMap(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, SMMU_BUFFER_S *pSmmuBuf);

HI_VOID HI_DRV_SMMU_UnmapAndRelease(SMMU_BUFFER_S *pSmmuBuf);

HI_S32  HI_DRV_SMMU_Alloc(const HI_CHAR *bufname, HI_U32 size, int align, SMMU_BUFFER_S *pSmmuBuf);

HI_S32  HI_DRV_SMMU_MapCache(SMMU_BUFFER_S *pSmmuBuf);

HI_S32  HI_DRV_SMMU_Flush(SMMU_BUFFER_S *pSmmuBuf);

HI_S32  HI_DRV_SMMU_Map(SMMU_BUFFER_S *pSmmuBuf);

HI_VOID HI_DRV_SMMU_Unmap(SMMU_BUFFER_S *pSmmuBuf);

HI_VOID HI_DRV_SMMU_Release(SMMU_BUFFER_S *pSmmuBuf);

/* map cma mem to smmu  */
HI_U32 HI_DRV_MMZ_MapToSmmu(MMZ_BUFFER_S *psMBuf);

/* unmap cma mem to smmu   */
HI_S32 HI_DRV_MMZ_UnmapFromSmmu(SMMU_BUFFER_S *psMBuf);

/* get phy addr addr by kvirt   */
HI_S32 HI_DRV_MMZ_GetPhyByVirt(MMZ_BUFFER_S *psMBuf);

/* get smmu addr addr by kvirt   */
HI_S32 HI_DRV_SMMU_GetSmmuAddrByVirt(SMMU_BUFFER_S *psMBuf);

/* get page table base address and the write or read address when smmu occurs error*/
/*
pu32ptaddr: the page table base address
                  moudules need to configure it to smmu common register SMMU_SCB_TTBR and SMMU_CB_TTBR

pu32err_rdaddr: the smmu hardware will give the address if the smmu hardward run error,and
                        the module can  read data which are always 0 from this address
                        moudules need to configure it smmu common register SMMU_ERR_RDADDR

pu32err_wraddr: the smmu hardware will give the address if the smmu hardward run error,and
                        the module can  write data to this address so can avoid to write other module's address
                        moudules need to configure it to their's smmu common register SMMU_ERR_WRADDR
*/
HI_VOID HI_DRV_SMMU_GetPageTableAddr(HI_U32 *pu32ptaddr, HI_U32 *pu32err_rdaddr, HI_U32 *pu32err_wraddr);


/* alloc sec cma mem */
HI_S32  HI_DRV_SECMMZ_Alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, MMZ_BUFFER_S *pSecMBuf);

/* free sec cma mem */
HI_S32 HI_DRV_SECMMZ_Release(const MMZ_BUFFER_S *pSecMBuf);

/* alloc sec sys mem  */
HI_S32  HI_DRV_SECSMMU_Alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, SMMU_BUFFER_S *pSecSmmuBuf);

/* free sec sys mem*/
HI_S32 HI_DRV_SECSMMU_Release(const SMMU_BUFFER_S *pSecSmmuBuf);

/*normal smmu map to sec smmu */
HI_S32 HI_DRV_SMMU_MapToSecSmmu(HI_U32 nonSecsmmu, SMMU_BUFFER_S *pSecSmmuBuf);

/*normal smmu unmap from sec smmu  */
HI_S32 HI_DRV_SMMU_UnmapFromSecSmmu(const SMMU_BUFFER_S *pSecSmmuBuf);

/* normal cma mem map to sec smmu*/
HI_S32 HI_DRV_MMZ_MapToSecSmmu(const MMZ_BUFFER_S *psMBuf, SMMU_BUFFER_S *pSecSmmuBuf);

/* normal cma mem unmap from sec smmu*/
HI_S32 HI_DRV_MMZ_UnmapFromSecSmmu(const SMMU_BUFFER_S *pSecSmmuBuf);


/** @} */

int HI_DRV_MMZ_Init(void);
void HI_DRV_MMZ_Exit(void);

HI_S32 DRV_MMZ_ModInit(HI_VOID);
HI_VOID DRV_MMZ_ModExit(HI_VOID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif

