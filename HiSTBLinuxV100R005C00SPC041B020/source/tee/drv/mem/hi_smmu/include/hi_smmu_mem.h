#ifndef __HI_SMMU_MEM_H
#define __HI_SMMU_MEM_H


#include "hi_smmu.h"
#include "sre_securemmu.h"

struct hisi_smmu_domain {
	unsigned long iova_start;
	unsigned long iova_size;
	unsigned long *bitmap;
	unsigned long bitmap_pfn_base;
	unsigned long bitmap_pfn_count;
};

struct sec_smmu {
	unsigned long pgtbl_pbase;
	unsigned long pgtbl_size;
	unsigned long r_err_base;
	unsigned long w_err_base;
};

struct hisi_smmu {
	void *pgtbl_addr;
	unsigned long pgtbl_size;
	struct hisi_smmu_domain *hisi_domain;
	struct sec_smmu *sec_smmu;
};

#define HI_SUCCESS	(0)
#define HI_FAILED	(-1)

#define INVIDE_ADDR	(0)

#define SZ_4K                           0x00001000
#define IS_ALIGNED(x, a)                (((x) & ((typeof(x))(a) - 1)) == 0)

#define pr_info(format, arg...)          uart_printf_func(" %s %d" format "", __func__, __LINE__, ##arg)
#define pr_err(format, arg...)          uart_printf_func(" [ERROR]: %s %d" format "", __func__, __LINE__, ##arg)
#define pr_warn(format, arg...)          uart_printf_func(" [WARNNING]: %s  %d " format"" , __func__, __LINE__, ##arg)

#define SECMEMAREA	1
#define NONSECMEM	0

extern void v7_dma_flush_range(unsigned long start, unsigned long end);
extern void v7_flush_kern_cache_all(void);
extern void uart_printf_func(const char *fmt, ...);

/*
 * func: map to sec-smmu
 * pageinfoaddr: input, the phys_addr of mem info buffer
 * total_size: input, the total size of the mem
 * return: smmu address if exec success
 *	   0 if exec failed
 */
unsigned long hisi_map_smmu(unsigned long pageinfoaddr,
				unsigned long total_size);

/*
 * func: map to sec-smmu
 * phys_addr: input, the phys_addr of mem  buffer
 * total_size: input, the total size of the mem
 * return: smmu address if exec success
 *	   0 if exec failed
 */
unsigned long hisi_map_smmu_by_phys(unsigned long phys_addr,
				unsigned long total_size);

/*
 * func: unmap from sec-smmu
 * smmu_addr: input, the sec_smmu of mem info buffer
 * total_size: input, the total size of the mem
 * return: 0 if exec success
 *	   -1 if exec failed
 */
int hisi_unamp_smmu(unsigned long smmu_addr, unsigned long total_size);

/*
 * func: map to sec-cpu in kernel
 * pageinfoaddr: input, the phys_addr of mem info buffer
 * total_size: input, the total size of the mem
 * sec_type: input, indicate if the mem is secure, 0:secure  1:no-secure
 * cached: input, indicate the cache attr when create page table
 * return: sec-cpu virt address if exec success
 *	   NULL if exec failed
 */
void *hisi_map_cpu(unsigned long pageinfoaddr, unsigned long total_size, secure_mode_type sec_type,
									cache_mode_type cached);
/*
 * func: unmap from sec-cpu in kernel
 * va_addr: input, the sec cpu virt addr of mem buffer
 * size: input, the total size of the mem
 * return:   NULL 
 */
void hisi_unmap_cpu(void *va_addr, unsigned long size);

/*
 * func: map to sec-cpu in ta
 * pageinfoaddr: input, the phys_addr of mem info buffer
 * total_size: input, the total size of the mem
 * sec_type: input, indicate if the mem is secure, 0:secure  1:no-secure
 * cached: input, indicate the cache attr when create page table
 * return: sec-cpu virt address if exec success
 *	   NULL if exec failed
 */
void *hisi_ta_map_cpu(unsigned long pageinfoaddr, unsigned long total_size, secure_mode_type sec_type,
									cache_mode_type cached);

/*
 * func: unmap from sec-cpu in ta
 * va_addr: input, the sec cpu virt addr of mem buffer
 * size: input, the total size of the mem
 * return:   NULL 
 */
void hisi_ta_unmap_cpu(void *va_addr, unsigned long size);

/*
 * func: flush the cache
 * virt: input, the virt addr of the mem
 * size: input, the size of mem
 * return: NULL
 */
void smmu_flush_cache_area(void *virt, unsigned long size);

/*
 * func: check the mem if it is secure
 * phys: input, the phys addr of mem
 * size: input, the size of mem
 * return:	1, means secure mem 
 *		0, means no secure mem
 */
int sec_mem_check(unsigned long phys, unsigned long size);
#endif
