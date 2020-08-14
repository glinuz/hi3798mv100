/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCO4ir2/uQ7BFDYEYESXdzFFpPTRP1CkEuTQUs7
G00Y4THS1N/e7mLki9J8vJrKcAXLW5Cb+2sp83PUWkSNg7/2c0smiyVPp3hE9f8I0MZu+gJw
OMCO7eqYiu4kaxT9n2nriEacOZ91iuLWXmFTMykom1uCA+yj+4LTbFUP+dJTHQ==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef __KERNEL__
#include <string.h>
#include <math.h>
#endif

#include "iMedia_common.h"
#include "iMedia_viddec.h"
#include "iMedia_error.h"
#include "iMedia_util.h"

#undef DECLARE_ALIGNED
#undef DECLARE_ALIGNED_8
#undef DECLARE_ALIGNED_16

#include "probe_header.h"
#include "Bitstream.h"
#include "golomb.h"

/*用来mpeg2解析头部信息 xiongfei20100302添加*/
#include "avcodec.h"
#include "mpeg12.h"
#include "h264.h"
#include "vc1.h"

/*x00141957 20100629*/
#include "vp56.h"

#define MAX_POC_CYCLE 256
#define MAX_REF_FRAME_NUM 16
#define MB_WIDTH  16
#define MB_HEIGHT 16
#define MAX_SPS_COUNT 32
#define MAX_PPS_COUNT 256
#define RETURN_OK  0
#define RETURN_ERR 1

#define FOUND_HEADER 1
#define FOUND_START 0
#define NOT_FOUND -1

#define  NEED_LENGTH 256
#define  IVIDEO_LEVEL_H264_9 9

#define MPEG4_HEADER_PREFIX_LEN 4
#define MPEG4_VOS_LEN 5
#define VISOBJSEQ_START_CODE 0x000001B0  //visual_object_sequence_start_code
#define VIDOBJLAY_START_CODE_MASK 0x00000120

#define  PSC_LENGTH_IN_BITS 22
#define  PSC_LENGTH_IN_BYTES 3

#define HEADER_PREFIX 3

static INT32 VP6_Decodeheader(UINT8 *pucSrcBuf, INT32 iLength, STRU_IVIDEO_STREAM_INFO *pstParams);
static INT32 VP6A_Decodeheader(UINT8 *pucSrcBuf, INT32 iLength, STRU_IVIDEO_STREAM_INFO *pstParams);
#if 0
static INT32 MPEG4_CHECK_PROFILE(STRU_IVIDEO_STREAM_INFO *pstStream);

static INT32 H264_Decode_RBSP_TRAILING(UINT8 *puSrc, UINT32 *piBitNum);
static INT32 H264_DECODE_NAL(const UINT8 *pSrc, UINT8 *pDst, INT32 *piDstLength, INT32 iLength);
static INT32 H264_DECODE_SPS(GetBitContext *pstBitContext, STRU_IVIDEO_STREAM_INFO *pstParams);
static INT32 H264_PARSE_SPS(UINT8 *pucSrcBuf, INT32 iLength, STRU_IVIDEO_STREAM_INFO *pstParams);
static INT32 H264_PARSE_PPS(UINT8 *pucSrcBuf, INT32 iLength, STRU_IVIDEO_STREAM_INFO *pstParams);
static INT32 H264_DECODE_PPS(GetBitContext *pstBitContext, STRU_IVIDEO_STREAM_INFO *pstParams);
static INT32 H264_GET_SODB(UINT8 *pucSrcBuf, INT32 iSrcLength, UINT8 *pucDstBuf, GetBitContext *pstBitContext);

extern int decode_vol_header(MpegEncContext *s, GetBitContext *gb);
extern int decode_sequence_header(AVCodecContext *avctx, GetBitContext *gb);
extern /*av_always_inline*/ int vc1_unescape_buffer(const uint8_t *src, int size, uint8_t *dst);


/* H264_ProbeHeader()
   There are some problem left:
   1. sps and pps match
   2. sps and pps correction */
INT32 H264_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 iRet = RETURN_OK;
    INT32 iStatus = *piStatus;

    UINT32 uiBufIndex = 0;
    UINT8 *pucSpsStartAddr = NULL;
    UINT8 *pucPpsStartAddr = NULL;
    UINT8 *pucSpsEndAddr = NULL;
    UINT8 *pucPpsEndAddr = NULL;
    INT32 iNaluType;
    INT32 iSpsFlag = NOT_FOUND;
    INT32 iPpsFlag = NOT_FOUND;

    // 保证至少剩余3个字节
    if (HEADER_PREFIX < uiNumBytes)
    {
        uiNumBytes -= HEADER_PREFIX;
    }
    else
    {
        *puiBytesConsumed = uiNumBytes;
        return IMEDIA_RET_SUCCESS;
    }

    if (META_DATA_FOUND_ONLY_SPS == *piStatus)
    {
        pucSpsStartAddr = pucSpsEndAddr = (UINT8 *)1;
    }
    else if (META_DATA_FOUND_ONLY_PPS == *piStatus)
    {
        pucPpsStartAddr = pucPpsEndAddr = (UINT8 *)1;
    }

    while (META_DATA_FOUND != iStatus)
    {
        while (uiBufIndex < uiNumBytes)
        {
            for (; uiBufIndex < uiNumBytes; uiBufIndex++)
            {
                /* find the start code: 0x000001 */
                if (pucBuf[uiBufIndex] == 0 && pucBuf[uiBufIndex + 1] == 0 && pucBuf[uiBufIndex + 2] == 1)
                {
                    break;
                }
            }

            if (uiBufIndex >= uiNumBytes)
            {
                break;
            }

            if (FOUND_START == iSpsFlag)
            {
                iSpsFlag = FOUND_HEADER;
            }

            if (FOUND_START == iPpsFlag)
            {
                iPpsFlag = FOUND_HEADER;
            }

            iNaluType = pucBuf[uiBufIndex + 3] & 0x1f;

            switch (iNaluType)
            {
                case H264_NAL_SPS:
                {
                    /* set the sps start address and start find flag */
                    if (NULL == pucSpsEndAddr && NULL == pucSpsStartAddr)
                    {
                        pucSpsStartAddr = pucBuf + uiBufIndex ;
                        iSpsFlag = FOUND_START;
                    }
                }
                break;

                case H264_NAL_PPS:
                {
                    /* set the pps start address and start find flag */
                    if ( NULL == pucPpsEndAddr && NULL == pucPpsStartAddr)
                    {
                        pucPpsStartAddr = pucBuf + uiBufIndex;
                        iPpsFlag = FOUND_START;
                    }
                }
                break;

                default:
                    break;
            }

            if (NULL != pucSpsStartAddr && FOUND_HEADER == iSpsFlag && NULL == pucSpsEndAddr)
            {
                pucSpsEndAddr = pucBuf + uiBufIndex;
            }

            if (NULL != pucPpsStartAddr && FOUND_HEADER == iPpsFlag && NULL == pucPpsEndAddr)
            {
                pucPpsEndAddr = pucBuf + uiBufIndex;
            }

            /* 2010/06/17 16:30:00 liuxw+00139685 */
            /* 如果是两个连续的sps或连续的pps，而且是第二个的时候，就先不跳过起始码 */
            /* skip the start code */
            if (!((FOUND_HEADER == iSpsFlag && H264_NAL_SPS == iNaluType) || (FOUND_HEADER == iPpsFlag &&  H264_NAL_PPS == iNaluType)))
            {
                uiBufIndex += 3;
            }

            /* if find the sps's or pps's start and end , then go out of the loop */
            if (FOUND_HEADER == iSpsFlag || FOUND_HEADER == iPpsFlag)
            {
                break;
            }
        }

        /* if reached the end of the buf and just find the start of sps , then check the last three byes. if they are  \
        start code ,then find the whole sps;if not , Consuemd bytes just include the bytes before sps start(not include) */
        /*lint -e613*/
        if (FOUND_HEADER == iSpsFlag && META_DATA_FOUND_ONLY_SPS != iStatus)
        {
            // found SPS
            UINT32 uiSpsLength = pucSpsEndAddr - pucSpsStartAddr;
            iRet = H264_PARSE_SPS(pucSpsStartAddr, uiSpsLength, pstParams);

            if (RETURN_OK != iRet)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "Parse the sps failed!\n");
                iSpsFlag = NOT_FOUND;
                pucSpsStartAddr = NULL;
                pucSpsEndAddr = NULL;
            }
            else
            {
                pstMetaData->iNumMetaData++;
                pstMetaData->apucMetaData[0]   = pucSpsStartAddr;
                pstMetaData->ausMetaDataLen[0] = uiSpsLength;
                iStatus = (META_DATA_FOUND_ONLY_PPS == iStatus) ? META_DATA_FOUND : META_DATA_FOUND_ONLY_SPS;
                iSpsFlag = NOT_FOUND;
            }
        }

        if (FOUND_HEADER == iPpsFlag && META_DATA_FOUND_ONLY_PPS != iStatus && META_DATA_FOUND != iStatus)
        {
            // found SPS
            UINT32 uiPpsLength = pucPpsEndAddr - pucPpsStartAddr;
            iRet = H264_PARSE_PPS(pucPpsStartAddr, uiPpsLength, pstParams);

            if (RETURN_OK != iRet)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "Parse the pps failed!\n");
                iPpsFlag = NOT_FOUND;
                pucPpsStartAddr = NULL;
                pucPpsEndAddr = NULL;
            }
            else
            {
                pstMetaData->iNumMetaData++;
                pstMetaData->apucMetaData[1]   = pucPpsStartAddr;
                pstMetaData->ausMetaDataLen[1] = uiPpsLength;
                iStatus = (META_DATA_FOUND_ONLY_SPS == iStatus) ? META_DATA_FOUND : META_DATA_FOUND_ONLY_PPS;
                iPpsFlag = NOT_FOUND;
            }
        }

        /*lint +e613*/
        if (uiBufIndex >= uiNumBytes)
        {
            break;
        }
    }

    if (META_DATA_FOUND != iStatus)
    {
        // only found sps start address
        if (pucSpsStartAddr && NULL == pucSpsEndAddr)
        {
            uiBufIndex = pucSpsStartAddr - pucBuf;
        }
        // only found pps start address
        else if (pucPpsStartAddr && NULL == pucPpsEndAddr)
        {
            uiBufIndex = pucPpsStartAddr - pucBuf;
        }
    }

    *puiBytesConsumed = uiBufIndex;
    *piStatus = iStatus;

    return IMEDIA_RET_SUCCESS;
}


#define HEADER_BUF_SIZE 1024
/* h264 decode sps and store some information in the pstParams */
static INT32 H264_PARSE_SPS(UINT8 *pucSrcBuf, INT32 iLength, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 iRet = RETURN_OK;

    GetBitContext stBitContext;
    UINT8 aucSPSBuf[HEADER_BUF_SIZE] = {0};

    if (HEADER_BUF_SIZE < iLength || 0 >= iLength)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Length[%d] is out of range[1-%d]!\n", iLength, HEADER_BUF_SIZE);
        return RETURN_ERR;
    }

    iRet = H264_GET_SODB(pucSrcBuf, iLength, aucSPSBuf, &stBitContext);

    if (RETURN_OK != iRet)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "H264_GET_SODB failed!\n");
        return iRet;
    }

    /* decode the sps */
    iRet = H264_DECODE_SPS(&stBitContext, pstParams);

    if (RETURN_OK != iRet)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "H264_DECODE_SPS failed!\n");
        memset(pstParams, 0, sizeof(*pstParams));
    }

    return iRet;
}


