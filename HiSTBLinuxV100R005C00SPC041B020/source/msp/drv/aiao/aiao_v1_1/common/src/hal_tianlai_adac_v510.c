#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <asm/io.h>

#include "hal_tianlai_adac_v510.h"
#include "hi_reg_common.h"

#define DEV_NAME "tianlai"

volatile S_Audio_Codec_REGS_TYPE   *g_pstRegTianlaiBase;// = (S_Audio_Codec_REGS_TYPE*)IO_ADDRESS(TIANLAI_BASE_ADDR_V100B);

#define TIANLAI_REG_SIZE (0x1000)   /*add*/

static HI_S32 HAL_TIANLAI_InitBaseAddr(HI_VOID)
{
#if defined CHIP_TYPE_hi3751v100b
    g_pstRegTianlaiBase = (S_Audio_Codec_REGS_TYPE *)ioremap_nocache(TIANLAI_BASE_ADDR_V100B, TIANLAI_REG_SIZE);
#elif defined CHIP_TYPE_hi3751v600
    g_pstRegTianlaiBase = (S_Audio_Codec_REGS_TYPE *)ioremap_nocache(TIANLAI_BASE_ADDR_V600, TIANLAI_REG_SIZE);
#elif defined CHIP_TYPE_hi3751lv500
    g_pstRegTianlaiBase = (S_Audio_Codec_REGS_TYPE *)ioremap_nocache(TIANLAI_BASE_ADDR_LV500, TIANLAI_REG_SIZE);
#elif defined CHIP_TYPE_hi3751v500
    g_pstRegTianlaiBase = (S_Audio_Codec_REGS_TYPE *)ioremap_nocache(TIANLAI_BASE_ADDR_V500, TIANLAI_REG_SIZE);
#elif defined CHIP_TYPE_hi3751v620
    g_pstRegTianlaiBase = (S_Audio_Codec_REGS_TYPE *)ioremap_nocache(TIANLAI_BASE_ADDR_V620, TIANLAI_REG_SIZE);

#else
    g_pstRegTianlaiBase = (S_Audio_Codec_REGS_TYPE*)IO_ADDRESS(TIANLAI_BASE_ADDR_V100);
#endif

    return HI_SUCCESS;
}

HI_U32 HAL_TIANLAI_SetTopInfo(S5_TIANLAI_TOP_INFO_E enTopInf)
{
    U_TOP_INFO TopInfo;
    TopInfo.u32 = g_pstRegTianlaiBase->TOP_INFO.u32;
    TopInfo.bits.switch_info = enTopInf;
    g_pstRegTianlaiBase->TOP_INFO.u32= TopInfo.u32;

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_AdcSetReset(HI_BOOL bReset)
{
    U_ADC_CFG_VALUE AdcCfgValReg;
    AdcCfgValReg.u32 = g_pstRegTianlaiBase->ADC_CFG_VALUE.u32;
    AdcCfgValReg.bits.adcl_rst_n = bReset;
    AdcCfgValReg.bits.adcr_rst_n = bReset;

    g_pstRegTianlaiBase->ADC_CFG_VALUE.u32= AdcCfgValReg.u32;


    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_AdcSetEn(HI_BOOL bEn)
{
    U_ADC_CFG_VALUE AdcCfgValReg;
    AdcCfgValReg.u32 = g_pstRegTianlaiBase->ADC_CFG_VALUE.u32;
    AdcCfgValReg.bits.adcl_enable = bEn;
    AdcCfgValReg.bits.adcr_enable = bEn;
    g_pstRegTianlaiBase->ADC_CFG_VALUE.u32= AdcCfgValReg.u32;

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_AdcSetBitWidth(S5_TIANLAI_BIT_DEPTH_E enBitWid)
{
    U_ADC_CFG_VALUE AdcCfgValReg;
    AdcCfgValReg.u32 = g_pstRegTianlaiBase->ADC_CFG_VALUE.u32;
    AdcCfgValReg.bits.i2s1_data_bits = enBitWid;
    AdcCfgValReg.bits.i2s2_data_bits = enBitWid;
    g_pstRegTianlaiBase->ADC_CFG_VALUE.u32= AdcCfgValReg.u32;

    return HI_SUCCESS;
}

/*bypass:1  nobypass:0*/
HI_U32 HAL_TIANLAI_AdcSetDigBypass(HI_BOOL bBypass)
{
    U_ADC_CFG_VALUE AdcCfgValReg;
    AdcCfgValReg.u32 = g_pstRegTianlaiBase->ADC_CFG_VALUE.u32;
    AdcCfgValReg.bits.dig_bypass = bBypass;
    g_pstRegTianlaiBase->ADC_CFG_VALUE.u32= AdcCfgValReg.u32;

    return HI_SUCCESS;
}

HI_U32 HAL_TIANLAI_AdcSetDigLoopEn(HI_BOOL bLoopEn)
{
    U_ADC_CFG_VALUE AdcCfgValReg;
    AdcCfgValReg.u32 = g_pstRegTianlaiBase->ADC_CFG_VALUE.u32;
    AdcCfgValReg.bits.dig_loop = bLoopEn;
    g_pstRegTianlaiBase->ADC_CFG_VALUE.u32= AdcCfgValReg.u32;

    return HI_SUCCESS;
}

/*default : S5_TIANLAI_SAMPLE_RATE_MCLK_128_2*/
static HI_U32 HAL_TIANLAI_AdcSampleRate(S5_TIANLAI_SAMPLERATE_E enSampleRate)
{
    U_ADC_CFG_VALUE AdcCfgValReg;
    AdcCfgValReg.u32 = g_pstRegTianlaiBase->ADC_CFG_VALUE.u32;
    AdcCfgValReg.bits.i2s1_fs_sel = enSampleRate;
    AdcCfgValReg.bits.i2s2_fs_sel = enSampleRate;
    g_pstRegTianlaiBase->ADC_CFG_VALUE.u32= AdcCfgValReg.u32;

    return HI_SUCCESS;
}

HI_U32 HAL_TIANLAI_AdcSetMute(HI_BOOL bMuteL, HI_BOOL bMuteR)
{
    U_ADC_REG_0_VALUE AdcReg0;
    AdcReg0.u32 = g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32;
    AdcReg0.bits.adcl_mute = bMuteL;
    AdcReg0.bits.adcr_mute = bMuteR;
    g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32= AdcReg0.u32;

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_AdcSetVolume(HI_U32 u32LeftVol, HI_U32 u32RightVol)
{
    U_ADC_REG_0_VALUE AdcReg0;
    AdcReg0.u32 = g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32;
    AdcReg0.bits.adcl_vol = u32LeftVol;
    AdcReg0.bits.adcr_vol = u32RightVol;
    g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32= AdcReg0.u32;

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_AdcSetHpfEn(HI_BOOL bHpfEnL, HI_BOOL bHpfEnR)
{
    U_ADC_REG_0_VALUE AdcReg0;
    AdcReg0.u32 = g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32;
    AdcReg0.bits.adcl_hpf_en = bHpfEnL;
    AdcReg0.bits.adcr_hpf_en = bHpfEnR;
    g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32= AdcReg0.u32;

    return HI_SUCCESS;
}

HI_U32 HAL_TIANLAI_AdcI2SSel(HI_U32 u32I2SSel_L, HI_U32 u32I2SSel_R)
{
    U_ADC_REG_0_VALUE AdcReg0;
    AdcReg0.u32 = g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32;
    AdcReg0.bits.adcl_i2ssel = u32I2SSel_L;
    AdcReg0.bits.adcr_i2ssel = u32I2SSel_R;
    g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32= AdcReg0.u32;

    return HI_SUCCESS;
}

/*L:0,R:1*/
HI_U32 HAL_TIANLAI_AdcLRSel(HI_BOOL bChnSelL, HI_U32 bChnSelR)
{
    U_ADC_REG_0_VALUE AdcReg0;
    AdcReg0.u32 = g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32;
    AdcReg0.bits.adcl_lrsel = bChnSelL;
    AdcReg0.bits.adcl_lrsel = bChnSelR;
    g_pstRegTianlaiBase->ADC_REG_0_VALUE.u32= AdcReg0.u32;

    return HI_SUCCESS;
}

HI_U32 HAL_TIANLAI_AdcSetMixPam(S5_TIANLAI_MIXER_TYPE_E enMixType, HI_BOOL bEn, HI_U32 u32Val)
{
    U_ADC_REG_1_VALUE AdcReg1;
    U_ADC_REG_2_VALUE AdcReg2;
    U_ADC_REG_3_VALUE AdcReg3;
    U_ADC_REG_4_VALUE AdcReg4;

    switch (enMixType)
    {
        case S5_TIANLAI_MIXER_TYPE_ADCL_DACL0:
            AdcReg1.u32 = g_pstRegTianlaiBase->ADC_REG_1_VALUE.u32;
            AdcReg1.bits.adcl2dacl0_mix_en  = bEn;
            AdcReg1.bits.adcl2dacl0_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_1_VALUE.u32= AdcReg1.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCR_DACL0:
            AdcReg1.u32 = g_pstRegTianlaiBase->ADC_REG_1_VALUE.u32;
            AdcReg1.bits.adcl2dacl0_mix_en  = bEn;
            AdcReg1.bits.adcl2dacl0_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_1_VALUE.u32= AdcReg1.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCL_DACR0:
            AdcReg1.u32 = g_pstRegTianlaiBase->ADC_REG_1_VALUE.u32;
            AdcReg1.bits.adcl2dacl0_mix_en  = bEn;
            AdcReg1.bits.adcl2dacl0_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_1_VALUE.u32= AdcReg1.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCR_DACR0:
            AdcReg1.u32 = g_pstRegTianlaiBase->ADC_REG_1_VALUE.u32;
            AdcReg1.bits.adcl2dacl0_mix_en  = bEn;
            AdcReg1.bits.adcl2dacl0_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_1_VALUE.u32= AdcReg1.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCL_DACL1:
            AdcReg2.u32 = g_pstRegTianlaiBase->ADC_REG_2_VALUE.u32;
            AdcReg2.bits.adcl2dacl1_mix_en  = bEn;
            AdcReg2.bits.adcl2dacl1_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_2_VALUE.u32= AdcReg2.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCR_DACL1:
            AdcReg2.u32 = g_pstRegTianlaiBase->ADC_REG_2_VALUE.u32;
            AdcReg2.bits.adcl2dacl1_mix_en  = bEn;
            AdcReg2.bits.adcl2dacl1_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_2_VALUE.u32= AdcReg2.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCL_DACR1:
            AdcReg2.u32 = g_pstRegTianlaiBase->ADC_REG_2_VALUE.u32;
            AdcReg2.bits.adcl2dacl1_mix_en  = bEn;
            AdcReg2.bits.adcl2dacl1_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_2_VALUE.u32= AdcReg2.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCR_DACR1:
            AdcReg2.u32 = g_pstRegTianlaiBase->ADC_REG_2_VALUE.u32;
            AdcReg2.bits.adcl2dacl1_mix_en  = bEn;
            AdcReg2.bits.adcl2dacl1_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_2_VALUE.u32= AdcReg2.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCL_DACL2:
            AdcReg3.u32 = g_pstRegTianlaiBase->ADC_REG_3_VALUE.u32;
            AdcReg3.bits.adcl2dacl2_mix_en  = bEn;
            AdcReg3.bits.adcl2dacl2_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_3_VALUE.u32= AdcReg3.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCR_DACL2:
            AdcReg3.u32 = g_pstRegTianlaiBase->ADC_REG_3_VALUE.u32;
            AdcReg3.bits.adcl2dacl2_mix_en  = bEn;
            AdcReg3.bits.adcl2dacl2_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_3_VALUE.u32= AdcReg3.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCL_DACR2:
            AdcReg3.u32 = g_pstRegTianlaiBase->ADC_REG_3_VALUE.u32;
            AdcReg3.bits.adcl2dacl2_mix_en  = bEn;
            AdcReg3.bits.adcl2dacl2_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_3_VALUE.u32= AdcReg3.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCR_DACR2:
            AdcReg3.u32 = g_pstRegTianlaiBase->ADC_REG_3_VALUE.u32;
            AdcReg3.bits.adcl2dacl2_mix_en  = bEn;
            AdcReg3.bits.adcl2dacl2_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_3_VALUE.u32= AdcReg3.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCL_DACL3:
            AdcReg4.u32 = g_pstRegTianlaiBase->ADC_REG_4_VALUE.u32;
            AdcReg4.bits.adcl2dacl3_mix_en  = bEn;
            AdcReg4.bits.adcl2dacl3_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_4_VALUE.u32= AdcReg4.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCR_DACL3:
            AdcReg4.u32 = g_pstRegTianlaiBase->ADC_REG_4_VALUE.u32;
            AdcReg4.bits.adcl2dacl3_mix_en  = bEn;
            AdcReg4.bits.adcl2dacl3_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_4_VALUE.u32= AdcReg4.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCL_DACR3:
            AdcReg4.u32 = g_pstRegTianlaiBase->ADC_REG_4_VALUE.u32;
            AdcReg4.bits.adcl2dacl3_mix_en  = bEn;
            AdcReg4.bits.adcl2dacl3_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_4_VALUE.u32= AdcReg4.u32;
            break;
        case S5_TIANLAI_MIXER_TYPE_ADCR_DACR3:
            AdcReg4.u32 = g_pstRegTianlaiBase->ADC_REG_4_VALUE.u32;
            AdcReg4.bits.adcl2dacl3_mix_en  = bEn;
            AdcReg4.bits.adcl2dacl3_mix_vol = u32Val;
            g_pstRegTianlaiBase->ADC_REG_4_VALUE.u32= AdcReg4.u32;
            break;
        default:
            break;

    }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_E enDACNum, HI_BOOL bReset)
{
    U_DAC0_CFG_VALUE DacCfgValReg0;
    U_DAC1_CFG_VALUE DacCfgValReg1;
    U_DAC2_CFG_VALUE DacCfgValReg2;
    U_DAC3_CFG_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32;
            DacCfgValReg0.bits.dacl_reset_n = bReset;
            DacCfgValReg0.bits.dacr_reset_n = bReset;
            g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32;
            DacCfgValReg1.bits.dacl_reset_n = bReset;
            DacCfgValReg1.bits.dacr_reset_n = bReset;
            g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32;
            DacCfgValReg2.bits.dacl_reset_n = bReset;
            DacCfgValReg2.bits.dacr_reset_n = bReset;
            g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32;
            DacCfgValReg3.bits.dacl_reset_n = bReset;
            DacCfgValReg3.bits.dacr_reset_n = bReset;
            g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSetEn(S5_TIANLAI_DAC_NUM_E enDACNum, HI_BOOL bEn)
{
    U_DAC0_CFG_VALUE DacCfgValReg0;
    U_DAC1_CFG_VALUE DacCfgValReg1;
    U_DAC2_CFG_VALUE DacCfgValReg2;
    U_DAC3_CFG_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32;
            DacCfgValReg0.bits.dacl_enable = bEn;
            DacCfgValReg0.bits.dacr_enable = bEn;
            g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32;
            DacCfgValReg1.bits.dacl_enable = bEn;
            DacCfgValReg1.bits.dacr_enable = bEn;
            g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32;
            DacCfgValReg2.bits.dacl_enable = bEn;
            DacCfgValReg2.bits.dacr_enable = bEn;
            g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32;
            DacCfgValReg3.bits.dacl_enable = bEn;
            DacCfgValReg3.bits.dacr_enable = bEn;
            g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSetBitWidth(S5_TIANLAI_DAC_NUM_E enDACNum, S5_TIANLAI_BIT_DEPTH_E enBitWid)
{
    U_DAC0_CFG_VALUE DacCfgValReg0;
    U_DAC1_CFG_VALUE DacCfgValReg1;
    U_DAC2_CFG_VALUE DacCfgValReg2;
    U_DAC3_CFG_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32;
            DacCfgValReg0.bits.i2s1_data_bits = enBitWid;
            DacCfgValReg0.bits.i2s2_data_bits = enBitWid;
            g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32;
            DacCfgValReg1.bits.i2s1_data_bits = enBitWid;
            DacCfgValReg1.bits.i2s2_data_bits = enBitWid;
            g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32;
            DacCfgValReg2.bits.i2s1_data_bits = enBitWid;
            DacCfgValReg2.bits.i2s2_data_bits = enBitWid;
            g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32;
            DacCfgValReg3.bits.i2s1_data_bits = enBitWid;
            DacCfgValReg3.bits.i2s2_data_bits = enBitWid;
            g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

/*bypass:1  nobypass:0*/
HI_U32 HAL_TIANLAI_DacSetDigBypass(S5_TIANLAI_DAC_NUM_E enDACNum, HI_BOOL bBypass)
{
    U_DAC0_CFG_VALUE DacCfgValReg0;
    U_DAC1_CFG_VALUE DacCfgValReg1;
    U_DAC2_CFG_VALUE DacCfgValReg2;
    U_DAC3_CFG_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32;
            DacCfgValReg0.bits.dig_bypass = bBypass;
            g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32;
            DacCfgValReg1.bits.dig_bypass = bBypass;
            g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32;
            DacCfgValReg2.bits.dig_bypass = bBypass;
            g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32;
            DacCfgValReg3.bits.dig_bypass = bBypass;
            g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacVolUpdate(S5_TIANLAI_DAC_NUM_E enDACNum, HI_BOOL bUpdate)
{
    U_DAC0_REG_0_VALUE DacCfgValReg00;
    U_DAC1_REG_0_VALUE DacCfgValReg01;
    U_DAC2_REG_0_VALUE DacCfgValReg02;
    U_DAC3_REG_0_VALUE DacCfgValReg03;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg00.u32 = g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32;
            DacCfgValReg00.bits.dacvu = bUpdate;
            g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32= DacCfgValReg00.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg01.u32 = g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32;
            DacCfgValReg01.bits.dacvu = bUpdate;
            g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32= DacCfgValReg01.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg02.u32 = g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32;
            DacCfgValReg02.bits.dacvu = bUpdate;
            g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32= DacCfgValReg02.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg03.u32 = g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32;
            DacCfgValReg03.bits.dacvu = bUpdate;
            g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32= DacCfgValReg03.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

/*bypass:1  nobypass:0*/
static HI_U32 HAL_TIANLAI_DacSampleRate(S5_TIANLAI_DAC_NUM_E enDACNum, S5_TIANLAI_SAMPLERATE_E enSampleRate)
{
    U_DAC0_CFG_VALUE DacCfgValReg0;
    U_DAC1_CFG_VALUE DacCfgValReg1;
    U_DAC2_CFG_VALUE DacCfgValReg2;
    U_DAC3_CFG_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32;
            DacCfgValReg0.bits.i2s1_fs_sel = enSampleRate;
            DacCfgValReg0.bits.i2s2_fs_sel = enSampleRate;
            g_pstRegTianlaiBase->DAC0_CFG_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32;
            DacCfgValReg1.bits.i2s1_fs_sel = enSampleRate;
            DacCfgValReg1.bits.i2s2_fs_sel = enSampleRate;
            g_pstRegTianlaiBase->DAC1_CFG_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32;
            DacCfgValReg2.bits.i2s1_fs_sel = enSampleRate;
            DacCfgValReg2.bits.i2s2_fs_sel = enSampleRate;
            g_pstRegTianlaiBase->DAC2_CFG_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32;
            DacCfgValReg3.bits.i2s1_fs_sel = enSampleRate;
            DacCfgValReg3.bits.i2s2_fs_sel = enSampleRate;
            g_pstRegTianlaiBase->DAC3_CFG_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_E enDACNum, HI_BOOL bSoftMuteEn)
{
    U_DAC0_REG_0_VALUE DacCfgValReg0;
    U_DAC1_REG_0_VALUE DacCfgValReg1;
    U_DAC2_REG_0_VALUE DacCfgValReg2;
    U_DAC3_REG_0_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32;
            DacCfgValReg0.bits.smutel = bSoftMuteEn;
            DacCfgValReg0.bits.smuter = bSoftMuteEn;
            g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32;
            DacCfgValReg1.bits.smutel = bSoftMuteEn;
            DacCfgValReg1.bits.smuter = bSoftMuteEn;
            g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32;
            DacCfgValReg2.bits.smutel = bSoftMuteEn;
            DacCfgValReg2.bits.smuter = bSoftMuteEn;
            g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32;
            DacCfgValReg3.bits.smutel = bSoftMuteEn;
            DacCfgValReg3.bits.smuter = bSoftMuteEn;
            g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_E enDACNum, HI_BOOL bSoftUnMuteEn)
{
    U_DAC0_REG_0_VALUE DacCfgValReg0;
    U_DAC1_REG_0_VALUE DacCfgValReg1;
    U_DAC2_REG_0_VALUE DacCfgValReg2;
    U_DAC3_REG_0_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32;
            DacCfgValReg0.bits.sunmutel = bSoftUnMuteEn;
            DacCfgValReg0.bits.sunmuter = bSoftUnMuteEn;
            g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32;
            DacCfgValReg1.bits.sunmutel = bSoftUnMuteEn;
            DacCfgValReg1.bits.sunmuter = bSoftUnMuteEn;
            g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32;
            DacCfgValReg2.bits.sunmutel = bSoftUnMuteEn;
            DacCfgValReg2.bits.sunmuter = bSoftUnMuteEn;
            g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32;
            DacCfgValReg3.bits.sunmutel = bSoftUnMuteEn;
            DacCfgValReg3.bits.sunmuter = bSoftUnMuteEn;
            g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSetI2SSel(S5_TIANLAI_DAC_NUM_E enDACNum, HI_U32 u32I2SSel_L, HI_U32 u32I2SSel_R)
{
    U_DAC0_REG_0_VALUE DacCfgValReg0;
    U_DAC1_REG_0_VALUE DacCfgValReg1;
    U_DAC2_REG_0_VALUE DacCfgValReg2;
    U_DAC3_REG_0_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32;
            DacCfgValReg0.bits.dacl_i2ssel = u32I2SSel_L;
            DacCfgValReg0.bits.dacr_i2ssel = u32I2SSel_R;
            g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32;
            DacCfgValReg1.bits.dacl_i2ssel = u32I2SSel_L;
            DacCfgValReg1.bits.dacr_i2ssel = u32I2SSel_R;
            g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32;
            DacCfgValReg2.bits.dacl_i2ssel = u32I2SSel_L;
            DacCfgValReg2.bits.dacr_i2ssel = u32I2SSel_R;
            g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32;
            DacCfgValReg3.bits.dacl_i2ssel = u32I2SSel_L;
            DacCfgValReg3.bits.dacr_i2ssel = u32I2SSel_R;
            g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSeLRSel(S5_TIANLAI_DAC_NUM_E enDACNum, HI_BOOL bChnSelL, HI_U32 bChnSelR)
{
    U_DAC0_REG_0_VALUE DacCfgValReg0;
    U_DAC1_REG_0_VALUE DacCfgValReg1;
    U_DAC2_REG_0_VALUE DacCfgValReg2;
    U_DAC3_REG_0_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32;
            DacCfgValReg0.bits.dacl_lrsel = bChnSelL;
            DacCfgValReg0.bits.dacr_lrsel = bChnSelR;
            g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32;
            DacCfgValReg1.bits.dacl_lrsel = bChnSelL;
            DacCfgValReg1.bits.dacr_lrsel = bChnSelR;
            g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32;
            DacCfgValReg2.bits.dacl_lrsel = bChnSelL;
            DacCfgValReg2.bits.dacr_lrsel = bChnSelR;
            g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32;
            DacCfgValReg3.bits.dacl_lrsel = bChnSelL;
            DacCfgValReg3.bits.dacr_lrsel = bChnSelR;
            g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

HI_U32 HAL_TIANLAI_DacSetMute(S5_TIANLAI_DAC_NUM_E enDACNum, HI_BOOL bMuteL, HI_BOOL bMuteR)
{
    U_DAC0_REG_1_VALUE DacCfgValReg0;
    U_DAC1_REG_1_VALUE DacCfgValReg1;
    U_DAC2_REG_1_VALUE DacCfgValReg2;
    U_DAC3_REG_1_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_REG_1_VALUE.u32;
            DacCfgValReg0.bits.dacl_mute = bMuteL;
            DacCfgValReg0.bits.dacr_mute = bMuteR;
            g_pstRegTianlaiBase->DAC0_REG_1_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_REG_1_VALUE.u32;
            DacCfgValReg1.bits.dacl_mute = bMuteL;
            DacCfgValReg1.bits.dacr_mute = bMuteR;
            g_pstRegTianlaiBase->DAC1_REG_1_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_REG_1_VALUE.u32;
            DacCfgValReg2.bits.dacl_mute = bMuteL;
            DacCfgValReg2.bits.dacr_mute = bMuteR;
            g_pstRegTianlaiBase->DAC2_REG_1_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_REG_1_VALUE.u32;
            DacCfgValReg3.bits.dacl_mute = bMuteL;
            DacCfgValReg3.bits.dacr_mute = bMuteR;
            g_pstRegTianlaiBase->DAC3_REG_1_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSetDeepFs(S5_TIANLAI_DAC_NUM_E enDACNum, S5_TIANLAI_DAC_DEEPFS_E enDeepFs)
{
    U_DAC0_REG_0_VALUE DacCfgValReg00;
    U_DAC1_REG_0_VALUE DacCfgValReg01;
    U_DAC2_REG_0_VALUE DacCfgValReg02;
    U_DAC3_REG_0_VALUE DacCfgValReg03;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg00.u32 = g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32;
            DacCfgValReg00.bits.dacl_deemphasis_fs = enDeepFs;
            DacCfgValReg00.bits.dacr_deemphasis_fs = enDeepFs;
            g_pstRegTianlaiBase->DAC0_REG_0_VALUE.u32= DacCfgValReg00.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg01.u32 = g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32;
            DacCfgValReg01.bits.dacl_deemphasis_fs = enDeepFs;
            DacCfgValReg01.bits.dacr_deemphasis_fs = enDeepFs;
            g_pstRegTianlaiBase->DAC1_REG_0_VALUE.u32= DacCfgValReg01.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg02.u32 = g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32;
            DacCfgValReg02.bits.dacl_deemphasis_fs = enDeepFs;
            DacCfgValReg02.bits.dacr_deemphasis_fs = enDeepFs;
            g_pstRegTianlaiBase->DAC2_REG_0_VALUE.u32= DacCfgValReg02.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg03.u32 = g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32;
            DacCfgValReg03.bits.dacl_deemphasis_fs = enDeepFs;
            DacCfgValReg03.bits.dacr_deemphasis_fs = enDeepFs;
            g_pstRegTianlaiBase->DAC3_REG_0_VALUE.u32= DacCfgValReg03.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_DacSetVolume(S5_TIANLAI_DAC_NUM_E enDACNum, HI_U32 u32LeftVol, HI_U32 u32RightVol)
{
    U_DAC0_REG_1_VALUE DacCfgValReg0;
    U_DAC1_REG_1_VALUE DacCfgValReg1;
    U_DAC2_REG_1_VALUE DacCfgValReg2;
    U_DAC3_REG_1_VALUE DacCfgValReg3;

    switch (enDACNum)
    {
        case S5_TIANLAI_DAC_NUM_0:
            DacCfgValReg0.u32 = g_pstRegTianlaiBase->DAC0_REG_1_VALUE.u32;
            DacCfgValReg0.bits.dacl_vol = u32LeftVol;
            DacCfgValReg0.bits.dacr_vol = u32RightVol;
            g_pstRegTianlaiBase->DAC0_REG_1_VALUE.u32= DacCfgValReg0.u32;
            break;
        case S5_TIANLAI_DAC_NUM_1:
            DacCfgValReg1.u32 = g_pstRegTianlaiBase->DAC1_REG_1_VALUE.u32;
            DacCfgValReg1.bits.dacl_vol = u32LeftVol;
            DacCfgValReg1.bits.dacr_vol = u32RightVol;
            g_pstRegTianlaiBase->DAC1_REG_1_VALUE.u32= DacCfgValReg1.u32;
            break;
        case S5_TIANLAI_DAC_NUM_2:
            DacCfgValReg2.u32 = g_pstRegTianlaiBase->DAC2_REG_1_VALUE.u32;
            DacCfgValReg2.bits.dacl_vol = u32LeftVol;
            DacCfgValReg2.bits.dacr_vol = u32RightVol;
            g_pstRegTianlaiBase->DAC2_REG_1_VALUE.u32= DacCfgValReg2.u32;
            break;
        case S5_TIANLAI_DAC_NUM_3:
            DacCfgValReg3.u32 = g_pstRegTianlaiBase->DAC3_REG_1_VALUE.u32;
            DacCfgValReg3.bits.dacl_vol = u32LeftVol;
            DacCfgValReg3.bits.dacr_vol = u32RightVol;
            g_pstRegTianlaiBase->DAC3_REG_1_VALUE.u32= DacCfgValReg3.u32;
            break;
        default:
            break;
        }

    return HI_SUCCESS;
}

static HI_U32 HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_E enPgaGain, HI_BOOL bBoost)
{
    U_ANA_REG_0 unAnaReg0;

    unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
    unAnaReg0.bits.rpga_gain = enPgaGain;
    unAnaReg0.bits.lpga_gain = enPgaGain;
    unAnaReg0.bits.lpga_boost = bBoost;
    unAnaReg0.bits.rpga_boost = bBoost;
    g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;

    return HI_SUCCESS;
}


//设置模拟寄存器的32位值到相应的四个寄存器中
HI_U32 HAL_TIANLAI_V510_SetAanReg(S5_TIANLAI_ANAREG_NUM_E enAnaRegNum, HI_U32 u32RegVal)
{
    switch (enAnaRegNum)
    {
        case S5_TIANLAI_ANAREG_NUM_0:
            g_pstRegTianlaiBase->ANA_REG_0.u32= u32RegVal;
            break;
        case S5_TIANLAI_ANAREG_NUM_1:
            g_pstRegTianlaiBase->ANA_REG_1.u32= u32RegVal;
            break;
        case S5_TIANLAI_ANAREG_NUM_2:
            g_pstRegTianlaiBase->ANA_REG_2.u32= u32RegVal;
            break;
        case S5_TIANLAI_ANAREG_NUM_3:
            g_pstRegTianlaiBase->ANA_REG_3.u32= u32RegVal;
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

//on:work; 0:off
static HI_U32 HAL_TIANLAI_V510_SetAdcOnOff(HI_BOOL bOnOff)
{

    /*工作*/
    if (HI_TRUE == bOnOff)
    {
        /* step 4: 将相应通道的PD_DAC置0*/
        U_ANA_REG_0 unAnaReg0;
        unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
        unAnaReg0.bits.adcr_pd  = 0;
        unAnaReg0.bits.adcl_pd  = 0;
        g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;

        HAL_TIANLAI_AdcSetEn(HI_TRUE);
    }
    else/*不工作*/
    {
        /* step 4: 将相应通道的PD_DAC置0*/
        U_ANA_REG_0 unAnaReg0;
        unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
        unAnaReg0.bits.adcr_pd  = 0;
        unAnaReg0.bits.adcl_pd  = 0;
        g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;

        HAL_TIANLAI_AdcSetEn(HI_FALSE);
    }

    return HI_SUCCESS;
}


//AVout为S5_TIANLAI_LINEOUT_1
HI_U32 HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_E enLineOut, S5_TIANLAI_LINOUT_LINEIN_E enLineIn);
HI_U32 HAL_TIANLAI_V510_SetLineInSuorce(S5_TIANLAI_LINEIN_SEL_E enLineInSel, HI_BOOL bByPass, HI_BOOL bAdcWork)
{
    if (bByPass == HI_TRUE)
    {
        //将AVout的输入变为bypass的目标源，此时需要知道AVout的定位为哪个输入，工厂菜单的定义在此体现。
        switch (enLineInSel)
        {
            case S5_TIANLAI_LINEIN_SEL_MIC:
                HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_2, S5_TIANLAI_LINEIN_SEL_MIC);
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_0dB, HI_TRUE);
                break;
            case S5_TIANLAI_LINEIN_SEL_L1:
                HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_2, S5_TIANLAI_LINEIN_SEL_L1);
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_0dB, HI_FALSE);
                break;
            case S5_TIANLAI_LINEIN_SEL_L2:
                HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_2, S5_TIANLAI_LINEIN_SEL_L2);
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_0dB, HI_FALSE);
                break;
            case S5_TIANLAI_LINEIN_SEL_L3:
                HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_2, S5_TIANLAI_LINEIN_SEL_L3);
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_0dB, HI_FALSE);
                break;
            case S5_TIANLAI_LINEIN_SEL_L4:
                HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_2, S5_TIANLAI_LINEIN_SEL_L4);
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_0dB, HI_FALSE);
                break;
             default:
                break;
        }

        //将AVout输入的DAC输入mute掉
        {
            U_ANA_REG_1 unAnaReg1;
            unAnaReg1.u32 = g_pstRegTianlaiBase->ANA_REG_1.u32;
            unAnaReg1.bits.dacr2_mute = 1;
            unAnaReg1.bits.dacl2_mute = 1;
            g_pstRegTianlaiBase->ANA_REG_1.u32= unAnaReg1.u32;
        }
    }
    else
    {
        HAL_TIANLAI_V510_SetAdcOnOff(bAdcWork);/*set adc on or off*/

        if (!bAdcWork)
        {
            return HI_SUCCESS;/*not work, return*/
        }
        else
        {
            U_ANA_REG_0 unAnaReg0;
            unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
            unAnaReg0.bits.rpga_sel = enLineInSel;
            unAnaReg0.bits.lpga_sel = enLineInSel;
            g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;
        }

        // adjust the input gain
        switch (enLineInSel)
        {
            case S5_TIANLAI_LINEIN_SEL_MIC:
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_0dB, HI_TRUE);
                break;
            case S5_TIANLAI_LINEIN_SEL_L1:
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_6dB, HI_FALSE);
                break;
            case S5_TIANLAI_LINEIN_SEL_L2:
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_6dB, HI_FALSE);
                break;
            case S5_TIANLAI_LINEIN_SEL_L3:
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_6dB, HI_FALSE);
                break;
            case S5_TIANLAI_LINEIN_SEL_L4:
                HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_6dB, HI_FALSE);
                break;
             default:
                break;
        }

        HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_2, S5_TIANLAI_LINOUT_LINEIN_DAC);

        //将AVout输入的DAC输入unmute掉
        #if 1
        {
            U_ANA_REG_1 unAnaReg1;
            unAnaReg1.u32 = g_pstRegTianlaiBase->ANA_REG_1.u32;
            unAnaReg1.bits.dacr2_mute = 0;
            unAnaReg1.bits.dacl2_mute = 0;
            g_pstRegTianlaiBase->ANA_REG_1.u32= unAnaReg1.u32;
        }
        #endif
    }

    return HI_SUCCESS;
}

