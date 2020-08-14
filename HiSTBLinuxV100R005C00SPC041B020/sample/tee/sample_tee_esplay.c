/******************************************************************************

  Copyright (C), 2014-2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : rawplay.c
  Version       : Initial Draft
  Description   : video raw data player, multi channel support
  History       :
  1.Date        : 2014/11/13
    Author      : y00226912
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
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_adp_mpi.h"
#include "hi_adp_hdmi.h"
#include "mpi_mmz.h"
#include "tee_client_api.h"

#define sample_printf  printf

#define TEEC_CMD_REE2TEE_MEM_COPY  0

static HI_BOOL g_StopEsThread = HI_FALSE;
static HI_BOOL g_bAdvancedProfile;
static HI_BOOL g_CodecVersion;

typedef struct
{
    FILE                 *VidFile;
    HI_BOOL               bReadFrameSize;
    HI_HANDLE             hAvplay;
    HI_UNF_VCODEC_TYPE_E  VidType;
}ES_THREAD_PARAM_S;

HI_S32 GetFrameSizeByVidType(HI_U32 *pFrameSize, HI_BOOL bReadFrameSize, HI_UNF_VCODEC_TYPE_E VidType, FILE *fp)
{
    HI_U32         Readlen;
    HI_U32         FrameSize = 0;
    HI_U8          Vp9FileHdr[32];
    HI_U8          Vp9FrmHdr[12];
    static HI_BOOL Vp9ParseFileHdr = HI_FALSE;
    const HI_CHAR  IVFSignature[] = "DKIF";

    switch (VidType)
    {
        case HI_UNF_VCODEC_TYPE_VP9 :
        {
            if(!Vp9ParseFileHdr)
            {
                if (fread(Vp9FileHdr, 1, 32, fp) != 32)
                {
                    sample_printf("ERR: read VP9 file header failed.\n");
                    break;
                }

                if (memcmp(IVFSignature, Vp9FileHdr, 4) != 0)
                {
                    sample_printf("ERR: VP9 file is not IVF file.\n");
                    break;
                }

                Vp9ParseFileHdr = HI_TRUE;
            }

            if (fread(Vp9FrmHdr, 1, 12, fp) == 12)
            {
                FrameSize = (Vp9FrmHdr[3] << 24) + (Vp9FrmHdr[2] << 16) + (Vp9FrmHdr[1] << 8) + Vp9FrmHdr[0];
            }
            else /* read vp9 file end*/
            {
                FrameSize = 0;
                Vp9ParseFileHdr = HI_FALSE;
                *pFrameSize = FrameSize;
                return HI_SUCCESS;
            }

            break;
        }

        default :
        {
            if (bReadFrameSize)
            {
                Readlen = fread(&FrameSize, 1, 4, fp);
                FrameSize = (Readlen == 4) ? FrameSize : 0x4000;
            }
            else
            {
                FrameSize = 0x4000;
            }

            break;
        }
    }

    *pFrameSize = FrameSize;

    return FrameSize == 0 ? HI_FAILURE : HI_SUCCESS;
}