/* h264 decode sps and store some infomation in the pstParams */
static INT32 H264_PARSE_PPS(UINT8 *pucSrcBuf, INT32 iLength, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 iRet = RETURN_OK;

    GetBitContext stBitContext;
    UINT8 aucPPSBuf[HEADER_BUF_SIZE] = {0};

    if (HEADER_BUF_SIZE < iLength || 0 >= iLength)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "the bufsize is not enough[%d] for store sps bitstream[%d]!\n", HEADER_BUF_SIZE, iLength);
        return RETURN_ERR;
    }

    iRet = H264_GET_SODB(pucSrcBuf, iLength, aucPPSBuf, &stBitContext);

    if (RETURN_OK != iRet)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "H264_GET_SODB failed!\n");
        return RETURN_ERR;
    }

    /* decode the sps */
    iRet = H264_DECODE_PPS(&stBitContext, pstParams);

    if (RETURN_OK != iRet)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "H264_DECODE_PPS failed!\n");
        return RETURN_ERR;
    }

    return RETURN_OK;
}

static INT32 H264_GET_SODB(UINT8 *pucSrcBuf, INT32 iSrcLength, UINT8 *pucDstBuf, GetBitContext *pstBitContext)
{
    INT32 iRet = RETURN_OK;

    UINT32 uiBitLength = 0;
    INT32 iDstLength = 0;

    /* check the validation of params */
    if (NULL == pucSrcBuf || 0 == iSrcLength)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "input buf is NULL or iLength is zero!\n");
        return RETURN_ERR;
    }

    /* check the validation of output buf */
    if (NULL == pucDstBuf )
    {
        IMEDIA_LOG(IMEDIA_WARNING, "output buf is NULL!\n");
        return RETURN_ERR;
    }

    /* check the validation of params */
    if (NULL == pstBitContext)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Bit context pointer is NULL!\n");
        return RETURN_ERR;
    }

    /* convert EBSP to RBSP */
    iRet = H264_DECODE_NAL(pucSrcBuf + 3, pucDstBuf, &iDstLength, iSrcLength - 3);

    if (RETURN_OK != iRet)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "H264_DECODE_NAL failed!\n");
        return iRet;
    }

    /* delete the last zero bytes */
    while (0 < iDstLength && 0 == pucDstBuf[iDstLength - 1])
    {
        iDstLength--;
    }

    /* convert RBSP to SODB */
    iRet = H264_Decode_RBSP_TRAILING(pucDstBuf + iDstLength - 1 , &uiBitLength);

    if (RETURN_OK != iRet)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "H264_Decode_RBSP_TRAILING failed!\n");
        return iRet;
    }

    /* Calculate the total bit-length of SODB */
    uiBitLength = !iDstLength ? 0 : (8 * iDstLength - uiBitLength);
    /* init the stBitContext: contain the bit consumed, buf pointer and so on */
    init_get_bits(pstBitContext, pucDstBuf, uiBitLength);

    return RETURN_OK;
}


static INT32 H264_DECODE_PPS(GetBitContext *pstBitContext, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    UINT32 uiPpsID;
    UINT32 uiSpsId;
    UINT32 uiEntropy;
    UINT32 uiSliceGroup;
    UINT32 uiRefCount[2];
    INT32 iBitLength = 0;
    INT32 iTemp = 0;
    INT32 iTemp2 = 0;

    if (NULL == pstBitContext)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Input BitContext is NULL!\n");
        return RETURN_ERR;
    }

    if (NULL == pstParams)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "output params pointer is NULL !\n");
        return RETURN_ERR;
    }

    iBitLength = pstBitContext->size_in_bits;

    uiPpsID = get_ue_golomb(pstBitContext);

    if (MAX_PPS_COUNT <= uiPpsID)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "PPS_ID[%u] is out of range[0-255]!\n", uiPpsID);
        //      return RETURN_ERR;
    }

    uiSpsId = get_ue_golomb_31(pstBitContext);

    if (MAX_SPS_COUNT <= uiSpsId)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "SPS_ID[%u] is out of range[0-31]!\n", uiSpsId);
        //      return RETURN_ERR;
    }

    uiEntropy = get_bits1(pstBitContext);

    get_bits1(pstBitContext); //pic_order_present
    uiSliceGroup = get_ue_golomb(pstBitContext) + 1; //slice_group_count

    if (1 != uiSliceGroup)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Do not support FMO currently!\n");
        //      return RETURN_ERR;
    }

    uiRefCount[0] = get_ue_golomb(pstBitContext) + 1;
    uiRefCount[1] = get_ue_golomb(pstBitContext) + 1;

    if (32 < uiRefCount[0] || 32 < uiRefCount[1])
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Reference Picture Count[%u:%u] is out of range[0-31]!\n", uiRefCount[0], uiRefCount[1]);
        //      return RETURN_ERR;
    }

    iTemp  = get_bits1(pstBitContext); //weighted_pred
    iTemp2 = get_bits(pstBitContext, 2); //weighted_bipred_idc

    if ((IVIDEO_PROFILE_H264_BASELINE == pstParams->eProfile && (iTemp != 0 || iTemp2 != 0)) || 3 == iTemp2)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Weight(P/B) is invalid!\n");
        //      return RETURN_ERR;
    }

    iTemp  = get_se_golomb(pstBitContext) /*+ 26*/; //init_qp
    iTemp2 = get_se_golomb(pstBitContext) /*+ 26*/; //init_qs

    if (-26 > iTemp || 26 <= iTemp || -26 > iTemp2 || 26 <= iTemp2)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Init_qp[%d]/Init_qs[%d] is invalid!\n", iTemp, iTemp2);
        //      return RETURN_ERR;
    }

    iTemp = get_se_golomb(pstBitContext);//chroma_qp_index_offset[0]

    if (-12 > iTemp || 12 < iTemp)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "chroma_qp_index_offset[%d] is invalid!\n", iTemp);
        //      return RETURN_ERR;
    }

    get_bits1(pstBitContext); //deblocking_filter_parameters_present
    get_bits1(pstBitContext);//constrained_intra_pred
    get_bits1(pstBitContext);//redundant_pic_cnt_present

    if (get_bits_count(pstBitContext) < iBitLength)
    {
        iTemp = get_bits1(pstBitContext); //transform_8x8_mode
        //        iTemp = get_bits1(pstBitContext);  // pic_scaling_matrix_present_flag
        {
            H264Context *h = NULL;
            SPS sps;
            PPS pps;
            uint8_t scaling_matrix4[6][16];
            uint8_t scaling_matrix8[2][64];

            // sizeof(H264Context) = 286KB，为了节省栈空间改为在堆中分配
            h = IMedia_Malloc(sizeof(H264Context));

            if (NULL == h)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "Malloc memory for H264Context failed!\n");
                return RETURN_ERR;
            }

            h->s.gb = *pstBitContext;
            pps.transform_8x8_mode = iTemp;
            decode_scaling_matrices(h, &sps, &pps, 0, scaling_matrix4, scaling_matrix8);
            *pstBitContext = h->s.gb;

            if (NULL != h)
            {
                IMedia_Free(h);
                h = NULL;
            }
        }

        iTemp  = get_se_golomb(pstBitContext); //second_chroma_qp_index_offset

        if (-12 > iTemp || 12 < iTemp)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "chroma_qp_index_offset[%d] is invalid!\n", iTemp);
            //          return RETURN_ERR;
        }
    }

    IMEDIA_LOG(IMEDIA_DEBUG, "sps:%u pps:%u entropy:%u ref0:%u ref1:%u\n", uiSpsId, uiPpsID, uiEntropy, uiRefCount[0], uiRefCount[1]);

    return RETURN_OK;
}

/* conver RBSP to SODB */
static INT32 H264_Decode_RBSP_TRAILING(UINT8 *pucSrc, UINT32 *piBitNum)
{
    INT32 iRet = RETURN_OK;

    UINT8 ucTempValue;
    UINT32 iIndex = 0;

    /* check the validation of the params */
    if (NULL == pucSrc)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "input buf is NULL!\n");
        return RETURN_ERR;
    }

    if (NULL == piBitNum)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "output pointer is NULL!\n");
        return RETURN_ERR;
    }

    ucTempValue = *pucSrc;

    /* find the first value that is 1 from the left to right of the last byte(ucTempValue) */
    for (iIndex = 1; iIndex < 9; iIndex++)
    {
        if (ucTempValue & 1)
        {
            *piBitNum = iIndex;
            break;
        }

        ucTempValue >>= 1;
    }

    return iRet;
}

#define MIN_SPS_SIZE 4
/* Convert from EBSP TO RBSP */
static INT32 H264_DECODE_NAL(const UINT8 *pucSrc, UINT8 *pucDst, INT32 *piDstLength, INT32 iLength)
{
    INT32 iRet = RETURN_OK;

    INT32 i, si, di;

    /* check the validation of the params */
    if (NULL == pucSrc)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "input buf is null!\n");
        return RETURN_ERR;
    }

    if (NULL == pucDst)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "output pointer is null!\n");
        return RETURN_ERR;
    }

    if (MIN_SPS_SIZE > iLength)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "the length[%d] of input bitstream is invalid!\n", iLength);
        return RETURN_ERR;
    }

    if (NULL == piDstLength)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "The dst size pointer is NULL!\n");
        return RETURN_ERR;
    }

    /* skip the nal byte */
    pucSrc++;
    iLength--;

    /* check if there is emulation_prevention_three_byte */
    for (i = 0; i + 1 < iLength; i += 2)
    {
        if (pucSrc[i])
        {
            continue;
        }

        if (0 < i && 0 == pucSrc[i - 1])
        {
            i--;
        }

        if (i + 2 < iLength && 0 == pucSrc[i + 1] && 3 >= pucSrc[i + 2])
        {
            /* startcode, so we must be past the end */
            if (3 != pucSrc[i + 2])
            {
                iLength = i;
            }

            break;
        }
    }

    /* if no emulation_prevention_three_byte ,return */
    if (i >= iLength - 1)
    {
        *piDstLength = iLength;
        memcpy(pucDst, pucSrc, iLength);
        return iRet;
    }

    /* copy the bytes before the first emulation_prevention_three_byte from src to dst */
    memcpy(pucDst, pucSrc, i);
    si = di = i;

    /* remove the emulation_prevention_three_byte */
    while (si + 2 < iLength)
    {
        /* if is not emulation_prevention_three_byte, just copy it */
        if (3 < pucSrc[si + 2])
        {
            pucDst[di++] = pucSrc[si++];
            pucDst[di++] = pucSrc[si++];
        }
        else if (0 == pucSrc[si] && 0 == pucSrc[si + 1])
        {
            /* if is emulation_prevention_three_byte, remove it */
            if (3 == pucSrc[si + 2])
            {
                pucDst[di++] = 0;
                pucDst[di++] = 0;
                si += 3;
                continue;
            }
            /* if is start code, return */
            else
            {
                goto NSC;
            }
        }

        pucDst[di++] = pucSrc[si++];
    }

    /* copy the last two bytes */
    while (si < iLength)
    {
        pucDst[di++] = pucSrc[si++];
    }

NSC:
    *piDstLength = di;

    return iRet;
}

