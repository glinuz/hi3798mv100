/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_proc_k.c
Version             : Initial Draft
Author              :
Created             : 2015/07/11
Description         :
Function List       :             
History             :
Date                            Author                  Modification
2015/07/11                                              Created file      
******************************************************************************/
#ifndef HI_BUILD_IN_BOOT 


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "tde_proc.h"
#include "tde_config.h"
#include "hi_gfx_sys_k.h"

#ifndef CONFIG_TDE_PROC_DISABLE
typedef struct _hiTDE_PROCINFO_S
{
    HI_U32          u32CurNode;
    TDE_HWNode_S    stTdeHwNode[TDE_MAX_PROC_NUM];
    HI_BOOL         bProcEnable;
}TDE_PROCINFO_S;

TDE_PROCINFO_S g_stTdeProcInfo =
{
	.u32CurNode = 0,
	.bProcEnable = HI_TRUE,
};

HI_VOID TDEProcEnable(HI_BOOL bEnable)
{
    g_stTdeProcInfo.bProcEnable = bEnable;
}

HI_VOID TDEProcRecordNode(TDE_HWNode_S* pHWNode)
{
    if ((!g_stTdeProcInfo.bProcEnable) || (HI_NULL == pHWNode))
    {
        return;
    }
    
    HI_GFX_Memcpy(&g_stTdeProcInfo.stTdeHwNode[g_stTdeProcInfo.u32CurNode], pHWNode, sizeof(TDE_HWNode_S));

    g_stTdeProcInfo.u32CurNode++;
    g_stTdeProcInfo.u32CurNode = (g_stTdeProcInfo.u32CurNode)%TDE_MAX_PROC_NUM;
}




HI_VOID TDEProcClearNode(HI_VOID)
{
    HI_GFX_Memset(&g_stTdeProcInfo.stTdeHwNode[0], 0, sizeof(g_stTdeProcInfo.stTdeHwNode));
    g_stTdeProcInfo.u32CurNode = 0;
}

int tde_write_proc(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)  
{
    UNUSED(file);
    UNUSED(buf);
    UNUSED(count);
    UNUSED(ppos);

    return 0;
}

int tde_read_proc(struct seq_file *p, HI_VOID *v)
{
    HI_S32 j = 0;
    HI_U32* pu32Cur= NULL;
    TDE_HWNode_S *pstHwNode = NULL;
    UNUSED(v);

    if (NULL == p) 
    {
        return 0;
    }

    pstHwNode = g_stTdeProcInfo.stTdeHwNode;
    p = wprintinfo(p);
   
    for (j = 0 ; j < g_stTdeProcInfo.u32CurNode; j++)
    {
        pu32Cur = (HI_U32*)&pstHwNode[j];
        TdeHalNodePrintInfo(p,pu32Cur);     
    }
    
    return 0;
}
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */


#endif 
