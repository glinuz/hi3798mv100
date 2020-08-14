#include <errno.h>
#include <stdlib.h>
#include "hi_type.h"
#include "hi_debug.h"
#include "pvr_debug.h"
#include "hi_pvr_debug.h"
#include "hi_pvr_tee.h"
#include "tee_client_api.h"
#include "tee_client_id.h"

#define HI_TEE_PVR_USE_SMMU

#define TEE_PVR_OPEN_TEE_CHANNEL    0
#define TEE_PVR_CLOSE_TEE_CHANNEL   1
#define TEE_PVR_PROCESS_TS_DATA     2
#define TEE_PVR_GET_TEE_STATE       4
#define TEE_PVR_COPY_TO_REE_TEST    8
#define TEE_PVR_CA_GET_ADDRINFO     16
#define TEE_PVR_COPY_FROM_REE       32


#define PVR_TEE_CHECK_INIT_RETURN()\
    do{\
        if (0 == s_stTeeCtrlInfo.s32InitCount)\
        {\
            HI_ERR_PVR("Please call HI_PVR_TeeInit before!\n");\
            return HI_FAILURE;\
        }\
    }while(0)
#define PVR_TEE_ADJUST_BUFFER_LEN(s32Length)\
    do{\
        if (REE_PVR_BUFFER_MIN_LEN > (s32Length))\
        {\
            (s32Length) = REE_PVR_BUFFER_MIN_LEN;\
        }\
        if (REE_PVR_BUFFER_MAX_LEN < (s32Length))\
        {\
            (s32Length) = REE_PVR_BUFFER_MAX_LEN;\
        }\
        if (0 != (s32Length)%REE_PVR_BUFFER_ALIGN)\
        {\
            (s32Length) = (s32Length)/REE_PVR_BUFFER_ALIGN * REE_PVR_BUFFER_ALIGN + REE_PVR_BUFFER_ALIGN;\
        }\
    }while(0)
/************************************************************************/    
typedef struct hiPVR_TEE_REC_DARA_S
{
    HI_U32  u32VirtulAddr;  //reserved, no use;    
    HI_U32  u32PhyAddr;
    HI_U32  u32Len;         
}PVR_TEE_REC_DARA_S;
typedef struct hiPVR_TEE_CTRL_INFO_S
{
    HI_S32 s32InitCount;
    TEEC_Context stContext;
    TEEC_Session stSession;
}PVR_TEE_CTRL_INFO_S;
typedef struct hiPVR_TEE_INDEX_INFO_S
{
    HI_S32 s32IndexNum;
    HI_S32 s32TsDataNum;
    HI_U64 u64DropLength;
    HI_U64 u64GlobalOffset[REE_PVR_MAX_INDEX_NUM];
    HI_U32 u32FrameSize[REE_PVR_MAX_INDEX_NUM];
    PVR_TEE_REC_DARA_S stRecData[2]; 
}PVR_TEE_INDEX_INFO_S;

static const TEEC_UUID PVR_uuid =
{ 
    0x11111111, 
    0x1111, 
    0x1111, 
    { 
        0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 
    } 
};

static PVR_TEE_CTRL_INFO_S s_stTeeCtrlInfo = 
{
    .s32InitCount = 0,
};
/************************************************************************/
static HI_S32 PVR_TeeMallocSmmuBuffer(HI_U32 *pu32SmmuAddr, HI_S32 s32Length, HI_CHAR *pchName);
static HI_S32 PVR_TeeFreeSmmuBuffer(HI_U32 u32SmmuAddr);
/*************************************************************************/

