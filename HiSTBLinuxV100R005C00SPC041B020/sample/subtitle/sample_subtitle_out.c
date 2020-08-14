#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_common.h"

#include "hi_unf_disp.h"
#include "hi_unf_avplay.h"
#include "hi_unf_so.h"

#ifdef ANDROID
#include "hi_adp_osd.h"
#else
#include "hi_go.h"
#endif

#include "sample_subtitle_out.h"

#define HI_FATAL_SUBT(fmt...)      HI_FATAL_PRINT(HI_ID_SUBT, fmt)
#define HI_ERR_SUBT(fmt...)        HI_ERR_PRINT(HI_ID_SUBT, fmt)
#define HI_WARN_SUBT(fmt...)       HI_WARN_PRINT(HI_ID_SUBT, fmt)
#define HI_INFO_SUBT(fmt...)       HI_INFO_PRINT(HI_ID_SUBT, fmt)

#define USED_SURFACE_TO_FILE 0

#define SUBT_OUTPUT_HANDLE (0xFE00)
#define BITWIDTH_8_BITS 8
#define BITWIDTH_32_BITS 32


#ifdef ANDROID
#define  DefaultSurfaceWidth 720
#define  DefaultSurfaceHeight 576
#define HI_INVALID_HANDLE (0xffffffff)
static HI_HANDLE s_hSurface = HI_INVALID_HANDLE;
static HI_U8 * bitmapBuffer = HI_NULL;
static HI_U32  SurfaceWidth=DefaultSurfaceWidth,SurfaceHeight=DefaultSurfaceHeight;
HI_U32 ScreenWidth = 1280;
HI_U32 ScreenHeight = 720;
#else
static HI_HANDLE s_hLayer = HIGO_INVALID_HANDLE;
static HI_HANDLE s_hLayerSurface = HIGO_INVALID_HANDLE;
static HI_HANDLE s_hSubtSurface = HIGO_INVALID_HANDLE;
static HI_HANDLE s_hFont = HIGO_INVALID_HANDLE;

static HI_S32 higoInit()
{
    HI_S32 s32Ret = 0;

    HIGO_LAYER_INFO_S stLayerInfo = {0};

    s32Ret = HI_GO_Init();

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to init higo! ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    (HI_VOID)HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);

    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    stLayerInfo.PixelFormat = HIGO_PF_8888;
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &s_hLayer);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to create the layer hd 0, ret = 0x%x !\n", s32Ret);
        goto RET;
    }

    s32Ret =  HI_GO_GetLayerSurface(s_hLayer, &s_hLayerSurface);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to get layer surface! s32Ret = 0x%x !\n", s32Ret);
        goto RET;
    }

    (HI_VOID)HI_GO_FillRect(s_hLayerSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);

     s32Ret = HI_GO_CreateText("../res/font/higo_gb2312.ubf", NULL,  &s_hFont);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to create the font:../higo/res/higo_gb2312.ubf !\n");
        goto RET;
    }

    (HI_VOID)HI_GO_SetTextColor(s_hFont, 0xffffffff);
    s32Ret = HI_GO_CreateSurface(720,576,HIGO_PF_8888,&s_hSubtSurface);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to create subt surface! s32Ret = 0x%x\n", s32Ret);
        goto RET;
    }
    (HI_VOID)HI_GO_FillRect(s_hSubtSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);

    return HI_SUCCESS;

RET:
    if (HIGO_INVALID_HANDLE != s_hSubtSurface)
    {
        (HI_VOID)HI_GO_FreeSurface(s_hSubtSurface);
        s_hSubtSurface = HIGO_INVALID_HANDLE;
    }

    if (HIGO_INVALID_HANDLE != s_hLayer)
    {
        (HI_VOID)HI_GO_DestroyLayer(s_hLayer);
        s_hLayer = HIGO_INVALID_HANDLE;
    }

    if (HIGO_INVALID_HANDLE != s_hFont)
    {
        (HI_VOID)HI_GO_DestroyText(s_hFont);
        s_hFont = HIGO_INVALID_HANDLE;
    }

    s_hLayerSurface = HIGO_INVALID_HANDLE;

    (HI_VOID)HI_GO_Deinit();

    return HI_FAILURE;
}

