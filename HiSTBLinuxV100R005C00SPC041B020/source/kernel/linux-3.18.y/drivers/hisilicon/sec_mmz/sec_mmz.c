
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/smp.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>

//#define TEEC_DEBUG
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "sec_mmz.h"

static const TEEC_UUID COMMON_uuid =
{
	0x0C0C0C0C, 0x0C0C, 0x0C0C,
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C }
};

static TEEC_Context context;
static TEEC_Session session;
static __maybe_unused unsigned int dev_id;
static int sec_mmz_init = 0;
extern void sec_mmz_test_main(void);
extern void dump_stack(void);

int HI_SEC_MMZ_Init(void)
{
	TEEC_Result result;
	TEEC_Operation operation;
	u8 package_name[] = "hisi_secmmz";
	u32 root_id = 0;

	if (sec_mmz_init) {
		printk(KERN_WARNING "Sec mmz has already been initialized!\n");
		dump_stack();
		return -1;
	}

	printk(KERN_INFO "Sec mmz start to init\n");
	result = TEEC_InitializeContext(NULL, &context, &dev_id);
	if(result != TEEC_SUCCESS) {
		TEEC_Error("TEEC_InitializeContext failed, ret=0x%x.", result);
		goto cleanup_1;
	}
	memset(&operation, 0x0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_NONE,
			TEEC_NONE,
			TEEC_MEMREF_TEMP_INPUT,
			TEEC_MEMREF_TEMP_INPUT);
	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = sizeof(root_id);
	operation.params[3].tmpref.buffer = (void *)(package_name);
	operation.params[3].tmpref.size = strlen(package_name) + 1;
	result = TEEC_OpenSession(&context, &session, &COMMON_uuid, TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL, dev_id);
	if(result != TEEC_SUCCESS) {
		TEEC_Error("TEEC_OpenSession failed, ret=0x%x.", result);
		goto cleanup_2;
	}

	printk(KERN_INFO "Sec mmz initialized.\n");
	sec_mmz_init++;
	return 0;

cleanup_2:
	TEEC_FinalizeContext(&context,dev_id);
cleanup_1:
	return -1;
}

int secmmz_enabled = 1;
static int __init secmmz_disable(char *str)
{
	secmmz_enabled = 0;
	return 1;
}
__setup("no_secmmz", secmmz_disable);

int SecMMZ_Init(void)
{

	if (!secmmz_enabled) {
		printk(KERN_INFO "Secure MMZ Disabled!\n");
		return 0;
	}

#ifdef CONFIG_SEC_MMZ_TEST
	sec_mmz_test_main();
#endif
	if (0 != HI_SEC_MMZ_Init()) {
		printk(KERN_ERR "Sec mmz init fail!\n");
		return -1;
	}

	return 0;
}

void HI_SEC_MMZ_DeInit(void)
{
	if (sec_mmz_init <= 0) {
		printk(KERN_WARNING "Sec mmz has NOT been initialized!\n");
		dump_stack();
		return;
	}

	TEEC_CloseSession(&session,dev_id);
	TEEC_FinalizeContext(&context,dev_id);

	if (sec_mmz_init > 0)
		sec_mmz_init--;

	printk(KERN_INFO "Sec mmz deinit.\n");
}

void SecMMZ_DeInit(void)
{
	HI_SEC_MMZ_DeInit();
}

void *HI_SEC_MMZ_New(unsigned long size , char *mmz_name, char *mmb_name)
{
	TEEC_Result result;
	TEEC_Operation operation;

	void * pa = 0;

	if (!mmz_name)
	{
		TEEC_Error("NULL mmz name, not supported!\n");
		return NULL;
	}

	if (!mmb_name)
	{
		TEEC_Error("NULL mmb name, not supported!\n");
		return NULL;
	}

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;

	operation.params[0].tmpref.buffer = mmz_name;
	operation.params[0].tmpref.size = strlen(mmz_name) + 1;

	operation.params[1].tmpref.buffer = mmb_name;
	operation.params[1].tmpref.size = strlen(mmb_name) + 1;
	operation.params[2].value.a = size;

	operation.paramTypes = TEEC_PARAM_TYPES(
				TEEC_MEMREF_TEMP_INPUT,
				TEEC_MEMREF_TEMP_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_OUTPUT);

	result = TEEC_InvokeCommand(&session, HI_MMZ_NEW, &operation, NULL, dev_id);
	if (result == TEEC_SUCCESS)
	{
		pa = (void *)operation.params[3].value.a;
		TEEC_Debug("new '%s'(phy_addr:0x%x, size:%lu) mmb from mmz(%s) successful.\n", mmb_name, (unsigned int)pa, size, mmz_name);
	} else {
		pa = NULL;
		TEEC_Error("new '%s'(size:%lu) mmb from mmz(%s) failed, ret=0x%x.\n", mmb_name, size, mmz_name, result);
	}

	return pa;
}

