// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  audio_path_reg_c_union_define.h
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  w00226427
// Version       :  1.0
// Date          :  Tester
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00271848 2015/05/23 11:11:23 Create file
// ******************************************************************************

#ifndef __AUDIO_PATH_REG_C_UNION_DEFINE_H__
#define __AUDIO_PATH_REG_C_UNION_DEFINE_H__

/* Define the union U_TX_AUDIO_CTRL */
/* Define the union U_TX_AUDIO_CTRL,ATTR:,ADDR:1000,INIT:000E4001,MASK:ff7f7,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aud_in_en     : 1  ; /* [0] */
        unsigned int    aud_mute_en   : 1  ; /* [1] */
        unsigned int    aud_layout    : 1  ; /* [2] */
        unsigned int    rsv_0         : 1  ; /* [3] */
        unsigned int    aud_i2s_en    : 4  ; /* [7:4] */
        unsigned int    aud_spdif_en  : 1  ; /* [8] */
        unsigned int    aud_src_en    : 1  ; /* [9] */
        unsigned int    aud_src_ctrl  : 1  ; /* [10] */
        unsigned int    rsv_1         : 1  ; /* [11] */
        unsigned int    aud_fifo0_map : 2  ; /* [13:12] */
        unsigned int    aud_fifo1_map : 2  ; /* [15:14] */
        unsigned int    aud_fifo2_map : 2  ; /* [17:16] */
        unsigned int    aud_fifo3_map : 2  ; /* [19:18] */
        unsigned int    rsv_2         : 12  ; /* [31:20] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_AUDIO_CTRL;

/* Define the union U_AUD_I2S_CTRL */
/* Define the union U_AUD_I2S_CTRL,ATTR:,ADDR:1004,INIT:00000B00,MASK:ff3f,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    i2s_hbra_on     : 1  ; /* [0] */
        unsigned int    i2s_1st_shift   : 1  ; /* [1] */
        unsigned int    i2s_ws_polarity : 1  ; /* [2] */
        unsigned int    i2s_justify     : 1  ; /* [3] */
        unsigned int    i2s_data_dir    : 1  ; /* [4] */
        unsigned int    i2s_vbit        : 1  ; /* [5] */
        unsigned int    rsv_3           : 2  ; /* [7:6] */
        unsigned int    i2s_length      : 4  ; /* [11:8] */
        unsigned int    i2s_ch_swap     : 4  ; /* [15:12] */
        unsigned int    rsv_4           : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUD_I2S_CTRL;

/* Define the union U_AUD_SPDIF_CTRL */
/* Define the union U_AUD_SPDIF_CTRL,ATTR:,ADDR:1008,INIT:00904084,MASK:fffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    spdif_1ui_lock   : 1  ; /* [0] */
        unsigned int    spdif_2ui_lock   : 1  ; /* [1] */
        unsigned int    i2s_cbit_order   : 1  ; /* [2] */
        unsigned int    spdif_fs_ovr_en  : 1  ; /* [3] */
        unsigned int    spdif_err_thresh : 6  ; /* [9:4] */
        unsigned int    spdif_size_sw    : 2  ; /* [11:10] */
        unsigned int    spdif_1ui_max    : 8  ; /* [19:12] */
        unsigned int    spdif_2ui_max    : 8  ; /* [27:20] */
        unsigned int    rsv_5            : 4  ; /* [31:28] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUD_SPDIF_CTRL;

/* Define the union U_AUD_CHST_CFG0 */
/* Define the union U_AUD_CHST_CFG0,ATTR:,ADDR:100C,INIT:00000000,MASK:ffffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chst_byte0_a              : 1  ; /* [0] */
        unsigned int    chst_byte0_b              : 1  ; /* [1] */
        unsigned int    chst_byte0_other          : 6  ; /* [7:2] */
        unsigned int    chst_byte1                : 8  ; /* [15:8] */
        unsigned int    chst_byte2                : 8  ; /* [23:16] */
        unsigned int    chst_byte3_fs             : 4  ; /* [27:24] */
        unsigned int    chst_byte3_clock_accuracy : 4  ; /* [31:28] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUD_CHST_CFG0;

/* Define the union U_AUD_CHST_CFG1 */
/* Define the union U_AUD_CHST_CFG1,ATTR:,ADDR:1010,INIT:0000000B,MASK:ffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    chst_byte4_length : 4  ; /* [3:0] */
        unsigned int    chst_byte4_org_fs : 4  ; /* [7:4] */
        unsigned int    chst_byte5_6  : 16  ; /* [23:8] */
        unsigned int    rsv_6             : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUD_CHST_CFG1;

