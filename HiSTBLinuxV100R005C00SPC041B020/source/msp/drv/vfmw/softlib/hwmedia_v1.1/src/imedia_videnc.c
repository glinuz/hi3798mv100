/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCO4ir2/uQ7BFDYEYESXdzFFpPTRP1CkEuTQUs7
G00Y4bnYd3t11cbROYiWVwrxuCW6J9WxdL0HXvpmSJzU22Jjatt/F4DDr61zB59x+ScjgJWH
DihEKH3pIj2p8T/vOXtx81cR5JGqOg5tCS7Td+EnwfwFns2TWzYOTziSz+yiqg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/************************************************************************
* Copyright(C), 2008-2013, Huawei Tech. Co., Ltd.
* All rights reserved.
*
* $Id: imedia_viddec.c,v 1.138 2010/01/12 09:39:22 songxiaogang Exp $
*
* Description：
*
* Reference Doc：
*
* Modification：//见文件尾
*
*************************************************************************/
#include <stdarg.h>
#include <fcntl.h>

#include "iMedia_error.h"
#include "iMedia_videnc.h"
#include "iMedia_util.h"
#include "quality_params.h"

#include "IHW264Enc.h"

#if defined(_MSC_VER)
#include <Shlwapi.h>
#elif defined(__GNUC__)
#include <dlfcn.h>
#endif

#define MAX_SCALE          64
#define MIN_FRAME_RATE     5
#define MAX_FRAME_RATE     120
#define DEFAULT_FRAME_RATE 25
#define MAX_BITRATE_KBPS   30000

#define FRAME_MASK_DROP   0
#define FRAME_MASK_ENCODE 1
#define FRAME_MASK_SKIP   2

typedef HW264E_RETURN (*funHW264E_Create)(IHEncoder *hhEncoder, const IHW264E_STATIC_PARAM_BASE *pstBaseParam, const IHW264E_STATIC_PARAM_CUSTOM *pstCustomParam, BOOL bOptimize, UINT32 uChannelID);
typedef HW264E_RETURN (*funHW264E_Control)(IHEncoder hEncoder, const IHW264E_DYN_PARAM_BASE *pstBaseParam, const IHW264E_DYN_PARAM_CUSTOM *pstCustomParam);
typedef HW264E_RETURN (*funHW264E_Encode)(IHEncoder hEncoder, const IHW264E_YUV *pstYUV, IHW264E_STREAM *pstStream, INT32 iFrmType, INT32 iStride);
typedef HW264E_RETURN (*funHW264E_Delete)(IHEncoder hEncoder);
typedef HW264E_RETURN (*funHW264E_GetEncStatus)(IHEncoder hEncoder, void *pstStatus, HW264E_STAUS_CMD eCmd);
typedef HW264E_RETURN (*funHW264E_GetSpsPps)(IHEncoder hEncoder, IHW264E_STREAM *pstStream);

typedef enum ENUM_IVIDENC_ENCODE_TYPE
{
    ENCODE_NORMAL = 0,
    ENCODE_TRICK_MODE,
} ENUM_IVIDENC_ENCODE_TYPE;

// 正常编码器上下文
typedef struct STRU_H264_CTX
{
    int handle;
    funHW264E_Create       pfnCreate;
    funHW264E_Control      pfnControl;
    funHW264E_Encode       pfnEncode;
    funHW264E_Delete       pfnDelete;
    funHW264E_GetEncStatus pfnGetEncStatus;
    funHW264E_GetSpsPps    pfnGetSpsPps;
} STRU_H264_CTX;

// 编码通道上下文，与外部接口
typedef struct STRU_VIDEO_ENCODE_CTX
{
    INT32 iCrcMask;                     // 防错码
    ENUM_IVIDEO_CODEC_STATUS eStatus;   // 状态
    ENUM_IVIDENC_ENCODE_TYPE eType;     // 当前编码类型
    void   *pstEncoder;                 // H264编码器句柄
    STRU_H264_CTX stModule;             // 编码器动态库模块
    UINT32 uiFrameMaxSize;              // 帧的最大尺寸
    UINT8  *pFrameStreamBuf;            // 存储临时数据的帧缓存
    STRU_IVIDENC_PARAMS   stParams;     // 存储配置静态参数
    STRU_IVIDENC_DYNAMIC_PARAMS stDynParams;     // 存储配置动态参数
    STRU_IVIDENC_QUALITY_PARAMS stQualityParams; // 存储质量控制参数

    UINT32  uiFrameDropBase;        // 丢帧基准，最大可为60 x 32
    UINT8   *pucFrameDropMask;      // 丢帧掩码表

    // 用于状态更新
    UINT32  uiTotalInputFrames;     // 输入总帧数
    UINT32  uiTotalOutputFrames;    // 输出总帧数
    UINT32  uiTotalDropFrames;      // 总共丢弃的帧数
    UINT32  uiTotalErrors;          // 总共发生错误数
    UINT64  uiTotalPSNR_Y;          // 总的PSNR_Y
    UINT32  uiPSNRFrames;           // 计算PSNR_Y的总帧数
    FLOAT32 fCurFramePSNR_Y;        // 当前帧的PSNR_Y
    BOOL    bEncodeLastFrameFlag;   // 是否已编码最后一帧，由于内部编码不支持编完最后一帧后再输入新YUV的功能，接口层需屏蔽

    // 用于性能统计字段
    UINT64  ullTotalSpendTime;      // 总共处理时间
    UINT64  ullPrevSpendTime;       // 上一次统计总共用的时间
    UINT32  uiPrevOutputFrames;     // 输出总帧数
} STRU_VIDEO_ENCODE_CTX;

static void HW264E_FreeLibrary(STRU_H264_CTX *pModule)
{
    if (!pModule->handle)
    { return; }

    IMEDIA_LOG(IMEDIA_INFO, "FreeLibrary %d\n", pModule->handle);

#if defined(_MSC_VER)
    FreeLibrary((HMODULE)pModule->handle);
#elif defined(__GNUC__)
    dlclose(pModule->handle);
#endif

    memset(pModule, 0, sizeof(STRU_H264_CTX));
}

static INT32 HW264E_LoadLibrary(STRU_H264_CTX *pModule, BOOL bTrickMode)
{
    char szModName[64] = { 0 };

#if defined(_MSC_VER)
    strncpy(szModName, bTrickMode ? "HW264EncTrick.dll" : "H264Encoder.dll", 64);
    pModule->handle = (int)LoadLibrary(szModName);
#elif defined(__GNUC__)
    strncpy(szModName, bTrickMode ? "libHW264EncTrick.so" : "libHW264Enc.so", 64);
    pModule->handle = dlopen(szModName, RTLD_NOW);
#endif

    if (!pModule->handle)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "LoadLibrary %s failed\n", szModName);
        return IMEDIA_RET_FILE_OPEN_FAIL;
    }

    IMEDIA_LOG(IMEDIA_INFO, "LoadLibrary %s succeed\n", szModName);

#if defined(_MSC_VER)
    pModule->pfnCreate  = (funHW264E_Create)GetProcAddress((HMODULE)pModule->handle, "HW264E_Create");
    pModule->pfnControl = (funHW264E_Control)GetProcAddress((HMODULE)pModule->handle, "HW264E_Control");
    pModule->pfnEncode  = (funHW264E_Encode)GetProcAddress((HMODULE)pModule->handle, "HW264E_Encode");
    pModule->pfnDelete  = (funHW264E_Delete)GetProcAddress((HMODULE)pModule->handle, "HW264E_Delete");
    pModule->pfnGetEncStatus = (funHW264E_GetEncStatus)GetProcAddress((HMODULE)pModule->handle, "HW264E_GetEncStatus");
    pModule->pfnGetSpsPps = (funHW264E_GetSpsPps)GetProcAddress((HMODULE)pModule->handle, "HW264E_GetSpsPps");
#else
    pModule->pfnCreate  = (funHW264E_Create)dlsym(pModule->handle, "HW264E_Create");
    pModule->pfnControl = (funHW264E_Control)dlsym(pModule->handle, "HW264E_Control");
    pModule->pfnEncode  = (funHW264E_Encode)dlsym(pModule->handle, "HW264E_Encode");
    pModule->pfnDelete  = (funHW264E_Delete)dlsym(pModule->handle, "HW264E_Delete");
    pModule->pfnGetEncStatus = (funHW264E_GetEncStatus)dlsym(pModule->handle, "HW264E_GetEncStatus");
    pModule->pfnGetSpsPps = (funHW264E_GetSpsPps)dlsym(pModule->handle, "HW264E_GetSpsPps");
#endif

    if (!pModule->pfnCreate || !pModule->pfnControl || !pModule->pfnEncode || !pModule->pfnDelete || !pModule->pfnGetEncStatus)
    {
        HW264E_FreeLibrary(pModule);
        IMEDIA_LOG(IMEDIA_ERROR, "GetProcAddress failed\n");
        return IMEDIA_RET_FILE_OTHER;
    }

    return IMEDIA_RET_SUCCESS;
}

// 获取采样帧间隔
// 计算公式 (ref_frames / ref_framerate) = scale * (dst_frames / dst_framerate)
static void CalcTrickmodeCaptureInterval(int scale, int ref_framerate, int dst_framerate, int *ref_frames, int *dst_frames)
{
    int i, j;

    for (i = 1; i <= dst_framerate; i++)
    {
        j = i * scale * ref_framerate;

        if (0 == (j % dst_framerate))
        {
            *dst_frames = i;
            *ref_frames = j / dst_framerate;
            break;
        }
    }
}

