#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hi_unf_avplay.h"
#include "hi_debug.h"

#ifdef ANDROID
#include "hi_adp_osd.h"
#include "hi_unf_disp.h"
#else
#include "hi_go.h"
#endif

#include "sample_teletext_out.h"
#include "sample_teletext_font.h"
#include "sample_teletext_msg.h"

#define HI_FATAL_TTX(fmt...)      HI_FATAL_PRINT(HI_ID_TTX, fmt)
#define HI_ERR_TTX(fmt...)        HI_ERR_PRINT(HI_ID_TTX, fmt)
#define HI_WARN_TTX(fmt...)       HI_WARN_PRINT(HI_ID_TTX, fmt)
#define HI_INFO_TTX(fmt...)       HI_INFO_PRINT(HI_ID_TTX, fmt)

#define   TTX_OSD_XOFFSET (40)
#define   TTX_OSD_YOFFSET (20)
#define   TTX_OSD_PAGEWIDTH (1200)    /** The width in display buffer, not in srceen. The max value of buffer width */
#define   TTX_OSD_PAGEHEIGHT (680)     /** The height  in display buffer, not in srceen. The max value of buffer height*/

#define  TTX_PIXBYTE (4)      /** 4 byets per pixel*/

#define  SYN_USING_LOCALTIME (1)

static HI_U32 s_u32RowNum = 0;
static HI_U32 s_u32ColNum = 0;
static HI_HANDLE hAVPlayer = HI_NULL;
static HI_U8 * pu8FontAddr = HI_NULL, * pu8MemAddr = HI_NULL;

#ifdef ANDROID
#define ANDROID_COLOR2ARGB(a, c)  (a) = (c&0xff00ff00)|((c>>16)&0x000000ff)|((c<<16)&0x00ff0000)
#define HI_INVALID_HANDLE (0xffffffff)
static HI_HANDLE s_hSurface = HI_INVALID_HANDLE;
HI_U32 ScreenWidth = 1280;
HI_U32 ScreenHeight = 720;
#else
static HI_HANDLE hLayer = HI_NULL, hLayerSurface = HI_NULL, hMemSurface = HI_NULL, hFontSurface = HI_NULL;
#endif

static HI_S32    TTX_FileDec(HI_CHAR *pszFileName, HI_HANDLE *pSurface);

static HI_S32    TTX_GetCurPlayingPTS(HI_S64 *ps64CurrentPts);
static HI_VOID   TTX_DrawChar(HI_UNF_TTX_DRAWCAHR_S* pstParam);
static HI_VOID   TTX_FillRect(HI_UNF_TTX_FILLRECT_S* pstParam);
static HI_VOID TTX_DrawDRCS(HI_UNF_TTX_DRAWDRCSCHAR_S* pstParam);
static HI_VOID   TTX_ShowOsd(HI_UNF_TTX_REFRESHLAYER_S* pstParam);
static HI_VOID   TTX_SendGUIMsg(HI_HANDLE  * phDispalyHandle);
static HI_S32    TTX_Create_Buffer(HI_UNF_TTX_BUFFER_PARAM_S * pstBufferParam);
static HI_S32    TTX_Destroy_Buffer(HI_VOID);

/**  Get video pts, used to sync video and teletext subtitle */
static HI_S32  TTX_GetCurPlayingPTS(HI_S64 *ps64CurrentPts)
{
    HI_UNF_AVPLAY_STATUS_INFO_S stAVPlayer = {{0}};

    if (HI_NULL == hAVPlayer)
    {
        return HI_FAILURE;
    }

    if (HI_SUCCESS != HI_UNF_AVPLAY_GetStatusInfo(hAVPlayer, &stAVPlayer))
    {
        return HI_FAILURE;
    }

#if SYN_USING_LOCALTIME
    {
        *ps64CurrentPts = stAVPlayer.stSyncStatus.u32LocalTime;
    }
#else
    {
        *ps64CurrentPts = stAVPlayer.stSyncStatus.u32LastVidPts;
    }
#endif
    return HI_SUCCESS;
}

