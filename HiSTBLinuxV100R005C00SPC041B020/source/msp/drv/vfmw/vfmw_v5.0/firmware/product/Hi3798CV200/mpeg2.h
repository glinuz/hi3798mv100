#ifndef __MPEG2_H__
#define __MPEG2_H__


//#include "basedef.h"
#include "public.h"
#include "bitstream.h"
#include "syn_cmn.h"
#include "vdm_hal.h"


#ifdef MPEG2_ENABLE

//协议头
#define PICTURE_START_CODE                         0x100
#define SLICE_START_CODE_MIN                       0x101
#define SLICE_START_CODE_MAX                       0x1AF
#define USER_DATA_START_CODE                       0x1B2
#define YXSB_WATERMARK_CODE                        0x1B1  /* 永新世博数字水印的私有数据，处理方式等同于user data */
#define SEQUENCE_HEADER_CODE                       0x1B3
#define SEQUENCE_ERROR_CODE                        0x1B4
#define EXTENSION_START_CODE                       0x1B5
#define SEQUENCE_END_CODE                          0x1B7
#define GROUP_START_CODE                           0x1B8
#define SYSTEM_START_CODE_MIN                      0x1B9
#define SYSTEM_START_CODE_MAX                      0x1FF

#define ISO_END_CODE                               0x1B9
#define PACK_START_CODE                            0x1BA
#define SYSTEM_START_CODE                          0x1BB

#define VIDEO_ELEMENTARY_STREAM                    0x1E0


#define PROFILE_422                               (128+5)
#define MAIN_LEVEL                                 8

#define MPEG2_BLOCK_SIZE                          (8*8)

/* picture coding type */
#define I_TYPE                                     1
#define P_TYPE                                     2
#define B_TYPE                                     3
#define D_TYPE                                     4

/* picture structure */
#define TOP_FIELD                                  1
#define BOTTOM_FIELD                               2
#define FRAME_PICTURE                              3

/* extension start code IDs */
#define SEQUENCE_EXTENSION_ID                      1
#define SEQUENCE_DISPLAY_EXTENSION_ID              2
#define QUANT_MATRIX_EXTENSION_ID                  3
#define COPYRIGHT_EXTENSION_ID                     4
#define SEQUENCE_SCALABLE_EXTENSION_ID             5
#define PICTURE_DISPLAY_EXTENSION_ID               7
#define PICTURE_CODING_EXTENSION_ID                8
#define PICTURE_SPATIAL_SCALABLE_EXTENSION_ID      9
#define PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID     10

//VFMW信息
#define NOTENOUGH_STREAM                          -1
#define PARSE_OVER                                 0
#define FOUND_SLCHDR                               1


//define  vdm state
#define DEC_OVER                                   1
#define DEC_ERROR                                  2
#define DEC_WAIT_STREAM                            4


#define MPEG2_ERROR                                (-1)
#define MPEG2_FALSE                                0
#define MPEG2_TRUE                                 1
#define MPEG2_NOTDEC                               2

#define STATE_GET_PICHDR                           0
#define STATE_GET_SLCHDR                           1

#define QUEUE_IMAGE_NUM                            20
#define MINIMUM_IMAGE_NUM                        4//6//modify by tbb from value=4


#define MPEG2_ERROR_SEQHDR                         (1<<0)
#define MPEG2_ERROR_SEQEXT                         (1<<1)
#define MPEG2_ERROR_SEQDISPEXT                     (1<<2)
#define MPEG2_ERROR_PICHDR                         (1<<3)

#define MPEG2_ERROR_PICEXT                         (1<<4)
#define MPEG2_ERROR_PICDISPEXT                     (1<<5)
#define MPEG2_ERROR_REFHDR                         (1<<6)
#define MPEG2_ERROR_REFPIC                         (1<<7)

#define MPEG2_FOUND_SEQHDR                         (1<<16)
#define MPEG2_FOUND_SEQEXT                         (1<<17)
#define MPEG2_FOUND_SEQDISPEXT                     (1<<18)
#define MPEG2_FOUND_PICHDR                         (1<<19)
#define MPEG2_FOUND_PICEXT                         (1<<20)
#define MPEG2_FOUND_PICDISPEXT                     (1<<21)
#define MPEG2_FOUND_SLICE                          (1<<22)
#define MPEG2_FOUND_NEXTHDR                        (1<<23)

