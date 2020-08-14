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

#ifndef EARLY_UARTH
#define EARLY_UARTH

void uart_early_init(void);
void uart_early_puts(const char *ss);
void uart_early_put_hex(int hex);
void uart_early_putc(int chr);

#define uart_early_printf(_fmt, args...) do { \
	char buf[64]; \
	snprintf(buf, sizeof(buf), _fmt, ##args); \
	uart_early_puts(buf); \
} while (0)

#endif /* EARLY_UARTH */
