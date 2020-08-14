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

#include "hal_tianlai_adac_v500.h"
#include "hi_drv_ao.h"

#include "hi_reg_common.h"
#include "drv_ao_private.h"

#if defined(HI_SND_MUTECTL_SUPPORT) && (defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100))
#include "hi_drv_sys.h"
#endif

#define  DBG_ADAC_DISABLE_TIMER

/*----------------------------audio codec-----------------------------------*/

/*
0~0x7f
0:  +6dB
6:   0dB
7f: -121dB
 */
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
#endif

HI_VOID Digfi_DacSetMute(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    // soft mute digi
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    
    Adac1.bits.smuter = 1;
    Adac1.bits.smutel = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;
}


HI_VOID Digfi_DacSetUnmute(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    // soft mute digi
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;

    Adac1.bits.smuter = 0;
    Adac1.bits.smutel = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;
}

HI_VOID Digfi_DacSetSampleRate(HI_UNF_SAMPLE_RATE_E SR)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    switch (SR)
    {
    case HI_UNF_SAMPLE_RATE_176K:
    case HI_UNF_SAMPLE_RATE_192K:
        Adac1.bits.sample_sel = 4;
        break;

    case HI_UNF_SAMPLE_RATE_88K:
    case HI_UNF_SAMPLE_RATE_96K:
        Adac1.bits.sample_sel = 3;
        break;

    case HI_UNF_SAMPLE_RATE_32K:
    case HI_UNF_SAMPLE_RATE_44K:
    case HI_UNF_SAMPLE_RATE_48K:
        Adac1.bits.sample_sel = 2;
        break;

    case HI_UNF_SAMPLE_RATE_16K:
    case HI_UNF_SAMPLE_RATE_22K:
    case HI_UNF_SAMPLE_RATE_24K:
        Adac1.bits.sample_sel = 1;
        break;

    case HI_UNF_SAMPLE_RATE_8K:
    case HI_UNF_SAMPLE_RATE_11K:
    case HI_UNF_SAMPLE_RATE_12K:
        Adac1.bits.sample_sel = 0;
        break;

    default:
        Adac1.bits.sample_sel = 2;
        break;
    }
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32 = Adac1.u32;

    return;
}

static HI_VOID Digfi_DacPoweup(HI_BOOL bResume)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;
    SC_PERI_TIANLAI_ADAC1 Adac1; 
#ifndef HI_SND_MUTECTL_SUPPORT
    if(HI_TRUE == bResume)
    {
        //msleep(100); //add for resume popfree
    }
#endif
    /* open soft mute */
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.smuter = 1;
    Adac1.bits.smutel = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;

    /* step 1: open popfree */
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.popfreel = 1;
    Adac0.bits.popfreer = 1;
#ifdef HI_SND_MUTECTL_SUPPORT
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    {
        HI_CHIP_PACKAGE_TYPE_E enPackageType = HI_CHIP_PACKAGE_TYPE_BUTT;

        HI_DRV_SYS_GetChipPackageType(&enPackageType);
        if (HI_CHIP_PACKAGE_TYPE_QFP_216 != enPackageType)
        {
            // fast power up enable
            Adac0.bits.fs = 1;
        }
        else
        {
            //fast power up not enable
            Adac0.bits.fs = 0; //add for popfree
        }
    }
#else
    // fast power up enable
    Adac0.bits.fs = 1;
#endif
#else
    //fast power up not enable
    Adac0.bits.fs = 0; //add for popfree
#endif    
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    /*step 2: pd_vref power up	*/
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.pd_vref = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;//0xf0 

    /*step 3: open DAC */
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.pd_dacr = 0;
    Adac0.bits.pd_dacl = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;//0x30

#ifdef HI_SND_MUTECTL_SUPPORT
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    {
        HI_CHIP_PACKAGE_TYPE_E enPackageType = HI_CHIP_PACKAGE_TYPE_BUTT;

        HI_DRV_SYS_GetChipPackageType(&enPackageType);
        if (HI_CHIP_PACKAGE_TYPE_QFP_216 != enPackageType)
        {
            if(HI_TRUE == bResume)
            {
                msleep(10);   
                Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
                Adac0.bits.fs = 0; //add for popfree
                g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
            }
        }
    }
