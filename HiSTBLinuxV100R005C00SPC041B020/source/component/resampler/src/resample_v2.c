/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCXgrHIPlGWwjY48PA756GbU69BErqnMH8NRXsO
3MWeW8lIaz95498o1oVbt40/G8E7VDVcmScc6VGxMo64wqCy1gAoppeVWwDg2hFGSyaV7k8s
g/AedHuL/IeoJiGWkFAel2UfvxU8ceftNGt2tQu4zMFPgcGisdVtqTEjm/1mJg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/




































#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembly.h"
#include "resampler_statname.h"
#include "resample_config.h"
#include "resample_rom.h"
#include "resampler_v2.h"
#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "hi_debug.h"
#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
 #include "kaiser.h"
#endif
#include "linear_src.h"

#ifndef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
typedef struct
{
    int                 inrate;
    int                 outrate;
    int                 quality;
    int                 nwing;
    const signed int   *nextphase;
    const signed short *filter;
} SRC_Info;

static SRC_Info resampler_info[] = {
 #ifdef ENA_QUALITY_LOW
    { 32000, 48000, QUALITY_LOW,     4, res_32_48_low_nextphasetab,  res_32_48_low_filttab	},
    { 44100, 48000, QUALITY_LOW,     4, res_44_48_low_nextphasetab,  res_44_48_low_filttab	},
    { 48000, 48000, QUALITY_LOW,     8, res_48_48_nextphasetab,      res_48_48_good_filttab },
 #endif

 #ifdef ENA_QUALITY_MEDIUM
    { 32000, 48000, QUALITY_MEDIUM,  8, res_32_48_mid_nextphasetab,  res_32_48_mid_filttab	},
    { 44100, 48000, QUALITY_MEDIUM,  8, res_44_48_mid_nextphasetab,  res_44_48_mid_filttab	},
    { 48000, 48000, QUALITY_MEDIUM,  8, res_48_48_nextphasetab,      res_48_48_good_filttab },

    /* no good quality, actully quality mid, 8000->11025, mid_nextphasetab equals to good_nextphasetab*/
    {  8000, 11025, QUALITY_GOOD,    8, res_8_11_good_nextphasetab,  res_8_11_mid_filttab	},

    /* no good quality, actully quality mid, 16000->22050, mid_nextphasetab equals to good_nextphasetab */
    { 16000, 22050, QUALITY_GOOD,    8, res_8_11_good_nextphasetab,  res_8_11_mid_filttab	},

    /* no good quality, actully quality mid, 8000->22050, mid_nextphasetab equals to good_nextphasetab */
    {  8000, 22050, QUALITY_GOOD,    8, res_8_22_good_nextphasetab,  res_8_22_mid_filttab	},

    /* no good quality, actully quality mid, 16000->44100, mid_nextphasetab equals to good_nextphasetab*/
    { 16000, 44100, QUALITY_GOOD,    8, res_8_22_good_nextphasetab,  res_8_22_mid_filttab	},

    /* no good quality, actully quality mid, 8000->44100, mid_nextphasetab equals to good_nextphasetab */
    {  8000, 44100, QUALITY_GOOD,    8, res_8_44_good_nextphasetab,  res_8_44_mid_filttab	},

    /* no good quality, actully quality mid, 11025->12000, mid_nextphasetab equals to good_nextphasetab */
    { 11025, 12000, QUALITY_GOOD,    8, res_11_12_good_nextphasetab, res_11_12_mid_filttab	},

    /* no good quality, actully quality mid, 22050->24000, mid_nextphasetab equals to good_nextphasetab */
    { 22050, 24000, QUALITY_GOOD,    8, res_11_12_good_nextphasetab, res_11_12_mid_filttab	},

    /* no good quality, actully quality mid 11025->16000, mid_nextphasetab equals to good_nextphasetab */
    { 11025, 16000, QUALITY_GOOD,    8, res_11_16_good_nextphasetab, res_11_16_mid_filttab	},

    /* no good quality, actully quality mid 22050->32000, mid_nextphasetab equals to good_nextphasetab */
    { 22050, 32000, QUALITY_GOOD,    8, res_11_16_good_nextphasetab, res_11_16_mid_filttab	},

    /* no good quality, actully quality mid 11025->24000, mid_nextphasetab equals to good_nextphasetab */
    { 11025, 24000, QUALITY_GOOD,    8, res_11_24_good_nextphasetab, res_11_24_mid_filttab	},

    /* no good quality, actully quality mid 22050->48000, mid_nextphasetab equals to good_nextphasetab */
    { 22050, 48000, QUALITY_GOOD,    8, res_11_24_good_nextphasetab, res_11_24_mid_filttab	},

    /* no good quality, actully quality mid 11025->32000, mid_nextphasetab equals to good_nextphasetab */
    { 11025, 32000, QUALITY_GOOD,    8, res_11_32_good_nextphasetab, res_11_32_mid_filttab	},

    /* no good quality, actully quality mid 11025->48000, mid_nextphasetab equals to good_nextphasetab */
    { 11025, 48000, QUALITY_GOOD,    8, res_11_48_good_nextphasetab, res_11_48_mid_filttab	},

    /* no good quality, actully quality mid 12000->22050, mid_nextphasetab equals to good_nextphasetab */
    { 12000, 22050, QUALITY_GOOD,    8, res_12_22_good_nextphasetab, res_12_22_mid_filttab	},

    /* no good quality, actully quality mid 24000->44100, mid_nextphasetab equals to good_nextphasetab */
    { 24000, 44100, QUALITY_GOOD,    8, res_12_22_good_nextphasetab, res_12_22_mid_filttab	},

    /* no good quality, actully quality mid 12000->44100, mid_nextphasetab equals to good_nextphasetab */
    { 12000, 44100, QUALITY_GOOD,    8, res_12_44_good_nextphasetab, res_12_44_mid_filttab	},
 #endif

 #ifdef ENA_QUALITY_GOOD
    {  8000, 12000, QUALITY_GOOD,	16, res_8_12_good_nextphasetab,  res_8_12_good_filttab	},
    {  8000, 16000, QUALITY_GOOD,	16, res_8_16_good_nextphasetab,  res_8_16_good_filttab	},
    {  8000, 24000, QUALITY_GOOD,	16, res_8_24_good_nextphasetab,  res_8_24_good_filttab	},
    {  8000, 32000, QUALITY_GOOD,	16, res_8_32_good_nextphasetab,  res_8_32_good_filttab	},
    {  8000, 48000, QUALITY_GOOD,	16, res_8_48_good_nextphasetab,  res_8_48_good_filttab	},

    { 11025, 22050, QUALITY_GOOD,	16, res_8_16_good_nextphasetab,  res_8_16_good_filttab	},
    { 11025, 44100, QUALITY_GOOD,	16, res_8_32_good_nextphasetab,  res_8_32_good_filttab	},

    { 12000, 16000, QUALITY_GOOD,	16, res_12_16_good_nextphasetab, res_12_16_good_filttab },

    { 12000, 24000, QUALITY_GOOD,	16, res_8_16_good_nextphasetab,  res_8_16_good_filttab	},
    { 12000, 32000, QUALITY_GOOD,	16, res_12_32_good_nextphasetab, res_12_32_good_filttab },
    { 12000, 48000, QUALITY_GOOD,	16, res_8_32_good_nextphasetab,  res_8_32_good_filttab	},

    { 16000, 24000, QUALITY_GOOD,	16, res_8_12_good_nextphasetab,  res_8_12_good_filttab	},
    { 16000, 32000, QUALITY_GOOD,	16, res_8_16_good_nextphasetab,  res_8_16_good_filttab	},
    { 16000, 48000, QUALITY_GOOD,	16, res_8_24_good_nextphasetab,  res_8_24_good_filttab	},

    { 22050, 44100, QUALITY_GOOD,	16, res_8_16_good_nextphasetab,  res_8_16_good_filttab	},
    { 24000, 32000, QUALITY_GOOD,	16, res_12_16_good_nextphasetab, res_12_16_good_filttab },
    { 24000, 48000, QUALITY_GOOD,	16, res_8_16_good_nextphasetab,  res_8_16_good_filttab	},

    { 32000, 44100, QUALITY_GOOD,	16, res_8_11_good_nextphasetab,  res_8_11_good_filttab	},
    { 32000, 48000, QUALITY_GOOD,	16, res_8_12_good_nextphasetab,  res_8_12_good_filttab	},

    { 44100, 48000, QUALITY_GOOD,	16, res_11_12_good_nextphasetab, res_11_12_good_filttab },
    { 48000, 48000, QUALITY_GOOD,    8, res_48_48_nextphasetab,      res_48_48_good_filttab },
 #endif
};
#endif

