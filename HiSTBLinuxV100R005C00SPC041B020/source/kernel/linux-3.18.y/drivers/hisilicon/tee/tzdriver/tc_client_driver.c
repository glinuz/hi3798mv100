/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : tc_client_driver.c
* Description :
* Platform    :
* Author      : qiqingchao
* Version     : V1.0
* Date        : 2012.12.10
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
#include <linux/module.h>
#include <linux/time.h>

#include <tc_ns_client.h>
#include "tee_client_constants.h"
#include "tee_client_id.h"
//#include "hi_drv_mmz.h"

#define TC_NS_CLIENT_DEV "tc_ns_client"
//#define TC_ASYNC_NOTIFY_SUPPORT
#define ROUND_UP(N, S) ((((N) + (S) - 1) / (S)) * (S))

#define MAPI_TZ_API 0x1     //monitor api map defined in mach-k3v2/k3v2-monitorAPI.h
#define EXCEPTION_MEM_SIZE (8*1024)     //mem for exception handling

#define TEEC_PARAM_TYPES( param0Type, param1Type, param2Type, param3Type) \
    ((param3Type) << 12 | (param2Type) << 8 | (param1Type) << 4 | (param0Type))

#define TEEC_PARAM_TYPE_GET( paramTypes, index) \
    (((paramTypes) >> (4*(index))) & 0x0F)

enum TC_NS_CMD_type {
    TC_NS_CMD_TYPE_INVALID = 0,
    TC_NS_CMD_TYPE_NS_TO_SECURE,
    TC_NS_CMD_TYPE_SECURE_TO_NS,
    TC_NS_CMD_TYPE_SECURE_TO_SECURE,
    TC_NS_CMD_TYPE_MAX
};

typedef union
{
    struct
    {
        unsigned int buffer;
        unsigned int size;
    } memref;
    struct
    {
        unsigned int a;
        unsigned int b;
    } value;
    struct
    {
        void* handle1;
        void* handle2;
        unsigned int offset;
        unsigned int size;
    } sharememref;
} TC_NS_Parameter;

typedef struct tag_TC_NS_Login{
    unsigned int method;
    unsigned int mdata;
} TC_NS_Login;

typedef struct tag_TC_NS_Operation{
    unsigned int    paramTypes;
    TC_NS_Parameter params[4];
} TC_NS_Operation;

typedef struct tag_TC_NS_SMC_CMD{
    unsigned int    uuid_phys;
    unsigned int    cmd_id;
    unsigned int    dev_file_id;
    unsigned int    context_id;
    unsigned int    agent_id;
    unsigned int    operation_phys;
    unsigned int    login_method;
    unsigned int    login_data;
    unsigned int    err_origin;
} TC_NS_SMC_CMD;

struct tc_notify_data {
    unsigned int dev_file_id;
    unsigned char uuid[16];
    unsigned int session_id;
};

#ifdef  TC_ASYNC_NOTIFY_SUPPORT
static struct tc_notify_data *notify_data = NULL;
static DEFINE_MUTEX(notify_data_lock);
#endif
static DEFINE_MUTEX(smc_lock);
static DEFINE_MUTEX(open_session_lock);
static DEFINE_MUTEX(close_session_lock);
static DEFINE_MUTEX(send_cmd_lock);
static DEFINE_MUTEX(shared_buf_rel_lock);
static DEFINE_MUTEX(load_app_lock);
static int load_app_lock_flag = 0;
/*DTS2013030508974 q00209673 begin*/
static DEFINE_MUTEX(device_file_cnt_lock);
/*DTS2013030508974 q00209673 end */
/************global reference start***********/
static dev_t tc_ns_client_devt;
static struct class *driver_class;
static struct cdev tc_ns_client_cdev;
static unsigned int device_file_cnt = 1;
static unsigned int exception_mem_addr;

typedef struct tag_TC_NS_DEV_List{
    unsigned int dev_file_cnt;
/*DTS2013030508974 q00209673 begin*/
    struct mutex dev_lock;
/*DTS2013030508974 q00209673 end */
    struct list_head dev_file_list;

} TC_NS_DEV_List;
static TC_NS_DEV_List tc_ns_dev_list;
/* DTS2013030508974 q00209673 begin
typedef struct tag_TC_NS_Shared_MEM_List{
    int shared_mem_cnt;
    struct list_head shared_mem_list;
} TC_NS_Shared_MEM_List;
static TC_NS_Shared_MEM_List tc_shared_mem_head;
DTS2013030508974 q00209673 end */
#ifdef K_MALLOC
typedef struct tag_TC_NS_Shared_MEM{
    void*            kernel_addr;
    void*             user_addr;
    unsigned int    len;
    struct list_head head;
} TC_NS_Shared_MEM;
#else
typedef struct tag_TC_NS_Shared_MEM{
    void*            kernel_addr;
    //MMZ_BUFFER_S mmz_addr;
    void*             user_addr;
    unsigned int    len;
    struct list_head head;
} TC_NS_Shared_MEM;
#endif
/************global reference end*************/

typedef struct tag_TC_NS_DEV_File{
    unsigned int dev_file_id;
    unsigned int service_cnt;
/*DTS2013030508974 q00209673 begin*/
    unsigned int shared_mem_cnt;
    struct mutex service_lock;
    struct mutex shared_mem_lock;
    struct list_head shared_mem_list;
/*DTS2013030508974 q00209673 end */
    struct list_head head;
    struct list_head services_list;
} TC_NS_DEV_File;

typedef struct tag_TC_NS_Service{
    unsigned char uuid[16];
/*DTS2013030508974 q00209673 begin*/
    struct mutex session_lock;
/*DTS2013030508974 q00209673 end */
    struct list_head head;
    struct list_head session_list;
} TC_NS_Service;

/**
 * @brief
 */
struct TC_wait_data {
    wait_queue_head_t send_cmd_wq;
    int               send_wait_flag;
};

typedef struct tag_TC_NS_Session{
    unsigned int session_id;
    struct list_head head;
    struct TC_wait_data wait_data;
} TC_NS_Session;

//for secure agent
struct __smc_event_data{
    unsigned int agent_id;
    struct mutex work_lock;
    wait_queue_head_t wait_event_wq;
    int ret_flag;
    wait_queue_head_t send_response_wq;
    int send_flag;
    struct list_head head;
    void* buffer;
};
//struct __smc_event_data* smc_event_data;

struct __agent_control{
    spinlock_t lock;
    struct list_head agent_list;
};
static struct __agent_control agent_control;

#if 0
static HI_S32 HI_DRV_MMZ_AllocAndMap_cached(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = 0;
    s32Ret = HI_DRV_MMZ_Alloc(bufname, zone_name, size, align, psMBuf);
    if (s32Ret)
    {
        TCERR("MMZ alloc error!\n");
        return -1;
    }

    s32Ret = HI_DRV_MMZ_MapCache(psMBuf);
    if (s32Ret)
    {
        TCERR("MMZ alloc error!\n");
        return -1;
    }

    return 0;
}

static HI_VOID HI_DRV_MMZ_UnmapAndRelease_cached(MMZ_BUFFER_S *psMBuf)
{
    HI_DRV_MMZ_Unmap(psMBuf);
    HI_DRV_MMZ_Release(psMBuf);
}
#endif
static unsigned int smc_send(unsigned int cmd_addr)
{
    register unsigned int r0 asm("r0") = MAPI_TZ_API;
    register unsigned int r1 asm("r1") = cmd_addr;
    register unsigned int r2 asm("r2") = TC_NS_CMD_TYPE_NS_TO_SECURE;
    do {
        asm volatile(
            __asmeq("%0", "r0")
            __asmeq("%1", "r0")
            __asmeq("%2", "r1")
            __asmeq("%3", "r2")
#if GCC_VERSION >= 40600//for hisi linux:by fangjian00208632
            ".arch_extension sec\n"
#endif
            "smc    #0  @ switch to secure world\n"
            : "=r" (r0)
            : "r" (r0), "r" (r1), "r" (r2));
    } while (r0 == 1);

    return r0;
}

unsigned int smc_send_on_cpu0(unsigned int cpu_id, unsigned int cmd_addr);
static unsigned int TC_NS_SMC(unsigned int cmd_addr)
{
    unsigned int cpu_id;
    unsigned int ret;

    cpu_id = smp_processor_id();
    //TCDEBUG(KERN_INFO"***TC_NS_SMC call start on cpu %d ***\n", cpu_id);
    /* TODO
       directily call smc_send on cpu0 will call SMP PREEMPT error,
       so just use thread to send smc
    */
#if 0
    /*cpu_id = get_cpu();
    put_cpu();*/

    if(cpu_id == 0) {
        ret = smc_send(cmd_addr);
    } else {
        mb();
        ret = smc_send_on_cpu0(cpu_id, cmd_addr);
    }
#else
    ret = smc_send_on_cpu0(cpu_id, cmd_addr);
#endif
    //TCDEBUG(KERN_INFO"***TC_NS_SMC call end on cpu %d***\n", cpu_id);

    return ret;
}

static unsigned int TC_NS_incomplete_proceed(TC_NS_SMC_CMD *smc_cmd, int session_id, unsigned int agent_id)
{
    int find_flag=0;
    unsigned int smc_cmd_phys;
    unsigned int ret = TEEC_PENDING2;
    struct __smc_event_data *event_data;
    unsigned long flags;

    spin_lock_irqsave(&agent_control.lock, flags);
    list_for_each_entry(event_data, &agent_control.agent_list, head){
        if(event_data->agent_id == agent_id){
            find_flag = 1;
            break;
        }
    }
    spin_unlock_irqrestore(&agent_control.lock, flags);

    if(find_flag == 0){
        //TODO if return, the pending task in S can't be resume!!
        TCERR("agent not exist\n");
        return TEEC_ERROR_GENERIC;
    }

    mutex_lock(&event_data->work_lock);

    //just return for the first call
    smc_cmd->context_id = session_id;
    smc_cmd_phys = virt_to_phys((void*)smc_cmd);
    mutex_lock(&smc_lock);
    TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

    event_data->ret_flag = 1;
    wake_up_interruptible(&event_data->wait_event_wq);
    wait_event_freezable(event_data->send_response_wq, event_data->send_flag);
    event_data->send_flag = 0;

    //send the incomplete id
    smc_cmd->context_id = session_id;
    smc_cmd_phys = virt_to_phys((void*)smc_cmd);
    mutex_lock(&smc_lock);
    ret = TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

    mutex_unlock(&event_data->work_lock);

    return ret;
}


