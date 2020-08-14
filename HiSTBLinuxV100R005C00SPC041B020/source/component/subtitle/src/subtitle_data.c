#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hi_type.h"
#include "subtitle_debug.h"
#include "subtitle_data.h"
#include "subtitle_parse.h"

/* if used sdk pes buffer directly,don't copy pes data,
*  data in pes buffer may be modified by new pes data when
*  subtitle parsing
*/
#define SUBT_USED_PES_BUFFER (1)
#define SUBT_PES_PACKET_MIN (26)
#define SUBT_PES_PACKET_LEN (24*1024)
#define SUBT_MAX_PES_PACKET_LEN (64*1024)
#define CACHE_NODE_NUM_MAX  (3)

typedef struct tagCache_Buffer_Node_S
{
    struct tagCache_Buffer_Node_S* next;
    HI_U8  au8Data[SUBT_PES_PACKET_LEN];
    HI_U32 u32DataSize;
} Cache_Buffer_Node_S;


typedef struct tagSUBT_DATA_RECV_S
{
    HI_VOID* hDataParse;

#if SUBT_USED_PES_BUFFER
    HI_U32    u32PESWritenLen; /*The data length of recving PES packket, in byte */
    HI_U8*     pu8WriteDataAddr; /*The data address of includeing start code PES packet */
    HI_U8     au8PesBuffer[SUBT_MAX_PES_PACKET_LEN];
#endif

    HI_U16    u16PageID;
    HI_U16    u16AncillaryID;

    Cache_Buffer_Node_S* pstCacheNode;
    Cache_Buffer_Node_S astCacheNode[CACHE_NODE_NUM_MAX];

} SUBT_DATA_RECV_S;

static HI_VOID Cache_Init(SUBT_DATA_RECV_S* pstDataRecv)
{
    HI_U8 i = 0;

    for (i = 0; i < (CACHE_NODE_NUM_MAX - 1); i++)
    {
        pstDataRecv->astCacheNode[i].next = &pstDataRecv->astCacheNode[i + 1];
    }

    pstDataRecv->astCacheNode[CACHE_NODE_NUM_MAX - 1].next = &pstDataRecv->astCacheNode[0];

    pstDataRecv->pstCacheNode = &pstDataRecv->astCacheNode[0];
}

static HI_S32 Cache_Put(SUBT_DATA_RECV_S* pstDataRecv, HI_U8* pu8Data, HI_U32 u32DataSize)
{
    if (u32DataSize < SUBT_PES_PACKET_MIN || u32DataSize > SUBT_PES_PACKET_LEN)
    {
        return HI_FAILURE;
    }

    memset(pstDataRecv->pstCacheNode->au8Data, 0, SUBT_PES_PACKET_LEN);
    memcpy(pstDataRecv->pstCacheNode->au8Data, pu8Data, u32DataSize);
    pstDataRecv->pstCacheNode->u32DataSize = u32DataSize;

    pstDataRecv->pstCacheNode = pstDataRecv->pstCacheNode->next;

    return HI_SUCCESS;
}

static HI_S32 Cache_Get(SUBT_DATA_RECV_S* pstDataRecv, HI_U8** ppu8Data, HI_U32* pu32DataSize)
{
    if (pstDataRecv->pstCacheNode->u32DataSize)
    {
        *ppu8Data = pstDataRecv->pstCacheNode->au8Data;
        *pu32DataSize = pstDataRecv->pstCacheNode->u32DataSize;

        pstDataRecv->pstCacheNode->u32DataSize = 0;
    }
    else
    {
        *ppu8Data = NULL;
        *pu32DataSize = 0;
    }

    pstDataRecv->pstCacheNode = pstDataRecv->pstCacheNode->next;

    return HI_SUCCESS;
}

static HI_VOID Cache_Reset(SUBT_DATA_RECV_S* pstDataRecv)
{
    HI_U8 i = 0;

    for (i = 0; i < CACHE_NODE_NUM_MAX; i++)
    {
        pstDataRecv->astCacheNode[i].u32DataSize = 0;
    }

    pstDataRecv->pstCacheNode = &pstDataRecv->astCacheNode[0];
}


