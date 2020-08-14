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

#include <config.h>
#include <compile.h>
#include <module.h>
#include <stdio.h>
#include <string.h>
#include <flash.h>
#include <malloc.h>
#include <env_set.h>
#include <errno.h>
#include <crc32.h>
#include <boot.h>
#include <cpuinfo.h>

//#define MODULE_DEBUG
#define MOULE_NAME  "Env: "
#include <debug.h>

#ifndef CONFIG_ENV_BACKUP_FROM
#  define CONFIG_ENV_BACKUP_FROM       CONFIG_ENV_FROM
#endif /* CONFIG_ENV_BACKUP_FROM */

#ifndef CONFIG_ENV_RANGE
#  define CONFIG_ENV_RANGE             CONFIG_ENV_SIZE
#endif /* CONFIG_ENV_RANGE */

struct env_t {
	uint32 crc32;
	char data[CONFIG_ENV_SIZE - sizeof(uint32)];
};

struct global_t {
	char *env_tail; /* point to next key start address */
	struct env_t env;
	struct flash_intf_t *flashif;
};
static struct global_t *global = NULL;
/*****************************************************************************/
/* item - NULL, or next key start */
static char *get_next_item(struct env_t *env, const char *item)
{
	const char *env_end = (char *)&env[1];
	const char *env_tail = global->env_tail;

	if (!item)
		return env->data;

	ASSERT(env_tail >= env->data && env_tail <= env_end);

	if (item >= env_tail)
		return NULL;

	ASSERT(item >= env->data);

	if (!*item)
		return NULL;

	while (item < env_tail)
		if (!*item++)
			return (char *)((!*item) ? NULL: item);

	return NULL;
}
/*****************************************************************************/

static char *find_env_item(struct env_t *env, const char *key, char **value)
{
	int length = (int)strnlen(key, CONFIG_ENV_SIZE);
	char *item = get_next_item(env, NULL);

	ASSERT(length < CONFIG_ENV_SIZE);

	if (value)
		*value = NULL;

	while (item) {
		if (!strncmp(key, item, length) && item[length] == '=') {
			if (value)
				*value = item + length + 1;
			return item;
		}
		item = get_next_item(env, item);
	}

	return NULL;
}
/*****************************************************************************/

static char *find_env_tail(char *str, int sz_str)
{
	char *ptr;
	char *ptr0 = str;
	char *ptr1 = str + sz_str - 1;

	if (!(ptr0[0] | ptr0[1]))
		return ptr0;

	if (ptr1[0] || ptr1[-1])
		return &ptr1[1];

	while (ptr1 > (ptr0 + 2)) {
		ptr = (char *)((uint32)ptr0 >> 1) + ((uint32)ptr1 >> 1);

		if (!(ptr[0] | ptr[1]))
			ptr1 = ptr;
		else
			ptr0 = ptr;
	}

	for (ptr = ptr0; ptr < ptr1; ptr++) {
		if (!(ptr[0] | ptr[1]))
			return &ptr[1];
	}

	return &ptr1[1];
}
/*****************************************************************************/

void env_dump(void)
{
	char *item;
	struct env_t *env = &global->env;

	ASSERT(global != NULL);

	item = get_next_item(env, NULL);
	while (item) {
		printf("%s\n", item);
		item = get_next_item(env, item);
	}
	printf("\nEnv size: (%d/%d)\n",
	       (uint32)(global->env_tail - env->data),
	       sizeof(env->data));
}
/*****************************************************************************/

