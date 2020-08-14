/******************************************************************************

  Copyright (C), 2001-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
 File Name     : hi_unf_i2c.h
Version       : Initial draft
Author        : HiSilicon multimedia software group
Created Date   : 2008-06-05
Last Modified by:
Description   : Application programming interfaces (APIs) of the external chip software (ECS)
Function List :
Change History:
******************************************************************************/
#ifndef __HI_UNF_I2C_H__
#define __HI_UNF_I2C_H__

#include "hi_common.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      I2C */
/** @{*/  /** <!-[I2C]*/

#define HI_UNF_I2C_Open  HI_UNF_I2C_Init
#define HI_UNF_I2C_Close HI_UNF_I2C_DeInit

/**Maximum I2C channel ID*/ /**CNcomment:最大I2C通道号*/
#define HI_I2C_MAX_NUM_USER (15)

/**Rate type of the I2C module*/
/**CNcomment:I2C的速率类型 */
typedef enum hiUNF_I2C_RATE_E
{
    HI_UNF_I2C_RATE_10K = 0, /**<Standard rate: 10 kbit/s*/         /**<CNcomment:标准速率：10kbit/s*/
    HI_UNF_I2C_RATE_50K, /**<Standard rate: 50 kbit/s*/   /**<CNcomment:标准速率：50kbit/s*/
    HI_UNF_I2C_RATE_100K, /**<Standard rate: 100 kbit/s*/  /**<CNcomment:标准速率：100kbit/s*/
    HI_UNF_I2C_RATE_200K, /**<Standard rate: 200 kbit/s*/  /**<CNcomment:标准速率：200kbit/s*/
    HI_UNF_I2C_RATE_300K, /**<Standard rate: 300 kbit/s*/  /**<CNcomment:标准速率：300kbit/s*/
    HI_UNF_I2C_RATE_400K, /**<Fast rate: 400 kbit/s*/      /**<CNcomment:快速速率：400kbit/s*/

    HI_UNF_I2C_RATE_BUTT
} HI_UNF_I2C_RATE_E;

/** @}*/  /** <!-- ==== Structure Definition End ====*/




/******************************* API Declaration *****************************/
/** \addtogroup      I2C*/
/** @{*/  /** <!-- -I2C=*/

