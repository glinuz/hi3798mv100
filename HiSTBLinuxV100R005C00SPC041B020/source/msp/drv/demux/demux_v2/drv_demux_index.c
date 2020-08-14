
/*!*********************************************************************
*
* Copyright (C), 2006 Hisilicon Tech. Co., Ltd.
*
* @file  pvr_index.c
* @Date  2009/4/3
* @Revision v1.0
* @brief make video stream index info for trick play.
*          support multi video standards
*
* Change History:
*
* Date             Author            Change
* ====             ======            ======
* 2009/4/3         z56361            Original
*
* Dependencies:
*
************************************************************************/

#include <linux/kernel.h>
#include <linux/string.h>

#include "hi_drv_mem.h"
#include "demux_debug.h"
#include "drv_demux_config.h"
#include "drv_demux_bitstream.h"
#include "drv_demux_index.h"

/*!@attention :define the following macro if you want PTS be set to I frames only */
//#define  PUT_PTS_ON_I_FRAME_ONLY

/*!=======================================================================
                              constants
  =======================================================================*/
#define  FIDX_MAX_CTX_NUM          DMX_REC_CNT /* maximum channel can be processed are the same as record channel */
#define  SC_SUPPORT_DATA_SIZE      16          /*! SC support data size, 16 bytes */

/*! nal unit type */
#define  FIDX_NALU_TYPE_SLICE  1
#define  FIDX_NALU_TYPE_IDR    5
//#define  FIDX_NALU_TYPE_SEI    6
#define  FIDX_NALU_TYPE_SPS    7
#define  FIDX_NALU_TYPE_PPS    8

//#ifdef HEVC_ENABLE
#define FIDX_HEVC_NAL_SLICE_TRAIL_N                      0      // 0
#define FIDX_HEVC_NAL_SLICE_TRAIL_R                      1      // 1
#define FIDX_HEVC_NAL_SLICE_TSA_N                         2       // 2
#define FIDX_HEVC_NAL_SLICE_TLA_R                         3       // 3
#define FIDX_HEVC_NAL_SLICE_STSA_N                       4       // 4
#define FIDX_HEVC_NAL_SLICE_STSA_R                       5       // 5
#define FIDX_HEVC_NAL_SLICE_RADL_N                       6       // 6
#define FIDX_HEVC_NAL_SLICE_RADL_R                       7       // 7
#define FIDX_HEVC_NAL_SLICE_RASL_N                        8       // 8
#define FIDX_HEVC_NAL_SLICE_RASL_R                        9       // 9

#define FIDX_HEVC_NAL_SLICE_BLA_W_LP                   16    // 16
#define FIDX_HEVC_NAL_SLICE_BLA_W_RADL              17    // 17
#define FIDX_HEVC_NAL_SLICE_BLA_N_LP                    18    // 18
#define FIDX_HEVC_NAL_SLICE_IDR_W_RADL               19    // 19
#define FIDX_HEVC_NAL_SLICE_IDR_N_LP                     20    // 20
#define FIDX_HEVC_NAL_SLICE_CRA                              21    // 21
#define FIDX_HEVC_NAL_RESERVED_IRAP_VCL22        22    //22
#define FIDX_HEVC_NAL_RESERVED_IRAP_VCL23        23    //23

#define FIDX_HEVC_NAL_VPS               32
#define FIDX_HEVC_NAL_SPS               33
#define FIDX_HEVC_NAL_PPS               34
#define FIDX_HEVC_NAL_PRE_SEI        39
#define FIDX_HEVC_NAL_SUF_SEI        40

#define FIDX_HEVC_I_SLICE                   2
#define FIDX_HEVC_P_SLICE                 1
#define FIDX_HEVC_B_SLICE                 0
//#endif


/*! slice type */
//#define  FIDX_SLICE_TYPE_P     0
//#define  FIDX_SLICE_TYPE_B     1
//#define  FIDX_SLICE_TYPE_I     2
//#define  FIDX_SLICE_TYPE_PP    5
//#define  FIDX_SLICE_TYPE_BB    6
//#define  FIDX_SLICE_TYPE_II    7


/*!=======================================================================
                               data structures
  =======================================================================*/
/*! start code type */
typedef enum hi_SC_TYPE_E
{
    SC_TYPE_UNKNOWN = 0,
    SC_TYPE_SPS,
    SC_TYPE_PPS,
    SC_TYPE_PIC,
    SC_TYPE_SLICE,
    SC_TYPE_NONSLICE,
    SC_TYPE_VPS,
    SC_TYPE_BUTT
} SC_TYPE_E;

/*! start code description */
typedef struct hi_SC_INFO_S
{
    SC_TYPE_E     eScType;
    HI_S32        s32ScID;           /*! for H.264, assign PPS or SPS ID; for non-H.264, assign the byte after 00 00 01 */
    HI_S32        s32SupScID;        /*! for H.264 only, record the SPS ID for the current used PPS */
    HI_S64        s64GlobalOffset;   /*! the offset of the start code, in the global(whole) stream data space */
    HI_S32        s32OffsetInPacket; /*! the offset of the start code, in the stream data packet */
    HI_S32        s32PacketCount;    /*! the stream data packet number where the start code was found */
    HI_S32        s32ProfileId;
    
    //#ifdef HEVC_ENABLE
    //VPS
    //SPS
    HI_S32      max_cu_depth;
    HI_S32      max_cu_width;
    HI_S32      max_cu_height;
    HI_S32      pic_width_in_luma_samples;
    HI_S32      pic_height_in_luma_samples;
    //HI_S32      sps_max_sub_layers_minus1;
    //PPS
    HI_S32      seq_parameter_set_id;
    HI_S32      dependent_slice_segments_enabled_flag;
    HI_S32      num_extra_slice_header_bits;
    //#endif
    
} SC_INFO_S;

/*! state of the instance */
typedef enum hi_CTX_STATE_E
{
    CTX_STATE_DISABLE = 0,
    CTX_STATE_ENABLE,
    CTX_STATE_BUTT
} CTX_STATE_E;


/*! context */
typedef struct hi_FIDX_CTX_S
{
    CTX_STATE_E   eCtxState;
    VIDSTD_E      eVideoStandard;  /*! video standard type */
    STRM_TYPE_E   eStrmType;       /*! stream type, ES or PES */
    HI_U32        u32PTS;          /*! current PTS, usually equals to the pts of the latest stream packet */
    SC_INFO_S     stSPS[32];       /*! start code of the sequence level parameters.
                                          H264  - sps
                                          MPEG2 - sequence header
                                          AVS   - sequence header
                                          MPEG4 - VOL or higher  */
    HI_U8         u8SpsFresh[32];  /*! one SPS can be used by one I frame only, an I frame without SPS will be treated as P or B frame */
    SC_INFO_S     stPPS[256];      /*! picture level parameter
                                          H264  - pps
                                          MPEG2 - picture header
                                          AVS   - picture header
                                          MPEG4 - VOP header  */

    //previous 2 bytes, for start code detection, to prevent the 00 or 00 00 lost
    HI_U8         u8Prev2Bytes[2]; /*! store the latest 2 byte  */

    //this SC store the latest tetected start code
    HI_S32        s32ThisSCValid;  /*! indicate the support data of this start code is ready for use */
    SC_INFO_S     stThisSC;                           /*! when a start code was found, the support data is probably not enough. If so, this start */
    HI_U8         u8ThisSCData[SC_SUPPORT_DATA_SIZE]; /*! has to be stored temporarily to wait more data */
    HI_S32        s32ThisSCDataLen;  /*! actual support data size, usually equals to SC_SUPPORT_DATA_SIZE */

    SC_INFO_S     stSEIFollowSlice; /*! record the SEI start code followed the last slice of the previous picture. Generally this SEI is the start of a new picture */

    //frame,a set of frame info probably can be generated after 'this SC' was processed
    FRAME_POS_S   stNewFramePos;     /*! frame info to be output, temporarily stored here */
    HI_S32        s32WaitFrameSize;  /*! indicate if most info of stNewFramePos has been ready, except the frame size */
    HI_S32        s32SpsID;          /*! H264: SPS ID */
    HI_S32        s32PpsID;          /*! H264: PPS ID */
    HI_U32       *Param;
    
    /* HEVC_ENABLE */
    HI_U32        u32NextPTS;
    SC_INFO_S     stVPS[16]; 
    HI_S32        s32VpsSpsPpsErr;
    HI_S32        s32IsRefIdc;

    HI_S64      s64NewFrmOffset;
    HI_S64      s64LastVpsOffset;
    HI_S64      s64LastSpsOffset;
    HI_S64      s64LastPpsOffset;
    HI_S64      s64LastSeiOffset;
    HI_S64      s64FirstNalOffset;

    HEVC_CTX_S  *pstHevcCtx;
    HI_U8       *pu8HevcSCData;   /* hevc special buffer, refer to u8ThisSCData. */
}FIDX_CTX_S;


/*!=======================================================================
                           static shared data
  =======================================================================*/
static FIDX_CTX_S  *s_FidxIIS = HI_NULL;


/*! callback, used to output infomation */
static HI_VOID (*s_OutputFramePosition)(HI_U32 *Param, FRAME_POS_S *pstScInfo);

/*=======================================================================
                           function declaration
  =======================================================================*/
static HI_S32 ProcessThisSC(HI_S32 InstIdx);
static HI_S32 ProcessSC_Mpeg2(HI_S32 InstIdx);
static HI_S32 ProcessSC_Avs(HI_S32 InstIdx);
static HI_S32 ProcessSC_Mpeg4(HI_S32 InstIdx);
static HI_S32 ProcessSC_H264(HI_S32 InstIdx);
//#ifdef HEVC_ENABLE
static HI_S32 ProcessSC_HEVC(HI_S32 InstIdx);
//#endif

/*!=======================================================================
                             macros
  =======================================================================*/
/*! print */
#define PRINT   HI_ERR_DEMUX

/*! assertion */
#define  FIDX_ASSERT_RET( cond, else_print )                            \
do {                                                                    \
    if( !(cond) )                                                       \
    {                                                                   \
        PRINT("pvr_index.c,L%d: %s\n", __LINE__, else_print );          \
        return FIDX_ERR;                                                \
    }                                                                   \
}while(0)
#define  FIDX_ASSERT( cond, else_print )                                \
do {                                                                    \
    if( !(cond) )                                                       \
    {                                                                   \
        PRINT("pvr_index.c,L%d: %s\n", __LINE__, else_print );          \
        return;                                                         \
    }                                                                   \
}while(0)

/*! fill support data of the SC */
#define FILL_SC_DATA(ptr,len)                                           \
do {                                                                    \
    if( pstCtx->s32ThisSCDataLen + (len) >= SC_SUPPORT_DATA_SIZE )      \
    {                                                                   \
        memcpy(pstCtx->u8ThisSCData + pstCtx->s32ThisSCDataLen, ((const void *)ptr),    \
            (size_t)(SC_SUPPORT_DATA_SIZE - pstCtx->s32ThisSCDataLen));         \
        pstCtx->s32ThisSCDataLen = SC_SUPPORT_DATA_SIZE;                \
    }                                                                   \
    else                                                                \
    {                                                                   \
        memcpy(pstCtx->u8ThisSCData + pstCtx->s32ThisSCDataLen, ((const void *)ptr),    \
            ((size_t)len));                                                     \
        pstCtx->s32ThisSCDataLen += (len);                              \
    }                                                                   \
} while(0)

/*! analyze a prepared  start code */
#define ANANYSE_SC()                                                    \
do {                                                                    \
    ProcessThisSC((HI_S32)InstIdx);                                             \
    pstCtx->s32ThisSCValid = 0;                                         \
    pstCtx->s32ThisSCDataLen = 0;                                       \
} while(0)

/*! output prepared frame position infomation */
#define OUTPUT_FRAME()                                                  \
do{                                                                     \
    if( NULL != s_OutputFramePosition )                                 \
    {                                                                   \
        (HI_VOID)s_OutputFramePosition(pstCtx->Param, &pstCtx->stNewFramePos );       \
    }                                                                   \
    memset(&pstCtx->stNewFramePos, 0, sizeof(FRAME_POS_S));             \
    pstCtx->stNewFramePos.eFrameType = FIDX_FRAME_TYPE_UNKNOWN;         \
} while(0)

/*! decide if the SC is valid */
#define IS_SC_WRONG()   \
( pstCtx->s32ThisSCDataLen < 3 ||   \
(pstCtx->u8ThisSCData[0] == 0x00 && pstCtx->u8ThisSCData[1]==0x00 && pstCtx->u8ThisSCData[2]==0x01) )

/*! mpeg4 have multi-level sequence parameter, select the highest level */
#define SELECT_SPS( sps, pSelSps )                                      \
do{                                                                     \
    if( SC_TYPE_SPS == (sps).eScType &&                                 \
        (sps).s64GlobalOffset < (pSelSps)->s64GlobalOffset &&           \
        (sps).s64GlobalOffset + 64 >= (pSelSps)->s64GlobalOffset )      \
    {                                                                   \
        (pSelSps) = &(sps);                                             \
    }                                                                   \
} while (0)

#define BITS_2_BYTES(bitn)   (((bitn) + (7))>>(3))

/*!=======================================================================
                       function implementation
  =======================================================================*/

/*!***********************************************************************
    @brief global init, clear context, and register call back
 ************************************************************************/
HI_VOID FIDX_Init(HI_VOID (*OutputFramePosition)(HI_U32 *Param, FRAME_POS_S *pstScInfo))
{
    HI_S32 i;

    s_OutputFramePosition = OutputFramePosition;

    s_FidxIIS = HI_KMALLOC(HI_ID_DEMUX, FIDX_MAX_CTX_NUM * sizeof(FIDX_CTX_S), GFP_KERNEL);
    if (!s_FidxIIS)
    {
        HI_ERR_DEMUX("malloc fidx ctx failed.\n");
        BUG();
    }
    else
    {  
        for( i = 0; i < FIDX_MAX_CTX_NUM; i++ )
        {
            s_FidxIIS[i].eCtxState = CTX_STATE_DISABLE;
        }
    }
}

HI_VOID FIDX_DeInit(HI_VOID)
{
    if (s_FidxIIS)
    {
        HI_KFREE(HI_ID_DEMUX, s_FidxIIS);
        s_FidxIIS = HI_NULL;
    }
}

/*!***********************************************************************
    @brief  open an instance
    @param[in]  VidStandard: video standard
    @return
        if success, return instance ID, 0~(FIDX_MAX_CTX_NUM-1)
        if fail, return -1
 ************************************************************************/
HI_S32  FIDX_OpenInstance(VIDSTD_E VidStandard, STRM_TYPE_E StrmType, HI_U32 *Param)
{
    HI_S32  ret = -1;
    HI_S32  i;

    FIDX_ASSERT_RET(VidStandard<VIDSTD_BUTT, "'VidStandard' out of range");
    FIDX_ASSERT_RET(StrmType<STRM_TYPE_BUTT, "'StrmType' out of range");

    /*! find an idle instance */
    for (i = 0; i < FIDX_MAX_CTX_NUM; i++)
    {
        if (s_FidxIIS[i].eCtxState != CTX_STATE_ENABLE)
        {
            memset(&s_FidxIIS[i], 0, sizeof(FIDX_CTX_S));

            s_FidxIIS[i].eCtxState          = CTX_STATE_ENABLE;
            s_FidxIIS[i].eVideoStandard     = VidStandard;
            s_FidxIIS[i].eStrmType          = StrmType;
            s_FidxIIS[i].u8Prev2Bytes[0]    = 0xff;
            s_FidxIIS[i].u8Prev2Bytes[1]    = 0xff;
            s_FidxIIS[i].Param              = Param;

            s_FidxIIS[i].s32VpsSpsPpsErr = 0;
            s_FidxIIS[i].s64NewFrmOffset = -1;
            s_FidxIIS[i].s64FirstNalOffset = -1;
            s_FidxIIS[i].s64LastVpsOffset = -1;
            s_FidxIIS[i].s64LastSpsOffset = -1;
            s_FidxIIS[i].s64LastPpsOffset = -1;
            s_FidxIIS[i].s64LastSeiOffset = -1;
            s_FidxIIS[i].pstHevcCtx = HI_NULL;
            
            if (VIDSTD_HEVC == VidStandard)
            {
                s_FidxIIS[i].pstHevcCtx = HI_VMALLOC(HI_ID_DEMUX, sizeof(HEVC_CTX_S));
                if (!s_FidxIIS[i].pstHevcCtx)
                {
                    HI_ERR_DEMUX("malloc hevc ctx failed.\n");
                    break;
                }
                
                HEVC_Init(s_FidxIIS[i].pstHevcCtx);
            }
        
            ret = i;

            break;
        }
    }

    return ret;
}

/*!***********************************************************************
    @brief   close instalce
 ************************************************************************/
HI_S32  FIDX_CloseInstance( HI_S32 InstIdx )
{
    FIDX_ASSERT_RET( InstIdx < FIDX_MAX_CTX_NUM, "InstIdx out of range");
    
    if( s_FidxIIS[InstIdx].eCtxState != CTX_STATE_ENABLE )
    {
        return FIDX_ERR;
    }
    else
    {
        if (s_FidxIIS[InstIdx].pstHevcCtx)
        {
            HI_VFREE(HI_ID_DEMUX, s_FidxIIS[InstIdx].pstHevcCtx);
        }
        
        memset( &s_FidxIIS[InstIdx], 0, sizeof(FIDX_CTX_S) );
        s_FidxIIS[InstIdx].eCtxState = CTX_STATE_DISABLE;
        
        return FIDX_OK;
    }
}

static HI_S32 IsPesSC(HI_U8 Code, VIDSTD_E eVidStd )
{
    HI_S32 ret = 0;

    if( eVidStd != VIDSTD_AUDIO_PES )
    {
        if( Code >= 0xe0 && Code <= 0xef )
        {
            ret = 1;
        }
    }
    else
    {
        if( Code >= 0xc0 && Code <= 0xdf )
        {
            ret = 1;
        }
    }

    return ret;
}

/*!***********************************************************************
@brief  entry of the start code process
@return
  if success return FIDX_OK, otherwise return FIDX_ERR
************************************************************************/
HI_S32 ProcessThisSC(HI_S32 InstIdx)
{
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];
    HI_S32 s32Ret = FIDX_ERR;

    switch( pstCtx->eVideoStandard )
    {
        case VIDSTD_MPEG2:
            s32Ret = ProcessSC_Mpeg2(InstIdx);
            break;
        case VIDSTD_H264:
            s32Ret = ProcessSC_H264(InstIdx);
            break;
        case VIDSTD_MPEG4:
            s32Ret = ProcessSC_Mpeg4(InstIdx);
            break;
        case VIDSTD_AVS:
            s32Ret = ProcessSC_Avs(InstIdx);
            break;
//#ifdef HEVC_ENABLE
        case VIDSTD_HEVC:
            s32Ret = ProcessSC_HEVC(InstIdx);
            break;
//#endif
        default:
            s32Ret = FIDX_ERR;
            break;
    }

    return s32Ret;
}


/*!***********************************************************************
@brief
    process mpeg2 start code
@return
  if success return FIDX_OK, otherwise return FIDX_ERR
************************************************************************/
HI_S32 ProcessSC_Mpeg2(HI_S32 InstIdx)
{
    HI_U8 Code;
    HI_S32 s32Ret = FIDX_ERR;
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];
    SC_INFO_S   *pstSpsSC = &pstCtx->stSPS[0];
    SC_INFO_S   *pstThisSC = &pstCtx->stThisSC;
    FRAME_POS_S *pstFrmPos = &pstCtx->stNewFramePos;

    FIDX_ASSERT_RET( 0!=pstCtx->s32ThisSCValid, "ThisSC is not valid\n");
    FIDX_ASSERT_RET( !IS_SC_WRONG(), "not enough data for ThisSC\n");

    Code = pstCtx->u8ThisSCData[0];

    /*! if a frame is found, but its size have not been calculated, it is time to calc the frame size, and output this frame */
    if( pstFrmPos->eFrameType != FIDX_FRAME_TYPE_UNKNOWN )
    {
        if( 0xb3 == Code || 0x00 == Code )  /*! this means end of the formal frame found, the frame size can be calculated */
        {
            pstFrmPos->s32FrameSize = (HI_S32)(pstThisSC->s64GlobalOffset - pstFrmPos->s64GlobalOffset);
            if( pstFrmPos->s32FrameSize < 0 )
            {
                /*! just incase */
                pstFrmPos->eFrameType = FIDX_FRAME_TYPE_UNKNOWN;
            }
            else
            {
                OUTPUT_FRAME();
            }
        }
    }

    if( 0xb3 == Code )  /*! sequence header */
    {
        pstThisSC->s32ScID = (HI_S32)Code;
        pstThisSC->eScType = SC_TYPE_SPS;
        memcpy( pstSpsSC, pstThisSC, sizeof(SC_INFO_S) );
        pstCtx->u8SpsFresh[0] = 1;
        s32Ret = FIDX_ERR;
    }
    else if (0x00 == Code)  /*! picture header */
    {
        if( pstSpsSC->eScType != SC_TYPE_SPS ) /*! seqence parameters still not ready */
        {
            s32Ret = FIDX_ERR;
        }
        else
        {
            HI_U8 picture_coding_type;

            picture_coding_type = (pstCtx->u8ThisSCData[2] >> 3) & 7;
            //FIDX_ASSERT_RET(picture_coding_type>=1 && picture_coding_type<=3,
            //  "MPEG2 picture_coding_type out of range");
            if((picture_coding_type<1) || (picture_coding_type>3))
            {
                return FIDX_ERR;
            }
            if( 1 == picture_coding_type && 1 == pstCtx->u8SpsFresh[0])  /*! I frame */
            {
                pstFrmPos->eFrameType        = FIDX_FRAME_TYPE_I;
                pstFrmPos->s64GlobalOffset   = pstSpsSC->s64GlobalOffset;
                pstFrmPos->s32OffsetInPacket = pstSpsSC->s32OffsetInPacket;
                pstFrmPos->s32PacketCount    = pstSpsSC->s32PacketCount;
                pstCtx->u8SpsFresh[0] = 0;
                pstFrmPos->u32PTS            = pstCtx->u32PTS;
                pstCtx->u32PTS               = 0xffffffff;
            }
            else  /*! P or B frame */
            {
                pstFrmPos->eFrameType = (3==picture_coding_type)? FIDX_FRAME_TYPE_B: FIDX_FRAME_TYPE_P;
                pstFrmPos->s64GlobalOffset   = pstThisSC->s64GlobalOffset;
                pstFrmPos->s32OffsetInPacket = pstThisSC->s32OffsetInPacket;
                pstFrmPos->s32PacketCount    = pstThisSC->s32PacketCount;
#ifndef PUT_PTS_ON_I_FRAME_ONLY
                pstFrmPos->u32PTS            = pstCtx->u32PTS;
                pstCtx->u32PTS               = 0xffffffff;
#endif
            }
            pstFrmPos->s32FrameSize = 0; /*! frame size need the position of the next start code */
            s32Ret = FIDX_OK;
        }
    }

    return s32Ret;
}

/*!***********************************************************************
@brief
  process the start code of AVS
@return
  if success(find the start of a frame, and can output one index record)
  return FIDX_OK, otherwise return FIDX_ERR
************************************************************************/
HI_S32 ProcessSC_Avs(HI_S32 InstIdx)
{
    HI_U8 Code;
    HI_S32 s32Ret = FIDX_ERR;
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];
    SC_INFO_S   *pstSpsSC = &pstCtx->stSPS[0];
    SC_INFO_S   *pstThisSC = &pstCtx->stThisSC;
    FRAME_POS_S *pstFrmPos = &pstCtx->stNewFramePos;

    FIDX_ASSERT_RET( 0!=pstCtx->s32ThisSCValid, "ThisSC is not valid\n");
    FIDX_ASSERT_RET( !IS_SC_WRONG(), "not enough data for ThisSC\n");

    Code = pstCtx->u8ThisSCData[0];

    /*! if a frame is found, but its size have not been calculated, it is time to calc the frame size, and output this frame */
    if( pstFrmPos->eFrameType != FIDX_FRAME_TYPE_UNKNOWN )
    {
        if( 0xb0==Code || 0xb1==Code || 0xb3==Code || 0xb6==Code )  /*! this means end of the formal frame found, the frame size can be calculated */
        {
            pstFrmPos->s32FrameSize = (HI_S32)(pstThisSC->s64GlobalOffset - pstFrmPos->s64GlobalOffset);
            if( pstFrmPos->s32FrameSize < 0 )
            {
                /*! just incase */
                pstFrmPos->eFrameType = FIDX_FRAME_TYPE_UNKNOWN;
            }
            else
            {
                OUTPUT_FRAME();
            }
        }
    }

    if( 0xb0 == Code )  /* sequence header */
    {
        pstThisSC->s32ScID = (HI_S32)Code;
        pstThisSC->eScType = SC_TYPE_SPS;
        pstThisSC->s32ProfileId = pstCtx->u8ThisSCData[1];
        memcpy( pstSpsSC, pstThisSC, sizeof(SC_INFO_S) );
        pstCtx->u8SpsFresh[0] = 1;
        s32Ret = FIDX_ERR;
    }
    else if (0xb3 == Code || 0xb6 == Code)  /* picture header */
    {
        if( pstSpsSC->eScType != SC_TYPE_SPS ) /*! seqence parameters still not ready */
        {
            s32Ret = FIDX_ERR;
        }
        else
        {
            if( 0xb3 == Code && 1 == pstCtx->u8SpsFresh[0] )  /*! I frame */
            {
                pstFrmPos->eFrameType        = FIDX_FRAME_TYPE_I;
                pstFrmPos->s64GlobalOffset   = pstSpsSC->s64GlobalOffset;
                pstFrmPos->s32OffsetInPacket = pstSpsSC->s32OffsetInPacket;
                pstFrmPos->s32PacketCount    = pstSpsSC->s32PacketCount;

                pstFrmPos->u32PTS            = pstCtx->u32PTS;
                pstCtx->u32PTS               = 0xffffffff;
            }
            else  /*! P or B frame. The I frame without SPS before head also treated as P frame */
            {
                HI_U8 picture_coding_type = 1;
                if( 0xb6 == Code )
                {
                    if(0x48 == pstThisSC->s32ProfileId)
                    {
                        picture_coding_type = (pstCtx->u8ThisSCData[4] >> 6) & 3;
                    }
                    else
                    {
                    picture_coding_type = (pstCtx->u8ThisSCData[3] >> 6) & 3;
                    }
                    FIDX_ASSERT_RET(1==picture_coding_type || 2==picture_coding_type,
                        "AVS picture_coding_type out of range");
                }

                pstFrmPos->eFrameType = (1==picture_coding_type)? FIDX_FRAME_TYPE_P: FIDX_FRAME_TYPE_B;
                pstFrmPos->s64GlobalOffset   = pstThisSC->s64GlobalOffset;
                pstFrmPos->s32OffsetInPacket = pstThisSC->s32OffsetInPacket;
                pstFrmPos->s32PacketCount    = pstThisSC->s32PacketCount;

#ifndef PUT_PTS_ON_I_FRAME_ONLY
                pstFrmPos->u32PTS            = pstCtx->u32PTS;
                pstCtx->u32PTS               = 0xffffffff;
#endif
            }
            pstFrmPos->s32FrameSize = 0; /*! frame size need the position of the next start code */
            s32Ret = FIDX_OK;
        }
    }

    return s32Ret;
}

