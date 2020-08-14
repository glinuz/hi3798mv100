/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : hi_gfx_comm.h
Version          : version 1.0
Author           :
Created          : 2016/08/13
Description      : Describes adp file. CNcomment:API跨平台适配 CNend\n
Function List    :

History          :
Date                Author                Modification
2016/08/13         y00181162
******************************************************************************/


#ifndef  _HI_GFX_COMM_H_
#define  _HI_GFX_COMM_H_


/***************************** SDK Version Macro Definition *********************/


/*********************************add include here******************************/
#if defined(CONFIG_GFX_MEM_MMZ)
   #include <string.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include <fcntl.h>
   #include <errno.h>
   #include <sys/ioctl.h>
   #include <sys/syscall.h>
   #include <sys/mman.h>
   #include "mpi_mmz.h"
#elif defined(CONFIG_GFX_MEM_ION)
   #include <sys/ioctl.h>
   #include <sys/syscall.h>
   #include <sys/mman.h>
   #include <fcntl.h>
   #include <errno.h>
   #include <string.h>
   #include <stdlib.h>
#else/**CONFIG_GFX_BVT_SDK**/
    #include <time.h>
    #include <sys/ioctl.h>
    #include <sys/mman.h>
    #include <string.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

#ifdef CONFIG_GFX_ANDROID_SDK
    #include <utils/Log.h>
#else
    #include <unistd.h>
    #include "hi_common.h"
#endif

#ifdef CONFIG_GFX_256BYTE_ALIGN /** 定义到Makefile和Android Makefile中 **/
#include "hi_math.h"
#endif

#include "hi_type.h"

/***************************** Macro Definition ******************************/
#if defined(HI_LOG_SUPPORT) && (0 == HI_LOG_SUPPORT)
	   #define GFX_Printf( fmt,args...)
