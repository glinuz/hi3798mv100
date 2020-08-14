/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hal_aiao.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2012-09-22   z40717     NULL         init.
 ********************************************************************************/
#include <linux/string.h>

#include "hi_type.h"
#include "hi_module.h"
#include "hi_drv_mem.h"
#include "hal_cast.h"
#include "hi_drv_struct.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"
#include "hi_drv_mem.h"
#include "hi_drv_module.h"

#include "hi_drv_ao.h"
#include "hi_audsp_common.h"
#include "circ_buf.h"
#include "hal_aoe.h"
#include "hal_cast.h"
#include "audio_util.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct
{
    HI_VOID* pCast[AIAO_CAST_BUTT];
} CAST_GLOBAL_SOURCE_S;

typedef struct
{
    HI_U32 uTotalByteWrite;
    HI_U32 uTryWriteCnt;
    HI_U32 uTotalByteRead;
    HI_U32 uTryReadCnt;
} CAST_PROC_STATUS_S;

typedef struct
{
    HI_U32 u32BufPhyAddr;  // hw aoe
    HI_U32 u32BufPhyWptr;  // hw aoe
    HI_U32 u32BufPhyRptr;  // hw aoe
    HI_U32 u32BufVirAddr;  // sw aoe
    HI_U32 u32BufVirWptr;  // sw aoe
    HI_U32 u32BufVirRptr;  // sw aoe
    HI_U32 u32BufSize;
} CAST_VIRTUAL_RBUF_S;

typedef struct
{
    AIAO_CAST_ATTR_S stUserAttr;

    /* internal state */
    AIAO_CAST_ID_E     enCast;
    HI_U32             u32BufFrameSize;
    AIAO_CAST_STATUS_E enCurnStatus;
    CIRC_BUF_S         stCB;
    CAST_PROC_STATUS_S stProc;
} CAST_CHN_STATE_S;

/* private state */
static CAST_GLOBAL_SOURCE_S g_CastRm;

#define CHECK_CAST_OPEN(Id) \
    do {\
        if (HI_NULL == g_CastRm.pCast[Id])\
        {\
            HI_ERR_AO("CAST(%d) is not create.\n", Id); \
            return HI_FAILURE; \
        } \
    } while (0)

static HI_VOID CASTFlushState(CAST_CHN_STATE_S* state)
{
    memset(&state->stProc, 0, sizeof(CAST_PROC_STATUS_S));
    CIRC_BUF_Flush(&state->stCB);
}

static AIAO_CAST_ID_E GetFreeCAST(HI_VOID)
{
    AIAO_CAST_ID_E enFreeCast;

    for (enFreeCast = AIAO_CAST_0; enFreeCast < AIAO_CAST_BUTT; enFreeCast++)
    {
        if (!g_CastRm.pCast[enFreeCast])
        {
            return enFreeCast;
        }
    }

    return AIAO_CAST_BUTT;
}

static HI_U32 UTIL_CalcFrameSize(HI_U32 uCh, HI_U32 uBitDepth)
{
    HI_U32 uFrameSize = 0;

    switch (uBitDepth)
    {
        case 16:
            uFrameSize = ((HI_U32)uCh) * sizeof(HI_U16);
            break;
        case 24:
            uFrameSize = ((HI_U32)uCh) * sizeof(HI_U32);
            break;
    }

    return uFrameSize;
}

