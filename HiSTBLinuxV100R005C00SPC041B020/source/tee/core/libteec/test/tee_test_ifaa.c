/*
 * tee_test_ifaa.c
 *
 * IFAA ta ut test realize.
 *
 * Copyright (c) 1998-2016 Huawei Technologies Co., Ltd. All rights
 * reserved.
 *
 * Xiangyu Lu <luxiangyu@huawei.com>, Mar, 2016.
 *
 * Modification history:
 *
 * 05 Jul 2016 Xiangyu Lu
 *      Create file.
 */

#include "tee_test_ifaa.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee_client_id.h"

#define IFAA_TA_PATH  "/system/bin/993e26b8-0273-408e-98d3-60c997c37121.sec"
#define SIGNTOOL_TA_PATH  "/system/bin/9b17660b-8968-4eed-917e-dd32379bd548.sec"
#define CMD_IFAA_Invoke 0x00000001
#define RSP_BUF_MAX_LEN 2048

bool is_little_endian(void)
{
	uint16_t data = 1;
	return *(uint8_t *)(&data);
}

uint32_t read32(const uint8_t *buffer)
{
	if (is_little_endian()) {
		return buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 |
		       buffer[0];
	}
	return buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 |
	       buffer[3];
}

static const TEEC_UUID ifaa_uuid = {
	0x993e26b8, 0x0273, 0x408e,
	{0x98, 0xd3, 0x60, 0xc9, 0x97, 0xc3, 0x71, 0x21}
};

static const TEEC_UUID signtool_uuid = {
	0x9b17660b, 0x8968, 0x4eed,
	{0x91, 0x7e, 0xdd, 0x32, 0x37, 0x9b, 0xd5, 0x48}
};

/* IFAA invoke command. */
typedef enum {
	/* IFAA common business command. */
	IFAA_TA_CMD_GET_DEVICE_ID					= 1 << 0,
	IFAA_TA_CMD_REGISTER						= 1 << 1,
	IFAA_TA_CMD_AUTHENTICATE					= 1 << 2,
	IFAA_TA_CMD_DEREGISTER						= 1 << 3,
	IFAA_TA_CMD_QUERY_STATUS					= 1 << 4,
	IFAA_TA_CMD_GEN_ASYMMETRIC_KEY				= 1 << 5,
	IFAA_TA_CMD_GET_PROTOCOL_VERSION			= 1 << 6,
	IFAA_TA_CMD_GET_CERT_ALG_ENCODE				= 1 << 7,
	/* IFAA test mode command. */
	IFAA_TA_CMD_READ_FILE						= 129,
	IFAA_TA_CMD_WRITE_FILE						= 130,
	IFAA_TA_CMD_DELETE_FILE						= 131,
	IFAA_INTERNAL_GET_LAST_IDENTIFIED_RESULT	= 132,
	IFAA_INTERNAL_GET_AUTHENTICATOR_VERSION		= 133,
	IFAA_INTERNAL_GET_ID_LIST					= 134,
	IFAA_INTERNAL_BIO_ID_COMPARE				= 135,
	IFAA_INTERNAL_SHA256						= 136,
	IFAA_INTERNAL_SIGN							= 137, /* performance */
	IFAA_INTERNAL_VERIFY						= 138, /* performance */
	IFAA_INTERNAL_KEY_GENERATE					= 139, /* performance */
	IFAA_INTERNAL_HMAC_SHA1						= 140,
	IFAA_INTERNAL_AUTHENTICATOR_SIGN			= 141, /* performance */
	IFAA_INTERNAL_AUTHENTICATOR_VERIFY			= 142, /* performance */
	/* TA interface fault injection test command. */
	TA_CMD_WITH_INOUT_BUFFER_NULL				= 300,
	TA_CMD_WITH_OUT_BUFFER_SMALL				= 301,
	TA_CMD_WITH_OUT_BUFFER_SIZE_SMALL			= 302,
	TA_CMD_WITH_IN_BUFFER_SMALL					= 303,
	TA_CMD_WITH_IN_BUFFER_LARGE					= 304,
	TA_CMD_WITH_IN_BUFFER_HUGE					= 305,
	/* Fault command buffer inject test. */
	TA_CMD_WITH_BAD_DATA_VERSION				= 400,
	TA_CMD_WITH_BAD_DATA_SIGN_SIZE				= 401,
	TA_CMD_WITH_BAD_DATA_SIGNATUREN				= 402,
	TA_CMD_WITH_BAD_DATA_PAK_SIZE				= 403,
	TA_CMD_WITH_BAD_DATA_PAK					= 404,
	TA_CMD_WITH_BAD_DATA_COMMAND				= 405,
	TA_CMD_WITH_BAD_DATA_PARAM_SIZE				= 406,
	TA_CMD_WITH_BAD_DATA_PARAM					= 407,
	TA_CMD_WITH_WRONG_ORDER_TLV_NODE			= 500,
	TA_CMD_WITH_WRONG_STRUCT_TLV_NODE			= 501,
	TA_CMD_LOAD_SIGNTOOL_TA						= 900,
} ifaa_ta_cmd;

