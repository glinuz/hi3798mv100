/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCXgrHIPlGWwjY48PA756GbU69BErqnMH8NRXsO
3MWeW03uP3GLSIchcxnV9uCX4NMGNjCeg7lJ/Zw2ZGX5LPHfKS38xNFQTONv1xGnr43bAsNx
DxFef5H/PKu5TfhyVRwNezJ7SKfQke0IJin0HxL/7E+3OcxprgHSG8AYAPjfIA==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/




































/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name       		    : 	linear_src.c
  Version        		     : 	Initial Draft
  Author         		     : 	Hisilicon multimedia software group
  Created       		      : 	2010/01/21
  Last Modified		        :
  Description  		        :
  Function List 		:
  History       		:
  1.Date        		: 	2010/01/21
    Author      		: 	z40717
    Modification   	        :	Created file

******************************************************************************/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */
//#include <stdafx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "hi_debug.h"
#include "assembly.h"
#include "linear_src.h"
#include "resampler_statname.h"

#ifdef LINEARSRC_FIXPOINT

/* TODO: use high precison MUL32_30 to replace MULSHIFT32(x,y)<<2 */

static HI_S32 g_s32SampleRateTab[] =
{
    8000,
    11025,
    12000,
    16000,
    22050,
    24000,
    32000,
    44100,
    48000,
    88200,
    96000,
    192000,
};

static HI_S32 g_s32IvtSfTab[] =
{
    0x00041893 >> 1, /* (1.0/8k)*2^30 */
    0x0002f8df >> 1, /* (1.0/11k)*2^30 */
    0x0002bb0d >> 1, /* (1.0/12k)*2^30 */
    0x00020c4a >> 1, /* (1.0/16k)*2^30 */
    0x00017c70 >> 1, /* (1.0/22k)*2^30 */
    0x00015d86 >> 1, /* (1.0/24k)*2^30 */
    0x00010625 >> 1, /* (1.0/32k*2^30 */
    0x0000be38 >> 1, /* (1.0/44k)*2^30 */
    0x0000aec3 >> 1, /* (1.0/48k)*2^30 */
    0x00005f1c >> 1, /* (1.0/88k)*2^30 */
    0x00005762 >> 1, /* (1.0/96k)*2^30 */
    0x00002bb1 >> 1, /* (1.0/192k)*2^30 */
};
#endif

HI_VOID LinearSRC_Create(SRC_Linear **ppLinearSrc, HI_S32 inRate, HI_S32 outRate, HI_S32 BitPerSample,
                               HI_S32 Channels)
{
	int n;
	SRC_Linear *pLinearSrc;

	*ppLinearSrc = NULL;

	if(inRate<=0 || inRate>192000)
	{
		HI_RESAMPLE_PRINT(HI_ID_SRC, "invalid inRate(%d)\n",inRate);
	}

	if(outRate<=0 || outRate>192000)
	{
		HI_RESAMPLE_PRINT(HI_ID_SRC, "invalid inRate(%d)\n",outRate);
	}
	
	if(outRate/inRate >6)
	{
		HI_RESAMPLE_PRINT(HI_ID_SRC, "inRate(%d),outRate(%d)\n",inRate,outRate);
	}
    pLinearSrc = (SRC_Linear *) HI_MALLOC(HI_ID_SRC, sizeof(SRC_Linear));
    if(NULL == pLinearSrc)   //for fix moto
    {
        HI_RESAMPLE_PRINT(HI_ID_SRC, "HI_MALLOC SRC_linear failure!\n");
        return;
    }
	memset(pLinearSrc,0,sizeof(SRC_Linear));
	if(pLinearSrc)
	{
#ifdef LINEARSRC_FIXPOINT
	    for (n = 0; n < sizeof(g_s32SampleRateTab) / sizeof(HI_S32); n++)
	    {
	        if (g_s32SampleRateTab[n] == outRate)
	        {
	            break;
	        }
	    }

	    pLinearSrc->IvtSFFactor = g_s32IvtSfTab[n];
#endif
	    for (n = 0; n < Channels; n++)
	    {
	        pLinearSrc->PreSample[n] = 0;
	    }
		pLinearSrc->BitPerSample = BitPerSample;
		pLinearSrc->Channels = Channels;
		pLinearSrc->AdjustInRate = inRate;
		pLinearSrc->InRate	  = inRate;
		pLinearSrc->OutRate   = outRate;
		*ppLinearSrc = pLinearSrc;	
	}
	
}
HI_VOID LinearSRC_Destroy(SRC_Linear *pLinearSrc)
{
	if(pLinearSrc)
		HI_FREE(HI_ID_SRC, pLinearSrc);
}


