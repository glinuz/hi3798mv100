#ifndef __HI_UNF_OTP_H__
#define __HI_UNF_OTP_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define HI_UNF_OTP_Open(HI_VOID) HI_UNF_OTP_Init(HI_VOID)
#define HI_UNF_OTP_Close(HI_VOID) HI_UNF_OTP_DeInit(HI_VOID)

/******************************* API declaration *****************************/
/** \addtogroup      OTP */
/** @{ */  /** <!-- [OTP] */


/**
\brief Init an otp device.
\brief CNcomment:初始化OTP设备。 CNend
\attention \n
N/A
\param N/A                                    				CNcomment:无。 CNend
\retval ::HI_SUCCESS  Call this API successful.           CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 				CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_Init(HI_VOID);

/**
\brief Deinit an otp device.
\brief CNcomment:去初始化OTP设备。 CNend
\attention \n
N/A
\param N/A                                    				CNcomment:无。 CNend
\retval ::HI_SUCCESS  Call this API successful.          CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 				CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_DeInit(HI_VOID);

/**
\brief Set customer key, the customer key is used by the customer to encrypt some private data.
\brief CNcomment:设置customer key，customer key由客户自己定义，且自定义用途。 CNend
\attention \n
N/A
\param[in] pKey:  Customer key to be written to otp.    				CNcomment:Customer key的值，待写入OTP。 CNend
\param[in] u32KeyLen:  The length of customer key, must be 16bytes.		CNcomment:Customer key的长度，必须为16字节。           CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_SetCustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen);

/**
\brief Get customer key, the customer key is used by the customer to encrypt some private data.
\brief CNcomment:获取customer key。 CNend
\attention \n
N/A
\param[in] pKey:Buffer to store the customer key read from otp.		CNcomment:存储customer key的buffer。 CNend
\param[in] u32KeyLen:The length of buffer, must be 16bytes. 			CNcomment:customer key 的长度，必须为16字节。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_GetCustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen);


/**
\brief Set stb private data, the stb private data is used by the customer to set some private data.
\brief CNcomment:设置客户私有数据。 CNend
\attention \n
N/A
\param[in] u32Offset:  The offset to set the private data, should be between 0 and 15.	CNcomment:设置stbprivData的偏移，必须为0~15之间。 CNend
\param[in] u8Data:  The private data to be set.							CNcomment:需要设置的值。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_SetStbPrivData(HI_U32 u32Offset, HI_U8 u8Data);

/**
\brief Get stb private data, the stb private data is used by the customer to set some private data.
\brief CNcomment:获取客户私有数据。 CNend
\attention \n
N/A
\param[in] u32Offset:  The offset to get the private data, should be between 0 and 15.  CNcomment:获取stbprivData的偏移，必须为0~15之间。 CNend
\param[out] pu8Data:  The data read from otp.  							CNcomment:获取的值。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_GetStbPrivData(HI_U32 u32Offset, HI_U8 *pu8Data);


/**
\brief  Set hdcp root key to otp, 16bytes length.
\brief CNcomment:向芯片内部写入hdcp root key，必须为16字节长。 CNend
\attention \n
N/A
\param[in] pu8RootKey:  Set hdcp root key to otp.					CNcomment设置hdcp root key到OTP。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_WriteHdcpRootKey(HI_U8 *pu8HdcpRootKey, HI_U32 u32Keylen);

/**
\brief  Get hdcp root key from otp, 16bytes length.
\brief CNcomment:读取芯片内部设置的hdcp root key，必须为16字节长。 CNend
\attention \n
N/A
\param[out] pu8RootKey:  Point to hdcp root key from otp.				CNcomment:从OTP中获取hdcp root key。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_ReadHdcpRootKey(HI_U8 *pu8HdcpRootKey, HI_U32 u32Keylen);

/**
\brief  Lock hdcp root key in otp.
\brief CNcomment:锁定芯片内部设置的hdcp root key。锁定后，hdcp root key将不可被读取。 CNend
\attention \n
N/A
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_LockHdcpRootKey(HI_VOID);

/**
\brief  Get hdcp root Key lock flag.
\brief CNcomment:读取芯片内部设置的hdcp root key的锁定标志位。 CNend
\attention \n
N/A
\param[out] pbLockFlag:  Point to hdcp root key lock flag from otp.		CNcomment:指向获取到的标志位。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_GetHdcpRootKeyLockFlag(HI_BOOL *pbLockFlag);

/**
\brief Set stb root key to otp.
\brief CNcomment:向芯片内部设置stb root key。 CNend
\attention \n
N/A
\param[in] u8StbRootKey:  Point to stb root key value.					CNcomment:Stb root key的值。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_WriteStbRootKey(HI_U8 *pu8StbRootKey, HI_U32 u32Keylen);

/**
\brief Get stb root key from otp.
\brief CNcomment:读取芯片内部设置的stb root key。 CNend
\attention \n
N/A
\param[out] pu8RootKey:  Point to stb root key from otp.				CNcomment:只想获取到的Stb root key。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_ReadStbRootKey(HI_U8 *pu8StbRootKey, HI_U32 u32Keylen);

/**
\brief Lock stb root key in otp.
\brief CNcomment:锁住芯片内部设置的stb root key。锁定后，stb root key将不可被读取。 CNend
\attention \n
N/A
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_LockStbRootKey(HI_VOID);

/**
\brief Get stb root Key lock flag
\brief CNcomment:获取芯片内部stb root key的锁定标志位。 CNend
\attention \n
N/A
\param[out] pbLockFlag:  Point to stb root key lock flag from otp.		CNcomment:指向获取到的标志位。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_GetStbRootKeyLockFlag(HI_BOOL *pbLockFlag);

/**
\brief Get ID Word lock flag
\brief CNcomment:获取芯片内部ID Word的锁定标志位。 CNend
\attention \n
N/A
\param[out] pbLockFlag:  Point to ID Word lock flag from otp.		CNcomment:指向获取到的标志位。 CNend
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_GetIDWordLockFlag(HI_BOOL *pbLockFlag);

/**
\brief burn chipset to normal chipset
\brief CNcomment:将芯片烧写成普通芯片。 CNend
\attention \n
N/A
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_BurnToNormalChipset(HI_VOID);

/**
\brief Burn nomal chipset to secure chipset
\brief CNcomment:将普通芯片烧写成安全芯片。 CNend
\attention \n
N/A
\retval ::HI_SUCCESS  Call this API successful.          			CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails. 							CNcomment:API系统调用失败。 CNend
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_BurnToSecureChipset(HI_VOID);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_OTP_H__ */