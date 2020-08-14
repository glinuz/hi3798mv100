/******************************************************************************

  Copyright (C), 2014-2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_tee_hdmi.c
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
#include "hi_tee_hdmi.h"
#include "tee_drv_hdmi.h"
#include "tee_log.h"
#include "hi_list.h"

#define CHECK_POINTER(p) if(p == HI_NULL) {\
                            ta_debug("input ptr is null! \n"); \
                            return HI_FAILURE; }

#define HDMI_ERROR_TAG      "[error]"
#define HDMI_INFO_TAG       "[info]"

#define HDMI_ERR(fmt, args...)  uart_printf_func("%s %s: " fmt "\n", HDMI_ERROR_TAG, __FUNCTION__, ## args)
//#define HDMI_INFO(fmt, args...) uart_printf_func("%s %s: " fmt "\n", HDMI_INFO_TAG, __FUNCTION__, ## args)
#define HDMI_INFO               ta_debug

typedef struct
{
    HI_TEE_HDMI_ID_E            enHdmi;
    HI_TEE_HDMI_HDCP_LEVLEL_E   enHdcp;
    List_Head_S                 list;
}HDMI_CONTENT_S;

static List_Head_S      gHDMIContextHead;
static HI_S32           gHdmiListCount = 0;

HI_BOOL CheckHandleIsValid(HI_HANDLE hHDMI)
{
    List_Head_S*                pList           = HI_NULL;
    HDMI_CONTENT_S*             pstHdmiCtx      = HI_NULL;
    HDMI_CONTENT_S*             pHdmiContext    = HI_NULL;
    HI_BOOL                     bIsVaild        = HI_FALSE;

    if(HI_NULL == hHDMI)
    {
        return HI_FALSE;
    }

    pstHdmiCtx = (HDMI_CONTENT_S *)hHDMI;

    HI_List_For_Each(pList, &gHDMIContextHead)
    {
        pHdmiContext = HI_LIST_ENTRY(pList, HDMI_CONTENT_S, list);
        if((HI_NULL != pHdmiContext) && (pHdmiContext == pstHdmiCtx))
        {
            HDMI_INFO("Find hdmi handle %p \n", pHdmiContext);
            bIsVaild = HI_TRUE;
            break;
        }
    }

    return bIsVaild;
}

#define CHECK_HANDLE(hHDMI) \
do { \
    if(HI_TRUE != CheckHandleIsValid(hHDMI)) { \
        HDMI_ERR("input HDMI handle is invalid!"); \
        return HI_FAILURE; \
    } \
}while(0)


HI_S32 HI_TEE_HDMI_CreateHandle(HI_HANDLE* phHDMI, HI_TEE_HDMI_ID_E enHdmi)
{
    HDMI_CONTENT_S* pstHdmiCtx = (HDMI_CONTENT_S*)TEE_Malloc(sizeof(HDMI_CONTENT_S), 0);
    if(NULL == pstHdmiCtx)
    {
        HDMI_ERR("malloc fail");
        return HI_FAILURE;
    }
    HDMI_INFO("Create handle in...\n");

    if(0 == gHdmiListCount)
    {
        HDMI_INFO("HI_LIST_HEAD_INIT_PTR");
        HI_LIST_HEAD_INIT_PTR(&gHDMIContextHead);
    }

    if(HI_TEE_HDMI_ID_BUTT <= enHdmi)
    {
        HDMI_ERR("input HDMI ID error!");
        phHDMI = HI_NULL;
        return HI_FAILURE;
    }
    pstHdmiCtx->enHdmi = enHdmi;
    pstHdmiCtx->enHdcp = 0;

    HI_List_Add(&pstHdmiCtx->list, &gHDMIContextHead);
    gHdmiListCount++;
    *phHDMI = pstHdmiCtx;

    HDMI_INFO("HI_List_Add %p, gHdmiListCount:%d\n", pstHdmiCtx, gHdmiListCount);
    HDMI_INFO("Create handle out...\n");

    return HI_SUCCESS;
}

HI_S32 HI_TEE_HDMI_DestoryHandle(HI_HANDLE hHDMI)
{
    HI_S32                      s32Ret              = HI_SUCCESS;
    HI_TEE_HDMI_HDCP_LEVLEL_E   enHdcpLevel         = HI_TEE_HDMI_HDCP_LEVLE_NONE;
    HI_TEE_HDMI_HDCP_LEVLEL_E   enCurHdcpLevel      = HI_TEE_HDMI_HDCP_LEVLE_NONE;
    HI_TEE_HDMI_ID_E            enHdmi              = HI_TEE_HDMI_ID_BUTT;
    List_Head_S*                pList               = HI_NULL;
    HDMI_CONTENT_S*             pHdmiContext        = HI_NULL;

    HDMI_INFO("Destory handle in...\n");
    CHECK_HANDLE(hHDMI);

    HDMI_CONTENT_S* pstHdmiCtx = (HDMI_CONTENT_S *)hHDMI;
    enHdmi = pstHdmiCtx->enHdmi;

    s32Ret = __HI_DRV_HDMI_GetHDCPStrategy(enHdmi, &enCurHdcpLevel);
    if(HI_SUCCESS != s32Ret)
    {
        HDMI_ERR("get hdcp strategy failed! \n");
        return HI_FAILURE;
    }

    HDMI_INFO("enCurHdcpLevel:%d \n", enCurHdcpLevel);
    if(HI_SUCCESS != s32Ret)
    {
        HDMI_ERR("HI_TEE_MON_GetHDCPStrategy fail: ret:%d", s32Ret);
        return s32Ret;
    }

    HI_List_Del(&pstHdmiCtx->list);
    TEE_Free(pstHdmiCtx);
    gHdmiListCount --;
    HDMI_INFO("HI_List_Del, gHdmiListCount:%d\n", gHdmiListCount);

    HI_List_For_Each(pList, &gHDMIContextHead)
    {
        pHdmiContext = HI_LIST_ENTRY(pList, HDMI_CONTENT_S, list);
        HDMI_INFO("HI_List_For_Each, pHdmiContext:%p hHdcp:%d \n", pHdmiContext, pHdmiContext->enHdcp);
        if((pHdmiContext->enHdcp > enHdcpLevel) && (enHdmi == pHdmiContext->enHdmi))
        {
            enHdcpLevel = pHdmiContext->enHdcp;
        }
    }

    if(enHdcpLevel > enCurHdcpLevel)
    {
        HDMI_INFO("set hdcplevel:%d \n", enHdcpLevel);
        s32Ret = __HI_DRV_HDMI_SetHDCPStrategy(enHdmi, enHdcpLevel);
        if(HI_SUCCESS != s32Ret)
        {
            HDMI_ERR("__HI_DRV_HDMI_GetHDCPStrategy fail: ret:%d \n", s32Ret);
            return s32Ret;
        }
    }

    HDMI_INFO("Destory handle out...\n");

    return s32Ret;
}

HI_S32 HI_TEE_HDMI_GetHDCPStatus(HI_HANDLE hHDMI, HI_TEE_HDMI_HDCP_STATUS_E *penHdcpStatus)
{
    HI_DRV_HDMI_DEVICE_ID_E     enHdmiID            = HI_DRV_HDMI_DEVICE_BUTT;
    HI_TEE_HDMI_HDCP_STATUS_E   enHdcpStatus        = HI_DRV_HDMI_HDCP_NONE;
    HI_S32                      s32Ret              = HI_SUCCESS;
    HDMI_CONTENT_S*             pstHdmiCtx          = HI_NULL;
    
	HDMI_INFO("get hdcp status in...\n");
    CHECK_POINTER(penHdcpStatus);
    CHECK_HANDLE(hHDMI);
    

    pstHdmiCtx = (HDMI_CONTENT_S *)hHDMI;
    enHdmiID = pstHdmiCtx->enHdmi;

    s32Ret = __HI_DRV_HDMI_GetHDCPStatus(enHdmiID, &enHdcpStatus);
    if(HI_SUCCESS != s32Ret)
    {
        HDMI_ERR("__HI_DRV_HDMI_GetHDCPStatus fail: ret:%d \n", s32Ret);
        return HI_FAILURE;
    }

    *penHdcpStatus = enHdcpStatus;
    HDMI_INFO("get hdcp status out...\n");

    return s32Ret;
}

HI_S32 HI_TEE_HDMI_SetHDCPStrategy(HI_HANDLE hHDMI, HI_TEE_HDMI_HDCP_LEVLEL_E enHdcp)
{
    HI_DRV_HDMI_DEVICE_ID_E     enHdmiID;
    HI_DRV_HDMI_HDCP_LEVEL_E    enDrvHdcp;
    HI_S32                      s32Ret          = HI_SUCCESS;
    HI_TEE_HDMI_HDCP_LEVLEL_E   enCurHdcpLevel  = HI_TEE_HDMI_HDCP_LEVLE_NONE;
    List_Head_S*                pList;
    HDMI_CONTENT_S*             pstHdmiCtx      = HI_NULL;

    HDMI_INFO("set hdcp status in...\n");
    CHECK_HANDLE(hHDMI);
    
    pstHdmiCtx = (HDMI_CONTENT_S *) hHDMI;
    pstHdmiCtx->enHdcp = enHdcp;

    s32Ret = __HI_DRV_HDMI_GetHDCPStrategy(pstHdmiCtx->enHdmi, &enCurHdcpLevel);
    if(HI_SUCCESS != s32Ret)
    {
        HDMI_ERR("__HI_DRV_HDMI_GetHDCPStrategy fail: s32Ret:%d", s32Ret);
        return s32Ret;
    }

    HDMI_INFO("set HDCP enHdcp:%d (enCurHdcpLevel:%d) \n", enHdcp, enCurHdcpLevel);
    s32Ret = __HI_DRV_HDMI_SetHDCPStrategy(pstHdmiCtx->enHdmi, pstHdmiCtx->enHdcp);
    if(HI_SUCCESS != s32Ret)
    {
        HDMI_ERR("__HI_DRV_HDMI_SetHDCPStrategy fail: s32Ret:%d", s32Ret);
        return s32Ret;
    }

    HDMI_INFO("set hdcp status out...\n");

    return s32Ret;
}

HI_S32 HI_TEE_HDMI_GetHDCPStrategy(HI_HANDLE hHDMI, HI_TEE_HDMI_HDCP_LEVLEL_E *penHdcp)
{
    HI_S32                      s32Ret          = HI_SUCCESS;
    HDMI_CONTENT_S*             pstHdmiCtx      = HI_NULL;
    HI_DRV_HDMI_HDCP_LEVEL_E    enHdcpLevel     = HI_DRV_HDMI_HDCP_LEVEL_NONE;

    HDMI_INFO("get hdcp strategy in...\n");
	CHECK_POINTER(penHdcp);
    CHECK_HANDLE(hHDMI);
    
    pstHdmiCtx = (HDMI_CONTENT_S*) hHDMI;

    s32Ret = __HI_DRV_HDMI_GetHDCPStrategy(pstHdmiCtx->enHdmi, &enHdcpLevel);
    if(HI_SUCCESS != s32Ret)
    {
        HDMI_ERR("__HI_DRV_HDMI_GetHDCPStrategy fail: s32Ret:%d", s32Ret);
        return HI_FAILURE;
    }

    *penHdcp = enHdcpLevel;
    HDMI_INFO("get hdcp strategy out...\n");

    return s32Ret;
}

HI_S32 HI_TEE_HDMI_GetHDCPCaps(HI_HANDLE hHDMI, HI_TEE_HDMI_HDCP_CAPS_S *pstHdcpCaps)
{
    HI_DRV_HDMI_DEVICE_ID_E     enHdmiID;
    HI_DRV_HDMI_HDCP_CAPS_S     stHdcpCaps;
    HI_S32                      s32Ret          = HI_SUCCESS;
    HDMI_CONTENT_S*             pstHdmiCtx      = HI_NULL;

    HDMI_INFO("get hdcp caps in...\n");
    CHECK_POINTER(pstHdcpCaps);
    CHECK_HANDLE(hHDMI);
    
    pstHdmiCtx = (HDMI_CONTENT_S*) hHDMI;

    enHdmiID = pstHdmiCtx->enHdmi;
	memset(&stHdcpCaps, 0, sizeof(HI_DRV_HDMI_HDCP_CAPS_S));

    s32Ret = __HI_DRV_HDMI_GetHDCPCaps(enHdmiID, &stHdcpCaps);
    if(HI_SUCCESS != s32Ret)
    {
        HDMI_ERR("__HI_DRV_HDMI_GetHDCPCaps fail: s32Ret:%d", s32Ret);
        return HI_FAILURE;
    }

    pstHdcpCaps->bHdcp14Support = stHdcpCaps.bHdcp14Support;
    pstHdcpCaps->bHdcp22Support = stHdcpCaps.bHdcp22Support;
    HDMI_INFO("get hdcp caps out...\n");

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

