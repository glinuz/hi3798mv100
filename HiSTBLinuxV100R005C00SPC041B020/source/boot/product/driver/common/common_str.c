#ifndef HI_MINIBOOT_SUPPORT
#include  "linux/string.h"
#else
#include <string.h>
#endif

#ifndef NULL
#define NULL 0
#endif
#define _U      0x01    /* upper */
#define _L      0x02    /* lower */
#define _D      0x04    /* digit */
#define _C      0x08    /* cntrl */
#define _P      0x10    /* punct */
#define _S      0x20    /* white space (space/lf/tab) */
#define _X      0x40    /* hex digit */
#define _SP     0x80    /* hard space (0x20) */


static unsigned char _ctype[] = {
	_C,_C,_C,_C,_C,_C,_C,_C,                        /* 0-7 */
	_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,         /* 8-15 */
	_C,_C,_C,_C,_C,_C,_C,_C,                        /* 16-23 */
	_C,_C,_C,_C,_C,_C,_C,_C,                        /* 24-31 */
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,                    /* 32-39 */
	_P,_P,_P,_P,_P,_P,_P,_P,                        /* 40-47 */
	_D,_D,_D,_D,_D,_D,_D,_D,                        /* 48-55 */
	_D,_D,_P,_P,_P,_P,_P,_P,                        /* 56-63 */
	_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,      /* 64-71 */
	_U,_U,_U,_U,_U,_U,_U,_U,                        /* 72-79 */
	_U,_U,_U,_U,_U,_U,_U,_U,                        /* 80-87 */
	_U,_U,_U,_P,_P,_P,_P,_P,                        /* 88-95 */
	_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,      /* 96-103 */
	_L,_L,_L,_L,_L,_L,_L,_L,                        /* 104-111 */
	_L,_L,_L,_L,_L,_L,_L,_L,                        /* 112-119 */
	_L,_L,_L,_P,_P,_P,_P,_C,                        /* 120-127 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                /* 128-143 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                /* 144-159 */
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
	_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
	_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
	_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
	_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
	_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};      /* 240-255 */

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#undef isalnum
#define isalnum(c)      ((__ismask(c)&(_U|_L|_D)) != 0)
#undef isalpha
#define isalpha(c)      ((__ismask(c)&(_U|_L)) != 0)
#undef iscntrl
#define iscntrl(c)      ((__ismask(c)&(_C)) != 0)
#undef isdigit
#define isdigit(c)      ((__ismask(c)&(_D)) != 0)
#undef isgraph
#define isgraph(c)      ((__ismask(c)&(_P|_U|_L|_D)) != 0)
#undef islower
#define islower(c)      ((__ismask(c)&(_L)) != 0)
#undef isprint
#define isprint(c)      ((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#undef ispunct
#define ispunct(c)      ((__ismask(c)&(_P)) != 0)
#undef isspace
#define isspace(c)      ((__ismask(c)&(_S)) != 0)
#undef isupper
#define isupper(c)      ((__ismask(c)&(_U)) != 0)
#undef isxdigit
#define isxdigit(c)     ((__ismask(c)&(_D|_X)) != 0)
#undef isascii
#define isascii(c) (((unsigned char)(c))<=0x7f)
#undef toascii
#define toascii(c) (((unsigned char)(c))&0x7f)
inline unsigned char __tolower(unsigned char c)
{
	        if (isupper(c))
			                c -= 'A'-'a';
		        return c;
}

static inline unsigned char __toupper(unsigned char c)
{
	        if (islower(c))
			                c -= 'a'-'A';
		        return c;
}
#undef tolower
#define tolower(c) __tolower(c)
#undef toupper
#define toupper(c) __toupper(c)

/*********************************************************************/

static char *strtok1 = NULL;

unsigned int strspn_2(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	unsigned int count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}

	return count;
}


char * strpbrk_2(const char * cs,const char * ct)
{
	const char *sc1,*sc2;

	for( sc1 = cs; *sc1 != '\0'; ++sc1) {
		for( sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *) sc1;
		}
	}
	return NULL;
}


char * strtok_2(char * s,const char * ct)
{
	char *sbegin, *send;

	sbegin  = s ? s : strtok1;
	if (!sbegin) {
		return NULL;
	}
	sbegin += strspn_2(sbegin,ct);
	if (*sbegin == '\0') {
		strtok1 = NULL;
		return( NULL );
	}
	send = strpbrk_2( sbegin, ct);
	if (send && *send != '\0')
		*send++ = '\0';
	strtok1 = send;
	return (sbegin);
}


unsigned long simple_strtoul_2(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

#if 0
unsigned int strlen_2(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

int memcmp_2(const void * cs,const void * ct,size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}
#endif

char * strstr_2(const char * s1,const char * s2)
{
	int l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *) s1;
	l1 = strlen(s1);
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1,s2,l2))
			return (char *) s1;
		s1++;
	}
	return NULL;
}


#if 0
char * strcpy_2(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}
#endif

#if 0
char * strncpy_2(char * dest,const char *src, unsigned int count)
{
	char *tmp = dest;
	while (count-- )&& (*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}
#else
char * strncpy_2(char * dest,const char *src, unsigned int count)
{
	char *tmp = dest;
	while (count-- )
	{
		*dest = *src;
		if(*src == '\0')
		{
			break;
		}
		dest++;
		src++;
	}
	return tmp;
}
#endif

