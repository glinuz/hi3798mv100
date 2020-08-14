#ifndef __HI_TTX_DEF_H__
#define __HI_TTX_DEF_H__

#ifndef WIN32
 #include "hi_type.h"
 #include "pthread.h"
 #include "hi_unf_ttx.h"

#else
typedef unsigned short HI_U16;
typedef unsigned char  HI_U8;
typedef unsigned int   HI_U32;

typedef short        HI_S16;
typedef char         HI_S8;
typedef int          HI_S32;
typedef void         HI_VOID;
typedef unsigned int HI_HANDLE;

#endif


#define TTX_VER_15

#ifdef TTX_VER_15

 #define TTX_PACKER_26
#endif

#define TTX_REQ_UNUSED (0xFFFFFFFF)          /* invalid Req flag */

#define TTX_MAX_SUBPAGENO (32)
#define TTX_LINE_SIZE (46)
#define TTX_LINE_MEM_SIZE (42)              /*the size we wanted*/
#define TTX_EN300706_PACKET_SIZE (45)

#define TTX_MAX_LINES (32)/*packet0-25 and M30*/
#define TTX_DEFAULT_IDENTIFIER (0x10)
#define TTX_MAX_MAGAZINE (8)                    /*the max magazine number*/
#define TTX_MAX_PAGE (100)                    /*the max page of magzine*/
#define TTX_SUBCODE2_MASK (0x07)
#define TTX_SUBCODE4_MASK (0x03)
#define TTX_PACKET26_NUM (16)
#define TTX_PACKET27_NUM (8)
#define TTX_PACKET28_NUM (5)

#define TTX_PACKET29_NUM (5)

#define TTX_MAG_TRANS_MODE_POS (13)                  /* Position of the contol byte/bit containig */
#define TTX_MAG_TRANS_MODE_BIT (0x02)              /* transmission mode. (serial or parallel) */
#define TTX_SERIAL_MODE_TRANS_BIT (1)
#define TTX_VPS_DATA_ID (0xc3)
#define TTX_DEFAULT_INITMAGAZINE (1)
#define TTX_DEFAULT_INITPAGE (0)
#define TTX_DEDAULT_INITSUBPAGE (0)
#define TTX_INV_DMXID (0xFFFF)
#define TTX_INV_PIDID (0xFFFF)

typedef  enum  tagTTX_MSG_ACTION_E
{
    TTX_MSG_ACTION_SHOWPAGE,
    TTX_MSG_ACTION_UPDATETIME,
    TTX_MSG_ACTION_FLASH,
    TTX_MSG_ACTION_INPUTNUMBER,
    TTX_MSG_ACTION_INVALIDREQUEST,
    TTX_MSG_ACTION_UPDATESUBPAGE,
    TTX_MSG_ACTION_SHOWPROMPT_SEARCH,
    TTX_MSG_ACTION_SHOWPROMPT_SEARCH_TIMEOUT,
    TTX_MSG_ACTION_OSDOUTPUT_ON,
    TTX_MSG_ACTION_OSDOUTPUT_OFF,
    TTX_MSG_ACTION_BUTT
} TTX_MSG_ACTION_E;

typedef enum tagTTX_OBJECTTYPE_E
{
    TTX_VBI,                                                                  /** VBI mod */
    TTX_OSD,                                                                  /**OSD mod*/
    TTX_OSD_VBI,                                                              /**OSD_VBI mod*/
    TTX_NONE
} TTX_OBJECTTYPE_E;

typedef  enum tagTTX_SEARCH_DIRECTION_E                                   /*search diraction*/
{
    TTX_NEXT_SUBPAGE = 1,                     /*search next subpage*/
    TTX_PREV_SUBPAGE,                       /*searchsprev subpage*/
    TTX_NEXT_PAGE,                          /*search next page*/
    TTX_NEXT2_PAGE,                         /*search next 2 page*/
    TTX_NEXT3_PAGE,                         /*search next 3 page*/
    TTX_NEXT4_PAGE,                         /**/
    TTX_NEXT5_PAGE,                         /**/
    TTX_PREV_PAGE,                          /*search prev page*/
    TTX_NEXT_MAGZINE,                       /*search next magazine*/
    TTX_PREV_MAGZINE,                       /*search prevmagzine*/
    TTX_AUTO_PLAY,                          /*auto play*/
    TTX_THIS_PAGE,
    TTX_INDEX_PAGE
} TTX_SEARCH_DIRECTION_E;

typedef enum tagTTX_PAGE_FILTER_STATUS_E
{
    TELETEXT_PAGE_PES_INIT = 0,
    TELETEXT_PAGE_PES_HEAD_RECIVED,
    TELETEXT_PAGE_PES_WHOLE_RECIVED,
    TELETEXT_PAGE_PES_COPY_SEG_OVER
} TTX_PAGE_FILTER_STATUS_E;

