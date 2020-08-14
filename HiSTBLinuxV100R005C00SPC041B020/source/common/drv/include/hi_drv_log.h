/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_log.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/03/10
  Description   :
  History       :
  1.Date        : 2006/03/10
    Author      : f47391
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_LOG_H__
#define __HI_DRV_LOG_H__

#include "hi_type.h"
#include "hi_module.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char* StorePath;

#define STORE_PATH StorePath

/*Define Debug Level For LOG                 */
#define HI_FATAL_LOG(fmt...)    HI_FATAL_PRINT(HI_ID_LOG, fmt)
#define HI_ERR_LOG(fmt...)      HI_ERR_PRINT(HI_ID_LOG, fmt)
#define HI_WARN_LOG(fmt...)     HI_WARN_PRINT(HI_ID_LOG, fmt)
#define HI_INFO_LOG(fmt...)     HI_INFO_PRINT(HI_ID_LOG, fmt)

typedef struct
{
    HI_CHAR *pszPath;
    HI_U32 u32PathLen;
}LOG_PATH_S;

/* compat define for LOG_PATH_S. */
typedef struct
{
    HI_U32 pszPath;
    HI_U32 u32PathLen;
}LOG_Compat_PATH_S;

typedef struct
{
    HI_CHAR *pszPath;
    HI_U32 u32PathLen;
}STORE_PATH_S;

/* compat define for STORE_PATH_S. */
typedef struct
{
    HI_U32 pszPath;
    HI_U32 u32PathLen;
}STORE_Compat_PATH_S;

#ifdef __KERNEL__

#include <linux/seq_file.h>

HI_S32 HI_DRV_LOG_Init(HI_VOID);
HI_VOID HI_DRV_LOG_Exit(HI_VOID);

HI_S32  HI_DRV_LOG_KInit(HI_VOID);
HI_VOID HI_DRV_LOG_KExit(HI_VOID);

HI_VOID HI_DRV_LOG_ConfgBufAddr(HI_U32 *addr);
HI_S32 HI_DRV_LOG_ProcRead(struct seq_file *s, HI_VOID *pArg);
HI_S32 HI_DRV_LOG_ProcWrite(struct file * file,  const char __user * buf,  size_t count, loff_t *ppos);
HI_S32 HI_DRV_LOG_BufferRead(HI_U8 *Buf,  HI_U32 BufLen, HI_U32 *pCopyLen, HI_BOOL bKernelCopy);
HI_S32 HI_DRV_LOG_BufferWrite(HI_U8 *Buf,  HI_U32 MsgLen, HI_U32 UserOrKer);
HI_S32 HI_DRV_LOG_SetPath(HI_CHAR *pszPath, HI_U32 u32PathLen);
HI_S32 HI_DRV_LOG_GetPath(HI_S8* ps8Buf, HI_U32 u32Len);
HI_S32 HI_DRV_LOG_SetStorePath(HI_CHAR *pszPath, HI_U32 u32PathLen);
HI_S32 HI_DRV_LOG_GetStorePath(HI_S8* ps8Buf, HI_U32 u32Len);
#endif

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef __HI_DRV_LOG_H__ */

