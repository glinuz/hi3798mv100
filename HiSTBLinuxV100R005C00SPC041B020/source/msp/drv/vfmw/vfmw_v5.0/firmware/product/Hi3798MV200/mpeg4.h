#ifndef __MPEG4_H__
#define __MPEG4_H__

#include "basedef.h"
#include "public.h"
#include "vfmw.h"
#include "vdm_hal.h"
#include "syntax.h"

/* 返回状态宏定义*/
#define MPEG4_FAIL            -1
#define MPEG4_OK              0
#define MPEG4_NOTDEC          3

#define MPEG4_INIT_SUCCESS         0
#define MPEG4_INIT_FAIL            -1

#define MPEG4_DEC_FAIL             -1
#define FRAME_REQ_ABNORMITY        -2
#define FRAME_REQ_FAIL             -1
#define FRAME_REQ_SUCCESS          0


/*按照最大空间分配 */
#define FRAME_ALLOC_NUM            8


/* 语法用到的宏定义*/
//#define    OVERFLOW                       1          // 码流越界
#define    SYN_SHOK                       3          // ShortHeader解码正常
#define    SYNTAX_OK                      1          // 语法解析正确，返回配置硬件状态

#define    NO_START_CODE                  0xc5       // 没有找到起始码
#define    SHORT_HEADER                   0xc4       // 短头格式起始码

#define    MARKER_THRESHOLD               6400          // marker的阈值为4字节
#define    SH_THRESHOLD                   18          // ShortHeader头标识计数//modify by l57648 from 8 to 18,Because we found 13 Short Header in X-Men stream.
#define    PEI_THRESHOLD                  16         // shortheader语法元素pei的阈值

/* coding_mode */
#define    ONLY_I_MODE                    1          // 解码模式
#define    NEXT_I_MODE                    2          // 解码模式

/*start_code */
#define    SHORT_VIDEO_START_CODE         0x00008000

#define    START_CODE_PRE                 0x00000100
#define    VIDOBJLAY_START_CODE_MASK      0x0000000f
#define    VIDOBJ_START_CODE_MASK         0x0000001f
#define    VIDOBJ_START_CODE              0x00000100
#define    VIDOBJLAY_START_CODE           0x00000120
#define    VISOBJSEQ_START_CODE           0x000001b0
#define    VISOBJSEQ_STOP_CODE            0x000001b1
#define    GRPOFVOP_START_CODE            0x000001b3
#define    VISOBJ_START_CODE              0x000001b5
#define    VOP_START_CODE                 0x000001b6
#define    USERDATA_START_CODE            0x000001b2
#define    HISI_END_OF_FRAME              0x0000011e
#define    HISI_MP4_STREAM_END_CODE       0x000001b600008000     // for bvt w00271806

// 这个值不能随便修改，如要修改需要同时改scd过滤头及PTS检测新帧seg两处地方
#define    HISI_STREAM_END_CODE           0x000001fe             // for eos y00226912

/* VOS层*/
#define    SP_LEVEL1        0x01
#define    SP_LEVEL2        0x02
#define    SP_LEVEL3        0x03
#define    SP_LEVEL0        0x08
#define    ASP_LEVEL0       0xF0
#define    ASP_LEVEL1       0xF1
#define    ASP_LEVEL2       0xF2
#define    ASP_LEVEL3       0xF3
#define    ASP_LEVEL4       0xF4
#define    ASP_LEVEL5       0xF5

#define    SYN_VOSOK        1              // VOS解码正常
#define    SYN_VOSERR       0              // VOS解码错误

/*VO层*/
#define    SYN_VOOK         1              // VO解码正常
#define    SYN_VOERR        0              // VO解码错误
#define    VIDEO_ID         1              // Visual_Object_type Video ID

/*GOP层*/
#define    SYN_GOPOK        1              // GOP解码正常
#define    SYN_GOPERR       0              // GOP解码错误

/*VOL层*/
#define    SYN_VOLOK                    1              // VOL解码正常
#define    SYN_VOLERR                   0              // VOL解码错误
#define    VO_Type_SP                   0x00000001     // video_object_type为SP
#define    VO_Type_ASP                  0x11           // video_object_type为ASP
#define    VIDOBJLAY_AR_EXTPAR          15             // Aspect_ratio_info为extended PAR
#define    VIDOBJLAY_SHAPE_RECTANGULAR  0              // video_object_layer_shape为矩形
#define    VIDOBJLAY_SHAPE_BINARY_ONLY  2
#define    SPRITE_NONE    0
#define    SPRITE_STATIC  1
#define    SPRITE_GMC	     2

