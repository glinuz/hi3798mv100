/**
 * @file tee_client_list.h
 *
 * Copyright(C), 2008-2013, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义链表数据类型和基本操作\n
*/

/** @defgroup TEEC_List 链表数据类型和基本操作
 *@ingroup TEEC_API
 */

#ifndef _TEE_CLIENT_LIST_H_
#define _TEE_CLIENT_LIST_H_

/**
 * @ingroup TEEC_List
 * 链表类型定义
 */
struct list_node {
    struct list_node *next;  /**< 指向next节点  */
    struct list_node *prev;  /**< 指向prev节点  */
};

/**
 * @ingroup TEEC_List
 * @brief 定义一个链表节点
 *
 * @par 描述:
 * 宏定义，定义一个链表节点，并对其初始化
 * @param name [IN] 定义的链表名称
 */
#define LIST_DECLARE(name) \
    struct list_node name = { \
        .next = &name, \
        .prev = &name, \
    }

/**
 * @ingroup  TEEC_List
 * @brief 初始化链表
 *
 * @par 描述:
 * 对链表头进行初始化操作
 *
 * @attention 无
 * @param list [IN/OUT]链表指针，取值不能为空
 *
 * @retval 无
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_list.h：该接口声明所在头文件
 * @since V100R002C00B301
 */
/*static inline void INIT_LIST_HEAD(struct list_node *list)
{
    list->next = list;
    list->prev = list;
}*/

/**
 * @ingroup TEEC_List
 * 获取链表list的next节点
 */
#define LIST_HEAD(list) ((list)->next)

/**
 * @ingroup TEEC_List
 * 获取链表list的prev节点
 */
#define LIST_TAIL(list) ((list)->prev)


/**
 * @ingroup TEEC_List
 * 判断链表list是否为空
 */
#define LIST_EMPTY(list) ((list) == (list)->next)

/**
 * @ingroup  TEEC_List
 * @brief 从链表头部插入新节点
 *
 * @par 描述:
 * 从链表头部插入新节点
 *
 * @attention 无
 * @param list [IN/OUT]链表头指针，取值不能为空
 * @param entry [IN/OUT]新增链表节点指针，取值不能为空
 *
 * @retval 无
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_list.h：该接口声明所在头文件
 * @see list_insert_tail
 * @since V100R002C00B301
 */
static inline void list_insert_head(struct list_node *list, struct list_node *entry)
{
    list->next->prev = entry;
    entry->next = list->next;
    entry->prev = list;
    list->next = entry;
}

/**
 * @ingroup  TEEC_List
 * @brief 从链表尾部插入新节点
 *
 * @par 描述:
 * 从链表尾部插入新节点
 *
 * @attention 无
 * @param list [IN/OUT]链表头指针，取值不能为空
 * @param entry [IN/OUT]新增链表节点指针，取值不能为空
 *
 * @retval 无
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_list.h：该接口声明所在头文件
 * @see list_insert_head
 * @since V100R002C00B301
 */
static inline void list_insert_tail(struct list_node *list, struct list_node *entry)
{
    entry->next = list;
    entry->prev = list->prev;
    list->prev->next = entry;
    list->prev = entry;
}

/**
 * @ingroup  TEEC_List
 * @brief 删除节点
 *
 * @par 描述:
 * 删除指定的节点
 *
 * @attention 返回后，用户需要释放删除节点的内存
 * @param entry [IN]待删除的链表节点指针，取值不能为空
 *
 * @retval 无
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_list.h：该接口声明所在头文件
 * @see 无
 * @since V100R002C00B301
 */
static inline void list_remove(struct list_node *entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
}

/**
 * @ingroup  TEEC_List
 * @brief 删除链表头结点
 *
 * @par 描述:
 * 删除指定链表的头节点
 *
 * @attention 返回后，用户可以释放删除节点的内存
 * @param list [IN]链表头指针，取值不能为空
 *
 * @retval #NULL 链表为空
 * @retval 非NULL 链表头结点
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_list.h：该接口声明所在头文件
 * @see list_remove_tail
 * @since V100R002C00B301
 */