typedef struct {
	int cmd_id;
	uint8_t *cmd_data;
	uint32_t cmd_len;
} IFAA_cmd_entry;

static IFAA_cmd_entry cmd_entrys[] = {
	/* IFAA common business command. */
	{ IFAA_TA_CMD_GET_DEVICE_ID, ifaa_get_device_cmd, sizeof(ifaa_get_device_cmd) },
	{ IFAA_TA_CMD_REGISTER, ifaa_register_cmd, sizeof(ifaa_register_cmd) },
	{ IFAA_TA_CMD_AUTHENTICATE, ifaa_authenticate_cmd, sizeof(ifaa_authenticate_cmd) },
	{ IFAA_TA_CMD_DEREGISTER, ifaa_deregister_cmd, sizeof(ifaa_deregister_cmd) },
	{ IFAA_TA_CMD_QUERY_STATUS, ifaa_query_status_cmd, sizeof(ifaa_query_status_cmd) },
	{ IFAA_TA_CMD_GEN_ASYMMETRIC_KEY, ifaa_get_asymmetric_key_cmd, sizeof(ifaa_get_asymmetric_key_cmd) },
	{ IFAA_TA_CMD_GET_PROTOCOL_VERSION, ifaa_get_protocol_version_cmd, sizeof(ifaa_get_protocol_version_cmd) },
	{ IFAA_TA_CMD_GET_CERT_ALG_ENCODE, ifaa_get_cert_alg_encode_cmd, sizeof(ifaa_get_cert_alg_encode_cmd) },
	/* IFAA test mode command. */
	/* TA interface fault injection test command. */
	{ TA_CMD_WITH_INOUT_BUFFER_NULL, ifaa_get_device_cmd, sizeof(ifaa_get_device_cmd) },
	{ TA_CMD_WITH_OUT_BUFFER_SMALL, ifaa_get_device_cmd, sizeof(ifaa_get_device_cmd) },
	{ TA_CMD_WITH_OUT_BUFFER_SIZE_SMALL, ifaa_get_device_cmd, sizeof(ifaa_get_device_cmd) },
	{ TA_CMD_WITH_IN_BUFFER_SMALL, ifaa_get_device_cmd, sizeof(ifaa_get_device_cmd) },
	{ TA_CMD_WITH_IN_BUFFER_LARGE, ifaa_get_device_cmd, sizeof(ifaa_get_device_cmd) },
	{ TA_CMD_WITH_IN_BUFFER_HUGE, ifaa_get_device_cmd, sizeof(ifaa_get_device_cmd) },
	/* Fault command buffer inject test. */
	{ TA_CMD_WITH_BAD_DATA_VERSION, ta_cmd_with_bad_data_version, sizeof(ta_cmd_with_bad_data_version) },
	{ TA_CMD_WITH_BAD_DATA_SIGN_SIZE, ta_cmd_with_bad_data_sign_size, sizeof(ta_cmd_with_bad_data_sign_size) },
	{ TA_CMD_WITH_BAD_DATA_SIGNATUREN, ta_cmd_with_bad_data_sign, sizeof(ta_cmd_with_bad_data_sign) },
	{ TA_CMD_WITH_BAD_DATA_PAK_SIZE, ta_cmd_with_bad_data_pak_size, sizeof(ta_cmd_with_bad_data_pak_size) },
	{ TA_CMD_WITH_BAD_DATA_PAK, ta_cmd_with_bad_data_pak, sizeof(ta_cmd_with_bad_data_pak) },
	{ TA_CMD_WITH_BAD_DATA_COMMAND, ta_cmd_with_bad_data_command, sizeof(ta_cmd_with_bad_data_command) },
	{ TA_CMD_WITH_BAD_DATA_PARAM_SIZE, ta_cmd_with_bad_data_param_size,	sizeof(ta_cmd_with_bad_data_param_size) },
	{ TA_CMD_WITH_BAD_DATA_PARAM, ta_cmd_with_bad_data_param, sizeof(ta_cmd_with_bad_data_param) },
	{ TA_CMD_WITH_WRONG_ORDER_TLV_NODE, ta_cmd_with_wrong_order_tlv, sizeof(ta_cmd_with_wrong_order_tlv) },
	{ TA_CMD_WITH_WRONG_STRUCT_TLV_NODE, ta_cmd_with_wrong_struct_tlv, sizeof(ta_cmd_with_wrong_struct_tlv) },
	{ TA_CMD_LOAD_SIGNTOOL_TA, ifaa_get_device_cmd, sizeof(ifaa_get_device_cmd) },
};

