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

#include <linux/mman.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/vmalloc.h>

#include "hi_drv_dev.h"
#include "drv_media_mem.h"
#include "drv_mmz_ioctl.h"
#include "hi_kernel_adapt.h"

#include "drv_tzsmmu.h"
#include "drv_tee_smmu_agent.h"

#ifdef CONFIG_COMPAT
#include "drv_mmz_compat.h"
#endif
#define error(s...) do{ PRINTK_CA(KERN_ERR "mmz_userdev:%s: ", __FUNCTION__); PRINTK_CA(s); }while(0)
#define warning(s...) do{ PRINTK_CA(KERN_WARNING "mmz_userdev:%s: ", __FUNCTION__); PRINTK_CA(s); }while(0)

#define HI_ZERO     0

/****************************fops*********************************/

LIST_HEAD(release_list);
struct release_info {
	hil_mmb_t  *mmb;
	struct list_head list;
};

int mmz_flush_dcache_mmb(struct mmb_info *pmi)
{
	hil_mmb_t *mmb;
	struct mmb_udata *udata = NULL;
	struct mmb_udata *p;
	unsigned int phyaddr;
	unsigned int iommu;

	/*
	 * If pmi->phys_addr is not  MMB_ADDR_INVALID, pmi->phys_addr
	 * is userd first.Because pmi->smmu_addr may be 0 and it is legal at
	 * the same time, but pmi->phys_addr hasn't such feature.So phys_addr
	 * is stricter.
	 */
	if (MMB_ADDR_INVALID != pmi->phys_addr) {
		phyaddr = pmi->phys_addr;
		iommu = 0;
	} else {
		phyaddr = pmi->smmu_addr;
		iommu = 1;
	}

	if (NULL == (mmb = hil_mmb_getby_phys(phyaddr, iommu))) {
		HI_PRINT("%s: err args!\n",__func__);
		return -EINVAL;
	}

	if (list_empty(&mmb->ulist)) {
		if (iommu)
			HI_PRINT("mmb smmu:0x%x have not mapped yet!\n",
							(unsigned int)phyaddr);
		else
			HI_PRINT("mmb phy:0x%x have not mapped yet!\n",
							(unsigned int)phyaddr);

		return -EINVAL;
	}

	list_for_each_entry (p, &mmb->ulist, list) {
		if (p->tgid == current->tgid) {
			udata = p;	
			break;
		}
	}

	if (NULL == udata || udata->map_cached == 0) {
		PRINTK_CA("%s->%d,error!\n", __func__,__LINE__);
		return -EINVAL;
	}

	flush_inner_cache((void *)udata->uvirt, mmb->length);
	flush_outer_cache_range(phyaddr, mmb->length, iommu);

	return 0;
}

int mmz_flush_all_dcache(void)
{
#ifdef CONFIG_SMP
        on_each_cpu((smp_call_func_t)mmz_flush_dcache_all, NULL, 1);
#else
        mmz_flush_dcache_all();
#endif

#ifndef CONFIG_64BIT
	/* just for A9 core */
	outer_flush_all();
#endif
	return 0;
}

int mmz_userdev_open(struct inode *inode, struct file *file)
{
	struct mmz_userdev_info *pmu;

	pmu = kmalloc(sizeof(*pmu), GFP_KERNEL);
	if(pmu ==NULL) {
		error("alloc mmz_userdev_info failed!\n");
		return -ENOMEM;
	}
	pmu->tpid = current->tgid;
	HI_INIT_MUTEX(&pmu->sem);
	pmu->mmap_tpid = 0;
	file->private_data = (void *)pmu;

	return HI_SUCCESS;
}

static int ioctl_mmb_alloc(struct file *file, unsigned int iocmd, struct mmb_info *pmi)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_alloc(pmi->mmb_name, pmi->size, pmi->align, pmi->mmz_name,
														HI_USER_ALLOC);
	if (NULL == mmb) {
		return -ENOMEM;
	}

	if (mmb->iommu)
		pmi->smmu_addr = mmb->iommu_addr;
	else
		pmi->phys_addr = mmb->phys_addr;

	return HI_SUCCESS;

}

hil_mmb_t *get_mmb_info(unsigned int addr, unsigned int iommu)
{
	hil_mmb_t *p;

	p = hil_mmb_getby_phys(addr, iommu);

	if (NULL == p) {
		HI_PRINT("mmb(0x%08X) not found!\n", addr);
		return NULL;
	}

	return p;
}