#define MPEG2_ALL_EXTENTION                        (MPEG2_FOUND_SEQEXT|MPEG2_FOUND_SEQDISPEXT|MPEG2_FOUND_PICEXT|MPEG2_FOUND_PICDISPEXT)
#define MPEG2_ALLSEQ_ERROR                         (MPEG2_ERROR_SEQHDR|MPEG2_ERROR_SEQEXT|MPEG2_ERROR_SEQDISPEXT|MPEG2_ERROR_PICHDR|MPEG2_ERROR_PICEXT|MPEG2_ERROR_PICDISPEXT)
#define MPEG2_ALLSEQHDR_ERROR                      (MPEG2_ERROR_SEQHDR|MPEG2_ERROR_SEQEXT|MPEG2_ERROR_SEQDISPEXT)
#define MPEG2_ALLPIC_FLAG                          (MPEG2_ERROR_PICHDR|MPEG2_FOUND_PICEXT|MPEG2_FOUND_PICDISPEXT|MPEG2_ERROR_PICHDR|MPEG2_ERROR_PICEXT|MPEG2_ERROR_PICDISPEXT)

#define PROTOCOL_UNKNOWN                           0
#define PROTOCOL_MPEG1                             1
#define PROTOCOL_MPEG2                             2

#define MAXBIT_BEFORE_EXT                          2048

#define MP2UD_FROM_UNKOWN                          0
#define MP2UD_FROM_SEQ                             1
#define MP2UD_FROM_PIC                             3


typedef struct
{
    UINT8  buf_state; //0:available to VDM write; 1: available to VO read
    UINT8  ref_flag;
    UINT32 err_level;
    SINT32 vo_fs_id_plus1; /* 如果显示图另外输出(比如做后处理)，则需要额外指定输出图 */
    SINT32 tf_fs_id_plus1; /* 如果TF使能，则需要保存滤波后的图象为后续TF提供参考 */
    IMAGE  imginf;
} MP2VOBUFINF;

/* BEGIN: Added by y62639, 2010/8/19 */
typedef struct
{
    SINT32 GopFrmCntNoRpt;
    SINT32 GopFldCnt;
    SINT32 GopFrmCnt;
} MP2_FRMRATE_STAT_INF_S;
/* END:   Added by y62639, 2010/8/19 */

typedef struct
{
    UINT8  valid_flag;
    UINT8  constrained_parameters_flag;
    UINT8  drop_flag;
    UINT8  composite_display_flag;

    /* ISO/IEC 13818-2 section 6.2.3.1: picture_coding_extension() header */
    UINT8  f_code[2][2];
    UINT8  intra_dc_precision;
    UINT8  picture_structure;
    UINT8  top_field_first;
    UINT8  frame_pred_frame_dct;
    UINT8  concealment_motion_vectors;
    UINT8  q_scale_type;

    UINT8  intra_vlc_format;
    UINT8  alternate_scan;

    UINT8  repeat_first_field;

    UINT8  chroma_420_type;
    UINT8  progressive_frame;
    UINT8  v_axis;
    UINT8  field_sequence;
    UINT8  sub_carrier;
    UINT8  picture_coding_type;
    UINT8  full_pel_forward_vector;
    UINT8  forward_f_code;
    UINT8  full_pel_backward_vector;
    UINT8  backward_f_code;
    UINT8  progressive_sequence;
    UINT8  chroma_format;
    UINT8  low_delay;
    UINT8  frame_rate_extension_n;
    UINT8  frame_rate_extension_d;

    UINT32 burst_amplitude;
    UINT32 sub_carrier_phase;

    UINT32 profile, level;
    // normative derived variables (as per ISO/IEC 13818-2)
    UINT32 horizontal_size;
    UINT32 vertical_size;
    UINT32 mb_width;
    UINT32 mb_height;
    UINT32 mb_height_progressive_subtract1mb;
    UINT32 bit_rate;
    UINT32 frame_rate;

    UINT32 load_intra_quantizer_matrix;
    UINT32 load_non_intra_quantizer_matrix;
    /* headers */

    /* ISO/IEC 13818-2 section 6.2.2.1:  sequence_header() */
    UINT32 aspect_ratio_information;
    UINT32 frame_rate_code;
    UINT32 bit_rate_value;
    UINT32 vbv_buffer_size;

    /* ISO/IEC 13818-2 section 6.2.2.6: group_of_pictures_header()  */
    UINT32 hour;
    UINT32 minute;
    UINT32 sec;
    UINT32 frame;
    UINT32 closed_gop;
    UINT32 broken_link;

    /* ISO/IEC 13818-2 section 6.2.2.3:  sequence_extension() */
    UINT32 profile_and_level_indication;

    /* ISO/IEC 13818-2 section 6.2.2.4:  sequence_display_extension() */
    UINT32 video_format;
    UINT32 color_description;
    UINT32 color_primaries;
    UINT32 transfer_characteristics;
    UINT32 matrix_coefficients;
    UINT32 display_horizontal_size;
    UINT32 display_vertical_size;

    /* ISO/IEC 13818-2 section 6.2.3: picture_header() */
    UINT32 temporal_reference;
    UINT32 vbv_delay;

    /* ISO/IEC 13818-2 section 6.2.3.3: picture_display_extension() header */
    UINT32 frame_center_horizontal_offset[3];
    UINT32 frame_center_vertical_offset[3];

} MP2SYNTAXINF;