static int TC_NS_SMC_Call(TC_NS_ClientContext *client_context, TC_NS_DEV_File *dev_file, bool is_global)
{
    int ret;
    unsigned int tee_ret;
    TC_NS_Operation *operation = NULL;
    TC_NS_SMC_CMD *smc_cmd = NULL;
    unsigned char uuid[17];
    TC_NS_ClientParam *client_param = NULL;
    unsigned int param_type;
    TC_NS_Shared_MEM *shared_mem = NULL;
    unsigned int smc_cmd_phys;
    int i;
    void *temp_buf;
    void *local_temp_buffer[4] = {0};
    unsigned int trans_paramtype_to_tee[4];
    //MMZ_BUFFER_S buf;
    //MMZ_BUFFER_S local_temp_buf[4];

    if(!client_context){
        TCERR("client_context is null");
        ret = -EINVAL;
        goto error;
    }

    smc_cmd = (TC_NS_SMC_CMD *)kzalloc(sizeof(TC_NS_SMC_CMD), GFP_KERNEL);
    if(!smc_cmd){
        TCERR("smc_cmd malloc failed");
        ret = -ENOMEM;
        goto error;
    }

    if(client_context->paramTypes != 0){
        operation = (TC_NS_Operation *)kzalloc(sizeof(TC_NS_Operation), GFP_KERNEL);
        if(!operation){
            TCERR("login malloc failed");
            ret = -ENOMEM;
            goto error;
        }
    }

    //check if operation is empty
    if(operation){
        for(i = 0; i < 4; i++){
            client_param = &(client_context->params[i]);
            param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
            //if buffer is not null
            //temp buffer
            if((param_type == TEEC_MEMREF_TEMP_INPUT) ||
                (param_type == TEEC_MEMREF_TEMP_OUTPUT)||
                (param_type  == TEEC_MEMREF_TEMP_INOUT)){
                temp_buf = (void *)kzalloc(*(unsigned int *)client_param->memref.size_addr, GFP_KERNEL);
                if(!temp_buf){
                    TCERR("temp_buf malloc failed");
                    ret = -ENOMEM;
                    goto error;
                }
                if((param_type == TEEC_MEMREF_TEMP_INPUT) ||
                    (param_type  == TEEC_MEMREF_TEMP_INOUT)){
                    if(copy_from_user(temp_buf, (void *)client_param->memref.buffer,
                        *(unsigned int *)client_param->memref.size_addr)) {
                        TCERR("copy from user failed \n");
                            ret = -ENOMEM;
                            goto error;
                    }
                }
                operation->params[i].memref.buffer = virt_to_phys((void *)temp_buf);
                local_temp_buffer[i] = temp_buf;
                //local_temp_buf[i] = buf;
                operation->params[i].memref.size = *(unsigned int *)client_param->memref.size_addr;
                trans_paramtype_to_tee[i] = param_type; //TEEC_MEMREF_TEMP_INPUT equal to TEE_PARAM_TYPE_MEMREF_INPUT
            }
            //register buffer
            else if((param_type == TEEC_MEMREF_PARTIAL_INPUT)||
                (param_type == TEEC_MEMREF_PARTIAL_OUTPUT)||
                (param_type == TEEC_MEMREF_PARTIAL_INOUT)){
                //find kernel addr refered to user addr
                list_for_each_entry(shared_mem, &dev_file->shared_mem_list, head){
                    if(shared_mem->user_addr == (void*)client_param->memref.buffer){
                        operation->params[i].memref.buffer =
                            virt_to_phys((void *)shared_mem->kernel_addr + client_param->memref.offset);
                        break;
                    }
                }
                if(!operation->params[i].memref.buffer){
                    TCERR("can not find shared buffer, exit\n");
                    ret = -EINVAL;
                    goto error;
                }
                operation->params[i].memref.size = *(unsigned int *)client_param->memref.size_addr;
                trans_paramtype_to_tee[i] = param_type - 8; //TEEC_MEMREF_PARTIAL_INPUT - TEE_PARAM_TYPE_MEMREF_INPUT = 8
            }
            //value
            else if((param_type == TEEC_VALUE_INPUT)||
                (param_type == TEEC_VALUE_OUTPUT)||
                (param_type == TEEC_VALUE_INOUT)){
                operation->params[i].value.a = *(u32 *)client_param->value.a_addr;
                operation->params[i].value.b = *(u32 *)client_param->value.b_addr;
                trans_paramtype_to_tee[i] = param_type; //TEEC_VALUE_INPUT equal to TEE_PARAM_TYPE_VALUE_INPUT
            }
            else if(param_type == TEEC_DX_SHARE_MEM){
                //find kernel addr refered to user addr
                list_for_each_entry(shared_mem, &dev_file->shared_mem_list, head){
                    if(shared_mem->user_addr == (void*)client_param->memref.buffer){
                        operation->params[i].sharememref.handle1 =
                            (void*)virt_to_phys((void *)shared_mem->kernel_addr);
                        break;
                    }
                }
                if(!operation->params[i].sharememref.handle1){
                    TCERR("can not find shared buffer, exit\n");
                    ret = -EINVAL;
                    goto error;
                }

                operation->params[i].sharememref.offset = client_param->memref.offset;
                operation->params[i].sharememref.size = *(unsigned int *)client_param->memref.size_addr;
                trans_paramtype_to_tee[i] = param_type;
                printk("operation->params[%d].sharememref.handle1 = 0x%x\n", i, (unsigned int)operation->params[i].sharememref.handle1);
            }
            else{
                TCDEBUG("param_type = TEEC_NONE\n");
                trans_paramtype_to_tee[i] = TEE_PARAM_TYPE_NONE;
            }
        }
    }
    uuid[0] = ((is_global == true ) ? 1 : 0);
    memcpy(uuid + 1, client_context->uuid, 16);
    smc_cmd->uuid_phys = virt_to_phys((void*)uuid);
    smc_cmd->cmd_id = client_context->cmd_id;
    smc_cmd->dev_file_id=dev_file->dev_file_id;
    smc_cmd->context_id = client_context->session_id;
    smc_cmd->err_origin = 0;

    if(operation) {
        operation->paramTypes = TEEC_PARAM_TYPES(
            trans_paramtype_to_tee[0],
            trans_paramtype_to_tee[1],
            trans_paramtype_to_tee[2],
            trans_paramtype_to_tee[3]);
        smc_cmd->operation_phys = virt_to_phys(operation);
    } else
        smc_cmd->operation_phys = 0;

    smc_cmd->login_method = client_context->login.method;
    smc_cmd->login_data = client_context->login.mdata;

    smc_cmd_phys = virt_to_phys((void*)smc_cmd);
    //send smc to secure world
    mutex_lock(&smc_lock);  
    tee_ret = TC_NS_SMC(smc_cmd_phys); 
    mutex_unlock(&smc_lock);

    client_context->session_id = smc_cmd->context_id;

    if(tee_ret != 0){
        while(tee_ret == TEEC_PENDING2 || tee_ret == TEEC_PENDING){
            if(tee_ret == TEEC_PENDING2){
                unsigned int agent_id = smc_cmd->agent_id;

                tee_ret = TC_NS_incomplete_proceed(smc_cmd, client_context->session_id, agent_id);
            }
#ifdef  TC_ASYNC_NOTIFY_SUPPORT
            else if(tee_ret == TEEC_PENDING){
                mutex_lock(&dev_file->service_lock);
                list_for_each_entry(service, &dev_file->services_list, head){
                    if(memcmp(service->uuid, client_context->uuid, 16) == 0){
                        list_for_each_entry(session,&service->session_list,head){
                            if(session->session_id == client_context->session_id){
                                printk("come into wait data\n");
                                wq=&session->wait_data;
                                break;
                            }
                        }
                        break;
                    }

                }
                mutex_unlock(&dev_file->service_lock);

                if(wq){
                    printk("before wait event \n");
                    if(wait_event_interruptible(wq->send_cmd_wq,
                        wq->send_wait_flag)) {
                        ret = -ERESTARTSYS;
                        printk("wait event error\n");
                        goto error;
                    }
                    wq->send_wait_flag = 0;
                }

                uuid[0] = ((is_global == true ) ? 1 : 0);
                memcpy(uuid + 1, client_context->uuid, 16);
                smc_cmd->uuid_phys = virt_to_phys((void*)uuid);
                smc_cmd->cmd_id = client_context->cmd_id;
                smc_cmd->dev_file_id=dev_file->dev_file_id;
                smc_cmd->context_id = client_context->session_id;
                smc_cmd_phys=virt_to_phys((void*)smc_cmd);

                //printk("service id:0x%x ,cmd id:0x%x  session_id:0x%x\n",notify_data->service_id,cmd_id,notify_data->session_id);

                mutex_lock(&smc_lock);
                tee_ret = TC_NS_SMC(smc_cmd_phys);
                mutex_unlock(&smc_lock);
            }
#endif
        }
        if(tee_ret != 0){
            goto error1;
        }
    }

    if(operation){
        for(i = 0; i < 4; i++){
            client_param = &(client_context->params[i]);
            param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
            if((param_type ==  TEEC_MEMREF_TEMP_OUTPUT)||
                (param_type  == TEEC_MEMREF_TEMP_INOUT)){
                //temp buffer
                if(copy_to_user((void*)client_param->memref.buffer,
                	local_temp_buffer[i], operation->params[i].memref.size)){
                    TCERR("copy to user failed while copying tempbuf\n");
                    ret = -ENOMEM;
                    goto error;
                }
                /* update size */
                *(unsigned int *)client_param->memref.size_addr = operation->params[i].memref.size;
            } else if((param_type ==  TEEC_MEMREF_PARTIAL_OUTPUT)||
                (param_type  == TEEC_MEMREF_PARTIAL_INOUT)){
                //share memory
                /* update size */
                *(unsigned int *)client_param->memref.size_addr = operation->params[i].memref.size;
            } else if((param_type == TEEC_VALUE_OUTPUT)||
                (param_type == TEEC_VALUE_INOUT)){
                //value
                *(u32 *)client_param->value.a_addr = operation->params[i].value.a;
                *(u32 *)client_param->value.b_addr = operation->params[i].value.b;
            }else if(param_type == TEEC_DX_SHARE_MEM){
                /* update size */
                *(unsigned int *)client_param->memref.size_addr = operation->params[i].sharememref.size;
            }
        }
    }

    ret = 0;
    goto error;

error1:
    if (tee_ret == TEEC_ERROR_SHORT_BUFFER) {
        /* update size */
        if(operation){
            for(i = 0; i < 4; i++){
                client_param = &(client_context->params[i]);
                param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
                if((param_type ==  TEEC_MEMREF_TEMP_OUTPUT)||
                    (param_type  == TEEC_MEMREF_TEMP_INOUT)) {
                    //temp buffer
                    //only when teec size >= tee size, we can copy buffer
                    // if teec size < tee size, this is the reason of errno TEEC_ERROR_SHORT_BUFFER, so cannot copy buffer
                    if (*(unsigned int *)client_param->memref.size_addr >= operation->params[i].memref.size)
                    {
                        {
                            if(copy_to_user((void*)client_param->memref.buffer,
                            	local_temp_buffer[i], operation->params[i].memref.size)){
                                TCERR("copy to user failed while copying tempbuf\n");
                                ret = -ENOMEM;
                                goto error;
                            }
                        }
                    }
                    *(unsigned int *)client_param->memref.size_addr = operation->params[i].memref.size;
                } else if((param_type ==  TEEC_MEMREF_PARTIAL_OUTPUT)||
                    (param_type  == TEEC_MEMREF_PARTIAL_INOUT)){
                    //if shared memory, just update size
                    *(unsigned int *)client_param->memref.size_addr = operation->params[i].memref.size;
                }
            }
        }
    }

    client_context->returns.code = tee_ret;
    client_context->returns.origin = smc_cmd->err_origin;

    TCERR("session_id(0x%x), cmd_id(0x%x) context smc_call returns error origin(0x%x), ret(0x%x).\n", 
	client_context->session_id, client_context->cmd_id, smc_cmd->err_origin, tee_ret);
    dump_stack();

    ret = EFAULT;

error:
    if(smc_cmd)
        kfree(smc_cmd);

    if(operation){
        for(i = 0; i < 4; i++){
            if(local_temp_buffer[i])
                kfree(local_temp_buffer[i]);
        }
        kfree(operation);
    }

    return ret;
}

