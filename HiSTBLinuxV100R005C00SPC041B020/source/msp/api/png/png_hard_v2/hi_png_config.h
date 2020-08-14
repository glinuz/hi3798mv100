/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

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
#define CONFIG_PNG_PROC_DISABLE
#define CONFIG_PNG_VERSION_DISABLE
#define CONFIG_PNG_STR_DISABLE
#endif

#endif