static int ioctl_mmb_user_unmap(struct file *file, unsigned int iocmd, struct mmb_info *pmi);

static int ioctl_mmb_free(struct file *file, unsigned int iocmd, struct mmb_info *pmi)
{
	int ret = 0;
	unsigned int iommu;
	unsigned int phyaddr;
	hil_mmb_t *mmb;

	if (MMB_ADDR_INVALID != pmi->phys_addr) {
		phyaddr = pmi->phys_addr;
		iommu = 0;
	} else {
		phyaddr = pmi->smmu_addr;
		iommu = 1;
	}

	mmb = hil_mmb_getby_phys(phyaddr, iommu);

	if (NULL == mmb) {
		HI_PRINT("%s : mmb free failed!\n",__func__);
		return -EPERM;
	}

	ret = hil_mmb_free(mmb);
	
	return ret;
}

static int ioctl_mmb_user_remap(struct file *file, unsigned int iocmd, struct mmb_info *pmi, int cached)
{
	struct mmz_userdev_info *pmu = file->private_data;
	unsigned long prot, flags, len, pgoff, addr;
	hil_mmb_t *mmb;	
	struct mmb_udata *udata;
	struct mmb_udata *p;
	unsigned long round_up_len;
	unsigned int iommu;
	unsigned int phyaddr;

	/*
	 * If pmi->phys_addr is not MMB_ADDR_INVALID , pmi->phys_addr
	 * is userd first.Because pmi->smmu_addr may be 0 and it is legal at
	 * the same time, but pmi->phys_addr hasn't such feature.So phys_addr
	 * is stricter.
	 */
	if (MMB_ADDR_INVALID != pmi->phys_addr) {
		phyaddr = pmi->phys_addr;
		iommu = 0;
	} else {
		phyaddr = pmi->smmu_addr;
		iommu = 1;
	}

	mmb = get_mmb_info(phyaddr, iommu);
	if (NULL == mmb)
		return -EPERM;
	prot = pmi->prot;
	flags = pmi->flags;
	pmu->mmap_tpid = current->tgid;

	if ((mmb->map_ref > 0) && (!list_empty(&mmb->ulist)) ) {
		list_for_each_entry (p, &mmb->ulist, list) {
			if (p->tgid == current->tgid) {
				if (p->map_cached == cached) {
					udata = p;
					mmb->phy_ref++;
					mmb->map_ref++;
					pmi->mapped = udata->uvirt;
					udata->map_ref++;	
					return 0;
				} else {
					HI_PRINT("mmb<%s> already mapped one cache \
						attr, can not be remap to other \
						attr\n", mmb->name);
					return -EINVAL;
				}
			}
		}
	}

	/*
	 * ion_map framwork is used here,so rule must fit to ion ramwork.
	 * vma->vm_pgoff is set to 0. pgoff will be passed to vma->vm_pgoff.  
	 */
	pgoff = 0;
	udata = kzalloc(sizeof(struct mmb_udata), GFP_KERNEL);

	addr = 0;
	len = PAGE_ALIGN(mmb->length);

	if(prot == 0)
		prot = PROT_READ | PROT_WRITE;
	if(flags == 0)
		flags = MAP_SHARED;

	down_write(&current->mm->mmap_sem);
	udata->tgid = current->tgid;
	udata->map_cached = cached;

	/*
	 * The following 3  structures  are needed in mmap function.
	 */
	pmu->mmap_tpid = current->tgid;
	pmu->private_data = udata;
	pmu->tmp = mmb;

	addr = do_mmap_pgoff(file, addr, len, prot, flags, pgoff, &round_up_len);
	up_write(&current->mm->mmap_sem);
	if(IS_ERR_VALUE(addr)) {
		error("do_mmap(file, 0, %lu, 0x%08lX, 0x%08lX, 0x%08lX) return 0x%08lX\n", 
				len, prot, flags, pgoff, addr);
		kfree(udata);
		return HI_FAILURE;
	}

	/*
	 * Following 3 parameters need to clear for next remap operation.
	 */
	pmu->mmap_tpid = 0;
	pmu->tmp = NULL;
	pmu->private_data = NULL;
	
	udata->uvirt = (void *)addr;
	udata->map_ref++;

	list_add_tail(&udata->list, &mmb->ulist);

	mmb->phy_ref++;
	mmb->map_ref++;
	pmi->mapped = udata->uvirt;

	return 0;
}

