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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/smp.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>
//#include <linux/tzDriver-dev.h>


#include "tee_client_api.h"
#include "tee_client_id.h"
//#include "../tzdriver/tc_ns_client.h"
#include "tc_ns_client.h"
#include "tee_client_app_load.h"

#define MAX_APP_INFO 100
#define SYSTEM_BIN_SIZE 23

extern int TC_NS_ClientOpen_from_kernel(struct inode *inode, struct file *file,unsigned int *dev_id);
extern long TC_NS_ClientIoctl_from_kernel(unsigned int dev_id, unsigned cmd,
    unsigned long arg);
extern int TC_NS_OpenSession_from_kernel(unsigned int dev_id, void* argp);
extern int TC_NS_ClientClose_from_kernel(struct inode *inode, struct file *file,unsigned int dev_id);
extern int TC_NS_CloseSession_from_kernel(unsigned int dev_id, void* argp);


TEEC_Result TEEC_CheckOperation (
    TEEC_Operation *operation);

void TEEC_Encode(
    TC_NS_ClientContext* cli_context,
    TEEC_UUID service_id,
    uint32_t session_id,
    uint32_t cmd_id,
    TC_NS_ClientLogin cli_login,
    TEEC_Operation* operation);

/*
 * Function:       TEEC_InitializeContext
 * Description:   This function initializes a new TEE Context, forming a connection between
 *                     this Client Application and the TEE identified by the string identifier name.
 * Parameters:   name: a zero-terminated string that describes the TEE to connect to.
 *                     If this parameter is set to NULL, the Implementation MUST select a default TEE.
 *                     context: a TEEC_Context structure that be initialized by the Implementation.
 * Return:         TEEC_SUCCESS: the initialization was successful.
 *                     other: initialization was not successful.
 */
TEEC_Result TEEC_InitializeContext (
    const char* name,
    TEEC_Context* context,unsigned int* dev_id)
{
    TEEC_Result ret;
    struct file test_file;

    /* First, check parameters is valid or not */
    if (context == NULL)
        return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;

    /* Paramters right, start execution */
    ret = TC_NS_ClientOpen_from_kernel(NULL,&test_file,dev_id);

    /*if (name == NULL)
        ret = sys_open(TC_NS_CLIENT_DEV_NAME, O_RDWR);
    else
        ret = sys_open(name, O_RDWR);*/

    if (ret != TEEC_SUCCESS) {
        TEEC_Error("open device failed\n");
        return (TEEC_Result)TEEC_ERROR_GENERIC;
    } else {
        TEEC_Debug("open device success\n");
        context->fd = *dev_id;
        INIT_LIST_HEAD(&context->session_list);
        INIT_LIST_HEAD(&context->shrd_mem_list);
        return TEEC_SUCCESS;
    }
}
EXPORT_SYMBOL(TEEC_InitializeContext);

/*
 * Function:       TEEC_FinalizeContext
 * Description:   This function finalizes an initialized TEE Context.
 * Parameters:   context: an initialized TEEC_Context structure which is to be finalized.
 * Return:         NULL
 */
void TEEC_FinalizeContext (
    TEEC_Context* context,unsigned int dev_id)
{
    struct list_node *ptr;
    TEEC_Session* session;
    TEEC_SharedMemory* shrdmem;
    struct file test_file;

    /* First, check parameters is valid or not */
    if (context == NULL)
        return;
#if 0
    /* Paramters right, start execution */
    if (!LIST_EMPTY(&context->session_list)) {
        TEEC_Debug("context still has sessions opened, close it\n");
        list_for_each(ptr, &context->session_list) {
            session = list_entry(ptr, TEEC_Session, head);
            TEEC_CloseSession(session);
        }
    }

    if (!LIST_EMPTY(&context->shrd_mem_list)) {
        TEEC_Debug("context contains unreleased Shared Memory blocks, release it\n");
        list_for_each(ptr, &context->shrd_mem_list) {
            shrdmem = list_entry(ptr, TEEC_SharedMemory, head);
            TEEC_ReleaseSharedMemory(shrdmem);
        }
    }
#endif
    TEEC_Debug("close device\n");
    //sys_close(context->fd);
    TC_NS_ClientClose_from_kernel(NULL,&test_file,dev_id);
    context->fd = 0;
}
EXPORT_SYMBOL(TEEC_FinalizeContext);

