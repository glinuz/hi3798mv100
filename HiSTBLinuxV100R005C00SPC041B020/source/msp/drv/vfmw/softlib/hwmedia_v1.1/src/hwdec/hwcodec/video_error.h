
/*******************************************************************************
Copyright (C), 2009-2012, Huawei Tech. Co., Ltd.
File name: VIDEO_error.h
Author & ID: 宋小刚+00133955
Version: 1.00
Date:  2010-01-18
Description: 本文件包括了返回值及错误码定义
Function List:
History:
1、20100118 编码完成 宋小刚+00133955
2、**
3、××
*******************************************************************************/
#ifndef __VIDEO_ERROR_H__
#define __VIDEO_ERROR_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

/** 模块返回值枚举类型定义 */
typedef enum ENUM_VIDEO_RETURN
{
    VIDEO_RET_SUCCESS = 0,             /**< 正常返回 */

    /** 参数类问题返回 */
    VIDEO_RET_PARAM_NULL = 0x0100,     /**< 参数指针为空 */
    VIDEO_RET_PARAM_INVALID,           /**< 参数无效 */
    VIDEO_RET_PARAM_IN_VALUE_NULL,     /**< 输入参数值指针为空 */
    VIDEO_RET_PARAM_IN_VALUE_INVALID,  /**< 输入参数值无效 */
    VIDEO_RET_PARAM_OUT_VALUE_NULL,    /**< 输出参数值指针为空 */
    VIDEO_RET_PARAM_OUT_VALUE_INVALID, /**< 输出参数值无效 */
    VIDEO_RET_PARAM_NOT_ENOUGH,        /**< 参数不足 */

    VIDEO_RET_PARAM_OTHER = 0x01FF,    /**< 参数类其他问题 */

    /** 对象类问题返回 */
    VIDEO_RET_OBJ_NULL = 0x0200,       /**< 对象指针为空 */
    VIDEO_RET_OBJ_BAD_MASK,            /**< 对象防错码毁坏 */
    VIDEO_RET_OBJ_INVALID,             /**< 无效对象 */
    VIDEO_RET_OBJ_NOT_FOUND,           /**< 对象没找到 */
    VIDEO_RET_OBJ_CREATE_FAIL,         /**< 对象创建失败 */
    VIDEO_RET_OBJ_NOT_INIT,            /**< 对象未初始化 */
    VIDEO_RET_OBJ_DELETE_FAIL,         /**< 对象删除失败 */

    VIDEO_RET_OBJ_OTHER = 0x02FF,      /**< 对象类其他问题 */

    /** 缓存（码流）类问题返回 */
    VIDEO_RET_MEM_MALLOC_FAIL = 0x0400,/**< 内存分配失败 */
    VIDEO_RET_MEM_FREE_FAIL,           /**< 内存释放失败 */
    VIDEO_RET_BUF_NULL,                /**< 缓存指针为空 */
    VIDEO_RET_BUF_BAD_MASK,            /**< 缓存防错码毁坏 */
    VIDEO_RET_BUF_INVALID,             /**< 码流无效，无法识别格式 */
    VIDEO_RET_BUF_OVER_FLOW,           /**< 缓存溢出 */
    VIDEO_RET_BUF_ADDR_NOT_ALIGN,      /**< 缓存地址未对齐 */
    VIDEO_RET_BUF_SIZE_NOT_ALIGN,      /**< 缓存大小未对齐 */
    VIDEO_RET_STREAM_ERROR,			/**< 码流解析错误 */

    VIDEO_RET_BUF_OTHER = 0x04FF,      /**< 参数类其他问题 */

    /** 控制命令类问题返回 */
    VIDEO_RET_CMD_INVALID = 0x1000,

    /** 其他问题返回 */
    VIDEO_RET_OTHER = 0x8000,
} ENUM_VIDEO_RETURN;

/** 算法错误号枚举类型定义 */
typedef enum ENUM_VIDEO_CODEC_ERROR_CODE
{
    VIDEO_CODEC_ERR_NOTHING = 0,    	/**< 正常返回 */

    /** 通用错误类型 */
    VIDEO_CODEC_ERR_UNSUPPORTED_PROFILE,
    VIDEO_CODEC_ERR_UNSUPPORTED_LEVEL,
    VIDEO_CODEC_ERR_UNSUPPORT_COLOR_SPACE,
    VIDEO_CODEC_ERR_INVALID_WIDTH,
    VIDEO_CODEC_ERR_INVALID_HEIGHT,
    VIDEO_CODEC_ERR_INVALID_WIDTH_PITCH,
    VIDEO_CODEC_ERR_INVALID_HEIGHT_PITCH,
    VIDEO_CODEC_ERR_INVALID_FRAMERATE,
    VIDEO_CODEC_ERR_INVALID_BITRATE,

    /** H264错误类型 */
    VIDEO_CODEC_ERR_INVALID_SPS = 0x0100,
    VIDEO_CODEC_ERR_INVALID_PPS,
    //...

    /** MPEG2错误类型 */

    /** H263错误类型 */

    /** MPEG4错误类型 */

    /** WMV/VC1错误类型 */

    /** VP6错误类型 */

} ENUM_VIDEO_CODEC_ERROR_CODE;

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#endif /**< __VIDEO_ERROR_H__ */


