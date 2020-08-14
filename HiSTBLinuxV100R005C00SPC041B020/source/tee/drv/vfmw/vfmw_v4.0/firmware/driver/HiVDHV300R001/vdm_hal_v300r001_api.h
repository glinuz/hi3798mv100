/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAZ8tsS4we+PBWC6a/UNlzCWIaw+Ujkv9NAY+as0
fg7WZPJIUfSzQJlmt1gORXgd+g02uWSIiZSbfqz8OUpLLMSgpPOeUIkY6tL59ei/gYEKKdAc
12YTam+MDlMJ+9Y2ugFhulbmHROQCP5e4YAqn1sjzcZCl05G6rZg64U6ulH3jg==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
#ifndef _VDM_HAL_V300R001_API_HEADER_
#define _VDM_HAL_V300R001_API_HEADER_


#include "basedef.h"
#include "mem_manage.h"
#include "vfmw.h"
#include "vdm_hal_v300r001_local.h"
#include "vfmw_ctrl.h"
#ifdef __cplusplus
extern "C" {
#endif

VOID VDMHAL_V300R001_GetHalMemSize(VDMHAL_MEM_SIZE_S *pVDMHALMemSize);
SINT32 VDMHAL_V300R001_OpenHAL(VDMHAL_OPENPARAM_S *pOpenParam);
VOID VDMHAL_V300R001_CloseHAL(SINT32 VdhId);
SINT32 VDMHAL_V300R001_ArrangeMem( SINT32 MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 FrameNum, SINT32 UserDec, ARRANGE_FLAG_E eFlag, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange );
SINT32 VDMHAL_V300R001_ArrangeMem_BTL( SINT32 MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 refNum, SINT32 disNum, SINT32 UserDec, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange, SINT32 btldbdrenable, SINT32 btl1Dt2Denable);
VOID VDMHAL_V300R001_ResetVdm( SINT32 VdhId );
VOID VDMHAL_V300R001_GlbReset( VOID );
VOID VDMHAL_V300R001_ResetGlb( VOID );
VOID VDMHAL_V300R001_ClearIntState( SINT32 VdhId );
VOID VDMHAL_V300R001_MaskInt( SINT32 VdhId );
VOID VDMHAL_V300R001_EnableInt( SINT32 VdhId );
SINT32 VDMHAL_V300R001_CheckReg(REG_ID_E reg_id, SINT32 VdhId);
VOID VDMHAL_V300R001_StartHwRepair(SINT32 VdhId);
VOID VDMHAL_V300R001_StartHwDecode(SINT32 VdhId);
SINT32 VDMHAL_V300R001_PrepareDec( VID_STD_E VidStd, VOID *pDecParam, SINT32 VdhId );
SINT32 VDMHAL_V300R001_IsVdmReady(SINT32 VdhId);
SINT32 VDMHAL_V300R001_IsVdmRun(SINT32 VdhId);
SINT32 VDMHAL_V300R001_PrepareRepair( VID_STD_E VidStd, VOID *pDecParam, SINT32 RepairTime, SINT32 VdhId );
SINT32 VDMHAL_V300R001_MakeDecReport(MAKE_DEC_REPORT_S *pMakeDecReport);
SINT32 VDMHAL_V300R001_CfgRpReg(CFG_REPAIR_REG_S *pCfgRepairReg);
SINT32 VDMHAL_V300R001_CfgRpMsg(CFG_REPAIR_MSG_S *pCfgRepairMsg);
SINT32 VDMHAL_V300R001_WriteMsgSlot(SINT32 *pDst, SINT32 *pSrc, UINT32 dnmsg_size);
SINT32 VDMHAL_V300R001_ReadMsgSlot(SINT32 *pDst, SINT32 *pSrc, UINT32 upmsg_size);
SINT32 VDMHAL_V300R001_BackupInfo(BACKUP_INFO_S *pBackUpInfo);
VOID VDMHAL_V300R001_GetCharacter(VOID);
VOID VDMHAL_V300R001_WriteBigTitle1DYuv( FILE *fpYuv, UINT8 *Yaddress, UINT32 Width, UINT32 Height, UINT32 chroma_idc);
#ifdef __cplusplus
}
#endif


#endif

