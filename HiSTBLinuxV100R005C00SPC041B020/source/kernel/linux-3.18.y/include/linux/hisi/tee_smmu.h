#ifndef __DRV_TZSMMU_H
#define __DRV_TZSMMU_H

typedef struct TEE_MEM_BUFFER_S
{
	struct sg_table *table;
	u32 u32Size;
}TEE_MEM_BUFFER_S;

struct tz_pageinfo {
	u32 phys_addr;
	u32 npages;
};

/*
 * sec_smmu: sec smmu address
 * phys_addr: phys of the mem
 * normal_smmu: no sec smmu address
 * total_size: the whole size of mem
 * pageinfoaddr: the buf phys of the mem info
 */
typedef struct tz_memblocks {
        u32 sec_smmu;
        u32 phys_addr;
	u32 normal_smmu;
        u32 total_size;
        u32 pageinfoaddr;
} TEE_MEMBLOCKS;

/*
 * phys_buf: phys addree of buffer,in which smmu mem is stored
 * list: list of buffer
 */
typedef struct tee_mem{
	u32 phys_buf;
	struct list_head list;
}TEE_MEM_LIST;

#ifdef CONFIG_TZDRIVER

/*
 * hisi_get_tee_meminfo -- get tz_mblock address by sec addr
 * Parameter:
 *	sec_addr: input parameter, sec addr, sec_smmu or phys
 *	flag: input parameter, indicate the type of sec_addr
 * Return:
 * 	the phys_addr of tz_block if exec success
 *	0 if exec failed
 */
u32 hisi_get_tee_meminfo(u32 sec_addr, int flag);

/*
 * hisi_tee_smmumem_into_ta -- put mem info togather
 * Parameter:
 *	teebuf: input parameter, sg table of meminfo
 *	addr: input parameter, phys or normal smmu
 *	iommu: input patameter,indicate the type of addr
 * Return:
 *	the phys addr of tz_mblock when executing successfully
 *	0 when executing failed
 */
u32 hisi_tee_smmumem_into_ta(TEE_MEM_BUFFER_S *teebuf,  u32 addr, int iommu);

/*
 * hisi_tee_smmumem_outfrom_ta -- free the tmp buffer
 * Parameter:
 *	buf_phys: input parameter, the phy of the tz_mblock.
 * Retrun: null
 *
 * This funcion will free the buffer which is used to put
 * sg table info togather.
 */
void hisi_tee_smmumem_outfrom_ta(u32 buf_phys);


/*
 * hisi_teesmmu_init -- init smmu CA to TA
 * Parameter: null
 * Return: null
 */
int hisi_teesmmu_init(void);


/*
 * hisi_teesmmu_exit -- exit smmu CA from TA and destroy CA
 * Parameter: null
 * Return: null
 */
void hisi_teesmmu_exit(void);

/*
 * hisi_secmem_alloc -- alloc secure mem
 * Parameter:
 *	teebuf: input parameter, sg table of meminfo
 *	addr: input parameter, phys addr or normal smmu
 *	iommu: input parameter, indicate the type of addr
 *	sec_smmu: output parameter, sec_smmu addr
 * Return:
 *	0 when executing successfully
 *	others when failed
 */
int hisi_secmem_alloc(TEE_MEM_BUFFER_S *teebuf, u32 addr, int iommu, u32 *sec_smmu);

/*
 * hisi_secmem_free -- free secure mem
 * Parameter:
 *	sec_addr: input parameter, sec addr of mem, sec smmu or phys
 *	flag: input parameter, indicate the type of sec_addr
 * Return:
 *	0 when executing successfully
 *	others when failed
 */
int hisi_secmem_free(u32 sec_addr, int flag);

/*
 * hisi_secmem_mapto_secsmmu -- map to sec smmu
 * parameter:
 *	teebuf: input parameter, mem list
 *	addr: input parameter, phys addr or normal smmu
 *	iommu: indicate the addr type
 *	sec_smmu:output parameter, sec smmu address
 * return:
 *	0 when exec success
 *	others when failed.
 */
int hisi_secmem_mapto_secsmmu(TEE_MEM_BUFFER_S *teebuf, u32 addr, int iommu, u32 *sec_smmu);

/*
 * hisi_secmem_unmap_from_secsmmu -- unmap from sec smmu
 * parameter:
 *	sec_smmu:input parameter, sec smmu address
 * return:
 *	0 when exec success
 *	others when failed.
 */
int hisi_secmem_unmap_from_secsmmu(u32 sec_smmu);

/*
 * hisi_secmem_agent_end ---- close agent of smmu
 * parameter:null
 * return:
 *	0, success
 *	-1, failed.
 */
int hisi_secmem_agent_end(void);

/*
 * hisi_sec_mem_proc: proc sec mem info
 */
void hisi_sec_mem_proc(void);

#else
static inline u32 hisi_get_tee_meminfo(u32 sec_addr, int flag)
{
	return 0;
}

static inline u32 hisi_tee_smmumem_into_ta(TEE_MEM_BUFFER_S *teebuf,
						u32 addr, int iommu)
{
	return 0;
}

static inline void hisi_tee_smmumem_outfrom_ta(u32 buf_phys)
{
	return;
}

static inline int hisi_teesmmu_init(void)
{
	return 0;
}

static inline void hisi_teesmmu_exit(void) {}

static inline int hisi_secmem_alloc(TEE_MEM_BUFFER_S *teebuf, u32 addr,
						int iommu, u32 *sec_smmu)
{
	return 0;
}

static inline int hisi_secmem_free(u32 sec_addr, int flag)
{
	return 0;
}

static inline int hisi_secmem_mapto_secsmmu(TEE_MEM_BUFFER_S *teebuf, u32 addr, int iommu, u32 *sec_smmu)
{
	return 0;
}

static inline int hisi_secmem_unmap_from_secsmmu(u32 sec_smmu)
{
	return 0;
}

static inline int hisi_secmem_agent_end(void)
{
	return 0;
}

static inline void hisi_sec_mem_proc(void)
{
	return;
}
#endif

#endif