/*bypass功能*/
HI_U32 HAL_TIANLAI_V510_SetLineOutSel(S5_TIANLAI_LINEOUT_E enLineOut, S5_TIANLAI_LINOUT_LINEIN_E enLineIn)
{
    U_ANA_REG_2 unAnaReg2;
    U_ANA_REG_3 unAnaReg3;

    switch (enLineOut)
    {
        case S5_TIANLAI_LINEOUT_1:
            unAnaReg2.u32 = g_pstRegTianlaiBase->ANA_REG_2.u32;
            unAnaReg2.bits.linein_avout1 = enLineIn;
            g_pstRegTianlaiBase->ANA_REG_2.u32= unAnaReg2.u32;
            break;
        case S5_TIANLAI_LINEOUT_2:
            unAnaReg2.u32 = g_pstRegTianlaiBase->ANA_REG_2.u32;
            unAnaReg2.bits.linein_avout2 = enLineIn;
            g_pstRegTianlaiBase->ANA_REG_2.u32= unAnaReg2.u32;
            break;
        case S5_TIANLAI_LINEOUT_3:
            unAnaReg3.u32 = g_pstRegTianlaiBase->ANA_REG_3.u32;
            unAnaReg3.bits.linein_sp = enLineIn;
            g_pstRegTianlaiBase->ANA_REG_3.u32= unAnaReg3.u32;
            break;
        case S5_TIANLAI_LINEOUT_4:
            unAnaReg3.u32 = g_pstRegTianlaiBase->ANA_REG_3.u32;
            unAnaReg3.bits.linein_hp = enLineIn;
            g_pstRegTianlaiBase->ANA_REG_3.u32= unAnaReg3.u32;
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

#if defined (CHIP_TYPE_hi3751v620)
HI_U32 HAL_TIANLAI_V510_SetAdacSrcSel(HI_VOID)
{
    U_ANA_ADAC_SRC_SEL AdacSrcSelValReg;
    AdacSrcSelValReg.u32 = g_pstRegTianlaiBase->ANA_ADAC_SRC_SEL.u32;
    AdacSrcSelValReg.bits.adac4_source_sel = 0x0;    //ADAC4->DAC0, for Analog AMP
    AdacSrcSelValReg.bits.adac3_source_sel = 0x2;     //ADAC3->DAC2, for AVOUT/SCART Audio Out
    AdacSrcSelValReg.bits.adac2_source_sel = 0x1;    //ADAC2->DAC1, for Headphone
    g_pstRegTianlaiBase->ANA_ADAC_SRC_SEL.u32= AdacSrcSelValReg.u32;

    return HI_SUCCESS;
}
#endif

/*上电流程使用，S5回片后代码*/
/*上下电的流程不必在FPGA阶段就已经确定，S5回片后，此设置会减少pop音。FPGA中不涉及。直接设置，在方案设计中增加。*/
static HI_U32 HAL_TIANLAI_Poweup(HI_VOID)
{
    U_ANA_REG_0 unAnaReg0;
    U_ANA_REG_1 unAnaReg1;
    U_ANA_REG_2 unAnaReg2;

    /* open soft mute,4 * DAC ，四个输出端设置为soft mute使能*/
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_0, HI_TRUE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_1, HI_TRUE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_2, HI_TRUE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_3, HI_TRUE);

#if 0
    /* step1: 先将dac的模拟mute控制设为1*/
    unAnaReg1.u32 = g_pstRegTianlaiBase->ANA_REG_1.u32;
    unAnaReg1.bits.dacr4_mute = 1;
    unAnaReg1.bits.dacr3_mute = 1;
    unAnaReg1.bits.dacr2_mute = 1;
    unAnaReg1.bits.dacr1_mute = 1;
    unAnaReg1.bits.dacl4_mute = 1;
    unAnaReg1.bits.dacl3_mute = 1;
    unAnaReg1.bits.dacl2_mute = 1;
    unAnaReg1.bits.dacl1_mute = 1;
    g_pstRegTianlaiBase->ANA_REG_1.u32= unAnaReg1.u32;
#endif
    /* step 3: 将popfree控制位置1*/
    unAnaReg2.u32 = g_pstRegTianlaiBase->ANA_REG_2.u32;
    unAnaReg2.bits.popfree_l1 = 1;
    unAnaReg2.bits.popfree_l2 = 1;
    unAnaReg2.bits.popfree_l3 = 1;
    unAnaReg2.bits.popfree_l4 = 1;
    unAnaReg2.bits.popfree_r1 = 1;
    unAnaReg2.bits.popfree_r2 = 1;
    unAnaReg2.bits.popfree_r3 = 1;
    unAnaReg2.bits.popfree_r4 = 1;
    g_pstRegTianlaiBase->ANA_REG_2.u32= unAnaReg2.u32;

    /*设置fs为正常上电模式*/
    unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
    unAnaReg0.bits.vref_fs = 0;
    g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;

    /* step 2: 将pd_vref置0 */
    unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
    unAnaReg0.bits.vref_pd = 0;
    g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;

    /* step 4: 将相应通道的PD_DAC置0*/
    unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
    unAnaReg0.bits.dacl1_pd = 0;
    unAnaReg0.bits.dacl2_pd = 0;
    unAnaReg0.bits.dacl3_pd = 0;
    unAnaReg0.bits.dacl4_pd = 0;
    unAnaReg0.bits.dacr1_pd = 0;
    unAnaReg0.bits.dacr2_pd = 0;
    unAnaReg0.bits.dacr3_pd = 0;
    unAnaReg0.bits.dacr4_pd = 0;
    unAnaReg0.bits.ctcm_pd  = 0;
    unAnaReg0.bits.ibias_pd = 0;
    unAnaReg0.bits.rpga_pd  = 0;
    unAnaReg0.bits.lpga_pd  = 0;
    unAnaReg0.bits.adcr_pd  = 1;
    unAnaReg0.bits.adcl_pd  = 1;
    g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;

    /* step 5: 将popfree控制置为0*/
    unAnaReg2.u32 = g_pstRegTianlaiBase->ANA_REG_2.u32;
    unAnaReg2.bits.popfree_l1 = 0;
    unAnaReg2.bits.popfree_l2 = 0;
    unAnaReg2.bits.popfree_l3 = 0;
    unAnaReg2.bits.popfree_l4 = 0;
    unAnaReg2.bits.popfree_r1 = 0;
    unAnaReg2.bits.popfree_r2 = 0;
    unAnaReg2.bits.popfree_r3 = 0;
    unAnaReg2.bits.popfree_r4 = 0;
    g_pstRegTianlaiBase->ANA_REG_2.u32= unAnaReg2.u32;

    /* step6: 先将dac的模拟mute控制设为0*/
    unAnaReg1.u32 = g_pstRegTianlaiBase->ANA_REG_1.u32;
    unAnaReg1.bits.dacr4_mute = 0;
    unAnaReg1.bits.dacr3_mute = 0;
    unAnaReg1.bits.dacr2_mute = 0;
    unAnaReg1.bits.dacr1_mute = 0;
    unAnaReg1.bits.dacl4_mute = 0;
    unAnaReg1.bits.dacl3_mute = 0;
    unAnaReg1.bits.dacl2_mute = 0;
    unAnaReg1.bits.dacl1_mute = 0;
    g_pstRegTianlaiBase->ANA_REG_1.u32= unAnaReg1.u32;

    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_0, HI_FALSE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_1, HI_FALSE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_2, HI_FALSE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_3, HI_FALSE);

    HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_0, HI_TRUE);
    HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_1, HI_TRUE);
    HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_2, HI_TRUE);
    HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_3, HI_TRUE);

    return HI_SUCCESS;
}

