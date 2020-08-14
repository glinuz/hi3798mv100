/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCXgrHIPlGWwjY48PA756GbU69BErqnMH8NRXsO
3MWeW/X5BLFU+LTX7vigFYVO2XSSeD51Er3BM+xujhoL9w/BqfYZYkWmXANM5ADXsnegEM7S
LPgRxoHtG/JRQBpA6vsH7n3swPIb6oSWHg/oMMgnzAPTZEx1+U4F6Os6Df3uog==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/




































#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assembly.h"
#include "resampler2X.h"
#include "resampler_statname.h"
#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "hi_debug.h"
/* leave a trace of the C source version in the object code */
static const char VERSION[] = "$Revision: 1.4 $";

/* filter state */

static int cvtShortShort(void *d, const void *s, int n)
{
    const short* src = (const short *)s;
    const short* srcEnd = src + n;
    short* dst = (short *)d;

    while (src != srcEnd)
    {
        *dst = *src;
        src += 1;
        dst += 1;
    }

    return dst - (short *)d;
}

void Resample2X_Flush(HResampler inst)
{
    state_t *s = (state_t *)inst;

    if (s != NULL)
    {
        memset(s->histbuf, 0, (NBLOCK + s->nhist) * sizeof(short));
        s->pcmbuf = s->histbuf + s->nhist;
    }
}

void Resampler2X_Free(HResampler inst)
{
    state_t *s = (state_t *)inst;

    if (s != NULL)
    {
        if (s->histbuf)
        {
            HI_FREE(HI_ID_SRC, s->histbuf);
        }

        HI_FREE(HI_ID_SRC, s);
    }
}

int Resampler2X_GetMaxOutputNum(HResampler inst, int insamps)
{
    state_t *s = (state_t *)inst;
    int inframes, outframes, outsamps;

    inframes  = (insamps + (s->nchans - 1));
    outframes = (int) ((double)inframes * s->outrate / s->inrate) + 1;  /* add 1 avoid 0.9 */
    outsamps = outframes;

    return outsamps;
}

int Resampler2X_GetMinInputNum(HResampler inst, int outsamps)
{
    state_t *s = (state_t *)inst;
    int inframes, outframes, insamps;

    outframes = (outsamps + (s->nchans - 1));
    inframes = (int) ((double)outframes * s->inrate / s->outrate);
    insamps = inframes;

    return insamps;
}

static short coef1x2_mid[] =
{   -14,      65, -201,   492, -1071, 2228, -5109, 29057,
    9881,  -3872, 2006, -1053,   509, -211,    65,   -10,
    -10,      65, -211,   509, -1053, 2006, -3872,	9881,
    29057, -5109, 2228, -1071,   492, -201,    65,   -14, };

static short coef1x2_low[] =
{  -196,   1191, -4406, 29167,
   8867,  -2341,   540,   -41,
   -41,		540, -2341,  8867,
   29167, -4406,  1191,  -196, };

static short coef2x1_mid[] =
{    -5,      -7,    32,	32, -106, -101,  254,  246, -527, -536, 1003, 1114, -1936, -2555, 4940, 14528,
     14528, 4940, -2555, -1936, 1114, 1003, -536, -527,  246,  254, -101, -106,    32,    32,	-7,    -5, };

static short coef2x1_low[] =
{   -21,	-98,   270,   595, -1171, -2203, 4433, 14583,
    14583, 4433, -2203, -1171,   595,	270,  -98,   -21, };

static short coef1x2[] =
{
    2,       -10,	25,   -56,	104, -177,	267, -368,	458, -509,	471, -279,	-194, 1231, -3984, 27810,
    11221, -5245, 3333, -2251, 1509, -972,	584, -318,	146,  -48,   -2,   17,   -19,	12,    -6,     1,
    1,        -6,	12,   -19,   17,   -2,	-48,  146, -318,  584, -972, 1509, -2251, 3333, -5245, 11221,
    27810, -3984, 1231,  -194, -279,  471, -509,  458, -368,  267, -177,  104,   -56,	25,   -10,     2,
};

