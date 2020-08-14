/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name             :    runtime_module.c
  Version               :     Initial Draft
  Author                :     Hisilicon multimedia software group
  Created               :     2012/09/07
  Last Modified        :
  Description          :
  Function List        :
  History                :
  1.Date                 :     2012/09/07
    Author               :
    Modification        :    Created file

******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <linux/module.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/seq_file.h>
#include <linux/version.h>

#include <asm/atomic.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/traps.h>
#include <linux/semaphore.h>

#include <linux/miscdevice.h>

#include <linux/vmalloc.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/io.h>
#include <asm/pgalloc.h>

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/slab.h>

#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/time.h>

#include <linux/statfs.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/namei.h>

#include "hi_drv_mem.h"

#include "sha1.h"
#include "hi_drv_mmz.h"
#include "hi_drv_advca.h"
#include "drv_advca_internal.h"
#include "drv_advca_external.h"

#if defined (CHIP_TYPE_hi3716mv300) || defined (CHIP_TYPE_hi3716mv310) || defined (CHIP_TYPE_hi3716mv320) || defined (CHIP_TYPE_hi3110ev500)

#define MAX_BUFFER_LENGTH   (0x20000) //128k
#define SC_SYS_BASE     (0x101E0000)
#define SC_SYSRES       (SC_SYS_BASE + 0x0004)

#define RUNTIME_CHECK_EN_REG_ADDR   0x10180084//OTP:runtime_check_en indicator:0x10180084[20]
#define MAX_IOMEM_SIZE  0x400

#define C51_BASE           0x600B0000  //0x600B0000
#define C51_SIZE             0x10000
#define C51_DATA           0x7000

#define SC_GEN27    (SC_SYS_BASE + 0x00EC) //This register is to inform the C51 code that the runtime check vector is ready.
#define PMOC_RUNTIME_CHECK_OK     0x80510003 //to inform the C51 code that the runtime check vector is ready.
#define KL_LPC_DISABLE  0x10000144 //Lock C51 RAM and reset signal, bit[0]:kl_lpc_rst_disable, bit[1]:kl_lpc_ram_wr_disable

#else //Chip type Hi3716CV200/Hi3716MV400/Hi3719CV100/Hi3716MV420/Hi3716MV410.

#define MAX_BUFFER_LENGTH                             (0x20000) //128k
#define SC_SYS_BASE                                   (0xF8000000)
#define SC_SYSRES                                     (SC_SYS_BASE + 0x0004)

#define RUNTIME_CHECK_EN_REG_ADDR                     0xF8AB0084    //OTP:runtime_check_en indicator :0xF8AB0084[20]
#define MAX_IOMEM_SIZE                                0x400

#define C51_BASE                                      0xf8400000
#define C51_SIZE                                      0x10000
#define C51_DATA                                      0x7000

#define SC_GEN27                                      (SC_SYS_BASE + 0x00EC) //This register is to inform the C51 code that the runtime check vector is ready.
#define PMOC_RUNTIME_CHECK_OK                         (0x80510003) //to inform the C51 code that the runtime check vector is ready.
//#define KL_LPC_DISABLE                                  (0xf8ab0000 + 0x184) //Lock C51 RAM and reset signal, bit[0]:kl_lpc_rst_disable, bit[1]:kl_lpc_ram_wr_disable
#define KL_LPC_DISABLE                                (0xf8a90000 + 0x144) //Lock C51 RAM and reset signal, bit[0]:kl_lpc_rst_disable, bit[1]:kl_lpc_ram_wr_disable
#define MCU_START_REG                                 (0xf840f000)
#endif

#if    defined (CHIP_TYPE_hi3716mv300) \
    || defined (CHIP_TYPE_hi3716mv310) \
    || defined (CHIP_TYPE_hi3716mv320) \
    || defined (CHIP_TYPE_hi3110ev500) \
    || defined (CHIP_TYPE_hi3716cv200) \
    || defined (CHIP_TYPE_hi3716mv400) \
    || defined (CHIP_TYPE_hi3719cv100) \
    || defined (CHIP_TYPE_hi3716mv410) \
    || defined (CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3798cv200) \
    || defined(CHIP_TYPE_hi3798mv200) \
    || defined(CHIP_TYPE_hi3798mv200_a)

#define HI_REG_WRITE32(addr, val) (*(volatile unsigned int*)(addr) = (val))
#define HI_REG_READ32(addr, val) ((val) = *(volatile unsigned int*)(addr))

