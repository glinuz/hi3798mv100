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

#include <pthread.h>
#include <signal.h>
#include <assert.h>

#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_storage_api.h"

//#define TEEC_DEBUG 1
#define TEE_TEST_AGENT  0x33
typedef struct thread_param_t {
	int agent_id;
	int repeat_cnt;
} thread_param;

void *agent_thread(void *agent_id);
static volatile int count = 0;
static pthread_mutex_t __count_lock;
static pthread_cond_t  cond;    /* condition to wait on / signal */
static pthread_mutex_t mtx;     /* mutex for the above */
static unsigned int done = 0;

void usage(void)
{
	printf("please input the number of agents you want to create!\n");
	printf("and the repeating times responds to TA\n");
	printf("tee_test_agent <num of agents> <repeating times in each agent>\n");
	printf("default test case: tee_test_agent 3 3\n");
}
int main(int argc, char *argv[])
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result = TEEC_SUCCESS;
	TEEC_Operation operation;
	TEEC_UUID svc_id = TEE_SERVICE_ECHO;
	pthread_t *threads;//[AGENT_CNT_MAX];
	int i, ret;
	void *pthread_ret;
	int agent_cnt, repeat_cnt;
	thread_param *param;
	pthread_attr_t attr;
	sigset_t set;

	if (argc < 3) {
		usage();
		agent_cnt = 3;
		repeat_cnt = 3;
	} else if (argc == 3) {
		agent_cnt = atoi(argv[1]);
		repeat_cnt = atoi(argv[2]);
	} else {
		usage();
		return -1;
	}

	sigemptyset(&set);
	sigfillset(&set);
	ret = pthread_sigmask(SIG_BLOCK, &set, NULL);
	if (ret)
		TEEC_Error("pthread_sigmask Failed");
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond, NULL);
	threads = (pthread_t *) malloc(sizeof(pthread_t) * agent_cnt);
	if (!threads) {
		TEEC_Error("Failed to get free memory, exits\n!");
		return -2;
	}
	param = (thread_param *) malloc(sizeof(thread_param) * agent_cnt);
	if (!param) {
		TEEC_Error("Failed to get free memory, exits\n!");
		free(threads);
		return -2;
	}
	pthread_mutex_init(&__count_lock, NULL);

	pthread_attr_init(&attr);

	printf("start tee_test_agent %d %d\n", agent_cnt, repeat_cnt);
	for (i = 0; i < agent_cnt; i++) {
		param[i].agent_id = i;
		param[i].repeat_cnt = repeat_cnt;
		ret = pthread_create(&threads[i], &attr, &agent_thread, (void *)&param[i]);
		if (ret) {
			TEEC_Error("Failed to create thread %d\n", i);
			free(threads);
			return -3;
		}
	}
	while (count != agent_cnt) {
		TEEC_Debug("Wait for agent threads to be ready before open session, count is %d\n", count);
		sleep(1);
	}
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
	TEEC_Debug("tee_test_agent session id 0x%x\n", session.session_id);

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE);
	operation.params[0].value.a = agent_cnt;
	operation.params[0].value.b = repeat_cnt;

	result = TEEC_InvokeCommand(
			 &session,
			 TEE_TEST_AGENT,
			 &operation,
			 NULL);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("tee_test_agent invoke failed\n");
		goto cleanup_3;
	} else {
		TEEC_Debug("[0] : %x %x\n", operation.params[0].value.a, operation.params[0].value.b);
	}

	pthread_mutex_lock(&mtx);
	done = 1;
	//pthread_cond_signal(&cond);
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mtx);
	/* Signal the others the invoke has finished, it's ok to unregister */
	for (i = 0; i < agent_cnt; i++) {
		ret = pthread_join(threads[i], &pthread_ret);
		if (ret) {
			TEEC_Error("Failed to join thread %d\n", i);
			goto cleanup_3;
		}
	}
	TEEC_Debug("tee_test_agent test cases all successful\n");
	printf("tee_test_agent test cases all successfully completed!\n");
cleanup_3:
	TEEC_CloseSession(&session);
cleanup_2:
	TEEC_FinalizeContext(&context);
cleanup_1:
	free(threads);
	return result;
}

void *agent_thread(void *param)
{
	TEEC_Result ret;
	int dev_fd = 0;
	uint32_t agent_id = (uint32_t)((thread_param *)param)->agent_id;
	int repeat_cnt = ((thread_param *)param)->repeat_cnt;
	int temp_rep_cnt = repeat_cnt;
	void *buffer;

	TEEC_Debug("agent thread %d, to repeat %d times waiting msg from TA!\n", agent_id, repeat_cnt);
	ret = TEEC_EXT_RegisterAgent(agent_id, &dev_fd, &buffer);
	pthread_mutex_lock(&__count_lock);
	++count;
	pthread_mutex_unlock(&__count_lock);
	if (ret != TEEC_SUCCESS) {
		TEEC_Error("Failed to register agent %d\n", agent_id);
		printf("Failed to register agent %d\n", agent_id);
		goto exit;
	}
	do {
		ret = TEEC_EXT_WaitEvent(agent_id, dev_fd);
		if (ret != TEEC_SUCCESS) {
			TEEC_Error("Failed to wait event from TA!\n");
			goto cleanup;
		}

		TEEC_Debug("agent thread %d received msg from TA\n", agent_id);
		TEEC_Debug("The msg from TA is: %s\n", (char *)buffer);
		memset(buffer, 0, 0x1000);
		snprintf((char *)buffer, 0x1000, "Simulate CA send response to TA (agent id %d, the %d time) passively\n", agent_id, temp_rep_cnt - repeat_cnt);
		ret = TEEC_EXT_SendEventResponse(agent_id, dev_fd);
		if (ret != TEEC_SUCCESS) {
			TEEC_Error("Failed to send response to TA\n");
			goto cleanup;
		}
	} while (--repeat_cnt);
	printf("test agent %d succeed to receive msg and response from/to TA\n", agent_id);

	/* Wait for Invoke to end so we can unregister the agent */
	pthread_mutex_lock(&mtx);
	if (!done)
		pthread_cond_wait(&cond, &mtx);
	pthread_mutex_unlock(&mtx);

cleanup:
	ret = TEEC_EXT_UnregisterAgent(agent_id, dev_fd, &buffer);
	if (ret != TEEC_SUCCESS) {
		TEEC_Error("Failed to unregister agent\n");
	}
exit:
	pthread_exit(NULL);
	return NULL;
}
