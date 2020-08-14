#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <linux/random.h>

#include "hi_debug.h"
#include "hi_mpi_mem.h"
#include "rtcp_priv.h"
#include "rtp_priv.h"
#include "hi_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif
#define RTCP_MEAN_DEVIATION_COUNT 16

#define RTCP_RAND_INDEX(num)  (num %10000)
#define RTCP_INVALID_VALUE_HI_U32 0Xffffffff
#define RTCP_INVALID_VALUE_HI_S32 0X7fffffff
#define RTCP_MSEL_PER_SAMPLES (8000 / 1000)
static VP_RTCPREPORTBLOCK g_RtcpReprotBlock[SOURCE_COUNT];
static VP_RTCP_SDES_CHUNK g_stSdesChunk;
#define RTCP_REPROT_TIME_MAX  5
static HI_U32 g_RtcpReprotTime[RTCP_REPROT_TIME_MAX][2];

static pthread_mutex_t g_RtcpMutex = PTHREAD_MUTEX_INITIALIZER;
#define HI_RTCP_LOCK() (void)pthread_mutex_lock(&g_RtcpMutex);
#define HI_RTCP_UNLOCK() (void)pthread_mutex_unlock(&g_RtcpMutex);

static HI_U32          RTCP_UTILS_TimeGetTime (HI_VOID);
static HI_U32          RTCP_UTILS_Rand (HI_VOID);
static HI_BOOL         RTCP_UTILS_SystemTimeCMP (HI_U32 ulBigTime, HI_U32 ulSmallTime);
static VP_RTP_RESULT   RTCP_GetSDESItem (VP_RTCP_SESSION* pstSession, HI_S8 uiType, HI_CHAR* pszValue);
static VP_RTP_RESULT   RTCP_SetSDESItem (VP_SOURCE* pstSource, HI_S8 uiType, HI_CHAR* pszValue, HI_S16 iLength);
static VP_RTP_RESULT   RTCP_BuildSDESItem (VP_RTCP_SESSION* pstSession, HI_S8 ucType, HI_CHAR* pstItem);
static VP_RTP_RESULT   RTCP_BuildSDESChunck (VP_RTCP_SESSION* pstSession, VP_RTCP_SDES_CHUNK* pstChunk,
        HI_U16* puiLength);
static HI_S32          RTCP_BuildReportBlocks (VP_RTCP_SESSION* pstSession, VP_RTCPREPORTBLOCK* pstReportBlock,
        HI_U32 ulTime);
static VP_RTP_RESULT   RTCP_UpdateReportBlocks (VP_tRTCPSessionHandle hHandle, HI_U32 ulTime);
static HI_VOID         RTCP_RemoveReportBlocks (VP_tRTCPSessionHandle hHandle, HI_U32 ulSSRC);
static HI_VOID         RTCP_InitializeSource (VP_SOURCE* pstSource, HI_U32 ulSequence, HI_U32 ulTimestamp);
static VP_SOURCE*      RTCP_LookupSource (VP_RTCP_SESSION* pstSession, HI_U32 ulSSRC);
static VP_SOURCE*      RTCP_CreateSource (VP_RTCP_SESSION*  pstSession, HI_U32 ulSSRC, HI_BOOL bProbation);
static HI_VOID         RTCP_DeleteSource (VP_tRTCPSessionHandle hHandle, HI_U32 ulSSRC);
static HI_BOOL         RTCP_UpdateSource (VP_SOURCE* pstSource, HI_U32 ulSequence, HI_U32 ulTimestamp);
static HI_VOID         RTCP_Pack_SRPacket (VP_tRTCPSessionHandle hHandle, HI_S8* pstPacket, HI_S32* piLen);
static HI_VOID         RTCP_Pack_RRPacket (VP_tRTCPSessionHandle hHandle, HI_S8* pstPacket, HI_S32* piLen);
static VP_RTP_RESULT   RTCP_Pack_SDESPacket (VP_tRTCPSessionHandle hHandle, HI_S8* pstPacket, HI_S32* piLen);
static HI_VOID         RTCP_Pack_APPPacket (VP_tRTCPSessionHandle hHandle, HI_S8* pstPacket, HI_S32* piLen);
static HI_VOID         RTCP_Pack_BYEPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket, HI_S32* piLen);
static VP_RTP_RESULT   RTCP_Parse_SRPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
        VP_RTCP_SR_INFO* pstRtcpSRInfo, HI_S32* piLen,
        HI_U32 ulTime);
static VP_RTP_RESULT RTCP_Parse_RRPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
                                            VP_RTCP_RR_INFO* pstRtcpRRInfo,  HI_S32* piLen, 
                                            HI_U32 ulTime);
static VP_RTP_RESULT   RTCP_Parse_SDESPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
        VP_RTCP_SDES_INFO* pstRtcpSDESInfo,
        HI_S32* piLen);
static VP_RTP_RESULT   RTCP_Parse_BYEPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
        VP_RTCP_BYE_INFO* pstRtcpBYEInfo,
        HI_S32* piLen);
static VP_RTP_RESULT   RTCP_Parse_APPPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
        VP_RTCP_APP_INFO* pstRtcpAPPInfo,
        HI_S32* piLen);
static VP_RTP_RESULT   RTCP_PackPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket);
static VP_RTP_RESULT   RTCP_Update (VP_tRTCPSessionHandle hHandle);
static HI_VOID         RTCP_GenerateCname (HI_CHAR* pszCNAME, HI_S8* piLen);
static HI_U32          RTCP_EstimateInterval (VP_tRTCPSessionHandle hHandle);
static VP_RTP_RESULT   RTCP_SendBye (VP_tRTCPSessionHandle hHandle);
static VP_SOURCE*      RTCP_CreateLocal (VP_RTCP_SESSION* pstSession);
static HI_VOID         RTCP_DeleteLocal (VP_RTCP_SESSION* pstSession);
static HI_VOID         RTCP_SessionInit (VP_RTCP_SESSION* pstSession);

/*平均偏差J(i) = J(i-1) + (|D(i-1,i)| - J(i-1))/16*/
static HI_U32 RTCP_UTILS_MeanDeviation (HI_U32 u32D, HI_U32 u32MD)
{
    return (u32MD + ( (HI_S32) (u32D * RTCP_MEAN_DEVIATION_COUNT - u32MD)) / RTCP_MEAN_DEVIATION_COUNT);
}

static HI_U32 RTCP_UTILS_TimeGetTime (HI_VOID)
{
    HI_U32 u32TimeMs = 0;

    HI_SYS_GetTimeStampMs (&u32TimeMs);

    return  u32TimeMs;

}


static HI_U32 RTCP_UTILS_Rand (HI_VOID)
{
    HI_U32 data = 0;
    HI_S32 fd = 0;
    struct timeval tv = {0};
    /* generate a random integer from /dev/random */    
    fd = open("/dev/urandom", O_RDONLY|O_NONBLOCK);
    if (fd >= 0)
    {
        if(-1 == read (fd,&data,sizeof (HI_U32)))
        {
            data = 0;
        }
        close (fd);
    }
    
    if (data == 0)
    {
        (void)gettimeofday (&tv, HI_NULL);
        srand (RTCP_RAND_INDEX ((HI_U32) tv.tv_usec));
        data = random();
    }
    
    return data;
}

static HI_BOOL RTCP_UTILS_SystemTimeCMP (HI_U32 ulBigTime, HI_U32 ulSmallTime)
{
    HI_U32 ulDifTime = 0;

    if (ulBigTime >= ulSmallTime)
    {
        return HI_TRUE;
    }
    else
    {
        ulDifTime = ulSmallTime - ulBigTime;

        /* revert time */
        if (ulDifTime > 0x80000000)
        {
            return HI_TRUE;
        }
        else
        {
            return HI_FALSE;
        }
    }
}


/* Get local SDEC item */
static VP_RTP_RESULT RTCP_GetSDESItem (VP_RTCP_SESSION* pstSession, HI_S8 uiType, HI_CHAR* pszValue)
{
    HI_CHAR* pszSrcval   = HI_NULL;
    VP_SOURCE* pstSource = HI_NULL;
    HI_U32 ucLen = 0;

    pstSource = pstSession->LOCAL;

    if (pstSource == HI_NULL)
    {
        return RTP_ERR_ERROR;
    }

    switch (uiType)
    {
    case RTCP_SDES_ID_CNAME:
    {
        pszSrcval = pstSource->stRtcpSdesInfo.szCNAME;
        break;
    }

    case RTCP_SDES_ID_NAME:
    {
        pszSrcval = pstSource->stRtcpSdesInfo.szNAME;
        break;
    }

    case RTCP_SDES_ID_EMAIL:
    {
        pszSrcval = pstSource->stRtcpSdesInfo.szEMAIL;
        break;
    }

    case RTCP_SDES_ID_PHONE:
    {
        pszSrcval = pstSource->stRtcpSdesInfo.szPHONE;
        break;
    }

    case RTCP_SDES_ID_LOCATION:
    {
        pszSrcval = pstSource->stRtcpSdesInfo.szLOC;
        break;
    }

    case RTCP_SDES_ID_TOOL:
    {
        pszSrcval = pstSource->stRtcpSdesInfo.szTOOL;
        break;
    }

    case RTCP_SDES_ID_NOTE:
    {
        pszSrcval = pstSource->stRtcpSdesInfo.szNOTE;
        break;
    }

    case RTCP_SDES_ID_PRIVATE:
    {
        pszSrcval = pstSource->stRtcpSdesInfo.szPRIV;
        break;
    }

    default:
    {
        return RTP_ERR_INVALIDPARAM;
    }
    }

    ucLen = (HI_U32) strlen (pszSrcval);
    strncpy (pszValue, pszSrcval, ucLen);
    pszValue[ucLen] = '\0';

    return RTP_ERR_OK;
}

static VP_RTP_RESULT RTCP_SetSDESItem (VP_SOURCE* pstSource, HI_S8 uiType, HI_CHAR* pszValue, HI_S16 iLength)
{
    HI_CHAR* pszDstval = HI_NULL;

    if ( (HI_NULL == pstSource) || (HI_NULL == pszValue))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    switch (uiType)
    {
    case RTCP_SDES_ID_CNAME:
    {
        pszDstval = pstSource->stRtcpSdesInfo.szCNAME;
        break;
    }

    case RTCP_SDES_ID_NAME:
    {
        pszDstval = pstSource->stRtcpSdesInfo.szNAME;
        break;
    }

    case RTCP_SDES_ID_EMAIL:
    {
        pszDstval = pstSource->stRtcpSdesInfo.szEMAIL;
        break;
    }

    case RTCP_SDES_ID_PHONE:
    {
        pszDstval = pstSource->stRtcpSdesInfo.szPHONE;
        break;
    }

    case RTCP_SDES_ID_LOCATION:
    {
        pszDstval = pstSource->stRtcpSdesInfo.szLOC;
        break;
    }

    case RTCP_SDES_ID_TOOL:
    {
        pszDstval = pstSource->stRtcpSdesInfo.szTOOL;
        break;
    }

    case RTCP_SDES_ID_NOTE:
    {
        pszDstval = pstSource->stRtcpSdesInfo.szNOTE;
        break;
    }

    case RTCP_SDES_ID_PRIVATE:
    {
        pszDstval = pstSource->stRtcpSdesInfo.szPRIV;
        break;
    }

    default:
    {
        pszDstval = HI_NULL;
        break;
    }
    }

    if (HI_NULL == pszDstval)
    {
        return RTP_ERR_ERROR;
    }
    else
    {
        if ( (SDES_ITEM_LENGTH - 1) < iLength)
        {
            iLength = SDES_ITEM_LENGTH - 1;
        }

        strncpy (pszDstval, pszValue, iLength);
        pszDstval[iLength] = '\0';
    }

    return RTP_ERR_OK;
}

