/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_sr.c
  Version       : Initial Draft
  Author        : 
  Created       : 2014/01/16
  Description   :
  History       :
******************************************************************************/
#include <linux/string.h>      /* memcpy() */
#include <linux/fcntl.h>
#include <linux/fs.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "pq_hal_comm.h"
#include "pq_hal_sr.h"


/* 获取开关SR开关状态 */
HI_VOID PQ_HAL_GetSREnable(HI_U32 u32ChId, HI_BOOL *bOnOff)
{
    S_VDP_REGS_TYPE* pstVdpReg = NULL;
    U_SR_CTRL  SR_CTRL;

    if (u32ChId >= VID_MAX)
    {
        HI_ERR_PQ("select wrong VP layer id\n");
        return;
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    SR_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVdpReg->SR_CTRL.u32) + u32ChId * VDP_VID_OFFSET);
    *bOnOff = SR_CTRL.bits.sr_en;

    return;
}

/* 开关SR模块ZME DEMO模式 */
HI_VOID PQ_HAL_SR_EnZme(HI_U32 u32ChId, SR_ZME_E enZme)
{
    S_VDP_REGS_TYPE* pstVdpReg = NULL;

    U_SR_CTRL  SR_CTRL;

    if (u32ChId >= VID_MAX)
    {
        HI_ERR_PQ("select wrong VP layer id\n");
        return;
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    SR_CTRL.u32 = PQ_HAL_RegRead((HI_U32) & (pstVdpReg->SR_CTRL.u32) + u32ChId * VDP_VID_OFFSET);
    SR_CTRL.bits.sr_disp_state = enZme;
    PQ_HAL_RegWrite(((HI_U32) & (pstVdpReg->SR_CTRL.u32) + u32ChId * VDP_VID_OFFSET), SR_CTRL.u32);

    return;
}

/* SR模块中的Sharpen的模块增强和细节增强 */
HI_VOID PQ_HAL_SetSRSpMode(HI_U32 u32ChId, SR_SHARP_MODE_E eHsh_nodir)
{
    S_VDP_REGS_TYPE* pstVdpReg = NULL;
    U_SR_SHARP_EN   SR_SHARP_EN;

    pstVdpReg = PQ_HAL_GetVdpReg();
    SR_SHARP_EN.u32 = PQ_HAL_RegRead((HI_U32) & (pstVdpReg->SR_SHARP_EN.u32) + u32ChId * VDP_VID_OFFSET);
    SR_SHARP_EN.bits.hsh_nodir  =  eHsh_nodir;
    PQ_HAL_RegWrite(((HI_U32) & (pstVdpReg->SR_SHARP_EN.u32) + u32ChId * VDP_VID_OFFSET), SR_SHARP_EN.u32);

    return;
}

HI_VOID PQ_HAL_GetSRSharpStr(HI_U32 u32ChId, HI_U32 *pu32SharpStr)
{
    S_VDP_REGS_TYPE* pstVdpReg = NULL;
    U_SR_SHARP_THR   SR_SHARP_THR;

    if (u32ChId >= VID_MAX)
    {
        HI_ERR_PQ("select wrong VP layer id\n");
        return;
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    SR_SHARP_THR.u32 = PQ_HAL_RegRead((HI_U32) & (pstVdpReg->SR_SHARP_THR.u32) + u32ChId * VDP_VID_OFFSET);
    *pu32SharpStr = SR_SHARP_THR.bits.sharp_gain;

    return;
}

HI_VOID PQ_HAL_SetSRSharpStr(HI_U32 u32ChId, HI_U32 u32SharpStr)
{
    S_VDP_REGS_TYPE* pstVdpReg = NULL;
    U_SR_SHARP_THR   SR_SHARP_THR;

    if (u32ChId >= VID_MAX)
    {
        HI_ERR_PQ("select wrong VP layer id\n");
        return;
    }

    pstVdpReg = PQ_HAL_GetVdpReg();
    SR_SHARP_THR.u32 = PQ_HAL_RegRead((HI_U32) & (pstVdpReg->SR_SHARP_THR.u32) + u32ChId * VDP_VID_OFFSET);
    SR_SHARP_THR.bits.sharp_gain = u32SharpStr;
    PQ_HAL_RegWrite(((HI_U32) & (pstVdpReg->SR_SHARP_THR.u32) + u32ChId * VDP_VID_OFFSET), SR_SHARP_THR.u32);

    return;
}