/************************************************************************
@brief
  process the start code of MPEG4
@return
  if success(find the start of a frame, and can output one index record)
  return FIDX_OK, otherwise return FIDX_ERR
************************************************************************/
HI_S32 ProcessSC_Mpeg4(HI_S32 InstIdx)
{
    HI_U8 Code;
    HI_S32 s32Ret = FIDX_ERR;
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];
    SC_INFO_S   *pstSpsSC = &pstCtx->stSPS[0];
    SC_INFO_S   *pstThisSC = &pstCtx->stThisSC;
    FRAME_POS_S *pstFrmPos = &pstCtx->stNewFramePos;

    FIDX_ASSERT_RET( 0!=pstCtx->s32ThisSCValid, "ThisSC is not valid\n");
    FIDX_ASSERT_RET( !IS_SC_WRONG(), "not enough data for ThisSC\n");

    Code = pstCtx->u8ThisSCData[0];

    /*! if a frame is found, but its size have not been calculated, it is time to calc the frame size, and output this frame */
    if( pstFrmPos->eFrameType != FIDX_FRAME_TYPE_UNKNOWN )
    {
        if( 0xb2 != Code )  /*! this means end of the formal frame found, the frame size can be calculated */
        {
            pstFrmPos->s32FrameSize = (HI_S32)(pstThisSC->s64GlobalOffset - pstFrmPos->s64GlobalOffset);
            if( pstFrmPos->s32FrameSize < 0 )
            {
                 /*! just incase */
                pstFrmPos->eFrameType = FIDX_FRAME_TYPE_UNKNOWN;
            }
            else
            {
                OUTPUT_FRAME();
            }
        }
    }

    /*! distribution of the sequence level parameters:pstCtx->u8ThisSCData[x]
       [0]: B0, Visual_Object_Sequence
       [1]: b5, Visual_Object
       [2]: 00~1f, Video_Object
       [3]: 20~2f, Video_Object_Layer
    */
    if( 0xb0 == Code )  /* Visual_Object_Sequence */
    {
        pstThisSC->s32ScID = (HI_S32)Code;
        pstThisSC->eScType = SC_TYPE_SPS;
        memcpy( &pstSpsSC[0], pstThisSC, sizeof(SC_INFO_S) );
        s32Ret = FIDX_ERR;
    }
    else if ( 0xb5 == Code )  /* b5, Visual_Object */
    {
        pstThisSC->s32ScID = (HI_S32)Code;
        pstThisSC->eScType = SC_TYPE_SPS;
        memcpy( &pstSpsSC[1], pstThisSC, sizeof(SC_INFO_S) );
        s32Ret = FIDX_ERR;
    }
    else if ( Code <= 0x1f )  /* Video_Object: 0x00 ~ 0x1f */
    {
        pstThisSC->s32ScID = (HI_S32)Code;
        pstThisSC->eScType = SC_TYPE_SPS;
        memcpy( &pstSpsSC[2], pstThisSC, sizeof(SC_INFO_S) );
        s32Ret = FIDX_ERR;
    }
    else if ( Code >= 0x20 && Code <= 0x2f )  /* Video_Object_Layer */
    {
        pstThisSC->s32ScID = (HI_S32)Code;
        pstThisSC->eScType = SC_TYPE_SPS;
        memcpy( &pstSpsSC[3], pstThisSC, sizeof(SC_INFO_S) );
        pstCtx->u8SpsFresh[3] = 1;
        s32Ret = FIDX_ERR;
    }
    else if (0xb6 == Code)  /* VOP: picture header */
    {
        if( pstSpsSC[3].eScType != SC_TYPE_SPS ) /*! seqence parameters still not ready */
        {
            s32Ret = FIDX_ERR;
        }
        else
        {
            HI_U8 picture_coding_type;
            SC_INFO_S *pstLatestSps;

            /*! some mpeg4 streams have no paramaters above VOL, or those parameters are only transmittered at very low frequency */
            pstLatestSps = &pstSpsSC[3];  /*! select VOL as the highest sequence parameter by default, if have higher, overload it */
            SELECT_SPS(pstSpsSC[2], pstLatestSps);
            SELECT_SPS(pstSpsSC[1], pstLatestSps);
            SELECT_SPS(pstSpsSC[0], pstLatestSps);

            picture_coding_type = (pstCtx->u8ThisSCData[1] >> 6) & 3;
            FIDX_ASSERT_RET(picture_coding_type<=2,
                "MPEG4 picture_coding_type out of range");

            if( 0 == picture_coding_type && 1== pstCtx->u8SpsFresh[3])  /*! I frame */
            {
                pstFrmPos->eFrameType        = FIDX_FRAME_TYPE_I;
                pstFrmPos->s64GlobalOffset   = pstLatestSps->s64GlobalOffset;
                pstFrmPos->s32OffsetInPacket = pstLatestSps->s32OffsetInPacket;
                pstFrmPos->s32PacketCount    = pstLatestSps->s32PacketCount;
                pstCtx->u8SpsFresh[3] = 0;

                pstFrmPos->u32PTS            = pstCtx->u32PTS;
                pstCtx->u32PTS               = 0xffffffff;
            }
            else /*! P or B frame. The I frame without SPS before head also treated as P frame */
            {
                pstFrmPos->eFrameType = (2==picture_coding_type)? FIDX_FRAME_TYPE_B: FIDX_FRAME_TYPE_P;
                pstFrmPos->s64GlobalOffset   = pstThisSC->s64GlobalOffset;
                pstFrmPos->s32OffsetInPacket = pstThisSC->s32OffsetInPacket;
                pstFrmPos->s32PacketCount    = pstThisSC->s32PacketCount;

#ifndef PUT_PTS_ON_I_FRAME_ONLY
                pstFrmPos->u32PTS            = pstCtx->u32PTS;
                pstCtx->u32PTS               = 0xffffffff;
#endif
            }
            pstFrmPos->s32FrameSize = 0; /*! frame size need the position of the next start code */
            s32Ret = FIDX_OK;
        }
    }

    return s32Ret;
}

static HI_U32 simple_ue_v( HI_U32 Code, HI_U32 *BitNum )
{
    HI_U32 Val;
    HI_U32 zeros;
    zeros = (HI_S32)PvrZerosMS_32(Code);
    if (zeros < 16)
    {
        *BitNum = (zeros<<1) + 1;
        Val = (Code>>(32-(*BitNum))) - 1;
    }
    else
    {
        *BitNum = 0;
        Val = 0xffffffff;
    }

    return Val;
}

static HI_S32  ParseSPS( HI_U8 *pu8Data, HI_S32 s32DataLen, HI_S32 *p32SpsID )
{
    HI_U32  u32Code, u32Val, u32BitNum;

    u32Code = ((HI_U32)pu8Data[4]<<24) | ((HI_U32)pu8Data[5]<<16) |
        ((HI_U32)pu8Data[6]<<8) | ((HI_U32)pu8Data[7]);

    u32Val = simple_ue_v( u32Code, &u32BitNum );
    FIDX_ASSERT_RET( u32Val<=31, "SPS ID out of range!" );
    FIDX_ASSERT_RET( ((u32BitNum+7)>>3)<=(HI_U32)(s32DataLen-4), "data NOT enough for SPS" );

    *p32SpsID = (HI_S32)u32Val;
    return FIDX_OK;
}

static HI_S32  ParsePPS( HI_U8 *pu8Data, HI_S32 s32DataLen, HI_S32 *ps32SPSID, HI_S32 *ps32PPSID )
{
    HI_S32 s32Code;
    HI_U32 u32Val, u32BitNum;
    PVR_BS Bs;

    PvrBsInit(&Bs, pu8Data, s32DataLen);
    PvrBsSkip(&Bs,8);

    /* pic_parameter_set_id */
    s32Code = PvrBsShow(&Bs, 32);
    u32Val = simple_ue_v((HI_U32)s32Code,&u32BitNum );
    FIDX_ASSERT_RET( u32Val<=255, "PPS ID out of range!" );
    FIDX_ASSERT_RET((HI_S32)BITS_2_BYTES((HI_U32)PvrBsPos(&Bs))<= s32DataLen, "data NOT enough for PPS" );
    *ps32PPSID = (HI_S32)u32Val;
    PvrBsSkip(&Bs, (HI_S32)u32BitNum );

    /* seq_parameter_set_id */
    s32Code = PvrBsShow(&Bs, 32);
    u32Val = simple_ue_v( (HI_U32)s32Code, &u32BitNum );
    FIDX_ASSERT_RET( u32Val<=31, "SPS ID out of range!" );
    FIDX_ASSERT_RET( (HI_S32)BITS_2_BYTES((HI_U32)PvrBsPos(&Bs))<=s32DataLen, "data NOT enough for SPS" );
    *ps32SPSID = (HI_S32)u32Val;

    return FIDX_OK;
}

static HI_S32  ParseSliceHeader( HI_U8 *pu8Data, HI_S32 s32DataLen, HI_S32 *ps32FirstMBInSlice,
    HI_S32 *ps32SliceType, HI_S32 *ps32PPSId )
{
    HI_S32  s32Code;
    HI_U32  u32Val, u32BitNum;
    PVR_BS Bs;

    PvrBsInit(&Bs, pu8Data, s32DataLen);
    PvrBsSkip(&Bs,8);

    /* first_mb_in_slice */
    s32Code = PvrBsShow( &Bs, 32 );
    u32Val = simple_ue_v((HI_U32)s32Code, &u32BitNum );
    FIDX_ASSERT_RET( u32Val<=8192, "'first_mb_in_slice' out of range!" );
    FIDX_ASSERT_RET( (HI_S32)BITS_2_BYTES((HI_U32)PvrBsPos(&Bs))<=s32DataLen, "data NOT enough for 'first_mb_in_slice'" );
    *ps32FirstMBInSlice = (HI_S32)u32Val;
    PvrBsSkip(&Bs, (HI_S32)u32BitNum );

    /* slice_type */
    s32Code = PvrBsShow( &Bs, 32 );
    u32Val = simple_ue_v((HI_U32)s32Code, &u32BitNum );
    FIDX_ASSERT_RET( (u32Val<=2 || (u32Val>=5&&u32Val<=7) ), "'slice_type' out of range!" );
    FIDX_ASSERT_RET( (HI_S32)BITS_2_BYTES((HI_U32)PvrBsPos(&Bs))<=s32DataLen, "data NOT enough for 'slice_type'" );
    *ps32SliceType = (HI_S32)u32Val;
    PvrBsSkip(&Bs, (HI_S32)u32BitNum );

    /* pic_parameter_set_id */
    s32Code = PvrBsShow( &Bs, 32 );
    u32Val = simple_ue_v( (HI_U32)s32Code, &u32BitNum );
    FIDX_ASSERT_RET( (u32Val<=255), "'pic_parameter_set_id' out of range!" );
    FIDX_ASSERT_RET( (HI_S32)BITS_2_BYTES((HI_U32)PvrBsPos(&Bs))<=s32DataLen, "data NOT enough for 'pic_parameter_set_id'" );
    *ps32PPSId = (HI_S32)u32Val;

    return FIDX_OK;
}

/*!
@brief
  For H264, produce frame information.
  This function is to be called when frame border is found. The frame information produced will be stored into 'stNewFramePos'
*/

static HI_VOID H264MakeFrame( FIDX_CTX_S *pstCtx )
{
    SC_INFO_S   *pstSpsSC = &pstCtx->stSPS[0];
    SC_INFO_S   *pstEndSC = &pstCtx->stSEIFollowSlice;
    SC_INFO_S   *pstThisSC = &pstCtx->stThisSC;
    FRAME_POS_S *pstFrmPos = &pstCtx->stNewFramePos;

    if( FIDX_FRAME_TYPE_I == pstFrmPos->eFrameType && 1 == pstCtx->u8SpsFresh[pstCtx->s32SpsID] )
    {
        /*! SPS should be treaded as the start of the I frame following the SPS. */
        //pstFrmPos->s64GlobalOffset = pstSpsSC[pstCtx->s32SpsID].s64GlobalOffset;
        pstFrmPos->s32OffsetInPacket = pstSpsSC[pstCtx->s32SpsID].s32OffsetInPacket;
        pstFrmPos->s32PacketCount = pstSpsSC[pstCtx->s32SpsID].s32PacketCount;
        pstCtx->u8SpsFresh[pstCtx->s32SpsID] = 0;
        if( SC_TYPE_NONSLICE != pstEndSC->eScType )
        {
            pstFrmPos->s32FrameSize = (HI_S32)(pstThisSC->s64GlobalOffset - 
                            pstFrmPos->s64GlobalOffset);
        }
        else
        {
            pstFrmPos->s32FrameSize = (HI_S32)(pstEndSC->s64GlobalOffset - 
                            pstFrmPos->s64GlobalOffset);
        }
    }
    else
    {
        if( FIDX_FRAME_TYPE_I == pstFrmPos->eFrameType && 1 != pstCtx->u8SpsFresh[pstCtx->s32SpsID] )
        {
            pstFrmPos->eFrameType = FIDX_FRAME_TYPE_P; /*! The I frame have no fresh SPS before head should be treaded as P frame */
        }
        if( SC_TYPE_NONSLICE != pstEndSC->eScType )
        {
            pstFrmPos->s32FrameSize = (HI_S32)(pstThisSC->s64GlobalOffset - pstFrmPos->s64GlobalOffset);
        }
        else
        {
            pstFrmPos->s32FrameSize = (HI_S32)(pstEndSC->s64GlobalOffset - pstFrmPos->s64GlobalOffset);
        }
    }

#ifdef PUT_PTS_ON_I_FRAME_ONLY
    /*! If This frame is P or B frame, do NOT consume PTS to prevent the following I frame have a PTS equals to -1 */
    if ( (FIDX_FRAME_TYPE_I != pstFrmPos->eFrameType) )
    {
        /* if the PTS has been used by current frame(and no new PTS was got later), restore the used PTS */
        if (0xffffffff == pstCtx->u32PTS)
        {
            pstCtx->u32PTS = pstFrmPos->u32PTS;
        }

        /* P or B frame have PTS equals to -1 */
        pstFrmPos->u32PTS = 0xffffffff;
    }
#endif
    return;
}

/*!***********************************************************************
@brief
  process H.264 start code
@return
if success(find the start of a frame, and can output one index record)
  return FIDX_OK, otherwise return FIDX_ERR
************************************************************************/
HI_S32 ProcessSC_H264(HI_S32 InstIdx)
{
    HI_S32 s32Ret = FIDX_ERR;
    HI_S32 NalUnitType;
    HI_S32 s32SpsID, s32PpsID, s32FirstMb, s32SliceType;
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];
    SC_INFO_S   *pstSpsSC = &pstCtx->stSPS[0];
    SC_INFO_S   *pstPpsSC = &pstCtx->stPPS[0];
    SC_INFO_S   *pstEndSC = &pstCtx->stSEIFollowSlice;
    SC_INFO_S   *pstThisSC = &pstCtx->stThisSC;
    FRAME_POS_S *pstFrmPos = &pstCtx->stNewFramePos;
    HI_U8       *pu8Data = &pstCtx->u8ThisSCData[0];
    static  HI_S32  SlcType2FrmType[10] = {2,3,1,0,0,2,3,1,0,0};

    FIDX_ASSERT_RET( 0!=pstCtx->s32ThisSCValid, "ThisSC is not valid\n");
    FIDX_ASSERT_RET( !IS_SC_WRONG(), "not enough data for ThisSC\n");

    if(0 != (pu8Data[0] & 0x80) )
    {
        return FIDX_ERR;
    }
    NalUnitType = (HI_S32)(pu8Data[0] & 31);

    /*! to find the frame border, the position of the start code after the last slice of this frame is needed */
    if( FIDX_NALU_TYPE_IDR != NalUnitType && FIDX_NALU_TYPE_SLICE != NalUnitType && SC_TYPE_UNKNOWN == pstEndSC->eScType )
    {
        memcpy( pstEndSC, pstThisSC, sizeof(SC_INFO_S) );
        pstEndSC->eScType = SC_TYPE_NONSLICE;
    }

    if( FIDX_NALU_TYPE_SPS == NalUnitType )
    {
        if( FIDX_OK == (s32Ret=ParseSPS( pu8Data, pstCtx->s32ThisSCDataLen, &s32SpsID )) )
        {
            /*! if the SPS with the same ID as the former is found, the end of the last frame is also found */
            if( FIDX_FRAME_TYPE_UNKNOWN != pstFrmPos->eFrameType && pstCtx->s32SpsID == s32SpsID )
            {
                H264MakeFrame(pstCtx);
                OUTPUT_FRAME();
            }
            pstThisSC->eScType = SC_TYPE_SPS;
            pstThisSC->s32ScID = s32SpsID;
            memcpy( &pstSpsSC[s32SpsID], pstThisSC, sizeof(SC_INFO_S) );
            pstCtx->u8SpsFresh[s32SpsID] = 1;
        }
    }
    else if ( FIDX_NALU_TYPE_PPS == NalUnitType )
    {
        if( FIDX_OK == (s32Ret=ParsePPS( pu8Data, pstCtx->s32ThisSCDataLen, &s32SpsID, &s32PpsID)) )
        {
            /*! check the if the SPS for this PPS have been parsed OK */
            if( SC_TYPE_SPS == pstCtx->stSPS[s32SpsID].eScType )
            {
                /*! if the PPS with the same ID as the former is found, the end of the last frame is also found */
                if( FIDX_FRAME_TYPE_UNKNOWN != pstFrmPos->eFrameType && pstCtx->s32PpsID == s32PpsID )
                {
                    H264MakeFrame(pstCtx);
                    OUTPUT_FRAME();
                }
                pstThisSC->eScType = SC_TYPE_PPS;
                pstThisSC->s32ScID = s32PpsID;
                pstThisSC->s32SupScID = s32SpsID;
                memcpy( &pstPpsSC[s32PpsID], pstThisSC, sizeof(SC_INFO_S) );
            }
            else
            {
                s32Ret = FIDX_ERR;
            }
        }
    }
    else if ( FIDX_NALU_TYPE_IDR == NalUnitType || FIDX_NALU_TYPE_SLICE == NalUnitType )
    {
        if( FIDX_OK == (s32Ret=ParseSliceHeader(pu8Data, pstCtx->s32ThisSCDataLen, &s32FirstMb, &s32SliceType, &s32PpsID)) )
        {
            /*! check if the SPS and PPS for this slice are parsed OK */
            if( SC_TYPE_PPS == pstPpsSC[s32PpsID].eScType &&
                SC_TYPE_SPS == pstSpsSC[pstPpsSC[s32PpsID].s32SupScID].eScType )
            {
                pstThisSC->eScType = SC_TYPE_SLICE;  /*! mark this SC is a slice SC*/
            }

            /*! find frame border, output one frame information if necessary */
            if( FIDX_FRAME_TYPE_UNKNOWN != pstFrmPos->eFrameType )
            {
                if( (0==s32FirstMb || s32PpsID!=pstCtx->s32PpsID) )
                {
                    /*! find frame border, the end of the last frame found. */
                    H264MakeFrame(pstCtx);
                    OUTPUT_FRAME();
                }
                else
                {
                    /*! This SC is still belongs to the last frame. It's slice type is ok to fresh the frame type */
                    HI_S32 s32FrmType = SlcType2FrmType[s32SliceType];
                    pstFrmPos->eFrameType = (s32FrmType > pstFrmPos->eFrameType)? s32FrmType: pstFrmPos->eFrameType;
                    pstThisSC->eScType = SC_TYPE_UNKNOWN;  /* mark ThisSC is not valid */
                }
            }

            /*! if ThisSC belongs to the next new frame, create new frame information here */
            if( FIDX_FRAME_TYPE_UNKNOWN == pstFrmPos->eFrameType && SC_TYPE_SLICE == pstThisSC->eScType )
            {
                if( SC_TYPE_NONSLICE != pstEndSC->eScType )
                {
                    pstFrmPos->s64GlobalOffset = pstThisSC->s64GlobalOffset;
                    pstFrmPos->s32OffsetInPacket = pstThisSC->s32OffsetInPacket;
                    pstFrmPos->s32PacketCount = pstThisSC->s32PacketCount;
                }
                else
                {
                    pstFrmPos->s64GlobalOffset = pstEndSC->s64GlobalOffset;
                    pstFrmPos->s32OffsetInPacket = pstEndSC->s32OffsetInPacket;
                    pstFrmPos->s32PacketCount = pstEndSC->s32PacketCount;
                }
                pstFrmPos->eFrameType = (FIDX_FRAME_TYPE_E)SlcType2FrmType[s32SliceType];
                pstFrmPos->s32FrameSize = 0;

                pstFrmPos->u32PTS = pstCtx->u32PTS;
                pstCtx->u32PTS = 0xffffffff;

                pstCtx->s32PpsID = s32PpsID;
                pstCtx->s32SpsID = pstPpsSC[s32PpsID].s32SupScID;
                pstEndSC->eScType = SC_TYPE_UNKNOWN;
            }
        }
        pstEndSC->eScType = SC_TYPE_UNKNOWN;  /*! clear SEI when slice found to ensure the SEI is the one following the slice */
    }

    return s32Ret;
}

#define dprint  HI_DBG_DEMUX
#define pos()   dprint("%s %d\n", __func__, __LINE__);

//#define HEVC_REF_FILE

#define MAX(a, b)         (( (a) < (b) ) ?  (b) : (a))
#define MIN(a, b)         (( (a) > (b) ) ?  (b) : (a))
#define ABS(x)            (( (x) <  0  ) ? -(x) : (x))
#define SIGN(a)           (( (a)<0 ) ? (-1) : (1))
#define MEDIAN(a, b, c)   ((a) + (b) + (c) - MIN((a), MIN((b), (c))) - MAX((a), MAX((b), (c))))

#define CLIP1(high, x)             (MAX( MIN((x), high), 0))
#define CLIP3(low, high, x)        (MAX( MIN((x), high), low))
#define CLIP255(x)                 (MAX( MIN((x), 255), 0))


#ifndef FIDX_BIG_ENDIAN
#define  ENDIAN32( x )                      \
        ( (x)<<24 ) |                       \
        ( ( (x) & 0x0000ff00 ) << 8 ) |     \
        ( ( (x) & 0x00ff0000 ) >> 8 ) |     \
        ( ( (x) >> 24 ) & 0x000000ff )

#else
#define  ENDIAN32( x )      (x)
#endif

static UINT8 CalcZeroNum[256] = 
{
    8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
};

static UINT32 ZerosMS_32(UINT32 data)
{
    UINT32 i, ZeroNum = 0, tmpZeroNum = 0;
    UINT32 tmpdata = 0; 
    
    for (i=4; i>0; i--)
    {
        tmpdata = (data & 0xff000000) >> 24;
        tmpZeroNum = (UINT32)CalcZeroNum[tmpdata];
        ZeroNum += tmpZeroNum;
        if (8 != tmpZeroNum) 
        {
            break;
        }
        data = data<<8;
    } 
    
    return ZeroNum;
}

static VOID BsInit( BS *pBS, UINT8 *pInput, SINT32 length )
{
    UINT32 Data;
    ULONG AlignWord = 0x3;

    pBS->pHead    = ( UINT8 *)(((ULONG)pInput)&(~AlignWord));
    pBS->pTail    = pBS->pHead + 8;
    pBS->BsLen    = length;

    Data = *(UINT32 *)pBS->pHead;
    pBS->Bufa     = ENDIAN32( Data );
    Data = *(UINT32 *)(pBS->pHead + 4);
    pBS->Bufb     = ENDIAN32( Data );

    pBS->BufPos   = (((ULONG)pInput)&0x3)<<3;
    pBS->TotalPos = 0;

    return;
}

#if 0
static SINT32 BsPos( BS *pBS )
{
	return( pBS->TotalPos );
}
#endif

static inline SINT32  BsShow( BS *pBS, SINT32 nBits )
{
    SINT32 ABBufPos = nBits + pBS->BufPos;
    UINT32 Data, Data1;

    if( ABBufPos > 32 )
    {
        Data  = pBS->Bufa << pBS->BufPos;   //( ABBufPos - 32 );
        Data1 = pBS->Bufb >> (32-pBS->BufPos);
        Data |= Data1;
        Data >>= (32-nBits);
    }
    else
    {
        Data  = (pBS->Bufa << pBS->BufPos) >> (32 - nBits);
    }

    return(Data);
}

static inline SINT32  BsSkip( BS *pBS, SINT32 nBits )
{
    SINT32 ABBufPos = nBits + pBS->BufPos;
    UINT32 Data1;

    pBS->TotalPos += nBits;

    if( ABBufPos >= 32 )
    {
        pBS->BufPos   =  ABBufPos - 32;

        pBS->Bufa = pBS->Bufb;
        Data1 = *(UINT32*)pBS->pTail;
        pBS->Bufb = ENDIAN32( Data1 );

        pBS->pTail += 4;
    }
    else
    {
        pBS->BufPos   += nBits;
    }

    return( nBits );
}

static inline SINT32  BsGet( BS *pBS, SINT32 nBits )
{
    UINT32 Data;

    Data = BsShow( pBS, nBits );
    BsSkip( pBS, nBits );

    return(Data);
}

static inline SINT32  BsBack( BS *pBS, SINT32 nBits )
{
    SINT32 PredBufPos;
    UINT32 Data1;

    if( nBits > pBS->TotalPos )
        nBits = pBS->TotalPos;

    PredBufPos     = (SINT32)pBS->BufPos - nBits;
    pBS->TotalPos -= nBits;

    if( PredBufPos >= 0 )
    {
        pBS->BufPos = PredBufPos;
    }
    else
    {
        pBS->pTail  -= 4;
        pBS->Bufb   = pBS->Bufa;
        Data1 = *(UINT32*)(pBS->pTail - 8);
        pBS->Bufa = ENDIAN32( Data1 );

        pBS->BufPos = PredBufPos + 32;

    }
    return( nBits );
}

static inline SINT32  BsResidBits( BS *pBS )
{
    return( 8*pBS->BsLen - pBS->TotalPos );
}

static SINT32 g_quantTSDefault4x4[16] =
{
    16,16,16,16,
    16,16,16,16,
    16,16,16,16,
    16,16,16,16
};

static SINT32 g_quantIntraDefault8x8[64] =
{
    16,16,16,16,17,18,21,24,
    16,16,16,16,17,19,22,25,
    16,16,17,18,20,22,25,29,
    16,16,18,21,24,27,31,36,
    17,17,20,24,30,35,41,47,
    18,19,22,27,35,44,54,65,
    21,22,25,31,41,54,70,88,
    24,25,29,36,47,65,88,115
};

static SINT32 g_quantInterDefault8x8[64] =
{
    16,16,16,16,17,18,20,24,
    16,16,16,17,18,20,24,25,
    16,16,17,18,20,24,25,28,
    16,17,18,20,24,25,28,33,
    17,18,20,24,25,28,33,41,
    18,20,24,25,28,33,41,54,
    20,24,25,28,33,41,54,71,
    24,25,28,33,41,54,71,91
};

static VOID HEVC_InitSigLastScan(HEVC_CTX_S *pHevcCtx, UINT32* pBuffZ, UINT32* pBuffH, UINT32* pBuffV, UINT32* pBuffD, SINT32 iWidth, SINT32 iHeight, SINT32 iDepth)
{
    UINT32 uiNumScanPos;
    UINT32 uiNextScanPos = 0;
    SINT32 uiScanLine, iPrimDim, iScndDim;
    UINT32 *pBuffTemp;
    UINT32 uiNumBlkSide, uiNumBlks, uiBlk, initBlkPos;
    SINT32 log2Blk; 
    UINT32 offsetY, offsetX, offsetD, offsetScan;
    SINT32 uiCnt, numBlkSide, offset;
    SINT32 x, y, blkY, blkX, iY, iX;

    uiNumScanPos  = iWidth * iWidth;

    if (iWidth < 16)
    {
        pBuffTemp = pBuffD;
        if (8 == iWidth)
        {
            pBuffTemp = pHevcCtx->sigLastScanCG32x32;
        }
        for (uiScanLine=0; uiNextScanPos<uiNumScanPos; uiScanLine++)
        {
            iPrimDim = uiScanLine;
            iScndDim = 0;
            while (iPrimDim >= iWidth)
            {
                    iScndDim++;
                    iPrimDim--;
            }
            while (iPrimDim >= 0 && iScndDim < iWidth)
            {
                    pBuffTemp[uiNextScanPos] = iPrimDim * iWidth + iScndDim ;
                    uiNextScanPos++;
                    iScndDim++;
                    iPrimDim--;
            }
        }
    }
    
    if (iWidth > 4)
    {
        uiNumBlkSide = iWidth >> 2;
        uiNumBlks    = uiNumBlkSide * uiNumBlkSide;
        log2Blk      = pHevcCtx->aucConvertToBit[uiNumBlkSide] + 1;

        for (uiBlk=0; uiBlk<uiNumBlks; uiBlk++)
        {
            uiNextScanPos = 0;
            initBlkPos    = pHevcCtx->auiSigLastScan[ SCAN_DIAG ][ log2Blk ][ uiBlk ];
            if (32 == iWidth)
            {
                initBlkPos = pHevcCtx->sigLastScanCG32x32[uiBlk];
            }
            offsetY = initBlkPos / uiNumBlkSide;
            offsetX = initBlkPos - offsetY * uiNumBlkSide;
            offsetD = 4 * (offsetX + offsetY * iWidth);
            offsetScan = 16 * uiBlk;
            for (uiScanLine=0; uiNextScanPos<16; uiScanLine++)
            {
                iPrimDim = uiScanLine ;
                iScndDim = 0;
                while (iPrimDim >= 4)
                {
                    iScndDim++;
                    iPrimDim--;
                }
                while (iPrimDim >= 0 && iScndDim < 4)
                {
                    pBuffD[uiNextScanPos + offsetScan] = iPrimDim * iWidth + iScndDim + offsetD;
                    uiNextScanPos++;
                    iScndDim++;
                    iPrimDim--;
                }
            }
        }
    }

    uiCnt = 0;
    if (iWidth > 2)
    {
        numBlkSide = iWidth >> 2;
        for (blkY=0; blkY<numBlkSide; blkY++)
        {
            for (blkX=0; blkX<numBlkSide; blkX++)
            {
                offset = blkY * 4 * iWidth + blkX * 4;
                for (y=0; y<4; y++)
                {
                    for (x=0; x<4; x++)
                    {
                        pBuffH[uiCnt] = y*iWidth + x + offset;
                        uiCnt++;
                    }
                }
            }
        }

        uiCnt = 0;
        for (blkX=0; blkX<numBlkSide; blkX++)
        {
            for (blkY=0; blkY<numBlkSide; blkY++)
            {
                offset = blkY * 4 * iWidth + blkX * 4;
                for (x=0; x<4; x++)
                {
                    for (y=0; y<4; y++)
                    {
                        pBuffV[uiCnt] = y*iWidth + x + offset;
                    	uiCnt ++;
                    }
                }
            }
        }
    }
    else
    {
        for (iY=0; iY<iHeight; iY++)
        {
            for (iX=0; iX<iWidth; iX++)
            {
                pBuffH[uiCnt] = iY*iWidth + iX;
                uiCnt ++;
            }
        }

        uiCnt = 0;
        for (iX=0; iX<iWidth; iX++)
        {
            for (iY=0; iY<iHeight; iY++)
            {
                pBuffV[uiCnt] = iY*iWidth + iX;
                uiCnt ++;
            }
        }
    }
    
    return;
}

