
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_unf_common.h"
#include "hi_unf_demux.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_filter.h"

#define PAT_TABLEID 0
#define EIT_TABLEID 0x4E
#define TS_PORT     DEFAULT_DVB_PORT

HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_ThirdParam;

HI_S32 DataCallBak(HI_S32 s32Filterid, HI_FILTER_CALLBACK_TYPE_E enCallbackType,
                   HI_UNF_DMX_DATA_TYPE_E eDataType, HI_U8 *pu8Buffer, HI_U32 u32BufferLength)
{
    if(HI_FILTER_CALLBACK_TYPE_TIMEOUT == enCallbackType)
    {
        printf(">>>>id:%d time out!\n",s32Filterid);
    }
    printf("Data Call Back Enter:");
    printf("id:%d,type:%x,datatype:%x,addr:%p,len:%d,\n",s32Filterid,enCallbackType,eDataType,pu8Buffer,u32BufferLength);
    #if 0
    if(u32BufferLength)
    {
        for(i=0;i<u32BufferLength;i++)
        {
            printf("%02x ",pu8Buffer[i]);
        }
    }
    printf("\n");
    #endif
    return 0;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])

{
    HI_S32 ret;
    HI_FILTER_ATTR_S stFilterAttr;
    HI_S32 s32FilterID[10];
    HI_U8 *pBuffer = HI_NULL;

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
        g_ThirdParam   = 64;
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

    ret = HI_UNF_DMX_Init();
    ret |= HI_UNF_DMX_AttachTSPort(0,TS_PORT);
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Demux_Init failed!\n");
        goto TUNER_DEINIT;
    }

    ret = HI_FILTER_Init();
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Init failed!\n");
        goto DMX_DEINIT;
    }
    stFilterAttr.u32DMXID = 0;
    stFilterAttr.u32FilterType = 0;//Section
    stFilterAttr.funCallback = DataCallBak;
    stFilterAttr.u32DirTransFlag = 1;//Transparent transmission mode
    stFilterAttr.u32PID = PAT_TSPID;
    stFilterAttr.u32CrcFlag = 0;
    stFilterAttr.u32TimeOutMs = 2000;
    memset(stFilterAttr.u8Mask,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Match,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Negate,0,DMX_FILTER_MAX_DEPTH);
    stFilterAttr.u8Match[0] = PAT_TABLEID;
    stFilterAttr.u32FilterDepth = 1;
    ret = HI_FILTER_Creat(&stFilterAttr,&s32FilterID[0]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Creat failed!\n");
        goto FILTER_DEINIT;
    }
    ret = HI_FILTER_Start(s32FilterID[0]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Start22 failed!\n");
        goto FILTER_DEINIT;
    }

    stFilterAttr.u32DMXID = 0;
    stFilterAttr.u32FilterType = 0;//Section
    stFilterAttr.funCallback = DataCallBak;
    stFilterAttr.u32DirTransFlag = 1;//Transparent transmission mode
    stFilterAttr.u32PID = EIT_TSPID;
    stFilterAttr.u32CrcFlag = 0;
    stFilterAttr.u32TimeOutMs = 2000;
    memset(stFilterAttr.u8Mask,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Match,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Negate,0,DMX_FILTER_MAX_DEPTH);
    stFilterAttr.u8Match[0] = EIT_TABLEID;
    stFilterAttr.u32FilterDepth = 1;
    ret = HI_FILTER_Creat(&stFilterAttr,&s32FilterID[1]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Creat failed!\n");
        goto DESTROY_FILTER0;
    }
    ret = HI_FILTER_Start(s32FilterID[1]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Start22 failed!\n");
        goto DESTROY_FILTER0;
    }

    stFilterAttr.u32DMXID = 0;
    stFilterAttr.u32FilterType = 1;//PES
    stFilterAttr.funCallback = DataCallBak;
    stFilterAttr.u32DirTransFlag = 1;//Transparent transmission mode
    stFilterAttr.u32PID = 0x202;
    stFilterAttr.u32CrcFlag = 0;
    stFilterAttr.u32TimeOutMs = 2000;
    memset(stFilterAttr.u8Mask,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Match,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Negate,0,DMX_FILTER_MAX_DEPTH);
    stFilterAttr.u32FilterDepth = 0;
    ret = HI_FILTER_Creat(&stFilterAttr,&s32FilterID[2]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Creat failed!\n");
        goto DESTROY_FILTER1;
    }
    ret = HI_FILTER_Start(s32FilterID[2]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Start22 failed!\n");
        goto DESTROY_FILTER1;
    }

    stFilterAttr.u32DMXID = 0;
    stFilterAttr.u32FilterType = 1;//PES
    stFilterAttr.funCallback = DataCallBak;
    stFilterAttr.u32DirTransFlag = 0;// Not transparent transmission mode
    pBuffer = malloc(64*1024);
    if(HI_NULL == pBuffer)
    {
       printf("malloc failed!\n");
       goto DESTROY_FILTER2;
    }
    stFilterAttr.pu8BufAddr = pBuffer;
    stFilterAttr.u32BufSize = 64*1024;
    stFilterAttr.u32PID = 0x2bc;
    stFilterAttr.u32CrcFlag = 0;
    stFilterAttr.u32TimeOutMs = 2000;
    memset(stFilterAttr.u8Mask,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Match,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Negate,0,DMX_FILTER_MAX_DEPTH);
    stFilterAttr.u32FilterDepth = 0;
    ret = HI_FILTER_Creat(&stFilterAttr,&s32FilterID[3]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Creat failed!\n");
        goto DESTROY_FILTER2;
    }
    ret = HI_FILTER_Start(s32FilterID[3]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Start22 failed!\n");
        goto DESTROY_FILTER2;
    }

    printf("press key to exit!\n");
    getchar();
    free(pBuffer);

    ret = HI_FILTER_Stop(s32FilterID[3]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Stop failed!\n");
        goto FILTER_DEINIT;
    }

    ret = HI_FILTER_Destroy(s32FilterID[3]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Destroy failed!\n");
        goto FILTER_DEINIT;
    }
DESTROY_FILTER2:
    ret = HI_FILTER_Stop(s32FilterID[2]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Stop failed!\n");
        goto FILTER_DEINIT;
    }

    ret = HI_FILTER_Destroy(s32FilterID[2]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Destroy failed!\n");
        goto FILTER_DEINIT;
    }
DESTROY_FILTER1:

    ret = HI_FILTER_Stop(s32FilterID[1]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Stop failed!\n");
        goto FILTER_DEINIT;
    }

    ret = HI_FILTER_Destroy(s32FilterID[1]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Destroy failed!\n");
        goto FILTER_DEINIT;
    }
DESTROY_FILTER0:

    ret = HI_FILTER_Stop(s32FilterID[0]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Stop failed!\n");
        goto FILTER_DEINIT;
    }

    ret = HI_FILTER_Destroy(s32FilterID[0]);
    if (HI_SUCCESS != ret)
    {
        printf("call HI_FILTER_Destroy failed!\n");
        goto FILTER_DEINIT;
    }

FILTER_DEINIT:
    HI_FILTER_DeInit();
DMX_DEINIT:
    HI_UNF_DMX_DeInit();
TUNER_DEINIT:
    HIADP_Tuner_DeInit();
    HI_SYS_DeInit();
    return ret;
}


