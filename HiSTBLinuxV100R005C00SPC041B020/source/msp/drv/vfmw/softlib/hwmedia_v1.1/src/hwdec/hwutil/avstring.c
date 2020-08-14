/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCE+3qMb8fKw2oL5XKa2bTka3uZietSfYPZEXmx
wU0iQIcDWnSIOX13pLIOqRPo4dCx1W37Jro0bWrgb9JSCMMReyZ4AB1qhey9eWlBxblGO3zV
uZF3Zd+bgOjn40ONDygaVWWZGw/PpdM82AQSgluygePbk82oQrnk0Pjtc4modg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/*
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 * Copyright (c) 2007 Mans Rullgard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "config.h"
#include <stdarg.h>
#include <stdio.h>
#include "avstring.h"

#ifndef __KERNEL__
#include <string.h>
#include <ctype.h>
#endif

#if defined(_MSC_VER)
#pragma  warning(disable:4996)
#endif

int av_strstart(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && *pfx == *str)
    {
        pfx++;
        str++;
    }

    if (!*pfx && ptr)
    { *ptr = str; }

    return !*pfx;
}

int av_stristart(const char *str, const char *pfx, const char **ptr)
{
    while (*pfx && toupper((unsigned)*pfx) == toupper((unsigned)*str))
    {
        pfx++;
        str++;
    }

    if (!*pfx && ptr)
    { *ptr = str; }

    return !*pfx;
}

size_t av_strlcpy(char *dst, const char *src, size_t size)
{
    size_t len = 0;

    while (++len < size && *src)
    { *dst++ = *src++; }

    if (len <= size)
    { *dst = 0; }

    return len + strlen(src) - 1;
}

size_t av_strlcat(char *dst, const char *src, size_t size)
{
    size_t len = strlen(dst);

    if (size <= len + 1)
    { return len + strlen(src); }

    return len + av_strlcpy(dst + len, src, size - len);
}

#ifndef __KERNEL__
size_t av_strlcatf(char *dst, size_t size, const char *fmt, ...)
{
    int len = strlen(dst);
    va_list vl;

    va_start(vl, fmt);
    len += vsnprintf(dst + len, (int)size > len ? size - len : 0, fmt, vl);
    va_end(vl);

    return len;
}
#endif