int env_set(const char *key, const char *value)
{
	char *item;
	int newlen;
	int orglen = 0;
	int leavelen;
	struct env_t *env = &global->env;
	char *env_tail = global->env_tail;
	const char *env_end = (char *)&env[1];

	ASSERT(global != NULL);

	if (!key || !*key) {
		pr_warn("key is invalid\n");
		return 0;
	}

	if (!value || !*value) {
		pr_warn("\"%s\" value is invalid\n", key);
		return 0;
	}

	item = find_env_item(env, key, NULL);
	if (item) {
		char *next;

		orglen = strlen(item) + 1;
		next = get_next_item(env, item);

		if (next)
			memcpy(item, next, env_tail - next);

		env_tail -= orglen;
	}

	if (env_tail >= env_end - 1) {
		pr_warn("env space is full\n");
		return 0;
	}

	leavelen = env_end - env_tail - 1;
	newlen = snprintf(env_tail, leavelen, "%s=%s", key, value) + 1;
	env_tail += newlen;
	env_tail[0] = '\0';

	if (newlen < orglen)
		memset(env_tail, 0, orglen - newlen);

	global->env_tail = env_tail;

	return 0;
}
/*****************************************************************************/

int env_clear(const char *key)
{
	int orglen;
	char *item, *next;
	struct env_t *env = &global->env;
	char *env_tail = global->env_tail;

	ASSERT(global != NULL);

	if (!key || !*key) {
		pr_warn("key is invalid\n");
		return 0;
	}

	item = find_env_item(env, key, NULL);
	if (!item)
		return 0;

	orglen = (int)strlen(item) + 1;
	next = get_next_item(env, item);
	if (next)
		memcpy(item, next, env_tail - next);

	env_tail -= orglen;

	memset(env_tail, 0, orglen);

	global->env_tail = env_tail;

	return 0;
}
/*****************************************************************************/

char *env_get(const char *key)
{
	char *value = NULL;

	if (!key || !*key) {
		pr_warn("key is invalid\n");
		return NULL;
	}

	ASSERT(global != NULL);

	find_env_item(&global->env, key, &value);

	return value;
}
/*****************************************************************************/

int env_get_number(const char *key, int *number)
{
	char *value;

	ASSERT(number != NULL);

	value = env_get(key);
	if (!value)
		return -EINVAL;

	*number = strtoul(value, NULL, 10);

	return 0;
}
/*****************************************************************************/

char *env_get_buf(char **envbuf, int *envsize)
{
	if (envsize)
		*envsize = sizeof(struct env_t);

	ASSERT(global != NULL);

	if (envbuf) {
		*envbuf = (char *)&global->env;
		return *envbuf;
	}

	return NULL;
}
/*****************************************************************************/

int env_save_to_flash(void)
{
	int64 ret;
	char *ptr;
	uint32 sz_erase;
	int stub, round;
	struct env_t *env = &global->env;
	struct flash_intf_t *flashif = global->flashif;
	const struct flash_info_t *finfo = flashif->info;

	ASSERT(global != NULL);

	if (!flashif) {
		pr_error("can't save Env.\n");
		return -EINVAL;
	}

	sz_erase = AROUND(CONFIG_ENV_SIZE, finfo->blocksize);

	ret = flash_erase(flashif, 0, (uint64)sz_erase);
	if (ret != sz_erase) {
		pr_warn("Erase Env Fail.\n");
		return ret;
	}

	env->crc32 = crc32(0, env->data, sizeof(env->data));

	ptr = (char *)env;
	round = (CONFIG_ENV_SIZE / finfo->pagesize) * finfo->pagesize;
	stub = CONFIG_ENV_SIZE % finfo->pagesize;

	ret = (int)flash_write(flashif, 0, (uint64)round, ptr, FALSE);
	if (ret != round) {
		pr_warn("Write Env fail.\n");
		return -EIO;
	}

	if (stub) {
		char *buff = malloc(finfo->pagesize);
		if (!buff) {
			pr_error("Out of memory.\n");
			return -ENOMEM;
		}

		memcpy(buff, ptr + round, stub);
		memset(buff + stub, 0, finfo->pagesize - stub);

		ret = (int)flash_write(flashif, (uint64)round,
			(uint64)finfo->pagesize, buff, FALSE);
		if (ret != (int)finfo->pagesize) {
			pr_warn("Write Env fail.\n");
			free(buff);
			return -EIO;
		}
		free(buff);
	}

	return 0;
}
/*****************************************************************************/

