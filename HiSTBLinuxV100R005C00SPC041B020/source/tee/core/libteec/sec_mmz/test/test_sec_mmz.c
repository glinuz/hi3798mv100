#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>


#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_app_load.h"

#include <sec_mmz.h>
#include <sec_mmz_common.h>
#include <drv_mmz_ioctl.h>

static const TEEC_UUID sec_mmz_uuid =
{
	SEC_MMZ_UUID
};

#define SEC_MMZ_ASSERT_PRINT(cond, fmt, arg...) \
		if (!(cond)) { \
			printf("[%s, %d]:" fmt "\n", __func__, __LINE__, ##arg); \
			while(1); \
		}

#define SEC_MMZ_ASSERT(cond) \
		SEC_MMZ_ASSERT_PRINT(cond, "%s", "assert");

#define sec_mmz_printf(fmt, arg...) \
	printf("[%s, %d]:" fmt "\n", __func__, __LINE__, ##arg);


#define TEST_MEM_SIZE ((2<<20) - 1)
#define SEC_MMZ_ZONE "SEC-MMZ"

extern int HI_SEC_MMZ_TA2CA(unsigned long TAphyaddr, unsigned long CAphyaddr, unsigned long CopySize);

static int nonsecmmzdev = -1;
static TEEC_Context sec_mmz_context;
static TEEC_Session sec_mmz_session;

void *alloc_testbuffer(size_t size, void **phyaddr)
{
	struct mmb_info mmi = {0};
	int ret = 0;

	nonsecmmzdev = open("/dev/mmz_userdev", O_RDWR);
	SEC_MMZ_ASSERT(nonsecmmzdev >= 0);

	mmi.size = TEST_MEM_SIZE;
	mmi.align = 0x100000;
	strncpy(mmi.mmb_name, "secmmzmmb", HIL_MMB_NAME_LEN - 1);
	mmi.mmb_name[HIL_MMB_NAME_LEN - 1] = '\0';

	ret = ioctl(nonsecmmzdev, IOC_MMB_ALLOC, &mmi);
	SEC_MMZ_ASSERT(!ret);
	mmi.prot = PROT_READ | PROT_WRITE;
	mmi.flags = MAP_SHARED;
	ret = ioctl(nonsecmmzdev, IOC_MMB_USER_REMAP, &mmi);
	SEC_MMZ_ASSERT(!ret);

	close(nonsecmmzdev);
	if (phyaddr)
		*phyaddr = (void *)(mmi.phys_addr);

	return mmi.mapped;
}

void free_testbuffer(void*virtaddr, void *phyaddr)
{
	struct mmb_info mmi;
	int ret = 0;

	if (nonsecmmzdev == -1) {
		nonsecmmzdev = open("/dev/mmz_userdev", O_RDWR);
		SEC_MMZ_ASSERT(nonsecmmzdev >= 0);
	}


	mmi.phys_addr = (unsigned long)phyaddr;
	mmi.mapped = virtaddr;

	ioctl(nonsecmmzdev, IOC_MMB_USER_UNMAP, &mmi);

	/*call ioctl to free*/
	ioctl(nonsecmmzdev, IOC_MMB_FREE, &mmi);
}
int test_init_deinit(void)
{
	int i;
	int ret = 0;
	void *pa1 = NULL;
	void *pa2 = NULL;

	sec_mmz_printf("start\n");
	for (i = 0; i < 1000; i++) {
		ret = HI_SEC_MMZ_Init();
		if (ret < 0)
			sec_mmz_printf("function return %d\n", ret);

		SEC_MMZ_ASSERT(!ret);

		HI_SEC_MMZ_DeInit();
	}

	ret = HI_SEC_MMZ_Init();
		if (ret < 0)
			sec_mmz_printf("function return %d\n", ret);

	pa1 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa1");
	pa2 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa2");

	SEC_MMZ_ASSERT(pa1);
	SEC_MMZ_ASSERT(pa2);

	ret = HI_SEC_MMZ_Delete((unsigned long)pa1);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	ret = HI_SEC_MMZ_Delete((unsigned long)pa2);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	HI_SEC_MMZ_DeInit();

	return 0;
}

int test_new_delete(void)
{
	int i;
	int ret = 0;
	void *pa1 = NULL;
	void *pa2 = NULL;

	sec_mmz_printf("start\n");
	ret = HI_SEC_MMZ_Init();
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);

	for (i = 0; i < 2000; i++) {
		pa1 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa1");
		pa2 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa2");

		SEC_MMZ_ASSERT(pa1);
		SEC_MMZ_ASSERT(pa2);
		SEC_MMZ_ASSERT(pa1 != pa2);
		//sec_mmz_printf("pa1=%p, pa2=%p\n", pa1, pa2);

		ret = HI_SEC_MMZ_Delete((unsigned long)pa1);
		if (ret < 0)
			sec_mmz_printf("function return %d\n", ret);
		SEC_MMZ_ASSERT(!ret);

		ret = HI_SEC_MMZ_Delete((unsigned long)pa2);
		if (ret < 0)
			sec_mmz_printf("function return %d\n", ret);
		SEC_MMZ_ASSERT(!ret);
	}

	HI_SEC_MMZ_DeInit();

	return 0;
}

int test_catota(void)
{
	void *buf1  = NULL;
	void *buf1_phyaddr = NULL;
	void *buf2 = NULL;
	void *buf2_phyaddr = NULL;
	void *pa1  = NULL;
	int ret = 0;

	sec_mmz_printf("start\n");
	ret = HI_SEC_MMZ_Init();
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);

	SEC_MMZ_ASSERT(!ret);

	pa1 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa1");

	sec_mmz_printf("pa1 = %p\n", pa1);

	SEC_MMZ_ASSERT(pa1);

	buf1 = alloc_testbuffer(TEST_MEM_SIZE, &buf1_phyaddr);
	SEC_MMZ_ASSERT(buf1);
	buf2 = alloc_testbuffer(TEST_MEM_SIZE, &buf2_phyaddr);
	SEC_MMZ_ASSERT(buf2);

	memset(buf1, 0x5A, TEST_MEM_SIZE);
	memset(buf2, 0x5A, TEST_MEM_SIZE);

	ret = HI_SEC_MMZ_CA2TA((unsigned long)buf1_phyaddr, (unsigned long)pa1, TEST_MEM_SIZE);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);
