
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x00000000;

	. = ALIGN(4);
	.text	:
	{
		__text_start = .;
		start.o	(.text)
		lowlevel_init.o	(.text)
		ddr_training_impl.o   (.text)
		ddr_training_ctl.o    (.text)
		ddr_training_boot.o   (.text)
		ddr_training_custom.o (.text)
		emmc_boot.o	(.text)
		chip.o	(.text)
		uart.o (.text)
		__init_end = .;
		ASSERT(((__init_end - __text_start) < 0x7400), "init sections too big!");
		image_data.o	(.text)
		*(.text)
	}
	__text_end = .;

	. = ALIGN(4);
	.image : { *(.image) }

	. = ALIGN(4);
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }

	. = ALIGN(4);
	.data : { *(.data) }

	. = ALIGN(4);
	.got : { *(.got) }

	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) }
	_end = .;
}
