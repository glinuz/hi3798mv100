/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

/*******************************************************************************/

#ifndef __LOADERDB_H__
#define __LOADERDB_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#define STBINFO  "deviceinfo"

typedef unsigned short HI_U16;
typedef unsigned char  HI_U8;
typedef unsigned char  HI_UCHAR;
typedef unsigned int   HI_U32;

typedef short        HI_S16;
typedef char         HI_S8;
typedef char         HI_CHAR;
typedef int          HI_S32;
typedef void         HI_VOID;
typedef unsigned int HI_HANDLE;

typedef enum {
    HI_FALSE    = 0,
    HI_TRUE     = 1,
} HI_BOOL;

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_2*/
/** @{ */  /** <!- [Data structure of loader upgrade parameter area] */

/**
 \brief attention \n
Serial number information includes the hardware version and vendor ID to be verified during the upgrade. Different service provider and vendors have different serial numbers for STBs.\n
CNcomment:序列号含有升级时需要比较的硬件版本号,厂商编号。不同的运营商和厂家有不同的规格机顶盒的序列号。\n CNend
A serial number contains the following information:\n
          External serial number, such as  00000003-00000001-1d000021      \n
          External serial number is in the following format:\n
          AAAAAAAA-BBBBBBBB-CCCCCCCC ....\n
          AAAAAAAA 	Terminal product vendor authentication number\n
          BBBBBBBB 	Hardware version\n
          CCCCCCCC 	Hareware serial number.\n

CNcomment:序列号的格式组成如下: \n
          外置序列号如:  00000003-00000001-1d000021。 \n
          外置序列号格式如下: \n
          AAAAAAAA-BBBBBBBB-CCCCCCCC ....\n
          AAAAAAAA 	终端产品生产厂商认证编号。 \n
          BBBBBBBB 	硬件版本号。\n
          CCCCCCCC 	硬件系列号。\n CNend
 \param[out] pstStbinfo  STB serial number information. The memory is allocated externally.CNcomment:机顶盒序列号信息.外部分配内存。 CNend
 \retval ::HI_SUCCESS Success. CNcomment:成功。 CNend
 \retval ::HI_FAILURE  This API fails to be called. CNcomment:API系统调用失败。 CNend
 \see ::HI_LOADER_STB_INFO_S \n
For details, see the definition in HI_LOADER_STB_INFO_S.
CNcomment:详细信息,请参考HI_LOADER_STB_INFO_S定义。 CNend
 */
/**STB infor */
typedef struct hiLOADER_STBINFO_S
{
    HI_U32 u32OUI; /**< Vendor ID *//**<CNcomment: 厂商id */
    HI_U32 u32AreaCode; /**< Region code *//**<CNcomment: 区域码*/
    HI_U32 u32HWVersion; /**< Hardware version *//**<CNcomment: 硬件版本号 */
    HI_U32 u32HWSerialNum; /**< Hardware serial number *//**<CNcomment: 硬件系列号*/
} HI_LOADER_STB_INFO_S;

#ifdef __cplusplus
 #if __cplusplus
}

 #endif
#endif

#endif

