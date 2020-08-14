/******************************************************************************

  Copyright (C), 2001-2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : tee_hdmi.c
  Version       : Initial Draft
  Author        : q00352704
  Created       : 2016/1/7
  Last Modified :
  Description   : TEE HDMI unf and mpi interface

******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
    {
#endif
#endif

#include "sre_syscalls_chip.h"
#include "tee_hdmi.h"
#include "tee_drv_hdmi.h"
#include "tee_log.h"

#define HDMI_CHECK_PTR(ptr)   \
do { \
        if (HI_NULL == ptr) { \
            return HI_FAILURE;\
        }\
}while(0)


HI_S32 TEE_MPI_HDMI_HDCP22_Init(HI_TEE_HDMI_ID_E enHdmi)
{
    HI_DRV_HDMI_DEVICE_ID_E enHdmiID;
    HI_S32 s32Ret = HI_SUCCESS;
    ta_debug("tee hdcp2.2 init in. \n");
    enHdmiID = enHdmi;
    s32Ret = __HI_DRV_HDMI_HDCP22_Init(enHdmiID);
    return s32Ret;
}

HI_S32 TEE_MPI_HDMI_HDCP14_Init(HI_TEE_HDMI_ID_E enHdmi)
{

    return HI_SUCCESS;                                                                                                                      
}

HI_S32 TEE_MPI_HDMI_TEEProc(HI_TEE_HDMI_ID_E enHdmi)
{
    __HI_DRV_HDMI_TeeProc(enHdmi);
    return HI_SUCCESS;
}

HI_S32 TEE_MPI_HDMI_SetHDCPCaps(HI_TEE_HDMI_ID_E enHdmi, HI_TEE_HDMI_HDCP_CAPS_S *pstHdcpCaps)
{
    HI_DRV_HDMI_HDCP_CAPS_S stDrvHdcpCaps;
    HI_DRV_HDMI_DEVICE_ID_E enHdmiID;
    HI_S32 s32Ret = HI_SUCCESS;
    if(HI_NULL == pstHdcpCaps)
    {
        return HI_FAILURE;
    }

    ta_debug("tee hdcp caps set:%d,%d. \n", pstHdcpCaps->bHdcp14Support, pstHdcpCaps->bHdcp22Support);
    
    enHdmiID = enHdmi;

    memcpy(&stDrvHdcpCaps, pstHdcpCaps, sizeof(HI_DRV_HDMI_HDCP_CAPS_S));
    s32Ret = __HI_DRV_HDMI_SetHDCPCaps(enHdmiID, &stDrvHdcpCaps);
    
    return s32Ret;
}

HI_S32 TEE_MPI_HDMI_IOCTRL(HI_TEE_HDMI_ID_E enHdmi, HI_VOID *pvData,HI_U32 u32Size)
{
    HI_DRV_HDMI_DEVICE_ID_E enHdmiID;
    HI_S32 s32Ret = HI_SUCCESS;

    enHdmiID = enHdmi;
    s32Ret = __HI_DRV_HDMI_IOCTRL(enHdmiID, pvData,u32Size);
    
    return s32Ret;
}


HI_S32 TEE_MPI_HDMI_Init(HI_TEE_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = __HI_DRV_HDMI_Init(enHdmi);

    return s32Ret;
}

HI_S32 TEE_MPI_HDMI_DeInit(HI_TEE_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = __HI_DRV_HDMI_DeInit(enHdmi);

    return s32Ret;
}

HI_S32 TEE_MPI_HDMI_Start(HI_TEE_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = __HI_DRV_HDMI_Start(enHdmi);

    return s32Ret;
}

HI_S32 TEE_MPI_HDMI_Stop(HI_TEE_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = __HI_DRV_HDMI_Stop(enHdmi);

    return s32Ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

