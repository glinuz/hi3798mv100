/**
* @file tee_mem_mgnt_api.h
*
* Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED.
*
* 描述：内存管理相关API
*/

/** @defgroup grp_mem_api 内存管理
*@ingroup TEE_API
*/

#ifndef __TEE_MEM_MGMT_API_H
#define __TEE_MEM_MGMT_API_H

#include "tee_internal_api.h"

enum MALLOC_HINT {
	ZERO = 0,
	NOT_ZERO = 1,
};

/**
* @ingroup  grp_mem_api
* @brief 将buffer指向的缓冲区的前size个字节填充为x,buffer可以指向任意类型的缓冲区。
*
* @par 描述:
* 无
*
* @attention 无
* @param buffer [OUT] 指向缓冲区的指针
* @param x [IN] 填充的值
* @param size [IN] 填充的字节数
*
* @retval 无
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
**/
void TEE_MemFill(void *buffer, uint32_t x, uint32_t size);

/**
* @ingroup  grp_mem_api
* @brief 从src指向的缓冲区拷贝size字节到dest指向的缓冲区
*
* @par 描述:
* 无
*
* @attention src和dest可以指向任意类型的缓冲区
* @param dest [OUT] 指向目标缓冲区的指针
* @param src [IN] 指向源缓冲区的指针
* @param size [IN] 拷贝的字节数
*
* @retval 无
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
**/
void TEE_MemMove(void *dest, void *src, uint32_t size);
/**
* @ingroup  grp_mem_api
* @brief 动态申请字节数为size的内存
*
* @par 描述:
* 返回的指针的对齐方式保证可以指向任何C语言中定义的基本类型，\n
* 参数hint是一个传递给分配器的标志。当前版本只实现了一种hint值，\n
* 后续版本如果增加hint的种类，则会增加相关的描述。
*
* @attention 无
* @param size [IN] 申请的内存大小
* @param hint [IN] 分配标志，0表示申请的内存在返回前已经被清零，其它值都被忽略
*
* @retval 不等于NULL的值 指向申请到的内存的指针
* @retval NULL 表示申请失败
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
* @see TEE_Realloc | TEE_Free
**/
void *TEE_Malloc(size_t size, uint32_t hint);

/**
* @ingroup  grp_mem_api
* @brief 释放动态申请的内存
*
* @par 描述:
* 如果buffer等于NULL，那么TEE_Free不做任何动作。\n
* 用户需要注意传递的buffer指针是通过TEE_Malloc或者TEE_Realloc申请的，\n
* 并且没有被TEE_Free释放过，否则后果将不可预料。
*
* @attention 无
* @param buffer [IN] 指向需要释放的内存的指针
*
* @retval 无
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
* @see TEE_Malloc | TEE_Realloc
**/
void TEE_Free(void *buffer);

/**
* @ingroup grp_mem_api
* @brief 修改动态内存的大小
*
* @par 描述:
* 修改大小后的内存如果大于原始大小，原内存中的内容会被保留，超出部分的内容是任意的。\n
* 在更改内存大小时，可能需要重新申请内存，如果这时分配失败，那么原始内存就会被保留，\n
* 同时函数返回NULL。\n
* 如果buffer等于NULL，那么函数功能就与TEE_Malloc相同。
*
* @attention
* 用户需要注意传递的buffer指针是通过TEE_Malloc或者TEE_Realloc申请的，
* 并且没有被TEE_Free释放过，否则后果将不可预料。
* @param buffer [IN] 指向需要修改大小的内存的指针
* @param new_size [IN] 修改后的大小
*
* @retval 不等于NULL的值 指向新的动态内存的指针
* @retval NULL 表示申请失败
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
* @see TEE_Malloc | TEE_Free
**/
void *TEE_Realloc(void *buffer, uint32_t new_size);

/**
* @ingroup grp_mem_api
* @brief 内存内容比较
*
* @par 描述:
* 字符串大小是按照顺序比较每个字符的ASCII大小，直到出现大小不一样的字符或者结束符。
*
* @attention 无
* @param buffer1 [IN] 指向比较的第一个缓冲区的指针
* @param buffer2 [IN] 指向比较的第二个缓冲区的指针
* @param size [IN] 比较的字节数
*
* @retval -1 buffer1指向的缓冲区小于buffer2指向的缓冲区
* @retval 0 buffer1指向的缓冲区等于buffer2指向的缓冲区
* @retval 1 buffer1指向的缓冲区大于buffer2指向的缓冲区
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
**/
int32_t TEE_MemCompare(void *buffer1, void *buffer2, uint32_t size);

/**
* @brief 检查buffer指向的缓冲区的访问权限
*
* @par 描述:
* 当前版本没有实现。
*
* @attention 无
* @param accessFlags [IN] 检查的访问类型
* @param buffer [IN] 指向需要检查的内存的指针
* @param size [IN] 需要检查的内存的大小
*
* @retval #TEE_SUCCESS 该内存拥有accessFlags指定的访问权限
* @retval #TEE_ERROR_ACCESS_DENIED 该内存没有有accessFlags指定的访问权限
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
**/
TEE_Result TEE_CheckMemoryAccessRights(uint32_t accessFlags, void *buffer, size_t size);

/**
* @ingroup grp_mem_api
* @brief
* 设置一个全局变量用于同一Instance内的多个Session间共享。\n
* Instance和Session的具体描述见用户开发手册相应章节。
*
* @par 描述:
* 无
*
* @attention 无
* @param instanceData [IN] 设置的全局变量的地址
*
* @retval 无
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
**/
void TEE_SetInstanceData(void *instanceData);

/**
* @ingroup grp_mem_api
* @brief 获取TEE_SetInstanceData设置的全局变量的指针
*
* @par 描述:
* 无
*
* @attention 无
* @param 无
*
* @retval 不等于NULL的值 指向TEE_SetInstanceData设置的全局变量的指针
* @retval NULL 没有InstanceData被设置
*
* @par 依赖:
* @li tee_internal_api.h：内部数据类型定义
**/
void *TEE_GetInstanceData(void);

bool TEE_IsSecureMemory(void *buffer, uint32_t size);

uint32_t get_mem_usage(bool show);

#endif

/**
* History: \n
* 2013-05-03，y00204347，Create \n
*
* vi: set expandtab ts=4 sw=4 tw=80:
*/
