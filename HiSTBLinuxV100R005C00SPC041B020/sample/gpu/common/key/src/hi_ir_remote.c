/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_ir_remote.c
Version		    : Initial Draft
Author		    :
Created		    : 2011/11/23
Description	    : the remote device
Function List 	: HI_RemoteDev_Initialize
                : HI_RemoteDev_Shutdown
                : HI_RemoteDev_GetInput


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef EGL_API_DIRECTFB

/*********************************add include here******************************/

#include "hi_ir_remote.h"
#include "hi_3d_config.h"

/***************************** Macro Definition ******************************/

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

typedef struct
{
    unsigned int CommonValue;
    unsigned int IrkeyValue;
} HI_adp_KEY_MAP_S;

/********************** Global Variable declaration **************************/

/**
** 运行 /home/y00181162/Develop/sample/ecs/sample_ir,然后按遥控器就可以知道键值
**/

static HI_adp_KEY_MAP_S s_PanIRMap[] =
{
    {HI_ADP_KEY_UP,    0x35caff00, /*   上   */        },
    {HI_ADP_KEY_LEFT,  0x6699ff00, /*   左   */        },
    {HI_ADP_KEY_RIGHT, 0x3ec1ff00, /*   右   */        },
    {HI_ADP_KEY_DOWN,  0x2dd2ff00, /*   下   */        },
    {HI_ADP_KEY_ENTER, 0x31ceff00, /*   OK   */        },
    {HI_ADP_KEY_DEL,	0x7788ff00 /*  DEL键 */        },
    {HI_ADP_KEY_BACK,  0x6f90ff00, /*  Back  */        },
    {HI_ADP_KEY_F2,    0x7689ff00, /*  red   */        },
    {HI_ADP_KEY_F1,    0x7b84ff00, /*  grn   */        },
    {HI_ADP_KEY_F3,    0x26d9ff00, /*  ylw   */        },
    {HI_ADP_KEY_F4,    0x6996ff00, /*  blu   */        },
    {HI_ADP_KEY_SET,   0x6a95ff00,                     },
    {HI_ADP_KEY_SUB,   0x738cff00,                     },
    {HI_ADP_KEY_SEEK,  0x7d82ff00,                     },
    {HI_ADP_KEY_INFO,  0x38c7ff00,                     },
    {HI_ADP_KEY_POWER, 0x639cff00,                     }
};

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_ir_remote.h **************************/

/******************************************************************************
func                     : HI_RemoteDev_Initialize
description              : initialize the remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_RemoteDev_Initialize(HI_VOID)
{
    HI_S32 s32RetVal = HI_FAILURE;

    s32RetVal = HI_UNF_IR_Open();
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32RetVal);
        return s32RetVal;
    }

    /** enable key up **/
    s32RetVal = HI_UNF_IR_EnableKeyUp(HI_TRUE);
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE("EnableKeyUp failed!\n");
        HI_UNF_IR_Close();
        return s32RetVal;
    }

    /** set repeat timeout **/
    s32RetVal = HI_UNF_IR_SetRepKeyTimeoutAttr(108);
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE("SetRepKeyTimeoutAttr failed!\n");
        HI_UNF_IR_Close();
        return s32RetVal;
    }

    s32RetVal = HI_UNF_IR_EnableRepKey(HI_TRUE);
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE("enable RepKey failed!\n");
        HI_UNF_IR_Close();
        return s32RetVal;
    }

    /** enable the remote device **/
    s32RetVal = HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE("enable ir failed!\n");
        HI_UNF_IR_Close();
        return s32RetVal;
    }

    return HI_SUCCESS;
}

/******************************************************************************
func                     : HI_RemoteDev_Shutdown
description              : close the remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_RemoteDev_Shutdown(HI_VOID)
{
    HI_S32 s32RetVal = HI_FAILURE;

    s32RetVal = HI_UNF_IR_Close();
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE("device file closed fail!\n");
        return s32RetVal;
    }

    return HI_SUCCESS;
}

/******************************************************************************
func                     : HI_RemoteDev_GetInput
description              : get the input of remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_RemoteDev_GetInput(HI_VOID)
{
    HI_U64 u64KeyId;
    HI_UNF_KEY_STATUS_E PressStatus;
    HI_U32 s_IMKeyCount = sizeof(s_PanIRMap) / sizeof(HI_adp_KEY_MAP_S);
    HI_U8 i;

    /** 只接受按下键 **/
    if (HI_SUCCESS != HI_UNF_IR_GetValue(&PressStatus, &u64KeyId, 0))
    {
        return HI_FAILURE;
    }

    if (HI_UNF_KEY_STATUS_DOWN != PressStatus)
    {
        return HI_FAILURE;
    }

    for (i = 0; i < s_IMKeyCount; i++)
    {
        if (s_PanIRMap[i].IrkeyValue == u64KeyId)
        {
            break;
        }
    }

    return (int)(s_PanIRMap[i].CommonValue);
}

#endif /** #ifndef EGL_API_DIRECTFB **/
