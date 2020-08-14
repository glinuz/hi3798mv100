/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

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
#include <string.h>
#include <pthread.h>

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
#include <linux/fb.h>
#include <linux/videodev2.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_vi.h"
#include "hi_unf_venc.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_ecs.h"
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_adp_hdmi_rx.h"

#define VI_TYPE_REAL 0
#define VI_TYPE_VIR_YUV 1

#ifndef PRINT_SMP
#define PRINT_SMP(fmt...)       printf(fmt)
#endif

/******************************************************************
********************** USB CAM ************************************
******************************************************************/
#define CAM_FPS 30

#define SAMPLE_USB_CAM_BUFFER_NUM 6

#define SAMPLE_VI_USB_CAM_WIDTH 640
#define SAMPLE_VI_USB_CAM_HEIGHT 480

typedef struct UsbCamBuffer
{
    HI_U8*       pu8VirAddr[SAMPLE_USB_CAM_BUFFER_NUM];     /* mmap address */
    HI_MMZ_BUF_S stMmzFrm[SAMPLE_USB_CAM_BUFFER_NUM];       /* MMZ buffer, malloc->MMZ->VI */
} USB_CAM_Buffer_S;

typedef struct hiUSB_CAM_THREAD_ARG_S
{
    HI_HANDLE hVi;
    HI_U32    u32USBCamFmt;
    int       UsbCamFd;
} USB_CAM_THREAD_ARG_S;

static char* UsbCamDev = "/dev/video0";
static HI_BOOL g_ViRunFlag = HI_FALSE;

USB_CAM_Buffer_S UsbCamBuff;

static pthread_t g_threadSampleUSBCam;
USB_CAM_THREAD_ARG_S g_stSampleUSBCamArgs;
static HI_BOOL g_bSampleUSBCamStop = HI_FALSE;

/******************************************************************
*************************** VENC **********************************
******************************************************************/
typedef struct hiVENC_THREAD_ARG_S
{
    HI_HANDLE   hVenc;
    HI_HANDLE   hAvplayForVdec;
    FILE*       pFileToSave;
} VENC_THREAD_ARG_S;

static pthread_t g_threadSampleVenc;
static VENC_THREAD_ARG_S g_stSampleVencArgs;
static HI_BOOL g_bSampleVencStop = HI_FALSE;

static void* SampleVencRoutine(void* args)
{
    VENC_THREAD_ARG_S* pArgs = (VENC_THREAD_ARG_S*)args;

    HI_S32 ret = HI_FAILURE;
    HI_UNF_VENC_STREAM_S vencStream;
    HI_UNF_STREAM_BUF_S vdecStream;
    static HI_U32 ptsForVdec = 0;

    HI_HANDLE hVenc = pArgs->hVenc;
    HI_HANDLE hAvplay = pArgs->hAvplayForVdec;
    FILE* pFile = pArgs->pFileToSave;

    while (!g_bSampleVencStop)
    {
        ret = HI_UNF_VENC_AcquireStream(hVenc, &vencStream, 0);
        if (HI_SUCCESS != ret)
        {
            usleep(10 * 1000);
            continue;
        }

        if (pFile) /* save to file */
        {
            if (vencStream.u32SlcLen > 0)
            {
                ret = fwrite(vencStream.pu8Addr, 1, vencStream.u32SlcLen, pFile);
                if (ret != vencStream.u32SlcLen)
                {
                    PRINT_SMP("fwrite err, write size=%d, slclen=%d\n", ret, vencStream.u32SlcLen);
                }
            }
        }

        if (HI_INVALID_HANDLE != hAvplay) /* send to VDEC */
        {
            ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID,
                                       vencStream.u32SlcLen, &vdecStream, 0);

            if (HI_SUCCESS == ret)
            {
                memcpy(vdecStream.pu8Data, vencStream.pu8Addr, vencStream.u32SlcLen);

                ptsForVdec = vencStream.u32PtsMs;
                if (0 == ptsForVdec)
                {
                    ptsForVdec = 1;
                }

                ret = HI_UNF_AVPLAY_PutBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID,
                                           vencStream.u32SlcLen, ptsForVdec);
                if (ret != HI_SUCCESS)
                {
                    PRINT_SMP("call HI_UNF_AVPLAY_PutBuf failed.\n");
                }
            }
            else
            {
                PRINT_SMP("call HI_UNF_AVPLAY_GetBuf failed(ERR=%#x), go on...\n", ret);
            }
        }

        ret = HI_UNF_VENC_ReleaseStream(hVenc, &vencStream);
        if (HI_SUCCESS != ret)
        {
            PRINT_SMP("HI_UNF_VENC_ReleaseStream failed, u32Ret=%x\n", ret);
        }
    }

    return NULL;
}

/*********************************************************************
 *******************************  USB cam *****************************
 **********************************************************************/
