/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name             :   aud_adac.c
  Version               :   Initial Draft
  Author                :   Hisilicon multimedia software group
  Created               :   2010/02/28
  Last Modified         :
  Description           :  Hifi audio dac interface
  Function List         :
  History               :
  1.Date                :   2010/02/28
    Author              :   z40717
    Modification        :   Created file

******************************************************************************/
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/seq_file.h>

#include "hi_drv_mmz.h"
#include "hi_drv_stat.h"
#include "hi_drv_sys.h"
#include "hi_drv_proc.h"
#include "hal_tianlai_adac_v700.h"
#include "hi_drv_ao.h"
#include "hi_reg_common.h"
#include "drv_ao_private.h"

#define DBG_ADAC_DISABLE_TIMER

static HI_VOID Digfi_DacSetVolume(HI_U32 left, HI_U32 right)
{
    U_PERI_TIANLA_ADAC0 Adac0;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_dacr_vol = right;
    Adac0.bits.adac_dig_dacl_vol = left;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
}

HI_VOID Digfi_DacSetDataBits(HI_VOID)
{
    U_PERI_TIANLA_ADAC1 Adac1;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.adac_data_bits = 0x0;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;
}

HI_VOID Digfi_DacSetAdj(HI_VOID)
{
    U_PERI_TIANLA_ADAC1 Adac1;
    U_PERI_TIANLA_ADAC2 Adac2;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.adac_adj_refbf = 0x2;
    Adac1.bits.adac_adj_dac = 0x4;
    Adac1.bits.adac_adj_ctcm = 0x1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_adj_dac_low_bias = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}

HI_VOID Digfi_DacSetPd(HI_VOID)
{
    U_PERI_TIANLA_ADAC2 Adac2;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_vref = 0;
    Adac2.bits.adac_pdb_ctcm_ibias = 1;
    Adac2.bits.adac_pd_ibias = 0;
    Adac2.bits.adac_pd_dacr = 0;
    Adac2.bits.adac_pd_dacl = 0;
    Adac2.bits.adac_pd_ctcm = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}

HI_VOID Digfi_DacSetChopper(HI_BOOL bEnable)
{
    U_PERI_TIANLA_ADAC2 Adac2;

    if (bEnable == HI_TRUE)
    {
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.adac_chop_bps_ctcm = 0;
        Adac2.bits.adac_chop_bps_dacvref = 0;
        Adac2.bits.adac_ctcm_chop_sel = 0;
        Adac2.bits.adac_dac_vref_chop_sel = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    }
    else
    {
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.adac_chop_bps_ctcm = 1;
        Adac2.bits.adac_chop_bps_dacvref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    }
}

HI_VOID Digfi_DacSetPath(HI_VOID)
{
    U_PERI_TIANLA_ADAC0 Adac0;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_dacl_path = 0;
    Adac0.bits.adac_dig_dacr_path = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
}

HI_VOID Digfi_DacSetDeemph(HI_BOOL bEnable, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    U_PERI_TIANLA_ADAC0 Adac0;

    if (bEnable == HI_TRUE)
    {
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.adac_dig_dacl_deemph = 1;
        Adac0.bits.adac_dig_dacr_deemph = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        switch (enSampleRate)
        {
            case HI_UNF_SAMPLE_RATE_48K:
                Adac0.bits.adac_dig_deemphasis_fs = 0;
                break;
            case HI_UNF_SAMPLE_RATE_44K:
                Adac0.bits.adac_dig_deemphasis_fs = 1;
                break;
            case HI_UNF_SAMPLE_RATE_32K:
                Adac0.bits.adac_dig_deemphasis_fs = 2;
                break;
            default:
                Adac0.bits.adac_dig_deemphasis_fs = 0;
                break;
        }
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    }
    else
    {
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.adac_dig_dacl_deemph = 0;
        Adac0.bits.adac_dig_dacr_deemph = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    }
}

HI_VOID Digfi_DacSetSampleRate(HI_UNF_SAMPLE_RATE_E SR)
{
    U_PERI_TIANLA_ADAC1 Adac1;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    switch (SR)
    {
        case HI_UNF_SAMPLE_RATE_176K:
        case HI_UNF_SAMPLE_RATE_192K:
            Adac1.bits.adac_clk_sel_sam = 4;
            break;

        case HI_UNF_SAMPLE_RATE_88K:
        case HI_UNF_SAMPLE_RATE_96K:
            Adac1.bits.adac_clk_sel_sam = 3;
            break;

        case HI_UNF_SAMPLE_RATE_32K:
        case HI_UNF_SAMPLE_RATE_44K:
        case HI_UNF_SAMPLE_RATE_48K:
            Adac1.bits.adac_clk_sel_sam = 2;
            break;

        case HI_UNF_SAMPLE_RATE_16K:
        case HI_UNF_SAMPLE_RATE_22K:
        case HI_UNF_SAMPLE_RATE_24K:
            Adac1.bits.adac_clk_sel_sam = 1;
            break;

        case HI_UNF_SAMPLE_RATE_8K:
        case HI_UNF_SAMPLE_RATE_11K:
        case HI_UNF_SAMPLE_RATE_12K:
            Adac1.bits.adac_clk_sel_sam = 0;
            break;

        default:
            Adac1.bits.adac_clk_sel_sam = 2;
            break;
    }
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;

    return;
}