/*
 * Function:       TEEC_OpenSession
 * Description:   This function opens a new Session
 * Parameters:   context: a pointer to an initialized TEE Context.
 *                     session: a pointer to a Session structure to open.
 *                     destination: a pointer to a UUID structure.
 *                     connectionMethod: the method of connection to use.
 *                     connectionData: any necessary data required to support the connection method chosen.
 *                     operation: a pointer to an Operation containing a set of Parameters.
 *                     returnOrigin: a pointer to a variable which will contain the return origin.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_OpenSession (
    TEEC_Context* context,
    TEEC_Session* session,
    const TEEC_UUID* destination,
    uint32_t connectionMethod,
    const void* connectionData,
    TEEC_Operation* operation,
    uint32_t* returnOrigin,
    unsigned int dev_id
    )
{
    int32_t ret;
    TEEC_Result teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
    uint32_t origin = TEEC_ORIGIN_API;
    TC_NS_ClientContext cli_context;
    TC_NS_ClientLogin cli_login = {0, 0};
    char linkc[MAX_APP_INFO], path[MAX_APP_INFO];
    char linkj[MAX_APP_INFO];/*, pathj[MAX_APP_INFO];*/
    int32_t retz;
    int  fd;
    uint32_t num=0;
    struct file test_file;

    /* First, check parameters is valid or not */
    if ((context == NULL) || (session == NULL) || (destination == NULL))
        goto error;

    switch (connectionMethod) {
        case TEEC_LOGIN_PUBLIC:
        case TEEC_LOGIN_USER:
        case TEEC_LOGIN_APPLICATION:
        case TEEC_LOGIN_USER_APPLICATION:
            if (connectionData != NULL)
                goto error;
            break;
        case TEEC_LOGIN_GROUP:
        case TEEC_LOGIN_GROUP_APPLICATION:
            if (connectionData == NULL)
                goto error;
            cli_login.method = connectionMethod;
            cli_login.mdata = *(uint32_t*)connectionData;
            break;
        case TEEC_LOGIN_IDENTIFY:
            if (connectionData != NULL)
                goto error;
#if 0
            cli_login.method = connectionMethod;
            //cli_login.mdata= *(uint32_t*)connectionData;

            memset(path, 0x00, MAX_APP_INFO);
            (void)snprintf(linkc,MAX_APP_INFO,"/proc/%d/exe", getpid());
            retz = readlink(linkc,path,MAX_APP_INFO-1);
            if(retz == -1 || retz >MAX_APP_INFO-1)
            {
                TEEC_Error("read link is error\n");
                goto error;
            }
            path[retz]='\0';
            num = retz;
            retz = strncmp(path,"/system/bin/app_process",SYSTEM_BIN_SIZE);
            if(!retz)
            {
                TEEC_Debug("java app\n");
                (void)snprintf(linkj,MAX_APP_INFO,"/proc/%d/cmdline", getpid());
                fd=file_open(linkj,"r");
                if(fd==NULL)
                {
                    TEEC_Error("fopen is error\n");
                    goto error;
                }
                memset(path, 0x00, MAX_APP_INFO);
                num = vfs_read(path,1,MAX_APP_INFO-1,fd);
                file_close(fd);
                if(num ==0 || num>MAX_APP_INFO-1)
                    goto error;
                //path[num]='\0';
            }
            else{
                TEEC_Debug("c app\n");
            }
            TEEC_Debug("path is %s, num is %d\n", path, num);
            operation->params[3].tmpref.buffer = path;
            operation->params[3].tmpref.size = num;
            break;
#endif
        default:
            goto error;
    }

    teec_ret = TEEC_CheckOperation(operation);
    if (teec_ret != TEEC_SUCCESS) {
        TEEC_Error("operation is invalid\n");
        goto error;
    }

    /* if success, should return a service id */
