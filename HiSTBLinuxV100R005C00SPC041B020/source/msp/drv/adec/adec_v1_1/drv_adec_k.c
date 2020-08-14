/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name       		    : 	adec_intf.c
  Version        		    : 	Initial Draft
  Author         		    : 	Hisilicon multimedia software group
  Created       		    : 	2006/01/23
  Last Modified		        :
  Description  		        :
  Function List 		:	So Much ....
  History       		:
  1.Date        		: 	2006/01/23
    Author      		: 	f47391
    Modification   	    :	Created file

******************************************************************************/
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/seq_file.h>

#include "hi_drv_mmz.h"
#include "hi_drv_stat.h"
#include "hi_drv_sys.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_error_mpi.h"

#include "hi_drv_adec.h"
#include "drv_adec_ext.h"

#ifdef ADEC_KERNEL_DECODE
#include "HA.AUDIO.MP3.decode.h"
#endif
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

#define ADEC_NAME "HI_ADEC"

#ifdef ADEC_KERNEL_DECODE

#define CHECK_ADEC_NULL_PTR(ptr) \
    do \
    { \
        if (NULL == ptr) \
        { \
            HI_ERR_ADEC("invalid NULL poiner!\n"); \
            return HI_FAILURE; \
        } \
    } while (0)

#define CHECK_ADEC_OPEN(hAdec) \
    do \
    {  \
        if (hAdec >= ADEC_INSTANCE_MAXNUM) \
        { \
            HI_ERR_ADEC("Invalid adec handle!\n"); \
            return HI_FAILURE; \
        } \
        if (HI_NULL == g_stAdecSlimChan[hAdec].hDecoder) \
        { \
            HI_ERR_ADEC("adec not open!\n"); \
            return HI_FAILURE; \
        } \
    } while (0)

typedef struct hiADEC_PTS_Queue_S
{
    HI_U32   u32LastPtsMs;
    HI_U32   u32LastStorePtsMs;
    HI_U32   ulPTSread;        /* PTS buffer read  ptr */
    HI_U32   ulPTSwrite;       /* PTS buffer write ptr */
    ADEC_PTS tPTSArry[ADEC_MAX_STORED_PTS_NUM];
} ADEC_PTS_QUE_S;

typedef struct hiADEC_Stream_OutBuf_S
{
    HI_U32           u32OutBufNum;
    HI_U32           u32BufReadIdx;
    HI_U32           u32BufWriteIdx;
    HI_U32*          pu32BaseAddr;
    ADEC_OUTPUTBUF_S outBuf[ADEC_MAX_WORK_BUFFER_NUMBER];
} ADEC_STREAM_OUTBUF_S;

typedef struct hiADEC_SLIM_CHAN_S
{
    ADEC_KERNEL_TYPE_E             enCodecType;
    HI_U32                         u32StreamCnt;
    HI_VOID*                       hAdecArry;
    HI_U32                         u32ContinueErrNum;
    HI_U32                         u32TotDecodeFrame;
    HI_U32                         u32PcmSamplesPerFrame;
    HI_UNF_SAMPLE_RATE_E           enSampleRate;
    HI_VOID*                       hDecoder;
    HI_HA_DECODE_S*                hDecoderEntry;
    ADEC_PTS_QUE_S                 PTSQue;
    ADEC_STREAM_BUFFER_S           InStreamBuf;
    ADEC_STREAM_OUTBUF_S           outStreamBuf;
    ADEC_SLIM_ATTR_S               stAdecAttrBackup;
    HI_UNF_STREAM_BUF_S            sStreamBackupArry[3];
} ADEC_SLIM_CHAN_S;

 #define MAX_ADEC_SLIM_SAMPLE_PER_FRAME 2048
 #define MAX_ADEC_SLIM_CHANNELS 2
 #define MAX_ADEC_SLIM_PADDING_SIZE 4096
 #define MAX_ADEC_SLIM_FRAME_SIZE (MAX_ADEC_SLIM_SAMPLE_PER_FRAME * MAX_ADEC_SLIM_CHANNELS * sizeof(HI_S16))

 #define MAX_ADEC_SLIM_AUDTYPE_MPEG 0

static ADEC_SLIM_CHAN_S g_stAdecSlimChan[ADEC_INSTANCE_MAXNUM];

extern HI_HA_DECODE_S g_ha_audio_mp3_decode_kel_entry;

static HI_VOID ADECKelResetMidState(ADEC_SLIM_CHAN_S *pstAdecChan)
{
    memset(pstAdecChan->sStreamBackupArry, 0, sizeof(HI_UNF_STREAM_BUF_S) * 3);
    pstAdecChan->u32StreamCnt = 0;
    pstAdecChan->u32ContinueErrNum = 0;
    pstAdecChan->u32StreamCnt = 0;
    pstAdecChan->u32ContinueErrNum = 0;
    pstAdecChan->u32TotDecodeFrame = 0;
    pstAdecChan->u32PcmSamplesPerFrame = 0;
    pstAdecChan->enSampleRate = 48000;
}

static HI_S32 ADECKelInitOutBuf(ADEC_STREAM_OUTBUF_S  *pstOutBuf, HI_U32 u32OutBufNum, HI_U32 uMaxBufSizePerFrame )
{
    HI_U32 i;
    HI_U32 *pu32Buf;

    //CHECK_ADEC_NULL_PTR(pstOutBuf);
    if(u32OutBufNum > 32 || u32OutBufNum == 0)
    {
        HI_FATAL_ADEC("adec err  u32OutBufNum (%d)\n",u32OutBufNum);
        return HI_FAILURE;
    }

    /* Allocate OutBuffer */
    pu32Buf = HI_KMALLOC(HI_ID_ADEC, u32OutBufNum * uMaxBufSizePerFrame, GFP_KERNEL);
    if (HI_NULL_PTR == pu32Buf)
    {
        HI_FATAL_ADEC("adec malloc error\n");
        return HI_FAILURE;
    }

    /* assigning buffer */
    for (i = 0; i < u32OutBufNum; i++)
    {
        pstOutBuf->outBuf[i].OutBuf = (HI_U32*)(((HI_U8*)pu32Buf) + i * uMaxBufSizePerFrame);
        pstOutBuf->outBuf[i].bFlag = HI_FALSE;
    }

    /* Reset output buffer pointer */
    pstOutBuf->pu32BaseAddr   = (HI_U32*)pu32Buf;
    pstOutBuf->u32BufReadIdx  = 0;
    pstOutBuf->u32BufWriteIdx = 0;
    pstOutBuf->u32OutBufNum = u32OutBufNum;
    //HI_INFO_ADEC(" uMaxBufSizePerFrame=%d\n", uMaxBufSizePerFrame);

    return HI_SUCCESS;
}

