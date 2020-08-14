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
#define _RTL8192EE_XMIT_C_

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

s32	rtl8192ee_init_xmit_priv(_adapter *padapter)
{
	s32	ret = _SUCCESS;
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);

	_rtw_spinlock_init(&pdvobjpriv->irq_th_lock);

#ifdef PLATFORM_LINUX
	tasklet_init(&pxmitpriv->xmit_tasklet,
	     (void(*)(unsigned long))rtl8192ee_xmit_tasklet,
	     (unsigned long)padapter);
#endif

	return ret;
}

void	rtl8192ee_free_xmit_priv(_adapter *padapter)
{
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);

	_rtw_spinlock_free(&pdvobjpriv->irq_th_lock);
}

s32 rtl8192ee_enqueue_xmitbuf(struct rtw_tx_ring	*ring, struct xmit_buf *pxmitbuf)
{
	_irqL irqL;
	_queue *ppending_queue = &ring->queue;
	
_func_enter_;	

	//DBG_8192C("+enqueue_xmitbuf\n");

	if(pxmitbuf==NULL)
	{		
		return _FAIL;
	}
	
	//_enter_critical(&ppending_queue->lock, &irqL);
	
	rtw_list_delete(&pxmitbuf->list);	
	
	rtw_list_insert_tail(&(pxmitbuf->list), get_list_head(ppending_queue));

	ring->qlen++;

	//DBG_8192C("FREE, free_xmitbuf_cnt=%d\n", pxmitpriv->free_xmitbuf_cnt);
		
	//_exit_critical(&ppending_queue->lock, &irqL);	

_func_exit_;	 

	return _SUCCESS;	
} 


struct xmit_buf * rtl8192ee_dequeue_xmitbuf(struct rtw_tx_ring	*ring)
{
	_irqL irqL;
	_list *plist, *phead;
	struct xmit_buf *pxmitbuf =  NULL;
	_queue *ppending_queue = &ring->queue;

_func_enter_;

	//_enter_critical(&ppending_queue->lock, &irqL);

	if(_rtw_queue_empty(ppending_queue) == _TRUE) {
		pxmitbuf = NULL;
	} else {

		phead = get_list_head(ppending_queue);

		plist = get_next(phead);

		pxmitbuf = LIST_CONTAINOR(plist, struct xmit_buf, list);

		rtw_list_delete(&(pxmitbuf->list));

		ring->qlen--;
	}
		
	//_exit_critical(&ppending_queue->lock, &irqL);	

_func_exit_;	 

	return pxmitbuf;	
} 

static u8 *get_txdesc(_adapter *padapter, u8 queue_index)
{
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct rtw_tx_ring	*ring;
	u8	*pdesc = NULL;
	int	idx=0;

   	ring = &pxmitpriv->tx_ring[queue_index];

	/* DO NOT use last entry. */
    /* To avoid wrap around overlap issue in cycler queue. */
	if (ring->qlen == (ring->entries-1)) {
		DBG_8192C("No more TX desc@%d, ring->idx = %d,idx = %d \n", queue_index, ring->idx, idx);
   		return NULL; 
	}    

   	if (queue_index != BCN_QUEUE_INX) {
		idx = (ring->idx + ring->qlen) % ring->entries;
   	} else {
		idx = 0;
   	}

  	pdesc = (u8 *)&ring->desc[idx];
	if((GET_TX_DESC_OWN_92E(pdesc)) && (queue_index != BCN_QUEUE_INX)) {
		DBG_8192C("No more TX desc@%d, ring->idx = %d,idx = %d \n", queue_index, ring->idx, idx);
		return NULL;
	}

	return pdesc;
}

u16	get_txdesc_buf_addr(u16 ff_hwaddr)
{

	u16	txbuf_desc_addr=REG_BEQ_TXBD_IDX_8192E;
	switch (ff_hwaddr)
	{
		case BK_QUEUE_INX:
			txbuf_desc_addr = REG_BKQ_TXBD_IDX_8192E;
			break;	
		
		case BE_QUEUE_INX:
			txbuf_desc_addr = REG_BEQ_TXBD_IDX_8192E;
			break;	
		
		case VI_QUEUE_INX:
			txbuf_desc_addr = REG_VIQ_TXBD_IDX_8192E;
			break;	
		
		case VO_QUEUE_INX:
			txbuf_desc_addr = REG_VOQ_TXBD_IDX_8192E;
			break;	
		
		case BCN_QUEUE_INX:
			txbuf_desc_addr = REG_BEQ_TXBD_IDX_8192E;   //need check
			break;	
		
		case TXCMD_QUEUE_INX:
			txbuf_desc_addr = REG_BEQ_TXBD_IDX_8192E;
			break;	
		
		case MGT_QUEUE_INX:
			txbuf_desc_addr = REG_MGQ_TXBD_IDX_8192E;
			break;	
			
		case HIGH_QUEUE_INX:
			txbuf_desc_addr = REG_HI0Q_TXBD_IDX_8192E;   //need check
			break;	

		default:
			break;
	}

	return txbuf_desc_addr;
}

struct xmit_frame *__rtw_alloc_cmdxmitframe_8192ee(struct xmit_priv *pxmitpriv,
		enum cmdbuf_type buf_type)
{
	_adapter *padapter = pxmitpriv->adapter;
	u16	queue_idx = BCN_QUEUE_INX;
	u8 *ptxdesc=NULL;

#ifdef CONFIG_CONCURRENT_MODE
	if(rtw_buddy_adapter_up(padapter))
		if(padapter->adapter_type > PRIMARY_ADAPTER)
			padapter = padapter->pbuddy_adapter;
#endif

	ptxdesc = get_txdesc(padapter, BCN_QUEUE_INX);		

	/* set OWN bit in Beacon tx descriptor */
	if (ptxdesc != NULL){
		SET_TX_BUFF_DESC_OWN_92E(ptxdesc, 0);
	}else{
		return(NULL);
	}

