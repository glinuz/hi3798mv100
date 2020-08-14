#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_disp.h"

#ifdef ANDROID
#include "hi_adp_osd.h"
#else
#include "hi_go.h"
#include "hi_go_text.h"
#include "hi_go_bliter.h"
#endif

#include "sample_cc_out.h"
#include "hi_unf_cc.h"

//#define CC_DEBUG

#ifdef CC_DEBUG
#define DBG_PRINT(fmt,args...)  printf(fmt,##args)
#else
#define DBG_PRINT(fmt,args...)
#endif

#define CC_FONT_FILE "../res/font/DroidSansMono.ttf"

#define COLOR2ARGB(a, c) (a) = ((c).u8Alpha << 24) | ((c).u8Red << 16) | ((c).u8Green << 8) | (c).u8Blue
#define ANDROID_COLOR2ARGB(a, c)  (a) = ((c).u8Alpha << 24) | ((c).u8Red << 16) | ((c).u8Green << 8) | (c).u8Blue

static HI_HANDLE s_hVBI = 0xffffff;

HI_U32 u32VirtualScreenWidth = 1280;
HI_U32 u32VirtualScreenHeight = 720;

HI_U32 u32CCSurfaceWidth;
HI_U32 u32CCSurfaceHeight;

#ifdef ANDROID
#define HI_INVALID_HANDLE (0xffffffff)
static HI_HANDLE s_hSurface = HI_INVALID_HANDLE;

#else
static HI_HANDLE s_hccLayer = HIGO_INVALID_HANDLE;
static HI_HANDLE s_LayerSurface = HIGO_INVALID_HANDLE;
static HI_HANDLE s_hFontOutStardard= HIGO_INVALID_HANDLE;
static HI_HANDLE s_hFontOutLarge= HIGO_INVALID_HANDLE;
static HI_HANDLE s_hFontOutSmall= HIGO_INVALID_HANDLE;
static HI_HANDLE s_hCCSurface = HIGO_INVALID_HANDLE;

#endif

static HI_S32 GetUnicode_Len(HI_U32 c)
{
    HI_S32 len = 0;
    if (c < 0x80) {len = 1;}
    else if (c < 0x800) {len = 2;}
    else if (c < 0x10000) {len = 3;}
    else if (c < 0x200000) {len = 4;}
    else if (c < 0x4000000) {len = 5;}
    else {len = 6;}
    return len;
}

static HI_S32 ConvUNICODEToUTF8code( HI_U32 c, HI_U8 *outbuf)
{
    HI_U32 len ;
    HI_S32 first;
    HI_S32 i;

    if (c < 0x80) {first = 0; len = 1;}
    else if (c < 0x800) {first = 0xc0;len = 2;}
    else if (c < 0x10000) {first = 0xe0;len = 3;}
    else if (c < 0x200000) {first = 0xf0;len = 4;}
    else if (c < 0x4000000) {first = 0xf8;len = 5;}
    else {first = 0xfc;len = 6;}

    for (i = len - 1; i > 0; --i)
    {
        outbuf[i] = (c & 0x3f) | 0x80;
        c >>= 6;
    }
    outbuf[0] = c | first;

    return len;
}


static HI_S32 UTF16LE_to_UTF8(const HI_U8 *pInbuf, HI_S32 InLen,HI_U8 *pOutbuf, HI_S32 *pOutLen )
{
    const HI_U8 *in = (const HI_U8*)pInbuf;
    HI_U8 *p = pOutbuf;
    HI_U32 unicode = 0;
    HI_S32 s32Outsize = *pOutLen;
    HI_S32 s32len = 0;
    HI_S32 i = 0;
    HI_U32 tmp = 0;

    for(i = 0; i < InLen;i += 2)
    {
        if((i+1) < InLen)
        {
            unicode = in[i];
            tmp = in[i+1];
            unicode |= tmp << 8;
        }
        else
        {
            break;
        }

        s32len =  GetUnicode_Len(unicode);
        if( s32Outsize < s32len )
        {
            break;
        }
        s32Outsize -= s32len;

        p += ConvUNICODEToUTF8code(unicode,p);

    }

    *p = 0;
    *pOutLen = p - pOutbuf;

    return HI_SUCCESS;
}

