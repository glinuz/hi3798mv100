/******************************* Include Files *******************************/

#include "drv_omxvdec_pts_recv.h"
//#include "hi_drv_vdec.h"
//#include "drv_vdec_private.h"
#include "drv_omxvdec_alg.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define PTS_REC_MAX_NUM         (16)
#define PTS_REC_MAX_FRAMERATE   (60)           /* fps*/
#define PTS_MAX_VALUE           (0x5B05B05)    /* ms */
#define PTS_MAX_PASSTIME        (120 * 1000)   /* ms */
#define FRMTIME_SERIES_CNT      (2)
#define FRMTIME_MAX_JUMP        (1000)


/*************************** Structure Definition ****************************/

typedef enum tagPTSREC_CHAN_STATUS_E
{
    PTSREC_CHAN_INITED = 0,
    PTSREC_CHAN_STOP  = 1,
    PTSREC_CHAN_START = 2,
    PTSREC_CHAN_BUTT
} PTSREC_CHAN_STATUS_E;

typedef struct tagPTSREC_CHAN_INFO_S
{
    PTSREC_CHAN_STATUS_E enStatus;          /* Status */
    HI_U32               u32LastSrcPts;     /* Last valid src pts */
    HI_U32               u32LastSrcFrmIdx;  /* The index of last frame with a valid src pts */
    HI_U32               u32FrameNum;       /* Total frame number */
    HI_U32               u32FrameTimeCnt;   /* Frame interval counter */
    HI_U32               u32LastCalcFrmTime;/* Last calculated frame time(us) */
    HI_U32               u32CalcFrameTime;  /* Calculated frame time(us) */
    HI_U32               u32SetFrameTime;   /* Setted frame time(us) */
    HI_U32               u32GesFrameTime;   /* Guessed frame time(us) */
    HI_S32               s32InterPtsDelta;  /*interleaved source, VPSS module swtich field to frame, need to adjust pts*/
    HI_FRMRATE_TYPE_E    enFrmRateType;     /* Frame rate type */
    OPTM_ALG_FRD_S       stPtsInfo;
} PTSREC_CHAN_INFO_S;

/***************************** Global Definition *****************************/
/***************************** Static Definition *****************************/

static PTSREC_CHAN_INFO_S s_stPtsRecChan[PTS_REC_MAX_NUM];
static HI_VOID PTSREC_FrameRateDetect(HI_HANDLE hHandle, IMAGE *pstImage);

/*********************************** Code ************************************/

static HI_VOID PTSREC_InitParam(HI_HANDLE hHandle)
{
    s_stPtsRecChan[hHandle].u32LastSrcPts = HI_INVALID_PTS;
    s_stPtsRecChan[hHandle].u32LastSrcFrmIdx = -1;
    s_stPtsRecChan[hHandle].u32FrameNum = 0;
    s_stPtsRecChan[hHandle].u32FrameTimeCnt = 0;
    s_stPtsRecChan[hHandle].u32LastCalcFrmTime = HI_INVALID_TIME;
    s_stPtsRecChan[hHandle].u32CalcFrameTime = HI_INVALID_TIME;
    s_stPtsRecChan[hHandle].u32GesFrameTime = HI_INVALID_TIME;
    s_stPtsRecChan[hHandle].s32InterPtsDelta = 0;
    s_stPtsRecChan[hHandle].stPtsInfo.u320_Pts = HI_INVALID_PTS;
    s_stPtsRecChan[hHandle].stPtsInfo.u32120_Pts = HI_INVALID_PTS;
    s_stPtsRecChan[hHandle].stPtsInfo.u32QueCnt = 0;
    s_stPtsRecChan[hHandle].stPtsInfo.u32QueStable = 0;
}

