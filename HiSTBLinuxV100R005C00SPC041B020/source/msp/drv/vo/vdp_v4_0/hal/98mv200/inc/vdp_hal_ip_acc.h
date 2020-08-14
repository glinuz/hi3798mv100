#ifndef __VDP_HAL_IP_ACC_H__
#define __VDP_HAL_IP_ACC_H__

//#include "c_union_define.h"
#include "hi_reg_common.h"
#include "vdp_define.h"


HI_VOID VDP_VID_SetAccAccEn(HI_U32 u32Data, HI_U32 acc_en);
HI_VOID VDP_VID_SetAccAccMode(HI_U32 u32Data, HI_U32 acc_mode);
HI_VOID VDP_VID_SetAccThdMedLow(HI_U32 u32Data, HI_U32 thd_med_low);
HI_VOID VDP_VID_SetAccThdHigh(HI_U32 u32Data, HI_U32 thd_high);
HI_VOID VDP_VID_SetAccThdLow(HI_U32 u32Data, HI_U32 thd_low);
HI_VOID VDP_VID_SetAccAccRst(HI_U32 u32Data, HI_U32 acc_rst);
HI_VOID VDP_VID_SetAccDbgMode(HI_U32 u32Data, HI_U32 acc_dbg_mode );
HI_VOID VDP_VID_SetAccDbgEn(HI_U32 u32Data, HI_U32 acc_dbg_en);
HI_VOID VDP_VID_SetAccAccMultiple(HI_U32 u32Data, HI_U32 acc_multiple);
HI_VOID VDP_VID_SetAccThdMedHigh(HI_U32 u32Data, HI_U32 thd_med_high);

HI_VOID VDP_VID_SetAccTab(HI_U32 u32Data, HI_U32 *upTable);


HI_U32 VDP_VID_SetAccReadCnt3Low(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt3Med(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt3High(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8MedLow(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8MedHigh(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadAccPixTotal(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8Hist0(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8Hist1(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8Hist2(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8Hist3(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8Hist4(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8Hist5(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8Hist6(HI_U32 u32Data);
HI_U32 VDP_VID_SetAccReadCnt8Hist7(HI_U32 u32Data);

#endif


