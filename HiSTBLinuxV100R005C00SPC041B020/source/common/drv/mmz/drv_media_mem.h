/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#ifndef __DRV_MEDIA_MEM_H__
#define __DRV_MEDIA_MEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/ion.h>
#include <linux/scatterlist.h>
#include <asm/cacheflush.h>

#include "hi_type.h"
#include "drv_mmz.h"
#include "drv_mmz_ioctl.h"

typedef unsigned int            HI_U32;

#define HIL_MAX_NAME_LEN        16

#ifndef PRINTK_CA
#ifdef  HI_ADVCA_FUNCTION_RELEASE
#define PRINTK_CA(fmt, args...)
#else
#define PRINTK_CA(fmt, args...) do{\
        HI_PRINT("%s(%d): " fmt, __FILE__, __LINE__, ##args); \
} while (0)
#endif
#endif

#define L2_CACHE_SIZE  (512*1024)
#ifdef CONFIG_64BIT
#define mmz_flush_dcache_all		flush_cache_all
#define mmz_flush_dcache_area		__flush_dcache_area
#else
#define mmz_flush_dcache_all		__cpuc_flush_kern_all
#define mmz_flush_dcache_area		__cpuc_flush_dcache_area
#endif

/* alloc status   */
#define HI_USER_ALLOC		(0)
#define HI_KERNEL_ALLOC		(1<<0)

/* unmap mode   */
#define EXCEPTION_FLAG		(0)
#define NORMAL_FLAG			(1<<0)

/* just for inf */
struct hil_media_memory_zone {
    char name[HIL_MAX_NAME_LEN];
    HI_U32 gfp;
    HI_U32 zone_start;
    HI_U32 nbytes;
    struct list_head list;
    int iommu;
    struct rb_root root;
    HI_U32 heap_id_mask;
    void (*destructor)(const void *);
};
typedef struct hil_media_memory_zone hil_mmz_t;

#define HIL_MMZ_FMT_S "PHYS(0x%08X, 0x%08X), GFP=%u, nBYTES=%uKB,   NAME=\"%s\""
#define hil_mmz_fmt_arg(p) (p)->zone_start,(p)->zone_start+(p)->nbytes-1,(p)->gfp,(p)->nbytes/SZ_1K,(p)->name

/* for inf & usr */
struct mmb_kdata {
	void *kvirt;
	HI_U32 kmap_ref;
	HI_U32 map_cached;
};

struct mmb_udata {
	pid_t tgid;
	void *uvirt;
	struct list_head list;
	HI_U32 map_ref;
	int map_fd;
	int unmap_flag; /* indicate if normal unmapd or process killed by system*/
	struct {
		HI_U32 prot : 8;    /* PROT_READ or PROT_WRITE */
		HI_U32 flags : 12;  /* MAP_SHARED or MAP_PRIVATE */
		HI_U32 map_cached : 1;
		HI_U32 reserved : 11; /* reserved, do not use */
	};
};

struct hil_media_memory_block {
    char name[HIL_MAX_NAME_LEN];
    struct hil_media_memory_zone *zone;
    struct ion_handle *handle; // to buffer in ion
    struct ion_client *client;
    struct rb_node node;	// cma node
    struct rb_node s_node;	//smmu/iommu node
    HI_U32 iommu_addr;
    HI_U32 phys_addr;
    HI_U32 sec_smmu;
    struct mmb_kdata *kdata;
    struct list_head ulist;
    HI_U32 length;
    int iommu;		// 0: cma mem , 1: iommu when allocating
    int cma_smmu_ref;
    int phy_ref;
    int map_ref;	//all map:user space and kernel space
    int sec_smmu_ref;  //sec smmu ref in secure os
    int sec_flag;	// 1: means the mem is sec mem   0: the mem is no sec mem
    int flag;	//indicate whether this buffer allocated in kernel space or not
};
typedef struct hil_media_memory_block hil_mmb_t;

struct mmz_userdev_info {
	pid_t tpid;
	pid_t mmap_tpid;
	struct semaphore sem;
	void *private_data;
	void *tmp;
};


