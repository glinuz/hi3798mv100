#ifndef __DIVX3_VFMW_H__
#define __DIVX3_VFMW_H__


#include "basedef.h"
#include "public.h"

#include "bitstream.h"
#include "syn_cmn.h"
#include "vdm_hal.h"


#define DIVX3_VFMW_MAX_PIC_WIDTH   127 //unit: MB
#define DIVX3_VFMW_MAX_PIC_HEIGHT  127 //unit: MB
#define DIVX3_VFMW_MAX_PIC_MB      (DIVX3_VFMW_MAX_PIC_WIDTH*DIVX3_VFMW_MAX_PIC_HEIGHT) //16129 //unit: MB  

#define DIVX3_QUEUE_IMAGE_NUM                         6
#define DIVX3_MINIMUM_IMAGE_NUM                       6
#define DIVX3_MAXIMUM_IMAGE_NUM                       6

#define DIVX3_MAX_SLOT_NUM                            (128)
#define DIVX3_UP_MSG_SLOT_INDEX                       0
#define DIVX3_RP_MSG_SLOT_INDEX                       1
#define DIVX3_PIC_DN_MSG_SLOT_INDEX                   2
#define DIVX3_SLC_DN_MSG_SLOT_INDEX                   3
#define DIVX3_SLC_DN_MSG_SLOT_FULL_THRESHOLD          (DIVX3_MAX_SLOT_NUM-4)

/*********************(64*4*X) 没部分空间都为64word整数倍****/
#define DIVX3_ALIGN_LEN                               (64*4)
#define DIVX3_SED_TOP_ADDR_LEN                        (64*4*DIVX3_VFMW_MAX_PIC_WIDTH)
#define DIVX3_PMV_TOP_ADDR_LEN                        (64*4*DIVX3_VFMW_MAX_PIC_WIDTH)
#define DIVX3_IT_TOP_ADDR_LEN                         (64*4*DIVX3_VFMW_MAX_PIC_WIDTH)
#define DIVX3_PMV_ONECOLMB_ADDR_LEN                   (20*4)              //计算得知，大致需要16words/mb，现在多分配一点             
#define DIVX3_MSG_SLOT_BURST_LEN                      (64*4)              // 1 burst, 64 word    
#define DIVX3_UP_MSG_SLOT_LEN                         DIVX3_MSG_SLOT_BURST_LEN
#define DIVX3_RP_MSG_SLOT_LEN                         DIVX3_MSG_SLOT_BURST_LEN
#define DIVX3_PIC_DN_MSG_SLOT_LEN                     DIVX3_MSG_SLOT_BURST_LEN
#define DIVX3_ONE_SLC_DN_MSG_SLOT_LEN                 DIVX3_MSG_SLOT_BURST_LEN
#define DIVX3_TOTAL_SLC_DN_MSG_SLOT_LEN               ( DIVX3_MSG_SLOT_BURST_LEN * (DIVX3_MAX_SLOT_NUM-3) )


typedef enum
{
    DIVX3_I_VOP = 0,
    DIVX3_P_VOP = 1,
} DIVX3_PIC_TYPE;

typedef enum
{
    DIVX3_VFMW_FALSE = 0,
    DIVX3_VFMW_TRUE  = 1
} DIVX3_TRUEFALSE;

#define DIVX3_NOTDEC  (-2)    //表示DIVX3DEC_Decode()里面有一个NAL没有解码就返回了，下次调用不用送码流了

typedef struct
{
    UINT32 prediction_type;
    UINT32 quantizer;
    UINT32 rounding_type;
    UINT32 vol_mode;
    UINT32 has_skips;
    UINT32 switch_rounding;

    UINT32 alternative_I_AC_chrom_DCT;  // 0/1: 311_0, 2: 311_10, 3: 311_11
    UINT32 alternative_I_AC_lum_DCT;    // 0/1: 311_0, 2: 311_10, 3: 311_11
    UINT32 alternative_I_DC_DCT;        // 0: 311_0, 1: 311_1
    UINT32 alternative_P_AC_DCT;        // 0/1: 311_0, 2: 311_10, 3: 311_11
    UINT32 alternative_P_DC_DCT;        // 0: 311_0, 1: 311_1
    UINT32 alternative_MV;              // 0: 311_0, 1: 311_1

    //key frame
    UINT32 pic_width_in_mb;
    UINT32 pic_height_in_mb;
    UINT32 pic_width_in_pix;
    UINT32 pic_height_in_pix;
    UINT32 total_mbs;

    /*the fisrt bitstream part*/
    UINT32  bit_len_0;
    UINT32  bit_offset_0;
    UADDR   bit_stream_addr_0;
    UINT32  bit_len_1;
    UINT32  bit_offset_1;
    UADDR   bit_stream_addr_1;

    UADDR   stream_base_addr;
} DIVX3_PictureHeader_S;


