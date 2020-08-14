
#ifndef __VFMW_ADAPTER_H__
#define __VFMW_ADAPTER_H__

#include "vfmw.h"

VOID   VFMW_OpenModule(VOID);
VOID   VFMW_ExitModule(VOID);
SINT32 VFMW_Init(VDEC_OPERATION_S *pArgs);
SINT32 VFMW_Exit(UINT8 IsSec);
SINT32 VFMW_Control(SINT32 ChanID, VDEC_CID_E eCmdID, VOID *pArgs);
SINT32 VFMW_Suspend(VOID);
SINT32 VFMW_Resume(VOID);
SINT32 VFMW_GetImage(SINT32 ChanID, IMAGE *pImage);
SINT32 VFMW_ReleaseImage(SINT32 ChanID, IMAGE *pImage);

#endif
