#include "AVL6381_Internal.h"
#include "AVL6381_fw_DTMB.h"
#include "AVL6381_freeze_DTMB.h"

extern AVL6381_uchar gAVL6381freezeflag;
AVL6381_ErrorStats   gsAVL6381ErrorStat = {0,0,{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},0,0};

AVL6381_uint32 AVL6381_Table_420 [4][3] = {
                                {15,     35,     56},
                                {73,     96,    130},
                                {0,       0,    155},
                                {121,   164,    187}
                              	};

AVL6381_uint32 AVL6381_Table_945 [4][3] = {
                                 {14,       34,      55},
                                 {72,       96,     130},
                                 {0,        0,      155},
                                 {120,      164,    190}
                               	};

AVL6381_uint32 AVL6381_Table_595 [4][3] = {
                                 {15,       35,     71},
                                 {73,       96,     130},
                                 {0,        0,      159},
                                 {122,     153,     198}
                              	};
#define THRESHOLD_NR_595    25


AVL6381_PLL_Conf gAVL6381PLLConfig[8] = 
{
    //| Ref Clk |R|F|bwadj|OD|  Demod  |R |F|bwadj|OD1|OD2|OD3|use pll adc|DLL out|DLL R|   FEC   |  SDRAM  |   ADC  |
    {  30000000, 2,50, 3,  5, 300000000,1,34, 3,  12,  25, 32,     0,        110,   15,  170000000,81600000,63750000},
    {  30000000, 2,44, 3,  6, 220000000,1,34, 3,  12,  25, 32,     0,        110,   15,  170000000,81600000,63750000},
    {  16000000, 1,42, 3,  5, 268800000,1,54, 3,  10,  24, 27,     1,        110,   15,  172800000,72000000,64000000},
    {  16000000, 1,42, 3,  6, 224000000,1,54, 3,  10,  24, 27,     1,        110,   15,  172800000,72000000,64000000},
    {  24000000, 2,75, 3,  6, 300000000,1,35, 3,  10,  21, 28,     0,        110,   15,  168000000,80000000,60000000},
  //{  24000000, 2,55, 3,  6, 220000000,1,35, 3,  10,  21, 28,     0,        110,   15,  168000000,80000000,60000000},
    {  24000000, 1,42, 2,  9, 224000000,1,42, 2,  12,  25, 2,      0,        95,    15,  168000000,80000000,60000000},
    {  27000000, 3,74, 3,  5, 266400000,1,32, 3,  10,  24, 27,     0,        110,   15,  172800000,72000000,64000000},
    {  27000000, 3,74, 3,  6, 222000000,1,32, 3,  10,  24, 27,     0,        110,   15,  172800000,72000000,64000000}
};

AVL6381_PLL_Conf gAVL6381SleepPLLConfig[] =
{
    //| Ref Clk |R|F|bwadj|OD|  Demod  |R|F|bwadj|OD1|OD2|OD3|use pll adc|DLL out|DLL R|   FEC   |  SDRAM  |   ADC  |
    {  30000000, 2,44, 3,  12,110000000,1,34, 3,  24,  32, 32,     0,        110,   15,  85000000,63750000,63750000},
    {  16000000, 1,42, 3,  12,112000000,1,54, 3,  20,  27, 27,     0,        110,   15,  86400000,64000000,64000000},
    {  24000000, 2,55, 3,  12,110000000,1,35, 3,  20,  28, 28,     0,        110,   15,  84000000,60000000,60000000},
    {  27000000, 3,74, 3,  12,111000000,1,32, 3,  20,  27, 27,     0,        110,   15,  86400000,64000000,64000000}
};

AVL6381_SDRAM_Conf gAVL6381SDRAMConfig = 
{
    63, 84, 84, 1, 42, 21, 42, 2  
};


AVL6381_AGCPola geAVL6381AGCPola = AGC_NORMAL;

AVL6381_MPEG_SerialDataPin 	geAVL6381SerialPin		= AVL6381_MPEG_SERIAL_D0;
AVL6381_MPEG_SERIAL_ORDER 	geAVL6381SerialOrder 	= AVL6381_MPEG_SERIAL_ORDER_MSB;
AVL6381_MPEG_SERIAL_SYNC 	geAVL6381SerialSync 	= AVL6381_MPEG_SERIAL_SYNC_1_PULSE;
AVL6381_MPEG_ErrorBit 		geAVL6381ErrorBit 		= AVL6381_MPEG_ERROR_BIT_DISABLE;
AVL6381_MPEG_ErrorBitPola 	geAVL6381ErrorPola 		= AVL6381_MPEG_ERROR_HIGH;
AVL6381_MPEG_ValidPola 		geAVL6381ValidPola 		= AVL6381_MPEG_VALID_HIGH;
AVL6381_MPEG_Packet_Len 	geAVL6381PacketLen 		= AVL6381_MPEG_188;
AVL6381_MPEG_PARALLEL_ORDER geAVL6381ParallelOrder 	= AVL6381_MPEG_PARALLEL_ORDER_NORMAL;
AVL6381_uint32 guiDTMBSerialMPEGContinuousHz = 56000000;	//56M
AVL6381_uint32 guiDVBCSerialMPEGContinuousHz = 85000000;	//85M
AVL6381_uint32 guiParallelMPEGContinuousHz   = 8500000;		//8.5M
AVL6381_uint32 guiDTMBSymbolRateSps 		 = 7560*1000;	//DTMB symbol rate, default value 7.56Msps

