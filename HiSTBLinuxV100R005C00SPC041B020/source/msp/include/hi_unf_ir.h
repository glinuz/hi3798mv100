/******************************************************************************

  Copyright (C), 2001-2014, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
File Name     : hi_unf_ecs_type.h
Version       : Initial draft
Author        : HiSilicon multimedia software group
Created Date  : 2012-08-24
Last Modified by:
Description   : Application programming interfaces (APIs) of IR
Function List :
Change History:
 ******************************************************************************/

#ifndef __HI_UNF_IR_H__
#define __HI_UNF_IR_H__

#include "hi_common.h"
#include "hi_error_mpi.h"
//#include "hi_unf_keyled.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      IR */
/** @{ */  /** <!-- [IR] */

/**status of key*/
/**CNcomment:按键状态*/
typedef enum  hiUNF_KEY_STATUS_E
{
    HI_UNF_KEY_STATUS_DOWN = 0 ,   /**<Pressed*/   /**<CNcomment:按下按键 */
    HI_UNF_KEY_STATUS_HOLD ,       /**<Hold*/      /**<CNcomment:按住不动 */
    HI_UNF_KEY_STATUS_UP ,         /**<Released*/  /**<CNcomment:抬起按键 */

    HI_UNF_KEY_STATUS_BUTT
}HI_UNF_KEY_STATUS_E ;

/**Code type supported by the infrared (IR) module*/
/**CNcomment:IR支持的码型*/
typedef enum hiUNF_IR_CODE_E
{
    HI_UNF_IR_CODE_NEC_SIMPLE = 0,  /**<NEC with simple repeat code*/   /**<CNcomment:NEC with simple repeat code码型*/
    HI_UNF_IR_CODE_TC9012,          /**<TC9012 code*/                   /**<CNcomment:TC9012码型*/
    HI_UNF_IR_CODE_NEC_FULL,        /**<NEC with full repeat code*/     /**<CNcomment:NEC with full repeat code码型*/
    HI_UNF_IR_CODE_SONY_12BIT,      /**<SONY 12-bit code */             /**<CNcomment:SONY 12BIT码型*/
    HI_UNF_IR_CODE_RAW,               /**<Raw code*/	                    /**<CNcomment:raw 码型*/
    HI_UNF_IR_CODE_BUTT
}HI_UNF_IR_CODE_E;

/**List of IR code type. For details about HI_UNF_IR_STD, see HI_UNF_IR_CODE_E.*/
/**CNcomment:IR码型列表,HI_UNF_IR_STD部分请参照HI_UNF_IR_CODE_E */
typedef enum hiUNF_IR_PROTOCOL_E
{
    HI_UNF_IR_NEC,               /**<For NEC protocol*/                          /**<CNcomment:NEC码型*/
    HI_UNF_IR_RC6A = 10,         /**<For RC6A protocol*/                         /**<CNcomment:RC6A码型*/
    HI_UNF_IR_RC5,               /**<For RC5 protocol*/                          /**<CNcomment:RC5码型*/
    HI_UNF_IR_LOW_LATENCY_PROTOCOL,
    HI_UNF_IR_RC6_MODE0,         /**<For RC6 mode 0*/                            /**<CNcomment:RC6 mode0码型*/
    HI_UNF_IR_RCMM,              /**<For RCMM 24/32 protocol*/                   /**<CNcomment:RCMM 24/32码型*/
    HI_UNF_IR_RUWIDO,            /**<For Ruwido protocol*/                       /**<CNcomment:Ruwido码型*/
    HI_UNF_IR_RCRF8,             /**<For RCRF8 protocol*/                        /**<CNcomment:RCRF8码型*/
    HI_UNF_IR_MULTIPLE,          /**<For multiple protocol support*/             /**<CNcomment:Multiple码型*/    
    HI_UNF_IR_RMAP,              /**<For RMAP protocol with Mancester coding*/   /**<CNcomment:RMAP 曼彻斯特码型*/
    HI_UNF_IR_RSTEP,             /**<For RSTEP protocol*/                        /**<CNcomment:RSTEP码型*/
    HI_UNF_IR_RMAP_DOUBLEBIT,    /**<For RMAP protocol with Double bit coding*/  /**<CNcomment:RMAP 双bit码型*/
    HI_UNF_IR_LOW_LATENCY_PRO_PROTOCOL,
    HI_UNF_IR_XMP,               /**<For XMP protocol*/                          /**<CNcomment:XMP码型*/
    HI_UNF_IR_USER_DEFINED,      /**<For user protocol*/                         /**<CNcomment:用户自定义码型*/    
    HI_UNF_IR_PROTOCOL_BUTT 
} HI_UNF_IR_PROTOCOL_E;

