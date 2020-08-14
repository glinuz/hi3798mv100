#include "tee_common.h"
#include "tee_internal_api.h"
#include "tee_mem_mgmt_api.h"
#define TA_DEBUG
#include "tee_log.h"
#include "tee_fs.h"
#include "tee_trusted_storage_api.h"
#include "tee_time_api.h"
#include "hi_tee_mem.h"
#include "sre_syscalls_chip.h"
#include "tee_pvr.h"

/*****************************************************************************
 Prototype       : TEE_PVR_DeInit
 Description     : De-initialize
 Input             : None
 Output           : None
 Return Value   : 
*****************************************************************************/
HI_S32 TEE_PVR_DeInit(HI_VOID)
{
    return __TEE_PVR_DeInit();
}

/*****************************************************************************
 Prototype       : TEE_PVR_RecOpenChannel
 Description     : open a recroding channel
 Input             : the buffer length
 Output           : channel id
 Return Value   : 
*****************************************************************************/
HI_S32 TEE_PVR_RecOpenChannel(TEE_Param stParams[4])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32RecChnId = -1;
    HI_S32 s32BufferLen = 0;

    /*the  buffer length shall be align by TEE_PVR_DATA_ALIGN*/
    s32BufferLen = stParams[0].value.b;  
    if ((HI_U32)s32BufferLen > TEE_PVR_MAX_BUF_LENGTH)
    {
        TEE_ERR_PVR("the data length is too long(%d), shall be less than(%d)", s32BufferLen, TEE_PVR_MAX_BUF_LENGTH);
        return HI_TEE_ERROR_PVR_INVALID_PARAM;
    }
    s32BufferLen = s32BufferLen/TEE_PVR_DATA_ALIGN * TEE_PVR_DATA_ALIGN + TEE_PVR_DATA_ALIGN;
    TEE_DEBUG_PVR("(stParams[0].value.b, s32BufferLen) = (%d, %d)\n", stParams[0].value.b, s32BufferLen);

    stParams[0].value.a = -1;
    
    s32Ret = __TEE_PVR_OpenChannel(&u32RecChnId,s32BufferLen,TEE_PVR_RECORD);
    if (HI_SUCCESS != s32Ret)
    {
        TEE_ERR_PVR("open tee record channel failed, ret = 0x%x\n", s32Ret);
    }
    else
    {
        stParams[0].value.a = u32RecChnId;
    }
    
    return s32Ret;
}

/*****************************************************************************
 Prototype       : TEE_PVR_RecCloseChannel
 Description     : close the recroding channel
 Input             : channel id
 Output           : None
 Return Value   : 
*****************************************************************************/
HI_S32 TEE_PVR_RecCloseChannel(TEE_Param stParams[4])
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    s32Ret = __TEE_PVR_CloseChannel(stParams[0].value.a);
    if (HI_SUCCESS != s32Ret)
    {
        TEE_ERR_PVR("close tee record channel(%d) failed, ret = 0x%x\n", stParams[0].value.a, s32Ret);
    }
    
    return s32Ret;
}

/*****************************************************************************
 Prototype       : TEE_PVR_RecProcTsData
 Description     : copy ts data from demux and process it
 Input             : channel id; 
                       the ts data addr form demux
 Output           : None
 Return Value   : 
*****************************************************************************/
HI_S32 TEE_PVR_RecProcTsData(TEE_Param stParams[4])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32RecChnId = stParams[1].value.a;
    HI_U32 u32NSSmmuAddr = stParams[0].value.a;
    
    s32Ret = __TEE_PVR_RecProcTsData(u32RecChnId, u32NSSmmuAddr);
    if (HI_SUCCESS != s32Ret)
    {
        TEE_ERR_PVR("process ts data failed, ret = 0x%x\n", s32Ret);
    }
    
    return s32Ret;
}

