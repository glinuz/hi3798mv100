#ifndef _COMMON_SEARCH_H__
#define _COMMON_SEARCH_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/********************Descriptor flag definition******************/
/********************CNcomment:描述符标识符定义******************/

#define STREAM_TYPE_11172_VIDEO         0x01
#define STREAM_TYPE_13818_VIDEO         0x02
#define STREAM_TYPE_11172_AUDIO         0x03
#define STREAM_TYPE_13818_AUDIO         0x04
#define STREAM_TYPE_14496_2_VIDEO       0x10    // MPEG4
#define STREAM_TYPE_14496_10_VIDEO      0x1B    // H264
#define STREAM_TYPE_AVS_VIDEO           0x42    // AVS
#define STREAM_TYPE_HEVC_VIDEO          0x24    // HEVC
#define STREAM_TYPE_13818_7_AUDIO       0x0F    // AAC
#define STREAM_TYPE_14496_3_AUDIO       0x11    // AAC
#define STREAM_TYPE_AC3_AUDIO           0x81    // AC3
#define STREAM_TYPE_SCTE                0x82    // TS packets containing SCTE data
#define STREAM_TYPE_DTS_AUDIO           0x82    // DTS
#define STREAM_TYPE_DOLBY_TRUEHD_AUDIO  0x83    // dolby true HD
#define STREAM_TYPE_DTS_MA				0x86	// DTS MA which conflict with CAPTION_SERVICE_DESCRIPTOR
#define STREAM_TYPE_PRIVATE             0x06    // PES packets containing private data

#define VIDEO_STREAM_DESCRIPTOR 0x02
#define AUDIO_STREAM_DESCRIPTOR 0x03
#define HIERACHY_DESCRIPTOR 0x04
#define REGISTRATION_DESCRIPTOR 0x05
#define DATA_STREAM_ALIGNMENT_DESCRIPTOR 0x06
#define TARGET_BACKGROUND_GRID_DESCRIPTOR 0x07
#define VIDEO_WINDOW_DESCRIPTOR 0x08
#define CA_DESCRIPTOR 0x09
#define LANGUAGE_DESCRIPTOR 0x0A
#define SYSTEM_CLOCK_DESCRIPTOR 0x0B
#define MULTIPLEX_BUFFER_USAGE_DESCRIPTOR 0x0C
#define COPYRIGHT_DESCRIPTOR 0x0D
#define MAXIMUM_BITRATE_DESCRIPTOR 0x0E
#define PRIVATE_DATA_INDICATOR_DESCRIPTOR 0x0F
#define SMOOTHING_BUFFER_DESCRIPTOR 0x10
#define STD_DESCRIPTOR 0x11
#define IBP_DESCRIPTOR 0x12