static short coef2x1[] =
{
    0,        1,	-3,    -5,   6,   12, -10,   -28,    8,  52,  -1,  -89,  -24, 133,	73, -184, -159, 229, 292, -255,
    -486, 235, 754, -140, -1126, -97, 1666, 615, -2623, -1992, 5610, 13905,
    13905, 5610, -1992, -2623, 615, 1666, -97, -1126, -140, 754, 235, -486, -255, 292, 229, -159, -184,  73, 133,  -24,
    -89,  -1,  52,    8,   -28, -10,   12,   6,    -5,    -3,    1,		0,
};

static short coef4x1[] =
{
    -14,   -35,  -29,  49,	193,   295,  179, -256, -864, -1197, -690, 980, 3570, 6250, 7958,
    7958, 6250, 3570, 980, -690, -1197, -864, -256,  179,	295,  193,	49,  -29,  -35,  -14,
};

static short coef1x4[] =
{
    -169,  818, -2847, 31847,  3996, -1129,  267,  -22,
    -238, 1404, -5028, 25086, 14418, -3707,  970, -124,
    -124,  970, -3707, 14418, 25086, -5028, 1404, -238,
    -22,   267, -1129,	3996, 31847, -2847,  818, -169,
};

static short coef6x1[] =
{
    -3,    -12,    -27,    -41,    -43,    -22,     29,    105, 
    185,    236,    217,     99,   -123,   -411,   -687,   -839, 
   -753,   -341,    425,   1490,   2718,   3914,   4865,   5392, 
 
   5392,   4865,   3914,   2718,   1490,    425,   -341,   -753, 
   -839,   -687,   -411,   -123,     99,    217,    236,    185, 
    105,     29,    -22,    -43,    -41,    -27,    -12,     -3, 
};

static short coef1x6[] =
{
  -131,    595,  -2042,  32357, 
   2554,   -735,    175,    -15, 
   -257,   1307,  -4517,  29194, 
   8944,  -2465,    631,    -72, 
   -243,   1420,  -5034,  23487, 
  16312,  -4118,   1115,   -161, 
   -161,   1115,  -4118,  16312, 
  23487,  -5034,   1420,   -243, 
    -72,    631,  -2465,   8944, 
  29194,  -4517,   1307,   -257, 
    -15,    175,   -735,   2554, 
  32357,  -2042,    595,   -131, 
};

static int ResamplerMono1X1Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    memcpy(outbuf, inbuf, insamps * sizeof(short));
    return insamps;
}

static int ResamplerMono1X2Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n;

    /* Sure CONFIG_RES_GENERATE_TRIGTABS_FLOAT is enable
       Thereis no coef2x1 or coef2x1 info at resampler_info;

     */
    if (s->nwing == 16)
    {
        coeftab = (int*)coef1x2;
    }
    else if (s->nwing == 8)
    {
        coeftab = (int*)coef1x2_mid;
    }
    else
    {
        coeftab = (int*)coef1x2_low;
    }

    /* convert input to delayline */
    insamps = cvtShortShort(s->pcmbuf, inbuf, insamps);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n++)
    {
        int x, cx, acc0;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));
        pcmptr++;
    }

    memmove(s->histbuf, s->histbuf + insamps, s->nhist * sizeof(short));
    return (insamps * 2);
}

static int ResamplerMono1X4Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n;

    /* Sure CONFIG_RES_GENERATE_TRIGTABS_FLOAT is enable
       Thereis no coef2x1 or coef2x1 info at resampler_info;

     */
    coeftab = (int*)coef1x4;

    /* convert input to delayline */
    insamps = cvtShortShort(s->pcmbuf, inbuf, insamps);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n++)
    {
        int x, cx, acc0;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));
        pcmptr++;
    }

    memmove(s->histbuf, s->histbuf + insamps, s->nhist * sizeof(short));
    return (insamps * 4);
}

static int ResamplerMono1X6Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n;

    /* Sure CONFIG_RES_GENERATE_TRIGTABS_FLOAT is enable
       Thereis no coef2x1 or coef2x1 info at resampler_info;

     */
    coeftab = (int*)coef1x6;

    /* convert input to delayline */
    insamps = cvtShortShort(s->pcmbuf, inbuf, insamps);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n++)
    {
        int x, cx, acc0;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));
        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));
        pcmptr++;
    }

    memmove(s->histbuf, s->histbuf + insamps, s->nhist * sizeof(short));
    return (insamps * 6);
}