#if 1
static HI_VOID Digfi_DacPoweup(HI_BOOL bResume)
{
    U_PERI_TIANLA_ADAC2 Adac2;
    U_PERI_TIANLA_ADAC3 Adac3;

    /* step 1: msel set 0 */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_msel = 0x0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    /* step 2: adac_pop_pullout_en set 1*/
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pop_pullout_en = 0;
    Adac2.bits.adac_pop_pu_pullb = 1;
#ifdef HI_SND_MUTECTL_SUPPORT
    // fast power up enable
    Adac2.bits.adac_fs = 0;
#else
    //fast power up not enable
    Adac2.bits.adac_fs = 0;
#endif
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 3.4: pd setting 1 */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_ctcm = 0;
    Adac2.bits.adac_pdb_ctcm_ibias = 1;
    Adac2.bits.adac_pd_ibias = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 5: rst_pop setting 0 */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_rst_pop = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    /* step 6: open cp adac_cp_en_soft adac_cp_pd setting 0 */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_cp_pd = 1;

    /*step 8:adac_cp_en_soft set 0*/
    Adac3.bits.adac_cp_en_soft = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    /* step 10: adac_pd_dacr/l set 0 */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_dacr = 0;
    Adac2.bits.adac_pd_dacl = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 11: adac_mute_lineout set 0 */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_mute_lineout = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 12: pop setting */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_pop_clk_sel = 0x1;
    Adac3.bits.adac_pop_dis = 0x1;
    Adac3.bits.adac_pop_pdm_delay_cfg = 0x0;
    Adac3.bits.adac_pop_spd_cfg = 0x1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    /* step 13: adac_pd_vref set 0 */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_vref = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 16: pd_lineout*/
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_pd_lineout = 0x0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_td_sel = 0x10;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 18: adac_pop_pullout_en set enable */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pop_pullout_en = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

#ifndef HI_SND_MUTECTL_SUPPORT
    msleep(500);
#endif

#ifdef HI_SND_MUTECTL_SUPPORT
    if (HI_TRUE == bResume)
    {
        msleep(10);
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.adac_fs = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    }
#endif

    return;
}
#else
static HI_VOID Digfi_Dac_Poweup(HI_BOOL bResume) //2VRM
{
    U_PERI_TIANLA_ADAC0 Adac0;
    U_PERI_TIANLA_ADAC2 Adac2;
    U_PERI_TIANLA_ADAC3 Adac3;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pop_pu_pullb = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_dacr = 0;
    Adac2.bits.adac_pd_dacl = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_chop_bps_lineout = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    /* step 1: msel 0 */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_msel = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_pop_dis = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_pd_dacl = 0;
    Adac0.bits.adac_dig_pd_dacr = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    /* step 2: pop pull en */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pop_pullout_en = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 3: pd ibias */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_ibias = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 4: pd ctcm */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_ctcm = 0;
    Adac2.bits.adac_pdb_ctcm_ibias = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 5: rst pop */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_rst_pop = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_cp_buffer_pd = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    /* step 6: rst pop */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_cp_en_soft = 1;
    Adac3.bits.adac_cp_pd = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;
    msleep(200);

    /* step 7: rst pop */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_cp_en_soft = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;
    msleep(200);

    /*step 8: open DAC */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_dacr = 0;
    Adac2.bits.adac_pd_dacl = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 10: pd verf */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_vref = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    msleep(500);

    /* step 11: pop pullout en */
    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_pd_lineout = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;
    msleep(200);

    /* step 12: pop pullout en */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
	Adac2.bits.adac_adj_dac_low_bias = 1;
    Adac2.bits.adac_pop_pullout_en = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pdb_ctcm_ibias = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_mute_lineout = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_mute_dacl = 0;
    Adac0.bits.adac_dig_mute_dacr = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_pop_dis = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

#ifdef HI_SND_SUPPORT_INTERNAL_OP_MAXGAIN
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_gain = 0x4;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
#endif
}
#endif

#ifdef HI_SND_MUTECTL_SUPPORT
HI_VOID ADAC_FastPowerEnable(HI_BOOL bEnable)
{
    U_PERI_TIANLA_ADAC2 Adac2;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_fs = (HI_U32)bEnable;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}
#endif