#define KO_DUMP_AREA (10 * 1024) // a dump area of check .text section of the module

static struct task_struct *g_pModuleCopyThread = NULL;
static struct task_struct *g_pFsCheckThread = NULL;
static unsigned int g_u32ModulePhyAddr = 0;
static unsigned char *g_pModuleVirAddr = NULL;
static int g_bRuntimeCheckInit = HI_FALSE;
//static int g_bC51CodeLoaded = HI_FALSE;
static int g_bRuntimeCheckEnable = HI_FALSE;
static int g_u32ModuleUsedSize = 0;
static MMZ_BUFFER_S g_stMmzBuf = {0};

static int g_bIsFirstCopy = HI_TRUE;

static int module_copy_thread_proc(void* argv);
static int fs_check_thread_proc(void* argv);
static int get_kernel_info(unsigned int *pu32StartAddr, unsigned int *pu32EndAddr);
static int calc_kernel_hash(unsigned int u32Hash[5]);
static int calc_fs_hash(unsigned int u32Hash[5]);
static int calc_ko_hash(unsigned int u32Hash[5]);
static int calc_ko_size(void);
static int store_check_vector(void);
static int get_fs_check_vector(unsigned int u32HashValue[5]);
void Config_LPC_Setting(void);


static void errorreset(void)
{
    unsigned int *pVIR_ADDRESS = NULL;
    HI_FATAL_CA("runtime check error, reboot\n");
    msleep(1000);
    /*  error process: reset the whole chipset */
    pVIR_ADDRESS  = (void*)(SC_SYSRES);
    *(unsigned int*)pVIR_ADDRESS = 0x1;
    return;
}

static int GetRuntimeCheckEnableFlag(int *pbRuntimeCheckFlag)
{
    unsigned int *pu32RegVirAddr = NULL;

    if(NULL == pbRuntimeCheckFlag)
        return -1;

    *pbRuntimeCheckFlag = HI_FALSE;

    pu32RegVirAddr = (unsigned int *)ioremap_nocache(RUNTIME_CHECK_EN_REG_ADDR, 32);
    if(pu32RegVirAddr == NULL)
    {
        return -1;
    }
    if(*pu32RegVirAddr & 0x100000)
    {
        *pbRuntimeCheckFlag = HI_TRUE;
    }
    iounmap((void*)pu32RegVirAddr);

    return 0;
}

int  RuntimeModule_Init(void)
{
    int Ret = 0;

    Ret = GetRuntimeCheckEnableFlag(&g_bRuntimeCheckEnable);
    if( (Ret != 0) || (g_bRuntimeCheckEnable != HI_TRUE) )
    {
        return 0;
    }
    if (NULL == g_pModuleCopyThread)
    {
        g_pModuleCopyThread = kthread_create(module_copy_thread_proc, NULL, "ModuleCopyThread");
        if (NULL == g_pModuleCopyThread)
        {
            return -1;
        }
        wake_up_process(g_pModuleCopyThread);
    }

    if (NULL == g_pFsCheckThread)
    {
        g_pFsCheckThread = kthread_create(fs_check_thread_proc, NULL, "FsCheckThread");
        if (NULL == g_pFsCheckThread)
        {
            return -1;
        }
        wake_up_process(g_pFsCheckThread);
    }

    return 0;
}

