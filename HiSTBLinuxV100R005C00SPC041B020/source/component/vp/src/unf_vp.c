/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : unf_vp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/03/19
  Description   :
  History       :
  1.Date        : 2011/03/19
    Author      :
    Modification: Created file
  2.Date        : 2012/11/27
    Author      : z00184432
    Modification: Reconstruct VP, add new interfaces, move to user from kernel mode
    
 3.Date        : 2013/12/20
   Author      : y00190777
   Modification: Reconstruct VP, add new interfaces
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <linux/videodev2.h>
#include <errno.h>
#include <netinet/in.h>

#include "hi_common.h"
#include "hi_error_mpi.h"
#include "hi_osal.h"
#include "hi_unf_vp.h"
#include "hi_unf_vi.h"
#include "hi_unf_avplay.h"
#include "hi_mpi_avplay.h"
#include "hi_unf_venc.h"
#include "hi_unf_vo.h"
#include "hi_mpi_vi.h"
#include "hi_mpi_mem.h"
#include "vp_priv.h"
#include "rtp_priv.h"
#include "rtp.h"
#include "hi_video_codec.h"
#include "hi_unf_pq.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

static VP_ATTR_S g_stVPAttr[MAX_VP_CHANNEL_NUM];
static HI_BOOL bVPInited = HI_FALSE;
static pthread_mutex_t g_VPMutex = PTHREAD_MUTEX_INITIALIZER;
#define HI_VP_LOCK() (void)pthread_mutex_lock(&g_VPMutex);
#define HI_VP_UNLOCK() (void)pthread_mutex_unlock(&g_VPMutex);

#ifdef VP_PROC_SUPPORT
static HI_PROC_ENTRY_S g_stVpProcEntry;
#endif

#define CHECK_VP_INIT \
    do {\
        HI_VP_LOCK(); \
        if (HI_FALSE == bVPInited)\
        { \
            HI_VP_UNLOCK(); \
            return HI_ERR_VP_NOT_INIT; \
        } \
        HI_VP_UNLOCK(); \
    } while (0)

#define CHECK_VP_HANDLE(hVP) \
    do {\
        HI_VP_LOCK(); \
        if (HI_NULL == hVP)\
        {\
            HI_VP_UNLOCK(); \
            return HI_ERR_VP_HANDLE_NOTEXIST; \
        } \
        if (HI_ID_VP != (HI_MOD_ID_E)((hVP >> 16) & 0xFFFF))\
        {\
            HI_VP_UNLOCK(); \
            return HI_ERR_VP_INVALID_HANDLE; \
        } \
        HI_VP_UNLOCK(); \
    } while (0)

#define CHECK_VP_NULL_PTR(ptr) \
    do {\
        if (NULL == (ptr))\
        {\
            HI_ERR_VP("PTR('%s') is NULL.\n", # ptr); \
            return HI_ERR_VP_PTR_NULL; \
        } \
    } while (0)

static HI_S32 VP_FindValidChannel(const HI_HANDLE hVP, HI_U32 *hId)
{
    HI_U32 i;

    for (i = 0; i < MAX_VP_CHANNEL_NUM; i++)
    {
        if (g_stVPAttr[i].hVP == hVP)
        {
            break;
        }
    }

    if (i == MAX_VP_CHANNEL_NUM)
    {
        return HI_ERR_VP_INVALID_HANDLE;
    }

    *hId = i;
    return HI_SUCCESS;
}

static HI_S32 VP_CheckAttr(const HI_UNF_VP_ATTR_S *pstAttr)
{
    CHECK_VP_NULL_PTR(pstAttr);

    if ((0 != pstAttr->bIsIpv4) && (1 != pstAttr->bIsIpv4))
    {
        HI_ERR_VP("wrong af inet, value %d!\n", pstAttr->bIsIpv4);
        return HI_ERR_VP_NOT_SUPPORT;
    }

    if (pstAttr->stVencAttr.enVencType != HI_UNF_VCODEC_TYPE_H264 &&
        pstAttr->stVencAttr.enVencType != HI_UNF_VCODEC_TYPE_HEVC)
    {
        HI_ERR_VP("Only support H.264!\n");
        return HI_ERR_VP_NOT_SUPPORT;
    }

    if (pstAttr->enDecType != HI_UNF_VCODEC_TYPE_H264 &&
        pstAttr->enDecType != HI_UNF_VCODEC_TYPE_HEVC)
    {
        HI_ERR_VP("Only support H.264!\n");
        return HI_ERR_VP_NOT_SUPPORT;
    }

    return HI_SUCCESS;
}

static HI_S32 VP_CheckSrcAttr(const HI_UNF_VP_SOURCE_ATTR_S *pstAttr)
{
    CHECK_VP_NULL_PTR(pstAttr);

    if (pstAttr->enSrcMode >= HI_UNF_VP_SRC_MODE_BUTT)
    {
        HI_ERR_VP("Invalid video source mode!\n");
        return HI_ERR_VP_NOT_SUPPORT;
    }

    if ((pstAttr->u32VIBufNum < MIN_VI_FB_NUM)
        || (pstAttr->u32VIBufNum > MAX_VI_FB_NUM))
    {
        HI_ERR_VP("Invalid vi buffer number(%d), should be %d-%d\n",
                  pstAttr->u32VIBufNum, MIN_VI_FB_NUM, MAX_VI_FB_NUM);
        return HI_ERR_VP_NOT_SUPPORT;
    }

    if (pstAttr->enSrcMode == HI_UNF_VP_SRC_MODE_USBCAM)
    {
        if ((0 == pstAttr->hSrc) || (HI_INVALID_HANDLE == pstAttr->hSrc))
        {
            HI_ERR_VP("Invalid handle in usbcam mode, 0x%08x\n", pstAttr->hSrc);
            return HI_ERR_VP_NOT_SUPPORT;
        }

    }

    if (pstAttr->enSrcMode == HI_UNF_VP_SRC_MODE_VI)
    {
        if (HI_ID_VI != ((pstAttr->hSrc >> 16) & 0xff))
        {
            HI_ERR_VP("Invalid vi handle!\n");
            return HI_ERR_VP_NOT_SUPPORT;
        }
    }

    return HI_SUCCESS;
}

static HI_VOID* VP_ScheduleCaptureYUV(HI_VOID* args)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_U32 u32Chn = (HI_U32)(HI_SIZE_T)args;
    enum   v4l2_buf_type type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    struct v4l2_buffer stUsbBuf;
    struct v4l2_format stUsbFmt;
    struct v4l2_streamparm stUsbStrm;
    HI_UNF_VIDEO_FRAME_INFO_S stFrame;
    HI_UNF_VCODEC_FRMRATE_S stFrameRate;
    HI_U32 u32Width, u32Height;

    memset(&stFrame, 0, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    memset(&stFrameRate, 0, sizeof(HI_UNF_VCODEC_FRMRATE_S));
    memset(&stUsbFmt, 0, sizeof(struct v4l2_format));
    memset(&stUsbStrm, 0, sizeof(struct v4l2_streamparm));

    /* release the unused buffer firstly */
    for (i = 0; i < g_stVPAttr[u32Chn].stSrcAttr.u32VIBufNum; i++)
    {
         memset(&stUsbBuf, 0, sizeof(struct v4l2_buffer));
         stUsbBuf.index = i;
         stUsbBuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
         stUsbBuf.memory = V4L2_MEMORY_MMAP;
         s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_QBUF, &stUsbBuf);
         if (s32Ret < 0)
         {
             HI_ERR_VP("Unable to queue buffer, ret = 0x%08x\n", s32Ret);
         }
     }
    

    /* begin to capture frame from camera */
    s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_STREAMON, &type);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to stream on, ret = 0x%08x\n", s32Ret);
        return HI_NULL;
    }

    stUsbFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_G_FMT, &stUsbFmt);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to get format, ret = 0x%08x\n", s32Ret);
        return HI_NULL;
    }

    u32Width  = stUsbFmt.fmt.pix.width;
    u32Height = stUsbFmt.fmt.pix.height;

    stUsbStrm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_G_PARM, &stUsbStrm);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to get frame rate, ret = 0x%08x\n", s32Ret);
        return HI_NULL;
    }

    if (stUsbStrm.parm.capture.timeperframe.numerator != 0)
    {
        stFrameRate.u32fpsInteger = stUsbStrm.parm.capture.timeperframe.denominator/stUsbStrm.parm.capture.timeperframe.numerator;
        stFrameRate.u32fpsDecimal = stUsbStrm.parm.capture.timeperframe.denominator%stUsbStrm.parm.capture.timeperframe.numerator;
    }

    while (!g_stVPAttr[u32Chn].bUSBCamStop)
    {
        stUsbBuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        stUsbBuf.memory = V4L2_MEMORY_MMAP;
        /* read frame from usb camera */
        s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_DQBUF, &stUsbBuf);
        if (s32Ret < 0)
        {
            usleep(10 * 1000);
            continue;
        }
        
        memcpy(g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[stUsbBuf.index].user_viraddr, \
               g_stVPAttr[u32Chn].stUsbcamPara.pMmapAddr[stUsbBuf.index], stUsbBuf.bytesused);

        
        /* reput frame into queue */
        s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_QBUF, &stUsbBuf);
        if (s32Ret < 0)
        {
            HI_ERR_VP("Unable to queue buffer, ret = 0x%08x\n", s32Ret);
        }
        
        stFrame.stVideoFrameAddr[0].u32YAddr   = g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[stUsbBuf.index].phyaddr;
        stFrame.stVideoFrameAddr[0].u32CAddr   = g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[stUsbBuf.index].phyaddr;
        stFrame.stVideoFrameAddr[0].u32YStride = u32Width * 2;
        stFrame.stVideoFrameAddr[0].u32CStride = u32Width * 2;
        stFrame.u32Width  = u32Width;
        stFrame.u32Height = u32Height;
        stFrame.u32AspectWidth  = u32Width;
        stFrame.u32AspectHeight = u32Height;
        stFrame.stFrameRate.u32fpsDecimal = stFrameRate.u32fpsDecimal;
        stFrame.stFrameRate.u32fpsInteger = stFrameRate.u32fpsInteger;
        stFrame.enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;
        stFrame.bProgressive = HI_TRUE;
        stFrame.enFieldMode = HI_UNF_VIDEO_FIELD_ALL;
        stFrame.bTopFieldFirst = HI_FALSE;
        stFrame.enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_NONE;
        stFrame.u32Circumrotate   = 0;
        stFrame.bVerticalMirror   = HI_FALSE;
        stFrame.bHorizontalMirror = HI_FALSE;
        stFrame.u32DisplayWidth   = u32Width;
        stFrame.u32DisplayHeight  = u32Height;
        stFrame.u32DisplayCenterX = u32Width / 2;
        stFrame.u32DisplayCenterY = u32Height / 2;
        stFrame.u32ErrorLevel = 0;

        s32Ret = HI_UNF_VI_QueueFrame(g_stVPAttr[u32Chn].hVI, &stFrame);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VI_QueueFrame failed, ret = 0x%08x\n", s32Ret);
            usleep(10 * 1000);
        }

        //Always try to Dequeue frame from vi
        s32Ret = HI_UNF_VI_DequeueFrame(g_stVPAttr[u32Chn].hVI, &stFrame);
        if (HI_SUCCESS == s32Ret)
        {
            HI_INFO_VP("VI Dqueue buffer, ret = 0x%08x\n", s32Ret);
        }
    }

    s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_STREAMOFF, &type);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to stream off, ret = 0x%08x\n", s32Ret);
    }

    return HI_NULL;
}

