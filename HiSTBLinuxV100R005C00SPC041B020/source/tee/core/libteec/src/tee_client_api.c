/*
 * FileName:
 * Author:         h00211444  Version: 0.0.1  Date: 2012-12-24
 * Description:
 * Version:
 * Function List:
 *                 1.
 * History:
 *     <author>   <time>    <version >   <desc>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> /*for errno*/
#include <sys/types.h> /*for open close*/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h> /*for ioctl*/
#include <sys/mman.h> /*for mmap*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#ifdef _ANDROID_PLATFORM_HISI
#include <cutils/properties.h>
#endif
#include <time.h>
#include <securec.h>
#include "tee_log.h"
/* #define TEEC_DEBUG */
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tc_ns_client.h"
#include "tee_client_app_load.h"
#include "tee_ca_daemon.h"
/*lint -e835 -e838*/
#define TEE_ERROR_CA_AUTH_FAIL 0xFFFFCFE5

extern int32_t TEEC_StartApp_old(
	TEEC_Context * context,
	const TEEC_UUID *srv_uuid);

/* Socket from which the file descriptor is read */
static int recv_file_descriptor(int socketfd)
{
	/* int sent_fd; */
	struct msghdr message;
	struct iovec iov[1];
	struct cmsghdr *control_message = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];
	int res;
	errno_t rc = EOK;

	rc = memset_s(&message, sizeof(message), 0, sizeof(struct msghdr));
	if (EOK != rc)
		return -1;

	rc = memset_s(ctrl_buf, sizeof(ctrl_buf), 0, CMSG_SPACE(sizeof(int)));
	if (EOK != rc)
		return -1;

	/* For the dummy data */
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_control = ctrl_buf;
	message.msg_controllen = CMSG_SPACE(sizeof(int));
	message.msg_iov = iov;
	message.msg_iovlen = 1;

	res = recvmsg(socketfd, &message, 0);
	if (res <= 0) {
		return res;
	}

	/* Iterate through header to find if there is a file descriptor */
	for (control_message = CMSG_FIRSTHDR(&message);
	     control_message != NULL;
	     control_message = CMSG_NXTHDR(&message,	control_message)) {
		if ((control_message->cmsg_level == SOL_SOCKET)
			&& (control_message->cmsg_type == SCM_RIGHTS)) {
			return *((int *) CMSG_DATA(control_message)); /*lint !e826 */
		}
	}

	return -1;
}

static int ca_daemon_connect()
{
	/* int s, t, len, ret = 0; */
	int s, ret = 0;
	uint32_t len;
	struct sockaddr_un remote;
	errno_t rc = EOK;

	rc = memset_s(&remote, sizeof(remote), 0, sizeof(struct sockaddr_un));
	if (EOK != rc)
		return -1;

	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		tlogd("can't open stream socket, errno=%d", errno);
		return -1;
	}

	tlogd("Trying to connect...\n");

	remote.sun_family = AF_UNIX;
	(void)strncpy_s(remote.sun_path, sizeof(remote.sun_path),
		TC_NS_SOCKET_NAME, sizeof(TC_NS_SOCKET_NAME));
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	remote.sun_path[0] = 0;

	if (connect(s, (struct sockaddr *)&remote, len) == -1) { /*lint !e713 */
		tlogd("connect() failed, errno=%d", errno);
		close(s);
		return -1;
	}

	tlogd("Connected.\n");

	int fd = recv_file_descriptor(s);
	if (fd >= 0) {
		tlogd("FD received!");
		ret = fd;
	}

	close(s);

	return ret;
}

/*
 * Function:      TEEC_InitializeContext
 * Description:   This function initializes a new TEE Context, forming a connection between
 *                this Client Application and the TEE identified by the string identifier name.
 * Parameters:   name: a zero-terminated string that describes the TEE to connect to.
 *                If this parameter is set to NULL, the Implementation MUST select a default TEE.
 *                context: a TEEC_Context structure that be initialized by the Implementation.
 * Return:        TEEC_SUCCESS: the initialization was successful.
 *                     other: initialization was not successful.
 */
