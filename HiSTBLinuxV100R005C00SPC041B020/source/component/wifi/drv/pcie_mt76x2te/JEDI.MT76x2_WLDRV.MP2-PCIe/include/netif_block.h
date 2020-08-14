/*
  * Copyright (c) 2016 MediaTek Inc.  All rights reserved.
  *
  * This software is available to you under a choice of one of two
  * licenses.  You may choose to be licensed under the terms of the GNU
  * General Public License (GPL) Version 2, available from the file
  * COPYING in the main directory of this source tree, or the
  * BSD license below:
  *
  *     Redistribution and use in source and binary forms, with or
  *     without modification, are permitted provided that the following
  *     conditions are met:
  *
  *      - Redistributions of source code must retain the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer.
  *
  *      - Redistributions in binary form must reproduce the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer in the documentation and/or other materials
  *        provided with the distribution.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */


#ifndef __NET_IF_BLOCK_H__
#define __NET_IF_BLOCK_H__

#include "link_list.h"
#include "rtmp.h"

#define FREE_NETIF_POOL_SIZE 32

typedef struct _NETIF_ENTRY {
	struct _NETIF_ENTRY *pNext;
	PNET_DEV pNetDev;
} NETIF_ENTRY, *PNETIF_ENTRY;

void initblockQueueTab(IN PRTMP_ADAPTER pAd);

BOOLEAN blockNetIf(IN PBLOCK_QUEUE_ENTRY pBlockQueueEntry, IN PNET_DEV pNetDev);

VOID releaseNetIf(IN PBLOCK_QUEUE_ENTRY pBlockQueueEntry);

VOID StopNetIfQueue(IN PRTMP_ADAPTER pAd, IN UCHAR QueIdx, IN PNDIS_PACKET pPacket);
#endif /* __NET_IF_BLOCK_H__ */
