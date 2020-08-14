/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ha_libmad_wrapper.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/3/09
  Description   : 
  History       :
  1.Date        : 2010/3/09
    Author      : z40717
    Modification: Created file

 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HA.AUDIO.MAD.decode.h"
#include "hi_audio_codec.h"
#include "mad.h"

typedef struct hiMp3Instance
{
    /* pointer to mad pstStream structure */
    struct mad_stream *pstStream;

    /* pointer to mad pstFrame structure */
    struct mad_frame *pstFrame;

    /* pointer to mad pstSynth structure */
    struct mad_synth *pstSynth;
} Mp3Instance;

static HI_VOID ClearOutput(HI_HADECODE_OUTPUT_S *pAOut )
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

static HI_HA_ERRORTYPE_E HA_MP3DecInit(HI_VOID * *                     phDecoder,
                                       const HI_HADECODE_OPENPARAM_S * pstOpenParam)
{
    Mp3Instance *hDev;

    if (!pstOpenParam || !phDecoder)
    {
        return HA_ErrorInvalidParameter;
    }

    if (HD_DEC_MODE_RAWPCM != pstOpenParam->enDecMode)
    {
        return HA_ErrorDecodeMode;
    }

    hDev = (Mp3Instance *)malloc(sizeof(Mp3Instance));
    if (HI_NULL == hDev)
    {
        return HA_ErrorInsufficientResources;
    }

    /** allocate resource for  mad structures */
    hDev->pstStream = malloc (sizeof(struct mad_stream));
    hDev->pstSynth = malloc (sizeof(struct mad_synth));
    hDev->pstFrame = malloc (sizeof(struct mad_frame));

    mad_stream_init (hDev->pstStream);
    mad_frame_init (hDev->pstFrame);
    mad_synth_init (hDev->pstSynth);

    *phDecoder = (HI_VOID *)hDev;

    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E HA_MP3DecDeInit(HI_VOID * hDecoder)
{
    Mp3Instance *hDev;

    if (!hDecoder)
    {
        return HA_ErrorInvalidParameter;
    }

    hDev = (Mp3Instance *)hDecoder;
    mad_synth_finish (hDev->pstSynth);
    mad_frame_finish (hDev->pstFrame);
    mad_stream_finish (hDev->pstStream);
    free(hDev->pstStream);
    free(hDev->pstFrame);
    free(hDev->pstSynth);
    free(hDev);
    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E HA_MP3DecReset(HI_VOID * hDecoder)
{
    if (!hDecoder)
    {
        return HA_ErrorInvalidParameter;
    }

    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E HA_MP3DecGetMaxPcmOutSize(HI_VOID * hDecoder,
                                                   HI_U32 *  pOutSizes)
{
    if (!hDecoder || !pOutSizes)
    {
        return HA_ErrorInvalidParameter;
    }

    *pOutSizes = 1152 * 2 * sizeof(HI_S16);

    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E  HA_MP3DecGetMaxBitsOutSize(HI_VOID * hDecoder,
                                                     HI_U32 *  pOutSizes)
{
    if (!hDecoder || !pOutSizes)
    {
        return HA_ErrorInvalidParameter;
    }

    *pOutSizes = 0;

    return HA_ErrorNone;
}

static HI_HA_ERRORTYPE_E HA_SetConfig(HI_VOID * hEncoder, HI_VOID *pstConfigStructure)
{
    if (!hEncoder)
    {
        return HA_ErrorInvalidParameter;
    }

    /*
      TODO
     */
    return HA_ErrorNone;
}


/** The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality.
 */
static inline int ScaleToInt32 (mad_fixed_t sample)
{
#if MAD_F_FRACBITS < 28
    /* round */
    sample += (1L << (28 - MAD_F_FRACBITS - 1));
#endif

    /* clip */
    if (sample >= MAD_F_ONE)
    {
        sample = MAD_F_ONE - 1;
    }
    else if (sample < -MAD_F_ONE)
    {
        sample = -MAD_F_ONE;
    }

#if MAD_F_FRACBITS < 28
    /* quantize */
    sample >>= (28 - MAD_F_FRACBITS);
#endif

    /* convert from 29 bits to 32 bits */
    return (HI_S32) (sample << 3);
}

/* output sample(s) in 16-bit signed interlaved PCM */
static HI_VOID OutputPcm(struct mad_pcm *pcm, HI_U32 u32Channels, HI_U32 s32Samples, HI_S16 *ps16OutData)
{
    mad_fixed_t const *ps32Left, *ps32Right;
    HI_S32 s32Count;

    ps32Left  = pcm->samples[0];
    ps32Right = pcm->samples[1];

    if (u32Channels == 1)
    {
        s32Count = s32Samples;

        while (s32Count--)
        {
            *ps16OutData++ = (ScaleToInt32 (*ps32Left++) >> 16) & 0xffff;
        }
    }
    else
    {
        s32Count = s32Samples;
        while (s32Count--)
        {
            *ps16OutData++ = (ScaleToInt32 (*ps32Left++) >> 16) & 0xffff;
            *ps16OutData++ = (ScaleToInt32 (*ps32Right++) >> 16) & 0xffff;
        }
    }
}

static HI_HA_ERRORTYPE_E HA_MP3DecDecodeFrame(HI_VOID *                hDecoder,
                                              HI_HADECODE_INPACKET_S * pApkt,
                                              HI_HADECODE_OUTPUT_S *   pAOut)
{
    Mp3Instance *hDev;
    HI_S32 s32Ret;
    HI_U32 u32BytesLeft, u32BytesConsumed;
    HI_U8 *pu8ReadPtr;
    HI_U32 u32Channels;
    HI_S32 s32Samples;
    HI_S16 *ps16OutData;
    //static HI_U32 s_u32Frame = 0;

    if (!hDecoder || !pApkt || !pAOut)
    {
        return HA_ErrorInvalidParameter;
    }

    ClearOutput(pAOut);

    s32Ret = HA_ErrorNone;
    hDev = (Mp3Instance *)hDecoder;
    pu8ReadPtr   = pApkt->pu8Data;
    u32BytesLeft = pApkt->s32Size;

    mad_stream_buffer(hDev->pstStream, pu8ReadPtr, u32BytesLeft);

    /* disables the CRC check */
    hDev->pstFrame->header.flags &= ~MAD_FLAG_PROTECTION;
    if (mad_frame_decode (hDev->pstFrame, hDev->pstStream) == -1)
    {
        u32BytesConsumed = hDev->pstStream->next_frame - hDev->pstStream->buffer;

        /* not enough data */
        if (hDev->pstStream->error == MAD_ERROR_BUFLEN)
        {
            s32Ret = HA_ErrorNotEnoughData;
        }
        else
        {
            hDev->pstStream->sync = 0;
            s32Ret = HA_ErrorStreamCorrupt;
        }

        pu8ReadPtr    += u32BytesConsumed;
        u32BytesLeft  -= u32BytesConsumed;
        pApkt->pu8Data = pu8ReadPtr;
        pApkt->s32Size = u32BytesLeft;
        return s32Ret;
    }
    else
    {
        u32BytesConsumed = hDev->pstStream->next_frame - hDev->pstStream->buffer;
        pu8ReadPtr    += u32BytesConsumed;
        u32BytesLeft  -= u32BytesConsumed;
    }

    /* if we're not resyncing/in error, check if caps need to be set again */
    s32Samples = MAD_NSBSAMPLES (&hDev->pstFrame->header) *
                 (hDev->pstStream->options & MAD_OPTION_HALFSAMPLERATE ? 16 : 32);
    u32Channels = MAD_NCHANNELS (&hDev->pstFrame->header);

    /* Update Parameter */
    pAOut->u32PcmOutSamplesPerFrame = s32Samples;
    pAOut->u32OutChannels   = u32Channels;
    pAOut->u32OutSampleRate = hDev->pstFrame->header.samplerate;
    pAOut->u32OrgChannels   = u32Channels;
    pAOut->u32OrgSampleRate = hDev->pstFrame->header.samplerate;
    pAOut->u32BitRate   = hDev->pstFrame->header.bitrate;
    pAOut->bInterleaved = HI_TRUE;
    pAOut->u32BitPerSample = 16;

    mad_synth_frame (hDev->pstSynth, hDev->pstFrame);
    ps16OutData = (unsigned short *)pAOut->ps32PcmOutBuf;
    OutputPcm(&(hDev->pstSynth->pcm), u32Channels, s32Samples, ps16OutData);
#if 0
    s_u32Frame++;
    if ((s_u32Frame & 0x3f) == 0)
    {
        printf("        mad_frame_decode =%d \n", s_u32Frame);
    }
#endif
    pApkt->pu8Data = pu8ReadPtr;
    pApkt->s32Size = u32BytesLeft;
    return s32Ret;
}

#ifdef HA_AUDIO_STATIC_LINK_MODE
HI_HA_DECODE_S g_ha_audio_mp3_decode_entry = {
#else
HI_HA_DECODE_S ha_audio_decode_entry = {
#endif
    .szName               = (const HI_PCHAR )"libmad mpeg audio decoder ",
    .enCodecID            =                                                      HA_AUDIO_ID_MAD,
    .uVersion.u32Version  =                                                   0x10000001,
    .pszDescription       = (const HI_PCHAR)"ha referrence mp3 decoder based libmad",
    .DecInit              = HA_MP3DecInit,
    .DecDeInit            = HA_MP3DecDeInit,
    .DecSetConfig         = HA_SetConfig,
    .DecGetMaxPcmOutSize  = HA_MP3DecGetMaxPcmOutSize,
    .DecGetMaxBitsOutSize = HA_MP3DecGetMaxBitsOutSize,
    .DecDecodeFrame       = HA_MP3DecDecodeFrame,
};
