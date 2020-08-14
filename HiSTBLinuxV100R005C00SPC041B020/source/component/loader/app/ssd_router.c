/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ssd_router.c
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

#include "com_debug.h"
#include "ssd_router.h"

#ifdef HI_ADVCA_SUPPORT
#ifdef HI_ADVCA_TYPE_VERIMATRIX
#include "vmx_ssd.h"
#endif
#endif

#ifdef HI_ADVCA_SUPPORT
#ifdef HI_ADVCA_TYPE_VERIMATRIX
#include "vmx_ssd.h"
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef HI_S32 (*ADVCA_SSD_FN_INIT)(HI_VOID);
typedef HI_S32 (*ADVCA_SSD_FN_DEINIT)(HI_VOID);

typedef HI_S32 (*ADVCA_SSD_FN_WHOLE_IMAGEVERIFY)
(
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
);

typedef HI_S32 (*ADVCA_SSD_FN_PARTITION_IMAGEVERIFY)
(
    HI_U8*      pu8PartitionName,
    HI_U64      u64FlashStartAddr,
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize,
    HI_U32*     pu32ImageVersion,
    HI_BOOL*    pbNeedUpgrade
);

typedef struct tagADVCA_SSD_ROUTER_S
{
    ADVCA_SSD_FN_INIT                   pfnInit;
    ADVCA_SSD_FN_DEINIT                 pfnDeInit;
    ADVCA_SSD_FN_WHOLE_IMAGEVERIFY      pfnWholeImageVerify;
    ADVCA_SSD_FN_PARTITION_IMAGEVERIFY  pfnParittionImageVerify;

} ADVCA_SSD_ROUTER_S;

static ADVCA_SSD_ROUTER_S s_stRouterPool =
{
#ifdef HI_ADVCA_SUPPORT
#ifdef HI_ADVCA_TYPE_VERIMATRIX
    WMX_SSD_Init,
    WMX_SSD_DeInit,
    WMX_SSD_WholeImageVerfy,
    WMX_SSD_PartitionImageVerfy,
#endif
#else
    HI_NULL_PTR,
    HI_NULL_PTR,
    HI_NULL_PTR,
    HI_NULL_PTR
#endif
};

static ADVCA_SSD_ROUTER_S* s_pstRouterIns = HI_NULL_PTR;

HI_S32 ADVCA_SSD_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    if (HI_NULL_PTR != s_stRouterPool.pfnInit)
    {
        s32Ret = s_stRouterPool.pfnInit();
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(s_stRouterPool.pfnInit(), s32Ret);
            return s32Ret;
        }
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 ADVCA_SSD_DeInit(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    if (HI_NULL_PTR != s_stRouterPool.pfnDeInit)
    {
        s32Ret = s_stRouterPool.pfnDeInit();
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(s_stRouterPool.pfnDeInit(), s32Ret);
            return s32Ret;
        }
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;

}

HI_S32 ADVCA_SSD_WholeImageVerify
(
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
)
{
    HI_U32 i = 0;
    HI_U32 u32ProtNum = 0;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8ImageData);
    COM_CHECK_PARAM(HI_NULL_PTR == ppu8DataAddr);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32DataSize);
    COM_CHECK_PARAM(0 == u32ImageSize);

    if (HI_NULL_PTR != s_stRouterPool.pfnWholeImageVerify)
    {
        s32Ret = s_stRouterPool.pfnWholeImageVerify(pu8ImageData, u32ImageSize, ppu8DataAddr, pu32DataSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(s_stRouterPool.pfnWholeImageVerify, s32Ret);
            return s32Ret;
        }

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    *ppu8DataAddr = pu8ImageData;
    *pu32DataSize = u32ImageSize;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 ADVCA_SSD_PartitionImageVerify
(
    HI_U8*      pu8PartitionName,
    HI_U64      u64FlashStartAddr,
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize,
    HI_U32*     pu32ImageVersion,
    HI_BOOL*    pbNeedUpgrade
)
{
    HI_U32 i = 0;
    HI_U32 u32ProtNum = 0;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8ImageData);
    COM_CHECK_PARAM(HI_NULL_PTR == ppu8DataAddr);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32DataSize);
    COM_CHECK_PARAM(0 == u32ImageSize);

    if (HI_NULL_PTR != s_stRouterPool.pfnParittionImageVerify)
    {
        s32Ret = s_stRouterPool.pfnParittionImageVerify(pu8PartitionName,
                                                        u64FlashStartAddr,
                                                        pu8ImageData,
                                                        u32ImageSize,
                                                        ppu8DataAddr,
                                                        pu32DataSize,
                                                        pu32ImageVersion,
                                                        pbNeedUpgrade);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(s_stRouterPool.pfnParittionImageVerify, s32Ret);
            return s32Ret;
        }

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    *ppu8DataAddr = pu8ImageData;
    *pu32DataSize = u32ImageSize;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 ADVCA_SSD_BlockDataVerify
(
    HI_U8*      pu8BlockData,
    HI_U32      u32BlockSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
)
{

}

HI_S32 ADVCA_SSD_BlockDataSignature
(
    HI_U8*      pu8BlockData,
    HI_U32      u32BlockSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
)
{

}

HI_S32 ADVCA_SSD_BlockDataEncrypt
(
    HI_U8*      pu8BlockData,
    HI_U32      u32BlockSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
)
{

}

HI_S32 ADVCA_SSD_BlockDataDecrypt
(
    HI_U8*      pu8BlockData,
    HI_U32      u32BlockSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
)
{

}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

