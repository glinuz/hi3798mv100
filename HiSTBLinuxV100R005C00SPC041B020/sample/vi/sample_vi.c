/******************************************************************************

  Copyright (C), 2001-2050, Hisilicon Tech. Co., Ltd.

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
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_vi.h"
#include "hi_adp.h"
#include "hi_adp_mpi.h"
#include "hi_adp_hdmi_rx.h"

/*User alloc frame buffer*/
#define USER_MMAP_BUFFER 0

HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 Ret;
    HI_UNF_ENC_FMT_E enFormat;
    HI_HANDLE hWin, hVi;
    HI_UNF_VI_ATTR_S stViAttr;
#if USER_MMAP_BUFFER
    HI_UNF_VI_BUFFER_ATTR_S stBufAttr;
    HI_MMZ_BUF_S stMmzBuf;
    HI_U32 u32Stride;
    HI_U32 i;
#endif

    HI_SYS_Init();
    HIADP_MCE_Exit();

    Ret = HI_UNF_VI_GetDefaultAttr(&stViAttr);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_VI_GetDefaultAttr failed\n");
    }

    Ret = HI_ADP_HDMIRX_Init_GetViAttr(&stViAttr);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_ADP_HDMIRX_Init failed\n");
        goto ERR_SYS_Deint;
    }

    if (argc > 7)
    {
        printf("Usage: %s [vo_format] [Width] [Height] [X] [Y] [VideoFormat]\n"
               "    [vo_format]: [720P_50] 2160P_30 | 2160P_24 | 720P_60 | 1080P_30 | 1080i_50 | 1080i_60\n"
               "    [VideoFormat]:  0:YUV422 | 1:YUV420\n",
               argv[0]);
        printf("Example:%s 720P_50 640 480 0 0 1 \n", argv[0]);
        return -1;
    }
    else if (argc == 2)
    {
        enFormat = HIADP_Disp_StrToFmt(argv[1]);
    }
    else if (argc == 4)
    {
        enFormat = HIADP_Disp_StrToFmt(argv[1]);
        stViAttr.stInputRect.s32Width  = strtoul(argv[2], NULL, 10);
        stViAttr.stInputRect.s32Height = strtoul(argv[3], NULL, 10);

    }
    else if (argc == 6)
    {
        enFormat = HIADP_Disp_StrToFmt(argv[1]);
        stViAttr.stInputRect.s32Width  = strtoul(argv[2], NULL, 10);
        stViAttr.stInputRect.s32Height = strtoul(argv[3], NULL, 10);
        stViAttr.stInputRect.s32X = strtoul(argv[4], NULL, 10);
        stViAttr.stInputRect.s32Y = strtoul(argv[5], NULL, 10);

    }
    else if (argc == 7)
    {
        enFormat = HIADP_Disp_StrToFmt(argv[1]);
        stViAttr.stInputRect.s32Width  = strtoul(argv[2], NULL, 10);
        stViAttr.stInputRect.s32Height = strtoul(argv[3], NULL, 10);
        stViAttr.stInputRect.s32X = strtoul(argv[4], NULL, 10);
        stViAttr.stInputRect.s32Y = strtoul(argv[5], NULL, 10);
        stViAttr.enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422 + (argv[6][0] - '0');
    }
    else
    {
        printf("Usage: %s [vo_format] [Width] [Height] [X] [Y] [VideoFormat]\n"
               "    [vo_format]: [720P_50] | 720P_60 | 1080P_30 | 1080i_50 | 1080i_60\n"
               "    [VideoFormat]:  0:YUV422 | 1:YUV420\n",
               argv[0]);
        printf("Example:%s 720P_50 640 480 0 0 1 \n", argv[0]);
        enFormat = HI_UNF_ENC_FMT_720P_50;
    }

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        printf("HIADP_Disp_Init failed\n");
        goto ERR0;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (HI_SUCCESS != Ret)
    {
        printf("HIADP_VO_Init failed\n");
        goto ERR1;
    }

    Ret = HIADP_VO_CreatWin(HI_NULL, &hWin);
    if (HI_SUCCESS != Ret)
    {
        printf("HIADP_VO_CreatWin failed\n");
        goto ERR2;
    }


    printf("@@[vo_format:%d][wh:%dx%d][xy:%dx%d]\n", \
           enFormat, \
           stViAttr.stInputRect.s32Width, \
           stViAttr.stInputRect.s32Height, \
           stViAttr.stInputRect.s32X, \
           stViAttr.stInputRect.s32Y);


    sleep(1);

    Ret = HI_UNF_VI_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_VI_Init failed\n");
        goto ERR3;
    }

#if USER_MMAP_BUFFER
    stViAttr.enBufMgmtMode = HI_UNF_VI_BUF_MMAP;
#endif

    Ret = HI_UNF_VI_Create(HI_UNF_VI_PORT0, &stViAttr, &hVi);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_VI_Create failed\n");
        goto ERR4;
    }

#if  USER_MMAP_BUFFER
    u32Stride = (stViAttr.stInputRect.s32Width + 15) & 0xFFFFFFF0;
    stMmzBuf.bufsize = u32Stride * stViAttr.stInputRect.s32Height * 2 * stViAttr.u32BufNum;
    HI_MMZ_Malloc(&stMmzBuf);
    memset(stMmzBuf.user_viraddr, 0, stMmzBuf.bufsize);
    stBufAttr.u32BufNum = stViAttr.u32BufNum;
    for (i = 0; i < stViAttr.u32BufNum; i++)
    {
        stBufAttr.u32PhyAddr[i] = stMmzBuf.phyaddr + i * u32Stride
                                  * stViAttr.stInputRect.s32Height * 2;
    }

    Ret = HI_UNF_VI_SetExternBuffer(hVi, &stBufAttr);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_VI_Create failed\n");
        goto ERR5;
    }
    printf("---USER_MMAP_BUFFER---\n");

#endif

    Ret = HI_UNF_VO_AttachWindow(hWin, hVi);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_AttachWindow failed\n");
        goto ERR5;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto ERR6;
    }

    Ret = HI_UNF_VI_Start(hVi);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VI_Start failed.\n");
        goto ERR7;
    }

    printf("press any key to quit!\n");
    getchar();

    HI_UNF_VI_Stop(hVi);
ERR7:
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
ERR6:
    HI_UNF_VO_DetachWindow(hWin, hVi);
ERR5:
    HI_UNF_VI_Destroy(hVi);
#if USER_MMAP_BUFFER
    HI_MMZ_Free(&stMmzBuf);
#endif
ERR4:
    HI_UNF_VI_DeInit();
ERR3:
    HI_UNF_VO_DestroyWindow(hWin);
ERR2:
    HIADP_VO_DeInit();
ERR1:
    HIADP_Disp_DeInit();
ERR0:
    HI_ADP_HDMIRX_DeInit();
ERR_SYS_Deint:
    HI_SYS_DeInit();

    return 0;
}
