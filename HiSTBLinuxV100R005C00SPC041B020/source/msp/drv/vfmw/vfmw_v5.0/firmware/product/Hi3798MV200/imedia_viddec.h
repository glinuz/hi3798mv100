#ifndef __IMEDIA_VIDDEC_H__
#define __IMEDIA_VIDDEC_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#include "imedia_common.h"
#include "imedia_error.h"

#pragma pack(push, 1)
/*****************************************************************************************
 * 视频解码模块常用数据类型
 *****************************************************************************************/
#define MAX_DELAY_FRAME     6
#define MAX_DISPLAY_FRAME   MAX_DELAY_FRAME
#define MAX_META_DATA_COUNT 4

/*========================================================================================
* 视频解码模块Debug标志
=======================================================================================*/
#define DEBUG_NOTHING     0x00
#define DEBUG_PIC_INFO    0x01
#define DEBUG_MB_INFO     0x02
#define DEBUG_DCT_COF     0x04
#define DEBUG_START_CODE  0x08
#define DEBUG_ERROR       0x10
#define DEBUG_REF_MANAGE  0x20
#define DEBUG_BUF_MANAGE  0x40

/*!视频解码模块元数据解析状态 */
typedef enum ENUM_IVIDDEC_META_DATA_STATUS
{
    META_DATA_NOT_FOUND = 0,      /**< 尚未找到元数据 */
    META_DATA_FOUND,              /**< 已找到元数据 */
    META_DATA_FOUND_ONLY_SPS,     /**< 仅找到H264(SPS), MPEG4(VOS) */
    META_DATA_FOUND_ONLY_PPS,     /**< 仅找到H264(PPS), MPEG4(VOL) */
} ENUM_IVIDDEC_META_DATA_STATUS;

/*!视频解码模块静态参数 */
/**< 可通过ProbeHeader接口查询码流的相关值，或按最大值设置 */
typedef struct STRU_IVIDDEC_PARAMS
{
    UINT8 *pucBuf;

    UINT16 usMaxWidth;        /**< 最大支持的宽度，Range[32-1920] */
    UINT16 usMaxHeight;       /**< 最大支持的高度，Range[16-1088] */
    UINT16 usMaxRefFrame;     /**< 最大参考帧个数，Range[0-16]，一期无限制作用 */
    UINT16 usMaxFrameBufNum;  /**< YUV的最大缓存帧数，Range[3-32] */

    ENUM_IVIDEO_CODEC_PROFILE    eProfile;        /**< profile idc，保留字段，一期没实现 */
    ENUM_IVIDEO_CODEC_LEVEL      eLevel;          /**< level idc，保留字段，一期没实现 */
    ENUM_IVIDEO_COLOR_SPACE_TYPE eColorSpaceType; /**< 码流图像的格式 (YUV420, YUV422, etc)，目前除了MPEG2支持YUV422之外，其它codec都只支持YUV420 */

    /**< 下列参数需要手动设置 */
    INT32  iFlags;             /**< 算法通道常用标记组合，建议值：0，详情见imedia_common.h中模块公共常用标记
                                    第1比特： IMEDIA_FLAG_DISABLE_ASM      汇编优化关闭标记
                                    第2比特： IMEDIA_FLAG_CALC_FPS         通道性能统计使能标记
                                    第9比特： IMEDIA_FLAG_DEBLOCK_FILTER   后置滤波标记(保留)
                                    第10比特：IMEDIA_FLAG_ERROR_RESILIENCE 错误隐藏标记(保留)
                                    其它比特该模块暂时都未使用 */

    BOOL   bForceOutYUV420Flag; /**< 解码器本身可输出YUV420、YUV422，但目前编码器只支持YUV420[将其值赋值为1] */

    /**< 当前通道内存的起始地址及大小 */
    INT32 iBufLength;

} STRU_IVIDDEC_PARAMS;

/*!视频解码模块码流元数据结构（序列头、图像头等等） */
typedef struct STRU_IVIDDEC_META_DATA
{
    UINT8 *apucMetaData[MAX_META_DATA_COUNT];    /**< 元数据指针数组 */
    UINT16 ausMetaDataLen[MAX_META_DATA_COUNT];  /**< 元数据长度，Range[0-1024] */
    INT32 iNumMetaData;                          /**< 元数据个数，Range[0-4] */
} STRU_IVIDDEC_META_DATA;

/*!视频解码模块码流元数据解析输出参数 */
typedef struct STRU_IVIDDEC_META_DATA_OUT_ARGS
{
    UINT32 uiBytesConsumed;                 /**< 已解析码流的字节数 */
    ENUM_IVIDDEC_META_DATA_STATUS eStatus;  /**< 元数据解析状态，应用层初始化为META_DATA_NOT_FOUND，之后调用probeheader时不能再进行修改 */
    STRU_IVIDDEC_META_DATA stMetaData;      /**< 元数据地址，需由应用层分配空间保存下来传给process接口使用 */
    STRU_IVIDEO_STREAM_INFO stStreamInfo;   /**< 解析元数据后获取的信息 */
} STRU_IVIDDEC_META_DATA_OUT_ARGS;