static int ResamplerMono2X1Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n;

    /* Sure CONFIG_RES_GENERATE_TRIGTABS_FLOAT is enable
       Thereis no coef2x1 or coef2x1 info at resampler_info;

     */
    if (s->nwing == 32)
    {
        coeftab = (int*)coef2x1;
    }
    else if (s->nwing == 16)
    {
        coeftab = (int*)coef2x1_mid;
    }
    else
    {
        coeftab = (int*)coef2x1_low;
    }

    /* convert input to delayline */
    insamps = cvtShortShort(s->pcmbuf, inbuf, insamps);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n += 2)
    {
        int x, cx, acc0;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        pcmptr += 2;
    }

    memmove(s->histbuf, s->histbuf + insamps, s->nhist * sizeof(short));
    return (insamps / 2);
}

static int ResamplerMono4X1Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n;

    /* Sure CONFIG_RES_GENERATE_TRIGTABS_FLOAT is enable
       Thereis no coef2x1 or coef2x1 info at resampler_info;

     */
    coeftab = (int*)coef4x1;

    /* convert input to delayline */
    insamps = cvtShortShort(s->pcmbuf, inbuf, insamps);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n += 4)
    {
        int x, cx, acc0;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 4)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
            x = *xptr++;
            cx = *cptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        pcmptr += 4;
    }

    memmove(s->histbuf, s->histbuf + insamps, s->nhist * sizeof(short));
    return (insamps / 4);
}

static int ResamplerMono6X1Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n;

    /* Sure CONFIG_RES_GENERATE_TRIGTABS_FLOAT is enable
       Thereis no coef2x1 or coef2x1 info at resampler_info;

     */
    coeftab = (int*)coef6x1;

    /* convert input to delayline */
    insamps = cvtShortShort(s->pcmbuf, inbuf, insamps);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n += 6)
    {
        int x, cx, acc0;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        acc0 = 0;
        for (i = 0; i < 2 * s->nwing; i += 6)
        {
            cx = *cptr++;
            x = *xptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
            x = *xptr++;
            cx = *cptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
            x = *xptr++;
            cx = *cptr++;
            acc0 = MLA16BB(acc0, x, cx);
            x = *xptr++;
            acc0 = MLA16BT(acc0, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((acc0 + 0x4000) >> 15));

        pcmptr += 6;
    }

    memmove(s->histbuf, s->histbuf + insamps, s->nhist * sizeof(short));
    return (insamps / 6);
}

static int ResamplerStereo1X1Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    memcpy(outbuf, inbuf, insamps * sizeof(short) * 2);
    return insamps;
}

static int ResamplerStereo1X2Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n, num;
    
    if (s->nwing == 16)
    {
        coeftab = (int*)coef1x2;
    }
    else if (s->nwing == 8)
    {
        coeftab = (int*)coef1x2_mid;
    }
    else
    {
        coeftab = (int*)coef1x2_low;
    }

    /* convert input to delayline */
    num = cvtShortShort(s->pcmbuf, inbuf, insamps * 2);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n++)
    {
        int x, cx, accL, accR;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));
        pcmptr++;
        pcmptr++;
    }

    memmove(s->histbuf, s->histbuf + num, s->nhist * sizeof(short));
    return (outptr - outbuf) / STEREO;
}

static int ResamplerStereo1X4Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n, num;

    /* Sure CONFIG_RES_GENERATE_TRIGTABS_FLOAT is enable
       Thereis no coef2x1 or coef2x1 info at resampler_info;

     */
    coeftab = (int*)coef1x4;

    /* convert input to delayline */
    num = cvtShortShort(s->pcmbuf, inbuf, insamps * 2);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n++)
    {
        int x, cx, accL, accR;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
             x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));
        pcmptr++;
        pcmptr++;
    }

    memmove(s->histbuf, s->histbuf + num, s->nhist * sizeof(short));
    return (outptr - outbuf) / STEREO;
}

static int ResamplerStereo1X6Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n, num;

    /* Sure CONFIG_RES_GENERATE_TRIGTABS_FLOAT is enable
       Thereis no coef2x1 or coef2x1 info at resampler_info;

     */
    coeftab = (int*)coef1x6;

    /* convert input to delayline */
    num = cvtShortShort(s->pcmbuf, inbuf, insamps * 2);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n++)
    {
        int x, cx, accL, accR;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));
        
        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));
        pcmptr++;
        pcmptr++;
    }

    memmove(s->histbuf, s->histbuf + num, s->nhist * sizeof(short));
    return (outptr - outbuf) / STEREO;
}

