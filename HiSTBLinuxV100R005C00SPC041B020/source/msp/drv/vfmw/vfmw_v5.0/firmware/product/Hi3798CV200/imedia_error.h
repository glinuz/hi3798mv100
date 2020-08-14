#ifndef __IMEDIA_ERROR_H__
#define __IMEDIA_ERROR_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

/** 模块返回值枚举类型定义 */
typedef enum ENUM_IMEDIA_RETURN
{
    IMEDIA_RET_SUCCESS = 0,             /**< 正常返回 */

    /** 参数类问题返回 */
    IMEDIA_RET_PARAM_NULL = 0x0100,     /**< 参数指针为空 */
    IMEDIA_RET_PARAM_INVALID,           /**< 参数无效 */
    IMEDIA_RET_PARAM_IN_VALUE_NULL,     /**< 输入参数值指针为空 */
    IMEDIA_RET_PARAM_IN_VALUE_INVALID,  /**< 输入参数值无效 */
    IMEDIA_RET_PARAM_OUT_VALUE_NULL,    /**< 输出参数值指针为空 */
    IMEDIA_RET_PARAM_OUT_VALUE_INVALID, /**< 输出参数值无效 */
    IMEDIA_RET_PARAM_NOT_ENOUGH,        /**< 参数不足 */

    IMEDIA_RET_PARAM_OTHER = 0x01FF,    /**< 参数类其它问题 */

    /** 对象类问题返回 */
    IMEDIA_RET_OBJ_NULL = 0x0200,       /**< 对象指针为空 */
    IMEDIA_RET_OBJ_BAD_MASK,            /**< 对象防错码毁坏 */
    IMEDIA_RET_OBJ_INVALID,             /**< 无效对象 */
    IMEDIA_RET_OBJ_NOT_FOUND,           /**< 对象没找到 */
    IMEDIA_RET_OBJ_CREATE_FAIL,         /**< 对象创建失败 */
    IMEDIA_RET_OBJ_NOT_INIT,            /**< 对象未初始化 */
    IMEDIA_RET_OBJ_DELETE_FAIL,         /**< 对象删除失败 */

    IMEDIA_RET_OBJ_OTHER = 0x02FF,      /**< 对象类其他问题 */

    /** 缓存（码流）类问题返回 */
    IMEDIA_RET_MEM_MALLOC_FAIL = 0x0400,/**< 内存分配失败 */
    IMEDIA_RET_MEM_FREE_FAIL,           /**< 内存释放失败 */
    IMEDIA_RET_BUF_NULL,                /**< 缓存指针为空 */
    IMEDIA_RET_BUF_BAD_MASK,            /**< 缓存防错码毁坏 */
    IMEDIA_RET_BUF_INVALID,             /**< 码流无效，无法识别格式 */
    IMEDIA_RET_BUF_OVER_FLOW,           /**< 缓存溢出 */
    IMEDIA_RET_BUF_ADDR_NOT_ALIGN,      /**< 缓存地址未对齐 */
    IMEDIA_RET_BUF_SIZE_NOT_ALIGN,      /**< 缓存大小未对齐 */
    IMEDIA_RET_BUF_SIZE_NOT_ENOUGH,     /**< 缓存大小不够 */

    IMEDIA_RET_BUF_OTHER = 0x04FF,      /**< 参数类其它问题 */

    /** 文件操作类问题返回 */
    IMEDIA_RET_FILE_OPEN_FAIL = 0x0800, /**< 打开文件失败 */
    IMEDIA_RET_FILE_READ_FAIL,          /**< 文件读取失败 */
    IMEDIA_RET_FILE_WRITE_FAIL,         /**< 文件写入失败 */
    IMEDIA_RET_FILE_CLOSE_FAIL,         /**< 关闭文件失败 */

    IMEDIA_RET_FILE_OTHER = 0x08FF,     /**< 文件操作类其他问题 */

    /** 控制命令类问题返回 */
    IMEDIA_RET_CMD_INVALID = 0x1000,

    /** 问题返回 */
    IMEDIA_RET_DECODE_INTERRUPT = 0x2000,
    IMEDIA_RET_ENCODE_INTERRUPT,

    /** 其他问题返回 */
    IMEDIA_RET_OTHER = 0x8000,
} ENUM_IMEDIA_RETURN;

/** 算法错误号枚举类型定义 */
/**
typedef struct STRU_IMEDIA_ERROR_CODE
{
    unsigned char CodecID;
    unsigned char SEQHeader : 4;
    unsigned char PicHeader : 4;
    unsigned char SliceHeader : 4;
    unsigned char MBHeader : 4;
    unsigned char Residual : 4;
    unsigned char Reserved : 4;
} STRU_IMEDIA_ERROR_CODE;*/