HI_S32 OMX_PTSREC_Init(HI_VOID)
{
    HI_S32 i;

    for (i = 0; i < PTS_REC_MAX_NUM; i++)
    {
        s_stPtsRecChan[i].enStatus = PTSREC_CHAN_INITED;
        s_stPtsRecChan[i].u32SetFrameTime = HI_INVALID_TIME;
        PTSREC_InitParam(i);
        OMX_OPTM_ALG_FrdInfo_Reset(&s_stPtsRecChan[i].stPtsInfo, 0);
        s_stPtsRecChan[i].stPtsInfo.u320_Pts = HI_INVALID_PTS;
        s_stPtsRecChan[i].stPtsInfo.u32120_Pts = HI_INVALID_PTS;
        s_stPtsRecChan[i].stPtsInfo.u32QueCnt = 0;
        s_stPtsRecChan[i].stPtsInfo.u32QueStable = 0;
        s_stPtsRecChan[i].enFrmRateType = HI_FRMRATE_TYPE_PTS;
    }

    return HI_SUCCESS;
}

HI_S32 OMX_PTSREC_DeInit(HI_VOID)
{
    HI_S32 i;

    for (i = 0; i < PTS_REC_MAX_NUM; i++)
    {
        if (s_stPtsRecChan[i].enStatus > PTSREC_CHAN_INITED)
        {
            OMX_PTSREC_Free(i);
            OmxPrint(OMX_WARN, "Free %d in deinit!\n", i);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 OMX_PTSREC_Alloc(HI_HANDLE hHandle)
{
    if (hHandle >= PTS_REC_MAX_NUM)
    {
        OmxPrint(OMX_ERR,  "Bad handle:%d\n", hHandle);
        return HI_FAILURE;
    }

    if (s_stPtsRecChan[hHandle].enStatus > PTSREC_CHAN_INITED)
    {
        OmxPrint(OMX_ERR, "Already alloc:%d\n", hHandle);
        return HI_FAILURE;
    }

    s_stPtsRecChan[hHandle].enStatus = PTSREC_CHAN_STOP;
    s_stPtsRecChan[hHandle].u32SetFrameTime = HI_INVALID_TIME;
    PTSREC_InitParam(hHandle);
    OMX_OPTM_ALG_FrdInfo_Reset(&s_stPtsRecChan[hHandle].stPtsInfo, 0);
    return HI_SUCCESS;
}

HI_S32 OMX_PTSREC_Free(HI_HANDLE hHandle)
{
    if (hHandle >= PTS_REC_MAX_NUM)
    {
        OmxPrint(OMX_ERR, "Bad handle:%d\n", hHandle);
        return HI_FAILURE;
    }

    s_stPtsRecChan[hHandle].enStatus = PTSREC_CHAN_INITED;
    return HI_SUCCESS;
}

HI_S32 OMX_PTSREC_Start(HI_HANDLE hHandle)
{
    if (hHandle >= PTS_REC_MAX_NUM)
    {
        OmxPrint(OMX_ERR, "Bad handle:%d\n", hHandle);
        return HI_FAILURE;
    }

    if (s_stPtsRecChan[hHandle].enStatus < PTSREC_CHAN_STOP)
    {
        OmxPrint(OMX_ERR, "Alloc first:%d\n", hHandle);
        return HI_FAILURE;
    }

    s_stPtsRecChan[hHandle].enStatus = PTSREC_CHAN_START;
    PTSREC_InitParam(hHandle);
    return HI_SUCCESS;
}

HI_S32 OMX_PTSREC_Stop(HI_HANDLE hHandle)
{
    if (hHandle >= PTS_REC_MAX_NUM)
    {
        OmxPrint(OMX_ERR, "Bad handle:%d\n", hHandle);
        return HI_FAILURE;
    }

    if (s_stPtsRecChan[hHandle].enStatus < PTSREC_CHAN_STOP)
    {
        OmxPrint(OMX_ERR, "Alloc first:%d\n", hHandle);
        return HI_FAILURE;
    }

    s_stPtsRecChan[hHandle].enStatus = PTSREC_CHAN_STOP;
    return HI_SUCCESS;
}

HI_S32 OMX_PTSREC_Reset(HI_HANDLE hHandle)
{
    if (hHandle >= PTS_REC_MAX_NUM)
    {
        OmxPrint(OMX_ERR, "Bad handle:%d\n", hHandle);
        return HI_FAILURE;
    }

    if (s_stPtsRecChan[hHandle].enStatus < PTSREC_CHAN_STOP)
    {
        OmxPrint(OMX_ERR, "Alloc first:%d\n", hHandle);
        return HI_FAILURE;
    }

    if (s_stPtsRecChan[hHandle].enStatus > PTSREC_CHAN_STOP)
    {
        OmxPrint(OMX_WARN, "Stop first:%d\n", hHandle);
        return HI_FAILURE;
    }

    PTSREC_InitParam(hHandle);
    OMX_OPTM_ALG_FrdInfo_Reset(&s_stPtsRecChan[hHandle].stPtsInfo, 0);
    return HI_SUCCESS;
}

HI_S32 OMX_PTSREC_SetFrmRate(HI_HANDLE hHandle, HI_FRMRATE_PARAM_S *pstFrmRate)
{
    HI_U32 u32DivNum;

    if (hHandle >= PTS_REC_MAX_NUM)
    {
        OmxPrint(OMX_ERR, "Bad handle:%d\n", hHandle);
        return HI_FAILURE;
    }

    if (s_stPtsRecChan[hHandle].enStatus < PTSREC_CHAN_STOP)
    {
        OmxPrint(OMX_ERR, "Alloc first:%d\n", hHandle);
        return HI_FAILURE;
    }

    s_stPtsRecChan[hHandle].enFrmRateType = pstFrmRate->enFrmRateType;

    if ((HI_FRMRATE_TYPE_USER == s_stPtsRecChan[hHandle].enFrmRateType)
        || (HI_FRMRATE_TYPE_USER_PTS == s_stPtsRecChan[hHandle].enFrmRateType))
    {
        // 1
        u32DivNum = pstFrmRate->stSetFrmRate.u32fpsInteger* 1000 + pstFrmRate->stSetFrmRate.u32fpsDecimal;

        if((u32DivNum > PTS_REC_MAX_FRAMERATE * 1000) || (0 == u32DivNum))
        {
            OmxPrint(OMX_ERR, "invalid frame rate, Integer(%d), Decimal(%d) \n", pstFrmRate->stSetFrmRate.u32fpsInteger, pstFrmRate->stSetFrmRate.u32fpsDecimal);
            return HI_FAILURE;
        }

        /*correct to us*/
        /*CNcomment: 保留小数点后三位 */
        s_stPtsRecChan[hHandle].u32SetFrameTime = 1000000000 / u32DivNum;
        OmxPrint(OMX_INFO, "set chan(%d) frm diff(%d)\n", hHandle,  s_stPtsRecChan[hHandle].u32SetFrameTime);
    }
    else
    {
        s_stPtsRecChan[hHandle].u32SetFrameTime = -1;
    }

    return HI_SUCCESS;
}


HI_S32 OMX_PTSREC_GetFrmRate(HI_HANDLE hHandle, HI_FRMRATE_PARAM_S *pstFrmRate)
{
    HI_U32 u32Val;

    if (hHandle >= PTS_REC_MAX_NUM)
    {
        OmxPrint(OMX_ERR, "Bad handle:%d\n", hHandle);
        return HI_FAILURE;
    }

    if (s_stPtsRecChan[hHandle].enStatus < PTSREC_CHAN_STOP)
    {
        OmxPrint(OMX_ERR, "Alloc first:%d\n", hHandle);
        return HI_FAILURE;
    }

    if (s_stPtsRecChan[hHandle].u32SetFrameTime != HI_INVALID_TIME)
    {
        u32Val = 1000000000 / s_stPtsRecChan[hHandle].u32SetFrameTime;
        pstFrmRate->stSetFrmRate.u32fpsInteger = u32Val / 1000;
        pstFrmRate->stSetFrmRate.u32fpsDecimal = u32Val % 1000;
    }
    else
    {
        OmxPrint(OMX_ERR, "Didn't set\n");
        pstFrmRate->stSetFrmRate.u32fpsInteger = 0;
        pstFrmRate->stSetFrmRate.u32fpsDecimal = 0;
    }

    return HI_SUCCESS;
}

HI_U32 OMX_PTSREC_GetInterPtsDelta(HI_HANDLE hHandle)
{
    return (HI_U32)s_stPtsRecChan[hHandle].s32InterPtsDelta;
}

/*
 * For HI_FRMRATE_TYPE_PTS:      Recover PTS and detect frame rate;
 * For HI_FRMRATE_TYPE_STREAM:   Only Recover PTS, use frame rate from stream;
 * For HI_FRMRATE_TYPE_USER:     Only Recover PTS, use user set frame rate;
 * For HI_FRMRATE_TYPE_USER_PTS: Recover PTS and detect frame rate;
 */
HI_U32 OMX_PTSREC_CalcStamp(HI_HANDLE hHandle, IMAGE *pstImage)
{
    HI_U32 u32NumPass = 0;
    HI_U32 u32PtsPass = 0;
    HI_U32 u32FrameTime = HI_INVALID_TIME;
    HI_U32 u32SrcPts, u32Pts;
    PTSREC_CHAN_INFO_S *pstPtsInfo = NULL;

    if (hHandle >= PTS_REC_MAX_NUM)
    {
        OmxPrint(OMX_ERR, "Bad handle:%d\n", hHandle);
        return HI_FAILURE;
    }

    pstPtsInfo = &s_stPtsRecChan[hHandle];

    if (pstPtsInfo->enStatus < PTSREC_CHAN_START)
    {
        OmxPrint(OMX_ERR, "Start first:%d\n", hHandle);
        return HI_FAILURE;
    }

    /* Temprary usage */
    pstPtsInfo->u32FrameNum++;
    pstImage->frame_num = pstPtsInfo->u32FrameNum;

    /* TOADD */
#if 0
    /* If there were some frames discard before this frame,calculate again */
    if (pstImage->frame_num == 0)
    {
        pstPtsInfo->u32LastSrcPts = -1;
    }
#endif


    /*calculate how many frames between two frames with valid src pts, ingore the loopback situation*/
    /*CNcomment: 计算两个有效PTS帧之间的帧数 无符号减法不考虑环回的情况 0 - 255 = 1*/
    if (HI_INVALID_PTS != pstPtsInfo->u32LastSrcPts)
    {
        u32NumPass = pstImage->frame_num - pstPtsInfo->u32LastSrcFrmIdx;
    }

    u32SrcPts = (HI_U32)pstImage->SrcPts;

    /* u32SrcPts is valid */
    if (HI_INVALID_PTS != u32SrcPts)
    {
        /* calculate StatPtsDelta based on pts delta and frame numbers between two valid src pts */
        /* CNcomment: 根据两个有效PTS间的PTS差值和帧数计算StatPtsDelta */
        if (HI_INVALID_PTS != pstPtsInfo->u32LastSrcPts)
        {
            if (u32SrcPts >= pstPtsInfo->u32LastSrcPts)
            {
                u32PtsPass = u32SrcPts - pstPtsInfo->u32LastSrcPts;
            }
            /* pts loopback */
            /* TODO:how to distinguish pts back and loopback */
            else if (u32SrcPts < pstPtsInfo->u32LastSrcPts)
            {
                OmxPrint(OMX_INFO, "Pts rewind\n");
                u32PtsPass = PTS_MAX_VALUE - pstPtsInfo->u32LastSrcPts + u32SrcPts;
            }

            /*calculate the frame time*/
            if (u32NumPass)
            {
                if (u32PtsPass <= PTS_MAX_PASSTIME)
                {
                    /*only successive twice u32FrameTime is the same, then update u32CalcFrameTime*/
                    /**CNcomment: 只有连续2次的FrameTime计算相同才会更新CalcFrameTime*/
                    if (HI_INVALID_TIME != pstPtsInfo->u32CalcFrameTime)
                    {
                        if (OPTM_ALG_ABS(u32PtsPass * 1000 / u32NumPass - pstPtsInfo->u32LastCalcFrmTime) > FRMTIME_MAX_JUMP)
                        {
                            pstPtsInfo->u32FrameTimeCnt = 0;
                        }

                        pstPtsInfo->u32FrameTimeCnt++;
                        if (pstPtsInfo->u32FrameTimeCnt >= FRMTIME_SERIES_CNT)
                        {
                            pstPtsInfo->u32CalcFrameTime = u32PtsPass * 1000 / u32NumPass;
                            pstPtsInfo->u32FrameTimeCnt = FRMTIME_SERIES_CNT;
                        }
                    }
                    else
                    {
                        pstPtsInfo->u32CalcFrameTime = u32PtsPass * 1000 / u32NumPass;
                        pstPtsInfo->u32FrameTimeCnt++;
                    }

                    pstPtsInfo->u32LastCalcFrmTime = u32PtsPass * 1000 / u32NumPass;

                    s_stPtsRecChan[hHandle].s32InterPtsDelta = pstPtsInfo->u32CalcFrameTime /2000;
                }
                else
                {
                    /*keep the u32CalcFrameTime while pts is abnormal*/
                    OmxPrint(OMX_WARN, "pts %d exception\n", u32SrcPts);
                    if (u32SrcPts >= pstPtsInfo->u32LastSrcPts) /* pts jump*/
                    {
                        s_stPtsRecChan[hHandle].s32InterPtsDelta = 20;
                    }
                    else
                    {
                        s_stPtsRecChan[hHandle].s32InterPtsDelta = -20; /* pts back and loopback  */
                    }
                }
            }
            else
            {
                OmxPrint(OMX_WARN, "Frame index %d repeat\n", pstImage->frame_num);
                s_stPtsRecChan[hHandle].s32InterPtsDelta = 20;
            }
        }
        else
        {
            OmxPrint(OMX_INFO, "Got first valid source pts:%d\n", u32SrcPts);
            s_stPtsRecChan[hHandle].s32InterPtsDelta = 20;
        }

        pstPtsInfo->u32LastSrcPts = u32SrcPts;
        pstPtsInfo->u32LastSrcFrmIdx = pstImage->frame_num;
        u32Pts = u32SrcPts;
    }
    /* u32SrcPts is invalid */
    else
    {
        if (HI_INVALID_PTS != pstPtsInfo->u32LastSrcPts)
        {
#if 0
            if (((HI_UNF_VCODEC_TYPE_REAL8 == enVdecType)
                 || (HI_UNF_VCODEC_TYPE_REAL9 == enVdecType))
                && (-1 != pstPtsInfo->u32SetFrameTime))
            {
                u32Pts = pstPtsInfo->u32LastSrcPts + pstPtsInfo->u32SetFrameTime / 1000;
                pstPtsInfo->u32LastSrcPts = u32Pts;
                pstImage->PTS = (HI_U64)u32Pts;
                return u32Pts;
            }
#endif

            if (HI_FRMRATE_TYPE_USER == pstPtsInfo->enFrmRateType)
            {
                /* Choose u32FrameTime from: 1.SetFrameTime 2.CalcFrameTime 3.GuessFrameTime*/
                if (HI_INVALID_TIME != pstPtsInfo->u32SetFrameTime)
                {
                    u32FrameTime = pstPtsInfo->u32SetFrameTime;
                }
                else if (HI_INVALID_TIME != pstPtsInfo->u32CalcFrameTime)
                {
                    u32FrameTime = pstPtsInfo->u32CalcFrameTime;
                }
                else
                {
                    if (HI_INVALID_TIME == pstPtsInfo->u32GesFrameTime)
                    {
                        if (pstImage->image_width <= 720 && pstImage->image_height <= 480)
                        {
                            pstPtsInfo->u32GesFrameTime = 33333;  /* MAYBE NTSC */
                        }
                        else if (pstImage->image_width <= 720 && pstImage->image_height <= 576)
                        {
                            pstPtsInfo->u32GesFrameTime = 40000;  /* MAYBE PAL */
                        }
                        else
                        {
                            pstPtsInfo->u32GesFrameTime = 40000;
                        }
                    }

                    u32FrameTime = pstPtsInfo->u32GesFrameTime;
                }
            }
            else /* HI_UNF_AVPLAY_FRMRATE_TYPE_USER_PTS ... */
            {
                /* Choose u32FrameTime from: 1.u32CalcFrameTime 2.u32SetFrameTime 3.GuessFrameTime*/
                if (HI_INVALID_TIME != pstPtsInfo->u32CalcFrameTime)
                {
                    u32FrameTime = pstPtsInfo->u32CalcFrameTime;
                }
                else if (HI_INVALID_TIME != pstPtsInfo->u32SetFrameTime)
                {
                    u32FrameTime = pstPtsInfo->u32SetFrameTime;
                }
                else
                {
                    if (HI_INVALID_TIME == pstPtsInfo->u32GesFrameTime)
                    {
                        if ((pstImage->image_width <= 720) && (pstImage->image_height <= 480))
                        {
                            pstPtsInfo->u32GesFrameTime = 33333;  /* MAYBE NTSC */
                        }
                        else if ((pstImage->image_width <= 720) && (pstImage->image_height <= 576))
                        {
                            pstPtsInfo->u32GesFrameTime = 40000;  /* MAYBE PAL */
                        }
                        else
                        {
                            pstPtsInfo->u32GesFrameTime = 40000;
                        }
                    }

                    u32FrameTime = pstPtsInfo->u32GesFrameTime;
                }
            }

            u32Pts = pstPtsInfo->u32LastSrcPts + (u32FrameTime * u32NumPass) / 1000;
            s_stPtsRecChan[hHandle].s32InterPtsDelta = u32FrameTime/2000;
        }
        else //if (HI_INVALID_PTS != pstPtsInfo->u32LastSrcPts)
        {
            u32Pts = HI_INVALID_PTS;
            s_stPtsRecChan[hHandle].s32InterPtsDelta = 0;
        }
    }
    #if 0
    if (0 == (pstImage->format & 0x300))/*PROGRESSIVE*/
    {
        s_stPtsRecChan[hHandle].s32InterPtsDelta = 0;
    }
    #endif

    pstImage->PTS = (HI_U64)u32Pts;
    PTSREC_FrameRateDetect(hHandle, pstImage);
    return u32Pts;
}

static HI_VOID PTSREC_FrameRateDetect(HI_HANDLE hHandle, IMAGE *pstImage)
{
    HI_U32 u32FrameRate;

    /*
     * For RWZB test, VO will use default frame rate adapted to output norm.
     * If frame type was set to STREAM type, need not detect, use stream frame rate.
     * If frame type was set to USER type, need not detect, use the user set value.
     */
    if (!((pstImage->optm_inf.Rwzb > 0) ||
          (HI_FRMRATE_TYPE_STREAM == s_stPtsRecChan[hHandle].enFrmRateType) ||
          (HI_FRMRATE_TYPE_USER == s_stPtsRecChan[hHandle].enFrmRateType)))
    {
        u32FrameRate = OMX_OPTM_ALG_FrameRateDetect(
                                &(s_stPtsRecChan[hHandle].stPtsInfo),
                                (HI_U32)pstImage->PTS);
        pstImage->frame_rate = u32FrameRate * 1024 / 10;
    }
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
