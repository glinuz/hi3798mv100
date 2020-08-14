#ifndef __VP8_H__
#define __VP8_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "basedef.h"
#include "vfmw.h"
#include "public.h"
#include "vdm_hal.h"
#include "bitstream.h"
#include "syn_cmn.h"

#define VP8_MB_FEATURE_TREE_PROBS   3

#define VP8_MAX_REF_LF_DELTAS       4
#define VP8_MAX_MODE_LF_DELTAS      4

#define VP8_BLOCK_TYPES                    4
#define VP8_COEF_BANDS                        8
#define VP8_PREV_COEF_CONTEXTS       3
#define VP8_coef_tokens 12
#define VP8_MAX_ENTROPY_TOKENS    VP8_coef_tokens

#define SEGMENT_DELTADATA   0

#define VP8_BINTRAMODES 10
#define VP8_SUBMVREFS 2
#define VP8_YMODES  5
#define VP8_UV_MODES 4
#define VP8_MAX_MB_SEGMENTS         4

#define VP8_GOLDEN_FRM    2
#define VP8_ALTREF_FRM    3
#define VP8_MAXREF_FRMS    4

#define BLOCK_TYPES 4
#define COEF_BANDS 8
#define PREV_COEF_CONTEXTS       3
#define vp8_coef_tokens 12


typedef enum
{
    mv_max  = 1023,              /* max absolute value of a MV component */
    MVvals = (2 * mv_max) + 1,   /* # possible values "" */

    mvlong_width = 10,       /* Large MVs have 9 bit magnitudes */
    mvnum_short = 8,         /* magnitudes 0 through 7 */

    /* probability offsets for coding each MV component */

    mvpis_short = 0,         /* short (<= 7) vs long (>= 8) */
    MVPsign,                /* sign for non-zero */
    MVPshort,               /* 8 short values = 7-position tree */

    MVPbits = MVPshort + mvnum_short - 1, /* mvlong_width long value bits */
    MVPcount = MVPbits + mvlong_width    /* (with independent probabilities) */
} VP8_ENUM_0;

typedef enum
{
    VP8_VFMW_FALSE = 0,
    VP8_VFMW_TRUE  = 1
} VP8_TRUEFALSE;

#define VP8_NOTDEC  (-2)    //?¡§¡è?¡ìa?¡ìo?VP8DEC_Decode()?¡ì?¨¨????¡ì?D?¡ì?????NAL???¡ì?D?a????¡ìa??¨¨|¡§????¡ì?¨¦??¡§o??????|¡§????¡ì??2??¡ì????¡ìa???¡ì?¨¦???¡ì?¨¦?
typedef enum
{
    VP8_KEY_FRAME = 0,
    VP8_INTER_FRAME = 1
} VP8_FRAME_TYPE;

typedef enum
{
    VP8_SIXTAP   = 0,
    VP8_BILINEAR = 1
} VP8_INTERPOLATIONFILTERTYPE;

typedef enum
{
    NORMAL_LOOPFILTER = 0,
    SIMPLE_LOOPFILTER = 1
} VP8_LOOPFILTERTYPE;


/* Macroblock level features */
typedef enum
{
    MB_LVL_ALT_Q = 0,               /* Use alternate Quantizer .... */
    MB_LVL_ALT_LF = 1,              /* Use alternate loop filter value... */
    MB_LVL_MAX = 2                  /* Number of MB level features supported */
} MB_LVL_FEATURES;

typedef enum
{
    ONE_PARTITION  = 0,
    TWO_PARTITION  = 1,
    FOUR_PARTITION = 2,
    EIGHT_PARTITION = 3
} TOKEN_PARTITION;

typedef enum
{
    REG_YUV = 0,    /* Regular yuv */
    INT_YUV = 1     /* The type of yuv that can be tranfer to and from RGB through integer transform */
} YUV_TYPE;


typedef enum
{
    RECON_CLAMP_REQUIRED        = 0,
    RECON_CLAMP_NOTREQUIRED     = 1
} CLAMP_TYPE;


