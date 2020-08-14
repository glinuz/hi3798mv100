/*****************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_aenc.c
* Description: Describe main functionality and purpose of this file.
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01      2009-11-24   z40717     NULL         Create this file.
*
*****************************************************************************/
#include "hi_type.h"
#include "hi_drv_aenc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define AENCGetRealChn(hAdec) do {hAenc = hAenc & 0xffff;} while (0)

HI_S32 HI_MPI_AENC_Init(const HI_CHAR* pszCodecNameTable[])
{
    return AENC_Init(pszCodecNameTable);
}

HI_S32 HI_MPI_AENC_RegisterEncoder(const HI_CHAR *pszCodecDllName)
{
    return AENC_RegisterEncoder(pszCodecDllName);
}

HI_S32 HI_MPI_AENC_ShowRegisterEncoder(HI_VOID)
{
    return AENC_ShowRegisterEncoder();
}

HI_S32 HI_MPI_AENC_DeInit(HI_VOID)
{ 
    return AENC_DeInit();
}

HI_S32 HI_MPI_AENC_Open(HI_HANDLE *phAenc, const HI_UNF_AENC_ATTR_S *pstAencAttr)
{
    HI_S32 retval;

    retval = AENC_Open(phAenc, pstAencAttr);
    if (HI_SUCCESS == retval)
    {
        *phAenc = *phAenc | (HI_ID_AENC << 16);
    }

    return retval;
}

HI_S32 HI_MPI_AENC_Close(HI_HANDLE hAenc)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_Close(hAenc);
    return retval;
}

HI_S32 HI_MPI_AENC_SendBuffer(HI_U32 hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_SendBuffer(hAenc, pstAOFrame);
    return retval;
}

HI_S32 HI_MPI_AENC_ReceiveStream(HI_HANDLE hAenc, AENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_ReceiveStream(hAenc, pstStream, u32TimeoutMs);

    return retval;
}

HI_S32 HI_MPI_AENC_ReleaseStream(HI_HANDLE hAenc, const AENC_STREAM_S *pstStream)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_ReleaseStream (hAenc, pstStream);

    return retval;
}

HI_S32 HI_MPI_AENC_SetAutoSRC(HI_HANDLE hAenc, HI_BOOL bEnable)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_SetAutoSRC (hAenc, bEnable);
    return retval;
}

HI_S32 HI_MPI_AENC_SetConfigEncoder(HI_HANDLE hAenc, HI_VOID *pstConfigStructure)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_SetConfigEncoder (hAenc, pstConfigStructure);
    return retval;
}

HI_S32 HI_MPI_AENC_SetEnable(HI_HANDLE hAenc, HI_BOOL bEnable)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_SetEnable(hAenc,bEnable);
    return retval;
}

HI_S32 HI_MPI_AENC_AttachInput(HI_HANDLE hAenc, HI_HANDLE hSource)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_AttachInput(hAenc, hSource);
    if(HI_SUCCESS != retval)
    {
        return retval;
    }
    return AENC_SetEnable(hAenc, HI_TRUE);
}

HI_S32 HI_MPI_AENC_GetAttachSrc(HI_HANDLE hAenc, HI_HANDLE *hSrc)
{
    AENCGetRealChn(hAenc);

    return AENC_GetAttachSrc(hAenc, hSrc);
}

HI_S32 HI_MPI_AENC_DetachInput(HI_HANDLE hAenc)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_DetachInput(hAenc);
    if(HI_SUCCESS != retval)
    {
        return retval;
    }
    return AENC_SetEnable(hAenc, HI_FALSE);
}

HI_S32 HI_MPI_AENC_SetAttr(HI_HANDLE hAenc, const HI_UNF_AENC_ATTR_S *pstAencAttr)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_SetAttr(hAenc, pstAencAttr);
    return retval;
}

HI_S32 HI_MPI_AENC_GetAttr(HI_HANDLE hAenc, HI_UNF_AENC_ATTR_S *pstAencAttr)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_GetAttr(hAenc, pstAencAttr);
    return retval;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
