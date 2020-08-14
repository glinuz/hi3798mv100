/******************************************************************************

Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mpi_mmz.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2006-07-18
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __MPI_MMZ_H__
#define __MPI_MMZ_H__

#include "hi_type.h"
#include "hi_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define HI_INVALID_MEM_ADDR     (0)
#define HI_MEM_ALGIN_SIZE       (0x1000)

/******************************* MPI declaration *****************************/
/*malloc memory for mmz and map to user-state address, bufname and bufsize used to input,
    physic address and user-state used to output*/
/*CNcomment: 申请mmz内存，并映射用户态地址,bufname和bufsize作为输入, 物理地址和用户态虚拟地址作为输出 */
HI_S32 HI_MPI_MMZ_Malloc(HI_MMZ_BUF_S *pstBuf);

/*free user-state map, release mmz memory, make sure that physic address, user-state address and lengh is right*/
/*CNcomment: 解除用户态地址的映射，并释放mmz内存,保证传入的物理地址、用户态虚拟地址和长度正确*/
HI_S32 HI_MPI_MMZ_Free(HI_MMZ_BUF_S *pstBuf);

/*malloc mmz memory for appointed mmz name, return physic address*/
/*CNcomment: 指定mmz的名字申请mmz内存，返回物理地址*/
HI_U32 HI_MPI_MMZ_New(HI_U32 u32Size , HI_U32 u32Align, HI_CHAR *ps8MMZName, HI_CHAR *ps8BufName);

/*CNcomment: 释放mmz内存 */
HI_S32 HI_MPI_MMZ_Delete(HI_U32 u32PhysAddr);

/*get physic address accordint to virtual address*/
/**CNcomment:  根据虚拟地址，获取对应的物理地址 */
HI_S32 HI_MPI_MMZ_GetPhyAddr(HI_VOID *pRefAddr, HI_U32 *pu32PhyAddr, HI_U32 *pu32Size);

/*map physic address of mmz memory to user-state virtual address, can appoint whether cached*/
/**CNcomment:  将mmz申请的物理地址映射成用户态虚拟地址，可以指定是否cached*/
HI_VOID *HI_MPI_MMZ_Map(HI_U32 u32PhysAddr, HI_BOOL bCached);

/*unmap user-state address of mmz memory*/
/**CNcomment:  解除mmz内存用户态地址的映射 */
HI_S32 HI_MPI_MMZ_Unmap(HI_U32 u32PhysAddr);

/*for cached memory, flush cache to memory*/
/**CNcomment:  对于cached内存，刷Dcache到内存 */
HI_S32 HI_MPI_MMZ_Flush(HI_U32 u32PhysAddr);

#ifdef HI_SMMU_SUPPORT
typedef struct
{
    HI_CHAR bufname[MAX_BUFFER_NAME_SIZE];
    HI_U32  bufsize;
 //   HI_BOOL auth_flag;      /**<the secure flag*/ /**<CNcomment:安全标记位，HI_TRUE表示需要设置安全标志，HI_FALSE表示不设置*/
//    HI_BOOL sec_map_flag;   /**<the secure flag for map to the secure CPU*/ /**<CNcomment:安全cpu映射的标志位，HI_TRUE表示需要做安全cpu映射，HI_FALSE表示不需要*/
} HI_MPI_SMMU_SEC_ATTR_S;

typedef struct
{
    HI_U32 smmu_addr;
    HI_U32 smmu_size;
//    HI_BOOL auth_flag;      /**<the secure flag*/ /**<CNcomment:安全标记位，HI_TRUE表示需要设置安全标志，HI_FALSE表示不设置*/
//    HI_BOOL sec_map_flag;   /**<the secure flag for map to the secure CPU*/ /**<CNcomment:安全cpu映射的标志位，HI_TRUE表示需要做安全cpu映射，HI_FALSE表示不需要*/
//    HI_U32 sec_virt;        /**<the map address of secure CPU*/ /**<CNcomment:和secmapflag对应，安全cpu的映射地址，secmapflag为1时有效*/
  //  HI_U32 fixed_buf_phy;   /**<the physical address of buffer used to reorganize the memory*/ /**<CNcomment: 内存整理的buffer的物理地址*/
//    HI_U32 fixed_buf_size;  /**<the size of buffer used to reorganize the memory*/ /**<CNcomment: 内存整理的buffer的size*/
} HI_MPI_SMMU_SEC_BUF_S;

HI_U32 HI_MPI_SMMU_New(HI_CHAR *pBufName, HI_U32 Size);

HI_S32 HI_MPI_SMMU_Delete(HI_U32 SmmuAddr);

/* get smmu address accordint to virtual address */
HI_S32 HI_MPI_SMMU_GetSmmuAddrByMapAddr(HI_VOID *pMapAddr, HI_U32 *pSmmuAddr, HI_U32 *pSize);

/*
 * map physic address of mmz memory to user-state virtual address, can appoint whether cached
 * bCached = 1, Cached; bCached = 0, no Cached
 */
HI_VOID *HI_MPI_SMMU_Map(HI_U32 SmmuAddr, HI_BOOL bCached);

HI_S32 HI_MPI_SMMU_Unmap(HI_U32 SmmuAddr);

/* for cached memory, flush cache to memory for smmu */
HI_S32 HI_MPI_SMMU_Flush(HI_U32 SmmuAddr);

/* map phy memory to smmu address */
HI_S32 HI_MPI_SMMU_MapPhytoSmmu(HI_U32 PhyAddr, HI_U32 *pSmmuAddr);

/* unmap smmu address */
HI_S32 HI_MPI_SMMU_UnmapPhyfromSmmu(HI_U32 SmmuAddr);

/* alloc secure memory in the nor secure side */
HI_S32 HI_MPI_SMMU_SecMemAlloc(const HI_MPI_SMMU_SEC_ATTR_S *pSecAttr, HI_MPI_SMMU_SEC_BUF_S *pSecBuf);

/* free secure memory in the nor secure side */
HI_S32 HI_MPI_SMMU_SecMemFree(HI_MPI_SMMU_SEC_BUF_S *pSecBuf);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MPI_MMZ_H__ */