/*****************************************************************************
 Prototype       : TEE_PVR_RecCopyToRee
 Description     : copy ts data from tee to ree. This API is defined for test, if WITH_HISI_TEST not defined, 
                       this API won't do the copy
 Input             :  the addr of TEE
                        the addr of REE
                        the length of data
 Output           : None
 Return Value   : 
*****************************************************************************/
HI_S32 TEE_PVR_RecCopyToRee(TEE_Param stParams[4])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32NSSmmuAddr = stParams[1].value.a;
    HI_U32 u32SecureSmmuAddr = stParams[2].value.a;
    HI_U32 u32BufLen = stParams[2].value.b;

    if (u32BufLen > TEE_PVR_MAX_BUF_LENGTH)
    {
        TEE_ERR_PVR("the data length is too long(%d), shall be less than(%d)", u32BufLen, TEE_PVR_MAX_BUF_LENGTH);
        return HI_TEE_ERROR_PVR_INVALID_PARAM;
    }
    
    s32Ret = __TEE_PVR_RecCopyToRee(u32SecureSmmuAddr, u32NSSmmuAddr,&u32BufLen);
    if (HI_SUCCESS != s32Ret)
    {
        TEE_ERR_PVR("channel(%d) copy ts data to ree failed, ret = 0x%x\n", u32SecureSmmuAddr, s32Ret);
    }
    
    return s32Ret;
}

/*****************************************************************************
 Prototype       : TEE_PVR_PlayCopyFromRee
 Description     : copy ts data from ree to tee. This API is defined for playback 
 Input             :  the addr of TEE
                        the addr of REE
                        the length of data
 Output           : None
 Return Value   : 
*****************************************************************************/
HI_S32 TEE_PVR_PlayCopyFromRee(TEE_Param stParams[4])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ReeAddr = stParams[0].value.a;
    HI_U32 u32TeeAddr = stParams[0].value.b;
    HI_U32 u32DataLen = stParams[1].value.a;
    
    if (u32DataLen > TEE_PVR_MAX_BUF_LENGTH)
    {
        TEE_ERR_PVR("the data length is too long(%d), shall be less than(%d)", u32DataLen, TEE_PVR_MAX_BUF_LENGTH);
        return HI_TEE_ERROR_PVR_INVALID_PARAM;
    }
    
    /*playback don't need tee channel current*/
    s32Ret = __TEE_PVR_PlayCopyFromRee(u32ReeAddr, u32TeeAddr, u32DataLen);
    if (HI_SUCCESS != s32Ret)
    {
        TEE_ERR_PVR("copy ts data from ree failed, ret = 0x%x\n", s32Ret);
    }
    
    return s32Ret;
}

/*****************************************************************************
 Prototype       : TEE_PVR_RecGetOutputBufInfo
 Description     : get the output buffer addr and length
 Input             : channel id
                        max length of returned
 Output           : None
 Return Value   : 
*****************************************************************************/
HI_S32 TEE_PVR_RecGetOutputBufInfo(TEE_Param stParams[4])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32RecChnId = stParams[0].value.a;
    HI_U32 u32SmmuAddr = 0;
    HI_U32 u32ValidLen = stParams[0].value.b;

    stParams[1].value.a = HI_NULL;
    stParams[1].value.b = 0;
    
    s32Ret = __TEE_PVR_RecGetOutputBufInfo(u32RecChnId, &u32SmmuAddr, &u32ValidLen);
    if (HI_SUCCESS != s32Ret)
    {
        TEE_ERR_PVR("channel(%d) get tee output buffer info failed, ret = 0x%x\n", u32RecChnId, s32Ret);
    }
    else
    {
        stParams[1].value.a = u32SmmuAddr;
        stParams[1].value.b = u32ValidLen;
    }
    return s32Ret;
}

/*****************************************************************************
 Prototype       : TEE_PVR_RecGetStatus
 Description     : get current status of the recording channel
 Input             : channel id
                     :  ree addr used to save the status
 Output           : current status
 Return Value   : 
*****************************************************************************/
HI_S32 TEE_PVR_RecGetStatus(TEE_Param stParams[4])
{
    return __TEE_PVR_RecGetStatus(stParams[0].value.a, stParams[1].value.a);
}


