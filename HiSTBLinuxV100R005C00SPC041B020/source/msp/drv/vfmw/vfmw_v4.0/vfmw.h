
/***********************************************************************
*
* Copyright (c) 2006 HUAWEI - All Rights Reserved
*
* File: $vfmw.h$
* Date: $2006/11/30$
* Revision: $v1.0$
* Purpose: interface header file of VFMW
*
*
* Change History:
*
* Date             Author            Change
* ====             ======            ======
* 2006/11/30       z56361            Original
*
*
* Dependencies:
*
************************************************************************/

#ifndef __VDEC_FIRMWARE_H__
#define __VDEC_FIRMWARE_H__
#if defined(VFMW_EXTRA_TYPE_DEFINE)
#include "hi_type.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(VFMW_EXTRA_TYPE_DEFINE)
#define UINT64 HI_U64
#define SINT64 HI_S64
#define UINT32 HI_U32
#define SINT32 HI_S32
#define UINT16 HI_U16
#define SINT16 HI_S16
#define UINT8  HI_U8
#define SINT8  HI_S8
#define VOID   HI_VOID
#define UADDR  HI_U32
#define ULONG  HI_SIZE_T
typedef const void CONSTVOID;
typedef unsigned   USIGN;
#else
typedef unsigned long long UINT64;
typedef long long          SINT64;
typedef unsigned int       UINT32;
typedef int                SINT32;
typedef unsigned short     UINT16;
typedef short              SINT16;
typedef char               SINT8;
typedef unsigned char      UINT8;

#ifndef VOID
typedef  void               VOID;
#endif
typedef unsigned int       UADDR;
typedef const void         CONSTVOID;
typedef unsigned           USIGN;
typedef unsigned long       ULONG;

#endif

#ifndef NULL
#define NULL               0L
#endif

#ifndef VFMW_BVT_SUPPORT
#define COMMAND_LINE_EXTRA 1
#endif
/***********************************************************************
      constants
 ***********************************************************************/
/* chip type definition. */
/* STB */
#define  VFMW_CHIP_TYPE_Hi3720         0
#define  VFMW_CHIP_TYPE_Hi3716C        1
#define  VFMW_CHIP_TYPE_Hi3716H        2
#define  VFMW_CHIP_TYPE_Hi3716M        3
#define  VFMW_CHIP_TYPE_Hi3716L        4
#define  VFMW_CHIP_TYPE_Hi3716MV300    5
#define  VFMW_CHIP_TYPE_Hi3712V100     6
#define  VFMW_CHIP_TYPE_Hi3715V100     7
#define  VFMW_CHIP_TYPE_Hi3716CV200ES  8
#define  VFMW_CHIP_TYPE_Hi3716CV200    9
#define  VFMW_CHIP_TYPE_Hi3798MV100    10
#define  VFMW_CHIP_TYPE_Hi3798CV200    11
#define  VFMW_CHIP_TYPE_Hi3716MV410    12

/* BVT */
#define  VFMW_CHIP_TYPE_Hi3531         50
#define  VFMW_CHIP_TYPE_Hi3535         51
#define  VFMW_CHIP_TYPE_Hi3536         52

/* DPT */
#define  VFMW_CHIP_TYPE_HiS5V100       100
#define  VFMW_CHIP_TYPE_HiS5V200       101

#define OUTPUT_IN_DEC            1
#define OUTPUT_IN_DSP            0

/* VFMW API return value */
#define  VDEC_OK                 0
#define  VDEC_ERR               -1
#define  VDEC_ERR_WRONG_PARAM   -2
#define  VDEC_ERR_NO_MEM        -3
#define  VDEC_ERR_VDEC_BUSY     -4
#define  VDEC_ERR_CHAN_FULL     -5
#define  VDEC_ERR_CHAN_RUN      -6
#define  VDEC_ERR_CHAN_STOP     -7
#define  VDEC_ERR_UNSUPPORT     -8

/* formal definition, to be removed later */
#define  VF_OK                   1
#define  VF_NO_MEM              -1
#define  VF_ERR_PARAM           -2
#define  VF_TIME_OUT            -3
#define  VF_VDM_ERR             -4
#define  VF_FORCE_STOP          -5
#define  VF_ERR_SYS             -20
#define  VF_ERR_SPS             -21
#define  VF_ERR_PPS             -22
#define  VF_ERR_SLICE           -23

/* extream value */
#define MAX_USRDAT_SIZE         1024

/* DAR(Display Aspect Ratio) */
typedef enum
{
    DAR_UNKNOWN = 0,
    DAR_4_3,
    DAR_16_9,
    DAR_221_100,
    DAR_235_100,
    DAR_IMG_SIZE,
    DAR_END_RESERVED
} VDEC_DAR_E;

/* PLUS_FS_NUM = DecFsNum - MaxRefFsNum */
#ifdef HAI_KANG_LESS_FRAMES_ENABLE
#define PLUS_FS_NUM  1
#else
#ifdef VFMW_CFG_CAP_HD_SIMPLE
#define PLUS_FS_NUM  1
#else
#define PLUS_FS_NUM  2
#endif
#endif

#define MIN_USERDEC_FRAME_NUM 3
/*PTS*/
#define PTS_NO_USE (UINT64)(-1)

/***********************************************************************
      enums
 ***********************************************************************/
// standard type
typedef enum
{
    VFMW_START_RESERVED = 0,
    VFMW_H264 = 0,
    VFMW_VC1,
    VFMW_MPEG4,
    VFMW_MPEG2,
    VFMW_H263,
    VFMW_DIVX3,
    VFMW_AVS,
    VFMW_JPEG,
    VFMW_REAL8 = 8,
    VFMW_REAL9 = 9,
    VFMW_VP6   = 10,
    VFMW_VP6F,
    VFMW_VP6A,
    VFMW_VP8,
    VFMW_VP9,
    VFMW_SORENSON,
    VFMW_MVC,
    VFMW_HEVC,
    VFMW_RAW,
    VFMW_USER,           /*## vfmw simply provide frame path. for external decoder, eg. mjpeg ##*/
    VFMW_END_RESERVED
} VID_STD_E;

