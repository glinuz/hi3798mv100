/**
\file
\brief GIF animation decoding example
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author w66592
\date 2009-11-11
*/

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hi_go.h"
#include "sample_displayInit.h"

/***************************** Macro Definition ******************************/
#define WINDOWSSTYLE 0      //window style: the background color is transparent or white, don't use the background color setted by gif file.

#define PIC_DIRNAME "../res/gif"
#define MAX_FILENAME_LEN 256
#define DEFPIXELFORMAT HIGO_PF_1555
#ifndef INVALID_HANDLE
#define INVALID_HANDLE 0
#endif
#define CHECKRET(Ret) \
    do { \
        if (Ret != HI_SUCCESS) \
        { \
            Printf("error :%x, line: %d\n", Ret, __LINE__); \
            return Ret; \
        } \
    }while (0)
    
#define DEBUGINFO(ret) \
    do { \
        if (ret != HI_SUCCESS) \
        { \
            Printf ("error :%x, line: %d\n", ret, __LINE__); \
        } \
    }while (0)
    
/*************************** Structure Definition ****************************/
typedef struct 
{
    HI_HANDLE BkSurface;  //the tmp bksurface for Render;
    HI_HANDLE DecHandle;  //handle of decoder
    HI_RECT   BKRect;    //record the region of last frame
}ANIMATION_INFO;
            


/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

//create a animation.
HI_S32 HI_ANIMATION_Create(HI_CHAR *pFileName, HI_HANDLE *phRender)
{
    HI_S32 ret;
    HIGO_DEC_ATTR_S SrcDesc;
    HIGO_DEC_PRIMARYINFO_S PrimaryInfo;
    ANIMATION_INFO *pRender;
    HI_HANDLE DecHanle, Surface;

    if ((pFileName == NULL)|| (phRender == NULL))
    {
        DEBUGINFO(HI_FAILURE);
        return HI_FAILURE;
    }    
    SrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    SrcDesc.SrcInfo.pFileName = pFileName;
    ret = HI_GO_CreateDecoder(&SrcDesc, &DecHanle);
    if (ret != HI_SUCCESS)
    {
        DEBUGINFO(HI_FAILURE);
        return HI_FAILURE;
    }

    ret = HI_GO_DecCommInfo(DecHanle, &PrimaryInfo);
    if (ret != HI_SUCCESS)
    {
        goto err1;
    }
    ret = HI_GO_CreateSurface(PrimaryInfo.ScrWidth, PrimaryInfo.ScrHeight, DEFPIXELFORMAT, &Surface);
    if (ret != HI_SUCCESS)
    {
        goto err1;
    }
    
    pRender = (ANIMATION_INFO *)malloc (sizeof(ANIMATION_INFO));
    if (pRender == NULL)
    {   
        ret = HI_FAILURE;
        goto err2;
    }
    memset (pRender, 0, sizeof(ANIMATION_INFO));
    pRender->BkSurface = Surface;
    pRender->DecHandle = DecHanle;

    *phRender = (unsigned long)pRender;
    return HI_SUCCESS;
err2:
    HI_GO_FreeSurface(Surface);
err1:
    HI_GO_DestroyDecoder(DecHanle);
    return ret;
}

//destroy animation. 
HI_S32 HI_ANIMATION_Destroy(HI_HANDLE hRender)
{
    ANIMATION_INFO *pRender;

    pRender = (ANIMATION_INFO *)(unsigned long)hRender;
    if (pRender == NULL)
    {
        DEBUGINFO(HI_FAILURE);
        return HI_FAILURE;
    }
    HI_GO_DestroyDecoder(pRender->DecHandle);

    HI_GO_FreeSurface(pRender->BkSurface);
    
    free(pRender);
    return HI_SUCCESS;
}