//#define CONFIG_SEC_MMZ_TA2CA
#ifndef CONFIG_SEC_MMZ_TA2CA
	sec_mmz_printf("Warning: TA2CA is not supported, we cannot check the data!! If you want to check the data, define CONFIG_SEC_MMZ_TA2CA!!\n");

	goto out;
#else
	ret = HI_SEC_MMZ_TA2CA((unsigned long)pa1, (unsigned long)buf2_phyaddr, TEST_MEM_SIZE);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	ret = memcmp(buf1, buf2, TEST_MEM_SIZE);
	if (ret != 0)
		sec_mmz_printf("diff at %d\n", ret);
	SEC_MMZ_ASSERT(!ret);
#endif
out:
	ret = HI_SEC_MMZ_Delete((unsigned long)pa1);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	HI_SEC_MMZ_DeInit();

	free_testbuffer(buf1, buf1_phyaddr);
	free_testbuffer(buf2, buf2_phyaddr);
	return 0;
}

int test_ta2ta(void)
{
	void *pa1 = 0;
	void *pa2 = 0;
	void *buf1  = NULL;
	void *buf1_phyaddr = NULL;
	void *buf2 = NULL;
	void *buf2_phyaddr = NULL;
	int i;
	int ret = 0;

	sec_mmz_printf("start\n");
	ret = HI_SEC_MMZ_Init();
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);

	SEC_MMZ_ASSERT(!ret);

	pa1 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa1");
	pa2 = HI_SEC_MMZ_New(TEST_MEM_SIZE, SEC_MMZ_ZONE, "aaa2");

	sec_mmz_printf("pa1 = %p\n", pa1);
	sec_mmz_printf("pa2 = %p\n", pa2);

	SEC_MMZ_ASSERT(pa1);
	SEC_MMZ_ASSERT(pa2);

	buf1 = alloc_testbuffer(TEST_MEM_SIZE, &buf1_phyaddr);
	SEC_MMZ_ASSERT(buf1);
	buf2 = alloc_testbuffer(TEST_MEM_SIZE, &buf2_phyaddr);
	SEC_MMZ_ASSERT(buf2);

	memset(buf1, 0x5A, TEST_MEM_SIZE);

	ret = HI_SEC_MMZ_CA2TA((unsigned long)buf1_phyaddr, (unsigned long)pa1, TEST_MEM_SIZE);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	memset(buf2, 0xA5, TEST_MEM_SIZE);
	ret = HI_SEC_MMZ_CA2TA((unsigned long)buf2_phyaddr, (unsigned long)pa2, TEST_MEM_SIZE);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	ret = HI_SEC_MMZ_TA2TA((unsigned long)pa1, (unsigned long)pa2, TEST_MEM_SIZE);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

