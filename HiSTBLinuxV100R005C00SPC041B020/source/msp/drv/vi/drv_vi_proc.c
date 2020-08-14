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
#include "hi_drv_vi.h"
#include "drv_vi.h"

extern VI_DRV_S g_ViDrv[MAX_VI_PORT][MAX_VI_CHN];

static HI_VOID VI_DRV_ProcHelp(HI_HANDLE hVi)
{
    HI_DRV_PROC_EchoHelper("echo save_yuv > /proc/msp/vi%04x\n", (HI_U32)(hVi & 0xffff));
    HI_DRV_PROC_EchoHelper("echo low_delay_stat start > /proc/msp/vi%04x\n", (HI_U32)(hVi & 0xffff));
    HI_DRV_PROC_EchoHelper("echo low_delay_stat stop > /proc/msp/vi%04x\n", (HI_U32)(hVi & 0xffff));
}

static HI_S32 VI_DRV_ProcSaveYuv_k(struct file *pfYUV, HI_UNF_VIDEO_FRAME_INFO_S *pstFrame)
{
    MMZ_BUFFER_S stMBuf;
    HI_S8 *ptr;
    HI_S32 nRet;
    HI_U8 *pu8Udata;
    HI_U8 *pu8Vdata;
    HI_U8 *pu8Ydata;
    HI_U32 i, j;
    HI_U32 u32Height;

    stMBuf.u32StartPhyAddr = pstFrame->stVideoFrameAddr[0].u32YAddr;
    if (!stMBuf.u32StartPhyAddr)
    {
        HI_ERR_VI("address '0x%x' is null!\n", pstFrame->stVideoFrameAddr[0].u32YAddr);
        return HI_FAILURE;
    }

    nRet = HI_DRV_MMZ_Map(&stMBuf);
    ptr = (HI_S8 *)stMBuf.pu8StartVirAddr;

    if (nRet)
    {
        HI_ERR_VI("address '0x%x' is not valid!\n", pstFrame->stVideoFrameAddr[0].u32YAddr);
        return HI_FAILURE;
    }
    if ((pstFrame->enVideoFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_420)
        ||(pstFrame->enVideoFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_420_UV))
    {
        u32Height = pstFrame->u32Height / 2;
    }
    else if((pstFrame->enVideoFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_422)
        ||(pstFrame->enVideoFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2))
    {
        u32Height = pstFrame->u32Height;
    }
    else
    {
        stMBuf.u32Size =  pstFrame->u32Height *  pstFrame->u32Width * 2;
        if (stMBuf.u32Size != HI_DRV_FILE_Write(pfYUV, ptr, stMBuf.u32Size))
        {
            HI_ERR_VI("line %d: fwrite fail!\n", __LINE__);
            goto ERR0;
        }

        HI_DRV_MMZ_Unmap(&stMBuf);

        return HI_SUCCESS;

    }

    pu8Udata = HI_KMALLOC(HI_ID_VI, pstFrame->u32Width * u32Height / 2, GFP_KERNEL);
    if (HI_NULL == pu8Udata)
    {
        goto ERR0;
    }

    pu8Vdata = HI_KMALLOC(HI_ID_VI, pstFrame->u32Width * u32Height / 2, GFP_KERNEL);
    if (HI_NULL == pu8Vdata)
    {
        goto ERR1;
    }

    pu8Ydata = HI_KMALLOC(HI_ID_VI, pstFrame->stVideoFrameAddr[0].u32YStride, GFP_KERNEL);
    if (HI_NULL == pu8Ydata)
    {
        goto ERR2;
    }

    /* write Y */
    for (i = 0; i < pstFrame->u32Height; i++)
    {
        memcpy(pu8Ydata, ptr, sizeof(HI_U8) * pstFrame->u32Width);

        if (pstFrame->u32Width != HI_DRV_FILE_Write(pfYUV, pu8Ydata, pstFrame->u32Width))
        {
            HI_ERR_VI("line %d: fwrite fail!\n", __LINE__);
        }

        ptr += pstFrame->stVideoFrameAddr[0].u32YStride;
    }

    /* U V transfer and save */
    for (i = 0; i < u32Height; i++)
    {
        for (j = 0; j < pstFrame->u32Width / 2; j++)
        {
            pu8Vdata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j];
            pu8Udata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j + 1];
        }

        ptr += pstFrame->stVideoFrameAddr[0].u32CStride;
    }

    /* write U */
    HI_DRV_FILE_Write(pfYUV, pu8Udata, pstFrame->u32Width * u32Height / 2);

    /* write V */
    HI_DRV_FILE_Write(pfYUV, pu8Vdata, pstFrame->u32Width * u32Height / 2);

    HI_KFREE(HI_ID_VI, pu8Udata);
    HI_KFREE(HI_ID_VI, pu8Vdata);
    HI_KFREE(HI_ID_VI, pu8Ydata);

    HI_DRV_MMZ_Unmap(&stMBuf);

    return HI_SUCCESS;
