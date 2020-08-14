/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: sample_dolby_win.c
 * Description:
 *       this sample finish the dolby windows drawn.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2011-07-11   184737
 ******************************************************************************/
#include <unistd.h>

#include "hi_go_gdev.h"
#include "hi_go_bliter.h"
#include "hi_adp_mpi.h"
#include "hi_go_text.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "dolby_win.h"
#include "dolby_ir.h"
#include "dolby_common.h"
#include "hi_go.h"

/*the handle of common Layer,Layersurface and Font*/
static HI_HANDLE s_hLayer, s_hLayerSurface, s_hFont = 0;

/* PixelFormat */
static HIGO_PF_E s_enPixelFormat = HIGO_PF_8888;

extern HI_U32 g_u32DolbyAcmod;
extern DOLBYPLUS_STREAM_INFO_S g_stDDpStreamInfo;
extern INPUT_KEY_MAP_S g_stIRMap[];

static HI_S32 DOLBY_ShowText(HI_VOID* pWindHandle, HI_CHAR *pCharBuffer, HI_U32 u32StartX, HI_U32 u32StartY, HI_U32 u32FontColor)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_RECT stRc = {0};
    WINDOW_S *pstWind = (WINDOW_S *)pWindHandle;

    if ((HI_NULL == pWindHandle) || (HI_NULL == pCharBuffer))
    {
        printf("input parameter invalid!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_GO_SetTextColor(s_hFont, u32FontColor);
    if (HI_SUCCESS != s32Ret)
    {
        printf("failed to HI_GO_SetTextColor\n");
        return HI_FAILURE;
    }

    stRc.x = u32StartX;
    stRc.y = u32StartY;
    s32Ret = HI_GO_GetTextExtent(s_hFont, pCharBuffer, &stRc.w, &stRc.h);
    if (HI_SUCCESS != s32Ret)
    {
        printf("failed to HI_GO_GetTextExtent\n");
        return HI_FAILURE;
    }

    s32Ret = HI_GO_TextOut(s_hFont, pstWind->hMemSurface, pCharBuffer, &stRc);
    if (HI_SUCCESS != s32Ret)
    {
        printf("%s  %x\n",pCharBuffer,s32Ret);
        printf("failed to HI_GO_TextOut\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 DOLBY_CreateWindMember(HI_VOID* pWindHandle, WMEMBTYPE_E enMembType,
                            POSITIONXY_S* pstMembPos, const HI_CHAR *pcDefString, HI_U8 u8MaxLength)
{
    WINDMEMBER_S *pstMemb = HI_NULL;
    WINDMEMBER_S *pstMembTmp = HI_NULL;
    WINDOW_S     *pstWind = (WINDOW_S *)pWindHandle;
    HI_CHAR      *pcStr = HI_NULL;

    /*params check*/
    if ((HI_NULL == pWindHandle) || (HI_NULL == pcDefString) || (0 == u8MaxLength))
    {
        printf("input params invalid!\n");
        return HI_FAILURE;
    }

    /* member position check */
    if ((pstMembPos->u32x > pstWind->u32WindLength) || (pstMembPos->u32y > pstWind->u32WindWidth))
    {
        printf("input position params invalid!\n");
        return HI_FAILURE;
    }

    /* alloc struct and input string mem */
    pstMemb = (WINDMEMBER_S *)malloc(sizeof(WINDMEMBER_S));
    if (HI_NULL == pstMemb)
    {
        printf("mem alloc fail!\n");
        return HI_FAILURE;
    }

    pcStr = (HI_CHAR *)malloc(sizeof(HI_U8) * (u8MaxLength + 1));  // +1 for \0
    if (HI_NULL == pcStr)
    {
        free(pstMemb);
        printf("mem alloc fail!\n");
        return (HI_U32)HI_NULL;
    }

    memset((HI_U8 *)pstMemb, 0, sizeof(WINDMEMBER_S));
    memset((HI_U8 *)pcStr, 0, sizeof(HI_U8) * (u8MaxLength + 1));

    /* init struct according to member type */
    pstMemb->pMembHandle = (HI_VOID*)pstMemb;
    pstMemb->enMembType = enMembType;
    /* first member */
    if (HI_NULL == pstWind->pstMemberList)
    {
        pstMemb->bIsOnFocus = HI_TRUE;

        /* set window focus to this member */
        pstWind->stFocusPos.u32x = pstMembPos->u32x;
        pstWind->stFocusPos.u32y = pstMembPos->u32y;
    }
    else
    {
        pstMemb->bIsOnFocus = HI_FALSE;
    }

    pstMemb->stMembPos.u32x = pstMembPos->u32x;
    pstMemb->stMembPos.u32y = pstMembPos->u32y;
    pstMemb->pstNext = HI_NULL;

    memcpy(pcStr, pcDefString, u8MaxLength);
    pstMemb->pcDefString = pcStr;
    if (APP_WMEMB_BUTTOM == enMembType)
    {
        pstMemb->u8TotalLen = u8MaxLength;
        pstMemb->u8SubFocusIndex = 0;
    }
    else if(APP_WMEMB_PASSWD == enMembType)
    {
        pstMemb->u8TotalLen = u8MaxLength;
        pstMemb->u8SubFocusIndex = 0;
        pstMemb->enQamNumber = QAM64;
        pstMemb->bIsOnEdit = HI_TRUE;
    }
    else
    {
        pstMemb->u8TotalLen = u8MaxLength;
        pstMemb->u8SubFocusIndex = (u8MaxLength - 1);
        pstMemb->enQamNumber = QAM64;
    }

    /* add to window member list */
    pstMembTmp = pstWind->pstMemberList;
    if (HI_NULL == pstMembTmp)  /* the first window member */
    {
        pstWind->pstMemberList = pstMemb;
    }
    else
    {
        while (1)
        {
            if (HI_NULL == pstMembTmp->pstNext)
            {
                pstMembTmp->pstNext = pstMemb;

                break;
            }

            pstMembTmp = pstMembTmp->pstNext;
        }
    }

    return HI_SUCCESS;
}


static HI_S32 DOLBY_BuildWindmeberList(HI_VOID* pWindHandle)
{
    WINDOW_S *pstWind = (WINDOW_S *)pWindHandle;
    WINDMEMBER_S *pstMembTmp = pstWind->pstMemberList;
    HI_S32 s32Ret;
    HI_RECT stRc={0};

    if (HI_NULL == pWindHandle)
    {
        printf("invalid window handle!\n");
        return HI_FAILURE;
    }

    while (HI_NULL != pstMembTmp)
    {
        if ((APP_WMEMB_BUTTOM  == pstMembTmp->enMembType) ||
            (APP_WMEMB_TRANS   == pstMembTmp->enMembType) ||
            (APP_WMEMB_TRANS_2 == pstMembTmp->enMembType))
        {
            if (HI_TRUE == pstMembTmp->bIsOnFocus)
            {
                HI_GO_SetTextColor(s_hFont, APP_TXT_FONTCOLOR_F);
                HI_GO_SetTextBGColor(s_hFont, APP_TXT_BGCOLOR_F);
            }
            else
            {
                HI_GO_SetTextColor(s_hFont, APP_TXT_FONTCOLOR);
                HI_GO_SetTextBGColor(s_hFont, APP_TXT_BGCOLOR);
            }
            stRc.x = pstMembTmp->stMembPos.u32x;
            stRc.y = pstMembTmp->stMembPos.u32y;
            stRc.w = 12 * pstMembTmp->u8TotalLen;
            stRc.h = GRPH_FONT_SIZE;

            s32Ret = HI_GO_FillRect(pstWind->hMemSurface, &stRc, YELLOW_COLOR, HIGO_COMPOPT_NONE);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_FillRect\n");
                 return HI_FAILURE;
            }

            s32Ret = HI_GO_GetTextExtent(s_hFont, pstMembTmp->pcDefString, &stRc.w, &stRc.h);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_GetTextExtent\n");
                 return HI_FAILURE;
            }

            s32Ret = HI_GO_TextOut(s_hFont, pstWind->hMemSurface, pstMembTmp->pcDefString, &stRc);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_TextOut\n");
                 return HI_FAILURE;
            }
        }

        if (APP_WMEMB_INPUT == pstMembTmp->enMembType)
        {
            if (HI_TRUE == pstMembTmp->bIsOnFocus)
            {
                HI_GO_SetTextColor(s_hFont, APP_TXT_FONTCOLOR_F);
                HI_GO_SetTextBGColor(s_hFont, APP_TXT_BGCOLOR_F);
            }
            else
            {
                HI_GO_SetTextColor(s_hFont, APP_TXT_FONTCOLOR);
                HI_GO_SetTextBGColor(s_hFont, APP_TXT_BGCOLOR);
            }

            stRc.x = pstMembTmp->stMembPos.u32x;
            stRc.y = pstMembTmp->stMembPos.u32y;

            stRc.w = 16 * pstMembTmp->u8TotalLen;
            stRc.h = GRPH_FONT_SIZE;

            s32Ret = HI_GO_FillRect(pstWind->hMemSurface, &stRc, YELLOW_COLOR, HIGO_COMPOPT_NONE);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_FillRect\n");
                 return HI_FAILURE;
            }

            s32Ret = HI_GO_GetTextExtent(s_hFont, pstMembTmp->pcDefString, &stRc.w, &stRc.h);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_GetTextExtent\n");
                 return HI_FAILURE;
            }

            s32Ret = HI_GO_TextOut(s_hFont, pstWind->hMemSurface, pstMembTmp->pcDefString, &stRc);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_TextOut\n");
                 return HI_FAILURE;
            }

            if (HI_TRUE == pstMembTmp->bIsOnEdit)
            {
                /* add cursor */
                stRc.x = pstMembTmp->stMembPos.u32x + pstMembTmp->u8SubFocusIndex * 11;
                stRc.y = pstMembTmp->stMembPos.u32y + 22;
                stRc.w = 10;
                stRc.h = 2;
                s32Ret = HI_GO_FillRect(pstWind->hMemSurface, &stRc, 0xFF888888, HIGO_COMPOPT_NONE);
                if (HI_SUCCESS != s32Ret)
                {
                     printf("failed to HI_GO_FillRect\n");
                     return HI_FAILURE;
                }
            }
        }

        if (APP_WMEMB_PASSWD == pstMembTmp->enMembType)
        {
            if (HI_TRUE == pstMembTmp->bIsOnFocus)
            {
                HI_GO_SetTextColor(s_hFont, APP_TXT_FONTCOLOR_F);
                HI_GO_SetTextBGColor(s_hFont, APP_TXT_BGCOLOR_F);
            }
            else
            {
                HI_GO_SetTextColor(s_hFont, APP_TXT_FONTCOLOR);
                HI_GO_SetTextBGColor(s_hFont, APP_TXT_BGCOLOR);
            }

            stRc.x = pstMembTmp->stMembPos.u32x;
            stRc.y = pstMembTmp->stMembPos.u32y;

            stRc.w = 16 * pstMembTmp->u8TotalLen;
            stRc.h = GRPH_FONT_SIZE;

            s32Ret = HI_GO_FillRect(pstWind->hMemSurface, &stRc, YELLOW_COLOR, HIGO_COMPOPT_NONE);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_FillRect\n");
                 return HI_FAILURE;
            }

            s32Ret = HI_GO_GetTextExtent(s_hFont, pstMembTmp->pcDefString, &stRc.w, &stRc.h);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_GetTextExtent\n");
                 return HI_FAILURE;
            }
            s32Ret = HI_GO_TextOut(s_hFont, pstWind->hMemSurface, pstMembTmp->pcDefString, &stRc);
            if (HI_SUCCESS != s32Ret)
            {
                 printf("failed to HI_GO_TextOut\n");
                 return HI_FAILURE;
            }

            if (HI_TRUE == pstMembTmp->bIsOnEdit)
            {
                /* add cursor */
                stRc.x = pstMembTmp->stMembPos.u32x + pstMembTmp->u8SubFocusIndex * 11 + pstMembTmp->u8SubFocusIndex/3;
                stRc.y = pstMembTmp->stMembPos.u32y + 22;
                stRc.w = 10;
                stRc.h = 2;
                s32Ret = HI_GO_FillRect(pstWind->hMemSurface,&stRc,0xFF888888, HIGO_COMPOPT_NONE);
                if (HI_SUCCESS != s32Ret)
                {
                     printf("failed to HI_GO_FillRect\n");
                     return HI_FAILURE;
                }
            }

        }

        pstMembTmp = pstMembTmp->pstNext;
    }

    return HI_SUCCESS;
}

