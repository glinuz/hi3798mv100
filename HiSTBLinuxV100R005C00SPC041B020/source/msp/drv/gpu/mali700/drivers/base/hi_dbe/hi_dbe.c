/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_dbe.c
Version             : Initial Draft
Author              : 
Created             : 2015/09/29
Description         :
Function List       : 
History             :
Date                       Author                   Modification
2015/09/29                 00270170                 Created file        
******************************************************************************/

#include <hi_dbe.h>
#include <linux/dma-buf.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/atomic.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0))
#include <linux/dma-attrs.h>
#include <linux/dma-mapping.h>
#endif

#define HI_DBE_PAGE_SIZE 4096

struct hidbe_alloc {
	/* the real alloc */
	int dbe_pages;
	struct page **pages;

	/* the debug usage tracking */
	int dbe_attached_devices;
	int dbe_device_mappings;
	int dbe_cpu_mappings;

	bool bde_contiguous;
	dma_addr_t dbe_contig_dma_addr;
};

static struct miscdevice s_dbe_device;

static int hidbe_attach(struct dma_buf *pdbe_buf, struct device *pdbe_dev, struct dma_buf_attachment *pdbe_attachment)
{
	struct hidbe_alloc	*pdbe_alloc;
	pdbe_alloc = pdbe_buf->priv;

	/* dma_buf is externally locked during call */
	pdbe_alloc->dbe_attached_devices++;
	
	return 0;
}

static void hidbe_detach(struct dma_buf *pdbe_buf, struct dma_buf_attachment *pdbe_attachment)
{
	struct hidbe_alloc *pdbe_alloc;
	pdbe_alloc = pdbe_buf->priv;

	/* dma_buf is externally locked during call */
	pdbe_alloc->dbe_attached_devices--;
}

static struct sg_table *hidbe_map(struct dma_buf_attachment *pdbe_attachment, enum dma_data_direction dbe_direction)
{
	struct sg_table *pdbe_sg;
	struct scatterlist *pdbe_iter;
	struct hidbe_alloc	*pdbe_alloc;
	int i;
	int ret;

	pdbe_alloc = pdbe_attachment->dmabuf->priv;

#if !(defined(ARCH_HAS_SG_CHAIN) || defined(CONFIG_ARCH_HAS_SG_CHAIN))
	/* if the ARCH can't chain we can't have allocs larger than a single sg can hold */
	if (pdbe_alloc->dbe_pages > SG_MAX_SINGLE_ALLOC)
		return ERR_PTR(-EINVAL);
#endif

	pdbe_sg = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!pdbe_sg)
		return ERR_PTR(-ENOMEM);

	/* from here we access the allocation object, so lock the dmabuf pointing to it */
	mutex_lock(&pdbe_attachment->dmabuf->lock);

	if (pdbe_alloc->bde_contiguous)
		ret = sg_alloc_table(pdbe_sg, 1, GFP_KERNEL);
	else
		ret = sg_alloc_table(pdbe_sg, pdbe_alloc->dbe_pages, GFP_KERNEL);
	if (ret) {
		mutex_unlock(&pdbe_attachment->dmabuf->lock);
		kfree(pdbe_sg);
		return ERR_PTR(ret);
	}

	if (pdbe_alloc->bde_contiguous) {
		sg_dma_len(pdbe_sg->sgl) = pdbe_alloc->dbe_pages * PAGE_SIZE;
		sg_set_page(pdbe_sg->sgl, pfn_to_page(PFN_DOWN(pdbe_alloc->dbe_contig_dma_addr)), pdbe_alloc->dbe_pages * PAGE_SIZE, 0);
		sg_dma_address(pdbe_sg->sgl) = pdbe_alloc->dbe_contig_dma_addr;
	} else {
		for_each_sg(pdbe_sg->sgl, pdbe_iter, pdbe_alloc->dbe_pages, i)
			sg_set_page(pdbe_iter, pdbe_alloc->pages[i], PAGE_SIZE, 0);
	}
#if 0
	/* GPU DDK does not need the virtual address, so we do not map sg here */
	if (!dma_map_sg(pdbe_attachment->dev, pdbe_sg->sgl, pdbe_sg->nents, dbe_direction)) {
		mutex_unlock(&pdbe_attachment->dmabuf->lock);
		sg_free_table(pdbe_sg);
		kfree(pdbe_sg);
		return ERR_PTR(-ENOMEM);
	}
#endif
	pdbe_alloc->dbe_device_mappings++;
	mutex_unlock(&pdbe_attachment->dmabuf->lock);
	return pdbe_sg;
}

static void hidbe_unmap(struct dma_buf_attachment *pdbe_attachment,
							 struct sg_table *pdbe_sg, enum dma_data_direction dbe_direction)
{
	struct hidbe_alloc *pdbe_alloc;

	pdbe_alloc = pdbe_attachment->dmabuf->priv;

#if 0
	/* GPU DDK does not need the virtual address, so we do not map sg in map_dma_buf */
	dma_unmap_sg(pdbe_attachment->dev, pdbe_sg->sgl, pdbe_sg->nents, dbe_direction);
#endif	
	sg_free_table(pdbe_sg);
	kfree(pdbe_sg);