typedef struct
{
    UINT8  buf_state; //0:available to VDH write; 1: available to VO read
    UINT8  ref_flag;
    UINT32 err_level;
    UINT32 TR;
    IMAGE  imginf;
} DIVX3_VOBUFINF;


typedef struct
{
    UINT8  IsStreamEndFlag;
    UINT8  Last_FieldStruct;
    UINT8  Imgbuf_allocated;
    UINT8  UsedDecMode;       //当前正在使用的解码模式(IPB, IP, I)
    UINT8 *image_vir_addr[DIVX3_MAXIMUM_IMAGE_NUM];
  	UINT8 *BsVirAddr;         //current bs virtual start addr
    UINT8 *BsVirAddr1;        //current bs virtual start addr

    UINT8 *vahb_vir_addr;         // VAHB物理地址，1024对齐
    UINT8 *upmsg_vir_addr;
    UINT8 *rpmsg_vir_addr;
    UINT8 *picdnmsg_vir_addr;
    UINT8 *slcdnmsg_vir_addr;
    UINT8 *sed_top_vir_addr;
    UINT8 *pmv_top_vir_addr;
    UINT8 *rcn_top_vir_addr;
    UINT8 *it_top_vir_addr;
    UINT8 *pmv_colmb_vir_addr;

    SINT32 BsOffset;          //byte offset from packet stream pointer
    UADDR  BsPhyAddr;         //current bs physical start addr
    SINT32 BsLength;
    SINT32 IsLastSeg;
    UADDR  BsPhyAddr1;        //current bs physical start addr
    SINT32 BsLength1;
    SINT32 IsLastSeg1;

    DIVX3_VOBUFINF VOBufInf[MAX_QUEUE_IMAGE_NUM];        //VO帧存管理信息
    DIVX3_PictureHeader_S PictureHeader;
    UINT32 PrevSwitchRounding;

    SINT32 TotalSliceNum;

    SINT32 PacketTail;
    UINT32 NewSeqInited;
    UINT32 StreamIntegrality; //码流中各种头的完整性和正确性信息
    UINT32 SeqCnt;
    UINT32 FrmCnt;
    UINT32 RefNum;
    UINT32 ErrorMbCnt;
    //UINT32 LastDecPicCodingType;
    SINT32 Used_byte;//until last packet
    UINT32 HDRState;//search pic or slc
    SINT32 HDRUsedBits;

    SINT32 YuvOutputOrder;


    /************** VAHB *************/
    UADDR    vahb_phy_addr;         // VAHB物理地址，1024对齐
    //up msg slot
    UADDR    upmsg_phy_addr;
    //repair msg slot
    UADDR    rpmsg_phy_addr;
    //pic down msg slot
    UADDR    picdnmsg_phy_addr;
    //slice down msg slot
    UADDR    slcdnmsg_phy_addr;
    //sed_top_addr
    //  PHYADDR    sed_top_phy_addr;
    //pmv_top_addr
    //  PHYADDR    pmv_top_phy_addr;
    //rcn_top_addr
    UADDR    rcn_top_phy_addr;
    //it_top_addr
    UADDR    it_top_phy_addr;
    // PMV ColMB存储空间
    //  PHYADDR    pmv_colmb_phy_addr;

    // 图像存储空间
    SINT32     QueueImageNum;       // 总共分配的图像个数
    UINT32     ddr_stride;
    UADDR    image_phy_addr[DIVX3_MAXIMUM_IMAGE_NUM];

    SINT32 FwdFsID;
    SINT32 CurFsID;
    SINT32 ToQueFsID;
} DIVX3_CODECINF;


typedef struct
{
    SYNTAX_EXTRA_DATA_S  *pstExtraData;
    DIVX3_DEC_PARAM_S *pDivx3DecParam;
    DIVX3_DEC_PARAM_S Divx3DecParam;
    DIVX3_CODECINF    CodecInfo;
    BS              Bs;
    IMAGE_VO_QUEUE  ImageQue;
    SINT32          ChanID;
    UINT32          FakeFrmHeightInMb;
    UINT32          FakeFrmWidthInMb;     
} DIVX3_CTX_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 DIVX3DEC_Init(DIVX3_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
VOID   DIVX3DEC_Destroy(DIVX3_CTX_S *pCtx);
SINT32 DIVX3DEC_Decode(DIVX3_CTX_S *pCtx, DEC_STREAM_PACKET_S *pPacket);
SINT32 DIVX3DEC_RecycleImage(DIVX3_CTX_S *pCtx, UINT32 ImgID);
SINT32 DIVX3DEC_GetRemainImg(DIVX3_CTX_S *pCtx);
UINT32 DIVX3DEC_VDMPostProc(DIVX3_CTX_S *pCtx, SINT32 ErrRatio, UINT32 SwitchRounding);
SINT32 DIVX3DEC_GetImageBuffer(DIVX3_CTX_S *pCtx);

#ifdef __cplusplus
}
#endif


#endif //__DIVX3_VFMW_H__
