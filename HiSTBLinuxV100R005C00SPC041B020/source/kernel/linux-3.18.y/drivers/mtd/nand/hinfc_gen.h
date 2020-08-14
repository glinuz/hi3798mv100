/******************************************************************************
 *    NAND Flash Controller V504 Device Driver
 *    Copyright (c) 2009-2010 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Czyong.
 *
******************************************************************************/

#ifndef __HINFC_GEN_H__
#define __HINFC_GEN_H__
/******************************************************************************/
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/string_helpers.h>
#include <asm/setup.h>
#include <linux/module.h>

#define SZ_4G                           0x100000000ULL
#define SZ_8G                           0x200000000ULL
#define SZ_16G                          0x400000000ULL

/******************************************************************************/

#define HINFC_VER_300                   (0x300)
#define HINFC_VER_301                   (0x301)
#define HINFC_VER_310                   (0x310)
#define HINFC_VER_504                   (0x504)
#define HINFC_VER_505                   (0x505)
#define HINFC_VER_600                   (0x600)
#define HINFC_VER_610                   (0x610)
#define HINFC_VER_620                   (0x620)

/******************************************************************************/

#define NAND_ECC_NONE                        0
#define NAND_ECC_1BIT_512                    1
#define NAND_ECC_4BIT                        2
#define NAND_ECC_4BIT_512                    3
#define NAND_ECC_4BYTE                       4
#define NAND_ECC_8BIT                        5
#define NAND_ECC_8BIT_512                    6
#define NAND_ECC_8BYTE                       7
#define NAND_ECC_13BIT                       8
#define NAND_ECC_16BIT                       9
#define NAND_ECC_18BIT                       10
#define NAND_ECC_24BIT                       11
#define NAND_ECC_27BIT                       12
#define NAND_ECC_28BIT                       13
#define NAND_ECC_32BIT                       14
#define NAND_ECC_40BIT                       15
#define NAND_ECC_41BIT                       16
#define NAND_ECC_42BIT                       17
#define NAND_ECC_48BIT                       18
#define NAND_ECC_60BIT                       19
#define NAND_ECC_64BIT                       20
#define NAND_ECC_72BIT                       21
#define NAND_ECC_80BIT                       22

extern char *nand_dbgfs_options;

const char *nand_ecc_name(int type);

struct nand_ctrl_info_t {
	int pagesize;
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
	int (*set_rr_param)(void *host, int param);
	int (*get_rr_param)(void *host);
	int (*reset_rr_param)(void *host);
};

struct ecc_info_t {
	int pagesize;
	int ecctype;
	int threshold;
	int section;
	void (*dump)(void *host, unsigned char ecc[], int *max_bitsflag);
};

struct nand_dev_t {
	struct nand_flash_dev flash_dev;

	char *start_type;
	unsigned char ids[8];
	int oobsize;
	int ecctype;

	/* (Controller) support ecc/page detect, driver don't need detect */
#define NANDC_HW_AUTO                         0x01
	/* (Controller) support ecc/page detect, and current ecc/page config finish */
#define NANDC_CONFIG_DONE                     0x02
	/* (Controller) is sync, default is async */
#define NANDC_IS_SYNC_BOOT                    0x04

/* (NAND) need randomizer */
#define NAND_RANDOMIZER                       0x10
/* (NAND) is ONFI interface, combine with sync/async symble */
#define NAND_IS_ONFI                          0x20
/* (NAND) support async and sync, such micron onfi, toshiba toggle 1.0 */
#define NAND_MODE_SYNC_ASYNC                  0x40
/* (NAND) support only sync, such samsung sync. */
#define NAND_MODE_ONLY_SYNC                   0x80

#define NAND_CHIP_MICRON   (NAND_MODE_SYNC_ASYNC | NAND_IS_ONFI)
/* This NAND is async, or sync/async, default is async mode, toggle1.0 interface */
#define NAND_CHIP_TOSHIBA_TOGGLE_10  (NAND_MODE_SYNC_ASYNC)
/* This NAND is only sync mode, toggle2.0 interface */
#define NAND_CHIP_TOSHIBA_TOGGLE_20   (NAND_MODE_ONLY_SYNC)
/* This NAND is only sync mode */
#define NAND_CHIP_SAMSUNG  (NAND_MODE_ONLY_SYNC)

	int flags;
	int read_retry_type;
};

#define IS_NANDC_HW_AUTO(_host)         ((_host)->flags & NANDC_HW_AUTO)
#define IS_NANDC_CONFIG_DONE(_host)     ((_host)->flags & NANDC_CONFIG_DONE)
#define IS_NANDC_SYNC_BOOT(_host)       ((_host)->flags & NANDC_IS_SYNC_BOOT)

#define IS_NAND_RANDOM(_dev)         ((_dev)->flags & NAND_RANDOMIZER)
#define IS_NAND_ONLY_SYNC(_dev)      ((_dev)->flags & NAND_MODE_ONLY_SYNC)
#define IS_NAND_SYNC_ASYNC(_dev)     ((_dev)->flags & NAND_MODE_SYNC_ASYNC)
#define IS_NAND_ONFI(_dev)           ((_dev)->flags & NAND_IS_ONFI)

#define ERSTR_HARDWARE  "Hardware configuration error. "
#define ERSTR_DRIVER    "Driver does not support. "

#define ENABLE                    1
#define DISABLE                   0

/*****************************************************************************/

extern int (*hinfc_param_adjust)(struct mtd_info *, struct nand_chip *, struct nand_dev_t *);

struct nand_flash_dev *hinfc_get_flash_type(struct mtd_info *mtd,
	struct nand_chip *chip, u8 *id_data, int *busw);

struct nand_flash_dev *hinfc_nand_probe(struct mtd_info *mtd,
					struct nand_chip *chip,
					struct nand_dev_t *nand_dev);
struct nand_flash_dev *hisfc_nand_probe(struct mtd_info *mtd,
					struct nand_chip *chip,
					struct nand_dev_t *nand_dev);

void hinfc_nand_param_adjust(struct mtd_info *mtd, struct nand_chip *chip);

void hinfc_show_info(struct mtd_info *mtd, char *vendor, char *chipname);

void hinfc_show_chipsize(struct nand_chip *chip);

int get_bits(unsigned int n);

#define hinfc_pr_msg(_fmt, arg...) printk(_fmt, ##arg)

#define hinfc_pr_bug(fmt, args...) do{\
	printk("%s(%d): bug " fmt, __FILE__, __LINE__, ##args); \
	while(1); \
} while (0)

/******************************************************************************/
#endif /* HINFC_COMMONH */

