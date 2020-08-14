/******************************************************************************

  Copyright (C), 2001-2014, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
File Name     : hi_unf_keyled.h
Version       : Initial draft
Author        : HiSilicon multimedia software group
Created Date  : 2012-08-24
Last Modified by:
Description   : Application programming interfaces (APIs) of the KEYLED
Function List :
Change History:
 ******************************************************************************/

#ifndef __HI_UNF_KEYLED_H__
#define __HI_UNF_KEYLED_H__

#include "hi_common.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      KEYLED */
/** @{ */  /** <!-- [KEYLED] */
/**keyled type*/
/**CNcomment:KEYLED类型 */
typedef enum  hiUNF_KEYLED_TYPE_E
{
    HI_UNF_KEYLED_TYPE_74HC164 = 0x0, /**<KEYLED type:74HC164*/                   /**<CNcomment:KEYLED型号: 74HC164 */
    HI_UNF_KEYLED_TYPE_PT6961,    /**<KEYLED type: PT6961*/                       /**<CNcomment:KEYLED型号：PT6961 */
    HI_UNF_KEYLED_TYPE_CT1642,    /**<KEYLED type: CT1642*/                       /**<CNcomment:KEYLED型号：CT1642 */
    HI_UNF_KEYLED_TYPE_PT6964,	  /**<KEYLED type: PT6964*/                       /**<CNcomment:KEYLED型号：PT6964 */
    HI_UNF_KEYLED_TYPE_FD650,     /**<KEYLED type: FD650*/                        /**<CNcomment:KEYLED型号：FD650 */
    HI_UNF_KEYLED_TYPE_GPIOKEY,   /**<KEYLED type :GPIOKEY*/   					 /**<CNcomment:GPIO按键面板*/
    HI_UNF_KEYLED_TYPE_BUTT       
}HI_UNF_KEYLED_TYPE_E;

/**Blink frequency level of the LED*/
/**CNcomment:LED闪烁频率级别*/
typedef enum hiUNF_KEYLED_LEVEL_E
{
    HI_UNF_KEYLED_LEVEL_1 = 0x01,   /**<Level 1, slowest*/  /**<CNcomment:级别1,闪烁频率最慢*/
    HI_UNF_KEYLED_LEVEL_2,          /**<Level 2*/           /**<CNcomment:级别2 */
    HI_UNF_KEYLED_LEVEL_3,          /**<Level 3*/           /**<CNcomment:级别3 */
    HI_UNF_KEYLED_LEVEL_4,          /**<Level 4*/           /**<CNcomment:级别4 */
    HI_UNF_KEYLED_LEVEL_5,          /**<Level 5, fastest*/  /**<CNcomment:级别5 , 闪烁频率最快*/

    HI_UNF_KEYLED_LEVEL_BUTT
}HI_UNF_KEYLED_LEVEL_E;

/**Blink sequence of LEDs*/
/**CNcomment:闪烁的LED序号*/
typedef enum hiUNF_KEYLED_LIGHT_E
{
    HI_UNF_KEYLED_LIGHT_1 = 0x01,   /**<The first LED blinks.*/   /**<CNcomment:第1个LED闪烁*/
    HI_UNF_KEYLED_LIGHT_2,          /**<The second LED blinks.*/  /**<CNcomment:第2个LED闪烁*/
    HI_UNF_KEYLED_LIGHT_3,          /**<The third LED blinks.*/   /**<CNcomment:第3个LED闪烁*/
    HI_UNF_KEYLED_LIGHT_4,          /**<The fourth LED blinks.*/  /**<CNcomment:第4个LED闪烁*/
    HI_UNF_KEYLED_LIGHT_ALL,        /**<All LEDs blink.*/        /**<CNcomment:所有LED均闪烁*/
    HI_UNF_KEYLED_LIGHT_NONE,       /**<All LEDs do not blink.*/  /**<CNcomment:所有LED均不闪烁*/

    HI_UNF_KEYLED_LIGHT_BUTT
}HI_UNF_KEYLED_LIGHT_E;

/**Display time of each LED*/
/**CNcomment:LED显示时间*/
typedef struct hiUNF_KEYLED_Time_S
{
    HI_U32 u32Hour;           /**<Hour*/   /**<CNcomment:时*/
    HI_U32 u32Minute;         /**<Minute*/ /**<CNcomment:分*/
}HI_UNF_KEYLED_TIME_S, *HI_UNF_KEYLED_TIME_S_PTR;