static int module_copy_thread_proc(void* argv)
{
    struct module *p = NULL;
    struct module *mod = NULL;
    struct list_head *pModules = NULL;
    int Ret = 0;
    unsigned char *pAddr = NULL;
    unsigned int u32ModuleSize;

    msleep(5000);

    HI_FATAL_CA("\n******** Runtime Check Initial ***********\n");

    g_bRuntimeCheckInit = HI_TRUE;
 
    g_u32ModuleUsedSize = calc_ko_size() + KO_DUMP_AREA;
    memset(&g_stMmzBuf, 0, sizeof(MMZ_BUFFER_S));

    Ret =  HI_DRV_MMZ_AllocAndMap("RUNTIME_CHECK", "RT", g_u32ModuleUsedSize, 0, &g_stMmzBuf);
    if (HI_FAILURE == Ret)
    {
        kthread_stop(g_pFsCheckThread);

        HI_FATAL_CA("Failed to malloc MMZ buffer!, reboot\n");
        errorreset();
        return -1;
    }
     
    g_u32ModulePhyAddr = g_stMmzBuf.u32StartPhyAddr;
    g_pModuleVirAddr = g_stMmzBuf.pu8StartVirAddr;

    if((!g_bRuntimeCheckEnable) || (g_u32ModulePhyAddr == 0))
    {
        errorreset();
        return 0;
    }

    memset(g_pModuleVirAddr, 0x0, g_u32ModuleUsedSize);

    module_get_pointer(&pModules);
    while (1)
    {
        pAddr = g_pModuleVirAddr;
        u32ModuleSize = 0;
         
        list_for_each_entry_rcu(mod, pModules, list)
        { 
            p = find_module(mod->name);
            if (p)
            {
                u32ModuleSize += p->core_text_size;
                if (g_u32ModuleUsedSize < u32ModuleSize)
                {
                    HI_FATAL_CA("total ko size error, reboot\n");
                    errorreset();
                    return 0;
                }
                 
                memcpy((void*)pAddr, (void*)(p->module_core), p->core_text_size);
                pAddr += p->core_text_size;
            }
        }
         if(g_u32ModuleUsedSize != u32ModuleSize + KO_DUMP_AREA) //alloc mem size is u32ModuleSize + KO_DUMP_AREA
        {
            HI_FATAL_CA("Runtime-Checking ko error, reboot\n");
            errorreset();
            return 0;
        }

        if(g_bIsFirstCopy)
        { 
            store_check_vector();
            Config_LPC_Setting();
            g_bIsFirstCopy = HI_FALSE;  //It means runtime-checking is ready.
        }
 
        msleep(1000);
    }

    return Ret;
}

void Config_LPC_Setting(void)
{
    HI_U8 * pu8VIR_ADDRESS = HI_NULL;

    if (HI_TRUE == g_bRuntimeCheckEnable)
    {
        HI_FATAL_CA("Resume Advca LPC feature\n");
        pu8VIR_ADDRESS  = (HI_U8*)ioremap_nocache(MCU_START_REG, 0x100);
        if (NULL == pu8VIR_ADDRESS)
        {
            HI_FATAL_CA("return\n");
            return;
        }
        writel(0x1, pu8VIR_ADDRESS);//Start MCU
        iounmap(pu8VIR_ADDRESS);
        pu8VIR_ADDRESS  = (HI_U8*)ioremap_nocache(SC_GEN27, 0x100);
        if (NULL == pu8VIR_ADDRESS)
        {
            HI_FATAL_CA("return\n");
            return;
        }
        writel(PMOC_RUNTIME_CHECK_OK, pu8VIR_ADDRESS);//runtime check vector is ready
        iounmap(pu8VIR_ADDRESS);
        pu8VIR_ADDRESS  = (HI_U8*)ioremap_nocache(KL_LPC_DISABLE, 0x100);
        if (NULL == pu8VIR_ADDRESS)
        {
            HI_FATAL_CA("return\n");
            return;
        }
        writel(0x3,pu8VIR_ADDRESS);//lpc_ram_wr_disable, lpc_rst_disable
        HI_FATAL_CA("End of Resume Advca LPC feature\n");
        iounmap(pu8VIR_ADDRESS);
    }

    return;
}

static int fs_check_thread_proc(void* argv)
{
    int Ret;
    unsigned int u32OrgFsHashValue[5] = {0,0,0,0,0};
    unsigned int u32HashValue[5] = {0,0,0,0,0};

    while(!g_bRuntimeCheckInit)
    {
        msleep(10);
    }

    while(g_bRuntimeCheckEnable)
    {
        if(g_bIsFirstCopy == HI_TRUE)//It means Runtime-Checking is not ready.
        {
            msleep(10000);
            continue;
        }
        //get the fs hash value from LPC.
        get_fs_check_vector(u32OrgFsHashValue);

        Ret = calc_fs_hash(u32HashValue);
        Ret = memcmp(u32OrgFsHashValue, u32HashValue, sizeof(u32HashValue));
        if(Ret)
        {
            HI_FATAL_CA("FS Runtime-Checking error, reboot\n");
            errorreset();
        }
        msleep(10000);
    }

    return 0;
}

