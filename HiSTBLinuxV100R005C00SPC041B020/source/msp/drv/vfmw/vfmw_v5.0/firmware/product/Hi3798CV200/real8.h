#ifndef __REAL8_VFMW_H__
#define __REAL8_VFMW_H__


#include "basedef.h"
#include "public.h"

#include "bitstream.h"
#include "syn_cmn.h"
#include "vdm_hal.h"


#define REAL8_VFMW_MAX_PIC_WIDTH   127 //unit: MB
#define REAL8_VFMW_MAX_PIC_HEIGHT  127 //unit: MB
#define REAL8_VFMW_MAX_PIC_MB      (REAL8_VFMW_MAX_PIC_WIDTH*REAL8_VFMW_MAX_PIC_HEIGHT) //16129 //unit: MB  

#define REAL8_QUEUE_IMAGE_NUM                    6
#define REAL8_MINIMUM_IMAGE_NUM                  6
#define REAL8_MAXIMUM_IMAGE_NUM                  6

#define REAL8_MAX_SLOT_NUM                       (MAX_SLICE_SLOT_NUM)
#define REAL8_UP_MSG_SLOT_INDEX                  0
#define REAL8_RP_MSG_SLOT_INDEX                  1
#define REAL8_PIC_DN_MSG_SLOT_INDEX              2
#define REAL8_SLC_DN_MSG_SLOT_INDEX              3
#define REAL8_SLC_DN_MSG_SLOT_FULL_THRESHOLD     (REAL8_MAX_SLOT_NUM-4)

/*********************(64*4*X) 没部分空间都为64word整数倍****/
#define REAL8_ALIGN_LEN                          (64*4)
#define REAL8_SED_TOP_ADDR_LEN                   (64*4*REAL8_VFMW_MAX_PIC_WIDTH)
#define REAL8_PMV_TOP_ADDR_LEN                   (64*4*REAL8_VFMW_MAX_PIC_WIDTH)
#define REAL8_RCN_TOP_ADDR_LEN                   (64*4*REAL8_VFMW_MAX_PIC_WIDTH)
#define REAL8_DBLK_TOP_ADDR_LEN                  (64*4*REAL8_VFMW_MAX_PIC_WIDTH)
#define REAL8_PMV_ONECOLMB_ADDR_LEN              (20*4)              //计算得知，大致需要16words/mb，现在多分配一点             
#define REAL8_MSG_SLOT_BURST_LEN                 (64*4)              // 1 burst, 64 word    
#define REAL8_UP_MSG_SLOT_LEN                    REAL8_MSG_SLOT_BURST_LEN
#define REAL8_RP_MSG_SLOT_LEN                    REAL8_MSG_SLOT_BURST_LEN
#define REAL8_PIC_DN_MSG_SLOT_LEN                REAL8_MSG_SLOT_BURST_LEN
#define REAL8_ONE_SLC_DN_MSG_SLOT_LEN            REAL8_MSG_SLOT_BURST_LEN
#define REAL8_TOTAL_SLC_DN_MSG_SLOT_LEN          ( REAL8_MSG_SLOT_BURST_LEN * (REAL8_MAX_SLOT_NUM-3) )

#define RV8_PARC_SQUARE        1

#define RV8_TR_SHIFT           14
#define RV8_TR_RND             (1 << (RV8_TR_SHIFT - 1))

#define RV8_TR_WRAP            256
#define RV8_TR_WRAP_RV         8192

#define RV8_MAX_NUM_RPR_SIZES  8

/* Filter strength tables */
/* default strength specified by RV_Default_Deblocking_Strength (0) */
//#define RV8_Default_Deblocking_Strength     0


typedef enum
{
    RV8_VFMW_FALSE = 0,
    RV8_VFMW_TRUE  = 1
} RV8_TRUEFALSE;

#define RV8_NOTDEC  (-2)    //表示REAL8DEC_Decode()里面有一个NAL没有解码就返回了，下次调用不用送码流了
typedef enum
{
    RV8_FID_RV89COMBO   = 0,
    RV8_FID_REALVIDEO30 = 1
} RV8_StreamVersion;

typedef enum
{
    RV8_ORG_FORCED_INTRAPIC = 0,
    RV8_ORG_INTRAPIC = 1,
    RV8_ORG_INTERPIC = 2,
    RV8_ORG_TRUEBPIC = 3
} EnumRV8PicCodType;

