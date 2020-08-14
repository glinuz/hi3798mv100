/*
 * FileName:      rootagent_api.h
 * Author:        z00321956
 * Description:   rootagent API
 * Version:       0.0.1
 * Date:          2016-04-01
 */
#ifndef _ROOT_MONITOR_H_
#define _ROOT_MONITOR_H_

#include "rootagent_common.h"

/*
 * TA UID
 */
#define UUID_RM_TA TEE_SERVICE_ANTIROOT

/**
 * @Function:  root_monitor_tee_init
 *
 * @brief: This function initial TEE environment
 * (context, session), create two share memory,
 * one is used to read, and anther is used to write.
 *
 * @attention: This function should be invoked first.
 *
 * @param: void
 *
 * @retval: see teek_client_constants.h
 *
 * @depend on: teek_client_api.c teek_root_monitor_mem.c
 *
 */
TEEC_Result root_monitor_tee_init(void);

 /**
 * @Function:  setting_config
 *
 * @brief: This function set the white information
 * which is used to do check operations
 * into the TEE environment
 *
 * @attention: When Linux kernel start up,
 * The function should be invoked
 * after root_monitor_tee_init, but before other functions.
 *
 * @param: TEE_RM_CONFIG, see teek_root_monitor_common.h
 *
 * @retval: see teek_client_constants.h
 *
 * @depend on: teek_client_api.c
 *
 */
TEEC_Result setting_config(struct RAGENT_CONFIG *config,
		struct RAGENT_ROOTPROC *root_proc);

 /**
 * @Function:  request_challenge
 *
 * @brief: This function request
 * a challenge generated
 * by TEE environment,
 * According to the challenge,
 * we do corresponding scanning and reply.
 *
 * @attention: null
 *
 * @param: void
 *
 * @retval: see teek_client_constants.h
 *
 * @depend on: teek_client_api.c teek_root_monitor_mem.c
 *
 */
TEEC_Result request_challenge(struct RAGENT_CHALLENGE *ichallenge);

 /**
 * @Function:  feedback_response
 *
 * @brief: This function send the result of scanning to TEE,
 * and TEE compare the result with white information.
 *
 * @attention: This function should be invoked after request_challenge.
 *
 * @param: void
 *
 * @retval: see teek_client_constants.h
 *
 * @depend on: teek_client_api.c teek_root_monitor_mem.c
 *
 */
TEEC_Result
feedback_response(struct RAGENT_RESPONSE *response,
		struct RAGENT_ROOTPROC *root_proc);

void root_monitor_finalize(void);
#endif