#ifndef VFMW_BVT_SUPPORT
#define STD_START_RESERVED VFMW_START_RESERVED
#define STD_H264           VFMW_H264
#define STD_VC1            VFMW_VC1
#define STD_MPEG4          VFMW_MPEG4
#define STD_MPEG2          VFMW_MPEG2
#define STD_H263           VFMW_H263
#define STD_DIVX3          VFMW_DIVX3
#define STD_AVS            VFMW_AVS
#define STD_JPEG           VFMW_JPEG
#define STD_REAL8          VFMW_REAL8
#define STD_REAL9          VFMW_REAL9
#define STD_VP6            VFMW_VP6
#define STD_VP6F           VFMW_VP6F
#define STD_VP6A           VFMW_VP6A
#define STD_VP8            VFMW_VP8
#define STD_SORENSON       VFMW_SORENSON
#define STD_MVC            VFMW_MVC
#define STD_HEVC           VFMW_HEVC
#define STD_VP9            VFMW_VP9
#define STD_RAW            VFMW_RAW
#define STD_USER           VFMW_USER
#define STD_END_RESERVED   VFMW_END_RESERVED
#endif

/* buffer flags bits masks omx buffer flag. */
#define VDEC_BUFFERFLAG_EOS		        (0x00000001)
#define VDEC_BUFFERFLAG_STARTTIME       (0x00000002)
#define VDEC_BUFFERFLAG_DECODEONLY	    (0x00000004)
#define VDEC_BUFFERFLAG_DATACORRUPT	    (0x00000008)
#define VDEC_BUFFERFLAG_ENDOFFRAME	    (0x00000010)
#define VDEC_BUFFERFLAG_SYNCFRAME	    (0x00000020)
#define VDEC_BUFFERFLAG_EXTRADATA	    (0x00000040)
#define VDEC_BUFFERFLAG_CODECCONFIG	    (0x00000080)

//omx extrension flag
#define VDEC_BUFFERFLAG_FIELD_TOP       (0x10000000)
#define VDEC_BUFFERFLAG_FIELD_BOTTOM    (0x20000000)
#define VDEC_BUFFERFLAG_FIELD_ALL       (0x40000000)

/* channel capacity level */
typedef enum hiCAP_LEVEL_E
{
    CAP_LEVEL_MPEG_QCIF = 0,
    CAP_LEVEL_MPEG_CIF,
    CAP_LEVEL_MPEG_D1,
    CAP_LEVEL_MPEG_720,
    CAP_LEVEL_MPEG_FHD,
    CAP_LEVEL_H264_QCIF,
    CAP_LEVEL_H264_CIF,
    CAP_LEVEL_H264_D1,
    CAP_LEVEL_H264_720,
    CAP_LEVEL_H264_FHD,
    CAP_LEVEL_H264_BYDHD,

    CAP_LEVEL_1280x800,
    CAP_LEVEL_800x1280,
    CAP_LEVEL_1488x1280,
    CAP_LEVEL_1280x1488,
    CAP_LEVEL_2160x1280,
    CAP_LEVEL_1280x2160,
    CAP_LEVEL_2160x2160,
    CAP_LEVEL_4096x2160,
    CAP_LEVEL_2160x4096,
    CAP_LEVEL_4096x4096,
    CAP_LEVEL_8192x4096,
    CAP_LEVEL_4096x8192,
    CAP_LEVEL_8192x8192,

    CAP_LEVEL_SINGLE_IFRAME_FHD,
    CAP_LEVEL_USER_DEFINE_WITH_OPTION,

    CAP_LEVEL_MVC_FHD,
    CAP_LEVEL_HEVC_QCIF,
    CAP_LEVEL_HEVC_CIF,
    CAP_LEVEL_HEVC_D1,
    CAP_LEVEL_HEVC_720,
    CAP_LEVEL_HEVC_FHD,
    CAP_LEVEL_HEVC_UHD,

    CAP_LEVEL_BUTT
} VDEC_CHAN_CAP_LEVEL_E;

