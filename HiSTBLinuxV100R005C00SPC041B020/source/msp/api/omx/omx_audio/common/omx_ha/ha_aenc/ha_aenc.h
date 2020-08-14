/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : ha_adec.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HA_AENC_H__
#define __HA_AENC_H__

#include <OMX_Core.h>
#include "hi_type.h"
#include "omx_audio_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


HI_S32      HA_OMX_InitEncoder(HI_AUDDATATYPE* pHAData);
HI_VOID     HA_OMX_DeInitEncoder(HI_AUDDATATYPE* pHAData);
HI_S32      HA_OMX_EncodeFrame(HI_AUDDATATYPE* pHAData, OMX_BUFFERHEADERTYPE* pInBufHdr, OMX_BUFFERHEADERTYPE* pOutBufHdr);
HI_VOID     HA_OMX_FlushAencInnerState(HI_AUDDATATYPE* pHAData);
HI_VOID     HA_OMX_UnRegisterEncoderLib(HI_AUDDATATYPE* pHAData);
HI_HA_ENCODE_S* HA_OMX_RegisterEncoderLib(const HI_CHAR* pszEncoderDllName);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HA_AENC_H__ */