static VP_RTP_RESULT RTCP_BuildSDESItem (VP_RTCP_SESSION* pstSession, HI_S8 ucType, HI_CHAR* pstItem)
{
    HI_CHAR pszValue[SDES_ITEM_LENGTH] = {0};
    HI_S32 rtpResult = 0;
    HI_S8 ucLen = 0;

    if ( (HI_NULL == pstSession) || (HI_NULL == pstItem))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    rtpResult = RTCP_GetSDESItem (pstSession, ucType, pszValue);

    if (0 != rtpResult)
    {
        return RTP_ERR_ERROR;
    }

    ucLen = (HI_S8) strlen (pszValue);
    *pstItem = (HI_S8) ucType;
    * (pstItem + 1) = ucLen;
    strncpy ( (pstItem + 2), pszValue, ucLen);
    * (pstItem + 2 + ucLen) = '\0';

    return RTP_ERR_OK;
}

static VP_RTP_RESULT RTCP_BuildSDESChunck (VP_RTCP_SESSION* pstSession, VP_RTCP_SDES_CHUNK* pstChunk,
        HI_U16* puiLength)
{
    HI_S8 ucLength = 0;
    HI_U16 uiLen = 0;
    HI_S8 uiNumPads  = 0;
    HI_CHAR* pBuffer = HI_NULL;
    HI_CHAR* pTemp = HI_NULL;
    VP_RTP_RESULT enRet;

    if ( (HI_NULL == pstSession) || (HI_NULL == pstChunk) || (HI_NULL == puiLength))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pBuffer = (HI_CHAR*) pstChunk;
    pTemp = pBuffer;
    enRet = RTCP_BuildSDESItem (pstSession, RTCP_SDES_ID_CNAME, pTemp);

    if (RTP_ERR_OK != enRet)
    {
        *puiLength = 0;
        return RTP_ERR_ERROR;
    }

    ucLength = * (pTemp + 1) + 2;
    uiLen += ucLength;
    pTemp = pBuffer + uiLen;
    enRet = RTCP_BuildSDESItem (pstSession, RTCP_SDES_ID_NAME, pTemp);

    if (RTP_ERR_OK != enRet)
    {
        *puiLength = 0;
        return RTP_ERR_ERROR;
    }

    ucLength = * (pTemp + 1) + 2;
    uiLen += ucLength;
    pTemp = pBuffer + uiLen;
    enRet = RTCP_BuildSDESItem (pstSession, RTCP_SDES_ID_EMAIL, pTemp);

    if (RTP_ERR_OK != enRet)
    {
        *puiLength = 0;
        return RTP_ERR_ERROR;
    }

    ucLength = * (pTemp + 1) + 2;
    uiLen += ucLength;
    pTemp = pBuffer + uiLen;
    enRet = RTCP_BuildSDESItem (pstSession, RTCP_SDES_ID_PHONE, pTemp);

    if (RTP_ERR_OK != enRet)
    {
        *puiLength = 0;
        return RTP_ERR_ERROR;
    }

    ucLength = * (pTemp + 1) + 2;
    uiLen += ucLength;
    pTemp = pBuffer + uiLen;
    enRet = RTCP_BuildSDESItem (pstSession, RTCP_SDES_ID_LOCATION, pTemp);

    if (RTP_ERR_OK != enRet)
    {
        *puiLength = 0;
        return RTP_ERR_ERROR;
    }

    ucLength = * (pTemp + 1) + 2;
    uiLen += ucLength;
    pTemp = pBuffer + uiLen;
    enRet = RTCP_BuildSDESItem (pstSession, RTCP_SDES_ID_TOOL, pTemp);

    if (RTP_ERR_OK != enRet)
    {
        *puiLength = 0;
        return RTP_ERR_ERROR;
    }

    ucLength = * (pTemp + 1) + 2;
    uiLen += ucLength;
    pTemp = pBuffer + uiLen;
    enRet = RTCP_BuildSDESItem (pstSession, RTCP_SDES_ID_NOTE, pTemp);

    if (RTP_ERR_OK != enRet)
    {
        *puiLength = 0;
        return RTP_ERR_ERROR;
    }

    ucLength = * (pTemp + 1) + 2;
    uiLen += ucLength;
    pTemp = pBuffer + uiLen;
    enRet = RTCP_BuildSDESItem (pstSession, RTCP_SDES_ID_PRIVATE, pTemp);

    if (RTP_ERR_OK != enRet)
    {
        *puiLength = 0;
        return RTP_ERR_ERROR;
    }

    ucLength = * (pTemp + 1) + 2;
    uiLen += ucLength;
    pTemp = pBuffer + uiLen;
    uiNumPads = 4 - ( (uiLen % 4) & 0x03);

    while (uiNumPads)
    {
        *pTemp = 0;
        pTemp++;
        uiLen++;
        uiNumPads--;
    }

    *puiLength = uiLen;

    return RTP_ERR_OK;
}

static HI_S32 RTCP_BuildReportBlocks (VP_RTCP_SESSION* pstSession, VP_RTCPREPORTBLOCK* pstReportBlock, HI_U32 ulTime)
{
    HI_BOOL bIsOver   = HI_FALSE;
    HI_S32 iFraction  = 0;
    HI_S32 lTotalLost = 0;
    HI_S32 lIExpected = 0;
    HI_S32 lIReceived = 0;
    HI_S32 lInterLost = 0;
    HI_S32 ulSequence = 0;
    HI_S32 ulExpected = 0;
    HI_U32 ulLSR  = 0;
    HI_U32 ulDLSR = 0;
    HI_S32 iCount = 0;
    HI_S8 i = 0;
    VP_SOURCE* pstSource = HI_NULL;

    if (HI_NULL == pstSession)
    {
        return RTP_ERR_INVALIDPARAM;
    }

    for (i = 0; SOURCE_COUNT > i; ++i)
    {

        pstSource = pstSession->pstSources[i];

        while (HI_NULL != pstSource)
        {
            if ((31 == iCount) || (0 == pstSession->ulSenders)|| (HI_FALSE == pstSource->bIsLocalRecv))
            {
                bIsOver = HI_TRUE;
                break;
            }

            if (pstSource->bIsSender)
            {
                ulSequence = pstSource->ulRTPCycles + pstSource->ulRTPSequence;

                /* number of expect received RTP packets start from session */
                ulExpected = ulSequence - pstSource->ulRTPOrgSequence + 1;

                /* total lost RTP packets start from session */
                lTotalLost = ulExpected - pstSource->ulRTPPackets;

                if (lTotalLost < 0)
                {
                    lTotalLost = 0;
                    break;
                }

                /* number of expect received RTP packets in current time slice */
                lIExpected = ulExpected - pstSource->ulRTPExptPackets;

                /* number of real received RTP packets in current time slice */
                lIReceived = pstSource->ulRTPPackets - pstSource->ulRTPLastPackets;

                /* total lost number of RTP packets in current time slice */
                lInterLost = lIExpected - lIReceived;

                /* save current received RTP packets number */
                pstSource->ulRTPLastPackets = pstSource->ulRTPPackets;

                /* save current reveived RTP packets bytes */
                pstSource->ulRTPLastBytes = pstSource->ulRTPBytes;

                /* save current expected RTP packets number */
                pstSource->ulRTPExptPackets = ulExpected;

                if ( (0 == lIExpected) || (0 >= lInterLost))
                {
                    iFraction = 0;
                }
                else
                {
                    iFraction = (lInterLost << 8) / lIExpected;
                }

                /* get sender information through parsing SR packets */
                ulLSR = ( ( (pstSource->stSenderInfo.ulNTPMSW) & 0xFFFF) << 16) |
                        ( ( (pstSource->stSenderInfo.ulNTPLSW) >> 16) & 0xFFFF);

                ulDLSR = (HI_U32) ( ( (ulTime - pstSource->ulRTCPLastSRTime) * 65536 / 1000));
                pstReportBlock->ulSSRC = htonl (pstSource->ulSSRC);
                pstReportBlock->ulFractLost = (HI_U32) iFraction;
                pstSession->stRtcpStatisticInfo.stSendInfo.uiLossFraction = (HI_U32) (iFraction * 100 / 256);
                pstReportBlock->ulTotalLost = htonl (lTotalLost & 0x00FFFFFF) >> 8;
                pstSession->stRtcpStatisticInfo.stSendInfo.uiTotalLostPacket = lTotalLost;
                pstReportBlock->ulSequence = htonl (ulSequence);
                pstReportBlock->ulJitter = htonl (pstSource->uiRTPJitter / RTCP_MEAN_DEVIATION_COUNT);
                pstReportBlock->ulLSR  = htonl (ulLSR);
                pstReportBlock->ulDLSR = htonl (ulDLSR);
                pstSource->bIsSender = HI_FALSE;
                pstSource->bIsRemoteRecv = HI_FALSE;
                pstSource->bIsLocalRecv = HI_FALSE;
                pstSession->ulSenders--;
                iCount++;
                pstReportBlock++;
            }

            pstSource = pstSource->pstNext;
        }

        if (bIsOver)
        {
            break;
        }
    }

    return iCount;
}

static VP_RTP_RESULT RTCP_UpdateReportBlocks (VP_tRTCPSessionHandle hHandle, HI_U32 ulTime)
{
    HI_S16 i = 0;
    HI_S16 j = 0;
    VP_RTCPREPORTNODE* pstHead = HI_NULL;
    VP_RTCPREPORTNODE* pstCurr = HI_NULL;
    VP_RTCPREPORTNODE* pstTemp = HI_NULL;
    VP_SOURCE* pstSource = HI_NULL;
    VP_RTCP_SESSION* pstSession = HI_NULL;

    if (HI_NULL == hHandle)
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;

    for (i = 0; SOURCE_COUNT > i; ++i)
    {
        pstSource = pstSession->pstSources[i];

        while (HI_NULL != pstSource)
        {
            for (j = 0; SOURCE_COUNT > j; j++)
            {
                pstHead = & (pstSource->stReportBlocks[j]);
                pstCurr = pstHead->pstNext;

                while (pstCurr != pstHead)
                {
                    if ( (ulTime - pstCurr->ulTime) > (pstSession->LOCAL->ulRTCPInterval * 3))
                    {
                        pstTemp = pstCurr;
                        pstCurr = pstCurr->pstPrev;
                        pstTemp->pstPrev->pstNext = pstTemp->pstNext;
                        pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
                        HI_FREE (HI_ID_USR_START, pstTemp);
                        pstTemp = HI_NULL;
                    }

                    pstCurr = pstCurr->pstNext;
                }
            }

            pstSource = pstSource->pstNext;
        }
    }

    return RTP_ERR_OK;
}

