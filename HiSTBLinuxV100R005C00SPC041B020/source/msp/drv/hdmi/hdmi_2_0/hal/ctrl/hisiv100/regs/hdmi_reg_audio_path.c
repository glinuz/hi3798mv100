// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  audio_path_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  w00226427
// Version       :  1.0
// Date          :  Tester
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00271848 2015/05/23 11:11:23 Create file
// ******************************************************************************


#include "hi_type.h"
#include "hdmi_reg_audio_path.h"
#include "hdmi_product_define.h"
#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif

/* Define the struct pointor of the module audio_path_reg */
volatile S_audio_path_reg_REGS_TYPE *gopaudio_path_regAllReg = NULL;

int HDMI_TX_S_audio_path_reg_REGS_TYPE_Init(void)
{
    if (gopaudio_path_regAllReg)
    {
        return HI_SUCCESS;
    }
    
    gopaudio_path_regAllReg = (volatile S_audio_path_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_audio_path_reg,4);

    if (!gopaudio_path_regAllReg)
    {
        HDMI_ERR("gopaudio_path_regAllReg = null,fail!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

int HDMI_TX_S_audio_path_reg_REGS_TYPE_DeInit(void)
{
    if (gopaudio_path_regAllReg)
    {
        HDMI_IO_UNMAP(gopaudio_path_regAllReg);
        gopaudio_path_regAllReg = HI_NULL;
    }
    
    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_fifo3_mapSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_fifo3_map
//  Input       : unsigned int uaud_fifo3_map: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_fifo3_mapSet(unsigned int uaud_fifo3_map)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_fifo3_map = uaud_fifo3_map;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_fifo2_mapSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_fifo2_map
//  Input       : unsigned int uaud_fifo2_map: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_fifo2_mapSet(unsigned int uaud_fifo2_map)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_fifo2_map = uaud_fifo2_map;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_fifo1_mapSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_fifo1_map
//  Input       : unsigned int uaud_fifo1_map: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_fifo1_mapSet(unsigned int uaud_fifo1_map)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_fifo1_map = uaud_fifo1_map;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_fifo0_mapSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_fifo0_map
//  Input       : unsigned int uaud_fifo0_map: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_fifo0_mapSet(unsigned int uaud_fifo0_map)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_fifo0_map = uaud_fifo0_map;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_src_ctrlSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_src_ctrl
//  Input       : unsigned int uaud_src_ctrl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_src_ctrlSet(unsigned int uaud_src_ctrl)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_src_ctrl = uaud_src_ctrl;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_src_enSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_src_en
//  Input       : unsigned int uaud_src_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_src_enSet(unsigned int uaud_src_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_src_en = uaud_src_en;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_spdif_enSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_spdif_en
//  Input       : unsigned int uaud_spdif_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_spdif_enSet(unsigned int uaud_spdif_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_spdif_en = uaud_spdif_en;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_i2s_enSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_i2s_en
//  Input       : unsigned int uaud_i2s_en: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_i2s_enSet(unsigned int uaud_i2s_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_i2s_en = uaud_i2s_en;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_layoutSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_layout
//  Input       : unsigned int uaud_layout: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_layoutSet(unsigned int uaud_layout)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_layout = uaud_layout;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_mute_enSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_mute_en
//  Input       : unsigned int uaud_mute_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_mute_enSet(unsigned int uaud_mute_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_mute_en = uaud_mute_en;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_in_enSet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_in_en
//  Input       : unsigned int uaud_in_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_in_enSet(unsigned int uaud_in_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_ctrl.bits.aud_in_en = uaud_in_en;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_ctrl.u32, "TX_AUDIO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_ch_swapSet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_ch_swap
//  Input       : unsigned int ui2s_ch_swap: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_ch_swapSet(unsigned int ui2s_ch_swap)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_i2s_ctrl.bits.i2s_ch_swap = ui2s_ch_swap;
    HDMITXRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32, "AUD_I2S_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_lengthSet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_length
//  Input       : unsigned int ui2s_length: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_lengthSet(unsigned int ui2s_length)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_i2s_ctrl.bits.i2s_length = ui2s_length;
    HDMITXRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32, "AUD_I2S_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_vbitSet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_vbit
//  Input       : unsigned int ui2s_vbit: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_vbitSet(unsigned int ui2s_vbit)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_i2s_ctrl.bits.i2s_vbit = ui2s_vbit;
    HDMITXRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32, "AUD_I2S_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_data_dirSet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_data_dir
//  Input       : unsigned int ui2s_data_dir: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_data_dirSet(unsigned int ui2s_data_dir)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_i2s_ctrl.bits.i2s_data_dir = ui2s_data_dir;
    HDMITXRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32, "AUD_I2S_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_justifySet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_justify
//  Input       : unsigned int ui2s_justify: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_justifySet(unsigned int ui2s_justify)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_i2s_ctrl.bits.i2s_justify = ui2s_justify;
    HDMITXRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32, "AUD_I2S_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_ws_polaritySet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_ws_polarity
//  Input       : unsigned int ui2s_ws_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_ws_polaritySet(unsigned int ui2s_ws_polarity)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_i2s_ctrl.bits.i2s_ws_polarity = ui2s_ws_polarity;
    HDMITXRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32, "AUD_I2S_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_1st_shiftSet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_1st_shift
//  Input       : unsigned int ui2s_1st_shift: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_1st_shiftSet(unsigned int ui2s_1st_shift)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_i2s_ctrl.bits.i2s_1st_shift = ui2s_1st_shift;
    HDMITXRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32, "AUD_I2S_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_hbra_onSet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_hbra_on
//  Input       : unsigned int ui2s_hbra_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_hbra_onSet(unsigned int ui2s_hbra_on)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_i2s_ctrl.bits.i2s_hbra_on = ui2s_hbra_on;
    HDMITXRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_i2s_ctrl.u32, "AUD_I2S_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_2ui_maxSet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_2ui_max
//  Input       : unsigned int uspdif_2ui_max: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_2ui_maxSet(unsigned int uspdif_2ui_max)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_spdif_ctrl.bits.spdif_2ui_max = uspdif_2ui_max;
    HDMITXRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32, "AUD_SPDIF_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_1ui_maxSet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_1ui_max
//  Input       : unsigned int uspdif_1ui_max: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_1ui_maxSet(unsigned int uspdif_1ui_max)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_spdif_ctrl.bits.spdif_1ui_max = uspdif_1ui_max;
    HDMITXRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32, "AUD_SPDIF_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_size_swSet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_size_sw
//  Input       : unsigned int uspdif_size_sw: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_size_swSet(unsigned int uspdif_size_sw)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_spdif_ctrl.bits.spdif_size_sw = uspdif_size_sw;
    HDMITXRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32, "AUD_SPDIF_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_err_threshSet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_err_thresh
//  Input       : unsigned int uspdif_err_thresh: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_err_threshSet(unsigned int uspdif_err_thresh)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_spdif_ctrl.bits.spdif_err_thresh = uspdif_err_thresh;
    HDMITXRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32, "AUD_SPDIF_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_fs_ovr_enSet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_fs_ovr_en
//  Input       : unsigned int uspdif_fs_ovr_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_fs_ovr_enSet(unsigned int uspdif_fs_ovr_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_spdif_ctrl.bits.spdif_fs_ovr_en = uspdif_fs_ovr_en;
    HDMITXRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32, "AUD_SPDIF_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_i2s_cbit_orderSet
//  Description : Set the value of the member AUD_SPDIF_CTRL.i2s_cbit_order
//  Input       : unsigned int ui2s_cbit_order: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_i2s_cbit_orderSet(unsigned int ui2s_cbit_order)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_spdif_ctrl.bits.i2s_cbit_order = ui2s_cbit_order;
    HDMITXRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32, "AUD_SPDIF_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_2ui_lockSet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_2ui_lock
//  Input       : unsigned int uspdif_2ui_lock: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_2ui_lockSet(unsigned int uspdif_2ui_lock)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_spdif_ctrl.bits.spdif_2ui_lock = uspdif_2ui_lock;
    HDMITXRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32, "AUD_SPDIF_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_1ui_lockSet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_1ui_lock
//  Input       : unsigned int uspdif_1ui_lock: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_1ui_lockSet(unsigned int uspdif_1ui_lock)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_spdif_ctrl.bits.spdif_1ui_lock = uspdif_1ui_lock;
    HDMITXRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_spdif_ctrl.u32, "AUD_SPDIF_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte3_clock_accuracySet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte3_clock_accuracy
//  Input       : unsigned int uchst_byte3_clock_accuracy: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte3_clock_accuracySet(unsigned int uchst_byte3_clock_accuracy)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg0.bits.chst_byte3_clock_accuracy = uchst_byte3_clock_accuracy;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32, "AUD_CHST_CFG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte3_fsSet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte3_fs
//  Input       : unsigned int uchst_byte3_fs: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte3_fsSet(unsigned int uchst_byte3_fs)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg0.bits.chst_byte3_fs = uchst_byte3_fs;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32, "AUD_CHST_CFG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte2Set
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte2
//  Input       : unsigned int uchst_byte2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte2Set(unsigned int uchst_byte2)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg0.bits.chst_byte2 = uchst_byte2;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32, "AUD_CHST_CFG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte1Set
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte1
//  Input       : unsigned int uchst_byte1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte1Set(unsigned int uchst_byte1)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg0.bits.chst_byte1 = uchst_byte1;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32, "AUD_CHST_CFG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte0_otherSet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte0_other
//  Input       : unsigned int uchst_byte0_other: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte0_otherSet(unsigned int uchst_byte0_other)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg0.bits.chst_byte0_other = uchst_byte0_other;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32, "AUD_CHST_CFG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte0_bSet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte0_b
//  Input       : unsigned int uchst_byte0_b: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte0_bSet(unsigned int uchst_byte0_b)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg0.bits.chst_byte0_b = uchst_byte0_b;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32, "AUD_CHST_CFG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte0_aSet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte0_a
//  Input       : unsigned int uchst_byte0_a: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte0_aSet(unsigned int uchst_byte0_a)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg0.bits.chst_byte0_a = uchst_byte0_a;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg0.u32, "AUD_CHST_CFG0");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG1_chst_byte5_6Set
//  Description : Set the value of the member AUD_CHST_CFG1.chst_aud_byte5_6
//  Input       : unsigned int uchst_byte5_6: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG1_chst_byte5_6Set(unsigned int uchst_byte5_6)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG1 o_aud_chst_cfg1;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG1.u32);
    o_aud_chst_cfg1.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg1.bits.chst_byte5_6 = uchst_byte5_6;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg1.u32, "AUD_CHST_CFG1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG1_chst_byte4_org_fsSet
//  Description : Set the value of the member AUD_CHST_CFG1.chst_byte4_org_fs
//  Input       : unsigned int uchst_byte4_org_fs: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG1_chst_byte4_org_fsSet(unsigned int uchst_byte4_org_fs)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG1 o_aud_chst_cfg1;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG1.u32);
    o_aud_chst_cfg1.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg1.bits.chst_byte4_org_fs = uchst_byte4_org_fs;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg1.u32, "AUD_CHST_CFG1");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG1_chst_byte4_lengthSet
//  Description : Set the value of the member AUD_CHST_CFG1.chst_byte4_length
//  Input       : unsigned int uchst_byte4_length: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG1_chst_byte4_lengthSet(unsigned int uchst_byte4_length)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG1 o_aud_chst_cfg1;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG1.u32);
    o_aud_chst_cfg1.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_chst_cfg1.bits.chst_byte4_length = uchst_byte4_length;
    HDMITXRegWrite(pu32RegAddr, o_aud_chst_cfg1.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_chst_cfg1.u32, "AUD_CHST_CFG1");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_spdif_max_2ui_stSet
//  Description : Set the value of the member TX_AUDIO_STATE.spdif_max_2ui_st
//  Input       : unsigned int uspdif_max_2ui_st: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_spdif_max_2ui_stSet(unsigned int uspdif_max_2ui_st)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_state.bits.spdif_max_2ui_st = uspdif_max_2ui_st;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_state.u32, "TX_AUDIO_STATE");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_spdif_max_1ui_stSet
//  Description : Set the value of the member TX_AUDIO_STATE.spdif_max_1ui_st
//  Input       : unsigned int uspdif_max_1ui_st: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_spdif_max_1ui_stSet(unsigned int uspdif_max_1ui_st)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_state.bits.spdif_max_1ui_st = uspdif_max_1ui_st;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_state.u32, "TX_AUDIO_STATE");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_aud_spdif_fsSet
//  Description : Set the value of the member TX_AUDIO_STATE.aud_spdif_fs
//  Input       : unsigned int uaud_spdif_fs: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_aud_spdif_fsSet(unsigned int uaud_spdif_fs)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_state.bits.aud_spdif_fs = uaud_spdif_fs;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_state.u32, "TX_AUDIO_STATE");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_aud_lengthSet
//  Description : Set the value of the member TX_AUDIO_STATE.aud_length
//  Input       : unsigned int uaud_length: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_aud_lengthSet(unsigned int uaud_length)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_state.bits.aud_length = uaud_length;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_state.u32, "TX_AUDIO_STATE");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_aud_spdif_new_fsSet
//  Description : Set the value of the member TX_AUDIO_STATE.aud_spdif_new_fs
//  Input       : unsigned int uaud_spdif_new_fs: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_aud_spdif_new_fsSet(unsigned int uaud_spdif_new_fs)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_state.bits.aud_spdif_new_fs = uaud_spdif_new_fs;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_state.u32, "TX_AUDIO_STATE");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_aud_inavailableSet
//  Description : Set the value of the member TX_AUDIO_STATE.aud_inavailable
//  Input       : unsigned int uaud_inavailable: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_aud_inavailableSet(unsigned int uaud_inavailable)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_audio_state.bits.aud_inavailable = uaud_inavailable;
    HDMITXRegWrite(pu32RegAddr, o_tx_audio_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_audio_state.u32, "TX_AUDIO_STATE");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_FIFO_CTRL_aud_fifo_ptr_diffSet
//  Description : Set the value of the member AUD_FIFO_CTRL.aud_fifo_ptr_diff
//  Input       : unsigned int uaud_fifo_ptr_diff: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_FIFO_CTRL_aud_fifo_ptr_diffSet(unsigned int uaud_fifo_ptr_diff)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_FIFO_CTRL o_aud_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_FIFO_CTRL.u32);
    o_aud_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_fifo_ctrl.bits.aud_fifo_ptr_diff = uaud_fifo_ptr_diff;
    HDMITXRegWrite(pu32RegAddr, o_aud_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_fifo_ctrl.u32, "AUD_FIFO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_FIFO_CTRL_aud_fifo_hbr_maskSet
//  Description : Set the value of the member AUD_FIFO_CTRL.aud_fifo_hbr_mask
//  Input       : unsigned int uaud_fifo_hbr_mask: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_FIFO_CTRL_aud_fifo_hbr_maskSet(unsigned int uaud_fifo_hbr_mask)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_FIFO_CTRL o_aud_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_FIFO_CTRL.u32);
    o_aud_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_fifo_ctrl.bits.aud_fifo_hbr_mask = uaud_fifo_hbr_mask;
    HDMITXRegWrite(pu32RegAddr, o_aud_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_fifo_ctrl.u32, "AUD_FIFO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_FIFO_CTRL_aud_fifo_testSet
//  Description : Set the value of the member AUD_FIFO_CTRL.aud_fifo_test
//  Input       : unsigned int uaud_fifo_test: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_FIFO_CTRL_aud_fifo_testSet(unsigned int uaud_fifo_test)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_FIFO_CTRL o_aud_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_FIFO_CTRL.u32);
    o_aud_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_fifo_ctrl.bits.aud_fifo_test = uaud_fifo_test;
    HDMITXRegWrite(pu32RegAddr, o_aud_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_fifo_ctrl.u32, "AUD_FIFO_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_ave_enSet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_ave_en
//  Input       : unsigned int uacr_cts_ave_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_ave_enSet(unsigned int uacr_cts_ave_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_ctrl.bits.acr_cts_ave_en = uacr_cts_ave_en;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32, "AUD_ACR_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_use_sw_ctsSet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_use_sw_cts
//  Input       : unsigned int uacr_use_sw_cts: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_use_sw_ctsSet(unsigned int uacr_use_sw_cts)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_ctrl.bits.acr_use_sw_cts = uacr_use_sw_cts;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32, "AUD_ACR_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_flt_enSet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_flt_en
//  Input       : unsigned int uacr_cts_flt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_flt_enSet(unsigned int uacr_cts_flt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_ctrl.bits.acr_cts_flt_en = uacr_cts_flt_en;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32, "AUD_ACR_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_gen_selSet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_gen_sel
//  Input       : unsigned int uacr_cts_gen_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_gen_selSet(unsigned int uacr_cts_gen_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_ctrl.bits.acr_cts_gen_sel = uacr_cts_gen_sel;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32, "AUD_ACR_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_hw_sw_selSet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_hw_sw_sel
//  Input       : unsigned int uacr_cts_hw_sw_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_hw_sw_selSet(unsigned int uacr_cts_hw_sw_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_ctrl.bits.acr_cts_hw_sw_sel = uacr_cts_hw_sw_sel;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32, "AUD_ACR_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_req_enSet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_req_en
//  Input       : unsigned int uacr_cts_req_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_req_enSet(unsigned int uacr_cts_req_en)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_ctrl.bits.acr_cts_req_en = uacr_cts_req_en;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_ctrl.u32, "AUD_ACR_CTRL");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CFG_acr_cts_chg_threSet
//  Description : Set the value of the member AUD_ACR_CFG.acr_cts_chg_thre
//  Input       : unsigned int uacr_cts_chg_thre: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CFG_acr_cts_chg_threSet(unsigned int uacr_cts_chg_thre)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CFG o_aud_acr_cfg;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CFG.u32);
    o_aud_acr_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_cfg.bits.acr_cts_chg_thre = uacr_cts_chg_thre;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_cfg.u32, "AUD_ACR_CFG");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CFG_acr_cts_threSet
//  Description : Set the value of the member AUD_ACR_CFG.acr_cts_thre
//  Input       : unsigned int uacr_cts_thre: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CFG_acr_cts_threSet(unsigned int uacr_cts_thre)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CFG o_aud_acr_cfg;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CFG.u32);
    o_aud_acr_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_cfg.bits.acr_cts_thre = uacr_cts_thre;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_cfg.u32, "AUD_ACR_CFG");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CFG_acr_ave_maxSet
//  Description : Set the value of the member AUD_ACR_CFG.acr_ave_max
//  Input       : unsigned int uacr_ave_max: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CFG_acr_ave_maxSet(unsigned int uacr_ave_max)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CFG o_aud_acr_cfg;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CFG.u32);
    o_aud_acr_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_cfg.bits.acr_ave_max = uacr_ave_max;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_cfg.u32, "AUD_ACR_CFG");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_AUD_ACR_CFG_acr_fm_val_swSet
//  Description : Set the value of the member AUD_ACR_CFG.acr_fm_val_sw
//  Input       : unsigned int uacr_fm_val_sw: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CFG_acr_fm_val_swSet(unsigned int uacr_fm_val_sw)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CFG o_aud_acr_cfg;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CFG.u32);
    o_aud_acr_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_aud_acr_cfg.bits.acr_fm_val_sw = uacr_fm_val_sw;
    HDMITXRegWrite(pu32RegAddr, o_aud_acr_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_aud_acr_cfg.u32, "AUD_ACR_CFG");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_ACR_N_VAL_SW_acr_n_val_swSet
//  Description : Set the value of the member ACR_N_VAL_SW.acr_n_val_sw
//  Input       : unsigned int uacr_n_val_sw: 20 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ACR_N_VAL_SW_acr_n_val_swSet(unsigned int uacr_n_val_sw)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_ACR_N_VAL_SW o_acr_n_val_sw;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->ACR_N_VAL_SW.u32);
    o_acr_n_val_sw.u32 = HDMITXRegRead(pu32RegAddr);
    o_acr_n_val_sw.bits.acr_n_val_sw = uacr_n_val_sw;
    HDMITXRegWrite(pu32RegAddr, o_acr_n_val_sw.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_acr_n_val_sw.u32, "ACR_N_VAL_SW");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_ACR_CTS_VAL_SW_acr_cts_val_swSet
//  Description : Set the value of the member ACR_CTS_VAL_SW.acr_cts_val_sw
//  Input       : unsigned int uacr_cts_val_sw: 20 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ACR_CTS_VAL_SW_acr_cts_val_swSet(unsigned int uacr_cts_val_sw)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_ACR_CTS_VAL_SW o_acr_cts_val_sw;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->ACR_CTS_VAL_SW.u32);
    o_acr_cts_val_sw.u32 = HDMITXRegRead(pu32RegAddr);
    o_acr_cts_val_sw.bits.acr_cts_val_sw = uacr_cts_val_sw;
    HDMITXRegWrite(pu32RegAddr, o_acr_cts_val_sw.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_acr_cts_val_sw.u32, "ACR_CTS_VAL_SW");

    return HI_SUCCESS;
}
//******************************************************************************
//  Function    : HDMI_ACR_CTS_VAL_HW_acr_cts_val_hwSet
//  Description : Set the value of the member ACR_CTS_VAL_HW.acr_cts_val_hw
//  Input       : unsigned int uacr_cts_val_hw: 20 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ACR_CTS_VAL_HW_acr_cts_val_hwSet(unsigned int uacr_cts_val_hw)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_ACR_CTS_VAL_HW o_acr_cts_val_hw;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->ACR_CTS_VAL_HW.u32);
    o_acr_cts_val_hw.u32 = HDMITXRegRead(pu32RegAddr);
    o_acr_cts_val_hw.bits.acr_cts_val_hw = uacr_cts_val_hw;
    HDMITXRegWrite(pu32RegAddr, o_acr_cts_val_hw.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_acr_cts_val_hw.u32, "ACR_CTS_VAL_HW");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_fifo3_mapGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_fifo3_map
//  Input       : unsigned int uaud_fifo3_map: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_fifo3_mapGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_fifo3_map;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_fifo2_mapGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_fifo2_map
//  Input       : unsigned int uaud_fifo2_map: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_fifo2_mapGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_fifo2_map;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_fifo1_mapGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_fifo1_map
//  Input       : unsigned int uaud_fifo1_map: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_fifo1_mapGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_fifo1_map;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_fifo0_mapGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_fifo0_map
//  Input       : unsigned int uaud_fifo0_map: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_fifo0_mapGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_fifo0_map;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_src_ctrlGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_src_ctrl
//  Input       : unsigned int uaud_src_ctrl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_src_ctrlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_src_ctrl;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_src_enGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_src_en
//  Input       : unsigned int uaud_src_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_src_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_src_en;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_spdif_enGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_spdif_en
//  Input       : unsigned int uaud_spdif_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_spdif_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_spdif_en;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_i2s_enGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_i2s_en
//  Input       : unsigned int uaud_i2s_en: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_i2s_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_i2s_en;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_layoutGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_layout
//  Input       : unsigned int uaud_layout: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_layoutGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_layout;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_mute_enGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_mute_en
//  Input       : unsigned int uaud_mute_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_mute_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_mute_en;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_CTRL_aud_in_enGet
//  Description : Set the value of the member TX_AUDIO_CTRL.aud_in_en
//  Input       : unsigned int uaud_in_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_CTRL_aud_in_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_CTRL o_tx_audio_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_CTRL.u32);
    o_tx_audio_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_ctrl.bits.aud_in_en;
}

//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_ch_swapGet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_ch_swap
//  Input       : unsigned int ui2s_ch_swap: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_ch_swapGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_i2s_ctrl.bits.i2s_ch_swap;
}