static HI_S32 higoDeInit()
{
    if (HIGO_INVALID_HANDLE != s_hSubtSurface)
    {
        (HI_VOID)HI_GO_FreeSurface(s_hSubtSurface);
        s_hSubtSurface = HIGO_INVALID_HANDLE;
    }

    if (HIGO_INVALID_HANDLE != s_hLayer)
    {
        (HI_VOID)HI_GO_DestroyLayer(s_hLayer);
        s_hLayer = HIGO_INVALID_HANDLE;
    }

    if (HIGO_INVALID_HANDLE != s_hFont)
    {
        (HI_VOID)HI_GO_DestroyText(s_hFont);
        s_hFont = HIGO_INVALID_HANDLE;
    }

    s_hLayerSurface = HIGO_INVALID_HANDLE;

    (HI_VOID)HI_GO_Deinit();

    return HI_FAILURE;
}

static HI_S32 ConvertRect(HI_HANDLE hSrcSurface, HI_HANDLE hDstSurface, HI_S32 x, HI_S32 y, HI_S32 w, HI_S32 h, HI_RECT *pstRect)
{
    HI_S32 s32SrcWidth = 0, s32SrcHeight = 0;
    HI_S32 s32DstWidth = 0, s32DstHeight = 0;

    (HI_VOID)HI_GO_GetSurfaceSize(hSrcSurface, &s32SrcWidth, &s32SrcHeight);
    (HI_VOID)HI_GO_GetSurfaceSize(hDstSurface, &s32DstWidth, &s32DstHeight);

    pstRect->x = (x * s32DstWidth) / s32SrcWidth;
    pstRect->y = (y * s32DstHeight) / s32SrcHeight;
    pstRect->w = (w * s32DstWidth) / s32SrcWidth;
    pstRect->h = (h * s32DstHeight) / s32SrcHeight;

    if ((pstRect->x + pstRect->w) > s32DstWidth)
    {
        pstRect->w = s32DstWidth - pstRect->x;
    }
    if ((pstRect->y + pstRect->h) > s32DstHeight)
    {
        pstRect->h = s32DstHeight - pstRect->y;
    }

    return HI_SUCCESS;
}

#endif

#ifdef ANDROID
HI_VOID OsdDeInit(HI_VOID)
{
    if (HI_INVALID_HANDLE != s_hSurface)
    {
        HIADP_OSD_ClearSurface(s_hSurface);
        HIADP_OSD_DestroySurface(s_hSurface);
        s_hSurface = HI_INVALID_HANDLE;
    }

    (HI_VOID)HIADP_OSD_DeInit();
    return;

}

