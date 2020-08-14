/**************************************************************************************
* Hisilicon MP3 decoder
* mp3dec.c - platform-independent top level MP3 decoder API
**************************************************************************************/

#include "mp3dec.h"
#include "mp3dec_wrap.h" /* includes mp3dec.h (public API) and internal, platform-independent API */
#include "pvmp3decoder_api.h"
#include "aud_osal.h"

/**************************************************************************************
* Function:    MP3InitDecoder
*
* Description: allocate memory for platform-specific data
*              clear all the user-accessible fields
*
* Inputs:      none
*
* Outputs:     none
*
* Return:      handle to mp3 decoder instance, 0 if malloc fails
**************************************************************************************/
HMP3Decoder MP3InitDecoder(HI_VOID)
{
    uint32 memRequirements;
    MP3DecInfo* mp3DecInfo;

    mp3DecInfo = (MP3DecInfo* )AUDOS_MEM_MALLOC(sizeof(MP3DecInfo));
    if (mp3DecInfo == NULL)
    {
        return NULL;
    }

    mp3DecInfo->mConfig.equalizerType = flat; /* layer1&2 just support flat EQ */
    mp3DecInfo->mConfig.crcEnabled = 0;
    
    memRequirements = pvmp3_decoderMemRequirements();
    mp3DecInfo->mDecoderBuf = AUDOS_MEM_MALLOC(memRequirements);
    if (mp3DecInfo->mDecoderBuf == NULL)
    {
        AUDOS_MEM_FREE(mp3DecInfo);
        return NULL;
    }

    pvmp3_InitDecoder(&mp3DecInfo->mConfig, mp3DecInfo->mDecoderBuf);

    return (HMP3Decoder)(mp3DecInfo);
}

/**************************************************************************************
* Function:    MP3GetLastFrameInfo
*
* Description: get info about last MP3 frame decoded (number of sampled decoded,
*                sample rate, bitrate, etc.)
*
* Inputs:      valid MP3 decoder instance pointer (HMP3Decoder)
*              pointer to MP3FrameInfo struct
*
* Outputs:     filled-in MP3FrameInfo struct
*
* Return:      none
*
* Notes:       call this right after calling MP3Decode
**************************************************************************************/
HI_VOID MP3GetLastFrameInfo(HMP3Decoder hMP3Decoder, MP3FrameInfo* mp3FrameInfo)

{
    MP3DecInfo* mp3DecInfo = (MP3DecInfo*)hMP3Decoder;

    if (!mp3DecInfo)
    {
        mp3FrameInfo->bitrate = 0;
        mp3FrameInfo->nChans   = 0;
        mp3FrameInfo->samprate = 0;
        mp3FrameInfo->bitsPerSample = 0;
        mp3FrameInfo->outputSamps = 0;
        mp3FrameInfo->layer   = 0;
        mp3FrameInfo->version = 0;
    }
    else
    {
        mp3FrameInfo->bitrate = mp3DecInfo->mConfig.bitRate;
        mp3FrameInfo->nChans   = mp3DecInfo->mConfig.num_channels;
        mp3FrameInfo->samprate = mp3DecInfo->mConfig.samplingRate;
        mp3FrameInfo->bitsPerSample = 16;
        mp3FrameInfo->outputSamps = mp3DecInfo->mConfig.outputFrameSize;
        mp3FrameInfo->version = mp3DecInfo->mConfig.version;
    }
}

/*****************************************************************************
  Function:    MP3DecodeFindSyncHeader
  Description: look for valid MPEG sync header.
  Calls:
  Called By:
  Input:       hMP3Decoder   --MP3-Decoder handle
               ppInbufPtr    --address of the pointer of start-point of the bitstream.
                           NOTE: bitstream should be in little endian format.
               pBytesLeft --pointer to BytesLeft that indicates bitstream numbers at input buffer;
                           after FindSync, pInbufPtr pointer to current bitsrream header, BytesLeft
                           indicates the left bytes.

  Output:      updated bitstream buffer state(ppInbufPtr, pBytesLeft)

  Return:      <0    : ERR_MP3_INDATA_UNDERFLOW
               other : Success, return number bytes  of current frame.
*****************************************************************************/

