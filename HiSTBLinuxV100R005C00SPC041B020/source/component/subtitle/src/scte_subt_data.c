#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hi_type.h"
#include "subtitle_debug.h"
#include "scte_subt_data.h"
#include "scte_subt_parse.h"

#define HI_SECTION_MAX_SIZE (0x10000) /* 64K */
#define SCTE_SECTION_MAX_SIZE (0x400)  /*1K*/
#define HI_SECTION_MAX_NUM (64)

typedef struct tagSCTE_SUBT_SECTION_S
{
    HI_U8   au8Data[SCTE_SECTION_MAX_SIZE];
    HI_U32  u32DataSize;
    HI_U16  u16TableExtention;
    HI_BOOL bUsed;
} SCTE_SUBT_SECTION_S;

typedef struct tagSCTE_SUBT_DATA_RECV_S
{
    HI_VOID*           hParse;
    SCTE_SUBT_SECTION_S astSCTESection[HI_SECTION_MAX_NUM];
    HI_U8               au8SectionAddr[HI_SECTION_MAX_SIZE];
    HI_U8*               pu8SectionWriteAddr;
    HI_U32              u32SectionSize;
} SCTE_SUBT_DATA_RECV_S;

static HI_VOID BufferReset(SCTE_SUBT_DATA_RECV_S* pstDataRecv)
{
    HI_U32 i = 0;

    for (i = 0; i < HI_SECTION_MAX_NUM; i++)
    {
        if (pstDataRecv->astSCTESection[i].bUsed)
        {
            pstDataRecv->astSCTESection[i].bUsed = HI_FALSE;
        }
    }
}

