/**********************************************************************

  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.

 ***********************************************************************
  File Name     : hi_mpi_ci.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/01/17
  Description   : 
  History       :
  1.Date        : 2014/01/17
    Author      : z00268517
    Modification: Created file
***********************************************************************/
#ifndef __HI_MPI_CI_H__
#define __HI_MPI_CI_H__

#include "hi_unf_ci.h"
#include "hi_drv_ci.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

HI_S32 HI_MPI_CI_Init(HI_VOID);

HI_S32 HI_MPI_CI_DeInit(HI_VOID);

HI_S32 HI_MPI_CI_SetAttr(HI_UNF_CI_PORT_E enCIPort, const HI_UNF_CI_ATTR_S *pstCIAttr);

HI_S32 HI_MPI_CI_GetAttr(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_ATTR_S *pstCIAttr);

HI_S32 HI_MPI_CI_Open(HI_UNF_CI_PORT_E enCIPort);

HI_S32 HI_MPI_CI_Close(HI_UNF_CI_PORT_E enCIPort);

HI_S32 HI_MPI_CI_PCCD_Open(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

HI_S32 HI_MPI_CI_PCCD_Close(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

HI_S32 HI_MPI_CI_PCCD_CtrlPower(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_CTRLPOWER_E enCtrlPower);

HI_S32 HI_MPI_CI_PCCD_Reset(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

HI_S32 HI_MPI_CI_PCCD_IsReady(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_READY_E_PTR penCardReady);

HI_S32 HI_MPI_CI_PCCD_Detect(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_STATUS_E_PTR penCardStatus);

HI_S32 HI_MPI_CI_PCCD_SetAccessMode(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                    HI_UNF_CI_PCCD_ACCESSMODE_E enAccessMode);

HI_S32 HI_MPI_CI_PCCD_GetStatus (HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                HI_UNF_CI_PCCD_STATUS_BIT_E enStatus, HI_U8 *pu8Value);

HI_S32 HI_MPI_CI_PCCD_IORead(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_U8 *pu8Buffer, HI_U32 *pu32ReadLen);

HI_S32 HI_MPI_CI_PCCD_IOWrite(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_U8 *pu8Buffer, HI_U32 u32WriteLen, HI_U32 *pu32WriteOKLen);

HI_S32 HI_MPI_CI_PCCD_CheckCIS(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

HI_S32 HI_MPI_CI_PCCD_WriteCOR(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

HI_S32 HI_MPI_CI_PCCD_IOReset(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

HI_S32 HI_MPI_CI_PCCD_NegBufferSize(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                    HI_U16 *pu16BufferSize);

HI_S32 HI_MPI_CI_PCCD_TSCtrl(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                        HI_UNF_CI_PCCD_TSCTRL_E enCMD, HI_UNF_CI_PCCD_TSCTRL_PARAM_U *pParam);

HI_S32 HI_MPI_CI_PCCD_GetCIS(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                             HI_U8 *pu8CisBuffer, HI_U32 *pu32CisLen);

HI_S32 HI_MPI_CI_PCCD_WriteCOREx(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                  HI_U16 u16Addr, HI_U8 u8Data);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#endif // __HI_MPI_CI_H__

