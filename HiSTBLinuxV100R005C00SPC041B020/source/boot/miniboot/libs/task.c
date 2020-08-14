/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
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

#include <task.h>
#include <irq.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>

#define MOULE_NAME "task: "
#include <debug.h>

struct task_t {
	uint32 msec;
	uint32 elapse;
	void (*handle)(void *args);
	void *args;

	uint32 count;
};

struct task_ctrl_t {
	struct task_t *task;
	int nr_task;
	int max_task;
};

static struct task_ctrl_t task_ctrl = {NULL, 0, 0};
#define GET_TASK(_index)  (&task_ctrl.task[_index])

/*****************************************************************************/

void task_loop(void)
{
	int nr_task = task_ctrl.nr_task;
	struct task_t *task;

	while (nr_task-- > 0) {
		task = GET_TASK(nr_task);
		task->elapse += CONFIG_TASK_CYCLE;

		if (task->elapse >= task->msec) {
			task->elapse = 0;
			task->handle(task->args);
			task->count++;
		}
	}
}
/*****************************************************************************/

int task_connect(uint32 msec, void (*handle)(void *args), void *args)
{
	int irqflags;
	struct task_t *task;

	if (task_ctrl.nr_task >= task_ctrl.max_task) {
		pr_error("not many space for task.\n");
		return -EINVAL;
	}

	irqflags = cpu_irq_save();

	task = GET_TASK(task_ctrl.nr_task);
	task_ctrl.nr_task++;

	task->msec = msec;
	task->elapse = 0;
	task->handle = handle;
	task->args = args;
	task->count = 0;

	cpu_irq_restore(irqflags);

	return 0;
}
/*****************************************************************************/

int task_init(uint32 count)
{
	int size;

	task_ctrl.max_task = count;
	size = task_ctrl.max_task * sizeof(struct task_t);
	
	task_ctrl.task = (struct task_t *)malloc(size);
	if (!task_ctrl.task) {
		pr_error("can't malloc memory.\n");
		return -EINVAL;
	}

	memset(task_ctrl.task, 0, size);
	task_ctrl.nr_task = 0;

	return 0;
}

