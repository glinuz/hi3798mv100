/*
 * global_srst
 */

 .align 2
.global global_srst
global_srst:
    mov     r4, #0
    movt    r4, #63488                  /* r4: REG_SC_BASE */
    mov     r0, #1
    str     r0, [r4, #4]                /* Set SC_SYSRES to 1 */
    dsb
    mov     r4, #0
    movt    r4, #63488                  /* r4: REG_SC_BASE */
    mov     r0, #1
    str     r0, [r4, #4]                /* Set SC_SYSRES to 1 */
    dsb
    mov     r4, #0
    movt    r4, #63488                  /* r4: REG_SC_BASE */
    mov     r0, #1
    str     r0, [r4, #4]                /* Set SC_SYSRES to 1 */
    dsb
    mov     r4, #0
    movt    r4, #63488                  /* r4: REG_SC_BASE */
    mov     r0, #1
    str     r0, [r4, #4]                /* Set SC_SYSRES to 1 */
    dsb
    mov     r4, #0
    movt    r4, #63488                  /* r4: REG_SC_BASE */
    mov     r0, #1
    str     r0, [r4, #4]                /* Set SC_SYSRES to 1 */
    dsb
    mov     r4, #0
    movt    r4, #63488                  /* r4: REG_SC_BASE */
    mov     r0, #1
    str     r0, [r4, #4]                /* Set SC_SYSRES to 1 */
    dsb
    mov     r4, #0
    movt    r4, #63488                  /* r4: REG_SC_BASE */
    mov     r0, #1
    str     r0, [r4, #4]                /* Set SC_SYSRES to 1 */
    dsb