static int get_kernel_info(unsigned int *pu32StartAddr, unsigned int *pu32EndAddr)
{
    unsigned char *pTmpbuf = NULL;
    struct file * fp = NULL;
    mm_segment_t fs;
    loff_t pos = 0;
    char s8TmpBuf[32];
    char *pstr = NULL;

    memset(s8TmpBuf, 0, sizeof(s8TmpBuf));
    memcpy(s8TmpBuf, "Kernel code", strlen("Kernel code"));
    s8TmpBuf[sizeof (s8TmpBuf) - 1] = '\0';

    memset(&fs, 0, sizeof(mm_segment_t));

    fs = get_fs();
    set_fs(KERNEL_DS);
    set_fs(fs);

    /* get file handle */
    fp = filp_open("/proc/iomem", O_RDONLY | O_LARGEFILE, 0644);
    if (IS_ERR(fp))
    {
        HI_FATAL_CA("open /proc/iomem error, reboot\n");
        errorreset();
        return -1;
    }

    pTmpbuf = kmalloc(MAX_IOMEM_SIZE, GFP_TEMPORARY);
    if(pTmpbuf == NULL)
    {
        filp_close(fp, NULL);
        HI_FATAL_CA("kmalloc error, reboot\n");
        errorreset();
        return -1;
    }
    memset(pTmpbuf, 0, MAX_IOMEM_SIZE);

    /* get file content */
    pos = 0;
    fs = get_fs();
    set_fs(KERNEL_DS);
    vfs_read(fp, pTmpbuf, MAX_IOMEM_SIZE -1, &pos);
    set_fs(fs);

    pstr = strstr(pTmpbuf, s8TmpBuf);
    if(pstr == NULL)
    {
        kfree(pTmpbuf);
        filp_close(fp, NULL);
        HI_FATAL_CA("Get file content error, reboot\n");
        errorreset();
        return -1;
    }
    pos = pstr - (char *)pTmpbuf;

    memset(s8TmpBuf, 0, sizeof(s8TmpBuf));
    memcpy(s8TmpBuf, pTmpbuf + pos - 20, 8);
    *pu32StartAddr = simple_strtoul(s8TmpBuf, 0, 16);

    memset(s8TmpBuf, 0, sizeof(s8TmpBuf));
    memcpy(s8TmpBuf, pTmpbuf + pos - 11, 8);
    *pu32EndAddr = simple_strtoul(s8TmpBuf, 0, 16);

    kfree(pTmpbuf);
    /* close file handle */
    filp_close(fp, NULL);

    return 0;
}

static int calc_kernel_hash(unsigned int u32Hash[5])
{
    unsigned int u32TmpValue = 0;
    unsigned int u32KernelStartAddr = 0, u32KernelEndAddr = 0, u32KernelSize = 0;
    int Ret = 0;
    sha1_context ctx;
    unsigned char u8HashValue[20];
    unsigned int * pKernelVirAddr = 0;
    unsigned int i = 0;
    unsigned int u32OneTimeLen = 0;
    unsigned int u32LeftLen = 0;
    unsigned int u32Offset = 0;

    memset(u8HashValue, 0, sizeof(char)*20);
    memset(&ctx, 0, sizeof(sha1_context));

    Ret = get_kernel_info(&u32KernelStartAddr, &u32KernelEndAddr);
    if(Ret != 0)
    {
        return -1;
    }

    u32KernelSize = u32KernelEndAddr - u32KernelStartAddr;
    pKernelVirAddr = phys_to_virt(u32KernelStartAddr);

    hi_sha1_starts(&ctx);

    u32LeftLen = u32KernelSize;
    while(u32LeftLen > 0)
    {
        u32OneTimeLen = u32LeftLen > MAX_BUFFER_LENGTH ? MAX_BUFFER_LENGTH : u32LeftLen;
        hi_sha1_update(&ctx, (unsigned char*)pKernelVirAddr + u32Offset, u32OneTimeLen);
        u32LeftLen -= u32OneTimeLen;
        u32Offset += u32OneTimeLen;
        msleep(10);
    }

    hi_sha1_finish(&ctx, u8HashValue);

    for(i = 0; i < 20; i += 4)
    {
        u32TmpValue = ((unsigned int)u8HashValue[i]) << 24;
        u32TmpValue |= ((unsigned int)u8HashValue[i+1]) << 16;
        u32TmpValue |= ((unsigned int)u8HashValue[i+2]) << 8;
        u32TmpValue |= ((unsigned int)u8HashValue[i+3]);
        u32Hash[i / 4] = u32TmpValue;
    }

    return 0;
}

