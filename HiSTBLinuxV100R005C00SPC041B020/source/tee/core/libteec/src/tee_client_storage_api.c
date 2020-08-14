#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <securec.h>
#include "tee_client_api.h"
#include "tee_client_id.h"
#include "tee_client_storage_api.h"
#include "tee_log.h"
/*lint -e835 -e838*/

static TEEC_Context g_context;
static TEEC_Session g_session;
static TEEC_Result  g_errno;

static void TEEC_InitGVar()
{
	errno_t rc = EOK;

	rc = memset_s(&g_context, sizeof(g_context), 0x00, sizeof(TEEC_Context));
	if (EOK != rc) {
		g_errno = (TEEC_Result)TEEC_ERROR_GENERIC;
		return;
	}

	rc = memset_s(&g_session, sizeof(g_session), 0x00, sizeof(TEEC_Session));
	if (EOK != rc) {
		g_errno = (TEEC_Result)TEEC_ERROR_GENERIC;
		return;
	}

	g_errno = TEEC_SUCCESS;
}

TEEC_Result TEEC_InitStorageService()
{
	TEEC_Result result;
	TEEC_UUID svc_id = TEE_SERVICE_STORAGE;
	TEEC_Operation operation;

	TEEC_InitGVar();

	/* First, Initialize Context */
	result = TEEC_InitializeContext(NULL, &g_context);

	if (result != TEEC_SUCCESS) {
		tloge("TEEC_InitializeContext failed\n");
		goto cleanup_1;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_INPUT); /*lint !e845 */


	/* Second, Open Session */
	result = TEEC_OpenSession(
			 &g_context,
			 &g_session,
			 &svc_id,
			 TEEC_LOGIN_IDENTIFY,
			 NULL,
			 &operation,
			 NULL);
	if (result != TEEC_SUCCESS) {
		tloge("TEEC_OpenSession failed\n");
		goto cleanup_2;
	}

	return result;

cleanup_2:
	TEEC_FinalizeContext(&g_context);
	TEEC_InitGVar();
cleanup_1:
	return result;
}

TEEC_Result TEEC_UninitStorageService()
{
	if ((g_context.fd == 0) || (g_session.session_id == 0))
		return (TEEC_Result)TEEC_ERROR_GENERIC;

	(void)TEEC_InvokeCommand(&g_session, STORAGE_CMD_ID_CLOSEALL, NULL, NULL);
	TEEC_CloseSession(&g_session);
	TEEC_FinalizeContext(&g_context);
	TEEC_InitGVar();

	return TEEC_SUCCESS;
}

int32_t TEEC_FOpen(char *path, uint32_t mode)
{
	TEEC_Operation operation;
	TEEC_Result result;
	int32_t fd = -1;
	uint32_t origin;

	if (path == NULL) {
		tloge("TEEC_FOpen failed, path is null\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
		return -1;
	}

	if ((mode & TEE_DATA_FLAG_CREATE) && (mode & TEE_DATA_FLAG_EXCLUSIVE)) {
		tloge("TEEC_FOpen failed, mode error, CREATE&EXCLUSIVE canot be concurrence\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
		return -1;
	}

	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0)) {
		tloge("TEEC_FOpen failed, Please Init Storage Service\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_STATE;
		return -1;
	}

	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_VALUE_INPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_NONE);
	operation.params[0].tmpref.buffer = path;
	operation.params[0].tmpref.size = strlen(path);
	operation.params[1].value.a = mode;
	operation.params[1].value.b = TEEC_VALUE_UNDEF;
	operation.params[2].value.a = fd; /*lint !e732 */
	operation.params[2].value.b = TEEC_VALUE_UNDEF;

	result = TEEC_InvokeCommand(
			 &g_session,
			 STORAGE_CMD_ID_OPEN,
			 &operation,
			 &origin);

	g_errno = result;

	if (result != TEEC_SUCCESS) {
		tloge("---->OpenFile Failed:0x%x, origin:0x%x\n", result, origin);
		return -1;
	}

	fd = operation.params[2].value.a; /*lint !e713 */
	tlogd("---->OpenFile Success,fd=0x%x\n", fd);
	return fd;
}

