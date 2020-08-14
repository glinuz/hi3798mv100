/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_input_dfb.c
Version		    : Initial Draft
Author		    :
Created		    : 2011/11/23
Description	    : the mouse device
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifdef EGL_API_DIRECTFB

/*********************************add include here******************************/

#include "hi_input_dfb.h"
#include "directfb_keynames.h"
#include "hi_3d_config.h"

/***************************** Macro Definition ******************************/

//#define NEED_USE_MOUSE

#define DFBCHECK(x...) \
    {                                                                     \
        err = x;                                                       \
        if (err != DFB_OK) {                                           \
            fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );   \
            DirectFBErrorFatal( # x, err );                           \
        }                                                              \
    }

#define GetLowAndHigh(x, low, high) \
    (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

typedef struct _tagMOUSE_DEVICE_INFO MOUSE_DEVICE_INFO;
struct _tagMOUSE_DEVICE_INFO
{
    DFBInputDeviceID             device_id;
    DFBInputDeviceDescription    desc;
    MOUSE_DEVICE_INFO           *next;
};

/********************** Global Variable declaration **************************/

/** 输入设备 **/
static MOUSE_DEVICE_INFO  *s_devices = NULL;

/* input interfaces: device and its buffer */
static IDirectFBEventBuffer *s_event;
static HI_S32 s_MouseX = 0;
static HI_S32 s_MouseY = 0;

static IDirectFB             *s_pdfb   = NULL;
static IDirectFBWindow       *s_window = NULL;
static DFBWindowID s_windowID = 0;
static IDirectFBEventBuffer  *s_EventBuffer = NULL;
static SCREEN_INFO s_ScreenInfo;
static NativeWindowType s_windowsurface = 0;

/******************************* API forward declarations *******************/

#if 0
static DFBEnumerationResult    enum_input_device(DFBInputDeviceID          device_id,
                                                 DFBInputDeviceDescription    desc,
                                                 void                        *data );

static const char *            get_device_name(MOUSE_DEVICE_INFO  *devices,
                                               DFBInputDeviceID                device_id );

#endif

static DFBInputDeviceTypeFlags get_device_type(MOUSE_DEVICE_INFO   *devices,
                                               DFBInputDeviceID     device_id );

static HI_VOID                 process_mouse_event( DFBInputEvent *evt );

/******************************* API realization *****************************/

/**************************** realize hi_mouse.h **************************/

/******************************************************************************
func                     : HI_Input_GetWindowHandle
description              : this window property are geted after create egl window
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_Input_GetWindowHandle( IDirectFB              *pdfb,
                                 IDirectFBWindow        *window,
                                 DFBWindowID             windowID,
                                 SCREEN_INFO             screeninfo,
                                 NativeWindowType        windowsurface)
{
    s_pdfb       = pdfb;
    s_window     = window;
    s_windowID   = windowID;
    s_ScreenInfo = screeninfo;
    s_windowsurface = windowsurface;
    return HI_SUCCESS;
}

/******************************************************************************
func                     : HI_DfbInput_Initialize
description              :
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_DfbInput_Initialize(HI_VOID)
{
    if ((NULL == s_pdfb)
       && (0 != s_ScreenInfo.s32offsetX)
       && (0 != s_ScreenInfo.s32offsetY)
       && (0 != s_ScreenInfo.s32ScreenWidth)
       && (0 != s_ScreenInfo.s32ScreenHeight)

    )
    {
        HI3D_TRACE(">> the dfb window initial failure >> \n");
        return HI_FAILURE;
    }

    s_window->CreateEventBuffer( s_window, &s_EventBuffer);
    if (NULL == s_EventBuffer)
    {
        HI3D_TRACE("<< ctreat dfb event buffer failure << \n");
        return HI_FAILURE;
    }

    s_window->AttachEventBuffer( s_window, s_EventBuffer );

    return HI_SUCCESS;
}

/******************************************************************************
func                     : HI_DfbInput_Shutdown
description              :
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_DfbInput_Shutdown(HI_VOID)
{
    if (NULL != s_EventBuffer)
    {
        s_EventBuffer->Release( s_EventBuffer );
    }

    return HI_SUCCESS;
}

/******************************************************************************
func                     : HI_DfbProcess_Input_Events
description              :
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
static HI_S32 HI_DfbProcess_Input_Events(DFBWindowEvent*    pEvt,
                                         IDirectFBWindow**  pActiveWindow)
{
    static IDirectFBWindow* ActiveWindow = NULL;

    s_EventBuffer->WaitForEventWithTimeout( s_EventBuffer, 0, 10 );

    /** 这个地方获取的事件有点问题 **/
    while (s_EventBuffer->GetEvent( s_EventBuffer, DFB_EVENT(pEvt)) == DFB_OK)
    {
        if (pEvt->window_id == s_windowID)
        {
            /** 判断鼠标响应在哪个window上，假如有多个window **/
            ActiveWindow = s_window;
        }

        *pActiveWindow = ActiveWindow;
    }

    return HI_SUCCESS;
}