static int ioctl_mmb_user_unmap(struct file *file, unsigned int iocmd, struct mmb_info *pmi)
{
	int ret = 0;
	unsigned long addr;
	unsigned int len;
	hil_mmb_t *mmb;	
	struct mmb_udata *udata = NULL;
	struct mmb_udata *p;
	unsigned int iommu;
	unsigned int phyaddr;
	int ref;

	/*
	 * If pmi->phys_addr is not MMB_ADDR_INVALID, pmi->phys_addr
	 * is userd first.Because pmi->smmu_addr may be 0 and it is legal at
	 * the same time, but pmi->phys_addr hasn't such feature.So phys_addr
	 * is stricter.
	 */
	if (MMB_ADDR_INVALID != pmi->phys_addr) {
		phyaddr = pmi->phys_addr;
		iommu = 0;
	} else {
		phyaddr = pmi->smmu_addr;
		iommu = 1;
	}

	if (NULL == (mmb = get_mmb_info(phyaddr, iommu)))
		return -EPERM;
	
	if(!mmb->map_ref) {
		PRINTK_CA(KERN_WARNING "%s addr(0x%08X) have'nt been mapped yet!\n",
			mmb->name, mmb->iommu ? mmb->iommu_addr : mmb->phys_addr);
		pmi->mapped = NULL;
		return -EIO;
	}

	if (list_empty(&mmb->ulist)) {
		PRINTK_CA(KERN_WARNING "%s addr(0x%08X) have'nt been user-mapped yet!\n",
			mmb->name, mmb->iommu ? mmb->iommu_addr : mmb->phys_addr);
		pmi->mapped = NULL;
		return -EIO;
	}

	list_for_each_entry (p, &mmb->ulist, list) {
		if (current->tgid == p->tgid){
			udata = p;
			break;
		}
	}

	if (NULL == udata) {
		/*
		 * do not call mmap yourself or unmap is called when
		 * process is killed
		 */
		HI_PRINT("do not call mmap() yourself!\n");
		return 0;
	}

	addr = (unsigned long)udata->uvirt;
	len  = PAGE_ALIGN(mmb->length);

	if (udata->map_cached) {
		mmz_flush_dcache_area((void *)udata->uvirt, (size_t)mmb->length);
		flush_outer_cache_range(phyaddr, mmb->length, iommu);
	}

	ref = udata->map_ref - 1;
	udata->unmap_flag = (udata->unmap_flag | NORMAL_FLAG);

	if (!ref) {
		/*
		* we register struct vm_operations_struct when mmap called
		* so ref count is decreased in vm_operations_struct->close,
		*/
		down_write(&current->mm->mmap_sem);
		sys_close(udata->map_fd);
		ret = do_munmap(current->mm, addr, len);
		up_write(&current->mm->mmap_sem);

		if(!IS_ERR_VALUE(ret)) {
			/** the memory may be used by system later so clean the L2 cache(L1 cache is guaranted by do_munmap)  **/
			udata->uvirt = NULL;
		}
		
		list_del(&udata->list);
		kfree(udata);
	} else {
		udata->map_ref--;
		mmb->map_ref--;
		mmb->phy_ref--;
	}

	pmi->mapped = NULL;

	if ((0 == (mmb->phy_ref)) && (0 == mmb->map_ref) && (0 == mmb->cma_smmu_ref)) {
		/* free mmb   */
		hil_mmb_free(mmb);
	}

	return ret;
}

// find mmbinfo by use addr
static hil_mmb_t *get_mmbinfo_byusraddr (unsigned long addr, struct mmb_udata *udata)
{
	hil_mmb_t *mmb = NULL;
	struct mmb_udata *p;
	hil_mmz_t *zone;

	if (addr == (unsigned long)NULL)
	{
		return NULL;
	}

	down(&mmz_lock);
	list_for_each_entry(zone, &mmz_list, list) {
		struct rb_node *n;
		for (n = rb_first(&zone->root); n; n = rb_next(n)) {
			hil_mmb_t *m;
			if (zone->iommu)
				m = rb_entry(n, hil_mmb_t, s_node);
			else
				m = rb_entry(n, hil_mmb_t, node);

			if ((!m->map_ref) && list_empty(&m->ulist))
				continue;
			list_for_each_entry(p, &m->ulist, list) {
				if ((p->tgid == current->tgid)
					&& ((unsigned long)p->uvirt <= addr)
					&& ((unsigned long)p->uvirt + m->length > addr)){
					mmb = m;
					memcpy(udata,p,sizeof(struct mmb_udata));
					goto end;
				}
			}
		}
	}
end:
	up(&mmz_lock);
	return mmb;
}

