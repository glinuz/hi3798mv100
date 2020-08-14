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
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hFont = HIGO_INVALID_HANDLE;
    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HI_HANDLE hLayer = HIGO_INVALID_HANDLE;
    HI_HANDLE hLayerSurface;
    HI_RECT rc = {0};
    HI_CHAR szText[] = "中国人民";           //Test the case when chinese words mix with english words;
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;

    s32Ret = Display_Init();
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    
    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR1;
    }

    s32Ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR2;
    }
    
    stLayerInfo.PixelFormat = HIGO_PF_8888;
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR2;
    }

    s32Ret =  HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if  (HI_SUCCESS != s32Ret)
    {
        goto ERR3;       
    }

    s32Ret = HI_GO_FillRect(hLayerSurface, NULL, 0xFF000000, HIGO_COMPOPT_NONE);
    if  (HI_SUCCESS != s32Ret)
    {
        goto ERR3;       
    }
    
    s32Ret = HI_GO_CreateText(NULL, "../res/font/higo_gb2312.ubf", &hFont);
    if (HI_SUCCESS != s32Ret) 
    {
        goto ERR3;        
    }

    s32Ret = HI_GO_SetTextColor(hFont, 0xffff0000);
    if (HI_SUCCESS != s32Ret) 
    {
        goto ERR4;        
    }
    
    rc.x = 200;
    rc.y = 100;
    rc.w = 400;
    rc.h = 500;

    s32Ret = HI_GO_SetTextStyle(hFont, HIGO_TEXT_STYLE_ITALIC|HIGO_TEXT_STYLE_BOLD);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR4;
    }

    s32Ret = HI_GO_TextOut(hFont, hLayerSurface, szText, &rc);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR4;
    }

    s32Ret = HI_GO_SetTextColor(hFont, 0xff00ff00);    
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR4;
    }
    
    rc.y += 50;
    s32Ret = HI_GO_SetTextStyle(hFont, HIGO_TEXT_STYLE_BOLD);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR4;
    }
    
    s32Ret = HI_GO_TextOut(hFont, hLayerSurface, szText, &rc);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR4;
    }

    s32Ret = HI_GO_SetTextColor(hFont, 0xff0000ff);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR4;
    }
    
    rc.y += 50;
    s32Ret = HI_GO_TextOut(hFont, hLayerSurface, szText, &rc);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR4;
    }

    s32Ret = HI_GO_RefreshLayer(hLayer, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR4;
    }

    getchar();

ERR4:
    HI_GO_DestroyText(hFont);
ERR3:
    HI_GO_DestroyLayer(hLayer);
ERR2:
    HI_GO_Deinit();
ERR1:
    Display_DeInit();
    
    return s32Ret;
}
