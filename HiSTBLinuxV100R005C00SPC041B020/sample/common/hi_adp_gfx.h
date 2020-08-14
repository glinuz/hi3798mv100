/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_adp_gfx.h
Version             : Initial Draft
Author              : 
Created             : 2016/07/18
Description         : display initial
Function List       : 

                                          
History             :
Date                        Author                  Modification
2016/07/18                  y00181162               Created file        
******************************************************************************/

#ifndef __HI_ADP_GFX_H__
#define __HI_ADP_GFX_H__


/*********************************add include here******************************/
#include <time.h>
#include <sys/time.h>

#include <directfb.h>
#include "hi_unf_disp.h"
#include "hi_adp_hdmi.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif /* __cplusplus */



/***************************** Macro Definition ******************************/
#ifdef HI_ADVCA_FUNCTION_RELEASE
#define SAMPLE_TRACE( fmt, args... )\
        do{}while (0)
#else
#define SAMPLE_TRACE( fmt, args... )\
        do { \
              fprintf(stderr,fmt, ##args );\
        } while (0)

#endif


#ifndef HI_ADVCA_FUNCTION_RELEASE
#define DF_TINIT()   struct timeval tv_start, tv_end; unsigned int time_cost,line_start
#define DF_TSTART()  gettimeofday(&tv_start, NULL);line_start = __LINE__
#define DF_TEND()    gettimeofday(&tv_end, NULL); \
                     time_cost = (   (tv_end.tv_usec - tv_start.tv_usec)                    \
                                   + (tv_end.tv_sec - tv_start.tv_sec)*1000000);            \
                     SAMPLE_TRACE("=======================================================\n"); \
                     SAMPLE_TRACE("FROM LINE: %d TO LINE: %d COST: %d us = %d ms\n",            \
                                          line_start, __LINE__, time_cost,time_cost/1000);  \
                     SAMPLE_TRACE("=======================================================\n")

#else
#define DF_TINIT()\
        do {} while (0)
#define DF_TSTART()\
    do {} while (0)
#define DF_TEND()\
    do {} while (0)
#endif


#define  SCREEN_WIDTH        1280
#define  SCREEN_HEIGHT       720
#define  DISPLAY_FMT         HI_UNF_ENC_FMT_720P_50


#define PIC0_PATH		     "./../share/directfb-examples/res/logo.jpg"
#define	PIC1_PATH		     "./../share/directfb-examples/res/test.jpg"
#define	PIC2_PATH		     "./../share/directfb-examples/res/desktop.png"
#define PIC3_PATH            "./../share/directfb-examples/res/dfblogo.png"
#define	COLOR_BYTES		     4

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_VOID df_printSurfaceProp(IDirectFBSurface * pSurface);

HI_VOID df_clearScreen(IDirectFBDisplayLayer  *layer);

HI_S32 df_dec_to_surface(const char* pcPicPath,HI_U32 u32Width,HI_U32 u32Height,IDirectFB *dfb, IDirectFBSurface **Surface);



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __HI_ADP_GFX_H__*/
