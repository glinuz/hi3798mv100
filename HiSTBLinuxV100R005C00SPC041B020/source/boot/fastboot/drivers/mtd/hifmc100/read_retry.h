/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
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
 * Create By Cai Zhiying 2015.7.27
 *
******************************************************************************/

#ifndef READ_RETRY_H
#define READ_RETRY_H

extern struct nand_read_retry nand_read_retry_micron;
extern struct nand_read_retry nand_read_retry_samsung;
extern struct nand_read_retry nand_read_retry_toshiba_v2012;
extern struct nand_read_retry nand_read_retry_toshiba_v2013;
extern struct nand_read_retry nand_read_retry_hynix_cg_adie;
extern struct nand_read_retry nand_read_retry_hynix_bc_cdie;

#endif /* READ_RETRY_H*/
