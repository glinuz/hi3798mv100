#include "tee_common.h"
#include "tee_internal_api.h"
#include "tee_mem_mgmt_api.h"
#define TA_DEBUG
#include "tee_log.h"
#include "tee_fs.h"
#include "tee_trusted_storage_api.h"
#include "tee_time_api.h"
#include "hi_tee_mem.h"
#include "sre_syscalls_ext.h"
#include "tee_avplay_utils.h"

HI_S32 TEE_AVPLAY_Ree2TeeMemCopy(TEE_Param stParams[4])
{
    HI_U32 ReeAddr = stParams[0].value.a;
    HI_U32 TeeAddr = stParams[0].value.b;
    HI_U32 DataLen = stParams[1].value.a;

    return __TEE_DRV_AVPLAY_Ree2TeeMemCopy(ReeAddr, TeeAddr, DataLen);
}