static VOID HEVC_InitScalingOrderTable(HEVC_CTX_S *pHevcCtx)
{
    SINT32 i, c;

    for (i=0;i<sizeof(pHevcCtx->aucConvertToBit);i++)
    {
        pHevcCtx->aucConvertToBit[i] = -1;
    }

#if 0 // 这个会越界
    c = 0;

    for (i = 4; i < HEVC_MAX_CU_SIZE; i *= 2)
    {
        pHevcCtx->aucConvertToBit[i] = c;
        c++;
    }

    pHevcCtx->aucConvertToBit[i] = c;
#else
    c = 0;

    for (i = 2; i < HEVC_MAX_CU_DEPTH; i++)
    {
        pHevcCtx->aucConvertToBit[(1 << i)] = c;
        c++;
    }

#endif

    c=2;
    for (i=0; i<HEVC_MAX_CU_DEPTH; i++)
    {  
      HEVC_InitSigLastScan(pHevcCtx, pHevcCtx->auiSigLastScan[0][i], pHevcCtx->auiSigLastScan[1][i], pHevcCtx->auiSigLastScan[2][i], pHevcCtx->auiSigLastScan[3][i], c, c, i);		
      c <<= 1;
    }  

    return;
}

static SINT8 HEVC_IsIUnit(UINT32 nal_unit_type) 
{
    return (nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_W_RADL \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_N_LP   \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_N_LP   \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_RADL \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_LP   \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_CRA)
            ? 1 : 0;
}

static SINT32 HEVC_IsFlushUnit(UINT32 nal_unit_type) 
{
    return (nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_W_RADL \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_N_LP   \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_N_LP   \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_RADL \
         || nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_LP)
            ? 1 : 0;
}

static VOID HEVC_InitDecPara(HEVC_CTX_S *pHevcCtx)
{
    UINT32 i;
    
    pos();

    pHevcCtx->LastDisplayPoc = -HEVC_MAX_INT;
 
    for (i=0; i<HEVC_MAX_FRAME_STORE; i++)
    {
        pHevcCtx->FrameStore[i].non_existing = 0; 
        pHevcCtx->FrameStore[i].eFrameStoreState = FS_NOT_USED;
        pHevcCtx->FrameStore[i].is_reference = 0; 
        pHevcCtx->FrameStore[i].poc = 0;
        pHevcCtx->FrameStore[i].frame.frame_store = & pHevcCtx->FrameStore[i];
    }
 
    for (i=0; i<HEVC_MAX_DPB_NUM; i++)
    {
        pHevcCtx->DPB.fs[i] = pHevcCtx->DPB.fs_negative_ref[i] = pHevcCtx->DPB.fs_positive_ref[i] = pHevcCtx->DPB.fs_ltref[i] = NULL;
    }
    pHevcCtx->DPB.used_size = 0;
    pHevcCtx->DPB.max_long_term_pic_idx = 0;
    pHevcCtx->DPB.ltref_frames_in_buffer = 0;
    pHevcCtx->DPB.negative_ref_frames_in_buffer = 0;
    pHevcCtx->DPB.positive_ref_frames_in_buffer = 0;
    pHevcCtx->DPB.size = HEVC_MAX_DPB_NUM;

    for (i=0; i<HEVC_MAX_LIST_SIZE; i++)
    {
        pHevcCtx->pListX[0][i] = pHevcCtx->pListX[1][i] = NULL;
    }
 
    memset(&pHevcCtx->CurrSlice, 0, sizeof(HEVC_SLICE_SEGMENT_HEADER));
    pHevcCtx->CurrSlice.slice_type = HEVC_ERR_SLICE;
    pHevcCtx->CurrSlice.new_pic_type = IS_NEW_PIC;
    pHevcCtx->CurrPic.pic_type = HEVC_ERR_FRAME;
    pHevcCtx->TotalSliceNum = 0; 
    
    pHevcCtx->bNewSequence = HEVC_TRUE;
    pHevcCtx->bNoOutputOfPriorPicsFlag = HEVC_FALSE;
    pHevcCtx->bNoRaslOutputFlag = HEVC_TRUE;
    
    pHevcCtx->AllowStartDec = 0;
    pHevcCtx->PocRandomAccess = HEVC_MAX_INT;
    pHevcCtx->PrevRAPIsBLA = HEVC_FALSE;

    pHevcCtx->scalingListSize[0] = 16;
    pHevcCtx->scalingListSize[1] = 64;
    pHevcCtx->scalingListSize[2] = 256;
    pHevcCtx->scalingListSize[3] = 1025;

    pHevcCtx->scalingListSizeX[0] = 4;
    pHevcCtx->scalingListSizeX[1] = 8;
    pHevcCtx->scalingListSizeX[2] = 16;
    pHevcCtx->scalingListSizeX[3] = 32;

    pHevcCtx->scalingListNum[0] = 6;
    pHevcCtx->scalingListNum[1] = 6;
    pHevcCtx->scalingListNum[2] = 6;
    pHevcCtx->scalingListNum[3] = 2;
    
    memset(&pHevcCtx->Bs,0,sizeof(BS));  
    pHevcCtx->pBS = &pHevcCtx->Bs;
    pHevcCtx->pCurrNal = &pHevcCtx->NalArray;
    
    return;
}

static UINT32 HEVC_ue_v(BS *pBs, SINT8 *Name)
{
    UINT32 NumBits, tmpbits;
    UINT32 Info;
    UINT32 LeadingZeros;

    tmpbits = BsShow(pBs, 32);
    LeadingZeros = ZerosMS_32(tmpbits);
    if (LeadingZeros < 32)
    {
        BsSkip(pBs, LeadingZeros);
        Info = BsShow(pBs, (LeadingZeros + 1)) - 1;
        BsSkip(pBs, (LeadingZeros + 1));
        NumBits = (LeadingZeros << 1) + 1;
    }
    else
    {
        Info = 0xffffeeee; 
        NumBits = 32;
        return Info;
    }

    return Info;
}

static UINT32 HEVC_u_v(BS *pBs, SINT32 v, SINT8 *Name)
{
    UINT32 Code;

    Code = BsGet(pBs, v);

    return Code;
}