struct tagTTX_PAGE_S
{
    HI_U32 u32ValidLines;                                           /* packet index*/
    HI_U16 u16subcode;                                              /*subpage number*/
    HI_U8  u8szLines[TTX_MAX_LINES][TTX_LINE_MEM_SIZE];                 /*common packet memory*/
    HI_U32 u32PTS;                                                  /**PTS*/
    HI_U32 u32ExtenValidLineH; /*entend packet index H*/
    HI_U32 u32ExtenValidLineL; /*entend packet index L*/
    HI_U8  u8szExtenLines[TTX_PACKET26_NUM + TTX_PACKET27_NUM + TTX_PACKET28_NUM + TTX_PACKET29_NUM][TTX_LINE_MEM_SIZE]; /*entern packet memory*/
};
typedef struct tagTTX_PAGE_S   TTX_PAGE_S;
typedef struct tagTTX_PAGE_S*  TTX_PAGE_S_PTR;

typedef struct tagTTX_SEGMENT_S
{
    HI_U16  u16DataLength;                                    /* data length*/
    HI_U8  *pu8SegmentData;                                     /*pointer to data*/
    struct tagTTX_SEGMENT_S* pstNext;
} TTX_SEGMENT_S, *TTX_SEGMENT_S_PTR;

typedef struct tagTTX_INIT_FILTER_S
{
    HI_U8 u8MagNum;                              /**the init Magazine num*/
    HI_U8 u8PageNum;                            /**the int page num*/
} TTX_INIT_FILTER_S, *TTX_INIT_FILTER_S_PTR;

typedef struct tagTTX_SEGMENT_QUEUE_S
{
    const HI_U8*      pu8BaseAddr;                /*Init address,pointer to start position*/
    HI_U32            u32MaxLen;                  /*the max length of this Queue*/
    pthread_mutex_t   Queue_lock;
    HI_BOOL           bFull;
    TTX_SEGMENT_S_PTR pstSegmentRear;              /* on receiving, pointer to write data */
    TTX_SEGMENT_S_PTR pstSegmentFront;             /* on parsing, pointer to read data */
} TTX_SEGMENT_QUEUE_S, *TTX_SEGMENT_QUEUE_S_PTR;

typedef struct tagTTX_REQUEST_S
{
    HI_UNF_TTX_PAGE_ADDR_S stReqPgAddr;
    HI_S32 (*NotifyFunction)(const HI_VOID* hTTX,                                /**callback function,if request success ,call this function*/
                           TTX_SEGMENT_S_PTR pstSegment);
    HI_VOID* pvUserData;  /**<user data*/
} TTX_Filter_S, *TTX_Filter_S_PTR;

typedef struct tagTTX_HANDLE_FILTER_S
{
    HI_U32       u32OpenOff;           /* illegal offset if unused */
    HI_BOOL      bHeaderFound;       /* To check whether page header is found or not */
    TTX_Filter_S stFilter;
} TTX_HANDLE_FILTER_S, *TTX_HANDLE_FILTER_S_PTR;

typedef struct tagTTX_DMX_PARAM_S
{
    HI_VOID* hDataRecv;
    HI_VOID* hDataParse;
    HI_UNF_TTX_REQUEST_RAWDATA_S stRequest; /**<Request for ttx raw data*/
} TTX_PARAM_S;

typedef enum tagTTX_LEVEL_E
{
    TTX_LEVEL_FATAL   = 0,    /**<致命错误, 此类错误需要特别关注，一般出现此类错误代表系统出现了重大问题 */
    TTX_LEVEL_ERROR   = 1,    /**<一般错误, 一般出现此类错误代表系统出现了比较大的问题，不能再正常运行 */
    TTX_LEVEL_WARNING = 2,    /**<告警信息, 一般出现此类信息代表系统可能出现问题，但是还能继续运行 */
    TTX_LEVEL_INFO = 3,       /**<提示信息, 一般是为提醒用户而输出，在定位问题的时候可以打开，一般情况下建议关闭 */
    TTX_LEVEL_BUTT
} TTX_LEVEL_E;


/**//*dongfuhai add start*/
/*
 * object pointer
 */

typedef struct tagTTX_OBJECT_POINTER_S
{
	HI_U8 u8SubPageNum : 4;
	HI_U8 u8TripletNum : 4; /*indicate the triplet containing the pointer data in the packet*/
	HI_U8 u8ObjectType : 2;
	HI_U8 u8PointerTab : 2; /*0:pointer locate in packt 1  1: pointer locate in packt 2 ,2: packet3   3:packet4*/
	HI_U8 u8PointerPos : 1;/*location of the pointer data within the triplet*/
} TTX_OBJECT_POINTER_S;

typedef struct tagTTX_MOT_INFO_S
{
    HI_UNF_TTX_PAGE_ADDR_S        stGPOP;
    HI_UNF_TTX_PAGE_ADDR_S        stNPOP;
    HI_UNF_TTX_PAGE_ADDR_S        stGDRCS;
    HI_UNF_TTX_PAGE_ADDR_S        stNDRCS;
    TTX_OBJECT_POINTER_S                    stDefaultObject[4];
    HI_BOOL            bFallback;
    HI_BOOL            bSidePanelLeft;
    HI_BOOL            bSidePanelRight;
    HI_BOOL            bBlackBgSubstitute;
}TTX_MOT_INFO_S;

