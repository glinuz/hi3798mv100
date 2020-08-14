/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#ifndef HINFCGENH
#define HINFCGENH
/******************************************************************************/

#include "nand_chip.h"

#define HINFC_VER_300                   (0x300)
#define HINFC_VER_301                   (0x301)
#define HINFC_VER_310                   (0x310)
#define HINFC_VER_504                   (0x504)
#define HINFC_VER_505                   (0x505)
#define HINFC_VER_600                   (0x600)
#define HINFC_VER_610                   (0x610)
#define HINFC_VER_620                   (0x620)

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

struct nand_ctrl_oob_t {
	int offset; /* controller oob area for app offset */
	int length; /* controller oob area for app length */
};

struct nand_ctrl_info_t {
	int pagesize;
	int ecctype;
	int oobsize; /* controller oob total size (app, ecc) */
	struct nand_ctrl_oob_t *layout;
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
	int (*enable)(struct hinfc_host *host);
	int (*disable)(struct hinfc_host *host);
};

struct read_retry_t {
	int type;
	int count;
	int (*set_rr_param)(struct hinfc_host *host, int param);
	int (*get_rr_param)(struct hinfc_host *host);
	int (*reset_rr_param)(struct hinfc_host *host);
	int (*enable_enhanced_slc)(struct hinfc_host *host, int enable);
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

const char *nand_page_name(int type);
int nandpage_size2type(int size);
int nandecc_size2type(int size);

/******************************************************************************/
#endif /* HINFCGENH */
