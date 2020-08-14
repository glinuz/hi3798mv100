/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name             :   sample_teletext_port.h
  Version               :   Initial Draft
  Author                :   Hisilicon multimedia software group
  Created               :   2012/03/12
  Last Modified         :
  Description           :
  Function List         :
  History               :
  1.Date                :   2012/03/12
    Author              :
Modification            :   Created file
******************************************************************************/

#ifndef __MPI_TELETEXT_PORT_H__
#define __MPI_TELETEXT_PORT_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

typedef struct tagTTX_PORT_S
{
    HI_HANDLE hChannelID;
    HI_HANDLE hFilterID;
    HI_U16    u16PID;
    HI_U16    u16DmxID;
    HI_HANDLE hTTX;
} TTX_PORT_S, *TTX_PORT_S_PTR;

HI_S32 TTX_Port_DmxInstall(TTX_PORT_S* pstTtxParam);

HI_S32 TTX_Port_DmxUninstall(TTX_PORT_S* pstTtxParam);

HI_S32 TTX_Port_DmxChangePid(HI_HANDLE hChannelID, HI_U16 u16Pid);

HI_S32 TTX_Port_CloseChannel(HI_HANDLE hChannelID);

HI_S32 TTX_Port_AcquireBuf(HI_HANDLE hChannelID, HI_U8 *pu8Data, HI_U32 *pu32DataLength);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __MPI_TELETEXT_PORT_H__*/
