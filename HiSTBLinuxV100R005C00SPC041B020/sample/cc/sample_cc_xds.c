

/*******************************************************************************
*                       Include files
*******************************************************************************/

#include <string.h>
#include <stdio.h>

#include "hi_debug.h"

#include "sample_cc_xds.h"

#define HI_FATAL_CC(fmt...)      HI_FATAL_PRINT(HI_ID_CC, fmt)
#define HI_ERR_CC(fmt...)        HI_ERR_PRINT(HI_ID_CC, fmt)
#define HI_WARN_CC(fmt...)       HI_WARN_PRINT(HI_ID_CC, fmt)
#define HI_INFO_CC(fmt...)       HI_INFO_PRINT(HI_ID_CC, fmt)
#define XDS_TRACE(fmt,args...)   printf(fmt,##args)


/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define MAX_PROGRAM_NAME_BYTE       33
#define MAX_CAPTION_SERVICES_LEN    8
#define MAX_COMPOSITE_PACKET_LEN    32
#define MAX_PROGRAM_NUM             8
#define MAX_PROGRAM_DESCRIPTION_LEN 32
#define MAX_CALLLETTER_LEN          6
#define MAX_TSID_LEN                4
#define MAX_SUPPLEMT_DATA_LEN       32
#define MAX_CHANNELMAP_HEADER_LEN   4
#define MAX_CHANNELMAP_PACKET_LEN   10
#define MAX_NWS_CODE_LEN            11
#define MAX_NWS_DATA_LEN            32


#define MIN_CC608_PROGNAME_LEN          2
#define MAX_CC608_PROGNAME_LEN          32
#define MIN_CC608_PROGTYPE_LEN          2
#define MAX_CC608_PROGTYPE_LEN          32
#define MIN_CC608_CAPTSERV_LEN          2
#define MAX_CC608_CAPTSERV_LEN          8
#define MAX_CC608_PROG_NUM              8
#define MAX_CC608_PROGDESC_LEN          32
#define MIN_CC608_NETNAME_LEN           2
#define MAX_CC608_NETNAME_LEN           32
#define MIN_CC608_CALLLET_LEN           4
#define MAX_CC608_CALLLET_LEN           6
#define MIN_CC608_SUPPDATA_LEN          2
#define MAX_CC608_SUPPDATA_LEN          32
#define MAX_CC608_CHID_LEN              6
#define MAX_CC608_NWSCODE_LEN           32
#define MAX_CC608_NWSMSG_LEN            32

/*****************************************************************************
*                       Type Definitions
*****************************************************************************/

/**Class Definitions in XDS Packet*//** CNcomment:XDS的分类 */
typedef enum tagXDS_CLASS_E
{
    XDS_CLASS_CUR,       /**<Current class*//**<CNcomment:Current类 */
    XDS_CLASS_FUT,       /**<Future class*//**<CNcomment:Future类 */
    XDS_CLASS_CHAN,      /**<Channel Information class*//**<CNcomment:Channel类 */
    XDS_CLASS_MISC,      /**<Miscellaneous class*//**<CNcomment:Miscellaneous类 */
    XDS_CLASS_PUB,       /**<Public Service class*//**<CNcomment:Public Service类 */
    XDS_CLASS_RESV,      /**<Reserved class*//**<CNcomment:预留 */
    XDS_CLASS_PRIV,      /**<Private Data class*//**<CNcomment:Private Data类 */
    XDS_CLASS_BUTT
} XDS_CLASS_E;