ERR2:
    HI_KFREE(HI_ID_VI, pu8Vdata);
ERR1:
    HI_KFREE(HI_ID_VI, pu8Udata);
ERR0:
    HI_DRV_MMZ_Unmap(&stMBuf);
    return HI_FAILURE;
}

static HI_S32 VI_DRV_ProcSaveYuv(HI_HANDLE hVi)
{
    HI_UNF_VI_E enPort;
    HI_U32 u32Chn;
    HI_S8 FileName[64];
    static HI_U32 u32Cnt = 0;
    struct file *fp;
    HI_S32 Ret;
    HI_UNF_VIDEO_FRAME_INFO_S *pstFrame;

    GET_PORT_CHN(hVi, enPort, u32Chn);
    pstFrame = &g_ViDrv[enPort][u32Chn].stFrame[0];

    Ret = HI_DRV_FILE_GetStorePath(FileName, 64);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VI("get path failed\n");
        return HI_FAILURE;
    }

    Ret = HI_OSAL_Snprintf(FileName, sizeof(FileName), "%s/vi_%dx%d_%02d.yuv",
                           FileName, pstFrame->u32Width, pstFrame->u32Height, u32Cnt++);
    if (0 == Ret)
    {
        HI_ERR_VI("HI_OSAL_Snprintf failed\n");
        return HI_FAILURE;
    }

    fp = HI_DRV_FILE_Open(FileName, 1);
    if (fp)
    {
        Ret = VI_DRV_ProcSaveYuv_k(fp, pstFrame);
        HI_DRV_FILE_Close(fp);
        HI_DRV_PROC_EchoHelper("save image in %s\n", FileName);
    }
    else
    {
        HI_ERR_VI("cannot open file %s!\n", FileName);
        Ret = HI_FAILURE;
    }

    return Ret;
}

HI_S32 VI_DRV_ParseProcPara(HI_CHAR *pProcPara,HI_CHAR **ppArg1,HI_CHAR **ppArg2)
{
    HI_CHAR *pChar = HI_NULL;

    if (strlen(pProcPara) == 0)
    {
        /* not fined arg1 and arg2, return failed */
        *ppArg1  = HI_NULL;
        *ppArg2  = HI_NULL;
        return HI_FAILURE;
    }

    /* find arg1 */
    pChar = pProcPara;
    while( (*pChar == ' ') && (*pChar != '\0') )
    {
        pChar++;
    }

    if (*pChar != '\0')
    {
        *ppArg1 = pChar;
    }
    else
    {
        *ppArg1  = HI_NULL;

        return HI_FAILURE;
    }

    /* ignor arg1 */
    while( (*pChar != ' ') && (*pChar != '\0') )
    {
        pChar++;
    }

    /* Not find arg2, return */
    if (*pChar == '\0')
    {
        *ppArg2 = HI_NULL;

        return HI_SUCCESS;
    }

    /* add '\0' for arg1 */
    *pChar = '\0';

    /* start to find arg2 */
    pChar = pChar + 1;
    while( (*pChar == ' ') && (*pChar != '\0') )
    {
        pChar++;
    }

    if (*pChar != '\0')
    {
        *ppArg2 = pChar;
    }
    else
    {
        *ppArg2 = HI_NULL;
    }

    return HI_SUCCESS;
}

