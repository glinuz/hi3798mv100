/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : loader_dbharden.c
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

#include "loader_debug.h"
#include "loader_crc32.h"
#include "loader_dbharden.h"

#ifdef HI_ADVCA_SUPPORT
#include "loader_cipher.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef HI_S32 (*LDDB_HARDEN_FN_INIT)(HI_VOID);
typedef HI_S32 (*LDDB_HARDEN_FN_DEINIT)(HI_VOID);
typedef HI_S32 (*LDDB_HARDEN_FN_VERIFY)(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue);
typedef HI_S32 (*LDDB_HARDEN_FN_SIGNATURE)(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue);
typedef HI_S32 (*LDDB_HARDEN_FN_ENCRYPT)(HI_U8* pu8Data, HI_U32 u32Size);
typedef HI_S32 (*LDDB_HARDEN_FN_DECRYPT)(HI_U8* pu8Data, HI_U32 u32Size);

typedef struct tagLDDB_HARDEN_ROUTRE_S
{
    LDDB_HARDEN_TYPE_E          enType;
    LDDB_HARDEN_FN_INIT         pfnInit;
    LDDB_HARDEN_FN_DEINIT       pfnDeInit;
    LDDB_HARDEN_FN_VERIFY       pfnVerify;
    LDDB_HARDEN_FN_SIGNATURE    pfnSignature;
    LDDB_HARDEN_FN_ENCRYPT      pfnEncrypt;
    LDDB_HARDEN_FN_DECRYPT      pfnDecrypt;

} LDDB_HARDEN_ROUTRE_S;

static HI_S32 lddbComDataVerify(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue);
static HI_S32 lddbComDataSignature(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue);
static HI_S32 lddbComDataEncrypt(HI_U8* pu8Data, HI_U32 u32Size);
static HI_S32 lddbComDataDecrypt(HI_U8* pu8Data, HI_U32 u32Size);
static HI_S32 lddbComDataInit(HI_VOID);
static HI_S32 lddbComDataDeInit(HI_VOID);

static LDDB_HARDEN_ROUTRE_S s_pstHardenRouter[] =
{
#ifdef HI_ADVCA_SUPPORT
    {
        LDDB_HARDEN_TYPE_CIPHER,
        LDDB_CIPHER_Init,
        LDDB_CIPHER_DeInit,
        LDDB_CIPHER_BlockVerify,
        LDDB_CIPHER_BlockSignature,
        LDDB_CIPHER_BlockEncrypt,
        LDDB_CIPHER_BlockDecrypt,
    },
#endif
    {
        LDDB_HARDEN_TYPE_COMMON,
        lddbComDataInit,
        lddbComDataDeInit,
        lddbComDataVerify,
        lddbComDataSignature,
        lddbComDataEncrypt,
        lddbComDataDecrypt
    }
};

#define LDDB_ROUTER_COUNT()     (sizeof(s_pstHardenRouter) / sizeof(LDDB_HARDEN_ROUTRE_S))

static HI_S32 lddbComDataVerify(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue)
{
    HI_U32 u32CrcValue = 0;
    HI_U32 u32CalValue = 0;

    u32CrcValue = (HI_U32)((pu8VerifyValue)[0] << 24)
                | (HI_U32)((pu8VerifyValue)[1] << 16)
                | (HI_U32)((pu8VerifyValue)[2] << 8)
                | (HI_U32)((pu8VerifyValue)[3]);

    u32CalValue = LDDB_CRC32(0, pu8Data, u32Size);
    if (u32CrcValue != u32CalValue)
    {
        LDDB_PrintErrCode(LDDB_ERR_DATABASE);
        return LDDB_ERR_DATABASE;
    }

    return HI_SUCCESS;
}

static HI_S32 lddbComDataSignature(HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue)
{
    HI_U32 u32CrcValue = 0;

    u32CrcValue = LDDB_CRC32(0, pu8Data, u32Size);

    pu8VerifyValue[0] = ((u32CrcValue >> 24) & 0xFF);
    pu8VerifyValue[1] = ((u32CrcValue >> 16) & 0xFF);
    pu8VerifyValue[2] = ((u32CrcValue >> 8) & 0xFF);
    pu8VerifyValue[3] = ((u32CrcValue) & 0xFF);

    return HI_SUCCESS;
}

