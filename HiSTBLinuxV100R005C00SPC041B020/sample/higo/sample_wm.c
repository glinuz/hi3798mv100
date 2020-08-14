/**
\file
\brief  exmaple of GIF decoding and display
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author x57522
\date 2008-9-17
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hi_go.h"
#include "sample_displayInit.h"

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hLayer;
    HIGO_WNDINFO_S WinInfo;
    HI_HANDLE hWindow;
    HI_S32 sWidth = 500, sHeight=400;
    HI_HANDLE hWinSurface;
    HIGO_LAYER_INFO_S LayerInfo;
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;
          
    /** initial*/
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
    
    /** get the HD layer's default parameters*/
    ret = HI_GO_GetLayerDefaultParam(eLayerID, &LayerInfo);
    if (HI_SUCCESS != ret)
    {
        goto ERR2;
    }
    
    /** create graphic layer */
    ret = HI_GO_CreateLayer(&LayerInfo, &hLayer);
    if (HI_SUCCESS != ret)
    {
        goto ERR2;
    }
    
    /** set the background color of graphic layer*/    
    ret = HI_GO_SetLayerBGColor(hLayer,0xffffffff);
    if (HI_SUCCESS != ret)
    {
        goto ERR3;
    }
    
    WinInfo.hLayer = hLayer;
    WinInfo.rect.x = 50;
    WinInfo.rect.y = 50;
    WinInfo.rect.w = sWidth;
    WinInfo.rect.h = sHeight;
    WinInfo.LayerNum = 0;
    WinInfo.PixelFormat = HIGO_PF_8888;
    WinInfo.BufferType = HIGO_BUFFER_SINGLE;
    
    /** create window*/
    ret = HI_GO_CreateWindowEx(&WinInfo, &hWindow);
    if (HI_SUCCESS != ret)
    {
        goto ERR3;
    }
    
    /** get surface of the window */
    ret = HI_GO_GetWindowSurface(hWindow, &hWinSurface);
    if (HI_SUCCESS != ret)
    {
        goto ERR4;
    }
    
    /** fill window*/
    ret = HI_GO_FillRect(hWinSurface,NULL,0xFFFF0000,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
        goto ERR4;
    }
        
    /** set the opacity of the window */ 
    ret =  HI_GO_SetWindowOpacity(hWindow,128); 
    if (HI_SUCCESS != ret)
    {
        goto ERR4;
    }
    
    /** should fresh the window if any changed to the window*/	
    ret = HI_GO_UpdateWindow(hWindow,NULL);
    if (HI_SUCCESS != ret)
    {
        goto ERR4;
    }
    
    //fresh the window to the graphic layer
    ret = HI_GO_RefreshLayer(hLayer, NULL);
    if (HI_SUCCESS != ret)
    {
        goto ERR4;
    }  
    
    Printf("press any key to change pos\n");
    getchar();
    
    /** change the position of the window*/
    ret = HI_GO_SetWindowPos(hWindow,300,200);
    if (HI_SUCCESS != ret)
    {
        goto ERR4;
    }

    /** fresh the window */
    ret = HI_GO_UpdateWindow(hWindow, NULL);
    if (HI_SUCCESS != ret)
    {
        goto ERR4;
    }
    
    //fresh the window to the graphic layer
    ret = HI_GO_RefreshLayer(hLayer, NULL);
    if (HI_SUCCESS != ret)
    {
        goto ERR4;
    }  
    printf("press any key to exit\n");
    getchar();
    
ERR4:
    HI_GO_DestroyWindow(hWindow);
ERR3:
    HI_GO_DestroyLayer(hLayer);
ERR2:
    HI_GO_Deinit();
ERR1:
    Display_DeInit();
    return ret;    
}
