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


#ifdef INF_PPA_SUPPORT

#include "rt_config.h"
#include <linux/skbuff.h>
#include <linux/netdevice.h>

extern INT rt28xx_send_packets(struct sk_buff *skb_p, struct net_device *net_dev);

int ifx_ra_start_xmit(struct net_device *rx_dev, struct net_device *tx_dev, struct sk_buff *skb, int len)
{
	if(tx_dev != NULL)
	{
		SET_OS_PKT_NETDEV(skb, tx_dev);
		rt28xx_send_packets(skb, tx_dev);
	}
	else if(rx_dev != NULL)
	{
		skb->protocol = eth_type_trans(skb, skb->dev);
		netif_rx(skb);
	}
	else
	{
		dev_kfree_skb_any(skb);
	}
	return 0;
}
#endif /* INF_PPA_SUPPORT */
