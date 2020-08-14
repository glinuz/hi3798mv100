#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <string.h>   /* for NULL */
#include <unistd.h>
#include <arpa/inet.h>

#include "hi_type.h"
#include "hi_unf_avplay.h"
#include "hi_unf_common.h"
#include "hi_adp.h"

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "HA.AUDIO.DTSHD.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"

#include "hi_adp_data.h"
#include "hi_adp_demux.h"
#include "hi_adp_search.h"
#include "hi_adp_tuner.h"

#define PGPAT_TIMEOUT (5000)
#define PGPMT_TIMEOUT (2000)
#define PGSDT_TIMEOUT (10000)

//#define SRCH_DEBUG

static HI_VOID ParseSDTDesc( const HI_U8* pu8DesData, HI_S32 s32DescLen, SDT_INFO *pstruProg)
{
    HI_U32 u32NameLength = 0;
    HI_U8 u8Tag = 0;
    HI_U8 u8Length;

    const HI_U8* pu8Data = HI_NULL;

    if ((HI_NULL == pu8DesData) || (HI_NULL == pstruProg))
    {
        return;
    }

    while (s32DescLen > 0)
    {
        u8Tag = *pu8DesData++;
        u8Length = *pu8DesData++;

        if (u8Length == 0)
        {
            return;
        }

        pu8Data = pu8DesData;
        pu8DesData += u8Length;

        s32DescLen -= (u8Length + 2);

        switch (u8Tag)
        {
        case SERVICE_DESCRIPTOR:
        {
            pstruProg->u32ServiceType = *pu8Data++;

            u32NameLength = *pu8Data++;
            pu8Data += u32NameLength;

            u32NameLength = *pu8Data++;

            if (0 == u32NameLength)
            {
                break;
            }


            /*Parse service name*/
            if (u32NameLength > MAX_PROGNAME_LENGTH - 1)
            {
                u32NameLength = (HI_U32)(MAX_PROGNAME_LENGTH - 1);
            }

            strncpy((HI_CHAR*)pstruProg->s8ProgName, (HI_CHAR*)pu8Data, u32NameLength);

            break;
        }
        case NVOD_REFERENCE_DESCRIPTOR:
        {
            while (u8Length > 0)
            {
                pstruProg->u32ServiceType = 0x04;

                u8Length = u8Length - 6;

                if (u8Length <= 0)
                {
                    break;
                }

                pu8Data += 6;
            }

            break;
        }
        case COUNTRY_AVAILABILITY_DESCRIPTOR:
        case LINKAGE_DESCRIPTOR:

        case MOSAIC_DESCRIPTOR:

            //printf("\n +++ SDT MOSAIC_DESCRIPTOR +++\n");
            break;
        case CA_IDENTIFIER_DESCRIPTOR:
        case TELEPHONE_DESCRIPTOR:
        case MULTILINGUAL_SERVICE_NAME_DESCRIPTOR:
        case PRIVATE_DATA_SPECIFIER_DESCRIPTOR:
        case DATA_BROADCAST_DESCRIPTOR:
        case STUFFING_DESCRIPTOR:
        case BOUQUET_NAME_DESCRIPTOR:
            break;
        default:
            break;
        }
    }
}

