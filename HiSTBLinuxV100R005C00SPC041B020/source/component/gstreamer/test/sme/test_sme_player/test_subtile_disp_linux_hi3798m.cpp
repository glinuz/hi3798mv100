/**< hdmi interface needs to be init in linux */
#if defined __LINUX__

#include "hi_go.h"
#include "osal_type.h"
#include "sme_macro.h"
#include "sme_log.h"
#include "osal_mem.h"
#include "string.h"
#include "test_subtile_disp.h"

//#define USE_CURSOR

typedef struct __tagHigoSubDisp
{
  HI_RECT stRectDft;//default
  HI_RECT stRectCur;
  HI_HANDLE hLayer;
  HI_HANDLE hLayerSurface;
  HI_HANDLE hFont;
  V_INT32 i32OffsetY;
  V_INT32 i32OffsetX;
  V_UINT64 u64LastId;
}ST_HIGO_SUB_DISP;

//<0 not find.
static V_INT32 FindLFInString(const V_CHAR* pszStr)
{
    V_INT32 i32Ret = -1;
    V_UINT32 u32Len = strlen(pszStr);
    V_UINT32 i = 0;

    for(i = 0; i < u32Len; i++)
    {
        if(pszStr[i] == '\n')
        {
            i32Ret = i;
            break;
        }
    }

    return i32Ret;
}

SUB_DISP_HDL Sub_Disp_Init(ST_SME_DISP_RECT stRect)
{
    ST_HIGO_SUB_DISP* pstThis = NULL;
    HI_S32 s32Ret = 0;
    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_1;
    HIGO_CURSOR_INFO_S stCursorInfo;

    pstThis = (ST_HIGO_SUB_DISP*)malloc(sizeof(ST_HIGO_SUB_DISP));
    if(NULL == pstThis)
    {
        printf("malloc failed!\n");
        goto ERR0;
    }

    VOS_Memset(pstThis, 0, sizeof(ST_HIGO_SUB_DISP));
    pstThis->hLayerSurface = HIGO_INVALID_HANDLE;
    pstThis->hFont = HIGO_INVALID_HANDLE;
    pstThis->hLayer = HIGO_INVALID_HANDLE;
    ICS_LOGW("sub rect:x:y:w:h=%d:%d:%d:%d",
        stRect.i32StartX, stRect.i32StartY, stRect.i32Width, stRect.i32Height);
    pstThis->stRectDft.x = stRect.i32StartX;
    pstThis->stRectDft.y = stRect.i32StartY;
    pstThis->stRectDft.w = stRect.i32Width;
    pstThis->stRectDft.h = stRect.i32Height;
    pstThis->stRectCur = pstThis->stRectDft;

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_Init FAILED\n");
        goto ERR1;
    }

#ifdef USE_CURSOR
    s32Ret = HI_GO_InitCursor();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_InitCursor\n");
        goto ERR2;
    }
#endif

    s32Ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
      printf("HI_GO_GetLayerDefaultParam FAILED\n");
        goto ERR3;
    }

    stLayerInfo.PixelFormat = HIGO_PF_8888;
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &pstThis->hLayer);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_CreateLayer\n");
        goto ERR3;
    }

    s32Ret =  HI_GO_GetLayerSurface(pstThis->hLayer, &pstThis->hLayerSurface);
    if  (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_GetLayerSurface\n");
        goto ERR4;
    }

    s32Ret = HI_GO_CreateText("/res/DroidSansFallbackLegacy.ttf", NULL,  &pstThis->hFont);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_CreateText\n");
        goto ERR4;
    }

    s32Ret = HI_GO_SetTextColor(pstThis->hFont, 0xffffffff);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_SetTextColor\n");
        goto ERR5;
    }
    s32Ret =  HI_GO_SetTextBGColor(pstThis->hFont, 0x00000000);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_SetTextBGColor failed:%#x\n", s32Ret);
    }

    s32Ret = HI_GO_SetInputCharSet(pstThis->hFont, HIGO_CHARSET_UTF8);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_SetInputCharSet\n");
        goto ERR5;
    }

