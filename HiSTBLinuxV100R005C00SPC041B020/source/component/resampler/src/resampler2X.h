
/*****************************************************************************
*             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_spdif.h
* Description:
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01      2006-11-01   z40717    NULL         Create this file.
* 0.02      2009-11-11   z40717    NULL         add 1X2/1x4 and 4x1/2x1 SRC
*     
*****************************************************************************/

#ifndef _RESAMPLERS_H_
#define _RESAMPLERS_H_

#include "resampler_public.h"
#include "resampler_statname.h"


#ifdef __cplusplus
extern "C" {
#endif

/* can't change these defination */

/* Max input sample number per-channel */
#define MAX_SAMPLE_PER_FRAME    2048

#define NBLOCK (4096*2)  

/**************************************************************************************
 * Function:    Resampler2X_Create
 *
 * Description: allocate memory for platform-specific data
 *              clear all the user-accessible fields
 *
 * Inputs:      inrate:  RESAMPLER_RATE_E
 *              outrate: RESAMPLER_RATE_E
 *              convertfrac: RESAMPLER_CONVERT_FRAC_E
 *              chans:   MONO or STEREO
 * Outputs:     none
 *
 * Return:      handle to Resampler instance, 0 if malloc fails
 **************************************************************************************/
HResampler Resampler2X_Create(int inrate, int outrate, RESAMPLER_CONVERT_FRAC_E convertfrac, int chans);


/**************************************************************************************
 * Function:    Resample2X_Flush
 *
 * Description: flush internal resampler state (after seeking, for example)
 *
 * Inputs:      valid Resampler instance pointer (HResampler)
 *
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
void Resampler2X_Flush(HResampler inst);


/**************************************************************************************
 * Function:    Resampler2X_Free
 *
 * Description: free platform-specific data allocated by ResamplerCreate
 *
 * Inputs:      valid Resampler instance pointer (HResampler)
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
void Resampler2X_Free(HResampler inst);


/**************************************************************************************
 * Function:    Resampler2X_Process
 *
 * Description: Resample pcm data to specific samplerate, only for interlaced format
 *
 * Inputs:      inst: valid Resampler instance pointer (HResampler)
 *              inbuf:   pointer to inputbuf
 *              insamps: input number of sample pointers
 * Outputs:     outbuf:  pointer to outputbuf 
 *
 * Return:      output sample number per-channel
 * Notes:       sure insamps < MAXFRAMESIZE
 
 
 **************************************************************************************/
int  Resampler2X_Process(HResampler inst, short *inbuf, int insamps, short *outbuf);


#ifdef __cplusplus
}
#endif

#endif /* _RESAMPLERS_H_ */