/*下电流程使用，S5回片后代码*/
static HI_U32 HAL_TIANLAI_Powedown(HI_VOID)
{
#if 1
    U_ANA_REG_0 unAnaReg0;
    U_ANA_REG_1 unAnaReg1;
    U_ANA_REG_2 unAnaReg2;

    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_0, HI_TRUE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_1, HI_TRUE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_2, HI_TRUE);
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_3, HI_TRUE);

    //msleep(10);

    /* step1: 先将dac的模拟mute控制设为1*/
    unAnaReg1.u32 = g_pstRegTianlaiBase->ANA_REG_1.u32;
    unAnaReg1.bits.dacr4_mute = 1;
    unAnaReg1.bits.dacr3_mute = 1;
    unAnaReg1.bits.dacr2_mute = 1;
    unAnaReg1.bits.dacr1_mute = 1;
    unAnaReg1.bits.dacl4_mute = 1;
    unAnaReg1.bits.dacl3_mute = 1;
    unAnaReg1.bits.dacl2_mute = 1;
    unAnaReg1.bits.dacl1_mute = 1;
    g_pstRegTianlaiBase->ANA_REG_1.u32= unAnaReg1.u32;

    /* step 2: 将popfree控制位置1*/
    unAnaReg2.u32 = g_pstRegTianlaiBase->ANA_REG_2.u32;
    unAnaReg2.bits.popfree_l1 = 1;
    unAnaReg2.bits.popfree_l2 = 1;
    unAnaReg2.bits.popfree_l3 = 1;
    unAnaReg2.bits.popfree_l4 = 1;
    unAnaReg2.bits.popfree_r1 = 1;
    unAnaReg2.bits.popfree_r2 = 1;
    unAnaReg2.bits.popfree_r3 = 1;
    unAnaReg2.bits.popfree_r4 = 1;
    g_pstRegTianlaiBase->ANA_REG_2.u32= unAnaReg2.u32;

    /* step 3: 将相应通道的PD_DAC置0*/
    unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
    unAnaReg0.bits.dacl1_pd = 1;
    unAnaReg0.bits.dacl2_pd = 1;
    unAnaReg0.bits.dacl3_pd = 1;
    unAnaReg0.bits.dacl4_pd = 1;
    unAnaReg0.bits.dacr1_pd = 1;
    unAnaReg0.bits.dacr2_pd = 1;
    unAnaReg0.bits.dacr3_pd = 1;
    unAnaReg0.bits.dacr4_pd = 1;
    g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;

