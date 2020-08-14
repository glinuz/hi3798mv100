/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by LaiYingJun
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

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <flash.h>
#include <cmd-words.h>

#define MOULE_NAME                     "Flash Tester: "
#include <debug.h>

#  define TEST_ERROR(_fmt, args...)          \
	printf(_RED "%s(%d): " _fmt _NONE, __FILE__, __LINE__, ##args)

#  define TEST_OUT(_fmt, args...) \
	printf(_YELLOW "%s(%d): " _fmt _NONE, __FILE__, __LINE__, ##args)

#  define RUN_CASE(num, case)  \
	printf("Run case %d ...", num);  \
	if (!(case(ptr))) {printf("OK!\n");} \
	else {printf("Failed!");}


#define PAD_VALUE      0x5A
#define TESTPART1_NAME "test1"
#define TESTPART1_SIZE 1*1024*1024
#define TESTPART2_NAME "test2"
#define TESTPART2_SIZE 1*1024*1024

/*****************************************************************************/

static int emmc_read_write(char *part, uint64 partsize, uint64 offset, uint64 size)
{
	char *buf = NULL;
	int64 ret = 0;
	int64 i = 0;
	
	ASSERT(part);

	struct flash_intf_t *handle = flash_open_part(FT_EMMC, part);
	if (!handle) {
		TEST_ERROR("Fail to open %s, you need to define %s(0x%X) partition first\n", part, part,  partsize);
		goto err;
	}


	buf = malloc(size);
	if (!buf) {
		TEST_ERROR("Fail to alloc buffer for %s\n", part);
		goto err;
	}

	memset(buf, PAD_VALUE, size);

	ret = flash_write(handle, offset, size, buf, 0);
	if (size != ret) {
			TEST_ERROR("Failt to write 0x%X bytes to %s, actual size 0x%X\n", size, part, ret);
			goto err;
	}

	memset(buf, 0xff, size);

	ret = flash_read(handle, offset, size, buf, 0);
	if (size != ret) {
		TEST_ERROR("Failt to read 0x%X bytes from %s, actual size 0x%X\n", size, part, ret);
		goto err;
	}
	
	for (i=0; i < size; i++) {
		if (buf[i] != PAD_VALUE) {
			TEST_ERROR("buf[%ll](0x%X) != 0x%X \n", i, PAD_VALUE);
			goto err;
		}
	}

	ret = 0;
out:
	if (handle) {
		flash_close(handle);
		handle = NULL;
	}
	if (buf) {
		free(buf);
		buf = NULL;
	}

	return ret;
err:
	ret = -1;
	goto out;
}
/*****************************************************************************/

static int flash_test_emmc_read_write(struct cmd_t *ptr)
{
	uint32 blocksize = 0;
	struct flash_info_t *emmc_info = NULL;

	ASSERT(!emmc_read_write(TESTPART1_NAME, TESTPART1_SIZE, 0, TESTPART1_SIZE));
	ASSERT(NULL != (emmc_info = (struct flash_info_t *)flash_get_info(FT_EMMC)));
	ASSERT(blocksize = emmc_info->blocksize);
	ASSERT(!emmc_read_write(TESTPART1_NAME, TESTPART1_SIZE, blocksize, TESTPART1_SIZE - blocksize));

	return  0;
}
/*****************************************************************************/

static int spiflash_read_write(char *part, uint64 partsize, uint64 offset, uint64 size)
{
	char *buf = NULL;
	int64 ret = 0;
	int64 i = 0;

	ASSERT(part);

	struct flash_intf_t *handle =  flash_open_part(FT_SPIFLASH, part);
	if (!handle) {
		TEST_ERROR("Fail to open %s, you need to define %s(0x%X) partition first\n", part, part,  partsize);
		goto err;
	}


	buf = malloc(size);
	if (!buf) {
		TEST_ERROR("Fail to alloc buffer for %s\n", part);
		goto err;
	}

	ret = flash_erase(handle, offset, size);
	if (size != ret) {
		TEST_ERROR("Failt to erase 0x%X bytes from %s, actual size 0x%X\n", size, part, ret);
		goto err;
	}

	memset(buf, PAD_VALUE, size);

	ret = flash_write(handle, offset, size, buf, 0);
	if (size != ret) {
		TEST_ERROR("Failt to write 0x%X bytes to %s, actual size 0x%X\n", size, part, ret);
		goto err;
	}

	memset(buf, 0xff, size);

	ret = flash_read(handle, offset, size, buf, 0);
	if (size != ret) {
		TEST_ERROR("Failt to read 0x%X bytes from %s, actual size 0x%X\n", size, part, ret);
		goto err;
	}

	for (i=0; i < size; i++) {
		if (buf[i] != PAD_VALUE) {
			TEST_ERROR("buf[%lld](0x%X) != 0x%X \n", i, buf[i], PAD_VALUE);
			goto err;
		}
	}


out:
	if (handle) {
		flash_close(handle);
		handle = NULL;
	}
	if (buf) {
		free(buf);
		buf = NULL;
	}

	return 0;
err:
	goto out;
}
/*****************************************************************************/

static int flash_test_spiflash_read_write(struct cmd_t *ptr)
{
	uint32 blocksize = 0;
	struct flash_info_t *spiflash_info = NULL;

	ASSERT(!spiflash_read_write(TESTPART2_NAME, TESTPART2_SIZE, 0, TESTPART2_SIZE));
	ASSERT(NULL != (spiflash_info = (struct flash_info_t *)flash_get_info(FT_SPIFLASH)));
	ASSERT(blocksize = spiflash_info->blocksize);
	ASSERT(!spiflash_read_write(TESTPART2_NAME, TESTPART2_SIZE, blocksize, TESTPART2_SIZE - blocksize));

	return  0;
}
/*****************************************************************************/

static int flash_test_spiflash_to_emmc_read_write(struct cmd_t *ptr)
{
	char *buf = NULL;
	int64 ret = 0;
	int64 i = 0;

	if (TESTPART1_SIZE != TESTPART2_SIZE) {
		TEST_OUT("TESTPART1_SIZE(0x%X) should equal to TESTPART2_SIZE(0x%X)\n", TESTPART1_SIZE, TESTPART2_SIZE);
		goto err;
	}

	struct flash_intf_t *handle_spiflash =  flash_open_part(FT_SPIFLASH, TESTPART2_NAME);
	if (!handle_spiflash) {
		TEST_ERROR("Fail to open %s, you need to define %s(0x%X) partition first\n", TESTPART2_NAME, TESTPART2_NAME,  TESTPART2_SIZE);
		goto err;
	}


	buf = malloc(TESTPART2_SIZE);
	if (!buf) {
		TEST_ERROR("Fail to alloc buffer for %s\n", TESTPART2_NAME);
		goto err;
	}

	ret = flash_erase(handle_spiflash, 0, TESTPART2_SIZE);
	if (TESTPART2_SIZE != ret) {
		TEST_ERROR("Failt to erase 0x%X bytes from %s, actual size 0x%X\n", TESTPART2_SIZE, TESTPART2_NAME, ret);
		goto err;
	}

	memset(buf, PAD_VALUE, TESTPART2_SIZE);

	ret = flash_write(handle_spiflash, 0, TESTPART2_SIZE, buf, 0);
	if (TESTPART2_SIZE != ret) {
		TEST_ERROR("Failt to write 0x%X bytes to %s, actual size 0x%X\n", TESTPART2_SIZE, TESTPART2_NAME, ret);
		goto err;
	}

	memset(buf, 0xff, TESTPART2_SIZE);

	ret = flash_read(handle_spiflash, 0, TESTPART2_SIZE, buf, 0);
	if (TESTPART2_SIZE != ret) {
		TEST_ERROR("Failt to read 0x%X bytes from %s, actual size 0x%X\n", TESTPART2_SIZE, TESTPART1_NAME, ret);
		goto err;
	}

	for (i=0; i < TESTPART2_SIZE; i++) {
		if (buf[i] != PAD_VALUE) {
			TEST_ERROR("buf[%lld](0x%X) != 0x%X \n", i, buf[i], PAD_VALUE);
			goto err;
		}
	}

	struct flash_intf_t *handle_emmc =  flash_open_part(FT_EMMC, TESTPART1_NAME);
	if (!handle_emmc) {
		TEST_ERROR("Fail to open %s, you need to define %s(0x%X) partition first\n", TESTPART1_NAME, TESTPART1_NAME,  TESTPART1_SIZE);
		goto err;
	}

	ret = flash_write(handle_emmc, 0, TESTPART1_SIZE, buf, 0);
	if (TESTPART1_SIZE != ret) {
		TEST_ERROR("Failt to write 0x%X bytes to %s, actual size 0x%X\n", TESTPART1_SIZE, TESTPART1_NAME, ret);
		goto err;
	}

	memset(buf, 0xff, TESTPART1_SIZE);

	ret = flash_read(handle_emmc, 0, TESTPART1_SIZE, buf, 0);
	if (TESTPART1_SIZE != ret) {
		TEST_ERROR("Failt to read 0x%X bytes from %s, actual size 0x%X\n", TESTPART1_SIZE, TESTPART1_NAME, ret);
		goto err;
	}

	for (i=0; i < TESTPART1_SIZE; i++) {
		if (buf[i] != PAD_VALUE) {
			TEST_ERROR("buf[%ll](0x%X) != 0x%X \n", i, PAD_VALUE);
			goto err;
		}
	}
	

out:
	if (handle_spiflash)
		flash_close(handle_spiflash);
	if (handle_spiflash)
		flash_close(handle_emmc);
	if (buf) {
		free(buf);
	}

	return 0;
err:
	goto out;
}
/*****************************************************************************/

static int flash_test_emmc_to_spiflash_read_write(struct cmd_t *ptr)
{
	char *buf = NULL;
	int64 ret = 0;
	int64 i = 0;

	if (TESTPART1_SIZE != TESTPART2_SIZE) {
		TEST_OUT("TESTPART1_SIZE(0x%X) should equal to TESTPART2_SIZE(0x%X)\n", TESTPART1_SIZE, TESTPART2_SIZE);
		goto err;
	}


	struct flash_intf_t *handle_emmc =  flash_open_part(FT_EMMC, TESTPART1_NAME);
	if (!handle_emmc) {
		TEST_ERROR("Fail to open %s, you need to define %s(0x%X) partition first\n", TESTPART1_NAME, TESTPART1_NAME,  TESTPART1_SIZE);
		goto err;
	}

	buf = malloc(TESTPART1_SIZE);
	if (!buf) {
		TEST_ERROR("Fail to alloc buffer for %s\n", TESTPART1_NAME);
		goto err;
	}

	memset(buf, PAD_VALUE, TESTPART1_SIZE);
	ret = flash_write(handle_emmc, 0, TESTPART1_SIZE, buf, 0);
	if (TESTPART1_SIZE != ret) {
		TEST_ERROR("Failt to write 0x%X bytes to %s, actual size 0x%X\n", TESTPART1_SIZE, TESTPART1_NAME, ret);
		goto err;
	}

	memset(buf, 0xff, TESTPART1_SIZE);

	ret = flash_read(handle_emmc, 0, TESTPART1_SIZE, buf, 0);
	if (TESTPART1_SIZE != ret) {
		TEST_ERROR("Failt to read 0x%X bytes from %s, actual size 0x%X\n", TESTPART1_SIZE, TESTPART1_NAME, ret);
		goto err;
	}

	for (i=0; i < TESTPART1_SIZE; i++) {
		if (buf[i] != PAD_VALUE) {
			TEST_ERROR("buf[%ll](0x%X) != 0x%X \n", i, PAD_VALUE);
			goto err;
		}
	}

	struct flash_intf_t *handle_spiflash =  flash_open_part(FT_SPIFLASH, TESTPART2_NAME);
	if (!handle_spiflash) {
		TEST_ERROR("Fail to open %s, you need to define %s(0x%X) partition first\n", TESTPART2_NAME, TESTPART2_NAME,  TESTPART2_SIZE);
		goto err;
	}


	ret = flash_erase(handle_spiflash, 0, TESTPART2_SIZE);
	if (TESTPART2_SIZE != ret) {
		TEST_ERROR("Failt to erase 0x%X bytes from %s, actual size 0x%X\n", TESTPART2_SIZE, TESTPART2_NAME, ret);
		goto err;
	}

	ret = flash_write(handle_spiflash, 0, TESTPART2_SIZE, buf, 0);
	if (TESTPART2_SIZE != ret) {
		TEST_ERROR("Failt to write 0x%X bytes to %s, actual size 0x%X\n", TESTPART2_SIZE, TESTPART2_NAME, ret);
		goto err;
	}

	memset(buf, 0xff, TESTPART2_SIZE);

	ret = flash_read(handle_spiflash, 0, TESTPART2_SIZE, buf, 0);
	if (TESTPART2_SIZE != ret) {
		TEST_ERROR("Failt to read 0x%X bytes from %s, actual size 0x%X\n", TESTPART2_SIZE, TESTPART1_NAME, ret);
		goto err;
	}

	for (i=0; i < TESTPART2_SIZE; i++) {
		if (buf[i] != PAD_VALUE) {
			TEST_ERROR("buf[%lld](0x%X) != 0x%X \n", i, buf[i], PAD_VALUE);
			goto err;
		}
	}


out:
	if (handle_spiflash)
		flash_close(handle_spiflash);
	if (handle_spiflash)
		flash_close(handle_emmc);
	if (buf) {
		free(buf);
	}

	return 0;
err:
	goto out;
}
/*****************************************************************************/

static int do_flash_test(struct cmd_t *ptr)
{
	RUN_CASE(1, flash_test_emmc_read_write);
	RUN_CASE(2, flash_test_spiflash_read_write);
	RUN_CASE(3, flash_test_spiflash_to_emmc_read_write);
	RUN_CASE(4, flash_test_emmc_to_spiflash_read_write);

	return 0;
}
/*****************************************************************************/
struct cmd_word_t cw_flash = {
        "flash", CMDWORD_TYPE_KEYWORD, _T("flashtest")
};

CMD({&cw_test, NULL},
    {&cw_flash, _T("flash test case"), do_flash_test})

