/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hiota_parse.h
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

#ifndef __HIOTA_PARSE_H__
#define __HIOTA_PARSE_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/
#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      HIOTA_PARSE */
/** @{ */  /** <!-- [HIOTA_PARSE] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

#define HIOTA_D_DOWNLOAD_TABID              (0xFE)
#define HIOTA_D_DOWNLOAD_TABEXTID           (0x0000)
#define HIOTA_D_DOWNLOAD_INFOTAG            (0xEA)
#define HIOTA_D_DOWNLOAD_PARTINFOTAG        (0xEB)
#define HIOTA_D_PARTITION_TABEXTID          (0x0000)
#define HIOTA_D_PARTITION_HEADTAG           (0xEC)

#define HIOTA_D_LEN_SECTION                 (4096)
#define HIOTA_D_LEN_DATAGRAM                (992)           /**< Size of each Datagram Section*/
#define HIOTA_D_LEN_STARTSTRING             (257)           /**< 256 + 1 */
#define HIOTA_D_LEN_DOWNLOADSTRING          (257)           /**< 256 + 1 */
#define HIOTA_D_LEN_HWSTRING                (257)           /**< 256 + 1 */
#define HIOTA_D_LEN_DLDESCRIPTION           (257)           /**< 256 + 1 */

#define HIOTA_D_MAXNUM_PARTITION            (64)
#define HIOTA_D_MAXLEN_PARTNAME             (64)

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      HIOTA_PARSE */
/** @{ */  /** <!-- [HIOTA_PARSE] */

/****** Enumeration definition ************/

typedef enum hiOTA_FLASHTYPE_E
{
    OTA_FLASHTYPE_SPI   = 0,
    OTA_FLASHTYPE_NAND  = 1,
    OTA_FLASHTYPE_EMMC  = 2,
    OTA_FLASHTYPE_BUTT
} OTA_FLASHTYPE_E;

typedef enum hiOTA_PARTITION_VERSION_E
{
    HIOTA_PARTITION_VERSION_APP         = 0x01,
    HIOTA_PARTITION_VERSION_KERNEL      = 0x02,
    HIOTA_PARTITION_VERSION_CA          = 0x04,
    HIOTA_PARTITION_VERSION_BOOTLOADER  = 0x08,
    HIOTA_PARTITION_VERSION_LOADER      = 0x10,
    HIOTA_PARTITION_VERSION_LOGO        = 0x20,
    HIOTA_PARTITION_VERSION_BUTT        = 0xff
} HIOTA_PARTITION_VERSION_E;
/****** Structure definition **************/

typedef struct hiOTA_DATAGRAM_INFO_S
{
    HI_U16  u16DatagramNum;
    HI_U8   u8PartitionNumber;
    HI_U32  u32SectionIndex;
    HI_U16  u16DataLength;
    HI_U8*  pu8Data;
} HIOTA_DATAGRAM_INFO_S;

typedef struct hiOTA_PARTITION_S
{
    /**< From Partition_Control */
    HI_U8   u8PartitionNumber;
    HI_U32  u8DownLoadType;         /**< Upgrade mode, partition, and file*/
    HI_U8   u8Reserved;             /**< Reserved for future use*/
    HI_U32  u16PartDatagramNumber;  /**< Number datagrams*/
    HI_U32  u32PartTotalSize;       /**< Size to upgrade*/
    HI_U32  u32PartOriSize;         /**< Original size*/
    HI_U32  u32PartOldVerStart;     /**< Original initial version*/
    HI_U32  u32PartOldVerEnd;       /**< Original end version*/
    HI_U32  u32PartNewVer;          /**< Version to upgrade*/
    HI_U64  u64FlashStartAddr;
    HI_CHAR acStartString[HIOTA_D_LEN_STARTSTRING];     /**< Name length pointer of the file to be upgraded*/
    HI_U32  u32FlashType;           /**< Flash type*/
    HI_U32  u32FlashIndex;          /**< Flash chip select*/
    HI_U64  u64FlashEndAddr;        /**< Address to upgrade*/
    HI_U32  u32PartVerifyTag;       /**< Partition Verify tag*/
    HI_U8   au8PartName[HIOTA_D_MAXLEN_PARTNAME]; /**< Partition Name */

    /**< From Download Control Info */
    HI_U8   u8DownloadMode;
    HI_U64  u64DownloadAddr;
    HI_CHAR acDownLoadString[HIOTA_D_LEN_DOWNLOADSTRING];  /**< Name length pointer of the file to be upgraded*/
    HI_U32  u32DownloadSize;       /**< Size to upgrade */
    HI_U32  u32DownloadCRC32;      /**< CRC to upgrade */

} HIOTA_PARTITION_S;