/* VDEC control command id, different function have different CID. */
typedef enum hiVDEC_CID_E
{
    VDEC_CID_GET_GLOBAL_STATE,      /* 0. get global state */
    VDEC_CID_GET_CAPABILITY,        /* 1. get the capacity of the decoder */
    VDEC_CID_GET_GLOBAL_CFG,        /* 2. get the configured info of the decoder */
    VDEC_CID_CFG_DECODER,           /* 3. congfig the decoder */

    VDEC_CID_CREATE_CHAN,           /* 4. create channel */
    VDEC_CID_CREATE_CHAN_WITH_OPTION, /*5. create channel with options */
    VDEC_CID_DESTROY_CHAN,          /* 6. destroy channel */
    VDEC_CID_DESTROY_CHAN_WITH_OPTION, /*7.destroy a channel created with options */
    VDEC_CID_GET_CHAN_CFG,          /* 8. get the configuration of the decode channel */
    VDEC_CID_CFG_CHAN,              /* 9. config the decode channel */
    VDEC_CID_GET_CHAN_STATE,        /* 10. get the state of the decode channel */
    VDEC_CID_START_CHAN,            /* 11. start channel */
    VDEC_CID_STOP_CHAN,             /* 12. stop channel */
    VDEC_CID_RESET_CHAN,            /* 13. reset channel */
    VDEC_CID_SET_STREAM_INTF,       /* 14. set the stream access interface for the decode channel */
    VDEC_CID_GET_IMAGE_INTF,        /* 15. get the stream access interface for the decode channel */
    VDEC_CID_GET_STREAM_SIZE,        /* 16. get the stream size(in byte) held by vfmw */
    VDEC_CID_SET_FRAME_BUF_INTF,    /*17*/
    VDEC_CID_GET_CHAN_MEMSIZE,      /* 18. get the memory budget for the specified channel capacity level */
    VDEC_CID_GET_CHAN_DETAIL_MEMSIZE_WITH_OPTION,  /*19. get the detailed memory budget according to the options */
    VDEC_CID_GET_CHAN_MEMADDR,      /* 20. get chan mem addr */
    VDEC_CID_GET_CHAN_ID_BY_MEM,    /* 21. querry the channel number according the memroy physical address */
    VDEC_CID_RELEASE_STREAM,        /* 22. reset scd to release stream buffers*/
    VDEC_CID_RESET_CHAN_WITH_OPTION,/* 23. reset channel with options to keep some characters of the channel */
    VDEC_CID_CFG_EXTRA,             /* 24. set decoder's extra_ref & extra_disp */
    //VDEC_CID_GET_CHAN_FRMSTATE,
    VDEC_CID_SET_FRAME_INTF,		/* 25. bvt set get frame interface */
    VDEC_CID_GET_USRDEC_FRAME = 30, /* 30 for VFMW_USER channel, get a frame block from vfmw */
    VDEC_CID_PUT_USRDEC_FRAME,      /* 31 for VFMW_USER channel, push a frame(info) into vfmw */

    VDEC_CID_REGISTER_EXTRA_ACCELERATOR,    /* 32 register external accelerator(normally refer to VEDU) */
    //add by z00222166, 2012.11.20
    VDEC_CID_SET_DISCARDPICS_PARAM,  /*33 get discard pictures parameters*/

    VDEC_CID_FRAME_BTL,				/* 34 capture*/
    VDEC_CID_EXTRA_WAKEUP_THREAD,   /* 35 bvt set wake up vfmw thread */
    VDEC_CID_SET_DBG_OPTION = 50,   /* 50 set debug options */
    VDEC_CID_GET_DGB_OPTION,        /* 51 get debug options */
    VDEC_CID_SET_PTS_TO_SEEK,       /* 52 set pts to be seeked by vfmw */
    VDEC_CID_SET_TRICK_MODE,        /* 54 set fast forward or backword speed*/
    VDEC_CID_SET_CTRL_INFO ,        /* 55 set pvr fast forward or backword stream info and control info*/
    VDEC_CID_SET_FRAME_RATE,
    VDEC_CID_START_LOWDLAY_CALC,
    VDEC_CID_STOP_LOWDLAY_CALC,
    VDEC_CID_ALLOC_MEM_TO_CHANNEL,   /* l00273086 */
#ifndef Hi3716MV410
    VDEC_CID_SET_MORE_GAP_ENABLE = 60, /* 60. bvt set enable more gap */
#else

    VDEC_CID_SET_MORE_GAP_ENABLE,
#endif
    VDEC_CID_ALLOC_MEM_TO_DECODER,
} VDEC_CID_E;

/* decode mode */
typedef enum
{
    IPB_MODE = 0,
    IP_MODE,
    I_MODE,
    DISCARD_MODE,
    DISCARD_B_BF_P_MODE    /* discard B before get first P */
} DEC_MODE_E;

/* user data source */
typedef enum
{
    USD_INVALID = 0,
    USD_MP2SEQ,
    USD_MP2GOP,
    USD_MP2PIC,
    USD_MP4VSOS,
    USD_MP4VSO,
    USD_MP4VOL,
    USD_MP4GOP,
    USD_H264,
    USD_AVSSEQ,
    USD_AVSPIC
} VDEC_USD_TYPE_E;

/* event type */
typedef enum
{
    EVNT_DISP_EREA = 1,
    EVNT_IMG_SIZE_CHANGE,
    EVNT_FRMRATE_CHANGE,
    EVNT_SCAN_CHANGE,
    EVNT_NEW_IMAGE,
    EVNT_USRDAT,
    EVNT_ASPR_CHANGE,
    EVNT_OUTPUT_IMG_SIZE_CHANGE,  /* means a frame with different size is to be output */
    EVNT_FIND_IFRAME = 20,
    EVNT_CAPTURE_BTL_OVER,
    EVNT_STREAM_ERR = 100,
    EVNT_VDM_ERR,
    EVNT_UNSUPPORT,
    EVNT_SE_ERR,
    EVNT_OVER_REFTHR,
    EVNT_OVER_OUTTHR,
    EVNT_REF_NUM_OVER,
    EVNT_SIZE_OVER,
    EVNT_SLICE_NUM_OVER,
    EVNT_SPS_NUM_OVER,
    EVNT_PPS_NUM_OVER,
    EVNT_IFRAME_ERR,
    EVNT_MEET_NEWFRM,
    EVNT_DECSYNTAX_ERR,
    EVNT_RATIO_NOTZERO,
    EVNT_LAST_FRAME,
    EVNT_RESOLUTION_CHANGE,
    EVNT_STREAM_END_SYNTAX,
    EVNT_NEED_ARRANGE,//l00273086
    EVNT_UNSUPPORT_SPEC,//y00226912
    EVNT_FAKE_FRAME,
#ifdef Hi3716MV410
    EVNT_ARRANGE_FRAME_BUF,
    EVNT_POST_MODULE_OCCUPY_ONE_FRAME,
    EVNT_REFRESH_CONFIG_FRAME_BUF,
#endif
    EVNT_VIDSTD_ERROR,//l00225186
    EVNT_SCD_BUFFER_REMAP,
    EVNT_VDH_BUFFER_REMAP,
} VDEC_EVNT_TYPE_E;

