/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cx_verify_api.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2016-01-15
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __CX_VERIFY_API_H__
#define __CX_VERIFY_API_H__

#include "hi_type.h"
#include "cx_verify_def.h"
#include "cx_adp.h"


/**
\brief  verify flash contents by partion name
CNcomment:根据flash分区名校验flash内容
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
HI_S32 HI_CX_VerifyFlashPartitionByName(HI_CHAR* pPartionName, HI_U32 u32RamAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_U32 *pu32CodeAddr);


/**
\brief  Verify flash contents by address of flash partion
CNcomment:根据flash分区地址校验flash内容
\attention \n
如果是加密存储在flash的镜像，会被先解密，然后校验签名；如果是明文镜像，直接校验签名。
\param[in] PartionAddr The address of flash partion       CNcomment:PartionAddr  flash 分区地址
\param[in] u32RamAddr   RAM address for image             CNcomment:pPartionName  存放image的内存地址
\param[in] pstAuthInfo  Authentication configure infor    CNcomment:pstAuthInfo  校验配置信息
\param[out] pu32CodeAddr After Authenticate finished ,image address in DDR CNcomment:ImgInDDRAddress 校验完成后，可用镜像处于DDR中的位置
\retval ::0 Success CNcomment:0                  成功
\retval ::-1 Faliure CNcomment:-1                失败
\see \n
None CNcomment:无
*/
HI_S32 HI_CX_VerifyFlashPartitionByAddr(HI_U32 PartionAddr,HI_U32 u32RamAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_U32 *pu32CodeAddr);



/**
\brief  Get image header structure by flash partition name
CNcomment:通过flash分区的名字获取image头部信息
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
HI_S32 HI_CX_GetFlashImgInfoByName(HI_CHAR* pPartionName, HI_CA_AuthInfo_S *pstAuthInfo, HI_CAImgHead_S* pstImgInfo, HI_BOOL *pIsEncrypt);

/**
\brief  Get image header structure by flash partition name
CNcomment:通过flash分区的名字获取image头部信息
\attention \n
\param[in] PartionAddr The address of flash partion       CNcomment:PartionAddr  flash 分区地址
\param[in] pstAuthInfo  Authentication configure infor    CNcomment:pstAuthInfo  校验配置信息
\param[out] pstImgInfo  Image header info                 CNcomment:pstAuthInfo  镜像头部信息
\param[out] pIsEncrypt  Is the image encrypted            CNcomment:ImgInDDRAddress 镜像是否加密
\retval ::0 Success CNcomment:0                  成功
\retval ::-1 Faliure CNcomment:-1                失败
\see \n
None CNcomment:无
*/
HI_S32 HI_CX_GetFlashImgInfoByAddr(HI_U32 PartionAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_CAImgHead_S* pstImgInfo, HI_BOOL *pIsEncrypt);


/*
Verify initialization
*/
HI_S32 HI_CX_VerifyInit(HI_VOID);


#endif /*__CX_VERIFY_API_H__*/