/* mpeg2 slice info */
typedef struct
{

    /*picture header0--sequence*/
    UINT32  mpeg1_flag;
    UINT32  pic_height_in_mb;
    UINT32  pic_width_in_mb;
    UINT32  pic_total_mb_num;


    /*slice header*/

    UINT32  FirstMbInSliceFlag; //用于是否为skip 宏块的判断，
    UINT32  slice_vertical_position;    //slice start code 后面的8bit, 01~AF
    UINT32  slice_vertical_position_extension; //扩展
    UINT32  previous_macroblock_address;   //记录上一个宏块绝对地址,对于slice的第一个宏块来说，表示上一个slice所在行的最后一个宏块地址
    UINT32  intra_slice;

    /*bitstream*/
    UINT32  ByteLen;

    /*up message*/
    UINT32  slicen_start_mb_num;
    SINT32  quantiser_scale_code;

    UINT8   *VirAddr;
    BS               *pBs;

    //    struct MP2_SLCINFO_S *pNextSliceInfo;
} MP2_SLCINFO_S;

typedef struct
{
    SINT8  IsLastSeg[2];
    UADDR  BsPhyAddr[2];        //current bs physical start addr
    SINT32 BsLength[2];
    SINT32 StreamID[2];
    UINT8 *BsVirAddr[2];        //current bs virtual start addr
} MP2_STREAM_PACKET;

typedef struct
{
    UINT8  LastDecPicCodingType;
    UINT8  Last_FieldStruct;

    MP2VOBUFINF   VOBufInf[MAX_QUEUE_IMAGE_NUM];       //VO帧存管理信息

    //MPEG2解码器全局变量
    SINT32 BsOffset;         //byte offset from packet stream pointer
    //    UINT32 BsPhyAddr;        //current bs physical start addr
    //    UINT8 *BsVirAddr;        //current bs virtual start addr
    //    SINT32 BsLength;
    //    SINT32 IsLastSeg;

    //  UINT32 BsPhyAddr1;       //current bs physical start addr
    //  UINT8 *BsVirAddr1;       //current bs virtual start addr
    //  SINT32 BsLength1;
    //  SINT32 IsLastSeg1;
    MP2_STREAM_PACKET stMp2Packet;

    UINT32 UsedDecMode;      //当前正在使用的解码模式(IPB, IP, I)
    SINT32 PacketTail;
    UINT32 NewSeqInited;
    UINT32 StreamIntegrality;//码流中各种头的完整性和正确性信息
    UINT32 SeqCnt;
    SINT32 Frame_Cnt;
    UINT32 Second_Field;
    VDEC_USRDAT_S *pUsrDatArray[MAX_USERDATA_NUM];
    SINT32 TotalUsrDatNum;
    //    VDEC_USRDAT_S *p_SeqUD;
    //    VDEC_USRDAT_S *p_PicUD;
    UINT32 MpegFlag;
    UINT32 RefNum;
    UINT32 LastTempReference;
    UINT32 Imgbuf_allocated;
    SINT32 Used_byte;        //until last packet
    UINT32 HDRState;         //search pic or slc
    SINT32 HDRUsedBits;
    SINT32 QueueImageNum;
    UADDR  DnMsgAddr;
    UADDR  UpMsgAddr;

    SINT32 FwdFsID;
    SINT32 BwdFsID;
    SINT32 CurFsID;
    SINT32 ToQueFsID;
    SINT32 TfFwdFsID;
    SINT32 TfBwdFsID;
    SINT32 TfCurFsID;

    UADDR  pmv_colmb_buf_phyaddr;
    UINT32 VahbStride;

    UINT32 AdjustQpDb;
    UINT32 AdjustQpDr;
    /* BEGIN: Added by y62639, 2010/8/19 */
    MP2_FRMRATE_STAT_INF_S GopHist[4];
    SINT32 RealFrmRate;
    SINT32 InstBpp1000;
    SINT32 AvgBpp1000;
    /* END:   Added by y62639, 2010/8/19 */
    UINT8 EstmatedPicQp;   // last frame

    UINT32 LastStreamIntegrality; //l00273086

} MP2CODECINF;


