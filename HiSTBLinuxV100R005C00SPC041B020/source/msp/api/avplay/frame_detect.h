/*
@ H264/MPEG2/MPEG4 frame type detect header file

@ Author:

@ Created:

@ Modified:
*/

#ifndef  __FRAME_DETECT_H__
#define  __FRAME_DETECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hi_type.h"

typedef struct hi_DF_CTX_S
{
    HI_S32      eCtxState;
    HI_S32      eVidStd;

    HI_S32      s32PreNalType;
    HI_S32      s32CurNalType;
    HI_U8      *pNalAddr;
    HI_SIZE_T   NalLen;
    HI_U8       u8NalData[64];

    HI_S32      s32NewFrmFlag;
    HI_S32      s32NewFrmType;

    HI_S32      s32CurFrmType;

    HI_S32      stSPS[32];
    HI_S32      u8SpsFresh[32];
    HI_S32      stPPS[256];
    HI_S32      s32SpsIdForPps[256];

    HI_S32      s32SpsID;
    HI_S32      s32PpsID;

} DF_CTX_S;

#define  FRAME_TYPE_N       0
#define  FRAME_TYPE_I       1
#define  FRAME_TYPE_P       2
#define  FRAME_TYPE_B       3

#define  STD_H264           1
#define  STD_MPEG2          2
#define  STD_MPEG4          3
#define  STD_HEVC           4

#define DF_MAX_CTX_NUM  64

HI_S32 DetFrm_Create(HI_HANDLE *hDFCtx, HI_S32 VidStd);
HI_S32 DetFrm_Destroy(HI_HANDLE hDFCtx);
HI_S32 DetFrm_Start(HI_HANDLE hDFCtx);
HI_S32 DetFrm_Stop(HI_HANDLE hDFCtx);
HI_S32 DetFrm_Check(HI_HANDLE hDFCtx);
HI_S32 DetFrm_GetDFCtx(HI_HANDLE hDFCtx, DF_CTX_S **pCtx);


#ifdef __cplusplus
}
#endif

#endif //_FRAME_DETECT_H_