#ifndef CONFIG_SEC_MMZ_TA2CA
	sec_mmz_printf("Warning: TA2CA is not supported, we cannot check the data!! If you want to check the data, define CONFIG_SEC_MMZ_TA2CA!!\n");
	goto out;
#else
	memset(buf2, 0xFF, TEST_MEM_SIZE);
	ret = HI_SEC_MMZ_TA2CA((unsigned long)pa2, (unsigned long)buf2_phyaddr, TEST_MEM_SIZE);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	ret = memcmp(buf1, buf2, TEST_MEM_SIZE);
	if (ret != 0)
		sec_mmz_printf("diff at %d\n", ret);
	SEC_MMZ_ASSERT(!ret);
#endif
out:
	ret = HI_SEC_MMZ_Delete((unsigned long)pa1);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	ret = HI_SEC_MMZ_Delete((unsigned long)pa2);
	if (ret < 0)
		sec_mmz_printf("function return %d\n", ret);
	SEC_MMZ_ASSERT(!ret);

	HI_SEC_MMZ_DeInit();

	free_testbuffer(buf1, buf1_phyaddr);
	free_testbuffer(buf2, buf2_phyaddr);
	return 0;
}

int test_sec_world_mmzapi(void)
{
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t cmd = 0;
	uint32_t origin;
	int32_t ret = 0;

	result = TEEC_InitializeContext(NULL, &sec_mmz_context);
	if(result != TEEC_SUCCESS) {
		TEEC_Error("teec initial failed\n");
		goto error1;
	}

	result = TEEC_OpenSession(&sec_mmz_context, &sec_mmz_session, &sec_mmz_uuid,
				TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
	if(result != TEEC_SUCCESS) {
		TEEC_Error("teec open sec_mmz_session failed\n");
		goto error2;
	}

	operation.started = 1;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE,
				TEEC_NONE,
				TEEC_NONE,
				TEEC_NONE);

	result = TEEC_InvokeCommand(&sec_mmz_session, HI_MMZ_TEST, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = ERR_IN_SEC_OS;
		goto error3;
	}

	TEEC_CloseSession(&sec_mmz_session);
	TEEC_FinalizeContext(&sec_mmz_context);
	return 0;
error3:
	TEEC_CloseSession(&sec_mmz_session);
error2:
	TEEC_FinalizeContext(&sec_mmz_context);
error1:
	return ret;
}

int main(void)
{
	sec_mmz_printf("Sec MMZ testing start!\n");
	SEC_MMZ_ASSERT(!test_init_deinit());
	SEC_MMZ_ASSERT(!test_new_delete());
	SEC_MMZ_ASSERT(!test_catota());
	SEC_MMZ_ASSERT(!test_ta2ta());
	SEC_MMZ_ASSERT(!test_sec_world_mmzapi());
	return 0;
}