TEEC_Result TEEC_InitializeContext(const char *name, TEEC_Context *context)
{
	int32_t ret = 0;
	int32_t fail_count = 50;/* allow fail 50 times */
	int32_t check_times = 0;

	tlogd("open device begin\n");

	(void)name;  /* supress warning */

	/* First, check parameters is valid or not */
	if (context == NULL) {
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}

	/* add retry to avoid app start before daemon */
	do {
		ret = ca_daemon_connect();
		if (ret == -1)
			tlogd("open device failed, retry %d times\n", check_times);
		else
			break;

		/* sleep 200 ms */
		{
			struct timespec ts;
			ts.tv_sec = 0;
			ts.tv_nsec = 200 * 1000 * 1000;

			if (nanosleep(&ts, NULL) != 0) {
				tlogd("nanosleep 200 ms error\n");
			}
		}

		check_times++;
	} while (check_times <= fail_count);

	if (check_times > fail_count) {
		tlogd("connect ca daemon failed after retry %d times\n", check_times);
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	tlogd("open device success\n");
	context->fd = (uint32_t)ret; /*lint !e713 */
	context->ta_path = NULL;
	INIT_LIST_HEAD(&context->session_list);
	INIT_LIST_HEAD(&context->shrd_mem_list);

	if (ret < 0) {
		tloge("connect() failed, fd %d", ret);
		return TEEC_ERROR_GENERIC;
	}

	return TEEC_SUCCESS;

}

/*
 * Function:       TEEC_FinalizeContext
 * Description:   This function finalizes an initialized TEE Context.
 * Parameters:   context: an initialized TEEC_Context structure which is to be finalized.
 * Return:         NULL
 */
void TEEC_FinalizeContext(TEEC_Context *context)
{
	struct list_node *ptr;
	TEEC_Session *session;
	TEEC_SharedMemory *shrdmem;

	/* First, check parameters is valid or not */
	if (context == NULL)
		return;

	/* Paramters right, start execution */
	if (!LIST_EMPTY(&context->session_list)) {
		tlogd("context still has sessions opened, close it\n");

		list_for_each(ptr, &context->session_list) {
			session = list_entry(ptr, TEEC_Session, head); /*lint !e826 !e413 */
			TEEC_CloseSession(session);
		}
	}

	if (!LIST_EMPTY(&context->shrd_mem_list)) {
		tlogd("context contains unreleased Shared Memory blocks, release it\n");

		list_for_each(ptr, &context->shrd_mem_list) {
			shrdmem = list_entry(ptr, TEEC_SharedMemory, head); /*lint !e826 !e413 */
			TEEC_ReleaseSharedMemory(shrdmem);
		}
	}

	tlogd("close device\n");
	close((int)context->fd);
	context->fd = 0;
}
/*
 * Function:       TEEC_OpenSession
 * Description:   This function opens a new Session
 * Parameters:   context: a pointer to an initialized TEE Context.
 *                     session: a pointer to a Session structure to open.
 *                     destination: a pointer to a UUID structure.
 *                     connectionMethod: the method of connection to use.
 *                     connectionData: any necessary data required to support the connection method chosen.
 *                     operation: a pointer to an Operation containing a set of Parameters.
 *                     returnOrigin: a pointer to a variable which will contain the return origin.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_OpenSession(TEEC_Context *context, TEEC_Session *session,
				const TEEC_UUID *destination, uint32_t connectionMethod,
				const void *connectionData, TEEC_Operation *operation,
				uint32_t *returnOrigin)
{
	int32_t ret;
	TEEC_Result teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	uint32_t origin = TEEC_ORIGIN_API;
	TC_NS_ClientContext cli_context;
	TC_NS_ClientLogin cli_login = {0, 0};

	/* First, check parameters is valid or not */
	if ((context == NULL) || (session == NULL) || (destination == NULL))
		goto error;

	switch (connectionMethod) {
	case TEEC_LOGIN_PUBLIC:
	case TEEC_LOGIN_USER:
	case TEEC_LOGIN_APPLICATION:
	case TEEC_LOGIN_USER_APPLICATION:
		if (connectionData != NULL) {
			goto error;
		}
		break;

	case TEEC_LOGIN_GROUP:
	case TEEC_LOGIN_GROUP_APPLICATION:
		if (connectionData == NULL) {
			goto error;
		}
		cli_login.method = connectionMethod;
		cli_login.mdata = *(uint32_t *)connectionData;
		break;

	case TEEC_LOGIN_IDENTIFY:
		if (connectionData != NULL)
			goto error;

		if (operation == NULL)
			goto error;

		cli_login.method = connectionMethod;
		/* Params 2 and 3 are used for ident by teecd hence ->TEEC_NONE*/
		operation->paramTypes = TEEC_PARAM_TYPES(
						TEEC_PARAM_TYPE_GET(operation->paramTypes, 0),
						TEEC_PARAM_TYPE_GET(operation->paramTypes, 1),
						TEEC_NONE,
						TEEC_NONE); /*lint !e845 */
		break;

	default:
		goto error;
	}

	teec_ret = TEEC_CheckOperation(operation);
	if (teec_ret != TEEC_SUCCESS) {
		tlogd("operation is invalid\n");
		goto error;
	}

	/* if success, should return a service id */
	ret = TEEC_StartApp(context, destination);
	if (ret < 0) {
		tlogd("start app error\n");
		ret = TEEC_StartApp_old(context, destination);
		if (ret < 0) {
			teec_ret = (TEEC_Result)TEEC_ERROR_TRUSTED_APP_LOAD_ERROR;
			goto error;
		}
	}

	/* Paramters right, start execution */
	/*
	 * note:before open session success, we should send session=0 as initial state.
	 */
	TEEC_Encode(&cli_context, *destination, 0/*session->session_id*/,
			GLOBAL_CMD_ID_OPEN_SESSION,
			cli_login, operation);

	int i = 30;
	do {
		ret = ioctl((int)context->fd, (int)TC_NS_CLIENT_IOCTL_SES_OPEN_REQ, &cli_context);
		i--;
		tlogd("open session  ioctl end, code=0x%x, origin=%d\n", cli_context.returns.code,
				cli_context.returns.origin);
	} while ((TEE_ERROR_CA_AUTH_FAIL  == (TEEC_Result)cli_context.returns.code) && i);

	if (ret == 0) {
		tlogd("open session success\n");
		session->session_id = cli_context.session_id;
		session->service_id = *destination;
		session->ops_cnt = 0;
		INIT_LIST_HEAD(&session->head);
		list_insert_tail(&context->session_list, &session->head);
		session->context = context;
		/* operation->session = session; */
		teec_ret = TEEC_SUCCESS;
	} else if (ret < 0) {
		tlogd("open session failed, ioctl errno = %d\n", ret);

		if (ret == -EFAULT)
			teec_ret = (TEEC_Result)TEEC_ERROR_ACCESS_DENIED;
		else if (ret == -ENOMEM)
			teec_ret = (TEEC_Result)TEEC_ERROR_OUT_OF_MEMORY;
		else if (ret == -EINVAL)
			teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
		else
			teec_ret = (TEEC_Result)TEEC_ERROR_GENERIC;

		origin = TEEC_ORIGIN_COMMS;
	} else {
		tlogd("open session failed, code=0x%x, origin=%d\n", cli_context.returns.code,
			   cli_context.returns.origin);
		teec_ret = (TEEC_Result)cli_context.returns.code;
		origin = cli_context.returns.origin;
	}

error:

	/* ONLY when ioctl returnCode!=0 and returnOrigin not NULL,
	 * set *returnOrigin
	 */
	if ((teec_ret != TEEC_SUCCESS) && (returnOrigin))
		*returnOrigin = origin;

	return teec_ret;
}

