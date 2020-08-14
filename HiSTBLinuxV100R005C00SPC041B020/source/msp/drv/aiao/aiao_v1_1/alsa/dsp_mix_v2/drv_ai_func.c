#include "hi_drv_proc.h"
#include "hi_drv_dev.h"
#include "hi_drv_mmz.h"

#include "hal_aiao_common.h"
#include "hal_aiao.h"

#include "drv_ai_private.h"
#include "hi_drv_ai.h"
#include "drv_ai_ioctl.h"

#include "drv_ai_func.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

int hi_ai_alsa_update_readptr(int handle, unsigned int* write_pos)
{
    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;
    AI_GetAiaoPortId(handle, &enPort);
    return HAL_AIAO_P_ALSA_UpdateRptr(enPort, NULL, *write_pos);
}

void hi_ai_alsa_query_writepos(int handle, unsigned int* pos)
{
    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;
    AI_GetAiaoPortId(handle, &enPort);
    *pos = HAL_AIAO_P_ALSA_QueryWritePos(enPort);
}

void hi_ai_alsa_query_readpos(int handle, unsigned int* pos)
{
    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;
    AI_GetAiaoPortId(handle, &enPort);
    *pos = HAL_AIAO_P_ALSA_QueryReadPos(enPort);
}

int hi_ai_alsa_flush_buffer(int handle)
{
    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;
    AI_GetAiaoPortId(handle, &enPort);
    return HAL_AIAO_P_ALSA_FLASH(enPort);
}

int hi_ai_alsa_get_proc_func(AIAO_IsrFunc** fun)
{
    return AI_GetIsrFunc(fun);
}

int hi_ai_alsa_get_attr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAiAttr)
{
    return HI_DRV_AI_GetDefaultAttr(enAiPort, pstAiAttr);
}

int hi_ai_alsa_open(AI_DRV_Create_Param_S* pstAiParam, struct file* file)
{
    int ret;

    ret = HI_DRV_AI_Init();
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    ret = HI_DRV_AI_Open(file);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = HI_DRV_AI_Create(pstAiParam, file);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    return ret;
}

int hi_ai_alsa_set_enable(int handle, HI_BOOL bEnable)
{
    return HI_DRV_AI_SetEnable(handle, bEnable);
}

int hi_ai_alsa_close(int handle, struct file* file)
{
    int ret;

    ret = HI_DRV_AI_Destroy(handle);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = HI_DRV_AI_Release(file);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    HI_DRV_AI_DeInit();

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