static int TC_NS_SMC_Call_from_kernel(TC_NS_ClientContext *client_context, TC_NS_DEV_File *dev_file, bool is_global)
{
    int ret;
    unsigned int tee_ret;
     TC_NS_Operation *operation = NULL;
    TC_NS_SMC_CMD *smc_cmd = NULL;
    unsigned char uuid[17];
    TC_NS_ClientParam *client_param = NULL;
    unsigned int param_type;
    TC_NS_Shared_MEM *shared_mem = NULL;
    unsigned int smc_cmd_phys;
    int i;
    void *temp_buf;
    void *local_temp_buffer[4] = {0};
    unsigned int trans_paramtype_to_tee[4];
    //MMZ_BUFFER_S buf;
    //MMZ_BUFFER_S local_temp_buf[4];

    if(!client_context){
        TCERR("client_context is null");
        ret = -EINVAL;
        goto error;
    }

    smc_cmd = (TC_NS_SMC_CMD *)kzalloc(sizeof(TC_NS_SMC_CMD), GFP_KERNEL);
    if(!smc_cmd){
        TCERR("smc_cmd malloc failed");
        ret = -ENOMEM;
        goto error;
    }

    if(client_context->paramTypes != 0){
        operation = (TC_NS_Operation *)kzalloc(sizeof(TC_NS_Operation), GFP_KERNEL);
        if(!operation){
            TCERR("login malloc failed");
            ret = -ENOMEM;
            goto error;
        }
    }

    //check if operation is empty
    if(operation){
        for(i = 0; i < 4; i++){
            client_param = &(client_context->params[i]);
            param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
            //if buffer is not null
            //temp buffer
            if((param_type == TEEC_MEMREF_TEMP_INPUT) ||
                (param_type == TEEC_MEMREF_TEMP_OUTPUT)||
                (param_type  == TEEC_MEMREF_TEMP_INOUT)){
                temp_buf = (void *)kzalloc(*(unsigned int *)client_param->memref.size_addr, GFP_KERNEL);
                if(!temp_buf){
                    TCERR("temp_buf malloc failed");
                    ret = -ENOMEM;
                    goto error;
                }

                
                if((param_type == TEEC_MEMREF_TEMP_INPUT) ||
                    (param_type  == TEEC_MEMREF_TEMP_INOUT)){
                    //printk("client_param->memref.buffer = 0x%p\n", client_param->memref.buffer);
                    {
                        memcpy(temp_buf, (void*)client_param->memref.buffer,
                            *(unsigned int *)client_param->memref.size_addr);
                    }
                }
                operation->params[i].memref.buffer = virt_to_phys((void *)temp_buf);
                local_temp_buffer[i] = temp_buf;
                //local_temp_buf[i] = buf;
                operation->params[i].memref.size = *(unsigned int *)client_param->memref.size_addr;
                trans_paramtype_to_tee[i] = param_type; //TEEC_MEMREF_TEMP_INPUT equal to TEE_PARAM_TYPE_MEMREF_INPUT
            }
            //register buffer
            else if((param_type == TEEC_MEMREF_PARTIAL_INPUT)||
                (param_type == TEEC_MEMREF_PARTIAL_OUTPUT)||
                (param_type == TEEC_MEMREF_PARTIAL_INOUT)){
                //find kernel addr refered to user addr
                list_for_each_entry(shared_mem, &dev_file->shared_mem_list, head){
                    if(shared_mem->user_addr == (void*)client_param->memref.buffer){
                        operation->params[i].memref.buffer =
                            virt_to_phys((void *)shared_mem->kernel_addr + client_param->memref.offset);
                        break;
                    }
                }
                if(!operation->params[i].memref.buffer){
                    TCERR("can not find shared buffer, exit\n");
                    ret = -EINVAL;
                    goto error;
                }
                operation->params[i].memref.size = *(unsigned int *)client_param->memref.size_addr;
                trans_paramtype_to_tee[i] = param_type - 8; //TEEC_MEMREF_PARTIAL_INPUT - TEE_PARAM_TYPE_MEMREF_INPUT = 8
            }
            //value
            else if((param_type == TEEC_VALUE_INPUT)||
                (param_type == TEEC_VALUE_OUTPUT)||
                (param_type == TEEC_VALUE_INOUT)){
                operation->params[i].value.a = *(u32 *)client_param->value.a_addr;
                operation->params[i].value.b = *(u32 *)client_param->value.b_addr;
                trans_paramtype_to_tee[i] = param_type; //TEEC_VALUE_INPUT equal to TEE_PARAM_TYPE_VALUE_INPUT
            }
            else if(param_type == TEEC_DX_SHARE_MEM){
                //find kernel addr refered to user addr
                list_for_each_entry(shared_mem, &dev_file->shared_mem_list, head){
                    if(shared_mem->user_addr == (void*)client_param->memref.buffer){
                        operation->params[i].sharememref.handle1 =
                            (void*)virt_to_phys((void *)shared_mem->kernel_addr);
                        break;
                    }
                }
                if(!operation->params[i].sharememref.handle1){
                    TCERR("can not find shared buffer, exit\n");
                    ret = -EINVAL;
                    goto error;
                }

                operation->params[i].sharememref.offset = client_param->memref.offset;
                operation->params[i].sharememref.size = *(unsigned int *)client_param->memref.size_addr;
                trans_paramtype_to_tee[i] = param_type;
                printk("operation->params[%d].sharememref.handle1 = 0x%x\n", i, (unsigned int)operation->params[i].sharememref.handle1);
            }
            else{
                TCDEBUG("param_type = TEEC_NONE\n");
                trans_paramtype_to_tee[i] = TEE_PARAM_TYPE_NONE;
            }
        }
    }
    uuid[0] = ((is_global == true ) ? 1 : 0);
    memcpy(uuid + 1, client_context->uuid, 16);
    smc_cmd->uuid_phys = virt_to_phys((void*)uuid);
    smc_cmd->cmd_id = client_context->cmd_id;
    smc_cmd->dev_file_id=dev_file->dev_file_id;
    smc_cmd->context_id = client_context->session_id;
    smc_cmd->err_origin = 0;

    if(operation) {
        operation->paramTypes = TEEC_PARAM_TYPES(
            trans_paramtype_to_tee[0],
            trans_paramtype_to_tee[1],
            trans_paramtype_to_tee[2],
            trans_paramtype_to_tee[3]);
        smc_cmd->operation_phys = virt_to_phys(operation);
    } else
        smc_cmd->operation_phys = 0;

    smc_cmd->login_method = client_context->login.method;
    smc_cmd->login_data = client_context->login.mdata;

    smc_cmd_phys = virt_to_phys((void*)smc_cmd);
    //send smc to secure world
    mutex_lock(&smc_lock);
    //flush_cache_all();
    tee_ret = TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

    client_context->session_id = smc_cmd->context_id;

    if(tee_ret != 0){
        while(tee_ret == TEEC_PENDING2 || tee_ret == TEEC_PENDING){
            if(tee_ret == TEEC_PENDING2){
                unsigned int agent_id = smc_cmd->agent_id;

                tee_ret = TC_NS_incomplete_proceed(smc_cmd, client_context->session_id, agent_id);
            }
#ifdef  TC_ASYNC_NOTIFY_SUPPORT
            else if(tee_ret == TEEC_PENDING){
                mutex_lock(&dev_file->service_lock);
                list_for_each_entry(service, &dev_file->services_list, head){
                    if(memcmp(service->uuid, client_context->uuid, 16) == 0){
                        list_for_each_entry(session,&service->session_list,head){
                            if(session->session_id == client_context->session_id){
                                printk("come into wait data\n");
                                wq=&session->wait_data;
                                break;
                            }
                        }
                        break;
                    }

                }
                mutex_unlock(&dev_file->service_lock);

                if(wq){
                    printk("before wait event \n");
                    if(wait_event_interruptible(wq->send_cmd_wq,
                        wq->send_wait_flag)) {
                        ret = -ERESTARTSYS;
                        printk("wait event error\n");
                        goto error;
                    }
                    wq->send_wait_flag = 0;
                }

                uuid[0] = ((is_global == true ) ? 1 : 0);
                memcpy(uuid + 1, client_context->uuid, 16);
                smc_cmd->uuid_phys = virt_to_phys((void*)uuid);
                smc_cmd->cmd_id = client_context->cmd_id;
                smc_cmd->dev_file_id=dev_file->dev_file_id;
                smc_cmd->context_id = client_context->session_id;
                smc_cmd_phys=virt_to_phys((void*)smc_cmd);

                //printk("service id:0x%x ,cmd id:0x%x  session_id:0x%x\n",notify_data->service_id,cmd_id,notify_data->session_id);

                mutex_lock(&smc_lock);
                tee_ret = TC_NS_SMC(smc_cmd_phys);
                mutex_unlock(&smc_lock);
            }
#endif
        }
        if(tee_ret != 0){
            goto error1;
        }
    }

    if(operation){
        for(i = 0; i < 4; i++){
            client_param = &(client_context->params[i]);
            param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
            if((param_type ==  TEEC_MEMREF_TEMP_OUTPUT)||
                (param_type  == TEEC_MEMREF_TEMP_INOUT)){
                //temp buffer
                {
                   memcpy((void*)client_param->memref.buffer,
                        local_temp_buffer[i], operation->params[i].memref.size);
                }
                /* update size */
                *(unsigned int *)client_param->memref.size_addr = operation->params[i].memref.size;
            } else if((param_type ==  TEEC_MEMREF_PARTIAL_OUTPUT)||
                (param_type  == TEEC_MEMREF_PARTIAL_INOUT)){
                //share memory
                /* update size */
                *(unsigned int *)client_param->memref.size_addr = operation->params[i].memref.size;
            } else if((param_type == TEEC_VALUE_OUTPUT)||
                (param_type == TEEC_VALUE_INOUT)){
                //value
                *(u32 *)client_param->value.a_addr = operation->params[i].value.a;
                *(u32 *)client_param->value.b_addr = operation->params[i].value.b;
            }else if(param_type == TEEC_DX_SHARE_MEM){
                /* update size */
                *(unsigned int *)client_param->memref.size_addr = operation->params[i].sharememref.size;
            }
        }
    }

    ret = 0;
    goto error;

error1:
    if (tee_ret == TEEC_ERROR_SHORT_BUFFER) {
        /* update size */
        if(operation){
            for(i = 0; i < 4; i++){
                client_param = &(client_context->params[i]);
                param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, i);
                if((param_type ==  TEEC_MEMREF_TEMP_OUTPUT)||
                    (param_type  == TEEC_MEMREF_TEMP_INOUT)) {
                    //temp buffer
                    //only when teec size >= tee size, we can copy buffer
                    // if teec size < tee size, this is the reason of errno TEEC_ERROR_SHORT_BUFFER, so cannot copy buffer
                    if (*(unsigned int *)client_param->memref.size_addr >= operation->params[i].memref.size)
                    {
                        {
                            memcpy((void*)client_param->memref.buffer,
                            	local_temp_buffer[i], operation->params[i].memref.size);
                        }
                    }
                    *(unsigned int *)client_param->memref.size_addr = operation->params[i].memref.size;
                } else if((param_type ==  TEEC_MEMREF_PARTIAL_OUTPUT)||
                    (param_type  == TEEC_MEMREF_PARTIAL_INOUT)){
                    //if shared memory, just update size
                    *(unsigned int *)client_param->memref.size_addr = operation->params[i].memref.size;
                }
            }
        }
    }

    client_context->returns.code = tee_ret;
    client_context->returns.origin = smc_cmd->err_origin;

    TCERR("session_id(0x%x), cmd_id(0x%x) context smc_call returns error origin(0x%x), ret(0x%x).\n", 
	client_context->session_id, client_context->cmd_id, smc_cmd->err_origin, tee_ret);
    dump_stack();

    ret = EFAULT;

error:
    if(smc_cmd)
        kfree(smc_cmd);
    if(operation){
        for(i = 0; i < 4; i++){
            if(local_temp_buffer[i])
                kfree(local_temp_buffer[i]);
        }
        kfree(operation);
    }

    return ret;
}

