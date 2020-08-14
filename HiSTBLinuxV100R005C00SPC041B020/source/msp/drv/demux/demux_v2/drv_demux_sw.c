
#include <linux/kthread.h>

#include "hi_type.h"
#include "hi_module.h"
#include "hi_drv_mmz.h"
#include "hi_drv_sys.h"
#include "hi_kernel_adapt.h"

#include "demux_debug.h"
#include "drv_demux_sw.h"
#include "hal_demux.h"
#include "drv_demux_reg.h"
#include "drv_demux_func.h"
#include "drv_demux_osal.h"
#include "hi_drv_file.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef DMX_USE_ECM

typedef union
{
    struct
    {
        unsigned int sync_byte                    : 8;
        unsigned int head_byte1                   : 8;/*transport_error_indicator    : 1;
                                                        payload_unit_start_indicator : 1;
                                                        transport_priority           : 1;
                                                        PID                          : 5;*/
        unsigned int head_byte2                   : 8;/*PID                          : 8;*/
        unsigned int head_byte3                   : 8;/*transport_scrambling_control : 2;
                                                        adaptation_field_control     : 2;
                                                        continuity_counter           : 4;*/
    } bits;

    unsigned int u32;
} U_DMX_TS_HEADER;

typedef struct
{
    U_DMX_TS_HEADER ts_header;
    HI_U8           u8Payload[184]; //include adaption and payload
} TS_PACKET;

typedef struct
{
    HI_U32 u32StartPhyAddr;
    HI_U16 u16BufSize;
    HI_U16 u16DataLen;
} st_SW_DESC;

typedef struct
{
    MMZ_BUFFER_S dataBuf;
    MMZ_BUFFER_S dscBuf;
    HI_U32       u32DescSize;
    HI_U32       u32ReadPtr;
    HI_U32       u32WritePtr;
    HI_U32       u32ProcesPtr;
    HI_U32       u32ProcesOffset;
    HI_U32       u32WriteOffset;
    HI_U8        u8TmpDataArry[2];
    HI_U8        u8TmpArryLen;
} st_SW_DataBuf;

typedef struct
{
    HI_U32        u32UsedFlag;
    HI_U32        u32RunFlag;
    HI_U32        u32CC; //continuity_counter in ts header
    st_SW_DataBuf stDataBuf;
    struct semaphore stSwMutex;
} ST_SW_ChnInfor;

static HI_U32 gu32SWThreadRunFlag = 0;

static ST_SW_ChnInfor stSwChn[DMX_CHANNEL_CNT];
static struct task_struct *pstDemuxSwThread = HI_NULL;

extern DMX_DEV_OSI_S *g_pDmxDevOsi;


#define LOCKSWCHN(chnid) \
    {\
        down(&(stSwChn[(chnid)].stSwMutex)); \
    } \

#define UNLOCKSWCHN(chnid) \
    {\
        up(&(stSwChn[(chnid)].stSwMutex)); \
    } \

#define DMX_SW_DESC_SIZE (sizeof(st_SW_DESC))

#define CHECKSECLEN(chnid,len)\
        if ((len) > DMX_MAX_SEC_LEN)\
        {\
            DMX_SwClrTmpData((chnid));\
            return HI_FAILURE;\
        }

static const unsigned int crc_table[256] = {
        0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,
        0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
        0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,
        0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
        0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3,
        0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
        0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef,
        0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
        0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,
        0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
        0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,
        0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
        0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4,
        0x0808d07d, 0x0cc9cdca, 0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
        0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,
        0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
        0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc,
        0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
        0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050,
        0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
        0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
        0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
        0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb, 0x4f040d56, 0x4bc510e1,
        0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
        0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5,
        0x3f9b762c, 0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
        0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,
        0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
        0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd,
        0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
        0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,
        0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
        0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,
        0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
        0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e,
        0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
        0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a,
        0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
        0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,
        0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
        0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,
        0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
        0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
    };

unsigned int demux_crc32(unsigned char *data, unsigned int len)
{
    unsigned int    i;
    unsigned int    crc = 0xffffffff;
    unsigned char  *ptr = data;

    for (i = 0; i < len; ++i)
    {
        crc = (crc << 8) ^ crc_table[((crc >> 24) ^ *ptr++) & 0xff];
    }

    return crc;
}

//check if ts packet is valid
static HI_S32  DMX_SwCheckTsPacket(HI_U32 u32ChId, TS_PACKET* ts)
{
    HI_U32 u32CC, u32CCNeed;
    HI_U8 u8Sync = 0;
    HI_U8 u8Pusi;
    HI_U8* p;

    p = (HI_U8*)ts;
    u8Sync = ts->ts_header.bits.sync_byte;
    if (u8Sync != 0x47)
    {
        return HI_FAILURE;
    }

    u32CC = ts->ts_header.bits.head_byte3 & 0x0f;
    u32CCNeed = stSwChn[u32ChId].u32CC + 1;
    if (u32CCNeed > 0x0f)
    {
        u32CCNeed = 0;
    }

    u8Pusi = ts->ts_header.bits.head_byte1 & 0x40;
    #if 0//do not drop the packet
    ///TODO:if drop need to judge the discontinuity_indicator in adaption field
    if (u32CC != u32CCNeed)
    {
        //cc not continue,drop the temp data
        DMX_SwClrTmpData(u32ChId);
        if (!u8Pusi) //for the packet have no pusi,drop it directly
        {
            return HI_FAILURE;
        }
    }
    #endif

    stSwChn[u32ChId].u32CC = u32CC;
    return HI_SUCCESS;
}

#if 0

//reserve for future use
static HI_S32  DMX_SwInsertData(HI_U32 u32ChId, HI_U8* pu8Data, HI_U32 u32DataLen)
{
    return HI_SUCCESS;
}

#endif

/**
 \brief get channel data status
 \attention
none
 \param[in] u32ChId
 \param[in] pstBufStatus

 \return 1     channel have data
        0     channel do not have data

 \see
 \li ::
 */
static HI_S32  DMX_SwGetDataStatus(HI_U32 u32ChId, st_SW_DataBuf* pstBufStatus)
{
    st_SW_DataBuf* pstChnBuf;
    st_SW_DESC*    pstWriteDesc;

    pstChnBuf = &stSwChn[u32ChId].stDataBuf;
    if (pstChnBuf->u32ProcesPtr != pstChnBuf->u32WritePtr)
    {
        return 1;
    }
    else
    {
        pstWriteDesc = (st_SW_DESC*)(pstChnBuf->dscBuf.u32StartVirAddr + pstChnBuf->u32WritePtr
                                     * DMX_SW_DESC_SIZE);
        if (pstWriteDesc->u16DataLen > pstChnBuf->u32ProcesOffset)
        {
            return 1;
        }
    }

    return 0;
}

/**
 \brief check section by filter
 \attention:return HI_FAILURE when the channel have no filter
none
 \param[in] Sec
 \param[in] ChnID

 \retval none
 \return HI_SUCCESS pass
        HI_FAILURE unpass

 \see
 \li ::
 */