/*!视频解码模块输入参数 */
typedef struct STRU_IVIDDEC_IN_ARGS
{
    UINT8 *pucBuf;                         /**< 码流的起始地址 */

    UINT32 uiNumBytes;                     /**< 码流的字节数,最大为64M字节 */
    ENUM_BITSTREAM_STATUS eBitstreamStatus;/**< 码流状态，用以标识码流的位置信息 */
    INT64  iPTS;                           /**< 码流对应的时间戳 */

    STRU_IVIDDEC_META_DATA *pstMetaData;   /**< ProbeHeader获取并由应用层保存下来的元数据地址 */
} STRU_IVIDDEC_IN_ARGS;

/*!视频解码模块输出参数 */
typedef struct STRU_IVIDDEC_OUT_ARGS
{
    BOOL   bLastFrameFlag;             /**< 是否为最后一帧输出 */
    INT32  iErrorCode;                 /**< 算法内部的错误码 */
    UINT32 uiBytesConsumed;            /**< 已解析码流的字节数 ,若=0，*/
    UINT32 uiDisplayID;                /**< 显示顺序编号，从当前分片起始位置开始计算，可用于计算时间 */
    INT64  iPTS;                       /**< 当前帧对应的时间戳 */
    ENUM_IVIDEO_FRAME_TYPE eFrameType; /**< 源码码流中当前帧的编码类型 */
    STRU_IVIDEO_PICTURE stPicture;     /**< 输出图像的相关信息（模块内部分配空间，应用层无需释放） */
} STRU_IVIDDEC_OUT_ARGS;

/*!视频解码模块码流参数 */
typedef struct STRU_IVIDDEC_STREAM
{
    UINT8 *pucBuf;              /**< 码流的起始地址 */
    UINT32 uiNumBytes;          /**< 码流的字节数,最大为64M字节 */
    INT64  iPTS;                /**< 码流对应的时间戳 */

} STRU_IVIDDEC_STREAM;

/*!视频解码模块帧码流输出参数 */
typedef struct STRU_IVIDDEC_FRAME_PARSE_OUT_ARGS
{
    INT32  iErrorCode;          /**< 算法内部的错误码 */
    UINT32 uiBytesConsumed;     /**< 已解析码流的字节数 */
    STRU_IVIDDEC_STREAM stFrame;/**< 帧码流缓存信息 */

} STRU_IVIDDEC_FRAME_PARSE_OUT_ARGS;


/*!解码通道状态信息 */
typedef struct STRU_IVIDDEC_STATUS
{
    ENUM_IVIDEO_CODEC_STATUS eCodecStatus; /**< 算法通道状态 */
    INT32  iErrorCode;        /**< 错误码 */
    UINT32 uiTotalErrors;     /**< 已发生码流异常的总次数 */
    UINT32 uiDecIFrames;      /**< 已解码的I帧数量 */
    UINT32 uiDecPFrames;      /**< 已解码的P帧数量 */
    UINT32 uiDecBFrames;      /**< 已解码的B帧数量 */
    UINT32 uiDisplayedFrames; /**< 已显示帧的数量 */
    UINT32 uiInUsedBufNum;    /**< 已使用的YUV BUF数量 */
    UINT32 uiFreeBufNum;      /**< 空闲的YUV BUF数量 */
    UINT32 uiAvgFps;          /**< 平均性能FPS * 1000，在开启性能计算标记后才有效 */
} STRU_IVIDDEC_STATUS;

/*!解码通道所需内存信息 */
typedef struct STRU_IVIDDEC_MEMORY_INFO
{
    UINT16  usWidth;                                /**< 分配空间所用到的图像的宽度[32-1920] */
    UINT16  usHeight;                               /**< 分配空间所用到的图像的宽度[16-1088] */
    UINT16  usBufNum;                               /**< 需分配的图像个数 */
    ENUM_IVIDEO_COLOR_SPACE_TYPE eColorFormat;		/**< 图像的色度空间：422、420等 */
} STRU_IVIDDEC_MEMORY_INFO;

/*****************************************************************************************
 * 视频解码模块接口
 *****************************************************************************************/