/*VOP层*/
#define    I_VOP                 0
#define    P_VOP                 1
#define    B_VOP                 2
#define    S_VOP                 3
#define    N_VOP                 4
#define    SYN_VOPOK             1              // VOP解码正常
#define    SYN_VOPERR            0              // VOP解码错误

/*USERDATA*/
#define    SYN_USERDATAOK        1              // USERDATA解码正常
#define    SYN_USERDATAERR       0              // USERDATA解码错误

/* seg num in vop */
#define    MAX_SEG_NUM_IN_VOP     1024
#define    MAX_PKT_NUM_IN_HD      64

#define MP4_FOUND_SEQHDR                       (0x1)
#define MP4_FOUND_VOPHDR                       (0x1<<1)
#define MP4_FOUND_VOLHDR                       (0x1<<2)
#define MP4_FOUND_SHHDR                        (0x1<<3)
#define MP4_FOUND_SLCHDR                       (0x1<<4)
#define MP4_FOUND_NEXTVOPHDR                   (0x1<<5)
#define MP4_FOUND_NEXTSHHDR                    (0x1<<6)
#define MP4_FOUND_NVOP                         (0x1<<7)
#define MP4_FOUND_HISIHDR                      (0x1<<8)
#define MP4_PIC_OK                             (MP4_FOUND_VOPHDR|MP4_FOUND_SLCHDR|MP4_FOUND_NEXTVOPHDR|MP4_FOUND_NVOP|MP4_FOUND_HISIHDR)
#define MP4_SH_PIC_OK                          (MP4_FOUND_SHHDR|MP4_FOUND_SLCHDR|MP4_FOUND_NEXTSHHDR)

#define SLICE_START_CODE_BASE_LEN   17

#define RDIV(a,b) (((a)>0 ? ((a) + ((b)>>1)) : ((a) - ((b)>>1)))/(b))
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))
#define FFMAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct
{
    SINT32 x;
    SINT32 y;
} MPEG4_MOTION_VECTOR;

typedef struct
{
    MPEG4_MOTION_VECTOR duv[3];
} MPEG4_WARPPOINTS;

typedef struct
{
    /*  0=none, 1=translation, 2,3 = warping
    *  a value of -1 means: "structure not initialized!" */
    SINT32 num_wp;
    /* {0,1,2,3}  =>   {1/2,1/4,1/8,1/16} pel */
    SINT32 accuracy;
    /* sprite size * 16 */
    SINT32 sW, sH;
    /* gradient, calculated from warp points */
    SINT32 dU[2], dV[2], Uo, Vo, Uco, Vco;
    /*协议中的含义
      Uo: du[0]*s/2;
      Vo: dv[0]*s/2;
    */
} MP4_GMC_DATA;

typedef struct
{
    UINT8     is_visual_object_identifier;
    UINT8     visual_object_verid;
    UINT8     visual_object_priority;
    UINT8     visual_object_type;
    UINT8     video_signal_type;
    UINT8     video_format;
    UINT8     video_range;
    UINT8     colour_description;

    UINT32    profile_and_level_indication;
    UINT32    colour_primaries;
    UINT32    transfer_characteristics;
    UINT32    matrix_coefficients;
} MP4_VOS;

typedef struct
{
    UINT8     is_object_layer_identifier;
    UINT8     video_object_layer_verid;
    UINT8     video_object_layer_priority;
    UINT8     aspect_ratio_info;
    UINT8     random_accessible_vol;
    UINT8     vol_control_parameters;
    UINT8     chroma_format;
    UINT8     low_delay;
    UINT8     video_object_layer_shape;
    UINT8     fixed_vop_rate;
    UINT8     interlaced;
    UINT8     obmc_disable;
    UINT8     sprite_enable;
    UINT8     not_8_bit;
    UINT8     quant_type;
    UINT8     load_intra_quant_mat;
    UINT8     load_nonintra_quant_mat;
    UINT8     quarter_sample;
    UINT8     complexity_estimation_disable;
    UINT8     resync_marker_disable;
    UINT8     data_partitioned;
    UINT8     newpred_enable;
    UINT8     reduced_resolution_vop_enable;
    UINT8     scalability;
    //l00165842 GMC
    UINT8     sprite_warping_points;
    UINT8     sprite_warping_accuracy;
    UINT8     sprite_brightness_change;

    UINT32    video_object_type_indication;
    UINT32    par_width;
    UINT32    par_height;
    UINT32    vbv_parameters;
    UINT32    first_half_bit_rate;
    UINT32    latter_half_bit_rate;
    UINT32    first_half_vbv_buffer_size;
    UINT32    latter_half_vbv_buffer_size;
    UINT32    first_half_vbv_occupancy;
    UINT32    latter_half_vbv_occupancy;
    UINT32    vop_time_incr_bits;
    UINT32    vop_time_increment_resolution;
    UINT32    fixed_vop_time_increment;
    UINT32    video_object_layer_width;
    UINT32    video_object_layer_height;
    UINT32    intra_quant_mat[64];
    UINT32    nonintra_quant_mat[64];
    UINT32    time_code;

} MP4_VOL;