HI_S32 LinearSRC_GetMaxOutputNum(SRC_Linear *pLinearSrc, HI_S32 insamps)
{
		HI_S32 outsamps;
	
		if (pLinearSrc == HI_NULL)
		{
			return -1;
		}
	
		outsamps = ((HI_U32)insamps * pLinearSrc->OutRate / pLinearSrc->InRate) + 1; // use int to calculate instead float
	
		return outsamps;

}

HI_S32 LinearSRC_CheckUpdate(SRC_Linear *pLinearSrc, HI_S32 inRate, HI_S32 outRate, HI_S32 BitPerSample,
                               HI_S32 Channels)
{
    HI_S32 change = 0;

    change += (pLinearSrc->BitPerSample != BitPerSample);
    change += (pLinearSrc->Channels != Channels);
    change += (pLinearSrc->InRate != inRate);
    change += (pLinearSrc->OutRate != outRate);
    if (change == 0)
    {
        return HI_FALSE;
    }
	return HI_TRUE;

}


//static SRC_Linear g_sSpeedAdjustSRC[MAX_HIAO_INPUT_PORT_NUMBER];

HI_VOID LinearSRC_UpdateChange(SRC_Linear *pLinearSrc, HI_S32 inRate, HI_S32 outRate, HI_S32 BitPerSample,
                               HI_S32 Channels)
{
    HI_S32 n;
    HI_S32 change = 0;

    change += (pLinearSrc->BitPerSample != BitPerSample);
    change += (pLinearSrc->Channels != Channels);
    change += (pLinearSrc->InRate != inRate);
    change += (pLinearSrc->OutRate != outRate);
    if (change == 0)
    {
        return;
    }

    /* reset state */
    pLinearSrc->BitPerSample = BitPerSample;
    pLinearSrc->Channels = Channels;
    pLinearSrc->AdjustInRate = inRate;
    pLinearSrc->InRate    = inRate;
    pLinearSrc->OutRate   = outRate;
    pLinearSrc->Remainder = 0;
    pLinearSrc->DisContinuity = 0;
    pLinearSrc->SpeedAdjust = 0;

#ifdef LINEARSRC_FIXPOINT
    for (n = 0; n < sizeof(g_s32SampleRateTab) / sizeof(HI_S32); n++)
    {
        if (g_s32SampleRateTab[n] == pLinearSrc->OutRate)
        {
            break;
        }
    }

    if(n < sizeof(g_s32SampleRateTab) / sizeof(HI_S32))    //for fix moto
    {
        pLinearSrc->IvtSFFactor = g_s32IvtSfTab[n];
    }
#endif

    for (n = 0; n < Channels; n++)
    {
        pLinearSrc->PreSample[n] = 0;
    }
}

HI_S32 LinearSRC_CheckDoSRC(SRC_Linear *pLinearSrc)
{
    /*
     * warning: can't compare s32SpeedAdjust and 0 only, you need to make fadetime useful to avoid nosize(baoyin)
     */
    if ((pLinearSrc->AdjustInRate == pLinearSrc->OutRate) && (pLinearSrc->DisContinuity == 0))
    {
        return 0;  /* dont do LinearSRC */
    }
    else
    {
        return 1;  /* do LinearSRC */
    }
}

