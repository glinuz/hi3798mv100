/*
 * Copyright (c) (2015 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    vdh_module.h
 *
 * Purpose: vdh hardware module revision relative header
 *
 * Author:  yangyichang 00226912
 *
 * Date:    06, 09, 2015
 *
 */

#ifndef __VDH_MODULE_H__
#define __VDH_MODULE_H__

#include "vfmw.h"


#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------- ENUM ------------------------------------*/

typedef enum
{
    NORMAL_SCD_TASK = 0,
    NORMAL_MFDE_TASK,
    SECURE_SCD_TASK,
    SECURE_MFDE_TASK,
}TASK_TYPE_E;

/*--------------------------------- STRUCTURE ---------------------------------*/

typedef struct 
{
    UINT32           reg_vdh_clk_sel;
    UINT32           reg_vdh_clk_skip;
}CRG_RegsMap_S;

typedef struct 
{
    UINT32           reg_list_addr;
    UINT32           reg_up_addr;
    UINT32           reg_up_len;
    UINT32           reg_buffer_first;
    UINT32           reg_buffer_last;
    UINT32           reg_buffer_ini;
    UINT32           reg_scd_int_mask;
    UINT32           reg_scd_protocol;
    UINT32           reg_avs_flag;
    UINT32           reg_emar_id;
    UINT32           reg_vdh_selrst; 
    UINT32           reg_vdh_ck_gt;
    UINT32           reg_dsp_sps_msg_addr;
    UINT32           reg_dsp_pps_msg_addr;
    UINT32           reg_dvm_msg_addr;
    UINT32           reg_sed_top_addr;
    UINT32           reg_ca_mn_addr;
    UINT32           reg_previous_byte_lsb;
    UINT32           reg_previous_byte_msb;
}SCD_RegsMap_S;

typedef struct 
{
    UINT32           reg_basic_cfg0;
    UINT32           reg_basic_cfg1;
    UINT32           reg_avm_addr;
    UINT32           reg_vam_addr;
    UINT32           reg_stream_base_addr;
    UINT32           reg_int_state;
    UINT32           reg_int_mask;
    UINT32           reg_part_dec_over_int_level;
    UINT32           reg_ystaddr;
    UINT32           reg_ystride;
    UINT32           reg_uvoffset;
    UINT32           reg_head_inf_offset;
    UINT32           reg_line_num_staddr;
    UINT32           reg_ystride_2bit;
    UINT32           reg_yoffset_2bit;
    UINT32           reg_uvoffset_2bit;
    UINT32           reg_ppfd_buf_addr;
    UINT32           reg_ppfd_buf_len;
    UINT32           reg_ref_pic_type;
    UINT32           reg_ff_apt_en;
    UINT32           reg_down_clk_cfg;
    UINT32           reg_smmu_int_mask;
}MFDE_RegsMap_S;

typedef struct 
{
    UINT8            ModuleId;
    UINT8            IsLowDelay;
    UINT8            IsRepair;
    TASK_TYPE_E      Type;
    SINT32           ChanId;
    UINT32           StartTime;
    CRG_RegsMap_S    CRG_Regs;
    SCD_RegsMap_S    SCD_Regs;
    MFDE_RegsMap_S   MFDE_Regs; 
}TASK_PARAM_S;


/*---------------------------------- FUNCTION ---------------------------------*/

/******************************************
 Init VDH Module
******************************************/
VOID VDH_Init_Module(VOID);

/******************************************
 Init Hardware
******************************************/
VOID VDH_Init_Hardware(VOID);

/******************************************
 Enable scd module 
******************************************/
VOID VDH_Enable_Scd_Module(SINT32 ModuleID);

/******************************************
 Disable scd module 
******************************************/
VOID VDH_Disable_Scd_Module(SINT32 ModuleID);

/******************************************
 Reset scd module 
******************************************/
VOID VDH_Reset_Scd_Module(SINT32 ModuleID);

/******************************************
 Enable mfde module 
******************************************/
VOID VDH_Enable_Mfde_Module(SINT32 ModuleID);

/******************************************
 Disable mfde module 
******************************************/
VOID VDH_Disable_Mfde_Module(SINT32 ModuleID);

/******************************************
 Reset mfde module 
******************************************/
VOID VDH_Reset_Mfde_Module(SINT32 ModuleID);

/******************************************
 Enable bpd module 
******************************************/
VOID VDH_Enable_Bpd_Module(SINT32 ModuleID);

/******************************************
 Disable bpd module 
******************************************/
VOID VDH_Disable_Bpd_Module(SINT32 ModuleID);

/******************************************
 Enable dsp module 
******************************************/
VOID VDH_Enable_Dsp_Module(SINT32 ModuleID);

/******************************************
 Disable dsp module 
******************************************/
VOID VDH_Disable_Dsp_Module(SINT32 ModuleID);

/******************************************
 Load dsp code 
******************************************/
VOID VDH_Load_Dsp_Code(SINT32 ModuleID, DSP_REQUIRE_E Require);

/******************************************
 Start Scd Module
******************************************/
VOID VDH_Start_Scd_Module(TASK_PARAM_S *pTask);

/******************************************
 Start Mfde Module
******************************************/
VOID VDH_Start_Mfde_Module(TASK_PARAM_S *pTask);

/******************************************
 Record crg reg data into task
******************************************/
VOID VDH_Record_CrgRegData(CRG_RegsMap_S *pRegMap, UINT32 Reg, UINT32 Data);

/******************************************
 Record scd reg data into task
******************************************/
VOID VDH_Record_ScdRegData(SCD_RegsMap_S *pRegMap, UINT32 Reg, UINT32 Data);

/******************************************
 Record mfde reg data into task
******************************************/
VOID VDH_Record_MfdeRegData(MFDE_RegsMap_S *pRegMap, UINT32 Reg, UINT32 Data);


#ifdef __cplusplus
}
#endif

#endif

