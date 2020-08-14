/******************************************************************************

  版权所有 (C), 2001-2015, 华为技术有限公司

******************************************************************************
    文 件 名   : vp9.h
    版 本 号   : 初稿
    作        者   : z00290437
    生成日期: 2015-02-01
    最近修改 :
    功能描述 : VP9语法解析头文件
         
  修改历史     :
    1.日    期       : 
    2.作    者       : 
    3.修改内容: 

******************************************************************************/

#ifndef _VP9_HEADER_
#define _VP9_HEADER_
#ifdef __cplusplus
extern "C" {
#endif

#include "basedef.h"
#include "vfmw.h"
#include "public.h"
#include "vdm_hal.h"
#include "bitstream.h"
#include "syn_cmn.h"

#include    "vfmw_osal_ext.h"

extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;

#define INLINE __inline
#define ALIGN8 __attribute__((aligned(4)))

#define VP9_DEC_ERR               -1
#define VP9_DEC_NORMAL         0
#define VP9_VFMW_FALSE         -1
#define VP9_VFMW_TRUE           0
#define VP9_NOTDEC             -2

//typedef UINT8 Vp9_Prob;
#define Vp9_Prob UINT8
typedef SINT8 Vp9_TreeIndex;
typedef Vp9_TreeIndex Vp9_Tree[];
typedef Vp9_TreeIndex *pVp9_Tree;
//typedef UINT32 VP9_VFMW_BD_VALUE;
#define VP9_VFMW_BD_VALUE UINT32 

#define ALIGN_POWER_OF_TWO(value, n) \
    (((value) + ((1 << (n)) - 1)) & ~((1 << (n)) - 1))       

#define ROUND_POWER_OF_TWO(value, n) \
    (((value) + (1 << ((n) - 1))) >> (n))

#define BLOCK_SIZE_GROUPS                        4
#define INTRA_MODES                                     (TM_PRED + 1)
#define MIN_TILE_WIDTH_B64                          4
#define MAX_TILE_WIDTH_B64                        64
#define MI_SIZE_LOG2                                      3
#define MI_BLOCK_SIZE_LOG2                       (6 - MI_SIZE_LOG2)  // 64 = 2^6
#define PARTITION_PLOFFSET                        4  // number of probability models per block size
#define NUM_PARTITION_CONTEXTS             (4 * PARTITION_PLOFFSET)
#define INTRA_INTER_CONTEXTS                   4
#define COMP_INTER_CONTEXTS                   5
#define REF_CONTEXTS                                  5
#define MAX_REF_LF_DELTAS                        4
#define MAX_REF_LF_DELTAS                        4
#define MAX_MODE_LF_DELTAS                     2
#define NUM_FRAME_CONTEXTS_LOG2        2
#define VP9_REF_TYPES                                 2
#define VP9_BLOCK_TYPES                            2
#define VP9_COEF_BANDS                              6
#define VP9_PREV_COEF_CONTEXTS            6
#define UNCONSTRAINED_NODES                   3
#define INTER_MODE_CONTEXTS                   7
#define NUM_FRAME_CONTEXTS                   (1 << NUM_FRAME_CONTEXTS_LOG2)
#define VP9_NUM_REF_FRAMES_LOG2         3
#define VP9_NUM_REF_FRAMES                   (1 << VP9_NUM_REF_FRAMES_LOG2)
#define VP9_SEGMENT_DELTADATA              0
#define VP9_MAX_SEGMENTS                         8
#define VP9_MAX_PROB                                  255
#define SEG_TREE_PROBS                            (VP9_MAX_SEGMENTS-1)
#define PREDICTION_PROBS                           3
#define VP9_BITDEPTH_8                                8
#define VP9_BITDEPTH_10                              10
#define VP9_BITDEPTH_12                              12
#define VP9_LOTS_OF_BITS                           0x40000000
#define VP9_SUBEXP_PARAM                          4
#define VP9_COEF_UPDATE_PROB                252
#define VP9_NMV_UPDATE_PROB                  252
#define VP9_MODE_UPDATE_PROB               252
//#define VP9_BD_VALUE_SIZE                          ((SINT32)sizeof(VP9_VFMW_BD_VALUE)*8)
#define VP9_BD_VALUE_SIZE                          32

typedef Vp9_Prob Vp9_CoeffProbsModel[VP9_REF_TYPES][VP9_COEF_BANDS][VP9_PREV_COEF_CONTEXTS][UNCONSTRAINED_NODES];
typedef UINT32 Vp9_Vfmw_CoeffCountModel[VP9_REF_TYPES][VP9_COEF_BANDS][VP9_PREV_COEF_CONTEXTS][UNCONSTRAINED_NODES + 1];

#define VP9_VFMW_MAX_COL_NUM              (VP9_MAX_PIC_WIDTH/256)
#define VP9_VFMW_MAX_ROW_NUM             (VP9_MAX_PIC_HEIGHT/256)

#define SWITCHABLE_FILTERS                       3
#define TX_SIZE_CONTEXTS                           2
#define MBSKIP_CONTEXTS                            3
#define VP9_MAXQ                                           255
#define QINDEX_BITS                                       8

/* Coefficient token alphabet */
#define VP9_ZERO_TOKEN                              0
#define VP9_ONE_TOKEN                                1
#define VP9_TWO_TOKEN                               2
#define VP9_EOB_MODEL_TOKEN                  3 
#define VP9_MODE_MV_COUNT_SAT	          20
#define VP9_BAND_COEFF_CONTEXTS(band) ((band) == 0 ? 3 : VP9_PREV_COEF_CONTEXTS)

#define INTER_MODES                                    (1 + NEWMV - NEARESTMV)
#define MV_JOINTS                                           4
#define VP9_SYNC_CODE_0                            0x49
#define VP9_SYNC_CODE_1                            0x83
#define VP9_SYNC_CODE_2                            0x42
#define MAX_LOOP_FILTER                             63

#define INTER_OFFSET(mode)                        ((mode) - NEARESTMV)

#define REF_INVALID_SCALE                           -1
#define REF_SCALE_SHIFT                              14

#define NUM_YV12_BUFFERS                                         (VP9_NUM_REF_FRAMES+1)//MAX_FRAME_NUM
#define ALLOWED_REFS_PER_FRAME                          3
#define COEF_COUNT_SAT                                             24
#define COEF_MAX_UPDATE_FACTOR                          112
#define COEF_COUNT_SAT_KEY                                    24
#define COEF_MAX_UPDATE_FACTOR_KEY                 112
#define COEF_COUNT_SAT_AFTER_KEY                      24
#define COEF_MAX_UPDATE_FACTOR_AFTER_KEY   128

#define VP9_MAX_PIC_WIDTH_IN_MI                              (VP9_MAX_PIC_WIDTH >> 3)
#define VP9_MV_CLASSES                                             11
#define VP9_CLASS0_BITS                                              1  /* bits at integer precision for class 0 */
#define VP9_CLASS0_SIZE                                             (1 << VP9_CLASS0_BITS)
#define VP9_MV_OFFSET_BITS                                     (VP9_MV_CLASSES + VP9_CLASS0_BITS - 2)
#define VP9_VFMW_SEGMENT_ABSDATA                     1

#define VP9_VFMW_COUNTS_COEF4X4_SIZE              (576)
#define VP9_VFMW_COUNTS_COEF8X8_SIZE              (576)
#define VP9_VFMW_COUNTS_COEF16X16_SIZE          (576)
#define VP9_VFMW_COUNTS_COEF32X32_SIZE           (576)
#define VP9_VFMW_COUNTS_PARTITION_SIZE            (64)
#define VP9_VFMW_COUNTS_YMODE_SIZE                  (40)
#define VP9_VFMW_COUNTS_UVMODE_SIZE               (100)
#define VP9_VFMW_COUNTS_SWINTERP_SIZE            (12)
#define VP9_VFMW_COUNTS_INETERMODE_SIZE        (28)
#define VP9_VFMW_COUNTS_INTRAINTER_SIZE           (8)
#define VP9_VFMW_COUNTS_COMPINTER_SIZE          (10)
#define VP9_VFMW_COUNTS_SINGLEREF_SIZE           (20)
#define VP9_VFMW_COUNTS_COMPREF_SIZE             (10)
#define VP9_VFMW_COUNTS_P8X8_SIZE                      (4)
#define VP9_VFMW_COUNTS_P16X16_SIZE                  (6)
#define VP9_VFMW_COUNTS_P32X32_SIZE                  (8)
#define VP9_VFMW_COUNTS_MBSKIP_SIZE                  (6)
#define VP9_VFMW_COUNTS_JOINTS_SIZE                  (4)
#define VP9_VFMW_COUNTS_SIGN_SIZE                      (4)
#define VP9_VFMW_COUNTS_CLASSES_SIZE              (22)
#define VP9_VFMW_COUNTS_CLASS0_SIZE                 (4)
#define VP9_VFMW_COUNTS_BITS_SIZE                       (40)
#define VP9_VFMW_COUNTS_CLASS0FP_SIZE            (16)
#define VP9_VFMW_COUNTS_FP_SIZE                          (8)
#define VP9_VFMW_COUNTS_CLASS0HP_SIZE             (4)
#define VP9_VFMW_COUNTS_HP_SIZE                          (4)

#define VP9_VFMW_COUNTS_COEF4X4_OFFSET        (0)
#define VP9_VFMW_COUNTS_COEF8X8_OFFSET        (VP9_VFMW_COUNTS_COEF4X4_OFFSET + VP9_VFMW_COUNTS_COEF4X4_SIZE)
#define VP9_VFMW_COUNTS_COEF16X16_OFFSET    (VP9_VFMW_COUNTS_COEF8X8_OFFSET + VP9_VFMW_COUNTS_COEF8X8_SIZE)
#define VP9_VFMW_COUNTS_COEF32X32_OFFSET    (VP9_VFMW_COUNTS_COEF16X16_OFFSET + VP9_VFMW_COUNTS_COEF16X16_SIZE)
#define VP9_VFMW_COUNTS_PARTITION_OFFSET      (VP9_VFMW_COUNTS_COEF32X32_OFFSET + VP9_VFMW_COUNTS_COEF32X32_SIZE)
#define VP9_VFMW_COUNTS_YMODE_OFFSET            (VP9_VFMW_COUNTS_PARTITION_OFFSET + VP9_VFMW_COUNTS_PARTITION_SIZE)
#define VP9_VFMW_COUNTS_UVMODE_OFFSET          (VP9_VFMW_COUNTS_YMODE_OFFSET + VP9_VFMW_COUNTS_YMODE_SIZE)
#define VP9_VFMW_COUNTS_SWINTERP_OFFSET       (VP9_VFMW_COUNTS_UVMODE_OFFSET + VP9_VFMW_COUNTS_UVMODE_SIZE)
#define VP9_VFMW_COUNTS_INETERMODE_OFFSET  (VP9_VFMW_COUNTS_SWINTERP_OFFSET + VP9_VFMW_COUNTS_SWINTERP_SIZE)
#define VP9_VFMW_COUNTS_INTRAINTER_OFFSET     (VP9_VFMW_COUNTS_INETERMODE_OFFSET + VP9_VFMW_COUNTS_INETERMODE_SIZE)
#define VP9_VFMW_COUNTS_COMPINTER_OFFSET     (VP9_VFMW_COUNTS_INTRAINTER_OFFSET + VP9_VFMW_COUNTS_INTRAINTER_SIZE)
#define VP9_VFMW_COUNTS_SINGLEREF_OFFSET      (VP9_VFMW_COUNTS_COMPINTER_OFFSET + VP9_VFMW_COUNTS_COMPINTER_SIZE)
#define VP9_VFMW_COUNTS_COMPREF_OFFSET        (VP9_VFMW_COUNTS_SINGLEREF_OFFSET + VP9_VFMW_COUNTS_SINGLEREF_SIZE)
#define VP9_VFMW_COUNTS_P8X8_OFFSET                 (VP9_VFMW_COUNTS_COMPREF_OFFSET + VP9_VFMW_COUNTS_COMPREF_SIZE)
#define VP9_VFMW_COUNTS_P16X16_OFFSET             (VP9_VFMW_COUNTS_P8X8_OFFSET + VP9_VFMW_COUNTS_P8X8_SIZE)
#define VP9_VFMW_COUNTS_P32X32_OFFSET             (VP9_VFMW_COUNTS_P16X16_OFFSET + VP9_VFMW_COUNTS_P16X16_SIZE)
#define VP9_VFMW_COUNTS_MBSKIP_OFFSET             (VP9_VFMW_COUNTS_P32X32_OFFSET + VP9_VFMW_COUNTS_P32X32_SIZE)
#define VP9_VFMW_COUNTS_JOINTS_OFFSET              (VP9_VFMW_COUNTS_MBSKIP_OFFSET + VP9_VFMW_COUNTS_MBSKIP_SIZE)
#define VP9_VFMW_COUNTS_SIGN_OFFSET                  (VP9_VFMW_COUNTS_JOINTS_OFFSET + VP9_VFMW_COUNTS_JOINTS_SIZE)
#define VP9_VFMW_COUNTS_CLASSES_OFFSET          (VP9_VFMW_COUNTS_SIGN_OFFSET + VP9_VFMW_COUNTS_SIGN_SIZE)
#define VP9_VFMW_COUNTS_CLASS0_OFFSET             (VP9_VFMW_COUNTS_CLASSES_OFFSET + VP9_VFMW_COUNTS_CLASSES_SIZE)
#define VP9_VFMW_COUNTS_BITS_OFFSET                   (VP9_VFMW_COUNTS_CLASS0_OFFSET + VP9_VFMW_COUNTS_CLASS0_SIZE)
#define VP9_VFMW_COUNTS_CLASS0FP_OFFSET         (VP9_VFMW_COUNTS_BITS_OFFSET + VP9_VFMW_COUNTS_BITS_SIZE)
#define VP9_VFMW_COUNTS_FP_OFFSET                      (VP9_VFMW_COUNTS_CLASS0FP_OFFSET + VP9_VFMW_COUNTS_CLASS0FP_SIZE)
#define VP9_VFMW_COUNTS_CLASS0HP_OFFSET         (VP9_VFMW_COUNTS_FP_OFFSET + VP9_VFMW_COUNTS_FP_SIZE)
#define VP9_VFMW_COUNTS_HP_OFFSET                      (VP9_VFMW_COUNTS_CLASS0HP_OFFSET + VP9_VFMW_COUNTS_CLASS0HP_SIZE)
#define VP9_VFMW_COUNTS_TOTAL                               (VP9_VFMW_COUNTS_HP_OFFSET + VP9_VFMW_COUNTS_HP_SIZE)

typedef enum
{
    TX_4X4     = 0,    // 4x4 dct transform    
    TX_8X8     = 1,    // 8x8 dct transform    
    TX_16X16 = 2,    // 16x16 dct transform    
    TX_32X32 = 3,    // 32x32 dct transform    
    TX_SIZES
} TX_SIZE;

typedef enum
{
    ONLY_4X4                = 0,    
    ALLOW_8X8             = 1,    
    ALLOW_16X16         = 2,    
    ALLOW_32X32         = 3,    
    TX_MODE_SELECT = 4,    
    TX_MODES              = 5,
}TX_MODE;

typedef enum
{
    EIGHTTAP                   = 0,    
    EIGHTTAP_SMOOTH  = 1,    
    EIGHTTAP_SHARP     = 2,    
    BILINEAR                     = 3,    
    SWITCHABLE              = 4 
} VP9_INTERPOLATIONFILTERTYPE;

typedef enum
{
    PARTITION_NONE,    
    PARTITION_HORZ,    
    PARTITION_VERT,    
    PARTITION_SPLIT,    
    PARTITION_TYPES,    
    PARTITION_INVALID = PARTITION_TYPES
} PARTITION_TYPE;

typedef enum
{
    SEG_LVL_ALT_Q           = 0,          // Use alternate Quantizer ....    
    SEG_LVL_ALT_LF         = 1,          // Use alternate loop filter value...    
    SEG_LVL_REF_FRAME = 2,          // Optional Segment reference frame    
    SEG_LVL_SKIP              = 3,           // Optional Segment (0,0) + skip mode    
    SEG_LVL_MAX               = 4            // Number of features supported
} SEG_LVL_FEATURES;

typedef enum
{
    NONE                             = -1,    
    INTRA_FRAME               = 0,    
    LAST_FRAME                = 1,    
    GOLDEN_FRAME           = 2,    
    ALTREF_FRAME            = 3,    
    MAX_REF_FRAMES       = 4
} MV_REFERENCE_FRAME;

typedef enum
{
    SINGLE_PREDICTION_ONLY  = 0,    
    COMP_PREDICTION_ONLY    = 1,    
    HYBRID_PREDICTION             = 2,    
    NB_PREDICTION_TYPES        = 3,
} COMPPREDMODE_TYPE;

typedef enum
{
    MV_JOINT_ZERO    = 0,           /* Zero vector */    
    MV_JOINT_HNZVZ   = 1,           /* Vert zero, hor nonzero */    
    MV_JOINT_HZVNZ   = 2,            /* Hor zero, vert nonzero */    
    MV_JOINT_HNZVNZ = 3,           /* Both components nonzero */
} MV_JOINT_TYPE;

typedef enum
{
    MV_CLASS_0 = 0,      /* (0, 2]     integer pel */    
    MV_CLASS_1 = 1,      /* (2, 4]     integer pel */    
    MV_CLASS_2 = 2,      /* (4, 8]     integer pel */    
    MV_CLASS_3 = 3,      /* (8, 16]    integer pel */    
    MV_CLASS_4 = 4,      /* (16, 32]   integer pel */    
    MV_CLASS_5 = 5,      /* (32, 64]   integer pel */    
    MV_CLASS_6 = 6,      /* (64, 128]  integer pel */    
    MV_CLASS_7 = 7,      /* (128, 256] integer pel */   
    MV_CLASS_8 = 8,      /* (256, 512] integer pel */    
    MV_CLASS_9 = 9,      /* (512, 1024] integer pel */    
    MV_CLASS_10 = 10,   /* (1024,2048] integer pel */
} MV_CLASS_TYPE;

typedef enum
{
    VP9_KEY_FRAME = 0,    
    VP9_INTER_FRAME = 1,    
    VP9_NUM_FRAME_TYPES,
} VP9_FRAME_TYPE;

typedef enum
{
    DC_PRED,          // Average of above and left pixels    
    V_PRED,            // Vertical    
    H_PRED,            // Horizontal    
    D45_PRED,        // Directional 45  deg = round(arctan(1/1) * 180/pi)    
    D135_PRED,      // Directional 135 deg = 180 - 45    
    D117_PRED,      // Directional 117 deg = 180 - 63    
    D153_PRED,      // Directional 153 deg = 180 - 27    
    D207_PRED,       // Directional 207 deg = 180 + 27    
    D63_PRED,         // Directional 63  deg = round(arctan(2/1) * 180/pi)    
    TM_PRED,          // True-motion    
    NEARESTMV,    
    NEARMV,    
    ZEROMV,    
    NEWMV,    
    MB_MODE_COUNT
} MB_PREDICTION_MODE;

typedef enum
{
    VP9_BSPROFILE_0,
    VP9_BSPROFILE_1,
    VP9_BSPROFILE_2,
    VP9_BSPROFILE_3,
    VP9_BSPROFILE_MAX
} VP9_VFMW_BSPROFILE_E;

typedef enum
{
    VP9_UNKNOWN = 0,
    VP9_BT_601 = 1,
    VP9_BT_709 = 2,
    VP9_SMPTE_170 = 3,
    VP9_SMPTE_240 = 4,
    VP9_BT_2020 = 5,
    VP9_BT_RESERVED = 6,
    VP9_BT_SRGB = 7
} VP9_VFMW_COLORSPACE_E;

typedef struct
{
    SINT32 new_fb_idx;
    VP9_FRAME_TYPE last_frame_type;
    VP9_FRAME_TYPE frame_type;    
    VP9_VFMW_BSPROFILE_E eProfile;
    SINT32 color_space;
    SINT32 pixvalue_type;
    SINT32 show_frame;    
    SINT32 repeat_frame;
    SINT32 frame_to_show;
    SINT32 last_show_frame;    
    SINT32 error_resilient_mode;    
    SINT32 subsampling_x;    
    SINT32 subsampling_y;    
    SINT32 width;    
    SINT32 height;    
    SINT32 display_width;    
    SINT32 display_height;    
    SINT32 bit_depth;    
    SINT32 last_width;    
    SINT32 last_height;    
    SINT32 refresh_frame_context;     
    SINT32 frame_parallel_decoding_mode;    
    UINT32 frame_context_idx;    
    SINT32 mi_cols;    
    SINT32 mi_rows;    
    SINT32 log2_tile_cols;    
    SINT32 log2_tile_rows;    
    SINT32 intra_only;    
    SINT32 reset_frame_context;    
    SINT32 refresh_frame_flags;   
    VP9_INTERPOLATIONFILTERTYPE mcomp_filter_type;
} VP9_CURRPIC_S;

typedef struct
{
    SINT32 base_qindex;
    SINT32 y_dc_delta_q;
    SINT32 uv_dc_delta_q;
    SINT32 uv_ac_delta_q;
} PIC_QUANT;

typedef struct
{
    UINT8 mode_ref_delta_enabled;    
    UINT8 mode_ref_delta_update;
    SINT8 ref_deltas[MAX_REF_LF_DELTAS];    
    SINT8 mode_deltas[MAX_MODE_LF_DELTAS];    
    SINT32 filter_level;    
    SINT32 sharpness_level;    
} VP9_LOOPFILTER_S;

typedef struct
{
    UINT8 lvl[VP9_MAX_SEGMENTS][MAX_REF_FRAMES][MAX_MODE_LF_DELTAS];    
    UINT8 mode_lf_lut[MB_MODE_COUNT];
} VP9_LOOPFILTERINFO_N_S;

typedef struct
{
    UINT8 enabled;    
    UINT8 update_map;    
    UINT8 update_data;    
    UINT8 abs_delta;    
    UINT8 temporal_update;    
    Vp9_Prob tree_probs[SEG_TREE_PROBS];    
    Vp9_Prob pred_probs[PREDICTION_PROBS];    
    SINT16 feature_data[VP9_MAX_SEGMENTS][SEG_LVL_MAX];    
    UINT32 feature_mask[VP9_MAX_SEGMENTS];
} VP9_SEGMENTATION_S;

typedef struct
{
    SINT32 y_width;    
    SINT32 y_height;    
    SINT32 y_crop_width;    
    SINT32 y_crop_height;    
}VP9_YV12BUFFER_S;

typedef struct
{
    int x_scale_fp;
    int y_scale_fp;
    int x_step_q4;  
    int y_step_q4;
} VP9_SCALE_FACTORS;

typedef struct
{
    Vp9_Prob p32x32[TX_SIZE_CONTEXTS][TX_SIZES - 1];    
    Vp9_Prob p16x16[TX_SIZE_CONTEXTS][TX_SIZES - 2];    
    Vp9_Prob p8x8[TX_SIZE_CONTEXTS][TX_SIZES - 3];
} VP9_VFMW_TX_PROBS_S;

typedef struct
{
    Vp9_Prob sign;    
    Vp9_Prob classes[VP9_MV_CLASSES - 1];    
    Vp9_Prob class0[VP9_CLASS0_SIZE - 1];    
    Vp9_Prob bits[VP9_MV_OFFSET_BITS];    
    Vp9_Prob class0_fp[VP9_CLASS0_SIZE][4 - 1];    
    Vp9_Prob fp[4 - 1];    
    Vp9_Prob class0_hp;    
    Vp9_Prob hp;
} VP9_VFMW_NMV_COMPONENT_S;

typedef struct
{
    Vp9_Prob joints[MV_JOINTS - 1];    
    VP9_VFMW_NMV_COMPONENT_S comps[2];
} VP9_NMV_CONTEXT_S;

typedef struct
{
    UINT32 *sign;//UINT32 sign[2];    
    UINT32 *classes;//UINT32 classes[VP9_MV_CLASSES];    
    UINT32 *class0;//UINT32 class0[VP9_CLASS0_SIZE];    
    UINT32 *bits;//UINT32 bits[VP9_MV_OFFSET_BITS][2];    
    UINT32 *class0_fp;//UINT32 class0_fp[VP9_CLASS0_SIZE][4];    
    UINT32 *fp;//UINT32 fp[4];    
    UINT32 *class0_hp;//UINT32 class0_hp[2];    
    UINT32 *hp;//UINT32 hp[2];
} VP9_VFMW_NMV_COMPONENT_COUNTS_S;

typedef struct
{
    UINT32 *joints;//UINT32 joints[MV_JOINTS];    
    VP9_VFMW_NMV_COMPONENT_COUNTS_S comps[2];
} VP9_VFMW_NMV_CONTEXT_COUNTS_S;

typedef struct
{
    Vp9_CoeffProbsModel coef_probs[TX_SIZES][VP9_BLOCK_TYPES];    
    Vp9_Prob y_mode_prob[BLOCK_SIZE_GROUPS][INTRA_MODES - 1];    
    Vp9_Prob uv_mode_prob[INTRA_MODES][INTRA_MODES - 1];    
    Vp9_Prob partition_prob[VP9_NUM_FRAME_TYPES][NUM_PARTITION_CONTEXTS][PARTITION_TYPES - 1];    
    Vp9_Prob switchable_interp_prob[SWITCHABLE_FILTERS + 1][SWITCHABLE_FILTERS - 1];    
    Vp9_Prob inter_mode_probs[INTER_MODE_CONTEXTS][INTER_MODES - 1];    
    Vp9_Prob intra_inter_prob[INTRA_INTER_CONTEXTS];   
    Vp9_Prob comp_inter_prob[COMP_INTER_CONTEXTS];    
    Vp9_Prob single_ref_prob[REF_CONTEXTS][2];    
    Vp9_Prob comp_ref_prob[REF_CONTEXTS];    
    VP9_VFMW_TX_PROBS_S tx_probs;    
    Vp9_Prob mbskip_probs[MBSKIP_CONTEXTS];    
    VP9_NMV_CONTEXT_S nmvc;
} VP9_FRAME_CONTEXT_S;//sizeof()=2KB

typedef struct
{
    UINT32 *p32x32;//UINT32 p32x32[TX_SIZE_CONTEXTS][TX_SIZES];    
    UINT32 *p16x16;//UINT32 p16x16[TX_SIZE_CONTEXTS][TX_SIZES - 1];    
    UINT32 *p8x8;//UINT32 p8x8[TX_SIZE_CONTEXTS][TX_SIZES - 2];
} VP9_VFMW_TX_COUNTS_S;

typedef struct
{
    Vp9_Vfmw_CoeffCountModel coef[TX_SIZES][VP9_BLOCK_TYPES];    
    UINT32 *y_mode;//UINT32 y_mode[BLOCK_SIZE_GROUPS][INTRA_MODES];    
    UINT32 *uv_mode;//UINT32 uv_mode[INTRA_MODES][INTRA_MODES];    
    UINT32 *partition;//UINT32 partition[NUM_PARTITION_CONTEXTS][PARTITION_TYPES];    
    UINT32 eob_branch[TX_SIZES][VP9_BLOCK_TYPES][VP9_REF_TYPES][VP9_COEF_BANDS][VP9_PREV_COEF_CONTEXTS];    
    UINT32 *switchable_interp;//UINT32 switchable_interp[SWITCHABLE_FILTERS + 1][SWITCHABLE_FILTERS];    
    UINT32 *inter_mode;//UINT32 inter_mode[INTER_MODE_CONTEXTS][INTER_MODES];    
    UINT32 *intra_inter;//UINT32 intra_inter[INTRA_INTER_CONTEXTS][2];    
    UINT32 *comp_inter;//UINT32 comp_inter[COMP_INTER_CONTEXTS][2];    
    UINT32 *single_ref;//UINT32 single_ref[REF_CONTEXTS][2][2];    
    UINT32 *comp_ref;//UINT32 comp_ref[REF_CONTEXTS][2];    
    VP9_VFMW_TX_COUNTS_S tx;    
    UINT32 *mbskip;//UINT32 mbskip[MBSKIP_CONTEXTS][2];    
    VP9_VFMW_NMV_CONTEXT_COUNTS_S mv;
} VP9_FRAME_COUNTS_S;

typedef struct
{
    UINT8  *pHead;
    UINT8  *pTail;
    UINT32 Bufa;
    UINT32 Bufb;
    SINT32 BsLen;
    SINT32 BufPos;
    SINT32 TotalPos;
    VP9_VFMW_BD_VALUE value;
    SINT32 count;
    UINT32 range;
} VP9_BSSED_S;

typedef struct
{
    UINT32 first_partition_size;    
    UINT32 pic_width_in_mb;
    UINT32 pic_height_in_mb;
    UINT32 pic_width_in_pix;
    UINT32 pic_height_in_pix;
    UINT32 segment_id_addr;    
    UINT32 segment_id_stride;    
    UINT32 curr_rcn_addr;    
    UINT32 alt_ref_addr;    
    UINT32 golden_ref_addr;    
    UINT32 last_ref_addr; 
    
    SINT32 lossless;    
    SINT32 allow_high_precision_mv;
    SINT32 allow_comp_inter_inter;    
    
    SINT32 ref_frame_sign_bias[MAX_REF_FRAMES];
    SINT32 fb_idx_ref_cnt[NUM_YV12_BUFFERS]; /* reference counts */    
    SINT32 fb_idx_ref_cnt_pre[NUM_YV12_BUFFERS];
    SINT32 ref_frame_map[VP9_NUM_REF_FRAMES]; /* maps fb_idx to reference slot */    
    SINT32 active_ref_idx[ALLOWED_REFS_PER_FRAME];    
    SINT32 idx_map_fsid[NUM_YV12_BUFFERS];  /*map dpb idx to fsp logic id*/

    TX_MODE tx_mode;    
    COMPPREDMODE_TYPE comp_pred_mode;    
    PIC_QUANT pic_q;    
    VP9_LOOPFILTERINFO_N_S lf_info;    
    VP9_LOOPFILTER_S lf;    
    VP9_FRAME_CONTEXT_S fc;    
    VP9_CURRPIC_S cp;    
    VP9_SEGMENTATION_S seg;    
    VP9_YV12BUFFER_S yv12_fb[NUM_YV12_BUFFERS];    
    VP9_SCALE_FACTORS scale_factors[ALLOWED_REFS_PER_FRAME];
} VP9_PIC_HEADER_S;

typedef struct
{
    UINT32 bit_stream_address_0;    
    UINT8 bit_offset_0;    
    UINT32 bit_len_0;    
    UINT32 bit_stream_address_1;    
    UINT8 bit_offset_1;    
    UINT32 bit_len_1;    
    UINT16 TileStartY;    
    UINT16 TileEndY;    
    UINT16 TileStartX;    
    UINT16 TileEndX;    
    UINT32 end_ctb_in_raster_addr;   
    UINT32 end_ctb_in_tile_addr;    
    UINT32 first_ctb_in_raster_addr;    
    UINT32 first_ctb_in_tile_addr;    
    ULONG next_tile_para_addr;
} VP9_TILE_INFO_S;

typedef struct
{
    UINT32 UsedDecMode;      
    UINT32 Imgbuf_allocated;
    UINT32 LastFilterType;
    UINT32 LastSharpnessLevel;
    UINT32 SegIdAddr;
    UINT32 SegIdSize;
    UINT8 *SegIdVirAddr;
    UINT32 SegIdStride;
    UINT32 CompressEn;
    
    SINT32 GoldenFsID;
    SINT32 AltFsID;
    SINT32 LastFsID;
    SINT32 CurFsID;	
    SINT32 ToQueFsID;	

    VP9_FRAME_TYPE FrameType;   
    VP9_FRAME_TYPE LastFrameType;
    VP9_PIC_HEADER_S NewHeaderInfo;
} VP9_CODECINFO;

typedef struct
{
    SYNTAX_EXTRA_DATA_S  *pstExtraData;	
    VP9_DEC_PARAM_S *pVp9DecParam;
    VP9_DEC_PARAM_S Vp9DecParam;	
    STREAM_PARAM StreamParam;	
    VP9_CODECINFO Vp9CodecInfo;
    IMAGE_VO_QUEUE ImageQue;
    BS Bs;
    
    VP9_FRAME_CONTEXT_S sFrameContext;
    VP9_FRAME_CONTEXT_S saFrameContexts[NUM_FRAME_CONTEXTS];
    VP9_FRAME_COUNTS_S sCounts;

    SINT32 CurFrmSize;
    SINT32 CurFrmNum;
    SINT32 NeedFrmNum;
    SINT32 CurRefNum;
    SINT32 MaxRefNum;
    UINT32 PreWidth;
    UINT32 PreHeight;
    UINT32 last_width;    //for frame msg
    UINT32 last_height;
    SINT32 cur_fb_idx;
    SINT32 IsCurNalBusy;
    SINT32 IsSuperFrame;
    SINT32 frameSize[8];
    SINT32 frameNum;
    SINT32 frameCount;
    UINT8* dataStart;
    UINT8* dataEnd;
    UINT32 dataSize;

    UINT32 uiKeyFrame;
    UINT32 uiIntraOnly;
    TX_MODE uiTxMode;    
    VP9_FRAME_TYPE eFrameType;
    VP9_FRAME_TYPE eLastFrameType;
    UINT32 uiErrorResilientMode;
    UINT32 uiFrameParallelDecodingMode;
    UINT32 uiAllowHighPrecisionMv;
    UINT32 uiMcompFilterType;
    
    SINT32 ChanID;
    UINT32 TotalTileNum;
    UINT32 iTileNumber;
    UINT32 QpSum;
    UINT32 QpNum;
} VP9_CTX_S;

SINT32 VP9DEC_Init(VP9_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
SINT32 VP9DEC_DecodePacket(VP9_CTX_S *pCtx, DEC_STREAM_PACKET_S *pPacket);
SINT32 VP9DEC_DecodeFame(VP9_CTX_S *pCtx,  UINT8 *data_s);
SINT32 VP9DEC_VDMPostProc(VP9_CTX_S *pCtx, SINT32 ErrRatio, SINT32 ModuleLowlyEnable, SINT32 VdhId);
SINT32 VP9DEC_GetRemainImg(VP9_CTX_S *pCtx);
SINT32 VP9DEC_GetImageBuffer(VP9_CTX_S *pCtx);
SINT32 VP9DEC_RecycleImage(VP9_CTX_S *pCtx, UINT32 ImgID);

VOID VP9_InitDecPara(VP9_CTX_S *pCtx);
VOID Vp9_SetPoolInfoFrame(  VP9_CTX_S *pCtx, UINT32 *puiPoolInfoFrame);
VOID Vp9_SetPoolInfoTile(VP9_CTX_S *pCtx, UINT32 *puiPoolInfoTile, SINT32, VP9_TILE_INFO_S *);
VOID Vp9_ProbBurstConvert(UINT32 *puiBurstIn, VP9_FRAME_CONTEXT_S *, VP9_SEGMENTATION_S *, VP9_FRAME_TYPE, UINT32);
VOID Vp9_RestoreCounts(VP9_CTX_S *pCtx, UINT32 *puiProbCountsBuf, VP9_FRAME_COUNTS_S *psFrameCounts);
VOID Vp9_ReadCoefProbs(VP9_FRAME_CONTEXT_S *fc, TX_MODE tx_mode, VP9_BSSED_S *r);
VOID Vp9_PrepareReadModeInfo(VP9_FRAME_CONTEXT_S *fc, VP9_BSSED_S *r, SINT32 frame_type, 
        SINT32 intra_only, SINT32 mcomp_filter_type, SINT32 allow_comp_inter_inter, 
        SINT32 allow_high_precision_mv, SINT32 *out_comp_pred_mode);
SINT32 Vp9_ReaderHasError(VP9_BSSED_S *r);
VOID Vp9_DiffUpdateProb(VP9_BSSED_S *r, Vp9_Prob *p);
VOID Vp9_ReadCoefProbsCommon(Vp9_CoeffProbsModel *coef_probs, VP9_BSSED_S *r);
VOID Vp9_ReadInterModeProbs(VP9_FRAME_CONTEXT_S *fc, VP9_BSSED_S *r);
VOID Vp9_ReadSwitchableInterpProbs(VP9_FRAME_CONTEXT_S *fc, VP9_BSSED_S *r);
VOID Vp9_ReadCompPred(VP9_FRAME_CONTEXT_S *fc, VP9_BSSED_S *r, 
        SINT32 allow_comp_inter_inter, SINT32 *out_comp_pred_mode);
VOID Vp9_ReadMvProbs(VP9_BSSED_S *r, VP9_NMV_CONTEXT_S *mvc, SINT32 allow_hp);
VOID Vp9_ReadMvProbs(VP9_BSSED_S *r, VP9_NMV_CONTEXT_S *mvc, SINT32 allow_hp);
VOID Vp9_AdaptCoefProbs(VP9_FRAME_CONTEXT_S *fc, VP9_FRAME_CONTEXT_S *pre_fc,
	VP9_FRAME_COUNTS_S *counts, TX_SIZE tx_size, UINT32 count_sat, UINT32 update_factor);
VOID Vp9_AdaptResidualProbs(VP9_FRAME_CONTEXT_S *frame_contexts,VP9_FRAME_CONTEXT_S *fc,
    VP9_FRAME_COUNTS_S *counts, SINT32 frame_context_idx,VP9_FRAME_TYPE frame_type,
    SINT32 intra_only,VP9_FRAME_TYPE last_frame_type);
UINT32 Vp9_AdaptProbs(UINT32 i, const Vp9_Tree tree, Vp9_Prob this_probs[], Vp9_Prob last_probs[], UINT32 num_events[]);
VOID Vp9_AdaptModeProbs(VP9_FRAME_CONTEXT_S *fc,VP9_FRAME_CONTEXT_S *pre_fc,
    VP9_FRAME_COUNTS_S *counts,SINT32 mcomp_filter_type,SINT32 tx_mode);
VOID Vp9_AdaptMvProbs(VP9_FRAME_CONTEXT_S *fc, VP9_FRAME_CONTEXT_S *pre_fc, 
    VP9_FRAME_COUNTS_S *counts, SINT32 allow_hp);

#ifdef __cplusplus
}
#endif
#endif 

//_VP9_HEADER_


