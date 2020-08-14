/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/

#ifdef RTMP_MAC_USB

#include	"rt_config.h"

#ifdef USB_BULK_BUF_PREALLOC
void *RTMPQMemAddr(int size, int index, dma_addr_t *pDmaAddr, int type);
#else
void *RTMPQMemAddr(int size, ra_dma_addr_t *pDmaAddr, int type);
#endif /*USB_BULK_BUF_PREALLOC */
enum BLK_TYPE {
	BLK_TX0 = 0,
	BLK_TX1,
	BLK_TX2,
	BLK_TX3,
	BLK_TX4,
#ifdef USB_BULK_BUF_PREALLOC
	BLK_NULL = 5 * BUF_ALIGMENT_RINGSIZE,
#else
	BLK_NULL,
#endif /*USB_BULK_BUF_PREALLOC */
	BLK_PSPOLL,
	BLK_RX0,
	BLK_RX1,
	BLK_RX2,
	BLK_RX3,
	BLK_RX4,
	BLK_RX5,
	BLK_RX6,
	BLK_RX7,
	BLK_CMD,
	BLK_NUM, /* total number */
};

/* truth table of valid combinations (guarangeed by config.mk)
 * PREALLOC			YES	YES	NO	NO
 * HAS_OLD_PRE_ALLOC		y	n	n	n
 * HAS_USB_BULK_BUF_ALIGMENT	n	y	y	n
 * HAS_USB_BULK_BUF_PREALLOC	n	y	n	n
*/
static NDIS_STATUS RTMPAllocUsbBulkBufStruct(IN RTMP_ADAPTER * pAd,
					IN PURB * ppUrb,
					IN PVOID * ppXBuffer,
					IN INT bufLen,
					IN ra_dma_addr_t *pDmaAddr, IN PSTRING pBufName)
{
#if !defined(USB_BULK_BUF_PREALLOC)
	/* if any one of pre-allocation is used, skip allocating buffer. */
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
#endif /* any PRE ALLOC */

	if (!ppUrb || !ppXBuffer)
		return NDIS_STATUS_INVALID_DATA;

	*ppUrb = RTUSB_ALLOC_URB(0);
	if (*ppUrb == NULL) {
		DBGPRINT(RT_DEBUG_ERROR,
			("<-- ERROR in Alloc urb struct for %s !\n",
			pBufName));
		return NDIS_STATUS_RESOURCES;
	}
#if !defined(USB_BULK_BUF_PREALLOC)
	*ppXBuffer = RTUSB_URB_ALLOC_BUFFER(pObj->pUsb_Dev, bufLen, pDmaAddr);
	if (*ppXBuffer == NULL) {
		DBGPRINT(RT_DEBUG_ERROR,
			("<-- ERROR in Alloc Bulk buffer for %s!\n",
			pBufName));
		return NDIS_STATUS_RESOURCES;
	}
#else
	/* for any pre-alloc, let caller do mapping */
	*ppXBuffer = NULL;
#endif /* any PRE ALLOC */

	return NDIS_STATUS_SUCCESS;
}

static NDIS_STATUS RTMPFreeUsbBulkBufStruct(RTMP_ADAPTER *pAd,
					PURB *ppUrb,
					PUCHAR *ppXBuffer,
					INT bufLen,
					ra_dma_addr_t data_dma)
{
	if (ppUrb && *ppUrb) {
		RTUSB_UNLINK_URB(*ppUrb);
		RTUSB_FREE_URB(*ppUrb);
		*ppUrb = NULL;
	}

	/* if any one of pre-allocation is used, skip freeing buffer. */
	if (ppXBuffer && *ppXBuffer) {
#if !defined(USB_BULK_BUF_PREALLOC)
		POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

		if (pObj && pObj->pUsb_Dev)
			RTUSB_URB_FREE_BUFFER(pObj->pUsb_Dev, bufLen,
				*ppXBuffer, data_dma);

#endif /* !defined(USB_BULK_BUF_PREALLOC */
		/* reset to NULL no matter pre-alloc is used */
		*ppXBuffer = NULL;
	}

	return NDIS_STATUS_SUCCESS;
}

VOID RTMPResetTxRxRingMemory(IN RTMP_ADAPTER *pAd)
{
	UINT index, i, acidx;
	PTX_CONTEXT pNullContext = &pAd->NullContext;
	PTX_CONTEXT pPsPollContext = &pAd->PsPollContext;
	PCMD_RSP_CONTEXT pCmdRspEventContext = &pAd->CmdRspEventContext;
	unsigned long IrqFlags;

	/* Free TxSwQueue Packet */
	for (index = 0; index < NUM_OF_TX_RING; index++) {
		PQUEUE_ENTRY pEntry;
		PNDIS_PACKET pPacket;
		PQUEUE_HEADER pQueue;

		RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
		pQueue = &pAd->TxSwQueue[index];
		while (pQueue->Head) {
			pEntry = RemoveHeadQueue(pQueue);
			pPacket = QUEUE_ENTRY_TO_PACKET(pEntry);
			RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
		}
		RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);
	}

	/* unlink all urbs for the RECEIVE buffer queue. */
	for (i = 0; i < (RX_RING_SIZE); i++) {
		PRX_CONTEXT pRxContext = &(pAd->RxContext[i]);

		if (pRxContext->pUrb)
			RTUSB_UNLINK_URB(pRxContext->pUrb);
	}

	if (pCmdRspEventContext->pUrb)
		RTUSB_UNLINK_URB(pCmdRspEventContext->pUrb);

	/* unlink PsPoll urb resource */
	if (pPsPollContext && pPsPollContext->pUrb)
		RTUSB_UNLINK_URB(pPsPollContext->pUrb);

	/* Free NULL frame urb resource */
	if (pNullContext && pNullContext->pUrb)
		RTUSB_UNLINK_URB(pNullContext->pUrb);

	/* Free mgmt frame resource */
	for (i = 0; i < MGMT_RING_SIZE; i++) {
		PTX_CONTEXT pMLMEContext = (PTX_CONTEXT) pAd->MgmtRing.Cell[i].AllocVa;

		if (pMLMEContext) {
			if (NULL != pMLMEContext->pUrb) {
				RTUSB_UNLINK_URB(pMLMEContext->pUrb);
				RTUSB_FREE_URB(pMLMEContext->pUrb);
				pMLMEContext->pUrb = NULL;
			}
		}

		if (NULL != pAd->MgmtRing.Cell[i].pNdisPacket) {
			RELEASE_NDIS_PACKET(pAd, pAd->MgmtRing.Cell[i].pNdisPacket,
				NDIS_STATUS_FAILURE);
			pAd->MgmtRing.Cell[i].pNdisPacket = NULL;
			if (pMLMEContext)
				pMLMEContext->TransferBuffer = NULL;
		}

	}

	/* Free Tx frame resource */
	for (acidx = 0; acidx < NUM_OF_TX_RING /* 4 */; acidx++) {
		PHT_TX_CONTEXT pHTTXContext = &(pAd->TxContext[acidx]);
#ifdef USB_BULK_BUF_ALIGMENT
		INT ringidx;

		for (ringidx = 0; ringidx < BUF_ALIGMENT_RINGSIZE; ringidx++) {
			if (pHTTXContext->pUrb[ringidx])
				RTUSB_UNLINK_URB(pHTTXContext->pUrb[ringidx]);
		}
#else
		if (pHTTXContext->pUrb)
			RTUSB_UNLINK_URB(pHTTXContext->pUrb);
#endif /* USB_BULK_BUF_ALIGMENT */

	}

	for (i = 0; i < 6; i++)
		NdisFreeSpinLock(&pAd->BulkOutLock[i]);

	NdisFreeSpinLock(&pAd->BulkInLock);
#ifdef MULTI_URB
	NdisFreeSpinLock(&pAd->SubmitBulkInLock);
	NdisFreeSpinLock(&pAd->ReadBulkInLock);
#endif /* endif */
	NdisFreeSpinLock(&pAd->CmdRspLock);
	NdisFreeSpinLock(&pAd->MLMEBulkOutLock);

	NdisFreeSpinLock(&pAd->CmdQLock);
#ifdef RALINK_ATE
	NdisFreeSpinLock(&pAd->GenericLock);
#endif /* RALINK_ATE */
	/* Clear all pending bulk-out request flags. */
	RTUSB_CLEAR_BULK_FLAG(pAd, 0xffffffff);

	for (i = 0; i < NUM_OF_TX_RING; i++)
		NdisFreeSpinLock(&pAd->TxContextQueueLock[i]);

