/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_dbg.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/02/13
  Description   :
  History       :
  1.Date        : 2006/02/13
    Author      : f47391
    Modification: Created file

******************************************************************************/



#ifndef __MPI_LOG_H__
#define __MPI_LOG_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "drv_log_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

HI_S32 HI_MPI_LogInit(HI_VOID);
HI_VOID HI_MPI_LogDeInit(HI_VOID);
HI_S32 HI_MPI_LogRead(HI_U8 *pBuf,  HI_U32 BufLen, HI_U32 *pCopyLen);
HI_S32 HI_MPI_LogLevelSet(HI_U32 u32ModId,  HI_LOG_LEVEL_E enLogLevel);
HI_S32 HI_MPI_LogPrintPosSet(HI_U32 u32ModId,  LOG_OUTPUT_POS_E enPrintPos);
HI_S32 HI_MPI_LogPathSet(const HI_CHAR* pszLogPath);
HI_S32 HI_MPI_StorePathSet(const HI_CHAR* pszLogPath);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __MPI_LOG_H__ */