/* Define the union U_TX_AUDIO_STATE */
/* Define the union U_TX_AUDIO_STATE,ATTR:,ADDR:1014,INIT:00000000,MASK:ffff3ff3,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aud_inavailable  : 1  ; /* [0] */
        unsigned int    aud_spdif_new_fs : 1  ; /* [1] */
        unsigned int    rsv_7            : 2  ; /* [3:2] */
        unsigned int    aud_length       : 4  ; /* [7:4] */
        unsigned int    aud_spdif_fs     : 6  ; /* [13:8] */
        unsigned int    rsv_8            : 2  ; /* [15:14] */
        unsigned int    spdif_max_1ui_st : 8  ; /* [23:16] */
        unsigned int    spdif_max_2ui_st : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TX_AUDIO_STATE;

/* Define the union U_AUD_FIFO_CTRL */
/* Define the union U_AUD_FIFO_CTRL,ATTR:,ADDR:1018,INIT:00000001,MASK:3f1f1f,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aud_fifo_test     : 5  ; /* [4:0] */
        unsigned int    rsv_9             : 3  ; /* [7:5] */
        unsigned int    aud_fifo_hbr_mask : 4  ; /* [11:8] */
        unsigned int    rsv_10            : 4  ; /* [15:12] */
        unsigned int    aud_fifo_ptr_diff : 6  ; /* [21:16] */
        unsigned int    rsv_11            : 10  ; /* [31:22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUD_FIFO_CTRL;

/* Define the union U_AUD_ACR_CTRL */
/* Define the union U_AUD_ACR_CTRL,ATTR:,ADDR:1040,INIT:00000005,MASK:1f,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acr_cts_req_en    : 1  ; /* [0] */
        unsigned int    acr_cts_hw_sw_sel : 1  ; /* [1] */
        unsigned int    acr_cts_gen_sel   : 1  ; /* [2] */
        unsigned int    acr_cts_flt_en    : 1  ; /* [3] */
        unsigned int    acr_use_sw_cts    : 1  ; /* [4] */
        unsigned int    acr_cts_ave_en    : 1  ; /* [5] */
        unsigned int    rsv_12            : 26  ; /* [31:6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUD_ACR_CTRL;

/* Define the union U_AUD_ACR_CFG */
/* Define the union U_AUD_ACR_CFG,ATTR:,ADDR:1044,INIT:00001040,MASK:ffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acr_fm_val_sw    : 3  ; /* [2:0] */
        unsigned int    acr_ave_max      : 5  ; /* [7:3] */
        unsigned int    acr_cts_thre     : 8  ; /* [15:8] */
        unsigned int    acr_cts_chg_thre : 8  ; /* [23:16] */
        unsigned int    rsv_13           : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AUD_ACR_CFG;

/* Define the union U_ACR_N_VAL_SW */
/* Define the union U_ACR_N_VAL_SW,ATTR:,ADDR:1048,INIT:00000000,MASK:fffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acr_n_val_sw : 20  ; /* [19:0] */
        unsigned int    rsv_14       : 12  ; /* [31:20] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ACR_N_VAL_SW;

/* Define the union U_ACR_CTS_VAL_SW */
/* Define the union U_ACR_CTS_VAL_SW,ATTR:,ADDR:104C,INIT:00000000,MASK:fffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acr_cts_val_sw : 20  ; /* [19:0] */
        unsigned int    rsv_15         : 12  ; /* [31:20] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ACR_CTS_VAL_SW;

/* Define the union U_ACR_CTS_VAL_HW */
/* Define the union U_ACR_CTS_VAL_HW,ATTR:,ADDR:1050,INIT:00000000,MASK:fffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    acr_cts_val_hw : 20  ; /* [19:0] */
        unsigned int    rsv_16         : 12  ; /* [31:20] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ACR_CTS_VAL_HW;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_TX_AUDIO_CTRL  TX_AUDIO_CTRL  ; /* 1000 */
    volatile U_AUD_I2S_CTRL   AUD_I2S_CTRL   ; /* 1004 */
    volatile U_AUD_SPDIF_CTRL AUD_SPDIF_CTRL ; /* 1008 */
    volatile U_AUD_CHST_CFG0  AUD_CHST_CFG0  ; /* 100C */
    volatile U_AUD_CHST_CFG1  AUD_CHST_CFG1  ; /* 1010 */
    volatile U_TX_AUDIO_STATE TX_AUDIO_STATE ; /* 1014 */
    volatile U_AUD_FIFO_CTRL  AUD_FIFO_CTRL  ; /* 1018 */
    unsigned int                       Reserved_0[9];
    volatile U_AUD_ACR_CTRL   AUD_ACR_CTRL   ; /* 1040 */
    volatile U_AUD_ACR_CFG    AUD_ACR_CFG    ; /* 1044 */
    volatile U_ACR_N_VAL_SW   ACR_N_VAL_SW   ; /* 1048 */
    volatile U_ACR_CTS_VAL_SW ACR_CTS_VAL_SW ; /* 104C */
    volatile U_ACR_CTS_VAL_HW ACR_CTS_VAL_HW ; /* 1050 */

} S_audio_path_reg_REGS_TYPE;