#else
    if(HI_TRUE == bResume)
    {
        msleep(10);   
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.fs = 0; //add for popfree
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    }
#endif
#endif  

    /*step 4: close profree */
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.popfreel = 0;
    Adac0.bits.popfreer = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;//0x00

    /*step 5: disable mute */
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.mute_dacl = 0;
    Adac0.bits.mute_dacr = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    /* soft unmute */
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.smutel = 0;      
    Adac1.bits.smuter = 0;
    Adac1.bits.sunmutel = 1;
    Adac1.bits.sunmuter = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;
     
    return;
}

#ifdef HI_SND_MUTECTL_SUPPORT
HI_VOID ADAC_FastPowerEnable(HI_BOOL bEnable)
{
	SC_PERI_TIANLAI_ADAC0 Adac0;

	Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.fs = (HI_U32)bEnable; //reset after power up,enable before power down
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
}
#endif

static HI_VOID Digfi_DacPowedown(HI_BOOL bSuspend)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;
    SC_PERI_TIANLAI_ADAC1 Adac1;

    // soft mute digi
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.smutel = 1;
    Adac1.bits.smuter = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;
    //udelay(1000); // request??

    /*step 1: enable mute */
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.mute_dacl = 1;
    Adac0.bits.mute_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    /*step 2: open popfree */
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.popfreel = 1;
    Adac0.bits.popfreer = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    /*step 3: close DAC */
    Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
    Adac0.bits.pd_dacl = 1;
    Adac0.bits.pd_dacr = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
#ifdef HI_SND_MUTECTL_SUPPORT
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    {
        HI_CHIP_PACKAGE_TYPE_E enPackageType = HI_CHIP_PACKAGE_TYPE_BUTT;

        HI_DRV_SYS_GetChipPackageType(&enPackageType);
        if (HI_CHIP_PACKAGE_TYPE_QFP_216 == enPackageType)
        {   //HI_SND_MUTECTL_SUPPORT is not defined
            if(HI_TRUE == bSuspend) //add for suspend popfree
            {
                Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
                Adac0.bits.fs = 0;
                Adac0.bits.popfreel = 1;
                Adac0.bits.popfreer = 1;
                Adac0.bits.pd_vref = 1;
                g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
         
                msleep(2*1000);
                Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
                Adac0.bits.fs = 1;
                Adac0.bits.popfreel = 0;
                Adac0.bits.popfreer = 0;
                g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
            }
            else
            {
                /*step 4: pd_vref power down	*/
                Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
                Adac0.bits.pd_vref = 1;
                g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
            }
        }
        else
        {   //HI_SND_MUTECTL_SUPPORT is defined
            /*step 4: pd_vref power down	*/
            Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
            Adac0.bits.pd_vref = 1;
            g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
        }
    }
#else
    {   //HI_SND_MUTECTL_SUPPORT is defined
        /*step 4: pd_vref power down	*/
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.pd_vref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    }
#endif

#else
    //HI_SND_MUTECTL_SUPPORT is not defined
    if(HI_TRUE == bSuspend) //add for suspend popfree
    {
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.fs = 0;
        Adac0.bits.popfreel = 1;
        Adac0.bits.popfreer = 1;
        Adac0.bits.pd_vref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
 
        msleep(2*1000);
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.fs = 1;
        Adac0.bits.popfreel = 0;
        Adac0.bits.popfreer = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    }
    else
    {
        /*step 4: pd_vref power down	*/
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.pd_vref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    }
#endif
    return;
}


static HI_VOID Digfi_DacInit(HI_UNF_SAMPLE_RATE_E SR, HI_BOOL bResume)
{
    Digfi_DacPoweup(bResume);
    Digfi_DacSetSampleRate(SR);
    Digfi_DacSetVolume(0x06, 0x06);   /* 0dB */
}