static int ioctl_mmb_user_getphyaddr(struct file *file, unsigned int iocmd, struct mmb_info *pmi)
{
	hil_mmb_t *p;
	struct mmb_udata udata;

	memset(&udata, 0, sizeof(udata));
	p = get_mmbinfo_byusraddr((unsigned long)pmi->mapped, &udata);
	if ((NULL == p))
		return -EPERM;
	if (p->iommu_addr != MMB_ADDR_INVALID)
		pmi->smmu_addr = p->iommu_addr + ((unsigned long)pmi->mapped -
					(unsigned long)udata.uvirt);

	if (p->phys_addr != MMB_ADDR_INVALID)
		pmi->phys_addr = p->phys_addr + ((unsigned long)pmi->mapped -
					(unsigned long)udata.uvirt);

	pmi->size = p->length - ((unsigned long)pmi->mapped - (unsigned long)udata.uvirt);
	return 0;
}

static int ioctl_mmb_user_cma_mapto_iommu(struct file *file, unsigned int iocmd, struct mmb_info *pmi)
{
	mmb_addr_t addr;

	addr = hil_mmb_cma_mapto_iommu (pmi->phys_addr, 0);
	if (addr == MMB_ADDR_INVALID) {
		HI_PRINT("%s: Phys:0x%x  cma mapto smmu failed!\n",__func__, (unsigned int)pmi->phys_addr);
		return -EPERM;
	}
	pmi->smmu_addr = addr;
	return 0;
}

static int ioctl_mmb_user_cma_unmapfrom_iommu(struct file *file, unsigned int iocmd, struct mmb_info *pmi)
{
	int ret;

	ret = hil_mmb_cma_unmapfrom_iommu(pmi->smmu_addr, 1);
	if (ret == HI_FAILURE) {
		HI_PRINT("%s: smmu:0x%x  cma unmapfrom smmu failed!\n",__func__, (unsigned int)pmi->smmu_addr);
		return -EPERM;
	}
	return HI_SUCCESS;
}

int mmz_userdev_ioctl_m(struct inode *inode, struct file *file, unsigned int cmd, struct mmb_info *pmi)
{
	int ret = 0;
	switch(_IOC_NR(cmd)) {
		case _IOC_NR(IOC_MMB_ALLOC):
			ret = ioctl_mmb_alloc(file, cmd, pmi);
			break;
		case _IOC_NR(IOC_MMB_FREE):
			ret = ioctl_mmb_free(file, cmd, pmi);
			break;

		case _IOC_NR(IOC_MMB_USER_REMAP):
			ret = ioctl_mmb_user_remap(file, cmd, pmi, 0);
			break;
		case _IOC_NR(IOC_MMB_USER_REMAP_CACHED):
			ret = ioctl_mmb_user_remap(file, cmd, pmi, 1);
			break;
		case _IOC_NR(IOC_MMB_USER_UNMAP):
			ret = ioctl_mmb_user_unmap(file, cmd, pmi);
			break;
		case _IOC_NR(IOC_MMB_USER_GETPHYADDR):
			ret = ioctl_mmb_user_getphyaddr(file, cmd, pmi);
			break;
		case _IOC_NR(IOC_MMB_USER_CMA_MAPTO_SMMU):
			ret = ioctl_mmb_user_cma_mapto_iommu(file, cmd, pmi);
			break;
		case _IOC_NR(IOC_MMB_USER_CMA_UNMAPTO_SMMU):
			ret = ioctl_mmb_user_cma_unmapfrom_iommu(file, cmd, pmi);
			break;
		default:
			error("invalid ioctl cmd = %08X\n", cmd);
			ret = -EINVAL;
			break;
	}

	return ret;
}

static int ioctl_mmb_secsmmu_maptosecsmmu(struct file *file, unsigned int cmd,
							struct sec_info *psi)
{
	int iommu;