#if 0
    ret = TEEC_StartApp(context, destination);
    if (ret < 0) {
        TEEC_Error("start app error\n");
        teec_ret = (TEEC_Result)TEEC_ERROR_TRUSTED_APP_LOAD_ERROR;
        goto error;
    }
#endif

    /* Paramters right, start execution */
    /*
     * note:before open session success, we should send session=0 as initial state.
     */
    TEEC_Encode(&cli_context, *destination, 0/*session->session_id*/,
        GLOBAL_CMD_ID_OPEN_SESSION, cli_login, operation);

    //ret = ioctl(context->fd, TC_NS_CLIENT_IOCTL_SES_OPEN_REQ, &cli_context);
    //printk("TC_NS_CLIENT_IOCTL_SES_OPEN_REQ = %x, %s, %d\n", TC_NS_CLIENT_IOCTL_SES_OPEN_REQ, __func__, __LINE__);
    TC_NS_ClientIoctl_from_kernel(dev_id, TC_NS_CLIENT_IOCTL_SES_OPEN_REQ, &cli_context );
    if (ret == 0) {
        TEEC_Debug("open session success\n");
        session->session_id = cli_context.session_id;
        session->service_id = *destination;
        session->ops_cnt = 0;
        INIT_LIST_HEAD(&session->head);
        //list_insert_tail(&context->session_list, &session->head);
        session->context = context;
        //operation->session = session;
        teec_ret = TEEC_SUCCESS;
    } else if (ret < 0) {
        TEEC_Error("open session failed, ioctl errno = %d\n", ret);
        if (ret == -EFAULT)
            teec_ret = (TEEC_Result)TEEC_ERROR_ACCESS_DENIED;
        else if (ret == -ENOMEM)
            teec_ret = (TEEC_Result)TEEC_ERROR_OUT_OF_MEMORY;
        else if (ret == -EINVAL)
            teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
        else
            teec_ret = (TEEC_Result)TEEC_ERROR_GENERIC;
        origin = TEEC_ORIGIN_COMMS;
    } else {
        TEEC_Error("open session failed, code=0x%x, origin=%d\n", cli_context.returns.code, cli_context.returns.origin);
        teec_ret = (TEEC_Result)cli_context.returns.code;
        origin = cli_context.returns.origin;
    }

error:
    /* ONLY when ioctl returnCode!=0 and returnOrigin not NULL,
     * set *returnOrigin
     */
    if((teec_ret != TEEC_SUCCESS) && (returnOrigin))
        *returnOrigin = origin;
    return teec_ret;
}
EXPORT_SYMBOL(TEEC_OpenSession);

/*
 * Function:       TEEC_CloseSession
 * Description:   This function closes an opened Session.
 * Parameters:   session: the session to close.
 * Return:         NULL
 */
void TEEC_CloseSession (
    TEEC_Session* session,unsigned int dev_id)
{
    int32_t ret;
    TC_NS_ClientContext cli_context;
    TC_NS_ClientLogin cli_login = {0, 0};
    struct list_node *ptr;
    TEEC_Session* temp_sess;
    bool found = false;
    struct file test_file;
    /* First, check parameters is valid or not */
    if (session ==  NULL)
        return;
#if 0
     list_for_each(ptr, &session->context->session_list) {
        temp_sess = list_entry(ptr, TEEC_Session, head);
        if (temp_sess == session) {
            found = true;
            break;
        }
    }
    if (!found) {
        TEEC_Error("session isnot in the context list\n");
        return;
    }
#endif

    /* Paramters all right, start execution */
    if (session->ops_cnt)
        TEEC_Error("session still has commands running\n");

    TEEC_Encode(&cli_context, session->service_id, session->session_id,
        GLOBAL_CMD_ID_CLOSE_SESSION, cli_login, NULL);

    //ret = ioctl(session->context->fd, TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ, &cli_context);
    TC_NS_ClientIoctl_from_kernel(dev_id,TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ,  &cli_context);
    #if 0
    if (ret == 0) {
        TEEC_Debug("close session success\n");
        session->session_id = 0;
        memset((uint8_t*)&session->service_id, 0x00, 16);
        session->ops_cnt = 0;
        list_remove(&session->head);
        session->context = NULL;
    } else {
        TEEC_Error("close session failed\n");
    }
    #endif
}
EXPORT_SYMBOL(TEEC_CloseSession);