//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_lengthGet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_length
//  Input       : unsigned int ui2s_length: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_lengthGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_i2s_ctrl.bits.i2s_length;
}

//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_vbitGet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_vbit
//  Input       : unsigned int ui2s_vbit: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_vbitGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_i2s_ctrl.bits.i2s_vbit;
}

//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_data_dirGet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_data_dir
//  Input       : unsigned int ui2s_data_dir: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_data_dirGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_i2s_ctrl.bits.i2s_data_dir;
}

//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_justifyGet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_justify
//  Input       : unsigned int ui2s_justify: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_justifyGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_i2s_ctrl.bits.i2s_justify;
}

//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_ws_polarityGet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_ws_polarity
//  Input       : unsigned int ui2s_ws_polarity: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_ws_polarityGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_i2s_ctrl.bits.i2s_ws_polarity;
}

//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_1st_shiftGet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_1st_shift
//  Input       : unsigned int ui2s_1st_shift: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_1st_shiftGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_i2s_ctrl.bits.i2s_1st_shift;
}

//******************************************************************************
//  Function    : HDMI_AUD_I2S_CTRL_i2s_hbra_onGet
//  Description : Set the value of the member AUD_I2S_CTRL.i2s_hbra_on
//  Input       : unsigned int ui2s_hbra_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_I2S_CTRL_i2s_hbra_onGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_I2S_CTRL o_aud_i2s_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_I2S_CTRL.u32);
    o_aud_i2s_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_i2s_ctrl.bits.i2s_hbra_on;
}