	if (NULL == psi) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}

	if (psi->phys_addr) {
		psi->sec_smmu = secmem_map_to_secsmmu(psi->phys_addr, 0);
		iommu = 0;
	} else {
		psi->sec_smmu = secmem_map_to_secsmmu(psi->nosec_smmu, 1);
		iommu = 1;
	}

	if (MMB_ADDR_INVALID == psi->sec_smmu) {
		if (iommu)
			HI_PRINT("%s map to sec smmu failed, nosec smmu :0x%x \n",
							__func__, psi->nosec_smmu);
		else
			HI_PRINT("%s map to sec smmu failed, phys:0x%x\n",
							__func__, psi->phys_addr);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

static int ioctl_mmb_secsmmu_unmapfromsmmu(struct file *file, unsigned int cmd,
							struct sec_info *psi)
{
	hil_mmb_t *mmb = NULL;
	int ret = 0;

	if (NULL == psi) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}

	mmb = hil_mmb_getby_sec_addr(psi->sec_smmu, 1);
	if (NULL == mmb) {
		HI_PRINT("%s cannot find mem by sec_smmu:0x%x \n",
					__func__, psi->sec_smmu);
		return HI_FAILURE;
	}

	ret = secmem_unmap_from_secsmmu(psi->sec_smmu, mmb->iommu);
	if (ret) {
		HI_PRINT("%s unmap from sec smmu failed,sec_smmu:0x%x \n",
					__func__, psi->sec_smmu);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

static int ioctl_mmb_secsmmu_alloc(struct file *file, unsigned int cmd,
							struct sec_info *psi)
{
	hil_mmb_t *mmb;

	if (NULL == psi) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}
	mmb = hil_mmb_alloc(psi->mmb_name, psi->size, psi->align, psi->mmz_name,
								HI_USER_ALLOC);
	if (NULL == mmb) {
		HI_PRINT("%s: alloc failed!\n", __func__);
		return HI_FAILURE;
	}

	if (mmb->iommu)
		psi->sec_smmu = secmem_alloc(mmb->iommu_addr, mmb->iommu);
	else
		psi->phys_addr = secmem_alloc(mmb->phys_addr, mmb->iommu);

	if ((MMB_ADDR_INVALID == psi->sec_smmu) &&
			(MMB_ADDR_INVALID == psi->phys_addr)) {
		hil_mmb_free(mmb);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

static int ioctl_mmb_secsmmu_free(struct file *file, unsigned int cmd,
							struct sec_info *psi)
{
	hil_mmb_t *mmb;
	unsigned long normal_addr;
	int iommu;

	if (NULL == psi) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}

	if (psi->phys_addr) {
		normal_addr = secmem_free(psi->phys_addr, 0);
		iommu = 0;
	} else {
		normal_addr = secmem_free(psi->sec_smmu, 1);
		iommu = 1;
	}

	if (MMB_ADDR_INVALID == normal_addr) {
		if (iommu)
			HI_PRINT("%s  secmem_free failed, sec_smmu:0x%x \n",
						__func__, psi->sec_smmu);
		else
			HI_PRINT("%s  secmem_free failed, phys_addr:0x%x \n",
						__func__, psi->phys_addr);
		return HI_FAILURE;
	}

	mmb = hil_mmb_getby_phys(normal_addr, iommu);
	if (NULL == mmb) {
		HI_PRINT("%s: free failed!\n", __func__);
		return HI_FAILURE;
	}

	hil_mmb_free(mmb);
	return HI_SUCCESS;
}

int mmz_userdev_ioctl_s(struct inode *inode, struct file *file, unsigned int cmd, struct sec_info *psi)
{
	int ret = 0;

	switch(_IOC_NR(cmd)) {
		case _IOC_NR(IOC_MMB_SECSMMU_ALLOC):
			ret = ioctl_mmb_secsmmu_alloc(file, cmd, psi);
			break;
		case _IOC_NR(IOC_MMB_SECSMMU_FREE):
			ret = ioctl_mmb_secsmmu_free(file, cmd, psi);
			break;
		case _IOC_NR(IOC_MMB_SECSMMU_MAPTOSECSMMU):
			ret = ioctl_mmb_secsmmu_maptosecsmmu(file, cmd, psi);
			break;
		case _IOC_NR(IOC_MMB_SECSMMU_UNMAPFROMSMMU):
			ret = ioctl_mmb_secsmmu_unmapfromsmmu(file, cmd, psi);
			break;
		default:
			error("invalid ioctl cmd = %08X\n", cmd);
			ret = -EINVAL;
			break;
	}
	return ret;
}

long mmz_userdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	struct mmz_userdev_info *pmu = file->private_data;

	down(&pmu->sem);

	if(_IOC_TYPE(cmd) == 'm') {
		struct mmb_info mi;

		if(_IOC_SIZE(cmd) != sizeof(mi) || arg == 0) {
			error("_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", _IOC_SIZE(cmd), arg);
			ret = -EINVAL;
			goto __error_exit;
		}
		memset(&mi, 0, sizeof(mi));
		if (copy_from_user(&mi, (void*)arg, _IOC_SIZE(cmd)))
		{
			PRINTK_CA("\nmmz_userdev_ioctl: copy_from_user error.\n");
			ret = -EFAULT;
			goto __error_exit;
		}

		ret = mmz_userdev_ioctl_m(file->f_dentry->d_inode,file, cmd, &mi);

		if(!ret && (cmd&IOC_OUT))
		{
			if (copy_to_user((void*)arg, &mi, _IOC_SIZE(cmd)))
			{
				PRINTK_CA("\nmmz_userdev_ioctl: copy_to_user error.\n");
				ret = -EFAULT;
				goto __error_exit;
			}
		}
	} else if(_IOC_TYPE(cmd) == 's') {
		struct sec_info si;

		/* for trustecocore TA   */
		if(_IOC_SIZE(cmd) != sizeof(si) || arg == 0) {
			error("_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", _IOC_SIZE(cmd), arg);
			ret = -EINVAL;
			goto __error_exit;
		}
		memset(&si, 0, sizeof(si));
		if (copy_from_user(&si, (void*)arg, _IOC_SIZE(cmd)))
		{
			PRINTK_CA("\nmmz_userdev_ioctl: copy_from_user error.\n");
			ret = -EFAULT;
			goto __error_exit;
		}

		ret = mmz_userdev_ioctl_s(file->f_dentry->d_inode,file, cmd, &si);

		if(!ret && (cmd&IOC_OUT))
		{
			if (copy_to_user((void*)arg, &si, _IOC_SIZE(cmd)))
			{
				PRINTK_CA("\nmmz_userdev_ioctl: copy_to_user error.\n");
				ret = -EFAULT;
				goto __error_exit;
			}
		}
	} else if(_IOC_TYPE(cmd) == 'c') {
		if (0 == arg){
			mmz_flush_all_dcache();	
			goto __error_exit;
		}
		switch(_IOC_NR(cmd)) {
			case _IOC_NR(IOC_MMB_FLUSH_DCACHE):
			{
				struct mmb_info mmi;

				if(_IOC_SIZE(cmd) != sizeof(mmi) || arg == 0) {
						error("_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", _IOC_SIZE(cmd), arg);
						ret = -EINVAL;
						goto __error_exit;
				}
				memset(&mmi, 0, sizeof(mmi));
				if (copy_from_user(&mmi, (void *)arg, _IOC_SIZE(cmd)))
				{
					PRINTK_CA("\nmmz_userdev_ioctl: copy_from_user error.\n");
					ret = -EFAULT;
					goto __error_exit;
				}
				mmz_flush_dcache_mmb(&mmi);
				break;
			}
			default:
				ret = -EINVAL;
				break;
		}
	} else {
		ret = -EINVAL;
	}

__error_exit:

	up(&pmu->sem);

	return ret;
}


static void mmz_vm_open(struct vm_area_struct *vma)
{
	return;
}

static void mmz_vm_close(struct vm_area_struct *vma)
{
	hil_mmb_t *mmb = vma->vm_private_data;
	struct mmb_udata *q, *p;
	int ref;
	int ret;

	ret = list_empty(&mmb->ulist);
	if (!ret) {
		list_for_each_entry_safe(q, p, &mmb->ulist, \
				list) {
			if (q->tgid != current->tgid)
				continue;
			if (q->unmap_flag & NORMAL_FLAG) {
				/* normal unmap   */
				q->map_ref--;
				mmb->map_ref--;
				mmb->phy_ref--;
				goto exit;
			}
			 ref = q->map_ref;
			 while (ref) {
				 q->map_ref--;
				 mmb->map_ref--;
				 mmb->phy_ref--;
				 ref = q->map_ref;
			}
			break;
		}
	}

exit:
	return;
}

static struct vm_operations_struct mmz_vma_ops = {
        .open = mmz_vm_open,
		.close = mmz_vm_close,
};

int mmz_userdev_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct mmz_userdev_info *pmu = file->private_data;
	hil_mmb_t *mmb = pmu->tmp;
	struct mmb_udata *udata = pmu->private_data;
	int map_fd;
	struct file *map_file;
	unsigned int cached;
	int ret;
	
	if(current->tgid != pmu->mmap_tpid) {
		error("do not call mmap() yourself!\n");
		return -EPERM;
	}

	if (mmb == NULL) {
		error("sys err\n");
		return -EPERM;
	}
	
	if (NULL == udata) {
		error("udata is not found!\n");
		return -EPERM;
	}

	/*
	 * only ION_FLAG_CACHED is meanings mapped cached and build map when page
	 * is used in Missing page exception.
	 * ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC means cached mapped and
	 * build map as soon as the func is called.
	 */
	if (udata->map_cached)
		cached = ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC;
	else
		cached = 0;

	set_buffer_cached(mmb->client, mmb->handle, cached);
	map_fd = ion_share_dma_buf_fd(mmb->client, mmb->handle);
	map_file = fget(map_fd); 
	if (!map_file)
		goto err;

	udata->map_fd = map_fd;

	if (!map_file->f_op->mmap) {
		goto err;
	}

	/*
	 * Ref count is decrease in vma->vm_ops->closed. And this satisfy:
	 * 1 we can decrease count of ref before release func is called
	 * 2 no effect in normal operation process
	 */
	vma->vm_private_data = mmb;
	vma->vm_ops = &mmz_vma_ops;
	mmz_vm_open(vma);

	ret = map_file->f_op->mmap(map_file, vma);
	fput(map_file);
	return ret;
err:
	return -EBADF;

}

