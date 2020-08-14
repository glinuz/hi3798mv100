
#include "drv_hdmi_intf.h"
#include "drv_hdmi_tee.h"

#define CHECK_POINTER(p) \
do{                      \
    if(HI_NULL == p){\
        HDMI_ERR("The pointer is null\n");       \
        return HI_FAILURE;\
    } \
}while(0) 

#define TEE_HDMI_TA_NAME        "hisi_drv_hdmi"
#define TEE_HDMI_TA_UID         0

/* HDMI CA test */
TEEC_UUID HdmiTaskUUID =
{
    0x08080601, \
    0x0100, \
    0x0000, \
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}
};

HI_S32 DRV_HDMI_TeeOpen(HDMI_DEVICE_ID_E enHdmiId)
{
    TEEC_Result     teec_result;
    HI_S32          s32Ret = HI_FAILURE; 
    HI_U32          u32RootId = 0;
    HDMI_DEVICE_S   *pstHdmiDev = HI_NULL;
    HDMI_TEE_INFO_S *pstTeeInfo = HI_NULL;

    pstHdmiDev = GetHdmiDevice(enHdmiId);
    CHECK_POINTER(pstHdmiDev);
    pstTeeInfo = &pstHdmiDev->stTeeInfo;
    
    if(pstTeeInfo->bTeeOpen == HI_TRUE)
    {
        HDMI_INFO("Tee task was opened. \n");
        return HI_SUCCESS;
    }

    memset(&(pstTeeInfo->stTeecContext), 0, sizeof(pstTeeInfo->stTeecContext));
    memset(&(pstTeeInfo->stTeecOperation), 0, sizeof(pstTeeInfo->stTeecOperation));
    memset(&(pstTeeInfo->stTeecSession), 0, sizeof(pstTeeInfo->stTeecSession));
    
    teec_result = TEEK_InitializeContext("task_stb_sec_hdmi", &(pstTeeInfo->stTeecContext));
    if(teec_result != TEE_SUCCESS)
    {
        HDMI_ERR("TEEK_Session init failed: 0x%x\n", teec_result);
        return HI_FAILURE;
    }

    pstTeeInfo->stTeecOperation.started = 1;
    pstTeeInfo->stTeecOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
    pstTeeInfo->stTeecOperation.params[2].tmpref.buffer = (HI_VOID *)(&u32RootId);
    pstTeeInfo->stTeecOperation.params[2].tmpref.size = sizeof(u32RootId);
    pstTeeInfo->stTeecOperation.params[3].tmpref.buffer = (HI_VOID *)(TEE_HDMI_TA_NAME);
    pstTeeInfo->stTeecOperation.params[3].tmpref.size = strlen(TEE_HDMI_TA_NAME) + 1;
    
    teec_result = TEEK_OpenSession( &(pstTeeInfo->stTeecContext),
    				                &(pstTeeInfo->stTeecSession),
    				                &HdmiTaskUUID,
    				                TEEC_LOGIN_IDENTIFY,
    				                HI_NULL,
    				                &(pstTeeInfo->stTeecOperation),
    				                HI_NULL);

    if(teec_result == TEE_SUCCESS)
    {
        pstTeeInfo->bTeeOpen = HI_TRUE;
        s32Ret = HI_SUCCESS;
    }
    else
    {
        pstTeeInfo->bTeeOpen = HI_FALSE;
        TEEK_FinalizeContext(&(pstTeeInfo->stTeecContext));
        HDMI_ERR("TEEK_Session open failed 0x%x\n", teec_result);
        s32Ret = HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 DRV_HDMI_TeeCmdSend(HDMI_DEVICE_ID_E enHdmiId, HDMI_TEE_PARAM_S *pstParam)
{
    HI_S32          s32Ret = HI_SUCCESS; 
    TEEC_Result     teec_result = TEE_SUCCESS;
    HDMI_DEVICE_S   *pstHdmiDev = HI_NULL;
    HDMI_TEE_INFO_S *pstTeeInfo = HI_NULL;

    pstHdmiDev = GetHdmiDevice(enHdmiId);
    CHECK_POINTER(pstHdmiDev);
    pstTeeInfo = &pstHdmiDev->stTeeInfo;
    CHECK_POINTER(pstParam);
    
    if(   (pstParam->enTeeCmd >= HI_HDMI_COMM_BUTT) 
       || (pstParam->enTeeCmd < HI_HDMI_HDCP14_INIT) 
       || (pstTeeInfo->bTeeOpen != HI_TRUE))
    {
        HDMI_ERR("Tee command(%d) is invailed or tee is not open(%d). \n", pstParam->enTeeCmd, pstTeeInfo->bTeeOpen);
        return HI_FAILURE;
    }

    pstTeeInfo->stTeecOperation.started = 1;
    switch(pstParam->enTeeCmd)
    {
        
        case HI_HDMI_HDCP_CAPS:
            pstTeeInfo->stTeecOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);

            pstTeeInfo->stTeecOperation.params[0].value.a = pstParam->seHdcpCaps.bHdcp14Support;
            pstTeeInfo->stTeecOperation.params[0].value.b = pstParam->seHdcpCaps.bHdcp22Support;
            break;
        case HI_HDMI_TEST_SET_HDCP_STRATEGY:
            pstTeeInfo->stTeecOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
            pstTeeInfo->stTeecOperation.params[0].value.a = pstParam->u32TestParam;
            break;
            
        case HI_HDMI_HDCP14_INIT ... HI_HDMI_HDCP14_EVENT :
            pstTeeInfo->stTeecOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE);
            memcpy(pstTeeInfo->au8TeeDataBuf,(HI_VOID *)&pstParam->enTeeCmd,sizeof(HI_U32));
            memcpy(pstTeeInfo->au8TeeDataBuf+sizeof(HI_U32),pstParam->pvData,pstParam->u32Size);
            pstTeeInfo->u32DataSize = pstParam->u32Size + sizeof(HI_U32);
            pstTeeInfo->stTeecOperation.params[1].tmpref.buffer = pstTeeInfo->au8TeeDataBuf;
            pstTeeInfo->stTeecOperation.params[1].tmpref.size   = pstTeeInfo->u32DataSize;
            break;
            
        default:
            pstTeeInfo->stTeecOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_NONE, TEEC_NONE);
            break;
            
    }
           
    teec_result = TEEK_InvokeCommand(&(pstTeeInfo->stTeecSession),
                                    pstParam->enTeeCmd,
                                    &(pstTeeInfo->stTeecOperation),
                                    NULL);
    if(teec_result != TEE_SUCCESS)
    {
        HDMI_ERR("TEE Command send error: 0x%x \n", teec_result);
    }

    s32Ret = (teec_result == TEE_SUCCESS) ? HI_SUCCESS : HI_FAILURE;

    HDMI_INFO("send command to tee:%d\n", pstParam->enTeeCmd);
    
    return s32Ret;
}

HI_S32 DRV_HDMI_TeeClose(HDMI_DEVICE_ID_E enHdmiId)
{
    HDMI_DEVICE_S   *pstHdmiDev = HI_NULL;
    HDMI_TEE_INFO_S *pstTeeInfo = HI_NULL;

    pstHdmiDev = GetHdmiDevice(enHdmiId);
    CHECK_POINTER(pstHdmiDev);
    pstTeeInfo = &pstHdmiDev->stTeeInfo;

    if(pstTeeInfo->bTeeOpen != HI_TRUE)
    {
        HDMI_INFO("Tee was not open! \n");
        return HI_SUCCESS;
    }

    TEEK_CloseSession(&(pstTeeInfo->stTeecSession));
    TEEK_FinalizeContext(&(pstTeeInfo->stTeecContext));

    pstTeeInfo->bTeeOpen = HI_FALSE;
    
    return HI_SUCCESS;
}





