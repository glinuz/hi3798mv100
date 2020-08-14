/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : ta_framework.h
* Description :
* Author      : yaodongdong@huawei.com
* Date        : 2012.12.31
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

#ifndef __TA_FRAMWORK_H_
#define __TA_FRAMWORK_H_

#include <sre_task.h>
#include <sre_mem.h>
#include <sre_sem.h>
#include <sre_msg.h>
#include <sre_securemmu.h>
#include <sre_ticktimer.h>


#include "tee_internal_api.h"
#include "tee_common.h"
#include "tee_fs.h"
#include "dx_infra_api.h"
#include "tee_time_api.h"
#include "tee_core_api.h"

#define CALL_TRUSTZONE_API  0x1

#define CMD_TYPE_NS_TO_SECURE       0x1
#define CMD_TYPE_SECURE_TO_SECURE   0x2
#define CMD_TYPE_SECURE_ANSWER      0x3
#define CMD_TYPE_SECURE_CONFIG      0xf

#define TASK_INVALID_HANDLE    ((TSK_HANDLE_T)0xffffffff)
#define MSG_DEFAULT_HANDLE    (0)
#define MSG_DEFAULT_VALUE    (0xff)

#define GLOBAL_SERVICE_NAME   OS_INIT_TASK_NAME
#define SSA_SERVICE_NAME "task_ssa"
#define SEC_STORAGE_AGENT
#define GP_API_USES_SS_AGENT

//judge return value is error
#define IS_ERR_RET(result) ((result==TEE_SUCCESS || result==TEE_PENDING || result==TEE_PENDING2) ? false : true)

typedef paddr_t      pa_t;
typedef unsigned int     va_t;

typedef struct
{
	unsigned int    uuid_phys;
	unsigned int    uuid_h_phys;
	unsigned int    cmd_id;
	unsigned int    dev_file_id;
	unsigned int    context;    // high_16bits = service_index | low16_bits = session_id
	unsigned int    agent_id;
	unsigned int    operation_phys;
	unsigned int    operation_h_phys;
	unsigned int    login_method;
	unsigned int    login_data_phy;
	unsigned int    login_data_h_phy;
	unsigned int    login_data_len;
	unsigned int    err_origin;
	unsigned int    ret_val;
	unsigned int    event_nr;
	unsigned int    remap;
	unsigned int    uid;
	bool   started;
}__attribute__((__packed__)) smc_cmd_t;

#define MAX_SMC_CMD 25

typedef struct
{
	volatile uint32_t last_in;
	smc_cmd_t in[MAX_SMC_CMD];
	volatile uint32_t last_out;
	smc_cmd_t out[MAX_SMC_CMD];
} __attribute__((__packed__)) nwd_cmd_t;

#define SERVICE_NAME_MAX    100
#define SESSION_MAX         5   //concurrent opened session count

#define AGENT_BUFF_SIZE     (4*1024)
//agent id
#define TEE_FS_AGENT_ID     0x46536673      //FSfs
#define TEE_MISC_AGENT_ID   0x4d495343      //MISC
#define TEE_RPMB_AGENT_ID 0x4abe6198 //RPMB
#define TEE_SOCKET_AGENT_ID 0x69e85664 //socket

//session context contain of service index(high 16 bits) and session id(low 16 bits)
#define SET_SESSION_CONTEXT(index, id)  ((index<<16) | (id&0x0000ffff))
#define SERVICE_INDEX_OF_CONTEXT(context)   (context >> 16)
#define SESSION_ID_OF_CONTEXT(context)  (context&0x0000ffff)

typedef struct {
	TEE_UUID uuid;
	uint32_t stack_size;
	uint32_t heap_size;
	bool single_instance;
	bool multi_session;
	bool keep_alive;
	char reserved;
	char *other_buff;   //TA's non-std property
	uint32_t other_len; //non-std propery buff len
} TA_property_t;

typedef struct {
	TEE_UUID uuid;
	uint64_t permissions;
} TA_permission_t;

typedef struct {
	TEE_UUID uuid;
	uint32_t threshold;
} TA_rpmb_threshold_t;

typedef struct {
	TEE_Result ret_val;
	uint32_t ret_origin;
	bool cmd_processed;
} TA_retval_t;

