/**
 * @file tee_trusted_storage_api.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：安全世界安全存储相关API  \n
*/

/**@defgroup TEE_TRUSTED_STORAGE_API  安全存储
* @ingroup TEE_API
*/

#ifndef __TEE_TRUSTED_STORAGE_API_H
#define __TEE_TRUSTED_STORAGE_API_H

#include "tee_internal_api.h"

/**
 * @ingroup TEE_TRUSTED_STORAGE_API
 * HANDLE_NULL的定义，无效object handle
 */
#define TEE_HANDLE_NULL 0x00000000

typedef struct s_list_node {
	struct s_list_node *pstPrev;
	struct s_list_node *pstNext;
} s_list;

struct __TEE_ObjectEnumHandle {
	//TEE_UUID uuid;
	uint32_t storageID;
	char ObjectID[64];
	uint32_t objectEnumType;
	s_list listhead;
};
typedef struct  __TEE_ObjectEnumHandle *TEE_ObjectEnumHandle;

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 * 数据流定位起始位置选项，用于#TEE_SeekObjectData函数 \n
*/
typedef enum {
	TEE_DATA_SEEK_SET = 0,     /**< 定位起始位置为数据流开始处 */
	TEE_DATA_SEEK_CUR,         /**< 定位起始位置为当前数据流位置 */
	TEE_DATA_SEEK_END          /**< 定位起始位置数据流末尾处 */
} TEE_Whence;

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 *  Storage ID，定义对应app的存储空间 \n
*/
enum Object_Storage_Constants {
	TEE_OBJECT_STORAGE_PRIVATE = 0x00000001,     /**< 对应每个应用单独的私有的存储空间，目前仅支持这一种 */
};

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 * #TEE_ObjectHandle的#handleFlags，决定该#TEE_ObjectHandle对object数据流的访问权限 \n
*/
enum Data_Flag_Constants {
	TEE_DATA_FLAG_ACCESS_READ = 0x00000001,        /**< 对数据流有读权限，可以进行读操作#TEE_ReadObjectData */
	TEE_DATA_FLAG_ACCESS_WRITE = 0x00000002,       /**< 对数据流有写权限，可以进行写操作#TEE_WriteObjectData和裁剪操作#TEE_TruncateObjectData */
	TEE_DATA_FLAG_ACCESS_WRITE_META = 0x00000004,  /**< 对数据流有WRITE_META权限，可以进行删除#TEE_CloseAndDeletePersistentObject和改名操作#TEE_RenamePersistentObject */
	TEE_DATA_FLAG_SHARE_READ = 0x00000010,         /**< 对数据流有共享读权限，可以打开多个#TEE_ObjectHandle并发读取 */
	TEE_DATA_FLAG_SHARE_WRITE = 0x00000020,        /**< 对数据流有共享写权限，可以打开多个#TEE_ObjectHandle并发写入 */
	TEE_DATA_FLAG_CREATE = 0x00000200,             /**< 未使用 */
	TEE_DATA_FLAG_EXCLUSIVE = 0x00000400,          /**< 保护已有同名数据文件。如果同名文件不存在，则新建数据文件；如果同名文件存在则报错 */
	TEE_DATA_FLAG_OVERWRITE = 0x00000400,          /**< 保护已有同名数据文件。如果同名文件不存在，则新建数据文件；如果同名文件存在则报错 */
	TEE_DATA_FLAG_AES256 = 0x10000000,  /**< bit24如果为1代表AES256, 为0代表AES128 */
};

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 * #TEE_ObjectHandle 的 #keyUsage，决定该object key的用法 \n
*/
enum Usage_Constants {
	TEE_USAGE_EXTRACTABLE = 0x00000001,    /**< object的key可以提取 */
	TEE_USAGE_ENCRYPT = 0x00000002,        /**< object的key可以用于加密 */
	TEE_USAGE_DECRYPT = 0x00000004,        /**< object的key可以用于解密 */
	TEE_USAGE_MAC = 0x00000008,            /**< object的key用于hash操作 */
	TEE_USAGE_SIGN = 0x00000010,           /**< object的key用于签名操作 */
	TEE_USAGE_VERIFY = 0x00000020,         /**< object的key用于验证操作 */
	TEE_USAGE_DERIVE = 0x00000040,         /**< object的key用于产生key的操作 */
	TEE_USAGE_DEFAULT = 0xFFFFFFFF,        /**< object初始化使用，默认赋给所有权限 */
};

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 * #TEE_ObjectHandle 的#handleFlags，表明该object的一些信息，是否是永久object、是否初始化等等 \n
*/
enum Handle_Flag_Constants {
	TEE_HANDLE_FLAG_PERSISTENT = 0x00010000,        /**< object为永久object */
	TEE_HANDLE_FLAG_INITIALIZED = 0x00020000,       /**< object已经初始化 */
	TEE_HANDLE_FLAG_KEY_SET = 0x00040000,           /**< 未使用 */
	TEE_HANDLE_FLAG_EXPECT_TWO_KEYS = 0x00080000,   /**< 未使用 */
};

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 * 系统资源限制，如数据流位置指示器可取最大值  \n
*/
enum Miscellaneous_Constants {
	TEE_DATA_MAX_POSITION = 0xFFFFFFFF,    /**< 数据流的位置指示器可取的最大字节长度 */
	TEE_OBJECT_ID_MAX_LEN = 64,            /**< objectID的最大字节长度 */
};

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 *
 * 数据流可以存储数据的最大字节数 \n
