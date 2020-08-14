/**
* @file sre_mem.h
*
* Copyright(C), 2008-2008, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
*
* 描述：内存基本功能对外头文件。 \n
*/


/** @defgroup SRE_mem 内存基本功能
 *@ingroup SRE_memory
*/

#ifndef _SRE_MEM_H
#define _SRE_MEM_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "sre_base.h"


/**
 * @ingroup SRE_mem
 * 内存块的类型总数。
 */
#define OS_SLICE_MAXTYPE            8
/**
 * @ingroup SRE_mem
 * 系统缺省的内存分区数量。
 * TA2TA share memory
 */
#define OS_MEM_DEFAULT_PTNUM        2
/**
 * @ingroup SRE_mem
 * 系统预留TA内存分区数量。
 */
#define OS_MEM_TASK_PTNUM        3
/**
 * @ingroup SRE_mem
 * 缺省私有FSC内存分区。
 */
#define OS_MEM_DEFAULT_FSC_PT    0
/**
 * @ingroup SRE_mem
 * OS支持核间通信的前提下，缺省共享静态FSC内存分区。
 */
#define OS_MEM_DEFAULT_MCSTA_FSC_PT     1

/**
 * @ingroup SRE_mem
 * task memory partition
 */
#define OS_MEM_DEFAULT_TASKMEM_PT    4

/**
 * @ingroup SRE_mem
 * 申请的POOL内存地址对齐标准:16字节对齐。
 */
#define OS_MEM_ADDR_ALIGN           16

/**
 * @ingroup SRE_mem
 * 申请的FSC内存地址对齐标准:4字节对齐。
 */
#define OS_MEM_FSC_ADDR_ALIGN           4

/**
 * @ingroup SRE_mem
 * 内存算法类型,目前仅#MEM_ARITH_MCPOOL支持Cache内存空间
 */
typedef enum
{
	MEM_ARITH_FSC,        /**< 私有FSC算法*/
	MEM_ARITH_POOL,       /**< 私有POOL动态算法*/
	MEM_ARITH_MCDYNFSC,   /**< 共享动态FSC算法*/
	MEM_ARITH_MCPOOL,     /**< 共享POOL动态算法*/
	MEM_ARITH_MCBLOCK,     /*共享Block内存算法，此算法对外体现但不支持*/
	MEM_ARITH_BUTT
}
OS_MEM_ARITH_E;

/**
 * @ingroup SRE_mem
 * 内存对齐方式
 */
typedef enum {
	MEM_ADDR_ALIGN_004  = 4  , /**< 4字节对齐*/
	MEM_ADDR_ALIGN_008  = 8  , /**< 8字节对齐*/
	MEM_ADDR_ALIGN_016  = 16  ,/**< 16字节对齐*/
	MEM_ADDR_ALIGN_032  = 32  ,/**< 32字节对齐*/
	MEM_ADDR_ALIGN_064  = 64  ,/**< 64字节对齐*/
	MEM_ADDR_ALIGN_128  = 128 ,/**< 128字节对齐*/
	MEM_ADDR_ALIGN_256  = 256 ,/**< 256字节对齐*/
	MEM_ADDR_ALIGN_1024 = 1024,/**< 1024字节对齐*/
	MEM_ADDR_ALIGN_4096 = 4096,/**< 4096字节对齐*/
	MEM_ADDR_BUTT              /**< 字节对齐非法*/
} OS_MEM_ALIGN_E;

/**
 * @ingroup SRE_mem
 * 动态创建分区接口参数结构体。
 */
typedef struct tagCreatePtPara {
	OS_MEM_ARITH_E  enArith;                           /**< 分区的算法类型                  */
	VOID           *pPtAddr;                           /**< 分区起始地址                    */
	UINT32          uwPtSize;                          /**< 分区大小 (单位：字节）          */
	BOOL            bCachable;                         /**< 指定该块物理内存是否为可cache   */
	UINT16          ausSliceSize[OS_SLICE_MAXTYPE];    /**< 最大八种内存块大小(单位：字节）  */
} MEM_CREATEPT_PARA;