static int calc_ko_hash(unsigned int u32Hash[5])
{
    unsigned int u32TmpValue = 0;
    sha1_context ctx;
    unsigned char u8HashValue[20];
    unsigned int i = 0;
    unsigned int u32OneTimeLen = 0;
    unsigned int u32LeftLen = 0;
    unsigned int u32Offset = 0;

    memset(u8HashValue, 0, sizeof(char)*20);
    memset(&ctx, 0, sizeof(sha1_context));
    hi_sha1_starts(&ctx);
    u32LeftLen = g_u32ModuleUsedSize;
    while(u32LeftLen > 0)
    {
        u32OneTimeLen = u32LeftLen > MAX_BUFFER_LENGTH ? MAX_BUFFER_LENGTH : u32LeftLen;
        hi_sha1_update(&ctx, g_pModuleVirAddr + u32Offset, u32OneTimeLen);
        u32LeftLen -= u32OneTimeLen;
        u32Offset += u32OneTimeLen;
        msleep(10);
    }
    hi_sha1_finish(&ctx, u8HashValue);

    for(i = 0; i < 20; i += 4)
    {
        u32TmpValue = ((unsigned int)u8HashValue[i]) << 24;
        u32TmpValue |= ((unsigned int)u8HashValue[i+1]) << 16;
        u32TmpValue |= ((unsigned int)u8HashValue[i+2]) << 8;
        u32TmpValue |= ((unsigned int)u8HashValue[i+3]);

        u32Hash[i / 4] = u32TmpValue;
    }

    return 0;
}

static int calc_ko_size(void)
{
    int u32ko_size = 0;
    struct module *p = NULL;
    struct module *mod = NULL;
    struct list_head *pModules = NULL;
 
    module_get_pointer(&pModules);
    list_for_each_entry_rcu(mod, pModules, list) {
        p = find_module(mod->name);
        if(p) {
            u32ko_size += p->core_text_size;
            //HI_FATAL_CA("module:%s, size: %d\n", mod->name, p->core_text_size);
        }
    }
 
    return u32ko_size;
}

static int calc_fs_hash(unsigned int u32Hash[5])
{
    int Ret = 0;
    unsigned char u8HashValue[20];
    unsigned int i = 0;
    unsigned int u32TmpValue = 0;
    static unsigned char *pTmpbuf = NULL;
    sha1_context ctx;
    struct file * fp = NULL;
    mm_segment_t fs = {0};
    struct path path = {0};
    struct kstatfs st = {0};
    loff_t pos = {0};
    unsigned int u32ReadLength = 0;
    unsigned int u32LeftLength = 0;

    memset(u8HashValue, 0, sizeof(char)*20);
    memset(&ctx, 0, sizeof(sha1_context));

    fs = get_fs();
    set_fs(KERNEL_DS);
    set_fs(fs);

    memset(&ctx, 0x00, sizeof(sha1_context));

    Ret = user_path("/", &path);
    if (0 != Ret || path.dentry == NULL)
    {
        HI_FATAL_CA("user_path() error, reboot\n");
        errorreset();
        return -1;
    }

    Ret = vfs_statfs(&path,&st);
    if (0 != Ret)
    {
        HI_FATAL_CA("vfs_statfs() error, reboot\n");
        errorreset();
        return -1;
    }
    path_put(&path);


    /* get file handle */
    fp = filp_open("/dev/ram0", O_RDONLY | O_LARGEFILE, 0644);
    if (IS_ERR(fp))
    {
        HI_FATAL_CA("filp_open() error, reboot\n");
        errorreset();
        return -1;
    }

    if(pTmpbuf == NULL)
    {
        pTmpbuf = HI_VMALLOC(HI_ID_CA, MAX_BUFFER_LENGTH);
        if(pTmpbuf == NULL)
        {
            filp_close(fp, NULL);
            HI_FATAL_CA("filp_close() error, reboot\n");
            errorreset();
            return -1;
        }
    }

    /* get file content */
    pos = 0;
    u32LeftLength = st.f_bsize * st.f_blocks;
    hi_sha1_starts(&ctx);
    while(u32LeftLength > 0)
    {
        u32ReadLength = u32LeftLength >= MAX_BUFFER_LENGTH ? MAX_BUFFER_LENGTH : u32LeftLength;
        u32LeftLength -= u32ReadLength;
        fs = get_fs();
        set_fs(KERNEL_DS);
        vfs_read(fp, pTmpbuf, u32ReadLength, &pos);
        set_fs(fs);
        hi_sha1_update(&ctx, pTmpbuf, u32ReadLength);
        msleep(100);
    }
    hi_sha1_finish(&ctx, u8HashValue);

    for(i = 0; i < 20; i += 4)
    {
        u32TmpValue = ((unsigned int)u8HashValue[i]) << 24;
        u32TmpValue |= ((unsigned int)u8HashValue[i+1]) << 16;
        u32TmpValue |= ((unsigned int)u8HashValue[i+2]) << 8;
        u32TmpValue |= ((unsigned int)u8HashValue[i+3]);

        u32Hash[i / 4] = u32TmpValue;
    }

    /* close file handle */
    filp_close(fp, NULL);

    return 0;
}

