/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
 
#define _HCI_OPS_OS_C_

//#include <drv_types.h>
#include <rtl8192e_hal.h>

/* Debug Buffer Descriptor Ring */
//#define BUF_DESC_DEBUG
#ifdef BUF_DESC_DEBUG
#define buf_desc_debug(...) do {\
			DBG_871X("BUF_DESC:" __VA_ARGS__);\
	}while(0)			
#else
#define buf_desc_debug(...)  do {} while(0)
#endif


static int rtl8192ee_init_rx_ring(_adapter * padapter)
{
	struct recv_priv	*precvpriv = &padapter->recvpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pci_dev	*pdev = pdvobjpriv->ppcidev;
	struct net_device	*dev = padapter->pnetdev;
	dma_addr_t *mapping = NULL;
	struct sk_buff *skb = NULL;
	u8	*rx_desc = NULL;
    	int	i, rx_queue_idx;

_func_enter_;

	//rx_queue_idx 0:RX_MPDU_QUEUE
	//rx_queue_idx 1:RX_CMD_QUEUE
	for(rx_queue_idx = 0; rx_queue_idx < 1/*RX_MAX_QUEUE*/; rx_queue_idx ++){
    		precvpriv->rx_ring[rx_queue_idx].desc = 
			pci_alloc_consistent(pdev,
							sizeof(*precvpriv->rx_ring[rx_queue_idx].desc) * precvpriv->rxringcount,
							&precvpriv->rx_ring[rx_queue_idx].dma);

    		if (!precvpriv->rx_ring[rx_queue_idx].desc 
			|| (unsigned long)precvpriv->rx_ring[rx_queue_idx].desc & 0xFF) {
        		DBG_8192C("Cannot allocate RX ring\n");
        		return _FAIL;
    		}

    		_rtw_memset(precvpriv->rx_ring[rx_queue_idx].desc, 0, sizeof(*precvpriv->rx_ring[rx_queue_idx].desc) * precvpriv->rxringcount);
    		precvpriv->rx_ring[rx_queue_idx].idx = 0;

    		for (i = 0; i < precvpriv->rxringcount; i++) {
			skb = rtw_skb_alloc(precvpriv->rxbuffersize);
        		if (!skb){
				DBG_8192C("Cannot allocate skb for RX ring\n");
            			return _FAIL;
        		}

        		rx_desc = (u8 *)(&precvpriv->rx_ring[rx_queue_idx].desc[i]);

			precvpriv->rx_ring[rx_queue_idx].rx_buf[i] = skb;

			mapping = (dma_addr_t *)skb->cb;

			//just set skb->cb to mapping addr for pci_unmap_single use
			*mapping = pci_map_single(pdev, skb_tail_pointer(skb),
						precvpriv->rxbuffersize,
						PCI_DMA_FROMDEVICE);

			//Reset FS, LS, Total len
			SET_RX_BUFFER_DESC_LS_92E(rx_desc,0);
			SET_RX_BUFFER_DESC_FS_92E(rx_desc,0);
			SET_RX_BUFFER_DESC_TOTAL_LENGTH_92E(rx_desc,0);
			SET_RX_BUFFER_DESC_DATA_LENGTH_92E(rx_desc, precvpriv->rxbuffersize);
			SET_RX_BUFFER_PHYSICAL_LOW_92E(rx_desc, *mapping);

			buf_desc_debug("RX:rx buffer desc addr[%d] = %x, skb(rx_buf) = %x, buffer addr (virtual = %x, physical = %x)\n", \
						i, (u32)&precvpriv->rx_ring[rx_queue_idx].desc[i], (u32)precvpriv->rx_ring[rx_queue_idx].rx_buf[i], (u32)(skb_tail_pointer(skb)), (u32)(*mapping));
    		}
	}

_func_exit_;

    	return _SUCCESS;
}

static void rtl8192ee_free_rx_ring(_adapter * padapter)
{
	struct recv_priv	*precvpriv = &padapter->recvpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pci_dev	*pdev = pdvobjpriv->ppcidev;
	int i, rx_queue_idx;

_func_enter_;

	//rx_queue_idx 0:RX_MPDU_QUEUE
	//rx_queue_idx 1:RX_CMD_QUEUE
	for (rx_queue_idx = 0; rx_queue_idx < 1/*RX_MAX_QUEUE*/; rx_queue_idx++) {
		for (i = 0; i < precvpriv->rxringcount; i++) {
			struct sk_buff *skb = precvpriv->rx_ring[rx_queue_idx].rx_buf[i];
			if (!skb)
				continue;

			pci_unmap_single(pdev,
					 *((dma_addr_t *) skb->cb),
					 precvpriv->rxbuffersize,
					 PCI_DMA_FROMDEVICE);
			rtw_skb_free(skb);
		}

		pci_free_consistent(pdev,
				    sizeof(*precvpriv->rx_ring[rx_queue_idx].desc) *
				    precvpriv->rxringcount,
				    precvpriv->rx_ring[rx_queue_idx].desc,
				    precvpriv->rx_ring[rx_queue_idx].dma);
		precvpriv->rx_ring[rx_queue_idx].desc = NULL;
	}

_func_exit_;
}


static int rtl8192ee_init_tx_ring(_adapter * padapter, unsigned int prio, unsigned int entries)
{
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pci_dev	*pdev = pdvobjpriv->ppcidev;
	struct tx_desc	*ring;
	u8				*tx_desc;
	dma_addr_t		dma;
	int	i;

_func_enter_;

	DBG_8192C("%s entries num:%d\n", __func__, entries);
	ring = pci_alloc_consistent(pdev, sizeof(*ring) * entries, &dma);
	if (!ring || (unsigned long)ring & 0xFF) {
		DBG_8192C("Cannot allocate TX ring (prio = %d)\n", prio);
		return _FAIL;
	}

	_rtw_memset(ring, 0, sizeof(*ring) * entries);
	pxmitpriv->tx_ring[prio].desc = ring;
	pxmitpriv->tx_ring[prio].dma = dma;
	pxmitpriv->tx_ring[prio].idx = 0;
	pxmitpriv->tx_ring[prio].entries = entries;
	_rtw_init_queue(&pxmitpriv->tx_ring[prio].queue);
	pxmitpriv->tx_ring[prio].qlen = 0;

	//DBG_8192C("%s queue:%d, ring_addr:%p\n", __func__, prio, ring);

_func_exit_;

	return _SUCCESS;
}
static void rtl8192ee_free_tx_ring(_adapter * padapter, unsigned int prio)
{
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pci_dev	*pdev = pdvobjpriv->ppcidev;
	struct rtw_tx_ring *ring = &pxmitpriv->tx_ring[prio];
	u8				*tx_desc;
	struct xmit_buf	*pxmitbuf;

_func_enter_;

	while (ring->qlen) {
		tx_desc = (u8 *)(&ring->desc[ring->idx]);

		SET_TX_DESC_OWN_92E(tx_desc, 0);

		if(prio != BCN_QUEUE_INX) {
			ring->idx = (ring->idx + 1) % ring->entries;
		}

		pxmitbuf = rtl8192ee_dequeue_xmitbuf(ring);
		if (pxmitbuf) {
			pci_unmap_single(pdev, GET_TX_DESC_TX_BUFFER_ADDRESS_92E(tx_desc), pxmitbuf->len, PCI_DMA_TODEVICE);
			rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
		} else {
			DBG_871X("%s(): qlen(%d) is not zero, but have xmitbuf in pending queue\n",__func__,ring->qlen);
			break;
		}
	}

	pci_free_consistent(pdev, sizeof(*ring->desc) * ring->entries, ring->desc, ring->dma);
	ring->desc = NULL;

_func_exit_;
}

