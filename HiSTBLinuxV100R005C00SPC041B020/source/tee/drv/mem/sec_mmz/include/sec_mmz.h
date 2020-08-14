#ifndef __SEC_MMZ__
#define __SEC_MMZ__

typedef unsigned long mmb_addr_t;

#define MMB_ADDR_INVALID (0)

#ifndef NULL
#define NULL (void *)0
#endif

#define SECURE_MEM 0

#define NON_SECURE_MEM 1

/**
\brief Add a memory pool.
CNcomment:\brief 新增一个内存池。CNend
\attention \n
N/A
\param[in]  name        The name of the memory pool to create.                         CNcomment:内存池名字 CNend
\param[in]  phys_addr   The phisical address of the memory pool.                       CNcomment:内存池的物理地址 CNend
\param[in]  size        The size of the memory pool.                                   CNcomment:内存池尺寸 CNend
\param[in]  alloc_type  Indicates whether the memory pool is secure or non-secure.     CNcomment:内存池的安全属性。CNend
\retval ::non-zero      Success, the memory pool pointer.                              CNcomment:成功, 返回内存块物理地址 CNend
\retval ::NULL          Calling this API fails.                                        CNcomment:API调用失败 CNend
\see \n
N/A
*/
void *    new_zone(const char *name, unsigned long phys_start, unsigned long size, unsigned int alloc_type);

/**
\brief Delete a memory pool.
CNcomment:\brief 删除一个内存池。CNend
\attention \n
N/A
\param[in]  zone        The size of the memory block to be allocated.             CNcomment:内存块尺寸 CNend
\no retval
\see \n
N/A
*/
void        delete_zone(void *zone);

/**
\brief Allocate memory block.
CNcomment:\brief 分配内存块。CNend
\attention \n
N/A
\param[in]  name        The name of the memory block.                             CNcomment:内存块名字。CNend
\param[in]  size        The size of the memory block to be allocated.             CNcomment:内存块尺寸 CNend
\param[in]  alloc_type  Indicates whether the memory is secure or non-secure.     CNcomment:内存块的安全属性。CNend
\param[in]  zone_name   The name of the memory pool to allocate from.             CNcomment:内存池名字 CNend
\retval ::   Return the phisical address of the block if succeed, else return -1. CNcomment:成功时返回内存块物理地址，否则返回-1 CNend
\see \n
N/A
*/
mmb_addr_t  new_mmb(const char *name, int size, unsigned int alloc_type, const char *zone_name);

/**
\brief Delete memory block.
CNcomment:\brief 释放内存块。CNend
\attention \n
N/A
\param[in]  addr        The address of the memory block to be deleted.             CNcomment:内存块起始物理地址 CNend
\see \n
N/A
*/
void        delete_mmb(mmb_addr_t addr);

/**
\brief Remap memory block，non-cacheable.
CNcomment:\brief 映射内存块, 不带cache。CNend
\attention \n
N/A
\param[in]  addr        The address of the memory block to be remaped.             CNcomment:内存块起始物理地址 CNend
\retval ::Return the virtual address of the block.                                 CNcomment:返回内存块虚拟地址 CNend
\see \n
N/A
*/
void *      remap_mmb(mmb_addr_t addr);

/**
\brief Remap memory block，cached.
CNcomment:\brief 映射内存块, 带cache。CNend
\attention \n
N/A
\param[in]  addr        The address of the memory block to be remaped.             CNcomment:内存块起始物理地址 CNend
\retval ::Return the virtual address of the block.                                 CNcomment:返回内存块虚拟地址 CNend
\see \n
N/A
*/
void *      remap_mmb_cached(mmb_addr_t addr);

/**
\brief Unmap memory block.
CNcomment:\brief 解映射内存块。CNend
\attention \n
N/A
\param[in]  mapped_addr        The address of the memory block to be unmaped.     CNcomment:内存块地址 CNend
\retval ::0        Success.                                                       CNcomment:成功, 返回内存块物理地址 CNend
\retval ::-1       Calling this API fails.                                        CNcomment:API调用失败 CNend
\see \n
N/A
*/
int         unmap_mmb(void *mapped_addr);


/**
\brief Unmap memory block from user space.
CNcomment:\brief 解映射内存块。CNend
\attention \n
N/A
\param[in]  mapped_addr        The address of the memory block to be unmaped.     CNcomment:内存块地址 CNend
\retval ::0        Success.                                                       CNcomment:成功, 返回内存块物理地址 CNend
\retval ::-1       Calling this API fails.                                        CNcomment:API调用失败 CNend
\see \n
N/A
*/
int         unmap_mmb_uk(void *mapped_addr);

/**
\brief print all mmz struct link list.
CNcomment:\brief 打印mmz的所有结构成员。CNend
\attention \n
N/A
*/
void        mmz_dump(void);

/**
\brief print all mmb struct link list.
CNcomment:\brief 打印mmb的所有结构成员。CNend
\attention \n
N/A
*/
void        mmb_dump(void);

/**
\brief flush mmb L2 data cache
CNcomment:\brief 刷新mmb的L2级数据缓存Nend
\attention \n
N/A
*/
int flush_mmb(unsigned long addr);

/**
\brief judge the mem is in sec mmz zone.
CNcomment:\brief 打印mmb的所有结构成员。CNend
\attention \n
N/A
*/
int is_sec_mmz(unsigned long phys_addr);


#endif /* __SEC_MMZ_KAPI__ */

