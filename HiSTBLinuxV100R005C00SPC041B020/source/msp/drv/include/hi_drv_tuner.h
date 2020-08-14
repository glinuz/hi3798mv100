#ifndef __HI_DRV_TUNER_H__
#define __HI_DRV_TUNER_H__

#include "hi_type.h"
#include "hi_drv_i2c.h"
#include "hi_debug.h"
#include "hi_drv_struct.h"
#include "drv_i2c_ext.h"
#include "hi_unf_frontend.h"
#include "drv_tuner_ioctl.h"


#define HI_FATAL_TUNER(fmt...) \
            HI_FATAL_PRINT(HI_ID_TUNER, fmt)

#define HI_ERR_TUNER(fmt...) \
            HI_ERR_PRINT(HI_ID_TUNER, fmt)

#define HI_WARN_TUNER(fmt...) \
            HI_WARN_PRINT(HI_ID_TUNER, fmt)

#define HI_INFO_TUNER(fmt...) \
            HI_INFO_PRINT(HI_ID_TUNER, fmt)

HI_S32 HI_DRV_TUNER_Init(HI_VOID);
HI_VOID HI_DRV_TUNER_DeInit(HI_VOID);
HI_S32 HI_DRV_TUNER_GetDeftAttr(HI_U32  u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );
HI_S32 HI_DRV_TUNER_SetAttr(HI_U32  u32tunerId , const HI_UNF_TUNER_ATTR_S *pstTunerAttr );
HI_S32 HI_DRV_TUNER_SetSatAttr(HI_U32   u32tunerId , const HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr);
HI_S32 HI_DRV_TUNER_SetCabAttr(HI_U32   u32tunerId , const HI_UNF_TUNER_CAB_ATTR_S *pstCabTunerAttr);
HI_S32 HI_DRV_TUNER_SetTerAttr(HI_U32   u32tunerId , const HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr);
HI_S32 HI_DRV_TUNER_Open (HI_U32    u32tunerId);
HI_S32 HI_DRV_TUNER_Close(HI_U32    u32tunerId);
HI_S32 HI_DRV_TUNER_Connect(HI_U32  u32tunerId , const HI_UNF_TUNER_CONNECT_PARA_S  *pstConnectPara,HI_U32 u32TimeOut);
HI_S32 HI_DRV_TUNER_GetStatus(HI_U32    u32tunerId , HI_UNF_TUNER_STATUS_S  *pstTunerStatus);
HI_S32 HI_DRV_TUNER_GetBer(HI_U32  u32tunerId , HI_U32 *pu32ber);
HI_S32 HI_DRV_TUNER_GetRs(HI_U32  u32tunerId , HI_U32 *pu32Rs);
HI_S32 HI_DRV_TUNER_GetSnr(HI_U32  u32tunerId , HI_U32 *pu32Snr);
HI_S32 HI_DRV_TUNER_GetRssi(HI_U32  u32tunerId , HI_U32 *pu32Rssi);
HI_S32 HI_DRV_TUNER_GetFreqSymbOffset(HI_U32  u32tunerId , HI_U32 *pu32Offset);
HI_S32 HI_DRV_TUNER_SetLnbOut(HI_U32  u32tunerId ,int LnbPower);
HI_S32 HI_DRV_TUNER_SetSwitchFreq(HI_U32 u32SwitchFreq);
HI_S32 HI_DRV_TUNER_GetSwitchFreq(HI_U32 *u32SwitchFreq);
HI_S32 HI_DRV_TUNER_SetLoopthrough(HI_U32  u32tunerId);
HI_S32 HI_DRV_TUNER_CancelLoopthrough(HI_U32  u32tunerId);
HI_S32 HI_DRV_TUNER_SetTsOut(HI_U32 u32tunerId, HI_UNF_TUNER_TSOUT_SET_S *stTsOut);

HI_S32 HI_DRV_TUNER_SetLNBConfig( HI_U32 u32TunerId, const HI_UNF_TUNER_FE_LNB_CONFIG_S *pstLNB);
HI_S32 HI_DRV_TUNER_GetLNBConfig( HI_U32 u32TunerId, HI_UNF_TUNER_FE_LNB_CONFIG_S *pstLNB);
HI_S32 HI_DRV_TUNER_SetLNBPower(HI_U32 u32TunerId, HI_UNF_TUNER_FE_LNB_POWER_E enLNBPower);
HI_S32 HI_DRV_TUNER_GetLNBPower(HI_U32 u32TunerId, HI_UNF_TUNER_FE_LNB_POWER_E *enLNBPower);
HI_S32 HI_DRV_TUNER_Switch22K(HI_U32 u32TunerId, HI_UNF_TUNER_SWITCH_22K_E enPort);
HI_S32 HI_DRV_TUNER_DISEQC_Switch4Port(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_SWITCH4PORT_S* pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_Switch16Port(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_SWITCH16PORT_S* pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_StorePos(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_POSITION_S *pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_GotoPos(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_POSITION_S *pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_SetLimit(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_LIMIT_S* pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_Move(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_MOVE_S* pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_Stop(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);
HI_S32 HI_DRV_TUNER_DISEQC_Recalculate(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_RECALCULATE_S* pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_CalcAngular(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_USALS_PARA_S* pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_GotoAngular(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_USALS_ANGULAR_S* pstPara);
HI_S32 HI_DRV_TUNER_DISEQC_Reset(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);
HI_S32 HI_DRV_TUNER_DISEQC_Standby(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);
HI_S32 HI_DRV_TUNER_DISEQC_WakeUp(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);

#endif

