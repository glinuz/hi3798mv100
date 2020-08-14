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

#ifndef DEBUGH
#define DEBUGH
/******************************************************************************/
#include <config.h>
#include <stdio.h>

#define _BEGIN          "\033["
#define _NORMAL         "0"
#define _HIGH           "1"
#define _FLICKER        "5"
#define _REVERSE        "7"
#define _END            "m"

#define frBLACK          "30"
#define frRED            "31"
#define frGREEN          "32"
#define frYELLOW         "33"
#define frBLUE           "34"
#define frPURPLE         "35"
#define frCYAN           "36"
#define frWHITE          "37" 

#define bgBLACK           "40"
#define bgRED             "41"
#define bgGREEN           "42" 
#define bgYELLOW          "43" 
#define bgBLUE            "44" 
#define bgPURPLE          "45"
#define bgCYAN            "46"
#define bgWHITE           "47"

#define _NONE           _BEGIN _NORMAL _END
#define _HIGH_LIGHT     _BEGIN _HIGH _END
#define _RED            _BEGIN frRED _END
#define _YELLOW         _BEGIN frYELLOW _END

#ifdef CONFIG_DEBUG
#  include <trace.h>
#  define ASSERT(_p) if (!(_p)) { \
	printf("%s(%s,%d):assert:(%s)\n", __FILE__, __FUNCTION__, __LINE__, #_p); \
	dump_stack();}
#  define ASSERT1(_p, _fmt, args...) if (!(_p)) { \
	printf("%s(%s,%d):assert:(%s)\n" _fmt, __FILE__, __FUNCTION__, __LINE__, #_p, ##args); \
	dump_stack();}
#else
#  define ASSERT(_p)
#  define ASSERT1(_p, _fmt, args...)
#endif

#ifdef MODULE_DEBUG
#  define dbgout(_fmt, args...) printf(_fmt, ##args)
#  define fdbgout(_fmt, args...) \
	printf(_YELLOW "%s(%d): " _fmt _NONE, __FILE__, __LINE__, ##args)
#else
#  define dbgout(_fmt, args...)
#  define fdbgout(_fmt, args...)
#endif

#ifndef MOULE_NAME
#  define __MOULE_NAME(_x) _x
#  define MOULE_NAME __MOULE_NAME(__FILE__)": "
#endif /* MOULE_NAME */

#define pr_error(_fmt, args...)  printf(MOULE_NAME "*** " _fmt, ##args)
#define pr_warn(_fmt, args...)   printf(MOULE_NAME "!!! " _fmt, ##args)
#define pr_info(_fmt, args...)   printf(MOULE_NAME _fmt, ##args)
#define pr_debug(_fmt, args...)  printf(MOULE_NAME "debug! " _fmt, ##args)

int dump_hex(uint32 addr, char *buf, uint32 sz_buf, uint32 width);

/******************************************************************************/
#endif /* DEBUGH */
