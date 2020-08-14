#ifndef _HISI_IOMMU_H_
#define _HISI_IOMMU_H_

#include <linux/list.h>
#include <linux/iommu.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#ifdef CONFIG_HISI_IOMMU_API
/**
 * hisi iommu domain interface
 */
size_t hisi_iommu_iova_size(void);
size_t hisi_iommu_iova_available(void);
void hisi_iommu_free_iova(unsigned long iova, size_t size);
unsigned long hisi_iommu_alloc_iova(size_t size, unsigned long align);
int hisi_iommu_map_range(unsigned long iova_start, struct scatterlist *sgl,
		unsigned long iova_size);
int hisi_iommu_unmap_range(unsigned long iova_start,
		unsigned long iova_size);

int hisi_iommu_map_domain(struct scatterlist *sg,
		struct iommu_map_format *format);
int hisi_iommu_unmap_domain(struct iommu_map_format *format);

phys_addr_t hisi_iommu_domain_iova_to_phys(unsigned long iova);

unsigned int hisi_iommu_page_size(void);
bool hisi_iommu_off_on(void);
int hisi_iommu_get_info(unsigned int *iova_start, unsigned int *pgtbl_base);

#else

/**
 * hisi iommu domain interface
 */
static inline int hisi_iommu_map_domain(struct scatterlist *sg,
		struct iommu_map_format *format)
{
	return 0;
}

static inline int hisi_iommu_unmap_domain(struct iommu_map_format *format)
{
	return 0;
}

static inline phys_addr_t hisi_iommu_domain_iova_to_phys(unsigned long iova)
{
	return 0;
}
static inline unsigned int hisi_iommu_page_size(void)
{
	return SZ_4K;
}

static bool hisi_iommu_off_on(void)
{
	return false;
}

static inline int hisi_iommu_get_info(unsigned int *iova_start,
					unsigned int *pgtbl_base)
{
	return 0;
}

#endif /* CONFIG_HISI_IODOMAIN_API */

#endif /* _HISI_IOMMU_H_ */
