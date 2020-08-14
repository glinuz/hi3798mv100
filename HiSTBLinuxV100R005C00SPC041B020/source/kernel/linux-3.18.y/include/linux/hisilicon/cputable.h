/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.25
 *
******************************************************************************/

#ifndef HISILICON_CPUTABLE_H
#define HISILICON_CPUTABLE_H

#define _HI3716CV200ES              (0x0019400200ULL)
#define _HI3716CV200ES_MASK         (0xFFFFFFFFFFULL)

#define _HI3716CV200                (0x0037160200ULL)
#define _HI3716CV200_MASK           (0xFFFFFFFFFFULL)

#define _HI3716HV200                (0x0437160200ULL)
#define _HI3719CV100                (0x0837160200ULL)
#define _HI3718CV100                (0x1037160200ULL)

#define _HI3716MV400                (0x1C37160200ULL)
#define _HI3716MV400_MASK           (0xFEFFFFFFFFULL)

#define _HI3716MV410                (0x0137160410LL)
#define _HI3716MV420                (0x0037160410LL)
#define _HI3716MV410_MASK           (0xFFFFFFFFFFLL)

#define _HI3719MV100A               (0x1E37160200ULL)
#define _HI3719MV100A_MASK          (0xFEFFFFFFFFULL)

#define _HI3719MV100                (0x0037190100ULL)
#define _HI3718MV100                (0x0437190100ULL)
#define _HI3718MV100_MASK           (0xFEFFFFFFFFULL)

#define _HI3798CV100A               (0x0019050100ULL)
#define _HI3798CV100A_MASK          (0xFFFFFFFFFFULL)

#define _HI3798CV100                (0x1C19050100ULL)
#define _HI3796CV100                (0x1819050100ULL)
#define _HI3798CV100_MASK           (0xFFFFFFFFFFULL)

#define _HI3798CV200_A              (0x0020281000ULL)
#define _HI3798CV200_B              (0x0120281000ULL)

#define _HI3798MV100_A              (0x0019400300ULL)
#define _HI3798MV100_A_MASK         (0xF0FFFFFFFFULL)

#define _HI3798MV100                (0x0137980100ULL)
#define _HI3796MV100                (0x0037980100ULL)
#define _HI3716DV100                (0x1037980100ULL)
#define _HI3798MV100_MASK           (0xF1FFFFFF0FULL)

#define _HI3798CV200                (0x0037980200LL)
#define _HI3798CV200_MASK           (0xFFFFFFFFFFLL)

#define _HI3798MV200                (0x0037986200LL)
#define _HI3798MV200_MASK           (0xFFFFFFFFFFLL)

#define _HI3798MV200_A              (0x0037988200LL)

#define _HI3796MV200                (0x0037960200ULL)
#define _HI3796MV200_MASK           (0xFFFFFFFFFFULL)

u64 get_chipid(u64 mask);

const char *get_cpuname(void);

extern u64 cpu_chipid;

#endif /* HISILICON_CPUTABLE_H*/