#ifndef ANDROID
static HI_U8 JustifyTranscode(HI_U8 u8justify)
{
    switch(u8justify)
    {
        case HI_UNF_CC_JUSTIFY_LEFT:
            u8justify = HIGO_LAYOUT_LEFT;
            break;
        case HI_UNF_CC_JUSTIFY_RIGHT:
            u8justify = HIGO_LAYOUT_RIGHT;
            break;
        case HI_UNF_CC_JUSTIFY_CENTER:
            u8justify = HIGO_LAYOUT_HCENTER;
            break;
        case HI_UNF_CC_JUSTIFY_FULL:
            u8justify = HIGO_LAYOUT_HCENTER;
            break;
        default:
            u8justify = HIGO_LAYOUT_LEFT;
            break;
    }
    return u8justify;
}
#endif

static HI_U8 ReverseChar(HI_U8 Data)
{
    HI_U8 b0,b1,b2,b3,b4,b5,b6,b7;
    HI_U8  Reversed;

    b0 = (Data >> 0) & 0x1 ;
    b1 = (Data >> 1) & 0x1 ;
    b2 = (Data >> 2) & 0x1 ;
    b3 = (Data >> 3) & 0x1 ;
    b4 = (Data >> 4) & 0x1 ;
    b5 = (Data >> 5) & 0x1 ;
    b6 = (Data >> 6) & 0x1 ;
    b7 = (Data >> 7) & 0x1 ;
    Reversed = (b7 << 0)
             | (b6 << 1)
             | (b5 << 2)
             | (b4 << 3)
             | (b3 << 4)
             | (b2 << 5)
             | (b1 << 6)
             | (b0 << 7);
    return(Reversed);
}

#ifdef ANDROID
static HI_S32 OsdInit(HI_VOID)
{
    HI_S32 s32Ret = 0;
    HIADP_SURFACE_ATTR_S    stSurAttr;
    HIADP_SURFACE_ATTR_S    stFontSurAttr;
    (HI_VOID)HIADP_OSD_Init();
    
    HI_UNF_DISP_GetVirtualScreen(HI_UNF_DISPLAY0, &u32VirtualScreenWidth, &u32VirtualScreenHeight);

    DBG_PRINT("w[%d]h[%d]\n",u32VirtualScreenWidth,u32VirtualScreenHeight);

    stSurAttr.u32Width = u32VirtualScreenWidth;
    stSurAttr.u32Height = u32VirtualScreenHeight;
    stSurAttr.enPixelFormat = HIADP_PF_8888;

    s32Ret = HIADP_OSD_CreateSurface(&stSurAttr, &s_hSurface);

    if (HI_SUCCESS != s32Ret)
    {
        printf( "failed to HIADP_OSD_CreateSurface, ret = 0x%x !\n", s32Ret);
        goto RET;
    }
    return HI_SUCCESS;

RET:
    if (HI_INVALID_HANDLE != s_hSurface)
    {
        (HI_VOID)HIADP_OSD_DestroySurface(s_hSurface);
        s_hSurface = HI_INVALID_HANDLE;
    }

    (HI_VOID)HIADP_OSD_DeInit();

    return HI_FAILURE;
}


