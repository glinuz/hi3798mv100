/*******************************************************************************
 *              Copyright 2005 - 2011, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName:    hi_drv_i2c.h
 * Description: This is internal include file for i2c
 *
 * History:
 * Version          Date            Author       DefectNum    Description
 *  0.01    2011-03-15     chenqiang      NULL       Create this file
 ******************************************************************************/
#ifndef __HI_DRV_I2C_H__
#define __HI_DRV_I2C_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3719cv100)

#define HI_STD_I2C_NUM  6

#elif  defined(CHIP_TYPE_hi3718mv100)

#define HI_STD_I2C_NUM  2

#elif  defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)|| defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)

#define HI_STD_I2C_NUM  3

#elif defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3798cv200) 

#define HI_STD_I2C_NUM  5

#elif  defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)

#define HI_STD_I2C_NUM  7

#elif defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b) 

#define HI_STD_I2C_NUM  8

#endif

#define HI_ERR_I2C_INVALID_PARA                     (HI_S32)(0x80440004)
#define HI_ERR_I2C_NULL_PTR                         (HI_S32)(0x80440005)
#define HI_ERR_I2C_WRITE_TIMEOUT                    (HI_S32)(0x80440008)
#define HI_ERR_I2C_READ_TIMEOUT                     (HI_S32)(0x80440009)
#define HI_ERR_I2C_READ_ACK_ERR                     (HI_S32)(0x8044000A)

/**Inter-integrated (I2C) channel used by the tuner*/
/**CNcomment:TUNER使用的I2C通道*/
#define HI_I2C_MAX_LENGTH  2048

#if 0
typedef enum hiUNF_TUNER_I2cChannel
{
    HI_UNF_I2C_CHANNEL_0 = 0, /**<I2C channel 0*/                               /**<CNcomment:I2C通道0*/
    HI_UNF_I2C_CHANNEL_1, /**<I2C channel 1*/                               /**<CNcomment:I2C通道1*/
    HI_UNF_I2C_CHANNEL_2, /**<I2C channel 2*/                               /**<CNcomment:I2C通道2*/
    HI_UNF_I2C_CHANNEL_3, /**<I2C channel 3*/                               /**<CNcomment:I2C通道3*/
    HI_UNF_I2C_CHANNEL_4, /**<I2C channel 4*/                               /**<CNcomment:I2C通道3*/
    HI_UNF_I2C_CHANNEL_MAX /**<Invalid value*/                               /**<CNcomment:非法边界值*/
} HI_UNF_TUNER_I2C_CHANNEL_E;
#endif

HI_S32 HI_DRV_I2C_SetRate(HI_U32 I2cNum, HI_U32 I2cRate);
HI_S32 HI_DRV_I2C_WriteConfig(HI_U32 I2cNum, HI_U8 I2cDevAddr);
HI_S32 HI_DRV_I2C_Write(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData, HI_U32 DataLen);
HI_S32 HI_DRV_I2C_Read(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 * pData, HI_U32 DataLen);
HI_S32 HI_DRV_I2C_Read_si(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 * pData, HI_U32 DataLen);
HI_S32 HI_DRV_I2C_Init(HI_VOID);
HI_VOID HI_DRV_I2C_Exit(HI_VOID);

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define I2C_DEGUB(format, arg...)
#else
//#define I2C_DEGUB(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define I2C_DEGUB(format...)     HI_ERR_PRINT(HI_ID_I2C, format)
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif


