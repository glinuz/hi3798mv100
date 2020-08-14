#ifndef __TEE_INIT_H
#define __TEE_INIT_H

#include "tee_internal_api.h"
#include "ta_framework.h"
#define GLOBAL_HANDLE 0
TEE_Result tee_init(struct TA_init_msg *init_msg);

void tee_exit(void);

void tee_session_init(uint32_t session_id);

void tee_session_exit(uint32_t session_id);

void tee_init_context(uint32_t session_id, uint32_t dev_id);

TEE_UUID *get_current_uuid(void);

uint32_t get_current_session_id(void);

uint32_t get_current_dev_id(void);

void set_global_handle(uint32_t handle);

uint32_t get_global_handle(void);

void set_current_session_type(uint32_t session_type);

uint32_t get_current_session_type(void);
#endif