/*
 * Function:       TEEC_CloseSession
 * Description:   This function closes an opened Session.
 * Parameters:   session: the session to close.
 * Return:         NULL
 */
void TEEC_CloseSession(TEEC_Session *session)
{
	int32_t ret;
	TC_NS_ClientContext cli_context;
	TC_NS_ClientLogin cli_login = {0, 0};
	struct list_node *ptr;
	TEEC_Session *temp_sess;
	bool found = false;

	/* First, check parameters is valid or not */
	if (session ==  NULL)
		return;

	list_for_each(ptr, &session->context->session_list) {
		temp_sess = list_entry(ptr, TEEC_Session, head); /*lint !e826 !e413 */

		if (temp_sess == session) {
			found = true;
			break;
		}
	}

	if (!found) {
		tlogd("session isnot in the context list\n");
		return;
	}

	/* Paramters all right, start execution */
	if (session->ops_cnt) {
		tlogd("session still has commands running\n");
	}

	TEEC_Encode(&cli_context, session->service_id, session->session_id,
		    GLOBAL_CMD_ID_CLOSE_SESSION, cli_login, NULL);

	ret = ioctl((int)session->context->fd, (int)TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ, &cli_context);
	if (ret == 0) {
		tlogd("close session success\n");
		session->session_id = 0;
		(void)memset_s((uint8_t *)&session->service_id, sizeof(session->service_id),
				0x00, 16);
		session->ops_cnt = 0;
		list_remove(&session->head);
		session->context = NULL;
	} else {
		tlogd("close session failed\n");
	}
}

