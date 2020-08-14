/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : sample_cylinder.c
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

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <GLES/gl.h>
#include <sys/time.h>

#include "hi_egl.h"
#include "hi_texture.h"
#include "hi_go.h"
#include "hi_cylinder.h"
#include "hi_engine_key.h"

/***************************** Macro Definition ******************************/

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

#define CYLINDER_MENU_WIDTH 400
#define CYLINDER_MENU_HEIGHT 300

#define   DEBUG_POS

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

#ifdef ADVCA_SUPPORT
static CYLNDER_IMAGE_S gs_CylinderImages[] =
{
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_01.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_02.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_03.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_04.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_05.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_06.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_07.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_08.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_09.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/3d_10.jpg", (HI_CHAR*)0, 0, {0, 0,
                                                                                                 CYLINDER_MENU_WIDTH,
                                                                                                 CYLINDER_MENU_HEIGHT}},
};
static CYLNDER_IMAGE_S gs_CylinderBackground = \
{(HI_CHAR*)0, (HI_CHAR*)"/home/stb/bin/gpu/res/photo_005.jpg", (HI_CHAR*)0, 0, {0, 0, DISPLAY_WIDTH,
                                                                                                 DISPLAY_HEIGHT}};

#else
static CYLNDER_IMAGE_S gs_CylinderImages[] =
{
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_01.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_02.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_03.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_04.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_05.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_06.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_07.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_08.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_09.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
    {(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/3d_10.jpg", (HI_CHAR*)0, 0, {0, 0, CYLINDER_MENU_WIDTH,
                                                                         CYLINDER_MENU_HEIGHT}},
};
static CYLNDER_IMAGE_S gs_CylinderBackground = \
{(HI_CHAR*)0, (HI_CHAR*)"../../res/jpg/photo_005.jpg", (HI_CHAR*)0, 0, {0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT}};
#endif

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize  *************/

/******************************* API declaration *****************************/

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

    HCYLINDER hCylinder;
    ENGINE_KEY_S pKey;

    HI_S32 s32Ret;

    hi_egl_setup(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 4, 0);

    hCylinder = HI_CYLINDER_Create(gs_CylinderImages, sizeof(gs_CylinderImages) / sizeof(gs_CylinderImages[0]),
                                   &gs_CylinderBackground, 0);
    if (NULL == hCylinder)
    {
        HI3D_TRACE("<%s> Line %d:Create cylinder failed!\n", __FUNCTION__, __LINE__);
        HI_GO_DeinitDecoder();
        hi_egl_destroy();
        return -1;
    }

    HI_CYLINDER_SetFrames(hCylinder, 5);
    HI_CYLINDER_SetMostStillSeconds(hCylinder, 100);
    HI_CYLINDER_SetReflection(hCylinder, CYLINDER_MENU_HEIGHT / 10.0, CYLINDER_MENU_HEIGHT * 2.0 / 3.0, 0.7);

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
        switch (pKey.keyCode)
        {
        case ENGINE_KEY_LEFT:
        {
            HI_CYLINDER_StarAction(hCylinder, CYLINDER_ACTION_TURNLEFT);
            break;
        }
        case ENGINE_KEY_RIGHT:
        {
            HI_CYLINDER_StarAction(hCylinder, CYLINDER_ACTION_TURNRIGHT);
            break;
        }
        case ENGINE_KEY_UP:
        {
            HI_CYLINDER_StarAction(hCylinder, CYLINDER_ACTION_TURNUP);
            HI_CYLINDER_AdjustViewAngle(hCylinder, 0.25);
            break;
        }
        case ENGINE_KEY_DOWN:
        {
            HI_CYLINDER_StarAction(hCylinder, CYLINDER_ACTION_TURNDOWN);
            HI_CYLINDER_AdjustViewAngle(hCylinder, -0.25);
            break;
        }
        case ENGINE_KEY_BUTT:
        {
            break;
        }
        default:
            break;
        }

        HI_CYLINDER_Draw(hCylinder);
        hi_egl_swap();
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

    HI_CYLINDER_Destroy(hCylinder);

    s32Ret = HI_ENGINE_KEY_DeInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("deinit key failure \n");
        return HI_FAILURE;
    }

    hi_egl_destroy();

    return 0;
}