*/
enum TEE_DATA_Size {
	TEE_DATA_OBJECT_MAX_SIZE = 0xFFFFFFFF     /**< object数据流可以存储数据的最大字节数 */
};

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 获取object信息
 *
 * @par 描述:
 * 获取object的#TEE_ObjectInfo，将其拷贝到参数objectInfo指向的空间，该空间由用户预分配
 *
 * @attention 无
 * @param object [IN]  获取#TEE_ObjectInfo的源#TEE_ObjectHandle
 * @param objectInfo [OUT]  指向结构体的指针，该结构体用来存放得到的#TEE_ObjectInfo
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

void TEE_GetObjectInfo(TEE_ObjectHandle object, TEE_ObjectInfo *objectInfo);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 限制object的#objectUsage位
 *
 * @par 描述:
 * 限制object的#objectUsage位，该位决定object中key的用法，取值范围为#Usage_Constants，对于参数objectUsage的flag位来说: \n
 *   如果该位设置为1，则不会改变object的usage flag  \n
 *   如果该位设置为0，则object相应的object usage flag清零  \n
 * 即与原来的flag位做与操作
 *
 * @attention 新建的object会包含所有#Usage_Constants，且usage flag只能被清零，不能被置位
 * @param object [IN]  需要限制的#TEE_ObjectHandle
 * @param objectUsage [IN]  用户想改变的#objectUsage
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

void TEE_RestrictObjectUsage(TEE_ObjectHandle object, uint32_t objectUsage);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 获取object的attribute
 *
 * @par 描述:
 * 获取#TEE_ObjectHandle所指向object的#TEE_Attribute结构体中union的buffer内容，且该union成员必须为ref
 *
 * @attention #TEE_Attribute结构体中union的成员必须为ref，并且如果该#TEE_Attribute为私密的需要该object的#Usage_Constants
 * 必须包括#TEE_USAGE_EXTRACTABLE
 * @param object [IN]  获取#TEE_Attribute的源#TEE_ObjectHandle
 * @param attributeID [IN]  想要获取#TEE_Attribute的ID，例如#TEE_ObjectAttribute，也可自定义
 * @param buffer [OUT]  指针，指向的buffer用来存放获取到的buffer中的内容
 * @param size [OUT]  指针，存放内容字节长度
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_ITEM_NOT_FOUND 在object中没有发现要找的#TEE_Attribute，或者该object未初始化
 * @retval #TEE_ERROR_SHORT_BUFFER 提供的buffer太小，不能完全存放获取到的内容
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

TEE_Result TEE_GetObjectBufferAttribute(TEE_ObjectHandle object, uint32_t attributeID, void *buffer, size_t *size);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 获取object的attribute
 *
 * @par 描述:
 * 获取#TEE_ObjectHandle所指向object的#TEE_Attribute结构体中union的value，且该union成员必须为value
 *
 * @attention #TEE_Attribute结构体中union的成员必须为value，并且如果该#TEE_Attribute为私密的需要该object的#Usage_Constants
 * 必须包括#TEE_USAGE_EXTRACTABLE
 * @param object [IN]  获取#TEE_Attribute的源#TEE_ObjectHandle
 * @param attributeID [IN]  想要获取#TEE_Attribute的ID，例如#TEE_ObjectAttribute，也可自定义
 * @param a [OUT]  指针，指向的空间用来存放value结构体的a
 * @param b [OUT]  指针，指向的空间用来存放value结构体的b
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_ITEM_NOT_FOUND 在object中没有发现要找的#TEE_Attribute，或者该object未初始化
 * @retval #TEE_ERROR_ACCESS_DENIED 试图获取一个私密的#TEE_Attribute但没有置位#TEE_USAGE_EXTRACTABLE
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