#if 0
    /* step4: 将mute控制设为0*/
    unAnaReg1.u32 = g_pstRegTianlaiBase->ANA_REG_1.u32;
    unAnaReg1.bits.dacr4_mute = 0;
    unAnaReg1.bits.dacr3_mute = 0;
    unAnaReg1.bits.dacr2_mute = 0;
    unAnaReg1.bits.dacr1_mute = 0;
    unAnaReg1.bits.dacl4_mute = 0;
    unAnaReg1.bits.dacl3_mute = 0;
    unAnaReg1.bits.dacl2_mute = 0;
    unAnaReg1.bits.dacl2_mute = 0;
    g_pstRegTianlaiBase->ANA_REG_1.u32= unAnaReg1.u32;
#endif

    /* step 5: 将pd_vref置0 */
    unAnaReg0.u32 = g_pstRegTianlaiBase->ANA_REG_0.u32;
    unAnaReg0.bits.vref_pd = 1;
    g_pstRegTianlaiBase->ANA_REG_0.u32= unAnaReg0.u32;
    #endif

    return HI_SUCCESS;
}

#if defined (CHIP_TYPE_hi3751v600) || defined (CHIP_TYPE_hi3751lv500) || defined (CHIP_TYPE_hi3751v500) || defined (CHIP_TYPE_hi3751v620)
static HI_VOID HAL_AudioCodec_Reset(HI_VOID)
{
#if defined (CHIP_TYPE_hi3751v500) || defined (CHIP_TYPE_hi3751v620)
    U_PERI_CRG163 AudioCodecCfg;

    AudioCodecCfg.u32 = g_pstRegCrg->PERI_CRG163.u32;
    AudioCodecCfg.bits.audio_codec_rst_n = 0;  /* Aduio Codec reset */
    g_pstRegCrg->PERI_CRG163.u32 = AudioCodecCfg.u32;

#else
    U_PERI_CRG_163 AudioCodecCfg;

    AudioCodecCfg.u32 = g_pstRegCrg->PERI_CRG_163.u32;
    AudioCodecCfg.bits.audio_codec_rst_n = 0;  /* Aduio Codec reset */
    g_pstRegCrg->PERI_CRG_163.u32 = AudioCodecCfg.u32;

#endif
}