static HI_VOID RTCP_RemoveReportBlocks (VP_tRTCPSessionHandle hHandle, HI_U32 ulSSRC)
{
    HI_S8 i = 0;
    HI_S8 j = 0;
    HI_U32 ulIndex = 0;
    VP_RTCPREPORTNODE* pstHead = HI_NULL;
    VP_RTCPREPORTNODE* pstCurr = HI_NULL;
    VP_RTCPREPORTNODE* pstTemp = HI_NULL;
    VP_SOURCE* pstSource = HI_NULL;
    VP_RTCP_SESSION* pstSession = HI_NULL;

    if (HI_NULL == hHandle)
    {
        return;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;

    for (i = 0; SOURCE_COUNT > i; ++i)
    {
        pstSource = pstSession->pstSources[i];

        while (HI_NULL != pstSource)
        {
            if (ulSSRC == pstSource->ulSSRC)
            {
                for (j = 0; SOURCE_COUNT > j; ++j)
                {
                    pstHead = & (pstSource->stReportBlocks[j]);
                    pstCurr = pstHead->pstNext;

                    while (pstCurr != pstHead)
                    {
                        pstTemp = pstCurr;
                        pstCurr = pstCurr->pstPrev;
                        pstTemp->pstPrev->pstNext = pstTemp->pstNext;
                        pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
                        HI_FREE (HI_ID_USR_START, pstTemp);
                        pstCurr = pstCurr->pstNext;
                    }

                    pstSource->stReportBlocks[j].pstNext = & (pstSource->stReportBlocks[j]);
                    pstSource->stReportBlocks[j].pstPrev = & (pstSource->stReportBlocks[j]);
                }
            }
            else
            {
                ulIndex = SOURCE_INDEX (ulSSRC);
                pstHead = & (pstSource->stReportBlocks[ulIndex]);
                pstCurr = pstHead->pstNext;

                while (pstCurr != pstHead)
                {
                    if (ulSSRC == pstCurr->stReportBlock.ulSSRC)
                    {
                        pstTemp = pstCurr;
                        pstCurr = pstCurr->pstPrev;
                        pstTemp->pstPrev->pstNext = pstTemp->pstNext;
                        pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
                        HI_FREE (HI_ID_USR_START, pstTemp);
                        pstTemp = HI_NULL;
                    }

                    pstCurr = pstCurr->pstNext;
                }
            }

            pstSource = pstSource->pstNext;
        }
    }

    return;
}

/* Initialize source when the source is confirmed valid */
static HI_VOID RTCP_InitializeSource (VP_SOURCE* pstSource, HI_U32 ulSequence, HI_U32 ulTimestamp)
{
    if (HI_NULL == pstSource)
    {
        return;
    }

    pstSource->ulRTPOrgSequence  = ulSequence;
    pstSource->ulRTPOrgTimestamp = ulTimestamp;
    pstSource->ulRTPBadSequence = RTP_SEQ_MOD + 1;
    pstSource->ulRTPSequence  = ulSequence;
    pstSource->ulRTPTimestamp = ulTimestamp;
    pstSource->ulRTPCycles  = 0;
    pstSource->ulRTPPackets = 0;
    pstSource->ulRTPBytes = 0;
    pstSource->ulRTPLastPackets = 0;
    pstSource->ulRTPLastBytes   = 0;
    pstSource->ulRTPExptPackets = 0;
    pstSource->ulRTPExptBytes = 0;
}

/* Look up whether the source exists */
static VP_SOURCE* RTCP_LookupSource (VP_RTCP_SESSION* pstSession, HI_U32 ulSSRC)
{
    HI_U32 ulIndex = 0;
    VP_SOURCE* pstSource = HI_NULL;

    ulIndex   = SOURCE_INDEX (ulSSRC);
    pstSource = pstSession->pstSources[ulIndex];

    while (HI_NULL != pstSource)
    {
        if (ulSSRC == pstSource->ulSSRC)
        {
            return pstSource;
        }

        pstSource = pstSource->pstNext;
    }

    return HI_NULL;
}

/* If the source has been created according to SSRC, return the address of source, else create source */
static VP_SOURCE* RTCP_CreateSource (VP_RTCP_SESSION*  pstSession, HI_U32 ulSSRC, HI_BOOL bProbation)
{
    HI_S32 i = 0;
    HI_U32 ulIndex = 0;
    VP_SOURCE* pstSource = HI_NULL;

    if (HI_NULL == pstSession)
    {
        return HI_NULL;
    }

    /* look up relative source according to given SSRC tag */
    pstSource = RTCP_LookupSource (pstSession, ulSSRC);

    /* found */
    if (HI_NULL != pstSource)
    {
        pstSource->ulLastActiveTime = RTCP_UTILS_TimeGetTime();
    }
    /* not found */
    else
    {
        pstSource = (VP_SOURCE*) HI_MALLOC (HI_ID_USR_START, sizeof (VP_SOURCE));

        if (HI_NULL != pstSource)
        {
            memset (pstSource, 0, sizeof (VP_SOURCE));
            pstSource->ulSSRC = ulSSRC;

            /* need checking continuous RTP packets */
            if (bProbation)
            {
                pstSource->iProbation = -1;
            }
            else
            {
                pstSource->iProbation = 0;
            }

            ulIndex = SOURCE_INDEX (ulSSRC);
            pstSource->pstNext = pstSession->pstSources[ulIndex];

            if (HI_NULL != pstSource->pstNext)
            {
                pstSource->pstNext->pstPrev = pstSource;
            }

            pstSession->pstSources[ulIndex] = pstSource;

            for (i = 0; SOURCE_COUNT > i; i++)
            {
                pstSource->stReportBlocks[i].pstNext = & (pstSource->stReportBlocks[i]);
                pstSource->stReportBlocks[i].pstPrev = & (pstSource->stReportBlocks[i]);
            }

            pstSource->ulLastActiveTime = RTCP_UTILS_TimeGetTime();
            pstSource->bRTPFirstSendPacket = HI_TRUE;
            pstSource->bRTPFirstRecvPacket = HI_TRUE;
            pstSession->ulMembers++;
        }
    }

    return pstSource;
}

/* Delete source */
static HI_VOID RTCP_DeleteSource (VP_tRTCPSessionHandle hHandle, HI_U32 ulSSRC)
{
    HI_U32 ulIndex = 0;
//    HI_U32 ulTime = 0;
    VP_SOURCE*     pstSource = HI_NULL;
    VP_RTCP_SESSION* pstSession = HI_NULL;

    if (HI_NULL == hHandle)
    {
        return;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;

//    ulTime  = RTCP_UTILS_TimeGetTime();
    ulIndex = SOURCE_INDEX (ulSSRC);

    /* look up relative source according to given SSRC tag */
    pstSource = RTCP_LookupSource (pstSession, ulSSRC);

    if (HI_NULL == pstSource)
    {
        return;
    }

    RTCP_RemoveReportBlocks (pstSession, ulSSRC);

    if (pstSession->pstSources[ulIndex] == pstSource)
    {
        pstSession->pstSources[ulIndex] = pstSource->pstNext;

        if (HI_NULL != pstSource->pstNext)
        {
            pstSource->pstNext->pstPrev = HI_NULL;
        }
    }
    else
    {
        pstSource->pstPrev->pstNext = pstSource->pstNext;

        if (HI_NULL != pstSource->pstNext)
        {
            pstSource->pstNext->pstPrev = pstSource->pstPrev;
        }
    }

    pstSession->ulMembers--;

    if (pstSession->ulMembers < pstSession->ulLastMembers)
    {
        pstSession->ulLastMembers = pstSession->ulMembers;
    }

    if (HI_TRUE == pstSource->bNeedSDES)
    {
        pstSession->ulCSRCs--;
    }

    if (pstSession->ulLastNotifyCSRC == pstSession->ulCSRCs)
    {
        pstSession->ulLastNotifyCSRC = 0;
    }

    HI_FREE (HI_ID_USR_START, pstSource);

    pstSource = HI_NULL;

    return;
}

/* Update squence and time stamp in source */
static HI_BOOL RTCP_UpdateSource (VP_SOURCE* pstSource, HI_U32 ulSequence, HI_U32 ulTimestamp)
{
    HI_S32 lDelta;

    if (HI_NULL == pstSource)
    {
        return HI_FALSE;
    }

    /* calculate sequence different of RTP packet */
    lDelta = ulSequence - pstSource->ulRTPSequence;

    /* check continuous RTP packets */
    if (0 < pstSource->iProbation)
    {
        if (ulSequence == (pstSource->ulRTPSequence + 1))
        {
            pstSource->iProbation--;
        }
        else
        {
            pstSource->iProbation = MIN_SEQUENTIAL - 1;
        }

        pstSource->ulRTPSequence  = ulSequence;
        pstSource->ulRTPTimestamp = ulTimestamp;

        if (0 == pstSource->iProbation)
        {
            RTCP_InitializeSource (pstSource, ulSequence, ulTimestamp);
        }
        else
        {
            return HI_FALSE;
        }
    }
    /* the number of lost packet is less than MAX_DROPOUT */
    else if (lDelta < MAX_DROPOUT)
    {
        if (ulSequence < pstSource->ulRTPSequence)
        {
            /* disorder */
            if (ulSequence + DISORDER_THRESHOLD >= pstSource->ulRTPSequence)
            {
                ;
            }
            /* revert */
            else
            {
                pstSource->ulRTPCycles += RTP_SEQ_MOD;
            }
        }

        pstSource->ulRTPSequence  = ulSequence;
        pstSource->ulRTPTimestamp = ulTimestamp;
    }
    /* the number of lost packet is out of permission range */
    else if (lDelta <= RTP_SEQ_MOD - MAX_DISORDER)
    {
        if (ulSequence == pstSource->ulRTPBadSequence)
        {
            RTCP_InitializeSource (pstSource, ulSequence, ulTimestamp);
        }
        else
        {
            pstSource->ulRTPBadSequence = (ulSequence + 1) & (RTP_SEQ_MOD - 1);
            return HI_FALSE;
        }
    }
    else
    {
        /* repeat or lost packets, actually, the UDP packets will not repeat */
        ;
    }

    return HI_TRUE;
}

static HI_VOID RTCP_Pack_SRPacket (VP_tRTCPSessionHandle hHandle, HI_S8* pstPacket, HI_S32* piLen)
{
    HI_U32 ulOffset   = 0;
    HI_U32 ulTempData = 0;
    HI_S32 iCount   = 0;
    HI_U32 ulTime   = 0;
    HI_U32 ulNTPMSW = 0;
    HI_U32 ulNTPLSW = 0;
    HI_S32 i = 0;
    static HI_S32 u32Num = 0;

    VP_RTCP_SR_INFO pstRtcpSRInfo;
    VP_RTCP_SESSION* pRtpSession = HI_NULL;
    HI_S8* pPacketData = pstPacket;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket))
    {
        return;
    }

    memset (g_RtcpReprotBlock, 0, sizeof (VP_RTCPREPORTBLOCK) * SOURCE_COUNT);
    memset (&pstRtcpSRInfo, 0, sizeof (VP_RTCP_SR_INFO));
    pRtpSession = (VP_RTCP_SESSION*) hHandle;
    ulTime   = RTCP_UTILS_TimeGetTime();
    iCount   = RTCP_BuildReportBlocks (pRtpSession, g_RtcpReprotBlock, ulTime);
    ulNTPMSW = (HI_U32) ( (ulTime / 1000) + SECS_1900_TO_1970);
    ulNTPLSW = (HI_U32) ( (ulTime % 1000) * 4294967); //1ms <=> 4294967.295

    g_RtcpReprotTime[u32Num % RTCP_REPROT_TIME_MAX][0] = ( (ulNTPMSW & 0xFFFF) << 16) | ( (ulNTPLSW >> 16) & 0xFFFF);
    g_RtcpReprotTime[u32Num % RTCP_REPROT_TIME_MAX][1] = ulTime;
    u32Num++;

    pstRtcpSRInfo.stRtcpHeader.ucVersion = RTCP_VERSION;
    pstRtcpSRInfo.stRtcpHeader.ucPadding = 0;
    pstRtcpSRInfo.stRtcpHeader.ucCount = (HI_S8) iCount;
    pstRtcpSRInfo.stRtcpHeader.ucPacketType = RTCP_SR;
    pstRtcpSRInfo.stRtcpHeader.usLength = (HI_U16) (6 + (6 * iCount));
    pstRtcpSRInfo.ulSSRC   = pRtpSession->LOCAL->ulSSRC;
    pstRtcpSRInfo.ulNTPMSW = ulNTPMSW;
    pstRtcpSRInfo.ulNTPLSW = ulNTPLSW;
    pstRtcpSRInfo.ulRtpTimestamp = RTP_PTS_MS_TO_90MS (ulTime);
    pstRtcpSRInfo.ulPacketSendCount = pRtpSession->LOCAL->ulRTPPackets;
    pstRtcpSRInfo.ulByteSendCount = pRtpSession->LOCAL->ulRTPBytes;

    if (pstRtcpSRInfo.stRtcpHeader.ucVersion == 2)
    {
        ulTempData |= 0x80000000;
    }

    if (pstRtcpSRInfo.stRtcpHeader.ucPadding == 1)
    {
        ulTempData |= 0x20000000;
    }

    ulTempData |= ( (pstRtcpSRInfo.stRtcpHeader.ucCount << 24) & 0x1f000000);
    ulTempData |= ( (pstRtcpSRInfo.stRtcpHeader.ucPacketType << 16) & 0x00ff0000);
    ulTempData |= (pstRtcpSRInfo.stRtcpHeader.usLength & 0x0000ffff);

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    ulTempData = pstRtcpSRInfo.ulSSRC;

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    ulTempData = (HI_U32) (pstRtcpSRInfo.ulNTPMSW);

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    ulTempData = (HI_U32) (pstRtcpSRInfo.ulNTPLSW);

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    /* suspicious code: the time stamp of SR packet should be consistent with NTP time */
    /* to synchronise with time, but here we use the last time stamp of sended RTP packet */
    ulTempData = (HI_U32) (pstRtcpSRInfo.ulRtpTimestamp);
    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    ulTempData = pstRtcpSRInfo.ulPacketSendCount;

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    ulTempData = pstRtcpSRInfo.ulByteSendCount;

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    for (i = 0; i < iCount; ++i)
    {
        memcpy (pPacketData + ulOffset, &g_RtcpReprotBlock[i], 24);
        ulOffset += 24;
    }

    *piLen = (pstRtcpSRInfo.stRtcpHeader.usLength + 1) * 4;
}

