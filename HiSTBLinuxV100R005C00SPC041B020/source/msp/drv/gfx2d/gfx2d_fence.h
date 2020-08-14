#ifdef GFX2D_FENCE_SUPPORT

#ifndef _GFX2D_FENCE_H_

#define _GFX2D_FENCE_H_

#include "hi_type.h"

HI_VOID GFX2D_FENCE_Open(HI_VOID);
HI_VOID GFX2D_FENCE_Close(HI_VOID);
HI_S32 GFX2D_FENCE_Create(const HI_CHAR *name);
HI_VOID GFX2D_FENCE_Destroy(HI_S32 fd);
HI_S32 GFX2D_FENCE_Wait(HI_S32 fd);
HI_S32 GFX2D_FENCE_WakeUp(HI_VOID);
HI_VOID GFX2D_FENCE_ReadProc(struct seq_file *p, HI_VOID *v);
#endif

#endif
