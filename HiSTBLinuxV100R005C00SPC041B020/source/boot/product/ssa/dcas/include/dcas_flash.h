/******************************************************************************

          Copyright (C), 2001-2015, Hisilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_flash.h
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

#ifndef __DCAS_FLASH_H__
#define __DCAS_FLASH_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"
#include "hi_flash.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup     DCAS_FLASH */
/** @{ */  /** <!-- [DCAS_FLASH] */

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     DCAS_FLASH */
/** @{ */  /** <!-- [DCAS_FLASH] */

/****** Enumeration definition ************/

/****** Structure definition **************/

typedef struct tagSSA_FLASH_OPENPARAM_S
{
    HI_CHAR*        pcPartitionName;
    HI_FLASH_TYPE_E enFlashType;
    HI_U64          u64Address;
    HI_U32          u32Length;

} SSA_FLASH_OPENPARAM_S;

/****** Union definition ******************/

/****** Global variable declaration *******/

/*nand and SPI max flash block size*/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     DCAS_FLASH */
/** @{ */  /** <!-- [DCAS_FLASH] */

HI_HANDLE SSA_FLASH_Open(SSA_FLASH_OPENPARAM_S* pstOpenParam);

HI_S32 SSA_FLASH_Close(HI_HANDLE hHandle);

HI_U32 SSA_FLASH_Read(HI_HANDLE hHandle, HI_U64 u64Address, HI_U8* pu8Buffer, HI_U32 u32BufferSize, HI_U32 u32ReadSize);

HI_S32 SSA_FLASH_Write(HI_HANDLE hHandle, HI_U64 u64Address, HI_U8* pu8Data, HI_U32 u32Size);

HI_S32 SSA_FLASH_GetPartitionSize(HI_HANDLE hHandle, HI_U32* pu32Size);


/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__DCAS_FLASH_H__*/