typedef enum
{
    COLOR_FMT_400,       /*YUV400*/
    COLOR_FMT_420,       /*YUV420*/
    COLOR_FMT_422_2x1,   /*YUV 422 2x1*/
    COLOR_FMT_422_1x2,   /*YUV 422 1x2*/
    COLOR_FMT_444,       /*YUV 444*/
    COLOR_FMT_410,       /*YUV 410*/
    COLOR_FMT_411,       /*YUV 411*/
    COLOR_FMT_BUTT       /*other unsupported format*/
} COLOR_FMT_E;

/* unsupport specification */
typedef enum
{
    SPEC_BIT_DEPTH,    // unsupport bit depth
    SPEC_BUTT
} UNSUPPORT_SPEC_E;

/*adapter type*/
typedef enum
{
    ADAPTER_TYPE_VDEC = 0,
    ADAPTER_TYPE_OMXVDEC,
    TYPE_BUTT
} VDEC_ADAPTER_TYPE_E;

/*channel purpose*/
typedef enum
{
    PURPOSE_DECODE = 1,
    PURPOSE_FRAME_PATH_ONLY,
    PURPOSE_BUTT
} VDEC_CHAN_PURPOSE_E;

/*channel memory allocation type*/
typedef enum
{
    MODE_ALL_BY_SDK = 1,
    MODE_ALL_BY_MYSELF,
    MODE_PART_BY_SDK,
    MODE_BUTT
} VDEC_CHAN_MEM_ALLOC_MODE_E;
/***********************************************************************
      structures
 ***********************************************************************/
/* VDEC capacity */
typedef struct hiVDEC_CAP_S
{
    SINT32    s32MaxChanNum;
    SINT32    s32MaxBitRate;
    SINT32    s32MaxFrameWidth;
    SINT32    s32MaxFrameHeight;
    SINT32    s32MaxPixelPerSec;
    VID_STD_E SupportedStd[32];
} VDEC_CAP_S;

typedef union
{
    struct
    {
        SINT32 IsAdvProfile;
        SINT32 CodecVersion;
    } Vc1Ext;

    struct
    {
        SINT32 bReversed;   /**<if the image need to be reversed, set to 1, otherwise set to 0 */
    } Vp6Ext;
} STD_EXTENSION_U;

/* channel config info */
typedef struct hiCHAN_CFG_S
{
    SINT32           eVidStd;            /* video compressing standard */
    STD_EXTENSION_U  StdExt;             /* extended info, for VC1 indicate AP or not, and other version info */
    SINT32           s32ChanPriority;    /* channel priority */
    SINT32           s32ChanErrThr;      /* channel error torlerance threshold. 0: zero torlerance; 100: display no matter how many error occured */
    SINT32           s32ChanStrmOFThr;   /* stream overflow control threshold, must >= 0, 0 means do not enable overflow control */
    DEC_MODE_E       s32DecMode;         /* decode mode，0：IPB， 1：IP， 2：I */
    SINT32           s32DecOrderOutput;  /* 0: output by display order，1:output by decode order */
    SINT32           s32DnrTfEnable;
    SINT32           s32DnrDispOutEnable;
    SINT32           s32BtlDbdrEnable;
    SINT32           s32Btl1Dt2DEnable;
    SINT32           s32LowdlyEnable;    /* lowdly enable */
    SINT32           s32VcmpEn;          /* frame compress enable */
    SINT32           s32WmEn;            /* water marker enable */
    SINT32           s32VcmpWmStartLine; /* water marker start line number */
    SINT32           s32VcmpWmEndLine;   /* water marker end line number */
    SINT32           s32SupportAllP;     /* support stream of all p frames */
    SINT32           s32ModuleLowlyEnable;
    SINT32           s32ChanLowlyEnable;
    SINT32           s32IsOmxPath;       /* specify for omx path */
    SINT32           s32MaxRawPacketNum;
    SINT32           s32MaxRawPacketSize;
    SINT32           s32ExtraFrameStoreNum;
    SINT32           s32MaxWidth;
    SINT32           s32MaxHeight;/*if the stream to be decoded w*h > s32MaxWidth*s32MaxHeight vfmw will stop syntax stream*/
} VDEC_CHAN_CFG_S;

//add by z00222166, 2012.11.20
typedef struct hiDISPIC_PARAM_S
{
    SINT32   s32Mode;   /*discard picture mode, 0: discard zero pictures, 1: discard pictures until s32Mode = 0, 2: discard s32DisNums pictures*/
    SINT32   s32DisNums;
} VDEC_DISPIC_PARAM_S;

/* userdata desc. */
typedef struct
{
    UINT8   data[MAX_USRDAT_SIZE]; /* USRDAT data entity */
    UINT8   pic_coding_type;
    UINT8   top_field_first;
    UINT32  pic_num_count;
    UINT32  dnr_used_flag;         /* internal used only, ignore */
    VDEC_USD_TYPE_E  from;         /* USRDAT source */
    UINT32  seq_cnt;               /** to be removed later */
    UINT32  seq_img_cnt;
    SINT32  data_size;             /* USRDAT size, in byte */
    UINT64  PTS;                   /* pts of the frame containning the userdata */
    /* for CC, valid when IsRegistered=1 */              //y00226912
    SINT8    IsRegistered;
    UINT8    itu_t_t35_country_code;
    UINT8    itu_t_t35_country_code_extension_byte;
    UINT16   itu_t_t35_provider_code;
} VDEC_USRDAT_S, USRDAT;