#ifdef TC_ASYNC_NOTIFY_SUPPORT
static void ipi_secure_notify( struct pt_regs *regs)
{
    TC_NS_DEV_File *temp_dev_file;
    TC_NS_Service*temp_svc;
    TC_NS_Session*temp_ses;
    int enc_found = 0;

    if(!notify_data)
        return;

    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(temp_dev_file, &tc_ns_dev_list.dev_file_list,head) {
        TCDEBUG("dev file id1 = %d, id2 = %d\n",temp_dev_file->dev_file_id, notify_data->dev_file_id);
        if(temp_dev_file->dev_file_id == notify_data->dev_file_id){
            list_for_each_entry(temp_svc, &temp_dev_file->services_list, head){
                TCDEBUG("uuid1 = 0x%x, uuid2 = 0x%x\n", *(unsigned int*)(temp_svc->uuid), *(unsigned int*)(notify_data->uuid));
                if(memcmp(temp_svc->uuid, notify_data->uuid, 16) == 0) {
                    list_for_each_entry(temp_ses, &temp_svc->session_list, head) {
                        TCDEBUG("session id1 = 0x%x, id2 = 0x%x\n", temp_ses->session_id, notify_data->session_id);
                        if(temp_ses->session_id == notify_data->session_id) {
                            TCDEBUG("send cmd ses id %d \n",temp_ses->session_id);
                            enc_found = 1;
                            break;
                        }
                    }
                    break;
                }

            }
        break;
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    if(enc_found) {
        temp_ses->wait_data.send_wait_flag = 1;
        wake_up_interruptible(&temp_ses->wait_data.send_cmd_wq);
    }

    return;
}
#endif

static int TC_NS_ServiceInit(TC_NS_DEV_File* dev_file, unsigned char* uuid, TC_NS_Service** new_service)
{
    int ret = 0;
    TC_NS_Service* service;
    TC_NS_Service* temp;

    service = (TC_NS_Service*)kzalloc(sizeof(TC_NS_Service), GFP_KERNEL);
    if(!service){
        TCERR("kmalloc failed \n");
        ret = -ENOMEM;
        goto error;
    }

    memcpy(service->uuid, uuid, 16);
    dev_file->service_cnt++;
    INIT_LIST_HEAD(&service->session_list);
    mutex_init(&service->session_lock);
    mutex_lock(&dev_file->service_lock);
    list_add_tail(&service->head, &dev_file->services_list);
    mutex_unlock(&dev_file->service_lock);
    *new_service = service;
    goto clean;

error:
    if (!list_empty(&dev_file->services_list)) {
        mutex_lock(&dev_file->service_lock);
        list_for_each_entry_safe(service, temp, &dev_file->services_list, head){
            list_del(&service->head);
            kfree(service);
        }
        mutex_unlock(&dev_file->service_lock);
    }

clean:
    return ret;
}

#if 0
static int TC_NS_ServiceExit(unsigned int dev_id, unsigned char* uuid)
{
    int ret = TEEC_SUCCESS;
    TC_NS_DEV_File *dev_file;
    TC_NS_Service *service, *service_temp;

    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            //mutex_lock(&dev_file->service_lock);
            list_for_each_entry_safe(service, service_temp, &dev_file->services_list, head){
                if(memcmp(service->uuid, uuid, 16) == 0) {
                    list_del(&service->head);
                    kfree(service);
                    dev_file->service_cnt--;
                    goto service_exit_ok;
                }
            }
            //mutex_unlock(&dev_file->service_lock);
        }
    }
service_exit_ok:
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    return ret;
}
#endif

static int TC_NS_OpenSession(unsigned int dev_id, void* argp)
{
    int ret = -EINVAL;
    TC_NS_DEV_File *dev_file;
    TC_NS_Service *service;
    TC_NS_Session *session;
    TC_NS_ClientContext client_context;
    bool success = false;

    if(copy_from_user(&client_context, argp, sizeof(TC_NS_ClientContext))){
        TCERR("copy from user failed\n");
        ret =  -ENOMEM;
        return ret;
    }

    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            //need service init or not
            list_for_each_entry(service, &dev_file->services_list, head){
                if(memcmp(service->uuid, client_context.uuid, 16) == 0){
                    goto find_service;
                }
            }
            //create a new service if we couldn't find it in list
            ret = TC_NS_ServiceInit(dev_file, client_context.uuid, &service);
            if(ret){
                TCERR("service init failed");
                break;
            }
            else
                goto find_service;
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);
    TCERR("find service failed");
    return ret;

find_service:
    mutex_unlock(&tc_ns_dev_list.dev_lock);
    session = (TC_NS_Session *)kzalloc(sizeof(TC_NS_Session), GFP_KERNEL);
    if(!session) {
        TCERR("kmalloc failed\n");
        ret =  -ENOMEM;
        return ret;
    }

    //send smc
    ret = TC_NS_SMC_Call(&client_context, dev_file, true);

    /*
     * 小于0是系统出错，直接返回负数
     * 大于0是SMC返回错误，需要copy_to_user，再返回正数
     * 等于0是正确，需要copy_to_user，再返回0
     */
    if (ret < 0) {
        goto error;
    } else if (ret > 0) {
        goto error1;
    } else {
        TCDEBUG("smc_call returns right\n");
        success = true;
    }
    session->session_id = client_context.session_id;

#ifdef TC_ASYNC_NOTIFY_SUPPORT
    session->wait_data.send_wait_flag=0;
    init_waitqueue_head(&session->wait_data.send_cmd_wq);
#endif

    mutex_lock(&service->session_lock);
    list_add_tail(&session->head, &service->session_list);
    mutex_unlock(&service->session_lock);

error1:
    if(copy_to_user(argp, &client_context, sizeof(client_context))) {
        TCERR("copy to user failed\n");
        ret =  -ENOMEM;
        if (success) {
            success = false;
            mutex_lock(&service->session_lock);
            list_del(&session->head);
            mutex_unlock(&service->session_lock);
        }
    }
    if (success)
        return ret;
error:
    kfree(session);
    return ret;
}

int TC_NS_OpenSession_from_kernel(unsigned int dev_id, void* argp)
{
    int ret = -EINVAL;
    TC_NS_DEV_File *dev_file;
    TC_NS_Service *service;
    TC_NS_Session *session;
    TC_NS_ClientContext client_context;
    bool success = false;
    memcpy(&client_context, argp, sizeof(TC_NS_ClientContext));
#if 0
    if(copy_from_user(&client_context, argp, sizeof(TC_NS_ClientContext))){
        TCERR("copy from user failed\n");
        ret =  -ENOMEM;
        return ret;
    }
#endif
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            list_for_each_entry(service, &dev_file->services_list, head){
                if(memcmp(service->uuid, client_context.uuid, 16) == 0){
                    goto find_service;
                }
            }
            ret = TC_NS_ServiceInit(dev_file, client_context.uuid, &service);
            if(ret){
                TCERR("service init failed");
                break;
            }
            else
                goto find_service;
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);
    TCERR("find service failed");
    return ret;

find_service:
    mutex_unlock(&tc_ns_dev_list.dev_lock);
    session = (TC_NS_Session *)kzalloc(sizeof(TC_NS_Session), GFP_KERNEL);
    if(!session) {
        TCERR("kmalloc failed\n");
        ret =  -ENOMEM;
        return ret;
    }

    //send smc
    ret = TC_NS_SMC_Call_from_kernel(&client_context, dev_file, true);

    /*
     * 小于0是系统出错，直接返回负数
     * 大于0是SMC返回错误，需要copy_to_user，再返回正数
     * 等于0是正确，需要copy_to_user，再返回0
     */
    if (ret < 0) {
        goto error;
    } else if (ret > 0) {
        goto error1;
    } else {
        TCDEBUG("smc_call returns right\n");
        success = true;
    }
    session->session_id = client_context.session_id;

#ifdef TC_ASYNC_NOTIFY_SUPPORT
    session->wait_data.send_wait_flag=0;
    init_waitqueue_head(&session->wait_data.send_cmd_wq);
#endif

    mutex_lock(&service->session_lock);
    list_add_tail(&session->head, &service->session_list);
    mutex_unlock(&service->session_lock);

error1:
    memcpy(argp, &client_context, sizeof(client_context));
    /*if(copy_to_user(argp, &client_context, sizeof(client_context))) {
        TCERR("copy to user failed\n");
        ret =  -ENOMEM;*/
       /* if (success) {
            success = false;
            mutex_lock(&service->session_lock);
            list_del(&session->head);
            mutex_unlock(&service->session_lock);
        }
    //}*/
    if (success)
        return ret;
error:
    kfree(session);
    return ret;
}

static int TC_NS_CloseSession(unsigned int dev_id, void* argp)
{
    int ret = -EINVAL;
    TC_NS_DEV_File *dev_file;
    TC_NS_Service *service;
    TC_NS_Session *session, *session_temp;
    TC_NS_ClientContext client_context;

    if(copy_from_user(&client_context, argp, sizeof(TC_NS_ClientContext))){
        TCERR("copy from user failed\n");
        ret =  -ENOMEM;
        return ret;
    }
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            list_for_each_entry(service, &dev_file->services_list, head){
                if(memcmp(service->uuid, client_context.uuid, 16) == 0){
                    //mutex_lock(&service->session_lock);
                    list_for_each_entry_safe(session, session_temp, &service->session_list, head){
                        if(session->session_id == client_context.session_id){
                            ret = TC_NS_SMC_Call(&client_context, dev_file, true);
                            if (ret == 0) {
                                TCDEBUG("close session smc success");
                                list_del(&session->head);
                                kfree(session);
                            } else {
                                TCERR("close session smc failed");
                            }
                        }
                    }
                    //mutex_unlock(&service->session_lock);
                }
            }
        }
    }
#if 0
/*DTS2013040705065 q00209673 begin for lock*/
session_close_fail:
    if(ret == TEEC_SUCCESS)
        ret = TEEC_ERROR_GENERIC;
    TCDEBUG("close session failed\n");
/*DTS2013040705065 q00209673 end for lock*/
#endif
    mutex_unlock(&tc_ns_dev_list.dev_lock);
    return ret;
}

int TC_NS_CloseSession_from_kernel(unsigned int dev_id, void* argp)
{
    int ret = -EINVAL;
    TC_NS_DEV_File *dev_file;
    TC_NS_Service *service;
    TC_NS_Session *session, *session_temp;
    TC_NS_ClientContext client_context;

    memcpy(&client_context, argp, sizeof(TC_NS_ClientContext));
    #if 0
    if(copy_from_user(&client_context, argp, sizeof(TC_NS_ClientContext))){
        TCERR("copy from user failed\n");
        ret =  -ENOMEM;
        return ret;
    }
    #endif

    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            list_for_each_entry(service, &dev_file->services_list, head){
                if(memcmp(service->uuid, client_context.uuid, 16) == 0){
                    //mutex_lock(&service->session_lock);
                    list_for_each_entry_safe(session, session_temp, &service->session_list, head){
                        if(session->session_id == client_context.session_id){
                            ret = TC_NS_SMC_Call_from_kernel(&client_context, dev_file, true);
                            if (ret == 0) {
                                TCDEBUG("close session smc success");
                                list_del(&session->head);
                                kfree(session);
                            } else {
                                TCERR("close session smc failed");
                            }
                        }
                    }
                    //mutex_unlock(&service->session_lock);
                }
            }
        }
    }
#if 0
/*DTS2013040705065 q00209673 begin for lock*/
session_close_fail:
    if(ret == TEEC_SUCCESS)
        ret = TEEC_ERROR_GENERIC;
    TCDEBUG("close session failed\n");
/*DTS2013040705065 q00209673 end for lock*/
#endif
    mutex_unlock(&tc_ns_dev_list.dev_lock);
    return ret;
}

static int TC_NS_Send_CMD(unsigned int dev_id, void* argp)
{
    int ret = -EINVAL;
    TC_NS_DEV_File *dev_file;
    TC_NS_Service *service;
    TC_NS_Session *session;
    TC_NS_ClientContext client_context;

    if(copy_from_user(&client_context, argp, sizeof(TC_NS_ClientContext))){
        TCERR("copy from user failed\n");
        ret =  -ENOMEM;
        return ret;
    }
    //check sessionid is validated or not
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            list_for_each_entry(service, &dev_file->services_list, head){
                if(memcmp(service->uuid, client_context.uuid, 16) == 0){
                    list_for_each_entry(session, &service->session_list, head){
                        if(session->session_id == client_context.session_id){
                            TCDEBUG("send cmd find session id %d\n", client_context.session_id);
                            goto find_session;
                        }
                    }
                }
            }
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);
    TCERR("send cmd can not find session id %d\n", client_context.session_id);
    return ret;

find_session:
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    //printk("before send smc call to non_secure in TC_NS_SEND_CMD\n");
    //send smc
    ret = TC_NS_SMC_Call(&client_context, dev_file, false);

    //printk("end of send smc call to non_secure in tc_ns send _cmd\n");

    /*
     * 小于0是系统出错，可以直接返回，
     * 大于0是SMC返回错误，需要copy_to_user
     * 等于0是正确，需要copy_to_user
     */
    if (ret < 0) {
        goto error;
    } else if (ret > 0) {
    } else {
        TCDEBUG("smc_call returns right\n");
    }

    if(copy_to_user(argp, &client_context, sizeof(client_context))) {
        TCERR("copy to user failed\n");
        ret =  -ENOMEM;
    }
error:
    return ret;
}