/**
 * @ingroup SRE_mem
 * 获取指定分区内存信息接口参数结构体。
 */
typedef struct tagMemStatInfo {
	UINT32 uwFreeSize;                              /**< 空闲内存的大小（单位：字节）  */
	UINT32 uwPeakUsed;                              /**< 已使用内存的大小（单位：字节）*/
	UINT32 uwPtSize;                                /**< 内存分区总大小                */
} OS_MEM_PT_STATE;

/**
 * @ingroup SRE_mem
 * 内存错误码：创建分区的分区大小太小
 *
 * 值: 0x02000101
 *
 * 解决方案：分区大小改大
 */
#define OS_ERRNO_MEM_PTCREATE_SIZE_TOOSMALL               SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x01)

/**
 * @ingroup SRE_mem
 * 内存错误码：分区号错误，或者申请的内存大小为0，或者内存申请的分区没有创建
 *
 * 值: 0x02000102
 *
 * 解决方案：请检查入参分区号，申请内存大小的有效性
 */
#define OS_ERRNO_MEM_ALLOC_PARA_ERR                         SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x02)

/**
 * @ingroup SRE_mem
 * 内存错误码：释放的地址为空
 *
 * 值: 0x02000103
 *
 * 解决方案：请检查释放的内存块地址是否正确。
 */
#define OS_ERRNO_MEM_FREE_ADDR_INVALID                    SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x03)
/**
 * @ingroup SRE_mem
 * 内存错误码：释放内存所在的分区无效(分区未创建或分区号大于分区数)
 *
 * 值: 0x02000104
 *
 * 解决方案：请检查分区号的有效性
 */
#define OS_ERRNO_MEM_FREE_PTNO_INVALID                    SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x04)

/**
 * @ingroup SRE_mem
 * 内存错误码：配置的最大可创建内存分区数过大(最大不能超过20)
 *
 * 值: 0x02000105
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_PTNUMBER_OVERMANY                      SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x05)

/**
 * @ingroup SRE_mem
 * 内存错误码：内存申请时申请的大小太大(可能为负值)
 *
 * 值: 0x02000106
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_ALLOC_SIZETOOLARGE                     SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x06)
/**
 * @ingroup SRE_mem
 * 内存错误码：内存配置的内存块类型都是零
 *
 * 值: 0x02000107
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_ALLSLICE_ZERO                       SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x07)
/**
 * @ingroup SRE_mem
 * 内存错误码：内存配置的内存块类型不是按递增的顺序配置的
 *
 * 值: 0x02000108
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_MEMSLICE_NOT_INORDER                SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x08)
/**
 * @ingroup SRE_mem
 * 内存错误码：创建分区入参有误(分区号应大于1且小于配置最大分区个数+2，分区信息结构体和分区起始地址不为空
 * 检查分区算法正确性、分区大小不为0)
 *
 * 值: 0x02000109
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_PTCREATE_PARAMETER_INVALID           SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x09)
/**
 * @ingroup SRE_mem
 * 内存错误码：分区已经创建
 *
 * 值: 0x0200010a
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_PT_ALREADY_CREATED                     SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x0a)
/**
 * @ingroup SRE_mem
 * 内存错误码：动态创建内存分区时，区段有重叠。
 *
 * 值: 0x0200010b
 *
 * 解决方案: 不同分区之间避免重叠
 */
#define OS_ERRNO_MEM_PT_OVERLAP                             SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x0b)
/**
 * @ingroup SRE_mem
 * 内存错误码：内存算法没有创建。
 *
 * 值: 0x0200010c
 *
 * 解决方案: 检查相应开关是否打开
 */
#define OS_ERRNO_MEM_ALGORITHM_UNINSTALLED                  SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x0c)

/**
 * @ingroup SRE_mem
 * 内存错误码：该内存分区未被创建或者分区号非法。
 *
 * 值: 0x0200010d
 *
 * 解决方案: 检查分区号的有效性
 */
#define OS_ERRNO_MEM_NO_USE                                 SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x0d)