typedef struct
{
    UINT8  bottom_Range_mapy_flag;
    UINT8  bottom_Range_mapy;
    UINT8  bottom_Range_mapuv_flag;
    UINT8  bottom_Range_mapuv;
    UINT8  pic_structure;       // 0: frame, 1: top, 2: bottom, 3: mbaff, 4: field pair
    UINT8  Range_mapy_flag;
    UINT8  Range_mapy;
    UINT8  Range_mapuv_flag;
    UINT8  Range_mapuv;
    UINT8  chroma_format_idc;   // 0: yuv400 , 1: yuv420
    UINT8  vc1_profile;
    SINT8  use_pic_qp_en;
    SINT8  s32VcmpEn;           /* frame compress enable */
    SINT8  s32WmEn;             /* water marker enable */
    VID_STD_E video_standard;

    SINT32 QP_Y;
    SINT32 QP_U;
    SINT32 QP_V;

    SINT32 Rangedfrm;
    SINT32 s32VcmpWmStartLine;  /* water marker start line number */
    SINT32 s32VcmpWmEndLine;    /* water marker end line number */
    SINT32 s32VcmpFrameWidth;   /* DNR output frame lines, = image_width up aligned to multiple of 16 */
    SINT32 s32VcmpFrameHeight;  /* DNR output frame lines, = image_height up aligned to multiple of 16 */
} IMAGE_DNR_S;

/* BEGIN: Added by y62639, 2010/8/19 */
typedef struct
{
    UINT8  bAvsPlus;
    SINT32 IsProgressiveSeq;
    SINT32 IsProgressiveFrm;
    SINT32 RealFrmRate;
    SINT32 MatrixCoef;
    UINT32 Rwzb;
} VDEC_OPTMALG_INFO_S;
/* END:   Added by y62639, 2010/8/19 */

typedef enum
{
    FRAME_PACKING_TYPE_NONE,             /* normal frame, not a 3D frame */
    FRAME_PACKING_TYPE_SIDE_BY_SIDE,     /* side by side */
    FRAME_PACKING_TYPE_TOP_BOTTOM,       /* top bottom */
    FRAME_PACKING_TYPE_TIME_INTERLACED,  /* time interlaced: one frame for left eye, the next frame for right eye */
    FRAME_PACKING_TYPE_BUTT
} FRAME_PACKING_TYPE_E;

typedef enum YUV_FORMAT_E
{
    //Semi-Planner
    SPYCbCr400 = 0,
    SPYCbCr411,
    SPYCbCr420,
    SPYCbCr422_1X2,
    SPYCbCr422_2X1,
    SPYCbCr444,

    //Planner
    PLNYCbCr400,
    PLNYCbCr411,
    PLNYCbCr420,
    PLNYCbCr422_1X2,
    PLNYCbCr422_2X1,
    PLNYCbCr444,
    PLNYCbCr410,
    YCbCrBUTT
} YUV_FORMAT_E;
typedef struct hiIMAGE_BTL_S
{
    UINT32 u32Is1D;
    SINT32 btl_imageid;
    UINT32 u32IsCompress;
    UINT32 u32HeadStride;  //YC head info stride when compress is used
    UINT32 u32HeadOffset;
    UADDR  u32YHeadAddr;
    UADDR  u32CHeadAddr;

    UINT32 u32CrStride;
    UINT32 u32CStride;
    UADDR  u32CrAddr;

    UINT32 u32DNROpen;
    UADDR  u32DNRInfoAddr;
    UINT32 u32DNRInfoStride;
    UINT32 u32Reversed;
    YUV_FORMAT_E YUVFormat;
} IMAGE_BTL_S;
/* decoded image description */
typedef struct
{
    UINT64      SrcPts;
    UINT64      PTS;
    UINT64      Usertag;
    UINT64      DispTime;
    UINT8      *luma_vir_addr;
    UINT8      *chrom_vir_addr;
    UINT8      *luma_tf_vir_addr;
    UINT8      *chrom_tf_vir_addr;
    UINT8      *luma_2d_vir_addr;
    UINT8      *chrom_2d_vir_addr;
    UINT8      *line_num_vir_addr;

    UINT32      u32AspectWidth;
    UINT32      u32AspectHeight;

    UINT32      DispEnableFlag;
    UINT32      DispFrameDistance;
    UINT32      DistanceBeforeFirstFrame;
    UINT32      GopNum;
    UINT32      u32RepeatCnt;

    UADDR       top_luma_phy_addr;
    UADDR       top_chrom_phy_addr;
    UADDR       btm_luma_phy_addr;
    UADDR       btm_chrom_phy_addr;

    UADDR       luma_phy_addr;
    UADDR       chrom_phy_addr;
    UADDR       luma_tf_phy_addr;
    UADDR       chrom_tf_phy_addr;
    UADDR       luma_2d_phy_addr;
    UADDR       chrom_2d_phy_addr;

    //HEVC 10bit
    UINT8      *luma_2bit_vir_addr;
    UADDR       luma_2bit_phy_addr;
    UINT8      *chrom_2bit_vir_addr;
    UADDR       chrom_2bit_phy_addr;
    UINT32      image_stride_2bit;

    UINT32      is_fld_save;  //0:frm, 1:fld
    UINT32      top_fld_type;
    UINT32      bottom_fld_type;

    UINT32      format;                           /* [1:0] frame_type: 00(I), 01(P), 10(B), 11(Reserved)*/
    /* [4:2] CSP: 000(YUV:4:2:0), 001(YUV:4:0:0), 010~111(Reserved)*/
    /* [7:5] Norm: 000(component), 001(PLA), 010(NTSC), 011(SECAM), 100(MAC), 101(Unspecified Video Format), 110~111(Reserved)*/
    /* [9:8] source_format: 00(progressive), 01(interlaced), 10(infered_progressive), 11(infered_interlaced)*/
    /* [11:10] field_valid_flag: 00(top_field invalid, bottom_field invalid), 01(top_field valid, bottom_field invalid),
                                            10(top_field invalid, bottom_field valid), 11(top_field valid, bottom_field valid)*/
    /* [13:12]top_field_first: 00(bottom field first), 01(top field first), 10(un-know), 11(Reserved)*/
    /* [16:14] aspect_ratio: 000(unspecified), 001(4:3), 010(16:9), 011(2.21:1),100(2.35:1),101(origin width and height), 111(Reserved)*/
    /* [31:17](Reserved)*/
    UINT32      image_width;
    UINT32      image_height;
    UINT32      disp_width;
    UINT32      disp_height;
    UINT32      disp_center_x;
    UINT32      disp_center_y;

    UINT32      frame_rate;     /* frame rate, in Q10 */
    UINT32      image_stride;
    UINT32      image_id;
    UINT32      error_level;
    UINT32      seq_cnt;
    UINT32      seq_img_cnt;
    VDEC_USRDAT_S     *p_usrdat[4];

    UINT32      chroma_idc;
    UINT32      bit_depth_luma;
    UINT32      bit_depth_chroma;
    UINT32      frame_num;
    SINT32      last_frame;
    SINT32      view_id;      //h264 mvc
    SINT32      image_id_1;
    UINT32      is_3D;
    UADDR       top_luma_phy_addr_1;
    UADDR       top_chrom_phy_addr_1;
    UADDR       btm_luma_phy_addr_1;
    UADDR       btm_chrom_phy_addr_1;
    UADDR       line_num_phy_addr;
    IMAGE_BTL_S BTLInfo_1;

    /* BEGIN: Added by y00226912, for hevc */
    UINT32      left_offset;
    UINT32      right_offset;
    UINT32      top_offset;
    UINT32      bottom_offset;
    /* END: Added by y00226912, for hevc */

#ifdef VFMW_BVT_SUPPORT
    UINT32      luma_sum_h;
    UINT32      luma_sum_l;
    UINT32      luma_historgam[32];
    SINT32      compress_luma_offset;
    SINT32      compress_chroma_offset;
#endif
    UINT32      is_SecureFrame;                	/*add for tvp support*/

    UINT32      is_1Dcompress;
    FRAME_PACKING_TYPE_E  eFramePackingType;
    IMAGE_DNR_S ImageDnr;
    IMAGE_BTL_S BTLInfo;
    /* BEGIN: Added by y62639, 2010/8/19 */
    VDEC_OPTMALG_INFO_S optm_inf;
    /* END:   Added by y62639, 2010/8/19 */
} IMAGE;

