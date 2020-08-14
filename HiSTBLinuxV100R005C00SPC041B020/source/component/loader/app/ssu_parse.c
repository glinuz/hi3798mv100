/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ssu_parse_v3.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "com_osal.h"
#include "com_parse.h"
#include "com_debug.h"
#include "ssu_parse.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/

#define SSU_D_POS_DSIVALIDDATA          (22)
#define SSU_D_SPECIFIERTYPE             (0x01)

/*************************** Structure Definition *****************************/
typedef enum tagSSU_DESCRIPTOR_TYPE_E
{
    SSU_E_DESCRIPTOR_TYPE_HW  = 0x01,
    SSU_E_DESCRIPTOR_TYPE_SW  = 0x02,
    SSU_E_DESCRIPTOR_TYPE_SSN = 0x40,
    SSU_E_DESCRIPTOR_TYPE_ESN = 0x41,

    SSU_E_DESCRIPTOR_TYPE_BUTT,

} SSU_DESCRIPTOR_TYPE_E;



/*************************** API Declaration **********************************/

static HI_S32 ssuParseMsgHeader
(
    HI_U8*                  pu8DataBuf,
    HI_U16                  u16DataLen,
    SSU_DSMCC_MSGHEADER_S*  pstMsgHeader
)
{
    HI_U32 u16TotalRead = 0;
    COM_SECTION_S stSection;
    HI_U8* pu8MsgData = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_MemSet(&stSection, 0x00, sizeof(stSection));
    s32Ret = COM_ParseSection(pu8DataBuf, u16DataLen, &stSection);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(COM_ParseSection, s32Ret);
        return s32Ret;
    }

    if (SSU_D_DSMCC_MINHEADLEN > stSection.u32PrivateDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pu8MsgData = stSection.pu8PrivateData;
    HI_READ_08BIT(pu8MsgData, pstMsgHeader->u8ProtocolDiscriminator, u16TotalRead);

    if (SSU_D_DSMCC_PROTOCOL != pstMsgHeader->u8ProtocolDiscriminator)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_08BIT(pu8MsgData, pstMsgHeader->u8DsmccType, u16TotalRead);

    if (SSU_D_DSMCC_TYPE != pstMsgHeader->u8DsmccType)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_16BIT(pu8MsgData, pstMsgHeader->u16MessageId, u16TotalRead);
    HI_READ_32BIT(pu8MsgData, pstMsgHeader->u32TransactionId, u16TotalRead);
    HI_READ_08BIT(pu8MsgData, pstMsgHeader->u8Reserved, u16TotalRead);
    HI_READ_08BIT(pu8MsgData, pstMsgHeader->u8AdapationLength, u16TotalRead);
    HI_READ_16BIT(pu8MsgData, pstMsgHeader->u16MessageLength, u16TotalRead);
    HI_SKIP_BYTES(pu8MsgData, pstMsgHeader->u8AdapationLength, u16TotalRead);

    if (u16TotalRead > stSection.u32PrivateDataLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pstMsgHeader->pu8MessageData = pu8MsgData;
    pstMsgHeader->u16MessageLength -= pstMsgHeader->u8AdapationLength;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssuParseCompatibility
(
    HI_U8*                   pu8DataBuf,
    HI_U16                   u16DataLen,
    SSU_COMPATIBILITY_DES_S* pstDescriptor,
    HI_U16*                  pu16DescriptorLen
)
{
    HI_U16 u16TotalRead = 0;
    HI_U8  u8SubDescriptorCount = 0;

    COM_DBG_FuncEnter();

    HI_READ_08BIT(pu8DataBuf, pstDescriptor->u8DescriptorType, u16TotalRead);
    HI_READ_08BIT(pu8DataBuf, pstDescriptor->u8DescriptorLength, u16TotalRead);
    HI_READ_08BIT(pu8DataBuf, pstDescriptor->u8SpecifierType, u16TotalRead);
    HI_READ_24BIT(pu8DataBuf, pstDescriptor->u32SpecifierData, u16TotalRead);
    HI_READ_16BIT(pu8DataBuf, pstDescriptor->u16Model, u16TotalRead);
    HI_READ_16BIT(pu8DataBuf, pstDescriptor->u16Version, u16TotalRead);
    HI_READ_08BIT(pu8DataBuf, u8SubDescriptorCount, u16TotalRead);
    HI_SKIP_BYTES(pu8DataBuf, u8SubDescriptorCount, u16TotalRead);

    if (u16DataLen < u16TotalRead)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    *pu16DescriptorLen = u16TotalRead;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssuParseCompatibilitys
(
    HI_U8*      pu8DescriptorData,
    HI_U16      u16DescriptorLength,
    SSU_GII_S*  pstGII
)
{
    HI_U16 i = 0;
    HI_U16 u16TotalRead = 0;
    HI_U16 u16RemainLen = 0;
    HI_U16 u16DesLen = 0;
    HI_U32 u16DesCount = 0;
    HI_U32 u32Version = 0;
    SSU_COMPATIBILITY_DES_S stCompatibility;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    HI_READ_16BIT(pu8DescriptorData, u16DesCount, u16TotalRead);

    if (0 == u16DesCount)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    for (i = 0; i < u16DesCount; i++)
    {
        if (u16TotalRead >= u16DescriptorLength)
        {
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        u16RemainLen = u16DescriptorLength - u16TotalRead;
        COM_MemSet(&stCompatibility, 0x00, sizeof(stCompatibility));
        s32Ret = ssuParseCompatibility(pu8DescriptorData, u16RemainLen, &stCompatibility, &u16DesLen);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(ssuParseCompatibility, s32Ret);
            return s32Ret;
        }

        HI_SKIP_BYTES(pu8DescriptorData, u16DesLen, u16TotalRead);

        if (SSU_D_SPECIFIERTYPE != stCompatibility.u8SpecifierType)
        {
            continue;
        }

        pstGII->u32ManufacturerID = stCompatibility.u32SpecifierData;
        u32Version = (stCompatibility.u16Model << 16) | stCompatibility.u16Version;

        switch (stCompatibility.u8DescriptorType)
        {
            case SSU_E_DESCRIPTOR_TYPE_HW:
            {
                pstGII->u32HardwareVersion = u32Version;
                break;
            }

            case SSU_E_DESCRIPTOR_TYPE_SW:
            {
                pstGII->u32SoftwareVersion = u32Version;
                break;
            }

            case SSU_E_DESCRIPTOR_TYPE_SSN:
            {
                pstGII->u32SerialNumberStart = u32Version;
                break;
            }

            case SSU_E_DESCRIPTOR_TYPE_ESN:
            {
                pstGII->u32SerialNumberEnd = u32Version;
                break;
            }

            default:
            {
                break;
            }
        }
    }

    if (u16DescriptorLength != u16TotalRead)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssuParseGroupInfoIndication
(
    HI_U8*      pu8DataBuf,
    HI_U16      u16DataLen,
    SSU_GII_S*  pstGII,
    HI_U16*     pu16DescriptorLen
)
{
    HI_U16 u16TotalRead = 0;
    HI_U16 u16DescriptorLength = 0;
    HI_U16 u16GroupInfoLength = 0;
    HI_U16 u16PrivateDataLength = 0;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    HI_READ_32BIT(pu8DataBuf, pstGII->u32GroupID, u16TotalRead);
#if defined (HI_LOADER_PROTOCOL_VER2)
    HI_SKIP_BYTES(pu8DataBuf, 4, u16TotalRead);
#endif
    HI_READ_32BIT(pu8DataBuf, pstGII->u32GroupSize, u16TotalRead);
    HI_READ_16BIT(pu8DataBuf, u16DescriptorLength, u16TotalRead);

    if ((0 == u16DescriptorLength)
        || (u16DescriptorLength > u16DataLen - u16TotalRead))
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    s32Ret = ssuParseCompatibilitys(pu8DataBuf, u16DescriptorLength, pstGII);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(ssuParseCompatibilitys, s32Ret);
        return s32Ret;
    }

    HI_SKIP_BYTES(pu8DataBuf, u16DescriptorLength,  u16TotalRead);
    HI_READ_16BIT(pu8DataBuf, u16GroupInfoLength,   u16TotalRead);
    HI_SKIP_BYTES(pu8DataBuf, u16GroupInfoLength,   u16TotalRead);
    HI_READ_16BIT(pu8DataBuf, u16PrivateDataLength, u16TotalRead);
    HI_SKIP_BYTES(pu8DataBuf, u16PrivateDataLength, u16TotalRead);

    if (u16DataLen < u16TotalRead)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    *pu16DescriptorLen = u16TotalRead;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSU_ParseDSI(HI_U8* pu8DataBuf, HI_U16 u16DataLen, SSU_DSI_S* pstDSI)
{
    HI_U16 i = 0;
    HI_U32 u32MemSize = 0;
    HI_U16 u16TotalRead = 0;
    HI_U16 u16RemianLen = 0;
    HI_U16 u16PrivateDataLength = 0;
    HI_U16 u16GroupSize = 0;
    HI_U8* pu8MessageData = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8DataBuf);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDSI);
    COM_CHECK_PARAM(0 == u16DataLen);

    s32Ret = ssuParseMsgHeader(pu8DataBuf, u16DataLen, &pstDSI->stMsgHeader);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(ssuParseMsgHeader, s32Ret);
        return s32Ret;
    }

    if (SSU_D_DSMCC_DSIMSGID != pstDSI->stMsgHeader.u16MessageId)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pu8MessageData = pstDSI->stMsgHeader.pu8MessageData;
    HI_SKIP_BYTES(pu8MessageData, SSU_D_POS_DSIVALIDDATA, u16TotalRead);
    HI_READ_16BIT(pu8MessageData, u16PrivateDataLength, u16TotalRead);

    if (u16PrivateDataLength + u16TotalRead != pstDSI->stMsgHeader.u16MessageLength)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_16BIT(pu8MessageData, pstDSI->u16NumberOfGroups, u16TotalRead);

    if (0 == pstDSI->u16NumberOfGroups)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    u32MemSize = pstDSI->u16NumberOfGroups * sizeof(SSU_GII_S);
    pstDSI->pstGroups = COM_MemMalloc(u32MemSize);

    if (HI_NULL_PTR == pstDSI->pstGroups)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pstDSI->pstGroups, 0x00, u32MemSize);

    for (i = 0; i < pstDSI->u16NumberOfGroups; i++)
    {
        u16RemianLen = pstDSI->stMsgHeader.u16MessageLength - u16TotalRead;
        s32Ret = ssuParseGroupInfoIndication(pu8MessageData, u16RemianLen, &pstDSI->pstGroups[i], &u16GroupSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(ssuParseGroupInfoIndication, s32Ret);

            COM_MemFreeSetNull(pstDSI->pstGroups);

            return s32Ret;
        }

        HI_SKIP_BYTES(pu8MessageData, u16GroupSize, u16TotalRead);
    }

    if (pstDSI->stMsgHeader.u16MessageLength != u16TotalRead)
    {
        COM_MemFreeSetNull(pstDSI->pstGroups);

        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSU_FreeDSI(SSU_DSI_S* pstDSI)
{
    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstDSI);

    if (HI_NULL_PTR != pstDSI->pstGroups)
    {
        COM_MemFreeSetNull(pstDSI->pstGroups);
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSU_ParseDII(HI_U8* pu8DataBuf, HI_U16 u16DataLen, SSU_DII_S* pstDII)
{
    HI_U16 i = 0;
    HI_U32 u32MemLen = 0;
    HI_U32 u16TotalRead = 0;
    HI_U16 u16PrivateDataLength = 0;
    HI_U16 u16Identification = 0;
    HI_U16 u16ReserveLen = 0;
    HI_U8  u8ModuleInfoLength = 0;
    HI_U8  u8ModuleInfoReadLen = 0;
    HI_U8* pu8MessageData = HI_NULL_PTR;
    SSU_DII_MODULE_S* pstModule = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8DataBuf);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDII);
    COM_CHECK_PARAM(0 == u16DataLen);

    s32Ret = ssuParseMsgHeader(pu8DataBuf, u16DataLen, &pstDII->stMsgHeader);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(ssuParseMsgHeader, s32Ret);
        return s32Ret;
    }

    if (SSU_D_DSMCC_DIIMSGID != pstDII->stMsgHeader.u16MessageId)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pu8MessageData = pstDII->stMsgHeader.pu8MessageData;
    HI_READ_32BIT(pu8MessageData, pstDII->u32DownloadId, u16TotalRead);
    HI_READ_16BIT(pu8MessageData, pstDII->u16BlockSize,  u16TotalRead);
    HI_READ_08BIT(pu8MessageData, pstDII->u8WindowSize,  u16TotalRead);
    HI_READ_08BIT(pu8MessageData, pstDII->u8AckPeriod,   u16TotalRead);
    HI_READ_32BIT(pu8MessageData, pstDII->u32TCDownloadWindow, u16TotalRead);
    HI_READ_32BIT(pu8MessageData, pstDII->u32TCDownloadscenario, u16TotalRead);
    HI_READ_16BIT(pu8MessageData, pstDII->u16CompatibilityDescriptorLength, u16TotalRead);

    if (0 != pstDII->u16CompatibilityDescriptorLength)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    HI_READ_16BIT(pu8MessageData, pstDII->u16NumberOfModules, u16TotalRead);

    if (0 == pstDII->u16NumberOfModules)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    u32MemLen = pstDII->u16NumberOfModules * sizeof(SSU_DII_MODULE_S);
    pstDII->pstModules = COM_MemMalloc(u32MemLen);
    if (HI_NULL_PTR == pstDII->pstModules)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pstDII->pstModules, 0x00, u32MemLen);

    for (i = 0; i < pstDII->u16NumberOfModules; i++)
    {
        u8ModuleInfoReadLen = 0;
        pstModule = &pstDII->pstModules[i];

        HI_READ_16BIT(pu8MessageData, pstModule->u16ModuleID, u16TotalRead);
        HI_READ_32BIT(pu8MessageData, pstModule->u32ModuleSize, u16TotalRead);
        HI_READ_08BIT(pu8MessageData, pstModule->u8ModuleVersion, u16TotalRead);
        HI_READ_08BIT(pu8MessageData, u8ModuleInfoLength, u16TotalRead);
#if defined (HI_LOADER_PROTOCOL_VER3) || defined (HI_LOADER_PROTOCOL_VER2)
        HI_READ_64BIT(pu8MessageData, pstModule->u64FlashStartAddr, u8ModuleInfoReadLen);
        HI_READ_64BIT(pu8MessageData, pstModule->u64FlashEndAddr, u8ModuleInfoReadLen);
#else defined (HI_LOADER_PROTOCOL_VER1)
        HI_READ_32BIT(pu8MessageData, pstModule->u64FlashStartAddr, u8ModuleInfoReadLen);
        HI_READ_32BIT(pu8MessageData, pstModule->u64FlashEndAddr, u8ModuleInfoReadLen);
#endif
        HI_READ_32BIT(pu8MessageData, pstModule->u32FlashType, u8ModuleInfoReadLen);
        HI_READ_32BIT(pu8MessageData, pstModule->u32FlashIndex, u8ModuleInfoReadLen);

#if defined (HI_LOADER_PROTOCOL_VER3)
        HI_READ_32BIT(pu8MessageData, pstModule->u32PartVersion, u8ModuleInfoReadLen);
        HI_READ_32BIT(pu8MessageData, pstModule->u32PartVerifyTag, u8ModuleInfoReadLen);
        HI_READ_16BIT(pu8MessageData, u16Identification, u8ModuleInfoReadLen);
        HI_READ_BYTES(pu8MessageData, u16Identification, pstModule->au8PartName, SSU_D_MAXLEN_PARTNAME, u8ModuleInfoReadLen);
        HI_READ_16BIT(pu8MessageData, u16ReserveLen, u8ModuleInfoReadLen);
        HI_SKIP_BYTES(pu8MessageData, u16ReserveLen, u8ModuleInfoReadLen);
#endif
        HI_READ_32BIT(pu8MessageData, pstModule->u32CRC32, u8ModuleInfoReadLen);

        if (u8ModuleInfoReadLen != u8ModuleInfoLength)
        {
            COM_MemFreeSetNull(pstDII->pstModules);

            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;

        }

        u16TotalRead += u8ModuleInfoLength;
    }

    HI_READ_16BIT(pu8MessageData, u16PrivateDataLength, u16TotalRead);
    HI_SKIP_BYTES(pu8MessageData, u16PrivateDataLength, u16TotalRead);

    if (pstDII->stMsgHeader.u16MessageLength != u16TotalRead)
    {
        COM_MemFreeSetNull(pstDII->pstModules);

        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSU_FreeDII(SSU_DII_S* pstDII)
{
    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstDII);

    if (HI_NULL_PTR != pstDII->pstModules)
    {
        COM_MemFreeSetNull(pstDII->pstModules);
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSU_ParseDDB(HI_U8* pu8DataBuf, HI_U16 u16DataLen, SSU_DDB_S* pstDDB)
{
    HI_U16 u16TotalRead = 0;
    HI_U8* pu8MessageData = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8DataBuf);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDDB);
    COM_CHECK_PARAM(0 == u16DataLen);

    s32Ret = ssuParseMsgHeader(pu8DataBuf, u16DataLen, &pstDDB->stMsgHeader);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(ssuParseMsgHeader, s32Ret);
        return s32Ret;
    }

    if (SSU_D_DSMCC_DDBMSGID != pstDDB->stMsgHeader.u16MessageId)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pu8MessageData = pstDDB->stMsgHeader.pu8MessageData;
    HI_READ_16BIT(pu8MessageData, pstDDB->u16ModuleId,     u16TotalRead);
    HI_READ_08BIT(pu8MessageData, pstDDB->u8ModuleVersion, u16TotalRead);
    HI_READ_08BIT(pu8MessageData, pstDDB->u8Reserved,      u16TotalRead);
    HI_READ_16BIT(pu8MessageData, pstDDB->u16BlockNumber,  u16TotalRead);

    if (u16TotalRead >= pstDDB->stMsgHeader.u16MessageLength)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pstDDB->puBlockData = pu8MessageData;
    pstDDB->u16BlockSize = pstDDB->stMsgHeader.u16MessageLength - u16TotalRead;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