/*
 * Function:       TEEC_InvokeCommand
 * Description:   This function invokes a Command within the specified Session.
 * Parameters:   session: the open Session in which the command will be invoked.
 *                     commandID: the identifier of the Command.
 *                     operation: a pointer to an Operation containing a set of Parameters.
 *                     returnOrigin: a pointer to a variable which will contain the return origin.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_InvokeCommand(TEEC_Session *session, uint32_t commandID,
						TEEC_Operation *operation, uint32_t *returnOrigin)
{
	int32_t ret;
	TEEC_Result teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	uint32_t origin = TEEC_ORIGIN_API;
	TC_NS_ClientContext cli_context;
	TC_NS_ClientLogin cli_login = {0, 0};

	/* First, check parameters is valid or not */
	if (session == NULL)
		goto error;

	teec_ret = TEEC_CheckOperation(operation);
	if (teec_ret != TEEC_SUCCESS) {
		tlogd("operation is invalid\n");
		goto error;
	}

	/* Paramters all right, start execution */
	session->ops_cnt++;
	TEEC_Encode(&cli_context, session->service_id, session->session_id, commandID, cli_login,
		operation);

	ret = ioctl((int)session->context->fd, (int)TC_NS_CLIENT_IOCTL_SEND_CMD_REQ, &cli_context);
	if (ret == 0) {
		tlogd("invoke cmd success\n");
		teec_ret = TEEC_SUCCESS;
	} else if (ret < 0) {
		tlogd("invoke cmd failed, ioctl errno = %d\n", ret);

		if (ret == -EFAULT)
			teec_ret = (TEEC_Result)TEEC_ERROR_ACCESS_DENIED;
		else if (ret == -ENOMEM)
			teec_ret = (TEEC_Result)TEEC_ERROR_OUT_OF_MEMORY;
		else if (ret == -EINVAL)
			teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
		else
			teec_ret = (TEEC_Result)TEEC_ERROR_GENERIC;

		origin = TEEC_ORIGIN_COMMS;
	} else {
		tlogd("invoke cmd failed, code=0x%x, origin=%d\n", cli_context.returns.code,
				cli_context.returns.origin);
		teec_ret = (TEEC_Result)cli_context.returns.code;
		origin = cli_context.returns.origin;
	}

	session->ops_cnt--;

error:

	/* ONLY when ioctl returnCode!=0 and returnOrigin not NULL,
	 * set *returnOrigin
	 */
	if ((teec_ret != TEEC_SUCCESS) && (returnOrigin)) {
		*returnOrigin = origin;
	}

	return teec_ret;
}

/*
 * Function:       TEEC_RegisterSharedMemory
 * Description:   This function registers a block of existing Client Application memory
 *                     as a block of Shared Memory within the scope of the specified TEE Context.
 * Parameters:   context: a pointer to an initialized TEE Context.
 *                     sharedMem: a pointer to a Shared Memory structure to register.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_RegisterSharedMemory(TEEC_Context *context, TEEC_SharedMemory *sharedMem)
{
	/* First, check parameters is valid or not */
	if ((context == NULL) || (sharedMem == NULL)) {
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}

	if ((sharedMem->buffer == NULL)
		|| ((sharedMem->flags != TEEC_MEM_INPUT)
			&& (sharedMem->flags != TEEC_MEM_OUTPUT)
			&& (sharedMem->flags != TEEC_MEM_INOUT))) {
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}

	/* Paramters all right, start execution */
	sharedMem->ops_cnt = 0;
	sharedMem->is_allocated = false;
	INIT_LIST_HEAD(&sharedMem->head);
	list_insert_tail(&context->shrd_mem_list, &sharedMem->head);
	sharedMem->context = context;

	return TEEC_SUCCESS;
}

/*
 * Function:       TEEC_AllocateSharedMemory
 * Description:   This function allocates a new block of memory as a block of
 *                     Shared Memory within the scope of the specified TEE Context.
 * Parameters:   context: a pointer to an initialized TEE Context.
 *                     sharedMem: a pointer to a Shared Memory structure to allocate.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_AllocateSharedMemory(TEEC_Context *context, TEEC_SharedMemory *sharedMem)
{
	TEEC_Result teec_ret;

	/* First, check parameters is valid or not */
	if ((context == NULL) || (sharedMem == NULL)) {
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}

	if ((sharedMem->flags != TEEC_MEM_INPUT)
		&& (sharedMem->flags != TEEC_MEM_OUTPUT)
		&& (sharedMem->flags != TEEC_MEM_INOUT)) {
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}

	/* Paramters all right, start execution */
	sharedMem->buffer = NULL;

	if (sharedMem->size != 0) {
		sharedMem->buffer = mmap(0, sharedMem->size, (PROT_READ | PROT_WRITE),
					MAP_SHARED, (int)context->fd, 0);
	}

	if (sharedMem->buffer == MAP_FAILED) {
		tlogd("mmap failed\n");
		sharedMem->buffer = NULL;
		teec_ret = (TEEC_Result)TEEC_ERROR_OUT_OF_MEMORY;
	} else {
		sharedMem->ops_cnt = 0;
		sharedMem->is_allocated = true;
		INIT_LIST_HEAD(&sharedMem->head);
		list_insert_tail(&context->shrd_mem_list, &sharedMem->head);
		sharedMem->context = context;
		teec_ret = TEEC_SUCCESS;
	}

	return teec_ret;
}

/*
 * Function:       TEEC_ReleaseSharedMemory
 * Description:   This function deregisters or deallocates a previously initialized
 *                      block of Shared Memory..
 * Parameters:   sharedMem: a pointer to a valid Shared Memory structure.
 * Return:         NULL
 */
