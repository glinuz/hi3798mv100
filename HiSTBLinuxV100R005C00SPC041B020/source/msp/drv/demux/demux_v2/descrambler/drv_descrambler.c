/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : drv_descrambler.c
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/major.h>
#include <asm/types.h>
#include <linux/fs.h>
#include <asm/atomic.h>

#include "hi_type.h"
#include "hi_drv_stat.h"
#include "hi_kernel_adapt.h"

#include "hi_unf_descrambler.h"
#include "hi_mpi_descrambler.h"
#include "hi_drv_descrambler.h"

#include "demux_debug.h"
#include "drv_demux_ioctl.h"
#include "drv_demux_config.h"
#include "drv_demux_define.h"
#include "drv_descrambler.h"
#include "drv_descrambler_func.h"
#include "drv_descrambler_ioctl.h"

HI_S32 DMXKeyIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_KEYS_NEW:
        {
            DMX_NewKey_S *Param = (DMX_NewKey_S*)arg;

            ret = HI_DRV_DMX_CreateDescrambler(Param->DmxId, &Param->DesramblerAttr, &Param->KeyHandle, file);

            break;
        }

        case CMD_DEMUX_KEYS_DEL:
        {
            ret = HI_DRV_DMX_DestroyDescrambler(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_KEYS_GET_DESCRAMBLERATTR:
        {
            DMX_DescramblerAttr_S *param = (DMX_DescramblerAttr_S *)arg;
            ret = HI_DRV_DMX_GetDescramblerAttr(param->hKeyHandle, &param->stDescramblerAttr);

            break;
        }

        case CMD_DEMUX_KEYS_SET_DESCRAMBLERATTR:
        {
            DMX_DescramblerAttr_S *param = (DMX_DescramblerAttr_S *)arg;
            ret = HI_DRV_DMX_SetDescramblerAttr(param->hKeyHandle, &param->stDescramblerAttr);

            break;
        }

        case CMD_DEMUX_KEYS_SET_EVEN:
        {
            DMX_KeySet_S *Param = (DMX_KeySet_S*)arg;

            ret = HI_DRV_DMX_SetDescramblerEvenKey(Param->KeyHandle, Param->Key);

            HI_DRV_STAT_Event(STAT_EVENT_CWSET, 0);

            break;
        }

        case CMD_DEMUX_KEYS_SET_ODD:
        {
            DMX_KeySet_S *Param = (DMX_KeySet_S*)arg;

            ret = HI_DRV_DMX_SetDescramblerOddKey(Param->KeyHandle, Param->Key);

            break;
        }
        
        case CMD_DEMUX_KEYS_SET_IVEVEN:
        {
            DMX_KeySet_S *Param = (DMX_KeySet_S*)arg;

            ret = HI_DRV_DMX_SetDescramblerEvenIVKey(Param->KeyHandle, Param->Key);

            break;
        }

        case CMD_DEMUX_KEYS_SET_IVODD:
        {
            DMX_KeySet_S *Param = (DMX_KeySet_S*)arg;

            ret = HI_DRV_DMX_SetDescramblerOddIVKey(Param->KeyHandle, Param->Key);

            break;
        }

        case CMD_DEMUX_KEYS_ATTACH:
        {
            DMX_KeyAttach_S *Param = (DMX_KeyAttach_S*)arg;

            ret = HI_DRV_DMX_AttachDescrambler(Param->KeyHandle, Param->ChanHandle);

            break;
        }

        case CMD_DEMUX_KEYS_DETACH:
        {
            DMX_KeyAttach_S *Param = (DMX_KeyAttach_S*)arg;

            ret = HI_DRV_DMX_DetachDescrambler(Param->KeyHandle, Param->ChanHandle);

            break;
        }

        case CMD_DEMUX_KEYS_GET_ID:
        {
            DMX_KeyAttach_S *Param = (DMX_KeyAttach_S*)arg;

            ret = HI_DRV_DMX_GetDescramblerKeyHandle(Param->ChanHandle, &Param->KeyHandle);

            break;
        }

        case CMD_DEMUX_KEYS_GET_FREE:
        {
            DMX_FreeKeyGet_S *Param = (DMX_FreeKeyGet_S*)arg;

            ret = HI_DRV_DMX_GetFreeDescramblerKeyCount(Param->DmxId, &Param->FreeCount);

            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

HI_VOID DmxDestroyAllDescrambler(Dmx_Set_S *DmxSet, struct file * file)
{
    HI_U32 KeyId;

    mutex_lock(&DmxSet->LockAllKey);
    
    for_each_set_bit(KeyId, DmxSet->KeyBitmap, DmxSet->DmxKeyCnt)
    {
        DMX_KeyInfo_S  *KeyInfo = &DmxSet->DmxKeyInfo[KeyId];

        if (KeyInfo->Owner == (pid_t)(unsigned long)file->private_data)
        {
            HI_DRV_DMX_DestroyDescrambler(DMX_KEYHANDLE(KeyInfo->DmxId, KeyId));
        }
    }

    mutex_unlock(&DmxSet->LockAllKey);
}


HI_S32 HI_DRV_DMX_CreateDescrambler(HI_U32 DmxId, HI_UNF_DMX_DESCRAMBLER_ATTR_S *DescAttr, HI_HANDLE *KeyHandle, HI_VOID *file)
{
    HI_S32 ret;
    HI_U32 KeyId;

    CHECKPOINTER(DescAttr);
    CHECKPOINTER(KeyHandle);

    ret = DMX_OsiDescramblerCreate(DmxId, &KeyId, DescAttr);
    if (HI_SUCCESS == ret)
    {
        *KeyHandle = DMX_KEYHANDLE(DmxId, KeyId);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_DestroyDescrambler(HI_HANDLE KeyHandle)
{
    HI_U32  KeyId, DmxId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);

    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);

    return DMX_OsiDescramblerDestroy(DmxId, KeyId);
}

HI_S32 HI_DRV_DMX_GetDescramblerAttr(HI_HANDLE KeyHandle, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDescramblerAttr)
{
    HI_U32  KeyId, DmxId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);

    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);

    return DMX_OsiDescramblerGetAttr(DmxId, KeyId, pstDescramblerAttr);
}

HI_S32 HI_DRV_DMX_SetDescramblerAttr(HI_HANDLE KeyHandle, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDescramblerAttr)
{
    HI_U32  KeyId, DmxId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);
    
    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);

    return DMX_OsiDescramblerSetAttr(DmxId, KeyId, pstDescramblerAttr);
}