HI_S32 OsdInit(HI_VOID)
{
    HI_S32 s32Ret = 0;
    HIADP_SURFACE_ATTR_S    stSurAttr;

    (HI_VOID)HIADP_OSD_Init();

    HI_UNF_DISP_GetVirtualScreen(HI_UNF_DISPLAY0, &ScreenWidth, &ScreenHeight);
    stSurAttr.u32Width = ScreenWidth;
    stSurAttr.u32Height = ScreenHeight;
    stSurAttr.enPixelFormat = HIADP_PF_8888;

    s32Ret = HIADP_OSD_CreateSurface(&stSurAttr, &s_hSurface);

    if (HI_SUCCESS != s32Ret)
    {
        printf( "failed to HIADP_OSD_CreateSurface, ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;

}

static HI_S32 AndroidConvertRect( HI_S32 x, HI_S32 y, HI_S32 w, HI_S32 h, HI_RECT_S*pstRect)
{

    pstRect->s32X = (x * ScreenWidth) / SurfaceWidth;
    pstRect->s32Y= (y * ScreenHeight) / SurfaceHeight;
    pstRect->s32Width = (w * ScreenWidth) / SurfaceWidth;
    pstRect->s32Height = (h * ScreenHeight) / SurfaceHeight;

    if (((HI_U32)pstRect->s32X + (HI_U32)pstRect->s32Width) > ScreenWidth)
    {
        pstRect->s32Width = ScreenWidth - pstRect->s32X;
    }
    if (((HI_U32)pstRect->s32Y + (HI_U32)pstRect->s32Height) > ScreenHeight)
    {
        pstRect->s32Height = ScreenHeight - pstRect->s32Y;
    }

    return HI_SUCCESS;
}

#endif


HI_S32 Subt_Output_OnDraw(HI_VOID* pUserData, const HI_UNF_SO_SUBTITLE_INFO_S *pstInfo, HI_VOID *pArg)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 s32Width = 0, s32Height = 0;

#ifdef ANDROID
    HI_RECT_S stSrcRect = {0, 0, 0, 0};
    HI_RECT_S stDesRect = {0, 0, 0, 0};
#else
    HI_PIXELDATA pData;
    HIGO_BLTOPT_S stBlitOpt;
    HI_RECT rect = {0, 0, 0, 0};
#endif

    if (0 == pstInfo->unSubtitleParam.stGfx.u32CanvasWidth || 0 == pstInfo->unSubtitleParam.stGfx.u32CanvasHeight)
    {
        return HI_SUCCESS;
    }

    HI_INFO_SUBT("display w/h is [%d][%d]\n", pstInfo->unSubtitleParam.stGfx.u32CanvasWidth,
                                                pstInfo->unSubtitleParam.stGfx.u32CanvasHeight);

#ifdef ANDROID

    if(HI_NULL==s_hSurface)
    {
        HIADP_SURFACE_ATTR_S    stSurAttr;

        (HI_VOID)HIADP_OSD_Init();

        stSurAttr.u32Width = ScreenWidth;
        stSurAttr.u32Height = ScreenHeight;
        stSurAttr.enPixelFormat = HIADP_PF_8888;

        s32Ret = HIADP_OSD_CreateSurface(&stSurAttr, &s_hSurface);
        if (HI_SUCCESS != s32Ret)
        {
                 printf( "failed to HIADP_OSD_CreateSurface, ret = 0x%x !\n", s32Ret);
                 return HI_FAILURE;
        }
    }

#else

    if (HIGO_INVALID_HANDLE != s_hSubtSurface)
    {
        (HI_VOID)HI_GO_LockSurface(s_hSubtSurface, pData, HI_TRUE);
        (HI_VOID)HI_GO_GetSurfaceSize(s_hSubtSurface, &s32Width, &s32Height);
        (HI_VOID)HI_GO_UnlockSurface(s_hSubtSurface);
    }

    if (s32Width != pstInfo->unSubtitleParam.stGfx.u32CanvasWidth ||
        s32Height != pstInfo->unSubtitleParam.stGfx.u32CanvasHeight)
    {
        if (HIGO_INVALID_HANDLE != s_hSubtSurface)
        {
            (HI_VOID)HI_GO_FreeSurface(s_hSubtSurface);
            s_hSubtSurface = HIGO_INVALID_HANDLE;
        }
        s32Ret = HI_GO_CreateSurface(pstInfo->unSubtitleParam.stGfx.u32CanvasWidth,pstInfo->unSubtitleParam.stGfx.u32CanvasHeight,
                                        HIGO_PF_8888,&s_hSubtSurface);
        if (s32Ret == HI_FAILURE)
        {
            return HI_FAILURE;
        }
    }
#endif

    switch (pstInfo->eType)
    {
    case HI_UNF_SUBTITLE_BITMAP:
        HI_INFO_SUBT("sub title bitmap! \n");
        HI_U32 u32Index=0;
        HI_U32 i = 0, j = 0;
        HI_U32 u32PaletteIdx = 0;

#ifdef ANDROID
     if(HI_NULL==bitmapBuffer)
     {
            bitmapBuffer = (HI_U8 *)malloc(SurfaceWidth*SurfaceHeight*4);
            if(HI_NULL==bitmapBuffer)
            {
                    return HI_FAILURE;
            }
     }

        if (SurfaceWidth != pstInfo->unSubtitleParam.stGfx.u32CanvasWidth ||
        SurfaceHeight != pstInfo->unSubtitleParam.stGfx.u32CanvasHeight)
    {
         if(bitmapBuffer != HI_NULL)
         {
              free(bitmapBuffer);
              bitmapBuffer=HI_NULL;
         }
          bitmapBuffer = (HI_U8 *)malloc(pstInfo->unSubtitleParam.stGfx.u32CanvasWidth*pstInfo->unSubtitleParam.stGfx.u32CanvasHeight*4);
         if(bitmapBuffer == HI_NULL)
         {
            HI_ERR_SUBT("create subt bitmapbuffer fail!\n");
            return HI_FAILURE;
         }
         SurfaceWidth=pstInfo->unSubtitleParam.stGfx.u32CanvasWidth;
         SurfaceHeight=pstInfo->unSubtitleParam.stGfx.u32CanvasHeight;
     }
     memset(bitmapBuffer, 0x00, SurfaceWidth * SurfaceHeight * 4 );
#else
        HI_U8 *pu8Surface = NULL;
        HI_RECT DstRect = {0, 0, 0, 0};


        (HI_VOID)HI_GO_LockSurface(s_hSubtSurface, pData, HI_TRUE);
        (HI_VOID)HI_GO_GetSurfaceSize(s_hSubtSurface, &s32Width, &s32Height);

        pu8Surface = (HI_U8*)pData[0].pData;

        if (NULL == pu8Surface || NULL == pstInfo->unSubtitleParam.stGfx.stPalette)
        {
            (HI_VOID)HI_GO_UnlockSurface(s_hSubtSurface);
            return HI_SUCCESS;
        }

        if (s32Width * s32Height * pData[0].Bpp < pstInfo->unSubtitleParam.stGfx.h * pstInfo->unSubtitleParam.stGfx.w * 4)
        {
            (HI_VOID)HI_GO_UnlockSurface(s_hSubtSurface);
            return HI_SUCCESS;
        }

#endif
        if(BITWIDTH_8_BITS == pstInfo->unSubtitleParam.stGfx.s32BitWidth)
        {
            for (i = 0; i < pstInfo->unSubtitleParam.stGfx.h; i++)
            {
                for (j = 0; j < pstInfo->unSubtitleParam.stGfx.w; j++)
                {
                    if (i * pstInfo->unSubtitleParam.stGfx.w + j > pstInfo->unSubtitleParam.stGfx.u32Len)
                    {
                        break;
                    }

                    u32PaletteIdx = pstInfo->unSubtitleParam.stGfx.pu8PixData[i * pstInfo->unSubtitleParam.stGfx.w + j];
                    if (u32PaletteIdx >= HI_UNF_SO_PALETTE_ENTRY)
                    {
                        break;
                    }
#ifdef ANDROID
                    bitmapBuffer[i * SurfaceWidth*4 + 4 * j + 3]
                        = pstInfo->unSubtitleParam.stGfx.stPalette[u32PaletteIdx].u8Alpha;
                    bitmapBuffer[i * SurfaceWidth*4 + 4 * j + 2]
                        = pstInfo->unSubtitleParam.stGfx.stPalette[u32PaletteIdx].u8Blue;
                    bitmapBuffer[i * SurfaceWidth*4 + 4 * j + 1]
                        = pstInfo->unSubtitleParam.stGfx.stPalette[u32PaletteIdx].u8Green;
                    bitmapBuffer[i * SurfaceWidth*4 + 4 * j + 0]
                        = pstInfo->unSubtitleParam.stGfx.stPalette[u32PaletteIdx].u8Red;
#else
                    pu8Surface[i * pData[0].Pitch + 4 * j + 3]
                        = pstInfo->unSubtitleParam.stGfx.stPalette[u32PaletteIdx].u8Alpha;
                    pu8Surface[i * pData[0].Pitch + 4 * j + 2]
                        = pstInfo->unSubtitleParam.stGfx.stPalette[u32PaletteIdx].u8Red;
                    pu8Surface[i * pData[0].Pitch + 4 * j + 1]
                        = pstInfo->unSubtitleParam.stGfx.stPalette[u32PaletteIdx].u8Green;
                    pu8Surface[i * pData[0].Pitch + 4 * j + 0]
                        = pstInfo->unSubtitleParam.stGfx.stPalette[u32PaletteIdx].u8Blue;
#endif
                }
            }
        }
        else if(BITWIDTH_32_BITS == pstInfo->unSubtitleParam.stGfx.s32BitWidth)
        {
            for (i = 0; i < pstInfo->unSubtitleParam.stGfx.h; i++)
            {
                for (j = 0; j < pstInfo->unSubtitleParam.stGfx.w; j++)
                {
                    if (i * (pstInfo->unSubtitleParam.stGfx.w) + j > (pstInfo->unSubtitleParam.stGfx.u32Len))
                    {
                        break;
                    }
#ifdef ANDROID
                    bitmapBuffer[i * SurfaceWidth*4 + 4 * j + 3]
                        =  pstInfo->unSubtitleParam.stGfx.pu8PixData[u32Index++];/*alpha*/
                    bitmapBuffer[i * SurfaceWidth*4 + 4 * j + 0]
                        = pstInfo->unSubtitleParam.stGfx.pu8PixData[u32Index++];/*u8Blue*/
                    bitmapBuffer[i * SurfaceWidth*4 + 4 * j + 1]
                        = pstInfo->unSubtitleParam.stGfx.pu8PixData[u32Index++];/*u8Green*/
                    bitmapBuffer[i * SurfaceWidth*4 + 4 * j + 2]
                        = pstInfo->unSubtitleParam.stGfx.pu8PixData[u32Index++];/*u8Red*/
#else
                    pu8Surface[i * pData[0].Pitch + 4 * j + 3]
                        = pstInfo->unSubtitleParam.stGfx.pu8PixData[u32Index++];/*alpha*/
                    pu8Surface[i * pData[0].Pitch + 4 * j + 2]
                        = pstInfo->unSubtitleParam.stGfx.pu8PixData[u32Index++];/*u8Red*/
                    pu8Surface[i * pData[0].Pitch + 4 * j + 1]
                        = pstInfo->unSubtitleParam.stGfx.pu8PixData[u32Index++];/*u8Green*/
                    pu8Surface[i * pData[0].Pitch + 4 * j + 0]
                        = pstInfo->unSubtitleParam.stGfx.pu8PixData[u32Index++];/*u8Blue*/
#endif
                }
            }
        }


#ifdef ANDROID
       stSrcRect.s32X = pstInfo->unSubtitleParam.stGfx.x;
       stSrcRect.s32Y = pstInfo->unSubtitleParam.stGfx.y;
       stSrcRect.s32Width = pstInfo->unSubtitleParam.stGfx.w;
       stSrcRect.s32Height = pstInfo->unSubtitleParam.stGfx.h;

       HIADP_SURFACE_ATTR_S pstAttr;
       pstAttr.enPixelFormat=HIADP_PF_8888;
       pstAttr.u32Width=SurfaceWidth;
       pstAttr.u32Height=SurfaceHeight;

       HIADP_OSD_SubDrawBitmap(s_hSurface,bitmapBuffer,&pstAttr,&stSrcRect);

#else
        (HI_VOID)HI_GO_UnlockSurface(s_hSubtSurface);

#if USED_SURFACE_TO_FILE
        HIGO_ENC_ATTR_S stEncAttr;
        stEncAttr.ExpectType = HIGO_IMGTYPE_BMP;
        stEncAttr.QualityLevel = 1;
        HI_GO_EncodeToFile(s_hSubtSurface, HI_NULL, &stEncAttr);
#endif

        rect.x = 0;
        rect.y = 0;
        rect.w = pstInfo->unSubtitleParam.stGfx.w;
        rect.h = pstInfo->unSubtitleParam.stGfx.h;
        (HI_VOID)ConvertRect(s_hSubtSurface, s_hLayerSurface, pstInfo->unSubtitleParam.stGfx.x, pstInfo->unSubtitleParam.stGfx.y,
                    pstInfo->unSubtitleParam.stGfx.w, pstInfo->unSubtitleParam.stGfx.h, &DstRect);

        memset(&stBlitOpt, 0, sizeof(HIGO_BLTOPT_S));
        stBlitOpt.EnableScale = HI_TRUE;
        (HI_VOID)HI_GO_Blit (s_hSubtSurface, &rect,
                       s_hLayerSurface, &DstRect,
                       &stBlitOpt);
        (HI_VOID)HI_GO_RefreshLayer(s_hLayer, &DstRect);
#endif

        break;

    case HI_UNF_SUBTITLE_TEXT:
        if (NULL == pstInfo->unSubtitleParam.stText.pu8Data)
        {
            return HI_FAILURE;
        }

        HI_INFO_SUBT("OUTPUT: %s \n", pstInfo->unSubtitleParam.stText.pu8Data);
#ifdef ANDROID
       stSrcRect.s32X = 0;
       stSrcRect.s32Y = 0;
       stSrcRect.s32Width = pstInfo->unSubtitleParam.stText.w;
       stSrcRect.s32Height = pstInfo->unSubtitleParam.stText.h;
       AndroidConvertRect( pstInfo->unSubtitleParam.stText.x, pstInfo->unSubtitleParam.stText.y,
                        pstInfo->unSubtitleParam.stText.w, pstInfo->unSubtitleParam.stText.h, &stDesRect);
       HI_CHAR* pszText = (HI_CHAR*)pstInfo->unSubtitleParam.stText.pu8Data;

       HIADP_CCTEXT_ATTR_S pstCCTextAttr;
       pstCCTextAttr.u32BufLen = strlen(pszText);
       pstCCTextAttr.u32bgColor = 0xff000000;
       pstCCTextAttr.u32fgColor = 0xffffffff;
       pstCCTextAttr.u8fontSize = 21;

        HIADP_SURFACE_ATTR_S pstSurAttr;
        pstSurAttr.enPixelFormat=HIADP_PF_8888;
        pstSurAttr.u32Width=ScreenWidth;
        pstSurAttr.u32Height=ScreenHeight;

        HIADP_OSD_DrawCCText(s_hSurface, &pstSurAttr,&stDesRect, (HI_CHAR*)pszText, &pstCCTextAttr);

#else
        rect.x = 0;
        rect.y = 0;
        rect.w = pstInfo->unSubtitleParam.stText.w;
        rect.h = pstInfo->unSubtitleParam.stText.h;
        (HI_VOID)ConvertRect(s_hSubtSurface, s_hLayerSurface, pstInfo->unSubtitleParam.stText.x, pstInfo->unSubtitleParam.stText.y,
                        pstInfo->unSubtitleParam.stText.w, pstInfo->unSubtitleParam.stText.h, &DstRect);

        if (HIGO_INVALID_HANDLE != s_hFont && HIGO_INVALID_HANDLE != s_hLayer)
        {
            HI_CHAR* pszText = (HI_CHAR*)pstInfo->unSubtitleParam.stText.pu8Data;

            (HI_VOID)HI_GO_TextOutEx(s_hFont, s_hSubtSurface, pszText, &rect,
                HIGO_LAYOUT_WRAP | HIGO_LAYOUT_HCENTER | HIGO_LAYOUT_BOTTOM);
            memset(&stBlitOpt, 0, sizeof(HIGO_BLTOPT_S));
            stBlitOpt.EnableScale = HI_TRUE;
            (HI_VOID)HI_GO_Blit (s_hSubtSurface, &rect,
                           s_hLayerSurface, &DstRect,
                           &stBlitOpt);
            (HI_VOID)HI_GO_RefreshLayer(s_hLayer, NULL);
        }
#endif

        break;

    case HI_UNF_SUBTITLE_ASS:
    default:
        break;
    }

    return HI_SUCCESS;
}

HI_S32 Subt_Output_OnClear(HI_VOID* pUserData, HI_VOID *pArg)
{
    HI_UNF_SO_CLEAR_PARAM_S* pParam = (HI_UNF_SO_CLEAR_PARAM_S*)pArg;

    HI_CHAR TextClear[] = "";

    HI_INFO_SUBT("CLEAR Subtitle!\n");

#ifdef ANDROID
    HI_RECT_S stSrcRect = {0, 0, 0, 0};
    HI_RECT_S stDesRect = {0, 0, 0, 0};
    AndroidConvertRect(pParam->x, pParam->y, pParam->w, pParam->h,&stDesRect);

    HIADP_SURFACE_ATTR_S    pstSurAttr;
    pstSurAttr.u32Width = ScreenWidth;
    pstSurAttr.u32Height = ScreenHeight;
    pstSurAttr.enPixelFormat = HIADP_PF_8888;

	if (stDesRect.s32Width == 0 || stDesRect.s32Height == 0)
    {
        stDesRect.s32X = 0 ;
        stDesRect.s32Y = 0 ;
        stDesRect.s32Width = ScreenWidth;
        stDesRect.s32Height = ScreenHeight;

        HIADP_OSD_FillCCRect(s_hSurface,&pstSurAttr, &stDesRect, 0x00000000);

    }
    else
    {
        HIADP_OSD_FillCCRect(s_hSurface, &pstSurAttr,&stDesRect, 0x00000000);
    }
#else
     HI_RECT rect = {0, 0, 0, 0};
    (HI_VOID)ConvertRect(s_hSubtSurface, s_hLayerSurface, pParam->x, pParam->y, pParam->w, pParam->h, &rect);
    if (rect.w == 0 || rect.h == 0)
    {
        (HI_VOID)HI_GO_FillRect(s_hLayerSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);
    }
    else
    {
        (HI_VOID)HI_GO_FillRect(s_hLayerSurface, &rect, 0x00000000, HIGO_COMPOPT_NONE);
    }

    if (HIGO_INVALID_HANDLE != s_hFont)
    {
        (HI_VOID)HI_GO_TextOutEx(s_hFont, s_hLayerSurface, TextClear, &rect,
            HIGO_LAYOUT_WRAP | HIGO_LAYOUT_HCENTER | HIGO_LAYOUT_BOTTOM);
    }
    (HI_VOID)HI_GO_RefreshLayer(s_hLayer, NULL);
#endif
    return HI_SUCCESS;
}

HI_S32 Subt_Output_Init(HI_HANDLE* phOut)
{

#ifdef ANDROID
     *phOut = SUBT_OUTPUT_HANDLE;
     return OsdInit();
#else
    if (HIGO_INVALID_HANDLE == s_hLayerSurface)
    {
        (HI_VOID)higoInit();
    }
    *phOut = SUBT_OUTPUT_HANDLE;
    return HI_SUCCESS;
#endif
}

HI_S32 Subt_Output_DeInit(HI_HANDLE hOut)
{
#ifdef ANDROID
     OsdDeInit();
     if(bitmapBuffer != HI_NULL)
     {
              free(bitmapBuffer);
              bitmapBuffer=HI_NULL;
      }
#else
    if (HIGO_INVALID_HANDLE != s_hLayerSurface)
    {
        (HI_VOID)higoDeInit();
    }
#endif
    return HI_SUCCESS;
}


