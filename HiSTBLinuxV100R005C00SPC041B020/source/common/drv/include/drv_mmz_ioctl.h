#ifndef __DRV_MMZ_IOCTL_H__
#define __DRV_MMZ_IOCTL_H__

#define HIL_MAX_NAME_LEN 16

/* remove pclint waring anonymous struct or union */

struct mmb_info {
    HI_U32 phys_addr;   /* phys-memory address */
    HI_U32 smmu_addr;
    HI_U32 align;       /* phys-memory's special align size */
    HI_U32 size;        /* length of memory, in bytes*/
    HI_U8* mapped;          /* userspace mapped ptr */

    struct {
        HI_U32 prot:8;  /* PROT_READ or PROT_WRITE */
        HI_U32 flags:12;/* MAP_SHARED or MAP_PRIVATE */
        HI_U32 reserved:12; /* reserved, do not use */
    };
    char mmb_name[HIL_MAX_NAME_LEN];
    char mmz_name[HIL_MAX_NAME_LEN];
    HI_U32 gfp;     /* reserved, do set to 0 */
};

struct dirty_area {
    HI_U32 dirty_phys_start;    /* dirty physical address */
    HI_U8* dirty_virt_start; /* dirty virtual  address,
    must be coherent with dirty_phys_addr */
    HI_U32 dirty_size;
};

struct sec_info {
    char mmb_name[HIL_MAX_NAME_LEN];
    char mmz_name[HIL_MAX_NAME_LEN];
    HI_U32 sec_smmu;
    HI_U32 nosec_smmu;
    HI_U32 phys_addr;
    HI_U32 size;
    HI_U32 align;
};

#define IOC_MMB_ALLOC               _IOWR('m', 10,  struct mmb_info)
#define IOC_MMB_ATTR                _IOR ('m', 11,  struct mmb_info)
#define IOC_MMB_FREE                _IOW ('m', 12,  struct mmb_info)
#define IOC_MMB_ALLOC_V2            _IOWR('m', 13,  struct mmb_info)
#define IOC_MMB_ALLOC_SHARE         _IOWR('m', 14,  struct mmb_info)
#define IOC_MMB_ALLOC_SHM_COM       _IOWR('m', 15,  struct mmb_info)
#define IOC_MMB_GET_SHM_COM         _IOWR('m', 16,  struct mmb_info)
#define IOC_MMB_FORCE_FREE          _IOW ('m', 17,  struct mmb_info)

#define IOC_MMB_USER_REMAP          _IOWR('m', 20,  struct mmb_info)
#define IOC_MMB_USER_REMAP_CACHED   _IOWR('m', 21,  struct mmb_info)
#define IOC_MMB_USER_UNMAP          _IOWR('m', 22,  struct mmb_info)
#define IOC_MMB_USER_GETPHYADDR     _IOWR('m', 23,  struct mmb_info)
#define IOC_MMB_USER_GETPHYADDR_S   _IOWR('m', 24,  struct mmb_info)
#define IOC_MMB_USER_CMA_MAPTO_SMMU _IOWR('m', 25,  struct mmb_info)
#define IOC_MMB_USER_CMA_UNMAPTO_SMMU   _IOWR('m', 26,  struct mmb_info)


#define IOC_MMB_SECSMMU_ALLOC       _IOWR('s', 12, struct sec_info)
#define IOC_MMB_SECSMMU_FREE        _IOWR('s', 13, struct sec_info)
#define IOC_MMB_SECSMMU_MAPTOSECSMMU      _IOWR('s', 10, struct sec_info)
#define IOC_MMB_SECSMMU_UNMAPFROMSMMU     _IOWR('s', 11, struct sec_info)

#define IOC_MMB_ADD_REF             _IO('r', 30)    /* ioctl(file, cmd, arg), arg is mmb_addr */
#define IOC_MMB_DEC_REF             _IO('r', 31)    /* ioctl(file, cmd, arg), arg is mmb_addr */

#define IOC_MMB_FLUSH_DCACHE        _IOWR('c', 40, struct mmb_info)

#define IOC_MMB_FLUSH_DCACHE_DIRTY  _IOW('d', 50, struct dirty_area)

#define IOC_MMB_TEST_CACHE          _IOW('t',  11,  struct mmb_info)



#endif /* __DRV_MMZ_IOCTL_H__ */

