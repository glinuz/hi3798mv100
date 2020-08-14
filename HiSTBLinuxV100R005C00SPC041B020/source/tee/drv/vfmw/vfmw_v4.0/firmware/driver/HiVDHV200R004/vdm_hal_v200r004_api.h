/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv/JzwTgKEHe/9N4yU5rqhECzcd0qHjxrE76U1z7jqu9Y5M3Mt4N+LrDnKskR
p9kctvx+J4nlzw4W/gdvECE93LhbHurm3gLKkfvoRnOy9JN+Fg4bu/VRNW9tekbJVUGKWGf3
f2h4CPxg23m7ZKLnFq4fQgNTVq9V/bhqlx2YY8lqvLU9c988It0Q6r8M5ZwFDQ==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
#ifndef _VDM_HAL_V200R004_API_HEADER_
#define _VDM_HAL_V200R004_API_HEADER_


#include "basedef.h"
#include "mem_manage.h"
#include "vfmw.h"
#include "vdm_hal_v200r004_local.h"

#ifdef __cplusplus
extern "C" {
#endif

VOID VDMHAL_V200R004_GetHalMemSize(VDMHAL_MEM_SIZE_S *pVDMHALMemSize);
SINT32 VDMHAL_V200R004_OpenHAL(VDMHAL_OPENPARAM_S *pOpenParam);
VOID VDMHAL_V200R004_CloseHAL(SINT32 VdhId);
SINT32 VDMHAL_V200R004_ArrangeMem( SINT32 MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 FrameNum, SINT32 UserDec, SINT32 ChanID, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange );
SINT32 VDMHAL_V200R004_ArrangeMem_BTL( SINT32 MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 refNum, SINT32 disNum, SINT32 UserDec, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange, SINT32 btldbdrenable, SINT32 btl1Dt2Denable);
VOID VDMHAL_V200R004_ResetVdm( SINT32 VdhId );
VOID VDMHAL_V200R004_GlbReset( VOID );
VOID VDMHAL_V200R004_ResetGlb( VOID );
VOID VDMHAL_V200R004_ClearIntState( SINT32 VdhId );
VOID VDMHAL_V200R004_MaskInt( SINT32 VdhId );
VOID VDMHAL_V200R004_EnableInt( SINT32 VdhId );
SINT32 VDMHAL_V200R004_CheckReg(REG_ID_E reg_id, SINT32 VdhId);
VOID VDMHAL_V200R004_StartHwRepair(SINT32 VdhId);
VOID VDMHAL_V200R004_StartHwDecode(SINT32 VdhId);
SINT32 VDMHAL_V200R004_PrepareDec( VID_STD_E VidStd, VOID *pDecParam, SINT32 VdhId );
SINT32 VDMHAL_V200R004_IsVdmReady(SINT32 VdhId);
SINT32 VDMHAL_V200R004_IsVdmRun(SINT32 VdhId);
SINT32 VDMHAL_V200R004_PrepareRepair( VID_STD_E VidStd, VOID *pDecParam, SINT32 RepairTime, SINT32 VdhId );
SINT32 VDMHAL_V200R004_MakeDecReport(MAKE_DEC_REPORT_S *pMakeDecReport);
SINT32 VDMHAL_V200R004_CfgRpReg(CFG_REPAIR_REG_S *pCfgRepairReg);
SINT32 VDMHAL_V200R004_CfgRpMsg(CFG_REPAIR_MSG_S *pCfgRepairMsg);
SINT32 VDMHAL_V200R004_WriteMsgSlot(SINT32 *pDst, SINT32 *pSrc, UINT32 dnmsg_size);
SINT32 VDMHAL_V200R004_ReadMsgSlot(SINT32 *pDst, SINT32 *pSrc, UINT32 upmsg_size);
SINT32 VDMHAL_V200R004_BackupInfo(BACKUP_INFO_S *pBackUpInfo);
VOID VDMHAL_V200R004_GetCharacter(VOID);
VOID VDMHAL_V200R004_WriteBigTitle1DYuv( FILE *fpYuv, UINT8 *Yaddress, UINT32 Width, UINT32 Height, UINT32 chroma_idc);
#ifdef __cplusplus
}
#endif


#endif