static HI_VOID HAL_AudioCodec_UndoReset(HI_VOID)
{
#if defined (CHIP_TYPE_hi3751v500) || defined (CHIP_TYPE_hi3751v620)
        U_PERI_CRG163 AudioCodecCfg;

    AudioCodecCfg.u32 = g_pstRegCrg->PERI_CRG163.u32;
    AudioCodecCfg.bits.audio_codec_rst_n = 0;  /* Aduio Codec reset */
    g_pstRegCrg->PERI_CRG163.u32 = AudioCodecCfg.u32;

    AudioCodecCfg.u32 = g_pstRegCrg->PERI_CRG163.u32;
    AudioCodecCfg.bits.audio_codec_rst_n = 1;  /* Aduio Codec undo reset */
    g_pstRegCrg->PERI_CRG163.u32 = AudioCodecCfg.u32;

#else
    U_PERI_CRG_163 AudioCodecCfg;

    AudioCodecCfg.u32 = g_pstRegCrg->PERI_CRG_163.u32;
    AudioCodecCfg.bits.audio_codec_rst_n = 0;  /* Aduio Codec reset */
    g_pstRegCrg->PERI_CRG_163.u32 = AudioCodecCfg.u32;

    AudioCodecCfg.u32 = g_pstRegCrg->PERI_CRG_163.u32;
    AudioCodecCfg.bits.audio_codec_rst_n = 1;  /* Aduio Codec undo reset */
    g_pstRegCrg->PERI_CRG_163.u32 = AudioCodecCfg.u32;

#endif
}
#endif

