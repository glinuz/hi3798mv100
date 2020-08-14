
#ifndef _HI_RESAMPLER_H_
#define _HI_RESAMPLER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void *HResampler;

/******************************* API Declaration *****************************/

/**************************************************************************************
 * Function:    HI_Resampler_Create
 *
 * Description: allocate memory for platform-specific data
 *              clear all the user-accessible fields
 *
 * Inputs:      inrate:  RESAMPLER_RATE_E
 *              outrate: RESAMPLER_RATE_E
 *              chans:   MONO or STEREO
 * Outputs:     none
 *
 * Return:      handle to Resampler instance, 0 if malloc fails
 **************************************************************************************/
HResampler HI_Resampler_Create(int inrate, int outrate, int chans);


/**************************************************************************************
 * Function:    HI_Resampler_Process
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
int  HI_Resampler_Process(HResampler inst, short *inbuf, int insamps, short *outbuf);


/**************************************************************************************
 * Function:    HI_Resampler_Free
 *
 * Description: free platform-specific data allocated by ResamplerCreate
 *
 * Inputs:      valid Resampler instance pointer (HResampler)
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
void HI_Resampler_Free(HResampler inst);

/** @}*/  /** <!-- ==== API Declaration End ====*/


#ifdef __cplusplus
}
#endif



#endif