	return __rtw_alloc_cmdxmitframe(pxmitpriv, CMDBUF_BEACON);
}

/*
  * Update Read/Write pointer
  * 		Read pointer is h/w descriptor index
  *		Write pointer is host desciptor index: For tx side, if own bit is set in packet index n, 
  *										host pointer (write pointer) point to index n + 1.)
  */
void fill_txdesc_own(_adapter *padapter, struct xmit_frame *pxmitframe, u8* txdesc, u32 current_desc_idx)
{
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct rtw_tx_ring	*ring;   	
	u16	queue_idx = 0, next_desc_idx;

	queue_idx = rtw_get_ff_hwaddr(pxmitframe);
	ring = &pxmitpriv->tx_ring[queue_idx];

	buf_desc_debug("TX: enter %s(%d), queue_idx = %d, desc_idx = %x, txdesc = %x, pointer = 0x%08x, dma_startaddr = 0x%08x\n ",\
					__FUNCTION__, __LINE__, queue_idx, current_desc_idx, (u32)txdesc, \
					rtw_read32(padapter, get_txdesc_buf_addr(queue_idx)),
					(u32)ring->dma);

	if(txdesc == NULL)
		return;

	if(queue_idx == BCN_QUEUE_INX)
	{
		//Adapter->CurrentTXWritePoint[QueueIndex] = Adapter->CurrentTXReadPoint[QueueIndex] =0;

		SET_TX_BUFF_DESC_OWN_92E(txdesc, 1);

	    // kick start
	  	rtw_write8( padapter, REG_RX_RXBD_NUM+1,
				    rtw_read8(padapter, REG_RX_RXBD_NUM+1) | BIT(4));

		return;
	}

	//8192EE_TODO
				if(0){
					int i, j;
					u8* buf= pxmitframe->buf_addr + TX_WIFI_INFO_SIZE;
					DBG_871X("##############%s############### \n", __FUNCTION__);
				
					
					for(i=0; i< 32;i++)
					{
						if((i%16==0) && (i!=0))
							_dbgdump("\n");
						_dbgdump("%02X ", *(buf+i));
				
						if(i%4==3)
							_dbgdump("\t");
					}
					DBG_871X("\n############################# \n");

				}	
//8192EE_TODO

	// update h/w index
	// for tx side, if own bit is set in packet index n, host pointer (write pointer) point to index n + 1.
	next_desc_idx = (current_desc_idx + 1) % ring->entries;
	rtw_write16(padapter, get_txdesc_buf_addr(queue_idx), next_desc_idx);
	buf_desc_debug("TX: exit %s(%d), after update h/w idx, pointer = 0x%08x\n", __FUNCTION__, __LINE__, rtw_read32(padapter, get_txdesc_buf_addr(queue_idx))); 
#if 0	
	Adapter->CurrentTXWritePoint[QueueIndex] = ((Adapter->CurrentTXWritePoint[QueueIndex]+1)%MaxTxDesc); 

	if(Adapter->AvailableDescToWrite[QueueIndex] >1)
	{
		Adapter->AvailableDescToWrite[QueueIndex]--;
		
		PlatformEFIOWrite2Byte(Adapter,GetDescAddressFromQueueIndex(QueueIndex),Adapter->CurrentTXWritePoint[QueueIndex]);

		if(QueueIndex==1)
		{
			lasttxwpoint= CurrentTXWritePoint;
		}
	}


	if(Adapter->AvailableDescToWrite[QueueIndex]<(MaxTxDesc-15))
	{
		u2Byte PointDiff=0;
		tmp4Byte=PlatformEFIORead4Byte(Adapter,GetDescAddressFromQueueIndex(QueueIndex)); //be queue.
		CurrentTXReadPoint = (u2Byte)((tmp4Byte >>16) & 0x0fff);
		CurrentTXWritePoint = (u2Byte)((tmp4Byte) & 0x0fff);		

		Adapter->CurrentTXWritePoint[QueueIndex]=CurrentTXWritePoint;

		if(Adapter->CurrentTXWritePoint[QueueIndex] !=CurrentTXWritePoint)
		{
			RT_DISP(FRW_POINT,FRW_POINT_DBG,("CurrentTXWritePoint[%d] = %d !=   TmpValue %x\n",QueueIndex,Adapter->CurrentTXWritePoint[QueueIndex],tmp4Byte));
		}
		Adapter->CurrentTXReadPoint[QueueIndex]=CurrentTXReadPoint;
		PointDiff = ((CurrentTXReadPoint>CurrentTXWritePoint)? (CurrentTXReadPoint-CurrentTXWritePoint):(TX_DESC_NUM_92E-CurrentTXWritePoint+CurrentTXReadPoint));
		
		Adapter->AvailableDescToWrite[QueueIndex]=PointDiff;
	}
#endif		
}


static u16 ffaddr2dma(u32 addr)
{
	u16	dma_ctrl;
	switch(addr)
	{
		case VO_QUEUE_INX:
			dma_ctrl = BIT3;
			break;
		case VI_QUEUE_INX:
			dma_ctrl = BIT2;
			break;
		case BE_QUEUE_INX:
			dma_ctrl = BIT1;
			break;
		case BK_QUEUE_INX:
			dma_ctrl = BIT0;
			break;
		case BCN_QUEUE_INX:
			dma_ctrl = BIT4;
			break;
		case MGT_QUEUE_INX:
			dma_ctrl = BIT6;
			break;
		case HIGH_QUEUE_INX:
			dma_ctrl = BIT7;
			break;
		default:
			dma_ctrl = 0;
			break;
	}

	return dma_ctrl;
}

/*
 * Fill tx buffer desciptor. Map each buffer address in tx buffer descriptor segment
 * Designed for tx buffer descriptor architecture
 * Input *pmem: pointer to the Tx Buffer Descriptor
 */
