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

#ifndef CPUINFOH
#define CPUINFOH

#include <stddef.h>

struct cpu_info_t {
	char *name;
	uint64 chipid;
	uint64 chipid_mask;
	uint32 max_ddr_size;
	int (*boot_media)(char **media);
	void (*get_clock)(unsigned int *cpu, unsigned int *timer);
	int (*get_ca_type)(void);
};

struct cpu_info_t *get_cpuinfo(void);

#define BOOT_MEDIA_UNKNOWN                     (0)
#define BOOT_MEDIA_DDR                         (1)
#define BOOT_MEDIA_NAND                        (2)
#define BOOT_MEDIA_SPIFLASH                    (3)
#define BOOT_MEDIA_SPI_NAND                    (4)
#define BOOT_MEDIA_EMMC                        (5)
#define BOOT_MEDIA_SD                          (6)
int get_bootmedia(char **media, void *arg);

int get_boot_media(void);

uint64 get_chipid(void);

unsigned int get_max_ddr_size(void);

const char *get_cpu_name(void);

unsigned int get_timer_clock(void);

int get_cpuno(void);

unsigned int get_ca_vendor(void);

#endif /* CPUINFOH */
