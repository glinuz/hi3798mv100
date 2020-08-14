/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  vi_proc.c
* Description:
*
***********************************************************************************/

#include "hi_drv_proc.h"
#include "hi_drv_file.h"
#include "hi_drv_mem.h"
#include "hi_drv_mmz.h"
#include "hi_drv_venc.h"
#include "drv_venc.h"
#include "hi_osal.h"
#include "hi_drv_log.h"
#include "drv_venc_osal.h"

#include "drv_vpss_ext.h"

#if (1 == HI_PROC_SUPPORT)
extern OPTM_VENC_CHN_S g_stVencChn[VENC_MAX_CHN_NUM];
extern VPSS_EXPORT_FUNC_S* pVpssFunc;
extern HI_S32 VENC_VpssEventHandle(HI_HANDLE hVenc, HI_DRV_VPSS_EVENT_E enEventID, HI_VOID* pstArgs);
#define D_VENC_GET_CHN(u32VeChn, hVencChn) \
    do {\
        if (hVencChn == NULL)\
        {\
            u32VeChn = VENC_MAX_CHN_NUM;\
            break;\
        }\
        u32VeChn = 0; \
        while (u32VeChn < VENC_MAX_CHN_NUM)\
        {   \
            if (g_stVencChn[u32VeChn].hVEncHandle == hVencChn)\
            { \
                break; \
            } \
            u32VeChn++; \
        } \
    } while (0)

#define D_VENC_GET_CHN_BY_UHND(u32VeChn, hVencUsrChn) \
    do {\
        if (hVencUsrChn == HI_INVALID_HANDLE)\
        {\
            u32VeChn = VENC_MAX_CHN_NUM;\
            break;\
        }\
        u32VeChn = 0; \
        while (u32VeChn < VENC_MAX_CHN_NUM)\
        {   \
            if (g_stVencChn[u32VeChn].hUsrHandle == hVencUsrChn)\
            { \
                break; \
            } \
            u32VeChn++; \
        } \
    } while (0)

static HI_VOID VENC_DRV_ProcHelp(HI_VOID)
{
    HI_DRV_PROC_EchoHelper("------ VENC Proc Help ------\n");
    HI_DRV_PROC_EchoHelper("USAGE:echo [cmd] [para1] [para2] > /proc/msp/vencXX\n");
    HI_DRV_PROC_EchoHelper("cmd = save_yuv,    para1 = start   start to save the yuv data before Encode\n");
    HI_DRV_PROC_EchoHelper("cmd = save_yuv,    para1 = stop    stop to save the yuv data before Encode\n");
    HI_DRV_PROC_EchoHelper("cmd = save_stream, para1 = second  save the streams after Encode for [para2] seconds\n");
    HI_DRV_PROC_EchoHelper("cmd = save_stream, para1 = frame  save the streams after Encode for [para2] frames\n");
    HI_DRV_PROC_EchoHelper("cmd = ClkGateEn,   para1          if [para1]=0,means unable clkGate\n");
    HI_DRV_PROC_EchoHelper("                                  if [para1]=1,means enable clkGate of frame level\n");
    HI_DRV_PROC_EchoHelper("                                  if [para1]=2,means enable clkGate both of frame and MB level\n");
    HI_DRV_PROC_EchoHelper("cmd = LowPowEn,    para1          if [para1]=0,means unable the low power control of algorithm\n");
    HI_DRV_PROC_EchoHelper("                                  if [para1]=0,means enable the low power control of algorithm\n");

}

#if 0
static HI_S32 VENC_DRV_ProcSaveYuv_k(struct file* pfYUV, HI_UNF_VIDEO_FRAME_INFO_S* pstFrame)
{
    VENC_BUFFER_S stMBuf;
    HI_S8* ptr;
    HI_S32 nRet;
    HI_U8* pu8Udata;
    HI_U8* pu8Vdata;
    HI_U8* pu8Ydata;
    HI_U32 i, j;

    stMBuf.u32StartPhyAddr = pstFrame->stVideoFrameAddr[0].u32YAddr;

    if (!stMBuf.u32StartPhyAddr)
    {
        HI_ERR_VENC("address '0x%x' is null!\n", pstFrame->stVideoFrameAddr[0].u32YAddr);
        return HI_FAILURE;
    }

    nRet = HI_DRV_VENC_Map(&stMBuf);
    ptr = (HI_S8*)stMBuf.pu8StartVirAddr;

    if (nRet)
    {
        HI_ERR_VENC("address '0x%x' is not valid!\n", pstFrame->stVideoFrameAddr[0].u32YAddr);
        return HI_FAILURE;
    }

    pu8Udata = HI_KMALLOC(HI_ID_VENC, pstFrame->u32Width * pstFrame->u32Height / 2 / 2, GFP_KERNEL);

    if (HI_NULL == pu8Udata)
    {
        goto ERR0;
    }

    pu8Vdata = HI_KMALLOC(HI_ID_VENC, pstFrame->u32Width * pstFrame->u32Height / 2 / 2, GFP_KERNEL);

    if (HI_NULL == pu8Vdata)
    {
        goto ERR1;
    }

    pu8Ydata = HI_KMALLOC(HI_ID_VENC, pstFrame->stVideoFrameAddr[0].u32YStride, GFP_KERNEL);

    if (HI_NULL == pu8Ydata)
    {
        goto ERR2;
    }

    /* write Y */
    for (i = 0; i < pstFrame->u32Height; i++)
    {
        memcpy(pu8Ydata, ptr, sizeof(HI_U8) * pstFrame->stVideoFrameAddr[0].u32YStride);

        if (pstFrame->u32Width != HI_DRV_FILE_Write(pfYUV, pu8Ydata, pstFrame->u32Width))
        {
            HI_ERR_VENC("line %d: fwrite fail!\n", __LINE__);
        }

        ptr += pstFrame->stVideoFrameAddr[0].u32YStride;
    }

    /* U V transfer and save */
    for (i = 0; i < pstFrame->u32Height / 2; i++)
    {
        for (j = 0; j < pstFrame->u32Width / 2; j++)
        {
            if (pstFrame->enVideoFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_420)
            {
                pu8Vdata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j];
                pu8Udata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j + 1];
            }
            else
            {
                pu8Udata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j];
                pu8Vdata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j + 1];
            }
        }

        ptr += pstFrame->stVideoFrameAddr[0].u32CStride;
    }

    /* write U */
    HI_DRV_FILE_Write(pfYUV, pu8Udata, pstFrame->u32Width * pstFrame->u32Height / 2 / 2);

    /* write V */
    HI_DRV_FILE_Write(pfYUV, pu8Vdata, pstFrame->u32Width * pstFrame->u32Height / 2 / 2);

    HI_KFREE(HI_ID_VENC, pu8Udata);
    HI_KFREE(HI_ID_VENC, pu8Vdata);
    HI_KFREE(HI_ID_VENC, pu8Ydata);

    HI_DRV_MMZ_Unmap(&stMBuf);

    return HI_SUCCESS;
