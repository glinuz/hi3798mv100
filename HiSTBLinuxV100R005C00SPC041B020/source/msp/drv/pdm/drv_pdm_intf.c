#include <linux/kernel.h>

#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_mem.h"
#include "hi_kernel_adapt.h"

#include "hi_drv_pdm.h"
#include "drv_pdm_ext.h"
#include "drv_pdm.h"
#include "hi_osal.h"

#define PDM_NAME "HI_PDM"

PDM_EXPORT_FUNC_S g_PdmExportFuncs =
{
    .pfnPDM_GetDispParam      = HI_DRV_PDM_GetDispParam,
    .pfnPDM_GetMceParam       = HI_DRV_PDM_GetMceParam,
    .pfnPDM_GetMceData        = HI_DRV_PDM_GetMceData,
    .pfnPDM_ReleaseReserveMem = HI_DRV_PDM_ReleaseReserveMem,
    .pfnPDM_GetData           = HI_DRV_PDM_GetData,
    .pfnGetSoundParam         = HI_DRV_PDM_GetSoundParam
};

PDM_GLOBAL_S g_PdmGlobal;

/*the function to get pdm tag data*/
extern int get_param_data(const char *name, char *buf, unsigned int buflen);

#if (1 == HI_PROC_SUPPORT)
static HI_VOID PDM_ProcDispTiming(struct seq_file *p, HI_UNF_DISP_TIMING_S *pstTiming)
{
    HI_CHAR *DataWith[] =
    {
        "WIDTH8",
        "WIDTH16",
        "WIDTH24"
    };

    PROC_PRINT(p, " ############ Custom Timing begin ##############\n");
    PROC_PRINT(p, "VFB:             %d   \n",pstTiming->VFB);
    PROC_PRINT(p, "VBB:             %d   \n",pstTiming->VBB);
    PROC_PRINT(p, "VACT:            %d   \n",pstTiming->VACT);
    PROC_PRINT(p, "HFB:             %d   \n",pstTiming->HFB);
    PROC_PRINT(p, "HBB:             %d   \n",pstTiming->HBB);
    PROC_PRINT(p, "HACT:            %d   \n",pstTiming->HACT);
    PROC_PRINT(p, "VPW:             %d   \n",pstTiming->VPW);
    PROC_PRINT(p, "HPW:             %d   \n",pstTiming->HPW);

    if (pstTiming->IDV)
    {
        PROC_PRINT(p, "IDV:             %s   \n","true");
    }
    else
    {
        PROC_PRINT(p, "IDV:             %s   \n","false");
    }

    if (pstTiming->IHS)
    {
        PROC_PRINT(p, "IHS:             %s   \n","true");
    }
    else
    {
        PROC_PRINT(p, "IHS:             %s   \n","false");
    }

    if (pstTiming->IVS)
    {
        PROC_PRINT(p, "IVS:             %s   \n","true");
    }
    else
    {
        PROC_PRINT(p, "IVS:             %s   \n","false");
    }

    if (pstTiming->ClockReversal)
    {
        PROC_PRINT(p, "ClockReversal:       %s   \n", "true");
    }
    else
    {
        PROC_PRINT(p, "ClockReversal:       %s   \n", "false");
    }

    if (pstTiming->DataWidth < HI_UNF_DISP_INTF_DATA_WIDTH_BUTT)
    {
        PROC_PRINT(p, "DataWidth:       %s   \n", DataWith[pstTiming->DataWidth]);
    }
    else
    {
        PROC_PRINT(p, "DataWidth:       %d   \n", pstTiming->DataWidth);
    }

    PROC_PRINT(p, "ItfFormat:       %d   \n", pstTiming->ItfFormat);

    if (pstTiming->DitherEnable)
    {
        PROC_PRINT(p, "DitherEnable:        %s   \n","true");
    }
    else
    {
        PROC_PRINT(p, "DitherEnable:        %s   \n","false");
    }

    PROC_PRINT(p, "ClkPara0:            0x%x   \n",pstTiming->ClkPara0);
    PROC_PRINT(p, "ClkPara1:            0x%x   \n",pstTiming->ClkPara1);

    if (pstTiming->bInterlace)
    {
        PROC_PRINT(p, "bInterlace:          %s   \n","true");
    }
    else
    {
        PROC_PRINT(p, "bInterlace:          %s   \n","false");
    }

    PROC_PRINT(p, "PixFreq:             %d   \n",pstTiming->PixFreq);
    PROC_PRINT(p, "VertFreq:            %d   \n",pstTiming->VertFreq);
    PROC_PRINT(p, "AspectRatioW:        %d   \n",pstTiming->AspectRatioW);
    PROC_PRINT(p, "AspectRatioH:        %d   \n",pstTiming->AspectRatioH);

    if (pstTiming->bUseGamma)
    {
        PROC_PRINT(p, "UseGamma:            %s   \n","true");
    }
    else
    {
        PROC_PRINT(p, "UseGamma:            %s   \n","false");
    }

    PROC_PRINT(p, "Reserve0:            %d   \n",pstTiming->Reserve0);
    PROC_PRINT(p, "Reserve1:            %d   \n",pstTiming->Reserve1);
    PROC_PRINT(p, " ############ Custom Timing end ##############\n");
}

