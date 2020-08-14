
typedef enum {
       HI_MMZ_NEW = 0x1000,
       HI_MMZ_DEL,
       HI_MMZ_CP_CA2TA,
       HI_MMZ_CP_TA2CA,
       HI_MMZ_CP_TA2TA,
	HI_MMZ_TEST,
	HI_MMZ_DEBUG
} E_HI_MMZ_CommandId;

/**
\brief Allocate secure memory block.
CNcomment:\brief 分配安全内存块。CNend
\attention \n
N/A
\param[in]  size        The size of the memory block to be allocated.             CNcomment:内存块尺寸 CNend
\param[in]  mmz_name    The name of the memory pool to allocate from.             CNcomment:内存池名字 CNend
\param[in]  mmb_name    The name of the memory block.                             CNcomment:内存块名字。CNend
\retval ::non-zero   Success, return the phisical address of the block.           CNcomment:成功, 返回内存块物理地址 CNend
\retval ::NULL       Calling this API fails.                                      CNcomment:API调用失败 CNend
\see \n
N/A
*/
void *HI_SEC_MMZ_New(unsigned long size , char *mmz_name, char *mmb_name);

/**
\brief Relieve the secure memory block.
CNcomment:\brief 释放安全内存块。CNend
\attention \n
N/A
\param[in]  phys_addr    The phisical address of the secure memory block.  CNcomment:待释放内存块的物理地址 CNend
\retval ::0   Success                                                      CNcomment:成功 CNend
\see \n
N/A
*/
int HI_SEC_MMZ_Delete(unsigned long phys_addr);

/**
\brief Memory copy from client side to trusted side.
CNcomment:\brief 非安全到安全的拷贝接口。CNend
\attention \n
N/A
\param[in]  CAphyaddr    The phisical address of client side.       CNcomment:非安全内存的物理地址 CNend
\param[in]  TAphyaddr    The phisical address of the trusted side.  CNcomment:安全内存块的物理地址 CNend
\param[in]  CopySize     The size to be copyed.                     CNcomment:拷贝的内存大小 CNend
\retval ::0   Success                                               CNcomment:成功 CNend
\see \n
N/A
*/
int HI_SEC_MMZ_CA2TA(unsigned long CAphyaddr, unsigned long TAphyaddr, unsigned long CopySize);

//int HI_SEC_MMZ_TA2CA(unsigned long TAphyaddr, unsigned long CAphyaddr, unsigned long CopySize);

/**
\brief Memory copy from secure mem1 to secure mem2.
CNcomment:\brief 安全到安全的拷贝接口。CNend
\attention \n
N/A
\param[in]  phyaddr1    The phisical address of secure mem1.        CNcomment:安全内存1的物理地址 CNend
\param[in]  phyaddr2    The phisical address of secure mem2.        CNcomment:安全内存2的物理地址 CNend
\param[in]  CopySize     The size to be copyed.                     CNcomment:拷贝的内存大小 CNend
\retval ::0   Success                                               CNcomment:成功 CNend
\see \n
N/A
*/
int HI_SEC_MMZ_TA2TA(unsigned long phyaddr1, unsigned long phyaddr2, unsigned long CopySize);


