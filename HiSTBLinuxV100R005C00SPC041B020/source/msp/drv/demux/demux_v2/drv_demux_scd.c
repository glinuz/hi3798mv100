/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pvr_scd.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/16
  Description   : process of scd read from DEMUX module
  History       :
  1.Date        : 2010/06/17
    Author      : j40671
    Modification: Created file

******************************************************************************/

#include <linux/kernel.h>
#include <linux/string.h>

#include "hi_type.h"

#include "demux_debug.h"
#include "drv_demux_scd.h"
#include "hi_drv_mem.h"
#include "hi_drv_mmz.h"
#include "hal_demux.h"

#define PVR_INDEX_INVALID_PTSMS             ((HI_U32)(-1))

/* pvr index's SCD descriptor                                               */
/* format between firmware index and demux scd */
typedef struct
{
    HI_U8  u8IndexType;                           /* type of index(pts,sc,pause,ts) */
    HI_U8  u8StartCode;                           /* type of start code */
    HI_U16 u16OffsetInTs;                         /* start code offset in a TS package */

    HI_U32 u32OffsetInDavBuf;                     /* start code offset in DAV buffer */

    HI_U8  au8ByteAfterSC[8];                      /* 8Byte next to SC */


    HI_U64 u64TSCnt;                              /* count of TS package */

    HI_U32 u32PtsMs;
    HI_U16 u16OverFlow;
    HI_U16 u16Reserv;
}PVR_INDEX_SCD_S;



static HI_VOID PVR_SCDDmxIdxToPvrIdx(const DMX_IDX_DATA_S *pDmxIndexData,
                PVR_INDEX_SCD_S *pPvrIndexData)
{
    PVR_INDEX_SCD_S indexData;

    indexData.u16OverFlow = !((pDmxIndexData->u32Chn_Ovflag_IdxType_Flags >> 28) & 0x1);
    if (indexData.u16OverFlow)
    {
        HI_INFO_DEMUX("indexData.u16OverFlow == 1\n");
    }

    indexData.au8ByteAfterSC[0]
        = (pDmxIndexData->u32ScType_Byte12AfterSc_OffsetInTs >> 16) & 0xffU;
    indexData.au8ByteAfterSC[1]
        = (pDmxIndexData->u32ScType_Byte12AfterSc_OffsetInTs >> 8) & 0xffU;
    indexData.au8ByteAfterSC[2]
        = (pDmxIndexData->u32TsCntHi8_Byte345AfterSc >> 16) & 0xffU;
    indexData.au8ByteAfterSC[3]
        = (pDmxIndexData->u32TsCntHi8_Byte345AfterSc >> 8) & 0xffU;
    indexData.au8ByteAfterSC[4]
        = (pDmxIndexData->u32TsCntHi8_Byte345AfterSc) & 0xffU;
    indexData.au8ByteAfterSC[5]
        = (pDmxIndexData->u32ScCode_Byte678AfterSc >> 16) & 0xffU;
    indexData.au8ByteAfterSC[6]
        = (pDmxIndexData->u32ScCode_Byte678AfterSc >> 8) & 0xffU;
    indexData.au8ByteAfterSC[7]
        = (pDmxIndexData->u32ScCode_Byte678AfterSc) & 0xffU;

    indexData.u16OffsetInTs = pDmxIndexData->u32ScType_Byte12AfterSc_OffsetInTs & 0x00ff;
    indexData.u64TSCnt = pDmxIndexData->u32TsCntHi8_Byte345AfterSc & 0xff000000;
    indexData.u64TSCnt = indexData.u64TSCnt << 8;
    indexData.u64TSCnt |= pDmxIndexData->u32TsCntLo32;
    indexData.u32OffsetInDavBuf = pDmxIndexData->u32BackPacetNum & 0x0001fffff;

    indexData.u8IndexType = (pDmxIndexData->u32Chn_Ovflag_IdxType_Flags >> 24) & 0xf;
    indexData.u8StartCode = (pDmxIndexData->u32ScType_Byte12AfterSc_OffsetInTs >> 24) & 0xff;
    //indexData.u32OffsetInDavBuf += indexData.u16OffsetInTs;
    indexData.u32PtsMs = PVR_INDEX_INVALID_PTSMS;
    indexData.u16Reserv = 0;

    if (DMX_INDEX_SC_TYPE_PTS == indexData.u8IndexType)
    {
        if (0 == (pDmxIndexData->u32TsCntHi8_Byte345AfterSc & 0x2))
        {
            indexData.u32PtsMs = PVR_INDEX_INVALID_PTSMS;
        }
        else
        {
            HI_U64 Pts      = 0;
            HI_U32 Pts33    = pDmxIndexData->u32TsCntHi8_Byte345AfterSc & 0x1;
            HI_U32 Pts32    = pDmxIndexData->u32ScCode_Byte678AfterSc;

            if (Pts33)
            {
                Pts = 0x100000000ULL;
            }

            Pts += (HI_U64)Pts32;

            Pts = Pts >> 1;

            indexData.u32PtsMs = (HI_U32)Pts;

            indexData.u32PtsMs /= 45;
        }

        //indexData.u8IndexType = DMX_INDEX_SC_TYPE_PIC;
        //indexData.u8StartCode = PVR_INDEX_SC_PIC;

        //indexData.au8ByteAfterSC[1] = s_refCounter++;
        //indexData.au8ByteAfterSC[1] = indexData.au8ByteAfterSC[1] << 6;
    }

    memcpy(pPvrIndexData, &indexData, sizeof(PVR_INDEX_SCD_S));
    return;
}