/**
 * @ingroup SRE_mem
 * 内存错误码：调用获取分区的使用情况接口时，该接口参数存在空指针。
 *
 * 值: 0x0200010e
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_INPUT_INVAILD                          SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x0e)
/**
 * @ingroup SRE_mem
 * 内存错误码：FSC内存算法不支持可Cache空间
 *
 * 值: 0x0200010f
 *
 * 解决方案: 改用不可Cache空间，或改用POOL算法。
 */
#define OS_ERRNO_MEM_PTCREATE_FSC_ARITH_UNCACHABLE          SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x0f)

/**
 * @ingroup SRE_mem
 * 内存错误码：分区号不合法，分区号范围[0,OS_MEM_MAX_PT_NUM+OS_MEM_DEFAULT_PTNUM-1],OS_MEM_MAX_PT_NUM是用户配置的分区数，OS_MEM_DEFAULT_PTNUM系统缺省的分区数。
 *
 * 值: 0x02000110
 *
 * 解决方案：请检查入参分区号
 */
#define OS_ERRNO_MEM_PTNO_INVALID                           SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x10)

/**
 * @ingroup SRE_mem
 * 内存错误码：申请的内存块大小为零
 *
 * 值: 0x02000111
 *
 * 解决方案：请检查申请内存大小的有效性
 */
#define OS_ERRNO_MEM_ALLOC_SIZE_ZERO                        SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x11)

/**
 * @ingroup SRE_mem
 * 内存错误码：内存申请的分区没有创建
 *
 * 值: 0x02000112
 *
 * 解决方案：请检查该分区是否已经创建
 */
#define OS_ERRNO_MEM_PT_NOT_CREATED                         SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x12)

/**
 * @ingroup SRE_mem
 * 内存错误码：对齐方式不合法
 *
 * 值: 0x02000113
 *
 * 解决方案：请检查入参对齐方式
 */
#define OS_ERRNO_MEM_ALLOC_ALIGNPOW_INVALID                 SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x13)

/**
 * @ingroup SRE_mem
 * 内存错误码：分区算法非法。OS支持的分区算法:MEM_ARITH_FSC、MEM_ARITH_POOL、MEM_ARITH_MCDYNFSC、MEM_ARITH_MCPOOL。
 *
 * 值: 0x02000114
 *
 * 解决方案：请检查分区算法参数
 */
#define OS_ERRNO_MEM_PT_ARITH_INVALID                       SRE_ERRNO_OS_ERROR(OS_MID_MEM, 0x14)

/**
 * @ingroup SRE_mem
 * OS缺省私有FSC内存错误码：从私有FSC分区中申请内存时，找不到空闲内存，申请失败。
 *
 * 值: 0x02000210
 *
 * 解决方案: 增大私有FSC内存分区大小
 */
#define OS_ERRNO_FSCMEM_ALLOC_NO_MEMORY                     SRE_ERRNO_OS_ERROR(OS_MID_FSCMEM, 0x10)

/**
 * @ingroup SRE_mem
 * 动态内存错误码：动态内存释放时要释放的内存块的头被破坏，或已释放
 *
 * 值: 0x02000311
 *
 * 解决方案: 保证对内存写操作时，不要出现写越界。
 *
 */
#define OS_ERRNO_MEM_FREE_SH_DAMAGED                        SRE_ERRNO_OS_ERROR(OS_MID_DYNMEM, 0x11)

/**
 * @ingroup SRE_mem
 * 动态内存错误码：动态(POOL)内存申请时申请的内存块被破坏
 *
 * 值: 0x02000312
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_DYNALLOC_SH_DAMAGED                    SRE_ERRNO_OS_ERROR(OS_MID_DYNMEM, 0x12)

/**
 * @ingroup SRE_mem
 * 内存错误码：私有POOL内存释放时内存块已经被释放(或者内存没有被申请过)
 *
 * 值: 0x02000314
 *
 * 解决方案: 检查释放的内存是否已申请，或者是否已经被释放。
 */
#define OS_ERRNO_MEM_FREE_POOL_MEM_ALREADY_FREE          SRE_ERRNO_OS_ERROR(OS_MID_DYNMEM, 0x14)

