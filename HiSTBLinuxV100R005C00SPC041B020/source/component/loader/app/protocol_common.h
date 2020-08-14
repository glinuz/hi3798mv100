/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : protocol_common.h
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

#ifndef __PROTOCOL_COMMON_H__
#define __PROTOCOL_COMMON_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "com_buffer.h"
#include "loader_dbinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      PROTOCOL_COMMON */
/** @{ */  /** <!-- [PROTOCOL_COMMON] */

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      PROTOCOL_COMMON */
/** @{ */  /** <!-- [PROTOCOL_COMMON] */

/****** Enumeration definition ************/

/****** Structure definition **************/

typedef struct tagPROT_PARTITION_INFO_S
{
    HI_U32  u32PartitionId;         /**< Partition id */
    HI_U32  u32FlashType;           /**< Flash type */
    HI_U32  u32FlashIndex;          /**< Image file system */
    HI_U64  u64FlashStartAddr;      /**< Partition start Address in Flash */
    HI_U64  u64FlashEndAddr;        /**< Partition end Address in Flash */
    HI_U32  u32ImageSize;           /**< Image Size */

} PROT_PARTITION_INFO_S;

typedef struct tagPROT_VERSION_INFO_S
{
    HI_U32  u32ManufacturerID;      /**< Manufacturer ID */
    HI_U32  u32SoftwareVersion;     /**< Software version- High level */
    HI_U32  u32HardwareVersion;     /**< Hardware version */
    HI_U32  u32SerialNumberStart;   /**< Start serial number*/
    HI_U32  u32SerialNumberEnd;     /**< End serial number */

} PROT_VERSION_INFO_S;

typedef struct tagPROT_DEVICE_INFO_S
{
    HI_U32  u32ManufacturerID;      /**< Manufacturer ID */
    HI_U32  u32SoftwareVersion;     /**< Software version */
    HI_U32  u32HardwareVersion;     /**< Hardware version */
    HI_U32  u32SerialNumber;        /**< Device Serial number */

} PROT_DEVICE_INFO_S;

typedef struct tagPROT_UPGRADE_INFO_S
{
    HI_U32  u32ManufacturerID;      /**< Manufacturer ID */
    HI_U32  u32SoftwareVersion;     /**< Software version */
    HI_U32  u32HardwareVersion;     /**< Hardware version */
    HI_U32  u32SerialNumberStart;   /**< Start serial number*/
    HI_U32  u32SerialNumberEnd;     /**< End serial number */

    HI_U32  u32ModuleNum;
    PROT_PARTITION_INFO_S astModules[COM_CFG_D_CNT_MODULE];

} PROT_UPGRADE_INFO_S;

typedef HI_S32 (*PROT_DATA_CALLBACK)(COM_BUFFER_DATABLOCK_S* pstData, HI_U32 u32Number);

typedef HI_S32 (*PROT_PROCESS_CALLBACK)(HI_U32 u32DownloadSize, HI_U32 u32TotalSize);

typedef HI_S32 (*PROT_FN_INIT)
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
);

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      PROTOCOL_COMMON */
/** @{ */  /** <!-- [PROTOCOL_COMMON] */

typedef HI_S32 (*PROT_FN_DEINIT)(HI_VOID);

typedef HI_S32 (*PROT_FN_GETVERSIONINFO)(PROT_VERSION_INFO_S* pstVersionInfo);

typedef HI_S32 (*PROT_FN_GETPARTITIONINFO)(PROT_PARTITION_INFO_S* pstPartBuff, HI_U32 u32PartBufSize, HI_U32* pu32PartCount);

typedef HI_S32 (*PROT_FN_PROCESS)(HI_VOID);


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __PROTOCOL_COMMON_H__ */