/**Type of current Class*//** CNcomment:Current类的类别 */
typedef enum tagXDS_CUR_TYPE_E
{
    XDS_CUR_PRG_ID        =   0x1,  /**<Program Identification Number*//**<CNcomment:节目的开始时间和日期 */
    XDS_CUR_TIMEINSHOW    =   0x2,  /**<Length/Time-in-Show*//**<CNcomment:节目的总时间 */
    XDS_CUR_PRG_NAME      =   0x3,  /**<Program Name*//**<CNcomment:节目名字 */
    XDS_CUR_PRG_TYPE      =   0x4,  /**<Program Type*//**<CNcomment:节目类型 */
    XDS_CUR_CONT_ADVSR    =   0x5,  /**<Content Advisory*//**<CNcomment:节目内容的限制级别 */
    XDS_CUR_AUD_SERVC     =   0x6,  /**<Audio Services*//**<CNcomment:音频节目的内容服务 */
    XDS_CUR_CAP_SERVC     =   0x7,  /**<Caption Services*//**<CNcomment:字幕服务 */
    XDS_CUR_CPY_REDIST    =   0x8,  /**<Copy and Redistribution Control Packet*//**<CNcomment:内容拷贝和发布控制 */
    XDS_CUR_COMP_1        =   0xC,  /**<Composite Packet-1*//**<CNcomment:综合内容包 */
    XDS_CUR_COMP_2        =   0xD,  /**<Composite Packet-2*//**<CNcomment:综合内容包 */
    XDS_CUR_PRG_DESC1     =   0x10, /**<Program Description Row 1*//**<CNcomment:节目的内容描述 */
    XDS_CUR_PRG_DESC2     =   0x11, /**<Program Description Row 2*//**<CNcomment:节目的内容描述 */
    XDS_CUR_PRG_DESC3     =   0x12, /**<Program Description Row 3*//**<CNcomment:节目的内容描述 */
    XDS_CUR_PRG_DESC4     =   0x13, /**<Program Description Row 4*//**<CNcomment:节目的内容描述 */
    XDS_CUR_PRG_DESC5     =   0x14, /**<Program Description Row 5*//**<CNcomment:节目的内容描述 */
    XDS_CUR_PRG_DESC6     =   0x15, /**<Program Description Row 6*//**<CNcomment:节目的内容描述 */
    XDS_CUR_PRG_DESC7     =   0x16, /**<Program Description Row 7*//**<CNcomment:节目的内容描述 */
    XDS_CUR_PRG_DESC8     =   0x17, /**<Program Description Row 8*//**<CNcomment:节目的内容描述 */
    XDS_CUR_TYPE_BUTT
} XDS_CUR_TYPE_E;

/**Type of Channel Information Class*//** CNcomment:频道信息类的类别 */
typedef enum tagXDS_CHAN_TYPE
{
    XDS_CHAN_NET_NAME      =   0x1,  /**<Network Name*//**<CNcomment:网络名称 */
    XDS_CHAN_CALL_LETTER   =   0x2,  /**<Call Letters (Station ID) and Native Channel*//**<CNcomment:Station ID或者Native Channel */
    XDS_CHAN_TAPE_DELAY    =   0x3,  /**<Tape Delay*//**<CNcomment:Tape延迟 */
    XDS_CHAN_TRNS_SIGNID   =   0x4,  /**<Transmission Signal Identifier (TSID)*//**<CNcomment:传输信号标识 */
    XDS_CHAN_TYPE_BUTT
} XDS_CHAN_TYPE;

/**Type of Miscellaneous Class*//** CNcomment:Miscellaneous类的类别 */
typedef enum tagXDS_MISC_TYPE
{
    XDS_MISC_TIMEOFDAY     =   0x1,  /**<Time of Day*//**<CNcomment:当前时间 */
    XDS_MISC_IMPL_CAPTID   =   0x2,  /**<Impulse Capture ID*//**<CNcomment:录制ID */
    XDS_MISC_SUPL_DATALOC  =   0x3,  /**<Supplemental Data Location*//**<CNcomment:其他数据的位置 */
    XDS_MISC_TIME_ZONE     =   0x4,  /**<Local Time Zone & DST Use*//**<CNcomment:加入夏令时的时间 */
    XDS_MISC_OUTOFBAND     =   0x40, /**<Out-of-Band Channel Number*//**<CNcomment:其他频道的频道号 */
    XDS_MISC_CHMAP_POINTER =   0x41, /**<Channel Map Pointer*//**<CNcomment:频道映射包的频道索引 */
    XDS_MISC_CHMAP_HEADER  =   0x42, /**<Channel Map Header Packet*//**<CNcomment:频道映射包头 */
    XDS_MISC_CHMAP_PAKT    =   0x43, /**<Channel Map Packet*//**<CNcomment:频道映射包 */
    XDS_MISC_TYPE_BUTT
} XDS_MISC_TYPE;

/**Type of Public Service Class*//** CNcomment:公共服务类的类别 */
typedef enum hiUNF_CC_XDS_PUB_TYPE
{
    HI_UNF_CC_XDS_PUB_NWS_CODE      =   0x1,  /*National Weather Service Code (WRSAME)*//**<CNcomment:国内气象信息码 */
    HI_UNF_CC_XDS_PUB_NWS_MSG       =   0x2,  /*National Weather Service Message*//**<CNcomment:国内气象信息消息 */
    HI_UNF_CC_XDS_PUB_TYPE_BUTT
} HI_UNF_CC_XDS_PUB_TYPE;

typedef struct _tagXDS_ProgramIDNum_S
{
    HI_U8 u8Minute;
    HI_U8 u8Hour;
    HI_U8 u8Data;
    HI_U8 u8Month;
}XDS_ProgramIDNum_S;