/** @} */  /** <!-- ==== Structure Definition End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      KEYLED */
/** @{ */  /** <!-- [KEYLED] */
/**---- keyled ----*/

/**
\brief Initializes the KEYLED module.
CNcomment:\brief 初始化KEYLED模块。CNend

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:重复调用本接口，会返回成功。CNend

\param N/A                                             CNcomment:无 CNend
\retval ::HI_SUCCESS Success                           CNcomment:成功 CNend
\retval ::HI_FAILURE  The KEYLED device fails to open. CNcomment:KEYLED设备打开失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_KEYLED_Init(HI_VOID);


/**
\brief Deinitializes the KEYLED module.
CNcomment:\brief 去初始化KEYLED模块。CNend

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:重复调用本接口，会返回成功。CNend

\param N/A                                             CNcomment:无 CNend
\retval ::HI_SUCCESS Success                           CNcomment:成功 CNend
\retval ::HI_FAILURE  The KEYLED device fails to close. CNcomment:KEYLED设备关闭失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_KEYLED_DeInit(HI_VOID);

/**
\brief Selects the type of the KEYLED.
CNcomment:\brief 选择KEYLED器件类型。CNend

\attention \n
It is recommended to call this API once after the KEYLED module is initialized.
CNcomment:建议初始化后只选择一次，不要重复调用。CNend

\param[in] enKeyLedType   KEYLED type                                           CNcomment:keyled类型。 CNend
\retval ::HI_SUCCESS   Success                                                  CNcomment:成功 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED device is not initialized.         CNcomment:KEYLED设备未初始化 CNend
\retval ::HI_ERR_KEYLED_INVALID_PARA  The parameter is invalid.                 CNcomment:参数非法 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                                     CNcomment: Ioctrl调用失败 CNend

\see \n
N/A
*/
HI_S32 HI_UNF_KEYLED_SelectType(HI_UNF_KEYLED_TYPE_E enKeyLedType);