// Frame Header type
typedef struct
{
    UINT32 range;
    UINT32 value;
    UINT32 count;
} VP8_BOOLCODER_S;

typedef struct
{
    UINT8 prob[MVPcount];  /* often come in row, col pairs */
} MV_CONTEXT;


typedef struct
{
    UINT8 bmode_prob [VP8_BINTRAMODES - 1];

    //*this two can use table here*//
    UINT8 ymode_prob [VP8_YMODES - 1]; /* interframe intra mode probs */
    UINT8 uv_mode_prob [VP8_UV_MODES - 1];

    UINT8 sub_mv_ref_prob [VP8_SUBMVREFS - 1];
    UINT8 coef_probs [BLOCK_TYPES] [COEF_BANDS] [PREV_COEF_CONTEXTS] [vp8_coef_tokens - 1];

    MV_CONTEXT mvc[2];
    MV_CONTEXT pre_mvc[2];  /* not to caculate the mvcost for the frame if mvc doesn't change. */
} FRAME_CONTEXT;

typedef struct
{
    // The size of the surface we want to draw to
    UINT32 VideoFrameWidth;
    UINT32 VideoFrameHeight;

    SINT32 YStride;
    SINT32 UVStride;

    // The number of horizontal and vertical blocks encoded
    UINT32 HFragPixels;
    UINT32 VFragPixels;

    // The UINT32ended Horizontal Scale
    UINT32 HScale;
    UINT32 HRatio;

    // The UINT32ended Vertical Scale
    UINT32 VScale;
    UINT32 VRatio;

    // The way in which we UINT32ended
    UINT32 ScalingMode;
    UINT32 reserved;
    UINT32 ExpandedFrameWidth;
    UINT32 ExpandedFrameHeight;
} VP8_ScaleType;

typedef struct
{
    UINT8  quantizer_update;
    UINT8  mode_ref_lf_delta_update;
    UINT8  mode_ref_lf_delta_enabled;

    UINT8  base_qindex;
    UINT8  last_kf_gf_q;  /* Q used on the last GF or KF */

    SINT8  y1dc_delta_q;
    SINT8  y2dc_delta_q;
    SINT8  y2ac_delta_q;
    SINT8  uvdc_delta_q;
    SINT8  uvac_delta_q;

    UINT8  filter_level;
    UINT8  sharpness_level;
    UINT8  delta_sign;

    UINT8  multi_token_partition;
    UINT8  segmentation_enabled;
    UINT8  update_mb_segmentation_map;
    UINT8  update_mb_segmentation_data;
    UINT8  mb_segement_abs_delta;

    UINT8  mb_segment_tree_probs[VP8_MB_FEATURE_TREE_PROBS];
    UINT8  segment_prob_update;
    SINT8  segment_feature_data[MB_LVL_MAX][VP8_MAX_MB_SEGMENTS];
    UINT8  ref_frame_delta_update_flag;
    UINT8  mb_mode_delta_update_flag;

    SINT8  ref_lf_deltas[VP8_MAX_REF_LF_DELTAS];                     /* 0 = Intra, Last, GF, ARF */
    //SINT8  last_mode_lf_deltas[VP8_MAX_MODE_LF_DELTAS];            /* 0 = BPRED, ZERO_MV, MV, SPLIT */
    SINT8  mode_lf_deltas[VP8_MAX_MODE_LF_DELTAS];                   /* 0 = BPRED, ZERO_MV, MV, SPLIT */

    UINT8  refresh_last_frame;       /* Two state 0 = NO, 1 = YES */
    UINT8  refresh_golden_frame;     /* Two state 0 = NO, 1 = YES */
    UINT8  refresh_alt_ref_frame;    /* Two state 0 = NO, 1 = YES */
    UINT8  copy_buffer_to_gf;        /* 0 none, 1 Last to GF, 2 ARF to GF */
    UINT8  copy_buffer_to_arf;       /* 0 none, 1 Last to ARF, 2 GF to ARF */
    UINT8  refresh_entropy_probs;    /* Two state 0 = NO, 1 = YES */

    UINT8  ref_frame_sign_bias[VP8_MAXREF_FRMS];    /* Two state 0, 1 */

    UINT8  prob_intra;
    UINT8  prob_last;
    UINT8  prob_gf;
    UINT8  prob_skip_false;

    VP8_FRAME_TYPE frame_type;
    UINT32 show_frame;
    UINT32 version;
    UINT32 first_partition_length_in_bytes;

    /* profile settings */
    UINT32 mb_no_coeff_skip;
    UINT32 no_lpf;
    UINT32 simpler_lpf;
    UINT32 use_bilinear_mc_filter;
    UINT32 full_pixel;
    VP8_INTERPOLATIONFILTERTYPE mcomp_filter_type;

    /*pic width & height*/
    UINT32 pic_width_in_mb;
    UINT32 pic_height_in_mb;
    UINT32 pic_width_in_pix;
    UINT32 pic_height_in_pix;
    UINT32 horiz_scale;
    UINT32 vert_scale;
    UINT32 total_mbs;

    UINT32 color_space;
    UINT32 clr_type;
    UINT32 clamping_type;

    VP8_LOOPFILTERTYPE filter_type;

    VP8_BOOLCODER_S bc;

    FRAME_CONTEXT lfc; /* last frame entropy */
    FRAME_CONTEXT fc;  /* this frame entropy */

} VP8_PICTUREHEADER_S;


