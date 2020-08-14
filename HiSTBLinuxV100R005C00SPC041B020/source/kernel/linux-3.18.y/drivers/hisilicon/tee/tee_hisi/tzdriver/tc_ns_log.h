#ifndef TC_NS_LOG_H_
#define TC_NS_LOG_H_

#include <linux/printk.h>

enum {
	TZ_DEBUG_VERBOSE = 0,
	TZ_DEBUG_DEBUG,
	TZ_DEBUG_INFO,
	TZ_DEBUG_WARN,
	TZ_DEBUG_ERROR,
};

#ifdef DEF_ENG
#define TEE_LOG_MASK TZ_DEBUG_INFO
#else
#define TEE_LOG_MASK TZ_DEBUG_WARN
#endif

#ifdef CONFIG_DEVCHIP_PLATFORM
#define tlogv(fmt, args...) /*lint -save -e774*/ \
do { \
	if (TZ_DEBUG_VERBOSE >= TEE_LOG_MASK) \
		pr_info("%s: " fmt, __func__, ## args); \
} while (0) /*lint -restore*/
#else
#define tlogv(fmt, args...) /*lint -save -e774*/ \
do { \
	if (TZ_DEBUG_VERBOSE >= TEE_LOG_MASK) \
		pr_info("(%i, %s)%s: " fmt, current->pid, current->comm,  __func__, ## args); \
} while (0) /*lint -restore*/
#endif

#ifdef CONFIG_DEVCHIP_PLATFORM
#define tlogd(fmt, args...) /*lint -save -e774*/ \
do { \
	if (TZ_DEBUG_DEBUG >= TEE_LOG_MASK) \
		pr_info("%s: " fmt, __func__, ## args); \
} while (0) /*lint -restore*/
#else
#define tlogd(fmt, args...) /*lint -save -e774*/ \
do { \
	if (TZ_DEBUG_DEBUG >= TEE_LOG_MASK) \
		pr_info("(%i, %s)%s: " fmt, current->pid, current->comm,  __func__, ## args); \
} while (0) /*lint -restore*/
#endif

#ifdef CONFIG_DEVCHIP_PLATFORM
#define tlogi(fmt, args...) /*lint -save -e774*/ \
do { \
	if (TZ_DEBUG_INFO >= TEE_LOG_MASK) \
		pr_info("%s: " fmt, __func__, ## args); \
} while (0) /*lint -restore*/
#else
#define tlogi(fmt, args...) /*lint -save -e774*/ \
do { \
	if (TZ_DEBUG_INFO >= TEE_LOG_MASK) \
		pr_info("(%i, %s)%s: " fmt, current->pid, current->comm,  __func__, ## args); \
} while (0) /*lint -restore*/
#endif

#ifdef CONFIG_DEVCHIP_PLATFORM
#define tlogw(fmt, args...) /*lint -save -e774*/ \
do { \
	if (TZ_DEBUG_WARN >= TEE_LOG_MASK) \
		pr_warn("%s: " fmt, __func__, ## args);\
} while (0) /*lint -restore*/
#else
#define tlogw(fmt, args...) /*lint -save -e774*/ \
do { \
	if (TZ_DEBUG_WARN >= TEE_LOG_MASK) \
		pr_warn("(%i, %s)%s: " fmt, current->pid, current->comm,  __func__, ## args);\
} while (0) /*lint -restore*/
#endif

#ifdef CONFIG_DEVCHIP_PLATFORM
#define tloge(fmt, args...) \
		pr_err("%s: " fmt, __func__, ## args)
#else
#define tloge(fmt, args...) \
		pr_err("(%i, %s)%s: " fmt, current->pid, current->comm,  __func__, ## args)

#endif
#endif