#ifndef ASSERT
 #if defined (_WIN32) && defined (_M_IX86) && (defined (_DEBUG) || defined (REL_ENABLE_ASSERTS))
  #define ASSERT(x) if (!(x)) __asm int 3;
 #else
  #define ASSERT(x)/* do nothing */
 #endif
#endif

static HI_S32 ResamplerStereoProcess24(HResampler inst, HI_S32 *inbuf, HI_S32 insamps, HI_S32 *outbuf,
                                       HI_S32 bInterleaved);

/* defaults */

#define DEF_ATTEN 90.0f

#define DEF_PASSBAND 0.88f
#define DEF_STOPBAND 1.0f
#define DEF_DCGAIN 1.0f

/*
 * Compute greatest common divisor.
 */
static int gcd(int a, int b)
{
    while (a != b)
    {
        if (a > b)
        {
            a -= b;
        }
        else
        {
            b -= a;
        }
    }

    return a;
}

#define M2_30 1073741824.0
#define M2_31 2147483648.0

#define MAX_DBL 2147483647.0
#define MIN_DBL -2147483648.0

#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
static short NormAndRound(double x, double q)
{
    if (x >= 0.0)
    {
        x = (x * q + 0.5);
    }
    else
    {
        x = (x * q - 0.5);
    }

    /* clip */
    if (x > MAX_DBL)
    {
        x = MAX_DBL;
    }

    if (x < MIN_DBL)
    {
        x = MIN_DBL;
    }

    return (short)(((int)x) >> 16);
}

#endif

static int cvtShortShort(void *d, const void *s, int n, int bInterleaved, int chans )
{
    short* dst = (short *)d;

    /* 16bit 处理 */
    if ((HI_FALSE == bInterleaved) && (STEREO == chans))
    {
        const short* src = (const short *)s;
        const short* srcEnd = src + n;
        const short* src_R = srcEnd;
        while (src != srcEnd)
        {
            *dst = *src;
            src += 1;
            dst   += 1;
            *dst   = *src_R;
            src_R += 1;
            dst += 1;
        }
    }
    else
    {
        const short* src = (const short *)s;
        const short* srcEnd;
        if (chans == 2)
        {
            n *= 2;
        }

        srcEnd = src + n;

        while (src != srcEnd)
        {
            *dst = *src;
            src += 1;
            dst += 1;
        }
    }
    return (dst - (short *)d);
}

static int cvt2int(void *d, const void *s, int n, int bitdepth, int bInterleaved, int chans )
{
    int* dst = (int *)d;

    /* 16bit 按 32 bit 统一处理 */
    if ((HI_FALSE == bInterleaved) && (STEREO == chans))
    {
        if (bitdepth == 16)
        {
            const short* src = (const short *)s;
            const short* srcEnd = src + n;
            const short* src_R = srcEnd;
            while (src != srcEnd)
            {
                *dst = *src << 16;
                src += 1;
                dst   += 1;
                *dst   = *src_R << 16;
                src_R += 1;
                dst += 1;
            }
        }
        else
        {
            const int* src = (const int *)s;
            const int* srcEnd = src + n;
            const int* src_R = srcEnd;
            while (src != srcEnd)
            {
                *dst = *src;
                src += 1;
                dst   += 1;
                *dst   = *src_R;
                src_R += 1;
                dst += 1;
            }
        }
    }
    else
    {
        if (chans == 2)
        {
            n *= 2;
        }

        if (bitdepth == 16)
        {
            const short* src = (const short *)s;
            const short* srcEnd = src + n;
            while (src != srcEnd)
            {
                *dst = *src << 16;
                src += 1;
                dst += 1;
            }
        }
        else
        {
            const int* src = (const int *)s;
            const int* srcEnd = src + n;
            while (src != srcEnd)
            {
                *dst = *src;
                src += 1;
                dst += 1;
            }
        }
    }

    return dst - (int *)d;
}

static HResampler RAInitResamplerRat(int inrate, int outrate, int nchans,
#ifndef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
                                     int atten, int passband, int stopband, int dcgain, int quality, int bitwidth,
                                     int isForec2Out32bit)
#else
                                     float atten, float passband, float stopband, float dcgain, int quality,
                                     int bitwidth)
