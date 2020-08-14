/**
 \file
 \brief pts recovery
 \copyright Shenzhen Hisilicon Co., Ltd.
 \version draft
 \author x57522
 \date 2011-4-21
 */

#ifndef __PTS_RECV_H__
#define __PTS_RECV_H__

/******************************* Include Files *******************************/

/* add include here */
#include "hi_type.h"
#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "vfmw.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_S32 PTSREC_Init(HI_VOID);
HI_S32 PTSREC_DeInit(HI_VOID);
HI_S32 PTSREC_Alloc(HI_HANDLE hHandle);
HI_S32 PTSREC_Free(HI_HANDLE hHandle);
HI_S32 PTSREC_Start(HI_HANDLE hHandle);
HI_S32 PTSREC_Stop(HI_HANDLE hHandle);
HI_S32 PTSREC_Reset(HI_HANDLE hHandle);
HI_S32 PTSREC_SetFrmRate(HI_HANDLE hHandle, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate);
HI_S32 PTSREC_GetFrmRate(HI_HANDLE hHandle, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate);
HI_U32 PTSREC_CalcStamp(HI_HANDLE hHandle, HI_UNF_VCODEC_TYPE_E enVdecType, IMAGE *pstImage);
HI_U32 PTSREC_GetInterPtsDelta(HI_HANDLE hHandle);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __PTS_RECV_H__ */
