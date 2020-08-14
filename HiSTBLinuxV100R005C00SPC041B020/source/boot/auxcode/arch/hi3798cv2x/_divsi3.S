# 1 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
# 1 "/home/c00132816/toolchains/arm-hisi-linux/toolchain/finalgcc_build/arm-hisi_soft-linux-uclibcgnueabi/libgcc//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "./_divsi3.vis" 1
 .hidden __divsi3
 .hidden __aeabi_idiv
 .hidden __aeabi_idivmod
# 1 "<command-line>" 2
# 1 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
@ libgcc routines for ARM cpu.
@ Division routines, written by Richard Earnshaw, (rearnsha@armltd.co.uk)
# 28 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.section .note.GNU-stack,"",%progbits
.previous
# 39 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
       .eabi_attribute 25, 1
# 145 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro cfi_pop advance, reg, cfa_offset

 .pushsection .debug_frame
 .byte 0x4
 .4byte \advance
 .byte (0xc0 | \reg)
 .byte 0xe
 .uleb128 \cfa_offset
 .popsection

.endm
.macro cfi_push advance, reg, offset, cfa_offset

 .pushsection .debug_frame
 .byte 0x4
 .4byte \advance
 .byte (0x80 | \reg)
 .uleb128 (\offset / -4)
 .byte 0xe
 .uleb128 \cfa_offset
 .popsection

.endm
.macro cfi_start start_label, end_label

 .pushsection .debug_frame
.Lstart_frame:
 .4byte .Lend_cie - .Lstart_cie @ Length of CIE
.Lstart_cie:
        .4byte 0xffffffff @ CIE Identifier Tag
        .byte 0x1 @ CIE Version
        .ascii "\0" @ CIE Augmentation
        .uleb128 0x1 @ CIE Code Alignment Factor
        .sleb128 -4 @ CIE Data Alignment Factor
        .byte 0xe @ CIE RA Column
        .byte 0xc @ DW_CFA_def_cfa
        .uleb128 0xd
        .uleb128 0x0

 .align 2
.Lend_cie:
 .4byte .Lend_fde-.Lstart_fde @ FDE Length
.Lstart_fde:
 .4byte .Lstart_frame @ FDE CIE offset
 .4byte \start_label @ FDE initial location
 .4byte \end_label-\start_label @ FDE address range
 .popsection

.endm
.macro cfi_end end_label

 .pushsection .debug_frame
 .align 2
.Lend_fde:
 .popsection
\end_label:

.endm



.macro RETLDM regs=, cond=, unwind=, dirn=ia
# 224 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
 .ifc "\regs",""
 ldr\cond pc, [sp], #8
 .else



 ldm\cond\dirn sp!, {\regs, pc}

 .endif

.endm
# 256 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro do_it cond, suffix=""
.endm
.macro shift1 op, arg0, arg1, arg2
 mov \arg0, \arg1, \op \arg2
.endm
# 298 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro shiftop name, dest, src1, src2, shiftop, shiftreg, tmp
 \name \dest, \src1, \src2, \shiftop \shiftreg
.endm



.macro ARM_LDIV0 name signed
 cmp r0, #0
 .ifc \signed, unsigned
 movne r0, #0xffffffff
 .else
 movgt r0, #0x7fffffff
 movlt r0, #0x80000000
 .endif
 b __aeabi_idiv0 (PLT)
.endm
# 326 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro THUMB_LDIV0 name signed
# 371 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
 .align 2
 bx pc
 nop
 .arm
 cmp r0, #0
 .ifc \signed, unsigned
 movne r0, #0xffffffff
 .else
 movgt r0, #0x7fffffff
 movlt r0, #0x80000000
 .endif
 b __aeabi_idiv0 (PLT)
 .thumb

.endm
# 401 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro FUNC_END name
 .size __\name, . - __\name
.endm

.macro DIV_FUNC_END name signed
 cfi_start __\name, .Lend_div0
.Ldiv0:



 ARM_LDIV0 \name \signed

 cfi_end .Lend_div0
 FUNC_END \name
.endm

.macro THUMB_FUNC_START name
 .globl \name
 .type \name,function
 .thumb_func
\name:
.endm
# 440 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro FUNC_START name
 .text
 .globl __\name
 .type __\name,function
 .align 0



__\name:
.endm
# 489 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro ARM_FUNC_START name
 .text
 .globl __\name
 .type __\name,function
 .align 0
 .arm
