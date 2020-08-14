#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define TEEC_DEBUG

#include "tee_client_api.h"
#include "tee_client_id.h"

int main(int argc, char* argv[])
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result;
	
	/* TEE_SERVICE_HISI_DEMO */
	TEEC_UUID svc_id = {0x0b0b0b0b, 0x0b0b, 0x0b0b,
            		    {0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}};

	TEEC_SharedMemory sharedMem;
	TEEC_SharedMemory nonZeroCopysharedMem;
	TEEC_Operation operation;
	TEEC_Operation sess_op;
	char testdata[1024];
	uint32_t origin;
	unsigned int cmd = 0;

	if (2 != argc )
    {
        printf("Usage: hisi_demo_client cmd \n"
               "       cmd:  0--Hello Test\n"
               "             1--Storage Test\n"
               "             2--MMZ Test via TA2TA\n"
               "             3--RPMB Test\n"
               "             4--Write RPMB Key\n"
		);
        return 0;
    }
    cmd = strtoul(argv[1],NULL, 0);

	/*===========================================
	[1] 初始化TEE环境
	===========================================*/
	result = TEEC_InitializeContext(NULL, &context);
	if(result != TEEC_SUCCESS) {
		TEEC_Error("InitializeContext failed, ReturnCode=0x%x\n", result);
		goto cleanup_1;
	} else {
		TEEC_Debug("InitializeContext success\n");
	}
	
	/*===========================================
	[2] 打开会话
	===========================================*/
	memset(&sess_op, 0, sizeof(TEEC_Operation));
    sess_op.started = 1;
    sess_op.paramTypes = TEEC_PARAM_TYPES(
					    TEEC_NONE,
					    TEEC_NONE,
					    TEEC_MEMREF_TEMP_INPUT,
					    TEEC_MEMREF_TEMP_INPUT);
	result = TEEC_OpenSession(&context, &session, &svc_id, 
								TEEC_LOGIN_IDENTIFY, NULL, &sess_op, &origin);
	if(result != TEEC_SUCCESS) {
		TEEC_Error("OpenSession failed, ReturnCode=0x%x, ReturnOrigin=0x%x\n", result, origin);
		goto cleanup_2;
	} else {
		TEEC_Debug("OpenSession success\n");
	}
	
	/*===========================================
	[3] 通过申请方式获取共享内存
	===========================================*/
	sharedMem.size = 1024;
	sharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_AllocateSharedMemory( &context, &sharedMem);
	if(result != TEEC_SUCCESS) {
		TEEC_Error("AllocateSharedMemory failed, ReturnCode=0x%x\n", result);
		goto cleanup_3;
	} else {
		TEEC_Debug("AllocateSharedMemory success buffer[0x%x]\n", sharedMem.buffer);
	}
	
	/*===========================================
	[4] 通过注册方式获取共享内存
	===========================================*/
	memset(testdata, 0x00, 1024);
	nonZeroCopysharedMem.buffer = testdata + 512;
	nonZeroCopysharedMem.size = 32;
	nonZeroCopysharedMem.flags = TEEC_MEM_INOUT;
	result = TEEC_RegisterSharedMemory( &context, &nonZeroCopysharedMem);
	if(result != TEEC_SUCCESS) {
		TEEC_Error("RegisterSharedMemory failed, ReturnCode=0x%x\n", result);
		goto cleanup_4;;
	} else {
		TEEC_Debug("RegisterSharedMemory success buffer[0x%x]\n", nonZeroCopysharedMem.buffer);
	}
	
	/*===========================================
	[5] 初始化operation，发送命令
	===========================================*/
	memset(&operation, 0x0, sizeof(TEEC_Operation));
	memset(nonZeroCopysharedMem.buffer, 0x0, nonZeroCopysharedMem.size);
	memset(sharedMem.buffer, 0x0, sharedMem.size);
	memset(testdata, 0x0, sizeof(testdata));
	
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INOUT, 
											TEEC_MEMREF_PARTIAL_INOUT, 
											TEEC_MEMREF_TEMP_INOUT,   
											TEEC_VALUE_INOUT); /* TEEC type will translate to TEE type */

	operation.params[0].memref.parent = &nonZeroCopysharedMem;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 32;
	operation.params[1].memref.parent = &sharedMem;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 128;
	operation.params[2].tmpref.buffer = testdata;
	operation.params[2].tmpref.size = 200;
	operation.params[3].value.a = 0x11111111;
	operation.params[3].value.b = 0x22222222;
	result = TEEC_InvokeCommand( &session, cmd, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("InvokeCommand failed, ReturnCode=0x%x, ReturnOrigin=0x%x\n", result, origin);
		while(1);
		goto cleanup_5;
	} else {
		TEEC_Debug("InvokeCommand success\n");
		TEEC_Debug("After InvokeCommand:");
		TEEC_Debug("nonZeroCopysharedMem: %s\n", (uint8_t*)nonZeroCopysharedMem.buffer);
		TEEC_Debug("sharedMem: %s\n", (uint8_t*)sharedMem.buffer);
		TEEC_Debug("testdata: %s\n", testdata);
		TEEC_Debug("value a=0x%x, b=0x%x\n", operation.params[3].value.a, operation.params[3].value.b);
	}
	
	cleanup_5:
		TEEC_ReleaseSharedMemory(&nonZeroCopysharedMem);
	cleanup_4:
		TEEC_ReleaseSharedMemory(&sharedMem);
	cleanup_3:
		TEEC_CloseSession(&session);
	cleanup_2:
		TEEC_FinalizeContext(&context);
	cleanup_1:
	return 0;
}

