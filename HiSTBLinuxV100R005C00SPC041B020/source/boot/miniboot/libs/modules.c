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

#include <boot.h>
#include <config.h>
#include <compile.h>
#include <module.h>

extern unsigned int __init_start;
extern unsigned int __init_end;

#define FOR_INIT_DATA(_pri, _fn) do { \
	uint32 *ptr; \
	for (ptr = &__init_start; ptr < &__init_end; ptr++) { \
		struct module_t *module = (struct module_t *)(*ptr); \
		if (module->priority == _pri && module->_fn) \
			module->_fn(module->ptr); \
	} \
} while (0)

/******************************************************************************/

void module_init(void)
{
	int ix;
	for (ix = 0; ix < MODULE_PRI_MAX; ix++)
		FOR_INIT_DATA(ix, init);
}
/*****************************************************************************/

void module_exit(void)
{
	int ix;
	for (ix = MODULE_PRI_MAX - 1; ix >= 0; ix--)
		FOR_INIT_DATA(ix, exit);
}
/*****************************************************************************/

void modules_reboot(void)
{
	int ix;
	for (ix = MODULE_PRI_MAX - 1; ix >= 0; ix--)
		FOR_INIT_DATA(ix, reboot);
}
