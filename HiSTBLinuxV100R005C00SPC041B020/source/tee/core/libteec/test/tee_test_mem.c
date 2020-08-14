#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include "tee_client_api.h"
#include "tee_client_id.h"

uint8_t *tee_output = "MEMREF_OUTPUT";
uint8_t *tee_inout = "the paramType is MEMREF_INOUT";
uint32_t tee_output_len;
uint32_t tee_inout_len;

uint32_t tee_output_a = 0xddd;
uint32_t tee_output_b = 0xccc;
uint32_t tee_inout_a = 0x555;
uint32_t tee_inout_b = 0x666;

int test_shrdmem_allocate(TEEC_Context *context)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory sharedMem;
	TEEC_Result result;

	/**test flags=5, return error**/
	sharedMem.size = 1024;
	sharedMem.flags = 0x05;
	result = TEEC_AllocateSharedMemory(
			 context,
			 &sharedMem);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("allocate success, but an error case(flags=5)\n");
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}

	/**right test case**/
	sharedMem.size = 1024;
	sharedMem.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT ;
	result = TEEC_AllocateSharedMemory(
			 context,
			 &sharedMem);
	if (result == TEEC_SUCCESS) {
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 0;
	} else {
		TEEC_Error("allocate failed\n");
		return 1;
	}
}

int test_shrdmem_allocate_input(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory sharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;

	/**allocate shared memory, right test case**/
	sharedMem.size = 1024;
	sharedMem.flags = TEEC_MEM_INPUT;
	result = TEEC_AllocateSharedMemory(
			 context,
			 &sharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}
	strcpy(sharedMem.buffer, "1122334455");
	strcpy(sharedMem.buffer + 100, "100200300400");

	memset(&operation, 0x00, sizeof(operation));
	/*test invoke, paramtype=out**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param_type=out)\n");
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}
	/*test invoke, paramtype=inout**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param_type=inout)\n");
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}
	/*test invoke, paramtype=in**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_MEMREF_WHOLE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 100;
	operation.params[0].memref.size = 10;
	operation.params[1].memref.parent = &sharedMem;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_SUCCESS) {
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 0;
	} else {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}
}

int test_shrdmem_allocate_output(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory sharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;

	/**allocate shared memory, right test case**/
	sharedMem.size = 1024;
	sharedMem.flags = TEEC_MEM_OUTPUT;
	result = TEEC_AllocateSharedMemory(
			 context,
			 &sharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}
	memset(sharedMem.buffer, 0x00, 1024);
	strcpy(sharedMem.buffer, "1122334455");
	strcpy(sharedMem.buffer + 100, "100200300400");

	memset(&operation, 0x00, sizeof(operation));
	/*test invoke, paramtype=in**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param_type=out)\n");
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}
	/*test invoke, paramtype=inout**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param_type=inout)\n");
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}
	/*test invoke, paramtype=out**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_MEMREF_WHOLE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 100;
	operation.params[0].memref.size = 30;
	operation.params[1].memref.parent = &sharedMem;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_SUCCESS) {
		TEEC_Debug("[0] : %s\n", sharedMem.buffer + 100);
		TEEC_Debug("[1] : %s\n", sharedMem.buffer);
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 0;
	} else {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}
}

int test_shrdmem_allocate_inout(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory sharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;

	/**allocate shared memory, right test case**/
	sharedMem.size = 1024;
	sharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_AllocateSharedMemory(
			 context,
			 &sharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}
	memset(sharedMem.buffer, 0x00, 1024);
	strcpy(sharedMem.buffer, "1122334455");
	strcpy(sharedMem.buffer + 100, "offset is 100");
	strcpy(sharedMem.buffer + 200, "offset is 200");
	strcpy(sharedMem.buffer + 300, "offset is 300");

	memset(&operation, 0x00, sizeof(operation));
	/**test invoke, param is null**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param in null)\n");
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}

	/*test invoke, param is whole.input.output.inout**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_WHOLE,
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_MEMREF_PARTIAL_INOUT);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	operation.params[1].memref.parent = &sharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 50;
	operation.params[2].memref.parent = &sharedMem;
	operation.params[2].memref.offset = 200;
	operation.params[2].memref.size = 50;
	operation.params[3].memref.parent = &sharedMem;
	operation.params[3].memref.offset = 300;
	operation.params[3].memref.size = 50;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_SUCCESS) {
		TEEC_Debug("[0] : %s\n", sharedMem.buffer);
		TEEC_Debug("[2] : %s\n", sharedMem.buffer + 200);
		TEEC_Debug("[3] : %s\n", sharedMem.buffer + 300);
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 0;
	} else {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}
}

int test_shrdmem_register(TEEC_Context *context)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory nonZeroCopysharedMem;
	TEEC_Result result;
	char testdata[256];
	uint32_t origin;

	/**test buffer=NULL, return error**/
	nonZeroCopysharedMem.buffer = NULL;
	nonZeroCopysharedMem.size = 256;
	nonZeroCopysharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_RegisterSharedMemory(
			 context,
			 &nonZeroCopysharedMem);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("register success, but an error case(buffer=NULL)\n");
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}

	/**test flags=5, return error**/
	nonZeroCopysharedMem.buffer = testdata;
	nonZeroCopysharedMem.size = 256;
	nonZeroCopysharedMem.flags = 0x5;
	result = TEEC_RegisterSharedMemory(
			 context,
			 &nonZeroCopysharedMem);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("register success, but an error case(flags=5)\n");
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}

	/**right test case**/
	nonZeroCopysharedMem.buffer = testdata;
	nonZeroCopysharedMem.size = 256;
	nonZeroCopysharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_RegisterSharedMemory(
			 context,
			 &nonZeroCopysharedMem);
	if (result == TEEC_SUCCESS) {
		TEEC_Debug("register success\n");
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 0;
	} else {
		TEEC_Error("register failed\n");
		return 1;
	}
}