typedef struct _tagXDS_LenTimeInShow_S
{
    HI_U8 u8LengthMin;
    HI_U8 u8LengthHour;
    HI_U8 u8ElapsedMin;
    HI_U8 u8ElapsedHour;
    HI_U8 u8ElapsedSec;
}XDS_LenTimeInShow_S;

typedef struct _tagXDSProgName_S
{
    HI_U8 au8ProgName[MAX_CC608_PROGNAME_LEN];
    HI_U8 u8ProgNameLen;
}XDSProgName_S;

typedef struct _tagXDSProgType_S
{
    HI_U8 au8ProgType[MAX_CC608_PROGTYPE_LEN];
    HI_U8 u8ProgTypeLen;
}XDSProgType_S;

typedef struct _tagXDSContAdv_S
{
    HI_U8 u8Char1;
    HI_U8 u8Char2;
}XDSContAdv_S;

typedef struct _tagXDSAudioService_S
{
    HI_U8 u8Main;
    HI_U8 u8SAP;
}XDSAudioService_S;

typedef struct _tagXDSCaptService_S
{
    HI_U8 au8CaptServices[MAX_CC608_CAPTSERV_LEN];
    HI_U8 u8CaptServicesLen;
}XDSCaptService_S;

typedef struct _tagXDSCopyAndRedist_S
{
    HI_U8 u8Byte1;
    HI_U8 u8Byte2;
}XDSCopyAndRedist_S;

typedef struct _tagXDSCompPacket1_S
{
    HI_U8 au8ProgramType[5];
    HI_U8 u8ContAdv;
    HI_U8 u8LengthMin;
    HI_U8 u8LengthHour;
    HI_U8 u8ElapsedMin;
    HI_U8 u8ElapsedHour;
    HI_U8 au8Title[22];
    HI_U8 u8TitleLen;
}XDSCompPacket1_S;

typedef struct _tagXDSCompPacket2_S
{
    XDS_ProgramIDNum_S stPRGStartTime;
    XDSAudioService_S stAudioServices;
    HI_U8 au8CaptServices[2];
    HI_U8 au8CallLetter[4];
    HI_U8 au8NativeChan[2];
    HI_U8 au8NetworkName[18];
    HI_U8 au8NetworkNameLen;
}XDSCompPacket2_S;

typedef struct _tagXDSProgDesc_S
{
    HI_U8 au8ProgDesc[MAX_CC608_PROGDESC_LEN];
    HI_U8 u8ProgDescLen;
}XDSProgDesc_S;

typedef struct _tagXDSNetworkName_S
{
    HI_U8 au8NetName[MAX_CC608_NETNAME_LEN];
    HI_U8 u8NetNameLen;
}XDSNetworkName_S;

typedef struct _tagXDSCallLetNatvCh_S
{
    HI_U8 au8CallLetter[4];
    HI_U8 au8NativeChan[2];
    HI_U8 u8NativeChanLen;
}XDSCallLetNatvCh_S;

typedef struct _tagXDSTapeDelay_S
{
    HI_U8 u8Minute;
    HI_U8 u8Hour;
}XDSTapeDelay_S;

typedef struct _tagXDSTranSignID_S
{
    HI_U8 u8TSID3to0;
    HI_U8 u8TSID7to4;
    HI_U8 u8TSID11to8;
    HI_U8 u8TSID15to12;
}XDSTranSignID_S;

typedef struct _tagXDSTimeOfDay_S
{
    HI_U8 u8Minute;
    HI_U8 u8Hour;
    HI_U8 u8Date;
    HI_U8 u8Month;
    HI_U8 u8Day;
    HI_U8 u8Year;
}XDSTimeOfDay_S;

typedef struct _tagXDSImpCaptID_S
{
    HI_U8 u8Minute;
    HI_U8 u8Hour;
    HI_U8 u8Date;
    HI_U8 u8Month;
    HI_U8 u8LengthMin;
    HI_U8 u8LengthHour;
}XDSImpCaptID_S;

typedef struct  _tagXDSSupplDataLoc_S
{
    HI_U8 au8SupplDataLoc[MAX_CC608_SUPPDATA_LEN];
    HI_U8 u8SupplDataLocLen;
}XDSSupplDataLoc_S;

typedef struct _tagXDSLocTimeZone_S
{
    HI_U8 u8LocTimeZoneHour;
}XDSLocTimeZone_S;

typedef struct _tagXDSOutOfBand_S
{
    HI_U8 u8ChanLow;
    HI_U8 u8ChanHigh;
}XDSOutOfBand_S;