/* frame desc for VFMW_USER */
typedef struct
{
    COLOR_FMT_E         enFmt;           /* color format */

    SINT32              s32IsFrameValid; /* 1: frame valid, to be output to vo;  0: frame invalid, just free the frame-store */

    UINT64              Pts;
    SINT32              s32IsSemiPlanar;

    SINT32              s32YWidth;
    SINT32              s32YHeight;

    UADDR               s32LumaPhyAddr;
    SINT32              s32LumaStride;

    UADDR               s32CbPhyAddr;    /* if semi-planar stand for the interlaced chrom addr; if planar stand for the Cb addr */
    UADDR               s32CrPhyAddr;    /* if semi-planar do not care，if planar stand for the Cr addr */
    SINT32              s32ChromStride;
    SINT32              s32ChromCrStride;
} USRDEC_FRAME_DESC_S;

/* global config */
typedef struct
{
    SINT32      reserved;
} VDEC_GLOBAL_CFG_S;

/* global state */
typedef struct
{
    SINT8   *vfmw_version_desc;
    UINT32  VFMW_version_number;
    SINT32  total_chan_num;
    SINT32  active_chan_num;
    SINT32  print_device;
    SINT32  print_enable;
} VDEC_GLOBAL_STATE_S;

/* channel state */
typedef struct
{
    UINT8   *mem_print_buf;

    UINT32  image_width;
    UINT32  image_height;
    UINT32  aspect_ratio;
    UINT32  scan_type;
    UINT32  video_format;
    UINT32  frame_rate;
    UINT32  bit_rate;
    UINT32  sub_video_standard;
    UINT32  profile;
    UINT32  level;
    SINT32  total_frame_num;
    SINT32  error_frame_num;
    SINT32  dec_error_frame_num;     /*the number of Frame which pErrRatio is not 0*/
    SINT32  decoded_1d_frame_num;   /* decoded, but not ready to display. generally refer to the decoded 1D frames */
    SINT32  buffered_stream_size;   /* the un-decoded stream seg produced by SCD */
    SINT32  buffered_stream_num;   /* the un-decoded stream seg num produced by SCD */

    UADDR   chan_mem_address;
    SINT32  chan_mem_size;
    SINT32  total_fstore_num;
    SINT32  total_pmv_num;
    SINT32  voque_detail;

    UINT32  stream_not_enough;
    UINT32  wait_disp_frame_num;
    UINT32  mpeg4_shorthead;      /* mpeg4 short head info, 1: sh 2:not sh  0:not mpeg4 stream */
    UINT32  total_disp_frame_num; /* total display num ( plus extra_disp ) */
    UINT32  is_field_flg; /* 0:frame 1:field */
} VDEC_CHAN_STATE_S;
#if 1
typedef struct hiVDEC_CHAN_FRMSTATUSINFO_S
{
    UINT32  u32DecodedFrmNum;  /* decoded, but not ready to display.*/
    UINT32  u32StrmSize;       /* the un-decoded stream seg produced by SCD */
    UINT32  u32StrmInBps;      /*average Bps */
    UINT32  u32OutBufFrmNum;   /*decoded, and ready to display*/
} VDEC_CHAN_FRMSTATUSINFO_S;
#endif
/* image accessing interface */
typedef struct hiIMAGE_INTF_S
{
    SINT32  image_provider_inst_id;
    SINT32  (*read_image)(SINT32 InstID, IMAGE *pImage);
    SINT32  (*release_image)(SINT32 InstID, IMAGE *pImage);
} IMAGE_INTF_S;