HI_S32 SCTE_SUBT_Data_Init(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 SCTE_SUBT_Data_DeInit(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 SCTE_SUBT_Data_Create(HI_VOID* hParse, HI_VOID** pphData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SCTE_SUBT_DATA_RECV_S* pstDataRecv = HI_NULL;

    pstDataRecv = (SCTE_SUBT_DATA_RECV_S*)malloc(sizeof(SCTE_SUBT_DATA_RECV_S));

    if (HI_NULL == pstDataRecv)
    {
        HI_ERR_SUBT("malloc data struct failure!!\n");
        return HI_FAILURE;
    }

    memset(pstDataRecv, 0, sizeof(SCTE_SUBT_DATA_RECV_S));
    pstDataRecv->hParse = hParse;
    *pphData = pstDataRecv;

    HI_INFO_SUBT("SCTE_SUBT_Data_Create success, with handle:0x%08x!...\n", *pphData);

    return s32Ret;
}

HI_S32 SCTE_SUBT_Data_Destroy(HI_VOID* hData)
{
    SCTE_SUBT_DATA_RECV_S* pstDataRecv = (SCTE_SUBT_DATA_RECV_S*)hData;

    if (HI_NULL == pstDataRecv)
    {
        HI_ERR_SUBT("SCTE_SUBT_Data_Destroy  param is HI_NULL!!\n");
        return HI_FAILURE;
    }

    HI_INFO_SUBT("begin to use handle:0x%08x!...\n", pstDataRecv);

    free((void*)pstDataRecv);
    pstDataRecv = HI_NULL;

    HI_INFO_SUBT("SCTE_SUBT_Data_Destroy  success...\n");

    return HI_SUCCESS;
}

HI_S32 SCTE_SUBT_Data_Reset(HI_VOID* hData)
{
    SCTE_SUBT_DATA_RECV_S* pstDataRecv = (SCTE_SUBT_DATA_RECV_S*)hData;

    if (HI_NULL == pstDataRecv)
    {
        HI_ERR_SUBT("param is HI_NULL!!\n");
        return HI_FAILURE;
    }

    BufferReset(pstDataRecv);

    return HI_SUCCESS;
}

HI_S32 SCTE_SUBT_Data_Inject(HI_VOID* hData, const HI_U8* pu8Data, HI_U32 u32DataSize)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U16 u16TableExtension = 0;
    HI_U16 u16LastSegment   = 0;
    HI_U16 u16SegmentNumber = 0;
    HI_U8  i = 0;
    SCTE_SUBT_DATA_RECV_S* pstDataRecv = (SCTE_SUBT_DATA_RECV_S*)hData;

    if ((HI_NULL == pstDataRecv) || (HI_NULL == pu8Data) || (0 == u32DataSize))
    {
        HI_ERR_SUBT("parameter is invalid!!\n");
        return HI_FAILURE;
    }

    if (u32DataSize > SCTE_SECTION_MAX_SIZE)
    {
        HI_ERR_SUBT("Data Size too long!!\n");
        return HI_FAILURE;
    }

    pstDataRecv->pu8SectionWriteAddr = pstDataRecv->au8SectionAddr;

    /*scte subtitle table_id:0xC6*/
    if (pu8Data[0] == 0xC6)
    {
        /*if the section is segmented*/
        if (pu8Data[3] & 0x40)
        {
            u16TableExtension = (pu8Data[4] << 8) | pu8Data[5];
            u16LastSegment   = ((pu8Data[6] << 8) | pu8Data[7]) >> 4;
            u16SegmentNumber = ((pu8Data[7] << 8) | pu8Data[8]) & 0x0fff;

            for (i = 0; i < HI_SECTION_MAX_NUM; i++)
            {
                if (pstDataRecv->astSCTESection[i].bUsed == HI_FALSE)
                {
                    if (u16SegmentNumber == 0)      // first segment
                    {
                        /*skip 5bytes indicates if is segmented*/

                        memcpy(pstDataRecv->astSCTESection[i].au8Data, pu8Data, 4);
                        memcpy(pstDataRecv->astSCTESection[i].au8Data + 4, pu8Data + 9, u32DataSize - 9 - 4);

                        pstDataRecv->astSCTESection[i].u32DataSize = u32DataSize - 9;
                        pstDataRecv->astSCTESection[i].u16TableExtention = u16TableExtension;

                    }
                    else if (u16SegmentNumber == u16LastSegment)
                    {
                        /*skip 5bytes indicates if is segmented and 4bytes header*/
                        memcpy(pstDataRecv->astSCTESection[i].au8Data, pu8Data + 9, u32DataSize - 9);
                        pstDataRecv->astSCTESection[i].u32DataSize = u32DataSize - 9;
                        pstDataRecv->astSCTESection[i].u16TableExtention = u16TableExtension;

                    }
                    else
                    {
                        memcpy(pstDataRecv->astSCTESection[i].au8Data, pu8Data + 9, u32DataSize - 9 - 4);
                        pstDataRecv->astSCTESection[i].u32DataSize = u32DataSize - 9 - 4;
                        pstDataRecv->astSCTESection[i].u16TableExtention = u16TableExtension;
                    }

                    pstDataRecv->astSCTESection[i].bUsed = HI_TRUE;
                    break;
                }
            }

            if (u16LastSegment == u16SegmentNumber)
            {
                pstDataRecv->pu8SectionWriteAddr = pstDataRecv->au8SectionAddr;

                for (i = 0; i < HI_SECTION_MAX_NUM; i++)
                {
                    if (pstDataRecv->astSCTESection[i].bUsed
                        && (pstDataRecv->astSCTESection[i].u16TableExtention == u16TableExtension))
                    {
                        memcpy(pstDataRecv->pu8SectionWriteAddr, pstDataRecv->astSCTESection[i].au8Data,
                               pstDataRecv->astSCTESection[i].u32DataSize);
                        pstDataRecv->pu8SectionWriteAddr += pstDataRecv->astSCTESection[i].u32DataSize;
                        pstDataRecv->astSCTESection[i].bUsed = HI_FALSE;
                    }
                }

                pstDataRecv->u32SectionSize = (HI_U32)(pstDataRecv->pu8SectionWriteAddr - pstDataRecv->au8SectionAddr /* - 1 TODO*/);

                /* update section size in head */
                pstDataRecv->au8SectionAddr[2] = (HI_U8)(pstDataRecv->u32SectionSize - 3);
                pstDataRecv->au8SectionAddr[1] = (HI_U8)((pstDataRecv->u32SectionSize - 3) >> 8);

                if (pstDataRecv->u32SectionSize > HI_SECTION_MAX_SIZE)
                {
                    HI_ERR_SUBT("Section Size too long!!\n");
                    return HI_FAILURE;
                }

                //buffer over,reset
                //BufferReset(pstDataRecv);

                if (pstDataRecv->hParse)
                {
                    s32Ret = SCTE_SUBT_Parse_ParseSection(pstDataRecv->hParse, pstDataRecv->au8SectionAddr,
                                                          pstDataRecv->u32SectionSize);

                    if (s32Ret != HI_SUCCESS)
                    {
                        HI_ERR_SUBT("Failed to SCTE_SUBT_Parse_ParseSection...\n");

                        return s32Ret;
                    }
                }
            }
        }
        else  //not segmented
        {
            memcpy(pstDataRecv->au8SectionAddr, pu8Data, u32DataSize);
            pstDataRecv->u32SectionSize = u32DataSize;

            if (pstDataRecv->hParse)
            {
                s32Ret = SCTE_SUBT_Parse_ParseSection(pstDataRecv->hParse, pstDataRecv->au8SectionAddr,
                                                      pstDataRecv->u32SectionSize);

                if (s32Ret != HI_SUCCESS)
                {
                    HI_ERR_SUBT("Failed to SCTE_SUBT_Parse_ParseSection...\n");

                    return s32Ret;
                }
            }
        }
    }
    else  /* pu8Data[0] is not 0xC6 */
    {
        HI_ERR_SUBT("No scte subtitle section data!!\n");
        return HI_FAILURE;
    }

    if (pstDataRecv->u32SectionSize < HI_SECTION_MAX_SIZE)
    {
        HI_INFO_SUBT("Success to desegmentation...\n");
    }
    else
    {
        HI_ERR_SUBT("Section is too large!!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