/*
	NdisFreeSpinLock(&pAd->MacTabLock);
	for(i=0; i<MAX_LEN_OF_BA_REC_TABLE; i++)
	{
		NdisFreeSpinLock(&pAd->BATable.BARecEntry[i].RxReRingLock);
	}
*/
}

/*
========================================================================
Routine Description:
	Calls USB_InterfaceStop and frees memory allocated for the URBs
    calls NdisMDeregisterDevice and frees the memory
    allocated in VNetInitialize for the Adapter Object

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
	None

Note:
========================================================================
*/
VOID RTMPFreeTxRxRingMemory(IN PRTMP_ADAPTER pAd)
{
	UINT i, acidx;
	PTX_CONTEXT pNullContext = &pAd->NullContext;
	PTX_CONTEXT pPsPollContext = &pAd->PsPollContext;
	PCMD_RSP_CONTEXT pCmdRspEventContext = &pAd->CmdRspEventContext;

	DBGPRINT(RT_DEBUG_TRACE, ("---> %s\n", __func__));

	/* Free all resources for the RECEIVE buffer queue. */
	for (i = 0; i < RX_RING_SIZE; ++i) {
		PRX_CONTEXT pRxContext = &pAd->RxContext[i];

		RTMPFreeUsbBulkBufStruct(pAd, &pRxContext->pUrb,
					(PUCHAR *) &pRxContext->TransferBuffer,
					MAX_RXBULK_SIZE, pRxContext->data_dma);
	}

	/* Command Response */
	RTMPFreeUsbBulkBufStruct(pAd, &pCmdRspEventContext->pUrb,
				(PUCHAR *) &pCmdRspEventContext->CmdRspBuffer,
				CMD_RSP_BULK_SIZE,
				pCmdRspEventContext->data_dma);

	/* Free PsPoll frame resource */
	RTMPFreeUsbBulkBufStruct(pAd, &pPsPollContext->pUrb,
				 (PUCHAR *) &pPsPollContext->TransferBuffer,
				 sizeof(TX_BUFFER), pPsPollContext->data_dma);

	/* Free NULL frame resource */
	RTMPFreeUsbBulkBufStruct(pAd, &pNullContext->pUrb,
				(PUCHAR *) &pNullContext->TransferBuffer,
				sizeof(TX_BUFFER), pNullContext->data_dma);

	/* Free mgmt frame resource */
	for (i = 0; i < MGMT_RING_SIZE; i++) {
		PTX_CONTEXT pMLMEContext = (PTX_CONTEXT) pAd->MgmtRing.Cell[i].AllocVa;

		if (pMLMEContext) {
			if (pMLMEContext->pUrb) {
				RTUSB_UNLINK_URB(pMLMEContext->pUrb);
				RTUSB_FREE_URB(pMLMEContext->pUrb);
				pMLMEContext->pUrb = NULL;
			}
		}

		if (pAd->MgmtRing.Cell[i].pNdisPacket) {
			RELEASE_NDIS_PACKET(pAd,
				pAd->MgmtRing.Cell[i].pNdisPacket,
				NDIS_STATUS_FAILURE);
			pAd->MgmtRing.Cell[i].pNdisPacket = NULL;
			if (pMLMEContext)
				pMLMEContext->TransferBuffer = NULL;
		}
	}
	if (pAd->MgmtDescRing.AllocVa) {
		os_free_mem(pAd, pAd->MgmtDescRing.AllocVa);
		pAd->MgmtDescRing.AllocVa = NULL;
	}

	/* Free Tx frame resource */
	for (acidx = 0; acidx < NUM_OF_TX_RING; acidx++) {
		PHT_TX_CONTEXT ht_tx = &pAd->TxContext[acidx];
#ifdef USB_BULK_BUF_ALIGMENT
		INT ringidx;

		for (ringidx = 0; ringidx < BUF_ALIGMENT_RINGSIZE; ++ringidx) {
			RTMPFreeUsbBulkBufStruct(pAd, &ht_tx->pUrb[ringidx],
						(PUCHAR *) &ht_tx->TransferBuffer[ringidx],
						sizeof(HTTX_BUFFER),
						ht_tx->data_dma[ringidx]);
		}
#else
		RTMPFreeUsbBulkBufStruct(pAd, &ht_tx->pUrb,
					(PUCHAR *) &ht_tx->TransferBuffer,
					sizeof(HTTX_BUFFER), ht_tx->data_dma);
#endif /* USB_BULK_BUF_ALIGMENT */
	}

	if (pAd->FragFrame.pFragPacket)
		RELEASE_NDIS_PACKET(pAd, pAd->FragFrame.pFragPacket, NDIS_STATUS_SUCCESS);

	DBGPRINT(RT_DEBUG_TRACE, ("<--- %s\n", __func__));
}

/*
========================================================================
Routine Description:
    Initialize receive data structures.

Arguments:
    pAd					Pointer to our adapter

Return Value:
	NDIS_STATUS_SUCCESS
	NDIS_STATUS_RESOURCES

Note:
	Initialize all receive releated private buffer, include those define
	in RTMP_ADAPTER structure and all private data structures. The major
	work is to allocate buffer for each packet and chain buffer to
	NDIS packet descriptor.
========================================================================
*/
static NDIS_STATUS NICInitRecv(RTMP_ADAPTER *pAd)
{
	UCHAR i;
	PCMD_RSP_CONTEXT pCmdRspEventContext = &pAd->CmdRspEventContext;

	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitRecv\n"));

	pAd->PendingRx = 0;
	pAd->NextRxBulkInReadIndex = 0;	/* Next Rx Read index */
	pAd->NextRxBulkInIndex = 0;	/*RX_RING_SIZE -1;  Rx Bulk pointer */
	pAd->NextRxBulkInPosition = 0;

	for (i = 0; i < (RX_RING_SIZE); i++) {
		PRX_CONTEXT pRxContext = &(pAd->RxContext[i]);

		ASSERT((pRxContext->TransferBuffer != NULL));
		ASSERT((pRxContext->pUrb != NULL));

		NdisZeroMemory(pRxContext->TransferBuffer, MAX_RXBULK_SIZE);

		pRxContext->pAd = pAd;
		pRxContext->pIrp = NULL;
		pRxContext->InUse = FALSE;
		pRxContext->IRPPending = FALSE;
		pRxContext->Readable = FALSE;
		pRxContext->bRxHandling = FALSE;
		pRxContext->BulkInOffset = 0;
	}

	pCmdRspEventContext->pAd = pAd;
	pCmdRspEventContext->InUse = FALSE;
	pCmdRspEventContext->Readable = FALSE;
	NdisZeroMemory(pCmdRspEventContext->CmdRspBuffer, CMD_RSP_BULK_SIZE);

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitRecv()\n"));

	return NDIS_STATUS_SUCCESS;
}

