
/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : avs_vfmw.h
  版 本 号   : 初稿
  作    者   : y62639
  生成日期   : 2007年12月14日
  最近修改   :
  功能描述   : avs vfmw header file
  函数列表   :
  修改历史   :
  1.日    期   : 2007年12月14日
    作    者   : y62639
    修改内容   : 创建文件

******************************************************************************/

#ifndef __AVS_H__
#define __AVS_H__


#include    "syntax.h"
#include    "basedef.h"
#include    "public.h"
#include    "bitstream.h"
#include    "vfmw.h"
#include    "vdm_hal.h"


/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define AVS_FALSE                            0
#define AVS_TRUE                             1
#define AVS_NOTDEC                           0
#define AVS_DEC                               1
#define AVS_WAIT                             3 //l00273086

#define ONE_PIC_ONE_PIC                      0             // enter "c" to continue decoding
#define CODE_SEQSTART                        0xB0
#define CODE_SEQEND                          0xB1
#define CODE_UDSTART                         0xB2
#define CODE_IPIC                            0xB3
#define CODE_RSV0                            0xB4
#define CODE_EXT                             0xB5
#define CODE_PBPIC                           0xB6
#define CODE_EDIT                            0xB7
#define CODE_RSV1                            0xB8
#define HISI_AVS_STREAM_END_CODE      0xFE

#define EXTCODE_SEQDISP                      0x2
#define EXTCODE_COPYRT                       0x4
#define EXTCODE_PICDISP                      0x7
#define EXTCODE_CMRPRM                       0xb

#define CUR_FRAME_BUF_NUM                    10
#define MIN_FRAME_BUF_NUM                    4
#define MAX_FRAME_BUF_NUM                    10

#define SEQ_EXT_AND_USD                     0
#define PIC_EXT_AND_USD                     1

#define I_PICTURE                            0
#define P_PICTURE                            1
#define B_PICTURE                            2

#define AVS_HDRERR_SEQ                       (0x1)
#define AVS_HDRERR_SEQDSP                    (0x1<<1)
#define AVS_HDRERR_PIC                       (0x1<<2)
#define AVS_HDRERR_PICDSP                    (0x1<<3)
#define AVS_FOUND_NEXTHDR                    (0X1<<14)
#define AVS_FOUND_SLCHDR                     (0X1<<15)
#define AVS_FOUND_SEQHDR                     (0x1<<16)
#define AVS_FOUND_SEQDSPHDR                  (0x1<<17)
#define AVS_FOUND_PICHDR                     (0x1<<18)
#define AVS_FOUND_PICDSPHDR                  (0x1<<19)
#define AVS_FOUND_UD                         (0x1<<20)
#define AVS_FOUND_COPYRT                     (0x1<<21)
#define AVS_FOUND_CMRPRM                     (0x1<<22)
#define AVS_BASIC_HDR                        (AVS_FOUND_SEQHDR|AVS_FOUND_PICHDR)
#define AVS_PIC_OK                           (AVS_FOUND_SEQHDR|AVS_FOUND_PICHDR|AVS_FOUND_SLCHDR|AVS_FOUND_NEXTHDR)
typedef struct
{
    UINT8  PicStruct;
    UINT8  PicCodingType;
    SINT8  TopFldFirst;
    SINT32 FspID;
    UINT32 pic_distance;
} FRAME_OBJ;


