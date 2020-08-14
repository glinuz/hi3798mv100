//==============================================================================
//           Copyright (C), 2008-2014, Huawei Tech. Co., Ltd.
//==============================================================================
//  文件名称: imedia_avc_api.h
//  文件描述: 该文件包含AVC/DRC算法库的各结构体定义信息和功能函数的声明信息
//            在使用算法库时需要包含此文件
//
//  结构体列表:
//      iMedia_AVC_STRU_MEM_CONFIG  -- 算法内存配置结构体
//      iMedia_AVC_STRU_MEM_SIZE    -- 算法内存大小结构体
//      iMedia_AVC_STRU_VERSION     -- 算法版本信息结构体
//      iMedia_AVC_STRU_PARAMS      -- AVC算法参数配置结构体
//      iMedia_DRC_STRU_PARAMS      -- DRC算法参数配置结构体
//      iMedia_AVC_DRC_STRU_PARAMS  -- 算法总接口参数配置结构体
//
//  函数列表:
//      iMedia_AVC_GetSize()        -- 获取内存大小
//      iMedia_AVC_Init()           -- 初始化算法实例(通道变量)
//      iMedia_AVC_Apply()          -- 算法处理应用
//      iMedia_AVC_SetParams()      -- 设置算法参数配置
//      iMedia_AVC_GetParams()      -- 获取算法参数配置
//      iMedia_AVC_GetVersion()     -- 获取算法库版本信息，包括所用编译器版本、
//                                     发布时间和版本号
//
//  修改记录: 见文件尾
//==============================================================================

#ifndef _IMEDIA_AVC_API_H_
#define _IMEDIA_AVC_API_H_

#include "imedia_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
//     基本常量定义信息
//==============================================================================
#define    IMEDIA_AVC_CHAN_MSG_VERLEN   (64)    // 算法库版本信息及发布时间字符串长度
#define    IMEDIA_AVC_SAMPLE_RATE       (48000) // 算法处理的采样率
#define    IMEDIA_AVC_CHAN_NUM          (2)     // 算法处理数据的声道数
#define    IMEDIA_AVC_FRAME_PCMNUM_S    (256)   // 算法处理一帧PCM数据单声道点数

//==============================================================================
//     返回码定义信息
//==============================================================================
// BEGIN: modified by x00227689 20141120 问题单: DTS2014112004156
#define    IMEDIA_AVC_EOK                      (0)     // 操作成功返回码
#define    IMEDIA_AVC_INV_MEMSIZE_PST          (-1)    // 无效的内存大小结构体指针
#define    IMEDIA_AVC_INV_MEMCFG_PST           (-2)    // 无效的内存配置结构体指针
#define    IMEDIA_AVC_INV_SAMPLE_RATE          (-3)    // 无效的采样率
#define    IMEDIA_AVC_INV_CHANNEL              (-4)    // 无效的声道数
#define    IMEDIA_AVC_INV_HANDLE               (-5)    // 无效句柄（指针为空）
#define    IMEDIA_AVC_INV_BASE                 (-6)    // 无效的基地址
//#define    IMEDIA_AVC_INV_MEM_ALIGN            (-7)    // 内存首地址没有8字节对齐
#define    IMEDIA_AVC_INV_MEM_SIZE             (-7)    // 内存大小小于要求值
#define    IMEDIA_AVC_INV_PARAMS               (-8)    // 无效的配置结构体指针
#define    IMEDIA_AVC_INV_PARA_MODE            (-9)    // 无效算法配置模式
#define    IMEDIA_AVC_INV_PARA_ATTACK          (-10)   // 无效AttackTime
#define    IMEDIA_AVC_INV_PARA_RELEASE         (-11)   // 无效ReleaseTime
#define    IMEDIA_AVC_INV_PARA_START_LEVEL     (-12)   // 无效的起调电平
#define    IMEDIA_AVC_INV_PARA_CORNER_LEVEL    (-13)   // 无效的转折电平
#define    IMEDIA_AVC_INV_PARA_MAX_GAIN        (-14)   // 无效的最大增益
#define    IMEDIA_AVC_INV_PARA_TARGET_LEVEL    (-15)   // 无效的目标电平
#define    IMEDIA_AVC_INV_PARA_CROSS_LEVEL     (-16)   // 转折电平小于等于起调电平
                                                       // 或转折电平加最大增益大于等于目标电平
                                                       // 或DRC模式下目标电平小于等于起调电平
#define    IMEDIA_AVC_INV_BIT_DEPTH            (-17)   // 无效的数据位宽配置
#define    IMEDIA_AVC_INV_INBUFF               (-18)   // 输入缓冲区为空
#define    IMEDIA_AVC_INV_MEM_ODD              (-19)   // 无效的奇地址
#define    IMEDIA_AVC_INV_OUTBUFF              (-20)   // 输出缓冲区为空
#define    IMEDIA_AVC_INV_OUTBUFF_UNALIGNED    (-21)   // 输出缓冲区地址没有4字节对齐
#define    IMEDIA_AVC_UNINITIED                (-22)   // 没有完成初始化
#define    IMEDIA_AVC_INV_PARA_VERSION         (-23)   // 无效版本信息结构体指针
// END: modified by x00227689 20141120 问题单: DTS2014112004156
//==============================================================================
//     配置参数定义信息
//==============================================================================
typedef enum
{
    IMEDIA_AVC_MODE = 0,
    IMEDIA_DRC_MODE = 1
}IMEDIA_AVC_DRC_MODE;