/*
 * Function:       TEEC_InvokeCommand
 * Description:   This function invokes a Command within the specified Session.
 * Parameters:   session: the open Session in which the command will be invoked.
 *                     commandID: the identifier of the Command.
 *                     operation: a pointer to an Operation containing a set of Parameters.
 *                     returnOrigin: a pointer to a variable which will contain the return origin.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_InvokeCommand(
    TEEC_Session*     session,
    uint32_t          commandID,
    TEEC_Operation*   operation,
    uint32_t*         returnOrigin,
    unsigned int dev_id)
{
    int32_t ret;
    TEEC_Result teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
    uint32_t origin = TEEC_ORIGIN_API;
    TC_NS_ClientContext cli_context;
    TC_NS_ClientLogin cli_login = {0, 0};
    struct file test_file;

    /* First, check parameters is valid or not */
    if (session == NULL)
        goto error;

    teec_ret = TEEC_CheckOperation(operation);
    if (teec_ret != TEEC_SUCCESS) {
        TEEC_Error("operation is invalid\n");
        goto error;
    }

    /* Paramters all right, start execution */
    session->ops_cnt++;
    TEEC_Encode(&cli_context, session->service_id, session->session_id,
        commandID, cli_login, operation);

    //ret = ioctl(session->context->fd, TC_NS_CLIENT_IOCTL_SEND_CMD_REQ, &cli_context);
    TC_NS_ClientIoctl_from_kernel(dev_id, TC_NS_CLIENT_IOCTL_SEND_CMD_REQ, &cli_context );
    if (ret == 0) {
        TEEC_Debug("invoke cmd success\n");
        teec_ret = TEEC_SUCCESS;
    } else if (ret < 0) {
        TEEC_Error("invoke cmd failed, ioctl errno = %d\n", ret);
        if (ret == -EFAULT)
            teec_ret = (TEEC_Result)TEEC_ERROR_ACCESS_DENIED;
        else if (ret == -ENOMEM)
            teec_ret = (TEEC_Result)TEEC_ERROR_OUT_OF_MEMORY;
        else if (ret == -EINVAL)
            teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
        else
            teec_ret = (TEEC_Result)TEEC_ERROR_GENERIC;
        origin = TEEC_ORIGIN_COMMS;
    } else{
        TEEC_Error("invoke cmd failed, code=0x%x, origin=%d\n", cli_context.returns.code, cli_context.returns.origin);
        teec_ret = (TEEC_Result)cli_context.returns.code;
        origin = cli_context.returns.origin;
    }
    session->ops_cnt--;

error:
    /* ONLY when ioctl returnCode!=0 and returnOrigin not NULL,
     * set *returnOrigin
     */
    if((teec_ret != TEEC_SUCCESS) && (returnOrigin))
        *returnOrigin = origin;
    return teec_ret;
}
EXPORT_SYMBOL(TEEC_InvokeCommand);

/*
 * Function:       TEEC_RegisterSharedMemory
 * Description:   This function registers a block of existing Client Application memory
 *                     as a block of Shared Memory within the scope of the specified TEE Context.
 * Parameters:   context: a pointer to an initialized TEE Context.
 *                     sharedMem: a pointer to a Shared Memory structure to register.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_RegisterSharedMemory (
    TEEC_Context* context,
    TEEC_SharedMemory* sharedMem)
{
#if 0
    /* First, check parameters is valid or not */
    if ((context == NULL) || (sharedMem == NULL))
        return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;

    if ((sharedMem->buffer == NULL)
        || ((sharedMem->flags != TEEC_MEM_INPUT)
            && (sharedMem->flags != TEEC_MEM_OUTPUT)
            && (sharedMem->flags != TEEC_MEM_INOUT)))
        return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;

    /* Paramters all right, start execution */
    sharedMem->ops_cnt = 0;
    sharedMem->is_allocated = false;
    INIT_LIST_HEAD(&sharedMem->head);
    list_insert_tail(&context->shrd_mem_list, &sharedMem->head);
    sharedMem->context = context;
