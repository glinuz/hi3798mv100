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

#ifndef INTFH
#define INTFH

#define KEY_LEFT                       0x8044
#define KEY_RIGHT                      0x8043
#define KEY_UP                         0x8041
#define KEY_DOWN                       0x8042

#define KEY_BACKSPACE                  0x08
#define KEY_DELETE                     0x7f
#define KEY_CTRL_C                     0x03

int cl_getc(void);

void cl_putc(int ch);

void cl_puts(const char *s);

#endif /* INTFH */