/**
 * @ingroup SRE_mem
 * 动态内存错误码：动态POOL内存申请时内存空间不够
 *
 * 值: 0x02000315
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_DYN_ALLOC_MEM_SHORTAGE                 SRE_ERRNO_OS_ERROR(OS_MID_DYNMEM, 0x15)

/**
 * @ingroup SRE_mem
 * 动态内存错误码：内存算法类型非法或OS不支持该内存算法
 *
 * 值: 0x02000316
 *
 * 解决方案: 查看输入的算法是否正确
 */
#define OS_ERRNO_MEM_ARITH_INVALID                          SRE_ERRNO_OS_ERROR(OS_MID_DYNMEM, 0x16)

/**
 * @ingroup SRE_mem
 * 共享内存错误码：共享动态POOL内存申请时内存空间不够
 *
 * 值: 0x02000417
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_SHARE_ALLOC_MEM_SHORTAGE               SRE_ERRNO_OS_ERROR(OS_MID_MCMEM, 0x17)

/**
 * @ingroup SRE_mem
 * 共享内存错误码：共享动态POOL内存申请时申请的内存块被破坏
 *
 * 值: 0x03000418
 *
 * 解决方案: 保证地址是内存地址，保证用户写操作不能越界。
 */
#define OS_ERRNO_MEM_SHAREALLOC_SH_DAMAGED                  SRE_ERRNO_OS_FATAL(OS_MID_MCMEM, 0x18)

/**
 * @ingroup SRE_mem
 * 共享内存错误码：共享动态POOL内存释放时内存块已经被释放(或者内存没有被申请过)
 *
 * 值: 0x02000419
 *
 * 解决方案: 检查该内存块是否已经申请，或者是否已经被释放。
 */
#define OS_ERRNO_MEM_FREE_SHARE_MEM_ALREADY_FREE         SRE_ERRNO_OS_ERROR(OS_MID_MCMEM, 0x19)
/**
 * @ingroup SRE_mem
 *
 * 内存错误码：不同核创建相同编号的共享分区时参数不相同，或者分区起始地址相同，但分区号不相同。
 *
 * 值: 0x0200041a
 *
 * 解决方案: 检查分区号，分区算法，分区起始地址，分区大小，分区块类型及大小的一致性
 */
#define OS_ERRNO_MEM_SHARE_PARA_INCONSISTENT                SRE_ERRNO_OS_ERROR(OS_MID_MCMEM, 0x1a)

/**
 * @ingroup SRE_mem
 * 共享内存错误码：共享静态FSC内存申请的尺寸与同BLOCK已申请的内存尺寸不匹配
 *
 * 值: 0x0200041c
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_MCSTA_FSCMEM_UNMATCHED                  SRE_ERRNO_OS_ERROR(OS_MID_MCMEM, 0x1c)

/**
 * @ingroup SRE_mem
 * 共享内存错误码：从共享静态FSC分区中申请内存时，找不到空闲内存，申请失败。
 *
 * 值: 0x0200041d
 *
 * 解决方案: 增大共享静态FSC内存分区大小
 */
#define OS_ERRNO_MCSTA_FSCMEM_ALLOC_NO_MEMORY                SRE_ERRNO_OS_ERROR(OS_MID_MCMEM, 0x1d)

/**
 * @ingroup SRE_mem
 * 共享内存错误码：共享静态FSC的内存不能释放
 *
 * 值: 0x0200041f
 *
 * 解决方案: 共享静态FSC算法不支持内存释放操作，请检查是否释放了错误的内存块。
 */
#define OS_ERRNO_MCSTA_FSCMEM_FREE_ISFORBIDEN                SRE_ERRNO_OS_ERROR(OS_MID_MCMEM, 0x1e)

/**
 * @ingroup SRE_mem
 * 共享内存错误码：从共享动态FSC分区中申请内存时，找不到空闲内存，申请失败。
 *
 * 值: 0x0200041f
 *
 * 解决方案: 增大共享动态FSC内存分区大小
 */
