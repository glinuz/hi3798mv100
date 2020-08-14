/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_proc.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                  y00181162               Created file        
******************************************************************************/

#ifndef  __TDE_PROC_H__
#define  __TDE_PROC_H__


#ifdef __cplusplus
      #if __cplusplus
extern "C"
{
      #endif
#endif /* __cplusplus */

#include "tde_hal.h"

#ifndef CONFIG_TDE_PROC_DISABLE

#define TDE_MAX_PROC_NUM 8
/*****************************************************************************
* Function:      TDEProcRecordNode
* Description:   Record TDE Node configure information
* Input:         null
* Output:        none
* Return:        none
* Others:        none
*****************************************************************************/
HI_VOID TDEProcRecordNode(TDE_HWNode_S* pHWNode);

int tde_read_proc(struct seq_file *p, HI_VOID *v);

int tde_write_proc(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos);  


#endif

#ifdef __cplusplus
        #if __cplusplus
	}
        #endif
   #endif /* __cplusplus */
#endif /*__TDE_PROC_H__ */