/*
========================================================================
Routine Description:
    Initialize transmit data structures.

Arguments:
    pAd					Pointer to our adapter

Return Value:
	NDIS_STATUS_SUCCESS
	NDIS_STATUS_RESOURCES

Note:
========================================================================
*/
static NDIS_STATUS NICInitTransmit(IN PRTMP_ADAPTER pAd)
{
	UCHAR i, acidx;
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	PTX_CONTEXT pNullContext = &(pAd->NullContext);
	PTX_CONTEXT pPsPollContext = &(pAd->PsPollContext);
	PTX_CONTEXT pMLMEContext = NULL;
	PVOID RingBaseVa;
	RTMP_MGMT_RING *pMgmtRing;
	PTX_BUFFER tx_buf;
	PURB pUrb;
	ra_dma_addr_t data_dma;

	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitTransmit\n"));

	/* Init 4 set of Tx + HCCA parameters */
	for (acidx = 0; acidx < NUM_OF_TX_RING; ++acidx) {
		/* Initialize all Transmit releated queues */
		InitializeQueueHeader(&pAd->TxSwQueue[acidx]);

		/* Next Local tx ring pointer waiting for buck out */
		pAd->NextBulkOutIndex[acidx] = acidx;
		pAd->BulkOutPending[acidx] = FALSE; /* Buck Out control flag */
	}

	/* TX_RING_SIZE, 4 ACs + HCCA */
	for (acidx = 0; acidx < NUM_OF_TX_RING; ++acidx) {
		PHT_TX_CONTEXT pHTTXContext = &pAd->TxContext[acidx];
		PHTTX_BUFFER ht_tx_buf;
#ifdef USB_BULK_BUF_ALIGMENT
		INT ringidx;

		for (ringidx = 0; ringidx < BUF_ALIGMENT_RINGSIZE; ringidx++) {
			ht_tx_buf = pHTTXContext->TransferBuffer[ringidx];
			pUrb = pHTTXContext->pUrb[ringidx];
			data_dma = pHTTXContext->data_dma[ringidx];

			ASSERT(ht_tx_buf);
			ASSERT(pUrb);
			/* GeK: [todo] not clear whole *pHTTXContext (except
			 * buf, urb, dma) as !defined(USB_BULK_BUF_ALIGMENT)?
			 */
			pHTTXContext->TransferBuffer[ringidx] = ht_tx_buf;
			pHTTXContext->pUrb[ringidx] = pUrb;
			pHTTXContext->data_dma[ringidx] = data_dma;
			NdisZeroMemory(ht_tx_buf->Aggregation,
				sizeof(ht_tx_buf->Aggregation));

		}
#else
		ht_tx_buf = pHTTXContext->TransferBuffer;
		pUrb = pHTTXContext->pUrb;
		data_dma = pHTTXContext->data_dma;
		ASSERT(ht_tx_buf);
		ASSERT(pUrb);

		NdisZeroMemory(pHTTXContext, sizeof(*pHTTXContext));
		pHTTXContext->TransferBuffer = ht_tx_buf;
		pHTTXContext->pUrb = pUrb;
		pHTTXContext->data_dma = data_dma;
		NdisZeroMemory(ht_tx_buf->Aggregation,
			sizeof(ht_tx_buf->Aggregation));
#endif /* USB_BULK_BUF_ALIGMENT */

		pHTTXContext->pAd = pAd;
		pHTTXContext->BulkOutPipeId = acidx;
		pHTTXContext->bRingEmpty = TRUE;
		pHTTXContext->bCopySavePad = FALSE;
	}

	/* MGMT_RING_SIZE */
	NdisZeroMemory(pAd->MgmtDescRing.AllocVa, pAd->MgmtDescRing.AllocSize);
	RingBaseVa = pAd->MgmtDescRing.AllocVa;

	/* Initialize MGMT Ring and associated buffer memory */
	pMgmtRing = &pAd->MgmtRing;
	for (i = 0; i < MGMT_RING_SIZE; i++) {
		/* link the pre-allocated Mgmt buffer to MgmtRing.Cell */
		pMgmtRing->Cell[i].AllocSize = sizeof(TX_CONTEXT);
		pMgmtRing->Cell[i].AllocVa = RingBaseVa;
		pMgmtRing->Cell[i].pNdisPacket = NULL;
		pMgmtRing->Cell[i].pNextNdisPacket = NULL;

		/*Allocate URB for MLMEContext */
		pMLMEContext = (PTX_CONTEXT) pAd->MgmtRing.Cell[i].AllocVa;
		pMLMEContext->pUrb = RTUSB_ALLOC_URB(0);
		if (pMLMEContext->pUrb == NULL) {
			DBGPRINT(RT_DEBUG_ERROR,
				("<-- fail to alloc urb for MLME[%d]\n", i));
			Status = NDIS_STATUS_RESOURCES;
			goto err;
		}
		pMLMEContext->pAd = pAd;
		pMLMEContext->SelfIdx = i;

		/* Offset to next ring descriptor address */
		RingBaseVa = (PUCHAR) RingBaseVa + sizeof(TX_CONTEXT);
	}
	DBGPRINT(RT_DEBUG_TRACE, ("MGMT Ring: total %d entry allocated\n", i));

	pAd->MgmtRing.TxSwFreeIdx = MGMT_RING_SIZE;
	pAd->MgmtRing.TxCpuIdx = 0;
	pAd->MgmtRing.TxDmaIdx = 0;

	/* NullContext */
	tx_buf = pNullContext->TransferBuffer;
	pUrb = pNullContext->pUrb;
	data_dma = pNullContext->data_dma;
	NdisZeroMemory(pNullContext, sizeof(*pNullContext));
	pNullContext->TransferBuffer = tx_buf;
	pNullContext->pUrb = pUrb;
	pNullContext->data_dma = data_dma;
	pNullContext->pAd = pAd;

	/* PsPollContext */
	tx_buf = pPsPollContext->TransferBuffer;
	pUrb = pPsPollContext->pUrb;
	data_dma = pPsPollContext->data_dma;
	NdisZeroMemory(pPsPollContext, sizeof(TX_CONTEXT));
	pPsPollContext->TransferBuffer = tx_buf;
	pPsPollContext->pUrb = pUrb;
	pPsPollContext->data_dma = data_dma;
	pPsPollContext->pAd = pAd;
	pPsPollContext->LastOne = TRUE;

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitTransmit(Status=%d)\n", Status));

	return Status;

	/* ERROR HANDLE */
err:
	/* MgmtDescRing.AllocVa is treated as non-null previously. Let
	 * RTMPFreeTxRxRingMemory() free MgmtDescRing.AllocVa buff.
	 */
	pMgmtRing = &pAd->MgmtRing;
	for (i = 0; i < MGMT_RING_SIZE; i++) {
		pMLMEContext = (PTX_CONTEXT) pAd->MgmtRing.Cell[i].AllocVa;
		if (!pMLMEContext)
			continue;
		/* Only free those allocated URBs in this function, so pass NULL
		 * and 0 to RTMPFreeUsbBulkBufStruct().
		 */
		RTMPFreeUsbBulkBufStruct(pAd, &pMLMEContext->pUrb, NULL, 0, 0);
	}

	return Status;
}