/** @} */  /** <!-- ==== Structure Definition End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      IR */
/** @{ */  /** <!-- [IR] */
/*---IR---*/

/**
\brief Starts the IR device.
CNcomment:\brief 打开IR设备。CNend

\attention \n
This API can be called repeatedly. Key IDs can be received only after you can start the IR device, and then call HI_UNF_IR_Enable. \n
CNcomment:重复调用会返回成功，打开设备后必须再调用HI_UNF_IR_Enable才能正常接收键值。CNend

\param N/A                                                                   CNcomment:无 CNend
\retval HI_SUCCESS Success                                                   CNcomment:成功 CNend
\retval ::HI_ERR_IR_OPEN_ERR   The IR device fails to open                   CNcomment:IR设备打开失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_Init(HI_VOID);

#define HI_UNF_IR_Open() HI_UNF_IR_Init()

/**
\brief Stops the IR device.
CNcomment:\brief 关闭IR设备。CNend

\attention \n
This API can be called repeatedly. \n
CNcomment:重复调用关闭会返回成功。CNend

\param  N/A                                                                  CNcomment:无 CNend
\retval HI_SUCCESS Success                                                   CNcomment:成功 CNend
\retval ::HI_ERR_IR_CLOSE_ERR  The IR device fails to close.                 CNcomment:IR设备关闭失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_DeInit(HI_VOID);

#define HI_UNF_IR_Close() HI_UNF_IR_DeInit()

/**
\brief Enables the IR device.
CNcomment:\brief 使能IR设备。CNend
\attention \n
N/A
\param[in] bEnable  IR enable. HI_TRUE: enabled; HI_FALSE: disabled           CNcomment:IR 使能开关, HI_TRUE 使能, HI_FALSE 禁用。CNend
\retval HI_SUCCESS Success                                                    CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.               CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.                   CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_ENABLE_FAILED It fails to enable IR device.               CNcomment:使能设备失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_Enable ( HI_BOOL bEnable);

/**
\brief Obtains the protocol type of a remote control.
CNcomment:\brief 获取遥控器的协议类型 。CNend

\attention \n
This interface is only supported in IR_STD mode. And it must be used after function "HI_UNF_IR_GetValueWithProtocol".\n
CNcomment:此接口只在IR_STD模式下支持。需要在HI_UNF_IR_GetValueWithProtocol函数后调用。 CNend

\param[out]  penProtocol  protocol type pointer,reference HI_UNF_IR_PROTOCOL_E for detail   CNcomment:协议类型指针  具体含义请参考::HI_UNF_IR_PROTOCOL_E CNend
\retval HI_SUCCESS Success                                                            CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                       CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_NULL_PTR  The pointer is invalid.                                 CNcomment: 指针为空 CNend
\see \n
N/A
*/

HI_S32 HI_UNF_IR_GetProtocol(HI_UNF_IR_PROTOCOL_E *penProtocol);