static inline HI_U64 PVR_SCDIndexCalcGlobalOffset(HI_BOOL bUseTimeStamp,const PVR_INDEX_SCD_S *pScData)
{
    HI_U64 offset; /* frame header offset (tota value) */

    if ( bUseTimeStamp)
    {
        offset = (pScData->u64TSCnt - pScData->u32OffsetInDavBuf - 1) * 192ULL + pScData->u16OffsetInTs;
    }
    else
    {
        offset = (pScData->u64TSCnt - pScData->u32OffsetInDavBuf - 1) * 188ULL + pScData->u16OffsetInTs;
    }
    
    return offset;
}


/*****************************************************************************
 Prototype       : DmxScdToVideoIndex
 Description     : transform SCD data to INDEX structure
 Input           : handle   **
                   pScData  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2010/06/17
    Author       : j40671
    Modification : Created function

*****************************************************************************/
HI_S32 DmxScdToVideoIndex(HI_BOOL bUseTimeStamp,const DMX_IDX_DATA_S *ScData, FINDEX_SCD_S *pstFidx)
{
    PVR_INDEX_SCD_S IndexData;
    HI_U64          CurrGlobalOffset = 0;

    PVR_SCDDmxIdxToPvrIdx(ScData, &IndexData);

    /* just only deal with SC of frame and pts */
    if (   (DMX_INDEX_SC_TYPE_PTS != IndexData.u8IndexType)
        && (DMX_INDEX_SC_TYPE_PIC != IndexData.u8IndexType)
        && (DMX_INDEX_SC_TYPE_PIC_SHORT != IndexData.u8IndexType) )
    {
        return HI_FAILURE;
    }

    if (unlikely(DMX_INDEX_SC_TYPE_PTS == IndexData.u8IndexType && PVR_INDEX_INVALID_PTSMS == IndexData.u32PtsMs))
    {
        return HI_FAILURE;
    }

    CurrGlobalOffset = PVR_SCDIndexCalcGlobalOffset(bUseTimeStamp, &IndexData);

    pstFidx->s64GlobalOffset    = (HI_S64)CurrGlobalOffset;
    pstFidx->u32PtsMs           = IndexData.u32PtsMs;
    pstFidx->u8IndexType        = IndexData.u8IndexType;
    pstFidx->u8StartCode        = IndexData.u8StartCode;

    memcpy(pstFidx->au8DataAfterSC, IndexData.au8ByteAfterSC, sizeof(IndexData.au8ByteAfterSC));
    pstFidx->pu8ExtraSCData       = HI_NULL;
    pstFidx->u32ExtraSCDataSize   = 0;
    pstFidx->u32ExtraSCDataPhyAddr = 0;

    return HI_SUCCESS;
}

#define TS_PKT_LEN DMX_TS_PACKET_LEN
#define TS_PKT_HEADER_LEN 4
#define PES_PKT_HEADER_LENGTH DMX_PES_HEADER_LENGTH

