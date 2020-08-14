/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong 2011-02-21
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
#ifndef _DEBUGH_
#define _DEBUGH_

/*****************************************************************************/

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

/*****************************************************************************/

#define DBG_OUT(fmt, arg...) \
	printk(_YELLOW "%s:%d: " fmt _NONE , __FILE__, __LINE__, ## arg)

#define PR_DBG(fmt, arg...) \
	pr_debug(_YELLOW fmt _NONE , ## arg);

#define ASSERT(_con, _fmt, _args...) if (!(_con)) { \
	printk(KERN_ERR "%s(%d): ASSERT(%s) " _fmt, __FILE__, __LINE__, #_con, ##_args); \
	dump_stack(); \
}

/******************************************************************************/

#endif /* _DEBUGH_ */

