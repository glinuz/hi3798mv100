
#include "vpss_cbb_alg.h"
#include "vpss_cbb_base.h"

HI_S32 VPSS_CBB_FRAME_SetConfig(CBB_REG_ADDR_S stRegAddr,
								CBB_FRAME_CHANNEL_E enFrameChan,
								HI_VOID* pArg1,
								HI_VOID* pArg2);
								
HI_S32 VPSS_CBB_INFO_SetConfig(CBB_REG_ADDR_S stRegAddr,
								CBB_INFO_CHANNEL_E enInfoChan,
								HI_VOID* pArg1,
								HI_VOID* pArg2);
								
HI_S32 VPSS_CBB_INFO_GetInfo(CBB_REG_ADDR_S stRegAddr,
								CBB_CALC_INFO_E enInfoChan,
								HI_VOID* pArg1,
								HI_VOID* pArg2);
								
HI_S32 VPSS_CBB_FUNC_SetConfig(CBB_REG_ADDR_S stRegAddr,
								CBB_FUNC_E enFunction,
								HI_VOID* pArg1,
								HI_VOID* pArg2);

HI_S32 VPSS_CBB_ALG_SetConfig(CBB_REG_ADDR_S stRegAddr,
								CBB_ALG_E enAlg,
								HI_VOID* pArg1,
								HI_VOID* pArg2);

HI_S32 VPSS_CBB_BASE_Operating(CBB_REG_ADDR_S stRegAddr,
								CBB_BASEOPT_E enAlg,
								HI_VOID* pArg1,
								HI_VOID* pArg2);