#endif
    return TEEC_SUCCESS;
}

/*
 * Function:       TEEC_AllocateSharedMemory
 * Description:   This function allocates a new block of memory as a block of
 *                     Shared Memory within the scope of the specified TEE Context.
 * Parameters:   context: a pointer to an initialized TEE Context.
 *                     sharedMem: a pointer to a Shared Memory structure to allocate.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_AllocateSharedMemory (
    TEEC_Context* context,
    TEEC_SharedMemory* sharedMem)
{
#if 0
    TEEC_Result teec_ret;
    /* First, check parameters is valid or not */
    if ((context == NULL) || (sharedMem == NULL))
        return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;

    if ((sharedMem->flags != TEEC_MEM_INPUT)
            && (sharedMem->flags != TEEC_MEM_OUTPUT)
            && (sharedMem->flags != TEEC_MEM_INOUT))
        return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;

    /* Paramters all right, start execution */
    sharedMem->buffer = NULL;
    if (sharedMem->size != 0)
        sharedMem->buffer = mmap(0, sharedMem->size,
            (PROT_READ | PROT_WRITE), MAP_SHARED, context->fd, 0);

    if (sharedMem->buffer == MAP_FAILED) {
        TEEC_Error("mmap failed\n");
        sharedMem->buffer = NULL;
        teec_ret = (TEEC_Result)TEEC_ERROR_OUT_OF_MEMORY;
    } else {
        sharedMem->ops_cnt = 0;
        sharedMem->is_allocated = true;
        INIT_LIST_HEAD(&sharedMem->head);
        list_insert_tail(&context->shrd_mem_list, &sharedMem->head);
        sharedMem->context = context;
        teec_ret = TEEC_SUCCESS;
    }

    return teec_ret;
#endif
}

/*
 * Function:       TEEC_ReleaseSharedMemory
 * Description:   This function deregisters or deallocates a previously initialized
 *                      block of Shared Memory..
 * Parameters:   sharedMem: a pointer to a valid Shared Memory structure.
 * Return:         NULL
 */
void TEEC_ReleaseSharedMemory (
    TEEC_SharedMemory* sharedMem)
{
    #if 0
    int32_t ret;
    struct list_node *ptr;
    TEEC_SharedMemory* temp_shardmem;
    bool found = false;
    /* DTS2013030408264 h00211444 begin */
    void* rel_addr;
    /* DTS2013030408264 h00211444 end */

    /* First, check parameters is valid or not */
    if (sharedMem == NULL) {
        return;
    }

    if (sharedMem->ops_cnt) {
        TEEC_Error("Shared Memory is used by a pending operation\n");
        return;
    }

    list_for_each(ptr, &sharedMem->context->shrd_mem_list) {
        temp_shardmem = list_entry(ptr, TEEC_SharedMemory, head);
        if (temp_shardmem == sharedMem) {
            found = true;
            break;
        }
    }
    if (!found) {
        TEEC_Error("Shared Memory isnot in the list\n");
        return;
    }

    /* Paramters right, start execution */
    if ((sharedMem->is_allocated) && (sharedMem->size != 0)) {
        /* DTS2013030408264 h00211444 begin */
        rel_addr = (void*)sharedMem->buffer;
        /* call munmap, notify os decrease mmap count */
        ret = munmap(sharedMem->buffer, sharedMem->size);
        if (ret) {
            TEEC_Error("Release SharedMemory failed, munmap error\n");
            return;
        }
        /* call ioctl, notify driver release memory */
        ret = ioctl(sharedMem->context->fd,
                TC_NS_CLIENT_IOCTL_SHRD_MEM_RELEASE, rel_addr);
        if (ret) {
            TEEC_Error("release SharedMemory failed, ioctl error\n");
            return;
        }
        /* DTS2013030408264 h00211444 end */
    }

    sharedMem->buffer = NULL;
    sharedMem->size = 0;
    sharedMem->flags = 0;
    sharedMem->ops_cnt = 0;
    list_remove(&sharedMem->head);
    sharedMem->context = NULL;
    #endif
}

