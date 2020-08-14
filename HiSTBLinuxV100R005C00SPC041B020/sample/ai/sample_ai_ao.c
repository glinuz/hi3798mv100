/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_ai_ao.c
  Version       : Initial Draft
  Author        : Hisilicon audio
  Created       : 2010/01/26
  Description   :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "hi_unf_common.h"
#include "hi_unf_ai.h"
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

HI_S32 main(HI_VOID)
{
    HI_S32 Ret;
    HI_HANDLE hAi;
    HI_UNF_AI_ATTR_S stAiAttr;
    HI_HANDLE hTrackSlave;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
    HI_CHAR InputCmd[32];

    HI_SYS_Init();

    Ret = HI_UNF_AI_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HI_UNF_AI_GetDefaultAttr(HI_UNF_AI_I2S0, &stAiAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_GetDefaultAttr failed\n");
        goto AI_DEINIT;
    }
    stAiAttr.u32PcmFrameMaxNum = 8;

    Ret = HI_UNF_AI_Create(HI_UNF_AI_I2S0, &stAiAttr, &hAi);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_GetDefaultAttr failed\n");
        goto AI_DEINIT;
    }

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed\n");
        goto AI_DESTROY;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto SND_DEINIT;
    }

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrackSlave);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto SND_DEINIT;
    }

    Ret = HI_UNF_SND_Attach(hTrackSlave, hAi);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_SLAVE_DESTROY;
    }

    Ret = HI_UNF_AI_SetEnable(hAi, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_SLAVE_DETACH;
    }

    while (1)
    {
        sample_printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }
    }

    HI_UNF_AI_SetEnable(hAi, HI_FALSE);

TRACK_SLAVE_DETACH:
    HI_UNF_SND_Detach(hTrackSlave, hAi);
TRACK_SLAVE_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrackSlave);
SND_DEINIT:
    HIADP_Snd_DeInit();
AI_DESTROY:
    HI_UNF_AI_Destroy(hAi);
AI_DEINIT:
    HI_UNF_AI_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();

    return 0;
}
