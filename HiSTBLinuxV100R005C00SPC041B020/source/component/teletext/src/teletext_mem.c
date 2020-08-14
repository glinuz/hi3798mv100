#include "teletext_mem.h"
#include "stdlib.h"
#include "string.h"

/**(2.8M)it's about  size of 1000 pages  */
#define TTX_MAX_MALLOCSIZE (2800000)
/**size of one page is 2772Byte *//*(32+16+8+5+5)*42*/
#define TTX_MAX_PAGESTORE (2048)
#define TTX_MEM_MINSIZE (80 * 1024)/**the minimum size of ttx module need*/

typedef struct tagTTX_MEM_ITEM_S
{
    HI_U8* pu8ItemAddr;
    HI_U32 u32ItemSize;
    struct tagTTX_MEM_ITEM_S* pstNext;
} TTX_MEM_ITEM_S, *TTX_MEM_ITEM_S_PTR;

typedef struct tagTTX_MEM_DATA_S
{
    HI_U8*             pu8MemAddr;
    HI_U32             u32MemSize;
    size_t             uMaxMalloc;
    TTX_MEM_ITEM_S_PTR pstItemHead;
} TTX_MEM_DATA_S, *TTX_MEM_DATA_S_PTR;

static TTX_MEM_DATA_S_PTR s_pstMemData = {0};
static HI_BOOL s_bInMalloc = HI_TRUE;
static HI_U8* s_pu8InMemAddr = HI_NULL;

#define TTX_MEM_CHECK(param) \
    do                                      \
    {                                       \
        if (param == HI_NULL)               \
        {                                   \
            return HI_FAILURE;              \
        }                                   \
    } while (0)

static HI_S32 TTX_Mem_IsFull(HI_U32 u32MemSize, TTX_MEM_ITEM_S** pstItem);

static HI_S32 TTX_Mem_IsFull(HI_U32 u32MemSize, TTX_MEM_ITEM_S** pstItem)
{
    size_t nOffset = 0;
    HI_U16 u16StructSize = 0;
    TTX_MEM_ITEM_S_PTR pstThisItem = HI_NULL;
    TTX_MEM_ITEM_S_PTR pstNextItem = HI_NULL;

    TTX_MEM_CHECK(s_pstMemData);

    u16StructSize = sizeof(TTX_MEM_ITEM_S);
    pstThisItem = s_pstMemData->pstItemHead;

    while (pstThisItem != HI_NULL)
    {
        pstNextItem = pstThisItem->pstNext;
        nOffset = (size_t)pstThisItem + pstThisItem->u32ItemSize + u16StructSize;

        if (pstNextItem != HI_NULL)
        {
            if (((size_t)pstNextItem - nOffset) < (u32MemSize + u16StructSize))
            {
                pstThisItem = pstThisItem->pstNext;
                continue;
            }
            else
            {
                (*pstItem) = pstThisItem;
                return HI_SUCCESS;
            }
        }
        else
        {
            if ((s_pstMemData->uMaxMalloc - nOffset) < (u32MemSize + u16StructSize))
            {
                return HI_FAILURE;
            }
            else
            {
                (*pstItem) = pstThisItem;

                return HI_SUCCESS;
            }
        }
    }

    return HI_FAILURE;
}