#else
    #ifdef CONFIG_GFX_ANDROID_SDK
        #if 0
        /** this is defined at *.c that will used **/
        #define LOG_TAG    "libjpeg"
        #endif
        #define GFX_Printf( fmt, args... )\
        do {\
             ALOGE(fmt, ##args );\
        }while (0)
    #else
        #define GFX_Printf( fmt, args... )\
        do {\
             fprintf(stderr,fmt, ##args );\
        } while (0)
    #endif
#endif


#ifdef CFG_HI_GFX_DEBUG_LOG_SUPPORT
   #ifdef LOG_TAG
     #undef LOG_TAG
   #endif
   #define LOG_TAG "libhi_gfx"
#endif


/**
 **打印函数
 **/
#if defined(CONFIG_GFX_MEM_MMZ) || defined(CONFIG_GFX_MEM_ION)

    #define ConvertID(module_id) (module_id + HI_ID_TDE - HIGFX_TDE_ID)
    #ifdef HI_ADVCA_FUNCTION_RELEASE
        #define HI_GFX_COMM_LOG_FATAL(module_id,fmt...)
        #define HI_GFX_COMM_LOG_ERROR(module_id,fmt...)
        #define HI_GFX_COMM_LOG_WARNING(module_id,fmt...)
        #define HI_GFX_COMM_LOG_INFO(module_id,fmt...)
    #else
        #define HI_GFX_COMM_LOG_FATAL(module_id, fmt...)   HI_TRACE(HI_LOG_LEVEL_FATAL,    ConvertID(module_id), fmt)
        #define HI_GFX_COMM_LOG_ERROR(module_id,fmt...)    HI_TRACE(HI_LOG_LEVEL_ERROR,    ConvertID(module_id), fmt)
        #define HI_GFX_COMM_LOG_WARNING(module_id,fmt...)  HI_TRACE(HI_LOG_LEVEL_WARNING,  ConvertID(module_id), fmt)
        #define HI_GFX_COMM_LOG_INFO(module_id,fmt...)     HI_TRACE(HI_LOG_LEVEL_INFO,     ConvertID(module_id), fmt)
    #endif
    #define ION_IOC_MAGIC                 'I'
    #define ION_IOC_ALLOC                 _IOWR(ION_IOC_MAGIC, 0,  struct ion_allocation_data)
    #define ION_IOC_FREE                  _IOWR(ION_IOC_MAGIC, 1,  struct ion_handle_data)
    #define ION_IOC_MAP                   _IOWR(ION_IOC_MAGIC, 2,  struct ion_fd_data)
    #define ION_IOC_SYNC                  _IOWR(ION_IOC_MAGIC, 7,  struct ion_fd_data)
    #define ION_IOC_PHYS                  _IOWR(ION_IOC_MAGIC, 8,  struct ion_phys_data)
    #define ION_IOC_MAP_IOMMU             _IOWR(ION_IOC_MAGIC, 9,  struct ion_map_iommu_data)
    #define ION_IOC_UNMAP_IOMMU           _IOWR(ION_IOC_MAGIC, 10, struct ion_map_iommu_data)
    #define ION_FLAG_CACHED               1
    #define ION_FLAG_CACHED_NEEDS_SYNC    2
    #define ION_HEAP_SYSTEM_MASK          (1 << ION_HEAP_TYPE_SYSTEM)
    #define ION_HEAP_SYSTEM_CONTIG_MASK   (1 << ION_HEAP_TYPE_SYSTEM_CONTIG)
    #define ION_HEAP_CARVEOUT_MASK        (1 << ION_HEAP_TYPE_CARVEOUT)
    #define ION_HEAP_TYPE_DMA_MASK        (1 << ION_HEAP_TYPE_DMA)
    #define ION_HEAP_CMA_MASK             (1 << ION_HEAP_ID_CMA)
    #define ION_NUM_HEAP_IDS              (sizeof(unsigned int) * 8)

#else/** CONFIG_GFX_BVT_SDK **/

    #define ConvertID(module_id) (module_id + HI_ID_TDE - HIGFX_TDE_ID)

    #ifdef HI_ADVCA_FUNCTION_RELEASE
        #define HI_GFX_COMM_LOG_FATAL(module_id,fmt...)
        #define HI_GFX_COMM_LOG_ERROR(module_id,fmt...)
        #define HI_GFX_COMM_LOG_WARNING(module_id,fmt...)
        #define HI_GFX_COMM_LOG_INFO(module_id,fmt...)

    #else
        #define HI_GFX_COMM_LOG_FATAL(module_id, fmt...)        HI_TRACE(HI_LOG_LEVEL_FATAL,      ConvertID(module_id), fmt)
        #define HI_GFX_COMM_LOG_ERROR(module_id,fmt...)         HI_TRACE(HI_LOG_LEVEL_ERROR,      ConvertID(module_id), fmt)
        #define HI_GFX_COMM_LOG_WARNING(module_id,fmt...)       HI_TRACE(HI_LOG_LEVEL_WARNING,    ConvertID(module_id), fmt)
        #define HI_GFX_COMM_LOG_INFO(module_id,fmt...)          HI_TRACE(HI_LOG_LEVEL_INFO,       ConvertID(module_id), fmt)
    #endif
        #define IOC_MMB_ALLOC                    _IOWR('m', 10,  mmb_info)
        #define IOC_MMB_FREE                     _IOW ('m', 12,  mmb_info)
        #define IOC_MMB_USER_REMAP               _IOWR('m', 20,  mmb_info)
        #define IOC_MMB_USER_REMAP_CACHED        _IOWR('m', 21,  mmb_info)
        #define IOC_MMB_USER_UNMAP               _IOWR('m', 22,  mmb_info)
        #define IOC_MMB_USER_GETPHYADDR          _IOWR('m', 23,  mmb_info)
        #define IOC_MMB_FLUSH_DCACHE             _IO  ('c', 40)
#endif


#ifndef CLOCK_MONOTONIC_RAW
    #define CLOCK_MONOTONIC_RAW    4
#endif

/** @} */    /*! <!-- Macro Definition end */


/*************************** Enum Definition ****************************/

/** \addtogroup      GFX COMM */
/** @{ */  /** <!-- 【GFX COMM】 */

/** enum of the module ID */
/** CNcomment:每个模块的ID号 CNend */
typedef enum tagHIGFX_MODE_ID_E
{
    HIGFX_TDE_ID      = 0,    /**< TDE ID           */
    HIGFX_JPGDEC_ID,          /**< JPEG DECODE ID   */
    HIGFX_JPGENC_ID,          /**< JPEG_ENCODE ID   */
    HIGFX_FB_ID,              /**< FRAMEBUFFER ID   */
    HIGFX_PNG_ID,             /**< PNG ID           */
    HIGFX_BUTT_ID,
}HIGFX_MODE_ID_E;


typedef enum tagHIGFX_MODE_MEM_E
{
    HIGFX_MODE_ALLOC    = 0,
    HIGFX_MODE_FREE,
    HIGFX_MODE_MAP,
    HIGFX_MODE_UNMAP,
    HIGFX_MODE_BUTT,
}HIGFX_MODE_MEM_E;


/** @} */  /*! <!-- enum Definition end */

/*************************** Structure Definition ****************************/


/** \addtogroup      GFX COMM */
/** @{ */  /** <!-- 【GFX COMM】 */
#if defined(CONFIG_GFX_MEM_MMZ) || defined(CONFIG_GFX_MEM_ION)
    typedef int ion_user_handle_t;
    typedef unsigned long ion_phys_addr_t;
    typedef struct
    {
        ion_user_handle_t pIonHandle;
        HI_S32 s32MapFd;
        HI_U32 u32Phyaddr;
        HI_VOID *pViraddr;
        HI_U32 u32Size;
    }HI_GFX_MEM_HANDLE_S;

    struct ion_allocation_data {
        size_t len;
        size_t align;
        unsigned int heap_id_mask;
        unsigned int flags;
        ion_user_handle_t handle;
    };

    struct ion_fd_data {
        ion_user_handle_t handle;
        int fd;
    };

    struct ion_handle_data {
        ion_user_handle_t handle;
    };

    struct ion_phys_data {
        ion_user_handle_t handle;
        size_t len;
        ion_phys_addr_t phys_addr;
    };

    struct iommu_map_format {
        unsigned long iova_start;
        unsigned long iova_size;
        unsigned long iommu_ptb_base;
        unsigned long iommu_iova_base;
        unsigned long phys_page_line;
        unsigned long virt_page_line;
        unsigned long is_tile;
    };

    struct ion_map_iommu_data {
        ion_user_handle_t handle;
        struct iommu_map_format format;
    };

    enum ion_heap_type {
        ION_HEAP_TYPE_SYSTEM,
        ION_HEAP_TYPE_SYSTEM_CONTIG,
        ION_HEAP_TYPE_CARVEOUT,
        ION_HEAP_TYPE_CHUNK,
        ION_HEAP_TYPE_DMA,
        ION_HEAP_ID_CMA = 15,
        ION_NUM_HEAPS   = 31,
    };
#else/**CONFIG_GFX_BVT_SDK**/
    typedef struct
    {
        unsigned long phys_addr;
        unsigned long align;
        unsigned long size;
        unsigned int order;
        void *mapped;
        union
        {
            struct
            {
                unsigned long prot  :8;
                unsigned long flags :12;
            };
            unsigned long w32_stuf;
        };
        char mmb_name[16];
        char mmz_name[32];
        unsigned long gfp;
    } mmb_info;
#endif



/** @} */  /*! <!-- Structure Definition end */


/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/

/** \addtogroup      GFX COMM */
/** @{ */  /** <!-- 【GFX COMM】 */


/**
\brief get the system time,not use gettimeofday to get time. CNcomment:获取系统时间，不使用gettimeofday的原因在于这个函数获取的时间有可能被客户的后台程序修改 CNend
\attention \n

\param[out] *pu32TimeMs  CNcomment:获取到的时间ms CNend\n
\param[out] *pu32TimeUs  CNcomment:获取到的时间us CNend\n
\retval ::HI_SUCCESS
\retval ::HI_FAILURE

\see \n
::HI_GFX_GetTimeStamp
*/
static inline HI_S32 HI_GFX_GetTimeStamp(HI_U32 *pu32TimeMs, HI_U32 *pu32TimeUs)
{

        HI_S32 ret;
        struct timespec timenow = {0, 0};
        clockid_t id = CLOCK_MONOTONIC_RAW;

        if(HI_NULL == pu32TimeMs)
        {
            return HI_FAILURE;
        }

        ret = clock_gettime(id, &timenow);
        if(ret < 0)
        {
            return HI_FAILURE;
        }

        *pu32TimeMs = (HI_U32)(timenow.tv_sec*1000 + timenow.tv_nsec/1000000);

        if(HI_NULL == pu32TimeUs)
        {
            return HI_SUCCESS;
        }

        *pu32TimeUs = (HI_U32)(timenow.tv_sec*1000000 + timenow.tv_nsec/1000);

        return HI_SUCCESS;

}

/**
\brief get stride from input width. CNcomment:根据图片的宽度来获取stride大小 CNend
\attention \n

\param[out] *pu32TimeMs  CNcomment:获取到的时间ms CNend\n
\param[out] *pu32TimeUs  CNcomment:获取到的时间us CNend\n
\retval ::HI_SUCCESS
\retval ::HI_FAILURE

\see \n
::HI_GFX_GetStride
*/
static inline HI_VOID HI_GFX_GetStride(HI_U32 u32SrcW, HI_U32 *pu32Stride,HI_U32 u32Align)
{

    #ifdef CONFIG_GFX_256BYTE_ALIGN
        *pu32Stride = u32Align;/** deal with warning **/
        *pu32Stride = HI_SYS_GET_STRIDE(u32SrcW);
    #else
        *pu32Stride = (u32SrcW + u32Align - 1) & (~(u32Align - 1));
    #endif
}

/***************************************************************************
* func          : HI_GFX_LOG_Display
* description   : display buffer information
* retval        :
* others:       : NA
****************************************************************************/
static inline HI_VOID HI_GFX_LOG_Display(HI_U32 u32Size, HI_U32 u32VirBuf, HI_U32 u32Buf, const HI_CHAR* pBufName, HIGFX_MODE_MEM_E eMemMod)
{
    UNUSED(u32Size);
    UNUSED(u32VirBuf);
    UNUSED(u32Buf);
    UNUSED(pBufName);
    UNUSED(eMemMod);
#ifdef CFG_HI_GFX_DEBUG_LOG_SUPPORT
    GFX_Printf("\n==============================================================\n");
    switch(eMemMod){
        case HIGFX_MODE_ALLOC:
            if(NULL != pBufName){
                GFX_Printf("alloc %s buffer information\n",pBufName);
            }else{
                GFX_Printf("alloc buffer information\n");
            }
            GFX_Printf("buffer[0x%x  0x%x] size = %d\n",u32Buf,u32Buf + u32Size,u32Size);
            break;
        case HIGFX_MODE_FREE:
            if(NULL != pBufName){
                GFX_Printf("free %s buffer information\n",pBufName);
            }else{
                GFX_Printf("free buffer information\n");
            }
            GFX_Printf("buffer 0x%x\n",u32Buf);
            break;
         case HIGFX_MODE_MAP:
            if(NULL != pBufName){
                GFX_Printf("map %s buffer information\n",pBufName);
            }else{
                GFX_Printf("map buffer information\n");
            }
            GFX_Printf("buffer 0x%x vir buffer 0x%x\n",u32Buf,u32VirBuf);
            break;
         case HIGFX_MODE_UNMAP:
            if(NULL != pBufName){
                GFX_Printf("unmap %s buffer information\n",pBufName);
            }else{
                GFX_Printf("unmap buffer information\n");
            }
            GFX_Printf("buffer 0x%x\n",u32Buf);
            break;
         default:
            break;
    }
    GFX_Printf("==============================================================\n");
#endif
    return;
}


#ifdef CONFIG_GFX_MEM_MMZ
/***************************************************************************
* func          : HI_GFX_FreeMem
* description   : free the mem that has alloced
                  CNcomment: 释放分配过的内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 HI_GFX_FreeMem(HI_U32 u32PhyAddr, HI_BOOL bMmu)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if(0 == u32PhyAddr){
        return HI_FAILURE;
    }
#ifdef CONFIG_GFX_MMU_SUPPORT
    if(bMmu){
        s32Ret = HI_MPI_SMMU_Delete(u32PhyAddr);
    }
    else
#endif
    {
        s32Ret = HI_MPI_MMZ_Delete(u32PhyAddr);
    }
    HI_GFX_LOG_Display(0,0,u32PhyAddr,NULL,HIGFX_MODE_FREE);
    return s32Ret;
}


/***************************************************************************
* func          : HI_GFX_AllocMem
* description   : alloc the mem that need
                  CNcomment: 分配需要的内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_U32 HI_GFX_AllocMem(HI_U32 u32Size , HI_U32 u32Align, HI_CHAR* pZoneName, HI_CHAR *pName, HI_BOOL *pbMmu)
{

    HI_U32 u32PhyAddr = 0;

#ifdef CONFIG_GFX_256BYTE_ALIGN
    HI_U32 u32MMZAlign  = 256;
#else
    HI_U32 u32MMZAlign  = u32Align;
#endif
    UNUSED(pZoneName);/** deal with warning **/

    *pbMmu = HI_FALSE;

    if(NULL == pName){
        GFX_Printf("=====the buffer name is null!\n");
        return 0;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    if( (NULL != pZoneName) && (0 == strncmp(pZoneName, "iommu", strlen("iommu"))) ){
        *pbMmu = HI_TRUE;
        u32PhyAddr = HI_MPI_SMMU_New(pName, u32Size);
    }else
#endif
    {
        *pbMmu = HI_FALSE;
        u32PhyAddr = HI_MPI_MMZ_New(u32Size, u32MMZAlign,NULL,pName);
    }
    HI_GFX_LOG_Display(u32Size,0,u32PhyAddr,pName,HIGFX_MODE_ALLOC);
    return u32PhyAddr;
}

/***************************************************************************
* func          : HI_GFX_Map
* description   :
                  CNcomment: 映射不带cache的虚拟内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_VOID *HI_GFX_Map(HI_U32 u32PhyAddr, HI_BOOL bMmu)
{
    if(0 == u32PhyAddr){
        return NULL;
    }
    HI_GFX_LOG_Display(0,0,u32PhyAddr,0,HIGFX_MODE_MAP);
#ifdef CONFIG_GFX_MMU_SUPPORT
    if(bMmu){
        return HI_MPI_SMMU_Map(u32PhyAddr, HI_FALSE);
    }
    else
#endif
    {
        return HI_MPI_MMZ_Map(u32PhyAddr, HI_FALSE);
    }
}

/***************************************************************************
* func          : HI_GFX_MapCached
* description   :
                  CNcomment: 映射带cache的虚拟内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_VOID *HI_GFX_MapCached(HI_U32 u32PhyAddr, HI_BOOL bMmu)
{
    if(0 == u32PhyAddr){
        return NULL;
    }
    HI_GFX_LOG_Display(0,0,u32PhyAddr,0,HIGFX_MODE_MAP);
#ifdef CONFIG_GFX_MMU_SUPPORT
    if(bMmu){
        return HI_MPI_SMMU_Map(u32PhyAddr, HI_TRUE);
    }
    else
#endif
    {
        return HI_MPI_MMZ_Map(u32PhyAddr, HI_TRUE);
    }
}

/***************************************************************************
* func          : HI_GFX_Unmap
* description   : un map phy ddr
                  CNcomment: 逆映射物理地址 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 HI_GFX_Unmap(HI_U32 u32PhyAddr, HI_BOOL bMmu)
{
    if(0 == u32PhyAddr){
        return HI_FAILURE;
    }
    HI_GFX_LOG_Display(0,0,u32PhyAddr,0,HIGFX_MODE_UNMAP);
#ifdef CONFIG_GFX_MMU_SUPPORT
    if(bMmu){
        return HI_MPI_SMMU_Unmap(u32PhyAddr);
    }
    else
#endif
    {
        return HI_MPI_MMZ_Unmap(u32PhyAddr);
    }
}
/***************************************************************************
* func          : HI_GFX_Flush
* description   :
                  CNcomment: 地址内容刷新 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 HI_GFX_Flush(HI_U32 u32PhyAddr, HI_BOOL bMmu)
{
#ifdef CONFIG_GFX_MMU_SUPPORT
    if(bMmu){
        return HI_MPI_SMMU_Flush(u32PhyAddr);
    }
    else
#endif
    {
        return HI_MPI_MMZ_Flush(u32PhyAddr);
    }
}

/***************************************************************************
* func          : HI_GFX_GetPhyaddr
* description   : get phy ddr from vir ddr
                  CNcomment: 从虚拟地址中获取物理地址 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 HI_GFX_GetPhyaddr(HI_VOID * pVir, HI_U32 *pPhyaddr, HI_U32 *pu32Size, HI_BOOL *pbMmu)
{
    HI_S32 s32Ret = HI_SUCCESS;

    *pbMmu = HI_FALSE;

    if(NULL == pVir || NULL == pPhyaddr){
        return HI_FAILURE;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    s32Ret    = HI_MPI_SMMU_GetSmmuAddrByMapAddr(pVir,pPhyaddr,pu32Size);
    *pbMmu    = HI_TRUE;
#else
    s32Ret = HI_MPI_MMZ_GetPhyAddr(pVir,pPhyaddr,pu32Size);
    *pbMmu = HI_FALSE;
#endif

    return s32Ret;
}

#elif defined(CONFIG_GFX_MEM_ION)

/***************************************************************************
* func          : gfx_mem_open
* description   : open ion device
                  CNcomment: 打开ion设备 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_mem_open()
{
    int fd = open("/dev/ion", O_RDWR);
    if(fd < 0){
         GFX_Printf("open /dev/ion failed!\n");
    }
    return fd;
}
/***************************************************************************
* func          : gfx_mem_close
* description   : close ion device
                  CNcomment: 关闭ion设备 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_VOID gfx_mem_close(int fd)
{
    if(fd >= 0){
        close(fd);
    }
}
/***************************************************************************
* func          : gfx_ion_ioctl
* description   : the ion ioctl
                  CNcomment: ioctl操作 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_ion_ioctl(int fd, int req, void *arg)
{
    int ret = ioctl(fd, req, arg);
    if(ret < 0) {
       GFX_Printf("ioctl %x failed with code %d\n", req,ret);
       return -errno;
    }
    return ret;
}

/***************************************************************************
* func          : gfx_ion_alloc
* description   : alloc mem
                  CNcomment: 内存分配 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_ion_alloc(int fd, size_t len, size_t align, unsigned int heap_mask, unsigned int flags, ion_user_handle_t *handle)
{
    int ret = 0;
    struct ion_allocation_data data = {
            .len   = len,
            .align = align,
            .heap_id_mask = heap_mask,
            .flags = flags,
        };

    ret = gfx_ion_ioctl(fd, ION_IOC_ALLOC, &data);
    if(ret < 0){
       return ret;
    }
    *handle = data.handle;
    return ret;
}

/***************************************************************************
* func          : gfx_ion_free
* description   : fremm mem
                  CNcomment: 内存释放 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_ion_smmu_free(int fd, ion_user_handle_t handle, unsigned long Phyaddr)
{
#ifdef CONFIG_GFX_MMU_SUPPORT
    struct ion_map_iommu_data map_iommu;

    memset(&map_iommu, 0, sizeof(struct ion_map_iommu_data));
    map_iommu.handle            = handle;
    map_iommu.format.iova_start = Phyaddr;
    return gfx_ion_ioctl(fd, ION_IOC_UNMAP_IOMMU, &map_iommu);
#else
    if(0 == fd || 0 == handle || 0 == Phyaddr){
        return HI_SUCCESS;
    }
    return HI_SUCCESS;
#endif
}

/***************************************************************************
* func          : gfx_ion_free
* description   : fremm mem
                  CNcomment: 内存释放 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_ion_free(int fd, ion_user_handle_t handle)
{
    struct ion_handle_data data = {
             .handle = handle,
        };
    return gfx_ion_ioctl(fd, ION_IOC_FREE, &data);
}


/***************************************************************************
* func          : gfx_ion_phys
* description   : get phy ddr
                  CNcomment: 获取连续的物理地址 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_ion_phys(int fd, ion_user_handle_t handle, HI_U32 *phys_addr, HI_U32 *len)
{
    int ret = 0;

    struct ion_phys_data data = {
            .handle = handle,
        };

    ret = gfx_ion_ioctl(fd, ION_IOC_PHYS, &data);
    if (ret < 0){
       return ret;
    }
    *phys_addr = data.phys_addr;
    *len = data.len;

    return ret;
}

/***************************************************************************
* func          : gfx_ion_smmu
* description   : get smmu ddr
                  CNcomment: 获取smmu地址 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_ion_smmu(int fd, ion_user_handle_t handle, HI_U32 *phys_addr, HI_U32 *len)
{
    int ret = 0;
    struct ion_map_iommu_data map_iommu;

    memset(&map_iommu, 0, sizeof(struct ion_map_iommu_data));
    map_iommu.handle = handle;
    ret = gfx_ion_ioctl(fd, ION_IOC_MAP_IOMMU, &map_iommu);
    if (ret < 0) {
        return ret;
    }

    *phys_addr = map_iommu.format.iova_start;
    *len       = map_iommu.format.iova_size;

    return ret;
}

/***************************************************************************
* func          : gfx_ion_map
* description   : map phy to vir
                  CNcomment: 内存映射 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_ion_map(int fd, ion_user_handle_t handle, size_t length, int prot, int flags, off_t offset, unsigned char **ptr, int *map_fd)
{
    int ret = 0;
    struct ion_fd_data data = {
            .handle = handle,
        };

    ret = gfx_ion_ioctl(fd, ION_IOC_MAP, &data);
    if(ret < 0){
        return ret;
    }

    *map_fd = data.fd;
    if(*map_fd < 0) {
        GFX_Printf("map ioctl returned negative fd\n");
        return -EINVAL;
    }
    *ptr = mmap(NULL, length, prot, flags, *map_fd, offset);
    if(*ptr == MAP_FAILED) {
        GFX_Printf("mmap failed\n");
        return -errno;
    }
    return ret;

}


/***************************************************************************
* func          : gfx_ion_sync_fd
* description   : sync
                  CNcomment: 同步 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 gfx_ion_sync_fd(int fd, int handle_fd)
{
    struct ion_fd_data data = {
        .fd = handle_fd,
    };
    return gfx_ion_ioctl(fd, ION_IOC_SYNC, &data);
}


/***************************************************************************
* func          : HI_GFX_FreeMem
* description   : free the mem that has alloced
                  CNcomment: 释放分配过的内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 HI_GFX_FreeMem(HI_S32 s32MMZDev,HI_U32 u32Phyaddr, HI_VOID *pMemHandle, HI_BOOL bMmu)
{
    HI_S32 s32Ret = HI_SUCCESS;

    UNUSED(u32Phyaddr);

    HI_GFX_LOG_Display(0,0,u32Phyaddr,0,HIGFX_MODE_FREE);

    if(bMmu){
       s32Ret = gfx_ion_smmu_free(s32MMZDev, ((HI_GFX_MEM_HANDLE_S *)pMemHandle)->pIonHandle,u32Phyaddr);
    }
    s32Ret |= gfx_ion_free(s32MMZDev, ((HI_GFX_MEM_HANDLE_S *)pMemHandle)->pIonHandle);
    if(s32Ret){
        return HI_FAILURE;
    }

    free(pMemHandle);
    pMemHandle = NULL;

    return HI_SUCCESS;
}


/***************************************************************************
* func          : HI_GFX_AllocMem
* description   : alloc the mem that need
                  CNcomment: 分配需要的内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_U32 HI_GFX_AllocMem(HI_S32 s32MMZDev,HI_U32 u32Size , HI_U32 u32Align, HI_CHAR* pZoneName, HI_CHAR *pName, HI_VOID **ppMemHandle, HI_BOOL *pbMmu)
{
    HI_GFX_MEM_HANDLE_S *pstMemHandle = NULL;
    HI_U32   u32Phyaddr = 0;
    HI_S32 s32Ret       = HI_SUCCESS;
    HI_U32 u32MMZAlign    = u32Align;
    HI_S32 s32MemSize   = 0;

    UNUSED(pName);

#ifdef CONFIG_GFX_256BYTE_ALIGN
    u32MMZAlign = 256;
#endif

    s32MemSize   = sizeof(HI_GFX_MEM_HANDLE_S);
    pstMemHandle = (HI_GFX_MEM_HANDLE_S *)malloc(s32MemSize);
    if(NULL == pstMemHandle){
        *ppMemHandle = NULL;
        return 0;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    if((NULL != pZoneName) && (0 == strncmp(pZoneName, "iommu", strlen("iommu")))){
       s32Ret = gfx_ion_alloc(s32MMZDev, u32Size, u32MMZAlign, ION_HEAP_SYSTEM_MASK, ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC, &(pstMemHandle->pIonHandle));
    }else
#endif
    {
        s32Ret = gfx_ion_alloc(s32MMZDev, u32Size, u32MMZAlign, ION_HEAP_CMA_MASK, ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC, &(pstMemHandle->pIonHandle));
    }

    if(s32Ret){
        GFX_Printf("ion_alloc failed!size:0x%x\n", u32Size);
        free(pstMemHandle);
        pstMemHandle = NULL;
        *ppMemHandle = NULL;
        return 0;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    if( (NULL != pZoneName) && (0 == strncmp(pZoneName, "iommu", strlen("iommu"))) ){
        *pbMmu = HI_TRUE;
        s32Ret = gfx_ion_smmu(s32MMZDev, pstMemHandle->pIonHandle, &u32Phyaddr, &u32Size);
        if(s32Ret){
            GFX_Printf("ion_phys failed!\n");
            gfx_ion_free(s32MMZDev, pstMemHandle->pIonHandle);
            free(pstMemHandle);
            pstMemHandle = NULL;
            *ppMemHandle = NULL;
            return 0;
        }
    }else
#endif
    {
        *pbMmu = HI_FALSE;
        s32Ret = gfx_ion_phys(s32MMZDev, pstMemHandle->pIonHandle, &u32Phyaddr, &u32Size);
        if(s32Ret){
            GFX_Printf("ion_phys failed!\n");
            gfx_ion_free(s32MMZDev, pstMemHandle->pIonHandle);
            free(pstMemHandle);
            pstMemHandle = NULL;
            *ppMemHandle = NULL;
            return 0;
        }
    }

    pstMemHandle->u32Phyaddr = u32Phyaddr;
    pstMemHandle->u32Size    = u32Size;
    *ppMemHandle             = pstMemHandle;

    HI_GFX_LOG_Display(u32Size,0,u32Phyaddr,pName,HIGFX_MODE_ALLOC);

    return u32Phyaddr;

}


/***************************************************************************
* func          : HI_GFX_Map
* description   :
                  CNcomment: 映射不带cache的虚拟内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_VOID *HI_GFX_Map(HI_S32 s32MMZDev,HI_U32 u32PhyAddr, HI_VOID *pMemHandle)
{
    unsigned char *ptr = HI_NULL;
    HI_S32 s32Ret = HI_SUCCESS;

    UNUSED(u32PhyAddr);

    s32Ret = gfx_ion_map(s32MMZDev,                                       \
                         ((HI_GFX_MEM_HANDLE_S *)pMemHandle)->pIonHandle, \
                         ((HI_GFX_MEM_HANDLE_S *)pMemHandle)->u32Size,    \
                         PROT_READ | PROT_WRITE, MAP_SHARED,              \
                         0,                                               \
                         &ptr,                                            \
                         &(((HI_GFX_MEM_HANDLE_S *)pMemHandle)->s32MapFd));
    if(s32Ret){
        GFX_Printf("ion_map failed!\n");
        return NULL;
    }
    ((HI_GFX_MEM_HANDLE_S *)pMemHandle)->pViraddr = ptr;

    HI_GFX_LOG_Display(0,(unsigned long)ptr,u32PhyAddr,NULL,HIGFX_MODE_MAP);

    return ptr;

}

/***************************************************************************
* func          : HI_GFX_MapCached
* description   :
                  CNcomment: 映射带cache的虚拟内存 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_VOID *HI_GFX_MapCached(HI_S32 s32MMZDev,HI_U32 u32PhyAddr, HI_VOID *pMemHandle)
{
    return HI_GFX_Map(s32MMZDev,u32PhyAddr,pMemHandle);
}

/***************************************************************************
* func          : HI_GFX_Unmap
* description   : un map phy ddr
                  CNcomment: 逆映射物理地址 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 HI_GFX_Unmap(HI_S32 s32MMZDev,HI_U32 u32PhyAddr, HI_VOID *pMemHandle)
{
    HI_U32 u32Size = 0;
    HI_S32 s32Ret  = 0;
    UNUSED(u32PhyAddr);
    UNUSED(s32MMZDev);

    if(NULL == pMemHandle){
        return HI_FAILURE;
    }
    u32Size = ((HI_GFX_MEM_HANDLE_S *)pMemHandle)->u32Size;
    if(0 == u32Size){
        return HI_FAILURE;
    }

    s32Ret  = munmap(((HI_GFX_MEM_HANDLE_S *)pMemHandle)->pViraddr, u32Size);
    close(((HI_GFX_MEM_HANDLE_S *)pMemHandle)->s32MapFd);
    ((HI_GFX_MEM_HANDLE_S *)pMemHandle)->s32MapFd = 0;

    HI_GFX_LOG_Display(0,0,u32PhyAddr,NULL,HIGFX_MODE_UNMAP);

    if(0 != s32Ret){
       return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***************************************************************************
* func          : HI_GFX_Flush
* description   :
                  CNcomment: 地址内容刷新 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 HI_GFX_Flush(HI_S32 s32MMZDev,HI_U32 u32PhyAddr, HI_VOID *pMemHandle)
{
    UNUSED(u32PhyAddr);
    return gfx_ion_sync_fd(s32MMZDev, ((HI_GFX_MEM_HANDLE_S *)pMemHandle)->s32MapFd);
}

/***************************************************************************
* func          : HI_GFX_GetPhyaddr
* description   : get phy ddr from vir ddr
                  CNcomment: 从虚拟地址中获取物理地址 CNend\n
* retval        : HI_SUCCESS 成功
* retval        : HI_FAILURE 失败
* others:       : NA
****************************************************************************/
static inline HI_S32 HI_GFX_GetPhyaddr(HI_S32 s32MMZDev,HI_VOID * pVir, HI_U32 *pPhyaddr, HI_U32 *pu32Size, HI_VOID *pMemHandle)
{
    /** deal with warning **/
    if(-1 == s32MMZDev || NULL == pVir || NULL == pPhyaddr || NULL == pu32Size || NULL == pMemHandle){
        return HI_FAILURE;
    }
    return HI_FAILURE;
}

#else/** CONFIG_GFX_BVT_SDK **/

static inline HI_S32 gfx_mem_open()
{
        int fd = open("/dev/mmz_userdev", O_RDWR);
        if (fd < 0)
        {
             GFX_Printf("open /dev/mmz_userdev failed!\n");
        }
        return fd;
}

static inline HI_VOID gfx_mem_close(int fd)
{
        close(fd);
}

static inline HI_VOID* GFX_MMZ_New(HI_S32 mmz,HI_U32 size,HI_U32 align,HI_CHAR *mmz_name, HI_CHAR *mmb_name)
{

          mmb_info   mmi;

          memset(&mmi,0,sizeof(mmi));

          mmi.size = size;
          mmi.align =align;

          if (mmz_name != NULL)
          {
              strncpy(mmi.mmz_name, mmz_name, strlen(mmi.mmz_name));
              mmi.mmz_name[strlen(mmz_name)]='\0';
          }

          if (mmb_name != NULL)
          {
              strncpy(mmi.mmb_name, mmb_name, strlen(mmi.mmb_name));
              mmi.mmz_name[strlen(mmb_name)]='\0';
          }

          if (ioctl(mmz, IOC_MMB_ALLOC, &mmi) !=0)
          {
             return NULL;
          }

          return (void *)mmi.phys_addr;

}

static inline HI_VOID *GFX_MMZ_Map(HI_S32 mmz, HI_VOID *phyAddr, HI_S32 cached)
{


          int s32Ret;

          mmb_info   mmi;
          memset(&mmi,0,sizeof(mmi));

          if(cached != 0 && cached != 1)
          {
            return NULL;
          }
          mmi.prot = PROT_READ | PROT_WRITE;
          mmi.flags = MAP_SHARED;
          mmi.phys_addr = (unsigned long)phyAddr;

          if(cached)
          {
              s32Ret = ioctl(mmz,IOC_MMB_USER_REMAP_CACHED, &mmi);
              if (s32Ret!=0)
              {
                 return NULL;
              }

          }
          else
          {
              s32Ret = ioctl(mmz,IOC_MMB_USER_REMAP, &mmi);
              if (s32Ret!=0)
              {
                 return NULL;
              }
          }

          return (void *)mmi.mapped;


}

static inline HI_S32 GFX_MMZ_UnMap(HI_S32 mmz, HI_VOID *phyAddr)
{

      mmb_info   mmi;
      memset(&mmi,0,sizeof(mmi));
      mmi.phys_addr = (unsigned long)phyAddr;
      return ioctl(mmz, IOC_MMB_USER_UNMAP, &mmi);

}
static inline HI_S32 GFX_MMZ_Delete(HI_S32 mmz, HI_VOID *phyAddr)
{

      mmb_info   mmi;
      memset(&mmi,0,sizeof(mmi));
      mmi.phys_addr = (unsigned long)phyAddr;
      return ioctl(mmz, IOC_MMB_FREE, &mmi);

}
static inline HI_S32 GFX_MMZ_Flush(HI_S32 mmz,HI_U32 u32PhyAddr)
{
    if (!u32PhyAddr)
    {
        return ioctl(mmz, IOC_MMB_FLUSH_DCACHE, NULL);
    }
    else
    {
        return ioctl(mmz, IOC_MMB_FLUSH_DCACHE, u32PhyAddr);
    }

}
static inline HI_S32 GFX_MMZ_GetPhyaddr(HI_S32 mmz,HI_VOID * pVir, HI_U32 *pu32Phyaddr, HI_U32 *pu32Size)
{

    int ret;
    mmb_info   mmi;
    memset(&mmi,0,sizeof(mmi));

    mmi.mapped = pVir;

    ret = ioctl(mmz, IOC_MMB_USER_GETPHYADDR, &mmi);
    if (ret)
    {
        return -1;
    }
    if (pu32Phyaddr)
    {
        *pu32Phyaddr = mmi.phys_addr;
    }
    if (pu32Size)
    {
        *pu32Size = mmi.size;
    }
    return 0;


}


/**
\brief free the mem that has alloced. CNcomment:释放分配过的内存 CNend
\attention \n

\param[in]    u32Phyaddr. CNcomment:物理地址 CNend\n

\retval ::HI_SUCCESS
\retval ::HI_FAILURE

\see \n
::HI_GFX_FreeMem
*/

static inline HI_S32 HI_GFX_FreeMem(HI_S32 s32MMZDev,HI_U32 u32Phyaddr, HI_VOID *pMemHandle)
{
    return GFX_MMZ_Delete(s32MMZDev,(HI_VOID*)u32Phyaddr);
}


/**
\brief alloc the mem that need. CNcomment:分配需要的内存 CNend\n
\attention \n

\param[in]    pName.        CNcomment:模块名   CNend\n
\param[in]    pZoneName.
\param[in]    u32LayerSize. CNcomment:内存大小 CNend\n

\retval ::HI_SUCCESS
\retval ::HI_FAILURE

\see \n
::HI_GFX_AllocMem
*/

static inline HI_VOID *HI_GFX_AllocMem(HI_S32 s32MMZDev,HI_U32 u32Size , HI_U32 u32Align, HI_CHAR* pZoneName, HI_CHAR *pName, HI_VOID **ppMemHandle, HI_BOOL *pbMmu)
{

    HI_VOID *pAddr = NULL;

#ifdef CONFIG_GFX_256BYTE_ALIGN
    HI_U32 u32MMZAlign = 256;
#else
    HI_U32 u32MMZAlign    = u32Align;
#endif

#if 0
    /**
     **不使用三级分配，提过性能
     **/
    pAddr = GFX_MMZ_New(s32MMZDev,u32Size, u32MMZAlign, pZoneName, pName);
    if(NULL != pAddr)
    {
        return pAddr;
    }

    pAddr = GFX_MMZ_New(s32MMZDev,u32Size, u32MMZAlign, "graphics", pName);
    if(NULL != pAddr)
    {
        return pAddr;
    }
#endif
    pAddr = GFX_MMZ_New(s32MMZDev,u32Size, u32MMZAlign, NULL, pName);

    return pAddr;

}


static inline HI_VOID *HI_GFX_Map(HI_S32 s32MMZDev,HI_U32 u32PhyAddr, HI_VOID *pMemHandle, HI_BOOL bMmu)
{
    return GFX_MMZ_Map(s32MMZDev,(HI_VOID*)u32PhyAddr,HI_FALSE);
}

static inline HI_VOID *HI_GFX_MapCached(HI_S32 s32MMZDev,HI_U32 u32PhyAddr, HI_VOID *pMemHandle, HI_BOOL bMmu)
{
    return GFX_MMZ_Map(s32MMZDev,(HI_VOID*)u32PhyAddr,HI_TRUE);
}

static inline HI_S32 HI_GFX_Unmap(HI_S32 s32MMZDev,HI_U32 u32PhyAddr, HI_VOID *pMemHandle, HI_BOOL bMmu)
{
    return GFX_MMZ_UnMap(s32MMZDev,(HI_VOID*)u32PhyAddr);
}

static inline HI_S32 HI_GFX_Flush(HI_S32 s32MMZDev,HI_U32 u32PhyAddr, HI_VOID *pMemHandle, HI_BOOL bMmu)
{
    return GFX_MMZ_Flush(s32MMZDev,u32PhyAddr);
}

static inline HI_S32 HI_GFX_GetPhyaddr(HI_S32 s32MMZDev,HI_VOID * pVir, HI_U32 *pu32Phyaddr, HI_U32 *pu32Size, HI_VOID *pMemHandle, HI_BOOL *pbMmu)
{
    return GFX_MMZ_GetPhyaddr(s32MMZDev,pVir, pu32Phyaddr, pu32Size);
}
#endif


/** @} */  /*! <!-- API declaration end */


#endif /*_HI_GFX_COMM_H_ */