static HI_S32 OsdDeInit(HI_VOID)
{

    if (HI_INVALID_HANDLE != s_hSurface)
    {
        HIADP_OSD_ClearSurface(s_hSurface);
        HIADP_OSD_DestroySurface(s_hSurface);
        s_hSurface = HI_INVALID_HANDLE;
    }

    (HI_VOID)HIADP_OSD_DeInit();

    return HI_SUCCESS;

}
#else
static HI_S32 HigoInit()
{
    HI_S32 s32Ret = 0;
    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HIGO_TEXT_INFO_S stFontInfo;

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("failed to init higo! ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);

    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    stLayerInfo.PixelFormat = HIGO_PF_8888;
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &s_hccLayer);

    if (HI_SUCCESS != s32Ret)
    {
        printf( "failed to create the layer hd 0, ret = 0x%x !\n", s32Ret);
        goto RET;
    }

    s32Ret =  HI_GO_GetLayerSurface(s_hccLayer, &s_LayerSurface);

    if (HI_SUCCESS != s32Ret)
    {
        printf( "failed to get layer surface! s32Ret = 0x%x !\n", s32Ret);
        goto RET;
    }


    (HI_VOID)HI_GO_FillRect(s_LayerSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);

    stFontInfo.pMbcFontFile = HI_NULL;
    stFontInfo.pSbcFontFile = CC_FONT_FILE;
    stFontInfo.u32Size = 20;

    if(HI_SUCCESS != HI_GO_CreateTextEx(&stFontInfo, &s_hFontOutStardard))
    {
        printf( "failed to create the stardard font:%s !ret =0x%x\n",CC_FONT_FILE,s32Ret);
        goto RET;
    }
    HI_GO_SetInputCharSet(s_hFontOutStardard,HIGO_CHARSET_UTF8);

    stFontInfo.u32Size = 24;
    if(HI_SUCCESS != HI_GO_CreateTextEx(&stFontInfo, &s_hFontOutLarge))
    {
        printf( "failed to create the large font:%s !ret =0x%x\n",CC_FONT_FILE,s32Ret);
        goto RET;
    }
    HI_GO_SetInputCharSet(s_hFontOutLarge,HIGO_CHARSET_UTF8);

    stFontInfo.u32Size = 14;
    if(HI_SUCCESS != HI_GO_CreateTextEx(&stFontInfo, &s_hFontOutSmall))
    {
        printf( "failed to create the small font:%s !ret =0x%x\n",CC_FONT_FILE,s32Ret);
        goto RET;
    }
    HI_GO_SetInputCharSet(s_hFontOutSmall,HIGO_CHARSET_UTF8);

    return HI_SUCCESS;

RET:
    if (HIGO_INVALID_HANDLE != s_hccLayer)
    {
        (HI_VOID)HI_GO_DestroyLayer(s_hccLayer);
        s_hccLayer = HIGO_INVALID_HANDLE;
    }

    if (HIGO_INVALID_HANDLE != s_hFontOutStardard)
    {
        (HI_VOID)HI_GO_DestroyText(s_hFontOutStardard);
        s_hFontOutStardard = HIGO_INVALID_HANDLE;
    }

    if (HIGO_INVALID_HANDLE != s_hFontOutLarge)
    {
        (HI_VOID)HI_GO_DestroyText(s_hFontOutLarge);
        s_hFontOutLarge = HIGO_INVALID_HANDLE;
    }

    if (HIGO_INVALID_HANDLE != s_hFontOutSmall)
    {
        (HI_VOID)HI_GO_DestroyText(s_hFontOutSmall);
        s_hFontOutSmall = HIGO_INVALID_HANDLE;
    }

    s_LayerSurface = HIGO_INVALID_HANDLE;

    (HI_VOID)HI_GO_Deinit();

    return HI_FAILURE;
}


static HI_S32 HigoDeInit()
{
    if (HIGO_INVALID_HANDLE != s_hCCSurface)
    {
        (HI_VOID)HI_GO_FreeSurface(s_hCCSurface);
        s_hCCSurface = HIGO_INVALID_HANDLE;
    }

    if (HIGO_INVALID_HANDLE != s_hccLayer)
    {
        (HI_VOID)HI_GO_DestroyLayer(s_hccLayer);
        s_hccLayer = HIGO_INVALID_HANDLE;
    }
    if (HIGO_INVALID_HANDLE != s_hFontOutStardard)
    {
        (HI_VOID)HI_GO_DestroyText(s_hFontOutStardard);
        s_hFontOutStardard = HIGO_INVALID_HANDLE;
    }
    if (HIGO_INVALID_HANDLE != s_hFontOutLarge)
    {
        (HI_VOID)HI_GO_DestroyText(s_hFontOutLarge);
        s_hFontOutLarge = HIGO_INVALID_HANDLE;
    }
    if (HIGO_INVALID_HANDLE != s_hFontOutSmall)
    {
        (HI_VOID)HI_GO_DestroyText(s_hFontOutSmall);
        s_hFontOutSmall = HIGO_INVALID_HANDLE;
    }
    s_LayerSurface = HIGO_INVALID_HANDLE;

    (HI_VOID)HI_GO_Deinit();

    return HI_SUCCESS;
}
#endif

