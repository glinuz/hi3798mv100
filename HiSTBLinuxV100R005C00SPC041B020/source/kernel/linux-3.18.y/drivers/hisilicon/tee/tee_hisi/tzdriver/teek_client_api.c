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
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <asm/cacheflush.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/kernel.h>

#include "teek_client_api.h"
#include "teek_client_id.h"
#include "teek_ns_client.h"
#include "tc_ns_log.h"
#include "securec.h"

static TEEC_Result TEEK_Encode(TC_NS_ClientContext *cli_context,
		 TEEC_UUID service_id,
		 uint32_t session_id,
		 uint32_t cmd_id,
		 TC_NS_ClientLogin *cli_login, TEEC_Operation *operation)
{
	uint32_t param_type[4];
	uint32_t param_cnt;
	uint32_t diff;
	errno_t sret;

	if (!cli_context || !cli_login) {
		tloge("cli_context or cli_login is null.\n");
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}
	diff =
		(uint32_t) TEEC_MEMREF_PARTIAL_INPUT -
		(uint32_t) TEEC_MEMREF_TEMP_INPUT;

	sret = memset_s(cli_context, sizeof(TC_NS_ClientContext),
			0x00, sizeof(TC_NS_ClientContext));
	if (EOK != sret) {
		tloge("memset_s error sret is %d.\n", sret);
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}
	sret = memcpy_s(cli_context->uuid, sizeof(cli_context->uuid),
			(uint8_t *)&service_id, 16);
	if (EOK != sret) {
		tloge("memcpy_s error sret is %d.\n", sret);
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}
	cli_context->session_id = session_id;
	cli_context->cmd_id = cmd_id;
	cli_context->returns.code = 0;
	cli_context->returns.origin = 0;

	cli_context->login.method = cli_login->method;
	cli_context->login.mdata = cli_login->mdata;

	/* support when operation is null */
	if (!operation)
		return TEEC_SUCCESS;
	cli_context->started = operation->cancel_flag;
	param_type[0] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 0);
	param_type[1] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 1);
	param_type[2] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 2);
	param_type[3] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 3);

	for (param_cnt = 0; param_cnt < 4; param_cnt++) {
		if ((TEEC_MEMREF_TEMP_INPUT == param_type[param_cnt])
		    || (TEEC_MEMREF_TEMP_OUTPUT == param_type[param_cnt])
		    || (TEEC_MEMREF_TEMP_INOUT == param_type[param_cnt])) {

			cli_context->params[param_cnt].memref.buffer
				=
					(__u64)operation->params[param_cnt].tmpref.buffer;
			cli_context->params[param_cnt].memref.size_addr =
				(__u64)&operation->params[param_cnt].tmpref.size;
		} else if ((TEEC_MEMREF_WHOLE == param_type[param_cnt])
			   || (TEEC_MEMREF_PARTIAL_INPUT == param_type[param_cnt])
			   || (TEEC_MEMREF_PARTIAL_OUTPUT ==
				   param_type[param_cnt])
			   || (TEEC_MEMREF_PARTIAL_INOUT ==
				   param_type[param_cnt])) {

			/* buffer offset len */
			if (TEEC_MEMREF_WHOLE == param_type[param_cnt]) {
				cli_context->params[param_cnt].memref.offset =
					0;
				cli_context->params[param_cnt].memref.
				size_addr =
					(__u64)&operation->params[param_cnt].
					memref.parent->size;
			} else {
				cli_context->params[param_cnt].memref.offset
					=
						operation->params[param_cnt].memref.offset;
				cli_context->params[param_cnt].memref.
				size_addr =
					(__u64)&operation->params[param_cnt].
					memref.size;
			}

			if (operation->params[param_cnt].memref.parent->
			    is_allocated) {
				cli_context->params[param_cnt].memref.buffer =
					(__u64)operation->params[param_cnt].memref.
					parent->buffer;
			} else {
				cli_context->params[param_cnt].memref.buffer
					=
						(__u64)operation->params[param_cnt].memref.
						parent->buffer +
						operation->params[param_cnt].memref.offset;
				cli_context->params[param_cnt].memref.offset =
					0;
			}
			/* DTSDTS2013030104869 h00211444 begin */
			/* translate the paramType to know the driver */
			if (TEEC_MEMREF_WHOLE == param_type[param_cnt]) {
				switch (operation->params[param_cnt].memref.
					parent->flags) {
				case TEEC_MEM_INPUT:
					param_type[param_cnt] =
						TEEC_MEMREF_PARTIAL_INPUT;
					break;
				case TEEC_MEM_OUTPUT:
					param_type[param_cnt] =
						TEEC_MEMREF_PARTIAL_OUTPUT;
					break;
				case TEEC_MEM_INOUT:
					param_type[param_cnt] =
						TEEC_MEMREF_PARTIAL_INOUT;
					break;
				default:
					param_type[param_cnt] =
						TEEC_MEMREF_PARTIAL_INOUT;
					break;
				}
			}
			/* if is not allocated,
			 * translate TEEC_MEMREF_PARTIAL_XXX
			 * to TEEC_MEMREF_TEMP_XXX */
			if (!operation->params[param_cnt].memref.parent->
			    is_allocated)
				param_type[param_cnt] =
					param_type[param_cnt] - diff;
			/* DTSDTS2013030104869 h00211444 end */
		} else if ((TEEC_VALUE_INPUT == param_type[param_cnt])
			   || (TEEC_VALUE_OUTPUT == param_type[param_cnt])
			   || (TEEC_VALUE_INOUT == param_type[param_cnt])) {

			cli_context->params[param_cnt].value.a_addr =
				(__u64 *)&operation->params[param_cnt].value.a;
			cli_context->params[param_cnt].value.b_addr =
				(__u64 *)&operation->params[param_cnt].value.b;
		} else if (TEEC_ION_INPUT == param_type[param_cnt]) {
			cli_context->params[param_cnt].value.a_addr
				= (__u64 *)&operation->params[param_cnt].ionref.ion_share_fd;
			cli_context->params[param_cnt].value.b_addr
				= (__u64 *)&operation->params[param_cnt].ionref.ion_size;
		} else if (TEEC_NONE == param_type[param_cnt]) {
			/* do nothing */
		} else {
			tloge("param_type[%d]=%d not correct\n",
					param_cnt, param_type[param_cnt]);
			return TEEC_ERROR_BAD_PARAMETERS;
		}
	}
	cli_context->paramTypes =
		TEEC_PARAM_TYPES(param_type[0], param_type[1], param_type[2],
				 param_type[3]);
	tlogv("cli param type %d\n", cli_context->paramTypes);
	return TEEC_SUCCESS;
}