typedef enum
{
    IMEDIA_AVC_BIT_DEPTH_16 = 0,
    IMEDIA_AVC_BIT_DEPTH_24 = 1
}IMEDIA_AVC_DRC_BIT_DEPTH;

//==============================================================================
//     复合数据类型定义信息
//==============================================================================
// 内存配置结构体
typedef struct tagAVC_STRU_MEM_CONFIG
{
    IMEDIA_INT32  iSampleRate;                                    // 数据采样率 48000
    IMEDIA_INT32  iChannels;                                      // 声道数 2
    IMEDIA_INT8   cReserve[8];                                    // 保留区
}iMedia_AVC_STRU_MEM_CONFIG, *iMedia_AVC_PST_MEM_CONFIG;


// 内存大小结构体
typedef struct tagAVC_STRU_MEM_SIZE
{
    IMEDIA_UINT32   uiStrSize;                                    // 通道变量大小, 单位为字节
    IMEDIA_INT32    iInSize;                                      // 保留参数
    IMEDIA_INT32    iOutSize;                                     // 保留参数
}iMedia_AVC_STRU_MEM_SIZE, *iMedia_AVC_PST_MEM_SIZE;


// 版本信息结构体
typedef struct  tagAVC_STRU_VERSION
{
    IMEDIA_UINT8    ucCgtVersion[IMEDIA_AVC_CHAN_MSG_VERLEN];     // Code Generation Tools版本号
    IMEDIA_UINT8    ucReleaseVer[IMEDIA_AVC_CHAN_MSG_VERLEN];     // 算法库版本号
    IMEDIA_UINT8    ucReleaseTime[IMEDIA_AVC_CHAN_MSG_VERLEN];    // 发布日期
} iMedia_AVC_STRU_VERSION, *iMedia_AVC_PST_VERSION;


// AVC算法配置参数结构体
typedef struct tagAVC_STRU_PARAMS
{
    IMEDIA_INT16  sStartLevel;   // 起调电平，范围[-40, -20]dB
    IMEDIA_INT16  sCornerLevel;  // 达到最大抬升增益的转折电平，范围(-40, -16)dB，需要大于起调电平
    IMEDIA_INT16  sMaxGain;      // 最大抬升增益，范围[0, 8]dB
    IMEDIA_INT16  sTargetLevel;  // 目标电平，范围[-16, -6]dB，转折电平加最大增益需小于目标电平
}iMedia_AVC_STRU_PARAMS, *iMedia_AVC_PST_PARAMS;


// DRC算法配置参数结构体
typedef struct tagDRC_STRU_PARAMS
{
    IMEDIA_INT16  sStartLevel;   // 起调电平，[-80, -1)
    IMEDIA_INT16  sTargetLevel;  // 目标电平，(-80, -1]，起调电平需小于目标电平
}iMedia_DRC_STRU_PARAMS, *iMedia_DRC_PST_PARAMS;


// 算法配置参数结构体
typedef struct tagAVC_DRC_STRU_PARAMS
{
    IMEDIA_INT16  sMode;         // 算法选择参数: 0 表示选择AVC算法，1 表示选择DRC算法，其余配置无效
    IMEDIA_INT16  sAttackTime;   // attack时间，范围[20, 2000]ms
    IMEDIA_INT16  sReleaseTime;  // release时间，范围[20, 2000]ms
    iMedia_AVC_STRU_PARAMS  stAVCParams; // AVC算法配置参数结构体
    iMedia_DRC_STRU_PARAMS  stDRCParams; // DRC算法配置参数结构体
    IMEDIA_INT16  sBitDepth;     // 数据位宽，0表示16bit，1表示24bit，其余配置无效，暂仅支持DRC算法
}iMedia_AVC_DRC_STRU_PARAMS, *iMedia_AVC_DRC_PST_PARAMS;


//==============================================================================
//     函数声明信息
//==============================================================================

//==============================================================================
// 函数名称: iMedia_AVC_GetSize
// 功能描述: 获取通道大小
// 输入参数:
//     pstConf    -- 内存配置结构体
// 输出参数:
//     pstMemSize -- 通道大小结构体
// 返回参数:
//     ret        -- 操作返回码IMEDIA_AVC_EOK表示成功，其他返回码表示失败
//==============================================================================
extern IMEDIA_INT32 iMedia_AVC_GetSize(iMedia_AVC_PST_MEM_SIZE pstMemSize,
                                       iMedia_AVC_PST_MEM_CONFIG pstConf);