static int store_check_vector(void)
{
    unsigned int u32HashValue[5] = {0};
    unsigned int *pu32Ptr = NULL;
    unsigned int i = 0;
    unsigned int u32SectionNum = 2; //kernel, KO
    unsigned int u32KernelStartAddr = 0, u32KernelEndAddr = 0, u32KernelSize = 0;
    unsigned int *pC51CheckVectorVirAddr = NULL;
 
    pC51CheckVectorVirAddr = ioremap_nocache(C51_BASE + C51_DATA + 0xE00, C51_SIZE - C51_DATA - 0xE00);
 
    //kernel hash value
    calc_kernel_hash(u32HashValue);
    for(i = 0; i < 5; i++)
    {
        pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x14) + i;
        HI_REG_WRITE32(pu32Ptr, u32HashValue[i]);
    }
 
    //ko hash value
    calc_ko_hash(u32HashValue);
    for(i = 0; i < 5; i++)
    {
        pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x28) + i;
        HI_REG_WRITE32(pu32Ptr, u32HashValue[i]);
    }
 
    //check section number, address, length
    pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x50);
    HI_REG_WRITE32(pu32Ptr, u32SectionNum);
 
    //kernel address, length
    get_kernel_info(&u32KernelStartAddr, &u32KernelEndAddr);
    u32KernelSize = u32KernelEndAddr - u32KernelStartAddr;
    pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x54);
    HI_REG_WRITE32(pu32Ptr, u32KernelStartAddr);

    pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x58);
    HI_REG_WRITE32(pu32Ptr, u32KernelSize);

    //KO address, length
    pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x5c);
    HI_REG_WRITE32(pu32Ptr, g_u32ModulePhyAddr);

    pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x60);
    HI_REG_WRITE32(pu32Ptr, g_u32ModuleUsedSize);
 
    //Store Hash value of FS.
    pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x64);
    u32SectionNum = 0x01;
    HI_REG_WRITE32(pu32Ptr, u32SectionNum);
 
    calc_fs_hash(u32HashValue);
    for(i = 0; i < 5; i ++)
    {
        pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x68) + i;
        HI_REG_WRITE32(pu32Ptr, u32HashValue[i]);
    }

    iounmap((void*)pC51CheckVectorVirAddr);

    return 0;
}

static int get_fs_check_vector(unsigned int u32HashValue[5])
{
    unsigned int *pu32Ptr = NULL;
    unsigned int i = 0;
    unsigned int *pC51CheckVectorVirAddr = NULL;

    pC51CheckVectorVirAddr = ioremap_nocache(C51_BASE + C51_DATA + 0xE00, C51_SIZE - C51_DATA - 0xE00);
    //get the fs hash value from LPC.
    for(i = 0; i < 5; i ++)
    {
        pu32Ptr = (unsigned int*)(((unsigned char*)pC51CheckVectorVirAddr) + 0x68) + i;
        HI_REG_READ32(pu32Ptr, u32HashValue[i]);
    }

    iounmap((void*)pC51CheckVectorVirAddr);

    return 0;
}

void RuntimeModule_Exit(void)
{
    if(g_bRuntimeCheckEnable != HI_TRUE)
    {
        return;
    }

    if(NULL != g_pModuleCopyThread)
    {
        kthread_stop(g_pModuleCopyThread);
    }

    if(NULL != g_pFsCheckThread)
    {
        kthread_stop(g_pFsCheckThread);
    }

    if (g_pModuleVirAddr != 0)
    {
        HI_DRV_MMZ_UnmapAndRelease(&g_stMmzBuf);
    }
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

