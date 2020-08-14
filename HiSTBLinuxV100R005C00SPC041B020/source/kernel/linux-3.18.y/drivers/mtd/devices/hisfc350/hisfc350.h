/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.11
 *
******************************************************************************/

#ifndef HISFC350H
#define HISFC350H

#define HISFC350_SYSCTRL_LENGTH                        (0x100)

/*****************************************************************************/
#define HISFC350_MAX_READY_WAIT_JIFFIES                (40 * HZ)

/*****************************************************************************/
#define HISFC350_MAX_CHIP                    2

#define HISFC350_DMA_ALIGN_SIZE              (256)
#define HISFC350_DMA_ALIGN_MASK              (HISFC350_DMA_ALIGN_SIZE-1)
#define HISFC350_DMA_MAX_SIZE                (4096)
#define HISFC350_DMA_MAX_MASK                (HISFC350_DMA_MAX_SIZE-1)

/*****************************************************************************/
#define HISFC350_GLOBAL_CONFIG                         0x0100
#define HISFC350_GLOBAL_CONFIG_READ_DELAY(_n)      (((_n) & 0x03) << 3)
#define HISFC350_GLOBAL_CONFIG_ADDR_MODE_4B        (1 << 2)
#define HISFC350_GLOBAL_CONFIG_WRITE_PROTECT       (1 << 1)
#define HISFC350_GLOBAL_CONFIG_SPI_MODE3           (1 << 0)

#define HISFC350_TIMING                                0x0110
#define HISFC350_TIMING_TSHSL(_n)                  ((_n) & 0xF)
#define HISFC350_TIMING_TCSS(_n)                   (((_n) & 0x7) << 8)
#define HISFC350_TIMING_TCSH(_n)                   (((_n) & 0x7) << 12)

#define HISFC350_INT_RAW_STATUS                        0x0120
#define HISFC350_INT_RAW_STATUS_DMA_DONE           (1<<1)
#define HISFC350_INT_STATUS                            0x0124
#define HISFC350_INT_MASK                              0x0128
#define HISFC350_INT_CLEAR                             0x012C
#define HISFC350_INT_CLEAR_DMA_DONE                (1<<1)

#define HISFC350_BUS_CONFIG1                           0x0200
#define HISFC350_BUS_CONFIG1_READ_EN             (1<<31)
#define HISFC350_BUS_CONFIG1_WRITE_EN            (1<<30)
#define HISFC350_BUS_CONFIG1_WRITE_INS(_n)       ((_n & 0xFF) << 22)
#define HISFC350_BUS_CONFIG1_WRITE_DUMMY_CNT(_n) ((_n & 0x7) << 19)
#define HISFC350_BUS_CONFIG1_WRITE_IF_TYPE(_n)   ((_n & 0x7) << 16)
#define HISFC350_BUS_CONFIG1_READ_INS(_n)        ((_n & 0xFF) << 8)
#define HISFC350_BUS_CONFIG1_READ_PREF_CNT(_n)   ((_n & 0x3) << 6)
#define HISFC350_BUS_CONFIG1_READ_DUMMY_CNT(_n)  ((_n & 0x7) << 3)
#define HISFC350_BUS_CONFIG1_READ_IF_TYPE(_n)    (_n & 0x7)

#define HISFC350_BUS_FLASH_SIZE                        0x0210
#define HISFC350_BUS_FLASH_SIZE_CS0_MASK         0x0F
#define HISFC350_BUS_FLASH_SIZE_CS1_MASK         (0x0F << 8)
#define HISFC350_BUS_BASE_ADDR_CS0                     0x0214
#define HISFC350_BUS_BASE_ADDR_CS1                     0x0218
#define HISFC350_BUS_ALIAS_ADDR                        0x021C
#define HISFC350_BUS_ALIAS_CS                          0x0220
#define HISFC350_BUS_DMA_CTRL                          0x0240
#define HISFC350_BUS_DMA_CTRL_START                (1 << 0)
#define HISFC350_BUS_DMA_CTRL_RW(_rw)              ((_rw)<<1)
#define HISFC350_BUS_DMA_CTRL_CS(_cs)              (((_cs) & 0x01) << 4)

#define HISFC350_BUS_DMA_MEM_SADDR                     0x0244
#define HISFC350_BUS_DMA_FLASH_SADDR                   0x0248
#define HISFC350_BUS_DMA_LEN                           0x024C
#define HISFC350_BUS_DMA_LEN_DATA_CNT(_n)          ((_n - 1) & 0x0FFFFFFF)
#define HISFC350_BUS_DMA_AHB_CTRL                      0x0250
#define HISFC350_BUS_DMA_AHB_CTRL_INCR4_EN         (1<<0)
#define HISFC350_BUS_DMA_AHB_CTRL_INCR8_EN         (1<<1)
#define HISFC350_BUS_DMA_AHB_CTRL_INCR16_EN        (1<<2)

