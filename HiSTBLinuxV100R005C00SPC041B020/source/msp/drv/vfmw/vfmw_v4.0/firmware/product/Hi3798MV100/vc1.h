
/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
  文 件 名   : vc1_vfmw.h
  版 本 号   : 初稿
  作    者   : y62639
  生成日期   : 2009年9月21日
  最近修改   :
  功能描述   : vc1 firmware 头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年9月21日
    作    者   : y62639
    修改内容   : 创建文件

******************************************************************************/

#ifndef _VC1_HEADER_
#define _VC1_HEADER_

#include    "basedef.h"
#include    "public.h"
#include    "bitstream.h"
#include	"vdm_hal.h"
#include    "syn_cmn.h"
#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------*
* 宏定义                                       *
*----------------------------------------------*/
#define VC1_VFMW_FALSE 0
#define VC1_VFMW_TRUE 1
#define VC1_NOT_DEC   2

#define VC1_OUTPUT_IN_DEC 1
#define VC1_OUTPUT_IN_DSP 0

#define VC1_REG_SIZE        128  //寄存器信息长度，单位WORDs
#define VC1_PICDNMSG_SIZE   96   //Pic下行消息长度，单位WORD
#define VC1_SLCDNMSG_SIZE   64   //Slc下行消息长度，单位WORD




#define VC1_SPSOK 1
#define VC1_ENTOK 2
#define VC1_PPSOK 4
#define VC1_MALCOK 8
#define VC1_ALLSLCOK 16
#define VC1_EOSOK 32

#define VC1_PRF_BLK_HSIZE 8
#define VC1_PRF_BLK_VSIZE 8
#define VC1_PRF_MAX_SAMPLE_VALUE (255)

#define VC1_PRF_MAX_FETCH_HSIZE    (8 + 7)
#define VC1_PRF_MAX_FETCH_VSIZE    (8 + 7)
#define VC1_PRF_MAX_FETCH_SIZE     (VC1_PRF_MAX_FETCH_HSIZE * VC1_PRF_MAX_FETCH_VSIZE)

#define VC1_MAX_PIC_WIDTH 128 // unit: MB
#define VC1_MAX_PIC_HEIGHT 128 // unit: MB
#define VC1_MAX_PIC_MB (VC1_MAX_PIC_WIDTH * VC1_MAX_PIC_HEIGHT) // 16129, unit: MB

#define VC1_MB_PITCH 16   /* 亮度块的边长 */
#define VC1_SUBMB_PITCH 8    /* 色度块的边长 */

#define VC1_ALL_BLK_4x4 15

#define VC1VFMW_MAX_FRAME_BUF_NUM 5
#define VC1VFMW_MAX_MBNUM 8192

#define VC1VFMW_MSG_SLOT_STAR_ADDR 0
#define VC1VFMW_DNMSG_NUM 2
#define VC1VFMW_UPMSG_NUM 1
#define VC1VFMW_RPMSG_NUM 1
#define VC1VFMW_MSG_BUF_SIZE (((((VC1VFMW_DNMSG_NUM + VC1VFMW_UPMSG_NUM + VC1VFMW_RPMSG_NUM) * 4) + 1023) >> 10) << 10)        /* 32 行深度:32*1024  */

#define VC1VFMW_NEIGHBOR_BUF_SIZE       ((((VC1VFMW_MAX_PIC_WIDTH * 4) + 1023) >> 10) << 10)  /* 32 行深度:32*1024  */

//#define VC1VDH_REG_PHY_ADDR 0x10100000       // VDH 寄存器映射到ARM空间的地址

//#ifndef SMP
//#define FPGA_RAW_BUF_LEN  (2*1024*1024)  //(6*1024*1024) SMP
//#else
//#define FPGA_RAW_BUF_LEN  (6*1024*1024)  //(6*1024*1024) SMP
//#endif

#define  MAX_PACKET_NUM  300//512
typedef enum
{
    VC1_SCEOS = 0xa,
    VC1_SCSLC,
    VC1_SCFLD,
    VC1_SCFRM,
    VC1_SCENT,
    VC1_SCSEQ,
    VC1_SCSLCUD = 0x1b,
    VC1_SCFLDUD,
    VC1_SCFRMUD,
    VC1_SCENTUD,
    VC1_SCSEQUD,
    HISI_VC1AP_STREAM_END_CODE = 0xfe,
} VC1_STARTCODE_EN;

typedef enum
{
    WMV1 = 0,
    MP43,
    WMV2,
    MP42,
    MP4S,
    WMV3,
    WMVA,
    WMVUNSPT,
    WMVC1
} VC1CODEC_VERSION_EN;

typedef enum
{
    VC1_VFMW_SIMPLE = 0,
    VC1_VFMW_MAIN,
    VC1_VFMW_ADVANCED
} VC1PROFILE_EN;

#if 0
typedef enum
{
    VC1_CODEC_DECSEQ = 0,
    VC1_CODEC_DECPIC
} VC1CODEC_EN;
#endif
typedef enum
{
    VC1_EXPLICITFRAMEQ0 = 0,
    VC1_EXPLICITFRAMEQ1,
    VC1_3QPDZ0,
    VC1_3QPDZ1
} VC1QUANTTYPE_EN;

typedef enum
{
    VC1_VFMW_IPIC = 0,
    VC1_VFMW_PPIC,
    VC1_VFMW_BPIC,
    VC1_VFMW_SPRITE, //not used, just for being conform with ref code
    VC1_VFMW_BIPIC,
    VC1_VFMW_SKPIC
} VC1_PICTYPE_EN;

typedef enum
{
    VC1_VFMW_PROGRESSIVE = 0,
    VC1_VFMW_FRAMEINTER = 2,
    VC1_VFMW_FIELDINTER
} VC1_PICSTRUCTURE_EN;

//typedef enum
//{
//VC1_FRAME= 0,
//VC1_FIELD
//} VC1_PICSTRUCTURE;


typedef enum
{
    VC1_II = 0,
    VC1_IP,
    VC1_PI,
    VC1_PP,
    VC1_BB,
    VC1_BBI,
    VC1_BIB,
    VC1_BIBI
} VC1_FLDPICTYPE_EN;
#if 0
typedef enum
{
    VC1_MVRNG64 = 0,
    VC1_MVRNG128,
    VC1_MVRNG512,
    VC1_MVRNG1024
} VC1_MVTRANGE_EN;

typedef enum
{
    VC1_DMV_HNVN = 0,
    VC1_DMV_HYVN,
    VC1_DMV_HNVY,
    VC1_DMV_HYVY
} VC1_DMVRANGE_EN;
#endif

typedef enum
{
    VC1_MIXMV = 0,
    VC1_1MV,
    VC1_1MVHP,
    VC1_1MVHPB,
    VC1_INTCMP
} VC1_MVMODE_EN;