static HI_S32 VI_DRV_ProcRead(struct seq_file *p, HI_VOID *v)
{
    DRV_PROC_ITEM_S *pProcItem;
    VI_STATISTIC_S *pStatInfo;
    VI_DRV_S *q;
    HI_HANDLE hVi;
    HI_UNF_VI_E enPort = HI_UNF_VI_PORT0;
    HI_U32 u32Chn = 0;
    HI_U32 i;
    HI_CHAR inputMode[][20] =
    {
        {"BT656_576I"     }, {"BT656_480I"     }, {"BT601_576I"     }, {"BT601_480I"     },
        {"BT1120_640X480P"}, {"BT1120_480P"    }, {"BT1120_576P"    }, {"BT1120_720P_50" },
        {"BT1120_720P_60" }, {"BT1120_1080I_50"}, {"BT1120_1080I_60"}, {"BT1120_1080P_25"},
        {"BT1120_1080P_30"}, {"BT1120_1080P_50"}, {"BT1120_1080P_60"}, {"DC_480P_30"     },
        {"DC_576P_30"     }, {"DC_720P_30"     }, {"DC_1080P_30"}, {"HDMI_RX"},
    };
    HI_CHAR videoFmt[][20] =
    {
        {"SEMIPLANAR_422"    }, {"SEMIPLANAR_420"}, {"SEMIPLANAR_400"	}, {"SEMIPLANAR_411"},
        {"SEMIPLANAR_422_1X2"}, {"SEMIPLANAR_444"}, {"SEMIPLANAR_420_UV"}, {"PACKAGE_UYVY"	},
        {"PACKAGE_YUYV"      }, {"PACKAGE_YVYU"  }, {"PLANAR_400"		}, {"PLANAR_411"	},
        {"PLANAR_420"        }, {"PLANAR_422_1X2"}, {"PLANAR_422_2X1"	}, {"PLANAR_444"	},
        {"PLANAR_410"        },
    };
    VI_FB_BUF_PROC stBufProc;

    pProcItem = p->private;
    hVi = (HI_HANDLE)((HI_ULONG)pProcItem->data);

    GET_PORT_CHN(hVi, enPort, u32Chn);
    q = &g_ViDrv[enPort][u32Chn];
    pStatInfo = &g_ViDrv[enPort][u32Chn].stStat;

    PROC_PRINT(p, "-------------------------- VI Info --------------------------\n");

    PROC_PRINT(p,
               "Type                :%s\n",
               (HI_TRUE == q->stAttr.bVirtual) ? "Virtual" : "Real");

    if (HI_FALSE == q->stAttr.bVirtual)
    {
        PROC_PRINT(p,
                   "InputRect           :%d/%d/%d/%d\n"
                   "InputMode           :%s\n"
                   "VideoFormat         :%s\n"
                   "BufferMgmt          :%s\n",
                   q->stAttr.stInputRect.s32X, q->stAttr.stInputRect.s32Y,
                   q->stAttr.stInputRect.s32Width, q->stAttr.stInputRect.s32Height,
                   inputMode[q->stAttr.enInputMode], videoFmt[q->stAttr.enVideoFormat],
                   (HI_UNF_VI_BUF_ALLOC == q->stAttr.enBufMgmtMode) ? "VI" : "User");
    }
    else
    {
        PROC_PRINT(p,
                   "InputRect           :%d/%d/%d/%d\n",
                   0, 0, q->stFrame[0].u32Width, q->stFrame[0].u32Height);
    }

    PROC_PRINT(p,
               "BufferNum           :%d\n"
               "State               :%s\n",
               q->stAttr.u32BufNum,
               (HI_TRUE == q->bStarted) ? "Start" : "Stop");

    for (i = 0; i < VI_MAX_VPSS_PORT; i++)
    {
        if (HI_INVALID_HANDLE != q->stPortParam[i].hPort)
        {
            PROC_PRINT(p,
                       "%s               :%s(port%x)\n",
                       (0 == i) ? "DstID" : "     ",
                       ((q->stPortParam[i].hDst >> 16) & 0xFF) == HI_ID_VO ? "win" : "venc",
                       q->stPortParam[i].hPort);
        }
    }

    PROC_PRINT(p, "\n------------------------- Statistics -------------------------\n");

    PROC_PRINT(p,
               "CAM/USER->VI\n"
               "CapFrame(Try/OK/Freq):%d/%d/%d\n\n",
               (HI_TRUE == q->stAttr.bVirtual) ? q->stStat.QueueTry : q->stStat.Cnt,
               (HI_TRUE == q->stAttr.bVirtual) ? q->stStat.QueueOK : q->stStat.Cnt,
               q->u32FrameRate);
#if 1
    if (HI_FALSE == q->stAttr.bVirtual)
    {
        PROC_PRINT(p,
                   "Interrupt(Cnt/Unload):%d/%d\n"
                   "  Y/CBusError       :%d/%d\n"
                   "  UpdateReg         :%d\n"
                   "  FieldThrow        :%d\n"
                   "  BufOverflow       :%d\n"
                   "  FrmStart          :%d\n"
                   "  Cc/ZeroInt        :%d/%d\n"
                   "  Top/BtmField      :%d/%d\n\n",
                   pStatInfo->Cnt, pStatInfo->Unload,
                   pStatInfo->YBusError, pStatInfo->CBusError,
                   pStatInfo->UpdateReg, pStatInfo->FieldThrow,
                   pStatInfo->BufOverflow, pStatInfo->FrmStart,
                   pStatInfo->CcInt, pStatInfo->ZeroInt,
                   pStatInfo->TopField, pStatInfo->BtmField);
    }
#endif


    PROC_PRINT(p,
               "VI->VPSS\n"
               "Acquire(Try/OK)     :%d/%d\n"
               "Release(Try/OK)     :%d/%d\n\n",
               pStatInfo->AcquireTry, pStatInfo->AcquireOK,
               pStatInfo->ReleaseTry, pStatInfo->ReleaseOK);

    PROC_PRINT(p,
               "VPSS->VI\n"
               "Acquire(Try/OK)     :%d/%d\n"
               "Release(Try/OK)     :%d/%d\n\n",
               pStatInfo->GetTry, pStatInfo->GetOK,
               pStatInfo->PutTry, pStatInfo->PutOK);

    PROC_PRINT(p,
               "VI->WIN\n"
               "SendFrame(Try/OK)   :%d/%d\n\n",
               pStatInfo->QWinTry, pStatInfo->QWinOK);

    PROC_PRINT(p,
               "VI->VENC\n"
               "SendFrame(Try/OK)   :%d/%d\n\n",
               pStatInfo->QVencTry, pStatInfo->QVencOK);

    PROC_PRINT(p,
               "VI->USER\n"
               "Acquire(Try/OK)     :%d/%d\n"
               "Release(Try/OK)     :%d/%d\n\n",
               pStatInfo->UsrAcqTry, pStatInfo->UsrAcqOK,
               pStatInfo->UsrRlsTry, pStatInfo->UsrRlsOK);

    memset(&stBufProc, 0x0, sizeof(VI_FB_BUF_PROC));
    VI_DRV_BufProc(&g_ViDrv[enPort][u32Chn].stFrameBuf, &stBufProc);

    PROC_PRINT(p,
               "VIBuffer(Total/Used):%d/%d\n",
               g_ViDrv[enPort][u32Chn].stAttr.u32BufNum, stBufProc.u32UsedNum);

    PROC_PRINT(p, "VIBufferDetail      :[");
    for (i = 0; i < g_ViDrv[enPort][u32Chn].stAttr.u32BufNum - 1; i++)
    {
        PROC_PRINT(p, "%d,", (HI_U32)stBufProc.stState[i]);
    }

    PROC_PRINT(p, "%d]\n\n", (HI_U32)stBufProc.stState[i]);

    return HI_SUCCESS;
}