void update_txdesc_buf(struct xmit_frame *pxmitframe, u8 *pmem, s32 sz)
{
	_adapter			*padapter = pxmitframe->padapter;
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	dma_addr_t		mapping;
	u32				i=0;
	u16				seg_num = ((TX_BUFFER_SEG_NUM ==0)?2: ((TX_BUFFER_SEG_NUM ==1)? 4:8) );
	u16				tx_page_size_reg = 2;
	u16 				page_size_length = 0;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);

	
	mapping = pci_map_single(pdvobjpriv->ppcidev, pxmitframe->buf_addr , sz + TX_WIFI_INFO_SIZE, PCI_DMA_TODEVICE);

	// Calculate page size. Total buffer length including TX_WIFI_INFO and PacketLen
	if(tx_page_size_reg > 0)
	{
		page_size_length = (sz + TX_WIFI_INFO_SIZE) /  (tx_page_size_reg * 128);
		if ( ((sz + TX_WIFI_INFO_SIZE) %  (tx_page_size_reg * 128)) > 0)
			page_size_length ++;	
	}

	
	// Reset all tx buffer desciprtor content
	// -- Reset first element
	SET_TX_BUFF_DESC_LEN_0_92E(pmem,0); 
	SET_TX_BUFF_DESC_PSB_92E(pmem,0);
	SET_TX_BUFF_DESC_OWN_92E(pmem, 0);
	// -- Reset second and other element
	for(i = 1 ; i < seg_num ; i++)
	{
		SET_TXBUFFER_DESC_LEN_WITH_OFFSET(pmem, i, 0);
		SET_TXBUFFER_DESC_AMSDU_WITH_OFFSET(pmem, i, 0);
		SET_TXBUFFER_DESC_ADD_LOW_WITH_OFFSET(pmem, i, 0);
	}

	// Fill buffer length of the first buffer, 
	// For 8192ee, it is requried that TX_WIFI_INFO is put in first segment, and the size
	// 		of the first segment cannot be larger than TX_WIFI_INFO_SIZE.
	SET_TX_BUFF_DESC_LEN_0_92E(pmem, TX_WIFI_INFO_SIZE);
	SET_TX_BUFF_DESC_PSB_92E(pmem, page_size_length);
	SET_TX_BUFF_DESC_ADDR_LOW_0_92E(pmem, mapping);

	// It is assumed that in linux implementation, packet is coalesced in only one buffer
	// Extension mode is not supported here
	SET_TXBUFFER_DESC_LEN_WITH_OFFSET(pmem, 1, sz);
	SET_TXBUFFER_DESC_AMSDU_WITH_OFFSET(pmem, 1, 0); // don't using extendsion mode.
	SET_TXBUFFER_DESC_ADD_LOW_WITH_OFFSET(pmem, 1, mapping + TX_WIFI_INFO_SIZE);		


	//8192EE_TODO
				if(0){
					int i, j;
					u8* buf= pxmitframe->buf_addr + TX_WIFI_INFO_SIZE;
					DBG_871X("##############%s############### \n", __FUNCTION__);
					buf[4] = 0x01;
					buf[5] = 0x02;
					buf[6] = 0x03;
					buf[7] = 0x04;
					buf[8] = 0x05;
					buf[9] = 0x06;				
					buf[16] = 0x07;
					buf[17] = 0x08;
					buf[18] = 0x09;
					buf[19] = 0x0a;
					buf[20] = 0x0b;
					buf[21] = 0x0c;				
					for(i=0; i< 32;i++)
					{
						if((i%16==0) && (i!=0))
							_dbgdump("\n");
						_dbgdump("%02X ", *(buf+i));
				
						if(i%4==3)
							_dbgdump("\t");
					}
					DBG_871X("\n############################# \n");

				}	
//8192EE_TODO

	buf_desc_debug("TX:%s, tx_buf_desc = 0x%08x\n", __FUNCTION__, (u32)pmem);
	buf_desc_debug("TX:%s, Offset00(0x%08x)\n", __FUNCTION__, *((u32 *)(pmem)));
	buf_desc_debug("TX:%s, Offset04(0x%08x)\n", __FUNCTION__, *((u32 *)(pmem+4)));
	buf_desc_debug("TX:%s, Offset08(0x%08x)\n", __FUNCTION__, *((u32 *)(pmem+8)));
	buf_desc_debug("TX:%s, Offset12(0x%08x)\n", __FUNCTION__, *((u32 *)(pmem+12)));

}
	
static s32 update_txdesc(struct xmit_frame *pxmitframe, u8 *pmem, s32 sz)
{	
	uint	qsel;
	u8 data_rate,pwr_status;
//8192EE_TODO	
//	dma_addr_t	mapping;
//8192EE_TODO
	_adapter			*padapter = pxmitframe->padapter;
	struct mlme_priv	*pmlmepriv = &padapter->mlmepriv;
	struct dvobj_priv		*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pkt_attrib	*pattrib = &pxmitframe->attrib;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);
	struct dm_priv	*pdmpriv = &pHalData->dmpriv;
	struct mlme_ext_priv	*pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info	*pmlmeinfo = &(pmlmeext->mlmext_info);
//8192EE_TODO	
	u8	*ptxdesc;	//pointer to TXWiFiInfo
	dma_addr_t *mapping = NULL;
//8192EE_TODO
	sint	bmcst = IS_MCAST(pattrib->ra);

#ifdef CONFIG_CONCURRENT_MODE
	if(rtw_buddy_adapter_up(padapter) && padapter->adapter_type > PRIMARY_ADAPTER)	
		pHalData = GET_HAL_DATA(padapter->pbuddy_adapter);				
