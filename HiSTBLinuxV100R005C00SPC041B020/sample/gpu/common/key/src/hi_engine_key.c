/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_engine_key.c
Version		    : Initial Draft
Author		    :
Created		    : 2011/11/23
Description	    : the input device
Function List 	:
                :
                :


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

/*********************************add include here******************************/

/***************************** Macro Definition ******************************/

#include "hi_engine_key.h"
#include "hi_3d_config.h"

#ifdef EGL_API_DIRECTFB
 #include "hi_input_dfb.h"
#else
 #include "hi_ir_remote.h"
#endif

/******************** to see which include file we want to use***************/
#define KEY_TINIT() struct timeval tv_start, tv_end; unsigned int time_cost, line_start
#define KEY_TSTART() gettimeofday(&tv_start, NULL); line_start = __LINE__
#define KEY_TEND() \
    gettimeofday(&tv_end, NULL); \
    time_cost = ((tv_end.tv_usec - tv_start.tv_usec) + (tv_end.tv_sec - tv_start.tv_sec)); \
    HI3D_TRACE("IN FILE:%s FROM LINE: %d TO LINE: %d COST: %d us\n", __FILE__, line_start, __LINE__, time_cost)

/*************************** Structure Definition ****************************/
typedef struct
{
    ENGINE_KEYCODE_E enKey;          /* 程序中使用的键值 */
    unsigned int     IrkeyValue;       /* 遥控实际对应的键值 */
} ENGINE_KEY_MAP_S;

/********************** Global Variable declaration **************************/

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_engine_key.h **************************/

/******************************************************************************
func                     : HI_ENGINE_KEY_Init
description              : initialize the remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_ENGINE_KEY_Init()
{
#ifdef EGL_API_DIRECTFB
    HI_S32 s32RetVal = HI_FAILURE;
    s32RetVal = HI_DfbInput_Initialize();
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE(">>>> dfb input init failure >>>>> \n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

#else
    HI_S32 s32RetVal = HI_FAILURE;

    s32RetVal = HI_RemoteDev_Initialize();
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE(">>>> ir remote init failure >>>>> \n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
#endif

}

/******************************************************************************
func                     : HI_ENGINE_KEY_DeInit
description              : close the remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_ENGINE_KEY_DeInit(HI_VOID)
{
#ifdef EGL_API_DIRECTFB
    HI_S32 s32RetVal = HI_FAILURE;
    s32RetVal = HI_DfbInput_Shutdown();
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE(">>>> dfb input close failure >>>\n");
        return s32RetVal;
    }

    HI3D_TRACE(">>>>>>>>>>>> dfb input close success >>>>>>>>>\n");
    return HI_SUCCESS;

#else
    HI_S32 s32RetVal = HI_FAILURE;

    s32RetVal = HI_RemoteDev_Shutdown();
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE(">>>> ir remote close failure >>>\n");
        return s32RetVal;
    }

    HI3D_TRACE(">>>>>>>>>>>> ir  remote close success >>>>>>>>>\n");
    return HI_SUCCESS;
#endif

}

/******************************************************************************
func                     : HI_ENGINE_KEY_Get
description              : get the input of remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_ENGINE_KEY_Get(ENGINE_KEY_S *pKey)
{
#ifdef EGL_API_DIRECTFB
    HI_S32 keyvalue = 0;

    pKey->keyCode   = ENGINE_KEY_BUTT;
    pKey->keyStatus = ENGINE_KEYSTATUS_DOWN;

    keyvalue = HI_DfbInput_GetEvents();
    if (HI_FAILURE == keyvalue)
    {
        return HI_FAILURE;
    }

    pKey->keyCode = keyvalue;

    return HI_SUCCESS;

#else
    HI_S32 keyvalue = 0;

    pKey->keyCode   = ENGINE_KEY_BUTT;
    pKey->keyStatus = ENGINE_KEYSTATUS_DOWN;

    keyvalue = HI_RemoteDev_GetInput();
    if (HI_FAILURE == keyvalue)
    {
        return HI_FAILURE;
    }

    pKey->keyCode = keyvalue;

    return HI_SUCCESS;
#endif
}
