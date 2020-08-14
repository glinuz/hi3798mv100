/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by ZhangYixing
 *
******************************************************************************/
#ifndef HIETH_DBG_H
#define HIETH_DBG_H

int hieth_dbg_init(void __iomem *base);
int hieth_dbg_deinit(void);
int multicast_dump_netdev_flags(u32 flags);
void multicast_dump_macaddr(u32 nr, char *macaddr);

#endif
