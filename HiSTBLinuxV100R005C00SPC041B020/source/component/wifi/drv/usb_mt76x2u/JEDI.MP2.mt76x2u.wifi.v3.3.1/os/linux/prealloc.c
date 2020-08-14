
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include "rt_config.h"

/* USB_BULK_BUF_PREALLOC depends on USB_BULK_BUF_ALIGMENT, and is guaranteed by
 * rules in config.mk.
*/
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
#endif /* USB_BULK_BUF_PREALLOC */
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

void *PreAllocBuffer[BLK_NUM];
unsigned long PreAllocDmaAddr[BLK_NUM];

void prealloc_free(void)
{
	int i;

	for (i = 0; i < BLK_NUM; ++i) {
		if (PreAllocBuffer[i]) {
			printk(KERN_INFO "<==[%d]:PreBuff:0x%p\n", i, PreAllocBuffer[i]);

			kfree(PreAllocBuffer[i]);

		}
	}
	memset(PreAllocBuffer, 0, sizeof(PreAllocBuffer));

	printk(KERN_INFO "%s ok\n", __func__);
}

static int prealloc_alloc(void)
{
	int tx, rx, k;

	memset(PreAllocBuffer, 0, sizeof(PreAllocBuffer));

	/* tx */
#ifdef USB_BULK_BUF_PREALLOC
	for (tx = BLK_TX0; tx < 5 * BUF_ALIGMENT_RINGSIZE; tx++) {
#else
	for (tx = BLK_TX0; tx < 5; ++tx) {
#endif /*USB_BULK_BUF_PREALLOC*/
		PreAllocBuffer[tx] = kmalloc(sizeof(HTTX_BUFFER),
					     GFP_KERNEL | GFP_DMA);
		if (!PreAllocBuffer[tx])
			goto fail;

		PreAllocDmaAddr[tx] = virt_to_phys(PreAllocBuffer[tx]);
	}
	printk(KERN_INFO "%s tx %zd num %d\n", __func__, sizeof(HTTX_BUFFER),
		tx - BLK_TX0);

	/* null pkt */
	PreAllocBuffer[BLK_NULL] = kmalloc(sizeof(TX_BUFFER),
					   GFP_KERNEL | GFP_DMA);
	if (!PreAllocBuffer[BLK_NULL])
		goto fail;

	PreAllocDmaAddr[BLK_NULL] = virt_to_phys(PreAllocBuffer[BLK_NULL]);
	printk(KERN_INFO "%s null pkt %zd\n", __func__, sizeof(TX_BUFFER));

	/* ps-poll */
	PreAllocBuffer[BLK_PSPOLL] = kmalloc(sizeof(TX_BUFFER),
					     GFP_KERNEL | GFP_DMA);
	if (!PreAllocBuffer[BLK_PSPOLL])
		goto fail;

	PreAllocDmaAddr[BLK_PSPOLL] = virt_to_phys(PreAllocBuffer[BLK_PSPOLL]);
	printk(KERN_INFO "%s ps-poll %zd\n", __func__, sizeof(TX_BUFFER));

	/* rx */
	for (rx = BLK_RX0; rx <= BLK_RX7; ++rx) {
		PreAllocBuffer[rx] = kmalloc(MAX_RXBULK_SIZE,
					     GFP_KERNEL | GFP_DMA);
		if (!PreAllocBuffer[rx])
			goto fail;

		PreAllocDmaAddr[rx] = virt_to_phys(PreAllocBuffer[rx]);
	}
	printk(KERN_INFO "%s rx %u num %d\n", __func__, MAX_RXBULK_SIZE,
	       rx - BLK_RX0);

	/* cmd-resp */
	PreAllocBuffer[BLK_CMD] = kmalloc(CMD_RSP_BULK_SIZE,
					  GFP_KERNEL | GFP_DMA);
	if (!PreAllocBuffer[BLK_CMD])
			goto fail;

	PreAllocDmaAddr[BLK_CMD] = virt_to_phys(PreAllocBuffer[BLK_CMD]);
	printk(KERN_INFO "%s cmd-resp %u\n", __func__, CMD_RSP_BULK_SIZE);

	/* all allocated info */
	for (k = 0; k < BLK_NUM; k++)
		printk(KERN_INFO "==>[%d]:PreBuff:0x%p, DmaAddr:0x%p\n", k, PreAllocBuffer[k],
		       (void *)PreAllocDmaAddr[k]);

	printk(KERN_INFO "%s ok\n", __func__);
	return 0; /* succeed */

fail:
	prealloc_free();
	return -ENOMEM;
}

#ifdef USB_BULK_BUF_PREALLOC
void *RTMPQMemAddr(int size, int index, dma_addr_t *pDmaAddr, int type)
{
	int idx = type;

	switch (type) {
	case BLK_TX0:
	case BLK_TX1:
	case BLK_TX2:
	case BLK_TX3:
	case BLK_TX4:
		if (size > sizeof(HTTX_BUFFER))
			return NULL;
		idx = type * BUF_ALIGMENT_RINGSIZE + index;
		break;
	case BLK_NULL:
	case BLK_PSPOLL:
		if (size > sizeof(TX_BUFFER))
			return NULL;
		break;
	case BLK_RX0:
	case BLK_RX1:
	case BLK_RX2:
	case BLK_RX3:
	case BLK_RX4:
	case BLK_RX5:
	case BLK_RX6:
	case BLK_RX7:
		if (size > MAX_RXBULK_SIZE)
			return NULL;
		break;
	case BLK_CMD:
		if (size > CMD_RSP_BULK_SIZE)
			return NULL;
		break;
	default:
		printk(KERN_INFO "unsupported type %d\n", type);
		return NULL;
	}
	/*printk("%s[%d]\n", __func__, idx);*/
	*pDmaAddr = PreAllocDmaAddr[idx];
	return PreAllocBuffer[idx];
}
#else
void *RTMPQMemAddr(int size, dma_addr_t *pDmaAddr, int type)
{
	switch (type) {
	case BLK_TX0:
	case BLK_TX1:
	case BLK_TX2:
	case BLK_TX3:
	case BLK_TX4:
		if (size > sizeof(HTTX_BUFFER))
			return NULL;
		break;
	case BLK_NULL:
	case BLK_PSPOLL:
		if (size > sizeof(TX_BUFFER))
			return NULL;
		break;
	case BLK_RX0:
	case BLK_RX1:
	case BLK_RX2:
	case BLK_RX3:
	case BLK_RX4:
	case BLK_RX5:
	case BLK_RX6:
	case BLK_RX7:
		if (size > MAX_RXBULK_SIZE)
			return NULL;
		break;
	case BLK_CMD:
		if (size > CMD_RSP_BULK_SIZE)
			return NULL;
		break;
	default:
		printk(KERN_INFO "unsupported type %d\n", type);
		return NULL;
	}

	*pDmaAddr = PreAllocDmaAddr[type];
	return PreAllocBuffer[type];
}
#endif /*USB_BULK_BUF_PREALLOC*/
EXPORT_SYMBOL(RTMPQMemAddr);

static int __init prealloc_init(void)
{
	return prealloc_alloc();
}

static void __exit prealloc_exit(void)
{
	prealloc_free();
}

module_init(prealloc_init);
module_exit(prealloc_exit);