#endif
{
    state_t *s;
    int divisor, up, dn, nhist;
    int i, nwing, ntaps;

#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
    int phase, nfilter;
    float beta, fpass, fstop;
    double *lpfilt;
#endif

    if ((nchans < 1) || (nchans > 2))
    {
        return HI_NULL;
    }

    if (passband >= stopband)
    {
        return HI_NULL;
    }

    /* reduce to smallest fraction */
    divisor = gcd(inrate, outrate);
    up = outrate / divisor;
    dn = inrate / divisor;
    if (up > 1280)
    {
        return HI_NULL;
    }                   /* supports standard rates to 96000Hz */

    if (nchans * ((dn + up - 1) / up) > 255)
    {
        return HI_NULL;
    }                   /* pcmstep exceeds short */

#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
    /* use defaults for values <= 0 */
    if (atten <= 0.0f)
    {
        atten = DEF_ATTEN;
    }

    if (passband <= 0.0f)
    {
        passband = DEF_PASSBAND;
    }

    if (stopband <= 0.0f)
    {
        stopband = DEF_STOPBAND;
    }

    if (dcgain <= 0.0f)
    {
        dcgain = DEF_DCGAIN;
    }

    /* compute the filter specs */
    fstop = 1.0f / MAX(up, dn);
    fpass = passband * fstop;
    fstop = stopband * fstop;

    KaiserEstim(fpass, fstop, atten, &nfilter, &beta);

    ntaps   = (nfilter + up - 1) / up;/* length of each filter phase */
    nwing   = (ntaps + 1) / 2;
    ntaps   = nwing * 2;            /* update ntaps */
    nfilter = nwing * up;           /* update nfilter */
    nhist = nchans * ntaps;
    s = (state_t *) HI_MALLOC(HI_ID_SRC, sizeof(state_t));
    s->isCloned = 0;
    s->bitwidth = bitwidth;

    //s->histbuf = (short *) calloc((NBLOCK + nhist), sizeof(short));
    //s->pcmbuf = s->histbuf + nhist;
    if (s->bitwidth == IN_PCM_16BIT_RAW)
    {
        if (isForec2Out32bit)
        {
            s->histbuf32 = (int *) HI_MALLOC(HI_ID_SRC, (NBLOCK + nhist) * sizeof(int));
            s->pcmbuf32 = s->histbuf32 + nhist;
        }
        else
        {
            s->histbuf = (short *) HI_MALLOC(HI_ID_SRC, (NBLOCK + nhist) * sizeof(short));
            s->pcmbuf = s->histbuf + nhist;
        }
    }
    else if (s->bitwidth == IN_PCM_24BIT_RAW)
    {
        s->histbuf32 = (int *) HI_MALLOC(HI_ID_SRC, (NBLOCK + nhist) * sizeof(int));
        s->pcmbuf32 = s->histbuf32 + nhist;
    }

    //	printf("up=%d down=%d ntaps=%d beta=%.2f atten=%.1fdB pass=%.0fHz stop=%.0fHz\n",
    //		up, dn, ntaps, beta, atten, 0.5*outrate*fpass*dn, 0.5*outrate*fstop*dn);

    /* malloc buffers */
    lpfilt = (double *) HI_MALLOC(HI_ID_SRC, nfilter * sizeof(double));

    /* create the lowpass filter */
    KaiserLowpass(nfilter, 0.5f * (fpass + fstop), beta, (dcgain * up), lpfilt);

    /* deinterleave into phases */
    s->filter = (short *) HI_MALLOC(HI_ID_SRC, nfilter * sizeof(short));
    for (phase = 0; phase < up; phase++)
    {
        for (i = 0; i < nwing; i++)
        {
            s->filter[phase * nwing + i] = NormAndRound(lpfilt[i * up + phase], M2_31);
        }
    }

    HI_FREE(HI_ID_SRC, lpfilt);

    s->nextphase = (int *) HI_MALLOC(HI_ID_SRC, up * sizeof(int));

    /* lookup tables, driven by phase */
    for (i = 0; i < up; i++)
    {
        int pcmstep;
        phase   = (i * dn) % up;
        pcmstep = nchans * ((((i + 1) * dn) / up) - ((i * dn) / up));
        s->nextphase[phase] = (pcmstep & 0xff) | ((((up - 1 - (((i + 1) * dn) % up))) << 8) & 0xfff00L) \
                              | (((((i + 1) * dn) % up) << 20) & 0xfff00000L);
    }
#if 0	//Just for debug  CA
    {
        FILE  *ftab;
        short j, *tabptr;
        ftab = fopen("d:\\tab.dat", "w");
        for (i = 0; i < up; i++)
        {
            tabptr = (short*) (&s->filter[nwing * i]);
            for (j = 0; j < nwing; j++)
            {
                fprintf(ftab, "%6d, ", ((short)tabptr[j]));
            }

            fprintf(ftab, "\n ");
        }

        fprintf(ftab, "\n ");
        fprintf(ftab, "\n ");
        for (i = 0; i < up; i++)
        {
            fprintf(ftab, "0x%.8x, ", s->nextphase[i]);
            if (!((i + 1) & 0x7))
            {
                fprintf(ftab, "\n ");
            }
        }

        fclose(ftab);
    }
#endif
#else
    {
        SRC_Info *srcinfoptr = resampler_info;
        int size = sizeof(resampler_info) / sizeof(SRC_Info);
 #ifdef NEW_RATE_SUPPORT
        for (i = 0; i < size; i++)
        {
            if ((srcinfoptr->inrate == inrate) && (srcinfoptr->outrate == outrate) \
                && (srcinfoptr->quality == quality))
            {
                break;
            }

            if ((srcinfoptr->inrate == inrate / 2) && (srcinfoptr->outrate == outrate / 2) \
                && (srcinfoptr->quality == quality))
            {
                break;
            }

            if ((srcinfoptr->inrate == inrate / 4) && (srcinfoptr->outrate == outrate / 4) \
                && (srcinfoptr->quality == quality))
            {
                break;
            }

            srcinfoptr++;
        }

 #else
        for (i = 0; i < size; i++)
        {
            if ((srcinfoptr->inrate == inrate) && (srcinfoptr->outrate == outrate) \
                && (srcinfoptr->quality == quality))
            {
                break;
            }

            srcinfoptr++;
        }
 #endif


        if (i == size)
        {
            return HI_NULL;
        }

        nwing = srcinfoptr->nwing;
        ntaps = nwing * 2;          /* update ntaps */
        nhist = nchans * ntaps;
        s = (state_t *) HI_MALLOC(HI_ID_SRC, sizeof(state_t));
        s->filter = (short*)srcinfoptr->filter;
        s->nextphase = (int*)srcinfoptr->nextphase;
        s->isCloned = 0;
        s->bitwidth = bitwidth; // save bit width info

        /* 16bit 按 32 bit 统一处理 */
        if (s->bitwidth == IN_PCM_16BIT_RAW)
        {
            if (isForec2Out32bit)
            {
                s->histbuf32 = (int *) HI_MALLOC(HI_ID_SRC, (NBLOCK + 64) * sizeof(int));
                s->pcmbuf32 = s->histbuf32 + nhist;
            }
            else
            {
                s->histbuf = (short *) HI_MALLOC(HI_ID_SRC, (NBLOCK + nhist) * sizeof(short));
                s->pcmbuf = s->histbuf + nhist;
            }
        }
        else if ((s->bitwidth == IN_PCM_24BIT_RAW)
                 || (s->bitwidth == IN_PCM_20BIT_RAW)
                 || (s->bitwidth == IN_PCM_18BIT_RAW))
        {
            s->histbuf32 = (int *) HI_MALLOC(HI_ID_SRC, (NBLOCK + 64) * sizeof(int));
            s->pcmbuf32 = s->histbuf32 + nhist;
        }
    }
#endif  /* CONFIG_RES_GENERATE_TRIGTABS_FLOAT */

    /* filter init */
    s->inrate  = inrate;
    s->outrate = outrate;
    s->up = up;
    s->dn = dn;
    s->nchans = nchans;
    s->nwing    = nwing;
    s->nhist    = nhist;
    s->phase    = 0;
    s->offset   = 0;
    s->rwingptr = (short*)(&s->filter[s->phase]);
    s->lwingptr = (short*)(&s->filter[nwing * (up - 1 - s->phase)]);
    s->isForec2Out32bit = isForec2Out32bit;

	s->isLinearMode = 0;
    s->isResampler2X = 0;
	s->pLinearSrc = NULL;		
    return (void *) s;
}