HI_S32 CC_Output_Init(HI_HANDLE* phOut)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_DISP_VBI_CFG_S stVBICfg = {0};

#ifdef ANDROID
    if (HI_INVALID_HANDLE == s_hSurface)
    {
        s32Ret = OsdInit();
    }
#else
    if (HIGO_INVALID_HANDLE == s_LayerSurface)
    {
        s32Ret = HigoInit();
    }
#endif

    stVBICfg.enType =  HI_UNF_DISP_VBI_TYPE_CC;
    HI_UNF_DISP_CreateVBI(HI_UNF_DISPLAY0,&stVBICfg,&s_hVBI);

    *phOut = 0xFE00;

    return s32Ret;
}

HI_S32 CC_Output_DeInit(HI_HANDLE hOut)
{
    HI_S32 s32Ret = HI_SUCCESS;

#ifdef ANDROID
    if (HI_INVALID_HANDLE != s_hSurface)
    {
        s32Ret = OsdDeInit();
    }
#else
    if (HIGO_INVALID_HANDLE != s_LayerSurface)
    {
        s32Ret = HigoDeInit();
    }
#endif

    if (0xffffff != s_hVBI)
    {
        (HI_VOID)HI_UNF_DISP_DestroyVBI(s_hVBI);
    }

    return s32Ret;
}