/**  Set handle of player , the  handle be used to get video pts */
HI_S32 SetAVplayHandle(HI_HANDLE hAVPlay)
{
    if (hAVPlay == HI_NULL)
    {
        return HI_FAILURE;
    }

    hAVPlayer = hAVPlay;
    return HI_SUCCESS;
}

/** Callback  function, register to ttx module. draw char ,refresh osd ,fill rectangle  ,get pts,  get localtime*/
HI_S32 TTX_SampleCallBack(HI_HANDLE hTTX, HI_UNF_TTX_CB_E enCB, HI_VOID *pvCBParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_TTX_FILLRECT_S *pstFillrectparam;
    HI_UNF_TTX_DRAWCAHR_S *pstDrawparam;
    HI_UNF_TTX_REFRESHLAYER_S *pstRefreshparam;
    HI_HANDLE hDispalyHandle;

    if ((HI_NULL == pvCBParam) || (enCB > HI_UNF_TTX_CB_BUTT))
    {
        return HI_FAILURE;
    }

    switch (enCB)
    {
    case HI_UNF_TTX_CB_APP_FILLRECT:
    {
        pstFillrectparam = (HI_UNF_TTX_FILLRECT_S *)pvCBParam;
        TTX_FillRect(pstFillrectparam);
        s32Ret = HI_SUCCESS;
    }
        break;
    case HI_UNF_TTX_CB_APP_DRAWCHAR:
    {
        pstDrawparam = (HI_UNF_TTX_DRAWCAHR_S *)pvCBParam;
        TTX_DrawChar(pstDrawparam);
        s32Ret = HI_SUCCESS;
    }
        break;
    case HI_UNF_TTX_CB_APP_REFRESH:
    {
        pstRefreshparam = (HI_UNF_TTX_REFRESHLAYER_S *)pvCBParam;
        TTX_ShowOsd(pstRefreshparam);
        s32Ret = HI_SUCCESS;
    }
        break;
    case HI_UNF_TTX_CB_GETPTS:
    {
        s32Ret = TTX_GetCurPlayingPTS((HI_S64 *)pvCBParam);
    }
        break;
    case HI_UNF_TTX_CB_TTX_TO_APP_MSG:
    {
        hDispalyHandle = *((HI_HANDLE  *)pvCBParam);
        TTX_SendGUIMsg(&hDispalyHandle);
        s32Ret = HI_SUCCESS;
    }
        break;
    case HI_UNF_TTX_CB_CREATE_BUFF:
    {
        s32Ret = TTX_Create_Buffer((HI_UNF_TTX_BUFFER_PARAM_S *)pvCBParam);
    }
        break;
    case HI_UNF_TTX_CB_DESTROY_BUFF:
    {
        s32Ret = TTX_Destroy_Buffer();
    }
        break;
    case HI_UNF_TTX_CB_APP_DRAWDRCSCHAR:
    {
       TTX_DrawDRCS((HI_UNF_TTX_DRAWDRCSCHAR_S*) pvCBParam);
       s32Ret = HI_SUCCESS;
    }

        break;
    default:
        break;
    }

    return s32Ret;
}

#ifdef ANDROID

HI_S32 getFileSize(FILE* f)
{
    long pos = ftell(f);
    fseek(f,0,SEEK_END);
    long len = ftell(f);
    fseek(f,pos,SEEK_SET);
    return len;
}

HI_S32 FontInit(HI_VOID)
{
    unsigned char *buffer;
    HI_U32 length;

    FILE* pfile = fopen("/system/ttx/font0.bin","rb");
    if(HI_NULL == pfile)
    {
        HI_ERR_TTX( "LoadFont: Load files failed for ttx font" );
        return HI_FAILURE;
    }
    length = getFileSize(pfile);
    buffer = (HI_U8*) malloc(length*sizeof(HI_U8));
    if (HI_NULL==buffer)
    {
        HI_ERR_TTX( "LoadFont: Memory alloc failed for ttx font " );
        return HI_FAILURE;
    }
    fread(buffer,1,length,pfile);

    pu8FontAddr = buffer;

    return HI_SUCCESS;
}

