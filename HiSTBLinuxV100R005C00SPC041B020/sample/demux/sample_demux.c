
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_unf_common.h"
#include "hi_unf_demux.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"

#define PAT_TABLEID 0
#define TUNER_PORT  0
#define TS_PORT     DEFAULT_DVB_PORT
#define READ_DATA_LEN 188*10

HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_ThirdParam;
static HI_U8 g_match[DMX_FILTER_MAX_DEPTH];
static HI_U8 g_mask[DMX_FILTER_MAX_DEPTH];
static HI_U8 g_Negate[DMX_FILTER_MAX_DEPTH];

HI_S32 SectionRecvFunc(HI_HANDLE Chandle)
{
     HI_U32 ret;
     HI_U32 i,j,k;
     HI_U32 u32AcquireNum = 10;
     HI_U32 pu32AcquiredNum;
     HI_UNF_DMX_DATA_S pstBuf[10];

     printf("receive start!\n");
     for(k = 0;k < 10;k++)
     {
        if(HI_SUCCESS != HI_UNF_DMX_AcquireBuf(Chandle,u32AcquireNum,&pu32AcquiredNum,pstBuf,5000))
        {
            printf("call HI_UNF_DMX_AcquireBuf failed!\n");
            usleep(10 * 1000);
            continue;
        }

        for(i=0;i<pu32AcquiredNum;i++)
        {
            for(j=0;j<pstBuf[i].u32Size;j++)
            {
                printf("%02x ",pstBuf[i].pu8Data[j]);
            }
            printf("\n");
        }

        ret = HI_UNF_DMX_ReleaseBuf(Chandle,pu32AcquiredNum,pstBuf);
        if (HI_SUCCESS != ret)
        {
            printf("call HI_UNF_DMX_ReleaseBuf failed!\n");
        }
     }
     printf("receive over!\n");
     return HI_SUCCESS;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])

{
    HI_S32 ret;
    HI_UNF_DMX_CHAN_ATTR_S stChnAttr;
    HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
    HI_HANDLE hChannel;
    HI_HANDLE hFilter1;
    //HI_UNF_TUNER_STATUS_S  pstTunerStatus;

    if (4 == argc)
    {
        g_TunerFreq  = strtol(argv[1],NULL,0);
        g_TunerSrate = strtol(argv[2],NULL,0);
        g_ThirdParam = strtol(argv[3],NULL,0);
    }
    else if(3 == argc)
    {
        g_TunerFreq  = strtol(argv[1],NULL,0);
        g_TunerSrate = strtol(argv[2],NULL,0);
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
    }
    else if(2 == argc)
    {
        g_TunerFreq  = strtol(argv[1],NULL,0);
        g_TunerSrate = (g_TunerFreq>1000) ? 27500 : 6875;
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
    }
    else
    {
        printf("Usage: %s freq [srate] [qamtype or polarization] [vo_format]\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
                "       vo_format:1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50  default HI_UNF_ENC_FMT_1080i_50\n",
                argv[0]);
        printf("now use default setting: 610 6875 64\n");

        g_TunerFreq  = 610;
        g_TunerSrate = 6875;
        g_ThirdParam = 64;
    }


    HI_SYS_Init();
    HIADP_MCE_Exit();
    ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Demux_Init failed.\n");
        goto TUNER_DEINIT;
    }

    ret = HIADP_Tuner_Connect(TUNER_USE,g_TunerFreq,g_TunerSrate,g_ThirdParam);
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    ret |= HI_UNF_DMX_Init();
    ret |= HI_UNF_DMX_AttachTSPort(0,TS_PORT);
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Demux_Init failed!\n");
        goto TUNER_DEINIT;
    }

    ret = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_GetChannelDefaultAttr failed!\n");
        goto DMX_DEINIT;
    }
    stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
    stChnAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    stChnAttr.u32BufSize = 16 * 1024;
    ret = HI_UNF_DMX_CreateChannel(0,&stChnAttr,&hChannel);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_CreateChannel failed!\n");
        goto DMX_DEINIT;
    }

    /* set filter attr */
    memset(&g_mask,0,DMX_FILTER_MAX_DEPTH);
    memset(&g_match,0,DMX_FILTER_MAX_DEPTH);
    memset(&g_Negate,0,DMX_FILTER_MAX_DEPTH);
    stFilterAttr.u32FilterDepth = 1;
    g_match[0] = PAT_TABLEID;
    memcpy(stFilterAttr.au8Mask,g_mask,DMX_FILTER_MAX_DEPTH);
    memcpy(stFilterAttr.au8Match,g_match,DMX_FILTER_MAX_DEPTH);
    memcpy(stFilterAttr.au8Negate,g_Negate,DMX_FILTER_MAX_DEPTH);
    ret = HI_UNF_DMX_CreateFilter(0,&stFilterAttr,&hFilter1);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_CreateFilter failed!\n");
        goto CHN_DESTROY;
    }
    ret = HI_UNF_DMX_AttachFilter(hFilter1,hChannel);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_AttachFilter failed!\n");
        HI_UNF_DMX_DestroyFilter(hFilter1);
        goto CHN_DESTROY;
    }

    ret = HI_UNF_DMX_SetChannelPID(hChannel,PAT_TSPID);
    ret |= HI_UNF_DMX_OpenChannel(hChannel);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_OpenChannel failed!\n");
        goto FILTER1_DESTROY;
    }
    /* start to recv section data, and meanwhile print received section */
    SectionRecvFunc(hChannel);

    ret = HI_UNF_DMX_CloseChannel(hChannel);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_UNF_DMX_CloseChannel failed!\n");
    }

FILTER1_DESTROY:
    HI_UNF_DMX_DetachFilter(hFilter1,hChannel);
    HI_UNF_DMX_DestroyFilter(hFilter1);
CHN_DESTROY:
    HI_UNF_DMX_DestroyChannel(hChannel);
DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(0);
    HI_UNF_DMX_DeInit();
TUNER_DEINIT:
    HIADP_Tuner_DeInit();
    HI_SYS_DeInit();
    return ret;
}


