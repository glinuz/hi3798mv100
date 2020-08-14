
#ifndef _SYNC_EXT_H_
#define _SYNC_EXT_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_drv_sync.h"

typedef HI_VOID (*FN_SYNC_VidProc)(HI_HANDLE, SYNC_VID_INFO_S *, SYNC_VID_OPT_S *);
typedef HI_VOID (*FN_SYNC_PcrProc)(HI_HANDLE, HI_U32);

typedef struct
{
    FN_SYNC_VidProc     pfnSYNC_VidProc;
    FN_SYNC_PcrProc     pfnSYNC_PcrProc;
} SYNC_EXPORT_FUNC_S;

HI_S32  SYNC_DRV_ModInit(HI_VOID);
HI_VOID SYNC_DRV_ModExit(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