static void ResampleFlush(HResampler inst)
{
    state_t *s = (state_t *)inst;

    if (s != HI_NULL)
    {
        s->phase    = 0;
        s->offset   = 0;
        s->rwingptr = (short*)(&s->filter[s->phase]);
        s->lwingptr = (short*)(&s->filter[s->nwing * (s->up - 1 - s->phase)]);
        s->isCloned = 0;
        if (s->bitwidth == IN_PCM_16BIT_RAW)
        {
            if (s->isForec2Out32bit)
            {
                memset(s->histbuf32, 0, (NBLOCK + s->nhist) * sizeof(int));
                s->pcmbuf32 = s->histbuf32 + s->nhist;
            }
            else
            {
                memset(s->histbuf, 0, (NBLOCK + s->nhist) * sizeof(short));
                s->pcmbuf = s->histbuf + s->nhist;
            }
        }
        else if ((s->bitwidth == IN_PCM_24BIT_RAW)
                 || (s->bitwidth == IN_PCM_20BIT_RAW)
                 || (s->bitwidth == IN_PCM_18BIT_RAW))
        {
            memset(s->histbuf32, 0, (NBLOCK + s->nhist) * sizeof(int));
            s->pcmbuf32 = s->histbuf32 + s->nhist;
        }
    }
}

static void ResamplerFree(HResampler inst)
{
    state_t *s = (state_t *)inst;

    if (s != HI_NULL)
    {
        if (!s->isCloned)
        {
#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
            if (s->filter)
            {
                HI_FREE(HI_ID_SRC, s->filter);
            }

            if (s->nextphase)
            {
                HI_FREE(HI_ID_SRC, s->nextphase);
            }
#endif

        }

        if (s->bitwidth == IN_PCM_16BIT_RAW)
        {
            if (s->isForec2Out32bit)
            {
                if (s->histbuf32)
                {
                    HI_FREE(HI_ID_SRC, s->histbuf32);
                }
            }
            else
            {
                if (s->histbuf)
                {
                    HI_FREE(HI_ID_SRC, s->histbuf);
                }
            }
        }
        else if ((s->bitwidth == IN_PCM_24BIT_RAW)
                 || (s->bitwidth == IN_PCM_20BIT_RAW)
                 || (s->bitwidth == IN_PCM_18BIT_RAW))
        {
            if (s->histbuf32)
            {
                HI_FREE(HI_ID_SRC, s->histbuf32);
            }
        }

        HI_FREE(HI_ID_SRC, s);
    }
}

static int ResamplerStereoProcess16(HResampler inst, int *inbuf, int insamps, short *outbuf, HI_S32 bInterleaved)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *pcmend;
    int   *rwingptr, *lwingptr;
    short *outptr;
    int *xptr, *yptr;
    int acc0, acc1;
    int i;
    //int up = s->up;
    int phase = s->phase;
    int nwing = s->nwing;
    short pcmstep;
    int   *nextphase = s->nextphase;

    ASSERT(!(insamps & 0x1));          /* stereo must be even */
    ASSERT(!((int)s->filter & 0x3));    /* filter addr must be even */
    ASSERT(s->offset == 0);

    /* convert input to float */
    insamps = cvtShortShort(s->pcmbuf, (short*)inbuf, insamps, bInterleaved, STEREO);

    /* restore filter state */
    pcmptr  = ((short*)s->pcmbuf) - STEREO * (nwing - 1);
    pcmend  = pcmptr + insamps;
    pcmptr += s->offset;
    outptr   = (short *)outbuf;
    rwingptr = (int*)s->rwingptr;
    lwingptr = (int*)s->lwingptr;

    /* filter */
    while (pcmptr < pcmend)
    {
        int x, y, cx, cy;

        xptr = (int*)(pcmptr - 2);
        yptr = (int*)(pcmptr + 0);
        acc0 = acc1 = 0;

        for (i = 0; i < nwing; i += 2)
        {
            cx = *rwingptr++;
            cy = *lwingptr++;
            x = *xptr--;
            y = *yptr++;
            acc0 = MLA16BB(acc0, x, cx);
            acc1 = MLA16TB(acc1, x, cx);
            acc0 = MLA16BB(acc0, y, cy);
            acc1 = MLA16TB(acc1, y, cy);

            x = *xptr--;
            y = *yptr++;
            acc0 = MLA16BT(acc0, x, cx);
            acc1 = MLA16TT(acc1, x, cx);
            acc0 = MLA16BT(acc0, y, cy);
            acc1 = MLA16TT(acc1, y, cy);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((acc1 + 0x4000) >> 15));

        phase   = nextphase[phase];
        pcmstep = (phase & 0xff);
        pcmptr += pcmstep;

        lwingptr = (int*)(s->filter + nwing * ((phase >> 8) & 0xfff));
        rwingptr = (int*)(s->filter + nwing * ((phase >> 20) & 0xfff));
        phase >>= 20;
    }

    /* save filter state */
    s->phase    = phase;
    s->offset   = pcmptr - pcmend;
    s->rwingptr = (short*)rwingptr;
    s->lwingptr = (short*)lwingptr;
    memmove(s->histbuf, s->histbuf + insamps, s->nhist * sizeof(short));

    return (outptr - outbuf) / STEREO;
}