/* Declare the struct pointor of the module audio_path_reg */
//extern volatile S_audio_path_reg_REGS_TYPE *gopaudio_path_regAllReg;

int HDMI_TX_S_audio_path_reg_REGS_TYPE_Init(HI_VOID);
int HDMI_TX_S_audio_path_reg_REGS_TYPE_DeInit(HI_VOID);


/* Declare the functions that set the member value */
int HDMI_TX_AUDIO_CTRL_aud_in_enSet(unsigned int uaud_in_en);
int HDMI_TX_AUDIO_CTRL_aud_mute_enSet(unsigned int uaud_mute_en);
int HDMI_TX_AUDIO_CTRL_aud_layoutSet(unsigned int uaud_layout);
int HDMI_TX_AUDIO_CTRL_aud_i2s_enSet(unsigned int uaud_i2s_en);
int HDMI_TX_AUDIO_CTRL_aud_spdif_enSet(unsigned int uaud_spdif_en);
int HDMI_TX_AUDIO_CTRL_aud_src_enSet(unsigned int uaud_src_en);
int HDMI_TX_AUDIO_CTRL_aud_src_ctrlSet(unsigned int uaud_src_ctrl);
int HDMI_TX_AUDIO_CTRL_aud_fifo0_mapSet(unsigned int uaud_fifo0_map);
int HDMI_TX_AUDIO_CTRL_aud_fifo1_mapSet(unsigned int uaud_fifo1_map);
int HDMI_TX_AUDIO_CTRL_aud_fifo2_mapSet(unsigned int uaud_fifo2_map);
int HDMI_TX_AUDIO_CTRL_aud_fifo3_mapSet(unsigned int uaud_fifo3_map);
int HDMI_AUD_I2S_CTRL_i2s_hbra_onSet(unsigned int ui2s_hbra_on);
int HDMI_AUD_I2S_CTRL_i2s_1st_shiftSet(unsigned int ui2s_1st_shift);
int HDMI_AUD_I2S_CTRL_i2s_ws_polaritySet(unsigned int ui2s_ws_polarity);
int HDMI_AUD_I2S_CTRL_i2s_justifySet(unsigned int ui2s_justify);
int HDMI_AUD_I2S_CTRL_i2s_data_dirSet(unsigned int ui2s_data_dir);
int HDMI_AUD_I2S_CTRL_i2s_vbitSet(unsigned int ui2s_vbit);
int HDMI_AUD_I2S_CTRL_i2s_lengthSet(unsigned int ui2s_length);
int HDMI_AUD_I2S_CTRL_i2s_ch_swapSet(unsigned int ui2s_ch_swap);
int HDMI_AUD_SPDIF_CTRL_spdif_1ui_lockSet(unsigned int uspdif_1ui_lock);
int HDMI_AUD_SPDIF_CTRL_spdif_2ui_lockSet(unsigned int uspdif_2ui_lock);
int HDMI_AUD_SPDIF_CTRL_i2s_cbit_orderSet(unsigned int ui2s_cbit_order);
int HDMI_AUD_SPDIF_CTRL_spdif_fs_ovr_enSet(unsigned int uspdif_fs_ovr_en);
int HDMI_AUD_SPDIF_CTRL_spdif_err_threshSet(unsigned int uspdif_err_thresh);
int HDMI_AUD_SPDIF_CTRL_spdif_size_swSet(unsigned int uspdif_size_sw);
int HDMI_AUD_SPDIF_CTRL_spdif_1ui_maxSet(unsigned int uspdif_1ui_max);
int HDMI_AUD_SPDIF_CTRL_spdif_2ui_maxSet(unsigned int uspdif_2ui_max);
int HDMI_AUD_CHST_CFG0_chst_byte0_aSet(unsigned int uchst_byte0_a);
int HDMI_AUD_CHST_CFG0_chst_byte0_bSet(unsigned int uchst_byte0_b);
int HDMI_AUD_CHST_CFG0_chst_byte0_otherSet(unsigned int uchst_byte0_other);
int HDMI_AUD_CHST_CFG0_chst_byte1Set(unsigned int uchst_byte1);
int HDMI_AUD_CHST_CFG0_chst_byte2Set(unsigned int uchst_byte2);
int HDMI_AUD_CHST_CFG0_chst_byte3_fsSet(unsigned int uchst_byte3_fs);
int HDMI_AUD_CHST_CFG0_chst_byte3_clock_accuracySet(unsigned int uchst_byte3_clock_accuracy);
int HDMI_AUD_CHST_CFG1_chst_byte4_lengthSet(unsigned int uchst_byte4_length);
int HDMI_AUD_CHST_CFG1_chst_byte4_org_fsSet(unsigned int uchst_byte4_org_fs);
int HDMI_AUD_CHST_CFG1_chst_byte5_6Set(unsigned int uchst_byte5_6);
int HDMI_TX_AUDIO_STATE_aud_inavailableSet(unsigned int uaud_inavailable);
int HDMI_TX_AUDIO_STATE_aud_spdif_new_fsSet(unsigned int uaud_spdif_new_fs);
int HDMI_TX_AUDIO_STATE_aud_lengthSet(unsigned int uaud_length);
int HDMI_TX_AUDIO_STATE_aud_spdif_fsSet(unsigned int uaud_spdif_fs);
int HDMI_TX_AUDIO_STATE_spdif_max_1ui_stSet(unsigned int uspdif_max_1ui_st);
int HDMI_TX_AUDIO_STATE_spdif_max_2ui_stSet(unsigned int uspdif_max_2ui_st);
int HDMI_AUD_FIFO_CTRL_aud_fifo_testSet(unsigned int uaud_fifo_test);
int HDMI_AUD_FIFO_CTRL_aud_fifo_hbr_maskSet(unsigned int uaud_fifo_hbr_mask);
int HDMI_AUD_FIFO_CTRL_aud_fifo_ptr_diffSet(unsigned int uaud_fifo_ptr_diff);
int HDMI_AUD_ACR_CTRL_acr_cts_req_enSet(unsigned int uacr_cts_req_en);
int HDMI_AUD_ACR_CTRL_acr_cts_hw_sw_selSet(unsigned int uacr_cts_hw_sw_sel);
int HDMI_AUD_ACR_CTRL_acr_cts_gen_selSet(unsigned int uacr_cts_gen_sel);
int HDMI_AUD_ACR_CTRL_acr_cts_flt_enSet(unsigned int uacr_cts_flt_en);
int HDMI_AUD_ACR_CTRL_acr_use_sw_ctsSet(unsigned int uacr_use_sw_cts);
int HDMI_AUD_ACR_CTRL_acr_cts_ave_enSet(unsigned int uacr_cts_ave_en);
int HDMI_AUD_ACR_CFG_acr_fm_val_swSet(unsigned int uacr_fm_val_sw);
int HDMI_AUD_ACR_CFG_acr_ave_maxSet(unsigned int uacr_ave_max);
int HDMI_AUD_ACR_CFG_acr_cts_threSet(unsigned int uacr_cts_thre);
int HDMI_AUD_ACR_CFG_acr_cts_chg_threSet(unsigned int uacr_cts_chg_thre);
int HDMI_ACR_N_VAL_SW_acr_n_val_swSet(unsigned int uacr_n_val_sw);
int HDMI_ACR_CTS_VAL_SW_acr_cts_val_swSet(unsigned int uacr_cts_val_sw);
int HDMI_ACR_CTS_VAL_HW_acr_cts_val_hwSet(unsigned int uacr_cts_val_hw);
/* Declare the functions that get the member value */
int HDMI_TX_AUDIO_CTRL_aud_in_enGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_mute_enGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_layoutGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_i2s_enGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_spdif_enGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_src_enGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_src_ctrlGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_fifo0_mapGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_fifo1_mapGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_fifo2_mapGet(HI_VOID);
int HDMI_TX_AUDIO_CTRL_aud_fifo3_mapGet(HI_VOID);
int HDMI_AUD_I2S_CTRL_i2s_hbra_onGet(HI_VOID);
int HDMI_AUD_I2S_CTRL_i2s_1st_shiftGet(HI_VOID);
int HDMI_AUD_I2S_CTRL_i2s_ws_polarityGet(HI_VOID);
int HDMI_AUD_I2S_CTRL_i2s_justifyGet(HI_VOID);
int HDMI_AUD_I2S_CTRL_i2s_data_dirGet(HI_VOID);
int HDMI_AUD_I2S_CTRL_i2s_vbitGet(HI_VOID);
int HDMI_AUD_I2S_CTRL_i2s_lengthGet(HI_VOID);
int HDMI_AUD_I2S_CTRL_i2s_ch_swapGet(HI_VOID);
int HDMI_AUD_SPDIF_CTRL_spdif_1ui_lockGet(HI_VOID);
int HDMI_AUD_SPDIF_CTRL_spdif_2ui_lockGet(HI_VOID);
int HDMI_AUD_SPDIF_CTRL_i2s_cbit_orderGet(HI_VOID);
int HDMI_AUD_SPDIF_CTRL_spdif_fs_ovr_enGet(HI_VOID);
int HDMI_AUD_SPDIF_CTRL_spdif_err_threshGet(HI_VOID);
int HDMI_AUD_SPDIF_CTRL_spdif_size_swGet(HI_VOID);
int HDMI_AUD_SPDIF_CTRL_spdif_1ui_maxGet(HI_VOID);
int HDMI_AUD_SPDIF_CTRL_spdif_2ui_maxGet(HI_VOID);
int HDMI_AUD_CHST_CFG0_chst_byte0_aGet(HI_VOID);
int HDMI_AUD_CHST_CFG0_chst_byte0_bGet(HI_VOID);
int HDMI_AUD_CHST_CFG0_chst_byte0_otherGet(HI_VOID);
int HDMI_AUD_CHST_CFG0_chst_byte1Get(HI_VOID);
int HDMI_AUD_CHST_CFG0_chst_byte2Get(HI_VOID);
int HDMI_AUD_CHST_CFG0_chst_byte3_fsGet(HI_VOID);
int HDMI_AUD_CHST_CFG0_chst_byte3_clock_accuracyGet(HI_VOID);
int HDMI_AUD_CHST_CFG1_chst_byte4_lengthGet(HI_VOID);
int HDMI_AUD_CHST_CFG1_chst_byte4_org_fsGet(HI_VOID);
int HDMI_AUD_CHST_CFG1_chst_byte5_6Get(HI_VOID);
int HDMI_TX_AUDIO_STATE_aud_inavailableGet(HI_VOID);
int HDMI_TX_AUDIO_STATE_aud_spdif_new_fsGet(HI_VOID);
int HDMI_TX_AUDIO_STATE_aud_lengthGet(HI_VOID);
int HDMI_TX_AUDIO_STATE_aud_spdif_fsGet(HI_VOID);
int HDMI_TX_AUDIO_STATE_spdif_max_1ui_stGet(HI_VOID);
int HDMI_TX_AUDIO_STATE_spdif_max_2ui_stGet(HI_VOID);
int HDMI_AUD_FIFO_CTRL_aud_fifo_testGet(HI_VOID);
int HDMI_AUD_FIFO_CTRL_aud_fifo_hbr_maskGet(HI_VOID);
int HDMI_AUD_FIFO_CTRL_aud_fifo_ptr_diffGet(HI_VOID);
int HDMI_AUD_ACR_CTRL_acr_cts_req_enGet(HI_VOID);
int HDMI_AUD_ACR_CTRL_acr_cts_hw_sw_selGet(HI_VOID);
int HDMI_AUD_ACR_CTRL_acr_cts_gen_selGet(HI_VOID);
int HDMI_AUD_ACR_CTRL_acr_cts_flt_enGet(HI_VOID);
int HDMI_AUD_ACR_CTRL_acr_use_sw_ctsGet(HI_VOID);
int HDMI_AUD_ACR_CTRL_acr_cts_ave_enGet(HI_VOID);
int HDMI_AUD_ACR_CFG_acr_fm_val_swGet(HI_VOID);
int HDMI_AUD_ACR_CFG_acr_ave_maxGet(HI_VOID);
int HDMI_AUD_ACR_CFG_acr_cts_threGet(HI_VOID);
int HDMI_AUD_ACR_CFG_acr_cts_chg_threGet(HI_VOID);
int HDMI_ACR_N_VAL_SW_acr_n_val_swGet(HI_VOID);
int HDMI_ACR_CTS_VAL_SW_acr_cts_val_swGet(HI_VOID);
int HDMI_ACR_CTS_VAL_HW_acr_cts_val_hwGet(HI_VOID);
#endif // __AUDIO_PATH_REG_C_UNION_DEFINE_H__