static SINT32 HEVC_DecPTL(HEVC_CTX_S *pHevcCtx,HEVC_PROFILE_TIER_LEVEL_S *pPTL, SINT32 ProfilePresentFlag, SINT32 MaxNumSubLayersMinus1)
{
    SINT32 i;

    if (ProfilePresentFlag)
    {
        HEVC_u_v(pHevcCtx->pBS, 8, "general_profile_space[]");
        HEVC_u_v(pHevcCtx->pBS, 32, "xxx");
        HEVC_u_v(pHevcCtx->pBS, 20, "general_reserved_zero_44bits[0..15]");
        HEVC_u_v(pHevcCtx->pBS, 28, "general_reserved_zero_44bits[16..31]");
    }

    pPTL->general_level_idc = HEVC_u_v(pHevcCtx->pBS, 8, "general_level_idc");

    for (i=0; i<MaxNumSubLayersMinus1; i++)
    {
        if (ProfilePresentFlag)
        {
            pPTL->sub_layer_profile_present_flag[i] = HEVC_u_v(pHevcCtx->pBS, 1, "sub_layer_profile_present_flag");        
        }
        pPTL->sub_layer_level_present_flag[i]   = HEVC_u_v(pHevcCtx->pBS, 1, "sub_layer_level_present_flag");
    }

    if (MaxNumSubLayersMinus1 > 0)
    {
        for (i = MaxNumSubLayersMinus1; i < 8; i++)
        {  
            HEVC_u_v(pHevcCtx->pBS,2,"reserved_zero_2bits");
        }        
    }

    for (i=0; i<MaxNumSubLayersMinus1;i++)
    {
        if (ProfilePresentFlag && pPTL->sub_layer_profile_present_flag[i])
        {
            HEVC_u_v(pHevcCtx->pBS, 8, "sub_layer_profile_space");
            HEVC_u_v(pHevcCtx->pBS, 32, "sub_layer_profile_compatibility_flag");
            HEVC_u_v(pHevcCtx->pBS, 20, "general_progressive_source_flag");
            HEVC_u_v(pHevcCtx->pBS, 28,"general_reserved_zero_44bits[16..31]");
        }

        if (pPTL->sub_layer_level_present_flag[i])
        {
            HEVC_u_v(pHevcCtx->pBS, 8, "sub_layer_level_idc");
        }
    }
    
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_ProcessVPS(HEVC_CTX_S *pHevcCtx, HEVC_VIDEO_PARAM_SET_S *pVPS)
{
    char buf[100];
    SINT32 ret;
    SINT32 i,j;
    SINT32 vps_max_sub_layers_minus1;

    pVPS->vps_reserved_three_2bits = HEVC_u_v(pHevcCtx->pBS, 2, "vps_reserved_three_2bits");
    if (pVPS->vps_reserved_three_2bits != 0x3)
    {
    	dprint( "vps_reserved_three_2bits(%d) not equal 0x3.\n", pVPS->vps_reserved_three_2bits); 	
    }

    pVPS->vps_max_layers_minus1 = HEVC_u_v(pHevcCtx->pBS, 6, "vps_max_layers_minus1");
    if (pVPS->vps_max_layers_minus1 < 0 || pVPS->vps_max_layers_minus1 > 63)
    {
        dprint( "vps_max_layers_minus1 out of range(0,63).\n");
        return HEVC_DEC_ERR;
    }

    vps_max_sub_layers_minus1 = HEVC_u_v(pHevcCtx->pBS, 3, "vps_max_sub_layers_minus1");
    if (vps_max_sub_layers_minus1 < 0 || vps_max_sub_layers_minus1 > 6)
    {
        dprint( "vps_max_sub_layers_minus1 out of range(0,6).\n");
        return HEVC_DEC_ERR;
    }

    pVPS->vps_max_sub_layers_minus1 = vps_max_sub_layers_minus1 + 1;
    pVPS->vps_temporal_id_nesting_flag = HEVC_u_v(pHevcCtx->pBS, 1, "vps_temporal_id_nesting_flag");

    pVPS->vps_reserved_0xffff_16bits = HEVC_u_v(pHevcCtx->pBS, 16, "vps_reserved_ffff_16bits");

    if (pVPS->vps_reserved_0xffff_16bits != 0xffff)
    {
        dprint( "vps_reserved_0xffff_16bits not equal 0xffff.\n");
        return HEVC_DEC_ERR;
    }

    ret = HEVC_DecPTL(pHevcCtx, &(pVPS->profile_tier_level), 1, vps_max_sub_layers_minus1);
    if (HEVC_DEC_NORMAL != ret)
    {
        dprint( "VPS HEVC_DecPTL error.\n");
        return HEVC_DEC_ERR;
    }

    pVPS->vps_sub_layer_ordering_info_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "vps_sub_layer_ordering_info_present_flag");

    for (i=0; i<=vps_max_sub_layers_minus1; i++)
    {
        pVPS->vps_max_dec_pic_buffering[i] = HEVC_ue_v(pHevcCtx->pBS,  "vps_max_dec_pic_buffering_minus1[i]")+1;
        pVPS->vps_num_reorder_pics[i] = HEVC_ue_v(pHevcCtx->pBS, "vps_num_reorder_pics[i]");
        pVPS->vps_max_latency_increase[i]= HEVC_ue_v(pHevcCtx->pBS,  "vps_max_latency_increase_plus1[i]");
        
        if (!pVPS->vps_sub_layer_ordering_info_present_flag)
        {
            for (i++; i <= vps_max_sub_layers_minus1; i++)
            {
                pVPS->vps_max_dec_pic_buffering[i] =  pVPS->vps_max_dec_pic_buffering[0] ;
                pVPS->vps_num_reorder_pics[i] = pVPS->vps_num_reorder_pics[0];
                pVPS->vps_max_latency_increase[i]=	pVPS->vps_max_latency_increase[0];
            }
            break;
        }
    }

    pVPS->vps_max_layer_id= HEVC_u_v(pHevcCtx->pBS, 6, "vps_max_layer_id");

    pVPS->vps_num_layer_sets_minus1 = HEVC_ue_v(pHevcCtx->pBS,  "vps_num_layer_sets_minus1");
    if (pVPS->vps_num_layer_sets_minus1 < 0 || pVPS->vps_num_layer_sets_minus1 > 1023)
    {
        dprint( "vps_num_layer_sets_minus1(%d) out of range(0,1023).\n", pVPS->vps_num_layer_sets_minus1);
        return HEVC_DEC_ERR;
    }

    for (i = 1; i <= pVPS->vps_num_layer_sets_minus1; i++)
    {
        //Operation point set
        for (j = 0; j <= pVPS->vps_max_layer_id; j++)
        {
            snprintf(buf, sizeof(buf), "layer_id_included_flag[%d][%d]", i, j);
            pVPS->layer_id_included_flag[i][j]= HEVC_u_v(pHevcCtx->pBS, 1, buf);
        }
    }

    pVPS->vps_timing_info_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "vps_timing_info_present_flag");
    if (pVPS->vps_timing_info_present_flag)
    {
        pVPS->vps_num_units_in_tick = HEVC_u_v(pHevcCtx->pBS, 32, "vps_num_units_in_tick");
        pVPS->vps_time_scale = HEVC_u_v(pHevcCtx->pBS, 32, "vps_time_scale");
        pVPS->vps_poc_proportional_to_timing_flag = HEVC_u_v(pHevcCtx->pBS, 1, "vps_poc_proportional_to_timing_flag");
        if (pVPS->vps_poc_proportional_to_timing_flag)
        {
            pVPS->vps_num_ticks_poc_diff_one_minus1 = HEVC_ue_v(pHevcCtx->pBS, "vps_num_ticks_poc_diff_one_minus1");
        }
        pVPS->vps_num_hrd_parameters = HEVC_ue_v(pHevcCtx->pBS, "vps_num_hrd_parameters");
        
        if (pVPS->vps_num_hrd_parameters > 0)
        {
            pVPS->cprms_present_flag[0] = HEVC_TRUE; 
        }
    }
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_DecVPS(HEVC_CTX_S *pHevcCtx)
{
    UINT32  vps_video_parameter_set_id;
    HEVC_VIDEO_PARAM_SET_S *vps_tmp = NULL;
    
    pos();

    vps_tmp = &pHevcCtx->TmpParam.VpsTmp;
    memset(vps_tmp, 0, sizeof(HEVC_VIDEO_PARAM_SET_S));
        
    vps_video_parameter_set_id = HEVC_u_v(pHevcCtx->pBS,4, "vps_video_parameter_set_id");

    if (vps_video_parameter_set_id > 15)
    {
        dprint( "pVPS->vps_video_parameter_set_id out of range(0,15).\n");
        return HEVC_DEC_ERR;
    }

    if (pHevcCtx->pVPS[vps_video_parameter_set_id].valid) 
    {
        vps_tmp->video_parameter_set_id  = vps_video_parameter_set_id;

        if (HEVC_DEC_NORMAL != HEVC_ProcessVPS(pHevcCtx, vps_tmp))
        {
            dprint( "VPS[%d] decode error0.\n", vps_video_parameter_set_id);
            return HEVC_DEC_ERR;
        }

        //if (HEVC_DEC_NORMAL != HEVC_VPSEqual(vps_tmp, &(pHevcCtx->pVPS[vps_video_parameter_set_id])))
        {
            vps_tmp->is_refresh = 1;
            vps_tmp->valid = 1;
            memmove(&(pHevcCtx->pVPS[vps_video_parameter_set_id]), vps_tmp, sizeof(HEVC_VIDEO_PARAM_SET_S));	 
        }
    }
    else
    {
        pHevcCtx->pVPS[vps_video_parameter_set_id].video_parameter_set_id = vps_video_parameter_set_id;   

        if (HEVC_DEC_NORMAL != HEVC_ProcessVPS(pHevcCtx, &(pHevcCtx->pVPS[vps_video_parameter_set_id])))
        {
            dprint( "VPS[%d] decode error1.\n", vps_video_parameter_set_id);
            pHevcCtx->pVPS[vps_video_parameter_set_id].is_refresh = 1;
            pHevcCtx->pVPS[vps_video_parameter_set_id].valid = 0;
            return HEVC_DEC_ERR;
        }
        pHevcCtx->pVPS[vps_video_parameter_set_id].is_refresh = 1;
        pHevcCtx->pVPS[vps_video_parameter_set_id].valid = 1;
    }

    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_se_v(BS *pBs, SINT8 *Name)
{
    UINT32 NumBits, tmpbits;
    SINT32 Info;
    UINT32 LeadingZeros;
    UINT32 InfoLastBit;

    tmpbits = BsShow(pBs, 32);
    LeadingZeros = ZerosMS_32(tmpbits);

    if (LeadingZeros < 32)
    {
        BsSkip(pBs, LeadingZeros);
        Info = BsShow(pBs, (LeadingZeros + 1)) - 1;
        InfoLastBit = Info & 1;
        Info = Info >> 1;
        Info = ((InfoLastBit & 1) ? (Info + 1) : -1 * Info);
        BsSkip(pBs, (LeadingZeros + 1));
        NumBits = (LeadingZeros << 1) + 1;
    }
    else
    {
        Info = 0x7fffffff;
        NumBits = 32;
        return Info;
    }

    return Info;
}

static SINT32* HEVC_GetScalingListDefaultAddress(UINT32 sizeId, UINT32 matrixId)
{
    SINT32 *src = NULL;
    
    switch(sizeId)
    {
        case SCALING_LIST_4x4:
            src = g_quantTSDefault4x4;
            break;
        case SCALING_LIST_8x8:
            src = (matrixId<3) ? g_quantIntraDefault8x8 : g_quantInterDefault8x8;
            break;
        case SCALING_LIST_16x16:
            src = (matrixId<3) ? g_quantIntraDefault8x8 : g_quantInterDefault8x8;
            break;
        case SCALING_LIST_32x32:
            src = (matrixId<1) ? g_quantIntraDefault8x8 : g_quantInterDefault8x8;
            break;
        default:
            dprint( "HEVC_GetScalingListDefaultAddress NULL.\n");
            src = NULL;
            break;
    }
    return src;
}

static SINT32 HEVC_DecScalingListData(HEVC_CTX_S *pHevcCtx,HEVC_SCALING_LIST *pScalingList)
{
    UINT8   scalingListPredModeFlag;
    UINT32  code, sizeId, matrixId, reflistId;
    SINT32  coefNum, i;
    SINT32  nextCoef;
    UINT32 *pScan = NULL;
    SINT32 *pDstScalingList = NULL;
    SINT32 *pMatrixAddress = NULL;
    
    memcpy(pScalingList->scalingListCoef[SCALING_LIST_32x32][3],pScalingList->scalingListCoef[SCALING_LIST_32x32][1],HEVC_MAX_MATRIX_COEF_NUM*sizeof(SINT32));

    for (sizeId=0; sizeId<SCALING_LIST_SIZE_NUM; sizeId++)
    {
        for (matrixId=0; matrixId<pHevcCtx->scalingListNum[sizeId]; matrixId++)
        {
            scalingListPredModeFlag = HEVC_u_v(pHevcCtx->pBS, 1, "scaling_list_pred_mode_flag");
            pScalingList->scaling_list_pred_mode_flag[sizeId][matrixId] = scalingListPredModeFlag;
            coefNum = MIN(HEVC_MAX_MATRIX_COEF_NUM, (SINT32)pHevcCtx->scalingListSize[sizeId]);
            pDstScalingList = pScalingList->scalingListCoef[sizeId][matrixId];
            pScan = (0 == sizeId)?pHevcCtx->auiSigLastScan[SCAN_DIAG][1] : pHevcCtx->sigLastScanCG32x32;

            if (!scalingListPredModeFlag) //Copy Mode
            {
                code = HEVC_ue_v(pHevcCtx->pBS, "scaling_list_pred_matrix_id_delta");
                if (code > matrixId)
                {
                    dprint( "scaling_list_pred_matrix_id_delta out of range(0,matrixId).\n");
                    return HEVC_DEC_ERR;
                }

                pScalingList->scaling_list_pred_matrix_id_delta[sizeId][matrixId] = code;
                pScalingList->refMatrixId[sizeId][matrixId] = (UINT32)((SINT32)matrixId - code);
                if (sizeId > SCALING_LIST_8x8)
                {
                    reflistId = pScalingList->refMatrixId[sizeId][matrixId];
                    code = (matrixId == reflistId)? 16 : pScalingList->scalingListDC[sizeId][reflistId];	
                    pScalingList->scalingListDC[sizeId][matrixId] = code;
                }
                reflistId = pScalingList->refMatrixId[sizeId][matrixId];
                pMatrixAddress = (matrixId == reflistId)? HEVC_GetScalingListDefaultAddress(sizeId, reflistId) :
                pScalingList->scalingListCoef[sizeId][reflistId];
                if (pMatrixAddress == NULL)
                {
                    dprint( "HEVC_DecScalingListData pMatrixAddress NULL.\n");
                    return HEVC_DEC_ERR;
                }
                memcpy(pDstScalingList, pMatrixAddress, sizeof(SINT32)*coefNum);	
            }
            else   //DPCM Mode
            {
                nextCoef = HEVC_SCALING_LIST_START_VALUE;
                if (sizeId > SCALING_LIST_8x8)
                {
                    pScalingList->scaling_list_dc_coef_minus8 = HEVC_se_v(pHevcCtx->pBS, "scaling_list_dc_coef_minus8");
                    if (pScalingList->scaling_list_dc_coef_minus8 < -7 || pScalingList->scaling_list_dc_coef_minus8 > 247)
                    {
                        dprint( "scaling_list_dc_coef_minus8 out of range(-7,247).\n");
                        return HEVC_DEC_ERR;
                    }
                    pScalingList->scalingListDC[sizeId][matrixId] = pScalingList->scaling_list_dc_coef_minus8 + 8;
                    nextCoef = pScalingList->scalingListDC[sizeId][matrixId];
                }
                for (i=0; i<coefNum; i++)
                {
                    pScalingList->scaling_list_delta_coef = HEVC_se_v(pHevcCtx->pBS, "scaling_list_delta_coef");
                    if (pScalingList->scaling_list_delta_coef < -128 || pScalingList->scaling_list_delta_coef > 127)
                    {
                        dprint( "scaling_list_dc_coef_minus8 out of range(-128,127).\n");
                        return HEVC_DEC_ERR;
                    }
                    nextCoef = (nextCoef + pScalingList->scaling_list_delta_coef + 256) % 256;
                    pDstScalingList[pScan[i]] = nextCoef;
                }
            }
        }
    }
    
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_DecShortTermRefPicSet(HEVC_CTX_S *pHevcCtx,HEVC_SEQ_PARAM_SET_S *pSPS, HEVC_SHORT_TERM_RPSET_S *pShortTermRpset, UINT32 idx)
{
    UINT32 rIdx, code, refIdc;
    UINT32 k=0, k1=0, k2=0, prev=0;
    SINT32 deltaRPS, j;
    SINT32 deltaPOC, temp;
    UINT8  used;
    UINT32 numNegPics, i;
    UINT32 used_by_curr_pic_flag, use_delta_flag;
    UINT32 delta_poc_s0_minus1, used_by_curr_pic_s0_flag, delta_poc_s1_minus1, used_by_curr_pic_s1_flag;
    HEVC_SHORT_TERM_RPSET_S *pTempRPset = NULL;

    if (idx > 0)
    {
        pShortTermRpset->inter_ref_pic_set_prediction_flag = HEVC_u_v(pHevcCtx->pBS, 1, "inter_ref_pic_set_prediction_flag");
    }
    else
    {
        pShortTermRpset->inter_ref_pic_set_prediction_flag = HEVC_FALSE;
    }
    if (pShortTermRpset->inter_ref_pic_set_prediction_flag)
    {
        if (idx == pSPS->num_short_term_ref_pic_sets)
        {
            code = HEVC_ue_v(pHevcCtx->pBS, "delta_idx_minus1");
            pShortTermRpset->delta_idx = code + 1;
        }
        else
        {
            code = 0;
            pShortTermRpset->delta_idx = 0;
        }
        
        if (pShortTermRpset->delta_idx > idx)
        {
            dprint( "delta_idx(%d) > idx(%d).\n", pShortTermRpset->delta_idx, idx);
            return HEVC_DEC_ERR;
        }

        rIdx = idx - 1 - code;
        if (rIdx > (idx-1))
        {
            dprint( "rIdx(%d) > (idx-1)(%d) or < 0).\n", rIdx, (idx-1));
            return HEVC_DEC_ERR;
        }

        pTempRPset = &(pSPS->short_term_ref_pic_set[rIdx]);

        pShortTermRpset->delta_rps_sign = HEVC_u_v(pHevcCtx->pBS, 1, "delta_rps_sign");
        pShortTermRpset->abs_delta_rps = HEVC_ue_v(pHevcCtx->pBS, "abs_delta_rps_minus1") + 1;
        deltaRPS = (1 - (pShortTermRpset->delta_rps_sign << 1)) * pShortTermRpset->abs_delta_rps;

        if (pTempRPset->num_of_pics > HEVC_MAX_NUM_REF_PICS)
        {
            dprint( "pTempRPset->num_of_pics(%d) out of range(0,15).\n", pTempRPset->num_of_pics);
            return HEVC_DEC_ERR;
        }

        for (i=0; i <= pTempRPset->num_of_pics; i++)
        {
            used_by_curr_pic_flag = HEVC_u_v(pHevcCtx->pBS, 1, "used_by_curr_pic_flag"); //first bit is "1" if Idc is 1
            refIdc = used_by_curr_pic_flag;

            if (0 == refIdc)
            {
                use_delta_flag = HEVC_u_v(pHevcCtx->pBS, 1, "use_delta_flag");//second bit is "1" if Idc is 2, "0" otherwise.
                refIdc = use_delta_flag << 1;
            }
            if (1 == refIdc || 2 == refIdc)
            {
                deltaPOC = deltaRPS + ((i < pTempRPset->num_of_pics)?pTempRPset->delta_poc[i]:0);
                pShortTermRpset->delta_poc[k] = deltaPOC;
                pShortTermRpset->used_flag[k] = (1 == refIdc);

                if (deltaPOC < 0)
                {
                    k1++;
                }
                else
                {
                    k2++;
                }
                k++;
            }

            pShortTermRpset->refIdc[i] = refIdc;
        }
        pShortTermRpset->numRefIdc = pTempRPset->num_of_pics + 1;
        pShortTermRpset->num_of_pics = k;
        pShortTermRpset->num_negative_pics = k1;
        pShortTermRpset->num_positive_pics = k2;

        if (pShortTermRpset->num_of_pics > HEVC_MAX_NUM_REF_PICS || pShortTermRpset->num_negative_pics > HEVC_MAX_NUM_REF_PICS || pShortTermRpset->num_positive_pics > HEVC_MAX_NUM_REF_PICS)
        {
            dprint( "num_of_pics out of range(0,15).\n");
            return HEVC_DEC_ERR;
        }

        // sortDeltaPOC: sort in increasing order (smallest first)
        for (i=1; i<pShortTermRpset->num_of_pics; i++)
        {
            deltaPOC = pShortTermRpset->delta_poc[i];
            used = pShortTermRpset->used_flag[i];
            for (j=i-1; j>=0; j--)
            {
                temp = pShortTermRpset->delta_poc[j];
                if (deltaPOC < temp)
                {
                    pShortTermRpset->delta_poc[j+1] = temp;
                    pShortTermRpset->used_flag[j+1] = pShortTermRpset->used_flag[j];
                    pShortTermRpset->delta_poc[j] = deltaPOC;
                    pShortTermRpset->used_flag[j] = used;
                }
            }
        }
        
        // flip the negative values to largest first
        numNegPics = pShortTermRpset->num_negative_pics;
        for (i=0, j=(SINT32)(numNegPics-1); i<numNegPics>>1; i++, j--)
        {
            deltaPOC = pShortTermRpset->delta_poc[i];
            used = pShortTermRpset->used_flag[i];
            pShortTermRpset->delta_poc[i] = pShortTermRpset->delta_poc[j];
            pShortTermRpset->used_flag[i] = pShortTermRpset->used_flag[j];
            pShortTermRpset->delta_poc[j] = deltaPOC;
            pShortTermRpset->used_flag[j] = used;
        }
    }
    else
    {
        pShortTermRpset->num_negative_pics = HEVC_ue_v(pHevcCtx->pBS, "num_negative_pics");
        if (pShortTermRpset->num_negative_pics > HEVC_MAX_NUM_REF_PICS)
        {
            dprint( "pTempRPset->num_negative_pics(%d) out of range(0,15).\n", pShortTermRpset->num_negative_pics);
            return HEVC_DEC_ERR;
        }

        pShortTermRpset->num_positive_pics = HEVC_ue_v(pHevcCtx->pBS, "num_positive_pics");
        if (pShortTermRpset->num_positive_pics > HEVC_MAX_NUM_REF_PICS)
        {
            dprint( "pTempRPset->num_positive_pics(%d) out of range(0,15).\n", pShortTermRpset->num_negative_pics);
            return HEVC_DEC_ERR;
        }

        pShortTermRpset->num_of_pics = pShortTermRpset->num_negative_pics + pShortTermRpset->num_positive_pics;
        if (pShortTermRpset->num_of_pics > HEVC_MAX_NUM_REF_PICS)
        {
            dprint( "pShortTermRpset->num_of_pics(%d) out of range[0,%d].\n",pShortTermRpset->num_of_pics, HEVC_MAX_NUM_REF_PICS);
            return HEVC_DEC_ERR;
        }

        for (i=0; i<pShortTermRpset->num_negative_pics; i++)
        {
            delta_poc_s0_minus1           = HEVC_ue_v(pHevcCtx->pBS, "delta_poc_s0_minus1");
            if (delta_poc_s0_minus1 > 32767)
            {
                dprint("delta_poc_s0_minus1(%d) out of range.\n", delta_poc_s0_minus1);
                return HEVC_DEC_ERR;
            }
            deltaPOC                      = prev - delta_poc_s0_minus1 - 1;
            prev                          = deltaPOC;
            pShortTermRpset->delta_poc[i] = deltaPOC;
            used_by_curr_pic_s0_flag      = HEVC_u_v(pHevcCtx->pBS, 1, "used_by_curr_pic_s0_flag");
            pShortTermRpset->used_flag[i] = used_by_curr_pic_s0_flag;
        }
        
        prev = 0;
        for (i=pShortTermRpset->num_negative_pics; i<pShortTermRpset->num_of_pics; i++)
        {
            delta_poc_s1_minus1           = HEVC_ue_v(pHevcCtx->pBS, "delta_poc_s1_minus1");
            deltaPOC                      = prev + delta_poc_s1_minus1 + 1;
            prev                          = deltaPOC;
            pShortTermRpset->delta_poc[i] = deltaPOC;
            used_by_curr_pic_s1_flag      = HEVC_u_v(pHevcCtx->pBS, 1, "used_by_curr_pic_s1_flag");
            pShortTermRpset->used_flag[i] = used_by_curr_pic_s1_flag;
        }
    }
    
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_ProcessSPS(HEVC_CTX_S *pHevcCtx, HEVC_SEQ_PARAM_SET_S *pSPS)
{
    UINT32 i;
    SINT32 ret;
    UINT32 log2_max_pic_order_cnt_lsb_minus4;

    const SINT32 g_CropUnitX[4] = {1,2,2,1};
    const SINT32 g_CropUnitY[4] = {1,2,1,1};

    pSPS->chroma_format_idc = HEVC_ue_v(pHevcCtx->pBS, "chroma_format_idc");

    // chroma_format_idc equal to 1 (4:2:0)
    if (pSPS->chroma_format_idc != 1)
    {
        if (pSPS->chroma_format_idc > 3)
        {
            dprint( "pSPS->chroma_format_idc out of range(0,3).\n");
            return HEVC_DEC_ERR;
        }
        else
        {
            dprint( "pSPS->chroma_format_idc not equal(%d) 1.\n", pSPS->chroma_format_idc);
            return HEVC_DEC_ERR;
        }
    }

    if (3 == pSPS->chroma_format_idc)
    {
        pSPS->separate_colour_plane_flag = HEVC_u_v(pHevcCtx->pBS, 1, "separate_colour_plane_flag");
        if (0 != pSPS->separate_colour_plane_flag)
        {
            dprint( "pSPS->separate_colour_plane_flag not equal 0.\n");	
        }
    }

    pSPS->pic_width_in_luma_samples   = HEVC_ue_v(pHevcCtx->pBS, "pic_width_in_luma_samples");
    if (pSPS->pic_width_in_luma_samples > 8192)
    {
        dprint( "pic_width_in_luma_samples out of range(0,8192).\n"); 
        return HEVC_DEC_ERR;
    }

    pSPS->pic_height_in_luma_samples  = HEVC_ue_v(pHevcCtx->pBS, "pic_height_in_luma_samples");
    if (pSPS->pic_height_in_luma_samples > 8192)
    {
        dprint( "pic_height_in_luma_samples out of range(0,4096).\n");
        return HEVC_DEC_ERR;
    }

    pSPS->conformance_window_flag = HEVC_u_v(pHevcCtx->pBS, 1, "conformance_window_flag");

    if (pSPS->conformance_window_flag)
    {
        pSPS->conf_win_left_offset   = HEVC_ue_v(pHevcCtx->pBS, "conf_win_left_offset");
        pSPS->conf_win_left_offset   = pSPS->conf_win_left_offset  * g_CropUnitX[pSPS->chroma_format_idc];
        pSPS->conf_win_right_offset  = HEVC_ue_v(pHevcCtx->pBS, "conf_win_right_offset");
        pSPS->conf_win_right_offset  = pSPS->conf_win_right_offset * g_CropUnitX[pSPS->chroma_format_idc];
        if (pSPS->pic_width_in_luma_samples < (pSPS->conf_win_left_offset + pSPS->conf_win_right_offset))
        {
            dprint( "conf_win_left_offset+conf_win_right_offset out of range.\n");
            return HEVC_DEC_ERR;
        }

        pSPS->conf_win_top_offset    = HEVC_ue_v(pHevcCtx->pBS, "conf_win_top_offset");
        pSPS->conf_win_top_offset    = pSPS->conf_win_top_offset   * g_CropUnitY[pSPS->chroma_format_idc];
        pSPS->conf_win_bottom_offset = HEVC_ue_v(pHevcCtx->pBS, "conf_win_bottom_offset");
        pSPS->conf_win_bottom_offset = pSPS->conf_win_bottom_offset* g_CropUnitY[pSPS->chroma_format_idc];
        if (pSPS->pic_height_in_luma_samples < (pSPS->conf_win_top_offset + pSPS->conf_win_bottom_offset))
        {
            dprint( "conf_win_top_offset+conf_win_bottom_offset out of range.\n");
            return HEVC_DEC_ERR;
        }
    }

    pSPS->bit_depth_luma  = HEVC_ue_v(pHevcCtx->pBS, "bit_depth_luma_minus8") + 8;
    if (pSPS->bit_depth_luma != 8)
    {
        if (pSPS->bit_depth_luma < 8 || pSPS->bit_depth_luma > 14 )
        {
            dprint( "bit_depth_luma(%d) out of range(8,14).\n", pSPS->bit_depth_luma);
            return HEVC_DEC_ERR;
        }
    }

    pSPS->bit_depth_chroma = HEVC_ue_v(pHevcCtx->pBS, "bit_depth_chroma_minus8") + 8;
    if (pSPS->bit_depth_chroma != 8)
    {
        if (pSPS->bit_depth_chroma < 8 || pSPS->bit_depth_chroma > 14 )
        {
            dprint( "bit_depth_chroma(%d) out of range[8,14].\n", pSPS->bit_depth_chroma);
            return HEVC_DEC_ERR;
        }
    }

    pHevcCtx->BitDepthY = pSPS->bit_depth_luma;
    pHevcCtx->BitDepthC = pSPS->bit_depth_chroma;

    pSPS->qp_bdOffset_y = (pSPS->bit_depth_luma - 8)   * 6;
    pSPS->qp_bdOffset_c = (pSPS->bit_depth_chroma - 8) * 6;

    log2_max_pic_order_cnt_lsb_minus4 = HEVC_ue_v(pHevcCtx->pBS, "log2_max_pic_order_cnt_lsb_minus4");
    if (log2_max_pic_order_cnt_lsb_minus4 > 12 )
    {
        dprint( "log2_max_pic_order_cnt_lsb_minus4 out of range[0,12].\n");
        return HEVC_DEC_ERR;
    }

    pSPS->max_pic_order_cnt_lsb = log2_max_pic_order_cnt_lsb_minus4 + 4;
    pSPS->bits_for_poc = pSPS->max_pic_order_cnt_lsb;

    pSPS->sps_sub_layer_ordering_info_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "sps_sub_layer_ordering_info_present_flag");

    for (i=0; i <= pSPS->sps_max_sub_layers_minus1; i++)
    {
        pSPS->max_dec_pic_buffering[i] = HEVC_ue_v(pHevcCtx->pBS, "sps_max_dec_pic_buffering_minus1")+1;
        pSPS->num_reorder_pics[i]	   = HEVC_ue_v(pHevcCtx->pBS, "sps_num_reorder_pics");
        pSPS->max_latency_increase[i]  = HEVC_ue_v(pHevcCtx->pBS, "sps_max_latency_increase_plus1");
        if (!pSPS->sps_sub_layer_ordering_info_present_flag)
        {
            for (i++; i <= pSPS->sps_max_sub_layers_minus1; i++)
            {
                pSPS->max_dec_pic_buffering[i] = pSPS->max_dec_pic_buffering[0];
                pSPS->num_reorder_pics[i]	   = pSPS->num_reorder_pics[0];
                pSPS->max_latency_increase[i]  = pSPS->max_latency_increase[0];
            }
            break;
        }
    }

    pSPS->log2_min_luma_coding_block_size_minus3   = HEVC_ue_v(pHevcCtx->pBS, "log2_min_coding_block_size_minus3");
    if (pSPS->log2_min_luma_coding_block_size_minus3 > 3)
    {
        dprint( "log2_min_luma_coding_block_size_minus3 out of range(0,3).\n");
        return HEVC_DEC_ERR;
    }

    pSPS->log2_diff_max_min_luma_coding_block_size = HEVC_ue_v(pHevcCtx->pBS, "log2_diff_max_min_coding_block_size");

    if (pSPS->log2_diff_max_min_luma_coding_block_size > 3)
    {
        dprint( "log2_diff_max_min_luma_coding_block_size out of range(0,3).\n");
        return HEVC_DEC_ERR;
    }

    pSPS->log2_min_cb_sizeY = pSPS->log2_min_luma_coding_block_size_minus3 + 3;
    pSPS->log2_ctb_sizeY    = pSPS->log2_min_cb_sizeY + pSPS->log2_diff_max_min_luma_coding_block_size;
    if (pSPS->log2_ctb_sizeY < 4 || pSPS->log2_ctb_sizeY > 6)
    {
        dprint( "log2_ctb_sizeY out of range(4,6).\n");
        return HEVC_DEC_ERR;
    }

    pSPS->min_cb_sizeY      = 1 << pSPS->log2_min_cb_sizeY;
    pSPS->ctb_sizeY         = 1 << pSPS->log2_ctb_sizeY;
    pSPS->max_cu_width      = 1 << pSPS->log2_ctb_sizeY;
    pSPS->max_cu_height     = 1 << pSPS->log2_ctb_sizeY;
    pSPS->ctb_num_width     = (pSPS->pic_width_in_luma_samples % pSPS->max_cu_width)
                              ? (pSPS->pic_width_in_luma_samples / pSPS->max_cu_width + 1)
                              : (pSPS->pic_width_in_luma_samples / pSPS->max_cu_width);
    pSPS->ctb_num_height    = (pSPS->pic_height_in_luma_samples % pSPS->max_cu_height)
                              ? (pSPS->pic_height_in_luma_samples / pSPS->max_cu_height + 1)
                              : (pSPS->pic_height_in_luma_samples / pSPS->max_cu_height);

    pSPS->log2_min_transform_block_size_minus2 = HEVC_ue_v(pHevcCtx->pBS, "log2_min_transform_block_size_minus2");
    if (pSPS->log2_min_transform_block_size_minus2 > 3)
    {
        dprint( "log2_min_transform_block_size_minus2 out of range(0,3).\n");
        return HEVC_DEC_ERR;
    }

    pSPS->log2_diff_max_min_transform_block_size = HEVC_ue_v(pHevcCtx->pBS, "log2_diff_max_min_transform_block_size");
    if (pSPS->log2_diff_max_min_transform_block_size > 3)
    {
        dprint( "log2_diff_max_min_transform_block_size out of range(0,3).\n");
        return HEVC_DEC_ERR;
    }
    pSPS->quadtree_tu_log2_min_size = pSPS->log2_min_transform_block_size_minus2 + 2;
    if (pSPS->quadtree_tu_log2_min_size >= pSPS->log2_min_cb_sizeY)
    {
        dprint( "quadtree_tu_log2_min_size not less than log2_min_cb_sizeY.\n");
        return HEVC_DEC_ERR;
    }

    pSPS->quadtree_tu_log2_max_size = pSPS->quadtree_tu_log2_min_size + pSPS->log2_diff_max_min_transform_block_size;
    if (pSPS->quadtree_tu_log2_max_size > 5 || pSPS->quadtree_tu_log2_max_size > pSPS->log2_ctb_sizeY)
    {
        dprint( "quadtree_tu_log2_max_size greater than Min( CtbLog2SizeY, 5 ).\n");
        return HEVC_DEC_ERR;
    }

    pSPS->max_transform_hierarchy_depth_inter = HEVC_ue_v(pHevcCtx->pBS, "max_transform_hierarchy_depth_inter");
    if (pSPS->max_transform_hierarchy_depth_inter > pSPS->log2_ctb_sizeY-pSPS->quadtree_tu_log2_min_size)
    {
        dprint( "max_transform_hierarchy_depth_inter out of range(0,CtbLog2SizeY-Log2MinTrafoSize).\n");
        return HEVC_DEC_ERR;
    }

    pSPS->max_transform_hierarchy_depth_intra = HEVC_ue_v(pHevcCtx->pBS, "max_transform_hierarchy_depth_intra");
    if (pSPS->max_transform_hierarchy_depth_intra > pSPS->log2_ctb_sizeY-pSPS->quadtree_tu_log2_min_size )
    {
        dprint( "max_transform_hierarchy_depth_intra out of range(0,CtbLog2SizeY-Log2MinTrafoSize).\n");
        return HEVC_DEC_ERR;
    }

    pSPS->quadtree_tu_max_depth_inter = pSPS->max_transform_hierarchy_depth_inter + 1;
    pSPS->quadtree_tu_max_depth_intra = pSPS->max_transform_hierarchy_depth_intra + 1;

    pHevcCtx->uiAddCuDepth = 0;
    while (((UINT32)(pSPS->max_cu_width >> pSPS->log2_diff_max_min_luma_coding_block_size))
            > (UINT32)(1 << (pSPS->quadtree_tu_log2_min_size + pHevcCtx->uiAddCuDepth)))
    {
        pHevcCtx->uiAddCuDepth++;
    }
    pSPS->max_cu_depth = pSPS->log2_diff_max_min_luma_coding_block_size + pHevcCtx->uiAddCuDepth;

    pSPS->scaling_list_enabled_flag = HEVC_u_v(pHevcCtx->pBS, 1, "scaling_list_enabled_flag");
    if (pSPS->scaling_list_enabled_flag)
    {
        pSPS->sps_scaling_list_data_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "sps_scaling_list_data_present_flag");
        if (pSPS->sps_scaling_list_data_present_flag)
        {
            ret = HEVC_DecScalingListData(pHevcCtx,&(pSPS->scaling_list));
            if (HEVC_DEC_NORMAL != ret)
            {
                dprint( "SPS HEVC_DecScalingListData error.\n");
                return HEVC_DEC_ERR;
            }
        }
    }

    pSPS->amp_enabled_flag = HEVC_u_v(pHevcCtx->pBS, 1, "amp_enabled_flag");
    pSPS->sample_adaptive_offset_enabled_flag = HEVC_u_v(pHevcCtx->pBS, 1, "sample_adaptive_offset_enabled_flag");

    pSPS->pcm_enabled_flag = HEVC_u_v(pHevcCtx->pBS, 1, "pcm_enabled_flag");
    if (pSPS->pcm_enabled_flag)
    {
        pSPS->pcm_bit_depth_luma = HEVC_u_v(pHevcCtx->pBS, 4, "pcm_sample_bit_depth_luma_minus1") + 1;
        pSPS->pcm_bit_depth_chroma = HEVC_u_v(pHevcCtx->pBS, 4, "pcm_sample_bit_depth_chroma_minus1") + 1;

        pSPS->log2_min_pcm_coding_block_size_minus3 = HEVC_ue_v(pHevcCtx->pBS, "log2_min_pcm_luma_coding_block_size_minus3");
        
        if (pSPS->log2_min_pcm_coding_block_size_minus3 > 2)
        {
            dprint( "log2_min_pcm_coding_block_size_minus3(%d) out of range[0,2].\n", pSPS->log2_min_pcm_coding_block_size_minus3);
            //return HEVC_DEC_ERR;		
        }

        pSPS->log2_diff_max_min_pcm_coding_block_size = HEVC_ue_v(pHevcCtx->pBS, "log2_diff_max_min_pcm_luma_coding_block_size");
        if (pSPS->log2_diff_max_min_pcm_coding_block_size > 2)
        {
            dprint( "log2_diff_max_min_pcm_coding_block_size(%d) out of range[0,2].\n", pSPS->log2_diff_max_min_pcm_coding_block_size);
            //return HEVC_DEC_ERR;		
        }
        pSPS->pcm_log2_min_size = pSPS->log2_min_pcm_coding_block_size_minus3 + 3;
        pSPS->pcm_log2_max_size = pSPS->pcm_log2_min_size + pSPS->log2_diff_max_min_pcm_coding_block_size;
        if (pSPS->pcm_log2_max_size > 5 || pSPS->pcm_log2_max_size > pSPS->log2_ctb_sizeY)
        {
            dprint( "pcm_log2_max_size greater than Min( CtbLog2SizeY, 5 ).\n");
            //return HEVC_DEC_ERR;		
        }

        pSPS->pcm_loop_filter_disable_flag = HEVC_u_v(pHevcCtx->pBS, 1, "pcm_loop_filter_disable_flag");
    }

    pSPS->num_short_term_ref_pic_sets  = HEVC_ue_v(pHevcCtx->pBS, "num_short_term_ref_pic_sets");
    if (pSPS->num_short_term_ref_pic_sets > 64)
    {
        dprint( "num_short_term_ref_pic_sets out of range[0,64].\n");
        return HEVC_DEC_ERR;
    }

    for (i=0; i<pSPS->num_short_term_ref_pic_sets; i++)
    {
        //get short term reference picture sets
        ret = HEVC_DecShortTermRefPicSet(pHevcCtx, pSPS, &(pSPS->short_term_ref_pic_set[i]), i);
        if (ret != HEVC_DEC_NORMAL)
        {
            dprint( "SPS HEVC_DecShortTermRefPicSet error.\n");
            return HEVC_DEC_ERR;
        }
    }

    pSPS->long_term_ref_pics_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "long_term_ref_pics_present_flag");
    if (pSPS->long_term_ref_pics_present_flag)
    {
        pSPS->num_long_term_ref_pic_sps = HEVC_ue_v(pHevcCtx->pBS, "num_long_term_ref_pic_sps");
        if (pSPS->num_long_term_ref_pic_sps > HEVC_MAX_LSB_NUM-1)
        {
            dprint( "num_long_term_ref_pic_sps out of range[0,32].\n");
            return HEVC_DEC_ERR;
        }

        for (i=0; i<pSPS->num_long_term_ref_pic_sps; i++)
        {
            pSPS->lt_ref_pic_poc_lsb_sps[i] 	  = HEVC_u_v(pHevcCtx->pBS, pSPS->max_pic_order_cnt_lsb, "lt_ref_pic_poc_lsb_sps");
            pSPS->used_by_curr_pic_lt_sps_flag[i] = HEVC_u_v(pHevcCtx->pBS, 1, "used_by_curr_pic_lt_sps_flag");
        }
    }
    pSPS->sps_temporal_mvp_enable_flag = HEVC_u_v(pHevcCtx->pBS, 1, "sps_temporal_mvp_enable_flag");
    pSPS->sps_strong_intra_smoothing_enable_flag = HEVC_u_v(pHevcCtx->pBS, 1, "sps_strong_intra_smoothing_enable_flag");

    pSPS->is_refresh = 1;

    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_DecSPS(HEVC_CTX_S *pHevcCtx)
{
    SINT32 ret;
    UINT32 video_parameter_set_id;
    UINT32 sps_max_sub_layers_minus1;
    UINT8 sps_temporal_id_nesting_flag;
    UINT32 sps_seq_parameter_set_id;
    HEVC_PROFILE_TIER_LEVEL_S profile_tier_level;
    HEVC_SEQ_PARAM_SET_S *sps_tmp = NULL;
    
    pos();

    sps_tmp = &pHevcCtx->TmpParam.SpsTmp;
    memset(sps_tmp, 0, sizeof(HEVC_SEQ_PARAM_SET_S));
    memset(&profile_tier_level,0,sizeof(HEVC_PROFILE_TIER_LEVEL_S));

    video_parameter_set_id    = HEVC_u_v(pHevcCtx->pBS, 4, "sps_video_parameter_set_id");
    if (video_parameter_set_id >= (pHevcCtx->MaxVpsNum))
    {
        dprint( "sps_video_parameter_set_id out of range(0,%d).\n",pHevcCtx->MaxVpsNum);
        return HEVC_DEC_ERR;
    }

    sps_max_sub_layers_minus1 = HEVC_u_v(pHevcCtx->pBS, 3, "sps_max_sub_layers_minus1");

    if (sps_max_sub_layers_minus1 > 6)
    {
        dprint( "sps_max_sub_layers_minus1 out of range(0,6).\n");
        return HEVC_DEC_ERR;
    }
    sps_temporal_id_nesting_flag = HEVC_u_v(pHevcCtx->pBS, 1, "sps_temporal_id_nesting_flag");

    ret = HEVC_DecPTL(pHevcCtx, &(profile_tier_level), 1, sps_max_sub_layers_minus1);
    if (HEVC_DEC_NORMAL != ret)
    {
        dprint( "SPS HEVC_DecPTL error.\n");
        return HEVC_DEC_ERR;
    }

    sps_seq_parameter_set_id = HEVC_ue_v(pHevcCtx->pBS,"sps_seq_parameter_set_id");
    if (sps_seq_parameter_set_id >= pHevcCtx->MaxSpsNum)
    {
        dprint( "sps_seq_parameter_set_id out of range(0,%d).\n",pHevcCtx->MaxSpsNum);
        return HEVC_DEC_ERR;
    }

    if (pHevcCtx->pSPS[sps_seq_parameter_set_id].valid) 
    {
        sps_tmp->video_parameter_set_id  = video_parameter_set_id;
        sps_tmp->sps_max_sub_layers_minus1  = sps_max_sub_layers_minus1;
        sps_tmp->sps_temporal_id_nesting_flag  = sps_temporal_id_nesting_flag;
        memcpy(&(sps_tmp->profile_tier_level),&(profile_tier_level),sizeof(HEVC_PROFILE_TIER_LEVEL_S));
        sps_tmp->seq_parameter_set_id  = sps_seq_parameter_set_id;

        if (HEVC_DEC_NORMAL != HEVC_ProcessSPS(pHevcCtx, sps_tmp))
        {
            memcpy(&pHevcCtx->SEISPS, &pHevcCtx->pSPS[sps_seq_parameter_set_id], sizeof(HEVC_SEQ_PARAM_SET_S));
            dprint( "SPS[%d] decode error0.\n", sps_seq_parameter_set_id);
            return HEVC_DEC_ERR;
        }

        //if (HEVC_DEC_NORMAL != HEVC_SPSEqual(&sps_tmp, &(pHevcCtx->SPS[sps_seq_parameter_set_id])))
        {
            sps_tmp->is_refresh = 1;
            sps_tmp->valid = 1;
            memmove(&(pHevcCtx->pSPS[sps_seq_parameter_set_id]), sps_tmp, sizeof(HEVC_SEQ_PARAM_SET_S));
            memcpy(&pHevcCtx->SEISPS, &pHevcCtx->pSPS[sps_seq_parameter_set_id], sizeof(HEVC_SEQ_PARAM_SET_S));
        }
    }
    else
    {
        pHevcCtx->pSPS[sps_seq_parameter_set_id].video_parameter_set_id  = video_parameter_set_id;
        pHevcCtx->pSPS[sps_seq_parameter_set_id].sps_max_sub_layers_minus1  = sps_max_sub_layers_minus1;
        pHevcCtx->pSPS[sps_seq_parameter_set_id].sps_temporal_id_nesting_flag  = sps_temporal_id_nesting_flag;
        memcpy(&(pHevcCtx->pSPS[sps_seq_parameter_set_id].profile_tier_level),&(profile_tier_level),sizeof(HEVC_PROFILE_TIER_LEVEL_S));
        pHevcCtx->pSPS[sps_seq_parameter_set_id].seq_parameter_set_id  = sps_seq_parameter_set_id;

        if (HEVC_DEC_NORMAL != HEVC_ProcessSPS(pHevcCtx, &(pHevcCtx->pSPS[sps_seq_parameter_set_id])))
        {
            dprint( "SPS[%d] decode error.\n", sps_seq_parameter_set_id);
            pHevcCtx->pSPS[sps_seq_parameter_set_id].is_refresh = 1;
            pHevcCtx->pSPS[sps_seq_parameter_set_id].valid = 0;
            return HEVC_DEC_ERR;
        }
        pHevcCtx->pSPS[sps_seq_parameter_set_id].is_refresh = 1;
        pHevcCtx->pSPS[sps_seq_parameter_set_id].valid = 1;
        memcpy(&pHevcCtx->SEISPS, &pHevcCtx->pSPS[sps_seq_parameter_set_id], sizeof(HEVC_SEQ_PARAM_SET_S));
    }

    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_ProcessPPS(HEVC_CTX_S *pHevcCtx, HEVC_PIC_PARAM_SET_S *pPPS)
{
    HEVC_SEQ_PARAM_SET_S *pSPS;
    SINT32 ret;
    SINT32 i;

    pPPS->loop_filter_across_tiles_enabled_flag = 1;
    pPPS->num_tile_columns = 1;
    pPPS->num_tile_rows = 1;
    pPPS->seq_parameter_set_id = HEVC_ue_v(pHevcCtx->pBS, "pps_seq_parameter_set_id");
    if (pPPS->seq_parameter_set_id < 0 || pPPS->seq_parameter_set_id >= (pHevcCtx->MaxPpsNum))
    {
        dprint( "pic_parameter_set_id(%d) out of range[0,15].\n", pPPS->seq_parameter_set_id);
        return HEVC_DEC_ERR;
    }
    else if (pHevcCtx->pSPS[pPPS->seq_parameter_set_id].valid == 0)
    {
        dprint( "SPS(%d) haven't decode.\n", pPPS->seq_parameter_set_id);    
        return HEVC_DEC_ERR;
    }

    pSPS = &pHevcCtx->pSPS[pPPS->seq_parameter_set_id];

    pPPS->dependent_slice_segments_enabled_flag  = HEVC_u_v(pHevcCtx->pBS, 1, "dependent_slice_segments_enabled_flag");
    pPPS->output_flag_present_flag               = HEVC_u_v(pHevcCtx->pBS, 1, "output_flag_present_flag");
    pPPS->num_extra_slice_header_bits            = HEVC_u_v(pHevcCtx->pBS, 3, "num_extra_slice_header_bits");   	
    pPPS->sign_data_hiding_flag                  = HEVC_u_v(pHevcCtx->pBS, 1, "sign_data_hiding_flag");
    pPPS->cabac_init_present_flag                = HEVC_u_v(pHevcCtx->pBS, 1, "cabac_init_present_flag");
    
    pPPS->num_ref_idx_l0_default_active = 1 + HEVC_ue_v(pHevcCtx->pBS, "num_ref_idx_l0_default_active_minus1");
    if (pPPS->num_ref_idx_l0_default_active < 0 || pPPS->num_ref_idx_l0_default_active > 15)
    {
        dprint( "num_ref_idx_l0_default_active out of range[0,15].\n");
        return HEVC_DEC_ERR;
    }

    pPPS->num_ref_idx_l1_default_active = 1 + HEVC_ue_v(pHevcCtx->pBS, "num_ref_idx_l1_default_active_minus1");
    if (pPPS->num_ref_idx_l1_default_active < 0 || pPPS->num_ref_idx_l1_default_active > 15)
    {
        dprint( "num_ref_idx_l1_default_active out of range[0,15].\n");
        return HEVC_DEC_ERR;
    }

    pPPS->pic_init_qp                   = 26 + HEVC_se_v(pHevcCtx->pBS, "init_qp_minus26");
    if (pPPS->pic_init_qp < (SINT32)(-(pSPS->qp_bdOffset_y)) || pPPS->pic_init_qp > 51)
    {
        dprint( "pic_init_qp(%d) out of range[-%d,51].\n", pPPS->pic_init_qp, (pSPS->qp_bdOffset_y));
        return HEVC_DEC_ERR;
    }

    pPPS->constrained_intra_pred_flag   = HEVC_u_v(pHevcCtx->pBS, 1, "constrained_intra_pred_flag");
    pPPS->transform_skip_enabled_flag   = HEVC_u_v(pHevcCtx->pBS, 1, "transform_skip_enabled_flag");
    pPPS->cu_qp_delta_enabled_flag      = HEVC_u_v(pHevcCtx->pBS, 1, "cu_qp_delta_enabled_flag");
    
    if (pPPS->cu_qp_delta_enabled_flag)
    {
        pPPS->diff_cu_qp_delta_depth    = HEVC_ue_v(pHevcCtx->pBS, "diff_cu_qp_delta_depth");
    }
    else
    {
        pPPS->diff_cu_qp_delta_depth    = 0;
    }

    if (pPPS->diff_cu_qp_delta_depth < 0 || pPPS->diff_cu_qp_delta_depth > 3)
    {
        dprint( "diff_cu_qp_delta_depth out of range[0,3].\n");
        return HEVC_DEC_ERR;	
    }
    pPPS->max_cu_qp_delta_depth         = pPPS->diff_cu_qp_delta_depth;

    pPPS->pic_cb_qp_offset              = HEVC_se_v(pHevcCtx->pBS, "pps_cb_qp_offset");
    if (pPPS->pic_cb_qp_offset < -12 || pPPS->pic_cb_qp_offset > 12)
    {
        dprint( "pic_cb_qp_offset out of range[-12,12].\n");
        return HEVC_DEC_ERR;	
    }

    pPPS->pic_cr_qp_offset              = HEVC_se_v(pHevcCtx->pBS, "pps_cr_qp_offset");
    if (pPPS->pic_cr_qp_offset < -12 || pPPS->pic_cr_qp_offset > 12)
    {
        dprint( "pic_cr_qp_offset out of range[-12,12].\n");
        return HEVC_DEC_ERR;	
    }

    pPPS->pic_slice_chroma_qp_offsets_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "pps_slice_chroma_qp_offsets_present_flag");
    pPPS->weighted_pred_flag                       = HEVC_u_v(pHevcCtx->pBS, 1, "weighted_pred_flag");
    pPPS->weighted_bipred_flag                     = HEVC_u_v(pHevcCtx->pBS, 1, "weighted_bipred_flag");
    pPPS->transquant_bypass_enable_flag            = HEVC_u_v(pHevcCtx->pBS, 1, "transquant_bypass_enable_flag");
    pPPS->tiles_enabled_flag                       = HEVC_u_v(pHevcCtx->pBS, 1, "tiles_enabled_flag");
    pPPS->entropy_coding_sync_enabled_flag         = HEVC_u_v(pHevcCtx->pBS, 1, "entropy_coding_sync_enabled_flag");

    if (pPPS->tiles_enabled_flag)
    {
        pPPS->num_tile_columns          = HEVC_ue_v(pHevcCtx->pBS, "num_tile_columns_minus1") + 1;
        if (pPPS->num_tile_columns <= 0 || pPPS->num_tile_columns > HEVC_MAX_TILE_COLUMNS)
        {
            dprint( "num_tile_columns(%d) out of range(0,%d).\n", pPPS->num_tile_columns, HEVC_MAX_TILE_COLUMNS);
            return HEVC_DEC_ERR;
        }

        pPPS->num_tile_rows             = HEVC_ue_v(pHevcCtx->pBS, "num_tile_rows_minus1") + 1;
        if (pPPS->num_tile_rows <= 0 || pPPS->num_tile_rows > HEVC_MAX_TILE_ROWS)
        {
            dprint( "num_tile_rows out(%d) of range(0,%d).\n", pPPS->num_tile_rows, HEVC_MAX_TILE_ROWS);
            return HEVC_DEC_ERR;
        }

        pPPS->uniform_spacing_flag      = HEVC_u_v(pHevcCtx->pBS, 1, "uniform_spacing_flag");

        if (!pPPS->uniform_spacing_flag)
        {
            for (i=0; i<(pPPS->num_tile_columns-1); i++)
            {
                pPPS->column_width[i] = HEVC_ue_v(pHevcCtx->pBS, "column_width_minus1") + 1;
                // 宽度约束
                if (pPPS->column_width[i] < 0 || pPPS->column_width[i] > pSPS->ctb_num_width )
                {
                    dprint("column_width[%d](%d) out of range (0 %d)\n", i,pPPS->column_width[i],pSPS->ctb_num_width);	
                    return HEVC_DEC_ERR;
                }
            }
            for (i=0; i<(pPPS->num_tile_rows-1); i++)
            {
                pPPS->row_height[i] = HEVC_ue_v(pHevcCtx->pBS, "row_height_minus1") + 1;
                // 高度约束	
                if (pPPS->row_height[i] < 0 || pPPS->row_height[i] > pSPS->ctb_num_height )
                {
                    dprint("row_height[%d](%d) out of range (0 %d)\n", i,pPPS->row_height[i],pSPS->ctb_num_height);
                    return HEVC_DEC_ERR;
                }
            }
        }

        if (pPPS->uniform_spacing_flag)
        {
            for (i=0; i<pPPS->num_tile_columns; i++)
            {
                pPPS->column_bd[i] = (i+1)*pSPS->ctb_num_width/pPPS->num_tile_columns - (i*pSPS->ctb_num_width)/pPPS->num_tile_columns;
            }
            for (i=0; i<pPPS->num_tile_rows; i++)
            {
                pPPS->row_bd[i] = (i+1)*pSPS->ctb_num_height/pPPS->num_tile_rows - (i*pSPS->ctb_num_height)/pPPS->num_tile_rows;
            }
        }
        else
        {
            pPPS->column_bd[pPPS->num_tile_columns-1] = pSPS->ctb_num_width;
            for (i=0; i<pPPS->num_tile_columns-1; i++)
            {
                pPPS->column_bd[i] = pPPS->column_width[i];
                pPPS->column_bd[pPPS->num_tile_columns-1] -= pPPS->column_bd[i];
            }

            if (pPPS->column_bd[pPPS->num_tile_columns-1] <= 0)
            {
                dprint("column_bd[%d](%d) <= 0, invalid!\n", pPPS->num_tile_columns-1, pPPS->column_bd[pPPS->num_tile_columns-1]);	
                return HEVC_DEC_ERR;
            }

            pPPS->row_bd[pPPS->num_tile_rows-1] = pSPS->ctb_num_height;
            for (i=0; i<pPPS->num_tile_rows-1; i++)
            {
                pPPS->row_bd[i] = pPPS->row_height[i];
                pPPS->row_bd[pPPS->num_tile_rows-1] -= pPPS->row_bd[i];
            }

            if (pPPS->row_bd[pPPS->num_tile_rows-1] <= 0)
            {
                dprint("row_bd[%d](%d) <= 0, invalid!\n", pPPS->num_tile_rows-1, pPPS->row_bd[pPPS->num_tile_rows-1]);	
                return HEVC_DEC_ERR;
            }

        }
        
        if (pPPS->num_tile_columns != 1 || pPPS->num_tile_rows != 1)
        {
            pPPS->loop_filter_across_tiles_enabled_flag = HEVC_u_v(pHevcCtx->pBS, 1, "loop_filter_across_tiles_enabled_flag");
        }
    }
    else
    {
        pPPS->column_bd[0] = pSPS->ctb_num_width;
        pPPS->row_bd[0] = pSPS->ctb_num_height;
    }
    
    for (i=0; i<pPPS->num_tile_columns; i++)
    {            
        if(pPPS->column_bd[i]*pSPS->max_cu_width <= 64 && pSPS->pic_width_in_luma_samples > 64)
        {
            dprint( "PPS tile width(%d) is too small.(Logic Unsupport)\n", pPPS->column_bd[i]*pSPS->max_cu_width);        
            return HEVC_DEC_ERR;
        }
    }
    for (i=0; i<pPPS->num_tile_rows; i++)
    {            
        if(pPPS->row_bd[i]*pSPS->max_cu_width < 64 )
        {
            dprint( "PPS tile height(%d) is too small.(Logic Unsupport)\n", pPPS->row_bd[i]*pSPS->max_cu_width);   
            return HEVC_DEC_ERR;
        }
    }

    pPPS->loop_filter_across_slices_enabled_flag  = HEVC_u_v(pHevcCtx->pBS, 1, "loop_filter_across_slices_enabled_flag");
    pPPS->deblocking_filter_control_present_flag  = HEVC_u_v(pHevcCtx->pBS, 1, "deblocking_filter_control_present_flag");
    if (pPPS->deblocking_filter_control_present_flag)
    {
        pPPS->deblocking_filter_override_enabled_flag = HEVC_u_v(pHevcCtx->pBS, 1, "deblocking_filter_override_enabled_flag");
        pPPS->pic_disable_deblocking_filter_flag      = HEVC_u_v(pHevcCtx->pBS, 1, "pps_disable_deblocking_filter_flag");
        if (!pPPS->pic_disable_deblocking_filter_flag)
        {
            pPPS->pps_beta_offset_div2 = HEVC_se_v(pHevcCtx->pBS, "pps_beta_offset_div2");
            if (pPPS->pps_beta_offset_div2 < -6 || pPPS->pps_beta_offset_div2 > 6)
            {
                dprint( "pps_beta_offset_div2(%d) out of range(-6,6).\n", pPPS->pps_beta_offset_div2);
                pPPS->pps_beta_offset_div2 = 0; //赋值强解
                //return HEVC_DEC_ERR;		
            }
            pPPS->pps_tc_offset_div2   = HEVC_se_v(pHevcCtx->pBS, "pps_tc_offset_div2");
            if (pPPS->pps_tc_offset_div2 < -6 || pPPS->pps_tc_offset_div2 > 6)
            {
                dprint( "pps_tc_offset_div2(%d) out of range(-6,6).\n", pPPS->pps_tc_offset_div2);
                pPPS->pps_tc_offset_div2 = 0; //赋值强解	
                // return HEVC_DEC_ERR;			
            }
        }
    }
    
    pPPS->pic_scaling_list_data_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "pps_scaling_list_data_present_flag");
    if (pPPS->pic_scaling_list_data_present_flag)
    {
        ret = HEVC_DecScalingListData(pHevcCtx,&(pPPS->scaling_list));
        if (HEVC_DEC_NORMAL != ret)
        {
            dprint( "PPS HEVC_DecScalingListData error.\n");
            return HEVC_DEC_ERR;
        }
    }
    pPPS->lists_modification_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "lists_modification_present_flag");
    pPPS->log2_parallel_merge_level = HEVC_ue_v(pHevcCtx->pBS, "log2_parallel_merge_level_minus2") + 2;
    if (pPPS->log2_parallel_merge_level < 2 || pPPS->log2_parallel_merge_level > pSPS->log2_ctb_sizeY)
    {
        dprint( "log2_parallel_merge_level(%d) out of range(2,log2_ctb_sizeY).\n", pPPS->log2_parallel_merge_level);
        return HEVC_DEC_ERR;	
    }
    pPPS->slice_segment_header_extension_present_flag = HEVC_u_v(pHevcCtx->pBS, 1, "slice_segment_header_extension_present_flag");
    pPPS->pps_extension_flag        = HEVC_u_v(pHevcCtx->pBS, 1, "pps_extension_flag");

    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_DecPPS(HEVC_CTX_S *pHevcCtx)
{
    UINT32 pic_parameter_set_id;
    HEVC_PIC_PARAM_SET_S *pps_tmp = NULL;
    
    pos();

    pps_tmp = &pHevcCtx->TmpParam.PpsTmp;
    memset(pps_tmp, 0, sizeof(HEVC_PIC_PARAM_SET_S));

    pic_parameter_set_id = HEVC_ue_v(pHevcCtx->pBS, "pps_pic_parameter_set_id");
    if (pic_parameter_set_id >= (pHevcCtx->MaxPpsNum))
    {
        dprint( "pic_parameter_set_id(%d) out of range(0,63).\n", pic_parameter_set_id);
        return HEVC_DEC_ERR;
    }
    if (pHevcCtx->pPPS[pic_parameter_set_id].valid) 
    {
        pps_tmp->pic_parameter_set_id = pic_parameter_set_id;
        if (HEVC_DEC_NORMAL != HEVC_ProcessPPS(pHevcCtx, pps_tmp))
        {
            dprint( "PPS[%d] decode error.\n", pic_parameter_set_id);
            return HEVC_DEC_ERR;
        }

        //if (HEVC_DEC_NORMAL != HEVC_PPSEqual(&pps_tmp, &(pHevcCtx->PPS[pic_parameter_set_id])))
        {
            pps_tmp->is_refresh = 1;
            pps_tmp->valid = 1;
            memmove(&(pHevcCtx->pPPS[pic_parameter_set_id]), pps_tmp, sizeof(HEVC_PIC_PARAM_SET_S));
        }
    }
    else
    {
        pHevcCtx->pPPS[pic_parameter_set_id].pic_parameter_set_id = pic_parameter_set_id;
        if (HEVC_DEC_NORMAL != HEVC_ProcessPPS(pHevcCtx, &(pHevcCtx->pPPS[pic_parameter_set_id])))
        {
            dprint( "PPS[%d] decode error.\n", pic_parameter_set_id);
            pHevcCtx->pPPS[pic_parameter_set_id].is_refresh = 1;
            pHevcCtx->pPPS[pic_parameter_set_id].valid = 0;
            return HEVC_DEC_ERR;
        }
        pHevcCtx->pPPS[pic_parameter_set_id].is_refresh = 1;
        pHevcCtx->pPPS[pic_parameter_set_id].valid = 1;
    }

    return HEVC_DEC_NORMAL;
}