HI_VOID FontDeInit(HI_VOID)
{
    if(HI_NULL!=pu8FontAddr)
    {
        free(pu8FontAddr);
        pu8FontAddr = HI_NULL;
    }
}
#else
/** Init higo , used to show teletext */
HI_S32  Higo_Teletext_Init()
{
    HI_S32 s32Ret;
    HI_CHAR * aszFileName;
    HIGO_LAYER_INFO_S stLayerInfo;
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;

    HI_PIXELDATA stPixData;

    /**initial resource*/
    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR1;
    }

    s32Ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR1;
    }

    stLayerInfo.PixelFormat   = HIGO_PF_8888;
    //stLayerInfo.ScreenWidth   = 1920;   /**  the width of screen */
    //stLayerInfo.ScreenHeight  = 1080;   /**  the height of  srceen */
    //stLayerInfo.DisplayWidth  = 1280;  /**  the width of display buffer */
    //stLayerInfo.DisplayHeight = 720;    /** the height of  display buffer */

    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    /**create the graphic layer and get the handler */
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TTX("HI_GO_CreateLayer failed !\n");
        goto ERR1;
    }

    s32Ret = HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TTX("HI_GO_GetLayerSurface failed !\n");
        goto ERR2;
    }

    /**decoding*/
    aszFileName = "../res/png/font0.png";
    s32Ret = TTX_FileDec(aszFileName, &hFontSurface);
    if (HI_SUCCESS == s32Ret)
    {
        if (HI_SUCCESS != HI_GO_LockSurface(hFontSurface, stPixData, HI_TRUE))
        {
            goto ERR3;
        }

        pu8FontAddr = (HI_U8 *)stPixData[0].pData;
        if (HI_SUCCESS != HI_GO_UnlockSurface(hFontSurface))
        {
            goto ERR3;
        }

        return s32Ret;
    }
    else
    {
        /* if not support png decoding, read font data from font0.bin */
        HI_S32 s32File = 0;

        s32Ret = HI_GO_CreateSurface(1020, 160, HIGO_PF_8888, &hFontSurface);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TTX("failed to HI_GO_CreateSurface\n");
            goto ERR2;
        }
        if (HI_SUCCESS != HI_GO_LockSurface(hFontSurface, stPixData, HI_TRUE))
        {
            goto ERR3;
        }
        pu8FontAddr = (HI_U8 *)stPixData[0].pData;

        (HI_VOID)HI_GO_FillRect(hFontSurface, NULL, 0x00000000, HIGO_COMPOPT_NONE);
        s32File = open("../res/font/font0.bin", O_RDONLY, 0);
        if (-1 == s32File)
        {
            goto ERR3;
        }
        lseek(s32File, 0, s32File);
        read(s32File, pu8FontAddr, 1020 * 160 * 4);
        close(s32File);
        if (HI_SUCCESS != HI_GO_UnlockSurface(hFontSurface))
        {
            goto ERR3;
        }
        return s32Ret;
    }

ERR3:
    s32Ret |= HI_GO_FreeSurface(hFontSurface);
ERR2:
    s32Ret |= HI_GO_DestroyLayer(hLayer);
ERR1:
    s32Ret |= HI_GO_Deinit();

    HI_ERR_TTX("Higo_Teletext_Init failed !\n");
    return s32Ret;
}

/** Deinit higo  */
HI_VOID   Higo_Teletext_DeInit()
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (hLayer)
    {
        s32Ret &= HI_GO_DestroyLayer(hLayer);
    }

    s32Ret &= HI_GO_FreeSurface(hFontSurface);
    s32Ret &= HI_GO_Deinit();

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TTX("call Higo_Teletext_DeInit failed !\n");
    }

    return;
}

/** decode the picture of font to a layer  */
static HI_S32  TTX_FileDec(HI_CHAR *pszFileName, HI_HANDLE *pSurface)
{
    HI_S32 s32Ret;
    HI_HANDLE hDecoder;
    HIGO_DEC_ATTR_S stSrcDesc;

    /** create decode*/
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pszFileName;
    s32Ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    /** decode it to Surface */
    s32Ret  = HI_GO_DecImgData(hDecoder, 0, NULL, pSurface);
    s32Ret |= HI_GO_DestroyDecoder(hDecoder);
    return s32Ret;
}
#endif

