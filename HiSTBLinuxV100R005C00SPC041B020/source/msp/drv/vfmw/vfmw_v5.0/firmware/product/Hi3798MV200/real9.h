#ifndef __REAL9_VFMW_H__
#define __REAL9_VFMW_H__

#include "basedef.h"
#include "public.h"

#include "bitstream.h"
#include "syn_cmn.h"
#include "vdm_hal.h"


#define REAL9_VFMW_MAX_PIC_WIDTH   127 //unit: MB
#define REAL9_VFMW_MAX_PIC_HEIGHT  127 //unit: MB
#define REAL9_VFMW_MAX_PIC_MB      (REAL9_VFMW_MAX_PIC_WIDTH*REAL9_VFMW_MAX_PIC_HEIGHT) //16129 //unit: MB  

#define REAL9_QUEUE_IMAGE_NUM                         6
#define REAL9_MINIMUM_IMAGE_NUM                       6
#define REAL9_MAXIMUM_IMAGE_NUM                       6

#define REAL9_MAX_SLOT_NUM                            (MAX_SLICE_SLOT_NUM)
#define REAL9_UP_MSG_SLOT_INDEX                       0
#define REAL9_RP_MSG_SLOT_INDEX                       1
#define REAL9_PIC_DN_MSG_SLOT_INDEX                   2
#define REAL9_SLC_DN_MSG_SLOT_INDEX                   3
#define REAL9_SLC_DN_MSG_SLOT_FULL_THRESHOLD          (REAL9_MAX_SLOT_NUM-4)

/*********************(64*4*X) 没部分空间都为64word整数倍****/
#define REAL9_ALIGN_LEN                               (64*4)
#define REAL9_SED_TOP_ADDR_LEN                        (64*4*REAL9_VFMW_MAX_PIC_WIDTH)
#define REAL9_PMV_TOP_ADDR_LEN                        (64*4*REAL9_VFMW_MAX_PIC_WIDTH)
#define REAL9_RCN_TOP_ADDR_LEN                        (64*4*REAL9_VFMW_MAX_PIC_WIDTH)
#define REAL9_DBLK_TOP_ADDR_LEN                       (64*4*REAL9_VFMW_MAX_PIC_WIDTH)
#define REAL9_PMV_ONECOLMB_ADDR_LEN                   (20*4)              //计算得知，大致需要16words/mb，现在多分配一点             
#define REAL9_MSG_SLOT_BURST_LEN                      (64*4)              // 1 burst, 64 word    
#define REAL9_UP_MSG_SLOT_LEN                         REAL9_MSG_SLOT_BURST_LEN
#define REAL9_RP_MSG_SLOT_LEN                         REAL9_MSG_SLOT_BURST_LEN
#define REAL9_PIC_DN_MSG_SLOT_LEN                     REAL9_MSG_SLOT_BURST_LEN
#define REAL9_ONE_SLC_DN_MSG_SLOT_LEN                 REAL9_MSG_SLOT_BURST_LEN
#define REAL9_TOTAL_SLC_DN_MSG_SLOT_LEN               ( REAL9_MSG_SLOT_BURST_LEN * (REAL9_MAX_SLOT_NUM-3) )

#define RV9_PARC_SQUARE        1

#define RV9_TR_SHIFT           14
#define RV9_TR_RND             (1 << (RV8_TR_SHIFT - 1))

#define RV9_TR_WRAP     256
#define RV9_TR_WRAP_RV  8192

/* Filter strength tables */
/* default strength specified by RV_Default_Deblocking_Strength (0) */
//#define RV9_Default_Deblocking_Strength     0


typedef enum
{
    RV9_VFMW_FALSE = 0,
    RV9_VFMW_TRUE  = 1
} RV9_TRUEFALSE;

#define RV9_NOTDEC  (-2)    //表示REAL9DEC_Decode()里面有一个NAL没有解码就返回了，下次调用不用送码流了
typedef enum
{
    RV9_FID_RV89COMBO   = 0,
    RV9_FID_REALVIDEO30 = 1
} RV9_StreamVersion;

typedef enum
{
    RV9_ORG_FORCED_INTRAPIC = 0,
    RV9_ORG_INTRAPIC = 1,
    RV9_ORG_INTERPIC = 2,
    RV9_ORG_TRUEBPIC = 3
} EnumRV9PicCodType;

typedef struct
{
    UINT8                   rtype; /* RV8 */
    UINT8                   deblocking_filter_passthrough;
    UINT8                   pquant;

    UINT32                  m_isForcedKey; /* forced keyframe */
    UINT32                  m_nGOBs;       /* number of GOBs per frame */
    UINT32                  m_nMBs;        /* number of MBs per GOB */
    UINT32                  m_mbaSize;     /* size of MBA field */

    EnumRV9PicCodType        ptype;
    SINT32                  mba;
    UINT32                  tr;
    UINT32                  trb;
    UINT32                  dbq;
    UINT32                  pctsz; /* RV8 */
    UINT32                  pwidth;
    UINT32                  pheight;
    UINT32                  pwidth_prev;
    UINT32                  pheight_prev;
} RV9_ParsePicParam;

typedef struct
{
    UINT8  *vir_addr;
    UADDR   phy_addr;
    UINT32  stride;
    UINT32  width;     // unit: MB
    UINT32  height;    // unit: MB
} RV9_PLANE_S;

