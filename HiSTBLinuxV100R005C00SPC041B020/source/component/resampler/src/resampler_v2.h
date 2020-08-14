
/*****************************************************************************
*             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: resampler_v2.h
* Description:
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01      2006-11-01   z40717    NULL         Create this file.
*
*****************************************************************************/

#ifndef _RESAMPLERS_V2_H_
#define _RESAMPLERS_V2_H_
#include "hi_type.h"
#include "resampler_public.h"
#include "resampler_statname.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Max input sample number per-channel */
#define MAXFRAMESIZE 2048

/*
   Mono or Stereo input pcm sample format defination
          =16bit:   fill 16bit word memory
          >16bit:   fill 32bit word memory, left alignment(valid data in high)

 */

enum
{
    IN_PCM_16BIT_RAW = 16,
    IN_PCM_18BIT_RAW = 18,
    IN_PCM_20BIT_RAW = 20,
    IN_PCM_24BIT_RAW = 24
};


/* X5 Resampler right & error code  */

#ifndef HI_SUCCESS
 #define HI_SUCCESS 0
#endif

#ifndef HI_ERR_RESAMPLE_PREFIX
 #define HI_ERR_RESAMPLE_PREFIX 0x80000000
#endif

/* input handle is invalid */
#define HI_ERR_RESAMPLE_HANDLE (HI_ERR_RESAMPLE_PREFIX | 0x0001)

/* pcm circ buffer state is invalid */
#define HI_ERR_RESAMPLE_PCMBUF (HI_ERR_RESAMPLE_PREFIX | 0x0002)

/* input sample number is more than MAXFRAMESIZE or input buffer size
   , or input sample number is not even*/
#define HI_ERR_RESAMPLE_SAMPLE_NUMBER (HI_ERR_RESAMPLE_PREFIX | 0x0003)

/* output pcm buffer space is not enough */
#define HI_ERR_RESAMPLE_OUTPCM_SPACE (HI_ERR_RESAMPLE_PREFIX | 0x0004)

/* the channels of input pcm is invalid */
#define HI_ERR_PCM_CHANNEL (HI_ERR_RESAMPLE_PREFIX | 0x0005)

/* the bit width of input pcm is invalid */
#define HI_ERR_PCM_FORMAT (HI_ERR_RESAMPLE_PREFIX | 0x0006)

/* invalid bypass flag */
#define HI_ERR_INVALID_BYPASSFLAG (HI_ERR_RESAMPLE_PREFIX | 0x0007)

/* error unknown */
#define HI_ERR_UNKNOWN (HI_ERR_RESAMPLE_PREFIX | 0x0008)

/*******************************************************************************
 * Function:    NEW_ResamplerCreate
 *
 * Description: allocate memory for platform-specific data
 *              clear all the user-accessible fields
 *
 * Inputs:      InRate:       8000  ~ 44100
 *              OutRate:      11025 ~ 48000
 *              Chans:        MONO or STEREO
 *              SampleFormat: IN_PCM_16BIT_RAW or
 *                            IN_PCM_18BIT_RAW  or
 *                            IN_PCM_20BIT_RAW  or
 *							  IN_PCM_24BIT_RAW
 * Outputs:     none
 * Notes:       InRate must be less than OutRate
 * Return:      handle to Resampler instance, 0 if malloc fails
 ******************************************************************************/
HResampler NEW_ResamplerCreate(HI_S32 InRate,
                               HI_S32 OutRate,
                               HI_S32 Chans,
                               HI_S32 SampleFormat,
                               HI_S32 isForec2Out32bit);

/*******************************************************************************
 * Function:    NEW_ResampleFlush
 *
 * Description: flush internal resampler state (after seeking, for example)
 *
 * Inputs:      valid Resampler instance pointer (HResampler)
 * Outputs:     none
 *
 * Return:      none
 ******************************************************************************/
void       NEW_ResampleFlush(HResampler pInst);

/*******************************************************************************
 * Function:    NEW_ResamplerFree
 *
 * Description: free platform-specific data allocated by NEW_ResamplerCreate
 *
 * Inputs:      valid Resampler instance pointer (HResampler)
 * Outputs:     none
 *
 * Return:      none
 ******************************************************************************/
void       NEW_ResamplerFree(HResampler pInst);

/*******************************************************************************
 * Function:    NEW_ResamplerGetMaxOutputNum
 *
 * Description: Caculate max output number at specific input number
 *
 * Inputs:      pInst:    valid Resampler instance pointer (HResampler)
 *              InSamps:  input data number per-channel, insamps must be even
 * Outputs:     none
 * Return:      >=0:      Success, return the max output number per-channel
 *              other:    Fail, return error code
 ******************************************************************************/
HI_S32     NEW_ResamplerGetMaxOutputNum(HResampler pInst,
                                        HI_S32         InSamps);

/*******************************************************************************
 * Function:    NEW_ResamplerProcessFrame
 *
 * Description: Resample one frame pcm data
 * Inputs:      pInst     :   valid Resampler instance pointer (HResampler)
 *              pInPcmBuf :   pointer to input linear pcmbuf
 *              pOutpcmBuf:   pointer to input linear pcmbuf
 *              InSamps   :   input pcm sample number per-channel, must be even.
 *              InRate    :   input  sample rate
 *              OutRate   :   Output sample rate
 * Outputs:     outbuf    :   pointer to outputbuf.
 *
 * Return:      >= 0  : Success, return output pcm sample number per-channel
 *              other : Fail, return error code.
 ******************************************************************************/
HI_S32     NEW_ResamplerProcessFrame(HResampler pInst,
                                     HI_S32 *       pInPcmBuf,
                                     HI_S32 *       pOutpcmBuf,
                                     HI_S32         InSamps,
                                     HI_S32         InRate,
                                     HI_S32         OutRate,
                                     HI_S32         bInterleaved);

HI_S32 NEW_ResamplerCheck(HResampler inst, HI_S32 inrate, HI_S32 outrate, HI_S32 chans, HI_S32 SampleFormat);

#ifdef __cplusplus
}
#endif

#endif /* _RESAMPLERS_V2_H_ */