void TEEC_ReleaseSharedMemory(TEEC_SharedMemory *sharedMem)
{
	int32_t ret;
	struct list_node *ptr;
	TEEC_SharedMemory *temp_shardmem;
	bool found = false;
	/* DTS2013030408264 h00211444 begin */
	/* void* rel_addr; */
	/* DTS2013030408264 h00211444 end */

	/* First, check parameters is valid or not */
	if (sharedMem == NULL) {
		return;
	}

	if (sharedMem->ops_cnt) {
		tlogd("Shared Memory is used by a pending operation\n");
		return;
	}

	list_for_each(ptr, &sharedMem->context->shrd_mem_list) {
		temp_shardmem = list_entry(ptr, TEEC_SharedMemory, head); /*lint !e826 !e413 */
		if (temp_shardmem == sharedMem) {
			found = true;
			break;
		}
	}

	if (!found) {
		tlogd("Shared Memory isnot in the list\n");
		return;
	}

	/* Paramters right, start execution */
	if ((sharedMem->is_allocated) && (sharedMem->size != 0)) {
		/* DTS2013030408264 h00211444 begin */
		/* rel_addr = (void*)sharedMem->buffer; */
		/* call munmap, notify os decrease mmap count */
		ret = munmap(sharedMem->buffer, sharedMem->size);
		if (ret) {
			tlogd("Release SharedMemory failed, munmap error\n");
			return;
		}

		/* call ioctl, notify driver release memory */
		/* ret = ioctl(sharedMem->context->fd, */
		/*         TC_NS_CLIENT_IOCTL_SHRD_MEM_RELEASE, rel_addr); */
		/* if (ret) { */
		/*     tlogd("release SharedMemory failed, ioctl error\n"); */
		/*     return; */
		/* } */
		/* DTS2013030408264 h00211444 end */
	}

	sharedMem->buffer = NULL;
	sharedMem->size = 0;
	sharedMem->flags = 0;
	sharedMem->ops_cnt = 0;
	list_remove(&sharedMem->head);
	sharedMem->context = NULL;
}

static TEEC_Result TEEC_CheckTmpRef(TEEC_TempMemoryReference tmpref)
{
	TEEC_Result ret;

	if ((tmpref.buffer == NULL) || (tmpref.size == 0)) {
		tlogd("tmpref buffer is null, or size is zero\n");
		ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	} else {
		ret = (TEEC_Result)TEEC_SUCCESS;
	}

	return ret;
}

static TEEC_Result TEEC_CheckMemRef(TEEC_RegisteredMemoryReference memref,	uint32_t param_type)
{
	if ((memref.parent == NULL) || (memref.parent->buffer == NULL)) {
		tlogd("parent of memref is null, or the buffer is zero\n");
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}

	if (param_type == TEEC_MEMREF_PARTIAL_INPUT) {
		if (!(memref.parent->flags & TEEC_MEM_INPUT))
			goto param_error;
	} else if (param_type == TEEC_MEMREF_PARTIAL_OUTPUT) {
		if (!(memref.parent->flags & TEEC_MEM_OUTPUT))
			goto param_error;
	} else if (param_type == TEEC_MEMREF_PARTIAL_INOUT) {
		/* DTS2013030509162 h00211444 begin */
		if (!(memref.parent->flags & TEEC_MEM_INPUT))
			goto param_error;

		if (!(memref.parent->flags & TEEC_MEM_OUTPUT))
			goto param_error;

		/* DTS2013030509162 h00211444 end */
	} else {
		/*  if type is TEEC_MEMREF_WHOLE, ignore it */
	}

	if ((param_type == TEEC_MEMREF_PARTIAL_INPUT)
		|| (param_type == TEEC_MEMREF_PARTIAL_OUTPUT)
		|| (param_type == TEEC_MEMREF_PARTIAL_INOUT)) {
		if ((memref.offset + memref.size) > memref.parent->size) {
			tlogd("offset + size exceed the parent size\n");
			return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
		}
	}

	return (TEEC_Result)TEEC_SUCCESS;
param_error:
	tlogd("type of memref not belong to the parent flags\n");
	return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
}

