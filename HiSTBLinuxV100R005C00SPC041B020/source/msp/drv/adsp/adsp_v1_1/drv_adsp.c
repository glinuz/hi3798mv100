/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpBeIL3vAuLvtpwmY1JB/WFe97hRqOpHstvurNiV
m9r5rmG2JHUxv3gdk3FD9IXdaBnBvCy6WcBOB5UVwY3YlkKICRO+Ilq1k18EFYGeRTzMKC0K
AptbNu0Vi2u71aLo/vZNii8FY+Pojq7UFKUgJEFv+e7RfZss7srQzUSs2g64rQ==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/
/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_adsp_intf_k.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/17
  Description   :
  History       :
  1.Date        : 2013/04/17
    Author      : zgjie
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#if defined (__aarch64__)
#include <asm/cacheflush.h>
#endif

/* Unf headers */
#include "hi_error_mpi.h"
#include "hi_reg_common.h"

/* Drv headers */
#include "drv_adsp_private.h"
#include "drv_adsp_ext.h"
#include "hi_audsp_common.h"
#include "hi_audsp_aoe.h"
#include "hi_audsp_aflt.h"
#include "hi_drv_mmz.h"

#ifdef HI_SND_DSP_HW_SUPPORT
/*kfile operation*/
#include "kfile_ops_func.h"
#include "dsp_elf_func.h"
#include <asm/cacheflush.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
#ifndef HI_SND_DSP_HW_SUPPORT
extern HI_S32 AOE_SwEngineCreate(HI_VOID);
extern HI_S32 AOE_SwEngineDestory(HI_VOID);
#endif

#if defined (HI_SND_AFLT_SWSIMULATE_SUPPORT)
extern HI_S32 AFLT_SwEngineCreate(HI_VOID);
extern HI_S32 AFLT_SwEngineDestory(HI_VOID);
#endif

#define ADSP_NAME "HI_ADSP"

#ifdef HI_SND_DSP_HW_SUPPORT
#define ADSP_ELF_USE_ARRAY //defaule config
//#define ADSP_ELF_USE_FILE
#define ADSP_ELFNAME "/mnt/HWAOE"

#define AOE_COMBUFFER_ADDR_ALIGN (32)
#define ELF_MAXLENGTH (2 * 1024 * 1024)
#define WAITING_LOOP 10

static volatile S_ADSP_CHN_REGS_TYPE* g_pADSPSysCrg = HI_NULL;
#endif

/*************************** Structure Definition ****************************/

/* Global parameter */
typedef struct
{
    atomic_t                atmOpenCnt;     /* Open times */
    HI_BOOL                 bReady;         /* Init flag */
#if (1 == HI_PROC_SUPPORT)
    ADSP_REGISTER_PARAM_S*  pstProcParam;   /* ADSP Proc functions */
#endif
    ADSP_EXPORT_FUNC_S stExtFunc;      /* ADSP extenal functions */
    ADSP_SETTINGS_S stADSPSetting;
} ADSP_GLOBAL_PARAM_S;

#if (1 == HI_PROC_SUPPORT)
static HI_VOID* g_pvAoeRegAddr = HI_NULL;
#ifdef __DPT__
static HI_VOID* g_pvAfltRegAddr = HI_NULL;
#endif
#endif

/***************************** Static Definition *****************************/
#ifdef HI_SND_DSP_HW_SUPPORT
static ADSP_AOESYS_CMD_RET_E  ADSP_AOE_Ack(HI_VOID)
{
    volatile HI_U32 loop = 0;
    U_ADSP_CTRL ADSP_SYSCTRL;

    for (loop = 0; loop < 100; loop++)
    {
        //udelay(1000);
        msleep(1);
        ADSP_SYSCTRL.u32 = g_pADSPSysCrg->ADSP_SYSCTRL.u32;
        if (ADSP_SYSCTRL.bits.aoe_cmd_done)
        {
            return (ADSP_AOESYS_CMD_RET_E)ADSP_SYSCTRL.bits.aoe_cmd_return_value;
        }
    }

    return ADSP_AOE_CMD_ERR_TIMEOUT;
}

