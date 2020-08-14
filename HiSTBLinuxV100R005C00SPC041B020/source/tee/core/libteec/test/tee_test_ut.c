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

#define QUERY_CMD 0x11
#define EXECUTE_CMD 0x12

void usage(const char *buffer)
{
	char *p = buffer;
	int i = 1;

	printf("It's tee framework ut test entry!\n");
	printf("Usage:tee_ut [options] [ut name]\n");
	printf("No options will list exist unit tests\n");
	printf("Options:\n ");
	printf("  -all:excute all exist ut\n");
	printf("[ut name]:excute the select ut\n");
	if (p != '\0') {
		printf("Here is unit test list:\n");
		while (*p != '\0') {
			printf("%d: %s\n", i, p);
			p += strlen(p) + 1;
			if ((p - buffer) > 256)
				break;
			i++;
		}
	} else
		printf("Here is no unit test\n");
}

int main(int argc, char *argv[])
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_Result result;
	TEEC_UUID svc_id = TEE_SERVICE_UT;
	int i = 1;
	char buffer1[512] = {0}, buffer2[512] = {0};
	char *p;
	uint32_t cmd;
	char delim[2] = ";";
	char all_flag = 0;
	int err = 0;

	if (argc > 1) {
		p = buffer1;
		while (i < argc) {
			if (!memcmp(argv[i], "-all", sizeof("-all"))) {
				all_flag = 1;
				break;
			} else {
				p = strncpy(p, argv[i], strlen(argv[i]));
				p += strlen(argv[i]);
				*(p++) = delim[0];   //replace '\0' to ';', easy for strtok
				i++;
			}
		}
	}

	result = TEEC_InitializeContext(
			 NULL,
			 &context);

	if (result != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed\n");
		err = 1;
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
		printf("TEEC_OpenSession failed\n");
		err = 1;
		goto cleanup_2;
	}/*else
        printf("open session success, session id = 0x%x\n", session.session_id);*/

	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.started = 1;
	operation.params[0].tmpref.buffer = buffer2;    //get unit tests names
	operation.params[0].tmpref.size = 512;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
						TEEC_NONE,
						TEEC_NONE,
						TEEC_NONE);
	cmd = QUERY_CMD;
	result = TEEC_InvokeCommand(
			 &session,
			 cmd,
			 &operation,
			 NULL);

	if (result != TEEC_SUCCESS) {
		printf("TEEC_InvokeCommand failed\n");
		err = 1;
		goto cleanup_3;
	}

	if (all_flag == 0 && *buffer1 == '\0') //no options, print usage
		usage(buffer2);
	else {
		char *token;
		char *save_ptr;
		char *str;

		if (all_flag) {
			memcpy(buffer1, buffer2, 512);
			for (str = buffer1;; str++) { //replace '\0' to ';', for easy to strtok
				if (*str == '\0') {
					if (*(str + 1) != '\0')
						*str = delim[0];
					else
						break;
				}
			}
		}

		for (str = buffer1;; str = NULL) {
			token = strtok_r(str, delim, &save_ptr);
			if (token == NULL)
				break;

			cmd = EXECUTE_CMD;
			operation.params[0].tmpref.buffer = token;    //put name to test
			operation.params[0].tmpref.size = strlen(token) + 1;
			operation.params[1].tmpref.buffer = buffer2;    //get test return buffer
			operation.params[1].tmpref.size = 512;
			operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
								TEEC_MEMREF_TEMP_OUTPUT,
								TEEC_NONE,
								TEEC_NONE);
			result = TEEC_InvokeCommand(
					 &session,
					 cmd,
					 &operation,
					 NULL);

			if (result != TEEC_SUCCESS) {
				printf("unit test \"%s\" failed : errno=%d\n", token, result);
				err = 1;
				goto cleanup_3;
			} else
				printf("unit test \"%s\" success\n", token);
		}

		printf("TEE unit test success!\n");
	}

cleanup_3:
	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	if (err)
		while (1);
	return 0;
}