static int TC_NS_Send_CMD_from_kernel(unsigned int dev_id, void* argp)
{
    int ret = -EINVAL;
    TC_NS_DEV_File *dev_file;
    TC_NS_Service *service;
    TC_NS_Session *session;
    TC_NS_ClientContext client_context;

    memcpy(&client_context, argp, sizeof(TC_NS_ClientContext));
    #if 0
    if(copy_from_user(&client_context, argp, sizeof(TC_NS_ClientContext))){
        TCERR("copy from user failed\n");
        ret =  -ENOMEM;
        return ret;
    }
    #endif
    //check sessionid is validated or not
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            list_for_each_entry(service, &dev_file->services_list, head){
                if(memcmp(service->uuid, client_context.uuid, 16) == 0){
                    list_for_each_entry(session, &service->session_list, head){
                        if(session->session_id == client_context.session_id){
                            TCDEBUG("send cmd find session id %x\n", client_context.session_id);
                            goto find_session;
                        }
                    }
                }
            }
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);
    TCERR("send cmd can not find session id %d\n", client_context.session_id);
    return ret;

find_session:
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    //printk("before send smc call to non_secure in TC_NS_SEND_CMD\n");
    //send smc
    ret = TC_NS_SMC_Call_from_kernel(&client_context, dev_file, false);

    //printk("end of send smc call to non_secure in tc_ns send _cmd\n");

    /*
     * 小于0是系统出错，可以直接返回，
     * 大于0是SMC返回错误，需要copy_to_user
     * 等于0是正确，需要copy_to_user
     */
    if (ret < 0) {
        goto error;
    } else if (ret > 0) {
    } else {
        TCDEBUG("smc_call returns right\n");
    }
    memcpy(argp, &client_context,sizeof(client_context));
error:
    return ret;
}

static int TC_NS_SharedMemoryRelease(unsigned int dev_id, void* argp)
{
    int ret = TEEC_SUCCESS;
    TC_NS_Shared_MEM *shared_mem, *shared_mem_temp;
    TC_NS_DEV_File *dev_file;
    unsigned int user_addr = (unsigned int)argp;

    //printk("user_addr = %08x\n", user_addr);
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            //mutex_lock(&dev_file->shared_mem_lock);
            list_for_each_entry_safe(shared_mem, shared_mem_temp, &dev_file->shared_mem_list, head){
                if(shared_mem->user_addr == (void*)user_addr){
                    list_del(&shared_mem->head);
                    if(shared_mem->kernel_addr){
                        free_pages((unsigned int)shared_mem->kernel_addr,
                            get_order(ROUND_UP(shared_mem->len, SZ_4K)));
                    if(shared_mem)
                        kfree(shared_mem);
                    }
                    dev_file->shared_mem_cnt--;
                    break;
                }
            }
            //mutex_unlock(&dev_file->shared_mem_lock);
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    return ret;
}

static struct __smc_event_data* find_event_control(unsigned long agent_id)
{
    struct __smc_event_data *event_data, *tmp_data=NULL;
    unsigned long flags;

    spin_lock_irqsave(&agent_control.lock, flags);
    list_for_each_entry(event_data, &agent_control.agent_list, head){
        if(event_data->agent_id == agent_id){
            tmp_data = event_data;
            break;
        }
    }
    spin_unlock_irqrestore(&agent_control.lock, flags);

    return tmp_data;
}

static int TC_NS_wait_event(unsigned long agent_id)
{
    int ret=-1;
    struct __smc_event_data *event_data=find_event_control(agent_id);

    if(event_data){
        ret = wait_event_freezable(event_data->wait_event_wq, event_data->ret_flag);
        event_data->ret_flag = 0;
    }

    return ret;
}

static int TC_NS_send_event_reponse(unsigned long agent_id)
{
    struct __smc_event_data *event_data=find_event_control(agent_id);

    if(event_data){
        event_data->send_flag = 1;
        wake_up_interruptible(&(event_data->send_response_wq));
    }

    return 0;
}

static int TC_NS_register_agent(unsigned int dev_id, unsigned int agent_id)
{
    TC_NS_Shared_MEM* shared_mem=NULL, *tmp_mem=NULL;
    TC_NS_DEV_File* dev_file;
    TC_NS_SMC_CMD smc_cmd = {0};
    unsigned int smc_cmd_phys;
    struct __smc_event_data *event_data=NULL;
    int ret;
    int find_flag = 0;
    unsigned long flags;
    unsigned char uuid[17] = {0};
    TC_NS_Operation operation = {0};

    TCDEBUG(KERN_INFO"register agent dev_id = %d\n", dev_id);

    spin_lock_irqsave(&agent_control.lock, flags);
    list_for_each_entry(event_data, &agent_control.agent_list, head){
        if(event_data->agent_id == agent_id){
            find_flag = 1;
            break;
        }
    }
    spin_unlock_irqrestore(&agent_control.lock, flags);

    if(find_flag){
        printk(KERN_WARNING"agent is exist\n");
        ret = TEEC_ERROR_GENERIC;
        goto error;
    }

    //find sharedmem
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            list_for_each_entry(tmp_mem, &dev_file->shared_mem_list, head){
                if(tmp_mem){
                    shared_mem = tmp_mem;
                    break;
                }
            }
            break;
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    if(!shared_mem){
        printk(KERN_WARNING"shared mem is not exist\n");
        ret = TEEC_ERROR_GENERIC;
        goto error;
    }

    operation.paramTypes = TEE_PARAM_TYPE_VALUE_INPUT;
    operation.paramTypes = operation.paramTypes << 12;
    operation.params[0].value.a = virt_to_phys(shared_mem->kernel_addr);
    operation.params[0].value.b = SZ_4K;

    //mmap shared mem in secure_os
    //memset(&smc_cmd, 0, sizeof(TC_NS_SMC_CMD));
    uuid[0] = 1;
    smc_cmd.uuid_phys = virt_to_phys(uuid);
    smc_cmd.cmd_id = GLOBAL_CMD_ID_REGISTER_AGENT;
    smc_cmd.operation_phys = virt_to_phys(&operation);
    smc_cmd.agent_id = agent_id;

    smc_cmd_phys = virt_to_phys(&smc_cmd);

    mutex_lock(&smc_lock);
    ret = TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

    if(ret == TEEC_SUCCESS){
        event_data = NULL;
        event_data = kmalloc(sizeof(struct __smc_event_data), GFP_KERNEL);
        if(!event_data){
            ret = -ENOMEM;
        }
        event_data->agent_id = agent_id;
        mutex_init(&event_data->work_lock);
        event_data->ret_flag = 0;
        event_data->send_flag = 0;
        event_data->buffer = shared_mem->kernel_addr;
        init_waitqueue_head(&(event_data->wait_event_wq));
        init_waitqueue_head(&(event_data->send_response_wq));
        INIT_LIST_HEAD(&event_data->head);

        spin_lock_irqsave(&agent_control.lock, flags);
        list_add_tail(&event_data->head, &agent_control.agent_list);
        spin_unlock_irqrestore(&agent_control.lock, flags);
    }

error:
    return ret;
}

static int TC_NS_unregister_agent(unsigned long agent_id)
{
    struct __smc_event_data *event_data=NULL;
    int ret;
    int find_flag = 0;
    unsigned long flags;
    TC_NS_SMC_CMD smc_cmd = {0};
    unsigned int smc_cmd_phys;
    unsigned char uuid[17] = {0};
    TC_NS_Operation operation = {0};

    spin_lock_irqsave(&agent_control.lock, flags);
    list_for_each_entry(event_data, &agent_control.agent_list, head){
        if(event_data->agent_id == agent_id){
            find_flag = 1;
            list_del(&event_data->head);
            break;
        }
    }
    spin_unlock_irqrestore(&agent_control.lock, flags);

    if(!find_flag){
        TCERR(KERN_WARNING"agent is not found\n");
        return TEEC_ERROR_GENERIC;
    }

    operation.paramTypes = TEE_PARAM_TYPE_VALUE_INPUT;
    operation.paramTypes = operation.paramTypes << 12;
    operation.params[0].value.a= virt_to_phys(event_data->buffer);
    operation.params[0].value.b = SZ_4K;

    //memset(&smc_cmd, 0, sizeof(TC_NS_SMC_CMD));
    uuid[0] = 1;
    smc_cmd.uuid_phys = virt_to_phys(uuid);
    smc_cmd.cmd_id = GLOBAL_CMD_ID_UNREGISTER_AGENT;
    smc_cmd.operation_phys = virt_to_phys(&operation);
    smc_cmd.agent_id = agent_id;

    smc_cmd_phys = virt_to_phys(&smc_cmd);

    mutex_lock(&smc_lock);
    ret = TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

    kfree(event_data);

    return ret;
}

#ifdef TC_ASYNC_NOTIFY_SUPPORT
static int TC_NS_register_notify_data_memery()
{

    TC_NS_SMC_CMD smc_cmd = {0};
    unsigned int smc_cmd_phys;
    int ret;
    unsigned char uuid[17] = {0};
    TC_NS_Operation operation = {0};

    operation.paramTypes = TEE_PARAM_TYPE_VALUE_INPUT;
    operation.paramTypes = operation.paramTypes << 12;
    operation.params[0].value.a= virt_to_phys(notify_data);
    operation.params[0].value.b = sizeof(struct tc_notify_data);

    //mmap shared mem in secure_os
    //memset(&smc_cmd, 0, sizeof(TC_NS_SMC_CMD));
    uuid[0] = 1;
    smc_cmd.uuid_phys = virt_to_phys((void*)uuid);
    smc_cmd.cmd_id = GLOBAL_CMD_ID_REGISTER_NOTIFY_MEMORY;
    smc_cmd.operation_phys = virt_to_phys(&operation);

    smc_cmd_phys = virt_to_phys(&smc_cmd);
    TCDEBUG("cmd. context_phys:%x\n",smc_cmd.context_id);
    mutex_lock(&smc_lock);
    ret = TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

error:
    return ret;
}

static int TC_NS_unregister_notify_data_memery()
{

    TC_NS_SMC_CMD smc_cmd = {0};
    unsigned int smc_cmd_phys;
    int ret;
    unsigned char uuid[17] = {0};
    TC_NS_Operation operation = {0};

    operation.paramTypes = TEE_PARAM_TYPE_VALUE_INPUT;
    operation.paramTypes = operation.paramTypes << 12;
    operation.params[0].value.a= virt_to_phys(notify_data);
    operation.params[0].value.b = sizeof(struct tc_notify_data);

    //mmap shared mem in secure_os
    //memset(&smc_cmd, 0, sizeof(TC_NS_SMC_CMD));
    uuid[0] = 1;
    smc_cmd.uuid_phys = virt_to_phys((void*)uuid);
    smc_cmd.cmd_id = GLOBAL_CMD_ID_UNREGISTER_NOTIFY_MEMORY;
    smc_cmd.operation_phys = virt_to_phys(&operation);
    TCDEBUG("cmd. context_phys:%x\n",smc_cmd.context_id);

    smc_cmd_phys = virt_to_phys(&smc_cmd);

    mutex_lock(&smc_lock);
    ret = TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

error:
    return ret;
}
#endif

