/**
\file
\brief example of surface memory usage
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author x57522
\date 2008-9-17
*/

#include <stdio.h>
#include <string.h>
#include "hi_go.h"
#include "sample_displayInit.h"

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_S32 main(HI_S32 argc,HI_CHAR **argv)
{
    HI_S32 ret = HI_SUCCESS;
    HIGO_LAYER_INFO_S stLayerInfoHD;
    HI_HANDLE hLayerHD,hLayerSurfaceHD;

    HI_SYS_Init();
    
    /** format,displaysize,screensize pixelformat should be the same as logo picture */
    HIADP_Disp_Init(HI_UNF_ENC_FMT_1080i_50);

    /** initial */
    ret = HI_GO_Init();
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
        
    /** create graphic layer  */    
    //ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfoSD);
    stLayerInfoHD.CanvasWidth = 1280;
    stLayerInfoHD.CanvasHeight = 720;
    stLayerInfoHD.DisplayWidth = 1280;
    stLayerInfoHD.DisplayHeight = 720;
    stLayerInfoHD.ScreenWidth = 1920;
    stLayerInfoHD.ScreenHeight = 1080;
    stLayerInfoHD.PixelFormat = HIGO_PF_8888;
    stLayerInfoHD.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    stLayerInfoHD.AntiLevel = HIGO_LAYER_DEFLICKER_AUTO;
    stLayerInfoHD.LayerID = HIGO_LAYER_HD_0;
    
    ret = HI_GO_CreateLayer(&stLayerInfoHD,&hLayerHD);
    if (HI_SUCCESS != ret)
    {
        goto ERR1;
    }
       
    /** get the graphic layer Surface */
    ret = HI_GO_GetLayerSurface (hLayerHD,&hLayerSurfaceHD); 
    if (HI_SUCCESS != ret)  
    {
       goto ERR2;
    }
    
    /** draw a rectangle on the Surface*/
    ret = HI_GO_FillRect(hLayerSurfaceHD,NULL,0xFFFF0000,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
       goto ERR2;
    }

    ret = HI_GO_RefreshLayer(hLayerHD, NULL);
    if (HI_SUCCESS != ret)
    {
       goto ERR2;
    }
    Printf("HD, SD both show red at the same time, there is no black screen, no flicker, please input anykey and stop it\n");	
    getchar();
    
 ERR2:
    HI_GO_DestroyLayer(hLayerHD);
ERR1:
    HI_GO_Deinit();
    Display_DeInit();
    
    return ret;
}