static HI_VOID* VP_ScheduleCaptureMJPEG(HI_VOID* args)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Chn = (HI_U32)(HI_SIZE_T)args;
    struct v4l2_buffer stUsbBuf;
    struct v4l2_format stUsbFmt;
    enum   v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    HI_UNF_STREAM_BUF_S stStreamBuf;
    HI_U32 i;
    HI_U32 u32PtsMs = HI_INVALID_PTS;

    memset(&stStreamBuf, 0, sizeof(HI_UNF_STREAM_BUF_S));
    memset(&stUsbFmt, 0, sizeof(struct v4l2_format));

    stUsbFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_G_FMT, &stUsbFmt);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to get format, ret = 0x%08x\n", s32Ret);
        return HI_NULL;
    }

    for (i = 0; i < g_stVPAttr[u32Chn].stSrcAttr.u32VIBufNum; i++)
    {
        memset(&stUsbBuf, 0, sizeof(struct v4l2_buffer));
        stUsbBuf.index = i;
        stUsbBuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        stUsbBuf.memory = V4L2_MEMORY_MMAP;
        s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_QBUF, &stUsbBuf);
        if (s32Ret < 0)
        {
            HI_ERR_VP("Unable to queue buffer, ret = 0x%08x\n", s32Ret);
        }
    }

    /* begin to capture frame from camera */
    s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_STREAMON, &type);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to stream on, ret = 0x%08x\n", s32Ret);
        return HI_NULL;
    }

    while (!g_stVPAttr[u32Chn].bUSBCamStop)
    {
        /* read frame from usb camera, get the index */
        stUsbBuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        stUsbBuf.memory = V4L2_MEMORY_MMAP;
        s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_DQBUF, &stUsbBuf);
        if (s32Ret < 0)
        {
            usleep(10 * 1000);
            continue;
        }

        s32Ret = HI_SYS_GetTimeStampMs(&u32PtsMs);
        if (HI_SUCCESS != s32Ret)
        {
            u32PtsMs = HI_INVALID_PTS;
        }

        s32Ret = HI_UNF_AVPLAY_GetBuf(g_stVPAttr[u32Chn].hLocalAVPLAY, HI_UNF_AVPLAY_BUF_ID_ES_VID, stUsbBuf.bytesused,
                                      &stStreamBuf, 0);
        if (HI_SUCCESS != s32Ret)
        {
            usleep(10 * 1000);
        }
        else
        {
            memcpy(stStreamBuf.pu8Data, g_stVPAttr[u32Chn].stUsbcamPara.pMmapAddr[stUsbBuf.index], stUsbBuf.bytesused);
            s32Ret = HI_UNF_AVPLAY_PutBuf(g_stVPAttr[u32Chn].hLocalAVPLAY, HI_UNF_AVPLAY_BUF_ID_ES_VID,
                                          stUsbBuf.bytesused, u32PtsMs);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VP("HI_UNF_AVPLAY_PutBuf failed, ret = 0x%08x\n", s32Ret);
            }
        }

        /* reput the frame just get, index equal */
        s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_QBUF, &stUsbBuf);
        if (s32Ret < 0)
        {
            HI_ERR_VP("Unable to queue buffer, ret = 0x%08x\n", s32Ret);
        }

    }

    s32Ret = ioctl(g_stVPAttr[u32Chn].stSrcAttr.hSrc, VIDIOC_STREAMOFF, &type);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to stream off, ret = 0x%08x\n", s32Ret);
    }

    return HI_NULL;
}

static HI_VOID* VP_ScheduleRtpSend(HI_VOID* args)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_VENC_STREAM_S stStream;
    HI_U32 i = 0;
    HI_U32 j = 0, k = 0;
    HI_U32 iFrameFreq  = 15;
    HI_U32 iFrameTotal = 6;
    HI_U32 u32Chn = (HI_U32)(HI_SIZE_T)args;
    HI_U8 *pu8Stream = HI_NULL;
    HI_U32 u32SysPtsMs = HI_INVALID_PTS;
    HI_U32 u32SlcLen = 0;
    HI_U8 ZeroDelay[] =
    {
        0x00, 0x00, 0x01, 0x1E,
        0x48, 0x53, 0x50, 0x49,
        0x43, 0x45, 0x4E, 0x44,
        0x00, 0x00, 0x01, 0x00
    };

    pu8Stream =  g_stVPAttr[u32Chn].pu8VencStream + NAL_AND_FU_PACKET_LEN;
    memset(&stStream, 0, sizeof(HI_UNF_VENC_STREAM_S));

    while (!g_stVPAttr[u32Chn].bNetSendStop)
    {
        if (HI_FALSE == g_stVPAttr[u32Chn].bSendEmptyPackage)
        {
            s32Ret = HI_UNF_VENC_AcquireStream(g_stVPAttr[u32Chn].hVENC, &stStream, 0);
            if (HI_SUCCESS != s32Ret)
            {
                usleep(10 * 1000);
                continue;
            }

            j++;
            memcpy((HI_VOID *)(pu8Stream), stStream.pu8Addr, stStream.u32SlcLen);
            u32SlcLen =  stStream.u32SlcLen;
            if(HI_TRUE == stStream.bFrameEnd)
            {
                memcpy((HI_VOID *)(&pu8Stream[stStream.u32SlcLen]), ZeroDelay, sizeof(ZeroDelay));
                u32SlcLen += sizeof(ZeroDelay);
            }
            s32Ret = RTP_Send(g_stVPAttr[u32Chn].hRTP, pu8Stream, u32SlcLen, stStream.u32PtsMs);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("RTP_Send failed, ret = 0x%08x\n", s32Ret);
            }

            s32Ret = HI_UNF_VENC_ReleaseStream(g_stVPAttr[u32Chn].hVENC, &stStream);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("HI_UNF_VENC_ReleaseStream failed, ret = 0x%08x\n", s32Ret);
                continue;
            }
            
            if ((j % iFrameFreq) == 0)
            {
                if (k < iFrameTotal)
                {
                    s32Ret = HI_UNF_VENC_RequestIFrame(g_stVPAttr[u32Chn].hVENC);
                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_ERR_VP("HI_UNF_VENC_RequestIFrame failed, ret = 0x%08x\n", s32Ret);
                        continue;
                    }
            
                    k++;
                }
            }
        }
        else
        {
            i = 0;
            memcpy((HI_VOID *)(pu8Stream), ZeroDelay, sizeof(ZeroDelay));
            (HI_VOID)HI_SYS_GetTimeStampMs(&u32SysPtsMs);
            s32Ret = RTP_Send(g_stVPAttr[u32Chn].hRTP, pu8Stream, sizeof(ZeroDelay), u32SysPtsMs);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("RTP_Send failed, ret = 0x%08x\n", s32Ret);
            }

            /* enhance the switching speed when receive is stopped */
            while (g_stVPAttr[u32Chn].bSendEmptyPackage && (i < NET_SEND_EMPTY_PACKAGE_INTEVAL / 10))
            {
                 usleep(10 * 1000);
                 i++;
            }
        }
    }

    return HI_NULL;
}

static HI_VOID* VP_ScheduleRtpRecv(HI_VOID* args)
{
    HI_S32 s32Ret = HI_FAILURE;
    size_t u32BufAddr;
    HI_U32 u32Len;
    HI_U32 u32PtsMs;
    HI_U32 u32Chn = (HI_U32)(HI_SIZE_T)args;
    HI_UNF_STREAM_BUF_S stAvplayBuf = {0};

    while (!g_stVPAttr[u32Chn].bNetRecvStop)
    {
        s32Ret = RTP_AcquireES(g_stVPAttr[u32Chn].hRTP, &u32BufAddr, &u32Len, &u32PtsMs);
        if (HI_SUCCESS != s32Ret)
        {
            usleep(10 * 1000);
            continue;
        }

        s32Ret = HI_UNF_AVPLAY_GetBuf(g_stVPAttr[u32Chn].hAVPLAY, HI_UNF_AVPLAY_BUF_ID_ES_VID, u32Len, &stAvplayBuf, 0);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_GetBuf failed, ret = 0x%08x\n", s32Ret);
            RTP_ReleaseES(g_stVPAttr[u32Chn].hRTP);
            continue;
        }

        if ((HI_NULL != stAvplayBuf.pu8Data))
        {
            memcpy((HI_VOID*)stAvplayBuf.pu8Data, (HI_VOID*)u32BufAddr, u32Len);
        }

        s32Ret = HI_UNF_AVPLAY_PutBuf(g_stVPAttr[u32Chn].hAVPLAY, HI_UNF_AVPLAY_BUF_ID_ES_VID, u32Len, u32PtsMs);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_PutBuf failed, ret = 0x%08x\n", s32Ret);
        }

        s32Ret = RTP_ReleaseES(g_stVPAttr[u32Chn].hRTP);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("RTP_ReleaseES failed, ret = 0x%08x\n", s32Ret);
        }
    }

    return HI_NULL;
}

static HI_S32 VP_AttachUsbCamYUV(HI_U32 u32Chn, HI_UNF_VP_SOURCE_ATTR_S *pstSrcAttr)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_VI_ATTR_S stViAttr;
    struct v4l2_format stUsbFmt;
    struct v4l2_buffer stUsbBuf;
    struct v4l2_requestbuffers stUsbRb;
    HI_U32 u32Height, u32Width;
    HI_U32 i;

    CHECK_VP_NULL_PTR(pstSrcAttr);

    memset(&stViAttr, 0, sizeof(HI_UNF_VI_ATTR_S));
    memset(&stUsbFmt, 0, sizeof(struct v4l2_format));
    memset(&stUsbBuf, 0, sizeof(struct v4l2_buffer));
    memset(&stUsbRb, 0, sizeof(struct v4l2_requestbuffers));

    stUsbFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s32Ret = ioctl(pstSrcAttr->hSrc, VIDIOC_G_FMT, &stUsbFmt);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to get format, ret = 0x%08x\n", s32Ret);
        return HI_FAILURE;
    }

    u32Height = stUsbFmt.fmt.pix.height;
    u32Width = stUsbFmt.fmt.pix.width;

    stViAttr.bVirtual  = HI_TRUE;
    stViAttr.u32BufNum = pstSrcAttr->u32VIBufNum;

    /* create a virtual VI to manage frame buffer, bind one channel to VO, the other channel to VENC */
    s32Ret = HI_UNF_VI_Create(HI_UNF_VI_PORT0, &stViAttr, &g_stVPAttr[u32Chn].hVI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VI_Create failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    stUsbRb.count = pstSrcAttr->u32VIBufNum;
    stUsbRb.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    stUsbRb.memory = V4L2_MEMORY_MMAP;

    s32Ret = ioctl(pstSrcAttr->hSrc, VIDIOC_REQBUFS, &stUsbRb);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to request buffers, ret = 0x%08x\n", s32Ret);
        s32Ret = HI_UNF_VI_Destroy(g_stVPAttr[u32Chn].hVI);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VI_Destroy failed, ret = 0x%08x\n", s32Ret);
        }

        g_stVPAttr[u32Chn].hVI = HI_INVALID_HANDLE;
        return HI_FAILURE;
    }

    for (i = 0; i < pstSrcAttr->u32VIBufNum; i++)
    {
        stUsbBuf.index = i;
        stUsbBuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        stUsbBuf.memory = V4L2_MEMORY_MMAP;
        s32Ret = ioctl(pstSrcAttr->hSrc, VIDIOC_QUERYBUF, &stUsbBuf);
        if (s32Ret < 0)
        {
            HI_ERR_VP("Unable to query buffer, i = %d, u32BufNum = %d\n", i, pstSrcAttr->u32VIBufNum);
            goto ERR0;
        }

        g_stVPAttr[u32Chn].stUsbcamPara.u32MmapLength[i] = stUsbBuf.length;
        g_stVPAttr[u32Chn].stUsbcamPara.pMmapAddr[i] = mmap(HI_NULL, stUsbBuf.length, PROT_READ | PROT_WRITE,
                                                            MAP_SHARED, pstSrcAttr->hSrc, (long)stUsbBuf.m.offset);
        if (g_stVPAttr[u32Chn].stUsbcamPara.pMmapAddr[i] == MAP_FAILED)
        {
            HI_ERR_VP("Unable to map buffer, ret = 0x%08x\n", errno);
            goto ERR0;
        }
    
        HI_OSAL_Snprintf(g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].bufname, 
                            sizeof(g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].bufname), "VpCamYuv_%d", i);

        g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].bufsize = u32Height * u32Width * 2;
        s32Ret = HI_MMZ_Malloc(&g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i]);
        if (s32Ret != HI_SUCCESS)
        {
            g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].bufsize = 0;
            goto ERR0;
        }
    }

    return HI_SUCCESS;

ERR0:
    for (i = 0; i < pstSrcAttr->u32VIBufNum; i++)
    {
        if (g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].bufsize)
        {
            HI_MMZ_Free(&g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i]);
            g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].user_viraddr = HI_NULL;
            g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].phyaddr = HI_NULL;
            g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].bufsize = 0;
        }
    }

    s32Ret = HI_UNF_VI_Destroy(g_stVPAttr[u32Chn].hVI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VI_Destroy failed, ret = 0x%08x\n", s32Ret);
    }
    
    g_stVPAttr[u32Chn].hVI = HI_INVALID_HANDLE;

    HI_ERR_VP("HI_MMZ_Malloc failed\n");
    return HI_ERR_VP_NOT_READY;
   
}

