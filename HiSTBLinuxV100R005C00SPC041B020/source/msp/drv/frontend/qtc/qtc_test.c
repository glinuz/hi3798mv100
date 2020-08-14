/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : qtc_test.c
Version       : v0.1
Author        : Hisilicon STB SOC group
Created       : 2011/5/27
Last Modified :
Description   : qtc test code
Function List :
History       :
1.Date        : 2011/6/14
  Author      : 
  Modification: 

******************************************************************************/
/* add include here */
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/random.h>
// for kernel socket
#include <linux/ip.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/udp.h>

#include "hi_type.h"
#include "hi_struct.h"
#include "hi_mmz.h"
#include "common_dev.h"
#include "common_proc.h"
#include "common_dev.h"
#include "common_mem.h"
#include "common_sys.h"

#include "sv_unf_osal.h"
#include "qtc_test.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif/* __cplusplus */


#define MOD_NAME_QTCTEST "soc_qtctest"
#define D_QTC_DATA_SIZE 0xa00000

void test_2ddr_burst8(int *src, int *des, HI_U32 times)
{
    HI_U32 i =0;
    
    /*times = size / 0x40,Because every time only reads 64 bytes*/
    for (i = 0; i < times; i++)
    {
        asm(
           "ldr r0, %0\n"
            "ldr r1, %1\n"
            "LDMIA   R0!,{R5-R12}\n"
            "STMIA   R1!,{R5-R12}\n"
            "LDMIA   R0!,{R5-R12}\n"
            "STMIA   R1!,{R5-R12}\n"
            :
            :"m"(src), "m"(des)
            :"r0","r1","r5","r6","r7","r8","r9","r10","r11","r12"
            );
        //src += 8;
        des += 0x10;
    }
}

static HI_VOID SocQtcTc_0_0_0(HI_VOID)
{
    HI_U8* pu8QtcRegAddr = HI_NULL;
    HI_U32 u32QtcPhyBuf;
    HI_U8* pu8QtcVirBuf= HI_NULL;
    HI_U32 u32QtcStatus = 0;
    HI_U32 i;
    SV_FILE *p_file_qtc;
    MMZ_BUFFER_S stMmzBuf;
    HI_U32 u32Times = D_QTC_DATA_SIZE >> 6;
    unsigned long u64IrqFlags = 0;
    D_SVPrintTc();    

    /* get QAM adc data */
    p_file_qtc = SV_klib_fopen("./qtc.bin", O_RDWR | O_CREAT/* |O_APPEND*/ , 00777);;

    pu8QtcRegAddr =(HI_U8*)ioremap_nocache(D_QTC_PHY_BASE_REG_ADDR,0x100);
    if(HI_NULL == pu8QtcRegAddr)
    {
        HI_ERR_QTC("ioremap_nocache map error.\n");
        return;
    }
    CMPI_MEM_AllocAndMapMem("QtcDataBuf", NULL, D_QTC_DATA_SIZE, 4, &stMmzBuf);
    u32QtcPhyBuf = stMmzBuf.u32StartPhyAddr;

    if(u32QtcPhyBuf == 0)
    {
        HI_ERR_QTC("buffer QtcDataBuf in alloc error\n");
        return ;
    }
    pu8QtcVirBuf = (HI_U8*)stMmzBuf.pu8StartVirAddr;
    HI_INFO_QTC("u32QtcPhyBuf = 0x%x\n", u32QtcPhyBuf);
    HI_INFO_QTC("u32QtcVirBuf = 0x%x\n", pu8QtcVirBuf);
    HI_INFO_QTC("u32Times = 0x%x\n", u32Times);
    
    local_irq_save(u64IrqFlags);
    *(volatile HI_U8 *)(pu8QtcRegAddr + 0x44) = 0;/*status reg : write 0*/


    *(volatile HI_U8 *)(pu8QtcRegAddr + 0x40) = 1;/*control reg : write 1, start*/
    //for(time_out = 0; time_out < 200; time_out++)
    {
        test_2ddr_burst8((HI_U8 *)pu8QtcRegAddr, (HI_U8 *)pu8QtcVirBuf, u32Times);
        //u32_ADC_data[time_out] = *(volatile HI_U32 *)(u32_VirQTC_PHY_BASE_ADDR + 0x00); /*read data*/
    }
    u32QtcStatus = *(volatile HI_U8 *)(pu8QtcRegAddr + 0x44);
    
    *(volatile HI_U8 *)(pu8QtcRegAddr + 0x40) = 0;/*control reg : write 0, stop*/
    
    *(volatile HI_U8 *)(pu8QtcRegAddr + 0x44) = 0;/*status reg : write 0*/
    iounmap(pu8QtcRegAddr);
    local_irq_restore(u64IrqFlags);

    if(0 != u32QtcStatus)
    {
        HI_INFO_QTC("qtc status = 0x%x\n", u32QtcStatus);
        HI_INFO_QTC("Error read qam adc data\n");
    }
    else
    {
        HI_INFO_QTC("Success read qam adc data\n");
    }
    
    for(i = 0; i < 0x20; i++)
    {
        HI_INFO_QTC("0x%x\n", *(((HI_U8 *)pu8QtcVirBuf) + i));
    }
    HI_INFO_QTC("L%d\n", __LINE__);
    SV_klib_fwrite((HI_S8*)pu8QtcVirBuf, D_QTC_DATA_SIZE, p_file_qtc);
    HI_INFO_QTC("L%d\n", __LINE__);
    SV_klib_fclose(p_file_qtc);
    HI_INFO_QTC("L%d\n", __LINE__);
    CMPI_MEM_UnmapAndReleaseMem(&stMmzBuf);
    return;
}

