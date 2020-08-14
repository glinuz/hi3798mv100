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

    SEC_DATA_START = 0xc0200000;
    . = ALIGN(32);
    _text_end = .;

    .data.rel.ro(SEC_DATA_START) : AT(_text_end){ *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro* .gnu.linkonce.d.rel.ro.*) }
    . = ALIGN(32);
    _tmp_end = (_text_end + SIZEOF(.data.rel.ro) + 31) & (~ 31);
    .dynamic : AT(_tmp_end){ *(.dynamic) }
    . = ALIGN(32);
    _tmp_end = (_tmp_end + SIZEOF(.dynamic) + 31) & (~ 31);
    .got : AT(_tmp_end){ *(.got.plt) *(.igot.plt) *(.got) *(.igot) }
    . = ALIGN(32);
    _tmp_end = (_tmp_end + SIZEOF(.got) + 31) & (~ 31);

    .data :  AT(_tmp_end){
        DATA_START = .;
        *(.data)
        *(.data*)
        *(.shared.data)
        DATA_END = .;
    }
    . = ALIGN(32);
    _tmp_end = (_tmp_end + SIZEOF(.data) + 31) & (~ 31);

    .ARM.extab : AT(_tmp_end){ *(.ARM.extab* .gnu.linkonce.armextab.*) }
    . = ALIGN(32);
    _tmp_end = (_tmp_end + SIZEOF(.ARM.extab) + 31) & (~ 31);
    .ARM.exidx : AT(_tmp_end){ *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
    . = ALIGN(32);
    _data_end = (_tmp_end + SIZEOF(.ARM.exidx) + 31) & (~ 31);

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

    . = ALIGN(32);
    ARM_LIB_STACKHEAP       = BSS_END  + 0x4000;
    ARM_LIB_USR_STACKHEAP   = ARM_LIB_STACKHEAP  + 0x400;
    ARM_LIB_SYS_STACKHEAP   = ARM_LIB_USR_STACKHEAP + 0x1000;
    ARM_LIB_IRQ_STACKHEAP   = ARM_LIB_SYS_STACKHEAP + 0x200;
    ARM_LIB_UND_STACKHEAP   = ARM_LIB_IRQ_STACKHEAP + 0x200;
    ARM_LIB_ABT_STACKHEAP   = ARM_LIB_UND_STACKHEAP + 0x200;
    ARM_LIB_FIQ_STACKHEAP   = ARM_LIB_ABT_STACKHEAP + 0x200;

    CODE_END = ARM_LIB_FIQ_STACKHEAP + 0x200;
}