HI_S32 CC_Output_OnDraw(HI_VOID* pUserData, HI_UNF_CC_DISPLAY_PARAM_S *pstDisplayParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32SurfaceWidth = 0, s32SurfaceHeight = 0;
#ifdef ANDROID
    HI_RECT_S SrcRect = {0};
#else
    HI_RECT SrcRect = {0};
    HIGO_BLTOPT_S stBlitOpt;
#endif

    if(NULL == pstDisplayParam)
    {
        return HI_FAILURE;
    }

    u32CCSurfaceWidth = pstDisplayParam->u32DisplayWidth;
    u32CCSurfaceHeight = pstDisplayParam->u32DisplayHeight;

#ifdef ANDROID
    SrcRect.s32X = pstDisplayParam->stRect.x;
    SrcRect.s32Y = pstDisplayParam->stRect.y;
    SrcRect.s32Width = pstDisplayParam->stRect.width;
    SrcRect.s32Height = pstDisplayParam->stRect.height;


   DBG_PRINT("DisplayWidth : %d, DisplayHeight : %d\n", pstDisplayParam->u32DisplayWidth, pstDisplayParam->u32DisplayHeight);
    if (s_hSurface == HI_INVALID_HANDLE)
    {
        DBG_PRINT("DisplayWidth : %d, DisplayHeight : %d\n", pstDisplayParam->u32DisplayWidth, pstDisplayParam->u32DisplayHeight);
        HIADP_SURFACE_ATTR_S    stSurAttr;
        
        stSurAttr.u32Width = u32VirtualScreenWidth;
        stSurAttr.u32Height = u32VirtualScreenHeight;
        stSurAttr.enPixelFormat = HIADP_PF_8888;
        HIADP_OSD_CreateSurface(&stSurAttr, &s_hSurface);

        HIADP_SURFACE_ATTR_S    stCCSurAttr;       
        stCCSurAttr.u32Width = u32CCSurfaceWidth;
        stCCSurAttr.u32Height = u32CCSurfaceHeight;
        stCCSurAttr.enPixelFormat = HIADP_PF_8888;

        HIADP_OSD_FillCCRect(s_hSurface,&stCCSurAttr, &SrcRect, 0x00000000);
    }
#else
    SrcRect.x = pstDisplayParam->stRect.x;
    SrcRect.y = pstDisplayParam->stRect.y;
    SrcRect.w = pstDisplayParam->stRect.width;
    SrcRect.h = pstDisplayParam->stRect.height;

    DBG_PRINT("DisplayWidth : %d, DisplayHeight : %d\n", pstDisplayParam->u32DisplayWidth, pstDisplayParam->u32DisplayHeight);
    if (HIGO_INVALID_HANDLE == s_hCCSurface)
    {
        DBG_PRINT("DisplayWidth : %d, DisplayHeight : %d\n", pstDisplayParam->u32DisplayWidth, pstDisplayParam->u32DisplayHeight);
        (HI_VOID)HI_GO_CreateSurface(pstDisplayParam->u32DisplayWidth,pstDisplayParam->u32DisplayHeight,
                                     HIGO_PF_8888,&s_hCCSurface);
        (HI_VOID)HI_GO_FillRect(s_hCCSurface, HI_NULL, 0x00000000, HIGO_COMPOPT_NONE);
    }
    else
    {
        (HI_VOID)HI_GO_GetSurfaceSize(s_hCCSurface, &s32SurfaceWidth, &s32SurfaceHeight);
        if(s32SurfaceWidth != pstDisplayParam->u32DisplayWidth
            || s32SurfaceHeight != pstDisplayParam->u32DisplayHeight)
        {
            DBG_PRINT("DisplayWidth : %d, DisplayHeight : %d\n", pstDisplayParam->u32DisplayWidth, pstDisplayParam->u32DisplayHeight);
            (HI_VOID)HI_GO_FreeSurface(s_hCCSurface);
            (HI_VOID)HI_GO_CreateSurface(pstDisplayParam->u32DisplayWidth,pstDisplayParam->u32DisplayHeight,
                                         HIGO_PF_8888,&s_hCCSurface);
            (HI_VOID)HI_GO_FillRect(s_hCCSurface, HI_NULL, 0x00000000, HIGO_COMPOPT_NONE);
        }
    }
#endif

    switch (pstDisplayParam->enOpt)
    {

    case HI_UNF_CC_OPT_DRAWTEXT:
    {
        HI_U8 *pu8InBuf = (HI_U8 *)pstDisplayParam->unDispParam.stText.pu16Text;
        HI_S32 s32InLen = pstDisplayParam->unDispParam.stText.u8TextLen * 2;
        HI_U8  pu8OutBuf[128];
        HI_S32 s32OutLen = sizeof(pu8OutBuf);
        HI_U32 u32bgColor,u32fgColor;
        HI_U8  u8justify = pstDisplayParam->unDispParam.stText.u8Justify;
        HI_U8  u8WordWrap = pstDisplayParam->unDispParam.stText.u8WordWrap;
        HI_U8  u8fontstyle = pstDisplayParam->unDispParam.stText.enFontStyle;
#ifndef ANDROID
        HIGO_LAYOUT_E enlayout;
#endif

        s32Ret = UTF16LE_to_UTF8(pu8InBuf,s32InLen,pu8OutBuf,&s32OutLen);
        if (HI_SUCCESS != s32Ret)
        {
            printf("failed to UTF16LE_to_UTF8,ret = %x\n",s32Ret);
        }
        
        DBG_PRINT("\nOnDraw : [%d,%s]\n",s32OutLen,pu8OutBuf);

#ifdef ANDROID
            ANDROID_COLOR2ARGB(u32bgColor, pstDisplayParam->unDispParam.stText.stBgColor);
            ANDROID_COLOR2ARGB(u32fgColor, pstDisplayParam->unDispParam.stText.stFgColor);
#else
        HI_HANDLE hFontOut;
        if(pstDisplayParam->unDispParam.stText.enFontSize == HI_UNF_CC_FONTSIZE_LARGE)
        {
            hFontOut = s_hFontOutLarge;
         }
        else if(pstDisplayParam->unDispParam.stText.enFontSize == HI_UNF_CC_FONTSIZE_SMALL)
        {
            hFontOut = s_hFontOutSmall;
        }
        else
        {
            hFontOut = s_hFontOutStardard;
        }
        COLOR2ARGB(u32bgColor, pstDisplayParam->unDispParam.stText.stBgColor);
        COLOR2ARGB(u32fgColor, pstDisplayParam->unDispParam.stText.stFgColor);
        HI_GO_SetTextColor(hFontOut, u32fgColor);
        HI_GO_SetTextBGColor(hFontOut, u32bgColor);

        if(0 == pstDisplayParam->unDispParam.stText.stBgColor.u8Alpha)
        {
            (HI_VOID)HI_GO_SetTextBGTransparent(hFontOut, HI_TRUE);
        }
        else
        {
            (HI_VOID)HI_GO_SetTextBGTransparent(hFontOut, HI_FALSE);
        }
#endif
        DBG_PRINT("u32bgColor:%#x\n",u32bgColor);
        DBG_PRINT("u32fgColor:%#x\n",u32fgColor);
        DBG_PRINT("u8edgeType:%d\n",pstDisplayParam->unDispParam.stText.enEdgetype);
        DBG_PRINT("u8fontSize:[%d],small,standard,large:1,2,3\n",pstDisplayParam->unDispParam.stText.enFontSize);

#ifdef ANDROID
            HIADP_CCTEXT_ATTR_S pstCCTextAttr;
            pstCCTextAttr.u32BufLen = (HI_U32)s32OutLen;
            pstCCTextAttr.u32bgColor = u32bgColor;
            pstCCTextAttr.u32fgColor = u32fgColor;
            pstCCTextAttr.u8fontSize = pstDisplayParam->unDispParam.stText.enFontSize;

            //underline
            if ((u8fontstyle ==  HI_UNF_CC_FONTSTYLE_UNDERLINE) || (u8fontstyle == HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE))
            {
                pstCCTextAttr.underline = HI_TRUE;
            }
            else
            {
                pstCCTextAttr.underline = HI_FALSE;
            }

            //italic
            if ((u8fontstyle ==  HI_UNF_CC_FONTSTYLE_ITALIC) || (u8fontstyle == HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE))
            {
                pstCCTextAttr.italic = HI_TRUE;
            }
            else
            {
                pstCCTextAttr.italic = HI_FALSE;
            }



            HIADP_SURFACE_ATTR_S pstAttr;
            pstAttr.enPixelFormat=HIADP_PF_8888;
            pstAttr.u32Width=u32CCSurfaceWidth;
            pstAttr.u32Height=u32CCSurfaceHeight;

            HIADP_OSD_DrawCCText(s_hSurface, &pstAttr,&SrcRect, (HI_CHAR*)pu8OutBuf, &pstCCTextAttr);
            
#else
            enlayout = JustifyTranscode(u8justify);

        if(u8WordWrap)
        {
            enlayout = enlayout | HIGO_LAYOUT_WRAP;
        }

        if((u8fontstyle ==  HI_UNF_CC_FONTSTYLE_ITALIC)||(u8fontstyle == HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE))
        {
            HI_GO_SetTextStyle(hFontOut,HIGO_TEXT_STYLE_ITALIC);
        }
        else
        {
            HI_GO_SetTextStyle(hFontOut,HIGO_TEXT_STYLE_NORMAL);
        }

        DBG_PRINT("Text Rect[%d %d %d %d]\n\n",SrcRect.x,SrcRect.y,SrcRect.w,SrcRect.h);



         s32Ret = HI_GO_TextOutEx(hFontOut, s_hCCSurface, (HI_CHAR *)pu8OutBuf, &SrcRect, enlayout);
        if (HI_SUCCESS != s32Ret)
        {
            printf("failed to text out char!,ret = %x\n",s32Ret);
        }

        memset(&stBlitOpt, 0, sizeof(HIGO_BLTOPT_S));
        stBlitOpt.EnableScale = HI_TRUE;
        (HI_VOID)HI_GO_Blit (s_hCCSurface, HI_NULL,
                   s_LayerSurface, HI_NULL,
                   &stBlitOpt);
        (HI_VOID)HI_GO_RefreshLayer(s_hccLayer, HI_NULL);
#endif

        break;
    }
    case HI_UNF_CC_OPT_FILLRECT:
    {
        HI_U32 u32Color;

#ifdef ANDROID
            ANDROID_COLOR2ARGB(u32Color, pstDisplayParam->unDispParam.stFillRect.stColor);

            DBG_PRINT("\nFillRect, u32Color:%#x, SrcRect[%d %d %d %d]\n\n", u32Color, SrcRect.s32X, SrcRect.s32Y, SrcRect.s32Width, SrcRect.s32Height);

            HIADP_SURFACE_ATTR_S pstAttr;
            pstAttr.enPixelFormat=HIADP_PF_8888;
            pstAttr.u32Width=u32CCSurfaceWidth;
            pstAttr.u32Height=u32CCSurfaceHeight;

            HIADP_OSD_FillCCRect(s_hSurface, &pstAttr, &SrcRect, u32Color);
#else
        COLOR2ARGB(u32Color, pstDisplayParam->unDispParam.stFillRect.stColor);

        DBG_PRINT("\nFillRect, u32Color:%#x, Rect[%d %d %d %d]\n\n",u32Color, SrcRect.x,SrcRect.y,SrcRect.w,SrcRect.h);

            (HI_VOID)HI_GO_FillRect(s_hCCSurface, &SrcRect, u32Color, HIGO_COMPOPT_NONE);

            memset(&stBlitOpt, 0, sizeof(HIGO_BLTOPT_S));
            stBlitOpt.EnableScale = HI_TRUE;
            (HI_VOID)HI_GO_Blit (s_hCCSurface, HI_NULL,
                                 s_LayerSurface, HI_NULL,
                                 &stBlitOpt);
            (HI_VOID)HI_GO_RefreshLayer(s_hccLayer, NULL);
#endif
            break;
        }
        case HI_UNF_CC_OPT_DRAWBITMAP:
        default:
            printf("Not implement\n");
            break;

    }

    return s32Ret;
}


