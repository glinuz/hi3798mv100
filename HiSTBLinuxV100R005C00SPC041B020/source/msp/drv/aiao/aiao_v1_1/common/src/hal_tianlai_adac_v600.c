/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name       		: 	aud_adac.c
  Version        		: 	Initial Draft
  Author         		: 	Hisilicon multimedia software group
  Created       		: 	2010/02/28
  Last Modified		    :
  Description  		    :  Hifi audio dac interface
  Function List 		:
  History       		:
  1.Date        		: 	2010/02/28
    Author      		: 	z40717
    Modification   	    :	Created file

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

#include "hal_tianlai_adac_v600.h"
#include "hi_drv_ao.h"

#include "hi_reg_common.h"
#include "drv_ao_private.h"

#define  DBG_ADAC_DISABLE_TIMER

static HI_VOID Digfi_DacSetVolume(HI_U32 left, HI_U32 right)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    
    Adac0.bits.dacr_vol = right;
    Adac0.bits.dacl_vol = left;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
}

#if 0
static HI_VOID Digfi_DacGetVolume(HI_U32 *left, HI_U32 *right)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    *left = Adac0.bits.dacl_vol;
    *right = Adac0.bits.dacr_vol;
}

static HI_S32 Digfi_DacGetMute(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    return Adac1.bits.smutel;
}

HI_VOID Digfi_DacSetMute(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    // soft mute digi
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    
    Adac1.bits.smuter = 1;
    Adac1.bits.smutel = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;
}
#endif

HI_VOID Digfi_DacSetDataBits(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;    
    Adac1.bits.data_bits = 3;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;
}

HI_VOID Digfi_DacSetAdj(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC2 Adac2;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;    
    Adac2.bits.adj_refbf = 1;
    Adac2.bits.adj_dac = 1;
    Adac2.bits.adj_ctcm = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}

HI_VOID Digfi_DacSetPd(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC2 Adac2;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;    
    Adac2.bits.pd_vref = 0;
    Adac2.bits.Pd_ctcm_ibias = 0;
    Adac2.bits.pd_ibias = 0;
    Adac2.bits.pd_dacr = 0;
    Adac2.bits.pd_dacl = 0;
    Adac2.bits.pd_ctcm = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}

HI_VOID Digfi_DacSetPopAdj(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;    
    Adac1.bits.pop_direct_r = 0;
    Adac1.bits.pop_direct_l = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;
}

HI_VOID Digfi_DacSetChopper(HI_BOOL bEnable)
{
    SC_PERI_TIANLAI_ADAC2 Adac2;

    if(bEnable == HI_TRUE)
    { 
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;        
        Adac2.bits.Chop_bps_ctcm = 0;
        Adac2.bits.Chop_bps_dacvref = 0;
        Adac2.bits.Chop_ctcm_sel = 0;
        Adac2.bits.Chop_dacvref_sel = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    }
    else
    {
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;        
        Adac2.bits.Chop_bps_ctcm = 1;
        Adac2.bits.Chop_bps_dacvref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    }
}

HI_VOID Digfi_DacSetPath(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;

    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;    
    Adac0.bits.dacl_path = 0;
    Adac0.bits.dacr_path = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
}

HI_VOID Digfi_DacSetDeemph(HI_BOOL bEnable, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;
    
    if(bEnable == HI_TRUE)
    {
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;    
        Adac0.bits.dacl_deemph = 1;
        Adac0.bits.dacr_deemph = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
        
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32; 
        switch (enSampleRate)
        {
            case HI_UNF_SAMPLE_RATE_48K:
                Adac0.bits.deemphasis_fs = 0;
                break;
            case HI_UNF_SAMPLE_RATE_44K:
                Adac0.bits.deemphasis_fs = 1;
                break;
            case HI_UNF_SAMPLE_RATE_32K:
                Adac0.bits.deemphasis_fs = 2;
                break;    
            default:
                Adac0.bits.deemphasis_fs = 0;
                break;
        }     
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    }
    else
    {
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;    
        Adac0.bits.dacl_deemph = 0;
        Adac0.bits.dacr_deemph = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    }    
}