#define MAX_H264_CHROMA_FORMAT 3
#define MIN_H264_CHROMA_FORMAT 0

static INT32 H264_DECODE_SPS(GetBitContext *pstBitContext, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 iRet = RETURN_OK;

    UINT32 uiSpsId;
    INT32 iChromaFormat = -1;
    INT32 iPocType = -1;
    INT32 uiPocCycleLength;
    UINT16 usMbWidth, usMbHeight;
    INT32 iFrameMbOnlyFlag;
    INT32 iCropFlag, iCropRight, iCropTop, iCropLeft, iCropBottom;
    //INT32  iSeqScalingMatrixPresentFlag = 0;
    INT32 aiSubWidthC[]  = {1, 2, 2, 1};
    INT32 aiSubHeightC[] = {1, 2, 1, 1};
    INT32 i;
    /* change color format from codec to interface layer */
    INT32 aiColorFmt[] = {IVIDEO_CSP_MONOCHROME, IVIDEO_CSP_YUV420, IVIDEO_CSP_YUV422, IVIDEO_CSP_YUV444};
    INT32 iTemp = 0;

    H264Context *h = NULL;
    SPS sps;

    /* check the validation of the params */
    if (NULL == pstBitContext)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Input params pointer is NULL !\n");
        return RETURN_ERR;
    }

    if (NULL == pstParams)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Output params pointer is NULL !\n");
        return RETURN_ERR;
    }

    // sizeof(H264Context) = 286KB，为了节省栈空间改为在堆中分配
    h = IMedia_Malloc(sizeof(H264Context));

    if (NULL == h)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Malloc memory for H264Context failed!\n");
        return RETURN_ERR;
    }

    /* get the Profile */
    pstParams->eProfile = (ENUM_IVIDEO_CODEC_PROFILE)get_bits(pstBitContext, 8);

    /* check the validation of the Profile: support Baseline,Main and High Profile currently */
    if (!(IVIDEO_PROFILE_H264_BASELINE == pstParams->eProfile || IVIDEO_PROFILE_H264_MAIN == pstParams->eProfile || IVIDEO_PROFILE_H264_HIGH == pstParams->eProfile))
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Unsupported Profile[%d]!\n", pstParams->eProfile);
        iRet = RETURN_ERR;
        goto PARSE_END;
    }

    get_bits1(pstBitContext);   //constraint_set0_flag
    get_bits1(pstBitContext);   //constraint_set1_flag
    get_bits1(pstBitContext);   //constraint_set2_flag
    get_bits1(pstBitContext);   //constraint_set3_flag
    get_bits(pstBitContext, 4); // reserved

    /* get the Level */
    pstParams->eLevel = (ENUM_IVIDEO_CODEC_LEVEL)get_bits(pstBitContext, 8);

    switch (pstParams->eLevel )
    {
        case IVIDEO_LEVEL_H264_10:
        case IVIDEO_LEVEL_H264_11:
        case IVIDEO_LEVEL_H264_12:
        case IVIDEO_LEVEL_H264_13:
        case IVIDEO_LEVEL_H264_20:
        case IVIDEO_LEVEL_H264_21:
        case IVIDEO_LEVEL_H264_22:
        case IVIDEO_LEVEL_H264_30:
        case IVIDEO_LEVEL_H264_31:
        case IVIDEO_LEVEL_H264_32:
        case IVIDEO_LEVEL_H264_40:
        case IVIDEO_LEVEL_H264_41:
        case IVIDEO_LEVEL_H264_42:
        case IVIDEO_LEVEL_H264_50:
        case IVIDEO_LEVEL_H264_51:
            break;

        default:
        {
            if (IVIDEO_PROFILE_H264_HIGH == pstParams->eProfile && IVIDEO_LEVEL_H264_9 == pstParams->eLevel)
            {
                break;
            }

            IMEDIA_LOG(IMEDIA_WARNING, "Unsupported Level[%d]!\n", pstParams->eLevel);
            //return RETURN_ERR;
        }
    }

    uiSpsId = get_ue_golomb_31(pstBitContext);

    if (MAX_SPS_COUNT <= uiSpsId )
    {
        IMEDIA_LOG(IMEDIA_WARNING, "The SPS id[%d] is exceed the Max[%d]!\n", uiSpsId, MAX_SPS_COUNT);
        //         return RETURN_ERR;
    }

    if (IVIDEO_PROFILE_H264_HIGH <= pstParams->eProfile)
    {
        iChromaFormat = get_ue_golomb_31(pstBitContext);

        /* transform the chroma format to the color space */
        if (MIN_H264_CHROMA_FORMAT > iChromaFormat || MAX_H264_CHROMA_FORMAT < iChromaFormat)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "Invalid chroma format[%d]!\n", iChromaFormat);
            iRet = RETURN_ERR;
            goto PARSE_END;
        }

        /* check the validation of picture colorspace: support YUV420 currently */
        switch (aiColorFmt[iChromaFormat])
        {
            case IVIDEO_CSP_YUV420:
                break;

            default:
            {
                IMEDIA_LOG(IMEDIA_WARNING, "Unsupported colour space[%d]!\n", iChromaFormat);
                iRet = RETURN_ERR;
                goto PARSE_END;
            }
        }

        if (3 == iChromaFormat)
        {
            get_bits1(pstBitContext); //sps->residual_color_transform_flag
        }

        iTemp = get_ue_golomb(pstBitContext) ; //bit_depth_luma_minus8

        if (0 != iTemp)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "bit_depth_luma_minus8[%d] is invalid or not supported!\n", iTemp);
            //          return RETURN_ERR;
        }

        iTemp = get_ue_golomb(pstBitContext) ; //bit_depth_chroma_minus8

        if (0 != iTemp)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "bit_depth_chroma_minus8[%d] is invalid or not supported!\n", iTemp);
            //          return RETURN_ERR;
        }

        get_bits1(pstBitContext);      //transform_bypass

        //      iSeqScalingMatrixPresentFlag = get_bits1(pstBitContext);
        {
            uint8_t scaling_matrix4[6][16];
            uint8_t scaling_matrix8[2][64];

            h->s.gb = *pstBitContext;
            decode_scaling_matrices(h, &sps, NULL, 1, scaling_matrix4, scaling_matrix8);
            *pstBitContext = h->s.gb;
        }
    }
    else
    {
        iChromaFormat = 1;
    }

    iTemp = get_ue_golomb(pstBitContext) ;  /* log2_max_frame_num_minux4 */

    if (12 < iTemp)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "log2_max_frame_num_minus4[%d] is invalid!\n", iTemp);
        //      return RETURN_ERR;
    }

    iPocType = get_ue_golomb_31(pstBitContext);  /* poc_type */

    switch (iPocType)
    {
        case 0:
        {
            iTemp = get_ue_golomb(pstBitContext);  //log2_max_poc_lsb_minus4

            if (12 < iTemp)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "log2_max_poc_lsb_minus4[%d] is invalid!\n", iTemp);
                //              return RETURN_ERR;
            }
        }
        break;

        case 1:
        {
            get_bits1(pstBitContext);      //delta_pic_order_always_zero_flag
            get_se_golomb(pstBitContext);  //offset_for_non_ref_pic
            get_se_golomb(pstBitContext);  //offset_for_top_to_bottom_field
            uiPocCycleLength = get_ue_golomb(pstBitContext);  //poc_cycle_length

            if (MAX_POC_CYCLE <= uiPocCycleLength)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "poc_cycle_length[%d] exceed MAX_POC_CYCLE[%d]!\n", uiPocCycleLength, MAX_POC_CYCLE);
                //              return RETURN_ERR;
                /* 2010/06/17 8:30:00 liuxw+00139685 */
                /* 若poc cycle length不合法，则进行修正 */
                uiPocCycleLength = MAX_POC_CYCLE - 1;     //TI修正为其自身的值(相当于没有修正),根据ffmpeg的方式进行修正
            }

            for (i = 0; i < uiPocCycleLength; i++)
            {
                get_se_golomb(pstBitContext);  //offset_for_ref_frame
            }
        }
        break;

        case 2:
            break;

        default:
        {
            IMEDIA_LOG(IMEDIA_WARNING, "Invalid POC type[%d]!\n", iPocType);
            iRet = RETURN_ERR;
            goto PARSE_END;
        }
    }

    /* get the max reference frame num */
    pstParams->uiRefFrameNum = (UINT32)get_ue_golomb_31(pstBitContext);

    if (MAX_REF_FRAME_NUM < pstParams->uiRefFrameNum)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Ref frame num[%d] exceed the Max[%d]!\n", pstParams->uiRefFrameNum, MAX_REF_FRAME_NUM);
        //      return RETURN_ERR;
    }

    get_bits1(pstBitContext);  /* gaps_in_frame_num_allowed_flag */
    usMbWidth  = get_ue_golomb(pstBitContext) + 1;  /* get the width of picture(unit:MBWidth) */
    usMbHeight = get_ue_golomb(pstBitContext) + 1;  /* get the height of picture(unit:MBHeight) */

    iFrameMbOnlyFlag = get_bits1(pstBitContext);

    if (0 == iFrameMbOnlyFlag)
    {
        get_bits1(pstBitContext);  /* sps->mb_aff */
    }
    else
    {
        pstParams->eContentType = IVIDEO_PROGRESSIVE;
    }

    iTemp = get_bits1(pstBitContext);  /*direct_8x8_inference_flag */

    if (0 == iFrameMbOnlyFlag && 0 == iTemp)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "direct_8x8_inference_flag should be equal to 1 when iFrameMbOnlyFlag equal to 0!\n");
        //      return RETURN_ERR;
    }

    /* get the picture crop flag */
    iCropFlag = get_bits1(pstBitContext);

    if (1 == iCropFlag)
    {
        iCropLeft   = get_ue_golomb(pstBitContext);
        iCropRight  = get_ue_golomb(pstBitContext);
        iCropTop    = get_ue_golomb(pstBitContext);
        iCropBottom = get_ue_golomb(pstBitContext);
    }
    else
    {
        iCropLeft   = 0;
        iCropRight  = 0;
        iCropTop    = 0;
        iCropBottom = 0;
    }

    pstParams->eColorSpaceType = (ENUM_IVIDEO_COLOR_SPACE_TYPE)aiColorFmt[iChromaFormat];

    /* calculate the picture width and height(unit: pixel) */
    pstParams->usWidth  = usMbWidth * MB_WIDTH;
    pstParams->usHeight = usMbHeight  * (2 - iFrameMbOnlyFlag) * MB_HEIGHT;

    /* get the really crop */
    iCropLeft     = aiSubWidthC[iChromaFormat] * iCropLeft;
    iCropRight    = aiSubWidthC[iChromaFormat] * iCropRight;
    iCropTop      = aiSubHeightC[iChromaFormat] * (2 - iFrameMbOnlyFlag) * iCropTop;
    iCropBottom   = aiSubHeightC[iChromaFormat] * (2 - iFrameMbOnlyFlag) * iCropBottom;

    if (1 == iCropFlag)
    {
        if ((iCropLeft + iCropRight) >= pstParams->usWidth || (iCropTop + iCropBottom) >= pstParams->usHeight)
        {
            IMEDIA_LOG(IMEDIA_WARNING, "The crop[%d(CropLeft+CropRight)>=%d(Width) or %d(CropTod+CropBottom)>=%d(Height)] is out of range!\n");
            iRet = RETURN_ERR;
            goto PARSE_END;
        }

        IMEDIA_LOG(IMEDIA_INFO, "Width: %d Height: %d before crop! leftcrop = %d rightcrop = %d topcrop = %d \
							   bottom crop = %d!\n", pstParams->usWidth, pstParams->usHeight, iCropLeft, iCropRight, iCropTop, iCropBottom);
    }

    /* calculate the source(after crop) picture width and height */
    pstParams->usWidth  -= (iCropLeft + iCropRight);
    pstParams->usHeight -= (iCropTop + iCropBottom);

    /* check the validation of picture width */
    if (IVIDEO_MAX_WIDTH < pstParams->usWidth || IVIDEO_MIN_WIDTH > pstParams->usWidth)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Invalid picture width[%d]!\n", pstParams->usWidth);
        iRet = RETURN_ERR;
        goto PARSE_END;
    }

    /* check the validation of picture height */
    if (IVIDEO_MAX_HEIGHT < pstParams->usHeight || IVIDEO_MIN_HEIGHT > pstParams->usHeight)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Invalid picture height[%d]!\n", pstParams->usHeight);
        iRet = RETURN_ERR;
        goto PARSE_END;
    }

    IMEDIA_LOG(IMEDIA_INFO, "The picture source width: %u source height: %u!\n", pstParams->usWidth, pstParams->usHeight);

    memset(h, 0, sizeof(H264Context));
    memset(&sps, 0, sizeof(sps));
    sps.sar.num = 1;
    sps.sar.den = 1;
    sps.vui_parameters_present_flag = get_bits1(pstBitContext);
    h->s.gb = *pstBitContext;

    if (sps.vui_parameters_present_flag)
    {
        decode_vui_parameters(h, &sps);
    }

    pstParams->stAspectRatio.usSarWidth  = sps.sar.num;
    pstParams->stAspectRatio.usSarHeight = sps.sar.den;