static inline HI_U32 DmxAdpFldLen(HI_U8 *pTsData)
{
    BUG_ON(0x47 != pTsData[0]);

    if (0x02 == ((pTsData[3] >> 4) & 0x03))
    {
        return TS_PKT_LEN - TS_PKT_HEADER_LEN;
    }
    else if (0x03 == ((pTsData[3] >> 4) & 0x03))
    {
       return  1 + (HI_U32)pTsData[4];
    }

    return 0;
}

static inline HI_U32 DmxPesHeaderLen(HI_U8 *pTsData)
{
    BUG_ON(0x47 != pTsData[0]);

    if (pTsData[1] & 0x40)
    {
        HI_U32 AdpLen = DmxAdpFldLen(pTsData); 

        return PES_PKT_HEADER_LENGTH + (HI_U32)pTsData[4 + AdpLen + 8];
    }

    return 0;
}

static inline HI_U32 DmxGetPid(HI_U8 *pTsData)
{
    BUG_ON(0x47 != *pTsData);

    return ((*(pTsData + 1) & 0x1F) << 8) | *(pTsData + 2);
}

static HI_S32 DmxParserSCData(HI_U32 Pid, HI_U64 ParserOffset, HI_U8 *TsRecBuf, HI_U32 TsRecBufSize, HI_U8 *SCDataBuf, HI_U32 SCDataBufLen)
{
    HI_U64 FrameGlobalOffset;
    HI_U8 *pData, *pDataSC;
    HI_U32 DestIdx, SrcIdx;

    FrameGlobalOffset = ParserOffset;
    pDataSC = (HI_U8*)(do_div(FrameGlobalOffset, (HI_U64)TsRecBufSize) + TsRecBuf);
    
    for (pData = pDataSC, DestIdx = 0, SrcIdx = 0;DestIdx < SCDataBufLen;SrcIdx ++, pData ++)
    {   
        /* buf rewind */
        if (pData == TsRecBuf + TsRecBufSize)
        {
            pData = TsRecBuf;
        }

        FrameGlobalOffset = ParserOffset + SrcIdx;
        if (unlikely(0x47 == *pData && 0 == do_div(FrameGlobalOffset, TS_PKT_LEN)))
        {
            if (Pid != DmxGetPid(pData))
            {
                SrcIdx += (TS_PKT_LEN) - 1; /* skip entire ts pkt, continue will increase SrcIdx immeidately, so here sub 1 */
                pData += (TS_PKT_LEN) - 1;

                continue;
            }
            else /* skip ts header,adp, pes header field. */
            {
                HI_U32 SkipLen = TS_PKT_HEADER_LEN + DmxAdpFldLen(pData) + DmxPesHeaderLen(pData);

                SrcIdx += SkipLen - 1;  /* continue will increase SrcIdx immeidately, so here sub 1 */
                pData += SkipLen - 1;
                
                continue;
            }
        }
        
        SCDataBuf[DestIdx ++] = *pData;
    }

    return HI_SUCCESS;
}

static HI_S32 DmxParserFilterSCData(HI_U32 Pid, HI_U64 ParserOffset, HI_U8 *TsRecBuf, HI_U32 TsRecBufSize, HI_U8 *SCDataBuf, HI_U32 SCDataBufLen, HI_U32 *MonoParseLen)
{
    HI_U64 FrameGlobalOffset;
    HI_U8 *pData, *pDataSC, FltData[3] = {0xFF, 0xFF, 0xFF};
    HI_U32 DestIdx, SrcIdx, FltIdx, CurFltIdx;

    FrameGlobalOffset = ParserOffset;
    pDataSC = (HI_U8*)(do_div(FrameGlobalOffset, (HI_U64)TsRecBufSize) + TsRecBuf);
    
    for (pData = pDataSC, SrcIdx = 0, DestIdx = 0, FltIdx = 0;DestIdx < SCDataBufLen;pData ++, SrcIdx ++)
    {   
        /* buf rewind */
        if (pData == TsRecBuf + TsRecBufSize)
        {
            pData = TsRecBuf;
        }

        FrameGlobalOffset = ParserOffset + SrcIdx;
        if (unlikely(0x47 == *pData && 0 == do_div(FrameGlobalOffset, TS_PKT_LEN)))
        {
            if (Pid != DmxGetPid(pData))
            {
                SrcIdx += (TS_PKT_LEN) - 1; /* skip entire ts pkt, continue will increase SrcIdx immeidately, so here sub 1 */
                pData += (TS_PKT_LEN) - 1;

                continue;
            }
            else /* skip ts header,adp, pes header field. */
            {
                HI_U32 SkipLen = TS_PKT_HEADER_LEN + DmxAdpFldLen(pData) + DmxPesHeaderLen(pData);

                SrcIdx += SkipLen - 1;  /* continue will increase SrcIdx immeidately, so here sub 1 */
                pData += SkipLen - 1;
                
                continue;
            }
        }

        /* according to hevc protocol , key pair('00 00 03') should filter '03' from data after start code. */
        CurFltIdx = FltIdx ++;
        FltData[CurFltIdx % 3] = *pData;

        if (unlikely(0x03 == *pData))
        {
            if ((0 == (CurFltIdx % 3) && 0x0 == FltData[1] && 0x0 == FltData[2]) ||
                (1 == (CurFltIdx % 3) && 0x0 == FltData[0] && 0x0 == FltData[2]) ||
                (2 == (CurFltIdx % 3) && 0x0 == FltData[0] && 0x0 == FltData[1]))
            {
                continue;
            }
        }
        
        SCDataBuf[DestIdx ++] = *pData;
    }

    if (MonoParseLen)
    {
        *MonoParseLen = SrcIdx;
    }

    return HI_SUCCESS;
}

