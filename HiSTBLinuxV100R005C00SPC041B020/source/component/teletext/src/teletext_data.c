/***********************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
* FileName   :  teletext_data.c
* Description:  Define functions used to storing date of teletext
*
* History:
* Version      Date             Author     DefectNum    Description
* main\1    2012-03-12                      NULL      Create this file.
***********************************************************************************/

#include <string.h>
#include "teletext_debug.h"
#include "teletext_data.h"
#include "teletext_parse.h"
#include "teletext_mem.h"
#include "teletext_def.h"

#define TTX_INV_ADDR (0)
#define TTX_TIMECODE_SIZE (8)

//#define TTX_MAX_PAGESTORE (800)
#define TTX_MAX_PAGESTORE (2048)

typedef struct tagTTX_PAGE_STORE_S
{
    TTX_PAGE_S stPage;
    struct tagTTX_PAGE_STORE_S* pstNext;
} TTX_PAGE_STORE_S, *TTX_PAGE_STORE_S_PTR;

typedef struct tagTTX_DATA_MAPPING_S
{
    HI_VOID* pPosition;
} TTX_DATA_MAPPING_S;

typedef struct tagTTX_DATA_STORE_S
{
    TTX_DATA_MAPPING_S     stszDataMap[TTX_MAX_PAGESTORE];
    TTX_PAGE_STORE_S_PTR   pstInitpageStore;
    HI_UNF_TTX_PAGE_ADDR_S stFilerPageAddr;
    HI_U8                  u8szM8_P30[TTX_LINE_MEM_SIZE];
    HI_BOOL                bM30;
    HI_U8                  au8TimeCode[TTX_TIMECODE_SIZE];
    pthread_mutex_t        TTX_mutex_datastore;
} TTX_DATA_STORE_S, *TTX_DATA_STORE_S_PTR;

static TTX_DATA_STORE_S_PTR s_pstDataStore = HI_NULL;

