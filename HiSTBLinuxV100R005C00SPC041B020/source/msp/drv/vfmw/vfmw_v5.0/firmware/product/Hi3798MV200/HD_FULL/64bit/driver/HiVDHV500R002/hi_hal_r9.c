/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCWefhnWud4XlKRRrKQQuEMP2npPcbDID7w6e/x
XMU7b/bBYUlZ9ZeEAd2+IbqhywCB0VBROsZb7GPGY37VOKjDbC7YufZhCSeXmvjCznpWYTWP
DovptwXgU3I8smdQdcYVt2zi/peCgxjZy7C/N4G1S6RZ3y1j8kAANzSwmhX46w==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/



















/***********************************************************************
*
* Copyright (c) 2010 Hisilicon - All Rights Reserved
*
* File: $vdm_hal_real9.c$
* Date: $2010/06/09$
* Revision: $v1.0$
* Purpose: HiVDHV100 IP REAL9 VDM HAL Module.
*
* Change History:
*
* Date       Author            Change
* ======     ======            ======
* 20100609   q45134            Original
*
************************************************************************/

#ifndef __VDM_HAL_REAL9_C__
#define __VDM_HAL_REAL9_C__

#include "vdm_hal.h"
#include "decparam.h"

VOID RV9HAL_V400R003_WriteReg(VDMHAL_HWMEM_S *pHwMem, RV9_DEC_PARAM_S *pRv9DecParam, SINT32 VdhId, VOID *pTask);
VOID RV9HAL_V400R003_WritePicMsg(VDMHAL_HWMEM_S *pHwMem, RV9_DEC_PARAM_S *pRv9DecParam);
VOID RV9HAL_V400R003_WriteSliceMsg(VDMHAL_HWMEM_S *pHwMem, RV9_DEC_PARAM_S *pRv9DecParam);


SINT32 RV9HAL_V400R003_InitHal( VOID )
{
    return VDMHAL_OK;
}


SINT32 RV9HAL_V400R003_StartDec( RV9_DEC_PARAM_S *pRv9DecParam, SINT32 VdhId, VOID *pTask)
{
    return VDMHAL_OK;
}

VOID RV9HAL_V400R003_WriteReg(VDMHAL_HWMEM_S *pHwMem, RV9_DEC_PARAM_S *pRv9DecParam, SINT32 VdhId, VOID *pTask)
{
    return;
}

VOID RV9HAL_V400R003_WritePicMsg(VDMHAL_HWMEM_S *pHwMem, RV9_DEC_PARAM_S *pRv9DecParam)
{
    return;
}

VOID RV9HAL_V400R003_WriteSliceMsg(VDMHAL_HWMEM_S *pHwMem, RV9_DEC_PARAM_S *pRv9DecParam)
{
    return;
}

#endif //__VDM_HAL_REAL9_C__