#endif //CONFIG_CONCURRENT_MODE

	// Step 1: Fill tx buffer descriptor. Assume only one buffer is used in Linux driver
	// update_txdesc_buf(pxmitframe, pmem, sz); //must be done after update_txdesc()

	// Step 2: Fill tx desciptor	
	ptxdesc = pxmitframe->buf_addr; 

	//reset descriptor content 
	_rtw_memset(ptxdesc, 0, TXDESC_SIZE);

//8192EE_TODO
//	mapping = pci_map_single(pdvobjpriv->ppcidev, pxmitframe->buf_addr , sz, PCI_DMA_TODEVICE);

//	_rtw_memset(ptxdesc, 0, TX_DESC_NEXT_DESC_OFFSET);
//8192EE_TODO

	
        //4 offset 0
//8192EE_TODO        
//	SET_TX_DESC_FIRST_SEG_92E(ptxdesc, 1);
//	SET_TX_DESC_LAST_SEG_92E(ptxdesc, 1);
	//SET_TX_DESC_OWN_8812(ptxdesc, 1);
//8192EE_TODO        	
	
	//SET_TX_DESC_PKT_SIZE_92E(ptxdesc, sz);
	SET_TX_DESC_PKT_SIZE_92E(ptxdesc, sz);
	SET_TX_DESC_OFFSET_92E(ptxdesc, TXDESC_SIZE + OFFSET_SZ);

#ifdef CONFIG_TX_EARLY_MODE	
	// No tested yet for 8192ee
	SET_TX_DESC_PKT_OFFSET_92E(ptxdesc, 1);
	SET_TX_DESC_OFFSET_92E(ptxdesc, TXDESC_SIZE + OFFSET_SZ + EARLY_MODE_INFO_SIZE);
#endif

	if (bmcst) {
		SET_TX_DESC_BMC_92E(ptxdesc, 1);
	}

	SET_TX_DESC_MACID_92E(ptxdesc, pattrib->mac_id);
	SET_TX_DESC_RATE_ID_92E(ptxdesc, pattrib->raid);

	SET_TX_DESC_QUEUE_SEL_92E(ptxdesc,  pattrib->qsel);

	if (!pattrib->qos_en) {
		SET_TX_DESC_EN_HWSEQ_92E(ptxdesc, 1); // Hw set sequence number
		SET_TX_DESC_HWSEQ_SEL_92E(ptxdesc, pattrib->hw_ssn_sel);
	} else {
		SET_TX_DESC_SEQ_92E(ptxdesc, pattrib->seqnum);
	}

	if((pxmitframe->frame_tag&0x0f) == DATA_FRAMETAG)
	{
		//DBG_8192C("pxmitframe->frame_tag == DATA_FRAMETAG\n");		

		fill_txdesc_sectype(pattrib, ptxdesc);		

		if (pattrib->ampdu_en==_TRUE) {
			SET_TX_DESC_AGG_ENABLE_92E(ptxdesc, 1);
			SET_TX_DESC_MAX_AGG_NUM_92E(ptxdesc, 0x1f);
			// Set A-MPDU aggregation.
			SET_TX_DESC_AMPDU_DENSITY_92E(ptxdesc, pattrib->ampdu_spacing/*pHalData->AMPDUDensity*/);
		} else {
			SET_TX_DESC_BK_92E(ptxdesc, 1);
		}
			
		if ((pattrib->ether_type != 0x888e) &&
		    (pattrib->ether_type != 0x0806) &&
		    (pattrib->ether_type != 0x88b4) &&
		    (pattrib->dhcp_pkt != 1)
#ifdef CONFIG_AUTO_AP_MODE
		    && (pattrib->pctrl != _TRUE)
#endif
		)
		{
			//Non EAP & ARP & DHCP type data packet

			fill_txdesc_vcs(pattrib, ptxdesc);
			fill_txdesc_phy(padapter, pattrib, ptxdesc);

			SET_TX_DESC_RTS_RATE_92E(ptxdesc, 0x8);//RTS Rate=24M
			//DATA/RTS  Rate FB LMT
			SET_TX_DESC_DATA_RATE_FB_LIMIT_92E(ptxdesc, 0x1f);
			SET_TX_DESC_RTS_RATE_FB_LIMIT_92E(ptxdesc, 0xf);

			if (pHalData->fw_ractrl == _FALSE) {
				SET_TX_DESC_USE_RATE_92E(ptxdesc, 1);
				
				if(pdmpriv->INIDATA_RATE[pattrib->mac_id] & BIT(7))
					SET_TX_DESC_DATA_SHORT_92E(ptxdesc, 	1);

				SET_TX_DESC_TX_RATE_92E(ptxdesc, (pdmpriv->INIDATA_RATE[pattrib->mac_id] & 0x7F));
			}

			if (padapter->fix_rate != 0xFF) { // modify data rate by iwpriv
				SET_TX_DESC_USE_RATE_92E(ptxdesc, 1);
				if(padapter->fix_rate & BIT(7))
					SET_TX_DESC_DATA_SHORT_92E(ptxdesc, 	1);
				if (!padapter->data_fb)
					SET_TX_DESC_DISABLE_FB_92E(ptxdesc, 1)
				SET_TX_DESC_TX_RATE_92E(ptxdesc, (padapter->fix_rate & 0x7F));
			}

		}
		else
		{
			// EAP data packet and ARP packet and DHCP.
			// Use the 1M data rate to send the EAP/ARP packet.
			// This will maybe make the handshake smooth.

			SET_TX_DESC_USE_RATE_92E(ptxdesc, 1);

			SET_TX_DESC_BK_92E(ptxdesc, 1);

			// HW will ignore this setting if the transmission rate is legacy OFDM.
			if (pmlmeinfo->preamble_mode == PREAMBLE_SHORT) {
				SET_TX_DESC_DATA_SHORT_92E(ptxdesc, 1);
			}

			SET_TX_DESC_TX_RATE_92E(ptxdesc, MRateToHwRate(pmlmeext->tx_rate));
		}

#ifdef CONFIG_TDLS
#ifdef CONFIG_XMIT_ACK
		/* CCX-TXRPT ack for xmit mgmt frames. */
		if (pxmitframe->ack_report) {
			SET_TX_DESC_SPE_RPT_92E(ptxdesc, 1);
			#ifdef DBG_CCX
			DBG_871X("%s set tx report\n", __func__);
			#endif
		}
#endif /* CONFIG_XMIT_ACK */
#endif
	}
	else if((pxmitframe->frame_tag&0x0f)== MGNT_FRAMETAG)
	{
		//DBG_8192C("pxmitframe->frame_tag == MGNT_FRAMETAG\n");	

		SET_TX_DESC_MBSSID_92E(ptxdesc, pattrib->mbssid);

		//offset 20
		SET_TX_DESC_RETRY_LIMIT_ENABLE_92E(ptxdesc, 1);
		if (pattrib->retry_ctrl == _TRUE) {
			SET_TX_DESC_DATA_RETRY_LIMIT_92E(ptxdesc, 6);
		} else {
			SET_TX_DESC_DATA_RETRY_LIMIT_92E(ptxdesc, 12);
		}

		SET_TX_DESC_USE_RATE_92E(ptxdesc, 1);

#ifdef CONFIG_INTEL_PROXIM
		if((padapter->proximity.proxim_on==_TRUE)&&(pattrib->intel_proxim==_TRUE)){
			DBG_871X("\n %s pattrib->rate=%d\n",__FUNCTION__,pattrib->rate);
			SET_TX_DESC_TX_RATE_8812(ptxdesc, pattrib->rate);
		}
		else
#endif
		{
			SET_TX_DESC_TX_RATE_92E(ptxdesc, MRateToHwRate(pmlmeext->tx_rate));
		}
#ifdef CONFIG_XMIT_ACK
		//CCX-TXRPT ack for xmit mgmt frames.
		if (pxmitframe->ack_report) {
			SET_TX_DESC_SPE_RPT_92E(ptxdesc, 1);
			#ifdef DBG_CCX
			DBG_871X("%s set tx report\n", __func__);
			#endif
		}
#endif //CONFIG_XMIT_ACK
	}
	else if((pxmitframe->frame_tag&0x0f) == TXAGG_FRAMETAG)
	{
		DBG_8192C("pxmitframe->frame_tag == TXAGG_FRAMETAG\n");
	}