static TEEC_Result TEEC_CheckTmpRef(
    TEEC_TempMemoryReference tmpref)
{
    TEEC_Result ret;

    if ((tmpref.buffer == NULL) || (tmpref.size == 0)) {
        TEEC_Error("tmpref buffer is null, or size is zero\n");
        ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
    } else {
        ret = (TEEC_Result)TEEC_SUCCESS;
    }
    return ret;
}

static TEEC_Result TEEC_CheckMemRef(
    TEEC_RegisteredMemoryReference memref,
    uint32_t param_type)
{
    if ((memref.parent == NULL) || (memref.parent->buffer == NULL)) {
        TEEC_Error("parent of memref is null, or the buffer is zero\n");
        return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
    }

    if (param_type == TEEC_MEMREF_PARTIAL_INPUT) {
        if (!(memref.parent->flags & TEEC_MEM_INPUT))
            goto param_error;
    } else if (param_type == TEEC_MEMREF_PARTIAL_OUTPUT) {
        if (!(memref.parent->flags & TEEC_MEM_OUTPUT))
            goto param_error;
    } else if (param_type == TEEC_MEMREF_PARTIAL_INOUT) {
        /* DTS2013030509162 h00211444 begin */
        if (!(memref.parent->flags & TEEC_MEM_INPUT))
            goto param_error;
        if (!(memref.parent->flags & TEEC_MEM_OUTPUT))
            goto param_error;
        /* DTS2013030509162 h00211444 end */
    } else {
        // if type is TEEC_MEMREF_WHOLE, ignore it
    }

    if ((param_type == TEEC_MEMREF_PARTIAL_INPUT)
        || (param_type == TEEC_MEMREF_PARTIAL_OUTPUT)
        || (param_type == TEEC_MEMREF_PARTIAL_INOUT)) {
        if ((memref.offset + memref.size) > memref.parent->size) {
            TEEC_Error("offset + size exceed the parent size\n");
            return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
        }
    }

    return (TEEC_Result)TEEC_SUCCESS;
param_error:
    TEEC_Error("type of memref not belong to the parent flags\n");
    return (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
}

/*
 * Function:       TEEC_CheckOperation
 * Description:   This function checks a operation is valid or not.
 * Parameters:   operation: a pointer to an Operation to be checked.
 * Return:         TEEC_SUCCESS: success
 *                     other: failure
 */
TEEC_Result TEEC_CheckOperation (
    TEEC_Operation *operation)
{
    uint32_t param_type[4];
    uint32_t param_cnt;
    TEEC_Result ret = TEEC_SUCCESS;

    if (operation == NULL)
        return ret;
#if 0
    if (!operation->started) {
        TEEC_Error("sorry, cancellation not support\n");
        return (TEEC_Result)TEEC_ERROR_NOT_IMPLEMENTED;
    }
#endif

    param_type[0] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 0);
    param_type[1] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 1);
    param_type[2] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 2);
    param_type[3] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 3);

    for (param_cnt = 0;param_cnt < 4;param_cnt++) {
        if ((param_type[param_cnt] == TEEC_MEMREF_TEMP_INPUT)
            || (param_type[param_cnt] == TEEC_MEMREF_TEMP_OUTPUT)
            || (param_type[param_cnt] == TEEC_MEMREF_TEMP_INOUT)) {

            if ((ret = TEEC_CheckTmpRef(operation->params[param_cnt].tmpref))
                    != TEEC_SUCCESS)
                break;
        } else if ((param_type[param_cnt] == TEEC_MEMREF_WHOLE)
            || (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INPUT)
            || (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_OUTPUT)
            || (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INOUT)) {

            if ((ret = TEEC_CheckMemRef(operation->params[param_cnt].memref,
                            param_type[param_cnt])) != TEEC_SUCCESS)
                break;
        } else if ((param_type[param_cnt] ==  TEEC_VALUE_INPUT)
            || (param_type[param_cnt] == TEEC_VALUE_OUTPUT)
            || (param_type[param_cnt] == TEEC_VALUE_INOUT)) {
            // if type is value, ignore it
        } else if (param_type[param_cnt] == TEEC_NONE){
            // if type is none, ignore it
        } else {
            TEEC_Error("paramType[%d]=%x is not support\n",
                    param_cnt, param_type[param_cnt]);
            ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
            break;
        }
    }

    return ret;
}

