/***********************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
* FileName   :  teletext_recv.c
* Description:  Define functions used to reveive date of teletext
*
* History:
* Version      Date             Author     DefectNum    Description
* main\1    2012-03-12                      NULL      Create this file.
***********************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "hi_unf_disp.h"
#include "hi_error_mpi.h"

#include "teletext_debug.h"
#include "teletext_data.h"
#include "teletext_recv.h"
#include "teletext_parse.h"
#include "teletext_mem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif  /* __cplusplus */

#define TTX_RECV_LOCK()  \
    do{\
        int ret = pthread_mutex_lock(&pstThisElem->mutex_recv_lock);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_lock(RECV) failure ret = 0x%x\n",ret);\
        }\
    }while(0)

#define TTX_RECV_UNLOCK()  \
    do{\
        int ret = pthread_mutex_unlock(&pstThisElem->mutex_recv_lock);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_unlock(RECV) failure ret = 0x%x\n",ret);\
        }\
    }while(0)


#define TTX_RECV_MAX_MAGAZINE_NUM (9)

static HI_VOID* s_hGlobalHandle;

static HI_U32 g_u32PTS  = 0;
static HI_S8 g_s8serial = -1;
static HI_U8 s_u8PreviousIndex = 9;
static TTX_PAGE_S g_astpage[TTX_RECV_MAX_MAGAZINE_NUM];
static HI_UNF_TTX_RAWDATA_S g_astRawData[TTX_RECV_MAX_MAGAZINE_NUM];

static HI_U8 s_u8aInvHamming8_4Tab[256] =
{
    0x01, 0xFF, 0xFF, 0x08, 0xFF, 0x0C, 0x04, 0xFF,
    0xFF, 0x08, 0x08, 0x08, 0x06, 0xFF, 0xFF, 0x08,
    0xFF, 0x0A, 0x02, 0xFF, 0x06, 0xFF, 0xFF, 0x0F,
    0x06, 0xFF, 0xFF, 0x08, 0x06, 0x06, 0x06, 0xFF,
    0xFF, 0x0A, 0x04, 0xFF, 0x04, 0xFF, 0x04, 0x04,
    0x00, 0xFF, 0xFF, 0x08, 0xFF, 0x0D, 0x04, 0xFF,
    0x0A, 0x0A, 0xFF, 0x0A, 0xFF, 0x0A, 0x04, 0xFF,
    0xFF, 0x0A, 0x03, 0xFF, 0x06, 0xFF, 0xFF, 0x0E,
    0x01, 0x01, 0x01, 0xFF, 0x01, 0xFF, 0xFF, 0x0F,
    0x01, 0xFF, 0xFF, 0x08, 0xFF, 0x0D, 0x05, 0xFF,
    0x01, 0xFF, 0xFF, 0x0F, 0xFF, 0x0F, 0x0F, 0x0F,
    0xFF, 0x0B, 0x03, 0xFF, 0x06, 0xFF, 0xFF, 0x0F,
    0x01, 0xFF, 0xFF, 0x09, 0xFF, 0x0D, 0x04, 0xFF,
    0xFF, 0x0D, 0x03, 0xFF, 0x0D, 0x0D, 0xFF, 0x0D,
    0xFF, 0x0A, 0x03, 0xFF, 0x07, 0xFF, 0xFF, 0x0F,
    0x03, 0xFF, 0x03, 0x03, 0xFF, 0x0D, 0x03, 0xFF,
    0xFF, 0x0C, 0x02, 0xFF, 0x0C, 0x0C, 0xFF, 0x0C,
    0x00, 0xFF, 0xFF, 0x08, 0xFF, 0x0C, 0x05, 0xFF,
    0x02, 0xFF, 0x02, 0x02, 0xFF, 0x0C, 0x02, 0xFF,
    0xFF, 0x0B, 0x02, 0xFF, 0x06, 0xFF, 0xFF, 0x0E,
    0x00, 0xFF, 0xFF, 0x09, 0xFF, 0x0C, 0x04, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x0E,
    0xFF, 0x0A, 0x02, 0xFF, 0x07, 0xFF, 0xFF, 0x0E,
    0x00, 0xFF, 0xFF, 0x0E, 0xFF, 0x0E, 0x0E, 0x0E,
    0x01, 0xFF, 0xFF, 0x09, 0xFF, 0x0C, 0x05, 0xFF,
    0xFF, 0x0B, 0x05, 0xFF, 0x05, 0xFF, 0x05, 0x05,
    0xFF, 0x0B, 0x02, 0xFF, 0x07, 0xFF, 0xFF, 0x0F,
    0x0B, 0x0B, 0xFF, 0x0B, 0xFF, 0x0B, 0x05, 0xFF,
    0xFF, 0x09, 0x09, 0x09, 0x07, 0xFF, 0xFF, 0x09,
    0x00, 0xFF, 0xFF, 0x09, 0xFF, 0x0D, 0x05, 0xFF,
    0x07, 0xFF, 0xFF, 0x09, 0x07, 0x07, 0x07, 0xFF,
    0xFF, 0x0B, 0x03, 0xFF, 0x07, 0xFF, 0xFF, 0x0E
};

static HI_S32  TTX_Recv_ParsePESPacket(HI_VOID* hData, HI_U8* pu8DataSrc, HI_U32 u32PayloadLen);
static HI_S32  TTX_Recv_ParseRAWPacket(HI_VOID* hData, HI_U8* pu8DataSrc, HI_U32 u32PacketLength);
static HI_S32  TTX_Recv_HandleFilter(const HI_VOID* hDataRecv, const HI_VOID* hDataParse,
                                     const TTX_Filter_S_PTR pstFilter);
static HI_S32  TTX_Recv_CheckPESStartCode(HI_U8* pu8SegmentData, HI_U32 u32Len, HI_U32* pu32Offset);
static HI_S32  TTX_Recv_OSDOutput(TTX_CONTEXT_S_PTR pstThisElem, HI_U8 u8Magazine, HI_U8 u8Page, TTX_PAGE_S* pstpage);
static HI_S32  TTX_Recv_OrganisePage(TTX_CONTEXT_S_PTR pstThisElem, HI_U8 u8MagazineFlag,
                                     TTX_PAGE_S_PTR pstPage,
                                     HI_U8* pu8Packet);
static HI_S32 TTX_Recv_OutputRawData(TTX_CONTEXT_S_PTR pstThisElem, HI_U8 u8MagazineFlag, HI_U8* pu8Packet);

static HI_S32 TTX_Recv_CheckPESStartCode(HI_U8* pu8SegmentData, HI_U32 u32Len, HI_U32* pu32Offset)
{
    HI_U32 i = 0;

    if (u32Len < MIN_TTX_PES_LEN)
    {
        return HI_FAILURE;
    }

    /*
     * 1. Ref[13818-1] 2.4.3.7 PES packet
     * PES packet_start_code_prefix:00 00 01(24bits), following stream_id, which constitutes the start code marking the start of packet
     * 2. Ref[Subtitling system.pdf] section 6. PES packet format
     * stream_id : 1011 1101[0xbd] standard for subtitle stream.
     */
    for (i = 0; (i + 3) < u32Len; i++)
    {
        if ((pu8SegmentData[i + 0] == 0x00)
            && (pu8SegmentData[i + 1] == 0x00)
            && (pu8SegmentData[i + 2] == 0x01)
            && (pu8SegmentData[i + 3] == 0xbd))
        {
            *pu32Offset = i;

            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}

static HI_S32 TTX_Recv_OSDOutput(TTX_CONTEXT_S_PTR pstThisElem, HI_U8 u8Magazine, HI_U8 u8Page, TTX_PAGE_S* pstpage)
{
    TTX_SEGMENT_S stSegment;
    HI_BOOL bSendPageToOSD = HI_FALSE;

    if ((HI_NULL == pstThisElem) || (0 == pstpage->u32ValidLines) || (u8Magazine > 7)/* || (u8Page > 99)*/)
    {
        return HI_FAILURE;
    }

    memset(&stSegment, 0x0, sizeof(TTX_SEGMENT_S));
    
    if (pstThisElem->pstFiltersPtr->u32OpenOff != TTX_REQ_UNUSED)
    {
        if (pstThisElem->bHold)
        {
            bSendPageToOSD = HI_FALSE;
            HI_WARN_TTX("Hold osd!!\n");
        }
        else if ((u8Magazine == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u8MagazineNum)
                 && (u8Page == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u8PageNum)
                 && (pstpage->u32PTS != 0))
        {
            bSendPageToOSD = HI_TRUE;
        }
        else if ((u8Magazine == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u8MagazineNum)
                 && (u8Page == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u8PageNum)
                 && (pstpage->u16subcode == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u16PageSubCode)
                 && (pstpage->u32PTS == 0) && (pstpage->u32ValidLines > 1))
        {
            bSendPageToOSD = HI_TRUE;
        }
        /*not find the initial page,then find the page which magine num and page num equal to the initial page*/
        else if ((u8Magazine == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u8MagazineNum)
                 && (u8Page == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u8PageNum)
                 && (pstpage->u32PTS == 0) && (pstpage->u32ValidLines > 1))
        {
            TTX_RECV_LOCK();

            if ((pstThisElem->u32NumActiveRequests == 0) && (!pstThisElem->bAutoPlay))
            {
                HI_WARN_TTX("this page already send to OSD!!\n");
            }
            else
            {
                bSendPageToOSD = HI_TRUE;
                pstThisElem->u32NumActiveRequests = 0;
                /*set the subpage to new*/
                pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u16PageSubCode = pstpage->u16subcode;
            }

            TTX_RECV_UNLOCK();
        }
        else if ((u8Magazine == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u8MagazineNum)
                 && (u8Page == pstThisElem->pstFiltersPtr->stFilter.stReqPgAddr.u8PageNum)
                 && (pstpage->u32PTS == 0) && (pstpage->u32ValidLines == 1) && (TTX_IsEraseFlagSet(pstpage)))
        {
            bSendPageToOSD = HI_TRUE;
        }
        else
        {
            HI_WARN_TTX("this page is not wanted!!\n");
        }

        if (bSendPageToOSD)
        {
            stSegment.pu8SegmentData = (HI_U8*)pstpage;
            stSegment.u16DataLength = sizeof(TTX_PAGE_S);
            (HI_VOID)(pstThisElem->pstFiltersPtr->stFilter.NotifyFunction)(s_hGlobalHandle, &stSegment);
        }

        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 TTX_Recv_HandleFilter(const HI_VOID* hDataRecv, const HI_VOID* hDataParse, const TTX_Filter_S_PTR pstFilter)
{
    HI_U8 u8MagNum = 0;
    HI_U8 u8PageNum = 0;
    HI_U16 u16SubpageNum = 0;
    TTX_PAGE_S stThispage;
    TTX_SEGMENT_S stSegment;
    TTX_CONTEXT_S_PTR pstThisElem = HI_NULL;

    TTX_RECV_CHECK(hDataRecv);
    TTX_RECV_CHECK(hDataParse);
    TTX_RECV_CHECK(pstFilter);

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;

    if (HI_FAILURE == TTX_Parse_GetPgAddr(pstFilter->stReqPgAddr, &u8MagNum, &u8PageNum, &u16SubpageNum))
    {
        return HI_FAILURE;
    }

    memset(&stThispage, 0, sizeof(TTX_PAGE_S));
    memset(&stSegment, 0, sizeof(TTX_SEGMENT_S));

    if (HI_SUCCESS == TTX_Data_Search(u8MagNum, u8PageNum, u16SubpageNum, &stThispage))
    {
        if (pstThisElem->bOpen)
        {
            stSegment.pu8SegmentData = (HI_U8*)&stThispage;
            stSegment.u16DataLength = sizeof(TTX_PAGE_S);
            return (pstFilter->NotifyFunction)(hDataParse, &stSegment);
        }
    }

    return HI_FAILURE;
}

HI_S32 TTX_Recv_ProcessData(HI_VOID* hDataRecv, HI_U8* pu8Data, HI_U32 u32DataLength)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32StartCodeOffset  = 0;
    HI_U32 u32RemainSpaceByte  = 0;
    HI_U32 u32PESPayloadLength = 0;
    TTX_CONTEXT_S_PTR pstThisElem = HI_NULL;                      /* main data pointer */
    TTX_PAGE_FILTER_STATUS_E eFilterStatus = TELETEXT_PAGE_PES_INIT;

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;

    if (pstThisElem->enPacketType == HI_UNF_TTX_PACKET_TYPE_RAW)
    {
        if (pstThisElem->bOpen)
        {
            s32Ret = TTX_Recv_ParseRAWPacket(pstThisElem, pu8Data, u32DataLength);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TTX("\n TTX_Recv_ParseETSIEN300706Packet failed!!\n");
            }
        }

        return s32Ret;
    }

    eFilterStatus = pstThisElem->enPageFilterStatus;

    switch (eFilterStatus)
    {
        case TELETEXT_PAGE_PES_INIT:
        {
            if (HI_SUCCESS == TTX_Recv_CheckPESStartCode(pu8Data, u32DataLength, &u32StartCodeOffset))
            {
                if (u32DataLength <= TTX_PES_PACKET_LEN)
                {
                    pstThisElem->pu8WriteDataAddr   = pstThisElem->u8szPesBuffer;
                    pstThisElem->enPageFilterStatus = TELETEXT_PAGE_PES_HEAD_RECIVED;
                    memcpy( pstThisElem->pu8WriteDataAddr, &pu8Data[u32StartCodeOffset], u32DataLength - u32StartCodeOffset);
                    pstThisElem->pu8WriteDataAddr += u32DataLength - u32StartCodeOffset;
                    pstThisElem->u32PESWritenLen = u32DataLength - u32StartCodeOffset;
                }
                else
                {
                    HI_WARN_TTX("TTX PES PACKET IS TOO LARGE!!\n");
                }
            }
            else
            {
                HI_WARN_TTX("NOT TELETEXT PES START CODE!!!!!\n");
            }
        }
        break;

        case TELETEXT_PAGE_PES_HEAD_RECIVED:
        {
            u32RemainSpaceByte = (HI_U32)(&pstThisElem->u8szPesBuffer[TTX_PES_PACKET_LEN] - pstThisElem->pu8WriteDataAddr);

            if (u32DataLength <= u32RemainSpaceByte)
            {
                memcpy(pstThisElem->pu8WriteDataAddr, pu8Data, u32DataLength);
                pstThisElem->pu8WriteDataAddr += u32DataLength;
                pstThisElem->u32PESWritenLen += u32DataLength;
            }
            else
            {
                HI_WARN_TTX("\nTELETEXT SECTION PACKET IS TOO LARGE!!\n");
            }
        }
        break;

        default:
        {
            HI_WARN_TTX("Filter status is unknown:%u\n", eFilterStatus);
        }
        break;
    }

    if (pstThisElem->enPageFilterStatus == TELETEXT_PAGE_PES_HEAD_RECIVED)
    {
        if (pstThisElem->u32PESWritenLen >= 6)
        {
            u32PESPayloadLength = (pstThisElem->u8szPesBuffer[4] << 8) | pstThisElem->u8szPesBuffer[5];

            if (pstThisElem->u32PESWritenLen >= (u32PESPayloadLength + 6))
            {
                pstThisElem->enPageFilterStatus = TELETEXT_PAGE_PES_WHOLE_RECIVED;
            }
        }
    }

    if (pstThisElem->enPageFilterStatus == TELETEXT_PAGE_PES_WHOLE_RECIVED)
    {
        pstThisElem->enPageFilterStatus = TELETEXT_PAGE_PES_INIT;

        if (pstThisElem->bVBIUsed)
        {
#if defined VER_X5HD  || defined VER_V1R4
            {
                HI_UNF_DISP_TTX_DATA_S stVBIData = {0};

                stVBIData.pu8DataAddr = pstThisElem->u8szPesBuffer;
                stVBIData.u32DataLen = u32PESPayloadLength + 6;
                s32Ret = HI_UNF_DISP_SendTtxData(HI_UNF_DISP_SD0, &stVBIData);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_WARN_TTX("\n HI_UNF_DISP_SendTtxData failed!!\n");
                }
            }

#endif
#if defined VER_V1R1

            if (HI_INVALID_HANDLE != pstThisElem->hVBIHandle)
            {
                HI_UNF_DISP_VBI_DATA_S stVbiData;

                memset(&stVbiData, 0, sizeof(HI_UNF_DISP_VBI_DATA_S));
                stVbiData.pu8DataAddr = pstThisElem->u8szPesBuffer;
                stVbiData.u32DataLen  = u32PESPayloadLength + 6;
                stVbiData.eType = HI_UNF_DISP_VBI_TYPE_TTX;
                s32Ret = HI_UNF_DISP_SendVBIData(pstThisElem->hVBIHandle, &stVbiData);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_WARN_TTX("\n HI_UNF_DISP_SendVbiData failed!!\n");
                }
            }
            else
            {
                HI_WARN_TTX("\n VBI Channel is invalid!!\n");
            }

#endif

#if defined VER_V1R2 || defined VER_V1R3

            if (HI_INVALID_HANDLE != pstThisElem->hVBIHandle)
            {
                HI_UNF_DISP_VBI_DATA_S stVbiData;

                memset(&stVbiData, 0, sizeof(HI_UNF_DISP_VBI_DATA_S));
                stVbiData.pu8DataAddr = pstThisElem->u8szPesBuffer;
                stVbiData.u32DataLen  = u32PESPayloadLength + 6;
                stVbiData.enType = HI_UNF_DISP_VBI_TYPE_TTX;
                s32Ret = HI_UNF_DISP_SendVBIData(pstThisElem->hVBIHandle, &stVbiData);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_WARN_TTX("\n HI_UNF_DISP_SendVbiData failed!!\n");
                }
            }
            else
            {
                HI_WARN_TTX("\n VBI Channel is invalid!!\n");
            }

#endif



        }

        if (pstThisElem->bOpen)
        {
            s32Ret = TTX_Recv_ParsePESPacket(pstThisElem, pstThisElem->u8szPesBuffer, u32PESPayloadLength);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_TTX("\n TTX_Recv_ParsePESPacket failed!!\n");
            }
        }

        memset(pstThisElem->u8szPesBuffer, 0, TTX_PES_PACKET_LEN);

        pstThisElem->u32PESWritenLen = 0;
    }

    return s32Ret;
}