typedef struct
{
    UINT8     vop_coding_type;
    UINT8     vop_coded;
    UINT8     vop_rounding_type;
    UINT8     vop_reduced_resolution;
    UINT8     intra_dc_vlc_thr;
    UINT8     top_field_first;
    UINT8     alternate_vertical_scan_flag;
    UINT8     vop_quant;
    UINT8     vop_fcode_forward;
    UINT8     vop_fcode_backward;
    UINT8     source_format;
    UINT8     num_gobs_invop;
    UINT8     num_mb_ingob;
    UINT32    temporal_reference;
    UINT32    split_screen_indicator;
    UINT32    document_camera_indicator;
    UINT32    full_picture_freeze_release;
    UINT32    mb_total;
    MPEG4_WARPPOINTS gmc_warp;
} MP4_VOP;

typedef struct
{
    UINT8     vos_decode_over;
    UINT8     vol_decode_over;
    UINT8     closed_gov;
    UINT8     broken_link;
    UINT8     packed_mode;
    UINT8     bs_version;
    UINT8     cartoon_mode;
    UINT8     coding_mode;
    UINT8     decode_syntax_OK;
    UINT8     is_short_header;

    UINT8     FF_BUG_EDGE_FIND_POINT;
    UINT8     FF_BUG_EDGE_EXTEND;
    UINT8     FF_BUG_DIVX500B413;
    UINT8     FF_BUG_QPEL_CHROMA;
    UINT8     FF_BUG_QPEL_CHROMA2;
    UINT8     FF_BUG_QPEL_FILED;
    MP4_VOS   syn_vos;
    MP4_VOL   syn_vol;
    MP4_VOP   syn_vop;

    UINT32    image_height;
    UINT32    image_width;
    UINT32    time_incr;
    UINT32    time_increment;
    UINT32    time_base;
    UINT32    last_time_base;
    UINT32    time;
    UINT32    last_non_b_time;
    UINT32    time_pp;
    UINT32    time_bp;
    UINT32    SH_num;
    /* divx specific, used to workaround (many) bugs in divx5 */
    UINT32    divx_version;
    UINT32    divx_build;
    UINT32    divx_packed;
    UINT32    xvid_build;
    /* lavc specific stuff, used to workaround bugs in libavcodec */
    UINT32    lavc_build;

} MP4_SYNTAX;

typedef struct
{
    UINT8     vos_decode_right;
    UINT8     vo_decode_right;
    UINT8     gop_decode_right;
    UINT8     vol_decode_right;
    UINT8     last_vol_decode_right;
    UINT8     vop_decode_right;
    UINT8     userdata_decode_right;
} MP4_SYNTAX_STATE;


/* VDM 相关数据 */
typedef struct
{
    UINT8     *pmv_vir_addr;
    UINT8     *frame_vir_addr;
    UADDR     pmv_phy_addr;
    UINT32    pmv_size;
    UADDR     frame_phy_addr;
    UINT32    frame_size;
} VDM_MEM_ALLOT;
typedef struct
{
    UADDR     image_curr_frame_addr;
    UINT32    image_curr_recon_addr_id;
    UINT32    image_ref0_addr_id;
    UINT32    image_ref1_addr_id;
    UADDR     pmv_curr_frame_addr;
    UADDR     pmv_backward_ref_addr;
} MSG_AV_IMG2PMV_ADDR;

/* 码流管理信息*/
typedef struct
{
    UINT8     *vir_addr;
    UADDR     phy_addr;
    UINT32    len;
    UINT32    offset;        //字节第几比特开始有效
} BITS_OBJ;
#if 0
typedef struct
{
    UINT8 *BsVirAddr[2];
    SINT8  IsLastSeg[2];
    UADDR  BsPhyAddr[2];
    SINT32 BsLength[2];
    SINT32 StreamID[2];

} MP4_STREAM_PACKET;
#endif