static HI_VOID RTCP_Pack_RRPacket (VP_tRTCPSessionHandle hHandle, HI_S8* pstPacket, HI_S32* piLen)
{
    HI_U32 ulOffset   = 0;
    HI_S32 ulTempData = 0;
    HI_U32 ulTime = 0;
    HI_S32 iCount = 0;
    HI_S32 i = 0;
    HI_S8* pPacketData = pstPacket;
    VP_RTCP_RR_INFO pstRtcpRRInfo;
    VP_RTCP_SESSION* pSession = HI_NULL;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket))
    {
        return;
    }

    memset ( (HI_VOID*) (&g_RtcpReprotBlock[0]), 0, sizeof (VP_RTCPREPORTBLOCK) * SOURCE_COUNT);
    memset (&pstRtcpRRInfo, 0, sizeof (VP_RTCP_RR_INFO));

    pSession = (VP_RTCP_SESSION*) hHandle;
    ulTime = RTCP_UTILS_TimeGetTime();
    iCount = RTCP_BuildReportBlocks (pSession, g_RtcpReprotBlock, ulTime);
    pstRtcpRRInfo.stRtcpHeader.ucCount  = (HI_S8) iCount;
    pstRtcpRRInfo.stRtcpHeader.usLength = (HI_U16) ( (6 * iCount) + 1);
    pstRtcpRRInfo.ulSSRC = pSession->LOCAL->ulSSRC;
    pstRtcpRRInfo.stRtcpHeader.ucPacketType = RTCP_RR;
    pstRtcpRRInfo.stRtcpHeader.ucVersion = RTCP_VERSION;
    pstRtcpRRInfo.stRtcpHeader.ucPadding = 0;

    if (pstRtcpRRInfo.stRtcpHeader.ucVersion == 2)
    {
        ulTempData |= 0x80000000;
    }

    if (pstRtcpRRInfo.stRtcpHeader.ucPadding == 1)
    {
        ulTempData |= 0x20000000;
    }

    ulTempData |= (pstRtcpRRInfo.stRtcpHeader.ucCount << 24) & 0x1f000000;
    ulTempData |= (pstRtcpRRInfo.stRtcpHeader.ucPacketType << 16) & 0x00ff0000;
    ulTempData |= (pstRtcpRRInfo.stRtcpHeader.usLength & 0x0000ffff);
    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    ulTempData = pstRtcpRRInfo.ulSSRC;

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    for (i = 0; i < iCount; ++i)
    {
        memcpy (pPacketData + ulOffset, &g_RtcpReprotBlock[i], 24);
        ulOffset += 24;
    }

    *piLen = (pstRtcpRRInfo.stRtcpHeader.usLength + 1) * 4;
}

static VP_RTP_RESULT RTCP_Pack_SDESPacket (VP_tRTCPSessionHandle hHandle, HI_S8* pstPacket, HI_S32* piLen)
{
    HI_U32 ulOffset   = 0;
    HI_U32 ulTempData = 0;
    HI_U16 uiLen = 0;
    VP_RTP_RESULT enRet;
    VP_RTCP_SDES_INFO pstRtcpSDESInfo;
    VP_RTCP_SESSION* pstRtpSession = HI_NULL;
    HI_S8* pPacketData = pstPacket;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pstRtpSession = (VP_RTCP_SESSION*) hHandle;
    memset (&g_stSdesChunk, 0, sizeof (VP_RTCP_SDES_CHUNK));
    memset (&pstRtcpSDESInfo, 0, sizeof (VP_RTCP_SDES_INFO));

    enRet = RTCP_BuildSDESChunck (pstRtpSession, &g_stSdesChunk, &uiLen);

    if (RTP_ERR_OK != enRet)
    {
        return RTP_ERR_ERROR;
    }

    pstRtcpSDESInfo.stRtcpHeader.ucVersion = RTCP_VERSION;
    pstRtcpSDESInfo.stRtcpHeader.ucPadding = 0;
    pstRtcpSDESInfo.stRtcpHeader.ucCount = 1;
    pstRtcpSDESInfo.stRtcpHeader.ucPacketType = RTCP_SDES;

    /* the length of chunk must be 4 aligned */
    pstRtcpSDESInfo.stRtcpHeader.usLength = 1 + (uiLen >> 2);
    pstRtcpSDESInfo.ulSSRC = pstRtpSession->LOCAL->ulSSRC;

    if (pstRtcpSDESInfo.stRtcpHeader.ucVersion == 2)
    {
        ulTempData |= 0x80000000;
    }

    if (pstRtcpSDESInfo.stRtcpHeader.ucPadding == 1)
    {
        ulTempData |= 0x20000000;
    }

    ulTempData |= ( (pstRtcpSDESInfo.stRtcpHeader.ucCount << 24) & 0x1f000000);
    ulTempData |= ( (pstRtcpSDESInfo.stRtcpHeader.ucPacketType << 16) & 0x00ff0000);
    ulTempData |= (pstRtcpSDESInfo.stRtcpHeader.usLength & 0x0000ffff);

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    ulTempData = pstRtcpSDESInfo.ulSSRC;
    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    memcpy (pPacketData + ulOffset, &g_stSdesChunk, uiLen);
    ulOffset += uiLen;

    *piLen = ulOffset;

    return RTP_ERR_OK;
}

static HI_VOID RTCP_Pack_APPPacket (VP_tRTCPSessionHandle hHandle, HI_S8* pstPacket, HI_S32* piLen)
{
    HI_S32 iLength    = 0;
    HI_U32 ulOffset   = 0;
    HI_S32 ulTempData = 0;
    VP_RTCP_APP_INFO pstRtcpAPPInfo;
    VP_RTCP_SESSION* pstSession = HI_NULL;
    HI_S8* pPacketData = pstPacket;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket))
    {
        return;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;
    memset (&pstRtcpAPPInfo, 0, sizeof (VP_RTCP_APP_INFO));

    HI_RTCP_LOCK();

    iLength = pstSession->stSendAppInfo.iLength;
    pstRtcpAPPInfo.stRtcpHeader.ucVersion = RTCP_VERSION;
    pstRtcpAPPInfo.stRtcpHeader.ucPadding = 0;
    pstRtcpAPPInfo.stRtcpHeader.ucCount = pstSession->stSendAppInfo.stRtcpHeader.ucCount;
    pstRtcpAPPInfo.stRtcpHeader.ucPacketType = RTCP_APP;
    pstRtcpAPPInfo.stRtcpHeader.usLength = (HI_S16) (2 + (iLength >> 2));

    if (pstRtcpAPPInfo.stRtcpHeader.ucVersion == 2)
    {
        ulTempData |= 0x80000000;
    }

    if (pstRtcpAPPInfo.stRtcpHeader.ucPadding == 1)
    {
        ulTempData |= 0x20000000;
    }

    ulTempData |= ( (pstRtcpAPPInfo.stRtcpHeader.ucCount << 24) & 0x1f000000);
    ulTempData |= ( (pstRtcpAPPInfo.stRtcpHeader.ucPacketType << 16) & 0x00ff0000);
    ulTempData |= (pstRtcpAPPInfo.stRtcpHeader.usLength & 0x0000ffff);

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    pstRtcpAPPInfo.ulSSRC = pstSession->LOCAL->ulSSRC;
    ulTempData = pstRtcpAPPInfo.ulSSRC;

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    memcpy (pPacketData + ulOffset, pstSession->stSendAppInfo.pcName, 4);
    ulOffset += 4;

    memcpy (pPacketData + ulOffset, pstSession->stSendAppInfo.pcData, (size_t) iLength);

    HI_RTCP_UNLOCK();
    ulOffset += iLength;
    *piLen = ulOffset;
}

static HI_VOID RTCP_Pack_BYEPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket, HI_S32* piLen)
{
    HI_U32 ulOffset   = 0;
    HI_S32 ulTempData = 0;
    HI_S8 i = 0;
    VP_RTCP_SESSION* pstSession = HI_NULL;
    VP_RTCP_BYE_INFO pstRtcpBYEInfo;
    HI_S8* pPacketData = (HI_S8*) pstPacket;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket))
    {
        return;
    }

    memset (&pstRtcpBYEInfo, 0, sizeof (VP_RTCP_BYE_INFO));

    pstSession = (VP_RTCP_SESSION*) hHandle;
    pstRtcpBYEInfo.stRtcpHeader.ucVersion = RTCP_VERSION;
    pstRtcpBYEInfo.stRtcpHeader.ucPadding = 0;
    pstRtcpBYEInfo.stRtcpHeader.ucCount = 1 + pstSession->ulCSRCs;
    pstRtcpBYEInfo.stRtcpHeader.ucPacketType = RTCP_BYE;
    pstRtcpBYEInfo.stRtcpHeader.usLength = 1;

    for (i = 0; i < pstSession->ulCSRCs; i++)
    {
        pstRtcpBYEInfo.ulCSRC[i + 1] = pstSession->pulCSRC[i];
    }

    if (pstRtcpBYEInfo.stRtcpHeader.ucVersion == 2)
    {
        ulTempData |= 0x80000000;
    }

    if (pstRtcpBYEInfo.stRtcpHeader.ucPadding == 1)
    {
        ulTempData |= 0x20000000;
    }

    ulTempData |= ( (pstRtcpBYEInfo.stRtcpHeader.ucCount << 24) & 0x1f000000);
    ulTempData |= ( (pstRtcpBYEInfo.stRtcpHeader.ucPacketType << 16) & 0x00ff0000);
    ulTempData |= (pstRtcpBYEInfo.stRtcpHeader.usLength & 0x0000ffff);

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    pstRtcpBYEInfo.ulCSRC[0] = pstSession->LOCAL->ulSSRC;
    ulTempData = pstRtcpBYEInfo.ulCSRC[0];

    ulTempData = htonl (ulTempData);
    memcpy (pPacketData + ulOffset, &ulTempData, 4);
    ulOffset += 4;

    for (i = 0; i < pstSession->ulCSRCs; ++i)
    {
        ulTempData = pstRtcpBYEInfo.ulCSRC[i + 1];
        ulTempData = htonl (ulTempData);
        memcpy (pPacketData + ulOffset, &ulTempData, 4);
        ulOffset += 4;
    }

    *piLen = ulOffset;
}

