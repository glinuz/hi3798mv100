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

#include <stddef.h>

union uint64_joint_t {
	struct {
		uint32 u32l;
		uint32 u32h;
	};
	uint64 u64;
};
/*****************************************************************************/
/*
 * logical shift right
 */
uint64 __aeabi_llsr(uint64 x, int shift)
{
	union uint64_joint_t d;

	if (shift <= 0)
		return x;

	d.u64 = x;

	if (shift < 32) {
		d.u32l >>= shift;
		d.u32l |= d.u32h << (32 - shift);
		d.u32h >>= shift;
	} else if (shift < 64) {
		d.u32l = d.u32h >> (shift - 32);
		d.u32h = 0;
	} else {
		return 0;
	}

	return d.u64;
}
/*****************************************************************************/
/*
 * logical shift left 
 */
uint64 __aeabi_llsl(uint64 x, int shift)
{
	union uint64_joint_t d;

	if (shift <= 0)
		return x;

	d.u64 = x;

	if (shift < 32) {
		d.u32h <<= shift;
		d.u32h |= d.u32l >> (32 - shift);
		d.u32l <<= shift;
	} else if (shift < 64) {
		d.u32h = d.u32l << (shift - 32);
		d.u32l = 0;
	} else {
		return 0;
	}

	return d.u64;
}


