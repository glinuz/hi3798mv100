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
#ifndef HINFC610_GENH
#define HINFC610_GENH
/******************************************************************************/

#include "nand_ctrl.h"
#include "hinfc610.h"

enum hinfc610_ecc_reg {
	hinfc610_ecc_none   = 0x00,
	hinfc610_ecc_8bit   = 0x01,
	hinfc610_ecc_13bit  = 0x02,
	hinfc610_ecc_18bit  = 0x03,
	hinfc610_ecc_24bit  = 0x04,
	hinfc610_ecc_27bit  = 0x05,
	hinfc610_ecc_32bit  = 0x06,
	hinfc610_ecc_41bit  = 0x07,
	hinfc610_ecc_48bit  = 0x08,
	hinfc610_ecc_60bit  = 0x09,
	hinfc610_ecc_72bit  = 0x0a,
	hinfc610_ecc_80bit  = 0x0b,
};

enum hinfc610_page_reg {
	hinfc610_pagesize_2K    = 0x01,
	hinfc610_pagesize_4K    = 0x02,
	hinfc610_pagesize_8K    = 0x03,
	hinfc610_pagesize_16K   = 0x04,
	hinfc610_pagesize_32K   = 0x05,
};

int hinfc610_get_pagesize(struct hinfc_host *host);

int hinfc610_set_pagesize(struct hinfc_host *host, int pagesize);

int hinfc610_get_ecctype(struct hinfc_host *host);

int hinfc610_set_ecctype(struct hinfc_host *host, int ecctype);

/******************************************************************************/
#endif /* HINFC610_GENH */
