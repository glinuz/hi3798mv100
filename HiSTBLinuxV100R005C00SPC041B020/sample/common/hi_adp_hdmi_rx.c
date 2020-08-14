/******************************************************************************

  Copyright (C), 2001-2050, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/01/27
  Description   :
  History       :
  1.Date        : 2014/06/9
    Author      : l00214567
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/time.h>
#include <linux/videodev2.h>

#include "hi_unf_common.h"
#include "hi_unf_vi.h"
#ifdef HI_HDMI_RX_INSIDE
#include "hi_unf_hdmirx.h"
#else
#include "hi_drv_hdmirx.h"
#endif
#include "hi_adp.h"


#if 1
#define hdmirx_printf(fmt...)       printf(fmt)
#else
#define hdmirx_printf
#endif


#ifdef HI_HDMI_RX_INSIDE

HI_S32 HI_ADP_HDMIRX_Init(HI_UNF_HDMIRX_TIMING_INFO_S * pstTiming)
{
    HI_UNF_HDMIRX_EDID_S pstEdid = {{
            0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x21, 0x33, 0x33, 0x34, 0x00, 0x00, 0x00, 0x97,
            0x2F, 0x0E, 0x01, 0x03, 0x80, 0x3C, 0x22, 0x61, 0x2B, 0xD4, 0x6C, 0xA3, 0x57, 0x49, 0x9C, 0x25,
            0x11, 0x48, 0x4B, 0xBD, 0xEF, 0x80, 0x81, 0xC1, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
            0x45, 0x00, 0xA0, 0x5A, 0x00, 0x00, 0x00, 0x1E, 0x66, 0x21, 0x56, 0xAA, 0x51, 0x00, 0x1E, 0x30,
            0x46, 0x8F, 0x33, 0x00, 0xA0, 0x5A, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x53,
            0x41, 0x4D, 0x53, 0x55, 0x4E, 0x47, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
            0x00, 0x18, 0x4B, 0x0F, 0x51, 0x11, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x34,

            0x02, 0x03, 0x30, 0xF1, 0x4D, 0x90, 0x1F, 0x04, 0x13, 0x05, 0x14, 0x03, 0x12, 0x20, 0x21, 0x22,
            0x07, 0x16, 0x23, 0x09, 0x07, 0x07, 0x83, 0x01, 0x00, 0x00, 0x02, 0x00, 0x0F, 0x72, 0x03, 0x0C,
            0x00, 0x10, 0x00, 0x80, 0x1E, 0x21, 0xD0, 0x08, 0x01, 0x40, 0x07, 0x3F, 0x40, 0x50, 0x90, 0xA0,
            0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C, 0x45, 0x00, 0x10, 0x09, 0x00, 0x00,
            0x00, 0x1E, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28, 0x55, 0x00, 0x10, 0x09,
            0x00, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20, 0xB8, 0x28, 0x55, 0x40,
            0x10, 0x09, 0x00, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20, 0x58, 0x2C,
            0x25, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21,
        },{0x10,0x20,0x30,0x40}
    };
    HI_UNF_HDMIRX_HDCP_S pstHdcp= {{
            0x80, 0x00, 0x48, 0x49, 0xcf, 0x57, 0x5c, 0xc7, 0xfc, 0x7e, 0x07, 0x14, 0x58, 0x3d, 0x1e, 0xb6,
            0x02, 0xbe, 0x2e, 0xf5, 0xff, 0xe4, 0x2a, 0x30, 0x11, 0xcc, 0xe9, 0x0e, 0x33, 0xaa, 0x87, 0xa4,
            0x95, 0xc0, 0xf3, 0x9d, 0xce, 0xab, 0x10, 0x3a, 0x71, 0x7d, 0x2a, 0xa8, 0xf1, 0x13, 0x0a, 0xa4,
            0x6e, 0x5c, 0x6e, 0x38, 0xf2, 0xbb, 0x1e, 0xbe, 0xcd, 0x65, 0xca, 0x10, 0x7d, 0x0a, 0x8b, 0xdb,
            0x50, 0x49, 0x71, 0x38, 0xad, 0x7b, 0x7b, 0x19, 0x9a, 0xbc, 0x91, 0xdd, 0x0e, 0x11, 0xd5, 0x03,
            0x7b, 0x29, 0x09, 0x6d, 0x66, 0x79, 0x99, 0x8f, 0x2e, 0x62, 0xe2, 0x2f, 0x35, 0xcc, 0x84, 0x2d,
            0x1a, 0xcb, 0x42, 0x8f, 0x30, 0xc1, 0x59, 0x4d, 0x09, 0xb3, 0x53, 0x5c, 0xdd, 0xfd, 0xca, 0xf7,
            0x3a, 0x04, 0xeb, 0x5d, 0xbb, 0x73, 0x17, 0xee, 0xf8, 0x32, 0x90, 0xd9, 0x48, 0x25, 0x8a, 0x93,
            0x8e, 0x77, 0x28, 0xaf, 0x96, 0x80, 0x1f, 0x2e, 0x50, 0x87, 0x47, 0xaf, 0x11, 0x20, 0xb0, 0x9d,
            0x39, 0xea, 0x66, 0x83, 0x6a, 0xa7, 0x27, 0x09, 0xd8, 0x64, 0x20, 0x5c, 0xab, 0x10, 0x54, 0xbb,
            0x55, 0xb9, 0x4d, 0x9a, 0xac, 0x61, 0xb4, 0xc3, 0xcf, 0x1a, 0x12, 0xcd, 0xbe, 0x4a, 0x20, 0xa5,
            0x95, 0x31, 0x03, 0x06, 0x88, 0x54, 0x1b, 0x3d, 0xb9, 0x73, 0xb9, 0x22, 0x8c, 0x88, 0x6c, 0x36,
            0xec, 0xc3, 0xe2, 0x3e, 0xec, 0x03, 0xd4, 0xee, 0x6f, 0x08, 0x9b, 0x90, 0xee, 0xfc, 0xc6, 0xe8,
            0xd1, 0xca, 0x17, 0xa3, 0xc1, 0xf0, 0x39, 0x15, 0x94, 0xe9, 0xa9, 0x94, 0x34, 0x84, 0x76, 0xcd,
            0x72, 0xdd, 0x48, 0x34, 0xec, 0x87, 0xde, 0x84, 0xa5, 0xfd, 0x75, 0x47, 0x0b, 0xde, 0xef, 0x23,
            0x7d, 0x97, 0x88, 0x88, 0x76, 0x74, 0xb9, 0x65, 0xc7, 0x32, 0x08, 0x3b, 0xcc, 0x04, 0xee, 0x8c,
            0xa7, 0xfa, 0x6e, 0x35, 0x2d, 0xef, 0xb8, 0xfc, 0x11, 0x8e, 0x96, 0x29, 0x4c, 0x40, 0x18, 0xaa,
            0x76, 0x45, 0x27, 0x7d, 0xf4, 0xf2, 0x06, 0x55, 0xe4, 0x7c, 0x89, 0x2d, 0x29, 0x70, 0x7e, 0x00,
            0x40, 0xce, 0x40, 0x5f, 0x66, 0xf2, 0x7e, 0xfc, 0x53, 0x0e, 0x4d, 0x59, 0x59, 0x6e, 0x64, 0x55,
            0x2e, 0x33, 0x0c, 0x6c, 0x45, 0x14, 0xc7, 0xf3, 0x57, 0xc8, 0x29, 0x91, 0xc7, 0x75, 0xbb, 0x28,
            0xa3, 0x7e, 0x70, 0x08, 0xe0, 0x7f, 0x53, 0x0a, 0x7e, 0xe0, 0x73, 0xa4
        }
    };
    HI_S32 s32Ret;
    HI_UNF_SIG_STATUS_E enSignalStatus;
    HI_UNF_HDMIRX_Init();
#if 1
    s32Ret = HI_UNF_HDMIRX_UpdateEdid(&pstEdid);
    if(s32Ret != HI_SUCCESS)
    {
        hdmirx_printf("EDID Init Fail!\n");
    }
    sleep(1);
#endif
    s32Ret = HI_UNF_HDMIRX_LoadHdcp(&pstHdcp);
    if(s32Ret != HI_SUCCESS)
    {
        hdmirx_printf("HDCP Load Fail!\n");
    }
    HI_UNF_HDMIRX_Connect(0);
    while(1)
    {
        hdmirx_printf("\nPlease Prepare HDMI Signal\n");

        s32Ret = HI_UNF_HDMIRX_GetSignalStatus(&enSignalStatus);

        if (HI_UNF_SIG_SUPPORT == enSignalStatus)
        {
            hdmirx_printf("status: %d \n",(HI_U32)enSignalStatus);
            break;
        }
        sleep(1);
        hdmirx_printf(".");
    }

    s32Ret = HI_UNF_HDMIRX_GetTimingInfo(pstTiming);
    if(HI_SUCCESS != s32Ret)
    {
        hdmirx_printf("HI_UNF_HDMIRX_GetTimingInfo error: 0x%08x \n",s32Ret);
        return s32Ret;
    }
    hdmirx_printf("VI mode:%dx%d@%d[%c]\n"
           "enColorSpace:%d\n"
           "enPixelFmt:%d\n"
           "enBitWidth:%d\n"
           "enOverSampleMode:%d\n"
           "en3dFmt:%d\n"
           "3dVblank:%d\n"
           "bDviMode:%d\n"
           "TimingIndex:%d\n", \
            pstTiming->u32Width, \
            pstTiming->u32Height,\
            pstTiming->u32FrameRate,\
            pstTiming->bInterlace ? 'I' : 'P',\
            pstTiming->enColorSpace,\
            pstTiming->enPixelFmt,\
            pstTiming->enBitWidth,\
            pstTiming->enOversample,\
            pstTiming->en3dFmt,\
            pstTiming->u32Vblank,\
            pstTiming->bPcMode,\
            pstTiming->u32TimingIdx);

    return HI_SUCCESS ;
}

HI_VOID HI_ADP_HDMIRX_DeInit(void)
{
    HI_UNF_HDMIRX_DisConnect();
    HI_UNF_HDMIRX_DeInit();
}

#else


static HI_S32 Hdmirx_DevFd = -1;

HI_S32 HI_ADP_HDMIRX_Init(HI_HDMIRX_TIMING_INFO_S *pstTiming)
{
    HI_S32 Ret = HI_FAILURE;
    HI_SIG_STATUS_E stSigStatus;
    HI_HDMIRX_SET_GPIO_PIN_S stGpioPin;

    Hdmirx_DevFd = open("/dev/hi_sil9293", O_RDWR);
    if (Hdmirx_DevFd < 0)
    {
        hdmirx_printf("open hi_sil9293 failed\n");
        return HI_FAILURE;
    }

    /*
        Hardware I2C :  stGpioPin.u32I2cNo <= HI_STD_I2C_NUM
        Software I2C :  stGpioPin.u32I2cNo =  0xFF;
                        stGpioPin.u32GpioScl = 20; //gpio2_4
                        stGpioPin.u32GpioSda = 19; //gpio2_3
    */
    stGpioPin.u32I2cNo = 3; //Hw I2C_3

    stGpioPin.u32GpioRst = 81; //gpio10_1
    stGpioPin.u32GpioInt = 82; //gpio10_2

    stGpioPin.bSigLowCi2ca = 1; //CI2CA Signal
    Ret = ioctl(Hdmirx_DevFd, CMD_HDMIRX_SET_GPIO_PIN, &stGpioPin);
    if (HI_SUCCESS != Ret)
    {
        hdmirx_printf("ioctl error CMD_HDMIRX_CONNECT = 0x%zx\n", CMD_HDMIRX_GET_STATUS);
        return HI_FAILURE;
    }

    Ret = ioctl(Hdmirx_DevFd, CMD_HDMIRX_CONNECT);
    if (HI_SUCCESS != Ret)
    {
        hdmirx_printf("ioctl error CMD_HDMIRX_CONNECT = 0x%zx\n", CMD_HDMIRX_GET_STATUS);
        return HI_FAILURE;
    }

    hdmirx_printf("Wait for connecting HDMIRX");
    while (1)
    {
        Ret = ioctl(Hdmirx_DevFd, CMD_HDMIRX_GET_STATUS, &stSigStatus);
        if (HI_SUCCESS != Ret)
        {
            hdmirx_printf("ioctl error CMD_HDMIRX_GET_STATUS = 0x%zx\n", CMD_HDMIRX_GET_STATUS);
            return HI_FAILURE;
        }

        if (HI_SIG_SUPPORT == stSigStatus)
        {
            break;
        }

        sleep(1);
        hdmirx_printf(".");
    }

    hdmirx_printf("\nConnected!\n");

    Ret = ioctl(Hdmirx_DevFd, CMD_HDMIRX_GET_ATTR, pstTiming);
    if (HI_SUCCESS != Ret)
    {
        hdmirx_printf("ioctl CMD_HDMIRX_GET_ATTR error\n");
        return HI_FAILURE;
    }

    hdmirx_printf("VI mode:%dx%d@%d[%c]\n"
           "enColorSpace:%d\n"
           "enPixelFmt:%d\n"
           "enBitWidth:%d\n"
           "enOverSampleMode:%d\n"
           "en3dFmt:%d\n"
           "bDviMode:%d\n"
           "u32Vblank:%d\n", \
            pstTiming->u32Width, \
            pstTiming->u32Height,\
            pstTiming->u32FrameRate,\
            pstTiming->bInterlace ? 'I' : 'P',\
            pstTiming->enColorSpace,\
            pstTiming->enPixelFmt,\
            pstTiming->enBitWidth,\
            pstTiming->enOverSampleMode,\
            pstTiming->en3dFmt,\
            pstTiming->bDviMode,\
            pstTiming->u32Vblank );

    return HI_SUCCESS;
}


