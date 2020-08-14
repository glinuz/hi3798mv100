/*****************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_aenc.c
* Description: Describe main functionality and purpose of this file.
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01      
*
*****************************************************************************/


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"

#include "hi_mpi_ai.h"
#include "hi_error_mpi.h"


HI_S32 HI_UNF_AI_Init(HI_VOID)
{

    return HI_MPI_AI_Init();
}

HI_S32   HI_UNF_AI_DeInit(HI_VOID)
{

    return HI_MPI_AI_DeInit();
}

HI_S32   HI_UNF_AI_GetDefaultAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S *pstAttr)
{

    return HI_MPI_AI_GetDefaultAttr(enAiPort,pstAttr);
}

HI_S32 HI_UNF_AI_SetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S *pstAttr)
{

    return HI_MPI_AI_SetAttr(hAI,pstAttr);
}

HI_S32 HI_UNF_AI_GetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S *pstAttr)
{

    return HI_MPI_AI_GetAttr(hAI,pstAttr);
}

HI_S32 HI_UNF_AI_Create(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S *pstAttr, HI_HANDLE *phandle)
{

    return HI_MPI_AI_Create(enAiPort,pstAttr,phandle);
}

HI_S32 HI_UNF_AI_Destroy(HI_HANDLE hAI)
{
    return HI_MPI_AI_Destroy(hAI);
}

HI_S32 HI_UNF_AI_SetEnable(HI_HANDLE hAI, HI_BOOL bEnable)
{
    return HI_MPI_AI_SetEnable(hAI,bEnable);
}

HI_S32 HI_UNF_AI_GetEnable(HI_HANDLE hAI, HI_BOOL *pbEnable)
{
    return HI_MPI_AI_GetEnable(hAI, pbEnable);
}

HI_S32 HI_UNF_AI_AcquireFrame(HI_HANDLE hAI, HI_UNF_AO_FRAMEINFO_S *pstFrame, HI_U32 u32TimeoutMs)
{
    return HI_MPI_AI_AcquireFrame(hAI,pstFrame);
}

HI_S32 HI_UNF_AI_ReleaseFrame(HI_HANDLE hAI, HI_UNF_AO_FRAMEINFO_S *pstFrame)
{
    return HI_MPI_AI_ReleaseFrame(hAI,pstFrame);
}

HI_S32 HI_UNF_AI_SetDelay(HI_HANDLE hAI, const HI_UNF_AI_DELAY_S *pstDelay)
{
    return HI_ERR_AI_NOTSUPPORT;
}

HI_S32 HI_UNF_AI_GetDelay(HI_HANDLE hAI, HI_UNF_AI_DELAY_S *pstDelay)
{
    return HI_ERR_AI_NOTSUPPORT;
}    

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