void TEEC_Encode(
    TC_NS_ClientContext* cli_context,
    TEEC_UUID service_id,
    uint32_t session_id,
    uint32_t cmd_id,
    TC_NS_ClientLogin cli_login,
    TEEC_Operation* operation)
{
    uint32_t param_type[4];
    uint32_t param_cnt;
    uint32_t diff;

    diff = (uint32_t)TEEC_MEMREF_PARTIAL_INPUT - (uint32_t)TEEC_MEMREF_TEMP_INPUT;

    memset(cli_context, 0x00, sizeof(TC_NS_ClientContext));

    memcpy(cli_context->uuid, (uint8_t*)&service_id, 16);
    cli_context->session_id = session_id;
    cli_context->cmd_id = cmd_id;
    cli_context->returns.code = 0;
    cli_context->returns.origin = 0;

    cli_context->login.method = cli_login.method;
    cli_context->login.mdata = cli_login.mdata;

    if ((operation == NULL) || (!operation->paramTypes))
        return;
//    cli_context->started = operation->cancel_flag;
    param_type[0] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 0);
    param_type[1] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 1);
    param_type[2] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 2);
    param_type[3] = TEEC_PARAM_TYPE_GET(operation->paramTypes, 3);

    for (param_cnt = 0;param_cnt < 4;param_cnt++) {
        if ((param_type[param_cnt] == TEEC_MEMREF_TEMP_INPUT)
            || (param_type[param_cnt] == TEEC_MEMREF_TEMP_OUTPUT)
            || (param_type[param_cnt] == TEEC_MEMREF_TEMP_INOUT)) {

            cli_context->params[param_cnt].memref.buffer
                = (uint32_t)operation->params[param_cnt].tmpref.buffer;
            cli_context->params[param_cnt].memref.size_addr
                = (uint32_t)&operation->params[param_cnt].tmpref.size;
        } else if ((param_type[param_cnt] == TEEC_MEMREF_WHOLE)
            || (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INPUT)
            || (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_OUTPUT)
            || (param_type[param_cnt] == TEEC_MEMREF_PARTIAL_INOUT)) {

            /* buffer offset len */
            if (param_type[param_cnt] == TEEC_MEMREF_WHOLE) {
                cli_context->params[param_cnt].memref.offset = 0;
                cli_context->params[param_cnt].memref.size_addr
                    = (uint32_t)&operation->params[param_cnt].memref.parent->size;
            } else {
                cli_context->params[param_cnt].memref.offset
                    = operation->params[param_cnt].memref.offset;
                cli_context->params[param_cnt].memref.size_addr
                    = (uint32_t)&operation->params[param_cnt].memref.size;
            }

            if (operation->params[param_cnt].memref.parent->is_allocated) {
                cli_context->params[param_cnt].memref.buffer
                    = (uint32_t)operation->params[param_cnt].memref.parent->buffer;
            } else {
                cli_context->params[param_cnt].memref.buffer
                    = (uint32_t)operation->params[param_cnt].memref.parent->buffer
                    + operation->params[param_cnt].memref.offset;
                cli_context->params[param_cnt].memref.offset = 0;
            }
            /* DTSDTS2013030104869 h00211444 begin */
            /* translate the paramType to know the driver */
            if (param_type[param_cnt] == TEEC_MEMREF_WHOLE) {
                switch (operation->params[param_cnt].memref.parent->flags) {
                    case TEEC_MEM_INPUT:
                        param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INPUT;
                        break;
                    case TEEC_MEM_OUTPUT:
                        param_type[param_cnt] = TEEC_MEMREF_PARTIAL_OUTPUT;
                        break;
                    case TEEC_MEM_INOUT:
                        param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INOUT;
                        break;
                    default:
                        param_type[param_cnt] = TEEC_MEMREF_PARTIAL_INOUT;
                        break;
                }
            }
            /* if is not allocated,
             * translate TEEC_MEMREF_PARTIAL_XXX to TEEC_MEMREF_TEMP_XXX */
            if (!operation->params[param_cnt].memref.parent->is_allocated)
                param_type[param_cnt] = param_type[param_cnt] - diff;
            /* DTSDTS2013030104869 h00211444 end */
        }  else if ((param_type[param_cnt] ==  TEEC_VALUE_INPUT)
            || (param_type[param_cnt] == TEEC_VALUE_OUTPUT)
            || (param_type[param_cnt] == TEEC_VALUE_INOUT)) {

            cli_context->params[param_cnt].value.a_addr
                = (uint32_t)&operation->params[param_cnt].value.a;
            cli_context->params[param_cnt].value.b_addr
                = (uint32_t)&operation->params[param_cnt].value.b;
        } else {
            /* if type is none, ignore it */
        }
    }
    cli_context->paramTypes = TEEC_PARAM_TYPES(
            param_type[0], param_type[1], param_type[2], param_type[3]);
    TEEC_Debug("cli param type %d\n", cli_context->paramTypes);
}
/*
 * Function:       TEEC_RequestCancellation
 * Description:   This function requests the cancellation of a pending open Session operation or
            a Command invocation operation.
 * Parameters:   operation:a pointer to a Client Application instantiated Operation structure
 * Return:         NULL
 */
