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
 * come from freebsd: src/sbin/hastd/crc32.h
******************************************************************************/
/*-
 *  COPYRIGHT (C) 1986 Gary S. Brown.  You may use this program, or
 *  code or tables extracted from it, as desired without restriction.
 *
 * $FreeBSD: src/sys/boot/common/crc32.h,v 1.1.2.2.4.1 2012/03/03 06:15:13 kensmith Exp $
 */

#ifndef _CRC32_H_
#define _CRC32_H_

#include <stddef.h>

uint32 crc32(uint32 crc, const void *buf, uint32 size);

#endif /* !_CRC32_H_ */