#if 0
HI_VOID Digfi_DacSetUnmute(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    // soft mute digi
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;

    Adac1.bits.smuter = 0;
    Adac1.bits.smutel = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;
}
#endif

HI_VOID Digfi_DacSetSampleRate(HI_UNF_SAMPLE_RATE_E SR)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    switch (SR)
    {
    case HI_UNF_SAMPLE_RATE_176K:
    case HI_UNF_SAMPLE_RATE_192K:
        Adac1.bits.clksel = 4;
        break;

    case HI_UNF_SAMPLE_RATE_88K:
    case HI_UNF_SAMPLE_RATE_96K:
        Adac1.bits.clksel = 3;
        break;

    case HI_UNF_SAMPLE_RATE_32K:
    case HI_UNF_SAMPLE_RATE_44K:
    case HI_UNF_SAMPLE_RATE_48K:
        Adac1.bits.clksel = 2;
        break;

    case HI_UNF_SAMPLE_RATE_16K:
    case HI_UNF_SAMPLE_RATE_22K:
    case HI_UNF_SAMPLE_RATE_24K:
        Adac1.bits.clksel = 1;
        break;

    case HI_UNF_SAMPLE_RATE_8K:
    case HI_UNF_SAMPLE_RATE_11K:
    case HI_UNF_SAMPLE_RATE_12K:
        Adac1.bits.clksel = 0;
        break;

    default:
        Adac1.bits.clksel = 2;
        break;
    }
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;

    return;
}

static HI_VOID Digfi_DacPoweup(HI_BOOL bResume)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;
    SC_PERI_TIANLAI_ADAC2 Adac2; 

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.Td_sel = 0x0;  //change it to 00,01,10,11
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 1: pop adj  */
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.pop_adj_res = 0x1;  //change it to 00,01,10,11
    Adac1.bits.pop_adj_clk = 0x1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;

    /* step 3: pd setting 2 */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.pd_dacr = 1;
    Adac2.bits.pd_dacl = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.mute_dacl = 1;
    Adac2.bits.mute_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.pop_direct_l = 0;
    Adac1.bits.pop_direct_r = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;    
 

     /* step 2: pd setting 1 */
     Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
     Adac2.bits.pd_ctcm = 0;
     Adac2.bits.Pd_ctcm_ibias = 0;
     Adac2.bits.pd_ibias = 0;
     g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /* step 4: open popfree */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.popfreel = 1;
    Adac2.bits.popfreer = 1;
#ifdef HI_SND_MUTECTL_SUPPORT
    // fast power up enable
    Adac2.bits.fs = 1;
#else
    //fast power up not enable
    Adac2.bits.fs = 0; //add for popfree
#endif    
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32; 
    
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.pd_vref = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32; 
   

    /* step 5: pop direct setting */
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.pop_direct_l = 1;
    Adac1.bits.pop_direct_r = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;

#ifndef HI_SND_MUTECTL_SUPPORT  
    msleep(500);   
#endif

    /*step 6: open DAC */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.pd_dacr = 0;
    Adac2.bits.pd_dacl = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    
#ifdef HI_SND_MUTECTL_SUPPORT  
    if(HI_TRUE == bResume)
    {
        msleep(10);   
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.fs = 0; //add for popfree
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    }
#endif  

    /*step 7: disable mute */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.mute_dacl = 0;
    Adac2.bits.mute_dacr = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;   

    return;
}

#ifdef HI_SND_MUTECTL_SUPPORT
HI_VOID ADAC_FastPowerEnable(HI_BOOL bEnable)
{
    SC_PERI_TIANLAI_ADAC2 Adac2;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.fs = (HI_U32)bEnable; //rese2t after power up,enable before power down
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}
#endif