static uint8_t *get_cmd(int cmd_id)
{
	uint32_t i = 0;
	for (; i < sizeof(cmd_entrys) / sizeof(IFAA_cmd_entry); ++i) {
		if (cmd_entrys[i].cmd_id == cmd_id) {
			return cmd_entrys[i].cmd_data;
		}
	}

	return NULL;
}

static uint32_t get_cmd_len(int cmd_id)
{
	uint32_t i = 0;
	for (; i < sizeof(cmd_entrys) / sizeof(IFAA_cmd_entry); ++i) {
		if (cmd_entrys[i].cmd_id == cmd_id) {
			return cmd_entrys[i].cmd_len;
		}
	}

	return 0;
}

int test_ifaa_invoke(TEEC_Session *session,
		     uint8_t *req_buf, uint32_t req_len,
		     uint8_t *rsp_buf, uint32_t *rsp_len)
{
	TEEC_Debug("------%s------\n", __FUNCTION__);
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;
	int ret = 0;
	uint32_t cmd = CMD_IFAA_Invoke;
	uint32_t rspcode = read32(rsp_buf);

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
						TEEC_NONE,
						TEEC_NONE,
						TEEC_MEMREF_TEMP_OUTPUT);
	operation.params[0].tmpref.buffer = req_buf;
	operation.params[0].tmpref.size = req_len;
	operation.params[3].tmpref.buffer = rsp_buf;
	operation.params[3].tmpref.size = *rsp_len;

	result = TEEC_InvokeCommand(session, cmd, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("Invoke failed, codes:0x%x, origin:0x%x.\n", result, origin);
		printf("Invoke failed, codes:0x%x, origin:0x%x.\n", result, origin);
		ret = -1;
	}

	if (rsp_buf != NULL && *rsp_len > 4) {
		rspcode = read32(rsp_buf);
		if (rspcode != 0) {
			printf("Invoke cmd failed, RSP code:%d\n", rspcode);
			return -1;
		}

		printf("Invoke cmd success, RSP code:%d\n", rspcode);
	}

	*rsp_len = operation.params[3].tmpref.size;
	printf("Invoke success, size=%d.\n", *rsp_len);

	return ret;
}