#define TTX_DATA_LOCK()  \
    do{\
        int ret = pthread_mutex_lock(&s_pstDataStore->TTX_mutex_datastore);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_lock(DATA) failure ret = 0x%x\n",ret);\
        }\
    }while(0)

#define TTX_DATA_UNLOCK()  \
    do{\
        int ret = pthread_mutex_unlock(&s_pstDataStore->TTX_mutex_datastore);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_unlock(DATA) failure ret = 0x%x\n",ret);\
        }\
    }while(0)

#define TTX_DATA_JUDGE() \
    do                                      \
    {                                       \
        if (s_pstDataStore == HI_NULL)      \
        {                                   \
            return HI_FAILURE;              \
        }                                   \
    } while (0)
#define TTX_DATA_CHECK(param) \
    do                                      \
    {                                       \
        if (param == HI_NULL)               \
        {                                   \
            return HI_FAILURE;              \
        }                                   \
    } while (0)

static HI_VOID TTX_Data_ClearSubpage(const TTX_PAGE_STORE_S_PTR pstClearPage);
static HI_S32  TTX_Data_BothSides(HI_U8* pu8Right, HI_U8* pu8Left, HI_U16 u16Curr);
static HI_S32  TTX_Data_ReplacePage(HI_U16 u16Pageten, TTX_PAGE_STORE_S** pstTmpPage);
static HI_S32  TTX_Data_StoreSubPage(HI_U16 u16Pageten, const TTX_PAGE_S_PTR pstPage);
static HI_S32  TTX_Data_StorePage(HI_U16 u16Pageten, TTX_PAGE_S_PTR pstPage);
static HI_S32  TTX_Data_FindReplacePage(HI_U16 u16Pageten, HI_U16* pu16ReplacePage);

static HI_VOID TTX_Data_ClearSubpage(const TTX_PAGE_STORE_S_PTR pstClearPage)
{
    TTX_PAGE_STORE_S_PTR pstPrevPage = HI_NULL;
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;

    pstThisPage = pstClearPage->pstNext;

    while (pstThisPage != HI_NULL)
    {
        pstPrevPage = pstThisPage;
        pstThisPage = pstThisPage->pstNext;
        TTX_Mem_Free(pstPrevPage);
    }

    return;
}

/*judge there is subpage or not*/
static HI_S32 TTX_Data_JudgeSubPageNum(HI_VOID* pPosition)
{
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;

    pstThisPage = (TTX_PAGE_STORE_S_PTR)pPosition;

    if (pstThisPage->pstNext != HI_NULL)
    {
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

static HI_S32 TTX_Data_BothSides(HI_U8* pu8Right, HI_U8* pu8Left, HI_U16 u16Curr)
{
    HI_U16 i = 0;
    HI_U16 u16Tmp = 0;

    if ((s_pstDataStore == HI_NULL) || (pu8Right == HI_NULL)
        || (pu8Left == HI_NULL) || (u16Curr >= TTX_MAX_PAGESTORE))
    {
        return HI_FAILURE;
    }

    u16Tmp = (HI_U16)(((u16Curr / 256) + 1) * 256);

    if (TTX_MAX_PAGESTORE < u16Tmp)
    {
        return HI_FAILURE;
    }

    for (i = u16Curr + 1; i < u16Tmp; i++)
    {
        if (s_pstDataStore->stszDataMap[i].pPosition != TTX_INV_ADDR)
        {
            (*pu8Right)++;
        }
    }

    u16Tmp = (HI_U16)((u16Curr / 256) * 256);

    for (i = u16Tmp; i < u16Curr; i++)
    {
        if (s_pstDataStore->stszDataMap[i].pPosition != TTX_INV_ADDR)
        {
            (*pu8Left)++;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 TTX_Data_FindReplacePage(HI_U16 u16Pageten, HI_U16* pu16ReplacePage)
{
    HI_U16 i = 0;
    HI_U16 j = 0;
    HI_U8 u8Right = 0;
    HI_U8 u8Left = 0;
    HI_U16 u16Tmp = 0;
    HI_U16 u16Tmp1 = 0;
    HI_U8 u8Magazine = 0;
    HI_U16 u16TmpPageten = 0;
    HI_VOID* pPosition = 0;
    HI_U8 szu8tmp[7] = {4, 5, 3, 6, 2, 7, 1};

    TTX_DATA_JUDGE();

    if ((s_pstDataStore->stFilerPageAddr.u8MagazineNum != (u16Pageten / 256)))
    {
        return HI_FAILURE;
    }

    u8Magazine = s_pstDataStore->stFilerPageAddr.u8MagazineNum;

    if (8 <= u8Magazine)
    {
        return HI_FAILURE;
    }

    for (i = 0; i < 7; i++)
    {
        u16Tmp  = ((u8Magazine + szu8tmp[i]) % 8) * 256;
        u16Tmp1 = u16Tmp + 0x80;

        for (j = 0; j < 0x80; j++)
        {
            u16Tmp = u16Tmp1 + j;
            pPosition = s_pstDataStore->stszDataMap[u16Tmp].pPosition;

            if (pPosition != TTX_INV_ADDR)
            {
                break;
            }

            u16Tmp = u16Tmp1 - j;
            pPosition = s_pstDataStore->stszDataMap[u16Tmp].pPosition;

            if (pPosition != TTX_INV_ADDR)
            {
                break;
            }
        }

        if (pPosition != TTX_INV_ADDR)
        {
            break;
        }
        else
        {
            u16Tmp = ((u8Magazine + szu8tmp[i]) % 8) * 256;
            pPosition = s_pstDataStore->stszDataMap[u16Tmp].pPosition;

            if (pPosition != TTX_INV_ADDR)
            {
                if (HI_SUCCESS == TTX_Data_JudgeSubPageNum(pPosition))
                {
                    break;
                }
                else
                {
                    pPosition = TTX_INV_ADDR;
                }
            }
        }
    }

    if (pPosition == TTX_INV_ADDR)                  /*only replace this magzine*/
    {
        u16TmpPageten = (s_pstDataStore->stFilerPageAddr.u8MagazineNum * 256)
                        + (s_pstDataStore->stFilerPageAddr.u8PageNum);

        if (HI_SUCCESS == TTX_Data_BothSides(&u8Right, &u8Left, u16TmpPageten))
        {
            if (u8Right > u8Left)
            {
                u16Tmp = (u8Magazine + 1) * 256 - 1;

                for (i = u16Tmp; i > u16TmpPageten; i--)
                {
                    pPosition = s_pstDataStore->stszDataMap[i].pPosition;

                    if (pPosition != TTX_INV_ADDR)
                    {
                        if (u16Pageten < u16TmpPageten)
                        {
                            u16Tmp = i;
                            break;
                        }
                        else
                        {
                            if (i > u16Pageten)
                            {
                                u16Tmp = i;
                                break;
                            }
                            else
                            {
                                return HI_FAILURE;   /*discart this  page*/
                            }
                        }
                    }
                }
            }
            else
            {
                u16Tmp = u8Magazine * 256;

                for (i = u16Tmp; i < u16TmpPageten; i++)
                {
                    pPosition = s_pstDataStore->stszDataMap[i].pPosition;

                    if (pPosition != TTX_INV_ADDR)
                    {
                        if (u16Pageten < u16TmpPageten)
                        {
                            if (i > u16Pageten)
                            {
                                return HI_FAILURE;   /*discart this  page*/
                            }
                            else
                            {
                                u16Tmp = i;
                                break;
                            }
                        }
                        else
                        {
                            u16Tmp = i;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (pPosition != TTX_INV_ADDR)
    {
        (*pu16ReplacePage) = u16Tmp;
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 TTX_Data_ReplacePage(HI_U16 u16Pageten, TTX_PAGE_STORE_S** pstTmpPage)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U16 u16ReplacePage = 0;
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;

    if (HI_SUCCESS == TTX_Data_FindReplacePage(u16Pageten, &u16ReplacePage))
    {
        if ((u16Pageten == u16ReplacePage) || (u16ReplacePage >= TTX_MAX_PAGESTORE))
        {
            return HI_FAILURE;
        }

        pstThisPage = (TTX_PAGE_STORE_S_PTR)s_pstDataStore->stszDataMap[u16ReplacePage].pPosition;

        TTX_Data_ClearSubpage(pstThisPage);

        pstThisPage->pstNext = HI_NULL;

        if (u16ReplacePage % 256 == 0) /*check whether the replace page is the first page of a magazine*/
        {
            pstThisPage = HI_NULL;

            pstThisPage = (TTX_PAGE_STORE_S_PTR)TTX_Mem_Malloc(sizeof(TTX_PAGE_STORE_S));

            if (HI_NULL == pstThisPage)
            {
                s32Ret = HI_FAILURE;
            }
            else
            {
                (*pstTmpPage) = pstThisPage;
                s32Ret = HI_SUCCESS;
            }
        }
        else
        {
            (*pstTmpPage) = pstThisPage;
            s_pstDataStore->stszDataMap[u16ReplacePage].pPosition = TTX_INV_ADDR;
            s32Ret = HI_SUCCESS;
        }
    }
    else
    {
        s32Ret = HI_FAILURE;
    }

    return s32Ret;
}

static HI_S32 TTX_Data_StoreSubPage(HI_U16 u16Pageten, const TTX_PAGE_S_PTR pstPage)
{
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;
    TTX_PAGE_STORE_S_PTR pstPrevPage = HI_NULL;
    TTX_PAGE_STORE_S_PTR pstTmpPage = HI_NULL;

    pstThisPage = (TTX_PAGE_STORE_S_PTR)s_pstDataStore->stszDataMap[u16Pageten].pPosition;

    /*find place where this subpage store*/
    while (pstThisPage != HI_NULL)
    {
        if (pstThisPage->stPage.u16subcode < pstPage->u16subcode)
        {
            pstPrevPage = pstThisPage;
            pstThisPage = pstThisPage->pstNext;
        }
        else
        {
            break;
        }
    }

    if ((pstThisPage != HI_NULL) && (pstThisPage->stPage.u16subcode == pstPage->u16subcode))
    {
        memcpy(&pstThisPage->stPage, pstPage, sizeof(TTX_PAGE_S));
    }
    else
    {
        pstTmpPage = (TTX_PAGE_STORE_S_PTR)TTX_Mem_Malloc(sizeof(TTX_PAGE_STORE_S));

        if (HI_NULL == pstTmpPage)
        {
            if (HI_SUCCESS != TTX_Data_ReplacePage(u16Pageten, &pstTmpPage))
            {
                return HI_FAILURE;
            }
        }

        memcpy(&pstTmpPage->stPage, pstPage, sizeof(TTX_PAGE_S));

        if (pstThisPage != HI_NULL)
        {
            if (HI_NULL == pstPrevPage)
            {
                pstTmpPage->pstNext = pstThisPage;
                s_pstDataStore->stszDataMap[u16Pageten].pPosition = pstTmpPage;
            }
            else
            {
                pstPrevPage->pstNext = pstTmpPage;
                pstTmpPage->pstNext = pstThisPage;
            }
        }
        else
        {
            if (pstPrevPage != HI_NULL)
            {
                pstPrevPage->pstNext = pstTmpPage;
                pstTmpPage->pstNext = HI_NULL;
                pstThisPage = pstTmpPage;
            }
        }
    }

    if (s_pstDataStore->bM30)
    {
        if (HI_NULL != pstThisPage)
        {
            pstThisPage->stPage.u32ValidLines |= (1 << 30);
            memcpy(pstThisPage->stPage.u8szLines[30], s_pstDataStore->u8szM8_P30, TTX_LINE_MEM_SIZE);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 TTX_Data_StorePage(HI_U16 u16Pageten, TTX_PAGE_S_PTR pstPage)
{
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;

    TTX_DATA_CHECK(pstPage);
    TTX_DATA_CHECK(s_pstDataStore);

    pstThisPage = (TTX_PAGE_STORE_S_PTR)TTX_Mem_Malloc(sizeof(TTX_PAGE_STORE_S));

    if (HI_NULL == pstThisPage)
    {
        if (HI_SUCCESS != TTX_Data_ReplacePage(u16Pageten, &pstThisPage))
        {
            return HI_FAILURE;
        }
    }

    s_pstDataStore->stszDataMap[u16Pageten].pPosition = pstThisPage;
    memcpy(&pstThisPage->stPage, pstPage, sizeof(TTX_PAGE_S));
    pstThisPage->pstNext = HI_NULL;

    return HI_SUCCESS;
}

HI_S32 TTX_Data_Init(HI_VOID)
{
    HI_U16 i = 0;

    s_pstDataStore = (TTX_DATA_STORE_S_PTR)TTX_Mem_Malloc(sizeof(TTX_DATA_STORE_S));

    TTX_DATA_CHECK(s_pstDataStore);

    TTX_Mem_Memset(s_pstDataStore, 0, sizeof(TTX_DATA_STORE_S));

    s_pstDataStore->pstInitpageStore = (TTX_PAGE_STORE_S_PTR)TTX_Mem_Malloc(sizeof(TTX_PAGE_STORE_S));

    if (HI_NULL == s_pstDataStore->pstInitpageStore)
    {
        TTX_Mem_Free(s_pstDataStore);
        s_pstDataStore = HI_NULL;
        return HI_FAILURE;
    }

    TTX_Mem_Memset(s_pstDataStore->pstInitpageStore, 0, sizeof(TTX_PAGE_STORE_S));

    s_pstDataStore->bM30 = HI_FALSE;

    for (i = 0; i < TTX_MAX_PAGESTORE; i++)
    {
        s_pstDataStore->stszDataMap[i].pPosition = TTX_INV_ADDR;
    }

    if (0 != pthread_mutex_init(&s_pstDataStore->TTX_mutex_datastore, HI_NULL))
    {
        TTX_Mem_Free(s_pstDataStore->pstInitpageStore);
        s_pstDataStore->pstInitpageStore = HI_NULL;
        TTX_Mem_Free(s_pstDataStore);
        s_pstDataStore = HI_NULL;
        return HI_FAILURE;
    }

    s_pstDataStore->stFilerPageAddr.u8MagazineNum = 1;
    s_pstDataStore->stFilerPageAddr.u8PageNum = 0;

    return HI_SUCCESS;
}

HI_S32 TTX_Data_DeInit()
{
    HI_U16 i = 0;
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;
    TTX_PAGE_STORE_S_PTR pstPrevPage = HI_NULL;

    TTX_DATA_JUDGE();

    for (i = 0; i < TTX_MAX_PAGESTORE; i++)
    {
        if (s_pstDataStore->stszDataMap[i].pPosition != TTX_INV_ADDR)
        {
            pstThisPage = (TTX_PAGE_STORE_S_PTR)s_pstDataStore->stszDataMap[i].pPosition;

            while (pstThisPage != HI_NULL)
            {
                pstPrevPage = pstThisPage;
                pstThisPage = pstThisPage->pstNext;
                TTX_Mem_Free(pstPrevPage);
            }
        }
    }

    if (0 != pthread_mutex_destroy(&s_pstDataStore->TTX_mutex_datastore))
    {
        HI_ERR_TTX("destroy TTX_mutex_datastore error\n");
    }

    TTX_Mem_Free(s_pstDataStore->pstInitpageStore);

    s_pstDataStore->pstInitpageStore = HI_NULL;

    TTX_Mem_Free(s_pstDataStore);

    s_pstDataStore = HI_NULL;

    return HI_SUCCESS;
}

HI_S32 TTX_Data_Reset()
{
    HI_U16 i = 0;
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;
    TTX_PAGE_STORE_S_PTR pstPrevPage = HI_NULL;

    TTX_DATA_JUDGE();

    TTX_DATA_LOCK();

    for (i = 0; i < TTX_MAX_PAGESTORE; i++)
    {
        if (s_pstDataStore->stszDataMap[i].pPosition != TTX_INV_ADDR)
        {
            pstThisPage = (TTX_PAGE_STORE_S_PTR)s_pstDataStore->stszDataMap[i].pPosition;

            while (pstThisPage != HI_NULL)
            {
                pstPrevPage = pstThisPage;
                pstThisPage = pstThisPage->pstNext;
                TTX_Mem_Memset(pstPrevPage, 0, sizeof(TTX_PAGE_STORE_S));
                TTX_Mem_Free(pstPrevPage);
            }
        }
    }

    TTX_Mem_Memset(s_pstDataStore->pstInitpageStore, 0, sizeof(TTX_PAGE_STORE_S));

    for (i = 0; i < TTX_MAX_PAGESTORE; i++)
    {
        s_pstDataStore->stszDataMap[i].pPosition = TTX_INV_ADDR;
    }

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Data_Search(HI_U8 u8MagNum, HI_U8 u8PageNum, HI_U16 u16SubpageNum, TTX_PAGE_S_PTR pstTmppage)
{
    HI_U16 u16TmpPage = 0;

    TTX_PAGE_STORE_S_PTR pstThispage = HI_NULL;

    TTX_DATA_JUDGE();

    if ((u8MagNum > 7)/* || (u8PageNum > 99)*/ || (pstTmppage == HI_NULL))
    {
        return HI_FAILURE;
    }

    TTX_DATA_LOCK();

    u16TmpPage = (HI_U16)(u8MagNum * 256 + u8PageNum);

    if (s_pstDataStore->stszDataMap[u16TmpPage].pPosition == TTX_INV_ADDR)
    {
        TTX_DATA_UNLOCK();
        return HI_FAILURE;
    }

    pstThispage = (TTX_PAGE_STORE_S_PTR)s_pstDataStore->stszDataMap[u16TmpPage].pPosition;

    if (pstThispage->stPage.u16subcode == u16SubpageNum)
    {
        memcpy(pstTmppage, &pstThispage->stPage, sizeof(TTX_PAGE_S));
    }
    else if (u16SubpageNum != 0)
    {
        if ((s_pstDataStore->stFilerPageAddr.u8MagazineNum != u8MagNum)
            || (s_pstDataStore->stFilerPageAddr.u8PageNum != u8PageNum))
        {
            TTX_DATA_UNLOCK();

            return HI_FAILURE;
        }
        else
        {
            pstThispage = pstThispage->pstNext;

            while (pstThispage != HI_NULL)
            {
                if (pstThispage->stPage.u16subcode == u16SubpageNum)
                {
                    break;
                }

                pstThispage = pstThispage->pstNext;
            }

            if (pstThispage != HI_NULL)
            {
                memcpy(pstTmppage, &pstThispage->stPage, sizeof(TTX_PAGE_S));
            }
            else
            {
                TTX_DATA_UNLOCK();
                return HI_FAILURE;
            }
        }
    }
    else
    {
        TTX_DATA_UNLOCK();
        return HI_FAILURE;
    }

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Data_SetFilter(HI_UNF_TTX_PAGE_ADDR_S stReqPgAddr)
{
    TTX_DATA_JUDGE();

    TTX_DATA_LOCK();

    s_pstDataStore->stFilerPageAddr.u8MagazineNum = stReqPgAddr.u8MagazineNum;
    s_pstDataStore->stFilerPageAddr.u8PageNum = stReqPgAddr.u8PageNum;

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Data_M30In(HI_U8* pu8Data)
{
    TTX_DATA_JUDGE();

    TTX_DATA_CHECK(pu8Data);

    TTX_DATA_LOCK();

    memcpy(s_pstDataStore->u8szM8_P30, &pu8Data[4], TTX_LINE_MEM_SIZE);
    s_pstDataStore->bM30 = HI_TRUE;

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Data_In(HI_U8 u8MagNum, HI_U8 u8PageNum, HI_U16 u16SubpageNum, TTX_PAGE_S* pstpage)
{
    HI_VOID* pPosition = 0;
    HI_U16 u16Page = 0;
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;

    TTX_DATA_JUDGE();

    if (!(pstpage->u32ValidLines & 0x01))
    {
        return HI_FAILURE;
    }
    else
    {
        if (pstpage->u32ValidLines == 0x01)
        {
            return HI_SUCCESS;
        }
    }

    TTX_DATA_LOCK();

    u16Page = (HI_U16)(u8MagNum * 256 + u8PageNum);

    pPosition = s_pstDataStore->stszDataMap[u16Page].pPosition;

    if (pPosition != TTX_INV_ADDR)                                            /*memory have this page*/
    {
        if (u16SubpageNum != 0)
        {
            if (HI_FAILURE == TTX_Data_StoreSubPage(u16Page, pstpage))
            {
                HI_INFO_TTX("TTX_Data_StoreSubPage error\n");
            }
        }
        else
        {
            pstThisPage = (TTX_PAGE_STORE_S_PTR)pPosition;
            memcpy(&pstThisPage->stPage, pstpage, sizeof(TTX_PAGE_S));
        }
    }
    else
    {
        if (HI_FAILURE == TTX_Data_StorePage(u16Page, pstpage))
        {
            HI_INFO_TTX("TTX_Data_StorePage error\n");
        }
    }

    TTX_DATA_UNLOCK();
    return HI_SUCCESS;
}


static HI_VOID TTX_Data_GetNextSubPage(TTX_PAGE_STORE_S_PTR* ppstThisPage, TTX_PAGE_STORE_S_PTR pstFirstPage, HI_U16 u16SubpageNum)
{
    while (*ppstThisPage != HI_NULL)
    {
        if ((*ppstThisPage)->stPage.u16subcode < u16SubpageNum)
        {
            (*ppstThisPage) = (*ppstThisPage)->pstNext;
            continue;
        }
        else
        {
            break;
        }
    }
    
    if ((*ppstThisPage) == NULL)                                /*the last -> the first*/
    {
        (*ppstThisPage) = pstFirstPage;
    }

    return;
}


static HI_VOID TTX_Data_GetPreSubPage(TTX_PAGE_STORE_S_PTR* ppstThisPage, 
                                              TTX_PAGE_STORE_S_PTR* ppstFirstPage,
                                              TTX_PAGE_STORE_S_PTR* ppstPrevPage, 
                                              HI_U16 u16SubpageNum)
{
    while ((*ppstThisPage != NULL) && (*ppstFirstPage != NULL))
    {
        if (((*ppstThisPage)->stPage.u16subcode > u16SubpageNum)
            && (*ppstFirstPage)->stPage.u16subcode <= u16SubpageNum)
        {
            break;
        }
        else
        {
            (*ppstPrevPage) = (*ppstThisPage);
            (*ppstThisPage) = (*ppstThisPage)->pstNext;
        }
    }
    
    (*ppstThisPage) = (*ppstPrevPage);

    return;
}



static HI_VOID TTX_Data_GetPrePage(TTX_PAGE_STORE_S_PTR* ppstThisPage, HI_U16* pu16Page)
{
    HI_U32 i = 0;
    HI_U16 u16Page = *pu16Page;
    HI_VOID* pPosition = HI_NULL;

    for (i = 0; i < TTX_MAX_PAGESTORE; i++)
    {
        /*skip the page num of 0xa-0xf,0x1a-0x1f........*/
        while (((u16Page & 0xf) > 9 ) || (((u16Page >> 4) & 0xf) > 9))
        {
            u16Page--;
            i++;
        }
    
        pPosition = s_pstDataStore->stszDataMap[u16Page].pPosition;
    
        if (pPosition == TTX_INV_ADDR)
        {
            if (u16Page == 0)
            {
                u16Page = TTX_MAX_PAGESTORE;
            }
    
            u16Page--;
        }
        else
        {
            *ppstThisPage = (TTX_PAGE_STORE_S_PTR)pPosition;
            break;
        }
    
        if (u16Page == 0)
        {
            u16Page = TTX_MAX_PAGESTORE - 1;
        }
    }


    *pu16Page = u16Page;
    
    return;
}


static HI_VOID TTX_Data_GetPreMagazine(TTX_PAGE_STORE_S_PTR* ppstThisPage, HI_U16* pu16Page)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U16 u16Page = *pu16Page;
    HI_VOID* pPosition = HI_NULL;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 256; j++)
        {
            /*skip the page num of 0xa-0xf,0x1a-0x1f........*/
            if (((u16Page & 0xf) > 9 ) || (((u16Page >> 4) & 0xf) > 9))
            {
                if ((u16Page & 0xff) < 0xff)
                {
                    u16Page++;
                    u16Page = u16Page % TTX_MAX_PAGESTORE;
                }
    
                continue;
            }
    
            pPosition = s_pstDataStore->stszDataMap[u16Page].pPosition;
    
            if (pPosition == TTX_INV_ADDR)
            {
                u16Page = (u16Page + 1) % TTX_MAX_PAGESTORE;
                continue;
            }
            else
            {
                *ppstThisPage = (TTX_PAGE_STORE_S_PTR)pPosition;
                break;
            }
        }
    
        if (pPosition != TTX_INV_ADDR)
        {
            break;
        }
    
        if (u16Page / 256 < 1)
        {
            u16Page = TTX_MAX_PAGESTORE;
        }
    
        u16Page = (HI_U16)(((u16Page / 256) - 1) * 256);
    }


    *pu16Page = u16Page;

    return;
}

static HI_VOID TTX_Data_GetNextMagazine(TTX_PAGE_STORE_S_PTR* ppstThisPage, HI_U16* pu16Page)
{
    HI_U32 i = 0;
    HI_U16 u16Page = *pu16Page;
    HI_VOID* pPosition = HI_NULL;

    for (i = 0; i < TTX_MAX_PAGESTORE; i++)
    {
        /*skip the page num of 0xa-0xf,0x1a-0x1f........*/
        while (((u16Page & 0xf) > 9 ) || (((u16Page >> 4) & 0xf) > 9))
        {
            u16Page++;
            u16Page = u16Page % TTX_MAX_PAGESTORE;
            i++;
        }
    
        pPosition = s_pstDataStore->stszDataMap[u16Page].pPosition;
    
        if (pPosition == TTX_INV_ADDR)
        {
            u16Page = (u16Page + 1) % TTX_MAX_PAGESTORE;
        }
        else
        {
            *ppstThisPage = (TTX_PAGE_STORE_S_PTR)pPosition;
            break;
        }
    }

    return;
}

static HI_VOID TTX_Data_GetThisPage(TTX_PAGE_STORE_S_PTR* ppstThisPage, HI_U16 u16SubpageNum)
{
    while (*ppstThisPage != HI_NULL)
    {
        if ((*ppstThisPage)->stPage.u16subcode == u16SubpageNum)
        {
            break;
        }
        else
        {
            (*ppstThisPage) = (*ppstThisPage)->pstNext;
        }
    }

}


HI_S32 TTX_Data_GetValidPage(HI_U8* pu8MagNum, HI_U8* pu8PageNum, HI_U16* pu16SubpageNum,
                             TTX_SEARCH_DIRECTION_E enSearchDir)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U16 i = 0;
    HI_U16 u16TmpMagNum  = 0;
    HI_U16 u16TmpPageNum = 0;
    HI_U16 u16Page  = 0;
    HI_U8 u8NextNum = 0;
    HI_VOID* pPosition = 0;
    TTX_PAGE_STORE_S_PTR pstThisPage = HI_NULL;
    TTX_PAGE_STORE_S_PTR pstPrevPage = HI_NULL;
    TTX_PAGE_STORE_S_PTR pstFirstPage = HI_NULL;

    TTX_DATA_JUDGE();

    if ((*pu8MagNum > 7) || /*(*pu8PageNum > 99) ||*/ (enSearchDir > TTX_INDEX_PAGE))
    {
        return HI_FAILURE;
    }

    u16TmpMagNum  = (HI_U16)(*pu8MagNum);
    u16TmpPageNum = (HI_U16)(*pu8PageNum);

    u16Page = (HI_U16)(u16TmpMagNum * 256 + u16TmpPageNum);

    TTX_DATA_LOCK();

    if ((enSearchDir <= TTX_NEXT5_PAGE) && (enSearchDir >= TTX_NEXT_PAGE))
    {
        u8NextNum = 0;

        for (i = 0; i < 2048; i++)
        {
            /*skip the page num of 0xa-0xf,0x1a-0x1f........*/
            while (((u16Page & 0xf) > 9 ) || (((u16Page >> 4) & 0xf) > 9))
            {
                u16Page ++;
                u16Page = u16Page % TTX_MAX_PAGESTORE;
                i++;
            }

            if (s_pstDataStore->stszDataMap[u16Page].pPosition != TTX_INV_ADDR)
            {
                u8NextNum++;
            }

            if (u8NextNum == ((enSearchDir + 1) - TTX_NEXT_PAGE))
            {
                pstThisPage   = (TTX_PAGE_STORE_S_PTR)s_pstDataStore->stszDataMap[u16Page].pPosition;
                (*pu8MagNum)  = (HI_U8)(u16Page / 256);
                (*pu8PageNum) = u16Page % 256;
                (*pu16SubpageNum) = (HI_U16)pstThisPage->stPage.u16subcode;

                TTX_DATA_UNLOCK();
                return HI_SUCCESS;
            }

            u16Page = (u16Page + 1) % TTX_MAX_PAGESTORE;
        }

        TTX_DATA_UNLOCK();

        return HI_FAILURE;
    }

    pPosition = s_pstDataStore->stszDataMap[u16Page].pPosition;

    if (pPosition == TTX_INV_ADDR)
    {
        pstThisPage = HI_NULL;
        pstFirstPage = HI_NULL;
    }
    else
    {
        pstThisPage = (TTX_PAGE_STORE_S_PTR)pPosition;
        pstFirstPage = (TTX_PAGE_STORE_S_PTR)pPosition;
    }

    switch (enSearchDir)
    {
        case TTX_NEXT_SUBPAGE:                                      /*search next valid subpage*/
        {
            TTX_Data_GetNextSubPage(&pstThisPage, pstFirstPage, (*pu16SubpageNum));
        }
        break;

        case TTX_PREV_SUBPAGE:                                         /*search prev valid subpage*/
        {
            TTX_Data_GetPreSubPage(&pstThisPage, &pstFirstPage, &pstPrevPage, (*pu16SubpageNum));
        }
        break;

        case TTX_PREV_PAGE:
        {
            TTX_Data_GetPrePage(&pstThisPage, &u16Page);
        }
        break;

        case TTX_PREV_MAGZINE:
        {
            TTX_Data_GetPreMagazine(&pstThisPage, &u16Page);
        }
        break;

        case TTX_NEXT_MAGZINE:
        {
            TTX_Data_GetNextMagazine(&pstThisPage, &u16Page);
        }
        break;

        case TTX_AUTO_PLAY:
            break;

        case TTX_THIS_PAGE:
        {
            TTX_Data_GetThisPage(&pstThisPage, *pu16SubpageNum);
        }
        break;

        case TTX_INDEX_PAGE:
        {
            if (0 != s_pstDataStore->pstInitpageStore->stPage.u32ValidLines)
            {
                (*pu16SubpageNum) = s_pstDataStore->pstInitpageStore->stPage.u16subcode;
                TTX_DATA_UNLOCK();
                return HI_SUCCESS;
            }
            else
            {
                TTX_DATA_UNLOCK();
                return HI_FAILURE;
            }
        }

        default:
            break;
    }

    if (pPosition != TTX_INV_ADDR)
    {
        (*pu8MagNum)  = (HI_U8)(u16Page / 256);
        (*pu8PageNum) = (HI_U8)(u16Page % 256);

        if (pstThisPage != HI_NULL)
        {
            (*pu16SubpageNum) = (HI_U16)(pstThisPage->stPage.u16subcode);
        }
        else
        {
            s32Ret = HI_FAILURE;
        }

        s32Ret = HI_SUCCESS;
    }
    else
    {
        s32Ret = HI_FAILURE;
    }

    TTX_DATA_UNLOCK();
    return s32Ret;
}

HI_S32 TTX_Data_GetSubpageNum(HI_U8 u8MagNum, HI_U8 u8PageNum, HI_U8* pSubpageNum, HI_U8* pu8Length)
{
    HI_U8 i = 0;
    HI_U16 u16Pageten  = 0;
    HI_VOID* pPosition = 0;
    TTX_PAGE_STORE_S_PTR pstPage = HI_NULL;

    TTX_DATA_JUDGE();

    if ((u8MagNum > 7) /*|| (u8PageNum > 99)*/)
    {
        return HI_FAILURE;
    }

    TTX_DATA_LOCK();

    u16Pageten = (u8MagNum * 256) + u8PageNum;

    pPosition = s_pstDataStore->stszDataMap[u16Pageten].pPosition;

    if (pPosition == TTX_INV_ADDR)
    {
        (*pu8Length) = 0;
        TTX_DATA_UNLOCK();
        return HI_FAILURE;
    }

    pstPage = (TTX_PAGE_STORE_S_PTR)pPosition;

    if (pstPage->stPage.u16subcode == 0)
    {
        (*pu8Length) = 0;
    }
    else
    {
        while ((pstPage != HI_NULL) && (i < (*pu8Length)))
        {
            pSubpageNum[i] = (HI_U8)pstPage->stPage.u16subcode;
            i++;
            pstPage = pstPage->pstNext;
        }

        (*pu8Length) = i;
    }

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Data_StoreInitPage(const TTX_PAGE_S* pstPage)
{
    HI_BOOL bStoreInitPage = HI_FALSE;

    TTX_DATA_JUDGE();

    TTX_DATA_LOCK();

    if (s_pstDataStore->pstInitpageStore->stPage.u32ValidLines != 0)
    {
        if (s_pstDataStore->pstInitpageStore->stPage.u16subcode > pstPage->u16subcode)
        {
            bStoreInitPage = HI_TRUE;
        }
    }
    else
    {
        bStoreInitPage = HI_TRUE;
    }

    if (bStoreInitPage)
    {
        memcpy(&s_pstDataStore->pstInitpageStore->stPage, pstPage, sizeof(TTX_PAGE_S));
        s_pstDataStore->pstInitpageStore->pstNext = HI_NULL;
    }

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Data_Set_TimeCode(HI_U8* pu8Data)
{
    TTX_DATA_JUDGE();

    TTX_DATA_CHECK(pu8Data);

    TTX_DATA_LOCK();

    memcpy(s_pstDataStore->au8TimeCode, pu8Data, TTX_TIMECODE_SIZE);

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Data_Get_TimeCode(HI_U8* pu8Data)
{
    TTX_DATA_JUDGE();

    TTX_DATA_CHECK(pu8Data);

    TTX_DATA_LOCK();

    memcpy(pu8Data, &s_pstDataStore->au8TimeCode[0], TTX_TIMECODE_SIZE);

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

/*find a packet from page*/
HI_S32 TTX_Data_FindPacket(TTX_PAGE_S* pstPage, HI_U8 u8PacketNo, HI_U8 u8DesignCode, HI_U8* pu8DataLine)
{
    HI_U8 u8Offset = 0;

    if (HI_NULL == pstPage)
    {
        return HI_FAILURE;
    }

    if (26 > u8PacketNo)
    {
        if ((pstPage->u32ValidLines >> u8PacketNo) & 0x1)
        {
            memcpy(pu8DataLine, pstPage->u8szLines[u8PacketNo], TTX_LINE_MEM_SIZE);
            return HI_SUCCESS;
        }
    }
    else
    {
        if (26 == u8PacketNo)
        {
            u8Offset = u8DesignCode;
        }
        else if (27 == u8PacketNo)
        {
            u8Offset = TTX_PACKET26_NUM + u8DesignCode;
        }
        else if (28 == u8PacketNo)
        {
            u8Offset = TTX_PACKET26_NUM + TTX_PACKET27_NUM + u8DesignCode;
        }
        else if (29 == u8PacketNo)
        {
            u8Offset = TTX_PACKET26_NUM + TTX_PACKET27_NUM + TTX_PACKET28_NUM + u8DesignCode;
        }

        if (32 > u8Offset)
        {
            if ((pstPage->u32ExtenValidLineL >> u8Offset) & 0x1)
            {
                memcpy(pu8DataLine, pstPage->u8szExtenLines[u8Offset], TTX_LINE_MEM_SIZE);
                return HI_SUCCESS;
            }
        }
        else
        {
            if ((pstPage->u32ExtenValidLineH >> (u8Offset - 32)) & 0x1)
            {
                memcpy(pu8DataLine, pstPage->u8szExtenLines[u8Offset], TTX_LINE_MEM_SIZE);
                return HI_SUCCESS;
            }
        }
    }

    return HI_FAILURE;
}

HI_S32 TTX_Data_FindPage(HI_U8 u8MagNum, HI_U8 u8PageNum, HI_U16 u16SubpageNum, TTX_PAGE_S* pstPage)
{
    HI_U16 u16TmpPage = 0;

    TTX_PAGE_STORE_S_PTR pstThispage = HI_NULL;

    TTX_DATA_JUDGE();

    if ((u8MagNum > 7) || (pstPage == HI_NULL))
    {
        return HI_FAILURE;
    }

    TTX_DATA_LOCK();

    u16TmpPage = (HI_U16)(u8MagNum * 256 + u8PageNum);

    if (s_pstDataStore->stszDataMap[u16TmpPage].pPosition == TTX_INV_ADDR)
    {
        TTX_DATA_UNLOCK();
        return HI_FAILURE;
    }

    pstThispage = (TTX_PAGE_STORE_S_PTR)s_pstDataStore->stszDataMap[u16TmpPage].pPosition;

    if (pstThispage->stPage.u16subcode == u16SubpageNum)
    {
        memcpy(pstPage, &pstThispage->stPage, sizeof(TTX_PAGE_S));
    }
    else if (u16SubpageNum != 0)
    {

        pstThispage = pstThispage->pstNext;

        while (pstThispage != HI_NULL)
        {
            if (pstThispage->stPage.u16subcode == u16SubpageNum)
            {
                break;
            }

            pstThispage = pstThispage->pstNext;
        }

        if (pstThispage != HI_NULL)
        {
            memcpy(pstPage, &pstThispage->stPage, sizeof(TTX_PAGE_S));
        }
        else
        {
            TTX_DATA_UNLOCK();
            return HI_FAILURE;
        }
    }
    else
    {
        TTX_DATA_UNLOCK();
        return HI_FAILURE;
    }

    TTX_DATA_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Data_FindFirstValidPage(HI_U8* pu8MagNum, HI_U8* pu8PageNum, HI_U16* pu16SubpageNum)
{
    HI_U16 u16TmpPage = 0;
    HI_U8 u8MagNum = 0;
    HI_U8 u8PageNum = 0;
    TTX_PAGE_STORE_S_PTR pstThispage = HI_NULL;


    TTX_DATA_JUDGE();

    if ((HI_NULL == pu8MagNum) || (HI_NULL == pu8PageNum) || (HI_NULL == pu16SubpageNum))
    {
        return HI_FAILURE;
    }

    TTX_DATA_LOCK();

    for (u8MagNum = 1; u8MagNum <= 8; u8MagNum++)
    {
        for (u8PageNum = 0; u8PageNum < 100; u8PageNum++)
        {
            u16TmpPage = (HI_U16)u8PageNum;
            TTX_DEC_TO_HEX(u16TmpPage);
            u16TmpPage += (HI_U16)((u8MagNum % 8) * 256);
            pstThispage = (TTX_PAGE_STORE_S_PTR)s_pstDataStore->stszDataMap[u16TmpPage].pPosition;

            if ((TTX_INV_ADDR != pstThispage) && (0 == pstThispage->stPage.u32PTS)) /*we only search the ttx initial page*/
            {
                *pu8MagNum = u8MagNum % 8;
                *pu8PageNum = TTX_DEC_TO_HEX(u8PageNum);
                *pu16SubpageNum = pstThispage->stPage.u16subcode;
                TTX_DATA_UNLOCK();
                return HI_SUCCESS;
            }
        }
    }

    TTX_DATA_UNLOCK();
    return HI_FAILURE;
}