HI_U32 SampleUsbCamInit(USB_CAM_THREAD_ARG_S* pArgs)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 i;
    struct v4l2_capability stUsbCap;
    struct v4l2_format stUsbFmt = {0};
    struct v4l2_streamparm* setfps;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers rb;
    enum v4l2_buf_type type;
    HI_S32 fps = CAM_FPS;
    char*   TmpCamDev = "/dev/video1";
    HI_U32 fdUsbCam  = 0;

    /* open usb camera with non-block mode */
    fdUsbCam = open(UsbCamDev, O_RDWR | O_NONBLOCK, 0);
    if (HI_FAILURE == fdUsbCam)
    {
        fdUsbCam = open(TmpCamDev, O_RDWR | O_NONBLOCK, 0);
        if (HI_FAILURE == fdUsbCam)
        {
            PRINT_SMP("open %s failed\r\n", TmpCamDev);
            return HI_FAILURE;
        }
    }
    else
    {
        PRINT_SMP("open %s Ok\r\n", UsbCamDev);
    }

    s32Ret = ioctl(fdUsbCam, VIDIOC_QUERYCAP, &stUsbCap);
    if (s32Ret < 0)
    {
        PRINT_SMP("Error opening device %s: unable to query device.\n", UsbCamDev);
        close(fdUsbCam);
        return HI_FAILURE;
    }
    
    PRINT_SMP("USB CAM'driver: %s\n\t card: %s\n\t bus_info: %s\n\t version: %d\n\t capabilities: 0x%x\n\t device_caps: 0x%x\n",
                stUsbCap.driver, \
                stUsbCap.card, \
                stUsbCap.bus_info, \
                stUsbCap.version, \
                stUsbCap.capabilities, \
                stUsbCap.device_caps);

    if ((stUsbCap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0)
    {
        PRINT_SMP("Error opening device %s: video capture not supported.\n", UsbCamDev);
        close(fdUsbCam);
        return HI_FAILURE;
    }

    if (!(stUsbCap.capabilities & V4L2_CAP_STREAMING))
    {
        PRINT_SMP("%s Not support streaming i/o\n", UsbCamDev);
        close(fdUsbCam);
        return HI_FAILURE;
    }

    /* test format if it supports */
    memset(&stUsbFmt, 0, sizeof(struct v4l2_format));
    stUsbFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(fdUsbCam, VIDIOC_G_FMT, &stUsbFmt) < 0)
    {
        PRINT_SMP("get format failed\n");
        close(fdUsbCam);
        return HI_FAILURE;
    }
    else
    {
        PRINT_SMP("USB CAM's orignal FMT:\n");
        PRINT_SMP("Width = %d\n", stUsbFmt.fmt.pix.width);
        PRINT_SMP("Height = %d\n", stUsbFmt.fmt.pix.height);
        PRINT_SMP("Image size = %d\n", stUsbFmt.fmt.pix.sizeimage);
        PRINT_SMP("pixelformat = %d\n", stUsbFmt.fmt.pix.pixelformat);
        PRINT_SMP("field = %d\n", stUsbFmt.fmt.pix.field);
        if (V4L2_PIX_FMT_MJPEG == stUsbFmt.fmt.pix.pixelformat)
        {
            PRINT_SMP("V4L2_PIX_FMT_MJPEG\r\n");
        }
        else if (V4L2_PIX_FMT_YUYV == stUsbFmt.fmt.pix.pixelformat)
        {
            PRINT_SMP("V4L2_PIX_FMT_YUYV\r\n");
        }
        else
        {
            PRINT_SMP("fmt unknown\r\n");
        }

        PRINT_SMP("\n");
        // pArgs->u32USBCamFmt = stUsbFmt.fmt.pix.pixelformat;
    }

    /* set format */
    stUsbFmt.fmt.pix.width  = SAMPLE_VI_USB_CAM_WIDTH;
    stUsbFmt.fmt.pix.height = SAMPLE_VI_USB_CAM_HEIGHT;
    stUsbFmt.fmt.pix.pixelformat = pArgs->u32USBCamFmt;
    s32Ret = ioctl(fdUsbCam, VIDIOC_S_FMT, &stUsbFmt);
    if (s32Ret < 0)
    {
        PRINT_SMP("Unable to set format: %d, err:%d.\n", stUsbFmt.fmt.pix.pixelformat, errno);
        close(fdUsbCam);
        return s32Ret;
    }

    if (ioctl(fdUsbCam, VIDIOC_G_FMT, &stUsbFmt) < 0)
    {
        PRINT_SMP("get format failed\n");
        close(fdUsbCam);
        return HI_FAILURE;
    }
    else
    {
        PRINT_SMP("After set attr:\r\nWidth = %d\n", stUsbFmt.fmt.pix.width);
        PRINT_SMP("Height = %d\n", stUsbFmt.fmt.pix.height);
        PRINT_SMP("Image size = %d\n", stUsbFmt.fmt.pix.sizeimage);
        PRINT_SMP("pixelformat = %d\n", stUsbFmt.fmt.pix.pixelformat);
        if (V4L2_PIX_FMT_MJPEG == stUsbFmt.fmt.pix.pixelformat)
        {
            PRINT_SMP("V4L2_PIX_FMT_MJPEG\r\n");
        }
        else if (V4L2_PIX_FMT_YUYV == stUsbFmt.fmt.pix.pixelformat)
        {
            PRINT_SMP("V4L2_PIX_FMT_YUYV\r\n");
        }
        else
        {
            PRINT_SMP("fmt unknown\r\n");
        }

        PRINT_SMP("\n");
    }

    setfps = (struct v4l2_streamparm*)calloc(1, sizeof(struct v4l2_streamparm));
    if (HI_NULL == setfps)
    {
        PRINT_SMP("calloc memory failed, %d, %s\n", __LINE__, __FUNCTION__);
        close(fdUsbCam);
        return HI_FAILURE;
    }

    memset(setfps, 0, sizeof(struct v4l2_streamparm));
    setfps->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    s32Ret = ioctl(fdUsbCam, VIDIOC_G_PARM, setfps);
    if (s32Ret == 0)
    {
        PRINT_SMP("\n  Frame rate:   %u/%u\n",
                  setfps->parm.capture.timeperframe.denominator,
                  setfps->parm.capture.timeperframe.numerator
                 );
    }
    else
    {
        perror("Unable to read out current frame rate");
        free(setfps);
        close(fdUsbCam);
        return HI_FAILURE;
    }

    if ((setfps->parm.capture.timeperframe.numerator == 1)
        && (setfps->parm.capture.timeperframe.denominator == fps))
    {
        ; /* OK, fps is aready the value we want */
    }
    else
    {
        /* set frame rate */
        setfps->parm.capture.timeperframe.numerator   = 1;
        setfps->parm.capture.timeperframe.denominator = fps;
        s32Ret = ioctl(fdUsbCam, VIDIOC_S_PARM, setfps);
        if (HI_FAILURE == s32Ret)
        {
            perror("Unable to set frame rate");
            free(setfps);
            close(fdUsbCam);
            return HI_FAILURE;
        }

        s32Ret = ioctl(fdUsbCam, VIDIOC_G_PARM, setfps);
        if (s32Ret == 0)
        {
            if ((setfps->parm.capture.timeperframe.numerator != 1)
                || (setfps->parm.capture.timeperframe.denominator != fps))
            {
                PRINT_SMP("\n  Frame rate:   %u/%u fps (requested frame rate %u fps is "
                          "not supported by device)\n",
                          setfps->parm.capture.timeperframe.denominator,
                          setfps->parm.capture.timeperframe.numerator,
                          fps);
            }
            else
            {
                PRINT_SMP("\n  Frame rate:   %d fps\n", fps);
            }
        }
        else
        {
            perror("Unable to read out current frame rate");
            free(setfps);
            close(fdUsbCam);
            return HI_FAILURE;
        }
    }

    PRINT_SMP("\nUSB CAM's output mode: %d \n", setfps->parm.output.outputmode);

    /* request buffers */
    memset(&rb, 0, sizeof(struct v4l2_requestbuffers));
    rb.count = SAMPLE_USB_CAM_BUFFER_NUM;
    rb.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;

    s32Ret = ioctl(fdUsbCam, VIDIOC_REQBUFS, &rb);
    if (s32Ret < 0)
    {
        PRINT_SMP("Unable to allocate buffers: %d.\n", errno);
        free(setfps);
        close(fdUsbCam);
        return s32Ret;
    }

    for (i = 0; i < SAMPLE_USB_CAM_BUFFER_NUM; i++)
    {
        snprintf(UsbCamBuff.stMmzFrm[i].bufname,
                 sizeof(UsbCamBuff.stMmzFrm[i].bufname), "CamYuv_%d", i);

        UsbCamBuff.stMmzFrm[i].bufsize = stUsbFmt.fmt.pix.height * stUsbFmt.fmt.pix.width * 2;
        s32Ret = HI_MMZ_Malloc(&UsbCamBuff.stMmzFrm[i]);
        if (s32Ret != HI_SUCCESS)
        {
            PRINT_SMP("HI_MMZ_Malloc failed\n");
            free(setfps);
            close(fdUsbCam);
            return s32Ret;
        }

        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        s32Ret = ioctl(fdUsbCam, VIDIOC_QUERYBUF, &buf);
        if (s32Ret < 0)
        {
            PRINT_SMP("Unable to query buffer (%d).\n", s32Ret);
            free(setfps);
            close(fdUsbCam);
            return s32Ret;
        }

        UsbCamBuff.pu8VirAddr[i] = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
                                        MAP_SHARED, fdUsbCam, buf.m.offset);
        if (HI_NULL == UsbCamBuff.pu8VirAddr[i])
        {
            PRINT_SMP("malloc failed\n");
            free(setfps);
            close(fdUsbCam);
            return s32Ret;
        }
    }

    /* release the unused buffer firstly */
    for (i = 0; i < SAMPLE_USB_CAM_BUFFER_NUM; i++)
    {
        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.index = i;
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        s32Ret = ioctl(fdUsbCam, VIDIOC_QBUF, &buf);
        if (s32Ret < 0)
        {
            PRINT_SMP("Unable to queue buffer %d.\n", s32Ret);
            free(setfps);
            close(fdUsbCam);
            return s32Ret;
        }
    }

    type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    /* begin to capture frame from camera */
    s32Ret = ioctl(fdUsbCam, VIDIOC_STREAMON, &type);
    if (s32Ret < 0)
    {
        PRINT_SMP("Unable to streamon (%d).\n", errno);
        free(setfps);
        close(fdUsbCam);
        return s32Ret;
    }

    free(setfps);
    pArgs->UsbCamFd = fdUsbCam;

    return HI_SUCCESS;
}