/******************************************************************************
func                     : HI_DfbInput_GetEvents
description              : get the key of remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_DfbInput_GetEvents(HI_VOID)
{
    DFBWindowEvent evt;
    IDirectFBWindow  *pActiveWindow = NULL;
    HI_S32 s32RetVal = HI_FAILURE;
    HI_S32 s32KeyVal = -1;
    static HI_S32 s32Count = 0;

    HI_BOOL IR_KEYBOARD = HI_FALSE;

    /** 二重指针的运用，要想能够获得该值，必须通过二重指针传入 **/
    evt.key_id = 0;

    s32RetVal = HI_DfbProcess_Input_Events(&evt, &pActiveWindow);
    if (HI_SUCCESS != s32RetVal)
    {
        HI3D_TRACE("<< get the dfb event failure ! <<\n");
        return s32RetVal;
    }

    if ((NULL == pActiveWindow) && (0 == s32Count))
    {
        s32Count = 1;
        HI3D_TRACE("<< get the active window failure ! <<\n");
    }
    else
    {
        switch (evt.type)
        {
#ifdef NEED_USE_MOUSE
        case DWET_BUTTONDOWN:                /** 鼠标按下 **/
            if (evt.button == DIBI_LEFT)
            {
                s_MouseX = evt.cx;
                s_MouseY = evt.cy;
            }

            break;

        case DWET_BUTTONUP:                /** 鼠标弹起 **/
            switch (evt.button)
            {
            case DIBI_LEFT:
                break;
            case DIBI_MIDDLE:
                break;
            case DIBI_RIGHT:
                break;
            default:
                break;
            }

            break;
#endif
        case DWET_KEYDOWN:
            switch (evt.key_id)
            {
                /** 遥控器和键盘操作 **/
            case DIKI_RIGHT:
                s32KeyVal = HI_ADP_KEY_RIGHT;
                break;
            case DIKI_LEFT:
                s32KeyVal = HI_ADP_KEY_LEFT;
                break;
            case DIKI_UP:
                s32KeyVal = HI_ADP_KEY_UP;
                break;
            case DIKI_DOWN:
                s32KeyVal = HI_ADP_KEY_DOWN;
                break;

            case DIKI_TAB:
            case DIKI_DELETE:
            case DIKI_BACKSPACE:
                s32KeyVal = HI_ADP_KEY_BACK;
                break;
            case DIKI_ENTER:
                s32KeyVal = HI_ADP_KEY_ENTER;
                break;
            case DIKI_F1:
                s32KeyVal = HI_ADP_KEY_F1;
                break;
            case DIKI_F2:
                s32KeyVal = HI_ADP_KEY_F2;
                break;
            case DIKI_F3:
                s32KeyVal = HI_ADP_KEY_F3;
                break;
            case DIKI_F4:
                s32KeyVal = HI_ADP_KEY_F4;
                break;
#if 0
                /** 需要的时候可以在DFB上面适配lirc.c文件 **/
            case:
                s32KeyVal = HI_ADP_KEY_SET;
                break;
            case:
                s32KeyVal = HI_ADP_KEY_SUB;
                break;
            case:
                s32KeyVal = HI_ADP_KEY_SEEK;
                break;
            case:
                s32KeyVal = HI_ADP_KEY_INFO;
                break;
            case:
                s32KeyVal = HI_ADP_KEY_POWER;
                break;
#endif

            default:
                break;
            }

            IR_KEYBOARD = HI_TRUE;
            break;
        case DWET_LOSTFOCUS:
            break;

        default:
            break;
        }
    }