HI_S32 DMX_SwSectionFilterCheck(HI_U32 ChanId, HI_U8 *Sect, HI_U32 SectLen)
{
    DMX_DEV_OSI_S      *pDmxDevOsi = g_pDmxDevOsi;
    DMX_FilterInfo_S   *FilterInfo;
    HI_U32              i;

    for (i = 0; i < DMX_FILTER_CNT; i++)
    {
        HI_U32  j;
        HI_U32  len = 0;

        FilterInfo = &pDmxDevOsi->DmxFilterInfo[i];
        if (ChanId != FilterInfo->ChanId)
        {
            continue;
        }

        if (0 == FilterInfo->Depth)
        {
            return HI_SUCCESS;
        }

        if (SectLen < FilterInfo->Depth + SECTION_LENGTH_FIELD_SIZE)
        {
            continue;
        }

        for (j = 0; j < FilterInfo->Depth; j++)
        {
            if (FilterInfo->Negate[j])
            {
                if ((FilterInfo->Match[j] & ~FilterInfo->Mask[j]) == (Sect[len] & ~FilterInfo->Mask[j]))
                {
                    break;
                }
            }
            else
            {
                if ((FilterInfo->Match[j] & ~FilterInfo->Mask[j]) != (Sect[len] & ~FilterInfo->Mask[j]))
                {
                    break;
                }
            }

            if (0 == j)
            {
                len = 3;
            }
            else
            {
                ++len;
            }
        }

        if (j == FilterInfo->Depth)
        {
            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}

/**
 \brief check section crc
 \attention
none
 \param[in] Sec
 \param[in] ChnID

 \retval none
 \return HI_SUCCESS pass
        HI_FAILURE unpass

 \see
 \li ::
 */
extern unsigned int demux_crc32(unsigned char *data, unsigned int len);

HI_S32 DMX_SwSectionCRC(HI_U32 ChnID, HI_U8 *Sec, HI_U32 u32SecLen)
{
    DMX_DEV_OSI_S *pDmxDevOsi = g_pDmxDevOsi;
    HI_UNF_DMX_CHAN_CRC_MODE_E CrcMode;
    HI_U32 u32Crc;

    CrcMode = pDmxDevOsi->DmxChanInfo[ChnID].ChanCrcMode;
    if (    (HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD == CrcMode)
        || ((HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD == CrcMode) && (Sec[1] & 0x80)) )
    {
        if (u32SecLen < 7) //crc error len < 7
        {
            HI_WARN_DEMUX("crc error:len < 7,len:%d!\n",u32SecLen);
            return HI_FAILURE;
        }

        u32Crc = demux_crc32(Sec, u32SecLen);
        if (u32Crc) //crc error
        {
            HI_WARN_DEMUX("crc error!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

/**
 \brief check if channel is overflow
 \param[in] u32ChId

 \retval none
 \return 1 : overflow or error happens
        0 : not overflow

 */
static HI_U32 DMX_SwCheckIfChnOvfl(HI_U32 u32ChId)
{
    st_SW_DataBuf* pstBufStatus = HI_NULL;
    HI_U32 u32UsedSize;

    pstBufStatus = &stSwChn[u32ChId].stDataBuf;

    if (pstBufStatus->u32WritePtr >= pstBufStatus->u32ReadPtr)
    {
        u32UsedSize = pstBufStatus->u32WritePtr - pstBufStatus->u32ReadPtr;
    }
    else
    {
        u32UsedSize = pstBufStatus->u32DescSize - pstBufStatus->u32ReadPtr + pstBufStatus->u32WritePtr;
    }

    if ((pstBufStatus->u32DescSize - u32UsedSize) < ((pstBufStatus->u32DescSize * DMX_SW_OVERFL_PERCENT) / 100))
    {
        return 1;
    }

    return 0;
}

static HI_S32 DMX_SwGetNewBlock(HI_U32 u32ChId, HI_U32 *pu32NewBlk)
{
    st_SW_DataBuf* pstDataBuf;
    st_SW_DESC*    pstWriteDesc;
    HI_U32 u32WritePtr;

    pstDataBuf  = &stSwChn[u32ChId].stDataBuf;
    u32WritePtr = pstDataBuf->u32WritePtr;
    DMXINC(u32WritePtr, pstDataBuf->u32DescSize);

    //write pointer can't equals read pointer,for you can't finger out if the queue is full or empty
    if (u32WritePtr == pstDataBuf->u32ReadPtr)
    {
        return HI_FAILURE;
    }

    pstWriteDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32WritePtr * DMX_SW_DESC_SIZE);
    pstWriteDesc->u16DataLen = 0;
    pstDataBuf->u32WritePtr = u32WritePtr;
    pstDataBuf->u32WriteOffset = 0;
    *pu32NewBlk = u32WritePtr;
    return HI_SUCCESS;
}

static HI_S32 DMX_SwParseTmpData(HI_U32 u32ChId)
{
    st_SW_DESC*    pstWriteDesc;
    st_SW_DataBuf* pstDataBuf;
    HI_U8*         pu8Data;
    HI_U32 u32WriteBlk;
    HI_U32 u32TempDataLen;
    HI_U32 u32SecLen;
    HI_U32 u32BufVirAddr;

    pstDataBuf     = &stSwChn[u32ChId].stDataBuf;
    u32WriteBlk    = pstDataBuf->u32WritePtr;
    pstWriteDesc   = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32WriteBlk * DMX_SW_DESC_SIZE);
    u32TempDataLen = pstDataBuf->u32WriteOffset - pstWriteDesc->u16DataLen;
    do
    {
        u32BufVirAddr = pstWriteDesc->u32StartPhyAddr
                        - pstDataBuf->dataBuf.u32StartPhyAddr
                        + pstDataBuf->dataBuf.u32StartVirAddr;
        pu8Data   = (HI_U8*)(u32BufVirAddr + pstWriteDesc->u16DataLen);
        u32SecLen = ((pu8Data[1] & 0x0f) << 8) + pu8Data[2] + 3;
        if (u32TempDataLen >= u32SecLen)
        {
            if (DMX_SwSectionFilterCheck(u32ChId, pu8Data, u32SecLen) != HI_SUCCESS
                || DMX_SwSectionCRC(u32ChId, pu8Data, u32SecLen) != HI_SUCCESS)
            {
                //drop this section
                if (u32TempDataLen > u32SecLen)
                {
                    memcpy(pu8Data, pu8Data + u32SecLen, u32TempDataLen - u32SecLen);
                }

                pstDataBuf->u32WriteOffset -= u32SecLen;
                if (pstDataBuf->u32WriteOffset < pstWriteDesc->u16DataLen)
                {
                    HI_ERR_DEMUX("DMX_SwParseTmpData error:offset:%x,len:%x!\n", pstDataBuf->u32WriteOffset,
                                 pstWriteDesc->u16DataLen);
                    pstDataBuf->u32WriteOffset = pstWriteDesc->u16DataLen;
                    return HI_SUCCESS;
                }
            }
            else
            {
                pstWriteDesc->u16DataLen += u32SecLen;

            }
        }
        else
        {
            break;
        }

        u32TempDataLen = pstDataBuf->u32WriteOffset - pstWriteDesc->u16DataLen;
    } while (u32TempDataLen > 3);

    return HI_SUCCESS;
}
static HI_S32 DMX_SwClrTmpData(HI_U32 u32ChId)
{
    st_SW_DESC*    pstWriteDesc;
    st_SW_DataBuf* pstDataBuf;
    HI_U32 u32WriteBlk;

    pstDataBuf   = &stSwChn[u32ChId].stDataBuf;
    u32WriteBlk  = pstDataBuf->u32WritePtr;
    pstWriteDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32WriteBlk * DMX_SW_DESC_SIZE);
    pstDataBuf->u32WriteOffset = pstWriteDesc->u16DataLen;
    pstDataBuf->u8TmpArryLen = 0;
    return HI_SUCCESS;
}
//u32Pusi = payload_unit_start_indicator. if u32Pusi stand for new section begines
static HI_S32  DMX_SwInsertTmpData(HI_U32 u32ChId, HI_U8* pu8Data, HI_U32 u32DataLen, HI_U32 u32Pusi)
{
    st_SW_DataBuf* pstDataBuf;
    st_SW_DESC*    pstWriteDesc;
    HI_U32 u32TempDataLen, u32ValidBufLen, u32NeedBufLen;
    HI_U32 u32TempDataStartAddr;
    HI_U32 u32WriteBlk;
    HI_S32 ret;
    HI_U32 u32BufVirAddr;
    HI_U32 u32CurSecLen;


    pstDataBuf  = &stSwChn[u32ChId].stDataBuf;
    u32WriteBlk = pstDataBuf->u32WritePtr;

    pstWriteDesc   = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32WriteBlk * DMX_SW_DESC_SIZE);
    u32TempDataLen = pstDataBuf->u32WriteOffset - pstWriteDesc->u16DataLen;
    u32BufVirAddr  = pstWriteDesc->u32StartPhyAddr
                     - pstDataBuf->dataBuf.u32StartPhyAddr
                     + pstDataBuf->dataBuf.u32StartVirAddr;
    if (u32DataLen == 0)
    {
        return HI_SUCCESS;
    }

    if (u32Pusi)
    {
        if (u32TempDataLen) //clear the pre temp data
        {
            DMX_SwParseTmpData(u32ChId);
        }

        pstDataBuf->u8TmpArryLen = 0; //clear temp arry data
        pstDataBuf->u32WriteOffset = pstWriteDesc->u16DataLen; //clear temp data
        if (u32DataLen >= 3)
        {
            do
            {
                u32ValidBufLen = pstWriteDesc->u16BufSize - pstDataBuf->u32WriteOffset;
                if (pu8Data[0] == 0xff) //blank data,drop it
                {
                    u32DataLen = 0;
                    DMX_SwClrTmpData(u32ChId);
                    break;
                }

                u32CurSecLen = ((pu8Data[1] & 0x0f) << 8) + pu8Data[2] + 3;
                CHECKSECLEN(u32ChId,u32CurSecLen);
                u32NeedBufLen = u32CurSecLen;
                if (u32ValidBufLen < u32NeedBufLen)
                {
                    ret = DMX_SwGetNewBlock(u32ChId, &u32WriteBlk); //get a new block
                    if (HI_SUCCESS != ret)
                    {
                        HI_WARN_DEMUX("DMX_SwGetNewBlock error:%x!\n", ret);
                        DMX_SwClrTmpData(u32ChId);
                        return HI_FAILURE;
                    }

                    pstWriteDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32WriteBlk
                                                 * DMX_SW_DESC_SIZE);
                    u32BufVirAddr = pstWriteDesc->u32StartPhyAddr
                                    - pstDataBuf->dataBuf.u32StartPhyAddr
                                    + pstDataBuf->dataBuf.u32StartVirAddr;
                }

                if (u32DataLen >= u32NeedBufLen)
                {
                    memcpy((HI_VOID*)(u32BufVirAddr + pstDataBuf->u32WriteOffset), pu8Data, u32NeedBufLen);
                    pstDataBuf->u32WriteOffset += u32NeedBufLen;
                    DMX_SwParseTmpData(u32ChId); //data pares,do filter & crc etc.
                    u32DataLen -= u32NeedBufLen;
                    pu8Data += u32NeedBufLen;
                }
                else
                {
                    memcpy((HI_VOID*)(u32BufVirAddr + pstDataBuf->u32WriteOffset), pu8Data, u32DataLen);
                    pstDataBuf->u32WriteOffset += u32DataLen;
                    u32DataLen = 0;
                }
            } while (u32DataLen >= 3);
        }

        if (u32DataLen)
        {
            if(pu8Data[0] != 0xff)
            {
                pstDataBuf->u8TmpDataArry[0] = pu8Data[0];
                if (u32DataLen == 2)
                {
                    pstDataBuf->u8TmpDataArry[1] = pu8Data[1];
                }
                pstDataBuf->u8TmpArryLen = u32DataLen;
            }
            else
            {
                DMX_SwClrTmpData(u32ChId);
            }
        }
    }
    else
    {
        if (!u32TempDataLen)
        {
            if (!pstDataBuf->u8TmpArryLen)
            {
                return HI_SUCCESS; //no temp data and no pusi,drop it
            }
            else
            {
                if ((pstDataBuf->u8TmpArryLen + u32DataLen) >= 3)
                {
                    if (pstDataBuf->u8TmpArryLen == 2)
                    {
                        u32CurSecLen = ((pstDataBuf->u8TmpDataArry[1] & 0x0f) << 8) + pu8Data[0] + 3;
                    }
                    else //u8TmpArryLen == 1
                    {
                        u32CurSecLen = ((pu8Data[0] & 0x0f) << 8) + pu8Data[1] + 3;
                    }
                    CHECKSECLEN(u32ChId,u32CurSecLen);

                    u32NeedBufLen = u32CurSecLen;
                    pstDataBuf->u32WriteOffset = pstWriteDesc->u16DataLen; //clear temp data
                    u32ValidBufLen = pstWriteDesc->u16BufSize - pstDataBuf->u32WriteOffset;
                    if (u32ValidBufLen < u32NeedBufLen)
                    {
                        ret = DMX_SwGetNewBlock(u32ChId, &u32WriteBlk); //get a new block
                        if (HI_SUCCESS != ret)
                        {
                            HI_WARN_DEMUX("DMX_SwGetNewBlock error:%x!\n", ret);
                            DMX_SwClrTmpData(u32ChId);
                            return HI_FAILURE;
                        }

                        pstWriteDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32WriteBlk
                                                     * DMX_SW_DESC_SIZE);
                        u32BufVirAddr = pstWriteDesc->u32StartPhyAddr
                                        - pstDataBuf->dataBuf.u32StartPhyAddr
                                        + pstDataBuf->dataBuf.u32StartVirAddr;
                    }

                    memcpy((HI_VOID*)(u32BufVirAddr + pstDataBuf->u32WriteOffset), pstDataBuf->u8TmpDataArry,
                                    pstDataBuf->u8TmpArryLen);
                    pstDataBuf->u32WriteOffset += pstDataBuf->u8TmpArryLen;
                    pstDataBuf->u8TmpArryLen = 0;
                }
                else
                {
                    //u8TmpArryLen = 1 && u32DataLen = 1
                    pstDataBuf->u8TmpDataArry[1] = pu8Data[0];
                    pstDataBuf->u8TmpArryLen = 2;
                    return HI_SUCCESS;
                }
            }
        }
        else //u32TempDataLen is >= 3
        {
            u32TempDataStartAddr = u32BufVirAddr + pstWriteDesc->u16DataLen;
            u32CurSecLen = ((*(HI_U8*)(u32TempDataStartAddr + 1) & 0x0f) << 8)
                            + *(HI_U8*)(u32TempDataStartAddr + 2) + 3;
        }

        u32ValidBufLen = pstWriteDesc->u16BufSize - pstDataBuf->u32WriteOffset;
        u32TempDataLen = pstDataBuf->u32WriteOffset - pstWriteDesc->u16DataLen;
        if (u32TempDataLen > u32CurSecLen)
        {
            HI_ERR_DEMUX("error:tmp bufferlen wrong,check it:%x,seclen:%x!\n", u32TempDataLen,
                         u32CurSecLen);
            return HI_FAILURE;
        }

        u32NeedBufLen = u32CurSecLen - u32TempDataLen;
        if (u32ValidBufLen < u32NeedBufLen)
        {
            HI_ERR_DEMUX("error:buffer status wrong,check it:%d,%d,seclen:%d,phyaddr:%x,datalen:%x!\n", u32ValidBufLen,
                         u32NeedBufLen, u32CurSecLen, pstWriteDesc->u32StartPhyAddr,
                         pstWriteDesc->u16DataLen);
            return HI_FAILURE;
        }

        u32TempDataStartAddr = u32BufVirAddr + pstWriteDesc->u16DataLen;
        if (u32DataLen < u32NeedBufLen)
        {
            memcpy((HI_VOID*)(u32BufVirAddr + pstDataBuf->u32WriteOffset), pu8Data, u32DataLen);
            pstDataBuf->u32WriteOffset += u32DataLen;
            return HI_SUCCESS;
        }

        memcpy((HI_VOID*)(u32BufVirAddr + pstDataBuf->u32WriteOffset), pu8Data, u32NeedBufLen);
        pstDataBuf->u32WriteOffset += u32NeedBufLen;
        DMX_SwParseTmpData(u32ChId);
        if (u32DataLen > u32NeedBufLen && pu8Data[u32NeedBufLen] != 0xff)//the data after section is not 0xff,prase it also
        {
            u32DataLen -= u32NeedBufLen;
            pu8Data    += u32NeedBufLen;
            pstDataBuf->u32WriteOffset = pstWriteDesc->u16DataLen; //clear temp data
            while (u32DataLen >= 3)
            {
                u32ValidBufLen = pstWriteDesc->u16BufSize - pstDataBuf->u32WriteOffset;
                if (pu8Data[0] == 0xff) //blank data,drop it
                {
                    u32DataLen = 0;
                    DMX_SwClrTmpData(u32ChId);
                    break;
                }

                u32CurSecLen = ((pu8Data[1] & 0x0f) << 8) + pu8Data[2] + 3;
                CHECKSECLEN(u32ChId,u32CurSecLen);
                u32NeedBufLen = u32CurSecLen;
                if (u32ValidBufLen < u32NeedBufLen)
                {
                    ret = DMX_SwGetNewBlock(u32ChId, &u32WriteBlk); //get a new block
                    if (HI_SUCCESS != ret)
                    {
                        HI_WARN_DEMUX("DMX_SwGetNewBlock error:%x!\n", ret);
                        return HI_FAILURE;
                    }

                    pstWriteDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32WriteBlk
                                                 * DMX_SW_DESC_SIZE);
                    u32BufVirAddr = pstWriteDesc->u32StartPhyAddr
                                    - pstDataBuf->dataBuf.u32StartPhyAddr
                                    + pstDataBuf->dataBuf.u32StartVirAddr;
                }

                if (u32DataLen >= u32NeedBufLen)
                {
                    memcpy((HI_VOID*)(u32BufVirAddr + pstDataBuf->u32WriteOffset), pu8Data, u32NeedBufLen);
                    pstDataBuf->u32WriteOffset += u32NeedBufLen;
                    DMX_SwParseTmpData(u32ChId); //data pares,do filter & crc etc.
                    u32DataLen -= u32NeedBufLen;
                    pu8Data += u32NeedBufLen;
                }
                else
                {
                    memcpy((HI_VOID*)(u32BufVirAddr + pstDataBuf->u32WriteOffset), pu8Data, u32DataLen);
                    pstDataBuf->u32WriteOffset += u32DataLen;
                    u32DataLen = 0;
                }
            }
            if (u32DataLen)
            {
                if(pu8Data[0] != 0xff)
                {
                    pstDataBuf->u8TmpDataArry[0] = pu8Data[0];
                    if (u32DataLen == 2)
                    {
                        pstDataBuf->u8TmpDataArry[1] = pu8Data[1];
                    }
                    pstDataBuf->u8TmpArryLen = u32DataLen;
                }
                else
                {
                    DMX_SwClrTmpData(u32ChId);
                }
            }
        }
    }

    return HI_SUCCESS;
}
static HI_S32  DMX_SwResetChannel(HI_U32 u32ChId)
{
    st_SW_DataBuf* pstDataBuf;
    st_SW_DESC*    pstWriteDesc;

    pstDataBuf = &stSwChn[u32ChId].stDataBuf;
    pstDataBuf->u32ReadPtr  = 0;
    pstDataBuf->u32WritePtr = 0;
    pstDataBuf->u32WriteOffset = 0;
    pstDataBuf->u32ProcesPtr = 0;
    pstDataBuf->u32ProcesOffset = 0;
    pstWriteDesc = (st_SW_DESC*)pstDataBuf->dscBuf.u32StartVirAddr;
    pstWriteDesc->u16DataLen = 0;
    return HI_SUCCESS;
}

