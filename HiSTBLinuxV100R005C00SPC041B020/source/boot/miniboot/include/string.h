/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
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

#ifndef STRINGH
#define STRINGH
/******************************************************************************/

#include <stddef.h>

extern char const hex2ascii_data[];

#define hex2ascii(hex) (hex2ascii_data[hex])

#define imax(_a, _b) ((_a) > (_b) ? (_a) : (_b))

#define bcpy(src, dst, len) memcpy((dst), (src), (len))
#define bzero(buf, size)    memset((buf), 0, (size))
#define bcmp(b1, b2, len)   (memcmp((b1), (b2), (len)) != 0)

void *memcpy(void *dst, const void *src, size_t len);
void *memset(void *b, int c, size_t len);
void *memmove(void *dst, const void *src, size_t size);
int memcmp(const void *b1, const void *b2, size_t len);
int strncmp(const char *s1, const char *s2, size_t len);
uint32 strnlen(const char *s, uint32 len);
unsigned long strtoul(const char *nptr, char **endptr, int base);
uint64 strtoull(const char *nptr, char **endptr, int base);
char *strncpy(char * dst, const char * src, size_t n);
uint64 memparse(const char *ptr, char **retptr);
char *strndup(const char *str, size_t n);
char *strtok(char *s, const char *delim);

#ifndef CONFIG_NO_UNSAFE_FUNCTION
int strcmp(const char *s1, const char *s2);
void strcpy(char *dst, const char *src);
void strcat(char *dst, const char *src);
char *strncat(char *dst, const char *src, size_t n);
char *strchr(const char *s, char ch);
char *strdup(const char *s);
size_t strlen(const char *s);
char *strstr(const char *s1, const char *s2);
#endif /* CONFIG_NO_UNSAFE_FUNCTION */

/******************************************************************************/
#endif /* STRINGH */