static HI_VOID SocQtcTc_0_0_3(HI_VOID)
{
    MMZ_BUFFER_S stMmzBufMv300;
    HI_U8* pu8QtcRegAddr = 0;
    HI_U32 u32QtcPhyBuf;
    HI_U8* pu8QtcVirBuf;
    unsigned long u64IrqFlags = 0;
    HI_U32 u32QtcStatus = 0;
    HI_U32 u32QtcContrl = 0;
    HI_U32 u32QtcTimeOut = 0;
    SV_FILE *p_file_qtc;
    
    pu8QtcRegAddr = (HI_U8*)ioremap_nocache(D_MV300_QTC_PHY_BASE_REG_ADDR,0x100);
    if(HI_NULL == pu8QtcRegAddr)
    {
        HI_ERR_QTC("ioremap_nocache map error.\n");
        return;
    }

    D_SVPrintTc(); 

    CMPI_MEM_AllocAndMapMem("QtcDataBuf", NULL, D_QTC_DATA_SIZE, 4, &stMmzBufMv300);
    u32QtcPhyBuf = stMmzBufMv300.u32StartPhyAddr;

    if(u32QtcPhyBuf == 0)
    {
        HI_ERR_QTC("buffer QtcDataBuf in alloc error\n");
        return ;
    }
    pu8QtcVirBuf = (HI_U8*)stMmzBufMv300.pu8StartVirAddr;
    HI_INFO_QTC("u32QtcPhyBuf = 0x%x\n", u32QtcPhyBuf);
    HI_INFO_QTC("u32QtcPhyEnd = 0x%x\n", u32QtcPhyBuf + D_QTC_DATA_SIZE);
    HI_INFO_QTC("u32QtcVirBuf = 0x%x\n", pu8QtcVirBuf);
    
    local_irq_save(u64IrqFlags);
    *(volatile HI_U8 *)(pu8QtcRegAddr + 0x48) = u32QtcPhyBuf;/*buffer start addr*/
    *(volatile HI_U8 *)(pu8QtcRegAddr + 0x4c) = u32QtcPhyBuf + D_QTC_DATA_SIZE;/*buffer end addr*/
    
    u32QtcContrl = *(volatile HI_U32 *)(pu8QtcRegAddr + 0x40);/*read contrl reg*/
    u32QtcContrl = u32QtcContrl | 0x7;/*collect data*/
    *(volatile HI_U8 *)(pu8QtcRegAddr + 0x40) = u32QtcContrl;
    
    do
    {
        msleep_interruptible(200);
        if(50 < u32QtcTimeOut)
        {
            HI_ERR_QTC("******QTC timeout******\n");
            break;
        }
        u32QtcTimeOut++;
        u32QtcStatus = *(volatile HI_U8 *)(pu8QtcRegAddr + 0x44);
    }while(!(u32QtcStatus & 0x3));
    iounmap(pu8QtcRegAddr);
    local_irq_restore(u64IrqFlags);

    if(u32QtcStatus & 0x1)
    {
        HI_INFO_QTC("******u32QtcStatus = 0x%x******\n", u32QtcStatus);
         /* get QAM adc data */
        p_file_qtc = SV_klib_fopen("./qtc.bin", O_RDWR | O_CREAT/* |O_APPEND*/ , 00777);
        SV_klib_fwrite((HI_S8*)stMmzBufMv300.Pu8StartVirAddr, D_QTC_DATA_SIZE, p_file_qtc);
        HI_INFO_QTC("L%d\n", __LINE__);
        SV_klib_fclose(p_file_qtc);
        HI_INFO_QTC("L%d\n", __LINE__);
    }
    else
    {
        HI_ERR_QTC("Qtc fifo overflow ,u32QtcStatus = 0x%x\n", u32QtcStatus);
    }
    
    CMPI_MEM_UnmapAndReleaseMem(&stMmzBufMv300);
    HI_INFO_QTC("L%d\n", __LINE__);
    
    return;
}

static HI_S32 __init SocQtcTest_ModInit(HI_VOID)
{
    HI_CHIP_TYPE_E enChipType;
    HI_U32 u32ChipVersion = 0;
    
    HI_DRV_SYS_GetChipVersion( &enChipType, &u32ChipVersion );
    if((HI_CHIP_TYPE_HI3716M == enChipType) && (HI_CHIP_VERSION_V300 == u32ChipVersion))
    {
        SocQtcTc_0_0_3();
        HI_INFO_QTC("Load soc_qtctest_mv300 success.  \t(%s)\n", VERSION_STRING);
    }
    else
    {
        SocQtcTc_0_0_0();
        HI_INFO_QTC("Load soc_qtctest success.  \t(%s)\n", VERSION_STRING);
    }
    
    return 0;
}

static HI_VOID SocQtcTest_ModExit(HI_VOID)
{
     HI_CHIP_TYPE_E enChipType;
    HI_U32 u32ChipVersion = 0;
    
    HI_DRV_SYS_GetChipVersion( &enChipType, &u32ChipVersion );
    if((HI_CHIP_TYPE_HI3716M == enChipType) && (HI_CHIP_VERSION_V300 == u32ChipVersion))
    {
        HI_INFO_QTC("unload soc_qtctest_mv300 success.  \n");
    }
    else
    {
        HI_INFO_QTC("unload soc_qtctest success.  \n");
    }
    return;
}


module_init(SocQtcTest_ModInit);
module_exit(SocQtcTest_ModExit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif/* __cplusplus */



