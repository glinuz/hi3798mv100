/**
\file
\brief bmp encode example
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author a
\date 2009-2-28
*/

#include "hi_go.h"
#include "sample_displayInit.h"

/***************************** Macro Definition ******************************/
#define SAMPLE_GIF_FILE "../res/gif/banner.gif"

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_S32 main()
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE  hDecoder, hDecSurface;
    HIGO_DEC_ATTR_S SrcDesc;
    HI_S32 Index = 0;
    HIGO_DEC_IMGATTR_S ImgAttr;
    HIGO_ENC_ATTR_S Attr;    
    
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

    /** create decode */
    SrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    SrcDesc.SrcInfo.pFileName = SAMPLE_GIF_FILE;
    ret = HI_GO_CreateDecoder(&SrcDesc, &hDecoder );
    if (HI_SUCCESS != ret)
    {
        goto ERR2;
    }

    ImgAttr.Format = HIGO_PF_1555;
    ImgAttr.Width = 1280;
    ImgAttr.Height = 720;
        
    /** picture decoding */
    ret = HI_GO_DecImgData(hDecoder, Index, &ImgAttr, &hDecSurface);
    if (HI_SUCCESS != ret)
    {
        goto ERR3;
    }
    Attr.ExpectType = HIGO_IMGTYPE_BMP;
    ret = HI_GO_EncodeToFile(hDecSurface, "/tmp/mybitmap.bmp", &Attr);
    if (HI_SUCCESS != ret)
    {
        goto ERR3;
    }
    
    /** free decode */
    HI_GO_FreeSurface(hDecSurface);
    
ERR3:
    HI_GO_DestroyDecoder(hDecoder);
ERR2:
    HI_GO_Deinit();
ERR1:
    Display_DeInit();
    
    return ret;
}