static HI_S32 PDM_ProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_S32 Ret;
    HI_DISP_PARAM_S DispParam;
    HI_UNF_PDM_SOUND_PARAM_S *pSoundParam;
    HI_U32 i;
    HI_U32 j;
    HI_CHAR *Aspect[HI_UNF_DISP_ASPECT_RATIO_BUTT] =
    {
        "auto",
        "4to3",
        "16to9",
        "221to1",
        "user"
    };

    HI_CHAR *MClk[] =
    {
        "128*fs",
        "256*fs",
        "384*fs",
        "512*fs",
        "768*fs",
        "1024*fs"
    };

    for (i = HI_UNF_DISPLAY0; i < HI_UNF_DISPLAY2; i++)
    {
        memset(&DispParam, 0, sizeof(DispParam));

        Ret = DRV_PDM_GetDispParam(i, &DispParam);
        if (HI_SUCCESS != Ret)
        {
            continue;
        }

        PROC_PRINT(p, "---------------------------Dispaly%d------------------------------\n", i);

        if (DispParam.enFormat == HI_UNF_ENC_FMT_BUTT)
        {
            PROC_PRINT(p, "format:              %s   \n","Custom timing");
            PDM_ProcDispTiming(p, &(DispParam.stDispTiming));
        }
        else
        {
            PROC_PRINT(p, "format:              %d   \n",DispParam.enFormat);
        }

        PROC_PRINT(p, "source display:          %d\n", DispParam.enSrcDisp);
        PROC_PRINT(p, "background color:        0x%02x%02x%02x\n",
            DispParam.stBgColor.u8Red, DispParam.stBgColor.u8Green, DispParam.stBgColor.u8Blue);
        PROC_PRINT(p, "HuePlus/Brightness/Contrast/Saturation: %d/%d/%d/%d\n",
            DispParam.u32HuePlus, DispParam.u32Brightness, DispParam.u32Contrast, DispParam.u32Saturation);
        PROC_PRINT(p, "virtual screen(Width/Height):        %d/%d\n",
            DispParam.u32VirtScreenWidth, DispParam.u32VirtScreenHeight);
        PROC_PRINT(p, "offset(Left/Top/Right/Bottom):       %d/%d/%d/%d\n",
            DispParam.stOffsetInfo.u32Left, DispParam.stOffsetInfo.u32Top,
            DispParam.stOffsetInfo.u32Right, DispParam.stOffsetInfo.u32Bottom);
        PROC_PRINT(p, "bGammaEnable:        %d\n",DispParam.bGammaEnable);
        PROC_PRINT(p, "pixelformat:         %d\n",DispParam.enPixelFormat);
        if ((DispParam.stAspectRatio.enDispAspectRatio != HI_UNF_DISP_ASPECT_RATIO_USER)
            && (DispParam.stAspectRatio.enDispAspectRatio != HI_UNF_DISP_ASPECT_RATIO_BUTT))
        {
            PROC_PRINT(p, "aspectRatio:         %s\n",Aspect[DispParam.stAspectRatio.enDispAspectRatio]);
        }
        else if (DispParam.stAspectRatio.enDispAspectRatio == HI_UNF_DISP_ASPECT_RATIO_USER)
        {
            PROC_PRINT(p, "aspectRatio:         %s(%dto%d)\n",Aspect[DispParam.stAspectRatio.enDispAspectRatio],
                       DispParam.stAspectRatio.u32UserAspectWidth, DispParam.stAspectRatio.u32UserAspectHeight);
        }

        for (j = HI_UNF_DISP_INTF_TYPE_HDMI; j < HI_UNF_DISP_INTF_TYPE_BUTT; j++)
        {
            if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_HDMI)
            {
                PROC_PRINT(p, "HDMI:            HDMI_%d\n", DispParam.stIntf[j].unIntf.enHdmi);
            }
            else if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_LCD)
            {
                PROC_PRINT(p, "LCD:             LCD_%d\n", DispParam.stIntf[j].unIntf.enLcd);
            }
            else if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_BT1120)
            {
                PROC_PRINT(p, "BT1120:              BT1120_%d\n", DispParam.stIntf[j].unIntf.enHdmi);
            }
            else if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_BT656)
            {
                PROC_PRINT(p, "BT656:           BT656_%d\n", DispParam.stIntf[j].unIntf.enHdmi);
            }
            else if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_YPBPR)
            {
                PROC_PRINT(p, "YPbPr(Y/Pb/Pr):          %d/%d/%d\n", DispParam.stIntf[j].unIntf.stYPbPr.u8DacY,
                                DispParam.stIntf[j].unIntf.stYPbPr.u8DacPb, DispParam.stIntf[j].unIntf.stYPbPr.u8DacPr);
            }
            else if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_RGB)
            {
                PROC_PRINT(p, "RGB(R/G/B):          %d/%d/%d\n", DispParam.stIntf[j].unIntf.stRGB.u8DacR,
                           DispParam.stIntf[j].unIntf.stRGB.u8DacG, DispParam.stIntf[j].unIntf.stRGB.u8DacB);
            }
            else if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_CVBS)
            {
                PROC_PRINT(p, "CVBS:            %d\n", DispParam.stIntf[j].unIntf.stCVBS.u8Dac);
            }
            else if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_SVIDEO)
            {
                PROC_PRINT(p, "SVIDEO(Y/C):         %d/%d\n", DispParam.stIntf[j].unIntf.stSVideo.u8DacY,
                           DispParam.stIntf[j].unIntf.stSVideo.u8DacC);
            }
            else if (DispParam.stIntf[j].enIntfType == HI_UNF_DISP_INTF_TYPE_VGA)
            {
                PROC_PRINT(p, "VGA(R/G/B):          %d/%d/%d\n", DispParam.stIntf[j].unIntf.stVGA.u8DacR,
                           DispParam.stIntf[j].unIntf.stVGA.u8DacG, DispParam.stIntf[j].unIntf.stVGA.u8DacB);
            }
        }

        if (HI_UNF_DISPLAY1 == i)
        {
            PROC_PRINT(p, "HDMI VidOutMode:        %d\n", DispParam.enVidOutMode);
            PROC_PRINT(p, "HDMI DeepColorMode:     %d\n", DispParam.enDeepColorMode);
        }
    }

    pSoundParam = HI_KMALLOC(HI_ID_PDM, sizeof(HI_UNF_PDM_SOUND_PARAM_S), GFP_KERNEL);
    if (HI_NULL == pSoundParam)
    {
        return 0;
    }

    for (i = HI_UNF_SND_0; i < HI_UNF_SND_BUTT; i++)
    {
        HI_BOOL bVolumeExistInBase = HI_FALSE;

        memset(pSoundParam, 0, sizeof(HI_UNF_PDM_SOUND_PARAM_S));

        Ret = HI_DRV_PDM_GetSoundParamEx(i, pSoundParam, &bVolumeExistInBase);
        if (HI_SUCCESS != Ret)
        {
            continue;
        }

        if ((pSoundParam->u32PortNum <= sizeof(pSoundParam->stOutport) / sizeof(pSoundParam->stOutport[0])))
        {
            PROC_PRINT(p, "---------------------------Sound%d------------------------------\n", i);
            for (j = 0; j < pSoundParam->u32PortNum; j++)
            {
                if (pSoundParam->stOutport[j].enOutPort == HI_UNF_SND_OUTPUTPORT_DAC0)
                {
                    if (bVolumeExistInBase)
                    {
                        PROC_PRINT(p, "DAC:                 %s       VOL:%3d\n", "DAC0", pSoundParam->au32Volume[j]);
                    }
                    else
                    {
                        PROC_PRINT(p, "DAC:                 %s\n", "DAC0");
                    }
                }

                if (pSoundParam->stOutport[j].enOutPort == HI_UNF_SND_OUTPUTPORT_SPDIF0)
                {
                    if (bVolumeExistInBase)
                    {
                        PROC_PRINT(p, "SPDIF:               %s     VOL:%3d\n", "SPDIF0", pSoundParam->au32Volume[j]);
                    }
                    else
                    {
                        PROC_PRINT(p, "SPDIF:               %s\n", "SPDIF0");
                    }
                }

                if (pSoundParam->stOutport[j].enOutPort == HI_UNF_SND_OUTPUTPORT_HDMI0)
                {
                    if (bVolumeExistInBase)
                    {
                        PROC_PRINT(p, "HDMI:                %s      VOL:%3d\n", "HDMI0", pSoundParam->au32Volume[j]);
                    }
                    else
                    {
                        PROC_PRINT(p, "HDMI:                %s\n", "HDMI0");
                    }
                }

                if (pSoundParam->stOutport[j].enOutPort == HI_UNF_SND_OUTPUTPORT_ARC0)
                {
                    if (bVolumeExistInBase)
                    {
                        PROC_PRINT(p, "ARC:                 %s       VOL:%3d\n", "ARC0", pSoundParam->au32Volume[j]);
                    }
                    else
                    {
                        PROC_PRINT(p, "ARC:                 %s\n", "ARC0");
                    }
                }

                if (   (pSoundParam->stOutport[j].enOutPort == HI_UNF_SND_OUTPUTPORT_I2S0)
                    || (pSoundParam->stOutport[j].enOutPort == HI_UNF_SND_OUTPUTPORT_I2S1) )
                {
                    PROC_PRINT(p, "I2S%d attr:              \n",
                               pSoundParam->stOutport[j].enOutPort - HI_UNF_SND_OUTPUTPORT_I2S0);

                    if (bVolumeExistInBase)
                    {
                        PROC_PRINT(p, " VOL :                   %3d\n", pSoundParam->au32Volume[j]);
                    }

                    if (pSoundParam->stOutport[j].unAttr.stI2sAttr.stAttr.bMaster)
                    {
                        PROC_PRINT(p, " Master:             TRUE\n");
                    }
                    else
                    {
                        PROC_PRINT(p, " Master:             FALSE\n");
                    }

                    if (pSoundParam->stOutport[j].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge)
                    {
                        PROC_PRINT(p, " PcmSampleRiseEdge:      TRUE\n");
                    }
                    else
                    {
                        PROC_PRINT(p, " PcmSampleRiseEdge:      FALSE\n");
                    }

                    PROC_PRINT(p, " Master colock:          %s\n",
                               MClk[pSoundParam->stOutport[j].unAttr.stI2sAttr.stAttr.enMclkSel]);

                    PROC_PRINT(p, " Bit colock:         %d DIV\n",
                               pSoundParam->stOutport[j].unAttr.stI2sAttr.stAttr.enBclkSel);

                    PROC_PRINT(p, " Channel:            %d\n",
                               pSoundParam->stOutport[j].unAttr.stI2sAttr.stAttr.enChannel);

                    if (pSoundParam->stOutport[j].unAttr.stI2sAttr.stAttr.enI2sMode == HI_UNF_I2S_STD_MODE)
                    {
                        PROC_PRINT(p, " Mode:           standard\n");
                    }
                    else
                    {
                        PROC_PRINT(p, " Mode:           pcm\n");
                    }

                    PROC_PRINT(p, " Bit Depth:          %d\n",
                               pSoundParam->stOutport[j].unAttr.stI2sAttr.stAttr.enBitDepth);

                    PROC_PRINT(p, " PCM Delay Cycle:        %d\n",
                               pSoundParam->stOutport[j].unAttr.stI2sAttr.stAttr.enPcmDelayCycle);
                }
            }
        }
    }

    HI_KFREE(HI_ID_PDM, pSoundParam);

    return 0;
}
#endif