AVL6381_ErrorCode SetPLL_6381(AVL6381_PLL_Conf *pPLL_Conf,struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r  = AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_demod_clkr, pPLL_Conf->m_Demod_PLL_DivR-1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_demod_clkf, pPLL_Conf->m_Demod_PLL_DivF-1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_demod_bwadj,pPLL_Conf->m_Demod_PLL_bwadj);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_demod_od,   pPLL_Conf->m_Demod_PLL_od-1);

    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_aux_clkr, pPLL_Conf->m_Aux_PLL_DivR-1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_aux_clkf, pPLL_Conf->m_Aux_PLL_DivF-1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_aux_bwadj,pPLL_Conf->m_Aux_PLL_bwadj);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_aux_od,   pPLL_Conf->m_Aux_PLL_od-1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_aux_od2,  pPLL_Conf->m_Aux_PLL_od2-1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_pll_aux_od3,  pPLL_Conf->m_Aux_PLL_od3-1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_clock_reset, 0);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_clock_reset, 1);
    tuner_mdelay(1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_dll_out_phase, pPLL_Conf->m_DDLL_out_phase);
	r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_dll_rd_phase,  pPLL_Conf->m_DDLL_read_phase);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_dll_deglitch_mode, 1);//actually, this is used to enable DLL normal mode
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_dll_init, 1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_dll_init, 0);

    if(pPLL_Conf->m_use_pll_adc == 0)
    {
        gsAVL6381_Config.m_ADCFrequency_Hz = pPLL_Conf->m_RefFrequency_Hz;
    }
    else
    {
        gsAVL6381_Config.m_ADCFrequency_Hz = pPLL_Conf->m_ADC_Frequency_Hz/2;
        r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_adc_clock_sel, 0xc);
    }

    gsAVL6381_Config.m_RefFrequency_Hz = pPLL_Conf->m_RefFrequency_Hz;
    gsAVL6381_Config.m_DemodFrequency_Hz = pPLL_Conf->m_DemodFrequency_Hz;
    gsAVL6381_Config.m_SDRAMFrequency_Hz = pPLL_Conf->m_SDRAM_Frequency_Hz;
    gsAVL6381_Config.m_FECFrequency_Hz = pPLL_Conf->m_FEC_Frequency_Hz;

	HI_INFO_TUNER("func = %s\n",__func__);
	HI_INFO_TUNER("m_ADCFrequency_Hz=%d m_RefFrequency_Hz=%d m_DemodFrequency_Hz=%d m_SDRAMFrequency_Hz=%d m_FECFrequency_Hz=%d\n",
					gsAVL6381_Config.m_ADCFrequency_Hz,
					gsAVL6381_Config.m_RefFrequency_Hz,
					gsAVL6381_Config.m_DemodFrequency_Hz,
					gsAVL6381_Config.m_SDRAMFrequency_Hz,
					gsAVL6381_Config.m_FECFrequency_Hz);

    return r;
}

AVL6381_ErrorCode DigitalCoreReset_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_digital_core_reset, 0);
    tuner_mdelay(2);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_digital_core_reset, 1);
    
    return r;
}

AVL6381_ErrorCode IBase_Initialize_6381(AVL6381_PLL_Conf *pPLL_Conf, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_uint32 uiSize = 0;
    AVL6381_uint32 i1 = 0;
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_puchar pInitialData;

    r = AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_c306_top_srst, 1);
    // Configure the PLL
    tuner_mdelay(10);
    r |= SetPLL_6381(pPLL_Conf, pAVLChip);
    r |= DigitalCoreReset_6381(pAVLChip);
	tuner_mdelay(2);

    if(gAVL6381freezeflag == 1)
    {
        pInitialData = AVL6381_freezeData_DTMB;
    }else{
        pInitialData = AVL6381_FwData_DTMB;
    }

    if( AVL6381_EC_OK == r )
    {
		HI_INFO_TUNER("func = %s  Success, start install fw\n",__func__);
        //load patch into the chip
        i1 = 0;
        i1 += 4;//skip the firmware version
        while(1)
        {
            uiSize = AVL6381_DeChunk32(pInitialData+i1);
            if(0 == uiSize)
            {
                break;
            }
            // Increment index past the size field (4 bytes) and past first byte of address field (always zero)
            i1 += 5;
            // Send the address (3 bytes) + data (uiSize bytes)
            r |= AVL6381_II2C_Write(pAVLChip, pInitialData+i1, uiSize+3);
            // Increment to the next burst of data to send
            i1 += (3 + uiSize);
        }
        //
        if(gAVL6381freezeflag == 1)
        {
            r |= AVL6381_II2C_Write32(pAVLChip, 0x0228, 0x00280000);
            r |= AVL6381_II2C_Write32(pAVLChip, 0x022c, 0x002d0008);
            r |= AVL6381_II2C_Write32(pAVLChip, 0x0230, 0x0028cb00);
            r |= AVL6381_II2C_Write32(pAVLChip, 0x0234, 0x002f2c08);
        }
        else
        {
            r |= AVL6381_II2C_Write32(pAVLChip, 0x0228, 0x00280000);
            r |= AVL6381_II2C_Write32(pAVLChip, 0x022c, 0x002d4000);
            r |= AVL6381_II2C_Write32(pAVLChip, 0x0230, 0x0028a000);
            r |= AVL6381_II2C_Write32(pAVLChip, 0x0234, 0x002e8000);
        }
        r |= AVL6381_II2C_Write8(pAVLChip, 0x0225, 1);

        r |= AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_demod_mode_caddr, 0);

        //startup 
        r |= AVL6381_II2C_Write16(pAVLChip, rc_mode_configuration_saddr, gsAVL6381_Config.ucStartupMode);
        r |= AVL6381_II2C_Write16(pAVLChip, rc_mode_switch_saddr, 0 );
        r |= AVL6381_II2C_Write32(pAVLChip, rs_AVL6381_core_ready_word_iaddr, 0x00000000);
        r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_c306_top_srst, 0);

    }else{
		HI_INFO_TUNER("func = %s  Failed, can not install fw\n",__func__);
    }

    return r;
}



AVL6381_ErrorCode CheckChipReady_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiCoreReadyWord = 0;
    AVL6381_uint32 uiCoreRunning = 0;

    r = AVL6381_II2C_Read32(pAVLChip, hw_AVL6381_c306_top_srst, &uiCoreRunning);
    r |= AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_core_ready_word_iaddr, &uiCoreReadyWord);

    if(AVL6381_EC_OK == r)
    {
        if( (1 == uiCoreRunning) || (uiCoreReadyWord != 0x5aa57ff7) )
        {
            r = AVL6381_EC_GeneralFail;
        }
    }
    
    return r;
}

AVL6381_ErrorCode GetRxOP_Status_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiCmd = 0;

    r = AVL6381_II2C_Read32(pAVLChip, rc_AVL6381_fw_command_iaddr, &uiCmd);

    if( AVL6381_EC_OK == r )
    {
        if( 0 != uiCmd )
        {
            r = AVL6381_EC_RUNNING;
        }
    }

    return(r);
}

AVL6381_ErrorCode SendRxOP_6381(AVL6381_uchar ucOpCmd, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uchar pucBuff[4];
    AVL6381_uint32 uiTemp;
    const AVL6381_uint16 uiTimeDelay = 10;
    const AVL6381_uint16 uiMaxRetries = 20;
    AVL6381_uint32 i = 0;

	AVL6381_Mutex_Lock(gsAVL6381_Config.m_semRx);
    while (AVL6381_EC_OK != GetRxOP_Status_6381(pAVLChip))
    {
        if (uiMaxRetries < i++)
        {
            r |= AVL6381_EC_RUNNING;
            break;
        }
      	tuner_mdelay(uiTimeDelay);
    }
    if( AVL6381_EC_OK == r )
    {
        pucBuff[0] = ucOpCmd;
        pucBuff[1] = 0;
        pucBuff[2] = 0;
        pucBuff[3] = 0;
        uiTemp = AVL6381_DeChunk32(pucBuff);
        r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_fw_command_iaddr, uiTemp);
    }
	AVL6381_Mutex_Unlock(gsAVL6381_Config.m_semRx);

    return r;
}