// 丢帧计算公式
// ifr - input frames 5
// ofr - output frames 3
static int CalcFrameSkipPattern(int ifr, int ofr, unsigned char pSkipPattern[], unsigned char defaultValue, unsigned char maskValue)
{
    //  ifr=fl*gl+fs*gs    //fl, gl, fs, gs
    int fs = (int)(ifr / ofr);     // ifr ---- ofr-- // 1
    int fl = fs + 1; // 2
    int gl = ifr - fs * ofr; // delta = 2
    int gs = ofr - gl; // 1
    int i, j;
    int maxgap = 0;
    int ming = gl > gs ? gs : gl; // 1

    j = -1;

    memset(pSkipPattern, defaultValue, ifr * sizeof(unsigned char));

    for (i = 0; i < ming; ++i)
    {
        j += fl; // 1
        pSkipPattern[j] = maskValue;
        j += fs;
        pSkipPattern[j] = maskValue;
    }

    if (gl > gs)
    {
        for (i = 0; i < gl - gs; ++i)
        {
            j += fl;

            if (j >= ifr) { break; }

            pSkipPattern[j] = maskValue;
        }
    }

    if (gs > gl)
    {
        for (i = 0; i < gs - gl; ++i)
        {
            j += fs;

            if (j >= ifr) { break; }

            pSkipPattern[j] = maskValue;
        }
    }

    j = -1;

    for (i = 1; i < ifr; ++i)
    {
        if (maskValue == pSkipPattern[i])
        {
            j = i - j - 1;

            if (maxgap < j) { maxgap = j; }

            j = i;
        }
    }

    return maxgap;
}
/*
static double psnr( INT64 iSsd, INT64 iSize )
{
    double f_mse = (double)iSsd / iSize;
    if (f_mse > 0.0000000001)
    {
        f_mse = 65025.0 / f_mse;
        f_mse = 10 * (FLOAT32)log10(f_mse);
    }

    return f_mse;
}*/

static double psnr( INT64 iSsd, INT64 iSize )
{
    double f_mse = (double)iSsd / ((double)65025.0 * (double)iSize);

    if ( f_mse <= 0.0000000001 )  //Max 100dB
    { return 100.0; }

    return (double)(-10.0 * log( f_mse ) / log( 10.0 ));
}

// 计算PSNR
static void CalculatePSNR(const IHW264E_YUV *pstOrig, const IHW264E_YUV *pstRecon,
                          INT32 iWidth, INT32 iHeight, INT32 iOrigStride, INT32 iReconStride, UINT32 uChannelID)

{
    const UINT8 *pDY = pstOrig->pucY;
    const UINT8 *pDU = pstOrig->pucU;
    const UINT8 *pDV = pstOrig->pucV;
    const UINT8 *pEY = pstRecon->pucY;
    const UINT8 *pEU = pstRecon->pucU;
    const UINT8 *pEV = pstRecon->pucV;

    int i, j;
    INT32 iDiff;
    INT64 iSsd[3] = {0};  //sum of square error
    //double dPsnr[3] = {0.0};
    //INT32 iHalfStrideE = iReconStride >> 1;
    //INT32 iHalfStrideD = iOrigStride >> 1;
    //INT32 iHalfHeight = iHeight >> 1;
    //INT32 iHalfWidth = iWidth >> 1;
    INT64 iSize = (INT64)iWidth * (INT64)iHeight;
    STRU_VIDEO_ENCODE_CTX *pstEncCtx = (STRU_VIDEO_ENCODE_CTX *)uChannelID;

    if (!pstEncCtx)
    {
        return;
    }

    if (!pDY || !pDU || !pDV || !pEY || !pEU || !pEV)
    {
        return;
    }

    // Y ssd
    for (j = 0; j < iHeight; j++)
    {
        for (i = 0; i < iWidth; i++)
        {
            iDiff = pEY[j * iReconStride + i] - pDY[j * iOrigStride + i];
            iSsd[0] += (INT64)iDiff * (INT64)iDiff;
        }
    }

    // UV ssd
    //for (j = 0; j < iHalfHeight; j++)
    //{
    //    for (i = 0; i < iHalfWidth; i++)
    //    {
    //        iDiff = pEU[j * iHalfStrideE + i] - pDU[j * iHalfStrideD + i];    //U dif
    //        iSsd[1] += (INT64)iDiff * (INT64)iDiff;

    //        iDiff = pEV[j * iHalfStrideE + i] - pDV[j * iHalfStrideD + i];    //V dif
    //        iSsd[2] += (INT64)iDiff * (INT64)iDiff;
    //    }
    //}

    //psnr for each frame
    //for(i = 0; i < 3; i++)
    //{
    //    g_sEncRun.stSeqStat.dPsnr[i] = i > 0 ? psnr(iSsd[i], iSize >> 2) : psnr(iSsd[i], iSize);
    //    g_sEncRun.stSeqStat.dPsnrMean[i] += g_sEncRun.stSeqStat.dPsnr[i];
    //}

    ////psnr for meaning
    //g_sEncRun.stSeqStat.dPsnrAvg += psnr(iSsd[0] + iSsd[1] + iSsd[2], iSize * 3 / 2);
    //g_sEncRun.stSeqStat.dSsdGlobal += (iSsd[0] + iSsd[1] + iSsd[2]);

    pstEncCtx->fCurFramePSNR_Y = (FLOAT32)psnr(iSsd[0], iSize);
    pstEncCtx->uiTotalPSNR_Y += (UINT64)(pstEncCtx->fCurFramePSNR_Y * 100);
    pstEncCtx->uiPSNRFrames++;
}

#define CHECK_VALUE(v, dst, ret) {if ((dst) != (v)) { return ret; }}
#define CHECK_VALUE_MIN(v, min_v, ret) {if ((v) < (min_v)) { return ret; }}
#define CHECK_VALUE_MAX(v, max_v, ret) {if ((v) > (max_v)) { return ret; }}
#define CHECK_VALUE_RANGE(v, min_v, max_v, ret) {if ((v) < (min_v) || (v) > (max_v)) { return ret; }}

// 该函数后续还需要调整
static INT32 H264_CheckMaxParams(STRU_IVIDENC_PARAMS *pstParams)
{
    /*lint -e685*/
    CHECK_VALUE_RANGE(pstParams->eProfile, IVIDEO_PROFILE_H264_BASELINE, IVIDEO_PROFILE_H264_HIGH, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstParams->eQualityLevel, QUALITY_LEVEL_HIGH_SPEED, QUALITY_LEVEL_CUSTOM, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstParams->eContentType, IVIDEO_PROGRESSIVE, IVIDEO_AUTO, IMEDIA_RET_PARAM_IN_VALUE_INVALID);

    CHECK_VALUE_RANGE(pstParams->usMaxWidth, IVIDEO_MIN_WIDTH, IVIDEO_MAX_WIDTH, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstParams->usMaxHeight, IVIDEO_MIN_HEIGHT, IVIDEO_MAX_HEIGHT, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_MAX(pstParams->uiEncScale, MAX_SCALE, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstParams->uiMaxKeyframeInterval, 1, IVIDENC_MAX_KEY_INTERVAl, IMEDIA_RET_PARAM_IN_VALUE_INVALID);

    CHECK_VALUE_RANGE(pstParams->uiMaxRefFrameNum, 1, IVIDENC_MAX_REF_FRAMES, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_MAX(pstParams->uiMaxBFrameNum, IVIDENC_MAX_B_FRAMES, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstParams->uiMaxMultipass, 1, 2, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    /*lint +e685*/

    return IMEDIA_RET_SUCCESS;
}

static INT32 H264_CheckDynParams(STRU_IVIDENC_PARAMS *pstParams, STRU_IVIDENC_DYNAMIC_PARAMS *pstDynParams)
{
    CHECK_VALUE_RANGE(pstDynParams->usWidth, IVIDEO_MIN_WIDTH, pstParams->usMaxWidth, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstDynParams->usHeight, IVIDEO_MIN_HEIGHT, pstParams->usMaxHeight, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstDynParams->uiInputRefFrameRate, MIN_FRAME_RATE * 1000, MAX_FRAME_RATE * 1000, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstDynParams->uiFrameRate, MIN_FRAME_RATE * 1000, pstDynParams->uiInputRefFrameRate, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstDynParams->uiBitrate, 10, MAX_BITRATE_KBPS, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstDynParams->uiVbvBufDelay, 500, 30000, IMEDIA_RET_PARAM_IN_VALUE_INVALID);
    CHECK_VALUE_RANGE(pstDynParams->uiKeyframeInterval, 1, pstParams->uiMaxKeyframeInterval, IMEDIA_RET_PARAM_IN_VALUE_INVALID);

    return IMEDIA_RET_SUCCESS;
}

static const char *GetFrameTypeString(INT32 iType)
{
    static const char *g_szFrameType[] = {"P", "B", "I", "SI", "SP", "IDR", "BREF"};

    if (iType >= IVIDEO_FRAME_P && iType <= IVIDEO_FRAME_BREF)
    { return (char *)g_szFrameType[iType]; }

    return "UNKNOWN";
}

// 获取算法通道的静态参数
static INT32 GetStaticParams(STRU_VIDEO_ENCODE_CTX *pstEncCtx, STRU_IVIDENC_PARAMS *params)
{
    // 检查相关参数
    if (NULL == params)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    memset(params, 0, sizeof(STRU_IVIDENC_PARAMS));

    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input pstEncCtx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    memcpy(params, &pstEncCtx->stParams, sizeof(STRU_IVIDENC_PARAMS));

    return IMEDIA_RET_SUCCESS;
}

// 获取算法通道的动态参数
static INT32 GetDynamicParams(STRU_VIDEO_ENCODE_CTX *pstEncCtx, STRU_IVIDENC_DYNAMIC_PARAMS *params)
{
    // 检查相关参数
    if (NULL == params)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    memset(params, 0, sizeof(STRU_IVIDENC_DYNAMIC_PARAMS));

    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input pstEncCtx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    if (IVIDEO_CODEC_RUNNING > pstEncCtx->eStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] eStatus[%d] < RUNNING\n", pstEncCtx, pstEncCtx->eStatus);
        return IMEDIA_RET_OBJ_NOT_INIT;
    }

    memcpy(params, &pstEncCtx->stDynParams, sizeof(STRU_IVIDENC_DYNAMIC_PARAMS));

    return IMEDIA_RET_SUCCESS;
}

// 获取序列头图像头NALU码流
static INT32 GetSpsPps(STRU_VIDEO_ENCODE_CTX *pstEncCtx, STRU_H264_FRAME_STREAM *params)
{
    INT32 i = 0;
    INT32 iRet = IMEDIA_RET_SUCCESS;
    IHW264E_STREAM stStream = { 0 };

    // 检查相关参数
    if (NULL == params)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    memset(params, 0, sizeof(STRU_H264_FRAME_STREAM));

    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input pstEncCtx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    if (IVIDEO_CODEC_RUNNING > pstEncCtx->eStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] eStatus[%d] < RUNNING\n", pstEncCtx, pstEncCtx->eStatus);
        return IMEDIA_RET_OBJ_NOT_INIT;
    }

    if (NULL == pstEncCtx->stModule.pfnGetSpsPps)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] GetProcAddress <GetSpsPps> failed\n", pstEncCtx, pstEncCtx->eStatus);
        return IMEDIA_RET_OBJ_NOT_FOUND;
    }

    stStream.pucStream = pstEncCtx->pFrameStreamBuf;
    stStream.iBufLen = pstEncCtx->uiFrameMaxSize;

    iRet = pstEncCtx->stModule.pfnGetSpsPps(pstEncCtx->pstEncoder, &stStream);

    params->pucStream  = stStream.pucStream;
    params->uiDataSize = (UINT32)stStream.iDataLen;
    params->uiFrameID = 0;//(UINT32)pstEncCtx->uiTotalOutputFrames++;
    params->uiDisplayID = 0;//(UINT32)stStream.iDisplayNum;
    params->bSkipFlag = 0;//stStream.bSkipFrm;
    params->usNaluCount = (UINT16)stStream.iNalNum;

    if (MAX_NALU_COUNT_OF_FRAME < params->usNaluCount)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "enc_ctx[%p] out NaluCount %u > %u\n", pstEncCtx, (UINT32)params->usNaluCount, MAX_NALU_COUNT_OF_FRAME);
        params->usNaluCount = MAX_NALU_COUNT_OF_FRAME;
    }

    for (i = 0; i < params->usNaluCount; i++)
    {
        params->stNalu[i].uiSize = (UINT32)stStream.stNAL[i].iNalSize;
        params->stNalu[i].pucData = stStream.stNAL[i].pucNalData;
        params->stNalu[i].eType = (ENUM_H264_NALU_TYPE)stStream.stNAL[i].eNaluType;
        params->stNalu[i].ePriority = (ENUM_H264_NALU_PRIORITY)stStream.stNAL[i].eNaluPriority;
    }

    return iRet;
}