static HI_BOOL  IsValidArea(HI_UNF_TTX_PAGEAREA_S * pstPageArea)
{
    if (HI_NULL == pstPageArea)
    {
        return HI_FALSE;
    }

    if ((pstPageArea->u32Row >= s_u32RowNum) || (pstPageArea->u32RowCount > s_u32RowNum))
    {
        return HI_FALSE;
    }

    if ((pstPageArea->u32Column >= s_u32ColNum) || (pstPageArea->u32ColumnCount > s_u32ColNum))
    {
        return HI_FALSE;
    }

    if ((pstPageArea->u32Row + pstPageArea->u32RowCount) > s_u32RowNum)
    {
        return HI_FALSE;
    }

    if ((pstPageArea->u32Column + pstPageArea->u32ColumnCount) > s_u32ColNum)
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

/** 画图原理**/

/** 图片中每个像素由4个字节组成，这个图片中的像素

有如下规律:  每个像素的最后一个字节，表示这个像素

应该填充前景色还是背景色，如果最后一个字节为255

则填充前景色，否则填充背景色。**/

/** 算法  */

/*1.获取图片缓存的地址和内存表面的地址。
2.根据起始地址，计算出图片缓存相应区域内每个像素的一维地址。
3.根据目的地址，计算内存表面中相应区域内每个像素的一维地址。
4.对于图片缓存相应区域内的每个像素，如果它的第四个字节为255，
   内存表面中相应的像素就 填充为前景色，否则填充为背景色。
5.如果需要放大字体，则内存相应像素的周围都需要填充为同样的颜色。*/

/** Draw a char in osd */
static HI_VOID TTX_DrawChar(HI_UNF_TTX_DRAWCAHR_S* pstParam)
{
    HI_BOOL bRedrawChar = HI_FALSE;
    HI_U32 i = 0, j = 0, k = 0, h = 0, m = 0, n = 0, u32Index,u32Color;
    HI_U32 u32SrcRow = 0, u32SrcColumn = 0;
    HI_U32 u32DesRow = 0, u32DesColumn = 0, u32DesRowCount = 0, u32DesColCount = 0;
    HI_U32 u32BytesPerRowInFont = 0;
    HI_U32 u32BytesPerRowInMem = 0;

    if (HI_NULL == pstParam)
    {
        HI_ERR_TTX("In DrawChar, Invalid param!\n");
        return;
    }

    if ((HI_NULL == pstParam->pstCharAttr) || !IsValidArea(pstParam->pstPageArea))
    {
        HI_ERR_TTX("In DrawChar, Invalid param !\n");
        return;
    }

    u32Index  = FindIndex(*(pstParam->pstCharAttr));
    u32SrcRow = u32Index % TTX_ROWOFFONT;
    u32SrcColumn = u32Index / TTX_ROWOFFONT;

    u32DesRow = pstParam->pstPageArea->u32Row;
    u32DesColumn   = pstParam->pstPageArea->u32Column;
    u32DesRowCount = pstParam->pstPageArea->u32RowCount;
    u32DesColCount = pstParam->pstPageArea->u32ColumnCount;

    u32BytesPerRowInFont = TTX_COLOFFONT * TTX_CHARWIDTH * TTX_PIXBYTE;
    u32BytesPerRowInMem = s_u32ColNum * TTX_CHARWIDTH * TTX_PIXBYTE;

    /**jude if it is Diacritical mark*/
    if((pstParam->pstCharAttr->enCharSet == HI_UNF_TTX_CHARSET_G2) && (pstParam->pstCharAttr->u32Index > 0x40) && (pstParam->pstCharAttr->u32Index < 0x50))
    {
        bRedrawChar = HI_TRUE;
    }
    else
    {
        bRedrawChar = HI_FALSE;
    }

    for (i = u32DesRow * TTX_CHARHEIGHT, n = u32SrcRow * TTX_CHARHEIGHT;
         i < (u32DesRow + u32DesRowCount) * TTX_CHARHEIGHT;  i += u32DesRowCount, n++)
    {
        for (j = u32DesColumn * TTX_CHARWIDTH, m = u32SrcColumn * TTX_CHARWIDTH;
             j < (u32DesColumn + u32DesColCount) * TTX_CHARWIDTH; j += u32DesColCount, m++)
        {
            for (k = 0; k < u32DesRowCount; k++)
            {
                for (h = 0; h < u32DesColCount; h++)
                {
                    if (*(pu8FontAddr + n * u32BytesPerRowInFont + m * TTX_PIXBYTE + 3) == 255)
                    {
#ifdef ANDROID
                        ANDROID_COLOR2ARGB(u32Color,pstParam->u32Foreground);
#else
                        u32Color = pstParam->u32Foreground;
#endif
                        *((HI_U32*)(pu8MemAddr + (i + k) * u32BytesPerRowInMem + (j + h) * TTX_PIXBYTE)) = u32Color;
                    }
                    else
                    {
                        if (!bRedrawChar)
                        {
#ifdef ANDROID
                            ANDROID_COLOR2ARGB(u32Color,pstParam->u32Background);
#else
                             u32Color = pstParam->u32Background;
#endif
                            *((HI_U32*)(pu8MemAddr + (i + k) * u32BytesPerRowInMem + (j + h) * TTX_PIXBYTE)) = u32Color;
                        }
                    }
                }
            }
        }
    }
}

/** 填充原理 */

/**  1.获取内存表面的虚拟地址
       2.计算出填充区域内每个像素的一维地址
       3.把区域内的每个像素都置为填充色
 */

/**  Use specify  color  to fill  a rectangle */
static HI_VOID TTX_FillRect(HI_UNF_TTX_FILLRECT_S* pstParam)
{
    HI_U32 i = 0, j = 0;
    HI_U32 u32Row = 0, u32Column = 0, u32RowCount = 0, u32ColCount = 0,u32Color;
    HI_U32 u32BytesPerRowInMem = 0;

    if ((HI_NULL == pstParam) || !IsValidArea(pstParam->pstPageArea))
    {
        HI_ERR_TTX("In FillRect, Invalid param!\n");

        return;
    }

    u32Row = pstParam->pstPageArea->u32Row;
    u32Column   = pstParam->pstPageArea->u32Column;
    u32RowCount = pstParam->pstPageArea->u32RowCount;
    u32ColCount = pstParam->pstPageArea->u32ColumnCount;
    u32BytesPerRowInMem = s_u32ColNum * TTX_CHARWIDTH * TTX_PIXBYTE;
    for (i = u32Row * TTX_CHARHEIGHT; i < (u32Row + u32RowCount) * TTX_CHARHEIGHT; i++)
    {
        for (j = u32Column * TTX_CHARWIDTH;  j < (u32Column + u32ColCount) * TTX_CHARWIDTH; j++)
        {
#ifdef ANDROID
               ANDROID_COLOR2ARGB(u32Color,pstParam->u32Color);
#else
               u32Color = pstParam->u32Color;
#endif
               *((HI_U32*)(pu8MemAddr + i * u32BytesPerRowInMem + j * TTX_PIXBYTE)) = u32Color;
        }
    }
    return;
}

/** 填充原理 */

/**  1.获取内存表面的虚拟地址
       2.计算出填充区域内每个像素的一维地址
       3.把区域内的每个像素都置为相应的颜色
 */

/**  Use specify  colors  to draw  a drcs */
static HI_VOID TTX_DrawDRCS(HI_UNF_TTX_DRAWDRCSCHAR_S* pstParam)
{
    HI_U32 i = 0, j = 0,m = 0, n = 0;
    HI_U32 u32Row = 0, u32Column = 0, u32RowCount = 0, u32ColCount = 0;
    HI_U32 u32BytesPerRowInMem = 0;

    if ((HI_NULL == pstParam) || !IsValidArea(pstParam->pstPageArea))
    {
        HI_ERR_TTX("In TTX_DrawDRCS, Invalid param!\n");

        return;
    }

    u32Row = pstParam->pstPageArea->u32Row;
    u32Column   = pstParam->pstPageArea->u32Column;
    u32RowCount = pstParam->pstPageArea->u32RowCount;
    u32ColCount = pstParam->pstPageArea->u32ColumnCount;
    u32BytesPerRowInMem = s_u32ColNum * TTX_CHARWIDTH * TTX_PIXBYTE;
    if(HI_UNF_TTX_DRCS_SIZE_NORMAL == pstParam->enDRCSSize)
    {
        for (i = u32Row * TTX_CHARHEIGHT, m = 0; i < (u32Row + u32RowCount) * TTX_CHARHEIGHT; i++, m = (m + 1 )/u32RowCount)
        {
            for (j = u32Column * TTX_CHARWIDTH, n = 0;  j < (u32Column + u32ColCount) * TTX_CHARWIDTH; j++, n = (n + 1)/u32ColCount)
            {
                *((HI_U32 *)(pu8MemAddr + i * u32BytesPerRowInMem + j * TTX_PIXBYTE)) = pstParam->pu32DRCSColorInfo[m * TTX_CHARWIDTH + j];
            }
        }
    }
    else
    {
        for (i = u32Row * TTX_CHARHEIGHT, m = 0; i < (u32Row + u32RowCount) * TTX_CHARHEIGHT; i++, m = (m + 1 )/u32RowCount)
        {
            for (j = u32Column * TTX_CHARWIDTH, n = 0;  j < (u32Column + u32ColCount) * TTX_CHARWIDTH; j++, n = (n + 1)/u32ColCount)
            {
                if(( m < TTX_CHARHEIGHT1) && (n < TTX_CHARWIDTH1))
                {
                    *((HI_U32 *)(pu8MemAddr + i * u32BytesPerRowInMem + j * TTX_PIXBYTE)) = pstParam->pu32DRCSColorInfo[m * TTX_CHARWIDTH + j];
                }
                else
                {
                    *((HI_U32 *)(pu8MemAddr + i * u32BytesPerRowInMem + j * TTX_PIXBYTE)) = pstParam->u32Background;
                }
            }
        }
    }

}


/** Refresh  osd  layer  */
static HI_VOID   TTX_ShowOsd(HI_UNF_TTX_REFRESHLAYER_S* pstParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32WidthPerCol;
    HI_U32 u32HeightPerRow;
#ifdef ANDROID
    HI_RECT_S stDesRect = {0};
    HI_RECT_S stSrcRect = {0};
#else
    HI_RECT stDesRect = {0};
    HI_RECT stSrcRect = {0};
    HI_PIXELDATA stPixData;
    HIGO_BLTOPT_S stBltOpt;

    memset(stPixData, 0, sizeof(stPixData));
    memset(&stBltOpt, 0, sizeof(stBltOpt));
    stBltOpt.EnableScale = HI_TRUE;
#endif
    if ((HI_NULL == pstParam) || !IsValidArea(pstParam->pstPageArea))
    {
        HI_ERR_TTX("In RefreshLayer, Invalid param!\n");

        return;
    }

    u32WidthPerCol  = TTX_OSD_PAGEWIDTH / s_u32ColNum;
    u32HeightPerRow = TTX_OSD_PAGEHEIGHT / s_u32RowNum;
#ifdef ANDROID

    stSrcRect.s32X = 0;
    stSrcRect.s32Y = 0;
    stSrcRect.s32Width = s_u32ColNum*TTX_CHARWIDTH;
    stSrcRect.s32Height = s_u32RowNum*TTX_CHARHEIGHT;

    stDesRect.s32X = 10;
    stDesRect.s32Y = 10;


    stDesRect.s32Width = ScreenWidth - stDesRect.s32X*2;
    stDesRect.s32Height = ScreenHeight - stDesRect.s32Y*2;

    HIADP_OSD_TeletextDrawBitmap(s_hSurface,pu8MemAddr,&stSrcRect,&stDesRect);
#else
    stSrcRect.x = pstParam->pstPageArea->u32Column * TTX_CHARWIDTH;
    stSrcRect.y = pstParam->pstPageArea->u32Row * TTX_CHARHEIGHT;
    stSrcRect.w = pstParam->pstPageArea->u32ColumnCount * TTX_CHARWIDTH;
    stSrcRect.h = pstParam->pstPageArea->u32RowCount * TTX_CHARHEIGHT;

    stDesRect.x = TTX_OSD_XOFFSET + pstParam->pstPageArea->u32Column * u32WidthPerCol;
    stDesRect.y = TTX_OSD_YOFFSET + pstParam->pstPageArea->u32Row * u32HeightPerRow;
    stDesRect.w = pstParam->pstPageArea->u32ColumnCount * u32WidthPerCol;
    stDesRect.h = pstParam->pstPageArea->u32RowCount * u32HeightPerRow;

    s32Ret &= HI_GO_Blit(hMemSurface, &stSrcRect, hLayerSurface, &stDesRect, &stBltOpt);
    s32Ret &= HI_GO_LockSurface(hLayer, stPixData, HI_TRUE);
    s32Ret &= HI_GO_RefreshLayer(hLayer, &stDesRect);
    s32Ret &= HI_GO_UnlockSurface(hLayer);
#endif

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TTX("TTX_ShowOsd failed !\n");
    }

    return;
}

