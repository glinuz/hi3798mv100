/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : sample_coverflow.c
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

/*********************************add include here******************************/

#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "hi_egl.h"
#include "hi_engine_key.h"
#include "hi_go.h"
#include "hi_coverflow.h"

/***************************** Macro Definition ******************************/

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize  *************/

unsigned long long hi_egl_get_time_ns( void )
{
    struct timeval tv;
    unsigned long long time_ns;

    gettimeofday(&tv, NULL);

    time_ns  = ((unsigned long long) tv.tv_sec) * 1000000000;
    time_ns += ((unsigned long long) tv.tv_usec) * 1000;
    return time_ns;
}

HI_S32 main(HI_S32 argc, HI_CHAR ** argv)
{
#ifdef FPS_RECORD
    unsigned long long time_save, time_record;
    int framescnt = 0;
#endif

    HI_S32 s32Ret;
    ENGINE_KEY_S pKey;
    HI_COVERFLOW_S sCoverFlow;

#ifdef ADVCA_SUPPORT
    HI_CHAR *pBackGround = "/home/stb/bin/gpu/res/back.png";
#else
    HI_CHAR *pBackGround = "../../res/png/back.png";
#endif

    memset(&sCoverFlow, 0, sizeof(HI_COVERFLOW_S));

#ifdef ADVCA_SUPPORT
    HI_CHAR *pImages[9] =
    {
        "/home/stb/bin/gpu/res/image1.png",
        "/home/stb/bin/gpu/res/image2.png",
        "/home/stb/bin/gpu/res/image3.png",
        "/home/stb/bin/gpu/res/image4.png",
        "/home/stb/bin/gpu/res/image5.png",
        "/home/stb/bin/gpu/res/image6.png",
        "/home/stb/bin/gpu/res/image7.png",
        "/home/stb/bin/gpu/res/image8.png",
        "/home/stb/bin/gpu/res/image9.png"
    };
#else
    HI_CHAR *pImages[9] =
    {
        "../../res/png/image1.png",
        "../../res/png/image2.png",
        "../../res/png/image3.png",
        "../../res/png/image4.png",
        "../../res/png/image5.png",
        "../../res/png/image6.png",
        "../../res/png/image7.png",
        "../../res/png/image8.png",
        "../../res/png/image9.png"
    };
#endif

    s32Ret = HI_COVERFLOW_InitGLES(DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("init gles failure \n");
        goto ShutDown;
        return HI_FAILURE;
    }

    HI_COVERFLOW_CreateCoverflow();

    s32Ret = HI_COVERFLOW_SetCoverflow(&sCoverFlow, \
                                       HI_TRUE, \
                                       30, \
                                       25.0f, \
                                       9, \
                                       pImages, \
                                       pBackGround );
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("set coverflow failure \n");
        goto ShutDown;
        return HI_FAILURE;
    }

    s32Ret = HI_ENGINE_KEY_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE(" FILE : %s == FUNCTION : %s == LINE : %d  >>> failure \n",
                   __FILE__, __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

#ifdef FPS_RECORD
    time_save = hi_egl_get_time_ns();
#endif
    while (ENGINE_KEY_BACK != pKey.keyCode)
    {
        s32Ret = HI_ENGINE_KEY_Get(&pKey);
        if (ENGINE_KEY_LEFT == pKey.keyCode)
        {
            sCoverFlow.eCoverStatus = COVER_TURN_LEFT;
        }
        else if (ENGINE_KEY_RIGHT == pKey.keyCode)
        {
            sCoverFlow.eCoverStatus = COVER_TURN_RIGHT;
        }

        HI_COVERFLOW_Render(&sCoverFlow);

#ifdef FPS_RECORD
        if (++framescnt == 100)
        {
            time_record = hi_egl_get_time_ns();
            fprintf(stderr, "fps is %f...\n", 1000.0 * 1000.0 * 1000.0 * 100 / (time_record - time_save));
            framescnt = 0;
            time_save = time_record;
        }
#endif
    }

ShutDown:

    s32Ret = HI_ENGINE_KEY_DeInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("deinit key failure \n");
        return HI_FAILURE;
    }

    HI_COVERFLOW_DestroyCoverflow(&sCoverFlow);

    return HI_SUCCESS;
}