AVL6381_ErrorCode DTMB_SetSpectrumPola_6381(AVL6381_SpectrumInversion enumSpectrumPola, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_int32 iNomCarrierFreqHz = 0;

    if(gsAVL6381_Config.m_InputSignal_Info.uiDTMBIFFreqHz > gsAVL6381_Config.m_ADCFrequency_Hz)
    {
        iNomCarrierFreqHz = gsAVL6381_Config.m_InputSignal_Info.uiDTMBIFFreqHz - gsAVL6381_Config.m_ADCFrequency_Hz;
    }
    else
    {
        iNomCarrierFreqHz = gsAVL6381_Config.m_InputSignal_Info.uiDTMBIFFreqHz;
    }

    switch(enumSpectrumPola)
    {
        case SPECTRUM_NORMAL:
            r = AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_spectrum_invert_caddr, 0);
            r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_nom_carrier_freq_Hz_iaddr, iNomCarrierFreqHz);
            break;
        case SPECTRUM_INVERTED:
            r = AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_spectrum_invert_caddr, 0);
            r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_nom_carrier_freq_Hz_iaddr, -iNomCarrierFreqHz);
            break;
        case SPECTRUM_AUTO:
            r = AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_spectrum_invert_caddr, 2);
            r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_nom_carrier_freq_Hz_iaddr, iNomCarrierFreqHz);//start from normal spectrum      
            break;
        default:
            break;
    }

    return r;
}

AVL6381_ErrorCode DTMB_InitRx_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    // Load the default configuration
    r = SendRxOP_6381(AVL6381_OP_RX_LD_DEFAULT, pAVLChip);

    r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_dmd_clk_Hz_iaddr, gsAVL6381_Config.m_DemodFrequency_Hz);
    r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_fec_clk_Hz_iaddr, gsAVL6381_Config.m_FECFrequency_Hz);
    r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_sdram_clk_Hz_iaddr, gsAVL6381_Config.m_SDRAMFrequency_Hz);
    r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_sample_rate_Hz_iaddr, gsAVL6381_Config.m_ADCFrequency_Hz);

    r |= AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_input_format_caddr, 1);//always offset binary
    r |= AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_input_select_caddr, 1);//always analog input

    r |= AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_iq_comp_enable_caddr, 0);//disable IQ compensation
        
    r |= AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_equalizer_mode_caddr, 1);//data aided

    r |= AVL6381_II2C_Write8(pAVLChip, rs_AVL6381_DTMB_fec_lock_caddr, 0);

    r |= DTMB_SetSpectrumPola_6381(SPECTRUM_NORMAL, pAVLChip);// default spectrum is normal

    return r;
}


AVL6381_ErrorCode DTMB_InitADC_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    // ADC I path or ADC Q path
    if(gsAVL6381_Config.m_InputSignal_Info.ueDTMBInputPath == ADC_IF_I || gsAVL6381_Config.m_InputSignal_Info.ueDTMBInputPath == ADC_IF_Q)
    {
        r = AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_tuner_type_caddr, 0);//IF input

        if(gsAVL6381_Config.m_InputSignal_Info.ueDTMBInputPath== ADC_IF_I)//I
        {
            r |= AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_ADC_I0_Q1_caddr, 0); //For ADC Init
        }
        else if(gsAVL6381_Config.m_InputSignal_Info.ueDTMBInputPath == ADC_IF_Q)//Q
        {
            r |= AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_ADC_I0_Q1_caddr, 1); //For ADC Init
        }
    }
    else
    {
        r |= AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_tuner_type_caddr, 1);//BB input, both I&Q ADCs are used
        r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_nom_carrier_freq_Hz_iaddr, 0);
    }

    r |= SendRxOP_6381(AVL6381_OP_RX_INIT_ADC, pAVLChip);
    
    return r;
}


AVL6381_uint32 ConvertNsToSDRAM_Clocks(AVL6381_uint32 SDRAM_ClkFrequency_Hz, AVL6381_uint16 Value_ns)
{
    AVL6381_uint32 SDRAM_Freq_100kHz;
    AVL6381_uint32 temp;
    AVL6381_uint32 uiValue_clks;

    SDRAM_Freq_100kHz = (SDRAM_ClkFrequency_Hz/100000);
    temp = (SDRAM_Freq_100kHz * 419 * Value_ns) - 41943;
    uiValue_clks = (temp >> 22);
    if((temp & 0x003FFFFF) != 0)
    {
        uiValue_clks++;
    }

    return uiValue_clks;
}


AVL6381_ErrorCode InitSDRAM_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 latencyValue = 0;
    AVL6381_uint32 uiTemp = 0;

    uiTemp = (((AVL6381_uint32)7) << 16);
    uiTemp |= (((AVL6381_uint32)10) << 8);
    r = AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_sdram_config_iaddr, uiTemp);
        
    latencyValue = ConvertNsToSDRAM_Clocks(gsAVL6381_Config.m_SDRAMFrequency_Hz, gAVL6381SDRAMConfig.m_SDRAM_TRC);
    uiTemp = ((latencyValue - 1) << 24);
    latencyValue = ConvertNsToSDRAM_Clocks(gsAVL6381_Config.m_SDRAMFrequency_Hz, gAVL6381SDRAMConfig.m_SDRAM_TXSR);
    uiTemp |= ((latencyValue - 1) << 16);
    latencyValue = ConvertNsToSDRAM_Clocks(gsAVL6381_Config.m_SDRAMFrequency_Hz, gAVL6381SDRAMConfig.m_SDRAM_TRCAR);
    uiTemp |= ((latencyValue - 1) << 8);
    uiTemp |= (AVL6381_uint32)(gAVL6381SDRAMConfig.m_SDRAM_TWR - 1);
    r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_sdram_timing_config_0_iaddr, uiTemp);
    latencyValue = ConvertNsToSDRAM_Clocks(gsAVL6381_Config.m_SDRAMFrequency_Hz, gAVL6381SDRAMConfig.m_SDRAM_TRP);
    uiTemp = ((latencyValue - 1) << 24);
    latencyValue = ConvertNsToSDRAM_Clocks(gsAVL6381_Config.m_SDRAMFrequency_Hz, gAVL6381SDRAMConfig.m_SDRAM_TRCD);
    uiTemp |= ((latencyValue - 1) << 16);
    latencyValue = ConvertNsToSDRAM_Clocks(gsAVL6381_Config.m_SDRAMFrequency_Hz, gAVL6381SDRAMConfig.m_SDRAM_TRAS);
    uiTemp |= ((latencyValue - 1) << 8);
    uiTemp |= (AVL6381_uint32)(gAVL6381SDRAMConfig.m_SDRAM_CAS_Latency - 1);
    r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_sdram_timing_config_1_iaddr, uiTemp); 

    //send command to init sdram
    r |= SendRxOP_6381(AVL6381_OP_RX_INIT_SDRAM, pAVLChip);

    return r;
}