//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_2ui_maxGet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_2ui_max
//  Input       : unsigned int uspdif_2ui_max: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_2ui_maxGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_spdif_ctrl.bits.spdif_2ui_max;
}

//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_1ui_maxGet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_1ui_max
//  Input       : unsigned int uspdif_1ui_max: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_1ui_maxGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_spdif_ctrl.bits.spdif_1ui_max;
}

//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_size_swGet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_size_sw
//  Input       : unsigned int uspdif_size_sw: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_size_swGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_spdif_ctrl.bits.spdif_size_sw;
}

//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_err_threshGet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_err_thresh
//  Input       : unsigned int uspdif_err_thresh: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_err_threshGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_spdif_ctrl.bits.spdif_err_thresh;
}

//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_fs_ovr_enGet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_fs_ovr_en
//  Input       : unsigned int uspdif_fs_ovr_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_fs_ovr_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_spdif_ctrl.bits.spdif_fs_ovr_en;
}

//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_i2s_cbit_orderGet
//  Description : Set the value of the member AUD_SPDIF_CTRL.i2s_cbit_order
//  Input       : unsigned int ui2s_cbit_order: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_i2s_cbit_orderGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_spdif_ctrl.bits.i2s_cbit_order;
}

//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_2ui_lockGet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_2ui_lock
//  Input       : unsigned int uspdif_2ui_lock: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_2ui_lockGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_spdif_ctrl.bits.spdif_2ui_lock;
}