HI_S32 CC_Output_GetTextSize(HI_VOID* pUserdata, HI_U16 *u16Str,HI_S32 s32StrNum, HI_S32 *ps32Width, HI_S32 *ps32Heigth)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_U8 *pu8InBuf = (HI_U8 *)u16Str;
    HI_S32 s32InLen = s32StrNum * 2;
    HI_U8 pu8OutBuf[128];
    HI_S32 s32OutLen = sizeof(pu8OutBuf);
    if((NULL == u16Str) || (0 == s32StrNum) || (NULL == ps32Width) || (NULL == ps32Heigth))
    {
        return HI_FAILURE;
    }

    *ps32Width = *ps32Heigth = 0;

    s32Ret = UTF16LE_to_UTF8(pu8InBuf,s32InLen,pu8OutBuf,&s32OutLen);
    if (HI_SUCCESS != s32Ret)
    {
        printf("failed to UTF16LE_to_UTF8,ret = %x\n",s32Ret);
    }
#ifdef ANDROID
    s32Ret = HIADP_OSD_GetCCTextSize((HI_CHAR* )pu8OutBuf, ps32Width, ps32Heigth);
    *ps32Heigth = *ps32Heigth + 6;
#else
    s32Ret = HI_GO_GetTextExtent(s_hFontOutStardard, (const HI_CHAR*)pu8OutBuf, ps32Width, ps32Heigth);
