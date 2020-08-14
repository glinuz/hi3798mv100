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

static unsigned int __next = 1;

static int __rand(unsigned int *value)
{
	int aa, bb, rr;

	if (*value == 0)
		*value = 123459876;
	aa = *value / 127773;
	bb = *value % 127773;
	rr = 16807 * bb - 2836 * aa;
	if (rr < 0)
		rr += 0x7fffffff;
	*value = rr;

	return rr;
}
/******************************************************************************/

int rand()
{
	return __rand(&__next);
}
/******************************************************************************/

void srand(unsigned int seed)
{
	__next = seed;
}
/******************************************************************************/