typedef struct
{
    /* 寄存器信息 */
    UINT32 mbamt_to_dec;
    UINT32 mv_output_en;
    UINT32 load_qmatrix_flag;
    UINT32 ddr_stride;
    UINT32 video_standard;
    UADDR  av_msg_addr;
    UADDR  va_msg_addr;
    UADDR  tab_base_addr;
    UADDR  stream_base_addr;

    UINT32 ref_pic_type0;  //bwd
    UINT32 ref_pic_type1;  //fwd0
    UINT32 ref_pic_type2;  //fwd1
    UINT32 ref_pic_type3;  //curr
    UADDR  ystaddr_1d;

    /* 下行消息池*/
    /*D0*/
    UINT32 pic_height_in_mb;
    UINT32 pic_width_in_mb;
    // add by l00225186, 2013.02.27
    UINT32 sample_precision;
    UINT32 chroma_format;
    // end add by l00225186
    /*D1*/
    UINT32 no_fwd_ref_flag;
    UINT32 pic_ref_flag;
    UINT32 ad_pred_mode_disable;
    UINT32 skip_mode_flag;
    UINT32 pic_qp;
    UINT32 fix_qp;
    UINT32 repeat_first_field;
    UINT32 top_field_first;
    UINT32 pic_coding_type;
    UINT32 picture_structure;
    UINT32 progressive_frame;
    /*D2*/
    UINT32 beta_offset;
    UINT32 alpha_c_offset;
    UINT32 loop_flt_para_flag;
    UINT32 loop_flt_disable;
    /*D3*/
    SINT32 bwd_picture_structure;
    SINT32 bwd_top_field_first;
    SINT32 fwd_picture_structure_1;
    SINT32 fwd_top_field_first_1;
    SINT32 fwd_picture_structure_0;
    SINT32 fwd_top_field_first_0;
    /*D4*/
    UADDR bwd_address_0;
    /*D5*/
    UADDR fwd_address_0;

    /*D6*/
    UADDR  fwd_address_1;          
    /*D7*/
    UADDR  rcn_address;            
    /*D8*/
    UINT32 col_pic_struct;
    UINT32 col_pic_type;
    /*D9*/
    UADDR  bit_stream_address_0;   
    /*D10*/
    UINT32 bit_offset_0;
    UINT32 bit_len_0;
    /*D11*/
    UADDR  bit_stream_address_1; 
    UINT32 bit_offset_1;
    UINT32 bit_len_1;
    /*D12*/
    UINT32 weighting_quant_flag;
    UINT32 chroma_quant_param_disable;
    UINT32 chroma_quant_param_delta_v;
    UINT32 chroma_quant_param_delta_u;
    UINT32 aec_enable;
    UINT32 p_filed_enhanced_flag;
    UINT32 b_filed_enhanced_flag;
    /*D13*/
    UADDR  current_pmv_addr;
    /*D14*/
    UADDR topmb_intra_addr;
    /*D15*/
    UADDR  compressed_data_addr;
    /*D16*/
    UADDR  colpic_mv_addr;         
    /*D17-D48*/
    UINT32 pmvtable[32];
    //add by l00225186, 2013.1.21
    /*D49*/
    UADDR  pmv_colmb_addr;
    /* WQ Matrix */
    SINT16 cur_wq_matrix[64];
} AVS_VDMCFG;


typedef struct
{
    UINT8    reserved_bits;
    UINT8    low_delay;
    UINT8    marker_bit;
    UINT8    chroma_format;
    UINT8    sample_precision;
    UINT8    aspect_ratio;
    UINT8    frame_rate_code;
    UINT8    progressive_sequence;
    UINT32   profile_id;
    UINT32   level_id;
    UINT32   horizontal_size;
    UINT32   vertical_size;
    UINT32   bit_rate_lower;
    UINT32   bit_rate_upper;
    UINT32   bbv_buffer_size;
} AVS_SEQ;


typedef struct
{
    UINT8    reserved_bits;
    UINT8    marker_bit;
    UINT8    video_format;
    UINT8    sample_range;
    UINT8    colour_description;
    UINT32   colour_primaries;
    UINT32   transfer_characteristics;
    UINT32   matrix_coefficients;
    UINT32   display_horizontal_size;
    UINT32   display_vertical_size;
    //add by y00226912, 2012.8.28
    UINT32 stereo_pakcing_mode;
} AVS_SDEXT;


typedef struct
{
    UINT8    progressive_frame;
    UINT8    picture_structure;
    UINT8    top_field_first;
    UINT8    repeat_first_field;
    UINT8    fixed_picture_qp;
    UINT8    picture_qp;
    UINT8    skip_mode_flag;
    UINT8    reserved_bits;
    UINT8    loop_filter_disable;
    UINT8    loop_filter_parameter_flag;
    UINT8    time_code_flag;
    UINT8    picture_coding_type;
    UINT8    advanced_pred_mode_disable;
    UINT8    picture_reference_flag;
    UINT8    no_forward_reference_flag;
    /* I Pic  */
    UINT32   bbv_delay;
    UINT32   bbv_delay_extension;
    UINT32   time_code;
    UINT32   marker_bit;
    UINT32   picture_distance;
    UINT32   bbv_check_times;
    UINT32   alpha_c_offset;
    UINT32   beta_offset;

    //add by l00225186, 2013.1.21
    UINT32 weighting_quant_flag;
    UINT32 mb_adapt_weighting_quant_disable;
    UINT32 chroma_quant_param_disable;
    UINT32 chroma_quant_param_delta_u;
    UINT32 chroma_quant_param_delta_v;
    UINT32 weighting_quant_param_index;
    UINT32 weighting_quant_model;
    UINT32 weighting_quant_param_delta1[6];
    UINT32 weighting_quant_param_delta2[6];
    SINT16 cur_wq_matrix[64];
    UINT32 aec_enable;
    //end add
    /* PB Pic  */
    UINT32   pb_picture_start_code;
    UINT32   p_filed_enhanced_flag;
    UINT32   b_filed_enhanced_flag;
    UINT32   derived_pic_coding_type;
} AVS_PIC;