//******************************************************************************
//  Function    : HDMI_AUD_SPDIF_CTRL_spdif_1ui_lockGet
//  Description : Set the value of the member AUD_SPDIF_CTRL.spdif_1ui_lock
//  Input       : unsigned int uspdif_1ui_lock: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_SPDIF_CTRL_spdif_1ui_lockGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_SPDIF_CTRL o_aud_spdif_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_SPDIF_CTRL.u32);
    o_aud_spdif_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_spdif_ctrl.bits.spdif_1ui_lock;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte3_clock_accuracyGet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte3_clock_accuracy
//  Input       : unsigned int uchst_byte3_clock_accuracy: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte3_clock_accuracyGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg0.bits.chst_byte3_clock_accuracy;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte3_fsGet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte3_fs
//  Input       : unsigned int uchst_byte3_fs: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte3_fsGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
	 
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg0.bits.chst_byte3_fs;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte2Get
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte2
//  Input       : unsigned int uchst_byte2: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
	 
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg0.bits.chst_byte2;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte1Get
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte1
//  Input       : unsigned int uchst_byte1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
	 
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg0.bits.chst_byte1;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte0_otherGet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte0_other
//  Input       : unsigned int uchst_byte0_other: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte0_otherGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
	 
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg0.bits.chst_byte0_other;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte0_bGet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte0_b
//  Input       : unsigned int uchst_byte0_b: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte0_bGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
	 
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg0.bits.chst_byte0_b;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG0_chst_byte0_aGet
//  Description : Set the value of the member AUD_CHST_CFG0.chst_byte0_a
//  Input       : unsigned int uchst_byte0_a: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG0_chst_byte0_aGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG0 o_aud_chst_cfg0;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG0.u32);
    o_aud_chst_cfg0.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg0.bits.chst_byte0_a;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG1_chst_byte5_6Get