static VOID HEVC_ApplyReferencePictureSet(HEVC_CTX_S *pHevcCtx)
{
    SINT32 i, j;
    SINT32 CurPoc;
    SINT32 RefPoc;
    SINT32 is_reference;
    SINT32 PocCycle;
    HEVC_SHORT_TERM_RPSET_S *pTempRps = NULL;
    HEVC_PIC_PARAM_SET_S    *pPPS = NULL;
    HEVC_SEQ_PARAM_SET_S    *pSPS = NULL;

    pPPS = &pHevcCtx->pPPS[pHevcCtx->CurrSlice.pic_parameter_set_id];
    pSPS = &pHevcCtx->pSPS[pPPS->seq_parameter_set_id];
    PocCycle = 1 << pSPS->max_pic_order_cnt_lsb;

    pTempRps = &(pHevcCtx->CurrSlice.short_term_ref_pic_set);
    for (i=0; i<pHevcCtx->DPB.used_size; i++)
    {
        is_reference = 0;

        // long term reference
        for (j=pTempRps->num_negative_pics+pTempRps->num_positive_pics; j<pTempRps->num_of_pics; j++)
        {
            if (pHevcCtx->CurrSlice.CheckLTMSB[j])
            {
                if ((pHevcCtx->DPB.fs[i]->frame.is_long_term) &&
                    (pHevcCtx->DPB.fs[i]->poc == pTempRps->poc[j]))
                {
                    is_reference = 1;
                    pHevcCtx->DPB.fs[i]->is_reference = 1;
                    pHevcCtx->DPB.fs[i]->eFrameStoreState = FS_IN_DPB;
                }
            }
            else if( (pHevcCtx->DPB.fs[i]->frame.is_long_term) 
                 && ((pHevcCtx->DPB.fs[i]->poc % PocCycle) == (pTempRps->poc[j] % PocCycle)) )
            {
                is_reference = 1;
                pHevcCtx->DPB.fs[i]->is_reference = 1;
                pHevcCtx->DPB.fs[i]->eFrameStoreState = FS_IN_DPB;
            }

            if(0 == is_reference)
            { 
                CurPoc = pHevcCtx->DPB.fs[i]->poc;
                RefPoc = pTempRps->poc[j];
                
                if(!pHevcCtx->CurrSlice.CheckLTMSB[j])
                {
                    CurPoc = CurPoc % PocCycle;
                    RefPoc = RefPoc % PocCycle;
                }

                if(pHevcCtx->DPB.fs[i]->is_reference && CurPoc == RefPoc)
                {
                    is_reference = 1;
                    pHevcCtx->DPB.fs[i]->is_reference = 1;
                    pHevcCtx->DPB.fs[i]->frame.is_long_term = 1;
                    pHevcCtx->DPB.fs[i]->eFrameStoreState = FS_IN_DPB;
                }
            }
        }
        
        // short term reference
        for (j=0; j<(pTempRps->num_negative_pics + pTempRps->num_positive_pics); j++)
        {
            if ((!pHevcCtx->DPB.fs[i]->frame.is_long_term) &&
                (pHevcCtx->DPB.fs[i]->poc == (pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[j])))
            {
                is_reference = 1;
                pHevcCtx->DPB.fs[i]->is_reference = 1;
                pHevcCtx->DPB.fs[i]->eFrameStoreState = FS_IN_DPB;
            }
        }
        
        // mark the picture as "unused for reference" if it is not in
        // the Reference Picture Set
        if ((pHevcCtx->DPB.fs[i]->poc != pHevcCtx->CurrSlice.poc) && (0 == is_reference))
        {
            pHevcCtx->DPB.fs[i]->is_reference = 0;
            pHevcCtx->DPB.fs[i]->frame.is_long_term = 0;
        }
    }

    return;
}

static VOID HEVC_UpdateLTReflist(HEVC_CTX_S *pHevcCtx)
{
    SINT32 i=0, j=0, k=0;
    SINT32 RpsPoc;
    SINT32 CurPoc;
    UINT8  DeltaPocMsbCycleLtFlag;
    SINT32 PocCycle;
    HEVC_SHORT_TERM_RPSET_S *pRps = NULL;
    HEVC_PIC_PARAM_SET_S    *pPPS = NULL;
    HEVC_SEQ_PARAM_SET_S    *pSPS = NULL;

    pPPS = &pHevcCtx->pPPS[pHevcCtx->CurrSlice.pic_parameter_set_id];
    pSPS = &pHevcCtx->pSPS[pPPS->seq_parameter_set_id];
    PocCycle = 1 << pSPS->max_pic_order_cnt_lsb;

    pRps = &(pHevcCtx->CurrSlice.short_term_ref_pic_set);
    for (i=(SINT32)pRps->num_of_pics-1; i>(SINT32)(pRps->num_negative_pics+pRps->num_positive_pics-1); i--)
    {
        if (pRps->used_flag[i])
        {
            RpsPoc = pRps->poc[i];
            DeltaPocMsbCycleLtFlag = pHevcCtx->CurrSlice.CheckLTMSB[i];
            RpsPoc = DeltaPocMsbCycleLtFlag ? RpsPoc : RpsPoc&(PocCycle-1);

            for (j=0; j<(SINT32)pHevcCtx->DPB.used_size; j++)
            {
                CurPoc = pHevcCtx->DPB.fs[j]->poc;
                CurPoc = DeltaPocMsbCycleLtFlag ? CurPoc : CurPoc&(PocCycle-1);
                if (CurPoc == RpsPoc)
                {
                    pHevcCtx->DPB.fs[j]->is_reference = 1;

                    pHevcCtx->DPB.fs[j]->frame.is_short_term = 0;
                    pHevcCtx->DPB.fs[j]->frame.is_long_term  = 1;
                    pHevcCtx->DPB.fs_ltref[k++] = pHevcCtx->DPB.fs[j];
                    break;
                }
            }
        }
    }
    pHevcCtx->DPB.ltref_frames_in_buffer = k;
    while (k<(SINT32)pHevcCtx->DPB.size)
    {
        pHevcCtx->DPB.fs_ltref[k++] = (HEVC_FRAMESTORE_S *)NULL;
    }
    
    return;
}

static VOID HEVC_UpdateReflist(HEVC_CTX_S *pHevcCtx)
{
    UINT32 i=0, j=0, k=0;
    HEVC_SHORT_TERM_RPSET_S *pRps = NULL;
    SINT32 poc;
    SINT32 val;
        
    pRps = &(pHevcCtx->CurrSlice.short_term_ref_pic_set);
    for (i=0; i<pRps->num_negative_pics; i++)
    {
        if (pRps->used_flag[i])
        {
            poc = pHevcCtx->CurrSlice.poc + pRps->delta_poc[i];
            for (j=0; j<pHevcCtx->DPB.used_size; j++)
            {
                if (poc == pHevcCtx->DPB.fs[j]->poc)
                {
                    pHevcCtx->DPB.fs[j]->is_reference = 1;
                    pHevcCtx->DPB.fs[j]->frame.is_short_term = 1;
                    pHevcCtx->DPB.fs[j]->frame.is_long_term  = 0;
                    pHevcCtx->DPB.fs_negative_ref[k++] = pHevcCtx->DPB.fs[j];
                    break;
                }
            }
        }
    }
    pHevcCtx->DPB.negative_ref_frames_in_buffer = k;
    while (k<(SINT32)pHevcCtx->DPB.size)
    {
        pHevcCtx->DPB.fs_negative_ref[k++] = (HEVC_FRAMESTORE_S *)NULL;
    }

    k = 0;
    val = pRps->num_negative_pics+pRps->num_positive_pics;
    for (; i<val; i++)
    {
        if (pRps->used_flag[i])
        {
            poc = pHevcCtx->CurrSlice.poc + pRps->delta_poc[i];
            for (j=0; j<pHevcCtx->DPB.used_size; j++)
            {
                if (poc == pHevcCtx->DPB.fs[j]->poc)
                {
                    pHevcCtx->DPB.fs[j]->is_reference = 1;
                    pHevcCtx->DPB.fs[j]->frame.is_short_term = 1;
                    pHevcCtx->DPB.fs[j]->frame.is_long_term  = 0;
                    pHevcCtx->DPB.fs_positive_ref[k++] = pHevcCtx->DPB.fs[j];
                    break;                   
                }
            }
        }
    }
    pHevcCtx->DPB.positive_ref_frames_in_buffer = k;

    while (k<pHevcCtx->DPB.size)
    {
        pHevcCtx->DPB.fs_positive_ref[k++] = (HEVC_FRAMESTORE_S *)NULL;
    }
    
    return;
}

static SINT32 HEVC_CheckThatAllRefPicsAreAvailable(HEVC_CTX_S *pHevcCtx, SINT32* piLostPoc)
{
    SINT32 i, j, isAvailable;
    SINT32 atLeastOneLost = 0;
    SINT32 atLeastOneRemoved = 0;
    SINT32 iPocLost = 0;
    SINT32 PocCycle;
    HEVC_SHORT_TERM_RPSET_S *pTempRps = NULL;
    HEVC_PIC_PARAM_SET_S    *pPPS = NULL;
    HEVC_SEQ_PARAM_SET_S    *pSPS = NULL;

    pPPS = &pHevcCtx->pPPS[pHevcCtx->CurrSlice.pic_parameter_set_id];
    pSPS = &pHevcCtx->pSPS[pPPS->seq_parameter_set_id];
    PocCycle = 1 << pSPS->max_pic_order_cnt_lsb;
  
    pTempRps = &(pHevcCtx->CurrSlice.short_term_ref_pic_set);

    // loop through all long-term pictures in the Reference Picture Set
    // to see if the picture should be kept as reference picture
    for(i = pTempRps->num_negative_pics+pTempRps->num_positive_pics; i < pTempRps->num_of_pics; i++)
    {
        isAvailable = 0;
        // loop through all pictures in the reference picture buffer
        for (j = 0; j < pHevcCtx->DPB.used_size; j++)
        {
            if(pHevcCtx->CurrSlice.CheckLTMSB[i])
            {
                if(pHevcCtx->DPB.fs[j]->frame.is_long_term && pHevcCtx->DPB.fs[j]->poc == pTempRps->poc[i] && pHevcCtx->DPB.fs[j]->is_reference)
                {
                    isAvailable = 1;
                }
            }
            else 
            {
                if(pHevcCtx->DPB.fs[j]->frame.is_long_term && ((pHevcCtx->DPB.fs[j]->poc % PocCycle) == (pTempRps->poc[i] % PocCycle)) && pHevcCtx->DPB.fs[j]->is_reference)
                {
                    isAvailable = 1;
                }
            }
        }
              
        // report that a picture is lost if it is in the Reference Picture Set
        // but not available as reference picture
        if(isAvailable == 0)    
        {            
            if (pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[i] >= pHevcCtx->PocRandomAccess)
            {
                if(!pTempRps->used_flag[i])
                {
                    //dprint(PRN_ERROR, "Long-term reference picture with POC = %3d seems to have been removed or not correctly decoded.\n", pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[i]);
                    atLeastOneRemoved = 1;
                }
                else
                {
                    //dprint(PRN_ERROR, "Long-term reference picture with POC = %3d is lost or not correctly decoded!\n", pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[i]);
                    //dprint(PRN_ERROR, "CurrSlice.poc=%d delta_poc[%d]=%d PocRandomAccess=%d\n", pHevcCtx->CurrSlice.poc, i, pTempRps->delta_poc[i], pHevcCtx->PocRandomAccess);
                    atLeastOneLost = 1;
                    iPocLost = pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[i];
                }
            }
        }
    }
    
    // loop through all short-term pictures in the Reference Picture Set
    // to see if the picture should be kept as reference picture
    for(i = 0; i < pTempRps->num_negative_pics+pTempRps->num_positive_pics; i++)
    {
        isAvailable = 0;
        // loop through all pictures in the reference picture buffer
        for (j = 0; j < pHevcCtx->DPB.used_size; j++)
        {
            if(!pHevcCtx->DPB.fs[j]->frame.is_long_term && pHevcCtx->DPB.fs[j]->poc == pHevcCtx->CurrSlice.poc+pTempRps->delta_poc[i] && pHevcCtx->DPB.fs[j]->is_reference)
            {
                isAvailable = 1;
            }
        }
        // report that a picture is lost if it is in the Reference Picture Set
        // but not available as reference picture
        if(isAvailable == 0)    
        {            
            if (pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[i] >= pHevcCtx->PocRandomAccess)
            {
                if(!pTempRps->used_flag[i])
                {
                    //dprint(PRN_ERROR, "Short-term reference picture with POC = %3d seems to have been removed or not correctly decoded.\n",  pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[i]);
                    atLeastOneRemoved = 1;
                }
                else
                {
                    //dprint(PRN_ERROR, "Short-term reference picture with POC = %3d is lost or not correctly decoded!\n", pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[i]);
                    atLeastOneLost = 1;
                    iPocLost = pHevcCtx->CurrSlice.poc + pTempRps->delta_poc[i];
                }
            }
        }
    }    
    
    if(atLeastOneLost)
    {
        *piLostPoc = iPocLost;
        return 0;
    }
    
    return 1;
}

static VOID HEVC_RemoveFrameStoreOutDPB(HEVC_CTX_S *pHevcCtx,UINT32 pos)
{
    UINT32 i;

    pHevcCtx->DPB.fs[pos]->eFrameStoreState = FS_NOT_USED;
    pHevcCtx->DPB.fs[pos]->is_reference = 0;
    pHevcCtx->DPB.fs[pos] = (HEVC_FRAMESTORE_S *)NULL;
    
    for (i=pos; i<= (pHevcCtx->DPB.used_size-1); i++)
    {
        pHevcCtx->DPB.fs[i] = pHevcCtx->DPB.fs[i+1];
    }
    pHevcCtx->DPB.fs[pHevcCtx->DPB.used_size-1] = (HEVC_FRAMESTORE_S *)NULL;
    pHevcCtx->DPB.used_size--;    

    return;    
}

static UINT32 HEVC_RemoveUnUsedFrameStore(HEVC_CTX_S *pHevcCtx)
{
    UINT32 i, j, record_used_size;
    SINT32 IsRemoved = HEVC_FALSE;

    record_used_size = pHevcCtx->DPB.used_size;
    for (i=0,j=0; i<record_used_size; i++)
    {
        if (0 == pHevcCtx->DPB.fs[j]->is_reference)
        {
            HEVC_RemoveFrameStoreOutDPB(pHevcCtx, j);
            IsRemoved = HEVC_TRUE;
        }
        else
        {
            j++;
        }
    }
    
    return IsRemoved;
}

static SINT32 HEVC_RefPicProcess(HEVC_CTX_S *pHevcCtx)
{
    SINT32 ret;
    SINT32 iLostPoc;

    HEVC_ApplyReferencePictureSet(pHevcCtx);
    HEVC_RemoveUnUsedFrameStore(pHevcCtx);
    
    ret = HEVC_CheckThatAllRefPicsAreAvailable(pHevcCtx, &iLostPoc);

    HEVC_UpdateReflist(pHevcCtx);
    HEVC_UpdateLTReflist(pHevcCtx);

    return HEVC_DEC_NORMAL;
}       

static VOID HEVC_PicTypeStatistic(HEVC_CTX_S *pHevcCtx)
{
    if(1 == HEVC_IsFlushUnit(pHevcCtx->CurrSlice.nal_unit_type))
    {
        pHevcCtx->CurrPic.pic_type = HEVC_IDR_FRAME;
    }
    else if (1 == HEVC_IsIUnit(pHevcCtx->CurrSlice.nal_unit_type))
    {
        pHevcCtx->CurrPic.pic_type = HEVC_I_FRAME;
    }
    else
    {
        switch (pHevcCtx->CurrSlice.slice_type)
        {
            case HEVC_B_SLICE:
                dprint("B_SLICE\n");
                pHevcCtx->CurrPic.pic_type = HEVC_B_FRAME;
                break;
            case HEVC_P_SLICE:
                dprint("P_SLICE\n");
                if (pHevcCtx->CurrPic.pic_type != HEVC_B_FRAME)
                {
                    pHevcCtx->CurrPic.pic_type = HEVC_P_FRAME;
                }
                break;
            case HEVC_I_SLICE:
                dprint("I_SLICE\n");
                if (pHevcCtx->CurrPic.pic_type != HEVC_B_FRAME 
                 && pHevcCtx->CurrPic.pic_type != HEVC_P_FRAME)
                {
                    pHevcCtx->CurrPic.pic_type = HEVC_I_FRAME;
                }
                break;
            default:
                pHevcCtx->CurrPic.pic_type = HEVC_ERR_FRAME;
                break;           
        }
    }
    
    return;
}