static HI_S32 DMX_SwGetTsHeaderLen(TS_PACKET* ts, HI_U32 *pu32HeaderLen)
{
    /*
    adaptation_field_control:
    00  reserved for future use by ISO/IEC
    01  no adaptation_field, payload only
    10  adaptation_field only, no payload
    11  adaptation_field followed by payload
     */
    HI_U32 u32AdapLen = 0;

    switch ((ts->ts_header.bits.head_byte3 >> 4) & 0x3)
    {
    case 1:
        *pu32HeaderLen = sizeof(U_DMX_TS_HEADER);
        return 0;
    case 2:
        *pu32HeaderLen = DMX_TS_PACKET_LEN;
        return 0;
    case 3:
        u32AdapLen = ts->u8Payload[0];
        *pu32HeaderLen = u32AdapLen + sizeof(U_DMX_TS_HEADER) + 1;
        return 0;
    default:
        return -1;
    }

    return -1;
}

//parse ts packet,insert data and manage tmp data in this function
static HI_S32  DMX_SwParseTsPacket(HI_U32 u32ChId, TS_PACKET* ts)
{
    HI_U8 u8Pointer;
    HI_U32 u32HeaderLen;
    HI_S32 s32Ret;

    if (0 != DMX_SwGetTsHeaderLen(ts, &u32HeaderLen))
    {
        HI_WARN_DEMUX("ts header syntax error!\n");
        return HI_FAILURE;
    }

    if (u32HeaderLen >= DMX_TS_PACKET_LEN) //no payload
    {
        return HI_SUCCESS;
    }


    if (ts->ts_header.bits.head_byte1 & 0x40) //payload_unit_start_indicator
    {
        u8Pointer = ts->u8Payload[u32HeaderLen - sizeof(U_DMX_TS_HEADER)];
        if (u8Pointer > (DMX_TS_PACKET_LEN - u32HeaderLen - 1))
        {
            HI_WARN_DEMUX("pointer invalid:%d,header:%d!\n", u8Pointer,u32HeaderLen);
            return HI_FAILURE;
        }
        if (u8Pointer)
        {
            //insert the payload befor the pointer
            s32Ret = DMX_SwInsertTmpData(u32ChId, &ts->u8Payload[u32HeaderLen - sizeof(U_DMX_TS_HEADER) + 1],
                                         u8Pointer, 0);
            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_DEMUX("insert tmp data error:%x!\n", s32Ret);
            }
        }

        //new section
        s32Ret = DMX_SwInsertTmpData(u32ChId, &ts->u8Payload[u32HeaderLen + u8Pointer + 1 - sizeof(U_DMX_TS_HEADER)],
                                     DMX_TS_PACKET_LEN - (u32HeaderLen + u8Pointer + 1), 1);
        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_DEMUX("insert tmp data error:%x!\n", s32Ret);
        }
    }
    else
    {
        s32Ret = DMX_SwInsertTmpData(u32ChId, &ts->u8Payload[u32HeaderLen - sizeof(U_DMX_TS_HEADER)],
                                     DMX_TS_PACKET_LEN - u32HeaderLen, 0);
        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_DEMUX("insert tmp data error:%x!\n", s32Ret);
        }
    }

    return HI_SUCCESS;
}