static HI_VOID ParsePMTDesc( const HI_U8* pu8DesData, HI_S32 s32DescLength, PMT_TB* pstruProg)
{
    HI_U8 u8Tag = 0;
    HI_U8 u8Length = 0;
    HI_U8 u8CntIndex = 0;

    if ((HI_NULL == pu8DesData) || (HI_NULL == pstruProg))
    {
        return;
    }

    while (s32DescLength > 0)
    {
        u8Tag = *pu8DesData++;
        u8Length = *pu8DesData++;
        if (u8Length == 0)
        {
            return;
        }

        switch (u8Tag)
        {
            case LANGUAGE_DESCRIPTOR:
            {
                break;
            }

            case CA_DESCRIPTOR:
            {
                //pstruProg->CASystem[pstruProg->u16CANum].u16CASystemID = pu8DesData[0] << 8 | pu8DesData[1];
                //pstruProg->CASystem[pstruProg->u16CANum].u16CAPID = pu8DesData[2] << 8 | pu8DesData[3];
                //pstruProg->u16CANum++;
                break;
            }

            case TELETEXT_DESCRIPTOR:
            {
                HI_U16 ii = 0;
                HI_U16 nCount = u8Length / 5;
                HI_U16 u16TtxIndex = pstruProg->u16TtxNum;

                for(ii = 0; ii < nCount; ii++)
                {
                    u8CntIndex = pstruProg->stTtxInfo[u16TtxIndex].u8DesInfoCnt;
                    if ((s32DescLength >= 5) && u8CntIndex < TTX_DES_MAX)
                    {
                        /* 8859-1 language code */
                        pstruProg->stTtxInfo[u16TtxIndex].stTtxDes[u8CntIndex].u32ISO639LanguageCode =
                                           (pu8DesData[ii*5+0] << 16) | (pu8DesData[ii*5 + 1] << 8) | (pu8DesData[ii*5 + 2]);
                        /* teletext_type */
                        pstruProg->stTtxInfo[u16TtxIndex].stTtxDes[u8CntIndex].u8TtxType = (pu8DesData[ii*5 + 3] >> 3) & 0x1f;
                        /* teletext_magazine_number */
                        pstruProg->stTtxInfo[u16TtxIndex].stTtxDes[u8CntIndex].u8TtxMagazineNumber = pu8DesData[ii*5 + 3] & 0x7;
                        /* teletext_page_number */
                        pstruProg->stTtxInfo[u16TtxIndex].stTtxDes[u8CntIndex].u8TtxPageNumber = pu8DesData[ii*5 + 4];
                        pstruProg->stTtxInfo[u16TtxIndex].u8DesInfoCnt++;
                    }
                }
                break;
            }

            case SUBTITLING_DESCRIPTOR:
            {
                HI_U16 ii = 0;
                HI_U16 nCount = u8Length / 8; /*8 =  3 + 1 + 2 + 2*/
                HI_U16 u16SubtIndex = pstruProg->u16SubtitlingNum;

                for(ii=0; ii<nCount; ii++)
                {
                    u8CntIndex = pstruProg->SubtitingInfo[u16SubtIndex].u8DesInfoCnt;
                   // printf("Subtitle tag s32DescLength:%u.............pstruProg->u16SubtitlingNum = %d, u8CntIndex = %d\n",s32DescLength, pstruProg->u16SubtitlingNum, u8CntIndex);
                    if ((s32DescLength >= 8) && u8CntIndex < SUBTDES_INFO_MAX)
                    {
                        /* 8859-1 language code */
                        pstruProg->SubtitingInfo[u16SubtIndex].DesInfo[u8CntIndex].u32LangCode =
                                           (pu8DesData[ii*8+0] << 16) | (pu8DesData[ii*8 + 1] << 8) | (pu8DesData[ii*8 + 2]);

                        /* page id */
                        pstruProg->SubtitingInfo[u16SubtIndex].DesInfo[u8CntIndex].u16PageID = (pu8DesData[ii*8 + 4] << 8) | (pu8DesData[ii*8 + 5]);

                        /* ancilary page id */
                        pstruProg->SubtitingInfo[u16SubtIndex].DesInfo[u8CntIndex].u16AncillaryPageID = (pu8DesData[ii*8 + 6] << 8) | (pu8DesData[ii*8 + 7]);

                        pstruProg->SubtitingInfo[u16SubtIndex].u8DesInfoCnt++;
                    }
                #if 0
                    sample_common_printf("[%d]lang code:%#x\n", pstruProg->u16SubtitlingNum, pstruProg->SubtitingInfo[pstruProg->u16SubtitlingNum].DesInfo[u8CntIndex].u32LangCode);
                    sample_common_printf("[%d]page id :%u\n", pstruProg->u16SubtitlingNum, pstruProg->SubtitingInfo[pstruProg->u16SubtitlingNum].DesInfo[u8CntIndex].u16PageID);
                    sample_common_printf("[%d]ancilary page id : %u\n", pstruProg->u16SubtitlingNum, pstruProg->SubtitingInfo[pstruProg->u16SubtitlingNum].DesInfo[u8CntIndex].u16AncillaryPageID);
                    sample_common_printf("[%d]count is %u\n\n", pstruProg->u16SubtitlingNum, pstruProg->SubtitingInfo[pstruProg->u16SubtitlingNum].u8DesInfoCnt);
                    sample_common_printf("[%d]pid is 0x%x\n\n", pstruProg->u16SubtitlingNum,pstruProg->SubtitingInfo[pstruProg->u16SubtitlingNum].u16SubtitlingPID);
                #endif
                }

                break;
            }

            case AC3_DESCRIPTOR:
            case AC3_PLUS_DESCRIPTOR:
                break;

            case COPYRIGHT_DESCRIPTOR:
            case MOSAIC_DESCRIPTOR:
                break;

            case STREAM_IDENTIFIER_DESCRIPTOR:
            case PRIVATE_DATA_SPECIFIER_DESCRIPTOR:
            case SERVICE_MOVE_DESCRIPTOR:
            case CA_SYSTEM_DESCRIPTOR:
            case DATA_BROADCAST_ID_DESCRIPTOR:
                break;
#if defined(STREAM_TYPE_CAPTURE_86)
            case CAPTION_SERVICE_DESCRIPTOR:
            {
                HI_U8 ServiceNum = 0;
                HI_U8 u8Index = 0;
                HI_U32 u32LangCode = 0;
                HI_U8 j = 0;

                ServiceNum = pu8DesData[0] & 0x1f;
                pstruProg->u16ClosedCaptionNum = ServiceNum;
                j ++;
                while(j < u8Length)
                {
                    /* Ref : 6.9.2 Caption Service Descriptor in ATSC a/65 */
                    for(u8Index = 0; u8Index < ServiceNum; u8Index++)
                    {
                        u32LangCode = (pu8DesData[j] << 16 | pu8DesData[j+1] << 8 | pu8DesData[j+2]);
                        j += 3;

                        pstruProg->stClosedCaption[u8Index].u8IsDigitalCC = pu8DesData[j] & 0x80;
                        if(pstruProg->stClosedCaption[u8Index].u8IsDigitalCC)
                        {
                            pstruProg->stClosedCaption[u8Index].u32LangCode = u32LangCode;
                            pstruProg->stClosedCaption[u8Index].u8ServiceNumber = pu8DesData[j] & 0x3f;
                            j += 1;
                            pstruProg->stClosedCaption[u8Index].u8IsEasyReader = pu8DesData[j] & 0x80;
                            pstruProg->stClosedCaption[u8Index].u8IsWideAspectRatio = pu8DesData[j] & 0x40;
                            j += 2;
                        }
                        else
                        {
                            j += 3;
                        }
                    }
                }
                break;
            }
#endif
            default:
                break;
        }

        pu8DesData    += u8Length;
        s32DescLength -= (u8Length + 2);
    }
}

HI_S32 SRH_ParsePAT( const HI_U8  *pu8SectionData, HI_S32 s32Length, HI_U8 *pSectionStruct)
{
    HI_U16 u16ProgrameNumber = 0;
    HI_U16 u16Pid  = 0x1FFF;
    HI_U16 u16TsId = 0;
    PAT_TB *pPatTb = (PAT_TB *)pSectionStruct;
    PAT_INFO *pPatInfo;

    if ((HI_NULL == pu8SectionData) || (HI_NULL == pSectionStruct))
    {
        return HI_FAILURE;
    }

#ifdef SRCH_DEBUG
    HI_S32 index;
    sample_common_printf("\n");
    for (index = 0; index < s32Length; index++)
    {
        //printf("0x%x ",pu8SectionData[index]);
    }

    sample_common_printf("\n");
#endif

    if (PAT_TABLE_ID != pu8SectionData[0])
    {
        return HI_FAILURE;
    }

    s32Length = (HI_S32)((pu8SectionData[1] << 8) | pu8SectionData[2]) & 0x0fff;

    u16TsId = (HI_U16)((pu8SectionData[3] << 8) | pu8SectionData[4]);

    pPatTb->u16TsID = u16TsId;

    pu8SectionData += 8;
    s32Length -= 9;

    while (s32Length >= 4)
    {
        pPatInfo = &(pPatTb->PatInfo[pPatTb->u16ProgNum]);

        u16ProgrameNumber = (HI_U16)((pu8SectionData[0] << 8) | pu8SectionData[1]);

        u16Pid = (HI_U16)(((pu8SectionData[2] & 0x1F) << 8) | pu8SectionData[3]);

        if (u16ProgrameNumber != 0x0000)
        {
            pPatInfo->u16ServiceID = u16ProgrameNumber;
            pPatInfo->u16PmtPid = u16Pid;
            pPatTb->u16ProgNum++;
#ifdef SRCH_DEBUG
            sample_common_printf(" parser PAT get PmtPid %d(0x%x) ServiceID %d(0x%x)  index %d\n",
                   pPatInfo->u16PmtPid, pPatInfo->u16PmtPid, pPatInfo->u16ServiceID, pPatInfo->u16ServiceID, index);
#endif
        }

        pu8SectionData += 4;
        s32Length -= 4;
    }

    return (HI_SUCCESS);
}

static HI_BOOL IsSubtitleStream(const HI_U8 *pu8Data, HI_S32 s32Length)
{
    HI_BOOL bRet = HI_FALSE;

    int nLen = s32Length;
    HI_U8 *pData = (HI_U8*)pu8Data;
    HI_U8 u8Tag = 0;
    HI_U8 u8Length = 0;

    while (nLen > 0)
    {
        u8Tag = *pData++;
        u8Length = *pData++;

        if (u8Tag == SUBTITLING_DESCRIPTOR)
        {
            bRet = HI_TRUE;
            break;
        }

        pData += u8Length;

        nLen -= (u8Length+2);
    }

    return bRet;
}