static HI_S32 PVR_TeeMallocSmmuBuffer(HI_U32 *pu32SmmuAddr, HI_S32 s32Length, HI_CHAR *pchName)
{
    HI_PVR_DEBUG_ENTER();
    if (HI_NULL == pu32SmmuAddr)
    {
        HI_ERR_PVR("null pointer for pstSecBuf!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    PVR_TEE_ADJUST_BUFFER_LEN(s32Length);
    
    *pu32SmmuAddr = HI_MPI_SMMU_New(pchName, s32Length);
    if (0 == *pu32SmmuAddr)
    {
        HI_ERR_PVR("calling HI_MPI_SMMU_New failed!!\n");
        return HI_ERR_PVR_NO_MEM;
    }
    HI_INFO_PVR("success to malloc mem(0x%08x, %d)", *pu32SmmuAddr, s32Length);
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}
static HI_S32 PVR_TeeFreeSmmuBuffer(HI_U32 u32SmmuAddr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_PVR_DEBUG_ENTER();
    if (0 == u32SmmuAddr)
    {
        HI_WARN_PVR("null pointer for input argument!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    s32Ret = HI_MPI_SMMU_Delete(u32SmmuAddr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("failed to calling HI_MPI_SMMU_Delete(0x%08x)!\n", u32SmmuAddr);
        return s32Ret;
    }
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}


HI_S32 HI_PVR_TeeInit(HI_VOID)
{
    TEEC_Result stResult;
    TEEC_Operation stOperation;
    HI_PVR_DEBUG_ENTER();
    s_stTeeCtrlInfo.s32InitCount++;
    if (1 == s_stTeeCtrlInfo.s32InitCount)
    {
        memset(&s_stTeeCtrlInfo.stContext, 0x00, sizeof(s_stTeeCtrlInfo.stContext));
        memset(&s_stTeeCtrlInfo.stSession, 0x00, sizeof(s_stTeeCtrlInfo.stSession));
        
        stResult = TEEC_InitializeContext(NULL, &s_stTeeCtrlInfo.stContext);
        if(TEEC_SUCCESS != stResult) 
        {
            s_stTeeCtrlInfo.s32InitCount--;
            HI_ERR_PVR("TEEC init failed,ret = 0x%08x\n", (HI_U32)stResult);
            return (HI_S32)stResult;
        }
        memset(&stOperation, 0x0, sizeof(TEEC_Operation));
        stOperation.started = 1;
        stOperation.cancel_flag = 0;
        stOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
        stResult = TEEC_OpenSession(&s_stTeeCtrlInfo.stContext, 
                                    &s_stTeeCtrlInfo.stSession, 
                                    &PVR_uuid,
                                    TEEC_LOGIN_IDENTIFY, 
                                    NULL, 
                                    &stOperation, 
                                    NULL);
        if(TEEC_SUCCESS != stResult) 
        {
            s_stTeeCtrlInfo.s32InitCount--;
            TEEC_FinalizeContext(&s_stTeeCtrlInfo.stContext);
            HI_ERR_PVR("TEEC openSession failed,ret = 0x%08x\n", (HI_U32)stResult);
            return (HI_S32)stResult;
        }
        HI_INFO_PVR("TEEC Init success!\n");
    }
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}
HI_S32 HI_PVR_TeeDeInit(HI_VOID)
{
    HI_PVR_DEBUG_ENTER();
    
    if (0 >= s_stTeeCtrlInfo.s32InitCount)
    {
        HI_INFO_PVR("Not Init the PVR Tee!\n");
        return HI_SUCCESS;
    }
    s_stTeeCtrlInfo.s32InitCount--;
    if (0 == s_stTeeCtrlInfo.s32InitCount)
    {
        TEEC_CloseSession(&s_stTeeCtrlInfo.stSession);
        TEEC_FinalizeContext(&s_stTeeCtrlInfo.stContext);
        HI_INFO_PVR("TEEC deinit success!\n");
    }
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}


HI_S32 HI_PVR_TeeOpenTeeChn(HI_U32 *pu32ChnId, HI_S32 s32Length)
{
    TEEC_Operation stCreateOperation;
    TEEC_Result stResult;
    HI_U32 u32Origin;
    
    HI_PVR_DEBUG_ENTER();
    
    if (HI_NULL == pu32ChnId)
    {
        HI_ERR_PVR("null pointer for input argument!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    PVR_TEE_CHECK_INIT_RETURN();
    PVR_TEE_ADJUST_BUFFER_LEN(s32Length);
    memset(&stCreateOperation, 0x00, sizeof(stCreateOperation));
    stCreateOperation.started = 1;
    stCreateOperation.params[0].value.a = 0;
    stCreateOperation.params[0].value.b = s32Length;

    stCreateOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
                                            TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_NONE);
    stResult = TEEC_InvokeCommand(&s_stTeeCtrlInfo.stSession, 
                                  TEE_PVR_OPEN_TEE_CHANNEL, 
                                  &stCreateOperation, 
                                  &u32Origin);
    if (TEEC_SUCCESS != stResult) 
    {
        HI_ERR_PVR("[%s_%d]Invoke failed, codes=0x%x, origin=0x%x\n", __FILE__, __LINE__, stResult, u32Origin);
        return (HI_S32)stResult;
    }
    *pu32ChnId = stCreateOperation.params[0].value.a;
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}
HI_S32 HI_PVR_TeeCloseTeeChn(HI_U32 u32TeeChnId)
{
    TEEC_Operation stFreeOperation;
    TEEC_Result stResult;
    HI_U32 u32Origin;
    
    HI_PVR_DEBUG_ENTER();
    PVR_TEE_CHECK_INIT_RETURN();
    
    memset(&stFreeOperation, 0x00, sizeof(stFreeOperation));
    stFreeOperation.started = 1;
    
    stFreeOperation.params[0].value.a = u32TeeChnId;
    stFreeOperation.params[0].value.b = 0;

    stFreeOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_NONE);

    stResult = TEEC_InvokeCommand(&s_stTeeCtrlInfo.stSession, 
                                  TEE_PVR_CLOSE_TEE_CHANNEL,
                                  &stFreeOperation, 
                                  &u32Origin);
    if (TEEC_SUCCESS != stResult) 
    {
        HI_ERR_PVR("[%s_%d]Invoke failed, codes=0x%x, origin=0x%x\n", __FILE__, __LINE__, stResult, u32Origin);
        return (HI_S32)stResult;
    }
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}
HI_S32 HI_PVR_TeeCopyToREE_Test(PVR_REE_BUFFER_INFO_S *pstReeBuf, HI_U32 u32TeeChnId)
{
    TEEC_Operation stCopyOperation;
    TEEC_Result stResult;
    HI_U32 u32Origin;

    HI_PVR_DEBUG_ENTER();
    if (HI_NULL == pstReeBuf)
    {
        HI_ERR_PVR("null pointer for input argument!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    PVR_TEE_CHECK_INIT_RETURN();
    pstReeBuf->u32Used = 0;
    memset(&stCopyOperation, 0x00, sizeof(stCopyOperation));
    stCopyOperation.started = 1;
    stCopyOperation.params[0].value.a = u32TeeChnId;
    stCopyOperation.params[0].value.b = 0;
    stCopyOperation.params[1].value.a = pstReeBuf->u32SmmuAddr;
    stCopyOperation.params[1].value.b = pstReeBuf->u32Size;
    stCopyOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
                                            TEEC_VALUE_INOUT,
                                            TEEC_NONE,
                                            TEEC_NONE);
    stResult = TEEC_InvokeCommand(&s_stTeeCtrlInfo.stSession, 
                                  TEE_PVR_COPY_TO_REE_TEST, 
                                  &stCopyOperation, 
                                  &u32Origin);
    if (TEEC_SUCCESS != stResult) 
    {
        HI_ERR_PVR("Invoke failed, codes=0x%x, origin=0x%x\n", stResult, u32Origin);
        return (HI_S32)stResult;
    }
    pstReeBuf->u32Used = stCopyOperation.params[1].value.b;
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_PVR_TeeGetReeBuffInfo(PVR_REE_BUFFER_INFO_S *pstReeBuf, 
                                HI_U8 **ppu8DataAddr, HI_U32 *pu32Len)
{

    HI_PVR_DEBUG_ENTER();
    if ((HI_NULL == pstReeBuf)
        || (HI_NULL == ppu8DataAddr)
        || (HI_NULL == pu32Len))
    {
        HI_ERR_PVR("null pointer for input argument!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    PVR_TEE_CHECK_INIT_RETURN();
    if (0 == pstReeBuf->u32Used)
    {
        return HI_TEE_ERROR_PVR_NO_ENOUGH_DATA;
    }
    *ppu8DataAddr = pstReeBuf->pUserAddr;
    *pu32Len = pstReeBuf->u32Used;
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}

/*填充一帧数据起始位置，保证188对齐，同步修改ts数据和index数据*/
HI_S32 HI_PVR_TeeAdjustTsData(HI_MPI_DMX_REC_DATA_INDEX_S *pstDmxRecData, 
                              HI_U32 u32TeeChnId, 
                              HI_U64 u64DropLength,
                              PVR_REE_BUFFER_INFO_S *pstReeBuff)
{
    TEEC_Operation stAdjustOperation;
    TEEC_Result stResult;
    HI_U32 u32Origin;
    PVR_TEE_INDEX_INFO_S *pstIndexInfo = HI_NULL;
    HI_S32 i = 0;
    
    HI_PVR_DEBUG_ENTER();
    
    if ((HI_NULL == pstDmxRecData)
        || (HI_NULL == pstReeBuff))
    {
        HI_ERR_PVR("null pointer for input argument!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    PVR_TEE_CHECK_INIT_RETURN();
    pstIndexInfo = (PVR_TEE_INDEX_INFO_S *)pstReeBuff->pUserAddr;
    if (HI_NULL == pstIndexInfo)
    {
        HI_ERR_PVR("no malloc/map the SMMU buffer!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    memset(pstIndexInfo, 0x00, sizeof(PVR_TEE_INDEX_INFO_S));
    pstIndexInfo->s32IndexNum = (HI_S32)pstDmxRecData->u32IdxNum;
    pstIndexInfo->s32TsDataNum = (HI_S32)pstDmxRecData->u32RecDataCnt;
    pstIndexInfo->u64DropLength = u64DropLength;
    if ((REE_PVR_MAX_INDEX_NUM < pstIndexInfo->s32IndexNum)
        || (2 < pstIndexInfo->s32TsDataNum))
    {
        HI_ERR_PVR("Invalid index num(%d) or ts data num(%d)!\n", 
            pstIndexInfo->s32IndexNum, pstIndexInfo->s32TsDataNum);
        return HI_FAILURE;
    }
    for (i = 0; i < pstIndexInfo->s32TsDataNum; i++)
    {
        pstIndexInfo->stRecData[i].u32VirtulAddr = 0;//pstDmxRecData->stRecData[i].pDataAddr;
        pstIndexInfo->stRecData[i].u32PhyAddr = pstDmxRecData->stRecData[i].u32DataPhyAddr;
        pstIndexInfo->stRecData[i].u32Len = pstDmxRecData->stRecData[i].u32Len;
    }
    for (i = 0; i < pstIndexInfo->s32IndexNum; i++)
    {
        pstIndexInfo->u64GlobalOffset[i] = pstDmxRecData->stIndex[i].u64GlobalOffset;
        pstIndexInfo->u32FrameSize[i] = pstDmxRecData->stIndex[i].u32FrameSize;
    }

    memset(&stAdjustOperation, 0x00, sizeof(stAdjustOperation));
    
    stAdjustOperation.started = 1;
    stAdjustOperation.params[0].value.a = pstReeBuff->u32SmmuAddr;
    stAdjustOperation.params[0].value.b = 0;
    stAdjustOperation.params[1].value.a = u32TeeChnId;
    stAdjustOperation.params[1].value.b = 0;
    stAdjustOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
                                            TEEC_VALUE_INOUT,
                                            TEEC_NONE,
                                            TEEC_NONE);
    stResult = TEEC_InvokeCommand(&s_stTeeCtrlInfo.stSession, 
                                  TEE_PVR_PROCESS_TS_DATA, 
                                  &stAdjustOperation, 
                                  &u32Origin);
    if (TEEC_SUCCESS != stResult) 
    {
        HI_ERR_PVR("Invoke failed, codes=0x%x, origin=0x%x\n", stResult, u32Origin);
        return (HI_S32)stResult;
    }
    
    for (i = 0; i < pstIndexInfo->s32IndexNum; i++)
    {
        pstDmxRecData->stIndex[i].u64GlobalOffset = pstIndexInfo->u64GlobalOffset[i];
        pstDmxRecData->stIndex[i].u32FrameSize = pstIndexInfo->u32FrameSize[i];
    }
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}
HI_S32 HI_PVR_TeeCopyDataFromRee(PVR_REE_BUFFER_INFO_S *pstReeBuf, HI_U32 u32PhyAddr, HI_S32 s32Len)
{
    TEEC_Operation stStateOperation;
    TEEC_Result stResult;
    HI_U32 u32Origin;
    
    HI_PVR_DEBUG_ENTER();
    if (HI_NULL == pstReeBuf)
    {
        HI_ERR_PVR("null pointer for input argument!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    if (0 >= s32Len)
    {
        HI_ERR_PVR("Invalid data length(%d) for copy\n", s32Len);
        return HI_ERR_PVR_INVALID_PARA;
    }
    
    PVR_TEE_CHECK_INIT_RETURN();
    
    memset(&stStateOperation, 0x00, sizeof(stStateOperation));
    stStateOperation.started = 1;
    stStateOperation.params[0].value.a = pstReeBuf->u32SmmuAddr;
    stStateOperation.params[0].value.b = u32PhyAddr;
    stStateOperation.params[1].value.a = s32Len;
    stStateOperation.params[1].value.b = 0;
    stStateOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
                                            TEEC_VALUE_INOUT,
                                            TEEC_NONE,
                                            TEEC_NONE);
    
    stResult = TEEC_InvokeCommand(&s_stTeeCtrlInfo.stSession, 
                                  TEE_PVR_COPY_FROM_REE, 
                                  &stStateOperation, 
                                  &u32Origin);
    if (TEEC_SUCCESS != stResult) 
    {
        HI_ERR_PVR("Invoke failed, codes=0x%x, origin=0x%x\n", stResult, u32Origin);
        return (HI_S32)stResult;
    }
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_PVR_TeeGetTeeState(HI_U32 u32TeeChnId, PVR_REE_BUFFER_INFO_S *pstTeeState)
{
    TEEC_Operation stStateOperation;
    TEEC_Result stResult;
    HI_U32 u32Origin;
    
    HI_PVR_DEBUG_ENTER();
    if (HI_NULL == pstTeeState)
    {
        HI_ERR_PVR("null pointer for input argument!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    PVR_TEE_CHECK_INIT_RETURN();
    
    memset(&stStateOperation, 0x00, sizeof(stStateOperation));
    stStateOperation.started = 1;
    stStateOperation.params[0].value.a = u32TeeChnId;
    stStateOperation.params[0].value.b = 0;
    stStateOperation.params[1].value.a = pstTeeState->u32SmmuAddr;
    stStateOperation.params[1].value.b = 0;
    stStateOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
                                            TEEC_VALUE_INOUT,
                                            TEEC_NONE,
                                            TEEC_NONE);
    
    stResult = TEEC_InvokeCommand(&s_stTeeCtrlInfo.stSession, 
                                  TEE_PVR_GET_TEE_STATE, 
                                  &stStateOperation, 
                                  &u32Origin);
    if (TEEC_SUCCESS != stResult) 
    {
        HI_ERR_PVR("Invoke failed, codes=0x%x, origin=0x%x\n", stResult, u32Origin);
        return (HI_S32)stResult;
    }
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}

HI_S32 HI_PVR_TeeMallocReeBuffer(PVR_REE_BUFFER_INFO_S *pstReeBuf, HI_S32 s32Length, HI_BOOL bShare, HI_CHAR *pchName)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_PVR_DEBUG_ENTER();
    PVR_TEE_CHECK_INIT_RETURN();
    if (HI_NULL == pstReeBuf)
    {
        HI_ERR_PVR("null pointer for input argument!\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    pstReeBuf->pUserAddr = HI_NULL;
    s32Ret = PVR_TeeMallocSmmuBuffer(&pstReeBuf->u32SmmuAddr, s32Length, pchName);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("failed to calling PVR_TeeMallocSmmuBuffer!ret = 0x%08x!\n", s32Ret);
        return s32Ret;
    }
    pstReeBuf->pUserAddr = HI_MPI_SMMU_Map(pstReeBuf->u32SmmuAddr, HI_FALSE);
    HI_INFO_PVR("(smmu_addr, pUserAddr) = (0x%08x, %p)!\n", 
        pstReeBuf->u32SmmuAddr, pstReeBuf->pUserAddr);
    pstReeBuf->u32Size = s32Length;
    pstReeBuf->u32Used = 0;
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}
HI_S32 HI_PVR_TeeFreeReeBuffer(PVR_REE_BUFFER_INFO_S *pstReeBuf, HI_BOOL bShare)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_PVR_DEBUG_ENTER();
    PVR_TEE_CHECK_INIT_RETURN();
    if (HI_NULL == pstReeBuf)
    {
        HI_WARN_PVR("null pointer for input argument!\n");
        return HI_SUCCESS;
    }
    s32Ret = HI_MPI_SMMU_Unmap(pstReeBuf->u32SmmuAddr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("failed to calling HI_MPI_SMMU_Unmap!ret = 0x%08x!\n", s32Ret);
    }
    s32Ret = PVR_TeeFreeSmmuBuffer(pstReeBuf->u32SmmuAddr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("failed to calling PVR_TeeFreeSmmuBuffer!ret = 0x%08x!\n", s32Ret);
        return s32Ret;
    }
    pstReeBuf->u32SmmuAddr = HI_NULL;
    pstReeBuf->pUserAddr = HI_NULL;
    pstReeBuf->u32Size = 0;
    pstReeBuf->u32Used = 0;
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}   

HI_S32 HI_PVR_TeeGetRecTeeBuf(HI_U32 u32TeeChnId, HI_U8 **ppu8VirAddr, HI_U32 *pu32PhyAddr, HI_U32 *pu32Len)
{
    TEEC_Operation stCreateOperation;
    TEEC_Result stResult;
    HI_U32 u32Origin;
    
    HI_PVR_DEBUG_ENTER();
    PVR_TEE_CHECK_INIT_RETURN();
    if ((HI_NULL == ppu8VirAddr)
        || (HI_NULL == pu32PhyAddr)
        || (HI_NULL == pu32Len))
    {
        HI_ERR_PVR("null pointer for input argument\n");
        return HI_ERR_PVR_NUL_PTR;
    }
    *ppu8VirAddr = HI_NULL;
    *pu32PhyAddr = HI_NULL;
    
    memset(&stCreateOperation, 0x00, sizeof(stCreateOperation));
    stCreateOperation.started = 1;
    stCreateOperation.params[0].value.a = u32TeeChnId;
    stCreateOperation.params[0].value.b = *pu32Len;
    *pu32Len = 0;
    stCreateOperation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
                                            TEEC_VALUE_INOUT,
                                            TEEC_VALUE_INOUT,
                                            TEEC_NONE);

    stResult = TEEC_InvokeCommand(&s_stTeeCtrlInfo.stSession, 
                                  TEE_PVR_CA_GET_ADDRINFO, 
                                  &stCreateOperation, 
                                  &u32Origin);
    if (TEEC_SUCCESS != stResult) 
    {
        HI_ERR_PVR("[%s_%d]Invoke failed, codes=0x%x, origin=0x%x\n", __FILE__, __LINE__, stResult, u32Origin);
        return (HI_S32)stResult;
    }
    HI_INFO_PVR("get length = %d\n", stCreateOperation.params[1].value.b);
    *pu32PhyAddr = stCreateOperation.params[1].value.a;
    *pu32Len = stCreateOperation.params[1].value.b;
    HI_PVR_DEBUG_EXIT();
    return HI_SUCCESS;
}

