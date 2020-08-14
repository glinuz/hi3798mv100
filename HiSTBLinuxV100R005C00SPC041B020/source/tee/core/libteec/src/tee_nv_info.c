/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : tee_nv_info.c
* Description :
* Platform    :
* Author      : qiqingchao
* Version     : V1.0
* Date        : 2013.5.21
* Notes       :
*
*------------------------------------------------------------------------------
* Modifications:
*   Date        Author          Modifications
*******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "securec.h"
#include "tee_client_type.h"
#include "tee_nv_info.h"
#include "tee_client_api.h"
/* #include "nve_interface.h" */

#define NV_NAME_LENGTH          8           /*NV name maximum length*/
#define NVE_NV_DATA_SIZE        104         /*NV data maximum length*/

/* #define NV_WRITE                0          NV write operation*/
#define NV_READ                 1           /*NV read  operation*/

struct nve_info_user {
	unsigned long nv_operation;             /*0-write,1-read*/
	unsigned long nv_number;                /*NV number you want to visit*/
	char nv_name[NV_NAME_LENGTH];
	unsigned long valid_size;
	unsigned char nv_data[NVE_NV_DATA_SIZE];
};

extern int nve_direct_access(struct nve_info_user *info_user);

int handle_nv(SecureBoot_LockInfo *info)
{
	struct nve_info_user nvinfo;
	nvinfo.nv_operation = NV_READ;
	nvinfo.nv_number = WIDEVINE_NV_WVLOCK_NUM;
	nvinfo.valid_size = WIDEVINE_NV_WVLOCK_SIZE;

	errno_t rc = strncpy_s(nvinfo.nv_name, strlen(WIDEVINE_NV_WVLOCK_NAME) + 1,
			       WIDEVINE_NV_WVLOCK_NAME, strlen(WIDEVINE_NV_WVLOCK_NAME) + 1);
	if (rc != EOK) {
		TEEC_Error("[error]handle_nv strncpy_s error: rc=%d\n", rc);
	}
	if (nve_direct_access(&nvinfo)) {
		TEEC_Debug("handle_nv: nve_direct_access read error\n");
		return -1;
	}

	rc = memcpy_s(info, sizeof(SecureBoot_LockInfo), nvinfo.nv_data,
		      sizeof(SecureBoot_LockInfo));
	if (rc != EOK) {
		TEEC_Error("[error]handle_nv memcpy_s error: rc=%d\n", rc);
	}
	TEEC_Debug("info:%s", (uint8_t *)info);
	return 0;
}