/** include all upgrade descriptor, download control struct not define again */
typedef struct hiOTA_UPGRADE_INFO_S
{
    HI_U32  u32ManufacturerID;                  /**<  Vendor ID */
    HI_U32  u32HardwareVersion;                 /**<  Hardware version */
    HI_U32  u32SoftwareVersion;                 /**< Software version- High level */
    HI_U8   u8DownloadTableId;                  /**< Upgrade package table_id*/
    HI_U8   u8KeyContorl;
    HI_U32  u32SerialNumberStart;               /**< Start serial number*/
    HI_U32  u32SerialNumberEnd;                 /**< End serial number*/
    HI_U32  u32DownloadDate;                    /**<  Download date and time */
    HI_U32  u32WebbrowserVersion;
    HI_U32  u32KernelVersion;
    HI_U32  u32CAVersion;
    HI_U32  u32BootLoaderVersion;
    HI_U32  u32LoaderVersion;                   /**< loader  version */
    HI_U32  u32LogoVersion;                     /**<  Startup picture version */
    HI_U32  u32APPVersion;                      /**< Application version  */
    HI_CHAR acHardwareString[HIOTA_D_LEN_HWSTRING];/**<  Upgrade description information*/
    HI_U32  u32DownloadDataTotalsize;           /**< Size of the entire upgrade data package*/

    HI_CHAR acDownloadDescription[HIOTA_D_LEN_DLDESCRIPTION];/**<  Upgrade description information*/
    HI_U8   u8ReservedTag;
    HI_U32  u32FullCRC32;                       /**<  CRC information to be updated*/
    HI_U32  u32MagicNum;                        /**< Magic number for packaging the upgrade stream */
    HI_U32  u32CRC32;
    HI_U8   u8PartitionCount;                   /**< Number of partitions to upgrade*/
    HIOTA_PARTITION_S astPartition[HIOTA_D_MAXNUM_PARTITION];                  /**< Partition information pointer*/
} HIOTA_UPGRADE_INFO_S;


/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      HIOTA_PARSE */
/** @{ */  /** <!-- [HIOTA_PARSE] */

#define HIOTA_GetPartitionIndex(u8PartitionNumber)  ((u8PartitionNumber) - 1)
#define HIOTA_GetPartitionNumber(u8PartitionIndex)  ((u8PartitionIndex) + 1)

HI_S32 HIOTA_ParseDatagramSection
(
    HI_U8*                  pu8SectionData,
    HI_U32                  u32SectionDataLen,
    HIOTA_DATAGRAM_INFO_S*  pstDatagramInfo
);

HI_S32 HIOTA_ParseDownloadControlSection
(
    HI_U8*                  pu8SectionData,
    HI_U32                  u32SectionDataLen,
    HIOTA_UPGRADE_INFO_S*   pstUpgradeInfo
);

HI_S32 HIOTA_ParsePartitionControlSection
(
    HI_U8*                  pu8SectionData,
    HI_U32                  u32SectionDataLen,
    HIOTA_PARTITION_S*      pstPartition
);

/** @} */  /** <!-- ==== API Declaration End ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__HIOTA_PARSE_H__*/
