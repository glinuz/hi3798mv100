/***********************************************************************************
*              Copyright 2015 - 2016, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_scrambler.c
* Description:  Define unf implement of demux scrambler.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20160512    NA                 NULL      Create this file.
***********************************************************************************/

#include "hi_tee_scrambler.h"
#include "sre_syscalls_chip.h"

#define HI_FATAL_DEMUX(fmt, args...)    uart_printf_func("[%s]:%s[%d]:" fmt "", "FATAL-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_ERR_DEMUX(fmt, args...)      uart_printf_func("[%s]:%s[%d]:" fmt "", "ERROR-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_WARN_DEMUX(fmt, args...)     uart_printf_func("[%s]:%s[%d]:" fmt "", "WARN-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_INFO_DEMUX(fmt, args...)      
#define HI_DBG_DEMUX(fmt, args...)     

HI_S32 HI_TEE_DMX_GetScramblerDefaultAttr(HI_TEE_DMX_SCRAMBLER_ATTR_S *pstScramblerAttr)
{
    if (!pstScramblerAttr)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }

    pstScramblerAttr->enScramblerType = HI_TEE_DMX_SCRAMBLER_TYPE_AES_ECB;
    pstScramblerAttr->enActiveKey     = HI_TEE_DMX_SCRAMBLER_EVEN_KEY;
    
    return HI_SUCCESS;
}

HI_S32 HI_TEE_DMX_CreateScrambler(HI_U32 u32DmxId, const HI_TEE_DMX_SCRAMBLER_ATTR_S *pstScramblerAttr, HI_HANDLE *phKey)
{
    if (!pstScramblerAttr || !phKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_CreateScrambler(u32DmxId, pstScramblerAttr, phKey);
}

HI_S32 HI_TEE_DMX_DestroyScrambler(HI_HANDLE hKey)
{
    return __TEE_DRV_DMX_DestroyScrambler(hKey);
}

HI_S32 HI_TEE_DMX_GetScramblerAttr(HI_HANDLE hKey, HI_TEE_DMX_SCRAMBLER_ATTR_S *pstAttr)
{
    if (!pstAttr)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }

    return __TEE_DRV_DMX_GetScramblerAttr(hKey, pstAttr);
}

HI_S32 HI_TEE_DMX_SetScramblerAttr(HI_HANDLE hKey, const HI_TEE_DMX_SCRAMBLER_ATTR_S *pstAttr)
{
    if (!pstAttr)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_SetScramblerAttr(hKey, pstAttr);
}

HI_S32 HI_TEE_DMX_SetScramblerEvenKey(HI_HANDLE hKey, const HI_U8 *pu8EvenKey)
{
    if (!pu8EvenKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_SetScramblerEvenKey(hKey, pu8EvenKey);
}

HI_S32 HI_TEE_DMX_SetScramblerOddKey(HI_HANDLE hKey, const HI_U8 *pu8OddKey)
{
    if (!pu8OddKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_SetScramblerOddKey(hKey, pu8OddKey);
}

HI_S32 HI_TEE_DMX_AttachScrambler(HI_HANDLE hKey, HI_HANDLE hChannel)
{
    return __TEE_DRV_DMX_AttachScrambler(hKey, hChannel);
}

HI_S32 HI_TEE_DMX_DetachScrambler(HI_HANDLE hKey, HI_HANDLE hChannel)
{
    return __TEE_DRV_DMX_DetachScrambler(hKey, hChannel);
}

HI_S32 HI_TEE_DMX_GetScramblerKeyHandle(HI_HANDLE hChannel, HI_HANDLE *phKey)
{
    if (!phKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        return HI_ERR_DMX_INVALID_PARA;  
    }
    
    return __TEE_DRV_DMX_GetScramblerKeyHandle(hChannel, phKey);
}