/*// 获取算法通道的质量控制参数
static INT32 GetQualityParams(STRU_VIDEO_ENCODE_CTX* pstEncCtx, STRU_IVIDENC_QUALITY_PARAMS* pstParams)
{
    // 检查相关参数
    if (NULL == pstParams)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input pstEncCtx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    // 当前算法通道的质量控制参数
    memcpy(pstParams, &pstEncCtx->stQualityParams, sizeof(STRU_IVIDENC_QUALITY_PARAMS));

    return IMEDIA_RET_SUCCESS;
}*/

// 从配置文件的内存映像中获取质量控制参数
static INT32 GetQualityParamsFromMemory(STRU_IVIDENC_QUALITY_PARAMS *pstParams, char *text)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;
    IMEDIA_INI_FILE pstIniHandle = NULL;

    // 检查参数
    if (NULL == pstParams)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == text)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input text NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (strlen(text) < 3)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input text buffer invalid\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    // 分析文件和内容
    iRet = IMedia_Ini_OpenFromMemory(text, &pstIniHandle);

    if (IMEDIA_RET_SUCCESS != iRet)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "IMedia_Ini_OpenFromMemory(%p) = %d\n", text, iRet);
        return iRet;
    }

    // 逐个参数读取
    pstParams->eLevel = (ENUM_IVIDEO_CODEC_LEVEL)IMedia_Ini_GetInteger(pstIniHandle, "LevelIDC", 31);
    pstParams->ePicCodingType = (ENUM_IVIDENC_PICTURE_CODING_TYPE)IMedia_Ini_GetInteger(pstIniHandle, "InterlaceType", PIC_CODING_FRAME);
    pstParams->iMultiPass = IMedia_Ini_GetInteger(pstIniHandle, "MultiPass", 2);

    pstParams->bCabacFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "SymbolMode", 0);
    pstParams->bDCT8x8Flag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "Transform8x8", 0);

    /**< 码控相关参数 */
    pstParams->uiVbvBufDelay = (UINT32)IMedia_Ini_GetInteger(pstIniHandle, "VbvBufDelay", 1000);
    pstParams->bSkipFrameFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "SkipEnable", 0);
    pstParams->iChromaQPOffset = IMedia_Ini_GetInteger(pstIniHandle, "ChromaQPOffset", 0);
    pstParams->iSecondChromaQPOffset = IMedia_Ini_GetInteger(pstIniHandle, "SecondChrQPOffset", 0);

    /**< 搜索算法相关参数 */
    pstParams->eMEMethod = (ENUM_IVIDENC_ME_METHOD)IMedia_Ini_GetInteger(pstIniHandle, "MotionEstimationType", ME_HEX);
    pstParams->iSubMELevel = IMedia_Ini_GetInteger(pstIniHandle, "SubMELevel", 5);
    pstParams->iMERange = IMedia_Ini_GetInteger(pstIniHandle, "MERange", 32);
    pstParams->iRefFrameNum = IMedia_Ini_GetInteger(pstIniHandle, "ReferenceNumber", 1);
    pstParams->bMixedRefFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "MixedReference", 0);
    pstParams->bChromaMEFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "ChromaME", 0);
    pstParams->bBiMEFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "BiME", 0);
    pstParams->bFastPSkipFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "FastSkip", 1);

    /**< 前处理算法相关参数 */
    pstParams->eScenecutType = (ENUM_IVIDENC_SCENECUT_METHOD)IMedia_Ini_GetInteger(pstIniHandle, "ScenecutType", SCENECUT_NONE);

    /**< B帧相关参数 */
    pstParams->eBFrameRefType = (ENUM_IVIDENC_BREF_METHOD)IMedia_Ini_GetInteger(pstIniHandle, "BHiercodingType", BREF_NONE);
    pstParams->eBFrameDirectMode = (ENUM_IVIDENC_DIRECT_MODE)IMedia_Ini_GetInteger(pstIniHandle, "DirectModeType", DIRECT_SPATIAL);
    pstParams->iBFrameNum = IMedia_Ini_GetInteger(pstIniHandle, "BPictureNumber", 0);
    //INT32  iBFrameDirect8x8;
    pstParams->bBFrameAdaptFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "BPictureAdapt", 1);
    pstParams->bBFrameRDOFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "BPictureRDO", 0);

    /**< 宏块模式相关参数 */
    pstParams->bI4x4Flag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "I4x4", 1);
    pstParams->bI8x8Flag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "I8x8", 0);
    pstParams->bPSub16x16Flag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "PSub16x16", 1);
    pstParams->bPSub8x8Flag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "PSub8x8", 1);
    pstParams->bBSub16x16Flag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "BSub16x16", 1);

    /**< 加权预测相关参数 */
    pstParams->bWPFlag = (BOOL)IMedia_Ini_GetInteger(pstIniHandle, "WeightedPrediction", 0);

    /**< 环路滤波相关参数 */
    pstParams->eLfDisableIdc = (ENUM_IVIDENC_FILTER_TYPE)IMedia_Ini_GetInteger(pstIniHandle, "LFDisableIdc", FILTER_FILTER);
    pstParams->iLFAlphaOffset = IMedia_Ini_GetInteger(pstIniHandle, "LFAlpha", 0);
    pstParams->iLFBetaOffset = IMedia_Ini_GetInteger(pstIniHandle, "LFBeta", 0);

    pstParams->eQuantType = (ENUM_IVIDENC_QUANT_TYPE)IMedia_Ini_GetInteger(pstIniHandle, "QuantType", QUANT_TRELLIS);
    // slice partition
    pstParams->eSliceMode = (ENUM_IVIDENC_SLICE_MODE)IMedia_Ini_GetInteger(pstIniHandle, "SliceMode", SLICE_MODE_ONLY_ONE);
    pstParams->iSliceParam = IMedia_Ini_GetInteger(pstIniHandle, "SliceArgument", 1024);

    // 释放相关资源
    IMedia_Ini_Close(pstIniHandle);
    pstIniHandle = NULL;

    return IMEDIA_RET_SUCCESS;
}

