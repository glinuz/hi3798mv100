#include <linux/slab.h>
#include "drv_vdec_alg.h"


//#define OPTM_ALG_ACMCHECKPRINT 0

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

HI_U32 OPTM_ALG_FrameRateDetect(OPTM_ALG_FRD_S *pPtsInfo,HI_U32 Pts)
{
    HI_S32   rateDiff;
    HI_U32   nowRate;
    if (pPtsInfo->u320_Pts == 0xffffffffuL)
    {
        pPtsInfo->u320_Pts = Pts;
    }
    else
    {
        pPtsInfo->u32QueCnt = pPtsInfo->u32QueCnt+1;
    }
    if (pPtsInfo->u32QueCnt == 61)
    {
        HI_U32 u32Delta = 0;
        HI_U32 u32NewRate = 0;
        if (Pts > pPtsInfo->u320_Pts)
            u32Delta = (Pts - pPtsInfo->u320_Pts)/60;
        else
            u32Delta = (pPtsInfo->u320_Pts - Pts)/60;
        if(0 != u32Delta)
        {
            u32NewRate = 10000/u32Delta;
        }
        if (OPTM_ALG_ABS(u32NewRate-pPtsInfo->u32QueRate)<10)
        {
            if (pPtsInfo->u32QueStable < 3)
                pPtsInfo->u32QueStable++;
        }
        else
        {
            pPtsInfo->u32QueRate = u32NewRate;
            pPtsInfo->u32QueStable = 0;
        }
        pPtsInfo->u320_Pts = Pts;
        pPtsInfo->u32QueCnt = 0;
    }
    if (pPtsInfo->InPTSSqn[0] != 0xffffffffuL)
    {
        pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr] = Pts;

        nowRate = OPTM_ALG_InPTSSqn_CalNowRate(pPtsInfo, 0);

        if (nowRate)
        {
            HI_U32 u32JumpDelta;
            rateDiff = nowRate - pPtsInfo->InFrameRateLast;

            /* For stream about 30fps, use bigger JUMP hreshold, accept wider PTS diff range */
            /* CNcomment: 对30fps左右的常用码流,放大JUMP门限，将PTS抖动纳入正常范围*/
            if(pPtsInfo->InFrameRateLast >= 200 && pPtsInfo->InFrameRateLast <= 400)
            {
                u32JumpDelta = 200;
            }
            else
            {
                u32JumpDelta = pPtsInfo->InFrameRateLast / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD;
            }
                
            /*
            if (OPTM_ALG_ABS(rateDiff) > 
                pPtsInfo->InFrameRateLast / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD)
            {
            */
            
            if (OPTM_ALG_ABS(rateDiff) > 
                u32JumpDelta)
            {
                pPtsInfo->unableTime++;
                if (10 == pPtsInfo->unableTime)
                {
                    pPtsInfo->InFrameRate = 0;
                    pPtsInfo->unableTime   = 0;
                }

                pPtsInfo->InFrameRateLast = nowRate;

                /* the StableThreshold = fps/10;
                   in order to detect the fps change quickly; */                       
                if (nowRate > 300)
                    pPtsInfo->StableThreshold = nowRate / 100;
                else
                    pPtsInfo->StableThreshold = 3;

                /* reset the PTSSqn,reserve the lastest two pts data*/
                OPTM_ALG_FrdInfo_Reset(pPtsInfo, 2);
            }
            else
            {
                HI_U32 u32Stable;
                pPtsInfo->InFrameRateEqueTime++;

                /* if 3 == pPtsInfo->InFrameRateEqueTime,
                    we think the fps has been stable,
                    so reset the unableTime*/
                if (3 == pPtsInfo->InFrameRateEqueTime 
                    || nowRate == pPtsInfo->InFrameRate)
                {
                    pPtsInfo->unableTime = 0;
                }

                /* For stream about 30fps, use bigger JUMP hreshold, accept wider PTS diff range */
                /* CNcomment: 对30fps左右的常用码流,加大帧率变化稳定门限，
                        保证不会因为两三帧稳定的PTS抖动导致无法检测出有效帧率*/
                /* calculate the times of occurrence of stability of new frame rate*/
                if(pPtsInfo->InFrameRateLast >= 200 && pPtsInfo->InFrameRateLast <= 400)
                {
                    u32Stable = 10;
                }
                else
                {
                    u32Stable = pPtsInfo->StableThreshold;
                }

                /*
                if (pPtsInfo->InFrameRateEqueTime == pPtsInfo->StableThreshold)
                {   
                    */
                if (pPtsInfo->InFrameRateEqueTime == u32Stable)
                {   
                    nowRate = OPTM_ALG_InPTSSqn_CalNowRate(pPtsInfo, 1);
                    rateDiff = nowRate - pPtsInfo->InFrameRate;

                    if (abs(rateDiff) > 
                        pPtsInfo->InFrameRate / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD)
                    {
                        OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo, nowRate);

                        pPtsInfo->InFrameRateLast = pPtsInfo->InFrameRate;
                    }
                }

                /* when InPTSSqn is full,calculate the average fps */
                if (pPtsInfo->nowPTSPtr == pPtsInfo->length-1)
                {
                    HI_S32 s32WaveDelta;
                    nowRate = OPTM_ALG_InPTSSqn_CalNowRate(pPtsInfo, 1);

                    rateDiff = OPTM_ALG_ABS(pPtsInfo->InFrameRate - nowRate);

                    /* For stream about 30fps, use bigger JUMP hreshold, accept wider PTS diff range */
                    /* CNcomment: 对30fps左右的常用码流,加大微小变化回归门限，
                        因为前面门限加大，
                        可能会导致全队列算出的平均帧率存在抖动，放大该门限，避免输出帧率频繁微小变化*/
                    if(pPtsInfo->InFrameRateLast >= 200 && pPtsInfo->InFrameRateLast <= 400)
                    {
                        s32WaveDelta = 50;
                    }
                    else
                    {
                        s32WaveDelta = pPtsInfo->InFrameRateLast
                            / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD
                            / OPTM_FRD_INFRAME_RATE_WAVE_THRESHOLD;
                    }
                    /* detect the tiny rate wave*/
                    /*
                    if (rateDiff 
                            > pPtsInfo->InFrameRateLast
                            / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD
                            / OPTM_FRD_INFRAME_RATE_WAVE_THRESHOLD)
                        */
                    if (rateDiff > s32WaveDelta)
                    {
                        if(pPtsInfo->InFrameRate != 0 )
                        {
                            OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo,
                                            (nowRate+pPtsInfo->InFrameRate) / 2);
                        }
                        else
                        {
                            OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo,
                                            nowRate);
                        }
                        pPtsInfo->InFrameRateLast = nowRate;
                    }   
                    else
                    {
                        /* the second time pPtsInfo->InFrameRateLast == nowRate,we adjust
                        InFrameRate as nowRate,because the tiny ratediff between InframeRate and actual
                        FrameRate may exist*/
                        if (pPtsInfo->InFrameRateLast == nowRate)
                        {
                            if(abs(rateDiff) > pPtsInfo->InFrameRateLast
                            / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD
                            / OPTM_FRD_INFRAME_RATE_WAVE_THRESHOLD)
                            {
                                if(pPtsInfo->InFrameRate != 0 )
                                {
                                    OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo,
                                            (nowRate+pPtsInfo->InFrameRate) / 2);
                                }
                                else
                                {
                                    OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo,
                                            nowRate);
                                }
                                //OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo, (nowRate+pPtsInfo->InFrameRate) / 2);
                            }
                            else
                            {
                                OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo, nowRate);
                            }
                        }
                        else
                        {
                            pPtsInfo->InFrameRateLast = nowRate;
                        }
                    }

                    /* reset the PTSSqn,reserve the lastest one pts data*/
                    OPTM_ALG_FrdInfo_Reset(pPtsInfo, 1);
                }
                else
                    pPtsInfo->nowPTSPtr++;
            }

        }
        else
        {   
            /* nowRate = 0,we think it is a unstable fps*/
            pPtsInfo->unableTime++;
            if (10 == pPtsInfo->unableTime)
            {
                pPtsInfo->InFrameRate = 0;
                pPtsInfo->unableTime   = 0;
            }

            OPTM_ALG_FrdInfo_Reset(pPtsInfo, 1);
        }


    }
    else
    {
        OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo,0);

        OPTM_ALG_FrdInfo_Reset(pPtsInfo,0);

        pPtsInfo->InPTSSqn[0] = Pts;
        pPtsInfo->nowPTSPtr = 1;
        pPtsInfo->unableTime = 0;
        pPtsInfo->InFrameRateLast = pPtsInfo->InFrameRate;


        /*DEBUG_PRINTK("\nFieldMode=ALL---VideoCombine=%d---Type=%d---Topfirst=%d---",
                        pFrame->enVideoCombine,
                        pFrame->enFrameType,
                        pFrame->bTopFieldFirst);*/
    }
    if(pPtsInfo->u32QueStable == 3 && pPtsInfo->InFrameRate == 0)
        pPtsInfo->InFrameRate = pPtsInfo->u32QueRate;
    return pPtsInfo->InFrameRate;
    
}