// When call this function, you should created before
HI_S32 HI_ANIMATION_GetPrimaryInfo(HI_HANDLE hRender, HIGO_DEC_PRIMARYINFO_S * pPrimaryInfo)
{
    HI_S32 Ret;
    ANIMATION_INFO *pRender;

    pRender = (ANIMATION_INFO *)(unsigned long)hRender;
    if (pRender == NULL)
    {
        DEBUGINFO(HI_FAILURE);
        return HI_FAILURE;
    }
    Ret = HI_GO_DecCommInfo(pRender->DecHandle, pPrimaryInfo);
    CHECKRET(Ret);
    return HI_SUCCESS;
}


// When call this function, you should created before. attention, there are 3 limitations 
// 1.    the decoding order is  (0- (framecount -1))
//2.     the same animation use the same destination surface each time
// 3.     the same animation use the same x0, y0 each time.
HI_S32 HI_ANIMATION_RenderFrame(HI_HANDLE hRender, HI_HANDLE DstSurface, HI_U32 Index, HI_S32 x0, HI_S32 y0, HI_S32 *pDelayTime)
{
    HI_S32 Disposal;
    HI_S32 Ret;
    HIGO_DEC_IMGINFO_S ImgInfo, LastImgInfo;
    HIGO_DEC_PRIMARYINFO_S PrimaryInfo;
    HI_COLOR BKColor;
    HIGO_BLTOPT_S BltOpt = {0};
    HI_HANDLE ImgSurface;
    HIGO_BLTOPT_S BltOptKey = {0};
    HI_RECT Rect, DstRect, LastRect;
    ANIMATION_INFO *pRender;

    pRender = (ANIMATION_INFO *)(unsigned long)hRender;
    
    Ret = HI_GO_DecCommInfo(pRender->DecHandle, &PrimaryInfo);
    CHECKRET(Ret);
#ifndef WINDOWSSTYLE
    if (PrimaryInfo.IsHaveBGColor)
    {
        BKColor = PrimaryInfo.BGColor;
    }
    else
    {
        BKColor = 0xffffffff;
    }
#else
    BKColor = 0xffffffff; //ATTENTION: This Color Can be transparent, for example: 0x00ffffff
#endif
    Ret = HI_GO_DecImgInfo(pRender->DecHandle, Index, &ImgInfo);
    CHECKRET(Ret);

    //get the scree rect in dst surface;
    DstRect.x = x0;
    DstRect.y = y0;
    DstRect.w = PrimaryInfo.ScrWidth;
    DstRect.h = PrimaryInfo.ScrHeight;

    if (0 == Index)
    {
        Ret = HI_GO_FillRect(DstSurface, &DstRect, BKColor, HIGO_COMPOPT_NONE);    
        CHECKRET(Ret);
    }
    else
    {
        HI_S32 LastDisposal;
        // get the last frame disposal method;
        
        Ret = HI_GO_DecImgInfo(pRender->DecHandle, (Index - 1), &LastImgInfo);
        CHECKRET(Ret);        
        LastDisposal = LastImgInfo.DisposalMethod;

        //get the last rect
        LastRect.x = x0 + pRender->BKRect.x;
        LastRect.y = y0 + pRender->BKRect.y;
        LastRect.w = pRender->BKRect.w;
        LastRect.h = pRender->BKRect.h;
        
        if (2 == LastDisposal)
        {
            //fill background;
            Ret = HI_GO_FillRect(DstSurface, &LastRect, BKColor, HIGO_COMPOPT_NONE);
            CHECKRET(Ret);
        }
        else if (3 == LastDisposal)
        {
            //convert to previous
            Ret = HI_GO_Blit(pRender->BkSurface, &(pRender->BKRect), DstSurface, &LastRect, &BltOpt );
            CHECKRET(Ret);
        }
    }

    //record info of 
    pRender->BKRect.x = ImgInfo.OffSetX;
    pRender->BKRect.y = ImgInfo.OffSetY;
    pRender->BKRect.w = ImgInfo.Width;
    pRender->BKRect.h = ImgInfo.Height;

    Disposal = ImgInfo.DisposalMethod;
    if (3 == Disposal)
    {
        //copy DstSurface to back surface, back the whole frame info
        Ret = HI_GO_Blit(DstSurface, &DstRect, pRender->BkSurface, NULL, &BltOpt);
        CHECKRET(Ret);      
    }

    //fill the new surface
    Ret = HI_GO_DecImgData(pRender->DecHandle, Index, NULL, &ImgSurface);    // the imgatt must be null
    CHECKRET(Ret);

    Rect.x = ImgInfo.OffSetX + x0;
    Rect.y = ImgInfo.OffSetY + y0;
    Rect.w = ImgInfo.Width;
    Rect.h = ImgInfo.Height;
    if (ImgInfo.IsHaveKey)
    {
        BltOptKey.ColorKeyFrom = HIGO_CKEY_SRC;
    }
    
    Ret = HI_GO_Blit(ImgSurface, HI_NULL, DstSurface, &Rect, &BltOptKey );
    if (Ret != HI_SUCCESS) 
        goto err;
    
    *pDelayTime = ImgInfo.DelayTime;

    HI_GO_FreeSurface(ImgSurface);
    return HI_SUCCESS;
err:
    HI_GO_FreeSurface(ImgSurface);
    
    return Ret;
}

