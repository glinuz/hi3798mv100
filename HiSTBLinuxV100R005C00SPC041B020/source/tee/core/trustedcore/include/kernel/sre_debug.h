#ifndef _SRE_DEBUG_H_
#define _SRE_DEBUG_H_

/* this switch config rtosck and framework can printf debug and erro info */

#ifdef DEBUG_SWITCH
#define uart_printf uart_printf_func
#else
#define uart_printf(fmt, ...)
#endif

/* config rtos debug info */
#define DEBUG_RTOSCK
#ifdef DEBUG_RTOSCK
#define DEBUG_RTOS_TAG "[debug]"
#define debug_rtos(fmt, args...) uart_printf("%s %s: " fmt "", DEBUG_RTOS_TAG, __FUNCTION__, ## args)
#else
#define debug_rtos(fmt, args...)
#endif

#define ERROR_RTOS_TAG "[error]"
#define rtos_error(fmt, args...) uart_printf("%s : " fmt "", ERROR_RTOS_TAG, ## args)

#endif
