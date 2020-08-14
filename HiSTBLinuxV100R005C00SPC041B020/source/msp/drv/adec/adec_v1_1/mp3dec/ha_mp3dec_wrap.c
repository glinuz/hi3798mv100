#include "hi_common.h"

#include "hi_audio_codec.h"
#include "mp3dec.h"
#include "aud_osal.h"
#include "HA.AUDIO.MP3.decode.h"
#include "mp3dec_wrap.h"
#include "pvmp3decoder_api.h"

#ifndef HI_ADVCA_FUNCTION_RELEASE
static HI_CHAR s_version[] __attribute__((used)) = USER_VERSION_STRING;
#endif

typedef struct hiMp3Instance
{
    HMP3Decoder hDecoder;
    HI_S32      bInterleaved;
    HI_S32      s32ChannelMode;
} Mp3Instance;

static HI_VOID ClearOutput(HI_HADECODE_OUTPUT_S* pAOut)
{
    pAOut->u32PcmOutSamplesPerFrame = 0;
    pAOut->u32BitsOutBytesPerFrame = 0;
    pAOut->u32OutChannels   = 0;
    pAOut->u32OutSampleRate = 0;
    pAOut->u32OrgChannels   = 0;
    pAOut->u32OrgSampleRate = 0;
    pAOut->u32BitPerSample  = 0;
    pAOut->u32BitRate = 0;
}