static HI_S32 DOLBY_PasteWind(HI_VOID* pWindHandle, HI_HANDLE hSurface)
{
    WINDOW_S *pstWind  = HI_NULL;
    HI_S32 s32Ret ;
    //HI_RECT stRc={0};
    HIGO_BLTOPT_S stBltOpt ={0};

    if (HI_NULL == pWindHandle)
    {
        printf("window handle or pRegion invalid!\n");
        return HI_FAILURE;
    }

    pstWind = (WINDOW_S *)pWindHandle;

    //stRc.x = pstWind->stWindPos.u32x;
    //stRc.y = pstWind->stWindPos.u32y;
    //stRc.w = pstWind->u32WindLength;
    //stRc.h = pstWind->u32WindWidth;

    s32Ret = HI_GO_Blit(pstWind->hMemSurface, NULL, hSurface, NULL, &stBltOpt);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_DOLBY_BuildAndPasteWind(HI_VOID* pWindHandle)
{
    HI_S32 s32Ret = 0;

    s32Ret = DOLBY_BuildWindmeberList(pWindHandle);
    if (HI_SUCCESS != s32Ret)
    {
        printf("WIN_BuildWindPic fail!\n");
        return HI_FAILURE;
    }

    s32Ret = DOLBY_PasteWind(pWindHandle, s_hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        printf("dolby_PasteWind fail!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HI_DOLBY_ShowWind(HI_VOID)
{
    HI_S32 s32Ret = 0;

    s32Ret = HI_GO_RefreshLayer(s_hLayer, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_GO_RefreshLayer\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 HI_DOLBY_CreatLayer(HI_VOID)
{
    HI_S32 s32Ret = 0;
    HIGO_LAYER_INFO_S stLayerInfo;

    s32Ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GO_Deinit();
        return HI_FAILURE;
    }
    stLayerInfo.ScreenWidth = LAYER_WIDTH;
    stLayerInfo.ScreenHeight = LAYER_HEIGHT;
    stLayerInfo.CanvasWidth = LAYER_WIDTH;
    stLayerInfo.CanvasHeight = LAYER_HEIGHT;
    stLayerInfo.DisplayWidth = LAYER_WIDTH;
    stLayerInfo.DisplayHeight = LAYER_HEIGHT;
    //stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;//HIGO_LAYER_FLUSH_NORMAL;
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    stLayerInfo.AntiLevel = HIGO_LAYER_DEFLICKER_AUTO;//HIGO_LAYER_DEFLICKER_HIGH;
    stLayerInfo.PixelFormat = s_enPixelFormat;
    stLayerInfo.LayerID = HIGO_LAYER_HD_0;

    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &s_hLayer);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GO_Deinit();
        printf("failed to create the layer sd 0, ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret =  HI_GO_GetLayerSurface(s_hLayer, &s_hLayerSurface);
    if  (HI_SUCCESS != s32Ret)
    {
        HI_GO_DestroyLayer(s_hLayer);
        HI_GO_Deinit();
        printf("failed to get layer surface! s32Ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }
    else
    {
        printf("GetLayerSurface success! hLayerSurface=%d\n", s_hLayerSurface);
    }

    s32Ret = HI_GO_FillRect(s_hLayerSurface,NULL,WHITE_COLOR,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
       printf("HI_GO_FillRect failed!\n");
       HI_GO_DestroyLayer(s_hLayer);
       HI_GO_Deinit();
       HIADP_Disp_DeInit();
       return HI_FAILURE;
    }

    s32Ret = HI_GO_RefreshLayer(s_hLayer,NULL);
    if (HI_SUCCESS != s32Ret)
    {
       printf("++++ HI_GO_RefreshLayer failed! s_hLayerSurface %d ret 0x%x\n",s_hLayerSurface,s32Ret);
       HI_GO_DestroyLayer(s_hLayer);
       HI_GO_Deinit();
       HIADP_Disp_DeInit();
       return HI_FAILURE;
    }

    s32Ret = HI_GO_CreateText(HI_NULL, "../res/font/DroidSansFallbackLegacy.ttf", &s_hFont);
    if (HI_SUCCESS != s32Ret)
    {
       printf("HI_GO_CreateText failed!\n");
       HI_GO_DestroyLayer(s_hLayer);
       HI_GO_Deinit();
       HIADP_Disp_DeInit();
       return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID* HI_DOLBY_CreateSurface(POSITIONXY_S* pstWindPos, HI_U32 u32Length, HI_U32 u32Width, WINDTYPE_E enWindType)
{
    WINDOW_S *pstWind = HI_NULL;
    HI_HANDLE hMemSurface = 0;
    HI_S32 u32Ret = 0;

    u32Ret = HI_GO_CreateSurface(u32Length, u32Width, s_enPixelFormat, &hMemSurface);
    if (HI_SUCCESS != u32Ret)
    {
       printf("%s->%d\n", __FUNCTION__, __LINE__);
       return HI_NULL;
    }

    /* mem alloc for window struct and window pic */
    pstWind = (WINDOW_S *)malloc(sizeof(WINDOW_S));
    if (HI_NULL == pstWind)
    {
        printf("wind create fail!\n");
        return HI_NULL;
    }
    memset((HI_U8 *)pstWind, 0, sizeof(WINDOW_S));

    /* init window struct */
    pstWind->pWindHandle = (HI_VOID*)pstWind;
    pstWind->enWindType   = enWindType;
    pstWind->stWindPos.u32x  = pstWindPos->u32x;
    pstWind->stWindPos.u32y  = pstWindPos->u32y;
    pstWind->u32WindLength = u32Length;
    pstWind->u32WindWidth  = u32Width;
    pstWind->hMemSurface = hMemSurface;
    pstWind->stFocusPos.u32x  = 0;
    pstWind->stFocusPos.u32y  = 0;
    pstWind->pstMemberList = HI_NULL;

    /* draw rect in memory surface */
    u32Ret = HI_GO_FillRect(hMemSurface, HI_NULL, WHITE_COLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != u32Ret)
    {
       HI_GO_FreeSurface(hMemSurface);
       return HI_NULL;
    }
    printf("pstWind->hMemSurface = 0x%x, %s, %d\n",hMemSurface,__FILE__,__LINE__);

    return pstWind->pWindHandle;
}

HI_VOID HI_DOLBY_DestroyWindMemberList(HI_VOID* pWindHandle)
{
    WINDOW_S     *pstWindTemp = HI_NULL;
    WINDMEMBER_S *pstWinMemTemp  = HI_NULL;
    WINDMEMBER_S *pstWinMemTemp2 = HI_NULL;
    HI_S32 i = 0;

    pstWindTemp   = (WINDOW_S *)pWindHandle;
    pstWinMemTemp = pstWindTemp->pstMemberList;
    if(HI_NULL != pstWindTemp->pstMemberList)
    {
        for (i = 0; i < MAX_MEMBER_LIST; i++)
        {
            free(pstWinMemTemp->pcDefString);
            pstWinMemTemp2 = pstWinMemTemp;
            pstWinMemTemp = pstWinMemTemp->pstNext;
            if (NULL == pstWinMemTemp)
            {
                break;
            }
            free(pstWinMemTemp2);
        }

        pstWindTemp->pstMemberList = HI_NULL;
    }
}

HI_S32 HI_DOLBY_CreatPrgSearch_Wind(HI_VOID* pWindHandle,PROGSEARCH_S *pstProgSearch)
{
    HI_S32  s32Ret = 0;
    HI_CHAR u8TmpBuf[7] = {0};
    POSITIONXY_S stPos  = {0};
    HI_U32 u32Freq = 0;
    HI_U16 u16symborate = 0;
    HI_CHAR  *acQamStr;
    //HI_CHAR  *acTunerStr;

    Hi_Dolby_Debug();

    WINDOW_S *pstWind = (WINDOW_S *)pWindHandle;

    s32Ret = HI_GO_FillRect(pstWind->hMemSurface, NULL, WHITE_COLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_GO_FillRect\n");
    }

    u32Freq = pstProgSearch->u32Freq;
    u16symborate = pstProgSearch->u16symborate;
    acQamStr = pstProgSearch->acQam;
    //acTunerStr = pstProgSearch->acTuner;

    (HI_VOID)DOLBY_ShowText(pWindHandle, DOLBY_L_TILE_SEARCHSTYLE, 50, 30, BACK_COLOR);

    (HI_VOID)DOLBY_ShowText(pWindHandle, DOLBY_L_TITLE_FREQ, 100, 100, BACK_COLOR);

    (HI_VOID)DOLBY_ShowText(pWindHandle, DOLBY_L_TITLE_SYM_RATE, 100, 200, BACK_COLOR);

    (HI_VOID)DOLBY_ShowText(pWindHandle, DOLBY_L_TITLE_MODULATE, 100, 300, BACK_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, " <|", 270, 300, BACK_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, " |>", 380, 300, BACK_COLOR);

    stPos.u32x =300;
    stPos.u32y = 100;
    memset(u8TmpBuf, 0, 7);
    sprintf(u8TmpBuf, "%03d", u32Freq);
    (HI_VOID)DOLBY_CreateWindMember(pWindHandle, APP_WMEMB_INPUT, &stPos, u8TmpBuf, 3);

    stPos.u32x = 300;
    stPos.u32y = 200;
    memset(u8TmpBuf, 0, 7);
    sprintf(u8TmpBuf, "%04d", u16symborate);
    (HI_VOID)DOLBY_CreateWindMember(pWindHandle, APP_WMEMB_INPUT, &stPos, u8TmpBuf, 4);

    stPos.u32x = 300;
    stPos.u32y = 300;
    (HI_VOID)DOLBY_CreateWindMember(pWindHandle, APP_WMEMB_TRANS, &stPos, acQamStr, 7);

    stPos.u32x = 400;
    stPos.u32y = 500;
    (HI_VOID)DOLBY_CreateWindMember(pWindHandle, APP_WMEMB_BUTTOM, &stPos, DOLBY_L_BUTTON_START, 9);

    /*add for window display*/
    s32Ret = HI_DOLBY_BuildAndPasteWind(pWindHandle);
    if(HI_SUCCESS != s32Ret)
    {
        printf("call HI_DOLBY_BuildAndPasteWind faild!\n");
        return HI_FAILURE;
    }

    HI_GO_SetLayerAlpha(s_hLayer,255);
    /* update window */
    s32Ret = HI_DOLBY_ShowWind();
    if(HI_SUCCESS != s32Ret)
    {
        printf("call HI_DOLBY_ShowWind faild!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HI_DOLBY_HidePrgSearch_Wind(HI_VOID* pWindHandle)
{
    WINDOW_S *pstWind = (WINDOW_S *)pWindHandle;
    HI_S32 s32Ret = 0;

    s32Ret = HI_GO_FillRect(pstWind->hMemSurface, NULL,0, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_GO_FillRect\n");
         return HI_FAILURE;
    }

    HI_DOLBY_DestroyWindMemberList(pWindHandle);

    s32Ret = HI_DOLBY_BuildAndPasteWind(pWindHandle);
    if (HI_SUCCESS != s32Ret)
    {
        printf("dolby_PasteWind fail!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_DOLBY_ShowWind();
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_DOLBY_ShowWind!\n");
         return HI_FAILURE;
    }
    HI_GO_SetLayerAlpha(s_hLayer,70);
    return HI_SUCCESS;
}


HI_S32 HI_DOLBY_ClearPrgSearch_Wind(HI_VOID* pWindHandle)
{
    WINDOW_S *pstWind = (WINDOW_S *)pWindHandle;
    HI_S32 s32Ret = 0;

    s32Ret = HI_GO_FillRect(pstWind->hMemSurface, NULL, WHITE_COLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_GO_FillRect\n");
         return HI_FAILURE;
    }

    HI_DOLBY_DestroyWindMemberList(pWindHandle);

    s32Ret = HI_DOLBY_BuildAndPasteWind(pWindHandle);
    if (HI_SUCCESS != s32Ret)
    {
        printf("dolby_PasteWind fail!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_DOLBY_ShowWind();
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_DOLBY_ShowWind!\n");
         return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 DOLBY_DrawChnBar(HI_VOID* pWindHandle,CHNBAR_S* pstChnbar)
{
    WINDOW_S  *pstWind = (WINDOW_S *)pWindHandle;
    HI_CHAR   u8VolBuf[7] = {0};
    HI_S32    s32Ret = 0;
    HI_RECT   stRc={0};
    HI_CHAR   acAudioTypeStr[ARRAY_MAX_SIZE];
    HI_CHAR   *pcTrackModeString[] = {
        "Track_Mode: STEREO",
        "Track_Mode: DOUBLE_MONO",
        "Track_Mode: DOUBLE_LEFT",
        "Track_Mode: DOUBLE_RIGHT",
        "Track_Mode: EXCHANGE",
        "Track_Mode: ONLY_RIGHT",
        "Track_Mode: ONLY_LEFT",
        "Track_Mode: MUTED",
        "Track_Mode: BUTT"
    };

    stRc.x = 0;
    stRc.y = 645;
    stRc.w = 1280;
    stRc.h = 70;

    s32Ret = HI_GO_FillRect(pstWind->hMemSurface, &stRc, BLUE_COLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ChannelBar failed to HI_GO_FillRect\n");
        return HI_FAILURE;
    }

#if 1
    /* display audio type */
    //pcAudioString = HI_DOLBY_GetAudioType();
    strcpy(acAudioTypeStr,HI_DOLBY_GetAudioType());

    if(HI_SUCCESS == strcmp("DOLBY",acAudioTypeStr))
    {
        if(0 == g_stDDpStreamInfo.s16StreamType)
        {
            strcpy(pstChnbar->acAduioTypeString,"Dolby Digital");
        }
        else
        {
            strcpy(pstChnbar->acAduioTypeString , "Dolby Digital Plus");
        }
    }
    else
    {
        strcpy(pstChnbar->acAduioTypeString,acAudioTypeStr);
    }

    (HI_VOID)DOLBY_ShowText(pWindHandle, "Audio: ", 50, 660, WHITE_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, pstChnbar->acAduioTypeString, 130, 660, WHITE_COLOR);
#endif
    /* display mono type */
    strcpy(pstChnbar->acMonoString , HI_DOLBY_GetMonoType(g_u32DolbyAcmod));
    (HI_VOID)DOLBY_ShowText(pWindHandle, "Mono:", 340, 660, WHITE_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, pstChnbar->acMonoString, 410, 660, WHITE_COLOR);

    /* display mono track */
    (HI_VOID)DOLBY_ShowText(pWindHandle, pcTrackModeString[pstChnbar->enTrackMode], 500, 660, WHITE_COLOR);

    /* display volume */
    (HI_VOID)DOLBY_ShowText(pWindHandle, "Volume:", 850, 660, WHITE_COLOR);
    memset(u8VolBuf, 0, 7);
    sprintf(u8VolBuf, "%03d", pstChnbar->u32Volume);
    (HI_VOID)DOLBY_ShowText(pWindHandle, u8VolBuf, 940, 660, WHITE_COLOR);

#if 1
    /* display video type */
    strcpy(pstChnbar->acVideoTypeString , HI_DOLBY_GetVideoType());
    (HI_VOID)DOLBY_ShowText(pWindHandle, "Video:", 1050, 660, WHITE_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, pstChnbar->acVideoTypeString, 1120, 660, WHITE_COLOR);
#endif
    /* update window */
    s32Ret = DOLBY_PasteWind(pWindHandle, s_hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ChannelBar dolby_PasteWind fail!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_DOLBY_ShowWind();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_DOLBY_ShowWind fail!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HI_DOLBY_HideChnBar(HI_VOID* pWindHandle)
{
    WINDOW_S  *pstWind = (WINDOW_S *)pWindHandle;
    HI_S32    s32Ret = 0;
    HI_RECT   stRc = {0};

    stRc.x = 0;
    stRc.y = 645;
    stRc.w = 1280;
    stRc.h = 70;

#if 1
    s32Ret = HI_GO_FillRect(pstWind->hMemSurface, &stRc, 0, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ChannelBar failed to HI_GO_FillRect\n");
        return HI_FAILURE;
    }
#endif

    /* update window */
    s32Ret = DOLBY_PasteWind(pWindHandle, s_hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ChannelBar dolby_PasteWind fail!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_DOLBY_ShowWind();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_DOLBY_ShowWind fail!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


HI_S32 HI_DOLBY_DispChnBarWin(HI_VOID* pWindHandle, CHNBAR_S* pstChnbar)
{
    HI_S32    s32Ret = 0;

    s32Ret = DOLBY_DrawChnBar(pWindHandle, pstChnbar);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_DOLBY_DrawChnBar fail!\n");
        return HI_FAILURE;
    }

#if 0
    if(*pbHideChanBar)
    {
        usleep(100 * 10000);
        DOLBY_HideChnBar(pWindHandle);
    }
    *pbHideChanBar =  HI_TRUE;
#endif
    return HI_SUCCESS;
}

HI_S32 DOLBY_DispChnNumWin(HI_VOID* pWindHandle, HI_S32 s32ProgNum)
{
    WINDOW_S  *pstWind = (WINDOW_S *)pWindHandle;
    HI_CHAR   u8VolBuf[7] = {0};
    HI_S32    s32Ret = 0;
    HI_RECT   stRc = {0};
    //HI_CHAR   acAudioTypeStr[ARRAY_MAX_SIZE];

    HI_GO_ShowLayer(s_hLayer, HI_TRUE);

    stRc.x = 20;
    stRc.y = 30;
    stRc.w = 100;
    stRc.h = 60;

    s32Ret = HI_GO_FillRect(pstWind->hMemSurface, &stRc, BLUE_COLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ChannelBar failed to HI_GO_FillRect\n");
        return HI_FAILURE;
    }

    /* display program ID */
    memset(u8VolBuf, 0, 7);
    sprintf(u8VolBuf, "%03d", s32ProgNum);
    (HI_VOID)DOLBY_ShowText(pWindHandle, u8VolBuf, 60, 40, WHITE_COLOR);

    /* update window */
    s32Ret = DOLBY_PasteWind(pWindHandle, s_hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ChannelBar dolby_PasteWind fail!\n");
        return HI_FAILURE;
    }
    HI_GO_SetLayerAlpha(s_hLayer,255);
    s32Ret = HI_DOLBY_ShowWind();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_DOLBY_ShowWind fail!\n");
        return HI_FAILURE;
    }

    usleep(100* 5000);

    s32Ret = HI_GO_FillRect(pstWind->hMemSurface, &stRc, 0, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ChannelBar failed to HI_GO_FillRect\n");
        return HI_FAILURE;
    }

    /* update window */
    DOLBY_PasteWind(pWindHandle, s_hLayerSurface);
    HI_GO_SetLayerAlpha(s_hLayer,70);
    HI_DOLBY_ShowWind();
    return HI_SUCCESS;
}

HI_VOID HI_DOLBY_CreatPassThrough_WIN(HI_VOID* pWindHandle, PASSTHROUGH_S* pstPassProp)
{
    POSITIONXY_S stPos  = {0};
    HI_RECT stRc = {0};
    HI_S32 s32Ret = 0;

    WINDOW_S *pstWind = (WINDOW_S *)pWindHandle;

    stRc.x = 350;
    stRc.y = 70;
    stRc.w = 450;
    stRc.h = 400;
    s32Ret = HI_GO_FillRect(pstWind->hMemSurface,&stRc,WHITE_COLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_GO_FillRect\n");
    }

    (HI_VOID)DOLBY_ShowText(pWindHandle, "config pass through:", 400, 100, BACK_COLOR);

    (HI_VOID)DOLBY_ShowText(pWindHandle, "output port", 430, 200, BACK_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, " <|", 550, 200, BACK_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, " |>", 730, 200, BACK_COLOR);

    (HI_VOID)DOLBY_ShowText(pWindHandle, "audio type:", 430, 300, BACK_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, " <|", 550, 300, BACK_COLOR);
    (HI_VOID)DOLBY_ShowText(pWindHandle, " |>", 730, 300, BACK_COLOR);

    stPos.u32x = 580;
    stPos.u32y = 200;
    (HI_VOID)DOLBY_CreateWindMember(pWindHandle, APP_WMEMB_TRANS, &stPos, pstPassProp->acAOPort0, 13);

    stPos.u32x = 580;
    stPos.u32y = 300;
    (HI_VOID)DOLBY_CreateWindMember(pWindHandle, APP_WMEMB_TRANS_2, &stPos, pstPassProp->acPassThrough0, 13);


    stPos.u32x = 600;
    stPos.u32y = 400;
    (HI_VOID)DOLBY_CreateWindMember(pWindHandle, APP_WMEMB_BUTTOM, &stPos, " OK ", 6);

    /* update window */
    HI_DOLBY_BuildAndPasteWind(pWindHandle);
    HI_GO_SetLayerAlpha(s_hLayer,255);
    (HI_VOID)HI_DOLBY_ShowWind();
}

HI_VOID HI_DOLBY_DestroyPassThrough_WIN(HI_VOID* pWindHandle)
{
    HI_RECT stRc={0};
    HI_S32 s32Ret = 0;

    WINDOW_S *pWind = (WINDOW_S *)pWindHandle;

    stRc.x = 0;
    stRc.y = 0;
    stRc.w = 1280;
    stRc.h = 645;
    s32Ret = HI_GO_FillRect(pWind->hMemSurface, &stRc, 0, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_GO_FillRect\n");
    }
    s32Ret = DOLBY_PasteWind(pWindHandle, s_hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        printf("dolby_PasteWind fail!\n");
    }

    s32Ret = HI_DOLBY_ShowWind();
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_DOLBY_ShowWind!\n");
    }

    HI_DOLBY_DestroyWindMemberList(pWindHandle);
    HI_GO_SetLayerAlpha(s_hLayer,70);
}

HI_VOID HI_DOLBY_DrawSearchMesg(HI_VOID* pWindHandle)
{
    HI_S32 s32Ret = HI_SUCCESS;

    WINDOW_S *pstWind = (WINDOW_S *)pWindHandle;

    s32Ret = HI_GO_FillRect(pstWind->hMemSurface, NULL, WHITE_COLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_GO_FillRect\n");
    }

    Hi_Dolby_Debug();

    (HI_VOID)DOLBY_ShowText(pWindHandle, DOLBY_L_SEARCH_MSG, 200, 100, BACK_COLOR);

    (HI_VOID)DOLBY_ShowText(pWindHandle, DOLBY_L_SEARCH_MSG_OK, 200, 200, BACK_COLOR);

    Hi_Dolby_Debug();

    s32Ret = DOLBY_PasteWind(pWindHandle, s_hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        printf("dolby_PasteWind fail!\n");
       // return HI_FAILURE;
    }

    s32Ret = HI_DOLBY_ShowWind();
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_DOLBY_ShowWind!\n");
    }

    Hi_Dolby_Debug();
}

HI_VOID HI_DOLBY_ClearSearchMesg(HI_VOID* pWindHandle)
{
    HI_S32 s32Ret = 0;
    HI_S32 i;
    HI_U32 u32PressStatus = 0, u32KeyId = 0;


    WINDOW_S *pWind = (WINDOW_S *)pWindHandle;

    while(1)
    {
        s32Ret = HI_DOLBY_IR_GetValue(&u32PressStatus, &u32KeyId);
        if (HI_SUCCESS == s32Ret)
        {
            for (i = 0; i < MAX_IR_MAP; i++)
            {
                if (g_stIRMap[i].u32KeyValue == u32KeyId)
                {
                    break;
                }
            }

            /* Invalid value */
            if(MAX_IR_MAP == i)
            {
                printf("Invalid key value, Please input right key value!\n");
                //return HI_FAILURE;
            }
            else
            {
                if(APP_KEY_SELECT == g_stIRMap[i].u32Event)
                {
                    break;
                }
            }
        }

        usleep(1000);
    }

    s32Ret = HI_GO_FillRect(pWind->hMemSurface, NULL, 0, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_GO_FillRect\n");
    }

    s32Ret = DOLBY_PasteWind(pWindHandle, s_hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        printf("dolby_PasteWind fail!\n");
       // return HI_FAILURE;
    }

    s32Ret = HI_DOLBY_ShowWind();
    if (HI_SUCCESS != s32Ret)
    {
         printf("failed to HI_DOLBY_ShowWind!\n");
    }
}
