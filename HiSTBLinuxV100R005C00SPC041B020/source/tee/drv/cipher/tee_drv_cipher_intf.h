/******************************************************************************
 *
 * Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
 * ******************************************************************************
 * File Name     : drv_cipher_intf.h
 * Version       : Initial
 * Author        : Hisilicon hisecurity team
 * Created       : 2013-12-28
 * Last Modified :
 * Description   : 
 * Function List :
 * History       :
 * ******************************************************************************/
#ifndef __TEE_CIPHER_INTF_H__
#define __TEE_CIPHER_INTF_H__

//#include "hi_common.h"
#include "tee_drv_cipher_define.h"
#include "tee_drv_cipher.h"
#include "hi_tee_drv_cipher.h"
#include "hi_tee_cipher_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/******************************* API Declaration *****************************/
/** \addtogroup      CIPHER */
/** @{ */  /** <!-- [CIPHER] */
/* ---CIPHER---*/
/** 
\brief  Init the cipher device.  CNcomment:初始化CIPHER设备。 CNend
\attention \n
This API is used to start the cipher device.
CNcomment:调用此接口初始化CIPHER设备。 CNend
\param N/A                   													CNcomment:无 CNend
\retval ::HI_SUCCESS  Call this API succussful. 								CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                                   	CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_FAILED_INIT  The cipher device fails to be initialized. CNcomment:CIPHER设备初始化失败 CNend
\see \n
N/A
*/
HI_S32 Cipher_Init(	HI_VOID);

/** 
\brief  Deinit the cipher device.
CNcomment:\brief  去初始化CIPHER设备。 CNend
\attention \n
This API is used to stop the cipher device. If this API is called repeatedly, HI_SUCCESS is returned, but only the first operation takes effect.
CNcomment:调用此接口关闭CIPHER设备。重复关闭返回成功，第一次起作用。 CNend

\param N/A                   													CNcomment:无 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 		CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                            			CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized.  		CNcomment:CIPHER设备未初始化 CNend
\see \n
N/A
*/
HI_VOID Cipher_DeInit(HI_VOID);

/** 
\brief Obtain a cipher handle for encryption and decryption.
CNcomment：创建一路Cipher句柄。 CNend

\param[in] cipher attributes                                             		CNcomment:cipher 属性。 CNend
\param[out] phCipher Cipher handle                                       		CNcomment:CIPHER句柄。 CNend
\retval ::HI_SUCCESS Call this API succussful. 						 			CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE Call this API fails.										CNcomment: API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized.  		CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_POINT  The pointer is null.              		CNcomment:指针参数为空 CNend
\retval ::HI_ERR_CIPHER_FAILED_GETHANDLE  The cipher handle fails to be obtained, because there are no available cipher handles. CNcomment: 获取CIPHER句柄失败，没有空闲的CIPHER句柄 CNend
\see \n
N/A
*/
HI_S32 Cipher_CreateHandle(HI_HANDLE* phCipher, const HI_TEE_CIPHER_ATTS_S *pstCipherAttr);

/** 
\brief Destroy the existing cipher handle. CNcomment:销毁已存在的CIPHER句柄。 CNend
\attention \n
This API is used to destroy existing cipher handles.
CNcomment:调用此接口销毁已经创建的CIPHER句柄。 CNend

\param[in] hCipher Cipher handle                 								CNcomment:CIPHER句柄。 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 		CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.    									CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. 		CNcomment:CIPHER设备未初始化 CNend
\see \n
N/A
*/
HI_S32 Cipher_DestroyHandle(HI_HANDLE hCipher);

/** 
\brief Configures the cipher control information.
CNcomment:\brief 配置CIPHER控制信息。 CNend
\attention \n
Before encryption or decryption, you must call this API to configure the cipher control information.
The first 64-bit data and the last 64-bit data should not be the same when using TDES algorithm.
CNcomment:进行加密解密前必须先使用此接口配置CIPHER的控制信息。使用TDES算法时，输入密钥的前后64 bit数据不能相同。 CNend

\param[in] hCipher Cipher handle.                                        		CNcomment:CIPHER句柄 CNend
\param[in] pstCtrl Cipher control information.                           		CNcomment:CIPHER控制信息 CNend
\retval ::HI_SUCCESS Call this API succussful. 						 			CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE Call this API fails.                           			CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. 		CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_POINT  The pointer is null.             		CNcomment:指针参数为空 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         		CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          		CNcomment:句柄非法 CNend
\see \n
N/A
*/
HI_S32 Cipher_ConfigHandle(HI_HANDLE hCipher, HI_TEE_CIPHER_CTRL_S* pstCtrl);