#ifdef USE_CURSOR
    s32Ret =  HI_GO_GetCursorInfo(&stCursorInfo);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_GetCursorInfo, s32Ret=%#x\n", s32Ret);
    }
    else
    {
        printf("stCursorInfo,hCursor=%#x,x,y:[%d,%d]\n",
            stCursorInfo.hCursor, stCursorInfo.HotspotX, stCursorInfo.HotspotY);
    }

    s32Ret = HI_GO_SetCursorInfo(&stCursorInfo);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_SetCursorPos, s32Ret=%#x\n", s32Ret);
    }

    s32Ret = HI_GO_AttchCursorToLayer(pstThis->hLayer);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_AttchCursorToLayer, s32Ret=%#x\n", s32Ret);
        goto ERR5;
    }

    s32Ret = HI_GO_ShowCursor(HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_ShowCursor\n");
    }

    s32Ret = HI_GO_SetCursorPos(0, 0);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_SetCursorPos\n");
    }
#endif

    goto ERR0;

ERR5:
    HI_GO_DestroyText(pstThis->hFont);
ERR4:
    HI_GO_DestroyLayer(pstThis->hLayer);

ERR3:
#ifdef USE_CURSOR
    HI_GO_DeinitCursor();
#endif

ERR2:
    HI_GO_Deinit();
ERR1:
    UTILS_MSAFEFREE(pstThis);

ERR0:

    return pstThis;
}

V_VOID Sub_Disp_DeInit(SUB_DISP_HDL hdlSubDisp)
{
    ST_HIGO_SUB_DISP* pstThis = (ST_HIGO_SUB_DISP*)hdlSubDisp;

    UTILS_MLOGE_RET_IF(NULL == pstThis, ("pstThis is null"));

    if(HIGO_INVALID_HANDLE != pstThis->hFont)
    {
        HI_GO_DestroyText(pstThis->hFont);
        pstThis->hFont = HIGO_INVALID_HANDLE;
    }

    if(HIGO_INVALID_HANDLE != pstThis->hLayer)
    {
#ifdef USE_CURSOR
        HI_GO_DetachCursorFromLayer(pstThis->hLayer);
#endif
        HI_GO_DestroyLayer(pstThis->hLayer);
        pstThis->hLayer = HIGO_INVALID_HANDLE;
    }

#ifdef USE_CURSOR
    HI_GO_DeinitCursor();
#endif

    HI_GO_Deinit();

    free(pstThis);

    return;
}

HIGO_TEXT_STYLE_E SmeStyleToHiStyle(E_SME_SUBTITLE_TEXT_STYLE eSmeStyle)
{
    HIGO_TEXT_STYLE_E eHiStyle = HIGO_TEXT_STYLE_NORMAL;

    if(E_SME_SUBTITLE_TEXT_STYLE_ITALIC == eSmeStyle)
    {
        eHiStyle = HIGO_TEXT_STYLE_ITALIC;
    }
    else if (E_SME_SUBTITLE_TEXT_STYLE_BOLD == eSmeStyle)
    {
        eHiStyle = HIGO_TEXT_STYLE_BOLD;
    }
    else
    {
        ;//use default.
    }

    return eHiStyle;
}

