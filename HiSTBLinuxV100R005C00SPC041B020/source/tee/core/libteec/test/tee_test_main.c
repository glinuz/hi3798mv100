#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h> 
#include "tee_client_api.h"
#include "tee_client_id.h"

#define MAXIMUM_SESSION 16
//#define SHAREMEMSIZE 1024
//#define SHAREMEMSIZE (((rand() % (2048-1))+ 1) * 1024) /*1~2M*/
static TEEC_Context gContext;
static int gInited;
static TEEC_Session gSession[MAXIMUM_SESSION]= {0};
static TEEC_Operation gOperation[MAXIMUM_SESSION];
static TEEC_SharedMemory gSharedMem[MAXIMUM_SESSION] = {0};


typedef enum {
	CMD_TEST_MULTI_SESSION_01 = 0x1,
	CMD_TEST_MULTI_SESSION_02,
	CMD_TEST_MULTI_SESSION_03,
	CMD_TEST_MULTI_SESSION_04,
	CMD_TEST_MULTI_SESSION_05
} TEE_OPERATION_ID;

/* example uuid */
static const TEEC_UUID testhello_uuid = {
	0x79b77788, 0x9789, 0x4a7a,
	{ 0xa2, 0xbe, 0xb6, 0x01, 0x55, 0xee, 0xf5, 0xf3 }
};



static int teec_open_session(int session_num)
{
        TEEC_Result result;
        TEEC_Operation operation;
        uint32_t origin;

        char version_buf[256] = {0};
        unsigned int buf_len = 256;

        memset(&operation, 0, sizeof(TEEC_Operation));
        operation.started = 1;
        operation.paramTypes = TEEC_PARAM_TYPES(
                        TEEC_NONE,
                        TEEC_NONE,
                        TEEC_MEMREF_TEMP_INPUT,
                        TEEC_MEMREF_TEMP_INPUT);
        result = TEEC_OpenSession(&gContext, &gSession[session_num], &testhello_uuid,
                        TEEC_LOGIN_IDENTIFY, NULL,  &operation, &origin);

        if(result != TEEC_SUCCESS)
        {
                printf("teec_init open %d session failed: result:%d orgi: %d\n",
                                session_num, (int)result, origin);
        }
        else
        {
                printf("teec_init open %d session OK\n", session_num);
        }

        return result;
}

static void free_shared_memory(int session)
{
	int session_num;
	
	for(session_num = 0; session_num < session; session_num++)
	{
		 if(gSharedMem[session_num].buffer)
   		 {
        	TEEC_ReleaseSharedMemory(&gSharedMem[session_num]);
        	gSharedMem[session_num].buffer = NULL;
			printf("free %d shared_memory  done\n", session_num);
   		 }
	}
   
}

uint32_t get_rand_number()
{
	 uint32_t rand_num; 
	 srand(time(NULL));
     rand_num = (rand()%(2048-1))+1;
	 return rand_num * 1024;
}

static TEEC_Result allocate_shared_memory()
{
    TEEC_Result result;
	int session_num;
	uint32_t sharememsize;

	for(session_num = 0; session_num < MAXIMUM_SESSION; session_num++)
	{
//		sleep(1);
		usleep(1000);
		sharememsize = get_rand_number();
		printf("start  %d allocate , size = %d bytes\n", session_num, sharememsize);
		gSharedMem[session_num].buffer = NULL;
    	gSharedMem[session_num].size = sharememsize;
    	gSharedMem[session_num].flags = TEEC_MEM_INOUT;
	   	result = TEEC_AllocateSharedMemory(&gContext, &gSharedMem[session_num]);
    	if(result != TEEC_SUCCESS)
    	{
        	printf("allocate %d failed. ret = %x\n", session_num, result);
        	goto cleanup_1;
    	}

		printf("allocate %d SharedMemory OK, size = %d bytes\n", session_num, sharememsize);

    	memset(gSharedMem[session_num].buffer, 0, sharememsize);

	
}
	return result;

cleanup_1:
       free_shared_memory(session_num);
       return result;	
}



static TEEC_Result teec_init()
{

 
       TEEC_Result result;
 
       uint32_t origin;
 
       TEEC_Operation operation;
	
	int session_num;

 
       if (gInited)
 
       {
 
               printf("teec_init OK. had init before. return success\n");
 
               return TEEC_SUCCESS;
 
       }

 
       result = TEEC_InitializeContext(NULL, &gContext);
 
       if(result != TEEC_SUCCESS) {
 
               printf("teec initial failed, result:%d\n", result);
 
               return -1;
 
       }

	
	for(session_num = 0; session_num < MAXIMUM_SESSION; session_num++)
	
	{
	
	        result = teec_open_session(session_num);
 
      			if (result != TEEC_SUCCESS) {
 
               	printf("teec_init OpenSession failed\n");
 
                   goto cleanup_1;
 
      			 }
	
	}
 
       gInited = 1;

 
       printf("teec_init init OK\n");
 
       return TEEC_SUCCESS;

cleanup_1:
 
       TEEC_FinalizeContext(&gContext);
 
       return result;
}


static int teec_uninit()
{

	
	int session_num;
 
       if (!gInited)
 
       {
 
               printf("gInited is not inited!\n");
 
               return -1;
 
       }

	
    free_shared_memory(MAXIMUM_SESSION);
	
	
	
	for(session_num = 0; session_num < MAXIMUM_SESSION; session_num++)
	
	{
 
      			 TEEC_CloseSession(&gSession[session_num]);
	
			 printf("TEEC_CloseSession %d!\n", session_num);
	
	}

 
       TEEC_FinalizeContext(&gContext);
	
	printf("after TEEC_FinalizeContext!\n");

 
       gInited = 0;

 
       return 0;
}