static int TC_NS_load_image_operation(TC_NS_ClientContext *client_context,
        TC_NS_Operation *operation, TC_NS_DEV_File *dev_file)
{
    int index;
    int ret = 1;
    TC_NS_Shared_MEM *shared_mem = NULL;
    TC_NS_ClientParam *client_param = NULL;
    unsigned int param_type;
    unsigned int trans_paramtype_to_tee[4];

    for (index = 0; index < 4; index++) {
        client_param = &(client_context->params[index]);
        param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, index);
        operation->params[index].memref.buffer = (unsigned int)NULL;

        /* TCDEBUG("--->param type is %d\n", param_type); */
        if (param_type == TEEC_MEMREF_PARTIAL_INOUT) {
            list_for_each_entry(shared_mem, &dev_file->shared_mem_list, head) {
                if (shared_mem->user_addr == (void*)client_param->memref.buffer) {
                    operation->params[index].memref.buffer =
                        virt_to_phys((void *)shared_mem->kernel_addr + client_param->memref.offset);
                    /* TCDEBUG("---:memref buff: 0x%x\n", operation->params[index].memref.buffer); */
                    break;
                }
            }
            if (!operation->params[index].memref.buffer) {
                TCERR("can not find shared buffer, exit\n");
                ret = -1;
                return ret;
            }
            operation->params[index].memref.size = *(unsigned int *)client_param->memref.size_addr;
        }

        //translate paramTypes from TEEC to TEE
        if((param_type == TEEC_MEMREF_TEMP_INPUT) ||
            (param_type == TEEC_MEMREF_TEMP_OUTPUT)||
            (param_type  == TEEC_MEMREF_TEMP_INOUT)){
            trans_paramtype_to_tee[index] = param_type; //TEEC_MEMREF_TEMP_INPUT equal to TEE_PARAM_TYPE_MEMREF_INPUT
        } else if((param_type == TEEC_MEMREF_PARTIAL_INPUT)||
            (param_type == TEEC_MEMREF_PARTIAL_OUTPUT)||
            (param_type == TEEC_MEMREF_PARTIAL_INOUT)){
            trans_paramtype_to_tee[index] = param_type - 8; //TEEC_MEMREF_PARTIAL_INPUT - TEE_PARAM_TYPE_MEMREF_INPUT = 8
        } else if((param_type == TEEC_VALUE_INPUT)||
            (param_type == TEEC_VALUE_OUTPUT)||
            (param_type == TEEC_VALUE_INOUT)){
            trans_paramtype_to_tee[index] = param_type; //TEEC_VALUE_INPUT equal to TEE_PARAM_TYPE_VALUE_INPUT
        } else{
            TCDEBUG("param_type = TEEC_NONE\n");
            trans_paramtype_to_tee[index] = TEE_PARAM_TYPE_NONE;
        }
    }

    operation->paramTypes = TEEC_PARAM_TYPES(
        trans_paramtype_to_tee[0],
        trans_paramtype_to_tee[1],
        trans_paramtype_to_tee[2],
        trans_paramtype_to_tee[3]);

    return ret;
}

static int TC_NS_load_image(unsigned int dev_id, void* argp)
{
    int ret;
    unsigned int smc_cmd_phys;
    TC_NS_SMC_CMD *smc_cmd = NULL;
    TC_NS_Operation *operation = NULL;
    TC_NS_ClientContext client_context;
    TC_NS_DEV_File *dev_file;
    int load_flag;
    unsigned char uuid[17] = {0};

    if(copy_from_user(&client_context, argp, sizeof(TC_NS_ClientContext))){
        TCERR("copy from user failed\n");
        ret =  -ENOMEM;
        return ret;
    }

    smc_cmd = (TC_NS_SMC_CMD *)kzalloc(sizeof(TC_NS_SMC_CMD), GFP_KERNEL);
    if(!smc_cmd){
        TCERR("smc_cmd malloc failed");
        ret = -ENOMEM;
        return ret;
    }

    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == dev_id){
            break;
        }
    }
    if (dev_file->dev_file_id != dev_id) {
        TCERR("dev file id erro\n");
        ret = IMG_LOAD_FIND_NO_DEV_ID;
        return ret;
    }

    /* load image operation */

    operation = (TC_NS_Operation *)kzalloc(sizeof(TC_NS_Operation), GFP_KERNEL);
    if(!operation){
        TCERR("login malloc failed");
        ret = -ENOMEM;
        goto operation_erro;
    }

    ret = TC_NS_load_image_operation(&client_context, operation, dev_file);
    if (ret < 0){
        ret = IMG_LOAD_FIND_NO_SHARE_MEM;
        goto buf_erro;
    }

    /* load image smc command */
    TCDEBUG("smc cmd id %d\n", client_context.cmd_id);
    smc_cmd->cmd_id = client_context.cmd_id;

    uuid[0] = 1;
    smc_cmd->uuid_phys = virt_to_phys((void*)uuid);
    smc_cmd->dev_file_id = dev_file->dev_file_id;
    smc_cmd->context_id = 0;
    smc_cmd->operation_phys = virt_to_phys(operation);
    //smc_cmd->login_phys = 0;
    smc_cmd_phys = virt_to_phys((void*)smc_cmd);

    TCDEBUG("secure app load smc command\n");
//    printk("operation phys 0x%x\n", smc_cmd->operation_phys);
    //printk("before load smc_cmd=0x%x, uuid=0x%x, smc_cmd->operation_phys=0x%x\n",smc_cmd_phys,smc_cmd->uuid_phys,smc_cmd->operation_phys);
    mutex_lock(&smc_lock);
    ret = TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

    if(ret != 0){
        TCERR("smc_call returns error ret 0x%x\n",ret);
        ret = IMG_LOAD_SECURE_RET_ERROR;
    }

    /* get ret flag in share memory buffer */
    load_flag = *((int*)client_context.params[0].memref.buffer);
    TCDEBUG("load flag is %d, buffer %x\n", load_flag, client_context.params[0].memref.buffer);
    if (load_flag != 1) {
        /* no need to continue load, need load again, load flag will be 1 */
        TCDEBUG("no need to continue load cmd\n");
        load_app_lock_flag = 0;
    }

    /* error: */
buf_erro:
    kfree(operation);
operation_erro:
    kfree(smc_cmd);

    return ret;
}

static int TC_NS_alloc_exception_mem(unsigned int dev_id, unsigned int agent_id)
{
    TC_NS_SMC_CMD smc_cmd = {0};
    unsigned int smc_cmd_phys;
    int ret;
    unsigned char uuid[17] = {0};
    void *exception_mem;
    TC_NS_Operation operation = {0};

    //TCDEBUG(KERN_INFO"register agent dev_id = %d\n", dev_id);
    exception_mem = (void *)kzalloc(EXCEPTION_MEM_SIZE, GFP_KERNEL);
    exception_mem_addr = (unsigned int)exception_mem;

    TCDEBUG(KERN_INFO"exception_mem virt addr = 0x%x, phys addr = 0x%x\n", exception_mem, virt_to_phys(exception_mem));

    //memset(&smc_cmd, 0, sizeof(TC_NS_SMC_CMD));
    uuid[0] = 1;
    smc_cmd.uuid_phys = virt_to_phys(uuid);
    smc_cmd.cmd_id = GLOBAL_CMD_ID_ALLOC_EXCEPTION_MEM;

    operation.paramTypes = TEE_PARAM_TYPE_VALUE_INPUT;
    operation.paramTypes = operation.paramTypes << 12;
    operation.params[0].value.a= virt_to_phys(exception_mem);
    operation.params[0].value.b = EXCEPTION_MEM_SIZE;

    smc_cmd.operation_phys = virt_to_phys(&operation);

    smc_cmd_phys = virt_to_phys(&smc_cmd);

    mutex_lock(&smc_lock);
    ret = TC_NS_SMC(smc_cmd_phys);
    mutex_unlock(&smc_lock);

    return ret;
}


static int TC_NS_store_exception_info(void)
{
    struct file *filep;
    char *p_excp_mem = (char *)exception_mem_addr;
    ssize_t write_len;
    mm_segment_t old_fs;
    loff_t pos = 0;

    /*exception handling, store trustedcore exception info to file*/
    filep=filp_open("/data/exception_log.log",O_CREAT|O_RDWR,0);
    if(IS_ERR(filep))
    {
        TCDEBUG("Failed to filp_open exception_log.log, filep=0x%x\n", filep);
        return -1;
    }

    TCDEBUG("Succeed to filp_open exception_log.log, filep=0x%x\n", filep);

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    write_len = vfs_write(filep, (char __user*)p_excp_mem, EXCEPTION_MEM_SIZE, &pos);
    if (write_len < 0)
    {
        TCDEBUG("Failed to write to exception_log.log\n");
    }
    else
    {
        TCDEBUG("Succeed to Write hello world to exception_log.log, write_len=%d\n", write_len);
    }
    pos = 0;
    //read_len = vfs_read(filep, (char __user*)read_buff, 15, &pos);
    //TCDEBUG("read_buff is %s, read_len=%d\n", read_buff,read_len);

    set_fs(old_fs);
    filp_close(filep,0);
    return 0;
}