int test_shrdmem_register_input(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory nonZeroCopysharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	char testdata[1024];
	uint32_t origin;

	memset(testdata, 0x00, 1024);
	strcpy(testdata, "1122334455");
	strcpy(testdata + 100, "offset is 100");
	strcpy(testdata + 200, "offset is 200");
	strcpy(testdata + 300, "offset is 300");

	/**register shared memory, right test case**/
	nonZeroCopysharedMem.buffer = testdata;
	nonZeroCopysharedMem.size = 1024;
	nonZeroCopysharedMem.flags = TEEC_MEM_INPUT;
	result = TEEC_RegisterSharedMemory(
			 context,
			 &nonZeroCopysharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}

	memset(&operation, 0x00, sizeof(operation));
	/*test invoke, paramtype=out**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param_type=out)\n");
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}
	/*test invoke, paramtype=inout**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param_type=inout)\n");
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}
	/*test invoke, paramtype=in**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_MEMREF_WHOLE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 100;
	operation.params[0].memref.size = 10;
	operation.params[1].memref.parent = &nonZeroCopysharedMem;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_SUCCESS) {
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 0;
	} else {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}
}

int test_shrdmem_register_output(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory nonZeroCopysharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	char testdata[1024];
	uint32_t origin;

	memset(testdata, 0x00, 1024);
	strcpy(testdata, "1122334455");
	strcpy(testdata + 100, "offset is 100");
	strcpy(testdata + 200, "offset is 200");
	strcpy(testdata + 300, "offset is 300");

	/**register shared memory, right test case**/
	nonZeroCopysharedMem.buffer = testdata;
	nonZeroCopysharedMem.size = 1024;
	nonZeroCopysharedMem.flags = TEEC_MEM_OUTPUT;
	result = TEEC_RegisterSharedMemory(
			 context,
			 &nonZeroCopysharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}

	memset(&operation, 0x00, sizeof(operation));
	/*test invoke, paramtype=in**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param_type=out)\n");
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}
	/*test invoke, paramtype=inout**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param_type=inout)\n");
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}
	/*test invoke, paramtype=out**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_WHOLE,
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	operation.params[1].memref.parent = &nonZeroCopysharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 30;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_SUCCESS) {
		TEEC_Debug("[0] : %s\n", nonZeroCopysharedMem.buffer);
		TEEC_Debug("[1] : %s\n", nonZeroCopysharedMem.buffer + 100);
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 0;
	} else {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}
}

int test_shrdmem_register_inout(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory nonZeroCopysharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	char testdata[1024];
	uint32_t origin;

	memset(testdata, 0x00, 1024);
	strcpy(testdata, "1122334455");
	strcpy(testdata + 100, "offset is 100");
	strcpy(testdata + 200, "offset is 200");
	strcpy(testdata + 300, "offset is 300");

	/**register shared memory, right test case**/
	nonZeroCopysharedMem.buffer = testdata;
	nonZeroCopysharedMem.size = 1024;
	nonZeroCopysharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_RegisterSharedMemory(
			 context,
			 &nonZeroCopysharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}

	memset(&operation, 0x00, sizeof(operation));
	/**test invoke, param is null**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 NULL);
	if (result == TEEC_SUCCESS) {
		TEEC_Error("invoke success, but an error case(param in null)\n");
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}

	/*test invoke, param is whole.input.output.inout**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_WHOLE,
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_MEMREF_PARTIAL_INOUT);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;
	operation.params[1].memref.parent = &nonZeroCopysharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 50;
	operation.params[2].memref.parent = &nonZeroCopysharedMem;
	operation.params[2].memref.offset = 200;
	operation.params[2].memref.size = 50;
	operation.params[3].memref.parent = &nonZeroCopysharedMem;
	operation.params[3].memref.offset = 300;
	operation.params[3].memref.size = 50;
	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_SUCCESS) {
		TEEC_Debug("[0] : %s\n", nonZeroCopysharedMem.buffer);
		TEEC_Debug("[2] : %s\n", nonZeroCopysharedMem.buffer + 200);
		TEEC_Debug("[3] : %s\n", nonZeroCopysharedMem.buffer + 300);
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 0;
	} else {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		return 1;
	}
}

int test_invoke_all(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory sharedMem;
	TEEC_SharedMemory nonZeroCopysharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	char testdata[1024];
	uint32_t origin;

	memset(testdata, 0x00, 1024);
	strcpy(testdata, "1122334455");
	strcpy(testdata + 100, "offset is 100");
	strcpy(testdata + 200, "offset is 200");
	strcpy(testdata + 300, "offset is 300");

	/**allocate shared memory, right test case**/
	sharedMem.size = 1024;
	sharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_AllocateSharedMemory(
			 context,
			 &sharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}
	memset(sharedMem.buffer, 0x00, 1024);
	strcpy(sharedMem.buffer, "1122334455");
	strcpy(sharedMem.buffer + 100, "offset is 100");
	strcpy(sharedMem.buffer + 200, "offset is 200");
	strcpy(sharedMem.buffer + 300, "offset is 300");

	/**register shared memory, right test case**/
	nonZeroCopysharedMem.buffer = testdata;
	nonZeroCopysharedMem.size = 1024;
	nonZeroCopysharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_RegisterSharedMemory(
			 context,
			 &nonZeroCopysharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}

	memset(&operation, 0x00, sizeof(operation));
	/**test invoke, param is null**/
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_MEMREF_WHOLE,
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_VALUE_INOUT);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 200;
	operation.params[0].memref.size = 50;
	operation.params[1].memref.parent = &sharedMem;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 0;
	operation.params[2].tmpref.buffer = testdata;
	operation.params[2].tmpref.size = 200;
	operation.params[3].value.a = 0x123;
	operation.params[3].value.b = 0x987;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_SUCCESS) {
		TEEC_Debug("[0] : %s\n", nonZeroCopysharedMem.buffer + 200);
		TEEC_Debug("[1] : %s\n", sharedMem.buffer);
		TEEC_Debug("[2] : %s\n", testdata);
		TEEC_Debug("[3] : %x %x\n", operation.params[3].value.a, operation.params[3].value.b);
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 0;
	} else {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
		TEEC_ReleaseSharedMemory(&sharedMem);
		return 1;
	}
}