/* global function */
HI_S32 HAL_CAST_Init(HI_VOID)
{
    AIAO_CAST_ID_E enCast;

    /* init rm */
    for (enCast = AIAO_CAST_0; enCast < AIAO_CAST_BUTT; enCast++)
    {
        g_CastRm.pCast[enCast] = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_VOID HAL_CAST_DeInit(HI_VOID)
{
    AIAO_CAST_ID_E enCast;

    /* deinit rm */
    for (enCast = AIAO_CAST_0; enCast < AIAO_CAST_BUTT; enCast++)
    {
        if (g_CastRm.pCast[enCast])
        {
            HAL_CAST_Destroy(enCast);
        }

        g_CastRm.pCast[enCast] = HI_NULL;
    }
}


HI_S32 HAL_CAST_Create(AIAO_CAST_ID_E* penCAST, AIAO_CAST_ATTR_S* pstAttr)
{
    CAST_CHN_STATE_S* state = HI_NULL;
    AIAO_CAST_ID_E enCAST;
    HI_S32 Ret = HI_FAILURE;

    // todo , check attr
    if (HI_NULL == pstAttr)
    {
        HI_FATAL_AO("pstAttr is null\n");
        return HI_FAILURE;
    }
    if (!pstAttr->extDmaMem.tBufPhyAddr || !pstAttr->extDmaMem.tBufVirAddr)
    {
        HI_FATAL_AO("BufPhyAddr(0x%x) BufVirAddr(0x%x) invalid\n", pstAttr->extDmaMem.tBufPhyAddr, pstAttr->extDmaMem.tBufVirAddr);
        return HI_FAILURE;
    }
    if (pstAttr->extDmaMem.u32BufSize < AIAO_CAST_BUFSIZE_MIN)
    {
        HI_FATAL_AO("BufSize(0x%x) less than MinSize(0x%x) invalid\n", pstAttr->extDmaMem.u32BufSize, AIAO_CAST_BUFSIZE_MIN);
        return HI_FAILURE;
    }

    enCAST = GetFreeCAST();
    if (AIAO_CAST_BUTT == enCAST)
    {
        HI_FATAL_AO("CAST source is not enough\n");
        return HI_FAILURE;
    }

    state = AUTIL_AO_MALLOC(HI_ID_AO, sizeof(CAST_CHN_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("malloc CAST_CHN_STATE_S failed\n");
        goto CAST_Create_ERR_EXIT;
    }

    memset(state, 0, sizeof(CAST_CHN_STATE_S));
    g_CastRm.pCast[enCAST] = (HI_VOID*)state;

    if (HI_SUCCESS != (Ret = HAL_CAST_SetAttr(enCAST, pstAttr)))
    {
        goto CAST_Create_ERR_EXIT;
    }

    state->enCurnStatus = AIAO_CAST_STATUS_STOP;
    state->enCast = enCAST;
    *penCAST = enCAST;
    return HI_SUCCESS;

CAST_Create_ERR_EXIT:
    *penCAST = AIAO_CAST_BUTT;
    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    g_CastRm.pCast[enCAST] = HI_NULL;
    return Ret;
}

HI_VOID HAL_CAST_Destroy(AIAO_CAST_ID_E enCAST)
{
    CAST_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_CastRm.pCast[enCAST])
    {
        return;
    }

    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_STOP != state->enCurnStatus)
    {
        HAL_CAST_Stop(enCAST);
    }

    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    g_CastRm.pCast[enCAST] = HI_NULL;
    return;
}

HI_S32 HAL_CAST_SetAttr(AIAO_CAST_ID_E enCAST, AIAO_CAST_ATTR_S* pstAttr)
{
    CAST_CHN_STATE_S* state = HI_NULL;

    CHECK_CAST_OPEN(enCAST);

    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];
    if (AIAO_CAST_STATUS_STOP != state->enCurnStatus)
    {
        return HI_FAILURE;
    }

    if (pstAttr->extDmaMem.u32BufSize < AIAO_CAST_BUFSIZE_MIN)
    {
        return HI_FAILURE;
    }

    CIRC_BUF_Init(&state->stCB,
                  (HI_U32*)(pstAttr->extDmaMem.tWptrAddr),
                  (HI_U32*)(pstAttr->extDmaMem.tRptrAddr),
                  (HI_U32*)(pstAttr->extDmaMem.tBufVirAddr),
                  pstAttr->extDmaMem.u32BufSize);

    state->u32BufFrameSize = UTIL_CalcFrameSize(pstAttr->u32BufChannels,
                             pstAttr->u32BufBitPerSample);

    memset(&state->stProc, 0, sizeof(CAST_PROC_STATUS_S));
    memcpy(&state->stUserAttr, pstAttr, sizeof(AIAO_CAST_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 HAL_CAST_GetAttr(AIAO_CAST_ID_E enCAST, AIAO_CAST_ATTR_S* pstAttr)
{
    CAST_CHN_STATE_S* state = HI_NULL;

    CHECK_CAST_OPEN(enCAST);

    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    memcpy(pstAttr, &state->stUserAttr, sizeof(AIAO_CAST_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 HAL_CAST_Start(AIAO_CAST_ID_E enCAST)
{
    CAST_CHN_STATE_S* state = HI_NULL;

    CHECK_CAST_OPEN(enCAST);
    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_START == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    state->enCurnStatus = AIAO_CAST_STATUS_START;
    return HI_SUCCESS;
}

HI_S32 HAL_CAST_Stop(AIAO_CAST_ID_E enCAST)
{
    CAST_CHN_STATE_S* state = HI_NULL;

    CHECK_CAST_OPEN(enCAST);
    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_STOP == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    CASTFlushState(state);
    state->enCurnStatus = AIAO_CAST_STATUS_STOP;
    return HI_SUCCESS;
}

HI_U32 HAL_CAST_QueryBufData(AIAO_CAST_ID_E enCAST)
{
    CAST_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_CastRm.pCast[enCAST])
    {
        HI_ERR_AO("CAST(%d) is not create.\n", enCAST);
        return 0 ;
    }
    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_STOP == state->enCurnStatus)
    {
        return 0;
    }

    return CIRC_BUF_QueryBusy(&state->stCB);
}

HI_U32 HAL_CAST_QueryBufFree(AIAO_CAST_ID_E enCAST)
{
    CAST_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_CastRm.pCast[enCAST])
    {
        HI_ERR_AO("CAST(%d) is not create.\n", enCAST);
        return 0 ;
    }
    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_STOP == state->enCurnStatus)
    {
        return 0;
    }

    return CIRC_BUF_QueryFree(&state->stCB);
}

HI_U32 HAL_CAST_ReadData(AIAO_CAST_ID_E enCAST, HI_U32* pu32DataOffset, HI_U32 u32DestSize)
{
    CAST_CHN_STATE_S* state = HI_NULL;
    HI_U32 Bytes;

    if (HI_NULL == g_CastRm.pCast[enCAST])
    {
        HI_ERR_AO("CAST(%d) is not create.\n", enCAST);
        return HI_FAILURE;
    }
    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_STOP == state->enCurnStatus)
    {
        return 0;
    }

    state->stProc.uTryReadCnt++;
    Bytes = CIRC_BUF_CastRead(&state->stCB, pu32DataOffset, u32DestSize); //just read not update readptr
    state->stProc.uTotalByteRead += Bytes;
    return Bytes;
}

HI_U32  HAL_CAST_ReleaseData(AIAO_CAST_ID_E enCAST, HI_U32 u32DestSize)
{
    CAST_CHN_STATE_S* state = HI_NULL;
    HI_U32 Bytes;

    if (HI_NULL == g_CastRm.pCast[enCAST])
    {
        HI_ERR_AO("CAST(%d) is not create.\n", enCAST);
        return 0 ;
    }
    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_STOP == state->enCurnStatus)
    {
        return 0;
    }

    state->stProc.uTryReadCnt++;
    Bytes = CIRC_BUF_CastRelese(&state->stCB, u32DestSize); //just update readptr not read
    state->stProc.uTotalByteRead += Bytes;
    return Bytes;
}

