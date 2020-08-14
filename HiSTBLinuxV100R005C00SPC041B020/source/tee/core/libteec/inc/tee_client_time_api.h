#ifndef __TEEC_CLIENT_TIME_API_H
#define __TEEC_CLIENT_TIME_API_H

#include "tee_client_type.h"

TEEC_Result TEEC_GetTEETime(struct timeval *tv);

#endif