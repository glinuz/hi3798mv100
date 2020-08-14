/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : higo_list.h
Version             : Initial Draft
Author              :
Created             : 2016/01/12
Description         : Á´±íÊ¹ÓÃ
Function List       :
History             :
Date                       Author                   Modification
2016/01/12                 y00181162                Created file
******************************************************************************/
#ifndef _HIGO_LIST_H
#define _HIGO_LIST_H

/*********************************add include here******************************/


/***************************** Macro Definition ******************************/
#ifndef ARCH_HAS_PREFETCH
#define ARCH_HAS_PREFETCH
#endif

/*************************** Structure Definition ****************************/
struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(list_name) { &(list_name), &(list_name) }

#define LIST_HEAD(list_name) \
    struct list_head list_name = LIST_HEAD_INIT(list_name)

#define INIT_LIST_HEAD(pttr) do { \
    (pttr)->next = (pttr); (pttr)->prev = (pttr); \
} while (0)


/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
void __list_add(struct list_head *pnew,struct list_head *pprev,struct list_head *pnext);

void list_add(struct list_head *pnew, struct list_head *pHead);

void list_add_tail(struct list_head *pnew, struct list_head *pHead);

void __list_del(struct list_head *pprev, struct list_head *pnext);

void list_del(struct list_head *pentry);

void list_del_init(struct list_head *pentry);

int list_empty(struct list_head *pHead);

#define list_entry(pttr, type, hmember) \
    ((type *)((char *)(pttr)-(unsigned long)(&((type *)0)->hmember)))


#endif