AVL6381_ErrorCode IRx_Initialize_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = DTMB_InitRx_6381(pAVLChip);
    r |= DTMB_InitADC_6381(pAVLChip);

    r |= InitSDRAM_6381(pAVLChip);

    return r;
}


AVL6381_ErrorCode DTMB_SetSymbolRate_6381(AVL6381_uint32 uiSymbolRateHz,struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    guiDTMBSymbolRateSps = uiSymbolRateHz;
    r = AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_symbol_rate_Hz_iaddr, uiSymbolRateHz);

    return r;
}


AVL6381_ErrorCode DTMB_SetMpegMode_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    
    r = AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_mpeg_posedge_caddr,gsAVL6381_Config.m_MPEG_Info.enumRisingFalling);
    r |= AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_mpeg_serial_caddr,gsAVL6381_Config.m_MPEG_Info.enumSerialParallel);

    return r;
}



AVL6381_ErrorCode SetMpegMode_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = DTMB_SetMpegMode_6381(pAVLChip);

    return r;
}

AVL6381_ErrorCode DTMB_SetMpegSerialPin_6381(AVL6381_MPEG_SerialDataPin enumSerialPin,struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_serial_outpin_sel_caddr,enumSerialPin);

    return r;
}


AVL6381_ErrorCode SetMpegSerialPin_6381(AVL6381_MPEG_SerialDataPin enumSerialPin, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    geAVL6381SerialPin = enumSerialPin;
    
    r = DTMB_SetMpegSerialPin_6381(enumSerialPin,pAVLChip);

    return r;
}

AVL6381_ErrorCode DTMB_SetMpegSerialOrder_6381(AVL6381_MPEG_SERIAL_ORDER enumSerialOrder, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_mpeg_seq_caddr,enumSerialOrder);

    return r;
}

AVL6381_ErrorCode SetMpegSerialOrder_6381(AVL6381_MPEG_SERIAL_ORDER enumSerialOrder, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    geAVL6381SerialOrder = enumSerialOrder;
    
    r = DTMB_SetMpegSerialOrder_6381(enumSerialOrder,pAVLChip);

    return r;
}

AVL6381_ErrorCode DTMB_SetMpegSerialSyncPulse_6381(AVL6381_MPEG_SERIAL_SYNC enumSerialSyncPulse, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_seri_sync_1_pulse_caddr,enumSerialSyncPulse);

    return r;
}


AVL6381_ErrorCode SetMpegSerialSyncPulse_6381(AVL6381_MPEG_SERIAL_SYNC enumSerialSyncPulse, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    geAVL6381SerialSync = enumSerialSyncPulse;
    
    r = DTMB_SetMpegSerialSyncPulse_6381(enumSerialSyncPulse,pAVLChip);

    return r;
}


AVL6381_ErrorCode DTMB_SetMpegErrorBit_6381(AVL6381_MPEG_ErrorBit enumErrorBit, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    
    r = AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_mpeg_ts_error_bit_en_caddr,enumErrorBit);

    return r;
}


AVL6381_ErrorCode SetMpegErrorBit_6381(AVL6381_MPEG_ErrorBit enumErrorBit, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    geAVL6381ErrorBit = enumErrorBit;
    
    r = DTMB_SetMpegErrorBit_6381(enumErrorBit, pAVLChip);

    return r;
}

AVL6381_ErrorCode DTMB_SetMpegErrorPola_6381(AVL6381_MPEG_ErrorBitPola enumErrorPola, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    
    r = AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_pkt_err_pol_caddr,enumErrorPola);

    return r;
}


AVL6381_ErrorCode SetMpegErrorPola_6381(AVL6381_MPEG_ErrorBitPola enumErrorPola, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    geAVL6381ErrorPola = enumErrorPola;
    
    r = DTMB_SetMpegErrorPola_6381(enumErrorPola, pAVLChip);

    return r;
}


AVL6381_ErrorCode DTMB_SetMpegValidPola_6381(AVL6381_MPEG_ValidPola enumValidPola, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    
    r = AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_mpeg_valid_inv_caddr,enumValidPola);

    return r;
}




AVL6381_ErrorCode SetMpegValidPola_6381(AVL6381_MPEG_ValidPola enumValidPola, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    geAVL6381ValidPola = enumValidPola;

    r = DTMB_SetMpegValidPola_6381(enumValidPola,pAVLChip);

    return r;
}


AVL6381_ErrorCode DTMB_SetMpegPacketLen_6381(AVL6381_MPEG_Packet_Len enumPacketLen, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = AVL6381_II2C_Write8(pAVLChip,rc_AVL6381_DTMB_ts0_tsp1_caddr,enumPacketLen);

    return r;
}



AVL6381_ErrorCode SetMpegPacketLen_6381(AVL6381_MPEG_Packet_Len enumPacketLen, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    geAVL6381PacketLen = enumPacketLen;

    r = DTMB_SetMpegPacketLen_6381(enumPacketLen, pAVLChip);

    return r;
}


AVL6381_ErrorCode DTMB_EnableMpegContinuousMode_6381(AVL6381_uint32 uiClockFreq_Hz, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiMpegRefClkFreq_Hz = 0;

    r =AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_cont_mpegclk_en_caddr, 1 );
    r |= AVL6381_II2C_Read32(pAVLChip, rc_AVL6381_DTMB_fec_clk_Hz_iaddr, &uiMpegRefClkFreq_Hz);

    if(gsAVL6381_Config.m_MPEG_Info.enumSerialParallel == AVL6381_MPEG_SERIAL)
    {
        r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_cntns_pkt_seri_rate_frac_n_iaddr, uiClockFreq_Hz);        
        r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_cntns_pkt_seri_rate_frac_d_iaddr, uiMpegRefClkFreq_Hz);
    }
    else
    {
        r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_cntns_pkt_para_rate_frac_n_iaddr,uiClockFreq_Hz);     
        r |= AVL6381_II2C_Write32(pAVLChip, rc_AVL6381_DTMB_cntns_pkt_para_rate_frac_d_iaddr, uiMpegRefClkFreq_Hz);
    }

    return r;
}


AVL6381_ErrorCode EnableMpegOutput_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_mpeg_bus_tri_enb, 0xfff);

    return r;
}