#ifdef CONFIG_MP_INCLUDED
	else if(((pxmitframe->frame_tag&0x0f) == MP_FRAMETAG) &&
		(padapter->registrypriv.mp_mode == 1))
	{
		fill_txdesc_for_mp(padapter, ptxdesc);
	}
#endif
	else
	{
		DBG_8192C("pxmitframe->frame_tag = %d\n", pxmitframe->frame_tag);

		SET_TX_DESC_USE_RATE_92E(ptxdesc, 1);
		SET_TX_DESC_TX_RATE_92E(ptxdesc, MRateToHwRate(pmlmeext->tx_rate));
	}

	SET_TX_DESC_TX_BUFFER_SIZE_92E(ptxdesc, sz);

	_dbg_dump_tx_info(padapter,pxmitframe->frame_tag,ptxdesc);	
	return 0;
}

s32 rtw_dump_xframe(_adapter *padapter, struct xmit_frame *pxmitframe)
{
	s32 ret = _SUCCESS;
	s32 inner_ret = _SUCCESS;
       _irqL irqL;
	int	t, sz, w_sz, pull=0;
	u32	ff_hwaddr;
	struct xmit_buf		*pxmitbuf = pxmitframe->pxmitbuf;
	struct pkt_attrib		*pattrib = &pxmitframe->attrib;
	struct xmit_priv		*pxmitpriv = &padapter->xmitpriv;
	struct dvobj_priv		*pdvobjpriv = adapter_to_dvobj(padapter);
	struct security_priv	*psecuritypriv = &padapter->securitypriv;
	u8					*ptxdesc;
	u16					current_desc_idx = 0;

	if ((pxmitframe->frame_tag == DATA_FRAMETAG) &&
	    (pxmitframe->attrib.ether_type != 0x0806) &&
	    (pxmitframe->attrib.ether_type != 0x888e) &&
	    (pxmitframe->attrib.dhcp_pkt != 1))
	{
		rtw_issue_addbareq_cmd(padapter, pxmitframe);
	}

	//mem_addr = pxmitframe->buf_addr;

       RT_TRACE(_module_rtl871x_xmit_c_,_drv_info_,("rtw_dump_xframe()\n"));

	for (t = 0; t < pattrib->nr_frags; t++)
	{
		if (inner_ret != _SUCCESS && ret ==_SUCCESS)
			ret = _FAIL;

		if (t != (pattrib->nr_frags - 1))
		{
			RT_TRACE(_module_rtl871x_xmit_c_,_drv_err_,("pattrib->nr_frags=%d\n", pattrib->nr_frags));

			sz = pxmitpriv->frag_len;
			sz = sz - 4 - (psecuritypriv->sw_encrypt ? 0 : pattrib->icv_len);					
		}
		else //no frag
		{
			sz = pattrib->last_txcmdsz;
		}

		ff_hwaddr = rtw_get_ff_hwaddr(pxmitframe);
#ifdef CONFIG_CONCURRENT_MODE
		if(!rtw_buddy_adapter_up(padapter))
				goto skip_if2_tx;
		if(padapter->adapter_type > PRIMARY_ADAPTER)
		{
			_adapter 	  *pri_adapter = padapter->pbuddy_adapter;
			struct dvobj_priv *pri_dvobjpriv = adapter_to_dvobj(pri_adapter);
		
			_enter_critical(&(pri_dvobjpriv->irq_th_lock), &irqL);

			ptxdesc = get_txdesc(pri_adapter, ff_hwaddr);		
		
			current_desc_idx = (pri_adapter->xmitpriv.tx_ring[ff_hwaddr].idx + 
								pri_adapter->xmitpriv.tx_ring[ff_hwaddr].qlen) % 
								pri_adapter->xmitpriv.tx_ring[ff_hwaddr].entries;
				
#ifndef CONFIG_BCN_ICF
			if (BCN_QUEUE_INX == ff_hwaddr)
				padapter->xmitpriv.beaconDMAing = _TRUE;
#endif

			if(ptxdesc == NULL)
			{
				_exit_critical(&pri_dvobjpriv->irq_th_lock, &irqL);
				rtw_sctx_done_err(&pxmitbuf->sctx, RTW_SCTX_DONE_TX_DESC_NA);
				rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
				DBG_8192C("##### Tx desc unavailable !#####\n");
				break;
			}

			update_txdesc(pxmitframe, ptxdesc, sz);

			/* update_txdesc_buf() must be called after update_txdesc().      */ 
            /* It rely on update_txdesc_buf() to map it into non cache memory */
			update_txdesc_buf(pxmitframe, ptxdesc, sz); 

			if (pxmitbuf->buf_tag != XMITBUF_CMD)
				rtl8192ee_enqueue_xmitbuf(&(pri_adapter->xmitpriv.tx_ring[ff_hwaddr]), pxmitbuf);

			pxmitbuf->len = sz + TX_WIFI_INFO_SIZE;
			w_sz = sz;

			wmb();
			fill_txdesc_own(padapter, pxmitframe, ptxdesc, current_desc_idx);
			_exit_critical(&pri_dvobjpriv->irq_th_lock, &irqL);

			//rtw_write16(pri_adapter, REG_PCIE_CTRL_REG, ffaddr2dma(ff_hwaddr));
			inner_ret = rtw_write_port(padapter, ff_hwaddr, w_sz, (unsigned char*)pxmitbuf);

		} else
skip_if2_tx:
#endif
		{
		_enter_critical(&pdvobjpriv->irq_th_lock, &irqL);

		ptxdesc = get_txdesc(pxmitframe->padapter, ff_hwaddr);

		current_desc_idx = (pxmitpriv->tx_ring[ff_hwaddr].idx + 
							pxmitpriv->tx_ring[ff_hwaddr].qlen) % 
							pxmitpriv->tx_ring[ff_hwaddr].entries;

#ifndef CONFIG_BCN_ICF
		if (BCN_QUEUE_INX == ff_hwaddr)
			padapter->xmitpriv.beaconDMAing = _TRUE;
#endif

		if(ptxdesc == NULL)
		{
			_exit_critical(&pdvobjpriv->irq_th_lock, &irqL);
			rtw_sctx_done_err(&pxmitbuf->sctx, RTW_SCTX_DONE_TX_DESC_NA);
			rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
			DBG_8192C("##### Tx desc unavailable !#####\n");
			break;
		}

		update_txdesc(pxmitframe, ptxdesc, sz);

		/* update_txdesc_buf() must be called after update_txdesc().      */ 
		/* It rely on update_txdesc_buf() to map it into non cache memory */
		update_txdesc_buf(pxmitframe, ptxdesc, sz); 
		
		if (pxmitbuf->buf_tag != XMITBUF_CMD)
			rtl8192ee_enqueue_xmitbuf(&pxmitpriv->tx_ring[ff_hwaddr], pxmitbuf);

		pxmitbuf->len = sz + TX_WIFI_INFO_SIZE;
		w_sz = sz;

		wmb();
		fill_txdesc_own(padapter, pxmitframe, ptxdesc, current_desc_idx);

		_exit_critical(&pdvobjpriv->irq_th_lock, &irqL);
//8192EE_DOTO		
//		rtw_write16(padapter, REG_PCIE_CTRL_REG, ffaddr2dma(ff_hwaddr));
//8192EE_DOTO		
		inner_ret = rtw_write_port(padapter, ff_hwaddr, w_sz, (unsigned char*)pxmitbuf);
		}

		rtw_count_tx_stats(padapter, pxmitframe, sz);
		RT_TRACE(_module_rtl871x_xmit_c_,_drv_info_,("rtw_write_port, w_sz=%d\n", w_sz));
		//DBG_8192C("rtw_write_port, w_sz=%d, sz=%d, txdesc_sz=%d, tid=%d\n", w_sz, sz, w_sz-sz, pattrib->priority);      

		//mem_addr += w_sz;

		//mem_addr = (u8 *)RND4(((SIZE_PTR)(mem_addr)));

	}
	
	rtw_free_xmitframe(pxmitpriv, pxmitframe);

	if  (ret != _SUCCESS)
		rtw_sctx_done_err(&pxmitbuf->sctx, RTW_SCTX_DONE_UNKNOWN);
	
	return ret;
}

