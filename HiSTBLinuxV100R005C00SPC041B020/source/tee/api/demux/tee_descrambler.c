/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_descrambler.c
* Description:  Define unf implement of demux descrambler.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/

#include "hi_tee_descrambler.h"
#include "sre_syscalls_chip.h"

#define HI_FATAL_DEMUX(fmt, args...)    uart_printf_func("[%s]:%s[%d]:" fmt "", "FATAL-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_ERR_DEMUX(fmt, args...)      uart_printf_func("[%s]:%s[%d]:" fmt "", "ERROR-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_WARN_DEMUX(fmt, args...)     uart_printf_func("[%s]:%s[%d]:" fmt "", "WARN-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_INFO_DEMUX(fmt, args...)      
#define HI_DBG_DEMUX(fmt, args...)     

HI_S32 HI_TEE_DMX_CreateDescrambler(HI_U32 u32DmxId, const HI_TEE_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr, HI_HANDLE *phKey)
{
    if (!pstDesramblerAttr || !phKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_CreateDescrambler(u32DmxId, pstDesramblerAttr, phKey, HI_NULL);
}

HI_S32 HI_TEE_DMX_DestroyDescrambler(HI_HANDLE hKey)
{
    return __TEE_DRV_DMX_DestroyDescrambler(hKey);
}

HI_S32 HI_TEE_DMX_GetDescramblerAttr(HI_HANDLE hKey, HI_TEE_DMX_DESCRAMBLER_ATTR_S *pstAttr)
{
    if (!pstAttr)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }

    return __TEE_DRV_DMX_GetDescramblerAttr(hKey, pstAttr);
}


HI_S32 HI_TEE_DMX_SetDescramblerAttr(HI_HANDLE hKey, HI_TEE_DMX_DESCRAMBLER_ATTR_S *pstAttr)
{
    if (!pstAttr)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_SetDescramblerAttr(hKey, pstAttr);
}

HI_S32 HI_TEE_DMX_SetDescramblerEvenKey(HI_HANDLE hKey, const HI_U8 *pu8EvenKey)
{
    if (!pu8EvenKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_SetDescramblerEvenKey(hKey, pu8EvenKey);
}

HI_S32 HI_TEE_DMX_SetDescramblerOddKey(HI_HANDLE hKey, const HI_U8 *pu8OddKey)
{
    if (!pu8OddKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_SetDescramblerOddKey(hKey, pu8OddKey);
}

HI_S32 HI_TEE_DMX_SetDescramblerEvenIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey)
{
    if (!pu8IVKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_SetDescramblerEvenIVKey(hKey, pu8IVKey);
}

HI_S32 HI_TEE_DMX_SetDescramblerOddIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey)
{
    if (!pu8IVKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_SetDescramblerOddIVKey(hKey, pu8IVKey);
}

HI_S32 HI_TEE_DMX_AttachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel)
{
    return __TEE_DRV_DMX_AttachDescrambler(hKey, hChannel);
}

HI_S32 HI_TEE_DMX_DetachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel)
{
    return __TEE_DRV_DMX_DetachDescrambler(hKey, hChannel);
}

HI_S32 HI_TEE_DMX_GetDescramblerKeyHandle(HI_HANDLE hChannel, HI_HANDLE *phKey)
{
    if (!phKey)
    {
        HI_ERR_DEMUX("Invalid parameter.\n");
        return HI_ERR_DMX_INVALID_PARA;  
    }
    
    return __TEE_DRV_DMX_GetDescramblerKeyHandle(hChannel, phKey);
}

HI_S32 HI_TEE_DMX_GetChannelHandle(HI_U32 u32DmxId, HI_U32 u32Pid, HI_HANDLE *phChannel)
{
    if (!phChannel)
    {
        HI_ERR_DEMUX("Invalid parameter.\n"); 
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    return __TEE_DRV_DMX_GetChannelHandle(u32DmxId, u32Pid, phChannel);
}