static long TC_NS_ClientIoctl(struct file *file, unsigned cmd,
    unsigned long arg)
{
    int ret = TEEC_ERROR_GENERIC;
    void *argp = (void __user *) arg;
    unsigned int dev_file_id = (unsigned int)file->private_data;
    //printk("cmd = %08x, TC_NS_CLIENT_IOCTL_SES_OPEN_REQ = %08x\n", cmd, TC_NS_CLIENT_IOCTL_SES_OPEN_REQ);
    switch(cmd){
        case TC_NS_CLIENT_IOCTL_SES_OPEN_REQ:{
            mutex_lock(&open_session_lock);
            ret = TC_NS_OpenSession(dev_file_id, argp);
            mutex_unlock(&open_session_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ:{
            mutex_lock(&close_session_lock);
            ret = TC_NS_CloseSession(dev_file_id, argp);
            mutex_unlock(&close_session_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_SEND_CMD_REQ:{
            mutex_lock(&send_cmd_lock);
            ret = TC_NS_Send_CMD(dev_file_id, argp);
            mutex_unlock(&send_cmd_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_SHRD_MEM_RELEASE:{
            mutex_lock(&shared_buf_rel_lock);
            ret = TC_NS_SharedMemoryRelease(dev_file_id, argp);
            mutex_unlock(&shared_buf_rel_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_WAIT_EVENT:{
            ret = TC_NS_wait_event(arg);
            break;
        }
        case TC_NS_CLIENT_IOCTL_SEND_EVENT_REPONSE:{
            ret = TC_NS_send_event_reponse(arg);
            break;
        }
        case TC_NS_CLIENT_IOCTL_REGISTER_AGENT:{
            ret = TC_NS_register_agent(dev_file_id, arg);
            break;
        }
        case TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT:{
            ret = TC_NS_unregister_agent(arg);
            break;
        }
        case TC_NS_CLIENT_IOCTL_NEED_LOAD_APP:{
            mutex_lock(&load_app_lock);
            load_app_lock_flag = 1;
            ret = TC_NS_load_image(dev_file_id, argp);
            if (ret != 0)
                load_app_lock_flag = 0;

            if (load_app_lock_flag == 0) {
                /* if no need to load app, or if ret fail, mutex unlock */
                mutex_unlock(&load_app_lock);
            }
            break;
        }
        case TC_NS_CLIENT_IOCTL_LOAD_APP_REQ:{
            ret = TC_NS_load_image(dev_file_id, argp);
            if (ret != 0)
                load_app_lock_flag = 0;

            if (load_app_lock_flag == 0) {
                /* if load app complete, mutex unlock */
                mutex_unlock(&load_app_lock);
            }
            break;
        }
        case TC_NS_CLIENT_IOCTL_LOAD_APP_EXCEPT:{
            ret = TEEC_SUCCESS;
            load_app_lock_flag = 0;
            mutex_unlock(&load_app_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_ALLOC_EXCEPTING_MEM:{
            ret = TC_NS_alloc_exception_mem(dev_file_id, (unsigned int)argp);
            break;
        }
        case TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ:{
            TCDEBUG("come into cancel cmd\n");
            ret = TC_NS_Send_CMD(dev_file_id, argp);
            TCDEBUG("cancel cmd end\n");
            break;
        }
        default:{
            TCERR("invalid cmd!");
            return ret;
        }
    }
    //ret = 0;

    TCDEBUG("TC_NS_ClientIoctl ret = 0x%x\n", ret);
    if (TEE_ERROR_TAGET_DEAD == ret || TEE_ERROR_GT_DEAD == ret)
        (void)TC_NS_store_exception_info();
    return ret;
}



long TC_NS_ClientIoctl_from_kernel(unsigned int dev_id, unsigned cmd,
    unsigned long arg)
{
    int ret = TEEC_ERROR_GENERIC;
    void *argp = (void __user *) arg;
    unsigned int dev_file_id = dev_id;//(unsigned int)file->private_data;
    //printk("cmd = %08x, TC_NS_CLIENT_IOCTL_SES_OPEN_REQ = %08x,send_cmd:%08x\n", cmd, TC_NS_CLIENT_IOCTL_SES_OPEN_REQ ,TC_NS_CLIENT_IOCTL_SEND_CMD_REQ);

    //printk("TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ = %x, TC_NS_CLIENT_IOCTL_SHRD_MEM_RELEASE = %x\n", TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ, TC_NS_CLIENT_IOCTL_SHRD_MEM_RELEASE);

    //printk("TC_NS_CLIENT_IOCTL_NEED_LOAD_APP = %x\n", TC_NS_CLIENT_IOCTL_NEED_LOAD_APP);
    //printk("TC_NS_CLIENT_IOCTL_LOAD_APP_EXCEPT = %x\n", TC_NS_CLIENT_IOCTL_LOAD_APP_EXCEPT);


    switch(cmd){
        case TC_NS_CLIENT_IOCTL_SES_OPEN_REQ:{
            mutex_lock(&open_session_lock);
            ret = TC_NS_OpenSession_from_kernel(dev_file_id, argp);
            mutex_unlock(&open_session_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ:{
            mutex_lock(&close_session_lock);
            ret = TC_NS_CloseSession_from_kernel(dev_file_id, argp);
            mutex_unlock(&close_session_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_SEND_CMD_REQ:{
            mutex_lock(&send_cmd_lock);
            ret = TC_NS_Send_CMD_from_kernel(dev_file_id, argp);
            mutex_unlock(&send_cmd_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_SHRD_MEM_RELEASE:{
            mutex_lock(&shared_buf_rel_lock);
            ret = TC_NS_SharedMemoryRelease(dev_file_id, argp);
            mutex_unlock(&shared_buf_rel_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_WAIT_EVENT:{
            ret = TC_NS_wait_event(arg);
            break;
        }
        case TC_NS_CLIENT_IOCTL_SEND_EVENT_REPONSE:{
            ret = TC_NS_send_event_reponse(arg);
            break;
        }
        case TC_NS_CLIENT_IOCTL_REGISTER_AGENT:{
            ret = TC_NS_register_agent(dev_file_id, arg);
            break;
        }
        case TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT:{
            ret = TC_NS_unregister_agent(arg);
            break;
        }
        case TC_NS_CLIENT_IOCTL_NEED_LOAD_APP:{
            mutex_lock(&load_app_lock);
            load_app_lock_flag = 1;
            ret = TC_NS_load_image(dev_file_id, argp);
            if (ret != 0)
                load_app_lock_flag = 0;

            if (load_app_lock_flag == 0) {
                /* if no need to load app, or if ret fail, mutex unlock */
                mutex_unlock(&load_app_lock);
            }
            break;
        }
        case TC_NS_CLIENT_IOCTL_LOAD_APP_REQ:{
            ret = TC_NS_load_image(dev_file_id, argp);
            if (ret != 0)
                load_app_lock_flag = 0;

            if (load_app_lock_flag == 0) {
                /* if load app complete, mutex unlock */
                mutex_unlock(&load_app_lock);
            }
            break;
        }
        case TC_NS_CLIENT_IOCTL_LOAD_APP_EXCEPT:{
            ret = TEEC_SUCCESS;
            load_app_lock_flag = 0;
            mutex_unlock(&load_app_lock);
            break;
        }
        case TC_NS_CLIENT_IOCTL_ALLOC_EXCEPTING_MEM:{
            ret = TC_NS_alloc_exception_mem(dev_file_id, (unsigned int)argp);
            break;
        }
        case TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ:{
            TCDEBUG("come into cancel cmd\n");
            ret = TC_NS_Send_CMD(dev_file_id, argp);
            TCDEBUG("cancel cmd end\n");
            break;
        }
        default:{
            TCERR("invalid cmd!");
            return ret;
        }
    }
    //ret = 0;

    TCDEBUG("TC_NS_ClientIoctl ret = 0x%x\n", ret);
    if (TEE_ERROR_TAGET_DEAD == ret || TEE_ERROR_GT_DEAD == ret)
        (void)TC_NS_store_exception_info();
    return ret;
}

static int TC_NS_ClientOpen(struct inode *inode, struct file *file)
{
    int ret = TEEC_ERROR_GENERIC;
    TC_NS_DEV_File *dev;

    TCDEBUG("tc_client_open\n");
    dev = (TC_NS_DEV_File *)kzalloc(sizeof(TC_NS_DEV_File), GFP_KERNEL);
    if(!dev){
        TCERR("dev malloc failed");
        return ret;
    }
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_add_tail(&dev->head, &tc_ns_dev_list.dev_file_list);
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    mutex_lock(&device_file_cnt_lock);
    tc_ns_dev_list.dev_file_cnt++;
    dev->dev_file_id = device_file_cnt;
    file->private_data = (void*)device_file_cnt;
    device_file_cnt++;
    mutex_unlock(&device_file_cnt_lock);

    dev->service_cnt = 0;
    INIT_LIST_HEAD(&dev->services_list);

    dev->shared_mem_cnt = 0;
    INIT_LIST_HEAD(&dev->shared_mem_list);

    mutex_init(&dev->service_lock);
    mutex_init(&dev->shared_mem_lock);

#ifdef TC_ASYNC_NOTIFY_SUPPORT
    mutex_lock(&notify_data_lock);
    if(!notify_data){
       notify_data = (struct tc_notify_data*)kmalloc(
                                        sizeof(struct tc_notify_data),
                                        GFP_KERNEL);
        if(!notify_data){
            TCERR("kmalloc failed for notification data\n");
            ret = -ENOMEM;
            return;
        }
         TCDEBUG("tc ns register notify data mem\n");
         ret=TC_NS_register_notify_data_memery();

        if(ret != TEEC_SUCCESS) {
            TCERR("Shared memory registration for \
            secure world notification failed\n");
            return ret;
            }
    }

    mutex_unlock(&notify_data_lock);
#endif

    ret = TEEC_SUCCESS;
    return ret;
}

int TC_NS_ClientOpen_from_kernel(struct inode *inode, struct file *file,unsigned int *dev_id){
    int ret = TEEC_ERROR_GENERIC;
    TC_NS_DEV_File *dev;

    TCDEBUG("tc_client_open\n");
    dev = (TC_NS_DEV_File *)kzalloc(sizeof(TC_NS_DEV_File), GFP_KERNEL);
    if(!dev){
        TCERR("dev malloc failed");
        return ret;
    }
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_add_tail(&dev->head, &tc_ns_dev_list.dev_file_list);
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    mutex_lock(&device_file_cnt_lock);
    tc_ns_dev_list.dev_file_cnt++;
    dev->dev_file_id = device_file_cnt;
    file->private_data = (void*)device_file_cnt;
    *dev_id = device_file_cnt;
    device_file_cnt++;
    mutex_unlock(&device_file_cnt_lock);

    dev->service_cnt = 0;
    INIT_LIST_HEAD(&dev->services_list);

    dev->shared_mem_cnt = 0;
    INIT_LIST_HEAD(&dev->shared_mem_list);

    mutex_init(&dev->service_lock);
    mutex_init(&dev->shared_mem_lock);

#ifdef TC_ASYNC_NOTIFY_SUPPORT
    mutex_lock(&notify_data_lock);
    if(!notify_data){
       notify_data = (struct tc_notify_data*)kmalloc(
                                        sizeof(struct tc_notify_data),
                                        GFP_KERNEL);
        if(!notify_data){
            TCERR("kmalloc failed for notification data\n");
            ret = -ENOMEM;
            return;
        }
         TCDEBUG("tc ns register notify data mem\n");
         ret=TC_NS_register_notify_data_memery();

        if(ret != TEEC_SUCCESS) {
            TCERR("Shared memory registration for \
            secure world notification failed\n");
            return ret;
            }
    }

    mutex_unlock(&notify_data_lock);
#endif

    ret = TEEC_SUCCESS;
    return ret;
}

static void close_session(TC_NS_DEV_File *dev, unsigned char *uuid, unsigned int session_id)
{
    TC_NS_ClientContext context = {{0}};
    int ret;

    memcpy(context.uuid, uuid, sizeof(context.uuid));
    context.session_id = session_id;
    context.cmd_id = GLOBAL_CMD_ID_CLOSE_SESSION;

    ret = TC_NS_SMC_Call(&context, dev, true);
    if(ret)
        printk(KERN_WARNING"close session failed, ret=0x%x\n", ret);
}


static int TC_NS_ClientClose(struct inode *inode, struct file *file)
{
    int ret = TEEC_ERROR_GENERIC;
    TC_NS_DEV_File *dev, *dev_temp;
    TC_NS_Service *service = NULL, *service_temp;
    TC_NS_Shared_MEM *shared_mem = NULL;
    TC_NS_Shared_MEM *shared_mem_temp = NULL;


#ifdef TC_ASYNC_NOTIFY_SUPPORT
	mutex_lock(&notify_data_lock);
	
    if((tc_ns_dev_list.dev_file_cnt-1) ==0){
    TCDEBUG("notify data unregister\n");
    TCDEBUG("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
    ret = TC_NS_unregister_notify_data_memery();

    if(ret != TEEC_SUCCESS) {
        TCERR("Shared memory un-registration for \
        secure world notification failed\n");
    }
    kfree(notify_data);
    notify_data=NULL;
    }
    else{
    TCDEBUG(" there is some notify data for some service\n");
    }
	
	mutex_unlock(&notify_data_lock);
#endif

    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry_safe(dev, dev_temp, &tc_ns_dev_list.dev_file_list, head){
        if(dev->dev_file_id == (unsigned int)file->private_data){
            //TCERR("0000000\n");
            //mutex_lock(&dev->service_lock);
            list_for_each_entry_safe(service, service_temp, &dev->services_list, head){
                //TCERR("0000000\n");
                if(service){
#if 1
                    /* close unclosed session */
                    if(!list_empty(&service->session_list)){
                        TC_NS_Session *session, *tmp_session;
                        list_for_each_entry_safe(session, tmp_session, &service->session_list, head){
                            printk("terminate opened service 0x%x\n", *(uint32_t*)service->uuid);
                            close_session(dev, service->uuid, session->session_id);
                            kfree(session);
                        }
                    }
#endif

                    #if 0
                    ret = TC_NS_ServiceExit(dev->dev_file_id, service->service_id);
                    if(ret){
                        //TCERR("11111111\n");
                        TCERR("service exit error\n");
                        return ret;
                    }
                    #endif
                    list_del(&service->head);
                    kfree(service);
                    dev->service_cnt--;
                }
            }
            //mutex_unlock(&dev->service_lock);

            //TCERR("1111111\n");
            //mutex_lock(&dev->shared_mem_lock);
            list_for_each_entry_safe(shared_mem, shared_mem_temp, &dev->shared_mem_list, head){
                if(shared_mem){
                    list_del(&shared_mem->head);
                    if(shared_mem->kernel_addr)
                        free_pages((unsigned int)shared_mem->kernel_addr,
                            get_order(ROUND_UP(shared_mem->len, SZ_4K)));
                    if(shared_mem)
                        kfree(shared_mem);
                    dev->shared_mem_cnt--;
                }
            }
            //mutex_unlock(&dev->shared_mem_lock);


            /*printk("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
            if(tc_ns_dev_list.dev_file_cnt){
                tc_ns_dev_list.dev_file_cnt--;
                printk("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
                }
            else
                TCERR("dev file list had been empty already");*/


            //TCERR("2222222\n");
            if(dev->service_cnt == 0 && list_empty(&dev->services_list)){
                //TCERR("222222\n");
                //TCDEBUG("tc_ns_client doesn't have any service");

                ret = TEEC_SUCCESS;
                //del dev from the list
                list_del(&dev->head);
                kfree(dev);
                //break;
            }
            else{
                TCERR("There are some service at the device,pls realse them first!");
                //return ret;
            }
                      TCDEBUG("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
            if(tc_ns_dev_list.dev_file_cnt != 0){
                //printk("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
                tc_ns_dev_list.dev_file_cnt--;
                TCDEBUG("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
                }
            else
                TCERR("dev file list had been empty already");
            break;
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);


      // printk("client close end \n");

    //TCERR("3333333\n");
    return ret;
}

int TC_NS_ClientClose_from_kernel(struct inode *inode, struct file *file,unsigned int dev_id)
{
    int ret = TEEC_ERROR_GENERIC;
    TC_NS_DEV_File *dev, *dev_temp;
    TC_NS_Service *service = NULL, *service_temp;
    TC_NS_Shared_MEM *shared_mem = NULL;
    TC_NS_Shared_MEM *shared_mem_temp = NULL;
    file->private_data = (void *)dev_id;

#ifdef TC_ASYNC_NOTIFY_SUPPORT
	mutex_lock(&notify_data_lock);

    if((tc_ns_dev_list.dev_file_cnt-1) ==0){
    TCDEBUG("notify data unregister\n");
    TCDEBUG("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
    ret = TC_NS_unregister_notify_data_memery();

    if(ret != TEEC_SUCCESS) {
        TCERR("Shared memory un-registration for \
        secure world notification failed\n");
    }
    kfree(notify_data);
    notify_data=NULL;
    }
    else{
        TCDEBUG(" there is some notify data for some service\n");
    }

	mutex_unlock(&notify_data_lock);
#endif

    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry_safe(dev, dev_temp, &tc_ns_dev_list.dev_file_list, head){
        if(dev->dev_file_id == (unsigned int)file->private_data){
            //TCERR("0000000\n");
            //mutex_lock(&dev->service_lock);
            list_for_each_entry_safe(service, service_temp, &dev->services_list, head){
                //TCERR("0000000\n");
                if(service){
#if 1
                    /* close unclosed session */
                    if(!list_empty(&service->session_list)){
                        TC_NS_Session *session, *tmp_session;
                        list_for_each_entry_safe(session, tmp_session, &service->session_list, head){
                            printk("terminate opened service 0x%x\n", *(uint32_t*)service->uuid);
                            close_session(dev, service->uuid, session->session_id);
                            kfree(session);
                        }
                    }
#endif
                    #if 0
                    ret = TC_NS_ServiceExit(dev->dev_file_id, service->service_id);
                    if(ret){
                        //TCERR("11111111\n");
                        TCERR("service exit error\n");
                        return ret;
                    }
                    #endif
                    list_del(&service->head);
                    kfree(service);
                    dev->service_cnt--;
                }
            }
            //mutex_unlock(&dev->service_lock);

            //TCERR("1111111\n");
            //mutex_lock(&dev->shared_mem_lock);
            list_for_each_entry_safe(shared_mem, shared_mem_temp, &dev->shared_mem_list, head){
                if(shared_mem){
                    list_del(&shared_mem->head);
                    if(shared_mem->kernel_addr)
                        free_pages((unsigned int)shared_mem->kernel_addr,
                            get_order(ROUND_UP(shared_mem->len, SZ_4K)));
                    if(shared_mem)
                        kfree(shared_mem);
                    dev->shared_mem_cnt--;
                }
            }
            //mutex_unlock(&dev->shared_mem_lock);


            /*printk("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
            if(tc_ns_dev_list.dev_file_cnt){
                tc_ns_dev_list.dev_file_cnt--;
                printk("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
                }
            else
                TCERR("dev file list had been empty already");*/


            //TCERR("2222222\n");
            if(dev->service_cnt == 0 && list_empty(&dev->services_list)){
                //TCERR("222222\n");
                //TCDEBUG("tc_ns_client doesn't have any service");

                ret = TEEC_SUCCESS;
                //del dev from the list
                list_del(&dev->head);
                kfree(dev);
                //break;
            }
            else{
                TCERR("There are some service at the device,pls realse them first!");
                //return ret;
            }
                      TCDEBUG("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
            if(tc_ns_dev_list.dev_file_cnt != 0){
                //printk("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
                tc_ns_dev_list.dev_file_cnt--;
                TCDEBUG("dev list  dev file cnt:%d\n",tc_ns_dev_list.dev_file_cnt);
                }
            else
                TCERR("dev file list had been empty already");
            break;
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);


      // printk("client close end \n");

    //TCERR("3333333\n");
    return ret;
}

static int TC_NS_ClientMalloc(struct file *filp, struct vm_area_struct *vma)
{
    int ret = TEEC_SUCCESS;
    int find_dev = 0;
    TC_NS_DEV_File *dev_file;
    unsigned long len = vma->vm_end - vma->vm_start;
    TC_NS_Shared_MEM *shared_mem;
    void * addr;

    addr =  (void*) __get_free_pages(GFP_KERNEL, get_order(ROUND_UP(len, SZ_4K)));
    if(!addr) {
        TCERR("get free pages failed \n");
        ret = -ENOMEM;
        return ret;
    }

     //printk("addr=0x%x, size=0x%x,virt_to_phys(addr)=0x%x\n",addr, len,virt_to_phys(addr));
    if (remap_pfn_range(vma, vma->vm_start, ((virt_to_phys(addr)) >> PAGE_SHIFT),
                len, vma->vm_page_prot)) {
        ret = -EAGAIN;
        return ret;
    }

    shared_mem = (TC_NS_Shared_MEM *)kmalloc(sizeof(TC_NS_Shared_MEM), GFP_KERNEL);
    if(!shared_mem) {
        TCERR("shared_mem kmalloc failed\n");
        ret = -ENOMEM;
        return ret;
    }

    shared_mem->kernel_addr = addr;
    shared_mem->len = len;
    shared_mem->user_addr = (void*)vma->vm_start;
    //printk("kernel_addr = %08x, user_addr = %08x\n", addr, (void*)vma->vm_start);
    mutex_lock(&tc_ns_dev_list.dev_lock);
    list_for_each_entry(dev_file, &tc_ns_dev_list.dev_file_list, head){
        if(dev_file->dev_file_id == (unsigned int)filp->private_data){
            find_dev = 1;
            dev_file->shared_mem_cnt++;
            //mutex_lock(&dev_file->shared_mem_lock);
            list_add_tail(&shared_mem->head, &dev_file->shared_mem_list);
            //mutex_unlock(&dev_file->shared_mem_lock);
        }
    }
    mutex_unlock(&tc_ns_dev_list.dev_lock);

    if(!find_dev){
        TCERR("can not find dev in malloc shared buffer!\n");
        ret = TEEC_ERROR_GENERIC;
    }

    return ret;
}

static const struct file_operations TC_NS_ClientFops = {
        .owner = THIS_MODULE,
        .open = TC_NS_ClientOpen,
        .release = TC_NS_ClientClose,
        .unlocked_ioctl = TC_NS_ClientIoctl,
        .mmap = TC_NS_ClientMalloc
};

#if 0
struct smc_strcut {
    struct task_struct *pthread;
    struct semaphore smc_sem;
    struct completion smc_wait;
    unsigned int cmd_addr;
    unsigned int cmd_ret;
};
static struct smc_strcut *p_smc_data=NULL;

static void init_smc_struct_data(struct smc_strcut *p)
{
    sema_init(&p->smc_sem, 0);
    init_completion(&p->smc_wait);
    p->pthread = NULL;
    p->cmd_addr = 0;
    p->cmd_ret = 0;
}
#endif

unsigned int smc_send_on_cpu0(unsigned int cpu_id, unsigned int cmd_addr)
{
    struct cpumask old_cpu_set;
    int ret = 0;
#if 0
    //printk(KERN_INFO"smc send on cpu0 call\n");
    p_smc_data->cmd_addr = cmd_addr;
    wmb();
    up(&p_smc_data->smc_sem);
    //printk(KERN_INFO"smc call wake smc_thread\n");
    wait_for_completion(&p_smc_data->smc_wait);
    rmb();
    return p_smc_data->cmd_ret;
#else

    /* save old and bind process affinity to cpu0 */
    if (cpu_id)
    {
        ret = sched_getaffinity(current->pid, &old_cpu_set);
        if (ret != 0)
        {
            printk("get process(%d) cpu affinity failed.\n", current->pid);
            return -1;
        }

        ret = sched_setaffinity(current->pid, cpumask_of(0));
        if (ret != 0)
        {
            printk("bind process(%d) to cpu0 failed.\n", current->pid);
            return -1;
        }
    }
	
    BUG_ON(0 != smp_processor_id());
	
    rmb();
    ret = smc_send(cmd_addr);
    wmb();
	
    /* restore old process cpu affinity */
    if (cpu_id)
    {
        ret = sched_setaffinity(current->pid, &old_cpu_set);
        if (ret != 0)
        {
            printk("restore process(%d) cpu affinity failed.\n", current->pid);
            return -1;
        }
    }

    return ret;
#endif
}

#if 0
void smc_thread_work(void)
{
    unsigned int cpu_id;
    unsigned int ret;

    while(1)
    {
        down(&p_smc_data->smc_sem);
        /*cpu_id = smp_processor_id();
        cpu_id = get_cpu();
        put_cpu();
        printk(KERN_INFO"---->smc thread start : on cpu %d\n", cpu_id);*/
        rmb();
        ret = smc_send(p_smc_data->cmd_addr);
        wmb();
        p_smc_data->cmd_ret = ret;
        complete(&p_smc_data->smc_wait);
    }
}
#endif

static int TC_NS_ClientInit(void)
{
    struct device *class_dev;
    int ret = 0;

    ret = alloc_chrdev_region(&tc_ns_client_devt, 0, 1, TC_NS_CLIENT_DEV);
    if(ret){
        TCERR("alloc_chrdev_region failed(%d)", ret);
        goto err1;
    }

    driver_class = class_create(THIS_MODULE, TC_NS_CLIENT_DEV);
    if(IS_ERR(driver_class)) {
        ret = PTR_ERR(driver_class);
        TCERR("class_create failed(%d)", ret);
        goto err2;
    }

    class_dev = device_create(driver_class, NULL, tc_ns_client_devt, NULL, TC_NS_CLIENT_DEV);
    if (IS_ERR(class_dev)) {
        ret = PTR_ERR(class_dev);
        TCERR("class_device_create failed(%d)", ret);
        goto err3;
    }

    cdev_init(&tc_ns_client_cdev, &TC_NS_ClientFops);
    ret = cdev_add(&tc_ns_client_cdev, MKDEV(MAJOR(tc_ns_client_devt), 0), 1);
    if (ret) {
        TCERR("cdev_add failed(%d)", ret);
        goto err4;
    }

    memset(&tc_ns_dev_list, 0, sizeof(tc_ns_dev_list));
    tc_ns_dev_list.dev_file_cnt = 0;
    INIT_LIST_HEAD(&tc_ns_dev_list.dev_file_list);
    mutex_init(&tc_ns_dev_list.dev_lock);

    spin_lock_init(&agent_control.lock);
    INIT_LIST_HEAD(&agent_control.agent_list);

#ifdef TC_ASYNC_NOTIFY_SUPPORT
    printk("register secure notify handler\n");
    register_secure_notify_handler(ipi_secure_notify);
#endif

    printk("TC_NS_Client initialized.\n");

    return 0;

err4:
    device_destroy(driver_class, tc_ns_client_devt);
err3:
    class_destroy(driver_class);
err2:
    unregister_chrdev_region(tc_ns_client_devt, 1);
err1:

    return ret;
}

static void TC_NS_ClientExit(void)
{
#ifdef TC_ASYNC_NOTIFY_SUPPORT
    unregister_secure_notify_handler();
#endif
    device_destroy(driver_class, tc_ns_client_devt);
    class_destroy(driver_class);
    unregister_chrdev_region(tc_ns_client_devt, 1);

    printk("TC_NS_Client removed.\n");
}

EXPORT_SYMBOL(TC_NS_ClientOpen_from_kernel);
EXPORT_SYMBOL(TC_NS_ClientIoctl_from_kernel);
EXPORT_SYMBOL(TC_NS_OpenSession_from_kernel);
EXPORT_SYMBOL(TC_NS_ClientClose_from_kernel);
EXPORT_SYMBOL(TC_NS_CloseSession_from_kernel);

MODULE_AUTHOR("q00209673");
MODULE_DESCRIPTION("TrustCore ns-client driver");
MODULE_VERSION("1.00");
MODULE_LICENSE("GPL");

module_init(TC_NS_ClientInit);
module_exit(TC_NS_ClientExit);
