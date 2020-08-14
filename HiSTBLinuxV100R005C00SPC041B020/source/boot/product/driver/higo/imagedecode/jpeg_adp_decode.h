/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_adp_decode.h
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      :
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/

#ifndef  __JPEG_ADP_DECODE_H__
#define  __JPEG_ADP_DECODE_H__


/*********************************add include here******************************/


#include "hi_go_logo.h"

/***************************** Macro Definition ********************************/


/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/

/***************************** API forward declarations ************************/

/***************************** API realization *********************************/


/***************************************************************************************
* func          : JPEG_ADP_CreateDecoder
* description   : CNcomment:创建解码器 CNend\n
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 JPEG_ADP_CreateDecode(HI_U32 *pDecHandle,HI_CHAR* pSrcBuf,HI_U32 u32SrcLen);

/***************************************************************************************
* func          : JPEG_ADP_DestroyDecoder
* description   : CNcomment:销毁解码器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 JPEG_ADP_DestroyDecode(HI_U32 DecHandle);


/***************************************************************************************
* func          : JPEG_ADP_GetImgInfo
* description   : CNcomment:获取图片信息CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 JPEG_ADP_GetImgInfo(HI_U32 DecHandle, HI_HANDLE hSurface);

/***************************************************************************************
* func          : JPEG_ADP_StartDecode
* description   : CNcomment:开始解码CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 JPEG_ADP_StartDecode(HI_U32 DecHandle, HI_HANDLE hSurface);


#endif /*__JPEG_ADP_DECODE_H__ */
