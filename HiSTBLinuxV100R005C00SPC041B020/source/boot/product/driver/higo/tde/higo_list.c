/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : higo_list.c
Version             : Initial Draft
Author              :
Created             : 2016/01/12
Description         : Á´±íÊ¹ÓÃ
Function List       :
History             :
Date                       Author                   Modification
2016/01/12                 y00181162                Created file
******************************************************************************/

/*********************************add include here******************************/
#include "higo_list.h"

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

void __list_add(struct list_head *pnew,struct list_head *pprev,struct list_head *pnext)
{
    pnext->prev = pnew;
    pnew->next  = pnext;
    pnew->prev  = pprev;
    pprev->next = pnew;
}

void list_add(struct list_head *pnew, struct list_head *pHead)
{
    __list_add(pnew, pHead, pHead->next);
}
void list_add_tail(struct list_head *pnew, struct list_head *pHead)
{
    __list_add(pnew, pHead->prev, pHead);
}
void __list_del(struct list_head *pprev, struct list_head *pnext)
{
    pnext->prev = pprev;
    pprev->next = pnext;
}

void list_del(struct list_head *pentry)
{
    __list_del(pentry->prev, pentry->next);
    pentry->next = (void *) 0;
    pentry->prev = (void *) 0;
}

void list_del_init(struct list_head *pentry)
{
    __list_del(pentry->prev, pentry->next);
    INIT_LIST_HEAD(pentry);
}

int list_empty(struct list_head *pHead)
{
    return pHead->next == pHead;
}