/* need flush ?????  */
static void force_mmb_free(hil_mmb_t *mmb)
{
	struct mmb_kdata *p;
	int ref;

	if (NULL == mmb) {
		HI_PRINT("%s: err args ,free mmb failed\n", __func__);
		return;
	}

	while(mmb->sec_smmu_ref) {
		if (mmb->sec_flag) {
			if (mmb->iommu)
				secmem_free(mmb->sec_smmu, 1);
			else
				secmem_free(mmb->phys_addr, 0);
		} else
			secmem_unmap_from_secsmmu(mmb->sec_smmu, mmb->iommu);
	}


	if (!mmb->iommu) {
		while (mmb->cma_smmu_ref)
			hil_mmb_cma_unmapfrom_iommu(mmb->iommu_addr, !mmb->iommu);
	}

	p = mmb->kdata;
	if (p) {
		ref = p->kmap_ref;
		while (ref) {
			p->kmap_ref--;
			mmb->map_ref--;
			mmb->phy_ref--;
			ref = p->kmap_ref;
			if (!p->kmap_ref) {
				/* need to check as user space*/
				vunmap(p->kvirt);
				kfree(p);
			}
		}
	}
	hil_mmb_free(mmb);
}

static void force_mem_free(void)
{
	struct release_info *p, *q;
	int ret;

	ret = list_empty(&release_list);
	if (ret) {
		/* no need to release   */
		return;
	}

	list_for_each_entry_safe(p, q, &release_list, list) {
		force_mmb_free(p->mmb);
		list_del(&p->list);
		kfree(p);
	}

}