typedef enum
{
    VC1_ALLFOUREDGE = 0,
    VC1_DOUBLEEDGE,
    VC1_SINGLEEDGE,
    VC1_ALLMB
} VC1_MBDQPROFILE_EN;

typedef enum
{
    VC1_RAW = 0,
    VC1_NORM2,
    VC1_DIFF2,
    VC1_NORM6,
    VC1_DIFF6,
    VC1_ROWSKIP,
    VC1_COLSKIP,
    VC1_DECIDED
} VC1_BITPLANEMODE_EN;
#if 0
typedef enum
{
    VC1_MB_1MV,
    VC1_MB_4MV,
    VC1_MB_FIELD,
    VC1_MB_FIELD4MV,
    VC1_MB_INTRA
} VC1_MBMVMODE_EN;
#endif
typedef enum
{
    VC1_BOTHFLDINT = 0,
    VC1_TOPHFLDINT,
    VC1_BOTFLDINT
} VC1_INTCOMPFIELD_EN;
typedef enum
{
    VC1_VFMW_TOPFLD = 0,
    VC1_VFMW_BOTFLD
} VC1_FLD_TB;


/**
* Description:
* Per macroblock quantizer step size enumeration
*/
typedef enum
{
    VC1_QUANTMODEDEFAULT,       /** All macroblocks use PQUANT */
    VC1_QUANTMODEALLEDGES,	    /** Edge macroblocks use ALTPQUANT */
    VC1_QUANTMODELEFTTOP,		/** Left/Top     macroblocks use ALTPQUANT */
    VC1_QUANTMODETOPRIGHT,		/** Top/Right    macroblocks use ALTPQUANT */
    VC1_QUANTMODERIGHTBOTTOM,	/** Right/Bottom macroblocks use ALTPQUANT */
    VC1_QUANTMODEBOTTOMLEFT,	/** Bottom/Left  macroblocks use ALTPQUANT */
    VC1_QUANTMODELEFT,			/** Left         macroblocks use ALTPQUANT */
    VC1_QUANTMODETOP,			/** Top          macroblocks use ALTPQUANT */
    VC1_QUANTMODERIGHT,			/** Right        macroblocks use ALTPQUANT */
    VC1_QUANTMODEBOTTOM,		/** Bottom       macroblocks use ALTPQUANT */
    VC1_QUANTMODEMBDUAL,		/** PQUANT/ALTPQUANT selected on macroblock basis */
    VC1_QUANTMODEMBANY			/** Any QUANT selected on a macroblock basis */
} VC1_QUANTMODE;



typedef struct
{
    SINT32 Code;
    SINT32 Len;
} VC1_VFMW_VLC_S;

typedef struct
{
    SINT8 *pElement;
    SINT32 Invert;
    SINT32 Imode;
    //SINT32 Element[VC1VFMW_MAX_PIC_MB];
} VC1_BITPLANE_S;

typedef struct
{
    UINT8 PROFILE;
    UINT8 FRMRTQ;
    UINT8 BITRTQ;
    UINT8 LOOPFILTER;
    UINT8 MULTIRES;
    UINT8 B16BITTRANSFORM; //corresponding to m_b16bitXform, an unknow flag from refcode but protocol document
    UINT8 FASTUVMC; //it's m_bUVHpelBilinear
    UINT8 EXTENDED_MV;
    UINT8 DQUANT;
    UINT8 VSTRANSFORM; //it's m_bXformSwitch
    UINT8 OVERLAP;
    UINT8 SYNCMARKER;
    UINT8 RANGERED;
    UINT8 MAXBFRAMES;
    UINT8 QUANTIZER;
    UINT8 FINTERPFLAG;
} VC1_SMPSEQSYNTAX_S;

typedef struct
{
    UINT8  PROFILE;
    UINT8  LEVEL;
    UINT8  CHROMFORMAT;
    UINT8  FRMRTQ_POSTPROC;
    UINT8  BITRTQ_POSTPROC;
    UINT8  POSTPROCFLAG;
    UINT8  PULLDOWN;
    UINT8  INTERLACE;
    UINT8  TFCNTRFLAG;
    UINT8  FINTERPFLAG;
    UINT8  DISPLAY_EXT;
    UINT8  ASPECT_RATIO_FLAG;
    UINT8  ASPECT_RATIO;
    UINT8  ASPECT_HORIZ_SIZE;
    UINT8  ASPECT_VERT_SIZE;
    UINT8  FRAMERATE_FLAG;
    UINT8  FRAMERATEIND;
    UINT8  FRAMERATENR;
    UINT8  FRAMERATEDR;
    UINT8  COLOR_FORMAT_FLAG;
    UINT8  COLOR_PRIM;
    UINT8  TRANSFER_CHAR;
    UINT8  MATRIX_COEF;
    UINT8  HRD_PARAM_FLAG;
    UINT8  HRD_NUM_LEAKY_BUCKETS;
    UINT8  BIT_RATE_EXPONENT;
    UINT8  BUFFER_SIZE_EXPONENT;
    UINT16 HRD_RATE[32];
    UINT16 HRD_BUFFER[32];
    UINT16 MAX_CODED_WIDTH;
    UINT16 MAX_CODED_HEIGHT;
    UINT16 DISP_HORIZ_SIZE;
    UINT16 DISP_VERT_SIZE;
    UINT16 FRAMERATEXP;

    //derived
    SINT32 HdrLen;
} VC1_ADVSEQSYNTAX_S;

typedef struct
{
    UINT8  BROKEN_LINK;
    UINT8  CLOSED_ENTRY;
    UINT8  PANSCAN_FLAG;
    UINT8  REFDIST_FLAG;
    UINT8  LOOPFILTER;
    UINT8  FASTUVMC;
    UINT8  EXTENDED_MV;
    UINT8  DQUANT;
    UINT8  VSTRANSFORM;
    UINT8  OVERLAP;
    UINT8  QUANTIZER;
    UINT8  CODED_SIZE_FLAG;
    UINT8  EXTENDED_DMV;
    UINT8  RANGE_MAPY_FLAG;
    UINT8  RANGE_MAPY;
    UINT8  RANGE_MAPUV_FLAG;
    UINT8  RANGE_MAPUV;
    UINT8  HRD_FULLNESS[32];
    UINT16 CODED_WIDTH;
    UINT16 CODED_HEIGHT;

    //derived
    SINT32 HdrLen;
} VC1_ADVENTSYNTAX_S;

typedef struct
{
    UINT8  DQUANTFRM;
    UINT8  DQPROFILE;
    UINT8  DQBILEVEL;
    UINT8  UseAltQp;
    UINT8  PQDIFF;
    UINT8  ABSPQ;
    UINT8  DQSBEDGE;
    UINT8  DQDBEDGE;

    //derived
    UINT8  ALTPQUANT;
    VC1_QUANTMODE QuantMode; // 哪条边用ALTPQUANT, fk ms
} VC1_VOPDQUANT_S;

