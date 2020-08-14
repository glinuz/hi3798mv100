/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_osilist.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                               Created file        
******************************************************************************/
#ifndef  __TDE_OSILIST_H__
#define  __TDE_OSILIST_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#ifndef HI_BUILD_IN_BOOT
#include <linux/list.h>
#else
#include "higo_list.h"
#endif
#include "tde_define.h"
#include "hi_tde_type.h"


/****************************************************************************/
/*                        TDE osi list types define                         */
/****************************************************************************/

/* Data struct of software list node */
typedef struct hiTDE_SWNODE_S
{
    struct list_head stList;    
    HI_S32 s32Handle;           		/* Job handle of the instruct */
    HI_S32 s32Index;            		/* Instruct serial number in job, form one on start, the same number is the same instruct */
    TDE_NODE_SUBM_TYPE_E enSubmType; 	/* current node type */
    TDE_NODE_BUF_S stNodeBuf;       	/* Node of operate config */
    HI_U32 u32PhyBuffNum;           	/* Number of physical buffer distributed */
} TDE_SWNODE_S;

/* Job definition */
typedef struct hiTDE_SWJOB_S
{
    struct list_head stList;    
    HI_S32 s32Handle;               /* Job handle */
    TDE_FUNC_CB       pFuncComplCB; /* Pointer of callback fuction */
    HI_VOID * pFuncPara;            /* Arguments of callback function */
    TDE_NOTIFY_MODE_E enNotiType;   /* Notice type after node completed */
    HI_U32 u32CmdNum;               /* Instruct number of job */
    HI_U32 u32NodeNum;              /* Node number of job */
    TDE_SWNODE_S *pstFirstCmd;      /* Software node of first instruct in job */
    TDE_SWNODE_S *pstLastCmd;       /* Software node of last instruct in job  */
    TDE_SWNODE_S *pstTailNode;      /* Last software node of job */
    #ifndef HI_BUILD_IN_BOOT
    wait_queue_head_t stQuery;      /* Wait queue used in query */
    pid_t  pid;   /* Job process ID*/
    #endif
    HI_BOOL bSubmitted;             /* If have submitted */
    HI_BOOL bAqUseBuff;             /* If using temporary buffer */
    HI_U8   u8WaitForDoneCount;     /* wait job count */        
}TDE_SWJOB_S;


/****************************************************************************/
/*                             TDE osi list functions define                */
/****************************************************************************/

/*****************************************************************************
* Function:      TdeOsiSuspListInit
* Description:   Initialize all lists inside software to use as TDE operation cache
* Input:         null
* Output:        none
* Return:        Success/fail
* Others:        none
*****************************************************************************/
HI_S32      TdeOsiListInit(HI_VOID);

/*****************************************************************************
* Function:      TdeOsiListTerm
* Description:   Release all lists inside software to use as TDE operation cache
* Input:         NULL
* Output:        none 
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID     TdeOsiListTerm(HI_VOID);

/*****************************************************************************
* Function:      TdeOsiListBeginJob
* Description:   Create list, return head pointer of list
* Input:         none
* Output:        none
* Return:        create task handle 
* Others:        none
*****************************************************************************/
HI_S32      TdeOsiListBeginJob(TDE_HANDLE *pHandle );

/*****************************************************************************
* Function:      TdeOsiListCancelJob
* Description:   Delete list of s32Handle point
* Input:         s32Handle: the list wait for delete
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_S32      TdeOsiListCancelJob(TDE_HANDLE s32Handle);


/*****************************************************************************
* Function:      TdeOsiListSubmitJob
* Description:   Create list, return head pointer of list
* Input:         s32Handle: Head pointer of job needing submit
*                bBlock: if block
*                u32TimeOut: time out
*                pFuncComplCB: callback fuction of complete operate
* Output:        none 
* Return:        success/fail/time out
* Others:        none
*****************************************************************************/
HI_S32      TdeOsiListSubmitJob(TDE_HANDLE s32Handle, 
                                HI_U32 u32TimeOut, TDE_FUNC_CB pFuncComplCB, HI_VOID *pFuncPara,
                                TDE_NOTIFY_MODE_E enNotiType);

/*****************************************************************************
 Prototype    : TdeOsiListWaitAllDone
 Description  : wait for all TDE operate completed
 Input        : None
 Output       : NONE
 Return Value : HI_SUCCESS,TDE operate completed
 Calls        :
 Called By    :
*****************************************************************************/
#ifndef HI_BUILD_IN_BOOT
HI_S32 TdeOsiListWaitAllDone(HI_VOID);

/*****************************************************************************
 Prototype    : TdeOsiListReset
 Description  : Reset all state, release list space 
 Input        : none
 Output       : NONE
 Return Value : HI_SUCCESS/HI_FAILURE
 Calls        :
 Called By    :
*****************************************************************************/
HI_VOID TdeOsiListReset(HI_VOID);

/*****************************************************************************
* Function:      TdeOsiListWaitForDone
* Description:   Query if submitted TDE operate is completed
* Input:         s32Handle: task handle 
*                u32TimeOut: if Time out 
* Output:        none
* Return:        Success/fail
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiListWaitForDone(TDE_HANDLE s32Handle, HI_U32 u32TimeOut);


/*****************************************************************************
* Function:      TdeOsiSqCompProc
* Description:   Interrupt to handling all list operate is completed
* Input:         none
* Output:        none
* Return:        task handle is created
* Others:        none
*****************************************************************************/
HI_VOID     TdeOsiListCompProc(HI_VOID);
#ifdef TDE_HWC_COOPERATE
HI_VOID TdeOsiListComp(HI_VOID);
#endif
/*****************************************************************************
* Function:      TdeOsiNodeComp
* Description:
* Input:         none
* Output:        none
* Return:        task handle is created
* Others:        none
*****************************************************************************/
HI_VOID     TdeOsiListNodeComp(HI_VOID);
#endif

/*****************************************************************************
* Function:      TdeOsiListGetPhyBuff
* Description:   Get one physical buffer, used in deflicker and zoom
* Input:         
* Output:        none
* Return:        physical address assigned
* Others:        none
*****************************************************************************/
HI_U32  TdeOsiListGetPhyBuff(HI_U32 u32CbCrOffset);


/*****************************************************************************
* Function:      TdeOsiListPutPhyBuff
* Description:   put back physical buffer
* Input:         u32BuffNum 
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID  TdeOsiListPutPhyBuff(HI_U32 u32BuffNum);

/*****************************************************************************
* Function:      TdeOsiListFreeSerialCmd
* Description:   FreeSerialCmd
* Input:         FirstCmd 
                     LastCmd
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TdeOsiListFreeSerialCmd(TDE_SWNODE_S *pstFstCmd, TDE_SWNODE_S *pstLastCmd);

HI_S32 TdeOsiListLockWorkingFlag(HI_SIZE_T *lock);

HI_S32 TdeOsiListUnlockWorkingFlag(HI_SIZE_T *lock);

HI_S32 TdeOsiListGetWorkingFlag(HI_BOOL *bFlag);




#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif  /* _TDE_OSILIST_H_ */