typedef struct tagTTX_P28_0_M29_0_INFO_S
{
    HI_U8 u8DefG0G2;
    HI_U8 u82ndG0;
    HI_U8 bBlackBgSubstitute : 1;
    HI_U8 bSidePanelLeft     : 1;
    HI_U8 bSidePanelRight    : 1;
    HI_U8 bSidePanelStatus   : 1;
    HI_U8 u8SidePanelCols;
    HI_U8 u8DefScreenColor;
    HI_U8 u8DefRowColor;
    HI_U8 u8ForeCLUT;
    HI_U8 u8BackCLUT;
    HI_U32 u32CLUT2_3[16];
}TTX_P28_0_M29_0_INFO_S;

typedef struct tagTTX_P28_4_M29_4_INFO_S
{
    HI_U8 u8DefG0G2;
    HI_U8 u82ndG0;
    HI_U8 bBlackBgSubstitute : 1;
    HI_U8 bSidePanelLeft     : 1;
    HI_U8 bSidePanelRight    : 1;
    HI_U8 bSidePanelStatus   : 1;
    HI_U8 u8SidePanelCols    : 4;
    HI_U8 u8DefScreenColor;
    HI_U8 u8DefRowColor;
    HI_U8 u8ForeCLUT;
    HI_U8 u8BackCLUT;
    HI_U32 u32CLUT0_1[16];
}TTX_P28_4_M29_4_INFO_S;


/*DCLUT information, define by packet X/28/1 or M/29/1, if X/28/1 and M/29/1
exist at same time ,the X/28/1 takes precedence over M/29/1*/
typedef struct tagTTX_DCLUT_S
{
    HI_U8   u8DCLUT4Global[4];
    HI_U8   u8DCLUT4Normal[4];
    HI_U8   u8DCLUT16Global[16];
    HI_U8   u8DCLUT16Normal[16];
}TTX_DCLUT_S;

typedef struct tagTTX_P28_1_M29_1_INFO_S
{
    HI_U8 u8G0Set;
    HI_U8 u8G1Set;
    TTX_DCLUT_S stDCLUT;
}TTX_P28_1_M29_1_INFO_S;

typedef struct tagTTX_M29_0_INFO_S
{
    HI_U8 u8DefG0G2;
    HI_U8 u82ndG0;
    HI_U8 bBlackBgSubstitute : 1;
    HI_U8 bSidePanelLeft     : 1;
    HI_U8 bSidePanelRight    : 1;
    HI_U8 bSidePanelStatus   : 1;
    HI_U8 u8SidePanelCols    : 4;
    HI_U8 u8DefScreenColor;
    HI_U8 u8DefRowColor;
    HI_U8 u8ForeCLUT;
    HI_U8 u8BackCLUT;
    HI_U32 u32CLUT2_3[16 * 12 / 8];
}TTX_M29_0_INFO_S;

typedef struct tagTTX_M29_1_INFO_S
{
    HI_U8 u8MagG0Set;
    HI_U8 u8MageG1Set;
    TTX_DCLUT_S stDCLUT;
}TTX_M29_1_INFO_S;


typedef struct tagTTX_M29_4_INFO_S
{
    HI_U8 u8DefG0G2;
    HI_U8 u82ndG0;
    HI_U8 bBlackBgSubstitute : 1;
    HI_U8 bSidePanelLeft     : 1;
    HI_U8 bSidePanelRight    : 1;
    HI_U8 bSidePanelStatus   : 1;
    HI_U8 u8SidePanelCols    : 4;
    HI_U8 u8DefScreenColor;
    HI_U8 u8DefRowColor;
    HI_U8 u8ForeCLUT;
    HI_U8 u8BackCLUT;
    HI_U32 u32CLUT0_1[16 * 12 / 8];
}TTX_M29_4_INFO_S;

#define LINK_FUNC_GPOP      (0)
#define LINK_FUNC_POP        (1)
#define LINK_FUNC_GDRCS    (2)
#define LINK_FUNC_DRCS      (3)

#define PAGE_FUNC_LOP                               (0)
#define PAGE_FUNC_DATA_BROADCAST       (1)
#define PAGE_FUNC_GPOP                            (2)
#define PAGE_FUNC_POP                              (3)
#define PAGE_FUNC_GDRCS                          (4)
#define PAGE_FUNC_DRCS                            (5)
#define PAGE_FUNC_MOT                              (6)
#define PAGE_FUNC_MIP                              (7)
#define PAGE_FUNC_BTT                               (8)
#define PAGE_FUNC_AIT                               (9)
#define PAGE_FUNC_MPT                              (10)
#define PAGE_FUNC_MPT_EX                        (11)
/*dongfuhai add end*/

#define TTX_TRACE( level, module, fmt, args... ) \
    if (1)                  \
    {                       \
        printf(fmt, ## args); \
    }                         

#endif