//data for async call
struct timer_notify_data {
	uint32_t dev_id;
	TEE_UUID uuid;
	uint32_t session_id;
	TEE_timer_property property;
	uint32_t expire_time;
};

#define NOTIFY_DATA_MAX_NUM  7
#define NOTIFY_DATA_ENTRY_SIZE  512
#define NOTIFY_BUFF_SIZE 504

typedef union {
	struct timer_notify_data notify_context_timer;
	char notify_context_tp[NOTIFY_BUFF_SIZE];
} notify_context;

typedef struct {
	uint32_t entry_type;
	uint32_t filled;
	notify_context notify_context;
} notify_data_entry;

enum notify_data_entry_type {
	NOTIFY_DATA_ENTRY_RESERVED,
	NOTIFY_DATA_ENTRY_TIMER,
	NOTIFY_DATA_ENTRY_TP,
	NOTIFY_DATA_ENTRY_MAX = 8
};

//global call TA entrypoit cmd
#define CALL_TA_OPEN_SESSION        0x11
#define CALL_TA_INVOKE_CMD          0x12
#define CALL_TA_CLOSE_SESSION       0x13
#define CALL_TA_OPEN_SESSION_INIT   0x17
#define CALL_TA_OPEN_SESSION_PROP   0x18

#define TA2TA_CALL                  0x41
#define MSG_ABORT_VALUE             0xf0
#define TEE_PANIC_VALUE             0xf1
#define TA_GET_AGENT_BUFFER         0x20
#define TA_CALL_AGENT               0x21
#define TA_LOCK_AGENT               0x22
#define TA_UNLOCK_AGENT             0x23
#define TA_GET_REEINFO              0x24
#define TA_GET_CALLERINFO           0x25
#define SSA_SET_CALLERINFO          0x26

#define CMD_TP_EVENT                0xDDEC

#define GLOBAL_TSK_ID (0)
#define REET_TSK_ID   (1)

struct global_to_TA_msg {
	uint32_t session_id;
	uint32_t session_type;
	uint32_t cmd_id;
	uint32_t param_type;
	TEE_Param *params;
	void *session_context;
	uint32_t dev_id;
	char first_session;
	char last_session;
	bool started;
};

struct TA_to_global_msg {
	TEE_Result ret;
	uint32_t agent_id;
	void *session_context;
	uint32_t ta2ta_from_taskid;
};

struct global_to_ta_for_uid {
	uint32_t userid;
	uint32_t appid;
	uint32_t cmd_id;
	uint32_t reserved;
};

struct TA_init_msg {
	void *fs_mem;   //fs agent share mem
	void *misc_mem; //misc agent share mem
	TA_property_t prop;
	uint32_t login_method;
	void *time_data;    //async call notify share data
	TEE_Time sys_time;
	uint32_t rtc_time;
};

struct TA_ContentPath_msg {
	ContentPath content_path;
};

struct TA2TA_ret_msg {
	TEE_Result ret;
	uint32_t origin;
	TEE_TASessionHandle session;
};

struct TA2TA_msg {
	TEE_Result ret;
	void *session_context;
	smc_cmd_t *cmd;
};


struct command_state {
	uint32_t cmd_id;
	uint32_t dev_id;
	uint32_t ta2ta_from_taskid;
	bool ta2ta_start;
	bool ta2ta_switch;
	bool ta2ta_done;
	bool started;
};

extern void init_non_std_property(char *buff, uint32_t len);
bool dynamic_service_exist(TEE_UUID *uuid, bool build_in);
TEE_Result register_service(smc_cmd_t *cmd, const char *name, TEE_UUID *uuid, struct sessison_task_info *info, bool build_in);
TEE_Result ta_framework_init(void);
void reset_ta_context(void);
void init_service_property(TEE_UUID *uuid, uint32_t stack, uint32_t heap,
			   bool single_instance, bool multi_session, bool keep_alive,
			   const char *other_buff, uint32_t other_len);
TEE_Result get_build_in_services_property(TEE_UUID *service, TA_property_t *property);
void init_task_handle_array(void);
void set_TEE_return(TEE_Result ret_val);
void set_TEE_processed(bool processed);