static int add_release_list(hil_mmb_t *m)
{
	struct release_info *info = NULL;

	if (NULL == m) {
		HI_PRINT("%s %d sys err\n", __func__, __LINE__);
		return -1;
	}

	info = kmalloc(sizeof(struct release_info), GFP_KERNEL);
	if (!info) {
		HI_PRINT("no mem, release failed \n");
		return -1;
	}

	info->mmb = m;
	list_add_tail(&info->list, &release_list);

	return 0;
}

int mmz_userdev_release(struct inode *inode, struct file *file)
{
	struct mmz_userdev_info *pmu = file->private_data;
	hil_mmz_t *zone, *z;
	struct mmb_udata *q, *p;
	int ret;

	down(&pmu->sem);
	down(&mmz_lock);
	list_for_each_entry_safe(zone, z, &mmz_list, list) {
		struct rb_node *n;
		for (n = rb_first(&zone->root); n; n = rb_next(n)) {
			hil_mmb_t *m;
			if (zone->iommu)
				m = rb_entry(n, hil_mmb_t, s_node);
			else
				m = rb_entry(n, hil_mmb_t, node);

			if (m->sec_smmu_ref && !(m->flag & HI_KERNEL_ALLOC)) {
				if (add_release_list(m))
					goto out;
				continue;
			}

			ret = list_empty(&m->ulist);
			if (!ret) {
				list_for_each_entry_safe(q, p, &m->ulist, \
									list) {
					if (q->tgid != current->tgid)
						continue;

					if (!(m->flag & HI_KERNEL_ALLOC)) {
						/*
						 * the buffer allocated in user
						 * space, free here
						 */
					if (add_release_list(m))
						goto out;
					}
				}
			}
		}
	}
out:
	up(&mmz_lock);
	force_mem_free();
	up(&pmu->sem);
	file->private_data = NULL;
	kfree(pmu);

	return 0;
}

