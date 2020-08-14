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

#ifdef HI_TEE_SUPPORT
#include "tee_client_api.h"
#endif

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define MAX_CODEC_NUM         (16)
#define DEFAULT_DISP_WIDTH    (1280)
#define DEFAULT_DISP_HEIGHT   (720)

static HI_U32 g_RectWinTable[MAX_CODEC_NUM][3] = 
{
    {1, 1, 1},  {2, 2, 1},  {3, 2, 2},  {4, 2, 2},
    {5, 3, 2},  {6, 3, 2},  {7, 3, 3},  {8, 3, 3},
    {9, 3, 3},  {10, 4, 3}, {11, 4, 3}, {12, 4, 3},
    {13, 4, 4}, {14, 4, 4}, {15, 4, 4}, {16, 4, 4},
};

typedef enum hiVIDEO_MODE_E
{
    VIDEO_MODE_STREAM = 0,     /*send by stream*/
    VIDEO_MODE_FRAME     ,     /*send by frame*/
    VIDEO_MODE_BUTT
}VIDEO_MODE_E;

typedef struct hiCODEC_PARAM_S
{
    HI_BOOL bVidPlay;
    HI_BOOL bTVPEnable;
    VIDEO_MODE_E eStreamMode;
    HI_BOOL bReadFrameSize;
    FILE   *pVidEsFile;
    pthread_t EsThread;
    HI_UNF_VCODEC_TYPE_E VdecType; 
    HI_BOOL bAdvancedProfile;
    HI_U32  CodecVersion;
    HI_HANDLE hAvplay;
    HI_HANDLE hWin;
}CODEC_PARAM_S;


CODEC_PARAM_S g_stCodec[MAX_CODEC_NUM];
#define MAX_FRAME_SIZE (0x100000)
HI_U32 ParseOneFrameSize(CODEC_PARAM_S *pCodec, HI_U8 *pu8Buf)
{
    HI_BOOL  bNewPic = HI_FALSE;
    HI_BOOL bFindStart, bFindEnd;
    HI_S32 i;
    HI_U32 u32Framesize = 0;
    HI_S32 s32ReadLen;
    HI_U32 s32UsedBytes = 0;
    FILE *fpStrm = pCodec->pVidEsFile;

    if ( HI_UNF_VCODEC_TYPE_HEVC == pCodec->VdecType)//hevc?D??o¡¥¨ºy,?¡ì3?o¨®D?¨¤??1D-¨°¨¦
    {
        bFindStart = HI_FALSE;  
        bFindEnd   = HI_FALSE;
        s32UsedBytes = ftell(fpStrm);

        s32ReadLen = fread(pu8Buf, 1, MAX_FRAME_SIZE, fpStrm);
        
        if (s32ReadLen == 0)
        { 

            u32Framesize = 0;
        }
        else
        {
            for (i=0; i<s32ReadLen-6; i++)
            {
                bNewPic = (pu8Buf[i+0] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1 
                    &&(((pu8Buf[i+3]&0x7D) >= 0x0 && (pu8Buf[i+3]&0x7D) <= 0x2A) || (pu8Buf[i+3]&0x1F) == 0x1) 
                    &&((pu8Buf[i+5]&0x80) == 0x80));//first slice segment
                    
                if (bNewPic)
                {
                    bFindStart = HI_TRUE;
                    i += 4;
                    break;
                }
            }

            for (; i<s32ReadLen-6; i++)
            {
                bNewPic = (pu8Buf[i+0] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1 
                    &&(((pu8Buf[i+3]&0x7D) >= 0x0 && (pu8Buf[i+3]&0x7D) <= 0x2A) || (pu8Buf[i+3]&0x1F) == 0x1) 
                    &&((pu8Buf[i+5]&0x80) == 0x80));//first slice segment
                    
                if (pu8Buf[i  ] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1  
                    &&(((pu8Buf[i+3]&0x7D) == 0x40) || ((pu8Buf[i+3]&0x7D) == 0x42) || ((pu8Buf[i+3]&0x7D) == 0x44)|| bNewPic)
                   )
                {
                    bFindEnd = HI_TRUE;
                    break;
                }
            }

            u32Framesize = i;

            //printf("$$$$$$$$$$$$$$$s32ReadLen 0x%x s32UsedBytes 0x%x------------\n", s32ReadLen, s32UsedBytes);
            if (bFindStart == HI_FALSE)
            {
                printf("hevc can not find start code! s32ReadLen 0x%x +++++++++++++\n", s32ReadLen);
            }
            else if (bFindEnd == HI_FALSE)
            {
                u32Framesize = i+6;
                //printf("hevc can not find end code!  s32ReadLen 0x%x ------------\n", s32ReadLen);
            }
        }
    }
    
    fseek(fpStrm, s32UsedBytes, SEEK_SET);
    return u32Framesize;
}