#define NETWORK_NAME_DESCRIPTOR 0x40
#define SERVICE_LIST_DESCRIPTOR 0x41
#define STUFFING_DESCRIPTOR 0x42
#define SATELLITE_DELIVERY_DESCRIPTOR 0x43
#define CABLE_DELIVERY_DESCRIPTOR 0x44
#define BOUQUET_NAME_DESCRIPTOR 0x47
#define SERVICE_DESCRIPTOR 0x48
#define COUNTRY_AVAILABILITY_DESCRIPTOR 0x49
#define LINKAGE_DESCRIPTOR 0x4A
#define NVOD_REFERENCE_DESCRIPTOR 0x4B
#define TIME_SHIFTED_SERVICE_DESCRIPTOR 0x4C
#define SHORT_EVENT_DESCRIPTOR 0x4D
#define EXTENDED_EVENT_DESCRIPTOR 0x4E
#define TIME_SHIFTED_EVENT_DESCRIPTOR 0x4F
#define COMPONENT_DESCRIPTOR 0x50
#define MOSAIC_DESCRIPTOR 0x51
#define STREAM_IDENTIFIER_DESCRIPTOR 0x52
#define CA_IDENTIFIER_DESCRIPTOR 0x53
#define CONTENT_DESCRIPTOR 0x54
#define PARENTAL_RATING_DESCRIPTOR 0x55
#define TELETEXT_DESCRIPTOR 0x56
#define TELEPHONE_DESCRIPTOR 0x57
#define LOCAL_TIME_OFFSET_DESCRIPTOR 0x58
#define SUBTITLING_DESCRIPTOR 0x59
#define TERRESTRIAL_DELIVERY_DESCRIPTOR 0x5A
#define MULTILINGUAL_NETWORK_NAME_DESCRIPTOR 0x5B
#define MULTILINGUAL_BOUQUET_NAME_DESCRIPTOR 0x5C
#define MULTILINGUAL_SERVICE_NAME_DESCRIPTOR 0x5D
#define MULTILINGUAL_COMPONENT_DESCRIPTOR 0x5E
#define PRIVATE_DATA_SPECIFIER_DESCRIPTOR 0x5F
#define SERVICE_MOVE_DESCRIPTOR 0x60
#define SHORT_SMOOTHING_BUFFER_DESCRIPTOR 0x61
#define FREQUENCY_LIST_DESCRIPTOR 0x62
#define PARTIAL_TRANSPORT_STREAM_DESCRIPTOR 0x63
#define DATA_BROADCAST_DESCRIPTOR 0x64
#define CA_SYSTEM_DESCRIPTOR 0x65
#define DATA_BROADCAST_ID_DESCRIPTOR 0x66
#define TRANSPORT_STREAM_DESCRIPTOR 0x67
#define DSNG_DESCRIPTOR 0x68
#define PDC_DESCRIPTOR 0x69
#define AC3_DESCRIPTOR 0x6A
#define AC3_PLUS_DESCRIPTOR 0x7A
#define ANCILLARY_DATA_DESCRIPTOR 0x6B
#define CELL_LIST_DESCRIPTOR 0x6C
#define CELL_FREQUENCY_LINK_DESCRIPTOR 0x6D
#define ANNOUNCEMENT_SUPPORT_DESCRIPTOR 0x6E

#define AC3_EXT_DESCRIPTOR 0x52

#define CAPTION_SERVICE_DESCRIPTOR 0x86

#define STREAM_TYPE_HEVC_VIDEO_IDENTIFY 0x48455643

#define MAX_PMT_LEN     1024
/***********TS PID defintion**************/

#define PAT_TSPID (0x0000)
#define CAT_TSPID (0x0001)
#define NIT_TSPID (0x0010)
#define EIT_TSPID (0x0012)
#define TOT_TSPID (0x0014)
#define TDT_TSPID (0x0014)
#define SDT_TSPID (0x0011)
#define BAT_TSPID (0x0011)

#define INVALID_TSPID (0x1fff)

/***********Table ID defintion**************/
#define PAT_TABLE_ID (0x00)
#define CAT_TABLE_ID (0x01)
#define PMT_TABLE_ID (0x02)
#define NIT_TABLE_ID_ACTUAL (0x40)
#define NIT_TABLE_ID_OTHER (0x41)

#define SDT_TABLE_ID_ACTUAL (0x42)
#define SDT_TABLE_ID_OTHER (0x46)

#define BAT_TABLE_ID (0x4A)
#define EIT_TABLE_ID_PF_ACTUAL (0x4E)
#define EIT_TABLE_ID_PF_OTHER (0x4F)
#define EIT_TABLE_ID_SCHEDULE_ACTUAL_LOW (0x50)
#define EIT_TABLE_ID_SCHEDULE_ACTUAL_HIGH (0x5F)

#define EIT_TABLE_ID_SCHEDULE_OTHER_LOW (0x60)
#define EIT_TABLE_ID_SCHEDULE_OTHER_HIGH (0x6F)

#define TDT_TABLE_ID (0x70)
#define TOT_TABLE_ID (0x73)

#define INVALID_TABLE_ID (0xff)
#define CHANNEL_MAX_PROG    256
#define PROG_MAX_VIDEO      8
#define PROG_MAX_AUDIO      8
#define PROG_MAX_CA         8

#define SUBTDES_INFO_MAX 10
#define SUBTITLING_MAX 15
#define CAPTION_SERVICE_MAX 16
#define TTX_DES_MAX    10
#define TTX_MAX        15

#define SUBT_TYPE_DVB  (0x1)
#define SUBT_TYPE_SCTE (0x2)
#define SUBT_TYPE_BOTH (SUBT_TYPE_DVB | SUBT_TYPE_SCTE)