static enum flash_type_t get_env_media(void)
{
	enum flash_type_t flashtype =
#if defined(CONFIG_ENV_SPIFLASH)
	FT_SPIFLASH;
#elif defined(CONFIG_ENV_NAND)
	FT_NAND;
#elif defined(CONFIG_ENV_EMMC)
	FT_EMMC;
#else
	FT_NONE;
#endif

	if (flashtype != FT_NONE)
		return flashtype;

	switch (get_bootmedia(NULL, NULL)) {
	default:
	case BOOT_MEDIA_UNKNOWN:  flashtype = FT_NONE;     break;
	case BOOT_MEDIA_NAND:     flashtype = FT_NAND;     break;
	case BOOT_MEDIA_SPI_NAND: flashtype = FT_NAND;     break;
	case BOOT_MEDIA_SPIFLASH: flashtype = FT_SPIFLASH; break;
	case BOOT_MEDIA_EMMC:     flashtype = FT_EMMC;     break;
	}

	return flashtype;
}
/*****************************************************************************/

static struct flash_intf_t *open_env_flash(enum flash_type_t flashtype,
					   uint32 from, char *macro)
{
	uint32 blockmask;
	struct flash_intf_t *flashif;
	const struct flash_info_t *flashinfo;

	if (flashtype == FT_NONE)
		return NULL;

	flashinfo = flash_get_info(flashtype);
	if (!flashinfo) {
		pr_warn("Not found %s, Env initiate fail.\n",
			flash_get_name(flashtype));
		return NULL;
	}

	blockmask = flashinfo->blocksize - 1;

	if (from & blockmask) {
		pr_warn("Env is in %s, %s(0x%x) does not align to blocksize(0x%x).\n",
			flash_get_name(flashtype),
			macro, CONFIG_ENV_FROM,
			flashinfo->blocksize);
		return NULL;
	}

	if (CONFIG_ENV_RANGE & blockmask) {
		pr_warn("Env is in %s, CONFIG_ENV_RANGE(0x%x) does not align to blocksize(0x%x) \n",
			flash_get_name(flashtype),
			CONFIG_ENV_RANGE,
			flashinfo->blocksize);
		return NULL;
	}

	flashif = flash_open_range(flashtype, (uint64)from,
				   (uint64)CONFIG_ENV_RANGE);
	if (!flashif) {
		pr_warn("Open flash %s fail.\n", flash_get_name(flashtype));
		return NULL;
	}

	return flashif;
}
/*****************************************************************************/

static int verify_env(struct env_t *env)
{
	if (env->crc32 != crc32(0, env->data, sizeof(env->data))) {
		fdbgout("env CRC32 check fail.\n");
		return FALSE;
	}
	return TRUE;
}
/*****************************************************************************/

static int read_env_flash(struct flash_intf_t *flashif, struct env_t *env)
{
	int ret;
	char *ptr;
	int stub, round;
	const struct flash_info_t *finfo = flashif->info;

	ASSERT(flashif != NULL && env != NULL);

	ptr = (char *)env;
	round = (CONFIG_ENV_SIZE / finfo->pagesize) * finfo->pagesize;
	stub = CONFIG_ENV_SIZE % finfo->pagesize;

	fdbgout("round:0x%x, stub:0x%x\n", round, stub);
	ret = (int)flash_read(flashif, 0, (uint64)round, ptr, FALSE);
	if (ret != round) {
		pr_warn("Read Env fail.\n");
		return -EIO;
	}

	if (stub) {
		char *buff = malloc(finfo->pagesize);
		if (!buff) {
			pr_error("Out of memory.\n");
			return -ENOMEM;
		}
		ret = (int)flash_read(flashif, (uint64)round,
			(uint64)finfo->pagesize, buff, FALSE);
		if (ret != (int)finfo->pagesize) {
			pr_warn("Read Env fail.\n");
			free(buff);
			return -EIO;
		}
		memcpy(ptr + round, buff, stub);
		free(buff);
	}

	if (!verify_env(env)) {
		pr_warn("Env Verify fail.\n");
		return -EINVAL;
	}

	global->env_tail = find_env_tail(env->data, sizeof(env->data));

	return 0;
}
/*****************************************************************************/