void load_signtoolTA()
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;

	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("TEEC initial failed.\n");
		return;
	}

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_INPUT);

	context.ta_path = (uint8_t *)SIGNTOOL_TA_PATH;

	printf("Start to load SIGNTOOL TA!\n");
	result = TEEC_OpenSession(&context, &session, &signtool_uuid,
				  TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("TEEC open session failed.\n");
		printf("TEEC open session failed.\n");
		TEEC_FinalizeContext(&context);
		return;
	}

	printf("Session id 0x%x\n", session.session_id);
	printf("Succeed to load and excute SIGNTOOL TA!\n");
}

static void usage(void)
{
	printf("You can use tee_test_ifaa #param to test IFAA service:e.g. tee_test_ifaa 1\n");
	printf("param1 is test command as belows:\n");
	printf("      1:IFAA_TA_CMD_GET_DEVICE_ID\n");
	printf("      2:IFAA_TA_CMD_REGISTER\n");
	printf("      4:IFAA_TA_CMD_AUTHENTICATE\n");
	printf("      8:IFAA_TA_CMD_DEREGISTER\n");
	printf("     16:IFAA_TA_CMD_QUERY_STATUS\n");
	printf("     32:IFAA_TA_CMD_GEN_ASYMMETRIC_KEY\n");
	printf("     64:IFAA_TA_CMD_GET_PROTOCOL_VERSION\n");
	printf("    128:IFAA_TA_CMD_GET_CERT_ALG_ENCODE\n");
	printf("    300:TA_CMD_WITH_INOUT_BUFFER_NULL\n");
	printf("    301:TA_CMD_WITH_OUT_BUFFER_SMALL\n");
	printf("    302:TA_CMD_WITH_OUT_BUFFER_SIZE_SMALL\n");
	printf("    303:TA_CMD_WITH_IN_BUFFER_SMALL\n");
	printf("    304:TA_CMD_WITH_IN_BUFFER_LARGE\n");
	printf("    305:TA_CMD_WITH_IN_BUFFER_HUGE\n");
	printf("    400:TA_CMD_WITH_BAD_DATA_VERSION\n");
	printf("    401:TA_CMD_WITH_BAD_DATA_SIGN_SIZE\n");
	printf("    402:TA_CMD_WITH_BAD_DATA_SIGNATUREN\n");
	printf("    403:TA_CMD_WITH_BAD_DATA_PAK_SIZE\n");
	printf("    404:TA_CMD_WITH_BAD_DATA_PAK\n");
	printf("    405:TA_CMD_WITH_BAD_DATA_COMMAND\n");
	printf("    406:TA_CMD_WITH_BAD_DATA_PARAM_SIZE\n");
	printf("    407:TA_CMD_WITH_BAD_DATA_PARAM\n");
	printf("    500:TA_CMD_WITH_WRONG_ORDER_TLV_NODE\n");
	printf("    501:TA_CMD_WITH_WRONG_STRUCT_TLV_NODE\n");
	printf("    900:TA_CMD_LOAD_SIGNTOOL_TA\n");
}