typedef struct hiRAW_EXTENSION_S
{
    UINT8   IsSeekPending;
    UINT32  Flags;
    UINT32  BufLen;
    UINT32  CfgWidth;
    UINT32  CfgHeight;
} RAW_EXTENSION_S;

/* stream packet struct */
typedef struct hiSTREAM_PACKET_S
{
    UADDR       PhyAddr;
    UINT8      *VirAddr;
    SINT32      Length;
    UINT64      Pts;
    UINT32      Index;
    UINT64      UserTag;
    UINT64      DispTime;
    UINT32      discontinue_count;
    UINT8       is_not_last_packet_flag;  //add by z00222166, 2012.11.09
    UINT8       is_stream_end_flag;        //add by y00226912, 2012.12.10
    UINT8       is_extern_eop_flag;      // add by l00228308, 2015.07.02	
    UINT32      DispEnableFlag;
    UINT32      DispFrameDistance;
    UINT32      DistanceBeforeFirstFrame;
    UINT32      GopNum;
    RAW_EXTENSION_S RawExt;               // omx raw buffer extension

} STREAM_DATA_S;

typedef struct hiVFMW_CONTROLINFO_S
{
    UINT32  u32IDRFlag;               /*IDR frame Flag, 1 means IDR frame.*/
    UINT32  u32BFrmRefFlag;           /*whether B frame is refer frame, 1 means B frame is refer frame.*/
    UINT32  u32ContinuousFlag;        /*whether send frame is continusous. 1 means continusous*/
    UINT32  u32BackwardOptimizeFlag;  /*Backward optimize flag, 1 means optimize the backward fast play performance*/
    UINT32  u32DispOptimizeFlag;      /*Display optimize flag, 1 means optimize the VO display performance*/
} VFMW_CONTROLINFO_S;

/* external frame store description(for OMX) */
typedef struct
{
    UADDR       PhyAddr;
    UINT8      *VirAddr;
    SINT32      Length;
    UINT8      *PrivateVirAddr;
    UADDR       PrivatePhyAddr;    /* store private frame info ,such as for fake 4K output*/
    SINT32      PrivateLength;
} EXTERNAL_FRAME_STORE_S;

/* stream accessing interface */
typedef struct hiSTREAM_INTF_S
{
    SINT32  stream_provider_inst_id;
    SINT32  (*read_stream)(SINT32 InstID, STREAM_DATA_S *pPacket);
    SINT32  (*release_stream)(SINT32 InstID, STREAM_DATA_S *pPacket);
} STREAM_INTF_S;

/* FRAME INTERFACE */
typedef struct hiFRAME_INTF_S
{
    SINT32  stream_provider_inst_id;
    SINT32  (*get_frame)(SINT32 InstID, EXTERNAL_FRAME_STORE_S *pExternalFrame, SINT32 ExpectedSize);
    SINT32  (*release_frame)(SINT32 InstID, EXTERNAL_FRAME_STORE_S *pExternalFrame);
} FRAME_INTF_S;

/* memory budget for a decode channel */
typedef struct hiCHAN_MEM_BUDGET
{
    SINT32  MinMemSize;
    SINT32  NormalMemSize;
    SINT32  FluentMemSize;
} CHAN_MEM_BUDGET_S;

/* memroy description */
typedef struct hiMEM_DESC_S
{
    UADDR  PhyAddr;
    VOID   *VirAddr;
    SINT32  Length;
    UINT32  IsSecMem;
} MEM_DESC_S;

typedef struct hiFRAME_BUF_S
{
    UADDR  u32StartVirAddr;
    UADDR  u32StartPhyAddr;
    UINT32 u32Size;
    UINT32 bufNodeStatus;
    UINT32 frameNum;

} FRAME_BUF_S;

/* detailed channel memory desc. */
typedef struct
{
    MEM_DESC_S  ChanMemVdh;
    MEM_DESC_S  ChanMemScd;
    MEM_DESC_S  ChanMemCtx;
} VDEC_CHAN_MEM_DETAIL_S;

/*For dynamic frame store param l00273086*/
typedef struct hiVDEC_CHAN_FRAME_STORE_PARAM_S
{
    UADDR  PhyAddr;
    UADDR  VirAddr;
    UINT32 Length;
    UINT32 FrameNum;
    UINT32 NeedMMZ;
} VDEC_CHAN_FRAME_STORE_PARAM_S;

