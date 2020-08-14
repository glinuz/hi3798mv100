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

#ifndef PARTSH
#define PARTSH
/******************************************************************************/

#include <stddef.h>
/*
 * @devpartname: part string name, such as mtdparts, blkdevparts;
 * @devname:     device name, such as "hinand", "hisfc", "mmcblk0"
 * @partname:    partition name which want get range.
 */
int flash_find_part(const char *devpartname, const char *devname,
		    const char *partname, uint64 *start, uint64 *size);


/******************************************************************************/
#endif /* PARTSH */
