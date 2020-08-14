/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ssu_parse.h
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

#ifndef __SSU_PARSE_H__
#define __SSU_PARSE_H__

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
/** \addtogroup      SSU_PARSE */
/** @{ */  /** <!-- [SSU_PARSE] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

#define SSU_D_DSMCC_PROTOCOL            (0x11)
#define SSU_D_DSMCC_TYPE                (0x03)

#define SSU_D_DSMCC_DSITABID            (0x3B)
#define SSU_D_DSMCC_DSIMSGID            (0x1006)

#define SSU_D_DSMCC_DIITABID            (0x3B)
#define SSU_D_DSMCC_DIIMSGID            (0x1002)

#define SSU_D_DSMCC_DDBTABID            (0x3C)
#define SSU_D_DSMCC_DDBMSGID            (0x1003)

#define SSU_D_DSMCC_MINHEADLEN          (12)
#define SSU_D_DSMCC_MAXMODULENUM        (256)
#define SSU_D_DSMCC_MAXSECTIONLEN       (4096)

#define SSU_D_MAXLEN_PARTNAME           (64)

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      SSU_PARSE */
/** @{ */  /** <!-- [SSU_PARSE] */

/****** Enumeration definition ************/

/****** Structure definition **************/

typedef struct tagSSU_COMPATIBILITY_DES_S
{
    HI_U8   u8DescriptorType;
    HI_U8   u8DescriptorLength;
    HI_U8   u8SpecifierType;
    HI_U32  u32SpecifierData;
    HI_U16  u16Model;
    HI_U16  u16Version;

} SSU_COMPATIBILITY_DES_S;


typedef struct tagSSU_DSMCC_MSGHEADER_S
{
    HI_U8   u8ProtocolDiscriminator;
    HI_U8   u8DsmccType;
    HI_U16  u16MessageId;
    HI_U32  u32TransactionId;
    HI_U8   u8Reserved;
    HI_U8   u8AdapationLength;
    HI_U16  u16MessageLength;
    HI_U8*  pu8MessageData;

} SSU_DSMCC_MSGHEADER_S;

typedef struct tagSSU_GII_S
{
    HI_U32  u32GroupID;
    HI_U32  u32GroupSize;

    HI_U32  u32ManufacturerID;              /**< Vendor ID */
    HI_U32  u32SoftwareVersion;             /**< Software version */
    HI_U32  u32HardwareVersion;             /**< Hardware version */
    HI_U32  u32SerialNumberStart;           /**< Start serial number*/
    HI_U32  u32SerialNumberEnd;             /**< End serial number*/

} SSU_GII_S;

typedef struct tagSSU_DSI_S
{
    SSU_DSMCC_MSGHEADER_S   stMsgHeader;
    HI_U16                  u16NumberOfGroups;
    SSU_GII_S*              pstGroups;

} SSU_DSI_S;


typedef struct tagSSU_DII_MODULE_S
{
    HI_U16                  u16ModuleID;
    HI_U32                  u32ModuleSize;
    HI_U8                   u8ModuleVersion;
    HI_U64                  u64FlashStartAddr;
    HI_U64                  u64FlashEndAddr;
    HI_U32                  u32FlashType;
    HI_U32                  u32FlashIndex;
    HI_U32                  u32PartVersion;        /**< Partition Version */
    HI_U32                  u32PartVerifyTag;      /**< Partition Verify Tag */
    HI_U8                   au8PartName[SSU_D_MAXLEN_PARTNAME]; /**< Partition Name */
    HI_U32                  u32CRC32;

} SSU_DII_MODULE_S;

typedef struct tagSSU_DII_S
{
    SSU_DSMCC_MSGHEADER_S   stMsgHeader;
    HI_U32                  u32DownloadId;
    HI_U16                  u16BlockSize;
    HI_U8                   u8WindowSize;
    HI_U8                   u8AckPeriod;
    HI_U32                  u32TCDownloadWindow;
    HI_U32                  u32TCDownloadscenario;
    HI_U16                  u16CompatibilityDescriptorLength;
    HI_U16                  u16NumberOfModules;
    SSU_DII_MODULE_S*       pstModules;

} SSU_DII_S;

typedef struct tagSSU_DDB_S
{
    SSU_DSMCC_MSGHEADER_S   stMsgHeader;
    HI_U16                  u16ModuleId;
    HI_U8                   u8ModuleVersion;
    HI_U8                   u8Reserved;
    HI_U16                  u16BlockNumber;
    HI_U16                  u16BlockSize;
    HI_U8*                  puBlockData;

} SSU_DDB_S;
/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      SSU_PARSE */
/** @{ */  /** <!-- [SSU_PARSE] */

HI_S32 SSU_ParseDSI(HI_U8* pu8DataBuf, HI_U16 u16DataLen, SSU_DSI_S* pstDSI);

HI_S32 SSU_FreeDSI(SSU_DSI_S* pstDSI);

HI_S32 SSU_ParseDII(HI_U8* pu8DataBuf, HI_U16 u16DataLen, SSU_DII_S* pstDII);

HI_S32 SSU_FreeDII(SSU_DII_S* pstDII);

HI_S32 SSU_ParseDDB(HI_U8* pu8DataBuf, HI_U16 u16DataLen, SSU_DDB_S* pstDDB);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__SSU_PARSE_H__*/
