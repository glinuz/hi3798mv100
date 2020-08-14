#ifndef __TEE_DRV_HDMI_COMMON_H__
#define __TEE_DRV_HDMI_COMMON_H__

#include <init_ext.h>
#include <sre_debug.h>
#include "hi_type.h"
#include "tee_log.h"
#include "sre_ticktimer_functions.h"
#include "tee_drv_hdmi.h"
#include "tee_drv_hdmi_hal.h"
#include "tee_drv_hdmi_srm.h"

/******************************************************************************/
/* HDMI Version                                                               */
/******************************************************************************/
#define HDMI_VER_MAJOR    1
#define HDMI_VER_MINOR    0
#define HDMI_VER_REVISE   0
#define HDMI_VER_DATE     20160921
#define HDMI_VER_TIMES    3

#define MAKE_VER_BIT(x) # x
#define MAKE_MACRO2STR(exp) MAKE_VER_BIT(exp)
#define MAKE_VERSION \
    MAKE_MACRO2STR(HDMI_VER_MAJOR) "."  \
    MAKE_MACRO2STR(HDMI_VER_MINOR) "."  \
    MAKE_MACRO2STR(HDMI_VER_REVISE) "." \
    MAKE_MACRO2STR(HDMI_VER_DATE) "."   \
    MAKE_MACRO2STR(HDMI_VER_TIMES)

#define CHECK_POINTER(p) if(p == HI_NULL) {\
                            ta_debug("input ptr is null! \n"); \
                            return HI_FAILURE; }


#define mSleep(msec)    SRE_SwMsleep((msec))
#define Sleep(sec)      SRE_SwSleep((sec))
#define HDMI_PROC       uart_printf_func
#define HDMI_ERR        uart_printf_func


typedef struct
{
    HI_U8                hotPlug;
    HI_U8                rsen;
}HdmiStatus_t;

typedef struct 
{
    HdmiStatus_t  hdmiStatus;
}HI_DRV_HDMI_HADRWARE_STATUS_S;

typedef struct 
{
    HI_BOOL             bTimerCreate;
    HI_BOOL             bTimerIsRunning;
    timer_event*        timer_event;
}HI_DRV_HDMI_TIMER_INFO_S;

typedef enum hiDRV_HDMI_HDCP_VERSION_E
{
    HI_DRV_HDMI_HDCP_VER_NONE = 0,  /** no hdcp version */
    HI_DRV_HDMI_HDCP_14,        /** use hdcp1.4 version */
    HI_DRV_HDMI_HDCP_22,        /** use hdcp2.2 version */
    HI_DRV_HDMI_HDCP_VER_BUTT
} HI_DRV_HDMI_HDCP_VERSION_E; 

typedef struct 
{
    HDMI_SRM_CTX_S                  stSrmCtx;
    HDMI_HDCP_DEVID_INFO_S          stDevIdInfo;
}HI_DRV_HDMI_SRM_INFO_S;

typedef enum 
{
    HI_DRV_HDMI_TEE_OK = 0,             /** TEE status OK */
    HI_DRV_HDMI_TEE_STOP_HDCP,          /** TEE stop HDCP */
    HI_DRV_HDMI_TEE_BUTT
} HI_DRV_HDMI_TEE_STATUS_E; 

typedef struct 
{
    HI_U32                          u32HdmiDevId;
    HI_BOOL                         enHdcpWasOn;
    HI_DRV_HDMI_HDCP_STATUS_E       enHdcpStatus;
    HI_DRV_HDMI_HDCP_VERSION_E      enHdcpVersion;
    HI_DRV_HDMI_HDCP_LEVEL_E        enHdcpLevel;
    HI_DRV_HDMI_HADRWARE_STATUS_S   stHardwareStatus;
    HI_BOOL                         bHdcp22ProLoad;
    HI_BOOL                         bHdcp14Succ;
    HI_DRV_HDMI_TIMER_INFO_S        stTimerInfo;
    HI_DRV_HDMI_HDCP_CAPS_S         stHdcpCaps;
    HI_DRV_HDMI_SRM_INFO_S          stSrmInfo;
    HDMI_HDCP_MUTE_STATUS_S         stMuteStatus;
    TEE_HDMI_HAL_S                  stTeeHdmiHal;
    HI_DRV_HDMI_TEE_STATUS_E        enTeeStatus;
    
}TEE_HDMI_DEVICE_S;


#endif


