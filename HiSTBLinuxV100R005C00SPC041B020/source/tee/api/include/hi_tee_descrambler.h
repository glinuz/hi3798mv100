/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  hi_tee_descrambler.h
* Description:  tee interfaces of demux descrambler.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/

#ifndef __TEE_DESCRAMBLER_H__
#define __TEE_DESCRAMBLER_H__

#include "hi_type.h"
#include "hi_tee_errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      Descrambler */
/** @{ */  /** <!-- [Descrambler] */

/**Conditional access (CA) type, indicating whether advanced CA is used.*/
/**CNcomment:CA类型，是否使用高安全CA*/
typedef enum hiTEE_DMX_CA_TYPE_E
{
    HI_TEE_DMX_CA_NORMAL = 0,    /**<Common CA*/ /**< CNcomment:普通CA*/
    HI_TEE_DMX_CA_ADVANCE,       /**<Advanced CA*/ /**< CNcomment:高安全CA*/

    HI_TEE_DMX_CA_BUTT
} HI_TEE_DMX_CA_TYPE_E;

/**CA Entropy reduction mode*/
/**CNcomment:熵减少模式*/
typedef enum hiTEE_DMX_CA_ENTROPY_REDUCTION_E
{
    HI_TEE_DMX_CA_ENTROPY_REDUCTION_CLOSE = 0,  /**<64bit*/
    HI_TEE_DMX_CA_ENTROPY_REDUCTION_OPEN,       /**<48bit*/

    HI_TEE_DMX_CA_ENTROPY_REDUCTION_BUTT
} HI_TEE_DMX_CA_ENTROPY_E;