// 获取算法通道的状态
static INT32 GetStatus(STRU_VIDEO_ENCODE_CTX *pstEncCtx, STRU_IVIDENC_STATUS *pstStatus)
{
    INT32 iRet = 0;
    IHW264E_STATUS_ENC stStatus;

    // 检查相关参数
    if (NULL == pstStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    memset(pstStatus, 0, sizeof(STRU_IVIDENC_STATUS));

    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input pstEncCtx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    pstStatus->eCodecStatus = pstEncCtx->eStatus;

    if (pstEncCtx->pstEncoder)
    {
        iRet = pstEncCtx->stModule.pfnGetEncStatus(pstEncCtx->pstEncoder, &stStatus, HW264E_GET_STATUS_ENC);

        //pstStatus->eCodecStatus     = (ENUM_IVIDEO_CODEC_STATUS)stStatus.eEncState;
        pstStatus->iErrorCode       = (INT32)stStatus.eErrorCode;
        pstStatus->uiTotalErrors    = pstEncCtx->uiTotalErrors;
        pstStatus->fAvgPSNR         = (pstEncCtx->uiPSNRFrames > 0) ? (FLOAT32)(((FLOAT64)pstEncCtx->uiTotalPSNR_Y / 100) / pstEncCtx->uiPSNRFrames) : 0.0;
        pstStatus->uiIFrames        = stStatus.uiEncIDRFrames + stStatus.uiEncIFrames;
        pstStatus->uiPFrames        = stStatus.uiEncPFrames;
        pstStatus->uiBFrames        = stStatus.uiEncBFrames;
        pstStatus->uiBRefFrames     = stStatus.uiEncBRefFrames;
        pstStatus->uiSkipFrames     = stStatus.uiSkipFrames;
        pstStatus->uiDropFrames     = pstEncCtx->uiTotalDropFrames;

        if (pstEncCtx->ullTotalSpendTime > 0)
        {
            pstStatus->fAvgFps = (FLOAT32)(pstEncCtx->uiTotalOutputFrames * 1000) / (FLOAT32)(pstEncCtx->ullTotalSpendTime);
        }

        return iRet;
    }

    return IMEDIA_RET_SUCCESS;
}

// 获取算法通道的流信息
static INT32 GetStreamInfo(STRU_VIDEO_ENCODE_CTX *pstEncCtx, STRU_IVIDEO_STREAM_INFO *pstStreamInfo)
{
    INT32 iRet = 0;
    IHW264E_STATUS_PARAM stStreamInfo;

    // 检查相关参数
    if (NULL == pstStreamInfo)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    memset(pstStreamInfo, 0, sizeof(STRU_IVIDEO_STREAM_INFO));
    pstStreamInfo->eProfile      = IVIDEO_PROFILE_UNKNOWN;
    pstStreamInfo->eLevel        = IVIDEO_LEVEL_UNKNOWN;

    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input pstEncCtx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    if (NULL == pstEncCtx->pstEncoder)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] pstEncoder NULL\n", pstEncCtx);
        return IMEDIA_RET_OBJ_NOT_INIT;
    }

    if (IVIDEO_CODEC_RUNNING > pstEncCtx->eStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] eStatus[%d] < RUNNING\n", pstEncCtx, pstEncCtx->eStatus);
        return IMEDIA_RET_OBJ_NOT_INIT;
    }

    iRet = pstEncCtx->stModule.pfnGetEncStatus(pstEncCtx->pstEncoder, &stStreamInfo, HW264E_GET_STATUS_PARAM);

    pstStreamInfo->eCodecType    = IVIDEO_CODEC_H264;
    pstStreamInfo->eProfile      = (ENUM_IVIDEO_CODEC_PROFILE)stStreamInfo.eProfile;
    pstStreamInfo->eLevel        = (ENUM_IVIDEO_CODEC_LEVEL)stStreamInfo.eLevel;
    pstStreamInfo->eContentType  = (HW264E_FRAME == stStreamInfo.eInterlace) ? IVIDEO_PROGRESSIVE : IVIDEO_INTERLACED;
    pstStreamInfo->eColorSpaceType = (ENUM_IVIDEO_COLOR_SPACE_TYPE)stStreamInfo.eColorSpace;
    pstStreamInfo->usWidth       = (UINT16)stStreamInfo.iWidth;
    pstStreamInfo->usHeight      = (UINT16)stStreamInfo.iHeight;
    pstStreamInfo->uiRefFrameNum = (UINT32)stStreamInfo.iReferenceNum;
    pstStreamInfo->uiFrameRate   = (UINT32)(stStreamInfo.fFrameRate * 1000);
    pstStreamInfo->uiBitrate     = (UINT32)stStreamInfo.iBitRate;
    pstStreamInfo->uiVbvBuf      = (UINT32)stStreamInfo.iVbvBufDelay;

    pstStreamInfo->stAspectRatio.usSarWidth = pstEncCtx->stDynParams.stAspectRatio.usSarWidth;
    pstStreamInfo->stAspectRatio.usSarHeight = pstEncCtx->stDynParams.stAspectRatio.usSarHeight;

    return iRet;
}

static void ResetContextStatus(STRU_VIDEO_ENCODE_CTX *pstEncCtx)
{
    pstEncCtx->uiTotalInputFrames = 0;
    pstEncCtx->uiTotalOutputFrames = 0;
    pstEncCtx->uiTotalDropFrames = 0;
    pstEncCtx->uiTotalErrors = 0;
    pstEncCtx->uiTotalPSNR_Y = 0;
    pstEncCtx->uiPSNRFrames = 0;
    pstEncCtx->fCurFramePSNR_Y = 0.0;
    pstEncCtx->ullTotalSpendTime = 0;
    pstEncCtx->ullPrevSpendTime = 0;
    pstEncCtx->uiPrevOutputFrames = 0;
    pstEncCtx->bEncodeLastFrameFlag = FALSE;
}