TEE_Result TEE_GetObjectValueAttribute(TEE_ObjectHandle object, uint32_t attributeID, uint32_t *a, uint32_t *b);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 关闭打开的#TEE_ObjectHandle
 *
 * @par 描述:
 * 关闭打开的#TEE_ObjectHandle，该object可以是永久object或者临时object
 *
 * @attention 无
 * @param object [IN]  需要关闭的#TEE_ObjectHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

void TEE_CloseObject(TEE_ObjectHandle object);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 分配一个未初始化的object
 *
 * @par 描述:
 * 分配一个未初始化的object，用来存放key，其中#objectType和#maxObjectSize必须指定，以预分配资源
 *
 * @attention 无
 * @param objectType [IN]  待创建的object的类型，取值可以为#TEE_ObjectType
 * @param maxObjectSize [IN]  object的最大字节数
 * @param object [OUT]  指针，指向新创建的object的handle
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_OUT_OF_MEMORY 没有足够的资源去分配
 * @retval #TEE_ERROR_NOT_SUPPORTED 该object提供的字节数不被支持
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/
TEE_Result TEE_AllocateTransientObject(uint32_t objectType, uint32_t maxObjectSize, TEE_ObjectHandle *object);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 释放一个已分配资源的临时object
 *
 * @par 描述:
 * 释放一个已分配资源的临时object，函数调用后该handle失效，并释放所有分配的资源，与#TEE_AllocateTransientObject配对使用
 *
 * @attention 无
 * @param object [IN]  需要释放的#TEE_ObjectHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

void TEE_FreeTransientObject(TEE_ObjectHandle object);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 重置transient object
 *
 * @par 描述:
 * 重置临时object到它的初始状态，即刚allocate之后的状态:已分配资源但没有存入key的未初始化object，可以重新用来存入key
 *
 * @attention 无
 * @param object [IN]  需要重置的#TEE_ObjectHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

void TEE_ResetTransientObject(TEE_ObjectHandle object);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 给一个未初始化的临时object赋attribute
 *
 * @par 描述:
 * 该函数将参数attrs中的attributes赋给一个未初始化的临时object，参数attrs由可信应用(Trusted APP)提供
 *
 * @attention 无
 * @param object [IN/OUT]  已创建但未初始化的#TEE_ObjectHandle
 * @param attrs [IN]  object attribute数组，可以为一个或者多个#TEE_Attribute
 * @param attrCount [IN]  数组成员个数
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS attribute不正确或者不一致，此时必须保证object仍然未初始化
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/
TEE_Result TEE_PopulateTransientObject(TEE_ObjectHandle object, TEE_Attribute *attrs, uint32_t attrCount);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 初始化一个buffer类型的#TEE_Attribute
 *
 * @par 描述:
 * 初始化一个buffer类型的#TEE_Attribute，即#TEE_Attribute结构体中union的成员必须为ref
 *
 * @attention 无
 * @param attr [OUT]  需要初始化的#TEE_Attribute
 * @param attributeID [IN]  赋给#TEE_Attribute的ID
 * @param buffer [IN]  该buffer存放要赋值的内容
 * @param length [IN]  赋值内容的字节长度
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/
void TEE_InitRefAttribute(TEE_Attribute *attr, uint32_t attributeID, void *buffer, size_t length);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 初始化一个value类型的#TEE_Attribute，即#TEE_Attribute结构体中union的成员必须为value
 *
 * @par 描述:
 * 初始化一个value类型的#TEE_Attribute
 *
 * @attention 无
 * @param attr [OUT]  需要初始化的#TEE_Attribute
 * @param attributeID [IN]  赋给#TEE_Attribute 的ID
 * @param a [IN]  赋值给#TEE_Attribute结构体中union的成员value b的值
 * @param b [IN]  赋值给#TEE_Attribute结构体中union的成员value a的值
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