static HI_BOOL g_StopThread = HI_FALSE;
static HI_BOOL g_PlayOnce = HI_FALSE;

HI_VOID EsTthread(HI_VOID *args)
{
    HI_S32 Ret = HI_FAILURE;
    HI_HANDLE hAvplay;
    HI_U32 Readlen   = 0;
    HI_U32 FrameSize = 0;
    HI_BOOL bIsEmpty       = HI_FALSE;
    HI_BOOL bVidBufAvail   = HI_TRUE;
    HI_U8         *pBuffer = HI_NULL;
    CODEC_PARAM_S *pCodec  = HI_NULL;
    HI_UNF_STREAM_BUF_S              stStreamBuf;
    HI_UNF_AVPLAY_FLUSH_STREAM_OPT_S stFlushOpt;
    
#ifdef HI_TEE_SUPPORT
    HI_MMZ_BUF_S stCAStreamBuf;
    HI_UNF_AVPLAY_TVP_ATTR_S AvTVPAttr;
    memset(&stCAStreamBuf, 0, sizeof(HI_MMZ_BUF_S));
    memset(&AvTVPAttr, 0, sizeof(HI_UNF_AVPLAY_TVP_ATTR_S));
#endif

    pCodec  = (CODEC_PARAM_S *)args;
    hAvplay = pCodec->hAvplay;
    bVidBufAvail = HI_FALSE;

#ifdef HI_TEE_SUPPORT    
    HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_TVP, &AvTVPAttr);
