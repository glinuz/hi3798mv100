/******************************************************************************
 Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_unf_version.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/23
Last Modified :
Description   : this file defines the UNF version code.
Function List :
History       :
******************************************************************************/
#ifndef __HI_UNF_VERSION_H__
#define __HI_UNF_VERSION_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*--------------------------------------------------------------------------------------------------------------*
 * Defintion of UNF version code.                                                                               *
 * CNcomment: 定义UNF接口的版本号 CNend                                                                         *
 *--------------------------------------------------------------------------------------------------------------*/
/********************************Struct Definition********************************/
/** \addtogroup      UNF_VERSION */
/** @{ */  /** <!-- [UNF_VERSION] */


#ifndef UNF_VERSION
#define UNF_VERSION(major, minor)   (((major) << 8) | (minor))
#endif

#ifndef UNF_VERSION_CODE
#define UNF_VERSION_CODE        UNF_VERSION(3, 2)
#define UNF_SUBVERSION_CODE     17
#define UNF_PRODUCT_CODE        1
#endif


/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_VERSION_H__ */