static HI_HA_ERRORTYPE_E HA_SetConfig(HI_VOID* hDecoder, HI_VOID* pstConfigStructure)
{
    HA_CODEC_PARAMETER_QUERY_S* pstParam;
    HA_CODEC_FORMAT_QUERY_PARAM_S* pstConfig;
    HA_MP3_GET_CHNANEL_MODE_S* pstCodecData;
    HA_CODEC_PACKETDECODER_QUERY_PARAM_S* pstCodecAttr;
    HI_S32* ps32Data;
    Mp3Instance* hDev;

    if (!pstConfigStructure)
    {
        return HA_ErrorInvalidParameter;
    }
    pstParam = (HA_CODEC_PARAMETER_QUERY_S*)pstConfigStructure;
    if (NULL == hDecoder)
    {
        switch (pstParam->enCmd)
        {
            case HA_CODEC_FORMAT_QUERY_CMD:
            {
                pstConfig = (HA_CODEC_FORMAT_QUERY_PARAM_S*)pstConfigStructure;
                if (FORMAT_MP1 == pstConfig->enFormat || FORMAT_MP2 == pstConfig->enFormat || FORMAT_MP3 == pstConfig->enFormat)
                {
                    return HA_ErrorNone;
                }
                else
                {
                    return HA_ErrorNotSupportCodec;
                }
            }
            case HA_CODEC_PACKETDECODER_QUERY_CMD:
                pstCodecAttr = (HA_CODEC_PACKETDECODER_QUERY_PARAM_S*)pstConfigStructure;
                pstCodecAttr->bPacketDecoder = HI_FALSE;
                break;
            default:
                return HA_ErrorInvalidParameter;
        }
    }
    else
    {
        switch (pstParam->enCmd)
        {
            case HA_CODEC_GET_MP3_CHNANEL_MODE_CMD:
                hDev = (Mp3Instance*)hDecoder;
                pstCodecData = (HA_MP3_GET_CHNANEL_MODE_S*)pstConfigStructure;
                ps32Data = (HI_S32*)(pstCodecData->peChannelMode);
                *ps32Data = hDev->s32ChannelMode;
                break;
            default:
                return HA_ErrorInvalidParameter;
        }
    }
    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E HA_MP3DecInit(HI_VOID** phDecoder,
                                       const HI_HADECODE_OPENPARAM_S* pOpenParam)
{
    HMP3Decoder hDecoder;
    Mp3Instance* hDev;

    if (!pOpenParam || !phDecoder)
    {
        return HA_ErrorInvalidParameter;
    }

    if (HD_DEC_MODE_RAWPCM != pOpenParam->enDecMode)
    {
        return HA_ErrorDecodeMode;
    }

    hDev = (Mp3Instance*)AUDOS_MEM_MALLOC(sizeof(Mp3Instance));
    if (HI_NULL == hDev)
    {
        return HA_ErrorInsufficientResources;
    }

    if ((hDecoder = MP3InitDecoder()) == 0)
    {
        AUDOS_MEM_FREE(hDev);
        return HA_ErrorInsufficientResources;
    }

    hDev->hDecoder       = (HI_VOID*)hDecoder;
    hDev->bInterleaved   = pOpenParam->sPcmformat.bInterleaved;
    hDev->s32ChannelMode = -1;
    *phDecoder = (HI_VOID*)hDev;

    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E HA_MP3DecDeInit(HI_VOID* hDecoder)
{
    Mp3Instance* hDev;

    if (!hDecoder)
    {
        return HA_ErrorInvalidParameter;
    }

    hDev = (Mp3Instance*)hDecoder;
    MP3FreeDecoder(hDev->hDecoder);
    AUDOS_MEM_FREE(hDev);

    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E HA_MP3DecGetMaxPcmOutSize(HI_VOID* hDecoder,
        HI_U32* pOutSizes)
{
    if (!hDecoder || !pOutSizes)
    {
        return HA_ErrorInvalidParameter;
    }

    *pOutSizes = 1152 * 2 * sizeof(HI_S16);

    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E  HA_MP3DecGetMaxBitsOutSize(HI_VOID* hDecoder,
        HI_U32* pOutSizes)
{
    if (!hDecoder || !pOutSizes)
    {
        return HA_ErrorInvalidParameter;
    }

    *pOutSizes = 0;

    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E HA_MP3DecDecodeFrame(HI_VOID*                 hDecoder,
        HI_HADECODE_INPACKET_S* pApkt,
        HI_HADECODE_OUTPUT_S*   pAOut)
{
    HI_S32 bytesLeft, err, frameBytes;
    HI_U8* readPtr;
    MP3FrameInfo Mp3FrameInfo;
    Mp3Instance* hDev;

    if (!hDecoder || !pApkt || !pAOut)
    {
        return HA_ErrorInvalidParameter;
    }

    hDev = (Mp3Instance*)hDecoder;

    readPtr   = pApkt->pu8Data;
    bytesLeft = pApkt->s32Size;

    ClearOutput(pAOut);

    if (bytesLeft < 8)
    {
        return HA_ErrorNotEnoughData;
    }

    frameBytes = MP3DecodeFindSyncHeader(hDev->hDecoder, &readPtr, &bytesLeft);
    if (frameBytes < 0)
    {
        pApkt->pu8Data = readPtr;
        pApkt->s32Size = bytesLeft;
        return HA_ErrorNotEnoughData;
    }

    /* decode one MP3 frame */
    err = MP3Decode(hDev->hDecoder, &readPtr, &bytesLeft, (HI_S16*)pAOut->ps32PcmOutBuf, 0);
    if (NO_DECODING_ERROR == err)
    {
        MP3GetLastFrameInfo(hDev->hDecoder, &Mp3FrameInfo);

        pAOut->u32PcmOutSamplesPerFrame = Mp3FrameInfo.outputSamps / Mp3FrameInfo.nChans;
        pAOut->u32OutChannels   = Mp3FrameInfo.nChans;
        pAOut->u32OutSampleRate = Mp3FrameInfo.samprate;
        pAOut->u32OrgChannels   = Mp3FrameInfo.nChans;
        pAOut->u32OrgSampleRate = Mp3FrameInfo.samprate;
        pAOut->u32BitRate   = Mp3FrameInfo.bitrate;
        pAOut->bInterleaved = hDev->bInterleaved;
        pAOut->u32BitPerSample = 16;
        err = HA_ErrorNone;
        //hDev->s32ChannelMode = ((MP3DecInfo*)(hDev->hDecoder))->mode;
    }
    else if (NO_ENOUGH_MAIN_DATA_ERROR == err)
    {
        err = HA_ErrorNotEnoughData;
    }
    else
    {
        err = HA_ErrorStreamCorrupt;
    }

    pApkt->pu8Data = readPtr;
    pApkt->s32Size = bytesLeft;
    return err;
}

#ifdef HA_AUDIO_STATIC_LINK_MODE
HI_HA_DECODE_S g_ha_audio_mp3_decode_kel_entry = {
#else
HI_HA_DECODE_S g_ha_audio_mp3_decode_kel_entry = {
#endif
    .szName               = (const HI_PCHAR )"android mp3",
    .enCodecID            = HA_AUDIO_ID_MP3,
    .uVersion.u32Version  = 0x10000001,
    .pszDescription       = (const HI_PCHAR)"Hisilicon mp3 decoder",
    .DecInit              = HA_MP3DecInit,
    .DecDeInit            = HA_MP3DecDeInit,
    .DecSetConfig         = HA_SetConfig,
    .DecGetMaxPcmOutSize  = HA_MP3DecGetMaxPcmOutSize,
    .DecGetMaxBitsOutSize = HA_MP3DecGetMaxBitsOutSize,
    .DecDecodeFrame       = HA_MP3DecDecodeFrame,
};