int main(int argc, char **argv)
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result;
	TEEC_Operation operation;
	uint8_t rsp_buf[RSP_BUF_MAX_LEN] = {1};
	uint32_t rsp_len = RSP_BUF_MAX_LEN;
	uint32_t cmd_id = 0;
	int ret = 0;
	uint32_t origin;

	if (argc != 2) {
		printf("Input error.\n");
		usage();
		return -1;
	}

	cmd_id = atoi(argv[1]);
	if (cmd_id == 500) {
		load_signtoolTA();
		return 0;
	}

	uint8_t *ifaa_cmd_data = NULL;
	ifaa_cmd_data = get_cmd(cmd_id);
	if (ifaa_cmd_data == NULL) {
		printf("Invalid cmd id!!\n");
		return -1;
	}
	printf("cmd len is:%d.\n", get_cmd_len(cmd_id));

	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("TEEC initial failed.\n");
		printf("TEEC initial failed.\n");
		return 0;
	}

	memset(&operation, 0x00, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
				       TEEC_NONE,
				       TEEC_NONE,
				       TEEC_MEMREF_TEMP_INPUT,
				       TEEC_MEMREF_TEMP_INPUT);

	context.ta_path = (uint8_t *)IFAA_TA_PATH;

	printf("Start to open IFAA TA!\n");
	result = TEEC_OpenSession(&context, &session, &ifaa_uuid,
				  TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("TEEC open session failed.\n");
		goto cleanup;
	}
	printf("Succeed to open IFAA TA!\n");

	switch (cmd_id) {
	case TA_CMD_LOAD_SIGNTOOL_TA: {
		//load_signtoolTA();
		return 0;
	}
	break;
	case TA_CMD_WITH_INOUT_BUFFER_NULL: {
		ret = test_ifaa_invoke(&session, NULL, get_cmd_len(cmd_id),
				       NULL, &rsp_len);
		printf("Test TA_CMD_WITH_INOUT_BUFFER_NULL return:%d.\n", ret);
	}
	break;
	case TA_CMD_WITH_OUT_BUFFER_SMALL: {
		/* Rsp buf should be 40 bytes. */
		uint8_t rsp_small_buf[4] = {1};
		ret = test_ifaa_invoke(&session, ifaa_cmd_data,
				       get_cmd_len(cmd_id), rsp_small_buf,
				       &rsp_len);
		printf("Test TA_CMD_WITH_OUT_BUFFER_SMALL return:%d.\n", ret);
	}
	break;
	case TA_CMD_WITH_OUT_BUFFER_SIZE_SMALL: {
		/* Rsp buf should be 40 bytes. */
		uint32_t rsp_small_len = 4;
		ret = test_ifaa_invoke(&session, ifaa_cmd_data,
				       get_cmd_len(cmd_id), rsp_buf,
				       &rsp_small_len);
		printf("Test TA_CMD_WITH_OUT_BUFFER_SIZE_SMALL return:%d.\n", ret);
	}
	break;
	case TA_CMD_WITH_IN_BUFFER_SMALL: {
		/* Req buf smaller than size. */
		ret = test_ifaa_invoke(&session, ifaa_cmd_data,
				       get_cmd_len(cmd_id) + 10, rsp_buf,
				       &rsp_len);
		printf("Test TA_CMD_WITH_IN_BUFFER_SMALL return:%d.\n", ret);
	}
	break;
	case TA_CMD_WITH_IN_BUFFER_LARGE: {
		/* Req buf smaller than size. */
		ret = test_ifaa_invoke(&session, ifaa_cmd_data,
				       get_cmd_len(cmd_id) - 2, rsp_buf,
				       &rsp_len);
		printf("Test TA_CMD_WITH_IN_BUFFER_LARGE return:%d.\n", ret);
	}
	break;
	case TA_CMD_WITH_IN_BUFFER_HUGE: {
		/* Req buf is very HUGE! */
		ret = test_ifaa_invoke(&session, ifaa_cmd_data,
				       get_cmd_len(cmd_id), rsp_buf, &rsp_len);
		printf("Test TA_CMD_WITH_IN_BUFFER_HUGE return:%d.\n", ret);
	}
	break;
	default: {
		if (test_ifaa_invoke(&session, ifaa_cmd_data,
				     get_cmd_len(cmd_id),
				     rsp_buf, &rsp_len) != 0) {
			printf("Test ifaa invoke failed.\n");
		}
	}
	break;
	}
	printf("Success to invoke ifaa cmd!\n");

	TEEC_CloseSession(&session);

cleanup:
	TEEC_FinalizeContext(&context);

	return 0;
}
