.globl _start
_start:
    b       reset

#if   defined (CHIP_TYPE_hi3716mv410) \
   || defined (CHIP_TYPE_hi3716mv420) \
   || defined (CHIP_TYPE_hi3716cv200) \
   || defined (CHIP_TYPE_hi3719cv100) \
   || defined (CHIP_TYPE_hi3719mv100) \
   || defined (CHIP_TYPE_hi3798mv100) \
   || defined (CHIP_TYPE_hi3796mv100) \
   || defined (CHIP_TYPE_hi3798cv200) \
   || defined(CHIP_TYPE_hi3798mv200) \
   || defined(CHIP_TYPE_hi3798mv200_a)
#define LOAD_ADDR  0xffff0000
#define STACK_ADDR 0xffff5000
#endif

#if  defined (CHIP_TYPE_hi3716mv310) \
  || defined (CHIP_TYPE_hi3110ev500) \
  || defined (CHIP_TYPE_hi3716mv330)
#define LOAD_ADDR  0x00000000
#define STACK_ADDR 0x00005000
#endif
/*
 * the actual reset code
 */
reset:
    //b .

    /* set stack to ram */
    ldr     x0, =STACK_ADDR
    mov     sp, x0

    /* main function */
    bl      main