static HI_S32 TTX_Recv_ProcPreviousPage(TTX_CONTEXT_S_PTR pstThisElem, TTX_PAGE_S_PTR pstPage)
{
    HI_U8 u8PcktAddr1 = 0;
    HI_U8 u8Magazinenum = 0;
    HI_U8 u8PageNumb1 = 0;
    HI_U8 u8PageNumb2 = 0;

    /* inverse hamming 8/4 */
    HI_U8 u8SubPCode1 = 0;
    HI_U8 u8SubPCode2 = 0;
    HI_U8 u8SubPCode3 = 0;
    HI_U8 u8SubPCode4 = 0;

    HI_U32 u32SubPageN = 0;
    HI_U8 u8TmpPageNo = TTX_BAD_INV_PAGE;

    if (pstPage->u32ValidLines & 0x01)
    {
        u8PcktAddr1   = s_u8aInvHamming8_4Tab[pstPage->u8szLines[0][TTX_PACKET_ADDR_POS1 - 4]];
        u8Magazinenum = u8PcktAddr1 & TTX_MAGAZINE_MASK;

        u8PageNumb1 = s_u8aInvHamming8_4Tab[pstPage->u8szLines[0][TTX_PAGE_NO_UNIT_POS - 4]];
        u8PageNumb2 = s_u8aInvHamming8_4Tab[pstPage->u8szLines[0][TTX_PAGE_NO_TENS_POS - 4]];
        u8TmpPageNo = (HI_U8)(u8PageNumb1 | (u8PageNumb2 << TTX_NYBBLE_SHIFT_CNT));

        u8SubPCode4 = s_u8aInvHamming8_4Tab[(pstPage->u8szLines[0][TTX_SUBPAGE_THOU_POS - 4])] & 0x03;
        u8SubPCode3 = s_u8aInvHamming8_4Tab[(pstPage->u8szLines[0][TTX_SUBPAGE_HUND_POS - 4])] & 0x0F;
        u8SubPCode2 = s_u8aInvHamming8_4Tab[(pstPage->u8szLines[0][TTX_SUBPAGE_TENS_POS - 4])] & 0x07;
        u8SubPCode1 = s_u8aInvHamming8_4Tab[(pstPage->u8szLines[0][TTX_SUBPAGE_UNIT_POS - 4])] & 0x0F;

        if ((u8SubPCode4 == 0x03) && (u8SubPCode3 == 0x0F) && (u8SubPCode2 == 0x07) && (u8SubPCode1 == 0x0F))
        {
            u32SubPageN = 0;
        }
        else
        {
            u32SubPageN = u8SubPCode2 * 10 + (u8SubPCode1 > 9 ? 0 : u8SubPCode1);
        }

        if (!(s_u8aInvHamming8_4Tab[(pstPage->u8szLines[0][TTX_C6_POS])] & 0x08)) /*C6 flag*/
        {
            pstPage->u32PTS = 0;
        }

        pstPage->u16subcode = (HI_U16)u32SubPageN;

        if (pstThisElem->bOpen)
        {

            if (HI_SUCCESS != TTX_Recv_OSDOutput(pstThisElem, u8Magazinenum, u8TmpPageNo, pstPage))
            {
                HI_WARN_TTX("TTX_Recv_OSDOutput failure!!\n");
            }
        }

        /*when not subtitle, store it */
        if (!pstThisElem->bIsSubData)
        {
            /*save the page to data buffer*/
            if (HI_FAILURE == TTX_Data_In(u8Magazinenum, u8TmpPageNo, (HI_U16)u32SubPageN, pstPage))
            {
                HI_WARN_TTX("TTX_Data_In failure!!\n");
            }

            if ((u8Magazinenum == pstThisElem->stInitFilter.u8MagNum)
                && (u8TmpPageNo == pstThisElem->stInitFilter.u8PageNum))
            {
                if (HI_SUCCESS != TTX_Data_StoreInitPage(pstPage))
                {
                    HI_WARN_TTX("TTX_Data_StoreInitPage failure!!\n");
                }
            }
        }
    }

    return HI_FAILURE;
}
static HI_S32 TTX_Recv_ParseRAWPacket(HI_VOID* hData, HI_U8* pu8DataSrc, HI_U32 u32PacketLength)
{
    HI_U8*  pData = HI_NULL;
    HI_U8  u8PcktAddr1 = 0;
    HI_U8 u8PcktAddr2 = 0;
    HI_U8 u8PacketNo = 0;
    HI_U8 u8Magazine = TTX_BAD_INV_MAGZINE;
    HI_U32 u32ExtractPoint = 0;
    HI_U8  u8szCurrentLine[TTX_LINE_SIZE] = {0};
    TTX_CONTEXT_S_PTR pstThisElem = HI_NULL;
    static HI_U8  s_au8LastLine[TTX_LINE_MEM_SIZE] = {0};

    TTX_RECV_CHECK(pu8DataSrc);
    TTX_RECV_CHECK(hData);

    pData = pu8DataSrc;
    pstThisElem = (TTX_CONTEXT_S_PTR)hData;

    u32ExtractPoint = 0;       /* Teletext data start */

    while (u32ExtractPoint < u32PacketLength)    /* loop on line data */
    {
        memset(u8szCurrentLine, 0, TTX_LINE_SIZE);

        u8PcktAddr1 = s_u8aInvHamming8_4Tab[(pData[(u32ExtractPoint + TTX_PACKET_ADDR_POS1 - 1)])];
        u8PcktAddr2 = s_u8aInvHamming8_4Tab[(pData[(u32ExtractPoint + TTX_PACKET_ADDR_POS2 - 1)])];

        if ((u8PcktAddr1 | u8PcktAddr2) == TTX_BAD_INV_HAMMING)     /*packet numbet error*/
        {
            HI_ERR_TTX("[%2X, %2X] : u8PcktAddr1 = %d, u8PcktAddr2 = %d, TTX_BAD_INV_HAMMING, continue",
                       (pData[(u32ExtractPoint + TTX_PACKET_ADDR_POS1 - 1)]),
                       (pData[(u32ExtractPoint + TTX_PACKET_ADDR_POS2 - 1)]),
                       u8PcktAddr1, u8PcktAddr2);
            u32ExtractPoint += TTX_EN300706_PACKET_SIZE;
            continue;
        }

        u8PacketNo = (HI_U8)((u8PcktAddr1 >> 3) | (u8PcktAddr2 << 1));     /*Packetnum*/
        u8Magazine = u8PcktAddr1 & TTX_MAGAZINE_MASK;              /*Magzinenum*/

        memcpy(&u8szCurrentLine[1], &pData[u32ExtractPoint], TTX_EN300706_PACKET_SIZE);

        if (strncmp((const char*)(&u8szCurrentLine[1 + 3]), (const char*)s_au8LastLine, TTX_LINE_MEM_SIZE) == 0)
        {
            HI_ERR_TTX("aaa, discard repeated packet\n");
            u32ExtractPoint += TTX_EN300706_PACKET_SIZE;
            continue;
        }
        else
        {
            memcpy(s_au8LastLine, &u8szCurrentLine[1 + 3], TTX_LINE_MEM_SIZE);
        }

        if ((30 == u8PacketNo) && (0 == u8Magazine))                /*store 8/30 packet*/
        {
            if (HI_SUCCESS != TTX_Data_M30In(u8szCurrentLine))
            {
                HI_ERR_TTX("TTX_Data_M30In failure!!\n");
            }

            u32ExtractPoint += TTX_EN300706_PACKET_SIZE;
            continue;
        }

        if (31 == u8PacketNo)
        {
            u32ExtractPoint += TTX_EN300706_PACKET_SIZE;
            continue;
        }

        if (0 == u8PacketNo)
        {
            if (s_u8PreviousIndex < 9)
            {
                TTX_Recv_ProcPreviousPage(pstThisElem, &g_astpage[s_u8PreviousIndex]);
            }

            if (((s_u8aInvHamming8_4Tab[pData[u32ExtractPoint + TTX_MAG_TRANS_MODE_POS - 1]]) & 0x01)) /*judge transmit mod is  parallel or serial*/
            {
                g_s8serial = 1;
            }
            else
            {
                g_s8serial = 0;
            }
        }

        if (g_s8serial == 1)
        {
            s_u8PreviousIndex = TTX_DEFAULT;

            if (HI_SUCCESS
                != TTX_Recv_OrganisePage(pstThisElem, TTX_DEFAULT, &g_astpage[TTX_DEFAULT], u8szCurrentLine))
            {
                HI_ERR_TTX("This packet is not valid!\n");
            }

            u32ExtractPoint += TTX_EN300706_PACKET_SIZE;
            continue;
        }
        else if (g_s8serial == 0)
        {
            s_u8PreviousIndex = u8Magazine;

            if (HI_SUCCESS
                != TTX_Recv_OrganisePage(pstThisElem, u8Magazine, &g_astpage[u8Magazine], u8szCurrentLine))
            {
                HI_ERR_TTX("This packet is not valid!\n");
            }

            u32ExtractPoint += TTX_EN300706_PACKET_SIZE;
            continue;
        }
        else
        {
            s_u8PreviousIndex = TTX_DEFAULT;

            if (HI_SUCCESS
                != TTX_Recv_OrganisePage(pstThisElem, TTX_DEFAULT, &g_astpage[TTX_DEFAULT], u8szCurrentLine))
            {
                HI_ERR_TTX("This packet is not valid!\n");
            }

            u32ExtractPoint += TTX_EN300706_PACKET_SIZE;
            continue;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 TTX_Recv_ParsePESPacket(HI_VOID* hData, HI_U8* pu8DataSrc, HI_U32 u32PayloadLen)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32PacketLength = 0;
    HI_U8 u8chPtsExistFlag = 0;
    HI_U8* pData = HI_NULL;
    HI_U8 u8PcktAddr1 = 0;
    HI_U8 u8PcktAddr2 = 0;
    HI_U8 u8PacketNo = 0;
    HI_U32 u32ExtractPoint = 0;
    HI_U8 u8Magazine = TTX_BAD_INV_MAGZINE;
    HI_U8 u8szCurrentLine[TTX_LINE_SIZE] = {0};
    TTX_CONTEXT_S_PTR pstThisElem = HI_NULL;

    TTX_RECV_CHECK(pu8DataSrc);
    TTX_RECV_CHECK(u32PayloadLen);
    TTX_RECV_CHECK(hData);

    pData = pu8DataSrc;
    pstThisElem = (TTX_CONTEXT_S_PTR)hData;

    if (pData[TTX_STREAM_ID_POS] != TTX_PRIVATE_STREAM_1)
    {
        s32Ret = HI_FAILURE;
        return s32Ret;
    }

    u32PacketLength  = (pData[TTX_PCKT_LEN_MSB_POS] << 8) + pData[TTX_PCKT_LEN_LSB_POS] + TTX_PCKT_HDR_LENGTH;
    u8chPtsExistFlag = pData[7] & 0x80;

    if (u8chPtsExistFlag)
    {
        HI_U32 nPTSVal = ((pData[9] & 0x0e) << 29) | ((pData[10] & 0xff) << 22)
                         | ((pData[11] & 0xfe) << 14) | ((pData[12] & 0xff) << 7)
                         | ((pData[13] & 0xfe) >> 1);

        g_u32PTS = nPTSVal / 90;

        if (pData[9] & 0x08)
        {
            g_u32PTS += 0x2D82D82;
        }
    }

    u32ExtractPoint = TTX_PES_DATA_START_POS;       /* Teletext data start */

    if ((pData[u32ExtractPoint - 1] < 0x10) || (pData[u32ExtractPoint - 1] > 0x1F))
    {
        s32Ret = HI_FAILURE;
        return s32Ret;
    }

    while (u32ExtractPoint < u32PacketLength)    /* loop on line data */
    {
        memset(u8szCurrentLine, 0, TTX_LINE_SIZE);

        if ((pData[u32ExtractPoint] != 0x02)
            && (pData[u32ExtractPoint] != 0x03))
        {
            u32ExtractPoint += TTX_LINE_SIZE;     /* step to next line */
            continue;
        }

        u8PcktAddr1 = s_u8aInvHamming8_4Tab[(pData[(u32ExtractPoint + TTX_PACKET_ADDR_POS1)])];
        u8PcktAddr2 = s_u8aInvHamming8_4Tab[(pData[(u32ExtractPoint + TTX_PACKET_ADDR_POS2)])];

        if ((u8PcktAddr1 | u8PcktAddr2) == TTX_BAD_INV_HAMMING)     /*packet numbet error*/
        {
            u32ExtractPoint += TTX_LINE_SIZE;
            continue;
        }

        u8PacketNo = (HI_U8)((u8PcktAddr1 >> 3) | (u8PcktAddr2 << 1));     /*Packetnum*/
        u8Magazine = u8PcktAddr1 & TTX_MAGAZINE_MASK;              /*Magzinenum*/

        memcpy(u8szCurrentLine, &pData[u32ExtractPoint], TTX_LINE_SIZE);

        if ((30 == u8PacketNo) && (0 == u8Magazine))                /*store 8/30 packet*/
        {
            if (HI_SUCCESS != TTX_Data_M30In(u8szCurrentLine))
            {
                HI_WARN_TTX("TTX_Data_M30In failure!!\n");
            }

            u32ExtractPoint += TTX_LINE_SIZE;
            continue;
        }

        if (31 == u8PacketNo)
        {
            u32ExtractPoint += TTX_LINE_SIZE;
            continue;
        }

        if (0 == u8PacketNo)
        {
            if (s_u8PreviousIndex < 9)
            {
                TTX_Recv_ProcPreviousPage(pstThisElem, &g_astpage[s_u8PreviousIndex]);
            }

            if (((s_u8aInvHamming8_4Tab[pData[u32ExtractPoint + TTX_MAG_TRANS_MODE_POS]]) & 0x01)) /*judge transmit mod is  parallel or serial*/
            {
                g_s8serial = 1;
            }
            else
            {
                g_s8serial = 0;
            }
        }

        if (g_s8serial == 1)
        {
            s_u8PreviousIndex = TTX_DEFAULT;

            if (HI_SUCCESS
                != TTX_Recv_OrganisePage(pstThisElem, TTX_DEFAULT, &g_astpage[TTX_DEFAULT], u8szCurrentLine))
            {
                HI_INFO_TTX("This packet is not valid!\n");
            }

            u32ExtractPoint += TTX_LINE_SIZE;
            continue;
        }
        else if (g_s8serial == 0)
        {
            s_u8PreviousIndex = u8Magazine;

            if (HI_SUCCESS
                != TTX_Recv_OrganisePage(pstThisElem, u8Magazine, &g_astpage[u8Magazine], u8szCurrentLine))
            {
                HI_INFO_TTX("This packet is not valid!\n");
            }

            u32ExtractPoint += TTX_LINE_SIZE;
            continue;
        }
        else
        {
            s_u8PreviousIndex = TTX_DEFAULT;

            if (HI_SUCCESS
                != TTX_Recv_OrganisePage(pstThisElem, TTX_DEFAULT, &g_astpage[TTX_DEFAULT], u8szCurrentLine))
            {
                HI_INFO_TTX("This packet is not valid!\n");
            }

            u32ExtractPoint += TTX_LINE_SIZE;
            continue;
        }
    }

    return HI_SUCCESS;
}


static HI_VOID TTX_CorrectInvalidChar(HI_U8* pu8Packet, HI_U8 u8PacketNo, TTX_PAGE_S stPrePage)
{
    int tj = 4;
    HI_U8 u8Check = 0;


    for ( ; tj < TTX_LINE_SIZE; tj++)
    {
        HI_U8 u8CurChar = pu8Packet[tj];

        if (0x00 != u8CurChar )
        {
            HI_U8 d0 = u8CurChar & 0x01;
            HI_U8 d1 = (u8CurChar & 0x02) >> 1;
            HI_U8 d2 = (u8CurChar & 0x04) >> 2;
            HI_U8 d3 = (u8CurChar & 0x08) >> 3;
            HI_U8 d4 = (u8CurChar & 0x10) >> 4;
            HI_U8 d5 = (u8CurChar & 0x20) >> 5;
            HI_U8 d6 = (u8CurChar & 0x40) >> 6;
            HI_U8 d7 = (u8CurChar & 0x80) >> 7;

            u8Check = d0 + d1 + d2 + d3 + d4 + d5 + d6 + d7;

            u8Check = u8Check & 0x01;

            //invalid character, use old data
            if (u8Check != 1)
            {
                pu8Packet[tj] = stPrePage.u8szLines[u8PacketNo][tj - 4];
            }
        }
    }
}
static HI_BOOL TTX_Recv_CheckPacketValid(HI_U8* pu8Packet)
{
    HI_U32 i = 0;

    if (HI_NULL == pu8Packet)
    {
        return HI_FALSE;
    }

    pu8Packet += 4;

    for (i = 0; i < TTX_LINE_MEM_SIZE; i++)
    {
        if (0 ==  pu8Packet[i])
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;

}


static HI_VOID TTX_Recv_OrganiseLine(HI_U8 u8Magazine, HI_U8 u8PacketNo, HI_U8 u8TmpPageNo, HI_U8* pu8Packet)
{
    HI_BOOL bFindPage = HI_FALSE;
    TTX_PAGE_S stPrePage;

    memset(&stPrePage, 0x0, sizeof(TTX_PAGE_S));

    if (HI_SUCCESS != TTX_Data_FindPage(u8Magazine, u8TmpPageNo, 0, &stPrePage))
    {
        return;
    }

    bFindPage = HI_TRUE;


    if (bFindPage)
    {
        if (0 != memcmp(&pu8Packet[4], &(stPrePage.u8szLines[u8PacketNo]), TTX_LINE_MEM_SIZE))
        {
            //HI_ERR_TTX("TTX_CorrectInvalidChar, u8PacketNo = 0x%x\n", u8PacketNo);

            TTX_CorrectInvalidChar(pu8Packet, u8PacketNo, stPrePage);
        }
    }

}

static HI_S32 TTX_Recv_OrganisePage(TTX_CONTEXT_S_PTR pstThisElem, HI_U8 u8MagazineFlag,
                                    TTX_PAGE_S_PTR pstPage,
                                    HI_U8* pu8Packet)
{
    HI_U8 u8PcktAddr1 = 0;
    HI_U8 u8PcktAddr2 = 0;
    HI_U8 u8PacketNo = 0;
    HI_U8 u8DesignationCode = 0;
    HI_U8 u8Magazine = TTX_BAD_INV_MAGZINE;
    static HI_U8 u8tmpmagzine;

    HI_U8 u8PageNumb1 = 0;
    HI_U8 u8PageNumb2 = 0;
    HI_U8 u8TTPageNo = TTX_BAD_INV_PAGE;

    /* inverse hamming 8/4 */
    HI_U8 u8SubPCode1 = 0;
    HI_U8 u8SubPCode2 = 0;
    HI_U8 u8SubPCode3 = 0;
    HI_U8 u8SubPCode4 = 0;

    HI_U8 u8TmpPageNo = TTX_BAD_INV_PAGE;

    u8PcktAddr1 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_PACKET_ADDR_POS1]];
    u8PcktAddr2 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_PACKET_ADDR_POS2]];

    if ((u8PcktAddr1 | u8PcktAddr2) == TTX_BAD_INV_HAMMING)     //packet numbet error
    {
        return HI_FAILURE;
    }

    u8PacketNo = (HI_U8)((u8PcktAddr1 >> 3) | (u8PcktAddr2 << 1));
    u8Magazine = u8PcktAddr1 & TTX_MAGAZINE_MASK;


    if (u8PacketNo == 0)
    {
        memset(pstPage, 0, sizeof(TTX_PAGE_S));

        u8PageNumb1 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_PAGE_NO_UNIT_POS]];
        u8PageNumb2 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_PAGE_NO_TENS_POS]];

        if ((u8PageNumb1 | u8PageNumb2) == TTX_BAD_INV_HAMMING)
        {
            return HI_FAILURE;
        }

        u8TTPageNo = (HI_U8)(u8PageNumb1 | (u8PageNumb2 << TTX_NYBBLE_SHIFT_CNT));

        if (u8TTPageNo == 0xff)                    /*one page end*/
        {
            return HI_SUCCESS;
        }

        TTX_Recv_OrganiseLine(u8Magazine, u8PacketNo, u8TTPageNo, pu8Packet);

        /*judge subpage data */
        u8SubPCode4 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_SUBPAGE_THOU_POS]];
        u8SubPCode3 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_SUBPAGE_HUND_POS]];
        u8SubPCode2 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_SUBPAGE_TENS_POS]];
        u8SubPCode1 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_SUBPAGE_UNIT_POS]];

        if ((((u8SubPCode1 | u8SubPCode2) | u8SubPCode3) | u8SubPCode4) == TTX_BAD_INV_HAMMING)
        {
            return HI_FAILURE;
        }

        if (HI_SUCCESS != TTX_Data_Set_TimeCode(&pu8Packet[TTX_LINE_SIZE - 8]))
        {
            HI_WARN_TTX("TTX_Data_Set_TimeCode failure!!\n");
        }

        pstPage->u32PTS = g_u32PTS;
        pstPage->u32ValidLines |= (1 << u8PacketNo);
        memcpy(pstPage->u8szLines[u8PacketNo], &pu8Packet[4], TTX_LINE_MEM_SIZE);

        if (u8MagazineFlag == TTX_DEFAULT)
        {
            u8tmpmagzine = u8Magazine;
        }

        (HI_VOID)TTX_Recv_OutputRawData(pstThisElem, u8MagazineFlag, pu8Packet);

        return HI_SUCCESS;
    }
    else
    {
        if (pstPage->u32ValidLines & 0x01)/*has received packet 0*/
        {
            if ((u8MagazineFlag == TTX_DEFAULT) && (u8tmpmagzine != u8Magazine))
            {
                return HI_FAILURE;
            }

            (HI_VOID)TTX_Recv_OutputRawData(pstThisElem, u8MagazineFlag, pu8Packet);

            u8PageNumb1 = s_u8aInvHamming8_4Tab[pstPage->u8szLines[0][TTX_PAGE_NO_UNIT_POS - 4]];
            u8PageNumb2 = s_u8aInvHamming8_4Tab[pstPage->u8szLines[0][TTX_PAGE_NO_TENS_POS - 4]];
            u8TmpPageNo = (HI_U8)(u8PageNumb1 | (u8PageNumb2 << TTX_NYBBLE_SHIFT_CNT));

            TTX_Recv_OrganiseLine(u8Magazine, u8PacketNo, u8TmpPageNo, pu8Packet);


            if ((u8PacketNo >= 1) && (u8PacketNo <= 25))
            {
                if ((pstPage->u32ValidLines >> u8PacketNo) & 0x01)        /*judge have the same packet*/
                {
                    if (HI_TRUE == TTX_Recv_CheckPacketValid(pu8Packet))
                    {
                        memset(pstPage->u8szLines[u8PacketNo], 0, TTX_LINE_MEM_SIZE);
                        memcpy(pstPage->u8szLines[u8PacketNo], &pu8Packet[4], TTX_LINE_MEM_SIZE);
                    }
                }
                else
                {
                    pstPage->u32ValidLines |= (1 << u8PacketNo);
                    memcpy(pstPage->u8szLines[u8PacketNo], &pu8Packet[4], TTX_LINE_MEM_SIZE);
                }

                return HI_SUCCESS;
            }
            else if ((u8PacketNo >= 26) && (u8PacketNo <= 29))                  /*store packet 26-29,packet 26,27,28,29 not only one packet */
            {
                HI_U8 u8Offset = 0;

                if ((u8MagazineFlag == TTX_DEFAULT) && (u8tmpmagzine != u8Magazine))
                {
                    return HI_FAILURE;
                }

                u8DesignationCode = s_u8aInvHamming8_4Tab[pu8Packet[TTX_DC_POS]];

                if (0xf < u8DesignationCode) /*max value of DesignationCode*/
                {
                    return HI_FAILURE;
                }

                if (26 == u8PacketNo)
                {
                    u8Offset = u8DesignationCode;
                }
                else if (27 == u8PacketNo)
                {
                    u8Offset = TTX_PACKET26_NUM + u8DesignationCode;
                }
                else if (28 == u8PacketNo)
                {
                    u8Offset = TTX_PACKET26_NUM + TTX_PACKET27_NUM + u8DesignationCode;
                }
                else if (29 == u8PacketNo)
                {
                    u8Offset = TTX_PACKET26_NUM + TTX_PACKET27_NUM + TTX_PACKET28_NUM + u8DesignationCode;
                }

                if (u8Offset >= TTX_PACKET26_NUM + TTX_PACKET27_NUM + TTX_PACKET28_NUM + TTX_PACKET29_NUM)
                {
                    return HI_FAILURE;
                }

                if (32 > u8Offset)
                {
                    pstPage->u32ExtenValidLineL |= (1 << u8Offset);
                }
                else
                {
                    pstPage->u32ExtenValidLineH |= (1 << (u8Offset - 32));
                }

                memcpy(pstPage->u8szExtenLines[u8Offset], &pu8Packet[4], TTX_LINE_MEM_SIZE);
                return HI_SUCCESS;
            }

        }/*end if ( u8PacketNo >=25 && u8PacketNo <=29)  */
    }/*end if (pstPage->u32ValidLines & 0x01)*/

    return HI_FAILURE;
}

