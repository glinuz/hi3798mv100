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

#ifndef DELAYH
#define DELAYH

unsigned int get_timer_value(void);
unsigned int get_ticks(void);
unsigned int get_ms_ticks(void);

unsigned int sys_now(void);

void udelay(unsigned int usec);

void mdelay(unsigned int msec);

unsigned int do_gettime(unsigned long *sec, unsigned short *msec,
			unsigned short *usec);

#endif /* DELAYH */
