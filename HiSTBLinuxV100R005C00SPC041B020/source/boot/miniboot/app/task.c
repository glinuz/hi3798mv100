/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include "app.h"

#if 0
static uint32 task1_args = 0;
static void task1(void *args)
{
	uint32 *times = (uint32 *)args;
	printf("task1: %d\n", (*times)++);
}

static uint32 task2_args = 0;
static void task2(void *args)
{
	uint32 *times = (uint32 *)args;
	printf("task2: %d\n", (*times)++);
}

static uint32 task3_args = 0;
static void task3(void *args)
{
	uint32 *times = (uint32 *)args;
	printf("task3: %d\n", (*times)++);
}

static uint32 task4_args = 0;
static void task4(void *args)
{
	uint32 *times = (uint32 *)args;
	printf("task4: %d\n", (*times)++);
}
#endif

void add_task(void)
{
//	task_connect(1000, task1, (void *)&task1_args);
//	task_connect(2000, task2, (void *)&task2_args);
//	task_connect(3000, task3, (void *)&task3_args);
//	task_connect(4000, task4, (void *)&task4_args);
}