/*
  * Packet should not be dequeued if there is no available descriptor
  * return: _SUCCESS if there is available descriptor
  */
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

static u8 check_nic_enough_desc_all(_adapter *padapter)
{
	u8 status = (check_tx_desc_resource(padapter, VI_QUEUE_INX) &&
			check_tx_desc_resource(padapter, VO_QUEUE_INX) &&
			check_tx_desc_resource(padapter, BE_QUEUE_INX) && 	
			check_tx_desc_resource(padapter, BK_QUEUE_INX) &&
			check_tx_desc_resource(padapter, MGT_QUEUE_INX) &&
			check_tx_desc_resource(padapter, HIGH_QUEUE_INX));	
	return status;
}

void rtl8192ee_xmitframe_resume(_adapter *padapter)
{
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct xmit_frame *pxmitframe = NULL;
	struct xmit_buf	*pxmitbuf = NULL;
	int res=_SUCCESS, xcnt = 0;

	RT_TRACE(_module_rtl871x_xmit_c_,_drv_info_,("rtl8192ee_xmitframe_resume()\n"));

	while(1)
	{
		if ((padapter->bDriverStopped == _TRUE)||(padapter->bSurpriseRemoved== _TRUE))
		{
			DBG_8192C("rtl8188ee_xmitframe_resume => bDriverStopped or bSurpriseRemoved \n");
			break;
		}
		
		if(!check_nic_enough_desc_all(padapter))
			break;
		
		pxmitbuf = rtw_alloc_xmitbuf(pxmitpriv);
		if(!pxmitbuf)
		{
			break;
		}

		pxmitframe =  rtw_dequeue_xframe(pxmitpriv, pxmitpriv->hwxmits, pxmitpriv->hwxmit_entry);
		
		if(pxmitframe)
		{
			pxmitframe->pxmitbuf = pxmitbuf;

			pxmitframe->buf_addr = pxmitbuf->pbuf;

			pxmitbuf->priv_data = pxmitframe;	

			if((pxmitframe->frame_tag&0x0f) == DATA_FRAMETAG)
			{	
				if(pxmitframe->attrib.priority<=15)//TID0~15
				{
					res = rtw_xmitframe_coalesce(padapter, pxmitframe->pkt, pxmitframe);
				}	
							
				rtw_os_xmit_complete(padapter, pxmitframe);//always return ndis_packet after rtw_xmitframe_coalesce 			
			}	

			RT_TRACE(_module_rtl871x_xmit_c_,_drv_info_,("rtl8192ee_xmitframe_resume(): rtw_dump_xframe\n"));

			if(res == _SUCCESS)
			{
				rtw_dump_xframe(padapter, pxmitframe);
			}
			else
			{
				rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
				rtw_free_xmitframe(pxmitpriv, pxmitframe);
			}

			xcnt++;
		}
		else
		{			
			rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
			break;
		}
	}
}

