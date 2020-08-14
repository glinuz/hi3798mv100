/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAZ8tsS4we+PBWC6a/UNlzCWIaw+Ujkv9NAY+as0
fg7WZPJIUfSzQJlmt1gORXgd+g02uWSIiZSbfqz8OUpLLMSgpPOeUIkY6tL59ei/gYEKKdAc
12YTam+MDlMJ+9Y2ugFhulbmHROQCP5e4YAqn1sjzcZCl05G6rZg64U6ulH3jg==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
#ifndef _VDM_HAL_V400R003_API_HEADER_
#define _VDM_HAL_V400R003_API_HEADER_


#include "basedef.h"
#include "mem_manage.h"
#include "vfmw.h"
//#include "vdm_hal_v400r003_local.h"
#include "vfmw_ctrl.h"
#ifdef __cplusplus
extern "C" {
#endif

SINT32 VDMHAL_V400R003_GetHalMemSize(VOID);
SINT32 VDMHAL_V400R003_CalcFsSize(SINT32 ChanID, SINT32 Width, SINT32 Height, SINT32 UserDec, VID_STD_E VidStd, VFMW_FRAME_SIZE_INFO_S *pstVfmwFrameSizeInfo);
SINT32 VDMHAL_V400R003_OpenHAL(VDMHAL_OPENPARAM_S *pOpenParam);
VOID VDMHAL_V400R003_CloseHAL(SINT32 VdhId);
SINT32 VDMHAL_V400R003_ArrangeMem( UADDR MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 FrameNum, SINT32 UserDec, ARRANGE_FLAG_E eFlag, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange );
SINT32 VDMHAL_V400R003_ArrangeMem_BTL( UADDR MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 refNum, SINT32 disNum, SINT32 UserDec, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange, SINT32 btldbdrenable, SINT32 btl1Dt2Denable);
VOID VDMHAL_V400R003_ResetVdm( SINT32 VdhId );
VOID VDMHAL_V400R003_SetSmmuPageTableAddr(UINT32 VdhId);
VOID VDMHAL_V400R003_EnableSmmu( VOID );
VOID VDMHAL_V400R003_GlbReset( VOID );
VOID VDMHAL_V400R003_GlbResetX( SINT32 VdhId);
VOID VDMHAL_V400R003_ResetGlb( VOID );
VOID VDMHAL_V400R003_ClearIntState( SINT32 VdhId );
VOID VDMHAL_V400R003_ClearMMUIntState(SINT32 VdhId);
VOID VDMHAL_V400R003_MaskInt( SINT32 VdhId );
VOID VDMHAL_V400R003_EnableInt( SINT32 VdhId );
SINT32 VDMHAL_V400R003_CheckReg(REG_ID_E reg_id, SINT32 VdhId);
UINT32 VDMHAL_V400R003_ReadMMUMask(SINT32 VdhId);
VOID   VDMHAL_V400R003_WriteMMUMask(UINT32 mask, SINT32 VdhId);
VOID VDMHAL_V400R003_StartHwRepair(SINT32 VdhId, VOID *pTask);
VOID VDMHAL_V400R003_StartHwDecode(SINT32 VdhId, VOID *pTask);
SINT32 VDMHAL_V400R003_PrepareDec( VID_STD_E VidStd, VOID *pDecParam, SINT32 VdhId, VOID *pTask);
SINT32 VDMHAL_V400R003_IsVdmReady(SINT32 VdhId);
SINT32 VDMHAL_V400R003_IsVdmRun(SINT32 VdhId);
SINT32 VDMHAL_V400R003_PrepareRepair( VID_STD_E VidStd, VOID *pDecParam, SINT32 RepairTime, SINT32 VdhId, VOID *pTask);
SINT32 VDMHAL_V400R003_MakeDecReport(MAKE_DEC_REPORT_S *pMakeDecReport);
SINT32 VDMHAL_V400R003_CfgRpReg(VID_STD_E VidStd, VDMHAL_HWMEM_S *pHwMem, VOID *pDecParam, SINT32 VdhId, VOID *pTask);
SINT32 VDMHAL_V400R003_CfgRpMsg(VDMHAL_REPAIR_PARAM_S *pRepairParam, VDMHAL_HWMEM_S *pHwMem, SINT32 VdhId);
SINT32 VDMHAL_V400R003_WriteMsgSlot(SINT32 *pDst, SINT32 *pSrc, UINT32 dnmsg_size);
SINT32 VDMHAL_V400R003_ReadMsgSlot(SINT32 *pDst, SINT32 *pSrc, UINT32 upmsg_size);
SINT32 VDMHAL_V400R003_BackupInfo(BACKUP_INFO_S *pBackUpInfo);
VOID VDMHAL_V400R003_GetCharacter(VOID);
VOID VDMHAL_V400R003_WriteBigTitle1DYuv( VOID *fpYuv, UINT8 *Yaddress, UINT32 Width, UINT32 Height, UINT32 chroma_idc);
VOID   CRG_ConfigReg(UINT32 Reg, UINT32 Data, VOID *pTask);
VOID   MFDE_ConfigReg(UINT32 Reg, UINT32 Data, SINT32 Id, VOID *pTask);
SINT32 VDMHAL_V400R003_GetRpuSize(VOID);

#ifdef __cplusplus
}
#endif


#endif

