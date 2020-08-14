/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_pwm.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/10/28
  Description   :
  History       :
  1.Date        : 2013/10/28
    Author      : h00183514
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
#include "hi_unf_pwm.h"

#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"


static pthread_t                   g_stSendThread;
static HI_BOOL                     g_bThreadRun = HI_FALSE;

HI_VOID *SendThread(HI_VOID *pArgs)
{
    /*cycle send 10110100*/
    while(g_bThreadRun)
    {
        /*send 1*/
        HI_UNF_PWM_SendSignal(HI_UNF_PWM_0, 560, 1690);

        /*send 0*/
        HI_UNF_PWM_SendSignal(HI_UNF_PWM_0, 560, 560);

        /*send 1*/
        HI_UNF_PWM_SendSignal(HI_UNF_PWM_0, 560, 1690);

        /*send 1*/
        HI_UNF_PWM_SendSignal(HI_UNF_PWM_0, 560, 1690);   

        /*send 0*/
        HI_UNF_PWM_SendSignal(HI_UNF_PWM_0, 560, 560);

        /*send 1*/
        HI_UNF_PWM_SendSignal(HI_UNF_PWM_0, 560, 1690);

        /*send 0*/
        HI_UNF_PWM_SendSignal(HI_UNF_PWM_0, 560, 560);

        /*send 0*/
        HI_UNF_PWM_SendSignal(HI_UNF_PWM_0, 560, 560);        
    }

    return HI_NULL;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                  Ret;
    HI_UNF_PWM_ATTR_S       stAttr;
    HI_CHAR                 InputCmd[32];
    
    Ret = HI_UNF_PWM_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_PWM_Init ERR, Ret=%#x\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_PWM_Open(HI_UNF_PWM_0);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_PWM_Open ERR, Ret=%#x\n", Ret);
        HI_UNF_PWM_DeInit();
        return Ret;
    }

    Ret = HI_UNF_PWM_GetAttr(HI_UNF_PWM_0, &stAttr);
    stAttr.u32Freq = 38 * 1000;
    stAttr.u32DutyRatio = 333;
    Ret |= HI_UNF_PWM_SetAttr(HI_UNF_PWM_0, &stAttr);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_PWM_Open ERR, Ret=%#x\n", Ret);
        HI_UNF_PWM_Close(HI_UNF_PWM_0);
        HI_UNF_PWM_DeInit();
        return Ret;
    }

    g_bThreadRun = HI_TRUE;
    pthread_create(&g_stSendThread, HI_NULL, (HI_VOID *)SendThread, HI_NULL);

    while (1)
    {
        printf("please input q to quit!\n");

        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }        
    }

    g_bThreadRun = HI_FALSE;
    pthread_join(g_stSendThread, HI_NULL);     
    
    HI_UNF_PWM_Close(HI_UNF_PWM_0);

    HI_UNF_PWM_DeInit();    

    return 0;
}