static void init_desc_ring_var(_adapter * padapter)
{
	struct recv_priv	*precvpriv = &padapter->recvpriv;
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	u8 i = 0;

	for (i = 0; i < HW_QUEUE_ENTRY; i++) {
		pxmitpriv->txringcount[i] = TXDESC_NUM;
	}

	//we just alloc 2 desc for beacon queue,
	//because we just need first desc in hw beacon.
	pxmitpriv->txringcount[BCN_QUEUE_INX] = 2;

	// BE queue need more descriptor for performance consideration
	// or, No more tx desc will happen, and may cause mac80211 mem leakage.
	//if(!padapter->registrypriv.wifi_spec)
	//	pxmitpriv->txringcount[BE_QUEUE_INX] = TXDESC_NUM_BE_QUEUE;

	pxmitpriv->txringcount[TXCMD_QUEUE_INX] = 2;

	precvpriv->rxbuffersize = MAX_RECVBUF_SZ;	//2048;//1024;
	precvpriv->rxringcount = PCI_MAX_RX_COUNT;	//128
}


u32 rtl8192ee_init_desc_ring(_adapter * padapter)
{
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	int	i, ret = _SUCCESS;

_func_enter_;

	init_desc_ring_var(padapter);

	ret = rtl8192ee_init_rx_ring(padapter);
	if (ret == _FAIL) {
		return ret;
	}

	// general process for other queue */
	for (i = 0; i < PCI_MAX_TX_QUEUE_COUNT; i++) {
		ret = rtl8192ee_init_tx_ring(padapter, i, pxmitpriv->txringcount[i]);
		if (ret == _FAIL)
			goto err_free_rings;
	}

	return ret;

err_free_rings:

	rtl8192ee_free_rx_ring(padapter);

	for (i = 0; i <PCI_MAX_TX_QUEUE_COUNT; i++)
		if (pxmitpriv->tx_ring[i].desc)
			rtl8192ee_free_tx_ring(padapter, i);

_func_exit_;

	return ret;
}

u32 rtl8192ee_free_desc_ring(_adapter * padapter)
{
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	u32 i;

_func_enter_;

	// free rx rings 
	rtl8192ee_free_rx_ring(padapter);

	// free tx rings
	for (i = 0; i < HW_QUEUE_ENTRY; i++) {
		rtl8192ee_free_tx_ring(padapter, i);
	}

_func_exit_;

	return _SUCCESS;
}

void rtl8192ee_reset_desc_ring(_adapter * padapter)
{
	_irqL	irqL;
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct recv_priv	*precvpriv = &padapter->recvpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct xmit_buf	*pxmitbuf = NULL;
	u8	*tx_desc, *rx_desc;
	int	i,rx_queue_idx;

	for(rx_queue_idx = 0; rx_queue_idx < 1; rx_queue_idx ++){
    		if(precvpriv->rx_ring[rx_queue_idx].desc) {
        		rx_desc = NULL;
        		for (i = 0; i < precvpriv->rxringcount; i++) {
				rx_desc = (u8 *)(&precvpriv->rx_ring[rx_queue_idx].desc[i]);
				SET_RX_STATUS_DESC_OWN_92E(rx_desc, 1);
        		}
        		precvpriv->rx_ring[rx_queue_idx].idx = 0;
    		}
	}

	_enter_critical(&pdvobjpriv->irq_th_lock, &irqL);
	for (i = 0; i < PCI_MAX_TX_QUEUE_COUNT; i++) {
		if (pxmitpriv->tx_ring[i].desc) {
			struct rtw_tx_ring *ring = &pxmitpriv->tx_ring[i];

			while (ring->qlen) {
				tx_desc = (u8 *)(&ring->desc[ring->idx]);

				SET_TX_DESC_OWN_92E(tx_desc, 0);

				if(i != BCN_QUEUE_INX) {
					ring->idx = (ring->idx + 1) % ring->entries;
				}

				pxmitbuf = rtl8192ee_dequeue_xmitbuf(ring);
				if (pxmitbuf) {
					pci_unmap_single(pdvobjpriv->ppcidev, GET_TX_DESC_TX_BUFFER_ADDRESS_92E(tx_desc), pxmitbuf->len, PCI_DMA_TODEVICE);
					rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
				} else {
					DBG_871X("%s(): qlen(%d) is not zero, but have xmitbuf in pending queue\n",__func__,ring->qlen);
					break;
				}
			}
			ring->idx = 0;
		}
	}
	_exit_critical(&pdvobjpriv->irq_th_lock, &irqL);
}

static void rtl8192ee_xmit_beacon(PADAPTER Adapter)
{
#if defined (CONFIG_AP_MODE) && defined (CONFIG_NATIVEAP_MLME)
	struct mlme_priv *pmlmepriv = &Adapter->mlmepriv;

	if(check_fwstate(pmlmepriv, WIFI_AP_STATE))
	{
		//send_beacon(Adapter);
		if(pmlmepriv->update_bcn == _TRUE)
		{
			tx_beacon_hdl(Adapter, NULL);
		}
	}
#endif
}

void rtl8192ee_prepare_bcn_tasklet(void *priv)
{
	_adapter	*padapter = (_adapter*)priv;

	rtl8192ee_xmit_beacon(padapter);
}

static u8 check_tx_desc_resource(_adapter *padapter, int prio)
{
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct rtw_tx_ring	*ring;

	ring = &pxmitpriv->tx_ring[prio];

	// for now we reserve two free descriptor as a safety boundary 
	// between the tail and the head 
	//
	if ((ring->entries - ring->qlen) >= 2) {
		return _TRUE;
	} else {
		//DBG_8192C("do not have enough desc for Tx \n");
		return _FALSE;
	}
}

/*
  *     Draw a line to show queue status. For debug
  *		i: queue index / W: HW index / h: host index / .: enpty entry / *: ready to DMA
  *     Exmaple:  R- 3- 4- 8 ..iW***h..... (i=3,W=4,h=8, *** means 3 tx_desc is reaady to dma)
  *        
  */
