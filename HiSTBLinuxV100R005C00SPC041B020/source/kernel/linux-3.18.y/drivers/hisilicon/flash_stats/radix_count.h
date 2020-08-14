/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
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
 * Create By Cai Zhiying 2015.8.10
 *
******************************************************************************/

#ifndef RADIX_COUNT_H
#define RADIX_COUNT_H

/******************************************************************************/

struct radix_u16 {
	u16 ***ptr;
	int m1, m2;
};
/******************************************************************************/

static inline int get_radix_u16(struct radix_u16 *radix, int index)
{
	u32 r0, r1, r2;

	r0 = (index >> 18) & 0x3FF;
	r1 = (index >> 9) & 0x3FF;
	r2 = index & 0x3FF;

	if (!radix->ptr)
		return 0;

	if (!radix->ptr[r0])
		return 0;

	if (!radix->ptr[r0][r1])
		return 0;

	return radix->ptr[r0][r1][r2];
}
/******************************************************************************/

static inline void set_radix_u16(struct radix_u16 *radix, int index, u16 value)
{
	u32 r0, r1, r2;

	r0 = (index >> 18) & 0x3FF;
	r1 = (index >> 9) & 0x3FF;
	r2 = index & 0x3FF;

	if (!radix->ptr) {
		radix->ptr = (u16 ***)kzalloc(sizeof(u16 **) << 10, GFP_KERNEL);
	}

	if (!radix->ptr[r0]) {
		radix->ptr[r0] = (u16 **)kzalloc(sizeof(u16 *) << 10, GFP_KERNEL);
		radix->m1++;
	}

	if (!radix->ptr[r0][r1]) {
		radix->ptr[r0][r1] = (u16 *)kzalloc(sizeof(u16) << 10, GFP_KERNEL);
		radix->m2++;
	}

	radix->ptr[r0][r1][r2] = value;
}
/******************************************************************************/

static inline void inc_radix_u16(struct radix_u16 *radix, int index, u16 value)
{
	u32 r0, r1, r2;

	r0 = (index >> 18) & 0x3FF;
	r1 = (index >> 9) & 0x3FF;
	r2 = index & 0x3FF;

	if (!radix->ptr) {
		radix->ptr = (u16 ***)kzalloc(sizeof(u16 **) << 10, GFP_KERNEL);
	}

	if (!radix->ptr[r0]) {
		radix->ptr[r0] = (u16 **)kzalloc(sizeof(u16 *) << 10, GFP_KERNEL);
		radix->m1++;
	}

	if (!radix->ptr[r0][r1]) {
		radix->ptr[r0][r1] = (u16 *)kzalloc(sizeof(u16) << 10, GFP_KERNEL);
		radix->m2++;
	}

	radix->ptr[r0][r1][r2] += value;
}
/******************************************************************************/

static inline void free_radix_u16(struct radix_u16 *radix)
{
	int r0, r1;

	if (!radix->ptr)
		return;

	for (r0 = 0; r0 < 1024 && radix->m1; r0++) {
		if (!radix->ptr[r0])
			continue;

		for (r1 = 0; r1 < 1024 && radix->m2; r1++) {
			if (!radix->ptr[r0][r1])
				continue;

			kfree(radix->ptr[r0][r1]);
			radix->ptr[r0][r1] = NULL;
			radix->m2--;
		}

		kfree(radix->ptr[r0]);
		radix->ptr[r0] = NULL;
		radix->m1--;
	}

	kfree(radix->ptr);
	radix->ptr = NULL;
}
/******************************************************************************/

#endif /* RADIX_COUNT_H */

