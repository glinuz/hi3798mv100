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
#ifndef HINFC620_GENH
#define HINFC620_GENH
/******************************************************************************/

#include "nand_ctrl.h"
#include "hinfc610.h"

enum hinfc620_ecc_reg {
	hinfc620_ecc_none   = 0x00,
	hinfc620_ecc_8bit   = 0x02,
	hinfc620_ecc_16bit  = 0x03,
	hinfc620_ecc_24bit  = 0x04,
	hinfc620_ecc_40bit  = 0x05,
	hinfc620_ecc_64bit  = 0x06,
	hinfc620_ecc_28bit  = 0x07,
	hinfc620_ecc_42bit  = 0x08,
};

enum hinfc620_page_reg {
	hinfc620_pagesize_2K    = 0x01,
	hinfc620_pagesize_4K    = 0x02,
	hinfc620_pagesize_8K    = 0x03,
	hinfc620_pagesize_16K   = 0x04,
	hinfc620_pagesize_32K   = 0x05,
};

int hinfc620_get_pagesize(struct hinfc_host *host);

int hinfc620_set_pagesize(struct hinfc_host *host, int pagesize);

int hinfc620_get_ecctype(struct hinfc_host *host);

int hinfc620_set_ecctype(struct hinfc_host *host, int ecctype);

/******************************************************************************/
#endif /* HINFC620_GENH */