static int ResamplerMonoProcess16(HResampler inst, short *inbuf, int insamps, short *outbuf, HI_S32 bInterleaved)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *pcmend;
    int   *rwingptr, *lwingptr;
    short *outptr;
    short *xptr, *yptr;
    int acc0, acc1;
    int i;
    //int up = s->up;
    int phase = s->phase;
    int nwing = s->nwing;
    int pcmstep, outsamps = 0;
    int *nextphase = s->nextphase;

    /* convert input to delayline */
    insamps = cvtShortShort(s->pcmbuf, inbuf, insamps, bInterleaved, MONO);

    /* restore filter state */
    pcmptr  = ((short*)s->pcmbuf) - (nwing - 1);
    pcmend  = pcmptr + insamps;
    pcmptr += s->offset;
    outptr   = (short *)outbuf;
    rwingptr = (int*)s->rwingptr;
    lwingptr = (int*)s->lwingptr;

    /* filter */
    while (pcmptr < pcmend)
    {
        int x, y, cx, cy;
        xptr = (pcmptr - 1);
        yptr = (pcmptr + 0);

        acc0 = acc1 = 0;

        for (i = 0; i < nwing; i += 2)
        {
            cx = *rwingptr++;
            cy = *lwingptr++;
            x = *xptr--;
            y = *yptr++;
            acc0 = MLA16BB(acc0, x, cx);
            acc1 = MLA16BB(acc1, y, cy);

            x = *xptr--;
            y = *yptr++;
            acc0 = MLA16BT(acc0, x, cx);
            acc1 = MLA16BT(acc1, y, cy);
        }

        acc0 += acc1;
        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));
        outsamps++;

        phase = nextphase[phase];
#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
        pcmstep = (phase & 0xff);
#else
        pcmstep = ((phase & 0xff) >> 1);     /* use same table of stereo */
#endif
        pcmptr  += pcmstep;
        lwingptr = (int*)(s->filter + nwing * ((phase >> 8) & 0xfff));
        rwingptr = (int*)(s->filter + nwing * ((phase >> 20) & 0xfff));
        phase >>= 20;
    }

    /* save filter state */
    s->phase    = phase;
    s->offset   = pcmptr - pcmend;
    s->rwingptr = (short*)rwingptr;
    s->lwingptr = (short*)lwingptr;
    memmove(s->histbuf, s->histbuf + insamps, s->nhist * sizeof(short));

    return (outptr - outbuf);
}

//#if defined (WIN32)
#if 1

static void x5_filter_nobypass(int *pcmptr32, int *pcmend32, int nwing, int **prwingptr, int **plwingptr,
                               int *inbuf, int **poutptr, short *filter, int *nextphase, int *pphase,
                               int insamps, int bitwidth)
{
    int *xptr, *yptr;
    int i;
    int acc0, acc1;
    int pcmstep;
    int *rwingptr = *prwingptr;
    int *lwingptr = *plwingptr;
    int *outptr = *poutptr;
    int phase = *pphase;

    /* filter */
    while (pcmptr32 < pcmend32)
    {
        int x, y, cx, cy;
        xptr = (int*)(pcmptr32 - 1);
        yptr = (int*)(pcmptr32 + 0);

        acc0 = acc1 = 0;
        for (i = 0; i < nwing; i += 2)
        {
            cx = *rwingptr++;
            cy = *lwingptr++;
            x = *xptr--;
            y = *yptr++;
            acc1 = MLA32_16B(acc1, x, cx);
            acc0 = MLA32_16B(acc0, y, cy);

            x = *xptr--;
            y = *yptr++;
            acc0 = MLA32_16B(acc0, x, cx);
            acc1 = MLA32_16B(acc1, y, cy);

            x = *xptr--;
            y = *yptr++;
            acc1 = MLA32_16T(acc1, x, cx);
            acc0 = MLA32_16T(acc0, y, cy);

            x = *xptr--;
            y = *yptr++;
            acc0 = MLA32_16T(acc0, x, cx);
            acc1 = MLA32_16T(acc1, y, cy);
        }

        // conever to IN_PCM_24BIT_RAW
        {
            int sign;

            //acc0 >>= 7;
            acc0 = (acc0 + 0x40) >> 7;
            sign = acc0 >> 31;
            if ((acc0 >> 23) != sign)
            {
                acc0 = sign ^ ((1 << 23) - 1);
            }
            else
            {
                acc0 &= 0xffffff;
            }

            *outptr++ = acc0 << 8;

            //acc1 >>= 7;
            acc1 = (acc1 + 0x40) >> 7;
            sign = acc1 >> 31;
            if ((acc1 >> 23) != sign)
            {
                acc1 = sign ^ ((1 << 23) - 1);
            }
            else
            {
                acc1 &= 0xffffff;
            }

            *outptr++ = acc1 << 8;
        }
        phase = nextphase[phase];
        lwingptr = (int*)(filter + nwing * ((phase >> 8) & 0xfff));
        rwingptr = (int*)(filter + nwing * ((phase >> 20) & 0xfff));
        pcmstep   = (phase & 0xff);
        pcmptr32 += pcmstep;
        phase >>= 20;
    }

    *prwingptr = rwingptr;
    *plwingptr = lwingptr;
    *poutptr = outptr;
    *pphase = phase;
}

#else
extern  x5_filter_nobypass(HI_S32 *pcmptr32, HI_S32 *pcmend32, HI_S32 nwing, HI_S32 **prwingptr, HI_S32 **plwingptr,
                           HI_S32 *inbuf, HI_S32 **poutptr, short *filter, HI_S32 *nextphase, HI_S32 *pphase,
                           HI_S32 insamps, HI_S32 bitwidth);
#endif

