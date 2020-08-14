/******************************************************************************
 *  Copyright (C) 2014 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2014.1.26
 *
******************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/genalloc.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/hikapi.h>
#include <linux/syscore_ops.h>

struct sram_ctrl_t {
	struct gen_pool *pool;
	char *backup_addr; /* backup address for suspend/resume */
	char *viraddr; /* sram virtual address */
	char *phyaddr; /* sram phy address */
	unsigned int size;
};

static struct sram_ctrl_t *sram_ctrl = NULL;
/******************************************************************************/

void *sram_alloc(size_t len, dma_addr_t *dma)
{
	unsigned long vaddr;

	if (dma)
		*dma = 0;

	if (!sram_ctrl || (dma && !sram_ctrl->phyaddr))
		return NULL;

	vaddr = gen_pool_alloc(sram_ctrl->pool, len);
	if (!vaddr)
		return NULL;

	if (dma)
		*dma = gen_pool_virt_to_phys(sram_ctrl->pool, vaddr);

	return (void *)vaddr;
}
EXPORT_SYMBOL(sram_alloc);

void sram_free(void *addr, size_t len)
{
	if (sram_ctrl)
		gen_pool_free(sram_ctrl->pool, (unsigned long)addr, len);
}
EXPORT_SYMBOL(sram_free);

int sram_suspend(void)
{
	if (sram_ctrl)
		memcpy(sram_ctrl->backup_addr, sram_ctrl->viraddr,
		       sram_ctrl->size);

	return 0;
}

void sram_resume(void)
{
	if (sram_ctrl)
		memcpy(sram_ctrl->viraddr, sram_ctrl->backup_addr,
		       sram_ctrl->size);
}

static struct syscore_ops sram_pm_syscore_ops = {
	.suspend	= sram_suspend,
	.resume		= sram_resume,
};

static int __init sram_init(void)
{
	int status = -ENOMEM;;

	sram_ctrl = kzalloc(sizeof(struct sram_ctrl_t) + CONFIG_SRAM_PHYS_SIZE,
			    GFP_KERNEL);
	if (!sram_ctrl)
		goto out;

	sram_ctrl->size = CONFIG_SRAM_PHYS_SIZE;
	sram_ctrl->phyaddr = (char *)CONFIG_SRAM_PHYS_START;
	sram_ctrl->backup_addr = (char *)&sram_ctrl[1];

	sram_ctrl->pool = gen_pool_create(ilog2(SRAM_GRANULARITY), -1);
	if (!sram_ctrl->pool)
		goto out1;

	sram_ctrl->viraddr = ioremap((unsigned long)sram_ctrl->phyaddr,
				     sram_ctrl->size);
	if (!sram_ctrl->viraddr)
		goto out2;

	status = gen_pool_add_virt(sram_ctrl->pool,
				   (unsigned long)sram_ctrl->viraddr,
				   (unsigned long)sram_ctrl->phyaddr,
				   sram_ctrl->size, -1);
	if (status < 0) {
		iounmap(sram_ctrl->viraddr);
		goto out2;
	}

#if defined(CONFIG_PM) && !defined(CONFIG_ARCH_S40) && !defined(CONFIG_ARCH_HI3798MX)
	register_syscore_ops(&sram_pm_syscore_ops);
#endif

out:
	WARN_ON(status < 0);
	return status;
out2:
	gen_pool_destroy(sram_ctrl->pool);
out1:
	kfree(sram_ctrl);
	sram_ctrl = NULL;
	goto out;
}
core_initcall(sram_init);
