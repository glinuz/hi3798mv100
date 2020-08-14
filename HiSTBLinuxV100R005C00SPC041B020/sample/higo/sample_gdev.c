/**
\file
\brief example of config and fresh graphic layer
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author x57522
\date 2008-9-17
*/

#include <stdio.h>
#include <stdlib.h>
#include "hi_go.h"
#include "sample_displayInit.h"

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HIGO_LAYER_INFO_S stLayerInfo;
    HI_HANDLE hLayer,hSurface;
    HI_RECT stRect = {100, 100, 500, 500};
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;

    /** initial */
    ret = Display_Init();
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    
    ret = HI_GO_Init();
    if (HI_SUCCESS != ret)
    {
        goto ERR1;
    }

    /** create graphic layer */
    ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != ret)
    {
        goto ERR2;
    }
    ret = HI_GO_CreateLayer(&stLayerInfo,&hLayer);
    if (HI_SUCCESS != ret)
    {
        goto ERR2;
    }

    /** get the graphic layer Surface */
    ret = HI_GO_GetLayerSurface (hLayer,&hSurface); 
    if (HI_SUCCESS != ret)  
    {
       goto ERR3;
    }
    
    /** draw a rectange on the surface */
    ret = HI_GO_FillRect(hSurface,&stRect,0xFFFF0000,HIGO_COMPOPT_NONE);
    stRect.x += stRect.w;
    ret |= HI_GO_FillRect(hSurface,&stRect,0xFF00FF00,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
       goto ERR3; 
    }

    /** set ColorKey of the graphic layer surface */
    ret = HI_GO_SetSurfaceColorKey(hSurface,0xFFFF0000);
    if (HI_SUCCESS != ret)
    {
       goto ERR3;  
    }

    /**fresh the graphic layer */ 
    ret = HI_GO_RefreshLayer(hLayer, NULL);
    if (HI_SUCCESS != ret)
    {
        goto ERR3;
    }
    Printf("please input anykey and stop it \n");
    getchar();

ERR3:
    HI_GO_DestroyLayer(hLayer);
ERR2:
    HI_GO_Deinit();
ERR1:
    Display_DeInit();
    
    return ret;
}
