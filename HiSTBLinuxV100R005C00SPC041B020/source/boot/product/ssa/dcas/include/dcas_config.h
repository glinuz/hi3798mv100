/******************************************************************************

          Copyright (C), 2001-2015, Hisilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_config.h
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2016-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2016-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

#ifndef __DCAS_CONFIG_H__
#define __DCAS_CONFIG_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup     DCAS_CONFIG */
/** @{ */  /** <!-- [DCAS_CONFIG] */

#if defined(HI_ANDROID_BOOT_SUPPORT)
#define SSA_MEM_SIZE_BOOTM                  (0x02000000)
#define SSA_MEM_SIZE_BOOTM_SIGN             (0x00100000)
#define SSA_MEM_SIZE_BOOTM_DATA             (SSA_MEM_SIZE_BOOTM - SSA_MEM_SIZE_BOOTM_SIGN)

#define SSA_MEM_SIZE_VERIFY                 (0x04000000)
#define SSA_MEM_SIZE_VERIFY_SIGN            (0x00100000)
#define SSA_MEM_SIZE_VERIFY_DATA            (SSA_MEM_SIZE_VERIFY - SSA_MEM_SIZE_VERIFY_SIGN)

#define SSA_MEM_ADDR_BOOTM                  (HI_U8*)(0x01FFFFC0)
#define SSA_MEM_ADDR_BOOTM_SIGN             (SSA_MEM_ADDR_BOOTM + SSA_MEM_SIZE_BOOTM_DATA)

#define SSA_MEM_ADDR_VERIFY1                (HI_U8*)(0x08000000)
#define SSA_MEM_ADDR_VERIFY1_SIGN           (SSA_MEM_ADDR_VERIFY1 + SSA_MEM_SIZE_VERIFY_DATA)

#define SSA_MEM_ADDR_VERIFY2                (HI_U8*)(0x0C000000)
#define SSA_MEM_ADDR_VERIFY2_SIGN           (SSA_MEM_ADDR_VERIFY2 + SSA_MEM_SIZE_VERIFY_DATA)

#else

#define SSA_MEM_SIZE_BOOTM                  (0x02000000)
#define SSA_MEM_SIZE_BOOTM_SIGN             (0x00100000)
#define SSA_MEM_SIZE_BOOTM_DATA             (SSA_MEM_SIZE_BOOTM - SSA_MEM_SIZE_BOOTM_SIGN)

#define SSA_MEM_SIZE_VERIFY                 (0x02000000)
#define SSA_MEM_SIZE_VERIFY_SIGN            (0x00100000)
#define SSA_MEM_SIZE_VERIFY_DATA            (SSA_MEM_SIZE_VERIFY - SSA_MEM_SIZE_VERIFY_SIGN)

#define SSA_MEM_ADDR_BOOTM                  (HI_U8*)(0x01FFFFC0)
#define SSA_MEM_ADDR_BOOTM_SIGN             (SSA_MEM_ADDR_BOOTM + SSA_MEM_SIZE_BOOTM_DATA)

#define SSA_MEM_ADDR_VERIFY1                (HI_U8*)(0x08000000)
#define SSA_MEM_ADDR_VERIFY1_SIGN           (SSA_MEM_ADDR_VERIFY1 + SSA_MEM_SIZE_VERIFY_DATA)

#define SSA_MEM_ADDR_VERIFY2                (HI_U8*)(0x0A000000)
#define SSA_MEM_ADDR_VERIFY2_SIGN           (SSA_MEM_ADDR_VERIFY2 + SSA_MEM_SIZE_VERIFY_DATA)

#endif

#define SSA_PARTSIZE_BOOT                   (0x00100000)
#define SSA_PARTSIZE_BOOTARGS               (0x00100000)

/** Partition define */
#define SSA_PARTNAME_BOOT                   "boot"
#define SSA_PARTNAME_BOOTARGS               "bootargs"
#define SSA_PARTNAME_LOGO                   "logo"
#define SSA_PARTNAME_LOADER                 "loader"
#define SSA_PARTNAME_LOADERBK               "loaderbak"
#define SSA_PARTNAME_KERNEL                 "kernel"
#define SSA_PARTNAME_SWINFO                 "softwareinfo"

#if defined(HI_ANDROID_BOOT_SUPPORT)
#define SSA_PARTNAME_ROOTFS                 "shatable"
#else
#define SSA_PARTNAME_ROOTFS                 "rootfs"
#endif

#define SSA_DCAS_FIX_PAGESIZE               (0x2000)
#define SSA_DCAS_LEN_VERSION                (8)
#define SSA_DCAS_LEN_SHA_VALUE              (32)
#define SSA_DCAS_LEN_SIGNATURE              (256)
#define SSA_DCAS_LEN_ORIGNAL_IMAGE_PATH     (256)
#define SSA_DCAS_LEN_RSA_PRIVATE_KEY_PATH   (256)
#define SSA_DCAS_LEN_MAGICNUMBER            (32)
#define SSA_DCAS_LEN_RSAPUBLICKEY           (512)
#define SSA_DCAS_STR_MAGICNUMBER            ("Hisilicon_ADVCA_ImgHead_MagicNum")
#define SSA_DCAS_SAMPLING_STEPSIZE          (10)

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     DCAS_CONFIG */
/** @{ */  /** <!-- [DCAS_CONFIG] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     DCAS_CONFIG */
/** @{ */  /** <!-- [DCAS_CONFIG] */

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif/*__DCAS_CONFIG_H__*/