#define OS_ERRNO_MCDYN_FSCMEM_ALLOC_NO_MEMORY                SRE_ERRNO_OS_ERROR(OS_MID_MCMEM, 0x1f)

/**
 * @ingroup SRE_mem
 * 共享内存错误码：不同image中对于OS缺省使用的1号分区配置不一致。
 *
 * 值: 0x02000420
 *
 * 解决方案: 请确保所有image中对于OS缺省使用的1号分区配置的起始地址一致、分区大小一致。
 */
#define OS_ERRNO_MCSTA_FSCMEM_PT_PARA_INCONSISTENT           SRE_ERRNO_OS_ERROR(OS_MID_MCMEM, 0x20)

/**
 *@ingroup SRE_mem
 *@brief 向RTOSck已创建的指定分区申请内存。
 *
 *@par 描述:
 *<li>在分区号为ucPtNo的分区中，申请大小为uwSize的内存。</li>
 *@attention
 *<ul>
 *<li>申请内存时的分区号，根据实际创建的分区号来使用。</li>
 *<li>调用函数后，注意判断返回的地址是否为空以避免后续访问空指针。</li>
 *<li>对于FSC内存算法，申请到的内存首地址是按4字节对齐的，对于POOL内存算法，申请到的内存首地址是按16字节对齐的</li>
 *<li>对于FSC分区，即使用户创建的分区足够大，一次最大可申请的内存块大小也不能超过0xFFE0字节。</li>
 *<li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中，用户可以通过黑匣子模块Dump出内存申请出错的信息。</li>
 *</ul>
 *@param uwMid        [IN] 类型#UINT32，申请的模块号。
 *@param ucPtNo       [IN] 类型#UINT8， 分区号。
 *@param uwSize       [IN] 类型#UINT32，申请的大小。
 *
 *@retval NULL  0，申请失败。
 *@retval 内存首地址值。
 *@par 依赖:
 *sre_mem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C00
 *@see SRE_MemFree
*/
extern VOID *SRE_MemAlloc(UINT32 uwMid, UINT8 ucPtNo, UINT32 uwSize);

/**
 *@ingroup SRE_mem
 *@brief 向RTOSck已创建的指定分区申请指定大小的内存块。
 *
 *@par 描述:
 *<ul>
 *<li>在分区号为ucPtNo的分区中，申请大小为uwSize的内存，对齐方式为enAlignPow。</li>
 *</ul>
 *@attention
 *<ul>
 *<li>申请内存分区时的分区号，根据实际创建的分区号来使用。</li>
 *<li>调用函数后，注意判断返回的地址是否为空以避免后续访问空指针。</li>
  *<li>enAlignPow是对齐方式，对于POOL算法，最低只支持16字节对齐方式。</li>
 *<li>对于FSC分区，即使用户创建的分区足够大，一次最大可申请的内存块大小也不能超过0xFFE0字节。</li>
 *<li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中，用户可以通过黑匣子模块Dump出内存申请出错的信息。</li>

 *</ul>
 *@param uwMid        [IN] 类型#UINT32，申请的模块号。
 *@param ucPtNo       [IN] 类型#UINT8， 分区号。
 *@param uwSize       [IN] 类型#UINT32，申请的大小。
 *@param enAlignPow   [IN] 类型#UINT8，动态对齐，有效取值范围为#OS_MEM_ALIGN_E。
 *
 *@retval NULL  0，申请失败。
 *@retval 内存首地址值。
 *@par 依赖:
 *sre_mem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C00
 *@see SRE_MemFree
*/
extern VOID *SRE_MemAllocAlign(UINT32 uwMid, UINT8 ucPtNo, UINT32 uwSize, OS_MEM_ALIGN_E enAlignPow);

