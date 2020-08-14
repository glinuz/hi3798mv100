/**
 \file
 \brief common module
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author
 \date 2011-10-9
 */

#ifndef __HI_SVR_CHARSET_H__
#define __HI_SVR_CHARSET_H__

#include "hi_type.h"
#include "hi_svr_charset_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/******************************* API declaration *****************************/
/** \addtogroup      Hiplayer */
/** @{ */  /** <!-- [Hiplayer] */

extern HI_CHARSET_FUN_S g_stCharsetMgr_s;

/**
\brief  init CHARSET module. CNcomment:初始化CHARSET模块CNend
\attention \n
None. CNcomment:无CNend
\param None. CNcomment:无CNend

\retval ::HI_SUCCESS success. CNcomment:初始化成功时返回HI_SUCCESS CNend
\retval ::HI_FAILURE fail. CNcomment:初始化失败时返回HI_FAILURE CNend

\see \n
None. CNcomment:无CNend
*/
HI_S32 HI_CHARSET_Init(HI_VOID);

/**
\brief deinit CHARSET module. CNcomment:去初始化CHARSET模块CNend
\attention \n
None. CNcomment:无CNend
\param None. CNcomment:无CNend

\retval ::HI_SUCCESS success. CNcomment:去初始化成功时返回HI_SUCCESS CNend
\retval ::HI_FAILURE fail. CNcomment:去初始化失败时返回HI_FAILURE CNend

\see \n
None.  CNcomment:无CNend
*/
HI_S32 HI_CHARSET_Deinit(HI_VOID);

/**
\brief regist charset lib. CNcomment:注册charset动态库CNend
\attention \n
None.  CNcomment:无CNend
\param[in] dllName charset lib name. CNcomment:charset动态库名称CNend

\retval ::HI_SUCCESS success. CNcomment:注册成功时返回HI_SUCCESS CNend
\retval ::HI_FAILURE fail, this lib regist already. CNcomment:注册失败时返回HI_FAILURE ,该动态库已经注册过了CNend

\see \n
None. CNcomment:无CNend
*/
HI_S32 HI_CHARSET_RegisterDynamic(const HI_CHAR *dllName);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_SVR_CHARSET_H__ */
