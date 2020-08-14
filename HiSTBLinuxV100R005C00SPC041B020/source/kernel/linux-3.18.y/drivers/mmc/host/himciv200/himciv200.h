#ifndef _HI_MCI_H_
#define _HI_MCI_H_

extern int trace_level;
#define HIMCI_TRACE_LEVEL  5
/*
   0 - all message
   1 - dump all register read/write
   2 - flow trace
   3 - timeouut err and protocol err
   */

#define HIMCI_TRACE_FMT         KERN_INFO
#define CARD_UNPLUGED           0
#define HI_MCI_DETECT_TIMEOUT   (HZ/2)
#define HI_MCI_REQUEST_TIMEOUT  (5 * HZ)
#define MAX_RETRY_COUNT         100

#define himci_trace(level, msg...) do { \
	if ((level) >= trace_level) { \
		printk(HIMCI_TRACE_FMT "%s:%d: ", __func__, __LINE__); \
		printk(msg); \
		printk("\n"); \
	} \
} while (0)

#define himci_error(msg...) do { \
	printk("%s:%d: ", __func__, __LINE__); \
	pr_err(msg); \
	if (himci_hung) \
		BUG(); \
	printk("\n"); \
} while (0)

struct himci_des {
	u32 idmac_des_ctrl;
	u32 idmac_des_buf_size;
	u32 idmac_des_buf_addr;
	u32 idmac_des_next_addr;
};

struct himciv200_host {
	struct mmc_host  *mmc;
	struct device    *dev;
	struct mmc_data  *data;
	struct mmc_request *mrq;
	struct mmc_command *cmd;

	void __iomem    *ioaddr;
	void __iomem    *ioshare_addr;
	void __iomem    *ldoaddr;

	struct clk *clk;
	spinlock_t lock;
	u32 irq;
	u32 devid;
	u32 card_status;
	u32 power_on;  /* bool, true - power is on */
	u32 ldo_shift;
	u32 tunning;
	u32 iovoltage;

	struct scatterlist  *dma_sg;
	void      *dma_vaddr;
	dma_addr_t dma_paddr;

	unsigned int dma_sg_num;
	unsigned int dma_alloc_size;
	unsigned int dma_dir;

	struct timer_list  timer;
	wait_queue_head_t  intr_wait;
#define HIMCI_PEND_DTO_b  (0)
#define HIMCI_PEND_DTO_m  (1 << HIMCI_PEND_DTO_b)
#define HIMCI_PEND_CD_b   (1)
#define HIMCI_PEND_CD_m   (1 << HIMCI_PEND_CD_b)
#define HIMCI_PEND_VOLT_SWITCH_b   (10)
#define HIMCI_PEND_VOLT_SWITCH_m   (1 << HIMCI_PEND_VOLT_SWITCH_b)
	unsigned long      pending_events;
};

#define mci_readl(host, reg) \
	readl((char *)(host)->ioaddr + reg)

#define mci_writel(host, reg, value) \
	writel((value), (char *)(host)->ioaddr + reg)

#endif
