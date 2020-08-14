/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : sme_log.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2011年10月10日
  最近修改   :
  功能描述   : ICS终端软件平台日志系统对外的日志接口
  函数列表   :
  修改历史   :
  1.日    期   : 2011年10月10日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/
#ifndef __ICS_LOG_H__
#define __ICS_LOG_H__

#include "osal_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*定义日志系统的开关枚举*/
typedef enum e_ICS_LogSwitch{
    ICS_LOG_SWITCH_OFF    = 0,
    ICS_LOG_SWITCH_ON     = 1,
}E_ICS_LOG_SWITCH;

/*定义日志系统的格式枚举，这里根据日志的详细程度定义两种类型日志
1. 简单型日志，只包含日志的内容；
2. 复杂型日志，包括日志内容、文件名、行号；*/
typedef enum e_ICS_LogStyle{
    ICS_LOG_STYLE_SIMPLE    = 0,
    ICS_LOG_STYLE_DETAILED  = 1,
}E_ICS_LOG_STYLE;

/*定一日志级别枚举*/
typedef enum e_ICS_LogLevel{
    ICS_LOG_VERBOSE    = 0,
    ICS_LOG_DEBUG      = 1,
    ICS_LOG_INFO       = 2,
    ICS_LOG_WARNING    = 3,
    ICS_LOG_ERROR      = 4,
    ICS_LOG_BUTT
}E_ICS_LOG_LEVEL;

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/*定义日志的模块名，外部使用日志系统时需定义自己的模块名，若不定义，系统默认
将模块名设置为ICS_LOG,即整个平台的名字*/
#ifndef MOD_NAME
#define MOD_NAME "SME"
#endif

/*定义平台的日志打印宏和整个日志系统的开关
1. 当平台日志编译开关ICS_RELEASE_VERSION未被定义或打开，那么就日志系统将输出日志*/

#ifdef ICS_RELEASE_VERSION
#define ICS_LOGV(pccFmt,...) ((V_VOID)0)
#define ICS_LOGI(pccFmt,...) ((V_VOID)0)
#define ICS_LOGW(pccFmt,...) ICS_Log(ICS_LOG_WARNING, MOD_NAME,\
                                                __FILE__, __FUNCTION__, __LINE__, pccFmt,##__VA_ARGS__)
#define ICS_LOGD(pccFmt,...) ((V_VOID)0)
#define ICS_LOGE(pccFmt,...) ICS_Log(ICS_LOG_ERROR, MOD_NAME,\
                                                __FILE__, __FUNCTION__, __LINE__, pccFmt,##__VA_ARGS__)
#else
#define ICS_LOGV(pccFmt,...) ICS_Log(ICS_LOG_VERBOSE, MOD_NAME,\
                                                __FILE__, __FUNCTION__, __LINE__, pccFmt,##__VA_ARGS__)
#define ICS_LOGI(pccFmt,...) ICS_Log(ICS_LOG_INFO, MOD_NAME,\
                                                __FILE__, __FUNCTION__, __LINE__, pccFmt,##__VA_ARGS__)
#define ICS_LOGW(pccFmt,...) ICS_Log(ICS_LOG_WARNING, MOD_NAME,\
                                                __FILE__, __FUNCTION__, __LINE__, pccFmt,##__VA_ARGS__)
#define ICS_LOGD(pccFmt,...) ICS_Log(ICS_LOG_DEBUG, MOD_NAME,\
												__FILE__, __FUNCTION__, __LINE__, pccFmt,##__VA_ARGS__)
#define ICS_LOGE(pccFmt,...) ICS_Log(ICS_LOG_ERROR, MOD_NAME,\
                                                __FILE__, __FUNCTION__, __LINE__, pccFmt,##__VA_ARGS__)
#endif

/*定义单条日志缓冲区buff大小*/
#ifndef ICS_LOG_BUF_SIZE
#define ICS_LOG_BUF_SIZE 4096
#endif