static HI_BOOL IsARIBCCStream(const HI_U8 *pu8Data, HI_S32 s32Length)
{
    HI_BOOL bRet = HI_FALSE;

    int nLen = s32Length;
    HI_U8 *pData = (HI_U8*)pu8Data;
    HI_U8 u8Tag = 0;
    HI_U8 u8Length = 0;
    HI_U8 u8ComponentTag = 0;

    while (nLen > 0)
    {
        u8Tag = *pData++;
        u8Length = *pData++;
        /*Ref ARIB-TR-B14v2_8-vol3-p3-2-E2.pdf  P3-107*/
        if (u8Tag == STREAM_IDENTIFIER_DESCRIPTOR)
        {
            u8ComponentTag = *pData;

            if((0x30 == u8ComponentTag) || (0x87 == u8ComponentTag))
            {
                bRet = HI_TRUE;
            }
            break;
        }

        pData += u8Length;

        nLen -= (u8Length+2);
    }

    return bRet;
}

HI_BOOL IsTtxStream(const HI_U8 *pu8Data, HI_S32 s32Length)
{
    HI_BOOL bRet = HI_FALSE;

    int nLen = s32Length;
    HI_U8 *pData = (HI_U8*)pu8Data;
    HI_U8 u8Tag = 0;
    HI_U8 u8Length = 0;

    while (nLen > 0)
    {
        u8Tag = *pData++;
        u8Length = *pData++;

        if (u8Tag == TELETEXT_DESCRIPTOR)
        {
            bRet = HI_TRUE;
            break;
        }

        pData += u8Length;

        nLen -= (u8Length+2);
    }

    return bRet;
}

static HI_BOOL IsAC3Stream(const HI_U8 *buf, HI_U32 len)
{
    while (len >= 2)
    {
        HI_U32 tag      = buf[0];
        HI_U32 tag_len  = buf[1];

        buf += 2;
        len -= 2;

        switch (tag)
        {
            case AC3_DESCRIPTOR:
            case AC3_PLUS_DESCRIPTOR:
            case AC3_EXT_DESCRIPTOR:
                return HI_TRUE;

            default:
                buf += tag_len;
                if (tag_len >= len)
                {
                    len = 0;
                }
                else
                {
                    len -= tag_len;
                }
        }
    }

    return HI_FALSE;
}

static HI_BOOL Is265Stream(const HI_U8 *pu8Data, HI_S32 s32Length)
{
    HI_BOOL bRet = HI_FALSE;

    int nLen = s32Length;
    HI_U8 *pData = (HI_U8*)pu8Data;
    HI_U8 u8Tag = 0;
    HI_U8 u8Length = 0;

    while (nLen > 0)
    {
        u8Tag = *pData++;
        u8Length = *pData++;

        if (u8Tag == REGISTRATION_DESCRIPTOR)
        {
            HI_U32 Identify = htonl(*((HI_U32*)pData));

            if (STREAM_TYPE_HEVC_VIDEO_IDENTIFY == Identify)
            {
                bRet = HI_TRUE;
                break;
            }
        }

        pData += u8Length;

        nLen -= (u8Length+2);
    }

    return bRet;
}