#define IMEDIA_ERROR_MASK_CODEC_ID 0xFF000000
#define IMEDIA_ERROR_MASK_SEQ      0x00F00000
#define IMEDIA_ERROR_MASK_PIC      0x000F0000
#define IMEDIA_ERROR_MASK_SLICE    0x0000F000
#define IMEDIA_ERROR_MASK_MB       0x00000F00
#define IMEDIA_ERROR_MASK_RESIDUAL 0x000000F0
#define IMEDIA_ERROR_MASK_RESERVED 0x0000000F

#define IMEDIA_ERROR_BITS_CODEC_ID 24
#define IMEDIA_ERROR_BITS_SEQ      20
#define IMEDIA_ERROR_BITS_PIC      16
#define IMEDIA_ERROR_BITS_SLICE    12
#define IMEDIA_ERROR_BITS_MB       8
#define IMEDIA_ERROR_BITS_RESIDUAL 4
#define IMEDIA_ERROR_BITS_RESERVED 0

#define IMEDIA_SET_ERR_CODEC_ID(c, e) ((c) |= ((e)<<IMEDIA_ERROR_BITS_CODEC_ID))
#define IMEDIA_SET_ERR_SEQ(c, e)      ((c) |= ((e)<<IMEDIA_ERROR_BITS_SEQ))
#define IMEDIA_SET_ERR_PIC(c, e)      ((c) |= ((e)<<IMEDIA_ERROR_BITS_PIC))
#define IMEDIA_SET_ERR_SLICE(c, e)    ((c) |= ((e)<<IMEDIA_ERROR_BITS_SLICE))
#define IMEDIA_SET_ERR_MB(c, e)       ((c) |= ((e)<<IMEDIA_ERROR_BITS_MB))
#define IMEDIA_SET_ERR_RESIDUAL(c, e) ((c) |= ((e)<<IMEDIA_ERROR_BITS_RESIDUAL))

#define IMEDIA_GET_ERR_CODEC_ID(e)    (((e) & IMEDIA_ERROR_MASK_CODEC_ID) >> IMEDIA_ERROR_BITS_CODEC_ID)
#define IMEDIA_GET_ERR_SEQ(e)         (((e) & IMEDIA_ERROR_MASK_SEQ) >> IMEDIA_ERROR_BITS_SEQ)
#define IMEDIA_GET_ERR_PIC(e)         (((e) & IMEDIA_ERROR_MASK_PIC) >> IMEDIA_ERROR_BITS_PIC)
#define IMEDIA_GET_ERR_SLICE(e)       (((e) & IMEDIA_ERROR_MASK_SLICE) >> IMEDIA_ERROR_BITS_SLICE)
#define IMEDIA_GET_ERR_MB(e)          (((e) & IMEDIA_ERROR_MASK_MB) >> IMEDIA_ERROR_BITS_MB)
#define IMEDIA_GET_ERR_RESIDUAL(e)    (((e) & IMEDIA_ERROR_MASK_RESIDUAL) >> IMEDIA_ERROR_BITS_RESIDUAL)
#define IMEDIA_GET_ERR_RESERVED(e)     ((e) & IMEDIA_ERROR_MASK_RESERVED)

/**序列头错误号枚举类型*/
typedef enum ENUM_IMEDIA_ERROR_SEQ
{
    IMEDIA_ERR_SEQ_NO_ERROR = 0,
    IMEDIA_ERR_SEQ_PROFILE_LEVEL,          /**< 无错误 */
    IMEDIA_ERR_SEQ_BIT_DEPTH,              /**< luma/chroma bit_depth类错误 */
    IMEDIA_ERR_SEQ_FRAME_NUM,              /**< 图像帧数类错误，如解码帧数，参考帧数等 */
    IMEDIA_ERR_SEQ_SIZE,                   /**< 图像尺寸类错误 */
    IMEDIA_ERR_SEQ_TIME_FRAMERATE,         /**< 图像时间戳，帧率类错误 */
    IMEDIA_ERR_SEQ_COLOR_SPACE,            /**< 色度空间错误 */
    IMEDIA_ERR_SEQ_ID,                     /**< 码流中id类错误 */
    IMEDIA_ERR_SEQ_QUANT,                  /**< 量化矩阵，量化精度类错误 */
    IMEDIA_ERR_SEQ_MARKER,                 /**< 功能开关，比特置位类错误 */
    IMEDIA_ERR_SEQ_SHAPE,                  /**< 图像形状类错误 */

    IMEDIA_ERR_SEQ_OTHER = 0x0F,           /**< 其它问题 */
} ENUM_IMEDIA_ERROR_SEQ;

