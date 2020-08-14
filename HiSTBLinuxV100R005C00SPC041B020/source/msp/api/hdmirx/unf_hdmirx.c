/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : unf_hdmirx.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/01/16
  Description   :
  History       :
  1.Date        : 2014/01/16
    Author      : t00202585
    Modification: Created file
******************************************************************************/
#include "hi_type.h"
#include "hi_unf_hdmirx.h"
#include "hi_mpi_hdmirx.h"

#ifdef __cplusplus
 #if __cplusplus
        extern "C" {
 #endif
#endif /* __cplusplus */
HI_S32 HI_UNF_HDMIRX_Init(HI_VOID)
{
    return HI_MPI_HDMIRX_Init();
}

HI_S32 HI_UNF_HDMIRX_DeInit(HI_VOID)
{
    return HI_MPI_HDMIRX_DeInit();
}

HI_S32 HI_UNF_HDMIRX_Connect(HI_UNF_HDMIRX_PORT_E enPort)
{
    return HI_MPI_HDMIRX_Connect(enPort);
}

HI_S32 HI_UNF_HDMIRX_DisConnect(HI_VOID)
{
    return HI_MPI_HDMIRX_DisConnect();
}
HI_S32 HI_UNF_HDMIRX_GetHdcpStatus(HI_BOOL *pbHdcpSta)
{
    return HI_MPI_HDMIRX_GetHdcpStatus(pbHdcpSta);
}
HI_S32 HI_UNF_HDMIRX_GetTimingInfo(HI_UNF_HDMIRX_TIMING_INFO_S *pstTimingInfo)
{
    return HI_MPI_HDMIRX_GetTimingInfo(pstTimingInfo);
}

HI_S32 HI_UNF_HDMIRX_GetAudioInfo(HI_UNF_AI_HDMI_ATTR_S *pstAudioInfo)
{
    return HI_MPI_HDMIRX_GetAudioInfo(pstAudioInfo);
}

HI_S32 HI_UNF_HDMIRX_GetSignalStatus(HI_UNF_SIG_STATUS_E *penStatus)
{
    return HI_MPI_HDMIRX_GetSigStatus(penStatus);
}

HI_S32 HI_UNF_HDMIRX_GetAudioStatus(HI_UNF_SIG_STATUS_E *penStatus)
{
    return HI_MPI_HDMIRX_GetAudioStatus(penStatus);
}

HI_S32 HI_UNF_HDMIRX_UpdateEdid(HI_UNF_HDMIRX_EDID_S *pstEdid)
{
    return HI_MPI_HDMIRX_UpdateEdid(pstEdid);
}
HI_S32 HI_UNF_HDMIRX_LoadHdcp(HI_UNF_HDMIRX_HDCP_S *pstHdcp)
{
    return HI_MPI_HDMIRX_LoadHdcp(pstHdcp);
}
/*
HI_S32 HI_UNF_HDMIRX_EnMainThread(HI_BOOL bEn)
{
    return HI_MPI_HDMIRX_EnMonitor(bEn);
}
*/
HI_S32 HI_UNF_HDMIRX_GetOffLineDetStatus(HI_UNF_HDMIRX_PORT_E enPort, HI_BOOL *pbConnected)
{
    HI_BOOL bEn=HI_FALSE;


    bEn = HI_MPI_HDMIRX_GetOfflineDetStatus(enPort, pbConnected);

    return bEn;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

