/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hifile_parse.h
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

#ifndef __HIFILE_PARSE_H__
#define __HIFILE_PARSE_H__

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
/** \addtogroup      HIFILE_PARSE */
/** @{ */  /** <!-- [HIFILE_PARSE] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

#define HIFILE_D_MAXNUM_FLASHMAP                (64)
#define HIFILE_D_MAXNUM_MANUFACTURER            (3)
#define HIFILE_D_MAXLEN_PARTNAME                (64)

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      HIFILE_PARSE */
/** @{ */  /** <!-- [HIFILE_PARSE] */

/****** Enumeration definition ************/

/****** Structure definition **************/

typedef struct tagHIFILE_FLASHMAP_S
{
    HI_U32      u32ImageID;                 /**< Image identify */
    HI_U32      u32ImageLength;             /**< Image length */
    HI_U32      u32ImageCRC;                /**< Image CRC value */
    HI_U32      u32ImageOffset;             /**< Image Address offset */
    HI_U64      u64PartitionStartAddr;      /**< Address in the flash */
    HI_U64      u64PartitionEndAddr;        /**< The end address of flash partition */
    HI_U32      u32FlashType;               /**< Flash type */
    HI_U32      u32FlashIndex;              /**< Flash chip select */

    HI_U32      u32PartVersion;             /**< Partition Version */
    HI_U32      u32PartVerifyTag;           /**< Partition Verify Tag */
    HI_U8       au8PartName[HIFILE_D_MAXLEN_PARTNAME]; /**< Partition Name */

    HI_U8*      pu8ImageData;

    HI_U32      u32ImageVersion;
    HI_BOOL     bNeedUpgrade;

} HIFILE_FLASHMAP_S;

typedef struct tagHIFILE_MANUINFO_S
{
    HI_U32      u32ManufacturerID;          /**< Manufacturer ID*/
    HI_U32      u32HardwareVersion;         /**< Hardware version*/
    HI_U32      u32SoftwareVersion;         /**< Software version*/
    HI_U32      u32SerialNumberStart;       /**< Start serial number*/
    HI_U32      u32SerialNumberEnd;         /**< End serial number*/
    HI_U32      u32DownloadType;            /**< USB upgrade download mode */
    HI_U32      u32Reserved;                /**< Reserved for future expansion */
    HI_U32      u32ImageLength;             /**< Total image length */
    HI_U16      u16FlashMapNum;             /**< Total flashmap count */
    HIFILE_FLASHMAP_S astFlashMap[HIFILE_D_MAXNUM_FLASHMAP];   /**< flashmap information*/
} HIFILE_MANUINFO_S;


typedef struct tagHIFILE_DOCUMENT_S
{
    HI_U32      u32HeaderLen;               /**< Length of the file head */
    HI_U32      u32FileLen;                 /**< Length of file */
    HI_U16      u16ManuNum;                 /**< Count of Manufacturer */
    HIFILE_MANUINFO_S   astManuInfo[HIFILE_D_MAXNUM_MANUFACTURER];  /**< Manufacturer information */
} HIFILE_DOCUMENT_S;


/****** Union definition ******************/

/****** Global variable declaration *******/

/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      HIFILE_PARSE */
/** @{ */  /** <!-- [HIFILE_PARSE] */

HI_S32 HIFILE_ParseHeader
(
    HI_U8*              pu8FileData,
    HI_U32              u32FileSize,
    HIFILE_DOCUMENT_S*  pstDocument
);

HI_S32 HIFILE_ParseData
(
    HI_U8*              pu8FileData,
    HI_U32              u32FileSize,
    HIFILE_MANUINFO_S*  pstManuInfo
);

HI_S32 HIFILE_ParseWholeFile
(
    HI_U8*              pu8FileData,
    HI_U32              u32FileSize,
    HIFILE_DOCUMENT_S*  pstDocument
);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__HIFILE_PARSE_H__*/