static int ResamplerStereo2X1Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n, num;

    if((insamps % 2) != 0)
        return 0;

    if (s->nwing == 32)
    {
        coeftab = (int*)coef2x1;
    }
    else if (s->nwing == 16)
    {
        coeftab = (int*)coef2x1_mid;
    }
    else
    {
        coeftab = (int*)coef2x1_low;
    }

    /* convert input to delayline */
    num = cvtShortShort(s->pcmbuf, inbuf, insamps * 2);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n += 2)
    {
        int x, cx, accL, accR;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 2)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        pcmptr += 2 * STEREO;
    }

    memmove(s->histbuf, s->histbuf + num, s->nhist * sizeof(short));
    return (outptr - outbuf) / STEREO;
}

static int ResamplerStereo4X1Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n, num;

    if((insamps % 2) != 0)
        return 0;

    coeftab = (int*)coef4x1;

    /* convert input to delayline */
    num = cvtShortShort(s->pcmbuf, inbuf, insamps * 2);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n += 4)
    {
        int x, cx, accL, accR;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 4)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
            x = *xptr++;
            cx = *cptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);    
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        pcmptr += 4 * STEREO;
    }

    memmove(s->histbuf, s->histbuf + num, s->nhist * sizeof(short));
    return (outptr - outbuf) / STEREO;
}

static int ResamplerStereo6X1Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;
    short *pcmptr, *outptr;
    int *cptr, *coeftab;
    int i, n, num;

    if((insamps % 2) != 0)
        return 0;

    coeftab = (int*)coef6x1;

    /* convert input to delayline */
    num = cvtShortShort(s->pcmbuf, inbuf, insamps * 2);
    pcmptr = s->histbuf;
    outptr = outbuf;

    /* filter */
    for (n = 0; n < insamps; n += 6)
    {
        int x, cx, accL, accR;
        short *xptr;

        cptr = (int*)coeftab;
        xptr = pcmptr;
        accL = 0;
        accR = 0;
        for (i = 0; i < 2 * s->nwing; i += 6)
        {
            cx = *cptr++;
            x = *xptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
            x = *xptr++;
            cx = *cptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
            x = *xptr++;
            cx = *cptr++;
            accL = MLA16BB(accL, x, cx);
            x = *xptr++;
            accR = MLA16BB(accR, x, cx);
            x = *xptr++;
            accL = MLA16BT(accL, x, cx);
            x = *xptr++;
            accR = MLA16BT(accR, x, cx);
        }

        *outptr++ = CLIPTOSHORT((int)((accL + 0x4000) >> 15));
        *outptr++ = CLIPTOSHORT((int)((accR + 0x4000) >> 15));

        pcmptr += 6 * STEREO;
    }

    memmove(s->histbuf, s->histbuf + num, s->nhist * sizeof(short));
    return (outptr - outbuf) / STEREO;
}

typedef int (*resample_proc)(HResampler inst, short *inbuf, int insamps, short *outbuf);

static const resample_proc resample_mono_proc_list[] = {
    ResamplerMono1X1Process,    //RESAMPLE_1X1
    ResamplerMono1X2Process,    //RESAMPLE_1X2
    ResamplerMono2X1Process,    //RESAMPLE_2X1
    ResamplerMono1X4Process,    //RESAMPLE_1X4
    ResamplerMono4X1Process,    //RESAMPLE_4X1
    ResamplerMono1X6Process,    //RESAMPLE_1X6
    ResamplerMono6X1Process,    //RESAMPLE_6X1
    NULL,      //RESAMPLE_1X3
    NULL,      //RESAMPLE_3X1
};

static const resample_proc resample_stero_proc_list[] = {
    ResamplerStereo1X1Process,    //RESAMPLE_1X1
    ResamplerStereo1X2Process,    //RESAMPLE_1X2
    ResamplerStereo2X1Process,    //RESAMPLE_2X1
    ResamplerStereo1X4Process,    //RESAMPLE_1X4
    ResamplerStereo4X1Process,    //RESAMPLE_4X1
    ResamplerStereo1X6Process,    //RESAMPLE_1X6
    ResamplerStereo6X1Process,    //RESAMPLE_6X1
    NULL,    //RESAMPLE_1X3
    NULL,    //RESAMPLE_3X1
};

