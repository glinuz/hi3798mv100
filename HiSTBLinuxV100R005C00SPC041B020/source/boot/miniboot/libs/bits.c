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

#include <stddef.h>

/*****************************************************************************/

int bits_count(uint32 nn)
{
	int count = 0;
	while (nn) {
		if (nn & 1) count++;
		nn >>= 1;
	}
	return count;
}
/*****************************************************************************/

int ffs(int nn)
{
	int pos = 1;
	int mask = 0xFFFF;
	int shift = 16;

	if (!nn)
		return 0;

	while (shift) {
		if (!(nn & mask)) {
			nn >>= shift;
			pos += shift;
		}
		shift >>= 1;
		mask >>= shift;
	}
	return pos;
}
/*****************************************************************************/

int ffs64(uint64 nn)
{
	int pos = 1;
	uint32 mask = 0xFFFFFFFF;
	uint32 shift = 32;

	if (!nn)
		return 0;

	while (shift) {
		if (!(nn & mask)) {
			nn >>= shift;
			pos += shift;
		}
		shift >>= 1;
		mask >>= shift;
	}
	return pos;
}