PARSE_END:

    if (NULL != h)
    {
        IMedia_Free(h);
        h = NULL;
    }

    return iRet;
}

static INT32 MPEG4_CHECK_PROFILE(STRU_IVIDEO_STREAM_INFO *pstStream)
{

    switch (pstStream->eProfile)
    {
        case IVIDEO_PROFILE_MPEG4_SP:
        {
            if ((pstStream->eLevel >= IVIDEO_LEVEL_MPEG4_0) && (pstStream->eLevel < IVIDEO_LEVEL_MPEG4_4))
            { break; }
        }

        /* else fall through */
        case IVIDEO_PROFILE_MPEG4_MP:
        {
            if ((pstStream->eLevel > IVIDEO_LEVEL_MPEG4_1) && (pstStream->eLevel < IVIDEO_LEVEL_MPEG4_5))
            { break; }
        }

        /* else fall through */
        case IVIDEO_PROFILE_MPEG4_ASP:
        {
            if (pstStream->eLevel <= IVIDEO_LEVEL_MPEG4_3b)
            { break; }
        }

        /* else fall through */
        default:
        {
            IMEDIA_LOG(IMEDIA_WARNING, "Unsupported MPEG4 Profile[%d] or Level[%d]!\n", pstStream->eProfile, pstStream->eLevel);
        }
    }

    return RETURN_OK;
}

/*
此函数用来寻找mpeg2码流中的seq header，如果有扩展头也一并寻找
若找到则返回header的地址和长度
若未找到则在piStatus中返回错误，并告诉上层
xiongfei 20100302
*/
INT32 MPEG2_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    //  INT32 iRet = RETURN_OK;

    /*在所读入的字节数中寻找序列头 ,可能没有gop头xiongfei20100210*/
    {
#define FIND_MPEG2_VOS 1
#define FIND_MPEG2_VOP 1
#define MPEG2_MAX_HEADER_LENGTH 800
        /*定义一些临时结构体用以解析头部信息 xiongfei20100302*/
        Mpeg1Context s1 ;

        AVCodecContext   *avctx = NULL;

        const UINT8 *pucBufPos = pucBuf;
        const UINT8 *pucBufEnd = pucBuf + uiNumBytes;
        INT32 iInputSize;
        INT32 iFindVosFlag = 0;
        INT32 iFindVopFlag = 0;
        UINT32 uiStartCode = (UINT32) - 1;

        /*根据c68632检视意见进行修改,当uiNumBytes<3时*puiBytesConsumed会反转 x00141957 20100418*/
        if (uiNumBytes <= PSC_LENGTH_IN_BYTES)
        {
            *puiBytesConsumed = uiNumBytes;
            return IMEDIA_RET_SUCCESS;
        }

        avctx = avcodec_alloc_context();

        if (NULL == avctx)
        {
            // 如果在当前缓冲区内无法找到起始码 爆warning xiongfei20100302
            IMEDIA_LOG(IMEDIA_ERROR, "avctx alloc failed!\n");
            return IMEDIA_RET_SUCCESS;//?
        }

        /*初始化解析头部所用临时结构体 xiongfei20100302*/
        avctx->priv_data = &s1;
        s1.mpeg_enc_ctx.avctx = avctx;

        /*x00141957 20100726*/
        s1.mpeg_enc_ctx.iDisplayHeight = 0;
        s1.mpeg_enc_ctx.iDisplayWeight = 0;

        iInputSize = uiNumBytes;

        while ((FIND_MPEG2_VOP != iFindVopFlag) || (FIND_MPEG2_VOS != iFindVosFlag) )
        {
            INT32 iDecodeHeaderFlag = 0;
            uiStartCode = (UINT32) - 1;

            pucBufPos = ff_find_start_code(pucBufPos, pucBufEnd, &uiStartCode);

            /*如果找不到头,则返回当前所找到头的起始地址，若没有则返回0*/
            if (uiStartCode > 0x1ff)
            {
                if (FIND_MPEG2_VOS == iFindVosFlag)
                {
                    *puiBytesConsumed = uiNumBytes - (UINT32)(pucBufEnd - pstMetaData->apucMetaData[0]);
                }
                else
                {
                    *puiBytesConsumed = uiNumBytes - 3;
                }

                *piStatus = META_DATA_NOT_FOUND;
                pstMetaData->iNumMetaData = 0;

                // 如果在当前缓冲区内无法找到起始码 爆warning xiongfei20100302
                IMEDIA_LOG(IMEDIA_WARNING, "BITSTREAM BUFFER is FINISHED!\n");
                avcodec_free_context(avctx);
                avctx = NULL;
                return IMEDIA_RET_SUCCESS;//?
            }
            else
            {
                iInputSize = pucBufEnd - pucBufPos;

                /*若找到起始码则根据各起始码类型解码header信息 xiongfei20100210 */
                switch (uiStartCode)
                {
                    case SEQ_START_CODE:
                    {
                        /*为防止当前比特数不够长而导致解码头部出错，做以下处理：当当前所剩下比特数小于1k时，放弃解码头部，跳出去从外部再读数据进来xionigfei20100312    */
                        if ( MPEG2_MAX_HEADER_LENGTH > (uiNumBytes - (UINT32)((UINT32)pucBufPos - 4 - (UINT32)pucBuf)))
                        {
                            *puiBytesConsumed = (UINT32)((UINT32)pucBufPos - 4 - (UINT32)pucBuf);
                            *piStatus = META_DATA_NOT_FOUND;
                            pstMetaData->iNumMetaData = 0;

                            // 如果在当前缓冲区内无法找到起始码 爆warning xiongfei20100302
                            IMEDIA_LOG(IMEDIA_WARNING, "BITSTREAM BUFFER is NOT ENOUGH!\n");
                            avcodec_free_context(avctx);
                            avctx = NULL;
                            return IMEDIA_RET_SUCCESS;//?
                        }

                        iDecodeHeaderFlag = mpeg1_decode_sequence(avctx, pucBufPos, iInputSize);

                        /*已经找到vos头  xiongfei20100210*/
                        if (iDecodeHeaderFlag == 0)
                        {
                            iFindVosFlag = FIND_MPEG2_VOS;
                            pstMetaData->apucMetaData[0] = (UINT8 *)pucBufPos - 4;
                        }
                    }
                    break;

                    case PICTURE_START_CODE:
                    {
                        if (FIND_MPEG2_VOS == iFindVosFlag)
                        {
                            *puiBytesConsumed = uiNumBytes - ((INT32)pucBufEnd - (INT32)pucBufPos + 4);
                            iFindVopFlag = FIND_MPEG2_VOP;
                            /*置位状态 表示已经获得所需数据 xiongfei20100212*/
                            *piStatus = META_DATA_FOUND;
                            pstMetaData->iNumMetaData = 1;
                            /* 退出 xiongfei20100302*/
                        }
                    }
                    break;

                    case EXT_START_CODE:
                    {
                        if (FIND_MPEG2_VOS == iFindVosFlag)
                        {
                            //guoshan+00101841 20100623 pc-lint warning: Warning 578 Declaration of symbol 's1' hides symbol 's1' (line 262),改s1为s2
                            //                             Mpeg1Context *s1 = avctx->priv_data;
                            //                             MpegEncContext *s = &s1->mpeg_enc_ctx;
                            Mpeg1Context *s2 = avctx->priv_data;
                            MpegEncContext *s = &s2->mpeg_enc_ctx;

                            int ext_type;
                            const UINT8  *pBuffer = pucBufPos;

                            /*为防止当前比特数不够长而导致解码头部出错，做以下处理：当当前所剩下比特数小于1k时，放弃解码头部，跳出去从外部再读数据进来xionigfei20100312    */
                            if ( MPEG2_MAX_HEADER_LENGTH > (uiNumBytes - (UINT32)((UINT32)pucBufPos - 4 - (UINT32)pucBuf)))
                            {
                                *puiBytesConsumed = (UINT32)((UINT32)pucBufPos - 4 - (UINT32)pucBuf);
                                *piStatus = META_DATA_NOT_FOUND;
                                pstMetaData->iNumMetaData = 0;

                                // 如果在当前缓冲区内无法找到起始码 爆warning xiongfei20100302
                                IMEDIA_LOG(IMEDIA_WARNING, "BITSTREAM BUFFER is NOT ENOUGH!\n");
                                avcodec_free_context(avctx);
                                avctx = NULL;
                                return IMEDIA_RET_SUCCESS;//?
                            }

                            /*x00141957 20100607 liuxiaowei测试误码*/
                            init_get_bits(&s->gb, pBuffer, iInputSize * 8);
                            ext_type = get_bits(&s->gb, 4);

                            switch (ext_type)
                            {
                                case 0x1:
                                case 0x2:
                                case 0x3:
                                    mpeg_decode_extension(avctx, pucBufPos, iInputSize);
                                    {

                                        /*根据k68699代码检视意见进行修改，解码扩展可能会失败导致无法获得色散模式 x00141957 20100518*/
                                        if (s->chroma_format != 1 && s->chroma_format != 2)
                                        {
                                            iFindVosFlag = 0;
                                            /*x00141957 20100607 如果chroma错误 则将指针置空*/
                                            pstMetaData->apucMetaData[0] = NULL;
                                        }
                                    }
                                    break;

                                default:
                                    break;
                            }


                        }
                    }
                    break;

                    default:
                    {

                    }
                    break;
                }
            }
        }

        /*记录并计算sar x00141957 20100726*/
        mpeg12_get_sar(&s1.mpeg_enc_ctx, &pstParams->stAspectRatio.usSarHeight, &pstParams->stAspectRatio.usSarWidth);

        /*从此函数退出有两种情况 ，一种是码流中没有找到帧头，而数据用尽，一种是找到 xiongfei20100211*/
        pstMetaData->ausMetaDataLen[0] = (UINT16)(pucBufPos - pstMetaData->apucMetaData[0] - 4);
        pstParams->usWidth = s1.mpeg_enc_ctx.width;
        pstParams->usHeight = s1.mpeg_enc_ctx.height;
        pstParams->eProfile = (ENUM_IVIDEO_CODEC_PROFILE)avctx->profile;
        pstParams->eLevel = (ENUM_IVIDEO_CODEC_LEVEL)avctx->level;
        pstParams->uiRefFrameNum = 2;

        /*将yuv模式付给pstParams让外面看见 xiongfei201000303*/
        if ( 1 == s1.mpeg_enc_ctx.chroma_format)
        {
            pstParams->eColorSpaceType = (ENUM_IVIDEO_COLOR_SPACE_TYPE)(s1.mpeg_enc_ctx.chroma_format - 1);
        }
        else if ( 2 == s1.mpeg_enc_ctx.chroma_format )
        {
            pstParams->eColorSpaceType = (ENUM_IVIDEO_COLOR_SPACE_TYPE)s1.mpeg_enc_ctx.chroma_format;
        }

        avcodec_free_context(avctx);
        avctx = NULL;
        return IMEDIA_RET_SUCCESS;
    }
}