	mutex_lock(&pdbe_attachment->dmabuf->lock);
	pdbe_alloc->dbe_device_mappings--;
	mutex_unlock(&pdbe_attachment->dmabuf->lock);
}

static void hidbe_release(struct dma_buf *pdbe_buf)
{
	struct hidbe_alloc *pdbe_alloc;
	pdbe_alloc = pdbe_buf->priv;

	kfree(pdbe_alloc->pages);
	kfree(pdbe_alloc);
}


static void hidbe_mmap_open(struct vm_area_struct *pdbe_vma)
{
	struct dma_buf *pdbe_dma_buf;
	struct hidbe_alloc *pdbe_alloc;
	pdbe_dma_buf = pdbe_vma->vm_private_data;
	pdbe_alloc = pdbe_dma_buf->priv;

	mutex_lock(&pdbe_dma_buf->lock);
	pdbe_alloc->dbe_cpu_mappings++;
	mutex_unlock(&pdbe_dma_buf->lock);
}

static void hidbe_mmap_close(struct vm_area_struct *pdbe_vma)
{
	struct dma_buf *pdbe_dma_buf;
	struct hidbe_alloc *pdbe_alloc;
	pdbe_dma_buf = pdbe_vma->vm_private_data;
	pdbe_alloc = pdbe_dma_buf->priv;

	BUG_ON(pdbe_alloc->dbe_cpu_mappings <= 0);
	mutex_lock(&pdbe_dma_buf->lock);
	pdbe_alloc->dbe_cpu_mappings--;
	mutex_unlock(&pdbe_dma_buf->lock);
}

static int hidbe_mmap_fault(struct vm_area_struct *pdbe_vma, struct vm_fault *pdbe_vmf)
{
	struct hidbe_alloc *pdbe_alloc;
	struct dma_buf *pdbe_dmabuf;
	struct page *pdbe_pageptr;

	pdbe_dmabuf = pdbe_vma->vm_private_data;
	pdbe_alloc = pdbe_dmabuf->priv;

	if (pdbe_vmf->pgoff > pdbe_alloc->dbe_pages)
		return VM_FAULT_SIGBUS;

	pdbe_pageptr = pdbe_alloc->pages[pdbe_vmf->pgoff];

	BUG_ON(!pdbe_pageptr);

	get_page(pdbe_pageptr);
	pdbe_vmf->page = pdbe_pageptr;

	return 0;
}

struct vm_operations_struct s_dbe_vm_ops = {
	.open = hidbe_mmap_open,
	.close = hidbe_mmap_close,
	.fault = hidbe_mmap_fault
};

static int hidbe_mmap(struct dma_buf *pdbe_dmabuf, struct vm_area_struct *pdbe_vma)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
	pdbe_vma->vm_flags |= VM_IO | VM_DONTEXPAND | VM_DONTDUMP;
#else
	pdbe_vma->vm_flags |= VM_RESERVED | VM_IO | VM_DONTEXPAND;
#endif
	pdbe_vma->vm_ops = &s_dbe_vm_ops;
	pdbe_vma->vm_private_data = pdbe_dmabuf;

	/*  we fault in the pages on access */

	/* call open to do the ref-counting */
	s_dbe_vm_ops.open(pdbe_vma);

	return 0;
}

static void *hidbe_kmap_atomic(struct dma_buf *pdbe_buf, unsigned long dbe_page_num)
{
	/* IGNORE */
	return NULL;
}

static void *hidbe_kmap(struct dma_buf *pdbe_buf, unsigned long dbe_page_num)
{
	/* IGNORE */
	return NULL;
}

static struct dma_buf_ops s_dbe_ops = {
	/* real handlers */
	.attach = hidbe_attach,
	.detach = hidbe_detach,
	.map_dma_buf = hidbe_map,
	.unmap_dma_buf = hidbe_unmap,
	.release = hidbe_release,
	.mmap = hidbe_mmap,

	/* nop handlers for mandatory functions we ignore */
	.kmap_atomic = hidbe_kmap_atomic,
	.kmap = hidbe_kmap
};