static int load_backup_flash(enum flash_type_t flashtype, struct env_t *env)
{
	int ret;
	struct flash_intf_t *flashif;

	if (CONFIG_ENV_BACKUP_FROM == CONFIG_ENV_FROM)
		return -ENOENT;

	flashif = open_env_flash(flashtype, CONFIG_ENV_BACKUP_FROM,
		"CONFIG_ENV_BACKUP_FROM");
	if (!flashif)
		return -ENOENT;

	pr_info("Load backup Env from %s, start:0x%x, size:0x%x\n",
		flash_get_name(flashtype),
		CONFIG_ENV_BACKUP_FROM,
		CONFIG_ENV_SIZE);

	ret = read_env_flash(flashif, env);

	flash_close(flashif);

	return ret;
}
/*****************************************************************************/

static int load_default(struct env_t *env)
{
	pr_info("Load default Env.\n");

	global->env_tail = env->data;
	memset(env, 0, sizeof(struct env_t));

#ifdef CONFIG_DEFAULT_BOOTCMD
	env_set("bootcmd", CONFIG_DEFAULT_BOOTCMD);
#endif
	env_set("ipaddr", "192.168.1.10");
	env_set("serverip", "192.168.1.1");
	env_set("netmask", "255.255.255.0");
	env_set("ethaddr", "00:43:59:5c:00:01");
	env_set("bootdelay", "1");

	return 0;
}

/*****************************************************************************/

static void env_init(void *ptr)
{
	int ret;
	struct env_t *env;
	struct flash_intf_t *flashif;
	enum flash_type_t flashtype = get_env_media();

	ASSERT(sizeof(struct global_t) < (uint32)(__global_end - __global_start));

	printf("\n");

	global = (struct global_t *)__global_start;

	flashif = global->flashif
		= open_env_flash(flashtype, CONFIG_ENV_FROM,
				 "CONFIG_ENV_FROM");

	env = &global->env;

	memset(env, 0, CONFIG_ENV_SIZE);
#if defined(CONFIG_PRODUCT_WITH_BOOT) && defined(CONFIG_SUPPORT_CA)
	if (!load_direct_env(env, CONFIG_ENV_FROM, CONFIG_ENV_SIZE)) {
		global->env_tail = find_env_tail(env->data, sizeof(env->data));
	} 
#ifdef CONFIG_ENV_BACKUP_FROM
	else {
		if (!load_direct_env(env, CONFIG_ENV_BACKUP_FROM, CONFIG_ENV_SIZE)) {	
			global->env_tail = find_env_tail(env->data, sizeof(env->data));
		}
	}
#endif
#endif
	if (verify_env(env)) {
		pr_info("Found Env in DDR address:0x%08x\n", (uint32)env);
		return;
	}

	if (flashif) {
		pr_info("Load Env from %s, start:0x%x, size:0x%x\n",
			 flash_get_name(flashtype),
			 CONFIG_ENV_FROM,
			 CONFIG_ENV_SIZE);
		ret = read_env_flash(flashif, env);
		if (!ret)
			return;
	}

	ret = load_backup_flash(flashtype, env);
	if (!ret)
		return;

	load_default(env);
}
/*****************************************************************************/

static void env_exit(void *priv)
{
	struct env_t *env = &global->env;
	env->crc32 = crc32(0, env->data, sizeof(env->data));
}
/*****************************************************************************/

static struct module_t module_evn_set = {
	.priority = MODULE_PRI_ENV,
	.init = env_init,
	.exit = env_exit,
};
MODULE(module_evn_set);
