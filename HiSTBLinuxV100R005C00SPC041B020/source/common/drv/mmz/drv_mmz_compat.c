#include <linux/string.h>
#include <linux/errno.h>
#include <linux/compat.h>
#include <asm/uaccess.h>

#include "hi_debug.h"
#include "drv_mmz_compat.h"
#include "drv_mmz_ioctl.h"

static int get_meminfo_from_user(struct compat_mmb_info *compat_mmb,
							struct mmb_info *mmi)
{
	int ret = 0;

	if ((NULL == compat_mmb) || (NULL == mmi)) {
		ret = -EFAULT;
		return ret;
	}

	strlcpy(mmi->mmz_name, compat_mmb->mmz_name, HIL_MAX_NAME_LEN);
	strlcpy(mmi->mmb_name, compat_mmb->mmb_name, HIL_MAX_NAME_LEN);

	mmi->phys_addr = compat_mmb->phys_addr;
	mmi->smmu_addr = compat_mmb->smmu_addr;
	mmi->align = compat_mmb->align;
	mmi->size = compat_mmb->size;
	mmi->mapped = compat_ptr(compat_mmb->mapped);

	mmi->prot = compat_mmb->prot;
	mmi->flags = compat_mmb->flags;

	return 0;
}

static int put_meminfo_to_user(struct compat_mmb_info *compat_mmb,
							struct mmb_info *mmi)
{
	int ret = 0;

	if ((NULL == compat_mmb) || (NULL == mmi)) {
		ret = -EFAULT;
		return ret;
	}

	compat_mmb->phys_addr = mmi->phys_addr;
	compat_mmb->smmu_addr = mmi->smmu_addr;
	compat_mmb->size = mmi->size;
	compat_mmb->mapped = ptr_to_compat(mmi->mapped);

	return 0;
}