HI_S32 TTX_Recv_Create(HI_VOID** pphDataRecv)
{
    TTX_CONTEXT_S_PTR pstThisElem = HI_NULL;
    TTX_HANDLE_FILTER_S_PTR pstQueueReq = HI_NULL;
    HI_U32 u32StructSize = 0;
    //HI_UNF_DISP_VBI_CFG_S stVBI_CFG;

    TTX_RECV_CHECK(pphDataRecv);

    u32StructSize = sizeof(TTX_CONTEXT_S);

    pstThisElem = (TTX_CONTEXT_S*)TTX_Mem_Malloc(u32StructSize);

    if (HI_NULL == pstThisElem)
    {
        HI_ERR_TTX("malloc TTX_CONTENT_S error\n");
        return HI_FAILURE;
    }

    TTX_Mem_Memset(pstThisElem, 0, u32StructSize);

    pstQueueReq = (TTX_HANDLE_FILTER_S_PTR)TTX_Mem_Malloc(sizeof(TTX_HANDLE_FILTER_S));

    if (HI_NULL == pstQueueReq)
    {
        HI_ERR_TTX("malloc TTX_HANDLE_FILTER_S error\n");
        TTX_Mem_Free(pstThisElem);

        return HI_FAILURE;
    }
    else
    {
        pstThisElem->pstFiltersPtr = pstQueueReq;
        pstThisElem->pstFiltersPtr->u32OpenOff   = TTX_REQ_UNUSED;
        pstThisElem->pstFiltersPtr->bHeaderFound = HI_FALSE;
    }

    if (0 != pthread_mutex_init(&pstThisElem->mutex_recv_lock, HI_NULL))
    {
        HI_ERR_TTX("init mutex fail\n");
        TTX_Mem_Free(pstQueueReq);
        TTX_Mem_Free(pstThisElem);
        return HI_FAILURE;
    }

    pstThisElem->stRequest.u32OpenOff   = TTX_REQ_UNUSED;
    pstThisElem->stRequest.bHeaderFound = HI_FALSE;

    pstThisElem->bVBIUsed = HI_FALSE;
    TTX_RECV_LOCK();
    pstThisElem->u32NumActiveRequests = 0;
    TTX_RECV_UNLOCK();
    pstThisElem->bOpen = HI_FALSE;
    pstThisElem->bIsSubData = HI_FALSE;
    pstThisElem->enPageFilterStatus = TELETEXT_PAGE_PES_INIT;
    pstThisElem->bAutoPlay = HI_TRUE;
    pstThisElem->bHold = HI_FALSE;
    pstThisElem->u8DataIdentifier = TTX_DEFAULT_IDENTIFIER;
    pstThisElem->pu8WriteDataAddr = pstThisElem->u8szPesBuffer;
    pstThisElem->u32PESWritenLen = 0;
    pstThisElem->enPacketType = HI_UNF_TTX_PACKET_TYPE_PES;
    g_u32PTS = 0;


    memset(g_astpage, 0x0, sizeof(g_astpage));
    memset(g_astRawData, 0x0, sizeof(g_astRawData));
    

    /* create VBI channel */
    pstThisElem->hVBIHandle = HI_INVALID_HANDLE;

#if defined VER_V1R2 || defined VER_V1R3
    HI_UNF_DISP_VBI_CFG_S stVBI_CFG;
    stVBI_CFG.enType =  HI_UNF_DISP_VBI_TYPE_TTX;
    (HI_VOID)HI_UNF_DISP_CreateVBI(HI_UNF_DISPLAY0, &stVBI_CFG, &pstThisElem->hVBIHandle);
#endif

#ifdef VER_V1R1
    HI_UNF_DISP_VBI_CFG_S stVBI_CFG;
    stVBI_CFG.eType =  HI_UNF_DISP_VBI_TYPE_TTX;
    (HI_VOID)HI_UNF_DISP_CreateVBI(HI_UNF_DISPLAY0, &stVBI_CFG, &pstThisElem->hVBIHandle);
#endif

    *pphDataRecv = pstThisElem;
    return HI_SUCCESS;
}