static HI_S32 lddbComDataEncrypt(HI_U8* pu8Data, HI_U32 u32Size)
{
    LDDB_DBG_FuncEnter();

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 lddbComDataDecrypt(HI_U8* pu8Data, HI_U32 u32Size)
{
    LDDB_DBG_FuncEnter();

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 lddbComDataInit(HI_VOID)
{
    LDDB_DBG_FuncEnter();

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 lddbComDataDeInit(HI_VOID)
{
    LDDB_DBG_FuncEnter();

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_HARDEN_Init(HI_VOID)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Count = 0;

    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    u32Count = LDDB_ROUTER_COUNT();

    for (u32Index = 0; u32Index < u32Count; u32Index++)
    {
        if (HI_NULL_PTR == s_pstHardenRouter[u32Index].pfnInit)
        {
            continue;
        }

        s32Ret = s_pstHardenRouter[u32Index].pfnInit();
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(s_pstHardenRouter[u32Index].pfnInit, s32Ret);
            return s32Ret;
        }
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_HARDEN_DeInit(HI_VOID)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Count = 0;

    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    u32Count = LDDB_ROUTER_COUNT();

    for (u32Index = 0; u32Index < u32Count; u32Index++)
    {
        if (HI_NULL_PTR == s_pstHardenRouter[u32Index].pfnDeInit)
        {
            continue;
        }

        s32Ret = s_pstHardenRouter[u32Index].pfnDeInit();
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(s_pstHardenRouter[u32Index].pfnDeInit, s32Ret);
            return s32Ret;
        }
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LDDB_HARDEN_DataVerify(LDDB_HARDEN_TYPE_E enVerifyType, HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Count = 0;

    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8VerifyValue);
    LDDB_CHECK_PARAM(0 == u32Size);

    u32Count = LDDB_ROUTER_COUNT();

    for (u32Index = 0; u32Index < u32Count; u32Index++)
    {
        if (enVerifyType != s_pstHardenRouter[u32Index].enType)
        {
            continue;
        }

        if (HI_NULL_PTR == s_pstHardenRouter[u32Index].pfnVerify)
        {
            LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
            return LDDB_ERR_UNSUPPORTED;
        }

        s32Ret = s_pstHardenRouter[u32Index].pfnVerify(pu8Data, u32Size, pu8VerifyValue);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(s_pstHardenRouter[u32Index].pfnVerify, s32Ret);
            return s32Ret;
        }

        LDDB_DBG_FuncExit();
        return HI_SUCCESS;
    }

    LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
    return LDDB_ERR_UNSUPPORTED;
}

HI_S32 LDDB_HARDEN_DataSignature(LDDB_HARDEN_TYPE_E enVerifyType, HI_U8* pu8Data, HI_U32 u32Size, HI_U8* pu8VerifyValue)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Count = 0;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8VerifyValue);
    LDDB_CHECK_PARAM(0 == u32Size);

    u32Count = LDDB_ROUTER_COUNT();

    for (u32Index = 0; u32Index < u32Count; u32Index++)
    {
        if (enVerifyType != s_pstHardenRouter[u32Index].enType)
        {
            continue;
        }

        if (HI_NULL_PTR == s_pstHardenRouter[u32Index].pfnSignature)
        {
            LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
            return LDDB_ERR_UNSUPPORTED;
        }

        s32Ret = s_pstHardenRouter[u32Index].pfnSignature(pu8Data, u32Size, pu8VerifyValue);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(s_pstHardenRouter[u32Index].pfnSignature, s32Ret);
            return s32Ret;
        }

        LDDB_DBG_FuncExit();
        return HI_SUCCESS;
    }

    LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
    return LDDB_ERR_UNSUPPORTED;
}

HI_S32 LDDB_HARDEN_DataEncrypt(LDDB_HARDEN_TYPE_E enVerifyType, HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Count = 0;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(0 == u32Size);

    u32Count = LDDB_ROUTER_COUNT();

    for (u32Index = 0; u32Index < u32Count; u32Index++)
    {
        if (enVerifyType != s_pstHardenRouter[u32Index].enType)
        {
            continue;
        }

        if (HI_NULL_PTR == s_pstHardenRouter[u32Index].pfnEncrypt)
        {
            LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
            return LDDB_ERR_UNSUPPORTED;
        }

        s32Ret = s_pstHardenRouter[u32Index].pfnEncrypt(pu8Data, u32Size);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(s_pstHardenRouter[u32Index].pfnEncrypt, s32Ret);
            return s32Ret;
        }

        LDDB_DBG_FuncExit();
        return HI_SUCCESS;
    }

    LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
    return LDDB_ERR_UNSUPPORTED;
}

HI_S32 LDDB_HARDEN_DataDecrypt(LDDB_HARDEN_TYPE_E enVerifyType, HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_U32 u32Index = 0;
    HI_U32 u32Count = 0;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    LDDB_CHECK_PARAM(0 == u32Size);

    u32Count = LDDB_ROUTER_COUNT();

    for (u32Index = 0; u32Index < u32Count; u32Index++)
    {
        if (enVerifyType != s_pstHardenRouter[u32Index].enType)
        {
            continue;
        }

        if (HI_NULL_PTR == s_pstHardenRouter[u32Index].pfnDecrypt)
        {
            LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
            return LDDB_ERR_UNSUPPORTED;
        }

        s32Ret = s_pstHardenRouter[u32Index].pfnDecrypt(pu8Data, u32Size);
        if (HI_SUCCESS != s32Ret)
        {
            LDDB_PrintFuncErr(s_pstHardenRouter[u32Index].pfnDecrypt, s32Ret);
            return s32Ret;
        }

        LDDB_DBG_FuncExit();
        return HI_SUCCESS;
    }

    LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
    return LDDB_ERR_UNSUPPORTED;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