ERR2:
    HI_KFREE(HI_ID_VENC, pu8Vdata);
ERR1:
    HI_KFREE(HI_ID_VENC, pu8Udata);
ERR0:
    HI_DRV_MMZ_Unmap(&stMBuf);
    return HI_FAILURE;
}

#endif

#if 0
static HI_S32 VENC_DRV_ProcSaveYuv(HI_HANDLE hVenc)
{

    HI_U32 u32Chn;
    HI_S8 FileName[64];
    static HI_U32 u32Cnt = 0;
    struct file* fp;
    HI_S32 Ret;
    HI_UNF_VIDEO_FRAME_INFO_S* pstFrame;

    GET_PORT_CHN(hVenc, enPort, u32Chn);
    pstFrame = &g_ViDrv[enPort][u32Chn].stFrame[0];

    Ret = HI_OSAL_Snprintf(FileName, sizeof(FileName), "/mnt/vi_%dx%d_%02d.yuv",
                           pstFrame->u32Width, pstFrame->u32Height, u32Cnt++);

    if (0 == Ret)
    {
        HI_ERR_VENC("HI_OSAL_Snprintf failed\n");
        return HI_FAILURE;
    }

    fp = HI_DRV_FILE_Open(FileName, 1);

    if (fp)
    {
        Ret = VENC_DRV_ProcSaveYuv_k(fp, pstFrame);
        HI_DRV_FILE_Close(fp);
        HI_PRINT("save image in %s\n", FileName);
    }
    else
    {
        HI_ERR_VENC("cannot open file %s!\n", FileName);
        Ret = HI_FAILURE;
    }

    return Ret;
}
#endif


static HI_S32 str2val(char* str, unsigned int* data)
{
    unsigned int i, d, dat, weight;

    dat = 0;

    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        i = 2;
        weight = 16;
    }
    else
    {
        i = 0;
        weight = 10;
    }

    for (; i < 10; i++)
    {
        if (str[i] < 0x20) { break; }
        else if (weight == 16 && str[i] >= 'a' && str[i] <= 'f')
        {
            d = str[i] - 'a' + 10;
        }
        else if (weight == 16 && str[i] >= 'A' && str[i] <= 'F')
        {
            d = str[i] - 'A' + 10;
        }
        else if (str[i] >= '0' && str[i] <= '9')
        {
            d = str[i] - '0';
        }
        else
        {
            return -1;
        }

        dat = dat * weight + d;
    }

    *data = dat;

    return 0;
}

static HI_S32 val2str(unsigned int data, int radix, char* str)
{
    char tmp[32] = {'\0'};
    unsigned int tmpval = data;
    int i, j;

    for (i = 0; i < 32; i++)
    {
        tmp[i] = (tmpval % radix) + '0';
        tmpval = tmpval / radix;

        if (tmpval == 0)
        { break; }
    }

    for (j = 0; (i >= 0) && (i < 32); i--) { str[j++] = tmp[i]; }

    str[j] = '\0';
    return 0;
}

static HI_U32 get_current_level(int EncWidth, int EncHeight)
{
    HI_U32 TotalMb = 0;
    HI_U32 WidthInMB = (EncWidth + 15) >> 4;
    HI_U32 HeightInMB = (EncHeight + 15) >> 4;
    HI_U32 level_idc;
    TotalMb = WidthInMB * HeightInMB;

    if (TotalMb <= 99)
    {
        level_idc = 10;
    }
    else if (TotalMb <= 396)
    {
        level_idc = 20;
    }
    else if (TotalMb <= 792)
    {
        level_idc = 21;
    }
    else if (TotalMb <= 1620)
    {
        level_idc = 30;
    }
    else if (TotalMb <= 3600)
    {
        level_idc = 31;
    }
    else if (TotalMb <= 5120)
    {
        level_idc = 32;
    }
    else if (TotalMb <= 8192)
    {
        level_idc = 41;
    }
    else
    {
        level_idc = 0;
    }

    return level_idc;
}