//  Description : Set the value of the member AUD_CHST_CFG1.chst_byte5_6
//  Input       : unsigned int uchst_byte5_6: 16 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG1_chst_byte5_6Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG1 o_aud_chst_cfg1;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG1.u32);
    o_aud_chst_cfg1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg1.bits.chst_byte5_6;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG1_chst_byte4_org_fsGet
//  Description : Set the value of the member AUD_CHST_CFG1.chst_byte4_org_fs
//  Input       : unsigned int uchst_byte4_org_fs: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG1_chst_byte4_org_fsGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG1 o_aud_chst_cfg1;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG1.u32);
    o_aud_chst_cfg1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg1.bits.chst_byte4_org_fs;
}

//******************************************************************************
//  Function    : HDMI_AUD_CHST_CFG1_chst_byte4_lengthGet
//  Description : Set the value of the member AUD_CHST_CFG1.chst_byte4_length
//  Input       : unsigned int uchst_byte4_length: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_CHST_CFG1_chst_byte4_lengthGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_CHST_CFG1 o_aud_chst_cfg1;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_CHST_CFG1.u32);
    o_aud_chst_cfg1.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_chst_cfg1.bits.chst_byte4_length;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_spdif_max_2ui_stGet
//  Description : Set the value of the member TX_AUDIO_STATE.spdif_max_2ui_st
//  Input       : unsigned int uspdif_max_2ui_st: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_spdif_max_2ui_stGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_state.bits.spdif_max_2ui_st;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_spdif_max_1ui_stGet
//  Description : Set the value of the member TX_AUDIO_STATE.spdif_max_1ui_st
//  Input       : unsigned int uspdif_max_1ui_st: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_spdif_max_1ui_stGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_state.bits.spdif_max_1ui_st;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_aud_spdif_fsGet
//  Description : Set the value of the member TX_AUDIO_STATE.aud_spdif_fs
//  Input       : unsigned int uaud_spdif_fs: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_aud_spdif_fsGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_state.bits.aud_spdif_fs;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_aud_lengthGet
//  Description : Set the value of the member TX_AUDIO_STATE.aud_length
//  Input       : unsigned int uaud_length: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_aud_lengthGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_state.bits.aud_length;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_aud_spdif_new_fsGet
//  Description : Set the value of the member TX_AUDIO_STATE.aud_spdif_new_fs
//  Input       : unsigned int uaud_spdif_new_fs: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_aud_spdif_new_fsGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_state.bits.aud_spdif_new_fs;
}

