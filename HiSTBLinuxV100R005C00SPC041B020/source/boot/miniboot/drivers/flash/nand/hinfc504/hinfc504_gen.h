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

#ifndef HINFC504_GENH
#define HINFC504_GENH
/******************************************************************************/

#include "nand_ctrl.h"
#include "hinfc504.h"

enum hinfc504_ecc_reg {
	hinfc504_ecc_none    = 0x00,
	hinfc504_ecc_1bit    = 0x01,
	hinfc504_ecc_4bit    = 0x02,
	hinfc504_ecc_4bytes  = 0x02,
	hinfc504_ecc_8bytes  = 0x03,
	hinfc504_ecc_24bit1k = 0x04,
	hinfc504_ecc_40bit1k = 0x05,
};

enum hinfc504_page_reg {
	hinfc504_pagesize_512   = 0x00,
	hinfc504_pagesize_2K    = 0x01,
	hinfc504_pagesize_4K    = 0x02,
	hinfc504_pagesize_8K    = 0x03,
	hinfc504_pagesize_16K   = 0x04,
};

int hinfc504_get_pagesize(struct hinfc_host *host);

void hinfc504_set_pagesize(struct hinfc_host *host, int pagesize);

int hinfc504_get_ecctype(struct hinfc_host *host);

void hinfc504_set_ecctype(struct hinfc_host *host, int ecctype);

/******************************************************************************/
#endif /* HINFC504_GENH */