HI_U32 SampleUsbCamDeinit(USB_CAM_THREAD_ARG_S* pArgs)
{
    enum v4l2_buf_type type;
    HI_S32 s32Ret;
    HI_U32 i;

    type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s32Ret = ioctl(pArgs->UsbCamFd, VIDIOC_STREAMOFF, &type);
    if (s32Ret < 0)
    {
        PRINT_SMP("Unable to streamoff (%d).\n", errno);
    }

    for (i = 0; i < SAMPLE_USB_CAM_BUFFER_NUM; i++)
    {
        if (UsbCamBuff.stMmzFrm[i].bufsize)
        {
            s32Ret = HI_MMZ_Free(&UsbCamBuff.stMmzFrm[i]);
            if (HI_SUCCESS != s32Ret)
            {
                PRINT_SMP("HI_MMZ_Free failed, ret = 0x%08x\n", s32Ret);
            }
            UsbCamBuff.stMmzFrm[i].bufsize = 0;
            munmap(UsbCamBuff.pu8VirAddr[i], UsbCamBuff.stMmzFrm[i].bufsize);
        }
    }

    close(pArgs->UsbCamFd);

    PRINT_SMP(" Deinit V4L2 \n");

    return HI_SUCCESS;
}

void* SampleUSBCamRoutine(void* args)
{
    HI_S32 s32Ret;
    HI_U32 totalFrame = 0;
    struct v4l2_buffer buf;
    struct v4l2_format stUsbFmt;
    struct v4l2_streamparm stUsbStrm;
    HI_U32 fail_cnt = 0;
    HI_UNF_VIDEO_FRAME_INFO_S stFrame;
    HI_UNF_VCODEC_FRMRATE_S stFrameRate;
    HI_U32 u32Width = SAMPLE_VI_USB_CAM_WIDTH;
    HI_U32 u32Height = SAMPLE_VI_USB_CAM_HEIGHT;

    USB_CAM_THREAD_ARG_S* pArgs = (USB_CAM_THREAD_ARG_S*)args;
    HI_HANDLE hVi = pArgs->hVi;

    memset(&stUsbFmt, 0, sizeof(struct v4l2_format));
    memset(&stFrame, 0, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    memset(&stFrameRate, 0, sizeof(HI_UNF_VCODEC_FRMRATE_S));

    stFrameRate.u32fpsInteger = CAM_FPS;
    stFrameRate.u32fpsDecimal = 0;

    while (!g_bSampleUSBCamStop)
    {
        if (HI_SUCCESS == SampleUsbCamInit(pArgs))
        {
            break;
        }
        else
        {
            SampleUsbCamDeinit(pArgs);
            usleep(100000);
        }
    }

    stUsbFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s32Ret = ioctl(pArgs->UsbCamFd, VIDIOC_G_FMT, &stUsbFmt);
    if (s32Ret < 0)
    {
        //PRINT_SMP("Unable to get format, ret = 0x%08x\n", s32Ret);
    }
    else
    {
        u32Width  = stUsbFmt.fmt.pix.width;
        u32Height = stUsbFmt.fmt.pix.height;
    }

    stUsbStrm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    s32Ret = ioctl(pArgs->UsbCamFd, VIDIOC_G_PARM, &stUsbStrm);
    if (s32Ret < 0)
    {
        //PRINT_SMP("Unable to get frame rate, ret = 0x%08x\n", s32Ret);
    }
    else
    {
        if (stUsbStrm.parm.capture.timeperframe.numerator != 0)
        {
            stFrameRate.u32fpsInteger = stUsbStrm.parm.capture.timeperframe.denominator / stUsbStrm.parm.capture.timeperframe.numerator;
            stFrameRate.u32fpsDecimal = stUsbStrm.parm.capture.timeperframe.denominator % stUsbStrm.parm.capture.timeperframe.numerator;
        }
    }

    while (!g_bSampleUSBCamStop)
    {
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        /* read data from usb camera */
        s32Ret = ioctl(pArgs->UsbCamFd, VIDIOC_DQBUF, &buf);
        if (s32Ret == -1)
        {
            usleep(10 * 1000);
            fail_cnt++;
            if (fail_cnt >= 300)
            {
                if (g_ViRunFlag)
                {
                    HI_UNF_VI_Stop(hVi);
                    g_ViRunFlag = HI_FALSE;
                }

                SampleUsbCamDeinit(pArgs);

                if (HI_SUCCESS == SampleUsbCamInit(pArgs))
                {
                    HI_UNF_VI_Start(hVi);
                    g_ViRunFlag = HI_TRUE;
                }

                fail_cnt = 0;
            }
        }
        else
        {
            totalFrame++;
            fail_cnt = 0;

            memcpy(UsbCamBuff.stMmzFrm[buf.index].user_viraddr, UsbCamBuff.pu8VirAddr[buf.index],
                   UsbCamBuff.stMmzFrm[buf.index].bufsize);

            stFrame.stVideoFrameAddr[0].u32YAddr   = UsbCamBuff.stMmzFrm[buf.index].phyaddr;
            stFrame.stVideoFrameAddr[0].u32CAddr   = UsbCamBuff.stMmzFrm[buf.index].phyaddr;
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

            if (V4L2_PIX_FMT_YUYV == pArgs->u32USBCamFmt)
            {
                s32Ret = HI_UNF_VI_QueueFrame(hVi, &stFrame);
                if (HI_SUCCESS != s32Ret)
                {
                    PRINT_SMP(" VI unable to queue buffer, ret = 0x%08x\n", s32Ret);
                    usleep(10 * 1000);
                }
            }
            else
            {
                PRINT_SMP("fmt unknown\r\n");
            }

            /* reput frame into queue */
            s32Ret = ioctl(pArgs->UsbCamFd, VIDIOC_QBUF, &buf);
            if (s32Ret < 0)
            {
                PRINT_SMP("Unable to queue buffer, ret = 0x%08x\n", s32Ret);
            }
        }

        //Always try to Dequeue frame from vi
        s32Ret = HI_UNF_VI_DequeueFrame(hVi, &stFrame);
        if (HI_SUCCESS != s32Ret)
        {
           // PRINT_SMP(" VI unable to dequeue buffer, ret = 0x%08x\n", s32Ret);
        }
    }

    SampleUsbCamDeinit(pArgs);
    return NULL;
}

HI_S32 HI_ADP_VIVENC_Init(HI_UNF_VI_E enViPort, HI_U32 u32ViMode,
                          HI_HANDLE* phVi, HI_UNF_VCODEC_TYPE_E enVencFmt, HI_HANDLE* phVenc)
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hVi;
    HI_HANDLE hVenc;
    HI_UNF_VI_ATTR_S stViAttr;
    HI_UNF_VENC_CHN_ATTR_S stVeAttr;
    HI_UNF_VENC_CHN_ATTR_S* pstVeAttr = &stVeAttr;
    HIAPI_RUN(HI_UNF_VI_Init(), ret);
    if (ret != HI_SUCCESS)
    {
        goto HDMIRX_DeInit;
    }

    HIAPI_RUN(HI_UNF_VI_GetDefaultAttr(&stViAttr), ret);
    if (ret != HI_SUCCESS)
    {
        goto VENC_DeInit;
    }

    HIAPI_RUN(HI_UNF_VENC_Init(), ret);
    if (ret != HI_SUCCESS)
    {
        goto VI_DeInit;
    }


    if (VI_TYPE_REAL == u32ViMode)
    {
        ret = HI_ADP_HDMIRX_Init_GetViAttr(&stViAttr);
        if (HI_SUCCESS != ret)
        {
            printf("HI_ADP_HDMIRX_Init failed\n");
            goto VENC_DeInit;
        }

        stViAttr.bVirtual = HI_FALSE;
        stViAttr.enBufMgmtMode = HI_UNF_VI_BUF_ALLOC;
        stViAttr.u32BufNum = 6;
    }
    else if (VI_TYPE_VIR_YUV == u32ViMode)
    {
        stViAttr.bVirtual = HI_TRUE;
        stViAttr.stInputRect.s32X = 0;
        stViAttr.stInputRect.s32Y = 0;
        stViAttr.stInputRect.s32Width  = SAMPLE_VI_USB_CAM_WIDTH;
        stViAttr.stInputRect.s32Height = SAMPLE_VI_USB_CAM_HEIGHT;
        stViAttr.enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;//HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
        stViAttr.enBufMgmtMode = HI_UNF_VI_BUF_ALLOC;
        stViAttr.u32BufNum = SAMPLE_USB_CAM_BUFFER_NUM;
    }

    sleep(1);
    HIAPI_RUN(HI_UNF_VI_Create(enViPort, &stViAttr, &(hVi)), ret);
    if (ret != HI_SUCCESS)
    {
        goto VENC_DeInit;
    }

    HIAPI_RUN(HI_UNF_VENC_GetDefaultAttr(pstVeAttr), ret);
    pstVeAttr->enVencType = enVencFmt;
    pstVeAttr->u32Height = stViAttr.stInputRect.s32Height;
    pstVeAttr->u32Width = stViAttr.stInputRect.s32Width;
    pstVeAttr->bSlcSplitEn  = HI_FALSE;
    pstVeAttr->u32Gop = 300;

    if (pstVeAttr->u32Width > 1280)
    {
        pstVeAttr->enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
        pstVeAttr->u32StrmBufSize   = 1920 * 1080 * 2;
        pstVeAttr->u32TargetBitRate = 5 * 1024 * 1024;
    }
    else if (pstVeAttr->u32Width > 720)
    {
        pstVeAttr->enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_720P;
        pstVeAttr->u32StrmBufSize   = 1280 * 720 * 2;
        pstVeAttr->u32TargetBitRate = 3 * 1024 * 1024;
    }
    else if (pstVeAttr->u32Width > 352)
    {
        pstVeAttr->enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_D1;
        pstVeAttr->u32StrmBufSize   = 720 * 576 * 2;
        pstVeAttr->u32TargetBitRate = 3 / 2 * 1024 * 1024;
    }
    else
    {
        pstVeAttr->enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_CIF;
        pstVeAttr->u32StrmBufSize   = 352 * 288 * 2;
        pstVeAttr->u32TargetBitRate = 800 * 1024;
    }

    if (VI_TYPE_REAL != u32ViMode)
    {
        pstVeAttr->u32InputFrmRate  = 30;
        pstVeAttr->u32TargetFrmRate = 30;
    }
    else
    {
        pstVeAttr->u32InputFrmRate  = 25;
        pstVeAttr->u32TargetFrmRate = 24;
    }

    HIAPI_RUN(HI_UNF_VENC_Create(&hVenc, pstVeAttr), ret);
    if (ret != HI_SUCCESS)
    {
        goto VI_Destroy;
    }

    HIAPI_RUN(HI_UNF_VENC_AttachInput(hVenc, hVi), ret);
    if (ret != HI_SUCCESS)
    {
        goto VENC_Destroy;
    }

    HIAPI_RUN(HI_UNF_VI_Start(hVi), ret);
    if (ret != HI_SUCCESS)
    {
        goto VENC_DetachInput;
    }

    g_ViRunFlag = HI_TRUE;

    HIAPI_RUN(HI_UNF_VENC_Start(hVenc), ret);
    if (ret != HI_SUCCESS)
    {
        goto VI_Stop;
    }

    *phVi   = hVi;
    *phVenc = hVenc;

    if (VI_TYPE_REAL != u32ViMode)
    {
        g_stSampleUSBCamArgs.hVi = hVi;
        g_stSampleUSBCamArgs.u32USBCamFmt = V4L2_PIX_FMT_YUYV;
        g_bSampleUSBCamStop = HI_FALSE;
        pthread_create(&g_threadSampleUSBCam, HI_NULL, SampleUSBCamRoutine, (void*)(&g_stSampleUSBCamArgs));
    }
    else
    {
        g_bSampleUSBCamStop = HI_TRUE;
    }

    return ret;

VI_Stop:
    HI_UNF_VI_Stop(hVi);
    g_ViRunFlag = HI_FALSE;

VENC_DetachInput:
    HI_UNF_VENC_DetachInput(hVenc);

VENC_Destroy:
    HI_UNF_VENC_Destroy(hVenc);

VI_Destroy:
    HI_UNF_VI_Destroy(hVi);

VENC_DeInit:
    HI_UNF_VENC_DeInit();

VI_DeInit:
    HI_UNF_VI_DeInit();

HDMIRX_DeInit:
    if (VI_TYPE_REAL == u32ViMode)
    {
        HI_ADP_HDMIRX_DeInit();
    }

    return ret;

}

