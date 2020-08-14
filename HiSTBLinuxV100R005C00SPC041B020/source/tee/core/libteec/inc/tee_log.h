#ifndef _TEEC_LOG_H_
#define _TEEC_LOG_H_
#ifdef _ANDROID_PLATFORM_HISI
#include <android/log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "libteec"

#ifdef DEF_ENG
#define TEE_LOG_MASK		ANDROID_LOG_INFO
#else
#define TEE_LOG_MASK		ANDROID_LOG_WARN
#endif

#define tlogv(...)/*lint -save -e774*/ \
	do { \
	    if (ANDROID_LOG_VERBOSE == TEE_LOG_MASK) \
			__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__); \
	} while (0) /*lint -restore*/

#define tlogd(...) /*lint -save -e774*/ \
	do { \
		if (ANDROID_LOG_DEBUG >= TEE_LOG_MASK) \
			__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); \
	} while (0) /*lint -restore*/

#define tlogi(...) /*lint -save -e774*/ \
	do { \
	    if (ANDROID_LOG_INFO >= TEE_LOG_MASK) \
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__); \
	} while (0) /*lint -restore*/

#define tlogw(...) /*lint -save -e774*/ \
	do { \
	    if (ANDROID_LOG_WARN >= TEE_LOG_MASK) \
			__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__); \
	} while (0) /*lint -restore*/

#define tloge(...) /*lint -save -e774*/ \
	do { \
	    if (ANDROID_LOG_ERROR >= TEE_LOG_MASK) \
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__); \
	} while (0) /*lint -restore*/
#else
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "libteec"
#define LOG_VERBOSE 0
#define LOG_DEBUG 1
#define LOG_INFO 2
#define LOG_WARN 3
#define LOG_ERROR 4

#ifdef DEF_ENG
#define TEE_LOG_MASK		LOG_INFO
#else
#define TEE_LOG_MASK		LOG_WARN
#endif

#define tlogv(fmt, args...)/*lint -save -e774*/ \
	do { \
	    if (LOG_VERBOSE == TEE_LOG_MASK) \
			printf("[verbose] %s: " fmt, LOG_TAG, ## args); \
	} while (0) /*lint -restore*/

#define tlogd(fmt, args...) /*lint -save -e774*/ \
	do { \
		if (LOG_DEBUG >= TEE_LOG_MASK) \
			printf("[debug] %s: " fmt, LOG_TAG, ## args); \
	} while (0) /*lint -restore*/

#define tlogi(fmt, args...) /*lint -save -e774*/ \
	do { \
	    if (LOG_INFO >= TEE_LOG_MASK) \
			printf("[info] %s: " fmt, LOG_TAG, ## args); \
	} while (0) /*lint -restore*/

#define tlogw(fmt, args...) /*lint -save -e774*/ \
	do { \
	    if (LOG_WARN >= TEE_LOG_MASK) \
			printf("[warn] %s: " fmt, LOG_TAG, ## args); \
	} while (0) /*lint -restore*/

#define tloge(fmt, args...) /*lint -save -e774*/ \
	do { \
	    if (LOG_ERROR >= TEE_LOG_MASK) \
			printf("[error] %s: " fmt, LOG_TAG, ## args); \
	} while (0) /*lint -restore*/
#endif
#endif