// 设置算法通道的动态参数
static INT32 SetDynamicParams(STRU_VIDEO_ENCODE_CTX *pstEncCtx, STRU_IVIDENC_DYNAMIC_PARAMS *params)
{
    INT32  iRet = IMEDIA_RET_SUCCESS;
    UINT32 uiFrameMaxSize = 0;
    UINT8 *pTempBuf = NULL;
    IHW264E_DYN_PARAM_BASE   stDynBaseParams;
    IHW264E_DYN_PARAM_CUSTOM stDynCustomParams;
    INT32  iInFrameRate = DEFAULT_FRAME_RATE;
    INT32  iOutFrameRate = DEFAULT_FRAME_RATE;

    // 检查相关参数
    if (NULL == params)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input pstEncCtx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    if (NULL == pstEncCtx->pstEncoder)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] pstEncoder NULL\n", pstEncCtx);
        return IMEDIA_RET_OBJ_NOT_INIT;
    }

    if (IVIDEO_CODEC_READY > pstEncCtx->eStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] eStatus[%d] < IVIDEO_CODEC_READY\n", pstEncCtx, pstEncCtx->eStatus);
        return IMEDIA_RET_OBJ_NOT_INIT;
    }

    // 检测参数是否合法
    iRet = H264_CheckDynParams(&pstEncCtx->stParams, params);

    if (IMEDIA_RET_SUCCESS != iRet)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "H264_CheckDynParams return %d\n", iRet);
        return iRet;
    }

    // 复制相关参数
    memcpy(&pstEncCtx->stDynParams, params, sizeof(STRU_IVIDENC_DYNAMIC_PARAMS));
    pstEncCtx->stDynParams.pszQualityCfgBuf = NULL;

    // 设置基本动态参数
    stDynBaseParams.iImgWidth       = (INT32)pstEncCtx->stDynParams.usWidth;
    stDynBaseParams.iImgHeight      = (INT32)pstEncCtx->stDynParams.usHeight;
    stDynBaseParams.iKeyInterval    = /*(ENCODE_TRICK_MODE == pstEncCtx->eType) ? 1 :*/ (INT32)pstEncCtx->stDynParams.uiKeyframeInterval;
    stDynBaseParams.bGopAdapt       = /*(ENCODE_TRICK_MODE == pstEncCtx->eType) ? 0 :*/ pstEncCtx->stDynParams.bGopAdaptFlag;
    stDynBaseParams.bRepeatParamSet = pstEncCtx->stDynParams.bRepeatHeaderFlag;
    stDynBaseParams.iBitRate        = (INT32)pstEncCtx->stDynParams.uiBitrate;
    stDynBaseParams.fFrameRate      = (float)pstEncCtx->stDynParams.uiFrameRate / 1000;
    stDynBaseParams.iVbvBufDelay    = (INT32)pstEncCtx->stDynParams.uiVbvBufDelay;
    stDynBaseParams.bSkipEnable     = pstEncCtx->stDynParams.bSkipFrameFlag;
    stDynBaseParams.iSarWidth       = (INT32)pstEncCtx->stDynParams.stAspectRatio.usSarWidth;
    stDynBaseParams.iSarHeight      = (INT32)pstEncCtx->stDynParams.stAspectRatio.usSarHeight;

    // 分析质量参数
    if (QUALITY_LEVEL_CUSTOM == pstEncCtx->stParams.eQualityLevel)
    {
        if (NULL == params->pszQualityCfgBuf)
        {
            IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] QUALITY_LEVEL_CUSTOM pszQualityCfgBuf NULL\n", pstEncCtx, params->pszQualityCfgBuf);
            return IMEDIA_RET_PARAM_IN_VALUE_NULL;
        }

        iRet = GetQualityParamsFromMemory(&pstEncCtx->stQualityParams, params->pszQualityCfgBuf);

        if (IMEDIA_RET_SUCCESS != iRet)
        {
            IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] GetQualityParamsFromMemory = %d\n", pstEncCtx, iRet);
            return iRet;
        }

        // 修正不符合快进编码要求的部分自定义参数，缺省参数集需要在编码器内部设置
        if (ENCODE_TRICK_MODE == pstEncCtx->eType)
        {
            pstEncCtx->stQualityParams.iBFrameNum = 0;
            pstEncCtx->stQualityParams.bBFrameAdaptFlag = 0;
            pstEncCtx->stQualityParams.bBFrameRDOFlag = 0;
            pstEncCtx->stQualityParams.eScenecutType = SCENECUT_NONE;
            //pstEncCtx->stQualityParams.iMultiPass = 1;
            pstEncCtx->stQualityParams.eBFrameRefType = BREF_NONE;
            pstEncCtx->stQualityParams.eSliceMode = SLICE_MODE_ONLY_ONE;

            // 在场序列的情况下只支持场编码
            if (IVIDEO_INTERLACED == pstEncCtx->stParams.eContentType && pstEncCtx->stParams.eProfile > IVIDEO_PROFILE_H264_BASELINE)
            {
                pstEncCtx->stQualityParams.ePicCodingType = PIC_CODING_FIELD;
            }
            else
            {
                pstEncCtx->stQualityParams.ePicCodingType = PIC_CODING_FRAME;
            }
        }

        // 设置自定义动态参数
        stDynCustomParams.iMultiPass    = pstEncCtx->stQualityParams.iMultiPass;
        stDynCustomParams.eInterlace    = (HW264E_INTERLACE)pstEncCtx->stQualityParams.ePicCodingType;
        stDynCustomParams.eLevel        = (HW264E_LEVEL)pstEncCtx->stQualityParams.eLevel;
        stDynCustomParams.eSliceMode    = (HW264E_SLICE_MODE)pstEncCtx->stQualityParams.eSliceMode;
        stDynCustomParams.iSliceParam   = pstEncCtx->stQualityParams.iSliceParam;
        stDynCustomParams.bCabac        = pstEncCtx->stQualityParams.bCabacFlag;
        stDynCustomParams.bDct8x8       = pstEncCtx->stQualityParams.bDCT8x8Flag;
        stDynCustomParams.iReferenceNum = pstEncCtx->stQualityParams.iRefFrameNum;
        stDynCustomParams.iChromaQPOffset    = pstEncCtx->stQualityParams.iChromaQPOffset;
        stDynCustomParams.iSecondChrQPOffset = pstEncCtx->stQualityParams.iSecondChromaQPOffset;
        stDynCustomParams.eScenecutType = (HW264E_SCENECUT_METHOD)pstEncCtx->stQualityParams.eScenecutType;
        stDynCustomParams.iBPicNum      = pstEncCtx->stQualityParams.iBFrameNum;
        stDynCustomParams.bBAdapt       = pstEncCtx->stQualityParams.bBFrameAdaptFlag;
        stDynCustomParams.eBRefType     = (HW264E_BREF_METHOD)pstEncCtx->stQualityParams.eBFrameRefType;
        stDynCustomParams.eDirectMode   = (HW264E_DIRECT_MODE)pstEncCtx->stQualityParams.eBFrameDirectMode;
        stDynCustomParams.bI4x4         = pstEncCtx->stQualityParams.bI4x4Flag;
        stDynCustomParams.bI8x8         = pstEncCtx->stQualityParams.bI8x8Flag;
        stDynCustomParams.bPSub16x16    = pstEncCtx->stQualityParams.bPSub16x16Flag;
        stDynCustomParams.bPSub8x8      = pstEncCtx->stQualityParams.bPSub8x8Flag;
        stDynCustomParams.bBSub16x16    = pstEncCtx->stQualityParams.bBSub16x16Flag;
        stDynCustomParams.bWP           = pstEncCtx->stQualityParams.bWPFlag;
        stDynCustomParams.eMEMethod     = (HW264E_ME_METHOD)pstEncCtx->stQualityParams.eMEMethod;
        stDynCustomParams.iMERange      = pstEncCtx->stQualityParams.iMERange;
        stDynCustomParams.iSubMELevel   = pstEncCtx->stQualityParams.iSubMELevel;
        stDynCustomParams.bMixedReference = pstEncCtx->stQualityParams.bMixedRefFlag;
        stDynCustomParams.bChromaME     = pstEncCtx->stQualityParams.bChromaMEFlag;
        stDynCustomParams.bBiME         = pstEncCtx->stQualityParams.bBiMEFlag;
        stDynCustomParams.bBPicRDO      = pstEncCtx->stQualityParams.bBFrameRDOFlag;
        stDynCustomParams.bFastPSkip    = pstEncCtx->stQualityParams.bFastPSkipFlag;
        stDynCustomParams.eQuantType    = (HW264E_QUANT_TYPE)pstEncCtx->stQualityParams.eQuantType;
        stDynCustomParams.eLFDisableIdc = (HW264E_FILTER_TYPE)pstEncCtx->stQualityParams.eLfDisableIdc;
        stDynCustomParams.iLFAlpha      = pstEncCtx->stQualityParams.iLFAlphaOffset;
        stDynCustomParams.iLFBeta       = pstEncCtx->stQualityParams.iLFBetaOffset;
    }
    else
    {
        memset(&stDynCustomParams, 0, sizeof(IHW264E_DYN_PARAM_CUSTOM));
    }

    // 修正动态参数
    if (ENCODE_TRICK_MODE == pstEncCtx->eType) /* && IVIDEO_INTERLACED == pstEncCtx->stParams.eContentType && !pstEncCtx->stDynParams.bSkipFrameFlag */
    {
        if (pstEncCtx->stDynParams.bSkipFrameFlag)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "enc_ctx[%p] trick mode unsupport skip frame && modify\n", pstEncCtx);
            pstEncCtx->stDynParams.bSkipFrameFlag = FALSE;
        }

        if (1 != pstEncCtx->stDynParams.uiKeyframeInterval)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "enc_ctx[%p] trick mode modify uiKeyframeInterval = 1, org value = %lu\n", pstEncCtx, pstEncCtx->stDynParams.uiKeyframeInterval);
            pstEncCtx->stDynParams.uiKeyframeInterval = 1;
            stDynBaseParams.iKeyInterval = (INT32)pstEncCtx->stDynParams.uiKeyframeInterval;
        }
    }

    // 设置动态参数
    iRet = pstEncCtx->stModule.pfnControl(pstEncCtx->pstEncoder, &stDynBaseParams, &stDynCustomParams);

    if (HW264E_S_OK != iRet)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] HW264E_Control[%p] = %ld\n", pstEncCtx, pstEncCtx->pstEncoder, iRet);
        return iRet;
    }

    pstEncCtx->eStatus = IVIDEO_CODEC_RUNNING;

    // 生成丢帧掩码表（是否要考虑加skip帧标记）
    // 由于一期不考虑NTSC <-> PAL转换，可以先按整数处理，快进编码时帧率的要求没那么严格
    // 29.97fps时存在问题
    iInFrameRate  = (INT32)((pstEncCtx->stDynParams.uiInputRefFrameRate + 500) / 1000);
    iOutFrameRate = (INT32)((pstEncCtx->stDynParams.uiFrameRate + 500) / 1000);
    CalcTrickmodeCaptureInterval((ENCODE_TRICK_MODE == pstEncCtx->eType && pstEncCtx->stDynParams.bSkipFrameFlag) ?
                                 1 : (int)pstEncCtx->stParams.uiEncScale, iInFrameRate, iOutFrameRate, &iInFrameRate, &iOutFrameRate);

    if (pstEncCtx->uiFrameDropBase < (UINT32)iInFrameRate)
    {
        pTempBuf = IMedia_Malloc(iInFrameRate * sizeof(UINT8));

        if (pTempBuf)
        {
            // 先释放原先的内存
            if (pstEncCtx->pucFrameDropMask)
            {
                IMedia_Free(pstEncCtx->pucFrameDropMask);
            }

            pstEncCtx->pucFrameDropMask = pTempBuf;
            memset(pstEncCtx->pucFrameDropMask, 0, iInFrameRate * sizeof(UINT8));
            IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] malloc FrameDropMask(%ld) = %p\n", pstEncCtx, iInFrameRate, pstEncCtx->pucFrameDropMask);
        }
        else
        {
            IMEDIA_LOG(IMEDIA_WARNING, "enc_ctx[%p] malloc(%ld) = NULL\n", pstEncCtx, iInFrameRate);
        }
    }

    if (iInFrameRate > iOutFrameRate)
    {
        iRet = CalcFrameSkipPattern(iInFrameRate, iOutFrameRate, pstEncCtx->pucFrameDropMask,
                                    (ENCODE_TRICK_MODE == pstEncCtx->eType && pstEncCtx->stDynParams.bSkipFrameFlag) ? FRAME_MASK_SKIP : FRAME_MASK_DROP, FRAME_MASK_ENCODE);
        pstEncCtx->uiFrameDropBase = iInFrameRate;
    }
    else
    {
        // 输入帧率比输出帧率低的情况下暂不考虑插入帧（一期无NTSC <-> PAL转换）
        pstEncCtx->uiFrameDropBase = iInFrameRate;
        memset(pstEncCtx->pucFrameDropMask, FRAME_MASK_ENCODE, pstEncCtx->uiFrameDropBase);
    }

    // 当前码率配置条件下帧的可能最大尺寸
    //uiFrameMaxSize = stDynBaseParams.iBitRate * 125; // kbps -> bytes
    //uiFrameMaxSize = IVIDEO_YUV_SIZE(pstEncCtx->stDynParams.usWidth, pstEncCtx->stDynParams.usHeight) << 1; // 2帧YUV大小，CModel缺省值
    uiFrameMaxSize = IVIDEO_YUV_SIZE(pstEncCtx->stParams.usMaxWidth, pstEncCtx->stParams.usMaxHeight) << 1;   // 2帧YUV大小，用最大宽高值可避免编码宽高变化时造成内存空间的重新分配
    uiFrameMaxSize = IMEDIA_MAX(uiFrameMaxSize, 131072); // 最少128KB
    uiFrameMaxSize = IMEDIA_MIN(uiFrameMaxSize, (MAX_BITRATE_KBPS * 125)); // 取2帧YUV及最大码率空间的最小值

    if (pstEncCtx->uiFrameMaxSize < uiFrameMaxSize)
    {
        // 创建临时帧码流缓存
        pTempBuf = IMedia_Malloc(uiFrameMaxSize * sizeof(UINT8));

        if (pTempBuf)
        {
            // 先释放原先的内存
            if (pstEncCtx->pFrameStreamBuf)
            {
                IMedia_Free(pstEncCtx->pFrameStreamBuf);
            }

            pstEncCtx->pFrameStreamBuf = pTempBuf;
            pstEncCtx->uiFrameMaxSize = uiFrameMaxSize;
            memset(pstEncCtx->pFrameStreamBuf, 0, uiFrameMaxSize * sizeof(UINT8));
            IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] malloc FrameStreamBuf(%lu) = %p\n", pstEncCtx, uiFrameMaxSize, pstEncCtx->pFrameStreamBuf);
        }
        else
        {
            IMEDIA_LOG(IMEDIA_WARNING, "enc_ctx[%p] malloc(%lu) = NULL\n", pstEncCtx, uiFrameMaxSize);
        }
    }

    ResetContextStatus(pstEncCtx);

    return IMEDIA_RET_SUCCESS;
}

static void HW264E_msg_callback(const char *pszFileName, UINT32 uLine, const char *pszModelName, IH264E_LOG_LEVEL eLevel, UINT32 uChannelID, const char *format, ...)
{
    char msg[2048] = { 0 };
    va_list ap;

    va_start(ap, format);
    vsnprintf(msg + strlen(msg), 1024 - strlen(msg), format, ap);
    va_end(ap);

#if defined(__GNUC__)
    IMedia_msg_format(pszFileName, uLine, pszModelName, eLevel, msg);
#else
    IMedia_msg_format(eLevel, msg);
#endif
}


