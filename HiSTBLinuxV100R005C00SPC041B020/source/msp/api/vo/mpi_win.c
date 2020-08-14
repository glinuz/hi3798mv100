/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_vo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/17
  Description   :
  History       :
  1.Date        : 2009/12/17
    Author      : w58735
    Modification: Created file

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <linux/types.h>

#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "hi_mpi_win.h"
#include "drv_win_ioctl.h"

#include "hi_mpi_avplay.h"
#include "hi_error_mpi.h"
#include "hi_drv_struct.h"
#include "hi_mpi_mem.h"
#include "mpi_mmz.h"

#include "drv_venc_ext.h"
#include "win_ratio.h"

#ifdef HI_VDP_VIDEO_LINEDCMP
#include "./vo_algbase/src/dcmp.h"
#endif
#ifdef HI_VDP_HDR_SUPPORT
#include "dolby_lib_api.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif


static HI_S32           g_VoDevFd = -1;
static const HI_CHAR    g_VoDevName[] = "/dev/"UMAP_DEVNAME_VO;
static pthread_mutex_t  g_VoMutex = PTHREAD_MUTEX_INITIALIZER;

#ifdef HI_VDP_HDR_SUPPORT
#define DOLBY_BUF_SIZE        (64 * 1024)
#define DOLBY_DMKS_BUF_SIZE   (sizeof(DmKsFxp_t))

static pthread_mutex_t  g_DolbyLibMutex = PTHREAD_MUTEX_INITIALIZER;
#define HI_DOLBY_LIB_LOCK()    (void)pthread_mutex_lock(&g_DolbyLibMutex);
#define HI_DOLBY_LIB_UNLOCK()  (void)pthread_mutex_unlock(&g_DolbyLibMutex);  

static HI_MMZ_BUF_S stDolbyBuf;
static HI_MMZ_BUF_S stDolbyDmKsBuf;

static HI_VOID* pDllModule = HI_NULL;
static HI_U32   s_u32DolbyLibInitCount = 0;

#ifdef ANDROID
static HI_CHAR* pszDolbyDllName = "/system/lib/libdolbyvision.so";
#else
static HI_CHAR* pszDolbyDllName = "/usr/lib/libdolbyvision.so";
#endif

static DOLBY_FUNC_ENTRY_S s_DolbyFuncEntry = {0};

#endif

#define HI_VO_LOCK()     (void)pthread_mutex_lock(&g_VoMutex);
#define HI_VO_UNLOCK()   (void)pthread_mutex_unlock(&g_VoMutex);

#define CHECK_VO_INIT()\
do{\
    HI_VO_LOCK();\
    if (g_VoDevFd < 0)\
    {\
        HI_ERR_WIN("VO is not init.\n");\
        HI_VO_UNLOCK();\
        return HI_ERR_VO_NO_INIT;\
    }\
    HI_VO_UNLOCK();\
}while(0)

#define CHECK_VO_NULL_PTR(a)\
    do{\
        if (HI_NULL == (a))\
        {\
            HI_ERR_WIN("para %s is null.\n", #a);\
            return HI_ERR_VO_NULL_PTR;\
        }\
    }while(0)

#ifdef HI_VDP_HDR_SUPPORT
HI_VOID  HI_MPI_WIN_RegisterAndGetDolbyLibInfo(WIN_DOLBY_LIB_INFO_S *pstDolbyLibInfo)
{
    HI_S32  s32Ret = HI_FAILURE;   
	dolby_vision_info_t dolby_vision_info;

    memset(&dolby_vision_info,0x0,sizeof(dolby_vision_info_t));
    
    pDllModule = dlopen(pszDolbyDllName, RTLD_LAZY | RTLD_GLOBAL);
    if (HI_NULL == pDllModule)
    {
        HI_WARN_WIN("Register %s fail:%s.\n", pszDolbyDllName, dlerror());
        memset(&s_DolbyFuncEntry, 0x0, sizeof(s_DolbyFuncEntry));
        pstDolbyLibInfo->bValid = HI_FALSE;
        return;
    }
    
    s_DolbyFuncEntry.pfnMetadata_parser_init    = dlsym(pDllModule, "metadata_parser_init");
    s_DolbyFuncEntry.pfnMetadata_parser_deinit  = dlsym(pDllModule, "metadata_parser_deinit");
    s_DolbyFuncEntry.pfnMetadata_parser_process = dlsym(pDllModule, "metadata_parser_process");
    s_DolbyFuncEntry.pfnBypass_metadata_update  = dlsym(pDllModule, "bypass_metadata_update");
    s_DolbyFuncEntry.pfnReg_init                = dlsym(pDllModule, "reg_init");
    s_DolbyFuncEntry.pfnReg_deinit              = dlsym(pDllModule, "reg_deinit");
    s_DolbyFuncEntry.pfnCommit_reg              = dlsym(pDllModule, "commit_reg");
    s_DolbyFuncEntry.pfnSet_display_config      = dlsym(pDllModule, "set_display_config");
    s_DolbyFuncEntry.pfnSet_grc_config          = dlsym(pDllModule, "set_grc_config");
	s_DolbyFuncEntry.pfnget_dolby_vision_info   = dlsym(pDllModule, "get_dolby_vision_info");
    
    if ((HI_NULL == s_DolbyFuncEntry.pfnMetadata_parser_init)
        || (HI_NULL == s_DolbyFuncEntry.pfnMetadata_parser_deinit)            
        || (HI_NULL == s_DolbyFuncEntry.pfnReg_init)
        || (HI_NULL == s_DolbyFuncEntry.pfnReg_deinit)
        || (HI_NULL == s_DolbyFuncEntry.pfnMetadata_parser_process)
        || (HI_NULL == s_DolbyFuncEntry.pfnSet_display_config)
        || (HI_NULL == s_DolbyFuncEntry.pfnSet_grc_config)
        || (HI_NULL == s_DolbyFuncEntry.pfnCommit_reg)
        || (HI_NULL == s_DolbyFuncEntry.pfnBypass_metadata_update)
        || (HI_NULL == s_DolbyFuncEntry.pfnget_dolby_vision_info)
        )
    {
        HI_ERR_WIN("Open s_DolbyFuncEntry functions fail.\n");
        memset(&s_DolbyFuncEntry, 0x0, sizeof(s_DolbyFuncEntry));
        pstDolbyLibInfo->bValid = HI_FALSE;
        return;
    }
    
    s32Ret = s_DolbyFuncEntry.pfnget_dolby_vision_info(&dolby_vision_info);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_WIN("Get dolbylib info fail.\n");
        memset(&s_DolbyFuncEntry, 0x0, sizeof(s_DolbyFuncEntry));
        pstDolbyLibInfo->bValid = HI_FALSE;
        return;
    }
    
    strncpy(pstDolbyLibInfo->cVersion, dolby_vision_info.pversion, sizeof(pstDolbyLibInfo->cVersion));
    strncpy(pstDolbyLibInfo->cBuildtime, dolby_vision_info.pbuild_time, sizeof(pstDolbyLibInfo->cBuildtime));
    strncpy(pstDolbyLibInfo->cUserName, dolby_vision_info.puser_name, sizeof(pstDolbyLibInfo->cUserName));
    pstDolbyLibInfo->u32UserID[0] = dolby_vision_info.user_id[0];
    pstDolbyLibInfo->u32UserID[1] = dolby_vision_info.user_id[1];
    pstDolbyLibInfo->u32UserID[2] = dolby_vision_info.user_id[2];
    pstDolbyLibInfo->u32UserID[3] = dolby_vision_info.user_id[3];
    pstDolbyLibInfo->bValid = HI_TRUE;
    
    return;
}

HI_S32  HI_MPI_WIN_AllocateDolbyBuf(HI_MMZ_BUF_S *pstDolbyVisionBuf,
                                    HI_MMZ_BUF_S *pstDolbyDmKsBuf)
{
    if ((HI_NULL == pstDolbyVisionBuf)
        || (HI_NULL == pstDolbyDmKsBuf))
    {
        return HI_ERR_VO_NULL_PTR;
    }
	
    if ((HI_NULL != pstDolbyVisionBuf->phyaddr)
        && (HI_NULL != pstDolbyDmKsBuf->phyaddr))
    {
        HI_ERR_WIN("dolby buffer has been allocated!\n");
        return HI_ERR_VO_MALLOC_FAILED;
    }
    
    memset(pstDolbyVisionBuf,0x0,sizeof(HI_MMZ_BUF_S));
    memset(pstDolbyDmKsBuf,0x0,sizeof(HI_MMZ_BUF_S));
	
    strncpy(pstDolbyVisionBuf->bufname, "VDP_DolbyVisionBuf", sizeof(pstDolbyVisionBuf->bufname));
    pstDolbyVisionBuf->bufsize = DOLBY_BUF_SIZE;
    
    strncpy(pstDolbyDmKsBuf->bufname, "VDP_DolbyDmKsBuf", sizeof(pstDolbyDmKsBuf->bufname));
    pstDolbyDmKsBuf->bufsize = DOLBY_DMKS_BUF_SIZE;
    
#ifdef HI_SMMU_SUPPORT
    pstDolbyVisionBuf->phyaddr = HI_MPI_SMMU_New(pstDolbyVisionBuf->bufname, pstDolbyVisionBuf->bufsize);
    if (HI_NULL == pstDolbyVisionBuf->phyaddr )
    {
        HI_FATAL_WIN("HI_MPI_SMMU_New stDolbyBuf %d failed!\n", pstDolbyVisionBuf->bufsize);
        return HI_ERR_VO_MALLOC_FAILED;
    }
    
    pstDolbyVisionBuf->user_viraddr = (HI_U8*)HI_MPI_SMMU_Map(pstDolbyVisionBuf->phyaddr, HI_FALSE);
    if (HI_NULL == pstDolbyVisionBuf->user_viraddr )
    {
        HI_FATAL_WIN("HI_MPI_SMMU_Map stDolbyBuf %d failed!\n", pstDolbyVisionBuf->bufsize);
        return HI_ERR_VO_NULL_PTR;
    }
        
    pstDolbyDmKsBuf->phyaddr = HI_MPI_SMMU_New(pstDolbyDmKsBuf->bufname, pstDolbyDmKsBuf->bufsize);
    if (HI_NULL == pstDolbyDmKsBuf->phyaddr )
    {
        HI_FATAL_WIN("HI_MPI_SMMU_New stDolbyDmKsBuf %d failed!\n", pstDolbyDmKsBuf->bufsize);
        return HI_ERR_VO_MALLOC_FAILED;
    }
    
    pstDolbyDmKsBuf->user_viraddr = (HI_U8*)HI_MPI_SMMU_Map(pstDolbyDmKsBuf->phyaddr, HI_FALSE);
    if (HI_NULL == pstDolbyDmKsBuf->user_viraddr )
    {
        HI_FATAL_WIN("HI_MPI_SMMU_Map stDolbyDmKsBuf %d failed!\n", pstDolbyDmKsBuf->bufsize);
        return HI_ERR_VO_NULL_PTR;
    }
#else
    if (HI_MPI_MMZ_Malloc(pstDolbyVisionBuf) != HI_SUCCESS)
    {
        HI_FATAL_WIN("HI_MPI_MMZ_Malloc stDolbyBuf %d failed!\n", pstDolbyVisionBuf->bufsize);
        return HI_ERR_VO_DEV_OPEN_ERR;

    }

    if (HI_MPI_MMZ_Malloc(pstDolbyDmKsBuf) != HI_SUCCESS)
    {
        HI_FATAL_WIN("HI_MPI_MMZ_Malloc stDolbyDmKsBuf %d failed!\n", pstDolbyDmKsBuf->bufsize);
        return HI_ERR_VO_DEV_OPEN_ERR;
    }
#endif
    return HI_SUCCESS;
}