static VP_RTP_RESULT RTCP_Parse_SRPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
        VP_RTCP_SR_INFO* pstRtcpSRInfo, HI_S32* piLen,
        HI_U32 ulTime)
{
    HI_U16 usOffset   = 0;
    HI_U32 ulTempData = 0;
    HI_S8 ucTempRecReportCount = 0;
    HI_S8 ucCycNumber = 0;
    HI_U32 ulTemp = 0;
    HI_S8* pPacketData = HI_NULL;
    VP_RTCP_SR_INFO* pstSRInfo   = HI_NULL;
    VP_RTCP_SESSION* pRtpSession = HI_NULL;
    VP_SOURCE* pstSource = HI_NULL;
    HI_U32 i = 0;
    HI_U32 u32NTP_LocalPrev = 0;
    HI_U32 u32NTP_LocalCurrent = 0;
    HI_U32 u32RtpTimestampPrev = 0;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket) || (HI_NULL == pstRtcpSRInfo))
    {
        return RTP_ERR_INVALIDPARAM;
    }


    pPacketData = (HI_S8*) (pstPacket);
    pstSRInfo = pstRtcpSRInfo;

    pRtpSession = (VP_RTCP_SESSION*) hHandle;

    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));

    if ( (ulTempData & 0xc0000000) != 0x80000000)
    {
        return RTP_ERR_PACKETILLEGAL;
    }
    else
    {
        pstSRInfo->stRtcpHeader.ucVersion = 2;
    }

    if ( (ulTempData & 0x20000000) == 0x20000000)
    {
        pstSRInfo->stRtcpHeader.ucPadding = 1;
    }
    else
    {
        pstSRInfo->stRtcpHeader.ucPadding = 0;
    }

    pstSRInfo->stRtcpHeader.ucCount = (HI_S8) ( (ulTempData & 0x1fffffff) >> 24);
    pstSRInfo->stRtcpHeader.ucPacketType = (HI_S8) ( (ulTempData & 0x00ffffff) >> 16);
    pstSRInfo->stRtcpHeader.usLength = (HI_U16) ( (ulTempData & 0x0000ffff));
    usOffset  += 4;
    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
    pstSRInfo->ulSSRC = ulTempData;

    /* can not find relative source */
    pstSource = RTCP_CreateSource (pRtpSession, pstSRInfo->ulSSRC, HI_FALSE);

    if (HI_NULL == pstSource)
    {
        return RTP_ERR_ERROR;
    }

    usOffset += 4;

    if (0 != pstSource->stSenderInfo.ulNTPMSW)
    {
        u32NTP_LocalPrev = ( (pstSource->stSenderInfo.ulNTPMSW - SECS_1900_TO_1970) * 1000) \
                           + (pstSource->stSenderInfo.ulNTPLSW / 4294967);
        u32RtpTimestampPrev =  pstSource->stSenderInfo.ulTimestamp;
    }

    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
    pstSRInfo->ulNTPMSW = ulTempData;
    pstSource->stSenderInfo.ulNTPMSW = ulTempData;
    usOffset += 4;

    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
    pstSRInfo->ulNTPLSW = ulTempData;
    pstSource->stSenderInfo.ulNTPLSW = ulTempData;
    usOffset += 4;

    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));


    pstSRInfo->ulRtpTimestamp = ulTempData;
    pstSource->stSenderInfo.ulTimestamp = ulTempData;
    usOffset += 4;

    /*本地时间单位与发送端时间单位比*/
    if ( (0 != u32NTP_LocalPrev) && (0 != u32RtpTimestampPrev))
    {
        u32NTP_LocalCurrent = ( (pstSource->stSenderInfo.ulNTPMSW - SECS_1900_TO_1970) * 1000) \
                              + (pstSource->stSenderInfo.ulNTPLSW / 4294967);

        if (u32NTP_LocalCurrent != u32NTP_LocalPrev)
        {
            pstSource->ulTimeRatio  = (pstSource->stSenderInfo.ulTimestamp - u32RtpTimestampPrev) \
                                      / (u32NTP_LocalCurrent - u32NTP_LocalPrev);

        }

    }


    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
    pstSRInfo->ulPacketSendCount = ulTempData;
    pstSource->stSenderInfo.ulPackets = ulTempData;
    usOffset += 4;

    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
    pstSRInfo->ulByteSendCount = ulTempData;
    pstSource->stSenderInfo.ulBytes = ulTempData;
    usOffset += 4;

    if (!pstSource->bIsSender)
    {
        pstSource->bIsSender = HI_TRUE;
        pRtpSession->ulSenders++;
    }

    pstSource->ulRTCPLastSRTime = ulTime;

    /* if there are many receiver report block, statistical information will only record last information of report block */
    for (ucTempRecReportCount = pstSRInfo->stRtcpHeader.ucCount; ucTempRecReportCount > 0; ucTempRecReportCount--)
    {
        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstSRInfo->stRecReportInfo[ucCycNumber].SSRC = ulTempData;
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        ulTemp = ( (ulTempData & 0xff000000) >> 24) * 100 / 256;
        pstSRInfo->stRecReportInfo[ucCycNumber].ucLostFraction = (HI_S8) ulTemp;
        pRtpSession->stRtcpStatisticInfo.stRecvInfo.uiLossFraction = (HI_S8) ulTemp;

        pstSRInfo->stRecReportInfo[ucCycNumber].ulPacketLostCount = (HI_U32) (ulTempData & 0x00ffffff);
        pRtpSession->stRtcpStatisticInfo.stRecvInfo.uiTotalLostPacket = (HI_U32) (ulTempData & 0x00ffffff);
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstSRInfo->stRecReportInfo[ucCycNumber].ulExtendedHighestSn = ulTempData;
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstSRInfo->stRecReportInfo[ucCycNumber].ulJitter = ulTempData;
        pRtpSession->stRtcpStatisticInfo.stSendInfo.uiJitter = ulTempData;
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstSRInfo->stRecReportInfo[ucCycNumber].ulLastSR = ulTempData;
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstSRInfo->stRecReportInfo[ucCycNumber].ulDelaySinceLastSR = ulTempData;
        usOffset += 4;

        if ( (pstSRInfo->stRecReportInfo[ucCycNumber].SSRC == pRtpSession->LOCAL->ulSSRC) \
              && (0 != pstSRInfo->stRecReportInfo[ucCycNumber].ulLastSR)\
              && (0 != pstSRInfo->stRecReportInfo[ucCycNumber].ulDelaySinceLastSR))
        {
            ulTempData = 0;

            for (i = 0; i < RTCP_REPROT_TIME_MAX; i++)
            {

                if (g_RtcpReprotTime[i][0] == pstSRInfo->stRecReportInfo[ucCycNumber].ulLastSR)
                {
                    ulTempData = pstSource->ulRTCPLastSRTime - g_RtcpReprotTime[i][1] \
                                 - pstSRInfo->stRecReportInfo[ucCycNumber].ulDelaySinceLastSR * 1000 / 65536;

                    pstSource->ulSendNetDelay  = (ulTempData > 0) ? (ulTempData / 2) : 0;
                    HI_INFO_RTP("NetDelay:%d\n", pstSource->ulSendNetDelay);      /*libo@201312*/

                    break;
                }
            }
            pstSource->bIsRemoteRecv = HI_TRUE;

            if ( (0 != ulTempData) && (0 != pstSource->ulTimeRatio))
            {
                pstSource->ulTimeOffest = pstSource->ulRTCPLastSRTime - ulTempData / 2 - (pstSource->stSenderInfo.ulTimestamp / pstSource->ulTimeRatio);
            }


        }

        ucCycNumber++;
    }

    *piLen = usOffset;

    return RTP_ERR_OK;
}

static VP_RTP_RESULT RTCP_Parse_RRPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
                                            VP_RTCP_RR_INFO* pstRtcpRRInfo,  HI_S32* piLen, HI_U32 ulTime)
{
    HI_U16 usOffset   = 0;
    HI_U32 ulTempData = 0;
    HI_S8 ucCycNumber = 0;
    HI_S8 ucTempRecReportCount = 0;
    VP_RTCP_RR_INFO* pstRRInfo = HI_NULL;
    HI_S8* pPacketData = HI_NULL;
    VP_RTCP_SESSION* pRtpSession = HI_NULL;
    VP_SOURCE*      pstSource = HI_NULL;
    HI_U32 i = 0;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket) || (HI_NULL == pstRtcpRRInfo))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pstRRInfo   = pstRtcpRRInfo;
    pPacketData = (HI_S8*) pstPacket;
    pRtpSession = (VP_RTCP_SESSION*) hHandle;

    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));

    if ( (ulTempData & 0xc0000000) != 0x80000000)
    {
        return RTP_ERR_PACKETILLEGAL;
    }

    pstRRInfo->stRtcpHeader.ucVersion = 2;

    if ( (ulTempData & 0x20000000) == 0x20000000)
    {
        pstRRInfo->stRtcpHeader.ucPadding = 1;
    }
    else
    {
        pstRRInfo->stRtcpHeader.ucPadding = 0;
    }

    pstRRInfo->stRtcpHeader.ucCount = (HI_S8) ( (ulTempData & 0x1fffffff) >> 24);
    pstRRInfo->stRtcpHeader.ucPacketType = (HI_S8) ( (ulTempData & 0x00ffffff) >> 16);
    pstRRInfo->stRtcpHeader.usLength = (HI_U16) (ulTempData & 0x0000ffff);
    usOffset += 4;

    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
    pstRRInfo->ulSSRC = ulTempData;
    pstSource = RTCP_CreateSource (pRtpSession, pstRRInfo->ulSSRC, HI_FALSE);

    if (HI_NULL == pstSource)
    {
        return RTP_ERR_ERROR;
    }

    usOffset += 4;

    for (ucTempRecReportCount = pstRRInfo->stRtcpHeader.ucCount; ucTempRecReportCount > 0; ucTempRecReportCount--)
    {
        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstRRInfo->stRecReportInfo[ucCycNumber].SSRC = ulTempData;
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstRRInfo->stRecReportInfo[ucCycNumber].ucLostFraction = (HI_S8) ( (ulTempData & 0xffffffff) >> 24);
        pstRRInfo->stRecReportInfo[ucCycNumber].ulPacketLostCount = (HI_U32) (ulTempData & 0x00ffffff);
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstRRInfo->stRecReportInfo[ucCycNumber].ulExtendedHighestSn = ulTempData;
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstRRInfo->stRecReportInfo[ucCycNumber].ulJitter = ulTempData;
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstRRInfo->stRecReportInfo[ucCycNumber].ulLastSR = ulTempData;
        usOffset += 4;

        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstRRInfo->stRecReportInfo[ucCycNumber].ulDelaySinceLastSR = ulTempData;
        usOffset += 4;

        if ( (pstRRInfo->stRecReportInfo[ucCycNumber].SSRC == pRtpSession->LOCAL->ulSSRC) \
        && (0 != pstRRInfo->stRecReportInfo[ucCycNumber].ulLastSR)\
        && (0 != pstRRInfo->stRecReportInfo[ucCycNumber].ulDelaySinceLastSR))
        {
            ulTempData = 0;

            for (i = 0; i < RTCP_REPROT_TIME_MAX; i++)
            {
                if (g_RtcpReprotTime[i][0] == pstRRInfo->stRecReportInfo[ucCycNumber].ulLastSR)
                {
                    ulTempData = ulTime - g_RtcpReprotTime[i][1] \
                                 - pstRRInfo->stRecReportInfo[ucCycNumber].ulDelaySinceLastSR * 1000 / 65536;

                    pstSource->ulSendNetDelay  = (ulTempData > 0) ? (ulTempData / 2) : 0;
                    HI_INFO_RTP("NetDelay:%d\n", pstSource->ulSendNetDelay);     
                    break;
                }
            }
            pstSource->bIsRemoteRecv = HI_TRUE;
            if ( (0 != ulTempData) && (0 != pstSource->ulTimeRatio))
            {
                pstSource->ulTimeOffest = pstSource->ulRTCPLastSRTime - ulTempData / 2 - (pstSource->stSenderInfo.ulTimestamp / pstSource->ulTimeRatio);
            }
        }

        ucCycNumber++;
    }

    *piLen = usOffset;

    return RTP_ERR_OK;
}