/**
 *@ingroup SRE_mem
 *@brief 释放申请的内存。
 *
 *@par 描述:
 *该接口根据内存块的地址pAddr，找到该内存块所属的内存分区，再根据分区号和用户使用的地址pAddr释放该内存。
 *@attention
 *<ul>
 *<li>如果返回值不是#SRE_OK，则内存不会被释放。</li>
 *<li>被破坏的内存不能被释放。</li>
 *</ul>
 *@param uwMid       [IN] 类型#UINT32，要释放的模块号。
 *@param pAddr       [IN] 类型#VOID *，释放的地址。
 *
 *@retval #OS_ERRNO_MEM_FREE_PTNO_INVALID      0x02000104，释放地址不在内存分区中。
 *@retval #OS_ERRNO_MEM_FREE_SH_DAMAGED          0x03000311，内存释放时要释放的内存块的头被破坏，或已释放。
 *@retval #OS_ERRNO_MEM_FREE_POOL_MEM_ALREADY_FREE        0x02000314，私有POOL内存释放时内存块已经被释放(或者内存没有被申请过)。
 *@retval #OS_ERRNO_MEM_FREE_SHARE_MEM_ALREADY_FREE       0x02000419，共享动态POOL内存释放时内存块已经被释放(或者内存没有被申请过)。
 *@retval #OS_ERRNO_MCSTA_FSCMEM_FREE_ISFORBIDEN       0x0200041e，共享静态FSC的内存不能释放。
 *@retval #SRE_OK                            0x00000000，内存释放成功。
 *@retval #OS_ERRNO_MEM_FREE_ADDR_INVALID      0x02000103，释放地址为空。
 *@par 依赖:
 *sre_mem.h：该接口声明所在的头文件。
 *@since RTOSck V100R001C00
 *@see SRE_MemAlloc
*/
extern UINT32 SRE_MemFree(UINT32 uwMid, VOID *pAddr);

/**
 *@ingroup SRE_mem
 *@brief 动态创建分区。
 *
 *@par 描述:
 *创建一个分区属性为pstPara，分区号为ucPtNo的内存分区。
 *@attention
 *<ul>
 *<li>创建内存分区时的分区号，取值范围是[2,#OS_MEM_MAX_PT_NUM+2)。</li>
 *<li>用户可以在配置文件(sre_config.h)中配置系统支持的最大分区数#OS_MEM_MAX_PT_NUM，上限为20；如用户定义#OS_MEM_MAX_PT_NUM为4，则可以使用的分区号应为:2，3，4，5。</li>
 *<li>除了私有FSC算法，其它算法都可以通过内存裁剪开关决定是否创建。 </li>
 *<li>os在支持核间通信的前提下，会占用2个分区(不计入系统支持的最大分区数内)，分区号从0到1，0号为系统使用的私有FSC、
      1号为系统使用的共享静态FSC</li>
 *<li>在tensilica平台下，创建内存时，用户配置的内存地址决定该内存是否可cache。</li>
 *<li>共享Block内存算法不支持</li>
 *</ul>
 *@param pstPara  [IN] 类型#MEM_CREATEPT_PARA *，待创建分区的信息。
 *@param ucPtNo   [IN] 类型#UINT8， 分区号。
 *
 *@retval #OS_ERRNO_MEM_PTCREATE_PARAMETER_INVALID 0x02000109，创建分区入参有误。
 *@retval #OS_ERRNO_MEM_PT_ALREADY_CREATED           0x0200010a，分区已经创建。
 *@retval #OS_ERRNO_MEM_PT_OVERLAP                   0x0200010b，动态创建内存分区时，区段有重叠。
 *@retval #OS_ERRNO_MEM_SHARE_PARA_INCONSISTENT      0x0200041a，共享分区参数配置不统一。不同核创建相同编号的共享分区时参数应相同。
 *@retval #OS_ERRNO_MEM_ALGORITHM_UNINSTALLED        0x0200010c，内存算法没有创建，可能是相应开关没有打开。
 *@retval #OS_ERRNO_MEM_PTCREATE_SIZE_TOOSMALL     0x02000101，创建分区的分区大小太小。
 *@retval #OS_ERRNO_FSCMEM_ALLOC_NO_MEMORY           0x02000210，创建分区时，从缺省私有FSC内存分区中申请分区控制块失败。
 *@retval #OS_ERRNO_MCSTA_FSCMEM_ALLOC_NO_MEMORY     0x0200041d，创建分区时，从缺省共享静态FSC内存分区中申请分区控制块失败。
 *@retval #SRE_OK 分区创建成功。
 *@par 依赖:
 *sre_mem.h：该接口声明所在的头文件。
 *@since UniDSP V100R001C01
 *@see SRE_MemFree | SRE_MemAlloc
*/
extern  UINT32 SRE_MemPtCreate(MEM_CREATEPT_PARA *pstPara, UINT8 ucPtNo);

