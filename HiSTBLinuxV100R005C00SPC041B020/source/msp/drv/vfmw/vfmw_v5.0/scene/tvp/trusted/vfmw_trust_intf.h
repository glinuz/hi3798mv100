
#ifndef __VFMW_TRUST_INTF_H__
#define __VFMW_TRUST_INTF_H__

#include "vfmw.h"
#ifdef VDH_DISTRIBUTOR_ENABLE
#include "vdh_secure.h"
#endif

#if defined(ENV_SOS_KERNEL)

SINT32 SEC_VFMW_Init(UINT32 pArgs);
SINT32 SEC_VFMW_Exit(VOID);
SINT32 SEC_VFMW_Resume(VOID);
SINT32 SEC_VFMW_Suspend(VOID);
SINT32 SEC_VFMW_Control(SINT32 ChanID, UINT32 eCmdID, UINT32 pArgs);
SINT32 SEC_VFMW_Thread(VOID);
SINT32 SEC_VFMW_Get_Image(SINT32 ChanID, UINT32 pImage);
SINT32 SEC_VFMW_Release_Image(SINT32 ChanID, UINT32 pImage);
SINT32 SEC_VFMW_Read_Proc(UINT32 Page, SINT32 Count);
SINT32 SEC_VFMW_Write_Proc(UINT32 Option, SINT32 Value);

#endif
#endif

