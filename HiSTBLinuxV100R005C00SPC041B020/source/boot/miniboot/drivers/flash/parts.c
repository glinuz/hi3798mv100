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

#include <flash.h>
#include <malloc.h>
#include <errno.h>
#include <env_set.h>
#include <string.h>
#include <find_parts.h>

//#define MODULE_DEBUG
#include <debug.h>

/*****************************************************************************/

static char *devpart_duplicate(const char *devpartname)
{
	size_t size;
	char *devpart;
	char *tail;
	char *bootargs = env_get("bootargs");

	if (!bootargs)
		return NULL;

	devpart = strstr(bootargs, devpartname);
	if (!devpart)
		return NULL;

	/* "mtdparts" or "blkdevparts" should appeare only once in bootargs */
	devpart += strlen(devpartname) + 1;
	if (*(devpart - 1) != '=')
		return NULL;

	tail = strchr(devpart, ' ');
	if (tail)
		size = tail - devpart;
	else
		size = strlen(devpart);

	return strndup(devpart, size);
}
/*****************************************************************************/
/*
 * bootargs="mtdparts=hinand:2M(boot),4M(kernel),60M(rootfs),40M(test),-(others)"
 * @devpartname - device partition name, such as "mtdparts", "blkdevparts"
 * @devname     - device name, such as "hinand", "hi_sfc", "emmcblk0"
 * @partname    - partition name. such as "boot", "kernel", ...
 *
 */
int flash_find_part(const char *devpartname, const char *devname,
		    const char *partname, uint64 *start, uint64 *size)
{
	char *str_part;

	ASSERT(devpartname != NULL && devname != NULL && partname != NULL);
	ASSERT(start != NULL && size != NULL);

	str_part = devpart_duplicate(devpartname);
	if (!str_part)
		return -ENODEV;

	return find_flash_part(str_part, devname, partname, start, size);
}