//******************************************************************************
//  Function    : HDMI_TX_AUDIO_STATE_aud_inavailableGet
//  Description : Set the value of the member TX_AUDIO_STATE.aud_inavailable
//  Input       : unsigned int uaud_inavailable: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_AUDIO_STATE_aud_inavailableGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_TX_AUDIO_STATE o_tx_audio_state;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->TX_AUDIO_STATE.u32);
    o_tx_audio_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_audio_state.bits.aud_inavailable;
}

//******************************************************************************
//  Function    : HDMI_AUD_FIFO_CTRL_aud_fifo_ptr_diffGet
//  Description : Set the value of the member AUD_FIFO_CTRL.aud_fifo_ptr_diff
//  Input       : unsigned int uaud_fifo_ptr_diff: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_FIFO_CTRL_aud_fifo_ptr_diffGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_FIFO_CTRL o_aud_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_FIFO_CTRL.u32);
    o_aud_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_fifo_ctrl.bits.aud_fifo_ptr_diff;
}

//******************************************************************************
//  Function    : HDMI_AUD_FIFO_CTRL_aud_fifo_hbr_maskGet
//  Description : Set the value of the member AUD_FIFO_CTRL.aud_fifo_hbr_mask
//  Input       : unsigned int uaud_fifo_hbr_mask: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_FIFO_CTRL_aud_fifo_hbr_maskGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_FIFO_CTRL o_aud_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_FIFO_CTRL.u32);
    o_aud_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_fifo_ctrl.bits.aud_fifo_hbr_mask;
}

