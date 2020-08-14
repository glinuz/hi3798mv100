/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : sample_dec.c
Version             : Initial Draft
Author              :
Created             : 2015/09/28
Description         :
Function List       :
History             :
Date                       Author                   Modification
2015/09/28                 y00181162                Created file
******************************************************************************/

/*********************************add include here******************************/
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hi_go.h"
#include "hi_go_decoder.h"
#include "sample_displayInit.h"

/***************************** Macro Definition ******************************/
#ifndef CONFIG_SUPPORT_CA_RELEASE
#define PIC_DIRNAME     "../res/jpg"
#else
#define PIC_DIRNAME     "/home/stb/bin/res/jpg"
#endif

#define PIC_SUFFIX               "jpg"
#define MAX_FILENAME_LEN         256
#define FLIP_COUNT               2


#define DISPLAY_WIDTH            1280
#define DISPLAY_HEIGHT           720


/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_S32 filter(const struct dirent *pentry)
{
    HI_CHAR *pos = NULL;
    pos = strrchr(pentry->d_name, '.');
    if(NULL == pos){
        return 0;
    }

    pos++;
    if (strcasecmp(pos, "jpg") != 0){
        return 0;
    }

    return 1;
}

HI_S32 file_dec(HI_CHAR *pszFileName, HI_HANDLE *pSurface)
{
    HI_S32 ret         = HI_SUCCESS;
    HI_HANDLE hDecoder = 0;
    HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGINFO_S stImgInfo;
    HIGO_DEC_IMGATTR_S stAttr;

    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pszFileName;

    ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if(HI_SUCCESS != ret){
        return ret;
    }

    ret = HI_GO_DecImgInfo(hDecoder, 0, &stImgInfo);
    if(HI_SUCCESS != ret){
        HI_GO_DestroyDecoder(hDecoder);
        return ret;
    }

    if(stImgInfo.Width > 4095){
       stAttr.Width = 4095;
    }else{
       stAttr.Width = stImgInfo.Width;
    }

    if(stImgInfo.Height > 4095){
        stAttr.Height = 4095;
    }else{
        stAttr.Height = stImgInfo.Height;
    }

    stAttr.Format = HIGO_PF_8888;

    if(stImgInfo.Width > 4095 || stImgInfo.Height > 4095){
        ret = HI_GO_DecImgData(hDecoder, 0, &stAttr, pSurface);
    }else{
        ret  = HI_GO_DecImgData(hDecoder, 0, NULL, pSurface);
    }
    ret |= HI_GO_DestroyDecoder(hDecoder);

    return ret;

}


HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_S32 s32FileNum;
    HI_HANDLE hDecSurface, hLayer, hLayerSurface;
    HI_RECT stRect;
    HI_CHAR aszFileName[MAX_FILENAME_LEN + 1];
    HIGO_LAYER_INFO_S stLayerInfo ;
    HIGO_BLTOPT_S stBltOpt = {0};
    HI_CHAR key;
    struct dirent **filelist = NULL;
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;

    ret = Display_Init();
    if(HI_SUCCESS != ret){
        return ret;
    }
    /**
     ** the gp_input == g0 input
     **/
    ret = HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISPLAY1, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(ret != HI_SUCCESS)
    {
        goto ERR1;
    }


    ret  = HI_GO_Init();
    if(HI_SUCCESS != ret){
        goto ERR1;
    }

    ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if(HI_SUCCESS != ret){
        goto ERR2;
    }

    stLayerInfo.ScreenWidth    = DISPLAY_WIDTH;
    stLayerInfo.ScreenHeight   = DISPLAY_HEIGHT;
    stLayerInfo.DisplayWidth   = DISPLAY_WIDTH;
    stLayerInfo.DisplayHeight  = DISPLAY_HEIGHT;
    stLayerInfo.CanvasWidth    = DISPLAY_WIDTH;
    stLayerInfo.CanvasHeight   = DISPLAY_HEIGHT;
    stLayerInfo.PixelFormat    = HIGO_PF_8888;
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;

    ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if(HI_SUCCESS != ret){
        goto ERR2;
    }
#if 1
    ret = HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if(HI_SUCCESS != ret){
        goto ERR3;
    }
#else
    ret = HI_GO_CreateSurface(DISPLAY_WIDTH,DISPLAY_HEIGHT,stLayerInfo.PixelFormat,&hLayerSurface);
    if(HI_SUCCESS != ret){
        goto ERR2;
    }
    /** 里面带刷新，不能平滑过渡**/
    ret = HI_GO_SetLayerSurface(hLayer,hLayerSurface);
    if(HI_SUCCESS != ret){
        goto ERR2;
    }
#endif

    s32FileNum = scandir(PIC_DIRNAME, &filelist, filter, alphasort);
    if(s32FileNum < 0){
        goto ERR3;
    }

    stRect.x = stRect.y = 0;
    stRect.w = stLayerInfo.DisplayWidth;
    stRect.h = stLayerInfo.DisplayHeight;

    stBltOpt.EnableScale = HI_TRUE;

    /** clean the graphic layer Surface */
    ret = HI_GO_FillRect(hLayerSurface, NULL, 0xff0000ff, HIGO_COMPOPT_NONE);
    if(s32FileNum < 0){
        goto ERR3;
    }
    sleep(5);
    while (s32FileNum--)
    {
        sprintf(aszFileName, "%s/%s", PIC_DIRNAME, filelist[s32FileNum]->d_name);
        ret = file_dec(aszFileName, &hDecSurface);
        if(HI_SUCCESS == ret){
            /** Blit it to graphic layer Surface */
            ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
            if(HI_SUCCESS == ret){
                stRect.x = (stRect.x + stRect.w ) % stLayerInfo.DisplayWidth;
                if(0 == stRect.x){
                    stRect.y = (stRect.y + stRect.h ) % stLayerInfo.DisplayHeight;
                }
                HI_GO_RefreshLayer(hLayer, NULL);
                sleep(1);
            }
            HI_GO_FreeSurface(hDecSurface);
        }else{
           Printf("dec fail ret (%d)\n",ret );
        }
        /** free the memory which saving the file name */
        free(filelist[s32FileNum]);
        usleep(200 * 1000);
    }

    Printf("press q to finish displaying\n");
    key = getchar();
    while('q' != key && 'Q' != key){
        key = getchar();
    }

    free(filelist);
    filelist = NULL;

ERR3:
    HI_GO_DestroyLayer(hLayer);
ERR2:
    HI_GO_Deinit();
ERR1:
    Display_DeInit();

    return ret;
}
