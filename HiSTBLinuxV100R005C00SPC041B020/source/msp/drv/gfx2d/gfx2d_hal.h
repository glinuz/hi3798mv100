/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_hal.h
* Description: Graphic 2D engine hal interface declaration.
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#include <linux/fs.h>

#include "hi_gfx2d_type.h"
#include "gfx2d_config.h"

#ifndef _GFX2D_HAL_H_
#define _GFX2D_HAL_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

typedef enum tagGFX2D_HAL_DEV_TYPE_E
{
    GFX2D_HAL_DEV_TYPE_HWC = 0x0,
} GFX2D_HAL_DEV_TYPE_E;

/*向硬件提交节点*/
typedef HI_S32 (*GFX2D_HAL_SubNode_FN_PTR)(HI_GFX2D_DEV_ID_E enDevId, HI_VOID *pNode);

/*将两个节点按照硬件要求链接成链表*/
typedef HI_S32 (*GFX2D_HAL_LinkNode_FN_PTR)(HI_VOID *pCurNode, HI_VOID *pNextNode);

/*设置节点ID*/
typedef HI_VOID (*GFX2D_HAL_SetNodeID_FN_PTR)(HI_VOID *pNode, HI_U32 u32NodeId);

/*释放节点*/
typedef HI_VOID (*GFX2D_HAL_FreeNode_FN_PTR)(HI_VOID *pNode);

/*中断处理*/
typedef HI_VOID (*GFX2D_HAL_NodeIsr_FN_PTR)(HI_VOID *pNode);

/*中断处理*/
typedef HI_VOID (*GFX2D_HAL_AllNodeIsr_FN_PTR)(HI_VOID);

/*硬件节点操作函数集*/
typedef struct tagGFX2D_HAL_NODE_OPS_S
{
    GFX2D_HAL_SubNode_FN_PTR pfnSubNode;
    GFX2D_HAL_LinkNode_FN_PTR pfnLinkNode;
    GFX2D_HAL_SetNodeID_FN_PTR pfnSetNodeID;
    GFX2D_HAL_FreeNode_FN_PTR pfnFreeNode;
    GFX2D_HAL_NodeIsr_FN_PTR pfnNodeIsr;
    GFX2D_HAL_AllNodeIsr_FN_PTR pfnAllNodeIsr;
} GFX2D_HAL_NODE_OPS_S;

/*******************************************************************************
* Function:      GFX2D_HAL_Init
* Description:
* Input:         in_para1     parameter description
*                in_para2     parameter description
* Output:        out_para1    parameter description
*                out_para2    parameter description
* Return:        HI_OK:                         success
*                HI_ERR_SYSM_NOMEMORY:          Allocate memory fail.
*                HI_ERR_SYSM_NODISKSPACE:       Disk space full.
*                HI_ERR_SYSM_FILEWRITE:         File write error.
* Others:        in_para1 should be none-zero
*******************************************************************************/
HI_S32 GFX2D_HAL_Init(HI_VOID);

/*******************************************************************************
* Function:      GFX2D_HAL_Deinit
* Description:
* Input:         in_para1     parameter description
*                in_para2     parameter description
* Output:        out_para1    parameter description
*                out_para2    parameter description
* Return:        HI_OK:                         success
*                HI_ERR_SYSM_NOMEMORY:          Allocate memory fail.
*                HI_ERR_SYSM_NODISKSPACE:       Disk space full.
*                HI_ERR_SYSM_FILEWRITE:         File write error.
* Others:        in_para1 should be none-zero
*******************************************************************************/
HI_S32 GFX2D_HAL_Deinit(HI_VOID);

/*******************************************************************************
* Function:      GFX2D_HAL_Deinit
* Description:
* Input:         in_para1     parameter description
*                in_para2     parameter description
* Output:        out_para1    parameter description
*                out_para2    parameter description
* Return:        HI_OK:                         success
*                HI_ERR_SYSM_NOMEMORY:          Allocate memory fail.
*                HI_ERR_SYSM_NODISKSPACE:       Disk space full.
*                HI_ERR_SYSM_FILEWRITE:         File write error.
* Others:        in_para1 should be none-zero
*******************************************************************************/
HI_S32 GFX2D_HAL_Open(HI_VOID);