static VP_RTP_RESULT RTCP_Parse_SDESPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
        VP_RTCP_SDES_INFO* pstRtcpSDESInfo,
        HI_S32* piLen)
{
    HI_S32 iPacketLen = 0;
    HI_S16 iLength  = 0;
    HI_U16 usOffset = 0;
    HI_U32 i = 0;
    HI_U32 ulTempData = 0;
    HI_CHAR* pszTemp = HI_NULL;
    VP_SOURCE* pstSource = HI_NULL;
    VP_RTCP_SESSION* pRtpSession = HI_NULL;
    HI_CHAR* pPacketData = (HI_CHAR*) pstPacket;
    VP_RTP_RESULT enRet;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket) || (HI_NULL == pstRtcpSDESInfo))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pRtpSession = (VP_RTCP_SESSION*) hHandle;
    ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));

    if ( (ulTempData & 0xc0000000) != 0x80000000)
    {
        return RTP_ERR_PACKETILLEGAL;
    }
    else
    {
        pstRtcpSDESInfo->stRtcpHeader.ucVersion = 2;
    }

    if ( (ulTempData & 0x20000000) == 0x20000000)
    {
        pstRtcpSDESInfo->stRtcpHeader.ucPadding = 1;
    }
    else
    {
        pstRtcpSDESInfo->stRtcpHeader.ucPadding = 0;
    }

    pstRtcpSDESInfo->stRtcpHeader.ucCount = (HI_S8) ( (ulTempData & 0x1fffffff) >> 24);
    pstRtcpSDESInfo->stRtcpHeader.ucPacketType = (HI_S8) ( (ulTempData & 0x00ffffff) >> 16);
    pstRtcpSDESInfo->stRtcpHeader.usLength = (HI_U16) ( (ulTempData & 0x0000ffff));

    iPacketLen = (pstRtcpSDESInfo->stRtcpHeader.usLength + 1) << 2;
    *piLen    = iPacketLen;
    usOffset += 4;

    for (i = 0; i < pstRtcpSDESInfo->stRtcpHeader.ucCount; ++i)
    {
        ulTempData = ntohl (* (HI_U32*) (pPacketData + usOffset));
        pstRtcpSDESInfo->ulSSRC = ulTempData;

        pstSource = RTCP_CreateSource (pRtpSession, pstRtcpSDESInfo->ulSSRC, HI_FALSE);

        if (HI_NULL == pstSource)
        {
            return RTP_ERR_ERROR;
        }

        usOffset += 4;

        iPacketLen -= usOffset;
        pszTemp = pPacketData + usOffset;

        while ( (iPacketLen > 0) && (0 != *pszTemp))
        {
            iLength = * (pszTemp + 1);

            if (iLength <= 0)
            {
                break;
            }

            HI_RTCP_LOCK();

            enRet = RTCP_SetSDESItem (pstSource, *pszTemp, (pszTemp + 2), iLength);

            if (RTP_ERR_OK != enRet)
            {
                HI_RTCP_UNLOCK();
                return RTP_ERR_ERROR;
            }

            HI_RTCP_UNLOCK();

            pszTemp = (pszTemp + 2 + iLength);

            iPacketLen -= (2 + iLength);
        }
    }

    return RTP_ERR_OK;
}

static VP_RTP_RESULT  RTCP_Parse_BYEPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
        VP_RTCP_BYE_INFO* pstRtcpBYEInfo,
        HI_S32* piLen)
{
    HI_S8 i = 0;
    HI_U32 ulCSRC     = 0;
    HI_U32 ulOffset   = 0;
    HI_S32 ulTempData = 0;
    VP_RTCP_SESSION* pstSession = HI_NULL;
    VP_SOURCE* pstSource = HI_NULL;
    HI_S8* pPacketData = (HI_S8*) pstPacket;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket) || (HI_NULL == pstRtcpBYEInfo) || (HI_NULL == piLen))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;

    ulTempData = ntohl (* (HI_U32*) (pPacketData + ulOffset));

    if ( (ulTempData & 0xc0000000) != 0x80000000)
    {
        return RTP_ERR_PACKETILLEGAL;
    }
    else
    {
        pstRtcpBYEInfo->stRtcpHeader.ucVersion = 2;
    }

    if ( (ulTempData & 0x20000000) == 0x20000000)
    {
        pstRtcpBYEInfo->stRtcpHeader.ucPadding = 1;
    }
    else
    {
        pstRtcpBYEInfo->stRtcpHeader.ucPadding = 0;
    }

    pstRtcpBYEInfo->stRtcpHeader.ucCount = (HI_S8) ( (ulTempData & 0x1fffffff) >> 24);
    pstRtcpBYEInfo->stRtcpHeader.ucPacketType = (HI_S8) ( (ulTempData & 0x00ffffff) >> 16);
    pstRtcpBYEInfo->stRtcpHeader.usLength = (HI_U16) ( (ulTempData & 0x0000ffff));
    ulOffset += 4;

    for (i = 0; i < pstRtcpBYEInfo->stRtcpHeader.ucCount; i++)
    {
        ulCSRC = ntohl (pstRtcpBYEInfo->ulCSRC[i]);

        pstSource = RTCP_CreateSource (pstSession, ulCSRC, HI_FALSE);

        if (HI_NULL != pstSource)
        {
            pstSource->bIsLeaver = HI_TRUE;
            pstSession->ulLeavers++;
        }
    }

    *piLen = (pstRtcpBYEInfo->stRtcpHeader.usLength + 1) * 4;

    return RTP_ERR_OK;
}

static VP_RTP_RESULT RTCP_Parse_APPPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
        VP_RTCP_APP_INFO* pstRtcpAPPInfo,
        HI_S32* piLen)
{
    HI_U32 ulSSRC     = 0;
    HI_U32 ulOffset   = 0;
    HI_S32 ulTempData = 0;
    HI_S8 i = 0;
    HI_S32 iLength = 0;
    VP_RTCP_SESSION* pstSession = HI_NULL;
    VP_SOURCE* pstSource = HI_NULL;
    HI_S8* pPacketData = (HI_S8*) pstPacket;
    VP_RTCPAPPDATA_STRU stAppInfo;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket) || (HI_NULL == pstRtcpAPPInfo) || (HI_NULL == piLen))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;

    ulTempData = ntohl (* (HI_U32*) (pPacketData + ulOffset));

    if ( (ulTempData & 0xc0000000) != 0x80000000)
    {
        return RTP_ERR_PACKETILLEGAL;
    }
    else
    {
        pstRtcpAPPInfo->stRtcpHeader.ucVersion = 2;
    }

    if ( (ulTempData & 0x20000000) == 0x20000000)
    {
        pstRtcpAPPInfo->stRtcpHeader.ucPadding = 1;
    }
    else
    {
        pstRtcpAPPInfo->stRtcpHeader.ucPadding = 0;
    }

    pstRtcpAPPInfo->stRtcpHeader.ucCount = (HI_S8) ( (ulTempData & 0x1fffffff) >> 24);
    pstRtcpAPPInfo->stRtcpHeader.ucPacketType = (HI_S8) ( (ulTempData & 0x00ffffff) >> 16);
    pstRtcpAPPInfo->stRtcpHeader.usLength = (HI_U16) ( (ulTempData & 0x0000ffff));
    iLength   = pstRtcpAPPInfo->stRtcpHeader.usLength - 2;
    ulOffset += 4;

    ulSSRC = ntohl (* (HI_U32*) (pPacketData + ulOffset));
    pstRtcpAPPInfo->ulSSRC = ulSSRC;
    ulOffset += 4;

    pstSource = RTCP_CreateSource (pstSession, ulSSRC, HI_FALSE);

    if (HI_NULL == pstSource)
    {
        return RTP_ERR_ERROR;
    }

    for (i = 0; i < 4; ++i)
    {
        pstRtcpAPPInfo->pcName[i] = * (pPacketData + ulOffset);
        ulOffset += 1;
    }

    if (0 < iLength)
    {
        pstRtcpAPPInfo->pcData  = (HI_S8*) (pPacketData + ulOffset);
        pstRtcpAPPInfo->iLength = iLength;
    }
    else
    {
        pstRtcpAPPInfo->pcData  = HI_NULL;
        pstRtcpAPPInfo->iLength = 0;
    }

    /* generate application packets information */
    if (HI_NULL != pstSession->pAPPDataCallBackProc)
    {
        memcpy (stAppInfo.cName, pstRtcpAPPInfo->pcName, 4);
        stAppInfo.pucData = (HI_U8*) pstRtcpAPPInfo->pcData;
        stAppInfo.uiDataSize = pstRtcpAPPInfo->iLength * 4;
        stAppInfo.ucType = pstRtcpAPPInfo->stRtcpHeader.ucCount;
        pstSession->pAPPDataCallBackProc (pstSession->iChannel, &stAppInfo, pstSession->pContext);
    }

    *piLen = (pstRtcpAPPInfo->stRtcpHeader.usLength + 1) * 4;

    return RTP_ERR_OK;
}

static VP_RTP_RESULT RTCP_PackPacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket)
{
    HI_S32 iLength = RTCP_PACKET_DATA_MAX_LEN;
    HI_S32 iLenPacket = 0;
    HI_S8* pBuf;
    VP_RTCP_SESSION* pstSession = HI_NULL;
    VP_RTP_RESULT enRet;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;
    pBuf = (HI_S8*) pstPacket->data;

    /* packets sending mechanism */
    if ( (!pstSession->LOCAL->bIsLeaver) && (pstSession->LOCAL->bRTPHasSent))
    {
        RTCP_Pack_SRPacket (hHandle, pBuf, &iLenPacket);
        iLength -= iLenPacket;
        pBuf = pBuf + iLenPacket;
    }
    else
    {
        RTCP_Pack_RRPacket (hHandle, pBuf, &iLenPacket);
        pBuf = pBuf + iLenPacket;
        iLength -= iLenPacket;
    }

    if (!pstSession->LOCAL->bIsLeaver)
    {
        enRet = RTCP_Pack_SDESPacket (hHandle, pBuf, &iLenPacket);

        if (RTP_ERR_OK != enRet)
        {
            iLenPacket = 0;
        }

        iLength -= iLenPacket;

        if (pstSession->iAppSendFlag)
        {
            pBuf = pBuf + iLenPacket;
            RTCP_Pack_APPPacket (hHandle, pBuf, &iLenPacket);
            iLength -= iLenPacket;
        }
    }
    else
    {
        RTCP_Pack_BYEPacket (hHandle, (VP_RTPPACKET_STRU*) pBuf, &iLenPacket);
        iLength -= iLenPacket;
    }

    pstPacket->ulLen = RTCP_PACKET_DATA_MAX_LEN - iLength;

    return RTP_ERR_OK;
}