typedef struct
{
    UINT8    *VirAddr;    // 码流的虚拟地址
    UADDR     PhyAddr;    // 码流的物理地址
    SINT32    Length1;    // 第一部分码流长度（字节数）
    SINT32    Length2;    // 第二部分码流长度（字节数）
} MP2STREAM_PARAM;


typedef struct
{
    SYNTAX_EXTRA_DATA_S  *pstExtraData;
    MP2_DEC_PARAM_S *pMp2DecParam;
    MP2_DEC_PARAM_S Mp2DecParam;
    MP2CODECINF   CodecInfo;
    MP2_SLCINFO_S SliceInfo;
    BS            Bs;

    /* intra_quantizer_matrix, non_intra_quantizer_matrix以前是全局变量，多通道存在互相干扰，所以放在通道上下文里: */
    UINT32 intra_quantizer_matrix[MPEG2_BLOCK_SIZE];
    UINT32 non_intra_quantizer_matrix[MPEG2_BLOCK_SIZE];
    DEC_STREAM_PACKET_S stCurPacket;
    MP2SYNTAXINF  SyntaxInf[2];
    MP2SYNTAXINF  *pSyntaxInfNew;
    MP2SYNTAXINF  *pSyntaxInfOld;
    IMAGE_VO_QUEUE  ImageQue;
    SINT32        ChanID;
    SINT32        numTotalFrame;
    SINT32        numOutputFrame;
    UINT32          PPicFound;
    UINT8         IsWaitToDecFlag;
    UINT8         IsWaitToReRange;
    UINT8         IsStreamEndFlag;

    UINT32        FakeFrmWidthInMb;    //add by l00228308 for fake frame
    UINT32        FakeFrmHeightInMb;    
} MP2_CTX_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 MPEG2DEC_Init(MP2_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
SINT32 MPEG2DEC_Destroy(MP2_CTX_S *pCtx);
SINT32 MPEG2DEC_Decode(MP2_CTX_S *pCtx, DEC_STREAM_PACKET_S *pPacket);
SINT32 MPEG2DEC_RecycleImage(MP2_CTX_S *pCtx, UINT32 img_id);
SINT32 MPEG2DEC_GetRemainImg(MP2_CTX_S *pCtx);
UINT32 MPEG2DEC_VDMPostProc(MP2_CTX_S *pCtx, SINT32 ErrRatio);
SINT32 MPEG2DEC_GetImageBuffer( MP2_CTX_S *pCtx );
SINT32 MP2_GetPacketState(MP2_CTX_S *pCtx);
VOID MP2_ClearCurPacket(SINT32 ChanID, MP2_STREAM_PACKET *pMp2Packet);
VOID MP2_ClearCurPic(MP2_CTX_S *pCtx);


#ifdef __cplusplus
}
#endif


#endif //MPEG2_ENABLE

#endif //__MPEG2_H__
