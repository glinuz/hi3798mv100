/*reference arm-linux-androideabi-ld.bfd --verbose*/
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

/* be careful to change the linker.ld, read the global-task-load code before change it */
SECTIONS
{
    . = 0x00000000;

    .globaltask : {
        PROCESS_START = .;
        *global_task.o(.text.start*)
    }

    . = ALIGN(0x1000);
    /* modify by l00202565 begin */
    /* because teelib defined from TEE_TEXT_START to TEE_LIB_END */
    /* needed section must be difined in this scope, and no used section put outside */
    .text : {
        TEE_TEXT_START = .;
        *(.text .text.*)           /* .text sections (code) */
        *(.stub)

        /* ARM glue code for ARM and thumb */
        *(.glue_7)
        *(.glue_7t)

        /* Workaround for an erratum in ARM's VFP11 coprocessor */
        *(.vfp11_veneer)
        /* GCC/ld specific */
        *(.gnu.linkonce.t.*)
        *(.gnu.linkonce.r.*)
        *(.gnu.warning)
        *(.gcc_except_table)
    }

    /* add for some link may set rodata before text section */
    /* can lead to rodata not copy when global load */
    . = ALIGN(16);
    .rodata : {
        *(.rodata .rodata.*)
        *(.rodata1)
    }

    /* used for stack frame unwinding on ARM */
    . = ALIGN(16);
    .ARM.exidx : {
        *(.ARM.exidx)
        *(.gnu.linkonce.armexidx.*)
    }

    . = ALIGN(0x1000);
    /*TEE export sections*/
    __tee_symtab : {
        TEE_SYMTAB_START = .;
        *(__tee_symtab)
        TEE_SYMTAB_END = .;
    }
    . = ALIGN(0x16);


    TEE_SYMSTR_START = .;
    __tee_symstr : {
         *(__tee_symstr)
    }

    /* GOT table */
    . = ALIGN(16);
    .got.plt : {*(.got.plt)}

    . = ALIGN(16);
    TEE_GOT_START = .;
    .got : {*(.got .got.*)}
    TEE_GOT_END = .;

    . = ALIGN(16);
    TEE_RELPLT_START = .;
    .rel.plt : {*(.rel.plt)}
    TEE_RELPLT_END = .;

    . = ALIGN(16);
    TEE_RELAPLT_START = .;
    .rela.plt : {*(.rela.plt)}
    TEE_RELAPLT_END = .;

    . = ALIGN(16);
    .rel.dyn : {
        TEE_RELDYN_START = .;
        *(.rel.dyn)
        *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*)
        TEE_RELDYN_END = .;
        TEE_RELADYN_START = .;
        *(.rela.dyn)
        *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
        TEE_RELADYN_END = .;
    }

    . = ALIGN(16);
    TEE_DYNSYM_START = .;
    .dynsym : { *(.dynsym) }
    TEE_DYNSYM_END = .;
    .dynstr : { *(.dynstr) }

    . = ALIGN(16);
    TEE_DYNAMIC_START = .;
    .dynamic : { *(.dynamic) }
    TEE_DYNAMIC_END = .;

    . = ALIGN(0x1000);
    .data : {
        *(.data .data.*)
        /* GCC/ld specific */
        *(.gnu.linkonce.d.*)
    }
    .data1 : {*(.data1)}
    .tdata : {*(.tdata .tdata.*)}

    . = ALIGN(16);
    .bss : {
        TEE_BSS_START = .;
        *(.bss .bss.*)
        *(.dynbss)
        /* GCC/ld specific */
        *(.gnu.linkonce.b.*)
     }
     tbss : {*(.tbss .tbss.*)}


    TEE_LIB_END = .;

    /* this seciton no need in libtee */
    .interp : { *(.interp) }
    .hash : { *(.hash) }
    .shstrtab : { *(.shstrtab) }
    .symtab : { *(.symtab) }
    .strtab : { *(.strtab) }

    .gnu.version : { *(.gnu.version) }
    .gnu.version_d : { *(.gnu.version_d) }
    .gnu.version_r : { *(.gnu.version_r) }

    /* Stabs debugging sections.  */
    .stab          : { *(.stab) }
    .stabstr       : { *(.stabstr) }
    .stab.excl     : { *(.stab.excl) }
    .stab.exclstr  : { *(.stab.exclstr) }
    .stab.index    : { *(.stab.index) }
    .stab.indexstr : { *(.stab.indexstr) }
    .comment       : { *(.comment) }

    /* debug info */
    .debug_info  : { *(.debug_info) }
    .debug_abbrev  : { *(.debug_abbrev) }
    .debug_loc  : { *(.debug_loc) }
    .debug_aranges  : { *(.debug_aranges) }
    .debug_ranges : { *(.debug_ranges) }
    .debug_line  : { *(.debug_line) }
    .debug_str : { *(.debug_str) }
    .comment : { *(.comment) }
    .debug_frame   : { *(.debug_frame) }
    .note.gnu.gold-ve : { *(.note.gnu.gold-ve) }
    .ARM.attributes : { *(.ARM.attributes) }

    /*
     * Remove relocation information for GOT and the headers.
     * All needed relocation informtion should already be matched above.
     */
    .junkedrel /*ALIGN(16)*/ :
    {
        *(.rel.*)
        *(.rela.*)
    }
    /* modify by l00202565 end */
}