typedef struct hiPAT_INFO_S
{
    HI_U16 u16ServiceID;    /*Progam 's SERVICE ID*/
    HI_U16 u16PmtPid;        /*Progam 's PMT ID*/
} PAT_INFO;

typedef struct hiPAT_TB_S
{
    HI_U16 u16ProgNum;
    HI_U16 u16TsID;
    PAT_INFO PatInfo[CHANNEL_MAX_PROG];
} PAT_TB;

typedef struct hiPMT_VIDEO_S
{
    HI_U32 u32VideoEncType;
    HI_U16 u16VideoPid;
} PMT_VIDEO;

typedef struct hiPMT_AUDIO_S
{
    HI_U32 u32AudioEncType;
    HI_U16 u16AudioPid;
} PMT_AUDIO;
typedef struct hiPMT_CA_S
{
    HI_U16 u16CASystemID;
    HI_U16 u16CAPID ;
} PMT_CA;

typedef struct hiPMP_SUBTITLE_DES_S
{
    HI_U32 u32LangCode; /* low 24-bit valid */
    HI_U8  u8SubtitleType;
    HI_U16 u16PageID;
    HI_U16 u16AncillaryPageID;

}PMP_SUBTITLE_DES;

typedef struct hiPMT_SUBTITLE_S
{
    HI_U16 u16SubtitlingPID;

    HI_U8 u8DesTag; /*  */
    HI_U8 u8DesLength;

    HI_U8 u8DesInfoCnt;

    PMP_SUBTITLE_DES DesInfo[SUBTDES_INFO_MAX];

}PMT_SUBTITLE;

typedef struct hiPMT_SCTE_SUBTITLE_S
{

    HI_U16 u16SCTESubtPID;
    HI_U32 u32LanguageCode;
}PMT_SCTE_SUBTITLE_S;

typedef struct hiPMT_CLOSED_CAPTION_S
{
    HI_U32 u32LangCode;
    HI_U8 u8IsDigitalCC;
    HI_U8 u8ServiceNumber;
    HI_U8 u8IsEasyReader;
    HI_U8 u8IsWideAspectRatio;
}PMT_CLOSED_CAPTION_S;

typedef struct hiPMT_TTX_DES_S
{
    HI_U32 u32ISO639LanguageCode; /* low 24-bit valid */
    HI_U8  u8TtxType;
    HI_U8  u8TtxMagazineNumber;
    HI_U8  u8TtxPageNumber;
} PMT_TTX_DES_S;

typedef struct hiPMT_TTX_S
{
    HI_U16 u16TtxPID;

    HI_U8 u8DesTag;
    HI_U8 u8DesLength;

    HI_U8 u8DesInfoCnt;

    PMT_TTX_DES_S stTtxDes[TTX_DES_MAX];

} PMT_TTX_S;


typedef struct hiPMT_TB_S
{
    HI_U16 u16ServiceID;
    HI_U16 u16PcrPid;
    HI_U16 u16VideoNum;
    HI_U16 u16AudoNum;
    HI_U16 u16CANum;
    PMT_VIDEO Videoinfo[PROG_MAX_VIDEO];
    PMT_AUDIO Audioinfo[PROG_MAX_AUDIO];
    PMT_CA CASystem[PROG_MAX_CA];

    HI_U16 u16SubtitlingNum;
    PMT_SUBTITLE SubtitingInfo[SUBTITLING_MAX];
    PMT_SCTE_SUBTITLE_S stSCTESubtInfo;
    HI_U16 u16ClosedCaptionNum;
    PMT_CLOSED_CAPTION_S stClosedCaption[CAPTION_SERVICE_MAX];
    HI_U16 u16ARIBCCPid;

    HI_U16 u16TtxNum;
    PMT_TTX_S stTtxInfo[TTX_MAX];
    HI_U8  u8PmtData[MAX_PMT_LEN];
    HI_U32 u32PmtLen;
} PMT_TB;

typedef enum hiRUN_STATE_E
{
    UnDefined = 0,
    NotRun,
    StartInSeconds,
    Pause,
    Running,
    Run_Reserved1,
    Run_Reserved2,
    Run_Reserved3
} RUN_STATE_E;

