#ifndef  __DRV_VENC_OSAL_EXT_H__
#define  __DRV_VENC_OSAL_EXT_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef HI_VOID*  (*FN_OSAL_MemSet)(HI_VOID* s, HI_S32 c, unsigned long n);
typedef HI_S32 (*FN_OSAL_printk)(const char* fmt, ...);

typedef struct Venc_Osal_Func_Ptr_S
{
    FN_OSAL_MemSet pfun_Osal_MemSet;
    FN_OSAL_printk pfun_Osal_printk;
} Venc_Osal_Func_Ptr;

#endif //__DRV_VENC_OSAL_EXT_H__