typedef struct
{
    UINT8  INTERPFRM;
    UINT8  FRMCNT;
    UINT8  RANGEREDFRM;
    UINT8  PTYPE;
    UINT8  BFRACTION;
    UINT8  BF;
    UINT8  PQINDEX;
    UINT8  HALFQP;
    UINT8  PQUANTIZER;
    UINT8  MVRANGE;
    UINT8  RESPIC;
    UINT8  MVMODE;
    UINT8  MVMODE2;
    UINT8  LUMSCALE;
    UINT8  LUMSHIFT;
    UINT8  MVTAB;
    UINT8  CBPTAB;
    UINT8  TTMBF;
    UINT8  TTFRM;
    UINT8  TRANSACFRM;
    UINT8  TRANSACFRM2;
    UINT8  TRANSDCTAB;
    UINT8  RNDCTRL;// add by l57648
    VC1_VOPDQUANT_S VOPDQUANT;
    VC1_BITPLANE_S  MVTYPEMB;
    VC1_BITPLANE_S  DIRECTMB;
    VC1_BITPLANE_S  SKIPMB;

    //derived
    SINT32 HdrLen;
    SINT32 MVTYPEMBMode;
    SINT32 DIRECTMBMode;
    SINT32 SKIPMBMode;
    SINT32 PQUANT;
    SINT32 ScaleFactor;
} VC1_SMPPICSYNTAX_S;

typedef struct
{
    UINT8  RNDCTRL;
    UINT8  PTYPE;
    UINT8  BFRACTION;
    UINT8  HALFQP;
    UINT8  TFF;
    UINT8  RFF;
    UINT8  RPTFRM;
    UINT8  PS_PRESENT;
    UINT8  UVSAMP;
    UINT8  INTERPFRM;
    UINT8  PQINDEX;
    UINT8  PQUANTIZER;
    UINT8  POSTPROC;
    UINT8  MVRANGE;
    UINT8  MVMODE;
    UINT8  MVMODE2;
    UINT8  LUMSCALE;
    UINT8  LUMSHIFT;

    UINT8  MVTAB;
    UINT8  CBPTAB;
    UINT8  TTMBF;
    UINT8  TTFRM;

    UINT8  TRANSACFRM;
    UINT8  TRANSACFRM2;
    UINT8  TRANSDCTAB;
    UINT8  DMVRANGE;
    UINT8  QMVSWITCH;
    UINT8  MBMODETAB;
    UINT8  BMVBPTAB;
    UINT8  QMVBPTAB;
    UINT8  CONDOVER;
    UINT8  REFDIST;
    UINT8  NUMREF;
    UINT8  REFFIELD;
    UINT8  MVTYPEMBMode;
    UINT8  DIRECTMBMode;
    UINT8  SKIPMBMode;
    UINT8  ACPREDMode;
    UINT8  OVERFLAGSMode;
    UINT8  FIELDTXMode;
    UINT8  FORWARDMBMode;
    UINT8  CurParity; //0 is top, 1 is bot

    SINT32 FCM;
    SINT32 TFCNTR;
    SINT32 PS_HOFFSET[4];
    SINT32 PS_VOFFSET[4];
    SINT32 PS_WIDTH[4];
    SINT32 PS_HEIGHT[4];
    SINT32 PQUANT;
    //for interlaced frame
    SINT32         INTCOMP;

    //for interlaced field
    SINT32         FPTYPE;
    SINT32         INTCOMPFIELD;
    SINT32         LUMSCALE2;
    SINT32         LUMSHIFT2;

    //derived
    SINT32 HdrLen;

    SINT32 NumOfPanScanWindows;
    SINT32 ScaleFactor;
    SINT32 FldPicType[2];

    //for intra pic
#ifndef VFMW_BPD_H_SUPPORT
    VC1_BITPLANE_S ACPRED;
    VC1_BITPLANE_S OVERFLAGS;
    VC1_BITPLANE_S MVTYPEMB;
    VC1_BITPLANE_S DIRECTMB;
    VC1_BITPLANE_S SKIPMB;
    VC1_BITPLANE_S FIELDTX;
    VC1_BITPLANE_S FORWARDMB;
#endif
    //for inter pic
    VC1_VOPDQUANT_S VOPDQUANT;

} VC1_ADVPICSYNTAX_S;

typedef struct
{
    SINT8  PIC_HEADER_FLAG;
    UADDR  SLICE_ADDR;

    //derived
    SINT32 HdrLen;
} VC1_ADVSLCSYNTAX_S;

typedef struct
{
    SINT8              IsAdvProfile;
    UINT8              RcvVersion;
    UINT8              CodecVersion;
    VC1_SMPSEQSYNTAX_S SMPSeqSyntax;
    SINT32             NumOfPic;
    SINT32             FrmWidth;
    SINT32             FrmHeight;
} VC1_PREPARSEINFO_S;

typedef struct
{
    UINT8  is_to_ref;
    UINT8  is_to_display;
    UINT8  RANGEREDFRM;
    UINT8  IntensityConvertTab[2][2][256];  // [TopFld/BotFld][Y/C][256]
    UINT8  PicStruct;
    UINT8  PicCodingType;
    UINT8  IsHalfPel[2];
    SINT8  TopFldFirst;
    IMAGE  img;
    UINT32 pic_distance;
    //FSP_LOGIC_FS_S *FS;
} VC1_VFMWFRAMEOBJ_S;
#if 0
typedef struct
{
    //VFMW_MEMORY MemInf;

    /*display control*/
    SINT32 YuvOrder;

    /*decoding control*/
    SINT32 DecStartPic;
    SINT32 DecEndPic;

    /*scene recovery control*/
    SINT32 SREnable;
    SINT32 SRAuto;
    SINT32 SRStartPic;
    SINT32 SREndPic;
} VC1_VFMWCONFIG_S;
#endif
typedef struct
{
    SINT8  FldFlag[2];
    SINT32 ListLen;
    SINT32 ImgObj[2];
} VC1_REFLIST_OBJS;