static HI_S32 DmxFixupHevcEsIndex(Dmx_Set_S *DmxSet, DMX_RecInfo_S *RecInfo, FINDEX_SCD_S *pstFidx)
{
    HI_S32 ret = HI_FAILURE;
    DMX_FQ_Info_S *RecDataFq = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    HI_U8 *pBuf = RecDataFq->BufVirAddr;
    HI_U32 BufLen = RecDataFq->BufSize;
    HI_U8  *pDataAfterSC, *pDupDataBuf;
    HI_U32 LoopTimes = 0;

    BUG_ON(sizeof(HI_U8) * HEVC_DUP_DATA_TOTAL_LEN != pstFidx->u32ExtraSCDataSize);

    pDupDataBuf = pstFidx->pu8ExtraSCData;
    
    ret = DmxParserSCData(RecInfo->IndexPid, pstFidx->s64GlobalOffset, pBuf, BufLen, pDupDataBuf, HEVC_DUP_DATA_CMP_LEN);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    /* verify start code first. */
    if (unlikely(!(0x0 == pDupDataBuf[0] && 0x0 == pDupDataBuf[1] && 
            (0x0 == pDupDataBuf[2] || 0x1 == pDupDataBuf[2] || 0x2 == pDupDataBuf[2] || 0x3 == pDupDataBuf[2]) && pstFidx->u8StartCode == pDupDataBuf[3])))
    {
        HI_ERR_DEMUX("invalid start Code(0x%02x 0x%02x 0x%02x 0x%02x) at offset(0x%llx).\n", pDupDataBuf[0], pDupDataBuf[1], pDupDataBuf[2], pDupDataBuf[3], pstFidx->s64GlobalOffset);
        ret = HI_FAILURE;
        goto out;
    }

    pDataAfterSC = pDupDataBuf + HEVC_DATA_OF_SC_OFFSET;
    
    /* verify saved bytes */
    if (unlikely(memcmp(pDataAfterSC, pstFidx->au8DataAfterSC, sizeof(HI_U8) * HEVC_DATA_OF_SC_SAVED_LEN)))
    {
        HI_ERR_DEMUX("dismatched bytes(offset:%llx):0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x.\n", pstFidx->s64GlobalOffset,
           pDataAfterSC[0], pDataAfterSC[1], pDataAfterSC[2], pDataAfterSC[3], pDataAfterSC[4], pDataAfterSC[5], pDataAfterSC[6],
           pDataAfterSC[7]);

           HI_ERR_DEMUX("saved bytes(offset:%llx):0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x.\n", pstFidx->s64GlobalOffset,
           pstFidx->au8DataAfterSC[0], pstFidx->au8DataAfterSC[1], pstFidx->au8DataAfterSC[2], pstFidx->au8DataAfterSC[3],
           pstFidx->au8DataAfterSC[4], pstFidx->au8DataAfterSC[5], pstFidx->au8DataAfterSC[6], pstFidx->au8DataAfterSC[7]);

        ret = HI_FAILURE;
        goto out;
    }

    /* verify received enough data */
    do 
    {
        HI_U64 TotalTsRecCnt = 0;
        HI_U32 MonoParseLen = 0;

        ret = DmxParserFilterSCData(RecInfo->IndexPid, pstFidx->s64GlobalOffset, pBuf, BufLen, pDupDataBuf, HEVC_DUP_DATA_TOTAL_LEN, &MonoParseLen);
        if (HI_SUCCESS != ret)
        {
            goto out;
        }

        /* ensure pDupDataBuf is valid data. */
        DmxHalGetRecTsCnt(DmxSet, RecInfo->ScdId, &TotalTsRecCnt);
        if (TotalTsRecCnt * TS_PKT_LEN - pstFidx->s64GlobalOffset >= MonoParseLen)
        {
            break;
        }
        else
        {
            /* wait 1s/2s/4s/8s/16s */
            if (10 == LoopTimes || 20 == LoopTimes || 40 == LoopTimes || 80 == LoopTimes || 160 == LoopTimes)
            {
                HI_ERR_DEMUX("not receive enough index data within %d seconds.\n", LoopTimes / 10);

                if (160 == LoopTimes)
                {
                    HI_ERR_DEMUX("escape wait index data.\n");
                    ret = HI_FAILURE;
                    goto out;
                }
            }

            LoopTimes ++;
            
            if (msleep_interruptible(100))
            {
                ret = HI_FAILURE;
                goto out;
            }
        }
    }while(1);

    ret = DmxParserFilterSCData(RecInfo->IndexPid, pstFidx->s64GlobalOffset, pBuf, BufLen, pDupDataBuf, HEVC_DUP_DATA_TOTAL_LEN, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

out:
    return ret;
}

#if defined(DMX_SECURE_CHANNEL_SUPPORT) && defined(DMX_TEE_SUPPORT)
static HI_S32 DmxTeecParserSCData(DMX_RecInfo_S *RecInfo, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE);
    operation.params[0].value.a = DMX_RECHANDLE(RecInfo->DmxId, RecInfo->RecId);
    operation.params[1].value.a = RecInfo->IndexPid;
    operation.params[1].value.b = ParserOffset;
    operation.params[2].value.a = IdxBufPhyAddr;
    operation.params[2].value.b = IdxBufLen;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_PARSER_SC_DATA, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_PARSER_SC_DATA to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }
    