static HI_VOID ADECKelDeInitOutBuf(ADEC_STREAM_OUTBUF_S  *pstOutBuf )
{
    HI_U32 i;

    //CHECK_NULL_PTR_NORET(pstOutBuf);

    if (HI_NULL_PTR != pstOutBuf->pu32BaseAddr)
    {
        HI_KFREE(HI_ID_ADEC, pstOutBuf->pu32BaseAddr);
        pstOutBuf->pu32BaseAddr = 0;
    }

    for (i = 0; i < pstOutBuf->u32OutBufNum; i++)
    {
        pstOutBuf->outBuf[i].OutBuf = HI_NULL_PTR;
        pstOutBuf->outBuf[i].bFlag = HI_FALSE;
    }

    pstOutBuf->u32BufReadIdx  = 0;
    pstOutBuf->u32BufWriteIdx = 0;
    return;
}

static HI_VOID ADECKelResetOutBuf(ADEC_STREAM_OUTBUF_S  *pstOutBuf, HI_U32 uMaxBufSizePerFrame )
{
    HI_U32 i;
    HI_S32 *ps32Buf;

    CHECK_NULL_PTR_NORET(pstOutBuf);
    CHECK_ADEC_OUTBUF_NUMBER_NORET(pstOutBuf->u32OutBufNum);
    CHECK_NULL_PTR_NORET(pstOutBuf->pu32BaseAddr);

    // Assigning output buffer
    ps32Buf = (HI_S32*)(pstOutBuf->pu32BaseAddr);
    for (i = 0; i < pstOutBuf->u32OutBufNum; i++)
    {
        pstOutBuf->outBuf[i].OutBuf = (HI_U32*)(ps32Buf + i * (uMaxBufSizePerFrame / sizeof(HI_S32)));
        pstOutBuf->outBuf[i].bFlag = HI_FALSE;
    }

    /* Reset output buffer pointer */
    pstOutBuf->u32BufReadIdx  = 0;
    pstOutBuf->u32BufWriteIdx = 0;
    return;
}

static HI_S32 ADECKelInitInBuf(ADEC_STREAM_BUFFER_S  *pstInBuf, HI_U32 u32BufSize, HI_U32 u32PaddingSize)
{
    if ((u32BufSize < ADEC_MIN_INPUT_BUFFER_SIZE) || (u32BufSize > ADEC_MAX_INPUT_BUFFER_SIZE))
    {
        HI_ERR_ADEC(" invalid input buffer size(%d) minsize(%d) maxsize(%d)!\n", u32BufSize,
                    ADEC_MIN_INPUT_BUFFER_SIZE, ADEC_MAX_INPUT_BUFFER_SIZE);
        return HI_FAILURE;
    }

    pstInBuf->pu8Data = HI_KMALLOC(HI_ID_ADEC, (u32BufSize + u32PaddingSize), GFP_KERNEL);
    if (HI_NULL_PTR == pstInBuf->pu8Data)
    {
        HI_ERR_ADEC("adec malloc error\n");
        return HI_FAILURE;
    }

    pstInBuf->pu8Data += u32PaddingSize;

    /* Read buffer size */
    pstInBuf->u32BufFree = u32BufSize;
    pstInBuf->u32BufSize = u32BufSize;
    pstInBuf->s32BufReadPos  = 0;
    pstInBuf->u32BufWritePos = 0;
    pstInBuf->u32BufPaddingSize = u32PaddingSize;

    return HI_SUCCESS;
}

static HI_VOID ADECKelDeInitInBuf(ADEC_STREAM_BUFFER_S  *pstInBuf )
{
    if (HI_NULL_PTR != pstInBuf->pu8Data)
    {
        HI_KFREE(HI_ID_ADEC, pstInBuf->pu8Data - pstInBuf->u32BufPaddingSize);
        pstInBuf->pu8Data = HI_NULL_PTR;
    }

    pstInBuf->u32BufFree = 0;
    pstInBuf->s32BufReadPos  = 0;
    pstInBuf->u32BufWritePos = 0;

    pstInBuf->u32BufPaddingSize = 0;
    return;
}

static HI_VOID ADECKelResetInBuf(ADEC_STREAM_BUFFER_S  *pstInBuf )
{
    /* Read buffer size */
    pstInBuf->u32BufFree = pstInBuf->u32BufSize;
    pstInBuf->s32BufReadPos  = 0;
    pstInBuf->u32BufWritePos = 0;
    return;
}

static HI_S32 ADECKelCreateDecoder(ADEC_SLIM_CHAN_S *pstAdecChan, ADEC_KERNEL_TYPE_E enCodecType)
{
    HI_HADECODE_OPENPARAM_S stOpenParms;
    HI_S32 sRet;

    switch (enCodecType)
    {
    case ADEC_KEL_TPYE_MPEG:
        HA_MP3_DecGetDefalutOpenParam(&(stOpenParms));
        pstAdecChan->hDecoderEntry = &g_ha_audio_mp3_decode_kel_entry;
        sRet = pstAdecChan->hDecoderEntry->DecInit(&(pstAdecChan->hDecoder), &stOpenParms);
        break;
    default:
        HI_ERR_ADEC( "output buffer full!\n" );
        return -1;
    }

    pstAdecChan->enCodecType = enCodecType;

    return 0;
}