__\name:
.endm

.macro ARM_CALL name
 bl __\name
.endm




.macro FUNC_ALIAS new old
 .globl __\new



 .set __\new, __\old

.endm


.macro ARM_FUNC_ALIAS new old
 .globl __\new
 .set __\new, __\old



.endm
# 537 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro WEAK name
 .weak __\name
.endm
# 562 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.macro ARM_DIV_BODY dividend, divisor, result, curbit
# 603 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
 clz \curbit, \dividend
 clz \result, \divisor
 sub \curbit, \result, \curbit
 rsbs \curbit, \curbit, #31
 addne \curbit, \curbit, \curbit, lsl #1
 mov \result, #0
 addne pc, pc, \curbit, lsl #2
 nop
 .set shift, 32
 .rept 32
 .set shift, shift - 1
 cmp \dividend, \divisor, lsl #shift
 adc \result, \result, \result
 subcs \dividend, \dividend, \divisor, lsl #shift
 .endr
# 689 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.endm

.macro ARM_DIV2_ORDER divisor, order



 clz \order, \divisor
 rsb \order, \order, #31
# 719 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.endm

.macro ARM_MOD_BODY dividend, divisor, order, spare



 clz \order, \divisor
 clz \spare, \dividend
 sub \order, \order, \spare
 rsbs \order, \order, #31
 addne pc, pc, \order, lsl #3
 nop
 .set shift, 32
 .rept 32
 .set shift, shift - 1
 cmp \dividend, \divisor, lsl #shift
 subcs \dividend, \dividend, \divisor, lsl #shift
 .endr
# 808 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
.endm

.macro THUMB_DIV_MOD_BODY modulo
 @ Load the constant 0x10000000 into our work register.
 mov work, #1
 lsl work, #28
.Loop1:
 @ Unless the divisor is very big, shift it up in multiples of
 @ four bits, since this is the amount of unwinding in the main
 @ division loop. Continue shifting until the divisor is
 @ larger than the dividend.
 cmp divisor, work
 bhs .Lbignum
 cmp divisor, dividend
 bhs .Lbignum
 lsl divisor, #4
 lsl curbit, #4
 b .Loop1
.Lbignum:
 @ Set work to 0x80000000
 lsl work, #3
.Loop2:
 @ For very big divisors, we must shift it a bit at a time, or
 @ we will be in danger of overflowing.
 cmp divisor, work
 bhs .Loop3
 cmp divisor, dividend
 bhs .Loop3
 lsl divisor, #1
 lsl curbit, #1
 b .Loop2
.Loop3:
 @ Test for possible subtractions ...
  .if \modulo
 @ ... On the final pass, this may subtract too much from the dividend,
 @ so keep track of which subtractions are done, we can fix them up
 @ afterwards.
 mov overdone, #0
 cmp dividend, divisor
 blo .Lover1
 sub dividend, dividend, divisor
.Lover1:
 lsr work, divisor, #1
 cmp dividend, work
 blo .Lover2
 sub dividend, dividend, work
 mov ip, curbit
 mov work, #1
 ror curbit, work
 orr overdone, curbit
 mov curbit, ip
.Lover2:
 lsr work, divisor, #2
 cmp dividend, work
 blo .Lover3
 sub dividend, dividend, work
 mov ip, curbit
 mov work, #2
 ror curbit, work
 orr overdone, curbit
 mov curbit, ip
.Lover3:
 lsr work, divisor, #3
 cmp dividend, work
 blo .Lover4
 sub dividend, dividend, work
 mov ip, curbit
 mov work, #3
 ror curbit, work
 orr overdone, curbit
 mov curbit, ip
.Lover4:
 mov ip, curbit
  .else
 @ ... and note which bits are done in the result. On the final pass,
 @ this may subtract too much from the dividend, but the result will be ok,
 @ since the "bit" will have been shifted out at the bottom.
 cmp dividend, divisor
 blo .Lover1
 sub dividend, dividend, divisor
 orr result, result, curbit
.Lover1:
 lsr work, divisor, #1
 cmp dividend, work
 blo .Lover2
 sub dividend, dividend, work
 lsr work, curbit, #1
 orr result, work
.Lover2:
 lsr work, divisor, #2
 cmp dividend, work
 blo .Lover3
 sub dividend, dividend, work
 lsr work, curbit, #2
 orr result, work