/* same as old ADAC */
static HI_VOID Digfi_ADACEnable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;
    SC_PERI_TIANLAI_ADAC1 Adac1;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_cken = 1;  /* ADAC  clk en */
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
    udelay(10);

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.rst = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.rst = 0;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;

    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.clksel2 = 0;			/* ADAC clksel2  2DIV */
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;

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
    SC_PERI_TIANLAI_ADAC1 Adac1;
    
    Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
    Adac1.bits.rst = 1;
    g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_srst_req = 1;  /* ADAC Datapath soft reset */
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;

    AdacCfg.u32 = g_pstRegCrg->PERI_CRG69.u32;
    AdacCfg.bits.adac_cken = 0;  /* ADAC  clk disable */
    g_pstRegCrg->PERI_CRG69.u32 = AdacCfg.u32;
    return;
}

/*
The start-up sequence consists on several steps in a pre-determined order as follows:
1. select the master clock mode (256 or 384 x Fs)
2. start the master clock
3. set pdz to high
4. select the sampling rate
5. reset the signal path (rstdpz to low and back to high after 100ns)
6. start the individual codec blocks
 */

HI_VOID ADAC_TIANLAI_Init(HI_UNF_SAMPLE_RATE_E enSR, HI_BOOL bResume)
{
    Digfi_ADACEnable();
    msleep(1);   //discharge
    Digfi_DacInit(enSR, bResume);
}

HI_VOID ADAC_TIANLAI_DeInit(HI_BOOL bSuspend)
{
    Digfi_DacPowedown(bSuspend);
    Digfi_ADACDisable();
    return;
}
static HI_VOID Digfi_DacPowerEnable(HI_BOOL bEnable)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;
    SC_PERI_TIANLAI_ADAC1 Adac1; 

    if(bEnable == HI_TRUE)
    {
        /* open soft mute */
        Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
        Adac1.bits.smuter = 1;
        Adac1.bits.smutel = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;
    
        /* step 1: open popfree */
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.popfreel = 1;
        Adac0.bits.popfreer = 1;
    
        //fast power up not enable
        Adac0.bits.fs = 0; //add for popfree
      
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    
        /*step 2: pd_vref power up	*/
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.pd_vref = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;//0xf0 
    
        /*step 3: open DAC */
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.pd_dacr = 0;
        Adac0.bits.pd_dacl = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;//0x30 
    
        /*step 4: close profree */
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.popfreel = 0;
        Adac0.bits.popfreer = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;//0x00
    
        /*step 5: disable mute */
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.mute_dacl = 0;
        Adac0.bits.mute_dacr = 0;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
    
        /* soft unmute */
        Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
        Adac1.bits.smutel = 0;      
        Adac1.bits.smuter = 0;
        Adac1.bits.sunmutel = 1;
        Adac1.bits.sunmuter = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;
    }
    else
    {
        // soft mute digi
        Adac1.u32 = g_pstRegPeri->PERI_TIANLA_ADAC1.u32;
        Adac1.bits.smutel = 1;
        Adac1.bits.smuter = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC1.u32= Adac1.u32;
        //udelay(1000); // request??
        
        /*step 1: enable mute */
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.mute_dacl = 1;
        Adac0.bits.mute_dacr = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
        
        /*step 2: open popfree */
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.popfreel = 1;
        Adac0.bits.popfreer = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
        
        /*step 3: close DAC */
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.pd_dacl = 1;
        Adac0.bits.pd_dacr = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;
        
        /*step 4: pd_vref power down    */
        Adac0.u32 = g_pstRegPeri->PERI_TIANLA_ADAC0.u32;
        Adac0.bits.pd_vref = 1;
        g_pstRegPeri->PERI_TIANLA_ADAC0.u32 = Adac0.u32;

    }
     
    return;
}

HI_VOID ADAC_TIANLAI_Enable(HI_UNF_SAMPLE_RATE_E enSR, HI_BOOL bEnable)
{
    if(bEnable == HI_TRUE)
    {
         Digfi_ADACEnable();
         msleep(1);   //discharge
         Digfi_DacPowerEnable(bEnable);
         Digfi_DacSetSampleRate(enSR);
         Digfi_DacSetVolume(0x06, 0x06);   /* 0dB */
    }
    else
    {
        Digfi_DacPowerEnable(bEnable);
        Digfi_ADACDisable();
    }
    return;
}