static SINT32 HEVC_AllocFrameStore(HEVC_CTX_S *pHevcCtx)
{
    UINT32 i = 0;

    pHevcCtx->CurrPic.state = HEVC_PIC_EMPTY;

    for (i=0; i<HEVC_MAX_FRAME_STORE; i++)
    {
        if (FS_NOT_USED == pHevcCtx->FrameStore[i].eFrameStoreState)
        {
            pHevcCtx->CurrPic.state                            = HEVC_PIC_DECODING;
            pHevcCtx->CurrPic.frame_store                      = &pHevcCtx->FrameStore[i];
            pHevcCtx->CurrPic.frame_store->non_existing        = 0;
            pHevcCtx->CurrPic.frame_store->frame.is_long_term  = 0;
            pHevcCtx->CurrPic.frame_store->frame.is_short_term = 0;
            break;
        }
    }
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_InitPic(HEVC_CTX_S *pHevcCtx)
{
    SINT32 ret;
    SINT32 PrevSpsId;
    SINT32 PicWidth, PicHeight, log2_ctb_sizeY;
    HEVC_VIDEO_PARAM_SET_S *vps      = NULL;
    HEVC_SEQ_PARAM_SET_S   *sps      = NULL;
    HEVC_PIC_PARAM_SET_S   *pps      = NULL;

    PrevSpsId = pHevcCtx->CurrSPS.seq_parameter_set_id;
    PicWidth = pHevcCtx->CurrSPS.pic_width_in_luma_samples;
    PicHeight = pHevcCtx->CurrSPS.pic_height_in_luma_samples;
    log2_ctb_sizeY = pHevcCtx->CurrSPS.log2_ctb_sizeY;
    
    if (pHevcCtx->CurrSlice.pic_parameter_set_id < 0 || pHevcCtx->CurrSlice.pic_parameter_set_id >= (pHevcCtx->MaxPpsNum))
    {
        dprint("CurrSlice.pic_parameter_set_id = %d\n", pHevcCtx->CurrSlice.pic_parameter_set_id);
        return HEVC_DEC_ERR;
    }
    
    pps = &pHevcCtx->pPPS[pHevcCtx->CurrSlice.pic_parameter_set_id];
    sps = &pHevcCtx->pSPS[pps->seq_parameter_set_id];
    vps = &pHevcCtx->pVPS[sps->video_parameter_set_id];

    if ((pHevcCtx->CurrVPS.video_parameter_set_id != vps->video_parameter_set_id)
        || (vps->is_refresh) || (!pHevcCtx->AllowStartDec))
    {
        vps->is_refresh = 0;
        memmove(&pHevcCtx->CurrVPS, vps, sizeof(HEVC_VIDEO_PARAM_SET_S));
    }

    if ((pHevcCtx->CurrSPS.seq_parameter_set_id != sps->seq_parameter_set_id)
        || (sps->is_refresh) || (!pHevcCtx->AllowStartDec)
        || PicWidth != sps->pic_width_in_luma_samples || PicHeight != sps->pic_height_in_luma_samples
        || log2_ctb_sizeY != sps->log2_ctb_sizeY)
    {
        sps->is_refresh = 0;
        memmove(&pHevcCtx->CurrSPS, sps, sizeof(HEVC_SEQ_PARAM_SET_S));
    }

    if ((pHevcCtx->CurrPPS.pic_parameter_set_id != pps->pic_parameter_set_id)
        || (pps->is_refresh) || (!pHevcCtx->AllowStartDec)
        || PicWidth != sps->pic_width_in_luma_samples || PicHeight != sps->pic_height_in_luma_samples
        || log2_ctb_sizeY != sps->log2_ctb_sizeY) 
    {
        pps->is_refresh = 0;
        memmove(&pHevcCtx->CurrPPS, pps, sizeof(HEVC_PIC_PARAM_SET_S));
    }
    
    ret = HEVC_AllocFrameStore(pHevcCtx);
    if (HEVC_DEC_NORMAL != ret)
    {
        dprint("HEVC_AllocFrameStore error\n");
        return HEVC_DEC_ERR;
    }

    pHevcCtx->CurrPic.state = HEVC_PIC_DECODING;
    pHevcCtx->CurrPic.thispoc = pHevcCtx->CurrSlice.poc;
    pHevcCtx->CurrPic.frame_store->poc = pHevcCtx->CurrSlice.poc;
    pHevcCtx->CurrPic.pic_type = HEVC_I_FRAME;
    pHevcCtx->TotalSliceNum = 0; 
    
    return HEVC_DEC_NORMAL;
}

static VOID Hevc_InitDecBuffers(HEVC_CTX_S *pHevcCtx)
{
    UINT32 i,j;

    pos();
    
    //Init DPB
    pHevcCtx->DPB.size = HEVC_MAX_DPB_NUM;
    pHevcCtx->DPB.used_size                     = 0;
    pHevcCtx->DPB.negative_ref_frames_in_buffer = 0;
    pHevcCtx->DPB.positive_ref_frames_in_buffer = 0;
    pHevcCtx->DPB.ltref_frames_in_buffer        = 0;
    pHevcCtx->DPB.max_long_term_pic_idx         = 0;

    for (i=0; i<HEVC_MAX_DPB_NUM; i++)
    {
        pHevcCtx->DPB.fs_ltref[i]        = NULL;
        pHevcCtx->DPB.fs_negative_ref[i] = NULL;
        pHevcCtx->DPB.fs_positive_ref[i] = NULL;
        pHevcCtx->DPB.fs[i]              = NULL;
    }

    //Init Frame store
    for (i=0; i<HEVC_MAX_FRAME_STORE; i++)
    {
        pHevcCtx->FrameStore[i].non_existing      = 0;
        pHevcCtx->FrameStore[i].eFrameStoreState  = FS_NOT_USED;   
        pHevcCtx->FrameStore[i].is_reference      = 0;   
        pHevcCtx->FrameStore[i].poc               = 0;   
        pHevcCtx->FrameStore[i].frame.frame_store = &pHevcCtx->FrameStore[i];   
    }

    //Init ListX
    for (i=0; i<2; i++)
    {
        for (j=0; j<HEVC_MAX_LIST_SIZE; j++)
        {
            pHevcCtx->pListX[i][j] = NULL;
        }
        pHevcCtx->CurrSlice.listXsize[i] = 0;
    }

    return;
}

static SINT32 compare_pic_by_poc_asc(VOID *arg1, VOID *arg2)
{
    if ((*(HEVC_FRAMESTORE_S**)arg1)->poc < (*(HEVC_FRAMESTORE_S**)arg2)->poc)
    {
        return -1;
    }
    
    if ((*(HEVC_FRAMESTORE_S**)arg1)->poc > (*(HEVC_FRAMESTORE_S**)arg2)->poc)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#define STKSIZ (8*sizeof(void*) - 2)

#define CUTOFF 8            /* testing shows that this is good value */

static void swap_kn (
    char *a,
    char *b,
    size_t width
    )
{
    char tmp;

    if ( a != b )
        while ( width-- ) {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
}

static void shortsort (
    char *lo,
    char *hi,
    size_t width,
    int (*comp)(void *, void *)
    )
{
    char *p, *max;

    while (hi > lo) {
        max = lo;
        for (p = lo+width; p <= hi; p += width) {
            if (comp(p, max) > 0) {
                max = p;
            }
        }
        swap_kn(max, hi, width);
        hi -= width;
    }
}

static void hevc_qsort (
    void *base,
    size_t num,
    size_t width,
    int (*comp)(void *, void *)
    )
{
    char *lo, *hi;             
    char *mid;                 
    char *loguy, *higuy;       
    size_t size;  
    char *lostk[STKSIZ], *histk[STKSIZ];
    int stkptr; 

    if (num < 2 || width == 0)
        return;  

    stkptr = 0; 

    lo = base;
    hi = (char *)base + width * (num-1);  

recurse:

    size = (hi - lo) / width + 1;  

    if (size <= CUTOFF) {
        shortsort(lo, hi, width, comp);
    }
    else {

        mid = lo + (size / 2) * width; 

        if (comp(lo, mid) > 0) {
            swap_kn(lo, mid, width);
        }
        if (comp(lo, hi) > 0) {
            swap_kn(lo, hi, width);
        }
        if (comp(mid, hi) > 0) {
            swap_kn(mid, hi, width);
        }

        loguy = lo;
        higuy = hi;

        for (;;) {

            if (mid > loguy) {
                do  {
                    loguy += width;
                } while (loguy < mid && comp(loguy, mid) <= 0);
            }

            if (mid <= loguy) {
                do  {
                    loguy += width;
                } while (loguy <= hi && comp(loguy, mid) <= 0);
            }
            
            do  {
                higuy -= width;
            } while (higuy > mid && comp(higuy, mid) > 0);

            if (higuy < loguy)
                break;

            swap_kn(loguy, higuy, width);

            if (mid == higuy)
                mid = loguy;

        }

        higuy += width;
        if (mid < higuy) {
            do  {
                higuy -= width;
            } while (higuy > mid && comp(higuy, mid) == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= width;
            } while (higuy > lo && comp(higuy, mid) == 0);
        }

        if ( higuy - lo >= hi - loguy ) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }  

            if (loguy < hi) {
                lo = loguy;
                goto recurse;
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;   
            }

            if (lo < higuy) {
                hi = higuy;
                goto recurse;   
            }
        }
    }

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse; 
    }
    else
        return;    
}

static SINT32 HEVC_InsertFrmInDPB(HEVC_CTX_S *pHevcCtx, UINT32 pos, HEVC_CURRPIC_S *pCurrPic)
{
    if (pos >= HEVC_MAX_DPB_NUM)
    {
        dprint("%s with invalid pos(%d)\n", __func__, pos);
        return HEVC_DEC_ERR;  
    }

    pHevcCtx->DPB.fs[pos] = pCurrPic->frame_store;
    pHevcCtx->DPB.fs[pos]->eFrameStoreState = FS_IN_DPB;
    pCurrPic->is_ref_idc = 1;
    pHevcCtx->DPB.fs[pos]->is_reference = (pCurrPic->is_ref_idc == 0)? 0: 1;
    pHevcCtx->DPB.fs[pos]->frame.frame_store = pCurrPic->frame_store;
    pHevcCtx->DPB.fs[pos]->pic_type = pHevcCtx->DPB.fs[pos]->frame.pic_type = pCurrPic->pic_type;
    pHevcCtx->DPB.fs[pos]->poc = pCurrPic->thispoc;
    pHevcCtx->DPB.fs[pos]->frame.poc = pCurrPic->thispoc;

    // DPB按照poc递增排序
    hevc_qsort((VOID *)pHevcCtx->DPB.fs, (pos+1), sizeof(HEVC_FRAMESTORE_S*), compare_pic_by_poc_asc);
    
    pHevcCtx->DPB.used_size++;
 
    pCurrPic->state = HEVC_PIC_EMPTY;

    return HEVC_DEC_NORMAL;    
}

static SINT32 HEVC_DecOrderProcess(HEVC_CTX_S *pHevcCtx)
{
    SINT32 ret;
    
    HEVC_RemoveUnUsedFrameStore(pHevcCtx);
    
    ret = HEVC_InsertFrmInDPB(pHevcCtx,pHevcCtx->DPB.used_size, &pHevcCtx->CurrPic);
    if (HEVC_DEC_NORMAL != ret)
    {
        dprint("%s call HEVC_InsertFrmInDPB Failed.\n", __func__);
        return HEVC_DEC_ERR;
    }

    return ret;
}

static SINT32 HEVC_StorePicInDPB(HEVC_CTX_S *pHevcCtx)
{
    SINT32 ret = HEVC_DEC_ERR;

    ret = HEVC_DecOrderProcess(pHevcCtx);
    if (HEVC_DEC_NORMAL != ret)
    {
        dprint("%s call HEVC_DecOrderProcess Failed\n", __func__);
    }

    return ret;
}

