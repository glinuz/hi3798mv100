/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-02-28
 *
******************************************************************************/
#ifndef DEBUGH
#define DEBUGH
/******************************************************************************/


#ifdef __ASSERT
#  define ASSERT(_condition) ((_condition) ? (void) 0 : \
	printf("%s(%d): %s", __FILE__, __LINE__, #_condition))
#else
#  define ASSERT(_condition)
#endif /* __ASSERT */


#ifdef __PR_MSG
#  define PR_MSG(fmt, args...)  \
	printf("%s(%d): " fmt, __FILE__, __LINE__, ##args)
#else
#  define PR_MSG(fmt, args...)
#endif /* __PR_MSG */


#ifdef __PR_HEX
#  define PR_HEX(_ptr, _num) do { \
	int ix; \
	unsigned char *p = (unsigned char *)(_ptr); \
	for (ix = 0; ix < (_num); ix++) { \
		printf("%02X ", p[ix]); \
		if (!((ix + 1) % 16)) printf("\n"); \
	} \
} while (0)
#else
#  define PR_HEX(_ptr, _num)
#endif /* __PR_MSG */


#ifdef __PR_TIME
extern char *do_getstrtime(char buffer[20]);
#  define PR_TIME(_fmt, _args...) do {\
	char buffer[20]; \
	printf("%s(%d):[%s] " _fmt, \
	       __FILE__, __LINE__, do_getstrtime(buffer), ##_args); \
} while (0)
#else
#  define PR_TIME(_fmt, _args...)
#endif /* __PR_TIME */


/******************************************************************************/
#endif /* DEBUGH */
