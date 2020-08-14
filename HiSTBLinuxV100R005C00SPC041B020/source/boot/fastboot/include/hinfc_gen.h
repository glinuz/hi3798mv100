/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/
#ifndef HINFCGENH
#define HINFCGENH
/******************************************************************************/

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

#define HINFC_VER_COMMON                (0x00)
#define HINFC_VER_300                   (0x300)
#define HINFC_VER_301                   (0x301)
#define HINFC_VER_310                   (0x310)
#define HINFC_VER_504                   (0x504)
#define HINFC_VER_505                   (0x505)
#define HINFC_VER_600                   (0x600)
#define HINFC_VER_610                   (0x610)
#define HINFC_VER_620                   (0x620)
/* for SPI_NAND */
#define HISFC_VER_400                   (0x400)

#define _512B                           (512)
#define _2K                             (2048)
#define _4K                             (4096)
#define _8K                             (8192)
#define _16K                            (16384)
#define _32K                            (32768)

#define NAND_PAGE_SHIFT                      9 /* 512 */
#define NAND_PAGE_512B                       0
#define NAND_PAGE_1K                         1
#define NAND_PAGE_2K                         2
#define NAND_PAGE_4K                         3
#define NAND_PAGE_8K                         4
#define NAND_PAGE_16K                        5
#define NAND_PAGE_32K                        6

/* default ecc unit is 1K */
#define NAND_ECC_NONE                        0
#define NAND_ECC_1BIT_512                    1
#define NAND_ECC_4BIT                        2
#define NAND_ECC_4BIT_512                    3
#define NAND_ECC_4BYTE                       4
#define NAND_ECC_8BIT                        3 /* need to be equal with NAND_ECC_4BIT_512 */
#define NAND_ECC_8BIT_512                    5
#define NAND_ECC_8BYTE                       6
#define NAND_ECC_13BIT                       7
#define NAND_ECC_16BIT                       5 /* need to be equal with NAND_ECC_8BIT_512 */
#define NAND_ECC_18BIT                       8
#define NAND_ECC_24BIT                       9
#define NAND_ECC_27BIT                       10
#define NAND_ECC_28BIT                       11
#define NAND_ECC_32BIT                       12
#define NAND_ECC_40BIT                       13
#define NAND_ECC_41BIT                       14
#define NAND_ECC_42BIT                       15
#define NAND_ECC_48BIT                       16
#define NAND_ECC_60BIT                       17
#define NAND_ECC_64BIT                       18
#define NAND_ECC_72BIT                       19
#define NAND_ECC_80BIT                       20

const char *nand_ecc_name(int type);

const char *nand_page_name(int type);

static inline int nandpage_type2size(int type)
{
	return (1 << (NAND_PAGE_SHIFT + type));
}

int nandpage_size2type(int size);
int nandecc_size2type(int size);

struct nand_config_table {
	int pagetype;
	int ecctype;
	unsigned int oobsize;
	struct nand_ecclayout *layout;
};

struct hinfc_host;

struct nand_sync {

#define SET_NAND_SYNC_TYPE(_mfr, _onfi, _version) \
	(((_mfr) & 0xFF) << 16) | (((_version) & 0xFF) << 8) | ((_onfi) & 0xFF)

#define GET_NAND_SYNC_TYPE_MFR(_type) (((_type) >> 16) & 0xFF)
#define GET_NAND_SYNC_TYPE_VER(_type) (((_type) >> 8) & 0xFF)
#define GET_NAND_SYNC_TYPE_INF(_type) ((_type)& 0xFF)

#define NAND_TYPE_ONFI_23_MICRON    SET_NAND_SYNC_TYPE(NAND_MFR_MICRON, NAND_IS_ONFI, 0x23)
#define NAND_TYPE_ONFI_30_MICRON    SET_NAND_SYNC_TYPE(NAND_MFR_MICRON, NAND_IS_ONFI, 0x30)
#define NAND_TYPE_TOGGLE_TOSHIBA    SET_NAND_SYNC_TYPE(NAND_MFR_TOSHIBA, 0, 0)
#define NAND_TYPE_TOGGLE_SAMSUNG    SET_NAND_SYNC_TYPE(NAND_MFR_SAMSUNG, 0, 0)

#define NAND_TYPE_TOGGLE_10         SET_NAND_SYNC_TYPE(0, 0, 0x10)
#define NAND_TYPE_ONFI_30           SET_NAND_SYNC_TYPE(0, NAND_IS_ONFI, 0x30)
#define NAND_TYPE_ONFI_23           SET_NAND_SYNC_TYPE(0, NAND_IS_ONFI, 0x23)

