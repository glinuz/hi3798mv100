/**
 \file
 \brief JPEG example
 \copyright Shenzhen Hisilicon Co., Ltd.
 \version draft
 \author Xue Jiancheng 57522
 \date 2008-7-24
 */
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
#define PIC_NAME "../res/jpg/image2.jpg"

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** decoding the picture and put it to surface */
HI_S32 file_dec(HI_CHAR *pszFileName, HI_HANDLE *pSurface)
{
    HI_S32 ret;
    HI_HANDLE hDecoder;
    HIGO_DEC_ATTR_S stSrcDesc;

    /** create decode*/
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pszFileName;
    ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    /** decode it to Surface */
    ret  = HI_GO_DecImgData(hDecoder, 0, NULL, pSurface);
    ret |= HI_GO_DestroyDecoder(hDecoder);

    return ret;
}

HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 ret;
    HI_HANDLE hDecSurface, hLayer, hLayerSurface;
    HI_RECT stRect;
    HIGO_LAYER_INFO_S stLayerInfo ;
    HIGO_BLTOPT_S stBltOpt = {0};
    HI_CHAR key;
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;
    HI_UNF_DISP_3D_E en3DMode = HI_UNF_DISP_3D_NONE;
    HI_UNF_ENC_FMT_E enEncFmt = HI_UNF_ENC_FMT_720P_50;

    ret = Display_Init();
    if (HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    /**initial resource*/
    ret  = HI_GO_Init();
    if (HI_SUCCESS != ret)
    {
        goto ERR1;
    }
    ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != ret)
    {
        goto ERR2;
    }

    stLayerInfo.PixelFormat = HIGO_PF_8888;
    /**create the graphic layer and get the handler */
    ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if (HI_SUCCESS != ret)
    {
        goto ERR2;
    }
    
    ret = HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if (HI_SUCCESS != ret)
    {
        goto ERR3;
    }

    stRect.x = stRect.y = 0;
    stRect.w = stLayerInfo.DisplayWidth;
    stRect.h = stLayerInfo.DisplayHeight;
    
    stBltOpt.EnableScale = HI_TRUE;

    /** clean the graphic layer Surface */
    HI_GO_FillRect(hLayerSurface, NULL, 0xff0000ff, HIGO_COMPOPT_NONE);
    
    /**decoding*/
    ret = file_dec(PIC_NAME, &hDecSurface);
    if (HI_SUCCESS == ret)
    {
        /** Blit it to graphic layer Surface */
        ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
        if (HI_SUCCESS != ret)
        {
            HI_GO_FreeSurface(hDecSurface);
            goto ERR3;
        }

        /**  free Surface */
        HI_GO_FreeSurface(hDecSurface);
    }
    
    HI_GO_RefreshLayer(hLayer, NULL);

    Printf("\nNOTE:\n");
    Printf("[0] mono format\n");
    Printf("[1] 3d stereo format(side by side)\n");
    Printf("[2] 3d stereo format(top and bottom)\n");
    Printf("[q]  finish displaying\n");
    Printf("please select number to set stereo encode picture:\n");
    
    while(1)
    {
        key = getchar();

        while(('q' != key) && ('Q' != key) 
                && ('0' != key) && ('1' != key) && ('2' != key))
        {
           key = getchar();
        }
        
        switch(key)
        {
            case '0':
                en3DMode = HI_UNF_DISP_3D_NONE;
                enEncFmt = HI_UNF_ENC_FMT_720P_50;
                break;
            case '1':
                en3DMode = HI_UNF_DISP_3D_SIDE_BY_SIDE_HALF;
                enEncFmt = HI_UNF_ENC_FMT_1080i_50;
                break;
            case '2':
                en3DMode = HI_UNF_DISP_3D_TOP_AND_BOTTOM;
                enEncFmt = HI_UNF_ENC_FMT_720P_50;
                break;
            case 'q':
            case 'Q':
                goto ERR3;
                break;
            default:
                break;
        }

        ret = HI_UNF_DISP_Set3DMode(HI_UNF_DISPLAY1, en3DMode, enEncFmt);
        if (HI_SUCCESS != ret)
        {
            goto ERR3;   
        }
        HI_GO_RefreshLayer(hLayer, NULL);
        
    }

ERR3:
    HI_GO_DestroyLayer(hLayer);
ERR2:
    HI_GO_Deinit();
ERR1:
    Display_DeInit();

    return ret;
}