typedef struct
{
    VC1_SMPSEQSYNTAX_S SMPSeqSyntax;
    VC1_ADVSEQSYNTAX_S AdvSeqSyntax;
    VC1_ADVENTSYNTAX_S AdvEntSyntax;
    VC1_SMPPICSYNTAX_S SMPPicSyntax;
    VC1_ADVPICSYNTAX_S AdvPicSyntax;
    VC1_ADVSLCSYNTAX_S AdvSlcSyntax[256]; //1080p just need these. max is 512 for VC1 original protocol  //20130730  维数128
} VC1_VFMWSYNTAX_S;
#if 0
typedef struct
{
    UADDR     PhyAddr;    // 码流的物理地址
    UINT8    *VirAddr;    // 码流的虚拟地址
    SINT32    Length;     // 码流长度（字节数）
    UADDR     PhyAddr2;    // 码流的物理地址
    UINT8    *VirAddr2;    // 码流的虚拟地址
    SINT32    Length2;     // 码流长度（字节数）
} SLICE_STREAM_PARAM;
#endif
typedef struct
{
    UINT8   *BsVirAddr[2];
    SINT8    IsLastSeg[2];
    UADDR    BsPhyAddr[2];
    SINT32   BsLength[2];
    SINT32   StreamID[2];

} VC1_STREAM_PACKET;

typedef struct
{
#ifndef VFMW_BPD_H_SUPPORT
    SINT8   ACPRED[VC1VFMW_MAX_PIC_MB];
    SINT8   OVERFLAGS[VC1VFMW_MAX_PIC_MB];
    SINT8   MVTYPEMB[VC1VFMW_MAX_PIC_MB];
    SINT8   DIRECTMB[VC1VFMW_MAX_PIC_MB];
    SINT8   SKIPMB[VC1VFMW_MAX_PIC_MB];
    SINT8   FIELDTX[VC1VFMW_MAX_PIC_MB];
    SINT8   FORWARDMB[VC1VFMW_MAX_PIC_MB];
#endif
    UINT8   CodecStatus;
    SINT8   RefId;
    UINT8   PicCodingType;
    SINT8   Last_Struct_IPB;  //no matter which type of last pic is, record it
    SINT8   PicStructure;
    SINT8   Last_Struct;// Last pic is I or P, record it
    SINT8   IsWaitToDec;
    //    SLICE_STREAM_PARAM  PackOfCurPic[256];
    //    SINT32              RcvPackNum;
    //    SINT32              ParsePckNum;
    UINT8   IsStreamEndFlag;
    UINT8   Is2ndFld;
    UINT8   LastIs2ndFld;
    SINT8   LastPicRNDCTRL;
    UINT8   CodecVersion;
    UINT8   PostCresent;
    UINT8   IntensityConvertTab[3][2][2][256];
    //  UINT8*  av_msg_vir_addr;
    //  UINT8*  va_msg_vir_addr;
    //  UINT8*  stream_base_vir_addr;
    //  UINT8*  sed_top_vir_addr;
    //  UINT8*  pmv_top_vir_addr;
    //  UINT8*  itrans_top_vir_addr;
    //  UINT8*  dblk_top_vir_addr;
    //  UINT8*  bitplane_info_vir_addr;
    //  UINT8*  intensity_conv_tab_vir_addr;
    //  UINT8*  pmv_colmb_vir_addr[2];
    UINT8  *BwdColmbVirAddr;
    UINT8  *CurrColmbVirAddr;
    //    UINT8*  slice_info_vir_addr[VC1VFMW_MAX_SLC_NUM];
    //  UINT8*  rpr_info_vir_addr;
    //  UINT8*  dnr_1d_vir_addr;
    //  UINT8*  dnr_2d_vir_addr;
    VC1_PREPARSEINFO_S  PreParseInfo;
    //    SLICE_STREAM_PARAM * pBsPack;
    //VC1_VFMWCONFIG_S *  pVfmwCfg;
    //VC1_VFMWCONFIG_S    VfmwCfg;
    BS                 *pBs;
    BS                  Bs;
    VC1_VFMWSYNTAX_S    SynNew;
    VC1_VFMWSYNTAX_S    SynNewPicInSlcHeader;
    VC1_SMPSEQSYNTAX_S  SMPSeqSyntax;
    VC1_ADVSEQSYNTAX_S  AdvSeqSyntax;
    VC1_ADVENTSYNTAX_S  AdvEntSyntax;
    VC1_SMPPICSYNTAX_S  SMPPicSyntax;
    VC1_ADVPICSYNTAX_S  AdvPicSyntax;
    VC1_ADVSLCSYNTAX_S  AdvSlcSyntax[256]; //1080p just need these. max is 512 for VC1 original protocol    //20130730  维数128
    VC1_VFMWFRAMEOBJ_S  FrameObj[VC1VFMW_MAX_FRAME_BUF_NUM];
    VC1_VFMWFRAMEOBJ_S  ZeroFrm;
    VC1_VFMWFRAMEOBJ_S *pCurFrm;
    VC1_VFMWFRAMEOBJ_S *pRef[3];    /*其中0为最近解出的参考图，1为较早解出的参考图*/
    SINT32              MbWidth;
    SINT32              MbHeight;
    SINT32              MbWidthLast;
    SINT32              MbHeightLast;
    UINT32              DispPicWidth;         //Frame width in pix
    UINT32              DispPicHeight;        //Frame height in in pix
    SINT32              DDR_Stride;
    SINT32              NumOfAllocatedFrameObj;
    VC1_REFLIST_OBJS    RefList0;
    VC1_REFLIST_OBJS    RefList1;
    UINT32              RefNum;
    SINT32              FwdFsID;
    SINT32              BwdFsID;
    SINT32              CurFsID;
    SINT32              ToQueFsID;
    //stream
    SINT32              SlcNum;
    VC1_SLCSTREAM_S     SlcStr[VC1VFMW_MAX_SLC_NUM];
    SINT32              PicCnt;

    VC1_STREAM_PACKET   stCurPacket;
    SINT32              FRFD, BRFD; //10.4.6.2 & 10.4.6.3 for B mv predict
    UINT32              PPicFound;
    UINT32              BPicCount;

    //add for multi res
    SINT32              CurPicWidth;
    SINT32              CurPicHeight;
    SINT32              CurPicMbWidth;
    SINT32              CurPicMbHeight;
    SINT32              CurPicMbNum;

    //    UINT32              av_msg_phy_addr;
    //    UINT32              va_msg_phy_addr;
    //    UINT32              stream_base_phy_addr;
    //    UINT32              sed_top_phy_addr;
    //    UINT32              pmv_top_phy_addr;
    //    UINT32              itrans_top_phy_addr;
    //    UINT32              dblk_top_phy_addr;
    //    UINT32              bitplane_info_phy_addr;
    //    UINT32              intensity_conv_tab_phy_addr ;
    //  UINT32              pmv_colmb_phy_addr[2];
    UADDR               BwdColmbAddr;
    UADDR               CurrColmbAddr;
    //    UINT32              slice_info_phy_addr[VC1VFMW_MAX_SLC_NUM];
    //  UINT32              rpr_info_phy_addr;
    //  UINT32              dnr_1d_phy_addr;
    //  UINT32              dnr_2d_phy_addr;
    SINT32              skpic_flag;
    UADDR               MVTYPEMB_ADDR;
    UADDR               SKIPMB_ADDR;
    UADDR               DIRECTMB_ADDR;
    UADDR               ACPRED_ADDR;
    UADDR               OVERFLAGS_ADDR;
    UADDR               FIELDTX_ADDR;
    UADDR               FORWARDMB_ADDR;

    UINT32              UsedDecMode;      //当前正在使用的解码模式(IPB, IP, I)

    UINT8               u8LastCodecStatus;//l00273086
} VC1_VFMWCODECINF_S;