static HI_S32 VP_DetachUsbCamYUV(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    if (HI_INVALID_HANDLE != g_stVPAttr[u32Chn].hVI)
    {
        s32Ret = HI_UNF_VI_Destroy(g_stVPAttr[u32Chn].hVI);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VI_Destroy failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[u32Chn].hVI = HI_INVALID_HANDLE;
    }

    for (i = 0; i < g_stVPAttr[u32Chn].stSrcAttr.u32VIBufNum; i++)
    {
        if (HI_SUCCESS != munmap(g_stVPAttr[u32Chn].stUsbcamPara.pMmapAddr[i],
                         g_stVPAttr[u32Chn].stUsbcamPara.u32MmapLength[i]))
        {
            HI_ERR_VP("VP munmap failed\n");
        }

        if (g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].bufsize)
        {
            HI_MMZ_Free(&g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i]);
            g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].user_viraddr = HI_NULL;
            g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].phyaddr = HI_NULL;
            g_stVPAttr[u32Chn].stUsbcamPara.stMmzFrm[i].bufsize = 0;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VP_AttachUsbCamMJPEG(HI_U32 u32Chn, HI_UNF_VP_SOURCE_ATTR_S *pstSrcAttr)
{
    HI_S32 s32Ret = HI_FAILURE;
    struct v4l2_format stUsbFmt;
    struct v4l2_buffer stUsbBuf;
    struct v4l2_requestbuffers stUsbRb;
    HI_UNF_WINDOW_ATTR_S stVirWinAttr;
    HI_UNF_AVPLAY_ATTR_S stAvplayAttr;
    HI_UNF_AVPLAY_OPEN_OPT_S stAvOpt;
    HI_UNF_VCODEC_ATTR_S stVdecAttr;
    HI_U32 i;

    memset(&stUsbFmt, 0, sizeof(struct v4l2_format));
    memset(&stUsbBuf, 0, sizeof(struct v4l2_buffer));
    memset(&stUsbRb, 0, sizeof(struct v4l2_requestbuffers));
    memset(&stVirWinAttr, 0, sizeof(HI_UNF_WINDOW_ATTR_S));
    memset(&stAvplayAttr, 0, sizeof(HI_UNF_AVPLAY_ATTR_S));
    memset(&stVdecAttr, 0, sizeof(HI_UNF_VCODEC_ATTR_S));

    stVirWinAttr.enDisp   = HI_UNF_DISPLAY1;
    stVirWinAttr.bVirtual = HI_TRUE;
    stVirWinAttr.stWinAspectAttr.enAspectCvrs = HI_UNF_VO_ASPECT_CVRS_IGNORE;
    stVirWinAttr.stWinAspectAttr.bUserDefAspectRatio = HI_FALSE;
    stVirWinAttr.stWinAspectAttr.u32UserAspectWidth  = 0;
    stVirWinAttr.stWinAspectAttr.u32UserAspectHeight = 0;
    stVirWinAttr.bUseCropRect = HI_FALSE;
    memset(&stVirWinAttr.stInputRect, 0x0, sizeof(HI_RECT_S));
    memset(&stVirWinAttr.stOutputRect, 0x0, sizeof(HI_RECT_S));

    s32Ret = HI_UNF_VO_CreateWindow(&stVirWinAttr, &g_stVPAttr[u32Chn].hVirWin);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VO_CreateWindow failed, ret = 0x%08x\n", s32Ret);
        goto ERR0;
    }

    s32Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&stAvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
    s32Ret |= HI_UNF_AVPLAY_Create(&stAvplayAttr, &g_stVPAttr[u32Chn].hLocalAVPLAY);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_Create failed, ret = 0x%08x\n", s32Ret);
        goto ERR1;
    }

    stAvOpt.enCapLevel = g_stVPAttr[u32Chn].stVencAttr.enCapLevel;
    s32Ret = HI_UNF_AVPLAY_ChnOpen(g_stVPAttr[u32Chn].hLocalAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stAvOpt);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_ChnOpen failed, ret = 0x%08x\n", s32Ret);
        goto ERR2;
    }


    s32Ret = HI_UNF_AVPLAY_GetAttr(g_stVPAttr[u32Chn].hLocalAVPLAY, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVdecAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_GetAttr failed, ret = 0x%08x\n", s32Ret);
        goto ERR3;
    }

    stVdecAttr.enType = HI_UNF_VCODEC_TYPE_MJPEG;
    stVdecAttr.enMode = HI_UNF_VCODEC_MODE_NORMAL;
    stVdecAttr.u32ErrCover = 100;
    stVdecAttr.u32Priority = 3;

    s32Ret = HI_UNF_AVPLAY_SetAttr(g_stVPAttr[u32Chn].hLocalAVPLAY, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVdecAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_SetAttr failed, ret = 0x%08x\n", s32Ret);
        goto ERR3;
    }

    stUsbFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s32Ret = ioctl(pstSrcAttr->hSrc, VIDIOC_G_FMT, &stUsbFmt);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to get format, ret = 0x%08x\n", s32Ret);
        goto ERR3;
    }

    stUsbRb.count = pstSrcAttr->u32VIBufNum;
    stUsbRb.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    stUsbRb.memory = V4L2_MEMORY_MMAP;

    /* request usbcam buffer, get virtual address of each frame */
    s32Ret = ioctl(pstSrcAttr->hSrc, VIDIOC_REQBUFS, &stUsbRb);
    if (s32Ret < 0)
    {
        HI_ERR_VP("Unable to request buffers, ret = 0x%08x\n", s32Ret);
        goto ERR3;
    }

    for (i = 0; i < pstSrcAttr->u32VIBufNum; i++)
    {
        stUsbBuf.index = i;
        stUsbBuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        stUsbBuf.memory = V4L2_MEMORY_MMAP;
        s32Ret = ioctl(pstSrcAttr->hSrc, VIDIOC_QUERYBUF, &stUsbBuf);
        if (s32Ret < 0)
        {
            HI_ERR_VP("Unable to query buffer, i = %d, u32BufNum = %d\n", i, pstSrcAttr->u32VIBufNum);
            goto ERR3;
        }

        g_stVPAttr[u32Chn].stUsbcamPara.u32MmapLength[i] = stUsbBuf.length;
        g_stVPAttr[u32Chn].stUsbcamPara.pMmapAddr[i] = mmap(HI_NULL, stUsbBuf.length, PROT_READ | PROT_WRITE,
                                                            MAP_SHARED, pstSrcAttr->hSrc, (long)stUsbBuf.m.offset);
        if (g_stVPAttr[u32Chn].stUsbcamPara.pMmapAddr[i] == MAP_FAILED)
        {
            HI_ERR_VP("Unable to map buffer, ret = 0x%08x\n", errno);
            goto ERR3;
        }
    }

    return HI_SUCCESS;

ERR3:
    s32Ret = HI_UNF_AVPLAY_ChnClose(g_stVPAttr[u32Chn].hLocalAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
ERR2:
    s32Ret = HI_UNF_AVPLAY_Destroy(g_stVPAttr[u32Chn].hLocalAVPLAY);
    g_stVPAttr[u32Chn].hLocalAVPLAY = HI_INVALID_HANDLE;
ERR1:
    s32Ret = HI_UNF_VO_DestroyWindow(g_stVPAttr[u32Chn].hVirWin);
    g_stVPAttr[u32Chn].hVirWin = HI_INVALID_HANDLE;
ERR0:
    return HI_FAILURE;
}

static HI_S32 VP_DetachUsbCamMJPEG(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    s32Ret = HI_UNF_AVPLAY_ChnClose(g_stVPAttr[u32Chn].hLocalAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_ChnClose failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_VO_DestroyWindow(g_stVPAttr[u32Chn].hVirWin);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VO_DestroyWindow failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_AVPLAY_Destroy(g_stVPAttr[u32Chn].hLocalAVPLAY);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_Destroy failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    g_stVPAttr[u32Chn].hVirWin = HI_INVALID_HANDLE;
    g_stVPAttr[u32Chn].hLocalAVPLAY = HI_INVALID_HANDLE;

    for (i = 0; i < g_stVPAttr[u32Chn].stSrcAttr.u32VIBufNum; i++)
    {
        if (HI_SUCCESS != munmap(g_stVPAttr[u32Chn].stUsbcamPara.pMmapAddr[i],
                                 g_stVPAttr[u32Chn].stUsbcamPara.u32MmapLength[i]))
        {
            HI_ERR_VP("VP munmap failed\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VP_AttachLocalWin(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;    
    HI_S32 s32RetN = HI_FAILURE;
    HI_UNF_AVPLAY_LOW_DELAY_ATTR_S stLowDelay;
    HI_HANDLE hWinSrc;
    HI_U32 i;

    i = u32Chn;
    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
    {
        hWinSrc = g_stVPAttr[i].hLocalAVPLAY;
    }
    else
    {
        hWinSrc = g_stVPAttr[i].hVI;
    }

    s32Ret = HI_UNF_VO_AttachWindow(g_stVPAttr[i].hLocalWin, hWinSrc);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_VO_AttachWindow failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hLocalWin, HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_VO_SetWindowEnable failed, ret = 0x%08x\n", s32Ret);
        goto ERR0;
    }

    s32Ret = HI_UNF_VO_SetQuickOutputEnable(g_stVPAttr[i].hLocalWin, HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_VO_SetQuickOutputEnable failed, ret = 0x%08x\n", s32Ret);
        goto ERR1;
    }

    if (g_stVPAttr[i].hLocalAVPLAY == hWinSrc)
    {
        if (!(VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState))
        {
            s32Ret = HI_UNF_AVPLAY_GetAttr(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY, &stLowDelay);
            stLowDelay.bEnable = HI_TRUE;
            s32Ret |= HI_UNF_AVPLAY_SetAttr(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY, &stLowDelay);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VP("HI_UNF_AVPLAY_SetAttr LOW_DELAY failed, ret = 0x%08x\n", s32Ret);
                goto ERR1;
            }
        }
    }

    return HI_SUCCESS;

ERR1:
    s32RetN = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hLocalWin, HI_FALSE);
    if (HI_SUCCESS != s32RetN)
    {
        HI_ERR_VP("HI_UNF_VO_SetWindowEnable failed, ret = 0x%08x\n", s32RetN);
    }
ERR0:
    s32RetN = HI_UNF_VO_DetachWindow(g_stVPAttr[i].hLocalWin, hWinSrc);
    if (HI_SUCCESS != s32RetN)
    {
        HI_ERR_VP("HI_UNF_VO_DetachWindow failed, ret = 0x%08x\n", s32RetN);
    }

    return s32Ret;
}

static HI_S32 VP_DetachLocalWin(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    i = u32Chn;
    s32Ret = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hLocalWin, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VO_SetWindowEnable failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
    {
        s32Ret = HI_UNF_VO_DetachWindow(g_stVPAttr[i].hLocalWin, g_stVPAttr[i].hLocalAVPLAY);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VO_DetachWindow failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        s32Ret = HI_UNF_VO_DetachWindow(g_stVPAttr[i].hLocalWin, g_stVPAttr[i].hVI);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VO_DetachWindow failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VP_AttachRemoteWin(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;    
    HI_S32 s32RetN = HI_FAILURE;
    HI_UNF_AVPLAY_LOW_DELAY_ATTR_S stLowDelay;
    HI_U32 i;

    i = u32Chn;
    s32Ret = HI_UNF_VO_AttachWindow(g_stVPAttr[i].hRemoteWin, g_stVPAttr[i].hAVPLAY);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_VO_AttachWindow failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hRemoteWin, HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_VO_SetWindowEnable failed, ret = 0x%08x\n", s32Ret);
        goto ERR0;
    }

    s32Ret = HI_UNF_AVPLAY_GetAttr(g_stVPAttr[i].hAVPLAY, HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY, &stLowDelay);
    stLowDelay.bEnable = HI_TRUE;
    s32Ret |= HI_UNF_AVPLAY_SetAttr(g_stVPAttr[i].hAVPLAY, HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY, &stLowDelay);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_SetAttr LOW_DELAY failed, ret = 0x%08x\n", s32Ret);
        goto ERR1;
    }

    return HI_SUCCESS;

ERR1:
    s32RetN = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hRemoteWin, HI_FALSE);
    if (HI_SUCCESS != s32RetN)
    {
        HI_ERR_VP("HI_UNF_VO_SetWindowEnable failed, ret = 0x%08x\n", s32RetN);
    }
ERR0:
    s32RetN = HI_UNF_VO_DetachWindow(g_stVPAttr[i].hRemoteWin, g_stVPAttr[i].hAVPLAY);
    if (HI_SUCCESS != s32RetN)
    {
        HI_ERR_VP("HI_UNF_VO_DetachWindow failed, ret = 0x%08x\n", s32RetN);
    }

    return s32Ret;
}