/**
\brief obtain name of remote control protocol
CNcomment:\brief 获取遥控器的协议名称 。CNend

\attention \n
when IR_TYPE=IR_LIRC is effective, this interface is not supported now.
CNcomment:当IR_TYPE=IR_LIRC时有效，此接口暂不支持 CNend

\param[out]  pProtocolName  used to save first address of the protocol name buffer    CNcomment:用于存储协议名称的缓冲区首地址 CNend
\param[in]   s32BufLen      used to save length of the protocol name buffer           CNcomment:用于存储协议名称的缓冲区长度 CNend
\retval ::HI_ERR_IR_UNSUPPORT   It is not supported.                                  CNcomment:操作不支持 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_GetProtocolName(HI_CHAR *pProtocolName, HI_S32 s32BufLen);

/**
\brief Obtains the key values and key status of the remote control.
CNcomment:\brief 获取遥控器的按键值和按键状态 。CNend

\attention \n

\param[out]  penPressStatus  Key status. For details about the definition, see the description of ::HI_UNF_KEY_STATUS_E.  CNcomment:按键状态。具体含义请参考::HI_UNF_KEY_STATUS_E CNend
\param[out]  pu64KeyId  Key value                                                     CNcomment:按键值 CNend
\param[out]  pszProtocolName  used to save first address of the protocol name buffer    CNcomment:用于存储协议名称的缓冲区首地址 CNend
\param[in]   s32NameSize      used to save length of the protocol name buffer           CNcomment:用于存储协议名称的缓冲区长度 CNend
\param[in] u32TimeoutMs  Timeout (in ms). 0: not blocked; 0xFFFFFFFF: infinite block  CNcomment:超时值, 单位是毫秒, 0 - 不阻塞, 0xFFFFFFFF-永久阻塞 CNend
\retval HI_SUCCESS Success                                                            CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                       CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_NULL_PTR  The pointer is invalid.                                 CNcomment: 指针为空 CNend
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.                           CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_SET_BLOCKTIME_FAILED  The IR device fails to set block time.      CNcomment:设置阻塞时间失败 CNend
\retval ::HI_ERR_IR_READ_FAILED  The IR device fails to read key.                     CNcomment:读取键值和状态失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_GetValueWithProtocol(HI_UNF_KEY_STATUS_E *penPressStatus, HI_U64 *pu64KeyId,
                                      HI_CHAR *pszProtocolName, HI_S32 s32NameSize, HI_U32 u32TimeoutMs);

#define HI_UNF_IR_GetValue(penPressStatus, pu64KeyId, u32TimeoutMs) HI_UNF_IR_GetValueWithProtocol(penPressStatus, \
                                                                                                   pu64KeyId, NULL, 0, \
                                                                                                   u32TimeoutMs)


/**
\brief Set key fetch or symbol fetch from ir driver.
CNcomment:\brief 设定从红外驱动获取的是键值还是裸电平 。CNend

\attention \n
when IR_TYPE=IR_S2 is effective.
CNcomment:当IR_TYPE=IR_S2时有效。CNend

\param[in] mode 0 means key mode. 1 means symbol mode                                 CNcomment:0获取键值，1获取裸电平 CNend
\retval HI_SUCCESS Success                                                            CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                       CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.                           CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_SET_FETCHMETHOD_FAILED  The IR device fails to set fetch method.  CNcomment:设置获取方式失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_SetFetchMode(HI_S32 s32Mode);

/**
\brief Obtains the raw symbols from ir driver.
CNcomment:\brief 获取遥控器的裸电平 。CNend

\attention \n
when IR_TYPE=IR_S2 is effective.
CNcomment:当IR_TYPE=IR_S2时有效。CNend

\param[out]  pu64lower  lower pluse value                                             CNcomment:裸电平对的低位 CNend
\param[out]  pu64upper  upper space value                                             CNcomment:裸电平对的高位 CNend
\param[in] s32TimeoutMs read timeout .                                                CNcomment:读超时时间。CNend

\retval HI_SUCCESS Success                                                            CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized. 					  CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_NULL_PTR  The pointer is invalid.								  CNcomment: 指针为空 CNend
\retval ::HI_ERR_IR_SET_BLOCKTIME_FAILED  The IR device fails to set block time.      CNcomment:设置阻塞时间失败 CNend
\retval ::HI_ERR_IR_READ_FAILED  The IR device fails to read key.					  CNcomment:读取键值和状态失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_GetSymbol(HI_U64 *pu64First, HI_U64* pu64Second, HI_U32 u32TimeoutMs);

/**
\brief Enables or disables the function of reporting the released status of a key.
CNcomment:\brief 设置是否上报按键弹起状态。CNend

\attention \n
The function is enabled by default.
CNcomment:如不设置，默认为打开。CNend

\param[in] bEnable      Key released enable.  CNcomment:按键弹起有效。CNend
                        0: disabled           CNcomment:0：关闭；CNend
                        1: enabled            CNcomment: 1：使能。CNend
\retval HI_SUCCESS  Success                                             CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.         CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.             CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_SET_KEYUP_FAILED  It fails to enable released key.  CNcomment:设置上报按键弹起状态失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_EnableKeyUp(HI_BOOL bEnable);

/**
\brief Enables or disables the function of reporting the same key value. If keys are pressed and held down, data is continuously transmitted to the receive buffer. Therefore, you can enable or disable this function for applications as required.
CNcomment:\brief 设置是否上报重复按键。一直按键时，数据会源源不断的送入到接收缓冲区，因此应用程序可以根据需要来设置是否上报重复按键。CNend

\attention \n
The function is enabled by default.\n
This API must work with HI_UNF_IR_RepKeyTimeoutVal. The API HI_UNF_IR_RepKeyTimeoutVal is used to set the interval of reporting the same key value.\n
If the function of reporting the same key value is enabled, the keys are pressed and held down, and the interval is set to 300 ms, data is reported once every 300 ms.
If the function is disabled, data is reported only once regardless of how long the keys are held down.
CNcomment:如不设置，默认为打开\n 
此接口需要和HI_UNF_IR_RepKeyTimeoutVal函数结合使用，由HI_UNF_IR_RepKeyTimeoutVal设置上报重复按键的间隔\n 
如果使能了重复按键上报，当按键一直处于按下状态，间隔设为300毫秒，则每300毫秒会上报一次数据\n 
如果禁止了重复按键上报，则不论按下多长时间，只上报一次数据。CNend
\param[in] bEnable     Repeat key report enable.    CNcomment:按键产生重复按键功能。CNend
                       0: disabled                  CNcomment:0：关闭；CNend
                       1: enabled                   CNcomment:1：使能。CNend
\retval HI_SUCCESS Success                                                     CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT   The IR device is not initialized.               CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_INVALID_PARA   The parameter is invalid.                   CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_SET_REPEAT_FAILED   It fails to enable repeat key.         CNcomment:设置上报重复键失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_EnableRepKey(HI_BOOL bEnable);

/**
\brief Sets the interval (in ms) of reporting the same key value.
CNcomment:\brief 设置上报重复按键的时间间隔，单位为ms 。CNend

\attention \n
This API is unavailable if the function of reporting the same key value is disabled by calling HI_UNF_IR_IsRepKey.
CNcomment:当HI_UNF_IR_IsRepKey配置为不上报重复按键时，此接口设置无效。CNend

\param[in] u32TimeoutMs   Interval of reporting the same key value. The interval ranges from 0 ms to 65,536 ms.\n
						  The value 0 will be set to 108, and the value bigger than 65536 will be set to 65536 \n
                          CNcomment:上报重复按键的时间间隔，设置范围：0ms～65536ms 。\n
						  等于0的参数会被强制设置成108，大于65536的参数会被强制设置成65536。CNend
\retval HI_SUCCESS Success CNcomment:                                          CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_SET_REPKEYTIMEOUT_FAILED  It fails to set repeat key timeout.  CNcomment:设置上报重复按键间隔失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_SetRepKeyTimeoutAttr(HI_U32 u32TimeoutMs);

/**
\brief Sets the code type of the remote control.
CNcomment:\brief 设置遥控器码型。CNend

\attention \n
when IR_TYPE=IR_STD is effective.
CNcomment:当IR_TYPE=IR_STD时有效。CNend


\param[in] enIRCode Four standard code types of the remote control are supported by default.  CNcomment:默认支持4种标准遥控器码型：CNend
                     HI_UNF_IR_CODE_NEC_SIMPLE:Nec With Simple code type. CNcomment:Nec Simple 码型。 CNend
                     HI_UNF_IR_CODE_TC9012:TC9012 code type.              CNcomment:TC9012 码型。     CNend
                     HI_UNF_IR_CODE_NEC_FULL:Nec With Full code type.     CNcomment:Nec Full 码型。   CNend
                     HI_UNF_IR_CODE_SONY_12BIT:Sony 12 Bit code type.     CNcomment:Sony 12 bit 码型。CNend
\retval HI_SUCCESS Success                                                 CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.            CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_INVALID_PARA   The parameter is invalid.               CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_SETFORMAT_FAILED It fails to set IR code type.         CNcomment:设置IR类型失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_SetCodeType(HI_UNF_IR_CODE_E enIRCode);

/**
\brief  Resets the IR device.
CNcomment:\brief  复位IR设备。CNend

\attention \n
This API is used to delete the key values in the buffer.
CNcomment:只是把buffer中的键值清掉。CNend

\param N/A         CNcomment:无 CNend
\retval HI_SUCCESS Success                                                      CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                 CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_RESET_FAILED  The IR device fails to reset.                 CNcomment:IR设备复位失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_Reset(HI_VOID);

/**
\brief enable an infrared code.
CNcomment:\brief  激活某类红外遥控器协议。CNend

\attention \n
when IR_TYPE=IR_S2 is effective.
CNcomment:当IR_TYPE=IR_S2时有效。CNend

\param N/A         CNcomment:无 CNend
\retval HI_SUCCESS Success                                                      CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                 CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_NULL_PTR  The pointer is invalid.							CNcomment: 指针为空 CNend
\retval ::HI_ERR_IR_INVALID_PARA   The parameter is invalid.                    CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_ENABLE_PROT_FAILED  It fails to enable an infrared code.    CNcomment:激活遥控协议失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_EnableProtocol(HI_CHAR* pszProtocolName);

/**
\brief disable an infrared code.
CNcomment:\brief  禁用某类红外遥控器协议。CNend

\attention \n
when IR_TYPE=IR_S2 is effective.
CNcomment:当IR_TYPE=IR_S2时有效。CNend

\param N/A         CNcomment:无 CNend
\retval HI_SUCCESS Success                                                      CNcomment:成功 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                 CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_NULL_PTR  The pointer is invalid.							CNcomment: 指针为空 CNend
\retval ::HI_ERR_IR_INVALID_PARA   The parameter is invalid.                    CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_DISABLE_PROT_FAILED   It fails to disalbe an infrared code. CNcomment:禁用遥控协议失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_DisableProtocol(HI_CHAR* pszProtocolName);

/**
\brief get the enable or disable status of  an infrared code.
CNcomment:\brief  获取某类红外遥控器协议激活状态。CNend

\attention \n
when IR_TYPE=IR_S2 is effective.
CNcomment:当IR_TYPE=IR_S2时有效。CNend

\param N/A         CNcomment:无 CNend
\retval HI_SUCCESS Success                                                      CNcomment:成功 CNend
\retval HI_FAILURE Failure                                                      CNcomment:失败 CNend
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                 CNcomment:IR设备未初始化 CNend
\retval ::HI_ERR_IR_NULL_PTR  The pointer is invalid.							CNcomment: 指针为空 CNend
\retval ::HI_ERR_IR_INVALID_PARA   The parameter is invalid.                    CNcomment:参数非法 CNend
\retval ::HI_ERR_IR_GET_PROTENABLE_FAILED It fails to get status of an infrared code.  CNcomment:获取遥控协议状态失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_IR_GetProtocolEnabled(HI_CHAR* pszProtocolName, HI_BOOL *pbEnabled);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __HI_UNF_IR_H__ */