/*需要等待AIAO时钟复位/解复位的操作后调用，否则配置的寄存器的值将不生效。*/
HI_U32 HAL_TIANLAI_V510_Init(S5_TIANLAI_SAMPLERATE_E enSampleRate)
{

    HAL_TIANLAI_InitBaseAddr();

#if defined (CHIP_TYPE_hi3751v600) || defined (CHIP_TYPE_hi3751lv500) || defined (CHIP_TYPE_hi3751v500) || defined (CHIP_TYPE_hi3751v620)
    HAL_AudioCodec_UndoReset();
#endif

    HAL_TIANLAI_AdcSetReset(HI_TRUE);
    HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_0, HI_TRUE);
    HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_1, HI_TRUE);
    HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_2, HI_TRUE);
    HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_3, HI_TRUE);

    msleep(1);

    /*set power up*/
    HAL_TIANLAI_Poweup();

    HAL_TIANLAI_DacSetEn(S5_TIANLAI_DAC_NUM_0, HI_TRUE);
    HAL_TIANLAI_DacSetEn(S5_TIANLAI_DAC_NUM_1, HI_TRUE);
    HAL_TIANLAI_DacSetEn(S5_TIANLAI_DAC_NUM_2, HI_TRUE);
    HAL_TIANLAI_DacSetEn(S5_TIANLAI_DAC_NUM_3, HI_TRUE);

    /******************0xff280004: 0x00F35A00 --> 0xFF035A00******************/
    HAL_TIANLAI_AdcSetBitWidth(S5_TIANLAI_BIT_DEPTH_16);

    /******************0xff280008: 0x1E1E0001 --> 0x1E1EC001******************/
    HAL_TIANLAI_AdcSetHpfEn(HI_TRUE, HI_TRUE);

    /******************0xff28001c: 0x00F35A00 --> 0xFF035A00******************/
    HAL_TIANLAI_DacSetBitWidth(S5_TIANLAI_DAC_NUM_0, S5_TIANLAI_BIT_DEPTH_16);

    /******************0xff280020: 0xC0000001 --> 0x08780001******************/
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_0, HI_FALSE);
    HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_0, HI_FALSE);
    HAL_TIANLAI_DacSetI2SSel(S5_TIANLAI_DAC_NUM_0, 0, 0);
    HAL_TIANLAI_DacSeLRSel(S5_TIANLAI_DAC_NUM_0, HI_FALSE, HI_TRUE);
    HAL_TIANLAI_DacSetDeepFs(S5_TIANLAI_DAC_NUM_0, S5_TIANLAI_DAC_DEEPFS_NULL);
    HAL_TIANLAI_DacVolUpdate(S5_TIANLAI_DAC_NUM_0, HI_TRUE);

    /******************0xff28001c: 0x00F35A00 --> 0xFF035A00******************/
    HAL_TIANLAI_DacSetBitWidth(S5_TIANLAI_DAC_NUM_1, S5_TIANLAI_BIT_DEPTH_16);

    /******************0xff280020: 0xC0000001 --> 0x08780001******************/
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_1, HI_FALSE);
    HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_1, HI_FALSE);
    HAL_TIANLAI_DacSetI2SSel(S5_TIANLAI_DAC_NUM_1, 0, 0);
    HAL_TIANLAI_DacSeLRSel(S5_TIANLAI_DAC_NUM_1, HI_FALSE, HI_TRUE);
    HAL_TIANLAI_DacSetDeepFs(S5_TIANLAI_DAC_NUM_1, S5_TIANLAI_DAC_DEEPFS_NULL);
    HAL_TIANLAI_DacVolUpdate(S5_TIANLAI_DAC_NUM_1, HI_TRUE);

    /******************0xff28001c: 0x00F35A00 --> 0xFF035A00******************/
    HAL_TIANLAI_DacSetBitWidth(S5_TIANLAI_DAC_NUM_2, S5_TIANLAI_BIT_DEPTH_16);

    /******************0xff280020: 0xC0000001 --> 0x08780001******************/
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_2, HI_FALSE);
    HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_2, HI_FALSE);
    HAL_TIANLAI_DacSetI2SSel(S5_TIANLAI_DAC_NUM_2, 0, 0);
    HAL_TIANLAI_DacSeLRSel(S5_TIANLAI_DAC_NUM_2, HI_FALSE, HI_TRUE);
    HAL_TIANLAI_DacSetDeepFs(S5_TIANLAI_DAC_NUM_2, S5_TIANLAI_DAC_DEEPFS_NULL);
    HAL_TIANLAI_DacVolUpdate(S5_TIANLAI_DAC_NUM_2, HI_TRUE);

    /******************0xff28001c: 0x00F35A00 --> 0xFF035A00******************/
    HAL_TIANLAI_DacSetBitWidth(S5_TIANLAI_DAC_NUM_3, S5_TIANLAI_BIT_DEPTH_16);

    /******************0xff280020: 0xC0000001 --> 0x08780001******************/
    HAL_TIANLAI_DacSetSoftMuteEn(S5_TIANLAI_DAC_NUM_3, HI_FALSE);
    HAL_TIANLAI_DacSetSoftUnMuteEn(S5_TIANLAI_DAC_NUM_3, HI_FALSE);
    HAL_TIANLAI_DacSetI2SSel(S5_TIANLAI_DAC_NUM_3, 0, 0);
    HAL_TIANLAI_DacSeLRSel(S5_TIANLAI_DAC_NUM_3, HI_FALSE, HI_TRUE);
    HAL_TIANLAI_DacSetDeepFs(S5_TIANLAI_DAC_NUM_3, S5_TIANLAI_DAC_DEEPFS_NULL);
    HAL_TIANLAI_DacVolUpdate(S5_TIANLAI_DAC_NUM_3, HI_TRUE);

    /*set sample rate*/
    HAL_TIANLAI_AdcSampleRate(enSampleRate);
    HAL_TIANLAI_DacSampleRate(S5_TIANLAI_DAC_NUM_0, enSampleRate);
    HAL_TIANLAI_DacSampleRate(S5_TIANLAI_DAC_NUM_1, enSampleRate);
    HAL_TIANLAI_DacSampleRate(S5_TIANLAI_DAC_NUM_2, enSampleRate);
    HAL_TIANLAI_DacSampleRate(S5_TIANLAI_DAC_NUM_3, enSampleRate);

    /*set volume*/
    HAL_TIANLAI_AdcSetVolume(0x1E, 0x1E);
    HAL_TIANLAI_DacSetVolume(S5_TIANLAI_DAC_NUM_0, 0x6, 0x6);
    HAL_TIANLAI_DacSetVolume(S5_TIANLAI_DAC_NUM_1, 0x6, 0x6);
    HAL_TIANLAI_DacSetVolume(S5_TIANLAI_DAC_NUM_2, 0x6, 0x6);
    HAL_TIANLAI_DacSetVolume(S5_TIANLAI_DAC_NUM_3, 0x6, 0x6);
#if defined (CHIP_TYPE_hi3751v620)
    HAL_TIANLAI_V510_SetAanReg(S5_TIANLAI_ANAREG_NUM_3, 0x15010300);
    HAL_TIANLAI_V510_SetAdacSrcSel();
#endif
//    HAL_TIANLAI_AnaSetInGain(S5_TIANLAI_ANA_PGA_GAIN_12dB);

    return HI_SUCCESS;
}

/*调用此函数后，再进行时钟的停止和下电等动作，避免下电pop音。*/
HI_U32 HAL_TIANLAI_V510_DeInit(HI_VOID)
{
    HAL_TIANLAI_Powedown();

    HAL_TIANLAI_AdcSetReset(HI_TRUE);
    HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_0, HI_TRUE);
    HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_1, HI_TRUE);
    HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_2, HI_TRUE);
    HAL_TIANLAI_DacSetReset(S5_TIANLAI_DAC_NUM_3, HI_TRUE);

#if defined (CHIP_TYPE_hi3751v600) || defined (CHIP_TYPE_hi3751lv500) || defined (CHIP_TYPE_hi3751v500) || defined (CHIP_TYPE_hi3751v620)
    HAL_AudioCodec_Reset();
#endif

    return HI_SUCCESS;
}