//******************************************************************************
//  Function    : HDMI_AUD_FIFO_CTRL_aud_fifo_testGet
//  Description : Set the value of the member AUD_FIFO_CTRL.aud_fifo_test
//  Input       : unsigned int uaud_fifo_test: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_FIFO_CTRL_aud_fifo_testGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_FIFO_CTRL o_aud_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_FIFO_CTRL.u32);
    o_aud_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_fifo_ctrl.bits.aud_fifo_test;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_ave_enGet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_ave_en
//  Input       : unsigned int uacr_cts_ave_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_ave_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_ctrl.bits.acr_cts_ave_en;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_use_sw_ctsGet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_use_sw_cts
//  Input       : unsigned int uacr_use_sw_cts: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_use_sw_ctsGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_ctrl.bits.acr_use_sw_cts;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_flt_enGet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_flt_en
//  Input       : unsigned int uacr_cts_flt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_flt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_ctrl.bits.acr_cts_flt_en;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_gen_selGet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_gen_sel
//  Input       : unsigned int uacr_cts_gen_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_gen_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_ctrl.bits.acr_cts_gen_sel;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_hw_sw_selGet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_hw_sw_sel
//  Input       : unsigned int uacr_cts_hw_sw_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_hw_sw_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_ctrl.bits.acr_cts_hw_sw_sel;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CTRL_acr_cts_req_enGet
//  Description : Set the value of the member AUD_ACR_CTRL.acr_cts_req_en
//  Input       : unsigned int uacr_cts_req_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CTRL_acr_cts_req_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CTRL o_aud_acr_ctrl;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CTRL.u32);
    o_aud_acr_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_ctrl.bits.acr_cts_req_en;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CFG_acr_cts_chg_threGet