AVL6381_ErrorCode TunerI2C_Initialize_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 bit_rpt_divider = 0;
    AVL6381_uint32 uiTemp = 0;


    r = AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_tuner_i2c_srst, 1);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_tuner_hw_i2c_bit_rpt_cntrl, 0x6);
    r |= AVL6381_II2C_Read32(pAVLChip, hw_AVL6381_tuner_i2c_cntrl, &uiTemp);
    uiTemp = (uiTemp&0xFFFFFFFE);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_tuner_i2c_cntrl, uiTemp);
    bit_rpt_divider = (0x2A)*(gsAVL6381_Config.m_DemodFrequency_Hz/1000)/(240*1000);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_tuner_hw_i2c_bit_rpt_clk_div, bit_rpt_divider);
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_tuner_i2c_srst, 0);

    return r;
}

AVL6381_ErrorCode DTMB_SetAGCPola_6381(AVL6381_AGCPola enumAGCPola, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_DTMB_ifagc_pol_caddr, enumAGCPola);

    return r;
}


AVL6381_ErrorCode SetAGCPola_6381(AVL6381_AGCPola enumAGCPola, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    
    geAVL6381AGCPola = enumAGCPola;

    r = DTMB_SetAGCPola_6381(enumAGCPola,pAVLChip);

    return r;
}

AVL6381_ErrorCode EnableAGC_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_rfagc_tri_enb, 1);

    return r;
}

AVL6381_ErrorCode ResetPER_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiTemp = 0;

    gsAVL6381ErrorStat.m_LostLock = 0;
    gsAVL6381ErrorStat.m_SwCntPktErrors.m_HighWord = 0;
    gsAVL6381ErrorStat.m_SwCntPktErrors.m_LowWord = 0;
    gsAVL6381ErrorStat.m_SwCntNumPkts.m_HighWord = 0;
    gsAVL6381ErrorStat.m_SwCntNumPkts.m_LowWord = 0;
    gsAVL6381ErrorStat.m_PktErrors.m_HighWord = 0;
    gsAVL6381ErrorStat.m_PktErrors.m_LowWord = 0;
    gsAVL6381ErrorStat.m_NumPkts.m_HighWord = 0;
    gsAVL6381ErrorStat.m_NumPkts.m_LowWord = 0;

    r = AVL6381_II2C_Read32(pAVLChip, hw_AVL6381_esm_cntrl, &uiTemp);
    uiTemp |= 0x00000001;
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_esm_cntrl, uiTemp);

    if(AVL6381_DEMOD_MODE_DTMB == gsAVL6381_Config.m_current_demod_mode)
    {
        r |= AVL6381_II2C_Write8(pAVLChip, rs_AVL6381_DTMB_lost_lock_caddr, 0);
    }
    else
    {
        return AVL6381_EC_GeneralFail;
    }

    r |= AVL6381_II2C_Read32(pAVLChip, hw_AVL6381_esm_cntrl, &uiTemp);
    uiTemp |= 0x00000008;
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_esm_cntrl, uiTemp);
    uiTemp |= 0x00000001;
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_esm_cntrl, uiTemp);
    uiTemp &= 0xFFFFFFFE;
    r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_esm_cntrl, uiTemp);

    return r;
}

AVL6381_ErrorCode ResetErrorStat_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiTemp = 0;

    r = AVL6381_II2C_Read32(pAVLChip,hw_AVL6381_auto1_manual0_mode,&uiTemp);
    if(uiTemp == ERROR_STAT_AUTO)//auto mode
    {
    	HI_INFO_TUNER("func=%s  ERROR_STAT_AUTO\n",__func__);
        //reset auto error stat
        r |= AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_tick_clear_req,0);
        r |= AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_tick_clear_req,1);
        r |= AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_tick_clear_req,0);
    }else{
		HI_INFO_TUNER("func=%s  ERROR_STAT_MANUAL\n",__func__);
    }

    //only reset PER, since currently, final BER isn't meaningful
    r |= ResetPER_6381(pAVLChip);

    return r;
}

AVL6381_ErrorCode InitErrorStat_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    
    AVL6381_uint64 time_tick_num = {0,0};

    r  = AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_auto1_manual0_mode,ERROR_STAT_AUTO);
    r |= AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_timetick1_bytetick0,ERROR_STAT_TIME);

    AVL6381_Multiply32(&time_tick_num, gsAVL6381_Config.m_FECFrequency_Hz/1000, 1000);
    r |= AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_time_tick_low,time_tick_num.m_LowWord);
    r |= AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_time_tick_high,time_tick_num.m_HighWord);

    r |= AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_byte_tick_low,gsAVL6381_Config.m_ErrorStat_Info.m_Number_Threshold_Byte);
    r |= AVL6381_II2C_Write32(pAVLChip,hw_AVL6381_byte_tick_high,0);//high 32-bit is not used

    r |= ResetErrorStat_6381(pAVLChip);

    return r;
}



//-------------------------
AVL6381_ErrorCode DTMB_GetLockStatus_6381(AVL6381_puint32 pucLockStatus, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uchar ucLockStatus = 0;

    r = AVL6381_II2C_Read8(pAVLChip, rs_AVL6381_DTMB_fec_lock_caddr, &ucLockStatus);

    *pucLockStatus = ucLockStatus;

    return r;
}


AVL6381_ErrorCode AVL6381_GetLockStatus(AVL6381_puint32 puiLockStatus, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    *puiLockStatus = 0;

    r = DTMB_GetLockStatus_6381(puiLockStatus, pAVLChip);

    return r;
}

AVL6381_ErrorCode DTMB_Halt_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiTemp = 0;

    r = AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_DTMB_lock_status_iaddr, &uiTemp);
    uiTemp &= 0xFFFFFEFF;
    r |= AVL6381_II2C_Write32(pAVLChip, rs_AVL6381_DTMB_lock_status_iaddr, uiTemp);

    return r;
}


AVL6381_ErrorCode AVL6381_Halt(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = SendRxOP_6381(AVL6381_OP_RX_HALT, pAVLChip);
    //This delay is needed to make sure the command gets processed by the firmware incase it is busy doing something else.
    tuner_mdelay(2);

    r = DTMB_Halt_6381(pAVLChip);
    
    return r;
}

AVL6381_ErrorCode DTMB_GetRunningLevel_6381(AVL6381_RunningLevel *pRunningLevel, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uchar uiState = 0;

    r = AVL6381_II2C_Read8(pAVLChip, rs_AVL6381_DTMB_demod_status_caddr, &uiState);

    if(uiState == 0)
    {
        *pRunningLevel = AVL6381_RL_HALT;
    }
    else
    {
        *pRunningLevel = AVL6381_RL_NORMAL;
    }

    return r;
}

AVL6381_ErrorCode DVBC_GetRunningLevel_6381(AVL6381_RunningLevel *pRunningLevel, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiState = 0;

    r = AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_DVBC_mode_status_iaddr, &uiState);

    if(uiState == 0)
    {
        *pRunningLevel = AVL6381_RL_HALT;
    }
    else
    {
        *pRunningLevel = AVL6381_RL_NORMAL;
    }

    return r;
}