#endif
    if (HI_SUCCESS != s32Ret)
    {
        printf("failed to HI_GO_GetTextExtent,ret = %x\n",s32Ret);
    }

    return  HI_SUCCESS;
}

HI_S32 CC_Output_Blit(HI_VOID* pUserPrivatData, HI_UNF_CC_RECT_S *pstSrcRect, HI_UNF_CC_RECT_S *pstDestRect)
{
#ifdef ANDROID
    HI_RECT_S stSrcRect = {0, 0, 0, 0};
    HI_RECT_S stDstRect = {0, 0, 0, 0};
#else
    HIGO_BLTOPT_S stBlitOpt;
    HI_RECT stSrcRect = {0, 0, 0, 0};
    HI_RECT stDstRect = {0, 0, 0, 0};
#endif
    if(NULL == pstSrcRect || NULL == pstDestRect)
    {
        return HI_FAILURE;
    }

#ifdef ANDROID
    if (HI_INVALID_HANDLE == s_hSurface)
    {
        DBG_PRINT("Surface has not create!\n");
        return HI_SUCCESS;
    }

    stSrcRect.s32X = pstSrcRect->x;
    stSrcRect.s32Y = pstSrcRect->y;
    stSrcRect.s32Width = pstSrcRect->width;
    stSrcRect.s32Height = pstSrcRect->height;

    stDstRect.s32X = pstDestRect->x;
    stDstRect.s32Y = pstDestRect->y;
    stDstRect.s32Width = pstDestRect->width;
    stDstRect.s32Height = pstDestRect->height;

    DBG_PRINT("Blit:[%d,%d,%d,%d]->[%d,%d,%d,%d]\n", stSrcRect.s32X, stSrcRect.s32Y, stSrcRect.s32Width, stSrcRect.s32Height, stDstRect.s32X, stDstRect.s32Y, stDstRect.s32Width, stDstRect.s32Height);

    HIADP_SURFACE_ATTR_S pstAttr;
    pstAttr.enPixelFormat=HIADP_PF_8888;
    pstAttr.u32Width=u32CCSurfaceWidth;
    pstAttr.u32Height=u32CCSurfaceHeight;

    HIADP_OSD_CCBlit(s_hSurface,&pstAttr, &stSrcRect, &stDstRect);
#else
    if (HIGO_INVALID_HANDLE == s_hCCSurface)
    {
        //printf("Surface has not create!\n");
        return HI_SUCCESS;
    }
    stSrcRect.x = pstSrcRect->x;
    stSrcRect.y = pstSrcRect->y;
    stSrcRect.w = pstSrcRect->width;
    stSrcRect.h = pstSrcRect->height;

    stDstRect.x = pstDestRect->x;
    stDstRect.y = pstDestRect->y;
    stDstRect.w = pstDestRect->width;
    stDstRect.h = pstDestRect->height;

    DBG_PRINT("Blit:[%d,%d,%d,%d]->[%d,%d,%d,%d]\n",stSrcRect.x,stSrcRect.y,stSrcRect.w,stSrcRect.h,stDstRect.x,stDstRect.y,stDstRect.w,stDstRect.h);
    (HI_VOID)HI_GO_Blit (s_hCCSurface, &stSrcRect,s_hCCSurface, &stDstRect, HI_NULL);

    memset(&stBlitOpt, 0, sizeof(HIGO_BLTOPT_S));
    stBlitOpt.EnableScale = HI_TRUE;
    (HI_VOID)HI_GO_Blit (s_hCCSurface, HI_NULL, s_LayerSurface, HI_NULL ,&stBlitOpt);
    (HI_VOID)HI_GO_RefreshLayer(s_hccLayer, HI_NULL);
#endif
    return HI_SUCCESS;
}

