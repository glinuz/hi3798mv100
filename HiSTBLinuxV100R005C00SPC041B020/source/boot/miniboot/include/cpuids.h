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

#ifndef CPUIDSH
#define CPUIDSH
/******************************************************************************/

#define _HI3716M                      (0x08)
#define _HI3716H                      (0x0D)
#define _HI3716C                      (0x1E)
#define _HI3712                       (0x00)

#define _HI3716M_V100                 (0x0837200200LL)
#define _HI3716M_V200                 (0x0837160200LL)
#define _HI3716M_V300                 (0x0837160300LL)
/* for Hi3716MV310 BGA, chipid is 0x0037160310LL, 
 * for Hi3716MV310 QFP, chipid is 0x0137160310LL */
#define _HI3716M_V310                 (0x0037160310LL)
#define _HI3716M_V320                 (0x0237160310LL)
#define _HI3110E_V500                 (0x0337160310LL)
#define _HI3716C_V200ES               (0x0019400200LL)
#define _HI3716C_V100                 (0x1E37200200LL)
#define _HI3716C_V200                 (0x0037160200LL)
#define _HI3716H_V100                 (0x0D37200200LL)
#define _HI3719C_V100                 (0x0837160200LL)
#define _HI3718C_V100                 (0x1037160200LL)
#define _HI3716X_MASK                 (0xFFFFFFFFFFLL)
#define _HI3716M_V400                 (0x1C37160200LL)
#define _HI3716MV400_MASK             (0xFEFFFFFFFFLL)
#define _HI3719M_V100                 (0x0037190100LL)
#define _HI3718M_V100                 (0x0437190100LL)
#define _HI3719M_MASK                 (0xFFFFFFFFFFLL)
#define _HI3718M_MASK                 (0xFEFFFFFFFFLL)
#define _HI3798CV100A                 (0x0019050100LL)
#define _HI3798CV100                  (0x1C19050100LL)
#define _HI3796CV100                  (0x1819050100LL)
#define _HI3798CV100A_MASK            (0xFFFFFFFFFFLL)

#define _HI3798M_V100_A               (0x0019400300LL)
#define _HI3798M_V100_A_MASK          (0xF0FFFFFFFFLL)

#define _HI3798M_V100                 (0x0137980100LL)
#define _HI3796M_V100                 (0x0037980100LL)
#define _HI3716D_V100                 (0x1037980100LL)
#define _HI3798M_V100_MASK            (0xF1FFFFFF0FLL)

#define _HI3716M_V410                 (0x0137160410LL)
#define _HI3716M_V420                 (0x0037160410LL)
#define _HI3716M_V410_MASK            (0xFFFFFFFFFFLL)

#define _HI3798C_V200                 (0x0037980200LL) //(0x0020981000LL)
#define _HI3798C_V200_MASK            (0xFFFFFFFFFFLL)

#define _HIFONE_B02                   (0x0020281000LL)
#define _HIFONE_B02_MASK              (0xFFFFFFFFFFLL)

#define _HI3798M_V200                 (0x0037986200LL) //(0x0020981000LL)
#define _HI3798M_V200_MASK            (0xFFFFFFFFFFLL)

#define _HI3798M_A_V200               (0x0037988200LL)
#define _HI3798M_A_V200_MASK          (0xFFFFFFFFFFLL)

#define _HI3712_V100                  (0x0037120100LL)
#define _HI3712_MASK                  (0x00FFFFFFFFLL)
#define _HI3712_V100A      (0)
#define _HI3712_V100B      (1)
#define _HI3712_V100C      (2)
#define _HI3712_V100D      (3)
#define _HI3712_V100E      (4)
#define _HI3712_V100F      (5)
#define _HI3712_V100G      (6)
#define _HI3712_V100I      (7)

/* The support device every chip platform */
#define DEV_HINFC_AUTO     0x01
#define DEV_HINFC301       0x02
#define DEV_HINFC504       0x03
#define DEV_HINFC610       0x04
#define DEV_HINFC_MASK     0x0F

#define DEV_EMMC_AUTO      0x10
#define DEV_EMMCV100       0x20
#define DEV_EMMCV200       0x20
#define DEV_EMMC_MASK      0xF0

#define DEV_HISFC_AUTO     0x100
#define DEV_HISFC300       0x200
#define DEV_HISFC350       0x300
#define DEV_HISFC_MASK     0xF00

/* bootstrap type */
#define SELF_BOOT_TYPE_NONE           (0)  /* boot type: NONE SELF BOOT */
#define SELF_BOOT_TYPE_UART           (1)  /* boot type: UART */
#define SELF_BOOT_TYPE_USBDEV         (2)  /* boot type: USBDEV */
#define SELF_BOOT_TYPE_USBHOST        (3)  /* boot type: USBHOST */

/* chipset ca type */
#define CHIPSET_CATYPE_INVALID        (-1) /* chipset catype: invalid type */
#define CHIPSET_CATYPE_NORMAL         (0)  /* chipset catype: NORMAL */
#define CHIPSET_CATYPE_CA             (1)  /* chipset catype: CA */
/******************************************************************************/

/* board type */
/* for hi3798cv2x board type. */
#define BOARD_TYPE_0            0x1
#define BOARD_TYPE_1            0x2
#define BOARD_TYPE_2            0x3
#define BOARD_TYPE_RESERVED     0x10
#define BOARD_TYPE_INVALID      0x11

#endif /* CPUIDSH */
