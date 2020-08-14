#ifndef _TEST_SUB_DISP_H_
#define _TEST_SUB_DISP_H_

#include "sme_media_type.h"
#include "sme_player_type.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef V_VOID* SUB_DISP_HDL;

SUB_DISP_HDL Sub_Disp_Init(ST_SME_DISP_RECT stRect);
V_VOID Sub_Disp_DeInit(SUB_DISP_HDL hdlSubDisp);

V_UINT32 Sub_DispCb(IN V_VOID* pvCtx,  IN ST_SUB_INFO stSubInfo);
V_UINT32 Sub_ClearCb(IN V_VOID* pvCtx,  IN V_UINT64 u64DataId);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
