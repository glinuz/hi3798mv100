/**
 * @file sre_errno.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：通用错误码定义头文件。 \n
 */

#ifndef _SRE_ERRNO_H
#define _SRE_ERRNO_H

#include "sre_typedef.h"
#include "sre_err.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/* 0x00表示OS,0xFF表示DRV,应用系统可以使用 1~254 来标示应用各个子系统 */
/**
 * @ingroup SRE_err
 * OS错误码标记位。(0x00表示OS,0xFF表示DRV,应用系统可以使用 1~254 来标示应用各个子系统)
 *
 */
#define SRE_ERRNO_OS_ID                                     ((UINT32)0x00 << 16)

/**
 * @ingroup SRE_err
 * 定义错误的等级:提示级别
 *
 */
#define SRE_ERRTYPE_NORMAL                                  ((UINT32)0x00 << 24)

/**
 * @ingroup SRE_err
 * 定义错误的等级:告警级别
 *
 */
#define SRE_ERRTYPE_WARN                                    ((UINT32)0x01 << 24)

/**
 * @ingroup SRE_err
 * 定义错误的等级:严重级别
 *
 */
#define SRE_ERRTYPE_ERROR                                   ((UINT32)0x02 << 24)

/**
 * @ingroup SRE_err
 * 定义错误的等级:致命级别
 *
 */
#define SRE_ERRTYPE_FATAL                                   ((UINT32)0x03 << 24)

/**
 * @ingroup  SRE_err
 * @brief 定义OS致命错误
 *
 * @par 描述:
 * 宏定义，定义OS致命错误
 *
 * @attention 无

 * @param  MID [IN] 模块ID编号。
 * @param  ERRNO [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖:
 * sre_errno.h: 该宏定义所在的头文件。
 * @since UniDSP V100R001C01
 * @see SRE_ERRNO_OS_ERROR | SRE_ERRNO_OS_WARN | SRE_ERRNO_OS_NORMAL
 */
#define SRE_ERRNO_OS_FATAL(MID,ERRNO)   \
            (SRE_ERRTYPE_FATAL | SRE_ERRNO_OS_ID | ((UINT32)(MID) << 8) |  (ERRNO))


/**
 * @ingroup  SRE_err
 * @brief 定义OS严重错误
 *
 * @par 描述:
 * 宏定义，定义OS严重错误
 *
 * @attention 无
 * @param  MID [IN] 模块ID编号。
 * @param  ERRNO [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖:
 * sre_errno.h: 该宏定义所在的头文件。
 * @since UniDSP V100R001C01
 * @see SRE_ERRNO_OS_FATAL | SRE_ERRNO_OS_WARN | SRE_ERRNO_OS_NORMAL
 */
#define SRE_ERRNO_OS_ERROR(MID,ERRNO)  \
            (SRE_ERRTYPE_ERROR | SRE_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))


/**
 * @ingroup  SRE_err
 * @brief 定义OS告警信息
 *
 * @par 描述:
 * 宏定义，定义OS告警
 *
 * @attention 无
 * @param  MID [IN] 模块ID编号。
 * @param  ERRNO [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖:
 * sre_errno.h: 该宏定义所在的头文件。
 * @since UniDSP V100R001C01
 * @see SRE_ERRNO_OS_FATAL | SRE_ERRNO_OS_ERROR | SRE_ERRNO_OS_NORMAL
 */
#define SRE_ERRNO_OS_WARN(MID,ERRNO)  \
            (SRE_ERRTYPE_WARN | SRE_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))


/**
 * @ingroup  SRE_err
 * @brief 定义OS提示信息
 *
 * @par 描述:
 * 宏定义，定义OS提示信息
 *
 * @attention 无
 * @param  MID [IN] 模块ID编号。
 * @param  ERRNO [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖:
 * sre_errno.h: 该宏定义所在的头文件。
 * @since UniDSP V100R001C01
 * @see SRE_ERRNO_OS_FATAL | SRE_ERRNO_OS_ERROR | SRE_ERRNO_OS_WARN
 */
#define SRE_ERRNO_OS_NORMAL(MID,ERRNO)  \
            (SRE_ERRTYPE_NORMAL | SRE_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))

#define REPORT_ERROR(uwErrNo)  \
    do \
    { \
        (VOID)SRE_ErrHandle("__FILE__", __LINE__, uwErrNo, 0, NULL); \
    } while (0)

#define REPORT_ERROR_P2(uwErrLine, uwErrNo)  \
    do \
    { \
        (VOID)SRE_ErrHandle("__FILE__", uwErrLine, uwErrNo, 0, NULL); \
    } while (0)

#define RETURN_ERROR(uwErrNo) \
    do \
    {\
       (VOID)SRE_ErrHandle("__FILE__", __LINE__, uwErrNo, 0, NULL); \
        \
        return uwErrNo; \
    } while (0)

#define RETURN_ERROR_P2(uwErrLine, uwErrNo) \
    do \
    {\
        (VOID)SRE_ErrHandle("__FILE__", uwErrLine, uwErrNo, 0, NULL); \
        \
        return uwErrNo; \
    }while(0)

#define GOTO_ERR_HANDLER(uwErrorNo) \
    do \
    { \
        uwErrNo    = uwErrorNo; \
        uwErrLine  = __LINE__; \
        goto ErrHandler; \
    }while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_ERRNO_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */


