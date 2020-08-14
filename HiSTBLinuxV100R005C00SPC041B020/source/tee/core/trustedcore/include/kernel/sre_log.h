#ifndef SRE_LOG_H_
#define SRE_LOG_H_

#include "sre_ticktimer.h"

extern void uart_printf_func(const char *fmt, ...);

struct snprintf_ctxt {
	char *next;
	int avail;
};

#define VERB_TAG		"[verb]"
#define DEBUG_TAG		"[debug]"
#define INFO_TAG		"[info]"
#define WARN_TAG		"[warn]"
#define ERROR_TAG		"[error]"

typedef enum {
	LOG_LEVEL_ERROR = 0,
	LOG_LEVEL_WARN = 1,
	LOG_LEVEL_INFO = 2,
	LOG_LEVEL_DEBUG = 3,
	LOG_LEVEL_VERBO = 4,
} LOG_LEVEL;

void printk(LOG_LEVEL log_level, const char *fmt, ...);

#define tlogv(fmt, args...) printk(LOG_LEVEL_VERBO, "%s %s:" fmt "", VERB_TAG, __func__, ## args)
#define tlogd(fmt, args...) printk(LOG_LEVEL_DEBUG, "%s %s:" fmt "", DEBUG_TAG, __func__, ## args)
#define tlogi(fmt, args...) printk(LOG_LEVEL_INFO, "%s %s:" fmt "", INFO_TAG, __func__, ## args)
#define tlogw(fmt, args...) printk(LOG_LEVEL_WARN, "%s %s:" fmt "", WARN_TAG, __func__, ## args)
#define tloge(fmt, args...) printk(LOG_LEVEL_ERROR, "%s %s:" fmt "", ERROR_TAG, __func__, ## args)

void test_klog_func(UINT32 level);


#endif