/*****************************************************************************************
* 函数说明：创建算法通道。
* 输入参数：
*        eCodecType    算法类型
*        params        编码器的一些基本信息的结构体指针，包括图像的宽高、比特率等信息
*        pfnReconFrame 当前通道重建帧YUV输出回调函数
* 输出参数：
*        pctx       算法通道句柄
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Videnc_Create(ENUM_IVIDEO_CODEC_TYPE eCodecType, STRU_IVIDENC_PARAMS *pstParams, IMEDIA_CODEC_CTX *pctx, funReconFrame *pfnCallBackRecFrame)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;
    STRU_VIDEO_ENCODE_CTX *pstEncCtx = NULL;
    IHW264E_STATIC_PARAM_BASE   stStaticBaseParams;
    IHW264E_STATIC_PARAM_CUSTOM stStaticCustomParams;

    // 1. 检测输入输出参数是否合法
    if (NULL == pstParams)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input params NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    // 目前只支持H264
    if (IVIDEO_CODEC_H264 != eCodecType)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "input eCodecType <%d> wrong\n", eCodecType);
        return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
    }

    if (NULL == pctx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "pctx NULL\n");
        return IMEDIA_RET_PARAM_NULL;
    }

    *pctx = NULL;

    // 检测参数是否合法
    iRet = H264_CheckMaxParams(pstParams);

    if (IMEDIA_RET_SUCCESS != iRet)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "H264_CheckMaxParams return %d\n", iRet);
        return iRet;
    }

    g_stGlobalInfo.bMallocFxnUsed = TRUE;

    // 2. 开始通道创建
    // 分配编码通道
    pstEncCtx = IMedia_Malloc(sizeof(STRU_VIDEO_ENCODE_CTX));

    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "failed to malloc enc_ctx memory\n");
        return  IMEDIA_RET_MEM_MALLOC_FAIL;
    }

    IMEDIA_LOG(IMEDIA_INFO, "malloc enc_ctx = %p succeed\n", pstEncCtx);

    // 初始化部分参数
    memset(pstEncCtx, 0, sizeof(STRU_VIDEO_ENCODE_CTX));
    memcpy(&pstEncCtx->stParams, pstParams, sizeof(STRU_IVIDENC_PARAMS));
    pstEncCtx->iCrcMask = CRC_MASK;
    pstEncCtx->eStatus = IVIDEO_CODEC_INVALID;

    if (1 > pstEncCtx->stParams.uiEncScale)
    {
        pstEncCtx->stParams.uiEncScale = 1;
    }

    pstEncCtx->eType = (pstEncCtx->stParams.uiEncScale > 1) ? ENCODE_TRICK_MODE : ENCODE_NORMAL;

    // 3. 创建H264编码器
    // 设置基本静态参数
    stStaticBaseParams.iMaxWidth        = pstParams->usMaxWidth;
    stStaticBaseParams.iMaxHeight       = pstParams->usMaxHeight;
    stStaticBaseParams.iMaxKeyInterval  = pstParams->uiMaxKeyframeInterval;
    stStaticBaseParams.eColorSpace      = HW264E_YUV_420;
    stStaticBaseParams.bSupportField    = (IVIDEO_PROGRESSIVE == pstParams->eContentType) ? 0 : 1;
    stStaticBaseParams.eProfile         = (HW264E_PROFILE)pstParams->eProfile;
    stStaticBaseParams.ePreset          = (HW264E_PRESET)pstParams->eQualityLevel;
    stStaticBaseParams.MallocFunc       = (HW264E_MALLOC_FXN)IMedia_Malloc;
    stStaticBaseParams.FreeFunc         = (HW264E_FREE_FXN)IMedia_Free;
    stStaticBaseParams.LogFunc          = (HW264E_LOG_FXN)HW264E_msg_callback;
    stStaticBaseParams.ReconFunc        = (HW264E_RECON_FXN)pfnCallBackRecFrame;
    stStaticBaseParams.CalcQualityFunc  = (pstParams->iFlags & IMEDIA_FLAG_CALC_PSNR) ? CalculatePSNR : NULL;

    // 设置自定义静态参数
    stStaticCustomParams.iMaxRefNum     = pstParams->uiMaxRefFrameNum;
    stStaticCustomParams.iMaxBNum       = pstParams->uiMaxBFrameNum;
    stStaticCustomParams.iMaxMultiPass  = pstParams->uiMaxMultipass;

    // 初始化缺省动态参数
    pstEncCtx->stDynParams.usWidth              = stStaticBaseParams.iMaxWidth;
    pstEncCtx->stDynParams.usHeight             = stStaticBaseParams.iMaxHeight;
    pstEncCtx->stDynParams.uiInputRefFrameRate  = 25000;
    pstEncCtx->stDynParams.uiFrameRate          = pstEncCtx->stDynParams.uiInputRefFrameRate;
    pstEncCtx->stDynParams.uiBitrate            = 2000; // 2Mbps
    pstEncCtx->stDynParams.uiVbvBufDelay        = 2000; // 2s
    pstEncCtx->stDynParams.uiKeyframeInterval   = stStaticBaseParams.iMaxKeyInterval;
    pstEncCtx->stDynParams.bGopAdaptFlag        = 1;
    pstEncCtx->stDynParams.bRepeatHeaderFlag    = 1;
    pstEncCtx->stDynParams.bSkipFrameFlag       = 0;
    pstEncCtx->stDynParams.stAspectRatio.usSarWidth = 1;
    pstEncCtx->stDynParams.stAspectRatio.usSarHeight = 1;
    pstEncCtx->stDynParams.pszQualityCfgBuf     = NULL;

    // 初始化缺省质量参数
    pstEncCtx->stQualityParams.uiVbvBufDelay = 1000;
    pstEncCtx->stQualityParams.bSkipFrameFlag = 0;
    pstEncCtx->stQualityParams.eSliceMode = SLICE_MODE_ONLY_ONE;
    pstEncCtx->stQualityParams.iSliceParam = 1300;

    iRet = HW264E_LoadLibrary(&pstEncCtx->stModule, (ENCODE_TRICK_MODE == pstEncCtx->eType));

    if (IMEDIA_RET_SUCCESS != iRet)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] LoadLibrary = %d failed\n", pstEncCtx, iRet);
        goto CREATE_FAILED;
    }

    IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] LoadLibrary succeed, handle = %d\n", pstEncCtx, pstEncCtx->stModule.handle);

    if (ENCODE_TRICK_MODE == pstEncCtx->eType)
    {
        // H264快进编码器, 调整部分参数
        //stStaticCustomParams.iMaxRefNum = 1;
        stStaticCustomParams.iMaxBNum = 0;
        //stStaticCustomParams.iMaxMultiPass = 1;
        pstEncCtx->stParams.uiMaxRefFrameNum = (UINT32)stStaticCustomParams.iMaxRefNum;
        pstEncCtx->stParams.uiMaxBFrameNum = (UINT32)stStaticCustomParams.iMaxBNum;
        pstEncCtx->stParams.uiMaxMultipass = (UINT32)stStaticCustomParams.iMaxMultiPass;
    }

    iRet = pstEncCtx->stModule.pfnCreate((IHEncoder *)&pstEncCtx->pstEncoder, &stStaticBaseParams, &stStaticCustomParams, (pstParams->iFlags & IMEDIA_FLAG_DISABLE_ASM) ? 0 : 1, (UINT32)pstEncCtx);

    if (IMEDIA_RET_SUCCESS != iRet)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "HW264E_Create(%p) = %d failed\n", pstEncCtx->pstEncoder, iRet);
        goto CREATE_FAILED;
    }

    IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] create sub_ctx[%p] succeed\n", pstEncCtx, pstEncCtx->pstEncoder);

    pstEncCtx->eStatus = IVIDEO_CODEC_READY;
    *pctx = pstEncCtx;

    // 在此处设置缺省动态参数的原因是可节省调用control(IMEDIA_SET_DYN_PARAMS)接口
    //iRet = SetDynamicParams(pstEncCtx, &pstEncCtx->stDynParams);
    //if (IMEDIA_RET_SUCCESS != iRet)
    //{
    //    IMEDIA_LOG(IMEDIA_WARNING, "SetDynamicParams = %d failed\n", iRet);
    //}

    IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] create succeed\n", pstEncCtx);

    return IMEDIA_RET_SUCCESS;

CREATE_FAILED:

    if (NULL != pstEncCtx)
    {
        HW264E_FreeLibrary(&pstEncCtx->stModule);
        IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] create failed and destroy ctx\n", pstEncCtx);
        pstEncCtx->iCrcMask = 0;
        pstEncCtx->eStatus = IVIDEO_CODEC_INVALID;
        IMedia_Free(pstEncCtx);
        pstEncCtx = NULL;
    }

    return iRet;
}

/*****************************************************************************************
* 函数说明：通道控制函数。
* 输入参数：
*        ctx     算法通道句柄
*        cmd:    控制参数的命令码
*           IMEDIA_GET_VERSION     获取版本信息                  wparam(STRU_IMEDIA_VERSION*)           lparam(NULL)
*           IMEDIA_GET_STATUS      获取算法通道状态信息          wparam(STRU_IVIDENC_STATUS*)           lparam(NULL)
*           IMEDIA_GET_STREAM_INFO 获取算法通道当前码流信息      wparam(STRU_IVIDEO_STREAM_INFO*)       lparam(NULL)
*           IMEDIA_GET_PARAMS      获取算法通道的参数信息        wparam(STRU_IVIDENC_PARAMS*)           lparam(NULL)
*           IMEDIA_GET_DYN_PARAMS  获取算法通道的动态参数        wparam(STRU_IVIDENC_DYNAMIC_PARAMS*)   lparam(NULL)
*           IMEDIA_GET_SPS_PPS     获取序列头图像头NALU码流      wparam(STRU_H264_FRAME_STREAM*)        lparam(NULL)
*           IMEDIA_SET_DYN_PARAMS  设置算法通道的动态参数        wparam(STRU_IVIDENC_DYNAMIC_PARAMS*)   lparam(NULL)
*        wparam    每种命令对应各自类型，可作为输入或输出
*        lparam    每种命令对应各自类型，可作为输入或输出
* 输出参数：
*        wparam    每种命令对应各自类型，可作为输入或输出
*        lparam    每种命令对应各自类型，可作为输入或输出
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Videnc_Control(IMEDIA_CODEC_CTX ctx, ENUM_IMEDIA_CMD cmd, void *wparam, void *lparam)
{
    switch (cmd)
    {
        case IMEDIA_GET_VERSION:
        {
            STRU_IMEDIA_VERSION *pstVersion = (STRU_IMEDIA_VERSION *)wparam;

            if (NULL == pstVersion)
            {
                return IMEDIA_RET_PARAM_NULL;
            }

            strncpy(pstVersion->cVersionChar, "iMedia MS-ENC V100R001P301", IMEDIA_VERSION_LENGTH);
            strncpy(pstVersion->cReleaseTime, __TIME__ " " __DATE__, IMEDIA_TIME_LENGTH);
#if defined(_MSC_VER)
            pstVersion->uiCompileVersion = IMEDIA_FOURCC('V', 'S', '9', '0');
#else
            pstVersion->uiCompileVersion = IMEDIA_FOURCC('G', 'C', 'C', '4');
#endif
        }
        break;

        case IMEDIA_GET_STATUS:
        {
            return GetStatus((STRU_VIDEO_ENCODE_CTX *)ctx, (STRU_IVIDENC_STATUS *)wparam);
        }

        //break;
        case IMEDIA_GET_STREAM_INFO:
        {
            return GetStreamInfo((STRU_VIDEO_ENCODE_CTX *)ctx, (STRU_IVIDEO_STREAM_INFO *)wparam);
        }

        //break;
        case IMEDIA_GET_PARAMS:
        {
            return GetStaticParams((STRU_VIDEO_ENCODE_CTX *)ctx, (STRU_IVIDENC_PARAMS *)wparam);
        }

        //break;
        case IMEDIA_GET_DYN_PARAMS:
        {
            return GetDynamicParams((STRU_VIDEO_ENCODE_CTX *)ctx, (STRU_IVIDENC_DYNAMIC_PARAMS *)wparam);
        }

        //break;
        case IMEDIA_GET_SPS_PPS:
        {
            return GetSpsPps((STRU_VIDEO_ENCODE_CTX *)ctx, (STRU_H264_FRAME_STREAM *)wparam);
        }

        //break;
        case IMEDIA_SET_DYN_PARAMS:
        {
            return SetDynamicParams((STRU_VIDEO_ENCODE_CTX *)ctx, (STRU_IVIDENC_DYNAMIC_PARAMS *)wparam);
        }

        //break;
        default:
        {
            IMEDIA_LOG(IMEDIA_ERROR, "invalid cmd\n");
            return IMEDIA_RET_CMD_INVALID;
        }

        //break;
    }

    return IMEDIA_RET_SUCCESS;
}

/*****************************************************************************************
* 函数说明：算法通道处理函数，每次调用仅输出一帧，通过输出参数ENUM_BITSTREAM_STATUS获取是否为最后一帧的码流。
*           在此之前需调用IMedia_Videnc_Control(...,cmd=IMEDIA_SET_DYN_PARAMS,...)接口
* 输入参数：
*        ctx        算法通道句柄
*        pstInArgs  输入参数信息（输入YUV等信息）
* 输出参数：
*        pstOutArgs 输出参数信息（输出码流地址由算法模块内部分配好后输出，应用层不能释放空间）
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Videnc_Process(IMEDIA_CODEC_CTX ctx, STRU_IVIDENC_IN_ARGS *pstInArgs, STRU_IVIDENC_OUT_ARGS *pstOutArgs)
{
    //INT32 iRet = IMEDIA_RET_SUCCESS;
    INT32 iStride = IVIDEO_MAX_WIDTH;
    INT32 iFrmMask = FRAME_MASK_ENCODE;
    STRU_VIDEO_ENCODE_CTX *pstEncCtx = (STRU_VIDEO_ENCODE_CTX *)ctx;
    IHW264E_YUV    stYUV = { 0 };
    IHW264E_STREAM stStream = { 0 };
    IHW264E_YUV   *pstYUV = NULL;
    HW264E_RETURN  eRet = HW264E_S_OK;
    UINT64         ullCurrentSysTime = 0;

    // 检查相关参数
    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "pstEncCtx NULL\n");
        return  IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    if (NULL == pstEncCtx->pstEncoder)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] pstEncoder NULL\n", pstEncCtx);
        return IMEDIA_RET_OBJ_NOT_INIT;
    }

    if (IVIDEO_CODEC_RUNNING != pstEncCtx->eStatus)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] eStatus[%d] != IVIDEO_CODEC_RUNNING\n", pstEncCtx, pstEncCtx->eStatus);
        return IMEDIA_RET_OBJ_NOT_INIT;
    }

    // 保证输入/输出参数结构体非空
    if (NULL == pstInArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] inArgs NULL\n", pstEncCtx);
        return IMEDIA_RET_PARAM_NULL;
    }

    if (NULL == pstOutArgs)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] outArgs NULL\n", pstEncCtx);
        return IMEDIA_RET_PARAM_NULL;
    }

    memset(pstOutArgs, 0, sizeof(STRU_IVIDENC_OUT_ARGS));
    pstOutArgs->eBitstreamStatus = BITSTREAM_UNKNOWN;
    pstOutArgs->stFrame.eFrameType = IVIDEO_FRAME_UNKNOWN;

    // 在输入图像非空时指定输入YUV
    if (NULL != pstInArgs->pstSource && NULL != pstInArgs->pstSource->apucBuf)
    {
        /* 2010/6/23 songxg+s00133955 [AZ1D02176] */
        // 当编码结束后，为了避免逻辑问题，不支持继续编码新YUV帧，原因是输出码流可能会花屏
        if (pstEncCtx->bEncodeLastFrameFlag)
        {
            /*if (ENCODE_TRICK_MODE != pstEncCtx->eType) */
            //IMEDIA_LOG(IMEDIA_WARNING, "enc_ctx[%p] Encode new YUV Maybe Mosaic, after last frame\n", pstEncCtx);
            IMEDIA_LOG(IMEDIA_WARNING, "enc_ctx[%p] Not Support new YUV after last frame && auto set NULL, call control(IMEDIA_SET_DYN_PARAMS) interface if need\n", pstEncCtx);
            pstYUV = NULL;
        }
        else
        {
            pstYUV = &stYUV;
            pstYUV->pucY = pstInArgs->pstSource->apucBuf[IVIDEO_Y];
            pstYUV->pucU = pstInArgs->pstSource->apucBuf[IVIDEO_U];
            pstYUV->pucV = pstInArgs->pstSource->apucBuf[IVIDEO_V];
            iStride = pstInArgs->pstSource->usWidthPitch;
        }
    }
    else if (!pstInArgs->bLastFrameFlag && !pstEncCtx->bEncodeLastFrameFlag)
    {
        // 尚未到最后编码的话，不允许出现输入YUV为NULL
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] source NULL before last frame\n", pstEncCtx);
        return IMEDIA_RET_PARAM_IN_VALUE_NULL;
    }

    // 判断是否进入了最后一帧编码
    if (pstInArgs->bLastFrameFlag && !pstEncCtx->bEncodeLastFrameFlag)
    {
        IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] enter last frame encode\n", pstEncCtx);
        pstEncCtx->bEncodeLastFrameFlag = TRUE;
    }

    // 根据源参考帧率和目标参考帧率判断是否进行编码
    if (pstYUV)
    {
        // 保证输入图像Y,U,V平面非空
        if (NULL == pstYUV->pucY || NULL == pstYUV->pucU || NULL == pstYUV->pucV)
        {
            IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] source Y || U || V buffer NULL\n", pstEncCtx);
            return IMEDIA_RET_PARAM_IN_VALUE_NULL;
        }

        if (pstInArgs->pstSource->usWidth != pstEncCtx->stDynParams.usWidth || pstInArgs->pstSource->usHeight != pstEncCtx->stDynParams.usHeight)
        {
            IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] inW(%u) != %u || inH(%u) != %u\n", pstEncCtx,
                       pstInArgs->pstSource->usWidth, pstEncCtx->stDynParams.usWidth,
                       pstInArgs->pstSource->usHeight, pstEncCtx->stDynParams.usHeight);
            return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        }

        if (pstInArgs->pstSource->usWidthPitch < pstInArgs->pstSource->usWidth) /*|| pstInArgs->pstSource->usWidthPitch > IVIDEO_MAX_PITCH*/
        {
            IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] inWPitch(%u) < inW(%u)\n", pstEncCtx,
                       pstInArgs->pstSource->usWidthPitch, pstEncCtx->stDynParams.usWidth);
            return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        }

        if (pstInArgs->pstSource->usWidthPitch % 2)
        {
            IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] inWPitch(%u) % 2 != 0\n", pstEncCtx, pstInArgs->pstSource->usWidthPitch);
            return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        }

        // 目前只支持YUV420，暂无色度空间选项，故将其注释
        /*if (IVIDEO_CSP_YUV420 != pstInArgs->pstSource->eColorSpaceType)
        {
            IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] eContentType(%d) != IVIDEO_CSP_YUV420 \n", pstEncCtx, pstInArgs->pstSource->eContentType);
            return IMEDIA_RET_PARAM_IN_VALUE_INVALID;
        }*/

        // 检查是否可以丢帧
        if (0 != pstEncCtx->uiFrameDropBase && pstEncCtx->pucFrameDropMask)
        {
            if (ENCODE_TRICK_MODE == pstEncCtx->eType && pstEncCtx->stDynParams.bSkipFrameFlag)
            {
                if (pstEncCtx->uiTotalInputFrames % pstEncCtx->stParams.uiEncScale)
                {
                    iFrmMask = FRAME_MASK_DROP;
                }
                else
                {
                    iFrmMask = pstEncCtx->pucFrameDropMask[(pstEncCtx->uiTotalInputFrames / pstEncCtx->stParams.uiEncScale) % pstEncCtx->uiFrameDropBase];
                }
            }
            else
            {
                iFrmMask = pstEncCtx->pucFrameDropMask[pstEncCtx->uiTotalInputFrames % pstEncCtx->uiFrameDropBase];
            }

            pstEncCtx->uiTotalInputFrames++;

            if (FRAME_MASK_DROP == iFrmMask)
            {
                pstEncCtx->uiTotalDropFrames++;
                IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] FrmID:%4d drop %4d\n", pstEncCtx, pstEncCtx->uiTotalInputFrames, pstEncCtx->uiTotalDropFrames);
                return IMEDIA_RET_SUCCESS;
            }
        }
    }

    if (pstEncCtx->stParams.iFlags & IMEDIA_FLAG_CALC_FPS)
    {
        ullCurrentSysTime = IMedia_OS_Milliseconds();
    }

    // 调用实际编码函数
    stStream.pucStream = pstEncCtx->pFrameStreamBuf;
    stStream.iBufLen = pstEncCtx->uiFrameMaxSize;

    if (ENCODE_TRICK_MODE == pstEncCtx->eType)
    {
        eRet = pstEncCtx->stModule.pfnEncode(pstEncCtx->pstEncoder, pstYUV, &stStream, (FRAME_MASK_SKIP == iFrmMask) ? HW264E_FRAME_SKIP : HW264E_FRAME_IDR
                                             /*((IVIDEO_PROGRESSIVE == pstEncCtx->stParams.eContentType) ? HW264E_FRAME_IDR : HW264E_FRAME_OTHER)*/, iStride);
    }
    else
    {
        eRet = pstEncCtx->stModule.pfnEncode(pstEncCtx->pstEncoder, pstYUV, &stStream, (pstInArgs->bForceKeyframeFlag) ? HW264E_FRAME_IDR : HW264E_FRAME_OTHER, iStride);
    }

    // 计算性能
    if (pstEncCtx->stParams.iFlags & IMEDIA_FLAG_CALC_FPS)
    {
        UINT32 uiSpendMS = 0;
        ullCurrentSysTime = IMedia_OS_Milliseconds() - ullCurrentSysTime;
        pstEncCtx->ullTotalSpendTime += ullCurrentSysTime;

        uiSpendMS = (UINT32)(pstEncCtx->ullTotalSpendTime - pstEncCtx->ullPrevSpendTime);

        if (1000 < uiSpendMS)
        {
            IMEDIA_LOG(IMEDIA_DEBUG, "enc_ctx[%p] fps: %.2f [process frames %u -> %u spend %u ms]\n",
                       pstEncCtx, (FLOAT32)((pstEncCtx->uiTotalOutputFrames - pstEncCtx->uiPrevOutputFrames) * 1000) / uiSpendMS,
                       pstEncCtx->uiPrevOutputFrames, pstEncCtx->uiTotalOutputFrames, uiSpendMS);
            pstEncCtx->ullPrevSpendTime = pstEncCtx->ullTotalSpendTime;
            pstEncCtx->uiPrevOutputFrames = pstEncCtx->uiTotalOutputFrames;
        }
    }

    // 设置输出信息
    pstOutArgs->iErrorCode = 0;
    //pstEncCtx->uiTotalErrors;
    //pstStatus->fAvgPSNR;

    switch (eRet)
    {
        case HW264E_S_END_OF_SOURCE: // 是否有码流输出
        {
            pstOutArgs->eBitstreamStatus = BITSTREAM_SEGMENT_END;

            /* 2010/07/07 14:25:00 songxg+00133955 [AZ1D02183] */
            /* 当编码结束后需将编码通道改为结束状态，避免后续再调用编码接口 */
            pstEncCtx->eStatus = IVIDEO_CODEC_STOPPED;
        }
        break;

        case HW264E_S_OK:
        {
            INT32 i = 0;
            IHW264E_STATUS_DEBUG stFrameInfo = { 0 };

            pstOutArgs->eBitstreamStatus = (0 == pstEncCtx->uiTotalOutputFrames) ? BITSTREAM_SEGMENT_BEGIN : BITSTREAM_OTHER;
            pstOutArgs->stFrame.pucStream  = stStream.pucStream;
            pstOutArgs->stFrame.uiDataSize = (UINT32)stStream.iDataLen;
            pstOutArgs->stFrame.uiFrameID = (UINT32)pstEncCtx->uiTotalOutputFrames++;
            pstOutArgs->stFrame.uiDisplayID = (UINT32)stStream.iDisplayNum;
            pstOutArgs->stFrame.bSkipFlag = stStream.bSkipFrm;
            pstOutArgs->stFrame.usNaluCount = (UINT16)stStream.iNalNum;

            if (MAX_NALU_COUNT_OF_FRAME < pstOutArgs->stFrame.usNaluCount)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "enc_ctx[%p] out NaluCount %u > %u\n", pstEncCtx, (UINT32)pstOutArgs->stFrame.usNaluCount, MAX_NALU_COUNT_OF_FRAME);
                pstOutArgs->stFrame.usNaluCount = MAX_NALU_COUNT_OF_FRAME;
            }

            for (i = 0; i < pstOutArgs->stFrame.usNaluCount; i++)
            {
                pstOutArgs->stFrame.stNalu[i].uiSize = (UINT32)stStream.stNAL[i].iNalSize;
                pstOutArgs->stFrame.stNalu[i].pucData = stStream.stNAL[i].pucNalData;
                pstOutArgs->stFrame.stNalu[i].eType = (ENUM_H264_NALU_TYPE)stStream.stNAL[i].eNaluType;
                pstOutArgs->stFrame.stNalu[i].ePriority = (ENUM_H264_NALU_PRIORITY)stStream.stNAL[i].eNaluPriority;
            }

            // 状态更新
            eRet = pstEncCtx->stModule.pfnGetEncStatus(pstEncCtx->pstEncoder, &stFrameInfo, HW264E_GET_STATUS_DEBUG);

            if (HW264E_S_OK == eRet)
            {
                pstOutArgs->stFrame.eFrameType = (ENUM_IVIDEO_FRAME_TYPE)stFrameInfo.eFrmType;
            }

            IMEDIA_LOG(IMEDIA_DEBUG, "enc_ctx[%p] FrmID:%3d/%3d Type:%s inOrder:%d/%d Field:%d Fade:%d SceneCut:%d WP:%d Direct:%d QP:%d %d %d Vbv:%4d Cplx:%d bytes:%d\n",
                       pstEncCtx,
                       pstEncCtx->uiTotalOutputFrames,
                       pstEncCtx->uiTotalInputFrames,
                       GetFrameTypeString((INT32)stFrameInfo.eFrmType),
                       stFrameInfo.uiInputOrderOfCur,
                       stFrameInfo.uiInputOrderOfLastIDR,
                       (INT32)stFrameInfo.bField,
                       (INT32)stFrameInfo.bFade,
                       (INT32)stFrameInfo.bSceneCut,
                       (INT32)stFrameInfo.eWPType,
                       (INT32)stFrameInfo.eDirectType,
                       stFrameInfo.iFrmAvgQP,
                       stFrameInfo.iFrmMaxQP,
                       stFrameInfo.iFrmMinQP,
                       stFrameInfo.iVbvFill,
                       stFrameInfo.iFrmCplx,
                       stFrameInfo.iBits);
        }
        break;

        default:
        {
            if (HW264E_FAILED(eRet)) //encoding error
            {
                IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] HW264E_Encode(%p) = %d\n", pstEncCtx, pstEncCtx->pstEncoder, eRet);
                return IMEDIA_RET_ENCODE_INTERRUPT;
            }
        }
        break;
    }

    return IMEDIA_RET_SUCCESS;
}

