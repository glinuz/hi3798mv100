
/*******************************************************************************
Copyright (C), 2009-2012, Huawei Tech. Co., Ltd.
File name: imedia_videnc.h
Author & ID: 宋小刚+00133955
Version: 1.00
Date:  2009-12-18
Description: 本文件包括了中间处理模块的接口函数及相关结构定义
Function List:

History:
* Revision 1.00  2010/03/09 10:00:00  songxg+00133955
* 初始版本编码完成
*
*******************************************************************************/
#ifndef __QUALITY_PARAMS_H__
#define __QUALITY_PARAMS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#include "iMedia_common.h"

#pragma pack(push, 1)

typedef enum ENUM_IVIDENC_ME_METHOD
{
    ME_DIA = 0,     /**< Diamond search*/
    ME_HEX,         /**< Hexagonal search */
    ME_UMH,         /**< Uneven Multi-hexagon search */
    //ME_FULL,      /**< Full motion search */
} ENUM_IVIDENC_ME_METHOD;

typedef enum ENUM_IVIDENC_RC_METHOD
{
    RC_CQP = 0,
    RC_CBR,
    RC_VBR,
} ENUM_IVIDENC_RC_METHOD;

typedef enum ENUM_IVIDENC_PICTURE_CODING_TYPE
{
    PIC_CODING_FRAME = 0,    /**< 帧编码 */
    PIC_CODING_FIELD,        /**< 场编码 */

    /**< 仅H264有效 */
    PIC_CODING_PICAFF,       /**< 帧场图像自适应编码(NO MBAFF) */
    PIC_CODING_MBAFF,        /**< 帧场宏块自适应编码(ONLY MBAFF) */
    PIC_CODING_PICAFF_MBAFF, /**< 帧场图像和宏块自适应编码 */
} ENUM_IVIDENC_PICTURE_CODING_TYPE;

typedef enum ENUM_IVIDENC_SCENECUT_METHOD
{
    SCENECUT_NONE = 0,       /* not detect scene-cut */
    SCENECUT_SIMPLE,         /* simple scene-cut detection method, real-time*/
    SCENECUT_COMPLEX,        /* complex scene-cut detection with fade */
} ENUM_IVIDENC_SCENECUT_METHOD;

/* Define B as reference picture method. */
typedef enum ENUM_IVIDENC_BREF_METHOD
{
    BREF_NONE,          /* no B picture as reference pic */
    BREF_PYRAMID        /* the center B ref picture as reference */
    //BREF_ALL          /* all B picture as reference pic, not support */
} ENUM_IVIDENC_BREF_METHOD;

typedef enum ENUM_IVIDENC_QUANT_TYPE
{
    QUANT_NORMAL,    /* normal quantization */
    QUANT_TRELLIS    /* trellis, Rate Distortion Optimized Quantization */
    //QUANT_AROUND   /* not support, Adaptive Rounding based on JVT-N011 */
} ENUM_IVIDENC_QUANT_TYPE;

typedef enum ENUM_IVIDENC_DIRECT_MODE
{
    DIRECT_NONE,         /* none */
    DIRECT_SPATIAL,      /* spatial direct mode */
    DIRECT_TEMPORAL,     /* temporal direct mode */
    DIRECT_ADAPT         /* adapt between spatial and temporal direct mode */
} ENUM_IVIDENC_DIRECT_MODE;

typedef enum ENUM_IVIDENC_WEIGHTED_PRED
{
    WP_OFF,
    WP_EXPLICIT,     /* Explicit Weighted Prediction, now not support */
    WP_IMPLICIT      /* Implicit Weighted Prediction */
} ENUM_IVIDENC_WEIGHTED_PRED;

typedef enum ENUM_IVIDENC_FILTER_TYPE
{
    FILTER_FILTER,    /* enable loop filter */
    FILTER_NONE,      /* disable loop filter*/
    FILTER_NOSLICE    /* no filter in slice edge */
} ENUM_IVIDENC_FILTER_TYPE;

/**< 仅H264有效 */
typedef enum ENUM_IVIDENC_SLICE_MODE
{
    SLICE_MODE_ONLY_ONE = 0,    /**< 整帧一个slice */
    SLICE_MODE_FIXMB,           /**< 按宏块数划分slice */
    SLICE_MODE_FIXBYTE,         /**< 按字节数划分slice */
    SLICE_MODE_STRIP,           /**< 按行划分slice（即一行宏块为一个） */
} ENUM_IVIDENC_SLICE_MODE;