typedef struct
{
    UINT8   *p_stream;
    UINT8    last_pack_in_nal;
    UADDR    strm_phy_addr;
    SINT32   stream_len;
    UINT64   pts;
} STREAM_PACKET;


typedef struct
{
    SINT32 tmp;
} VC1_DECPARA_S;


typedef union
{
    struct
    {
        USIGN forwardmb : 1;
        USIGN directmb  : 1;
        USIGN mbtypemb  : 1;
        USIGN fieldtx   : 1;
        USIGN skipmb    : 1;
        USIGN acpred    : 1;
        USIGN overflags : 1;
        USIGN reserved  : 1;
    } bits;
    UINT8 u8;
} VC1BITPLANE_MB;
#if 0
// VC1 Slice Info
typedef struct
{
    UINT32  D0;
    UINT32  D1;
    UINT32  D2;
    UINT32  D3;
    UINT32  D4;
    UINT32  D63;
} VC1_MSGSLOT_S;

typedef struct
{
    UINT8  CodecVersion;
    UINT8  PTYPE;
    UINT8  PicStruct;
    UINT8  NUMREF;
    UINT8  MVRANGE;
    UINT8  REFDIST;
    UINT8  OVERLAP;
    UINT8  LOOPFILTER;    // 0: dblk enable, 1: dblk disable
    UINT8  RNDCTRL;
    UINT8  HALFQP; 			// add z53517

    UINT8  *luma_vir_addr;
    UINT8  *chrom_vir_addr;
    UINT8  *luma_2d_vir_addr;
    UINT8  *chrom_2d_vir_addr;
    UINT8  *luma_fwd_vir_addr;
    UINT8  *chrom_fwd_vir_addr;
    UINT8  *luma_bwd_vir_addr;
    UINT8  *chrom_bwd_vir_addr;

    UINT8  MVMODE;
    UINT8  MVMODE2;
    UINT8  LUMSCALE;
    UINT8  LUMSHIFT;
    UINT8  RANGE_MAPY_FLAG;
    UINT8  RANGE_MAPY;
    UINT8  RANGE_MAPUV_FLAG;
    UINT8  RANGE_MAPUV;

    SINT32 MbWidth;
    SINT32 MbHeight;
    UINT32 DispPicWidth;         //Frame width in pix
    UINT32 DispPicHeight;        //Frame height in in pix
    UINT8  FASTUVMC;
    SINT32 FCM;

    SINT32 PQUANT;
    SINT32 bUniform;

    UADDR luma_phy_addr;
    UADDR  chrom_phy_addr;
    UADDR  luma_2d_phy_addr;
    UADDR  chrom_2d_phy_addr;
    UINT32 image_stride;
    UADDR  luma_fwd_phy_addr;
    UADDR  chrom_fwd_phy_addr;
    UADDR  luma_bwd_phy_addr;
    UADDR  chrom_bwd_phy_addr;
    SINT32 RefPicStruct[2];

    SINT32 CurIsHalfPel;
    SINT32 ColIsHalfPel;
    SINT32 ScaleFactor;
    SINT32 IsSecondFld;
    UINT8  CurParity;
    SINT32 FRFD, BRFD;

    //VDH_REG_EMAR_RPT_TO_S EmarRptTo;
    SINT32 VfmwPicCnt;
} VC1_PICBROADCAST_S;

typedef struct
{
    UINT32 bit_offset_0;
    UINT32 bit_len_0;
    UADDR  bit_stream_addr_0;
    UINT32 bit_offset_1;
    UINT32 bit_len_1;
    UADDR  bit_stream_addr_1;

    UINT32 SliceQP;
    UINT32 osvquant;
    UINT32 dblk_filter_passthrough;
    UINT32 first_mb_in_slice;
    UADDR  next_slice_para_addr;
} VC1_SLICEBROADCAST_S;

typedef struct
{
    UINT8  *vir_addr;
    UINT32  byte_pos;
    UINT32  byte_len;
    UINT32  start_offset;
    UINT32  end_offset;
    UINT32  SliceNum;
} VC1_VCTRLINFO_S;
typedef struct
{
    SINT8   XInt;       // 整像素点
    SINT8   YInt;
    SINT8   XFrac;      // 0: 整像素点,  1: 半像素点
    SINT8   YFrac;
    UINT8   PicStruct;
    /* 下列变量为预取参数 */
    SINT32  SubWidth;
    SINT32  SubHeight;

    SINT32  XPixWidth;  // 参考图像宽高 (色度亮度要分别计算)
    SINT32  YPixHeight;
    SINT32  PixScale;
    UADDR   StartAddr;  // 参考图像(帧或场)左上角像素位置
} VC1_PRCBLOCKINFO_S;