static int MP3FindSyncWord(unsigned char* buf, int nBytes)
{
    int i;

    /* find byte-aligned syncword - need 12 (MPEG 1,2) or 11 (MPEG 2.5) matching bits */
    for (i = 0; i < nBytes - 1; i++)
    {
        if (((buf[i + 0] & SYNCWORDH) == SYNCWORDH) && (buf[i + 1] & SYNCWORDL) == SYNCWORDL)
        {
            return i;
        }
    }

    return -1;
}

int MP3DecodeFindSyncHeader(HMP3Decoder hMP3Decoder, unsigned char **inbuf, int *bytesLeft)
{
    int offset, frmBytes, totBytes;
    MP3DecInfo *mp3DecInfo = (MP3DecInfo *)hMP3Decoder;
    int nReserved = 0;

    if (!mp3DecInfo)
    {
        return ERR_MP3_NULL_POINTER;
    }

    totBytes = *bytesLeft;

    while (1)
    {
        if ((*bytesLeft) < 4)
        {
            return ERR_MP3_INDATA_UNDERFLOW; /* 输入数据尚不足以译码header */
        }

        /* find start of next MP3 frame - assume EOF if no sync found */
        offset = MP3FindSyncWord(*inbuf, *bytesLeft);
        if (offset < 0)
        {
            /* 至输入buffer有效数据结束处都没有检测到有效帧头 */
            *inbuf += (*bytesLeft) - 4;
            *bytesLeft = 4;
            return ERR_MP3_INDATA_UNDERFLOW;
        }

        *inbuf += offset;
        *bytesLeft -= offset;
        if (*bytesLeft < 4)
        {
            return ERR_MP3_INDATA_UNDERFLOW; /* 输入数据尚不足以译码header */
        }

        /* unpack frame header */
        mp3DecInfo->mConfig.pInputBuffer = *inbuf;
        mp3DecInfo->mConfig.inputBufferCurrentLength = *bytesLeft;
        mp3DecInfo->mConfig.inputBufferMaxLength  = *bytesLeft;
        mp3DecInfo->mConfig.inputBufferUsedLength = nReserved;
        mp3DecInfo->mConfig.outputFrameSize = 0;
        mp3DecInfo->mConfig.pOutputBuffer = (int16*)(HI_NULL);
        frmBytes = pvmp3_UnpackFrameHeader(&mp3DecInfo->mConfig, mp3DecInfo->mDecoderBuf);
        if (frmBytes <= 0)
        {
            *inbuf += 1; /* 本次帧头无效，输入指针挪至下一字节处准备再次寻找帧头 (L40186) */
            *bytesLeft -= 1; /* 能否不返回，再次检测帧头直到有效帧头出现或者剩余输入不足判断一个有效帧头为止再返回上层 */
            continue;
        }

        if ((*bytesLeft) >= (frmBytes + 6))
        {
            if ((((*inbuf)[frmBytes + 0] & SYNCWORDH)
                 == SYNCWORDH) && ((*inbuf)[frmBytes + 1] & SYNCWORDL) == SYNCWORDL)
            {

                return frmBytes;
            }
            else
            {
                *inbuf += 1;
                *bytesLeft -= 1;
                continue;
            }
        }
        else
        {
            return ERR_MP3_INDATA_UNDERFLOW;
        }

    }

    return ERR_MP3_INDATA_UNDERFLOW;
}

/*****************************************************************************
  Function:    MP3FreeDecoder
  Description: destroy MP3-Decoder, free the memory.
  Calls:
  Called By:
  Input:       hMP3Decoder --Mp3-Decoder handle
  Output:
  Return:
  Others:
*****************************************************************************/
HI_VOID MP3FreeDecoder(HMP3Decoder hMP3Decoder)
{
    MP3DecInfo* mp3DecInfo = (MP3DecInfo*)hMP3Decoder;

    if (mp3DecInfo)
    {
        if (mp3DecInfo->mDecoderBuf)
        {
            AUDOS_MEM_FREE(mp3DecInfo->mDecoderBuf);
            mp3DecInfo->mDecoderBuf = NULL;
        }

        if (mp3DecInfo)
        {
            AUDOS_MEM_FREE(mp3DecInfo);
            mp3DecInfo = NULL;
        }
    }
}

