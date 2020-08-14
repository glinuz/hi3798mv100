/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	      : drv_jpge_ext.h
Version		       : Initial Draft
Author		        :
Created		       : 2016/03/03
Description	    :
Function List  	:


History       	 :
Date			         	Author        		Modification
2016/03/03		    y00181162  		    Created file
******************************************************************************/
#ifndef __DRV_JPGE_EXT_H__
#define __DRV_JPGE_EXT_H__


/*********************************add include here******************************/
#include "hi_drv_dev.h"
#include "hi_jpge_type.h"


/*****************************************************************************/


/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/
typedef HI_VOID  (*FN_JPGE_Init)        (HI_VOID);
typedef HI_VOID  (*FN_JPGE_DeInit)      (HI_VOID);
typedef HI_S32   (*FN_JPGE_CreateChn)   (HI_S32  *pEncHandle, Jpge_EncCfg_S *pEncCfg );
typedef HI_S32   (*FN_JPGE_EncodeFrame) (HI_S32  EncHandle,   Venc2Jpge_EncIn_S *pEncIn, HI_BOOL *pBufferFull);
typedef HI_S32   (*FN_JPGE_DestroyChn)  (HI_S32  EncHandle );
typedef HI_S32   (*FN_JPGE_Suspend)     (PM_BASEDEV_S *, pm_message_t);
typedef HI_S32   (*FN_JPGE_Resume)      (PM_BASEDEV_S *);

typedef struct
{
	   FN_JPGE_Init		      	pfnJpgeInit;
	   FN_JPGE_DeInit		     pfnJpgeDeInit;
	   FN_JPGE_CreateChn		  pfnJpgeCreateChn;
    FN_JPGE_EncodeFrame		pfnJpgeEncodeFrame;
	   FN_JPGE_DestroyChn		 pfnJpgeDestroyChn;
	   FN_JPGE_Suspend			   pfnJpgeSuspend;
	   FN_JPGE_Resume			    pfnJpgeResume;
}JPGE_EXPORT_FUNC_S;

HI_S32  JPGE_DRV_ModInit(HI_VOID);
HI_VOID JPGE_DRV_ModExit(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__DRV_JPGE_EXT_H__ */