static HI_U32 DMX_SwGetRunChannelNum(HI_VOID)
{
    HI_U32 i, u32Count = 0;

    for (i = 0; i < DMX_CHANNEL_CNT; i++)
    {
        if (stSwChn[i].u32UsedFlag && stSwChn[i].u32RunFlag)
        {
            u32Count++;
        }
    }

    return u32Count;
}



#define DMX_ACQUIRE_BUF_NUM 500
//#define DMX_SW_DEBUG
#ifdef DMX_SW_DEBUG
static struct file * g_stTsHandle = HI_NULL;
#endif
static HI_S32 HI_DMX_SWExtractthread(HI_VOID *arg)
{
    static DMX_UserMsg_S stReqBufTmp[DMX_ACQUIRE_BUF_NUM];
    DMX_DEV_OSI_S *pDmxDevOsi = g_pDmxDevOsi;
    HI_U32 u32AcquiredNum;
    HI_U32 u32BufVirAddr;
    HI_S32 ret;
    HI_U32 i, j;

#ifdef DMX_SW_DEBUG
    g_stTsHandle = HI_DRV_FILE_Open("/hi/rv.ts", 1);
#endif

    while (1)
    {
        for (i = 0; i < DMX_CHANNEL_CNT; i++)
        {
            if (kthread_should_stop())
            {
                break;
            }
            if (!(stSwChn[i].u32UsedFlag) || !(stSwChn[i].u32RunFlag) )
            {
               continue;
            }

            LOCKSWCHN(i);

            if (!DMX_OsiGetChnDataFlag(i))
            {
                UNLOCKSWCHN(i);
                continue;
            }
            if (DMX_SwCheckIfChnOvfl(i))
            {
                DMX_OsiResetChannel(i,DMX_FLUSH_TYPE_PLAY);//reset channel common buffer
                UNLOCKSWCHN(i);
                continue;
            }


            //read ts packet
            ret = DMX_OsiReadDataRequset(i, DMX_ACQUIRE_BUF_NUM, &u32AcquiredNum, stReqBufTmp, 0);
            if (HI_SUCCESS != ret)
            {
                UNLOCKSWCHN(i);
                HI_WARN_DEMUX("read data error:%x!\n", ret);
                continue;
            }


            //parse ts packet
            for (j = 0; j < u32AcquiredNum; j++)
            {
                u32BufVirAddr = stReqBufTmp[j].u32BufStartAddr
                                - pDmxDevOsi->DmxFqInfo[DMX_FQ_COMMOM].u32BufPhyAddr
                                + pDmxDevOsi->DmxFqInfo[DMX_FQ_COMMOM].u32BufVirAddr;
#ifdef DMX_SW_DEBUG
                HI_DRV_FILE_Write(g_stTsHandle, u32BufVirAddr, 188);
#endif
                if (HI_SUCCESS == DMX_SwCheckTsPacket(i, (TS_PACKET*)u32BufVirAddr) && !(DMX_SwCheckIfChnOvfl(i)))
                {
                    if (HI_SUCCESS != DMX_SwParseTsPacket(i, (TS_PACKET*)u32BufVirAddr))
                    {
                        //pare ts packet error
                        HI_WARN_DEMUX("channel:%d parse ts packet error!\n", i);
                    }
                }
            }

            ret = DMX_OsiReleaseReadData(i, u32AcquiredNum, stReqBufTmp);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_DEMUX("release data error:%x\n", ret);
            }

            UNLOCKSWCHN(i);
        }

        if (kthread_should_stop())
        {
            break;
        }

        msleep(1);
    }