/*!视频编码质量控制参数*/
// 对视频编码质量和性能有较大影响的参数，对算法较熟悉的人可通过下面参数优化（后续根据测试情况可能会有部分字段调整）
typedef struct STRU_IVIDENC_QUALITY_PARAMS
{
    ENUM_IVIDEO_CODEC_LEVEL  eLevel;    /**< level idc */
    ENUM_IVIDENC_PICTURE_CODING_TYPE ePicCodingType; /**< 图像编码类型 (FRAME, FIELD, PICAFF, ...) */
    INT32  iMultiPass;

    BOOL   bCabacFlag;      /**< 熵编码模式 0-CAVLC 1-CABAC */
    BOOL   bDCT8x8Flag;

    /**< 码控相关参数 */
    //INT32  iRCMethod;            /**< 码控类型，一期只有CBR(用ABR实现) */
    UINT32 uiVbvBufDelay;          /**< VBV Buffer, 单位ms, Range[500-30000] */
    //UINT32 uiInitVbvBuf;         /**< Init VBV Buffer, 单位ms */
    BOOL   bSkipFrameFlag;         /**< 主动跳帧标识，在无法满足目标码流时主动跳帧标记 */
    INT32  iChromaQPOffset;        /**< Chroma QP offset (-12..12) */
    INT32  iSecondChromaQPOffset;  /**< Secondary chroma QP offset  */
    //INT32  iQPISlice;             /**< I Slices初始QP值 (0-51) */
    //INT32  iQPSlice;              /**< non-I Slices初始QP值 */
    //INT32  iRCQPMax;              /**< 最大QP值  Range[0,51] */
    //INT32  iRCQPMin;              /**< 最小QP值 Range[0,51] */
    //INT32  iRCQPDiff;             /**< 码控调整时QP变化的最大值 */

    /**< 搜索算法相关参数 */
    ENUM_IVIDENC_ME_METHOD eMEMethod;  /**< ME类型: Full Search or Low Power ME or Hybrid ME */
    INT32  iSubMELevel;
    INT32  iMERange;
    INT32  iRefFrameNum;
    BOOL   bMixedRefFlag;
    BOOL   bChromaMEFlag;
    BOOL   bBiMEFlag;
    BOOL   bFastPSkipFlag;

    /**< 前处理算法相关参数 */
    ENUM_IVIDENC_SCENECUT_METHOD eScenecutType;

    /**< B帧相关参数 */
    ENUM_IVIDENC_BREF_METHOD eBFrameRefType;         /**< B帧做参考类型 */
    ENUM_IVIDENC_DIRECT_MODE eBFrameDirectMode;
    INT32  iBFrameNum;
    //INT32  iBFrameDirect8x8;
    BOOL   bBFrameAdaptFlag;
    BOOL   bBFrameRDOFlag;

    /**< 宏块模式相关参数 */
    BOOL  bI4x4Flag;        //use I4x4 or not.
    BOOL  bI8x8Flag;        //use I8x8 or not.
    BOOL  bPSub16x16Flag;   //use sub P16x16 or not.
    BOOL  bPSub8x8Flag;     //use sub P8x8 or not.
    BOOL  bBSub16x16Flag;   //use sub B16x16 or not.

    /**< 加权预测相关参数 */
    BOOL  bWPFlag;          //weighted prediction on or not.

    /**< 环路滤波相关参数 */
    ENUM_IVIDENC_FILTER_TYPE eLfDisableIdc;         /**< Loop Filter enable/disable control */
    INT32  iLFAlphaOffset;         /**< Alpha & C0 offset div. 2，Range[-6-6] */
    INT32  iLFBetaOffset;          /**< Beta offset div. 2, Range[-6-6] */

    ENUM_IVIDENC_QUANT_TYPE eQuantType;
    // slice partition
    ENUM_IVIDENC_SLICE_MODE eSliceMode;             /**< slice划分模式 */
    INT32  iSliceParam;            /**< 当slice划分模式为SLICE_MODE_FIXMB、SLICE_MODE_FIXBYTE时，相应的值 */
} STRU_IVIDENC_QUALITY_PARAMS;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#endif /**< __QUALITY_PARAMS_H__ */

/************************************************************************
* $Log$
************************************************************************/
