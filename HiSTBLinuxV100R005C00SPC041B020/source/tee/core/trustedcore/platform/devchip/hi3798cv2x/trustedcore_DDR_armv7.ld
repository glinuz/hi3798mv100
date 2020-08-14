/*reference arm-linux-androideabi-ld.bfd --verbose*/
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0x00000000;

    .text : {
        CODE_START = .;
        *(.inittext)
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        TEXT_END = .;
    }

    /*RTOSck added*/
    .srvcall.init : {
        SRV_INIT_CALL_START = .;
        *(.srvcall.init)
        SRV_INIT_CALL_END = .;
    }
    /*Thirdparty driver init added*/
    . = ALIGN(4);
    .tc_drv_descs : {
        __TC_DRV_DESCS_START__ = .;
        *(.tc_drv_descs)
        __TC_DRV_DESCS_END__ = .;
    }

    . = ALIGN(0x200000);
    .data.rel.ro : { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro* .gnu.linkonce.d.rel.ro.*) }
    .dynamic : { *(.dynamic) }
    .got : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) }

    .data :  {
        DATA_START = .;
        *(.data)
        *(.data*)
        *(.shared.data)
        DATA_END = .;
    }

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) }
    .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }

    SHARED_BSS_START = .;
    /*
    .shared.bss : {
    *(.shared.bss)
    }
    */
    SHARED_BSS_END = .;

    .bss : {
        BSS_START = .;
        *(.dynbss)
        *(.bss .gnu.linkonce.b.*)
        *(COMMON)
        /*RTOSck added*/
        *(.shared.bss)
        BSS_END = .;
    }

    /*Secure Pagetable begin*/
    . = ALIGN(0x4000);
    PAGE_TABLE_START = .;
    .bss.pagetable : AT(PAGE_TABLE_START){
        *(.bss.pagetable)
        PAGE_TABLE_END = .;
    }
    /*Secure Pagetable end*/

    ARM_LIB_STACKHEAP       = PAGE_TABLE_END  + 0x4000;
    ARM_LIB_USR_STACKHEAP   = ARM_LIB_STACKHEAP  + 0x400;
    ARM_LIB_SYS_STACKHEAP   = ARM_LIB_USR_STACKHEAP + 0x1000;
    ARM_LIB_IRQ_STACKHEAP   = ARM_LIB_SYS_STACKHEAP + 0x200;
    ARM_LIB_UND_STACKHEAP   = ARM_LIB_IRQ_STACKHEAP + 0x200;
    ARM_LIB_ABT_STACKHEAP   = ARM_LIB_UND_STACKHEAP + 0x200;
    ARM_LIB_MON_STACKHEAP   = ARM_LIB_ABT_STACKHEAP + 0x1000;
    ARM_LIB_FIQ_STACKHEAP   = ARM_LIB_MON_STACKHEAP + 0x200;

    SEC_CPU_START = ARM_LIB_FIQ_STACKHEAP + 0x1000;
    CODE_END = SEC_CPU_START + 0x8000;
}