HI_S32 HI_DRV_DMX_SetDescramblerEvenKey(HI_HANDLE KeyHandle, HI_U8 *Key)
{
    HI_U32  KeyId, DmxId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);

    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);

    return DMX_OsiDescramblerSetKey(DmxId, KeyId, DMX_KEY_TYPE_EVEN, Key);
}

HI_S32 HI_DRV_DMX_SetDescramblerOddKey(HI_HANDLE KeyHandle, HI_U8 *Key)
{
    HI_U32  KeyId, DmxId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);
    
    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);

    return DMX_OsiDescramblerSetKey(DmxId, KeyId, DMX_KEY_TYPE_ODD, Key);
}
HI_S32 HI_DRV_DMX_SetDescramblerEvenIVKey(HI_HANDLE KeyHandle, HI_U8 *Key)
{
    HI_U32  KeyId, DmxId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);

    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);

    return DMX_OsiDescramblerSetIVKey(DmxId, KeyId, DMX_KEY_TYPE_EVEN, Key);
}

HI_S32 HI_DRV_DMX_SetDescramblerOddIVKey(HI_HANDLE KeyHandle, HI_U8 *Key)
{
    HI_U32  KeyId, DmxId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);

    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);

    return DMX_OsiDescramblerSetIVKey(DmxId, KeyId, DMX_KEY_TYPE_ODD, Key);
}

HI_S32 HI_DRV_DMX_AttachDescrambler(HI_HANDLE KeyHandle, HI_HANDLE ChanHandle)
{
    HI_U32  KeyId, DmxId, ChanId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);
    DMX_CHECK_CHANHANDLE(ChanHandle);

    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);
    ChanId = DMX_CHANID(ChanHandle);

    return DMX_OsiDescramblerAttach(DmxId, KeyId, ChanId);
}

HI_S32 HI_DRV_DMX_DetachDescrambler(HI_HANDLE KeyHandle, HI_HANDLE ChanHandle)
{
    HI_U32  KeyId, DmxId, ChanId;
    
    DMX_CHECK_KEYHANDLE(KeyHandle);
    DMX_CHECK_CHANHANDLE(ChanHandle);

    DmxId = DMX_DMXID(KeyHandle);
    KeyId = DMX_KEYID(KeyHandle);
    ChanId = DMX_CHANID(ChanHandle);

    return DMX_OsiDescramblerDetach(DmxId, KeyId, ChanId);
}

HI_S32 HI_DRV_DMX_GetDescramblerKeyHandle(HI_HANDLE ChanHandle, HI_HANDLE *KeyHandle)
{
    HI_S32  ret;
    HI_U32  KeyId, DmxId, ChanId;

    CHECKPOINTER(KeyHandle);
    DMX_CHECK_CHANHANDLE(ChanHandle);

    DmxId = DMX_DMXID(ChanHandle);
    ChanId = DMX_CHANID(ChanHandle);

    ret = DMX_OsiDescramblerGetKeyId(DmxId, ChanId, &KeyId);
    if (HI_SUCCESS == ret)
    {
        *KeyHandle = DMX_KEYHANDLE(DmxId, KeyId);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_GetFreeDescramblerKeyCount(HI_U32 DmxId, HI_U32 *FreeCount)
{
    CHECKPOINTER(FreeCount);

    return DMX_OsiDescramblerGetFreeKeyNum(DmxId, FreeCount);
}
//for OpenTV5
EXPORT_SYMBOL(HI_DRV_DMX_CreateDescrambler);
EXPORT_SYMBOL(HI_DRV_DMX_DestroyDescrambler);
EXPORT_SYMBOL(HI_DRV_DMX_SetDescramblerEvenKey);
EXPORT_SYMBOL(HI_DRV_DMX_SetDescramblerOddKey);
EXPORT_SYMBOL(HI_DRV_DMX_SetDescramblerEvenIVKey);
EXPORT_SYMBOL(HI_DRV_DMX_SetDescramblerOddIVKey);
EXPORT_SYMBOL(HI_DRV_DMX_AttachDescrambler);
EXPORT_SYMBOL(HI_DRV_DMX_DetachDescrambler);
EXPORT_SYMBOL(HI_DRV_DMX_GetDescramblerKeyHandle);
EXPORT_SYMBOL(HI_DRV_DMX_GetFreeDescramblerKeyCount);