#ifdef BUF_DESC_DEBUG
void _draw_queue(PADAPTER Adapter, int prio) 
{
	int i;
	u8 line[TXDESC_NUM+1]; 
	u16 hw, host;
	u32	index,tmp_4bytes=0;

	struct xmit_priv	*pxmitpriv = &Adapter->xmitpriv;
	struct rtw_tx_ring	*ring = &pxmitpriv->tx_ring[prio];

	tmp_4bytes = rtw_read32(Adapter, get_txdesc_buf_addr(prio));
	hw   = (u16)((tmp_4bytes>>16) & 0x7ff);
	host = (u16) (tmp_4bytes & 0x7ff);

	index = ring->idx;
	_rtw_memset(line,'.',TXDESC_NUM);

	//ready to return to driver
	if (index <= hw) {
		for (i=index;i<hw;i++)
			line[i] = ':';
	} else { //wrap
		for (i=index;i<TXDESC_NUM;i++)
			line[i] = ':';
		for (i=0;i<hw;i++)
			line[i] = ':';
	}

	//ready to dma
	if (hw <= host) {
		for (i=hw;i<host;i++)
			line[i] = '*';
	} else { //wrap
		for (i=hw;i<TXDESC_NUM;i++)
			line[i] = '*';
		for (i=0;i<host;i++)
			line[i] = '*';
	}

	line[index] = 'i'; // software queue index
	line[host] = 'h';  // host index
	line[hw] = 'W';	   // hardware index
	line[TXDESC_NUM] = 0x0;

	//Q2:10-20-30: 
	buf_desc_debug("Q%d:%02d-%02d-%02d %s\n", prio, index, hw, host, line);
}
#endif

/*
  * 		Read pointer is h/w descriptor index
  *		Write pointer is host desciptor index: For tx side, if own bit is set in packet index n, 
  *									      host pointer (write pointer) point to index n + 1.
  */
u32 rtl8192ee_check_txdesc_closed(_adapter *padapter, u32 queue_idx, u32 index)
{

	struct mlme_priv		*pmlmepriv = &(padapter->mlmepriv);
	static u8 stop_report_count = 0;
	u16 check_state = _TRUE;	
	u16	desc_idx_hw =0, desc_idx_host =0;
	u32	tmp_4bytes=0;
	bool is_desc_closed = _FALSE;


	// For normal driver, check Read/Write Point each interrupt wastes CPU utilization.
#if (MP_DRIVER == 0)	// normal driver
	if(stop_report_count > 15 || !pmlmepriv->LinkDetectInfo.bBusyTraffic)
		check_state = _TRUE;
	else
		check_state = _FALSE;
#endif

	buf_desc_debug("TX:enter %s(%d): queue_idx = %d, desc_index = %04x, stop_report_count = %d, check_state = %d\n", __FUNCTION__, __LINE__, queue_idx, index, stop_report_count, check_state);

	if(!check_state)
	{
		is_desc_closed = _FALSE;
		return is_desc_closed;
	}

	if((queue_idx == BCN_QUEUE_INX))
	{
		// For 92C download reserved page by BEACON_QUEUE.
		is_desc_closed = _TRUE;
		return is_desc_closed;
	}
	
	if (padapter->bDriverStopped || padapter->bSurpriseRemoved)
	{
		// For 92C download reserved page by BEACON_QUEUE.
		is_desc_closed = _TRUE;
		return is_desc_closed;
	}
	
	tmp_4bytes = rtw_read32(padapter, get_txdesc_buf_addr(queue_idx));
	desc_idx_hw = (u16)((tmp_4bytes>>16) & 0x7ff);
	desc_idx_host = (u16)(tmp_4bytes & 0x7ff);

#ifdef BUF_DESC_DEBUG
	_draw_queue(padapter, queue_idx);
#endif

	if(desc_idx_host > desc_idx_hw)
	{
		if( index < desc_idx_host && index >= desc_idx_hw)
			is_desc_closed = _FALSE;
		else
			is_desc_closed = _TRUE;
	}
	else if(desc_idx_host < desc_idx_hw)
	{
		if(index >= desc_idx_host && index < desc_idx_hw)
			is_desc_closed= _TRUE;
		else
			is_desc_closed= _FALSE;				
	}
	else //if(desc_idx_host == desc_idx_hw)
	{
		if(index !=desc_idx_hw)
			is_desc_closed = _TRUE;
		else
			is_desc_closed = _FALSE;
	}

	if(queue_idx < BCN_QUEUE_INX)
	{
		if(!is_desc_closed)
			stop_report_count++;
		else
			stop_report_count = 0;	
	}

	buf_desc_debug("TX:exit %s(%d): r/w reg = 0x%08x, desc_idx_host = %04x, desc_idx_hw = %04x, is_desc_closed = %d, stop_report_count = %d\n", __FUNCTION__, __LINE__, tmp_4bytes, desc_idx_host, desc_idx_hw, is_desc_closed, stop_report_count);

	return is_desc_closed;
	
}

#ifdef CONFIG_BCN_ICF
static void rtl8192ee_tx_isr(PADAPTER Adapter, int prio)
{
	struct xmit_priv	*pxmitpriv = &Adapter->xmitpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(Adapter);
	struct rtw_tx_ring	*ring = &pxmitpriv->tx_ring[prio];
#ifdef CONFIG_CONCURRENT_MODE
	PADAPTER pbuddy_adapter = Adapter->pbuddy_adapter;
#endif
	struct xmit_buf	*pxmitbuf;
	u8	*tx_desc;
	u16	tmp_4bytes;
	u16	desc_idx_hw =0, desc_idx_host =0;
	

	while(ring->qlen)
	{
		tx_desc = (u8 *)&ring->desc[ring->idx];

		//  beacon use cmd buf Never run into here
		if(!rtl8192ee_check_txdesc_closed(Adapter, prio, ring->idx))
			return;

		buf_desc_debug("TX: %s, queue_idx = %d, tx_desc = %04x, close desc [%04x] and update ring->idx to [%04x]\n", __FUNCTION__, prio, (u32)tx_desc, ring->idx, (ring->idx + 1) % ring->entries); 
		ring->idx = (ring->idx + 1) % ring->entries;						

		pxmitbuf = rtl8192ee_dequeue_xmitbuf(ring);

		if (pxmitbuf) {
			pci_unmap_single(pdvobjpriv->ppcidev, GET_TX_DESC_TX_BUFFER_ADDRESS_92E(tx_desc), pxmitbuf->len, PCI_DMA_TODEVICE);
			rtw_sctx_done(&pxmitbuf->sctx);
			rtw_free_xmitbuf(&(pxmitbuf->padapter->xmitpriv), pxmitbuf);
		} else {
			DBG_871X("%s(): qlen(%d) is not zero, but have xmitbuf in pending queue\n",__func__,ring->qlen);
		}
	}

	if(check_tx_desc_resource(Adapter, prio)) {
		if ((check_fwstate(&Adapter->mlmepriv, _FW_UNDER_SURVEY) != _TRUE)
			&& rtw_txframes_pending(Adapter))
		{
			// try to deal with the pending packets
			tasklet_hi_schedule(&(Adapter->xmitpriv.xmit_tasklet));
		}
#ifdef CONFIG_CONCURRENT_MODE
		if ((check_fwstate(&pbuddy_adapter->mlmepriv, _FW_UNDER_SURVEY) != _TRUE)
			&& rtw_txframes_pending(pbuddy_adapter))
		{
			// try to deal with the pending packets
			tasklet_hi_schedule(&(pbuddy_adapter->xmitpriv.xmit_tasklet));
		}
#endif
	}
}

