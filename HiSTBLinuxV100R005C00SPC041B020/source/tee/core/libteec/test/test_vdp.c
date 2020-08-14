#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "mpi_mmz.h"

#include "hi_type.h"

#include "hi_common.h"
//#include "hi_adp.h"
#include "hi_type.h"

#define SAMPLE_GET_INPUTCMD(InputCmd) fgets((char *)(InputCmd), (sizeof(InputCmd) - 1), stdin)

 int LoadFile_Direct(FILE *pfPic, unsigned int u32BufBaseAddr, unsigned long u32FileSize)
{
	char *pPicPtr;
	unsigned long size, readbyte, size1, i = 0;
	
	

	/*clean data*/
	 pPicPtr = (char *)u32BufBaseAddr;
	printf("%s,%d\n",__func__,__LINE__);
	 size = u32FileSize;
	 printf("%s,%d\n",__func__,__LINE__);
	 while(size > 0)
	 {
	 	
		 if (size > 1024)
		 {
		  	printf("%s,%d\n",__func__,__LINE__);
			 memset(pPicPtr, 0x0, 1024);
			 readbyte = 1024;
			 
		 }
		 else
		 {
		   printf("%s,%d\n",__func__,__LINE__);
			 memset(pPicPtr, 0x0, size);
			 readbyte = size;
			
		 }
		 printf("%d,%d\n",size,readbyte);

		 pPicPtr += readbyte;
		 size -= readbyte;
		 
	 }

	 
	/*read data */
	 pPicPtr = (char *)u32BufBaseAddr;;
	 size = u32FileSize;
	 
	 printf("loading data size %08x ..............please wait\r\n", size);
	 while(size > 0)
	 {
		 readbyte = fread(pPicPtr, 1, size, pfPic);
		 pPicPtr += readbyte;
		 size -= readbyte;
	 }


	return 0;
}



 /* example uuid */
static const TEEC_UUID EXAMPLE_uuid =
{ \
    0x1E1E1E1E, \
    0x1E1E, \
    0x1E1E, \
    { \
        0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E \
    } \
};

#define HELLO_CMD_INIT                     	0
#define HELLO_CMD_ONOFF                     1
#define HELLO_CMD_ROWS            			2
#define HELLO_CMD_COLS               		3
#define HELLO_CMD_XPOS               		4
#define HELLO_CMD_YPOS               		5
#define HELLO_CMD_SIZE               		6
#define HELLO_CMD_SP_V               		7
#define HELLO_CMD_SP_H               		8
#define HELLO_CMD_SCALE               		9
#define HELLO_CMD_BG_ON               		10
#define HELLO_CMD_STR_MU               		11


int main(void)
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Result result;
    TEEC_Operation operation;
    uint32_t origin;
	FILE *pfPic;
	uint32_t u32BufBaseAddr;
	uint32_t u32BufvirAddr;
	uint32_t u32FileSize;
	struct stat fileData;
    char *str1 = "It's a vdp nice day today!";
    char *str2 = "Let's enjoy vdp sec OS ok the life!";
	HI_S32 ret;
	HI_U32 index = 0;
	HI_CHAR 				InputCmd[32];

	HI_MMZ_BUF_S stBuf;
	HI_U32 i;
	 char *pchar = HI_NULL;

	
	char * picname = "/tmp/vdp_vmx_coef_test.b";

	pfPic = fopen(picname, "rb");
	if (pfPic == NULL)
	{
		 printf("Open picture failed.\n");
		 return -1;
	}	
	
	
	if (0 == stat(picname, &fileData))
	{
		printf("filesize %u.\n", fileData.st_size);
	} 
	
	u32FileSize = fileData.st_size;
	
	strcpy(stBuf.bufname,"SEC_VDP");
    stBuf.bufsize = 8*1024;
    stBuf.overflow_threshold = 100;
    stBuf.underflow_threshold = 0;
    ret = HI_MPI_MMZ_Malloc(&stBuf);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MMZ_Malloc failed:%x\n",ret);
        return HI_FAILURE;
    }
	u32BufBaseAddr = stBuf.phyaddr;
	
	printf("\n HI_MMZ_New ,---%x.\n",u32BufBaseAddr);

	u32BufvirAddr = (char *)HI_MPI_MMZ_Map(u32BufBaseAddr, 0);

	printf("u32FileSize = %d u32BufBaseAddr 0x%x,u32BufvirAddr 0x%x \n", u32FileSize,u32BufBaseAddr,u32BufvirAddr);
	LoadFile_Direct(pfPic, u32BufvirAddr, u32FileSize);

	printf("%s,%d\n",__func__,__LINE__);

	
    result = TEEC_InitializeContext(NULL, &context);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("TEEC init failed\n");
        goto cleanup_1;
    }
	printf("%s,%d\n",__func__,__LINE__);

    result = TEEC_OpenSession(&context, &session, &EXAMPLE_uuid,
            TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
    if(result != TEEC_SUCCESS) {
        TEEC_Error("Open session failed\n");
        goto cleanup_2;
    }
	printf("%s,%d\n",__func__,__LINE__);
	pchar = u32BufvirAddr;
	for (i=0;i <= 100;i++)
	{
		printf("%d,",pchar[i]);
	}
	printf("%s,%d\n",__func__,__LINE__);

    operation.started = 1;
    operation.params[0].tmpref.buffer = u32BufvirAddr;
    operation.params[0].tmpref.size = u32FileSize;

    operation.params[1].tmpref.buffer = str2;
    operation.params[1].tmpref.size = strlen(str2) + 1;
    operation.params[2].value.a = u32BufBaseAddr;
	//operation.params[2].value.a = u32BufvirAddr;
    operation.params[2].value.b = u32FileSize;
	operation.params[3].value.a = u32BufvirAddr;
	operation.params[3].value.b = 0;
	printf("%s,%d\n",__func__,__LINE__);
	

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_VALUE_OUTPUT);
#if 0
    result = TEEC_InvokeCommand(&session, HELLO_CMD_INIT, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
    }
	printf("%s,%d\n",__func__,__LINE__);