HI_S32 GetCodecParam(HI_CHAR *Ptr, HI_UNF_VCODEC_TYPE_E *VdecType, HI_BOOL *bReadFrameSize)
{
    *bReadFrameSize = HI_FALSE;

    if (!strcasecmp("mpeg2", Ptr))
    {
        *VdecType = HI_UNF_VCODEC_TYPE_MPEG2;
    }
    else if (!strcasecmp("mpeg4", Ptr))
    {
        *VdecType = HI_UNF_VCODEC_TYPE_MPEG4;
    }
    else if (!strcasecmp("h263", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_H263;
    }
    else if (!strcasecmp("sor", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_SORENSON;
    }
    else if (!strcasecmp("vp6", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_VP6;
    }
    else if (!strcasecmp("vp6f", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_VP6F;
    }
    else if (!strcasecmp("vp6a", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_VP6A;
    }
    else if (!strcasecmp("h264", Ptr))
    {
        *VdecType = HI_UNF_VCODEC_TYPE_H264;
    }
    else if (!strcasecmp("h265", Ptr))
    {
        *VdecType = HI_UNF_VCODEC_TYPE_HEVC;
    }
    else if (!strcasecmp("mvc", Ptr))
    {
        *VdecType = HI_UNF_VCODEC_TYPE_MVC;
    }
    else if (!strcasecmp("avs", Ptr))
    {
        *VdecType = HI_UNF_VCODEC_TYPE_AVS;
    }
    else if (!strcasecmp("real8", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_REAL8;
    }
    else if (!strcasecmp("real9", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_REAL9;
    }
    else if (!strcasecmp("vc1ap", Ptr))
    {
        *VdecType = HI_UNF_VCODEC_TYPE_VC1;
        g_bAdvancedProfile = 1;
        g_CodecVersion     = 8;
    }
    else if (!strcasecmp("vc1smp5", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_VC1;
        g_bAdvancedProfile = 0;
        g_CodecVersion     = 5;
    }
    else if (!strcasecmp("vc1smp8", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_VC1;
        g_bAdvancedProfile = 0;
        g_CodecVersion     = 8;
    }
    else if (!strcasecmp("vp8", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_VP8;
    }
    else if (!strcasecmp("vp9", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_VP9;
    }
    else if (!strcasecmp("divx3", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_DIVX3;
    }
    else if (!strcasecmp("mvc", Ptr))
    {
        *VdecType = HI_UNF_VCODEC_TYPE_MVC;
    }
    else if (!strcasecmp("mjpeg", Ptr))
    {
        *bReadFrameSize = HI_TRUE;
        *VdecType = HI_UNF_VCODEC_TYPE_MJPEG;
    }
    else
    {
        sample_printf("unsupport vid codec type!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID EsTthread(HI_VOID *args)
{
    HI_S32                   Ret = HI_FAILURE;
    HI_U32                   EsBufSize = 0;
    HI_U32                   Readlen   = 0;
    HI_U32                   FrameSize = 0;
    HI_U32                   SmmuAddr = 0;
    HI_VOID                 *pSmmuUserAddr = HI_NULL;
    HI_UNF_STREAM_BUF_S      StreamBuf;
    HI_HANDLE                hAvplay;
    HI_UNF_VCODEC_TYPE_E     VdecType;
    FILE                    *VidFile;
    ES_THREAD_PARAM_S       *pEsThreadParam;
    HI_BOOL                  bReadFrameSize;
    HI_BOOL                  bTaInit = HI_FALSE;
    TEEC_Context             teeContext;
    TEEC_Session             teeSession;
    TEEC_Operation           operation;
    TEEC_UUID                svc_id = {0x99985ef1, 0xd62d, 0x4524,
                             {0x9d, 0xd1, 0xd9, 0x83, 0x45, 0x48, 0xd9, 0x8e}
                             }; /*AVPLAY TA UUID*/

    if (TEEC_SUCCESS != TEEC_InitializeContext(HI_NULL, &teeContext))
    {
        sample_printf("ERR: [%s %d] TEEC_InitializeContext Failed!\n", __FUNCTION__, __LINE__);
        exit(0);
    }

    memset(&operation, 0x0, sizeof(TEEC_Operation));
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_MEMREF_TEMP_INPUT
                                            );

    if (TEEC_SUCCESS != TEEC_OpenSession(&teeContext,
                                         &teeSession,
                                         &svc_id,
                                         TEEC_LOGIN_IDENTIFY,
                                         HI_NULL, &operation,
                                         HI_NULL)
                                         )
    {
        sample_printf("ERR: [%s %d] TEEC_OpenSession Failed!\n", __FUNCTION__, __LINE__);
        exit(0);
    }

    bTaInit = HI_TRUE;

    pEsThreadParam  = (ES_THREAD_PARAM_S*)args;

    hAvplay        = pEsThreadParam->hAvplay;
    VidFile        = pEsThreadParam->VidFile;
    bReadFrameSize = pEsThreadParam->bReadFrameSize;
    VdecType       = pEsThreadParam->VidType;

    while (!g_StopEsThread)
    {
        EsBufSize = bReadFrameSize ? 0x200000 : 0x4000;
        Ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, EsBufSize, &StreamBuf, 0);
        if (HI_SUCCESS == Ret)
        {
            Ret = GetFrameSizeByVidType(&FrameSize, bReadFrameSize, VdecType, VidFile);
            if (HI_SUCCESS != Ret)
            {
                break;
            }

            if(!SmmuAddr)
            {
                SmmuAddr = HI_MPI_SMMU_New("ReeStreamBuf", EsBufSize);
                if (!SmmuAddr)
                {
                    sample_printf("ERR: [%s %d] HI_MPI_SMMU_New\n", __FUNCTION__, __LINE__);
                    break;
                }

                pSmmuUserAddr = HI_MPI_SMMU_Map(SmmuAddr, HI_FALSE);
                if (!pSmmuUserAddr)
                {
                    sample_printf("ERR: [%s %d] HI_MPI_SMMU_Map\n", __FUNCTION__, __LINE__);
                    break;
                }
            }

            Readlen = fread((HI_U8 *)pSmmuUserAddr, sizeof(HI_U8), FrameSize, VidFile);
            if (Readlen > 0)
            {
                operation.started = 1;
                operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
                operation.params[0].value.a = (HI_U32)SmmuAddr;
                operation.params[0].value.b = (HI_U32)StreamBuf.pu8Data;
                operation.params[1].value.a = Readlen;
                operation.params[1].value.b = TEEC_VALUE_UNDEF;

                if (TEEC_SUCCESS != TEEC_InvokeCommand(&teeSession, TEEC_CMD_REE2TEE_MEM_COPY, &operation, HI_NULL))
                {
                    sample_printf("ERR: [%s %d] TEEC_InvokeCommand Failed!\n", __FUNCTION__, __LINE__);
                    exit(0);
                }

                Ret = HI_UNF_AVPLAY_PutBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, Readlen, 0);
                if (Ret != HI_SUCCESS)
                {
                    sample_printf("ERR: HI_UNF_AVPLAY_PutBuf failed 0x%x\n", Ret);
                }

                FrameSize = 0;
            }
            else if (Readlen == 0)
            {

                sample_printf("read vid file end and rewind!\n");
                rewind(VidFile);
            }
        }
        else
        {
            /* wait for buffer */
            usleep(1000 * 10);
        }
    }

    if (SmmuAddr)
    {
        HI_MPI_SMMU_Unmap(SmmuAddr);
        HI_MPI_SMMU_Delete(SmmuAddr);
    }

    if (HI_TRUE == bTaInit)
    {
        TEEC_CloseSession(&teeSession);
        TEEC_FinalizeContext(&teeContext);
    }

    return;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32                        Ret = HI_FAILURE;
    HI_CHAR                       InputCmd[32];
    HI_UNF_ENC_FMT_E              enDefaultFmt = HI_UNF_ENC_FMT_720P_50;
    HI_UNF_AVPLAY_ATTR_S          stAvplayAttr;
    HI_UNF_SYNC_ATTR_S            stAvSyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S      stStopAttr;
    HI_UNF_VCODEC_ATTR_S          stVcodecAttr;
    HI_UNF_AVPLAY_OPEN_OPT_S      stMaxCapbility;
    pthread_t                     EsThread;
    HI_UNF_VCODEC_TYPE_E          VdecType;
    FILE                          *pVidEsFile;
    HI_BOOL                       bReadFrameSize;
    ES_THREAD_PARAM_S             EsThreadParam;
    HI_HANDLE                     hAvplay;
    HI_HANDLE                     hWin;
    HI_RECT_S                     stRectWin;
    HI_UNF_AVPLAY_TVP_ATTR_S      stAvTVPAttr;

    if (argc < 3)
    {
        printf(" usage: sample_tee_esplay vfile vtype\n");
        printf(" vtype: h264/h265/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sor\n");
        printf(" options:\n"
               " 1080p_50 or 1080p_60  video output at 1080p_50 or 1080p_60\n");

        printf(" examples: \n");
        printf("   sample_tee_esplay vfile h264 1080p_60\n");

        return HI_FAILURE;
    }

    if (argc > 3)
    {
        enDefaultFmt = HIADP_Disp_StrToFmt(argv[3]);
    }

    Ret = GetCodecParam(argv[2], &VdecType, &bReadFrameSize);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    pVidEsFile = fopen(argv[1], "rb");
    if (HI_NULL == pVidEsFile)
    {
        sample_printf("open file %s error!\n", argv[1]);
        return HI_FAILURE;
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(enDefaultFmt);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call DispInit failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call DispInit failed.\n");
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&stAvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
    Ret |= HI_UNF_AVPLAY_Create(&stAvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto VO_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stAvSyncAttr);
    stAvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stAvSyncAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto AVPLAY_DESTROY;
    }

    memset(&stRectWin, 0, sizeof(stRectWin));
    Ret |= HIADP_VO_CreatWin(&stRectWin, &hWin);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call VoInit failed.\n");
        HIADP_VO_DeInit();
        goto AVPLAY_DESTROY;
    }

    if (HI_UNF_VCODEC_TYPE_MVC == VdecType)
    {
        stMaxCapbility.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
        stMaxCapbility.enDecType       = HI_UNF_VCODEC_DEC_TYPE_BUTT;
        stMaxCapbility.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_MVC;
    }
    else
    {
        stMaxCapbility.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_4096x2160;
        stMaxCapbility.enDecType       = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
        stMaxCapbility.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;
    }

    stAvTVPAttr.bEnable = HI_TRUE;
    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_TVP, &stAvTVPAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr TVP failed.\n");
        goto VO_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stMaxCapbility);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto VO_DESTROY;
    }

    /*set compress attr*/
    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVcodecAttr);

    if (HI_UNF_VCODEC_TYPE_VC1 == VdecType)
    {
        stVcodecAttr.unExtAttr.stVC1Attr.bAdvancedProfile = g_bAdvancedProfile;
        stVcodecAttr.unExtAttr.stVC1Attr.u32CodecVersion  = g_CodecVersion;
    }
    else if (HI_UNF_VCODEC_TYPE_VP6 == VdecType)
    {
        stVcodecAttr.unExtAttr.stVP6Attr.bReversed = 0;
    }

    stVcodecAttr.enType = VdecType;
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVcodecAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto VCHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_VO_AttachWindow failed.\n");
        goto VCHN_CLOSE;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETATCH;
    }

    Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Start failed.\n");
        goto WIN_DETATCH;
    }

    g_StopEsThread = HI_FALSE;

    EsThreadParam.hAvplay        = hAvplay;
    EsThreadParam.VidFile        = pVidEsFile;
    EsThreadParam.bReadFrameSize = bReadFrameSize;
    EsThreadParam.VidType        = VdecType;
    pthread_create(&EsThread, HI_NULL, (HI_VOID *)EsTthread, (HI_VOID *)&EsThreadParam);

    while (1)
    {
        printf("please input the q to quit!, s to toggle spdif pass-through, h to toggle hdmi pass-through\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }
    }

    g_StopEsThread = HI_TRUE;
    pthread_join(EsThread, HI_NULL);

    stStopAttr.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    stStopAttr.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStopAttr);

WIN_DETATCH:
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

VCHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

VO_DESTROY:
    HI_UNF_VO_DestroyWindow(hWin);

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(hAvplay);

VO_DEINIT:
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

SYS_DEINIT:
    fclose(pVidEsFile);
    pVidEsFile = HI_NULL;

    return HI_SUCCESS;
}


