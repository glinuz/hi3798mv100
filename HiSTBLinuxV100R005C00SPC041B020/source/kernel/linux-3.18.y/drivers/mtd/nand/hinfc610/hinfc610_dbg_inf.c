/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-04
 *
******************************************************************************/

#include <linux/debugfs.h>
#include <asm/uaccess.h>

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg.h"

extern int hinfc610_dbgfs_debug_init(struct hinfc_host *host);

#ifdef CONFIG_HINFC610_DBG_NAND_DEBUG
extern struct hinfc610_dbg_inf_t *hinfc610_dbg_inf[];
#endif

void hinfc610_dbg_write(struct hinfc_host *host)
{
#ifdef CONFIG_HINFC610_DBG_NAND_DEBUG
	struct hinfc610_dbg_inf_t **inf;
	for (inf = hinfc610_dbg_inf; *inf; inf++)
		if ((*inf)->enable && (*inf)->write)
			(*inf)->write(host);
#endif
}

void hinfc610_dbg_erase(struct hinfc_host *host)
{
#ifdef CONFIG_HINFC610_DBG_NAND_DEBUG
	struct hinfc610_dbg_inf_t **inf;
	for (inf = hinfc610_dbg_inf; *inf; inf++)
		if ((*inf)->enable && (*inf)->erase)
			(*inf)->erase(host);
#endif
}

void hinfc610_dbg_read(struct hinfc_host *host)
{
#ifdef CONFIG_HINFC610_DBG_NAND_DEBUG
	struct hinfc610_dbg_inf_t **inf;
	for (inf = hinfc610_dbg_inf; *inf; inf++)
		if ((*inf)->enable && (*inf)->read)
			(*inf)->read(host);
#endif
}

void hinfc610_dbg_read_retry(struct hinfc_host *host, int index)
{
#ifdef CONFIG_HINFC610_DBG_NAND_DEBUG
	struct hinfc610_dbg_inf_t **inf;
	for (inf = hinfc610_dbg_inf; *inf; inf++)
		if ((*inf)->enable && (*inf)->read_retry)
			(*inf)->read_retry(host, index);
#endif
}

int hinfc610_dbg_init(struct hinfc_host *host)
{
#ifdef CONFIG_HINFC610_DBG_NAND_DEBUG
	return hinfc610_dbgfs_debug_init(host);
#endif
}
