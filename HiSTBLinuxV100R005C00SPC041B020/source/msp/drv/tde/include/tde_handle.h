/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_handle.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                                 Created file        
******************************************************************************/
#ifndef  __TDE_HANDLE_H__
#define  __TDE_HANDLE_H__

#ifdef __cplusplus
      #if __cplusplus
extern "C"
{
      #endif
#endif /* __cplusplus */
#ifndef HI_BUILD_IN_BOOT
#include <linux/list.h>
#include <linux/mm.h>
#else
#include "higo_list.h"
#endif
#include "tde_define.h"
#include "tde_osilist.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiHandleMgr
{
    struct list_head stHeader;  /* Use to organiza handle */
    #ifndef HI_BUILD_IN_BOOT
    spinlock_t lock;
    #endif
    HI_S32   handle;
    HI_VOID *res;
} HI_HANDLE_MGR;

#define TDE_MAX_HANDLE_VALUE 0x7fffffff

HI_BOOL  initial_handle(HI_VOID);
        
HI_VOID  get_handle(HI_HANDLE_MGR *res, HI_S32 *handle);

/*****************************************************************************
* Function:      query_handle
* Description:   Query the job according to the job handle and get the job resource
* Input:         handle:job ID res:job strcut pointer
* Output:        res:job resource
* Return:        Success/fail
* Others:        none
*****************************************************************************/
HI_BOOL  query_handle(HI_S32 handle, HI_HANDLE_MGR **res);
/*****************************************************************************
* Function:      release_handle
* Description:   Delete the handle node from the global handle list according to the handle value. 
* Input:         handle:job ID 
* Output:        none
* Return:        Success/fail
* Others:        none
*****************************************************************************/
HI_BOOL  release_handle(HI_S32 handle);
/*****************************************************************************
* Function:      destroy_handle
* Description:   Free global handle list head node
* Input:          none
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID destroy_handle(HI_VOID);
#ifndef HI_BUILD_IN_BOOT
/*****************************************************************************
* Function:      TdeFreePendingJob
* Description:   Free the job which is not submitted  when execute Ctrl +C (kill the current process).
* Input:          none
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeFreePendingJob(HI_VOID);
#endif

#ifdef __cplusplus
        #if __cplusplus
}
        #endif
   #endif /* __cplusplus */
#endif /*__TDE_HANDLE_H__ */
