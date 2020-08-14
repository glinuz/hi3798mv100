/******************************************************************************
 *    COPYRIGHT (C) 2013 Lyj. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Lyj 2013-12-03
 *
******************************************************************************/
#ifndef _MMC_IF_H
#define _MMC_IF_H
/******************************************************************************/

#include <stddef.h>
#include <flash.h>

/*****************************************************************************/

struct flash_info_t *get_mmcdev_info(void);

int mmcdev_check_align(uint64 from, uint64 *length);
int64 mmcdev_write(uint64 from, const char *buf, uint64 size);
int64 mmcdev_read(uint64 from, char *buf, uint64 size);
int mmcdev_erase(uint64 from, uint64 size);

/******************************************************************************/
#endif /* _MMC_IF_H */