#endif
	while(1)
	{
        printf("please input TEST  CMD :\n");
		printf("i -->HELLO_CMD_INIT \n");
		printf("o -->HELLO_CMD_ONOFF \n");
		printf("r -->HELLO_CMD_ROWS \n");
		printf("c -->HELLO_CMD_COLS \n");
		printf("x -->HELLO_CMD_XPOS \n");
		printf("y -->HELLO_CMD_YPOS \n");
		printf("s -->HELLO_CMD_SIZE \n");
		printf("v -->HELLO_CMD_SP_V \n");
		printf("h -->HELLO_CMD_SP_H \n");
		printf("z -->HELLO_CMD_SCALE \n");
		printf("b -->HELLO_CMD_BG_ON \n");
		printf("m -->HELLO_CMD_STR_MU \n");
		printf("f -->HELLO_CMD_FLUSH \n");
		printf("please input 'q' to quit!\n");
		

        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }
				
     	if ('n' == InputCmd[0])
        {
			printf(" Select video layer \n");
			
            SAMPLE_GET_INPUTCMD(InputCmd);
            index = atoi(InputCmd);
			if (index == 1 )
				index =1;
			else
				index = 0;
			
			printf("---Select --video ---->%d;\n",index);

            continue;
        }
		

        if ('i' == InputCmd[0])
        {
			printf("Test HELLO_CMD_INIT \n");
			
            SAMPLE_GET_INPUTCMD(InputCmd);
            operation.params[2].value.b = atoi(InputCmd);
			operation.params[3].value.b = index;
			 result = TEEC_InvokeCommand(&session, HELLO_CMD_INIT, &operation, &origin);
		    if (result != TEEC_SUCCESS) {
		        TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		    }

            continue;
        }

		if ('o' == InputCmd[0])
        {
			printf("Test HELLO_CMD_ONOFF \n");
			
            SAMPLE_GET_INPUTCMD(InputCmd);
            operation.params[2].value.b = atoi(InputCmd);
			operation.params[3].value.b = index;
			 result = TEEC_InvokeCommand(&session, HELLO_CMD_ONOFF, &operation, &origin);
		    if (result != TEEC_SUCCESS) {
		        TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		    }
            continue;
        }

     	if ('r' == InputCmd[0])
        {
			printf("Test HELLO_CMD_ROWS \n");
			
            SAMPLE_GET_INPUTCMD(InputCmd);
            operation.params[2].value.b = atoi(InputCmd);
			operation.params[3].value.b = index;
			 result = TEEC_InvokeCommand(&session, HELLO_CMD_ROWS, &operation, &origin);
		    if (result != TEEC_SUCCESS) {
		        TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		    }

            continue;
        }
		
		if ('c' == InputCmd[0])
		{
			printf("Test HELLO_CMD_COLS \n");
			
			SAMPLE_GET_INPUTCMD(InputCmd);
			operation.params[2].value.b = atoi(InputCmd);
			operation.params[3].value.b = index;
			 result = TEEC_InvokeCommand(&session, HELLO_CMD_COLS, &operation, &origin);
			if (result != TEEC_SUCCESS) {
				TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
			}

			continue;
		}
		
		if ('x' == InputCmd[0])
		{
			printf("Test HELLO_CMD_XPOS \n");
			
			SAMPLE_GET_INPUTCMD(InputCmd);
			operation.params[2].value.b = atoi(InputCmd);
			operation.params[3].value.b = index;
			 result = TEEC_InvokeCommand(&session, HELLO_CMD_XPOS, &operation, &origin);
			if (result != TEEC_SUCCESS) {
				TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
			}

			continue;
		}
		
		if ('y' == InputCmd[0])
		{
			printf("Test HELLO_CMD_YPOS \n");
			
			SAMPLE_GET_INPUTCMD(InputCmd);
			operation.params[2].value.b = atoi(InputCmd);
			operation.params[3].value.b = index;
			 result = TEEC_InvokeCommand(&session, HELLO_CMD_YPOS, &operation, &origin);
			if (result != TEEC_SUCCESS) {
				TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
			}

			continue;
		}

  		if ('h' == InputCmd[0])
		  {
			  printf("Test HELLO_CMD_SP_H \n");
			  
			  SAMPLE_GET_INPUTCMD(InputCmd);
			  operation.params[2].value.b = atoi(InputCmd);
			  operation.params[3].value.b = index;
			   result = TEEC_InvokeCommand(&session, HELLO_CMD_SP_H, &operation, &origin);
			  if (result != TEEC_SUCCESS) {
				  TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
			  }
	  
			  continue;
		  }
  
  		if ('v' == InputCmd[0])
	  {
		  printf("Test HELLO_CMD_SP_V \n");
		  
		  SAMPLE_GET_INPUTCMD(InputCmd);
		  operation.params[2].value.b = atoi(InputCmd);
		  operation.params[3].value.b = index;
		   result = TEEC_InvokeCommand(&session, HELLO_CMD_SP_V, &operation, &origin);
		  if (result != TEEC_SUCCESS) {
			  TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		  }
  
		  continue;
	  }

	  if ('z' == InputCmd[0])
	  {
		  printf("Test HELLO_CMD_SCALE \n");
		  
		  SAMPLE_GET_INPUTCMD(InputCmd);
		  operation.params[2].value.b = atoi(InputCmd);
		  operation.params[3].value.b = index;
		   result = TEEC_InvokeCommand(&session, HELLO_CMD_SCALE, &operation, &origin);
		  if (result != TEEC_SUCCESS) {
			  TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		  }
  
		  continue;
	  }

	 if ('b' == InputCmd[0])
	  {
		  printf("Test HELLO_CMD_BG_ON \n");
		  
		  SAMPLE_GET_INPUTCMD(InputCmd);
		  operation.params[2].value.b = atoi(InputCmd);
		  operation.params[3].value.b = index;
		   result = TEEC_InvokeCommand(&session, HELLO_CMD_BG_ON, &operation, &origin);
		  if (result != TEEC_SUCCESS) {
			  TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		  }
  
		  continue;
	  }
	
	if ('s' == InputCmd[0])
	  {
		  printf("Test HELLO_CMD_SIZE \n");
		  
		  SAMPLE_GET_INPUTCMD(InputCmd);
		  operation.params[2].value.b = atoi(InputCmd);
		  operation.params[3].value.b = index;
		   result = TEEC_InvokeCommand(&session, HELLO_CMD_SIZE, &operation, &origin);
		  if (result != TEEC_SUCCESS) {
			  TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		  }
  
		  continue;
	  }

	
	if ('m' == InputCmd[0])
	  {
		  printf("Test HELLO_CMD_STR_MU \n");
		  
		  SAMPLE_GET_INPUTCMD(InputCmd);
		  operation.params[2].value.b = atoi(InputCmd);
		  operation.params[3].value.b = index;
		   result = TEEC_InvokeCommand(&session, HELLO_CMD_STR_MU, &operation, &origin);
		  if (result != TEEC_SUCCESS) {
			  TEEC_Error("Invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		  }
  
		  continue;
	  }
	}


    printf("The Secure World return %c%c\n", (char)(operation.params[2].value.a), (char)(operation.params[3].value.a));
	sleep(3);

	TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_1:
	HI_MPI_MMZ_Unmap( u32BufBaseAddr);
	HI_MPI_MMZ_Delete(u32BufBaseAddr);
    return 0;
}
