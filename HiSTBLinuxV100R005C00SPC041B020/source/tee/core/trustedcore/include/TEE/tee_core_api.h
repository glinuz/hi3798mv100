#ifndef __TEE_CORE_API_H
#define __TEE_CORE_API_H

#include "tee_internal_api.h"
#ifndef _TEE_TASessionHandle
#define _TEE_TASessionHandle
typedef uint32_t TEE_TASessionHandle;
#endif

#define SESSION_FROM_CA             0
#define SESSION_FROM_TA             1
#define SESSION_FROM_UNKNOWN       (0xFF)

void TEE_Panic(TEE_Result panicCode);

TEE_Result TEE_OpenTASession(TEE_UUID *destination, uint32_t cancellationRequestTimeout,
			     uint32_t paramTypes, TEE_Param params[4],
			     TEE_TASessionHandle *session, uint32_t *returnOrigin);

void TEE_CloseTASession(TEE_TASessionHandle session);

TEE_Result TEE_InvokeTACommand(TEE_TASessionHandle session, uint32_t cancellationRequestTimeout,
			       uint32_t commandID, uint32_t paramTypes,
			       TEE_Param params[4], uint32_t *returnOrigin);

bool TEE_GetCancellationFlag(void);

bool TEE_UnmaskCancellation(void);

bool TEE_MaskCancellation(void);

uint32_t TEE_GetSessionType(void);
#endif