/*
 * Function:       TEEC_CheckOperation
 * Description:   This function checks a operation is valid or not.
 * Parameters:   operation: a pointer to an Operation to be checked.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_CheckOperation(TEEC_Operation *operation)
{
	uint32_t param_type[4];
	uint32_t param_cnt;
	TEEC_Result ret = TEEC_SUCCESS;
	/* GP Support operation is NULL
	 * operation: a pointer to a Client Application initialized TEEC_Operation structure,
	 * or NULL if there is no payload to send or if the Command does not need to support
	 * cancellation.
	 * */
	if (operation == NULL)
		return ret;

	if (!operation->started) {
		tlogd("sorry, cancellation not support\n");
		return (TEEC_Result)TEEC_ERROR_NOT_IMPLEMENTED;
	}

	param_type[0] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 0); /*lint !e845 */
	param_type[1] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 1);
	param_type[2] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 2);
	param_type[3] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 3);

	for (param_cnt = 0; param_cnt < 4; param_cnt++) {
		if ((param_type[param_cnt] == TEEC_MEMREF_TEMP_INPUT)
			|| (param_type[param_cnt] == TEEC_MEMREF_TEMP_OUTPUT)
			|| (param_type[param_cnt] == TEEC_MEMREF_TEMP_INOUT)) {

			if ((ret = TEEC_CheckTmpRef(operation->params[param_cnt].tmpref))
				!= TEEC_SUCCESS) {
				tlogd("param_cnt is %d\n", param_cnt);
				break;
			}
		} else if ((param_type[param_cnt] == TEEC_MEMREF_WHOLE)
				|| (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INPUT)
				|| (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_OUTPUT)
				|| (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INOUT)) {

			if ((ret = TEEC_CheckMemRef(operation->params[param_cnt].memref,
							param_type[param_cnt])) != TEEC_SUCCESS)
				break;
		} else if ((param_type[param_cnt] ==  TEEC_VALUE_INPUT)
				|| (param_type[param_cnt] == TEEC_VALUE_OUTPUT)
				|| (param_type[param_cnt] == TEEC_VALUE_INOUT)) {
			/*  if type is value, ignore it */
		} else if (param_type[param_cnt] == TEEC_ION_INPUT) {
			if (operation->params[param_cnt].ionref.ion_share_fd < 0
				|| operation->params[param_cnt].ionref.ion_size == 0) {
				tlogd("check failed: ion_share_fd=%d and ion_size=%d!\n",
					operation->params[param_cnt].ionref.ion_share_fd,
					operation->params[param_cnt].ionref.ion_size);
				ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
				break;
			}
		} else if (param_type[param_cnt] == TEEC_NONE) {
			/*  if type is none, ignore it */
		} else {
			tlogd("paramType[%d]=%x is not support\n",
					param_cnt, param_type[param_cnt]);
			ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
			break;
		}
	}

	return ret;
}