#ifdef DMX_SW_DEBUG
    HI_DRV_FILE_Close(g_stTsHandle);
#endif
    return HI_SUCCESS;
}

HI_S32 HI_DMX_SwInit()
{
    HI_U32 i;

    memset(&stSwChn, 0, sizeof(ST_SW_ChnInfor) * DMX_CHANNEL_CNT);
    for (i = 0; i < DMX_CHANNEL_CNT; i++)
    {
        HI_INIT_MUTEX(&stSwChn[i].stSwMutex);
    }

    return HI_SUCCESS;
}

//close the pre channel,detach filter,set channel type to post and init the sw channel buffer etc.
HI_S32 HI_DMX_SwNewChannel(HI_U32 u32ChannelId)
{
    DMX_DEV_OSI_S  *pDmxDevOsi = g_pDmxDevOsi;
    DMX_ChanInfo_S *ChanInfo;
    st_SW_DESC     *pstDescPtr;
    HI_CHAR         BufName[16];
    HI_U32 u32BlockNum;
    HI_S32 s32Ret;
    HI_U32 i;

    ChanInfo = &pDmxDevOsi->DmxChanInfo[u32ChannelId];
    if ((HI_UNF_DMX_CHAN_TYPE_SEC != ChanInfo->ChanType) && (HI_UNF_DMX_CHAN_TYPE_ECM_EMM != ChanInfo->ChanType))
    {
        HI_ERR_DEMUX("not support channel type:%d\n", u32ChannelId);
        return HI_FAILURE;
    }

    //1. close channel
    DMX_OsiCloseChannel(u32ChannelId);

    //2. set channel to post type
    DmxHalSetChannelAttr(u32ChannelId, DMX_CH_GENERAL);
    DmxHalSetChannelTsPostMode(u32ChannelId, DMX_ENABLE);
    DmxHalSetChannelTsPostThresh(u32ChannelId, DMX_DEFAULT_POST_TH);
    DmxHalSetChannelDataType(u32ChannelId, DMX_CHAN_DATA_TYPE_SEC);

    if (ChanInfo->ChanBufSize < DMX_SW_CHNBUF_MINLEN)
    {
        ChanInfo->ChanBufSize = DMX_SW_CHNBUF_MINLEN;
    }
    else if (ChanInfo->ChanBufSize > DMX_SW_CHNBUF_MAXLEN)
    {
        ChanInfo->ChanBufSize = DMX_SW_CHNBUF_MAXLEN;
    }

    //3.malloc buffer,align with 4k
    u32BlockNum = ChanInfo->ChanBufSize / DMX_MAX_SEC_LEN;
    ChanInfo->ChanBufSize = u32BlockNum * DMX_MAX_SEC_LEN;

    memset(&stSwChn[u32ChannelId].stDataBuf, 0, sizeof(st_SW_DataBuf));
    snprintf(BufName, 16,"DMX_SWChannelBuf[%u]", u32ChannelId);
    s32Ret = HI_DRV_MMZ_AllocAndMap(BufName, HI_NULL, ChanInfo->ChanBufSize, 4,
                                       &(stSwChn[u32ChannelId].stDataBuf.dataBuf));
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_DEMUX("Malloc %s desc failed\n", BufName);
        return HI_ERR_DMX_ALLOC_MEM_FAILED;
    }

    snprintf(BufName, 16,"DMX_SWDescriptor[%u]", u32ChannelId);
    s32Ret = HI_DRV_MMZ_AllocAndMap(BufName, HI_NULL, u32BlockNum * sizeof(st_SW_DESC), 4,
                                       &(stSwChn[u32ChannelId].stDataBuf.dscBuf));
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_DEMUX("Malloc %s desc failed\n", BufName);
        HI_DRV_MMZ_UnmapAndRelease(&stSwChn[u32ChannelId].stDataBuf.dataBuf);
        return HI_ERR_DMX_ALLOC_MEM_FAILED;
    }

    pstDescPtr = (st_SW_DESC*)stSwChn[u32ChannelId].stDataBuf.dscBuf.u32StartVirAddr;

    //init desc
    for (i = 0; i < u32BlockNum; i++)
    {
        pstDescPtr[i].u32StartPhyAddr = stSwChn[u32ChannelId].stDataBuf.dataBuf.u32StartPhyAddr +
                                        i * DMX_MAX_SEC_LEN;
        pstDescPtr[i].u16BufSize = DMX_MAX_SEC_LEN;
        pstDescPtr[i].u16DataLen = 0;
    }

    stSwChn[u32ChannelId].stDataBuf.u32DescSize = u32BlockNum;
    stSwChn[u32ChannelId].u32CC = DMX_SW_INVALID_CC;
    stSwChn[u32ChannelId].u32UsedFlag = 1;
    ChanInfo->u32SwFlag = 1;
    return s32Ret;
}