long compat_mmz_userdev_ioctl(struct file *filp, unsigned int cmd,
							unsigned long arg)
{
	long ret = 0;
	struct mmz_userdev_info *pmu = filp->private_data;

	if (!filp->f_op->unlocked_ioctl)
		return -ENOTTY;

	down(&pmu->sem);
	if(_IOC_TYPE(cmd) == 'm') {
		struct compat_mmb_info compat_mmb;
		struct mmb_info mmi;

		if(_IOC_SIZE(cmd) != sizeof(compat_mmb) || arg == 0) {
			HI_PRINT("_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", _IOC_SIZE(cmd), arg);
			ret = -EINVAL;
			goto exit;
		}

		memset(&compat_mmb, 0, sizeof(compat_mmb));
		memset(&mmi, 0, sizeof(mmi));
		if (copy_from_user(&compat_mmb, (void __user *)arg,
						_IOC_SIZE(cmd))) {
			ret = -EFAULT;
			HI_PRINT("%s %d copy_from_user failed!\n",
						__func__, __LINE__);
			goto exit;
		}

		ret = get_meminfo_from_user(&compat_mmb, &mmi);
		if (ret) {
			ret = -EFAULT;
			HI_PRINT("%s get_meminfo_from_user failed\n",
							__func__);
			goto exit;
		}

		switch(_IOC_NR(cmd)) {
		case _IOC_NR(COMPAT_IOC_MMB_ALLOC):
		{
			ret = mmz_userdev_ioctl_m(filp->f_dentry->d_inode, filp,
							IOC_MMB_ALLOC, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s COMPAT_ION_IOC_ALLOC failed!\n",
						__func__);
				goto exit;
			}
			break;
		}
		case _IOC_NR(COMPAT_IOC_MMB_FREE):
		{
			ret = mmz_userdev_ioctl_m(filp->f_dentry->d_inode, filp,
							IOC_MMB_FREE, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s COMPAT_IOC_MMB_FREE!\n",
						__func__);
				goto exit;
			}
			break;
		}
		case _IOC_NR(COMPAT_IOC_MMB_USER_REMAP):
		{
			ret = mmz_userdev_ioctl_m(filp->f_dentry->d_inode, filp,
						IOC_MMB_USER_REMAP, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s COMPAT_IOC_MMB_USER_REMAP \
						failed!\n", __func__);
				goto exit;
			}
			break;
		}
		case _IOC_NR(COMPAT_IOC_MMB_USER_REMAP_CACHED):
		{
			ret = mmz_userdev_ioctl_m(filp->f_dentry->d_inode, filp,
					IOC_MMB_USER_REMAP_CACHED, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s COMPAT_IOC_MMB_USER_REMAP_CACHED \
						failed!\n", __func__);
				goto exit;
			}
			break;
		}
		case _IOC_NR(COMPAT_IOC_MMB_USER_UNMAP):
		{
			ret = mmz_userdev_ioctl_m(filp->f_dentry->d_inode, filp,
						IOC_MMB_USER_UNMAP, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s COMPAT_IOC_MMB_USER_UNMAP \
						failed!\n", __func__);
				goto exit;
			}

			break;
		}
		case _IOC_NR(COMPAT_IOC_MMB_USER_GETPHYADDR):
		{
			ret = mmz_userdev_ioctl_m(filp->f_dentry->d_inode, filp,
					IOC_MMB_USER_GETPHYADDR, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s COMPAT_IOC_MMB_USER_GETPHYADDR \
						failed!\n", __func__);
				goto exit;
			}

			break;
		}
		case _IOC_NR(COMPAT_IOC_MMB_USER_CMA_MAPTO_SMMU):
		{
			ret = mmz_userdev_ioctl_m(filp->f_dentry->d_inode, filp,
					IOC_MMB_USER_CMA_MAPTO_SMMU, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s \
				COMPAT_IOC_MMB_USER_CMA_MAPTO_SMMU failed!\n",
						__func__);
				goto exit;
			}

			break;
		}
		case _IOC_NR(COMPAT_IOC_MMB_USER_CMA_UNMAPTO_SMMU):
		{
			ret = mmz_userdev_ioctl_m(filp->f_dentry->d_inode, filp,
					IOC_MMB_USER_CMA_UNMAPTO_SMMU, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s \
				COMPAT_IOC_MMB_USER_CMA_UNMAPTO_SMMU \
						failed!\n", __func__);
				goto exit;
			}

			break;
		}
		default:
		{
			HI_PRINT("CMD FAILED!\n");
			ret = -ENOIOCTLCMD;
			goto exit;
		}
		}
		if (_IOC_DIR(cmd) & _IOC_READ) {
			ret = put_meminfo_to_user(&compat_mmb, &mmi);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s put_meminfo_to_user failed!\n",
						__func__);
			}
			if (copy_to_user((void __user *)arg, &compat_mmb,
						_IOC_SIZE(cmd))) {
				ret = -EFAULT;
				HI_PRINT("%s %d copy_from_user failed!\n",
					__func__, __LINE__);
				goto exit;
			}
		}
	} else if(_IOC_TYPE(cmd) == 'c'){
		struct compat_mmb_info compat_mmb;
		struct mmb_info mmi;

		switch(_IOC_NR(cmd)) {
		case _IOC_NR(COMPAT_IOC_MMB_FLUSH_DCACHE):
		{
			if (0 == arg) {
				mmz_flush_all_dcache();
			} else {
				if(_IOC_SIZE(cmd) != sizeof(compat_mmb) || arg == 0) {
					HI_PRINT("_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", _IOC_SIZE(cmd), arg);
					ret = -EINVAL;
					goto exit;
				}
				memset(&compat_mmb, 0, sizeof(compat_mmb));
				memset(&mmi, 0, sizeof(mmi));
				if (copy_from_user(&compat_mmb,
					(void __user *)arg,_IOC_SIZE(cmd))) {
					ret = -EFAULT;
					HI_PRINT("%s %d copy_from_user failed!\n",
						__func__, __LINE__);
					goto exit;
				}

				ret = get_meminfo_from_user(&compat_mmb, &mmi);
				if (ret) {
					ret = -EFAULT;
					HI_PRINT("%s get_meminfo_from_user failed\n",
								__func__);
					goto exit;
				}
				mmz_flush_dcache_mmb(&mmi);
			}
			break;
		}
		default:
		{
			ret = -ENOIOCTLCMD;
			goto exit;
		}
		}

	} else if(_IOC_TYPE(cmd) == 's') {
		struct sec_info si;

		if(_IOC_SIZE(cmd) != sizeof(si) || arg==0) {
			HI_PRINT("_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", _IOC_SIZE(cmd), arg);
			ret = -EINVAL;
			goto exit;
		}
		memset(&si, 0, sizeof(si));
		if (copy_from_user(&si, (void __user *)arg,
					_IOC_SIZE(cmd))) {
			ret = -EFAULT;
			HI_PRINT("%s %d copy_from_user failed!\n",
					__func__, __LINE__);
				goto exit;
		}

		switch(_IOC_NR(cmd)) {
		case _IOC_NR(COMPAT_IOC_MMB_SECSMMU_MAPTOSECSMMU):
		{
			ret = mmz_userdev_ioctl_s(filp->f_dentry->d_inode,filp,
					IOC_MMB_SECSMMU_MAPTOSECSMMU, &si);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s \
				COMPAT_IOC_MMB_SECSMMU_MAPTOSECSMMU failed!\n",
						__func__);
				goto exit;
			}
			break;
		}
		case _IOC_NR(COMPAT_IOC_MMB_SECSMMU_UNMAPFROMSMMU):
		{
			ret = mmz_userdev_ioctl_s(filp->f_dentry->d_inode,filp,
					IOC_MMB_SECSMMU_UNMAPFROMSMMU, &si);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s \
				COMPAT_IOC_MMB_SECSMMU_UNMAPFROMSMMU \
				failed!\n", __func__);
				goto exit;
			}
			break;

		}
		case _IOC_NR(COMPAT_IOC_MMB_SECSMMU_ALLOC):
		{
			ret = mmz_userdev_ioctl_s(filp->f_dentry->d_inode,filp,
					IOC_MMB_SECSMMU_ALLOC, &si);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s \
				COMPAT_IOC_MMB_SECSMMU_ALLOC failed!\n",
						__func__);
				goto exit;
			}
			break;

		}
		case _IOC_NR(COMPAT_IOC_MMB_SECSMMU_FREE):
		{
			ret = mmz_userdev_ioctl_s(filp->f_dentry->d_inode,filp,
					IOC_MMB_SECSMMU_FREE, &si);
			if (ret) {
				ret = -EFAULT;
				HI_PRINT("%s \
				COMPAT_IOC_MMB_SECSMMU_FREE failed!\n",
						__func__);
				goto exit;
			}
			break;
		}
		default:
		{
			ret = -ENOIOCTLCMD;
			goto exit;
		}
		}
		if (_IOC_DIR(cmd) & _IOC_READ) {
			if (copy_to_user((void __user *)arg, &si,
						_IOC_SIZE(cmd))) {
				ret = -EFAULT;
				HI_PRINT("%s %d copy_to_user failed!\n",
					__func__, __LINE__);
				goto exit;
			}
		}
	}

exit:
	up(&pmu->sem);
	return ret;
}