#define hil_mmb_phys(p)     ({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->phys_addr;})
#define hil_mmb_length(p)   ({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->length;})
#define hil_mmb_name(p)     ({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->name;})
#define hil_mmb_zone(p)     ({hil_mmb_t *__mmb=(p); BUG_ON(__mmb==NULL); __mmb->zone;})

#define HIL_MMB_MAP2KERN    (1<<0)
#define HIL_MMB_MAP2KERN_CACHED (1<<1)
#define HIL_MMB_RELEASED    (1<<2)

#define HIL_MMB_KVIRT(p)	(\
	 (NULL == p->kdata) ? NULL : p->kdata->kvirt)

#ifdef HI_SMMU_SUPPORT
#define HIL_MMB_FMT_S "phys(0x%08X, 0x%08X), smmu=(0x%08X, 0x%08X),kvirt=%p, length=%uKB,    name=\"%s\""
#define hil_mmb_fmt_arg(p) (p)->phys_addr, (unsigned int)mmz_grain_align((p)->phys_addr+(p)->length)-1,(p)->iommu_addr, (unsigned int)mmz_grain_align((p)->iommu_addr+(p)->length)-1, HIL_MMB_KVIRT(p),(p)->length/SZ_1K,(p)->name
#else
#define HIL_MMB_FMT_S "phys(0x%08X, 0x%08X),kvirt=%p, length=%uKB,    name=\"%s\""
#define hil_mmb_fmt_arg(p) (p)->phys_addr, (unsigned int)mmz_grain_align((p)->phys_addr+(p)->length)-1,HIL_MMB_KVIRT(p),(p)->length/SZ_1K,(p)->name
#endif

extern struct list_head mmz_list;
extern struct semaphore mmz_lock;
struct mmz_iommu{
        struct ion_client *client;
};

/*********nAPI_0 for inf *********/
extern hil_mmz_t *hil_mmz_create(const char *name, HI_U32 gfp, HI_U32 zone_start,
            HI_U32 nbytes);
extern int hil_mmz_destroy(hil_mmz_t *zone);
extern int hil_mmz_register(hil_mmz_t *zone);
extern int hil_mmz_unregister(hil_mmz_t *zone);
extern hil_mmb_t *hil_mmb_getby_phys(HI_U32 addr, HI_U32 iommu);
extern hil_mmb_t *hil_mmb_getby_kvirt(void *virt);
extern hil_mmb_t *hil_mmb_getby_sec_addr(HI_U32 sec_addr, HI_U32 iommu);
extern int set_sec_mmb_flag(u32 addr, int iommu);
extern int clr_sec_mmb_flag(u32 addr, int iommu);
extern int is_sec_mem(u32 addr, int iommu);
extern int sec_delay_release_for_mem(u32 addr, int iommu);
extern int sec_mmb_get(u32 addr, int iommu, u32 sec_smmu);
extern int sec_mmb_put(u32 addr, int iommu);

extern void* hil_mmb_map2kern(hil_mmb_t *mmb);
extern void* hil_mmb_map2kern_cached(hil_mmb_t *mmb);
extern int hil_mmb_unmap(hil_mmb_t *mmb, void *addr);
extern hil_mmb_t *hil_mmbinfo_getby_kvirt(void *virt);

/********** API_1 for inf & usr *********/
extern hil_mmb_t *hil_mmb_alloc(const char *name, HI_U32 size,
			HI_U32 align, const char *mmz_name, int kbuf);
extern int hil_mmb_free(hil_mmb_t *mmb);
extern mmb_addr_t hil_mmb_cma_mapto_iommu (mmb_addr_t addr, int iommu);
extern int hil_mmb_cma_unmapfrom_iommu(mmb_addr_t addr, int iommu);
extern struct sg_table *hil_get_meminfo(hil_mmb_t *mmb);

extern int mmz_read_proc(struct seq_file *m, void *v);
extern int mmz_write_proc(struct file *file, const char __user *buffer,
                                   HI_U32 count, void *data);
void flush_outer_cache_range(mmb_addr_t phyaddr, mmb_addr_t len,
					HI_U32 iommu);
void flush_inner_cache(void *viraddr, HI_U32 len);
int HI_DRV_MMZ_Init(void);
void HI_DRV_MMZ_Exit(void);

#ifdef __cplusplus
}
#endif

#endif