HI_VOID HI_MPI_WIN_ReleaseDolbyBuf(HI_MMZ_BUF_S *pstDolbyVisionBuf,
                                   HI_MMZ_BUF_S *pstDolbyDmKsBuf)
{
    if ((HI_NULL == pstDolbyVisionBuf)
        || (HI_NULL == pstDolbyDmKsBuf))
    {
        return;
    }
#ifdef HI_SMMU_SUPPORT
    if (HI_NULL != pstDolbyVisionBuf->phyaddr)
    {
        if(HI_NULL != pstDolbyVisionBuf->user_viraddr)
        {
            HI_MPI_SMMU_Unmap(pstDolbyVisionBuf->phyaddr);
        }
        HI_MPI_SMMU_Delete(pstDolbyVisionBuf->phyaddr);
    }

    if (HI_NULL != pstDolbyDmKsBuf->phyaddr)
    {
        if(HI_NULL != pstDolbyDmKsBuf->user_viraddr)
        {
            HI_MPI_SMMU_Unmap(pstDolbyDmKsBuf->phyaddr);
        }
        HI_MPI_SMMU_Delete(pstDolbyDmKsBuf->phyaddr);
    }
#else
    if (HI_NULL != pstDolbyVisionBuf->phyaddr)
    {
        HI_MPI_MMZ_Free(pstDolbyVisionBuf);
    }
    
    if (HI_NULL != pstDolbyDmKsBuf->phyaddr)
    {
        HI_MPI_MMZ_Free(pstDolbyDmKsBuf);
    }
    
#endif
    memset(pstDolbyVisionBuf, 0x0, sizeof(HI_MMZ_BUF_S));
    memset(pstDolbyDmKsBuf, 0x0, sizeof(HI_MMZ_BUF_S));
    return;
}
#endif

HI_S32 HI_MPI_WIN_Init(HI_VOID)
{
    HI_VO_LOCK();

    if (g_VoDevFd > 0)
    {
        HI_VO_UNLOCK();
        return HI_SUCCESS;
    }

    g_VoDevFd = open(g_VoDevName, O_RDWR|O_NONBLOCK, 0);

    if (g_VoDevFd < 0)
    {
        HI_FATAL_WIN("open VO err.\n");
        HI_VO_UNLOCK();
        return HI_ERR_VO_DEV_OPEN_ERR;
    }
	
#ifdef HI_VDP_HDR_SUPPORT
    HI_S32 Ret = HI_FAILURE;
	WIN_DOLBY_LIB_INFO_S stDolbyLibInfo;
    
	memset(&stDolbyLibInfo, 0, sizeof(stDolbyLibInfo));

    HI_MPI_WIN_RegisterAndGetDolbyLibInfo(&stDolbyLibInfo);
    
    Ret = HI_MPI_WIN_AllocateDolbyBuf(&stDolbyBuf,&stDolbyDmKsBuf);
    if (HI_SUCCESS != Ret)
    {
		HI_FATAL_WIN("HI_MPI_WIN_AllocateDolbyBuf failed!\n");
        goto ERR_PROCESS_RELEASE_RESOURCE;
    }

	Ret = HI_MPI_WIN_SetDolbyLibInfo(&stDolbyLibInfo);
	if (HI_SUCCESS != Ret)
	{        
		HI_FATAL_WIN("HI_MPI_WIN_SetDolbyLibInfo failed!\n");
		goto ERR_PROCESS_RELEASE_RESOURCE;
	}

#endif

    HI_VO_UNLOCK();

    return HI_SUCCESS;
    
#ifdef HI_VDP_HDR_SUPPORT
ERR_PROCESS_RELEASE_RESOURCE: 
    HI_MPI_WIN_ReleaseDolbyBuf(&stDolbyBuf,&stDolbyDmKsBuf);
    
    close(g_VoDevFd);
	g_VoDevFd = -1;
	HI_VO_UNLOCK();
	return Ret;
#endif    
}