HI_S32 SRH_ParsePMT ( const HI_U8 *pu8SectionData, HI_S32 s32Length, HI_U8 *pSectionStruct)
{
    HI_U8 u8StreamType = 0;

    HI_U16 u16DesLen = 0;
    HI_U16 u16Pid  = 0x1FFF;
    PMT_TB *pPmtTb = (PMT_TB *)pSectionStruct;

    if ((HI_NULL == pu8SectionData) || (HI_NULL == pSectionStruct))
    {
        return HI_FAILURE;
    }

    if (PMT_TABLE_ID != pu8SectionData[0])
    {
        return HI_FAILURE;
    }

    memcpy(pPmtTb->u8PmtData, pu8SectionData, s32Length);
    pPmtTb->u32PmtLen = s32Length;
   // printf("enter length : %d\n", s32Length);

    s32Length = (HI_S32)((pu8SectionData[1] << 8) | pu8SectionData[2]) & 0x0fff;

   // printf("s32Length = %d, %#x:%#x\n", s32Length, pu8SectionData[1], pu8SectionData[2]);

    pPmtTb->u16ServiceID = (HI_U16)((pu8SectionData[3] << 8) | pu8SectionData[4]);

    pu8SectionData += 8; /* skip 8-byte to PCR_PID*/

    u16Pid = (HI_U16)(((pu8SectionData[0] & 0x1F) << 8) | pu8SectionData[1]);
    u16DesLen = (HI_U16)(((pu8SectionData[2] & 0x0F) << 8) | pu8SectionData[3]);

    pPmtTb->u16PcrPid = u16Pid;

    pu8SectionData += 4;

    s32Length -= 9;

   // printf("pcr pid = %#x, deslength:%d, s32Length = %d\n", u16Pid, u16DesLen, s32Length);

    if (u16DesLen > 0)
    {
        ParsePMTDesc(pu8SectionData, u16DesLen, pPmtTb);
        pu8SectionData += u16DesLen;
        s32Length -= u16DesLen;
    }

    while (s32Length > 4)
    {
        u8StreamType = pu8SectionData[0];

        u16Pid = (((pu8SectionData[1] & 0x1F) << 8) | pu8SectionData[2]);
        u16DesLen = (((pu8SectionData[3] & 0x0F) << 8) | pu8SectionData[4]);

        pu8SectionData += 5;
        s32Length -= 5;

        //printf("stream type : %#x, pid:%#x, length:%d, u16DesLen = %d\n", u8StreamType, u16Pid, s32Length, u16DesLen);

        switch (u8StreamType)
        {
            /*video stream*/
        case STREAM_TYPE_14496_10_VIDEO:
        case STREAM_TYPE_14496_2_VIDEO:
        case STREAM_TYPE_11172_VIDEO:
        case STREAM_TYPE_13818_VIDEO:
        case STREAM_TYPE_AVS_VIDEO:
        case STREAM_TYPE_HEVC_VIDEO:
        {
           // printf("video stream type is %#x, pid=%#x\n", u8StreamType, u16Pid);
            if (pPmtTb->u16VideoNum < PROG_MAX_VIDEO)
            {
                pPmtTb->Videoinfo[pPmtTb->u16VideoNum].u16VideoPid = u16Pid;

                if (u8StreamType == STREAM_TYPE_14496_10_VIDEO)
                {
                    pPmtTb->Videoinfo[pPmtTb->u16VideoNum].u32VideoEncType = HI_UNF_VCODEC_TYPE_H264;
                }
                else if (u8StreamType == STREAM_TYPE_14496_2_VIDEO)
                {
                    pPmtTb->Videoinfo[pPmtTb->u16VideoNum].u32VideoEncType = HI_UNF_VCODEC_TYPE_MPEG4;
                }
                else if (u8StreamType == STREAM_TYPE_AVS_VIDEO)
                {
                    pPmtTb->Videoinfo[pPmtTb->u16VideoNum].u32VideoEncType = HI_UNF_VCODEC_TYPE_AVS;
                }
                else if (u8StreamType == STREAM_TYPE_HEVC_VIDEO)
                {
                    pPmtTb->Videoinfo[pPmtTb->u16VideoNum].u32VideoEncType = HI_UNF_VCODEC_TYPE_HEVC;
                }
                else
                {
                    pPmtTb->Videoinfo[pPmtTb->u16VideoNum].u32VideoEncType = HI_UNF_VCODEC_TYPE_MPEG2;
                }

                if (u16DesLen > 0)
                {
                    ParsePMTDesc(pu8SectionData, u16DesLen, pPmtTb);
                    pu8SectionData += u16DesLen;
                    s32Length -= u16DesLen;
                }

                pPmtTb->u16VideoNum++;
            }

            break;
        }

        /*audio stream*/
        case STREAM_TYPE_11172_AUDIO:
        case STREAM_TYPE_13818_AUDIO:
        case STREAM_TYPE_14496_3_AUDIO:
        case STREAM_TYPE_13818_7_AUDIO:
        case STREAM_TYPE_AC3_AUDIO:
    #ifndef STREAM_TYPE_SCTE_82H
        case STREAM_TYPE_DTS_AUDIO:
    #endif
        case STREAM_TYPE_DOLBY_TRUEHD_AUDIO:
#if ! defined (STREAM_TYPE_CAPTURE_86)
	case STREAM_TYPE_DTS_MA:
#endif
        {
           // printf("audio stream type is %#x, pid = %#x\n", u8StreamType, u16Pid);
            if(pPmtTb->u16AudoNum < PROG_MAX_AUDIO)
            {
                pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u16AudioPid = u16Pid;
#ifdef SRCH_DEBUG
                sample_common_printf(" audio type = 0x%x \n", u8StreamType);
#endif
                if ((u8StreamType == STREAM_TYPE_13818_7_AUDIO) || (u8StreamType == STREAM_TYPE_14496_3_AUDIO))
                {
                    pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u32AudioEncType = HA_AUDIO_ID_AAC;
                }
                else if (u8StreamType == STREAM_TYPE_AC3_AUDIO)
                {
                    pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u32AudioEncType = HA_AUDIO_ID_DOLBY_PLUS;
                }
                else if (u8StreamType == STREAM_TYPE_DOLBY_TRUEHD_AUDIO)
                {
                    pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u32AudioEncType = HA_AUDIO_ID_DOLBY_TRUEHD;
                }
                else if (u8StreamType == STREAM_TYPE_DTS_AUDIO
#if ! defined (STREAM_TYPE_CAPTURE_86)				
					|| u8StreamType == STREAM_TYPE_DTS_MA
#endif
				)
                {
                    pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u32AudioEncType = HA_AUDIO_ID_DTSHD;
                }
                else
                {
                    pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u32AudioEncType = HA_AUDIO_ID_MP3;
                }

                if (u16DesLen > 0)
                {
                    ParsePMTDesc(pu8SectionData, u16DesLen, pPmtTb);
                    pu8SectionData += u16DesLen;
                    s32Length -= u16DesLen;
                }

                pPmtTb->u16AudoNum++;
            }

            break;
        }
#if 1
        case STREAM_TYPE_PRIVATE:
        {
            //printf("private stream...........pid=%#x\n", u16Pid);
            if (u16DesLen > 0 )/* subtitling stream info */
            {
                if  (IsSubtitleStream(pu8SectionData, u16DesLen))
                {
                    if (SUBTITLING_MAX > pPmtTb->u16SubtitlingNum)
                    {
                        pPmtTb->SubtitingInfo[pPmtTb->u16SubtitlingNum].u16SubtitlingPID = u16Pid;

                        //printf("using pPmtTb->u16SubtitlingNum = %d to parse description\n", pPmtTb->u16SubtitlingNum);

                        ParsePMTDesc(pu8SectionData, u16DesLen, pPmtTb);

                        pPmtTb->u16SubtitlingNum++;
                    }
                    else
                    {
                        sample_common_printf("Subtitle language is over than the max number:%d\n", SUBTITLING_MAX);
                    }
                }
                else if (IsARIBCCStream(pu8SectionData, u16DesLen))
                {
                    pPmtTb->u16ARIBCCPid = u16Pid;
                }
                else if (IsTtxStream(pu8SectionData, u16DesLen))
                {
                    if (TTX_MAX > pPmtTb->u16TtxNum)
                    {
                        pPmtTb->stTtxInfo[pPmtTb->u16TtxNum].u16TtxPID = u16Pid;

                        ParsePMTDesc(pu8SectionData, u16DesLen, pPmtTb);

                        pPmtTb->u16TtxNum++;
                    }
                }
                else if (Is265Stream(pu8SectionData, u16DesLen))
                {
                    pPmtTb->Videoinfo[pPmtTb->u16VideoNum].u16VideoPid = u16Pid;
                    pPmtTb->Videoinfo[pPmtTb->u16VideoNum].u32VideoEncType = HI_UNF_VCODEC_TYPE_HEVC;

                    pPmtTb->u16VideoNum ++;
                }
                else
                {
                    if (pPmtTb->u16AudoNum < PROG_MAX_AUDIO)
                    {
                        pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u16AudioPid = u16Pid;

                        // dolby parse
                        if (IsAC3Stream(pu8SectionData, u16DesLen))
                        {
                            pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u32AudioEncType = HA_AUDIO_ID_DOLBY_PLUS;
                        }
                        else
                        {
                            pPmtTb->Audioinfo[pPmtTb->u16AudoNum].u32AudioEncType = HA_AUDIO_ID_MP3;
                        }

                        pPmtTb->u16AudoNum++;
                    }
                }

                pu8SectionData += u16DesLen;
                s32Length -= u16DesLen;
            }

        }
        break;
#endif

    #ifdef STREAM_TYPE_SCTE_82H
        case STREAM_TYPE_SCTE:
        {
            if (u16DesLen > 0)
            {
                ParsePMTDesc(pu8SectionData, u16DesLen, pPmtTb);
                pu8SectionData += u16DesLen;
                s32Length -= u16DesLen;
            }
            pPmtTb->stSCTESubtInfo.u16SCTESubtPID = u16Pid;

            break;
        }
    #endif

        default:
        {
            if (u16DesLen > 0)
            {
                ParsePMTDesc(pu8SectionData, u16DesLen, pPmtTb);
                pu8SectionData += u16DesLen;
                s32Length -= u16DesLen;
            }
        }
        break;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SRH_ParseSDT(const HI_U8 *pu8SectionData, HI_S32 s32Length, HI_U8 *pSectionStruct)
{
    HI_U8 u8EitFlag = 0;
    HI_U8 u8EitFlag_PF = 0;
    HI_U8 u8RunStatus = 0;
    HI_U8 u8FreeCA = 0;
    HI_U16 u16NetworkId = 0x1FFF;
    HI_U16 u16ProgramNumber = 0;
    HI_U16 u16TsId   = 0;
    HI_S32 s32DesLen = 0;

    SDT_TB *pSdtTb = (SDT_TB *)pSectionStruct;
    SDT_INFO *pSdtInfo;

    if ((HI_NULL == pu8SectionData) || (HI_NULL == pSectionStruct))
    {
        return HI_FAILURE;
    }

    if (SDT_TABLE_ID_ACTUAL != pu8SectionData[0])
    {
        return HI_FAILURE;
    }

    s32Length = (HI_S32)((pu8SectionData[1] << 8) | pu8SectionData[2]) & 0x0fff;

    u16TsId = (HI_U16)((pu8SectionData[3] << 8) | pu8SectionData[4]);
    u16NetworkId = (HI_U16)((pu8SectionData[8] << 8) | pu8SectionData[9]);

    pSdtTb->u16TsId  = u16TsId;
    pSdtTb->u16NetID = u16NetworkId;

    pu8SectionData += 11;
    s32Length -= 11;

    while (s32Length > 4)
    {
        pSdtInfo = &(pSdtTb->SdtInfo[pSdtTb->u32ProgNum]);

        u16ProgramNumber = (HI_U16)((pu8SectionData[0] << 8) | pu8SectionData[1]);

        u8EitFlag = (HI_U8)((pu8SectionData[2] & 0x02) >> 1);
        u8EitFlag_PF = (HI_U8)(pu8SectionData[2] & 0x01);

        u8RunStatus = (HI_U8)((pu8SectionData[3] & 0xE0) >> 5);

        u8FreeCA = (HI_U8)((pu8SectionData[3] & 0x10) >> 4);

        pSdtInfo->u16ServiceID = u16ProgramNumber;
        pSdtInfo->u8EitFlag = u8EitFlag;
        pSdtInfo->u8EitFlag_PF = u8EitFlag_PF;
        pSdtInfo->RunState = u8RunStatus;
        pSdtInfo->CAMode = u8FreeCA;

        s32DesLen = (HI_S32)(((pu8SectionData[3] & 0x0F) << 8) | pu8SectionData[4]);

        pu8SectionData += 5;
        s32Length -= 5;

        ParseSDTDesc( pu8SectionData, s32DesLen, pSdtInfo);

        pu8SectionData += s32DesLen;
        s32Length -= s32DesLen;
        pSdtTb->u32ProgNum++;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* Function:      PG_PATRequest
* Description:   get Pat table
* Data Accessed:
* Data Updated:
* Input:             None
* Output:           None
* Return:      >=0 filter ID,HI_FAILURE
* Others:
*****************************************************************************/

HI_S32 SRH_PATRequest(HI_U32 u32DmxID, PAT_TB *pat_tb)
{
    HI_S32 s32Ret;

    DMX_DATA_FILTER_S stDataFilter;

    //printf("\n ++++ PAT Request dmxid = %d \n",u32DmxID);

    memset(stDataFilter.u8Match, 0, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));
    memset(stDataFilter.u8Mask, 0, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));
    memset(stDataFilter.u8Negate, 0, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));

    stDataFilter.u32TSPID   = PAT_TSPID;
    stDataFilter.u32TimeOut = PGPAT_TIMEOUT;
    stDataFilter.u16FilterDepth = 1;
    stDataFilter.u8Crcflag = 0;

    stDataFilter.u8Match[0] = PAT_TABLE_ID;
    stDataFilter.u8Mask[0] = 0x0;

    /* set call back func*/
    stDataFilter.funSectionFunCallback = &SRH_ParsePAT;
    stDataFilter.pSectionStruct = (HI_U8 *)pat_tb;

    /* start data filter*/
    s32Ret = DMX_SectionStartDataFilter(u32DmxID, &stDataFilter);
    if (s32Ret != HI_SUCCESS)
    {
        sample_common_printf("\n No PAT received \n");
    }

    return s32Ret;
}


HI_S32 SRH_PMTRequest(HI_U32 u32DmxID, PMT_TB *pmt_tb, HI_U16 u16PmtPid, HI_U16 u16ServiceId)
{
    HI_S32 s32Ret;
    DMX_DATA_FILTER_S stDataFilter;

#ifdef SRCH_DEBUG
    sample_common_printf("+++++++++++PMT PID %d(0x%x) ServiceID %d(0x%x)++++++++\n", u16PmtPid, u16PmtPid, u16ServiceId, u16ServiceId);
#endif
    memset(stDataFilter.u8Match, 0x00, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));
    memset(stDataFilter.u8Mask, 0xff, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));
    memset(stDataFilter.u8Negate, 0x00, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));

    stDataFilter.u32TSPID   = u16PmtPid;
    stDataFilter.u32TimeOut = PGPMT_TIMEOUT;
    stDataFilter.u16FilterDepth = 3;
    stDataFilter.u8Crcflag = 0;

    stDataFilter.u8Match[0] = PMT_TABLE_ID;
    stDataFilter.u8Mask[0] = 0x0;

    /* set service id*/
    stDataFilter.u8Match[1] = (HI_U8)((u16ServiceId >> 8) & 0x00ff);
    stDataFilter.u8Mask[1] = 0;

    stDataFilter.u8Match[2] = (HI_U8)(u16ServiceId & 0x00ff);
    stDataFilter.u8Mask[2] = 0;

    /* set call back func*/
    stDataFilter.funSectionFunCallback = &SRH_ParsePMT;
    stDataFilter.pSectionStruct = (HI_U8 *)pmt_tb;

    /* start data filter*/
    s32Ret = DMX_SectionStartDataFilter(u32DmxID, &stDataFilter);
    if (s32Ret == HI_FAILURE)
    {
        sample_common_printf("DMX_SectionStartDataFilter failed to receive PMT\n");
    }

