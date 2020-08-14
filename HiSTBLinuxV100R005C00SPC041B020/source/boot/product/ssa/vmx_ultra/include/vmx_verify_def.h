/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : vmx_verify_def.h
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

#ifndef __VMX_VERIFY_DEF_H__
#define __VMX_VERIFY_DEF_H__

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
/** \addtogroup     VMX_VERIFY_DEF */
/** @{ */  /** <!-- [VMX_VERIFY_DEF] */

/** VMX Partition Structure Config */
#define SSA_VMX_SIG_OFFSET              (16)
#define SSA_VMX_SIG_LENGTH              (256)
#define SSA_VMX_IMG_OFFSET              (SSA_VMX_SIG_OFFSET + SSA_VMX_SIG_LENGTH)
#define SSA_VMX_VERIFY_MODE             (0)
#define SSA_VMX_ALIGN_BYTE              (0x0000000F)

#if defined(HI_ANDROID_BOOT_SUPPORT)
#define SSA_VERIFY_ADDR_KERNEL          (HI_U8*)(0x01FFFFC0 - SSA_VMX_IMG_OFFSET)
#define SSA_VERIFY_ADDR_LOADER          (HI_U8*)(0x01FFFFC0 - SSA_VMX_IMG_OFFSET)
#define SSA_VERIFY_ADDR_SHARE           (HI_U8*)(0x08000000)
#define SSA_VERIFY_ADDR_ROOTFS          (HI_U8*)(0x08000000)
#define SSA_VERIFY_ADDR_LOADERBK        (HI_U8*)(0x08000000)
#define SSA_VERIFY_ADDR_RESERVE         (HI_U8*)(0x0C000000)

#define SSA_VERIFY_SIZE_KERNEL          (0x02000000)
#define SSA_VERIFY_SIZE_LOADER          (0x02000000)
#define SSA_VERIFY_SIZE_SHARE           (0x04000000)
#define SSA_VERIFY_SIZE_ROOTFS          (0x04000000)
#define SSA_VERIFY_SIZE_RESERVE         (0x04000000)

#else
#define SSA_VERIFY_ADDR_KERNEL          (HI_U8*)(0x01FFFFC0 - SSA_VMX_IMG_OFFSET)
#define SSA_VERIFY_ADDR_LOADER          (HI_U8*)(0x01FFFFC0 - SSA_VMX_IMG_OFFSET)
#define SSA_VERIFY_ADDR_SHARE           (HI_U8*)(0x08000000)
#define SSA_VERIFY_ADDR_ROOTFS          (HI_U8*)(0x08000000)
#define SSA_VERIFY_ADDR_LOADERBK        (HI_U8*)(0x08000000)
#define SSA_VERIFY_ADDR_RESERVE         (HI_U8*)(0x0A000000)

#define SSA_VERIFY_SIZE_KERNEL          (0x02000000)
#define SSA_VERIFY_SIZE_LOADER          (0x02000000)
#define SSA_VERIFY_SIZE_SHARE           (0x02000000)
#define SSA_VERIFY_SIZE_ROOTFS          (0x02000000)
#define SSA_VERIFY_SIZE_LOADERBK        (0x02000000)
#define SSA_VERIFY_SIZE_RESERVE         (0x02000000)

#endif

/** Partition define */
#define SSA_PARTNAME_LOADER             "loader"
#define SSA_PARTNAME_LOADERBK           "loaderbak"
#define SSA_PARTNAME_KERNEL             "kernel"
#define SSA_PARTNAME_SWINFO             "softwareinfo"

#if defined(HI_ANDROID_BOOT_SUPPORT)
#define SSA_PARTNAME_ROOTFS             "shatable"
#else
#define SSA_PARTNAME_ROOTFS             "rootfs"
#endif

#define SSA_PARTSIZE_BOOTARGS           (0x00100000)

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     VMX_VERIFY_DEF */
/** @{ */  /** <!-- [VMX_VERIFY_DEF] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     VMX_VERIFY_DEF */
/** @{ */  /** <!-- [VMX_VERIFY_DEF] */

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif/*__VMX_VERIFY_DEF_H__*/