/**
 \brief Init the I2C device.
CNcomment:\brief 初始化I2C（the Inter-Integrated Circuit）设备。CNend

 \param N/A                                                               CNcomment:无。CNend
 \retval 0 Success                                                        CNcomment:成功。CNend
 \retval ::HI_ERR_I2C_OPEN_ERR  Open I2c Error				  CNcomment:I2C无效参数。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_Init (HI_VOID);

/**
 \brief  DeInit the I2C device.
CNcomment:\brief 去初始化I2C设备。CNend

 \attention \n
This API is called after I2C operations are completed.\n
CNcomment:在I2C操作结束后调用此接口\n CNend

 \param N/A                                                        CNcomment:无。CNend
 \retval 0 Success                                                 CNcomment: 成功。CNend
 \retval ::HI_ERR_I2C_CLOSE_ERR  Close I2c Error.	 	   CNcomment:I2C设备未初始化。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_DeInit(HI_VOID);

/**
 \brief  Get the number of I2C module.
CNcomment:\brief 去初始化I2C设备。CNend

 \attention \n
Call this API to get the number of I2C module befor read/write data.\n
CNcomment:在使用I2C读写数据之前调用此接口获取芯片的I2C模块数目\n CNend

 \param N/A                                                        CNcomment:无。CNend
 \retval 0 Success                                                 CNcomment: 成功。CNend
 \retval ::	 
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_GetCapability(HI_U32 *pu32I2cNum);

/**
The I2C device is not initialized.
CNcomment:\brief 创建一路GpioI2c。CNend

 \attention \n
If the specified GPIO pins are used, this API fails to be called.\n
CNcomment:如果Gpio已被占用会失败\n CNend

 \param[out] u32I2cNum  ID of the obtained I2C bus                            CNcomment:得到I2C总线号。CNend
 \param[in] u32SCLGpioNo  SCL Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \param[in] u32SDAGpioNo  SDA Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:管脚号，取值范围为0～103，不同的芯片类型范围不一样。CNend
 \retval 0 Success                                                           CNcomment:成功。CNend
 \retval ::HI_FAILURE	Create gpioi2c failed								CNcomment:失败。CNend
 \retval ::HI_ERR_I2C_NULL_PTR 		The pointer parameter is NULL			CNcomment:指针参数为空。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA  The parameter is invalid.               CNcomment:参数非法。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_CreateGpioI2c(HI_U32 *pu32I2cNum, HI_U32 u32SCLGpioNo, HI_U32 u32SDAGpioNo);

/**
 \brief Destroys a inter-integrated circuit (I2C) channel that simulates the general-purpose input/output (GPIO) function.
CNcomment:\brief 销毁一路GpioI2c。CNend

 \attention \n
If the I2C channel is not used, a code indicating success is returned.\n
CNcomment:如果该GpioI2c未使用，直接返回成功\n CNend

 \param[in] u32I2cNum ID of the I2C bus to be destroyed        CNcomment:要销毁的I2C总线号。CNend
 \retval 0  Success                                            CNcomment:成功。CNend
 \retval ::HI_FAILURE	Destroy gpioi2c failed				  CNcomment:失败。CNend
 \retval ::HI_ERR_GPIO_INVALID_PARA  The parameter is invalid. CNcomment:参数非法。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_DestroyGpioI2c(HI_U32 u32I2cNum);

/**
 \brief Reads data by using the I2C bus.
CNcomment:\brief 通过I2C读数据。CNend

 \attention \n
N/A
 \param[in] u32I2cNum  I2C bus of the device to be read           CNcomment:所读取设备使用的I2C总线。CNend
 \param[in] u8DevAddress  Address of a device on the I2C bus      CNcomment:设备在I2C总线上的地址。CNend
 \param[in] u32RegAddr  On-chip offset address of a device        CNcomment:设备的片内偏移地址。CNend
 \param[in] u32RegAddrCount  Length of an on-chip offset address. CNcomment:片内偏移地址的长度单位。CNend
                      1: 8-bit sub address                       CNcomment:1：表示8bit子地址；CNend
                      2: 16-bit sub address                      CNcomment:2：表示16bit子地址；CNend
                      3: 24-bit sub address                      CNcomment:3：表示24bit子地址；CNend
                      4: 32-bit sub address                      CNcomment:4：表示32bit子地址。CNend

 \param[out] pu8Buf   Buffer for storing the data to be read                            CNcomment:读Buffer，存放读取数据。CNend
 \param[in] u32Length  Length of the data to be read                                    CNcomment:要读取的数据长度。CNend
 \retval 0 Success                                                                      CNcomment:成功。CNend
 \retval ::HI_FAILURE	Read data failed					  	CNcomment:失败。CNend
 \retval ::HI_ERR_I2C_NOT_INIT  The I2C device is not initialized.                      CNcomment:I2C设备未初始化。CNend
 \retval ::HI_ERR_I2C_NULL_PTR  The I2C pointer is invalid.                        	   CNcomment:I2C无效指针。CNend
 \retval ::HI_ERR_I2C_INVALID_PARA  The I2C parameter is invalid.                       CNcomment:I2C无效参数。CNend
 \retval ::HI_ERR_I2C_FAILED_READ  Data fails to be read by using the I2C bus.          CNcomment:I2C读数据失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_Read(HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U32 u32RegAddr,
                       HI_U32 u32RegAddrCount, HI_U8 *pu8Buf, HI_U32 u32Length);

/**
 \brief Writes data by using the I2C bus. That is, you can call this API to write data to the device mounted on the I2C bus through the I2C channel.
CNcomment:\brief 通过I2C写数据。通过I2C通道，向I2C总线上挂接的设备进行写操作。CNend

 \attention \n
N/A
 \param[in] u32I2cNum  I2C bus of the device to be written         CNcomment:待写设备使用的I2C总线。CNend
 \param[in] u8DevAddress  Address of a device on the I2C bus       CNcomment:设备在I2C总线上的地址。CNend
 \param[in] u32RegAddr  On-chip offset address of a device         CNcomment:设备的片内偏移地址。CNend
 \param[in] u32RegAddrCount Length of an on-chip offset address.   CNcomment:片内偏移地址的长度单位。CNend
                    1: 8-bit sub address                          CNcomment:1：表示8bit子地址；CNend
                    2: 16-bit sub address                         CNcomment:2：表示16bit子地址；CNend
                    3: 24-bit sub address                         CNcomment:3：表示24bit子地址；CNend
                    4: 32-bit sub address                         CNcomment:4：表示32bit子地址。CNend

 \param[in]  pu8Buf   Buffer for storing the data to be written                         CNcomment:写Buffer，存放待写入数据。CNend
 \param[in] u32Length  Length of the data to be written                                 CNcomment:要写入的数据的长度。CNend
 \retval 0  Success                                                                     CNcomment:成功。CNend
 \retval ::HI_FAILURE	Write data failed					  	CNcomment:失败。CNend
 \retval ::HI_ERR_I2C_NOT_INIT  The I2C device is not initialized.                      CNcomment:I2C设备未初始化。CNend
 \retval ::HI_ERR_I2C_NULL_PTR  The I2C pointer is invalid.                        	   CNcomment:I2C无效指针。CNend
 \retval ::HI_ERR_I2C_INVALID_PARA  The I2C parameter is invalid.                       CNcomment:I2C无效参数。CNend
 \retval ::HI_ERR_I2C_FAILED_WRITE  Data fails to be written by using the I2C bus.      CNcomment:I2C写数据失败。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_Write(HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U32 u32RegAddr,
                        HI_U32 u32RegAddrCount, HI_U8 * pu8Buf, HI_U32 u32Length);

/**
 \brief Sets the transfer rate of the I2C bus.
CNcomment:\brief 设置I2C的传输速率。CNend

 \attention \n
Call this API Only be effect in standard i2c, gpio simulate i2c is noneffective.\n 
If you do not call this API to set the transfer rate, the rate 100 kbit/s is used by default.\n
CNcomment:此接口仅对标准的i2c通道有效，对gpio 模拟 i2c 无效。\nCNend
CNcomment:如果不调用此接口，系统采用400Kbit/s作为缺省速率。\n CNend

 \param[in] u32I2cNum  D of channel corresponding to the device to be written on the I2C bus                         CNcomment:所写设备在I2C总线上的通道号。CNend
 \param[in] enI2cRate  I2C clock rate. For details about the definition, see the description of ::HI_UNF_I2C_RATE_E. CNcomment:I2C时钟速率。含义请参考::HI_UNF_I2C_RATE_E。CNend
 \retval 0  Success                                                                                                  CNcomment:成功。CNend
 \retval ::HI_FAILURE	Set rate failed									   	     CNcomment:失败。CNend
 \retval ::HI_ERR_I2C_NOT_INIT  The I2C device is not initialized.                                                   CNcomment:I2C设备未初始化。CNend
 \retval ::HI_ERR_I2C_INVALID_PARA  The I2C parameter is invalid.                                                    CNcomment:I2C无效参数。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_SetRate(HI_U32 u32I2cNum, HI_UNF_I2C_RATE_E enI2cRate);

/**
 \brief Sets the transfer rate of the I2C bus.
CNcomment:\brief 设置I2C的传输速率。CNend

 \attention \n
Call this API Only be effect in standard i2c, gpio simulate i2c is noneffective.\n 
If you do not call this API to set the transfer rate, the rate 100 kbit/s is used by default.\n
CNcomment:此接口仅对标准的i2c通道有效，对gpio 模拟 i2c 无效。\n
如果不调用此接口，系统采用400Kbit/s作为缺省速率。\n CNend

 \param[in] u32I2cNum  D of channel corresponding to the device to be written on the I2C bus                         CNcomment:所写设备在I2C总线上的通道号。CNend
 \param[in] u32I2cRate  I2C clock rate.  CNcomment:I2C时钟速率。CNend
 \retval 0  Success                                                                                                  CNcomment:成功。CNend
 \retval ::HI_FAILURE	Set rate failed									   	     CNcomment:失败。CNend
 \retval ::HI_ERR_I2C_NOT_INIT  The I2C device is not initialized.                                                   CNcomment:I2C设备未初始化。CNend
 \retval ::HI_ERR_I2C_INVALID_PARA  The I2C parameter is invalid.                                                    CNcomment:I2C无效参数。CNend
 \see \n
N/A
 */
HI_S32 HI_UNF_I2C_SetRateEx(HI_U32 u32I2cNum, HI_U32 u32I2cRate);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_UNF_ECS_TYPE_H__ */