static int hidbe_ioctl_wrap_func(struct hidbe_ioctl_wrap __user *pdbe_buf)
{
	struct hidbe_ioctl_wrap dbe_alloc_req;
	struct hidbe_alloc *pdbe_alloc;
	struct dma_buf *pdbe_dma_buf;
	int i = 0;
	int fd;

#if !(defined(ARCH_HAS_SG_CHAIN) || defined(CONFIG_ARCH_HAS_SG_CHAIN))
	dev_err(s_dbe_device.this_device, "ARCH_HAS_SG_CHAIN not defined!\n");
#endif

	if (copy_from_user(&dbe_alloc_req, pdbe_buf, sizeof(dbe_alloc_req))) {
		dev_err(s_dbe_device.this_device, "%s: couldn't get user data", __func__);
		goto no_input;
	}

	/*buffer size need to transform into page size*/
	dbe_alloc_req.dbe_size = (dbe_alloc_req.dbe_size  + HI_DBE_PAGE_SIZE - 1) / HI_DBE_PAGE_SIZE;
	
	if (!dbe_alloc_req.dbe_size) {
		dev_err(s_dbe_device.this_device, "%s: no size specified", __func__);
		goto invalid_size;
	}

	pdbe_alloc = kzalloc(sizeof(struct hidbe_alloc), GFP_KERNEL);
	if (NULL == pdbe_alloc) {
		dev_err(s_dbe_device.this_device, "%s: couldn't alloc object", __func__);
		goto no_alloc_object;
	}

	pdbe_alloc->dbe_pages = dbe_alloc_req.dbe_size;
	pdbe_alloc->bde_contiguous = true;

	pdbe_alloc->pages = kzalloc(sizeof(struct page *) * pdbe_alloc->dbe_pages, GFP_KERNEL);
	if (!pdbe_alloc->pages) {
		dev_err(s_dbe_device.this_device,
				"%s: couldn't alloc %d page structures", __func__,
				pdbe_alloc->dbe_pages);
		goto free_alloc_object;
	}

	{
		dma_addr_t dma_aux;

		DEFINE_DMA_ATTRS(attrs);

		dma_set_attr(DMA_ATTR_WRITE_COMBINE, &attrs);

        pdbe_alloc->dbe_contig_dma_addr = dbe_alloc_req.dbe_phyaddr;
		dma_aux = pdbe_alloc->dbe_contig_dma_addr;
		for (i = 0; i < pdbe_alloc->dbe_pages; i++) {
			pdbe_alloc->pages[i] = pfn_to_page(PFN_DOWN(dma_aux));
			dma_aux += PAGE_SIZE;
		}
	} 

	/* alloc ready, let's export it */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0))
	pdbe_dma_buf = dma_buf_export(pdbe_alloc, &s_dbe_ops,
			pdbe_alloc->dbe_pages << PAGE_SHIFT, O_CLOEXEC|O_RDWR, NULL);
#else
	pdbe_dma_buf = dma_buf_export(pdbe_alloc, &s_dbe_ops,
			pdbe_alloc->dbe_pages << PAGE_SHIFT, O_CLOEXEC|O_RDWR);
#endif

	if (IS_ERR_OR_NULL(pdbe_dma_buf)) {
		dev_err(s_dbe_device.this_device, "%s: couldn't export dma_buf", __func__);
		goto no_export;
	}

	/* get fd for buf */
	fd = dma_buf_fd(pdbe_dma_buf, O_CLOEXEC);

	if (fd < 0) {
		dev_err(s_dbe_device.this_device, "%s: couldn't get fd from dma_buf", __func__);
		goto no_fd;
	}

	return fd;

no_fd:
	dma_buf_put(pdbe_dma_buf);
no_export:
	/* i still valid */
	{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0))
		DEFINE_DMA_ATTRS(attrs);

		dma_set_attr(DMA_ATTR_WRITE_COMBINE, &attrs);
		dma_free_attrs(s_dbe_device.this_device,
						pdbe_alloc->dbe_pages * PAGE_SIZE,
						NULL, pdbe_alloc->dbe_contig_dma_addr, &attrs);
#else
		dma_free_writecombine(s_dbe_device.this_device,
								pdbe_alloc->dbe_pages * PAGE_SIZE,
								NULL, pdbe_alloc->dbe_contig_dma_addr);
#endif
	}
	kfree(pdbe_alloc->pages);
free_alloc_object:
	kfree(pdbe_alloc);
no_alloc_object:
invalid_size:
no_input:
	return -EFAULT;
}

static long hidbe_ioctl(struct file *pdbe_file, unsigned int dbe_cmd, unsigned long dbe_arg)
{
	switch (dbe_cmd) {
    case DBE_COMMAND_WRAP:
        return hidbe_ioctl_wrap_func((struct hidbe_ioctl_wrap __user *)dbe_arg);
        
	default:
		return -ENOTTY;
	}
}

static const struct file_operations s_dbe_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = hidbe_ioctl,
	.compat_ioctl = hidbe_ioctl,
};

static int __init hidbe_init(void)
{
	int res;
	s_dbe_device.minor = MISC_DYNAMIC_MINOR;
	s_dbe_device.name = "hi_dbe";
	s_dbe_device.fops = &s_dbe_fops;

	res = misc_register(&s_dbe_device);
	if (res) {
		printk(KERN_WARNING"Misc device registration failed of 'hi_dbe'\n");
		return res;
	}
	s_dbe_device.this_device->coherent_dma_mask = DMA_BIT_MASK(32);

	dev_info(s_dbe_device.this_device, "hi_dbe ready\n");
	return 0;

}

static void __exit hidbe_exit(void)
{
	misc_deregister(&s_dbe_device);
}

module_init(hidbe_init);
module_exit(hidbe_exit);
MODULE_LICENSE("GPL");