HI_S32 HI_DMX_SwDestoryChannel(HI_U32 u32ChannelId)
{
    DMX_DEV_OSI_S  *pDmxDevOsi = g_pDmxDevOsi;
    DMX_ChanInfo_S *pNewChInfo;

    pNewChInfo = &pDmxDevOsi->DmxChanInfo[u32ChannelId];
    if (stSwChn[u32ChannelId].u32RunFlag)
    {
        HI_DMX_SwCloseChannel(u32ChannelId);
    }

    LOCKSWCHN(u32ChannelId);
    DmxHalSetChannelTsPostMode(u32ChannelId, DMX_DISABLE);
    stSwChn[u32ChannelId].u32UsedFlag = 0;
    HI_DRV_MMZ_UnmapAndRelease(&stSwChn[u32ChannelId].stDataBuf.dataBuf);
    HI_DRV_MMZ_UnmapAndRelease(&stSwChn[u32ChannelId].stDataBuf.dscBuf);
    UNLOCKSWCHN(u32ChannelId);
    pNewChInfo->u32SwFlag = 0;
    return HI_SUCCESS;
}

//start sw parse thread
HI_S32 HI_DMX_SwOpenChannel(HI_U32 u32ChannelId)
{
    if (!stSwChn[u32ChannelId].u32UsedFlag)
    {
        HI_ERR_DEMUX("channel not in use:%d\n", u32ChannelId);
        return HI_FAILURE;
    }

    if (stSwChn[u32ChannelId].u32RunFlag) //already running
    {
        return HI_SUCCESS;
    }

    LOCKSWCHN(u32ChannelId);
    if (!gu32SWThreadRunFlag) //thread not run
    {
        pstDemuxSwThread = kthread_create(HI_DMX_SWExtractthread, NULL, "DmxSw"); //create thread
        if (IS_ERR(pstDemuxSwThread))
        {
            HI_ERR_DEMUX("create kthread failed!\n");
            UNLOCKSWCHN(u32ChannelId);
            return HI_FAILURE;
        }
        gu32SWThreadRunFlag = 1;
        wake_up_process(pstDemuxSwThread);
    }

    stSwChn[u32ChannelId].u32RunFlag = 1;
    UNLOCKSWCHN(u32ChannelId);

    return HI_SUCCESS;
}

HI_S32 HI_DMX_SwCloseChannel(HI_U32 u32ChannelId)
{
    if (!stSwChn[u32ChannelId].u32UsedFlag)
    {
        HI_ERR_DEMUX("channel not in use:%d\n", u32ChannelId);
        return HI_FAILURE;
    }

    if (!stSwChn[u32ChannelId].u32RunFlag) //not running
    {
        return HI_SUCCESS;
    }

    LOCKSWCHN(u32ChannelId);

    stSwChn[u32ChannelId].u32RunFlag = 0;
    if ((DMX_SwGetRunChannelNum() == 0) && (pstDemuxSwThread != HI_NULL) && gu32SWThreadRunFlag) //no running sw channel,stop thread
    {
        kthread_stop(pstDemuxSwThread);
        gu32SWThreadRunFlag = 0;
        pstDemuxSwThread = HI_NULL;
    }
    DMX_SwResetChannel(u32ChannelId);

    UNLOCKSWCHN(u32ChannelId);

    return HI_SUCCESS;
}

HI_S32 HI_DMX_SwGetChannelDataStatus(HI_U32 u32ChId)
{
    HI_S32 s32Ret;
    st_SW_DataBuf* pstBufStatus;

    if (!stSwChn[u32ChId].u32UsedFlag && stSwChn[u32ChId].u32RunFlag)
    {
        HI_ERR_DEMUX("channel not in use:%d\n", u32ChId);
        return HI_FAILURE;
    }

    //LOCKSWCHN(u32ChId); may call in interupt service routine,do not lock
    s32Ret = DMX_SwGetDataStatus(u32ChId, pstBufStatus);
    //UNLOCKSWCHN(u32ChId);
    return s32Ret;
}