int test_invoke_allocateMem_returnLen(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory sharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin = TEEC_ORIGIN_TRUSTED_APP;

	/**allocate shared memory, right test case**/
	sharedMem.size = 1024;
	sharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_AllocateSharedMemory(
			 context,
			 &sharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}
	memset(sharedMem.buffer, 0x00, 1024);
	strcpy(sharedMem.buffer, "11223344556677889900");
	strcpy(sharedMem.buffer + 100, "offset is 100");
	strcpy(sharedMem.buffer + 200, "offset is 200");
	strcpy(sharedMem.buffer + 300, "offset is 300");

	/* for test TEEC_MEMREF_PARTIAL_OUTPUT, [0] size > tee size, [1] size < tee size*/
	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 20;
	operation.params[1].memref.parent = &sharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 10;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", sharedMem.buffer, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", sharedMem.buffer + 100, operation.params[1].memref.size);
		if (operation.params[0].memref.size != tee_output_len) {
			TEEC_Error("11: operation.params[0].memref.size=%d\n", operation.params[0].memref.size);
			goto error;
		} else if (strncmp(sharedMem.buffer, tee_output, tee_output_len) != 0) {
			TEEC_Error("12: sharedMem.buffer=%s\n", sharedMem.buffer);
			goto error;
		} else if (operation.params[1].memref.size != tee_output_len) {
			TEEC_Error("13: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("14: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

	/* for test TEEC_MEMREF_PARTIAL_INOUT, [0] size > tee size, [1] size < tee size*/
	memset(sharedMem.buffer, 0x00, 1024);
	strcpy(sharedMem.buffer, "11223344556677889900");
	strcpy(sharedMem.buffer + 100, "offset is 100");
	strcpy(sharedMem.buffer + 200, "offset is 200");
	strcpy(sharedMem.buffer + 300, "offset is 300");

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 50;
	operation.params[1].memref.parent = &sharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 20;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", sharedMem.buffer, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", sharedMem.buffer + 100, operation.params[1].memref.size);
		if (operation.params[0].memref.size != tee_inout_len) {
			TEEC_Error("21: operation.params[0].memref.size=%d\n", operation.params[0].memref.size);
			goto error;
		} else if (strncmp(sharedMem.buffer, tee_inout, tee_inout_len) != 0) {
			TEEC_Error("22: sharedMem.buffer=%s\n", sharedMem.buffer);
			goto error;
		} else if (operation.params[1].memref.size != tee_inout_len) {
			TEEC_Error("23: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("24: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

	/* for test , [0] is INPUT, [1] size > tee size, [2] size > tee size, [3] size < tee size */
	memset(sharedMem.buffer, 0x00, 1024);
	strcpy(sharedMem.buffer, "11223344556677889900");
	strcpy(sharedMem.buffer + 100, "offset is 100");
	strcpy(sharedMem.buffer + 200, "offset is 200");
	strcpy(sharedMem.buffer + 300, "offset is 300");

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_MEMREF_PARTIAL_INOUT);
	operation.params[0].memref.parent = &sharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 20;
	operation.params[1].memref.parent = &sharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 50;
	operation.params[2].memref.parent = &sharedMem;
	operation.params[2].memref.offset = 200;
	operation.params[2].memref.size = 50;
	operation.params[3].memref.parent = &sharedMem;
	operation.params[3].memref.offset = 300;
	operation.params[3].memref.size = 20;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", sharedMem.buffer, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", sharedMem.buffer + 100, operation.params[1].memref.size);
		TEEC_Debug("[2] : buffer=%s, size=%d\n", sharedMem.buffer + 200, operation.params[2].memref.size);
		TEEC_Debug("[3] : buffer=%s, size=%d\n", sharedMem.buffer + 300, operation.params[3].memref.size);
		if (operation.params[1].memref.size != tee_output_len) {
			TEEC_Error("31: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		} else if (strncmp(sharedMem.buffer + 100, tee_output, tee_output_len) != 0) {
			TEEC_Error("32: sharedMem.buffer+100=%s\n", sharedMem.buffer + 100);
			goto error;
		} else if (operation.params[2].memref.size != tee_inout_len) {
			TEEC_Error("33: operation.params[2].memref.size=%d\n", operation.params[2].memref.size);
			goto error;
		} else if (strncmp(sharedMem.buffer + 200, tee_inout, tee_inout_len) != 0) {
			TEEC_Error("34: sharedMem.buffer+200=%s\n", sharedMem.buffer + 200);
			goto error;
		} else if (operation.params[3].memref.size != tee_inout_len) {
			TEEC_Error("35: operation.params[3].memref.size=%d\n", operation.params[3].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("36: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

success:
	TEEC_ReleaseSharedMemory(&sharedMem);
	return 0;
error:
	TEEC_ReleaseSharedMemory(&sharedMem);
	return 1;
}

int test_invoke_regiterMem_returnLen(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_SharedMemory nonZeroCopysharedMem;
	TEEC_Result result;
	TEEC_Operation operation;
	char testdata[1024];
	uint32_t origin = TEEC_ORIGIN_TRUSTED_APP;

	memset(testdata, 0x00, 1024);
	strcpy(testdata, "1122334455667890");
	strcpy(testdata + 100, "offset is 100");
	strcpy(testdata + 200, "offset is 200");
	strcpy(testdata + 300, "offset is 300");

	/**register shared memory, right test case**/
	nonZeroCopysharedMem.buffer = testdata;
	nonZeroCopysharedMem.size = 1024;
	nonZeroCopysharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_RegisterSharedMemory(
			 context,
			 &nonZeroCopysharedMem);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("allocate failed\n");
		return 1;
	}

	/* for test TEEC_MEMREF_PARTIAL_OUTPUT, [0] size > tee size, [1] size < tee size*/
	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 20;
	operation.params[1].memref.parent = &nonZeroCopysharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 10;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", nonZeroCopysharedMem.buffer, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", nonZeroCopysharedMem.buffer + 100, operation.params[1].memref.size);
		if (operation.params[0].memref.size != tee_output_len) {
			TEEC_Error("11: operation.params[0].memref.size=%d\n", operation.params[0].memref.size);
			goto error;
		} else if (strncmp(nonZeroCopysharedMem.buffer, tee_output, tee_output_len) != 0) {
			TEEC_Error("12: nonZeroCopysharedMem.buffer=%s\n", nonZeroCopysharedMem.buffer);
			goto error;
		} else if (operation.params[1].memref.size != tee_output_len) {
			TEEC_Error("13: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("14: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

	/* for test TEEC_MEMREF_PARTIAL_INOUT, [0] size > tee size, [1] size < tee size*/
	memset(nonZeroCopysharedMem.buffer, 0x00, 1024);
	strcpy(nonZeroCopysharedMem.buffer, "11223344556677889900");
	strcpy(nonZeroCopysharedMem.buffer + 100, "offset is 100");
	strcpy(nonZeroCopysharedMem.buffer + 200, "offset is 200");
	strcpy(nonZeroCopysharedMem.buffer + 300, "offset is 300");

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 50;
	operation.params[1].memref.parent = &nonZeroCopysharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 20;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", nonZeroCopysharedMem.buffer, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", nonZeroCopysharedMem.buffer + 100, operation.params[1].memref.size);
		if (operation.params[0].memref.size != tee_inout_len) {
			TEEC_Error("21: operation.params[0].memref.size=%d\n", operation.params[0].memref.size);
			goto error;
		} else if (strncmp(nonZeroCopysharedMem.buffer, tee_inout, tee_inout_len) != 0) {
			TEEC_Error("22: nonZeroCopysharedMem.buffer=%s\n", nonZeroCopysharedMem.buffer);
			goto error;
		} else if (operation.params[1].memref.size != tee_inout_len) {
			TEEC_Error("23: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("24: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

	/* for test , [0] is INPUT, [1] size > tee size, [2] size > tee size, [3] size < tee size */
	memset(nonZeroCopysharedMem.buffer, 0x00, 1024);
	strcpy(nonZeroCopysharedMem.buffer, "11223344556677889900");
	strcpy(nonZeroCopysharedMem.buffer + 100, "offset is 100");
	strcpy(nonZeroCopysharedMem.buffer + 200, "offset is 200");
	strcpy(nonZeroCopysharedMem.buffer + 300, "offset is 300");

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_PARTIAL_INPUT,
				       TEEC_MEMREF_PARTIAL_OUTPUT,
				       TEEC_MEMREF_PARTIAL_INOUT,
				       TEEC_MEMREF_PARTIAL_INOUT);
	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 20;
	operation.params[1].memref.parent = &nonZeroCopysharedMem;
	operation.params[1].memref.offset = 100;
	operation.params[1].memref.size = 50;
	operation.params[2].memref.parent = &nonZeroCopysharedMem;
	operation.params[2].memref.offset = 200;
	operation.params[2].memref.size = 50;
	operation.params[3].memref.parent = &nonZeroCopysharedMem;
	operation.params[3].memref.offset = 300;
	operation.params[3].memref.size = 20;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", nonZeroCopysharedMem.buffer, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", nonZeroCopysharedMem.buffer + 100, operation.params[1].memref.size);
		TEEC_Debug("[2] : buffer=%s, size=%d\n", nonZeroCopysharedMem.buffer + 200, operation.params[2].memref.size);
		TEEC_Debug("[3] : buffer=%s, size=%d\n", nonZeroCopysharedMem.buffer + 300, operation.params[3].memref.size);
		if (operation.params[1].memref.size != tee_output_len) {
			TEEC_Error("31: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		} else if (strncmp(nonZeroCopysharedMem.buffer + 100, tee_output, tee_output_len) != 0) {
			TEEC_Error("32: nonZeroCopysharedMem.buffer+100=%s\n", nonZeroCopysharedMem.buffer + 100);
			goto error;
		} else if (operation.params[2].memref.size != tee_inout_len) {
			TEEC_Error("33: operation.params[2].memref.size=%d\n", operation.params[2].memref.size);
			goto error;
		} else if (strncmp(nonZeroCopysharedMem.buffer + 200, tee_inout, tee_inout_len) != 0) {
			TEEC_Error("24: nonZeroCopysharedMem.buffer+200=%s\n", nonZeroCopysharedMem.buffer + 200);
			goto error;
		} else if (operation.params[3].memref.size != tee_inout_len) {
			TEEC_Error("35: operation.params[3].memref.size=%d\n", operation.params[3].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("36: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

success:
	TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
	return 0;
error:
	TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
	return 1;
}

int test_invoke_tempMem_returnLen(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Result result;
	TEEC_Operation operation;
	char testdata[1024];
	uint32_t origin = TEEC_ORIGIN_TRUSTED_APP;

	/* for test TEEC_MEMREF_PARTIAL_OUTPUT, [0] size > tee size, [1] size < tee size*/
	memset(testdata, 0x00, 1024);
	strcpy(testdata, "1122334455667890");
	strcpy(testdata + 100, "offset is 100");
	strcpy(testdata + 200, "offset is 200");
	strcpy(testdata + 300, "offset is 300");

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_TEMP_OUTPUT,
				       TEEC_MEMREF_TEMP_OUTPUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].tmpref.buffer = testdata;
	operation.params[0].tmpref.size = 20;
	operation.params[1].tmpref.buffer = testdata + 100;
	operation.params[1].tmpref.size = 10;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", testdata, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", testdata + 100, operation.params[1].memref.size);
		if (operation.params[0].memref.size != tee_output_len) {
			TEEC_Error("11: operation.params[0].memref.size=%d\n", operation.params[0].memref.size);
			goto error;
		} else if (strncmp(testdata, tee_output, tee_output_len) != 0) {
			TEEC_Error("12: testdata=%s\n", testdata);
			goto error;
		} else if (operation.params[1].memref.size != tee_output_len) {
			TEEC_Error("13: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("14: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

	/* for test TEEC_MEMREF_PARTIAL_INOUT, [0] size > tee size, [1] size < tee size*/
	memset(testdata, 0x00, 1024);
	strcpy(testdata, "1122334455667890");
	strcpy(testdata + 100, "offset is 100");
	strcpy(testdata + 200, "offset is 200");
	strcpy(testdata + 300, "offset is 300");

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].tmpref.buffer = testdata;
	operation.params[0].tmpref.size = 50;
	operation.params[1].tmpref.buffer = testdata + 100;
	operation.params[1].tmpref.size = 20;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", testdata, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", testdata + 100, operation.params[1].memref.size);
		if (operation.params[0].memref.size != tee_inout_len) {
			TEEC_Error("21: operation.params[0].memref.size=%d\n", operation.params[0].memref.size);
			goto error;
		} else if (strncmp(testdata, tee_inout, tee_inout_len) != 0) {
			TEEC_Error("22: testdata=%s\n", testdata);
			goto error;
		} else if (operation.params[1].memref.size != tee_inout_len) {
			TEEC_Error("23: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("24: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

	/* for test , [0] is INPUT, [1] size > tee size, [2] size > tee size, [3] size < tee size */
	memset(testdata, 0x00, 1024);
	strcpy(testdata, "1122334455667890");
	strcpy(testdata + 100, "offset is 100");
	strcpy(testdata + 200, "offset is 200");
	strcpy(testdata + 300, "offset is 300");

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_OUTPUT,
				       TEEC_MEMREF_TEMP_INOUT,
				       TEEC_MEMREF_TEMP_INOUT);
	operation.params[0].tmpref.buffer = testdata;
	operation.params[0].tmpref.size = 20;
	operation.params[1].tmpref.buffer = testdata + 100;
	operation.params[1].tmpref.size = 50;
	operation.params[2].tmpref.buffer = testdata + 200;
	operation.params[2].tmpref.size = 50;
	operation.params[3].tmpref.buffer = testdata + 300;
	operation.params[3].tmpref.size = 20;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_ERROR_SHORT_BUFFER) {
		TEEC_Debug("[0] : buffer=%s, size=%d\n", testdata, operation.params[0].memref.size);
		TEEC_Debug("[1] : buffer=%s, size=%d\n", testdata + 100, operation.params[1].memref.size);
		TEEC_Debug("[2] : buffer=%s, size=%d\n", testdata + 200, operation.params[2].memref.size);
		TEEC_Debug("[3] : buffer=%s, size=%d\n", testdata + 300, operation.params[3].memref.size);
		if (operation.params[1].memref.size != tee_output_len) {
			TEEC_Error("31: operation.params[1].memref.size=%d\n", operation.params[1].memref.size);
			goto error;
		} else if (strncmp(testdata + 100, tee_output, tee_output_len) != 0) {
			TEEC_Error("32: testdata+100=%s\n", testdata + 100);
			goto error;
		} else if (operation.params[2].memref.size != tee_inout_len) {
			TEEC_Error("33: operation.params[2].memref.size=%d\n", operation.params[2].memref.size);
			goto error;
		} else if (strncmp(testdata + 200, tee_inout, tee_inout_len) != 0) {
			TEEC_Error("34: testdata+200=%s\n", testdata + 200);
			goto error;
		} else if (operation.params[3].memref.size != tee_inout_len) {
			TEEC_Error("35: operation.params[3].memref.size=%d\n", operation.params[3].memref.size);
			goto error;
		}
	} else {
		TEEC_Error("36: invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}

success:
	return 0;
error:
	return 1;
}

int test_invoke_value(TEEC_Context *context, TEEC_Session *session)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin = TEEC_ORIGIN_TRUSTED_APP;

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_VALUE_INOUT,
				       TEEC_VALUE_OUTPUT);
	operation.params[0].value.a = 0xA00;
	operation.params[0].value.b = 0xB00;
	operation.params[1].value.a = 0xA01;
	operation.params[1].value.b = 0xB01;
	operation.params[2].value.a = 0xA02;
	operation.params[2].value.b = 0xB02;
	operation.params[3].value.a = 0xA03;
	operation.params[3].value.b = 0xB03;

	result = TEEC_InvokeCommand(
			 session,
			 ECHO_CMD_ID_SEND_CMD,
			 &operation,
			 &origin);
	if (result == TEEC_SUCCESS) {
		TEEC_Debug("[0] : a=0x%x, b=0x%x\n", operation.params[0].value.a, operation.params[0].value.b);
		TEEC_Debug("[1] : a=0x%x, b=0x%x\n", operation.params[1].value.a, operation.params[1].value.b);
		TEEC_Debug("[2] : a=0x%x, b=0x%x\n", operation.params[2].value.a, operation.params[2].value.b);
		TEEC_Debug("[3] : a=0x%x, b=0x%x\n", operation.params[3].value.a, operation.params[3].value.b);
		if ((operation.params[0].value.a != 0xA00) || (operation.params[0].value.b != 0xB00)) {
			TEEC_Error("[0] : a=0x%x, b=0x%x\n", operation.params[0].value.a, operation.params[0].value.b);
			goto error;
		} else if ((operation.params[1].value.a != tee_output_a) || (operation.params[1].value.b != tee_output_b)) {
			TEEC_Error("[1] : a=0x%x, b=0x%x\n", operation.params[1].value.a, operation.params[1].value.b);
			goto error;
		} else if ((operation.params[2].value.a != tee_inout_a) || (operation.params[2].value.b != tee_inout_b)) {
			TEEC_Error("[2] : a=0x%x, b=0x%x\n", operation.params[2].value.a, operation.params[2].value.b);
			goto error;
		} else if ((operation.params[3].value.a != tee_output_a) || (operation.params[3].value.b != tee_output_b)) {
			TEEC_Error("[3] : a=0x%x, b=0x%x\n", operation.params[3].value.a, operation.params[3].value.b);
			goto error;
		}
	} else {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		goto error;
	}


success:
	return 0;
error:
	return 1;
}

int main(void)
{
	tee_output_len = strlen(tee_output);
	tee_inout_len = strlen(tee_inout);

	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result;
	TEEC_UUID svc_id = TEE_SERVICE_ECHO;
	TEEC_Operation operation;
	int err = 0;

	result = TEEC_InitializeContext(
			 NULL,
			 &context);

	if (result != TEEC_SUCCESS) {
		goto cleanup_1;
	}
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_INPUT);
	result = TEEC_OpenSession(
			 &context,
			 &session,
			 &svc_id,
			 TEEC_LOGIN_IDENTIFY,
			 NULL,
			 &operation,
			 NULL);

	if (result != TEEC_SUCCESS) {
		goto cleanup_2;
	}
	TEEC_Debug("session id 0x%x\n", session.session_id);
#if 1
	if (test_shrdmem_allocate(&context) != 0) {
		TEEC_Error("test test_shrdmem_allocate failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_shrdmem_allocate success\n");
	}

	if (test_shrdmem_allocate_input(&context, &session) != 0) {
		TEEC_Error("test test_shrdmem_allocate_input failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_shrdmem_allocate_input success\n");
	}

	if (test_shrdmem_allocate_output(&context, &session) != 0) {
		TEEC_Error("test test_shrdmem_allocate_output failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_shrdmem_allocate_output success\n");
	}

	if (test_shrdmem_allocate_inout(&context, &session) != 0) {
		TEEC_Error("test test_shrdmem_allocate_inout failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_shrdmem_allocate_inout success\n");
	}

	if (test_shrdmem_register(&context) != 0) {
		TEEC_Error("test test_shrdmem_register failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_shrdmem_register success\n");
	}

	if (test_shrdmem_register_input(&context, &session) != 0) {
		TEEC_Error("test test_shrdmem_register_input failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_shrdmem_register_input success\n");
	}

	if (test_shrdmem_register_output(&context, &session) != 0) {
		TEEC_Error("test test_shrdmem_register_output failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_shrdmem_register_output success\n");
	}

	if (test_shrdmem_register_inout(&context, &session) != 0) {
		TEEC_Error("test test_shrdmem_register_inout failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_shrdmem_register_inout success\n");
	}

	if (test_invoke_all(&context, &session) != 0) {
		TEEC_Error("test test_invoke_all failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_invoke_all success\n");
	}
#endif
	if (test_invoke_allocateMem_returnLen(&context, &session) != 0) {
		TEEC_Error("test test_invoke_allocateMem_returnLen failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_invoke_allocateMem_returnLen success\n");
	}
	if (test_invoke_regiterMem_returnLen(&context, &session) != 0) {
		TEEC_Error("test test_invoke_regiterMem_returnLen failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_invoke_regiterMem_returnLen success\n");
	}
	if (test_invoke_tempMem_returnLen(&context, &session) != 0) {
		TEEC_Error("test test_invoke_tempMem_returnLen failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_invoke_tempMem_returnLen success\n");
	}
	if (test_invoke_value(&context, &session) != 0) {
		TEEC_Error("test test_invoke_value failed\n");
		err = 1;
		goto cleanup_3;
	} else {
		TEEC_Debug("test test_invoke_value success\n");
	}

	printf("ALL test case are PASS\n");

cleanup_3:
	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	if (err)
		while (1);
	return 0;
}