AVL6381_ErrorCode GetRunningLevel_6381(AVL6381_RunningLevel *pRunningLevel, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    
    r = DTMB_GetRunningLevel_6381(pRunningLevel,pAVLChip);

    return r;
}


AVL6381_ErrorCode DTMB_AutoLockChannel_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = AVL6381_II2C_Write8(pAVLChip, rc_AVL6381_get_config_from_tps_caddr, 1);//get channel info from TPS

    r |= AVL6381_II2C_Write8(pAVLChip, rs_AVL6381_DTMB_fixed_header_caddr, 0);//not fixed header
    
    r |= AVL6381_II2C_Write32(pAVLChip, rs_AVL6381_DTMB_demod_lock_status_iaddr, 0);//set the unlock status

    r |= SendRxOP_6381(AVL6381_OP_RX_ACQUIRE,pAVLChip);

    return r;
}


AVL6381_ErrorCode AutoLockChannel_6381(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

   	HI_INFO_TUNER("---->[%s,%d] DTMB_AutoLockChannel_6381 \n",__func__,__LINE__);
    r = DTMB_AutoLockChannel_6381(pAVLChip);

    return r;
}


AVL6381_ErrorCode AVL6381_AutoLock(struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_RunningLevel enumRunningLevel = AVL6381_RL_NORMAL;
    AVL6381_uint32  uiCnt = 0;

    r = AVL6381_Halt(pAVLChip);
    if(r != AVL6381_EC_OK)
    {
        return(r);
    }

    while((enumRunningLevel != AVL6381_RL_HALT) && (uiCnt<10))
    {
        r = GetRunningLevel_6381(&enumRunningLevel,pAVLChip);
        if(r != AVL6381_EC_OK)
        {
            return(r);
        }
        tuner_mdelay(10);
        uiCnt++;
    }

    if(enumRunningLevel != AVL6381_RL_HALT)
    {
        return AVL6381_EC_RUNNING;
    }

    r |= AutoLockChannel_6381(pAVLChip);

    return r;
}

//=============set mode=============
AVL6381_ErrorCode AVL6381_GetMode(AVL6381_puchar pucurrent_mode, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 mode_data;

    r = AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_current_active_mode_iaddr, &mode_data);
    *pucurrent_mode = (AVL6381_uchar)mode_data;
    return r;
}

AVL6381_ErrorCode AVL6381_SetMode(AVL6381_uchar mode, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiTimeoutCount = 20;
    AVL6381_uint32 uiDelayUnit = 10; /* Time-out window 10*20 ms */
    AVL6381_uint32 uiCounter = 0;
    AVL6381_uchar ucPLLIndex = 0;
    AVL6381_uint32 uiMaxRetries = 10;
    AVL6381_uint32 uiIndex = 0;
    AVL6381_uchar r_mode;

    r = AVL6381_GetMode(&r_mode, pAVLChip);
    if(r_mode == mode)
    {
    	HI_INFO_TUNER("AVL6381_SetMode r_mode=mode=%d\n",mode);
        return AVL6381_EC_OK;
    }
	else
		HI_INFO_TUNER("AVL6381_SetMode r_mode= %d mode=%d\n",r_mode, mode);
    
    gsAVL6381_Config.ucStartupMode = mode;
    gsAVL6381_Config.m_current_demod_mode = mode;
    
    if(gAVL6381freezeflag == 1)
    {
        r |= AVL6381_Halt(pAVLChip);
        if(AVL6381_EC_OK == r )
        {
            while (AVL6381_EC_OK != GetRxOP_Status_6381(pAVLChip))
            {
                if (uiMaxRetries < uiIndex++)
                {
                    r |= AVL6381_EC_RUNNING;
                    return r;
                 }
                 tuner_mdelay(uiDelayUnit);
             }
         }
         r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_cpu_ddc_srst, 0);
         tuner_mdelay(1);
         r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_cpu_ddc_srst, 1);                                                        
         r |= AVL6381_II2C_Write32(pAVLChip, rs_AVL6381_core_ready_word_iaddr, 0x0);
         r |= SendRxOP_6381(AVL6381_OP_RX_CHANGE_MODE,pAVLChip);
		 if(AVL6381_EC_OK == r )
		 {
			 uiIndex = 0;
			 while (AVL6381_EC_OK != GetRxOP_Status_6381(pAVLChip))
			 {
				 if ((10*uiMaxRetries) < uiIndex++)
				 {
					 r |= AVL6381_EC_RUNNING;
					 return r;
				 }
				 tuner_mdelay(uiDelayUnit);
			 }
		 }
		 uiIndex = 0;
         while (AVL6381_EC_OK != CheckChipReady_6381(pAVLChip))
         {
             if (uiMaxRetries < uiIndex++)
             {
                 return AVL6381_EC_GeneralFail;
             }
             tuner_mdelay(uiDelayUnit);
         }
  
         ucPLLIndex = gsAVL6381_Config.m_InputSignal_Info.ueRefConfig * 2;
		 
         r = AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_c306_top_srst, 1);
         r |= SetPLL_6381(&gAVL6381PLLConfig[ucPLLIndex], pAVLChip);
         r |= AVL6381_II2C_Write32(pAVLChip, rs_AVL6381_core_ready_word_iaddr, 0x0);
         r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_c306_top_srst, 0);
    }
    else
    {
        ucPLLIndex = gsAVL6381_Config.m_InputSignal_Info.ueRefConfig * 2;
        r |= IBase_Initialize_6381(&(gAVL6381PLLConfig[ucPLLIndex]),pAVLChip);
    }
    tuner_mdelay(uiDelayUnit);
    for(uiCounter = 0; uiCounter < uiTimeoutCount; uiCounter++)
    {
        r = CheckChipReady_6381(pAVLChip);
        if(r == AVL6381_EC_OK)
        {
            break;
        }
        tuner_mdelay(uiDelayUnit);
    }
    
    /* Receiver Init */
    r |= IRx_Initialize_6381(pAVLChip);
    
    if(mode == AVL6381_DEMOD_MODE_DTMB)
    {
        r |= DTMB_SetSymbolRate_6381(gsAVL6381_Config.m_InputSignal_Info.uiDTMBSymbolRateHz, pAVLChip);
    }
    
    r |= SetMpegMode_6381(pAVLChip);
    r |= SetMpegSerialPin_6381(geAVL6381SerialPin,pAVLChip);
    r |= SetMpegSerialOrder_6381(geAVL6381SerialOrder,pAVLChip);
    r |= SetMpegSerialSyncPulse_6381(geAVL6381SerialSync,pAVLChip);
    r |= SetMpegErrorBit_6381(geAVL6381ErrorBit,pAVLChip);
    r |= SetMpegErrorPola_6381(geAVL6381ErrorPola,pAVLChip);
    r |= SetMpegValidPola_6381(geAVL6381ValidPola,pAVLChip);
    r |= SetMpegPacketLen_6381(geAVL6381PacketLen,pAVLChip);
    
    if(gsAVL6381_Config.m_MPEG_Info.enumConti == AVL6381_MPEG_CONTINUOUS_ENABLE)
    {
        if(gsAVL6381_Config.m_MPEG_Info.enumSerialParallel == AVL6381_MPEG_SERIAL)
        {
            r |= DTMB_EnableMpegContinuousMode_6381(guiDTMBSerialMPEGContinuousHz,pAVLChip);
        }
        else
        {
            r |= DTMB_EnableMpegContinuousMode_6381(guiParallelMPEGContinuousHz,pAVLChip);
        }
    }
    

    r |= EnableMpegOutput_6381(pAVLChip);

    /* Reset tuner I2C module */
    r |= TunerI2C_Initialize_6381(pAVLChip);

    /* Set AGC */
    r |= SetAGCPola_6381(geAVL6381AGCPola,pAVLChip);

    r |= EnableAGC_6381(pAVLChip);

    /* Error statistics Setting */
    r |= InitErrorStat_6381(pAVLChip);

	r |= AVL6381_II2C_Write32( pAVLChip, rc_AVL6381_DVBC_ddc__clks_per_2x_symbol_dvbc_iaddr, 10);

    return r;
}


