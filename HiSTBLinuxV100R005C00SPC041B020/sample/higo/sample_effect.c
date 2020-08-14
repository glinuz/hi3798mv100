/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	    : sample_effect.c
Version		    : Initial Draft
Author		    : 
Created		    : 2016/01/01
Description	    : The user will use this api to realize some function
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2016/01/01		    y00181162  		    Created file      	
******************************************************************************/

/*********************************add include here******************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "hi_effect.h"
#include "hi_go.h"
#include "sample_displayInit.h"


/***************************** Macro Definition     ***************************/

#define DEBUG_INFO(ret)       Printf(" FILE:%s, LINE:%d, RET:%x\n", __FILE__, __LINE__, ret)

#define PIC_DIRNAME           "../res/jpg"
#define PIC_SUFFIX            "jpg"
#define MAX_FILENAME_LEN      256


#define MAX_DISPLAY_NUM       100  /** 循环显示次数**/

/***************************** Structure Definition ***************************/

/********************** Global Variable declaration **************************/

static HI_HANDLE hLayer = -1;

/********************** API forward declarations    **************************/

/**********************       API realization       **************************/


/*****************************************************************************
* func			: RefreshLayer
* description	: 刷新图层
* retval		: HI_SUCCESS
* retval		: HI_FAILURE
* others:		: NA
*****************************************************************************/
HI_S32 RefreshLayer(HI_VOID)
{

    HI_S32 ret ;
    ret = HI_GO_RefreshLayer(hLayer, NULL);
    if (ret != HI_SUCCESS)
    {
        DEBUG_INFO(ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;    
}

/*****************************************************************************
* func			: CreateLayer
* description	: 创建图层并获取surface
* retval		: HI_SUCCESS
* retval		: HI_FAILURE
* others:		: NA
*****************************************************************************/
HI_S32 CreateLayer(HI_HANDLE *pSurface)
{
    HIGO_LAYER_INFO_S stLayerInfo;
    HI_S32 ret ;

    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    stLayerInfo.ScreenWidth  = 1920;
    stLayerInfo.ScreenHeight = 1080;
    stLayerInfo.PixelFormat  = HIGO_PF_8888;
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;
    ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if (HI_SUCCESS != ret)
    {
        DEBUG_INFO(ret);    
        return HI_FAILURE;
    }

    ret = HI_GO_GetLayerSurface(hLayer, pSurface);
    if (HI_SUCCESS != ret)
    {
        DEBUG_INFO(ret);
        return HI_FAILURE;
    }
    return ret;
}


/*****************************************************************************
* func			: DestroyLayer
* description	: 销毁图层
* retval		: HI_SUCCESS
* retval		: HI_FAILURE
* others:		: NA
*****************************************************************************/
HI_S32 DestroyLayer(HI_HANDLE Surface)
{
    HI_GO_DestroyLayer(hLayer);
    return 0;
}

/*****************************************************************************
* func			: filter
* description	: 获取jpeg图片资源
* retval		: HI_SUCCESS
* retval		: HI_FAILURE
* others:		: NA
*****************************************************************************/
HI_S32 filter(const struct dirent *pentry)
{
    HI_CHAR *pos = NULL;

    pos = strrchr(pentry->d_name, '.');
    if (NULL == pos)
    {
        return 0;
    }

    pos++;
    if (strcasecmp(pos, "jpg") != 0)
    {
        return 0;
    }

    return 1;
}

/*****************************************************************************
* func			: file_dec
* description	: jpeg解码
* retval		: HI_SUCCESS
* retval		: HI_FAILURE
* others:		: NA
*****************************************************************************/
HI_S32 file_dec(HI_CHAR *pszFileName, HI_HANDLE *pSurface)
{
    HI_S32 ret;
    HI_HANDLE hDecoder;
    HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGATTR_S ImgAttr;

    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pszFileName;
    ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ImgAttr.Width  = 1280 ;
    ImgAttr.Height = 720;
    ImgAttr.Format = HIGO_PF_8888;
    ret  = HI_GO_DecImgData(hDecoder, 0, &ImgAttr, pSurface);
    ret |= HI_GO_DestroyDecoder(hDecoder);

    return ret;
	
}


/*****************************************************************************
* func			: main
* description	: 主程序
* retval		: HI_SUCCESS
* retval		: HI_FAILURE
* others:		: NA
*****************************************************************************/
HI_S32 main(HI_S32 argc,HI_CHAR **argv)
{

    HI_S32 ret,s32FileNum;
	HI_BOOL bExit = HI_FALSE;
	static HI_S32 s32Conut = 0;
	
    HI_HANDLE hLayerSurface;
    struct dirent **filelist = NULL;
    HI_CHAR aszFileName[MAX_FILENAME_LEN + 1];
    HI_HANDLE hDecSurface;

    ret = Display_Init();
    if (HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    /*Initialization*/
    /*CNcomment:初始化*/
    ret = HI_GO_Init();
    if (HI_SUCCESS != ret)
    {
        DEBUG_INFO(ret);
        return HI_FAILURE;
    }

    /*Create layer*/
    /*CNcomment:创建图层*/
    ret = CreateLayer(&hLayerSurface);
    if (HI_SUCCESS != ret)
    {
        DEBUG_INFO(ret);
        goto err1;
    }

    /*Initialize effect module*/
    /*CNcomment:初始化特效*/
    ret = HI_Effect_Init(RefreshLayer, HI_FALSE);
    if (HI_SUCCESS != ret)
    {
        DEBUG_INFO(ret);
        goto err2;
    }

	while(HI_FALSE == bExit)
	{
	    /*Get jpeg image list in the dir specified by mocro PIC_DIRNAME*/
	    /*CNcomment:获取指定文件夹下的JPG文件*/
	    s32FileNum = scandir(PIC_DIRNAME, &filelist, filter, alphasort);
	    if (s32FileNum < 0)
	    {
	        DEBUG_INFO(ret);
	        goto err3;
	    }
	    srand(time(0));
	    while (s32FileNum--)
	    {
	        sprintf(aszFileName, "%s/%s", PIC_DIRNAME, filelist[s32FileNum]->d_name);

	        /*Decode image*/
	        /*CNcomment:图片解码*/
	        ret = file_dec(aszFileName, &hDecSurface);
	        if (HI_SUCCESS == ret)
	        {
	            ret = HI_Effect_Play(hDecSurface, hLayerSurface,rand()%11);
	            if (ret != HI_SUCCESS)
	            {
	                DEBUG_INFO(ret);
	            }
	            /*Release surface*/
	            /*CNcomment:释放Surface */
	            HI_GO_FreeSurface(hDecSurface);
	        }

	        /*Free mem alloc for file list*/
	        /*CNcomment:释放文件名空间*/
	        free(filelist[s32FileNum]);
			
	        usleep(200 * 1000);
			
	    }

		s32Conut++;
		if(s32Conut > MAX_DISPLAY_NUM)
		{
			bExit = HI_TRUE;
		}
		
	}
	
    /*Destroy layer*/
    /*CNcomment:销毁图层*/
    ret = DestroyLayer(hLayerSurface);
    if (HI_SUCCESS != ret)
    {
        DEBUG_INFO(ret);
    }

    /*Delete initialization of effect module*/
    /*CNcomment:特效去初始化*/
    ret = HI_Effect_Deinit();
    if (HI_SUCCESS != ret)
    {
        DEBUG_INFO(ret);
    }

    /*Delete initialization of HIGO module*/
    /*CNcomment:HIGO 去初始化*/
    ret = HI_GO_Deinit();
    if (HI_SUCCESS != ret)
    {
        DEBUG_INFO(ret);
    }

    return HI_SUCCESS;
    
err3:
    HI_Effect_Deinit();
err2:
    DestroyLayer(hLayerSurface);
err1:
    HI_GO_Deinit();
    return ret;
}
