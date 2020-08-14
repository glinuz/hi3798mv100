/******************************************************************************
Copyright (C), 2012-2014 Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_module_ioctl.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The module manager.
Function List : None.
History       :
******************************************************************************/

#ifndef __DRV_MODULE_IOCTL_H__
#define __DRV_MODULE_IOCTL_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "hi_type.h"

#define MAX_MODULE_NAME (16)

typedef struct tagMODULE_ALLOC
{
    HI_U32 u32ModuleID;
    HI_U8  u8ModuleName[MAX_MODULE_NAME+12];      /* '_' 1 + pid 10 */
    HI_S32 s32Status;
}MODULE_ALLOC_S;

// !! Bytes:20
typedef struct tagMODULE_INFO
{
    HI_U32 u32ModuleID;
    HI_U8  u8ModuleName[MAX_MODULE_NAME+12];      /* '_' 1 + pid 10 */
    HI_VOID* pFnCallback;
    
    HI_S32 s32RegCount;
    HI_U32 *pFile;
}MODULE_INFO_S;

/* compat define for MODULE_INFO_S. */
typedef struct Compat_tagMODULE_INFO
{
    HI_U32 u32ModuleID;
    HI_U8  u8ModuleName[MAX_MODULE_NAME+12];      /* '_' 1 + pid 10 */
    HI_U32 pFnCallback;
    
    HI_S32 s32RegCount;
    HI_U32 u32File;
}Compat_MODULE_INFO_S;

// !! Bytes:24
typedef struct tagMODULE
{
    MODULE_INFO_S       stModuleInfo;
    HI_U32              u32ItemCnt;
    struct tagMODULE* pNextModule;
}MODULE_S;

// !! Bytes:28
typedef struct tagMODULE_POOL
{
    HI_U32  u32Idle;
    MODULE_S stItem;
}MODULE_POOL_S;


// !! If this case, first malloc MODULE_COUNT_MAX count ITEM, and hold it until this MODULE terminate.
#define MODULE_COUNT_MAX (256) /* 256x28 = 7K */


typedef enum
{
    IOC_MODULE_CMD_SET_COUNT,
    IOC_MODULE_CMD_ADD_INFO,
    IOC_MODULE_CMD_DEL_INFO,
    IOC_MODULE_CMD_GET_INFO,

    IOC_MODULE_CMD_SET_FUNC,
    IOC_MODULE_CMD_GET_FUNC,

    IOC_MODULE_CMD_MEM_ADD,
    IOC_MODULE_CMD_MEM_DEL,

    IOC_MODULE_CMD_ALLOC_ID,

    IOC_MODULE_CMD_BUTT
}IOC_MODULE_CMD_E;

#define CMD_ADD_MODULE_INFO         _IOWR(HI_ID_MODULE, IOC_MODULE_CMD_ADD_INFO, MODULE_INFO_S)
#define CMD_ADD_Compat_MODULE_INFO  _IOWR(HI_ID_MODULE, IOC_MODULE_CMD_ADD_INFO, Compat_MODULE_INFO_S)
#define CMD_DEL_MODULE_INFO         _IOW(HI_ID_MODULE, IOC_MODULE_CMD_DEL_INFO, MODULE_INFO_S)
#define CMD_DEL_Compat_MODULE_INFO  _IOW(HI_ID_MODULE, IOC_MODULE_CMD_DEL_INFO, Compat_MODULE_INFO_S)
#define CMD_GET_MODULE_INFO         _IOWR(HI_ID_MODULE, IOC_MODULE_CMD_GET_INFO, MODULE_INFO_S)
#define CMD_GET_Compat_MODULE_INFO  _IOWR(HI_ID_MODULE, IOC_MODULE_CMD_GET_INFO, Compat_MODULE_INFO_S)
#define CMD_SET_MODULE_FUNC         _IOW(HI_ID_MODULE, IOC_MODULE_CMD_SET_FUNC, MODULE_INFO_S)
#define CMD_GET_MODULE_FUNC         _IOR(HI_ID_MODULE, IOC_MODULE_CMD_GET_FUNC, MODULE_INFO_S)
#define CMD_MEM_ADD_INFO            _IOW(HI_ID_MODULE, IOC_MODULE_CMD_MEM_ADD, MODULE_MEM_INFO_S)
#define CMD_MEM_DEL_INFO            _IOW(HI_ID_MODULE, IOC_MODULE_CMD_MEM_DEL, MODULE_MEM_INFO_S)
#define CMD_ALLOC_MODULE_ID         _IOWR(HI_ID_MODULE, IOC_MODULE_CMD_ALLOC_ID, MODULE_ALLOC_S)


#ifdef __cplusplus
}
#endif

#endif