static HI_VOID Digfi_DacPowedown(HI_BOOL bSuspend)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;
    SC_PERI_TIANLAI_ADAC2 Adac2;
    HI_U32 u32Volume;

    for(u32Volume = 0x06; u32Volume <= 0x7f; u32Volume += 10)
    {
        Digfi_DacSetVolume(u32Volume,u32Volume);
        msleep(1);
    }

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.pop_adj_res = 0x3;
    Adac1.bits.pop_adj_clk = 0x1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.pop_direct_l = 1;
    Adac1.bits.pop_direct_r = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.mute_dacl = 1;
    Adac2.bits.mute_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    /*step 2: open popfree */
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.popfreel = 1;
    Adac2.bits.popfreer = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.pd_dacl = 1;
    Adac2.bits.pd_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.pop_direct_l = 0;
    Adac1.bits.pop_direct_r = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32; 

#ifndef HI_SND_MUTECTL_SUPPORT
    msleep(2000);
    if(HI_TRUE == bSuspend) //add for suspend popfree
    {
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.fs = 0;
        Adac2.bits.popfreel = 1;
        Adac2.bits.popfreer = 1;
        Adac2.bits.pd_vref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
 
        msleep(2*1000);
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.fs = 1;
        Adac2.bits.popfreel = 0;
        Adac2.bits.popfreer = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
    }
    else
#endif
    {
        /*step 4: pd_vref power down */
        Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
        Adac2.bits.pd_vref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
#ifndef HI_SND_MUTECTL_SUPPORT
        msleep(50);  
#endif
    }
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.pd_ctcm = 1;
    Adac2.bits.Pd_ctcm_ibias = 1;
    Adac2.bits.pd_ibias = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32 = Adac2.u32;
}

static HI_VOID Digfi_DacInit(HI_UNF_SAMPLE_RATE_E SR, HI_BOOL bResume)
{
    Digfi_DacPoweup(bResume);
    Digfi_DacSetDataBits();
    Digfi_DacSetSampleRate(SR);
    Digfi_DacSetVolume(0x06, 0x06);   /* 0dB */
}

/* same as old ADAC */
static HI_VOID Digfi_ADACEnable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;
    SC_PERI_TIANLAI_ADAC2 Adac2;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_cken = 1;  /* ADAC  clk en */
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
    udelay(10);

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.RST = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32= Adac2.u32;
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.RST = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32= Adac2.u32;

    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.clksel = 0;    /* ADAC clksel2  2DIV */
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32= Adac2.u32;
    
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.clkdgesel = 1; //falling edge 
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32= Adac2.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 1;  /* ADAC  soft reset */
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
    
    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 0; /* ADAC  undo soft reset */
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
    return;
}

/* same as old ADAC */
static HI_VOID Digfi_ADACDisable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;
    SC_PERI_TIANLAI_ADAC2 Adac2;
    
    Adac2.u32 = g_pstRegPeri->PERI_TIANLA_ADAC2.u32;
    Adac2.bits.RST = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC2.u32= Adac2.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 1;  /* ADAC Datapath soft reset */
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_cken = 0;  /* ADAC  clk disable */
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
    return;
}

static HI_VOID Digfi_DacInitDigital(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;
    
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.pd_dacr = 0;
    Adac0.bits.pd_dacl = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.mute_dacl = 0;
    Adac0.bits.mute_dacr = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    
    Digfi_DacSetVolume(0x7f, 0x7f); 
    Digfi_DacSetPath();
}

static HI_VOID Digfi_DacDeInitDigital(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.mute_dacl = 1;
    Adac0.bits.mute_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.pd_dacr = 1;
    Adac0.bits.pd_dacl = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
}

HI_VOID ADAC_TIANLAI_Init(HI_UNF_SAMPLE_RATE_E enSR, HI_BOOL bResume)
{
    Digfi_ADACEnable(); 
    msleep(1);
    Digfi_DacInitDigital();
    Digfi_DacInit(enSR, bResume);
}

HI_VOID ADAC_TIANLAI_DeInit(HI_BOOL bSuspend)
{
    Digfi_DacPowedown(bSuspend);
    Digfi_DacDeInitDigital();
    Digfi_ADACDisable();
    return;
}

