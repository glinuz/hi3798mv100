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

#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/cacheflush.h>
#include <linux/interrupt.h>

EXPORT_SYMBOL(saved_command_line);
#ifdef CONFIG_ARM64
EXPORT_SYMBOL(__flush_dcache_area);
#endif

#if defined(CONFIG_SMP)
EXPORT_SYMBOL(__irq_set_affinity);
#endif

#ifdef CONFIG_GPIOLIB
extern struct spinlock gpio_lock;
extern struct list_head gpio_chips;

EXPORT_SYMBOL(gpio_lock);
EXPORT_SYMBOL(gpio_chips);
#endif

char *audio_options_string = NULL;
static int __init  audio_options_setup(char *s)
{
	audio_options_string = s;
	return 1;
}
__setup("audio=", audio_options_setup);
EXPORT_SYMBOL(audio_options_string);

char *pq_options_string = NULL;
static int __init  pq_options_setup(char *s)
{
	pq_options_string = s;
	return 1;
}
__setup("pq=", pq_options_setup);
EXPORT_SYMBOL(pq_options_string);

static bool dongle = false;
static int __init  dongle_options_setup(char *s)
{
	dongle = true;
	return 0;
}
early_param("dongle", dongle_options_setup);

bool is_dongle(void)
{
	return dongle;
}
EXPORT_SYMBOL(is_dongle);