static HI_S32 VI_DRV_ProcWrite(struct file *file, const char __user *buf, size_t count,
                               loff_t *ppos)
{
    struct seq_file *q = file->private_data;
    DRV_PROC_ITEM_S *pProcItem = q->private;
    HI_CHAR  ProcPara[64]={0};
    HI_CHAR *pArg1, *pArg2;
    HI_HANDLE hVi;
    HI_UNF_VI_E enPort;
    HI_U32 u32Chn;
    HI_U32 iPort, iChn;
    HI_S32 Ret;

    hVi = (HI_HANDLE)((HI_ULONG)pProcItem->data);
    if (HI_INVALID_HANDLE == hVi)
    {
        return -EFAULT;
    }

    GET_PORT_CHN(hVi, enPort, u32Chn);

    if(count >= sizeof(ProcPara))
    {
        HI_ERR_VI("your echo parameter string is too long!\n");
        return -EFAULT;
    }

    if (count >= 1)
    {
        memset(ProcPara, 0, sizeof(ProcPara));
        if (copy_from_user(ProcPara, buf, count))
        {
            HI_ERR_VI("copy_from_user failed.\n");
            return -EFAULT;
        }

        ProcPara[count] = '\0';

        Ret = VI_DRV_ParseProcPara(ProcPara,&pArg1,&pArg2);
        if (HI_SUCCESS != Ret)
        {
            VI_DRV_ProcHelp(hVi);
            return -EFAULT;
        }

        if (HI_SUCCESS == HI_OSAL_Strncmp(pArg1, "save_yuv", strlen("save_yuv")))
        {
            Ret = VI_DRV_ProcSaveYuv(hVi);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_VI("VI_DRV_ProcSaveYuv failed\n");
            }
        }
        else if (HI_SUCCESS == HI_OSAL_Strncmp(pArg1, "low_delay_stat", strlen("low_delay_stat")))
        {
            if (pArg2 == HI_NULL)
            {
                VI_DRV_ProcHelp(hVi);
                return -EFAULT;
            }

            if (HI_SUCCESS == HI_OSAL_Strncmp(pArg2, "start", strlen("start")))
            {
                if (!g_ViDrv[enPort][u32Chn].bLowDelayStat)
                {
                    HI_INFO_VI("Start LowDelay Statistics, vi%04x\n", (HI_U32)(hVi & 0xffff));
                    g_ViDrv[enPort][u32Chn].bLowDelayStat = HI_TRUE;
                    HI_DRV_LD_Start_Statistics(SCENES_VID_CAP, hVi);
                    //switch vi  instance
                    for (iPort = 0; iPort < MAX_VI_PORT; iPort++)
                    {
                        for ( iChn = 0 ; iChn < MAX_VI_CHN ; iChn++ )
                        {
                            if ((g_ViDrv[iPort][iChn].bLowDelayStat) && ((enPort != iPort) || (u32Chn != iChn)))
                            {
                                g_ViDrv[iPort][iChn].bLowDelayStat = HI_FALSE;
                            }
                        }
                    }
                }
            }
            else if (HI_SUCCESS == HI_OSAL_Strncmp(pArg2, "stop", strlen("stop")))
            {
                if (g_ViDrv[enPort][u32Chn].bLowDelayStat)
                {
                    HI_INFO_VI("Stop LowDelay Statistics, vi%04x\n", (HI_U32)(hVi & 0xffff));
                    HI_DRV_LD_Stop_Statistics();
                    g_ViDrv[enPort][u32Chn].bLowDelayStat = HI_FALSE;
                }
            }
            else
            {
                VI_DRV_ProcHelp(hVi);
            }
        }
        else
        {
            VI_DRV_ProcHelp(hVi);
        }
    }
    else
    {
        VI_DRV_ProcHelp(hVi);
    }

    return count;
}