typedef struct
{
    UINT8                   rtype; /* RV8 */
    UINT8                   deblocking_filter_passthrough;
    UINT8                   pquant;

    UINT32                  m_pctszSize; /* size of PCTSZ (RealVideo RPR size index) */
    UINT32                  *m_pSizes; /* location of RealVideo RPR size array */
    UINT32                  m_numSizes; /* number of RPR sizes */

    UINT32                  m_isForcedKey; /* forced keyframe */
    UINT32                  m_nGOBs;       /* number of GOBs per frame */
    UINT32                  m_nMBs;        /* number of MBs per GOB */
    UINT32                  m_mbaSize;     /* size of MBA field */

    EnumRV8PicCodType        ptype;
    SINT32                  mba;
    UINT32                  tr;
    UINT32                  trb;
    UINT32                  dbq;
    UINT32                  pctsz; /* RV8 */
    UINT32                  pwidth;
    UINT32                  pheight;
    UINT32                  pwidth_prev;
    UINT32                  pheight_prev;
} RV8_ParsePicParam;

typedef struct
{
    UINT8  *vir_addr;
    UADDR   phy_addr;
    UINT32  stride;
    UINT32  width;     // unit: MB
    UINT32  height;    // unit: MB
} RV8_PLANE_S;

typedef struct
{
    RV8_PLANE_S  SrcFrame;
    RV8_PLANE_S  DstFrame;
} RV8_RPRINFO_S;

typedef enum
{
    RV8_INTRAPIC    = 0,
    RV8_INTERPIC    = 1,
    RV8_TRUEBPIC    = 2,
    RV8_FRUPIC      = 3
} RV8_PicCodType;

typedef struct
{
    UINT8           	pixel_aspect_ratio;  //no use ?????
    RV8_ParsePicParam  	Rv8ParsePicParam;

    UINT32 				Fid;
    RV8_PicCodType 		PicCodType;
    UINT32 				pic_height_in_pixel;
    UINT32 				pic_width_in_pixel;
    UINT32 				pic_height_in_mb;
    UINT32 				pic_width_in_mb;
    UINT32 				total_mbs;
    UINT32      		m_uTRWrap;
    UINT32      		TR;
    UINT32      		Rounding;   /* Round-off for half-pixel motion compensation   */
    UINT32      		Deblocking_Filter_Passthrough;
    /* This is a non-standard option used in RealVideo */
    /* that indicates that the deblocking filter in annex J */
    /* should not be called. The option will be ignored if */
    /* Annex J is off or if the bitstream is standards-based. */

    UINT32          	PQUANT;
    UINT32            	OSVQUANT;
    UINT32             	TRB;
    UINT32             	DBQUANT;  //no use ?????
    UADDR              	stream_base_addr;
} RV8_PictureHeader;

typedef struct
{
    UINT32 SliceQP;
    UINT32 osvquant;
    UINT32 dblk_filter_passthrough;
    UINT32 first_mb_in_slice;
    UINT32 last_mb_in_slice;

    UINT32 bit_offset_0;
    UINT32 bit_len_0;
    UADDR  bit_stream_addr_0;
    UINT32 bit_offset_1;
    UINT32 bit_len_1;
    UADDR  bit_stream_addr_1;
} RV8_SliceHeader;

typedef struct
{
    UINT8  buf_state; //0:available to VDH write; 1: available to VO read
    UINT8  ref_flag;
    IMAGE imginf;
    UINT32 err_level;
    UINT32 TR;
} RV8_VOBUFINF;


