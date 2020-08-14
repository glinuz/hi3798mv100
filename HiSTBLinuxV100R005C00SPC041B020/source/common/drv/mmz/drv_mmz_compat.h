#ifndef __DRV_MMZ_COMPAT_H__
#define __DRV_MMZ_COMPAT_H__

#include "drv_media_mem.h"

#define HIL_MAX_NAME_LEN 16

typedef unsigned int            HI_U32;
typedef unsigned char           HI_U8;

/* remove pclint waring anonymous struct or union */

struct compat_mmb_info {
	HI_U32 phys_addr;	/* phys-memory address */
	HI_U32 smmu_addr;
	HI_U32 align;		/* phys-memory's special align size */
	HI_U32 size;		/* length of memory, in bytes*/
	HI_U32 mapped;			/* userspace mapped ptr */

	struct {
		HI_U32 prot:8;	/* PROT_READ or PROT_WRITE */
		HI_U32 flags:12;/* MAP_SHARED or MAP_PRIVATE */
		HI_U32 reserved:12; /* reserved, do not use */
	};
	char mmb_name[HIL_MAX_NAME_LEN];
	char mmz_name[HIL_MAX_NAME_LEN];
	HI_U32 gfp;		/* reserved, do set to 0 */
};

#if IS_ENABLED(CONFIG_COMPAT)
long compat_mmz_userdev_ioctl(struct file *filp, unsigned int cmd,
						unsigned long arg);
#else
#define compat_mmz_userdev_ioctl NULL
#endif

struct mmz_userdev_info;
struct mmb_info;
struct sec_info;

extern int mmz_flush_dcache_mmb(struct mmb_info *pmi);
extern int mmz_flush_all_dcache(void);
extern int mmz_userdev_ioctl_m(struct inode *inode, struct file *file,
			unsigned int cmd, struct mmb_info *pmi);
extern int mmz_userdev_ioctl_s(struct inode *inode, struct file *file,
			unsigned int cmd, struct sec_info *psi);



#define COMPAT_IOC_MMB_ALLOC               _IOWR('m', 10,  struct compat_mmb_info)
#define COMPAT_IOC_MMB_ATTR                _IOR ('m', 11,  struct compat_mmb_info)
#define COMPAT_IOC_MMB_FREE                _IOW ('m', 12,  struct compat_mmb_info)

#define COMPAT_IOC_MMB_USER_REMAP		_IOWR('m', 20,  struct compat_mmb_info)
#define COMPAT_IOC_MMB_USER_REMAP_CACHED 	_IOWR('m', 21,  struct compat_mmb_info)
#define COMPAT_IOC_MMB_USER_UNMAP			_IOWR('m', 22,  struct compat_mmb_info)
#define COMPAT_IOC_MMB_USER_GETPHYADDR		_IOWR('m', 23,  struct compat_mmb_info)
#define COMPAT_IOC_MMB_USER_CMA_MAPTO_SMMU	_IOWR('m', 25,  struct compat_mmb_info)
#define COMPAT_IOC_MMB_USER_CMA_UNMAPTO_SMMU	_IOWR('m', 26,  struct compat_mmb_info)

//for tee
#define COMPAT_IOC_MMB_SECSMMU_MAPTOSECSMMU        _IOWR('s', 10, struct sec_info)
#define COMPAT_IOC_MMB_SECSMMU_UNMAPFROMSMMU        _IOWR('s', 11, struct sec_info)
#define COMPAT_IOC_MMB_SECSMMU_ALLOC        _IOWR('s', 12, struct sec_info)
#define COMPAT_IOC_MMB_SECSMMU_FREE        _IOWR('s', 13, struct sec_info)


#define COMPAT_IOC_MMB_FLUSH_DCACHE		_IOWR('c', 40, struct compat_mmb_info)

#endif /* __DRV_MMZ_COMPAT_H__ */