typedef struct _tagXDSChMapPointer_S
{
    HI_U8 u8TuneChanLow;
    HI_U8 u8TuneChanHigh;
}XDSChMapPointer_S;

typedef struct _tagXDSChMapHeader_S
{
    HI_U8 u8ChanLow;
    HI_U8 u8ChanHigh;
    HI_U8 u8Version;
}XDSChMapHeader_S;

typedef struct _tagXDSChMapPacket_S
{
    HI_U8 u8UserChanLow;
    HI_U8 u8UserChanHigh;
    HI_U8 u8TuneChanLow;
    HI_U8 u8TuneChanHigh;
    HI_U8 au8ChID[MAX_CC608_CHID_LEN];
    HI_U8 u8ChIDLen;
}XDSChMapPacket_S;

typedef struct _tagXDSNatWeaServcCode_S
{
    HI_U8 au8NatWeaServcCode[MAX_CC608_NWSCODE_LEN];
    HI_U8 u8NatWeaServcCodeLen;
}XDSNatWeaServcCode_S;

typedef struct _tagXDSNatWeaServcMsg_S
{
    HI_U8 u8NatWeaServcMsg[MAX_CC608_NWSMSG_LEN];
    HI_U8 u8NatWeaServcMsgLen;
}XDSNatWeaServcMsg_S;

typedef HI_S32 (*XDSPacketDecoder_FN)(HI_U8 *pu8PacketData, HI_U8 u8PacketLen);

static XDSPacketDecoder_FN  pfnXDSPacketDecoder[XDS_CLASS_BUTT][XDS_MISC_TYPE_BUTT];
/*******************************************************************************
*                       Static Function Definition
*******************************************************************************/