HI_S32 VI_DRV_ProcAdd(HI_HANDLE hVi)
{
    HI_S32 Ret;
    DRV_PROC_ITEM_S *pProcItem;
    HI_CHAR ProcName[12];
    HI_UNF_VI_E enPort = HI_UNF_VI_PORT0;
    HI_U32 u32Chn = 0;

    if (HI_INVALID_HANDLE == hVi)
    {
        return HI_FAILURE;
    }

    GET_PORT_CHN(hVi, enPort, u32Chn);

    Ret = HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "vi%04x", (HI_U32)(hVi & 0xffff));
    if (0 == Ret)
    {
        HI_ERR_VI("HI_OSAL_Snprintf failed!\n");
        return HI_FAILURE;
    }

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_ERR_VI("VI add proc failed!\n");
        return HI_FAILURE;
    }

    pProcItem->data  = (HI_VOID *)((HI_ULONG)hVi);
    pProcItem->read  = VI_DRV_ProcRead;
    pProcItem->write = VI_DRV_ProcWrite;

    init_timer(&g_ViDrv[enPort][u32Chn].viTimer);
    g_ViDrv[enPort][u32Chn].viTimer.expires  = jiffies + (HZ);
    g_ViDrv[enPort][u32Chn].viTimer.function = (void*)VI_DRV_ProcTimer;
    g_ViDrv[enPort][u32Chn].viTimer.data = (HI_LENGTH_T)(hVi);
    add_timer(&g_ViDrv[enPort][u32Chn].viTimer);

    return HI_SUCCESS;
}