HI_S32 HI_ADP_VIVENC_DeInit(HI_HANDLE hVi, HI_HANDLE hVenc, HI_U32 u32ViMode)
{
    HI_S32 ret = HI_SUCCESS;

    HIAPI_RUN(HI_UNF_VENC_Stop(hVenc), ret);
    HIAPI_RUN(HI_UNF_VENC_DetachInput(hVenc), ret);

    HIAPI_RUN(HI_UNF_VENC_Destroy(hVenc), ret);
    HIAPI_RUN(HI_UNF_VENC_DeInit(), ret);

    if (!g_bSampleUSBCamStop)
    {
        g_bSampleUSBCamStop = HI_TRUE;
        pthread_join(g_threadSampleUSBCam, NULL);
    }

    if (g_ViRunFlag)
    {
        HIAPI_RUN(HI_UNF_VI_Stop(hVi), ret);
        g_ViRunFlag = HI_FALSE;
    }
    HIAPI_RUN(HI_UNF_VI_Destroy(hVi), ret);
    HIAPI_RUN(HI_UNF_VI_DeInit(), ret);

    if (VI_TYPE_REAL == u32ViMode)
    {
        HI_ADP_HDMIRX_DeInit();
    }

    return ret;
}

HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 ret;
    HI_HANDLE hVi;
    HI_HANDLE hVenc;
    HI_HANDLE hWinVdecVo;
    HI_HANDLE hWinViVo;
    HI_HANDLE hAvplay;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_AVPLAY_LOW_DELAY_ATTR_S stLowDelay;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_CHAR strmfilename[64];
    HI_RECT_S stRectViVo;
    HI_UNF_VCODEC_TYPE_E enVencFmt = HI_UNF_VCODEC_TYPE_H264;
    HI_S32 s32ViPort = HI_UNF_VI_PORT0;
    HI_U32 u32ViMode = 0;
    HI_BOOL bEnableVIVO = HI_TRUE;
    HI_UNF_ENC_FMT_E voFormat = HI_UNF_ENC_FMT_720P_50;
    HI_UNF_VCODEC_TYPE_E enVdecFmt;

    if (6 == argc)
    {
        enVencFmt = strtol(argv[1], NULL, 0);
        s32ViPort = strtol(argv[2], NULL, 0);
        u32ViMode = strtol(argv[3], NULL, 0);
        bEnableVIVO = strtol(argv[4], NULL, 0);
        voFormat = HIADP_Disp_StrToFmt(argv[5]);
    }
    else if (5 == argc)
    {
        enVencFmt = strtol(argv[1], NULL, 0);
        s32ViPort = strtol(argv[2], NULL, 0);
        u32ViMode = strtol(argv[3], NULL, 0);
        bEnableVIVO = strtol(argv[4], NULL, 0);
    }
    else if (4 == argc)
    {
        enVencFmt = strtol(argv[1], NULL, 0);
        s32ViPort = strtol(argv[2], NULL, 0);
        u32ViMode = strtol(argv[3], NULL, 0);
    }
    else if (3 == argc)
    {
        enVencFmt = strtol(argv[1], NULL, 0);
        s32ViPort = strtol(argv[2], NULL, 0);
        u32ViMode = 0;
    }
    else if (2 == argc)
    {
        enVencFmt = strtol(argv[1], NULL, 0);
        s32ViPort = HI_UNF_VI_PORT0;
        u32ViMode = 0;
    }
    else
    {
        PRINT_SMP("Usage: %s vencFmt [viPort] [viMode] [bEnViVo] [voFormat]\n"
                  "       vencFmt: [0:H264] | 1:H263 | 2:MPEG4\n"
                  "       viPort:  0 | [1]\n"
                  "       viMode:  [0: VI REAL] | 1: USBCAM YUV \n"
                  "       bEnViVo: 0:disable VIVO | [1:enable VIVO]\n"
                  "       voFormat:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n"
                  , argv[0]);
        PRINT_SMP("Example:%s 0 0 0 1 1080i_50\n", argv[0]);

        return 0;
    }

    if ((u32ViMode > 1) || enVencFmt > 0)
    {
        PRINT_SMP("input param unsupport\n");
        return 0;
    }

    switch (enVencFmt)
    {
        case 0:
            sprintf(strmfilename, "venc_vi%d.h264", s32ViPort);
            enVdecFmt = HI_UNF_VCODEC_TYPE_H264;
            enVencFmt = HI_UNF_VCODEC_TYPE_H264;
            break;
        case 1:
            sprintf(strmfilename, "venc_vi%d.h263", s32ViPort);
            enVdecFmt = HI_UNF_VCODEC_TYPE_H263;
            enVencFmt = HI_UNF_VCODEC_TYPE_H263;
            break;
        case 2:
            sprintf(strmfilename, "venc_vi%d.mp4", s32ViPort);
            enVdecFmt = HI_UNF_VCODEC_TYPE_MPEG4;
            enVencFmt = HI_UNF_VCODEC_TYPE_MPEG4;
            break;
        default:
            enVdecFmt = HI_UNF_VCODEC_TYPE_H264;
            enVencFmt = HI_UNF_VCODEC_TYPE_H264;
            sprintf(strmfilename, "venc_vi%d.h264", s32ViPort);
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    ret = HIADP_Disp_Init(voFormat);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("call HIADP_Disp_DeInit failed.\n");
        goto SYS_DEINIT;
    }

    ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);

    stRectViVo.s32X = 0;
    stRectViVo.s32Y = 100;
    stRectViVo.s32Width  = 640;
    stRectViVo.s32Height = 480;
    ret |= HIADP_VO_CreatWin(&stRectViVo, &hWinVdecVo);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    stRectViVo.s32X = 640;
    stRectViVo.s32Y = 100;
    stRectViVo.s32Width  = 640;
    stRectViVo.s32Height = 480;
    ret |= HIADP_VO_CreatWin(&stRectViVo, &hWinViVo);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    ret  = HIADP_AVPlay_RegADecLib();
    ret |= HI_UNF_AVPLAY_Init();
    if (ret != HI_SUCCESS)
    {
        PRINT_SMP("call HI_UNF_AVPLAY_Init failed.\n");
    }

    ret  = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
    AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
    ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (ret != HI_SUCCESS)
    {
        PRINT_SMP("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    ret  = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto CHN_CLOSE;
    }

    ret = HI_UNF_VO_AttachWindow(hWinVdecVo, hAvplay);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("call HI_UNF_VO_AttachWindow failed.\n");
        goto CHN_CLOSE;
    }

    ret = HI_UNF_VO_SetWindowEnable(hWinVdecVo, HI_TRUE);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY, &stLowDelay);
    stLowDelay.bEnable = HI_TRUE;
    ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_LOW_DELAY, &stLowDelay);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("call HI_UNF_AVPLAY_SetAttr LOW_DELAY failed.\n");
        goto WIN_DETACH;
    }

    HIAPI_RUN(HIADP_AVPlay_SetVdecAttr(hAvplay, enVdecFmt, HI_UNF_VCODEC_MODE_NORMAL), ret);

    HIAPI_RUN(HI_ADP_VIVENC_Init(s32ViPort, u32ViMode, &hVi, enVencFmt, &hVenc), ret);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("call HI_ADP_VIVENC_Init failed.\n");
        goto WIN_DETACH;
    }

    g_stSampleVencArgs.hAvplayForVdec = hAvplay;
    g_stSampleVencArgs.hVenc = hVenc;

    // g_stSampleVencArgs.pFileToSave = fopen(strmfilename, "wb");
    g_stSampleVencArgs.pFileToSave = NULL;

    if (bEnableVIVO)
    {
        HI_UNF_VO_SetWindowZorder(hWinViVo, HI_LAYER_ZORDER_MOVETOP);
        ret = HI_UNF_VO_AttachWindow(hWinViVo, hVi);
        if (HI_SUCCESS != ret)
        {
            PRINT_SMP("call HI_UNF_VO_AttachWindow failed.\n");
            goto VIVENC_DeInit;
        }

        ret = HI_UNF_VO_SetWindowEnable(hWinViVo, HI_TRUE);
        if (HI_SUCCESS != ret)
        {
            PRINT_SMP("call HI_UNF_VO_SetWindowEnable failed.\n");
            goto WIN_DETACH_VIVO;
        }

        ret = HI_UNF_VO_SetQuickOutputEnable(hWinViVo, HI_TRUE);
        if (HI_SUCCESS != ret)
        {
            PRINT_SMP("HI_UNF_VO_SetQuickOutputEnable failed, ret = 0x%08x\n", ret);
            goto WIN_DETACH_VIVO;
        }
    }

    HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);

    pthread_create(&g_threadSampleVenc, HI_NULL, SampleVencRoutine, (void*)(&g_stSampleVencArgs));

    sleep(1);
    PRINT_SMP("\npress any key to quit!\n");
    getchar();

    g_bSampleVencStop = HI_TRUE;
    pthread_join(g_threadSampleVenc, NULL);

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &Stop);

WIN_DETACH_VIVO:
    if (bEnableVIVO)
    {
        HI_UNF_VO_SetWindowEnable(hWinViVo, HI_FALSE);
        HI_UNF_VO_DetachWindow(hWinViVo, hVi);
    }

VIVENC_DeInit:
    HI_ADP_VIVENC_DeInit(hVi, hVenc, u32ViMode);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWinVdecVo, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWinVdecVo, hAvplay);

CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

    HI_UNF_AVPLAY_Destroy(hAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

    HI_UNF_VO_DestroyWindow(hWinViVo);
    HI_UNF_VO_DestroyWindow(hWinVdecVo);
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    return ret;
}