/*
========================================================================
Routine Description:
    Allocate DMA memory blocks for send, receive.

Arguments:
    pAd					Pointer to our adapter

Return Value:
	NDIS_STATUS_SUCCESS
	NDIS_STATUS_FAILURE
	NDIS_STATUS_RESOURCES

Note:
========================================================================
*/
NDIS_STATUS RTMPAllocTxRxRingMemory(IN PRTMP_ADAPTER pAd)
{
	NDIS_STATUS Status = NDIS_STATUS_FAILURE;
	PTX_CONTEXT pNullContext = &pAd->NullContext;
	PTX_CONTEXT pPsPollContext = &pAd->PsPollContext;
	PCMD_RSP_CONTEXT pCmdRspEventContext = &pAd->CmdRspEventContext;
	INT i, acidx;
#ifdef USB_BULK_BUF_ALIGMENT
	INT ringidx;
#endif /* USB_BULK_BUF_ALIGMENT */

	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __func__));

	do {
		/* Init send data structures and related parameters */
		/* TX_RING_SIZE, 4 ACs */
		for (acidx = 0; acidx < NUM_OF_TX_RING /*4 */ ; acidx++) {
			PHT_TX_CONTEXT pHTTXContext = &pAd->TxContext[acidx];

			NdisZeroMemory(pHTTXContext, sizeof(*pHTTXContext));
#ifdef USB_BULK_BUF_ALIGMENT
			/*Allocate URB and bulk buffer */
			for (ringidx = 0; ringidx < BUF_ALIGMENT_RINGSIZE; ringidx++) {
				DBGPRINT(RT_DEBUG_TRACE, ("allocate tx ringidx %d\n", ringidx));
				Status = RTMPAllocUsbBulkBufStruct(pAd,
								&pHTTXContext->pUrb[ringidx],
								(PVOID *) &
								pHTTXContext->TransferBuffer
								[ringidx], sizeof(HTTX_BUFFER),
								&pHTTXContext->data_dma[ringidx],
								"HTTxContext");

				if (Status != NDIS_STATUS_SUCCESS)
					goto err;
#ifdef USB_BULK_BUF_PREALLOC
				ASSERT(!pHTTXContext->TransferBuffer[ringidx]);
				pHTTXContext->TransferBuffer[ringidx] = RTMPQMemAddr(
					sizeof(HTTX_BUFFER),
					ringidx,
					&pHTTXContext->data_dma[ringidx],
					acidx + BLK_TX0);
				if (pHTTXContext->TransferBuffer[ringidx] == NULL)
					goto err;
#endif /* USB_BULK_BUF_PREALLOC */
			} /* end of ringidx loop */
#else /* !defined(USB_BULK_BUF_ALIGMENT) */
			/*Allocate URB and bulk buffer */
			Status = RTMPAllocUsbBulkBufStruct(pAd,
							   &pHTTXContext->pUrb,
							   (PVOID *) &pHTTXContext->TransferBuffer,
							   sizeof(HTTX_BUFFER),
							   &pHTTXContext->data_dma, "HTTxContext");
			if (Status != NDIS_STATUS_SUCCESS)
				goto err;
#endif /* USB_BULK_BUF_ALIGMENT */
		}

		/* MGMT_RING_SIZE */
		/* Allocate MGMT ring descriptor's memory */
		pAd->MgmtDescRing.AllocSize = MGMT_RING_SIZE * sizeof(TX_CONTEXT);
		os_alloc_mem_suspend(pAd, (PUCHAR *) (&pAd->MgmtDescRing.AllocVa),
			pAd->MgmtDescRing.AllocSize);
		if (pAd->MgmtDescRing.AllocVa == NULL) {
			DBGPRINT_ERR(("Failed to allocate a big buffer for MgmtDescRing!\n"));
			Status = NDIS_STATUS_RESOURCES;
			goto err;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("%s mgmt va 0x%p size %ld\n",
					__func__, pAd->MgmtDescRing.AllocVa,
					pAd->MgmtDescRing.AllocSize));

		/* NullContext */
		NdisZeroMemory(pNullContext, sizeof(*pNullContext));
		/*Allocate URB */
		Status = RTMPAllocUsbBulkBufStruct(pAd,
						&pNullContext->pUrb,
						(PVOID *) &pNullContext->TransferBuffer,
						sizeof(TX_BUFFER),
						&pNullContext->data_dma, "TxNullContext");
		if (Status != NDIS_STATUS_SUCCESS)
			goto err;

#ifdef USB_BULK_BUF_PREALLOC
		ASSERT(!pNullContext->TransferBuffer);
		pNullContext->TransferBuffer = RTMPQMemAddr(
			sizeof(TX_BUFFER),
			0,
			&pNullContext->data_dma,
			BLK_NULL);
		if (pNullContext->TransferBuffer == NULL)
			goto err;
#endif /* USB_BULK_BUF_PREALLOC */

		/* PsPollContext */
		NdisZeroMemory(pPsPollContext, sizeof(*pPsPollContext));
		/*Allocate URB */
		Status = RTMPAllocUsbBulkBufStruct(pAd,
						&pPsPollContext->pUrb,
						(PVOID *) &pPsPollContext->TransferBuffer,
						sizeof(TX_BUFFER),
						&pPsPollContext->data_dma, "TxPsPollContext");
		if (Status != NDIS_STATUS_SUCCESS)
			goto err;

#ifdef USB_BULK_BUF_PREALLOC
		ASSERT(!pPsPollContext->TransferBuffer);
		pPsPollContext->TransferBuffer = RTMPQMemAddr(
			sizeof(TX_BUFFER),
			0,
			&pPsPollContext->data_dma,
			BLK_PSPOLL);
		if (pPsPollContext->TransferBuffer == NULL)
			goto err;
#endif /* USB_BULK_BUF_PREALLOC */

		/* Init receive data structures and related parameters */
		for (i = 0; i < RX_RING_SIZE; i++) {
			PRX_CONTEXT pRxContext = &pAd->RxContext[i];

			/*Allocate URB */
			Status = RTMPAllocUsbBulkBufStruct(pAd,
							&pRxContext->pUrb,
							(PVOID *) &pRxContext->TransferBuffer,
							MAX_RXBULK_SIZE,
							&pRxContext->data_dma, "RxContext");
			if (Status != NDIS_STATUS_SUCCESS)
				goto err;
#ifdef USB_BULK_BUF_PREALLOC
			ASSERT(!pRxContext->TransferBuffer);
			pRxContext->TransferBuffer = RTMPQMemAddr(
				MAX_RXBULK_SIZE,
				0,
				&pRxContext->data_dma,
				i + BLK_RX0);
			if (pRxContext->TransferBuffer == NULL)
				goto err;
#endif /* USB_BULK_BUF_PREALLOC */
		}

		/* Init command response event related parameters */
		Status = RTMPAllocUsbBulkBufStruct(pAd,
						&pCmdRspEventContext->pUrb,
						(PVOID *) &pCmdRspEventContext->CmdRspBuffer,
						CMD_RSP_BULK_SIZE,
						&pCmdRspEventContext->data_dma,
						"CmdRspEventContext");

		if (Status != NDIS_STATUS_SUCCESS)
			goto err;

#ifdef USB_BULK_BUF_PREALLOC
		ASSERT(!pCmdRspEventContext->CmdRspBuffer);
		pCmdRspEventContext->CmdRspBuffer = RTMPQMemAddr(
			CMD_RSP_BULK_SIZE,
			0,
			&pCmdRspEventContext->data_dma,
			BLK_CMD);
		if (!pCmdRspEventContext->CmdRspBuffer)
			goto err;
#endif /* USB_BULK_BUF_PREALLOC */

		NdisZeroMemory(&pAd->FragFrame, sizeof(pAd->FragFrame));
		pAd->FragFrame.pFragPacket = RTMP_AllocateFragPacketBuffer(pAd, RX_BUFFER_NORMSIZE);

		if (pAd->FragFrame.pFragPacket == NULL)
			Status = NDIS_STATUS_RESOURCES;

	} while (FALSE);

	DBGPRINT(RT_DEBUG_TRACE, ("<-- %s, Status=%x\n", __func__, Status));
	return Status;

err:
	Status = NDIS_STATUS_RESOURCES;
	RTMPFreeTxRxRingMemory(pAd);
	DBGPRINT(RT_DEBUG_TRACE, ("<-- %s, Status=%x\n", __func__, Status));
	return Status;
}

NDIS_STATUS RTMPInitTxRxRingMemory(IN RTMP_ADAPTER *pAd)
{
	INT num;
	NDIS_STATUS Status;

	/* Init the CmdQ and CmdQLock */
	NdisAllocateSpinLock(pAd, &pAd->CmdQLock);
	NdisAcquireSpinLock(&pAd->CmdQLock);
	RTInitializeCmdQ(&pAd->CmdQ);
	NdisReleaseSpinLock(&pAd->CmdQLock);

	NdisAllocateSpinLock(pAd, &pAd->MLMEBulkOutLock);
	NdisAllocateSpinLock(pAd, &pAd->BulkInLock);
#ifdef MULTI_URB
	NdisAllocateSpinLock(pAd, &pAd->SubmitBulkInLock);
	NdisAllocateSpinLock(pAd, &pAd->ReadBulkInLock);
#endif /* endif */
	NdisAllocateSpinLock(pAd, &pAd->CmdRspLock);
	for (num = 0; num < 6; num++)
		NdisAllocateSpinLock(pAd, &pAd->BulkOutLock[num]);

	for (num = 0; num < NUM_OF_TX_RING; num++)
		NdisAllocateSpinLock(pAd, &pAd->TxContextQueueLock[num]);

#ifdef RALINK_ATE
	NdisAllocateSpinLock(pAd, &pAd->GenericLock);
#endif /* RALINK_ATE */

	NICInitRecv(pAd);

	Status = NICInitTransmit(pAd);

#ifdef RXPKT_THREAD
	/* Init RX Paket Q and Rx Packet Lock */
	InitializeQueueHeader(&pAd->rxPktQ);
	NdisAllocateSpinLock(pAd, &pAd->rxPktQLock);
#endif /* RXPKT_THREAD */
	return Status;

}

/*
========================================================================
Routine Description:
    Write WLAN MAC address to USB 2870.

Arguments:
    pAd					Pointer to our adapter

Return Value:
	NDIS_STATUS_SUCCESS

Note:
========================================================================
*/
NDIS_STATUS RTUSBWriteHWMACAddress(RTMP_ADAPTER *pAd)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	LARGE_INTEGER NOW;

	/* initialize the random number generator */
	RTMP_GetCurrentSystemTime(&NOW);

	/* Write New MAC address to MAC_CSR2 & MAC_CSR3 & let ASIC know our new MAC */
	AsicSetDevMac(pAd, pAd->CurrentAddress);

	return Status;
}

/*
========================================================================
Routine Description:
    Disable DMA.

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
	None

Note:
========================================================================
*/
VOID RT28XXDMADisable(IN RTMP_ADAPTER *pAd)
{
	/* no use */
}