/*****************************************************************************************
* 函数说明：删除算法通道。
* 输入参数：
*        ctx    算法通道句柄
* 输出参数：
*        无
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Videnc_Delete(IMEDIA_CODEC_CTX ctx)
{
    INT32 iRet = IMEDIA_RET_SUCCESS;
    STRU_VIDEO_ENCODE_CTX *pstEncCtx = (STRU_VIDEO_ENCODE_CTX *)ctx;

    // 检查相关参数
    if (NULL == pstEncCtx)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "pstEncCtx NULL\n");
        return  IMEDIA_RET_PARAM_NULL;
    }

    if (CRC_MASK != pstEncCtx->iCrcMask)
    {
        IMEDIA_LOG(IMEDIA_ERROR, "enc_ctx[%p] iCrcMask[0x%x] wrong\n", pstEncCtx, pstEncCtx->iCrcMask);
        return IMEDIA_RET_OBJ_BAD_MASK;
    }

    // 释放临时帧码流缓存
    if (pstEncCtx->pFrameStreamBuf)
    {
        IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] free pFrameStreamBuf(%p)\n", pstEncCtx, pstEncCtx->pFrameStreamBuf);
        IMedia_Free(pstEncCtx->pFrameStreamBuf);
        pstEncCtx->pFrameStreamBuf = NULL;
        pstEncCtx->uiFrameMaxSize = 0;
    }

    // 释放临时掩码缓存
    if (pstEncCtx->pucFrameDropMask)
    {
        IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] free pucFrameDropMask(%p)\n", pstEncCtx, pstEncCtx->pucFrameDropMask);
        IMedia_Free(pstEncCtx->pucFrameDropMask);
        pstEncCtx->pucFrameDropMask = NULL;
        pstEncCtx->uiFrameDropBase = 0;
    }

    // 释放编码器内部通道
    if (pstEncCtx->pstEncoder)
    {
        IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] free sub_ctx[%p]\n", pstEncCtx, pstEncCtx->pstEncoder);
        iRet = pstEncCtx->stModule.pfnDelete((IHEncoder)pstEncCtx->pstEncoder);
        pstEncCtx->pstEncoder = NULL;
        HW264E_FreeLibrary(&pstEncCtx->stModule);
    }

    // 释放整个编码通道
    IMEDIA_LOG(IMEDIA_INFO, "enc_ctx[%p] free %d\n", pstEncCtx, iRet);
    pstEncCtx->iCrcMask = 0;
    pstEncCtx->eStatus = IVIDEO_CODEC_INVALID;
    IMedia_Free(pstEncCtx);
    pstEncCtx = NULL;

    return iRet;
}


/************************************************************************
* $Log$
************************************************************************/