HI_VOID LinearSRC_SetSpeedAdjust(SRC_Linear *pLinearSrc, HI_S32 SpeedAdjust)
{
    if (pLinearSrc->SpeedAdjust == SpeedAdjust)
    {
        return;
    }

#if 1
    /* check SpeedAdjust at app */
    if (SpeedAdjust > MAX_LINEAR_SPEED_ADJUST)
    {
        return;
    }
    else if (SpeedAdjust < -MAX_LINEAR_SPEED_ADJUST)
    {
        return;
    }
#endif

    if ((0 == pLinearSrc->SpeedAdjust) && (0 != SpeedAdjust))
    {
        pLinearSrc->DisContinuity = 1;  /* not continuous */
    }

    if ((0 != pLinearSrc->SpeedAdjust) && (0 == SpeedAdjust))
    {
        pLinearSrc->DisContinuity = 1;  /* not continuous */
    }

    pLinearSrc->SpeedAdjust  = SpeedAdjust;
    pLinearSrc->AdjustInRate = (pLinearSrc->InRate * (1000 + SpeedAdjust)) / 1000;
}

static HI_S32 LinearSRC_32bitProcessFrame(SRC_Linear* pInst,
                                          HI_S32 *    pInPcmBuf,
                                          HI_S32 *    pOutpcmBuf,
                                          HI_S32      InSamps)
{
    HI_S32 insamps, outsamps;
    HI_S32 sum, out = 0, in = 0, ch;
    HI_S32 *pcmbuf  = (HI_S32*)pInPcmBuf;
    HI_S32 Channels = pInst->Channels;

    insamps  = InSamps;
    outsamps = 0;

    if (pInst->DisContinuity && (insamps > 0))
    {
        if (pInst->AdjustInRate == pInst->OutRate) /* fade out */
        {
            for (ch = 0; ch < Channels; ch++)
            {
                pOutpcmBuf[Channels * out + ch] = (pInst->PreSample[ch]);
            }

            outsamps++;
            out++;
            pInst->DisContinuity = 0;
        }
        else  /* fade in */
        {
            for (ch = 0; ch < Channels; ch++)
            {
                (pInst->PreSample[ch]) = (HI_S32)(pcmbuf[Channels * in + ch]);
            }

            in++;
            insamps--;
            pInst->DisContinuity = 0;
        }
    }

    if (pInst->AdjustInRate == pInst->OutRate)
    {
        while (insamps > 0)
        {
            for (ch = 0; ch < Channels; ch++)
            {
                pOutpcmBuf[Channels * out + ch] = pcmbuf[Channels * in + ch];
            }

            outsamps++;
            out++;
            in++;
            insamps--;
        }
    }
    else
    {
        if (insamps > 0)
        {
            while (pInst->Remainder < pInst->OutRate)
            {
                for (ch = 0; ch < Channels; ch++)
                {
#ifdef LINEARSRC_FIXPOINT
                    sum  = MULSHIFT32(pcmbuf[Channels * (in + 0) + ch], pInst->IvtSFFactor * pInst->Remainder);
                    sum += MULSHIFT32(pInst->PreSample[ch], pInst->IvtSFFactor * (pInst->OutRate - pInst->Remainder));
					sum = CLIPTOSHORT(sum >>14);/* IvtSFFactor is 2.30 format */
					pOutpcmBuf[Channels * out + ch] =sum << 16;
#else
                    HI_S64 s64sample0, s64sample1;
                    s64sample0 = (HI_S64)(pcmbuf[Channels * in + ch]);
                    s64sample1 = (HI_S64)(pInst->PreSample[ch]);
                    pOutpcmBuf[Channels * out
                               + ch] = (HI_S32)((s64sample0 * pInst->Remainder + s64sample1
                                                 * (pInst->OutRate - pInst->Remainder)) / pInst->OutRate);
#endif
                }

                outsamps++;
                out++;
                pInst->Remainder += pInst->AdjustInRate;
            }

            pInst->Remainder -= pInst->OutRate;
            insamps--;
        }

        while (insamps > 0)
        {
            while (pInst->Remainder < pInst->OutRate)
            {
                for (ch = 0; ch < Channels; ch++)
                {
#ifdef LINEARSRC_FIXPOINT
/* TODO: use high precison MUL32_30 to replace MULSHIFT32(x,y)<<2 */
                    sum  = MULSHIFT32(pcmbuf[Channels * (in + 1) + ch], pInst->IvtSFFactor * pInst->Remainder);
                    sum += MULSHIFT32(pcmbuf[Channels
                                             * (in + 0) + ch], pInst->IvtSFFactor * (pInst->OutRate - pInst->Remainder));
					sum = CLIPTOSHORT(sum >>14);/* IvtSFFactor is 2.30 format */
					pOutpcmBuf[Channels * out + ch] =sum << 16;

#else
                    HI_S64 s64sample0, s64sample1;
                    s64sample0 = (HI_S64)(pcmbuf[Channels * (in + 1) + ch]);
                    s64sample1 = (HI_S64)(pcmbuf[Channels * (in + 0) + ch]);
                    pOutpcmBuf[Channels * out
                               + ch] = (HI_S32)((s64sample0 * pInst->Remainder + s64sample1
                                                 * (pInst->OutRate - pInst->Remainder)) / pInst->OutRate);
#endif
                }

                outsamps++;
                out++;
                pInst->Remainder += pInst->AdjustInRate;
            }

            pInst->Remainder -= pInst->OutRate;
            in++;
            insamps--;
        }

        for (ch = 0; ch < Channels; ch++)
        {
            (pInst->PreSample[ch]) = (HI_S32)(pcmbuf[Channels * in + ch]);
        }
    }

    return outsamps;
}