/*
========================================================================
Routine Description:
    Enable DMA.

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
	None

Note:
========================================================================
*/
VOID RT28XXDMAEnable(RTMP_ADAPTER *pAd)
{
	USB_DMA_CFG_STRUC UsbCfg;

#ifdef RTMP_MAC
	if (pAd->chipCap.hif_type == HIF_RTMP) {
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x4);

		if (AsicWaitPDMAIdle(pAd, 200, 1000) == FALSE) {
			if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))
				return;
		}

		RtmpusecDelay(50);

		WPDMA_GLO_CFG_STRUC GloCfg;

		RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);
		GloCfg.field.EnTXWriteBackDDONE = 1;
		GloCfg.field.EnableRxDMA = 1;
		GloCfg.field.EnableTxDMA = 1;
		RTMP_IO_WRITE32(pAd, WPDMA_GLO_CFG, GloCfg.word);
		DBGPRINT(RT_DEBUG_TRACE, ("<== WRITE DMA offset 0x208 = 0x%x\n", GloCfg.word));

		UsbCfg.word = 0;
		UsbCfg.field.phyclear = 0;
		/* usb version is 1.1,do not use bulk in aggregation */
		if ((pAd->BulkInMaxPacketSize >= 512) && (pAd->usb_ctl.usb_aggregation))
			UsbCfg.field.RxBulkAggEn = 1;
		else
			UsbCfg.field.RxBulkAggEn = 0;
		/* for last packet, PBF might use more than limited, so minus 2 to prevent from error */
		UsbCfg.field.RxBulkAggLmt = (MAX_RXBULK_SIZE / 1024) - 3;
		UsbCfg.field.RxBulkAggTOut = 0x80;	/* 2006-10-18 */
		UsbCfg.field.RxBulkEn = 1;
		UsbCfg.field.TxBulkEn = 1;
	}
#endif /* RTMP_MAC */

#if defined(MT76x0) || defined(MT76x2)
	if (IS_MT76x0(pAd) || IS_MT76x2(pAd)) {
		USB_CFG_READ(pAd, &UsbCfg.word);

		/* USB1.1 do not use bulk in aggregation */
		if ((pAd->BulkInMaxPacketSize >= 512) && (pAd->usb_ctl.usb_aggregation))
			UsbCfg.field_76xx.RxBulkAggEn = 1;
		else {
			DBGPRINT(RT_DEBUG_OFF, ("disable usb rx aggregagion\n"));
			UsbCfg.field_76xx.RxBulkAggEn = 0;
		}

		/* for last packet, PBF might use more than limited, so minus 2 to prevent from error */
		UsbCfg.field_76xx.RxBulkAggLmt = (MAX_RXBULK_SIZE / 1024) - 3;
		UsbCfg.field_76xx.RxBulkAggTOut = 0x80;

		UsbCfg.field_76xx.RxBulkEn = 1;
		UsbCfg.field_76xx.TxBulkEn = 1;

		if (IS_MT76x2(pAd))
			UsbCfg.field_76xx.RX_DROP_OR_PADDING = 1;
	}
#endif /* defined(MT76x0) || defined(MT76x2) */

	USB_CFG_WRITE(pAd, UsbCfg.word);
}

/********************************************************************
  *
  *	2870 Beacon Update Related functions.
  *
  ********************************************************************/

/*
========================================================================
Routine Description:
    Write Beacon buffer to Asic.

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
	None

Note:
========================================================================
*/
VOID RT28xx_UpdateBeaconToAsic(IN RTMP_ADAPTER *pAd,
			IN INT apidx, IN ULONG FrameLen, IN ULONG UpdatePos)
{
	PUCHAR pBeaconFrame = NULL;
	UCHAR *ptr;
	UINT i, padding;
	BEACON_SYNC_STRUCT *pBeaconSync = pAd->CommonCfg.pBeaconSync;
	UINT32 longValue;
/*	USHORT			shortValue;*/
	BOOLEAN bBcnReq = FALSE;
	UCHAR bcn_idx = 0;
	UINT8 TXWISize = pAd->chipCap.TXWISize;

#ifdef CONFIG_AP_SUPPORT
	if ((apidx < pAd->ApCfg.BssidNum) && (apidx < MAX_MBSSID_NUM(pAd))) {
		bcn_idx = pAd->ApCfg.MBSSID[apidx].BcnBufIdx;
		pBeaconFrame = (PUCHAR) pAd->ApCfg.MBSSID[apidx].BeaconBuf;
		bBcnReq = BeaconTransmitRequired(pAd, apidx, &pAd->ApCfg.MBSSID[apidx]);
	}
#endif /* CONFIG_AP_SUPPORT */

	if (pBeaconFrame == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("pBeaconFrame is NULL!\n"));
		return;
	}

	if (pBeaconSync == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("pBeaconSync is NULL!\n"));
		return;
	}

	if (bBcnReq == FALSE) {
		/* when the ra interface is down, do not send its beacon frame */
		/* clear all zero */
		for (i = 0; i < TXWISize; i += 4)
			RTMP_CHIP_UPDATE_BEACON(pAd, pAd->BeaconOffset[bcn_idx] + i, 0x00, 4);

		pBeaconSync->BeaconBitMap &= (~(BEACON_BITMAP_MASK & (1 << bcn_idx)));
		NdisZeroMemory(pBeaconSync->BeaconTxWI[bcn_idx], TXWISize);
	} else {
		ptr = (PUCHAR) &pAd->BeaconTxWI;
#ifdef RT_BIG_ENDIAN
		RTMPWIEndianChange(pAd, ptr, TYPE_TXWI);
#endif /* endif */
		/* If BeaconTxWI changed, we need to rewrite the TxWI for the Beacon frames. */
		if (NdisEqualMemory(pBeaconSync->BeaconTxWI[bcn_idx], &pAd->BeaconTxWI, TXWISize)
			== FALSE) {
			pBeaconSync->BeaconBitMap &= (~(BEACON_BITMAP_MASK & (1 << bcn_idx)));
			NdisMoveMemory(pBeaconSync->BeaconTxWI[bcn_idx], &pAd->BeaconTxWI,
				TXWISize);
		}

		if ((pBeaconSync->BeaconBitMap & (1 << bcn_idx)) != (1 << bcn_idx)) {
			for (i = 0; i < TXWISize; i += 4) {
				longValue =
					*ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16) +
					(*(ptr + 3) << 24);
				RTMP_CHIP_UPDATE_BEACON(pAd, pAd->BeaconOffset[bcn_idx] + i,
					longValue, 4);
				ptr += 4;
			}
		}

		ptr = pBeaconSync->BeaconBuf[bcn_idx];
		padding = (FrameLen & 0x01);
		NdisZeroMemory((PUCHAR) (pBeaconFrame + FrameLen), padding);
		FrameLen += padding;
		for (i = 0; i < FrameLen /*HW_BEACON_OFFSET */; i += 2) {
			if (NdisEqualMemory(ptr, pBeaconFrame, 2) == FALSE) {
				NdisMoveMemory(ptr, pBeaconFrame, 2);
				longValue = *ptr + (*(ptr + 1) << 8);
				RTMP_CHIP_UPDATE_BEACON(pAd,
					pAd->BeaconOffset[bcn_idx] + TXWISize + i,
					longValue, 2);
			}
			ptr += 2;
			pBeaconFrame += 2;
		}

		pBeaconSync->BeaconBitMap |= (1 << bcn_idx);

		/* For AP interface, set the DtimBitOn so that we can send
		*  Bcast/Mcast frame out after this beacon frame.
		*/
#ifdef CONFIG_AP_SUPPORT
		{
			ptr =
			    (PUCHAR) (pAd->ApCfg.MBSSID[apidx].BeaconBuf +
				      pAd->ApCfg.MBSSID[apidx].TimIELocationInBeacon);
			if ((*(ptr + 4)) & 0x01)
				pBeaconSync->DtimBitOn |= (1 << apidx);
			else
				pBeaconSync->DtimBitOn &= ~(1 << apidx);
		}
#endif /* CONFIG_AP_SUPPORT */
	}

}

VOID RTUSBBssBeaconStop(IN RTMP_ADAPTER *pAd)
{
	BEACON_SYNC_STRUCT *pBeaconSync;
	int i, offset;
	BOOLEAN Cancelled = TRUE;
	UINT8 TXWISize = pAd->chipCap.TXWISize;

	pBeaconSync = pAd->CommonCfg.pBeaconSync;
	if (pBeaconSync && pBeaconSync->EnableBeacon) {
		INT NumOfBcn = 0;

#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
			NumOfBcn = pAd->ApCfg.BssidNum + MAX_MESH_NUM;
		}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
			NumOfBcn = MAX_MESH_NUM;
		}
