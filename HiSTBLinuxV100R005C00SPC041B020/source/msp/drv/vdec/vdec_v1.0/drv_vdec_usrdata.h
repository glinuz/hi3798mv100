/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_vdec_usrdata.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/31
  Description   :
  History       :
  1.Date        : 2013/01/31
    Author      : l00185424
    Modification: Created file

******************************************************************************/



#ifndef __DRV_VDEC_USRDATA_H__
#define __DRV_VDEC_USRDATA_H__

/******************************* Include Files *******************************/

/* add include here */
#include "hi_type.h"
#include "hi_unf_common.h"
#include "drv_vdec_ioctl.h"
#include "vfmw.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define VDEC_USERDATA_IDENTIFIER_DVB1   (0x34394147)
#define VDEC_USERDATA_IDENTIFIER_AFD    (0x31475444)

#define VDEC_USERDATA_TYPE_DVB1_CC      (0x03)
#define VDEC_USERDATA_TYPE_DVB1_BAR     (0x06)
#define VDEC_USERDATA_NEED_ARRANGE (1)
/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_S32 USRDATA_Init(HI_VOID);
HI_S32 USRDATA_DeInit(HI_VOID);
HI_S32 USRDATA_Alloc(HI_HANDLE hHandle, HI_DRV_VDEC_USERDATABUF_S* pstBuf);
HI_S32 USRDATA_SetUserAddr(HI_HANDLE hHandle, HI_U32 u32Addr);
HI_S32 USRDATA_Free(HI_HANDLE hHandle);
HI_S32 USRDATA_Start(HI_HANDLE hHandle);
HI_S32 USRDATA_Stop(HI_HANDLE hHandle);
HI_S32 USRDATA_Reset(HI_HANDLE hHandle);
HI_S32 USRDATA_Acq(HI_HANDLE hHandle, HI_UNF_VIDEO_USERDATA_S* pstUsrData, HI_UNF_VIDEO_USERDATA_TYPE_E* penType);
HI_S32 USRDATA_Rls(HI_HANDLE hHandle, HI_UNF_VIDEO_USERDATA_S* pstUsrData);
HI_S32 USRDATA_SetEosFlag(HI_HANDLE hHandle);
HI_S32 USRDATA_Arrange(HI_HANDLE hHandle, USRDAT* pstUsrData);
HI_S32 USRDATA_Put(HI_HANDLE hHandle, USRDAT* pstUsrData, HI_UNF_VIDEO_USERDATA_TYPE_E enType);
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __DRV_VDEC_USRDATA_H__ */

