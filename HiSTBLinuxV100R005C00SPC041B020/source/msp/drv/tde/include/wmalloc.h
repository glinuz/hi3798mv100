/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : wmalloc.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                 y00181162           Created file        
******************************************************************************/
#ifndef  __WMALLOC_H__
#define  __WMALLOC_H__


#ifdef __cplusplus
      #if __cplusplus
extern "C"
{
      #endif
#endif /* __cplusplus */

#include "hi_type.h"

/*CNcomment:*****************************************************************************
*Function: wmeminit
*Description: init memery pool. CNcomment:TDE内存池初始化
*Input: none
*Output: none
*Return: success/error code. CNcomment:成功/错误码
others: none
*******************************************************************************/
HI_S32 wmeminit(void);

/******************************************************************************
*Function: wmemterm
*Description: Deinit memery pool. CNcomment:TDE内存池去初始化
*Input: none
*Output: none
*Return: none
others: none
*******************************************************************************/
HI_VOID wmemterm(void);

/******************************************************************************
*Function: wgetphy
*Description: get the physics address of memery. CNcomment:获取内存的物理地址
*Input: ptr  the pointer of memery. CNcomment:指向内存的指针
*Output:  physics address. CNcomment:物理地址
*Return: none
others: none
*******************************************************************************/
HI_U32 wgetphy(HI_VOID *ptr);

/******************************************************************************
*Function: wgetphy
*Description: get the virtual address of memery. CNcomment:获取内存的虚拟地址
*Input: phyaddr  physics address. CNcomment:物理地址
*Output: virtual address. CNcomment:虚拟地址
*Return: none
others: none
*******************************************************************************/
HI_VOID * wgetvrt(HI_U32 phyaddr);

/******************************************************************************
*Function: wgetfreenum
*Description:get the number of the free unit. CNcomment:获取内存的剩余的Unit单元数
*Input: none
*Output: none
*Return: the number of the free unit. CNcomment:剩余的单元数
others: none
*******************************************************************************/
HI_U32 wgetfreenum(HI_VOID);
#ifndef TDE_BOOT
struct seq_file * wprintinfo(struct seq_file *page);
#endif


#ifdef __cplusplus
        #if __cplusplus
}
        #endif
   #endif /* __cplusplus */
#endif /*__WMALLOC_H__ */