#endif /* CONFIG_STA_SUPPORT */

		RTMPCancelTimer(&pAd->CommonCfg.BeaconUpdateTimer, &Cancelled);

		for (i = 0; i < NumOfBcn; i++) {
			NdisZeroMemory(pBeaconSync->BeaconBuf[i], HW_BEACON_OFFSET);
			NdisZeroMemory(pBeaconSync->BeaconTxWI[i], TXWISize);

			for (offset = 0; offset < HW_BEACON_OFFSET; offset += 4)
				RTMP_CHIP_UPDATE_BEACON(pAd, pAd->BeaconOffset[i] + offset, 0x00,
					4);

			pBeaconSync->CapabilityInfoLocationInBeacon[i] = 0;
			pBeaconSync->TimIELocationInBeacon[i] = 0;
		}
		pBeaconSync->BeaconBitMap = 0;
		pBeaconSync->DtimBitOn = 0;
	}
}

VOID RTUSBBssBeaconStart(IN RTMP_ADAPTER *pAd)
{
	int apidx;
	BEACON_SYNC_STRUCT *pBeaconSync;
	UINT8 TXWISize = pAd->chipCap.TXWISize;
/*	LARGE_INTEGER	tsfTime, deltaTime;*/

	pBeaconSync = pAd->CommonCfg.pBeaconSync;
	if (pBeaconSync && pBeaconSync->EnableBeacon) {
		INT NumOfBcn = 0;

#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
			NumOfBcn = pAd->ApCfg.BssidNum + MAX_MESH_NUM;
		}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
			NumOfBcn = MAX_MESH_NUM;
		}
#endif /* CONFIG_STA_SUPPORT */

		for (apidx = 0; apidx < NumOfBcn; apidx++) {
			UCHAR CapabilityInfoLocationInBeacon = 0;
			UCHAR TimIELocationInBeacon = 0;
#ifdef CONFIG_AP_SUPPORT
			IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
				{
					CapabilityInfoLocationInBeacon =
						pAd->ApCfg.MBSSID[apidx].
						CapabilityInfoLocationInBeacon;
					TimIELocationInBeacon =
						pAd->ApCfg.MBSSID[apidx].TimIELocationInBeacon;
				}
			}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */

			NdisZeroMemory(pBeaconSync->BeaconBuf[apidx], HW_BEACON_OFFSET);
			pBeaconSync->CapabilityInfoLocationInBeacon[apidx] =
				CapabilityInfoLocationInBeacon;
			pBeaconSync->TimIELocationInBeacon[apidx] = TimIELocationInBeacon;
			NdisZeroMemory(pBeaconSync->BeaconTxWI[apidx], TXWISize);
		}
		pBeaconSync->BeaconBitMap = 0;
		pBeaconSync->DtimBitOn = 0;
		pAd->CommonCfg.BeaconUpdateTimer.Repeat = TRUE;

		pAd->CommonCfg.BeaconAdjust = 0;
		pAd->CommonCfg.BeaconFactor = 0xffffffff / (pAd->CommonCfg.BeaconPeriod << 10);
		pAd->CommonCfg.BeaconRemain =
			(0xffffffff % (pAd->CommonCfg.BeaconPeriod << 10)) + 1;
		DBGPRINT(RT_DEBUG_TRACE,
			 ("RTUSBBssBeaconStart:BeaconFactor=%d, BeaconRemain=%d!\n",
			  pAd->CommonCfg.BeaconFactor, pAd->CommonCfg.BeaconRemain));
		/* Fix BeaconUpdateTimer not started properly (because timer was not initialized */
		RTMPInitTimer(pAd, &pAd->CommonCfg.BeaconUpdateTimer,
			GET_TIMER_FUNCTION(BeaconUpdateExec), pAd, TRUE);
		RTMPSetTimer(&pAd->CommonCfg.BeaconUpdateTimer,
			10 /*pAd->CommonCfg.BeaconPeriod */);

	}
}

VOID RTUSBBssBeaconInit(IN RTMP_ADAPTER *pAd)
{
	BEACON_SYNC_STRUCT *pBeaconSync;
	int i, j;
	UINT8 TXWISize = pAd->chipCap.TXWISize;

	os_alloc_mem(pAd, (PUCHAR *) (&pAd->CommonCfg.pBeaconSync), sizeof(BEACON_SYNC_STRUCT));

	if (pAd->CommonCfg.pBeaconSync) {
		pBeaconSync = pAd->CommonCfg.pBeaconSync;
		NdisZeroMemory(pBeaconSync, sizeof(BEACON_SYNC_STRUCT));
		for (i = 0; i < HW_BEACON_MAX_COUNT(pAd); i++) {
			NdisZeroMemory(pBeaconSync->BeaconBuf[i], HW_BEACON_OFFSET);
			pBeaconSync->CapabilityInfoLocationInBeacon[i] = 0;
			pBeaconSync->TimIELocationInBeacon[i] = 0;
			os_alloc_mem(pAd, &pBeaconSync->BeaconTxWI[i], TXWISize);
			if (pBeaconSync->BeaconTxWI[i])
				NdisZeroMemory(pBeaconSync->BeaconTxWI[i], TXWISize);
			else
				goto error2;
		}
		pBeaconSync->BeaconBitMap = 0;

		/* RTMPInitTimer(pAd, &pAd->CommonCfg.BeaconUpdateTimer,
		*  GET_TIMER_FUNCTION(BeaconUpdateExec), pAd, TRUE);
		*/
		pBeaconSync->EnableBeacon = TRUE;
	} else
		goto error1;

	return;

error2:
	for (j = 0; j < i; j++)
		os_free_mem(pAd, pBeaconSync->BeaconTxWI[j]);

	os_free_mem(pAd, pAd->CommonCfg.pBeaconSync);

error1:
	DBGPRINT(RT_DEBUG_ERROR, ("memory are not available\n"));
}

VOID RTUSBBssBeaconExit(IN RTMP_ADAPTER *pAd)
{
	BEACON_SYNC_STRUCT *pBeaconSync;
	BOOLEAN Cancelled = TRUE;
	int i;

	if (pAd->CommonCfg.pBeaconSync) {
		pBeaconSync = pAd->CommonCfg.pBeaconSync;
		pBeaconSync->EnableBeacon = FALSE;
		RTMPCancelTimer(&pAd->CommonCfg.BeaconUpdateTimer, &Cancelled);
		pBeaconSync->BeaconBitMap = 0;

		for (i = 0; i < HW_BEACON_MAX_COUNT(pAd); i++) {
			NdisZeroMemory(pBeaconSync->BeaconBuf[i], HW_BEACON_OFFSET);
			pBeaconSync->CapabilityInfoLocationInBeacon[i] = 0;
			pBeaconSync->TimIELocationInBeacon[i] = 0;
			os_free_mem(pAd, pBeaconSync->BeaconTxWI[i]);
		}

		os_free_mem(pAd, pAd->CommonCfg.pBeaconSync);
		pAd->CommonCfg.pBeaconSync = NULL;
	}
}

