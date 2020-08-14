#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mem.h"
#include "hi_drv_struct.h"
#include "mpi_mmgr.h"
#include "mpi_mem_base.h"


#define MODULE_BASE_ID (HI_ID_BUTT)

#define MEM_ADD_DEL(flag) ( (flag) > 0 ? (1) : (-1))


HI_HANDLE g_hMemAdp = 0;

HI_S32 g_s32SMemFd = -1;

#define MEM_OPEN_FILE(fd, name)                                             \
do{                                                                         \
    if (-1 == fd)                                                           \
    {                                                                       \
        fd = open("/dev/"name, O_RDWR);                                     \
        if (fd < 0)                                                         \
        {                                                                   \
            HI_ERR_MEM("open %s failure, %s\n", name, strerror(errno)); \
            return HI_FAILURE;                                              \
        }                                                                   \
    }                                                                       \
}while(0)

#define MEM_CLOSE_FILE(fd)                                                  \
do{                                                                         \
    if (-1 != fd)                                                           \
    {                                                                       \
        close(fd);                                                          \
    }                                                                       \
}while(0)


#ifdef CMN_MMGR_SUPPORT
static HI_S32 Mem_UpdataInfo(HI_U32 u32ModuleID, MEM_TYPE_E enMemType, HI_S32 s32Size)
{
    HI_S32 s32Ret = HI_FAILURE;
    MODULE_MEM_INFO_S stModuleMem = {0};

    stModuleMem.u32ModuleID = u32ModuleID;

    switch(enMemType)
    {
        case MEM_TYPE_MMZ:
            stModuleMem.u32SizeMMZ= s32Size*MEM_ADD_DEL(s32Size);
            HI_INFO_MEM("find the module ID 0x%08x and op mmz memory size:%d\n", u32ModuleID, s32Size);
        break;
        case MEM_TYPE_USR:
            stModuleMem.u32SizeUsrMem = s32Size*MEM_ADD_DEL(s32Size);
            HI_INFO_MEM("find the module ID 0x%08x and op user memory size:%d\n", u32ModuleID, s32Size);
        break;
        case MEM_TYPE_KERNEL:
            stModuleMem.u32SizeKernelMem = s32Size*MEM_ADD_DEL(s32Size);
            HI_INFO_MEM("find the module ID 0x%08x and op kernel memory size:%d\n", u32ModuleID, s32Size);
        break;
        default:
        break;
    }

    if (s32Size < 0)
    {
        s32Ret = ioctl(g_s32SMemFd, CMD_MEM_DEL_INFO, &stModuleMem);
    }
    else
    {
        s32Ret = ioctl(g_s32SMemFd, CMD_MEM_ADD_INFO, &stModuleMem);
    }

    return s32Ret;
}
#endif

HI_S32 Module_Mem_Adp_Init(HI_U32 u32ModuleCount)
{
    MEM_OPEN_FILE(g_s32SMemFd, UMAP_DEVNAME_MEM2);

#ifdef CMN_MMGR_SUPPORT  
    MEM_POOL_Init(MEM_POOL_COUNT, Mem_UpdataInfo);
#endif

    return HI_SUCCESS;
}

HI_S32 Module_Mem_Adp_DeInit(HI_VOID)
{
    MEM_CLOSE_FILE(g_s32SMemFd);

    g_s32SMemFd = -1;

#ifdef CMN_MMGR_SUPPORT
    MEM_POOL_DeInit();
#endif
    return HI_SUCCESS;
}

HI_S32 Module_Mem_AddModuleInfo(MODULE_INFO_S* pstModule)
{
    HI_S32 s32Ret = HI_FAILURE;
    
    s32Ret = ioctl(g_s32SMemFd, CMD_ADD_MODULE_INFO, pstModule);

    if (HI_FAILURE == s32Ret)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 Module_Mem_DelModuleInfo(MODULE_INFO_S* pstModule)
{
    HI_S32 s32Ret = HI_FAILURE;
    
    s32Ret = ioctl(g_s32SMemFd, CMD_DEL_MODULE_INFO, pstModule);

    if (HI_FAILURE == s32Ret)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}