#else //!CONFIG_BCN_ICF 
static void rtl8192ee_tx_isr(PADAPTER Adapter, int prio)
{
	struct xmit_priv	*pxmitpriv = &Adapter->xmitpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(Adapter);
	struct rtw_tx_ring	*ring = &pxmitpriv->tx_ring[prio];
#ifdef CONFIG_CONCURRENT_MODE
	PADAPTER pbuddy_adapter = Adapter->pbuddy_adapter;
#endif
	struct xmit_buf	*pxmitbuf;
	u8	*tx_desc;
	u16	tmp_4bytes;
	u16	desc_idx_hw =0, desc_idx_host =0;
	

	while(ring->qlen)
	{
		tx_desc = (u8 *)&ring->desc[ring->idx];

		//  beacon packet will only use the first descriptor defautly,
		//  check register to see whether h/w has consumed buffer descriptor
		// 
		if (prio != BCN_QUEUE_INX) {
			if(!rtl8192ee_check_txdesc_closed(Adapter, prio, ring->idx))
				return;

			buf_desc_debug("TX: %s, queue_idx = %d, tx_desc = %04x, close desc [%04x] and update ring->idx to [%04x]\n", __FUNCTION__, prio, (u32)tx_desc, ring->idx, (ring->idx + 1) % ring->entries); 
			ring->idx = (ring->idx + 1) % ring->entries;						
		} else if ( prio == BCN_QUEUE_INX) {
			SET_TX_DESC_OWN_92E(tx_desc, 0);
		}

		pxmitbuf = rtl8192ee_dequeue_xmitbuf(ring);
		if (pxmitbuf) {
			pci_unmap_single(pdvobjpriv->ppcidev, GET_TX_DESC_TX_BUFFER_ADDRESS_92E(tx_desc), pxmitbuf->len, PCI_DMA_TODEVICE);
			rtw_sctx_done(&pxmitbuf->sctx);
			rtw_free_xmitbuf(&(pxmitbuf->padapter->xmitpriv), pxmitbuf);
		} else {
			DBG_871X("%s(): qlen(%d) is not zero, but have xmitbuf in pending queue\n",__func__,ring->qlen);
		}
	}

	if((prio != BCN_QUEUE_INX) && check_tx_desc_resource(Adapter, prio)) {
		if ((check_fwstate(&Adapter->mlmepriv, _FW_UNDER_SURVEY) != _TRUE)
			&& rtw_txframes_pending(Adapter))
		{
			// try to deal with the pending packets
			tasklet_hi_schedule(&(Adapter->xmitpriv.xmit_tasklet));
		}
#ifdef CONFIG_CONCURRENT_MODE
		if ((check_fwstate(&pbuddy_adapter->mlmepriv, _FW_UNDER_SURVEY) != _TRUE)
			&& rtw_txframes_pending(pbuddy_adapter))
		{
			// try to deal with the pending packets
			tasklet_hi_schedule(&(pbuddy_adapter->xmitpriv.xmit_tasklet));
		}
#endif
	}
}
#endif //CONFIG_BCN_ICF


