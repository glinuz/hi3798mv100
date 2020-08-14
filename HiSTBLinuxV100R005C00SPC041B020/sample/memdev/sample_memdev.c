/******************************************************************************

  Copyright (C), 2013-2023, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_memdev.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/03
  Description   :
  History       :
  1.Date        : 2013/07/03
    Author      : l00185424
    Modification: Created file

******************************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "hi_common.h"

#define SAMPLE_PRINT printf

HI_S32 MMAP_Mothed1(HI_U32 u32Address, HI_U32 u32Value)
{
    HI_S32 Ret;
    HI_U32 u32ReadValue;

    Ret = HI_SYS_ReadRegister(u32Address, &u32ReadValue);
    if (Ret != HI_SUCCESS)
    {
        SAMPLE_PRINT("HI_SYS_ReadRegister failure!\n");
        return Ret;
    }
    SAMPLE_PRINT("[%s] Read %#x: %#x\n", __FUNCTION__, u32Address, u32ReadValue);

    Ret = HI_SYS_WriteRegister(u32Address, u32Value);
    if (Ret != HI_SUCCESS)
    {
        SAMPLE_PRINT("HI_SYS_WriteRegister failure!\n");
        return Ret;
    }
    SAMPLE_PRINT("[%s] Write %#x: %#x\n", __FUNCTION__, u32Address, u32Value);

    Ret = HI_SYS_ReadRegister(u32Address, &u32ReadValue);
    if (Ret != HI_SUCCESS)
    {
        SAMPLE_PRINT("HI_SYS_ReadRegister failure!\n");
        return Ret;
    }
    SAMPLE_PRINT("[%s] Read %#x: %#x\n", __FUNCTION__, u32Address, u32ReadValue);
    return HI_SUCCESS;
}

HI_S32 MMAP_Mothed2(HI_U32 u32Address, HI_U32 u32Value)
{
    HI_S32 Ret;
    HI_U32 *pu32Vir;

    Ret = HI_SYS_MapRegister(u32Address, 4, (HI_VOID*)&pu32Vir);
    if (Ret != HI_SUCCESS)
    {
        SAMPLE_PRINT("HI_SYS_MapRegister failure!\n");
        return Ret;
    }

    SAMPLE_PRINT("[%s] Read %#x: %#x\n", __FUNCTION__, u32Address, *pu32Vir);
    *(volatile HI_U32*)pu32Vir = u32Value;
    SAMPLE_PRINT("[%s] Write %#x: %#x\n", __FUNCTION__, u32Address, u32Value);
    SAMPLE_PRINT("[%s] Read %#x: %#x\n", __FUNCTION__, u32Address, *(volatile HI_U32*)pu32Vir);

    Ret = HI_SYS_UnmapRegister(pu32Vir);
    if (Ret != HI_SUCCESS)
    {
        SAMPLE_PRINT("HI_SYS_UnmapRegister failure!\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 MMAP_Mothed3(HI_U32 u32Address, HI_U32 u32Value)
{
    HI_S32 s32Fd;
    HI_U32 *pu32Vir;
    HI_U32 u32AddrPage;
    HI_U32 u32AddrInPage;

    u32AddrPage = u32Address & (~0xfff);
    u32AddrInPage = u32Address & 0xfff;

    s32Fd = open("/dev/hi_mem", O_RDWR);

    pu32Vir = (HI_VOID *)mmap(HI_NULL_PTR, 4, PROT_READ|PROT_WRITE, MAP_SHARED, s32Fd, u32AddrPage);
    if (pu32Vir == MAP_FAILED)
    {
        SAMPLE_PRINT("Address 0x%08x map to virtual mem failed!\n", u32AddrPage);
        return HI_FAILURE;
    }
    SAMPLE_PRINT("[%s] Read %#x: %#x\n", __FUNCTION__, u32Address, *(volatile HI_U32*)(pu32Vir + u32AddrInPage/4));
    *(volatile HI_U32*)(pu32Vir + u32AddrInPage/4) = u32Value;
    SAMPLE_PRINT("[%s] Write %#x: %#x\n", __FUNCTION__, u32Address, u32Value);
    SAMPLE_PRINT("[%s] Read %#x: %#x\n", __FUNCTION__, u32Address, *(volatile HI_U32*)(pu32Vir + u32AddrInPage/4));

    munmap(pu32Vir, 4);
    close(s32Fd);

    return HI_SUCCESS;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_U32 u32Addr;
    HI_U32 u32Value;

    HI_SYS_Init();

    if (argc < 3)
    {
        SAMPLE_PRINT("Usage: %s HEX_ADDR HEX_VALUE [METHOD]\n", argv[0]);
        return -1;
    }

    u32Addr = strtoul(argv[1], HI_NULL, 16);
    u32Value = strtoul(argv[2], HI_NULL, 16);

    if (argc >= 4)
    {
        switch (atoi(argv[3]))
        {
            case 1:
                MMAP_Mothed1(u32Addr, u32Value);
                break;
            case 2:
                MMAP_Mothed2(u32Addr, u32Value);
                break;
            case 3:
                MMAP_Mothed3(u32Addr, u32Value);
                break;
            default:
                MMAP_Mothed1(u32Addr, u32Value);
                MMAP_Mothed2(u32Addr, u32Value);
                MMAP_Mothed3(u32Addr, u32Value);
                break;
        }
    }
    else
    {
        MMAP_Mothed1(u32Addr, u32Value);
        MMAP_Mothed2(u32Addr, u32Value);
        MMAP_Mothed3(u32Addr, u32Value);
    }

    HI_SYS_DeInit();
    return 0;
}