/*******************************************************************************
* Function:      GFX2D_HAL_Deinit
* Description:
* Input:         in_para1     parameter description
*                in_para2     parameter description
* Output:        out_para1    parameter description
*                out_para2    parameter description
* Return:        HI_OK:                         success
*                HI_ERR_SYSM_NOMEMORY:          Allocate memory fail.
*                HI_ERR_SYSM_NODISKSPACE:       Disk space full.
*                HI_ERR_SYSM_FILEWRITE:         File write error.
* Others:        in_para1 should be none-zero
*******************************************************************************/
HI_S32 GFX2D_HAL_Close(HI_VOID);

/*******************************************************************************
* Function:      GFX2D_HAL_Compose
* Description:
* Input:         in_para1     parameter description
*                in_para2     parameter description
* Output:        out_para1    parameter description
*                out_para2    parameter description
* Return:        HI_OK:                         success
*                HI_ERR_SYSM_NOMEMORY:          Allocate memory fail.
*                HI_ERR_SYSM_NODISKSPACE:       Disk space full.
*                HI_ERR_SYSM_FILEWRITE:         File write error.
* Others:        in_para1 should be none-zero
*******************************************************************************/
HI_S32 GFX2D_HAL_Compose(const HI_GFX2D_DEV_ID_E enDevId,
                         const HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                         const HI_GFX2D_SURFACE_S *pstDstSurface,
                         HI_VOID **ppNode,
                         GFX2D_HAL_DEV_TYPE_E *penNodeType);

#ifndef GFX2D_PROC_UNSUPPORT
/*******************************************************************************
* Function:      GFX2D_HAL_ReadProc
* Description:
* Input:         in_para1     parameter description
*                in_para2     parameter description
* Output:        out_para1    parameter description
*                out_para2    parameter description
* Return:        HI_OK:                         success
*                HI_ERR_SYSM_NOMEMORY:          Allocate memory fail.
*                HI_ERR_SYSM_NODISKSPACE:       Disk space full.
*                HI_ERR_SYSM_FILEWRITE:         File write error.
* Others:        in_para1 should be none-zero
*******************************************************************************/
HI_VOID GFX2D_HAL_ReadProc(struct seq_file *p, HI_VOID *v);

/*******************************************************************************
* Function:      GFX2D_HAL_WriteProc
* Description:
* Input:         in_para1     parameter description
*                in_para2     parameter description
* Output:        out_para1    parameter description
*                out_para2    parameter description
* Return:        HI_OK:                         success
*                HI_ERR_SYSM_NOMEMORY:          Allocate memory fail.
*                HI_ERR_SYSM_NODISKSPACE:       Disk space full.
*                HI_ERR_SYSM_FILEWRITE:         File write error.
* Others:        in_para1 should be none-zero
*******************************************************************************/
HI_S32 GFX2D_HAL_WriteProc(struct file *file, const char __user *buf,
                           size_t count, loff_t *ppos);
#endif

HI_VOID GFX2D_HAL_GetNodeOps(const GFX2D_HAL_DEV_TYPE_E enDevType,
                             GFX2D_HAL_NODE_OPS_S **ppstNodeOps);

HI_S32 GFX2D_HAL_GetIntStatus(const GFX2D_HAL_DEV_TYPE_E enDevType);

HI_U32 GFX2D_HAL_GetIsrNum(const HI_GFX2D_DEV_ID_E enDevId,
                           const GFX2D_HAL_DEV_TYPE_E enDevType);

HI_U32 GFX2D_HAL_GetBaseAddr(const HI_GFX2D_DEV_ID_E enDevId,
                             const GFX2D_HAL_DEV_TYPE_E enDevType);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif  /*_GFX2D_HAL_H_*/