#define VC1_SEQHDR     (0x0f)
#define VC1_ENTRYPOINT (0x0e)

INT32 WMV3_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 iRet = RETURN_OK;

    INT32 iStatus = *piStatus;
    UINT32 uiBufIndex = 0;
    UINT8 *pucSeqStartAddr = NULL;
    UINT8 *pucSeqEndAddr = NULL;
    INT32 iBduType = -1;
    INT32 iSeqFlag = NOT_FOUND;
    AVCodecContext *pstAvctx = NULL;

    /* 2010/07/14 14:30:00 liuxw+00139685 [AZ1D02219] */
    /* 将判断条件修改为大于等于 */
    //if(HEADER_PREFIX > uiNumBytes)
    if (HEADER_PREFIX >= uiNumBytes)
    {
        *puiBytesConsumed = uiNumBytes;
        return IMEDIA_RET_SUCCESS;
    }
    else
    {
        uiNumBytes -= HEADER_PREFIX;
    }

    pstAvctx = avcodec_alloc_context();

    if (NULL == pstAvctx)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "Malloc memory for avctx failed!\n");
        return RETURN_ERR;
    }

    while (META_DATA_FOUND != iStatus)
    {
        while (uiBufIndex < uiNumBytes)
        {
            for (; uiBufIndex < uiNumBytes; uiBufIndex++)
            {
                /* find the start code: 0x000001 */
                if (pucBuf[uiBufIndex] == 0 && pucBuf[uiBufIndex + 1] == 0 && pucBuf[uiBufIndex + 2] == 1)
                {
                    break;
                }
            }

            if (uiBufIndex >= uiNumBytes)
            {
                break;
            }

            if (FOUND_START == iSeqFlag)
            {
                iSeqFlag = FOUND_HEADER;
            }

            iBduType = pucBuf[uiBufIndex + 3];

            switch (iBduType)
            {
                case VC1_SEQHDR:
                {
                    /* set the sps start address and start find flag */
                    if (NULL == pucSeqStartAddr && NULL == pucSeqEndAddr)
                    {
                        pucSeqStartAddr = pucBuf + uiBufIndex ;
                        iSeqFlag = FOUND_START;
                    }
                }
                break;

                default:
                    break;
            }

            if (NULL != pucSeqStartAddr && FOUND_HEADER == iSeqFlag && NULL == pucSeqEndAddr)
            {
                pucSeqEndAddr = pucBuf + uiBufIndex;
            }

            if (!(FOUND_HEADER == iSeqFlag && VC1_SEQHDR == iBduType))
            {
                uiBufIndex += 3;
            }

            if (FOUND_HEADER == iSeqFlag)
            {
                break;
            }

        }

        /*lint -e613*/
        if (FOUND_HEADER == iSeqFlag)
        {
            // found SPS
            UINT32 uiSeqLength = pucSeqEndAddr - pucSeqStartAddr;
            VC1Context vc1;
            UINT8 aucBuf[NEED_LENGTH];
            UINT32 uiTmpSize = 0;

            pstAvctx->priv_data = &vc1;
            vc1.s.avctx = pstAvctx;

            uiTmpSize = vc1_unescape_buffer(pucSeqStartAddr + 4, uiSeqLength - 4, aucBuf);
            init_get_bits(&vc1.s.gb, aucBuf, uiTmpSize * 8);

            iRet = decode_sequence_header(pstAvctx, &vc1.s.gb);

            if (RETURN_OK != iRet)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "Parse the seq header failed!\n");
                iSeqFlag = NOT_FOUND;
                pucSeqStartAddr = NULL;
                pucSeqEndAddr = NULL;
            }
            else
            {
                pstMetaData->iNumMetaData++;
                pstMetaData->apucMetaData[0]   = pucSeqStartAddr;
                pstMetaData->ausMetaDataLen[0] = uiSeqLength;
                iStatus  = META_DATA_FOUND;
                iSeqFlag = NOT_FOUND;

                if (VC1_SEQHDR != iBduType)
                {
                    uiBufIndex -= 3;
                }

                /*lint -e64*/
                /* init the pstParams */
                pstParams->eProfile = vc1.profile;
                pstParams->eLevel   = IVIDEO_LEVEL_UNKNOWN;
                /*lint +e64*/
                //if(IVIDEO_PROFILE_VC1_SP == pstParams->eProfile)
                //{
                //    pstParams->eLevel   = IVIDEO_LEVEL_WMV3_MAIN;
                //}
                //else
                //{
                //    pstParams->eLevel   = IVIDEO_LEVEL_WMV3_HIGH;
                //}
                pstParams->eColorSpaceType = IVIDEO_CSP_YUV420;
                pstParams->uiRefFrameNum = 1;
                pstParams->usWidth = pstAvctx->coded_width;
                pstParams->usHeight = pstAvctx->coded_height;
                pstParams->eContentType = IVIDEO_PROGRESSIVE;
                /* 2010/07/26 14:30:00 liuxw+00139685 */
                /* wmv3没有此类语法元素则设为默认值 */
                pstParams->stAspectRatio.usSarWidth  = 1;
                pstParams->stAspectRatio.usSarHeight = 1;
            }
        }

        /*lint +e613*/

        if (uiBufIndex >= uiNumBytes)
        {
            break;
        }
    }

    if (META_DATA_FOUND != iStatus)
    {
        // only found seq header start address
        if (pucSeqStartAddr && NULL == pucSeqEndAddr)
        {
            uiBufIndex = pucSeqStartAddr - pucBuf;
        }
    }

    if (pstAvctx)
    {
        avcodec_free_context(pstAvctx);
        pstAvctx = NULL;
    }

    *puiBytesConsumed = uiBufIndex;
    *piStatus = iStatus;

    return IMEDIA_RET_SUCCESS;
}

INT32 VC1_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    // INT32 iRet = RETURN_OK;

    return IMEDIA_RET_SUCCESS;
}

INT32 MPEG4_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 iRet = RETURN_OK;

    MpegEncContext stMpegCxt;
    AVCodecContext *pstAvctx = NULL;

    UINT32 uiStartCode = 0xff;
    UINT32 uiTmp  = 0;
    UINT32 uiVOSOffset = 0; //guoshan +00101841 20100414保存起始码的偏移
    UINT32 uiVOLOffset = 0; //guoshan +00101841 20100414保存起始码的偏移
    INT32 i = 0;
    INT32 iStatus = *piStatus;
    INT32 iFoundFlag = 0;  //guoshan + 00101841 20100601 iFoundFlag =1表示解析到VOS起始码;iFoundFlag =2表示解析到VOL起始码;

    INT32 iProfile = 0;

    memset(&stMpegCxt, 0, sizeof(MpegEncContext));

    pstAvctx = avcodec_alloc_context();

    if (NULL == pstAvctx)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "avcodec_alloc_context() failed!\n");
        return RETURN_ERR;
    }

    stMpegCxt.avctx = pstAvctx;
    pstAvctx->priv_data = &stMpegCxt;

    *puiBytesConsumed = 0; //初始化
    pstMetaData->iNumMetaData = 0;

    /* 保证至少>5个字节 */
    if (uiNumBytes <= MPEG4_HEADER_PREFIX_LEN)
    {
        *puiBytesConsumed = uiNumBytes;  //guoshan+0010841 没有更多的码流,就表示数据读完了
        IMEDIA_LOG(IMEDIA_WARNING, "Not enough uiNumBytes[%d] for VOS/VOL start code!\n", uiNumBytes);
        goto MPEG4_CLEAN;
    }

    pstParams->eColorSpaceType = IVIDEO_CSP_IYUV;

    init_get_bits(&stMpegCxt.gb, pucBuf, uiNumBytes * 8);
    stMpegCxt.bitstream_buffer_size = 0;

    /*码流对齐后开始找32bits的visual_oject_sequence_start_code码*/
    align_get_bits(&stMpegCxt.gb);

    for (i = stMpegCxt.gb.size_in_bits - get_bits_count(&stMpegCxt.gb); i > 32; i -= 8)
    {
        if (META_DATA_FOUND == iStatus) //guoshan +00101841 20100525 头已解析成功，退出
        {
            break;
        }

        /* use the bits after the test */
        uiTmp = get_bits(&stMpegCxt.gb, 8);
        uiStartCode = ((uiStartCode << 8) | uiTmp) & 0xFFFFFFFF;

        if (VISOBJSEQ_START_CODE == uiStartCode && (iStatus != META_DATA_FOUND_ONLY_SPS))
        {
            uiVOSOffset = stMpegCxt.gb.index - 32;
            uiVOSOffset = uiVOSOffset >> 3;  //guoshan +00101841 20100415 转换成字节数
            *puiBytesConsumed = uiVOLOffset;
            iFoundFlag = 1;

            if (uiNumBytes < uiVOSOffset + MPEG4_VOS_LEN) ////guoshan +00101841 20100415 保证VOS头后至少有5个字节
            {
                IMEDIA_LOG(IMEDIA_WARNING, "NOT ENOUGH Bytes for VOS parsing!\n");
                goto MPEG4_CLEAN;
            }

            pstMetaData->apucMetaData[0] = pucBuf + uiVOSOffset;
            pstMetaData->ausMetaDataLen[0] = MPEG4_VOS_LEN;
            pstMetaData->iNumMetaData++;
            iProfile = get_bits(&stMpegCxt.gb, 8);
            pstParams->eProfile = (ENUM_IVIDEO_CODEC_PROFILE)((iProfile & 0xF0) >> 4);
            pstParams->eLevel = (ENUM_IVIDEO_CODEC_LEVEL)(iProfile & 0x0F);
            //guoshan + 00101841 20100415 传递相关参数给pstParams

            iRet = MPEG4_CHECK_PROFILE(pstParams); //guoshan + 00101841 20100525 该函数恒返回OK，不用再对其返回值进行判断

            iStatus = (META_DATA_FOUND_ONLY_PPS == iStatus) ? META_DATA_FOUND : META_DATA_FOUND_ONLY_SPS;
            IMEDIA_LOG(IMEDIA_INFO, "Found VOS header!\n");

            align_get_bits(&stMpegCxt.gb);
            uiStartCode = 0xff;  //重新置位后再找VOL header
        }
        else if ((VIDOBJLAY_START_CODE_MASK == (uiStartCode & 0xFFFFFFF0)) && (iStatus != META_DATA_FOUND_ONLY_PPS))
        {
            UINT32 uiVolEnd = 0;

            uiVOLOffset = stMpegCxt.gb.index >> 3;
            uiVOLOffset -= MPEG4_HEADER_PREFIX_LEN; //guoshan+00101841 20100525 回退到VOL头的首地址
            *puiBytesConsumed = uiVOLOffset;
            iFoundFlag = 2;

            if (uiNumBytes < uiVOLOffset + NEED_LENGTH) //guoshan+00101841 20100415 保证VOL头后至少有NEED_LENGTH个字节
            {
                IMEDIA_LOG(IMEDIA_WARNING, "NOT ENOUGH Bytes for VOL parsing!\n");
                goto MPEG4_CLEAN;
            }

            iRet = decode_vol_header(&stMpegCxt, &stMpegCxt.gb);
            uiVolEnd = ((UINT32)stMpegCxt.gb.index + 7) >> 3;

            if (RETURN_OK != iRet)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "VOL parsing failed!\n");
                align_get_bits(&stMpegCxt.gb);
                i = stMpegCxt.gb.size_in_bits - get_bits_count(&stMpegCxt.gb);
                uiVOLOffset = 0;
                iFoundFlag = 0;  //guoshan+00101841 20100601 解析失败，标志位复位，重新开始
            }
            else
            {
                pstMetaData->apucMetaData[1] = pucBuf + uiVOLOffset;
                pstMetaData->ausMetaDataLen[1] = uiVolEnd - uiVOLOffset;  //guoshan+00101841 20100413 32bits占用4个bytes, 是8字节对齐的
                pstMetaData->iNumMetaData++;
                pstAvctx->iChromaFormat = stMpegCxt.chroma_format;
                iStatus = (META_DATA_FOUND_ONLY_SPS == iStatus) ? META_DATA_FOUND : META_DATA_FOUND_ONLY_PPS;

                IMEDIA_LOG(IMEDIA_INFO, "Found VOL header!\n");

                pstParams->usWidth = stMpegCxt.width;
                pstParams->usHeight = stMpegCxt.height;
                pstParams->uiRefFrameNum = 1;
                //guoshan+00101841 20100723 增加par信息输出
                pstParams->stAspectRatio.usSarWidth = stMpegCxt.avctx->sample_aspect_ratio.num;
                pstParams->stAspectRatio.usSarHeight = stMpegCxt.avctx->sample_aspect_ratio.den;

                //guoshan+00101841 20100803 增加frame类型信息输出
                pstParams->eContentType = stMpegCxt.progressive_frame ? IVIDEO_PROGRESSIVE : IVIDEO_INTERLACED;

                align_get_bits(&stMpegCxt.gb);
            }

            uiStartCode = 0xff;  //重新置位后再找VOL header
        }
    }

    if (iStatus == META_DATA_NOT_FOUND
        || (iStatus == META_DATA_FOUND_ONLY_SPS && iFoundFlag != 2)
        || (iStatus == META_DATA_FOUND_ONLY_PPS && iFoundFlag != 1))
    {
        *puiBytesConsumed = uiNumBytes - 4;
    }