static SINT32 HEVC_WritePicMsg(HEVC_CTX_S *pHevcCtx)
{
    SINT32 cnt;
    pHevcCtx->HevcFrmPoc= pHevcCtx->CurrPic.thispoc;
    pHevcCtx->HevcFrmType = HEVC_ERR_FRAME;
    pHevcCtx->HevcRefNum = 0;
    for(cnt = 0; cnt < 16; cnt++)
    {
        pHevcCtx->HevcRefPoc[cnt] = 0;
    }
    dprint("\n\ng_HevcFrmPoc = %d\n", pHevcCtx->HevcFrmPoc);
   
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_WriteSliceMsg(HEVC_CTX_S *pHevcCtx)
{
    SINT32 i, m;
    SINT32 refPicOk;
    
    pHevcCtx->TotalSliceNum++;

    if(pHevcCtx->CurrSlice.slice_type == HEVC_I_SLICE)
    {
        dprint("HEVC_I_SLICE\n");
    }
    else if(pHevcCtx->CurrSlice.slice_type == HEVC_P_SLICE)
    {
        dprint("HEVC_P_SLICE\n");
    }
    else if(pHevcCtx->CurrSlice.slice_type == HEVC_B_SLICE)
    {
        dprint("HEVC_B_SLICE\n");
    }
    else
    {
        dprint("HEVC_NON_SLICE\n");
    }
    
    dprint("listXsize[0] = %d\n", pHevcCtx->CurrSlice.listXsize[0]);    
    for (i=0; i<pHevcCtx->CurrSlice.listXsize[0]; i++)
    {
            dprint("[%d]%d, ", i, pHevcCtx->pListX[0][i]->poc);
            
            refPicOk = 0;
            for(m=0;m<pHevcCtx->HevcRefNum;m++)
            {
                if(pHevcCtx->HevcRefPoc[m] == pHevcCtx->pListX[0][i]->poc)
                {
                    refPicOk = 1;
                    break;
                }
            }
            if(refPicOk == 0)
            {
                pHevcCtx->HevcRefPoc[pHevcCtx->HevcRefNum++] = pHevcCtx->pListX[0][i]->poc;
            }
    }
    if(pHevcCtx->CurrSlice.listXsize[0] > 0)
    {
        dprint("\n");
    }

    dprint("listXsize[1] = %d\n", pHevcCtx->CurrSlice.listXsize[1]);    
    for (i=0; i<pHevcCtx->CurrSlice.listXsize[1]; i++)
    {
            dprint("[%d]%d, ", i, pHevcCtx->pListX[1][i]->poc);

            refPicOk = 0;
            for(m=0;m<pHevcCtx->HevcRefNum;m++)
            {
                if(pHevcCtx->HevcRefPoc[m] == pHevcCtx->pListX[1][i]->poc)
                {
                    refPicOk = 1;
                    break;
                }
            }
            if(refPicOk == 0)
            {
                pHevcCtx->HevcRefPoc[pHevcCtx->HevcRefNum++] = pHevcCtx->pListX[1][i]->poc;
            }
    }
    if(pHevcCtx->CurrSlice.listXsize[1] > 0)
    {
        dprint("\n");
    }
        
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_InitListX(HEVC_CTX_S *pHevcCtx)
{
    HEVC_STORABLEPIC_S *pTempListX[2][HEVC_MAX_LIST_SIZE];
    HEVC_REF_PIC_LISTS_MODDIFICATION *pRPLModify;
    UINT32 i,idx,RefFrameNum;
    UINT32 NegRefFrameNum = 0;
    UINT32 PosRefFrameNum = 0;
    UINT32 LTRefFrameNum = 0;

    memset(pTempListX, 0, sizeof(pTempListX));

    idx = 0;
    for (i=0; i<pHevcCtx->DPB.negative_ref_frames_in_buffer; i++)
    {
        if ((1 == pHevcCtx->DPB.fs_negative_ref[i]->is_reference) &&
            (0 == pHevcCtx->DPB.fs_negative_ref[i]->frame.is_long_term) &&
            (1 == pHevcCtx->DPB.fs_negative_ref[i]->frame.is_short_term))
        {
            pTempListX[0][idx++] = &pHevcCtx->DPB.fs_negative_ref[i]->frame;
        }
    }
    NegRefFrameNum = idx;
    
    for (i=0; i<pHevcCtx->DPB.positive_ref_frames_in_buffer; i++)
    {
        if ((1 == pHevcCtx->DPB.fs_positive_ref[i]->is_reference) &&
            (0 == pHevcCtx->DPB.fs_positive_ref[i]->frame.is_long_term) &&
            (1 == pHevcCtx->DPB.fs_positive_ref[i]->frame.is_short_term))
        {
            pTempListX[0][idx++] = &pHevcCtx->DPB.fs_positive_ref[i]->frame;
        }
    }     
    PosRefFrameNum = idx - NegRefFrameNum;
    
    // long term handling
    for (i=0; i<pHevcCtx->DPB.ltref_frames_in_buffer; i++)
    {
        if ((1 == pHevcCtx->DPB.fs_ltref[i]->is_reference) &&
            (1 == pHevcCtx->DPB.fs_ltref[i]->frame.is_long_term) &&
            (0 == pHevcCtx->DPB.fs_ltref[i]->frame.is_short_term))
        {
            pTempListX[0][idx++] = &pHevcCtx->DPB.fs_ltref[i]->frame;
        }
    }
    LTRefFrameNum = idx - NegRefFrameNum - PosRefFrameNum;
    RefFrameNum = idx;

    if (RefFrameNum==0)
    {
        pHevcCtx->CurrSlice.listXsize[0] = 0;
        pHevcCtx->CurrSlice.listXsize[1] = 0;
        memset(pHevcCtx->pListX, 0, sizeof(pHevcCtx->pListX));
        
        return HEVC_DEC_NORMAL;
    }
    
    if (HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type)
    {
        idx = 0;
        for (i=0; i<PosRefFrameNum; i++)
        {
            pTempListX[1][idx++] = pTempListX[0][NegRefFrameNum + i];
        }
        for (i=0; i<NegRefFrameNum; i++)
        {
            pTempListX[1][idx++] = pTempListX[0][i];
        }
        for (i=0; i<LTRefFrameNum; i++)
        {
            pTempListX[1][idx++] = pTempListX[0][NegRefFrameNum + PosRefFrameNum + i];
        }
        //pHevcCtx->CurrSlice.listXsize[1] = list1Idx;
    }
    
    // set max size
    pHevcCtx->CurrSlice.listXsize[0] =  pHevcCtx->CurrSlice.num_ref_idx[0];
    pHevcCtx->CurrSlice.listXsize[1] =  pHevcCtx->CurrSlice.num_ref_idx[1]; 
    
    idx = 0;
    pRPLModify = &(pHevcCtx->CurrSlice.ref_pic_lists_modification);
    for (i=0; i<pHevcCtx->CurrSlice.listXsize[0]; i++)
    {
        idx = pRPLModify->ref_pic_list_modification_flag_l0?pRPLModify->list_entry_l0[i]:i%RefFrameNum;
        pHevcCtx->pListX[0][i] = pTempListX[0][idx];
    }

    if (HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type)
    {
        for (i=0; i<pHevcCtx->CurrSlice.listXsize[1]; i++)
        {
            idx = pRPLModify->ref_pic_list_modification_flag_l1?pRPLModify->list_entry_l1[i]:i%RefFrameNum;	
            pHevcCtx->pListX[1][i] = pTempListX[1][idx];        
        }

        //for generalized b slice
        if (pHevcCtx->CurrSlice.listXsize[1] == 0)
        {
            memcpy(pHevcCtx->pListX[1],pHevcCtx->pListX[0],sizeof(pHevcCtx->pListX[0]));
            pHevcCtx->CurrSlice.listXsize[1] = pHevcCtx->CurrSlice.listXsize[0];
        }
    }

    // set the unused list entries to a valid one
    for (i=pHevcCtx->CurrSlice.listXsize[0]; i<(HEVC_MAX_LIST_SIZE); i++)
    {
        pHevcCtx->pListX[0][i] = pHevcCtx->pListX[0][0];
    }
    for (i=pHevcCtx->CurrSlice.listXsize[1]; i<(HEVC_MAX_LIST_SIZE); i++)
    {
        pHevcCtx->pListX[1][i] = pHevcCtx->pListX[1][0];
    }
    
    // InitlistX over
    return HEVC_DEC_NORMAL;    
}

static SINT32 HEVC_RepairList(HEVC_CTX_S *pHevcCtx)
{
    SINT8  found = -1;
    UINT32 i, i_list, i_list_cnt;
    HEVC_STORABLEPIC_S *pRepairPic = (HEVC_STORABLEPIC_S *)NULL;

    switch (pHevcCtx->CurrSlice.slice_type)
    {
        case HEVC_P_SLICE:
            i_list_cnt = 1;
            break;
        case HEVC_B_SLICE:
            i_list_cnt = 2;
            break;
        case HEVC_I_SLICE:
            i_list_cnt = 0;
            break;
        default:
            dprint( "%s unkown slice type(%d).\n", __func__, pHevcCtx->CurrSlice.slice_type);
            return HEVC_DEC_ERR;
    }

    for (i_list=0; i_list<i_list_cnt; i_list++)
    {
        if (0 < pHevcCtx->CurrSlice.listXsize[i_list])
        {
            found = -1;
            for (i = 0; i < pHevcCtx->CurrSlice.listXsize[i_list]; i++)
            {
                if ((HEVC_STORABLEPIC_S *)NULL != pHevcCtx->pListX[i_list][i])
                {
                    found = i;
                    pRepairPic = pHevcCtx->pListX[i_list][i];
                    break;
                }
            }
            
            for (i=0; i < pHevcCtx->CurrSlice.listXsize[i_list]; i++)
            {
                if ((HEVC_STORABLEPIC_S *)NULL == pHevcCtx->pListX[i_list][i])
                {
                    dprint( "%s: pListX[%d][%d] = NULL, repair it with pos(%d).\n", __func__, i_list, i, found);
                    pHevcCtx->pListX[i_list][i] = pRepairPic;
                }
            }
        }
        else
        {
        }
    }

    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_DecList(HEVC_CTX_S *pHevcCtx)
{
    SINT32 ret;
    
    if (HEVC_I_SLICE == pHevcCtx->CurrSlice.slice_type)
    {
        pHevcCtx->CurrSlice.listXsize[0] = 0;
        pHevcCtx->CurrSlice.listXsize[1] = 0;
        return HEVC_DEC_NORMAL;
    }

    ret = HEVC_InitListX(pHevcCtx);
    if (HEVC_DEC_NORMAL != ret)
    {
        dprint( "HEVC_InitListX error.\n");
        return HEVC_DEC_ERR;       
    }

    ret = HEVC_RepairList(pHevcCtx);
    if (HEVC_DEC_NORMAL != ret)
    {
        dprint( "HEVC_RepairList error.\n");
        return HEVC_DEC_ERR;       
    }
    
    return HEVC_DEC_NORMAL;
}

static UINT8 HEVC_IsReferenceNalu(HEVC_CTX_S *pHevcCtx)
{
    return ((pHevcCtx->CurrSlice.nal_unit_type <= NAL_UNIT_RESERVED_VCL_R15) && (pHevcCtx->CurrSlice.nal_unit_type%2 != 0)) 
        || ((pHevcCtx->CurrSlice.nal_unit_type >= NAL_UNIT_CODED_SLICE_BLA_W_LP) && (pHevcCtx->CurrSlice.nal_unit_type <= NAL_UNIT_RESERVED_IRAP_VCL23));
}

static SINT32 HEVC_DecPredWeightTable(HEVC_CTX_S *pHevcCtx)
{
    UINT32 i, j;
    HEVC_SEQ_PARAM_SET_S    *pSPS = NULL;
    HEVC_PIC_PARAM_SET_S    *pPPS = NULL;
    HEVC_PRED_WEIGHT_TABLE  *pPwt = NULL;
    HEVC_PRED_WEIGHT_TABLE  Pwt;

    pPwt = &Pwt;
    pPPS = &pHevcCtx->pPPS[pHevcCtx->CurrSlice.pic_parameter_set_id];
    pSPS = &pHevcCtx->pSPS[pPPS->seq_parameter_set_id];

    if (((HEVC_P_SLICE == pHevcCtx->CurrSlice.slice_type) && (pPPS->weighted_pred_flag)) ||
        ((HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type) && (pPPS->weighted_bipred_flag)))
    {
        HEVC_ue_v(pHevcCtx->pBS, "luma_log2_weight_denom");
        
        if (0 != pSPS->chroma_format_idc)
        {
            HEVC_se_v(pHevcCtx->pBS, "delta_chroma_log2_weight_denom");
    }

        for (i=0; i<pHevcCtx->CurrSlice.num_ref_idx[0]; i++)
        {
            pPwt->luma_weight_l0_flag[i] = HEVC_u_v(pHevcCtx->pBS, 1, "luma_weight_l0_flag");
        }
        if (0 != pSPS->chroma_format_idc)
        {
            for (i=0; i<pHevcCtx->CurrSlice.num_ref_idx[0]; i++)
            {
                pPwt->chroma_weight_l0_flag[i] = HEVC_u_v(pHevcCtx->pBS, 1, "chroma_weight_l0_flag");
            }
        }

        for (i=0; i<pHevcCtx->CurrSlice.num_ref_idx[0]; i++)
        {
            if (pPwt->luma_weight_l0_flag[i])
            {
                HEVC_se_v(pHevcCtx->pBS, "delta_luma_weight_l0");
                HEVC_se_v(pHevcCtx->pBS, "luma_offset_l0");
            }
            if (0 != pSPS->chroma_format_idc)
            {
                if (pPwt->chroma_weight_l0_flag[i])
                {
                    for (j=0; j<2; j++)
                    {
                        HEVC_se_v(pHevcCtx->pBS, "delta_chroma_weight_l0");
                        HEVC_se_v(pHevcCtx->pBS, "delta_chroma_offset_l0");
                    }
                }
            }
        }

        if (HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type)
        {
            for (i=0; i<pHevcCtx->CurrSlice.num_ref_idx[1]; i++)
            {
                pPwt->luma_weight_l1_flag[i] = HEVC_u_v(pHevcCtx->pBS, 1, "luma_weight_l1_flag");
            }
            if (0 != pSPS->chroma_format_idc)
            {
                for (i=0; i<pHevcCtx->CurrSlice.num_ref_idx[1]; i++)
                {
                    pPwt->chroma_weight_l1_flag[i] = HEVC_u_v(pHevcCtx->pBS, 1, "chroma_weight_l1_flag");
                }
            }
            for (i=0; i<pHevcCtx->CurrSlice.num_ref_idx[1]; i++)
            {
                if (pPwt->luma_weight_l1_flag[i])
                {
                    HEVC_se_v(pHevcCtx->pBS, "delta_luma_weight_l1");
                    HEVC_se_v(pHevcCtx->pBS, "luma_offset_l1");
                }
                if (0 != pSPS->chroma_format_idc)
                {
                    if (pPwt->chroma_weight_l1_flag[i])
                    {
                        for (j=0; j<2; j++)
                        {
                            HEVC_se_v(pHevcCtx->pBS, "delta_chroma_weight_l1");
                            HEVC_se_v(pHevcCtx->pBS, "delta_chroma_offset_l1");
                        }
                    }
                }
            }
        }
    }
    
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_IsNewPic(HEVC_CTX_S *pHevcCtx)
{
    SINT32 ret = 0;
    
    if (HEVC_MAX_INT == pHevcCtx->PocRandomAccess)
    {
        if (pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_CRA ||
            pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_LP ||
            pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_N_LP ||
            pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_RADL )
        {
            pHevcCtx->PocRandomAccess = pHevcCtx->CurrSlice.poc;
        }
        else if ((pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_W_RADL) ||(pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_N_LP))
        {
            pHevcCtx->PocRandomAccess = -HEVC_MAX_INT;
        }
        else
        {
            return IS_SKIP_PIC;
        }
    }
    else if ((pHevcCtx->CurrSlice.poc < pHevcCtx->PocRandomAccess)&&
            (pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_RASL_R || pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_RASL_N))
    {
        //pHevcCtx->LastDisplayPoc++;
        //return IS_SKIP_PIC;
    }
    else if ((pHevcCtx->bNoRaslOutputFlag)&&
            (pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_RASL_R || pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_RASL_N))
    {
        //pHevcCtx->LastDisplayPoc++;
        //return IS_SKIP_PIC;        
    }

    /* check if picture should be skipped because of association with a previous BLA picture */
    if (pHevcCtx->PrevRAPIsBLA && pHevcCtx->CurrSlice.poc < pHevcCtx->PocCRA &&
        (pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_RASL_R || pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_RASL_N))
    {
        //pHevcCtx->LastDisplayPoc++;
        //return IS_SKIP_PIC;   
    }
    
    if (!pHevcCtx->CurrSlice.dependent_slice_segment_flag)
    {
        if (pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_W_RADL 
         || pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_N_LP)
        {
            pHevcCtx->PocCRA = pHevcCtx->CurrSlice.poc;
            pHevcCtx->PrevRAPIsBLA = HEVC_FALSE;
        }
        else if (pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_CRA)
        {
            pHevcCtx->PocCRA = pHevcCtx->CurrSlice.poc;
            pHevcCtx->PrevRAPIsBLA = HEVC_FALSE;            
        }
        else if (pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_LP
              || pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_RADL
              || pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_N_LP)
        {
            pHevcCtx->PocCRA = pHevcCtx->CurrSlice.poc;
            pHevcCtx->PrevRAPIsBLA = HEVC_TRUE;               
        }
    }
    
    ret |= pHevcCtx->CurrSlice.first_slice_segment_in_pic_flag;	

    return ret;
}

static UINT8 HEVC_GetRapPicFlag(HEVC_CTX_S *pHevcCtx)
{
    return pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_W_RADL
        || pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_N_LP
        || pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_N_LP
        || pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_RADL
        || pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_BLA_W_LP
        || pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_CRA;
}

static UINT8 HEVC_GetIdrPicFlag(HEVC_CTX_S *pHevcCtx)
{
    return pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_W_RADL 
        || pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_N_LP;
}

static SINT32 HEVC_DecRefPicListsModification(HEVC_CTX_S *pHevcCtx,HEVC_SLICE_SEGMENT_HEADER *pSlice, HEVC_REF_PIC_LISTS_MODDIFICATION *pRpListModification, SINT32 numRpsCurrTempList)
{
    UINT32 i, length, temp;

    temp = numRpsCurrTempList;
    length = 1;
    numRpsCurrTempList--;
    while (numRpsCurrTempList >>= 1)
    {
        length++;
    }

    pRpListModification->ref_pic_list_modification_flag_l0 = HEVC_u_v(pHevcCtx->pBS, 1, "ref_pic_list_modification_flag_l0");
    if (pRpListModification->ref_pic_list_modification_flag_l0 && (temp > 1))
    {
        for (i=0; i<pSlice->num_ref_idx[0]; i++)
        {
            pRpListModification->list_entry_l0[i] = HEVC_u_v(pHevcCtx->pBS, length, "list_entry_l0");
        }
    }
    
    if (HEVC_B_SLICE == pSlice->slice_type)
    {
        pRpListModification->ref_pic_list_modification_flag_l1 = HEVC_u_v(pHevcCtx->pBS, 1, "ref_pic_list_modification_flag_l1");
        if (pRpListModification->ref_pic_list_modification_flag_l1 && (temp > 1))
        {
            for (i=0; i<pSlice->num_ref_idx[1]; i++)
            {
                pRpListModification->list_entry_l1[i] = HEVC_u_v(pHevcCtx->pBS, length, "list_entry_l1");
            }
        }
    }

    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_DecSliceSegmentHeader(HEVC_CTX_S *pHevcCtx, UINT32 IsDecSlice)
{
    UINT32 RAPPicFlag=0, IDRPicFlag=0;
    SINT32 numCUs=0, maxParts=0, reqBitsOuter=0, reqBitsInner=0;
    UINT32 lcuAddress=0, innerAddress=0, sliceAddress=0, bitsForLongTermPicInSPS=0;
    UINT32 i=0, j=0, isSAOEnabled=0, isDBFEnabled=0;
    SINT32 numRpsCurrTempList=0, offset=0;
    SINT32 pocLsbLt=0, lt_idx_sps=0;
    SINT32 prevDeltaMSB=0, deltaPocMSBCycleLT=0;
    UINT32 MaxPocLsb=0, PocLsb=0, PocMsb=0;
    UINT32 PrePocLsb=0, PrePocMsb=0;
    SINT32 PocLTCurr=0;
    UINT32 MaxCollocatedRefIdx=0, MaxNumEntryPointOffsets=0;
    UINT32 numBits=0;
    UINT32 ue=0;
    SINT32 ret=0;
    SINT32 bitsLeft=0;

    HEVC_VIDEO_PARAM_SET_S  *pVPS = NULL;
    HEVC_SEQ_PARAM_SET_S    *pSPS = NULL;
    HEVC_PIC_PARAM_SET_S    *pPPS = NULL;
    HEVC_SHORT_TERM_RPSET_S *pRPS = NULL;

    pos();

    pHevcCtx->CurrSlice.slice_type = HEVC_ERR_SLICE;
    
    pHevcCtx->CurrSlice.first_slice_segment_in_pic_flag = HEVC_u_v(pHevcCtx->pBS, 1, "first_slice_segment_in_pic_flag");

    RAPPicFlag = HEVC_GetRapPicFlag(pHevcCtx);
    IDRPicFlag = HEVC_GetIdrPicFlag(pHevcCtx);

    if (RAPPicFlag)
    {
        pHevcCtx->CurrSlice.no_output_of_prior_pics_flag = HEVC_u_v(pHevcCtx->pBS, 1, "no_output_of_prior_pics_flag");
    }
    
    pHevcCtx->CurrSlice.pic_parameter_set_id = HEVC_ue_v(pHevcCtx->pBS, "slice_pic_parameter_set_id");
    if (pHevcCtx->CurrSlice.pic_parameter_set_id < 0 || pHevcCtx->CurrSlice.pic_parameter_set_id >= (pHevcCtx->MaxPpsNum)) 
    {
        dprint( "%s: pic_parameter_set_id(%d) out of range(0 %d)!\n", __func__, pHevcCtx->CurrSlice.pic_parameter_set_id, pHevcCtx->MaxPpsNum);
        return HEVC_DEC_ERR;
    }

    pPPS = &pHevcCtx->pPPS[pHevcCtx->CurrSlice.pic_parameter_set_id];
    pSPS = &pHevcCtx->pSPS[pPPS->seq_parameter_set_id];
    pVPS = &pHevcCtx->pVPS[pSPS->video_parameter_set_id];

    if ((pPPS->dependent_slice_segments_enabled_flag) && (!pHevcCtx->CurrSlice.first_slice_segment_in_pic_flag))
    {
        pHevcCtx->CurrSlice.dependent_slice_segment_flag = HEVC_u_v(pHevcCtx->pBS, 1, "dependent_slice_segment_flag");
    }
    else
    {
        pHevcCtx->CurrSlice.dependent_slice_segment_flag = HEVC_FALSE;
    }

    if (0 == pSPS->max_cu_width || 0 == pSPS->max_cu_height)
    {
        dprint("max_cu_width(%d)/max_cu_height(%d) = 0\n", pSPS->max_cu_width, pSPS->max_cu_height);
        return HEVC_DEC_ERR;
    }
    
    numCUs = ((pSPS->pic_width_in_luma_samples + pSPS->max_cu_width -1)/pSPS->max_cu_width)
            *((pSPS->pic_height_in_luma_samples+ pSPS->max_cu_height-1)/pSPS->max_cu_height);
    maxParts = (1 << (pSPS->max_cu_depth << 1));

    while (numCUs > (1<<reqBitsOuter))
    {
        reqBitsOuter++;
    }

#if 0    
    while (numParts > (1<<reqBitsInner))
    {
        reqBitsInner++;
    }
#endif    
    
    if (!pHevcCtx->CurrSlice.first_slice_segment_in_pic_flag)
    {
        pHevcCtx->CurrSlice.slice_segment_address = HEVC_u_v(pHevcCtx->pBS, reqBitsInner+reqBitsOuter, "slice_segment_address");
        if (pHevcCtx->CurrSlice.slice_segment_address < 0 || pHevcCtx->CurrSlice.slice_segment_address >= (pSPS->ctb_num_width)*(pSPS->ctb_num_height))
        {
            dprint( "slice_segment_address(%d) out of range(0,%d).\n", pHevcCtx->CurrSlice.slice_segment_address, (pSPS->ctb_num_width)*(pSPS->ctb_num_height));
            return HEVC_DEC_ERR;
        }
        lcuAddress   = pHevcCtx->CurrSlice.slice_segment_address >> reqBitsInner;
        innerAddress = pHevcCtx->CurrSlice.slice_segment_address - (lcuAddress << reqBitsInner);
    }
    sliceAddress = maxParts * lcuAddress + innerAddress;
    pHevcCtx->CurrSlice.dependent_slice_curstart_cuaddr = sliceAddress;
    pHevcCtx->CurrSlice.dependent_slice_curend_cuaddr   = numCUs * maxParts;
    pHevcCtx->CurrSlice.poc = pHevcCtx->LastSlicePoc;

    pHevcCtx->CurrSlice.slice_type = HEVC_I_SLICE;
    if (!pHevcCtx->CurrSlice.dependent_slice_segment_flag)
    {
        for (i = 0; i < pPPS->num_extra_slice_header_bits; i++)
        {
            ue = HEVC_u_v(pHevcCtx->pBS, 1, "slice_reserved_flag");
        }

        pHevcCtx->CurrSlice.slice_type = HEVC_ue_v(pHevcCtx->pBS, "slice_type");
        if (pHevcCtx->CurrSlice.slice_type < 0 || pHevcCtx->CurrSlice.slice_type > 2)
        {
            dprint( "slice_type(%d) out of range(0,2].\n", pHevcCtx->CurrSlice.slice_type);
            return HEVC_DEC_ERR;
        }

        if (pPPS->output_flag_present_flag)
        {
            pHevcCtx->CurrSlice.pic_output_flag = HEVC_u_v(pHevcCtx->pBS, 1, "pic_output_flag");
        }
        else
        {
            pHevcCtx->CurrSlice.pic_output_flag = HEVC_TRUE;
        }
        
        if (!IDRPicFlag)
        {
            pHevcCtx->CurrSlice.pic_order_cnt_lsb = HEVC_u_v(pHevcCtx->pBS, pSPS->bits_for_poc, "pic_order_cnt_lsb");
            
            /* calc poc of current slice*/
            PocLsb = pHevcCtx->CurrSlice.pic_order_cnt_lsb;
            MaxPocLsb = 1 << pSPS->max_pic_order_cnt_lsb;
            PrePocLsb = pHevcCtx->PrevSlicePoc % MaxPocLsb;
            PrePocMsb = pHevcCtx->PrevSlicePoc - PrePocLsb;

            if ((PocLsb < PrePocLsb) && ((PrePocLsb - PocLsb) >= (MaxPocLsb / 2)))
            {
                PocMsb = PrePocMsb + MaxPocLsb;
            }
            else if ((PocLsb > PrePocLsb) && ((PocLsb - PrePocLsb) > (MaxPocLsb / 2)))
            {
                PocMsb = PrePocMsb - MaxPocLsb;
            }
            else
            {
                PocMsb = PrePocMsb;
            }

            if (NAL_UNIT_CODED_SLICE_BLA_W_LP   == pHevcCtx->CurrSlice.nal_unit_type
                || NAL_UNIT_CODED_SLICE_BLA_W_RADL == pHevcCtx->CurrSlice.nal_unit_type
                || NAL_UNIT_CODED_SLICE_BLA_N_LP   == pHevcCtx->CurrSlice.nal_unit_type)
            {
                PocMsb = 0;
            }
            pHevcCtx->CurrSlice.poc = PocLsb + PocMsb;
            pHevcCtx->LastSlicePoc = pHevcCtx->CurrSlice.poc;

            if ((0 == pHevcCtx->CurrSlice.nuh_temporal_id) 
                && (HEVC_IsReferenceNalu(pHevcCtx) && (pHevcCtx->CurrSlice.nal_unit_type != NAL_UNIT_CODED_SLICE_RASL_R) && (pHevcCtx->CurrSlice.nal_unit_type != NAL_UNIT_CODED_SLICE_RADL_R)))
            {
                pHevcCtx->CurrSlice.prev_poc = pHevcCtx->CurrSlice.poc;
                pHevcCtx->PrevSlicePoc = pHevcCtx->CurrSlice.poc;
            }

            pHevcCtx->CurrSlice.short_term_ref_pic_set_sps_flag = HEVC_u_v(pHevcCtx->pBS, 1, "short_term_ref_pic_set_sps_flag");

            if (!pHevcCtx->CurrSlice.short_term_ref_pic_set_sps_flag)
            {
                ret = HEVC_DecShortTermRefPicSet(pHevcCtx,pSPS, &(pHevcCtx->CurrSlice.short_term_ref_pic_set), pSPS->num_short_term_ref_pic_sets);
                if (HEVC_DEC_NORMAL != ret)
                {
                    dprint( "SH HEVC_DecShortTermRefPicSet error.\n");
                    return HEVC_DEC_ERR;
                }
            }
            else
            {
                while ((1u << numBits) < pSPS->num_short_term_ref_pic_sets)
                {
                    numBits++;
                }
                if (numBits > 0)
                {
                    pHevcCtx->CurrSlice.short_term_ref_pic_set_idx = HEVC_u_v(pHevcCtx->pBS,numBits, "short_term_ref_pic_set_idx"); 
                }
                else
                {
                    pHevcCtx->CurrSlice.short_term_ref_pic_set_idx = 0;
                }
                if (pHevcCtx->CurrSlice.short_term_ref_pic_set_idx < 0 || pHevcCtx->CurrSlice.short_term_ref_pic_set_idx > pSPS->num_short_term_ref_pic_sets-1)
                {
                    dprint( "short_term_ref_pic_set_idx(%d) out of range[0,%d].\n", pHevcCtx->CurrSlice.short_term_ref_pic_set_idx, pSPS->num_short_term_ref_pic_sets - 1);
                    return HEVC_DEC_ERR;
                }
                pHevcCtx->CurrSlice.short_term_ref_pic_set = pSPS->short_term_ref_pic_set[pHevcCtx->CurrSlice.short_term_ref_pic_set_idx];				
            }
            pRPS = &(pHevcCtx->CurrSlice.short_term_ref_pic_set);

            if (pSPS->long_term_ref_pics_present_flag)
            {
                offset = pRPS->num_negative_pics + pRPS->num_positive_pics;
                if (pSPS->num_long_term_ref_pic_sps > 0)
                {
                    pHevcCtx->CurrSlice.num_long_term_sps = HEVC_ue_v(pHevcCtx->pBS, "num_long_term_sps");
                    if (pHevcCtx->CurrSlice.num_long_term_sps > pSPS->num_long_term_ref_pic_sps)
                    {
                        dprint( "num_long_term_sps(%d) out of range[0,%d].\n", pHevcCtx->CurrSlice.num_long_term_sps, pSPS->num_long_term_ref_pic_sps);
                        return HEVC_DEC_ERR;
                    }
                }
                bitsForLongTermPicInSPS = 0;
                while (pSPS->num_long_term_ref_pic_sps > ((UINT32)(1<<bitsForLongTermPicInSPS)))
                {
                    bitsForLongTermPicInSPS++;
                }
                pHevcCtx->CurrSlice.num_long_term_pics = HEVC_ue_v(pHevcCtx->pBS, "num_long_term_pics");
                pRPS->num_of_longterm_pics = pHevcCtx->CurrSlice.num_long_term_sps + pHevcCtx->CurrSlice.num_long_term_pics;

                if ((pRPS->num_negative_pics + pRPS->num_positive_pics + pRPS->num_of_longterm_pics) > HEVC_MAX_NUM_REF_PICS)
                {
                    dprint( "SH ERROR: (num_negative_pics(%d) + num_positive_pics(%d) + num_of_longterm_pics(%d)) > %d\n", pRPS->num_negative_pics, pRPS->num_positive_pics, pRPS->num_of_longterm_pics, HEVC_MAX_NUM_REF_PICS);
                    return HEVC_DEC_ERR;
                }

                for (i=0, j=offset+pRPS->num_of_longterm_pics-1; i<pRPS->num_of_longterm_pics; j--,i++)
                {
                    if (i < pHevcCtx->CurrSlice.num_long_term_sps)
                    {
                        lt_idx_sps = 0;
                        if (bitsForLongTermPicInSPS)
                        {
                            pHevcCtx->CurrSlice.lt_idx_sps[i] = HEVC_u_v(pHevcCtx->pBS, bitsForLongTermPicInSPS, "lt_idx_sps");
                            lt_idx_sps = pHevcCtx->CurrSlice.lt_idx_sps[i];
                            if (lt_idx_sps < 0 || lt_idx_sps > pSPS->num_long_term_ref_pic_sps-1)
                            {
                                dprint( "lt_idx_sps[%d] = %d, but out of range[0,%d].\n", i, lt_idx_sps, pSPS->num_long_term_ref_pic_sps-1);
                                return HEVC_DEC_ERR;
                            }    
                        }  

                        pocLsbLt = pSPS->lt_ref_pic_poc_lsb_sps[lt_idx_sps];
                        pRPS->used_flag[j] = pSPS->used_by_curr_pic_lt_sps_flag[lt_idx_sps];
                    }
                    else
                    {
                        pHevcCtx->CurrSlice.poc_lsb_lt[i] = HEVC_u_v(pHevcCtx->pBS, pSPS->bits_for_poc, "poc_lsb_lt");
                        pocLsbLt = pHevcCtx->CurrSlice.poc_lsb_lt[i];
                        pHevcCtx->CurrSlice.used_by_curr_pic_lt_flag[i] = HEVC_u_v(pHevcCtx->pBS, 1, "used_by_curr_pic_lt_flag");
                        pRPS->used_flag[j] = pHevcCtx->CurrSlice.used_by_curr_pic_lt_flag[i];
                    }

                    pHevcCtx->CurrSlice.pocLsbLt[i] = pocLsbLt;
                    pHevcCtx->CurrSlice.delta_poc_msb_present_flag[i] = HEVC_u_v(pHevcCtx->pBS, 1, "delta_poc_msb_present_flag");
                    if (pHevcCtx->CurrSlice.delta_poc_msb_present_flag[i])
                    {
                        pHevcCtx->CurrSlice.delta_poc_msb_cycle_lt[i] = HEVC_ue_v(pHevcCtx->pBS, "delta_poc_msb_cycle_lt");
                        if (i==0 || i==pHevcCtx->CurrSlice.num_long_term_sps)  
                        {
                            deltaPocMSBCycleLT = pHevcCtx->CurrSlice.delta_poc_msb_cycle_lt[i];
                        }
                        else
                        {
                            deltaPocMSBCycleLT = pHevcCtx->CurrSlice.delta_poc_msb_cycle_lt[i] + prevDeltaMSB;
                        }
                        pHevcCtx->CurrSlice.deltaPocMSBCycleLT[j] = deltaPocMSBCycleLT;

                        pHevcCtx->CurrSlice.CheckLTMSB[j] = 1;
                    }
                    else
                    {
                        deltaPocMSBCycleLT = (i==0 || i==pHevcCtx->CurrSlice.num_long_term_sps)?0:prevDeltaMSB;
                        pHevcCtx->CurrSlice.CheckLTMSB[j] = 0;
                    }
                    //prevLSB = pocLsbLt;
                    prevDeltaMSB = deltaPocMSBCycleLT;
                }

                for (i=0, j=offset+pRPS->num_of_longterm_pics-1; i<pRPS->num_of_longterm_pics; j--,i++)
                {
                    if (pHevcCtx->CurrSlice.delta_poc_msb_present_flag[i])
                    {
                        PocLTCurr = pHevcCtx->CurrSlice.poc - pHevcCtx->CurrSlice.deltaPocMSBCycleLT[j] * MaxPocLsb
                                            - PocLsb + pHevcCtx->CurrSlice.pocLsbLt[i];
                        pRPS->poc[j] = PocLTCurr;
                        pRPS->delta_poc[j] = PocLTCurr - pHevcCtx->CurrSlice.poc;                       
                    }
                    else
                    {
                        pRPS->poc[j] = pHevcCtx->CurrSlice.pocLsbLt[i];
                        pRPS->delta_poc[j] = pHevcCtx->CurrSlice.pocLsbLt[i] - pHevcCtx->CurrSlice.poc;
                    }
                }

                pRPS->num_of_pics = offset + pRPS->num_of_longterm_pics;
                if (pRPS->num_of_pics > HEVC_MAX_NUM_REF_PICS)
                {
                    dprint( "ERROR: pRPS->num_of_pics(%d) > %d.\n", pRPS->num_of_pics, HEVC_MAX_NUM_REF_PICS);
                    return HEVC_DEC_ERR;
                }
            }
            if (pSPS->sps_temporal_mvp_enable_flag)
            {
            pHevcCtx->CurrSlice.slice_temporal_mvp_enable_flag = HEVC_u_v(pHevcCtx->pBS, 1, "slice_temporal_mvp_enable_flag");
            }
            else
            {
            pHevcCtx->CurrSlice.slice_temporal_mvp_enable_flag = HEVC_FALSE;
            }
        }
        else
        {
            pHevcCtx->CurrSlice.poc = 0;
            pHevcCtx->LastSlicePoc = pHevcCtx->CurrSlice.poc;
            if (0 == pHevcCtx->CurrSlice.nuh_temporal_id)
            {
                pHevcCtx->CurrSlice.prev_poc = 0;
                pHevcCtx->PrevSlicePoc = 0;
            }
        }

        if (pSPS->sample_adaptive_offset_enabled_flag)
        {
        pHevcCtx->CurrSlice.slice_sao_luma_flag = HEVC_u_v(pHevcCtx->pBS, 1, "slice_sao_luma_flag");
        pHevcCtx->CurrSlice.slice_sao_chroma_flag = HEVC_u_v(pHevcCtx->pBS, 1, "slice_sao_chroma_flag");
        }

        if (IDRPicFlag)
        {
        pHevcCtx->CurrSlice.slice_temporal_mvp_enable_flag = HEVC_FALSE;
        }

        if (pHevcCtx->CurrSlice.slice_type != HEVC_I_SLICE)
        {
            pHevcCtx->CurrSlice.num_ref_idx_active_override_flag = HEVC_u_v(pHevcCtx->pBS, 1, "num_ref_idx_active_override_flag");
            if (pHevcCtx->CurrSlice.num_ref_idx_active_override_flag)
            {
                pHevcCtx->CurrSlice.num_ref_idx_l0_active = HEVC_ue_v(pHevcCtx->pBS, "num_ref_idx_l0_active_minus1") + 1;
                if (pHevcCtx->CurrSlice.num_ref_idx_l0_active > HEVC_MAX_NUM_REF_PICS)
                {
                    dprint( "num_ref_idx_l0_active(%d) out of range(0,%d).\n", pHevcCtx->CurrSlice.num_ref_idx_l0_active, HEVC_MAX_NUM_REF_PICS);
                    return HEVC_DEC_ERR;
                }

                pHevcCtx->CurrSlice.num_ref_idx[0] = pHevcCtx->CurrSlice.num_ref_idx_l0_active;

                if (HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type)
                {
                    pHevcCtx->CurrSlice.num_ref_idx_l1_active = HEVC_ue_v(pHevcCtx->pBS, "num_ref_idx_l1_active_minus1") + 1;
                    if (pHevcCtx->CurrSlice.num_ref_idx_l1_active > HEVC_MAX_NUM_REF_PICS)
                    {
                        dprint( "num_ref_idx_l1_active(%d) out of range(0,%d).\n", pHevcCtx->CurrSlice.num_ref_idx_l1_active, HEVC_MAX_NUM_REF_PICS);
                        return HEVC_DEC_ERR;
                    }	
                    pHevcCtx->CurrSlice.num_ref_idx[1] = pHevcCtx->CurrSlice.num_ref_idx_l1_active;
                }
                else
                {
                    pHevcCtx->CurrSlice.num_ref_idx_l1_active = 0;
                    pHevcCtx->CurrSlice.num_ref_idx[1] = 0;
                }
            }
            else
            {
                pHevcCtx->CurrSlice.num_ref_idx[0] = pPPS->num_ref_idx_l0_default_active;
                if (HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type)
                {
                    pHevcCtx->CurrSlice.num_ref_idx[1] = pPPS->num_ref_idx_l1_default_active;
                }
                else
                {
                    pHevcCtx->CurrSlice.num_ref_idx[1] = 0;
                }
            }
        }

        if (HEVC_I_SLICE == pHevcCtx->CurrSlice.slice_type)
        {
            numRpsCurrTempList = 0;
        }
        else
        {
            if (pRPS == NULL)
            {
                dprint( "pRPS is null.\n");
                return HEVC_DEC_ERR;
            }

            if (pRPS->num_of_pics > HEVC_MAX_NUM_REF_PICS)
            {
                dprint( "pRPS->num_of_pics out(%d) of range(0,%d).\n", pRPS->num_of_pics, HEVC_MAX_NUM_REF_PICS);
                return HEVC_DEC_ERR;
            }

            for (i=0; i<pRPS->num_of_pics; i++)
            {
                if (pRPS->used_flag[i])
                {
                    numRpsCurrTempList++;
                }
            }
        }

        if (HEVC_I_SLICE != pHevcCtx->CurrSlice.slice_type)
        {
            if (pPPS->lists_modification_present_flag && numRpsCurrTempList>1)
            {
                ret = HEVC_DecRefPicListsModification(pHevcCtx,&pHevcCtx->CurrSlice, &(pHevcCtx->CurrSlice.ref_pic_lists_modification), numRpsCurrTempList);
                if (HEVC_DEC_NORMAL != ret)
                {
                    dprint( "SH HEVC_DecRefPicListsModification error.\n");
                    return HEVC_DEC_ERR;
                }
            }
        }
        if (HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type)
        {
            pHevcCtx->CurrSlice.mvd_l1_zero_flag = HEVC_u_v(pHevcCtx->pBS, 1, "mvd_l1_zero_flag");
        }
        if ((HEVC_I_SLICE != pHevcCtx->CurrSlice.slice_type)&& (pPPS->cabac_init_present_flag))
        {
            pHevcCtx->CurrSlice.cabac_init_flag = HEVC_u_v(pHevcCtx->pBS, 1, "cabac_init_flag");
        }
        if (pHevcCtx->CurrSlice.slice_temporal_mvp_enable_flag)
        {
            if (HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type)
            {
                pHevcCtx->CurrSlice.collocated_from_l0_flag = HEVC_u_v(pHevcCtx->pBS, 1, "collocated_from_l0_flag");
            }
            else
            {
                pHevcCtx->CurrSlice.collocated_from_l0_flag = 1;
            }
            if ((HEVC_I_SLICE != pHevcCtx->CurrSlice.slice_type) &&
                ((pHevcCtx->CurrSlice.collocated_from_l0_flag && (pHevcCtx->CurrSlice.num_ref_idx[0] > 1)) ||
                (!pHevcCtx->CurrSlice.collocated_from_l0_flag && (pHevcCtx->CurrSlice.num_ref_idx[1] > 1))))
            {
                pHevcCtx->CurrSlice.collocated_ref_idx = HEVC_ue_v(pHevcCtx->pBS, "collocated_ref_idx");

                if (pHevcCtx->CurrSlice.collocated_from_l0_flag)
                {
                    MaxCollocatedRefIdx = pHevcCtx->CurrSlice.num_ref_idx_l0_active - 1;
                }
                else
                {
                    MaxCollocatedRefIdx = pHevcCtx->CurrSlice.num_ref_idx_l1_active - 1;
                }
                if (pHevcCtx->CurrSlice.collocated_ref_idx < 0 || pHevcCtx->CurrSlice.collocated_ref_idx > MaxCollocatedRefIdx)
                {
                    dprint( "collocated_from_l0_flag=%d, collocated_ref_idx(%d) out of range(0,%d).\n", pHevcCtx->CurrSlice.collocated_from_l0_flag, pHevcCtx->CurrSlice.collocated_ref_idx, MaxCollocatedRefIdx);
                    return HEVC_DEC_ERR;
                }
            }
        }
        if ((pPPS->weighted_pred_flag && (HEVC_P_SLICE == pHevcCtx->CurrSlice.slice_type)) ||
            (pPPS->weighted_bipred_flag && (HEVC_B_SLICE == pHevcCtx->CurrSlice.slice_type)))
        {
            ret = HEVC_DecPredWeightTable(pHevcCtx);
            if (HEVC_DEC_NORMAL != ret)
            {
                dprint( "SH HEVC_DecPredWeightTable error.\n");
                return HEVC_DEC_ERR;
            }
        }

        if (HEVC_I_SLICE != pHevcCtx->CurrSlice.slice_type)
        {
            pHevcCtx->CurrSlice.max_num_merge_cand = MRG_MAX_NUM_CANDS - HEVC_ue_v(pHevcCtx->pBS, "five_minus_max_num_merge_cand");
            if (pHevcCtx->CurrSlice.max_num_merge_cand < 1 || pHevcCtx->CurrSlice.max_num_merge_cand > 5)
            {
                dprint( "SH max_num_merge_cand(%d) out of range(1,5).\n", pHevcCtx->CurrSlice.max_num_merge_cand);
                return HEVC_DEC_ERR;
            }
        }
        pHevcCtx->CurrSlice.slice_qp_delta = HEVC_se_v(pHevcCtx->pBS, "slice_qp_delta");
        pHevcCtx->CurrSlice.slice_qp = pHevcCtx->CurrSlice.slice_qp_delta + pPPS->pic_init_qp;
        if (pHevcCtx->CurrSlice.slice_qp < (SINT32)(-pSPS->qp_bdOffset_y) || pHevcCtx->CurrSlice.slice_qp > 51)
        {
            dprint( "SH slice_qp(%d) out of range [-%d,51].\n", pHevcCtx->CurrSlice.slice_qp, pSPS->qp_bdOffset_y);
            return HEVC_DEC_ERR;
        }

        if (pPPS->pic_slice_chroma_qp_offsets_present_flag)
        {
            pHevcCtx->CurrSlice.slice_cb_qp_offset = HEVC_se_v(pHevcCtx->pBS, "slice_cb_qp_offset");

            if (pHevcCtx->CurrSlice.slice_cb_qp_offset < -12 || pHevcCtx->CurrSlice.slice_cb_qp_offset > 12)
            {
                dprint( "SH slice_cb_qp_offset(%d) out of range[-12,12].\n", pHevcCtx->CurrSlice.slice_cb_qp_offset);
                return HEVC_DEC_ERR;
            }

            if ((pHevcCtx->CurrSlice.slice_cb_qp_offset + pPPS->pic_cb_qp_offset) < -12 ||
            (pHevcCtx->CurrSlice.slice_cb_qp_offset + pPPS->pic_cb_qp_offset) > 12)
            {
                dprint( "SH slice_cb_qp_offset(%d)+pic_cb_qp_offset(%d) out of range[-12,12].\n", pHevcCtx->CurrSlice.slice_cb_qp_offset, pPPS->pic_cb_qp_offset);
                return HEVC_DEC_ERR;
            }

            pHevcCtx->CurrSlice.slice_cr_qp_offset = HEVC_se_v(pHevcCtx->pBS, "slice_cr_qp_offset");

            if (pHevcCtx->CurrSlice.slice_cr_qp_offset < -12 || pHevcCtx->CurrSlice.slice_cr_qp_offset > 12)
            {
                dprint( "SH slice_cr_qp_offset(%d) out of range[-12,12].\n", pHevcCtx->CurrSlice.slice_cr_qp_offset);
                return HEVC_DEC_ERR;
            }

            if ((pHevcCtx->CurrSlice.slice_cr_qp_offset + pPPS->pic_cr_qp_offset) < -12 ||
                (pHevcCtx->CurrSlice.slice_cr_qp_offset + pPPS->pic_cr_qp_offset) > 12)
            {
                dprint( "SH slice_cr_qp_offset+pPPS->pic_cr_qp_offset(%d) out of range[-12,12].\n", (pHevcCtx->CurrSlice.slice_cr_qp_offset + pPPS->pic_cr_qp_offset));
                return HEVC_DEC_ERR;
            }
        }

        if (pPPS->deblocking_filter_control_present_flag)
        {
            if (pPPS->deblocking_filter_override_enabled_flag)
            {
                pHevcCtx->CurrSlice.deblocking_filter_override_flag = HEVC_u_v(pHevcCtx->pBS, 1, "deblocking_filter_override_flag");
            }
            else
            {
                pHevcCtx->CurrSlice.deblocking_filter_override_flag = 0;
            }
            if (pHevcCtx->CurrSlice.deblocking_filter_override_flag)
            {
                pHevcCtx->CurrSlice.slice_disable_deblocking_filter_flag = HEVC_u_v(pHevcCtx->pBS, 1, "slice_disable_deblocking_filter_flag");
                if (!pHevcCtx->CurrSlice.slice_disable_deblocking_filter_flag)
                {
                    pHevcCtx->CurrSlice.slice_beta_offset_div2 = HEVC_se_v(pHevcCtx->pBS, "slice_beta_offset_div2");
                    if (pHevcCtx->CurrSlice.slice_beta_offset_div2 < -6 || pHevcCtx->CurrSlice.slice_beta_offset_div2 > 6)
                    {
                        dprint( "SH slice_beta_offset_div2(%d) out of range(-6,6).\n", pHevcCtx->CurrSlice.slice_beta_offset_div2);
                        pHevcCtx->CurrSlice.slice_beta_offset_div2 = pPPS->pps_beta_offset_div2; // 赋值强解	
                        //return HEVC_DEC_ERR;
                    }
                    pHevcCtx->CurrSlice.slice_tc_offset_div2 = HEVC_se_v(pHevcCtx->pBS, "slice_tc_offset_div2");
                    if (pHevcCtx->CurrSlice.slice_tc_offset_div2  < -6 || pHevcCtx->CurrSlice.slice_tc_offset_div2  > 6)
                    {
                        dprint( "SH slice_tc_offset_div2(%d) out of range(-6,6).\n", pHevcCtx->CurrSlice.slice_tc_offset_div2);
                        pHevcCtx->CurrSlice.slice_tc_offset_div2 = pPPS->pps_tc_offset_div2; // 赋值强解	
                        // return HEVC_DEC_ERR;
                    }
                }
            }
            else
            {
                pHevcCtx->CurrSlice.slice_disable_deblocking_filter_flag = pPPS->pic_disable_deblocking_filter_flag;
                pHevcCtx->CurrSlice.slice_beta_offset_div2 = pPPS->pps_beta_offset_div2;
                pHevcCtx->CurrSlice.slice_tc_offset_div2   = pPPS->pps_tc_offset_div2;
            }
        }

        isSAOEnabled = (!pSPS->sample_adaptive_offset_enabled_flag)? (HEVC_FALSE): (pHevcCtx->CurrSlice.slice_sao_luma_flag || pHevcCtx->CurrSlice.slice_sao_chroma_flag);
        isDBFEnabled = !(pHevcCtx->CurrSlice.slice_disable_deblocking_filter_flag);
        if (pPPS->loop_filter_across_slices_enabled_flag && (isSAOEnabled || isDBFEnabled))
        {
            pHevcCtx->CurrSlice.slice_loop_filter_across_slices_enabled_flag = HEVC_u_v(pHevcCtx->pBS, 1, "slice_loop_filter_across_slices_enabled_flag");
        }
        else
        {
            pHevcCtx->CurrSlice.slice_loop_filter_across_slices_enabled_flag = pPPS->loop_filter_across_slices_enabled_flag;
        }
    }

    if (pPPS->tiles_enabled_flag || pPPS->entropy_coding_sync_enabled_flag)
    {
        pHevcCtx->CurrSlice.num_entry_point_offsets = HEVC_ue_v(pHevcCtx->pBS, "num_entry_point_offsets");

        if (0 == pPPS->tiles_enabled_flag && 1 == pPPS->entropy_coding_sync_enabled_flag)
        {
            MaxNumEntryPointOffsets = pSPS->ctb_num_height - 1;
        }
        else if (1 == pPPS->tiles_enabled_flag && 0 == pPPS->entropy_coding_sync_enabled_flag)
        {
            MaxNumEntryPointOffsets = pPPS->num_tile_columns * pPPS->num_tile_rows - 1;
        }
        else
        {
            MaxNumEntryPointOffsets = pPPS->num_tile_columns * pSPS->ctb_num_height - 1;
        }

        if (pHevcCtx->CurrSlice.num_entry_point_offsets > MIN(MaxNumEntryPointOffsets, 255))
        {
            dprint( "CurrSlice.num_entry_point_offsets(%d) out of range[0,%d].\n", pHevcCtx->CurrSlice.num_entry_point_offsets, MIN(MaxNumEntryPointOffsets, 255));
            return HEVC_DEC_ERR;
        }
        
        if (pHevcCtx->CurrSlice.num_entry_point_offsets > 0)
        {
            pHevcCtx->CurrSlice.offset_len = 1 + HEVC_ue_v(pHevcCtx->pBS, "offset_len_minus1");
            if (pHevcCtx->CurrSlice.offset_len < 1 || pHevcCtx->CurrSlice.offset_len > 32)
            {
            	dprint( "CurrSlice.offset_len(%d) out of range[1,32].\n", pHevcCtx->CurrSlice.offset_len);
            	return HEVC_DEC_ERR;
            }    
        }

        for (i=0; i<pHevcCtx->CurrSlice.num_entry_point_offsets; i++)
        {
            pHevcCtx->CurrSlice.entry_point_offset[i] = HEVC_u_v(pHevcCtx->pBS, pHevcCtx->CurrSlice.offset_len, "entry_point_offset_minus1");
        }
    }
    else
    {
        pHevcCtx->CurrSlice.num_entry_point_offsets = 0;
    }
    
    if (pPPS->slice_segment_header_extension_present_flag)
    {
        pHevcCtx->CurrSlice.slice_segment_header_extension_length = HEVC_ue_v(pHevcCtx->pBS, "slice_segment_header_extension_length");
        if (pHevcCtx->CurrSlice.slice_segment_header_extension_length > 256 )
        {
            dprint( "slice_segment_header_extension_length(%d) out of range [0,256]\n", pHevcCtx->CurrSlice.slice_segment_header_extension_length);
            return HEVC_DEC_ERR;
        }

        bitsLeft = BsResidBits(pHevcCtx->pBS);
        if (bitsLeft < pHevcCtx->CurrSlice.slice_segment_header_extension_length)
        {
            dprint( "bitsLeft(%d) < slice_segment_header_extension_length(%d)\n",bitsLeft,pHevcCtx->CurrSlice.slice_segment_header_extension_length);
            return HEVC_DEC_ERR;
        }

        for (i=0; i<pHevcCtx->CurrSlice.slice_segment_header_extension_length; i++)
        {
            pHevcCtx->CurrSlice.slice_segment_header_extension_data_byte = HEVC_u_v(pHevcCtx->pBS, 8, "slice_segment_header_extension_data_byte");
        }
    }
    
    if (pHevcCtx->CurrSlice.dependent_slice_curstart_cuaddr == 0)
    {
        if(pHevcCtx->bNewSequence)
        {
            if (RAPPicFlag)
            {
            if (IsDecSlice)   
            {
                    pHevcCtx->bNewSequence = HEVC_FALSE;
            }
                pHevcCtx->bNoRaslOutputFlag = HEVC_TRUE;
            }
        }
        else
        {
            if (RAPPicFlag)
            {
                pHevcCtx->bNoRaslOutputFlag = HEVC_FALSE;
            }
        }
    } 
    
    if (pHevcCtx->bNoRaslOutputFlag && pHevcCtx->CurrSlice.poc != 0 && RAPPicFlag)
    {
        if(pHevcCtx->CurrSlice.nal_unit_type == NAL_UNIT_CODED_SLICE_CRA)
        {
            pHevcCtx->bNoOutputOfPriorPicsFlag = HEVC_TRUE;
        }
        else
        {
            pHevcCtx->bNoOutputOfPriorPicsFlag = HEVC_FALSE;
        }
    }
    else
    {
        pHevcCtx->bNoOutputOfPriorPicsFlag = HEVC_FALSE;
    }
    
    pHevcCtx->CurrSlice.new_pic_type = HEVC_IsNewPic(pHevcCtx);

    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_CountRef(HEVC_CTX_S *pHevcCtx)
{
     SINT32 x,y;
     SINT32 min;
     char cType;

     pHevcCtx->HevcFrmType = pHevcCtx->CurrPic.pic_type;
     if(pHevcCtx->HevcFrmType == HEVC_I_FRAME ||
        pHevcCtx->HevcFrmType == HEVC_IDR_FRAME)
     {
        cType = 'I';
     }
     else if(pHevcCtx->HevcFrmType == HEVC_P_FRAME)
     {
        cType = 'P';
     }
     else if(pHevcCtx->HevcFrmType == HEVC_B_FRAME)
     {
        cType = 'B';
     }
     else
     {
        cType = 'N';
     }
     
     dprint("F(%d): Poc=%d, Type=%c, Ref=%d\n", pHevcCtx->HevcFrmCnt, pHevcCtx->HevcFrmPoc, cType, pHevcCtx->HevcRefNum);
     #ifdef HEVC_REF_FILE
     fprintf(g_fpRef, "F(%d): Poc=%d, Type=%c, Ref=%d\n", pHevcCtx->HevcFrmCnt, pHevcCtx->HevcFrmPoc, cType, pHevcCtx->HevcRefNum);
     #endif

     pHevcCtx->HevcFrmCnt++;
     if(pHevcCtx->HevcRefNum > 0)
     {
        for(x=0; x<pHevcCtx->HevcRefNum; x++)
        {
            min = pHevcCtx->HevcRefPoc[x];
            for(y=x; y<pHevcCtx->HevcRefNum; y++)
            {
                if(min > pHevcCtx->HevcRefPoc[y])
                {
                    min = pHevcCtx->HevcRefPoc[y];
                    pHevcCtx->HevcRefPoc[y] = pHevcCtx->HevcRefPoc[x];
                    pHevcCtx->HevcRefPoc[x] = min;
                }
            }
        }
        
        for(x=0; x<pHevcCtx->HevcRefNum; x++)
        {
            dprint(" %d, ", pHevcCtx->HevcRefPoc[x]);
            #ifdef HEVC_REF_FILE
            fprintf(g_fpRef, " %d, ", pHevcCtx->HevcRefPoc[x]);
            #endif
        }
        #ifdef HEVC_REF_FILE
        fprintf(g_fpRef, "\n");
        #endif
        dprint("\n");
     }
     #ifdef HEVC_REF_FILE
     fprintf(g_fpRef, "\n");
     fflush(g_fpRef);
     #endif

    pHevcCtx->Dmx_HevcFrmCnt = pHevcCtx->HevcFrmCnt;
    pHevcCtx->Dmx_HevcFrmPoc = pHevcCtx->HevcFrmPoc;
    pHevcCtx->Dmx_HevcFrmType = pHevcCtx->HevcFrmType;
    pHevcCtx->Dmx_HevcRefNum = pHevcCtx->HevcRefNum;
    memcpy(pHevcCtx->Dmx_HevcRefPoc, pHevcCtx->HevcRefPoc, 16*4);
    
    return HEVC_DEC_NORMAL;
}

static SINT32 HEVC_DecSlice(HEVC_CTX_S *pHevcCtx)
{
    pos();
    
    memset(&pHevcCtx->CurrSlice, 0, sizeof(HEVC_SLICE_SEGMENT_HEADER));
    pHevcCtx->CurrSlice.nal_unit_type   = pHevcCtx->pCurrNal->nal_unit_type;
    
    HEVC_DecSliceSegmentHeader(pHevcCtx, 1);

    if (HEVC_IsFlushUnit(pHevcCtx->CurrSlice.nal_unit_type)  || HEVC_TRUE==pHevcCtx->bNoOutputOfPriorPicsFlag)
    {
        Hevc_InitDecBuffers(pHevcCtx);
    }
    
    dprint("new_pic_type=%d, TotalSliceNum=%d\n", pHevcCtx->CurrSlice.new_pic_type, pHevcCtx->TotalSliceNum);
    
    if(IS_NEW_PIC == pHevcCtx->CurrSlice.new_pic_type)
    {
        if(pHevcCtx->CurrPic.state == HEVC_PIC_DECODING)
        {
            HEVC_CountRef(pHevcCtx);
            HEVC_StorePicInDPB(pHevcCtx);
        }
        
        HEVC_RefPicProcess(pHevcCtx);
        HEVC_InitPic(pHevcCtx);
        HEVC_WritePicMsg(pHevcCtx);
    }

    HEVC_PicTypeStatistic(pHevcCtx);

    if (!pHevcCtx->CurrSlice.dependent_slice_segment_flag)
    {
         HEVC_DecList(pHevcCtx);
    }
    HEVC_WriteSliceMsg(pHevcCtx);

    pHevcCtx->prev_pic_parameter_set_id = pHevcCtx->CurrSlice.pic_parameter_set_id;
    pHevcCtx->prev_poc = pHevcCtx->CurrSlice.poc;
    pHevcCtx->AllowStartDec = 1;

    return HEVC_DEC_NORMAL;
}

SINT32 HEVC_Init(HEVC_CTX_S *pHevcCtx)
{
     memset(pHevcCtx, 0, sizeof(HEVC_CTX_S));

    pHevcCtx->MaxVpsNum   = HEVC_MAX_VIDEO_PARAM_SET_ID;
    pHevcCtx->MaxSpsNum   = HEVC_MAX_SEQ_PARAM_SET_ID;
    pHevcCtx->MaxPpsNum   = HEVC_MAX_PIC_PARAM_SET_ID;
    
    memset(pHevcCtx->pVPS, 0, (pHevcCtx->MaxVpsNum)*sizeof(HEVC_VIDEO_PARAM_SET_S));
    memset(pHevcCtx->pSPS, 0, (pHevcCtx->MaxSpsNum)*sizeof(HEVC_SEQ_PARAM_SET_S));
    memset(pHevcCtx->pPPS, 0, (pHevcCtx->MaxPpsNum)* sizeof(HEVC_PIC_PARAM_SET_S));
    
    pHevcCtx->CurrVPS.video_parameter_set_id = pHevcCtx->MaxVpsNum ; 
    pHevcCtx->CurrPPS.pic_parameter_set_id   = pHevcCtx->MaxSpsNum ; 
    pHevcCtx->CurrSPS.seq_parameter_set_id   = pHevcCtx->MaxPpsNum ; 

    HEVC_InitScalingOrderTable(pHevcCtx);
 
    pHevcCtx->LastDisplayPoc = -HEVC_MAX_INT;
    
    HEVC_InitDecPara(pHevcCtx);  

    pHevcCtx->prev_pic_parameter_set_id = pHevcCtx->MaxPpsNum;
    pHevcCtx->prev_poc = 9999;

    //第1帧无效数据上报错误帧
    pHevcCtx->HevcFrmPoc = 0;
    pHevcCtx->HevcFrmType = HEVC_ERR_FRAME;
    pHevcCtx->HevcRefNum = 0;

    pHevcCtx->Dmx_HevcFrmPoc = 0;
    pHevcCtx->Dmx_HevcFrmType = HEVC_ERR_FRAME;
    pHevcCtx->Dmx_HevcRefNum = 0;

    return HEVC_DEC_NORMAL;
}

#if 0
UINT32 HEVC_f_v(BS *pBs, SINT32 v, SINT8 *Name)
{
    UINT32 Code;

    Code = BsGet(pBs, v);

    return Code;
}
#endif

HI_S32 Hevc_GetFirstNal(HI_S32 InstIdx)
{
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];
    SC_INFO_S   *pstThisSC = &pstCtx->stThisSC;

    pstCtx->s64FirstNalOffset = pstThisSC->s64GlobalOffset;
    
    if(pstCtx->s64LastSeiOffset<pstCtx->s64FirstNalOffset && pstCtx->s64LastSeiOffset>=0)
    {
        pstCtx->s64FirstNalOffset = pstCtx->s64LastSeiOffset;
        
    }
    if(pstCtx->s64LastPpsOffset<pstCtx->s64FirstNalOffset && pstCtx->s64LastPpsOffset>=0)
    {
        pstCtx->s64FirstNalOffset = pstCtx->s64LastPpsOffset;
    }
    if(pstCtx->s64LastSpsOffset<pstCtx->s64FirstNalOffset && pstCtx->s64LastSpsOffset>=0)
    {
        pstCtx->s64FirstNalOffset = pstCtx->s64LastSpsOffset;
    }
    if(pstCtx->s64LastVpsOffset<pstCtx->s64FirstNalOffset && pstCtx->s64LastVpsOffset>=0)
    {
        pstCtx->s64FirstNalOffset = pstCtx->s64LastVpsOffset;
    }
    pstCtx->s64LastSeiOffset = -1;
    pstCtx->s64LastPpsOffset = -1;
    pstCtx->s64LastSpsOffset = -1;
    pstCtx->s64LastVpsOffset = -1;
    return 0;
}

HI_S32 HEVC_MakeFrame(HI_S32 InstIdx)
{
    HI_S32 s32Ret = FIDX_OK;
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];
    FRAME_POS_S *pstFrmPos = &pstCtx->stNewFramePos;
    HEVC_CTX_S *pHevcCtx = pstCtx->pstHevcCtx;

    pstFrmPos->s32FrameSize = pstCtx->s64FirstNalOffset - pstCtx->s64NewFrmOffset;
    pstFrmPos->s64GlobalOffset = pstCtx->s64NewFrmOffset;
    pstCtx->s64NewFrmOffset = pstCtx->s64FirstNalOffset;
    if(pstFrmPos->s32FrameSize <= 0)
    {
        return FIDX_ERR;
    }
    HI_INFO_DEMUX(" HEVC_MakeFrame s32FrameSize = %d glboffset %llx nfmoffset %llx\n", pstFrmPos->s32FrameSize,
            pstFrmPos->s64GlobalOffset,pstCtx->s64NewFrmOffset);
    
    pstFrmPos->s32CurPoc = pHevcCtx->Dmx_HevcFrmPoc;
    pstFrmPos->s32RefNum = pHevcCtx->Dmx_HevcRefNum;
    memcpy(pstFrmPos->s32RefPoc, pHevcCtx->Dmx_HevcRefPoc, 16*4);

    if(pHevcCtx->Dmx_HevcFrmType == HEVC_IDR_FRAME)
    {
        pstFrmPos->eFrameType = FIDX_FRAME_TYPE_IDR;
    }
    else if(pHevcCtx->Dmx_HevcFrmType == HEVC_I_FRAME)
    {
        pstFrmPos->eFrameType = FIDX_FRAME_TYPE_I;
    }
    else if(pHevcCtx->Dmx_HevcFrmType == HEVC_P_FRAME)
    {
        pstFrmPos->eFrameType = FIDX_FRAME_TYPE_P;
    }
    else  if(pHevcCtx->Dmx_HevcFrmType == HEVC_B_FRAME)
    {
        pstFrmPos->eFrameType = FIDX_FRAME_TYPE_B;
    }
    else
    {
        pstFrmPos->eFrameType = FIDX_FRAME_TYPE_UNKNOWN;
    }
    
    if(pstFrmPos->eFrameType==FIDX_FRAME_TYPE_IDR || 
       pstFrmPos->eFrameType==FIDX_FRAME_TYPE_I)
    {
        pstFrmPos->u32PTS = pstCtx->u32NextPTS;
        pstCtx->u32NextPTS = pstCtx->u32PTS;
        pstCtx->u32PTS  = 0xffffffff;
    }
    else
    {
        #ifndef PUT_PTS_ON_I_FRAME_ONLY
        pstFrmPos->u32PTS  = pstCtx->u32NextPTS;
        #endif
        pstCtx->u32NextPTS = pstCtx->u32PTS;
        #ifndef PUT_PTS_ON_I_FRAME_ONLY
        pstCtx->u32PTS  = 0xffffffff;
        #endif
    }
    
    return s32Ret;
}

HI_S32 ProcessSC_HEVC(HI_S32 InstIdx)
{
    HI_S32 s32Ret = FIDX_OK;
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];
    SC_INFO_S   *pstThisSC = &pstCtx->stThisSC;
    HEVC_CTX_S *pHevcCtx = pstCtx->pstHevcCtx;

    FIDX_ASSERT_RET( 0!=pstCtx->s32ThisSCValid, "ThisSC is not valid\n");
    FIDX_ASSERT_RET( !IS_SC_WRONG(), "not enough data for ThisSC\n");

    pos();
    dprint("pstCtx->DataLen = 0x%x\n", pstCtx->s32ThisSCDataLen);
    dprint("Data: %08x, %08x\n", *(HI_U32*)(pstCtx->pu8HevcSCData+0), *(HI_U32*)(pstCtx->pu8HevcSCData+4));
    
    if(pstCtx->s64NewFrmOffset < 0)
    {
        pstCtx->s64NewFrmOffset = pstThisSC->s64GlobalOffset;
    }
    
    BsInit(pHevcCtx->pBS, (UINT8 *)(&pstCtx->pu8HevcSCData[0]), pstCtx->s32ThisSCDataLen);

    pHevcCtx->pCurrNal->forbidden_zero_bit      = BsGet(pHevcCtx->pBS, 1);
    pHevcCtx->pCurrNal->nal_unit_type           = BsGet(pHevcCtx->pBS, 6);
    pHevcCtx->pCurrNal->nuh_reserved_zero_6bits = BsGet(pHevcCtx->pBS, 6);
    pHevcCtx->pCurrNal->nuh_temporal_id         = BsGet(pHevcCtx->pBS, 3) - 1;
    dprint("nal_unit_type = %x\n", pHevcCtx->pCurrNal->nal_unit_type);
    
    if (pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_EOS || pHevcCtx->pCurrNal->nal_unit_type == NAL_UNIT_EOB)
    {
            pHevcCtx->bNewSequence = HEVC_TRUE;
    }

    dprint("entry proc hevc globaloffset %llx\n", pstThisSC->s64GlobalOffset);
    switch (pHevcCtx->pCurrNal->nal_unit_type)
    {
        case NAL_UNIT_VPS:
            dprint("nal = VPS\n");
            s32Ret = HEVC_DecVPS(pHevcCtx);
            pstCtx->s64LastVpsOffset = pstThisSC->s64GlobalOffset;
            break;
        case NAL_UNIT_SPS:
            dprint("nal = SPS\n");
            s32Ret = HEVC_DecSPS(pHevcCtx);
            pstCtx->s64LastSpsOffset = pstThisSC->s64GlobalOffset;
            break;
        case NAL_UNIT_PPS:
            dprint("nal = PPS\n");
            s32Ret = HEVC_DecPPS(pHevcCtx);
            pstCtx->s64LastPpsOffset = pstThisSC->s64GlobalOffset;
            break;
        case NAL_UNIT_PREFIX_SEI:
            pstCtx->s64LastSeiOffset = pstThisSC->s64GlobalOffset;
            break;
        case NAL_UNIT_SUFFIX_SEI:
            dprint("nal = SEI\n");
            break;
        case NAL_UNIT_CODED_SLICE_TRAIL_R:
        case NAL_UNIT_CODED_SLICE_TRAIL_N:
        case NAL_UNIT_CODED_SLICE_TLA_R:
        case NAL_UNIT_CODED_SLICE_TSA_N:
        case NAL_UNIT_CODED_SLICE_STSA_R:
        case NAL_UNIT_CODED_SLICE_STSA_N:
        case NAL_UNIT_CODED_SLICE_BLA_W_LP:
        case NAL_UNIT_CODED_SLICE_BLA_W_RADL:
        case NAL_UNIT_CODED_SLICE_BLA_N_LP:
        case NAL_UNIT_CODED_SLICE_IDR_W_RADL:
        case NAL_UNIT_CODED_SLICE_IDR_N_LP:
        case NAL_UNIT_CODED_SLICE_CRA:
        case NAL_UNIT_CODED_SLICE_RADL_N:
        case NAL_UNIT_CODED_SLICE_RADL_R:
        case NAL_UNIT_CODED_SLICE_RASL_N:
        case NAL_UNIT_CODED_SLICE_RASL_R:
            dprint("nal = Slice\n");
            s32Ret = HEVC_DecSlice(pHevcCtx);
            
            if(IS_NEW_PIC == pHevcCtx->CurrSlice.new_pic_type)
            {
                Hevc_GetFirstNal(InstIdx);
                
                if(FIDX_OK == HEVC_MakeFrame(InstIdx))
                {
                    OUTPUT_FRAME();
                }
            }
            break;
        default:
            dprint("nal = Non\n");
            break;
    }
    
    return s32Ret;
}

/*!***********************************************************************
@brief
    feed start code to FIDX.
    there are 2 method to feed necessary information to FIDX:
    1. feed stream directly. Call FIDX_MakeFrameIndex()
    2. feed start code. In this method, the start code must be scanned outside,
       This call this function to create index.
 ************************************************************************/
HI_VOID  FIDX_FeedStartCode(HI_S32 InstIdx, const FINDEX_SCD_S *pstSC)
{
    FIDX_CTX_S *pstCtx;
    FRAME_POS_S PesFrame;

    pstCtx = &s_FidxIIS[InstIdx];

    /* Get SC info */
    pstCtx->stThisSC.s32ScID = pstSC->u8StartCode;
    pstCtx->stThisSC.s64GlobalOffset = pstSC->s64GlobalOffset;
    pstCtx->stThisSC.s32OffsetInPacket = 0;
    pstCtx->stThisSC.s32PacketCount = 0;

    /* fill SC data */
    if (VIDSTD_HEVC == pstCtx->eVideoStandard)
    {
        /* adapt u8ThisSCData for general checking. */
        pstCtx->u8ThisSCData[0] = pstSC->u8StartCode;
        memcpy(pstCtx->u8ThisSCData + 1, pstSC->pu8ExtraSCData + 4, 8); /* +4 for skip 00 00 01 0x. */

        pstCtx->pu8HevcSCData = pstSC->pu8ExtraSCData + 3; /* +3 for skip 00 00 01*/
        pstCtx->s32ThisSCDataLen = pstSC->u32ExtraSCDataSize - 3;
    }
    else 
    {
        pstCtx->u8ThisSCData[0] = pstSC->u8StartCode;
        memcpy(pstCtx->u8ThisSCData + 1, pstSC->au8DataAfterSC, 8);
        pstCtx->s32ThisSCDataLen = 1 + 8;
    }
    pstCtx->s32ThisSCValid = 1;

    /*! if this SC is a PES SC, output it here simply, otherwise process it according to the video standard */
    if( 1 == IsPesSC(pstSC->u8StartCode, pstCtx->eVideoStandard) )
    {
        /*! the PTS after PES SC is valid, record it */
        pstCtx->u32PTS  = pstSC->u32PtsMs;

        /*! report the PES position */
        memset( &PesFrame, 0, sizeof(FRAME_POS_S) );
        PesFrame.eFrameType = FIDX_FRAME_TYPE_PESH;
        PesFrame.s64GlobalOffset = pstCtx->stThisSC.s64GlobalOffset;
        PesFrame.s32OffsetInPacket = pstCtx->stThisSC.s32OffsetInPacket;
        PesFrame.s32PacketCount = pstCtx->stThisSC.s32PacketCount;
        PesFrame.u32PTS = pstSC->u32PtsMs;
        
        if( NULL != s_OutputFramePosition )
        {
            (HI_VOID)s_OutputFramePosition( pstCtx->Param, &PesFrame );
        }

        pstCtx->s32ThisSCValid = 0;
        pstCtx->s32ThisSCDataLen = 0;
    }
    else
    {
        ANANYSE_SC();
    }

    return;
}

#if 0
/*!***********************************************************************
@brief
    to see if the SC is usful for index making
@return
    if the SC is usful return FIDX_OK, otherwise return FIDX_ERR
 ************************************************************************/
HI_S32  FIDX_IsSCUseful(HI_S32 InstIdx, HI_U8 u8StartCode)
{
    HI_S32 s32Ret = FIDX_OK;
    FIDX_CTX_S *pstCtx = &s_FidxIIS[InstIdx];

    /*! filter the SC according to the video standard */
    switch( pstCtx->eVideoStandard )
    {
    case VIDSTD_MPEG2:
        if ((u8StartCode != 0x00) && (u8StartCode != 0xb3))
        {
            s32Ret = FIDX_ERR;
        }
        break;

    case VIDSTD_H264:
        if ((u8StartCode & 0x80) != 0)
        {
            s32Ret = FIDX_ERR;
        }
        else
        {
            u8StartCode &= 0x1f;
            if ( (u8StartCode != FIDX_NALU_TYPE_SLICE) && (u8StartCode != FIDX_NALU_TYPE_IDR) &&
                 (u8StartCode != FIDX_NALU_TYPE_SPS) && (u8StartCode != FIDX_NALU_TYPE_PPS) )
            {
                s32Ret = FIDX_ERR;
            }
        }
        break;

    case VIDSTD_MPEG4:
        if ( (u8StartCode != 0xb0) && (u8StartCode != 0xb5) &&  /* visual_object_sequense & visual_object */
             (u8StartCode > 0x2f) && /* video_object(00~1f) & video_object_layer(20~2f) */
             (u8StartCode != 0xb6) ) /* video_object_plane */
        {
            s32Ret = FIDX_ERR;
        }
        break;

    case VIDSTD_AVS:
        if ((u8StartCode != 0x00) && (u8StartCode != 0xb3))
        {
            s32Ret = FIDX_ERR;
        }
        break;
    default:
        s32Ret = FIDX_ERR;
        break;
    }

    return s32Ret;
}
#endif