int32_t TEEC_FClose(int32_t fd)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;

	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0)) {
		tloge("TEEC_FClose failed, Please Init Storage Service\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_STATE;
		return -1;
	}

	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE); /*lint !e845 */
	operation.params[0].value.a = fd; /*lint !e732 */
	operation.params[0].value.b = TEEC_VALUE_UNDEF;

	result = TEEC_InvokeCommand(
			 &g_session,
			 STORAGE_CMD_ID_CLOSE,
			 &operation,
			 &origin);

	g_errno = result;

	if (result != TEEC_SUCCESS) {
		tloge("---->CloseFile Failed:0x%x, origin:0x%x\n", result, origin);
		return -1;
	}

	tlogd("---->CloseFile success\n");
	return 0;
}

size_t TEEC_FRead(int32_t fd, uint8_t *buf, size_t size)
{
	TEEC_Operation operation;
	TEEC_Result result;
	size_t read_size = 0;
	uint32_t origin;

	if (buf == NULL) {
		tloge("TEEC_FRead failed, read buffer is null\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
		return 0;
	}

	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0)) {
		tloge("TEEC_FRead failed, Please Init Storage Service\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_STATE;
		return 0;
	}

	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_MEMREF_TEMP_OUTPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_NONE);
	operation.params[0].value.a = fd; /*lint !e732 */
	operation.params[0].value.b = TEEC_VALUE_UNDEF;
	operation.params[1].tmpref.buffer = buf;
	operation.params[1].tmpref.size = size;
	operation.params[2].value.a = read_size;
	operation.params[2].value.b = TEEC_VALUE_UNDEF;

	result = TEEC_InvokeCommand(
			 &g_session,
			 STORAGE_CMD_ID_READ,
			 &operation,
			 &origin);

	g_errno = result;
	tlogd("---->ReadFile ret=0x%x\n", g_errno);

	if (result != TEEC_SUCCESS) {
		tloge("---->ReadFile Failed:0x%x, origin:0x%x\n", result, origin);
		return 0;
	}

	read_size = operation.params[2].value.a;
	tlogd("---->ReadFile success,buf=%s,readsize=0x%zx\n", buf, read_size);
	return read_size;
}

size_t TEEC_FWrite(int32_t fd, uint8_t *buf, size_t size)
{
	TEEC_Operation operation;
	TEEC_Result result;
	size_t write_size = 0;
	uint32_t origin;

	if (buf == NULL) {
		tloge("TEEC_FWrite failed, write buffer is null\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_STATE;
		return 0;
	}

	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0)) {
		tloge("TEEC_FWrite failed, Please Init Storage Service\n");
		g_errno = (TEEC_Result)TEEC_ERROR_GENERIC;
		return 0;
	}

	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_NONE);
	operation.params[0].value.a = fd; /*lint !e732 */
	operation.params[0].value.b = TEEC_VALUE_UNDEF;
	operation.params[1].tmpref.buffer = buf;
	operation.params[1].tmpref.size = size;
	operation.params[2].value.a = write_size;
	operation.params[2].value.b = TEEC_VALUE_UNDEF;
	result = TEEC_InvokeCommand(
			 &g_session,
			 STORAGE_CMD_ID_WRITE,
			 &operation,
			 &origin);

	g_errno = result;
	tlogd("---->WriteFile ret=0x%x\n", g_errno);

	if (result != TEEC_SUCCESS) {
		tloge("---->WriteFile Failed:0x%x, origin:0x%x\n", result, origin);
		return 0;
	}

	write_size = operation.params[2].value.a;
	tlogd("---->WriteFile success,writesize=0x%zx\n", write_size);
	return write_size;
}