MPEG4_CLEAN:
    *piStatus = iStatus;

    if (pstAvctx)
    {
        avcodec_free_context(pstAvctx);
        pstAvctx = NULL;
    }

    return IMEDIA_RET_SUCCESS;
}
#endif

INT32 H263_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 iRet = RETURN_OK;

    MpegEncContext stH263Cxt;
    Picture stPic;  //guoshan +00101841 20100413这个结构体的声明只是为了防止修改h263_decode_picture_header(),造成实际解码中的问题，其值在probeHeader中无用
    AVCodecContext *pstAvctx = NULL;

    UINT32 uiStartCode = 0;
    INT32 i = 0;
    INT32 iIndex = 0; //guoshan +00101841 20100414保存起始码的首地址
    INT32 iIndex_IN_BYTES = 0; //guoshan +00101841 20100414保存起始码的首地址


    INT32 iStatus = *piStatus;

    memset(&stH263Cxt, 0, sizeof(MpegEncContext));
    memset(&stPic, 0, sizeof(Picture));

    pstAvctx = avcodec_alloc_context();

    if (NULL == pstAvctx)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "avcodec_alloc_context() failed!\n");
        return RETURN_ERR;
    }

    stH263Cxt.avctx = pstAvctx;
    stH263Cxt.current_picture_ptr = &stPic;
    pstAvctx->priv_data = &stH263Cxt;

    *puiBytesConsumed = 0; //初始化

    pstParams->eColorSpaceType = IVIDEO_CSP_IYUV;
    memset(pstMetaData, 0, sizeof(STRU_IVIDDEC_META_DATA));

    /* 保证至少>3个字节 */
    if (uiNumBytes <= PSC_LENGTH_IN_BYTES)
    {
        *puiBytesConsumed = uiNumBytes;  //guoshan +00101841 20100529
        IMEDIA_LOG(IMEDIA_WARNING, "Not enough uiNumBytes[%d] for probeHeader!\n", uiNumBytes);
        goto H263_CLEAN;
    }

    init_get_bits(&stH263Cxt.gb, pucBuf, uiNumBytes * 8);
    stH263Cxt.bitstream_buffer_size = 0;

    /*码流对齐后开始找22bits的PSC码*/
    align_get_bits(&stH263Cxt.gb);

    uiStartCode = get_bits(&stH263Cxt.gb, PSC_LENGTH_IN_BITS - 8); //guoshan+00101841 20100531 这里bit偏移了14位

    for (i = stH263Cxt.gb.size_in_bits - get_bits_count(&stH263Cxt.gb); i > 24; i -= 8)
    {
        if (META_DATA_FOUND == iStatus)
        {
            break;
        }

        uiStartCode = ((uiStartCode << 8) | get_bits(&stH263Cxt.gb, 8)) & 0x003FFFFF;   //guoshan +00101841 20100412 取22bits

        if (uiStartCode == 0x20) //guoshan +00101841 20100412: found start code
        {
            iIndex = stH263Cxt.gb.index - PSC_LENGTH_IN_BITS;
            iIndex_IN_BYTES = iIndex >> 3;
            *puiBytesConsumed = iIndex_IN_BYTES;  //guoshan+00101841 20100526:这种情况下,*puiBytesConsumed回退到起始码的首地址, 22bits占3个bytes

            //guoshan + 00101841: found PSC, but left buffer is not enough to parse
            if ( (uiNumBytes - iIndex_IN_BYTES) < NEED_LENGTH)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "BITSTREAM BUFFER is NOT ENOUGH!\n");
                goto H263_CLEAN;
            }

            /*这个地方码流起始地址已经偏到PSC的尾部了, index清0, 利用原有函数重新开始解析*/
            /*guoshan+00101841 20100531此处不应清0，应该把stH263Cxt.gb.index回位到PSC首地址即可*/
            //stH263Cxt.gb.index = 0;
            stH263Cxt.gb.index = iIndex;
            iRet = h263_decode_picture_header(&stH263Cxt);

            if (RETURN_OK == iRet)
            {
                /*guoshan + 00101841 20100413 17:30:00*/
                /*在解析成功的情况下，PSC从码流首地址开始时，*puiBytesConsumed = pstMetaData->ausMetaDataLen，其余情况则不然，特别注意*/
                iStatus = META_DATA_FOUND;
                /*guoshan + 00101841 20100529 14:00:00*/
                /*probeheader解析后的保存其序列数据，H.263只有帧头信息，不用保存出来了*/
                //                 pstMetaData->iNumMetaData = 1;
                //                 pstMetaData->apucMetaData[0] = pucBuf + ((iIndex + 7) >> 3);  //guoshan +00101841 20100413 22bits占用3个bytes
                //                 pstMetaData->ausMetaDataLen[0] = (stH263Cxt.gb.index - iIndex + 7) >> 3;  //guoshan + 00101841 20100413 09:20:00从起始码的实际起始位置开始统计

                /*guoshan+00101841 20100413 15:15:00*/
                /*H263 level info cannot be concluded in the header*/
                pstParams->usWidth = stH263Cxt.width;
                pstParams->usHeight = stH263Cxt.height;
                pstParams->eProfile = (stH263Cxt.h263_plus == 0) ? IVIDEO_PROFILE_H263 : IVIDEO_PROFILE_H263_PLUS;
                pstParams->uiRefFrameNum = 1;
                //guoshan+00101841 20100723 增加par信息输出
                pstParams->stAspectRatio.usSarWidth = stH263Cxt.avctx->sample_aspect_ratio.num;
                pstParams->stAspectRatio.usSarHeight = stH263Cxt.avctx->sample_aspect_ratio.den;


                break;
            }
            else
            {
                IMEDIA_LOG(IMEDIA_WARNING, "H263 Header probe failed!\n");
                //guoshan + 00101841 20100526 循环继续找头
                align_get_bits(&stH263Cxt.gb);
                uiStartCode = get_bits(&stH263Cxt.gb, PSC_LENGTH_IN_BITS - 8); //guoshan+00101841 20100531 这里bit再次偏移了14位
                i = stH263Cxt.gb.size_in_bits - get_bits_count(&stH263Cxt.gb);
            }
        }
    }

    if (iStatus != META_DATA_FOUND)
    {
        *puiBytesConsumed = uiNumBytes - 3;
    }

H263_CLEAN:
    *piStatus = iStatus;

    if (pstAvctx)
    {
        avcodec_free_context(pstAvctx);
        pstAvctx = NULL;
    }

    return IMEDIA_RET_SUCCESS;
}

static INT32 GetBitstream(const UINT8 *pucSrc, UINT8 *pucDst, INT32 *piDstLength, INT32 iLength)
{
    INT32 iRet = RETURN_OK;

    INT32 di;

    /* check the validation of the params */
    if (NULL == pucSrc)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "input buf is null!\n");
        return RETURN_ERR;
    }

    if (NULL == pucDst)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "output pointer is null!\n");
        return RETURN_ERR;
    }

    if (NULL == piDstLength)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "The dst size pointer is NULL!\n");
        return RETURN_ERR;
    }

    memcpy(pucDst, pucSrc, iLength);

    di = iLength;

    /*x00141957 20100630添加 将防错码去掉*/
    {
        int i, j;

        for (i = 0; i < iLength - 4; i++)
        {
            if ((pucDst[i] == 0) && (pucDst[i + 1] == 0) && (pucDst[i + 2] == 3) && ((pucDst[i + 3] == 3) || (pucDst[i + 3] == 1)))
            {
                for (j = i + 2; j < iLength - 2; j++)
                {
                    pucDst[j] = pucDst[j + 1];
                }

                di--;
            }
        }
    }

    *piDstLength = di;

    return iRet;
}