/*
    ========================================================================
    Routine Description:
	For device work as AP mode but didn't have TBTT interrupt event, we need a mechanism
	to update the beacon context in each Beacon interval. Here we use a periodical timer
	to simulate the TBTT interrupt to handle the beacon context update.

    Arguments:
	SystemSpecific1         - Not used.
	FunctionContext         - Pointer to our Adapter context.
	SystemSpecific2         - Not used.
	SystemSpecific3         - Not used.

    Return Value:
	None

    ========================================================================
*/
VOID BeaconUpdateExec(IN PVOID SystemSpecific1,
		      IN PVOID FunctionContext, IN PVOID SystemSpecific2, IN PVOID SystemSpecific3)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) FunctionContext;
	LARGE_INTEGER tsfTime_a;	/*, tsfTime_b, deltaTime_exp, deltaTime_ab; */
	UINT32 delta, delta2MS, period2US, remain, remain_low, remain_high;

	if (pAd->CommonCfg.IsUpdateBeacon == TRUE) {
		ReSyncBeaconTime(pAd);

#ifdef CONFIG_AP_SUPPORT
#ifdef RT_CFG80211_P2P_SUPPORT
		if (pAd->cfg80211_ctrl.isCfgInApMode == RT_CMD_80211_IFTYPE_AP
#ifdef RT_CFG80211_AP_SUPPORT
		    || CFG_AP_ON(pAd)
#endif /* RT_CFG80211_AP_SUPPORT */
		    )
#else
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
#endif /* RT_CFG80211_P2P_SUPPORT */
		{
#ifndef RT_CFG80211_SUPPORT
			BEACON_SYNC_STRUCT *pBeaconSync = pAd->CommonCfg.pBeaconSync;
#endif
			ULONG UpTime;
			INT i = 0;

			/* update channel utilization */
			NdisGetSystemUpTime(&UpTime);

#ifdef AP_QLOAD_SUPPORT
			QBSS_LoadUpdate(pAd, UpTime);
#endif /* AP_QLOAD_SUPPORT */

			for (i = 0; i < pAd->ApCfg.BssidNum; ++i) {
#ifdef RT_CFG80211_SUPPORT
				if (pAd->ApCfg.DtimCount[i] == 0) {
#else
				if (pAd->ApCfg.DtimCount[i] == 0 && pBeaconSync->DtimBitOn) {
#endif
					POS_COOKIE pObj;

					pObj = (POS_COOKIE) pAd->OS_Cookie;
					RTMP_OS_TASKLET_SCHE(&pObj->tbtt_task);
				}
			}
#ifdef RT_CFG80211_SUPPORT
			if (CFG_P2PGO_ON(pAd)) {
				PNET_DEV pNetDev =
					RTMP_CFG80211_FindVifEntry_ByType(pAd,
						RT_CMD_80211_IFTYPE_P2P_GO);
				if (pNetDev != NULL)
					RT_CFG80211_BEACON_TIM_UPDATE(pAd,
						pNetDev);
			}
#ifdef RT_CFG80211_AP_SUPPORT
			if (CFG_AP_ON(pAd)) {
				PNET_DEV pNetDev =
					RTMP_CFG80211_FindVifEntry_ByType(pAd,
						RT_CMD_80211_IFTYPE_AP);

				if (pNetDev != NULL)
					RT_CFG80211_BEACON_TIM_UPDATE(pAd,
						pNetDev);
			}
#endif /* RT_CFG80211_AP_SUPPORT */
#else
			APUpdateAllBeaconFrame(pAd);
#endif /* RT_CFG80211_SUPPORT  */
		}
#endif /* CONFIG_AP_SUPPORT */

	}

	RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &tsfTime_a.u.LowPart);
	RTMP_IO_READ32(pAd, TSF_TIMER_DW1, &tsfTime_a.u.HighPart);

	/*
	   Calculate next beacon time to wake up to update.

	   BeaconRemain = (0xffffffff % (pAd->CommonCfg.BeaconPeriod << 10)) + 1;

	   Background: Timestamp (us) % Beacon Period (us) shall be 0 at TBTT

	   Formula:     (a+b) mod m = ((a mod m) + (b mod m)) mod m
	   (a*b) mod m = ((a mod m) * (b mod m)) mod m

	   ==> ((HighPart * 0xFFFFFFFF) + LowPart) mod Beacon_Period
	   ==> (((HighPart * 0xFFFFFFFF) mod Beacon_Period) +
	   (LowPart mod (Beacon_Period))) mod Beacon_Period
	   ==> ((HighPart mod Beacon_Period) * (0xFFFFFFFF mod Beacon_Period)) mod
	   Beacon_Period

	   Steps:
	   1. Calculate the delta time between now and next TBTT;

	   delta time = (Beacon Period) - ((64-bit timestamp) % (Beacon Period))

	   (1) If no overflow for LowPart, 32-bit, we can calcualte the delta
	   time by using LowPart;

	   delta time = LowPart % (Beacon Period)

	   (2) If overflow for LowPart, we need to care about HighPart value;

	   delta time = (BeaconRemain * HighPart + LowPart) % (Beacon Period)

	   Ex: if the maximum value is 0x00 0xFF (255), Beacon Period = 100,
	   TBTT timestamp will be 100, 200, 300, 400, ...
	   when TBTT timestamp is 300 = 1*56 + 44, means HighPart = 1,
	   Low Part = 44

	   2. Adjust next update time of the timer to (delta time + 10ms).
	 */

	/*positive=getDeltaTime(tsfTime_a, expectedTime, &deltaTime_exp); */
	period2US = (pAd->CommonCfg.BeaconPeriod << 10);
	remain_high = pAd->CommonCfg.BeaconRemain * tsfTime_a.u.HighPart;
	remain_low = tsfTime_a.u.LowPart % (pAd->CommonCfg.BeaconPeriod << 10);
	remain = (remain_high + remain_low) % (pAd->CommonCfg.BeaconPeriod << 10);
	delta = (pAd->CommonCfg.BeaconPeriod << 10) - remain;

	delta2MS = (delta >> 10);
	if (delta2MS > 150) {
		pAd->CommonCfg.BeaconUpdateTimer.TimerValue = 100;
		pAd->CommonCfg.IsUpdateBeacon = FALSE;
	} else {
		pAd->CommonCfg.BeaconUpdateTimer.TimerValue = delta2MS + 10;
		pAd->CommonCfg.IsUpdateBeacon = TRUE;
	}
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		if ((pAd->CommonCfg.Channel > 14)
		    && (pAd->CommonCfg.bIEEE80211H == 1)
		    && (pAd->Dot11_H.RDMode == RD_SWITCHING_MODE)) {
			ChannelSwitchingCountDownProc(pAd);
		}
	}
#endif /* CONFIG_AP_SUPPORT */
}

/********************************************************************
  *
  *	2870 Radio on/off Related functions.
  *
  ********************************************************************/
VOID RT28xxUsbMlmeRadioOn(IN PRTMP_ADAPTER pAd)
{

	DBGPRINT(RT_DEBUG_TRACE, ("RT28xxUsbMlmeRadioOn()\n"));

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF))
		return;

	ASIC_RADIO_ON(pAd, MLME_RADIO_ON);

	/* Clear Radio off flag */
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	    APStartUp(pAd);
#endif /* CONFIG_AP_SUPPORT */

#ifdef LED_CONTROL_SUPPORT
	/* Set LED */
#ifdef CONFIG_STA_SUPPORT
	RTMPSetLED(pAd, LED_RADIO_ON);
#endif /* CONFIG_STA_SUPPORT */
#ifdef CONFIG_AP_SUPPORT
	RTMPSetLED(pAd, LED_LINK_UP);
#endif /* CONFIG_AP_SUPPORT */
#endif /* LED_CONTROL_SUPPORT */

}

VOID RT28xxUsbMlmeRadioOFF(IN PRTMP_ADAPTER pAd)
{
#ifdef WSC_INCLUDED
#ifdef WSC_LED_SUPPORT
	UINT WPSLedMode10;
#endif /* WSC_LED_SUPPORT */
#endif /* WSC_INCLUDED */

	DBGPRINT(RT_DEBUG_TRACE, ("RT28xxUsbMlmeRadioOFF()\n"));

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF))
		return;

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);

#ifdef WSC_INCLUDED
#ifdef WSC_LED_SUPPORT
	if (LED_MODE(pAd) == WPS_LED_MODE_10) {
		WPSLedMode10 = LINK_STATUS_WPS_MODE10_TURN_OFF;
		RTEnqueueInternalCmd(pAd, CMDTHREAD_LED_WPS_MODE10, &WPSLedMode10,
			sizeof(WPSLedMode10));
	}
#endif /* WSC_LED_SUPPORT */
#endif /* WSC_INCLUDED */

#ifdef CONFIG_STA_SUPPORT
	/* Clear PMKID cache. */
	pAd->StaCfg.SavedPMKNum = 0;
	RTMPZeroMemory(pAd->StaCfg.SavedPMK, sizeof(pAd->StaCfg.SavedPMK));

	/* Link down first if any association exists */
	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)) {
		if (INFRA_ON(pAd) || ADHOC_ON(pAd)) {
			MLME_DISASSOC_REQ_STRUCT DisReq;
			MLME_QUEUE_ELEM *pMsgElem;

			os_alloc_mem(pAd, (UCHAR **) &pMsgElem, sizeof(MLME_QUEUE_ELEM));
			if (pMsgElem) {
				COPY_MAC_ADDR(&DisReq.Addr, pAd->CommonCfg.Bssid);
				DisReq.Reason = REASON_DISASSOC_STA_LEAVING;

				pMsgElem->Machine = ASSOC_STATE_MACHINE;
				pMsgElem->MsgType = MT2_MLME_DISASSOC_REQ;
				pMsgElem->MsgLen = sizeof(MLME_DISASSOC_REQ_STRUCT);
				NdisMoveMemory(pMsgElem->Msg, &DisReq,
					sizeof(MLME_DISASSOC_REQ_STRUCT));

				MlmeDisassocReqAction(pAd, pMsgElem);
				os_free_mem(NULL, pMsgElem);

				RtmpusecDelay(1000);
			}
		}
	}