typedef struct
{
    UINT8 *BsVirAddr;        //current bs virtual start addr
    UINT8 *BsVirAddr1;        //current bs virtual start addr
    SINT8  IsLastSeg;
    SINT8  IsLastSeg1;
    UINT8  UsedDecMode;      //当前正在使用的解码模式(IPB, IP, I)
    UINT8  HDRState;          //search pic or slc
    UINT8  Imgbuf_allocated;
    UINT8  LastDecPicCodingType;
    UINT8  Last_FieldStruct;
    UINT8  RefNum;
    SINT8  YuvOutputOrder;

    UINT8 *vahb_vir_addr;         // VAHB物理地址，1024对齐
    UINT8 *upmsg_vir_addr;
    UINT8 *rpmsg_vir_addr;
    UINT8 *picdnmsg_vir_addr;
    UINT8 *slcdnmsg_vir_addr;
    UINT8 *sed_top_vir_addr;
    UINT8 *pmv_top_vir_addr;
    UINT8 *rcn_top_vir_addr;
    UINT8 *pmv_colmb_vir_addr;
    UINT8 *rpr_vir_addr[2];  // 0 used for forward, 1 used for backward
    UINT8 *image_vir_addr[REAL8_MAXIMUM_IMAGE_NUM];


    RV8_VOBUFINF VOBufInf[MAX_QUEUE_IMAGE_NUM];        //VO帧存管理信息

    RV8_PictureHeader PictureHeader;
    RV8_SliceHeader SliceHeader;
    UINT32 PrevPicQP;
    UINT32 PrevPicMb0QP;

    SINT32 TotalSliceNum;
    SINT32 SliceByteOffset[RV8_MAX_SLC_PARAM_NUM];

    SINT32 BsOffset;         //byte offset from packet stream pointer
    UADDR  BsPhyAddr;        //current bs physical start addr
    SINT32 BsLength;
    UADDR  BsPhyAddr1;        //current bs physical start addr
    SINT32 BsLength1;

    SINT32 PacketTail;
    UINT32 NewSeqInited;
    UINT32 StreamIntegrality;//码流中各种头的完整性和正确性信息
    UINT32 SeqCnt;
    UINT32 FrmCnt;
    UINT32 ErrorMbCnt;
    SINT32 Used_byte;//until last packet
    SINT32 HDRUsedBits;

    SINT32 trb;
    SINT32 trd;
    SINT32 Ratio0;
    SINT32 Ratio1;

    /************** VAHB *************/
    UADDR      vahb_phy_addr;         // VAHB物理地址，1024对齐

    //up msg slot
    UADDR      upmsg_phy_addr;

    //repair msg slot
    UADDR      rpmsg_phy_addr;

    //pic down msg slot
    UADDR      picdnmsg_phy_addr;

    //slice down msg slot
    UADDR      slcdnmsg_phy_addr;

    //sed_top_addr
    //  PHYADDR    sed_top_phy_addr;

    //pmv_top_addr
    //  PHYADDR    pmv_top_phy_addr;

    //rcn_top_addr
    UADDR      rcn_top_phy_addr;

    // PMV ColMB存储空间
    //  PHYADDR    pmv_colmb_phy_addr;

    UINT32     rpr_stride;
    UADDR      rpr_phy_addr[2];  // 0 used for forward, 1 used for backward

    // 图像存储空间
    SINT32     QueueImageNum;       // 总共分配的图像个数
    UINT32     ddr_stride;
    UADDR      image_phy_addr[REAL8_MAXIMUM_IMAGE_NUM];

    SINT32 FwdFsID;
    SINT32 BwdFsID;
    SINT32 CurFsID;
    SINT32 ToQueFsID;
    UINT32 PPicFound;
} RV8_CODECINF;

typedef struct
{
    SYNTAX_EXTRA_DATA_S  *pstExtraData;
    RV8_DEC_PARAM_S *pRv8DecParam;
    RV8_DEC_PARAM_S Rv8DecParam;
    RV8_CODECINF   CodecInfo;
    BS            Bs;
    IMAGE_VO_QUEUE  ImageQue;
    SINT32        ChanID;
    UINT32          FakeFrmHeightInMb;
    UINT32          FakeFrmWidthInMb;     
} RV8_CTX_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 REAL8DEC_Init(RV8_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
VOID   REAL8DEC_Destroy(RV8_CTX_S *pCtx);
SINT32 REAL8DEC_Decode(RV8_CTX_S *pCtx, DEC_STREAM_PACKET_S *pPacket);
SINT32 REAL8DEC_RecycleImage(RV8_CTX_S *pCtx, UINT32 ImgID);
SINT32 REAL8DEC_GetRemainImg(RV8_CTX_S *pCtx);
UINT32 REAL8DEC_VDMPostProc(RV8_CTX_S *pCtx, SINT32 ErrRatio, UINT32 Mb0QpInCurrPic);
SINT32 REAL8DEC_GetImageBuffer( RV8_CTX_S *pCtx );
VOID REAL8DEC_Support(SINT32 *flag);

#ifdef __cplusplus
}
#endif


#endif //__REAL8_VFMW_H__