out:
    return ret;
}

static HI_S32 DmxTeecParserFilterSCData(DMX_RecInfo_S *RecInfo, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen, HI_U32 *MonoParserLen)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT);
    operation.params[0].value.a = DMX_RECHANDLE(RecInfo->DmxId, RecInfo->RecId);
    operation.params[1].value.a = RecInfo->IndexPid;
    operation.params[1].value.b = ParserOffset;
    operation.params[2].value.a = IdxBufPhyAddr;
    operation.params[2].value.b = IdxBufLen;
    
    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_PARSER_FILTER_SC_DATA, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_PARSER_FILTER_SC_DATA to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

    if (MonoParserLen)
    {
        *MonoParserLen = operation.params[3].value.a;
    }
    
out:
    return ret;
}

static HI_S32 DmxFixupSecureHevcEsIndex(Dmx_Set_S *DmxSet, DMX_RecInfo_S *RecInfo, FINDEX_SCD_S *pstFidx)
{
    HI_S32 ret = HI_FAILURE;
    DMX_FQ_Info_S  *RecDataFq = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    HI_U8  *pDataAfterSC, *pDupDataBuf;
    HI_U64 u64FrameGlobalOffset;
    HI_U32 LoopTimes = 0;

    BUG_ON(sizeof(HI_U8) * HEVC_DUP_DATA_TOTAL_LEN != pstFidx->u32ExtraSCDataSize);

    pDupDataBuf = pstFidx->pu8ExtraSCData;

    u64FrameGlobalOffset = pstFidx->s64GlobalOffset;

    ret = DmxTeecParserSCData(RecInfo, do_div(u64FrameGlobalOffset, (HI_U64)RecDataFq->BufSize), pstFidx->u32ExtraSCDataPhyAddr, pstFidx->u32ExtraSCDataSize);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    /* verify start code first. */
    if (unlikely(!(0x0 == pDupDataBuf[0] && 0x0 == pDupDataBuf[1] && 
            (0x0 == pDupDataBuf[2] || 0x1 == pDupDataBuf[2] || 0x2 == pDupDataBuf[2] || 0x3 == pDupDataBuf[2]) && pstFidx->u8StartCode == pDupDataBuf[3])))
    {
        HI_ERR_DEMUX("invalid start Code(0x%02x 0x%02x 0x%02x 0x%02x) at offset(0x%llx).\n", pDupDataBuf[0], pDupDataBuf[1], pDupDataBuf[2], pDupDataBuf[3], pstFidx->s64GlobalOffset);
        ret = HI_FAILURE;
        goto out;
    }

    pDataAfterSC = pDupDataBuf + HEVC_DATA_OF_SC_OFFSET;
    
    /* verify saved bytes */
    if (unlikely(memcmp(pDataAfterSC, pstFidx->au8DataAfterSC, sizeof(HI_U8) * HEVC_DATA_OF_SC_SAVED_LEN)))
    {
        HI_ERR_DEMUX("dismatched bytes(offset:%llx):0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x.\n", pstFidx->s64GlobalOffset,
           pDataAfterSC[0], pDataAfterSC[1], pDataAfterSC[2], pDataAfterSC[3], pDataAfterSC[4], pDataAfterSC[5], pDataAfterSC[6],
           pDataAfterSC[7]);

           HI_ERR_DEMUX("saved bytes(offset:%llx):0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x.\n", pstFidx->s64GlobalOffset,
           pstFidx->au8DataAfterSC[0], pstFidx->au8DataAfterSC[1], pstFidx->au8DataAfterSC[2], pstFidx->au8DataAfterSC[3],
           pstFidx->au8DataAfterSC[4], pstFidx->au8DataAfterSC[5], pstFidx->au8DataAfterSC[6], pstFidx->au8DataAfterSC[7]);

        ret = HI_FAILURE;
        goto out;
    }

    /* verify received enough data */
    do 
    {
        HI_U64 TotalTsRecCnt = 0;
        HI_U32 MonoParseLen = 0;

        u64FrameGlobalOffset = pstFidx->s64GlobalOffset;

        ret = DmxTeecParserFilterSCData(RecInfo, do_div(u64FrameGlobalOffset, (HI_U64)RecDataFq->BufSize), pstFidx->u32ExtraSCDataPhyAddr, pstFidx->u32ExtraSCDataSize, &MonoParseLen);
        if (HI_SUCCESS != ret)
        {
            goto out;
        }

        /* ensure pDupDataBuf is valid data. */
        DmxHalGetRecTsCnt(DmxSet, RecInfo->ScdId, &TotalTsRecCnt);
        if (TotalTsRecCnt * TS_PKT_LEN - pstFidx->s64GlobalOffset >= MonoParseLen)
        {
            break;
        }
        else
        {
            /* wait 1s/2s/4s/8s/16s */
            if (10 == LoopTimes || 20 == LoopTimes || 40 == LoopTimes || 80 == LoopTimes || 160 == LoopTimes)
            {
                HI_ERR_DEMUX("not receive enough index data within %d seconds.\n", LoopTimes / 10);

                if (160 == LoopTimes)
                {
                    HI_ERR_DEMUX("escape wait index data.\n");
                    ret = HI_FAILURE;
                    goto out;
                }
            }

            LoopTimes ++;

            if (msleep_interruptible(100))
            {
                ret = HI_FAILURE;
                goto out;
            }
        }
    }while(1);

    u64FrameGlobalOffset = pstFidx->s64GlobalOffset;
    
    ret = DmxTeecParserFilterSCData(RecInfo, do_div(u64FrameGlobalOffset, (HI_U64)RecDataFq->BufSize), pstFidx->u32ExtraSCDataPhyAddr, pstFidx->u32ExtraSCDataSize, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

out:    
    return ret;
}
#else
static HI_S32 DmxFixupSecureHevcEsIndex(Dmx_Set_S *DmxSet, DMX_RecInfo_S *RecInfo, FINDEX_SCD_S *pstFidx)
{
    HI_ERR_DEMUX("DMX_SECURE_CHANNEL_SUPPORT or DMX_TEE_SUPPORT not configured.\n");
    return HI_FAILURE;
}
#endif

HI_S32 DmxFixupHevcIndex(Dmx_Set_S *DmxSet, DMX_RecInfo_S *RecInfo, FINDEX_SCD_S *pstFidx)
{
    HI_S32 ret = HI_FAILURE;

    if (DMX_INDEX_SC_TYPE_PIC == pstFidx->u8IndexType)
    {
        if (HI_UNF_DMX_SECURE_MODE_TEE == RecInfo->SecureMode)
        {
            ret = DmxFixupSecureHevcEsIndex(DmxSet, RecInfo, pstFidx);
        }
        else
        {                                    
            ret = DmxFixupHevcEsIndex(DmxSet, RecInfo, pstFidx);
        }
    }
    else if (DMX_INDEX_SC_TYPE_PIC_SHORT == pstFidx->u8IndexType)
    {
        BUG();
    }
    else
    {
        ret = HI_SUCCESS;
    }

    return ret;
}

HI_S32 DmxScdToAudioIndex(HI_MPI_DMX_REC_INDEX_S *CurrFrame, const DMX_IDX_DATA_S *ScData)
{
    HI_U32  IndexType   = (ScData->u32Chn_Ovflag_IdxType_Flags >> 24) & 0xF;
    HI_U32  PtsValid    = (ScData->u32TsCntHi8_Byte345AfterSc >> 1) & 0x1;
    // HI_U64  LastOffset  = CurrFrame->u64GlobalOffset;
    HI_U64  Pts         = 0;
    HI_U32  Pts33       = ScData->u32TsCntHi8_Byte345AfterSc & 0x1;
    HI_U32  Pts32       = ScData->u32ScCode_Byte678AfterSc;
    HI_U32  PtsValue;
    HI_U64  TsCount;
    HI_U32  OffsetInTs;
    HI_U32  BackTsCount;

    if (DMX_INDEX_SC_TYPE_PTS != IndexType)
    {
        return HI_FAILURE;
    }

    if (Pts33)
    {
        Pts = 0x100000000ULL;
    }

    Pts += (HI_U64)Pts32;

    Pts = Pts >> 1;

    PtsValue = (HI_U32)Pts;

    PtsValue /= 45;

    OffsetInTs  = ScData->u32ScType_Byte12AfterSc_OffsetInTs & 0xff;
    TsCount     = (HI_U64)(ScData->u32TsCntHi8_Byte345AfterSc & 0xff000000) << 8;
    TsCount     = TsCount | ScData->u32TsCntLo32;
    BackTsCount = ScData->u32BackPacetNum & 0x0001fffff;

    CurrFrame->enFrameType      = HI_UNF_FRAME_TYPE_I;
    CurrFrame->u32PtsMs         = PtsValid ? PtsValue : PVR_INDEX_INVALID_PTSMS;
    CurrFrame->u64GlobalOffset  = (TsCount - BackTsCount - 1) * 188 + OffsetInTs;
    //CurrFrame->u32FrameSize     = (HI_U32)(CurrFrame->u64GlobalOffset - LastOffset);
    CurrFrame->u32DataTimeMs    = ScData->u32SrcClk / 90;

    return HI_SUCCESS;
}

HI_VOID DmxRecUpdateFrameInfo(HI_U32 *Param, FRAME_POS_S *IndexInfo)
{
    HI_MPI_DMX_REC_INDEX_S *FrameInfo = (HI_MPI_DMX_REC_INDEX_S*)Param;
    HI_U32 idx;

    switch (IndexInfo->eFrameType)
    {
        case FIDX_FRAME_TYPE_I :
            FrameInfo->enFrameType = HI_UNF_FRAME_TYPE_I;
            break;

        case FIDX_FRAME_TYPE_P :
            FrameInfo->enFrameType = HI_UNF_FRAME_TYPE_P;
            break;

        case FIDX_FRAME_TYPE_B :
            FrameInfo->enFrameType = HI_UNF_FRAME_TYPE_B;
            break;

        case FIDX_FRAME_TYPE_IDR :
            FrameInfo->enFrameType = HI_UNF_FRAME_TYPE_IDR;
            break;

        default :
            return;
    }

    FrameInfo->u32PtsMs          = IndexInfo->u32PTS;
    FrameInfo->u64GlobalOffset   = (HI_U64)IndexInfo->s64GlobalOffset;
    FrameInfo->u32FrameSize      = (HI_U32)IndexInfo->s32FrameSize;

    FrameInfo->s16CurPoc = (HI_S16)IndexInfo->s32CurPoc;
    FrameInfo->u16RefPocCnt = (HI_U16)IndexInfo->s32RefNum;
    for (idx = 0; idx < FrameInfo->u16RefPocCnt; idx ++)
    {
        FrameInfo->as16RefPoc[idx] = (HI_S16)IndexInfo->s32RefPoc[idx];
    }
}