static HI_S32 LinearSRC_16bitProcessFrame(SRC_Linear* pInst,
                                          HI_S16 *    pInPcmBuf,
                                          HI_S16 *    pOutpcmBuf,
                                          HI_S32      InSamps)
{
    HI_S32 insamps, outsamps;
    HI_S32 sum, out = 0, in = 0, ch;
    HI_S16 *pcmbuf  = (HI_S16*)pInPcmBuf;
    HI_S32 Channels = pInst->Channels;

    insamps  = InSamps;
    outsamps = 0;

    if (pInst->DisContinuity && (insamps > 0))
    {
        if (pInst->AdjustInRate == pInst->OutRate) /* fade out */
        {
            for (ch = 0; ch < Channels; ch++)
            {
                pOutpcmBuf[Channels * out + ch] = (pInst->PreSample[ch]);
            }

            outsamps++;
            out++;
            pInst->DisContinuity = 0;
        }
        else  /* fade in */
        {
            for (ch = 0; ch < Channels; ch++)
            {
                (pInst->PreSample[ch]) = (HI_S32)(pcmbuf[Channels * in + ch]);
            }

            in++;
            insamps--;
            pInst->DisContinuity = 0;
        }
    }

    if (pInst->AdjustInRate == pInst->OutRate)
    {
        while (insamps > 0)
        {
            for (ch = 0; ch < Channels; ch++)
            {
                pOutpcmBuf[Channels * out + ch] = pcmbuf[Channels * in + ch];
            }

            outsamps++;
            out++;
            in++;
            insamps--;
        }
    }
    else
    {
        if (insamps > 0)
        {
            while (pInst->Remainder < pInst->OutRate)
            {
                for (ch = 0; ch < Channels; ch++)
                {
#ifdef LINEARSRC_FIXPOINT
                    sum  = MULSHIFT32(pcmbuf[Channels * (in + 0) + ch], pInst->IvtSFFactor * pInst->Remainder);
                    sum += MULSHIFT32(pInst->PreSample[ch], pInst->IvtSFFactor * (pInst->OutRate - pInst->Remainder));
					pOutpcmBuf[Channels * out + ch] = CLIPTOSHORT(sum << 2);/* IvtSFFactor is 2.30 format */
#else
                    pOutpcmBuf[Channels * out + ch] = ((pcmbuf[Channels * in
                                                               + ch] * pInst->Remainder + pInst->PreSample[ch]
                                                        * (pInst->OutRate - pInst->Remainder)) / pInst->OutRate);
#endif
                }

                outsamps++;
                out++;
                pInst->Remainder += pInst->AdjustInRate;
            }

            pInst->Remainder -= pInst->OutRate;
            insamps--;
        }

        while (insamps > 0)
        {
            while (pInst->Remainder < pInst->OutRate)
            {
                for (ch = 0; ch < Channels; ch++)
                {
#ifdef LINEARSRC_FIXPOINT
                    sum  = MULSHIFT32(pcmbuf[Channels * (in + 1) + ch], pInst->IvtSFFactor * pInst->Remainder);
                    sum += MULSHIFT32(pcmbuf[Channels
                                             * (in + 0) + ch], pInst->IvtSFFactor * (pInst->OutRate - pInst->Remainder));
					pOutpcmBuf[Channels * out + ch] = CLIPTOSHORT(sum << 2);/* IvtSFFactor is 2.30 format */

#else
                    pOutpcmBuf[Channels * out
                               + ch] = ((pcmbuf[Channels
                                                * (in + 1) + ch] * pInst->Remainder + pcmbuf[Channels * in + ch]
                                         * (pInst->OutRate - pInst->Remainder)) / pInst->OutRate);
#endif
                }

                outsamps++;
                out++;
                pInst->Remainder += pInst->AdjustInRate;
            }

            pInst->Remainder -= pInst->OutRate;
            in++;
            insamps--;
        }

        for (ch = 0; ch < Channels; ch++)
        {
            (pInst->PreSample[ch]) = (HI_S32)(pcmbuf[Channels * in + ch]);
        }
    }
    return outsamps;
}


