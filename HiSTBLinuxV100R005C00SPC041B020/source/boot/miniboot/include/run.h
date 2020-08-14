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

#ifndef RUNH
#define RUNH
/******************************************************************************/

#include <stddef.h>


#define HI_ADVCA_MAGIC          "Hisilicon_ADVCA_ImgHead_MagicNum"
#define HI_ADVCA_MAGIC_SIZE     32
#define HI_ADVCA_HEADER_SIZE    0x2000

#ifdef CONFIG_TEE_SUPPORT

extern int is_trustedcore_img(char *buf);
extern int do_load_secure_os(unsigned long addr, unsigned long org_offset, unsigned long img_dst, int run, uint32_t *rtos_load_addr);

#endif /* CONFIG_TEE_SUPPORT */

int kern_load(const char *addr);

uint32 get_kern_tags(uint32 kernel);

int image_load(const char *addr, const char *param);

/******************************************************************************/
#endif /* RUNH */
