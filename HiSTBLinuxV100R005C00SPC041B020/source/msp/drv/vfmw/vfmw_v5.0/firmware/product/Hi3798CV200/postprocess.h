

/***********************************************************************
*
* Copyright (c) 2009 Hisilicon - All Rights Reserved
*
* File: $postprocess.h$
* Date: $2009/04/08$
* Revision: $v1.0$
* Purpose: VDEC Post Process.
*
* Change History:
*
* Date       Author            Change
* ======     ======            ======
* 20090408   q45134            Original
*
************************************************************************/

#ifndef __POST_PROCESS_H__
#define __POST_PROCESS_H__


#include "basedef.h"
#include "vdm_hal.h"

#define OPEN_REPAIR


typedef enum
{
    POSTPRO_TRUE  = 0,
    POSTPRO_FALSE = 1
} POSTPRO_RETVAL_E;

typedef enum
{
    RP_STRATAGE_NORMAL,
    RP_STRATAGE_DISABLE_REPAIR,
    RP_STRATAGE_BUTT
} RP_STRATAGE_E;

typedef struct
{
    SINT32    VdhId;
    VID_STD_E VidStd;
    VOID     *pDecParam;
} GetErrRatio_S;

#ifdef __cplusplus
extern "C" {
#endif

SINT32 PostPro_GetErrRatio(GetErrRatio_S *pParam);
RP_STRATAGE_E PostPro_GetRepairStratage(VID_STD_E VidStd, VOID *pDecParam);
#ifdef VFMW_MPEG4_SUPPORT
SINT32 PostPro_CopyYuv(MP4_DEC_PARAM_S *pMp4DecParam, SINT32 VdhId, VOID *pTask);
#endif
SINT32 PostPro_SetFullRepair( VID_STD_E VidStd, VOID *pDecParam, SINT32 VdhId );
#ifdef VFMW_VC1_SUPPORT  
SINT32 PostPro_CopyYuv_Vc1(VC1_DEC_PARAM_S *pVc1DecParam, SINT32 VdhId, VOID *pTask);
#endif
#ifdef VFMW_VP6_SUPPORT
SINT32 PostPro_CopyYuv_Vp6(VP6_DEC_PARAM_S *pVp6DecParam, SINT32 VdhId, VOID *pTask);
#endif
#ifdef __cplusplus
}
#endif


#endif //__POST_PROCESS_H__