HI_S32  LinearSRC_ProcessFrame(SRC_Linear*  pInst,
                               HI_S32 *     pInPcmBuf,
                               HI_S32 *     pOutpcmBuf,
                               HI_S32       InSamps)
{
	HI_S32 outsampls;
    if (16 == pInst->BitPerSample)
    {
        outsampls =  LinearSRC_16bitProcessFrame(pInst, (HI_S16*)pInPcmBuf, (HI_S16*)pOutpcmBuf, InSamps);
    }
    else
    {
        outsampls =  LinearSRC_32bitProcessFrame(pInst, (HI_S32*)pInPcmBuf, (HI_S32*)pOutpcmBuf, InSamps);
    }
	return outsampls;
}


static HI_VOID  Volume_16bitProcessFrame(
	                        HI_S16 *     pPcmBuf,
                               HI_S32       InSamps,
                               HI_S32       Channels,
                               HI_S16       s16Volume)
{
	         HI_S32 sum, n;
                for (n = 0; n < (InSamps*Channels); n++)
                {
                    sum  = MULF32_16B((HI_S32)pPcmBuf[n],s16Volume);
                    pPcmBuf[n] = (HI_S16)sum;
                }
}

static HI_VOID  Volume_32itProcessFrame(
	                        HI_S32 *     pPcmBuf,
                               HI_S32       InSamps,
                               HI_S32       Channels,
                               HI_S16       s16Volume)
{
	         HI_S32 sum, n;
                for (n = 0; n < (InSamps*Channels); n++)
                {
                    sum  = MULF32_16B((HI_S32)pPcmBuf[n],s16Volume);
                    pPcmBuf[n] = (HI_S32)sum;
                }
}

HI_VOID  Volume_ProcessFrame(
	                        HI_S32 *     ps32PcmBuf,
                               HI_S32       InSamps,
                               HI_S32       BitPerSample,
                               HI_S32       Channels,
                               HI_S16       s16Volume)
{
    if (16 == BitPerSample)
    {
        Volume_16bitProcessFrame((HI_S16*)ps32PcmBuf, InSamps,Channels,s16Volume);
    }
    else
    {
        Volume_32itProcessFrame((HI_S32*)ps32PcmBuf, InSamps,Channels,s16Volume);
    }
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