HI_S32 Cipher_GetHandleConfig(HI_HANDLE hCipher, HI_TEE_CIPHER_CTRL_S* pstCtrl);

/** 
\brief Performs encryption. 
CNcomment:\brief 进行加密。 CNend

\attention \n
This API is used to perform encryption by using the cipher module.
The length of the encrypted data should be a multiple of 8 in TDES mode and 16 in AES mode. Besides, the length can not be bigger than 0xFFFFF.After this operation, the result will affect next operation.If you want to remove vector, you need to config IV(config pstCtrl->stChangeFlags.bit1IV with 1) by transfering HI_TEE_CIPHER_ConfigHandle.
CNcomment:使用CIPHER进行加密操作。TDES模式下加密的数据长度应当是8的倍数，AES下应当是16的倍数。此外，加密数据长度不能长于0xFFFFF。本次操作完成后，此次操作的向量运算结果会作用于下一次操作，如果要清除向量，需要在下次加密操作之前调用HI_TEE_CIPHER_ConfigHandle重新配置IV(需要设置pstCtrl->stChangeFlags.bit1IV为1)。 CNend
\param[in] hCipher Cipher handle  												CNcomment:CIPHER句柄 CNend
\param[in] u32SrcPhyAddr Physical address of the source data            		CNcomment:源数据物理地址 CNend
\param[in] u32DestPhyAddr Physical address of the target data           		CNcomment:目的数据物理地址 CNend
\param[in] u32ByteLength   Length of the encrypted data                 		CNcomment:加密数据长度 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 		CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                           			CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. 		CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         		CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          		CNcomment:句柄非法 CNend
\see \n
N/A
*/
HI_S32 Cipher_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);

/** 
\brief 进行加密
\attention \n
This API is used to perform decryption by using the cipher module.
The length of the decrypted data should be a multiple of 8 in TDES mode and 16 in AES mode. Besides, the length can not be bigger than 0xFFFFF.After this operation, the result will affect next operation.If you want to remove vector, you need to config IV(config pstCtrl->stChangeFlags.bit1IV with 1) by transfering HI_TEE_CIPHER_ConfigHandle.
CNcomment:使用CIPHER进行解密操作。TDES模式下解密的数据长度应当是8的倍数，AES下应当是16的倍数。此外，解密数据长度不能长于0xFFFFF。本次操作完成后，此次操作的向量运算结果会作用于下一次操作，如果要清除向量，需要在下次解密操作之前调用HI_TEE_CIPHER_ConfigHandle重新配置IV(需要设置pstCtrl->stChangeFlags.bit1IV为1)。 CNend
\param[in] hCipher Cipher handle.                                        		CNcomment:CIPHER句柄 CNend
\param[in] u32SrcPhyAddr Physical address of the source data.            		CNcomment:源数据物理地址 CNend
\param[in] u32DestPhyAddr Physical address of the target data.           		CNcomment:目的数据物理地址 CNend
\param[in] u32ByteLength Length of the decrypted data                   		CNcomment:解密数据长度 CNend
\retval ::HI_SUCCESS Call this API succussful. 						 			CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE Call this API fails.                           			CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. 		CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         		CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          		CNcomment:句柄非法 CNend
\see \n
N/A
*/
HI_S32 Cipher_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);


