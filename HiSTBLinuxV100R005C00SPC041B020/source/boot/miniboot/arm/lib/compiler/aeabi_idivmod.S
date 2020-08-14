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
 * Come from : LLVM compiler-rt-master/lib/arm/aeabi_idivmod.S
 * URL : https://github.com/llvm-mirror/compiler-rt/archive/master.zip 
******************************************************************************/

//===-- aeabi_idivmod.S - EABI idivmod implementation ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "assembly.h"

// struct { int quot, int rem} __aeabi_idivmod(int numerator, int denominator) {
//   int rem, quot;
//   quot = __divmodsi4(numerator, denominator, &rem);
//   return {quot, rem};
// }

        .syntax unified
        .align 2
DEFINE_COMPILERRT_FUNCTION(__aeabi_idivmod)
        push    { lr }
        sub     sp, sp, #4
        mov     r2, sp
        bl      SYMBOL_NAME(__divmodsi4)
        ldr     r1, [sp]
        add     sp, sp, #4
        pop     { pc }