typedef struct
{
    RV9_PLANE_S  SrcFrame;
    RV9_PLANE_S  DstFrame;
} RV9_RPRINFO_S;

typedef enum
{
    RV9_INTRAPIC    = 0,
    RV9_INTERPIC    = 1,
    RV9_TRUEBPIC    = 2,
    RV9_FRUPIC      = 3
} RV9_PicCodType;

typedef struct
{
    UINT8             	pixel_aspect_ratio;  //no use ?????
    UINT32 				Fid;
    RV9_PicCodType 		PicCodType;
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

    UINT32            	PQUANT;
    UINT32            	OSVQUANT;
    UINT32            	TRB;
    UINT32            	DBQUANT;  //no use ?????
    RV9_ParsePicParam 	Rv9ParsePicParam;

    UADDR             stream_base_addr;
} RV9_PictureHeader;

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
} RV9_SliceHeader;

typedef struct
{
    UINT8  buf_state; //0:available to VDH write; 1: available to VO read
    UINT8  ref_flag;
    IMAGE  imginf;
    UINT32 err_level;
    UINT32 TR;
} RV9_VOBUFINF;


typedef struct
{
    UINT8 *BsVirAddr;        //current bs virtual start addr
    SINT8  IsLastSeg;
    SINT8  IsLastSeg1;
    UINT8 *BsVirAddr1;        //current bs virtual start addr
    UINT8  UsedDecMode;      //当前正在使用的解码模式(IPB, IP, I)
    UINT8  RefNum;
    UINT8  LastDecPicCodingType;
    UINT8  Last_FieldStruct;
    UINT8  Imgbuf_allocated;
    UINT8  HDRState;//search pic or slc
    SINT8  YuvOutputOrder;

    UINT8     *vahb_vir_addr;         // VAHB物理地址，1024对齐
    UINT8     *upmsg_vir_addr;
    UINT8     *rpmsg_vir_addr;
    UINT8     *picdnmsg_vir_addr;
    UINT8     *slcdnmsg_vir_addr;
    UINT8     *sed_top_vir_addr;
    UINT8     *pmv_top_vir_addr;
    UINT8     *rcn_top_vir_addr;
    UINT8     *pmv_colmb_vir_addr;
    UINT8     *rpr_vir_addr[2];  // 0 used for forward, 1 used for backward
    UINT8     *image_vir_addr[REAL9_MAXIMUM_IMAGE_NUM];

    RV9_VOBUFINF VOBufInf[MAX_QUEUE_IMAGE_NUM];        //VO帧存管理信息

    RV9_PictureHeader PictureHeader;
    RV9_SliceHeader SliceHeader;
    UINT32 PrevPicQP;
    UINT32 PrevPicMb0QP;

    SINT32 TotalSliceNum;
    SINT32 SliceByteOffset[RV9_MAX_SLC_PARAM_NUM];

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

    //RV9_VOBUFINF *pFWD;
    //RV9_VOBUFINF *pBWD;
    //RV9_VOBUFINF *pRCN;
    //RV9_VOBUFINF *pToQue;

    SINT32 trb;
    SINT32 trd;
    SINT32 Ratio0;
    SINT32 Ratio1;

    /************** VAHB *************/
    UADDR    vahb_phy_addr;         // VAHB物理地址，1024对齐

    //up msg slot
    UADDR    upmsg_phy_addr;

    //repair msg slot
    UADDR    rpmsg_phy_addr;

    //pic down msg slot
    UADDR    picdnmsg_phy_addr;

    //slice down msg slot
    UADDR      slcdnmsg_phy_addr;

    //sed_top_addr
    // PHYADDR    sed_top_phy_addr;

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
    UADDR      image_phy_addr[REAL9_MAXIMUM_IMAGE_NUM];

    SINT32 FwdFsID;
    SINT32 BwdFsID;
    SINT32 CurFsID;
    SINT32 ToQueFsID;
    UINT32 PPicFound;
} RV9_CODECINF;

typedef struct
{
    SYNTAX_EXTRA_DATA_S  *pstExtraData;
    RV9_DEC_PARAM_S *pRv9DecParam;
    RV9_DEC_PARAM_S Rv9DecParam;
    RV9_CODECINF    CodecInfo;
    BS              Bs;
    IMAGE_VO_QUEUE  ImageQue;
    SINT32          ChanID;
    UINT32          FakeFrmHeightInMb;
    UINT32          FakeFrmWidthInMb;     
} RV9_CTX_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 REAL9DEC_Init(RV9_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
VOID   REAL9DEC_Destroy(RV9_CTX_S *pCtx);
SINT32 REAL9DEC_Decode(RV9_CTX_S *pCtx, DEC_STREAM_PACKET_S *pPacket);
SINT32 REAL9DEC_RecycleImage(RV9_CTX_S *pCtx, UINT32 ImgID);
SINT32 REAL9DEC_GetRemainImg(RV9_CTX_S *pCtx);
UINT32 REAL9DEC_VDMPostProc(RV9_CTX_S *pCtx, SINT32 ErrRatio, UINT32 Mb0QpInCurrPic);
SINT32 REAL9DEC_GetImageBuffer( RV9_CTX_S *pCtx );
VOID   REAL9DEC_Support(SINT32 *flag);
#ifdef __cplusplus
}
#endif


#endif //__REAL9_VFMW_H__
