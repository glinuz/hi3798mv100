/******************************************************************************
Copyright (C), 2009-2019, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mmz_struct.h
Version       : Initial Draft
Author        : Hisilicon
Created       : 2009/12/16
Last Modified :
Description   : Defines the common data type of the system.
                CNcomment: 系统共用的数据类型定义
Function List :
History       :
******************************************************************************/
/**
 * \file
 * \brief Describes the information about basic data types. CNcomment: 提供基本数据类型的相关信息
 */
#ifndef __MMZ_STRUCT_H__
#define __MMZ_STRUCT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_1_2 */
/** @{ */  /** <!-- 【Common data structure. CNcomment: 通用数据结构】 */


/**Maximum bytes of a buffer name*/
/**CNcomment:  buffer命名的最大字节数 */
#define MAX_BUFFER_NAME_SIZE 16

/**alloc_type*/
/**CNcomment:  alloc_type */
#define SECURE_MEM 0
#define NON_SECURE_MEM 1

/**Structure of an MMZ buffer*/
/**CNcomment:  MMZ buffer信息结构 */
typedef struct hiMMZ_BUF_S
{
    char bufname[MAX_BUFFER_NAME_SIZE];  /**<Strings of an MMZ buffer name*/ /**<CNcomment:  MMZ buffer名字字符串 */
    unsigned int  phyaddr;                /**<Physical address of an MMZ buffer*/ /**<CNcomment:  MMZ buffer物理地址 */
    unsigned int alloc_type;        /**<Indicate to alloc mem from the non-secure or secure memory*/ /**<CNcomment:  指示从安全内存还是非安全内存中分配 */
    char  *kernel_viraddr;         /**<Kernel-state virtual address of an MMZ buffer*/ /**<CNcomment:  MMZ buffer内核态虚拟地址 */
    char  *user_viraddr;           /**<User-state virtual address of an MMZ buffer*/ /**<CNcomment:  MMZ buffer用户态虚拟地址 */
    unsigned int  bufsize;                /**<Size of an MMZ buffer*/ /**<CNcomment:  MMZ buffer大小 */
    unsigned int  overflow_threshold;     /**<Overflow threshold of an MMZ buffer, in percentage. For example, the value 100 indicates 100%.*/ /**<CNcomment:  MMZ buffer上溢水线，按百分比设置，例如: 100 indicates 100%.*/
    unsigned int  underflow_threshold;    /**<Underflow threshold of an MMZ buffer, in percentage. For example, the value 0 indicates 0%.*/ /**<CNcomment:  MMZ buffer下溢水线，按百分比设置，例如: 0 indicates 0%.*/
}HI_MMZ_BUF_S;

/*End of CMPI */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __MMZ_STRUCT_H__ */