void TEE_InitValueAttribute(TEE_Attribute *attr, uint32_t attributeID, uint32_t a, uint32_t b);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 用一个初始化的object给未初始化的object赋值#TEE_Attribute
 *
 * @par 描述:
 * 该函数用一个初始化的object给一个未初始化的object赋值#TEE_Attribute，相当于把srcobject的#TEE_Attribute拷贝到destobject中
 *
 * @attention 两个object的#TEE_Attribute 类型、个数必须匹配
 * @param destObject [IN]  未初始化需要赋值的#TEE_ObjectHandle
 * @param srcObject [IN]  已初始化用来给另一个object赋值的#TEE_ObjectHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

void TEE_CopyObjectAttributes(TEE_ObjectHandle destObject, TEE_ObjectHandle srcObject);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 产生随机key或者key-pair
 *
 * @par 描述:
 * 该函数产生随机key或者key-pair，并赋值给临时object
 *
 * @attention 无
 * @param object [IN]  临时object，用来存放产生的key
 * @param keySize [IN]  要求的key的字节大小
 * @param params [IN]  产生key需要的参数
 * @param paramCount [IN]  产生key需要的参数个数
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_BAD_PARAMETERS: #产生的key和临时object所能存放的key类型不一致
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/
TEE_Result TEE_GenerateKey(TEE_ObjectHandle object, uint32_t keySize, TEE_Attribute *params, uint32_t paramCount);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 创建一个新的永久object
 *
 * @par 描述:
 * 创建一个新的永久object，可以直接初始化数据流和#TEE_Attribute，用户可以用返回的handle来访问object的#TEE_Attribute和数据流
 *
 * @attention 无
 * @param storageID [IN]   对应每个应用单独的存储空间，取值为#Object_Storage_Constants，目前仅支持#TEE_STORAGE_PRIVATE.
 * @param objectID [IN]  object  identifier，即要创建的object的名字
 * @param objectIDLen [IN]  object  identifier 的字节长度
 * @param flags [IN]  object创建后的flags，取值可以为#Data_Flag_Constants 或 #Handle_Flag_Constants中的一个或多个
 * @param attributes [IN]  临时object的#TEE_ObjectHandle，用来初始化object的#TEE_Attribute，可以为#TEE_HANDLE_NULL
 * @param initialData [IN]  初始数据，用来初始化数据流数据
 * @param initialDataLen [IN] initialData 初始数据的字节长度
 * @param object [OUT]  函数成功执行后返回的#TEE_ObjectHandle
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_ITEM_NOT_FOUND:   该storageID 不存在或者不能找到object identifier
 * @retval #TEE_ERROR_ACCESS_CONFLICT 访问权限冲突
 * @retval #TEE_ERROR_OUT_OF_MEMORY 没有足够的资源来完成操作
 * @retval #TEE_ERROR_STORAGE_NO_SPACE 没有足够的空间来创建object
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/
TEE_Result TEE_CreatePersistentObject(uint32_t storageID, void *ojbectID, size_t objectIDLen, uint32_t flags, TEE_ObjectHandle attributes, void *initialData, size_t initialDataLen, TEE_ObjectHandle *object);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 打开一个已存在的永久object
 *
 * @par 描述:
 * 打开一个已存在的永久object，返回的handle用户可以用来访问object的#TEE_Attribute和数据流
 *
 * @attention 无
 * @param storageID [IN]   对应每个应用单独的存储空间，取值为#Object_Storage_Constants，目前仅支持#TEE_STORAGE_PRIVATE
 * @param objectID [IN]  object  identifier，即要打开的object的名字
 * @param objectIDLen [IN]  object  identifier 的字节长度
 * @param flags [IN]  object打开后的flags，取值可以为#Data_Flag_Constants 或 #Handle_Flag_Constants中的一个或多个
 * @param object [OUT]  函数成功执行后返回的#TEE_ObjectHandle
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_ITEM_NOT_FOUND:   storageID 不存在或者不能找到object identifier
 * @retval #TEE_ERROR_ACCESS_CONFLICT 访问权限冲突
 * @retval #TEE_ERROR_OUT_OF_MEMORY 没有足够的资源来完成操作
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/
TEE_Result TEE_OpenPersistentObject(uint32_t storageID, void *ojbectID, size_t objectIDLen, uint32_t flags, TEE_ObjectHandle *object);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 从object的数据流读取数据，object必须为永久object
 *
 * @par 描述:
 * 该函数从object的数据流读取size字节数据到buffer指针指定的buffer,该#TEE_ObjectHandle必须已经用#TEE_DATA_FLAG_ACCESS_READ权限打开
 *
 * @attention 无
 * @param objbect [IN]   要读取的源#TEE_ObjectHandle
 * @param buffer [OUT]  存放读取数据
 * @param size [IN]  要读取的数据字节数
 * @param count [OUT]  实际读取到的数据字节数
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功，目前仅返回成功，其它错误返回值将在后续版本实现
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/
TEE_Result TEE_ReadObjectData(TEE_ObjectHandle ojbect, void *buffer, size_t size, uint32_t *count);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 往object的数据流写入数据，object必须为永久object
 *
 * @par 描述:
 * 该函数从buffer往object的数据流写入size字节的数据,#TEE_ObjectHandle必须已用#TEE_DATA_FLAG_ACCESS_WRITE权限打开
 *
 * @attention 无
 * @param ojbect [IN]   要写入的目标#TEE_ObjectHandle
 * @param buffer [IN]  存放要写入的源数据
 * @param size [IN]  要写入的数据字节数
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/
TEE_Result TEE_WriteObjectData(TEE_ObjectHandle ojbect, void *buffer, size_t size);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 改变object的数据流大小，object必须为永久object
 *
 * @par 描述:
 * 该函数改变数据流字节大小。如果size小于当前数据流的size则删除所有超出的字节。如果size大于当前数据流的size则用'0'扩展
 * #TEE_ObjectHandle必须用#TEE_DATA_FLAG_ACCESS_WRITE权限打开
 *
 * @attention 无
 * @param object [IN]   要修改的#TEE_ObjectHandle
 * @param size [IN]  数据流的新的数据大小
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_STORAGE_NO_SPACE 没有足够的空间来执行操作
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

