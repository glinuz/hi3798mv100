/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : com_buffer.h
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

#ifndef __COM_BUFFER_H__
#define __COM_BUFFER_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/
#include "com_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      COM_BUFFER */
/** @{ */  /** <!-- [COM_BUFFER] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      COM_BUFFER */
/** @{ */  /** <!-- [COM_BUFFER] */

/****** Enumeration definition ************/

typedef enum tagDOWNLOAD_STATUS_E
{
    COM_BUFFER_E_STATUS_NORMAL = 0,
    COM_BUFFER_E_STATUS_NOMEM,
    COM_BUFFER_E_STATUS_COMPLETE,
    COM_BUFFER_STATUS_BUTT
} COM_BUFFER_STATUS_E;

/****** Structure definition **************/
typedef struct tagCOM_BUFFER_MODULEINFO_S
{
    HI_U32  u32ModuleID;
    HI_U32  u32ModuleSize;

} COM_BUFFER_MODULEINFO_S;

typedef struct tagCOM_BUFFER_INITPARAM_S
{
    HI_U8*                  pu8MemBuff;
    HI_U32                  u32MemSize;
    HI_U32                  u32BlockSize;
    HI_U32                  u32ModuleNum;
    COM_BUFFER_MODULEINFO_S astModuleBuf[COM_CFG_D_CNT_MODULE];

} COM_BUFFER_INITPARAM_S;

typedef struct tagCOM_BUFFER_DATABLOCK_S
{
    HI_U32  u32ModuleID;            /**< Partition id*/
    HI_U8*  pu8DataBuff;            /**< Data buffer */
    HI_U32  u32DataSize;            /**< Size to data */
    HI_BOOL bLastData;              /**< Whether it is the last paragraph of data */
} COM_BUFFER_DATABLOCK_S;

/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      COM_BUFFER */
/** @{ */  /** <!-- [COM_BUFFER] */

HI_S32 COM_BUFFER_Init(COM_BUFFER_INITPARAM_S* pstInitParam);

HI_S32 COM_BUFFER_DeInit(HI_VOID);

HI_S32 COM_BUFFER_PutData
(
    HI_U32          u32ModuleIndex,
    HI_U32          u32BlockIndex,
    const HI_U8*    pu8BlockData,
    HI_U32          u32BlockDataLen
);

HI_S32 COM_BUFFER_GetData(COM_BUFFER_DATABLOCK_S* pstData, HI_U32 u32DataLen, HI_U32* pu32DataLen);

HI_U32 COM_BUFFER_GetSize(HI_VOID);

HI_U32 COM_BUFFER_GetBlockSize(HI_VOID);

HI_S32 COM_BUFFER_GetStatus(COM_BUFFER_STATUS_E* penStatus);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__COM_BUFFER_H__*/
