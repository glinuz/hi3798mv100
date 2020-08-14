/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : HA.AUDIO.WMA.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 
 Last Modified :
 Description   : 
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_WMA_H__
#define __HISI_AUDIO_DECODER_WMA_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HA_WMA9STD_ID 0x0006        

typedef struct hiWMA_FORMAT_S
{
    HI_U16 wFormatTag;          /* format type,0x160->WMAV1,0x161->WMAV2, 0x162->WMAV3 */
    HI_U16 nChannels;            /* number of channels (i.e. mono, stereo...) */
    HI_U32 nSamplesPerSec;   /* sample rate */
    HI_U32 nAvgBytesPerSec;  /* for buffer estimation */
    HI_U16 nBlockAlign;          /* block size of data */
    HI_U16 wBitsPerSample;   /* number of bits per sample of mono data */
    HI_U16 cbSize;                /* the count in bytes of the size of */
    HI_U16 cbExtWord[16];       /* extra information (after cbSize).
                                WMAV1: need  4 Bytes extra information at least
                                    WMAV2: need 10 Bytes extra information at least
                                WMAV3: need 18 Bytes extra information at least
                                 */
} WMA_FORMAT_S;

#define HA_WMA_DecGetDefalutOpenParam(pOpenParam, pstPrivateParams) \
do{ HI_S32 i; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_RAWPCM; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 2; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_FALSE; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 48000; \
    for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
    { \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
    } \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(WMA_FORMAT_S); \
}while(0)

/*=====================================================================
	CMP demux, only use for debug
=====================================================================*/

#if 1
#define HA_WMA_ParseCmp(pfcmp, pstWfx) \
({ HI_S32 nRead; \
    nRead=0; \
    fread(&(((WMA_FORMAT_S *)pstWfx)->wFormatTag), sizeof(HI_U16), 1, (FILE *)pfcmp); \
    fread(&(((WMA_FORMAT_S *)pstWfx)->nChannels), sizeof(HI_U16), 1, (FILE *)pfcmp); \
    fread(&(((WMA_FORMAT_S *)pstWfx)->nSamplesPerSec), sizeof(HI_U32), 1, (FILE *)pfcmp); \
    fread(&(((WMA_FORMAT_S *)pstWfx)->nAvgBytesPerSec), sizeof(HI_U32), 1, (FILE *)pfcmp); \
    fread(&(((WMA_FORMAT_S *)pstWfx)->nBlockAlign), sizeof(HI_U16), 1, (FILE *)pfcmp); \
    fread(&(((WMA_FORMAT_S *)pstWfx)->wBitsPerSample), sizeof(HI_U16), 1, (FILE *)pfcmp); \
    fread(&(((WMA_FORMAT_S *)pstWfx)->cbSize), sizeof(HI_U16), 1, (FILE *)pfcmp); \
    nRead += 7*sizeof(HI_U16)+ 2*sizeof(HI_U32); \
    if (((WMA_FORMAT_S *)pstWfx)->wFormatTag != 1) \
    { \
    } \
    if (((WMA_FORMAT_S *)pstWfx)->cbSize == 4) \
    { \
        ((WMA_FORMAT_S *)pstWfx)->wFormatTag = 0x160; \
        fread(&(((WMA_FORMAT_S *)pstWfx)->cbExtWord), 1, ((WMA_FORMAT_S *)pstWfx)->cbSize, (FILE *)pfcmp); \
       	nRead +=((WMA_FORMAT_S *)pstWfx)->cbSize; \
    } \
    else if ((((WMA_FORMAT_S *)pstWfx)->cbSize == 10) || (((WMA_FORMAT_S *)pstWfx)->cbSize == (10 + 22))) \
    { \
        ((WMA_FORMAT_S *)pstWfx)->wFormatTag = 0x161; \
        fread(&(((WMA_FORMAT_S *)pstWfx)->cbExtWord), 1, ((WMA_FORMAT_S *)pstWfx)->cbSize, (FILE *)pfcmp); \
       	nRead +=((WMA_FORMAT_S *)pstWfx)->cbSize; \
    } \
    else \
    { \
    } \
    nRead; \
})

#else
static HI_S32 HA_WMA_ParseCmp(FILE *pfcmp, WMA_FORMAT_S *pstWfx)
{
       HI_S32 nRead=0;

#if 1                                                                 
	fread(&(pstWfx->wFormatTag), sizeof(HI_U16), 1, pfcmp);       
	fread(&(pstWfx->nChannels), sizeof(HI_U16), 1, pfcmp);        
	fread(&(pstWfx->nSamplesPerSec), sizeof(HI_U32), 1, pfcmp);   
	fread(&(pstWfx->nAvgBytesPerSec), sizeof(HI_U32), 1, pfcmp);  
	fread(&(pstWfx->nBlockAlign), sizeof(HI_U16), 1, pfcmp);      
	fread(&(pstWfx->wBitsPerSample), sizeof(HI_U16), 1, pfcmp);   
	fread(&(pstWfx->cbSize), sizeof(HI_U16), 1, pfcmp);           
	nRead += 7*sizeof(HI_U16)+ 2*sizeof(HI_U32);                  
#endif                                                                
	if (pstWfx->wFormatTag != 1) // not v1 or v2
	{
		//HA_PRINT("Support only CMP V1 format wFormatTag=0x%x!\n",pstWfx->wFormatTag);
		return -1;
	}

	if (pstWfx->cbSize == 4)  //g45208
	{
		//WMA v1 bitstream
		pstWfx->wFormatTag = 0x160;
		fread(&pstWfx->cbExtWord, 1, pstWfx->cbSize, pfcmp);
       	nRead +=pstWfx->cbSize;
	}
	else if ((pstWfx->cbSize == 10) || (pstWfx->cbSize == (10 + 22) ) )  //g45208
	{
		//WMA v2/v3 bitstream
		pstWfx->wFormatTag = 0x161;
		fread(&pstWfx->cbExtWord, 1, pstWfx->cbSize, pfcmp);
       	nRead +=pstWfx->cbSize;
	}
	else
	{
		//HA_PRINT("Error cbSize=%d\n",pstWfx->cbSize);
		return -1;
	}
	return nRead;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_WMA_H__ */

