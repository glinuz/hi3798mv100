/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: sample_dolby_main.c
 * Description:
 *       this sample finish the dolby authentication.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2011-07-11   184737
 ******************************************************************************/
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "hi_common.h"
#include "hi_type.h"
#include "hi_unf_common.h"
#include "hi_go_comm.h"
#include "hi_adp_mpi.h"
#include "dolby_ir.h"
#include "dolby_dvbplay.h"
#include "dolby_win.h"
#include "dolby_common.h"
#include "hi_go.h"

extern HI_BOOL        g_bDrawChnBar;
static HI_HANDLE      hAvplay = 0;
static HI_VOID*       g_pWindHandle = 0;

/*dvbplay program table*/
PMT_COMPACT_TBL *g_pstProgTbl = HI_NULL;

pthread_mutex_t g_Mutex;

static HI_VOID HI_DOLBY_TsChngTask(HI_VOID)
{
    while(1)
    {
        //printf("HI_DOLBY_TsChngTask start !\n");
        Hi_Dolby_Debug();
        HI_DOLBY_DetectTsChng(0, &hAvplay,g_pWindHandle);
        usleep(100 * 2000);
        Hi_Dolby_Debug();
    }

    //printf("HI_DOLBY_TsChngTask over !\n");
}

/* dolby main function */
HI_S32 main(HI_VOID)

{
    HI_S32         s32Ret = 0;
    POSITIONXY_S   stPos = {0};
    CHNBAR_S       stChnbar;
    HI_BOOL        bUpdteChanBar = HI_FALSE;
    HI_U32         u32PressStatus = 0, u32KeyId = 0;
    HI_UNF_SND_GAIN_ATTR_S stGain;
    pthread_t      tid;
    //struct timeval stStart = {0, 0};
    //struct timeval stEnd = {0, 0};

    pthread_mutex_init(&g_Mutex, NULL);

    HIADP_MCE_Exit();

    /*OSD Init*/
    s32Ret = HI_DOLBY_OSDInit();
    if (HI_SUCCESS != s32Ret)
    {
        printf("call DOLBY_OSDInit faild!\n");
        return HI_FAILURE;
    }
    s32Ret = HI_DOLBY_IR_Open();
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HI_DOLBY_IR_Open faild!\n");
        return HI_FAILURE;
    }

    /* draw the common layer and surface*/
    s32Ret = HI_DOLBY_CreatLayer();
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HI_DOLBY_CreatLayer faild!\n");
        return HI_FAILURE;
    }

    stPos.u32x = 0;
    stPos.u32y = 0;
    g_pWindHandle  = HI_DOLBY_CreateSurface(&stPos, LAYER_WIDTH, LAYER_HEIGHT, APP_WIND_NOTES);
    if(HI_NULL == g_pWindHandle)
    {
        printf("call HI_DOLBY_CreateSurface faild!\n");
        return HI_FAILURE;
    }

    /* draw programe serach window and then start dvbplay*/
    do
    {
        Hi_Dolby_Debug();
        s32Ret = HI_DOLBY_ProcProgSearchAndPlay(g_pWindHandle, &hAvplay, &g_pstProgTbl);
        if (HI_SUCCESS != s32Ret)
        {
            Hi_Dolby_Debug();
            HI_DOLBY_ClearPrgSearch_Wind(g_pWindHandle);
            HI_DOLBY_DrawSearchMesg(g_pWindHandle);
            HI_DOLBY_ClearSearchMesg(g_pWindHandle);
        }
    } while(HI_SUCCESS != s32Ret);
    Hi_Dolby_Debug();
    /*diaplay channel bar*/
    stChnbar.enTrackMode = (HI_UNF_TRACK_MODE_E)0;
    stChnbar.u32Volume = VOL_NUM;
    stGain.bLinearMode = HI_TRUE;
    stGain.s32Gain = VOL_NUM;
    HI_UNF_SND_SetVolume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_ALL,&stGain);
    HI_UNF_SND_SetMute(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_ALL, HI_FALSE);

    s32Ret = HI_DOLBY_DispChnBarWin(g_pWindHandle, &stChnbar);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HI_DOLBY_DrawChannelBar failed.\n");
        return HI_FAILURE;
    }

    pthread_create(&tid , NULL, (HI_VOID *)HI_DOLBY_TsChngTask, NULL);

    /*key process*/
    while(1)
    {

        /*check callback event*/
        if(g_bDrawChnBar)
        {
            bUpdteChanBar = HI_TRUE;
            g_bDrawChnBar = HI_FALSE;
        }

        /* check IR event*/
        s32Ret = HI_DOLBY_IR_GetValue(&u32PressStatus, &u32KeyId);
        if (HI_SUCCESS == s32Ret)
        {
            pthread_mutex_lock(&g_Mutex);
            HI_DOLBY_DvbKeyEvent(g_pWindHandle, u32KeyId, &hAvplay, &stChnbar, &bUpdteChanBar);
            pthread_mutex_unlock(&g_Mutex);
        }

        /*update channel bar*/
        if(bUpdteChanBar)
        {
            HI_DOLBY_DispChnBarWin(g_pWindHandle, &stChnbar);
            bUpdteChanBar = HI_FALSE;
        }

        usleep(100 * 1000);
    }

    pthread_cancel(tid);
    HI_DOLBY_DvbPlayDeinit(&hAvplay);
    HI_DOLBY_OSDDeinit();
    HI_DOLBY_ClearPrgSearch_Wind(g_pWindHandle);
}