TEE_Result TEE_TruncateObjectData(TEE_ObjectHandle object, uint32_t size);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 设置object的数据流位置指示器，object必须为永久object
 *
 * @par 描述:
 * 该函数设置#TEE_ObjectHandle所指向的数据流位置，将数据流位置设置为:偏移的起始位置+offset \n
 * 参数whence控制偏移的起始位置，取值可以为#TEE_Whence，含义如下：\n
 *	   #TEE_DATA_SEEK_SET，数据流偏移的起始位置为文件头，即0处  \n
 *	   #TEE_DATA_SEEK_CUR，数据流偏移的起始位置为当前位置，即在当前位置的基础上偏移offset  \n
 *	   #TEE_DATA_SEEK_END，数据流偏移的起始位置为文件末尾  \n
 * 参数offset为正数时向后偏移，即向文件尾部方向，负数时向前偏移，即文件头部方向
 *
 * @attention 无
 * @param object [IN]   要设置的#TEE_ObjectHandle
 * @param offset [IN]  数据流位置移动的字节数
 * @param whence [IN]  计算数据流偏移的初始位置
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_OVERFLOW 操作使位置指示器的值超过了其系统限制#TEE_DATA_MAX_POSITION
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
*/

TEE_Result TEE_SeekObjectData(TEE_ObjectHandle object, int32_t offset, TEE_Whence whence);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 关闭打开的#TEE_ObjectHandle，并删除object
 *
 * @par 描述:
 * 关闭打开的#TEE_ObjectHandle，并删除object，object必须为永久object且必须已用#TEE_DATA_FLAG_ACCESS_WRITE_META权限打开
 *
 * @attention 无
 * @param object [IN]  需要关闭并删除的#TEE_ObjectHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */

void TEE_CloseAndDeletePersistentObject(TEE_ObjectHandle object);


/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 获取object  的信息: 数据部分长度和数据流的当前位置
 *
 * @par 描述:
 * 获取object数据部分的信息，数据部分的总长度和数据流当前的位置
 *
 * @attention 无
 *  *@param object [IN]   要设置的#TEE_ObjectHandle
 * @param pos [out]  数据流位置
 * @param len [IN]  数据流长度
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */

TEE_Result TEE_InfoObjectData(TEE_ObjectHandle object, uint32_t *pos, uint32_t *len);
/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 同步打开的#TEE_ObjectHandle到磁盘
 *
 * @par 描述:
 * 同步打开的#TEE_ObjectHandle，并同步对应的安全属性文件(4个)到磁盘
 *
 * @attention 无
 * @param object [IN]  需要同步的#TEE_ObjectHandle
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */

