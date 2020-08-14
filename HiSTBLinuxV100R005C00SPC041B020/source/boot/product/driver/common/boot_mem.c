#include <uboot.h>
#include "hi_common.h"
#include "hi_type.h"

#ifndef HI_MINIBOOT_SUPPORT
extern void *reserve_mem_alloc(unsigned int size, unsigned int *allocsize);
#endif

HI_S32 HI_MEM_Alloc(HI_U32 *pu32PhyAddr, HI_U32 Len)
{
    HI_U32      AllocSize;
    
    //*pu32PhyAddr = (HI_U32)malloc(Len);
    *pu32PhyAddr = (HI_U32)reserve_mem_alloc(Len, &AllocSize);
    if (HI_NULL != *pu32PhyAddr) 
        return HI_SUCCESS;
    else
        return HI_FAILURE;
}

HI_VOID HI_MEM_Free(HI_VOID *pPhyAddr)
{
#ifdef HI_MINIBOOT_SUPPORT
    resfree(pPhyAddr);
#endif
    return;
}

#if 0
static MMZ_INFO_S       g_MmzInfo = 
{
    .name       = {""},
    .gfp        = 0,
    .startAddr  = 0,
    .size       = 0
};

static HI_U8            *g_pAllocPos = HI_NULL;

static unsigned int parse_mem(char *cmd)
{
    char c;
    char *s = NULL;
    unsigned int l;

    // 0
    s = strstr(cmd, "mem=");
    if (!s)
    {
        HI_ERR_COMMON("not mem args ");
        return 0;
    }

    // 1
    l = 0;
    c = *(s + 4 + l);
    while ((c != '\0') && (c != 'M') && (c != 'G'))
    {
        l++;
        c = *(s + 4 + l);
    }

    if (!l)
    {
        HI_ERR_COMMON("mem args args inValid");
        return 0;
    }

    *(s + 4 + l) = '\0';

    // 2
    l = simple_strtoul_2(s + 4, NULL, 10);

    if (c == 'G')
    {
        l *= 1024;
    }
    
    return l;
}

/*such as:  mmz=ddr,0,0x86000000,160M*/
static int parse_mmz(char *cmd)
{
    char *p = NULL;
    char *s = NULL;

    s = strstr(cmd, "mmz=");
    if (!s)
    {
        strncpy(g_MmzInfo.name, "ddr", strlen("ddr")+1);
        g_MmzInfo.gfp = 0;
        g_MmzInfo.startAddr = 0x86000000;
        g_MmzInfo.size = 0xa000000;
        return 0;
    }

    p = strtok(s, "=");
    if (!p)
    {
        HI_ERR_COMMON("mmz args args inValid");
        return -1;
    }

    p = strtok(NULL, ",");
    if (!p)
    {
        HI_ERR_COMMON("mmz args args inValid");
        return -1;
    }

    strncpy(g_MmzInfo.name, p, strlen(p)+1);

    p = strtok(NULL, ",");
    if (!p)
    {
        HI_ERR_COMMON("mmz args args inValid");
        return -1;
    }

    g_MmzInfo.gfp = simple_strtoul_2(p, NULL, 16);

    p = strtok(NULL, ",");
    if (!p)
    {
        HI_ERR_COMMON("mmz args args inValid");
        return -1;
    }

    g_MmzInfo.startAddr = simple_strtoul_2(p, NULL, 16);

    p = strtok(NULL, ",");
    if (!p)
    {
        HI_ERR_COMMON("mmz args args inValid");
        return -1;
    }

    g_MmzInfo.size  = simple_strtoul_2(p, NULL, 10);
    g_MmzInfo.size *= 0x100000;

    HI_INFO_COMMON("g_MmzInfo startAddr = 0x%x size:%d\n", g_MmzInfo.startAddr, g_MmzInfo.size);
    return 0;
}

HI_S32 HI_MMZ_Init()
{
    HI_S32      Ret;
    HI_CHAR     *pTmp = HI_NULL;
    HI_CHAR     Bootarg[BOOT_ARGS_SIZE] = {0};
    HI_U32      mem;
    HI_U32      stride;
    HI_U32      tmp;

    pTmp = getenv("bootargs");

    strncpy(Bootarg, pTmp, BOOT_ARGS_SIZE);

    mem = parse_mem(Bootarg);
    tmp = (HI_U32)malloc(1);
    stride = (tmp - MEM_BASE) / (1024 * 1024);
    if(mem < stride)
    {
        HI_ERR_COMMON("!!! mem=%d too smal for fastplay which should > %d\n", mem, stride);
	    free((HI_U32 *)tmp);
        return HI_FAILURE;
    }

    strncpy(Bootarg, pTmp, BOOT_ARGS_SIZE);
    Ret = parse_mmz(Bootarg);
    if(HI_SUCCESS != Ret)
    {
        free((HI_U32 *)tmp);
        return HI_FAILURE;
    }

    g_pAllocPos = (HI_U8 *)(g_MmzInfo.startAddr + g_MmzInfo.size - HI_MMZ_INFOZONE_SIZE);

    free((HI_U32 *)tmp);
    return HI_SUCCESS;
}

HI_S32 HI_MMZ_DeInit()
{
    g_pAllocPos = HI_NULL;
    
    return HI_SUCCESS;
}

HI_S32 HI_MMZ_GetInfoZoneAddr(MMZ_BUFFER_S *psMBuf)
{
    psMBuf->u32StartPhyAddr = g_MmzInfo.startAddr + g_MmzInfo.size - HI_MMZ_INFOZONE_SIZE;
    psMBuf->u32StartVirAddr = g_MmzInfo.startAddr + g_MmzInfo.size - HI_MMZ_INFOZONE_SIZE;
    psMBuf->u32Size = HI_MMZ_INFOZONE_SIZE;

    return HI_SUCCESS;
}

HI_S32 HI_MMZ_Alloc(const char *bufname, char *zone_name, unsigned int size, int align, MMZ_BUFFER_S *psMBuf)
{
    HI_U32      AlignSize;

    if(HI_NULL == g_pAllocPos)
    {
        HI_ERR_COMMON("ERR: init mmz first!\n");
        return HI_FAILURE;
    }

    /*if the malloc memory is not align to 8K, align it*/
    /*CNComment: 如果分配的内存大小不是对齐到8K的，进行对齐处理*/
    if(0x00 != (size & 0x000000ff))
    {
        AlignSize = (size | 0xff) + 1;
    }
    else
    {
        AlignSize = size;
    }

    /*not enough mmz memory*/
    /*CNComment: MMZ内存已经分完*/
    if((HI_U32)g_pAllocPos - AlignSize <= g_MmzInfo.startAddr)
    {
        HI_ERR_COMMON("ERR: mmz is not enough!\n");
        return HI_FAILURE;
    }

    g_pAllocPos -= AlignSize;

    HI_INFO_COMMON("INFO: bufname: %s, g_pAllocPos: %#x, size: %#x, AlignSize : %#x\n", 
    bufname, g_pAllocPos, size, AlignSize);

    psMBuf->u32StartPhyAddr = (HI_U32)g_pAllocPos;
    psMBuf->u32StartVirAddr = (HI_U32)g_pAllocPos;
    psMBuf->u32Size = size;
    
    return HI_SUCCESS;
}

void HI_MMZ_Release(MMZ_BUFFER_S *psMBuf)
{
    return;
}

#endif
