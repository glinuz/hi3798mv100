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

#include <cpuinfo.h>
#include <boot.h>

/*****************************************************************************/

uint64 get_chipid(void)
{
	static uint64 chipid = 0;

	if (!chipid) {
		struct cpu_info_t *info = get_cpuinfo();
		chipid = info->chipid & info->chipid_mask;
	}

	return chipid;
}
/*****************************************************************************/

unsigned int get_max_ddr_size(void)
{
	return get_cpuinfo()->max_ddr_size;
}
/******************************************************************************/

int get_bootmedia(char **media, void *arg)
{
	static char *media_name = NULL;
	static unsigned int media_type = 0;

	if (!media_name)
		media_type = get_cpuinfo()->boot_media(&media_name);
	if (media)
		*media = media_name;

	return media_type;
}
/******************************************************************************/
/* sdk use it */
int get_boot_media(void)
{
	return get_bootmedia(NULL, NULL);
}
/*****************************************************************************/

const char *get_cpu_name(void)
{
	return get_cpuinfo()->name;
}
/*****************************************************************************/

unsigned int get_ca_vendor(void)
{
	static unsigned long ca_vendor = 0;
	if (ca_vendor)
		return ca_vendor;
	ca_vendor = get_ca_vendor_reg();
	return ca_vendor;
}
/*****************************************************************************/

