/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_sil9293.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/03/06
  Description   :
  History       :
  1.Date        : 2013/03/06
    Author      : t00202585,j00169368
    Modification: Created file

******************************************************************************/

#include <linux/delay.h>
#include <linux/kthread.h>

#include "hi_drv_proc.h"
#include "drv_sil9293.h"
#include "proc_sil9293.h"

HDMIRX_CONTEXT_S g_stHdmirxCtx = {0};

extern HI_U32 g_u32HdmiRxI2cNum;
extern HI_U32 gpioScl;
extern HI_U32 gpioSda;
extern HI_U32 gpioRst;
extern HI_U32 gpioInt;
extern bool bLowCi2ca;

HI_S32 SIL9293_STABLE_SearchTiming(HI_HDMIRX_TIMING_INFO_S *pstTiming)
{
    HDMIRX_CHECK_NULL_PTR(pstTiming);
    if ((pstTiming->u32Height > 1080) || (pstTiming->u32Width > 1920)
        || (pstTiming->u32Height == 0) || (pstTiming->u32Width == 0))
    {
        HI_ERR_HDMIRX("u32Height:%d, u32Width:%d\n", pstTiming->u32Height, pstTiming->u32Width);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
HI_S32 SIL9293_CHECK_Bt656(HI_HDMIRX_TIMING_INFO_S *pstTiming)
{

    if((1440 == pstTiming->u32Width)\
        &&((480 == pstTiming->u32Height)||(576 == pstTiming->u32Height))\
        &&(HI_TRUE == pstTiming->bInterlace))
    {
        SIL9293_HAL_SetBt656Enable(HI_TRUE);
        return HI_SUCCESS;
    }
    else if(pstTiming->u32Width < pstTiming->u32Height)
    {
        SIL9293_HAL_SetBt656Enable(HI_FALSE);
    }
    
    return HI_FAILURE;

}

HI_VOID SIL9293_STABLE_GetTiming(HI_HDMIRX_TIMING_INFO_S *pstTiming)
{
    if (pstTiming == HI_NULL)
    {
        return;
    }

    SIL9293_HAL_GetWH(&pstTiming->u32Width, &pstTiming->u32Height);
    SIL9293_HAL_GetFrameRate(&pstTiming->u32FrameRate);

    pstTiming->enPixelFmt = HI_PIXEL_FORMAT_YUV_422;
    pstTiming->enBitWidth = HI_PIXEL_BITWIDTH_8BIT;

    SIL9293_HAL_GetInterlace(&pstTiming->bInterlace);
    if (pstTiming->bInterlace)
    {
        pstTiming->u32Height = pstTiming->u32Height * 2;
    }

    if (pstTiming->u32Height >= 720)
    {
        pstTiming->enColorSpace = HI_COLOR_SPACE_YCBCR_709;
    }
    else
    {
        pstTiming->enColorSpace = HI_COLOR_SPACE_YCBCR_601;
    }
    SIL9293_HAL_GetOverSample(&pstTiming->enOverSampleMode);
    //SIL9293_HAL_GetColorSpace(&pstTiming->enColorSpace);

    SIL9293_HAL_GetICLK(&pstTiming->enOverSampleMode);

    pstTiming->en3dFmt   = HI_VIDEO_3D_FORMAT_2D;
    pstTiming->u32Vblank = 0;

    SIL9293_HAL_GetDviMode(&pstTiming->bDviMode);
}

HI_VOID SIL9293_STABLE_Working(HI_VOID)
{
    HI_S32 s32Ret;
    HI_HDMIRX_TIMING_INFO_S stTiming;
    static HI_S32 s32Number = 0;      

    SIL9293_STABLE_GetTiming(&stTiming);

    s32Ret = SIL9293_STABLE_SearchTiming(&stTiming);
    if (HI_SUCCESS == s32Ret)
    {
        if (0 == memcmp(&g_stHdmirxCtx.stTiming, &stTiming, sizeof(stTiming)))
        {
            s32Number = 0;
            g_stHdmirxCtx.enSigStatus = HI_SIG_SUPPORT;
        }
        else 
        {
            s32Number++;
            HI_ERR_HDMIRX("u32Height:%d, u32Width:%d,%d\n", stTiming.u32Height, stTiming.u32Width,s32Number);
        }
        
        if(3 == s32Number)
        {
            s32Number = 0;
            g_stHdmirxCtx.enSigStatus = HI_SIG_UNSTABLE;
            memcpy(&g_stHdmirxCtx.stTiming, &stTiming, sizeof(stTiming));

            /* re-connect */
            HI_ERR_HDMIRX("u32Height:%d, u32Width:%d\n", stTiming.u32Height, stTiming.u32Width);
            g_stHdmirxCtx.u32StableCnt = 0;
            SIL9293_HAL_ResetHdcp();
            SIL9293_HAL_Reset();
        }
    }
    else
    {
        HI_ERR_HDMIRX("u32Height:%d, u32Width:%d,%d\n", stTiming.u32Height, stTiming.u32Width,s32Number);
        s32Number++;
    }
    
    if(3 == s32Number)
    {
        HI_ERR_HDMIRX("u32Height:%d, u32Width:%d,%d\n", stTiming.u32Height, stTiming.u32Width,s32Number);
        s32Number = 0;
        g_stHdmirxCtx.enSigStatus = HI_SIG_NOT_SUPPORT;
        g_stHdmirxCtx.u32StableCnt = 0;
        SIL9293_HAL_ResetHdcp();
        SIL9293_HAL_Reset();

    }

    SIL9293_HAL_CalcSWFS(&g_stHdmirxCtx.u32AudioFs);
    SIL9293_HAL_UpdateFS(g_stHdmirxCtx.u32AudioFs);
}

HDMIRX_STABLE_E SIL9293_STABLE_GetState(HI_VOID)
{
    g_stHdmirxCtx.u32StableCnt++;

    if (g_stHdmirxCtx.u32StableCnt < HDMIRX_WILL_STABLE_CNT)
    {
        if (HI_HDCP_STATE_SUCCESS == g_stHdmirxCtx.enHdcpState)
        {
            g_stHdmirxCtx.u32StableCnt = HDMIRX_WILL_STABLE_CNT;
            return HDMIRX_STABLE_PREPROCESS;
        }

        return HDMIRX_STABLE_READY;
    }

    if (g_stHdmirxCtx.u32StableCnt == HDMIRX_WILL_STABLE_CNT)
    {
        g_stHdmirxCtx.enStabState = HDMIRX_STABLE_PREPROCESS;
        return HDMIRX_STABLE_PREPROCESS;
    }

    if ((g_stHdmirxCtx.u32StableCnt > HDMIRX_WILL_STABLE_CNT)
        && (g_stHdmirxCtx.u32StableCnt < HDMIRX_BEEN_STABLE_CNT))
    {
        g_stHdmirxCtx.enStabState = HDMIRX_STABLE_START;
        return HDMIRX_STABLE_START;
    }

    if (g_stHdmirxCtx.u32StableCnt >= HDMIRX_BEEN_STABLE_CNT)
    {
        g_stHdmirxCtx.enStabState = HDMIRX_STABLE_WORKING;
        return HDMIRX_STABLE_WORKING;
    }

    return HDMIRX_STABLE_READY;
}

HI_VOID SIL9293_STABLE_handle(HI_VOID)
{
    HDMIRX_STABLE_E enStabState;

    enStabState = SIL9293_STABLE_GetState();

    SIL9293_HAL_UpTmdsDcCtl();

    switch (enStabState)
    {
    case HDMIRX_STABLE_READY:
        break;
    case HDMIRX_STABLE_PREPROCESS:
        SIL9293_HAL_ClearInt();
        SIL9293_HAL_ResetAudioFIFO();
        SIL9293_HAL_ResetDcFIFO();

        //SIL9293_HAL_ResetAAC();
        break;
    case HDMIRX_STABLE_START:
        SIL9293_STABLE_GetTiming(&g_stHdmirxCtx.stTiming);

        SIL9293_CHECK_Bt656(&g_stHdmirxCtx.stTiming);
        break;
    case HDMIRX_STABLE_WORKING:
        SIL9293_STABLE_Working();
        msleep(100);
        break;
    default:
        HI_ASSERT(0);
        break;
    }
}

HI_VOID SIL9293_NOSTABLE_handle(HI_VOID)
{
    SIL9293_HAL_SetFSSel(HI_TRUE);
    SIL9293_HAL_UpTmdsDcCtl();

    g_stHdmirxCtx.enSigStatus  = HI_SIG_UNSTABLE;
    g_stHdmirxCtx.u32StableCnt = 0;
    g_stHdmirxCtx.u32AudioFs = 0;
}

HI_VOID SIL9293_WAITCONECT_Handle(HI_VOID)
{
    g_stHdmirxCtx.enSigStatus  = HI_SIG_NO_SIGNAL;
    g_stHdmirxCtx.u32StableCnt = 0;
    g_stHdmirxCtx.u32AudioFs = 0;
}

HI_VOID SIL9293_UpIPState(HI_VOID)
{
    HI_BOOL bHasClock;

    SIL9293_HAL_GetClock(&bHasClock);

    if (!bHasClock)
    {
        g_stHdmirxCtx.bHasClkEven = HI_FALSE;
        g_stHdmirxCtx.enIpState = HDMIRX_IP_STATE_WAITCONNET;
        return;
    }

    g_stHdmirxCtx.enIpState = HDMIRX_IP_STATE_STABLE;
}

HI_S32 SIL9293_Thread(HI_VOID *pData)
{
    while (g_stHdmirxCtx.bLoopThreadEn)
    {
        g_stHdmirxCtx.u32LoopCnt++;

        SIL9293_UpIPState();

        SIL9293_HAL_GetHdcpStatus(&g_stHdmirxCtx.enHdcpState);

        switch (g_stHdmirxCtx.enIpState)
        {
        case HDMIRX_IP_STATE_WAITCONNET:
            SIL9293_WAITCONECT_Handle();
            break;
        case HDMIRX_IP_STATE_NOSTABLE:
            SIL9293_NOSTABLE_handle();
            break;
        case HDMIRX_IP_STATE_STABLE:
            SIL9293_STABLE_handle();
            break;
        default:
            HI_ASSERT(0);
            break;
        }

        msleep(30);
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SIL9293_Connect(HI_VOID)
{
    HI_S32 Ret;

    Ret = SIL9293_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_HDMIRX("SIL9293_Init failed.\n");
        return HI_FAILURE;
    }

    memset(&g_stHdmirxCtx, 0, sizeof(g_stHdmirxCtx));

    g_stHdmirxCtx.enSigStatus   = HI_SIG_UNSTABLE;
    g_stHdmirxCtx.bLoopThreadEn = HI_TRUE;
    g_stHdmirxCtx.bHasClkEven   = HI_FALSE;
    g_stHdmirxCtx.pstLoopThread = kthread_run(SIL9293_Thread, NULL, "HI_SIL9293");
    if (IS_ERR(g_stHdmirxCtx.pstLoopThread))
    {
        HI_FATAL_HDMIRX("Unable to start hdmirx kernel thread.\n");
        g_stHdmirxCtx.pstLoopThread = HI_NULL;
        SIL9293_DeInit();
        return HI_FAILURE;
    }

    g_stHdmirxCtx.bOpened = HI_TRUE;

    return HI_SUCCESS;
}

HI_VOID HI_DRV_SIL9293_DisConnect(HI_VOID)
{
    g_stHdmirxCtx.bLoopThreadEn = HI_FALSE;
    if (g_stHdmirxCtx.pstLoopThread)
    {
        kthread_stop(g_stHdmirxCtx.pstLoopThread);
        g_stHdmirxCtx.pstLoopThread = NULL;
    }

    memset(&g_stHdmirxCtx, 0, sizeof(g_stHdmirxCtx));

    SIL9293_DeInit();
    g_stHdmirxCtx.bOpened = HI_FALSE;
}

HI_S32 HI_DRV_SIL9293_SetGpioPin(HI_HDMIRX_SET_GPIO_PIN_S *pstGpioPin)
{
    HDMIRX_CHECK_NULL_PTR(pstGpioPin);
    g_u32HdmiRxI2cNum   = pstGpioPin->u32I2cNo;
    gpioScl   = pstGpioPin->u32GpioScl;
    gpioSda   = pstGpioPin->u32GpioSda;
    gpioRst   = pstGpioPin->u32GpioRst;
    gpioInt   = pstGpioPin->u32GpioInt;
    bLowCi2ca = pstGpioPin->bSigLowCi2ca;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SIL9293_GetStatus(HI_SIG_STATUS_E *pstSigStatus)
{
    HDMIRX_CHECK_NULL_PTR(pstSigStatus);

    *pstSigStatus = g_stHdmirxCtx.enSigStatus;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SIL9293_GetTiming(HI_HDMIRX_TIMING_INFO_S *pstTiming)
{
    HDMIRX_CHECK_NULL_PTR(pstTiming);

    memcpy(pstTiming, &g_stHdmirxCtx.stTiming, sizeof(HI_HDMIRX_TIMING_INFO_S));

    if (g_stHdmirxCtx.enSigStatus != HI_SIG_SUPPORT)
    {
        HI_WARN_HDMIRX("HdmiRx work is unstable!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SIL9293_Init(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = SIL9293_HAL_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMIRX("ERROR: SIL9293_HAL_Init failed\n");
        return s32Ret;
    }

    SIL9293_HAL_SetPowerOn(HI_TRUE);
    SIL9293_HAL_SetVideoAuto();
    SIL9293_HAL_SetAudioAuto();
    SIL9293_HAL_SetDDCEn(HI_TRUE);
    SIL9293_HAL_SetHPG(HI_TRUE);
    SIL9293_HAL_SetTMDSEn(HI_TRUE);
    SIL9293_HAL_Reset();

    return HI_SUCCESS;
}

HI_VOID SIL9293_DeInit(HI_VOID)
{
    SIL9293_HAL_SetDDCEn(HI_FALSE);
    SIL9293_HAL_SetTMDSEn(HI_FALSE);
    SIL9293_HAL_SetHPG(HI_FALSE);

    msleep(500);

    SIL9293_HAL_SetPowerOn(HI_FALSE);
    SIL9293_HAL_DeInit();
}
#if (1 == HI_PROC_SUPPORT)
HI_S32 SIL9293_ProcShow(struct seq_file *p)
{
    HI_U32 u32Pixclk, u32HTot, u32VTot;
    HI_COLOR_SPACE_E enColorSpace;
    HI_PIXEL_FORMAT_E enPixelFmt;
    HI_PIXEL_BITWIDTH_E enBitWidth;
    HI_OVERSAMPLE_MODE_E enOverSampleMode;
    HI_HDMIRX_TIMING_INFO_S stTiming;
    HI_U8 i,u8Value;

    HI_DRV_SIL9293_GetTiming(&stTiming);
    SIL9293_HAL_GetOverSample(&enOverSampleMode);
    SIL9293_HAL_GetPixFmt(&enPixelFmt);
    SIL9293_HAL_GetColorSpace(&enColorSpace);
    SIL9293_HAL_GetColorDepth(&enBitWidth);
    SIL9293_HAL_GetPixClk10KHZ(&u32Pixclk);
    SIL9293_HAL_GetHVTotal(&u32HTot, &u32VTot);

    PROC_PRINT(p, "------ Connect Info ------\n");
    PROC_PRINT(p,
                    "IPStat       :%s\n"
                    "HdcpStat     :%s\n"
                    "StableCnt    :%d\n"
                    "AudioFs      :%d\n"
                    "Pixclk       :%d\n"
                    "HTot         :%d\n"
                    "VTot         :%d\n"
                    "SigStat      :%s\n",
                    EnIpStateToStr(g_stHdmirxCtx.enIpState),
                    EnHdcpStateToStr(g_stHdmirxCtx.enHdcpState),
                    g_stHdmirxCtx.u32StableCnt, g_stHdmirxCtx.u32AudioFs,
                    u32Pixclk, u32HTot, u32VTot,
                    EnSigStateToStr(g_stHdmirxCtx.enSigStatus));

    PROC_PRINT(p, "\n------ Timing Info 0 ------\n");
    PROC_PRINT(p,
                    "Width        :%d\n"
                    "Height       :%d\n"
                    "FrfRate      :%d\n"
                    "bInter       :%s\n"
                    "bDvi         :%s\n"
                    "3DFmt        :%s\n"
                    "vBlank       :%d\n",
                    stTiming.u32Width,
                    stTiming.u32Height,
                    stTiming.u32FrameRate,
                    BooleanToStr(stTiming.bInterlace),
                    BooleanToStr(stTiming.bDviMode),
                    En3dFmtToStr(stTiming.en3dFmt),
                    stTiming.u32Vblank);

    PROC_PRINT(p, "\n------ Timing Info 1(TO VI) ------\n");
    PROC_PRINT(p,
                    "ColourSpace  :%s\n"
                    "PixFormat    :%s\n"
                    "BitWidth     :%s\n"
                    "OverSample   :%s\n",
                    EnClrSpcToStr(stTiming.enColorSpace),
                    EnPixFmtToStr(stTiming.enPixelFmt),
                    EnPixBWToStr(stTiming.enBitWidth),
                    EnOverSmpMdToStr(stTiming.enOverSampleMode));

    PROC_PRINT(p, "\n------ Timing Info 1(From Tx) ------\n");
    PROC_PRINT(p,
                    "ColourSpace  :%s\n"
                    "PixFormat    :%s\n"
                    "BitWidth     :%s\n"
                    "OverSample   :%s\n\n",
                    EnClrSpcToStr(enColorSpace),
                    EnPixFmtToStr(enPixelFmt),
                    EnPixBWToStr(enBitWidth),
                    EnOverSmpMdToStr(enOverSampleMode));
    
    PROC_PRINT(p, "\n------ AVI InfoFrame ------\n");
    for(i = 0x40; i <= 0x52; i++)
    {
        SIL9293_Read(0x68, i, &u8Value);
        PROC_PRINT(p,"%02x,",u8Value);
    }
    PROC_PRINT(p,"\n");
    
    PROC_PRINT(p, "\n------ Audio InfoFrame ------\n");
    for(i = 0x80; i <= 0x8d; i++)
    {
        SIL9293_Read(0x68, i, &u8Value);
        PROC_PRINT(p,"%02x,",u8Value);
    }
    PROC_PRINT(p,"\n");

	/*
    //--NO MODIFY : COMMENT BY CODINGPARTNER--
    PROC_PRINT(p,"-----------0x64-----------\n");
    for(i = 0x0; i <= 0x6f; i++)
    {
        SIL9293_Read(0x64, i, &u8Value);
        PROC_PRINT(p,"%02x:%02x\n",i,u8Value);
    }
    PROC_PRINT(p,"-----------0x68-----------\n");
    for(i = 0x0; i <= 0x3f; i++)
    {
        SIL9293_Read(0x68, i, &u8Value);
        PROC_PRINT(p,"%02x:%02x\n",i,u8Value);
    }
    //SPD InfoFrame decodes value
    SIL9293_Write(0x68, 0x7f, 0x83);
    PROC_PRINT(p, "\n------ SPD InfoFrame ------\n");
    for(i = 0x60; i <= 0x7e; i++)
    {
        SIL9293_Read(0x68, i, &u8Value);
        PROC_PRINT(p,"%02x,",u8Value);
    }
    PROC_PRINT(p,"\n");
    
    //MPEG InfoFrame decodes value
    SIL9293_Write(0x68, 0xbf, 0x85);
    PROC_PRINT(p, "\n------ MPEG InfoFrame ------\n");
    for(i = 0xa0; i <= 0xbe; i++)
    {
        SIL9293_Read(0x68, i, &u8Value);
        PROC_PRINT(p,"%02x,",u8Value);
    }
    PROC_PRINT(p,"\n");
	*/
    /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    
return HI_SUCCESS;
}
#endif
