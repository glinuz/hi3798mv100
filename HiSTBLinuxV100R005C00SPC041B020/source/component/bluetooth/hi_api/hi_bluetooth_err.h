/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_bluetooth_err.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/04/21
  Description   : error code of BLUETOOTH
  History       :
  1.Date        : 2011/04/21
    Author      : f00172091
    Modification: Created file

******************************************************************************/
/** 
\file
\brief provide the common error code for BLUETOOTH module
*/
/* add include here */
#ifndef __HI_BLUETOOTH_ERRORCODE_H__
#define __HI_BLUETOOTH_ERRORCODE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */

/***************************** Macro Definition ******************************/
/** \addtogroup      H_3_1 */
/** @{ */  /** <!--  [BLUETOOTH ERROR CODE] */


#define HI_ERR_BLUETOOTH_NULL_PTR                  (HI_S32)(0x80510001)
#define HI_ERR_BLUETOOTH_INVALID_PARA              (HI_S32)(0x80510002)
#define HI_ERR_BLUETOOTH_MALLOC_MEM_FAILURE        (HI_S32)(0x80510003)
#define HI_ERR_BLUETOOTH_ADP_FAILURE               (HI_S32)(0x80510004)
#define HI_ERR_BLUETOOTH_SCAN_FAILURE              (HI_S32)(0x80510005)
#define HI_ERR_BLUETOOTH_PAIR_FAILURE              (HI_S32)(0x80510006)
#define HI_ERR_BLUETOOTH_GET_PAIRED_DEV_FAILURE    (HI_S32)(0x80510007)
#define HI_ERR_BLUETOOTH_REMOVE_PAIRED_DEV_FAILURE (HI_S32)(0x80510008)
#define HI_ERR_BLUETOOTH_GET_DEV_STATUS_FAILURE    (HI_S32)(0x80510009)
#define HI_ERR_BLUETOOTH_CONNECT_DEV_FAILURE       (HI_S32)(0x8051000a)
#define HI_ERR_BLUETOOTH_OPEN_FILE_FAILURE         (HI_S32)(0x8051000b)
#define HI_ERR_BLUETOOTH_PTHREAD_BUSY              (HI_S32)(0x8051000c)
#define HI_ERR_BLUETOOTH_NOT_CONNECTED             (HI_S32)(0x8051000d)
#define HI_ERR_BLUETOOTH_HEADSET_RING_FAILURE      (HI_S32)(0x8051000e)

/** @} */  /*! <!-- Macro Definition end */



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __HI_BLUETOOTH_ERRORCODE_H__ */