HI_S32 CC_Output_VBIOutput(HI_VOID* pUserData, HI_UNF_CC_VBI_DADA_S *pstVBIDataField1, HI_UNF_CC_VBI_DADA_S *pstVBIDataField2)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U8 buf[25] = {0x00,0x00,0x01,0xbd,0x00,0x13,0x8f,0x80,0x05,0x21,0x25,0xb5,0xf1,0x27,0x99,0xc5,
                                      0x03,0x00,0x00,0x00,0xc5,0x03,0x00,0x00,0x00};
    
    HI_UNF_DISP_VBI_DATA_S stVBIData = {0};

    buf[17] = 0xf5;
    buf[18] = ReverseChar(pstVBIDataField1->u8Data1);
    buf[19] = ReverseChar(pstVBIDataField1->u8Data2);

    buf[22] = 0xd5;
    buf[23] = ReverseChar(pstVBIDataField2->u8Data1);
    buf[24] = ReverseChar(pstVBIDataField2->u8Data2);

    if ((buf[18] == 0) && (buf[19] == 0)) //顶场数据为0
    {
        if ((buf[23] != 0) || (buf[24] != 0)) //底场数据不为0，只传底场
        {
            buf[17] = buf[22];
            buf[18] = buf[23];
            buf[19] = buf[24];

            buf[20] = 0xff; //0xc5->0xff
            buf[21] = 0x0; //0x03->0x0
        }
    }
    else if ((buf[23] == 0) && (buf[24] == 0)) //顶场数据不为0，底场数据为0，只传顶场
    {
        buf[20] = 0xff; //0xc5->0xff
        buf[21] = 0x0; //0x03->0x0
    }

    stVBIData.enType = HI_UNF_DISP_VBI_TYPE_CC;
    stVBIData.pu8DataAddr = buf;
    stVBIData.u32DataLen = sizeof(buf);


    if(0xffffff != s_hVBI)
    {
        Ret = HI_UNF_DISP_SendVBIData(s_hVBI,&stVBIData);
    }

    return Ret;

}