static VP_RTP_RESULT RTCP_Update (VP_tRTCPSessionHandle hHandle)
{
    HI_S8 i = 0;
    HI_U32 ulDelay = 0;
    HI_U32 ulTime = 0;
    VP_SOURCE* pstSource = HI_NULL;
    VP_SOURCE* pstNext = HI_NULL;
    VP_RTCP_SESSION* pstSession = HI_NULL;

    if (HI_NULL == hHandle)
    {
        return RTP_ERR_INVALIDPARAM;
    }

    ulTime = RTCP_UTILS_TimeGetTime();
    pstSession = (VP_RTCP_SESSION*) hHandle;

    /* update each second */
    if (RTCP_UPDATEDATA_TIME >= (ulTime - pstSession->ulLastUpdateTime))
    {
        return RTP_ERR_OK;
    }

    pstSession->ulLastUpdateTime = ulTime;
    ulDelay = ulTime - pstSession->LOCAL->ulRTPLastSendTime;

    if (ulDelay >= (HI_U32) (2 * RTCP_EstimateInterval (pstSession)))
    {
        pstSession->LOCAL->bRTPHasSent = HI_FALSE;
    }

    for (i = 0; SOURCE_COUNT > i; i++)
    {
        pstSource = pstSession->pstSources[i];

        while (HI_NULL != pstSource)
        {
            //printf ("@[%d]pstSource:%p,%x,%x\n", i, pstSource, pstSource->bIsLeaver, pstSource->bIsSender); /*libo@201312*/
            pstNext = pstSource->pstNext;
            ulDelay = ulTime - pstSource->ulLastActiveTime;

            /* receive BYE packet in two seconds */
            if (pstSource->bIsLeaver && (SOURCE_LEAVED_DELAY < ulDelay))
            {
                RTCP_DeleteSource (pstSession, pstSource->ulSSRC);
                pstSource = pstNext;
                continue;
            }

            /* do not receive RTP packet in RTCP interval */
            if ( (pstSource->ulSSRC != pstSession->LOCAL->ulSSRC)
                    && ( (2 * pstSession->LOCAL->ulRTCPInterval) < ulDelay))
            {
                if (pstSource->bIsSender)
                {
                    pstSource->bIsSender = HI_FALSE;
                    pstSession->ulSenders--;
                }
            }

            /* do not receive RTP or RTCP packet in five RTCP interval */
            if ( (pstSource->ulSSRC != pstSession->LOCAL->ulSSRC)
                    && ( (5 * pstSession->LOCAL->ulRTCPInterval) < ulDelay))
            {
                RTCP_DeleteSource (pstSession, pstSource->ulSSRC);
            }

            pstSource = pstNext;
        }
    }

    return RTCP_UpdateReportBlocks (pstSession, ulTime);
}

static HI_VOID RTCP_GenerateCname (HI_CHAR* pszCNAME, HI_S8* piLen)
{
    return;
}

static HI_U32 RTCP_EstimateInterval (VP_tRTCPSessionHandle hHandle)
{
    VP_RTCP_SESSION* pstSession = HI_NULL;

    pstSession = (VP_RTCP_SESSION*) hHandle;

    return pstSession->uiRtcpSendInterval;
}

static VP_RTP_RESULT RTCP_SendBye (VP_tRTCPSessionHandle hHandle)
{
    VP_RTCP_SESSION* pstSession = HI_NULL;

    if (HI_NULL == hHandle)
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;

    /* neither sender nor receiver */
    if ( (!pstSession->LOCAL->bRTPHasSent) && (!pstSession->LOCAL->bRTCPHasSent))
    {
        return RTP_ERR_OK;
    }

    pstSession->LOCAL->bIsLeaver = HI_TRUE;
    pstSession->ulMembers--;

    return RTP_ERR_OK;
}

/* Create local session */
static VP_SOURCE* RTCP_CreateLocal (VP_RTCP_SESSION* pstSession)
{
    HI_U32 ulSSRC = 0;
    HI_U32 ulSequence  = 0;
    HI_U32 ulTimestamp = 0;
    HI_S8 piLen = 0;
    VP_SOURCE* pstLocal  = HI_NULL;

    /* generate random number */
    ulSSRC = ( (RTCP_UTILS_Rand() & 0xFFFF) << 16) | (RTCP_UTILS_Rand() & 0xFFFF);
    ulSequence  = (RTCP_UTILS_Rand() & 0xFFFF);
    ulTimestamp = ( (RTCP_UTILS_Rand() & 0xFFFF) << 16) | (RTCP_UTILS_Rand() & 0xFFFF);

    pstLocal = (VP_SOURCE*) HI_MALLOC (HI_ID_USR_START, sizeof (VP_SOURCE));

    if (HI_NULL != pstLocal)
    {
        memset (pstLocal, 0, sizeof (VP_SOURCE));

        RTCP_GenerateCname (pstLocal->stRtcpSdesInfo.szCNAME, &piLen);

        /* use 10k as default value */
        pstLocal->ulRTPBandwidth   = 10240;
        pstLocal->ulRTCPBandwidth  = 10240;
        pstLocal->ulLastActiveTime =0; 
        pstSession->LOCAL = pstLocal;
        pstLocal->ulSSRC = ulSSRC;
        pstLocal->ulRTPOrgSequence  = ulSequence;
        pstLocal->ulRTPOrgTimestamp = ulTimestamp;
        pstLocal->ulRTPSequence  = ulSequence;
        pstLocal->ulRTPTimestamp = ulTimestamp;
        pstLocal->bRTPFirstSendPacket = HI_TRUE;
        pstLocal->bRTPFirstRecvPacket = HI_TRUE;
        pstLocal->ulRTCPAverageSize  = RTCP_INVALID_VALUE_HI_U32;
        pstLocal->ulRTCPLastSendTime = pstLocal->ulLastActiveTime;
        pstLocal->ulRTCPNextSendTime = pstLocal->ulLastActiveTime + DEFAULT_RTCP_SEND_INTERVAL;
        pstSession->ulLastUpdateTime = pstLocal->ulLastActiveTime;
        pstLocal->ulRTCPInterval = DEFAULT_RTCP_SEND_INTERVAL;
        pstSession->uiState = SESSION_OPEN;
    }

    return pstLocal;
}

/* Delete local session */
static HI_VOID RTCP_DeleteLocal (VP_RTCP_SESSION* pstSession)
{
    if (HI_NULL == pstSession)
    {
        return;
    }

    if (HI_NULL != pstSession->LOCAL)
    {
        RTCP_DeleteSource (pstSession, pstSession->LOCAL->ulSSRC);
        HI_FREE (HI_ID_USR_START, pstSession->LOCAL);
        pstSession->LOCAL = HI_NULL;
    }


    return;
}

static HI_VOID RTCP_SessionInit (VP_RTCP_SESSION* pstSession)
{
    HI_S32 i;

    for (i = 0; i < SOURCE_COUNT; ++i)
    {
        pstSession->pstSources[i] = HI_NULL;
    }

    pstSession->ulRTPInvalidPackets = 0;
    pstSession->ulRTPInvalidBytes = 0;
    pstSession->ulRTCPInvalidPackets = 0;
    pstSession->ulRTCPInvalidBytes = 0;
    pstSession->ulLastMembers = 0;
    pstSession->ulMembers = 0;
    pstSession->ulSenders = 0;
    pstSession->ulLeavers = 0;
    pstSession->pulCSRC = HI_NULL;
    pstSession->ulCSRCs = 0;
    pstSession->ulLastNotifyCSRC = 0;

    return;
}

VP_RTP_RESULT VP_RTCP_CreateSession (VP_tRTCPSessionHandle* hHandle, HI_S32 iChID)
{
    VP_RTCP_SESSION* phmeRtpSession = HI_NULL;

    if (HI_NULL == hHandle)
    {
        return RTP_ERR_INVALIDPARAM;
    }

    phmeRtpSession = (VP_RTCP_SESSION*) HI_MALLOC (HI_ID_USR_START, sizeof (VP_RTCP_SESSION));

    if (HI_NULL == phmeRtpSession)
    {
        return RTP_ERR_MEMORY;
    }

    memset (phmeRtpSession, 0, sizeof (VP_RTCP_SESSION));
    RTCP_SessionInit (phmeRtpSession);
    phmeRtpSession->LOCAL = RTCP_CreateLocal (phmeRtpSession);

    if (HI_NULL == phmeRtpSession->LOCAL)
    {
        HI_FREE (HI_ID_USR_START, phmeRtpSession);
        return RTP_ERR_MEMORY;
    }
    memset (g_RtcpReprotTime, 0, sizeof (g_RtcpReprotTime));

    phmeRtpSession->iChannel = iChID;
    phmeRtpSession->stSendAppInfo.pcData = (HI_S8*) phmeRtpSession->ucSendAppDataBuf;

    /* enable RTCP as default function */
    phmeRtpSession->bRtcpEnable = HI_TRUE;
    phmeRtpSession->uiRtcpSendInterval = DEFAULT_RTCP_SEND_INTERVAL;
    *hHandle = (VP_tRTCPSessionHandle*) phmeRtpSession;

    return RTP_ERR_OK;
}

VP_RTP_RESULT VP_RTCP_DeleteSession (VP_tRTCPSessionHandle hHandle)
{
    VP_RTCP_SESSION* pstSession = HI_NULL;
    VP_SOURCE* pstSource  = HI_NULL;
    VP_SOURCE* pstNextSrc = HI_NULL;
    HI_S32 i = 0;

    if (HI_NULL == hHandle)
    {
        return RTP_ERR_INVALIDPARAM;
    }

    RTCP_SendBye (hHandle);

    pstSession = (VP_RTCP_SESSION*) hHandle;
    pstSession->uiState = SESSION_CLOSE;

    /* delete all source except local source */
    for (i = 0; i < SOURCE_COUNT; i++)
    {
        pstSource = pstSession->pstSources[i];

        while (HI_NULL != pstSource)
        {
            pstNextSrc = pstSource->pstNext;

            if (pstSource->ulSSRC != pstSession->LOCAL->ulSSRC)
            {
                RTCP_DeleteSource (pstSession, pstSource->ulSSRC);
            }

            pstSource = pstNextSrc;
        }
    }

    /* delete local source */
    RTCP_DeleteLocal (pstSession);
    HI_FREE (HI_ID_USR_START, pstSession);
    pstSession = HI_NULL;

    return RTP_ERR_OK;
}

VP_RTP_RESULT VP_RTCP_ParsePacket (VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
                                   VP_RTCP_INFO* pstRtcpInfo)
{
    HI_U32 TempData = 0;
    HI_S32 iLength = 0;
    HI_S32 iRecvPackLen = 0;
    HI_U32 ulTime = 0;
    HI_S8* pBuf = HI_NULL;
    HI_S32 iRet = 0;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket) || (HI_NULL == pstRtcpInfo))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    iLength = pstPacket->ulLen;
    pBuf   = (HI_S8*) pstPacket;
    ulTime = RTCP_UTILS_TimeGetTime();
    
    while (iLength > 0)
    {
        TempData  = ntohl (* (HI_U32*) (pBuf));
        TempData &= 0x00ff0000;
        TempData = TempData >> 16;

        switch (TempData)
        {
        case RTCP_SR:
            iRet = RTCP_Parse_SRPacket (hHandle, (VP_RTPPACKET_STRU*) pBuf, & (pstRtcpInfo->stRtcpSRInfo), &iRecvPackLen,
                                        ulTime);
            break;

        case RTCP_RR:
            iRet = RTCP_Parse_RRPacket (hHandle, (VP_RTPPACKET_STRU*) pBuf, & (pstRtcpInfo->stRtcpRRInfo), &iRecvPackLen,
                                        ulTime);
            break;

        case RTCP_SDES:
            iRet = RTCP_Parse_SDESPacket (hHandle, (VP_RTPPACKET_STRU*) pBuf, & (pstRtcpInfo->stRtcpSDESInfo),
                                          &iRecvPackLen);
            break;

        case RTCP_BYE:
            iRet = RTCP_Parse_BYEPacket (hHandle, (VP_RTPPACKET_STRU*) pBuf, & (pstRtcpInfo->stRtpBYEInfo), &iRecvPackLen);
            break;

        case RTCP_APP:
            iRet = RTCP_Parse_APPPacket (hHandle, (VP_RTPPACKET_STRU*) pBuf, & (pstRtcpInfo->stRtcpAppInfo),
                                         &iRecvPackLen);
            break;

        default:
            return RTP_ERR_ERROR;
        }

        if (RTP_ERR_OK != iRet)
        {
            return RTP_ERR_ERROR;
        }

        if (iRecvPackLen <= 0)
        {
            return RTP_ERR_ERROR;
        }

        pBuf = pBuf + iRecvPackLen;
        iLength -= iRecvPackLen;
    }

    return RTP_ERR_OK;
}