static HI_S32 ResamplerStereoProcess24(HResampler inst, HI_S32 *inbuf, HI_S32 insamps, HI_S32 *outbuf,
                                       HI_S32 bInterleaved)
{
    state_t *s = (state_t *)inst;
    HI_S32 *pcmptr32, *pcmend32;
    HI_S32 *rwingptr, *lwingptr;
    HI_S32 *outptr;
    HI_S32 phase = s->phase;
    HI_S32 nwing = s->nwing;
    HI_S32   *nextphase = s->nextphase;

    ASSERT(!(insamps & 0x1));          /* stereo must be even */
    ASSERT(!((HI_S32)s->filter & 0x3)); /* filter addr must be even */
    ASSERT(s->offset == 0);

    /* convert input to float */
    insamps = cvt2int(s->pcmbuf32, inbuf, insamps, s->bitwidth, bInterleaved, STEREO);

    /* restore filter state */
    pcmptr32  = s->pcmbuf32 - STEREO * (nwing - 1);
    pcmend32  = pcmptr32 + insamps;
    pcmptr32 += s->offset;

    outptr   = outbuf;
    rwingptr = (HI_S32*)s->rwingptr;
    lwingptr = (HI_S32*)s->lwingptr;

    x5_filter_nobypass(pcmptr32, pcmend32, nwing, &rwingptr, &lwingptr,
                       inbuf, &outptr, s->filter, nextphase, &phase, insamps, s->bitwidth);

    /* save filter state */
    s->phase    = phase;
    s->offset   = 0;
    s->rwingptr = (short*)rwingptr;
    s->lwingptr = (short*)lwingptr;

    memmove(s->histbuf32, s->histbuf32 + insamps, s->nhist * sizeof(HI_S32));

    return (outptr - outbuf) / STEREO;
}

static int ResamplerMonoProcess24(HResampler inst, int *inbuf, int insamps, int *outbuf)
{
    state_t *s = (state_t *)inst;
    int *pcmptr32, *pcmend32;
    int   *rwingptr, *lwingptr;
    int *outptr;
    int *xptr32, *yptr32;
    int acc0, acc1;
    int i;
    int phase = s->phase;
    int nwing = s->nwing;
    int pcmstep, outsamps = 0;
    int *nextphase = s->nextphase;

    /* convert input to delayline */
    insamps = cvt2int(s->pcmbuf32, inbuf, insamps, s->bitwidth, HI_TRUE, MONO);

    pcmptr32  = s->pcmbuf32 - (nwing - 1);
    pcmend32  = pcmptr32 + insamps;
    pcmptr32 += s->offset;

    outptr   = outbuf;
    rwingptr = (int*)s->rwingptr;
    lwingptr = (int*)s->lwingptr;

    {
        /* filter */
        while (pcmptr32 < pcmend32)
        {
            int x, y, cx, cy;
            xptr32 = pcmptr32 - 1;
            yptr32 = pcmptr32 + 0;
            acc0 = acc1 = 0;

            for (i = 0; i < nwing; i += 2)
            {
                cx = *rwingptr++;
                cy = *lwingptr++;
                x = *xptr32--;
                y = *yptr32++;
                acc0 = MLA32_16B(acc0, x, cx);
                acc1 = MLA32_16B(acc1, y, cy);

                x = *xptr32--;
                y = *yptr32++;
                acc0 = MLA32_16T(acc0, x, cx);
                acc1 = MLA32_16T(acc1, y, cy);
            }

            acc0 += acc1;
            if (s->bitwidth == IN_PCM_16BIT_RAW)
            {
                *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));
            }
            else if (s->bitwidth == IN_PCM_18BIT_RAW)
            {
                int sign;

                //acc0 >>= 13;
                acc0 = (acc0 + 0x1000) >> 13;
                sign = acc0 >> 31;
                if ((acc0 >> 17) != sign)
                {
                    acc0 = sign ^ ((1 << 17) - 1);
                }
                else
                {
                    acc0 &= 0x3ffff;
                }

                *outptr++ = acc0;
            }
            else if (s->bitwidth == IN_PCM_20BIT_RAW)
            {
                int sign;

                //acc0 >>= 13;
                acc0 = (acc0 + 0x400) >> 11;
                sign = acc0 >> 31;
                if ((acc0 >> 19) != sign)
                {
                    acc0 = sign ^ ((1 << 19) - 1);
                }
                else
                {
                    acc0 &= 0xfffff;
                }

                *outptr++ = acc0;
            }
            else
            {
                int sign;
                acc0 += 0x40;
                acc0 >>= 7;
                sign = acc0 >> 31;
                if ((acc0 >> 23) != sign)
                {
                    acc0 = sign ^ ((1 << 23) - 1);
                }
                else
                {
                    acc0 &= 0xffffff;
                }

                *outptr++ = acc0;
            }

            outsamps++;

            phase = nextphase[phase];
#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
            pcmstep = (phase & 0xff);
#else
            pcmstep = ((phase & 0xff) >> 1);     /* use same table of stereo */
#endif
            pcmptr32 += pcmstep;

            lwingptr = (int*)(s->filter + nwing * ((phase >> 8) & 0xfff));
            rwingptr = (int*)(s->filter + nwing * ((phase >> 20) & 0xfff));
            phase >>= 20;
        }
    }

    /* save filter state */
    s->phase    = phase;
    s->offset   = pcmptr32 - pcmend32;
    s->rwingptr = (short*)rwingptr;
    s->lwingptr = (short*)lwingptr;
    memmove(s->histbuf32, s->histbuf32 + insamps, s->nhist * sizeof(int));

    return (outptr - outbuf);
}

#ifndef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
HI_S32 ResampleConfig(state_t *s, HI_S32 inrate, HI_S32 outrate)
{
    int divisor, up, dn, nhist;
    int i, nwing, ntaps;
    SRC_Info *srcinfoptr = resampler_info;
    int size = sizeof(resampler_info) / sizeof(SRC_Info);

    /* reduce to smallest fraction */
    divisor = gcd(inrate, outrate);
    up = outrate / divisor;
    dn = inrate / divisor;

    if (up > 1280)
    {
        return -1;
    }               /* supports standard rates to 96000Hz */

    if (s->nchans * ((dn + up - 1) / up) > 255)
    {
        return -1;
    }               /* pcmstep exceeds short */

    for (i = 0; i < size; i++)
    {
 #ifdef NEW_RATE_SUPPORT
        if ((srcinfoptr->inrate == inrate) && (srcinfoptr->outrate == outrate))
        {
            break;
        }
        else if ((srcinfoptr->inrate == inrate / 2) && (srcinfoptr->outrate == outrate / 2))
        {
            break;
        }
        else if ((srcinfoptr->inrate == inrate / 4) && (srcinfoptr->outrate == outrate / 4))
        {
            break;
        }

 #else
        if ((srcinfoptr->inrate == inrate) && (srcinfoptr->outrate == outrate))
        {
            break;
        }
 #endif


        srcinfoptr++;
    }

    if (i == size)
    {
        return -1;
    }

    nwing = srcinfoptr->nwing;
    ntaps = nwing * 2;              /* update ntaps */
    nhist = s->nchans * ntaps;

    s->filter = (short*)srcinfoptr->filter;
    s->nextphase = (int*)srcinfoptr->nextphase;
    s->isCloned = 0;

    if (s->bitwidth == IN_PCM_16BIT_RAW)
    {
        s->pcmbuf32 = s->histbuf32 + nhist;
    }
    else if ((s->bitwidth == IN_PCM_24BIT_RAW)
             || (s->bitwidth == IN_PCM_20BIT_RAW)
             || (s->bitwidth == IN_PCM_18BIT_RAW))
    {
        s->pcmbuf32 = s->histbuf32 + nhist;
    }

    /* filter init */
    s->inrate  = inrate;
    s->outrate = outrate;
    s->up = up;
    s->dn = dn;
    s->nwing    = nwing;
    s->nhist    = nhist;
    s->phase    = 0;
    s->offset   = 0;
    s->rwingptr = (short*)(&s->filter[s->phase]);
    s->lwingptr = (short*)(&s->filter[nwing * (up - 1 - s->phase)]);
    return 0;
}