AVL6381_ErrorCode DTMB_NoSignalDetection_6381(AVL6381_uint32 *puiNoSignal, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint16 usTemp = 0;

    *puiNoSignal = 0;

    r = AVL6381_II2C_Read16(pAVLChip, rs_AVL6381_DTMB_num_early_reacq_saddr, &usTemp);
    
    if(usTemp >= 4)
    {
        *puiNoSignal = 1;
    }

    return r;
}


AVL6381_ErrorCode AVL6381_NoSignalDetection(AVL6381_uint32 *puiNoSignal,struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

    r = DTMB_NoSignalDetection_6381(puiNoSignal,pAVLChip);

    return r;
}


AVL6381_ErrorCode AVL6381_DTMB_GetSignalInfo(AVL6381_DTMB_SignalInfo *pSignalInfo, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiTemp = 0;

    r = AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_DTMB_mode_status_iaddr, &uiTemp);
    pSignalInfo->m_HeaderMode = (AVL6381_DTMB_FrameHeaderMode)(uiTemp & 0x00000003);
    pSignalInfo->m_CodeRate = (AVL6381_DTMB_FEC_Rate)((uiTemp >> 8) & 0x00000003);
    pSignalInfo->m_CarrierMode = (AVL6381_DTMB_CarrierMode)((uiTemp >> 16) & 0x00000001);
    pSignalInfo->m_InterleaveMode = (AVL6381_DTMB_InterleaveMode)((uiTemp >> 24) & 0x00000001);

    r |= AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_DTMB_modulation_status_iaddr, &uiTemp);
    pSignalInfo->m_Modulation = (AVL6381_DTMB_ModulationMode)(uiTemp & 0x00000003);
    pSignalInfo->m_MappingMode = (AVL6381_DTMB_NR_Mode)((uiTemp >> 8) & 0x00000001);
    return r;
}

AVL6381_ErrorCode AVL6381_DTMB_GetSNR(AVL6381_puint32 puiSNR_db, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint16 uiTemp = 0;

    r = AVL6381_II2C_Read16(pAVLChip, rs_AVL6381_DTMB_snr_dB_x100_avg_saddr, &uiTemp);

    *puiSNR_db = (AVL6381_uint32)uiTemp;

    return r;
}

AVL6381_ErrorCode AVL6381_DTMB_GetSignalLevel(AVL6381_puint32 puiSignalLevel, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiTemp = 0;

    r = AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_DTMB_signal_strength_iaddr, &uiTemp);

    if( AVL6381_EC_OK == r)
    {
        uiTemp += 0x2000000;
        uiTemp &= 0x3ffffff;
        *puiSignalLevel = uiTemp >> 10;
    }
	HI_INFO_TUNER("func= %s SignalLevel=%d \n",__func__,*puiSignalLevel);

    return r;
}

AVL6381_ErrorCode AVL6381_DTMB_GetSignalQuality(AVL6381_puint32 puiSignal_Quality, AVL6381_int32 iNormalization_Factor, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiEstimated_SNR = 0;
    AVL6381_DTMB_SignalInfo SignalInfo;
    AVL6381_int32 iThreshold_SNR = 0, window = 0;
    AVL6381_int32 iSignal_Quality = 0,iEstimated_SNR = 0;

    if ((iNormalization_Factor < 4) || (iNormalization_Factor >6))
    {
        iNormalization_Factor = 4;
    }

    r = AVL6381_DTMB_GetSNR(&uiEstimated_SNR, pAVLChip);

    if(r != AVL6381_EC_OK)
    {
        *puiSignal_Quality = 0;
        return r;
    }

    r |= AVL6381_DTMB_GetSignalInfo(&SignalInfo, pAVLChip);

    if(r != AVL6381_EC_OK)
    {
        *puiSignal_Quality = 0;
        return r;
    }

    iEstimated_SNR = uiEstimated_SNR;

    if (SignalInfo.m_MappingMode == RX_NR_ON)
        iThreshold_SNR = THRESHOLD_NR_595;
    else if (SignalInfo.m_HeaderMode == RX_PN420)
        iThreshold_SNR   =  AVL6381_Table_420[SignalInfo.m_Modulation][SignalInfo.m_CodeRate];
    else if (SignalInfo.m_HeaderMode == RX_PN595)
        iThreshold_SNR   =  AVL6381_Table_595[SignalInfo.m_Modulation][SignalInfo.m_CodeRate];
    else if (SignalInfo.m_HeaderMode == RX_PN945)
        iThreshold_SNR   =  AVL6381_Table_945[SignalInfo.m_Modulation][SignalInfo.m_CodeRate];


    if ((iThreshold_SNR == 0) || (iEstimated_SNR == 0))
    {
         *puiSignal_Quality = 0;
    }
    else
    {

        if ((iThreshold_SNR * 2) < 100)
        {
            window = iThreshold_SNR * 2;
        }
        else
        {
            window = 100;
        }

        iSignal_Quality = (iEstimated_SNR - (((iThreshold_SNR * 10) - (iNormalization_Factor * window))))/(window/10);

        if (iSignal_Quality > 100)
        {
            *puiSignal_Quality = 100;
        }
        else if (iSignal_Quality < 0)
        {
            *puiSignal_Quality = 0;
        }
        else
        {
            *puiSignal_Quality = iSignal_Quality;
        }
    }

    return r;
}

