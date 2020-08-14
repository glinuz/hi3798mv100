/******************************************************************************
*    Copyright (c) 2009-2010 by Hisi.
*    All rights reserved.
* ***
*    Create by Czyong. 2010-12-16
*
******************************************************************************/

#ifndef HISFC350H
#define HISFC350H
/******************************************************************************/
#ifndef CONFIG_HISFC350_REG_BASE_ADDRESS
#define CONFIG_HISFC350_REG_BASE_ADDRESS               (0xF9800000)
#  warning NOT config CONFIG_HISFC350_REG_BASE_ADDRESS, \
	used default value, maybe invalid.
#endif /* CONFIG_HISFC350_REG_BASE_ADDRESS */

#ifndef CONFIG_HISFC350_BUFFER_BASE_ADDRESS
#define CONFIG_HISFC350_BUFFER_BASE_ADDRESS            (0xFA000000)
#  warning NOT config CONFIG_HISFC350_BUFFER_BASE_ADDRESS, \
	used default value, maybe invalid.
#endif /* CONFIG_HISFC350_BUFFER_BASE_ADDRESS */

#ifndef CONFIG_HISFC350_CHIP_NUM
#define CONFIG_HISFC350_CHIP_NUM                       (2)
#  warning NOT config CONFIG_HISFC350_CHIP_NUM, \
	used default value, maybe invalid.
#endif /* CONFIG_HISFC350_CHIP_NUM */

//#define CONFIG_HISFC350_SHOW_CYCLE_TIMING
/*****************************************************************************/
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

#define HISFC350_VERSION                               0x01F8

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
/* ddr start addr should be in [0x6000_0000, 0x6FFF_FFFF] */
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
	unsigned long long chipsize;
	unsigned int erasesize;
	void *iobase;

	unsigned int addrcycle;
	struct spi_operation  read[1];
	struct spi_operation  write[1];
	struct spi_operation  erase[1];

	void * host;
	struct spi_driver *driver;
};

struct hisfc_host {
	struct spi_flash  spiflash[1];
	void  *iobase;
	void  *regbase;
	void  *lock;
	void  *cfgreg;
	unsigned int erasesize;

	void (*set_system_clock)(struct hisfc_host *host,
		struct spi_operation *op, int clk_en);

	void (*set_host_addr_mode)(struct hisfc_host *host, int enable);

	char *dma_buffer;
	int num_chip;
	struct hisfc_spi spi[CONFIG_HISFC350_CHIP_NUM+1];
	char buffer[HISFC350_DMA_MAX_SIZE];
};

#define SPIFLASH_TO_HOST(_spiflash)     ((struct hisfc_host *)(_spiflash))
/*****************************************************************************/
#define hisfc_read(_host, _reg) \
	readl((unsigned)((char *)_host->regbase + (_reg)))

#define hisfc_write(_host, _reg, _value) \
	writel((unsigned)(_value), (unsigned)((char *)_host->regbase + (_reg)))

#define HISFC350_CMD_WAIT_CPU_FINISH(_host) do {\
	unsigned int timeout = 0x10000000; \
	while (((hisfc_read((_host), HISFC350_CMD_CONFIG) \
		& HISFC350_CMD_CONFIG_START)) && timeout) \
		--timeout; \
	if (!timeout) \
		DBG_BUG("cmd wait cpu finish timeout\n"); \
} while (0)

#define HISFC350_DMA_WAIT_CPU_FINISH(_host) do {\
	unsigned int timeout = 0x10000000; \
	while (((hisfc_read((_host), HISFC350_BUS_DMA_CTRL) \
		& HISFC350_BUS_DMA_CTRL_START)) && timeout) \
		--timeout; \
	if (!timeout) \
		DBG_BUG("dma wait cpu finish timeout\n"); \
} while (0)

/*****************************************************************************/
#if 0
#  define DBG_MSG(_fmt, arg...)
#else
#  define DBG_MSG(_fmt, arg...) \
	printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);
#endif

#define DBG_WARN(_fmt, arg...) \
	printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);

#undef DBG_BUG
#define DBG_BUG(fmt, args...) do {\
	printf("%s(%d): BUG: " fmt, __FILE__, __LINE__, ##args); \
	while(1); \
} while (0)
/*****************************************************************************/
#ifndef NULL
#  define NULL         (void*)0
#endif
/******************************************************************************/
#endif /* HISFC350H */