.Lover3:
 lsr work, divisor, #3
 cmp dividend, work
 blo .Lover4
 sub dividend, dividend, work
 lsr work, curbit, #3
 orr result, work
.Lover4:
  .endif

 cmp dividend, #0 @ Early termination?
 beq .Lover5
 lsr curbit, #4 @ No, any more bits to do?
 beq .Lover5
 lsr divisor, #4
 b .Loop3
.Lover5:
  .if \modulo
 @ Any subtractions that we should not have done will be recorded in
 @ the top three bits of "overdone". Exactly which were not needed
 @ are governed by the position of the bit, stored in ip.
 mov work, #0xe
 lsl work, #28
 and overdone, work
 beq .Lgot_result

 @ If we terminated early, because dividend became zero, then the
 @ bit in ip will not be in the bottom nibble, and we should not
 @ perform the additions below. We must test for this though
 @ (rather relying upon the TSTs to prevent the additions) since
 @ the bit in ip could be in the top two bits which might then match
 @ with one of the smaller RORs.
 mov curbit, ip
 mov work, #0x7
 tst curbit, work
 beq .Lgot_result

 mov curbit, ip
 mov work, #3
 ror curbit, work
 tst overdone, curbit
 beq .Lover6
 lsr work, divisor, #3
 add dividend, work
.Lover6:
 mov curbit, ip
 mov work, #2
 ror curbit, work
 tst overdone, curbit
 beq .Lover7
 lsr work, divisor, #2
 add dividend, work
.Lover7:
 mov curbit, ip
 mov work, #1
 ror curbit, work
 tst overdone, curbit
 beq .Lgot_result
 lsr work, divisor, #1
 add dividend, work
  .endif
.Lgot_result:
.endm
# 1134 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
 ARM_FUNC_START divsi3
 ARM_FUNC_ALIAS aeabi_idiv divsi3

 cmp r1, #0
 beq .Ldiv0
.divsi3_nodiv0:
 eor ip, r0, r1 @ save the sign of the result.
 do_it mi
 rsbmi r1, r1, #0 @ loops below use unsigned.
 subs r2, r1, #1 @ division by 1 or -1 ?
 beq 10f
 movs r3, r0
 do_it mi
 rsbmi r3, r0, #0 @ positive dividend value
 cmp r3, r1
 bls 11f
 tst r1, r2 @ divisor is power of 2 ?
 beq 12f

 ARM_DIV_BODY r3, r1, r0, r2

 cmp ip, #0
 do_it mi
 rsbmi r0, r0, #0
 bx lr

10: teq ip, r0 @ same sign ?
 do_it mi
 rsbmi r0, r0, #0
 bx lr

11: do_it lo
 movlo r0, #0
 do_it eq,t
 moveq r0, ip, asr #31
 orreq r0, r0, #1
 bx lr

12: ARM_DIV2_ORDER r1, r2

 cmp ip, #0
 mov r0, r3, lsr r2
 do_it mi
 rsbmi r0, r0, #0
 bx lr



 DIV_FUNC_END divsi3 signed
# 1195 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
ARM_FUNC_START aeabi_idivmod
 cmp r1, #0
 beq .Ldiv0
 stmfd sp!, { r0, r1, lr }
 bl .divsi3_nodiv0
 ldmfd sp!, { r1, r2, lr }
 mul r3, r2, r0
 sub r1, r1, r3
 bx lr

 FUNC_END aeabi_idivmod
# 1750 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm"
# 1 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/ieee754-df.S" 1
# 39 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/ieee754-df.S"
@ For FPA, float words are always big-endian.
@ For VFP, floats words follow the memory system mode.
# 1751 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm" 2
# 1 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/ieee754-sf.S" 1
# 1752 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm" 2
# 1 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/bpabi.S" 1
# 67 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/bpabi.S"
.macro test_div_by_zero signed
# 94 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/bpabi.S"
 cmp r3, #0
 cmpeq r2, #0
 bne 2f
 cmp r1, #0
 cmpeq r0, #0
 .ifc \signed, unsigned
 movne r1, #0xffffffff
 movne r0, #0xffffffff
 .else
 movlt r1, #0x80000000
 movlt r0, #0
 movgt r1, #0x7fffffff
 movgt r0, #0xffffffff
 .endif
 b __aeabi_ldiv0 (PLT)
2:

.endm
# 1753 "../../../gcc-4.4-2010q1/libgcc/../gcc/config/arm/lib1funcs.asm" 2