V_UINT32 Sub_DispCb(IN V_VOID* pvCtx,  IN ST_SUB_INFO stSubInfo)
{
    ST_HIGO_SUB_DISP* pstThis = (ST_HIGO_SUB_DISP*)pvCtx;
    HI_S32 s32Ret = 0;
    HIGO_TEXT_STYLE_E eHiStyle = HIGO_TEXT_STYLE_NORMAL;
    HI_S32 Width=0, Height =0;
    V_INT32 i32Offset = 0;
    HI_RECT stRect ;
    V_CHAR* pszTmp = NULL;
    V_CHAR acToDraw[8192] = {0,};
    V_INT32 i32LineHeight = 28;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstThis || NULL == stSubInfo.U_SUB_INFO.stText.pu8SubData,
        ICS_FAIL, ("pstThis is null"));

    if(stSubInfo.U_SUB_INFO.stText.u64Id != pstThis->u64LastId
       || 0 == stSubInfo.U_SUB_INFO.stText.u64Id)
    {
        pstThis->u64LastId = stSubInfo.U_SUB_INFO.stText.u64Id;
        pstThis->i32OffsetX = 0;
        pstThis->i32OffsetY = 0;
    }

    s32Ret = HI_GO_SetTextColor(pstThis->hFont, (HI_COLOR)stSubInfo.U_SUB_INFO.stText.u32Color);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_SetTextColor, failed(%#x)\n", s32Ret);
    }

    if(E_SME_SUBTITLE_TEXT_STYLE_UNDERLINE == stSubInfo.U_SUB_INFO.stText.eStyle)
    {
        ICS_LOGE("underline text:%s", stSubInfo.U_SUB_INFO.stText.pu8SubData);
    }

    eHiStyle = SmeStyleToHiStyle(stSubInfo.U_SUB_INFO.stText.eStyle);
    s32Ret = HI_GO_SetTextStyle(pstThis->hFont, eHiStyle);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_SetTextStyle, failed(%#x)\n", s32Ret);
    }

    if(-1 != stSubInfo.U_SUB_INFO.stText.i32PosX && -1 != stSubInfo.U_SUB_INFO.stText.i32PosY)
    {
        pstThis->stRectCur.x = stSubInfo.U_SUB_INFO.stText.i32PosX;
        pstThis->stRectCur.y = stSubInfo.U_SUB_INFO.stText.i32PosY;
    }
    else
    {
         pstThis->stRectCur =  pstThis->stRectDft;
    }
    pszTmp = (V_CHAR*)stSubInfo.U_SUB_INFO.stText.pu8SubData;

    while(i32Offset >= 0 && strlen(pszTmp) > 0)
    {
        V_INT32 i32CurOffsetX = 0;
        V_INT32 i32CurOffsetY = 0;

        stRect = pstThis->stRectCur;
        VOS_Memset(acToDraw, 0, sizeof(acToDraw));
        i32Offset = FindLFInString(pszTmp);
        if(i32Offset < 0)
        {
            VOS_Memcpy(acToDraw, pszTmp, strlen(pszTmp));
            acToDraw[strlen(pszTmp)] = 0;
            pszTmp[0] = 0;
            s32Ret = HI_GO_GetTextExtent(pstThis->hFont, acToDraw, &Width, &Height);
            if (HI_SUCCESS != s32Ret)
            {
                printf("HI_GO_GetTextExtent failed(%#x)\n", s32Ret);
            }
            else
            {
                i32CurOffsetX = Width;
            }
        }
        else if(0 == i32Offset)
        {
            pszTmp++;
            //we must change line.
            pstThis->i32OffsetX = 0;
            pstThis->i32OffsetY += i32LineHeight;
            continue;
        }
        else
        {
            VOS_Memcpy(acToDraw, pszTmp, i32Offset);
            pszTmp += (i32Offset +1);
            acToDraw[i32Offset] = 0;

            s32Ret = HI_GO_GetTextExtent(pstThis->hFont, acToDraw, &Width, &Height);
            if (HI_SUCCESS != s32Ret)
            {
                printf("HI_GO_GetTextExtent failed(%#x)\n", s32Ret);
            }
            else
            {
                //printf("%s:w:h[%d,%d]\n", acToDraw, Width, Height);
                i32CurOffsetX = -pstThis->i32OffsetX;
                i32CurOffsetY = Height;
            }
        }


        stRect.x += pstThis->i32OffsetX;
        if(pstThis->stRectCur.w < pstThis->i32OffsetX + 16)
        {
            stRect.w = 16;
            stRect.x -= 16;
            printf("error width exceed!\n");
        }
        else
        {
            stRect.w = pstThis->stRectCur.w - pstThis->i32OffsetX;
        }

        stRect.y += pstThis->i32OffsetY;
        if(pstThis->stRectCur.h < pstThis->i32OffsetY + 8)
        {
            stRect.h = 8;
            stRect.y -= 8;
            printf("error width exceed!\n");
        }
        else
        {
            stRect.h = pstThis->stRectCur.h - pstThis->i32OffsetY;
        }

        s32Ret = HI_GO_TextOutEx(pstThis->hFont, pstThis->hLayerSurface,
            acToDraw, &stRect,
            (HIGO_LAYOUT_E)(HIGO_LAYOUT_LEFT | HIGO_LAYOUT_TOP | HIGO_LAYOUT_WRAP));
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_GO_TextOutEx failed,%#x\n", s32Ret);
        }

        s32Ret = HI_GO_RefreshLayer(pstThis->hLayer, &stRect);
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_GO_RefreshLayer failed\n");
        }
        pstThis->i32OffsetX += i32CurOffsetX;
        pstThis->i32OffsetY += i32CurOffsetY;
    }
