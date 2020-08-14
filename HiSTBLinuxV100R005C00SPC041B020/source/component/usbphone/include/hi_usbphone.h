
#ifndef __HI_USB_PHONE_H__
#define __HI_USB_PHONE_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#include "hi_common.h"

/***************************** Macro Definition ******************************/
#define HI_USB_PHONE_BUF_SIZE   128

#define HI_ERR_USB_PHONE_OPEN_ERR                   (HI_S32)(0x80550001)
#define HI_ERR_USB_PHONE_NOT_OPENED                 (HI_S32)(0x80550002)
#define HI_ERR_USB_PHONE_NOT_INIT                   (HI_S32)(0x80550003)
#define HI_ERR_USB_PHONE_INVALID_PARA               (HI_S32)(0x80550004)
#define HI_ERR_USB_PHONE_NULL_PTR                   (HI_S32)(0x80550005)
#define HI_ERR_USB_PHONE_INVALID_OPT                (HI_S32)(0x80550006)
#define HI_ERR_USB_PHONE_SEND_ERR                   (HI_S32)(0x80550007)
#define HI_ERR_USB_PHONE_RECEIVE_ERR                (HI_S32)(0x80550008)

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition ****************************/
/** define usb phone state */
typedef enum
{
    HI_USB_PHONE_STATE_IDLE = 0,        /**< idle state*/
    HI_USB_PHONE_STATE_TALK,            /**< talk state*/
    HI_USB_PHONE_STATE_REG,             /**< register state*/
    HI_USB_PHONE_STATE_RING,            /**< ring state*/
    HI_USB_PHONE_STATE_BUTT
}HI_USB_PHONE_STATE_E;

/** define usb phone events */
typedef enum
{
    HI_USB_PHONE_EVENT_BS_REG = 0,      /**< Base register status*/
    HI_USB_PHONE_EVENT_OFF_HOOK,        /**< off hook */
    HI_USB_PHONE_EVENT_ON_HOOK,         /**< on hook */
    HI_USB_PHONE_EVENT_KEY,             /**< keys */
    HI_USB_PHONE_EVENT_HOLD,            /**< hold */  
    HI_USB_PHONE_EVENT_DISCON,          /**< disconnect */  
    HI_USB_PHONE_EVENT_BUTT
}HI_USB_PHONE_EVENT_E;

/** difine the tone type of usb phone */
typedef enum
{
    HI_USB_PHONE_BUSY_TONE = 0,         /**< busy tone */
    HI_USB_PHONE_HOWLER_TONE,           /**< howler tone */
    HI_USB_PHONE_TONE_BUTT  
}HI_USB_PHONE_TONE_TYPE_E;

/** define the test mode for WIFI disturbance */
typedef enum
{
    HI_USB_PHONE_WIFI_OFF = 0,              /**< WIFI OFF */
    HI_USB_PHONE_WIFI_MODE1 = 1,            /**< WIFI ON, the base frequency jump between CH0-CH18 */
    HI_USB_PHONE_WIFI_MODE2 = 2,            /**< WIFI ON, the base frequency jump between CH0-CH14 and CH91-CH94 */
    HI_USB_PHONE_WIFI_MODE3 = 3,            /**< WIFI ON, the base frequency jump between CH0-CH5 and CH82-CH94 */
    HI_USB_PHONE_WIFI_MODE4 = 4,            /**< WIFI ON, the base frequency jump between CH76-CH94 */
    HI_USB_PHONE_WIFI_MODE_BUTT  
}HI_USB_PHONE_WIFI_MODE_E;

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration *****************************/
/**
 \brief Define the usb phone event call back function
 \attention \n
 None
 \param[in] enEvent   The usb phone event type defined by HI_USB_PHONE_EVENT_E
 \param[in] s32EventValue   The usb phone event value
 \param[in] u32Args   The private parameter from user application. 
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

typedef HI_S32 (*HI_USB_PHONE_EVENT_CB_FN)(HI_USB_PHONE_EVENT_E enEvent, HI_S32 s32EventValue, HI_U32 u32Args);

/**
 \brief Initial the usb phone module
 \attention \n
 None
 \param  N/A
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_Init(HI_VOID);

/**
 \brief Deinitial the usb phone module
 \attention \n
 None
 \param  N/A
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_DeInit(HI_VOID);

/**
 \brief Open the usb phone 
 \attention \n
 None
 \param[in] pszDev   The device node of usb phone serial port such as "/dev/ttyUSB0"
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_Open(HI_CHAR *pszDev);

/**
 \brief Close the usb phone
 \attention \n
 None
 \param  N/A
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_Close(HI_VOID);

/**
 \brief Enable or disable usb phone ring 
 \attention \n
 None
 \param[in] bEnable   HI_TRUE -- enable ring, HI_FALSE -- disable ring
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_Ring(HI_BOOL bEnable);

/**
 \brief Play tone 
 \attention \n
 None
 \param[in] enToneType   tone type defined by HI_USB_PHONE_TONE_TYPE_E
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_PlayTone(HI_USB_PHONE_TONE_TYPE_E enToneType);

/**
 \brief Stop tone 
 \attention \n
 None
 \param[in] bOnHook   HI_TRUE -- stop tone with on hook state, HI_FALSE -- stop tone only
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_StopTone(HI_BOOL bOnHook);

/**
 \brief Display the call in ID 
 \attention \n
 None
 \param[in] pszCallID   the call in ID string
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_DisplayCallID(HI_CHAR *pszCallID);

/**
 \brief Enable or disable connection tone
 \attention \n
 None
 \param[in] bEnable   HI_TRUE -- enable , HI_FALSE -- disable , the default setting is enable
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_Config_ConTone(HI_BOOL bEnable);

/**
 \brief Get the base version
 \attention \n
 None
 \param[out] pszBSVersion   base version string pointer
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_GetBSVersion(HI_CHAR **pszBSVersion);

/**
 \brief Get the usb phone state
 \attention \n
 None
 \param[out] penState   usb phone state 
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
HI_S32 HI_USB_PHONE_GetState(HI_USB_PHONE_STATE_E *penState);


/**
 \brief Set the test mode for WIFI disturbance
 \attention \n
 None
 \param[in] enWifiMode   The test mode defined by HI_USB_PHONE_WIFI_MODE_E
 \retval ::0  HI_SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

HI_S32 HI_USB_PHONE_WIFI_Test(HI_USB_PHONE_WIFI_MODE_E enWifiMode);

/**
 \brief Register call back function for usb phone  events 
 \attention \n
 None
 \param[in] pfnEventCB   The pointer of call back function , NULL to unregister.
 \param[in] u32Args  The private parameter from user application.
 \retval ::0  HI_SUCCESS
 \retval ::HI_ERR_VOIP_HME_NO_INIT  
 \see \n
None
 */

HI_S32 HI_USB_PHONE_RegisterEvent(HI_USB_PHONE_EVENT_CB_FN pfnEventCB, HI_U32 u32Args);
/** @}*/  /** <!-- ==== API Declaration End ====*/


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif /* End of #ifndef __HI_USB_PHONE_H__*/