/**
\brief Enables the key function.
CNcomment:\brief 打开按键功能。CNend
\attention \n
The KEYLED module starts to receive key values after the key function is enabled.\n
The error code HI_SUCCESS is returned if this API is called repeatedly.\n
CNcomment:打开后开始接收按键\n
重复调用本接口，会返回成功。CNend
\param N/A                                                                      CNcomment:无 CNend
\retval ::HI_SUCCESS Success                                                    CNcomment:成功 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED device is not initialized.         CNcomment:KEYLED设备未初始化 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                                     CNcomment: Ioctrl调用失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_Open(HI_VOID);

/**
\brief Disables the key function.
CNcomment:\brief 关闭按键功能。CNend

\attention \n
The KEYLED module stops receiving key IDs after the key function is disabled.\n
This API is valid when it is called for the first time. If this API is called repeatedly, the error code HI_SUCCESS is returned.\n
CNcomment:关闭后停止接收按键\n
本接口第一次调用起作用,重复调用返回成功。CNend

\param N/A                                                                      CNcomment:无 CNend
\retval ::HI_SUCCESS Success                                                    CNcomment:成功 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED device is not initialized.         CNcomment:KEYLED设备未初始化 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                                     CNcomment: Ioctrl调用失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_Close(HI_VOID);

/**
\brief Clears the key values that are not received.
CNcomment:\brief 清空当前没有接收的按键。CNend

\attention \n
This API is used to clear the buffer for storing the key values. In this way, the initial state is returned.
CNcomment:清空按键buffer，回到初始状态。CNend
\param N/A Success                                                              CNcomment:无 CNend
\retval ::HI_SUCCESS Success                                                    CNcomment:成功 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED device is not initialized.         CNcomment:KEYLED设备未初始化 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                                     CNcomment: Ioctrl调用失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_Reset(HI_VOID);


/**
\brief Obtains a key value.
CNcomment:\brief 获取一个按键值。CNend

\attention \n
This API is used to obtain the key value that is received first in the buffer. After this API is called, the key value and key status are returned.\n
The key status contains pressed, hold and released.\n
This API needs to work with ::HI_UNF_KEY_SetBlockTime.\n
If there are no key values, the API waits until timeout occurs. In this case, the error code ::HI_ERR_KEYLED_TIMEOUT is returned.\n
If the timeout is set to 0 in non-block mode, the error code ::HI_ERR_KEYLED_NO_NEW_KEY is returned when there are no key values.\n
If the timeout is set to 0xFFFFFFFF, it indicates infinite wait.\n
CNcomment:获取buffer中最早到达的按键值，返回按键值的数值和状态\n
状态包含按下，长按和弹起\n
接口配合::HI_UNF_KEY_SetBlockTime接口一起使用\n
没有按键时，进行等待，超时后退出并返回超时错误码\n
超时值为0时为非阻塞接口，没有按键直接返回没有按键错误码\n
超时值设置为0xffffffff，一直等待。CNend
\param[out] pu32PressStatus Key status: pressed(0x0), hold(0x1) or released(0x2) CNcomment:按键状态是按下，长按还是弹起。CNend
\param[out] pu32KeyId  Key value                                                 CNcomment:按键值。CNend
\retval ::HI_SUCCESS  Success                                                    CNcomment:成功 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.          CNcomment:模块没有初始化 CNend
\retval ::HI_ERR_KEYLED_NULL_PTR  The pointer is null.                           CNcomment:指针参数为空 CNend
\retval ::HI_ERR_KEYLED_NO_NEW_KEY  There are no key values.                     CNcomment:没有按键 CNend
\retval ::HI_ERR_KEYLED_TIMEOUT  Waiting for key values times out.               CNcomment:等待按键超时 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_GetValue(HI_U32 * pu32PressStatus, HI_U32 * pu32KeyId);

/**
\brief Sets the timeout of reading key IDs.
CNcomment:\brief 设置按键读取的超时时间。CNend

\attention \n
If the timeout is set to 0, the API is not blocked. If there is no key ID, the error code HI_ERR_KEYLED_NO_NEW_KEY is returned.\n
If the timeout is set to 0xFFFFFFFF, it indicates infinite wait.\n
The default time is 500 ms.\n
CNcomment:超时值为0时不阻塞，没有按键直接返回没有按键错误码\n
超时值设置为0xffffffff，一直等待。\n 
默认的超时判断时间为500ms。CNend
\param[in] u32BlockTimeMs   Timeout, in ms                               CNcomment:超时时间，单位ms。CNend
\retval ::HI_SUCCESS Success                                             CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                              CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:模块没有初始化 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_SetBlockTime(HI_U32 u32BlockTimeMs);


/**
\brief Set the time of pressing a key repeatedly.
CNcomment:\brief 设置重复按键的时间。CNend

\attention \n
If a key is pressed for a period of time, it considers that the key is pressed repeatedly.\n
The time ranges from 108 ms to 65536 ms. If the time is not within the range, the time is automatically restricted to this range.\n
The default time is 200 ms.\n
CNcomment:当某个键处于按下状态，超过设置的时间值判断为重复按键\n
重复按键的设置范围在108ms到65536ms之间，超过这个范围会自动约束到上限或下限上\n
默认的重复按键判断时间为200ms。CNend

\param[in] u32RepTimeMs  Time of pressing a key repeatedly, in ms       CNcomment:判断为重复按键的时间，单位ms。CNend
\retval ::HI_SUCCESS Success                                            CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                              CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized. CNcomment:模块没有初始化 CNend
\see \n 
N/A
*/
HI_S32 HI_UNF_KEY_RepKeyTimeoutVal(HI_U32 u32RepTimeMs);

/**
\brief Enables or disables the function of reporting the same key ID.
CNcomment:\brief 设置是否使能重复按键。CNend

\attention \n
The ID of a pressed key is reported consecutively only after the report function is enabled.\n
CNcomment:只有使能后，一直按住某个键，才能连续上报键值。CNend

\param[in] u32IsRepKey Repeat key report enable. 1: enabled; 0: disabled   CNcomment:是否使能重复按键，1 使能，0 不使能。CNend
\retval ::HI_SUCCESS Success                                               CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                                CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.    CNcomment:模块没有初始化 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_IsRepKey(HI_U32 u32IsRepKey);

/**
\brief Enables or disables the function of reporting the released status of a key.
CNcomment:\brief 设置是否上报按键弹起。CNend

\attention \n
The released statuses of keys are reported only after the report function is enabled. Otherwise, only the pressed statuses are reported.\n
CNcomment:使能后才会上报按键弹起事件，不然只上报按下事件。CNend

\param[in] u32IsKeyUp Repeat key report enable. 1: enabled; 0: disabled  CNcomment:是否使能重复按键，1 使能，0 不使能。CNend
\retval ::HI_SUCCESS Success                                             CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                              CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:模块没有初始化 CNend
\see \n 
N/A
*/
HI_S32 HI_UNF_KEY_IsKeyUp(HI_U32 u32IsKeyUp);