HI_U32 HAL_CAST_WriteBufData(AIAO_CAST_ID_E enCAST, HI_U8* pu32Src, HI_U32 u32SrcBytes)
{
    CAST_CHN_STATE_S* state = HI_NULL;
    HI_U32 Bytes;

    if (HI_NULL == g_CastRm.pCast[enCAST])
    {
        HI_ERR_AO("CAST(%d) is not create.\n", enCAST);
        return 0 ;
    }
    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_STOP == state->enCurnStatus)
    {
        return 0;
    }

    state->stProc.uTryWriteCnt++;
    Bytes = CIRC_BUF_Write(&state->stCB, pu32Src, u32SrcBytes);
    state->stProc.uTotalByteWrite += Bytes;
    return Bytes;
}

HI_VOID HAL_CAST_GetBufDelayMs(AIAO_CAST_ID_E enCAST, HI_U32* pDelayms)
{
    CAST_CHN_STATE_S* state = HI_NULL;
    HI_U32 FreeBytes = 0;

    if (HI_NULL == g_CastRm.pCast[enCAST])
    {
        *pDelayms = 0;
        return;
    }

    state = (CAST_CHN_STATE_S*)g_CastRm.pCast[enCAST];

    if (AIAO_CAST_STATUS_STOP == state->enCurnStatus)
    {
        *pDelayms = 0;
        return;
    }

    FreeBytes = CIRC_BUF_QueryBusy(&state->stCB);
    *pDelayms = CALC_LATENCY_MS(state->stUserAttr.u32BufSampleRate, state->u32BufFrameSize, FreeBytes);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