static HI_S32 ADECKelResetDecoder(ADEC_SLIM_CHAN_S *pstAdecChan)
{
    HI_HADECODE_OPENPARAM_S stOpenParms;

    switch (pstAdecChan->enCodecType)
    {
    case ADEC_KEL_TPYE_MPEG:
        HA_MP3_DecGetDefalutOpenParam(&(stOpenParms));
        pstAdecChan->hDecoderEntry->DecDeInit(pstAdecChan->hDecoder);
        pstAdecChan->hDecoderEntry->DecInit(&(pstAdecChan->hDecoder), &stOpenParms);
        break;
    default:
        return 0;
    }

    return 0;
}

static HI_S32 ADECKelDestroyDecoder(ADEC_SLIM_CHAN_S *pstAdecChan)
{
    switch (pstAdecChan->enCodecType)
    {
    case ADEC_KEL_TPYE_MPEG:
        pstAdecChan->hDecoderEntry->DecDeInit(pstAdecChan->hDecoder);
        break;
    default:
        return 0;
    }

    pstAdecChan->enCodecType = -1;
    pstAdecChan->hDecoder = NULL;
    return 0;
}

static HI_VOID ADECKelRestPTSQuet(ADEC_PTS_QUE_S *pstPTSQue)
{
    //CHECK_NULL_PTR_NORET(pstPTSQue);

    memset(pstPTSQue->tPTSArry, 0, sizeof(ADEC_PTS) * ADEC_MAX_STORED_PTS_NUM);
    pstPTSQue->ulPTSread    = 0;
    pstPTSQue->ulPTSwrite   = 0;
    pstPTSQue->u32LastPtsMs = -1;
    pstPTSQue->u32LastStorePtsMs = -1;
    return;
}

/****************************************************************************
 * Function:    ADECKelFindPTS
 *
 * Description: try to find current frame u32PtsMs
 *
 * Inputs:      none
 *
 * Outputs:     none
 *
 * Return:      Success : HI_SUCCESS
 *              Failure : HI_FAILURE
 *****************************************************************************/
static HI_U32 ADECKelFindPTS(ADEC_PTS_QUE_S *pstPTSQue, HI_U32 u32Pos, HI_U32 u32PcmOutSamplesPerFrame,
                             HI_U32 u32OutSampleRate, HI_U32 *pu32OrgPTS)
{
    HI_U32 u32PtsMs;
    HI_U32 Pos, FoundPtsPos = (HI_U32)-1;
    HI_U32 rdPos;
    HI_U32 wtPos;
    ADEC_PTS *ptPTS = HI_NULL;

    rdPos = (HI_U32)pstPTSQue->ulPTSread;
    wtPos = (HI_U32)pstPTSQue->ulPTSwrite;
    ptPTS = pstPTSQue->tPTSArry;

    for (Pos = rdPos; Pos != wtPos;  Pos = (Pos + 1) % ADEC_MAX_STORED_PTS_NUM)
    {
        if (ptPTS[Pos].u32BegPos < ptPTS[Pos].u32EndPos)
        {
            if ((ptPTS[Pos].u32BegPos <= u32Pos) && (ptPTS[Pos].u32EndPos > u32Pos))
            {
                FoundPtsPos = ptPTS[Pos].u32PtsMs;
                break;
            }
        }
        else
        {
            if ((ptPTS[Pos].u32BegPos <= u32Pos) || (ptPTS[Pos].u32EndPos > u32Pos))
            {
                FoundPtsPos = ptPTS[Pos].u32PtsMs;
                break;
            }
        }
    }

    if (((HI_U32)-1) == FoundPtsPos)
    {
        /*can not find a valid PTS*/
        HI_U32 u32Delta;
        u32Delta = (u32PcmOutSamplesPerFrame * 1000) / u32OutSampleRate;
        u32PtsMs = pstPTSQue->u32LastPtsMs + u32Delta;
        if (Pos != wtPos)
        {
            /* Found a invalid PTS */
            pstPTSQue->ulPTSread = (HI_U32)Pos;
        }
    }
    else
    {
        /* Found a valid PTS */
        u32PtsMs = FoundPtsPos;
        ptPTS[Pos].u32PtsMs  = (HI_U32)(-1);
        pstPTSQue->ulPTSread = (HI_U32)Pos;
    }

    *pu32OrgPTS = FoundPtsPos;

    pstPTSQue->u32LastPtsMs = u32PtsMs;
    return u32PtsMs;
}

static HI_VOID ADECKelStorePTS (ADEC_PTS_QUE_S   *pstPTSQue, const ADEC_STREAM_BUFFER_S  *pstInBuf, HI_U32 u32PtsMs,
                                HI_U32 u32BsSize )
{
    ADEC_PTS *ptPTSArray = pstPTSQue->tPTSArry;

    if (0xffffffff != u32PtsMs)
    {
        if (u32PtsMs == pstPTSQue->u32LastStorePtsMs)
        {
            HI_INFO_ADEC("discard same PTS(%d)\n", u32PtsMs);
            u32PtsMs = 0xffffffff;
        }
    }

    /* make sure there are space to store */
    if (0xffffffff == u32PtsMs)
    {
        if (pstPTSQue->ulPTSwrite != 0)
        {
            ptPTSArray[pstPTSQue->ulPTSwrite - 1].u32EndPos =
                (ptPTSArray[pstPTSQue->ulPTSwrite - 1].u32EndPos + u32BsSize) % pstInBuf->u32BufSize;
        }
        else
        {
            ptPTSArray[ADEC_MAX_STORED_PTS_NUM - 1].u32EndPos =
                (ptPTSArray[ADEC_MAX_STORED_PTS_NUM - 1].u32EndPos + u32BsSize) % pstInBuf->u32BufSize;
        }
    }
    else if ((pstPTSQue->ulPTSwrite + 1) % ADEC_MAX_STORED_PTS_NUM != pstPTSQue->ulPTSread)
    {
        ptPTSArray[pstPTSQue->ulPTSwrite].u32PtsMs  = u32PtsMs;
        ptPTSArray[pstPTSQue->ulPTSwrite].u32BegPos = (HI_U32)pstInBuf->u32BufWritePos;
        ptPTSArray[pstPTSQue->ulPTSwrite].u32EndPos = (HI_U32)((HI_U32)pstInBuf->u32BufWritePos
                                                               + u32BsSize) % pstInBuf->u32BufSize;
        pstPTSQue->ulPTSwrite = (pstPTSQue->ulPTSwrite + 1) % ADEC_MAX_STORED_PTS_NUM;
    }
    else
    {
        HI_WARN_ADEC("Not enough PTS buffer, discard current PTS(%d)\n", u32PtsMs);
    }
}

