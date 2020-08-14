/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: output.inl
* Description:
*             used by secureprintoutput_a.c and secureprintoutput_w.c to include.
*             This file provides a template function for ANSI and UNICODE compiling
*             by different type definition. The functions of securec_output_s or
*             securec_woutput_s  provides internal implementation for printf family
*             API, such as sprintf, swprintf_s.
* History:
*     1. Date: 2014/10/31
*        Author:  LiShunda 254400
*        Modification: use system sprintf API to format float number which can keep compatible
*                      this float string output
*     2. Date: 2014/11/21
*        Author:  LiShunda 254400
*        Modification: improve performance on sprintf_s serial function, modify the algorithm
*                      of data conversion from integer to string, and replace some function call
*                      with macro.
* see: http://www.cplusplus.com/reference/cstdio/printf/
********************************************************************************
*/

#ifndef OUTPUT_INL_2B263E9C_43D8_44BB_B17A_6D2033DECEE5
#define OUTPUT_INL_2B263E9C_43D8_44BB_B17A_6D2033DECEE5

#include <linux/kernel.h>
#include "secureprintoutput.h"

#define write_string write_string_a
#define write_char write_char_a
#define write_multi_char write_multi_char_a

#define DISABLE_N_FORMAT

#ifndef INT_MAX
#define INT_MAX (0x7FFFFFFF)
#endif

#ifndef _CVTBUFSIZE
#define _CVTBUFSIZE (309+40)       /* # of digits in max float point value */
#endif

#ifndef INT_MAX
#define INT_MAX       2147483647
#endif
#ifndef INT_MAX_DIV_TEN
#define INT_MAX_DIV_TEN       21474836
#endif

#ifndef MUL10
#define MUL10(x) ((((x) << 2) + (x)) << 1)
#endif

#define  MUL10_ADD_BEYOND_MAX(val,ch) ((val > INT_MAX_DIV_TEN))

#ifdef STACK_SIZE_LESS_THAN_1K
#define SECURE_FMT_STR_LEN (8)
#else
#define SECURE_FMT_STR_LEN (16)
#endif
#define  MASK_PCLINT_NO_OVERFLOW /*lint !e586 */

#define TCHAR char
#define _T

static int indirectSprintf(char *strDest, const char *format, ...)
{
	int ret = 0;
	va_list arglist;

	va_start(arglist, format);
	ret = vsprintf(strDest, format, arglist); /*lint !e668*/
	va_end(arglist);

	return ret;
}

/*
fast conversion for %d %i %u

unsigned fastDivu10(unsigned n) {
    unsigned q, r;
    q = (n >> 1) + (n >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q >> 3;
    r = n - (((q << 2) + q) << 1);
    return q + (r > 9);
}
*/


#define SAFE_WRITE_STR(src, txtLen, _stream, outChars)      \
    if (txtLen < 12 /* for mobile number length */) {       \
        for (ii = 0; ii < txtLen; ++ii) {                   \
            *((TCHAR*)(_stream->_ptr)) = *(src);            \
            _stream->_ptr += sizeof(TCHAR);                 \
            ++(src);                                        \
        }                                                   \
    }else {                                                 \
        memcpy(_stream->_ptr, src, txtLen * sizeof(TCHAR)); \
        _stream->_ptr += txtLen * sizeof(TCHAR);            \
    }                                                       \
    _stream->_cnt -= txtLen * sizeof(TCHAR);                \
    *(outChars) = *(outChars) + (txtLen);

#define SECUREC_SPECIAL(_val, Base)                                 \
    case Base:                                              \
    do {                                                    \
        *--text.sz = digits[_val % Base];                   \
    }while ((_val /= Base) /* != 0 */);                     \
    break

#define SAFE_WRITE_CHAR(_ch, _stream, outChars)             \
    *((TCHAR*)(_stream->_ptr)) = (TCHAR)_ch;                \
    _stream->_ptr += sizeof(TCHAR);                         \
    _stream->_cnt -= sizeof(TCHAR);                         \
    *(outChars) = *(outChars) + 1;

#define SAFE_PADDING(padChar, padLen, _stream, outChars)    \
    for (ii = 0; ii < padLen; ++ii) {                       \
        *((TCHAR*)(_stream->_ptr)) = (TCHAR)padChar;        \
        _stream->_ptr += sizeof(TCHAR);                     \
    }                                                       \
    _stream->_cnt -= padLen * sizeof(TCHAR);                \
    *(outChars) = *(outChars) + (padLen);



#define IS_REST_BUF_ENOUGH(needLen) ((int)(stream->_cnt - (int)needLen * sizeof(TCHAR))  > 0)