static HI_S32 ADSP_AOE_SetCmd(ADSP_AOESYS_CMD_E newcmd)
{
    ADSP_AOESYS_CMD_RET_E Ack;
    U_ADSP_CTRL ADSP_SYSCTRL;
    ADSP_SYSCTRL.u32 = g_pADSPSysCrg->ADSP_SYSCTRL.u32;

    switch (newcmd)
    {
        case ADSP_AOE_CMD_START:
            ADSP_SYSCTRL.bits.aoe_cmd = ADSP_AOE_CMD_START;
            break;

        case ADSP_AOE_CMD_STOP:
            ADSP_SYSCTRL.bits.aoe_cmd = ADSP_AOE_CMD_STOP;
            break;

        default:
            return HI_SUCCESS;
    }

    ADSP_SYSCTRL.bits.aoe_cmd_done = 0;
    g_pADSPSysCrg->ADSP_SYSCTRL.u32 =  ADSP_SYSCTRL.u32;

    Ack = ADSP_AOE_Ack();
    if (ADSP_AOE_CMD_DONE != Ack)
    {
        HI_ERR_ADSP("ADSP_AOE SetCmd(0x%x) failed(0x%x)\n", newcmd, Ack);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef __DPT__
static ADSP_AFLTSYS_CMD_RET_E  ADSP_AFLT_Ack(HI_VOID)
{
    volatile HI_U32 loop = 0;
    U_ADSP_CTRL ADSP_SYSCTRL;

    for (loop = 0; loop < 100; loop++)
    {
        //udelay(1000);
        msleep(1);
        ADSP_SYSCTRL.u32 = g_pADSPSysCrg->ADSP_SYSCTRL.u32;
        if (ADSP_SYSCTRL.bits.aflt_cmd_done)
        {
            return (ADSP_AFLTSYS_CMD_RET_E)ADSP_SYSCTRL.bits.aflt_cmd_return_value;
        }
    }

    return ADSP_AFLT_CMD_ERR_TIMEOUT;
}

static HI_S32 ADSP_AFLT_SetCmd(ADSP_AFLTSYS_CMD_E newcmd)
{
    ADSP_AFLTSYS_CMD_RET_E Ack;
    U_ADSP_CTRL          ADSP_SYSCTRL;
    ADSP_SYSCTRL.u32 = g_pADSPSysCrg->ADSP_SYSCTRL.u32;

    switch (newcmd)
    {
        case ADSP_AFLT_CMD_START:
            ADSP_SYSCTRL.bits.aflt_cmd = ADSP_AFLT_CMD_START;
            break;

        case ADSP_AFLT_CMD_STOP:
            ADSP_SYSCTRL.bits.aflt_cmd = ADSP_AFLT_CMD_STOP;
            break;

        default:
            return HI_SUCCESS;
    }

    ADSP_SYSCTRL.bits.aflt_cmd_done = 0;
    g_pADSPSysCrg->ADSP_SYSCTRL.u32 =  ADSP_SYSCTRL.u32;

    Ack = ADSP_AFLT_Ack();
    if (ADSP_AFLT_CMD_DONE != Ack)
    {
        HI_ERR_ADSP("ADSP_AFLT SetCmd(0x%x) failed(0x%x)\n", newcmd, Ack);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#endif

static ADSP_SYSCOM_CMD_RET_E  ADSP_SYS_Ack(HI_VOID)
{
    volatile HI_U32 loop = 0;
    U_ADSP_CTRL          ADSP_SYSCTRL;

    for (loop = 0; loop < 100; loop++)
    {
        //udelay(1000);
        msleep(1);
        ADSP_SYSCTRL.u32 = g_pADSPSysCrg->ADSP_SYSCTRL.u32;
        if (ADSP_SYSCTRL.bits.sys_cmd_done)
        {
            return (ADSP_SYSCOM_CMD_RET_E)ADSP_SYSCTRL.bits.sys_cmd_return_value;
        }
    }

    return ADSP_SYSCOM_CMD_ERR_TIMEOUT;
}

static HI_S32 ADSP_SYS_SetCmd(ADSP_SYSCOM_CMD_E newcmd, HI_BOOL bBlock)
{
    ADSP_SYSCOM_CMD_RET_E Ack;
    U_ADSP_CTRL ADSP_SYSCTRL;

    ADSP_SYSCTRL.u32 = g_pADSPSysCrg->ADSP_SYSCTRL.u32;

    switch (newcmd)
    {
        case ADSP_SYSCOM_CMD_START:
            ADSP_SYSCTRL.bits.sys_cmd = ADSP_SYSCOM_CMD_START;
            break;

        case ADSP_SYSCOM_CMD_STOP:
            ADSP_SYSCTRL.bits.sys_cmd = ADSP_SYSCOM_CMD_STOP;
            break;

        default:
            return HI_SUCCESS;
    }

    ADSP_SYSCTRL.bits.sys_cmd_done = 0;
    g_pADSPSysCrg->ADSP_SYSCTRL.u32 =  ADSP_SYSCTRL.u32;
    if (HI_TRUE == bBlock)
    {
        Ack = ADSP_SYS_Ack();
        if (ADSP_SYSCOM_CMD_DONE != Ack)
        {
            HI_ERR_ADSP("ADSP_SYS SetCmd(0x%x) failed(0x%x)\n", newcmd, Ack);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}
#endif

static HI_S32 ADSP_LoadFirmware(ADSP_CODEID_E u32DspCodeId);
static HI_S32 ADSP_UnLoadFirmware(ADSP_CODEID_E u32DspCodeId);
static HI_S32 ADSP_GetFirmwareInfo(ADSP_CODEID_E u32DspCodeId, ADSP_FIRMWARE_INFO_S* pstFrm);
static HI_S32 ADSP_GetAoeFwmInfo(ADSP_CODEID_E u32DspCodeId, ADSP_FIRMWARE_AOE_INFO_S* pstInfo);

static ADSP_GLOBAL_PARAM_S s_stAdspDrv =
{
    .atmOpenCnt                    = ATOMIC_INIT(0),
    .bReady                        = HI_FALSE,

#if (1 == HI_PROC_SUPPORT)
    .pstProcParam                  = HI_NULL,
#endif

    .stExtFunc                     =
    {
        .pfnADSP_LoadFirmware      = ADSP_LoadFirmware,
        .pfnADSP_UnLoadFirmware    = ADSP_UnLoadFirmware,
        .pfnADSP_GetFirmwareInfo   = ADSP_GetFirmwareInfo,
        .pfnADSP_GetAoeFwmInfo     = ADSP_GetAoeFwmInfo,
        .pfnADSP_DrvResume         = ADSP_DRV_Resume,
        .pfnADSP_DrvSuspend        = ADSP_DRV_Suspend,
    }
};

static HI_S32 ADSP_LoadFirmware(ADSP_CODEID_E u32DspCodeId)
{
    HI_S32 s32Ret = HI_FAILURE;

    switch (u32DspCodeId)
    {
        case ADSP_CODE_AOE:
#ifdef HI_SND_DSP_HW_SUPPORT
            s32Ret = ADSP_AOE_SetCmd(ADSP_AOE_CMD_START);
#else
            s32Ret = AOE_SwEngineCreate();
#endif
            break;

#ifdef __DPT__
        case ADSP_CODE_AFLT:
#if defined (HI_SND_AFLT_SWSIMULATE_SUPPORT)
            s32Ret = AFLT_SwEngineCreate();
#else
            s32Ret = ADSP_AFLT_SetCmd(ADSP_AFLT_CMD_START);
#endif
            break;
#endif
        default:
            HI_ERR_ADSP("dont support DspCode(%d)\n", u32DspCodeId);
            break;
    }

    if (HI_SUCCESS == s32Ret)
    {
        s_stAdspDrv.bReady = HI_TRUE;
    }

    return s32Ret;
}

static HI_S32 ADSP_UnLoadFirmware(ADSP_CODEID_E u32DspCodeId)
{
    HI_S32 s32Ret = HI_SUCCESS;

    switch (u32DspCodeId)
    {
        case ADSP_CODE_AOE:
#ifdef HI_SND_DSP_HW_SUPPORT
            s32Ret = ADSP_AOE_SetCmd(ADSP_AOE_CMD_STOP);
#else
            s32Ret = AOE_SwEngineDestory();
#endif
            break;

#ifdef __DPT__
        case ADSP_CODE_AFLT:
#ifdef HI_SND_DSP_HW_SUPPORT
            s32Ret = ADSP_AFLT_SetCmd(ADSP_AFLT_CMD_STOP);
#else
            s32Ret = AFLT_SwEngineDestory();
#endif
            break;
#endif

        default:
            HI_WARN_ADSP("dont support DspCode(%d)\n", u32DspCodeId);
            break;
    }

    if (HI_SUCCESS == s32Ret)
    {
        s_stAdspDrv.bReady = HI_FALSE;
    }

    return s32Ret;
}

static HI_S32 ADSP_GetFirmwareInfo(ADSP_CODEID_E u32DspCodeId, ADSP_FIRMWARE_INFO_S* pstFrm)
{
    if ((HI_NULL == pstFrm))
    {
        HI_ERR_ADSP("Bad param!\n");
        return HI_FAILURE;
    }

    // todo

    return HI_SUCCESS;
}

static HI_S32 ADSP_GetAoeFwmInfo(ADSP_CODEID_E u32DspCodeId, ADSP_FIRMWARE_AOE_INFO_S* pstInfo)
{
    HI_S32 Ret = HI_SUCCESS;

    switch (u32DspCodeId)
    {
        case ADSP_CODE_AOE:
#ifdef HI_SND_DSP_HW_SUPPORT
            pstInfo->bAoeSwFlag = HI_FALSE;
#else
            pstInfo->bAoeSwFlag = HI_TRUE;
#endif
            break;
        default:
            HI_ERR_ADSP("dont support DspCode(%d)\n", u32DspCodeId);
            Ret = HI_FAILURE;
            break;
    }

    return Ret;
}

/*********************************** Code ************************************/
#ifdef HI_SND_DSP_HW_SUPPORT

#if defined ADSP_ELF_USE_ARRAY
static const HI_UCHAR dsp_elf_array[] =
{
#include "firmware/hififw.dat"
};
#endif

static int mem_is_reserved(u32 phyaddr, u32 size)
{
    u32  pfn_start;
    u32  pfn_end;

    pfn_start = __phys_to_pfn(phyaddr);
    pfn_end = __phys_to_pfn(phyaddr + size);

    for (; pfn_start < pfn_end; pfn_start++)
    {
        struct page* page = pfn_to_page(pfn_start);
        if (!PageReserved(page))
        {
            return -ENOMEM;
        }
    }

    return 0;
}
static HI_S32 Dsp_LoadElf(HI_VOID)
{
    HI_S32 s32Ret;
    HI_UCHAR* pcELFBuf = HI_NULL;

    // TODO: ADSP_ID_AOE没有定义
    pcELFBuf = HI_VMALLOC(ADSP_ID_AOE, ELF_MAXLENGTH);
    if (HI_NULL == pcELFBuf)
    {
        HI_FATAL_ADSP("pcELFBuf HI_VMALLOC fail\n");
        return HI_FAILURE;
    }
    memset((HI_VOID*)pcELFBuf, 0, ELF_MAXLENGTH);

#ifdef  ADSP_ELF_USE_FILE
    //1. read elf file
    struct file* fpELF;
    HI_U32 u32readsize = 0;
    fpELF = kfile_open(ADSP_ELFNAME, O_RDONLY, 0);
    if (HI_NULL == fpELF)
    {
        HI_FATAL_ADSP("ELF file %s open fail\n", ADSP_ELFNAME);
        HI_VFREE(ADSP_ID_AOE, pcELFBuf);
        return HI_FAILURE;
    }

    u32readsize = kfile_read(pcELFBuf, ELF_MAXLENGTH, fpELF);
    if (u32readsize <= 0)
    {
        HI_FATAL_ADSP("ELF file  read fail\n");
        kfile_close(fpELF);
        HI_VFREE(ADSP_ID_AOE, pcELFBuf);
        return -EACCES;
    }

    kfile_close(fpELF);
    HI_INFO_ADSP("Read ELF file size 0x%x \n", u32readsize);

#elif defined ADSP_ELF_USE_ARRAY
    memcpy(pcELFBuf, dsp_elf_array, sizeof(dsp_elf_array));
    HI_INFO_ADSP("Read ELF array size 0x%x \n", sizeof(dsp_elf_array));
#endif

    //judge whether the addr for adsp is reserve
    s32Ret = mem_is_reserved(DSP_DDR_CODE_BASEADDR, DSP_DDR_CODE_MAXSIZE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("mem_is_reserved return : %d !\n", s32Ret);
        HI_FATAL_ADSP(" Memory is not reserved, adsp addr = 0x%x, size = 0x%x\n", DSP_DDR_CODE_BASEADDR, DSP_DDR_CODE_MAXSIZE);
        HI_VFREE(ADSP_ID_AOE, pcELFBuf);
        return s32Ret;
    }

    //2. load elf file
    //flush ddr memory
    HI_INFO_ADSP("ResetELFSection\n");
    ResetELFSection(DSP_DDR_CODE_BASEADDR, DSP_DDR_CODE_MAXSIZE);

    //copy elf to reg and ddr
    CopyELFSection(pcELFBuf);

    //flush cache to ddr (all size)
#ifdef __aarch64__
    __flush_dcache_area(phys_to_virt(DSP_DDR_CODE_BASEADDR), DSP_DDR_CODE_MAXSIZE);
#else
    __cpuc_flush_dcache_area(phys_to_virt(DSP_DDR_CODE_BASEADDR), DSP_DDR_CODE_MAXSIZE);
    outer_flush_range(DSP_DDR_CODE_BASEADDR, DSP_DDR_CODE_MAXSIZE);
#endif

    //check elf copyed right
    if (CheckELFPaser(pcELFBuf) == HI_FAILURE)
    {
        HI_FATAL_ADSP("ELF Load  fail\n");
        HI_VFREE(ADSP_ID_AOE, pcELFBuf);
        return HI_FAILURE;
    }

    HI_VFREE(ADSP_ID_AOE, pcELFBuf);

    return HI_SUCCESS;
}

static HI_S32 Dsp_ResetBoot(HI_VOID)
{
    U_PERI_CRG52 uTmpVal1;
    HI_S32 s32Ret;
    //remap reg

#if defined(CHIP_TYPE_hi3751v100) || defined(CHIP_TYPE_hi3751v100b)
    U_DSP_CTRL  uTmpVal;
    //3. reset dsp
    uTmpVal.u32 = g_pstRegPeri->DSP_CTRL.u32;

#ifdef ADSP_SELLECT_DSP0
    uTmpVal.bits.peri_wdg1_en_dsp0 = 0;
    uTmpVal.bits.peri_ocdhaltonreset_dsp0 = 0;
#if 1  //boot from bootreg
    uTmpVal.bits.peri_statvectorsel_dsp0 = 0;
#else //boot from ddr
    uTmpVal.bits.peri_statvectorsel_dsp0 = 1;
#endif
    uTmpVal.bits.peri_runstall_dsp0 = 0;
#endif

#ifdef ADSP_SELLECT_DSP1
    uTmpVal.bits.peri_wdg1_en_dsp1 = 0;
    uTmpVal.bits.peri_ocdhaltonreset_dsp1 = 0;
    //boot from bootreg
    uTmpVal.bits.peri_statvectorsel_dsp1 = 0;
    uTmpVal.bits.peri_runstall_dsp1 = 0;
#endif

    g_pstRegPeri->DSP_CTRL.u32 = uTmpVal.u32;

#ifdef ADSP_SELLECT_DSP0
    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp_clk_sel = 1;           //300M
    uTmpVal1.bits.dsp0_cken = 1;            //dsp0 clk enable
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp0_srst_req = 1;          //dsp0 bus sreset
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp0_srst_req = 0;
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;
#endif

#ifdef ADSP_SELLECT_DSP1
    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp_clk_sel = 1;           //S5 300M
    uTmpVal1.bits.dsp1_cken = 1;            //dsp1 clk enable
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp1_srst_req = 1;     //dsp1 bus sreset
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp1_srst_req = 0;
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;
#endif

#elif  defined(CHIP_TYPE_hi3751v600)  \
    || defined(CHIP_TYPE_hi3751lv500) \
    || defined(CHIP_TYPE_hi3751v500)  \
    || defined(CHIP_TYPE_hi3751v620)
    //3. reset dsp
    U_DSP_CTRL  uTmpVal;
    uTmpVal.u32 = g_pstRegPeri->DSP_CTRL.u32;

    uTmpVal.bits.peri_wdg1_en_dsp1 = 0;
    uTmpVal.bits.peri_ocdhaltonreset_dsp1 = 0;
    uTmpVal.bits.peri_statvectorsel_dsp1 = 0;
    uTmpVal.bits.peri_runstall_dsp1 = 0;

    g_pstRegPeri->DSP_CTRL.u32 = uTmpVal.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
#if !defined(CHIP_TYPE_hi3751v500) && !defined(CHIP_TYPE_hi3751v620)      //v500 have no clock select
    uTmpVal1.bits.dsp_clk_sel = 0b11;          //S5 300M
#endif
    uTmpVal1.bits.dsp0_cken = 1;            //dsp1 clk enable
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp0_srst_req = 1;        //dsp1 bus sreset
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp0_srst_req = 0;
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

#else  //#if defined CHIP_TYPE_hi3751v100  for cv200 etc
    U_DSP0_CTRL  uTmpVal;
    //3. reset dsp
    uTmpVal.u32 = g_pstRegPeri->DSP0_CTRL.u32;
    uTmpVal.bits.wdg1_en_dsp0 = 0;
    uTmpVal.bits.ocdhaltonreset_dsp0 = 0;

    //boot from bootreg
    uTmpVal.bits.statvectorsel_dsp0 = 0;
    uTmpVal.bits.runstall_dsp0 = 0;
    g_pstRegPeri->DSP0_CTRL.u32 = uTmpVal.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;

    uTmpVal1.u32 = 0x77;
    uTmpVal1.bits.dsp_clk_sel = 0;  //345.6M
    //uTmpVal1..bits.dsp_cken = 1;
    uTmpVal1.bits.dsp0_cken = 1;   //dsp0 clk enable
    //uTmpVal1.bits.dsp1_cken = 0;   //dsp1 clk disable
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp_srst_req  = 1;  //dsp bus sreset
    uTmpVal1.bits.dsp0_srst_req = 1;   //dsp0 sreset
    //uTmpVal1.bits.dsp1_srst_req = 1;   //dsp0 sreset
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;

    uTmpVal1.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal1.bits.dsp_srst_req  = 0;
    uTmpVal1.bits.dsp0_srst_req = 0;
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal1.u32;
#endif  //#if defined CHIP_TYPE_hi3751v100

    //4. check dsp running
#if 1
    s32Ret = ADSP_SYS_SetCmd(ADSP_SYSCOM_CMD_START, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("Dsp start  fail: 0x%x!\n", s32Ret);
        return s32Ret;
    }
#else
    HI_INFO_ADSP("u32RunningFlagVirtAddr=0x%x  \n", u32RunningFlagVirtAddr);
    pdsp0_running_flag_addr = (HI_U32*)u32RunningFlagVirtAddr;

    if (HI_NULL == pdsp0_running_flag_addr)
    {
        HI_FATAL_ADSP("IoRemap Dsp0 Running Flag addr  fail\n");
        return HI_FAILURE;
    }

    while (0 == u32RunFlagCheck)
    {
        u32WaitingLoop++;
        if (*pdsp0_running_flag_addr != AOE_RUNNING_FLAG)
        {
            if (u32WaitingLoop == WAITING_LOOP)
            {
                HI_FATAL_ADSP("Dsp0 Start TimeOut \n");
                break;
            }
            else
            {
                udelay(1000);
            }
        }
        else
        {
            u32RunFlagCheck = 1;
        }
    }

    if (0 != u32RunFlagCheck)
    {
        HI_INFO_ADSP("Dsp0 running, And The Value 0x%x \n", *(volatile HI_U32*)pdsp0_running_flag_addr);
    }
    else
    {
        HI_FATAL_ADSP("Dsp0 running Failed  \n");
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}

#endif

HI_S32 ADSP_DRV_Open(struct inode* inode, struct file*  filp)
{
    if (atomic_inc_return(&s_stAdspDrv.atmOpenCnt) == 1)
    {}

    return HI_SUCCESS;
}

HI_S32 ADSP_DRV_Release(struct inode* inode, struct file*  filp)
{
    /* Not the last close, only close the channel match with the 'filp' */
    if (atomic_dec_return(&s_stAdspDrv.atmOpenCnt) != 0)
    {}
    /* Last close */
    else
    {}

    return HI_SUCCESS;
}

#if (1 == HI_PROC_SUPPORT)
const HI_U32 ADSP_AOE_RateProc(HI_U32 u32Rate)
{
    HI_U32 g_u32RateTab[] =
    {
        8000,
        11025,
        12000,
        16000,
        22050,
        24000,
        32000,
        44100,
        48000,
        88200,
        96000,
        176400,
        192000,
    };

    if (u32Rate < sizeof(g_u32RateTab) / sizeof(g_u32RateTab[0]))
    {
        return g_u32RateTab[u32Rate];
    }

    return g_u32RateTab[0];
}

const HI_U32  ADSP_AOE_ChProc(HI_U32 ch)
{
    switch (ch)
    {
        case 0x00:
            return 1;
        case 0x01:
            return 2;
        case 0x03:
            return 8;
        default:
            return 0;
    }
}

const HI_U32  ADSP_AOE_WidthProc(HI_U32 width)
{
    switch (width)
    {
        case 0x00:
            return 8;
        case 0x01:
            return 16;
        case 0x02:
            return 24;
        default:
            return 0;
    }
}

const HI_CHAR*   ADSP_AOE_StatusName(HI_U32 u32Cmd, HI_U32 u32Done)
{
    const HI_CHAR* apcName[4] =
    {
        "start",
        "stop",
        "pause",
        "flush",
    };

    if (u32Done == 0 || u32Cmd >= 4)
    {
        return "invalid";
    }

    return apcName[u32Cmd];
}

#ifdef __DPT__
const HI_CHAR* ADSP_AFLT_CmdName(AFLT_CMD_E enCmd)
{
    const HI_CHAR* apcName[AFLT_CMD_BUTT] =
    {
        "Create",
        "Destroy",
        "Start",
        "Stop",
        "SetParam",
        "GetParam",
        "SetConfig",
        "GetConfig",
        "RestoreSetting",
        "StoreSetting",
        "InitInbuf",
        "DeInitInbuf",
        "InitOutbuf",
        "DeInitOutbuf",
    };

    if (enCmd < AFLT_CMD_BUTT)
    {
        return apcName[enCmd];
    }

    return "UnknownCmd";
}

const HI_CHAR* ADSP_AFLT_CompName(AFLT_COMPONENT_ID_E enComp)
{
    const HI_CHAR* apcName[AFLT_BUTT] =
    {
        "SWSEffect",
        "SRS3DEffect",
        "DolbyDV258",
        "DDPDecode",
        "TrueHDDecode",
        "DTSHDDecode",
        "PcmDecode",
        "MP3Decode",
        "MP3XADecode",
        "DTSHDXADecode",
        "DBXEffect"
    };

    if (enComp < AFLT_BUTT)
    {
        return apcName[enComp];
    }

    return "UnknownComp";
}
#endif

const HI_U32 ADSP_BufUsedSizeProc(HI_U32 u32Size, HI_U32 u32ReadPtr, HI_U32 u32WritePtr)
{
    if (u32ReadPtr <= u32WritePtr)
    {
        return u32WritePtr - u32ReadPtr;
    }
    else
    {
        return u32Size - (u32ReadPtr - u32WritePtr);
    }
}

const HI_U32 ADSP_BufUsedPercProc(HI_U32 u32Size, HI_U32 u32UsedSize)
{
    if (0 == u32Size)
    {
        return 0;
    }
    else
    {
        return u32UsedSize * 100 / u32Size;
    }
}

const HI_U32 ADSP_BufLatencyMsProc(HI_U32 u32UsedSize, HI_U32 u32FrameSize, HI_U32 u32Rate)
{
    if (0 == u32FrameSize || 0 == u32Rate)
    {
        return 0;
    }
    else
    {
        return (u32UsedSize / u32FrameSize) * 1000 / u32Rate;
    }
}

const HI_U32 ADSP_FrameSizeProc(HI_U32 u32BitWidth, HI_U32 u32Channel)
{
    if (16 == u32BitWidth)
    {
        return u32Channel * sizeof(HI_S16);
    }
    else
    {
        return u32Channel * sizeof(HI_S32);
    }
}

static HI_U32 ADSP_GetAefInBufDelayMs(S_MIXER_REGS_TYPE* pEneReg)
{
    HI_U32 u32SampleRate = 0;
    HI_U32 u32FrmLatency = 0;
    HI_U32 u32WriteWrap  = pEneReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_wrap;
    HI_U32 u32WriteIdx   = pEneReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_idx;
    HI_U32 u32ReadWrap   = pEneReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_wrap;
    HI_U32 u32ReadIdx    = pEneReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_idx;

    u32FrmLatency = ((u32WriteWrap == u32ReadWrap) ? (u32WriteIdx - u32ReadIdx) : (AOE_AEF_PERIODBUF_NUM - u32ReadIdx + u32WriteIdx));
    u32SampleRate = ADSP_AOE_RateProc(pEneReg->ENGINE_ATTR.bits.fs);

    return (u32FrmLatency * AOE_ENGINE_PROC_SAMPLES * 1000 / u32SampleRate);
}

static HI_U32 ADSP_GetAefOutBufDelayMs(S_MIXER_REGS_TYPE* pEneReg)
{
    HI_U32 u32SampleRate = 0;
    HI_U32 u32FrmLatency = 0;
    HI_U32 u32WriteWrap  = pEneReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_wrap;
    HI_U32 u32WriteIdx   = pEneReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_idx;
    HI_U32 u32ReadWrap   = pEneReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_wrap;
    HI_U32 u32ReadIdx    = pEneReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_idx;

    u32FrmLatency = ((u32WriteWrap == u32ReadWrap) ? (u32WriteIdx - u32ReadIdx) : (AOE_AEF_PERIODBUF_NUM - u32ReadIdx + u32WriteIdx));
    u32SampleRate = ADSP_AOE_RateProc(pEneReg->ENGINE_ATTR.bits.fs);

    return (u32FrmLatency * AOE_ENGINE_PROC_SAMPLES * 1000 / u32SampleRate);
}

static HI_VOID ADSP_AOE_Proc(struct seq_file* p, HI_VOID* v)
{
    HI_U32 i;
    HI_U32 j = 0;
    S_AOE_REGS_TYPE*   pComReg;
    S_MIXER_REGS_TYPE* pEneReg;
    S_AIP_REGS_TYPE*   pAipReg;
    S_AOP_REGS_TYPE*   pAopReg;
    HI_VOID* pRptr = HI_NULL;
    HI_VOID* pWptr = HI_NULL;
    HI_VOID* pTmpAddr  = HI_NULL;
    HI_U32 u32UsedSize = 0, u32FrameSize = 0, u32AipId = AOE_MAX_AIP_NUM, u32AopId = 0;

    S_CAST_REGS_TYPE*   pCastReg;
    HI_U32 tmp = 0;

    HI_U32   u32DupAopId = AOE_MAX_AOP_NUM;

    PROC_PRINT( p, "\n---------------------------------  AOE Status  -------------------------------------------\n");

    pComReg =  (S_AOE_REGS_TYPE*)g_pvAoeRegAddr;
    PROC_PRINT( p,
                "CommonStatus   :ExecuteTimeOutCnt(%d), ScheduleTimeOutCnt(%d)\n",
                pComReg->AOE_STATUS1.bits.excute_timeout_cnt, pComReg->AOE_STATUS1.bits.schedule_timeout_cnt);

    for (i = 0; i < AOE_MAX_ENGINE_NUM; i++)
    {
        pEneReg = (S_MIXER_REGS_TYPE*)(g_pvAoeRegAddr + AOE_ENGINE_REG_OFFSET + i * sizeof(S_MIXER_REGS_TYPE));
        if ((pEneReg->ENGINE_CTRL.bits.cmd == 0 || pEneReg->ENGINE_CTRL.bits.cmd == 2) //when start or pause, proc
            && pEneReg->ENGINE_CTRL.bits.cmd_done == 1)
        {
            PROC_PRINT( p,
                        "ENGINE(%d)   :Status(%s), Format(%s), Rate(%d), Ch(%d), Width(%d), AefMask(0x%x)",
                        i,
                        ADSP_AOE_StatusName(pEneReg->ENGINE_CTRL.bits.cmd, pEneReg->ENGINE_CTRL.bits.cmd_done),
                        pEneReg->ENGINE_ATTR.bits.format == 0 ? "pcm" : "iec61937",
                        ADSP_AOE_RateProc(pEneReg->ENGINE_ATTR.bits.fs),
                        ADSP_AOE_ChProc(pEneReg->ENGINE_ATTR.bits.ch),
                        ADSP_AOE_WidthProc(pEneReg->ENGINE_ATTR.bits.precision),
                        pEneReg->ENGINE_ATT_AEF.bits.aef_att_ena);

            if (pEneReg->ENGINE_ATT_AEF.bits.aef_att_ena & 0xf0)
            {
                PROC_PRINT( p, ", FifoDelayNum(%d)\n", pEneReg->ENGINE_ATTR.bits.aef_delay);
            }
            else
            {
                PROC_PRINT( p, "\n");
            }

            if (pEneReg->ENGINE_ATT_AEF.bits.aef_att_ena & 0xf0)
            {
                ADSP_ReadAddr(pEneReg->ENGINE_AEF_INBUF_ADDR, pTmpAddr);
                PROC_PRINT( p,
                            "            :AefInbufAddr(0x%p), AefInbufPeriodSize(0x%x), AefInbufPeriodNum(0x%x), AefInbufRidx/Widx/Latency(0x%x(%d)/0x%x(%d)/%dms)\n",
                            pTmpAddr,
                            pEneReg->ENGINE_AEF_INBUF_SIZE.bits.periond_size,
                            pEneReg->ENGINE_AEF_INBUF_SIZE.bits.periond_num,
                            pEneReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_idx,
                            pEneReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_wrap,
                            pEneReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_idx,
                            pEneReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_wrap,
                            ADSP_GetAefInBufDelayMs(pEneReg));

                ADSP_ReadAddr(pEneReg->ENGINE_AEF_OUTBUF_ADDR, pTmpAddr);
                PROC_PRINT( p,
                            "            :AefOutbufAddr(0x%p), AefOutbufPeriodSize(0x%x), AefOutbufPeriodNum(0x%x), AefOutbufRidx/Widx/Latency(0x%x(%d)/0x%x(%d)/%dms)\n",
                            pTmpAddr,
                            pEneReg->ENGINE_AEF_OUTBUF_SIZE.bits.periond_size,
                            pEneReg->ENGINE_AEF_OUTBUF_SIZE.bits.periond_num,
                            pEneReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_idx,
                            pEneReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_wrap,
                            pEneReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_idx,
                            pEneReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_wrap,
                            ADSP_GetAefOutBufDelayMs(pEneReg));
            }

            u32DupAopId = AOE_MAX_AOP_NUM;
            for (j = 0; j < AOE_MAX_AIP_NUM; j++)
            {
                if ((pEneReg->ENGINE_MIX_SRC.bits.aip_fifo_ena) & (1L << j))
                {
                    pAipReg = (S_AIP_REGS_TYPE*)(g_pvAoeRegAddr + AOE_AIP_REG_OFFSET + j * sizeof(S_AIP_REGS_TYPE));
                    PROC_PRINT( p,
                                "Aip(%d)      :Status(%s), Mute(%s), Vol(%ddB/%ddB), Prescale(%s%d.%ddB), ChMode(%d), SpeedAdjCnt(%d)\n",
                                j,
                                ADSP_AOE_StatusName(pAipReg->AIP_CTRL.bits.cmd, pAipReg->AIP_CTRL.bits.cmd_done),
                                (0 == pAipReg->AIP_EXT_CTRL.bits.mute) ? "Off" : "On",
                                (HI_S32)(pAipReg->AIP_EXT_CTRL.bits.volume_l) - 0x79,
                                (HI_S32)(pAipReg->AIP_EXT_CTRL.bits.volume_r) - 0x79,
                                (0x79 == pAipReg->AIP_EXT2_CTRL.bits.prescale_int && 1 == pAipReg->AIP_EXT2_CTRL.bits.prescale_dec_dir) ? "-" : "",
                                (HI_S32)(pAipReg->AIP_EXT2_CTRL.bits.prescale_int) - 0x79,
                                pAipReg->AIP_EXT2_CTRL.bits.prescale_dec * 125,
                                pAipReg->AIP_EXT_CTRL.bits.channel_mode,
                                pAipReg->AIP_FSADJ_CNT);

                    if (pAipReg->AIP_FIFO_ATTR.bits.fifo_bypass == 1)
                    {
                        u32AipId = j;
                    }

                    if (1 == pAipReg->AIP_BUF_SIZE.bits.buff_flag)
                    {
#ifdef HI_SND_DSP_HW_SUPPORT
                        pRptr = ioremap_nocache(pAipReg->AIP_BUF_RPTR.ADDR_LOW, sizeof(HI_U32));
                        pWptr = ioremap_nocache(pAipReg->AIP_BUF_WPTR.ADDR_LOW, sizeof(HI_U32));
#else
                        //AIP 目前仅支持间接寻址，即:pAipReg->AIP_BUF_SIZE.bits.buff_flag = 0
                        ADSP_ReadAddr(pAipReg->AIP_BUF_WPTR, pRptr);
                        ADSP_ReadAddr(pAipReg->AIP_BUF_RPTR, pWptr);
#endif
                    }
                    else
                    {
                        pRptr = (HI_VOID*)(&pAipReg->AIP_BUF_RPTR.ADDR_LOW);
                        pWptr = (HI_VOID*)(&pAipReg->AIP_BUF_WPTR.ADDR_LOW);
                    }
                    u32UsedSize  = ADSP_BufUsedSizeProc(pAipReg->AIP_BUF_SIZE.bits.buff_size, *((HI_U32*)pRptr), *((HI_U32*)pWptr));
                    u32FrameSize = ADSP_FrameSizeProc(ADSP_AOE_WidthProc(pAipReg->AIP_BUFF_ATTR.bits.aip_precision), ADSP_AOE_ChProc(pAipReg->AIP_BUFF_ATTR.bits.aip_ch));
                    ADSP_ReadAddr(pAipReg->AIP_BUF_ADDR, pTmpAddr);
                    PROC_PRINT( p,
                                "            :Buff Format(%s), Rate(%.6d), Ch(%d), Width(%d), Addr(0x%p), Size/Used/Latency(0x%.6x/%d%%/%dms), Rptr/Wptr(0x%.6x/0x%.6x)\n",
                                (0 == pAipReg->AIP_BUFF_ATTR.bits.aip_format) ? "pcm" : "iec61937",
                                ADSP_AOE_RateProc(pAipReg->AIP_BUFF_ATTR.bits.aip_fs),
                                ADSP_AOE_ChProc(pAipReg->AIP_BUFF_ATTR.bits.aip_ch),
                                ADSP_AOE_WidthProc(pAipReg->AIP_BUFF_ATTR.bits.aip_precision),
                                pTmpAddr,
                                pAipReg->AIP_BUF_SIZE.bits.buff_size,
                                ADSP_BufUsedPercProc(pAipReg->AIP_BUF_SIZE.bits.buff_size, u32UsedSize),
                                ADSP_BufLatencyMsProc(u32UsedSize, u32FrameSize, ADSP_AOE_RateProc(pAipReg->AIP_BUFF_ATTR.bits.aip_fs)),
                                *((HI_U32*)pRptr),
                                *((HI_U32*)pWptr));
                    if (pEneReg->ENGINE_ATTR.bits.format == 0)  // LPCM
                    {
                        u32UsedSize = ADSP_BufUsedSizeProc(pAipReg->AIP_FIFO_SIZE.bits.aip_fifo_size, pAipReg->AIP_FIFO_RPTR, pAipReg->AIP_FIFO_WPTR);
                        u32FrameSize = ADSP_FrameSizeProc(ADSP_AOE_WidthProc(pAipReg->AIP_FIFO_ATTR.bits.fifo_precision), ADSP_AOE_ChProc(pAipReg->AIP_FIFO_ATTR.bits.fifo_ch));
                        PROC_PRINT( p,
                                    "            :Fifo Format(%s), Rate(%.6d), Ch(%d), Width(%d), Addr(0x%.8x), Size/Used/Latency/Underflow(0x%.4x/%d%%/%dms/%d), Rptr/Wptr(0x%.4x/0x%.4x)\n",
                                    (0 == pAipReg->AIP_FIFO_ATTR.bits.fifo_format) ? "pcm" : "iec61937",
                                    ADSP_AOE_RateProc(pAipReg->AIP_FIFO_ATTR.bits.fifo_fs),
                                    ADSP_AOE_ChProc(pAipReg->AIP_FIFO_ATTR.bits.fifo_ch),
                                    ADSP_AOE_WidthProc(pAipReg->AIP_FIFO_ATTR.bits.fifo_precision),
                                    pAipReg->AIP_FIFO_ADDR,
                                    pAipReg->AIP_FIFO_SIZE.bits.aip_fifo_size,
                                    ADSP_BufUsedPercProc(pAipReg->AIP_FIFO_SIZE.bits.aip_fifo_size, u32UsedSize),
                                    ADSP_BufLatencyMsProc(u32UsedSize, u32FrameSize, ADSP_AOE_RateProc(pAipReg->AIP_FIFO_ATTR.bits.fifo_fs)),
                                    pAipReg->AIP_FIFO_SIZE.bits.aip_fifo_underflow_cnt,
                                    pAipReg->AIP_FIFO_RPTR,
                                    pAipReg->AIP_FIFO_WPTR);
                    }

                    if(pAipReg->AIP_EXT2_CTRL.bits.is_duplicate)
                    {
                        u32DupAopId = pAipReg->AIP_EXT2_CTRL.bits.dest_aop;
                        PROC_PRINT( p, "             DuplicateAop(%d)\n", u32DupAopId);
                    }
                }
            }
            for (j = 0; j < AOE_MAX_AOP_NUM; j++)
            {
                if (((pEneReg->ENGINE_ROU_DST.bits.aop_buf_ena) & (1L << j)) || ((1L << u32DupAopId) & (1L << j)))
                {
                    pAopReg = (S_AOP_REGS_TYPE*)(g_pvAoeRegAddr + AOE_AOP_REG_OFFSET + j * sizeof(S_AOP_REGS_TYPE));
                    if (pAopReg->AOP_CTRL.bits.cast_flag == 1) // aop is cast
                    {
                        PROC_PRINT( p,
                                    "Aop(%d,Cast) :Status(%s), AddMute(%s), AefBypass(%s), Vol(%ddB/%ddB), Mute(%s), Latency(%dms), DelayComps(%dms)\n",
                                    j,
                                    ADSP_AOE_StatusName(pAopReg->AOP_CTRL.bits.cmd, pAopReg->AOP_CTRL.bits.cmd_done),
                                    (pAopReg->AOP_CTRL.bits.ena_add_mute == 1) ? "on" : "off",
                                    (0 == pAopReg->AOP_CTRL.bits.ena_aef_bypass) ? "Off" : "On",
                                    (HI_S32)(pAopReg->AOP_EXT_CTRL.bits.volume_l) - 0x79,
                                    (HI_S32)(pAopReg->AOP_EXT_CTRL.bits.volume_l) - 0x79,
                                    (0 == pAopReg->AOP_EXT_CTRL.bits.mute) ? "Off" : "On",
                                    pAopReg->AOP_BUFF_ATTR.bits.buf_latency,
                                    pAopReg->AOP_CTRL.bits.realdelayms);
                    }
                    else
                    {
                        PROC_PRINT( p,
                                    "Aop(%d)      :Status(%s), AefBypass(%s), Vol(%s%d.%ddB/%s%d.%ddB), Balance(%d), mute(%s) , Latency(%dms), AD(%s), DelayComps(%dms)\n",
                                    j,
                                    ADSP_AOE_StatusName(pAopReg->AOP_CTRL.bits.cmd, pAopReg->AOP_CTRL.bits.cmd_done),
                                    (0 == pAopReg->AOP_CTRL.bits.ena_aef_bypass) ? "Off" : "On",
                                    (0x79 == pAopReg->AOP_EXT_CTRL.bits.volume_l && 1 == pAopReg->AOP_EXT_CTRL.bits.volumedec_dir_l) ? "-" : "",
                                    (HI_S32)(pAopReg->AOP_EXT_CTRL.bits.volume_l) - 0x79,
                                    pAopReg->AOP_EXT_CTRL.bits.volumedec_l * 125,
                                    (0x79 == pAopReg->AOP_EXT_CTRL.bits.volume_r && 1 == pAopReg->AOP_EXT_CTRL.bits.volumedec_dir_r) ? "-" : "",
                                    (HI_S32)(pAopReg->AOP_EXT_CTRL.bits.volume_r) - 0x79,
                                    pAopReg->AOP_EXT_CTRL.bits.volumedec_r * 125,
                                    (0 == pAopReg->AOP_EXT_CTRL.bits.balance_dir) ? pAopReg->AOP_EXT_CTRL.bits.balance_val : -pAopReg->AOP_EXT_CTRL.bits.balance_val,
                                    (0 == pAopReg->AOP_EXT_CTRL.bits.mute) ? "Off" : "On",
                                    pAopReg->AOP_BUFF_ATTR.bits.buf_latency,
                                    (0 == pAopReg->AOP_CTRL.bits.ena_ad) ? "Off" : "On",
                                    pAopReg->AOP_CTRL.bits.realdelayms);
                    }

                    if (1 == pAopReg->AOP_BUF_SIZE.bits.buff_flag)
                    {
#ifdef HI_SND_DSP_HW_SUPPORT
                        pRptr = ioremap_nocache(pAopReg->AOP_BUF_RPTR.ADDR_LOW, sizeof(HI_U32));
                        pWptr = ioremap_nocache(pAopReg->AOP_BUF_WPTR.ADDR_LOW, sizeof(HI_U32));
#else
                        ADSP_ReadAddr(pAopReg->AOP_BUF_RPTR, pRptr);
                        ADSP_ReadAddr(pAopReg->AOP_BUF_WPTR, pWptr);
#endif
                    }
                    else  //Aop is Cast
                    {
                        pRptr = (HI_VOID*)(&pAopReg->AOP_BUF_RPTR);
                        pWptr = (HI_VOID*)(&pAopReg->AOP_BUF_WPTR);
                    }

                    u32UsedSize = ADSP_BufUsedSizeProc(pAopReg->AOP_BUF_SIZE.bits.buff_size, *((HI_U32*)pRptr), *((HI_U32*)pWptr));
                    u32FrameSize = ADSP_FrameSizeProc(ADSP_AOE_WidthProc(pAopReg->AOP_BUFF_ATTR.bits.buf_precision), ADSP_AOE_ChProc(pAopReg->AOP_BUFF_ATTR.bits.buf_ch));
                    ADSP_ReadAddr(pAopReg->AOP_BUF_ADDR, pTmpAddr);
                    PROC_PRINT( p,
                                "            :Buff Format(%s), Rate(%.6d), Ch(%d), Width(%d), Addr(0x%p), Size/Used/Latency(0x%.4x/%d%%/%dms), Rptr/Wptr(0x%.4x/0x%.4x)\n",
                                pAopReg->AOP_BUFF_ATTR.bits.buf_format == 0 ? "pcm" : "iec61937",
                                ADSP_AOE_RateProc(pAopReg->AOP_BUFF_ATTR.bits.buf_fs),
                                ADSP_AOE_ChProc(pAopReg->AOP_BUFF_ATTR.bits.buf_ch),
                                ADSP_AOE_WidthProc(pAopReg->AOP_BUFF_ATTR.bits.buf_precision),
                                pTmpAddr,
                                pAopReg->AOP_BUF_SIZE.bits.buff_size,
                                ADSP_BufUsedPercProc(pAopReg->AOP_BUF_SIZE.bits.buff_size, u32UsedSize),
                                ADSP_BufLatencyMsProc(u32UsedSize, u32FrameSize, ADSP_AOE_RateProc(pAopReg->AOP_BUFF_ATTR.bits.buf_fs)),
                                *((HI_U32*)pRptr),
                                *((HI_U32*)pWptr)
                              );
                    if (pAopReg->AOP_CTRL.bits.audiodelay)
                    {
                        PROC_PRINT( p,
                                    "            :AddMuteCnt(%.6u)\n", pAopReg->AOP_ADDMUTE_CNT);
                    }
                    if (pAopReg->AOP_CTRL.bits.cast_flag == 1) //Aop is Cast
                    {
                        for (tmp = 0; tmp < AOE_MAX_CAST_NUM; tmp++)
                        {
                            pCastReg = (S_CAST_REGS_TYPE*)(g_pvAoeRegAddr + AOE_CAST_REG_OFFSET + tmp * sizeof(S_CAST_REGS_TYPE));
                            if (pCastReg->CAST_AOP_ID == j)
                            {
                                PROC_PRINT( p,
                                            "            :BuffWritCnt(Total/Nomal/AddMute)(%.6u/%.6u/%.6u), BuffOverWriteCnt(%.6u)\n",
                                            pCastReg->CAST_BUF_WRITETOTAL_CNT,
                                            pCastReg->CAST_BUF_WRITEOK_CNT,
                                            pCastReg->CAST_BUF_WRITEMUTE_CNT,
                                            pCastReg->CAST_BUF_OVERWRITE_CNT);

                                u32UsedSize = ADSP_BufUsedSizeProc(pCastReg->CAST_FIFO_SIZE, pCastReg->CAST_FIFO_RPTR, pCastReg->CAST_FIFO_WPTR);
                                PROC_PRINT( p,
                                            "            :FIFO Size/Used/Latency(0x%.4x/%d%%/%dms), Rptr/Wptr(0x%.4x/0x%.4x), FIFOWrite(Try/OK)(%.6u/%.6u), ForceFIFOReadCnt(%.6u)\n",
                                            pCastReg->CAST_FIFO_SIZE,
                                            ADSP_BufUsedPercProc(pCastReg->CAST_FIFO_SIZE, u32UsedSize),
                                            ADSP_BufLatencyMsProc(u32UsedSize, u32FrameSize, ADSP_AOE_RateProc(pAopReg->AOP_BUFF_ATTR.bits.buf_fs)),
                                            pCastReg->CAST_FIFO_RPTR,
                                            pCastReg->CAST_FIFO_WPTR,
                                            pCastReg->CAST_FIFO_TRY_CNT,
                                            pCastReg->CAST_FIFO_OK_CNT,
                                            pCastReg->CAST_FIFO_FORCE_READ_CNT);
                            }

                        }
                    }
                }
            }
            PROC_PRINT( p, "\n");
        }
    }
    if (u32AipId != AOE_MAX_AIP_NUM)
    {
        pAipReg = (S_AIP_REGS_TYPE*)(g_pvAoeRegAddr + AOE_AIP_REG_OFFSET + u32AipId * sizeof(S_AIP_REGS_TYPE));
        if (1 == pAipReg->AIP_BUF_SIZE.bits.buff_flag)
        {
#ifdef HI_SND_DSP_HW_SUPPORT
            pRptr = ioremap_nocache(pAipReg->AIP_BUF_RPTR.ADDR_LOW, sizeof(HI_U32)); //TODO unmap
            pWptr = ioremap_nocache(pAipReg->AIP_BUF_WPTR.ADDR_LOW, sizeof(HI_U32));

#else
            //AIP 目前仅支持间接寻址，即:pAipReg->AIP_BUF_SIZE.bits.buff_flag = 0
            ADSP_ReadAddr(pAipReg->AIP_BUF_RPTR, pRptr);
            ADSP_ReadAddr(pAipReg->AIP_BUF_WPTR, pWptr);
#endif
        }
        else
        {
            pRptr = (HI_VOID*)(&pAipReg->AIP_BUF_RPTR.ADDR_LOW);
            pWptr = (HI_VOID*)(&pAipReg->AIP_BUF_WPTR.ADDR_LOW);
        }
        u32UsedSize = ADSP_BufUsedSizeProc(pAipReg->AIP_BUF_SIZE.bits.buff_size, *(HI_U32*)pRptr, *(HI_U32*)pWptr);
        u32FrameSize = ADSP_FrameSizeProc(ADSP_AOE_WidthProc(pAipReg->AIP_BUFF_ATTR.bits.aip_precision), ADSP_AOE_ChProc(pAipReg->AIP_BUFF_ATTR.bits.aip_ch));
        PROC_PRINT( p,
                    "Karaoke     :Aip(%d), Size/Used/Latency(0x%.6x/%d%%/%dms)\n",
                    u32AipId,
                    pAipReg->AIP_BUF_SIZE.bits.buff_size,
                    ADSP_BufUsedPercProc(pAipReg->AIP_BUF_SIZE.bits.buff_size, u32UsedSize),
                    ADSP_BufLatencyMsProc(u32UsedSize, u32FrameSize, ADSP_AOE_RateProc(pAipReg->AIP_BUFF_ATTR.bits.aip_fs))
                  );
        pAopReg = (S_AOP_REGS_TYPE*)(g_pvAoeRegAddr + AOE_AOP_REG_OFFSET + u32AopId * sizeof(S_AOP_REGS_TYPE));
        if (1 == pAopReg->AOP_BUF_SIZE.bits.buff_flag)
        {
#ifdef HI_SND_DSP_HW_SUPPORT
            pRptr = ioremap_nocache(pAopReg->AOP_BUF_RPTR.ADDR_LOW, sizeof(HI_U32)); //TODO unmap
            pWptr = ioremap_nocache(pAopReg->AOP_BUF_WPTR.ADDR_LOW, sizeof(HI_U32));
#else
            ADSP_ReadAddr(pAopReg->AOP_BUF_RPTR, pRptr);
            ADSP_ReadAddr(pAopReg->AOP_BUF_WPTR, pWptr);
#endif
        }
        else  //Aop is Cast
        {
            pRptr = (HI_VOID*)(&pAopReg->AOP_BUF_RPTR);
            pWptr = (HI_VOID*)(&pAopReg->AOP_BUF_WPTR);
        }
        u32UsedSize = ADSP_BufUsedSizeProc(pAopReg->AOP_BUF_SIZE.bits.buff_size, *(HI_U32*)pRptr, *(HI_U32*)pWptr);
        u32FrameSize = ADSP_FrameSizeProc(ADSP_AOE_WidthProc(pAopReg->AOP_BUFF_ATTR.bits.buf_precision), ADSP_AOE_ChProc(pAopReg->AOP_BUFF_ATTR.bits.buf_ch));
        PROC_PRINT( p,
                    "            :Aop(%d), Size/Used/Latency(0x%.4x/%d%%/%dms)\n",
                    u32AopId,
                    pAopReg->AOP_BUF_SIZE.bits.buff_size,
                    ADSP_BufUsedPercProc(pAopReg->AOP_BUF_SIZE.bits.buff_size, u32UsedSize),
                    ADSP_BufLatencyMsProc(u32UsedSize, u32FrameSize, ADSP_AOE_RateProc(pAopReg->AOP_BUFF_ATTR.bits.buf_fs))
                  );
    }

    return;
}

#ifdef __DPT__
static HI_VOID ADSP_AFLT_Proc(struct seq_file* p, HI_VOID* v)
{
    HI_U32 i;
    S_AFLT_REGS_TYPE* pAfltReg;
    S_AFLT_COM_ATTR*  pAfltComReg;
    HI_VOID* pTmpAddr;

    PROC_PRINT( p, "\n---------------------------------  AFLT Status  ------------------------------------------\n");

    pAfltComReg = (S_AFLT_COM_ATTR*)g_pvAfltRegAddr;

    PROC_PRINT( p,
                "ComStatus  :ExecuteTimeOutCnt(%d)\n",
                pAfltComReg->AFLT_STATUS0.bits.timeout_cnt);

    PROC_PRINT( p, "\n");

    for (i = 0; i < AFLT_MAX_CHAN_NUM; i++)
    {
        pAfltReg = (S_AFLT_REGS_TYPE*)(g_pvAfltRegAddr + AFLT_CHN_REG_OFFSET + i * sizeof(S_AFLT_REGS_TYPE));
        if (pAfltReg->ATTR.bits.use_flag) //aflt be created
        {
            ADSP_ReadAddr(pAfltReg->MSGPOOL_ADDR, pTmpAddr);
            PROC_PRINT( p,
                        "AFLT(%d) :Type(%s), CompName(%s), Status(%s), MsgPoolAddr(0x%p), MsgPoolSize(0x%x)\n",
                        i,
                        (pAfltReg->ATTR.bits.master_flag == 0) ? "slave" : "master",
                        ADSP_AFLT_CompName(pAfltReg->ATTR.bits.filter_id),
                        (pAfltReg->ATTR.bits.status == 0) ? "stop" : "start",
                        pTmpAddr,
                        pAfltReg->ATTR.bits.msgpool_size);

            PROC_PRINT( p,
                        "        :SettingIndex(0x%x), SettingSize(0x%x), CmdType(%s), CmdStatus(%s), CmdRetVal(%d)\n",
                        pAfltReg->CTRL.bits.param_index,
                        pAfltReg->CTRL.bits.param_wordsize,
                        ADSP_AFLT_CmdName((AFLT_CMD_E)pAfltReg->CTRL.bits.cmd),
                        (pAfltReg->CTRL.bits.cmd_done == 1) ? "done" : "no done",
                        pAfltReg->CTRL.bits.cmd_return_value);
            if (pAfltReg->ATTR.bits.master_flag == 1)
            {
                ADSP_ReadAddr(pAfltReg->IP_BUF_ADDR, pTmpAddr);
                PROC_PRINT( p,
                            "        :PtsBoundary(0x%x), InbufAddr(0x%p), InbufSize/Used(0x%x/%d%%), InbufRptr/Wptr(0x%x/0x%x), StreamReadPos(0x%x)\n",
                            pAfltReg->IPBUF_BOUNDARY,
                            pTmpAddr,
                            pAfltReg->IP_BUF_SIZE.bits.inbuff_size,
                            ADSP_BufUsedPercProc(pAfltReg->IP_BUF_SIZE.bits.inbuff_size, ADSP_BufUsedSizeProc(pAfltReg->IP_BUF_SIZE.bits.inbuff_size, pAfltReg->IP_BUF_RPTR, pAfltReg->IP_BUF_WPTR)),
                            pAfltReg->IP_BUF_RPTR,
                            pAfltReg->IP_BUF_WPTR,
                            pAfltReg->READPOS);
                PROC_PRINT( p,
                            "        :InfifoAddr(0x%x), InfifoSize(0x%x), InfifoRptr(0x%x), InfifoBytesLeft(0x%x)\n",
                            pAfltReg->IP_FIFO_ADDR,
                            pAfltReg->IP_FIFO_SIZE,
                            pAfltReg->IP_FIFO_RPTR,
                            pAfltReg->IP_FIFO_BYTES);
                ADSP_ReadAddr(pAfltReg->OP_BUF_ADDR, pTmpAddr);
                PROC_PRINT( p,
                            "        :OutbufAddr(0x%p), OutbufPeriodSize(0x%x), OutbufPeriodNum(0x%x), OutbufRidx/Widx(0x%x(%d)/0x%x(%d))\n",
                            pTmpAddr,
                            pAfltReg->OP_BUF_SIZE.bits.periond_size,
                            pAfltReg->OP_BUF_SIZE.bits.periond_num,
                            pAfltReg->OP_BUF_RIDX.bits.periond_read_idx,
                            pAfltReg->OP_BUF_RIDX.bits.periond_read_wrap,
                            pAfltReg->OP_BUF_WIDX.bits.periond_write_idx,
                            pAfltReg->OP_BUF_WIDX.bits.periond_write_wrap);
            }
            PROC_PRINT( p,
                        "        :TryCnt(%d), FrameCnt(%d), ErrFrameCnt(%d), ExcuteTimeOutCnt(%d), ScheduleTimeOutCnt(%d)\n",
                        pAfltReg->TRY_EXE_CNT,
                        pAfltReg->STATUS0.bits.frame_cnt,
                        pAfltReg->STATUS0.bits.frame_err_cnt,
                        pAfltReg->TIMEOUT.bits.exe_timeout_cnt,
                        pAfltReg->TIMEOUT.bits.sche_timeout_cnt);
            PROC_PRINT( p, "\n");
        }
    }

    return;
}
#endif

static HI_VOID ADSP_ProcDeInit(HI_VOID)
{
    if (g_pvAoeRegAddr)
    {
        iounmap((HI_VOID*)g_pvAoeRegAddr);
        g_pvAoeRegAddr = HI_NULL;
    }

#ifdef __DPT__
    if (g_pvAfltRegAddr)
    {
        iounmap((HI_VOID*)g_pvAfltRegAddr);
        g_pvAfltRegAddr = HI_NULL;
    }
#endif
}

static HI_S32 ADSP_ProcInit(HI_VOID)
{
#ifdef __aarch64__
    g_pvAoeRegAddr = ioremap_wc(AOE_COM_REG_BASE, AOE_REG_LENGTH);
#else
    g_pvAoeRegAddr = ioremap_nocache(AOE_COM_REG_BASE, AOE_REG_LENGTH);
#endif
    if (HI_NULL == g_pvAoeRegAddr)
    {
        HI_PRINT("AOE_COM_REG_BASE ioremap error!\n");
        return HI_FAILURE;
    }
    memset(g_pvAoeRegAddr, 0, AOE_REG_LENGTH);

#ifdef __DPT__
#ifdef __aarch64__
    g_pvAfltRegAddr = ioremap_wc(AFLT_COM_REG_BASE, AFLT_REG_LENGTH);
#else
    g_pvAfltRegAddr = ioremap_nocache(AFLT_COM_REG_BASE, AFLT_REG_LENGTH);
#endif
    if (HI_NULL == g_pvAfltRegAddr)
    {
        HI_PRINT("AFLT_COM_REG_BASE ioremap error!\n");
        ADSP_ProcDeInit();
        return HI_FAILURE;
    }
    memset(g_pvAfltRegAddr, 0, AFLT_REG_LENGTH);
#endif

    return HI_SUCCESS;
}
#endif

#ifdef HI_SND_DSP_PROC_LOG_SUPPORT

static DEFINE_MUTEX(adsp_ram_lock);
static struct ADSP_Ram_Zone_S s_stRamZone;

/* convert physical address to logic address */
static HI_S32 ADSP_Proc_Log_Map(phys_addr_t start, phys_addr_t size,
                                struct ADSP_Ram_Zone_S* prz)
{
    prz->pVaddr = HI_DRV_Mmap_Kern(start, size);
    prz->pstBuffer = (struct ADSP_Ram_Buffer_S*)prz->pVaddr;

    return 0;
}

#include <linux/timer.h>

/* Close the log. */
#define DSPLOG_ACTION_CLOSE          0
/* Open the log. */
#define DSPLOG_ACTION_OPEN           1
/* Read from the log. */
#define DSPLOG_ACTION_READ           2

static DEFINE_SPINLOCK(adsp_log_lock);
static DECLARE_WAIT_QUEUE_HEAD(dsp_log_wait);
static atomic_t open_count = ATOMIC_INIT(0);
static atomic_t need_wait = ATOMIC_INIT(0);
static struct timer_list adsp_timer;
static volatile HI_U32 read_addr = 0;

static void adsp_timer_fn(unsigned long arg)
{
    wake_up_interruptible(&dsp_log_wait);
    mod_timer(&adsp_timer, jiffies + msecs_to_jiffies(10));
}

static int do_dsplog(int type, char __user* buf, int len)
{
    unsigned i;
    char c;
    int error = 0;

    struct ADSP_Ram_Buffer_S* pstRamBuffer = (struct ADSP_Ram_Buffer_S*)s_stRamZone.pstBuffer;

    switch (type)
    {
        case DSPLOG_ACTION_CLOSE:    /* Close log */
            del_timer(&adsp_timer);
            atomic_dec(&open_count);
            break;
        case DSPLOG_ACTION_OPEN:    /* Open log */
            if (atomic_read(&open_count) > 0)
            {
                error = -EBUSY;
                goto out;
            }
            HI_ERR_ADSP("@@> 0x%x\n", pstRamBuffer->u32Sig);
            atomic_inc(&open_count);

            if (pstRamBuffer->u32BufferSize <= pstRamBuffer->u32Size)   /* log buffer not full */
            {
                read_addr = 0;
            }
            else
            {
                read_addr = pstRamBuffer->u32Start +
                            ((LOG_SIZE - HOLE_SIZE - sizeof(struct ADSP_Ram_Buffer_S)) >> 2);
                if (read_addr >= LOG_SIZE - HOLE_SIZE - sizeof(struct ADSP_Ram_Buffer_S))
                {
                    read_addr = 0;
                }
            }

            init_timer(&adsp_timer);
            adsp_timer.expires = jiffies + msecs_to_jiffies(10);
            adsp_timer.function = adsp_timer_fn;
            add_timer(&adsp_timer);
            break;
        case DSPLOG_ACTION_READ:    /* Read from log */
            error = -EINVAL;
            if (!buf || len < 0)
            {
                goto out;
            }
            error = 0;
            if (!len)
            {
                goto out;
            }
            if (!access_ok(VERIFY_WRITE, buf, len))
            {
                error = -EFAULT;
                goto out;
            }

            if (pstRamBuffer->u32BufferSize == 0)
            {
                goto out;
            }

            error = wait_event_interruptible(dsp_log_wait,
                                             (read_addr != pstRamBuffer->u32Start));

            if (error < 0)
            {
                error = 0;
                goto out;
            }

            if (atomic_read(&open_count) == 1)
            {
                atomic_set(&need_wait, 0);
                msleep(50);
            }

            i = 0;
            spin_lock(&adsp_log_lock);
            while (!error && i < len)
            {
                barrier();
                c = *(volatile HI_CHAR*)((HI_VIRT_ADDR_T)s_stRamZone.pstBuffer +
                                         (HI_VIRT_ADDR_T)sizeof(struct ADSP_Ram_Buffer_S) + (HI_VIRT_ADDR_T    )read_addr);

                spin_unlock(&adsp_log_lock);
                error = __put_user(c, buf);
                buf++;
                i++;

                barrier();
                if (read_addr == (pstRamBuffer->u32Start - 1))
                {
                    spin_lock(&adsp_log_lock);
                    atomic_set(&need_wait, 1);
                    read_addr++;
                    if (read_addr >= LOG_SIZE - HOLE_SIZE - sizeof(struct ADSP_Ram_Buffer_S))
                    {
                        read_addr = 0;
                    }
                    break;
                }

                read_addr++;
                if (read_addr >= LOG_SIZE - HOLE_SIZE - sizeof(struct ADSP_Ram_Buffer_S))
                {
                    read_addr = 0;
                }

                cond_resched();
                spin_lock(&adsp_log_lock);
            }
            spin_unlock(&adsp_log_lock);

            if (!error)
            {
                error = i;
            }

            break;
        default:
            error = -EINVAL;
            break;
    }
out:
    return error;
}

static int dspmsg_open(struct inode* inode, struct file* file)
{
    return do_dsplog(DSPLOG_ACTION_OPEN, NULL, 0);
}

static int dspmsg_release(struct inode* inode, struct file* file)
{
    (void) do_dsplog(DSPLOG_ACTION_CLOSE, NULL, 0);
    return 0;
}

static ssize_t dspmsg_read(struct file* file, char __user* buf,
                           size_t count, loff_t* ppos)
{
    return do_dsplog(DSPLOG_ACTION_READ, buf, count);
}

static const struct file_operations proc_dspmsg_operations =
{
    .read       = dspmsg_read,
    .open       = dspmsg_open,
    .release    = dspmsg_release,
    .llseek     = generic_file_llseek,
};

static HI_S32 ADSP_Proc_Log_Init(void)
{
    if (ADSP_Proc_Log_Map(LOG_ADDR, LOG_SIZE, &s_stRamZone))
    {
        HI_FATAL_ADSP("failed to map dsp ram buffer\n");
        return -ENOMEM;
    }

    proc_create("dsp_log", S_IRUSR, NULL, &proc_dspmsg_operations);

    return 0;
}

static HI_S32 ADSP_Proc_Log_DeInit(void)
{
    remove_proc_entry("dsp_log", NULL);
#if 0
    vunmap(s_stRamZone.pVaddr);
#endif
    HI_DRV_Unmap_Kern(LOG_ADDR, s_stRamZone.pVaddr);

    return 0;
}
#endif

#if (1 == HI_PROC_SUPPORT)

#ifdef HI_SND_DSP_HW_SUPPORT
static HI_VOID ADSP_COM_Proc(struct seq_file* p, HI_VOID* v)
{
    HI_U32 j;

    PROC_PRINT( p, "\n---------------------------------  Common Status  ------------------------------------------\n");
    PROC_PRINT( p, "DebugValue     :");
    for (j = 0; j < DSP_DEBUG_REG_NUM; j++)
    {
        PROC_PRINT( p,
                    "Debug%d(0x%x/%d)",
                    j,
                    g_pADSPSysCrg->ADSP_DEBUG[j],
                    g_pADSPSysCrg->ADSP_DEBUG[j]);
        if ((j + 1) % 4) //a line 4 reg
        {
            PROC_PRINT( p, ", ");
        }
        else
        {
            PROC_PRINT( p, "\n                ");
        }
    }
    PROC_PRINT( p, "\n");
    return;
}
#endif

HI_S32 ADSP_DRV_ReadProc(struct seq_file* p, HI_VOID* v)
{
    if (s_stAdspDrv.bReady == HI_FALSE)
    {
        PROC_PRINT( p, "\n-------------  ADSP NOT OPEN  -------------\n");
        return HI_SUCCESS;
    }

    ADSP_AOE_Proc(p, v);
#ifdef __DPT__
    ADSP_AFLT_Proc(p, v);
#endif
#ifdef HI_SND_DSP_HW_SUPPORT
    ADSP_COM_Proc(p, v);
#endif
    return HI_SUCCESS;
}

HI_S32 ADSP_DRV_WriteProc(struct file* file,
                          const char __user* buf, size_t count, loff_t* ppos)
{
    return count;
}

HI_S32 ADSP_DRV_RegisterProc(ADSP_REGISTER_PARAM_S* pstParam)
{
    HI_CHAR aszBuf[16];
    DRV_PROC_ITEM_S*  pProcItem;

    /* Check parameters */
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    s_stAdspDrv.pstProcParam = pstParam;

    /* Create proc */
    snprintf(aszBuf, sizeof(aszBuf), "*adsp");
    pProcItem = HI_DRV_PROC_AddModule(aszBuf, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_FATAL_ADSP("Create adsp proc entry fail!\n");
        return HI_FAILURE;
    }

    /* Set functions */
    pProcItem->read  = s_stAdspDrv.pstProcParam->pfnReadProc;
    pProcItem->write = s_stAdspDrv.pstProcParam->pfnWriteProc;

    return HI_SUCCESS;
}

HI_VOID ADSP_DRV_UnregisterProc(HI_VOID)
{
    HI_CHAR aszBuf[16];

    snprintf(aszBuf, sizeof(aszBuf), "*adsp");
    HI_DRV_PROC_RemoveModule(aszBuf);
    s_stAdspDrv.pstProcParam = HI_NULL;
}
#endif

#ifdef HI_SND_DSP_HW_SUPPORT
static HI_S32 ADSP_HW_PowerOn(HI_VOID)
{
    HI_S32 s32Ret;
    U_ADSP_CTRL ADSP_SYSCTRL;

    g_pADSPSysCrg = (volatile S_ADSP_CHN_REGS_TYPE* )ioremap_nocache(DSP0_SHARESRAM_CHAN0_BASEADDR, sizeof(S_ADSP_CHN_REGS_TYPE));
    if (HI_NULL == g_pADSPSysCrg)
    {
        HI_ERR_ADSP("g_pADSPSysCrg ioremap error!\n");
        return HI_FAILURE;
    }

    /* Reg(SysCrg) ONLY reset here, Avoid command covered by dsp init */
    memset((HI_VOID*)g_pADSPSysCrg, 0, sizeof(S_ADSP_CHN_REGS_TYPE));

    ADSP_SYSCTRL.u32 = g_pADSPSysCrg->ADSP_SYSCTRL.u32;
    ADSP_SYSCTRL.bits.aoe_cmd = ADSP_AOE_CMD_BUTT;
    ADSP_SYSCTRL.bits.aflt_cmd = ADSP_AFLT_CMD_BUTT;
    ADSP_SYSCTRL.bits.sys_cmd = ADSP_SYSCOM_CMD_BUTT;
    ADSP_SYSCTRL.bits.aoe_cmd_done = 1;
    ADSP_SYSCTRL.bits.aflt_cmd_done = 1;
    ADSP_SYSCTRL.bits.sys_cmd_done = 1;
    g_pADSPSysCrg->ADSP_SYSCTRL.u32 =  ADSP_SYSCTRL.u32;

    s32Ret = Dsp_LoadElf();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("Dsp load Elf fail: 0x%x!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = Dsp_ResetBoot();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("Dsp Boot fail: 0x%x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_VOID ADSP_HW_PowerOff(HI_VOID)
{
    U_PERI_CRG52 uTmpVal;
    uTmpVal.u32 = g_pstRegCrg->PERI_CRG52.u32;
    uTmpVal.bits.dsp0_srst_req = 1;   //dsp0 sreset
    uTmpVal.bits.dsp0_cken = 0;   //dsp0 clk disable
    g_pstRegCrg->PERI_CRG52.u32 = uTmpVal.u32;

    ADSP_SYS_SetCmd(ADSP_SYSCOM_CMD_STOP, HI_FALSE);

    if (g_pADSPSysCrg)
    {
        iounmap((HI_VOID*)g_pADSPSysCrg);
        g_pADSPSysCrg = HI_NULL;
    }
}
#endif

static HI_VOID ADSP_GetSettings(HI_VOID)
{
}

static HI_VOID ADSP_RestoreSettings(HI_VOID)
{
}

HI_S32 ADSP_DRV_Suspend(PM_BASEDEV_S* pdev, pm_message_t state)
{
    ADSP_GetSettings();
#ifdef HI_SND_DSP_HW_SUPPORT
    ADSP_HW_PowerOff();
#endif
    HI_PRINT("ADSP suspend OK\n");
    return HI_SUCCESS;
}

HI_S32 HI_DRV_ADSP_Init(HI_VOID)
{
    return ADSP_DRV_Init();
}

HI_VOID HI_DRV_ADSP_DeInit(HI_VOID)
{
    ADSP_DRV_Exit();
}

HI_S32 ADSP_DRV_Resume(PM_BASEDEV_S* pdev)
{
    ADSP_RestoreSettings();
#ifdef HI_SND_DSP_HW_SUPPORT
    if (HI_SUCCESS != ADSP_HW_PowerOn())
    {
        HI_FATAL_ADSP("Resume ADSP_HW_PowerOn fail\n");
        return HI_FAILURE;
    }
#endif
    HI_PRINT("ADSP resume OK\n");
    return HI_SUCCESS;
}

HI_S32 ADSP_DRV_Init(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_DRV_MODULE_Register(HI_ID_ADSP, ADSP_NAME, (HI_VOID*)&s_stAdspDrv.stExtFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("Reg module fail:%#x!\n", s32Ret);
        return s32Ret;
    }

#if (1 == HI_PROC_SUPPORT)
    s32Ret = ADSP_ProcInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("ADSP_ProcInit Failed!\n");
        HI_DRV_MODULE_UnRegister(HI_ID_ADSP);
        return HI_FAILURE;
    }
#endif

#ifdef HI_SND_DSP_PROC_LOG_SUPPORT
    s32Ret = ADSP_Proc_Log_Init();
    if (s32Ret)
    {
        HI_FATAL_ADSP("ADSP_Proc_Log_Init Failed!\n");
#if (1 == HI_PROC_SUPPORT)
        ADSP_ProcDeInit();
#endif
        HI_DRV_MODULE_UnRegister(HI_ID_ADSP);
        return HI_FAILURE;
    }
#endif

#ifdef HI_SND_DSP_HW_SUPPORT
    s32Ret = ADSP_HW_PowerOn();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("ADSP_HW_PowerOn Failed!\n");
#ifdef HI_SND_DSP_PROC_LOG_SUPPORT
        ADSP_Proc_Log_DeInit();
#endif
#if (1 == HI_PROC_SUPPORT)
        ADSP_ProcDeInit();
#endif
        HI_DRV_MODULE_UnRegister(HI_ID_ADSP);
        return HI_FAILURE;
    }
#endif
    return HI_SUCCESS;
}

HI_VOID ADSP_DRV_Exit(HI_VOID)
{
#ifdef HI_SND_DSP_HW_SUPPORT
    ADSP_HW_PowerOff();
#endif

#ifdef HI_SND_DSP_PROC_LOG_SUPPORT
    ADSP_Proc_Log_DeInit();
#endif

#if (1 == HI_PROC_SUPPORT)
    ADSP_ProcDeInit();
#endif

    HI_DRV_MODULE_UnRegister(HI_ID_ADSP);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