HI_S32 TTX_Mem_Init(HI_U8* pu8MemAddr, HI_U32 u32MemSize)
{
    if ((HI_NULL == pu8MemAddr) || (0 == u32MemSize))
    {
        s_bInMalloc = HI_TRUE;
        s_pu8InMemAddr = (HI_U8*)malloc(TTX_MAX_MALLOCSIZE);

        TTX_MEM_CHECK(s_pu8InMemAddr);

        s_pstMemData = (TTX_MEM_DATA_S_PTR)s_pu8InMemAddr;
        s_pstMemData->pu8MemAddr   = s_pu8InMemAddr;
        s_pstMemData->u32MemSize   = TTX_MAX_MALLOCSIZE;
        s_pstMemData->uMaxMalloc = (size_t)s_pu8InMemAddr + TTX_MAX_MALLOCSIZE;
        s_pstMemData->pstItemHead  = (TTX_MEM_ITEM_S_PTR)(s_pu8InMemAddr + sizeof(TTX_MEM_DATA_S));
        s_pstMemData->pstItemHead->pstNext = HI_NULL;
        s_pstMemData->pstItemHead->u32ItemSize = 0;
    }
    else
    {
        if (u32MemSize < TTX_MEM_MINSIZE)
        {
            return HI_FAILURE;
        }
        else
        {
            s_pstMemData = (TTX_MEM_DATA_S_PTR)pu8MemAddr;
            s_pstMemData->pu8MemAddr   = pu8MemAddr;
            s_pstMemData->u32MemSize   = u32MemSize;
            s_pstMemData->uMaxMalloc = (size_t)pu8MemAddr + u32MemSize;
            s_pstMemData->pstItemHead  = (TTX_MEM_ITEM_S_PTR)(pu8MemAddr + sizeof(TTX_MEM_DATA_S));
            s_pstMemData->pstItemHead->pstNext = HI_NULL;
            s_pstMemData->pstItemHead->u32ItemSize = 0;
            s_bInMalloc = HI_FALSE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 TTX_Mem_DeInit(HI_VOID)
{
    TTX_MEM_CHECK(s_pstMemData);

    if (s_bInMalloc)
    {
        free (s_pu8InMemAddr);
        s_pu8InMemAddr = HI_NULL;
        s_pstMemData = HI_NULL;
    }
    else
    {
        memset(s_pstMemData, 0, sizeof(TTX_MEM_DATA_S));
    }

    return HI_SUCCESS;
}

HI_VOID* TTX_Mem_Malloc(HI_U32 u32MemSize)
{
    HI_S32 s32Ret = 0;
    TTX_MEM_ITEM_S_PTR pstThisItem = HI_NULL;
    TTX_MEM_ITEM_S_PTR pstTmpItem = HI_NULL;

    s32Ret = TTX_Mem_IsFull(u32MemSize, &pstThisItem);

    if (HI_SUCCESS == s32Ret)
    {
        pstTmpItem = (TTX_MEM_ITEM_S_PTR)((size_t)pstThisItem + pstThisItem->u32ItemSize + sizeof(TTX_MEM_ITEM_S));

        pstTmpItem->u32ItemSize = u32MemSize;
        pstTmpItem->pu8ItemAddr = (HI_U8*)((size_t)pstTmpItem + sizeof(TTX_MEM_ITEM_S));

        if (pstThisItem->pstNext != HI_NULL)
        {
            pstTmpItem->pstNext  = pstThisItem->pstNext;
            pstThisItem->pstNext = pstTmpItem;
        }
        else
        {
            pstThisItem->pstNext = pstTmpItem;
            pstTmpItem->pstNext = HI_NULL;
        }

        return (HI_VOID*)(pstTmpItem->pu8ItemAddr);
    }
    else
    {
        return (HI_VOID*)HI_NULL;
    }
}

HI_VOID TTX_Mem_Free(const HI_VOID* pvMemStart)
{
    TTX_MEM_ITEM_S_PTR pstThisItem = HI_NULL;
    TTX_MEM_ITEM_S_PTR pstPrevItem = HI_NULL;

    if (s_pstMemData != HI_NULL)
    {
        pstPrevItem = pstThisItem = s_pstMemData->pstItemHead;
    }

    while ((pstThisItem != HI_NULL))
    {
        if (pstThisItem->pu8ItemAddr != (HI_U8*)pvMemStart)
        {
            pstPrevItem = pstThisItem;
            pstThisItem = pstThisItem->pstNext;
        }
        else
        {
            break;
        }
    }

    if ((HI_NULL == pstThisItem) || (HI_NULL == pstPrevItem))
    {
        return;
    }
    else
    {
        if (pstThisItem->pstNext != HI_NULL)
        {
            pstPrevItem->pstNext = pstThisItem->pstNext;
        }
        else
        {
            pstPrevItem->pstNext = HI_NULL;
        }

        if (pstThisItem->u32ItemSize <= TTX_MAX_MALLOCSIZE)
        {
            memset(pstThisItem, 0, pstThisItem->u32ItemSize + sizeof(TTX_MEM_ITEM_S));
        }
    }

    return;
}

HI_VOID TTX_Mem_Memset(HI_VOID* pvMemStart, HI_S32 s32InitNum, HI_U32 u32MemSize)
{
    memset(pvMemStart, s32InitNum, u32MemSize);
}

HI_S32 TTX_Mem_GetSpareSize(HI_U32* u32SpareSize)
{
    size_t nItemLength = 0;
    HI_U32 u32SpareLength = 0;
    TTX_MEM_ITEM_S_PTR pstThisItem = HI_NULL;
    TTX_MEM_ITEM_S_PTR pstPrevItem = HI_NULL;

    TTX_MEM_CHECK(s_pstMemData);

    pstThisItem = s_pstMemData->pstItemHead;

    while (pstThisItem != HI_NULL)
    {
        pstPrevItem = pstThisItem;
        pstThisItem = pstThisItem->pstNext;
    }

    if (pstPrevItem != HI_NULL)
    {
        nItemLength = (size_t)pstPrevItem + pstPrevItem->u32ItemSize + sizeof(TTX_MEM_ITEM_S);
    }
    else
    {
        return HI_FAILURE;
    }

    u32SpareLength = ((size_t)s_pstMemData + s_pstMemData->u32MemSize) - nItemLength;

    (*u32SpareSize) = u32SpareLength;

    return HI_SUCCESS;
}

