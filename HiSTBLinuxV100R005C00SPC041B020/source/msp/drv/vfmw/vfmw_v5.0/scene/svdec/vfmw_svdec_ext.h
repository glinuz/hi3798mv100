
/* SVDEC */
#include "syn_cmn.h"
#include "mem_manage.h"
#include "syntax.h"

/* SVDEC USE FUNCTION */
typedef VOID   (*FN_VFMW_KlibFlushCache)(VOID *, UINT32, UINT32);
typedef VOID   (*FN_VFMW_KernelFlushCache)(VOID *, UINT32, UINT32);
typedef SINT32 (*FN_VFMW_MemMapRegisterAddr)(UADDR, UINT32, MEM_RECORD_S *);
typedef SINT32 (*FN_VFMW_InsertImgToVoQueue)(SINT32, VID_STD_E, VOID *, IMAGE_VO_QUEUE *, IMAGE *);
typedef SINT32 (*FN_VFMW_Dprint)(UINT32, const SINT8 *, ...);
typedef SINT32 (*FN_VFMW_VctrlGetChanIDByCtx)(VOID *);
typedef UADDR  (*FN_VFMW_MemVir2Phy)(UINT8 *);
typedef VOID*  (*FN_VFMW_MemPhy2Vir)(UADDR);
typedef VOID   (*FN_VFMW_ResetVoQueue)(IMAGE_VO_QUEUE *);
typedef SINT32 (*FN_VFMW_RegisterSoftDecoder)(iMediaSDEC_FUNC_S *);
typedef VOID   (*FN_VFMW_UnRegisterSoftDecoder)(VOID);
typedef VOID   (*FN_VFMW_LastFrameNotify)(SINT32, UINT32);

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