/**帧头错误号枚举类型*/
typedef enum ENUM_IMEDIA_ERROR_PIC
{
    IMEDIA_ERR_PIC_NO_ERROR = 0,           /**< 无错误 */
    IMEDIA_ERR_PIC_ID,                     /**< 图像id类错误 */
    IMEDIA_ERR_PIC_SLICE_GROUP_COUNT,      /**< pps 所指向slice group个数不合法 */
    IMEDIA_ERR_PIC_REF,                    /**< 参考帧类错误 */
    IMEDIA_ERR_PIC_FRAME_TYPE,             /**< 图像类型类错误 */
    IMEDIA_ERR_PIC_MARKER,                 /**< 图像各比特标志位类错误 */
    IMEDIA_ERR_PIC_QP,                     /**< 图像QP类错误 */
    IMEDIA_ERR_PIC_FCODE,                  /**< 图像F_CODE类错误 */

    IMEDIA_ERR_PIC_OTHER = 0x0F,           /**< 其它问题 */
} ENUM_IMEDIA_ERROR_PIC;

/**slice头错误号枚举类型*/
typedef enum ENUM_IMEDIA_ERROR_SLICE
{
    IMEDIA_ERR_SLICE_NO_ERROR = 0,
    IMEDIA_ERR_SLICE_TYPE,                /**< slice type是否合法 */
    IMEDIA_ERR_SLICE_ID,                  /**< slice id类错误 */
    IMEDIA_ERR_SLICE_ADDR,                /**< slice地址类错误 */
    IMEDIA_ERR_SLICE_REF,                 /**< slice中参考帧类问题 */
    IMEDIA_ERR_SLICE_VLC,                 /**< slice熵编码类问题 */
    IMEDIA_ERR_SLICE_WEIGHTED,            /**< 加权相关参数类错误 */
    IMEDIA_ERR_SLICE_QP,                  /**< slice qp类错误 */
    IMEDIA_ERR_SLICE_DEBLOCKING,          /**< deblocking类错误 */
    IMEDIA_ERR_SLICE_NUM,                 /**< slice中slice或mb数目类错误 */
    IMEDIA_ERR_SLICE_BITS,                /**< slice中所耗费比特数错误 */
    IMEDIA_ERR_SLICE_MARKER,              /**< 标记类错误 */

    IMEDIA_ERR_SLICE_OTHER = 0x0F,        /**< 其它问题 */
} ENUM_IMEDIA_ERROR_SLICE;

/**宏块头错误号枚举类型*/
typedef enum ENUM_IMEDIA_ERROR_MB
{
    IMEDIA_ERR_MB_NO_ERROR = 0,
    IMEDIA_ERR_MB_SKIP_MB_RUN,         /**< skip_mb_run不合法 */
    IMEDIA_ERR_MB_TYPE,                /**< mb type类错误 */
    IMEDIA_ERR_MB_MODE,                /**< 预测模式类错误 */
    IMEDIA_ERR_MB_REF,                 /**< 参考帧类错误 */
    IMEDIA_ERR_MB_CBP,                 /**< cbp类错误 */
    IMEDIA_ERR_MB_QP,                  /**< qp类错误 */
    IMEDIA_ERR_MB_MV,                  /**< mv类错误 */
    IMEDIA_ERR_MB_MARKER,              /**< 标记类错误 */

    IMEDIA_ERR_MB_OTHER = 0x0F,        /**< 宏块中其它错误 */
} ENUM_IMEDIA_ERROR_MB;

/**残差数据错误号枚举类型*/
typedef enum ENUM_IMEDIA_ERROR_RESIDUAL
{
    IMEDIA_ERR_RESIDUAL_NO_ERROR = 0,
    IMEDIA_ERR_RESIDUAL_DC,            /**< DC错误 */
    IMEDIA_ERR_RESIDUAL_AC,            /**< AC错误 */
    IMEDIA_ERR_RESIDUAL_TOTAL_COEFF,   /**< cavlc解码中total coeff参数错误 */
    IMEDIA_ERR_RESIDUAL_ZERO_LEFT,     /**< cavlc解码中zero left参数错误 */
    IMEDIA_ERR_RESIDUAL_LEVEL,         /**< 熵解码中level类错误 */

    IMEDIA_ERR_RESIDUAL_OTHER = 0x0F,  /**< 其它问题 */
} ENUM_IMEDIA_ERROR_RESIDUAL;

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#endif /**< __IMEDIA_ERROR_H__ */

/************************************************************************
* $Log$
************************************************************************/