typedef enum hiCA_MODE_E
{
    CA_NotNeed = 0,
    CA_Need
} CA_MODE_E;

typedef struct hiSDT_INFO_S
{
    HI_U16          u16ServiceID;
    HI_U8       u8EitFlag;
    HI_U8       u8EitFlag_PF;
    RUN_STATE_E RunState;
    CA_MODE_E   CAMode;

    HI_U32      u32ServiceType;
    HI_S8   s8ProgName[32];
} SDT_INFO;

typedef struct hiSDT_TB_S
{
    HI_U32 u32ProgNum;
    HI_U16 u16TsId;
    HI_U16 u16NetID;
    SDT_INFO SdtInfo[CHANNEL_MAX_PROG];
} SDT_TB;


typedef struct hiPMT_COMPACT_PROG_S
{
    HI_U32 ProgID;          /* program ID */
    HI_U32 PmtPid;          /*program PMT PID*/
    HI_U32 PmtRemapPid; /* overlapped when remux, remap to new pid */
    HI_U32 PcrPid;          /*program PCR PID*/

    HI_U32   VideoType;
    HI_U16               VElementNum;        /* video stream number */
    HI_U16               VElementPid;        /* the first video stream PID*/
    HI_U16               VElementRemapPid; /* overlapped when remux, remap to new pid */

    HI_U32   AudioType;
    HI_U16               AElementNum;        /* audio stream number */
    HI_U16               AElementPid;        /* the first audio stream PID*/
    HI_U16               AElementRemapPid; /* overlapped when remux, remap to new pid */

    HI_U16 u16CANum;
    PMT_CA CASystem[PROG_MAX_CA];
    PMT_AUDIO Audioinfo[PROG_MAX_AUDIO];     /* multi-audio info, added by gaoyanfeng 00182102 */

    HI_U32              SubtType;            /*0---NONE,1---DVB,2---SCTE,3---BOTH*/
    HI_U16              u16SubtitlingNum;
    PMT_SUBTITLE        SubtitingInfo[SUBTITLING_MAX];
    PMT_SCTE_SUBTITLE_S stSCTESubtInfo;
    HI_U16               u16ClosedCaptionNum;
    PMT_CLOSED_CAPTION_S stClosedCaption[CAPTION_SERVICE_MAX];
    HI_U16              u16ARIBCCPid;

    HI_U16 u16TtxNum;
    PMT_TTX_S stTtxInfo[TTX_MAX];
    HI_U8  u8PmtData[MAX_PMT_LEN];
    HI_U32 u32PmtLen;
} PMT_COMPACT_PROG;

typedef struct hiPMT_COMPACT_TBL_S
{
    HI_U32            prog_num;
    PMT_COMPACT_PROG *proginfo;
} PMT_COMPACT_TBL;

HI_S32      DVB_SearchStart(HI_U32 u32DmxID);
HI_VOID     DVB_SaveSearch(HI_U32 u32FrontendID);
HI_VOID     DVB_ListProg();
HI_S32      SRH_ParseSDT(const HI_U8 *pu8SectionData, HI_S32 s32Length, HI_U8 *pSectionStruct);
HI_S32      SRH_ParsePMT ( const HI_U8 *pu8SectionData, HI_S32 s32Length, HI_U8 *pSectionStruct);
HI_S32      SRH_ParsePAT( const HI_U8  *pu8SectionData, HI_S32 s32Length, HI_U8 *pSectionStruct);
HI_S32      SRH_PATRequest(HI_U32 u32DmxID,PAT_TB *pat_tb);
HI_S32      SRH_PMTRequest(HI_U32 u32DmxID,PMT_TB *pmt_tb,HI_U16 u16PmtPid, HI_U16 u16ServiceId);
HI_S32      SRH_SDTRequest(HI_U32 u32DmxID,SDT_TB *sdt_tb);
/******************************************Search public interface***********************************/
HI_VOID     HIADP_Search_Init();
HI_S32      HIADP_Search_GetAllPmt(HI_U32 u32DmxId,PMT_COMPACT_TBL **ppProgTable);
HI_S32      HIADP_Search_FreeAllPmt(PMT_COMPACT_TBL *pProgTable);
HI_VOID     HIADP_Search_DeInit();

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif /*__SEARCH_H__*/