TEE_Result TEE_SyncPersistentObject(TEE_ObjectHandle object);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 改变object identifier，即改变object的名字
 *
 * @par 描述:
 * 改变object identifier，该#TEE_ObjectHandle必须用#TEE_DATA_FLAG_ACCESS_WRITE_META权限打开
 *
 * @attention 无
 * @param ojbect [IN/OUT]   要修改的object handle
 * @param newObjectID [IN]  新的object identifier
 * @param newObjectIDLen [IN]  新的object identifier长度
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */
TEE_Result TEE_RenamePersistentObject(TEE_ObjectHandle  object, void *newObjectID, size_t newObjectIDLen);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 分配一个未初始化的object enumerator 的handle
 *
 * @par 描述:
 * 分配一个未初始化的object enumerator的handle
 *
 * @attention 无
 * @param object [OUT]  指针，指向新创建的object enumerator 的 handle
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ERROR_OUT_OF_MEMORY 没有足够的资源去分配
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */
TEE_Result TEE_AllocatePersistentObjectEnumerator(TEE_ObjectEnumHandle *objectEnumerator);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 释放一个已分配资源的object enumerator handle
 *
 * @par 描述:
 * 释放一个已分配资源的临时object enumerator handle，函数调用后该handle失效，并释放所有分配的资源，与#TEE_AllocatePersistentObjectEnumerator配对使用
 *
 * @attention 无
 * @param object [IN]  需要释放的#TEE_ObjectEnumHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */
void TEE_FreePersistentObjectEnumerator(TEE_ObjectEnumHandle objectEnumerator);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 重置object enumerator
 *
 * @par 描述:
 * 重置临时object enumerator 到它的初始状态，即刚allocate之后的状态
 *
 * @attention 无
 * @param object [IN]  需要重置的object enumerator 的#TEE_ObjectEnumHandle
 *
 * @retval 无
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */
void TEE_ResetPersistentObjectEnumerator(TEE_ObjectEnumHandle objectEnumerator);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 初始化object enumerator，开始列举给定存储空间的所有object，object的信息可以通过#TEE_GetNextPersistentObject函数获取
 *
 * @par 描述:
 * 开始列举给定存储空间的所有object，object的信息可以通过#TEE_GetNextPersistentObject函数获取
 *
 * @attention 无
 * @param object [IN]  已分配好的object enumerator 的#TEE_ObjectEnumHandle
 * @param storageID [IN]   对应每个应用单独的存储空间，取值为#Object_Storage_Constants，目前仅支持#TEE_STORAGE_PRIVATE
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ITEM_NOT_FOUND storageID不是#TEE_STORAGE_PRIVATE或者在#TEE_STORAGE_PRIVATE存储空间内没有object
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */
TEE_Result TEE_StartPersistentObjectEnumerator(TEE_ObjectEnumHandle objectEnumerator,  uint32_t   storageID);

/**
 * @ingroup  TEE_TRUSTED_STORAGE_API
 * @brief 获取object enumerator中的下一个object，并返回object的#TEE_ObjectInfo, objectID , objectIDLen
 *
 * @par 描述:
 *  获取object enumerator中的下一个object，并返回object的#TEE_ObjectInfo, objectID , objectIDLen信息
 *
 * @attention 无
 * @param object [IN]  已初始化好的object enumerator 的#TEE_ObjectEnumHandle
 * @param objectInfo [OUT]  指向结构体的指针，该结构体用来存放得到的#TEE_ObjectInfo
 * @param objectInfo [OUT]  指向一段buffer的指针，用来存放得到的objectID
 * @param objectInfo [OUT]  用来存放得到的objectIDLen
 *
 * @retval #TEE_SUCCESS 表示该函数执行成功
 * @retval #TEE_ITEM_NOT_FOUND enumerator已经没有object或者enumerator没有被初始化
 *
 * @par 依赖:
 * @li Tee_trusted_storage_api.h：该接口声明所在的头文件。
 * @since TrustedCore V100R002C00B302
 */
TEE_Result TEE_GetNextPersistentObject(TEE_ObjectEnumHandle objectEnumerator,  TEE_ObjectInfo *objectInfo, void *objectID,  size_t *objectIDLen);

#endif
/**
 * History: \n
 * 2013-3-10，z00202529，初始化该文件 \n
 * 2013-6-27，z00202529，增加Persistent Object Enumeration Functions  \n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
