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

#ifndef MODULEH
#define MODULEH

#define MODULE_PRI_BASE                    0
#define MODULE_PRI_DRV                     1
#define MODULE_PRI_ENV                     2
#define MODULE_PRI_DEF                     8
#define MODULE_PRI_MAX                     10

struct module_t {
	int priority;         /* the function run priority */
	void *ptr; /* function parameter */
	void (*init)(void *ptr);   /* system init call */
	void (*exit)(void *ptr);   /* before entry kernel or go call */
	void (*reboot)(void *ptr); /* before reboot call */
};

#define MODULE(module) \
	const static int __attribute__ ((section(".init"),used)) \
		p##module = (int)&module

void module_init(void);

void module_exit(void);

void modules_reboot(void);

#endif /* MODULEH */