static const int resampler_nwing[] = {
    0,      //RESAMPLE_1X1
    16,     //RESAMPLE_1X2
    16,     //RESAMPLE_2X1
    4,      //RESAMPLE_1X4
    15,     //RESAMPLE_4X1
    4,      //RESAMPLE_1X6
    24,     //RESAMPLE_6X1
    0,      //RESAMPLE_1X3
    0,      //RESAMPLE_3X1
};

static HResampler Resampler2X_Init(int InRate, int OutRate, RESAMPLER_CONVERT_FRAC_E enConvertFrac,RESAMPLER_CHAN enChan)
{
    state_t *s;
    int nhist, nwing, ntaps;

    if ((enChan < 1) || (enChan > 2))
    {
        return NULL;
    }

    {
        nwing = resampler_nwing[enConvertFrac];
        ntaps = nwing * 2;          /* update ntaps */
        nhist = enChan * ntaps;
        s = (state_t *) HI_MALLOC(HI_ID_SRC, sizeof(state_t));
        if (!s)
        {
            return NULL;
        }

        s->histbuf = (short *) HI_CALLOC(HI_ID_SRC, ((NBLOCK * enChan) + nhist), sizeof(short));
        if (!(s->histbuf))
        {
            HI_FREE(HI_ID_SRC, s);
            return NULL;
        }

        s->pcmbuf = s->histbuf + nhist;
    }

    /* filter init */
    s->inrate  = InRate;
    s->outrate = OutRate;
    s->nchans = enChan;
    s->nwing = nwing;
    s->nhist = nhist;
    s->convertfrac = enConvertFrac;
    s->isResampler2X = 1;

    Resample2X_Flush(s);
    return (void *) s;
}

    
HResampler Resampler2X_Create(int inrate, int outrate, RESAMPLER_CONVERT_FRAC_E convertfrac, int chans)
{
    
//    printf("convert, i [%d], num [%d], channels [%d], in [%d], out[%d]\n",i,num,chans,inrate,outrate);

    return Resampler2X_Init(inrate, outrate, convertfrac, chans);
}

int  Resampler2X_Process(HResampler inst, short *inbuf, int insamps, short *outbuf)
{
    state_t *s = (state_t *)inst;

    if (!inst || !inbuf || !outbuf)
    {
        return 0;
    }

    if (insamps > MAX_SAMPLE_PER_FRAME)
    {
        HI_RESAMPLE_PRINT(HI_ID_SRC, "%s() %d, invalid para, insamps [%d].\n",__FUNCTION__,__LINE__,insamps);
        return 0;
    }
	
    if(s->convertfrac >= RESAMPLE_UNSUPPORT)
    {
        HI_RESAMPLE_PRINT(HI_ID_SRC, "%s() %d, invalid convert fraction [%d].\n",__FUNCTION__,__LINE__,s->convertfrac);
        return 0;
    }

    if(MONO == s->nchans)
    {
        if(NULL == resample_mono_proc_list[s->convertfrac])
        {
            HI_RESAMPLE_PRINT(HI_ID_SRC, "%s() %d, not support, convert fraction [%d].\n",__FUNCTION__,__LINE__,s->convertfrac);
            return 0;
        }

        return resample_mono_proc_list[s->convertfrac](inst, inbuf, insamps, outbuf);
    }
    else if(STEREO == s->nchans)
    {
        if(NULL == resample_stero_proc_list[s->convertfrac])
        {
            HI_RESAMPLE_PRINT(HI_ID_SRC, "%s() %d, not support, convert fraction [%d].\n",__FUNCTION__,__LINE__,s->convertfrac);
            return 0;
        }

        return resample_stero_proc_list[s->convertfrac](inst, inbuf, insamps, outbuf);
    }
    else
    {
        HI_RESAMPLE_PRINT(HI_ID_SRC, "%s() %d, invalid channels [%d].\n",__FUNCTION__,__LINE__,s->nchans);
        return 0;
    }
   		
}