#if 0
static void uninterleave(short*  pTimein,
                         short*  pTimeOut,
                         int     frameSize,
                         short*  channels)
{
    int i, j;
    int ch = *channels;

    if (ch == 1)
    {
        for (j = 0; j < frameSize; j++)
        {
            pTimeOut[j ] = pTimein[j];
            pTimeOut[j + frameSize] = pTimein[j];
        }

        *channels = 2;
    }
    else if (ch == 2)
    {
        for (i = 0; i < ch; i++)
        {
            for (j = 0; j < frameSize; j++)
            {
                pTimeOut[i * frameSize + j] = pTimein[j * ch + i];
            }
        }
    }
}
#endif

/**************************************************************************************
* Function:    MP3Decode
*
* Description: decode one frame of MP3 data
*
* Inputs:      valid MP3 decoder instance pointer (HMP3Decoder)
*              double pointer to buffer of MP3 data (containing headers + mainData)
*              number of valid bytes remaining in inbuf
*              pointer to outbuf, big enough to hold one frame of decoded PCM samples
*              flag indicating whether MP3 data is normal MPEG format (useSize = 0)
*                or reformatted as "self-contained" frames (useSize = 1)
*
* Outputs:     PCM data in outbuf, interleaved LRLRLR... if stereo
*                number of output samples = nGrans * nGranSamps * nChans
*              updated inbuf pointer, updated bytesLeft
*
* Return:      error code, defined in mp3dec.h (0 means no error, < 0 means error)
*
* Notes:       switching useSize on and off between frames in the same stream
*                is not supported (bit reservoir is not maintained if useSize on)
**************************************************************************************/
HI_S32  MP3Decode(HMP3Decoder hMP3Decoder, HI_U8** ppInbufPtr, HI_S32* pBytesLeft, HI_S16* pOutPcm, HI_S32 nReserved)
{
    MP3DecInfo* mp3DecInfo = (MP3DecInfo*)hMP3Decoder;
    ERROR_CODE decoderErr = NO_DECODING_ERROR;
    HI_S32 Ret;
    int bytesMax = *pBytesLeft;

    mp3DecInfo->mConfig.pInputBuffer = *ppInbufPtr;
    mp3DecInfo->mConfig.inputBufferCurrentLength = *pBytesLeft;
    mp3DecInfo->mConfig.inputBufferMaxLength  = *pBytesLeft;
    mp3DecInfo->mConfig.inputBufferUsedLength = 0;
    mp3DecInfo->mConfig.outputFrameSize = (1152 * 2);
    mp3DecInfo->mConfig.pOutputBuffer = (int16*)(pOutPcm);

    decoderErr = pvmp3_framedecoder(&mp3DecInfo->mConfig, mp3DecInfo->mDecoderBuf);

    if ((mp3DecInfo->mConfig.inputBufferUsedLength < 0) || (mp3DecInfo->mConfig.inputBufferUsedLength > bytesMax)
        || (mp3DecInfo->mConfig.pInputBuffer < *ppInbufPtr)
        || (mp3DecInfo->mConfig.pInputBuffer > (*ppInbufPtr + bytesMax)))
    {
        mp3DecInfo->mConfig.bitRate = 0;
        mp3DecInfo->mConfig.num_channels = 0;
        mp3DecInfo->mConfig.samplingRate = 0;
        mp3DecInfo->mConfig.outputFrameSize = 0;
        mp3DecInfo->mConfig.version = 0;
        mp3DecInfo->mConfig.inputBufferUsedLength = bytesMax;
        decoderErr = READ_FILE_ERROR;
    }


    if ((NO_DECODING_ERROR == decoderErr) || (NO_ENOUGH_MAIN_DATA_ERROR == decoderErr))
    {
        Ret = ERR_MP3_NONE;
    }
    else
    {
        Ret = ERR_MP3_UNKNOWN;
    }

    *ppInbufPtr += mp3DecInfo->mConfig.inputBufferUsedLength;
    *pBytesLeft -= mp3DecInfo->mConfig.inputBufferUsedLength;

    return Ret;
}
