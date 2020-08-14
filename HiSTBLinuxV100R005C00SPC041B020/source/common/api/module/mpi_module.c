#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

#include "hi_debug.h"
#include "hi_drv_struct.h"
#include "mpi_module.h"
#include "drv_module_ioctl.h"
#include "mpi_mem_base.h"

#ifdef CMN_MMGR_SUPPORT  
#include "mpi_mmgr.h"
#endif

#define MODULE_BASE_ID (HI_DEBUG_ID_BUTT)

///////////////////////////////////////////////////////////////////////////////
//// global variable definition
///////////////////////////////////////////////////////////////////////////////
static pthread_mutex_t g_ModuleMutex = PTHREAD_MUTEX_INITIALIZER;

#define MODULE_LOCK(x)                       \
do{                                                       \
    HI_S32 s32Ret = pthread_mutex_lock(x); 				  \
    if ( s32Ret != 0 )                                    \
    {													  \
    	HI_ERR_PRINT(HI_ID_MODULE, "MODULE_LOCK error\n"); \
    }                                                     \
}while(0)

#define MODULE_UNLOCK(x)                                  \
do{                                                       \
    HI_S32 s32Ret = pthread_mutex_unlock(x);              \
    if ( s32Ret != 0 )                                    \
    {    												  \
    	HI_ERR_PRINT(HI_ID_MODULE, "MODULE_UNLOCK error\n"); \
    }                                                     \
}while(0)


static HI_S32 g_s32ModuleFd = -1;

#define MODULE_OPEN_FILE(fd, name)                     \
do{                                                    \
    if (-1 == fd)                                      \
    {                                                  \
        fd = open("/dev/"name, O_RDWR);                \
        if (fd < 0)                                    \
        {                                              \
            MODULE_UNLOCK(&g_ModuleMutex);             \
            HI_ERR_PRINT(HI_ID_MODULE, "open %s failure\n", name); \
            return HI_FAILURE;                         \
        }                                              \
    }                                                  \
}while(0)

#define MODULE_CLOSE_FILE(fd)                          \
do{                                                    \
    if (-1 != fd)                                      \
    {                                                  \
        close(fd);                                     \
    }                                                  \
}while(0)


HI_S32 HI_MODULE_Init(HI_VOID)
{
	MODULE_LOCK(&g_ModuleMutex);

    if (g_s32ModuleFd != -1)
    {
		MODULE_UNLOCK(&g_ModuleMutex);
        return HI_SUCCESS;
    }
   
    //!! NOTES: the following MACRO maybe fail to return, in advance.
    MODULE_OPEN_FILE(g_s32ModuleFd, UMAP_DEVNAME_MODULE);
    
#ifdef CMN_MMGR_SUPPORT     
    Module_Mem_Adp_Init(MODULE_COUNT_MAX);
#endif

    MODULE_UNLOCK(&g_ModuleMutex);

    HI_INFO_PRINT(HI_ID_MODULE, "success and malloc module header the heap memory.\n");

    return HI_SUCCESS;
}

HI_S32 HI_MODULE_DeInit(HI_VOID)
{
    MODULE_LOCK(&g_ModuleMutex);
    
#ifdef CMN_MMGR_SUPPORT     
    Module_Mem_Adp_DeInit();
#endif

    MODULE_CLOSE_FILE(g_s32ModuleFd);

    g_s32ModuleFd = -1;

    MODULE_UNLOCK(&g_ModuleMutex);

    HI_INFO_PRINT(HI_ID_MODULE, "has been called and free the heap memory,successfully.\n");

    return HI_SUCCESS;
}

HI_S32 HI_MODULE_Register(HI_U32 u32ModuleID, const HI_CHAR * pszModuleName)
{
    HI_S32 s32Ret = HI_FAILURE;
    MODULE_INFO_S stModule = {0};
	
	MODULE_LOCK(&g_ModuleMutex);

    if (g_s32ModuleFd == -1 || NULL == pszModuleName)
    {
        MODULE_UNLOCK(&g_ModuleMutex);
        HI_ERR_PRINT(HI_ID_MODULE, "param invalid!\n");
        return HI_FAILURE;
    }

    memcpy(stModule.u8ModuleName, pszModuleName, sizeof(stModule.u8ModuleName));
    stModule.u32ModuleID = u32ModuleID;

    s32Ret = ioctl(g_s32ModuleFd,  CMD_ADD_MODULE_INFO, &stModule);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PRINT(HI_ID_MODULE, "Add module fail:%#x!\n", s32Ret);
        MODULE_UNLOCK(&g_ModuleMutex);
        return HI_FAILURE;
    }

    MODULE_UNLOCK(&g_ModuleMutex);
    HI_INFO_PRINT(HI_ID_MODULE, "stModule %s and module ID = 0x%08x\n",stModule.u8ModuleName, stModule.u32ModuleID);
    
#ifdef CMN_MMGR_SUPPORT 
    s32Ret = Module_Mem_AddModuleInfo(&stModule);

    if (s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }
    
#endif

    return HI_SUCCESS;
}