INT32 VP6_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
#define HEADER_BUF_SIZE 1024
    INT32 pos;
    UINT32 uiLength;
    UINT8 *pucBuffer = pucBuf;
    INT32 iRet;
    UINT8 aucVP6HeaderBuf[HEADER_BUF_SIZE] = {0};
    INT32 iDstLength;
    INT32 iSrcLength;

    /*找到帧头以及成功解码标志位 x00141957 20100714*/
    INT32 DecodeSuccess;
    INT32 FindHeader;
    INT32 NumBytes = (INT32)uiNumBytes;
    *puiBytesConsumed = 0; //初始化

    /*初始化各标志位 x00141957 20100714*/
    DecodeSuccess = 0;
    FindHeader = 0;
#define FIND_HEADER 1
#define DECODE_SUCCESS 1

    /*x00141957 20100714 若找不到正确frame header，则一直找下去，直到码流结束*/
    while (DECODE_SUCCESS != DecodeSuccess)
    {
        /* 保证至少>3个字节 */
        if (PSC_LENGTH_IN_BYTES >= NumBytes)
        {
            *piStatus = META_DATA_NOT_FOUND;
            *puiBytesConsumed = NumBytes;
            IMEDIA_LOG(IMEDIA_WARNING, "Not enough uiNumBytes[%d] for probeHeader!\n", NumBytes);
            return IMEDIA_RET_SUCCESS;
        }

        /*开始寻找起始码 x00141957 20100706*/
        for (pos = 0; pos < NumBytes - 2; pos++)
        {
            if ((pucBuffer[pos] == 0) && (pucBuffer[pos + 1] == 0) && (pucBuffer[pos + 2] == 1))
            {
                /*找到起始码 x00141957 20100626*/
                FindHeader = FIND_HEADER;
                break;
            }

            /*问题单号：AZ1D02228
            修改人：熊飞 +00141957
            时间：2010/7/21
            问题描述： 当第一次输入中没有帧头时导致后面一直找不到帧头
            问题修改： 逐字节寻找帧头*/
            //pucBuffer += 3;
        }

        /*如果码流耗尽却还没有找到头 x00141957 20100716*/
        if (FIND_HEADER != FindHeader)
        {
            *piStatus = META_DATA_NOT_FOUND;
            *puiBytesConsumed = NumBytes - 3;
            return IMEDIA_RET_SUCCESS;
        }

        pucBuffer += pos;

        /*问题单号：AZ1D02284
        修改人：熊飞 +00141957
        时间：2010/08/31
        问题描述： VP6解码器解码1080P分辨率的flv码流文件时，程序发生异常退出
        问题修改： 随着buffer的推进，实际有用字节数递减*/
        NumBytes -= pos;

        uiLength = NumBytes - (pos + 3);


        /*调用函数 解析码流头 x00141957 20100626*/
        /*vp6 帧头I帧最大为81个比特，P帧最大为43个比特，所以最小要求为11个字节 这里设为20 x00141957 20100706*/
#define MIN_VP6_HEADER 20

        if (MIN_VP6_HEADER > uiLength)
        {
            *puiBytesConsumed = NumBytes - (uiLength + 3);
            *piStatus = META_DATA_NOT_FOUND;
            IMEDIA_LOG(IMEDIA_WARNING, "Not enough uiNumBytes[%d] for decodeHeader!\n", NumBytes);
            return IMEDIA_RET_SUCCESS;
        }

        /*最少读900字节进去 去掉0x03 x00141957 20100707*/
#define VP6_MAX_HEADER_LENGTH 900

        if (VP6_MAX_HEADER_LENGTH <= NumBytes)
        {
            iSrcLength = VP6_MAX_HEADER_LENGTH;
        }
        else
        {
            iSrcLength = NumBytes;
        }

        iRet = GetBitstream(pucBuffer, aucVP6HeaderBuf, &iDstLength, iSrcLength);

        if ( RETURN_OK == iRet)
        {
            iRet = VP6_Decodeheader(aucVP6HeaderBuf, uiLength, pstParams);

            if ( RETURN_OK == iRet)
            {
                DecodeSuccess = DECODE_SUCCESS;
            }
            else
            {
                /*如果解码头部失败，则将buffer中的起始码去掉，重新开始寻找起始码 x00141957 20100716*/
                FindHeader = 0;
                pucBuffer += 3;
                /*问题单号：AZ1D02284
                修改人：熊飞 +00141957
                时间：2010/08/31
                问题描述： VP6解码器解码1080P分辨率的flv码流文件时，程序发生异常退出
                问题修改： 随着buffer的推进，实际有用字节数递减*/
                NumBytes -= 3;
            }
        }
    }

    *piStatus = META_DATA_FOUND;
    pstMetaData->iNumMetaData = 0;
    pstMetaData->apucMetaData[0] = NULL;

    return IMEDIA_RET_SUCCESS;
}


INT32 VP6A_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
#define HEADER_BUF_SIZE 1024
    INT32 pos;
    UINT32 uiLength;
    UINT8 *pucBuffer = pucBuf;
    INT32 iRet;
    UINT8 aucVP6HeaderBuf[HEADER_BUF_SIZE] = {0};
    INT32 iDstLength;
    INT32 iSrcLength;

    /*找到帧头以及成功解码标志位 x00141957 20100714*/
    INT32 DecodeSuccess;
    INT32 FindHeader;
    INT32 NumBytes = (INT32)uiNumBytes;
    *puiBytesConsumed = 0; //初始化

    /*初始化各标志位 x00141957 20100714*/
    DecodeSuccess = 0;
    FindHeader = 0;
#define FIND_HEADER 1
#define DECODE_SUCCESS 1

    /*x00141957 20100714 若找不到正确frame header，则一直找下去，直到码流结束*/
    while (DECODE_SUCCESS != DecodeSuccess)
    {
        /* 保证至少>3个字节 */
        if (PSC_LENGTH_IN_BYTES >= NumBytes)
        {
            *piStatus = META_DATA_NOT_FOUND;
            *puiBytesConsumed = NumBytes;
            IMEDIA_LOG(IMEDIA_WARNING, "Not enough uiNumBytes[%d] for probeHeader!\n", NumBytes);
            return IMEDIA_RET_SUCCESS;
        }

        /*开始寻找起始码 x00141957 20100706*/
        for (pos = 0; pos < NumBytes - 2; pos++)
        {
            if ((pucBuffer[pos] == 0) && (pucBuffer[pos + 1] == 0) && (pucBuffer[pos + 2] == 1))
            {
                /*找到起始码 x00141957 20100626*/
                FindHeader = FIND_HEADER;
                break;
            }

            /*问题单号：AZ1D02228
            修改人：熊飞 +00141957
            时间：2010/7/21
            问题描述： 当第一次输入中没有帧头时导致后面一直找不到帧头
            问题修改： 逐字节寻找帧头*/
            //pucBuffer += 3;
        }

        /*如果码流耗尽却还没有找到头 x00141957 20100716*/
        if (FIND_HEADER != FindHeader)
        {
            *piStatus = META_DATA_NOT_FOUND;
            *puiBytesConsumed = NumBytes - 3;
            return IMEDIA_RET_SUCCESS;
        }

        pucBuffer += pos;

        /*问题单号：AZ1D02284
        修改人：熊飞 +00141957
        时间：2010/08/31
        问题描述： VP6解码器解码1080P分辨率的flv码流文件时，程序发生异常退出
        问题修改： 随着buffer的推进，实际有用字节数递减*/
        NumBytes -= pos;

        uiLength = NumBytes - (pos + 3);


        /*调用函数 解析码流头 x00141957 20100626*/
        /*vp6 帧头I帧最大为81个比特，P帧最大为43个比特，所以最小要求为11个字节 这里设为20 x00141957 20100706*/
#define MIN_VP6_HEADER 20

        if (MIN_VP6_HEADER > uiLength)
        {
            *puiBytesConsumed = NumBytes - (uiLength + 3);
            *piStatus = META_DATA_NOT_FOUND;
            IMEDIA_LOG(IMEDIA_WARNING, "Not enough uiNumBytes[%d] for decodeHeader!\n", NumBytes);
            return IMEDIA_RET_SUCCESS;
        }

        /*最少读900字节进去 去掉0x03 x00141957 20100707*/
#define VP6_MAX_HEADER_LENGTH 900

        if (VP6_MAX_HEADER_LENGTH <= NumBytes)
        {
            iSrcLength = VP6_MAX_HEADER_LENGTH;
        }
        else
        {
            iSrcLength = NumBytes;
        }

        iRet = GetBitstream(pucBuffer, aucVP6HeaderBuf, &iDstLength, iSrcLength);

        if ( RETURN_OK == iRet)
        {

            iRet = VP6A_Decodeheader(aucVP6HeaderBuf, uiLength, pstParams);

            if ( RETURN_OK == iRet)
            {
                DecodeSuccess = DECODE_SUCCESS;
            }
            else
            {
                /*如果解码头部失败，则将buffer中的起始码去掉，重新开始寻找起始码 x00141957 20100716*/
                FindHeader = 0;
                pucBuffer += 3;
                /*问题单号：AZ1D02284
                修改人：熊飞 +00141957
                时间：2010/08/31
                问题描述： VP6解码器解码1080P分辨率的flv码流文件时，程序发生异常退出
                问题修改： 随着buffer的推进，实际有用字节数递减*/
                NumBytes -= 3;
            }
        }
    }

    *piStatus = META_DATA_FOUND;
    pstMetaData->iNumMetaData = 0;
    pstMetaData->apucMetaData[0] = NULL;

    return IMEDIA_RET_SUCCESS;
}

