/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : drv_descrambler_ioctl.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#ifndef __DRV_DESCRAMBLER_IOCTL_H__
#define __DRV_DESCRAMBLER_IOCTL_H__

#include "hi_type.h"

#include "hi_unf_descrambler.h"

typedef struct
{
    HI_U32                          DmxId;
    HI_UNF_DMX_DESCRAMBLER_ATTR_S   DesramblerAttr;
    HI_HANDLE                       KeyHandle;
} DMX_NewKey_S;

typedef struct
{
    HI_HANDLE                       hKeyHandle;
    HI_UNF_DMX_DESCRAMBLER_ATTR_S   stDescramblerAttr;
} DMX_DescramblerAttr_S;


#define DMX_KEY_SIZE    16

typedef struct
{
    HI_HANDLE   KeyHandle;
    HI_U8       Key[DMX_KEY_SIZE];
} DMX_KeySet_S;

typedef struct
{
    HI_HANDLE   KeyHandle;
    HI_HANDLE   ChanHandle;
} DMX_KeyAttach_S;

typedef struct
{
    HI_U32  DmxId;
    HI_U32  FreeCount;
} DMX_FreeKeyGet_S;

#define CMD_DEMUX_KEYS_NEW                  _IOWR(HI_ID_DEMUX, 0x50, DMX_NewKey_S)              /* apply for a free key */
#define CMD_DEMUX_KEYS_DEL                  _IOW (HI_ID_DEMUX, 0x51, HI_HANDLE)                 /* delete an allocated key */
#define CMD_DEMUX_KEYS_SET_EVEN             _IOW (HI_ID_DEMUX, 0x52, DMX_KeySet_S)              /* set even key */
#define CMD_DEMUX_KEYS_SET_ODD              _IOW (HI_ID_DEMUX, 0x53, DMX_KeySet_S)              /* set odd key */
#define CMD_DEMUX_KEYS_ATTACH               _IOW (HI_ID_DEMUX, 0x54, DMX_KeyAttach_S)           /* attach a key to a channel */
#define CMD_DEMUX_KEYS_DETACH               _IOW (HI_ID_DEMUX, 0x55, DMX_KeyAttach_S)           /* detach a key from a channel */
#define CMD_DEMUX_KEYS_GET_ID               _IOWR(HI_ID_DEMUX, 0x56, DMX_KeyAttach_S)           /* get key id on a channel */
#define CMD_DEMUX_KEYS_GET_FREE             _IOWR(HI_ID_DEMUX, 0x57, DMX_FreeKeyGet_S)          /* get free key count */
#define CMD_DEMUX_KEYS_SET_IVEVEN           _IOW (HI_ID_DEMUX, 0x58, DMX_KeySet_S)              /* set iv even key */
#define CMD_DEMUX_KEYS_SET_IVODD            _IOW (HI_ID_DEMUX, 0x59, DMX_KeySet_S)              /* set iv odd key */
#define CMD_DEMUX_KEYS_GET_DESCRAMBLERATTR  _IOWR (HI_ID_DEMUX, 0x5a, DMX_DescramblerAttr_S)     /* get descrambler attr */
#define CMD_DEMUX_KEYS_SET_DESCRAMBLERATTR  _IOW (HI_ID_DEMUX, 0x5b, DMX_DescramblerAttr_S)     /* set descrambler attr */

#endif  // __DRV_DESCRAMBLER_IOCTL_H__