HI_VOID HI_ADP_HDMIRX_DeInit(void)
{
    ioctl(Hdmirx_DevFd, CMD_HDMIRX_DIS_CONNECT, NULL);
    close(Hdmirx_DevFd);
}

#endif

HI_S32 HI_ADP_HDMIRX_Init_GetViAttr(HI_UNF_VI_ATTR_S *pstViAttr)
{
    HI_S32 Ret;

#ifdef HI_HDMI_RX_INSIDE

    HI_UNF_HDMIRX_TIMING_INFO_S stTimingInfo;

    Ret = HI_ADP_HDMIRX_Init(&stTimingInfo);
    if (HI_SUCCESS != Ret)
    {
        hdmirx_printf("HI_ADP_HDMIRX_Init failed\n");
        return HI_FAILURE;
    }
    pstViAttr->bVirtual = HI_FALSE;
    pstViAttr->enInputMode =HI_UNF_VI_MODE_HDMIRX ;
    pstViAttr->stInputRect.s32X= 0;
    pstViAttr->stInputRect.s32Y= 0;
    pstViAttr->stInputRect.s32Width  = stTimingInfo.u32Width;
    pstViAttr->stInputRect.s32Height = stTimingInfo.u32Height;
    pstViAttr->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422;
    pstViAttr->enBufMgmtMode = HI_UNF_VI_BUF_ALLOC;
    pstViAttr->u32BufNum = 6;

    pstViAttr->bInterlace = stTimingInfo.bInterlace;
    pstViAttr->u32Width = stTimingInfo.u32Width;
    pstViAttr->u32Height = stTimingInfo.u32Height;
    pstViAttr->u32Vblank = stTimingInfo.u32Vblank;
    pstViAttr->u32FrameRate = stTimingInfo.u32FrameRate;
    pstViAttr->en3dFmt = stTimingInfo.en3dFmt;
    pstViAttr->enPixFmt = stTimingInfo.enPixelFmt;
    pstViAttr->enSrcBitWidth = stTimingInfo.enBitWidth;
    pstViAttr->enOverSample = stTimingInfo.enOversample;
    pstViAttr->enColorSpace = stTimingInfo.enColorSpace;
    pstViAttr->enDst3DT2DMode = HI_UNF_VI_3DT2D_OFF;
    pstViAttr->enDstBitWidth = HI_UNF_PIXEL_BITWIDTH_8BIT;

    return HI_SUCCESS;

#else
    HI_HDMIRX_TIMING_INFO_S stTiming;

    Ret = HI_ADP_HDMIRX_Init(&stTiming);
    if (HI_SUCCESS != Ret)
    {
        hdmirx_printf("HI_ADP_HDMIRX_Init failed\n");
        return HI_FAILURE;
    }

    if (((stTiming.u32Width == 1440)||(stTiming.u32Width == 720))\
            &&(stTiming.u32Height == 480)\
            &&(stTiming.bInterlace == HI_TRUE))
    {
        pstViAttr->enInputMode = HI_UNF_VI_MODE_BT656_480I;
        stTiming.u32Width  = 720;
    }
    else if (((stTiming.u32Width == 1440)||(stTiming.u32Width == 720))\
             &&(stTiming.u32Height == 576)\
             &&(stTiming.bInterlace == HI_TRUE))
    {
        pstViAttr->enInputMode = HI_UNF_VI_MODE_BT656_576I;
        stTiming.u32Width  = 720;
    }
    else if ((stTiming.u32Width == 640) && (stTiming.u32Height == 480))
    {
        pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_640X480P;
    }
    else if ((stTiming.u32Width == 720) && (stTiming.u32Height == 480))
    {
        pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_480P;
    }
    else if ((stTiming.u32Width == 720) && (stTiming.u32Height == 576))
    {
        pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_576P;
    }
    else if ((stTiming.u32Width == 1280) && (stTiming.u32Height == 720))
    {
        if (50 >= stTiming.u32FrameRate)
        {
            pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_720P_50;
        }
        else
        {
            pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_720P_60;
        }
    }
    else if ((stTiming.u32Width == 1920) && (stTiming.u32Height == 1080))
    {
        if (stTiming.bInterlace == HI_TRUE)
        {
            if (50 >= stTiming.u32FrameRate)
            {
                pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_1080I_50;
            }
            else
            {
                pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_1080I_60;
            }
        }
        else
        {
            if (25 >= stTiming.u32FrameRate)
            {
                pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_1080P_25;
            }
            else if (30 >= stTiming.u32FrameRate)
            {
                pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_1080P_30;
            }
            else if (50 >= stTiming.u32FrameRate)
            {
                pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_1080P_50;
            }
            else
            {
                pstViAttr->enInputMode = HI_UNF_VI_MODE_BT1120_1080P_60;
            }
        }
    }
    else
    {
        pstViAttr->enInputMode = HI_UNF_VI_MODE_HDMIRX ;
        //HI_ADP_HDMIRX_DeInit();
        //return HI_FAILURE;
    }

    pstViAttr->bVirtual = HI_FALSE;
    pstViAttr->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422;
    pstViAttr->stInputRect.s32Width  = stTiming.u32Width;
    pstViAttr->stInputRect.s32Height = stTiming.u32Height;
    pstViAttr->stInputRect.s32X= 0;
    pstViAttr->stInputRect.s32Y= 0;
    pstViAttr->enBufMgmtMode = HI_UNF_VI_BUF_ALLOC;
    pstViAttr->u32BufNum = 6;

    pstViAttr->bInterlace = stTiming.bInterlace;
    pstViAttr->u32Width = stTiming.u32Width;
    pstViAttr->u32Height = stTiming.u32Height;
    pstViAttr->u32FrameRate = stTiming.u32FrameRate;
    pstViAttr->en3dFmt = stTiming.en3dFmt;
    pstViAttr->enPixFmt = stTiming.enPixelFmt;
    pstViAttr->enSrcBitWidth = stTiming.enBitWidth;
    pstViAttr->enOverSample = stTiming.enOverSampleMode;
    pstViAttr->enColorSpace = stTiming.enColorSpace;
    pstViAttr->enDst3DT2DMode = HI_UNF_VI_3DT2D_OFF;
    pstViAttr->enDstBitWidth = HI_UNF_PIXEL_BITWIDTH_8BIT;

    return HI_SUCCESS;
#endif
}