TEE_Result proceed_return_code(smc_cmd_t *cmd);

TEE_Result get_TEE_return(void);
bool is_command_processed(void);
bool is_ta2ta_command(void);
void clear_ta2ta_return_bit(void);
void TA2TA_switch_step1(void);
TEE_Result TA2TA_switch_step2(smc_cmd_t *cmd);
TEE_Result start_TA_task(smc_cmd_t *cmd, uint32_t cmd_type);

TEE_Result cmd_ns_get_params(uint32_t task_id, smc_cmd_t *cmd, uint32_t *param_type, TEE_Param **params);
TEE_Result cmd_secure_get_params(uint32_t task_id, smc_cmd_t *cmd, uint32_t *param_type, TEE_Param **params);
TEE_Result cmd_global_ns_get_params(smc_cmd_t *cmd, uint32_t *param_type, TEE_Param **params);

TEE_Result init_ta_context(smc_cmd_t *cmd, uint32_t cmd_type, uint32_t task_id, bool *is_global, uint32_t *cmd_id);
TEE_Result init_ta2ta_context(smc_cmd_t **out_cmd, UINT32 ta_cmd, uint32_t cmd_type, uint32_t task_id, bool *is_global, uint32_t *cmd_id);
TEE_Result init_session_context(uint32_t task_id, TEE_Result result, smc_cmd_t **cmd, uint32_t *cmd_type);
uint32_t get_session_task_id(); /*ta2ta_from_taskid*/
uint32_t get_current_task_id(); /*taskid*/

TEE_Result start_reet(TEE_UUID *uuid, uint32_t *reet_id);

bool need_load_srv(TEE_UUID *uuid);
TEE_Result open_session(smc_cmd_t *cmd, uint32_t cmd_type, uint32_t task_id, bool *sync);
TEE_Result close_session(smc_cmd_t *cmd, uint32_t cmd_type, bool *sync);
TEE_Result kill_task(smc_cmd_t *cmd, uint32_t cmd_type);
TEE_Result kill_crash_task(void);
bool process_invoke_cmd();

TEE_Result process_unlock_agent(struct TA_to_global_msg *ret_msg, uint32_t task_id);
TEE_Result process_open_session(smc_cmd_t *cmd, uint32_t cmd_type);
void process_open_session_error(smc_cmd_t *cmd);
void process_timer_message(uint32_t timer_class, timer_private_data_kernel *timer_data);

void *map_ns_cmd(pa_t cmd_phy);
TEE_Result map_ns_operation(smc_cmd_t *cmd);
TEE_Result unmap_ns_operation(smc_cmd_t *cmd);
TEE_Result map_secure_operation(UINT32 tacmd, smc_cmd_t **out_cmd, UINT32 task_id);
TEE_Result map_secure_agent_operation(smc_cmd_t *cmd);
TEE_Result unmap_secure_operation(smc_cmd_t *cmd, uint32_t task_id, uint8_t uuid_only);


TEE_Result register_notify_memery(smc_cmd_t *cmd);
TEE_Result unregister_notify_memery(smc_cmd_t *cmd);
TEE_Result register_agent(smc_cmd_t *cmd);
TEE_Result unregister_agent(smc_cmd_t *cmd);
TEE_Result process_get_agent_buffer(struct TA_to_global_msg *ret_msg, uint32_t task_id);
TEE_Result process_get_reeinfo(struct TA_to_global_msg *ret_msg, uint32_t task_id);
void process_get_callerinfo(uint32_t task_id);
TEE_Result set_agent_id(smc_cmd_t *cmd, uint32_t agent_id);
void map_exception_mem(smc_cmd_t *cmd);
bool task_unlock_agents();

TEE_Result init_content_path(smc_cmd_t *cmd);
TEE_Result terminate_content_path(smc_cmd_t *cmd);
void process_terminate_content_path();

void init_startup_time(void);
TEE_Result get_tee_time(smc_cmd_t *cmd);
TEE_Result tee_get_agent_buffer(uint32_t agent_id, paddr_t *buffer, uint32_t *length);
TEE_Result tee_unlock_agents(uint32_t task_id);
void dump_task(void);

#endif