#ifdef SRCH_DEBUG
    HI_S32 i;
    sample_common_printf("ServiceID %d  PcrPID %#x  VideoNum %d  AudioNum %d\n", pmt_tb->u16ServiceID,
           pmt_tb->u16PcrPid, pmt_tb->u16VideoNum, pmt_tb->u16AudoNum);
    for (i = 0; i < pmt_tb->u16VideoNum; i++)
    {
        sample_common_printf("Video[%d] PID %#x Vtype %#x\n", i, pmt_tb->Videoinfo[i].u16VideoPid,
               pmt_tb->Videoinfo[i].u32VideoEncType);
    }

    for (i = 0; i < pmt_tb->u16AudoNum; i++)
    {
        sample_common_printf("Audio[%d] PID %#x Atype %#x\n", i, pmt_tb->Audioinfo[i].u16AudioPid,
               pmt_tb->Audioinfo[i].u32AudioEncType);
    }

    sample_common_printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
#endif
    return s32Ret;
}



HI_S32 SRH_SDTRequest(HI_U32 u32DmxID, SDT_TB *sdt_tb)
{
    HI_S32 s32Ret;
    DMX_DATA_FILTER_S stDataFilter;

#ifdef SRCH_DEBUG
    sample_common_printf("\n ++++ SDT Request");
#endif
    memset(stDataFilter.u8Match, 0x00, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));
    memset(stDataFilter.u8Mask, 0xff, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));
    memset(stDataFilter.u8Negate, 0x00, DMX_FILTER_MAX_DEPTH * sizeof(HI_U8));

    stDataFilter.u32TSPID   = SDT_TSPID;                      //set ts pid
    stDataFilter.u32TimeOut = PGSDT_TIMEOUT;    // timeout
    stDataFilter.u16FilterDepth = 4;
    stDataFilter.u8Crcflag = 0;

    stDataFilter.u8Match[0] = SDT_TABLE_ID_ACTUAL;
    stDataFilter.u8Mask[0] = 0x0;

    /* set call back func*/
    stDataFilter.funSectionFunCallback = &SRH_ParseSDT;
    stDataFilter.pSectionStruct = (HI_U8 *)sdt_tb;

     /* start data filter*/
    s32Ret = DMX_SectionStartDataFilter(0, &stDataFilter);
    if (s32Ret == HI_FAILURE)
    {
        sample_common_printf("\n No SDT received\n");
    }

    return (s32Ret);
}

