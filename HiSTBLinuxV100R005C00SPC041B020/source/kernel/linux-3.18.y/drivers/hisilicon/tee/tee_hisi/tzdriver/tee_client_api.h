/**
 * @file tee_client_api.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义非安全客户端接口\n
 */

/** @defgroup TEEC_API 客户端(非安全侧)接口
 */
/**@defgroup TEEC_BASIC_FUNC 通用接口
 *@ingroup TEEC_API
*/
/* TODO: hi3798mx compile */
#ifndef _TEEC_CLIENT_API_H_
#define _TEEC_CLIENT_API_H_

#include "teek_client_api.h"

#define TEEC_InitializeContext(name, context, noused) TEEK_InitializeContext(name, context)

#define TEEC_FinalizeContext(context, noused) TEEK_FinalizeContext(context)

#define TEEC_OpenSession(context, session, destination, connectionMethod, connectionData, operation, returnOrigin, noused) \
		TEEK_OpenSession(context, session, destination, connectionMethod, connectionData, operation, returnOrigin)	

#define TEEC_CloseSession(session, noused) TEEK_CloseSession(session)

#define TEEC_InvokeCommand(session, commandID, operation, returnOrigin, noused) TEEK_InvokeCommand(session, commandID, operation, returnOrigin)

#define TEEC_RegisterSharedMemory(context, sharedMem) TEEK_RegisterSharedMemory(context, sharedMem)

#define TEEC_AllocateSharedMemory(context, sharedMem) TEEK_AllocateSharedMemory(context, sharedMem)

#define TEEC_ReleaseSharedMemory(sharedMem) TEEK_ReleaseSharedMemory(sharedMem)

#define TEEC_RequestCancellation(operation) TEEK_RequestCancellation(operation)

#endif