//  Description : Set the value of the member AUD_ACR_CFG.acr_cts_chg_thre
//  Input       : unsigned int uacr_cts_chg_thre: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CFG_acr_cts_chg_threGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CFG o_aud_acr_cfg;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CFG.u32);
    o_aud_acr_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_cfg.bits.acr_cts_chg_thre;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CFG_acr_cts_threGet
//  Description : Set the value of the member AUD_ACR_CFG.acr_cts_thre
//  Input       : unsigned int uacr_cts_thre: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CFG_acr_cts_threGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CFG o_aud_acr_cfg;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CFG.u32);
    o_aud_acr_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_cfg.bits.acr_cts_thre;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CFG_acr_ave_maxGet
//  Description : Set the value of the member AUD_ACR_CFG.acr_ave_max
//  Input       : unsigned int uacr_ave_max: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CFG_acr_ave_maxGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CFG o_aud_acr_cfg;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CFG.u32);
    o_aud_acr_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_cfg.bits.acr_ave_max;
}

//******************************************************************************
//  Function    : HDMI_AUD_ACR_CFG_acr_fm_val_swGet
//  Description : Set the value of the member AUD_ACR_CFG.acr_fm_val_sw
//  Input       : unsigned int uacr_fm_val_sw: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUD_ACR_CFG_acr_fm_val_swGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_AUD_ACR_CFG o_aud_acr_cfg;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->AUD_ACR_CFG.u32);
    o_aud_acr_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_aud_acr_cfg.bits.acr_fm_val_sw;
}

//******************************************************************************
//  Function    : HDMI_ACR_N_VAL_SW_acr_n_val_swGet
//  Description : Set the value of the member ACR_N_VAL_SW.acr_n_val_sw
//  Input       : unsigned int uacr_n_val_sw: 20 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ACR_N_VAL_SW_acr_n_val_swGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_ACR_N_VAL_SW o_acr_n_val_sw;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->ACR_N_VAL_SW.u32);
    o_acr_n_val_sw.u32 = HDMITXRegRead(pu32RegAddr);
    return o_acr_n_val_sw.bits.acr_n_val_sw;
}

//******************************************************************************
//  Function    : HDMI_ACR_CTS_VAL_SW_acr_cts_val_swGet
//  Description : Set the value of the member ACR_CTS_VAL_SW.acr_cts_val_sw
//  Input       : unsigned int uacr_cts_val_sw: 20 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ACR_CTS_VAL_SW_acr_cts_val_swGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_ACR_CTS_VAL_SW o_acr_cts_val_sw;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->ACR_CTS_VAL_SW.u32);
    o_acr_cts_val_sw.u32 = HDMITXRegRead(pu32RegAddr);
    return o_acr_cts_val_sw.bits.acr_cts_val_sw;
}

//******************************************************************************
//  Function    : HDMI_ACR_CTS_VAL_HW_acr_cts_val_hwGet
//  Description : Set the value of the member ACR_CTS_VAL_HW.acr_cts_val_hw
//  Input       : unsigned int uacr_cts_val_hw: 20 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ACR_CTS_VAL_HW_acr_cts_val_hwGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    
    U_ACR_CTS_VAL_HW o_acr_cts_val_hw;
    pu32RegAddr = (HI_U32*)&(gopaudio_path_regAllReg->ACR_CTS_VAL_HW.u32);
    o_acr_cts_val_hw.u32 = HDMITXRegRead(pu32RegAddr);
    return o_acr_cts_val_hw.bits.acr_cts_val_hw;
}