#if 1
static HI_VOID Digfi_DacPowedown(HI_BOOL bSuspend)
{
    U_PERI_TIANLA_ADAC2 Adac2;
    HI_U32 u32Volume;

    for (u32Volume = 0x06; u32Volume <= 0x7f; u32Volume += 10)
    {
        Digfi_DacSetVolume(u32Volume, u32Volume);
        msleep(1);
    }

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_mute_lineout = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pop_pullout_en = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_dacl = 0;
    Adac2.bits.adac_pd_dacr = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

#ifndef HI_SND_MUTECTL_SUPPORT
    msleep(2000);
    if (HI_TRUE == bSuspend)
    {
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.adac_fs = 0;
        Adac2.bits.adac_pd_vref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

        msleep(2 * 1000);
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.adac_fs = 1;
        Adac2.bits.adac_popfreel = 0;
        Adac2.bits.adac_popfreer = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    }
    else
#endif
    {
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.adac_pd_vref = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
#ifndef HI_SND_MUTECTL_SUPPORT
        msleep(50);
#endif
    }
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_ctcm = 0;
    Adac2.bits.adac_pdb_ctcm_ibias = 1;
    Adac2.bits.adac_pd_ibias = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}
#else
static HI_VOID Digfi_DacPowedown(HI_BOOL bSuspend)  //2VRM
{
    U_PERI_TIANLA_ADAC0 Adac0;
    U_PERI_TIANLA_ADAC2 Adac2;
    U_PERI_TIANLA_ADAC3 Adac3;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_gain = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_mute_dacl = 1;
    Adac0.bits.adac_dig_mute_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_pop_dis = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pop_pullout_en = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_pd_lineout = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;
    msleep(250);

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_vref = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    msleep(500);

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_dacl = 1;
    Adac2.bits.adac_pd_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_cp_pd = 1;
    Adac3.bits.adac_cp_buffer_pd = 1;
    Adac3.bits.adac_cp_en_soft = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_ctcm = 1;
    Adac2.bits.adac_pd_ibias = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pd_dacl = 1;
    Adac2.bits.adac_pd_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_mute_lineout = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_pop_dis = 0;
    Adac3.bits.adac_rst_pop = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.adac_pop_pu_pullb = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}
#endif
HI_VOID Digfi_Dac3(HI_VOID)
{
    U_PERI_TIANLA_ADAC3 Adac3;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_ctrl7 = 0x0;
    Adac3.bits.adac_cp_buffer_pd = 0x1;
    Adac3.bits.adac_cp_clk_sel = 0x0;
    Adac3.bits.adac_cp_dt_ctrl = 0x3;
    Adac3.bits.adac_pd_clkramp = 0x0;
    Adac3.bits.adac_chop_bps_lineout = 0x1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;
}

static HI_VOID Digfi_DacInit(HI_UNF_SAMPLE_RATE_E SR, HI_BOOL bResume)
{
    Digfi_DacPoweup(bResume);
    Digfi_DacSetDataBits();
    Digfi_DacSetSampleRate(SR);
    Digfi_DacSetVolume(0x06, 0x06);
}

#if 1
static HI_VOID Digfi_ADACEnable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;
    U_PERI_TIANLA_ADAC3 Adac3;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_cken = 1;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
    udelay(10);

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_rst = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_clksel = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_clkdgesel = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 1;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 0;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
}

static HI_VOID Digfi_ADACDisable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;
    U_PERI_TIANLA_ADAC3 Adac3;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_rst = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 1;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_cken = 0;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
}
#else
static HI_VOID Digfi_ADACEnable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;
    U_PERI_TIANLA_ADAC3 Adac3;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_cken = 1;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
    udelay(10);

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_rst = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_rst = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_clksel = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 1;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 0;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
}

static HI_VOID Digfi_ADACDisable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;
    U_PERI_TIANLA_ADAC3 Adac3;

    Adac3.u32 = g_pstRegPeri->PERI_TIANLA_ADAC3.u32;
    Adac3.bits.adac_rst = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC3.u32 = Adac3.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 1;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_cken = 0;
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
}
#endif

static HI_VOID Digfi_DacInitDigital(HI_VOID)
{
    U_PERI_TIANLA_ADAC0 Adac0;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_pd_dacr = 0;
    Adac0.bits.adac_dig_pd_dacl = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_mute_dacl = 0;
    Adac0.bits.adac_dig_mute_dacr = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    Digfi_DacSetVolume(0x7f, 0x7f);
    Digfi_DacSetPath();
}

static HI_VOID Digfi_DacDeInitDigital(HI_VOID)
{
    U_PERI_TIANLA_ADAC0 Adac0;
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_mute_dacl = 1;
    Adac0.bits.adac_dig_mute_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.adac_dig_pd_dacr = 1;
    Adac0.bits.adac_dig_pd_dacl = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
}

HI_VOID ADAC_TIANLAI_Init(HI_UNF_SAMPLE_RATE_E enSR, HI_BOOL bResume)
{
    Digfi_ADACEnable();
    msleep(1);
#if 1
    Digfi_DacInitDigital();
    Digfi_DacSetAdj();
    Digfi_DacSetPd();
    Digfi_Dac3();
#endif
    Digfi_DacInit(enSR, bResume);
}

HI_VOID ADAC_TIANLAI_DeInit(HI_BOOL bSuspend)
{
    Digfi_DacPowedown(bSuspend);
#if 1
    Digfi_DacDeInitDigital();
#endif
    Digfi_ADACDisable();
}