static HI_S32 VP_DetachRemoteWin(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    i = u32Chn;
    s32Ret = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hRemoteWin, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VO_SetWindowEnable failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = HI_UNF_VO_DetachWindow(g_stVPAttr[i].hRemoteWin, g_stVPAttr[i].hAVPLAY);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VO_DetachWindow failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 VP_RTP_Create(HI_U32 u32Chn, RTP_CONTROL_MODE_E enCtrlMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_HANDLE hRTP = HI_INVALID_HANDLE;
    RTP_ATTR_S stRtpAttr;

    i = u32Chn;
    memset(&stRtpAttr, 0, sizeof(RTP_ATTR_S));

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hRTP)
    {
        HI_INFO_VP("VP RTP has already been created\n");
        return HI_SUCCESS;
    }

    /* create RTP/RTCP session */
    s32Ret = RTP_GetDefaultAttr(&stRtpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("Get RTP default attributes failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    stRtpAttr.bEnableRtcp = HI_TRUE;
    stRtpAttr.bIsIpv4 = g_stVPAttr[i].bIsIpv4;
    stRtpAttr.enCtrlMod = enCtrlMode;
    stRtpAttr.u32SplitSize   = g_stVPAttr[i].stRtpAttr.u32SplitSize;
    stRtpAttr.u32PayLoadType = g_stVPAttr[i].stRtpAttr.u32PayLoadType;
    stRtpAttr.u32RecvBufSize = g_stVPAttr[i].stRtpAttr.u32RecvBufSize;
    stRtpAttr.u32RecvBufNum  = g_stVPAttr[i].stRtpAttr.u32RecvBufNum;
    stRtpAttr.u32SortDepth   = g_stVPAttr[i].stRtpAttr.u32SortDepth;

    if (HI_TRUE == stRtpAttr.bIsIpv4)
    {
        memcpy(&stRtpAttr.stLocalNet.stIpv4Attr, &g_stVPAttr[i].stLocalNetAttr.stIpv4Attr, sizeof(struct sockaddr_in));
        memcpy(&stRtpAttr.stRemoteNet.stIpv4Attr, &g_stVPAttr[i].stRemoteNetAttr.stIpv4Attr, sizeof(struct sockaddr_in));
    }
    else
    {
        memcpy(&stRtpAttr.stLocalNet.stIpv6Attr, &g_stVPAttr[i].stLocalNetAttr.stIpv6Attr, sizeof(struct sockaddr_in6));
        memcpy(&stRtpAttr.stRemoteNet.stIpv6Attr, &g_stVPAttr[i].stRemoteNetAttr.stIpv6Attr, sizeof(struct sockaddr_in6));
    }

    s32Ret = RTP_Create(&stRtpAttr, &hRTP);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("Create RTP session failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    if (stRtpAttr.bEnableRtcp)
    {
        (HI_VOID)RTP_SetRtcpAttr(hRTP, g_stVPAttr[i].stRtcpAttr.bEnable, g_stVPAttr[i].stRtcpAttr.u32SendInterval);
    }

    g_stVPAttr[i].hRTP = hRTP;

    return HI_SUCCESS;
}

static HI_S32 VP_RTP_Destroy(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_INVALID_HANDLE != g_stVPAttr[u32Chn].hRTP)
    {
        s32Ret = RTP_Destroy(g_stVPAttr[u32Chn].hRTP);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("RTP_Destroy failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[u32Chn].hRTP = HI_INVALID_HANDLE;
    }

    return HI_SUCCESS;
}

static HI_S32 VP_RTP_SwitchCtrlMode(HI_U32 u32Chn, RTP_CONTROL_MODE_E enCtrlMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    RTP_ATTR_S stRtpAttr;

    i = u32Chn;
    memset(&stRtpAttr, 0, sizeof(RTP_ATTR_S));

    if (HI_INVALID_HANDLE == g_stVPAttr[i].hRTP)
    {
        HI_ERR_VP("RTP has not been created\n");
        return HI_ERR_VP_NOT_READY;
    }

    s32Ret = RTP_GetAttr(g_stVPAttr[i].hRTP, &stRtpAttr);
    stRtpAttr.enCtrlMod = enCtrlMode;    
    s32Ret |= RTP_SetAttr(g_stVPAttr[i].hRTP, &stRtpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("RTP_SetAttr failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 VP_USBCAM_Thread_Create(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    i = u32Chn;
    g_stVPAttr[i].bUSBCamStop = HI_FALSE;
    if (V4L2_PIX_FMT_YUYV == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
    {
        s32Ret = pthread_create(&g_stVPAttr[i].threadUSBCam, HI_NULL, VP_ScheduleCaptureYUV, HI_NULL);
    }
    else if (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
    {
        s32Ret = pthread_create(&g_stVPAttr[i].threadUSBCam, HI_NULL, VP_ScheduleCaptureMJPEG, HI_NULL);
    }
    else
    {
        HI_ERR_VP("Only support YUYV/MJPEG format!\n");
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("can not create VP_ScheduleCaptureYUV or VP_ScheduleCaptureMJPEG\n");
        g_stVPAttr[i].bUSBCamStop = HI_TRUE;
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 VP_USBCAM_Thread_Destroy(HI_U32 u32Chn)
{
    if (!g_stVPAttr[u32Chn].bUSBCamStop)
    {
        g_stVPAttr[u32Chn].bUSBCamStop = HI_TRUE;
        if (pthread_join(g_stVPAttr[u32Chn].threadUSBCam, NULL))
        {
            HI_ERR_VP("pthread_join failed, line = %d\n", __LINE__);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VP_PreView_Start(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    i = u32Chn;
    s32Ret = VP_AttachLocalWin(i);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("VP_AttachLocalWin failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    if (!(VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState))
    {
        if (HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        {
            s32Ret = VP_USBCAM_Thread_Create(i);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_USBCAM_Thread_Create failed, ret = 0x%08x\n", s32Ret);
                (HI_VOID)VP_DetachLocalWin(i);
                return s32Ret;
            }
        }
        
        if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
            && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
        {
            s32Ret = HI_UNF_AVPLAY_Start(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("HI_UNF_AVPLAY_Start failed, ret = 0x%08x\n", s32Ret);
                (HI_VOID)VP_USBCAM_Thread_Destroy(i);
                (HI_VOID)VP_DetachLocalWin(i);
                return s32Ret;
            }
        }
        else
        {
            s32Ret = HI_UNF_VI_Start(g_stVPAttr[i].hVI);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("HI_UNF_VI_Start failed, ret = 0x%08x\n", s32Ret);
                (HI_VOID)VP_USBCAM_Thread_Destroy(i);
                (HI_VOID)VP_DetachLocalWin(i);
                return s32Ret;
            }
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VP_PreView_Stop(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_AVPLAY_STOP_OPT_S stStopOpt;
    HI_U32 i;

    i = u32Chn;
    if (!(VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState))
    {
        if (HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        {
            (HI_VOID)VP_USBCAM_Thread_Destroy(i);
        }

        if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
            && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
        {
            stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
            stStopOpt.u32TimeoutMs = 0;
            s32Ret = HI_UNF_AVPLAY_Stop(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStopOpt);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("HI_UNF_AVPLAY_Stop failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }
        }
        else
        {
            s32Ret = HI_UNF_VI_Stop(g_stVPAttr[i].hVI);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VP("HI_UNF_VI_Stop failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }
        }
    }

    s32Ret = VP_DetachLocalWin(i);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("VP_DetachLocalWin failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 VP_LocalVideoMjpeg(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_AVPLAY_LOW_DELAY_ATTR_S stLowDelay;
    HI_U32 i;

    i = u32Chn;
    if(u32Chn >= MAX_VP_CHANNEL_NUM)
    {
        HI_ERR_VP("parameter err, u32Chn = 0x%x\n", u32Chn);
        return HI_FAILURE;
    }
    
    s32Ret = HI_UNF_VENC_AttachInput(g_stVPAttr[i].hVENC, g_stVPAttr[i].hVirWin);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VENC_AttachInput failed, ret = 0x%08x\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_VO_AttachWindow(g_stVPAttr[i].hVirWin,g_stVPAttr[i].hLocalAVPLAY);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VO_DetachWindow failed, ret = 0x%08x\n", s32Ret);
        goto ERR0;
    }

    s32Ret = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hVirWin, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VO_SetWindowEnable failed, ret = 0x%08x\n", s32Ret);
        goto ERR1;
    }

    if (!(VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState))
    {
        s32Ret = HI_UNF_AVPLAY_GetAttr(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY, &stLowDelay);
        stLowDelay.bEnable = HI_TRUE;
        s32Ret |= HI_UNF_AVPLAY_SetAttr(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY, &stLowDelay);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_SetAttr LOW_DELAY failed, ret = 0x%08x\n", s32Ret);
            goto ERR1;
        }

        s32Ret = HI_UNF_AVPLAY_Start(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_Start failed, ret = 0x%08x\n", s32Ret);
            goto ERR1;
        }
    }
    return HI_SUCCESS;
ERR1:
    s32Ret = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hVirWin, HI_FALSE);
    s32Ret = HI_UNF_VO_DetachWindow(g_stVPAttr[i].hVirWin,g_stVPAttr[i].hLocalAVPLAY);
ERR0:
    s32Ret = HI_UNF_VENC_DetachInput(g_stVPAttr[i].hVENC);

    return HI_FAILURE;
}

static HI_S32 VP_SendLocalVideo_Start(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    i = u32Chn;
    if (!(VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState))
    {
        s32Ret = VP_RTP_Create(i, RTP_ONLY_SEND);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_Create failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        s32Ret = VP_RTP_SwitchCtrlMode(i, RTP_SEND_AND_RECV);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_SwitchCtrlMode failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }

    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
         && (!(VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState)))
    {
        s32Ret = VP_USBCAM_Thread_Create(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_USBCAM_Thread_Create failed, ret = 0x%08x\n", s32Ret);
            goto ERR0;
        }
    }

    if (HI_FALSE == g_stVPAttr[i].bSendEmptyPackage)
    {
        g_stVPAttr[i].bNetSendStop = HI_FALSE;
        if (pthread_create(&g_stVPAttr[i].threadNetSend, HI_NULL, VP_ScheduleRtpSend, (HI_VOID*)(HI_SIZE_T)i))
        {
            HI_ERR_VP("can not create VP_ScheduleRtpSend\n");
            g_stVPAttr[i].bNetSendStop = HI_TRUE;
            goto ERR1;
        }
     }
    else
    {
        /*net send empty package -- > stop*/
        g_stVPAttr[i].bSendEmptyPackage = HI_FALSE;
    }

    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
    {
        s32Ret = VP_LocalVideoMjpeg(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_LocalVideoMjpeg failed, ret = 0x%08x\n", s32Ret);
            goto ERR2;
        }
    }
    else
    {
        s32Ret = HI_UNF_VENC_AttachInput(g_stVPAttr[i].hVENC, g_stVPAttr[i].hVI);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VENC_AttachInput failed, ret = 0x%08x\n", s32Ret);
            goto ERR2;
        }

        if (!(VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState))
        {
            s32Ret = HI_UNF_VI_Start(g_stVPAttr[i].hVI);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VP("HI_UNF_VI_Start failed, ret = 0x%08x\n", s32Ret);
                goto ERR3;
            }
        }
    }

    s32Ret = HI_UNF_VENC_Start(g_stVPAttr[i].hVENC);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_VENC_Start failed, ret = 0x%08x\n", s32Ret);
        goto ERR4;
    }

    return HI_SUCCESS;
    
ERR4:
    if (!(VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState))
    {
        if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
            && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
        {
            s32Ret = HI_UNF_AVPLAY_Stop(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        }
        else
        {
            s32Ret = HI_UNF_VI_Stop(g_stVPAttr[i].hVI);
        }
    }

    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
    {
        s32Ret = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hVirWin, HI_FALSE);
        s32Ret = HI_UNF_VO_DetachWindow(g_stVPAttr[i].hVirWin,g_stVPAttr[i].hLocalAVPLAY);
    }
    
ERR3:
    s32Ret = HI_UNF_VENC_DetachInput(g_stVPAttr[i].hVENC);

ERR2:
    g_stVPAttr[i].bNetSendStop = HI_TRUE;
    pthread_join(g_stVPAttr[i].threadNetSend, NULL);
ERR1:
    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
         && (!(VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState)))
    {
        (HI_VOID)VP_USBCAM_Thread_Destroy(i);
    }
ERR0:
    if (!(VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState))
    {
        (HI_VOID)VP_RTP_Destroy(i);
    }
    else if (HI_FALSE == g_stVPAttr[i].bSendEmptyPackage)
    {
        s32Ret = VP_RTP_SwitchCtrlMode(i, RTP_ONLY_RECV);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_SwitchCtrlMode failed, ret = 0x%08x\n", s32Ret);
        }
    }

    return HI_FAILURE;
}

static HI_S32 VP_SendLocalVideo_Stop(HI_U32 u32Chn)
{
    HI_UNF_AVPLAY_STOP_OPT_S stStopOpt;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    i = u32Chn;
    if (!(VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState))
    {
        if (!g_stVPAttr[i].bNetSendStop)
        {
            g_stVPAttr[i].bNetSendStop = HI_TRUE;
            if (pthread_join(g_stVPAttr[i].threadNetSend, NULL))
            {
                HI_ERR_VP("pthread_join failed, line = %d\n", __LINE__);
                return HI_FAILURE;
            }
        }

        s32Ret = VP_RTP_Destroy(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_Destroy failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        /*net  send empty package --> start*/
        g_stVPAttr[i].bSendEmptyPackage = HI_TRUE;
        /* no need to wait venc operation finish,because venc stop, release stream can be done.*/

    }

    s32Ret = HI_UNF_VENC_Stop(g_stVPAttr[i].hVENC);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VENC_Stop failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
    {
        s32Ret = HI_UNF_VO_SetWindowEnable(g_stVPAttr[i].hVirWin, HI_FALSE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VO_SetWindowEnable failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    
        s32Ret = HI_UNF_VO_DetachWindow(g_stVPAttr[i].hVirWin,g_stVPAttr[i].hLocalAVPLAY);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VO_DetachWindow failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = HI_UNF_VENC_DetachInput(g_stVPAttr[i].hVENC);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VENC_DetachInput failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    if (!(VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState))
    {
        if (HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        {
            (HI_VOID)VP_USBCAM_Thread_Destroy(i);
        }
        
        if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
            && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt))
        {
            stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
            stStopOpt.u32TimeoutMs = 0;
            s32Ret = HI_UNF_AVPLAY_Stop(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStopOpt);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("HI_UNF_AVPLAY_Stop failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }
        }
        else
        {
            s32Ret = HI_UNF_VI_Stop(g_stVPAttr[i].hVI);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VP("HI_UNF_VI_Stop failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VP_RecvRemoteVideo_Start(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    i = u32Chn;
    s32Ret = VP_AttachRemoteWin(i);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("VP_AttachRemoteWin failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    if (!(VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState))
    {
        s32Ret = VP_RTP_Create(i, RTP_ONLY_RECV);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_Create failed, ret = 0x%08x\n", s32Ret);
            (HI_VOID)VP_DetachRemoteWin(i);
            return s32Ret;
        }
    }
    else
    {
        s32Ret = VP_RTP_SwitchCtrlMode(i, RTP_SEND_AND_RECV);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_SwitchCtrlMode failed, ret = 0x%08x\n", s32Ret);
            (HI_VOID)VP_DetachRemoteWin(i);
            return s32Ret;
        }
    }

    g_stVPAttr[i].bNetRecvStop = HI_FALSE;
    s32Ret= pthread_create(&g_stVPAttr[i].threadNetRecv, HI_NULL, VP_ScheduleRtpRecv, (HI_VOID*)(HI_SIZE_T)i);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("can not create VP_ScheduleRtpRecv\n");
        g_stVPAttr[i].bNetRecvStop = HI_TRUE;
        goto ERR0;
    }

    s32Ret = HI_UNF_AVPLAY_Start(g_stVPAttr[i].hAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_Start failed, ret = 0x%08x\n", s32Ret);
        goto ERR1;
    }

    return HI_SUCCESS;

ERR1:    
    g_stVPAttr[i].bNetRecvStop = HI_TRUE;
    pthread_join(g_stVPAttr[i].threadNetRecv, NULL);
ERR0:
    if (!(VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState))
    {
        (HI_VOID)VP_RTP_Destroy(i);
    }
    else
    {
        s32Ret = VP_RTP_SwitchCtrlMode(i, RTP_ONLY_SEND);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_SwitchCtrlMode failed, ret = 0x%08x\n", s32Ret);
        }
    }

    (HI_VOID)VP_DetachRemoteWin(i);

    return HI_FAILURE;
}

static HI_S32 VP_RecvRemoteVideo_Stop(HI_U32 u32Chn)
{
    HI_UNF_AVPLAY_STOP_OPT_S stStopOpt;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    i = u32Chn;
    if (!g_stVPAttr[i].bNetRecvStop)
    {
        g_stVPAttr[i].bNetRecvStop = HI_TRUE;
        if (pthread_join(g_stVPAttr[i].threadNetRecv, NULL))
        {
            HI_ERR_VP("pthread_join failed, line = %d\n", __LINE__);
            return HI_FAILURE;
        }
    }

    if (!(VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState))
    {
        /*net send empty packages --- running */
        if (HI_TRUE == g_stVPAttr[i].bSendEmptyPackage)
        {
            g_stVPAttr[i].bSendEmptyPackage = HI_FALSE;

            if (!g_stVPAttr[i].bNetSendStop)
            {
                g_stVPAttr[i].bNetSendStop = HI_TRUE;
                if (pthread_join(g_stVPAttr[i].threadNetSend, NULL))
                {
                    HI_ERR_VP("pthread_join failed, line = %d\n", __LINE__);
                    return HI_FAILURE;
                }
            }
        }

        s32Ret = VP_RTP_Destroy(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_Destroy failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        s32Ret = VP_RTP_SwitchCtrlMode(i, RTP_ONLY_SEND);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RTP_SwitchCtrlMode failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }

    stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    stStopOpt.u32TimeoutMs = 0;
    s32Ret = HI_UNF_AVPLAY_Stop(g_stVPAttr[i].hAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStopOpt);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_Stop failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    (HI_VOID)VP_DetachRemoteWin(i);

    return HI_SUCCESS;
}


HI_S32 VP_StartImageOptimize(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_CODEC_VIDEO_CMD_S  stVdecCmdPara = {0};
    HI_UNF_AVPLAY_SCENE_MODE_E sceneNode = HI_UNF_AVPLAY_SCENE_MODE_NORMAL;
    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = HI_UNF_PQ_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_PQ_Init failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    } 
    
    s32Ret = HI_UNF_PQ_SetImageMode(HI_UNF_DISPLAY0, HI_UNF_OPTION_MODE_VIDEOPHONE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_PQ_SetImageMode failed, ret = 0x%08x\n", s32Ret);
    }
    HI_UNF_PQ_DeInit();

    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
        && g_stVPAttr[i].hLocalAVPLAY != HI_INVALID_HANDLE)
    {
        sceneNode = HI_UNF_AVPLAY_SCENE_MODE_VIDEOPHONE_PREVIEW;
        stVdecCmdPara.u32CmdID = HI_UNF_AVPLAY_SET_SCENE_MODE_CMD;
        stVdecCmdPara.pPara = &sceneNode;
        s32Ret = HI_UNF_AVPLAY_Invoke(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_INVOKE_VCODEC, &stVdecCmdPara);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_Invoke local failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }
    if (g_stVPAttr[i].hAVPLAY != HI_INVALID_HANDLE)
    {
        sceneNode = HI_UNF_AVPLAY_SCENE_MODE_VIDEOPHONE_REMOTE;
        stVdecCmdPara.u32CmdID = HI_UNF_AVPLAY_SET_SCENE_MODE_CMD;
        stVdecCmdPara.pPara = &sceneNode;
        s32Ret = HI_UNF_AVPLAY_Invoke(g_stVPAttr[i].hAVPLAY, HI_UNF_AVPLAY_INVOKE_VCODEC, &stVdecCmdPara);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_Invoke remote failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }
    return HI_SUCCESS;
}


HI_S32 VP_StopImageOptimize(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_CODEC_VIDEO_CMD_S  stVdecCmdPara = {0};
    HI_UNF_AVPLAY_SCENE_MODE_E sceneNode = HI_UNF_AVPLAY_SCENE_MODE_NORMAL;
    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    s32Ret = HI_UNF_PQ_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_PQ_Init failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    } 
    
    s32Ret = HI_UNF_PQ_SetImageMode(HI_UNF_DISPLAY0, HI_UNF_OPTION_MODE_NORMAL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_PQ_SetImageMode failed, ret = 0x%08x\n", s32Ret);
    }
    
    HI_UNF_PQ_DeInit();
    
    stVdecCmdPara.u32CmdID = HI_UNF_AVPLAY_SET_SCENE_MODE_CMD;
    stVdecCmdPara.pPara = &sceneNode;
    if ((HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
        && (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
        && g_stVPAttr[i].hLocalAVPLAY != HI_INVALID_HANDLE)
    {
        s32Ret = HI_UNF_AVPLAY_Invoke(g_stVPAttr[i].hLocalAVPLAY, HI_UNF_AVPLAY_INVOKE_VCODEC, &stVdecCmdPara);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_Invoke local failed, ret = 0x%08x\n", s32Ret);
        }
    }
    if (g_stVPAttr[i].hAVPLAY != HI_INVALID_HANDLE)
    {
        s32Ret = HI_UNF_AVPLAY_Invoke(g_stVPAttr[i].hAVPLAY, HI_UNF_AVPLAY_INVOKE_VCODEC, &stVdecCmdPara);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_Invoke remote failed, ret = 0x%08x\n", s32Ret);
        }
    }
    return HI_SUCCESS;
}


#ifdef VP_PROC_SUPPORT
static HI_S32 VPShowProc(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{    
    HI_U32 i=0;
    VP_ATTR_S *pVPAttr = g_stVPAttr;
    HI_S8 pVpStats[][8] = {"YES", "NO", "Run", "Stop"};
    HI_S8 pVpSrcMode[][8] = { "USBCAM", "VI", "BUTT"};
    HI_S8 pVpUSBMode[][8] = { "YUYV", "MJPEG", "BUTT"};
    HI_S8 pVpStreamType[][32] = {"MPEG2", "MPEG4 DIVX4 DIVX5", "AVS", "H263", "H264",
                             "REAL8", "REAL9", "VC-1", "VP6", "VP6F", "VP6A", "MJPEG",
                             "SORENSON SPARK", "DIVX3", "RAW", "JPEG", "VP8", "MSMPEG4V1",
                             "MSMPEG4V2", "MSVIDEO1", "WMV1", "WMV2", "RV10", "RV20",
                             "SVQ1", "SVQ3", "H261", "VP3", "VP5", "CINEPAK", "INDEO2",
                             "INDEO3", "INDEO4", "INDEO5", "MJPEGB", "MVC", "HEVC", "DV", "INVALID"};

    HI_PROC_Printf(pstBuf, "\n---------Hisilicon VP Out Info---------\n");

    for(i = 0; i < MAX_VP_CHANNEL_NUM; i++)
    {
        if (pVPAttr[i].hVP != HI_INVALID_HANDLE)
        {        
            HI_PROC_Printf(pstBuf, "chan %d infomation:\n", i);

            HI_PROC_Printf(pstBuf, "\t---- VP Source Info ----\t\n");
            if (VP_ENGINE_SRCATTED & pVPAttr[i].u32VpState)
            {
                HI_PROC_Printf(pstBuf, "\tAttached   \t:%s\n", pVpStats[0]);
                HI_PROC_Printf(pstBuf, "\tVP SrcMode     \t:%s\n", pVpSrcMode[pVPAttr[i].stSrcAttr.enSrcMode]);

                if (HI_UNF_VP_SRC_MODE_USBCAM == pVPAttr[i].stSrcAttr.enSrcMode)
                {
                    if (V4L2_PIX_FMT_YUYV == pVPAttr[i].stUsbcamPara.u32CapFmt)
                    {
                        HI_PROC_Printf(pstBuf, "\tCap Type   \t:%s\n", pVpUSBMode[0]);
                    }

                    if (V4L2_PIX_FMT_MJPEG == pVPAttr[i].stUsbcamPara.u32CapFmt)
                    {
                        HI_PROC_Printf(pstBuf, "\tCap Type   \t:%s\n", pVpUSBMode[1]);
                    }
                }

                HI_PROC_Printf(pstBuf, "\tSrc handle  \t:%#x\n", pVPAttr[i].stSrcAttr.hSrc);                
                HI_PROC_Printf(pstBuf, "\tBuf count   \t:%d\n", pVPAttr[i].stSrcAttr.u32VIBufNum);
            }
            else
            {
                HI_PROC_Printf(pstBuf, "\tAttached   \t:%s\n", pVpStats[1]);
            }

            HI_PROC_Printf(pstBuf, "\t---- VP Preview Info ----\t\n");
            if (VP_ENGINE_PREVIEW & pVPAttr[i].u32VpState)
            {
                HI_PROC_Printf(pstBuf, "\tWork State   \t:%s\n", pVpStats[2]);
                HI_PROC_Printf(pstBuf, "\tWindow_ID   \t:win%04x\n", pVPAttr[i].hLocalWin & 0xffff);                
                if (V4L2_PIX_FMT_MJPEG == pVPAttr[i].stUsbcamPara.u32CapFmt)
                {
                    HI_PROC_Printf(pstBuf, "\tAvplay_ID   \t:avplay%02d\n", pVPAttr[i].hLocalAVPLAY & 0xff );
                }
                else
                {
                    HI_PROC_Printf(pstBuf, "\tVi_ID   \t:vi%04d\n", pVPAttr[i].hVI & 0xffff );
                }
            }
            else
            {
                HI_PROC_Printf(pstBuf, "\tWork State   \t:%s\n", pVpStats[3]);
            }

            HI_PROC_Printf(pstBuf, "\t---- VP Sender Info ---- \t\n");
            if (VP_ENGINE_SENDED & pVPAttr[i].u32VpState)
            {
                HI_PROC_Printf(pstBuf, "\tWork State   \t:%s\n", pVpStats[2]);
                HI_PROC_Printf(pstBuf, "\tVenc Type  \t:%s\n", pVpStreamType[pVPAttr[i].stVencAttr.enVencType]);                
                HI_PROC_Printf(pstBuf, "\tVenc_ID   \t:venc%02d\n", pVPAttr[i].hVENC & 0xff);
                if (V4L2_PIX_FMT_MJPEG == pVPAttr[i].stUsbcamPara.u32CapFmt)
                {
                    HI_PROC_Printf(pstBuf, "\tAvplay_ID   \t:avplay%02d\n", pVPAttr[i].hLocalAVPLAY & 0xff );
                }
                else
                {
                    HI_PROC_Printf(pstBuf, "\tVi_ID   \t:vi%04d\n", pVPAttr[i].hVI & 0xffff );
                }
            }
            else
            {
                HI_PROC_Printf(pstBuf, "\tWork State   \t:%s\n", pVpStats[3]);
            }

            HI_PROC_Printf(pstBuf, "\t---- VP EmptyPack Info ----\t\n");
            if (pVPAttr[i].bSendEmptyPackage)
            {
                HI_PROC_Printf(pstBuf, "\tEmptyPackage  \t:%s\n", pVpStats[2]);
            }
            else
            {
                HI_PROC_Printf(pstBuf, "\tEmptyPackage  \t:%s\n", pVpStats[3]);
            }
            
            HI_PROC_Printf(pstBuf, "\t---- VP Receiver Info ----\t\n");
            if (VP_ENGINE_RECVED & pVPAttr[i].u32VpState)
            {
                HI_PROC_Printf(pstBuf, "\tWork State   \t:%s\n", pVpStats[2]);
                HI_PROC_Printf(pstBuf, "\tVdec Type  \t:%s\n", pVpStreamType[pVPAttr[i].enDecType]);                
                HI_PROC_Printf(pstBuf, "\tAvplay_ID   \t:avplay%02d\n", pVPAttr[i].hAVPLAY & 0xff );
                HI_PROC_Printf(pstBuf, "\tWindow_ID   \t:win%04x\n", pVPAttr[i].hRemoteWin & 0xffff);
            }
            else
            {
                HI_PROC_Printf(pstBuf, "\tWork State   \t:%s\n", pVpStats[3]);
            }

            HI_PROC_Printf(pstBuf, "\t----- VP Network Info -----\t\n"); 
            if (HI_TRUE == pVPAttr[i].bIsIpv4)
            {
                HI_PROC_Printf(pstBuf, "\tIpv4    \t:%s\n", pVpStats[0]); 
            }
            else
            {
                HI_PROC_Printf(pstBuf, "\tIpv4    \t:%s\n", pVpStats[1]); 
            }

            if (HI_INVALID_HANDLE != pVPAttr[i].hRTP)
            {
                HI_S32 s32Ret;
                RTP_RTCP_REPORT_S stReport;

                if (HI_TRUE == pVPAttr[i].stRtcpAttr.bEnable)
                {
                    HI_PROC_Printf(pstBuf, "\tRtcp Enable  \t:%s\n",  pVpStats[0]);                
                }
                else
                {
                    HI_PROC_Printf(pstBuf, "\tRtcp Enable  \t:%s\n",  pVpStats[1]);                
                }

                HI_PROC_Printf(pstBuf, "\tSendInterval   \t:%d(ms)\n", pVPAttr[i].stRtcpAttr.u32SendInterval);
                
                s32Ret = RTP_GetRtcpReport(pVPAttr[i].hRTP, &stReport);
                if (HI_SUCCESS == s32Ret)
                {
                   HI_PROC_Printf(pstBuf, "\t---Sender side Statics: \t\n");

                   HI_PROC_Printf(pstBuf, "\tSend(byte)  \t:%u\n", stReport.u32SendBytes);                
                   HI_PROC_Printf(pstBuf, "\tSend(packet)   \t:%u\n", stReport.u32SendPackets);
                   HI_PROC_Printf(pstBuf, "\tLostRate   \t:%u%%\n", stReport.stSendInfo.u32LossPercent);                
                   HI_PROC_Printf(pstBuf, "\tLost Packets   \t:%u\n", stReport.stSendInfo.u32LostPackets);
                   HI_PROC_Printf(pstBuf, "\tJitter  \t:%u(ms)\n", stReport.stSendInfo.u32Jitter);                
                   HI_PROC_Printf(pstBuf, "\tDelay   \t:%u(ms)\n", stReport.stSendInfo.u32Delay);

                   HI_PROC_Printf(pstBuf, "\t---Receiver side Statics: \t\n");
                   HI_PROC_Printf(pstBuf, "\tRecv(byte)  \t:%u\n", stReport.u32RecvBytes);                
                   HI_PROC_Printf(pstBuf, "\tRecv(packet)   \t:%u\n", stReport.u32RecvPacketNum);
                   HI_PROC_Printf(pstBuf, "\tLostRate   \t:%u%%\n", stReport.stRecvInfo.u32LossPercent);                
                   HI_PROC_Printf(pstBuf, "\tLost Packets   \t:%u\n", stReport.stRecvInfo.u32LostPackets);
                   HI_PROC_Printf(pstBuf, "\tJitter  \t:%u(ms)\n", stReport.stRecvInfo.u32Jitter);                
                   HI_PROC_Printf(pstBuf, "\tDelay   \t:%u(ms)\n", stReport.stRecvInfo.u32Delay);
                }
                else
                {
                     HI_ERR_VP("RTP_GetRtcpReport failed, ret = 0x%08x\n", s32Ret);
                }
 
            }
            else
            {
                HI_PROC_Printf(pstBuf, "\t RTP have not been created   \t\n");
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSetProc(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 u32ChnNum = 0;
    
    if (2 != u32Argc)
    {
        HI_ERR_VP("echo vp argc is incorrect.\n");
        return HI_FAILURE;
    }

    u32ChnNum = strtoul((HI_CHAR *)pu8Argv[0], HI_NULL, 10);

    if (u32ChnNum >= MAX_VP_CHANNEL_NUM)
    {
        HI_ERR_VP("invalid channel number %d\n.", u32ChnNum);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#endif

HI_S32 HI_UNF_VP_Init(HI_VOID)
{
    HI_U32 i;
    HI_S32 ret;

    if (HI_TRUE == bVPInited)
    {
        HI_WARN_VP("VP Module has been Initialized!\n");
        return HI_SUCCESS;
    }

    (HI_VOID)RTP_Init();

#ifdef VP_PROC_SUPPORT
        HI_SYS_Init();
        ret = HI_MODULE_Register(HI_ID_VP, VP_USR_PROC_DIR);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_VP("HI_MODULE_Register(\"%s\") return %d\n", VP_USR_PROC_DIR, ret);
        }

        /* Add proc dir */
        ret = HI_PROC_AddDir(VP_USR_PROC_DIR);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_VP("HI_PROC_AddDir(\"%s\") return %d\n", VP_USR_PROC_DIR, ret);
        }

        /* Will be added at /proc/hisi/${DIRNAME} directory */
        g_stVpProcEntry.pszDirectory = VP_USR_PROC_DIR;
        g_stVpProcEntry.pszEntryName = VP_USR_PROC_ENTRY_NAME;
        g_stVpProcEntry.pfnShowProc = VPShowProc;
        g_stVpProcEntry.pfnCmdProc = VPSetProc;
        g_stVpProcEntry.pPrivData = g_stVPAttr;
        ret = HI_PROC_AddEntry(HI_ID_VP, &g_stVpProcEntry);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_VP("HI_PROC_AddEntry(\"%s\") return %d\n", VP_USR_PROC_ENTRY_NAME, ret);
        }
#endif

    for (i = 0; i < MAX_VP_CHANNEL_NUM; i++)
    {
        g_stVPAttr[i].hVP = HI_INVALID_HANDLE;
    }

    bVPInited = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_DeInit(HI_VOID)
{
    HI_U32 i;

    if (HI_FALSE == bVPInited)
    {
        HI_WARN_VP("VP Module is not Initialized!\n");
        return HI_SUCCESS;
    }

    for (i = 0; i < MAX_VP_CHANNEL_NUM; i++)
    {
        if (HI_INVALID_HANDLE != g_stVPAttr[i].hVP)
        {
            HI_ERR_VP("VP engine(handle 0x%x) not been destroied.\n", g_stVPAttr[i].hVP);
            return HI_ERR_VP_ILLEGAL_STATE;
        }
    }

    (HI_VOID)RTP_DeInit();

#ifdef VP_PROC_SUPPORT
    HI_PROC_RemoveEntry(HI_ID_VP, &g_stVpProcEntry);
    HI_PROC_RemoveDir(VP_USR_PROC_DIR);
    HI_MODULE_UnRegister(HI_ID_VP);
    HI_SYS_DeInit();
#endif

    bVPInited = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_GetDefaultAttr(HI_UNF_VP_ATTR_S *pstDefAttr)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_VENC_CHN_ATTR_S stVencAttr;

    CHECK_VP_NULL_PTR(pstDefAttr);
    memset(&stVencAttr, 0, sizeof(HI_UNF_VENC_CHN_ATTR_S));
    s32Ret = HI_UNF_VENC_GetDefaultAttr(&stVencAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    memcpy(&pstDefAttr->stVencAttr, &stVencAttr, sizeof(HI_UNF_VENC_CHN_ATTR_S));

    pstDefAttr->stVencAttr.enVencType = HI_UNF_VCODEC_TYPE_H264;
    pstDefAttr->stVencAttr.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_720P;
    pstDefAttr->stVencAttr.u32Width  = 640;
    pstDefAttr->stVencAttr.u32Height = 480;
    pstDefAttr->stVencAttr.u32InputFrmRate  = 30;
    pstDefAttr->stVencAttr.u32TargetFrmRate = 30;
    pstDefAttr->stVencAttr.u32TargetBitRate = 3 * 1024 * 1024;
    pstDefAttr->stVencAttr.u32StrmBufSize = 1280 * 720 * 2;
    pstDefAttr->stVencAttr.u32Gop = 60;
    pstDefAttr->stVencAttr.bSlcSplitEn  = HI_FALSE;
    pstDefAttr->stVencAttr.bQuickEncode = HI_TRUE;
    pstDefAttr->enDecType = HI_UNF_VCODEC_TYPE_H264;
    pstDefAttr->bIsIpv4 = HI_TRUE;

    pstDefAttr->stRtpAttr.u32SplitSize   = RTP_DEFAULT_SPLIT_SIZE;
    pstDefAttr->stRtpAttr.u32PayLoadType = RTP_PT_H264; // todo, add if-else
    pstDefAttr->stRtpAttr.u32RecvBufSize = RTP_MAX_UDP_LEN;
    pstDefAttr->stRtpAttr.u32RecvBufNum  = RTP_DEFAULT_REORDER_BUF_NUM;
    pstDefAttr->stRtpAttr.u32SortDepth = RTP_DEFAULT_SORT_DEPTH;
    memset(&pstDefAttr->stLocalNetAttr, 0, sizeof(HI_UNF_VP_NET_ATTR_U));
    memset(&pstDefAttr->stRemoteNetAttr, 0, sizeof(HI_UNF_VP_NET_ATTR_U));

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_Create(const HI_UNF_VP_ATTR_S *pstAttr, HI_HANDLE *phVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_UNF_AVPLAY_ATTR_S stAvAttr;
    HI_UNF_AVPLAY_OPEN_OPT_S stAvOpt;
    HI_UNF_VCODEC_ATTR_S stAvVdecAttr;
    HI_HANDLE hAvplay;
    HI_HANDLE hVenc;
    HI_U8 *pu8VencStream = HI_NULL;

    CHECK_VP_INIT;
    CHECK_VP_NULL_PTR(pstAttr);
    CHECK_VP_NULL_PTR(phVP);
    s32Ret = VP_CheckAttr(pstAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    for (i = 0; i < MAX_VP_CHANNEL_NUM; i++)
    {
        if (HI_INVALID_HANDLE == g_stVPAttr[i].hVP)
        {
            break;
        }
    }

    if (MAX_VP_CHANNEL_NUM == i)
    {
        HI_ERR_VP("Create VP failed, max number reached!\n");
        return HI_ERR_VP_NOT_SUPPORT;
    }

    memset(&g_stVPAttr[i], 0, sizeof(VP_ATTR_S));
    memset(&stAvAttr, 0, sizeof(HI_UNF_AVPLAY_ATTR_S));
    memset(&stAvOpt, 0, sizeof(HI_UNF_AVPLAY_OPEN_OPT_S));
    memset(&stAvVdecAttr, 0, sizeof(HI_UNF_VCODEC_ATTR_S));
    s32Ret = HI_UNF_AVPLAY_GetDefaultConfig(&stAvAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_GetDefaultConfig failed, ret = 0x%08x\n", s32Ret);
        goto ERR0;
    }

    s32Ret = HI_UNF_AVPLAY_Create(&stAvAttr, &hAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_Create failed, ret = 0x%08x\n", s32Ret);
        goto ERR0;
    }

    g_stVPAttr[i].hAVPLAY = hAvplay;
    stAvOpt.enDecType  = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
    stAvOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
    stAvOpt.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;
    s32Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stAvOpt);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_ChnOpen failed, ret = 0x%08x\n", s32Ret);
        goto ERR1;
    }

    s32Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stAvVdecAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_GetAttr failed, ret = 0x%08x\n", s32Ret);
        goto ERR2;
    }

    stAvVdecAttr.enType = pstAttr->enDecType;
    stAvVdecAttr.enMode = HI_UNF_VCODEC_MODE_NORMAL;
    stAvVdecAttr.u32ErrCover  = 100;
    stAvVdecAttr.u32Priority  = 3;
    stAvVdecAttr.bOrderOutput = HI_TRUE;
    s32Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stAvVdecAttr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VP("HI_UNF_AVPLAY_SetAttr failed, ret = 0x%08x\n", s32Ret);
        goto ERR2;
    }

    s32Ret = HI_UNF_VENC_Create(&hVenc, &pstAttr->stVencAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VENC_Create failed, ret = 0x%08x\n", s32Ret);
        goto ERR2;
    }

    g_stVPAttr[i].hVENC = hVenc;

    pu8VencStream = (HI_U8 *)HI_MALLOC(HI_ID_VP, MAX_VENC_ONE_FRAME_SIZE + NAL_AND_FU_PACKET_LEN);
    if (HI_NULL == pu8VencStream)
    {
        HI_ERR_VP("VP malloc failed.\n");
        goto ERR3;
    }

    memset(pu8VencStream, 0, MAX_VENC_ONE_FRAME_SIZE + NAL_AND_FU_PACKET_LEN);
    g_stVPAttr[i].pu8VencStream = pu8VencStream;

    g_stVPAttr[i].hVI  = HI_INVALID_HANDLE;
    g_stVPAttr[i].hRTP = HI_INVALID_HANDLE;
    g_stVPAttr[i].hLocalWin  = HI_INVALID_HANDLE;
    g_stVPAttr[i].hRemoteWin = HI_INVALID_HANDLE;
    g_stVPAttr[i].hVirWin = HI_INVALID_HANDLE;
    g_stVPAttr[i].hLocalAVPLAY = HI_INVALID_HANDLE;
    g_stVPAttr[i].stRtcpAttr.bEnable = HI_TRUE;
    g_stVPAttr[i].stRtcpAttr.u32SendInterval = 5000;
    g_stVPAttr[i].bIsIpv4   = pstAttr->bIsIpv4;
    g_stVPAttr[i].enDecType = pstAttr->enDecType;
    memcpy(&g_stVPAttr[i].stVencAttr, &pstAttr->stVencAttr, sizeof(HI_UNF_VENC_CHN_ATTR_S));
    memcpy(&g_stVPAttr[i].stLocalNetAttr, &pstAttr->stLocalNetAttr, sizeof(HI_UNF_VP_NET_ATTR_U));
    memcpy(&g_stVPAttr[i].stRemoteNetAttr, &pstAttr->stRemoteNetAttr, sizeof(HI_UNF_VP_NET_ATTR_U));
    memcpy(&g_stVPAttr[i].stRtpAttr, &pstAttr->stRtpAttr, sizeof(HI_UNF_VP_RTP_ATTR_S));

    *phVP = (HI_ID_VP << 16) | i;

    g_stVPAttr[i].hVP = *phVP;
    return HI_SUCCESS;

ERR3:
    HI_UNF_VENC_Destroy(g_stVPAttr[i].hVENC);
    g_stVPAttr[i].hVENC = HI_INVALID_HANDLE;
ERR2:
    HI_UNF_AVPLAY_ChnClose(g_stVPAttr[i].hAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
ERR1:
    HI_UNF_AVPLAY_Destroy(g_stVPAttr[i].hAVPLAY);
    g_stVPAttr[i].hAVPLAY = HI_INVALID_HANDLE;
ERR0:
    return HI_FAILURE;
}

HI_S32 HI_UNF_VP_Destroy(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if ((VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState) 
        || (VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState)
        || (VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState))
    {
        HI_ERR_VP("Illegal state, check whether VP engine has been stopped\n");
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    if ((HI_INVALID_HANDLE != g_stVPAttr[i].hLocalWin) || (HI_INVALID_HANDLE != g_stVPAttr[i].hRemoteWin))
    {
        HI_ERR_VP("Detach local/remote window first, %x, %x\n", g_stVPAttr[i].hLocalWin, g_stVPAttr[i].hRemoteWin);
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    if ((V4L2_PIX_FMT_YUYV == g_stVPAttr[i].stUsbcamPara.u32CapFmt) && (HI_INVALID_HANDLE != g_stVPAttr[i].hVI))
    {
        HI_ERR_VP("Detach source first, YUV mode\n");
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    if ((V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
        && (HI_INVALID_HANDLE != g_stVPAttr[i].hLocalAVPLAY))
    {
        HI_ERR_VP("Detach source first, MJPEG mode\n");
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    s32Ret = VP_RTP_Destroy(i);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("VP_RTP_Destroy failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hVENC)
    {
        s32Ret = HI_UNF_VENC_Destroy(g_stVPAttr[i].hVENC);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_VENC_Destroy failed, ret = 0x%08x\n", s32Ret);
        }
    }

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hAVPLAY)
    {
        s32Ret = HI_UNF_AVPLAY_ChnClose(g_stVPAttr[i].hAVPLAY, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_ChnClose failed, ret = 0x%08x\n", s32Ret);
        }

        s32Ret = HI_UNF_AVPLAY_Destroy(g_stVPAttr[i].hAVPLAY);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("HI_UNF_AVPLAY_Destroy failed, ret = 0x%08x\n", s32Ret);
        }
    }

    if (HI_NULL != g_stVPAttr[i].pu8VencStream)
    {
        HI_FREE(HI_ID_VP, g_stVPAttr[i].pu8VencStream);
        g_stVPAttr[i].pu8VencStream = HI_NULL;
    }

    memset(&g_stVPAttr[i], 0, sizeof(VP_ATTR_S));
    g_stVPAttr[i].hVP          = HI_INVALID_HANDLE;
    g_stVPAttr[i].hVI          = HI_INVALID_HANDLE;
    g_stVPAttr[i].hVENC        = HI_INVALID_HANDLE;
    g_stVPAttr[i].hAVPLAY      = HI_INVALID_HANDLE;
    g_stVPAttr[i].hRTP         = HI_INVALID_HANDLE;
    g_stVPAttr[i].hLocalWin    = HI_INVALID_HANDLE;
    g_stVPAttr[i].hRemoteWin   = HI_INVALID_HANDLE;
    g_stVPAttr[i].hVirWin      = HI_INVALID_HANDLE;
    g_stVPAttr[i].hLocalAVPLAY = HI_INVALID_HANDLE;

    return s32Ret;
}

HI_S32 HI_UNF_VP_AttachSource(HI_HANDLE hVP, HI_UNF_VP_SOURCE_ATTR_S *pstSrcAttr)
{
    HI_S32 s32Ret = HI_FAILURE;
    struct v4l2_format stUsbFmt;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    CHECK_VP_NULL_PTR(pstSrcAttr);
    s32Ret = VP_CheckSrcAttr(pstSrcAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    memset(&stUsbFmt, 0, sizeof(struct v4l2_format));
    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (VP_ENGINE_SRCATTED & g_stVPAttr[i].u32VpState)
    {
        HI_ERR_VP("Illegal state, source has already been attached\n");
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    if (HI_UNF_VP_SRC_MODE_USBCAM == pstSrcAttr->enSrcMode)
    {
        stUsbFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        s32Ret = ioctl(pstSrcAttr->hSrc, VIDIOC_G_FMT, &stUsbFmt);
        if (s32Ret < 0)
        {
            HI_ERR_VP("Unable to get format, ret = 0x%08x\n", s32Ret);
            return HI_FAILURE;
        }
        else
        {
            g_stVPAttr[i].stUsbcamPara.u32CapFmt = stUsbFmt.fmt.pix.pixelformat;
        }

        if (V4L2_PIX_FMT_YUYV == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
        {
            s32Ret = VP_AttachUsbCamYUV(i, pstSrcAttr);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_AttachUsbCamYUV, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }
        }
        else if (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
        {
            s32Ret = VP_AttachUsbCamMJPEG(i, pstSrcAttr);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_AttachUsbCamMJPEG, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }
        }
        else
        {
            HI_ERR_VP("Only support YUYV/MJPEG format!\n");
            return HI_ERR_VP_NOT_SUPPORT;
        }
    }
    else if (HI_UNF_VP_SRC_MODE_VI == pstSrcAttr->enSrcMode)
    {
        /* in VI mode, user creates VI outside, only transfer hander here to attach VENC */
        g_stVPAttr[i].hVI = pstSrcAttr->hSrc;
    }
    else
    {
        HI_ERR_VP("Unrecognized source mode\n");
        return HI_FAILURE;
    }

    g_stVPAttr[i].u32VpState |= VP_ENGINE_SRCATTED;
    memcpy(&g_stVPAttr[i].stSrcAttr, pstSrcAttr, sizeof(HI_UNF_VP_SOURCE_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_DetachSource(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (!(VP_ENGINE_SRCATTED & g_stVPAttr[i].u32VpState))
    {
        HI_ERR_VP("Illegal state, check whether source has been attached with VP\n");
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    if ((VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState) || (VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState))
    {
        HI_ERR_VP("Illegal state, VP engine should be stopped firstly, VP state is (0x%x)\n", g_stVPAttr[i].u32VpState);
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    if (HI_UNF_VP_SRC_MODE_USBCAM == g_stVPAttr[i].stSrcAttr.enSrcMode)
    {
        if (V4L2_PIX_FMT_YUYV == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
        {
            s32Ret = VP_DetachUsbCamYUV(i);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_DetachUsbCamYUV failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }
        }
        else if (V4L2_PIX_FMT_MJPEG == g_stVPAttr[i].stUsbcamPara.u32CapFmt)
        {
            s32Ret = VP_DetachUsbCamMJPEG(i);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_DetachUsbCamMJPEG failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }
        }
        else
        {
            HI_ERR_VP("Only support YUYV/MJPEG format!\n");
            return HI_ERR_VP_NOT_SUPPORT;
        }
    }
    else
    {
        /* in VI mode, user creates or destroys VI outside */
        g_stVPAttr[i].hVI = HI_INVALID_HANDLE;
    }

    g_stVPAttr[i].u32VpState &= ~VP_ENGINE_SRCATTED;

    memset(&g_stVPAttr[i].stSrcAttr, 0, sizeof(HI_UNF_VP_SOURCE_ATTR_S));
    memset(&g_stVPAttr[i].stUsbcamPara, 0, sizeof(VP_USBCAM_PARAM_S));
    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_AttachLocalWin(HI_HANDLE hVP, HI_HANDLE hLocalWin)
{
    HI_S32 s32Ret  = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_INVALID_HANDLE == hLocalWin)
    {
        HI_ERR_VP("para hLocalWin is invalid.\n");
        return HI_ERR_VP_INVALID_PARA;
    }

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hLocalWin)
    {
        HI_ERR_VP("Local window has already been attached before\n");
        return HI_ERR_VP_NOT_SUPPORT;
    }

    g_stVPAttr[i].hLocalWin = hLocalWin;
    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_DetachLocalWin(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState)
    {
        HI_ERR_VP("Illegal state, VP preview should be stopped firstly\n");
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hLocalWin)
    {
        g_stVPAttr[i].hLocalWin = HI_INVALID_HANDLE;
        return HI_SUCCESS;
    }
    else
    {
        HI_ERR_VP("Local window is not attached\n");
        return HI_ERR_VP_NOT_SUPPORT;
    }
}

HI_S32 HI_UNF_VP_AttachRemoteWin(HI_HANDLE hVP, HI_HANDLE hRemoteWin)
{
    HI_S32 s32Ret  = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_INVALID_HANDLE == hRemoteWin)
    {
        HI_ERR_VP("para hRemoteWin is invalid.\n");
        return HI_ERR_VP_INVALID_PARA;
    }

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hRemoteWin)
    {
        HI_ERR_VP("Remote window has already been attached before\n");
        return HI_ERR_VP_NOT_SUPPORT;
    }

    g_stVPAttr[i].hRemoteWin = hRemoteWin;
    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_DetachRemoteWin(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState)
    {
        HI_ERR_VP("Illegal state, VP receiver should be stopped firstly\n");
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hRemoteWin)
    {
        g_stVPAttr[i].hRemoteWin = HI_INVALID_HANDLE;
        return HI_SUCCESS;
    }
    else
    {
        HI_ERR_VP("Remote window is not attached\n");
        return HI_ERR_VP_NOT_SUPPORT;
    }

}

HI_S32 HI_UNF_VP_GetAttr(HI_HANDLE hVP, HI_UNF_VP_ATTR_S *pstAttr)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    CHECK_VP_NULL_PTR(pstAttr);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    pstAttr->bIsIpv4   = g_stVPAttr[i].bIsIpv4;
    pstAttr->enDecType = g_stVPAttr[i].enDecType;
    memcpy(&pstAttr->stVencAttr, &g_stVPAttr[i].stVencAttr, sizeof(HI_UNF_VENC_CHN_ATTR_S));
    memcpy(&pstAttr->stLocalNetAttr, &g_stVPAttr[i].stLocalNetAttr, sizeof(HI_UNF_VP_NET_ATTR_U));
    memcpy(&pstAttr->stRemoteNetAttr, &g_stVPAttr[i].stRemoteNetAttr, sizeof(HI_UNF_VP_NET_ATTR_U));
    memcpy(&pstAttr->stRtpAttr, &g_stVPAttr[i].stRtpAttr, sizeof(HI_UNF_VP_RTP_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_SetAttr(HI_HANDLE hVP, const HI_UNF_VP_ATTR_S *pstAttr)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    CHECK_VP_NULL_PTR(pstAttr);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = VP_CheckAttr(pstAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    /* set dynamic attributes here, such as GOP, frame rate, bit rate, etc. */
    s32Ret = HI_UNF_VENC_SetAttr(g_stVPAttr[i].hVENC, &pstAttr->stVencAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("HI_UNF_VENC_SetAttr failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    g_stVPAttr[i].bIsIpv4   = pstAttr->bIsIpv4;
    g_stVPAttr[i].enDecType = pstAttr->enDecType;
    memcpy(&g_stVPAttr[i].stVencAttr, &pstAttr->stVencAttr, sizeof(HI_UNF_VENC_CHN_ATTR_S));
    memcpy(&g_stVPAttr[i].stLocalNetAttr, &pstAttr->stLocalNetAttr, sizeof(HI_UNF_VP_NET_ATTR_U));
    memcpy(&g_stVPAttr[i].stRemoteNetAttr, &pstAttr->stRemoteNetAttr, sizeof(HI_UNF_VP_NET_ATTR_U));

    if (!((VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState) || (VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState)))
    {
        memcpy(&g_stVPAttr[i].stRtpAttr, &pstAttr->stRtpAttr, sizeof(HI_UNF_VP_RTP_ATTR_S));
    }
    else
    {
        if (0 != memcmp((const char *)(&g_stVPAttr[i].stRtpAttr), (const char *)&pstAttr->stRtpAttr, sizeof(HI_UNF_VP_RTP_ATTR_S)))
        {
            HI_ERR_VP("vp has started, do not support set this rtp attr now\n");
            return HI_ERR_RTP_NOT_SUPPORT;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_Start(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = VP_StartImageOptimize(hVP);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("VP_StartImageOptimize fail, s32Ret = %d\n", s32Ret);
    }

    if (!(VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState))
    {    
        if (g_stVPAttr[i].hLocalWin == HI_INVALID_HANDLE)
        {
            HI_ERR_VP("Illegal state, local window should be attached, %x!\n", 
                g_stVPAttr[i].hLocalWin);
            return HI_ERR_VP_ILLEGAL_STATE;
        }

        if (!(VP_ENGINE_SRCATTED & g_stVPAttr[i].u32VpState))
        {
            HI_ERR_VP("Illegal state, check whether source has been attached\n");
            return HI_ERR_VP_ILLEGAL_STATE;
        }

        s32Ret = VP_PreView_Start(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_PreView_Start failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    
        g_stVPAttr[i].u32VpState |= VP_ENGINE_PREVIEW;
    }

    if (!(VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState))
    {
        if (g_stVPAttr[i].hRemoteWin == HI_INVALID_HANDLE) 
        {
            HI_ERR_VP("Illegal state, remote window should be attached, %x!\n", 
                g_stVPAttr[i].hRemoteWin);
            return HI_ERR_VP_ILLEGAL_STATE;
        }

        s32Ret = VP_RecvRemoteVideo_Start(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RecvRemoteVideo_Start failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[i].u32VpState |= VP_ENGINE_RECVED;
    }

    if (!(VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState))
    {
        /* source should be attached before start vp engine */
        if (!(VP_ENGINE_SRCATTED & g_stVPAttr[i].u32VpState))
        {
            HI_ERR_VP("Illegal state, check whether source has been attached\n");
            return HI_ERR_VP_ILLEGAL_STATE;
        }

        s32Ret = VP_SendLocalVideo_Start(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_Start_SendLocalVideo failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[i].u32VpState |= VP_ENGINE_SENDED;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_Stop(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    s32Ret = VP_StopImageOptimize(hVP);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("VP_StopImageOptimize fail, s32Ret = %d\n", s32Ret);
    }
    if (VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState)
    {
        s32Ret = VP_RecvRemoteVideo_Stop(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_RecvRemoteVideo_Stop failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[i].u32VpState &= ~VP_ENGINE_RECVED;
    }

    if (VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState)        
    {
        s32Ret = VP_SendLocalVideo_Stop(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_SendLocalVideo_Stop failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[i].u32VpState &= ~VP_ENGINE_SENDED;
    }

    if (VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState)
    {
        s32Ret = VP_PreView_Stop(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_PreView_Stop failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[i].u32VpState &= ~VP_ENGINE_PREVIEW;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_StartPreView(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    s32Ret = VP_StartImageOptimize(hVP);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("VP_StartImageOptimize fail, s32Ret = %d\n", s32Ret);
    }
    if (!(VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState))
    {    
        if (g_stVPAttr[i].hLocalWin == HI_INVALID_HANDLE)
        {
            HI_ERR_VP("Illegal state, local window should be attached, %x!\n", 
                g_stVPAttr[i].hLocalWin);
            return HI_ERR_VP_ILLEGAL_STATE;
        }

        if (!(VP_ENGINE_SRCATTED & g_stVPAttr[i].u32VpState))
        {
            HI_ERR_VP("Illegal state, check whether source has been attached\n");
            return HI_ERR_VP_ILLEGAL_STATE;
        }

        s32Ret = VP_PreView_Start(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_PreView_Start failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[i].u32VpState |= VP_ENGINE_PREVIEW;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_StopPreView(HI_HANDLE hVP)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    s32Ret = VP_StopImageOptimize(hVP);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("VP_StopImageOptimize fail, s32Ret = %d\n", s32Ret);
    }
    if (VP_ENGINE_PREVIEW & g_stVPAttr[i].u32VpState)
    {
        s32Ret = VP_PreView_Stop(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("VP_PreView_Stop failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }

        g_stVPAttr[i].u32VpState &= ~VP_ENGINE_PREVIEW;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_StartRemote(HI_HANDLE hVP, HI_UNF_VP_REMOTE_MODE_E enRmtMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    if (enRmtMode < HI_UNF_VP_RMT_MODE_SEND)
    {
        HI_ERR_VP("para enRmtMode is invalid.\n");
        return HI_ERR_VP_INVALID_PARA;
    }

    if (enRmtMode >= HI_UNF_VP_RMT_MODE_BUTT)
    {
        HI_ERR_VP("para enRmtMode is invalid.\n");
        return HI_ERR_VP_INVALID_PARA;
    }

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    s32Ret = VP_StartImageOptimize(hVP);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("VP_StartImageOptimize fail, s32Ret = %d\n", s32Ret);
    }
    if (enRmtMode & ((HI_U32)HI_UNF_VP_RMT_MODE_RECV))
    {
        if (!(VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState))
        {
            if (g_stVPAttr[i].hRemoteWin == HI_INVALID_HANDLE) 
            {
                HI_ERR_VP("Illegal state, remote window should be attached, %x!\n", 
                    g_stVPAttr[i].hRemoteWin);
                return HI_ERR_VP_ILLEGAL_STATE;
            }

            s32Ret = VP_RecvRemoteVideo_Start(i);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_RecvRemoteVideo_Start failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }

            g_stVPAttr[i].u32VpState |= VP_ENGINE_RECVED;
        }
    }

    if (enRmtMode & ((HI_U32)HI_UNF_VP_RMT_MODE_SEND))
    {
        if (!(VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState))
        {
            /* source should be attached before start vp engine */
            if (!(VP_ENGINE_SRCATTED & g_stVPAttr[i].u32VpState))
            {
                HI_ERR_VP("Illegal state, check whether source has been attached\n");
                return HI_ERR_VP_ILLEGAL_STATE;
            }

            s32Ret = VP_SendLocalVideo_Start(i);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_Start_SendLocalVideo failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }

            g_stVPAttr[i].u32VpState |= VP_ENGINE_SENDED;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_StopRemote(HI_HANDLE hVP, HI_UNF_VP_REMOTE_MODE_E enRmtMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);

    if (enRmtMode < HI_UNF_VP_RMT_MODE_SEND)
    {
        HI_ERR_VP("para enRmtMode is invalid.\n");
        return HI_ERR_VP_INVALID_PARA;
    }

    if (enRmtMode >= HI_UNF_VP_RMT_MODE_BUTT)
    {
        HI_ERR_VP("para enRmtMode is invalid.\n");
        return HI_ERR_VP_INVALID_PARA;
    }

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    s32Ret = VP_StopImageOptimize(hVP);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("VP_StopImageOptimize fail, s32Ret = %d\n", s32Ret);
    }
    if (enRmtMode & ((HI_U32)HI_UNF_VP_RMT_MODE_RECV))
    {
        if (VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState)
        {
            s32Ret = VP_RecvRemoteVideo_Stop(i);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_RecvRemoteVideo_Stop failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }

            g_stVPAttr[i].u32VpState &= ~VP_ENGINE_RECVED;
        }
    }

    if (enRmtMode & ((HI_U32)HI_UNF_VP_RMT_MODE_SEND))
    {
        if (VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState)        
        {
            s32Ret = VP_SendLocalVideo_Stop(i);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VP("VP_SendLocalVideo_Stop failed, ret = 0x%08x\n", s32Ret);
                return s32Ret;
            }

            g_stVPAttr[i].u32VpState &= ~VP_ENGINE_SENDED;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_ConfigRtcp(HI_HANDLE hVP, HI_UNF_VP_RTCP_CFG_S *pstRtcpCfg)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    CHECK_VP_NULL_PTR(pstRtcpCfg);

    if ((pstRtcpCfg->u32SendInterval < 500) || (pstRtcpCfg->u32SendInterval > 65535) 
        || (pstRtcpCfg->u32SendInterval % 500))
    {
        HI_ERR_VP("RTCP send interval(%d) is invalid.\n", pstRtcpCfg->u32SendInterval);
        return HI_ERR_VP_INVALID_PARA;
    }

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    g_stVPAttr[i].stRtcpAttr.bEnable = (HI_BOOL)pstRtcpCfg->u32EnableFlag;
    g_stVPAttr[i].stRtcpAttr.u32SendInterval = pstRtcpCfg->u32SendInterval;

    if (!((VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState) || (VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState)))
    {
        /* The RTP/RTCP session is created in HI_UNF_VP_Start, if user configs RTCP attributes before HI_UNF_VP_Start,
            only save config here, and the configuration will not take effect until HI_UNF_VP_Start */
        return HI_SUCCESS;
    }
    else
    {
        /* set RTCP attributes dynamicly */
        s32Ret = RTP_SetRtcpAttr(g_stVPAttr[i].hRTP, (HI_BOOL)pstRtcpCfg->u32EnableFlag, pstRtcpCfg->u32SendInterval);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VP("RTP_SetRtcpAttr failed, ret = 0x%08x\n", s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_GetNetStatics(HI_HANDLE hVP, HI_UNF_VP_NET_STA_S *pstNetStatics)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    RTP_RTCP_REPORT_S stReport;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    CHECK_VP_NULL_PTR(pstNetStatics);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (!((VP_ENGINE_SENDED & g_stVPAttr[i].u32VpState) || (VP_ENGINE_RECVED & g_stVPAttr[i].u32VpState)))
    {
        HI_ERR_VP("Illegal state, check whether VP engine has been started\n");
        return HI_ERR_VP_ILLEGAL_STATE;
    }

    s32Ret = RTP_GetRtcpReport(g_stVPAttr[i].hRTP, &stReport);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VP("RTP_GetRtcpReport failed, ret = 0x%08x\n", s32Ret);
        return s32Ret;
    }

    pstNetStatics->u32SendBytes = stReport.u32SendBytes;
    pstNetStatics->u32SendPacketNum = stReport.u32SendPackets;
    pstNetStatics->u32RecvBytes = stReport.u32RecvBytes;
    pstNetStatics->u32RecvPacketNum = stReport.u32RecvPacketNum;
    pstNetStatics->stRecvInfo.u32Delay  = stReport.stRecvInfo.u32Delay;
    pstNetStatics->stRecvInfo.u32Jitter = stReport.stRecvInfo.u32Jitter;
    pstNetStatics->stRecvInfo.u32TotalLostPacket = stReport.stRecvInfo.u32LostPackets;
    pstNetStatics->stRecvInfo.u32LossFraction = stReport.stRecvInfo.u32LossPercent;
    pstNetStatics->stSendInfo.u32Delay  = stReport.stSendInfo.u32Delay;
    pstNetStatics->stSendInfo.u32Jitter = stReport.stSendInfo.u32Jitter;
    pstNetStatics->stSendInfo.u32TotalLostPacket = stReport.stSendInfo.u32LostPackets;
    pstNetStatics->stSendInfo.u32LossFraction = stReport.stSendInfo.u32LossPercent;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VP_GetVencHandle(HI_HANDLE hVP, HI_HANDLE *phVenc)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    CHECK_VP_NULL_PTR(phVenc);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hVENC)
    {
        *phVenc = g_stVPAttr[i].hVENC;
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 HI_UNF_VP_GetAvplayHandle(HI_HANDLE hVP, HI_HANDLE *phAvplay)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    CHECK_VP_INIT;
    CHECK_VP_HANDLE(hVP);
    CHECK_VP_NULL_PTR(phAvplay);

    s32Ret = VP_FindValidChannel(hVP, &i);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_INVALID_HANDLE != g_stVPAttr[i].hAVPLAY)
    {
        *phAvplay = g_stVPAttr[i].hAVPLAY;
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