	int type;
	int (*enable)(struct nand_chip *chip);
	int (*disable)(struct nand_chip *chip);
};

struct read_retry_t {
	int type;
	int count;
	int (*set_rr_param)(struct hinfc_host *host, int param);
	int (*get_rr_param)(struct hinfc_host *host);
	int (*reset_rr_param)(struct hinfc_host *host);
	int (*enable_enhanced_slc)(struct hinfc_host *host, int enable);
};

struct ecc_info_t {
	int pagesize;
	int ecctype;
	int threshold;
	int section;
	void (*dump)(struct hinfc_host *host, unsigned char ecc[],
			int *max_bitsflag);
};

struct nand_dev_t {
	struct nand_flash_dev flash_dev;

	char *start_type;
	unsigned char ids[8];
	int oobsize;
	int ecctype;

/* (Controller) Current support ecc/page detect, in some condition it does not support. */
#define NANDC_HW_AUTO                         0x01
/* (Controller) support ecc/page detect, and current controller ecc/page has configured finish */
#define NANDC_CONFIG_DONE                     0x02
/* (Controller) has configured sync mode by hardware PIN, default is async */
#define NANDC_IS_SYNC_BOOT                    0x04

/* (NAND) need randomizer */
#define NAND_RANDOMIZER                       0x10
/* (NAND) is ONFI interface, combine with sync/async symble */
#define NAND_IS_ONFI                          0x20
/* (NAND) support async and sync, such micron onfi, toshiba toggle 1.0 */
#define NAND_MODE_SYNC_ASYNC                  0x40
/* (NAND) support only sync, such samsung sync. */
#define NAND_MODE_ONLY_SYNC                   0x80

/* This NAND is async, or sync/async, default is async mode, onfi interface */
#define NAND_CHIP_MICRON   (NAND_MODE_SYNC_ASYNC | NAND_IS_ONFI)
/* This NAND is async, or sync/async, default is async mode, toggle1.0 interface */
#define NAND_CHIP_TOSHIBA_TOGGLE_10  (NAND_MODE_SYNC_ASYNC)
/* This NAND is only sync mode, toggle2.0 interface */
#define NAND_CHIP_TOSHIBA_TOGGLE_20   (NAND_MODE_ONLY_SYNC)
/* This NAND is only sync mode */
#define NAND_CHIP_SAMSUNG  (NAND_MODE_ONLY_SYNC)
	int flags;

#define NAND_RR_NONE                         0x00
#define NAND_RR_HYNIX_BG_BDIE                0x10
#define NAND_RR_HYNIX_BG_CDIE                0x11
#define NAND_RR_HYNIX_CG_ADIE                0x12
#define NAND_RR_MICRON                       0x20
#define NAND_RR_SAMSUNG                      0x30
#define NAND_RR_TOSHIBA_V2012                0x40
#define NAND_RR_TOSHIBA_V2013                0x41
#define NAND_RR_MASK                         0xF0
	int read_retry_type;

	int hostver; /* host controller version. */
	int is_spi_nand;
};

#define IS_NANDC_HW_AUTO(_host)     ((_host)->flags & NANDC_HW_AUTO)
#define IS_NANDC_CONFIG_DONE(_host) ((_host)->flags & NANDC_CONFIG_DONE)
#define IS_NANDC_SYNC_BOOT(_host)   ((_host)->flags & NANDC_IS_SYNC_BOOT)

#define IS_NAND_RANDOM(_dev)     ((_dev)->flags & NAND_RANDOMIZER)
#define IS_NAND_ONLY_SYNC(_dev)  ((_dev)->flags & NAND_MODE_ONLY_SYNC)
#define IS_NAND_SYNC_ASYNC(_dev) ((_dev)->flags & NAND_MODE_SYNC_ASYNC)
#define IS_NAND_ONFI(_dev)       ((_dev)->flags & NAND_IS_ONFI)


#define ERSTR_HARDWARE  "Hardware configuration error. "
#define ERSTR_DRIVER    "Driver does not support. "

#define ENABLE                    1
#define DISABLE                   0

extern struct nand_flash_dev *(*nand_get_spl_flash_type)(struct mtd_info *mtd,
	struct nand_chip *chip, struct nand_dev_t *nand_dev);

extern int (*hinfc_nand_param_adjust)(struct mtd_info *mtd, struct nand_chip *chip,
	struct nand_dev_t *nand_dev);

extern int (*hisfc_nand_param_adjust)(struct mtd_info *mtd, struct nand_chip *chip,
	struct nand_dev_t *nand_dev);

/******************************************************************************/
#endif /* HINFCGENH */
