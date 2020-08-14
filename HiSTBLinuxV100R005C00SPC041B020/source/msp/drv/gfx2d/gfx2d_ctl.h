/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_ctl.h
* Description: Graphic 2D engine ctl interface declaration.
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#ifndef _GFX2D_CTL_H_
#define _GFX2D_CTL_H_

#include "gfx2d_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/*******************************************************************************
* Function:      GFX2D_CTL_Init
* Description:   ctl层初始化
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
HI_S32 GFX2D_CTL_Init(HI_VOID);

/*******************************************************************************
* Function:      GFX2D_CTL_Deinit
* Description:   ctl层去初始化
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
HI_S32 GFX2D_CTL_Deinit(HI_VOID);

/*******************************************************************************
* Function:      GFX2D_CTL_Open
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
HI_S32 GFX2D_CTL_Open(HI_VOID);

/*******************************************************************************
* Function:      GFX2D_CTL_Close
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
HI_S32 GFX2D_CTL_Close(HI_VOID);


/*******************************************************************************
* Function:      GFX2D_CTL_ComposeSync
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
HI_S32 GFX2D_CTL_Compose(const HI_GFX2D_DEV_ID_E enDevId,
                         HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                         HI_GFX2D_SURFACE_S *pstDstSurface,
                         HI_BOOL bSync,
                         const HI_U32 u32Timeout);

/*******************************************************************************
* Function:      GFX2D_CTL_WaitAllDone
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
HI_S32 GFX2D_CTL_WaitAllDone(const HI_GFX2D_DEV_ID_E enDevId,
                             const HI_U32 u32Timeout);

/*******************************************************************************
* Function:      GFX2D_CTL_ReadProc
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
HI_S32 GFX2D_CTL_ReadProc(struct seq_file *p, HI_VOID *v);


/*******************************************************************************
* Function:      GFX2D_CTL_WriteProc
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
HI_S32 GFX2D_CTL_WriteProc(struct file *file, const char __user *buf, \
                           size_t count, loff_t *ppos);

HI_S32 GFX2D_CTL_Resume(HI_VOID);

HI_BOOL GFX2D_CTL_CheckOpen(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif  /*_GFX2D_CTL_H_*/
