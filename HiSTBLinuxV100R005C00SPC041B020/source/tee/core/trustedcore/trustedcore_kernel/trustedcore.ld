/*reference arm-linux-androideabi-ld.bfd --verbose*/
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0x00000000;

    .text : {
        CODE_START = .;
        *sre_reset_vector.o(.text)
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

    /*Secondary cpu mointor begin*/
    SEC_MONITOR_ADDR = 0x2EE00000;
     . = ALIGN(32);
     _sram_start = .;

    .sram.secmonitor(SEC_MONITOR_ADDR) : AT(_sram_start) {
        SEC_MONITOR_START = .;
        *(.sram.secmonitor)
        SEC_MONITOR_END = .;
    }

    /* modify by l00202565, ld action different by different toolchain */
    /*. = (_sram_start + SIZEOF(.sram.secmonitor) + 31) & (~ 31);*/
    _sram_end = (_sram_start + SIZEOF(.sram.secmonitor) + 31) & (~ 31);
    . = _sram_end;
    /*Secondary cpu mointor end*/

    .data.rel.ro : AT(_sram_end){ *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro* .gnu.linkonce.d.rel.ro.*) }
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

    /* Suspend Process let DDR enter selfrefresh, so we must locate secure pagetable in sram*/
    /*
    SECURE_PAGETABLE_ADDR = 0xf8008000;
    SECURE_PAGETABLE_SR_DDR = 0x2cf1c000;
    SECONDARY_CPU_ENTRY_ADDRESS = 0x2cf18000;
    .bss.pagetable(SECURE_PAGETABLE_ADDR) : {
        SECURE_PAGETABLE_START = .;
        *(.bss.pagetable)
        SECURE_PAGETABLE_END = .;
    }
    */

    ARM_LIB_STACKHEAP       = PAGE_TABLE_END  + 0x40000;
    ARM_LIB_USR_STACKHEAP   = ARM_LIB_STACKHEAP  + 0x400;
    ARM_LIB_SYS_STACKHEAP   = ARM_LIB_USR_STACKHEAP + 0x1000;
    ARM_LIB_IRQ_STACKHEAP   = ARM_LIB_SYS_STACKHEAP + 0x200;
    ARM_LIB_UND_STACKHEAP   = ARM_LIB_IRQ_STACKHEAP + 0x200;
    ARM_LIB_ABT_STACKHEAP   = ARM_LIB_UND_STACKHEAP + 0x200;
    ARM_LIB_MON_STACKHEAP   = ARM_LIB_ABT_STACKHEAP + 0x1000;
    ARM_LIB_FIQ_STACKHEAP   = ARM_LIB_MON_STACKHEAP + 0x200;

    CODE_END = ARM_LIB_FIQ_STACKHEAP;
}