void TEEC_Encode(TC_NS_ClientContext *cli_context, TEEC_UUID service_id,
			uint32_t session_id, uint32_t cmd_id, TC_NS_ClientLogin cli_login,
			TEEC_Operation *operation)
{
	uint32_t param_type[4];
	uint32_t param_cnt;
	uint32_t diff;
	errno_t rc = EOK;

	diff = (uint32_t)TEEC_MEMREF_PARTIAL_INPUT - (uint32_t)TEEC_MEMREF_TEMP_INPUT;

	rc = memset_s(cli_context, sizeof(TC_NS_ClientContext), 0x00, sizeof(TC_NS_ClientContext));
	if (EOK != rc) {
		return;
	}

	rc = memcpy_s(cli_context->uuid, sizeof(cli_context->uuid), (uint8_t *)&service_id, 16);
	if (EOK != rc) {
		return;
	}

	cli_context->session_id = session_id;
	cli_context->cmd_id = cmd_id;
	cli_context->returns.code = 0;
	cli_context->returns.origin = 0;

	cli_context->login.method = cli_login.method;
	cli_context->login.mdata = cli_login.mdata;

	if ((operation == NULL) || (!operation->paramTypes))
		return;

	cli_context->started = operation->cancel_flag;
	param_type[0] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 0); /*lint !e845 */
	param_type[1] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 1);
	param_type[2] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 2);
	param_type[3] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 3);

	for (param_cnt = 0; param_cnt < 4; param_cnt++) {
		if ((param_type[param_cnt] == TEEC_MEMREF_TEMP_INPUT)
		|| (param_type[param_cnt] == TEEC_MEMREF_TEMP_OUTPUT)
		|| (param_type[param_cnt] == TEEC_MEMREF_TEMP_INOUT)) {

			TEEC_TempMemoryReference *tmpref = &operation->params[param_cnt].tmpref;

			cli_context->params[param_cnt].memref.buffer
				= (unsigned long)tmpref->buffer;
			cli_context->params[param_cnt].memref.buffer_h_addr
				= ((unsigned long)tmpref->buffer) >> 32;
			cli_context->params[param_cnt].memref.size_addr
				= (unsigned long)&tmpref->size;
			cli_context->params[param_cnt].memref.size_h_addr
				= ((unsigned long)&tmpref->size) >> 32;
		} else if ((param_type[param_cnt] == TEEC_MEMREF_WHOLE)
			|| (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INPUT)
			|| (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_OUTPUT)
			|| (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INOUT)) {

			TEEC_RegisteredMemoryReference *memref =
					&operation->params[param_cnt].memref;

			/* buffer offset len */
			if (param_type[param_cnt] == TEEC_MEMREF_WHOLE) {

				cli_context->params[param_cnt].memref.offset = 0;
				cli_context->params[param_cnt].memref.size_addr
					= (unsigned long)&memref->parent->size;
				cli_context->params[param_cnt].memref.size_h_addr
					= ((unsigned long)&memref->parent->size) >> 32;
			} else {
				cli_context->params[param_cnt].memref.offset
					= memref->offset;
				cli_context->params[param_cnt].memref.size_addr
					= (unsigned long)&memref->size;
				cli_context->params[param_cnt].memref.size_h_addr
					= ((unsigned long)&memref->size) >> 32;
			}

			if (operation->params[param_cnt].memref.parent->is_allocated) {
				cli_context->params[param_cnt].memref.buffer
					= (unsigned long)memref->parent->buffer;
				cli_context->params[param_cnt].memref.buffer_h_addr
					= ((unsigned long)memref->parent->buffer) >> 32;
			} else {
				cli_context->params[param_cnt].memref.buffer
					= (unsigned long)((unsigned char *)memref->parent->buffer
					  + memref->offset);
				cli_context->params[param_cnt].memref.buffer_h_addr
					= (unsigned long)(
						(unsigned char *)memref->parent->buffer
						+ memref->offset) >> 32;
				cli_context->params[param_cnt].memref.offset = 0;
			}

			/* DTSDTS2013030104869 h00211444 begin */
			/* translate the paramType to know the driver */
			if (param_type[param_cnt] == TEEC_MEMREF_WHOLE) {
				switch (memref->parent->flags) {
				case TEEC_MEM_INPUT:
					param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INPUT;
					break;

				case TEEC_MEM_OUTPUT:
					param_type[param_cnt] = TEEC_MEMREF_PARTIAL_OUTPUT;
					break;

				case TEEC_MEM_INOUT:
					param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INOUT;
					break;

				default:
					param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INOUT;
					break;
				}
			}

			/* if is not allocated,
			 * translate TEEC_MEMREF_PARTIAL_XXX to TEEC_MEMREF_TEMP_XXX */
			if (!memref->parent->is_allocated)
				param_type[param_cnt] = param_type[param_cnt] - diff;

			/* DTSDTS2013030104869 h00211444 end */
		}  else if ((param_type[param_cnt] ==  TEEC_VALUE_INPUT)
				|| (param_type[param_cnt] == TEEC_VALUE_OUTPUT)
				|| (param_type[param_cnt] == TEEC_VALUE_INOUT)) {
			TEEC_Value *value = &operation->params[param_cnt].value;

			cli_context->params[param_cnt].value.a_addr
				= (unsigned long)&value->a;
			cli_context->params[param_cnt].value.a_h_addr
				= ((unsigned long)&value->a) >> 32;
			cli_context->params[param_cnt].value.b_addr
				= (unsigned long)&value->b;
			cli_context->params[param_cnt].value.b_h_addr
				= ((unsigned long)&value->b) >> 32;
		} else if (param_type[param_cnt] == TEEC_ION_INPUT) {
			TEEC_IonReference *ionref = &operation->params[param_cnt].ionref;
			cli_context->params[param_cnt].value.a_addr
				= (unsigned long)&ionref->ion_share_fd;
			cli_context->params[param_cnt].value.a_h_addr
				= ((unsigned long)&ionref->ion_share_fd) >> 32;
			cli_context->params[param_cnt].value.b_addr
				= (unsigned long)&ionref->ion_size;
			cli_context->params[param_cnt].value.b_h_addr
				= ((unsigned long)&ionref->ion_size) >> 32;
		} else {
			/* if type is none, ignore it */
		}
	}

	cli_context->paramTypes = TEEC_PARAM_TYPES(param_type[0], param_type[1], param_type[2],
				  param_type[3]);
	tlogd("cli param type %x\n", cli_context->paramTypes);
}
/*
 * Function:       TEEC_RequestCancellation
 * Description:   This function requests the cancellation of a pending open Session operation or
			a Command invocation operation.
 * Parameters:   operation:a pointer to a Client Application instantiated Operation structure
 * Return:         void
 */
void TEEC_RequestCancellation(TEEC_Operation *operation)
{
	int32_t ret;
	TEEC_Result teec_ret;
	TC_NS_ClientContext cli_context;
	TC_NS_ClientLogin cli_login = {0, 0};
	TEEC_Session *session;

	/* First, check parameters is valid or not */
	if (operation == NULL) {
		return;
	}

	session = operation->session;

	teec_ret = TEEC_CheckOperation(operation);
	if (teec_ret != TEEC_SUCCESS) {
		tlogd("operation is invalid\n");
		return;
	}

	/* Paramters all right, start execution */
	/*  session->ops_cnt++; */
	TEEC_Encode(&cli_context, session->service_id, session->session_id,
			TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ, cli_login, operation);

	ret = ioctl((int)session->context->fd,
		(int)TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ, &cli_context);
	if (ret == 0) {
		tlogd("invoke cmd success\n");
	} else if (ret < 0) {
		tlogd("invoke cmd failed, ioctl errno = %d\n", ret);
	} else {
		tlogd("invoke cmd failed, code=0x%x, origin=%d\n", cli_context.returns.code,
			   cli_context.returns.origin);
	}

	return;
}

