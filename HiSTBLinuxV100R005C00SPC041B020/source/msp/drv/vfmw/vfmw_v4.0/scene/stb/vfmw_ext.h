#ifndef __VFMW_EXT_H__
#define __VFMW_EXT_H__

typedef HI_VOID  (*FN_VFMW_OpenModule)(HI_VOID);
typedef HI_VOID  (*FN_VFMW_ExitModule)(HI_VOID);
typedef HI_S32   (*FN_VFMW_Init)(HI_S32 (*VdecCallback)(HI_S32, HI_S32, HI_VOID *));
typedef HI_S32   (*FN_VFMW_InitWithOperation)(VDEC_OPERATION_S *);
typedef HI_S32   (*FN_VFMW_Control)(HI_S32, VDEC_CID_E, HI_VOID *);
typedef HI_S32   (*FN_VFMW_Exit)(HI_VOID);
typedef HI_S32   (*FN_VFMW_Suspend)(HI_VOID);
typedef HI_S32   (*FN_VFMW_Resume)(HI_VOID);
typedef HI_S32   (*FN_VFMW_SetDbgOption)(HI_U32, HI_U8 *);

typedef HI_S32   (*FN_VFMW_TrustDecoderInit)(VDEC_OPERATION_S *pArgs);
typedef HI_S32   (*FN_VFMW_TrustDecoderExit)(HI_VOID);


typedef struct
{
    FN_VFMW_OpenModule          pfnVfmwOpenModule;
    FN_VFMW_ExitModule          pfnVfmwExitModule;
    FN_VFMW_Init                pfnVfmwInit;
    FN_VFMW_InitWithOperation   pfnVfmwInitWithOperation;
    FN_VFMW_Control             pfnVfmwControl;
    FN_VFMW_Exit                pfnVfmwExit;
    FN_VFMW_Suspend             pfnVfmwSuspend;
    FN_VFMW_Resume              pfnVfmwResume;
    FN_VFMW_SetDbgOption        pfnVfmwSetDbgOption;
    FN_VFMW_TrustDecoderInit    pfnVfmwTrustDecoderInit;
    FN_VFMW_TrustDecoderExit    pfnVfmwTrustDecoderExit;

}   VFMW_EXPORT_FUNC_S;
#endif