static HI_S32 VENC_DRV_ProcRead(struct seq_file* p, HI_VOID* v)
{
    HI_S32 s32Ret = HI_FAILURE;
    DRV_PROC_ITEM_S* pProcItem;
    VeduEfl_EncPara_S* pEncPara = NULL;
    VeduEfl_EncPara_S* hVenc;
    HI_U32 u32ChnID = 0;
    HI_U32 u32SkipFrmNum = 0;
    HI_U32 u32LevelID = 0;
    VeduEfl_StatInfo_S StatInfo;
    VeduEfl_StatInfo_S* pStatInfo = &StatInfo;
    HI_U32  srcID;
    HI_CHAR srcTab[4][8] = {{"VI"}, {"Win"}, {"DISP"}, {"User"}};

    HI_CHAR szProtocol[][8] = {"MPEG2", "MPEG4", "AVS",  "H.263",    "H.264", "REAL8", "REAL9",
                               "VC1",   "VP6", "VP6F", "VP6A", "SORENSON", "MJPEG", "DIVX3", "RAW",   "JPEG",  "UNKNOWN"
                              };
    HI_CHAR szAVCProfile[][16] = {"Baseline", "Main", "Extended", "High", "UNKNOWN"};
    HI_CHAR szEncodeLevel[][8] = {"QCIF", "CIF", "D1",  "720P", "1080P", "UNKNOWN"};

    HI_CHAR szBoolTab[][8] = {"FALSE", "TRUE"};
    HI_CHAR szStateTab[][8] = {"Stop", "Start"};
    HI_CHAR szPixFormat[][16] = {"SP420_UV", "SP420_VU", "Planer420",  "Planer422", "Package422_YUYV", "Package422_UYVY", "Package422_YVYU", "SP422", "UNKNOWN"};
    HI_CHAR szBufStateTab[][8] = {"NA", "NA", "NA", "NA", "NA", "NA"};
    HI_CHAR szMaxMinQP[][8] = {"NA", "NA"};

    HI_CHAR szRCSkipThr[8] = "";

    pProcItem = p->private;
    hVenc = (VeduEfl_EncPara_S*)pProcItem->data;

    D_VENC_GET_CHN(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);

    pEncPara = (VeduEfl_EncPara_S*)g_stVencChn[u32ChnID].hVEncHandle;

    s32Ret = VENC_DRV_EflQueryStatInfo(g_stVencChn[u32ChnID].hVEncHandle, pStatInfo);

    if (s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    switch (g_stVencChn[u32ChnID].enSrcModId)
    {
        case HI_ID_VI:
            srcID = 0;
            break;

        case HI_ID_VO:
            srcID = 1;
            break;

        case HI_ID_DISP:
            srcID = 2;
            break;

        default:
            srcID = 3;
            break;
    }

    if ((3 == srcID) && (!g_stVencChn[u32ChnID].bWithoutVPSS))
    {
        memcpy(szBufStateTab[0], "6", 2);
        memcpy(szBufStateTab[1], "6", 2);
        memcpy(szBufStateTab[2], "NA", 3);
        val2str(pStatInfo->VpssQueueBufNum, 10, szBufStateTab[3]);
        val2str(pStatInfo->VpssImgBufNum, 10, szBufStateTab[4]);
        memcpy(szBufStateTab[5], "NA", 3);

    }
    else
    {
        memcpy(szBufStateTab[0], "NA", 3);
        memcpy(szBufStateTab[1], "NA", 3);
        memcpy(szBufStateTab[2], "6", 2);
        memcpy(szBufStateTab[3], "NA", 3);
        memcpy(szBufStateTab[4], "NA", 3);
        val2str(pStatInfo->QueueNum, 10, szBufStateTab[5]);
    }

    if (g_stVencChn[u32ChnID].stChnUserCfg.enVencType == HI_UNF_VCODEC_TYPE_JPEG)
    {
        /* CBR */
        memcpy(szMaxMinQP[0], "NA", 3);
        memcpy(szMaxMinQP[1], "NA", 3);
    }
    else
    {
        /* VBR */
        val2str( g_stVencChn[u32ChnID].stChnUserCfg.u32MaxQp, 10, szMaxMinQP[0]);
        val2str( g_stVencChn[u32ChnID].stChnUserCfg.u32MinQp, 10, szMaxMinQP[1]);
    }

    if (g_stVencChn[u32ChnID].stChnUserCfg.u32DriftRateThr > 100)
    {
        memcpy(szRCSkipThr, "NA", 3);
    }
    else
    {
        val2str(g_stVencChn[u32ChnID].stChnUserCfg.u32DriftRateThr, 10, szRCSkipThr);
        HI_OSAL_Strncat(szRCSkipThr, "%", 2);
    }

    u32LevelID = get_current_level(g_stVencChn[u32ChnID].stChnUserCfg.u32Width, g_stVencChn[u32ChnID].stChnUserCfg.u32Height);
    u32SkipFrmNum = pStatInfo->QuickEncodeSkip + pStatInfo->ErrCfgSkip + pStatInfo->FrmRcCtrlSkip
                    + pStatInfo->TooFewBufferSkip + pStatInfo->SamePTSSkip + pStatInfo->TooManyBitsSkip;

    PROC_PRINT(p, "--------------------- VENC[%02d] -----------------------\n", u32ChnID);
    PROC_PRINT(p, "-------------------- User  Config ----------------------\n");
    PROC_PRINT(p,
               "Version                       :%s\n"
               "CodecID                       :%s(0x%x)\n"
               "Capability                    :%s\n"
               "Profile(Level)                :%s(%d)\n"
               "Resolution                    :%uX%u\n"
               "TargetBitRate                 :%u(kbps)\n"
               "Gop                           :%u\n"
               "FrmRate(Input/OutPut)         :%u/%u(fps)\n"
               "priority                      :%u\n"
               "QuickEncode                   :%s\n"
               "DriftRateThr                  :%s\n"
               "Split                         :Enable(%s)	Size(%u byte)\n"
               "StreamBufSize                 :%u(KB)\n"
               "MaxQP/MinQP                   :%s/%s\n"
               "QLevel                        :%u\n"
               "Rotation                      :%u\n"
               "AutoRequestIFrm               :Enable(%s)\n"
               ,
               VENC_FIRMWARE_VERSION,
               szProtocol[g_stVencChn[u32ChnID].stChnUserCfg.enVencType], g_stVencChn[u32ChnID].stChnUserCfg.enVencType,
               szEncodeLevel[g_stVencChn[u32ChnID].stChnUserCfg.enCapLevel],
               szAVCProfile[g_stVencChn[u32ChnID].stChnUserCfg.enVencProfile], u32LevelID,
               g_stVencChn[u32ChnID].stChnUserCfg.u32Width,
               g_stVencChn[u32ChnID].stChnUserCfg.u32Height,
               g_stVencChn[u32ChnID].stChnUserCfg.u32TargetBitRate / 1000U,
               g_stVencChn[u32ChnID].stChnUserCfg.u32Gop,
               g_stVencChn[u32ChnID].stChnUserCfg.u32InputFrmRate, g_stVencChn[u32ChnID].stChnUserCfg.u32TargetFrmRate,
               g_stVencChn[u32ChnID].stChnUserCfg.u8Priority,
               szBoolTab[g_stVencChn[u32ChnID].stChnUserCfg.bQuickEncode],
               szRCSkipThr,
               szBoolTab[g_stVencChn[u32ChnID].stChnUserCfg.bSlcSplitEn], g_stVencChn[u32ChnID].u32SliceSize ,
               g_stVencChn[u32ChnID].stChnUserCfg.u32StrmBufSize / 1000U,
               szMaxMinQP[0], szMaxMinQP[1],
               g_stVencChn[u32ChnID].stChnUserCfg.u32Qlevel,
               g_stVencChn[u32ChnID].stChnUserCfg.u32RotationAngle,
               szBoolTab[pEncPara->ICatchEnable]);
    PROC_PRINT(p, "------------------ Real-time  Statistics ----------------------\n");
    PROC_PRINT(p,
               "WorkStatus                    :%s\n"
               "SourceID                      :%s%02u\n"
               "FrameInfo                     :%s\n"
               "InputFrmRate(Use/Real)        :%u/%u(fps)\n"
               "TargetFrmRate(Use/Real)       :%u/%u(fps)\n"
               "BitRate                       :%u(kbps)\n"
               "EncodeNum                     :%u\n"
               "SkipNum                       :Total(%u) FrmRateCtrl(%u) SamePTS(%u) QuickEncode(%u) TooFewBuf(%u) TooManyBits(%u),ErrCfg(%u)\n"
               "TooFewBufferWaitCnt           :%d\n"
               "TooFewBufferQueFailCnt        :%d\n"
               "QueueFrmByAttachCnt           :%d\n"
               "FrameBuffer:\n"
               "    VpssQueueBuf(Total/Used)  :%s/%s\n"
               "    VpssImgBuf  (Total/Used)  :%s/%s\n"
               "    VencQueueBuf(Total/Used)  :%s/%s\n"
               "StreamBuffer:\n"
               "    Total/Used/Percent(Bytes) :%u/%u/%u%%\n"
               "Statistics(Total):\n"
               "    AcquireFrame(Try/OK)      :%d/%d\n"
               "    ReleaseFrame(Try/OK)      :%d/%d\n"
               "    AcquireStream(Try/OK)     :%d/%d\n"
               "    ReleaseStream(Try/OK)     :%d/%d\n"
               "Statistics(PerSecond):\n"
               "    AcquireFrame(Try/OK)      :%d/%d\n"
               "    ReleaseFrame(Try/OK)      :%d/%d\n"
               "    AcquireStream(Try/OK)     :%d/%d\n"
               "    ReleaseStream(Try/OK)     :%d/%d\n"
               ,
               szStateTab[g_stVencChn[u32ChnID].bEnable],
               srcTab[srcID], (g_stVencChn[u32ChnID].hSource == HI_INVALID_HANDLE ? 0 : g_stVencChn[u32ChnID].hSource & 0xff),
               szPixFormat[pStatInfo->u32FrameType],
               pStatInfo->u32RealSendInputRrmRate, g_stVencChn[u32ChnID].u32LastSecInputFps,
               pStatInfo->u32RealSendOutputFrmRate, g_stVencChn[u32ChnID].u32LastSecEncodedFps,
               g_stVencChn[u32ChnID].u32LastSecKbps * 8 / 1000U,
               (pStatInfo->GetFrameNumOK - u32SkipFrmNum),
               u32SkipFrmNum,
               pStatInfo->FrmRcCtrlSkip, pStatInfo->SamePTSSkip, pStatInfo->QuickEncodeSkip,
               pStatInfo->TooFewBufferSkip, pStatInfo->TooManyBitsSkip, pStatInfo->ErrCfgSkip,
               pStatInfo->TooFewBufferWaitCnt, pStatInfo->TooFewBufferQueFailCnt, pStatInfo->QueueFrmByAttachCnt,
               szBufStateTab[0], szBufStateTab[3],
               szBufStateTab[1], szBufStateTab[4],
               szBufStateTab[2], szBufStateTab[5],
               g_stVencChn[u32ChnID].stChnUserCfg.u32StrmBufSize, pStatInfo->UsedStreamBuf, pStatInfo->UsedStreamBuf * 100 / g_stVencChn[u32ChnID].stChnUserCfg.u32StrmBufSize,
               pStatInfo->GetFrameNumTry, pStatInfo->GetFrameNumOK,
               pStatInfo->PutFrameNumTry, pStatInfo->PutFrameNumOK,
               pStatInfo->GetStreamNumTry, pStatInfo->GetStreamNumOK,
               pStatInfo->PutStreamNumTry, pStatInfo->PutStreamNumOK,

               g_stVencChn[u32ChnID].u32LastSecTryNum, g_stVencChn[u32ChnID].u32LastSecOKNum,
               g_stVencChn[u32ChnID].u32LastSecPutNum, g_stVencChn[u32ChnID].u32LastSecPutNum,
               g_stVencChn[u32ChnID].u32LastSecStrmGetTryNum, g_stVencChn[u32ChnID].u32LastSecStrmGetOKNum,
               g_stVencChn[u32ChnID].u32LastSecStrmRlsTryNum, g_stVencChn[u32ChnID].u32LastSecStrmRlsOKNum);

    return HI_SUCCESS;
}


static void VENC_DRV_ProcWrite_SaveYuv(HI_CHAR *str, HI_U32 u32ChnID)
{
    if (!HI_OSAL_Strncmp(str,"start",256))
    {
       g_stVencChn[u32ChnID].stProcWrite.bSaveYUVFileRun = HI_TRUE;
    }
    else if (!HI_OSAL_Strncmp(str,"stop",256))
    {
       g_stVencChn[u32ChnID].stProcWrite.bSaveYUVFileRun = HI_FALSE;
    }
    else
    {
       VENC_DRV_ProcHelp();
    }

    return;
}

static HI_S32 VENC_DRV_ProcWrite_TimeModeSecond(HI_S32 num, size_t count, HI_CHAR *buf, HI_U32 u32ChnID)
{
    HI_CHAR str3[256];
    HI_CHAR FileName[64];
    HI_CHAR s_VencSavePath[64];
    HI_U32 parm;    
    HI_U32 j = 0;
    HI_U32 i = num;

    memset(str3, 0, sizeof(str3));
    memset(FileName, 0, sizeof(FileName));
    memset(s_VencSavePath, 0, sizeof(s_VencSavePath));
    
    for (; i < count; i++)
    {
        if (j == 0 && buf[i] == ' ') { continue; }
        if (buf[i] > ' ') { str3[j++] = buf[i]; }
        if (j > 0 && buf[i] == ' ') { break; }
    }
    str3[j] = '\0';
    
    if(str2val(str3, &parm) != 0)
    {
        HI_ERR_VENC("error: echo cmd '%s' is worng!\n", buf);
        
        return HI_FAILURE;
    }

    if ( parm > 3600 )
    {
        HI_ERR_VENC("error: not support save too large stream file!\n");
        
        return HI_FAILURE;
    }

    HI_DRV_LOG_GetStorePath(s_VencSavePath, 64);
    HI_OSAL_Snprintf(FileName, 64, "%s/%s", s_VencSavePath,g_stVencChn[u32ChnID].stProcWrite.StreamFileName);
    g_stVencChn[u32ChnID].stProcWrite.bTimeModeRun = HI_TRUE;
    g_stVencChn[u32ChnID].stProcWrite.fpSaveFile = VENC_DRV_OsalFopen(FileName,  O_RDWR | O_CREAT|O_APPEND, 0);
    if (g_stVencChn[u32ChnID].stProcWrite.fpSaveFile == HI_NULL)
    {
        HI_ERR_VENC("Can not create %s file.\n", FileName);
        g_stVencChn[u32ChnID].stProcWrite.bTimeModeRun = HI_FALSE;
        
        return HI_FAILURE;
    }

    msleep(1000 * parm);
    g_stVencChn[u32ChnID].stProcWrite.bTimeModeRun = HI_FALSE;
    VENC_DRV_OsalFclose(g_stVencChn[u32ChnID].stProcWrite.fpSaveFile);

    return count;

}

static HI_S32 VENC_DRV_ProcWrite_TimeModeFrame(HI_S32 num, size_t count, HI_CHAR *buf, HI_U32 u32ChnID)
{
    HI_CHAR str3[256];
    HI_CHAR FileName[64];
    HI_CHAR s_VencSavePath[64];
    HI_U32 parm;    
    HI_U32 j = 0;
    HI_U32 i = num;
    HI_U32 cnt = 0;

    memset(str3, 0, sizeof(str3));
    memset(FileName, 0, sizeof(FileName));
    memset(s_VencSavePath, 0, sizeof(s_VencSavePath));

    for (; i < count; i++)
    {
        if (j == 0 && buf[i] == ' ') { continue; }
        if (buf[i] > ' ') { str3[j++] = buf[i]; }
        if (j > 0 && buf[i] == ' ') { break; }
    }
    str3[j] = '\0';
    
    if (str2val(str3, &parm) != 0)
    {
        HI_ERR_VENC("error: echo cmd '%s' is worng!\n", buf);
        
        return HI_FAILURE;
    }

    if (parm > 100000)
    {
        HI_ERR_VENC("error: not support save too large YUV file!\n");
        
        return HI_FAILURE;
    }

    HI_DRV_LOG_GetStorePath(s_VencSavePath, 64);
    HI_OSAL_Snprintf(FileName, 64, "%s/%s", s_VencSavePath,g_stVencChn[u32ChnID].stProcWrite.StreamFileName);
    g_stVencChn[u32ChnID].stProcWrite.bFrameModeRun = HI_TRUE;
    g_stVencChn[u32ChnID].stProcWrite.fpSaveFile = VENC_DRV_OsalFopen(FileName,  O_RDWR | O_CREAT|O_APPEND, 0);

    if (g_stVencChn[u32ChnID].stProcWrite.fpSaveFile == HI_NULL)
    {
        HI_ERR_VENC("Can not create %s file.\n", FileName);
        g_stVencChn[u32ChnID].stProcWrite.bFrameModeRun = HI_FALSE;
        
        return HI_FAILURE;
    }

    g_stVencChn[u32ChnID].stProcWrite.u32FrameModeCount = parm;
    while (1)
    {
        /* if the frame count reaches to aim, break */
        if (g_stVencChn[u32ChnID].stProcWrite.bFrameModeRun == HI_FALSE)
        {
            break;
        }
        else
        {
            cnt++;
            if (cnt == 6000)
            {
                HI_ERR_VENC("Time of saving frame is 60s,stop saving!\n");
                break;
            }              
            msleep(100);
        }
    }
    VENC_DRV_OsalFclose(g_stVencChn[u32ChnID].stProcWrite.fpSaveFile);

    return count;
}

static HI_S32 VENC_DRV_ProcWrite_SaveStream(HI_CHAR *str, HI_S32 i, size_t count, HI_CHAR *buf, HI_U32 u32ChnID)
{
    if (!HI_OSAL_Strncmp(str,"second",256))   /*time mode*/
    {
       return VENC_DRV_ProcWrite_TimeModeSecond(i, count, buf, u32ChnID);
    }
    else if (!HI_OSAL_Strncmp(str,"frame",256))
    {
       return VENC_DRV_ProcWrite_TimeModeFrame(i, count, buf, u32ChnID);
    }
    else
    {
       VENC_DRV_ProcHelp();
    }

    return HI_SUCCESS;
}

static void VENC_DRV_ProcWrite_ClkGateEn(HI_CHAR *str, VeduEfl_EncPara_S* hVenc)
{
    VeduEfl_EncPara_S *pEncPara  = (VeduEfl_EncPara_S *)hVenc;
    if (!HI_OSAL_Strncmp(str,"0",256))
    {
       pEncPara->ClkGateEn = 0;
    }
    else if (!HI_OSAL_Strncmp(str,"1",256))
    {
       pEncPara->ClkGateEn = 1;
    }
    else if (!HI_OSAL_Strncmp(str,"2",256))
    {
       pEncPara->ClkGateEn = 2;
    }
    else
    {
       VENC_DRV_ProcHelp();
    }

    return;
}

static void VENC_DRV_ProcWrite_LowPowEn(HI_CHAR *str, VeduEfl_EncPara_S* hVenc)
{
    VeduEfl_EncPara_S *pEncPara  = (VeduEfl_EncPara_S *)hVenc;
    if (!HI_OSAL_Strncmp(str,"0",256))
    {
        pEncPara->IntraLowpowEn    = 0;
        pEncPara->intpLowpowEn     = 0;
        pEncPara->fracLowpowEn     = 0;
    }
    else if (!HI_OSAL_Strncmp(str,"1",256))
    {
        pEncPara->IntraLowpowEn    = 1;
        pEncPara->intpLowpowEn     = 1;
        pEncPara->fracLowpowEn     = 1;
    }
    else
    {
       VENC_DRV_ProcHelp();
    }

    return;
}

static void VENC_DRV_ProcWrite_WithoutVPSS(HI_CHAR* str, VeduEfl_EncPara_S* hVenc, HI_U32 u32ChnID)
{
    VeduEfl_EncPara_S*  pEncPara  = (VeduEfl_EncPara_S*)hVenc;
    VPSS_HANDLE hVPSS;
    VPSS_HANDLE hPort0;
    HI_DRV_VPSS_CFG_S stVPSSCfg;
    HI_DRV_VPSS_PORT_CFG_S stPort0Cfg;
    HI_S32 Ret = HI_SUCCESS;

    if (!HI_OSAL_Strncmp(str, "0", 256))
    {
        if (pEncPara->WithoutVPSS)
        {
            if (!g_stVencChn[u32ChnID].bEnable)  /*if the channel not start! OK*/
            {
                /* without VPSS --> with VPSS*/

                /* create VPSS Instance*/
                if (g_stVencChn[u32ChnID].hVPSS == HI_INVALID_HANDLE)
                {
                    /*create VPSS instance*/
                    Ret |= (pVpssFunc->pfnVpssGetDefaultCfg)(&stVPSSCfg);
                    stVPSSCfg.enProgInfo  = HI_DRV_VPSS_PRODETECT_PROGRESSIVE;
                    Ret |= (pVpssFunc->pfnVpssCreateVpss)(&stVPSSCfg, &hVPSS);
                    Ret |= (pVpssFunc->pfnVpssSendCommand)(hVPSS, HI_DRV_VPSS_USER_COMMAND_START, NULL);

                    if (Ret == HI_SUCCESS)
                    {
                        g_stVencChn[u32ChnID].hVPSS = hVPSS;
                    }
                }

                /* create VPSS port[0]*/
                if (g_stVencChn[u32ChnID].hPort[0] == HI_INVALID_HANDLE)
                {
                    Ret |= (pVpssFunc->pfnVpssGetDefaultPortCfg)(&stPort0Cfg);
                    stPort0Cfg.s32OutputWidth            = pEncPara->PicWidth;
                    stPort0Cfg.s32OutputHeight           = pEncPara->PicHeight;
                    stPort0Cfg.u32MaxFrameRate           = g_stVencChn[u32ChnID].stChnUserCfg.u32TargetFrmRate;//pstCreateInfo->stAttr.u32InputFrmRate;
                    stPort0Cfg.eDstCS                    = HI_DRV_CS_BT709_YUV_LIMITED;
                    stPort0Cfg.stBufListCfg.eBufType     = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;
                    stPort0Cfg.stBufListCfg.u32BufNumber = 6;
                    stPort0Cfg.stBufListCfg.u32BufSize   = 2 * (pEncPara->PicWidth * pEncPara->PicHeight);
                    stPort0Cfg.stBufListCfg.u32BufStride = (pEncPara->PicWidth + (15)) & ~(15);           //16对齐
                    //stPort0Cfg.eAspMode                = HI_DRV_ASP_RAT_MODE_FULL;                      /*不同比例缩放策略*/
                    stPort0Cfg.eFormat                   = HI_DRV_PIX_FMT_NV21;                           /* 21  Y/CrCb 4:2:0  */
                    Ret |= (pVpssFunc->pfnVpssCreatePort)(g_stVencChn[u32ChnID].hVPSS, &stPort0Cfg, &hPort0);

                    if (Ret == HI_SUCCESS)
                    {
                        g_stVencChn[u32ChnID].hPort[0]    = hPort0;
                    }

                    /*注册vpss回调函数*/
                    Ret |= (pVpssFunc->pfnVpssRegistHook)(g_stVencChn[u32ChnID].hVPSS, g_stVencChn[u32ChnID].hUsrHandle, VENC_VpssEventHandle);
                    Ret |= (pVpssFunc->pfnVpssSetSourceMode)(g_stVencChn[u32ChnID].hVPSS, VPSS_SOURCE_MODE_USERACTIVE, HI_NULL);
                    if (Ret != HI_SUCCESS)
                    {
                        HI_ERR_VENC("pfnVpssSetSourceMode VPSS_SOURCE_MODE_USERACTIVE ERROR!\n");
                    }

                    (pVpssFunc->pfnVpssEnablePort)(g_stVencChn[u32ChnID].hPort[0], HI_TRUE);
                }

                if (Ret == HI_SUCCESS)
                {
                    g_stVencChn[u32ChnID].bWithoutVPSS = HI_FALSE;
                    pEncPara->WithoutVPSS = 0;
                }
            }
        }
    }
    else
    {
        if (!pEncPara->WithoutVPSS)
        {
            if (!g_stVencChn[u32ChnID].bEnable)  /*if the channel not start! OK*/
            {
                /* with VPSS --> without VPSS*/
                /* destory VPSS Port[0] & VPSS Instance */
                if (g_stVencChn[u32ChnID].hPort[0] != HI_INVALID_HANDLE)
                {
                    (pVpssFunc->pfnVpssEnablePort)(g_stVencChn[u32ChnID].hPort[0], HI_FALSE);
                    Ret |= (pVpssFunc->pfnVpssDestroyPort)(g_stVencChn[u32ChnID].hPort[0]);

                    if (Ret == HI_SUCCESS)
                    {
                        g_stVencChn[u32ChnID].hPort[0] = HI_INVALID_HANDLE;
                    }
                }

                if (g_stVencChn[u32ChnID].hVPSS != HI_INVALID_HANDLE)
                {
                    Ret |= (pVpssFunc->pfnVpssSendCommand)(g_stVencChn[u32ChnID].hVPSS, HI_DRV_VPSS_USER_COMMAND_STOP, NULL);
                    Ret |= (pVpssFunc->pfnVpssDestroyVpss)(g_stVencChn[u32ChnID].hVPSS);

                    if (Ret == HI_SUCCESS)
                    {
                        g_stVencChn[u32ChnID].hVPSS = HI_INVALID_HANDLE;
                    }
                }

                if (Ret == HI_SUCCESS)
                {
                    g_stVencChn[u32ChnID].bWithoutVPSS = HI_TRUE;
                    pEncPara->WithoutVPSS = 1;
                }
            }
        }
    }

    return ;
}

static void VENC_DRV_ProcWrite_ParseOneWord(HI_S32 count, HI_S32 *ppos, HI_CHAR *buf, HI_CHAR *str)
{
    HI_S32 j = 0;
    HI_S32 i = *ppos;
    for (; i < count; i++)
    {
        if (j == 0 && buf[i] == ' ') { continue; }
        if (buf[i] > ' ') { str[j++] = buf[i]; }
        if (j > 0 && buf[i] == ' ') { break; }
	}
	str[j] = '\0';
    *ppos = i;
    
    return;
}

static HI_S32 VENC_DRV_ProcWrite(struct file *file, const char __user* buffer, size_t count, loff_t *ppos)
{
    struct seq_file *q = file->private_data;
    DRV_PROC_ITEM_S *pProcItem = q->private;
    VeduEfl_EncPara_S *hVenc;
    static HI_CHAR buf[256];
    static HI_CHAR str1[256];
    static HI_CHAR str2[256];
    HI_U32 u32ChnID;
    HI_S32 pos = 0;
    
    memset(buf, 0, sizeof(buf));
    memset(str1, 0, sizeof(str1));
    memset(str2, 0, sizeof(str2));

    hVenc = (VeduEfl_EncPara_S*)(pProcItem->data);
    D_VENC_GET_CHN(u32ChnID, hVenc);

    if (u32ChnID >= VENC_MAX_CHN_NUM)
    {
        return HI_FAILURE;
    }

    if (count >= sizeof(buf))
    {
        HI_PRINT("MMZ: your echo parameter string is too long!\n");

        return -EIO;
    }

    if (count >= 1)
    {
        if (copy_from_user(buf, buffer, count))
        {
            HI_PRINT("MMZ: copy_from_user failed!\n");

            return -EIO;
        }

        buf[count] = '\0';

        /* dat1 */
        VENC_DRV_ProcWrite_ParseOneWord(count, &pos, buf, str1);

        /* dat2 */
        VENC_DRV_ProcWrite_ParseOneWord(count, &pos, buf, str2);

        if (!HI_OSAL_Strncmp(str1, "save_yuv", 256))
        {
            VENC_DRV_ProcWrite_SaveYuv(str2, u32ChnID);
        }
        else if (!HI_OSAL_Strncmp(str1, "save_stream", 256))
        {
            return VENC_DRV_ProcWrite_SaveStream(str2, pos, count, buf, u32ChnID);
        }
        else if (!HI_OSAL_Strncmp(str1, "ClkGateEn", 256))
        {
            VENC_DRV_ProcWrite_ClkGateEn(str2, hVenc);
        }
        else if (!HI_OSAL_Strncmp(str1, "LowPowEn", 256))
        {
            VENC_DRV_ProcWrite_LowPowEn(str2, hVenc);
        }
        else if (!HI_OSAL_Strncmp(str1, "WithoutVPSS", 256))
        {
            VENC_DRV_ProcWrite_WithoutVPSS(str2, hVenc, u32ChnID);
        }
        else
        {
            VENC_DRV_ProcHelp();
        }
    }
    else
    {
        VENC_DRV_ProcHelp();
    }

    return count;

}
#endif
HI_S32 VENC_DRV_ProcAdd(VeduEfl_EncPara_S* hVenc, HI_U32 u32ChnID)   /**/
{
#if (1 == HI_PROC_SUPPORT)
    HI_S32 Ret;
    DRV_PROC_ITEM_S* pProcItem;
    HI_CHAR ProcName[12];

    if (NULL == hVenc)
    {
        return HI_FAILURE;
    }

    Ret = HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "venc%02x", u32ChnID);

    if (0 == Ret)
    {
        HI_ERR_VENC("HI_OSAL_Snprintf failed!\n");
        return HI_FAILURE;
    }

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);

    if (!pProcItem)
    {
        HI_ERR_VENC("VENC add proc failed!\n");
        return HI_FAILURE;
    }

    pProcItem->data  = (HI_VOID*)hVenc;
    pProcItem->read  = VENC_DRV_ProcRead;
    pProcItem->write = VENC_DRV_ProcWrite;
#endif
    return HI_SUCCESS;
}

HI_VOID VENC_DRV_ProcDel(VeduEfl_EncPara_S* hVenc, HI_U32 u32ChnID)
{
#if (1 == HI_PROC_SUPPORT)
    HI_S32 Ret;
    HI_CHAR ProcName[12];

    if (NULL == hVenc)
    {
        return;
    }

    Ret = HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "venc%02x", u32ChnID);

    if (0 == Ret)
    {
        return;
    }

    HI_DRV_PROC_RemoveModule(ProcName);
#endif
}