//==============================================================================
// 函数名称: iMedia_AVC_Init
// 功能描述: 初始化算法实例(通道变量)，并返回其句柄
// 输入参数:
//     pMemBase   -- 通道变量起始地址
//     uiMemSize  -- 通道变量空间大小
//     pParams    -- 算法参数配置结构体指针
// 输出参数:
//     ppHandle   -- 对象句柄
// 返回参数:
//     ret        -- 操作返回码 IMEDIA_AVC_EOK 表示成功，其他返回码表示失败
//==============================================================================
extern IMEDIA_INT32 iMedia_AVC_Init(IMEDIA_VOID **ppHandle,
                                    const IMEDIA_VOID *pMemBase,
                                    IMEDIA_UINT32 uiMemSize,
                                    const iMedia_AVC_DRC_PST_PARAMS pParams);

//==============================================================================
// 函数名称: iMedia_AVC_Apply
// 功能描述: 算法处理应用(处理一帧双声道512点，约5.333ms)
// 输入参数:
//     pHandle    -- 对象句柄
//     PcmIn     -- 输入数据缓冲区指针
// 输出参数:
//     PcmOut    -- 输出数据缓冲区指针
// 返回参数:
//     ret        -- 操作返回码 IMEDIA_AVC_EOK 表示成功，其他返回码表示失败
//==============================================================================
extern IMEDIA_INT32 iMedia_AVC_Apply(IMEDIA_VOID *pHandle,
                                     const IMEDIA_VOID *PcmIn,
                                     IMEDIA_VOID *PcmOut);

//==============================================================================
// 函数名称: iMedia_AVC_SetParams
// 功能描述: 动态参数配置接口，切换参数配置时使用。
//           注意: 该接口仅是调试接口，只能在调试阶段使用，不建议现网使用。
// 输入参数:
//     pHandle    -- 对象句柄
//     pParams    -- 算法参数配置结构体指针
// 输出参数:
//     pHandle    -- 对象句柄
// 返回参数:
//     ret        -- 操作返回码 IMEDIA_AVC_EOK 表示成功，其他返回码表示失败
//==============================================================================
extern IMEDIA_INT32 iMedia_AVC_SetParams(IMEDIA_VOID* pHandle,
                                         const iMedia_AVC_DRC_PST_PARAMS pParams);

//==============================================================================
// 函数名称: iMedia_AVC_GetParams
// 功能描述: 获取当前算法的参数配置
// 输入参数:
//     pHandle    -- 对象句柄
// 输出参数:
//     pParams    -- 算法参数配置结构体指针
// 返回参数:
//     ret        -- 操作返回码 IMEDIA_AVC_EOK 表示成功，其他返回码表示失败
//==============================================================================
extern IMEDIA_INT32 iMedia_AVC_GetParams(IMEDIA_VOID *pHandle,
                                         iMedia_AVC_DRC_PST_PARAMS pParams);

//==============================================================================
// 函数名称: iMedia_AVC_GetVersion
// 功能描述: 获取算法库版本信息，包括所用编译器版本、发布时间和版本号
// 输入参数:
//     ppVersion    -- 版本信息结构体句柄
// 输出参数:
//     ppVersion    -- 版本信息结构体句柄
// 返回参数:
//     ret          -- 操作返回码 IMEDIA_AVC_EOK 表示成功，其他返回码表示失败
//==============================================================================
extern IMEDIA_INT32 iMedia_AVC_GetVersion(iMedia_AVC_PST_VERSION *ppVersion);

#ifdef __cplusplus
}
#endif

#endif


//==============================================================================
// 修改记录:
//  2013-6-4 14:02:23    建立文件
//                       创建人：杜艳斌
//  2013-7-2 15:32:13    修改文件，HiFi2数据结构修改，增加滤波器精度flag
//                       修改人：黄涛/h00207014
//  2013-8-12 15:32:13   修改文件，修改配置参数结构体中声道数的注释
//                       修改人：黄涛/h00207014
//  2013-11-05 14:25:13  修改拖音问题,增大通道变量,配置参数中增加变量iRmsThreshold
//                       问题单号：DTS2013112101855
//                       修改人：黄涛/h00207014
//  2013-11-15 09:45:23  修改栈使用过大问题,将栈移入通道
//                       问题单号：DTS2013112101619
//                       修改人：黄涛/h00207014
//  2014-01-07 09:45:23  K3V3_HiFi3移植,删除HighQFiag
//                       修改人：黄涛/h00207014
//  2014-09-17 15:05:33  修改帧长为双声道512点，内部仅开放一个子带
//                       修改人：熊佳/x00227689
//  2014-10-27 12:05:33  完成算法接口修改，读工具生成的文件方式改为结构体赋值方式
//                       修改人：熊佳/x00227689
//  2014-11-20 14:58:30  修改问题单: DTS2014112004156
//                       修改人：熊佳/x00227689
//  2015-06-27 17:20:30  完成24bitDRC兼容16bit修改
//                       修改人：熊佳/x00227689
//==============================================================================