typedef struct
{
    UINT8   RNDCTRL;
    UINT8   MVMODE;
    UINT8   MVMODE2;
    UINT8   LUMSCALE;
    UINT8   LUMSHIFT;
    UINT8   PicCodingType;

    UINT8   FwBwFlg;
    UINT8   YcFlg;           //0: luma;  1: chroma
    SINT8   XFrac;
    SINT8   YFrac;
    UINT8   PrcBuf[15 * 15];
    UINT8   Prc2PrfBuf[8 * 26];

    /* 从pmv模块传下来的数据 */
    SINT32  LumaMvSub[4][2][2];      // [BlockIdx][Foreward/Backward][x/y]
    SINT32  ChromaMvSub[4][2][2];
    //SINT32  Prd[4][2];
    SINT32  Apc[4][2];
    //SINT32  ChrPrd[4][2];
    SINT32  ChrApc[4][2];
    SINT32  FldPrd;
    //SINT32  OppositeFld;
    SINT32 Profile;
    UINT8  CurParity;

    /* 从sed模块传下来的数据 */
    SINT32  Mbx;
    SINT32  Mby;
    SINT32  Mbn;

    /* 从vctrl广播下来的数据 */
    UINT32  DDRStride;
    UINT32  DDRHeight;
    UINT32  PicWidth;         //Frame width in pix
    UINT32  PicHeight;        //Frame height in in pix
    UINT32  DispPicWidth;         //Frame width in pix
    UINT32  DispPicHeight;        //Frame height in in pix

    UADDR   pic_addressx[3];  // pic_addressx[0]: 前向参考帧地址;   pic_addressx[1]: 后向参考帧地址
    // pic_addressx[3]: 当前参考帧地址
    SINT32  RefPicStruct[2];
    UINT8   PicStruct;
    SINT32  OppRefChroma[2];
    SINT32  IsSecondFld;

    /* 输出 */
    SINT32  BlockPartIdx;
} VC1_PRCINFO_S;
typedef enum
{
    VC1_PMV_MB_1MV,
    VC1_PMV_MB_4MV,
    VC1_PMV_MB_FIELD,
    VC1_PMV_MB_FIELD4MV,
    VC1_PMV_MB_INTRA
} VC1_PMV_MBMODE_EN;
#endif
#if 0
typedef enum
{
    VC1_PMV_FORWARD  = 0,
    VC1_PMV_BACKWARD,
    VC1_PMV_INTERPOLATE,
    VC1_PMV_DIRECT ,
} VC1_PMV_MBTYPE_EN;         // define MB type
typedef enum
{
    TL = 0,
    TR,
    BL,
    BR
} SOME;
#endif
#if 0
typedef struct
{
    SINT32 Prd[4][2]; // order [blk][fwd/back]
    SINT32 Apc[4][2]; // order [blk][fwd/back]
    SINT32 LumMv[4][2][2]; // order: [blk] [fwd/back] [x/y]
    SINT32 ChrPrd[4][2]; // order [blk][fwd/back]
    SINT32 ChrApc[4][2]; // order [blk][fwd/back]
    SINT32 ChrMv[4][2][2]; // order: [fwd/back] [x/y]
    SINT32 IsIntraL[4];
    SINT32 IsIntraC;
    SINT32 Is1Mv;
    SINT32 FldPrd;

    SINT32 MvForDblk[5][2][2];   // [block][fwd/bwd][x/y]  added by fuyang
} VC1_PMV_MBINFO_S;
#endif
/***************************************************************************************
y62639 add for mv calculation for B pic here (start)
 ****************************************************************************************/
#if 0
typedef struct
{
    SINT32 Prd[4];
    SINT32 Apc[4];
    SINT32 LumMv[4][2];
    SINT32 IsIntra[4];
    SINT32 Is1Mv;
} VC1_PMV_COLMB_INF;
typedef struct
{
    SINT32             CurPicType;
    SINT32             CurPicStruct;
    SINT32             CurParity;
    SINT32             Profile;
    SINT32             Mbx, Mby;
    SINT32             Mbn;
    SINT32             MbWidth, MbHeight;
    SINT32             ScaleFactor;
    SINT32             CurIsHalfPel;
    SINT32             ColIsHalfPel;
    SINT32             Is2ndFld;
    SINT32             DirectMvApc[2][2]; // order [top/bot][fwd/back]
    SINT32             Mv[2][2][2]; // order: [top/bot] [fwd/back] [x/y]
    VC1_PMV_COLMB_INF *pColMb;
} VC1_PMV_DIRECT_INF;
#endif
/***************************************************************************************
~y62639 add for mv calculation for B pic here (end)
 ****************************************************************************************/
#if 0
typedef struct
{
    UINT8  CodecVersion;
    SINT32 MbWidth;
    SINT32 MbHeight;
    UINT8  FASTUVMC;
    SINT32 MvRange[4]; //hor low, hor high, ver low, ver high
    SINT32 XMvRange, XMvFlag;
    SINT32 YMvRange, YMvFlag;
    UINT8  CurPicType;
    UINT8  CurPicStruct;
    UINT8  CurParity; //0 is top, 1 is bot
    UINT8  Is2ndFld;
    UINT8  REFDIST;
    SINT32 FRFD, BRFD;
    SINT32 SlcMbyBound;
    UINT8  REFFIELD;
    UINT8  NUMREF; //only for field picture 0: 1 ref field, 1: 2 ref fields
    SINT32 Mbx;
    SINT32 Mby;
    SINT32 Mbn;
    SINT32 CONDOVER;
    SINT32 OVERFLAGMB;
    SINT32 FIELDTX;
    SINT32 bIntraMB;
    SINT32 FieldFrameTransform;

    SINT32 MbAvaliable[5];//0:Left; 1:LeftTop; 2: Top; 3: TopRight; 5: CurrMbID
    SINT32 HYBRIDPRED[4];
    SINT32 Mvd[4][2][2];
    VC1_PMV_MBINFO_S CurMb; //needs be purged at each mb
    VC1_PMV_DIRECT_INF DirectInfo;
    SINT32           SKIPMBBIT;
    SINT32           MVMODEBIT;
    UINT8            MbMode;  //get info from sed MBMODE.MbType, indicate 1mv or 4 mv
    SINT32           DIRECTBBIT;
    SINT32           BMVTYPE;
    SINT32           MVSW;
    SINT32           ForwardBMb;
    VC1_PMV_MBMODE_EN           PvMbMode; // 1 mv, 4 mv
    VC1_PMV_MBTYPE_EN           PvMbType;  // forward, backward, etc
    SINT32           BlockCbp[6][4];
    SINT32           BlkTransformType[6];
    SINT32           TTMB;
    SINT32           CurIsHalfPel;
    SINT32           ColIsHalfPel;
    SINT32           iOffSet; // for scale field mv to frame mv
    SINT32           TempColMv[2]; //temp mv to save to col mv buffer, only used in main profile
    SINT32           OppRefChroma[2];  // 0: OppRef;  1: SameRef
    SINT32           SliceNum;
    SINT32           Profile;
    SINT32           ScaleFactor;
    SINT32           PicCnt;
} VC1_PMVINFO_S;
#endif

typedef struct
{
    UINT8  MVMODE;
    UINT8  MVMODE2;
    UINT8  FwBwFlg;
    UINT8  YcFlg; //0: luma;  1: chroma
    SINT8  XFrac;
    SINT8  YFrac;

    SINT32 Block8x8PartIdx;
    SINT32 MvX;
    SINT32 MvY;
    SINT32 InStride;
    SINT32 FldPrd;

    SINT32 SubWidth;
    SINT32 SubHeight;
    SINT32 iRndCtrl;

    SINT32 PrfBuf[VC1_PRF_MAX_FETCH_HSIZE *VC1_PRF_MAX_FETCH_VSIZE];
    SINT32 Prc2PrfBuf[8 * 26];
    SINT32 Prf2RcnBuf[2][VC1_PRF_BLK_HSIZE *VC1_PRF_BLK_VSIZE];
} VC1_PRFINFO_S;

typedef enum
{
    VC1_FALSE = 0,
    VC1_TRUE
} VC1_RETTYPE_E;

#if 0
typedef enum
{
    VC1_WMV1 = 0,
    VC1_MP43,
    VC1_WMV2,
    VC1_MP42,
    VC1_MP4S,
    VC1_WMV3,
    VC1_WMVA,
} VC1_STYL;


