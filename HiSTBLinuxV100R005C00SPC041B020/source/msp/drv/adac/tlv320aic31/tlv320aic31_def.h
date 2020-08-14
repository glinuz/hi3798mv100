#ifndef _INC_TLV320AIC31_DEF
#define _INC_TLV320AIC31_DEF


typedef union{
    struct
    {
        unsigned char reserved2 :4;
        unsigned char bit_work_dri_ctrl:1;
        unsigned char reserved1:1;
        unsigned char work_clock_dic_ctrl:1;
        unsigned char bit_clock_dic_ctrl:1;
    }bit;
    unsigned char b8;
}Ctrl_Mode;

typedef union{
    struct
    {
        unsigned char input_vol_level_ctrl:7;
        unsigned char if_mute_route:1;
    }bit;
    unsigned char b8;
}Adc_Pga_Dac_Gain_Ctrl;
    
typedef union{
    struct 
    {   
        unsigned char in2r_adc_input_level_sample:4;
        unsigned char in2l_adc_input_level_sample:4;
    }bit;
    unsigned char b8;
}In2_Adc_Ctrl_Sample;

typedef union{
    struct
    {
        unsigned adc_pga_step_ctrl:2;
        unsigned adc_ch_power_ctrl:1;
        unsigned char in1_adc_input_level:4;
        unsigned char mode:1;
    }bit;
    unsigned char b8;
}In1_Adc_Ctrl;

typedef union{
    struct
    {
        unsigned char reserved:4;
        unsigned char data_length:2;  
        unsigned char transfer_mode:2;                      
    }bit;
    unsigned char b8;
}Serial_Int_Ctrl;

typedef union{
    struct
    {
        unsigned char power_status:1;
        unsigned char vol_ctrl_status:1;
        unsigned char power_down_ctrl:1;
        unsigned char if_mute:1;
        unsigned char output_level:4;
    }bit;
    unsigned char b8;
}Line_Hpcom_Out_Ctrl;
typedef union{
    struct
    {
        unsigned char reserved1:1;
        unsigned char right_dac_datapath_ctrl:2;
        unsigned char left_dac_datapath_ctrl:2;
        unsigned char reserved2:3;
    }bit;
    unsigned char b8;
}Codec_Datapath_Setup_Ctrl;
typedef union{
    struct
    {
        unsigned char reserved:6;
        unsigned char right_dac_power_ctrl:1;
        unsigned char left_dac_power_ctrl:1;
    }bit;
    unsigned char b8;
}DAC_POWER_CTRL;
typedef union{
    struct
    {
        unsigned char reserved:4;
        unsigned char right_dac_swi_ctrl:2;
        unsigned char left_dac_swi_ctrl:2;
    }bit;
    unsigned char b8;
}DAC_OUTPUT_SWIT_CTRL;

#endif

