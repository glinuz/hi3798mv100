/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	    : hi_png_config.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/05/14
Description	    : config the gao an macro and other CFLASS
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2011/11/23		            		    Created file      	
******************************************************************************/
#ifndef __PNG_CONFIG_H__
#define __PNG_CONFIG_H__

#define CONFIG_PNG_PREMULTIALPHA_ENABLE  

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define CONFIG_PNG_DEBUG_DISABLE
//#define CONFIG_PNG_PROC_DISABLE
#define CONFIG_PNG_VERSION_DISABLE
#define CONFIG_PNG_STR_DISABLE
#endif

#define CONFIG_PNG_USE_SDK_CRG_ENABLE

#endif
