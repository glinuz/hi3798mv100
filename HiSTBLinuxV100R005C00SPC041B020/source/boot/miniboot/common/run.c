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

#include <compile.h>
#include <stdio.h>
#include <string.h>
#include <run.h>
#include <env_set.h>
#include <module.h>
#include <mmu.h>
#include <cache.h>
#include <crc32.h>
#include <irq.h>
#include <board.h>
#include <bootimg.h>
#include <malloc.h>

#ifdef CONFIG_ARM64_SUPPORT
extern int is_fip(char *buf);
extern int load_fip(char *buf);
#endif

/******************************************************************************/

static int check_image(const char *addr, kernel_fn *kernel)
{
	uint32 tmp;
	const char *data;
	struct img_hdr_t hdr;

	memcpy(&hdr, addr, sizeof(struct img_hdr_t));

	if (SWAP32(hdr.magic) != UIMG_MAGIC) {
		printf("The image format invalid.\n");
		return -1;
	}

	hdr.hcrc = 0;
	tmp = crc32(0, &hdr, sizeof(struct img_hdr_t));
	if (SWAP32(((struct img_hdr_t *)addr)->hcrc) != tmp) {
		printf("The image head crc32 check fail.\n");
		return -1;
	}

	hdr.name[sizeof(hdr.name)-1] = '\0';
	hdr.entry = SWAP32(hdr.entry);
	hdr.size = SWAP32(hdr.size);
	hdr.load = SWAP32(hdr.load);
	hdr.dcrc = SWAP32(hdr.dcrc);

	printf("Kernel Name:   %s\n", hdr.name);
	printf("Kernel Size:   %d\n", hdr.size);
	printf("Load Address:  0x%08X\n", hdr.load);
	printf("Entry Address: 0x%08X\n", hdr.entry);

	data = addr + sizeof(struct img_hdr_t);

#ifdef CONFIG_VERIFY_KERNEL
	printf("Verifying CRC ... ");
	tmp = crc32(0, data, hdr.size);
	if (tmp != hdr.dcrc) {
		printf("Fail\n");
		return -1;
	}
	printf("OK\n");
#endif /* CONFIG_VERIFY_KERNEL */

	if ((uint32)data != hdr.load) {
		printf("Loading Kernel Image ... ");
		memcpy((char *)hdr.load, data, hdr.size);
		printf("OK\n");
	}

	if (kernel)
		*kernel = (kernel_fn)hdr.entry;

	return 0;
}
/******************************************************************************/

int kern_load(const char *addr)
{
	int ret;
	uint32 params;
	char *buf = (char *)addr;
	unsigned int kernel_buf = (unsigned int)buf;
	kernel_fn kernel = NULL;

#ifdef CONFIG_TEE_SUPPORT
	/* Load Trustedcore OS */
	if (is_trustedcore_img(buf)) {
		return do_load_secure_os((u32)buf, 0, 0, 0, NULL);
	}
#ifdef CONFIG_SUPPORT_CA_RELEASE
	else if (!memcmp((char *)buf, HI_ADVCA_MAGIC, HI_ADVCA_MAGIC_SIZE) && is_trustedcore_img(buf + HI_ADVCA_HEADER_SIZE)) {
		printf("Boot Hisilicon ADVCA SecureOS Image ...\n");
		return do_load_secure_os((u32)buf, HI_ADVCA_HEADER_SIZE, 0, 0, NULL);
	}
#endif
#endif

#ifdef CONFIG_SUPPORT_CA
	printf("Check Hisilicon_ADVCA ...\n");
	if (memcmp((char *)buf, HI_ADVCA_MAGIC, HI_ADVCA_MAGIC_SIZE)) {
		printf("Not hisilicon ADVCA image ...\n");
	} else {
		printf("Boot hisilicon ADVCA image ...\n");
		buf += HI_ADVCA_HEADER_SIZE;
	}
#endif

	if (!check_bootimg((char *)buf, MAX_BOOTIMG_LEN)) {
		char *bootargs;
		unsigned int initrd_start;
		unsigned int initrd_size;
		if (bootimg_get_kernel(buf, MAX_BOOTIMG_LEN, (unsigned int*)&kernel_buf)) {
			printf("Wrong Image Format. \n");
			return 1;
		}

		if (!bootimg_get_ramfs(buf, MAX_BOOTIMG_LEN, &initrd_start, &initrd_size)) {
			bootargs = env_get("bootargs");
			if (bootargs) {
				int sz_str = strlen(bootargs) + 0x40;
				char *str = malloc(sz_str);
				if (!str) {
					printf("malloc failed.\n");
					return 1;
				}
				memset(str, 0, sz_str);
				snprintf(str, sz_str - 1,
					 "%s initrd=0x%X,0x%X", bootargs,
					 initrd_start, initrd_size);

				env_set("bootargs", str);

				free(str);

				show_bootimg_header(buf);
			}
		}
	}

#ifdef CONFIG_ARM64_SUPPORT
	if (is_fip((char *)kernel_buf)) {
		return load_fip((char *)kernel_buf);
	}
#endif

	ret = check_image((char *)kernel_buf, &kernel);
	if (ret)
		return ret;

	module_exit();
	cpu_disable_irq();

	printf ("Starting kernel ...\n\n");

#ifdef CONFIG_NET
	setup_eth_param();
#endif

	params = get_kern_tags((uint32)kernel);

	mmu_cache_disable();
	cache_flush_all();

	kernel(0, CONFIG_MACHINE_ID, params);

	return 0;
}
/******************************************************************************/

int image_load(const char *addr, const char *param)
{
	int (*entry)(const char *param) = (int(*)(const char *))addr;

	module_exit();

	cpu_disable_irq();
	mmu_cache_disable();
	cache_flush_all();

	invalidIcache();
	DSB();
	ISB();

	return entry(param);
}