typedef enum
{
    VC1_MBTYPE_INTRA,         // 0
} VC1_MBTYPE_E;

enum
{
    VC1_MIXED_MV = 0,
    VC1_ALL_1MV,
    VC1_ALL_1MV_HALFPEL,
    VC1_ALL_1MV_HALFPEL_BILINEAR,
    VC1_INTENSITY_COMPENSATION
};



typedef enum
{
    VC1_LUMA_PRED = 0,
    VC1_CHROMA_PRED
} VC1_PREDTYPE_E;

typedef enum
{
    VC1_Y = 0,
    VC1_U,
    VC1_V
} VC1_YUV_E;
#endif
//y62639 add




//~y62639 add
#if 0
typedef enum
{
    //VC1_PREDNONE = 0,
    VC1_BLK8x8 = 0,
    VC1_BLK8x4,
    VC1_BLK4x8,
    VC1_BLK4x4
} VC1_BLKTYPE_E;

typedef struct
{
    SINT32 DcAcPred[6][2][8];  // 6 is blocknum,  2 is Horz and Vert, 8 is block size,
    SINT32 Quant;
    SINT32 Avail;
} VC1_BLOCK;

typedef enum
{
    VC1_PREDLEFT = 0,
    VC1_PREDTOP = 1,
    VC1_PREDNONE = 2
} VC1_PREDDIR_E;

typedef struct
{
    /* broadcast */

    SINT8  acpred_flag[6];
    UINT8  PicStruct;
    SINT8  EndOfSlice;
    UINT8  HALFQP;
    UINT8  MbType;   // I Mb, 1MV Mb, 4MV Mb, Skip MB: 0, 1, 2, 3
    //SINT32 is_short_header;
    //UINT32 pAddrQuantTab;
    //SINT32 alternate_vertical_scan;
    UINT32 PicWidthInMb;
    UINT32 PicHeightInMb;

    /* 从sed模块输入 */
    SINT32 Mbx;
    SINT32 Mby;
    SINT32 Mbn;
    SINT32 SliceNum;
    SINT32 FIELDTX;
    SINT32 bIntraMB;
    SINT32 FieldFrameTransform;

    SINT32 iDCStepSizeY;   //
    SINT32 iDCStepSizeC;   // 与iDCStepSizeY 完全相同
    SINT32 iDoubleStepSize;
    SINT32 iStepMinusStepIsEven;
    SINT32 bRotatedIdct;    //Rotate Flag
    SINT32 iOverlap;
    SINT32 b16bitXform;
    SINT32 PicType;
    SINT32 Profile;

    SINT32 PQUANT;
    SINT32 bUniform;

    VC1_BLKTYPE_E BlkType[6];

    SINT32 DefaultPredY;  //预测缺省值
    SINT32 DefaultPredC;  //预测缺省值, 与DefaultPredY 完全相同

    SINT32 bIntra[6];         //
    //SINT32 dct_type;         // 0: frame  1: field;
    SINT32 Cbp[6][4];         //
    SINT32 Index[6][64];
    SINT32 Level[6][64];
    SINT32 bFirstMbOfSlice;
    SINT32 quant;
    SINT32 quant_type;

    /* Itrans需要维护的数据 */

    //VC1_BLOCK CurrInfoLeft;
    //VC1_BLOCK CurrInfoUp;

    VC1_PREDDIR_E acpred_directions[6];
    SINT32  ScaledDCPred;

    //SINT32      dc_step[6];
    SINT32 DefaulPredVal;  // 缺省的预测值

    /* output到rcn模块 */
    SINT32 Residual[6][64];
} VC1_ITRANSINFO_S;
#endif
#if 0
typedef struct
{
    UINT8  PicStruct;
    UINT8  MbType;
    UINT8  pic_coding_type;
    UINT32 PicWidthInMb;    // only for debug
    UINT32 PicHeightInMb;  // only for debug

    /***** input from pmv  *****/
    UINT32 Mbn;
    UINT32 Mbx;    /* 以宏块为单位 */
    UINT32 Mby;    /* 以宏块为单位 */
    SINT32 FldPrd;
    SINT32 FieldFrameTransform;
    SINT32 Ratio0;
    SINT32 Ratio1;

    /* output to dblk */
    SINT32 Recon[6][64];      /* 宏块的重构像素 */

    UINT32 IsSkipMb;
    SINT32 IsIntraL[4];
    SINT32 IsIntraC;
    SINT32 field_prediction;          // 预测方式
    SINT32 Prd[4][2];          // [blockidx][foreward/backward]
    SINT32 ChrPrd[4][2];          // [blockidx][foreward/backward]

    /* 为rcn硬件优化预留, pv中不使用 */
    SINT32 dct_type;           // dct变换方式 0: frame/1: field;

    /***** input from itrans  *****/
    SINT32 Residual[6][64];

    /***** input from prf *****/
    UINT32 PredY[4][2][64];      // [blockidx][0: 前向预测/1: 后向预测]
    UINT32 PredC[4][2][32];      // UV 间插存放

} VC1_RCNINFO_S;

typedef struct
{
    UINT32 IsIntraLumaBlk[4];
    UINT32 IsIntraChromaBlk;
    UINT32 SliceNum;
    SINT32 OVERFLAGMB;

    SINT32 MbPixel[384];  // 16x16 + 8x8 + 8x8
} VC1_OVERLAPMBINFO_S;

typedef struct
{
    UINT32 Profile;
    UINT8  PicCodingType;
    SINT32 PQUANT;
    UINT8  OVERLAP;    // 0: overlap disable, 1: overlap enable
    SINT32 SliceStartMbFlag;
    SINT32 SliceEndMbFlag;
    UINT8  PicStruct;

    SINT32 CONDOVER;
    SINT32 OVERFLAGMB;

    // 当前宏块相关信息
    UINT32 Mbx;
    UINT32 Mby;
    UINT32 Mbn;
    UINT32 MbWidth;
    UINT32 MbHeight;

    UINT32 IsIntraLumaBlk[4];
    UINT32 IsIntraChromaBlk;
    UINT32 SliceNum;

    SINT32 ReconPixel[384];               // Rcn传来的MB像素，准备进行Overlap
    SINT32 Combined4Mb[256 * 4 + 64 * 4 + 64 * 4];
    SINT32 ForDblkCombined4Mb[256 * 4 + 64 * 4 + 64 * 4];
} VC1_OVERLAPINFO_S;

typedef struct
{
    UINT32 SliceNum;
    UINT32 BlkTransType[6];
    SINT32 FIELDTX;
    SINT32 bIntraMB;

    UINT32 IsIntraBlk4x4[24];
    UINT32 CbpBlk4x4[24];
    SINT32 Mvx[24][2];    //[blk4x4][Fwd/Bwd]
    SINT32 Mvy[24][2];    //[blk4x4][Fwd/Bwd]

    SINT32 Bs[48];  //先水平，后垂直；先亮度后色度；从上到下，从左到右

    SINT32 MbPixel[384];  // 16x16 + 8x8 + 8x8
} VC1_DBLKMBINFO_S;