TEEC_Result TEEC_EXT_GetTEEInfo(TEEC_EXT_TEEInfo *tee_info_data, uint32_t *length)
{
	if (NULL == tee_info_data || *length != sizeof(TEEC_EXT_TEEInfo)) {
		tlogd("input param error:tee_info_data=%p, *length=%u\n", tee_info_data, *length);
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}

	tee_info_data->version = TEEC_VERSION;
	tee_info_data->reserve1 = TEEC_VALUE_UNDEF;
	tee_info_data->reserve2 = TEEC_VALUE_UNDEF;
	tee_info_data->reserve3 = TEEC_VALUE_UNDEF;
	return TEEC_SUCCESS;
}

TEEC_Result TEEC_EXT_RegisterAgent(uint32_t agent_id, int *dev_fd, void **buffer)
{
	int fd;
	void *buff = NULL;
	int ret;

	if (NULL == dev_fd || NULL == buffer) {
		tlogd("Failed to open tee client dev!\n");
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	fd = ca_daemon_connect();
	if (fd < 0) {
		tlogd("Failed to open tee client dev!\n");
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	buff = mmap(0, 0x1000, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
	if ((void *)buff == MAP_FAILED) {
		(void)close(fd);
		tlogd("mmap failed while register agent!\n");
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	ret = ioctl(fd, (int)TC_NS_CLIENT_IOCTL_REGISTER_AGENT, agent_id);
	if (ret) {
		ret = munmap(buff, 0x1000);
		if (ret) {
			tlogd("munmap failed while register agent!\n");
		}

		(void)close(fd);
		tlogd("ioctl failed, failed to register agent!\n");
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	*dev_fd = fd;
	*buffer = buff;

	return TEEC_SUCCESS;
}

TEEC_Result TEEC_EXT_WaitEvent(uint32_t agent_id, int dev_fd)
{
	int ret;

	do {
		ret = ioctl(dev_fd, (int)TC_NS_CLIENT_IOCTL_WAIT_EVENT, agent_id);
		if (!ret) {
			tlogd("Agent %d succeed to wait event!\n", agent_id);
			break;
		}

		tlogd("Agent %d thread interuptted by signals, just ignore!\n", agent_id);
	} while (ret);

	return TEEC_SUCCESS;
}

TEEC_Result TEEC_EXT_SendEventResponse(uint32_t agent_id, int dev_fd)
{
	int ret;

	ret = ioctl(dev_fd, (int)TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE, agent_id);
	if (ret) {
		tlogd("Agent %d failed to send response, ret is %d!\n", agent_id, ret);
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	return TEEC_SUCCESS;
}

TEEC_Result TEEC_EXT_UnregisterAgent(uint32_t agent_id, int dev_fd, void **buffer)
{
	int ret;

	ret = ioctl(dev_fd, (int)TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT, agent_id);
	if (ret) {
		tlogd("Failed to unregister agent %d, ret is %d\n", agent_id, ret);
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	(void)close(dev_fd);
	*buffer = NULL;
	return TEEC_SUCCESS;
}

TEEC_Result TEEC_EXT_TST(TEEC_Session *session, TEEC_Operation *operation)
{
	TC_NS_ClientContext cli_context;
	TC_NS_ClientLogin cli_login = {0, 0};
	int32_t ret;
	TEEC_Result result;

	if (NULL == session) {
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	TEEC_Encode(&cli_context, session->service_id, 0/*session->session_id*/,
			GLOBAL_CMD_ID_OPEN_SESSION, cli_login, operation);
	ret = ioctl((int)session->context->fd, (int)TC_NS_CLIENT_IOCTL_TST_CMD_REQ, &cli_context);
	if (ret == 0)
		result = TEEC_SUCCESS;
	else
		result = (TEEC_Result)TEEC_ERROR_GENERIC;

	return result;
}


TEEC_Result TEEC_EXT_Tui_send_event(TEEC_TUI_Parameter *tui_param)
{
	int m_tuifd = -1;
	int ret = TEEC_SUCCESS;
	TEEC_Result result;

	m_tuifd = ca_daemon_connect();
	if (m_tuifd < 0) {
		tlogd("Failed to open tee client dev!\n");
		return (TEEC_Result)TEEC_ERROR_GENERIC;
	}

	ret = ioctl(m_tuifd, (int)TC_NS_CLIENT_IOCTL_TUI_EVENT, tui_param);
	if (ret) {
		result = (TEEC_Result)TEEC_ERROR_GENERIC;
		tlogd("Failed to sened event event_type %d, ret is %d\n", tui_param->event_type, ret);
	} else
		result = TEEC_SUCCESS;

	(void)close(m_tuifd);
	return result;
}