#if 0
    s32Ret = HI_GO_TextOutEx(pstThis->hFont, pstThis->hLayerSurface,
        (const HI_CHAR*)stSubInfo.U_SUB_INFO.stText.pu8SubData, &stRect,
        (HIGO_LAYOUT_E)(HIGO_LAYOUT_LEFT | HIGO_LAYOUT_TOP | HIGO_LAYOUT_WRAP));
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_TextOutEx failed,%#x\n", s32Ret);
    }

    s32Ret = HI_GO_RefreshLayer(pstThis->hLayer, &pstThis->stRectCur);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_RefreshLayer failed\n");
    }

    if(stSubInfo.U_SUB_INFO.stText.u64Id)
    {
        s32Ret = HI_GO_GetTextExtent(pstThis->hFont,
            //(const HI_CHAR*)stSubInfo.U_SUB_INFO.stText.pu8SubData,
            "akdjflajfsafj;as\nasdfjlajflasjfasadjkaldjfaljdfaka;jdl\naslkdjflkasjf\naslkdfjsla",
            &Width, &Height);
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_GO_GetTextExtent failed\n");
        }
        else
        {
            printf("%s:w:h[%d,%d]\n", stSubInfo.U_SUB_INFO.stText.pu8SubData, Width, Height);
        }
    }
#endif
    return ICS_SUCCESS;
}

V_UINT32 Sub_ClearCb(IN V_VOID* pvCtx,  IN V_UINT64 u64DataId)
{

  ST_HIGO_SUB_DISP* pstThis = (ST_HIGO_SUB_DISP*)pvCtx;
  HI_S32 s32Ret = 0;

  UTILS_MLOGE_RET_VAL_IF(NULL == pstThis, ICS_FAIL, ("pstThis is null"));
  s32Ret = HI_GO_FillRect(pstThis->hLayerSurface, &pstThis->stRectCur, 0x00000000,
      HIGO_COMPOPT_NONE);
  if (HI_SUCCESS != s32Ret)
  {
      printf("HI_GO_FillRect failed,s32Ret=%#x\n",s32Ret);
  }

  (HI_VOID)HI_GO_TextOutEx(pstThis->hFont, pstThis->hLayerSurface, "", &pstThis->stRectCur,
      (HIGO_LAYOUT_E)(HIGO_LAYOUT_WRAP | HIGO_LAYOUT_VCENTER | HIGO_LAYOUT_BOTTOM));

  s32Ret = HI_GO_RefreshLayer(pstThis->hLayer, NULL);
  if (HI_SUCCESS != s32Ret)
  {
      printf("HI_GO_RefreshLayer failed\n");
  }

  return ICS_SUCCESS;
}

#endif