static struct file_operations mmz_userdev_fops = {
	.owner		= THIS_MODULE,
	.open		= mmz_userdev_open,
	.release	= mmz_userdev_release,
	.unlocked_ioctl  = mmz_userdev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl   = compat_mmz_userdev_ioctl,
#endif
	.mmap		= mmz_userdev_mmap,
};
#if (1 == HI_PROC_SUPPORT)
static int proc_mmz_read(struct inode *inode, struct file *file)
{
	return single_open(file, mmz_read_proc, PDE_DATA(inode));
}

static struct file_operations proc_mmz_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_mmz_read,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= NULL,
	.release	= single_release,
};

/****************************proc**********************************/
#define MEDIA_MEM_NAME  "media-mem"
static int media_mem_proc_init(void)
{

	struct proc_dir_entry *p;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
	p = create_proc_entry(MEDIA_MEM_NAME, 0444,NULL);
	if(p == NULL)
		return -1;
	p->read_proc = mmz_read_proc;
	p->write_proc = NULL;
#else
	p = proc_create(MEDIA_MEM_NAME, 0444, NULL,&proc_mmz_fops);
	if(p == NULL)
		return -1;
#endif


        return 0;
}

static void media_mem_proc_exit(void)
{
	remove_proc_entry(MEDIA_MEM_NAME, NULL);
}

#endif
/********************init**************************/

static PM_DEVICE_S mmz_userdev = {
	.minor	= HIMEDIA_DYNAMIC_MINOR,
	.name	= "mmz_userdev",
	.owner = THIS_MODULE,
	.app_ops= &mmz_userdev_fops 
};

int DRV_MMZ_ModInit(void)
{
	HI_S32 s32Ret = 0;

	HI_DRV_MMZ_Init();
#if (1 == HI_PROC_SUPPORT)
	media_mem_proc_init();
#endif
	s32Ret = HI_DRV_PM_Register(&mmz_userdev);
	if(s32Ret != HI_SUCCESS)
	{
#if (1 == HI_PROC_SUPPORT)
		media_mem_proc_exit();
#endif
		HI_DRV_MMZ_Exit();
		return HI_FAILURE;
	}
#ifdef MODULE
	HI_PRINT("Load hi_mmz.ko success.\t\t(%s)\n", VERSION_STRING);
#endif

#ifdef HI_SMMU_SUPPORT
    smmu_agent_register();
#endif
	return 0;
}

void DRV_MMZ_ModExit(void)
{
	HI_DRV_PM_UnRegister(&mmz_userdev);
#if (1 == HI_PROC_SUPPORT)
	media_mem_proc_exit();
#endif    
	HI_DRV_MMZ_Exit();
	
#ifdef HI_SMMU_SUPPORT
    smmu_agent_unregister();
#endif

#ifdef MODULE
    HI_PRINT("remove hi_mmz.ko success.\n");
#endif
}


#ifdef MODULE
module_init(DRV_MMZ_ModInit);
module_exit(DRV_MMZ_ModExit);
#endif

EXPORT_SYMBOL(DRV_MMZ_ModInit);
EXPORT_SYMBOL(DRV_MMZ_ModExit);
    

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yangwei");

