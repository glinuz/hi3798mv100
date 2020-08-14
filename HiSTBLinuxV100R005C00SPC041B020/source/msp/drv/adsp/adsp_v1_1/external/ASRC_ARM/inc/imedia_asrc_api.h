//==============================================================================
//           Copyright (C), 2008-2014, Huawei Tech. Co., Ltd.
//==============================================================================
//  文件名称: imedia_asrc_api.h
//  文件描述: 对外接口头文件
//
//  结构体列表:
//      iMedia_ASRC_STRU_MEM_CONFIG  -- 内存配置结构体
//      iMedia_ASRC_STRU_PARAMS      -- 参数配置结构体
//      iMedia_ASRC_STRU_MEM_SIZE    -- 内存大小结构体
//      iMedia_ASRC_STRU_Version     -- 版本信息结构体
//      iMedia_ASRC_STRU_DATA        -- I/O数据结构体
//
//  函数列表:
//      iMedia_ASRC_GetSize()        -- 获取通道变量大小
//      iMedia_ASRC_Init()           -- 初始化算法实例
//      iMedia_ASRC_Flush()          -- 清空通道历史值
//      iMedia_ASRC_Apply()          -- 算法处理应用
//      iMedia_ASRC_GetParams()      -- 获取算法参数配置
//      iMedia_ASRC_GetVersion()     -- 获取算法库版本信息，
//                                      包括所用编译器版本发布时间和版本号
//
//  修改记录: 见文件尾
//==============================================================================
#ifndef _IMEDIA_ASRC_API_H_
#define _IMEDIA_ASRC_API_H_

#include "imedia_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
//     返回码定义信息
//==============================================================================
#define IMEDIA_ASRC_EOK                     (0)          //操作成功
#define IMEDIA_ASRC_INV_HANDLE              (-1)         //无效句柄，算法句柄为NULL
#define IMEDIA_ASRC_INV_BASE                (-2)         //无效的基地址，基地址为NULL
#define IMEDIA_ASRC_INV_MEM_SIZE            (-3)         //存储缓冲区小于要求长度
#define IMEDIA_ASRC_INV_MEMSIZE_PST         (-4)         //内存尺寸结构体指针为空
#define IMEDIA_ASRC_INV_MEMCFG_PST          (-5)         //参数配置结构体指针为空
#define IMEDIA_ASRC_INV_PARAMS              (-6)         //配置结构体指针为NULL
#define IMEDIA_ASRC_INV_SAMPLERATE          (-7)         //无效的采样率
#define IMEDIA_ASRC_INV_CHANNELS            (-8)         //无效的声道数
#define IMEDIA_ASRC_INV_INBITS              (-9)         //无效的输入位宽
#define IMEDIA_ASRC_INV_DATASIZE_IN         (-10)        //无效的输入数据长度
#define IMEDIA_ASRC_INV_DATA                (-11)        //数据结构体指针为空
#define IMEDIA_ASRC_INV_UNINITIED           (-12)        //通道变量未初始化
#define IMEDIA_ASRC_INV_INBUFF              (-13)        //输入缓冲区为空
#define IMEDIA_ASRC_INV_OUTBUFF             (-14)        //输出缓冲区为空
#define IMEDIA_ASRC_INV_INBUFF_ADDRESS      (-15)        //无效的输入缓冲区地址
#define IMEDIA_ASRC_INV_OUTBUFF_ADDRESS     (-16)        //无效的输出缓冲区地址
#define IMEDIA_ASRC_INV_VERSION             (-17)        //无效版本信息结构体指针

//==============================================================================
//     枚举类型定义
//==============================================================================
//声道数范围
typedef enum
{
    IMEDIA_ASRC_MONO    = 1,                             //单声道
    IMEDIA_ASRC_STEREO  = 2                              //双声道,左右声道数据交叉存放
}iMedia_ASRC_CHANNELS;

//输入比特位宽范围
typedef enum
{
    IMEDIA_ASRC_16BITS  = 16                             //位宽16bit
}iMedia_ASRC_INBITS;

//采样率范围
typedef enum
{
    IMEDIA_ASRC_44100HZ = 44100,                         //采样率44100Hz
    IMEDIA_ASRC_48000HZ = 48000                          //采样率48000Hz
}iMedia_ASRC_SAMPLERATE;

//==============================================================================
//     复合数据类型定义信息
//==============================================================================

//内存配置结构体
typedef struct tagiMedia_ASRC_STRU_MEM_CONFIG
{
    IMEDIA_INT32 iInSampleRate;                                 //输入数据采样率
    IMEDIA_INT32 iOutSampleRate;                                //输出数据采样率
    IMEDIA_INT32 iInBits;                                       //数据位宽
    IMEDIA_INT32 iChannels;                                     //声道数
}iMedia_ASRC_STRU_MEM_CONFIG, *iMedia_ASRC_PST_MEM_CONFIG;

//参数配置结构体
typedef struct tagiMedia_ASRC_STRU_PARAMS
{
    IMEDIA_INT32 iInSampleRate;                                 //输入数据采样率
    IMEDIA_INT32 iOutSampleRate;                                //输出数据采样率
    IMEDIA_INT32 iInBits;                                       //数据位宽
    IMEDIA_INT32 iChannels;                                     //声道数
    IMEDIA_INT32 iReserve[4];                                   //保留区
}iMedia_ASRC_STRU_PARAMS, *iMedia_ASRC_PST_PARAMS;

//内存大小结构体
typedef struct tagiMedia_ASRC_STRU_MEM_SIZE
{
    IMEDIA_UINT32 uiStrSize;                                    //通道变量大小，单位为字节
}iMedia_ASRC_STRU_MEM_SIZE, *iMedia_ASRC_PST_MEM_SIZE;