#endif /* CONFIG_STA_SUPPORT */

	/* Set Radio off flag */
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
		/* Link down first if any association exists */
		if (INFRA_ON(pAd) || ADHOC_ON(pAd))
			LinkDown(pAd, FALSE);
		RtmpusecDelay(10000);

		/*==========================================*/
		/* Clean up old bss table */
#ifndef ANDROID_SUPPORT
/* because abdroid will get scan table when interface down, so we not clean scan table */
		BssTableInit(&pAd->ScanTab);
#endif /* ANDROID_SUPPORT */

	}
#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	    APStop(pAd);
#endif /* CONFIG_AP_SUPPORT */

#ifdef LED_CONTROL_SUPPORT
	RTMPSetLED(pAd, LED_RADIO_OFF);
#endif /* LED_CONTROL_SUPPORT */

	ASIC_RADIO_OFF(pAd, MLME_RADIO_OFF);
}

VOID RT28xxUsbAsicRadioOff(RTMP_ADAPTER *pAd)
{
	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __func__));

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);

	if (pAd->CommonCfg.CentralChannel)
		AsicTurnOffRFClk(pAd, pAd->CommonCfg.CentralChannel);
	else
		AsicTurnOffRFClk(pAd, pAd->CommonCfg.Channel);

#ifdef CONFIG_STA_SUPPORT
	/* send POWER-SAVE command to MCU. Timeout 40us. */
	AsicSendCommandToMcu(pAd, 0x30, 0xff, 0xff, 0x02, FALSE);
	/* Stop bulkin pipe */
	if ((pAd->PendingRx > 0) && (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))) {
		RTUSBCancelPendingBulkInIRP(pAd);
		pAd->PendingRx = 0;
	}
#endif /* CONFIG_STA_SUPPORT */
	DBGPRINT(RT_DEBUG_TRACE, ("<== %s\n", __func__));

}

VOID RT28xxUsbAsicRadioOn(RTMP_ADAPTER *pAd)
{
	UINT32 MACValue = 0;
	BOOLEAN brc;
	UINT RetryRound = 0;
	WPDMA_GLO_CFG_STRUC GloCfg;
#ifdef RTMP_RF_RW_SUPPORT
	UCHAR rfreg = 0;
#endif /* RTMP_RF_RW_SUPPORT */
	RTMP_CHIP_OP *pChipOps = &pAd->chipOps;
	NDIS_STATUS ret;

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __func__));

	if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev, pObj->intf)) == 1) {
		DBGPRINT(RT_DEBUG_TRACE, ("RT28xxUsbAsicRadioOn: autopm_resume success\n"));
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
	} else if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev, pObj->intf)) == (-1)) {
		DBGPRINT(RT_DEBUG_ERROR, ("RT28xxUsbAsicRadioOn autopm_resume fail ------\n"));
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		return;
	} else
		DBGPRINT(RT_DEBUG_TRACE, ("RT28xxUsbAsicRadioOn: autopm_resume do nothing\n"));

#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

	/* make some traffic to invoke EvtDeviceD0Entry callback function */

	RTUSBReadMACRegister(pAd, 0x1000, &MACValue);
	DBGPRINT(RT_DEBUG_TRACE,
		("A MAC query to invoke EvtDeviceD0Entry, MACValue = 0x%x\n", MACValue));

	/* 1. Send wake up command. */
	{
		RetryRound = 0;
		do {
			brc = AsicSendCommandToMcu(pAd, 0x31, PowerWakeCID, 0x00, 0x02, FALSE);
			if (brc)
				/* Wait command ok. */
				brc = AsicCheckCommandOk(pAd, PowerWakeCID);

			if (brc)
				break;	/* PowerWakeCID cmd successed */

			DBGPRINT(RT_DEBUG_WARN, ("PSM :WakeUp Cmd Failed, retry %d\n", RetryRound));

			/* try 10 times at most */
			if ((RetryRound++) > 10)
				break;
			/* delay and try again */
			RtmpusecDelay(200);
		} while (TRUE);
		if (RetryRound > 10)
			DBGPRINT(RT_DEBUG_WARN,
				("PSM :ASIC 0x31 WakeUp Cmd may Fail %d*******\n", RetryRound));
	}

	/* 2. Enable Tx/Rx DMA. */
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x4);
	AsicWaitPDMAIdle(pAd, 200, 1000);

	RtmpusecDelay(50);
	RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);
	GloCfg.field.EnTXWriteBackDDONE = 1;
	GloCfg.field.WPDMABurstSIZE = pAd->chipCap.WPDMABurstSIZE;
	GloCfg.field.EnableRxDMA = 1;
	GloCfg.field.EnableTxDMA = 1;
	RTMP_IO_WRITE32(pAd, WPDMA_GLO_CFG, GloCfg.word);
	DBGPRINT(RT_DEBUG_TRACE, ("<== WRITE DMA offset 0x208 = 0x%x\n", GloCfg.word));

	/* enable RX of MAC block */
	AsicSetRxFilter(pAd);

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0xc);

	/* 3. Turn on RF */
/*	RT28xxUsbAsicRFOn(pAd);*/
	if (pChipOps->AsicReverseRfFromSleepMode)
		pChipOps->AsicReverseRfFromSleepMode(pAd, FALSE);

#ifdef RTMP_RF_RW_SUPPORT
/*for 3xxx ? need to reset R07 for VO......*/
	ret = RT30xxReadRFRegister(pAd, RF_R07, &rfreg);
	if (ret != STATUS_SUCCESS)
		DBGPRINT(RT_DEBUG_WARN, ("ret=%d, RT30xxReadRFRegister is not successful!\n", ret));
	rfreg = rfreg | 0x1;
	RT30xxWriteRFRegister(pAd, RF_R07, rfreg);
#endif /* RTMP_RF_RW_SUPPORT */

	/* 4. Clear idle flag */
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);

	/* Send Bulkin IRPs after flag fRTMP_ADAPTER_IDLE_RADIO_OFF is cleared. */
	/*      */
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	    RTUSBBulkReceive(pAd);
#endif /* CONFIG_STA_SUPPORT */
	DBGPRINT(RT_DEBUG_TRACE, ("<== %s\n", __func__));

}

BOOLEAN AsicCheckCommandOk(IN PRTMP_ADAPTER pAd, IN UCHAR Command)
{
	UINT32 CmdStatus, CID, i;
	UINT32 ThisCIDMask = 0;
	INT ret;

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd)) {
		RTMP_SEM_EVENT_WAIT(&pAd->reg_atomic, ret);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", ret));
			return FALSE;
		}
	}
#endif /* RTMP_MAC_USB */

	i = 0;
	do {
		RTUSBReadMACRegister(pAd, H2M_MAILBOX_CID, &CID);
		if ((CID & CID0MASK) == Command) {
			ThisCIDMask = CID0MASK;
			break;
		} else if ((((CID & CID1MASK) >> 8) & 0xff) == Command) {
			ThisCIDMask = CID1MASK;
			break;
		} else if ((((CID & CID2MASK) >> 16) & 0xff) == Command) {
			ThisCIDMask = CID2MASK;
			break;
		} else if ((((CID & CID3MASK) >> 24) & 0xff) == Command) {
			ThisCIDMask = CID3MASK;
			break;
		}

		RtmpusecDelay(100);
		i++;
	} while (i < 200);

	ret = FALSE;
	RTUSBReadMACRegister(pAd, H2M_MAILBOX_STATUS, &CmdStatus);
	if (i < 200) {
		if (((CmdStatus & ThisCIDMask) == 0x1) || ((CmdStatus & ThisCIDMask) == 0x100)
		    || ((CmdStatus & ThisCIDMask) == 0x10000)
		    || ((CmdStatus & ThisCIDMask) == 0x1000000))
			ret = TRUE;
	}
	RTUSBWriteMACRegister(pAd, H2M_MAILBOX_STATUS, 0xffffffff, FALSE);
	RTUSBWriteMACRegister(pAd, H2M_MAILBOX_CID, 0xffffffff, FALSE);

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(pAd))
		RTMP_SEM_EVENT_UP(&pAd->reg_atomic);
#endif /* RTMP_MAC_USB */

	return ret;

}

#endif /* RTMP_MAC_USB */