static u8 check_nic_enough_desc(_adapter *padapter, struct pkt_attrib *pattrib)
{
	u32 prio;
	struct xmit_priv	*pxmitpriv = &padapter->xmitpriv;
	struct rtw_tx_ring	*ring;

	switch(pattrib->qsel)
	{
		case 0:
		case 3:
			prio = BE_QUEUE_INX;
		 	break;
		case 1:
		case 2:
			prio = BK_QUEUE_INX;
			break;				
		case 4:
		case 5:
			prio = VI_QUEUE_INX;
			break;		
		case 6:
		case 7:
			prio = VO_QUEUE_INX;
			break;
		default:
			prio = BE_QUEUE_INX;
			break;
	}

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

static s32 xmitframe_direct(_adapter *padapter, struct xmit_frame *pxmitframe)
{
	s32 res = _SUCCESS;

	res = rtw_xmitframe_coalesce(padapter, pxmitframe->pkt, pxmitframe);
	if (res == _SUCCESS) {
		rtw_dump_xframe(padapter, pxmitframe);
	}

	return res;
}

/*
 * Return
 *	_TRUE	dump packet directly
 *	_FALSE	enqueue packet
 */
static s32 pre_xmitframe(_adapter *padapter, struct xmit_frame *pxmitframe)
{
        _irqL irqL;
	s32 res;
	struct xmit_buf *pxmitbuf = NULL;
	struct xmit_priv *pxmitpriv = &padapter->xmitpriv;
	struct pkt_attrib *pattrib = &pxmitframe->attrib;
	struct mlme_priv *pmlmepriv = &padapter->mlmepriv;

#ifdef CONFIG_CONCURRENT_MODE
	PADAPTER pbuddy_adapter = padapter->pbuddy_adapter;
	struct mlme_priv *pbuddy_mlmepriv = &(pbuddy_adapter->mlmepriv);	
#endif // CONFIG_CONCURRENT_MODE
	
	_enter_critical_bh(&pxmitpriv->lock, &irqL);

	if ( (rtw_txframes_sta_ac_pending(padapter, pattrib) > 0) ||
		(check_nic_enough_desc(padapter, pattrib) == _FALSE))
		goto enqueue;


	if (check_fwstate(pmlmepriv, _FW_UNDER_SURVEY) == _TRUE)
		goto enqueue;

#ifdef CONFIG_CONCURRENT_MODE
	if (check_fwstate(pbuddy_mlmepriv, _FW_UNDER_SURVEY|_FW_UNDER_LINKING) == _TRUE)
		goto enqueue;
#endif // CONFIG_CONCURRENT_MODE
	pxmitbuf = rtw_alloc_xmitbuf(pxmitpriv);
	if (pxmitbuf == NULL)
		goto enqueue;

	_exit_critical_bh(&pxmitpriv->lock, &irqL);

	pxmitframe->pxmitbuf = pxmitbuf;
	pxmitframe->buf_addr = pxmitbuf->pbuf;
	pxmitbuf->priv_data = pxmitframe;

	if (xmitframe_direct(padapter, pxmitframe) != _SUCCESS) {
		rtw_free_xmitbuf(pxmitpriv, pxmitbuf);
		rtw_free_xmitframe(pxmitpriv, pxmitframe);
	}

	return _TRUE;

enqueue:
	res = rtw_xmitframe_enqueue(padapter, pxmitframe);
	_exit_critical_bh(&pxmitpriv->lock, &irqL);

	if (res != _SUCCESS) {
		RT_TRACE(_module_xmit_osdep_c_, _drv_err_, ("pre_xmitframe: enqueue xmitframe fail\n"));
		rtw_free_xmitframe(pxmitpriv, pxmitframe);

		pxmitpriv->tx_drop++;
		return _TRUE;
	}

	return _FALSE;
}

s32 rtl8192ee_mgnt_xmit(_adapter *padapter, struct xmit_frame *pmgntframe)
{
	return rtw_dump_xframe(padapter, pmgntframe);
}

/*
 * Return
 *	_TRUE	dump packet directly ok
 *	_FALSE	temporary can't transmit packets to hardware
 */
s32	rtl8192ee_hal_xmit(_adapter *padapter, struct xmit_frame *pxmitframe)
{
	return pre_xmitframe(padapter, pxmitframe);
}

s32	rtl8192ee_hal_xmitframe_enqueue(_adapter *padapter, struct xmit_frame *pxmitframe)
{
	struct xmit_priv 	*pxmitpriv = &padapter->xmitpriv;
	s32 err;
	
	if ((err=rtw_xmitframe_enqueue(padapter, pxmitframe)) != _SUCCESS) 
	{
		rtw_free_xmitframe(pxmitpriv, pxmitframe);

		pxmitpriv->tx_drop++;					
	}
	else
	{
		if (check_nic_enough_desc(padapter, &pxmitframe->attrib) == _TRUE) {
#ifdef PLATFORM_LINUX
			tasklet_hi_schedule(&pxmitpriv->xmit_tasklet);
#endif
		}
	}

	return err;	
}

#ifdef  CONFIG_HOSTAPD_MLME

static void rtl8812ae_hostap_mgnt_xmit_cb(struct urb *urb)
{	
#ifdef PLATFORM_LINUX
	struct sk_buff *skb = (struct sk_buff *)urb->context;

	//DBG_8192C("%s\n", __FUNCTION__);

	dev_kfree_skb_any(skb);
#endif	
}

s32 rtl8192ee_hostap_mgnt_xmit_entry(_adapter *padapter, _pkt *pkt)
{
#ifdef PLATFORM_LINUX
	u16 fc;
	int rc, len, pipe;	
	unsigned int bmcst, tid, qsel;
	struct sk_buff *skb, *pxmit_skb;
	struct urb *urb;
	unsigned char *pxmitbuf;
	struct tx_desc *ptxdesc;
	struct rtw_ieee80211_hdr *tx_hdr;
	struct hostapd_priv *phostapdpriv = padapter->phostapdpriv;	
	struct net_device *pnetdev = padapter->pnetdev;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	struct dvobj_priv *pdvobj = adapter_to_dvobj(padapter);	

	
	//DBG_8192C("%s\n", __FUNCTION__);

	skb = pkt;
	
	len = skb->len;
	tx_hdr = (struct rtw_ieee80211_hdr *)(skb->data);
	fc = le16_to_cpu(tx_hdr->frame_ctl);
	bmcst = IS_MCAST(tx_hdr->addr1);

	if ((fc & RTW_IEEE80211_FCTL_FTYPE) != RTW_IEEE80211_FTYPE_MGMT)
		goto _exit;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)) // http://www.mail-archive.com/netdev@vger.kernel.org/msg17214.html
	pxmit_skb = dev_alloc_skb(len + TXDESC_SIZE);			