static TEEC_Result TEEK_CheckTmpRef(TEEC_TempMemoryReference tmpref)
{
	TEEC_Result ret;

	if ((!tmpref.buffer) || (0 == tmpref.size)) {
		tloge("tmpref buffer is null, or size is zero\n");
		ret = (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
	} else {
		ret = (TEEC_Result) TEEC_SUCCESS;
	}
	return ret;
}

static TEEC_Result TEEK_CheckMemRef(TEEC_RegisteredMemoryReference memref,
				    uint32_t param_type)
{
	if ((!memref.parent) || (!memref.parent->buffer)) {
		tloge("parent of memref is null, or the buffer is zero\n");
		return (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
	}

	if (TEEC_MEMREF_PARTIAL_INPUT == param_type) {
		if (!(memref.parent->flags & TEEC_MEM_INPUT))
			return (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
	} else if (TEEC_MEMREF_PARTIAL_OUTPUT == param_type) {
		if (!(memref.parent->flags & TEEC_MEM_OUTPUT))
			return (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
	} else if (TEEC_MEMREF_PARTIAL_INOUT == param_type) {
		/* DTS2013030509162 h00211444 begin */
		if (!(memref.parent->flags & TEEC_MEM_INPUT))
			return (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
		if (!(memref.parent->flags & TEEC_MEM_OUTPUT))
			return (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
		/* DTS2013030509162 h00211444 end */
	} else if (TEEC_MEMREF_WHOLE == param_type) {
		/* if type is TEEC_MEMREF_WHOLE, ignore it*/
	} else {
			return (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
	}

	if ((param_type == TEEC_MEMREF_PARTIAL_INPUT)
	    || (param_type == TEEC_MEMREF_PARTIAL_OUTPUT)
	    || (param_type == TEEC_MEMREF_PARTIAL_INOUT)) {
		if ((memref.offset + memref.size) > memref.parent->size
				|| (memref.offset + memref.size) < memref.offset
				|| (memref.offset + memref.size) < memref.size) {
			tloge("offset + size exceed the parent size\n");
			return (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
		}
	}

	return (TEEC_Result) TEEC_SUCCESS;
}

/*
 * Function:       TEEC_CheckOperation
 * Description:   This function checks a operation is valid or not.
 * Parameters:   operation: a pointer to an Operation to be checked.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEK_CheckOperation(TEEC_Operation *operation)
{
	uint32_t param_type[4];
	uint32_t param_cnt;
	TEEC_Result ret = TEEC_SUCCESS;

	/* GP Support operation is NULL
	 * operation: a pointer to a Client Application initialized TEEC_Operation structure,
	 * or NULL if there is no payload to send or if the Command does not need to support
	 * cancellation.
	 * */
	if (!operation)
		return (TEEC_Result)TEEC_SUCCESS;

	if (!operation->started) {
		tloge("sorry, cancellation not support\n");
		return (TEEC_Result) TEEC_ERROR_NOT_IMPLEMENTED;
	}

	param_type[0] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 0);
	param_type[1] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 1);
	param_type[2] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 2);
	param_type[3] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 3);

	for (param_cnt = 0; param_cnt < 4; param_cnt++) {
		if ((TEEC_MEMREF_TEMP_INPUT == param_type[param_cnt])
		    || (TEEC_MEMREF_TEMP_OUTPUT == param_type[param_cnt])
		    || (TEEC_MEMREF_TEMP_INOUT == param_type[param_cnt])) {

			ret = TEEK_CheckTmpRef(operation->params
					       [param_cnt].tmpref);
			if (ret != TEEC_SUCCESS)
				break;
		} else if ((TEEC_MEMREF_WHOLE == param_type[param_cnt])
			   || (TEEC_MEMREF_PARTIAL_INPUT ==
				   param_type[param_cnt])
			   || (TEEC_MEMREF_PARTIAL_OUTPUT ==
				   param_type[param_cnt])
			   || (TEEC_MEMREF_PARTIAL_INOUT ==
				   param_type[param_cnt])) {

			ret = TEEK_CheckMemRef(operation->params
					       [param_cnt].memref,
					       param_type[param_cnt]);
			if (ret != TEEC_SUCCESS)
				break;
		} else if ((TEEC_VALUE_INPUT == param_type[param_cnt])
			   || (TEEC_VALUE_OUTPUT == param_type[param_cnt])
			   || (TEEC_VALUE_INOUT == param_type[param_cnt])) {
			/* if type is value, ignore it*/
		} else if (TEEC_NONE == param_type[param_cnt]) {
			/* if type is none, ignore it*/
		} else if (TEEC_ION_INPUT == param_type[param_cnt]) {
			if (operation->params[param_cnt].ionref.ion_share_fd < 0) {
				tloge("operation check failed: ion_handle is invalid!\n");
				ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
				break;
			}
		} else {
			tloge("paramType[%d]=%x is not support\n",
				   param_cnt, param_type[param_cnt]);
			ret = (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
			break;
		}
	}

	return ret;
}

/*
 * Function:      TEEK_IsAgentAlive
 * Description:   This function check if the special agent
 *				  is launched.Used For HDCP key.
 *                e.g. If sfs agent is not alive,
 *                you can not do HDCP key write to SRAM.
 * Parameters:   agent_id.
 * Return:      1:agent is alive
 *              0:agent not exsit.
 */
int TEEK_IsAgentAlive(unsigned int agent_id)
{
	return is_agent_alive(agent_id);
}
EXPORT_SYMBOL(TEEK_IsAgentAlive);

/*
 * Function:       TEEC_InitializeContext
 * Description:   This function initializes a new TEE Context,
 *				forming a connection between this
 *				Client Application
 *				and the TEE identified by
 *				the string identifier name.
 * Parameters:   name: a zero-terminated string that describes
 *						the TEE to connect to.
 *                     If this parameter is set to NULL,
 *                     the Implementation MUST select a default TEE.
 *                     context: a TEEC_Context structure that
 *					be initialized by the Implementation.
 * Return:         TEEC_SUCCESS: the initialization was successful.
 *                     other: initialization was not successful.
 */
TEEC_Result TEEK_InitializeContext(const char *name, TEEC_Context *context)
{
	int32_t ret;

	/* name current not used */
	(void)(name);

	tlogd("TEEK_InitializeContext Started:\n");

	/* First, check parameters is valid or not */
	if (!context) {
		tloge("context is null, not correct\n");
		return (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
	}

	context->dev = NULL;
	/* Paramters right, start execution */

	ret = TC_NS_ClientOpen((TC_NS_DEV_File **)&context->dev,
			       TEE_REQ_FROM_KERNEL_MODE);

	if (TEEC_SUCCESS != ret) {
		tloge("open device failed\n");
		return (TEEC_Result) TEEC_ERROR_GENERIC;
	}

	tlogd("open device success\n");
	INIT_LIST_HEAD((struct list_head *)&context->session_list);
	INIT_LIST_HEAD((struct list_head *)&context->shrd_mem_list);
	return TEEC_SUCCESS;
}
EXPORT_SYMBOL(TEEK_InitializeContext);

/*
 * Function:       TEEC_FinalizeContext
 * Description:   This function finalizes an initialized TEE Context.
 * Parameters:   context: an initialized TEEC_Context structure
 *					which is to be finalized.
 * Return:         NULL
 */
void TEEK_FinalizeContext(TEEC_Context *context)
{
	struct list_node *ptr;
	TEEC_Session *session;
	/*TEEC_SharedMemory* shrdmem;*/

	tlogd("TEEK_FinalizeContext started\n");

	/* First, check parameters is valid or not */
	if (!context) {
		tloge("context is null, not correct\n");
		return;
	}

	/* Paramters right, start execution */
	if (!LIST_EMPTY(&context->session_list)) {
		tlogi("context still has sessions opened, close it\n");
		list_for_each(ptr, &context->session_list) {
			session = list_entry(ptr, TEEC_Session, head);
			TEEK_CloseSession(session);
		}
	}

	tlogd("close device\n");
	TC_NS_ClientClose(context->dev, 0);
}
EXPORT_SYMBOL(TEEK_FinalizeContext);

/*
 * Function:       TEEC_OpenSession
 * Description:   This function opens a new Session
 * Parameters:   context: a pointer to an initialized TEE Context.
 *                 session: a pointer to a Session structure to open.
 *                 destination: a pointer to a UUID structure.
 *                 connectionMethod: the method of connection to use.
 *                 connectionData: any necessary data required to
 *					support the connection method chosen.
 *                 operation: a pointer to an Operation containing
 *					a set of Parameters.
 *                 returnOrigin: a pointer to a variable which will
 *						contain the return origin.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEK_OpenSession(TEEC_Context *context,
			     TEEC_Session *session,
			     const TEEC_UUID *destination,
			     uint32_t connectionMethod,
			     const void *connectionData,
			     TEEC_Operation *operation,
			     uint32_t *returnOrigin)
{
	int32_t ret;
	TEEC_Result teec_ret = (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
	uint32_t origin = TEEC_ORIGIN_API;
	TC_NS_ClientContext cli_context;
	TC_NS_ClientLogin cli_login = { 0, 0 };
	TC_NS_DEV_File *dev_file;
	uint32_t param_type[4] = { 0 };
	errno_t sret;

	tlogd("TEEK_OpenSession Started:\n");
	/* connectionData current not used */
	(void)(connectionData);
	/* returnOrigin maybe null, so only when it is valid, we set
	 * origin(error come from which module)
	 * */
	if (returnOrigin)
		*returnOrigin = origin;

	/* First, check parameters is valid or not */
	if (!context || !session || !destination || !operation
			|| TEEC_LOGIN_IDENTIFY != connectionMethod) {
		tloge("invalid input params\n");
		return teec_ret;
	}
	param_type[3] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 3);
	param_type[2] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 2);
	if (TEEC_MEMREF_TEMP_INPUT != param_type[3]
			|| TEEC_MEMREF_TEMP_INPUT != param_type[2]) {
		tloge("invalid param type 0x%x\n", operation->paramTypes);
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}
	if (NULL == operation->params[3].tmpref.buffer
			|| NULL == operation->params[2].tmpref.buffer
			|| 0 == operation->params[3].tmpref.size
			|| 0 == operation->params[2].tmpref.size) {
		tloge("invalid operation params(NULL)\n");
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}
	cli_login.method = TEEC_LOGIN_IDENTIFY;
	dev_file = (TC_NS_DEV_File *)(context->dev);
	if (!dev_file) {
		tloge("invalid context->dev (NULL)\n");
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}
	dev_file->pkg_name_len = operation->params[3].tmpref.size;
	if (operation->params[3].tmpref.size > (MAX_PACKAGE_NAME_LEN - 1)) {
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	} else {
		sret = memset_s(dev_file->pkg_name, sizeof(dev_file->pkg_name),
				0, MAX_PACKAGE_NAME_LEN);
		if (EOK != sret) {
			tloge("memset_s error sret is %d.\n", sret);
			return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
		}
		sret = memcpy_s(dev_file->pkg_name, sizeof(dev_file->pkg_name),
				operation->params[3].tmpref.buffer,
				operation->params[3].tmpref.size);
		if (EOK != sret) {
			tloge("memcpy_s error sret is %d.\n", sret);
			return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
		}
	}
	dev_file->pub_key_len = 0;
	dev_file->login_setup = 1;

	teec_ret = TEEK_CheckOperation(operation);
	if (TEEC_SUCCESS != teec_ret) {
		tloge("operation is invalid\n");
		return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
	}

	/* Paramters right, start execution */
	/*
	 * note:before open session success,
	 * we should send session=0 as initial state.
	 */
	teec_ret = TEEK_Encode(&cli_context, *destination, 0 /*session->session_id */,
		    GLOBAL_CMD_ID_OPEN_SESSION, &cli_login, operation);
	if (TEEC_SUCCESS != teec_ret) {
		tloge("encode failed\n");
		return teec_ret;
	}

	ret = TC_NS_OpenSession(context->dev, &cli_context);
	if (0 == ret) {
		tlogd("open session success\n");
		session->session_id = cli_context.session_id;
		session->service_id = *destination;
		session->ops_cnt = 0;
		INIT_LIST_HEAD((struct list_head *)&session->head);
		list_insert_tail(&context->session_list, &session->head);
		session->context = context;
		teec_ret = TEEC_SUCCESS;
	} else if (ret < 0) {
		tloge("open session failed, ioctl errno = %d\n", ret);
		if (-EFAULT == ret)
			teec_ret = (TEEC_Result) TEEC_ERROR_ACCESS_DENIED;
		else if (-ENOMEM == ret)
			teec_ret = (TEEC_Result) TEEC_ERROR_OUT_OF_MEMORY;
		else if (-EINVAL == ret)
			teec_ret = (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
		else
			teec_ret = (TEEC_Result) TEEC_ERROR_GENERIC;
		origin = TEEC_ORIGIN_COMMS;
	} else {
		tloge("open session failed, code=0x%x, origin=%d\n",
			   cli_context.returns.code,
			   cli_context.returns.origin);
		teec_ret = (TEEC_Result) cli_context.returns.code;
		origin = cli_context.returns.origin;
	}

	/* ONLY when ioctl returnCode!=0 and returnOrigin not NULL,
	 * set *returnOrigin
	 */
	if ((TEEC_SUCCESS != teec_ret) && (returnOrigin))
		*returnOrigin = origin;
	return teec_ret;
}
EXPORT_SYMBOL(TEEK_OpenSession);

/*
 * Function:       TEEC_CloseSession
 * Description:   This function closes an opened Session.
 * Parameters:   session: the session to close.
 * Return:         NULL
 */
void TEEK_CloseSession(TEEC_Session *session)
{
	int32_t ret;
	TC_NS_ClientContext cli_context;
	TC_NS_ClientLogin cli_login = { 0, 0 };
	struct list_node *ptr;
	TEEC_Session *temp_sess;
	bool found = false;
	errno_t sret;

	tlogd("TEEK_CloseSession started\n");

	/* First, check parameters is valid or not */
	if (!session || !session->context) {
		tloge("input invalid session or session->context is null\n");
		return;
	}

	list_for_each(ptr, &session->context->session_list) {
		temp_sess = list_entry(ptr, TEEC_Session, head);
		if (temp_sess == session) {
			found = true;
			break;
		}
	}
	if (!found) {
		tloge("session isnot in the context list\n");
		return;
	}

	/* Paramters all right, start execution */
	if (session->ops_cnt)
		tloge("session still has commands running\n");

	if (TEEC_SUCCESS != TEEK_Encode(&cli_context, session->service_id, session->session_id,
		    GLOBAL_CMD_ID_CLOSE_SESSION, &cli_login, NULL)) {
		tloge("encode failed, just return\n");
		return;
	}

	ret = TC_NS_CloseSession(session->context->dev, &cli_context);

	if (0 == ret) {
		tlogd("close session success\n");
		session->session_id = 0;
		sret = memset_s((uint8_t *)(&session->service_id), sizeof(session->service_id),
				0x00, 16);
		if (EOK != sret) {
			tloge("memset_s error sret is %d.\n", sret);
			/* TEEK_CloseSession is void so go on execute */
		}
		session->ops_cnt = 0;
		list_remove(&session->head);
		session->context = NULL;
	} else {
		tloge("close session failed\n");
	}
}
EXPORT_SYMBOL(TEEK_CloseSession);

/*
 * Function:       TEEC_InvokeCommand
 * Description:   This function invokes a Command within the specified Session.
 * Parameters:   session: the open Session in which the command will be invoked.
 *               commandID: the identifier of the Command.
 *               operation: a pointer to an Operation containing
 *					a set of Parameters.
 *               returnOrigin: a pointer to a variable which will
 *					   contain the return origin.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEK_InvokeCommand(TEEC_Session *session,
			       uint32_t commandID,
			       TEEC_Operation *operation,
			       uint32_t *returnOrigin)
{
	int32_t ret;
	TEEC_Result teec_ret = (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
	uint32_t origin = TEEC_ORIGIN_API;
	TC_NS_ClientContext cli_context;
	TC_NS_ClientLogin cli_login = { 0, 0 };

	tlogd("TEEK_InvokeCommand Started:\n");

	/* First, check parameters is valid or not */
	if (!session || !session->context) {
		tloge("input invalid session or session->context is null\n");
		if (returnOrigin)
			*returnOrigin = origin;
		return teec_ret;
	}

	teec_ret = TEEK_CheckOperation(operation);
	if (TEEC_SUCCESS != teec_ret) {
		tloge("operation is invalid\n");
		if (returnOrigin)
			*returnOrigin = origin;
		return teec_ret;
	}

	/* Paramters all right, start execution */
	session->ops_cnt++;
	teec_ret = TEEK_Encode(&cli_context, session->service_id, session->session_id,
		    commandID, &cli_login, operation);
	if (TEEC_SUCCESS != teec_ret) {
		tloge("encode failed\n");
		session->ops_cnt--;
		if (returnOrigin)
			*returnOrigin = origin;
		return teec_ret;
	}

	ret = TC_NS_Send_CMD(session->context->dev, &cli_context);
	if (0 == ret) {
		tlogd("invoke cmd success\n");
		teec_ret = TEEC_SUCCESS;
	} else if (ret < 0) {
		tloge("invoke cmd failed, ioctl errno = %d\n", ret);
		if (-EFAULT == ret)
			teec_ret = (TEEC_Result) TEEC_ERROR_ACCESS_DENIED;
		else if (-ENOMEM == ret)
			teec_ret = (TEEC_Result) TEEC_ERROR_OUT_OF_MEMORY;
		else if (-EINVAL == ret)
			teec_ret = (TEEC_Result) TEEC_ERROR_BAD_PARAMETERS;
		else
			teec_ret = (TEEC_Result) TEEC_ERROR_GENERIC;
		origin = TEEC_ORIGIN_COMMS;
	} else {
		tloge("invoke cmd failed, code=0x%x, origin=%d\n",
			   cli_context.returns.code,
			   cli_context.returns.origin);
		teec_ret = (TEEC_Result) cli_context.returns.code;
		origin = cli_context.returns.origin;
	}
	session->ops_cnt--;

	/* ONLY when ioctl returnCode!=0 and returnOrigin not NULL,
	 * set *returnOrigin
	 */
	if ((TEEC_SUCCESS != teec_ret) && (returnOrigin))
		*returnOrigin = origin;
	return teec_ret;
}
EXPORT_SYMBOL(TEEK_InvokeCommand);

/*
 * Function:      TEEC_RegisterSharedMemory
 * Description:   This function registers a block of existing
 *				  Client Application memory
 *                as a block of Shared Memory within the scope of
 *                the specified TEE Context.
 * Parameters:   context: a pointer to an initialized TEE Context.
 *               sharedMem: a pointer to a Shared Memory structure to register.
 * Return:       TEEC_SUCCESS: success
 *               other: failure
 */
TEEC_Result TEEK_RegisterSharedMemory(TEEC_Context *context,
				      TEEC_SharedMemory *sharedMem)
{
	tloge("TEEK_RegisterSharedMemory not supported\n");
	return TEEC_ERROR_NOT_SUPPORTED;
}
EXPORT_SYMBOL(TEEK_RegisterSharedMemory);

/*
 * Function:       TEEC_AllocateSharedMemory
 * Description:   This function allocates a new block of memory as a block of
 *                Shared Memory within the scope of the specified TEE Context.
 * Parameters:   context: a pointer to an initialized TEE Context.
 *               sharedMem: a pointer to a Shared Memory structure to allocate.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEK_AllocateSharedMemory(TEEC_Context *context,
				      TEEC_SharedMemory *sharedMem)
{
	tloge("TEEK_AllocateSharedMemory not supported\n");
	return TEEC_ERROR_NOT_SUPPORTED;
}
EXPORT_SYMBOL(TEEK_AllocateSharedMemory);

/*
 * Function:       TEEC_ReleaseSharedMemory
 * Description:   This function deregisters or deallocates
 * a previously initialized block of Shared Memory..
 * Parameters:   sharedMem: a pointer to a valid Shared Memory structure.
 * Return:         NULL
 */
void TEEK_ReleaseSharedMemory(TEEC_SharedMemory *sharedMem)
{
	tloge("TEEK_ReleaseSharedMemory not supported\n");
}
EXPORT_SYMBOL(TEEK_ReleaseSharedMemory);


/*
 * Function:       TEEC_RequestCancellation
 * Description:   This function requests the cancellation of
 * a pending open Session operation or
 * a Command invocation operation.
 * Parameters:   operation:a pointer to a Client Application
 * instantiated Operation structure
 * Return:         NULL
 */
void TEEK_RequestCancellation(TEEC_Operation *operation)
{
	tloge("TEEK_RequestCancellation not supported\n");
}