HI_VOID OPTM_ALG_FrdInfo_Reset(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 ptsNum)
{
    HI_U32 nowPts;
    HI_U32 prePts;
    HI_U32 i;
    
    if (ptsNum)
    {
        if (ptsNum == 1)
        {
            nowPts = pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr];
            for (i = 0; i < OPTM_FRD_IN_PTS_SEQUENCE_LENGTH; i++)
            {
                pPtsInfo->InPTSSqn[i] = 0xffffffff;
            }
            pPtsInfo->InPTSSqn[0] = nowPts;
            pPtsInfo->nowPTSPtr = 1;
            pPtsInfo->InFrameRateEqueTime = 0;
        }
        else//ptsNum == 2
        {
            nowPts = pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr];
            prePts = pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr-1];
            for (i = 0; i < OPTM_FRD_IN_PTS_SEQUENCE_LENGTH; i++)
            {
                pPtsInfo->InPTSSqn[i] = 0xffffffff;
            }
            pPtsInfo->InPTSSqn[0] = prePts;
            pPtsInfo->InPTSSqn[1] = nowPts;
            pPtsInfo->nowPTSPtr = 2;
            pPtsInfo->InFrameRateEqueTime = 1;
        }
    }
    else
    {
        for (i = 0; i < OPTM_FRD_IN_PTS_SEQUENCE_LENGTH; i++)
        {
            pPtsInfo->InPTSSqn[i] = 0xffffffff;
        }
        pPtsInfo->InFrameRateEqueTime = 0;
		pPtsInfo->u320_Pts = 0xffffffff;
        pPtsInfo->u32120_Pts = 0xffffffff;
        pPtsInfo->u32QueCnt = 0;
        pPtsInfo->u32QueStable = 0;
    }
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