HI_S32 SUBT_DataRecv_Init(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 SUBT_DataRecv_DeInit(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 SUBT_DataRecv_Create(HI_U16 u16PageID, HI_U16 u16AncillaryID, HI_VOID** pphDataRecv)
{
    HI_S32 s32Ret = HI_SUCCESS;

    SUBT_DATA_RECV_S* pstDataRecv = NULL;

    if (NULL == pphDataRecv)
    {
        HI_ERR_SUBT("param is NULL...\n");

        return HI_FAILURE;
    }

    pstDataRecv = (SUBT_DATA_RECV_S*)malloc(sizeof(SUBT_DATA_RECV_S));

    if (NULL == pstDataRecv)
    {
        HI_ERR_SUBT("malloc data struct failure...\n");

        return HI_FAILURE;
    }

    memset(pstDataRecv, 0, sizeof(SUBT_DATA_RECV_S));

    pstDataRecv->u16PageID = u16PageID;
    pstDataRecv->u16AncillaryID = u16AncillaryID;
    pstDataRecv->hDataParse = 0;

#if SUBT_USED_PES_BUFFER
    pstDataRecv->pu8WriteDataAddr = pstDataRecv->au8PesBuffer;
    pstDataRecv->u32PESWritenLen = 0;
#endif

    Cache_Init(pstDataRecv);

    *pphDataRecv = pstDataRecv;

    HI_INFO_SUBT("success, with handle:0x%08x!\n", *pphDataRecv);

    return s32Ret;
}

HI_S32 SUBT_DataRecv_Destroy(HI_VOID* hDataRecv)
{
    SUBT_DATA_RECV_S* pstDataRecv = (SUBT_DATA_RECV_S*)hDataRecv;

    if (NULL == pstDataRecv)
    {
        HI_ERR_SUBT("param is NULL...\n");

        return HI_FAILURE;
    }

    HI_INFO_SUBT("begin to use handle:0x%08x!\n", pstDataRecv);

    free((void*)pstDataRecv);
    pstDataRecv = HI_NULL;

    HI_INFO_SUBT("success!\n");

    return HI_SUCCESS;
}

HI_S32 SUBT_DataRecv_Reset(HI_VOID* hDataRecv, HI_BOOL bRecvFlag)
{
    SUBT_DATA_RECV_S* pstDataRecv = (SUBT_DATA_RECV_S*)hDataRecv;

    if (NULL == pstDataRecv)
    {
        HI_ERR_SUBT("param is NULL...\n");

        return HI_FAILURE;
    }

    if (HI_TRUE == bRecvFlag)
    {

#if SUBT_USED_PES_BUFFER
        memset(pstDataRecv->au8PesBuffer, 0, sizeof(pstDataRecv->au8PesBuffer));
        pstDataRecv->pu8WriteDataAddr = pstDataRecv->au8PesBuffer;
        pstDataRecv->u32PESWritenLen = 0;
#endif

        Cache_Reset(pstDataRecv);
    }

    return HI_SUCCESS;
}

HI_S32 SUBT_DataRecv_Updata(HI_VOID* hDataRecv, HI_U16 u16PageID, HI_U16 u16AncillaryID)
{
    SUBT_DATA_RECV_S* pstDataRecv = (SUBT_DATA_RECV_S*)hDataRecv;

    if (NULL == pstDataRecv)
    {
        HI_ERR_SUBT("param is NULL...\n");

        return HI_FAILURE;
    }

    pstDataRecv->u16PageID = u16PageID;
    pstDataRecv->u16AncillaryID = u16AncillaryID;

    HI_INFO_SUBT("successfully update page id to %d and ancillary id to %d\n", u16PageID, u16AncillaryID);

    return HI_SUCCESS;
}

HI_S32 SUBT_DataRecv_BindParsing(HI_VOID* hDataRecv, HI_VOID* hDataParse)
{
    SUBT_DATA_RECV_S* pstDataRecv = (SUBT_DATA_RECV_S*)hDataRecv;

    if (NULL == pstDataRecv)
    {
        HI_ERR_SUBT("param is NULL...\n");

        return HI_FAILURE;
    }

    pstDataRecv->hDataParse = hDataParse;

    return HI_SUCCESS;
}

HI_S32 SUBT_DataRecv_UnbindParsing(HI_VOID* hDataRecv)
{
    SUBT_DATA_RECV_S* pstDataRecv = (SUBT_DATA_RECV_S*)hDataRecv;

    if (NULL == pstDataRecv)
    {
        HI_ERR_SUBT("param is NULL...\n");

        return HI_FAILURE;
    }

    pstDataRecv->hDataParse = 0;

    return HI_SUCCESS;
}

HI_S32 SUBT_DataRecv_Redo(HI_VOID* hDataRecv)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SUBT_DATA_RECV_S* pstDataRecv = (SUBT_DATA_RECV_S*)hDataRecv;
    HI_U8* pu8Data = NULL;
    HI_U32 u32DataSize = 0;

    if (NULL == pstDataRecv)
    {
        HI_ERR_SUBT("param is NULL...\n");

        return HI_FAILURE;
    }


    /* redo parse cache pes data */
    do
    {
        s32Ret = Cache_Get(pstDataRecv, &pu8Data, &u32DataSize);

        if ((HI_SUCCESS == s32Ret) && u32DataSize && pu8Data)
        {
            if (pstDataRecv->hDataParse)
            {
                /*6 is the startcode(4 bytes) plus PES_packet_length field(2 bytes), DataSize include this 6 byte,*/
                s32Ret |= SUBT_DataParse_ParsePESPacket(pstDataRecv->hDataParse, pu8Data, (u32DataSize - 6),
                                                        pstDataRecv->u16PageID, pstDataRecv->u16AncillaryID);

                if (s32Ret != HI_SUCCESS)
                {
                    HI_ERR_SUBT("failed to SUBT_DataParse_ParsePESPacket...\n");
                }
            }
        }
    }
    while (u32DataSize);

    return HI_SUCCESS;
}


HI_S32 SUBT_DataRecv_Inject(HI_VOID* hDataRecv, HI_U8* pu8Data, HI_U32 u32DataSize)
{
    HI_S32 s32Ret = HI_SUCCESS;
#if SUBT_USED_PES_BUFFER
    HI_U32 u32RemainSpaceByte = 0;
#endif
    HI_U32 u32PESPayloadLength = 0;

    SUBT_DATA_RECV_S* pstDataRecv = (SUBT_DATA_RECV_S*)hDataRecv;

    if (NULL == pstDataRecv || NULL == pu8Data || 0 == u32DataSize)
    {
        HI_ERR_SUBT("parameter is invalid...\n");

        return HI_FAILURE;
    }

#if SUBT_USED_PES_BUFFER

    if (pstDataRecv->u32PESWritenLen == 0)
    {
        /*
              * 1. Ref[13818-1] 2.4.3.7 PES packet
              * PES packet_start_code_prefix:00 00 01(24bits), following stream_id, which constitutes the start code marking the start of packet
              * 2. Ref[Subtitling system.pdf] section 6. PES packet format
              * stream_id : 1011 1101[0xbd] standard for subtitle stream.
              */
        if (   (pu8Data[0] == 0x00) && (pu8Data[1] == 0x00)
               && (pu8Data[2] == 0x01) && (pu8Data[3] == 0xbd))
        {
            if (u32DataSize <= SUBT_MAX_PES_PACKET_LEN)
            {
                pstDataRecv->pu8WriteDataAddr = pstDataRecv->au8PesBuffer;

                memcpy(pstDataRecv->pu8WriteDataAddr, pu8Data, u32DataSize);

                pstDataRecv->pu8WriteDataAddr += u32DataSize;
                pstDataRecv->u32PESWritenLen = u32DataSize;
            }
            else
            {
                HI_WARN_SUBT("SUBT PES PACKET IS TOO LARGE!!\n");
            }
        }
        else
        {
            HI_INFO_SUBT("NOT SUBTITLE PES START CODE!!!!!\n");
        }
    }
    else
    {
        u32RemainSpaceByte = &pstDataRecv->au8PesBuffer[SUBT_MAX_PES_PACKET_LEN] - pstDataRecv->pu8WriteDataAddr;

        if (u32DataSize <= u32RemainSpaceByte)
        {
            memcpy(pstDataRecv->pu8WriteDataAddr, pu8Data, u32DataSize);

            pstDataRecv->pu8WriteDataAddr += u32DataSize;
            pstDataRecv->u32PESWritenLen += u32DataSize;
        }
        else
        {
            HI_WARN_SUBT("\nSUBTITLE SECTION PACKET IS TOO LARGE!!\n");

            /* discard the received data, reset pes buffer */
            pstDataRecv->pu8WriteDataAddr = pstDataRecv->au8PesBuffer;
            pstDataRecv->u32PESWritenLen = 0;
        }
    }

    if (pstDataRecv->u32PESWritenLen >= 6)
    {
        /* PES packet_length(payload size): A 16-bit field after start code(24-bit) and stream_id(8-bit), that is, the fifth and sixth byte */
        u32PESPayloadLength = (pstDataRecv->au8PesBuffer[4] << 8) | pstDataRecv->au8PesBuffer[5];

        /* The whole pes data received ok */
        if (pstDataRecv->u32PESWritenLen >= (u32PESPayloadLength + 6))
        {
            HI_INFO_SUBT("PayloadLen=0x%x,PageID=%d,AncillaryID=%d\n",
                         u32PESPayloadLength, pstDataRecv->u16PageID, pstDataRecv->u16AncillaryID);

            if (u32PESPayloadLength + 6 <= SUBT_PES_PACKET_LEN )
            {
                s32Ret = Cache_Put(pstDataRecv, pu8Data, u32PESPayloadLength + 6);
            }

            /* started  parsing pes packet data  */
            if (pstDataRecv->hDataParse)
            {
                s32Ret = SUBT_DataParse_ParsePESPacket(pstDataRecv->hDataParse, pstDataRecv->au8PesBuffer,
                                                       u32PESPayloadLength, pstDataRecv->u16PageID, pstDataRecv->u16AncillaryID);

                if (s32Ret != HI_SUCCESS)
                {
                    HI_ERR_SUBT("failed to SUBT_DataParse_ParsePESPacket...\n");
                }
            }

            memset(pstDataRecv->au8PesBuffer, 0, sizeof(pstDataRecv->au8PesBuffer));
            pstDataRecv->pu8WriteDataAddr = pstDataRecv->au8PesBuffer;
            pstDataRecv->u32PESWritenLen = 0;

            return s32Ret;
        }
    }

#else

    if (u32DataSize < 6)
    {
        HI_ERR_SUBT("u32DataSize < 6...\n");

        return HI_FAILURE;
    }

    if ((pu8Data[0] == 0x00) && (pu8Data[1] == 0x00)
        && (pu8Data[2] == 0x01) && (pu8Data[3] == 0xbd) )
    {
        u32PESPayloadLength = (pu8Data[4] << 8) | pu8Data[5];

        if (u32DataSize >= (u32PESPayloadLength + 6))
        {
            HI_INFO_SUBT("PayloadLen=0x%x,PageID=%d,AncillaryID=%d\n",
                         u32PESPayloadLength, pstDataRecv->u16PageID, pstDataRecv->u16AncillaryID);

            if (u32PESPayloadLength + 6 <= SUBT_PES_PACKET_LEN )
            {
                s32Ret = Cache_Put(pstDataRecv, pu8Data, u32PESPayloadLength + 6);
            }

            /* started  parsing pes packet data  */
            if (pstDataRecv->hDataParse)
            {
                s32Ret = SUBT_DataParse_ParsePESPacket(pstDataRecv->hDataParse, pu8Data,
                                                       u32PESPayloadLength, pstDataRecv->u16PageID, pstDataRecv->u16AncillaryID);

                if (s32Ret != HI_SUCCESS)
                {
                    HI_ERR_SUBT("failed to SUBT_DataParse_ParsePESPacket...\n");

                    return s32Ret;
                }
            }
        }
    }

#endif

    return HI_SUCCESS;
}