HI_S32 TTX_Recv_Destroy(HI_VOID* hDataRecv)
{
    TTX_CONTEXT_S_PTR pstThisElem = NULL;
    TTX_HANDLE_FILTER_S_PTR pstQueueReq = NULL;

    TTX_RECV_CHECK(hDataRecv);

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;
    pstQueueReq = pstThisElem->pstFiltersPtr;
    pstQueueReq->u32OpenOff   = TTX_REQ_UNUSED;
    pstQueueReq->bHeaderFound = HI_FALSE;
    pstThisElem->bVBIUsed = HI_FALSE;
    pstThisElem->bOpen = HI_FALSE;

    TTX_Mem_Free(pstThisElem->pstFiltersPtr);
    pstThisElem->pstFiltersPtr = HI_NULL;

#if defined VER_V1R1 || defined VER_V1R2 || defined VER_V1R3

    if (HI_INVALID_HANDLE != pstThisElem->hVBIHandle)
    {
        (HI_VOID)HI_UNF_DISP_DestroyVBI(pstThisElem->hVBIHandle);
        pstThisElem->hVBIHandle = HI_INVALID_HANDLE;
    }

#endif

    if (0 != pthread_mutex_destroy(&(pstThisElem->mutex_recv_lock)))
    {}

    TTX_Mem_Free(pstThisElem);
    pstThisElem = HI_NULL;

    memset(g_astpage, 0x0, sizeof(g_astpage));
    memset(g_astRawData, 0x0, sizeof(g_astRawData));

    return HI_SUCCESS;
}

