/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : hi_gfx_sys.h
Version          : version 1.0
Author           :
Created          : 2016/08/30
Description      : define system function
Function List    :

History          :
Date                Author                Modification
2016/08/30          y00181162
******************************************************************************/
#ifndef  _HI_GFX_SYS_H_
#define  _HI_GFX_SYS_H_


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

/*********************************add include here******************************/

/***************************** Macro Definition ********************************/
#define HI_GFX_Memcpy(dst,src,size)                          memcpy(dst, src, size);
#define HI_GFX_Memset(dst,value,size)                        memset(dst, value, size);

/*************************** Enum Definition ***********************************/

/*************************** Structure Definition ******************************/


/********************** Global Variable declaration ****************************/


/******************************* API declaration *******************************/
#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */
#endif /*_HI_GFX_SYS_H_ */