#else			
	pxmit_skb = netdev_alloc_skb(pnetdev, len + TXDESC_SIZE);
#endif		

	if(!pxmit_skb)
		goto _exit;

	pxmitbuf = pxmit_skb->data;

	urb = usb_alloc_urb(0, GFP_ATOMIC);
	if (!urb) {
		goto _exit;
	}

	// ----- fill tx desc -----	
	ptxdesc = (struct tx_desc *)pxmitbuf;	
	_rtw_memset(ptxdesc, 0, sizeof(*ptxdesc));
		
	//offset 0	
	ptxdesc->txdw0 |= cpu_to_le32(len&0x0000ffff); 
	ptxdesc->txdw0 |= cpu_to_le32(((TXDESC_SIZE+OFFSET_SZ)<<OFFSET_SHT)&0x00ff0000);//default = 32 bytes for TX Desc
	ptxdesc->txdw0 |= cpu_to_le32(OWN | FSG | LSG);

	if(bmcst)	
	{
		ptxdesc->txdw0 |= cpu_to_le32(BIT(24));
	}	

	//offset 4	
	ptxdesc->txdw1 |= cpu_to_le32(0x00);//MAC_ID

	ptxdesc->txdw1 |= cpu_to_le32((0x12<<QSEL_SHT)&0x00001f00);

	ptxdesc->txdw1 |= cpu_to_le32((0x06<< 16) & 0x000f0000);//b mode

	//offset 8			

	//offset 12		
	ptxdesc->txdw3 |= cpu_to_le32((le16_to_cpu(tx_hdr->seq_ctl)<<16)&0xffff0000);

	//offset 16		
	ptxdesc->txdw4 |= cpu_to_le32(BIT(8));//driver uses rate
		
	//offset 20

	rtl8188e_cal_txdesc_chksum(ptxdesc);
	// ----- end of fill tx desc -----

	//
	skb_put(pxmit_skb, len + TXDESC_SIZE);
	pxmitbuf = pxmitbuf + TXDESC_SIZE;
	_rtw_memcpy(pxmitbuf, skb->data, len);

	//DBG_8192C("mgnt_xmit, len=%x\n", pxmit_skb->len);


	// ----- prepare urb for submit -----
	
	//translate DMA FIFO addr to pipehandle
	//pipe = ffaddr2pipehdl(pdvobj, MGT_QUEUE_INX);
	pipe = usb_sndbulkpipe(pdvobj->pusbdev, pHalData->Queue2EPNum[(u8)MGT_QUEUE_INX]&0x0f);
	
	usb_fill_bulk_urb(urb, pdvobj->pusbdev, pipe,
			  pxmit_skb->data, pxmit_skb->len, rtl8188ee_hostap_mgnt_xmit_cb, pxmit_skb);
	
	urb->transfer_flags |= URB_ZERO_PACKET;
	usb_anchor_urb(urb, &phostapdpriv->anchored);
	rc = usb_submit_urb(urb, GFP_ATOMIC);
	if (rc < 0) {
		usb_unanchor_urb(urb);
		kfree_skb(skb);
	}
	usb_free_urb(urb);

	
_exit:	
	
	dev_kfree_skb_any(skb);

#endif

	return 0;

}
#endif