extern UINT32 SRE_MemPtDelete(UINT8 ucPtNo);

/**
 *@ingroup SRE_mem
 *@brief 获取指定内存分区的内存占用率。
 *
 *@par 描述:
 *获取分区号为ucPtNo的内存分区的占用率。
 *@attention
 *<ul>
 *<li>对于pool算法，已使用的内存空间包括申请之后释放的内存空间，由于内存申请之后，即使释放，其内存块类型已经被指定，不能再自由分配。</li>
 *<li>对于fsc算法，已使用的内存空间不包括申请之后释放的内存空间，释放的空间可以重新被分配。</li>
 *</ul>
 *@param ucPtNo         [IN] 类型#UINT8， 分区号。
 *
 *@retval 内存分区占用率         操作成功。
 *@retval 0                     操作失败或分区号不存在或用户查询的分区占用率为零。
 *@par 依赖:
 *sre_mem.h：该接口声明所在的头文件。
 *@since UniDSP V200R003C03
 *@see SRE_MemPtCreate
*/
extern UINT32 SRE_MemUsageGet(UINT8 ucPtNo);

/**
 *@ingroup SRE_mem
 *@brief 查询内存号为ucPtNo的内存分区的基本信息，将信息返回给pstMemPtState所指向的结构体。
 *
 *@par 描述:
 *输入分区号，查询该分区的总大小，历史使用峰值和剩余内存的大小
 *@attention
 *<ul>
 *<li>支持pool算法和fsc算法</li>
 *<li>内存分区总大小是指用户申请的分区大小与该分区控制块的大小的总和</li>
 *<li>历史使用峰值是指分区历史使用最大的内存空间的大小。
 对于pool算法来说，由于内存申请之后，即使释放，其内存块类型已经被指定，不能再自由分配。所以对于pool算法，即使内存申请之后释放，这块空间也算作被使用的；
 对于fsc算法，申请了又被释放的内存可以重新分配，算作未被使用。</li>
 *<li>分区剩余内存大小，对于pool算法指未经格式化的空闲内存的大小，即不包括已经申请然后被释放的空闲内存的大小；对于fsc算法，包括申请然后被释放的空闲内存的大小</li>
 *</ul>
 *@param ucPtNo         [IN] 类型#UINT8， 分区号。
 *@param pstMemPtState  [OUT] 类型#OS_MEM_PT_STATE *，保存内存分区的使用状态信息。
 *
 *@retval #SRE_OK                   0x00000000，操作成功。
 *@retval #OS_ERRNO_MEM_INPUT_INVAILD   0x0200010e，结构体为空。
 *@retval #OS_ERRNO_MEM_NO_USE          0x0200010d，分区未创建或分区号非法。
 *@retval #OS_ERRNO_MEM_ARITH_INVALID   0x02000316，内存算法类型非法或OS不支持该内存算法。
 *@par 依赖:
 *sre_mem.h：该接口声明所在的头文件。
 *@since UniDSP V200R003C03
 *@see SRE_MemPtCreate
*/
extern UINT32 SRE_MemPtStateGet(UINT8 ucPtNo, OS_MEM_PT_STATE *pstMemPtState);
/**
 *查询pPtAddr的地址是否在内存号为ucPtNo的内存分区。
 */
extern OS_SEC_TEXT UINT32 SRE_PtMemCheck(UINT8 ucPtNo, VOID *pPtAddr);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _SRE_MEM_H */
/**
 * History:
 * 2009-1-20 a149045: Create this file.
 * 2009-9-3  z00134877: change this file.
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

