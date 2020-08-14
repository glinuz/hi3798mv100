/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
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

#ifndef HIUAPIH
#define HIUAPIH

#include <linux/types.h>

#define HIKAPI_GET_RAM_SIZE       1
#define HIKAPI_GET_CMA_SIZE       2
#define HIKAPI_GET_MMZ_SIZE       3
/**
 * get memory size information, such CMA size, RAM size.
 *
 * param:
 *   size    specify size,
 *   flags   what size you want get.
 *           HIKAPI_GET_RAM_SIZE - get ram size, unit is 1M
 *           HIKAPI_GET_CMA_SIZE - get cma size, unit is 1M
 *           HIKAPI_GET_MMZ_SIZE - get mmz size, unit is 1M
 * retval:
 *   0       success
 *   other   fail.
 */
int get_mem_size(unsigned int *size, int flags);

/*
 * get ddr size, unit is bytes
 */
unsigned long get_dram_size(void);

/* reference man semctl */
long os_semctl(int semid, int semnum, int cmd, unsigned long arg);

/* reference man semget */
long os_semget(key_t key, int nsems, int semflg);

/* return sdk version to kernel */
const char *get_sdkversion(void);

/* ARBITRARY:  SRAM allocations are multiples of this 2^N size */
#define SRAM_GRANULARITY	512

#ifdef CONFIG_SUPPORT_SRAM_MANAGER
void *sram_alloc(size_t len, dma_addr_t *dma);
void sram_free(void *addr, size_t len);
int sram_suspend(void);
void sram_resume(void);
#else
#  define sram_alloc(_len, _dma)   NULL
#  define sram_free(_addr, _len)
#  define sram_suspend()   do { } while (0)
#  define sram_resume()    do { } while (0)
#endif

/*
 * name         param name
 * buf          param buffer pointer;
 * buflen       param length;
 *
 * return       -1:    not found this param;
 *              -2:    input parameter bad;
 *              other: parameter real length;
 */
int get_param_data(const char *name, char *buf, unsigned int buflen);
int set_param_data(const char *name, char *buf, unsigned int buflen);

char *ultohstr(u64 size, char *str, int len);

int pdm_free_reserve_mem(u32 phyaddr, u32 size);

/******************************************************************************/

#include <linux/hisilicon/cputable.h>


#endif /* HIUAPIH */
