/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_verify_api.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __CA_VERIFY_API_H__
#define __CA_VERIFY_API_H__

#include "hi_type.h"
#include "ca_verify_def.h"

#define ADVCA_MODE    0
#define COMMON_MODE     1


/**
\brief  Verify partition signature by partitionname, signature partitionname and its offset
CNcomment:根据flash分区名、签名分区名和偏移量，校验flash内容
\param[in] u32MemAddr   RAM address for image               CNcomment:pPartionName  存放image的内存地址
\param[in] pPartitionName    Flash partion name             CNcomment:pPartionName  flash 分区名
\param[in] PartitionSignName Flash signature partion name   CNcomment:PartitionSignName  签名分区名
\param[in] offset            The offset of sign header      CNcomment:offset  签名头部的偏移量
\retval ::0 Success CNcomment:0                  成功
\retval ::-1 Faliure CNcomment:-1                失败
\see \n
None CNcomment:无
*/
HI_S32 HI_CA_Verify_Signature(HI_U32 u32MemAddr, HI_CHAR *pPartitionName, HI_CHAR *PartitionSignName, HI_U32 offset);

/*
    Choose randomly partial section to verify partition.
*/
HI_S32 HI_CA_Verify_Signature_Ex(HI_U32 u32MemAddr,HI_CHAR *PartitionImagenName, HI_CHAR *PartitionSignName, HI_U32 offset);



/**
\brief  verify bootargs parameter area
CNcomment:校验bootargs分区flash内容
\attention \n
\param[in] u32MemAddr         RAM address for image       CNcomment:pPartionName  存放image的内存地址
\param[in] u32ParaPartionAddr Bootargs partion address    CNcomment:u32ParaPartionAddr  bootargs分区地址
\param[in] u32ParaParitonSize Bootargs partion size       CNcomment:u32ParaParitonSize  bootargs分区大小
\retval ::0 Success CNcomment:0                  成功
\retval ::-1 Faliure CNcomment:-1                失败
\see \n
None CNcomment:无
*/
HI_S32 HI_CA_Verify_BootPara(HI_U32 u32MemAddr, HI_U32 u32ParaPartionAddr, HI_U32 u32ParaParitonSize);


/**
\brief  Get advaced CA image header structure by flash partition name
CNcomment:通过flash分区的名字获取高安image头部信息
\attention \n
\param[in] pPartionName Flash partion name                CNcomment:pPartionName  flash 分区名
\param[in] pstAuthInfo  Authentication configure infor    CNcomment:pstAuthInfo  校验配置信息
\param[out] pstImgInfo  Image header info                 CNcomment:pstAuthInfo  镜像头部信息
\param[out] pIsEncrypt  Is the image encrypted            CNcomment:ImgInDDRAddress 镜像是否加密
\retval ::0 Success CNcomment:0                  成功
\retval ::-1 Faliure CNcomment:-1                失败
\see \n
None CNcomment:无
*/
HI_S32 HI_CA_GetAdvcaImgInfoByPartName(HI_U8* pPartionName, HI_CA_AuthInfo_S *pstAuthInfo, HI_CAImgHead_S* pstImgInfo, HI_BOOL *pIsEncrypt);


/**
\brief  Get advaced CA image header structure by flash partition addr
CNcomment:通过flash分区的地址获取高安image头部信息
\attention \n
\param[in] PartionAddr Flash partion address              CNcomment:pPartionName  flash 分区地址
\param[in] pstAuthInfo  Authentication configure infor    CNcomment:pstAuthInfo  校验配置信息
\param[out] pstImgInfo  Image header info                 CNcomment:pstAuthInfo  镜像头部信息
\param[out] pIsEncrypt  Is the image encrypted            CNcomment:ImgInDDRAddress 镜像是否加密
\retval ::0 Success CNcomment:0                  成功
\retval ::-1 Faliure CNcomment:-1                失败
\see \n
None CNcomment:无
*/
HI_S32 HI_CA_GetAdvcaImgInfoByAddr(HI_U32 PartionAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_CAImgHead_S* pstImgInfo, HI_BOOL *pIsEncrypt);


/**
\brief  In advca mode, verify flash contents by partion name
CNcomment:高安模式下根据flash分区名校验flash内容
\attention \n
如果是加密存储在flash的镜像，会被先解密，然后校验签名；如果是明文镜像，直接校验签名。
\param[in] pPartionName Flash partion name    CNcomment:pPartionName  flash 分区名
\param[in] u32RamAddr   RAM address for image CNcomment:pPartionName  存放image的内存地址
\param[in] pstAuthInfo  Authentication configure infor    CNcomment:pstAuthInfo  校验配置信息
\param[out] pu32CodeAddr After Authenticate finished ,image address in DDR CNcomment:ImgInDDRAddress 校验完成后，可用镜像处于DDR中的位置
\retval ::0 Success CNcomment:0                  成功
\retval ::-1 Faliure CNcomment:-1                失败
\see \n
None CNcomment:无
*/
HI_S32 HI_CA_AdvcaVerifyByPartName(HI_U8* pPartionName,HI_U32 u32RamAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_U32 *pu32CodeAddr);


/**
\brief  In advca mode, verify flash contents by partion addr
CNcomment:高安模式下根据flash分区地址校验flash内容
\attention \n
如果是加密存储在flash的镜像，会被先解密，然后校验签名；如果是明文镜像，直接校验签名。
\param[in] pPartionName Flash partion address CNcomment:pPartionName  flash 分区addr
\param[in] u32RamAddr   RAM address for image CNcomment:pPartionName  存放image的内存地址
\param[in] pstAuthInfo  Authentication configure infor    CNcomment:pstAuthInfo  校验配置信息
\param[out] pu32CodeAddr After Authenticate finished ,image address in DDR CNcomment:ImgInDDRAddress 校验完成后，可用镜像处于DDR中的位置
\retval ::0 Success CNcomment:0                  成功
\retval ::-1 Faliure CNcomment:-1                失败
\see \n
None CNcomment:无
*/
HI_S32 HI_CA_AdvcaVerifyByPartAddr(HI_U32 PartitionAddr,HI_U32 u32RamAddr, HI_CA_AuthInfo_S *pstAuthInfo,HI_U32 *pu32CodeAddr);



#endif /*__CA_VERIFY_API_H__*/