typedef struct
{
    DEC_STREAM_PACKET_S stLastPacket;
    MP4_SCD_UPMSG   stScdUpMsg;
    UINT32 StreamId[MAX_PKT_NUM_IN_HD];
    SINT32 StreamIDNum;
    UINT32 HeadCode;
    SINT32 GetHeadFirstFlag;
    SINT32 CountVopNum;
} MP4_HDPKT_INFO_S;


typedef struct
{
    UINT8  FwdCodingType;
    UINT8  BwdCodingType;
    UINT8  CurCodingType;

    UINT8  TmpUserData[MAX_USRDAT_SIZE + 8]; /* +8 为了确保安全 */
    MP4_DEC_PARAM_S *pMp4DecParam;
    MP4_DEC_PARAM_S Mp4DecParam;
    DEC_STREAM_PACKET_S  stCurPacket;
    MP4_SCD_UPMSG        ScdUpMsg;
    UINT32 StreamID[MAX_SEG_NUM_IN_VOP];
    UINT32 StreamIDNum;
    UINT32 PicIntegrality;
    UINT32 IsWaitToDecFlag;

    MP4_VOS MP4Vos;
    MP4_VOL MP4Vol;
    MP4_VOL MP4VolLastBack; /* 记录最近一次解析OK的信息 */
    MP4_VOP MP4Vop;
    MP4_SYNTAX MP4Syntax;
    MP4_SYNTAX_STATE MP4SyntaxState;
    BITS_OBJ BitsObj;

    SYNTAX_EXTRA_DATA_S *pstExtraData;
    VDM_MEM_ALLOT VdmMemAllot;
    SINT32 NumOfAllocatedFrameObj;

    DEC_MODE_E DecMode;
    MP4_HDPKT_INFO_S stHdPktInfo;

    UINT32 SeqCnt;
    UINT32 ImgCntOfSeq;
    UINT32 ErrorLevel;

    UINT32 InitComplete;
    UINT32 FrameNum;
    UINT32 OldPicWidth;
    UINT32 OldPicHeight;
    UINT32 NewPicWidth;
    UINT32 NewPicHeight;
    UINT32 UserID;
    UINT32 PPicFound;  // 0:表示还没有发现P帧 1表示发现了P帧或者发现了两个I帧 2表示发现了第一个I帧。例如 IIBBBIBB

    VDEC_USRDAT_S  *UsrData[4];
    VDEC_USRDAT_S  *CurUsrData;

    /* 如果IP VOP出错，寻找下一个IVOP的标示，=1代表IPVOP出错*/
    UINT32 IPVopErr;

    IMAGE_VO_QUEUE     ImageQue;
    SINT32        ChanID;

    //FSP Used
    //IMAGE  img;
    SINT32 FspPartitionOK;
    SINT32 RefNum;
    SINT32 FwdFsID;
    SINT32 FwdTimePP;
    SINT32 BwdFsID;
    SINT32 BwdTimePP;
    SINT32 CurFsID;
    SINT32 ToQueFsID;
    UINT8 IsStreamEndFlag;
    UINT8 SeekReset;
    UINT32 LastPicIntegrality;
    MP4_SCD_UPMSG LastScdUpMsg;
} MP4_CTX_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 MPEG4DEC_Init(MP4_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
VOID MPEG4DEC_Destroy(MP4_CTX_S *pCtx);
SINT32 MPEG4DEC_Decode(MP4_CTX_S *pCtx, const DEC_STREAM_PACKET_S *ptr_strm_para);
SINT32 MPEG4DEC_RecycleImage(MP4_CTX_S *pCtx, UINT32 ImgID);
SINT32 MPEG4DEC_GetRemainImg(MP4_CTX_S *pCtx);
SINT32 MPEG4DEC_VDMPostProc(MP4_CTX_S *pCtx, SINT32 ErrRatio, LUMA_INFO_S *pLumaInfo);
SINT32 MPEG4DEC_GetImageBuffer( MP4_CTX_S *pCtx );
SINT32 MP4_GetPacketState(MP4_CTX_S *pCtx);
VOID MP4_ClearCurPic(MP4_CTX_S *pCtx);
VOID MP4_Clean_HeadPacket(MP4_CTX_S *pCtx);
UINT32 MPEG4DEC_GetImageWidth(MP4_CTX_S *pCtx);
UINT32 MPEG4DEC_GetImageHeight(MP4_CTX_S *pCtx);

#ifdef __cplusplus
}
#endif


#endif //__MPEG4_H__