AVL6381_ErrorCode AVL6381_GetSQI(AVL6381_puint32 puiSignalQuality, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiLockStatus = 0;
    
    *puiSignalQuality = 0;

    r = AVL6381_GetLockStatus(&uiLockStatus,pAVLChip);
    if( uiLockStatus == 0)
    {
        return r;
    }

    r = AVL6381_DTMB_GetSignalQuality(puiSignalQuality, 4, pAVLChip);

    return r;
}


AVL6381_ErrorCode AVL6381_GetBER(AVL6381_puint32 puiBER_x10e9, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiHwCntBitErrors = 0;
    AVL6381_uint32 uiHwCntNumBits = 0;
    AVL6381_uint32 uiTemp = 0;
    AVL6381_uchar ucTemp = 0;
    AVL6381_uint16 usTemp = 0;
    AVL6381_uint64 uiTemp64 = {0,0};

    r = AVL6381_II2C_Read32(pAVLChip, hw_AVL6381_ber_err_cnt, &uiHwCntBitErrors);
    r |= AVL6381_II2C_Read32(pAVLChip, hw_AVL6381_byte_num, &uiHwCntNumBits);

	uiHwCntNumBits <<= 3;

    if(uiHwCntNumBits > (1 << 31))
    {
        r |= AVL6381_II2C_Read32(pAVLChip,  hw_AVL6381_esm_cntrl, &uiTemp);
        uiTemp |= 0x00000002;
        r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_esm_cntrl, uiTemp);
        r |= AVL6381_II2C_Read32(pAVLChip,  hw_AVL6381_ber_err_cnt, &uiHwCntBitErrors);
        r |= AVL6381_II2C_Read32(pAVLChip,  hw_AVL6381_byte_num, &uiHwCntNumBits);
        uiTemp &= 0xFFFFFFFD;
        r |= AVL6381_II2C_Write32(pAVLChip, hw_AVL6381_esm_cntrl, uiTemp);
        uiHwCntNumBits <<= 3;
        AVL6381_Add32To64(&gsAVL6381ErrorStat.m_SwCntNumBits, uiHwCntNumBits);
        AVL6381_Add32To64(&gsAVL6381ErrorStat.m_SwCntBitErrors, uiHwCntBitErrors);
        uiHwCntNumBits = 0;
        uiHwCntBitErrors = 0;
    }
	
    gsAVL6381ErrorStat.m_NumBits.m_HighWord = gsAVL6381ErrorStat.m_SwCntNumBits.m_HighWord;
    gsAVL6381ErrorStat.m_NumBits.m_LowWord = gsAVL6381ErrorStat.m_SwCntNumBits.m_LowWord;
    AVL6381_Add32To64(&gsAVL6381ErrorStat.m_NumBits, uiHwCntNumBits);
    gsAVL6381ErrorStat.m_BitErrors.m_HighWord = gsAVL6381ErrorStat.m_SwCntBitErrors.m_HighWord;
    gsAVL6381ErrorStat.m_BitErrors.m_LowWord = gsAVL6381ErrorStat.m_SwCntBitErrors.m_LowWord;
    AVL6381_Add32To64(&gsAVL6381ErrorStat.m_BitErrors, uiHwCntBitErrors);

	// Compute the BER
    AVL6381_Multiply32(&uiTemp64, gsAVL6381ErrorStat.m_BitErrors.m_LowWord, (AVL6381_uint32)1e9);
    uiTemp = AVL6381_Divide64(gsAVL6381ErrorStat.m_NumBits, uiTemp64);
    gsAVL6381ErrorStat.m_BER = uiTemp;
    *puiBER_x10e9 = uiTemp;
	
    if(AVL6381_DEMOD_MODE_DTMB == gsAVL6381_Config.m_current_demod_mode)
    {
        r |= AVL6381_II2C_Read8(pAVLChip, rs_AVL6381_DTMB_lost_lock_caddr, &ucTemp);
        usTemp = ucTemp;
    }
    else
    {
        return (AVL6381_EC_GeneralFail);
    }
    gsAVL6381ErrorStat.m_LostLock = usTemp;

    return r;
}


AVL6381_ErrorCode AVL6381_DTMB_GetCarrierFreqOffset(AVL6381_pint32 piFreqOffsetHz, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_uint32 uiTemp = 0;
    AVL6381_int32 iTemp = 0;
    AVL6381_int64 iProduct = {0,0};
    AVL6381_uint32 uiBinOffset = 0;
    AVL6381_uint32 uiRefFreqHz = 0;

    r = AVL6381_II2C_Read32(pAVLChip, rc_AVL6381_DTMB_sample_rate_Hz_iaddr, &uiRefFreqHz);
    r |= AVL6381_II2C_Read32(pAVLChip, hw_AVL6381_DTMB_cfo_freq_est, &uiTemp);
    iTemp = (AVL6381_int32)(uiTemp << 2);
    iTemp >>= 2;
    AVL6381_Multiply32By16(&iProduct, iTemp, (AVL6381_int16)(uiRefFreqHz/(AVL6381_int16)10000));
    iTemp = (AVL6381_int32)(((iProduct.m_HighWord & 0x000001FF) << 23) + (iProduct.m_LowWord >> 9));
    AVL6381_Multiply32By16(&iProduct, iTemp, 625);
    iTemp = (AVL6381_int32)(((iProduct.m_HighWord & 0x0001FFFF) << 15) + (iProduct.m_LowWord >> 17));
    r |= AVL6381_II2C_Read32(pAVLChip, rs_AVL6381_DTMB_carrier_freq_bin_Hz_iaddr, &uiBinOffset);
    iTemp -= (AVL6381_int32)uiBinOffset;

    *piFreqOffsetHz = iTemp;

    return r;
}

AVL6381_ErrorCode AVL6381_DTMB_Get_CarrierFreq_Offset(AVL6381_puint32 puiFreq, AVL6381_pint32 piFreqOffsetHz, struct AVL6381_Chip * pAVLChip)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;

	r = AVL6381_II2C_Read32(pAVLChip, rc_AVL6381_DTMB_symbol_rate_Hz_iaddr, puiFreq);
	if(r != AVL6381_EC_OK)
	{
		HI_INFO_TUNER("func= %s get CarrierFreq failed\n",__func__);
		return AVL6381_EC_GeneralFail;
	}
	
	r = AVL6381_DTMB_GetCarrierFreqOffset(piFreqOffsetHz, pAVLChip);
	if(r != AVL6381_EC_OK)
	{
		HI_INFO_TUNER("func= %s get CarrierFreq Offset failed\n",__func__);
		return AVL6381_EC_GeneralFail;
	}	

	return r;	
}