/*这里需要考虑到如果是p frame，则没有宽高信息，需要退出重新寻找 x00141957 20100714*/
static INT32 VP6_Decodeheader(UINT8 *pucSrcBuf, INT32 iLength, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 key_frame = 0;
    INT32 filter_header = 0;
    INT32 dequant = 0;
    INT32 use_huffman = 0;
    INT32 filter_mode = 0;
    INT32 filter_selection = 0;
    INT32 sample_variance_threshold = 0;
    INT32 max_vector_length = 0;

    UINT8 *buf = (pucSrcBuf + 3);
    INT32 buf_size = iLength;
    INT32 parse_filter_info = 0;
    INT32 coeff_offset = 0;
    INT32 vrt_shift = 0;
    INT32 sub_version;
    INT32 rows, cols;
    INT32 res = RETURN_OK;
    VP56RangeCoder vp56RC;
    VP56RangeCoder *c = &vp56RC;
    /*问题单号：AZ1D02316
    修改人：熊飞 +00141957
    时间：2010/09/09
    问题描述： 解码1080p图像与ffmpeg不一致
    问题修改： 将flv中额外信息带入码流中*/
    UINT8 cropsize = buf[0];

    INT32 separated_coeff;

    buf++;
    iLength --;
    separated_coeff = buf[0] & 1;


    key_frame = !(buf[0] & 0x80);
    dequant = (buf[0] >> 1) & 0x3F;

    if (key_frame)
    {
        sub_version = buf[1] >> 3;

        if (sub_version > 8)
        {
            /*x00141957 20100714*/
            av_log(NULL, AV_LOG_ERROR, "sub_version[%d] is wrong !\n" , sub_version);
            return -1;
        }

        filter_header = buf[1] & 0x06;

        if (buf[1] & 1)
        {
            av_log(NULL, AV_LOG_ERROR, "interlacing not supported\n");
            return -1;
        }

        if (separated_coeff || !filter_header)
        {
            coeff_offset = AV_RB16(buf + 2) - 2;
            buf += 2;
            buf_size -= 2;
        }

        rows = buf[2];  /* number of stored macroblock rows */
        cols = buf[3];  /* number of stored macroblock cols */
        /* buf[4] is number of displayed macroblock rows */
        /* buf[5] is number of displayed macroblock cols */

        vp56_init_range_decoder(c, buf + 6, buf_size - 6);
        vp56_rac_gets(c, 2);

        parse_filter_info = filter_header;

        if (sub_version < 8)
        { vrt_shift = 5; }
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR, "it isn't keyframe! \n");
        return -1;
    }

    if (parse_filter_info)
    {
        if (vp56_rac_get(c))
        {
            filter_mode = 2;
            sample_variance_threshold = vp56_rac_gets(c, 5) << vrt_shift;
            max_vector_length = 2 << vp56_rac_gets(c, 3);
        }
        else if (vp56_rac_get(c))
        {
            filter_mode = 1;
        }
        else
        {
            filter_mode = 0;
        }

        if (sub_version > 7)
        { filter_selection = vp56_rac_gets(c, 4); }
        else
        { filter_selection = 16; }
    }

    use_huffman = vp56_rac_get(c);

    av_log(NULL, AV_LOG_DEBUG, "vp6 info: sample_variance_threshold = %d, max_vector_length =%d ,use_huffman = %d , coeff_offset = %d, dequant =%d, filter_mode =%d, filter_selection = %d\n", sample_variance_threshold , max_vector_length, use_huffman, coeff_offset, dequant, filter_mode, filter_selection);

    /*添加：如果解析成功 则将信息写入pstParams x00141957 20100629*/
    pstParams->stAspectRatio.usSarHeight = 1;
    pstParams->stAspectRatio.usSarWidth = 1;
    /*问题单号：AZ1D02316
    修改人：熊飞 +00141957
    时间：2010/09/09
    问题描述： 解码1080p图像与ffmpeg不一致
    问题修改： 将flv中额外信息带入码流中*/
    pstParams->usWidth = (cols << 4) - (((unsigned int)cropsize) >> 4);
    pstParams->usHeight = (rows << 4) - (((unsigned int)cropsize) & 0x0F);
    /*lint -e64*/
    pstParams->eProfile = sub_version;
    /*lint +e64*/
    pstParams->uiRefFrameNum = 2;
    /*x00141957 20100803添加*/
    pstParams->eColorSpaceType = IVIDEO_CSP_YUV420;
    pstParams->eContentType = IVIDEO_PROGRESSIVE;
    return res;
}


/*这里需要考虑到如果是p frame，则没有宽高信息，需要退出重新寻找 x00141957 20100714*/
static INT32 VP6A_Decodeheader(UINT8 *pucSrcBuf, INT32 iLength, STRU_IVIDEO_STREAM_INFO *pstParams)
{
    INT32 key_frame = 0;
    INT32 filter_header = 0;
    INT32 dequant = 0;
    INT32 use_huffman = 0;
    INT32 filter_mode = 0;
    INT32 filter_selection = 0;
    INT32 sample_variance_threshold = 0;
    INT32 max_vector_length = 0;

    UINT8 *buf = (pucSrcBuf + 3);
    INT32 buf_size = iLength;
    INT32 parse_filter_info = 0;
    INT32 coeff_offset = 0;
    INT32 vrt_shift = 0;
    INT32 sub_version;
    INT32 rows, cols;
    INT32 res = RETURN_OK;
    VP56RangeCoder vp56RC;
    VP56RangeCoder *c = &vp56RC;
    /*问题单号：AZ1D02316
    修改人：熊飞 +00141957
    时间：2010/09/09
    问题描述： 解码1080p图像与ffmpeg不一致
    问题修改： 将flv中额外信息带入码流中*/
    UINT8 cropsize = buf[0];

    INT32 separated_coeff;

    buf++;
    iLength --;

    /*去掉alpha offset长度 x00141957 20110402*/
    buf += 3;
    iLength -= 3;

    separated_coeff = buf[0] & 1;


    key_frame = !(buf[0] & 0x80);
    dequant = (buf[0] >> 1) & 0x3F;

    if (key_frame)
    {
        sub_version = buf[1] >> 3;

        if (sub_version > 8)
        {
            /*x00141957 20100714*/
            av_log(NULL, AV_LOG_ERROR, "sub_version[%d] is wrong !\n" , sub_version);
            return -1;
        }

        filter_header = buf[1] & 0x06;

        if (buf[1] & 1)
        {
            av_log(NULL, AV_LOG_ERROR, "interlacing not supported\n");
            return -1;
        }

        if (separated_coeff || !filter_header)
        {
            coeff_offset = AV_RB16(buf + 2) - 2;
            buf += 2;
            buf_size -= 2;
        }

        rows = buf[2];  /* number of stored macroblock rows */
        cols = buf[3];  /* number of stored macroblock cols */
        /* buf[4] is number of displayed macroblock rows */
        /* buf[5] is number of displayed macroblock cols */

        vp56_init_range_decoder(c, buf + 6, buf_size - 6);
        vp56_rac_gets(c, 2);

        parse_filter_info = filter_header;

        if (sub_version < 8)
        { vrt_shift = 5; }
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR, "it isn't keyframe! \n");
        return -1;
    }

    if (parse_filter_info)
    {
        if (vp56_rac_get(c))
        {
            filter_mode = 2;
            sample_variance_threshold = vp56_rac_gets(c, 5) << vrt_shift;
            max_vector_length = 2 << vp56_rac_gets(c, 3);
        }
        else if (vp56_rac_get(c))
        {
            filter_mode = 1;
        }
        else
        {
            filter_mode = 0;
        }

        if (sub_version > 7)
        { filter_selection = vp56_rac_gets(c, 4); }
        else
        { filter_selection = 16; }
    }

    use_huffman = vp56_rac_get(c);

    av_log(NULL, AV_LOG_DEBUG, "vp6 info: sample_variance_threshold = %d, max_vector_length =%d ,use_huffman = %d , coeff_offset = %d, dequant =%d, filter_mode =%d, filter_selection = %d\n", sample_variance_threshold , max_vector_length, use_huffman, coeff_offset, dequant, filter_mode, filter_selection);

    /*添加：如果解析成功 则将信息写入pstParams x00141957 20100629*/
    pstParams->stAspectRatio.usSarHeight = 1;
    pstParams->stAspectRatio.usSarWidth = 1;
    /*问题单号：AZ1D02316
    修改人：熊飞 +00141957
    时间：2010/09/09
    问题描述： 解码1080p图像与ffmpeg不一致
    问题修改： 将flv中额外信息带入码流中*/
    pstParams->usWidth = (cols << 4) - (((unsigned int)cropsize) >> 4);
    pstParams->usHeight = (rows << 4) - (((unsigned int)cropsize) & 0x0F);
    /*lint -e64*/
    pstParams->eProfile = sub_version;
    /*lint +e64*/
    pstParams->uiRefFrameNum = 2;
    /*x00141957 20100803添加*/
    pstParams->eColorSpaceType = IVIDEO_CSP_YUV420;
    pstParams->eContentType = IVIDEO_PROGRESSIVE;
    return res;
}

INT32 Sorenson_Spark_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams)
{
#define  SORENSON_HEADER_LEN  100
#define  SORENSON_START_CODE_LEN  17
#define  SORENSON_START_CODE  0x01

    INT32 iRet = RETURN_OK;

    MpegEncContext stSorensonCxt;
    AVCodecContext *pstAvctx = NULL;

    UINT32 uiStartCode = 0;
    INT32 i = 0;
    INT32 iIndex = 0; //guoshan +00101841 20100414保存起始码的首地址
    INT32 iIndex_IN_BYTES = 0; //guoshan +00101841 20100414保存起始码的首地址


    INT32 iStatus = *piStatus;

    memset(&stSorensonCxt, 0, sizeof(MpegEncContext));

    pstAvctx = avcodec_alloc_context();

    if (NULL == pstAvctx)
    {
        IMEDIA_LOG(IMEDIA_WARNING, "avcodec_alloc_context() failed!\n");
        return RETURN_ERR;
    }

    stSorensonCxt.avctx = pstAvctx;
    pstAvctx->priv_data = &stSorensonCxt;

    *puiBytesConsumed = 0; //初始化

    pstParams->eColorSpaceType = IVIDEO_CSP_IYUV;
    memset(pstMetaData, 0, sizeof(STRU_IVIDDEC_META_DATA));

    /* 保证至少>3个字节 */
    if (uiNumBytes <= PSC_LENGTH_IN_BYTES)
    {
        *puiBytesConsumed = uiNumBytes;  //guoshan +00101841 20100529
        IMEDIA_LOG(IMEDIA_WARNING, "Not enough uiNumBytes[%d] for probeHeader!\n", uiNumBytes);
        goto SORENSON_CLEAN;
    }

    init_get_bits(&stSorensonCxt.gb, pucBuf, uiNumBytes * 8);
    stSorensonCxt.bitstream_buffer_size = 0;

    /*码流对齐后开始找17bits的startcode*/
    align_get_bits(&stSorensonCxt.gb);

    uiStartCode = get_bits(&stSorensonCxt.gb, SORENSON_START_CODE_LEN - 8);

    for (i = stSorensonCxt.gb.size_in_bits - get_bits_count(&stSorensonCxt.gb); i > 24; i -= 8)
    {
        if (META_DATA_FOUND == iStatus)
        {
            break;
        }

        uiStartCode = ((uiStartCode << 8) | get_bits(&stSorensonCxt.gb, 8)) & 0x0001FFFF;

        if (uiStartCode == SORENSON_START_CODE)
        {
            iIndex = stSorensonCxt.gb.index - SORENSON_START_CODE_LEN;
            iIndex_IN_BYTES = iIndex >> 3;
            *puiBytesConsumed = iIndex_IN_BYTES;

            //guoshan + 00101841: found startcode, but left buffer is not enough to parse
            if ( (uiNumBytes - iIndex_IN_BYTES) < SORENSON_HEADER_LEN)
            {
                IMEDIA_LOG(IMEDIA_WARNING, "BITSTREAM BUFFER is NOT ENOUGH!\n");
                goto SORENSON_CLEAN;
            }

            stSorensonCxt.gb.index = iIndex;
            iRet = flv_h263_decode_picture_header(&stSorensonCxt);

            if (RETURN_OK == iRet)
            {
                iStatus = META_DATA_FOUND;

                pstParams->usWidth = stSorensonCxt.width;
                pstParams->usHeight = stSorensonCxt.height;
                pstParams->eProfile = (stSorensonCxt.h263_flv - 1);
                pstParams->uiRefFrameNum = 1;
                break;
            }
            else
            {
                IMEDIA_LOG(IMEDIA_WARNING, "Sorenson Spark Header probe failed!\n");
                //guoshan + 00101841 20100526 循环继续找头
                align_get_bits(&stSorensonCxt.gb);
                uiStartCode = get_bits(&stSorensonCxt.gb, SORENSON_START_CODE_LEN - 8); //guoshan+00101841 20100531 这里bit再次偏移了14位
                i = stSorensonCxt.gb.size_in_bits - get_bits_count(&stSorensonCxt.gb);
            }
        }
    }

    if (iStatus != META_DATA_FOUND)
    {
        *puiBytesConsumed = uiNumBytes - 3;
    }

SORENSON_CLEAN:
    *piStatus = iStatus;

    if (pstAvctx)
    {
        avcodec_free_context(pstAvctx);
        pstAvctx = NULL;
    }

    return IMEDIA_RET_SUCCESS;
}