HI_S32 HI_MODULE_RegisterByName(const HI_CHAR * pszModuleName, HI_U32* pu32ModuleID)
{
    HI_S32 s32Ret = HI_FAILURE;
    MODULE_ALLOC_S stModule = {0};

    if ((NULL == pszModuleName) || (strlen(pszModuleName)==0) || (strlen(pszModuleName)>MAX_MODULE_NAME-1) || (NULL == pu32ModuleID))
    {
        HI_ERR_PRINT(HI_ID_MODULE, "name invalid!\n");
        return HI_FAILURE;
    }

    memcpy(stModule.u8ModuleName, pszModuleName, sizeof(stModule.u8ModuleName));

    if (g_s32ModuleFd == -1)
    {
        (HI_VOID)HI_MODULE_Init();
    }

    MODULE_LOCK(&g_ModuleMutex);
    s32Ret = ioctl(g_s32ModuleFd, CMD_ALLOC_MODULE_ID, &stModule);
    MODULE_UNLOCK(&g_ModuleMutex);
    
    if (HI_SUCCESS == s32Ret)
    {
        if (1 == stModule.s32Status)
        {
            *pu32ModuleID = stModule.u32ModuleID;
            HI_INFO_PRINT(HI_ID_MODULE, "HAD_REGISTERED %s %d\n", stModule.u8ModuleName, *pu32ModuleID);
            return HI_SUCCESS;
        }
        else if (0 == stModule.s32Status)
        {
            s32Ret = HI_MODULE_Register(stModule.u32ModuleID, (HI_CHAR*)stModule.u8ModuleName);
            if (HI_SUCCESS == s32Ret)
            {
                *pu32ModuleID = stModule.u32ModuleID;
                HI_INFO_PRINT(HI_ID_MODULE, "FIRST REGISTER %s %d\n", stModule.u8ModuleName, *pu32ModuleID);
            }
        }
    }

    return s32Ret;
}

HI_S32 HI_MODULE_UnRegister(HI_U32 u32ModuleID)
{
    HI_S32 s32Ret = HI_FAILURE;
    MODULE_INFO_S stModule = {0};
	
	MODULE_LOCK(&g_ModuleMutex);

    if (g_s32ModuleFd == -1)
    {
		MODULE_UNLOCK(&g_ModuleMutex);
        return HI_FAILURE;
    }

    stModule.u32ModuleID = u32ModuleID;
    
    s32Ret = ioctl(g_s32ModuleFd, CMD_GET_MODULE_INFO, &stModule);

    if (s32Ret != HI_SUCCESS)
    {
        MODULE_UNLOCK(&g_ModuleMutex);
        return s32Ret;
    }

#ifdef CMN_MMGR_SUPPORT
    if (stModule.s32RegCount == 1)
    {
        s32Ret = Module_Mem_DelModuleInfo(&stModule);
        
    }
#endif

    s32Ret = ioctl(g_s32ModuleFd, CMD_DEL_MODULE_INFO, &stModule);

    MODULE_UNLOCK(&g_ModuleMutex);

    return s32Ret;
}

HI_S32 HI_MODULE_GetModuleID(const HI_U8* pu8ModuleName)
{
    HI_S32 s32Ret = HI_FAILURE;
    MODULE_INFO_S stModule = {0};
	
	MODULE_LOCK(&g_ModuleMutex);

    if (g_s32ModuleFd == -1 || pu8ModuleName == NULL)
    {
		MODULE_UNLOCK(&g_ModuleMutex);
		HI_ERR_PRINT(HI_ID_MODULE, "param invalid!\n");
        return HI_FAILURE;
    }

    memcpy(stModule.u8ModuleName, pu8ModuleName, sizeof(stModule.u8ModuleName));
   
    s32Ret = ioctl(g_s32ModuleFd, CMD_GET_MODULE_INFO, &stModule);
    MODULE_UNLOCK(&g_ModuleMutex);

    if (HI_SUCCESS == s32Ret)
    {
        return (HI_S32)stModule.u32ModuleID;
    }

    return (HI_S32)HI_INVALID_MODULE_ID;
}

HI_U8* HI_MODULE_GetModuleName(HI_U32 u32ModuleID)
{
    HI_S32 s32Ret = HI_FAILURE;

    static MODULE_INFO_S stModule = {0};
	
	MODULE_LOCK(&g_ModuleMutex);

    if (g_s32ModuleFd == -1)
    {
		MODULE_UNLOCK(&g_ModuleMutex);
		
		HI_ERR_PRINT(HI_ID_MODULE, "deveice not open!\n");
		
        return NULL;
    }

    memset(&stModule, 0, sizeof(stModule));

    stModule.u32ModuleID = u32ModuleID;

    
    s32Ret = ioctl(g_s32ModuleFd, CMD_GET_MODULE_INFO, &stModule);
    MODULE_UNLOCK(&g_ModuleMutex);

    if (HI_SUCCESS == s32Ret)
    {
        return stModule.u8ModuleName;
    }

    return NULL;
}