HI_S32 HI_DMX_SwGetChannelSwBufAddr(HI_U32 u32ChId, MMZ_BUFFER_S* pstSwBuf)
{
    if (!stSwChn[u32ChId].u32UsedFlag)
    {
        HI_ERR_DEMUX("channel not in use:%d\n", u32ChId);
        return HI_FAILURE;
    }

    pstSwBuf->u32StartPhyAddr = stSwChn[u32ChId].stDataBuf.dataBuf.u32StartPhyAddr;
    pstSwBuf->u32StartVirAddr = stSwChn[u32ChId].stDataBuf.dataBuf.u32StartVirAddr;
    pstSwBuf->u32Size = stSwChn[u32ChId].stDataBuf.dataBuf.u32Size;
    return HI_SUCCESS;
}

//semilar with function DMXOsiParseSection
extern HI_U32 GetSectionLength(HI_U32 u32BufStartAddr, HI_U32 u32BufLen, HI_U32 u32Offset);
extern HI_U32 s_u32DMXDropCnt[];
static HI_S32 DMXSwParseSection(HI_U32 *pu32Parsed, HI_U32 u32AcqNum, HI_U32 *pu32Offset,
                                HI_U32 u32BufPhyAddr, HI_U32 u32BufLen, HI_U32 u32ChnID,
                                DMX_UserMsg_S* psMsgList)
{
    HI_U32 i;
    HI_U32 u32Seclen;
    HI_U32 u32BufVirAddr;
    st_SW_DataBuf* pstDataBuf;

    if (*pu32Offset >= u32BufLen)
    {
        return HI_FAILURE;
    }

    pstDataBuf = &stSwChn[u32ChnID].stDataBuf;

    u32BufVirAddr = u32BufPhyAddr
                    - pstDataBuf->dataBuf.u32StartPhyAddr
                    + pstDataBuf->dataBuf.u32StartVirAddr;

    for (i = *pu32Parsed; i < u32AcqNum; i++)
    {
        u32Seclen = GetSectionLength(u32BufVirAddr, u32BufLen, *pu32Offset);
        if ((0 == u32Seclen) || (u32Seclen > DMX_MAX_SEC_LEN))
        {
            s_u32DMXDropCnt[u32ChnID]++;
            HI_ERR_DEMUX("  Section Parser  Error seclen:%x,phaddr:%x,u32BufLen:%x,offset:%x!\n", u32Seclen,
                         u32BufPhyAddr, u32BufLen, *pu32Offset);

            *pu32Offset = u32BufLen; //jump the error section
            break;
        }

        psMsgList[i].u32BufStartAddr = u32BufPhyAddr + *pu32Offset;
        psMsgList[i].u32MsgLen = u32Seclen;
        *pu32Offset += u32Seclen;

        if (*pu32Offset >= u32BufLen)
        {
            i++;
            break;
        }
    }

    if (i == *pu32Parsed)
    {
        return HI_FAILURE;
    }

    *pu32Parsed = i;
    return HI_SUCCESS;
}

static HI_S32 DMX_SwReadDataRequset(HI_U32         u32ChId,
                                    HI_U32         u32AcqNum,
                                    HI_U32 *       pu32AcqedNum,
                                    DMX_UserMsg_S* psMsgList)
{
    st_SW_DataBuf* pstDataBuf;
    st_SW_DESC*    pstCurDesc;
    HI_U32 u32DataLen, u32Offset;
    HI_U32 u32WriteBlk, u32ProcsBlk;
    HI_S32 s32Ret;

    if (!stSwChn[u32ChId].u32UsedFlag)
    {
        HI_ERR_DEMUX("channel not in use:%d\n", u32ChId);
        return HI_FAILURE;
    }

    pstDataBuf = &stSwChn[u32ChId].stDataBuf;
    u32Offset     = pstDataBuf->u32ProcesOffset;
    u32ProcsBlk   = pstDataBuf->u32ProcesPtr;
    u32WriteBlk   = pstDataBuf->u32WritePtr;
    *pu32AcqedNum = 0;
    while (u32ProcsBlk != u32WriteBlk)
    {
        pstCurDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32ProcsBlk * DMX_SW_DESC_SIZE);
        u32DataLen = pstCurDesc->u16DataLen;
        if (u32Offset >= u32DataLen)
        {
            DMXINC(u32ProcsBlk, pstDataBuf->u32DescSize);
            pstDataBuf->u32ProcesPtr = u32ProcsBlk;
            pstDataBuf->u32ProcesOffset = 0;
            u32Offset = 0;
            continue;
        }

        s32Ret = DMXSwParseSection(pu32AcqedNum, u32AcqNum, &u32Offset, pstCurDesc->u32StartPhyAddr,
                                   u32DataLen, u32ChId, psMsgList);
        if (s32Ret != HI_SUCCESS)
        {
            if (u32Offset >= u32DataLen)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (u32Offset >= u32DataLen)
            {
                continue;
            }

            if (*pu32AcqedNum >= u32AcqNum)
            {
                break;
            }
        }

        DMXINC(u32ProcsBlk, pstDataBuf->u32DescSize);
        u32Offset = 0;
    }

    if (*pu32AcqedNum > 0)
    {
        pstDataBuf->u32ProcesPtr = u32ProcsBlk;
        pstDataBuf->u32ProcesOffset = u32Offset;
        return HI_SUCCESS;
    }

    u32Offset   = pstDataBuf->u32ProcesOffset;
    u32ProcsBlk = pstDataBuf->u32ProcesPtr;
    u32WriteBlk = pstDataBuf->u32WritePtr;
    if (u32WriteBlk == u32ProcsBlk)
    {
        pstCurDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32WriteBlk * DMX_SW_DESC_SIZE);
        u32DataLen = pstCurDesc->u16DataLen;
        if (u32DataLen > u32Offset)
        {
            s32Ret = DMXSwParseSection(pu32AcqedNum, u32AcqNum, &u32Offset, pstCurDesc->u32StartPhyAddr,
                                       u32DataLen, u32ChId, psMsgList);
            if (*pu32AcqedNum > 0)
            {
                pstDataBuf->u32ProcesOffset = u32Offset;
                return HI_SUCCESS;
            }
            if (u32Offset >= u32DataLen)
            {
                pstDataBuf->u32ProcesOffset = u32Offset;
            }

            return HI_ERR_DMX_NOAVAILABLE_DATA;
        }
    }

    return HI_ERR_DMX_NOAVAILABLE_DATA;
}

HI_S32 HI_DMX_SwReadDataRequest(HI_U32         u32ChId,
                                HI_U32         u32AcqNum,
                                HI_U32 *       pu32AcqedNum,
                                DMX_UserMsg_S* psMsgList,
                                HI_U32         u32TimeOutMs)
{
    HI_S32 s32Ret;
    DMX_DEV_OSI_S  *pDmxDevOsi = g_pDmxDevOsi;
    DMX_ChanInfo_S *pChInfo;
    HI_U32          TimeMs;

    if (!stSwChn[u32ChId].u32UsedFlag)
    {
        HI_ERR_DEMUX("channel not in use:%d\n", u32ChId);
        return HI_FAILURE;
    }

    pChInfo = &pDmxDevOsi->DmxChanInfo[u32ChId];
    pChInfo->u32TotolAcq++;

    HI_DRV_SYS_GetTimeStampMs(&TimeMs);

    pChInfo->u32AcqTimeInterval = TimeMs - pChInfo->u32AcqTime;
    pChInfo->u32AcqTime = TimeMs;

    LOCKSWCHN(u32ChId);
    s32Ret = DMX_SwReadDataRequset(u32ChId, u32AcqNum, pu32AcqedNum, psMsgList);
    UNLOCKSWCHN(u32ChId);
    if ((s32Ret == HI_SUCCESS) && pu32AcqedNum)
    {
        pChInfo->u32HitAcq++;
        return HI_SUCCESS;
    }

    if (!u32TimeOutMs)
    {
        return s32Ret;
    }

    while (u32TimeOutMs >= 100)
    {
        msleep(100);
        LOCKSWCHN(u32ChId);
        s32Ret = DMX_SwReadDataRequset(u32ChId, u32AcqNum, pu32AcqedNum, psMsgList);
        UNLOCKSWCHN(u32ChId);
        if ((s32Ret == HI_SUCCESS) && pu32AcqedNum)
        {
            pChInfo->u32HitAcq++;
            return HI_SUCCESS;
        }

        u32TimeOutMs -= 100;
    }

    return s32Ret;
}