/**Type of the descrambler protocol.*/
/**CNcomment:解扰器协议类型*/
typedef enum hiTEE_DMX_DESCRAMBLER_TYPE_E
{
    HI_TEE_DMX_DESCRAMBLER_TYPE_CSA2      = 0,   /**<CSA2.0*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_CSA3      ,      /**<CSA3.0*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_AES_IPTV  ,      /**<AES IPTV of SPE*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_AES_ECB   ,      /**<SPE AES ECB*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_AES_CI    ,      /**<SPE AES CIPLUS*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_DES_CI    ,      /**<DES CIPLUS*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_DES_CBC   ,      /**<DES CBC*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_AES_NS    ,      /**<AES NS-Mode*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_SMS4_NS   ,      /**<SMS4 NS-Mode*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_SMS4_IPTV ,      /**<SMS4 IPTV*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_SMS4_ECB  ,      /**<SMS4 ECB*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_SMS4_CBC  ,      /**<SMS4 CBC*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_AES_CBC   ,      /**<AES CBC*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_DES_IPTV,
    HI_TEE_DMX_DESCRAMBLER_TYPE_TDES_IPTV,
    HI_TEE_DMX_DESCRAMBLER_TYPE_TDES_ECB,
    HI_TEE_DMX_DESCRAMBLER_TYPE_TDES_CBC,
    HI_TEE_DMX_DESCRAMBLER_TYPE_BUTT
} HI_TEE_DMX_DESCRAMBLER_TYPE_E;

/**Attribute of the key area.*/
/**CNcomment:密钥区属性*/
typedef struct hiTEE_DMX_DESCRAMBLER_ATTR_S
{
    HI_TEE_DMX_CA_TYPE_E enCaType;                    /**<Whether the descrambler adopts advanced CA.*/ /**< CNcomment:解扰器是否使用高安全CA*/
    HI_TEE_DMX_DESCRAMBLER_TYPE_E enDescramblerType;  /**<Descrambling protocol type of the descrambler*/ /**< CNcomment:解扰器解扰协议类型*/
    HI_TEE_DMX_CA_ENTROPY_E enEntropyReduction;       /**<CA Entropy reduction mode,for CSA2.0*/ /**< CNcomment:熵减少模式，CSA2.0有效*/
} HI_TEE_DMX_DESCRAMBLER_ATTR_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      Descrambler */
/** @{ */  /** <!--[Descrambler]*/

/**
\brief Creates a key area. The key area type and descrambling protocol type can be selected.CNcomment:创建一个密钥区,支持选择高安全CA和解扰协议类型。CNend
\attention \n
When an advanced CA key area is created, the descrambling protocol depends on the hardware and interface settings are ignored.\n
CNcomment:如果是高安全CA，解扰协议已经由硬件决定，接口的设置被忽略。CNend
\param[in] u32DmxId   DEMUX ID. CNcomment: DEMUX号。CNend
\param[in] pstDesramblerAttr  Pointer to the attributes of a key area.CNcomment:密钥区属性指针。CNend
\param[out] phKey      Pointer to the handle of a created key area.CNcomment:指针类型，输出申请到的密钥区Handle。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\retval ::HI_ERR_DMX_NOFREE_KEY  There is no available key area.CNcomment:没有空闲的密钥区。CNend
\retval ::HI_ERR_DMX_NOT_SUPPORT  Not support HI_TEE_DMX_DESCRAMBLER_ATTR_S type.CNcomment:不支持的HI_TEE_DMX_DESCRAMBLER_ATTR_S类型。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_CreateDescrambler(HI_U32 u32DmxId, const HI_TEE_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr, HI_HANDLE *phKey);

/**
\brief Destroys an existing key area.CNcomment:销毁创建的密钥区。CNend
\attention \n
If a key area is attached to a channel, the key area needs to be detached from the channel first, but the channel is not disabled.\n
If a key area is detached or destroyed before the attached channel is disabled, an error may occur during data receiving.
CNcomment:如果密钥区绑定在通道上，会先从通道上解绑定密钥区，但是注意不会关闭通道\n
如果没有关闭通道则进行密钥区的解绑定或销毁操作，可能导致数据接收的错误。CNend
\param[in] hKey  Handle of the key area to be destroyed.CNcomment:待删除的密钥区Handle。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_DestroyDescrambler(HI_HANDLE hKey);

/**
\brief Gets the attributes of a Descrambler.CNcomment:获取密钥配置属性。CNend
\attention \n
NA.\n
CNcomment:无。CNend
\param[in] hKey   key handle. CNcomment: key句柄。CNend
\param[out] pstAttr  Pointer to the attributes of a key area.CNcomment:密钥区属性指针。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\retval ::HI_ERR_DMX_NOFREE_KEY  There is no available key area.CNcomment:没有空闲的密钥区。CNend
\retval ::HI_ERR_DMX_NOT_SUPPORT  Not support HI_TEE_DMX_DESCRAMBLER_ATTR_S type.CNcomment:不支持的HI_TEE_DMX_DESCRAMBLER_ATTR_S类型。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_GetDescramblerAttr(HI_HANDLE hKey, HI_TEE_DMX_DESCRAMBLER_ATTR_S *pstAttr);

/**
\brief Sets the attributes of a Descrambler.CNcomment:设置密钥配置属性。CNend
\attention \n
NA.\n
CNcomment:无。CNend
\param[in] hKey   key handle. CNcomment: key句柄。CNend
\param[out] pstAttr  Pointer to the attributes of a key area.CNcomment:密钥区属性指针。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\retval ::HI_ERR_DMX_NOFREE_KEY  There is no available key area.CNcomment:没有空闲的密钥区。CNend
\retval ::HI_ERR_DMX_NOT_SUPPORT  Not support HI_TEE_DMX_DESCRAMBLER_ATTR_S type.CNcomment:不支持的HI_TEE_DMX_DESCRAMBLER_ATTR_S类型。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_SetDescramblerAttr(HI_HANDLE hKey, HI_TEE_DMX_DESCRAMBLER_ATTR_S *pstAttr);

/**
\brief Sets the even keys of a key area. This API is used to configure the DEMUX descrambler based on even keys after the CA system obtains control words.CNcomment:设置密钥区的偶密钥。CA系统得到控制字后，可调用本接口将偶密钥配置到DEMUX解扰模块。CNend
\attention \n
pEvenKey points to the even key data to be set. The data consists of 16 bytes: CW1, CW2, ..., and CW16.\n
The key value can be set dynamically, that is, the key value can be set at any time after a key area is created.\n
The initial value of each key is 0, which indicates that data is not descrambled.
CNcomment:pEvenKey指向要设置的偶密钥数据。数据共16byte，byte依次是CW1、CW2、……、CW16\n
支持密钥区的动态设置，可以在密钥区申请后的任意时刻设置密钥值\n
当设置密钥之前，密钥的初时值都是0，表示不做解扰。CNend
\param[in] hKey  Handle of the key area to be set.CNcomment:待设置的密钥区句柄。CNend
\param[in] pu8EvenKey  Pointer to the 16-byte even key data to be set.CNcomment:指针类型，指向要设置的偶密钥数据，必须是16个字节的数组。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_SetDescramblerEvenKey(HI_HANDLE hKey, const HI_U8 *pu8EvenKey);

/**
\brief Sets the odd keys of a key area. This API is used to configure the DEMUX descrambler based on odd keys after the CA system obtains control words.CNcomment:设置密钥区的奇密钥。CA系统得到控制字后，可调用本接口将奇密钥配置到DEMUX解扰模块。CNend
\attention \n
pOddKey points to the odd key data to be set. The data consists of 16 bytes: CW1, CW2, ..., and CW16.\n
The key value can be set dynamically, that is, the key value can be set at any time after a key area is created.\n
The initial value of each key is 0, which indicates that data is not descrambled.
CNcomment:pOddKey指向要设置的奇密钥数据。奇密钥数据共16byte，byte依次是CW1、CW2、……、CW16\n
支持密钥区的动态设置，可以在密钥区申请后的任意时刻设置密钥值\n
当设置密钥之前，密钥的初时值都是0，表示不做解扰。CNend
\param[in] hKey  Handle of the key area to be set.CNcomment:待设置的密钥区句柄。CNend
\param[in] pu8OddKey   Pointer to the 16-byte odd key data to be set.CNcomment:指针类型，指向要设置的奇密钥数据，必须是16个字节的数组。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_SetDescramblerOddKey(HI_HANDLE hKey, const HI_U8 *pu8OddKey);

/**
\brief Set Even IV.For algs do not use IV,do not care.CNcomment:设置偶密钥区的初始化向量。对于不涉及初始化向量的解扰算法可以不关注。CNend
\attention \n
pu8IVKey points to the iv key data to be set.The data consists of 16 bytes: CW1, CW2, ..., and CW16.\n
The key value can be set dynamically, that is, the key value can be set at any time after a key area is created.
CNcomment:pu8IVKey指向要设置的初始化向量数据。奇密钥数据共16byte，byte依次是CW1、CW2、……、CW16\n
支持密钥区的动态设置，可以在密钥区申请后的任意时刻设置。CNend
\param[in] hKey  Handle of the key area to be set.CNcomment:待设置的密钥区句柄。CNend
\param[in] pu8IVKey   Pointer to the 16-byte IV key data to be set.CNcomment:指针类型，指向要设置的奇密钥数据，必须是16个字节的数组。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_SetDescramblerEvenIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey);

/**
\brief Set Odd IV.For algs do not use IV,do not care.CNcomment:设置奇密钥区的初始化向量。对于不涉及初始化向量的解扰算法可以不关注。CNend
\attention \n
pu8IVKey points to the iv key data to be set.The data consists of 16 bytes: CW1, CW2, ..., and CW16.\n
The key value can be set dynamically, that is, the key value can be set at any time after a key area is created.
CNcomment:pu8IVKey指向要设置的初始化向量数据。奇密钥数据共16byte，byte依次是CW1、CW2、……、CW16\n
支持密钥区的动态设置，可以在密钥区申请后的任意时刻设置。CNend
\param[in] hKey  Handle of the key area to be set.CNcomment:待设置的密钥区句柄。CNend
\param[in] pu8IVKey    Pointer to the 16-byte IV key data to be set.CNcomment:指针类型，指向要设置的奇密钥数据，必须是16个字节的数组。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_SetDescramblerOddIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey);

/**
\brief Attaches a key area to a specific channel.CNcomment:绑定密钥区到指定通道。CNend
\attention \n
A key area can be attached to multiple channels that belong to different DEMUXs.\n
The static loading data in the key areas that are attached to all types of channels can be descrambled.\n
The same key area or different key areas cannot be attached to the same channel.
CNcomment:一个密钥区可以绑定到多个通道上，通道可以属于不同的DEMUX\n
可以对所有类型的通道绑定密钥区进行数据的解扰\n
不允许重复绑定相同或不同的密钥区到同一个通道上。CNend
\param[in] hKey    Handle of the key area to be attached.CNcomment:待绑定的密钥区句柄。CNend
\param[in] hChannel   Channel handle.CNcomment:通道句柄。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_ATTACHED_KEY  A key area is attached to the channel.CNcomment:通道上已经有一个密钥区绑定在上面。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_AttachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel);

/**
\brief Detaches a key area from a channel.CNcomment:将密钥区从通道上解绑定。CNend
\attention \n
The key area used by a channel can be detached dynamically. That is, you can call this API to detach a key area at any time after it is attached.\n
The scrambled data, however, may not be descrambled after the key area is detached, which causes data error.\n
The value of a key area retains even after it is detached. If the key area is attached again, its value is still the previously configured value.\n
If you do not want to descramble data, you can detach the corresponding key area or set all key values to 0.
CNcomment:可以动态的解绑定通道使用的密钥区，可以在绑定后的任意时刻使用此接口解绑定密钥区\n
但是解绑定后可能导致加扰数据没有被解扰，导致数据错误\n
解绑定密钥区并不能改变密钥区的值，如果重新绑定密钥区，密钥值仍然是上次设置的值\n
如果不想进行解扰，除了解绑定密钥区之外，也可以直接将密钥值全部设置为0来实现。CNend
\param[in] hKey    Handle of the key area to be detached.CNcomment:待解绑定的密钥区句柄。CNend
\param[in] hChannel  Channel handle.CNcomment:通道句柄。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NOATTACH_KEY  No key areas are attached to the channel.CNcomment:通道上没有绑定任何密钥区。CNend
\retval ::HI_ERR_DMX_UNMATCH_KEY  The specified key area is not attached to the specified channel.CNcomment:指定的密钥区没有绑定在指定的通道上。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_DetachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel);

/**
\brief Obtains the handle of the key area that is attached to a channel.CNcomment:获取通道绑定的密钥区句柄。CNend
\attention \n
If no key area is attached to the channel, the error code HI_ERR_DMX_NOATTACH_KEY is returned when you call this API.
CNcomment:当通道没有绑定密钥区时，调用本接口返回HI_ERR_DMX_NOATTACH_KEY错误码。CNend
\param[in] hChannel  Handle of the channel to be queried.CNcomment:要查询的通道句柄。CNend
\param[out] phKey     Pointer to the handle of the key area that is attached to a channel (output).CNcomment:指针类型，输出通道绑定的密钥区句柄。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\retval ::HI_ERR_DMX_NOATTACH_KEY  No key areas are attached to the channel.CNcomment:通道上没有绑定任何密钥区。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_GetDescramblerKeyHandle(HI_HANDLE hChannel, HI_HANDLE *phKey);

/**
\brief Obtains the handle of the channel with a specific PID.CNcomment:获取配置某PID的通道句柄。CNend
\attention \n
You must specify the DEMUX ID when calling this API, because the channel PIDs of different DEMUXs can be the same.\n
You cannot query the PID that is equal to or greater than 0x1FFF; otherwise, the error code HI_ERR_DMX_INVALID_PARA is returned.\n
If no channel with a specific PID is found, an error code is returned.
CNcomment:因不同DEMUX设备的通道允许设置相同的PID，调用本接口需要指定DEMUX ID\n
不允许查询0x1fff及以上的非法PID值，否则返回参数非法错误码\n
如果没有查询到任何通道设置了要查询的PID，将返回错误码。CNend
\param[in] u32DmxId    DEMUX ID. CNcomment: DEMUX号。CNend
\param[in] u32Pid      Channel PID.CNcomment:通道PID。CNend
\param[out] phChannel Pointer to channel handle.CNcomment: 指针类型，输出通道Handle。CNend
\retval ::HI_SUCCESS Success.CNcomment:成功。CNend
\retval ::HI_FAILURE  Calling this API fails.CNcomment:API系统调用失败。CNend
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:模块没有初始化。CNend
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:输入参数非法。CNend
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:指针参数为空。CNend
\retval ::HI_ERR_DMX_UNMATCH_CHAN  No matched channel is found.CNcomment:没有查询到匹配通道。CNend
\see \n
 N/A.CNcomment:无。CNend
*/
HI_S32 HI_TEE_DMX_GetChannelHandle(HI_U32 DmxId, HI_U32 Pid, HI_HANDLE *ChanHandle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __TEE_DESCRAMBLER_H__ */
