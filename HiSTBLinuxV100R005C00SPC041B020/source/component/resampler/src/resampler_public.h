
#ifndef _RESAMPLER_PUBLIC_H_
#define _RESAMPLER_PUBLIC_H_

#include "hi_resampler.h"
#include "resampler_statname.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LINEAR_CHANNELS 10

#define LINEARSRC_FIXPOINT /* fast fix point linear */

typedef struct
{
    int BitPerSample;
    int Channels;
    int AdjustInRate;
    int InRate;
    int OutRate;
    int Remainder;
    int DisContinuity;
    int SpeedAdjust;
#ifdef  LINEARSRC_FIXPOINT
    int IvtSFFactor;
#endif
    int PreSample[MAX_LINEAR_CHANNELS];
} SRC_Linear;


typedef struct
{
    int      up;
    int      dn;
    int      inrate;
    int      outrate;
    int      convertfrac;
    int      nchans;
    int      nwing;
    int      nhist;
    int      phase;
    int      offset;
    int      isCloned;
    int      bitwidth;  // indicate bit width
    short   *histbuf;   // for 24bit width
    short   *pcmbuf;    // for 24bit width
    int   *  histbuf32; // for 24bit width
    int   *  pcmbuf32;  // for 24bit width
    short *  lwingptr;
    short *  rwingptr;
    short *  filter;
    int   *  nextphase;
    int   isForec2Out32bit;
    int      isLinearMode;
    int      isResampler2X;
	SRC_Linear *pLinearSrc;

} state_t;

/*
   sample rate defination
 */
typedef enum
{
    RESAMPLE_RATE_08K = 8000,
    RESAMPLE_RATE_11K = 11025,
    RESAMPLE_RATE_12K = 12000,
    RESAMPLE_RATE_16K = 16000,
    RESAMPLE_RATE_22K = 22050,
    RESAMPLE_RATE_24K = 24000,
    RESAMPLE_RATE_32K = 32000,
    RESAMPLE_RATE_44K = 44100,
    RESAMPLE_RATE_48K = 48000,
    RESAMPLE_RATE_88K = 88200,
    RESAMPLE_RATE_96K  = 96000,
	RESAMPLE_RATE_176K = 176400,
    RESAMPLE_RATE_192K = 192000
} RESAMPLER_RATE_E;

typedef enum
{
    RESAMPLE_1X1 = 0x0000,
    RESAMPLE_1X2 = 0x0001,
    RESAMPLE_2X1 = 0x0002,
    RESAMPLE_1X4 = 0x0003,
    RESAMPLE_4X1 = 0x0004,
    RESAMPLE_1X6 = 0x0005,
    RESAMPLE_6X1 = 0x0006,
    RESAMPLE_1X3 = 0x0007,
    RESAMPLE_3X1 = 0x0008,
    RESAMPLE_UNSUPPORT,
} RESAMPLER_CONVERT_FRAC_E;

typedef enum {
    MONO   = 1,
    STEREO = 2,
}RESAMPLER_CHAN;

#ifdef __cplusplus
}
#endif

#endif


