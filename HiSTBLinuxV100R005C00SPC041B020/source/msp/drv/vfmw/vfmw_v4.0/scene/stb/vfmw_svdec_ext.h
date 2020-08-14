
/* SVDEC */
#include "syn_cmn.h"
#include "mem_manage.h"
#include "syntax.h"

/* SVDEC USE FUNCTION */
typedef HI_VOID  (*FN_VFMW_KlibFlushCache)(HI_VOID *, HI_U32, HI_U32);
typedef HI_VOID  (*FN_VFMW_KernelFlushCache)(HI_VOID *, HI_U32, HI_U32);
typedef HI_S32   (*FN_VFMW_MemMapRegisterAddr)(UADDR, HI_U32, MEM_RECORD_S *);
typedef HI_S32   (*FN_VFMW_InsertImgToVoQueue)(HI_S32, VID_STD_E, HI_VOID *, IMAGE_VO_QUEUE *, IMAGE *);
#ifndef HI_ADVCA_FUNCTION_RELEASE
typedef SINT32 (*FN_VFMW_Dprint)(UINT32, const SINT8 *, ...);
#else
typedef VOID (*FN_VFMW_Dprint)(VOID);
#endif
typedef HI_S32   (*FN_VFMW_VctrlGetChanIDByCtx)(HI_VOID *);
typedef UADDR   (*FN_VFMW_MemVir2Phy)(HI_U8 *);
typedef HI_VOID *(*FN_VFMW_MemPhy2Vir)(UADDR);
typedef HI_VOID  (*FN_VFMW_ResetVoQueue)(IMAGE_VO_QUEUE *);
typedef HI_S32   (*FN_VFMW_RegisterSoftDecoder)(iMediaSDEC_FUNC_S *);
typedef HI_VOID  (*FN_VFMW_UnRegisterSoftDecoder)(HI_VOID);
typedef HI_VOID  (*FN_VFMW_LastFrameNotify)(HI_S32, HI_U32);

/* SVDEC USE FUNCTION */
typedef struct
{
    FN_VFMW_KlibFlushCache          pfnVfmwKlibFlushCache;
    FN_VFMW_KernelFlushCache        pfnVfmwKernelFlushCache;
    FN_VFMW_MemMapRegisterAddr      pfnVfmwMemMapRegisterAddr;
    FN_VFMW_InsertImgToVoQueue      pfnVfmwInsertImgToVoQueue;
    FN_VFMW_Dprint                  pfnVfmwDprint;
    FN_VFMW_VctrlGetChanIDByCtx     pfnVfmwVctrlGetChanIDByCtx;
    FN_VFMW_MemVir2Phy              pfnVfmwMemVir2Phy;
    FN_VFMW_MemPhy2Vir              pfnVfmwMemPhy2Vir;
    FN_VFMW_ResetVoQueue            pfnVfmwResetVoQueue;
    FN_VFMW_RegisterSoftDecoder     pfnVfmwRegisterSoftDecoder;
    FN_VFMW_UnRegisterSoftDecoder   pfnVfmwUnRegisterSoftDecoder;
    FN_VFMW_LastFrameNotify         pfnVfmwLastFrameNotify;
}   VFMW_SVDEC_EXPORT_FUNC_S;