#endif

    if ((pCodec->VdecType == HI_UNF_VCODEC_TYPE_HEVC) && (pCodec->eStreamMode == VIDEO_MODE_FRAME))
    {
        pCodec->bReadFrameSize = HI_TRUE;
    }

    while (!g_StopThread)
    {
        if (pCodec->bVidPlay)
        {
            if (HI_FALSE != pCodec->bReadFrameSize)
            {
                Ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, MAX_FRAME_SIZE, &stStreamBuf, 0);
            }
            else
            {
                Ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, 0x4000, &stStreamBuf, 0);
            }
            
            if (HI_SUCCESS == Ret)
            {
                bVidBufAvail = HI_TRUE;
                
                if (HI_FALSE != pCodec->bReadFrameSize)
                {
                    if (pCodec->VdecType == HI_UNF_VCODEC_TYPE_HEVC)
                    {
                         FrameSize = ParseOneFrameSize(pCodec, stStreamBuf.pu8Data);
                    }
                    else
                    {
                        Readlen = fread(&FrameSize, 1, 4, pCodec->pVidEsFile);
                        if (Readlen != 4)
                        {
                            FrameSize = 0x4000;
                        }
                    }
                }
                else
                {
                    FrameSize = 0x4000;
                }
                
#ifdef HI_TEE_SUPPORT
                if(HI_TRUE == AvTVPAttr.bEnable)
                {
                    if(0 == stCAStreamBuf.phyaddr)
                    {
                        strncpy(stCAStreamBuf.bufname, "CAStreamBuf", sizeof(stCAStreamBuf.bufname));
                        stCAStreamBuf.overflow_threshold  = 100;
                        stCAStreamBuf.underflow_threshold = 0;
                        if (HI_FALSE != pCodec->bReadFrameSize)
                        {
                            stCAStreamBuf.bufsize = MAX_FRAME_SIZE;
                        }
                        else
                        {
                            stCAStreamBuf.bufsize = 0x4000;
                        }

                        Ret = HI_MMZ_Malloc(&stCAStreamBuf);
                        if (HI_SUCCESS != Ret)
                        {
                            sample_printf("MMZ_Malloc CAStreamBuf %d failed!\n", stCAStreamBuf.bufsize);
                            break;
                        }
                    }
                    pBuffer = (HI_U8 *)stCAStreamBuf.user_viraddr;
                }
                else
#endif
                {
                    pBuffer = (HI_U8 *)stStreamBuf.pu8Data;
                }
                
                Readlen = fread(pBuffer, sizeof(HI_S8), FrameSize, pCodec->pVidEsFile);

#ifdef HI_TEE_SUPPORT
                if(AvTVPAttr.bEnable == HI_TRUE)
                {
                    HI_SEC_MMZ_CA2TA(stCAStreamBuf.phyaddr, (unsigned long)stStreamBuf.pu8Data, Readlen);
                }
#endif
                if (Readlen > 0)
                {
                    Ret = HI_UNF_AVPLAY_PutBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, Readlen, 0);
                    if (Ret != HI_SUCCESS)
                    {
                        sample_printf("call HI_UNF_AVPLAY_PutBuf failed.\n");
                    }

                    FrameSize = 0;
                }
                else if (Readlen == 0)
                {
                    if (HI_TRUE == g_PlayOnce)
                    {
                        sample_printf("Esplay flush stream.\n");
                        HI_UNF_AVPLAY_FlushStream(hAvplay, &stFlushOpt);
                        do 
                        {
                            Ret = HI_UNF_AVPLAY_IsBuffEmpty(hAvplay, &bIsEmpty);
                            if (Ret != HI_SUCCESS)
                            {
                                sample_printf("call HI_UNF_AVPLAY_IsBuffEmpty failed.\n");
                                break;
                            }
                        }while(bIsEmpty != HI_TRUE);
                        sleep(5);
                        sample_printf("Finish, Codec %d exit!\n", pCodec->hAvplay);
                        break;
                    }
                    else
                    {
                        sample_printf("Codec %d read vid file end and rewind!\n", pCodec->hAvplay);
                        rewind(pCodec->pVidEsFile);
                    }
                }
                else
                {
                    sample_printf("Codec %d read vid file error!\n", pCodec->hAvplay);
                }
            }
            else
            {
                bVidBufAvail = HI_FALSE;
            }
        }

        /* wait for buffer */
        if (HI_FALSE == bVidBufAvail)
        {
            usleep(1000 * 10);
        }
    }
    
#ifdef HI_TEE_SUPPORT
    if(AvTVPAttr.bEnable == HI_TRUE)
    {   
        HI_MMZ_Free(&stCAStreamBuf);
    }
#endif

    return;
}

