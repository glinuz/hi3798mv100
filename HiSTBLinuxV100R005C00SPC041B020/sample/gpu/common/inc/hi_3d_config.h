/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_3d_config.h
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    : config the gao an macro and other CFLASS
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_3D_CONFIG__
#define __HI_3D_CONFIG__

/*********************************add include here******************************/

/*****************************************************************************/

/*****************************************************************************/

#ifdef __cplusplus
 #if __cplusplus

extern "C"
{
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#ifdef CONFIG_SUPPORT_CA_RELEASE

 #define GRAPHICS_ADVCA_VERSION

 #define HI3D_TRACE(fmt, args... )

#else

 #ifdef HI3D_DEBUG

  #ifdef __KERNEL__
   #define HI3D_TRACE(fmt, args... ) \
    do { \
        printk(fmt, ## args ); \
    } while (0)
  #else
   #define HI3D_TRACE(fmt, args... ) \
    do { \
        printf("%s\n %s(): %d Line\n: ", __FILE__, __FUNCTION__, __LINE__ ); \
        printf(fmt, ## args ); \
    } while (0)
  #endif

 #else
  #define HI3D_TRACE(fmt, args... )

 #endif

#endif

#if 0
 #ifdef ADVCA_SUPPORT
  #define __INIT__
  #define __EXIT__
 #else
  #define __INIT__ __init
  #define __EXIT__ __exit
 #endif
#endif

/*************************** Structure Definition ****************************/

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

#ifdef __cplusplus

 #if __cplusplus
}

 #endif

#endif    /* __cplusplus */

#endif /* __HI_3D_CONFIG__*/