static HI_S32 ADECKelOpenChannel(ADEC_SLIM_CHAN_S *pstAdecChan, ADEC_SLIM_ATTR_S *pstAdecAttr)
{
    HI_S32 nRet;

    // 1  Create decoder
    nRet = ADECKelCreateDecoder(pstAdecChan, pstAdecAttr->enCodecType);
    if (HI_SUCCESS != nRet)
    {
        HI_ERR_ADEC(" ADECKelCreateDecoder fail errCode=0x%x! \n", nRet);
        return HI_FAILURE;
    }

    // 2  Allocate Input Buffer
    nRet = ADECKelInitInBuf(&(pstAdecChan->InStreamBuf), pstAdecAttr->u32InBufSize, MAX_ADEC_SLIM_PADDING_SIZE);
    if (HI_SUCCESS != nRet)
    {
        ADECKelDestroyDecoder(pstAdecChan);
        HI_ERR_ADEC(" AENCInitInBuf fail errCode=0x%x! \n", nRet);
        return HI_FAILURE;
    }

    // 3  Allocate Output Buffer
    nRet = ADECKelInitOutBuf(&(pstAdecChan->outStreamBuf), pstAdecAttr->u32OutBufNum, MAX_ADEC_SLIM_FRAME_SIZE);
    if (HI_SUCCESS != nRet)
    {
        ADECKelDestroyDecoder(pstAdecChan);
        ADECKelDeInitInBuf(&(pstAdecChan->InStreamBuf));
        HI_ERR_ADEC(" AENCInitInBuf fail errCode=0x%x! \n", nRet);
        return HI_FAILURE;
    }

    // 4 Reset PTSQue
    ADECKelRestPTSQuet(&(pstAdecChan->PTSQue));

    memcpy(&(pstAdecChan->stAdecAttrBackup), pstAdecAttr, sizeof(ADEC_SLIM_ATTR_S));

    return HI_SUCCESS;
}

static HI_S32 ADECKelCloseChannel(ADEC_SLIM_CHAN_S *pstAdecChan)
{
    if (pstAdecChan->hAdecArry)
    {
        HI_KFREE(HI_ID_ADEC, pstAdecChan->hAdecArry);
        pstAdecChan->hAdecArry = HI_NULL;
    }

    ADECKelRestPTSQuet(&(pstAdecChan->PTSQue));
    ADECKelDeInitOutBuf(&(pstAdecChan->outStreamBuf));
    ADECKelDeInitInBuf(&(pstAdecChan->InStreamBuf));
    ADECKelDestroyDecoder(pstAdecChan);

    return HI_SUCCESS;
}

static HI_S32 ADECKelSendStream (ADEC_SLIM_CHAN_S *pstAdecChan, const HI_UNF_STREAM_BUF_S *pstStream, HI_U32 u32PtsMs)
{
    HI_S32 sRet = HI_SUCCESS;

    ADEC_PTS_QUE_S   *pstPTSQue     = HI_NULL_PTR;
    ADEC_STREAM_BUFFER_S  *pstInBuf = HI_NULL_PTR;

    if (pstStream->u32Size == 0)
    {
        HI_WARN_ADEC("stream u32Size is 0 \n" );
        return HI_SUCCESS;
    }

    /* more check, required? */
    if ((pstStream->u32Size > ADEC_MAX_INPUT_BLOCK_SIZE) || (pstStream->pu8Data == HI_NULL_PTR))
    {
        HI_ERR_ADEC("stream u32Bytes(%u) exceed maxsize(%d) or invaid pu8Data(0x%x)!\n",
                    pstStream->u32Size,
                    ADEC_MAX_INPUT_BLOCK_SIZE,
                    pstStream->pu8Data );
        return HI_FAILURE;
    }

    pstPTSQue = &(pstAdecChan->PTSQue);
    pstInBuf = &(pstAdecChan->InStreamBuf);

    /* Input Buffer full */
    if (pstInBuf->u32BufFree >= pstStream->u32Size)
    {
        ADECKelStorePTS(pstPTSQue, pstInBuf, u32PtsMs, pstStream->u32Size);

        /*Copy data into Input Buffer*/
        if (((HI_U32)pstInBuf->u32BufWritePos + pstStream->u32Size) <= pstInBuf->u32BufSize)
        {
            memcpy((HI_VOID *)(pstInBuf->pu8Data + pstInBuf->u32BufWritePos), (HI_VOID *)pstStream->pu8Data,
                   pstStream->u32Size);
        }
        else
        {
            memcpy((HI_VOID *)(pstInBuf->pu8Data
                               + pstInBuf->u32BufWritePos), (HI_VOID *)pstStream->pu8Data,
                   (HI_U32)(pstInBuf->u32BufSize
                            - pstInBuf->u32BufWritePos));
            memcpy((HI_VOID *)pstInBuf->pu8Data, (HI_VOID *)(pstStream->pu8Data
                                                             + (HI_U32)(pstInBuf->u32BufSize - pstInBuf->u32BufWritePos)),
                   pstStream->u32Size - (HI_U32)(pstInBuf->u32BufSize - pstInBuf->u32BufWritePos));
        }

        /* recaculate free buffer size */
        pstInBuf->u32BufWritePos = (pstInBuf->u32BufWritePos + pstStream->u32Size) % (pstInBuf->u32BufSize);
        pstInBuf->u32BufFree -= pstStream->u32Size;
    }
    else
    {
        /* no space for input buffer, we try to decode */
        sRet = HI_ERR_ADEC_IN_BUF_FULL;
    }
    return sRet;
}
static HI_S32 ADECKelResetChannel(ADEC_SLIM_CHAN_S *pstAdecChan)
{
    ADECKelResetDecoder(pstAdecChan);
    ADECKelResetOutBuf(&(pstAdecChan->outStreamBuf), MAX_ADEC_SLIM_FRAME_SIZE);
    ADECKelResetInBuf(&(pstAdecChan->InStreamBuf));
    ADECKelRestPTSQuet(&(pstAdecChan->PTSQue));
    ADECKelResetMidState(pstAdecChan);

    return HI_SUCCESS;
}