#ifdef NEED_USE_MOUSE
    switch (evt.type)
    {
    case DWET_MOTION:           /** 鼠标运动 **/
        s_MouseX = evt.cx;
        s_MouseY = evt.cy;
        break;

    case DWET_KEYDOWN:
        switch (evt.key_symbol)
        {
        case DIKS_ESCAPE:
            break;
        case DIKS_SMALL_Q:
            break;
        case DIKS_CAPITAL_Q:
            break;
        case DIKS_BACK:
            break;
        case DIKS_STOP:
            break;
        default:
            break;
        }

        break;
    default:
        break;
    }
#endif


    /*********************************************************************
                        这个地方可以由rightware公司来修正顺序
    *********************************************************************/
#if 0
    IDirectFBSurface  *surface;
    surface = s_windowsurface;
    surface->Flip(surface, NULL, DSFLIP_ONSYNC);
#endif

    return s32KeyVal;
}

/******************************************************************************

                       layer上的，参见df_input

******************************************************************************/

/******************************************************************************
func                     : HI_Mouse_GetKey
description              :
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_Mouse_GetKey(HI_VOID)
{
    DFBInputDeviceKeySymbol last_symbol = DIKS_NULL;

    DFBInputEvent evt;

    if (s_event->GetEvent( s_event, DFB_EVENT(&evt)) == DFB_OK)
    {
        DFBInputDeviceTypeFlags device_type;

        device_type = get_device_type( s_devices, evt.device_id );

        switch (evt.type)
        {
        case DIET_AXISMOTION:
            if (device_type & DIDTF_MOUSE)
            {
                process_mouse_event( &evt );
            }

            break;
        default:
            break;
        }
    }

    if (evt.type == DIET_KEYRELEASE)
    {
        if (((last_symbol == DIKS_ESCAPE) || (last_symbol == DIKS_EXIT))
           && ((evt.key_symbol == DIKS_ESCAPE) || (evt.key_symbol == DIKS_EXIT)))
        {
            return HI_FAILURE;
        }

        last_symbol = evt.key_symbol;
    }

    s_event->WaitForEvent( s_event );

    return HI_SUCCESS;
}

#if 0
static DFBEnumerationResult enum_input_device(DFBInputDeviceID             device_id,
                                              DFBInputDeviceDescription    desc,
                                              void                        *data )
{
    MOUSE_DEVICE_INFO **devices = data;
    MOUSE_DEVICE_INFO  *device;

    device = malloc( sizeof(MOUSE_DEVICE_INFO));

    device->device_id = device_id;
    device->desc = desc;
    device->next = *devices;

    *devices = device;
    return DFENUM_OK;
}

static const char *get_device_name(MOUSE_DEVICE_INFO  *devices,
                                   DFBInputDeviceID    device_id )
{
    while (devices)
    {
        if (devices->device_id == device_id)
        {
            return devices->desc.name;
        }

        devices = devices->next;
    }

    return "<unknown>";
}

#endif

static DFBInputDeviceTypeFlags get_device_type(MOUSE_DEVICE_INFO   *devices,
                                               DFBInputDeviceID     device_id )
{
    while (devices)
    {
        if (devices->device_id == device_id)
        {
            return devices->desc.type;
        }

        devices = devices->next;
    }

    return DIDTF_NONE;
}

static HI_VOID process_mouse_event( DFBInputEvent *evt )
{
    if (evt->type == DIET_AXISMOTION)
    {
        if (evt->flags & DIEF_AXISABS)
        {
            switch (evt->axis)
            {
            case DIAI_X:
                s_MouseX = evt->axisabs;
                break;
            case DIAI_Y:
                s_MouseY = evt->axisabs;
                break;
            case DIAI_Z:
                break;
            default:
                break;
            }
        }
        else if (evt->flags & DIEF_AXISREL)
        {
            switch (evt->axis)
            {
            case DIAI_X:
                s_MouseX += evt->axisrel;
                break;
            case DIAI_Y:
                s_MouseY += evt->axisrel;
                break;
            case DIAI_Z:
                break;
            default:
                break;
            }
        }

        s_MouseX = GetLowAndHigh (s_MouseX, 0, s_ScreenInfo.s32ScreenWidth - 1);
        s_MouseY = GetLowAndHigh  (s_MouseY, 0, s_ScreenInfo.s32ScreenHeight - 1);
    }
}

#endif /** #ifdef EGL_API_DIRECTFB **/