/** 
\brief Encrypt multiple packaged data.
CNcomment:\brief 进行多个包数据的加密。 CNend
\attention \n
You can not encrypt more than 128 data package one time. When HI_ERR_CIPHER_BUSY return, the data package you send will not be deal, the custmer should decrease the number of data package or run cipher again.Note:When encrypting more than one packaged data, every one package will be calculated using initial vector configured by HI_TEE_CIPHER_ConfigHandle.Previous result will not affect the later result.
CNcomment:每次加密的数据包个数最多不能超过128个。返回HI_ERR_CIPHER_BUSY的时候，送入数据包一个也不会被处理，用户需要减少送入的数据包的数量或者再次尝试加密。注意: 对于多个包的操作，每个包都使用HI_TEE_CIPHER_ConfigHandle配置的向量进行运算，前一个包的向量运算结果不会作用于下一个包的运算，每个包都是独立运算的。前一次函数调用的结果也不会影响后一次函数调用的运算结果。 CNend
\param[in] hCipher cipher handle                                                                  CNcomment:CIPHER句柄。 CNend
\param[in] pstDataPkg data package ready for cipher                                               CNcomment:待加密的数据包。 CNend
\param[in] u32DataPkgNum  number of package ready for cipher                                      CNcomment:待加密的数据包个数。 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 						  CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                                                     	  CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  cipher device have not been initialized                         CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  parameter error                                             CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  handle invalid                                            CNcomment:句柄非法 CNend
\retval ::HI_ERR_CIPHER_BUSY  hardware is busy, it can not deal with all data package once time   CNcomment:硬件正忙，无法一次性处理全部的数据包 CNend
\see \n
N/A
*/
HI_S32 Cipher_EncryptMulti(HI_HANDLE hCipher, HI_TEE_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum);


/** 
\brief Decrypt multiple packaged data.
CNcomment:\brief 进行多个包数据的解密。 CNend
\attention \n
You can not decrypt more than 128 data package one time.When HI_ERR_CIPHER_BUSY return, the data package you send will not be deal, the custmer should decrease the number of data package or run cipher again.Note:When decrypting more than one packaged data, every one package will be calculated using initial vector configured by HI_TEE_CIPHER_ConfigHandle.Previous result will not affect the later result. 
CNcomment:每次解密的数据包个数最多不能超过128个。返回HI_ERR_CIPHER_BUSY的时候，送入数据包一个也不会被处理，用户需要减少送入的数据包的数量或者再次尝试解密。注意: 对于多个包的操作，每个包都使用HI_TEE_CIPHER_ConfigHandle配置的向量进行运算，前一个包的向量运算结果不会作用于下一个包的运算，每个包都是独立运算的，前一次函数调用的结果也不会影响后一次函数调用的运算结果。 CNend
\param[in] hCipher cipher handle                                                                 CNcomment:CIPHER句柄。 CNend
\param[in] pstDataPkg data package ready for cipher                                              CNcomment:待解密的数据包。 CNend
\param[in] u32DataPkgNum  number of package ready for cipher                                     CNcomment:待解密的数据包个数。 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 						 CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                                                    	 CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  cipher device have not been initialized                        CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  parameter error                                            CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  handle invalid                                           CNcomment:句柄非法 CNend
\retval ::HI_ERR_CIPHER_BUSY  hardware is busy, it can not deal with all data package once time  CNcomment:硬件正忙，无法一次性处理全部的数据包 CNend
\see \n
N/A
*/
HI_S32 Cipher_DecryptMulti(HI_HANDLE hCipher, HI_TEE_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum);

/** 
\brief used for AES CBC CTS mode decryption only.
CNcomment:\brief 仅适用于AES CBC CTS特殊模式的解密。 CNend
\attention \n
\param[in] hCipher cipher handle                                                                 CNcomment:CIPHER句柄。 CNend
\param[in] u32SrcPhyAddr address of input data                                              	 CNcomment:待解密的数据包的地址。 CNend
\param[in] u32DestPhyAddr  address of output data                                     			 CNcomment:解密结果的输出地址。 CNend
\param[in] au8IV  Initial vector for decryption                                     			 CNcomment:初始化向量。 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 						 CNcomment:API系统调用成功 CNend
\retval ::HI_FAILURE  Call this API fails.                                                    	 CNcomment:API系统调用失败 CNend
\retval ::HI_ERR_CIPHER_NOT_INIT  cipher device have not been initialized                        CNcomment:CIPHER设备未初始化 CNend
\retval ::HI_ERR_CIPHER_INVALID_PARA  parameter error                                            CNcomment:参数错误 CNend
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  handle invalid                                           CNcomment:句柄非法 CNend
\retval ::HI_ERR_CIPHER_BUSY  hardware is busy, it can not deal with all data package once time  CNcomment:硬件正忙，无法一次性处理全部的数据包 CNend
\see \n
N/A
*/
HI_S32 Cipher_Decrypt_AESCBCCTS(HI_HANDLE hCipherHandle, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength, const HI_U8 au8IV[16]);