s32	rtl8192ee_interrupt(PADAPTER Adapter)
{
	_irqL	irqL;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(Adapter);
	struct xmit_priv	*pxmitpriv = &Adapter->xmitpriv;
	int	ret = _SUCCESS;

	_enter_critical(&pdvobjpriv->irq_th_lock, &irqL);

	DBG_COUNTER(Adapter->int_logs.all);

	//read ISR: 4/8bytes
	if (InterruptRecognized8192EE(Adapter) == _FALSE) {
		DBG_COUNTER(Adapter->int_logs.err);
		ret = _FAIL;
		goto done;
	}

	//<1> beacon related
	if (pHalData->IntArray[0] & IMR_TXBCN0OK_8192E) {
#ifdef CONFIG_BCN_ICF
		//do nothing
		DBG_COUNTER(Adapter->int_logs.tbdok);
#else
		PADAPTER bcn_adapter = Adapter;

		DBG_COUNTER(Adapter->int_logs.tbdok);
#ifdef CONFIG_CONCURRENT_MODE
		if  ((Adapter->pbuddy_adapter)&&check_fwstate(&Adapter->pbuddy_adapter->mlmepriv, WIFI_AP_STATE))
			bcn_adapter = Adapter->pbuddy_adapter;
#endif
		if (bcn_adapter->xmitpriv.beaconDMAing) {
			bcn_adapter->xmitpriv.beaconDMAing = _FAIL;
			rtl8192ee_tx_isr(Adapter, BCN_QUEUE_INX);
		} 
#endif //CONFIG_BCN_ICF
	}

	if (pHalData->IntArray[0] & IMR_TXBCN0ERR_8192E) {
#ifdef CONFIG_BCN_ICF
		DBG_COUNTER(Adapter->int_logs.tbder);
		DBG_8192C("IMR_TXBCN0ERR interrupt!\n");		
#else //!CONFIG_BCN_ICF
		PADAPTER bcn_adapter = Adapter;

		DBG_COUNTER(Adapter->int_logs.tbder);
#ifdef CONFIG_CONCURRENT_MODE
		if  ((Adapter->pbuddy_adapter)&&check_fwstate(&Adapter->pbuddy_adapter->mlmepriv, WIFI_AP_STATE))
			bcn_adapter = Adapter->pbuddy_adapter;
#endif
		if (bcn_adapter->xmitpriv.beaconDMAing) {
			bcn_adapter->xmitpriv.beaconDMAing = _FAIL;
			rtl8192ee_tx_isr(Adapter, BCN_QUEUE_INX);
		}
#endif //CONFIG_BCN_ICF
	}

	//if (pHalData->IntArray[0] & IMR_TXBCN0ERR_8192E) {
	if (pHalData->IntArray[0] & IMR_BCNDERR0_8192E) {
#ifdef CONFIG_BCN_ICF
		DBG_COUNTER(Adapter->int_logs.bcnderr);
		DBG_8192C("IMR_TXBCN0ERR_8192E interrupt!\n");		
#else //!CONFIG_BCN_ICF
		//  Release resource and re-transmit beacon to HW
		PADAPTER bcn_adapter = Adapter;
		struct tasklet_struct  *bcn_tasklet;

		DBG_COUNTER(Adapter->int_logs.bcnderr);
#ifdef CONFIG_CONCURRENT_MODE
		if  ((Adapter->pbuddy_adapter)&&check_fwstate(&Adapter->pbuddy_adapter->mlmepriv, WIFI_AP_STATE))
			bcn_adapter = Adapter->pbuddy_adapter;
#endif 
		rtl8192ee_tx_isr(Adapter, BCN_QUEUE_INX);
		bcn_adapter->mlmepriv.update_bcn = _TRUE;
		bcn_tasklet = &bcn_adapter->recvpriv.irq_prepare_beacon_tasklet;
		tasklet_hi_schedule(bcn_tasklet);
#endif //CONFIG_BCN_ICF
	}

	if (pHalData->IntArray[0] & IMR_BCNDMAINT0_8192E) {
		struct tasklet_struct  *bcn_tasklet;

		DBG_COUNTER(Adapter->int_logs.bcndma);
#ifdef CONFIG_CONCURRENT_MODE
		if (Adapter->iface_type == IFACE_PORT1)
			bcn_tasklet = &Adapter->pbuddy_adapter->recvpriv.irq_prepare_beacon_tasklet;
		else
#endif
		bcn_tasklet = &Adapter->recvpriv.irq_prepare_beacon_tasklet;
		tasklet_hi_schedule(bcn_tasklet);
	}

	//<2> Rx related
	if ((pHalData->IntArray[0] & (IMR_ROK_8192E|IMR_RDU_8192E)) || (pHalData->IntArray[1] & IMR_RXFOVW_8192E)) {

		DBG_COUNTER(Adapter->int_logs.rx);

		if (pHalData->IntArray[0] & IMR_RDU_8192E)
			DBG_COUNTER(Adapter->int_logs.rx_rdu);

		if (pHalData->IntArray[1] & IMR_RXFOVW_8192E)
			DBG_COUNTER(Adapter->int_logs.rx_fovw);

		pHalData->IntrMask[0] &= (~(IMR_ROK_8192E|IMR_RDU_8192E));
		pHalData->IntrMask[1] &= (~IMR_RXFOVW_8192E);		
		rtw_write32(Adapter, REG_HIMR0_8192E, pHalData->IntrMask[0]);
		rtw_write32(Adapter, REG_HIMR1_8192E, pHalData->IntrMask[1]);
		tasklet_hi_schedule(&Adapter->recvpriv.recv_tasklet);
	}

	//<3> Tx related
	if (pHalData->IntArray[1] & IMR_TXFOVW_8192E) {
		DBG_COUNTER(Adapter->int_logs.txfovw);
		//DBG_8192C("IMR_TXFOVW!\n");
	}

	/*if (pHalData->IntArray[0] & IMR_TX_MASK) {
		pHalData->IntrMask[0] &= (~(IMR_TX_MASK));
		rtw_write32(Adapter, REG_HIMR0_8812, pHalData->IntrMask[0]);
		tasklet_hi_schedule(&pxmitpriv->xmit_tasklet);
	}*/

	if (pHalData->IntArray[0] & IMR_MGNTDOK_8192E) {
		DBG_COUNTER(Adapter->int_logs.mgntok);
		//DBG_8192C("Manage ok interrupt!\n");		
		rtl8192ee_tx_isr(Adapter, MGT_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_HIGHDOK_8192E) {
		DBG_COUNTER(Adapter->int_logs.highdok);
		//DBG_8192C("HIGH_QUEUE ok interrupt!\n");
		rtl8192ee_tx_isr(Adapter, HIGH_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_BKDOK_8192E) {
		DBG_COUNTER(Adapter->int_logs.bkdok);
		//DBG_8192C("BK Tx OK interrupt!\n");
		rtl8192ee_tx_isr(Adapter, BK_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_BEDOK_8192E) {
		DBG_COUNTER(Adapter->int_logs.bedok);
		//DBG_8192C("BE TX OK interrupt!\n");
		rtl8192ee_tx_isr(Adapter, BE_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_VIDOK_8192E) {
		DBG_COUNTER(Adapter->int_logs.vidok);
		//DBG_8192C("VI TX OK interrupt!\n");
		rtl8192ee_tx_isr(Adapter, VI_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_VODOK_8192E) {
		DBG_COUNTER(Adapter->int_logs.vodok);
		//DBG_8192C("Vo TX OK interrupt!\n");
		rtl8192ee_tx_isr(Adapter, VO_QUEUE_INX);
	}

done:

	_exit_critical(&pdvobjpriv->irq_th_lock, &irqL);

	return ret;
}

/*	Aries add, 20120305
	clone another recvframe and associate with secondary_adapter.
*/
#ifdef CONFIG_CONCURRENT_MODE
static int rtl812ee_if2_clone_recvframe(_adapter *sec_padapter, union recv_frame *precvframe, union recv_frame *precvframe_if2, struct  sk_buff *clone_pkt, u8 *pphy_info)
{
	struct rx_pkt_attrib	*pattrib = NULL;

	precvframe_if2->u.hdr.adapter = sec_padapter;
	_rtw_init_listhead(&precvframe_if2->u.hdr.list);
	precvframe_if2->u.hdr.precvbuf = NULL;	//can't access the precvbuf for new arch.
	precvframe_if2->u.hdr.len=0;

	_rtw_memcpy(&precvframe_if2->u.hdr.attrib, &precvframe->u.hdr.attrib, sizeof(struct rx_pkt_attrib));
	pattrib = &precvframe_if2->u.hdr.attrib;

	if(clone_pkt)
	{
		clone_pkt->dev = sec_padapter->pnetdev;
		precvframe_if2->u.hdr.pkt = clone_pkt;
		precvframe_if2->u.hdr.rx_head = clone_pkt->head;
		precvframe_if2->u.hdr.rx_data = precvframe_if2->u.hdr.rx_tail = clone_pkt->data;
		precvframe_if2->u.hdr.rx_end = skb_end_pointer(clone_pkt);
	}
	else
	{
		DBG_8192C("rtl8188ee_rx_mpdu:can not allocate memory for skb_copy\n");
		return _FAIL;
	}
	recvframe_put(precvframe_if2, clone_pkt->len);

	if( (pattrib->physt) && (pattrib->pkt_rpt_type == NORMAL_RX))
		rx_query_phy_status(precvframe_if2, pphy_info);
	
	return _SUCCESS;
}
#endif

/*
  * Wait until rx data is ready
  * 	return value: _SUCCESS if Rx packet is ready, _FAIL if not ready
  */

u32
rtl8192ee_wait_rxrdy(
	_adapter 			*padapter,
	u8*				rx_desc,		//rx_buf_desc
	u16				rx_queue_idx
)
{

	struct recv_priv	*precvpriv = &padapter->recvpriv;
	u8 first_seg = 0, last_seg = 0;
	u16 total_len = 0, read_cnt=0;
	static BOOLEAN	start_rx=_FALSE;
	u16 status = _SUCCESS;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);	

	if(rx_desc == NULL)
		return _FAIL;
	
	total_len= (u2Byte)GET_RX_BUFFER_DESC_TOTAL_LENGTH_92E(rx_desc);
	first_seg = (u1Byte)GET_RX_BUFFER_DESC_FS_92E(rx_desc);
	last_seg = (u1Byte)GET_RX_BUFFER_DESC_LS_92E(rx_desc);

	buf_desc_debug("RX:%s enter: rx_buf_desc addr = %p, total_len=%d, first_seg=%d, last_seg=%d, read_cnt %d, index %d, address %p \n",\
					__FUNCTION__, (u8 *)&precvpriv->rx_ring[rx_queue_idx].desc[precvpriv->rx_ring[rx_queue_idx].idx], total_len, first_seg, last_seg, read_cnt, precvpriv->rx_ring[rx_queue_idx].idx, rx_desc);

#if defined(USING_RX_TAG)
	//Rx Tag not ported
	if(!start_rx)
	{
		start_rx = _TRUE;
		pHalData->RxTag = 1;
	}else
	{			
		while( total_len != (pHalData->RxTag+1)  )
		{
			
			read_cnt++;	

			total_len= (u2Byte)GET_RX_BUFFER_DESC_TOTAL_LENGTH_92E(rx_desc);

			first_seg = (u1Byte)GET_RX_BUFFER_DESC_FS_92E(rx_desc);

			last_seg = (u1Byte)GET_RX_BUFFER_DESC_LS_92E(rx_desc);


			if(read_cnt>10000)	
			{
				pHalData->RxTag =total_len;
				break;
			}

			if(total_len == 0 && pHalData->RxTag== 0x1fff)
				break;
		}		
		pHalData->RxTag = total_len;
	}
#else
	while(total_len ==0 && first_seg==0 && last_seg ==0)
	{

		read_cnt++;	

		total_len= (u2Byte)GET_RX_BUFFER_DESC_TOTAL_LENGTH_92E(rx_desc);

		first_seg = (u1Byte)GET_RX_BUFFER_DESC_FS_92E(rx_desc);

		last_seg = (u1Byte)GET_RX_BUFFER_DESC_LS_92E(rx_desc);

		if(read_cnt>20)
		{			
			status = _FAIL;			
			break;
		}
	}
#endif	
		
	buf_desc_debug("RX:%s exit total_len=%d, rx_tag = %d, first_seg=%d, last_seg=%d, read_cnt %d\n",\
						__FUNCTION__, total_len, pHalData->RxTag, first_seg, last_seg, read_cnt);

	return status;
	
}


/*
  * Check the number of rxdec to be handled between
  * 	"index of RX queue descriptor maintained by host (write pointer)" and 
  *   "index of RX queue descriptor maintained by hardware (read pointer)"
  */
u16 rtl8192ee_check_rxdesc_remain(_adapter *padapter, int	rx_queue_idx)
{
	struct recv_priv	*precvpriv = &padapter->recvpriv;
	u16	desc_idx_hw =0, desc_idx_host =0,num_rxdesc_to_handle=0;
	u32	tmp_4bytes=0;
	static u16 last_idx_hw = 0;
	static BOOLEAN	start_rx=FALSE;

	tmp_4bytes = rtw_read32(padapter, REG_RXQ_TXBD_IDX_8192E);
	desc_idx_hw = (u16)((tmp_4bytes>>16) & 0x7ff);
	desc_idx_host = (u16)(tmp_4bytes & 0x7ff);

	// make sure driver does not handle packet if hardware pointer keeps in zero 
	// 	in initial state
	buf_desc_debug("RX:%s(%d) reg_value %x\n", __FUNCTION__, __LINE__, tmp_4bytes);

	if(desc_idx_hw>0)
		start_rx=TRUE;
	if(!start_rx)
		return 0;

	if((last_idx_hw >(RX_DESC_NUM_92E/2)) && (desc_idx_hw <=(RX_DESC_NUM_92E/2)))
	{
		buf_desc_debug("RX:Rx desc is turn around\n");
		num_rxdesc_to_handle= RX_DESC_NUM_92E-desc_idx_host;
	}
	else
	{
		num_rxdesc_to_handle = (desc_idx_hw>=desc_idx_host) ? (desc_idx_hw-desc_idx_host):(RX_DESC_NUM_92E-desc_idx_host+desc_idx_hw);
	}
	
	if(num_rxdesc_to_handle ==0)
		return 0;

	precvpriv->rx_ring[rx_queue_idx].idx = desc_idx_host;		

	buf_desc_debug("RX:%s  reg_value %x,  last_desc_idx_hw = %x, current hw idx %x, current host idx %x, remain desc to handle = %d\n",\
					__FUNCTION__, tmp_4bytes, last_idx_hw, desc_idx_hw, desc_idx_host, num_rxdesc_to_handle);

	last_idx_hw = desc_idx_hw;
	
	return num_rxdesc_to_handle;
}


static void rtl8192ee_rx_mpdu(_adapter *padapter)
{
	struct recv_priv	*precvpriv = &padapter->recvpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	_queue			*pfree_recv_queue = &precvpriv->free_recv_queue;
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(padapter);
	union recv_frame	*precvframe = NULL;
	u8				*pphy_info = NULL;
	struct rx_pkt_attrib	*pattrib = NULL;
	u8	qos_shift_sz = 0;
	u32	skb_len, alloc_sz;
	int	rx_queue_idx = RX_MPDU_QUEUE;
	u32	count = precvpriv->rxringcount;
	u16	remaing_rxdesc = 0;

#ifdef CONFIG_CONCURRENT_MODE
	_adapter *sec_padapter = padapter->pbuddy_adapter;
	union recv_frame	*precvframe_if2 = NULL;
	u8 *paddr1 = NULL ;
	u8 *secondary_myid = NULL ;

	struct sk_buff  *clone_pkt = NULL;
#endif	
	//RX NORMAL PKT

	remaing_rxdesc = rtl8192ee_check_rxdesc_remain(padapter, rx_queue_idx);
	while(remaing_rxdesc)
	{
		u8	*rx_desc = (u8 *)&precvpriv->rx_ring[rx_queue_idx].desc[precvpriv->rx_ring[rx_queue_idx].idx];//rx descriptor
		struct sk_buff *skb = precvpriv->rx_ring[rx_queue_idx].rx_buf[precvpriv->rx_ring[rx_queue_idx].idx];//rx pkt

		buf_desc_debug("RX:%s(%d), rx_desc addr = %x, total_length = %d, ring idx = %d, skb(rx_buf)=%x, buffer address (virtual = %x, phisycal = %x)\n", __FUNCTION__, __LINE__, (u32)rx_desc, GET_RX_BUFFER_DESC_TOTAL_LENGTH_92E(rx_desc), precvpriv->rx_ring[rx_queue_idx].idx, (u32)skb, (u32)(skb_tail_pointer(skb)), GET_RX_BUFFER_PHYSICAL_LOW_92E(rx_desc));

					
//8192EE_TODO		
		// wait until packet is ready. this operation is similar to check own bit and should be
		//		called before pci_unmap_single which release memory mapping
		
		if(rtl8192ee_wait_rxrdy(padapter,rx_desc, rx_queue_idx) != _SUCCESS)
		{
			buf_desc_debug("RX:%s(%d) packet not ready\n", __FUNCTION__, __LINE__);
		
		}
		
		{
			struct sk_buff  *pkt_copy = NULL;

			precvframe = rtw_alloc_recvframe(pfree_recv_queue);

			if(precvframe==NULL)
			{
				RT_TRACE(_module_rtl871x_recv_c_,_drv_err_,("recvbuf2recvframe: precvframe==NULL\n"));
				DBG_8192C("recvbuf2recvframe: precvframe==NULL\n");
				goto done;
			}

			_rtw_init_listhead(&precvframe->u.hdr.list);
//8192EE_TODO			
			//?? Not sure why 8192eu call this ??
			//precvframe->u.hdr.precvbuf = NULL;	//can't access the precvbuf for new arch.			
//8192EE_TODO						
			precvframe->u.hdr.len=0;

			pci_unmap_single(pdvobjpriv->ppcidev,
					*((dma_addr_t *)skb->cb), 
					precvpriv->rxbuffersize, 
					PCI_DMA_FROMDEVICE);


			rtl8192e_query_rx_desc_status(precvframe, skb->data);
			pattrib = &precvframe->u.hdr.attrib;

			buf_desc_debug("RX:%s(%d), pkt_len = %d, pattrib->drvinfo_sz = %d, pattrib->qos = %d, pattrib->shift_sz = %d\n", \
							__FUNCTION__, __LINE__, pattrib->pkt_len, pattrib->drvinfo_sz, pattrib->qos, pattrib->shift_sz );


			if(pattrib->physt)
			{	
				pphy_info = (u8 *)(skb->data) + RX_WIFI_INFO_SIZE;
			}

			//	Modified by Albert 20101213
			//	For 8 bytes IP header alignment.
			if (pattrib->qos)	//	Qos data, wireless lan header length is 26
			{
				qos_shift_sz = 6;
			}
			else
			{
				qos_shift_sz = 0;
			}

			skb_len = pattrib->pkt_len;

			// for first fragment packet, driver need allocate 1536+drvinfo_sz+RX_WIFI_INFO_SIZE to defrag packet.
			// modify alloc_sz for recvive crc error packet by thomas 2011-06-02
			if((pattrib->mfrag == 1)&&(pattrib->frag_num == 0)){
				//alloc_sz = 1664;	//1664 is 128 alignment.
				if(skb_len <= 1650)
					alloc_sz = 1664;
				else
					alloc_sz = skb_len + 14;
			}
			else {
				alloc_sz = skb_len;
				//	6 is for IP header 8 bytes alignment in QoS packet case.
				//	8 is for skb->data 4 bytes alignment.
				alloc_sz += 14;
			}


			pkt_copy = rtw_skb_alloc(alloc_sz);
			pkt_copy->len = skb_len;
			if(pkt_copy)
			{
				pkt_copy->dev = padapter->pnetdev;
				precvframe->u.hdr.pkt = pkt_copy;
				skb_reserve( pkt_copy, 8 - ((SIZE_PTR)( pkt_copy->data ) & 7 ));//force pkt_copy->data at 8-byte alignment address
				skb_reserve( pkt_copy, qos_shift_sz );//force ip_hdr at 8-byte alignment address according to shift_sz.
				_rtw_memcpy(pkt_copy->data, (skb->data + RX_WIFI_INFO_SIZE + pattrib->drvinfo_sz + pattrib->shift_sz), skb_len);

				buf_desc_debug("RX:%s(%d)offset = %d, skb_len=%d\n", __FUNCTION__, __LINE__, RX_WIFI_INFO_SIZE + pattrib->drvinfo_sz + pattrib->shift_sz, skb_len);
			
				precvframe->u.hdr.rx_head = pkt_copy->head;
				precvframe->u.hdr.rx_data = precvframe->u.hdr.rx_tail = pkt_copy->data;
				precvframe->u.hdr.rx_end = skb_end_pointer(pkt_copy);
			}
			else
			{	
				DBG_8192C("rtl8192ee_rx_mpdu:can not allocate memory for skb copy\n");
				*((dma_addr_t *) skb->cb) = pci_map_single(pdvobjpriv->ppcidev, skb_tail_pointer(skb), precvpriv->rxbuffersize, PCI_DMA_FROMDEVICE);
				goto done;
			}

			recvframe_put(precvframe, skb_len);

#ifdef CONFIG_CONCURRENT_MODE
			if ((!rtw_buddy_adapter_up(padapter)) || (pattrib->pkt_rpt_type != NORMAL_RX))
				goto skip_if2_recv;

			paddr1 = GetAddr1Ptr(precvframe->u.hdr.rx_data);
			if(IS_MCAST(paddr1) == _FALSE) {	//unicast packets
				secondary_myid = adapter_mac_addr(sec_padapter);

				if(_rtw_memcmp(paddr1, secondary_myid, ETH_ALEN)) {
					pkt_copy->dev = sec_padapter->pnetdev;
					precvframe->u.hdr.adapter = sec_padapter;
				}
			} else {
				precvframe_if2 = rtw_alloc_recvframe(pfree_recv_queue);
				if(precvframe_if2==NULL) {
					DBG_8192C("%s(): precvframe_if2==NULL\n", __func__);
					goto done;
				}
				clone_pkt = skb_copy(pkt_copy, GFP_ATOMIC);
				if (!clone_pkt){
					DBG_8192C("%s(): skb_copy==NULL\n", __func__);
					rtw_free_recvframe(precvframe_if2, pfree_recv_queue);
					goto done;
				}
				if (rtl812ee_if2_clone_recvframe(sec_padapter, precvframe, 
						precvframe_if2, clone_pkt, pphy_info) != _SUCCESS) 
					rtw_free_recvframe(precvframe_if2, pfree_recv_queue);
		
				if(rtw_recv_entry(precvframe_if2) != _SUCCESS)
					RT_TRACE(_module_hci_ops_os_c_,_drv_info_,("recvbuf2recvframe: rtw_recv_entry(precvframe) != _SUCCESS\n"));
			}
skip_if2_recv:
#endif

			if(pattrib->pkt_rpt_type == NORMAL_RX)//Normal rx packet
			{
				if (pattrib->physt)
					rx_query_phy_status(precvframe, pphy_info);

				if(rtw_recv_entry(precvframe) != _SUCCESS)
					//RT_TRACE(_module_hci_ops_os_c_,_drv_err_,("recvbuf2recvframe: rtw_recv_entry(precvframe) != _SUCCESS\n"));
					RT_TRACE(_module_hci_ops_os_c_,_drv_info_,("recvbuf2recvframe: rtw_recv_entry(precvframe) != _SUCCESS\n"));
			} else {
				if(pattrib->pkt_rpt_type == C2H_PACKET) {
					//DBG_871X("rx C2H_PACKET \n");
					C2HPacketHandler_8192E(padapter,precvframe->u.hdr.rx_data,pattrib->pkt_len);
				}
				rtw_free_recvframe(precvframe, pfree_recv_queue);
			}
			*((dma_addr_t *) skb->cb) = pci_map_single(pdvobjpriv->ppcidev, skb_tail_pointer(skb), precvpriv->rxbuffersize, PCI_DMA_FROMDEVICE);
		}
done:
		
		SET_RX_BUFFER_PHYSICAL_LOW_92E(rx_desc, *((dma_addr_t *)skb->cb));
		SET_RX_BUFFER_DESC_DATA_LENGTH_92E(rx_desc, precvpriv->rxbuffersize);
		
		precvpriv->rx_ring[rx_queue_idx].idx = (precvpriv->rx_ring[rx_queue_idx].idx + 1) % precvpriv->rxringcount;

		rtw_write16(padapter, REG_RXQ_TXBD_IDX_8192E, precvpriv->rx_ring[rx_queue_idx].idx);
		
		buf_desc_debug("RX:%s(%d) reg_value %x\n", __FUNCTION__, __LINE__, rtw_read32(padapter, REG_RXQ_TXBD_IDX_8192E));

		remaing_rxdesc--;
	}

}

void rtl8192ee_recv_tasklet(void *priv)
{
	_irqL	irqL;
	_adapter	*padapter = (_adapter*)priv;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);

	rtl8192ee_rx_mpdu(padapter);
	_enter_critical(&pdvobjpriv->irq_th_lock, &irqL);
	pHalData->IntrMask[0] |= (IMR_ROK_8192E|IMR_RDU_8192E);
	pHalData->IntrMask[1] |= IMR_RXFOVW_8192E;	
	rtw_write32(padapter, REG_HIMR0_8192E, pHalData->IntrMask[0]);
	rtw_write32(padapter, REG_HIMR1_8192E, pHalData->IntrMask[1]);
	_exit_critical(&pdvobjpriv->irq_th_lock, &irqL);
}

static u8 pci_read8(struct intf_hdl *pintfhdl, u32 addr)
{
	struct dvobj_priv  *pdvobjpriv = (struct dvobj_priv  *)pintfhdl->pintf_dev;
//	printk("%s, addr=%08x,  val=%02x \n", __func__, addr,  readb((u8 *)pdvobjpriv->pci_mem_start + addr));
	return 0xff & readb((u8 *)pdvobjpriv->pci_mem_start + addr);
}

static u16 pci_read16(struct intf_hdl *pintfhdl, u32 addr)
{
	struct dvobj_priv  *pdvobjpriv = (struct dvobj_priv  *)pintfhdl->pintf_dev;
//	printk("%s, addr=%08x,  val=%04x \n", __func__, addr,  readw((u8 *)pdvobjpriv->pci_mem_start + addr));
	return readw((u8 *)pdvobjpriv->pci_mem_start + addr);
}

static u32 pci_read32(struct intf_hdl *pintfhdl, u32 addr)
{
	struct dvobj_priv  *pdvobjpriv = (struct dvobj_priv  *)pintfhdl->pintf_dev;
//	printk("%s, addr=%08x,  val=%08x \n", __func__, addr,  readl((u8 *)pdvobjpriv->pci_mem_start + addr));
	return readl((u8 *)pdvobjpriv->pci_mem_start + addr);
}

//2009.12.23. by tynli. Suggested by SD1 victorh. For ASPM hang on AMD and Nvidia.
// 20100212 Tynli: Do read IO operation after write for all PCI bridge suggested by SD1.
// Origianally this is only for INTEL.
static int pci_write8(struct intf_hdl *pintfhdl, u32 addr, u8 val)
{
	struct dvobj_priv  *pdvobjpriv = (struct dvobj_priv  *)pintfhdl->pintf_dev;

	writeb(val, (u8 *)pdvobjpriv->pci_mem_start + addr);
	//readb((u8 *)pdvobjpriv->pci_mem_start + addr);
	return 1;
}

static int pci_write16(struct intf_hdl *pintfhdl, u32 addr, u16 val)
{	
	struct dvobj_priv  *pdvobjpriv = (struct dvobj_priv  *)pintfhdl->pintf_dev;
	writew(val, (u8 *)pdvobjpriv->pci_mem_start + addr);
	//readw((u8 *)pdvobjpriv->pci_mem_start + addr);
	return 2;
}

static int pci_write32(struct intf_hdl *pintfhdl, u32 addr, u32 val)
{
	struct dvobj_priv  *pdvobjpriv = (struct dvobj_priv  *)pintfhdl->pintf_dev;
	writel(val, (u8 *)pdvobjpriv->pci_mem_start + addr);
	//readl((u8 *)pdvobjpriv->pci_mem_start + addr);
	return 4;
}


static void pci_read_mem(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *rmem)
{
	
}

static void pci_write_mem(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *wmem)
{
	
}

static u32 pci_read_port(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *rmem)
{	
	return 0;
}

void rtl8192ee_xmit_tasklet(void *priv)
{
	_irqL	irqL;
	_adapter			*padapter = (_adapter*)priv;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	//struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;

	/*_enter_critical(&pdvobjpriv->irq_th_lock, &irqL);

	if (pHalData->IntArray[0] & IMR_MGNTDOK_8812) {
		//DBG_8192C("Manage ok interrupt!\n");		
		rtl8812ae_tx_isr(padapter, MGT_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_HIGHDOK_8812) {
		//DBG_8192C("HIGH_QUEUE ok interrupt!\n");
		rtl8812ae_tx_isr(padapter, HIGH_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_BKDOK_8812) {
		//DBG_8192C("BK Tx OK interrupt!\n");
		rtl8812ae_tx_isr(padapter, BK_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_BEDOK_8812) {
		//DBG_8192C("BE TX OK interrupt!\n");
		rtl8812ae_tx_isr(padapter, BE_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_VIDOK_8812) {
		//DBG_8192C("VI TX OK interrupt!\n");
		rtl8812ae_tx_isr(padapter, VI_QUEUE_INX);
	}

	if (pHalData->IntArray[0] & IMR_VODOK_8812) {
		//DBG_8192C("Vo TX OK interrupt!\n");
		rtl8812ae_tx_isr(padapter, VO_QUEUE_INX);
	}

	pHalData->IntrMask[0] |= IMR_TX_MASK;
	rtw_write32(padapter, REG_HIMR0_8812, pHalData->IntrMask[0]);

	_exit_critical(&pdvobjpriv->irq_th_lock, &irqL);
*/
	//if (check_fwstate(&padapter->mlmepriv, _FW_UNDER_SURVEY) != _TRUE)
	{
		// try to deal with the pending packets
		rtl8192ee_xmitframe_resume(padapter);
	}

}

static u32 pci_write_port(struct intf_hdl *pintfhdl, u32 addr, u32 cnt, u8 *wmem)
{
	_adapter			*padapter = (_adapter *)pintfhdl->padapter;

	padapter->pnetdev->trans_start = jiffies;

	return 0;
}

void rtl8192ee_set_intf_ops(struct _io_ops	*pops)
{
	_func_enter_;
	
	_rtw_memset((u8 *)pops, 0, sizeof(struct _io_ops));	

	pops->_read8 = &pci_read8;
	pops->_read16 = &pci_read16;
	pops->_read32 = &pci_read32;

	pops->_read_mem = &pci_read_mem;
	pops->_read_port = &pci_read_port;	

	pops->_write8 = &pci_write8;
	pops->_write16 = &pci_write16;
	pops->_write32 = &pci_write32;

	pops->_write_mem = &pci_write_mem;
	pops->_write_port = &pci_write_port;
		
	_func_exit_;

}