HI_S32 GetCodecParam(HI_CHAR *argv[], CODEC_PARAM_S *pCodec, HI_U32 uNoPrint)
{
    if (!strcasecmp("mpeg2", argv[1]))
    {
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_MPEG2;
    }
    else if (!strcasecmp("mpeg4", argv[1]))
    {
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_MPEG4;
    }
    else if (!strcasecmp("h263", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_H263;
    }
    else if (!strcasecmp("sor", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_SORENSON;
    }
    else if (!strcasecmp("vp6", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_VP6;
    }
    else if (!strcasecmp("vp6f", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_VP6F;
    }
    else if (!strcasecmp("vp6a", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_VP6A;
    }
    else if (!strcasecmp("h264", argv[1]))
    {
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_H264;
    }
	else if (!strcasecmp("h265", argv[1]))
    {
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_HEVC;
    }	
    else if (!strcasecmp("mvc", argv[1]))
    {
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_MVC;
    }
    else if (!strcasecmp("avs", argv[1]))
    {
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_AVS;
    }
    else if (!strcasecmp("real8", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_REAL8;
    }
    else if (!strcasecmp("real9", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_REAL9;
    }
    else if (!strcasecmp("vc1ap", argv[1]))
    {
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_VC1;
        pCodec->bAdvancedProfile = 1;
        pCodec->CodecVersion     = 8;
    }
    else if (!strcasecmp("vc1smp5", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_VC1;
        pCodec->bAdvancedProfile = 0;
        pCodec->CodecVersion     = 5;  //WMV3
    }
    else if (!strcasecmp("vc1smp8", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_VC1;
        pCodec->bAdvancedProfile = 0;
        pCodec->CodecVersion     = 8;  //not WMV3
    }
    else if (!strcasecmp("vp8", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_VP8;
    }
    else if (!strcasecmp("divx3", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_DIVX3;
    }
    else if (!strcasecmp("mvc", argv[1]))
    {
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_MVC;
    }
    else if (!strcasecmp("mjpeg", argv[1]))
    {
        pCodec->bReadFrameSize = HI_TRUE;
        pCodec->VdecType = HI_UNF_VCODEC_TYPE_MJPEG;
    }
    else
    {
        if (uNoPrint == 0)
        {
            sample_printf("unsupport vid codec type!\n");
        }
        return HI_FAILURE;
    }

    pCodec->pVidEsFile = fopen(argv[0], "rb");
    if (HI_NULL == pCodec->pVidEsFile)
    {
        if (uNoPrint == 0)
        {
            sample_printf("open file %s error!\n", argv[1]);
        }
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID GetWinParam(HI_U32 TotalWinNum, HI_U32 Index, HI_RECT_S *pRectWin)
{
	HI_U32 AvrWidth, AvrHeight;
    HI_U32 AlignXNum, AlignYNum;

    AlignXNum   = g_RectWinTable[TotalWinNum-1][1];
    AlignYNum   = g_RectWinTable[TotalWinNum-1][2];

    AvrWidth  = DEFAULT_DISP_WIDTH/AlignXNum;
    AvrHeight = DEFAULT_DISP_HEIGHT/AlignYNum;

    pRectWin->s32X = (Index%AlignXNum)*AvrWidth;
    pRectWin->s32Y = (Index/AlignXNum)*AvrHeight;;
    pRectWin->s32Width  = AvrWidth;
    pRectWin->s32Height = AvrHeight;

    return;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32  Ret = HI_FAILURE;
    HI_S32  pos, index;
    HI_S32  CurCodecNum  = 0;
	HI_S32  s32FrameRate = 0;
    HI_CHAR InputCmd[32];
    HI_UNF_ENC_FMT_E enDefaultFmt = HI_UNF_ENC_FMT_720P_50;
    CODEC_PARAM_S   *pCodec       = HI_NULL;
    HI_RECT_S                     stRectWin;
    HI_UNF_AVPLAY_ATTR_S          stAvplayAttr;
    HI_UNF_SYNC_ATTR_S            stAvSyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S      stStopAttr;
    HI_UNF_VCODEC_ATTR_S          stVcodecAttr;
    HI_UNF_AVPLAY_FRMRATE_PARAM_S stFramerate;
    HI_UNF_AVPLAY_OPEN_OPT_S      stMaxCapbility;
#ifdef HI_TEE_SUPPORT
    HI_UNF_AVPLAY_TVP_ATTR_S      stAvTVPAttr;
#endif

    if (argc < 3)
    {
        printf(" usage: sample_rawplay vfile vtype -[options] \n");
        printf(" vtype: h264/h265/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sor\n");
        printf(" options:\n"
               " -once,     run one time only \n"
               " -fps 60,   run on on 60 fps \n"
               " 1080p_50 or 1080p_60  video output at 1080p_50 or 1080p_60\n");
#ifdef HI_TEE_SUPPORT
        printf(" -tvpon,   turn on tvp only the first channel run on tvp \n");
        printf(" -tvpoff,  turn off tvp \n");
#endif
        printf(" examples: \n");
        printf("   sample_rawplay vfile0  h264 vfile1 h265 ...\n");
        
        return HI_FAILURE;
    }

    memset(g_stCodec, 0, MAX_CODEC_NUM*sizeof(CODEC_PARAM_S));

    for (index = 0; index < MAX_CODEC_NUM; index++)
    {
        pos = 1+index*2;
        if (pos >= argc-1)
        {
            break;
        }
        
        Ret = GetCodecParam(&argv[pos], &g_stCodec[index], CurCodecNum);  
        if (Ret == HI_SUCCESS)
        {
            CurCodecNum++;
            g_stCodec[index].bVidPlay = HI_TRUE;
        }
        else if (0 == CurCodecNum)
        {
            sample_printf("Get Codec param failed!\n");
            return HI_FAILURE;
        }
        else
        {
            break;
        }
    }
    sample_printf("%d source to play.\n", CurCodecNum);

    for (index = 0; index < argc; index++)
    {
        if (strcasestr("1080p_50", argv[index]))
        {
            sample_printf("ESPlay use 1080p_50 output\n");
            enDefaultFmt = HI_UNF_ENC_FMT_1080P_50;
            break;
        }
        else if (strcasestr("1080p_60", argv[index]))
        {
            sample_printf("ESPlay use 1080p_60 output\n");
            enDefaultFmt = HI_UNF_ENC_FMT_1080P_60;
            break;
        }
        else if (strcasestr("-once", argv[index]))
        {
            sample_printf("Play once only.\n");
            g_PlayOnce = HI_TRUE;
        }
		else if (strcasestr("-fps", argv[index]))
		{
            s32FrameRate = atoi(argv[index+1]);
			if(s32FrameRate < 0)
			{
	            sample_printf("Invalid fps.\n");
				return HI_FAILURE;
			}
		}
        else if (strcasestr("stream", argv[index]))
        {
            for (pos = 0; pos < CurCodecNum; pos++)
            {
                g_stCodec[pos].eStreamMode = VIDEO_MODE_STREAM;
            }
        }
        else if (strcasestr("frame", argv[index]))
        {
            for (pos = 0; pos < CurCodecNum; pos++)
            {
                g_stCodec[pos].eStreamMode = VIDEO_MODE_FRAME;
            }
        }
#ifdef HI_TEE_SUPPORT
        else if (strcasestr("-tvpon", argv[index]))
        {
            for (pos = 0; pos < CurCodecNum; pos++)
            {
                g_stCodec[pos].bTVPEnable = HI_TRUE;
            }
        }
        else if (strcasestr("-tvpon1", argv[index]))
        {
            for (pos = 0; pos < 1; pos++)
            {
                g_stCodec[pos].bTVPEnable = HI_TRUE;
            }
        }
        else if (strcasestr("-tvpon2", argv[index]))
        {
            for (pos = 0; pos < 2; pos++)
            {
                g_stCodec[pos].bTVPEnable = HI_TRUE;
            }
        }
        else if (strcasestr("-tvpoff", argv[index]))
        {
        }
#endif
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

    for (index = 0; index < CurCodecNum; index++)
    {
        pCodec = &g_stCodec[index];
        if (pCodec->bVidPlay)
        {
            Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&stAvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
            stAvplayAttr.stStreamAttr.u32VidBufSize = 5*1024*1024;
            Ret |= HI_UNF_AVPLAY_Create(&stAvplayAttr, &pCodec->hAvplay);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
                goto VO_DEINIT;
            }
            
            Ret = HI_UNF_AVPLAY_GetAttr(pCodec->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stAvSyncAttr);
            stAvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
            Ret |= HI_UNF_AVPLAY_SetAttr(pCodec->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stAvSyncAttr);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
                goto AVPLAY_DESTROY;
            }
            
            GetWinParam(CurCodecNum, index, &stRectWin);
            Ret |= HIADP_VO_CreatWin(&stRectWin, &pCodec->hWin);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call VoInit failed.\n");
                HIADP_VO_DeInit();
                goto AVPLAY_DESTROY;
            }
            
            if (HI_UNF_VCODEC_TYPE_MVC == pCodec->VdecType)
            {
                stMaxCapbility.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
                stMaxCapbility.enDecType       = HI_UNF_VCODEC_DEC_TYPE_BUTT;
                stMaxCapbility.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_MVC;
            }
            else
            {
                stMaxCapbility.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_4096x2160;
                stMaxCapbility.enDecType       = HI_UNF_VCODEC_DEC_TYPE_BUTT;
                stMaxCapbility.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_BUTT;
            }
        
#ifdef HI_TEE_SUPPORT
            stAvTVPAttr.bEnable = pCodec->bTVPEnable;
            Ret = HI_UNF_AVPLAY_SetAttr(pCodec->hAvplay, HI_UNF_AVPLAY_ATTR_ID_TVP, &stAvTVPAttr);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_AVPLAY_SetAttr TVP failed.\n");
                goto VO_DESTROY;
            }
#endif
            Ret = HI_UNF_AVPLAY_ChnOpen(pCodec->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stMaxCapbility);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
                goto VO_DESTROY;
            }
        
            /*set compress attr*/
            Ret = HI_UNF_AVPLAY_GetAttr(pCodec->hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVcodecAttr);
        
            if (HI_UNF_VCODEC_TYPE_VC1 == pCodec->VdecType)
            {
                stVcodecAttr.unExtAttr.stVC1Attr.bAdvancedProfile = pCodec->bAdvancedProfile;
                stVcodecAttr.unExtAttr.stVC1Attr.u32CodecVersion  = pCodec->CodecVersion;
            }
            else if (HI_UNF_VCODEC_TYPE_VP6 == pCodec->VdecType)
            {
                stVcodecAttr.unExtAttr.stVP6Attr.bReversed = 0;
            }
        
            stVcodecAttr.enType = pCodec->VdecType;
         
            Ret |= HI_UNF_AVPLAY_SetAttr(pCodec->hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVcodecAttr);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
                goto VCHN_CLOSE;
            }
        
            Ret = HI_UNF_VO_AttachWindow(pCodec->hWin, pCodec->hAvplay);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_VO_AttachWindow failed.\n");
                goto VCHN_CLOSE;
            }
        
            Ret = HI_UNF_VO_SetWindowEnable(pCodec->hWin, HI_TRUE);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
                goto WIN_DETATCH;
            }
        
            if (0 != s32FrameRate)
            {
               stFramerate.enFrmRateType = HI_UNF_AVPLAY_FRMRATE_TYPE_USER;
               stFramerate.stSetFrmRate.u32fpsInteger = s32FrameRate;
               stFramerate.stSetFrmRate.u32fpsDecimal = 0;
               HI_UNF_AVPLAY_SetAttr(pCodec->hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM, &stFramerate);
                if (Ret != HI_SUCCESS)
                {
                    sample_printf("call HIADP_AVPlay_SetVdecAttr failed.\n");
                    goto WIN_DETATCH;
                }
            }
        
            Ret = HI_UNF_AVPLAY_Start(pCodec->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_AVPLAY_Start failed.\n");
                goto WIN_DETATCH;
            }
        }
    }
    
#ifdef HI_TEE_SUPPORT
    HI_SEC_MMZ_Init();
#endif

    g_StopThread = HI_FALSE;
    for (index = 0; index < CurCodecNum; index++)
    {
        pCodec = &g_stCodec[index];
        pthread_create(&pCodec->EsThread, HI_NULL, (HI_VOID *)EsTthread, (HI_VOID *)pCodec);
    }

    while (1)
    {
        printf("please input the q to quit!, s to toggle spdif pass-through, h to toggle hdmi pass-through\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }

        //FOR MVC TEST
        if ('3' == InputCmd[0] && 'd' == InputCmd[1]  )
        {
            HI_UNF_DISP_Set3DMode(HI_UNF_DISPLAY1,
            HI_UNF_DISP_3D_FRAME_PACKING,
            HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING);
        }
        
#ifdef HI_TEE_SUPPORT
        if ('t' == InputCmd[0] || 'T' == InputCmd[0])
        {
            if(stAvTVPAttr.bEnable == HI_TRUE)
            {
                sample_printf("call HI_UNF_AVPLAY_SetAttr TVP false.\n");
                stAvTVPAttr.bEnable = HI_FALSE;
            }
            else
            {
                sample_printf("call HI_UNF_AVPLAY_SetAttr TVP true.\n");
                stAvTVPAttr.bEnable = HI_TRUE;
            }

            stStopAttr.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
            stStopAttr.u32TimeoutMs = 0;
            HI_UNF_AVPLAY_Stop(g_stCodec[0].hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStopAttr);

            Ret = HI_UNF_AVPLAY_SetAttr(g_stCodec[0].hAvplay, HI_UNF_AVPLAY_ATTR_ID_TVP, &stAvTVPAttr);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_AVPLAY_SetAttr TVP failed.\n");
                break;
            }

            Ret = HI_UNF_AVPLAY_Start(g_stCodec[0].hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_AVPLAY_Start failed.\n");
                break;
            }
        }
#endif
    }

    g_StopThread = HI_TRUE;
    for (index = 0; index < CurCodecNum; index++)
    {
        pCodec = &g_stCodec[index];
        pthread_join(pCodec->EsThread, HI_NULL);
        stStopAttr.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        stStopAttr.u32TimeoutMs = 0;
        HI_UNF_AVPLAY_Stop(pCodec->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStopAttr);
    }
    
#ifdef HI_TEE_SUPPORT
    HI_SEC_MMZ_DeInit();
#endif

WIN_DETATCH:
    for (index = 0; index < CurCodecNum; index++)
    {
        pCodec = &g_stCodec[index];
        HI_UNF_VO_SetWindowEnable(pCodec->hWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(pCodec->hWin, pCodec->hAvplay);
    }

VCHN_CLOSE:
    for (index = 0; index < CurCodecNum; index++)
    {
        pCodec = &g_stCodec[index]; 
        HI_UNF_AVPLAY_ChnClose(pCodec->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    }

VO_DESTROY:
    for (index = 0; index < CurCodecNum; index++)
    {
        pCodec = &g_stCodec[index]; 
        HI_UNF_VO_DestroyWindow(pCodec->hWin);
    }
    
AVPLAY_DESTROY:
    for (index = 0; index < CurCodecNum; index++)
    {
        pCodec = &g_stCodec[index]; 
        HI_UNF_AVPLAY_Destroy(pCodec->hAvplay);
    }
    
VO_DEINIT:
    HIADP_VO_DeInit();
    
DISP_DEINIT:
    HIADP_Disp_DeInit();
    
AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();
    for (index = 0; index < CurCodecNum; index++)
    {
        pCodec = &g_stCodec[index]; 
        fclose(pCodec->pVidEsFile);
        pCodec->pVidEsFile = HI_NULL;
    }
    
    return HI_SUCCESS;
}