#endif

static HI_S32 ResamplerProcessFrame(HResampler inst,
                                 HI_S32 *   inbuf,
                                 HI_S32 *   pOutpcm,
                                 HI_S32     insamps,
                                 HI_S32     inrate,
                                 HI_S32     outrate,
                                 HI_S32     bInterleaved)
{
    state_t *s = (state_t *)inst;
    int nsamps;
    int maxout;

    if (s == HI_NULL)
    {
        return HI_ERR_RESAMPLE_HANDLE;
    }

    if (inbuf == HI_NULL)
    {
        return HI_ERR_RESAMPLE_PCMBUF;
    }

    if ((insamps > MAXFRAMESIZE) || ((insamps & 0x1) != 0))
    {
        return HI_ERR_RESAMPLE_SAMPLE_NUMBER;
    }

    if (pOutpcm == HI_NULL)
    {
        return HI_ERR_RESAMPLE_PCMBUF;
    }

    if ((s->inrate != inrate) || (s->outrate != outrate))
    {
#ifndef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
        ResampleConfig(s, inrate, outrate);
#endif
    }

    maxout = NEW_ResamplerGetMaxOutputNum(inst, insamps);
    if (maxout < 0)
    {
        if (maxout == HI_ERR_RESAMPLE_HANDLE)
        {
            return HI_ERR_RESAMPLE_HANDLE;
        }
        else if (maxout == HI_ERR_RESAMPLE_SAMPLE_NUMBER)
        {
            return HI_ERR_RESAMPLE_SAMPLE_NUMBER;
        }
        else
        {
            return HI_ERR_UNKNOWN;
        }
    }

    if (s->nchans == 2)
    {
        if (s->bitwidth == IN_PCM_16BIT_RAW)
        {
            if (s->isForec2Out32bit == 0)
            {
                nsamps = ResamplerStereoProcess16(inst, inbuf, insamps, (short *)pOutpcm, bInterleaved);
            }
            else
            {
                nsamps = ResamplerStereoProcess24(inst, inbuf, insamps, pOutpcm, bInterleaved);
            }
        }
        else if ((s->bitwidth == IN_PCM_24BIT_RAW)
                 || (s->bitwidth == IN_PCM_20BIT_RAW)
                 || (s->bitwidth == IN_PCM_18BIT_RAW))
        {
            nsamps = ResamplerStereoProcess24(inst, inbuf, insamps, pOutpcm, bInterleaved);
        }
        else
        {
            return HI_ERR_PCM_FORMAT;
        }
    }
    else if (s->nchans == 1)
    {
        if (s->bitwidth == IN_PCM_16BIT_RAW)
        {
            if (s->isForec2Out32bit == 0)
            {
                nsamps = ResamplerMonoProcess16(inst, (short *)inbuf, insamps, (short *)pOutpcm, bInterleaved);
            }
            else
            {
                nsamps = ResamplerMonoProcess24(inst, inbuf, insamps, pOutpcm);
            }
        }
        else if ((s->bitwidth == IN_PCM_24BIT_RAW)
                 || (s->bitwidth == IN_PCM_20BIT_RAW)
                 || (s->bitwidth == IN_PCM_18BIT_RAW))
        {
            nsamps = ResamplerMonoProcess24(inst, inbuf, insamps, pOutpcm);
        }
        else
        {
            return HI_ERR_PCM_FORMAT;
        }
    }
    else
    {
        return HI_ERR_PCM_CHANNEL;
    }

    return nsamps;
}

static HI_S32 ResamplerGetMaxOutputNum(HResampler inst, HI_S32 insamps)
{
    state_t *s = (state_t *)inst;
    HI_S32 inframes, outframes, outsamps;

    if (s == HI_NULL)
    {
        return HI_ERR_RESAMPLE_HANDLE;
    }

    if ((insamps > MAXFRAMESIZE) || ((insamps & 0x1) != 0))
    {
        return HI_ERR_RESAMPLE_SAMPLE_NUMBER;
    }

    inframes = (insamps + (s->nchans - 1));

    //outframes = (int) ((double)inframes * s->outrate / s->inrate) + 1;  /* add 1 avoid 0.9 */
#if 1
    outframes = ((HI_U32)inframes * s->outrate / s->inrate) + 1; // use int to calculate instead float
#else
    outframes = (inframes * s->outrate * 17 / s->inrate) / 16; // use int to calculate instead float
#endif
    outsamps = outframes;

    return outsamps;
}

static HResampler ResamplerCreate(HI_S32 inrate, HI_S32 outrate, HI_S32 chans, HI_S32 SampleFormat,
                               HI_S32 isForec2Out32bit)
{
#ifndef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
    int dcgain = 1;
    int atten;
    int passband;
    int stopband;
#else
    float dcgain = DEF_DCGAIN;
    float atten;
    float passband;
    float stopband;
#endif

#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
    int quality = QUALITY_MEDIUM;
#else
    int quality = QUALITY_GOOD; // enforce quality = QUALITY_GOOD, even if quality is actually ENA_QUALITY_MEDIUM
#endif

    //#ifndef CONFIG_RES_GENERATE_TRIGTABS_FLOAT

    if (inrate >= outrate)
    {
        return HI_NULL;
    }

    if ((chans != MONO) && (chans != STEREO))
    {
        return HI_NULL;
    }

    if ((SampleFormat != IN_PCM_16BIT_RAW) && (SampleFormat != IN_PCM_24BIT_RAW)
        && (SampleFormat != IN_PCM_20BIT_RAW) && (SampleFormat != IN_PCM_18BIT_RAW))
    {
        return HI_NULL;
    }

    switch (quality)
    {
#ifdef ENA_QUALITY_LOW
    case QUALITY_LOW:
        passband = 0.50f;  // 8 tap, 7Mhz ARM9E
        stopband = 1.50f;
        atten = 60.0f;
        break;
#endif

#ifdef CONFIG_RES_GENERATE_TRIGTABS_FLOAT
#ifdef ENA_QUALITY_MEDIUM
    case QUALITY_MEDIUM:
        passband = 0.70f;  // 16 tap, 10Mhz ARM9E
        stopband = 1.27f;
        atten = 70.0f;
        break;
#endif
#endif

#ifdef ENA_QUALITY_GOOD
    case QUALITY_GOOD:
        passband = 0.77f;  // 32 tap, 15Mhz ARM9E
        stopband = 1.09f;
        atten = 80.0f;
        break;
#endif

        /*
            case qualityHigh:
                passband = 0.88f ; // 96 tap
                stopband = 1.00f ;
                atten    = 90.0f ;
                break ;
            case qualityHyper:
                passband = 0.904f ; // 128 tap
                stopband = 1.000f ;
                atten    = 96.00f ;
                break ;
         */
    default:
        return HI_NULL;
    }

    return RAInitResamplerRat(inrate, outrate, chans, atten, passband, stopband, dcgain, quality, SampleFormat,
                              isForec2Out32bit);
}



