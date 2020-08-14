/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	    : drv_jpge_ioctl.h
Version		     : Initial Draft
Author		      :
Created		     : 2016/03/03
Description	  :
Function List :


History       :
Date				         Author        		Modification
2016/03/03		    y00181162  		    Created file
******************************************************************************/
#ifndef __DRV_JPGE_IOCTL_H__
#define __DRV_JPGE_IOCTL_H__


/*********************************add include here******************************/
#include <linux/ioctl.h>
#include "hi_jpge_type.h"

/*****************************************************************************/


/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define JPGE_IOC_MAGIC            'j'

/*************************** Structure Definition ****************************/


/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/
#define JPGE_CREATE_CMD            _IOWR(JPGE_IOC_MAGIC, 100, Jpge_EncCfg_S)

#define JPGE_ENCODE_CMD            _IOWR(JPGE_IOC_MAGIC, 101, Jpge_EncInfo_S)
#define JPGE_COMPAT_ENCODE_CMD     _IOWR(JPGE_IOC_MAGIC, 101, Jpge_Compat_EncInfo_S)

#define JPGE_DESTROY_CMD           _IOW(JPGE_IOC_MAGIC,  102, JPGE_HANDLE)

#define JPGE_CREATEHANDLE_CMD      _IO(JPGE_IOC_MAGIC,   103)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__DRV_JPGE_IOCTL_H__ */