static inline struct list_node* list_remove_head(struct list_node *list)
{
    struct list_node* entry = NULL;
    if (!LIST_EMPTY(list)) {
        entry = list->next;
        list_remove(entry);
    }
    return entry;
}

/**
 * @ingroup  TEEC_List
 * @brief 删除链表尾结点
 *
 * @par 描述:
 * 删除指定链表的尾节点
 *
 * @attention 返回后，用户可以释放删除节点的内存
 * @param list [IN]链表头指针，取值不能为空
 *
 * @retval NULL 链表为空
 * @retval 非空 链表尾结点
 *
 * @par 依赖:
 * @li libteec：该接口所属的共享库
 * @li tee_client_list.h：该接口声明所在头文件
 * @see list_remove_head
 * @since V100R002C00B301
 */
static inline struct list_node* list_remove_tail(struct list_node *list)
{
    struct list_node* entry = NULL;
    if (!LIST_EMPTY(list)) {
        entry = list->prev;
        list_remove(entry);
    }
    return entry;
}

/**
 * @ingroup  TEEC_List
 * @brief 从成员member的指针ptr找到其所属数据结构type的指针
 *
 * @par 描述:
 * 从成员member的指针ptr找到其所属数据结构type的指针(起始地址)
 *
 * @attention 无
 * @param ptr [IN]链表指针，取值不能为空
 * @param type [IN]指针ptr所在的结构体类型
 * @param member [IN]指针ptr指向结构体type中的成员member
 *
 * @retval 非空 返回ptr所在结构体type的起始地址，返回类型为type*
 *
 * @since V100R002C00B301
 */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * @ingroup  TEEC_List
 * @brief 遍历链表节点
 *
 * @par 描述:
 * 遍历链表，返回每个链表节点
 *
 * @attention 无
 * @param pos [OUT]链表指针，取值不能为空
 * @param list [IN]链表头指针，取值不能为空
 *
 * @retval 非空 返回链表节点
 *
 * @since V100R002C00B301
 */
#define list_for_each(pos, list) \
    for (pos = (list)->next; pos != (list); pos = pos->next)

/**
 * @ingroup  TEEC_List
 * @brief 遍历链表节点所在数据结构的起始地址
 *
 * @par 描述:
 * 遍历链表，并获取链表节点所在数据结构的起始地址
 *
 * @attention 无
 * @param pos [IN]链表指针，取值不能为空
 * @param list [IN]链表头指针，取值不能为空
 * @param member [IN]链表节点所属的成员member
 *
 * @retval 非空 链表节点所在数据结构的起始地址，返回类型为typeof(*pos)
 *
 * @since V100R002C00B301
 */
#define list_for_each_entry(pos, list, member) \
    for (pos = list_entry((list)->next, typeof(*pos), member); \
            &pos->member != (list); \
            pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * @ingroup  TEEC_List
 * @brief 遍历链表节点所在数据结构的起始地址
 *
 * @par 描述:
 * 遍历链表，并获取链表节点所在数据结构的起始地址
 *
 * @attention 无
 * @param pos [IN]链表指针，取值不能为空
 * @param n [IN]链表指针，取值不能为空
 * @param list [IN]链表头指针，取值不能为空
 * @param member [IN]链表节点所属的成员member
 *
 * @retval 非空 链表节点所在数据结构的起始地址，返回类型为typeof(*pos)
 *
 * @since V100R002C00B301
 */
#define list_for_each_entry_safe(pos, n, list, member) \
    for (pos = list_entry((list)->next, typeof(*pos), member), \
            n = list_entry(pos->member.next, typeof(*pos), member); \
            &pos->member != (list); \
            pos = n, n = list_entry(n->member.next, typeof(*n), member))

#endif
/**
 * History: \n
 * 2013-4-12 h00211444: Create this file\n
 *
 * vi: set expandtab ts=4 sw=4 tw=80:
*/