HI_S32 VI_DRV_ProcDel(HI_HANDLE hVi)
{
    HI_S32 Ret;
    HI_CHAR ProcName[12];
    HI_UNF_VI_E enPort = HI_UNF_VI_PORT0;
    HI_U32 u32Chn = 0;

    if (HI_INVALID_HANDLE == hVi)
    {
        return HI_FAILURE;
    }

    GET_PORT_CHN(hVi, enPort, u32Chn);
    del_timer(&g_ViDrv[enPort][u32Chn].viTimer);

    Ret = HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "vi%04x", (HI_U32)(hVi & 0xffff));
    if (0 == Ret)
    {
        return HI_FAILURE;
    }

    HI_DRV_PROC_RemoveModule(ProcName);
    return HI_SUCCESS;
}

HI_S32 VI_DRV_ProcTimer(HI_LENGTH_T data)
{
    HI_HANDLE hVi = (HI_HANDLE)data;
    HI_UNF_VI_E enPort = HI_UNF_VI_PORT0;
    HI_U32 u32Chn = 0;
    VI_DRV_S *q = HI_NULL;

    GET_PORT_CHN(hVi, enPort, u32Chn);
    q = &g_ViDrv[enPort][u32Chn];
    if (HI_TRUE == q->stAttr.bVirtual)
    {
        q->u32FrameRate = q->stStat.QueueOK - q->stStat.QueueOKLast;
        q->stStat.QueueOKLast = q->stStat.QueueOK;
    }
    else
    {
        q->u32FrameRate   = q->stStat.Cnt - q->stStat.CntLast;
        q->stStat.CntLast = q->stStat.Cnt;
    }

    q->viTimer.expires  = jiffies + (HZ);
    q->viTimer.function = (void*)VI_DRV_ProcTimer;
    q->viTimer.data = (HI_LENGTH_T)(hVi);
    add_timer(&q->viTimer);

    return HI_SUCCESS;
}
