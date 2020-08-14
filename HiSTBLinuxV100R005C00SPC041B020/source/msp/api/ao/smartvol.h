#ifndef SMARTVOL_H
#define SMARTVOL_H


#ifdef __cplusplus
extern "C" {
#endif

#define ENA_FIXPOINT

#ifndef HI_S8
#define HI_S8 char
#endif

#ifndef HI_S16
#define HI_S16 short
#endif

#ifndef HI_S32
#define HI_S32 int
#endif

#ifndef HI_U8
#define HI_U8 unsigned char
#endif

#ifndef HI_U16
#define HI_U16 unsigned short
#endif

#ifndef HI_U32
#define HI_U32 unsigned int
#endif

#ifndef HI_VOID
#define HI_VOID void
#endif

#ifndef HI_NULL
#define HI_NULL 0
#endif

#ifdef ENA_FIXPOINT
typedef int    sample_t;
#else
typedef double sample_t;
#endif

#ifndef HI_SUCCESS
#define HI_SUCCESS 0
#endif

#ifndef HI_ERR_SMARTVOL_PREFIX
#define HI_ERR_SMARTVOL_PREFIX 0x80000000
#endif

/* input handle is invalid */
#define HI_ERR_SMARTVOL_HANDLE       (HI_ERR_SMARTVOL_PREFIX | 0x0001)

/* PCM buffer is invalid */
#define HI_ERR_SMARTVOL_PCMBUF       (HI_ERR_SMARTVOL_PREFIX | 0x0002)

/* PCM buffer is invalid */
#define HI_ERR_SMARTVOL_MALLOC_FAIL  (HI_ERR_SMARTVOL_PREFIX | 0x0003)

/* input parameter is invalid */
#define HI_ERR_SMARTVOL_PARA_INVALID (HI_ERR_SMARTVOL_PREFIX | 0x0004)

/* unknown error */
#define HI_ERR_SMARTVOL_UNKNOWN      (HI_ERR_SMARTVOL_PREFIX | 0x0005)

#define ENA_INTERLEAVED_FORMAT  /* Interleaved pcm format, add by z40717 */


#define REALTIME_DETECT_ENABLE       (0x1)
#define AMPLIFER_RESTRICT_ENABLE     (0x2)
#define BIGVOLUME_DETECT_ENABLE      (0x4)
#define VAD_ENABLE                   (0x8)

typedef struct _SamrtVol_In_Para_S
{
    HI_S32 samprate;               // sample rate
    HI_S32 nchans;                 // number of channels
    HI_S32 framelen;               // samples in one frame
    HI_S32 bitdepth;               // bit depth
    HI_S32 fadetime;               // time for fade (ms)
    HI_S32 analyzetime;            // time for one analyze cycle(ms)

    HI_U32 ControlPara;
    HI_U32 ChannelNo;
    HI_S32 RandomVol;
#ifdef ENA_INTERLEAVED_FORMAT
    HI_S32 Interleaved;           // 0:none-Interleaved, 1: Interleaved
#endif
} SamrtVol_In_Para_S, *SamrtVol_In_Para_P;

typedef void* HSmartVol;

/*****************************************************************************
Function:    HI_SmartGetDefaultConfig
Description: get default config parameters
Calls:
Called By:
Input:       InPara -- Vol controller parameters
Output:
Return:      Return:      =0 success
             <0 error code
Others:
*****************************************************************************/
HI_S32 HI_SmartGetDefaultConfig    (SamrtVol_In_Para_P pInPara);

/*****************************************************************************
Function:    HI_SmartVolOpen
Description: create and initial smartvol controller
Calls:
Called By:
Input:       InPara -- Vol controller parameters
Output:
Return:      >0 handle of smartvol controller
             =0 error
Others:
*****************************************************************************/
HSmartVol HI_SmartVolOpen    (SamrtVol_In_Para_P pInPara);

/*****************************************************************************
Function:    HI_SmartVolPro
Description: do smartvol control
Calls:
Called By:
Input:       hSmartVol -- handle of smartvol controller
             inbuf     -- input buffer
             GainMod   -- gain modified
             len       -- sample number of input buffer per channel
Output:      inbuf     -- output buffer == input buffer
Return:      =0 success
             <0 error code
Others:
*****************************************************************************/
HI_S32    HI_SmartVolPro     (HSmartVol hSmartVol, HI_S32* inLbuf, HI_S32* inRbuf, HI_S32 GainMod, HI_S32 len);

/*****************************************************************************
Function:    HI_SmartVolClear
Description: if channel changed, clear the status
Calls:
Called By:
Input:       hSmartVol -- handle of smartvol controller
             InPara    -- Vol controller parameters
Output:
Return:      =0 success
             <0 error code
Others:
*****************************************************************************/
HI_S32    HI_SmartVolClear   (HSmartVol hSmartVol, SamrtVol_In_Para_P pInPara);

/*****************************************************************************
Function:    HI_SmartVolEnable
Description: enable the smartvol control function
Calls:
Called By:
Input:       hSmartVol -- handle of smartvol controller
Output:
Return:      =0 success
             <0 error code
Others:
*****************************************************************************/
HI_S32    HI_SmartVolEnable  (HSmartVol hSmartVol);

/*****************************************************************************
Function:    HI_SmartVolDisable
Description: disable the smartvol control function
Calls:
Called By:
Input:       hSmartVol -- handle of smartvol controller
Output:
Return:      =0 success
             <0 error code
Others:
*****************************************************************************/
HI_S32    HI_SmartVolDisable (HSmartVol hSmartVol);

/*****************************************************************************
Function:    HI_SmartVolClose
Description: close smartvol controller, and free memory
Calls:
Called By:
Input:       hSmartVol -- handle of smartvol controller
Output:
Return:
Others:
*****************************************************************************/
HI_VOID   HI_SmartVolClose   (HSmartVol hSmartVol);

#ifdef __cplusplus
}
#endif

#endif /* SMARTVOL_H */