#ifdef _XXXUNICODE
int securec_woutput_s
#else
int securec_output_s
#endif
(
	SECUREC_XPRINTF_STREAM *stream,/*lint !e40 !e10 */
	const char *format,
	va_list argptr
)
{
	/*lint !e10 */
	/* literal string to print null ptr, define it on stack rather than const text area
	is to avoid gcc warning with pointing const text with variable */
	static char STR_NULL_STRING[] = "(null)";
	/* change to next line    wchar_t WSTR_NULL_STRING[] =  L"(null)";  */
	static wchar_t WSTR_NULL_STRING[] = {L'(', L'n', L'u', L'l', L'l', L')', L'\0'};

	char *heapBuf = NULL;
	union {
		char *sz;                   /* pointer text to be printed, not zero terminated */
		wchar_t *wz;
	} text;

	static const char _itoa_upper_digits[] = "0123456789ABCDEFX";
	static const char _itoa_lower_digits[] = "0123456789abcdefx";
	const char *digits = _itoa_upper_digits;
	int ii = 0;

	int hexAdd = 0;                 /* 0:lower x; 1: upper X */
	int flags = 0;
	unsigned int radix;
	int charsOut;                   /* characters written*/
	int fldWidth = 0;
	int precision = 0;
	int prefixLen = 0;
	int padding = 0;

	int textLen;                    /* length of the text*/
	int bufferSize = 0;               /* size of text.sz */

	int dynWidth =  0, dynPrecision =  0;
	int noOutput =  0;
	int bufferIsWide = 0;             /* flag for buffer contains wide chars */

	FMT_STATE state, laststate;              /* current state */
	CHARTYPE chclass;              /* current character class*/

	wchar_t wchar = 0;
	TCHAR prefix[2] = {0};
	TCHAR ch;                       /* currently read character*/
	union {
		char sz[BUFFERSIZE];
#ifdef _XXXUNICODE
		wchar_t wz[BUFFERSIZE];
#endif  /* _XXXUNICODE */
	} buffer;



	static const UINT8T securec__lookuptable_s[] = {
		/* ' ' */  0x06,
		/* '!' */  0x80,
		/* '"' */  0x80,
		/* '#' */  0x86,
		/* '$' */  0x80,
		/* '%' */  0x81,
		/* '&' */  0x80,
		/* ''' */  0x00,
		/* '(' */  0x00,
		/* ')' */  0x10,
		/* '*' */  0x03,
		/* '+' */  0x86,
		/* ',' */  0x80,
		/* '-' */  0x86,
		/* '.' */  0x82,
		/* '/' */  0x80,
		/* '0' */  0x14,
		/* '1' */  0x05,
		/* '2' */  0x05,
		/* '3' */  0x45,
		/* '4' */  0x45,
		/* '5' */  0x45,
		/* '6' */  0x85,
		/* '7' */  0x85,
		/* '8' */  0x85,
		/* '9' */  0x05,
		/* ':' */  0x00,
		/* ';' */  0x00,
		/* '<' */  0x30,
		/* '=' */  0x30,
		/* '>' */  0x80,
		/* '?' */  0x50,
		/* '@' */  0x80,
		/* 'A' */  0x80,
		/* 'B' */  0x00,
		/* 'C' */  0x08,
		/* 'D' */  0x00,
		/* 'E' */  0x28,
		/* 'F' */  0x28,
		/* 'G' */  0x38,
		/* 'H' */  0x50,
		/* 'I' */  0x57,
		/* 'J' */  0x80,
		/* 'K' */  0x00,
		/* 'L' */  0x07,
		/* 'M' */  0x00,
		/* 'N' */  0x37,
		/* 'O' */  0x30,
		/* 'P' */  0x30,
		/* 'Q' */  0x50,
		/* 'R' */  0x50,
		/* 'S' */  0x88,
		/* 'T' */  0x00,
		/* 'U' */  0x00,
		/* 'V' */  0x00,
		/* 'W' */  0x20,
		/* 'X' */  0x28,
		/* 'Y' */  0x20,
		/* 'Z' */  0x87,
		/* '[' */  0x80,
		/* '\' */  0x80,
		/* ']' */  0x00,
		/* '^' */  0x00,
		/* '_' */  0x00,
		/* '`' */  0x60,
		/* 'a' */  0x60,
		/* 'b' */  0x60,
		/* 'c' */  0x68,
		/* 'd' */  0x68,
		/* 'e' */  0x68,
		/* 'f' */  0x08,
		/* 'g' */  0x08,
		/* 'h' */  0x07,
		/* 'i' */  0x78,
		/* 'j' */  0x77,
		/* 'k' */  0x70,
		/* 'l' */  0x77,
		/* 'm' */  0x70,
		/* 'n' */  0x70,
		/* 'o' */  0x08,
		/* 'p' */  0x08,
		/* 'q' */  0x77,
		/* 'r' */  0x00,
		/* 's' */  0x08,
		/* 't' */  0x77,
		/* 'u' */  0x08,
		/* 'v' */  0x00,
		/* 'w' */  0x07,
		/* 'x' */  0x08,
		/* 'y' */  0x00,
		/* 'z' */  0x57
	};

	charsOut = 0;
	textLen = 0;
	laststate = state = STAT_NORMAL;               /* starting state */
	text.sz = NULL;

	/*loop each format character */
	/* remove format != NULL */
	while ((ch = *format++) != _T('\0') && charsOut >= 0) {
		laststate = state;
		chclass = FIND_CHAR_CLASS(securec__lookuptable_s, ch);
		state = FIND_NEXT_STATE(securec__lookuptable_s, chclass, laststate);

		switch (state) {
		case STAT_INVALID:
			return -1; /*input format is wrong, directly return*/
		case STAT_NORMAL:

NORMAL_STATE:

			/* normal state, write character */
#ifdef _XXXUNICODE
			bufferIsWide = 1;
#else  /* _XXXUNICODE */
			bufferIsWide =  0;
#endif  /* _XXXUNICODE */
			if (IS_REST_BUF_ENOUGH(1 /* only one char */)) {
				SAFE_WRITE_CHAR(ch, stream, &charsOut);
			} else {
				write_char(ch, stream, &charsOut);
			}

			continue;

		case STAT_PERCENT:
			/* set default values */
			prefixLen = fldWidth = 0;
			noOutput = 0;
			flags = 0;
			precision = -1;
			bufferIsWide = 0;
			break;

		case STAT_FLAG:
			/* set flag based on which flag character */
			switch (ch) {
			case _T('-'):
				flags |= FLAG_LEFT;
				break;
			case _T('+'):
				flags |= FLAG_SIGN;       /* force sign indicator */
				break;
			case _T(' '):
				flags |= FLAG_SIGN_SPACE;
				break;
			case _T('#'):
				flags |= FLAG_ALTERNATE;
				break;
			case _T('0'):
				flags |= FLAG_LEADZERO;   /* pad with leading zeros */
				break;
			default:
				break;
			}
			break;

		case STAT_WIDTH:
			/* update width value */
			if (ch == _T('*')) {
				/* get width*/
				fldWidth = va_arg(argptr, int); /*lint !e826 !e10 !e718 !e746*/
				if (fldWidth < 0) {
					flags |= FLAG_LEFT;
					fldWidth = -fldWidth;
				}
				dynWidth = 1;
			} else {
				if (laststate != STAT_WIDTH) {
					fldWidth = 0;
				}
				if (MUL10_ADD_BEYOND_MAX(fldWidth, ch)) {
					return -1;
				}
				fldWidth = MUL10(fldWidth) + (ch - _T('0'));
				dynWidth = 0;
			}
			break;

		case STAT_DOT:
			precision = 0;
			break;

		case STAT_PRECIS:
			/* update precison value */
			if (ch == _T('*')) {
				/* get precision from arg list */
				precision =  va_arg(argptr, int); /*lint !e826 !e10 */
				if (precision < 0) {
					precision = -1;
				}
				dynPrecision = 1;
			} else {
				/* add digit to current precision */
				if (MUL10_ADD_BEYOND_MAX(precision, ch)) {
					return -1;
				}
				precision = MUL10(precision)  + (ch - _T('0'));
				dynPrecision = 0;
			}
			break;

		case STAT_SIZE:
			/* read a size specifier, set the flags based on it */
			switch (ch) {
#ifdef COMPATIBLE_LINUX_FORMAT
			case _T('j'):
				flags |= FLAG_INTMAX;
				break;
#endif
			case _T('q'):
			case _T('L'):
				flags |= FLAG_LONGLONG | FLAG_LONG_DOUBLE;
				break;
			case _T('l'):
				if (*format == _T('l')) {
					++format;
					flags |= FLAG_LONGLONG;   /* long long */
				} else {
					flags |= FLAG_LONG;       /* long int or wchar_t */
				}
				break;
			case _T('t') :
				flags |= FLAG_PTRDIFF;
				break;
#ifdef COMPATIBLE_LINUX_FORMAT
			case _T('z') :
				flags |= FLAG_SIZE;
				break;
#endif
				/*lint -fallthrough */
			case _T('I'):
#ifdef SECUREC_ON_64BITS
				flags |= FLAG_I64;    /* 'I' => INT64T on 64bits systems */
#endif
				if ((*format == _T('6')) && (*(format + 1) == _T('4'))) {
					format += 2;
					flags |= FLAG_I64;    /* I64 => INT64T */
				} else if ((*format == _T('3')) && (*(format + 1) == _T('2'))) {
					format += 2;
					flags &= ~FLAG_I64;   /* I32 => __int32 */
				} else if ((*format == _T('d')) ||
					   (*format == _T('i')) ||
					   (*format == _T('o')) ||
					   (*format == _T('u')) ||
					   (*format == _T('x')) ||
					   (*format == _T('X'))) {
					/* no further doing. */
				} else {
					state = STAT_NORMAL;
					goto NORMAL_STATE; /*lint !e801*/
				}
				break;

			case _T('h'):
				if (*format == _T('h'))
					flags |= FLAG_CHAR;   /* char */
				else
					flags |= FLAG_SHORT;  /* short int */
				break;

			case _T('w'):
				flags |= FLAG_WIDECHAR;  /*  wide char */
				break;
			default:
				break;

			}
			break;

		case STAT_TYPE:

			switch (ch) {

			case _T('C'):   /* wide char */
				if (!(flags & (FLAG_SHORT | FLAG_LONG | FLAG_WIDECHAR))) {

#ifdef _XXXUNICODE
					flags |= FLAG_SHORT;
#else  /* _XXXUNICODE */
					flags |= FLAG_WIDECHAR;
#endif  /* _XXXUNICODE */
				}

				/* fall into 'c' case */
				/*lint -fallthrough */
			case _T('c'): {
#if (defined(COMPATIBLE_LINUX_FORMAT)) && !(defined(__hpux)) && !(defined(__SOLARIS))
				flags &= ~FLAG_LEADZERO;
#endif
				/* print a single character specified by int argument */
#ifdef _XXXUNICODE
				bufferIsWide =  1;
				wchar = (wchar_t)va_arg(argptr, int);  /*lint !e826 !e10*/
				if (flags & FLAG_SHORT) {
					/* format multibyte character */
					char tempchar[2];
					{
						tempchar[0] = (char)(wchar & 0x00ff);
						tempchar[1] = '\0';
					}

					if (mbtowc(buffer.wz, tempchar, (size_t)MB_CUR_MAX) < 0) {
						/* ignore if conversion was unsuccessful */
						noOutput = 1;
					}
				} else {
					buffer.wz[0] = wchar;
				}
				text.wz = buffer.wz;
				textLen = 1;    /* print just a single character */
#else  /* _XXXUNICODE */
				if (flags & (FLAG_LONG | FLAG_WIDECHAR)) {
					wchar = (wchar_t)va_arg(argptr, int);  /*lint !e826 !e10 */
					/* convert to multibyte character */

					//textLen = wctomb(buffer.sz, wchar);
					if (textLen < 0) {
						noOutput = 1;
					}
				} else {
					/* format multibyte character */
					unsigned short temp;
					temp = (unsigned short) va_arg(argptr, int); /*lint !e826 !e10 */
					{
						buffer.sz[0] = (char) temp;
						textLen = 1;
					}
				}
				text.sz = buffer.sz;
#endif  /* _XXXUNICODE */
			}
			break;

			case _T('Z'):
				flags |= FLAG_SIZE;
				break;

			case _T('S'):   /* wide char string */
#ifndef _XXXUNICODE
				if (!(flags & (FLAG_SHORT | FLAG_LONG | FLAG_WIDECHAR))) {
					flags |= FLAG_WIDECHAR;
				}
#else  /* _XXXUNICODE */
				if (!(flags & (FLAG_SHORT | FLAG_LONG | FLAG_WIDECHAR))) {
					flags |= FLAG_SHORT;
				}
#endif  /* _XXXUNICODE */
				/*lint -fallthrough */
			case _T('s'): {
				/* a string */

				int i;
				char *p;       /* temps */
				wchar_t *pwch;

#if (defined(COMPATIBLE_LINUX_FORMAT)) && (!defined(__UNIX))
				flags &= ~FLAG_LEADZERO;
#endif
				i = (precision == -1) ? INT_MAX : precision;
				text.sz = va_arg(argptr, char *); /*lint !e826 !e64 !e10 */

				/* scan for null upto i characters */
#ifdef _XXXUNICODE
#if defined(COMPATIBLE_LINUX_FORMAT)
				if (!(flags & FLAG_LONG)) {
					flags |= FLAG_SHORT;
				}
#endif
				if (flags & FLAG_SHORT) {
					if (text.sz == NULL) { /* NULL passed, use special string */
						text.sz = STR_NULL_STRING;
					}
					p = text.sz;
					for (textLen = 0; textLen < i && *p; textLen++) {
						++p;
					}
					/* textLen now contains length in multibyte chars */
				} else {
					if (text.wz == NULL) { /* NULL passed, use special string */
						text.wz = WSTR_NULL_STRING;
					}
					bufferIsWide = 1;
					pwch = text.wz;
					while (i-- && *pwch) {
						++pwch;
					}
					textLen = (int)(pwch - text.wz);      /*lint !e946*/ /* in wchar_ts */
					/* textLen now contains length in wide chars */
				}
#else  /* _XXXUNICODE */
				if (flags & (FLAG_LONG | FLAG_WIDECHAR)) {
					if (text.wz == NULL) { /* NULL passed, use special string */
						text.wz = WSTR_NULL_STRING;
					}
					bufferIsWide = 1;
					pwch = text.wz;
					while (i-- && *pwch) {
						++pwch;
					}
					textLen = (int)(pwch - text.wz);  /*lint !e946*/
				} else {
					if (text.sz == NULL) { /* meet NULL, use special string */
						text.sz = STR_NULL_STRING;
					}
					p = text.sz;
					if (INT_MAX == i) {
						/* precision NOT assigned */
						while (*p) {
							++p;
						}
					} else {
						/* precision assigned */
						while (i-- && *p) {
							++p;
						}
					}
					textLen = (int)(p - text.sz);  /*lint !e946*/  /* length of the string */
				}

#endif  /* _XXXUNICODE */
			}
			break;


			case _T('n'): {
				/* write count of characters*/

				/* if %n is disabled, we skip an arg and print 'n' */
#ifdef DISABLE_N_FORMAT
				return -1;
#else
				void *p;        /* temp */
				p =  va_arg(argptr, void *); /* LSD util_get_ptr_arg(&argptr);*/

				/* store chars out into short/long/int depending on flags */
#ifdef SECUREC_ON_64BITS
				if (flags & FLAG_LONG) {
					*(long *)p = charsOut;
				} else
#endif  /* SECUREC_ON_64BITS */

					if (flags & FLAG_SHORT) {
						*(short *)p = (short) charsOut;
					} else {
						*(int *)p = charsOut;
					}

				noOutput = 1;              /* force no output */
				break;
#endif
			}

			case _T('E'):
			case _T('F'):
			case _T('G'):
			case _T('A'):
				ch += _T('a') - _T('A');    /* convert format char to lower */
				/*lint -fallthrough */
			case _T('e'):
			case _T('f'):
			case _T('g'):
			case _T('a'): {
				return -1;
#if 0
				/* floating point conversion */
				text.sz = buffer.sz;        /* output buffer for float string with default size*/
				bufferSize = BUFFERSIZE;

				/* compute the precision value */
				if (precision < 0) {
					precision = 6;
				} else if (precision == 0 && ch == _T('g')) {
					precision = 1;
				}

				/* calc buffer size to store long double value */
				if (flags & FLAG_LONG_DOUBLE) {
					if (precision > (INT_MAX - CVTBUFSIZE_LB)) {
						noOutput = 1;
						break;
					}
					bufferSize =  CVTBUFSIZE_LB +  precision ;
				} else {
					if (precision > (INT_MAX - _CVTBUFSIZE)) {
						noOutput = 1;
						break;
					}
					bufferSize = _CVTBUFSIZE + precision ;
				}
				if (fldWidth > bufferSize) {
					bufferSize = fldWidth ;
				}

				if (bufferSize >= BUFFERSIZE) {
					/* the current vlaue of BUFFERSIZE could NOT store the formatted float string */
					heapBuf = (char *)SECC_MALLOC(((size_t)bufferSize + (size_t)2));/*lint !e586 */ /* size include '+' and '\0'*/
					if (heapBuf != NULL) {
						text.sz = heapBuf;
					} else {
						noOutput = 1;
						break;
					}
				}


				{
					/* add following code to call system sprintf API for float number */
					const TCHAR *pFltFmt = format - 2; /* point to the position before 'f' or 'g' */
					int k = 0, fltFmtStrLen;
					TCHAR fltFmtStr[SECURE_FMT_STR_LEN];
					TCHAR *pFltFmtStr = fltFmtStr;

					while (_T('%') != *pFltFmt) { /* must meet '%' */
						--pFltFmt;
					}
					fltFmtStrLen = format - pFltFmt + 1; /* with ending terminator */
					if (fltFmtStrLen > SECURE_FMT_STR_LEN) {
						/* if SECURE_FMT_STR_LEN is NOT enough, alloc a new buffer */
						pFltFmtStr = (TCHAR *)SECC_MALLOC(fltFmtStrLen * sizeof(TCHAR));
						if (NULL == pFltFmtStr) {
							noOutput = 1;
							break;
						}
					}
					for (; k < fltFmtStrLen - 1; ++k) {
						pFltFmtStr[k] = *(pFltFmt + k); /* copy the format string */
					}
					pFltFmtStr[k] = _T('\0');

#ifdef _XXXUNICODE
					for (k = 0; k < fltFmtStrLen - 1; ++k) {
						/* convert wchar to char */
						*((char *)pFltFmtStr + k) = (char) * (pFltFmt + k);
					}
					*((char *)pFltFmtStr + k) = '\0';
#endif

#ifdef COMPATIBLE_LINUX_FORMAT
					if (flags & FLAG_LONG_DOUBLE) {
						long double tmp;
						tmp = va_arg(argptr, long double); /*lint !e826 !e10 */
						/* call system sprintf to format float value */
						if (dynWidth && dynPrecision) {
							textLen = indirectSprintf(text.sz, (char *)pFltFmtStr, fldWidth, precision, tmp);
							MASK_PCLINT_NO_OVERFLOW
						} else if (dynWidth) {
							textLen = indirectSprintf(text.sz, (char *)pFltFmtStr, fldWidth, tmp);
							MASK_PCLINT_NO_OVERFLOW
						} else if (dynPrecision) {
							textLen = indirectSprintf(text.sz, (char *)pFltFmtStr, precision, tmp);
							MASK_PCLINT_NO_OVERFLOW
						} else {
							textLen = indirectSprintf(text.sz, (char *)pFltFmtStr, tmp);
							MASK_PCLINT_NO_OVERFLOW
						}
					} else
#endif
					{
						double tmp;
						tmp = va_arg(argptr, double); /*lint !e826 !e10 */
						if (dynWidth && dynPrecision) {
							textLen = indirectSprintf(text.sz, (char *)pFltFmtStr, fldWidth, precision, tmp);
							MASK_PCLINT_NO_OVERFLOW
						} else if (dynWidth) {
							textLen = indirectSprintf(text.sz, (char *)pFltFmtStr, fldWidth, tmp);
							MASK_PCLINT_NO_OVERFLOW
						} else if (dynPrecision) {
							textLen = indirectSprintf(text.sz, (char *)pFltFmtStr, precision, tmp);
							MASK_PCLINT_NO_OVERFLOW
						} else {
							textLen = indirectSprintf(text.sz, (char *)pFltFmtStr, tmp);
							MASK_PCLINT_NO_OVERFLOW
						}
					}

					if (fltFmtStrLen > SECURE_FMT_STR_LEN) {
						/* if buffer is alloced on heap, free it */
						SECC_FREE(pFltFmtStr); /*lint !e673*/
					}
					if (textLen > bufferSize) {
						/* bufferSize is large enough, this should NOT occur */
						noOutput = 1;
						break;
					}

					padding = 0; /* no padding*/
					flags = 0;   /* clear all internal flags */
					goto FILL_STRING_BUFFER; /*lint !e801*/
				}
#endif
			}
			case _T('d'):
			case _T('i'):
				/* signed decimal output */
				flags |= FLAG_SIGNED;
				radix = 10;
				goto COMMON_INT; /*lint !e801*/

			case _T('u'):
				radix = 10;
				goto COMMON_INT;  /*lint !e801*/

			case _T('p'):
				/* print a pointer*/
#if defined(COMPATIBLE_WIN_FORMAT)
				flags &= ~FLAG_LEADZERO;
#else
				flags |=  FLAG_POINTER;
#endif
#if (defined(COMPATIBLE_LINUX_FORMAT) || defined(SECUREC_VXWORKS_PLATFORM)) && (!defined(__UNIX))
#if defined(SECUREC_VXWORKS_PLATFORM)
				precision = 1;
#else
				precision = 0;
#endif
				flags |=  FLAG_ALTERNATE;            /* "0x" is not default prefix in UNIX */
				hexAdd = 0;
#ifdef SECUREC_ON_64BITS
				flags |= FLAG_I64;                    /* converting an int64 */
#else
				flags |= FLAG_LONG;                   /* converting a long */
#endif
				goto COMMON_HEX; /*lint !e801 */
#else
#if defined(_AIX) || defined(__SOLARIS)
				precision = 1;
#else
				precision = 2 * sizeof(void *);
#endif
#ifdef SECUREC_ON_64BITS
				flags |= FLAG_I64;                    /* converting an int64 */
#else
				flags |= FLAG_LONG;                   /* converting a long */
#endif
#endif

#if defined(__UNIX)
				hexAdd = 0;
				goto COMMON_HEX;  /*lint !e801*/
#endif

				/*lint -fallthrough */
			case _T('X'):
				/* unsigned upper hex output */
				hexAdd = 1;
				goto COMMON_HEX;  /*lint !e801*/

			case _T('x'):
				/* unsigned lower hex output */
				hexAdd = 0;     /* set hexAdd for lowercase hex */
				/* DROP THROUGH TO COMMON_HEX */

COMMON_HEX:
				radix = 16;
				digits = hexAdd == 0 ? _itoa_lower_digits : _itoa_upper_digits ;
				if (flags & FLAG_ALTERNATE) {
					/* alternate form means '0x' prefix */
					prefix[0] = _T('0');
					prefix[1] = (TCHAR)(digits[16]);  /* 'x' or 'X' */ /*lint !e571*/

#if (defined(COMPATIBLE_LINUX_FORMAT) || defined(SECUREC_VXWORKS_PLATFORM))
					if ('p' == ch) {
						prefix[1] =  _T('x');
					}
#endif
#if defined(_AIX) || defined(__SOLARIS)
					if ('p' == ch) {
						prefixLen = 0;
					} else {
						prefixLen = 2;
					}
#else
					prefixLen = 2;
#endif

				}
				goto COMMON_INT;  /*lint !e801*/

			case _T('o'):
				/* unsigned octal output */
				radix = 8;
				if (flags & FLAG_ALTERNATE) {
					/* alternate form means force a leading 0 */
					flags |= FLAG_FORCE_OCTAL;
				}
				/* DROP THROUGH to COMMON_INT */

COMMON_INT: {

					UINT64T number = 0; /* number to convert */
#if defined(SECUREC_VXWORKS_VERSION_5_4)
					UINT32T digit = 0;   /* ascii value of digit */
#endif
					INT64T l;           /* temp long value */
					unsigned char tch;
#if defined(SECUREC_VXWORKS_VERSION_5_4)
					UINT32T puiQuotientHigh = 0;
					UINT32T puiQuotientLow = 0;
#endif

					/* read argument into variable l*/
					if (flags & FLAG_I64) {
						l = va_arg(argptr, INT64T); /*lint !e826 !e530 !e516 !e78 */
					} else if (flags & FLAG_LONGLONG) {
						l = va_arg(argptr, INT64T); /*lint !e826 !e516 !e78 */
					}

					else

#ifdef SECUREC_ON_64BITS
						if (flags & FLAG_LONG) {
							l = va_arg(argptr, long);
						} else
#endif  /* SECUREC_ON_64BITS */
							if (flags & FLAG_CHAR) {
								if (flags & FLAG_SIGNED) {
									l = (char) va_arg(argptr, int); /* sign extend */ /*lint !e826 !e10 */
									if (l >= 128) { /* on some platform, char is always unsigned */
										flags |= FLAG_NEGATIVE;
										tch = (char)~l;
										l = tch + 1;
									}
								} else {
									l = (unsigned char) va_arg(argptr, int); /* zero-extend*/ /*lint !e826 !e10 */
								}

							} else if (flags & FLAG_SHORT) {
								if (flags & FLAG_SIGNED) {
									l = (short) va_arg(argptr, int); /* sign extend */ /*lint !e826 !e10 */
								} else {
									l = (unsigned short) va_arg(argptr, int); /* zero-extend*/ /*lint !e826 !e10 */
								}

							}
#ifdef COMPATIBLE_LINUX_FORMAT
							else if (flags & FLAG_PTRDIFF) {
								l = (ptrdiff_t) va_arg(argptr, ptrdiff_t);      /* sign extend */ /*lint !e826 !e10 !e78 !e530 */
							} else if (flags & FLAG_SIZE) {
								if (flags & FLAG_SIGNED) {
									if (sizeof(size_t) == sizeof(long)) { /*lint !e506*/
										l = va_arg(argptr, long);               /* sign extend */ /*lint !e826 !e10 */
									} else if (sizeof(size_t) == sizeof(long long)) { /*lint !e506*/
										l = va_arg(argptr, long long);          /* sign extend */ /*lint !e826 !e10 */
									} else {
										l = va_arg(argptr, int);                /* sign extend */ /*lint !e826 !e10 */
									}
								} else {
									l = (size_t) va_arg(argptr, size_t);        /* sign extend */ /*lint !e826 !e10 !e78 !e516 !e530 */
								}
							} else if (flags & FLAG_INTMAX) {
								if (flags & FLAG_SIGNED) {
									l = va_arg(argptr, INT64T);               /* sign extend */ /*lint !e826 !e10 !e78 !e516 !e530 */
								} else {
									l = (UINT64T)va_arg(argptr, UINT64T);    /* sign extend */ /*lint !e826 !e10 !e78 !e516 !e571 */
								}
							}
#endif
							else {
								if (flags & FLAG_SIGNED) {
									l = va_arg(argptr, int);  /*lint !e826 !e10 */
								} else {
									l = (unsigned int) va_arg(argptr, int);  /* zero-extend*/ /*lint !e826 !e10 */
								}

							}

					/* check for negative; copy into number */
					if ((flags & FLAG_SIGNED) && l < 0) { /*lint !e539 */
						number = -l;  /*lint !e732*/
						flags |= FLAG_NEGATIVE;
					} else {
						number = l; /*lint !e732*/
					}
#ifdef COMPATIBLE_LINUX_FORMAT
					if ((flags & FLAG_INTMAX) == 0)
#endif
					{
						if (((flags & FLAG_I64) == 0) && ((flags & FLAG_LONGLONG) == 0)) {
#ifdef SECUREC_ON_64BITS
							if (((flags & FLAG_LONG) == 0) && ((flags & FLAG_PTRDIFF) == 0) && ((flags & FLAG_SIZE) == 0))
#endif
								number &= 0xffffffff;
						}
					}

					/* check precision value for default */
					if (precision < 0) {
						precision = 1;  /* default precision */
					} else {
#if defined(COMPATIBLE_WIN_FORMAT)
						flags &= ~FLAG_LEADZERO;
#else
						if (!(flags & FLAG_POINTER)) {
							flags &= ~FLAG_LEADZERO;
						}
#endif
						if (precision > MAXPRECISION) {
							precision = MAXPRECISION;
						}
					}

					/* Check if data is 0; if so, turn off hex prefix,if 'p',add 0x prefix,else not add prefix */

					if (number == 0) {
#if !(defined(SECUREC_VXWORKS_PLATFORM)||defined(__hpux))
						prefixLen = 0;
#else
						if (('p' == ch) && (flags & FLAG_ALTERNATE))
							prefixLen = 2;
						else
							prefixLen = 0;
#endif
					}

					/*  Convert data to ASCII */

					text.sz = &buffer.sz[BUFFERSIZE - 1];

					if (number > 0) {
#ifdef SECUREC_ON_64BITS
						switch (radix) {
							/* the compiler will optimize each one */
							SECUREC_SPECIAL(number, 10);
							SECUREC_SPECIAL(number, 16);
							SECUREC_SPECIAL(number, 8);
						}
#else /* for 32 bits system */
						if (number <= 0xFFFFFFFFUL) {
							/* in most case, the value to be converted is small value */
							UINT32T n32Tmp = (UINT32T)number;
							switch (radix) {
								SECUREC_SPECIAL(n32Tmp, 16);
								SECUREC_SPECIAL(n32Tmp, 8);
#ifdef _AIX
								/* the compiler will optimize div 10 */
								SECUREC_SPECIAL(n32Tmp, 10);
#else
							case 10: {
								/* fast div 10 */
								UINT32T q, r;
								do {
									*--text.sz = digits[n32Tmp % 10];
									q = (n32Tmp >> 1) + (n32Tmp >> 2);
									q = q + (q >> 4);
									q = q + (q >> 8);
									q = q + (q >> 16);
									q = q >> 3;
									r = n32Tmp - (((q << 2) + q) << 1);
									n32Tmp = (r > 9) ? (q + 1) : q;
								} while (n32Tmp /* != 0 */);
							}
#endif
							} /* end switch */
						} else {
							/* the value to be converted is greater than 4G */
#if defined(SECUREC_VXWORKS_VERSION_5_4)
							do {
								if (0 != U64Div32((UINT32T)((number >> 16) >> 16),
										  (UINT32T)number,
										  radix,
										  &puiQuotientHigh,
										  &puiQuotientLow,
										  &digit)) {
									noOutput = 1;
									break;
								}
								*--text.sz = digits[digit];

								number = (UINT64T)puiQuotientHigh;
								number = (number << 32) + puiQuotientLow;
							} while (number /* != 0 */);
#else
#ifndef CONFIG_DEVCHIP_PLATFORM
							switch (radix) {
								/* the compiler will optimize div 10 */
								SECUREC_SPECIAL(number, 10);
								SECUREC_SPECIAL(number, 16);
								SECUREC_SPECIAL(number, 8);
							}
#endif
#endif
						}
#endif
					} /* END if (number > 0)*/
					textLen = (int)((char *)&buffer.sz[BUFFERSIZE - 1] - text.sz); /*lint !e946*/ /* compute length of number */
					if (precision > textLen) {
						for (ii = 0; ii < precision - textLen; ++ii) {
							*--text.sz = '0';
						}
						textLen = precision;
					}

					/* Force a leading zero if FORCEOCTAL flag set */
					if ((flags & FLAG_FORCE_OCTAL) && (textLen == 0 || text.sz[0] != '0')) {
						*--text.sz = '0';
						++textLen;      /* add a zero */
					}
				}
				break;
			default:
				break;
			}

			if (!noOutput) {
				if (flags & FLAG_SIGNED) {
					if (flags & FLAG_NEGATIVE) {
						/* prefix is a '-' */
						prefix[0] = _T('-');
						prefixLen = 1;
					} else if (flags & FLAG_SIGN) {
						/* prefix is '+' */
						prefix[0] = _T('+');
						prefixLen = 1;
					} else if (flags & FLAG_SIGN_SPACE) {
						/* prefix is ' ' */
						prefix[0] = _T(' ');
						prefixLen = 1;
					}
				}

#if defined(COMPATIBLE_LINUX_FORMAT) && (!defined(__UNIX))
				if ((flags & FLAG_POINTER) && (0 == textLen)) {
					flags &= ~FLAG_LEADZERO;
					text.sz = &buffer.sz[BUFFERSIZE - 1];
					*text.sz-- = '\0';
					*text.sz-- = ')';
					*text.sz-- = 'l';
					*text.sz-- = 'i';
					*text.sz-- = 'n';
					*text.sz = '(';
					textLen = 5;
				}
#endif

				/* calculate amount of padding */
				padding = (fldWidth - textLen) - prefixLen;

				/* put out the padding, prefix, and text, in the correct order */

				if (!(flags & (FLAG_LEFT | FLAG_LEADZERO)) && padding > 0) {
					/* pad on left with blanks */
					if (IS_REST_BUF_ENOUGH(padding)) {
						SAFE_PADDING(_T(' '), padding, stream, &charsOut);
					} else {
						write_multi_char(_T(' '), padding, stream, &charsOut);
					}
				}

				/* write prefix */
				if (prefixLen > 0) {
					TCHAR *pPrefix = prefix;
					if (IS_REST_BUF_ENOUGH(prefixLen)) {
						SAFE_WRITE_STR(pPrefix, prefixLen, stream, &charsOut); /*lint !e670 !e534*/
					} else {
						write_string(prefix, prefixLen, stream, &charsOut);
					}
				}

				if ((flags & FLAG_LEADZERO) && !(flags & FLAG_LEFT) && padding > 0) {
					/* write leading zeros */
					if (IS_REST_BUF_ENOUGH(padding)) {
						SAFE_PADDING(_T('0'), padding, stream, &charsOut);
					} else {
						write_multi_char(_T('0'), padding, stream, &charsOut);
					}
				}

FILL_STRING_BUFFER:
				/* write text */
#ifndef _XXXUNICODE
				if (bufferIsWide && (textLen > 0)) {
					wchar_t *p;
					int retval;
					int count;
					/* int e = 0;*/
					char L_buffer[MB_LEN_MAX + 1];

					p = text.wz;
					count = textLen;
					while (count--) {

						//retval = wctomb(L_buffer, *p++);
						if (retval <= 0) {
							charsOut = -1;
							break;
						}
						write_string(L_buffer, retval, stream, &charsOut);
					}
				} else {
					/* coverity[overrun-local] */
					if (IS_REST_BUF_ENOUGH(textLen)) {
						SAFE_WRITE_STR(text.sz, textLen, stream, &charsOut); /*lint !e534*/
					} else {
						write_string(text.sz, textLen, stream, &charsOut);
					}
				}
#else  /* _XXXUNICODE */
				if (!bufferIsWide && textLen > 0) {
					char *p;
					int retval, count;

					p = text.sz;
					count = textLen;
					while (count > 0) {
						/* coverity[overrun-buffer-val] */
						retval = mbtowc(&wchar, p, (size_t)MB_CUR_MAX);
						if (retval <= 0) {
							charsOut = -1;
							break;
						}
						write_char(wchar, stream, &charsOut);
						p += retval;
						count -= retval;
					}
				} else {
					if (IS_REST_BUF_ENOUGH(textLen)) {
						SAFE_WRITE_STR(text.wz, textLen, stream, &charsOut); /*lint !e534*/
					} else {
						write_string(text.wz, textLen, stream, &charsOut);
					}
				}
#endif  /* _XXXUNICODE */

				if (charsOut >= 0 && (flags & FLAG_LEFT) && padding > 0) {
					/* pad on right with blanks */
					if (IS_REST_BUF_ENOUGH(padding)) {
						SAFE_PADDING(_T(' '), padding, stream, &charsOut);
					} else {
						write_multi_char(_T(' '), padding, stream, &charsOut);
					}
				}

				/* we're done! */
			}
			if (heapBuf != NULL) {
				SECC_FREE(heapBuf);/*lint !e586 !e516*/
				heapBuf = NULL;
			}
			break;
		}
	}

	if (state != STAT_NORMAL && state != STAT_TYPE) {
		return -1;
	}

	return charsOut;        /* the number of characters written */
}
#endif /*OUTPUT_INL_2B263E9C_43D8_44BB_B17A_6D2033DECEE5*/