void TEEC_RequestCancellation(TEEC_Operation *operation)
{
#if 0
    int32_t ret;
    TEEC_Result teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
    TC_NS_ClientContext cli_context;
    TC_NS_ClientLogin cli_login = {0, 0};
    TEEC_Session *session = operation->session;

    printk("before iotcl send\n");
    /* First, check parameters is valid or not */
    if (session == NULL)
        goto error;

    teec_ret = TEEC_CheckOperation(operation);
    if (teec_ret != TEEC_SUCCESS) {
        TEEC_Error("operation is invalid\n");
        goto error;
    }

    /* Paramters all right, start execution */
   // session->ops_cnt++;
    TEEC_Encode(&cli_context, session->service_id, session->session_id,
        TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ, cli_login, operation);

    //ret = ioctl(session->context->fd, TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ, &cli_context);
    if (ret == 0) {
        TEEC_Debug("invoke cmd success\n");
        teec_ret = TEEC_SUCCESS;
    } else if (ret < 0) {
        TEEC_Error("invoke cmd failed, ioctl errno = %d\n", ret);
        if (ret == -EFAULT)
            teec_ret = (TEEC_Result)TEEC_ERROR_ACCESS_DENIED;
        else if (ret == -ENOMEM)
            teec_ret = (TEEC_Result)TEEC_ERROR_OUT_OF_MEMORY;
        else if (ret == -EINVAL)
            teec_ret = (TEEC_Result)TEEC_ERROR_BAD_PARAMETERS;
        else
            teec_ret = (TEEC_Result)TEEC_ERROR_GENERIC;
    } else{
        TEEC_Error("invoke cmd failed, code=0x%x, origin=%d\n", cli_context.returns.code, cli_context.returns.origin);
        teec_ret = (TEEC_Result)cli_context.returns.code;
    }

error:
    /* ONLY when ioctl returnCode!=0 and returnOrigin not NULL,
     * set *returnOrigin
     */
   // if((teec_ret != TEEC_SUCCESS) && (returnOrigin))
   //     *returnOrigin = origin;
#endif
    return;

}