#define HISFC350_CMD_CONFIG                            0x0300
#define HISFC350_CMD_CONFIG_MEM_IF_TYPE(_n)        (((_n) & 0x07) << 17)
#define HISFC350_CMD_CONFIG_DATA_CNT(_n)           (((_n - 1) & 0x3F) << 9)
#define HISFC350_CMD_CONFIG_RW_READ                (1<<8)
#define HISFC350_CMD_CONFIG_DATA_EN                (1<<7)
#define HISFC350_CMD_CONFIG_DUMMY_CNT(_n)          (((_n) & 0x07) << 4)
#define HISFC350_CMD_CONFIG_ADDR_EN                (1<<3)
#define HISFC350_CMD_CONFIG_SEL_CS(_cs)            (((_cs) & 0x01) << 1)
#define HISFC350_CMD_CONFIG_START                  (1<<0)

#define HISFC350_CMD_INS                               0x0308
#define HISFC350_CMD_ADDR                              0x030C
#define HISFC350_CMD_ADDR_MASK                     0x3FFFFFFF
#define HISFC350_CMD_DATABUF0                          0x0400
#define HISFC350_CMD_DATABUF15                         0x043C

#define HISFC350_IFCYCLE_STD           0
#define HISFC350_IFCYCLE_DUAL          1
#define HISFC350_IFCYCLE_DUAL_ADDR     2
#define HISFC350_IFCYCLE_DUAL_CMD      3
#define HISFC350_IFCYCLE_QUAD          5
#define HISFC350_IFCYCLE_QUAD_ADDR     6
#define HISFC350_IFCYCLE_QUAD_CMD      7

#define HISFC350_REG_BUF_SIZE \
	(HISFC350_CMD_DATABUF15 - HISFC350_CMD_DATABUF0 + 0x04)

#undef  READ
#define READ           1

#undef  WRITE
#define WRITE          0

#undef  FALSE
#define FALSE          0

#undef  TRUE
#define TRUE           1

/*****************************************************************************/
#define HISFC350_VERSION                                         (0x01F8)

#include "hisfc_ids.h"

struct hisfc_spi;

struct spi_driver {
	int (*wait_ready)(struct hisfc_spi *spi);
	int (*write_enable)(struct hisfc_spi *spi);
	int (*entry_4addr)(struct hisfc_spi *spi, int enable);
	int (*bus_prepare)(struct hisfc_spi *spi, int op);
};

struct hisfc_spi {
	char *name;
	int chipselect;
	u64 chipsize;
	u32 erasesize;
	void __iomem *iobase;

	unsigned int addrcycle;
	struct spi_operation  read[1];
	struct spi_operation  write[1];
	struct spi_operation  erase[1];

	void *host;
	struct spi_driver *driver;
};

struct hisfc_host {
	struct mtd_info   mtd[1];
	void __iomem *iobase;
	void __iomem *regbase;
	struct mutex lock;
	u32 iobase_phyaddr;

	struct clk *clk;

	void (*set_host_4addr_mode)(struct hisfc_host *host, int enable);

	char *dma_viraddr;
	dma_addr_t dma_phyaddr;
	int num_chip;

	u32 erasesize;
	u64 chipsize;
	struct hisfc_spi spi[HISFC350_MAX_CHIP + 1];

	struct spi_operation *clkops;
};

#define MTD_TO_HOST(_mtd)  ((struct hisfc_host *)(_mtd))

#define hisfc_read(_host, _reg) \
	readl(_host->regbase + _reg)

#define hisfc_write(_host, _reg, _value) \
	writel(_value, _host->regbase + _reg)

#define HISFC350_CMD_WAIT_CPU_FINISH(_host) do {\
	unsigned int timeout = 0x10000000; \
	while (((hisfc_read((_host), HISFC350_CMD_CONFIG) \
	& HISFC350_CMD_CONFIG_START)) && timeout) \
	--timeout; \
	if (!timeout) panic("%s: cmd wait cpu finish timeout", __func__); \
} while (0)

#define HISFC350_DMA_WAIT_CPU_FINISH(_host) do {\
	unsigned int timeout = 0x10000000; \
	while (((hisfc_read((_host), HISFC350_BUS_DMA_CTRL) \
	& HISFC350_BUS_DMA_CTRL_START)) && timeout) { \
	--timeout; cond_resched(); }\
	if (!timeout) panic("%s: dma wait cpu finish timeout", __func__); \
} while (0)

void hisfc350_set_rate(struct hisfc_host *host, struct spi_operation *clkops);

/******************************************************************************/
#endif /* HISFC350H */