/*******************************************
tag format is version=1.0.0.0  fb=0x85000000,0x10000  baseparam=0x86000000,0x2000 бнбн
*******************************************/
HI_S32 PDM_GetTagBuf(HI_VOID)
{
    HI_CHAR PdmTag[512];
    HI_U32 PdmLen;
    HI_CHAR TmpBuf[32];
    HI_CHAR                 *p, *q;

    memset(PdmTag, 0x0, 512);

    PdmLen = get_param_data("pdm_tag", PdmTag, 512);
    if (PdmLen >= 512)
    {
        return HI_FAILURE;
    }

    PdmTag[511] = '\0';

    p = PdmTag;

    g_PdmGlobal.u32BufNum = 0;

    while (*p != '\0')
    {
        p = strstr(p, " ");
        if (0 == p)
        {
            return HI_SUCCESS;
        }

        q = strstr(p, "=");
        if (0 == q)
        {
            return HI_SUCCESS;
        }

        p++;

        memcpy(g_PdmGlobal.stBufInfo[g_PdmGlobal.u32BufNum].as8BufName, p, q - p);

        p = q + 1;
        q = strstr(p, ",");
        if (0 == q)
        {
            return HI_FAILURE;
        }

        memset(TmpBuf, 0x0, sizeof(TmpBuf));
        memcpy(TmpBuf, p, q - p);

        g_PdmGlobal.stBufInfo[g_PdmGlobal.u32BufNum].pPhyAddr = (HI_U8 *)simple_strtoul(TmpBuf, NULL, 16);

        p = q + 1;
        q = strstr(p, " ");
        if (0 == q)
        {
            q = PdmTag + PdmLen;
        }

        memset(TmpBuf, 0x0, sizeof(TmpBuf));
        memcpy(TmpBuf, p, q - p);

        g_PdmGlobal.stBufInfo[g_PdmGlobal.u32BufNum].u32Lenth = simple_strtoul(TmpBuf, NULL, 16);

        g_PdmGlobal.u32BufNum++;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_PDM_Init(HI_VOID)
{
    HI_S32 ret;

    ret = HI_DRV_MODULE_Register(HI_ID_PDM, PDM_NAME, (HI_VOID*)&g_PdmExportFuncs);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_PDM("ERR: HI_DRV_MODULE_Register!\n");
        return ret;
    }

    memset(&g_PdmGlobal, 0x0, sizeof(PDM_GLOBAL_S));

    HI_INIT_MUTEX(&g_PdmGlobal.PdmMutex);

    ret = PDM_GetTagBuf();
    if (HI_SUCCESS != ret)
    {
        memset(g_PdmGlobal.stBufInfo, 0x0, sizeof(PDM_BUF_INFO_S) * PDM_MAX_BUF_NUM);
    }

    return ret;
}

HI_S32 HI_DRV_PDM_DeInit(HI_VOID)
{
    HI_DRV_MODULE_UnRegister(HI_ID_PDM);

    return HI_SUCCESS;
}

HI_S32 PDM_DRV_ModInit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    HI_CHAR ProcName[16];
    DRV_PROC_ITEM_S *pProcItem = HI_NULL;
#endif

#ifndef HI_MCE_SUPPORT
    HI_DRV_PDM_Init();
#endif

#if (1 == HI_PROC_SUPPORT)
    HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s", HI_MOD_PDM);

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if (HI_NULL != pProcItem)
    {
        pProcItem->read  = PDM_ProcRead;
        pProcItem->write = HI_NULL;
    }
#endif
    return 0;
}

HI_VOID PDM_DRV_ModExit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    HI_CHAR ProcName[16];

    HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "%s", HI_MOD_PDM);
    HI_DRV_PROC_RemoveModule(ProcName);
#endif
#ifndef HI_MCE_SUPPORT
    HI_DRV_PDM_DeInit();
#endif

    return;
}

#ifdef MODULE
module_init(PDM_DRV_ModInit);
module_exit(PDM_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");