HI_S32 DVB_SearchStart(HI_U32 u32DmxID)
{
    DB_PROGRAM_S stProgInfo;
    PAT_TB pat_tb;
    PMT_TB pmt_tb;
    SDT_TB sdt_tb;

    HI_U32 i, j;
    HI_S32 s32Ret;

    memset(&pat_tb, 0, sizeof(pat_tb));
    memset(&pmt_tb, 0, sizeof(pmt_tb));
    memset(&sdt_tb, 0, sizeof(sdt_tb));

    s32Ret = SRH_PATRequest(u32DmxID, &pat_tb);
    if (s32Ret == HI_FAILURE)
    {
        return HI_FAILURE;
    }

    for (i = 0; i < pat_tb.u16ProgNum; i++)
    {
        memset(&stProgInfo, 0, sizeof(DB_PROGRAM_S));
        stProgInfo.u16FrontendID = SEARCHING_FRONTEND_ID;
        stProgInfo.u16TsID   = pat_tb.u16TsID;
        stProgInfo.u16PmtPid = pat_tb.PatInfo[i].u16PmtPid;
        stProgInfo.u16ServiceID = pat_tb.PatInfo[i].u16ServiceID;

        memset(&pmt_tb, 0, sizeof(PMT_TB));
        if (SRH_PMTRequest(u32DmxID, &pmt_tb, stProgInfo.u16PmtPid, stProgInfo.u16ServiceID) == HI_SUCCESS)
        {
            stProgInfo.u16PcrPid = pmt_tb.u16PcrPid;
            stProgInfo.u8AudioChannel = pmt_tb.u16AudoNum;
            for (j = 0; j <= pmt_tb.u16AudoNum; j++)
            {
                stProgInfo.AudioEX[j].u16audiopid = pmt_tb.Audioinfo[j].u16AudioPid;
                stProgInfo.AudioEX[j].u32AudioEncType = pmt_tb.Audioinfo[j].u32AudioEncType;
            }

            stProgInfo.u8VideoChannel = pmt_tb.u16VideoNum;
            stProgInfo.VideoEX.u16VideoPid = pmt_tb.Videoinfo[0].u16VideoPid;
            stProgInfo.VideoEX.u32VideoEncType = pmt_tb.Videoinfo[0].u32VideoEncType;
        }

        DB_AddDVBProg(&stProgInfo);
    }

    s32Ret = SRH_SDTRequest(u32DmxID, &sdt_tb);
    if (s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    for (i = 0; i < sdt_tb.u32ProgNum; i++)
    {
        memset(&stProgInfo, 0, sizeof(DB_PROGRAM_S));

        s32Ret = DB_GetDVBProgInfoByServiceID(sdt_tb.SdtInfo[i].u16ServiceID, &stProgInfo);
        if (s32Ret == HI_FAILURE)
        {
            continue;
        }

        stProgInfo.u16NetworkID  = sdt_tb.u16NetID;
        stProgInfo.enServiceType = sdt_tb.SdtInfo[i].u32ServiceType;
        memcpy(stProgInfo.ProgramName, sdt_tb.SdtInfo[i].s8ProgName, MAX_PROGNAME_LENGTH);
        DB_SetDVBProgInfo(s32Ret, &stProgInfo);
    }

    return HI_SUCCESS;
}

HI_VOID DVB_SaveSearch(HI_U32 u32FrontendID)
{
    DB_PROGRAM_S stProgInfo;

    HI_U32 i;

    for (i = 0; i < DB_GetProgTotalCount(); i++)
    {
        if (HI_FAILURE == DB_GetDVBProgInfo(i, &stProgInfo))
        {
            break;
        }

        if (stProgInfo.u16FrontendID == SEARCHING_FRONTEND_ID)
        {
            stProgInfo.u16FrontendID = u32FrontendID;
            DB_SetDVBProgInfo(i, &stProgInfo);
        }
    }
}

HI_VOID DVB_ListProg()
{
    HI_S32 i, j;
    DB_PROGRAM_S stProgInfo;

    sample_common_printf("\tList TV program\n");

    for (i = 0; i < DB_GetProgTotalCount(); i++)
    {
        if (HI_FAILURE != DB_GetDVBProgInfo(i, &stProgInfo))
        {
            sample_common_printf("\n%-10d %-20s \n", i, stProgInfo.ProgramName);
            if (stProgInfo.u8VideoChannel > 0)
            {
                sample_common_printf("\tVideo Stream PID = %d(0x%04x) ", stProgInfo.VideoEX.u16VideoPid,
                       stProgInfo.VideoEX.u16VideoPid);
                switch (stProgInfo.VideoEX.u32VideoEncType)
                {
                case HI_UNF_VCODEC_TYPE_H264:
                    sample_common_printf("\tStream Type H264\n");
                    break;
                case HI_UNF_VCODEC_TYPE_MPEG4:
                    sample_common_printf("\tStream Type MPEG4\n");
                    break;
                case HI_UNF_VCODEC_TYPE_MPEG2:
                    sample_common_printf("\tStream Type MPEG2\n");
                    break;
                case HI_UNF_VCODEC_TYPE_HEVC:
                    sample_common_printf("\tStream Type HEVC\n");
                    break;
                default:
                    sample_common_printf("\tStream Type error\n");
                }
            }

            for (j = 0; j < stProgInfo.u8AudioChannel; j++)
            {
                sample_common_printf("\tAudio Stream PID = %d(0x%04x)", stProgInfo.AudioEX[j].u16audiopid,
                       stProgInfo.AudioEX[j].u16audiopid);
                switch (stProgInfo.AudioEX[j].u32AudioEncType)
                {
                case HA_AUDIO_ID_AAC:
                    sample_common_printf("\tStream Type AAC\n");
                    break;
                case HA_AUDIO_ID_MP3:
                    sample_common_printf("\tStream Type MP3\n");
                    break;
                case HA_AUDIO_ID_DOLBY_PLUS:
                    sample_common_printf("\tStream Type AC3\n");
                    break;
                case HA_AUDIO_ID_DTSHD:
                    sample_common_printf("\tStream Type DTSHD\n");
                    break;
                case HA_AUDIO_ID_DRA:
                    sample_common_printf("\tStream Type DRA\n");
                    break;
                default:
                    sample_common_printf("\tStream Type error\n");
                }
            }
        }
    }

    sample_common_printf("\n\n");
}

static HI_BOOL g_bSrchInit = HI_FALSE;

HI_VOID HIADP_Search_Init()
{
    if (HI_FALSE == g_bSrchInit)
    {
        g_bSrchInit = HI_TRUE;
    }

    return;
}

/*get PMT table*/
HI_S32  HIADP_Search_GetAllPmt(HI_U32 u32DmxId, PMT_COMPACT_TBL **ppProgTable)
{
    HI_S32 i, j, s32Ret;
    PAT_TB pat_tb;
    PMT_TB pmt_tb;
    SDT_TB sdt_tb;
    PMT_COMPACT_TBL *pProgTable = HI_NULL;
    HI_U32 u32Promnum = 0;
    HI_U32 u32Cnt = 0;

    HI_U32  u32SubtitleIndex = 0;
    HI_U32 u32SubtDespIndex = 0;

    if(!ppProgTable)
    {
        sample_common_printf("para is null pointer!\n");
        return HI_FAILURE;
    }

    memset(&pat_tb, 0, sizeof(pat_tb));
    memset(&pmt_tb, 0, sizeof(pmt_tb));
    memset(&sdt_tb, 0, sizeof(sdt_tb));

    do
    {
        s32Ret = SRH_PATRequest(u32DmxId, &pat_tb);
        if (HI_SUCCESS != s32Ret)
        {
            sample_common_printf("failed to search PAT\n");
            return HI_FAILURE;
        }

        if (0 != pat_tb.u16ProgNum)
        {
            break;
        }
        usleep(5000);
        u32Cnt += 5;
    }while(u32Cnt < 5000);

    pProgTable = (PMT_COMPACT_TBL*)malloc(sizeof(PMT_COMPACT_TBL));
    if (HI_NULL == pProgTable)
    {
        sample_common_printf("have no memory for pat\n");
        return HI_FAILURE;
    }

    pProgTable->prog_num = pat_tb.u16ProgNum;
    pProgTable->proginfo = (PMT_COMPACT_PROG*)malloc(pat_tb.u16ProgNum * sizeof(PMT_COMPACT_PROG));
    if (HI_NULL == pProgTable->proginfo)
    {
        sample_common_printf("have no memory for pat\n");
        free(pProgTable);
        pProgTable = HI_NULL;
        return HI_FAILURE;
    }

    memset(pProgTable->proginfo, 0, pat_tb.u16ProgNum * sizeof(PMT_COMPACT_PROG));

    sample_common_printf("\n\nALL Program Infomation[%d]:\n",pat_tb.u16ProgNum);


    for (i = 0; i < pat_tb.u16ProgNum; i++)
    {
        if ((pat_tb.PatInfo[i].u16ServiceID == 0) || (pat_tb.PatInfo[i].u16PmtPid == 0x1fff))
        {
            continue;
        }

        memset(&pmt_tb, 0, sizeof(PMT_TB));
        s32Ret = SRH_PMTRequest(u32DmxId, &pmt_tb, pat_tb.PatInfo[i].u16PmtPid, pat_tb.PatInfo[i].u16ServiceID);
        if (HI_SUCCESS != s32Ret)
        {
            sample_common_printf("failed to search PMT[%d]\n", i);
            continue;
        }

        pProgTable->proginfo[u32Promnum].ProgID = pat_tb.PatInfo[i].u16ServiceID;
        pProgTable->proginfo[u32Promnum].PmtPid = pat_tb.PatInfo[i].u16PmtPid;
        pProgTable->proginfo[u32Promnum].PcrPid = pmt_tb.u16PcrPid;
        pProgTable->proginfo[u32Promnum].VideoType   = pmt_tb.Videoinfo[0].u32VideoEncType;
        pProgTable->proginfo[u32Promnum].VElementNum = pmt_tb.u16VideoNum;
        pProgTable->proginfo[u32Promnum].VElementPid = pmt_tb.Videoinfo[0].u16VideoPid;
        pProgTable->proginfo[u32Promnum].AudioType   = pmt_tb.Audioinfo[0].u32AudioEncType;
        pProgTable->proginfo[u32Promnum].AElementNum = pmt_tb.u16AudoNum;
        pProgTable->proginfo[u32Promnum].AElementPid = pmt_tb.Audioinfo[0].u16AudioPid;

        pProgTable->proginfo[u32Promnum].u16SubtitlingNum = pmt_tb.u16SubtitlingNum;
        pProgTable->proginfo[u32Promnum].stSCTESubtInfo.u16SCTESubtPID= pmt_tb.stSCTESubtInfo.u16SCTESubtPID;
        pProgTable->proginfo[u32Promnum].u16ClosedCaptionNum= pmt_tb.u16ClosedCaptionNum;
        memcpy(pProgTable->proginfo[u32Promnum].stClosedCaption, pmt_tb.stClosedCaption, sizeof(pmt_tb.stClosedCaption));
        pProgTable->proginfo[u32Promnum].u16ARIBCCPid = pmt_tb.u16ARIBCCPid;
        pProgTable->proginfo[u32Promnum].u16TtxNum = pmt_tb.u16TtxNum;
        memcpy(pProgTable->proginfo[u32Promnum].stTtxInfo, pmt_tb.stTtxInfo, sizeof(PMT_TTX_S)*TTX_MAX);
        memcpy(pProgTable->proginfo[u32Promnum].u8PmtData, pmt_tb.u8PmtData, pmt_tb.u32PmtLen);
        pProgTable->proginfo[u32Promnum].u32PmtLen = pmt_tb.u32PmtLen;

        for(j=0; j<pmt_tb.u16AudoNum; j++)
        {
            /* added by gaoyanfeng 00182102 for multi-audio begin */
            if (j < PROG_MAX_AUDIO)
            {
                pProgTable->proginfo[u32Promnum].Audioinfo[j].u16AudioPid = pmt_tb.Audioinfo[j].u16AudioPid;
                pProgTable->proginfo[u32Promnum].Audioinfo[j].u32AudioEncType = pmt_tb.Audioinfo[j].u32AudioEncType;
            }
            /* added by gaoyanfeng 00182102 for multi-audio end */
        }

       pProgTable->proginfo[u32Promnum].u16CANum = pmt_tb.u16CANum;
       for (j=0; j<pmt_tb.u16CANum; j++)
       {
           if (j < PROG_MAX_CA)
           {
                pProgTable->proginfo[u32Promnum].CASystem[j].u16CASystemID = pmt_tb.CASystem[j].u16CASystemID;
                pProgTable->proginfo[u32Promnum].CASystem[j].u16CAPID = pmt_tb.CASystem[j].u16CAPID;
           }
        }
        //printf("pProgTable->proginfo[%d].u16SubtitlingNum ============ %d\n", u32Promnum, pProgTable->proginfo[u32Promnum].u16SubtitlingNum);

        /* parse and deal with subtitling info */
        for (u32SubtitleIndex=0; u32SubtitleIndex<pProgTable->proginfo[u32Promnum].u16SubtitlingNum; u32SubtitleIndex++)
        {
            pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].u16SubtitlingPID =
                pmt_tb.SubtitingInfo[u32SubtitleIndex].u16SubtitlingPID;

            pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].u8DesInfoCnt = pmt_tb.SubtitingInfo[u32SubtitleIndex].u8DesInfoCnt;

            //printf("pProgTable->proginfo[%d].SubtitingInfo[%d].u8DesInfoCnt ============ %d\n",u32Promnum, u32SubtitleIndex, pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].u8DesInfoCnt);

            for (u32SubtDespIndex=0; u32SubtDespIndex<pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].u8DesInfoCnt; u32SubtDespIndex++)
            {
                pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u32LangCode =
                    pmt_tb.SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u32LangCode;

                pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u16PageID =
                    pmt_tb.SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u16PageID;

                pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u16AncillaryPageID =
                    pmt_tb.SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u16AncillaryPageID;

#if 0
                sample_common_printf("pid:0x%02x,desp:%u, page id:%u, ancilary page id:%u, lang code: %#x\n", pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].u16SubtitlingPID,
                           u32SubtDespIndex,
                           pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u16PageID
                           , pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u16AncillaryPageID
                           , pProgTable->proginfo[u32Promnum].SubtitingInfo[u32SubtitleIndex].DesInfo[u32SubtDespIndex].u32LangCode);
#endif
            }
        }

        sample_common_printf("Channel Num = %d, Program ID = %d PMT PID = 0x%x,\n", u32Promnum + 1, pat_tb.PatInfo[i].u16ServiceID,
               pat_tb.PatInfo[i].u16PmtPid);
        for (j = 0; j < pmt_tb.u16AudoNum; j++)
        {
            sample_common_printf("\tAudio Stream PID   = 0x%x\n", pmt_tb.Audioinfo[j].u16AudioPid);

            switch (pmt_tb.Audioinfo[j].u32AudioEncType)
            {
            case HA_AUDIO_ID_MP3:
                sample_common_printf("\tAudio Stream Type MP3\n");
                break;
            case HA_AUDIO_ID_AAC:
                sample_common_printf("\tAudio Stream Type AAC\n");
                break;
            case HA_AUDIO_ID_DOLBY_PLUS:
                sample_common_printf("\tAudio Stream Type AC3\n");
                break;
            case HA_AUDIO_ID_DTSHD:
                sample_common_printf("\tAudio Stream Type DTS\n");
                break;
            default:
                sample_common_printf("\tAudio Stream Type error\n");
            }
        }

        for (j = 0; j < pmt_tb.u16VideoNum; j++)
        {
            sample_common_printf("\tVideo Stream PID   = 0x%x\n",pmt_tb.Videoinfo[j].u16VideoPid);
            switch (pmt_tb.Videoinfo[j].u32VideoEncType)
            {
            case HI_UNF_VCODEC_TYPE_H264:
                sample_common_printf("\tVideo Stream Type H264\n");
                break;
            case HI_UNF_VCODEC_TYPE_MPEG2:
                sample_common_printf("\tVideo Stream Type MP2\n");
                break;
            case HI_UNF_VCODEC_TYPE_MPEG4:
                sample_common_printf("\tVideo Stream Type MP4\n");
                break;
            case HI_UNF_VCODEC_TYPE_HEVC:
                sample_common_printf("\tVideo Stream Type HEVC\n");
                break;
            case HI_UNF_VCODEC_TYPE_AVS:
                sample_common_printf("\tVideo Stream Type AVS\n");
                break;
            default:
                sample_common_printf("\tVideo Stream Type error\n");
            }
        }

        if(pmt_tb.u16SubtitlingNum > 0)
        {
            pProgTable->proginfo[u32Promnum].SubtType |= SUBT_TYPE_DVB;
            sample_common_printf("\tDVB subtitle NUM   = 0x%x\n",pmt_tb.u16SubtitlingNum);
        }
        if(pmt_tb.stSCTESubtInfo.u16SCTESubtPID)
        {
            pProgTable->proginfo[u32Promnum].SubtType |= SUBT_TYPE_SCTE;
            sample_common_printf("\tSCTE subtitle PID  = 0x%x\n",pmt_tb.stSCTESubtInfo.u16SCTESubtPID);
        }
        if(pProgTable->proginfo[u32Promnum].u16ClosedCaptionNum > 0)
        {
            //HI_U8 i = 0;

            for(j = 0; j < pProgTable->proginfo[u32Promnum].u16ClosedCaptionNum; j++)
            {
                if(pProgTable->proginfo[u32Promnum].stClosedCaption[j].u8IsDigitalCC)
                {
                    sample_common_printf("\tClosed Captioning 708, language : %#x, service num : %d\n",
                        pProgTable->proginfo[u32Promnum].stClosedCaption[j].u32LangCode,
                        pProgTable->proginfo[u32Promnum].stClosedCaption[j].u8ServiceNumber);
                }
                else
                {
                    sample_common_printf("\tClosed Captioning 608\n");
                }
            }
        }
        if(pProgTable->proginfo[u32Promnum].u16ARIBCCPid)
        {
            sample_common_printf("\tARIB CC PID            = 0x%x\n",pProgTable->proginfo[u32Promnum].u16ARIBCCPid);
        }

        if(pProgTable->proginfo[u32Promnum].u16TtxNum > 0)
        {
            sample_common_printf("\tTeletext NUM           = 0x%x\n",pmt_tb.u16TtxNum);
            for(j = 0; j < pProgTable->proginfo[u32Promnum].u16TtxNum; j++)
            {
                sample_common_printf("\tTeletext%d PID            = 0x%x\n",j, pmt_tb.stTtxInfo[j].u16TtxPID);
            }
        }

        if ((pmt_tb.u16VideoNum > 0) || (pmt_tb.u16AudoNum > 0))
        {
            u32Promnum++;
        }
    }

    pProgTable->prog_num = u32Promnum;

    sample_common_printf("\n");

    *ppProgTable = pProgTable;

    return u32Promnum ? HI_SUCCESS : HI_FAILURE;
}

HI_S32  HIADP_Search_FreeAllPmt(PMT_COMPACT_TBL *pProgTable)
{
    if (HI_NULL != pProgTable)
    {
        if (HI_NULL != pProgTable->proginfo)
        {
            free(pProgTable->proginfo);
            pProgTable->proginfo = HI_NULL;
        }

        free(pProgTable);
        pProgTable = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_VOID HIADP_Search_DeInit()
{
    if (HI_TRUE == g_bSrchInit)
    {
        g_bSrchInit = HI_FALSE;
    }

    return;
}