static HI_S32 ResamplerCheck(HResampler inst, HI_S32 inrate, HI_S32 outrate, HI_S32 chans, HI_S32 SampleFormat)
{
    state_t *s = (state_t *)inst;
#if 0
    if (s == HI_NULL)
    {
        return HI_FALSE;
    }

    if ((chans != MONO) && (chans != STEREO))
    {
        return HI_FALSE;
    }

    if ((SampleFormat != IN_PCM_24BIT_RAW) && (SampleFormat != IN_PCM_24BIT_RAW)
        && (SampleFormat != IN_PCM_20BIT_RAW) && (SampleFormat != IN_PCM_18BIT_RAW))
    {
        return HI_FALSE;
    }
#else
    if(s->inrate!=inrate)
    	return HI_TRUE;
    if(s->outrate!=outrate)
    	return HI_TRUE;
    if(s->nchans!=chans)
    	return HI_TRUE;
    if(s->bitwidth!=SampleFormat)
    	return HI_TRUE;
    
#endif
    return HI_FALSE;
}



HResampler NEW_ResamplerCreate(HI_S32 inrate, HI_S32 outrate, HI_S32 chans, HI_S32 SampleFormat,
                               HI_S32 isForec2Out32bit)
{
	HResampler inst;
	inst =  ResamplerCreate(inrate, outrate, chans, SampleFormat,isForec2Out32bit);
	if(inst)
	{
		return inst;
	}
	else
	{
		state_t *s;
		SRC_Linear *pLinearSrc;
		LinearSRC_Create(&pLinearSrc,inrate, outrate, SampleFormat,chans);
		if(pLinearSrc)
		{
			s = (state_t *) HI_MALLOC(HI_ID_SRC, sizeof(state_t));
			if(!s)
			{
				LinearSRC_Destroy(pLinearSrc);
				return NULL;
			}				
			s->isLinearMode = 1;
            s->isResampler2X = 0;
			s->pLinearSrc = pLinearSrc;		
			return (void *) s;
		}
	}
	return NULL;
}

void NEW_ResamplerFree(HResampler inst)
{
    if (inst != HI_NULL)
    {
		state_t *s = (state_t *)inst;
		if(0==s->isLinearMode)
		{
			ResamplerFree(s);
		}
		else
		{
			LinearSRC_Destroy(s->pLinearSrc);
			HI_FREE(HI_ID_SRC, s);
		}
    }
}

void NEW_ResampleFlush(HResampler inst)
{

    if (inst != HI_NULL)
    {
		state_t *s = (state_t *)inst;
		if(0==s->isLinearMode)
		{
			ResampleFlush(inst);
		}
		else
		{

		}
    }
	
}


HI_S32 NEW_ResamplerCheck(HResampler inst, HI_S32 inrate, HI_S32 outrate, HI_S32 chans, HI_S32 SampleFormat)
{
    if (inst != HI_NULL)
    {
		state_t *s = (state_t *)inst;
		if(0==s->isLinearMode)
		{
			return ResamplerCheck(inst,inrate, outrate, chans, SampleFormat);
		}
		else
		{
			return LinearSRC_CheckUpdate(s->pLinearSrc, (HI_S32)inrate, (HI_S32)outrate,
								   SampleFormat, (HI_S32)chans);
		}
    }
	return HI_ERR_RESAMPLE_HANDLE;
}

HI_S32 NEW_ResamplerGetMaxOutputNum(HResampler inst, HI_S32 insamps)
{
    if (inst != HI_NULL)
    {
		state_t *s = (state_t *)inst;
		if(0==s->isLinearMode)
		{
			return ResamplerGetMaxOutputNum(inst,insamps);
		}
		else
		{
			return LinearSRC_GetMaxOutputNum(s->pLinearSrc,insamps);
		}
    }
	return HI_ERR_RESAMPLE_HANDLE;
}

HI_S32 NEW_ResamplerProcessFrame(HResampler inst,
                                 HI_S32 *   inbuf,
                                 HI_S32 *   pOutpcm,
                                 HI_S32     insamps,
                                 HI_S32     inrate,
                                 HI_S32     outrate,
                                 HI_S32     bInterleaved)
{
    if (inst != HI_NULL)
    {
		state_t *s = (state_t *)inst;
		if(0==s->isLinearMode)
		{
			return ResamplerProcessFrame(inst,inbuf,pOutpcm,insamps,inrate,outrate,bInterleaved);
		}
		else
		{
			return LinearSRC_ProcessFrame(s->pLinearSrc,inbuf,pOutpcm,insamps);
		}
    }
	return HI_ERR_RESAMPLE_HANDLE;
}

#if 0
static int __init resample_init(void)
{
    HI_RESAMPLE_PRINT(HI_ID_SRC, "--------resample init----------\n");

    return 0;
}

static void __exit resample_exit(void)
{
    HI_RESAMPLE_PRINT(HI_ID_SRC, "--------resample exit----------\n");
}

module_init(resample_init);
module_exit(resample_exit);

MODULE_AUTHOR("Hisilicon AUDIO Team!");
MODULE_DESCRIPTION("Hisilicon Device driver for X5");
MODULE_LICENSE("Copyright(c)");

EXPORT_SYMBOL(ResampleFlush);
EXPORT_SYMBOL(ResamplerProcessFrame);
EXPORT_SYMBOL(ResamplerCreate);
EXPORT_SYMBOL(ResamplerFree);
#endif
