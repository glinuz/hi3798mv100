#ifndef __FV_DBG_H__
#define __FV_DBG_H__

#include <stdlib.h>
#include <stdio.h>

//#define FV_DEBUG

#ifdef FV_DEBUG
#define fv_debug(arg...) \
	do { \
		printf(arg);\
	} while(0)
#else
#define fv_debug(arg...)
#endif

#define fv_error(arg...)\
	do { \
		printf(arg);\
	} while (0)

#endif
