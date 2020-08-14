/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : com_config.h
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

#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

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
/** \addtogroup      COM_CONFIG */
/** @{ */  /** <!-- [COM_CONFIG] */

#define COM_CFG_D_CNT_MODULE                    (64)

/** sometimes ,upgrade data can't be download for long long time
with some reason, we consider that it's timeout when any datagram
has been received COM_CFG_D_UPGRADE_TIMES times*/
#define COM_CFG_D_UPGRADE_TIMES                 (10)

#define COM_CFG_D_UPGRADE_FILENAME              "usb_update.bin"

#ifdef HI_LOADER_BOOTLOADER
#define COM_CFG_D_UPGRADE_FILEPATH              ""
#else
#define COM_CFG_D_UPGRADE_FILEPATH              "/tmp"
#endif

/** allowed Number of failure times, If the tims of upgrade failed more than the number, no longer try*/
#define COM_CFG_D_RETRY_TIMES                   (3)


/** When using Hipro tool, use 32M buffer to receive and write file */
#define COM_CFG_D_HIPRO_AVAILABLE_MEM           (0x2000000)

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     COM_CONFIG */
/** @{ */  /** <!-- [COM_DEBUG] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      COM_CONFIG */
/** @{ */  /** <!-- [COM_CONFIG] */

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__COM_CONFIG_H__*/
