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

#ifndef ASM_IO_H
#define ASM_IO_H

#define readb(_a)        (*(volatile unsigned char *)(_a))
#define readw(_a)        (*(volatile unsigned short *)(_a))
#define readl(_a)        (*(volatile unsigned int *)(_a))

#define writeb(_v, _a)   (*(volatile unsigned char *)(_a) = (_v))
#define writew(_v, _a)   (*(volatile unsigned short *)(_a) = (_v))
#define writel(_v, _a)   (*(volatile unsigned int *)(_a) = (_v))

#endif /* ASM_IO_H */