//版本信息结构体
#define IMEDIA_ASRC_VERSION_MSG_VERLEN        (64)              //算法库版本信息字符串长度
typedef struct tagASRC_STRU_VERSION
{
    IMEDIA_UINT8 ucCgtVersion[IMEDIA_ASRC_VERSION_MSG_VERLEN];  //编译器版本号
    IMEDIA_UINT8 ucReleaseVer[IMEDIA_ASRC_VERSION_MSG_VERLEN];  //算法库版本号
    IMEDIA_UINT8 ucReleaseTime[IMEDIA_ASRC_VERSION_MSG_VERLEN]; //发布日期
}iMedia_ASRC_STRU_VERSION, *iMedia_ASRC_PST_VERSION;

//I/O数据结构体
typedef struct tagASRC_STRU_DATA
{
    IMEDIA_VOID  *piInData;                                     //输入数据地址，需数据位宽字节数对齐
    IMEDIA_VOID  *piOutData;                                    //输出数据地址，需数据位宽字节数对齐
    IMEDIA_INT32 iInSize;                                       //输入样点数，获取实时每帧的输入样点数，单声道范围(0,1024]，双声道范围(0,2048]
    IMEDIA_INT32 iOutSize;                                      //输出样点数，获取实时每帧的输出样点数，单声道输出(0,1120),双声道输出(0,2240)
}iMedia_ASRC_STRU_DATA, *iMedia_ASRC_PST_DATA;

//==============================================================================
//     函数声明
//==============================================================================

//==============================================================================
// 函数名称: iMedia_ASRC_GetSize
// 功能描述: 获取通道大小
// 输入参数:
//     pstConf     -- 内存配置结构体
// 输出参数:
//     pstMemSize  -- 通道大小结构体
// 返回参数:
//     ret         -- 操作返回码 IMEDIA_ASRC_EOK表示成功，其他返回码表示失败
//==============================================================================
IMEDIA_INT32 iMedia_ASRC_GetSize(iMedia_ASRC_PST_MEM_SIZE pstMemSize, iMedia_ASRC_PST_MEM_CONFIG pstConf);

//==============================================================================
// 函数名称: iMedia_ASRC_Init
// 功能描述: 初始化算法实例(通道变量)，并返回其句柄
// 输入参数:
//     ppHandle    -- 对象句柄
//     pMemBase    -- 通道变量起始地址
//     uiMemSize   -- 通道变量空间大小
//     pstParams   -- 参数配置结构体
// 输出参数:
//     ppHandle    -- 对象句柄
// 返回参数:
//     ret         -- 操作返回码 IMEDIA_ASRC_EOK表示成功，其他返回码表示失败
//==============================================================================
IMEDIA_INT32 iMedia_ASRC_Init(IMEDIA_VOID **ppHandle,  const IMEDIA_VOID *pMemBase, IMEDIA_UINT32 uiMemSize, iMedia_ASRC_PST_PARAMS pstParams);

//==============================================================================
// 函数名称: iMedia_ASRC_Flush
// 功能描述: 保留通道初始化参数，清空通道历史值
// 输入参数:
//     pHandle   -- 对象句柄
// 输出参数:
//     pHandle   -- 对象句柄
// 返回参数:
//     ret       -- 操作返回码 IMEDIA_ASRC_EOK表示成功，其他返回码表示失败
//==============================================================================
IMEDIA_INT32 iMedia_ASRC_Flush(IMEDIA_VOID *pHandle);

//==============================================================================
// 函数名称: iMedia_ASRC_Apply
// 功能描述: 算法处理应用
// 输入参数:
//     pHandle   -- 对象句柄
//     pstDatas  -- I/0数据结构体指针
// 输出参数:
//     pstDatas
// 返回参数:
//     ret       -- 操作返回码 IMEDIA_ASRC_EOK表示成功，其他返回码表示失败
//==============================================================================
IMEDIA_INT32 iMedia_ASRC_Apply(IMEDIA_VOID *pHandle, iMedia_ASRC_PST_DATA pstDatas);

//==============================================================================
// 函数名称: iMedia_ASRC_GetParams
// 功能描述: 获取配置
// 输入参数:
//     pHandle     -- 对象句柄
// 输出参数:
//     pstParams   -- 参数配置结构体
// 返回参数:
//     ret         -- 操作返回码 IMEDIA_ASRC_EOK表示成功，其他返回码表示失败
//==============================================================================
IMEDIA_INT32 iMedia_ASRC_GetParams(IMEDIA_VOID *pHandle,  iMedia_ASRC_PST_PARAMS pstParams);

//==============================================================================
// 函数名称: iMedia_ASRC_GetVersion
// 功能描述: 获取算法库版本信息，包括所用编译器版本、发布时间和版本号
// 输入参数:
//     ppVersion    -- 版本信息结构体句柄
// 输出参数:
//     ppVersion    -- 版本信息结构体句柄
// 返回参数:
//     ret          -- 操作返回码 IMEDIA_ASRC_EOK表示成功，其他返回码表示失败
//==============================================================================
IMEDIA_INT32 iMedia_ASRC_GetVersion(iMedia_ASRC_PST_VERSION *ppVersion);

#ifdef __cplusplus
}
#endif

#endif


//==============================================================================
// 修改记录:
// 2014-1-29 11:45:34   创建文件
//                      修改人: 黄涛/h00207014
// 2014-12-19 14:10:11  修改文件
//                      修改人：闫丽霞/y00278990
//==============================================================================
