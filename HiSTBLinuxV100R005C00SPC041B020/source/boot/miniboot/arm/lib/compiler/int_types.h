/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
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
 * Come from : LLVM compiler-rt-master/lib/int_lib.h
 * URL : https://github.com/llvm-mirror/compiler-rt/archive/master.zip
******************************************************************************/

#ifndef INT_TYPES_H
#define INT_TYPES_H

# define COMPILER_RT_ABI __attribute__((pcs("aapcs")))

#define CHAR_BIT 8


typedef unsigned long long du_int;
typedef          long long di_int;

typedef unsigned su_int;
typedef      int si_int;

typedef union
{
    long long all;
    struct
    {
        unsigned low;
        int high;
    }s;
} dwords;

typedef union
{
    du_int all;
    struct
    {
        su_int low;
        su_int high;
    }s;
} udwords;


typedef union
{
    su_int u;
    float f;
} float_bits;

typedef union
{
    udwords u;
    double  f;
} double_bits;

typedef struct
{
    udwords low;
    udwords high;
} uqwords;

typedef union
{
    uqwords     u;
    long double f;
} long_double_bits;

#endif /* INT_TYPES_H */