HI_S32 HI_MPI_WIN_DeInit(HI_VOID)
{
    HI_S32 Ret;

    HI_VO_LOCK();

    if (g_VoDevFd < 0)
    {
        HI_VO_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = close(g_VoDevFd);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_WIN("DeInit VO err.\n");
        HI_VO_UNLOCK();
        return HI_ERR_VO_DEV_CLOSE_ERR;
    }

    g_VoDevFd = -1;

#ifdef HI_VDP_HDR_SUPPORT
    HI_MPI_WIN_ReleaseDolbyBuf(&stDolbyBuf,&stDolbyDmKsBuf);
#endif

    HI_VO_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Create(const HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWindow)
{
    HI_S32          Ret = HI_SUCCESS;
    WIN_CREATE_S    VoWinCreate;

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (!phWindow)
    {
        HI_ERR_WIN("para phWindow is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (pWinAttr->enDisp >= HI_DRV_DISPLAY_BUTT
        && pWinAttr->bVirtual == HI_FALSE)
    {
        HI_ERR_WIN("para pWinAttr->enVo is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (pWinAttr->enARCvrs >= HI_DRV_ASP_RAT_MODE_BUTT)
    {
        HI_ERR_WIN("para pWinAttr->enAspectCvrs is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    memcpy(&VoWinCreate.WinAttr, pWinAttr, sizeof(HI_DRV_WIN_ATTR_S));
    VoWinCreate.bVirtScreen = HI_TRUE;
	VoWinCreate.bMCE = HI_FALSE;

    Ret = ioctl(g_VoDevFd, CMD_WIN_CREATE, &VoWinCreate);
    if (Ret != HI_SUCCESS)
    {
		HI_ERR_WIN("  HI_MPI_WIN_Create failed.\n");
        return Ret;
    }

    *phWindow = VoWinCreate.hWindow;
    
    /* DTS2016032107380 Init dolby lib here instead of in WIN_INT.Android may create  
    * different wins while ONLY call one time WIN_INT, thus may cause error status of 
    * dolby lib.count the time of initial dolby lib when create win successfully, this  
    * count will be used when de-init dolby lib when destory win in multi-wins situation.*/
#ifdef HI_VDP_HDR_SUPPORT
    if ((s_DolbyFuncEntry.pfnMetadata_parser_init)
        && (s_DolbyFuncEntry.pfnMetadata_parser_deinit)
        && (s_DolbyFuncEntry.pfnReg_init))
    {        
        HI_DOLBY_LIB_LOCK();
        Ret = s_DolbyFuncEntry.pfnMetadata_parser_init();
        if (Ret)
        {
            HI_ERR_WIN("metadata_parser_init fail.\n");
            HI_DOLBY_LIB_UNLOCK();
            return Ret;
        }
    
        Ret = s_DolbyFuncEntry.pfnReg_init();
        if (Ret)
        {
            HI_ERR_WIN("reg_init fail.\n");
			(HI_VOID)s_DolbyFuncEntry.pfnMetadata_parser_deinit();
            HI_DOLBY_LIB_UNLOCK();
            return Ret;
        }

        /* record init times for de-init dolby lib when destory win. */
        s_u32DolbyLibInitCount++;
        HI_DOLBY_LIB_UNLOCK();
    }
#endif
    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Create_Ext(const HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWindow, HI_BOOL bVirtScreen)
{
    HI_S32           Ret = HI_SUCCESS;;
    WIN_CREATE_S  VoWinCreate;

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (!phWindow)
    {
        HI_ERR_WIN("para phWindow is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (pWinAttr->enDisp >= HI_DRV_DISPLAY_BUTT
        && pWinAttr->bVirtual == HI_FALSE)
    {
        HI_ERR_WIN("para pWinAttr->enVo is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (pWinAttr->enARCvrs >= HI_DRV_ASP_RAT_MODE_BUTT)
    {
        HI_ERR_WIN("para pWinAttr->enAspectCvrs is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    memcpy(&VoWinCreate.WinAttr, pWinAttr, sizeof(HI_DRV_WIN_ATTR_S));
    VoWinCreate.bVirtScreen = bVirtScreen;

    Ret = ioctl(g_VoDevFd, CMD_WIN_CREATE, &VoWinCreate);
    if (Ret != HI_SUCCESS)
    {
		HI_ERR_WIN("  HI_MPI_WIN_Create failed.\n");
        return Ret;
    }

    *phWindow = VoWinCreate.hWindow;

   /* DTS2016032107380 Init dolby lib here instead of in WIN_INT.Android may create  
   * different wins while ONLY call one time WIN_INT, thus may cause error status of 
   * dolby lib.count the time of initial dolby lib when create win successfully, this  
   * count will be used when de-init dolby lib when destory win in multi-wins situation.*/
#ifdef HI_VDP_HDR_SUPPORT
    if ((s_DolbyFuncEntry.pfnMetadata_parser_init)
        && (s_DolbyFuncEntry.pfnMetadata_parser_deinit)
        && (s_DolbyFuncEntry.pfnReg_init))
    {
        HI_DOLBY_LIB_LOCK();
        Ret = s_DolbyFuncEntry.pfnMetadata_parser_init();
        if (Ret)
        {
            HI_ERR_WIN("metadata_parser_init fail.\n");
            HI_DOLBY_LIB_UNLOCK();
            return Ret;
        }
    
        Ret = s_DolbyFuncEntry.pfnReg_init();
        if (Ret)
        {
            HI_ERR_WIN("reg_init fail.\n");
			(HI_VOID)s_DolbyFuncEntry.pfnMetadata_parser_deinit();
            HI_DOLBY_LIB_UNLOCK();
            return Ret;
        }
        
        /* record init times for de-init dolby lib when destory win. */
        s_u32DolbyLibInitCount++;
        HI_DOLBY_LIB_UNLOCK();
    }
#endif

    return HI_SUCCESS;
}


HI_S32 HI_MPI_WIN_Destroy(HI_HANDLE hWindow)
{
    HI_S32      Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_DESTROY, &hWindow);
    if (HI_SUCCESS != Ret)
    {
		HI_ERR_WIN("  HI_MPI_WIN_Destroy failed.\n");
        return Ret;
    }

#ifdef HI_VDP_HDR_SUPPORT
    HI_DOLBY_LIB_LOCK();
    /* It may initial dolby lib many times when create multi-wins, in case of the last
     * win may use for DolbyVision, de-init dolby lib operation CANNOT be done until destory
     * the last win.*/
    if(1 == s_u32DolbyLibInitCount)
    {
        if ((s_DolbyFuncEntry.pfnMetadata_parser_deinit)
            && (s_DolbyFuncEntry.pfnReg_deinit))
        {
            (HI_VOID)s_DolbyFuncEntry.pfnReg_deinit();
            (HI_VOID)s_DolbyFuncEntry.pfnMetadata_parser_deinit();
        }
    }
    
    /* remember to decrease init count. */
     s_u32DolbyLibInitCount --;
    HI_DOLBY_LIB_UNLOCK();
#endif

    return Ret;
}

HI_S32 HI_MPI_WIN_GetInfo(HI_HANDLE hWin, HI_DRV_WIN_INFO_S * pstInfo)
{
    HI_S32 Ret;
    WIN_PRIV_INFO_S WinPriv;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_NULL_PTR(pstInfo);
    CHECK_VO_INIT();

    WinPriv.hWindow = hWin;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_INFO, &WinPriv);
	if (HI_SUCCESS == Ret)
	{
		*pstInfo = WinPriv.stPrivInfo;
	}

    return Ret;
}

HI_S32 HI_MPI_WIN_GetPlayInfo(HI_HANDLE hWin, HI_DRV_WIN_PLAY_INFO_S * pstInfo)
{
    HI_S32 Ret = HI_SUCCESS;
    WIN_PLAY_INFO_S WinPlay;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    
    CHECK_VO_NULL_PTR(pstInfo);
    CHECK_VO_INIT();

    WinPlay.hWindow = hWin;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_PLAY_INFO, &WinPlay);
	if (HI_SUCCESS == Ret)
	{
		*pstInfo = WinPlay.stPlayInfo;
	}

    return Ret;
}


HI_S32 HI_MPI_WIN_SetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_HANDLE_S *pstSrc)
{
    HI_S32 Ret;
    WIN_SOURCE_S VoWinAttach;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    CHECK_VO_NULL_PTR(pstSrc);
    CHECK_VO_INIT();

    VoWinAttach.hWindow = hWin;
    VoWinAttach.stSrc   = *pstSrc;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_SOURCE, &VoWinAttach);

    return Ret;
}

HI_S32 HI_MPI_WIN_GetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_HANDLE_S *pstSrc)
{
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_SetEnable(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    HI_S32            Ret;
    WIN_ENABLE_S   VoWinEnable;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if ((bEnable != HI_TRUE) &&(bEnable != HI_FALSE))
    {
        HI_ERR_WIN("para bEnable is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinEnable.hWindow = hWindow;
    VoWinEnable.bEnable = bEnable;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_ENABLE, &VoWinEnable);

    return Ret;
}


HI_S32 HI_MPI_VO_SetMainWindowEnable(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    return HI_FAILURE;
}


HI_S32 HI_MPI_WIN_GetEnable(HI_HANDLE hWindow, HI_BOOL *pbEnable)
{
    HI_S32          Ret = HI_SUCCESS;
    WIN_ENABLE_S    VoWinEnable;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    
    CHECK_VO_NULL_PTR(pbEnable);
    CHECK_VO_INIT();

    memset((HI_VOID*)&VoWinEnable, 0, sizeof(WIN_ENABLE_S));
    VoWinEnable.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_ENABLE, &VoWinEnable);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pbEnable = VoWinEnable.bEnable;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VO_GetMainWindowEnable(HI_HANDLE hWindow, HI_BOOL *pbEnable)
{
    return HI_FAILURE;
}


HI_S32 HI_MPI_VO_GetWindowsVirtual(HI_HANDLE hWindow, HI_BOOL *pbVirutal)
{
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_AcquireFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    HI_S32           Ret = HI_SUCCESS;
    WIN_FRAME_S      VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    
    CHECK_VO_NULL_PTR(pFrameinfo);
    CHECK_VO_INIT();

    memset((HI_VOID*)&VoWinFrame, 0, sizeof(WIN_FRAME_S));    
    VoWinFrame.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_VIR_ACQUIRE, &VoWinFrame);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pFrameinfo, &(VoWinFrame.stFrame), sizeof(HI_DRV_VIDEO_FRAME_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_ReleaseFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    HI_S32           Ret = HI_SUCCESS;
    WIN_FRAME_S      VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_NULL_PTR(pFrameinfo);
    CHECK_VO_INIT();

    memset((HI_VOID*)&VoWinFrame, 0, sizeof(WIN_FRAME_S)); 
    
    VoWinFrame.hWindow = hWindow;
	VoWinFrame.stFrame = *pFrameinfo;

    Ret = ioctl(g_VoDevFd, CMD_WIN_VIR_RELEASE, &VoWinFrame);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetAttr(HI_HANDLE hWindow, const HI_DRV_WIN_ATTR_S *pWinAttr)
{
    HI_S32           Ret = HI_SUCCESS;
    WIN_CREATE_S     VoWinCreate;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    
    CHECK_VO_NULL_PTR(pWinAttr);
    
    if (pWinAttr->enARCvrs >= HI_DRV_ASP_RAT_MODE_BUTT)
    {
        HI_ERR_WIN("para pWinAttr->enAspectCvrs is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    memset((HI_VOID*)&VoWinCreate, 0, sizeof(WIN_CREATE_S));
    VoWinCreate.hWindow = hWindow;
    memcpy(&VoWinCreate.WinAttr, pWinAttr, sizeof(HI_DRV_WIN_ATTR_S));

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_ATTR, &VoWinCreate);

    return Ret;
}

HI_S32 HI_MPI_WIN_GetAttr(HI_HANDLE hWindow, HI_DRV_WIN_ATTR_S *pWinAttr)
{
    HI_S32           Ret = HI_SUCCESS;
    WIN_CREATE_S  VoWinCreate;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_NULL_PTR(pWinAttr);
    CHECK_VO_INIT();
    
    memset((HI_VOID*)&VoWinCreate, 0, sizeof(WIN_CREATE_S));
    VoWinCreate.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_ATTR, &VoWinCreate);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pWinAttr, &VoWinCreate.WinAttr, sizeof(HI_DRV_WIN_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetZorder(HI_HANDLE hWindow, HI_DRV_DISP_ZORDER_E enZFlag)
{
    HI_S32            Ret;
    WIN_ZORDER_S   VoWinZorder;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (enZFlag >= HI_DRV_DISP_ZORDER_BUTT)
    {
        HI_ERR_WIN("para enZFlag is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinZorder.hWindow = hWindow;
    VoWinZorder.eZFlag = enZFlag;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_ZORDER, &VoWinZorder);

    return Ret;
}

HI_S32 HI_MPI_WIN_GetZorder(HI_HANDLE hWindow, HI_U32 *pu32Zorder)
{
    HI_S32            Ret = HI_SUCCESS;
    WIN_ORDER_S   VoWinOrder;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (!pu32Zorder)
    {
        HI_ERR_WIN("para SrcHandle is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    memset((HI_VOID*)&VoWinOrder, 0, sizeof(WIN_ORDER_S));
    VoWinOrder.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_ORDER, &VoWinOrder);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pu32Zorder = VoWinOrder.Order;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Attach(HI_HANDLE hWindow, HI_HANDLE hSrc)
{

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Detach(HI_HANDLE hWindow, HI_HANDLE hSrc)
{

    return HI_FAILURE;
}

HI_S32 HI_MPI_VO_SetWindowRatio(HI_HANDLE hWindow, HI_U32 u32WinRatio)
{

    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_Freeze(HI_HANDLE hWindow, HI_BOOL bEnable, HI_DRV_WIN_SWITCH_E enWinFreezeMode)
{
    HI_S32           Ret;
    WIN_FREEZE_S  VoWinFreeze;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if ((bEnable != HI_TRUE)
      &&(bEnable != HI_FALSE)
       )
    {
        HI_ERR_WIN("para bEnable is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (enWinFreezeMode >= HI_DRV_WIN_SWITCH_BUTT)
    {
        HI_ERR_WIN("para enWinFreezeMode is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinFreeze.hWindow = hWindow;
    VoWinFreeze.bEnable = bEnable;
    VoWinFreeze.eMode   = enWinFreezeMode;

    Ret = ioctl(g_VoDevFd, CMD_WIN_FREEZE, &VoWinFreeze);

    return Ret;
}

HI_S32 HI_MPI_WIN_GetFreezeStat(HI_HANDLE hWindow, HI_BOOL *bEnable, HI_DRV_WIN_SWITCH_E *enWinFreezeMode)
{
    HI_S32           Ret;
    WIN_FREEZE_S    stWinFreeze;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if ((!bEnable) || (!enWinFreezeMode))
    {
        HI_ERR_WIN("param ptr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    stWinFreeze.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_FREEZE_STATUS, &stWinFreeze);
    if (HI_SUCCESS == Ret)
    {
        *bEnable = stWinFreeze.bEnable;
        *enWinFreezeMode = stWinFreeze.eMode;
    }

    return Ret;
}

HI_S32 HI_MPI_WIN_SetFieldMode(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_SendFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32             Ret = HI_SUCCESS;
    WIN_FRAME_S     VoWinFrame;
    
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    
    if (!pFrame)
    {
        HI_ERR_WIN("para pFrame is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    memset((HI_VOID*)&VoWinFrame, 0, sizeof(WIN_FRAME_S));
    VoWinFrame.hWindow  = hWindow;
	VoWinFrame.stFrame = *pFrame;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SEND_FRAME, &VoWinFrame);

    return Ret;
}

HI_S32 HI_MPI_WIN_DequeueFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32             Ret = HI_SUCCESS;
    WIN_FRAME_S VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (!pFrame)
    {
        HI_ERR_WIN("para pFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();
    
    memset((HI_VOID*)&VoWinFrame, 0, sizeof(WIN_FRAME_S));
    VoWinFrame.hWindow = hWindow;
    
    Ret = ioctl(g_VoDevFd, CMD_WIN_DQ_FRAME, &VoWinFrame);
    if (HI_SUCCESS == Ret)
    {
		*pFrame = VoWinFrame.stFrame;
    }

    return Ret;
}

HI_S32 HI_MPI_WIN_QueueSyncFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame,HI_U32 *pu32FenceFd)
{
    HI_S32             Ret;
    WIN_SYNCFRAME_S     VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (!pFrame)
    {
        HI_ERR_WIN("para pFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinFrame.hWindow = hWindow;
	VoWinFrame.stFrame = *pFrame;
	if (pu32FenceFd == HI_NULL)
	{
		VoWinFrame.u32FenceFd = 0x0;
	}
	else
	{
		VoWinFrame.u32FenceFd = 0xffffffff;
	}

	Ret = ioctl(g_VoDevFd, CMD_WIN_QUSYNC_FRAME, &VoWinFrame);
	if ((HI_SUCCESS == Ret) && (pu32FenceFd != HI_NULL))
	{
		*pu32FenceFd = VoWinFrame.u32FenceFd;
	}

    return Ret;
}

#ifdef HI_VDP_HDR_SUPPORT
/* this method is to slove 32bit User + 64bit Kernel problem.
   DmKsFxp_t has pointers, User and Kernel may have different size.
*/
static HI_VOID HI_MPI_WIN_Transfer2HisiDmKs(DmKsFxp_t *pstDolbyDmKs,Hisi_DmKsFxp_t *pstHisiDmKs)
{
    pstHisiDmKs->bypassShift = pstDolbyDmKs->bypassShift;
    pstHisiDmKs->frmBuf0     = (uint32_t)(uintptr_t)pstDolbyDmKs->frmBuf0;
    pstHisiDmKs->frmBuf1     = (uint32_t)(uintptr_t)pstDolbyDmKs->frmBuf1;
    pstHisiDmKs->frmBuf2     = (uint32_t)(uintptr_t)pstDolbyDmKs->frmBuf2;
    pstHisiDmKs->ksDmCtrl    = pstDolbyDmKs->ksDmCtrl;
    pstHisiDmKs->ksFrmFmtG   = pstDolbyDmKs->ksFrmFmtG;
    pstHisiDmKs->ksFrmFmtI   = pstDolbyDmKs->ksFrmFmtI;
    pstHisiDmKs->ksFrmFmtO   = pstDolbyDmKs->ksFrmFmtO;
    pstHisiDmKs->ksIMap      = pstDolbyDmKs->ksIMap;
    pstHisiDmKs->ksIMapG     = pstDolbyDmKs->ksIMapG;
    pstHisiDmKs->ksOMap      = pstDolbyDmKs->ksOMap;
    pstHisiDmKs->ksTMap      = pstDolbyDmKs->ksTMap;
    pstHisiDmKs->ksTMapG     = pstDolbyDmKs->ksTMapG;
    pstHisiDmKs->ksUds       = pstDolbyDmKs->ksUds;
    pstHisiDmKs->rowPitchNum = pstDolbyDmKs->rowPitchNum;

    return ;
    
}
#endif


HI_S32 HI_MPI_WIN_QueueFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32                Ret = HI_SUCCESS;
    WIN_FRAME_S           stVoWinFrame;
#ifdef HI_VDP_HDR_SUPPORT
    WIN_DISP_OUT_TYPE_S   stVoWinDispOutType;
    dm_metadata_t         *p_dm_data = HI_NULL;
    DmKsFxp_t             *p_tmp_dm_reg = HI_NULL;
    Hisi_DmKsFxp_t        *p_dst_dm_reg = HI_NULL;
    target_info_t          tgt_info ;
    src_grcinfo_t          grc_info ;
    src_videoinfo_t        video_info;

    memset(&stVoWinDispOutType,0x0,sizeof(WIN_DISP_OUT_TYPE_S));
    memset(&tgt_info,0x0,sizeof(target_info_t));
    memset(&grc_info,0x0,sizeof(src_grcinfo_t));
    memset(&video_info,0x0,sizeof(src_videoinfo_t));
#endif
    memset(&stVoWinFrame, 0x0, sizeof(WIN_FRAME_S));

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (!pFrame)
    {
        HI_ERR_WIN("para pFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();
    
    stVoWinFrame.hWindow = hWindow;
	stVoWinFrame.stFrame = *pFrame;
    
#ifdef  HI_VDP_HDR_SUPPORT
    /* if receive HDR10 frame and there exist dolbyvision lib,
     * generate the DM configurations with dolbyvision lib.
     */
    if((HI_DRV_VIDEO_FRAME_TYPE_HDR10 == pFrame->enSrcFrameType)
        && (s_DolbyFuncEntry.pfnSet_display_config)
        && (s_DolbyFuncEntry.pfnSet_grc_config)
        && (s_DolbyFuncEntry.pfnCommit_reg))
    {
        /* assigned location for configurations. 
         * Attention! There are no rpu/composer/update_metadata in HDR10 frame.
         */
        p_dm_data    = (dm_metadata_t *)stDolbyBuf.user_viraddr;
        p_dst_dm_reg = (Hisi_DmKsFxp_t*)((char*)p_dm_data + sizeof(dm_metadata_t));
        p_tmp_dm_reg = (DmKsFxp_t*)stDolbyDmKsBuf.user_viraddr;
        /*step1: Acquire Disp output type. */
        stVoWinDispOutType.hWindow = hWindow;
        Ret = ioctl(g_VoDevFd, CMD_WIN_GET_DISP_OUT_TYPE, &stVoWinDispOutType);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_WIN("Get Disp Output Type Fail.\n");
            return Ret;
        }

        /* adjust disp output type,HDR10 can ONLY output SDR(cert)/HDR10(cert) signal. */
        switch(stVoWinDispOutType.enDispOutType)
        {
            case HI_DRV_DISP_TYPE_NORMAL:
            case HI_DRV_DISP_TYPE_DOLBY:
            {
                stVoWinFrame.stDolbyHdr10Cfg.enDispOutType = HI_DRV_DISP_TYPE_NORMAL;
                tgt_info.tgt_type = display_out_sdr_normal;
                break;
            }
            case HI_DRV_DISP_TYPE_SDR_CERT:
            {
                stVoWinFrame.stDolbyHdr10Cfg.enDispOutType = HI_DRV_DISP_TYPE_SDR_CERT; 
                tgt_info.tgt_type = display_out_sdr_authentication; 
                break;
            }
            case HI_DRV_DISP_TYPE_HDR10:
            {
                stVoWinFrame.stDolbyHdr10Cfg.enDispOutType = HI_DRV_DISP_TYPE_HDR10; 
                tgt_info.tgt_type = display_out_hdr10;
                break;
            }   
            case HI_DRV_DISP_TYPE_HDR10_CERT:
            {
                stVoWinFrame.stDolbyHdr10Cfg.enDispOutType = HI_DRV_DISP_TYPE_HDR10_CERT; 
                tgt_info.tgt_type = display_out_hdr10;
                break;
            }
            default:
            {
                stVoWinFrame.stDolbyHdr10Cfg.enDispOutType = HI_DRV_DISP_TYPE_NORMAL; 
                tgt_info.tgt_type = display_out_sdr_normal;
                break;
            }               
        }

        /* step2: set dolby lib display config. */
        tgt_info.w = 1920;
        tgt_info.h = 1080;    
        Ret = s_DolbyFuncEntry.pfnSet_display_config(tgt_info);
        if (Ret)
        {
            HI_ERR_WIN("set_display_config fail!\n");
            return Ret;
        }
        
        /* step3: set dolby lib graphic config. */
        grc_info.w = 1920;
        grc_info.h = 1080;
        Ret = s_DolbyFuncEntry.pfnSet_grc_config(grc_info);
        if (Ret)
        {
            HI_ERR_WIN("set_grc_config fail!\n");
            return Ret;
        }

        /* step4: commit reg. */
        video_info.w = pFrame->u32Width;
        video_info.h = pFrame->u32Height;
        video_info.video_type = src_video_hdr10;
        video_info.bits = (pFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_8BIT) ?  8 : 10;
        Ret = s_DolbyFuncEntry.pfnCommit_reg(p_dm_data, p_tmp_dm_reg, video_info);
        if (Ret)
        {
            HI_ERR_WIN("commit_reg fail!\n");
            return Ret;
        }

        stVoWinFrame.stDolbyHdr10Cfg.u32PhyAddr = stDolbyBuf.phyaddr;
        stVoWinFrame.stDolbyHdr10Cfg.u32CompLen = 0;//there's no composer in hdr10 frame.
        stVoWinFrame.stDolbyHdr10Cfg.u32DmCfgLen = sizeof(dm_metadata_t);
        /* transfer dolby dm_reg to Hisi dm_reg. */
        HI_MPI_WIN_Transfer2HisiDmKs(p_tmp_dm_reg,p_dst_dm_reg);
        stVoWinFrame.stDolbyHdr10Cfg.u32DmRegLen = sizeof(Hisi_DmKsFxp_t);
        stVoWinFrame.stDolbyHdr10Cfg.u32UpMdLen = 0;//there's no update metadata.
        stVoWinFrame.stDolbyHdr10Cfg.u32TotalLen = stDolbyBuf.bufsize;        
        
    }
#endif

	Ret = ioctl(g_VoDevFd, CMD_WIN_QU_FRAME, &stVoWinFrame);

    return Ret;
}

HI_S32 HI_MPI_WIN_QueueUselessFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32             Ret;
    WIN_FRAME_S     VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (!pFrame)
    {
        HI_ERR_WIN("para pFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinFrame.hWindow = hWindow;
	VoWinFrame.stFrame = *pFrame;

    Ret = ioctl(g_VoDevFd, CMD_WIN_QU_ULSFRAME, &VoWinFrame);

    return Ret;
}


HI_S32 HI_MPI_WIN_QueueDolbyFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S* pBLFrm, HI_DRV_VIDEO_FRAME_S* pELFrm)
{
#ifdef HI_VDP_HDR_SUPPORT
    HI_S32             Ret;
    WIN_DOLBYFRAME_S   stVoWinDolbyFrm;
    WIN_DISP_OUT_TYPE_AND_GFX_STATE_S stWinDispTypeGfxState;
    HI_DRV_VIDEO_DOLBY_METADATA_STREAM_S stMetadata;
    HI_BOOL bBLHasMeta = HI_FALSE;
    HI_BOOL bELHasMeta = HI_FALSE;
    rpu_ext_config_fixpt_main_t *p_composer_data = HI_NULL;
	dm_metadata_t *p_dm_data = HI_NULL;
    HI_U32         u32UpMdLen = 0;

#if 0
		FILE  *fp_dmks;
		FILE  *fp_com;

		fp_dmks = fopen("dm_cfg_1_frame", "rb");
		if (!fp_dmks)
						printf("open file dm_cfg_1_frame failed\r\n");
		fp_com = fopen("comp_1_frame", "rb");
		if (!fp_dmks)
						printf("open file comp_1_frame failed\r\n");
#endif

    DmKsFxp_t      *p_tmp_dm_reg = HI_NULL;
    Hisi_DmKsFxp_t *p_dst_dm_reg = HI_NULL;
    dm_metadata_base_t *p_hdmi_metadata = HI_NULL;
    target_info_t tgt_info ;
    src_grcinfo_t grc_info ;
    src_videoinfo_t video_info;
    HI_U8 *p_rpu = HI_NULL;
    HI_U32  rpu_len;
    HI_CHAR temp_md[512];

    memset(&stVoWinDolbyFrm, 0x0, sizeof(WIN_DOLBYFRAME_S));
    memset(&stMetadata, 0x0, sizeof(HI_DRV_VIDEO_DOLBY_METADATA_STREAM_S));
    memset(&tgt_info,0x0,sizeof(target_info_t));
    memset(&grc_info,0x0,sizeof(src_grcinfo_t));
    memset(&video_info,0x0,sizeof(src_videoinfo_t));

    //step1. check valid,including ptr and metadata(ONLY one frame has metadata-bitStream).
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (HI_NULL == pBLFrm)
    {
        HI_ERR_WIN("para pBLFrminfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if((!s_DolbyFuncEntry.pfnMetadata_parser_process)
        || (!s_DolbyFuncEntry.pfnSet_display_config)
        || (!s_DolbyFuncEntry.pfnSet_grc_config)
        || (!s_DolbyFuncEntry.pfnCommit_reg)
        || (!s_DolbyFuncEntry.pfnBypass_metadata_update))
    {
        HI_ERR_WIN("s_DolbyFuncEntry is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    if (HI_NULL == pELFrm)
    {
        stVoWinDolbyFrm.bELValid = HI_FALSE;

        bBLHasMeta = pBLFrm->unHDRInfo.stDolbyInfo.bMetadataValid;
        //set EL frame NULL
        memset(&stVoWinDolbyFrm.stELFrm, 0x0, sizeof(HI_DRV_VIDEO_FRAME_S));
    }
    else
    {
        stVoWinDolbyFrm.bELValid = HI_TRUE;

        bBLHasMeta = pBLFrm->unHDRInfo.stDolbyInfo.bMetadataValid;
        bELHasMeta = pELFrm->unHDRInfo.stDolbyInfo.bMetadataValid;
        //copy EL frame
        memcpy(&stVoWinDolbyFrm.stELFrm, pELFrm, sizeof(HI_DRV_VIDEO_FRAME_S));
    }

    if ((bBLHasMeta && bELHasMeta) ||
        (!bBLHasMeta && !bELHasMeta))
    {
        HI_ERR_WIN("Dolby Frame has wrong metadata!\n");
        return HI_ERR_VO_FRAME_INFO_ERROR;
    }

    if (bBLHasMeta)
    {
        stMetadata.u32Addr   = pBLFrm->unHDRInfo.stDolbyInfo.stMetadata.u32Addr;
        stMetadata.u32Length = pBLFrm->unHDRInfo.stDolbyInfo.stMetadata.u32Length;
    }
    else if (bELHasMeta)
    {
        stMetadata.u32Addr   = pELFrm->unHDRInfo.stDolbyInfo.stMetadata.u32Addr;
        stMetadata.u32Length = pELFrm->unHDRInfo.stDolbyInfo.stMetadata.u32Length;
    }

    //step2. Call Dolby's metadata parser API,start to parse metadata-bit stream.Should consider security memory.

#ifdef HI_SMMU_SUPPORT
		p_rpu = (HI_U8*)HI_MPI_SMMU_Map(stMetadata.u32Addr, HI_FALSE);
#else
		//p_rpu = (HI_U8*)HI_MPI_MMZ_Map(stMetadata.u32Addr, HI_FALSE);
		p_rpu =  HI_MMAP(stMetadata.u32Addr, stMetadata.u32Length);
#endif


    if (!p_rpu)
    {
        HI_FATAL_WIN("memap stMetadata %08x failed\r\n", stMetadata.u32Addr);
        return -1;
    }
    rpu_len = stMetadata.u32Length;
    p_composer_data = (rpu_ext_config_fixpt_main_t*) stDolbyBuf.user_viraddr;
    p_dm_data       = (dm_metadata_t *)(stDolbyBuf.user_viraddr + 
                                        sizeof(rpu_ext_config_fixpt_main_t));
    p_dst_dm_reg = (Hisi_DmKsFxp_t*)((char*)p_dm_data + sizeof(dm_metadata_t));
    p_hdmi_metadata = (dm_metadata_base_t*)((char*)p_dst_dm_reg + sizeof(Hisi_DmKsFxp_t));
    p_tmp_dm_reg = (DmKsFxp_t*)stDolbyDmKsBuf.user_viraddr;

    Ret = s_DolbyFuncEntry.pfnMetadata_parser_process((HI_CHAR*)p_rpu, rpu_len, p_composer_data, p_dm_data, temp_md);
    if (Ret)
    {
        HI_ERR_WIN("Metadata Parser Error!\n");
#ifdef HI_SMMU_SUPPORT
        HI_MPI_SMMU_Unmap(stMetadata.u32Addr);
#else
        HI_MUNMAP(p_rpu);
#endif
        return Ret;
    }

#ifdef HI_SMMU_SUPPORT
	HI_MPI_SMMU_Unmap(stMetadata.u32Addr);
#else
	HI_MUNMAP(p_rpu);
	//HI_MPI_MMZ_Unmap(stMetadata.u32Addr);
#endif

    //step3. Aquire the Disp output.
    stWinDispTypeGfxState.hWindow = hWindow;
    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_DISP_OUT_TYPE_AND_GFX_STATE, &stWinDispTypeGfxState);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_WIN("Get Disp Output Type Fail.\n");
        return Ret;
    }
    
    stVoWinDolbyFrm.stDolbyInfo.enDispOutType = stWinDispTypeGfxState.enDispOutType;
    
    //step4. Call Dolby's generate register API,start to generate register value.
    //step4-1. set display configration according to Disp's Output type.
    video_info.w = pBLFrm->u32Width;
    video_info.h = pBLFrm->u32Height;
    video_info.bits = (pBLFrm->enBitWidth == HI_DRV_PIXEL_BITWIDTH_8BIT) ?  8 : 10;
    if (p_dm_data->base.signal_color_space == 0)
    {
        video_info.video_type = src_video_dolby_yuv;
        if (p_dm_data->base.signal_bit_depth != 14)
        {
            HI_ERR_WIN("dolby invalid bit depth = %d when src_video_dolby_yuv\r\n ",
                    p_dm_data->base.signal_bit_depth);
        }
    }
    else
    {
        video_info.video_type = src_video_dolby_ipt;
        if (p_dm_data->base.signal_bit_depth != 12)
        {
            HI_ERR_WIN("dolby invalid bit depth = %d when src_video_dolby_ipt\r\n ",
                    p_dm_data->base.signal_bit_depth);
        }
    }

    tgt_info.w = 1920;
    tgt_info.h = 1080;
    if (stWinDispTypeGfxState.enDispOutType == HI_DRV_DISP_TYPE_NORMAL)
    {
        tgt_info.tgt_type = display_out_sdr_normal;
    }
    else if (stWinDispTypeGfxState.enDispOutType == HI_DRV_DISP_TYPE_SDR_CERT)
    {
        tgt_info.tgt_type = display_out_sdr_authentication; 
    }
    else if (stWinDispTypeGfxState.enDispOutType == HI_DRV_DISP_TYPE_DOLBY)
    {
        tgt_info.tgt_type = display_out_dolby_ipt; 
    }
    else if ((stWinDispTypeGfxState.enDispOutType == HI_DRV_DISP_TYPE_HDR10)
        || (stWinDispTypeGfxState.enDispOutType == HI_DRV_DISP_TYPE_HDR10_CERT))
    {
        tgt_info.tgt_type = display_out_hdr10;
    }

    Ret = s_DolbyFuncEntry.pfnSet_display_config(tgt_info);
    if (Ret)
    {
        HI_ERR_WIN("set_display_config fail!\n");
        return Ret;
    }

    grc_info.w = 1920;
    grc_info.h = 1080;
    Ret = s_DolbyFuncEntry.pfnSet_grc_config(grc_info);
    if (Ret)
    {
        HI_ERR_WIN("set_grc_config fail!\n");
        return Ret;
    }

    //step4-2. commit_reg
    Ret = s_DolbyFuncEntry.pfnCommit_reg(p_dm_data, p_tmp_dm_reg, video_info);
    if (Ret)
    {
        HI_ERR_WIN("commit_reg fail!\n");
        return Ret;
    }

    //step5. Call Dolby's Update metadata API if Disp Output type is Dolby.
    if (HI_DRV_DISP_TYPE_DOLBY == stVoWinDolbyFrm.stDolbyInfo.enDispOutType)
    {
        s_DolbyFuncEntry.pfnBypass_metadata_update((dm_metadata_base_t*)temp_md, p_hdmi_metadata,
            SIGNAL_CS_IPT , SIGNAL_RANGE_FULL,stWinDispTypeGfxState.bNeedProcessGfx, &u32UpMdLen);
    }
    else
    {
        //TODO.
    }

#if 0
	fread(p_dst_dm_reg, 1, sizeof(*p_dst_dm_reg), fp_dmks);
	fread(p_composer_data, 1, sizeof(*p_composer_data), fp_com);

	fclose(fp_dmks);
	fclose(fp_com);
#endif

    stVoWinDolbyFrm.hWindow                   = hWindow;
    stVoWinDolbyFrm.stDolbyInfo.u32PhyAddr    = stDolbyBuf.phyaddr;
    stVoWinDolbyFrm.stDolbyInfo.u32CompLen    = sizeof(rpu_ext_config_fixpt_main_t);
    stVoWinDolbyFrm.stDolbyInfo.u32DmCfgLen   = sizeof(dm_metadata_t);
    HI_MPI_WIN_Transfer2HisiDmKs(p_tmp_dm_reg,p_dst_dm_reg);
    stVoWinDolbyFrm.stDolbyInfo.u32DmRegLen   = sizeof(Hisi_DmKsFxp_t);
    stVoWinDolbyFrm.stDolbyInfo.u32UpMdLen    = u32UpMdLen;
    stVoWinDolbyFrm.stDolbyInfo.u32TotalLen   = stDolbyBuf.bufsize;
	memcpy(&stVoWinDolbyFrm.stBLFrm, pBLFrm, sizeof(HI_DRV_VIDEO_FRAME_S));
    
    /**Judge whether the length is valid.**/
    if (stDolbyBuf.bufsize < (stVoWinDolbyFrm.stDolbyInfo.u32CompLen + \
                              stVoWinDolbyFrm.stDolbyInfo.u32DmCfgLen + \
                              stVoWinDolbyFrm.stDolbyInfo.u32DmRegLen + \
                              stVoWinDolbyFrm.stDolbyInfo.u32UpMdLen))
    {
        HI_ERR_WIN("metadata buffer size is too small!\n");
        return HI_ERR_VO_WIN_UNSUPPORT;
    }

    if (video_info.video_type == src_video_dolby_yuv)
    {
        stVoWinDolbyFrm.stBLFrm.unHDRInfo.stDolbyInfo.bCompatible = HI_TRUE;
    }
    else
    {
        stVoWinDolbyFrm.stBLFrm.unHDRInfo.stDolbyInfo.bCompatible = HI_FALSE;
    }
	
    Ret = ioctl(g_VoDevFd, CMD_WIN_QUDOLBY_FRAME, &stVoWinDolbyFrm);

    return Ret;
#else
	HI_ERR_WIN("Don't supprt Dolby Frame\n");
	return HI_ERR_VO_WIN_UNSUPPORT;
#endif
}

HI_S32 HI_MPI_WIN_Reset(HI_HANDLE hWindow, HI_DRV_WIN_SWITCH_E enWinFreezeMode)
{
    WIN_RESET_S   VoWinReset;
    HI_S32 Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (enWinFreezeMode >= HI_DRV_WIN_SWITCH_BUTT)
    {
        HI_ERR_WIN("para enWinFreezeMode is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinReset.hWindow = hWindow;
    VoWinReset.eMode = enWinFreezeMode;

    Ret = ioctl(g_VoDevFd, CMD_WIN_RESET, &VoWinReset);

    return Ret;
}

HI_S32 HI_MPI_WIN_Pause(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    WIN_PAUSE_S   VoWinPause;
    HI_S32           Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if ((bEnable != HI_TRUE)
      &&(bEnable != HI_FALSE)
       )
    {
        HI_ERR_WIN("para bEnable is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinPause.hWindow = hWindow;
    VoWinPause.bEnable = bEnable;

    Ret = ioctl(g_VoDevFd, CMD_WIN_PAUSE, &VoWinPause);

    return Ret;
}


HI_S32 HI_MPI_VO_GetWindowDelay(HI_HANDLE hWindow, HI_DRV_WIN_PLAY_INFO_S *pDelay)
{
    WIN_PLAY_INFO_S   VoWinDelay;
    HI_S32 Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (!pDelay)
    {
        HI_ERR_WIN("para pDelay is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinDelay.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_PLAY_INFO, &VoWinDelay);
    if (HI_SUCCESS == Ret)
    {
        *pDelay = VoWinDelay.stPlayInfo;
    }

    return Ret;
}

HI_S32 HI_MPI_WIN_SetStepMode(HI_HANDLE hWindow, HI_BOOL bStepMode)
{
    HI_S32              Ret;
    WIN_STEP_MODE_S  WinStepMode;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    WinStepMode.hWindow = hWindow;
    WinStepMode.bStep = bStepMode;

    Ret = ioctl(g_VoDevFd, CMD_WIN_STEP_MODE, &WinStepMode);

    return Ret;
}

HI_S32 HI_MPI_WIN_SetStepPlay(HI_HANDLE hWindow)
{
    HI_S32      Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_STEP_PLAY, &hWindow);

    return Ret;
}

HI_S32 HI_MPI_VO_DisableDieMode(HI_HANDLE hWindow)
{
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_SetExtBuffer(HI_HANDLE hWindow, HI_DRV_VIDEO_BUFFER_POOL_S* pstBufAttr)
{
    HI_S32      Ret;
    WIN_BUF_POOL_S  bufferAttr;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_NULL_PTR(pstBufAttr);
    CHECK_VO_INIT();

    bufferAttr.hwin = hWindow;
    bufferAttr.stBufPool = *pstBufAttr;

    Ret = ioctl(g_VoDevFd, CMD_WIN_VIR_EXTERNBUF, &bufferAttr);

    return Ret;
}

HI_S32 HI_MPI_WIN_SetQuickOutput(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    WIN_SET_QUICK_S stQuickOutputAttr;
    HI_S32      Ret = HI_FAILURE;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    stQuickOutputAttr.hWindow = hWindow;
    stQuickOutputAttr.bQuickEnable = bEnable;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_QUICK, &stQuickOutputAttr);

    return Ret;
}

HI_S32 HI_MPI_WIN_GetQuickOutputStatus(HI_HANDLE hWindow, HI_BOOL *bQuickOutputEnable)
{
    WIN_SET_QUICK_S stQuickOutputAttr;
    HI_S32      Ret = HI_FAILURE;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    CHECK_VO_NULL_PTR(bQuickOutputEnable);
    CHECK_VO_INIT();
    
    stQuickOutputAttr.hWindow = hWindow;
    
    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_QUICK, &stQuickOutputAttr);
    if (Ret == HI_SUCCESS)
    {
        *bQuickOutputEnable = stQuickOutputAttr.bQuickEnable;
    }

    return Ret;
}

HI_S32 HI_MPI_VO_UseDNRFrame(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    return HI_FAILURE;
}

#define STRIDE_ALIGN_NUM 15
HI_S32 HI_MPI_VO_CapturePictureExt(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pstCapPicture)
{
    HI_S32            Ret = HI_SUCCESS;
    WIN_CAPTURE_S     VoWinCapture;
    HI_U32   height = 0, y_stride_Output = 0;
#if HI_VDP_VIDEO_LINEDCMP 
    HI_U32 i = 0,y_stride = 0;
    HI_U32          OriginFrameSize = 0, DcmpOutputBufSize = 0;
    HI_UCHAR        *DecompressOutBuf = HI_NULL, *DecompressInBuf = HI_NULL;
    HI_UCHAR        *Inptr = HI_NULL, *Outptr = HI_NULL;
#endif    
	HI_DRV_PIX_FORMAT_E  ePixFormat_Out = HI_DRV_PIX_BUTT;

    CHECK_VO_INIT();
    if ((HI_INVALID_HANDLE == hWindow) || (!pstCapPicture))
    {
        HI_ERR_WIN(" invalid param.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    /*first,  we get a frame from the window.*/
    VoWinCapture.hWindow = hWindow;
    Ret = ioctl(g_VoDevFd, CMD_VO_WIN_CAPTURE_START, &VoWinCapture);
    if (Ret != HI_SUCCESS)
        return Ret;

    height = VoWinCapture.CapPicture.u32Height;
	y_stride_Output = (VoWinCapture.CapPicture.u32Width + STRIDE_ALIGN_NUM) & 0xfffffff0;
	
#if HI_VDP_VIDEO_LINEDCMP
	y_stride = VoWinCapture.CapPicture.stBufAddr[0].u32Stride_Y;

	if (0 == y_stride)
	{
		HI_ERR_WIN("Stride_Y is 0\r\n");
		goto ret_err;
	}

    if (HI_DRV_PIX_FMT_NV21 == VoWinCapture.CapPicture.ePixFormat)
    {
        OriginFrameSize = height * y_stride * 3 / 2 + height * 4;
    }
	else if (HI_DRV_PIX_FMT_NV21_CMP == VoWinCapture.CapPicture.ePixFormat)
	{
        OriginFrameSize = height * y_stride * 3 / 2 + 16 * height * 3 / 2;
	}
    else  if (HI_DRV_PIX_FMT_NV61_2X1 == VoWinCapture.CapPicture.ePixFormat)
    {
        OriginFrameSize = height * y_stride * 2 + height * 4;
    }
	else
	{
		HI_ERR_WIN("pixel fmt not supported %d.\n",VoWinCapture.CapPicture.ePixFormat);
		goto ret_err;
	}

	DcmpOutputBufSize = VoWinCapture.driver_supply_addr.length;

    DecompressInBuf =(HI_UCHAR *)(HI_MMAP(VoWinCapture.CapPicture.stBufAddr[0].u32PhyAddr_Y, OriginFrameSize));
	if (HI_NULL == DecompressInBuf)
	{
		HI_ERR_WIN("DecompressInBuf buffer map fail\r\n");
		goto ret_err;
	}
	
    DecompressOutBuf = (HI_UCHAR *)(HI_MMAP(VoWinCapture.driver_supply_addr.startPhyAddr,
                                                DcmpOutputBufSize));
	if (HI_NULL == DecompressOutBuf)
	{
		if (HI_SUCCESS != HI_MUNMAP((void*)DecompressInBuf))
	    {
	        HI_ERR_WIN("decompress buffer unmap fail\r\n");
	    }
		goto ret_err;
	}

    /*third  step, we make a decompress or simple copy from driver-vpss frame to user frame.*/
    Inptr = DecompressInBuf;
    Outptr = DecompressOutBuf;

	if ( HI_DRV_PIX_FMT_NV21_CMP == VoWinCapture.CapPicture.ePixFormat)
	{
    #ifdef HI_VDP_VIDEO_LINEDCMP
		(HI_VOID)Dcmp_Func(&VoWinCapture.CapPicture,
						   DcmpOutputBufSize,
						   Inptr,
						   Outptr);

		ePixFormat_Out = HI_DRV_PIX_FMT_NV21;
	#endif
	}
	else
	{
		ePixFormat_Out = VoWinCapture.CapPicture.ePixFormat;

		for(i = 0 ; i < OriginFrameSize / y_stride; i++)
		{
			memcpy(Outptr, Inptr, y_stride);
			Inptr += y_stride;
			Outptr += y_stride;
		}
	}
    if (HI_SUCCESS != HI_MUNMAP((void*)DecompressInBuf))
    {
        HI_ERR_WIN("decompress buffer unmap fail\r\n");
        (HI_VOID)HI_MUNMAP((void*)DecompressOutBuf);
        goto ret_err;
    }

    if (HI_SUCCESS != HI_MUNMAP((void*)DecompressOutBuf))
    {
        HI_ERR_WIN("decompress buffer unmap fail\r\n");
        goto ret_err;
    }
#else
    ePixFormat_Out = HI_DRV_PIX_FMT_NV21;
#endif


    Ret = ioctl(g_VoDevFd, CMD_VO_WIN_CAPTURE_RELEASE, &VoWinCapture);
    if (Ret != HI_SUCCESS)
        return Ret;

    *pstCapPicture = VoWinCapture.CapPicture;
    /*FIXME: we should know how the 3d will be dealed with*/
    pstCapPicture->stBufAddr[0].u32PhyAddr_Y = VoWinCapture.driver_supply_addr.startPhyAddr;

    pstCapPicture->stBufAddr[0].u32PhyAddr_C =
        pstCapPicture->stBufAddr[0].u32PhyAddr_Y +
        height * y_stride_Output;

	pstCapPicture->stBufAddr[0].u32Stride_Y =  y_stride_Output;
	pstCapPicture->stBufAddr[0].u32Stride_C =  y_stride_Output;
	pstCapPicture->ePixFormat  = ePixFormat_Out;

    return HI_SUCCESS;
	
#if HI_VDP_VIDEO_LINEDCMP
ret_err:
#endif	
	Ret = ioctl(g_VoDevFd, CMD_VO_WIN_CAPTURE_RELEASE, &VoWinCapture);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
	return HI_FAILURE;

}


HI_S32 HI_MPI_WIN_CapturePicture(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pstCapPicture)
{
    HI_S32            Ret = HI_SUCCESS;

    Ret = HI_MPI_VO_CapturePictureExt(hWindow, pstCapPicture);
    return Ret;
}


HI_S32 HI_MPI_WIN_CapturePictureRelease(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pstCapPicture)
{
    HI_S32          Ret = HI_SUCCESS;
    WIN_CAPTURE_S   VoWinRls;

    CHECK_VO_INIT();
    if ((HI_INVALID_HANDLE == hWindow) || (!pstCapPicture))
    {
        HI_ERR_WIN("invalid  param.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    VoWinRls.hWindow = hWindow;
    VoWinRls.CapPicture = *pstCapPicture;
    VoWinRls.driver_supply_addr.startPhyAddr = pstCapPicture->stBufAddr[0].u32PhyAddr_Y;

    Ret = ioctl(g_VoDevFd, CMD_VO_WIN_CAPTURE_FREE, &VoWinRls);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_CapturePictureRelease fail (INVALID_PARA)\r\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetRotation(HI_HANDLE hWindow, HI_DRV_ROT_ANGLE_E enRotation)
{
    HI_S32          Ret = HI_SUCCESS;
    WIN_ROTATION_S  VoWinRotation;
    VoWinRotation.hWindow = hWindow;
    VoWinRotation.enRotation = enRotation;
    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_ROTATION, &VoWinRotation);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_SetRotation fail \r\n");
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_GetRotation(HI_HANDLE hWindow, HI_DRV_ROT_ANGLE_E *penRotation)
{
    HI_S32          Ret = HI_SUCCESS;
    WIN_ROTATION_S  VoWinRotation;
    VoWinRotation.hWindow = hWindow;
    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_ROTATION, &VoWinRotation);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_GetRotation fail \r\n");
        return Ret;
    }
    *penRotation = VoWinRotation.enRotation;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetFlip(HI_HANDLE hWindow, HI_BOOL bHoriFlip, HI_BOOL bVertFlip)
{
    HI_S32          Ret = HI_SUCCESS;
    WIN_FLIP_S  VoWinFlip;
    VoWinFlip.hWindow = hWindow;
    VoWinFlip.bHoriFlip = bHoriFlip;
    VoWinFlip.bVertFlip = bVertFlip;
    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_FLIP, &VoWinFlip);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_SetFlip fail \r\n");
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_GetFlip(HI_HANDLE hWindow, HI_BOOL *pbHoriFlip, HI_BOOL *pbVertFlip)
{
    HI_S32          Ret = HI_SUCCESS;
    WIN_FLIP_S  VoWinFlip;
    VoWinFlip.hWindow = hWindow;
    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_FLIP, &VoWinFlip);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_GetRotation fail \r\n");
        return Ret;
    }
    *pbHoriFlip = VoWinFlip.bHoriFlip;
    *pbVertFlip = VoWinFlip.bVertFlip;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetMode(HI_HANDLE hWindow, HI_DRV_WINDOW_MODE_E enWinMode)
{
    HI_S32        Ret = HI_SUCCESS;
    WIN_MODE_S    VoWinMode;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("param hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if(enWinMode >= HI_DRV_WINDOW_MODE_BUTT)
    {
        HI_ERR_WIN("param enWinMode is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    VoWinMode.hWindow = hWindow;
    VoWinMode.enWinMode = enWinMode;
    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_MODE, &VoWinMode);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_SetMode fail.\n");
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_CloseHdrPath(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    HI_S32          Ret = HI_SUCCESS;
    WIN_ENABLE_S    stWinHdrEnable;
    stWinHdrEnable.hWindow = hWindow;
    stWinHdrEnable.bEnable = bEnable;

    Ret = ioctl(g_VoDevFd, CMD_WIN_CLOSE_HDR_PATH, &stWinHdrEnable);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_CloseHdrPath fail \r\n");
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetDolbyLibInfo(WIN_DOLBY_LIB_INFO_S *pstDolbyLibInfo)
{
    HI_S32  Ret = HI_SUCCESS;
    WIN_DOLBY_LIB_INFO_S stDolbyLibInfo;
    memset(&stDolbyLibInfo,0x0,sizeof(WIN_DOLBY_LIB_INFO_S));
    
    if(HI_NULL == pstDolbyLibInfo)
    {
        return HI_ERR_VO_NULL_PTR;
    }
    
    memcpy(&stDolbyLibInfo,pstDolbyLibInfo,sizeof(WIN_DOLBY_LIB_INFO_S));

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_DOLBY_LIB_STATUS, &stDolbyLibInfo);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_SetDolbyLibInfo fail \r\n");
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32  HI_MPI_WIN_Debug(WIN_DEBUG_CMD_AND_ARGS_S *pstWinDebugCmdAndArgs)
{
    HI_S32                    Ret = HI_SUCCESS;

    if(HI_NULL == pstWinDebugCmdAndArgs)
    {
        return HI_ERR_VO_NULL_PTR;
    }
    
    Ret = ioctl(g_VoDevFd, CMD_WIN_DEBUG, pstWinDebugCmdAndArgs);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_Debug fail \r\n");
        return Ret;
    }
    return HI_SUCCESS;
}

#if 0
HI_S32 HI_MPI_VO_SetWindowExtAttr(HI_HANDLE hWindow, VO_WIN_EXTATTR_E detType, HI_BOOL bEnable)
{
    HI_S32      Ret;
    VO_WIN_DETECT_S stDetType;
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();
    stDetType.hWindow = hWindow;
    stDetType.detType = detType;
    stDetType.bEnable = bEnable;
    Ret = ioctl(g_VoDevFd, CMD_VO_SET_DET_MODE, &stDetType);

    return Ret;
}

HI_S32 HI_MPI_VO_GetWindowExtAttr(HI_HANDLE hWindow, VO_WIN_EXTATTR_E detType, HI_BOOL *bEnable)
{
    HI_S32      Ret;
    VO_WIN_DETECT_S stDetType;
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    CHECK_VO_INIT();

    stDetType.hWindow = hWindow;
    stDetType.detType = detType;
    Ret = ioctl(g_VoDevFd, CMD_VO_GET_DET_MODE, &stDetType);
    *bEnable = stDetType.bEnable;

    return Ret;
}
#endif

HI_S32 HI_MPI_WIN_Suspend(HI_VOID)
{
    HI_U32 u32Value = 0;
    HI_S32 Ret;

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_SUSPEND, &u32Value);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_Suspend failed\n");
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Resume(HI_VOID)
{
    HI_U32 u32Value = 0;
    HI_S32 Ret;

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_RESUM, &u32Value);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_Resume failed\n");
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_GetHandle(WIN_GET_HANDLE_S *pstWinHandle)
{
    HI_S32 Ret;

    CHECK_VO_NULL_PTR(pstWinHandle);
    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_HANDLE, pstWinHandle);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_GetHandle failed\n");
    }

    return Ret;
}


HI_S32 HI_MPI_WIN_AttachWinSink(HI_HANDLE hWin, HI_HANDLE hSink)
{
    HI_S32      Ret;
    WIN_ATTACH_S stAttach;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    stAttach.enType = ATTACH_TYPE_SINK;
    stAttach.hWindow = hWin;
    stAttach.hMutual = hSink;

    Ret = ioctl(g_VoDevFd, CMD_WIN_ATTACH, &stAttach);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_AttachSink failed\n");
    }

    return Ret;
}

HI_S32 HI_MPI_WIN_DetachWinSink(HI_HANDLE hWin, HI_HANDLE hSink)
{
    HI_S32      Ret;
    WIN_ATTACH_S stAttach;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    stAttach.enType = ATTACH_TYPE_SINK;
    stAttach.hWindow = hWin;
    stAttach.hMutual = hSink;

    Ret = ioctl(g_VoDevFd, CMD_WIN_DETACH, &stAttach);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_DetachSink failed\n");
    }

    return Ret;
}


HI_S32 HI_MPI_WIN_GetLatestFrameInfo(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S  *frame_info)
{
    HI_S32      Ret;
    WIN_FRAME_S frame_struct;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_NULL_PTR(frame_info);
    CHECK_VO_INIT();
    
    frame_struct.hWindow = hWin;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_LATESTFRAME_INFO, &frame_struct);
    if (Ret != HI_SUCCESS)
    {
		HI_ERR_WIN("get latest frame info failed\n");
		return Ret;
    }

    *frame_info  = frame_struct.stFrame;
    return Ret;
}

HI_S32 HI_MPI_WIN_GetUnloadTimes(HI_HANDLE hWin, HI_U32 *pu32Time)
{
    HI_S32      Ret;
    WIN_UNLOAD_S stWinUnload;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_NULL_PTR(pu32Time);
    CHECK_VO_INIT();

    stWinUnload.hWindow = hWin;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_UNLOAD, &stWinUnload);
    if (Ret != HI_SUCCESS)
    {
		HI_ERR_WIN("get latest frame info failed\n");
		return Ret;
    }

    *pu32Time  = stWinUnload.u32Times;
    return Ret;
}

#if defined(CHIP_TYPE_hi3716dv100)   \
        || defined(CHIP_TYPE_hi3798mv100)\
        || defined(CHIP_TYPE_hi3796mv100)

#define VDEC_CAPABILITY_WIDTH   3840
#define VDEC_CAPABILITY_HEIGHT  2160

#elif defined(CHIP_TYPE_hi3798mv200)\
        || defined(CHIP_TYPE_hi3796mv200)\
        || defined(CHIP_TYPE_hi3798cv100)\
        || defined(CHIP_TYPE_hi3798cv200)\
        || defined(CHIP_TYPE_hi3798mv200_a)

#define VDEC_CAPABILITY_WIDTH   4096
#define VDEC_CAPABILITY_HEIGHT  2304

#else
#define VDEC_CAPABILITY_WIDTH   0
#define VDEC_CAPABILITY_HEIGHT  0
#endif

static HI_S32 MPI_WIN_GetVdecAbility(HI_CHIP_TYPE_E enChipType,
        HI_U32 *pu32WidthMax,
        HI_U32 *pu32HeightMax)
{
    HI_S32 s32Ret = HI_SUCCESS;

    *pu32WidthMax = VDEC_CAPABILITY_WIDTH;
    *pu32HeightMax = VDEC_CAPABILITY_HEIGHT;

    if (0 == *pu32WidthMax || 0 == *pu32HeightMax)
    {
        HI_ERR_WIN("Invalid ChipType %d\n", enChipType);
        s32Ret = HI_FAILURE;
    }

    return s32Ret;
}


HI_S32 HI_MPI_WIN_CalMediaRect(HI_CHIP_TYPE_E enChipType,
                              HI_RECT_S stStreamRect,
                              HI_RECT_S *pstMediaRect)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32OriW = 0,u32OriH = 0;
    HI_U32 u32DstW = 0,u32DstH = 0;
    HI_U32 u32WidthLevel = 0, u32HeightLevel = 0;

    CHECK_VO_NULL_PTR(pstMediaRect);

    u32OriW = (HI_U32)stStreamRect.s32Width;
    u32OriH = (HI_U32)stStreamRect.s32Height;

    s32Ret = MPI_WIN_GetVdecAbility(enChipType,&u32WidthLevel,&u32HeightLevel);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_WIN("Can't get Width/Height Level\n");
        return s32Ret;
    }

    if(u32OriW < u32WidthLevel && u32OriH < u32HeightLevel)
    {
        u32DstW = u32OriW;
        u32DstH = u32OriH;
    }
    else
    {
        HI_U32 u32RatioW;
        HI_U32 u32RatioH;
        HI_U32 u32TmpH;
        HI_U32 u32TmpW;

		if (u32OriW >= u32WidthLevel && u32OriH >= u32HeightLevel)
		{
			u32DstW = u32WidthLevel;
			u32DstH = u32HeightLevel;
		}
		else
		{
			u32RatioW = u32OriW*2048/u32WidthLevel;
			u32RatioH = u32OriH*2048/u32HeightLevel;

			if (u32RatioW > u32RatioH)
			{
				u32TmpW = u32OriW*2048/u32RatioW;
				u32TmpH = u32OriH*2048/u32RatioW;
			}
			else
			{
				u32TmpW = u32OriW*2048/u32RatioH;
				u32TmpH = u32OriH*2048/u32RatioH;
			}

			u32TmpW = u32TmpW & 0xfffffffe;
			u32TmpH = u32TmpH & 0xfffffffc;

			u32DstW = u32TmpW;
			u32DstH = u32TmpH;
		}
    }

    pstMediaRect->s32Height = (HI_S32)u32DstH;
    pstMediaRect->s32Width = (HI_S32)u32DstW;

    return s32Ret;
}

HI_S32 HI_MPI_WIN_CalAspectRegion(HI_MPI_WIN_RATIO_PARA_S stInPara,HI_RECT_S *pstRevisedRect)
{
	HI_S32 s32Ret;

	ALG_RATIO_OUT_PARA_S stOutPara;
	ALG_RATIO_IN_PARA_S stInRatioPara;

    CHECK_VO_NULL_PTR(pstRevisedRect);

	stInRatioPara.SrcImgH = stInPara.SrcImgH;
	stInRatioPara.SrcImgW = stInPara.SrcImgW;
	stInRatioPara.DeviceWidth = stInPara.DeviceWidth;
	stInRatioPara.DeviceHeight = stInPara.DeviceHeight;
	stInRatioPara.eAspMode = stInPara.eAspMode;
	stInRatioPara.stOutWnd = stInPara.stOutWnd;
	stInRatioPara.stInWnd = stInPara.stInWnd;

	if (stInRatioPara.stInWnd.s32Width != stInRatioPara.SrcImgW
		|| stInRatioPara.stInWnd.s32Height != stInRatioPara.SrcImgH)
	{
		stInRatioPara.AspectWidth =
			stInPara.AspectWidth*stInPara.SrcImgW/stInPara.stInWnd.s32Width;
		stInRatioPara.AspectHeight =
			stInPara.AspectHeight*stInPara.SrcImgH/stInPara.stInWnd.s32Height;

		stInRatioPara.SrcImgW = stInPara.stInWnd.s32Width;
		stInRatioPara.SrcImgH = stInPara.stInWnd.s32Height;
	}
	else
	{
		stInRatioPara.AspectWidth = stInPara.AspectWidth;
		stInRatioPara.AspectHeight = stInPara.AspectHeight;
	}

	stInRatioPara.stScreen = stInPara.stScreen;
	stInRatioPara.stUsrAsp.bUserDefAspectRatio = HI_FALSE;

	if (stInPara.eAspMode == HI_DRV_ASP_RAT_MODE_FULL)
	{
		stOutPara.stOutWnd.s32X = stInPara.stOutWnd.s32X;
		stOutPara.stOutWnd.s32Y = stInPara.stOutWnd.s32Y;
		stOutPara.stOutWnd.s32Width = stInPara.stOutWnd.s32Width;
		stOutPara.stOutWnd.s32Height = stInPara.stOutWnd.s32Height;
	}
	else if (stInPara.eAspMode == HI_DRV_ASP_RAT_MODE_LETTERBOX)
	{
		s32Ret = WIN_RATIO_GetAspCfg(&stOutPara,&stInRatioPara);
		if (s32Ret != HI_SUCCESS)
		{
			HI_FATAL_WIN("GetAspCfg Failed\n");
		}
	}
	else
	{
		stOutPara.stOutWnd.s32X = stInPara.stOutWnd.s32X;
		stOutPara.stOutWnd.s32Y = stInPara.stOutWnd.s32Y;
		stOutPara.stOutWnd.s32Width = stInPara.stOutWnd.s32Width;
		stOutPara.stOutWnd.s32Height = stInPara.stOutWnd.s32Height;
		HI_FATAL_WIN("Invalid aspect mode %d\n",stInPara.eAspMode);
	}

	pstRevisedRect->s32X = stOutPara.stOutWnd.s32X;
	pstRevisedRect->s32Y = stOutPara.stOutWnd.s32Y;
	pstRevisedRect->s32Width = stOutPara.stOutWnd.s32Width;
	pstRevisedRect->s32Height  = stOutPara.stOutWnd.s32Height;

	return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetFrostGlassLevel(HI_HANDLE hWindow, HI_DRV_WINDOW_FROST_E eFrostMode)
{
	HI_S32 s32Ret = HI_FAILURE;	
    WIN_FROST_MODE_S   stWinFrostMode = {0};
	
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
	
	if (HI_DRV_WINDOW_FROST_BUTT <= eFrostMode)
	{
		HI_ERR_WIN("para eFrostMode is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
	}
	
    CHECK_VO_INIT();

    stWinFrostMode.hWindow = hWindow;
    stWinFrostMode.eFrostMode = eFrostMode;

    s32Ret = ioctl(g_VoDevFd, CMD_WIN_SET_FROST_MODE, &stWinFrostMode);

	return s32Ret;
}

HI_S32 HI_MPI_WIN_GetFrostGlassLevel(HI_HANDLE hWindow, HI_DRV_WINDOW_FROST_E *peFrostMode)
{
	HI_S32 s32Ret = HI_FAILURE;
    WIN_FROST_MODE_S   stWinFrostMode = {0};

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (!peFrostMode)
    {
        HI_ERR_WIN("para peFrostMode is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    stWinFrostMode.hWindow = hWindow;

    s32Ret = ioctl(g_VoDevFd, CMD_WIN_GET_FROST_MODE, &stWinFrostMode);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
	
    *peFrostMode = stWinFrostMode.eFrostMode;
	
	return s32Ret;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