int HI_SEC_MMZ_Delete(unsigned long phys_addr)
{
	TEEC_Result result;
	TEEC_Operation operation;
	int ret  = -1;

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.params[0].value.a = phys_addr;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
				TEEC_NONE,
				TEEC_NONE,
				TEEC_NONE);

	result = TEEC_InvokeCommand(&session, HI_MMZ_DEL, &operation, NULL, dev_id);
	if (result != TEEC_SUCCESS)
	{
		TEEC_Error("delete mmb(phy_addr:0x%lx) failed, ret=0x%x.\n", phys_addr, result);
		ret = -1;
	}
	else
	{
		TEEC_Debug("delete mmb(phy_addr:0x%lx) successful.\n", phys_addr);
		ret = 0;
	}

	return ret;
}

int HI_SEC_MMZ_CA2TA(unsigned long CAphyaddr, unsigned long TAphyaddr, unsigned long CopySize)
{
	TEEC_Result result;
	TEEC_Operation operation;
	int ret = -1;

	operation.started = 1;
	operation.params[0].value.a = CAphyaddr;
	operation.params[1].value.a = TAphyaddr;
	operation.params[2].value.a = CopySize;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_NONE);


	result = TEEC_InvokeCommand(&session, HI_MMZ_CP_CA2TA, &operation, NULL, dev_id);
	if (result != TEEC_SUCCESS)
	{
		TEEC_Error("trans CAphy(0x%lx) to TAphy(0x%lx) failed, ret=0x%x.\n", CAphyaddr, TAphyaddr, result);
		ret = -1;
	}
	else
	{
		TEEC_Debug("trans CAphy(0x%lx) to TAphy(0x%lx) successful.\n", CAphyaddr, TAphyaddr);
		ret = 0;
	}

	return ret;
}

#ifdef CONFIG_SEC_MMZ_TA2CA
int HI_SEC_MMZ_TA2CA(unsigned long TAphyaddr, unsigned long CAphyaddr, unsigned long CopySize)
{
	TEEC_Result result;
	TEEC_Operation operation;

	operation.started = 1;
	operation.params[0].value.a = TAphyaddr;
	operation.params[1].value.a = CAphyaddr;
	operation.params[2].value.a = CopySize;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_NONE);


	result = TEEC_InvokeCommand(
				&session,
				HI_MMZ_CP_TA2CA,
				&operation,
				NULL,
				dev_id);

	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x\n", result);
	}

	return 0;
}
#endif

int HI_SEC_MMZ_TA2TA(unsigned long phyaddr1, unsigned long phyaddr2, unsigned long CopySize)
{
	TEEC_Result result;
	TEEC_Operation operation;
	int ret = -1;

	operation.started = 1;
	operation.params[0].value.a = phyaddr1;
	operation.params[1].value.a = phyaddr2;
	operation.params[2].value.a = CopySize;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_NONE);


	result = TEEC_InvokeCommand(&session, HI_MMZ_CP_TA2TA, &operation, NULL, dev_id);
	if (result != TEEC_SUCCESS)
	{
		TEEC_Error("trans TAphy(0x%lx) to TAphy(0x%lx) failed, ret=0x%x.\n", phyaddr1, phyaddr2, result);
		ret = -1;
	}
	else
	{
		TEEC_Debug("trans TAphy(0x%lx) to TAphy(0x%lx) successful.\n", phyaddr1, phyaddr2);
		ret = 0;
	}

	return ret;
}

EXPORT_SYMBOL(HI_SEC_MMZ_New);
EXPORT_SYMBOL(HI_SEC_MMZ_Delete);
EXPORT_SYMBOL(HI_SEC_MMZ_CA2TA);
#ifdef CONFIG_SEC_MMZ_TA2CA
EXPORT_SYMBOL(HI_SEC_MMZ_TA2CA);
#endif
EXPORT_SYMBOL(HI_SEC_MMZ_TA2TA);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hisilicon Secure MMZ Driver");

module_init(SecMMZ_Init);
module_exit(SecMMZ_DeInit);