HI_S32  TTX_Recv_SetFilter(HI_VOID* hDataRecv, HI_VOID* hDataParse, TTX_Filter_S_PTR pstFilter )
{
    TTX_CONTEXT_S_PTR pstThisElem = NULL;                 /* element for this Handle */
    TTX_HANDLE_FILTER_S_PTR pstQueueReq = NULL;

    TTX_RECV_CHECK(pstFilter);
    TTX_RECV_CHECK(hDataRecv);
    TTX_RECV_CHECK(hDataParse);

    if (HI_SUCCESS != TTX_Data_SetFilter(pstFilter->stReqPgAddr))
    {
        return HI_FAILURE;
    }

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;
    s_hGlobalHandle = hDataParse;
    pstQueueReq = pstThisElem->pstFiltersPtr;               /* point to Filter queue */
    pstQueueReq->stFilter   = *pstFilter;               /* structure copy data */
    pstQueueReq->u32OpenOff = 1;
    TTX_RECV_LOCK();
    pstThisElem->u32NumActiveRequests = 1;
    TTX_RECV_UNLOCK();

    if (HI_SUCCESS == TTX_Recv_HandleFilter(hDataRecv, hDataParse, pstFilter))
    {
        TTX_RECV_LOCK();
        pstThisElem->u32NumActiveRequests = 0;
        TTX_RECV_UNLOCK();
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 TTX_Recv_ClearFilter(HI_VOID* hDataRecv)
{
    TTX_CONTEXT_S_PTR pstThisElem = HI_NULL;
    TTX_HANDLE_FILTER_S_PTR pstQueueReq = NULL;

    TTX_RECV_CHECK(hDataRecv);

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;
    pstQueueReq = pstThisElem->pstFiltersPtr;
    pstQueueReq->u32OpenOff = TTX_REQ_UNUSED;

    TTX_RECV_LOCK();
    pstThisElem->u32NumActiveRequests = 0;
    TTX_RECV_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 TTX_Recv_Hold(HI_VOID* hDataRecv, HI_BOOL bHold)
{
    TTX_CONTEXT_S_PTR pstThisElem = NULL;

    TTX_RECV_CHECK(hDataRecv);

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;

    TTX_RECV_LOCK();

    pstThisElem->bHold = bHold;

    TTX_RECV_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 TTX_Recv_AutoPlay(HI_VOID* hDataRecv, HI_U8 u8Enable)
{
    TTX_CONTEXT_S_PTR pstThisElem = NULL;

    TTX_RECV_CHECK(hDataRecv);

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;

    TTX_RECV_LOCK();

    if (u8Enable == 0)
    {
        pstThisElem->bAutoPlay = HI_FALSE;
    }
    else if (u8Enable == 1)
    {
        pstThisElem->bAutoPlay = HI_TRUE;
    }
    else
    {
        pstThisElem->bAutoPlay = pstThisElem->bAutoPlay ? HI_FALSE : HI_TRUE;
    }

    TTX_RECV_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 TTX_Recv_SetInitpage(HI_VOID* hDataRecv, HI_U32 u32MagazineNum, HI_U32 u32PageNum, HI_BOOL bSubtitle)
{
    TTX_CONTEXT_S_PTR pstThisElem = NULL;

    TTX_RECV_CHECK(hDataRecv);

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;

    TTX_RECV_LOCK();

    if (bSubtitle)
    {
        pstThisElem->bIsSubData = HI_TRUE;
        pstThisElem->stInitFilter.u8MagNum  = (HI_U8)u32MagazineNum;
        pstThisElem->stInitFilter.u8PageNum = (HI_U8)u32PageNum;
    }
    else
    {
        pstThisElem->bIsSubData = HI_FALSE;

        if ((u32MagazineNum == 0xffff) && (u32PageNum == 0xffff))
        {
            pstThisElem->stInitFilter.u8MagNum  = 1;
            pstThisElem->stInitFilter.u8PageNum = 0;
        }
        else
        {
            pstThisElem->stInitFilter.u8MagNum  = (HI_U8)u32MagazineNum;
            pstThisElem->stInitFilter.u8PageNum = (HI_U8)u32PageNum;
        }
    }

    TTX_RECV_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 TTX_Recv_SetOutputType(HI_VOID* hDataRecv, TTX_OBJECTTYPE_E enType, HI_BOOL bEnable)
{
    TTX_CONTEXT_S_PTR pstThisElem = NULL;

    TTX_RECV_CHECK(hDataRecv);

    if ((enType != TTX_VBI) && (enType != TTX_OSD) && (enType != TTX_OSD_VBI))
    {
        return HI_FAILURE;
    }

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;

    if ((enType == TTX_VBI) || (enType == TTX_OSD_VBI))
    {
        if (bEnable)
        {
            pstThisElem->bVBIUsed = HI_TRUE;
        }
        else
        {
            pstThisElem->bVBIUsed = HI_FALSE;
        }
    }

    if ((enType == TTX_OSD) || (enType == TTX_OSD_VBI))
    {
        if (bEnable)
        {
            pstThisElem->bOpen = HI_TRUE;
        }
        else
        {
            pstThisElem->bOpen = HI_FALSE;
        }
    }

    pstThisElem->enMode = enType;

    return HI_SUCCESS;
}

HI_S32  TTX_Recv_SetRequest(HI_VOID* hDataRecv, TTX_Filter_S_PTR pstFilter)
{
    TTX_CONTEXT_S_PTR pstThisElem = NULL;      /* element for this Handle */

    TTX_RECV_CHECK(pstFilter);
    TTX_RECV_CHECK(hDataRecv);

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;
    pstThisElem->stRequest.stFilter   = *pstFilter;
    pstThisElem->stRequest.u32OpenOff = 1;

    return HI_SUCCESS;
}

HI_S32 TTX_Recv_ClearRequest(HI_VOID* hDataRecv)
{
    TTX_CONTEXT_S_PTR pstThisElem = HI_NULL;

    TTX_RECV_CHECK(hDataRecv);

    pstThisElem = (TTX_CONTEXT_S_PTR)hDataRecv;
    pstThisElem->stRequest.u32OpenOff = TTX_REQ_UNUSED;

    return HI_SUCCESS;
}

static HI_S32 TTX_Recv_OutputRawData(TTX_CONTEXT_S_PTR pstThisElem, HI_U8 u8MagazineFlag, HI_U8* pu8Packet)
{
    HI_U8 u8PcktAddr1 = 0;
    HI_U8 u8PcktAddr2 = 0;
    HI_U8 u8PacketNo = 0;
    HI_UNF_TTX_RAWDATA_S* pstRawData = &g_astRawData[u8MagazineFlag];

    if (TTX_REQ_UNUSED == pstThisElem->stRequest.u32OpenOff)
    {
        return HI_FAILURE;
    }

    u8PcktAddr1 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_PACKET_ADDR_POS1]];
    u8PcktAddr2 = s_u8aInvHamming8_4Tab[pu8Packet[TTX_PACKET_ADDR_POS2]];

    u8PacketNo = (HI_U8)((u8PcktAddr1 >> 3) | (u8PcktAddr2 << 1));

    if (u8PacketNo == 0)
    {
        if (pstRawData->u32ValidLines & 0x01)
        {
            TTX_SEGMENT_S stSegment;

            if (pstThisElem->stRequest.stFilter.NotifyFunction)
            {
                stSegment.pu8SegmentData = (HI_U8*)pstRawData;
                stSegment.u16DataLength = sizeof(HI_UNF_TTX_RAWDATA_S);
                (HI_VOID)(pstThisElem->stRequest.stFilter.NotifyFunction)((HI_VOID*)pstThisElem->stRequest.stFilter.pvUserData, &stSegment);
            }
        }

        memset(pstRawData, 0, sizeof(HI_UNF_TTX_RAWDATA_S));
        pstRawData->u32ValidLines |= (1 << u8PacketNo);
        memcpy(pstRawData->au8Lines[u8PacketNo], &pu8Packet[0], TTX_LINE_SIZE);

        return HI_SUCCESS;
    }
    else
    {
        if (pstRawData->u32ValidLines & 0x01)
        {
            if ((pstRawData->u32ValidLines >> u8PacketNo) & 0x01)        /*judge have the same packet*/
            {
                memset(pstRawData->au8Lines[u8PacketNo], 0, TTX_LINE_SIZE);
            }

            pstRawData->u32ValidLines |= (1 << u8PacketNo);
            memcpy(pstRawData->au8Lines[u8PacketNo], &pu8Packet[0], TTX_LINE_SIZE);
            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  /* __cplusplus */