/**************************************animaiton sample**********************************************************/
#define RENDER_POS_X 150
#define RENDER_POS_Y 150

HI_S32 filter(const struct dirent *pentry)
{
    HI_CHAR *pos = NULL;

    pos = strrchr(pentry->d_name, '.');
    if (NULL == pos)
    {
        return 0;
    }

    pos++;
    if (strcasecmp(pos, "gif") != 0)
    {
        return 0;
    }

    return 1;
}

HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HIGO_DEC_PRIMARYINFO_S PrimaryInfo;
    HI_U32 Index = 0;
    HI_S32 s32FileNum;    
    struct dirent **filelist = NULL;
    HI_HANDLE hLayer, hLayerSurface;
    HI_S32 LoopCount;
    HI_S32 DelayTime;
    HI_HANDLE hRender;
    HI_CHAR aszFileName[MAX_FILENAME_LEN + 1];
    HIGO_LAYER_INFO_S stLayerInfo ;
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;
    
    /** initial the resource */
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
    
    ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != ret)
    {
        goto ERR2;
    }
   
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;
    /** create the graphic layer and get the handler*/
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

    ret = HI_GO_FillRect(hLayerSurface , NULL, 0xffffffff, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
        goto ERR3;
    }
    
    /** get the jpeg file from the direction   */
    s32FileNum = scandir(PIC_DIRNAME, &filelist, filter, alphasort);
    if (s32FileNum < 0)
    {
        ret = HI_FAILURE;
        goto ERR3;
    }

    while (s32FileNum--)
    {
        sprintf(aszFileName, "%s/%s", PIC_DIRNAME, filelist[s32FileNum]->d_name);
        Printf ("dec file name:%s\n", aszFileName);

        //create render
        ret = HI_ANIMATION_Create(aszFileName, &hRender);
        if (ret != HI_SUCCESS)
        {   
            break;
        }

        //get render info
        ret = HI_ANIMATION_GetPrimaryInfo(hRender, &PrimaryInfo);
        if (ret != HI_SUCCESS)
        {   
            HI_ANIMATION_Destroy(hRender);        
            break;
        }

        //render all the frame
        LoopCount = PrimaryInfo.Count;
        Index = 0;
        while(LoopCount--)
        {
            ret = HI_ANIMATION_RenderFrame(hRender, hLayerSurface, Index,  RENDER_POS_X, RENDER_POS_Y , &DelayTime);
            if (ret != HI_SUCCESS)
                break;
            
            if (DelayTime == 0)
            {
               usleep(10000*20);            
            }
            else
            {
                usleep(10000*DelayTime);
            }
            
            Index ++;
            
            if (Index == (PrimaryInfo.Count))
                Index = 0;
            
            ret = HI_GO_RefreshLayer(hLayer, NULL);
            if (ret != HI_SUCCESS)
                break;
        }

        //destory the render
        HI_ANIMATION_Destroy(hRender);
    }

ERR3:
    HI_GO_DestroyLayer(hLayer);
ERR2:
    HI_GO_Deinit();
ERR1:
    Display_DeInit();
    
    return ret;
}
