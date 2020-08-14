/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : com_parse.h
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

#ifndef __COM_PARSE_H__
#define __COM_PARSE_H__

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
/** \addtogroup      COM_PARSE */
/** @{ */  /** <!-- [COM_PARSE] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

/****** Macro Functions definition ********/

#define HI_READ_64BIT(pcByte, Result, Length)   \
    {                                           \
        (Result) = (HI_U64)((pcByte)[0]) << 56  \
                 | (HI_U64)((pcByte)[1]) << 48  \
                 | (HI_U64)((pcByte)[2]) << 40  \
                 | (HI_U64)((pcByte)[3]) << 32  \
                 | (HI_U64)((pcByte)[4]) << 24  \
                 | (HI_U64)((pcByte)[5]) << 16  \
                 | (HI_U64)((pcByte)[6]) << 8   \
                 | (HI_U64)((pcByte)[7]);       \
        (pcByte) += 8;                          \
        (Length) += 8;                          \
    }

#define HI_READ_32BIT(pcByte, Result, Length)   \
    {                                           \
        (Result) = (HI_U32)((pcByte)[0] << 24)  \
                 | (HI_U32)((pcByte)[1] << 16)  \
                 | (HI_U32)((pcByte)[2] << 8)   \
                 | (HI_U32)((pcByte)[3]);       \
        (pcByte) += 4;                          \
        (Length) += 4;                          \
    }

#define HI_READ_24BIT(pcByte, Result, Length)   \
    {                                           \
        (Result) = (HI_U32)((pcByte)[0] << 16)  \
                 | (HI_U32)((pcByte)[1] << 8)   \
                 | (HI_U32)((pcByte)[2]);       \
        (pcByte) += 3;                          \
        (Length) += 3;                          \
    }

#define HI_READ_16BIT(pcByte, Result, Length)   \
    {                                           \
        (Result) = (HI_U16)((pcByte)[0] << 8)   \
                 | (HI_U16)((pcByte)[1]);       \
        (pcByte) += 2;                          \
        (Length) += 2;                          \
    }

#define HI_READ_12BIT(pByte, Result, Length)    \
    {                                           \
        (Result) = ((HI_U16)((pByte)[0] << 8)  \
                 | (HI_U16)((pByte)[1]))       \
                 & (0x0FFF);                    \
        (pByte)  += 2;                          \
        (Length) += 2;                          \
    }

#define HI_READ_08BIT(pcByte, Result, Length)   \
    {                                           \
        (Result) = (HI_U8)((pcByte)[0]);        \
        (pcByte) += 1;                          \
        (Length) += 1;                          \
    }

#define HI_SKIP_BYTES(pcByte, ByteCount, Length)                        \
    {                                                                   \
        (pcByte) = (pcByte) + (ByteCount);                              \
        (Length) += (ByteCount);                                        \
    }

#define HI_READ_BYTES(pcByte, ByteCount, Buffer, BufferSize, Length)    \
    {                                                                   \
        if (0 < (ByteCount))                                            \
        {                                                               \
            HI_U32 u32Size = (BufferSize);                              \
                                                                        \
            if (u32Size > (ByteCount))                                  \
            {                                                           \
                u32Size = (ByteCount);                                  \
            }                                                           \
                                                                        \
            COM_MemCpy((Buffer), (pcByte), u32Size);                    \
            (pcByte) += (ByteCount);                                    \
            (Length) += (ByteCount);                                    \
        }                                                               \
    }

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      COM_PARSE */
/** @{ */  /** <!-- [COM_PARSE] */

/****** Enumeration definition ************/

/****** Structure definition **************/

typedef struct tagCOM_SECTION_S
{
    HI_U8   u8TableID;
    HI_U16  u16SectionLength;
    HI_U16  u16TableIDExtension;
    HI_U8   u8SoftwareVersion;
    HI_U8   u8SectionNumber;
    HI_U8   u8LastSectionNumber;
    HI_U8*  pu8PrivateData;
    HI_U32  u32PrivateDataLen;
    HI_U32  u32CRC32;

} COM_SECTION_S;

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      COM_PARSE */
/** @{ */  /** <!-- [COM_PARSE] */

HI_U32 COM_CRC32(HI_U32 u32Crc, const HI_U8* pu8Data, HI_U32 u32Len);

HI_S32 COM_ParseSection
(
    HI_U8*          pu8SectionData,
    HI_U32          u32SectionDataLen,
    COM_SECTION_S*  pstSection
);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /*__COM_PARSE_H__*/