static TEEC_Result test_invoke_command(void)
{

 
       TEEC_Result result;
 
       uint32_t origin;
	
	int session_num;
#if 0 
	
	result = allocate_shared_memory();
 
       if (result != TEEC_SUCCESS) {
 
               printf("allocate_shared_memory failed, %x\n", result);
 
               goto cleanup_1;
 
       }	

 
       result = teec_init();
 
       if (result != TEEC_SUCCESS) {
 
               printf("teec_init failed, %x\n", result);
 
               return result;
 
       }
#endif
	
    for(session_num = 0; session_num < MAXIMUM_SESSION; session_num++)
	
    {
	
		gOperation[session_num].started = 1;
 
       	gOperation[session_num].cancel_flag = 0;
 
       	gOperation[session_num].paramTypes = TEEC_PARAM_TYPES(
 
                       TEEC_MEMREF_PARTIAL_INOUT,
 
                       TEEC_VALUE_OUTPUT,
 
                       TEEC_NONE,
 
                       TEEC_NONE);

 
       	gOperation[session_num].params[0].memref.parent = &gSharedMem[session_num];
 
       	gOperation[session_num].params[0].memref.offset = 0;
 
       	gOperation[session_num].params[0].memref.size = gSharedMem[session_num].size;
		printf("InvokeCommand %d started\n", session_num);
 
       	result = TEEC_InvokeCommand(&gSession, CMD_TEST_MULTI_SESSION_01, &gOperation[session_num], &origin);
	
		if (result != TEEC_SUCCESS) {
 
               printf("TEEC_Invoke %d Command failed, %x\n",session_num, result);
 
               return result;
 
       	}
			printf("InvokeCommand %d OK\n", session_num);
	
	}	

	
	return result;
}


static TEEC_Result test_multi_session(void)
{

 
      TEEC_Result result;

	
	printf("*********start**********\n");	   
 
       result = teec_init();
 
       if (result != TEEC_SUCCESS) {
 
               printf("teec_init failed, %x\n", result);
 
               goto cleanup_1;
 
       }
	
	printf("*********after teec_init**********\n");

	
	result = allocate_shared_memory();
 
       if (result != TEEC_SUCCESS) {
 
               printf("allocate_shared_memory failed, %x\n", result);
 
               goto cleanup_1;
 
       }	

	
	printf("*********after allocate_shared_memory **********\n");
	
	
	
	result = test_invoke_command();
 
       if (result != TEEC_SUCCESS) {
 
               printf("test_invoke_command failed, %x\n", result);
 
               goto cleanup_1;
 
       }
	
	printf("*********after test_invoke_command**********\n");
	
	teec_uninit();
	
    return result;	

	
	
cleanup_1:
 
   free_shared_memory(MAXIMUM_SESSION);
 
   TEEC_FinalizeContext(&gContext);
 
   return result;
}


/* some modifications are needed */
static TEEC_Result teec_allocateSharedMemory()
{

 
       TEEC_SharedMemory shared_mem;
 
       TEEC_Result result;
 
       uint32_t mem_input = (uint32_t)TEEC_MEM_INPUT;
 
       uint32_t mem_output = (uint32_t)TEEC_MEM_OUTPUT;
 
       TEEC_Operation operation;
 
       uint32_t origin;
 
       char *buff = "xxxxxxxxxxxyyyyyyyyyy";

 
       shared_mem.size = strlen(buff) + 1;
 
       shared_mem.flags = mem_input | mem_output;

 
       memset(&operation, 0, sizeof(TEEC_Operation));
 
       operation.started = 1;
 
       operation.cancel_flag = 0;
 
       operation.paramTypes = TEEC_PARAM_TYPES(
 
                       TEEC_MEMREF_PARTIAL_INOUT,
 
                       TEEC_VALUE_OUTPUT,
 
                       TEEC_NONE,
 
                       TEEC_NONE);

 
       operation.params[0].memref.parent = &shared_mem;
 
       operation.params[0].memref.offset = 0;
 
       operation.params[0].memref.size = shared_mem.size;



 
       teec_init();
 
       result = TEEC_AllocateSharedMemory(&gContext, &shared_mem);
 
       if (result != TEEC_SUCCESS) {
 
               printf("test allocate failed, %x\n", result);
 
               return result;
 
       }
 
       result = TEEC_InvokeCommand(&gSession, 2, &operation, &origin);
 
       teec_uninit();

 
       return result;		
}



static TEEC_Result test_multi_session_bak(void)
{

 
       TEEC_Result result;
 
       TEEC_Result ret;
	
	TEEC_Operation operation;
	
	char buffer1[512] = {0}, buffer2[512] = {0};

	
	memset(&operation, 0, sizeof(TEEC_Operation));
	
	operation.started = 1;
	
	operation.params[0].tmpref.buffer = buffer2;    //get unit tests names
	
	operation.params[0].tmpref.size = 512;
	
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
	
						TEEC_NONE,
	
						TEEC_NONE,
	
						TEEC_NONE);
	
	
 
       result = teec_init();
 
       if (result == TEEC_SUCCESS) { 
	
        ret = TEEC_InvokeCommand(&gSession[0], CMD_TEST_MULTI_SESSION_01, &operation, NULL);
	
		if (ret != TEEC_SUCCESS) {
	
			TEEC_Error("teec_init OpenSession failed\n");
	
			return ret;
	
		}
 
       	}
	
	
 
       teec_uninit();
	
    return ret;	
}


int main(void)
{

	
	TEEC_Result result;
	
	result = test_multi_session();
 
       if(result != TEEC_SUCCESS) {
 
               printf("teec initial failed, result:%d\n", result);
 
               return -1;
 
       }

	
	return result;
}