typedef struct
{
    UINT32 frame_centre_horizontal_offset[3];
    UINT32 frame_centre_vertical_offset[3];
} AVS_PDEXT;


typedef struct
{
	UINT8     bAvsPlus;
    UINT32    NumOfFrameCentreOffsets;
    AVS_SEQ   syn_seq;
    AVS_SDEXT syn_sdext;
    AVS_PIC   syn_pic;
    AVS_PDEXT syn_pdext;
} AVS_SYNTAX;


typedef struct
{
    UINT8             *pVdmBsPos; /*配置给VDM的码流起始指针(byte对齐)*/
    FRAME_OBJ          ZeroFrm;
    FRAME_OBJ          FrameObj[5];	//B fram alway in 4 Buf// ref[3] + 1 for Bframe
    FRAME_OBJ         *pCurFrm;
    FRAME_OBJ         *pRef[3];   /*其中0为最近解出的参考图，1为较早解出的参考图*/
    /*码流信息*/
    SINT32             UsedByte;  /*跟踪调试信息，整个码流的消耗byte数*/
    SINT32             PicMbWidth;
    SINT32             PicMbHeight;
    UINT32             Integrity;
    SINT32             NewSeq;
    UINT32             SeqCnt;
    SINT32             SizeChanged;
    AVS_SYNTAX         Syntax;
    //DEC_STREAM_PACKET_S*      pBsPack;
    BS                *pBs;
    SINT32             VdmBsLen;  /*配置给VDM的码流长度(bytes)*/
    /*资源信息*/
    SINT32             NumOfAllocatedFrameObj;//现在没有使用此量
    SINT32             CurFrameObjId;

    UADDR              pmv_colmb_buf_phyaddr_bpic;
    UADDR              pmv_colmb_buf_phyaddr;
    SINT32             DDR_Stride;
    IMAGE_VO_QUEUE     *pstImgQue;
    /*pmv的表信息*/
    SINT32             Idx2Dist[8]; /*curfld:dir:reffld*/
    SINT32             MvEScale[32];
    SINT32             DirectScale[8];
    SINT32             SymScale[4];
    SINT32             BBlkDist[8];
    /*其它控制信息*/
    SINT32             UsedDecMode;   /* 解码模式，I，IP，IPB */
    SINT32             RefNum;         /* 已解出来的参考图个数 */
    SINT32             ErrRatio;      /* 一帧图象的解码错误率，0~100 */
    SINT32             YuvOutputOrder;
    SINT32             PicCnt;
    /*usr data*/
    VDEC_USRDAT_S             *pSeqUD;
    VDEC_USRDAT_S             *pPicUD;
    //add by l00225186, 2013.1.21
    UINT32           chroma_format;
    UINT32           sample_precision;

    //add by l00225186, 2013.1.21
    UINT32 b_filed_enhanced_flag;

    UINT32             u32LastIntegrity; //l00273086
} AVS_CODECINF;


typedef struct
{
    UINT8              IsStreamEndFlag;
    SINT32             ChanID;
    SINT32             FspPartitionOK;
    SYNTAX_EXTRA_DATA_S *pstExtraData;
    AVS_DEC_PARAM_S    *pAvsDecParam;
    AVS_DEC_PARAM_S    AvsDecParam;
    AVS_CODECINF       CodecInfo;
    BS                 Bs;
    AVS_SYNTAX         SyntaxNew;
    IMAGE_VO_QUEUE     ImageQue;
    SINT32             IsPackWaitDec;
    DEC_STREAM_PACKET_S LastPacket;
    AVS_VDMCFG         AvsCfg;

    UINT32             FakeFrmHeight;
    UINT32             FakeFrmWidth;  
} AVS_CTX_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 AVSDEC_Init(AVS_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
SINT32 AVSDEC_Destroy(AVS_CTX_S *pCtx);
SINT32 AVSDEC_Decode(AVS_CTX_S *pCtx, DEC_STREAM_PACKET_S *p_Packet);
SINT32 AVSDEC_RecycleImage(AVS_CTX_S *pCtx, UINT32 img_id);
SINT32 AVSDEC_GetRemainImg(AVS_CTX_S *pCtx);
SINT32 AVSDEC_VDMPostProc(AVS_CTX_S *pCtx, SINT32 ErrRatio);
SINT32 AVSDEC_GetImageBuffer( AVS_CTX_S *pCtx );
SINT32 AVS_GetPacketState(AVS_CTX_S *pCtx);
SINT32 AVS_ClearCurPic(AVS_CTX_S *pCtx);


extern STREAM_PARAM g_Packet;

#ifdef __cplusplus
}
#endif


#endif //__AVS_H__
