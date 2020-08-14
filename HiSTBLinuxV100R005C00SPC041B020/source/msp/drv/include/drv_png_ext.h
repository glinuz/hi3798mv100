#ifndef __DRV_PNG_EXT_H__
#define __DRV_PNG_EXT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_drv_dev.h"

typedef HI_S32  (*FN_PNG_Suspend)(PM_BASEDEV_S *, pm_message_t);
typedef HI_S32  (*FN_PNG_Resume)(PM_BASEDEV_S *);

typedef struct
{
	FN_PNG_Suspend			pfnPngSuspend;
	FN_PNG_Resume			pfnPngResume;
} PNG_EXPORT_FUNC_S;

void PNG_DRV_ModExit(void);
int PNG_DRV_ModInit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__DRV_PNG_EXT_H__*/