typedef struct
{
    UINT8 *pY;
    UINT8 *pCb;
    UINT8 *pCr;

    UINT32 Profile;
    UINT8  PicCodingType;
    SINT32 PQUANT;
    UINT8  LOOPFILTER;    // 0: dblk disable, 1: dblk enable
    SINT32 SliceStartMbFlag;
    SINT32 SliceEndMbFlag;
    SINT32 IsFirstMbIntra;
    SINT32 SaveStride;
    UINT8  PicStruct;

    // 当前宏块相关信息
    UINT32 Mbx;
    UINT32 Mby;
    UINT32 Mbn;
    UINT32 MbWidth;
    UINT32 MbHeight;
    SINT32 IsCurrMb1Mv;
    UINT32 TTMB;

    VC1_DBLKMBINFO_S MbInfo;
    VC1_DBLKMBINFO_S ChromaMbInfo;

    SINT32 Combined4Mb[256 * 4 + 64 * 4 + 64 * 4];
    SINT32 TmpCombined4Mb[256 * 4 + 64 * 4 + 64 * 4];

    SINT32 CombinedChroma6Mb[64 * 6 + 64 * 6];
    SINT32 TmpCombinedChroma6Mb[64 * 6 + 64 * 6];
} VC1_DBLKINFO_S;

typedef struct
{
    UINT32 Mbx, Mby;
    UINT32 Mbn;
    UINT32 MbWidth, MbHeight;
    UINT32 FieldIndex;                 // 0: top_field, 1: btm_field
    UINT8  PicStruct;
    UINT8  picture_structure;

    UADDR   src_phy_addr;
    UINT8	 *p_src_vir_addr;
    UINT32  src_stride;
    UADDR   dst_phy_addr;
    UINT8	 *p_dst_vir_addr;
    UINT32  dst_stride;

    UINT32 Profile;
    UINT8  PicCodingType;
    UINT8  RANGE_MAPY_FLAG;
    UINT8  RANGE_MAPY;
    UINT8  RANGE_MAPUV_FLAG;
    UINT8  RANGE_MAPUV;
} VC1_DNRINFO_S;

typedef struct
{
    VC1_VFMWCODECINF_S  VfmwInf;
    VC1_PICBROADCAST_S *pPicBroadCast;

    VC1_VCTRLINFO_S      *pVctrlInfo;
    VC1_PMVINFO_S        *pPmvInfo;
    VC1_PRCINFO_S    *pPrcInfo;
    VC1_PRFINFO_S    *pPrfInfo;
    VC1_ITRANSINFO_S     *pItransInfo;
    VC1_RCNINFO_S        *pRcnInfo;
    VC1_OVERLAPINFO_S    *pOverlapInfo;
    VC1_DBLKINFO_S       *pDblkInfo;
    VC1_DNRINFO_S       *pDnrInfo;
} VC1_INFO_S;
#endif

typedef struct
{
    SYNTAX_EXTRA_DATA_S  *pstExtraData;
    VC1_DEC_PARAM_S      *pVc1DecParam;
    VC1_DEC_PARAM_S       Vc1DecParam;
    DEC_STREAM_PACKET_S   LastPacket;
    VC1_VFMWCODECINF_S    CodecInfo;
    BS                    Bs;
    IMAGE_VO_QUEUE        ImageQue;
    SINT32                ChanID;
} VC1_CTX_S;

#if 0
/*for VDH print ctrl */
typedef enum
{
    MDL_BASIC = 0,
    MDL_VCTRL,
    MDL_SED,
    MDL_ITRANS,
    MDL_PMV,
    MDL_PRC,
    MDL_PRF,
    MDL_RCN,
    MDL_DBLK
} PRINT_MODULE_TYPE;
typedef struct
{
    FILE   *fp_VDHLog;
    FILE   *fp_Golden;
    SINT32  PrintModule;          // 默认打印BASIC信息
    SINT32  PicStart;             // 默认从第0个图像开始打印
    SINT32  PicEnd;               // 默认打印到最后一个图像
    SINT32  MbStart;              // 默认从第0个宏块开始打印
    SINT32  MbEnd;                // 默认打印到最后一个宏块
    SINT32  PrintDev;             // 打印设备, 0,屏幕,1 文件
    SINT32  (*Fun_VDH_Print)(SINT32 module, const SINT8 *format, ...);
    VOID    (*Fun_VDH_Flush)(VOID);
} VDH_PRINT_CTRL;
#endif

SINT32        VC1DEC_Decode(VC1_CTX_S *pVC1_CTX_S, DEC_STREAM_PACKET_S *p_Packet);
SINT32        VC1_Vfmw_DecodeArbiter(VC1_CTX_S *pVC1_CTX_S);
SINT32        VC1_Vfmw_PrepareDecode(VC1_CTX_S *pVC1_CTX_S);
SINT32        VC1_Vfmw_ParseHeader(VC1_VFMWCODECINF_S *pCodecInf, VC1_VFMWSYNTAX_S *pSyntax, SINT32 ChanID);
SINT32        VC1_Vfmw_StartDecode(VC1_CTX_S *pVC1_CTX_S);
//VOID          VC1_Vfmw_ReleasePacket(VC1_VFMWCODECINF_S *pCodecInf);
SINT32        VC1_Vfmw_UpdataLastPicRNDCTRL(VC1_VFMWCODECINF_S *pCodecInf);
SINT32        VC1_Vfmw_DecPQUANT(SINT32 Quantizer, SINT32 PqIndex);
SINT32        Vc1SMPSeqHdr(UINT8 *pBitBuf, SINT32 ByteLen, VC1_PREPARSEINFO_S *pVc1Info);
SINT32        VC1_Vfmw_BuildRefList(VC1_VFMWCODECINF_S *pCodecInf, SINT32 PicType, SINT32 SndFld, SINT32 IsTop);
UINT32        VC1DEC_VDMPostProc(VC1_CTX_S *pCtx, SINT32 ErrRatio);
SINT32        VC1DEC_GetRemainImg(VC1_CTX_S *pCtx);
SINT32        VC1DEC_GetImageBuffer(VC1_CTX_S *pCtx);
SINT32        VC1DEC_Init(VC1_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
SINT32        VC1DEC_RecycleImage(VC1_CTX_S *pCtx, UINT32 img_id);
SINT32        VC1_GetPacketState(VC1_CTX_S *pCtx);
VOID          VC1_ClearCurPic(VC1_VFMWCODECINF_S *pCodeInfo, SINT32 ChanID);


#ifdef __cplusplus
}
#endif

#endif //_VC1_HEADER_