VP_RTP_RESULT VP_RTCP_SendCtrl (VP_tRTCPSessionHandle hHandle, VP_PACKET_STRU* pstPacket)
{
    HI_S8 i = 0;
    HI_U32 ulTime = 0;
    HI_U32 ulInterval = 0;
    HI_U32 ulSendTime = 0;
    VP_RTPPACKET_STRU* pBuffer  = HI_NULL;
    VP_RTCP_SESSION* pstSession = HI_NULL;
    VP_SOURCE* pstSource  = HI_NULL;
    VP_RTP_RESULT iResult = RTP_ERR_ERROR;

    if ( (HI_NULL == hHandle) || (HI_NULL == pstPacket))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    pstSession = (VP_RTCP_SESSION*) hHandle;

    if (HI_NULL == pstSession->LOCAL)
    {
        return RTP_ERR_INVALIDPARAM;
    }

    if (pstSession->bRtcpEnable == HI_FALSE)
    {
        return RTP_ERR_ERROR;
    }

    pBuffer = (VP_RTPPACKET_STRU*) & (pstPacket->u.stRtpPack);
    ulTime = RTCP_UTILS_TimeGetTime();

    /* ready to send */
    if (RTCP_UTILS_SystemTimeCMP (ulTime, pstSession->LOCAL->ulRTCPNextSendTime))
    {
        pstPacket->enPackType = VP_RTCP;
        ulInterval = RTCP_EstimateInterval (pstSession);
        ulSendTime = pstSession->LOCAL->ulRTCPLastSendTime + ulInterval;

        if (ulTime >= ulSendTime)
        {
            RTCP_PackPacket (hHandle, pBuffer);

            /* use 5000ms as default value, do not estimate interval */
            ulInterval = RTCP_EstimateInterval (pstSession);
            pstSession->LOCAL->bRTCPHasSent = HI_TRUE;
            pstSession->LOCAL->ulRTCPLastSendTime = ulTime;
            pstSession->LOCAL->ulRTCPNextSendTime = ulTime + ulInterval;

            for (i = 0; SOURCE_COUNT > i; i++)
            {
                pstSource = pstSession->pstSources[i];

                while (HI_NULL != pstSource)
                {
                    pstSource->bIsSender = HI_FALSE;
                    pstSource = pstSource->pstNext;
                }
            }

            pstSession->ulSenders = 0;
            pstSession->iAppSendFlag = 0;

            iResult = RTP_ERR_OK;
        }
        else
        {
            pstSession->LOCAL->ulRTCPNextSendTime = ulSendTime;
            iResult = RTP_ERR_ERROR;
        }

        pstSession->ulLastMembers = pstSession->ulMembers;
    }

    /* whether it is the time to send application packets */
    if (pstSession->iAppSendFlag)
    {
        pstPacket->enPackType = VP_RTCP;
        pstSession->iAppSendFlag = 0;
        RTCP_Pack_APPPacket (pstSession, (HI_S8*) pBuffer->data, (HI_S32*) & (pBuffer->ulLen));
        iResult = RTP_ERR_OK;
    }

    /* update RTCP information */
    RTCP_Update (hHandle);

    return iResult;
}

/* Update source info after parse RTP packets */
HI_VOID VP_RTCP_ParseRtpUpdateSource (VP_RTCP_SESSION* pstSession, HI_U32 ulPacketLen, VP_RTP_INFO* pstRtpInfo)
{
    HI_S32 i = 0;
    VP_SOURCE* pstSource = HI_NULL;
    HI_BOOL bIsValid = HI_FALSE;
    HI_U32 ulTimeNow = 0;

    ulTimeNow = RTCP_UTILS_TimeGetTime();
    pstSource = RTCP_CreateSource (pstSession, pstRtpInfo->ulSSRC, HI_FALSE);

    if (HI_NULL != pstSource)
    {
        /* update sequence and time stamp */
        if (pstSource->bRTPFirstRecvPacket)
        {
            pstSource->bRTPFirstRecvPacket = HI_FALSE;
            pstSource->ulRTPOrgSequence  = pstRtpInfo->usSequenceNumber;
            pstSource->ulRTPOrgTimestamp = pstRtpInfo->ulTimestamp;
            pstSource->ulRTPSequence  = pstRtpInfo->usSequenceNumber;
            pstSource->ulRTPTimestamp = pstRtpInfo->ulTimestamp;
            pstSource->ulLastPacketTs = pstRtpInfo->ulTimestamp;
            pstSource->ulLastPacketArriveTime = ulTimeNow;
        }

        /* redundant code */
        /* SSRC will check the validity of continuous RTP packets */
        /* checking the validity of continuous RTP packets means, checking received packets of current source, */
        /* while find the sequence of N RTP packets are continuous, we believe the current source is valid, */
        /* than update and calculate the correlative information of the source */
        if (0 > pstSource->iProbation)
        {
            pstSource->iProbation = MIN_SEQUENTIAL;
            pstSource->ulRTPSequence = pstRtpInfo->usSequenceNumber - 1;
        }

        /* update received information */
        bIsValid = RTCP_UpdateSource (pstSource, pstRtpInfo->usSequenceNumber, pstRtpInfo->ulTimestamp);

        /* update successfully */
        if (bIsValid)
        {
            /* create CSRC in RTP packet */
            for (i = 0; i < pstRtpInfo->ucCSRCcount; i++)
            {
                VP_SOURCE* pstTempSource = HI_NULL;
                pstTempSource = RTCP_CreateSource (pstSession, pstRtpInfo->ulCSRC[i], HI_FALSE);

                if (HI_NULL == pstTempSource)
                {
                    continue;
                }
            }

            /* still not the sender */
            if (!pstSource->bIsSender)
            {
                pstSource->bIsSender = HI_TRUE;
                pstSession->ulSenders++;
            }

            pstSource->ulLastPacketTs = pstRtpInfo->ulTimestamp;
            pstSource->ulLastPacketArriveTime = ulTimeNow;
            pstSource->ulRTPPackets++;
            pstSource->ulRTPBytes += ulPacketLen;
            pstSource->bIsLocalRecv = HI_TRUE;
        }
    }
}

VP_RTP_RESULT VP_RTCP_EnableRtcp (VP_tRTCPSessionHandle hHandle, HI_BOOL bEnable)
{
    VP_RTCP_SESSION* pstSession = HI_NULL;

    if (HI_NULL != hHandle)
    {
        pstSession = (VP_RTCP_SESSION*) hHandle;
        pstSession->bRtcpEnable = bEnable;
    }

    return RTP_ERR_OK;
}

VP_RTP_RESULT VP_RTCP_CfgRtcpSendInterval (VP_tRTCPSessionHandle hHandle, HI_S32 iInterval)
{
    VP_RTCP_SESSION* pstSession = HI_NULL;

    if (HI_NULL != hHandle)
    {
        pstSession = (VP_RTCP_SESSION*) hHandle;
        pstSession->uiRtcpSendInterval = iInterval;
    }

    return RTP_ERR_OK;
}


VP_RTP_RESULT VP_RTCP_RecvCalculateDelayAndJitter (VP_RTCP_SESSION* pstSession, HI_U32 u32Ssrc, HI_U32 u32PtsMs)
{
    VP_SOURCE* pstSource = HI_NULL;
    HI_S32 u32Delay = 0;
    HI_U32 u32Time = 0;
    HI_U32 u32Jitter = 0;

    u32Time = RTCP_UTILS_TimeGetTime();

    if ( (0 == u32Ssrc) || (0 == u32PtsMs) || (HI_NULL == pstSession))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    /* look up relative source according to given SSRC tag */
    pstSource = RTCP_LookupSource (pstSession, u32Ssrc);

    if (HI_NULL == pstSource)
    {
        return RTP_ERR_ERROR;
    }

    if ((0 != pstSource->ulTimeRatio)&&(0 != pstSource->ulSendNetDelay))
    {
        u32Delay = u32Time - pstSource->ulTimeOffest - (u32PtsMs / pstSource->ulTimeRatio);
        u32Delay = (u32Delay > 0) ? u32Delay : 0;
        pstSource->ulRecvDelay = RTCP_UTILS_MeanDeviation (u32Delay, pstSource->ulRecvDelay);
        u32Time = pstSession->stRtcpStatisticInfo.stRecvInfo.uiDelay;
        pstSession->stRtcpStatisticInfo.stRecvInfo.uiDelay = (pstSource->ulRecvDelay + RTCP_MEAN_DEVIATION_COUNT / 2) \
                / RTCP_MEAN_DEVIATION_COUNT;
    }

    if ( (0 != u32Delay) && (0 != u32Time))
    {
        u32Jitter = (u32Delay > u32Time) ? (u32Delay - u32Time) : (u32Time - u32Delay);
        pstSource->ulRecvJitter = RTCP_UTILS_MeanDeviation (u32Jitter, pstSource->ulRecvJitter);
        pstSession->stRtcpStatisticInfo.stRecvInfo.uiJitter = (pstSource->ulRecvJitter + RTCP_MEAN_DEVIATION_COUNT / 2) \
                / RTCP_MEAN_DEVIATION_COUNT;
    }
    HI_INFO_RTP("Recv<-Delay:%2d~%2d,Jitter:%2d~%2d\n", u32Delay, (pstSource->ulRecvDelay / RTCP_MEAN_DEVIATION_COUNT), u32Jitter, (pstSource->ulRecvJitter / RTCP_MEAN_DEVIATION_COUNT));    /*libo@201312*/

    return RTP_ERR_OK;
}




VP_RTP_RESULT VP_RTCP_SendCalculateDelayAndJitter (VP_RTCP_SESSION* pstSession, HI_U32 u32PtsMs)
{
    VP_SOURCE* pstSource = HI_NULL;
    HI_S32 u32Delay = 0;
    HI_U32 u32Time = 0;
    HI_U32 u32Jitter = 0;
    HI_U32 i = 0;

    u32Time = RTCP_UTILS_TimeGetTime();

    if ((0 == u32PtsMs) || (HI_NULL == pstSession))
    {
        return RTP_ERR_INVALIDPARAM;
    }

    for (i = 0; SOURCE_COUNT > i; ++i)
    {

        pstSource = pstSession->pstSources[i];

        if ((HI_NULL != pstSource)&&(pstSource->bIsRemoteRecv))
        {

            if (0 != pstSource->ulSendNetDelay)
            {
                u32Delay = u32Time - u32PtsMs + pstSource->ulSendNetDelay;
                u32Delay = (u32Delay > 0) ? u32Delay : 0;
                pstSource->ulSendDelay = RTCP_UTILS_MeanDeviation (u32Delay, pstSource->ulSendDelay);
                u32Time = pstSession->stRtcpStatisticInfo.stSendInfo.uiDelay;
                pstSession->stRtcpStatisticInfo.stSendInfo.uiDelay = (pstSource->ulSendDelay + RTCP_MEAN_DEVIATION_COUNT / 2) \
                                                                         / RTCP_MEAN_DEVIATION_COUNT;
            }

            if ( (0 != u32Delay) && (0 != u32Time))
            {
                u32Jitter = (u32Delay > u32Time) ? (u32Delay - u32Time) : (u32Time - u32Delay);
                pstSource->ulSendJitter = RTCP_UTILS_MeanDeviation (u32Jitter, pstSource->ulSendJitter);
                pstSession->stRtcpStatisticInfo.stSendInfo.uiJitter = (pstSource->ulSendJitter + RTCP_MEAN_DEVIATION_COUNT / 2) \
                                                                         / RTCP_MEAN_DEVIATION_COUNT;
            }
            HI_INFO_RTP("Send->Delay:%2d~%2d,Jitter:%2d~%2d\n", u32Delay, (pstSource->ulSendDelay / RTCP_MEAN_DEVIATION_COUNT), u32Jitter, (pstSource->ulSendJitter / RTCP_MEAN_DEVIATION_COUNT));     /*libo@201312*/
        }
    }

    return RTP_ERR_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
