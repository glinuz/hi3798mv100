/*reference arm-linux-androideabi-ld.bfd --verbose*/
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    . = 0x00000000;

    . = ALIGN(0x1000);
    .text : {
        *(.text)           /* .text sections (code) */
        *(.text*)           /* .text sections (code) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
     }

    . = ALIGN(0x1000);
    ro_data_start = .;
    .rodata(ro_data_start) : AT(ro_data_start) {
        *(.rodata)
        *(.rodata*)
    }

    . = ALIGN(0x1000);
    data_start = .;
    .data(data_start) : AT(data_start) {
        *(.data)
        *(.data*)
    }

    .bss : {
        TA_BSS_START = .;
        *(.bss)
        TA_BSS_END = .;
    }


    .rel.text : {
        *(.rel.text)
    }

    .rel.data : {
        *(.rel.data)
    }
    
    /* for elf parse */
    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) }
    .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
}