static HI_VOID   TTX_SendGUIMsg(HI_HANDLE  * phDispalyHandle)
{
    HI_HANDLE hDispalyHandle;
    MESSAGE_S stMsg;

    if (phDispalyHandle == HI_NULL)
    {
        return;
    }

    hDispalyHandle = (*phDispalyHandle);

    stMsg.pu8MsgData   = (HI_U8 *) &hDispalyHandle;
    stMsg.u16MsgLength = sizeof(hDispalyHandle);
    if (HI_SUCCESS != MsgQueue_En(&stMsg))
    {
        HI_ERR_TTX("call MsgQueue_En failed !\n");
    }
}

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

#endif

static HI_S32 TTX_Create_Buffer(HI_UNF_TTX_BUFFER_PARAM_S* pstBufferParam)
{

    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pstBufferParam)
    {
        return HI_FAILURE;
    }

    s_u32RowNum = pstBufferParam->u32Row;
    s_u32ColNum = pstBufferParam->u32Column;

#ifdef ANDROID
    if (HI_NULL == pu8MemAddr)
    {
        pu8MemAddr = (HI_U8*)malloc(TTX_CHARWIDTH * s_u32ColNum*TTX_CHARHEIGHT * s_u32RowNum*TTX_PIXBYTE);
        if(HI_NULL == pu8MemAddr)
        {
                goto BEBACK;
        }
    }