/**
\brief Enables the display function of LEDs.
CNcomment:\brief 打开数码管显示功能。CNend

\attention \n
The LEDs work only after the display function is enabled.\n
The error code HI_SUCCESS is returned if this API is called repeatedly.\n
CNcomment:打开后才会有数码管的显示\n
重复调用本接口，会返回成功。CNend

\param N/A                                     CNcomment:无 CNend
\retval ::HI_SUCCESS Success                   CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.    CNcomment: Ioctrl调用失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_LED_Open(HI_VOID);

/**
\brief Disables the display function of LEDs.
CNcomment:\brief 关闭数码管显示功能。CNend

\attention \n
The LEDs do not work after the display function is disabled.\n
This API is valid when it is called for the first time. If this API is called repeatedly, the error code HI_SUCCESS is returned.\n
CNcomment:关闭后停止数码管显示\n
本接口第一次调用起作用,重复调用返回成功。CNend
 
\param N/A                                    CNcomment:无 CNend
\retval ::HI_SUCCESS Success                  CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.   CNcomment: Ioctrl调用失败 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_LED_Close(HI_VOID);


/**
\brief Displays characters based on the type of the input code.
CNcomment:\brief 根据传入的码型值进行显示。CNend

\attention \n
Each LED displays a character that corresponds to the value of one byte.\n
During displaying, the values of the four characters to be displayed need to be combined to form a 32-bit unsigned int data segment. The data serves as the parameter value of this API.\n
The four LEDs from left to right display bit[7:0], bit[15:8], bit[23:16], and bit[31:24] of the data respectively.\n
For example, if you want to display "0123" on LEDs, you need to combine the display values corresponding to these characters to form the following unsigned int variable:\n
  Unsigned int parameter = 0x03 + (0x9F <<8) + (0x25<<16) + (0x0D <<24);\n
Where, 0x03, 0x9F, 0x25, and 0x0D are the display values of 0, 1, 2, and 3 respectively.\n
The following shows the mapping between the segments of a 7-segment LED and data bits:\n
       --7--       \n
      |     |      \n
     2|     |6     \n
      |--1--       \n
      |     |      \n
     3|     |5     \n
       --4-- .0    \n
The common display characters and values of the LEDs on HiSilicon demo board are as follows in the case of co-anode (the display characters and values need to be reversed in the case of co-cathode):
  Digitals 0-9: 0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09\n
  Upper-case letters A-Z (the characters that cannot be displayed are expressed as 0xFF):\n
     0xFF, 0xFF, 0x63, 0xFF, 0x61, 0x71, 0xFF, 0x91, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF,\n
     0xFF, 0x03, 0x31, 0xFF, 0xFF, 0x49, 0xFF, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF\n
  Lower-case letters a-z (the characters that cannot be displayed are expressed as 0xFF):\n
    0xFF, 0xC1, 0xE5, 0x85, 0x21, 0xFF, 0x09, 0xD1, 0xDF, 0xFF, 0xFF, 0x9F, 0xFF,\n
    0xD5, 0xC5, 0x31, 0x19, 0xFF, 0x49, 0xFF, 0xC7, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF
CNcomment:	每个数码管要显示的字符对应一个字节的显示值。\n
显示时需要将4 个待显示字符对应的显示值组合成一个unsiged int（32bit）数据作为此接口的参数值。\n
从左到右的4 个LED 依次显示数据的bit[7:0]、bit[15:8]、bit[23:16]、bit[31:24]。\n
例如：在LED 上显示“0123”，则需要将这些字符对应的显示值组合成一个Unsigned int 的变量：\n
   Unsigned int parameter = 0x03 + (0x9F <<8) + (0x25<<16) + (0x0D <<24)；\n
其中：0x03、0x9F、0x25、0x0D 分别是‘0’、‘1’、‘2’、‘3’的显示值。\n
七段数码管段数与数据的bit对应关系如下:\n
       --7--       \n
      |     |      \n
     2|     |6     \n
      |--1--       \n
      |     |      \n
     3|     |5     \n
       --4-- .0    \n
海思DEMO板数码管常用显示字符及其对应的显示值如下(针对共阳极，共阴极要取反)：\n
  . 字符0-9：0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09\n
  . 大写字母A-Z（不能显示的用0xFF 表示）：\n
     0xFF, 0xFF, 0x63, 0xFF, 0x61, 0x71, 0xFF, 0x91, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF,\n
     0xFF, 0x03, 0x31, 0xFF, 0xFF, 0x49, 0xFF, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF\n
  . 小写字母a-z（不能显示的用0xFF 表示）：\n
     0xFF, 0xC1, 0xE5, 0x85, 0x21, 0xFF, 0x09, 0xD1, 0xDF, 0xFF, 0xFF, 0x9F, 0xFF,\n
     0xD5, 0xC5, 0x31, 0x19, 0xFF, 0x49, 0xE1, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF CNend
\param[in] u32CodeValue 4-digit value to be displayed                   CNcomment:需要显示的4位数值。CNend
\retval ::HI_SUCCESS Success                                            CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                             CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized. CNcomment:模块没有初始化 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_LED_Display(HI_U32 u32CodeValue);


/**
\brief Displays the time on LEDs.
CNcomment:\brief 在数码管上显示时间。CNend

\attention \n
The values of hour and minute need to be converted into corresponding decimal values. For example, if you want to display 22:45, you need to set hour to 22 and set minute to 45.\n
After the start time is set, the time is displayed on LEDs. At the same time, the colon (:) in the time blinks at the frequency of 1 Hz.\n
Because the time is displayed in 24-hour format, the maximum value of hour is 23, and the maximum value of minute is 59.\n
CNcomment:小时、分钟只需配置成相应的十进制值就行了。比如显示22：45，则hour=22，minute=45。\n
传入时间数据结构，然后在数码管上显示，显示时间的同时冒号会以1HZ的频率闪烁\n
时间的范围注意按24小时制，最大支持小时为23，分为59。CNend

\param[in] stLedTime Time                                                CNcomment:时间。CNend
\retval ::HI_SUCCESS  Success                                            CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                              CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:模块没有初始化 CNend
\retval ::HI_ERR_KEYLED_INVALID_PARA  The input parameter is invalid.    CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_LED_DisplayTime(HI_UNF_KEYLED_TIME_S stLedTime);

/**
\brief Sets the ID of the blinked LED.
CNcomment:\brief 设置要闪烁的LED 序号。CNend

\attention \n
The IDs of the LEDs on the board are 1, 2, 3, and 4 from left to right. If you want 4 LEDs to blink together, set 5.\n
CNcomment:单板上的数码管的序号从左到右依次为：1、2、3、4。设置4个数码管一起闪烁:5；CNend

\param[in] enPin  Pin ID                                                 CNcomment:要显示的序号。CNend
\retval ::HI_SUCCESS Success                                             CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                              CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:模块没有初始化 CNend
\retval ::HI_ERR_KEYLED_INVALID_PARA  The input parameter is invalid.    CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_E enPin);

/**
\brief Sets the blink level of an LED.
CNcomment:\brief 设置LED 闪烁级别。CNend

\attention \n
The higher the level, the faster an LED blinks.
CNcomment:级别越高闪烁速度越快。CNend

\param[in] enLevel  Blink level from 1 to 5.                             CNcomment:闪烁级别，设置范围:1 到 5 CNend
\retval ::HI_SUCCESS Success                                             CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                              CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:模块没有初始化 CNend
\retval ::HI_ERR_KEYLED_INVALID_PARA The input parameter is invalid.     CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_E enLevel);

/**
\brief  whether enable the lockfreq led.
CNcomment:\brief 设置锁频LED。CNend

\attention \n
At present,only FD650 KEYLED support this.
CNcomment:目前只有FD650面板支持该设置CNend

\param[in] setLock  If lock ,set to HI_TRUE. Otherwise,set to HI_FALSE.                            CNcomment:LOCKLED设置，锁定时设置为HI_TRUE，否则，设置为HI_FALSE.CNend
\retval ::HI_SUCCESS Success                                             CNcomment:成功 CNend
\retval ::HI_FAILURE  Calling Ioctrl fails.                              CNcomment: Ioctrl调用失败 CNend
\retval ::HI_ERR_KEYLED_INVALID_PARA The input parameter is invalid.     CNcomment:输入参数非法 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_LED_SetLockPin(HI_BOOL setLock);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __HI_UNF_KEYLED_H__ */


