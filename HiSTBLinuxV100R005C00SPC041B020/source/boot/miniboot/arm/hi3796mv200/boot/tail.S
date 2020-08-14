@/******************************************************************************
@ *    Copyright (c) 2009-2016 by Hisi.
@ *    All rights reserved.
@ * ***
@ *    Create by Lyj. 2014-05-13
@ *
@******************************************************************************/

#include <config.h>

#ifndef CONFIG_NO_HEAD
#ifdef CONFIG_UNIFIED_BOOT
.section .tail,#alloc

_checked_area_reserve:
	.fill 0x10,1,0

.align	8 /* align to 0x100 */
.globl _checked_area_end
_checked_area_end:

/*
 * +-----------------------------------------+
 * | 8. Boot Signature                       |
 * +-----------------------------------------+
 */
_boot_sign_area:
	.fill 0x100,1,0

.globl _total_boot_area_end
 _total_boot_area_end:
 
/*
 * +-----------------------------------------+
 * | 9. Reg list                             |
 * +-----------------------------------------+
 */
.align	9 /* align to 0x200 */
.globl reg_list_start
reg_list_start:
_boot_reg0:
#ifdef BOOT_REG0
	.incbin	BOOT_REG0
#endif

.=_boot_reg0 + CONFIG_REG_SIZE
_boot_reg1:
#ifdef BOOT_REG1
	.incbin	BOOT_REG1
#endif

.=_boot_reg1 + CONFIG_REG_SIZE
_boot_reg2:
#ifdef BOOT_REG2
	.incbin	BOOT_REG2
#endif

.=_boot_reg2 + CONFIG_REG_SIZE
_boot_reg3:
#ifdef BOOT_REG3
	.incbin	BOOT_REG3
#endif

.=_boot_reg3 + CONFIG_REG_SIZE
_boot_reg4:
#ifdef BOOT_REG4
	.incbin	BOOT_REG4
#endif

.=_boot_reg4 + CONFIG_REG_SIZE
_boot_reg5:
#ifdef BOOT_REG5
	.incbin	BOOT_REG5
#endif

.=_boot_reg5 + CONFIG_REG_SIZE
_boot_reg6:
#ifdef BOOT_REG6
	.incbin	BOOT_REG6
#endif

.=_boot_reg6 + CONFIG_REG_SIZE
_boot_reg7:
#ifdef BOOT_REG7
	.incbin	BOOT_REG7
#endif

.=_boot_reg7 + CONFIG_REG_SIZE
reg_list_end:
.globl _boot_end
_boot_end:

#endif
#endif

