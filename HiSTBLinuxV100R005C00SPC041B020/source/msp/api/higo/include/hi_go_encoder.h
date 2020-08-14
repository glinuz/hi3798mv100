/**
 \file
 \brief Describes the header file of the encoder module. CNcomment:encoder头文件 CNend
 \author Shenzhen Hisilicon Co., Ltd.
 \version 1.0
 \author 
 \date 2005-4-23
 */
 
#ifndef __HI_GO_ENCODER_H__
#define __HI_GO_ENCODER_H__

#include "hi_go_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/*************************** Structure Definition ****************************/
/** \addtogroup      HIGO_ENC */
/** @{ */  /** <!-- [HIGO_ENC] */

/**Encoder attributes*/
/** CNcomment:编码器属性*/
typedef struct 
{
   HIGO_IMGTYPE_E ExpectType;   /**<Type of the encoded picture*//**<CNcomment:编码图片类型*/
   HI_U32 QualityLevel;        /**<The quality level ranges from 1 to 99. The higher the level, the better the quality, and the greater the encoded picture or occupied memory. The QualityLevel parameter is valid for .jpeg pictures only.*//**<CNcomment:1-99级, 级别越高，质量越好，编码出来图像文件或内存也越大，只对JPEG有效*/ 	
}HIGO_ENC_ATTR_S;
/** @} */  /*! <!-- Structure Definition end */

/******************************* API declaration *****************************/
/** \addtogroup      HIGO_ENC */
/** @{ */  /** <!-- [HIGO_ENC] */
 /** 
\brief Initializes the encoder. CNcomment:编码器初始化 CNend
\attention \n
When ::HI_GO_Init is called, this application programming interface (API) is also called.
CNcomment: ::HI_GO_Init中已包含对该接口的调用 CNend
\param  N/A

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE
\retval ::HIGO_ERR_DEPEND_TDE

\see \n
::HI_GO_Init \n
::HI_GO_DeinitDecoder
*/

HI_S32 HI_GO_InitEncoder(HI_VOID);


/** 
\brief Deinitializes the encoder. CNcomment:编码器去初始化 CNend
\attention \n
When ::HI_GO_Deinit is called, this API is also called.
CNcomment: ::HI_GO_Deinit中已包含对该接口的调用 CNend
\param  N/A

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_Deinit \n
::HI_GO_InitDecoder
*/

HI_S32 HI_GO_DeinitEncoder(HI_VOID);

/**
\brief Encodes a surface in a specified format, and saves the encoded surface in a file. 
CNcomment:将一个surface编码到指定图像格式并保存到文件中 CNend
\attention \n
The .bmp encoding format is supported.
The hardware platform determines whether the .jpeg encoding foramt is supported.
CNcomment:支持编码成bmp格式文件 \n
(是否支持编码成JPEG,取决于硬件平台) CNend

\param[in] hSurface  Surface to be encoded. CNcomment:需要编码的surface, CNend
\param[in] pFile    Name of the encoded file. This parameter can be empty. If this parameter is not set, the encoded file is named [year]-[month]-[date]-[hour]-[minute]-[second].  
                        CNcomment:编码后的文件名，可以为空，为空则用当前时间[年]-[月]-[日]-[时]-[分]-[秒]来命名 CNend
\param[in] pAttr    Encoding attributes. This parameter cannot be empty. CNcomment:编码设置的属性，不可为空 CNend

\retval ::HI_SUCCESS Success.
\retval ::HI_FAILURE
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INVFILE
\retval ::HIGO_ERR_INVSRCTYPE
\retval ::HIGO_ERR_INVIMAGETYPE
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVMIRRORTYPE
\retval ::HIGO_ERR_INVROTATETYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_DEPEND_JPGE

\see \n
*/
HI_S32 HI_GO_EncodeToFile(HI_HANDLE hSurface, const HI_CHAR* pFile, const HIGO_ENC_ATTR_S* pAttr);

/**
\brief Encodes a surface in a specified picture format and saves it in a memory. Ensure that the memory is sufficient.
CNcomment:将一个surface编码到指定图像格式并保存到内存中，请自行保证指定内存块足够大 CNend
\attention \n
The data can be encoded as .bmp data, and saved in a specified memory.
The hardware platform determines whether the .jpeg encoding format is supported.
CNcomment:支持编码成bmp格式数据存放到指定的内存中 
(是否支持编码成JPEG,取决于硬件平台) CNend

\param[in] hSurface     Surface to be encoded. CNcomment:需要编码的surface, CNend
\param[in] pMem         Start address of the memory for storing the encoded pictures. CNcomment:保存编码后的图像数据内存块的起始地址 CNend
\param[in] MemLen       Size of a specified memory. CNcomment:指定内存的大小 CNend
\param[out] pOutLen     Actual size of the used memory, indicating the length of encoded data. Ensure that the value of pOutLen is smaller than or equal to the value of MemLen. 
                                CNcomment:实际使用的内存大小，编码后数据长度(自行确保pOutLen <= MemLen) CNend
\param[in] pAttr        Encoding attributes. This parameter cannot be empty. CNcomment:编码设置的属性，不可为空 CNend

\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INVFILE
\retval ::HIGO_ERR_INVSRCTYPE
\retval ::HIGO_ERR_INVFILE
\retval ::HIGO_ERR_INVIMAGETY
\retval ::HIGO_ERR_INVIMAGETYPE
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVMIRRORTYPE
\retval ::HIGO_ERR_INVROTATETYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\retval ::HIGO_ERR_DEPEND_JPGE

\see \n
*/

HI_S32 HI_GO_EncodeToMem(HI_HANDLE hSurface, HI_U8* pMem, HI_U32 MemLen, HI_U32* pOutLen, const HIGO_ENC_ATTR_S* pAttr);
  
/** @} */  /** <!-- ==== API declaration end ==== */


#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif /* __HI_TYPE_H__ */