/* user defined channel option*/
typedef struct
{
    VDEC_ADAPTER_TYPE_E eAdapterType;          /* channel type vdec/omxvdec */
    VDEC_CHAN_PURPOSE_E Purpose;               /* channel purpose, indicate if this channel is used for decoding or frame path only*/
    VDEC_CHAN_MEM_ALLOC_MODE_E MemAllocMode;   /* who alloc memory for the channel */
    SINT32 s32MaxWidth;                        /* max resolution supported by the channel*/
    SINT32 s32MaxHeight;
    SINT32 s32MaxSliceNum;                     /* for H264,H265 max slice number */
    SINT32 s32MaxVpsNum;                       /* for H264,H265 max vps number */
    SINT32 s32MaxSpsNum;                       /* for H264,H265 max sps number */
    SINT32 s32MaxPpsNum;                       /* for H264,H265 max pps number */
    SINT32 s32MaxRefFrameNum;                  /* max reference frame num*/
    SINT32 s32SupportBFrame;                   /* if support B frame. 1: yes, 0: no */
    SINT32 s32SupportH264;                     /* if this channel support H.264/H.265 decoding. 2: both 1: 264 only 0: no */
    SINT32 s32ScdLowdlyEnable;                 /* if this channel support scd lowdly. 1: yes, 0: no */
    SINT32 s32ReRangeEn;                       /* when resolution change, if the framestore be re-partitioned according to the new resolution. */
    /* 1:yes. can decode smaller(but more ref) stream, but one or more frame may be discarded */
    /* 0:no.  no frame discarded, but the stream with more ref can not dec, even if the total memory is enough */
    SINT32 s32SCDBufSize;                      /* SCD buf size */
    SINT32 s32DisplayFrameNum;                 /* user defined display frame num */
    SINT32 s32SlotWidth;                       /*if Purpose==PURPOSE_FRAME_PATH_ONLY, frame store width*/
    SINT32 s32SlotHeight;                      /*if Purpose==PURPOSE_FRAME_PATH_ONLY, frame store height*/
    SINT32 s32BtlDbdrEnable;                   /*btl calculate DBDR info flag, 1:enable,0:disable*/
    SINT32 s32Btl1Dt2DEnable;                  /*btl do 1D to 2D flag, 1:enable,0:disable*/
    VDEC_CHAN_MEM_DETAIL_S MemDetail;
    UINT32 u32DynamicFrameStoreAllocEn; //l00273086
    SINT32 s32DelayTime;//l00273086
    SINT32 s32ExtraFrameStoreNum;//l00273086
    UINT32 u32SelfAdaptionDFS;//l00273086
    UINT32 u32CfgFrameNum;//l00273086
    UINT32 u32NeedMMZ;//l00273086
    UINT32 u32MaxMemUse;//l00273086
    SINT32 s32IsSecMode;                       /*add sec mode*/
} VDEC_CHAN_OPTION_S;

/* user defined channel reset option*/
typedef struct
{
    SINT32 s32KeepBS;                          /* keep bs in the scd buffer */
    SINT32 s32KeepSPSPPS;                      /* keep global info in ctx for seek reset, default 0 */
    SINT32 s32KeepFSP;                         /* keep fsp info for seek reset, default 0 */
} VDEC_CHAN_RESET_OPTION_S;

typedef struct
{
    SINT32  VdhDetailMem;
    SINT32  ScdDetailMem;
    SINT32  ChanCtxDetailMem;
} DETAIL_MEM_SIZE;

/* external specified operations(method) */
typedef struct
{
    VDEC_ADAPTER_TYPE_E eAdapterType;
    SINT32 (*VdecCallback)(SINT32 ChanID, SINT32 type, VOID *p_args);
    MEM_DESC_S (*mem_malloc)(UINT8 *MemName, UINT32 len, UINT32 align);
    VOID (*mem_free)( MEM_DESC_S mem);
} VDEC_OPERATION_S;

/* decode parameter, usally for the external accelerator(VEDU eg.) */
typedef struct hiDECPARAM_INF_S
{
    SINT32      ChanID;            /*chan ID*/
    VID_STD_E   VidStd;            /*video standard*/
    VOID        *pDecParam;        /*pointer to the dec param */
} DECPARAM_INF_S;

/* desc of the external accelerator */
typedef struct hiEXTRA_ACCELERATOR_S
{
    SINT32  (*GetDecParam)(DECPARAM_INF_S *pDecParamInfo);
    VOID    (*PostProcess)(SINT32 ChanID, SINT32 ErrorRatio, UINT32 u32LumaPixSum);      /*ErrorRatio: 0-100, 0: no error, 100: all wrong*/
    SINT32  (*AcceleratorCharacter)(DECPARAM_INF_S *pDecParamInfo);   /*get some info from the external accelerator.
                                                                        pDecParamInfo: decode parameter.
                                                                        'character' is a 32-bits data, may be composed with many information.
                                                                        nowadays only tell the vfmw if the accelerator can decode such dec-param or not*/
} EXTRA_ACCELERATOR_S;


/*外部帧存分配接口和结构*/
typedef struct FSP_MEM_DESC_S
{
    UADDR       PhyAddr;
    VOID       *VirAddr;
    SINT32      Length;
} FSP_MEM_DESC_S;

typedef struct FSP_FRAME_INFO_S
{
    FSP_MEM_DESC_S stFrameInfo;
    UINT32 u32NeedPmv;
    FSP_MEM_DESC_S stPmvInfo;
} FSP_FRAME_INFO_S;

typedef struct FSP_FRAME_INTF_S
{
    SINT32 (*IsFrameAvalible)(SINT32 s32InstID);
    SINT32 (*AllocFrame)(SINT32 s32InstID, FSP_FRAME_INFO_S *pFrameInfo, UINT32 uImage_width, UINT32 uImage_height );
    SINT32 (*ReleaseFrame)(SINT32 s32InstID, UADDR u32FsPhyAddr);
    SINT32 (*SetFrameRef)(SINT32 s32InstID, UADDR u32FsPhyAddr, UINT32 u32IsRef);
} FSP_FRAME_INTF_S;


/***********************************************************************
      functions
 ***********************************************************************/
VOID   VDEC_OpenModule(VOID);
VOID   VDEC_ExitModule(VOID);

VOID VDM_CloseHardware(SINT32 VdhId);
VOID DSP_CloseHardware(SINT32 VdhId);

SINT32 BPD_CloseHardware(VOID);
SINT32 VDEC_Init(SINT32 (*VdecCallback)(SINT32 ChanID, SINT32 eEventID, VOID *pArgs));
SINT32 VDEC_InitWithOperation(VDEC_OPERATION_S *pArgs);
SINT32 VDEC_Control(SINT32 ChanID, VDEC_CID_E eCmdID, VOID *pArgs);
SINT32 VDEC_Exit(VOID);
SINT32 VDEC_Suspend(VOID);
SINT32 VDEC_Resume(VOID);
SINT32 VCTRL_SetDbgOption (UINT32 opt, UINT8 *p_args);


#ifdef __cplusplus
}
#endif

#endif  // __VDEC_FIRMWARE_H__