/*定义日志模块号长度大小*/
#ifndef ICS_LOG_TAG_LENGTH
#define ICS_LOG_TAG_LENGTH 256
#endif

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
 /*****************************************************************************
 函 数 名  : ICS_SetLogSwitch
 功能描述  : 设置某级别日志开关
 输入参数  : IN E_ICS_LOG_LEVEL eLogLevel      :日志级别
             IN E_ICS_LOG_SWITCH eDebugSwitch  :日志开关
 输出参数  : 无
 返 回 值  : V_VOID
 修改历史      :
  1.日    期   : 2011年10月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
V_VOID ICS_SetLogSwitch(IN E_ICS_LOG_LEVEL eLogLevel, \
                        IN E_ICS_LOG_SWITCH eDebugSwitch);

/*****************************************************************************
 函 数 名  : ICS_GetLogSwitchStatus
 功能描述  : 获取某级别日志开关
 输入参数  : IN E_ICS_LOG_LEVEL eLogLevel        :日志级别
 输出参数  : OUT E_ICS_LOG_SWITCH *peDebugSwitch :日志开关 指针
 返 回 值  : V_UINT32
 修改历史      :
  1.日    期   : 2011年10月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
V_UINT32 ICS_GetLogSwitchStatus(IN E_ICS_LOG_LEVEL eLogLevel, \
                                OUT E_ICS_LOG_SWITCH *peDebugSwitch);

/*****************************************************************************
 函 数 名  : ICS_SetLogLevel
 功能描述  : 设置日志开关群，设置指定级别之上(包括指定界别)的日志开关都打开，
             之下的都关闭
 输入参数  : IN E_ICS_LOG_LEVEL eLogLevel  :日志级别
 输出参数  : 无
 返 回 值  : V_VOID
 修改历史      :
  1.日    期   : 2011年10月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
V_VOID ICS_SetLogLevel(IN E_ICS_LOG_LEVEL eLogLevel);

/*****************************************************************************
 函 数 名  : ICS_GetLogLevel
 功能描述  : 获取日志级别
 输入参数  : 无
 输出参数  : 无
 返 回 值  : E_ICS_LOG_LEVEL :日志级别
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年8月1日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/

E_ICS_LOG_LEVEL ICS_GetLogLevel();


/*****************************************************************************
 函 数 名  : ICS_SetLogStyle
 功能描述  : 设置日志的风格，包括简约型和详细型
 输入参数  : IN E_ICS_LOG_STYLE eLogStyle  :日志风格
 输出参数  : 无
 返 回 值  : V_VOID
 修改历史      :
  1.日    期   : 2011年10月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
V_VOID ICS_SetLogStyle(IN E_ICS_LOG_STYLE eLogStyle);

/*****************************************************************************
 函 数 名  : ICS_Log
 功能描述  : 打印日志函数入口(此接口不建议外部使用，外部请使用ICS_LOGV/I/W/D/E)
 输入参数  : IN E_ICS_LOG_LEVEL eLogLevel     :日志级别
             IN const V_CHAR *pcszModName     :日志模块名
             IN const V_CHAR *pcszFileName    :日志所在文件名
             IN const V_CHAR *pcszFuncName    :日志所在函数名
             IN V_UINT32 u32LineNum           :日志所在行号
             IN const V_CHAR * pcszFmt        :日志格式化字符串
             ...
 输出参数  : 无
 返 回 值  :
 修改历史      :
  1.日    期   : 2011年10月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
V_VOID ICS_Log(IN E_ICS_LOG_LEVEL eLogLevel,
               IN const V_CHAR *pcszModName,
               IN const V_CHAR *pcszFileName,
               IN const V_CHAR *pcszFuncName,
               IN V_UINT32 u32LineNum,
               IN const V_CHAR *pcszFmt, ...);

/*****************************************************************************
 函 数 名  : ICS_FlushLogBuff
 功能描述  : 将缓冲区的日志信息输出至文件
 输入参数  :
             ...
 输出参数  : 无
 返 回 值  :
 修改历史      :
  1.日    期   : 2015年7月24日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
V_VOID ICS_FlushLogBuff();


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __ICS_LOG_H__ */
