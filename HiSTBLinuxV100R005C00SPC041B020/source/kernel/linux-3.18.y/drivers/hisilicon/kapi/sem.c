/******************************************************************************
 *  Copyright (C) 2014 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Lai Yingjun 2014.1.26
 *
******************************************************************************/
#define pr_fmt(fmt) "os_sem: " fmt

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/syscalls.h>
#include <linux/ipc.h>

/******************************************************************************/

long os_semctl(int semid, int semnum, int cmd, unsigned long arg)
{
	return sys_semctl(semid, semnum, cmd, arg);
}
EXPORT_SYMBOL(os_semctl);

/******************************************************************************/

long os_semget(key_t key, int nsems, int semflg)
{
	return sys_semget(key, nsems, semflg);
}
EXPORT_SYMBOL(os_semget);