/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_boot_logo.h
Version             : Initial Draft
Author              : 
Created             : 2015/07/02
Description         : 
                       CNcomment: logo头文件 CNend\n
Function List   : 
History         :
Date                        Author                  Modification
2015/07/02                                          modiy by y00181162     
******************************************************************************/
#ifndef __HI_BOOT_LOGO_H__
#define __HI_BOOT_LOGO_H__


/*********************************add include here******************************/
#include "hi_common.h"
#include "hi_flash.h"
#include "hi_unf_disp.h"

#ifdef HI_LOADER_APPLOADER
#include "hi_go_gdev.h"
#include "hi_go_decoder.h"
#include "hi_go_bliter.h"
#endif

#include "hi_drv_pdm.h"
/***************************** Macro Definition ******************************/

#define HI_ERR_MCE(format...)     HI_ERR_PRINT(HI_ID_FASTPLAY, format)
#define HI_INFO_MCE(format...)    HI_INFO_PRINT(HI_ID_FASTPLAY, format)

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/

/***************************************************************************************
* func          : HI_LOGO_DecImg
* description   : CNcomment: 图片解码 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HI_LOGO_DecImg(HI_LOGO_PARAM_S stLogoParam, HI_U32 u32LogoDataAddr, HI_HANDLE *hSurface);

#endif /*__HI_BOOT_LOGO_H__ */
