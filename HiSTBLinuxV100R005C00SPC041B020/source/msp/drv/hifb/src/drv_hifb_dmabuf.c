/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_dmabuf.c
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifdef CONFIG_DMA_SHARED_BUFFER

/*********************************add include here******************************/
#include <linux/version.h>
#include <linux/dma-buf.h>
#include <linux/highmem.h>
#include <linux/memblock.h>
#include <linux/slab.h>
#include "hi_gfx_comm_k.h"
#include "drv_hifb_mem.h"

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/
struct hifb_memblock_pdata{
    phys_addr_t phys_base;
};

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
static struct sg_table *hifb_memblock_map(struct dma_buf_attachment *attach, enum dma_data_direction direction)
{
    struct hifb_memblock_pdata *pfbdata = attach->dmabuf->priv;
    unsigned long pfbfn       = PFN_DOWN(pfbdata->phys_base);
    struct page *pfbage       = pfn_to_page(pfbfn);
    struct sg_table *pfbtable = NULL;
    int s32Ret = 0;

    pfbtable = HI_GFX_KZALLOC(HIGFX_FB_ID,sizeof(*pfbtable), GFP_KERNEL);
    if(!pfbtable){
        HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
        return ERR_PTR(-ENOMEM);
    }

    s32Ret = sg_alloc_table(pfbtable, 1, GFP_KERNEL);
    if (s32Ret < 0)
    {
        goto err;
    }

    sg_set_page(pfbtable->sgl, pfbage, attach->dmabuf->size, 0);
    dma_map_sg(attach->dev, pfbtable->sgl, 1, direction);

    return pfbtable;

err:
    HI_GFX_KFREE(HIGFX_FB_ID,pfbtable);
    return ERR_PTR(s32Ret);
}

static void hifb_memblock_unmap(struct dma_buf_attachment *attach, struct sg_table *pfbtable, enum dma_data_direction direction)
{
    UNUSED(attach);
    UNUSED(direction);
    if(NULL == pfbtable){
        return;
    }
    dma_unmap_sg(attach->dev, pfbtable->sgl, 1, direction);
    sg_free_table(pfbtable);
    HI_GFX_KFREE(HIGFX_FB_ID,pfbtable);
}

static void __init_memblock hifb_memblock_release(struct dma_buf *buf)
{
    struct hifb_memblock_pdata *pfbdata = NULL;
    if(NULL == buf){
        return;
    }
    pfbdata = buf->priv;
    if(NULL == pfbdata){
        return;
    }
    HI_GFX_KFREE(HIGFX_FB_ID,pfbdata);
    buf->priv = NULL;
}

static void *hifb_memblock_do_kmap(struct dma_buf *buf, unsigned long pgoffset,bool atomic)
{
    struct hifb_memblock_pdata *pfbdata = buf->priv;
    unsigned long pfbfn = PFN_DOWN(pfbdata->phys_base) + pgoffset;
    struct page *pfbage = pfn_to_page(pfbfn);

    if (atomic){
        return kmap_atomic(pfbage);
    }
    else{
        return kmap(pfbage);
    }
}

static void *hifb_memblock_kmap_atomic(struct dma_buf *buf,unsigned long pgoffset)
{
    return hifb_memblock_do_kmap(buf, pgoffset, true);
}

static void hifb_memblock_kunmap_atomic(struct dma_buf *buf,unsigned long pgoffset, void *vaddr)
{
    kunmap_atomic(vaddr);
}

static void *hifb_memblock_kmap(struct dma_buf *buf, unsigned long pgoffset)
{
    return hifb_memblock_do_kmap(buf, pgoffset, false);
}

static void hifb_memblock_kunmap(struct dma_buf *buf, unsigned long pgoffset,void *vaddr)
{
    kunmap(vaddr);
}

static int hifb_memblock_mmap(struct dma_buf *buf, struct vm_area_struct *vma)
{
    struct hifb_memblock_pdata *pfbdata = NULL;
    if(NULL == buf){
        return -1;
    }
    pfbdata = buf->priv;
    if(NULL == pfbdata){
        return -1;
    }
    vma->vm_page_prot =  pgprot_writecombine(vma->vm_page_prot);
    return remap_pfn_range(vma, vma->vm_start, PFN_DOWN(pfbdata->phys_base) + vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot);
}


struct dma_buf_ops hifb_memblock_ops = {
    .map_dma_buf     = hifb_memblock_map,
    .unmap_dma_buf   = hifb_memblock_unmap,
    .release         = hifb_memblock_release,
    .kmap_atomic     = hifb_memblock_kmap_atomic,
    .kunmap_atomic   = hifb_memblock_kunmap_atomic,
    .kmap            = hifb_memblock_kmap,
    .kunmap          = hifb_memblock_kunmap,
    .mmap            = hifb_memblock_mmap,
};

/**
 * hifb_memblock_export - export a memblock reserved area as a dma-buf
 *
 * @base: base physical address
 * @size: memblock size
 * @flags: mode flags for the dma-buf's file
 *
 * @base and @size must be page-aligned.
 *
 * Returns a dma-buf on success or ERR_PTR(-errno) on failure.
 */
struct dma_buf *hifb_memblock_export(phys_addr_t phys_base, size_t size, int flags)
{
    struct hifb_memblock_pdata *pfbdata = NULL;
    struct dma_buf *pfbbuf = NULL;

    if (PAGE_ALIGN(phys_base) != phys_base || PAGE_ALIGN(size) != size){
        HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
        return ERR_PTR(-EINVAL);
    }

    pfbdata = HI_GFX_KZALLOC(HIGFX_FB_ID,sizeof(struct hifb_memblock_pdata), GFP_KERNEL);

    if(NULL == pfbdata){
        HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
        return ERR_PTR(-ENOMEM);
    }

    pfbdata->phys_base = phys_base;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
    pfbbuf = dma_buf_export(pfbdata, &hifb_memblock_ops, size, flags);
#else
    pfbbuf = dma_buf_export(pfbdata, &hifb_memblock_ops, size, flags, NULL);
#endif

    if( IS_ERR(pfbbuf) ){
        HI_GFX_KFREE(HIGFX_FB_ID,pfbdata);
    }

    return pfbbuf;
}
#endif