static HI_S32 _CC608_XDS_DecodeProgIDNum(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    HI_U8 Minute,Hour,Date,Month;
    XDS_ProgramIDNum_S astProgramIDNum[1];

    if( len != 4 )
    {
        HI_ERR_CC("Invalide length:%d for Data/Time Packet!\n",len);
        return HI_FAILURE;
    }

    Minute = (buf[0] & 0x3F);
    Hour = (buf[1] & 0x1F);
    Date = (buf[2] & 0x1F);
    Month = (buf[3] & 0x0F);
    if( ( Minute > 59 ) || ( Hour > 23 ) || ( Date > 31 )||( Month > 12 ))
    {
        HI_ERR_CC("Invalide Time/Date Value:Minute=%d,Hour=%d,Date=%d,Month=%d\n",Minute,Hour,Date,Month);
        return HI_FAILURE;
    }

    astProgramIDNum->u8Minute =   buf[0];
    astProgramIDNum->u8Hour   =   buf[1];
    astProgramIDNum->u8Data   =   buf[2];
    astProgramIDNum->u8Month  =   buf[3];

    XDS_TRACE("Program Start Time: %.2d-%.2d %.2d:%.2d\n",Month,Date,Hour,Minute);
    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeLenAndTimeInShow(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    HI_U8 length[6];
    HI_U8 i;
    XDS_LenTimeInShow_S astLenTimeInShow[1];

    if( len >  6 )
    {
        HI_ERR_CC("Invalide length:%d for length/Elapse Packet!\n",len);
        return HI_FAILURE;
    }

    memset(length, 0, 6);
    for ( i = 0; i < len; i++)
    {
        length[i] = (buf[i] & 0x3F);
    }

    if( (length[0] > 59) || (length[2] > 59) || (length[4] > 59) )
    {
        HI_ERR_CC("Invalide Show length value:len(min)=%d,ET(min)=%d,ET(s)=%d\n",length[0],length[2],length[4]);
        return HI_FAILURE;
    }

    astLenTimeInShow->u8LengthMin     =   buf[0];
    astLenTimeInShow->u8LengthHour    =   buf[1];
    astLenTimeInShow->u8ElapsedMin    =   buf[2];
    astLenTimeInShow->u8ElapsedHour   =   buf[3];
    astLenTimeInShow->u8ElapsedSec    =   buf[4];

    XDS_TRACE("Program Length:%d:%d, Elapsed Time:%d:%d:%d\n",length[1],length[0],length[3],length[2],length[4]);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeProgName(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    HI_U8 i;
    XDSProgName_S astProgName[1];

    if((len < MIN_CC608_PROGNAME_LEN) || (len > MAX_CC608_PROGNAME_LEN))
    {
        HI_ERR_CC("XDS program title overflow!\n");
        return HI_FAILURE;
    }

    for ( i = 0; i < len; i++)
    {
        if( ((buf[i] < 0x20) && (buf[i] != 0)) || (buf[i] > 0x7f) )
        {
            HI_ERR_CC("Invalide Value for Program Title buf[%d]=%x\n",i, buf[i]);
            return HI_FAILURE;
        }
    }


    astProgName->u8ProgNameLen    =   len;
    memcpy(astProgName->au8ProgName,buf,len);

    XDS_TRACE("Program name:\n");
    for(i=0; i<len; i++)
    {
        XDS_TRACE("%c ",astProgName->au8ProgName[i]);
    }
    XDS_TRACE("\n");

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeProgType(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    HI_U8 i;
    XDSProgType_S astProgType[1];

    if( len > MAX_CC608_PROGTYPE_LEN )
    {
        HI_ERR_CC("Invalide length:%d for program type packet\n",len);
        return HI_FAILURE;
    }

    for ( i = 0; i < len; i++)
    {
        if( ((buf[i] < 0x20)&&(buf[i] != 0)) || (buf[i] > 0x7f) )
        {
            HI_ERR_CC("Invalide value for program type:buf[%d]=%x\n",i, buf[i]);
            return HI_FAILURE;
        }
    }


    astProgType->u8ProgTypeLen = len;
    memcpy(astProgType->au8ProgType,buf,len);

    XDS_TRACE("Program type:\n");
    for(i=0; i<len; i++)
    {
        XDS_TRACE("%c ",astProgType->au8ProgType[i]);
    }
    XDS_TRACE("\n");

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeContentAdvisory(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    //HI_U8 len  = u8PacketLen;
    XDSContAdv_S astContAdv[1];

    astContAdv->u8Char1   =   buf[0];
    astContAdv->u8Char2   =   buf[1];

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeAudioService(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    //HI_U8 len  = u8PacketLen;
    XDSAudioService_S astAudioService[1];


    astAudioService->u8Main   =   buf[0];
    astAudioService->u8SAP    =   buf[1];

    XDS_TRACE("Audio Service: 0x%x 0x%x\n",astAudioService->u8Main,astAudioService->u8SAP);
    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeCaptionService(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSCaptService_S astCaptService[1];

    if( len < MIN_CC608_CAPTSERV_LEN || len > MAX_CC608_CAPTSERV_LEN )
    {
        HI_ERR_CC("Invalide len = %d for caption service packet!\n",len);
        return HI_FAILURE;
    }

    astCaptService->u8CaptServicesLen   =   len;
    memcpy(astCaptService->au8CaptServices,buf,len);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeCopyAndRedist(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSCopyAndRedist_S astCopyAndRedist[1];

    if( len != 2 )
    {
        HI_ERR_CC("Invalide len = %d for CGMSA packet!\n",len);
        return HI_FAILURE;
    }

    astCopyAndRedist->u8Byte1 =   buf[0];
    astCopyAndRedist->u8Byte2 =   buf[1];

    XDS_TRACE("Copy and Redistribution: 0x%x 0x%x\n",buf[0],buf[1]);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeCompPacket1(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSCompPacket1_S astCompPacket1[1];

    if( len < 10 || len > 32 )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    memcpy(astCompPacket1->au8ProgramType,buf,5);
    astCompPacket1->u8ContAdv       =   buf[5];
    astCompPacket1->u8LengthMin     =   buf[6];
    astCompPacket1->u8LengthHour    =   buf[7];
    astCompPacket1->u8ElapsedMin    =   buf[8];
    astCompPacket1->u8ElapsedHour   =   buf[9];
    astCompPacket1->u8TitleLen      =   len - 10;
    memcpy(astCompPacket1->au8Title,&buf[10],len-10);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeCompPacket2(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSCompPacket2_S astCompPacket2[1];

    if( len < 14 || len > 32 )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astCompPacket2->stPRGStartTime.u8Minute   =   buf[0];
    astCompPacket2->stPRGStartTime.u8Hour     =   buf[1];
    astCompPacket2->stPRGStartTime.u8Data     =   buf[2];
    astCompPacket2->stPRGStartTime.u8Month    =   buf[3];
    astCompPacket2->stAudioServices.u8Main    =   buf[4];
    astCompPacket2->stAudioServices.u8SAP     =   buf[5];
    memcpy(astCompPacket2->au8CaptServices,&buf[6],2);
    memcpy(astCompPacket2->au8CallLetter,&buf[8],4);
    memcpy(astCompPacket2->au8NativeChan,&buf[12],2);
    astCompPacket2->au8NetworkNameLen        =   len - 14;
    memcpy(astCompPacket2->au8NetworkName,&buf[14],len-14);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeProgDescription(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSProgDesc_S astXDSProgDesc[1];

    if( len > MAX_CC608_PROGDESC_LEN )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astXDSProgDesc->u8ProgDescLen = len;
    memcpy(astXDSProgDesc->au8ProgDesc,buf,len);

    return 0;
}

static HI_S32 _CC608_XDS_DecodeNetworkName(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSNetworkName_S  astNetworkName[1];

    if(len < MIN_CC608_NETNAME_LEN)
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    if(len > MAX_CC608_NETNAME_LEN)
    {
        len = MAX_CC608_NETNAME_LEN;
    }

    astNetworkName->u8NetNameLen  =   len;
    memcpy(astNetworkName->au8NetName,buf,len);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeCallLetter(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSCallLetNatvCh_S astCallLetNatvCh[1];

    if(len > MAX_CC608_CALLLET_LEN)
    {
        HI_ERR_CC("Invalide len = %d for composite packet and reset to length=6\n",len);
        len = MAX_CC608_CALLLET_LEN;
    }

    if(len <= MIN_CC608_CALLLET_LEN)
    {
        XDS_TRACE("Call Letter: %c %c %c %c\n",buf[0],buf[1],buf[2],buf[3]);
    }
    else
    {
        XDS_TRACE("Call Letter: %c %c %c %c %c %c\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
    }

    memcpy(astCallLetNatvCh->au8CallLetter,buf, (len > 4)? 4: len);

    if(len < MIN_CC608_CALLLET_LEN)
    {
        len = MIN_CC608_CALLLET_LEN;
    }
    astCallLetNatvCh->u8NativeChanLen =   len - MIN_CC608_CALLLET_LEN;
    memcpy(astCallLetNatvCh->au8NativeChan,&buf[MIN_CC608_CALLLET_LEN],len - MIN_CC608_CALLLET_LEN);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeTapeDelay(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSTapeDelay_S astTapeDelay[1];

    if( len != 2 )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astTapeDelay->u8Minute    =   buf[0];
    astTapeDelay->u8Hour      =   buf[1];

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeTSID(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSTranSignID_S astTransSignID[1];

    if( len != 4 )
    {
        HI_ERR_CC("Invalide len = %d for TSID packet!\n",len);
        return HI_FAILURE;
    }

    astTransSignID->u8TSID3to0    =   buf[0];
    astTransSignID->u8TSID7to4    =   buf[1];
    astTransSignID->u8TSID11to8   =   buf[2];
    astTransSignID->u8TSID11to8   =   buf[3];

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeTimeOfDay(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    HI_U8 Minute,Hour,Date,Month,Week,Year;
    XDSTimeOfDay_S astTimeOfDay[1];

    if( len != 6 )
    {
        HI_ERR_CC("Invalide length:%d for Data/Time Packet!\n",len);
        return HI_FAILURE;
    }

    Minute = (buf[0] & 0x3F);
    Hour = (buf[1] & 0x1F);
    Date = (buf[2] & 0x1F);
    Month = (buf[3] & 0x0F);
    Week = (buf[4] & 0x07 );
    Year = (buf[5] & 0x3f );
    if( ( Minute > 59 ) || ( Hour > 23 ) || ( Date > 31 )||( Month > 12 ))
    {
        HI_ERR_CC("Invalide Time/Date Value:Minute=%d,Hour=%d,Date=%d,Month=%d\n",Minute,Hour,Date,Month);
        return HI_FAILURE;
    }

    astTimeOfDay->u8Minute    =   Minute;
    astTimeOfDay->u8Hour      =   Hour;
    astTimeOfDay->u8Date      =   Date;
    astTimeOfDay->u8Month     =   Month;
    astTimeOfDay->u8Day       =   Week;
    astTimeOfDay->u8Year      =   Year;

    XDS_TRACE("Time of Day: %.2d-%.2d %.2d:%.2d, Week:%d,Year:%d\n",Month,Date,Hour,Minute,Week,Year+1990);
    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeImpCaptureID(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSImpCaptID_S astImpCaptID[1];

    if( len != 6 )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astImpCaptID->u8Minute    =   buf[0];
    astImpCaptID->u8Hour      =   buf[1];
    astImpCaptID->u8Date      =   buf[2];
    astImpCaptID->u8Month     =   buf[3];
    astImpCaptID->u8LengthMin =   buf[4];
    astImpCaptID->u8LengthHour=   buf[5];

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeSuplDataLoc(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSSupplDataLoc_S astSupplDataLoc[1];

    if( (len < MIN_CC608_SUPPDATA_LEN) || (len > MAX_CC608_SUPPDATA_LEN) )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astSupplDataLoc->u8SupplDataLocLen    =   len;
    memcpy(astSupplDataLoc,buf,len);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeTimeZone(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSLocTimeZone_S astLocTimeZone[1];

    if( len != 2 )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    if( buf[1] != 0)
    {
        HI_WARN_CC("The second character of Local Time Zone & DST Use packet is not NULL!\n");
    }

    astLocTimeZone->u8LocTimeZoneHour =   buf[0];

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeOutOfBand(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSOutOfBand_S astOutOfBand[1];

    if( len != 2 )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astOutOfBand->u8ChanLow   =   buf[0];
    astOutOfBand->u8ChanHigh  =   buf[1];

    return 0;
}

static HI_S32 _CC608_XDS_DecodeChMapPointer(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSChMapPointer_S astChMapPointer[1];

    if( len != 2 )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astChMapPointer->u8TuneChanLow    =   buf[0];
    astChMapPointer->u8TuneChanHigh   =   buf[1];

    return 0;
}

static HI_S32 _CC608_XDS_DecodeChMapHeader(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSChMapHeader_S astChMapHeader[1];

    if( len != 4 )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astChMapHeader->u8ChanLow =   buf[0];
    astChMapHeader->u8ChanHigh=   buf[1];
    astChMapHeader->u8Version =   buf[2];

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeChMapPacket(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSChMapPacket_S astChMapPacket[1];

    if( (len < 2) || (len > 10))
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astChMapPacket->u8UserChanLow =   buf[0];
    astChMapPacket->u8UserChanHigh=   buf[1];

    if(len > 2)
    {
        astChMapPacket->u8TuneChanLow =   buf[2];
        astChMapPacket->u8TuneChanHigh=   buf[3];

        if(len > 4)
        {
            astChMapPacket->u8ChIDLen =   len - 4;
            memcpy(astChMapPacket->au8ChID,&buf[4],len-4);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeNWSCode(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSNatWeaServcCode_S astNatWeaServcCode[1];

    if( len > MAX_CC608_NWSCODE_LEN )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astNatWeaServcCode->u8NatWeaServcCodeLen  =   len;
    memcpy(astNatWeaServcCode->au8NatWeaServcCode,buf,len);

    return HI_SUCCESS;
}

static HI_S32 _CC608_XDS_DecodeNWSData(HI_U8 *pu8PacketData, HI_U8 u8PacketLen)
{
    HI_U8 *buf = pu8PacketData;
    HI_U8 len  = u8PacketLen;
    XDSNatWeaServcMsg_S astNatWeaServcMsg[1];

    if( len > MAX_CC608_NWSMSG_LEN )
    {
        HI_ERR_CC("Invalide len = %d for composite packet-1!\n",len);
        return HI_FAILURE;
    }

    astNatWeaServcMsg->u8NatWeaServcMsgLen  =   len;
    memcpy(astNatWeaServcMsg->u8NatWeaServcMsg,buf,len);

    return HI_SUCCESS;
}

HI_VOID CC608_XDS_Init()
{
    HI_S32 j = 0;

    memset(pfnXDSPacketDecoder,0,sizeof(pfnXDSPacketDecoder));

    // current class
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_PRG_ID]       =   _CC608_XDS_DecodeProgIDNum;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_TIMEINSHOW]   =   _CC608_XDS_DecodeLenAndTimeInShow;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_PRG_NAME]     =   _CC608_XDS_DecodeProgName;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_PRG_TYPE]     =   _CC608_XDS_DecodeProgType;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_CONT_ADVSR]   =   _CC608_XDS_DecodeContentAdvisory;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_AUD_SERVC]    =   _CC608_XDS_DecodeAudioService;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_CAP_SERVC]    =   _CC608_XDS_DecodeCaptionService;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_CPY_REDIST]   =   _CC608_XDS_DecodeCopyAndRedist;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_COMP_1]       =   _CC608_XDS_DecodeCompPacket1;
    pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_COMP_2]       =   _CC608_XDS_DecodeCompPacket2;
    for(j=0; j<8; j++)
    {
        pfnXDSPacketDecoder[XDS_CLASS_CUR][XDS_CUR_PRG_DESC1+j]  =  _CC608_XDS_DecodeProgDescription;
    }

    // future class
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_PRG_ID]       =   _CC608_XDS_DecodeProgIDNum;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_TIMEINSHOW]   =   _CC608_XDS_DecodeLenAndTimeInShow;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_PRG_NAME]     =   _CC608_XDS_DecodeProgName;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_PRG_TYPE]     =   _CC608_XDS_DecodeProgType;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_CONT_ADVSR]   =   _CC608_XDS_DecodeContentAdvisory;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_AUD_SERVC]    =   _CC608_XDS_DecodeAudioService;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_CAP_SERVC]    =   _CC608_XDS_DecodeCaptionService;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_CPY_REDIST]   =   _CC608_XDS_DecodeCopyAndRedist;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_COMP_1]       =   _CC608_XDS_DecodeCompPacket1;
    pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_COMP_2]       =   _CC608_XDS_DecodeCompPacket2;
    for(j=0; j<8; j++)
    {
        pfnXDSPacketDecoder[XDS_CLASS_FUT][XDS_CUR_PRG_DESC1+j]  =  _CC608_XDS_DecodeProgDescription;
    }


    // channel class
    pfnXDSPacketDecoder[XDS_CLASS_CHAN][XDS_CHAN_NET_NAME]    =   _CC608_XDS_DecodeNetworkName;
    pfnXDSPacketDecoder[XDS_CLASS_CHAN][XDS_CHAN_CALL_LETTER] =   _CC608_XDS_DecodeCallLetter;
    pfnXDSPacketDecoder[XDS_CLASS_CHAN][XDS_CHAN_TAPE_DELAY]  =   _CC608_XDS_DecodeTapeDelay;
    pfnXDSPacketDecoder[XDS_CLASS_CHAN][XDS_CHAN_TRNS_SIGNID] =   _CC608_XDS_DecodeTSID;

    // miscellaneous class
    pfnXDSPacketDecoder[XDS_CLASS_MISC][XDS_MISC_TIMEOFDAY]   =   _CC608_XDS_DecodeTimeOfDay;
    pfnXDSPacketDecoder[XDS_CLASS_MISC][XDS_MISC_IMPL_CAPTID] =   _CC608_XDS_DecodeImpCaptureID;
    pfnXDSPacketDecoder[XDS_CLASS_MISC][XDS_MISC_SUPL_DATALOC]=   _CC608_XDS_DecodeSuplDataLoc;
    pfnXDSPacketDecoder[XDS_CLASS_MISC][XDS_MISC_TIME_ZONE]   =   _CC608_XDS_DecodeTimeZone;
    pfnXDSPacketDecoder[XDS_CLASS_MISC][XDS_MISC_OUTOFBAND]   =   _CC608_XDS_DecodeOutOfBand;
    pfnXDSPacketDecoder[XDS_CLASS_MISC][XDS_MISC_CHMAP_POINTER]=  _CC608_XDS_DecodeChMapPointer;
    pfnXDSPacketDecoder[XDS_CLASS_MISC][XDS_MISC_CHMAP_HEADER]=   _CC608_XDS_DecodeChMapHeader;
    pfnXDSPacketDecoder[XDS_CLASS_MISC][XDS_MISC_CHMAP_HEADER]=   _CC608_XDS_DecodeChMapPacket;

    // public class
    pfnXDSPacketDecoder[XDS_CLASS_PUB][HI_UNF_CC_XDS_PUB_NWS_CODE]     =   _CC608_XDS_DecodeNWSCode;
    pfnXDSPacketDecoder[XDS_CLASS_PUB][HI_UNF_CC_XDS_PUB_NWS_MSG]      =   _CC608_XDS_DecodeNWSData;
}

HI_S32 CC608_XDS_Decode(HI_U8 u8XDSClass, HI_U8 u8XDSType, HI_U8 *pu8Data, HI_U8 u8DataLen)
{
    XDSPacketDecoder_FN pFnXDSPacketDecoder;

    XDS_TRACE("CC608_XDS_Decode, u8XDSClass=%d, u8XDSType=%d,u8DataLen=%d\n",u8XDSClass,u8XDSType,u8DataLen);

    if((u8XDSClass >= XDS_CLASS_BUTT) || (u8XDSType >= XDS_MISC_TYPE_BUTT))
    {
        HI_ERR_CC("Invalid XDS packet class %d or type %d.\n",u8XDSClass,u8XDSType);
        return HI_FAILURE;
    }

    pFnXDSPacketDecoder = pfnXDSPacketDecoder[u8XDSClass][u8XDSType];

    if(pFnXDSPacketDecoder == HI_NULL)
    {
        return HI_FAILURE;
    }

    if( pu8Data && u8DataLen )
    {
        (HI_VOID)pFnXDSPacketDecoder(pu8Data,u8DataLen);
    }

    return HI_SUCCESS;
}

