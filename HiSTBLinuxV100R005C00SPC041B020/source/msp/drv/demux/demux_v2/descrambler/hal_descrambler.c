/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hal_descrambler.c
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#include "hi_type.h"

#include "hi_unf_descrambler.h"

#include "demux_debug.h"
#include "drv_demux_config.h"
#include "drv_demux_reg.h"
#include "drv_descrambler_reg.h"
#include "hal_descrambler.h"

/***********************************************************************************
* Function      : DmxHalSetChannelCWIndex
* Description   : Set Channel CW Index
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelCWIndex(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 cwIndex)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);
    BUG_ON(cwIndex >= DmxSet->DmxKeyCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    dmx_pidctrl.bits.cw_index = cwIndex;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);
}

/***********************************************************************************
* Function      : DmxHalSetChannelDsc
* Description   : Enable Channel Dsc
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetChannelDsc(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_BOOL Enable)
{
    U_DMX_PID_CTRL dmx_pidctrl;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    dmx_pidctrl.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId));
    dmx_pidctrl.bits.do_scram = Enable;
    DMX_WRITE_REG(DmxSet->IoBase, DMX_PID_CTRL(ChanId), dmx_pidctrl.u32);
}

HI_U32 DmxHalGetOptCAType(Dmx_Set_S * DmxSet)
{
    return DMX_READ_REG(DmxSet->IoBase, CA_INFO0);
}

/***********************************************************************************
* Function      : DmxHalSetEntropyReduction
* Description   : Set the valid bits of cw
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetEntropyReduction(Dmx_Set_S *DmxSet, HI_U32 KeyId, HI_UNF_DMX_CA_ENTROPY_E EntropyReduction)
{
    HI_U32 Value;

    Value = DMX_READ_REG(DmxSet->IoBase, CA_ENTROPY);
    if (HI_UNF_DMX_CA_ENTROPY_REDUCTION_CLOSE == EntropyReduction)
    {
        Value |= (1 << KeyId);
    }
    else
    {
        Value &= ~(1 << KeyId);
    }

    DMX_WRITE_REG(DmxSet->IoBase, CA_ENTROPY, Value);
}

/***********************************************************************************
* Function      : DmxHalInitSpeCWOrder
* Description   : set ca type spe cw order as byte and word change
* Input         :
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalInitSpeCWOrder(Dmx_Set_S * DmxSet)
{
    HI_U32 Value;

    Value = DMX_READ_REG(DmxSet->IoBase, DMX_CW_CTRL0);

    Value |= 0x33300;   //for spe type:byte and word change
    DMX_WRITE_REG(DmxSet->IoBase, DMX_CW_CTRL0, Value);
}

/***********************************************************************************
* Function      : DmxHalInitTdesCWOrder
* Description   : set ca type tdes cw order as byte and word change
* Input         :
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalInitTdesCWOrder(Dmx_Set_S * DmxSet)
{
    HI_U32 Value;

    Value = DMX_READ_REG(DmxSet->IoBase, DMX_CW_CTRL2);

    Value |= 0x333;   //for tdes ecb and tdes cbc and tdes iptv:byte and word change
    DMX_WRITE_REG(DmxSet->IoBase, DMX_CW_CTRL2, Value);
}

HI_VOID DmxHalInitDesCWOrder(Dmx_Set_S * DmxSet)
{
    HI_U32 Value;

    Value = DMX_READ_REG(DmxSet->IoBase, DMX_CW_CTRL0);

    Value |= 0x3300000;   //for des ecb(ci) and des cbc :byte and word change
    DMX_WRITE_REG(DmxSet->IoBase, DMX_CW_CTRL0, Value);
}

/***********************************************************************************
* Function      : DmxHalSetCWWord1
* Description   : Set CW Word
* Input         :
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalSetCWWord(Dmx_Set_S *DmxSet,
        HI_U32                          GroupId,
        HI_U32                          WordId,
        HI_U32                          Key,
        DMX_KEY_TYPE_E                  KeyType,
        DMX_CW_TYPE                     CWType,
        HI_UNF_DMX_DESCRAMBLER_TYPE_E   DescType
    )
{
    U_CW_SET1   reg;
    HI_U32      type;

    BUG_ON(GroupId >= DmxSet->DmxKeyCnt);

    switch (DescType)
    {
        case HI_UNF_DMX_DESCRAMBLER_TYPE_CSA3 :
            type = 0x10;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_AES_IPTV :
            type = 0x20;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_AES_ECB :
            type = 0x21;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_AES_CI :
        case HI_UNF_DMX_DESCRAMBLER_TYPE_AES_CBC :
            type = 0x22;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_DES_IPTV :
            type = 0x30;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_DES_CI :
            type = 0x32;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_DES_CBC :
            type = 0x33;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_AES_NS :
            type = 0x40;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_SMS4_NS :
            type = 0x41;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_SMS4_IPTV :
            type = 0x50;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_SMS4_ECB :
            type = 0x51;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_SMS4_CBC :
            type = 0x53;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_TDES_IPTV :
            type = 0x70;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_TDES_ECB :
            type = 0x71;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_TDES_CBC :
            type = 0x73;
            break;

        case HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2 :
        default :
            type = 0;
    }

    //firstly, config the control register, and then config cw data register, logic map

    reg.value = DMX_READ_REG(DmxSet->IoBase, CW_SET);

    reg.bits.cw_word_id     = WordId;
    reg.bits.cw_odd_even    = (DMX_KEY_TYPE_ODD == KeyType) ? 1 : 0;
    reg.bits.cw_group_id    = GroupId;
    reg.bits.cw_type        = type;
    reg.bits.cw_iv_sel      = (DMX_KEY_IV == CWType) ? 1 : 0;

    DMX_WRITE_REG(DmxSet->IoBase, CW_SET, reg.value);

    DMX_WRITE_REG(DmxSet->IoBase, DMX_CW_DATA, Key);
}

/***********************************************************************************
* Function      :  DmxHalSetChanCwTabId
* Description   :
* Input         :
* Output        :
* Return        :
* Others        :
***********************************************************************************/
HI_VOID DmxHalSetChanCwTabId(Dmx_Set_S * DmxSet, HI_U32 ChanId, HI_U32 TabId)
{
    HI_U32 value;
    HI_U32 offset = ChanId & 0xF;

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);

    value = DMX_READ_REG(DmxSet->IoBase, CHAN_CW_TAB_ID(ChanId));

    value &= ~(0x3 << (offset * 2));
    value |= TabId << (offset * 2);

    DMX_WRITE_REG(DmxSet->IoBase, CHAN_CW_TAB_ID(ChanId), value);
}