/** 
\brief Init the hash module, if other program is using the hash module, the API will return failure.
CNcomment:\brief 初始化HASH模块，如果有其他程序正在使用HASH模块，返回失败状态。 CNend

\attention \n
N/A

\param[in] pstHashAttr: The hash calculating structure input.                                      CNcomment:用于计算hash的结构体参数 CNend
\param[out] pHashHandle: The output hash handle.               									CNcomment:输出的hash句柄 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 						CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                       								CNcomment: API系统调用失败 CNend

\see \n
N/A
*/
HI_S32 Cipher_HashInit(HI_TEE_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle);

/** 
\brief Calculate the hash, if the size of the data to be calculated is very big and the DDR ram is not enough, this API can calculate the data one block by one block.
CNcomment:\brief 计算hash值，如果需要计算的数据量比较大，该接口可以实现一个block一个block的计算，避免数据量比较大的情况下，内存不足的问题。 CNend

\attention \n
N/A

\param[in] hHashHandl:  Hash handle.                                  		CNcomment:Hash句柄。 CNend
\param[in] pu8InputData:  The input data buffer.                  			CNcomment:输入数据缓冲 CNend
\param[in] u32InputDataLen:  The input data length, attention: the block length input must be 4bytes aligned except the last block!			CNcomment:输入数据的长度。重要： 输入数据块的长度必须是4字节对齐，最后一个block无此限制。 CNend
\retval ::HI_SUCCESS  Call this API succussful. 						 	CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                       		CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 Cipher_HashUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen);



/** 
\brief Get the final hash value, after calculate all of the data, call this API to get the final hash value and close the handle.If there is some reason need to interupt the calculation, this API should also be call to close the handle.
CNcomment:获取hash值，在计算完所有的数据后，调用这个接口获取最终的hash值，该接口同时会关闭hash句柄。如果在计算过程中，需要中断计算，也必须调用该接口关闭hash句柄。 CNend

\attention \n
N/A

\param[in] hHashHandle:  Hash handle.                                  		CNcomment:Hash句柄。 CNend
\param[out] pu8OutputHash:  The final output hash value.                   	CNcomment:输出的hash值。 CNend

\retval ::HI_SUCCESS  Call this API succussful. 						 	CNcomment:API系统调用成功。 CNend
\retval ::HI_FAILURE  Call this API fails.                       		CNcomment:API系统调用失败。 CNend

\see \n
N/A
*/
HI_S32 Cipher_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash);

HI_S32 Cipher_HashHmacInit(HI_HANDLE hHashHandle, HI_U32 u32TotalLen);
HI_S32 Cipher_GetRandomNumber(HI_U32 *pu32Rnd);
HI_S32 Cipher_GetIVOut(HI_HANDLE hCipher, HI_U8 *pu8IVOut);
HI_S32 cipher_drv_modInit(HI_VOID);

HI_S32 Cipher_Test(HI_U32 u32Cmd, HI_VOID *pu8Param, HI_U32 u32Size);

HI_S32 DRV_CIPHER_CalcRsa_SW(CIPHER_RSA_DATA_S *pCipherRsaData);
HI_S32 DRV_CIPHER_GenRsaKey_SW(CIPHER_RSA_KEY_S *pstRsaKey);
HI_S32 DRV_CIPHER_RSA_CallRMul(HI_U8*X, HI_U8* A, HI_U8* B, HI_U32 Xsize, HI_U32 Asize, HI_U32 Bsize);

HI_VOID * s_Cipher_RemapPhyAddr(HI_U32 u32PhyAddr, HI_BOOL bIsSMMu, HI_BOOL bIsSec);
HI_VOID s_Cipher_UnmapPhyAddr(HI_U32 u32PhyAddr, HI_VOID* pu8SrcVirAddr, HI_BOOL bIsSMMu, HI_BOOL bIsSec);

HI_S32 TEE_CIPHER_Ioctl(HI_U32 cmd, HI_VOID *argp);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_CIPHER_INTF_H__ */
