#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_app_load.h"

#include "sec_mmz.h"
#include <sec_mmz_common.h>

#define SUCCESS 0
#define ERR_IN_SEC_OS -1
#define NO_SEC_OS -2

static TEEC_Context sec_mmz_context;
static TEEC_Session sec_mmz_session;
static unsigned int inited = 0;

static const TEEC_UUID sec_mmz_uuid =
{
	SEC_MMZ_UUID
};

int HI_SEC_MMZ_Init(void)
{
//TODO: XXX
#define HI_TVP_SUPPORT
#ifdef HI_TVP_SUPPORT
	TEEC_Result result;

	if(inited > 0) {
		inited ++;
	} else {
		result = TEEC_InitializeContext(NULL, &sec_mmz_context);
		if(result != TEEC_SUCCESS) {
			TEEC_Error("teec initial failed\n");
			goto cleanup_1;
		}

		result = TEEC_OpenSession(&sec_mmz_context, &sec_mmz_session, &sec_mmz_uuid,
					TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
		if(result != TEEC_SUCCESS) {
			TEEC_Error("teec open sec_mmz_session failed\n");
			goto cleanup_2;
		}

		inited ++;
	}

	return 0;
cleanup_2:
	TEEC_FinalizeContext(&sec_mmz_context);
cleanup_1:
	return ERR_IN_SEC_OS;
#else
	return NO_SEC_OS;
#endif
}

void HI_SEC_MMZ_DeInit(void)
{
	if(inited > 1) {
		inited--;
	} else if(inited == 1) {
		TEEC_CloseSession(&sec_mmz_session);
		TEEC_FinalizeContext(&sec_mmz_context);
		inited = 0;
	}

	return;
}

void *HI_SEC_MMZ_New(unsigned long size , char *mmz_name, char *mmb_name)
{
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;
	void * pa = NULL;

	if (!mmz_name) {
		printf("NULL mmz name, not supported!\n");
		goto error;
	}

	if (!mmb_name) {
		printf("NULL mmb name, not supported!\n");
		goto error;
	}

	operation.started = 1;
	operation.params[0].tmpref.buffer = mmz_name;
	operation.params[0].tmpref.size = strlen(mmz_name) + 1;

	operation.params[1].tmpref.buffer = mmb_name;
	operation.params[1].tmpref.size = strlen(mmb_name) + 1;
	operation.params[2].value.a = size;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
				TEEC_MEMREF_TEMP_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_OUTPUT);

	result = TEEC_InvokeCommand(&sec_mmz_session, HI_MMZ_NEW, &operation, &origin);

	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
	} else {
		pa = (void *)operation.params[3].value.a;
	}

	return pa;
error:
	return NULL;
}

int HI_SEC_MMZ_Delete(unsigned long phys_addr)
{
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;
	int32_t ret = 0;

	operation.started = 1;
	operation.params[0].value.a = phys_addr;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
				TEEC_NONE,
				TEEC_NONE,
				TEEC_NONE);

	result = TEEC_InvokeCommand(&sec_mmz_session, HI_MMZ_DEL, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = ERR_IN_SEC_OS;
		goto error;
	}

	return 0;
error:
	return ret;
}

int HI_SEC_MMZ_CA2TA(unsigned long CAphyaddr, unsigned long TAphyaddr, unsigned long CopySize)
{
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;
	uint32_t ret = 0;

	operation.started = 1;
	operation.params[0].value.a = CAphyaddr;
	operation.params[1].value.a = TAphyaddr;
	operation.params[2].value.a = CopySize;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_OUTPUT);

	result = TEEC_InvokeCommand(&sec_mmz_session, HI_MMZ_CP_CA2TA, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = ERR_IN_SEC_OS;
		goto error;
	}

	ret = operation.params[3].value.a;

	if(ret) {
		TEEC_Error("Error in copy!\n");
		goto error;
	}

	return 0;
error:
	return ret;
}
//TODO: XXX
//#define CONFIG_SEC_MMZ_TA2CA
#ifdef CONFIG_SEC_MMZ_TA2CA
int HI_SEC_MMZ_TA2CA(unsigned long TAphyaddr, unsigned long CAphyaddr, unsigned long CopySize)
{
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;
	uint32_t ret = 0;

	operation.started = 1;
	operation.params[0].value.a = TAphyaddr;
	operation.params[1].value.a = CAphyaddr;
	operation.params[2].value.a = CopySize;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_NONE);

	result = TEEC_InvokeCommand(&sec_mmz_session, HI_MMZ_CP_TA2CA, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = ERR_IN_SEC_OS;
		goto error;
	}

	return 0;
error:
	return ret;
}
#endif

int HI_SEC_MMZ_TA2TA(unsigned long phyaddr1, unsigned long phyaddr2, unsigned long CopySize)
{
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;
	uint32_t ret = 0;

	operation.started = 1;
	operation.params[0].value.a = phyaddr1;
	operation.params[1].value.a = phyaddr2;
	operation.params[2].value.a = CopySize;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_INPUT,
				TEEC_VALUE_OUTPUT);

	result = TEEC_InvokeCommand(&sec_mmz_session, HI_MMZ_CP_TA2TA, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		ret = ERR_IN_SEC_OS;
		goto error;
	}

	ret = operation.params[3].value.a;

	if(ret) {
		TEEC_Error("Error in copy!\n");
		goto error;
	}

	return 0;
error:
	return ret;
}