HI_S32 HI_DMX_SwReleaseReadData(HI_U32 u32ChId, HI_U32 u32RelNum, DMX_UserMsg_S* psMsgList)
{
    st_SW_DataBuf* pstDataBuf;
    st_SW_DESC*    pstCurDesc;
    st_SW_DESC*    pstNextDesc;
    DMX_DEV_OSI_S  *pDmxDevOsi = g_pDmxDevOsi;
    DMX_ChanInfo_S   *pChInfo;
    HI_U32 u32RealseFlag, u32NextBlkValidFlag;
    HI_U32 u32WriteBlk, u32ReadBlk, u32ReadBlkNext;
    HI_U32 i = 0;
    HI_U32          TimeMs;

    pstDataBuf = &stSwChn[u32ChId].stDataBuf;
    if (!stSwChn[u32ChId].u32UsedFlag)
    {
        HI_ERR_DEMUX("channel not in use:%d\n", u32ChId);
        return HI_FAILURE;
    }

    LOCKSWCHN(u32ChId);
    pChInfo = &pDmxDevOsi->DmxChanInfo[u32ChId];
    u32ReadBlk  = pstDataBuf->u32ReadPtr;
    u32WriteBlk = pstDataBuf->u32WritePtr;
    pChInfo->u32Release++;

    HI_DRV_SYS_GetTimeStampMs(&TimeMs);

    pChInfo->u32RelTimeInterval = TimeMs - pChInfo->u32RelTime;
    pChInfo->u32RelTime = TimeMs;

    while (u32ReadBlk != u32WriteBlk)
    {
        u32ReadBlkNext = u32ReadBlk;
        DMXINC(u32ReadBlkNext, pstDataBuf->u32DescSize);

        //if (u32ReadBlkNext != u32WriteBlk)
        {
            u32NextBlkValidFlag = 1;
        }

        for (; i < u32RelNum; i++)
        {
            u32RealseFlag = 0;
            pstCurDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32ReadBlk * DMX_SW_DESC_SIZE);
            if ((psMsgList[i].u32BufStartAddr >= pstCurDesc->u32StartPhyAddr)
                && (psMsgList[i].u32BufStartAddr <= (pstCurDesc->u32StartPhyAddr + pstCurDesc->u16DataLen)))
            {
                if ((psMsgList[i].u32BufStartAddr + psMsgList[i].u32MsgLen)
                    >= (pstCurDesc->u32StartPhyAddr + pstCurDesc->u16DataLen))
                {
                    u32RealseFlag = 1;
                    i++;
                }
            }
            else if (u32NextBlkValidFlag)
            {
                pstNextDesc = (st_SW_DESC*)(pstDataBuf->dscBuf.u32StartVirAddr + u32ReadBlkNext * DMX_SW_DESC_SIZE);
                if ((psMsgList[i].u32BufStartAddr >= pstNextDesc->u32StartPhyAddr)
                    && (psMsgList[i].u32BufStartAddr <= (pstNextDesc->u32StartPhyAddr + pstNextDesc->u16DataLen)))
                {
                    u32RealseFlag = 1;
                }
                else
                {
                    HI_ERR_DEMUX("psMsgList[%d].u32BufStartAddr:%x,len:%x,bufferAddr:%x,len:%x,nextptr:%x,nexlen:%x,desc phy:%x\n",
                                 i, psMsgList[i].u32BufStartAddr, psMsgList[i].u32MsgLen, pstCurDesc->u32StartPhyAddr,
                                 pstCurDesc->u16DataLen, pstNextDesc->u32StartPhyAddr, pstNextDesc->u16DataLen,
                                 pstDataBuf->dscBuf.u32StartPhyAddr);
                    UNLOCKSWCHN(u32ChId);
                    return HI_ERR_DMX_INVALID_PARA;
                }
            }

            if (u32RealseFlag)
            {
                pstCurDesc->u16DataLen = 0;
                if (pstDataBuf->u32ProcesPtr == u32ReadBlk)
                {
                    DMXINC(pstDataBuf->u32ProcesPtr, pstDataBuf->u32DescSize);
                    pstDataBuf->u32ProcesOffset = 0;
                }
                DMXINC(u32ReadBlk, pstDataBuf->u32DescSize);
                pstDataBuf->u32ReadPtr = u32ReadBlk;
                break;
            }
        }

        if (i == u32RelNum)
        {
            break;
        }
    }

    UNLOCKSWCHN(u32ChId);
    return HI_SUCCESS;
}

/*
 * This is used to peek the first u32PeekLen of a SEC or PES packet. It does not consume the data in
 * the internal demux section buffer, i.e. it simply copies the requested amount of data
 * but does not move the consumer pointer.
 */
HI_S32 HI_DMX_SwPeekDataRequest(HI_U32         u32ChId,
                                HI_U32         u32PeekLen,
                                DMX_UserMsg_S* psMsgList)
{
    HI_S32 s32Ret;
    DMX_DEV_OSI_S  *pDmxDevOsi = g_pDmxDevOsi;
    DMX_ChanInfo_S *pChInfo;
    HI_U32 u32WriteBlk, u32ProcsBlk,u32Offset;
    HI_U32 u32AcqedNum;
    st_SW_DataBuf* pstDataBuf;

    if (!stSwChn[u32ChId].u32UsedFlag)
    {
        HI_ERR_DEMUX("channel not in use:%d\n", u32ChId);
        return HI_FAILURE;
    }

    pChInfo = &pDmxDevOsi->DmxChanInfo[u32ChId];
    LOCKSWCHN(u32ChId);
    pstDataBuf = &stSwChn[u32ChId].stDataBuf;
    u32Offset     = pstDataBuf->u32ProcesOffset;//store the pointers
    u32ProcsBlk   = pstDataBuf->u32ProcesPtr;
    u32WriteBlk   = pstDataBuf->u32WritePtr;
    s32Ret = DMX_SwReadDataRequset(u32ChId, 1, &u32AcqedNum, psMsgList);
   
    pstDataBuf->u32ProcesOffset = u32Offset;//restore the pointers
    pstDataBuf->u32ProcesPtr    = u32ProcsBlk;
    pstDataBuf->u32WritePtr     = u32WriteBlk;
    UNLOCKSWCHN(u32ChId);
    
    return HI_FAILURE;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