typedef struct
{
    UINT8  refresh_last_frame;       /* Two state 0 = NO, 1 = YES */
    UINT8  refresh_golden_frame;     /* Two state 0 = NO, 1 = YES */
    UINT8  refresh_alt_ref_frame;    /* Two state 0 = NO, 1 = YES */
    UINT8  copy_buffer_to_gf;        /* 0 none, 1 Last to GF, 2 ARF to GF */
    UINT8  copy_buffer_to_arf;       /* 0 none, 1 Last to ARF, 2 GF to ARF */
    UINT8  refresh_entropy_probs;    /* Two state 0 = NO, 1 = YES */

    VP8_PICTUREHEADER_S OldHeaderInfo;
    VP8_PICTUREHEADER_S NewHeaderInfo;

    UINT32 ddr_stride;
    UINT32 UsedDecMode;
    UINT32 RefNum;
    UINT32 Imgbuf_allocated;
    UINT32 LastFilterType;
    UINT32 LastSharpnessLevel;
    UADDR  SegIdAddr;
    /*ptr for ref & cur frame*/
    SINT32 GoldenFsID;
    SINT32 AltFsID;
    SINT32 LastFsID;
    SINT32 CurFsID;
    SINT32 ToQueFsID;
    VP8_FRAME_TYPE FrameType;
    VP8_FRAME_TYPE LastFrameType;
} VP8_CODECINF;


typedef struct
{
    SYNTAX_EXTRA_DATA_S *pstExtraData;
    VP8_DEC_PARAM_S    *pVp8DecParam;
    VP8_DEC_PARAM_S    Vp8DecParam;
    STREAM_PARAM      StreamParam;
    STREAM_PARAM	   *pVp8StreamParam;
    BS                 Bs;

    VP8_CODECINF       Vp8CodecInfo;
    IMAGE_VO_QUEUE     ImageQue;
    SINT32        	   ChanID;
    UINT32          FakeFrmHeightInMb;
    UINT32          FakeFrmWidthInMb;     
} VP8_CTX_S;


SINT32 VP8DEC_Init(VP8_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
SINT32 VP8DEC_Destroy(VP8_CTX_S *pCtx);
SINT32 VP8DEC_Decode(VP8_CTX_S *pCtx, DEC_STREAM_PACKET_S *pPacket);
SINT32 VP8DEC_RecycleImage(VP8_CTX_S *pCtx, UINT32 ImgID);
SINT32 VP8DEC_GetRemainImg(VP8_CTX_S *pCtx);
SINT32 VP8DEC_VDMPostProc(VP8_CTX_S *pCtx, SINT32 ErrRatio);
SINT32 VP8DEC_GetImageBuffer(VP8_CTX_S *pCtx);


#ifdef __cplusplus
}
#endif

#endif //__VP8_VFMW_H__