/*****************************************************************************************
* 函数说明：从码流中解析元数据类型，同一文件只需在开始创建算法通道前调用即可。
* 输入参数：
*     eCodecType 算法类型，具体见ENUM_IVIDEO_CODEC_TYPE定义
*     pstInArgs  输入参数信息
* 输出参数：
*     pstOutArgs 输出参数信息
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_ProbeHeader(ENUM_IVIDEO_CODEC_TYPE eCodecType, STRU_IMEDIA_SINGLE_BUFFER *pstInArgs, STRU_IVIDDEC_META_DATA_OUT_ARGS *pstOutArgs);

/*****************************************************************************************
* 函数说明：寻问解码算法所需的内存大小。
* 输入参数：
*     eCodecType 算法类型，具体见ENUM_IVIDEO_CODEC_TYPE定义
*     pstMemInfo     图像的最大宽度、 图像的最高高度、最大图像个数以及色度空间等信息
* 输出参数：
*      piSize       解码器所需的总的内存大小
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_QueryMemSize(ENUM_IVIDEO_CODEC_TYPE eCodecType, STRU_IVIDDEC_MEMORY_INFO *pstMemInfo , INT32 *piSize);

/*****************************************************************************************
* 函数说明：创建算法通道。
* 输入参数：
*     eCodecType 算法类型，具体见ENUM_IVIDEO_CODEC_TYPE定义
*     params     指向存储图像的一些基本信息的结构体（包括图像的宽高等）
* 输出参数：
*      pctx       算法通道句柄指针
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_Create(ENUM_IVIDEO_CODEC_TYPE eCodecType, STRU_IVIDDEC_PARAMS *pstParams, IMEDIA_CODEC_CTX *pctx);

/*****************************************************************************************
* 函数说明：算法通道控制函数。
* 输入参数：
*     ctx       算法通道句柄
*     cmd:      控制参数的命令码
*         IMEDIA_GET_VERSION     获取版本信息                  wparam(STRU_IMEDIA_VERSION*)         lparam(NULL)
*         IMEDIA_GET_STATUS      获取算法通道状态信息          wparam(STRU_IVIDDEC_STATUS*)         lparam(NULL)
*         IMEDIA_GET_STREAM_INFO 获取算法通道当前码流信息      wparam(STRU_IVIDEO_STREAM_INFO*)     lparam(NULL)
*         IMEDIA_GET_PARAMS      获取算法通道的参数信息        wparam(STRU_IVIDDEC_PARAMS*)         lparam(NULL)
*         IMEDIA_SET_DEBUG_FLAG  设置调试内容标记              wparam(int*)                         lparam(NULL)
*         IMEDIA_FLUSH           清空算法通道的cache信息       wparam(NULL)                         lparam(NULL)
*         IMEDIA_RESET           复位算法通道                  wparam(NULL)                         lparam(NULL)
*         IMEDIA_PICTURE_RELEASE 释放解码模块输出的YUV帧控制权 wparam(STRU_IVIDEO_PICTURE*)         lparam(NULL)
*     wparam    每种命令对应各自类型，可作为输入或输出
*     lparam    每种命令对应各自类型，可作为输入或输出
* 输出参数：
*     wparam    每种命令对应各自类型，可作为输入或输出
*     lparam    每种命令对应各自类型，可作为输入或输出
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_Control(IMEDIA_CODEC_CTX ctx, ENUM_IMEDIA_CMD cmd, void *wparam, void *lparam);


/*****************************************************************************************
* 函数说明：码流帧解析接口，从输入码流中解析出一帧数据，注意输出的帧缓存有可能是输入码流地址，也有可能是内部缓存地址（当输入码流不足一帧时内部会拼接）。
*           该接口在海思ARM软解码项目中仅提供给应用层做调试
* 输入参数：
*     ctx        算法通道句柄
*     pstInArgs  输入参数信息（输入码流的地址、长度及时间戳等）
* 输出参数：
*     pstOutArgs 输出参数信息（帧缓存的地址和长度）
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_FrameParse(IMEDIA_CODEC_CTX ctx, STRU_IVIDDEC_STREAM *pstInArgs, STRU_IVIDDEC_FRAME_PARSE_OUT_ARGS *pstOutArgs);


/*****************************************************************************************
* 函数说明：码流帧解码接口，每次只解码一帧，延迟输出的帧需重复调用该接口（输入码流地址赋值为NULL），直到输出参数中指示为最后一帧时。
* 输入参数：
*     ctx        算法通道句柄
*     pstInArgs  输入参数信息（输入码流帧的地址、长度及时间戳等）
* 输出参数：
*     pstOutArgs 输出参数信息（解码输出图像的相关信息）
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_FrameDecode(IMEDIA_CODEC_CTX ctx, STRU_IVIDDEC_STREAM *pstInArgs, STRU_IVIDDEC_OUT_ARGS *pstOutArgs);

/*****************************************************************************************
* 函数说明：码流处理接口，包括帧解析和帧解码功能，每次调用最多只输出一帧，通过输出参数判断是否为最后一帧。
*           该接口在海思ARM软解码项目中仅提供给应用层做调试
* 输入参数：
*     ctx        算法通道句柄
*     pstInArgs  输入参数信息（输入码流的地址、长度及时间戳等）
* 输出参数：
*     pstOutArgs 输出参数信息（解码输出图像的相关信息）
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_Process(IMEDIA_CODEC_CTX ctx, STRU_IVIDDEC_IN_ARGS *pstInArgs, STRU_IVIDDEC_OUT_ARGS *pstOutArgs);

/*****************************************************************************************
* 函数说明：删除算法通道。
* 输入参数：
*     ctx       算法通道句柄
* 输出参数：无
* 返 回 值：0-成功，其他值见返回值定义。
*****************************************************************************************/
EXPORT_API INT32 IMedia_Viddec_Delete(IMEDIA_CODEC_CTX ctx);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#endif /**< __IMEDIA_VIDDEC_H__ */

/************************************************************************
* $Log$
************************************************************************/