/***********************************************************************************
* Function      : DmxHalSetDmxIvEnable
* Description   :
* Input         :
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetDmxIvEnable(Dmx_Set_S * DmxSet, HI_U32 RawDmxId, HI_BOOL Enable)
{
    U_DMX_CW_SET stDmxCWSet;

    BUG_ON(RawDmxId >= DmxSet->DmxCnt);

    stDmxCWSet.u32 = DMX_READ_REG(DmxSet->IoBase, DMX_CW_SET(RawDmxId));
    if (Enable != 0)
    {
        stDmxCWSet.bits.cw_iv_en = 1;
    }
    else
    {
        stDmxCWSet.bits.cw_iv_en = 0;
    }
    DMX_WRITE_REG(DmxSet->IoBase, DMX_CW_SET(RawDmxId), stDmxCWSet.u32);
}

/***********************************************************************************
* Function      :  DmxHalSetCSA3Reset
* Description   :
* Input         :  Enable 1 : set pvr csa3.0 soft reset
*                         0 : clear pvr csa3.0 soft reset
* Output        :
* Return        :
* Others:
***********************************************************************************/
HI_VOID DmxHalSetCSA3Reset(HI_BOOL Enable)
{
#if 0    
    U_PVR_CSA3_RST reg;

    reg.value = DMX_READ_REG(PVR_CSA3_RST);

    reg.bits.pvr_csa3_soft_rst = Enable;
    DMX_WRITE_REG(PVR_CSA3_RST, reg.value);
#endif    
}

#if defined(DMX_SECURE_KEY_SUPPORT) && !defined(DMX_TEE_SUPPORT)
HI_VOID DmxHalSetSecureKey(Dmx_Set_S *DmxSet, HI_U32 KeyId)
{
    HI_U32 value;

    BUG_ON(KeyId >= DmxSet->DmxKeyCnt);

    value = DMX_READ_REG(DmxSet->IoBase, CW_SECURE_FLAG);
    value |= (1 << KeyId);
    DMX_WRITE_REG(DmxSet->IoBase, CW_SECURE_FLAG, value);
}
#endif