static HI_BOOL isChangeAttr(ADEC_SLIM_CHAN_S *pstAdecChan, ADEC_SLIM_ATTR_S *pstAdecAttr)
{
    ADEC_SLIM_ATTR_S *pstBackupAttr;

    pstBackupAttr = &(pstAdecChan->stAdecAttrBackup);

    if (pstBackupAttr->enCodecType != pstAdecAttr->enCodecType)
    {
        return HI_TRUE;
    }

    if (pstBackupAttr->u32InBufSize != pstAdecAttr->u32InBufSize)
    {
        return HI_TRUE;
    }

    if (pstBackupAttr->u32OutBufNum != pstAdecAttr->u32OutBufNum)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 HI_DRV_ADEC_Open(HI_HANDLE *phAdec, ADEC_SLIM_ATTR_S *pstAdecAttr)
{
    HI_U32 i;
    HI_S32 nRet;
    ADEC_SLIM_CHAN_S *pstAdecChan = HI_NULL_PTR;
    HI_VOID  *ptrmem = HI_NULL_PTR;

    for (i = 0; i < ADEC_INSTANCE_MAXNUM; i++)
    {
        if (!g_stAdecSlimChan[i].hDecoder)
        {
            break;
        }
    }

    if (i >= ADEC_INSTANCE_MAXNUM)
    {
        HI_ERR_ADEC("source is not enough to support so much adec\n");
        return HI_FAILURE;
    }

    pstAdecChan = &g_stAdecSlimChan[i];

    nRet = ADECKelOpenChannel(pstAdecChan, pstAdecAttr);
    if (HI_SUCCESS != nRet)
    {
        HI_ERR_ADEC("  fail errCode=0x%x! \n", nRet);
        return HI_FAILURE;
    }

    ptrmem = HI_KMALLOC(HI_ID_ADEC, ADEC_MAX_INPUT_BLOCK_SIZE, GFP_KERNEL);
    if (!ptrmem)
    {
        ADECKelCloseChannel(pstAdecChan);
        return HI_FAILURE;
    }

    pstAdecChan->hAdecArry = (HI_VOID*)ptrmem;
    ADECKelResetMidState(pstAdecChan);
    *phAdec = (HI_HANDLE)i;
    return HI_SUCCESS;
}

HI_S32 HI_DRV_ADEC_Close(HI_HANDLE hAdec)
{
    ADEC_SLIM_CHAN_S *pstAdecChan = HI_NULL_PTR;

    if (hAdec >= ADEC_INSTANCE_MAXNUM)
    {
        return HI_SUCCESS;
    }

    pstAdecChan = &g_stAdecSlimChan[hAdec];
    ADECKelCloseChannel(pstAdecChan);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_ADEC_SetAttr(HI_HANDLE hAdec, ADEC_SLIM_ATTR_S *pstAdecAttr)
{
    ADEC_SLIM_CHAN_S *pstAdecChan = HI_NULL_PTR;
    HI_S32  nRet;
    HI_VOID  *ptrmem = HI_NULL_PTR;

    CHECK_ADEC_OPEN(hAdec);
    CHECK_ADEC_NULL_PTR(pstAdecAttr);

    pstAdecChan = &g_stAdecSlimChan[hAdec];
    if (HI_FALSE == isChangeAttr(pstAdecChan, pstAdecAttr))
    {
        return HI_SUCCESS;
    }

    ADECKelCloseChannel(pstAdecChan);
    nRet = ADECKelOpenChannel(pstAdecChan, pstAdecAttr);
    if (HI_SUCCESS != nRet)
    {
        HI_ERR_ADEC(" ADEC_Kel_SetAttr fail errCode=0x%x! \n", nRet);
        return HI_FAILURE;
    }

    ptrmem = HI_KMALLOC(HI_ID_ADEC, ADEC_MAX_INPUT_BLOCK_SIZE, GFP_KERNEL);
    if (!ptrmem)
    {
        ADECKelCloseChannel(pstAdecChan);
        return HI_FAILURE;
    }

    pstAdecChan->hAdecArry = (HI_VOID*)ptrmem;
    ADECKelResetMidState(pstAdecChan);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_ADEC_GetAttr(HI_HANDLE hAdec, ADEC_SLIM_ATTR_S  *pstAttr)
{
    ADEC_SLIM_CHAN_S *pstAdecChan;

    CHECK_ADEC_OPEN(hAdec);
    CHECK_ADEC_NULL_PTR(pstAttr);

    pstAdecChan = &g_stAdecSlimChan[hAdec];

    memcpy(pstAttr, &(pstAdecChan->stAdecAttrBackup), sizeof(ADEC_SLIM_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_DRV_ADEC_Reset(HI_HANDLE hAdec)
{
    ADEC_SLIM_CHAN_S *pstAdecChan;
    CHECK_ADEC_OPEN(hAdec);

    pstAdecChan = &g_stAdecSlimChan[hAdec];
    ADECKelResetChannel(pstAdecChan);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_ADEC_SendStream(HI_HANDLE hAdec, const HI_UNF_STREAM_BUF_S *pstStream, HI_U32 u32PtsMs)
{
    HI_S32 retval;
    ADEC_SLIM_CHAN_S *pstAdecChan;
    
    CHECK_ADEC_OPEN(hAdec);
    CHECK_ADEC_NULL_PTR(pstStream);
    pstAdecChan = &g_stAdecSlimChan[hAdec];
    retval = ADECKelSendStream(pstAdecChan, pstStream, u32PtsMs);
    return retval;
}

HI_S32 HI_DRV_ADEC_Pull(HI_HANDLE hAdec)
{
    HI_S32 sRet = 0;
    HI_S32 s32BsLeftBytes, s32CurnFramePos;
    HI_U32 nextTail;

    ADEC_SLIM_CHAN_S *pstAdecChan = HI_NULL_PTR;
    ADEC_PTS_QUE_S   *pstPTSQue = HI_NULL_PTR;
    ADEC_STREAM_BUFFER_S  *pstInBuf  = HI_NULL_PTR;
    ADEC_STREAM_OUTBUF_S  *pstOutBuf = HI_NULL_PTR;
    ADEC_OUTPUTBUF_S *ptRetBuf = HI_NULL_PTR;

    HI_HADECODE_INPACKET_S avpkt;
    HI_HADECODE_OUTPUT_S sOut;

    CHECK_ADEC_OPEN(hAdec);

    pstAdecChan = &g_stAdecSlimChan[hAdec];
    pstPTSQue = &(pstAdecChan->PTSQue);
    pstInBuf  = &(pstAdecChan->InStreamBuf);
    pstOutBuf = &(pstAdecChan->outStreamBuf);
    ptRetBuf = pstOutBuf->outBuf;


    /*get unread area length*/
    s32BsLeftBytes = (HI_S32)(pstInBuf->u32BufSize - pstInBuf->u32BufFree);
    while (s32BsLeftBytes >= 7)
    {
        /* check availability of output buffer */
        nextTail = ((pstOutBuf->u32BufWriteIdx + 1) % (pstOutBuf->u32OutBufNum));
        if ((nextTail == pstOutBuf->u32BufReadIdx) || (ptRetBuf[pstOutBuf->u32BufWriteIdx].bFlag == HI_TRUE))
        {
            HI_INFO_ADEC( "output buffer full!\n" );
            break;
        }

 #if 1
        sOut.ps32PcmOutBuf = (HI_S32*)(ptRetBuf[pstOutBuf->u32BufWriteIdx].OutBuf);
        sOut.u32PcmOutBufSize = MAX_ADEC_SLIM_FRAME_SIZE;

        sOut.ps32BitsOutBuf = NULL;
        sOut.u32BitsOutBufSize = 0;
 #else
        sOut.ps32PcmOutBuf = pstAOFrame->ps32PcmBuffer;
        sOut.u32PcmOutBufSize = 2048 * 2 * sizeof(HI_S16);

        sOut.ps32BitsOutBuf = NULL;
        sOut.u32BitsOutBufSize = 0;
 #endif

        /*get linear-buffer unread data length*/
        if ((HI_S32)pstInBuf->u32BufWritePos <= pstInBuf->s32BufReadPos)
        {
            s32BsLeftBytes = (HI_S32)(pstInBuf->u32BufSize) - pstInBuf->s32BufReadPos;
        }

        /* setup bitstream input */
        avpkt.s32Size = (HI_S32)(s32BsLeftBytes);
        avpkt.pu8Data = pstInBuf->pu8Data + pstInBuf->s32BufReadPos;

        /* decode one audio frame */
        s32CurnFramePos = pstInBuf->s32BufReadPos;   /* look for PTS */
        sRet = pstAdecChan->hDecoderEntry->DecDecodeFrame(pstAdecChan->hDecoder, &avpkt, &sOut);
        if ((s32BsLeftBytes < avpkt.s32Size) || (avpkt.pu8Data < (pstInBuf->pu8Data + pstInBuf->s32BufReadPos))
            || (avpkt.pu8Data > (pstInBuf->pu8Data + pstInBuf->s32BufReadPos + s32BsLeftBytes)))
        {
            HI_ERR_ADEC("DecDecodeFrame err: out of input data range\n");

            /*reset decoder and all buffer*/
            ADECKelResetChannel(pstAdecChan);
            break;
        }

        if (HA_ErrorNotEnoughData == sRet)
        {
            if (pstInBuf->s32BufReadPos >= (HI_S32)pstInBuf->u32BufWritePos)
            {
                HI_S32 s32CopySize = (HI_S32)(pstInBuf->u32BufSize - (HI_U32)(avpkt.pu8Data - pstInBuf->pu8Data));
                HI_S32 s32PaddingSize = MAX_ADEC_SLIM_PADDING_SIZE;
                if (s32CopySize > s32PaddingSize)
                {
                    HI_ERR_ADEC("adec maxload not find a syncheader PaddingSize=%d, need=%d\n", s32PaddingSize,
                                s32CopySize);
                    s32CopySize = s32PaddingSize;
                }
                memcpy(pstInBuf->pu8Data - s32CopySize, pstInBuf->pu8Data + (pstInBuf->u32BufSize-s32CopySize), (HI_U32)s32CopySize);
                pstInBuf->s32BufReadPos = -s32CopySize;
                pstInBuf->u32BufFree += (HI_U32)(s32BsLeftBytes - s32CopySize);
                break;
            }
            else
            {
                pstInBuf->s32BufReadPos = (HI_S32)(avpkt.pu8Data - pstInBuf->pu8Data);
                pstInBuf->u32BufFree += (HI_U32)(s32BsLeftBytes - avpkt.s32Size);
                break;
            }

        }
        else if (HA_ErrorNone == sRet)    /* Success */
        {
            HI_U32 u32OrgPTS;

            /* look for PTS */
            if (s32CurnFramePos < 0)
            {
                s32CurnFramePos += (HI_S32)pstInBuf->u32BufSize;
            }

            if ((sOut.u32OutSampleRate == 0) || (sOut.u32OutSampleRate > 192000))
            {
                HI_ERR_ADEC("Invalid SampleRate(%d)! \n", sOut.u32OutSampleRate);
                return HI_FAILURE;
            }

 #if 1
            ptRetBuf[pstOutBuf->u32BufWriteIdx].u32PTS = ADECKelFindPTS(pstPTSQue, (HI_U32)s32CurnFramePos,
                                                                        sOut.u32PcmOutSamplesPerFrame,
                                                                        sOut.u32OutSampleRate, &u32OrgPTS);
            ptRetBuf[pstOutBuf->u32BufWriteIdx].u32OrgPTS = u32OrgPTS;
            ptRetBuf[pstOutBuf->u32BufWriteIdx].u32PcmOutSamplesPerFrame = sOut.u32PcmOutSamplesPerFrame;
            ptRetBuf[pstOutBuf->u32BufWriteIdx].u32BitsOutBytesPerFrame = sOut.u32BitsOutBytesPerFrame;
            ptRetBuf[pstOutBuf->u32BufWriteIdx].u32OutChannels   = sOut.u32OutChannels;
            ptRetBuf[pstOutBuf->u32BufWriteIdx].u32OutSampleRate = sOut.u32OutSampleRate;
            ptRetBuf[pstOutBuf->u32BufWriteIdx].bInterleaved = sOut.bInterleaved;
            ptRetBuf[pstOutBuf->u32BufWriteIdx].u32BitPerSample = sOut.u32BitPerSample;
            ptRetBuf[pstOutBuf->u32BufWriteIdx].ps32PcmOutBuf  = sOut.ps32PcmOutBuf;
            ptRetBuf[pstOutBuf->u32BufWriteIdx].ps32BitsOutBuf = sOut.ps32BitsOutBuf;

            /* move the tail of output buffer */
            pstOutBuf->u32BufWriteIdx = (pstOutBuf->u32BufWriteIdx + 1) % (pstOutBuf->u32OutBufNum);
            pstAdecChan->u32ContinueErrNum = 0;
            pstAdecChan->u32TotDecodeFrame++;
            pstAdecChan->u32PcmSamplesPerFrame = sOut.u32OutSampleRate;

 #else
            pstAOFrame->u32PtsMs = ADECKelFindPTS(pstPTSQue, (HI_U32)s32CurnFramePos,
                                                  sOut.u32PcmOutSamplesPerFrame,
                                                  sOut.u32OutSampleRate, &u32OrgPTS);

            ptRetBuf[pstOutBuf->u32BufWriteIdx].u32OrgPTS = u32OrgPTS;
            pstAOFrame->u32PcmSamplesPerFrame = sOut.u32PcmOutSamplesPerFrame;
            pstAOFrame->u32BitsBytesPerFrame = 0;
            pstAOFrame->u32Channels     = sOut.u32OutChannels;
            pstAOFrame->u32SampleRate   = sOut.u32OutSampleRate;
            pstAOFrame->bInterleaved    = sOut.bInterleaved;
            pstAOFrame->s32BitPerSample = sOut.u32BitPerSample;
            pstAOFrame->u32FrameIndex  += 1;
            pstInBuf->s32BufReadPos = (HI_S32)(avpkt.pu8Data - pstInBuf->pu8Data);
            pstInBuf->u32BufFree += (HI_U32)(s32BsLeftBytes - avpkt.s32Size);
            break;
 #endif
        }
        else     /* Failure */
        {
            //HI_MPI_EVT_Notify(EVENT_MSG_ADEC_ERR, HI_NULL, 0);
            HI_ERR_ADEC( "Decode HA_ErrorStreamCorrupt \n");

            /* in case of error, drop all the data in buffer */
            if (++ (pstAdecChan->u32ContinueErrNum) >= 8)
            {
                /*reset decoder and all buffer*/
                ADECKelResetChannel(pstAdecChan);
                HI_ERR_ADEC("ADEC detect continue %d error frame\n", 8);
            }
        }

        pstInBuf->s32BufReadPos = (HI_S32)(avpkt.pu8Data - pstInBuf->pu8Data);
        pstInBuf->u32BufFree += (HI_U32)(s32BsLeftBytes - avpkt.s32Size);
        s32BsLeftBytes = (HI_S32)(pstInBuf->u32BufSize - pstInBuf->u32BufFree);
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_ADEC_ReceiveFrame(HI_HANDLE hAdec, AO_FRAMEINFO_S *pstAOFrame, ADEC_EXTFRAMEINFO_S *pstExtInfo)
{
    ADEC_SLIM_CHAN_S *pstAdecChan = HI_NULL_PTR;
    ADEC_STREAM_OUTBUF_S  *pstOutBuf = HI_NULL_PTR;
    ADEC_OUTPUTBUF_S *ptOutElem = HI_NULL_PTR;

    /* Check parameter */
    CHECK_ADEC_OPEN(hAdec);
    CHECK_ADEC_NULL_PTR(pstAOFrame);

    pstAOFrame->u32PcmSamplesPerFrame = 0;
    pstAOFrame->u32BitsBytesPerFrame = 0;

    pstAdecChan = &g_stAdecSlimChan[hAdec];
    pstOutBuf = &(pstAdecChan->outStreamBuf);

    /* No data */
    if (pstOutBuf->u32BufReadIdx == pstOutBuf->u32BufWriteIdx)
    {
        return HI_ERR_ADEC_OUT_BUF_EMPTY;
    }

    ptOutElem = &(pstOutBuf->outBuf[pstOutBuf->u32BufReadIdx]);

    /* fill frame info */
    pstAOFrame->u32FrameIndex = pstOutBuf->u32BufReadIdx;
    pstAOFrame->bInterleaved = ptOutElem->bInterleaved;
    pstAOFrame->s32BitPerSample = (HI_S32)ptOutElem->u32BitPerSample;
    pstAOFrame->u32SampleRate = ptOutElem->u32OutSampleRate;
    pstAOFrame->u32Channels = ptOutElem->u32OutChannels;
    pstAOFrame->u32PtsMs = ptOutElem->u32PTS;
    pstAOFrame->u32PcmSamplesPerFrame = ptOutElem->u32PcmOutSamplesPerFrame;
    pstAOFrame->u32BitsBytesPerFrame = ptOutElem->u32BitsOutBytesPerFrame;
    pstAOFrame->tPcmBuffer  = (HI_VIRT_ADDR_T)ptOutElem->ps32PcmOutBuf;
    pstAOFrame->tBitsBuffer = (HI_VIRT_ADDR_T)ptOutElem->ps32BitsOutBuf;

    /* add extend adec infomation */
    if (pstExtInfo)
    {
        if (pstOutBuf->u32BufWriteIdx >= pstOutBuf->u32BufReadIdx)
        {
            pstExtInfo->u32FrameLeftNum = pstOutBuf->u32BufWriteIdx - pstOutBuf->u32BufReadIdx;
        }
        else
        {
            pstExtInfo->u32FrameLeftNum = pstOutBuf->u32OutBufNum
                                          - (pstOutBuf->u32BufReadIdx - pstOutBuf->u32BufWriteIdx);
        }

        pstExtInfo->u32FrameDurationMs = (ptOutElem->u32PcmOutSamplesPerFrame
                                          * 1000) / ((HI_U32)ptOutElem->u32OutSampleRate);
        pstExtInfo->u32OrgPtsMs = ptOutElem->u32OrgPTS;
    }

    if ((ptOutElem->u32OutSampleRate > 192000) ||
        (ptOutElem->u32OutSampleRate < 8000) ||
        (ptOutElem->u32BitPerSample > 24) ||
        (ptOutElem->u32BitPerSample < 8))
    {
        ptOutElem->bFlag = HI_FALSE;

        /* Move header */
        pstOutBuf->u32BufReadIdx = (pstOutBuf->u32BufReadIdx + 1) % (pstOutBuf->u32OutBufNum);

        //HI_WARN_ADEC("internal error:unsupport sample rate %d or bit depth %d.\n", ptOutElem->u32OutSampleRate,
        //         ptOutElem->u32BitPerSample);
        return HI_FAILURE;
    }
    pstAdecChan->enSampleRate = ptOutElem->u32OutSampleRate;

    if (pstAOFrame->u32BitsBytesPerFrame == 0)
    {
        pstAOFrame->tBitsBuffer = HI_NULL;
    }

    ptOutElem->bFlag = HI_TRUE;

    /* Move header */
    pstOutBuf->u32BufReadIdx = (pstOutBuf->u32BufReadIdx + 1) % (pstOutBuf->u32OutBufNum);

    return HI_SUCCESS;
}

/****************************************************************************
 * Function:    ADEC_ReleaseFrame
 *
 * Description: Free Audio Frame
 *
 * Inputs:      none
 *
 * Outputs:     none
 *
 * Return:      Success : HI_SUCCESS
 *              Failure : HI_FAILURE
 *****************************************************************************/
HI_S32 HI_DRV_ADEC_ReleaseFrame(HI_HANDLE hAdec, const AO_FRAMEINFO_S *pstAOFrame)
{
    ADEC_SLIM_CHAN_S *pstAdecChan = HI_NULL_PTR;
    ADEC_STREAM_OUTBUF_S  *pstOutBuf = HI_NULL_PTR;

    /* Check parameter */
    CHECK_ADEC_OPEN(hAdec);
    CHECK_ADEC_NULL_PTR(pstAOFrame);

    pstAdecChan = &g_stAdecSlimChan[hAdec];
    pstOutBuf = &(pstAdecChan->outStreamBuf);

    /* free buffer */
    (pstOutBuf->outBuf)[pstAOFrame->u32FrameIndex].bFlag = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 HI_DRV_ADEC_GetStatusInfo(HI_HANDLE hAdec, ADEC_STATUSINFO_S *pstStatusinfo)
{
    ADEC_SLIM_CHAN_S *pstAdecChan = HI_NULL_PTR;
    ADEC_STREAM_OUTBUF_S  *pstOutBuf = HI_NULL_PTR;
    ADEC_STREAM_BUFFER_S  *pstInBuf = HI_NULL_PTR;

    /* Check parameter */
    CHECK_ADEC_OPEN(hAdec);
    CHECK_ADEC_NULL_PTR(pstStatusinfo);

    pstAdecChan = &g_stAdecSlimChan[hAdec];

    pstOutBuf = &(pstAdecChan->outStreamBuf);

    pstStatusinfo->bWorking = HI_TRUE;
    pstStatusinfo->u32OutBufNum = pstOutBuf->u32OutBufNum;
    if (pstOutBuf->u32BufWriteIdx >= pstOutBuf->u32BufReadIdx)
    {
        pstStatusinfo->u32UsedBufNum = pstOutBuf->u32BufWriteIdx - pstOutBuf->u32BufReadIdx;
    }
    else
    {
        pstStatusinfo->u32UsedBufNum = pstOutBuf->u32OutBufNum - (pstOutBuf->u32BufReadIdx - pstOutBuf->u32BufWriteIdx);
    }

    pstInBuf = &(pstAdecChan->InStreamBuf);
    pstStatusinfo->u32BufferSize = pstInBuf->u32BufSize;
    pstStatusinfo->u32BufferUsed = pstInBuf->u32BufSize - pstInBuf->u32BufFree;
    pstStatusinfo->u32BufferAvailable = pstInBuf->u32BufFree;
    pstStatusinfo->u32TotDecodeFrame = pstAdecChan->u32TotDecodeFrame;
    if (pstAdecChan->enSampleRate)
    {
        pstStatusinfo->u32OutBufDurationMs = (pstAdecChan->u32PcmSamplesPerFrame * 1000
                                              * pstStatusinfo->u32UsedBufNum) / ((HI_U32)pstAdecChan->enSampleRate);
        pstStatusinfo->u32FrameDurationMs = (pstAdecChan->u32PcmSamplesPerFrame
                                             * 1000) / ((HI_U32)pstAdecChan->enSampleRate);
    }

    pstStatusinfo->u32CodecID   = (HI_U32)pstAdecChan->enCodecType;
    pstStatusinfo->u32Channels  = 2;
    pstStatusinfo->enSampleRate = pstAdecChan->enSampleRate;
    pstStatusinfo->enBitDepth = 16;

    return HI_SUCCESS;
}

#endif

HI_S32 HI_DRV_ADEC_Init(HI_VOID)
{
    HI_S32 ret;

    ret = HI_DRV_MODULE_Register(HI_ID_ADEC, ADEC_NAME, (HI_VOID*)HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_ADEC("HI_DRV_MODULE_Register Adec failed\n");
        return ret;
    }

    ret = HI_DRV_MODULE_Register(HI_ID_KARAOKE, "HI_KARAOKE", (HI_VOID*)HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_ADEC("HI_DRV_MODULE_Register Adec failed\n");
        return ret;
    }

    return HI_SUCCESS;
}

HI_VOID  HI_DRV_ADEC_DeInit(HI_VOID)
{
    HI_DRV_MODULE_UnRegister(HI_ID_ADEC);
    return;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