int32_t TEEC_FSeek(int32_t fd, int32_t offset, int32_t fromwhere)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;

	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0)) {
		tloge("TEEC_FSeek failed, Please Init Storage Service\n");
		g_errno = (TEEC_Result)TEEC_ERROR_GENERIC;
		return 0;
	}

	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_VALUE_INPUT,
				       TEEC_NONE,
				       TEEC_NONE); /*lint !e845 */
	operation.params[0].value.a = (uint32_t)fd;
	operation.params[0].value.b = (uint32_t)offset;
	operation.params[1].value.a = (uint32_t)fromwhere;
	operation.params[1].value.b = TEEC_VALUE_UNDEF;
	result = TEEC_InvokeCommand(
			 &g_session,
			 STORAGE_CMD_ID_SEEK,
			 &operation,
			 &origin);

	g_errno = result;
	tlogd("---->FSeek ret=0x%x\n", g_errno);

	if (result != TEEC_SUCCESS) {
		tloge("---->FSeek Failed:0x%x, origin:0x%x\n", result, origin);
		return 0;
	}

	return result; /*lint !e713 */
}

size_t TEEC_FInfo(int32_t fd, uint32_t *pos, uint32_t *len)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;

	if (NULL == pos || NULL == len) {
		tloge("---->TEEC_FInfo invalid input para\n");
		return 0;
	}
	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0)) {
		tloge("TEEC_FInfo failed, Please Init Storage Service\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_STATE;
		return 0;
	}

	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_VALUE_OUTPUT,
				       TEEC_NONE,
				       TEEC_NONE); /*lint !e845 */
	operation.params[0].value.a = fd; /*lint !e732 */
	operation.params[0].value.b = TEEC_VALUE_UNDEF;

	result = TEEC_InvokeCommand(
			 &g_session,
			 STORAGE_CMD_ID_FINFO,
			 &operation,
			 &origin);

	g_errno = result;
	tlogd("---->TEEC_FInfo ret=0x%x\n", g_errno);

	if (result != TEEC_SUCCESS) {
		tloge("---->TEEC_FInfo Failed:0x%x, origin:0x%x\n", result, origin);
		return 0;
	}

	*pos = operation.params[1].value.a;
	*len = operation.params[1].value.b;
	tlogd("---->TEEC_FInfo success,pos=0x%x,len=0x%x\n", *pos, *len);
	return result;
}

int32_t TEEC_FCloseAndDelete(int32_t fd)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;

	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0)) {
		tloge("TEEC_FCloseAndDelete failed, Please Init Storage Service\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_STATE;
		return -1;
	}

	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE); /*lint !e845 */
	operation.params[0].value.a = fd; /*lint !e732 */
	operation.params[0].value.b = TEEC_VALUE_UNDEF;

	result = TEEC_InvokeCommand(
			 &g_session,
			 STORAGE_CMD_ID_REMOVE,
			 &operation,
			 &origin);

	g_errno = result;

	if (result != TEEC_SUCCESS) {
		tloge("---->TEEC_FCloseAndDelete Failed:0x%x, origin:0x%x\n", result, origin);
		return -1;
	}

	tlogd("---->TEEC_FCloseAndDelete success\n");
	return 0;
}

int32_t TEEC_FSync(int32_t fd)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin;

	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0)) {
		tloge("TEEC_FSync failed, Please Init Storage Service\n");
		g_errno = (TEEC_Result)TEEC_ERROR_BAD_STATE;
		return -1;
	}

	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_VALUE_INPUT,
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_NONE); /*lint !e845 */
	operation.params[0].value.a = fd; /*lint !e732 */
	operation.params[0].value.b = TEEC_VALUE_UNDEF;

	result = TEEC_InvokeCommand(
			 &g_session,
			 STORAGE_CMD_ID_FSYNC,
			 &operation,
			 &origin);

	g_errno = result;

	if (result != TEEC_SUCCESS) {
		tloge("---->TEEC_FSync Failed:0x%x, origin:0x%x\n", result, origin);
		return -1;
	}

	tlogd("---->TEEC_FSync success\n");
	return 0;

}

TEEC_Result TEEC_GetFErr()
{
	return g_errno;
}