#else
    HI_PIXELDATA stPixData;
    if (HI_SUCCESS
        != HI_GO_CreateSurface(TTX_CHARWIDTH * s_u32ColNum, TTX_CHARHEIGHT * s_u32RowNum, HIGO_PF_8888, &hMemSurface))
    {
        HI_ERR_TTX("HI_GO_CreateSurface failed ! row = %d, col = %d\n", s_u32RowNum, s_u32ColNum);
        s32Ret = HI_FAILURE;
        goto BEBACK;
    }

    if (HI_SUCCESS != HI_GO_LockSurface(hMemSurface, stPixData, HI_TRUE))
    {
        s32Ret &= HI_GO_FreeSurface(hMemSurface);
        s32Ret = HI_FAILURE;
        HI_ERR_TTX("HI_GO_LockSurface failed !\n");
        goto BEBACK;
    }

    pu8MemAddr = (HI_U8 *)stPixData[0].pData;
    if (HI_SUCCESS != HI_GO_UnlockSurface(hMemSurface))
    {
        s32Ret &= HI_GO_FreeSurface(hMemSurface);
        s32Ret = HI_FAILURE;
        HI_ERR_TTX("HI_GO_UnlockSurface failed !\n");
        goto BEBACK;
    }
#endif
    s32Ret = HI_SUCCESS;
    return HI_SUCCESS;

BEBACK:
    HI_ERR_TTX("TTX_Create_Buffer failed !\n");
    return HI_FAILURE;
}

static HI_S32 TTX_Destroy_Buffer()
{
#ifdef ANDROID
    free(pu8MemAddr);
    pu8MemAddr = HI_NULL;
#else
    if (0 == hMemSurface)
    {
        return HI_FAILURE;
    }

    if (HI_SUCCESS != HI_GO_FreeSurface(hMemSurface))
    {
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}
