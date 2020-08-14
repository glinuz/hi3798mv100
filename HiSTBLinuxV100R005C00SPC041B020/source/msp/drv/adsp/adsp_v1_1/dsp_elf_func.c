/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpBeIL3vAuLvtpwmY1JB/WFe97hRqOpHstvurNiV
m9r5rs1vEO6jljnvMN1ddORzR0UCl1g2yI4K65zEWg8PkFCb+c6jq4F3HoEcuKlwi18vqu/8
ubwdAqxYV91fchlLl6cFBxECI/1YJZXfPJkcUshux/R0kDLUeccaYHhfAbklDQ==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/dma-mapping.h>
#include <linux/kthread.h>
#include "dsp_elf_func.h"
#include "hi_audsp_common.h"
#include "hi_reg_common.h"
#include "drv_adsp_private.h"

#if 0
#define DSP0_PRIM_VECTOR    (HI_U32*)0x101E0FC0
#define DSP0_REF_PRIM_VECTOR    (HI_U32*)0x10020400
#endif


void  PRIN_NULL(const char *s1, ...)
{
    return;
}

HI_S32 ResetELFSection(HI_U32 u32BaseAddr, HI_U32 u32Size)
{
    HI_U32      *vAddr = HI_NULL;

    if(u32BaseAddr > HI_PERI_BASE_ADDR)
        vAddr = ioremap_nocache(u32BaseAddr, u32Size);
    else
        vAddr = phys_to_virt(u32BaseAddr);

    if(HI_NULL == vAddr)
    {
        HI_ERR_ADSP("memory mmap failed\n");
        return HI_FAILURE;
    }
    memset((HI_VOID*)vAddr, 0, u32Size);

    if(u32BaseAddr > HI_PERI_BASE_ADDR)
    {
        iounmap(vAddr);
    }
    return  0;
}


HI_S32 MemcpySection(HI_U32 *pdst, HI_U32 *psrc, HI_U32 ifilesize)
{
    HI_U32 value;
    HI_S32 i;

    HI_U32 *udst = pdst ;
    HI_UCHAR *usrc = (HI_UCHAR *)psrc ;
    HI_VIRT_ADDR_T flag  = (HI_VIRT_ADDR_T)pdst ;

    if( (HI_VIRT_ADDR_T)flag & 3 )
    {
        ELF_PRINT("warning dst address is not align in 32bit\n");
    }

    for(i=0;i<((ifilesize+3)>>2);i++)
    {
        value   = usrc[0]|(usrc[1]<<8)|(usrc[2]<<16)|(usrc[3]<<24);
        udst[i] = value;
        usrc   += 4;
    }
    return 0;
}

HI_S32 CopyELFSection(HI_UCHAR *pElfAddr)
{
    HI_S32  j;
    Elf32_Ehdr *phdr = (Elf32_Ehdr *)pElfAddr;
    Elf32_Phdr *ppdr = (Elf32_Phdr *)(pElfAddr+phdr->e_phoff);
    HI_U32      *vAddr;

    for( j = 0 ; j < phdr->e_phnum ; j++)
    {
        if( ( ppdr[j].p_type == PT_LOAD ) && ( ppdr[j].p_filesz != 0) )
        {
#if 0
            if (ppdr[j].p_paddr == DSP0_PRIM_VECTOR)
            {
                //remap addr
                ppdr[j].p_paddr = DSP0_REF_PRIM_VECTOR;
            }
            if (ppdr[j].p_paddr == DSP1_PRIM_VECTOR)
            {
                //remap addr
                ppdr[j].p_paddr = DSP1_REF_PRIM_VECTOR;
            }
#endif
#if 0
            vAddr = ioremap_nocache( ppdr[j].p_paddr , ppdr[j].p_filesz + 3 );
#else
            if(ppdr[j].p_paddr > HI_PERI_BASE_ADDR)
                vAddr = ioremap_nocache( ppdr[j].p_paddr , ppdr[j].p_filesz + 3 );
        else
                vAddr = phys_to_virt(ppdr[j].p_paddr);
#endif
            if(HI_NULL == vAddr)
            {
                return HI_FAILURE;
            }
            ELF_PRINT("Phy addr: 0x%.8x;size: 0x%.8x, flag(%d)\n",ppdr[j].p_paddr,ppdr[j].p_filesz,ppdr[j].p_paddr > HI_PERI_BASE_ADDR);
            MemcpySection((HI_U32*)vAddr , (HI_U32 *)(pElfAddr + ppdr[j].p_offset) , ppdr[j].p_filesz);
            if(ppdr[j].p_paddr > HI_PERI_BASE_ADDR)
                iounmap(vAddr);
        }
    }
    return  0;
}


HI_S32 CheckELFPaser(HI_UCHAR* pElfAddr)
{
    HI_S32 i,j;
    Elf32_Ehdr *phdr = (Elf32_Ehdr *)pElfAddr;
    Elf32_Phdr *ppdr = (Elf32_Phdr *)(pElfAddr+phdr->e_phoff);
    HI_U32 *vAddr;

    for( j = 0 ; j < phdr->e_phnum ; j++)
    {
        if( ( ppdr[j].p_type == PT_LOAD) && ( ppdr[j].p_filesz != 0))
        {

#if 0
            if (ppdr[j].p_paddr == DSP0_PRIM_VECTOR)
            {
                //重映射地址
                ppdr[j].p_paddr = DSP0_REF_PRIM_VECTOR;
            }
            if (ppdr[j].p_paddr == DSP1_PRIM_VECTOR)
            {
                //重映射地址
                ppdr[j].p_paddr = DSP1_REF_PRIM_VECTOR;
            }
#endif
            //将ELF解析出来的物理地址映射到虚拟地址
#if 0
            vAddr = ioremap_nocache( ppdr[j].p_paddr , ppdr[j].p_filesz + 3 );
#else
            if(ppdr[j].p_paddr > HI_PERI_BASE_ADDR)
                vAddr = ioremap_nocache( ppdr[j].p_paddr , ppdr[j].p_filesz + 3 );
            else
                vAddr = phys_to_virt( ppdr[j].p_paddr);
#endif
            if(HI_NULL == vAddr)
            {
                return HI_FAILURE;
            }

            for (i=0;(i<ppdr[j].p_filesz>>2);i++)
            {
                    //虚拟地址对应的值，与ELF中解析的值比较
                    if (vAddr[i]!=((HI_U32 *)(pElfAddr + ppdr[j].p_offset))[i])
                    {
                        ELF_PRINT("Error addr 0x%x: 0x%x - 0x%x:0x%x \r\n",ppdr[j].p_paddr ,vAddr[i],((HI_U32 *)(pElfAddr + ppdr[j].p_offset))[i],((HI_U32 *)(pElfAddr + ppdr[j].p_offset))[i]);
                        if(ppdr[j].p_paddr > HI_PERI_BASE_ADDR)
                            iounmap(vAddr);
                        return HI_FAILURE;
                    }
            }
            if(ppdr[j].p_paddr > HI_PERI_BASE_ADDR)
                iounmap(vAddr);

        }
    }

    return HI_SUCCESS;

}
