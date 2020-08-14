
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
		arch/arm/cpu/hi3798mx/start.o	(.text)
		arch/arm/cpu/hi3798mx/lowlevel_init.o	(.text)
		arch/arm/cpu/hi3798mx/emmc_boot.o	(.text)
		arch/arm/cpu/hi3798mx/chip.o	(.text)
		arch/arm/cpu/hi3798mx/uart.o	(.text)
		drivers/ddr/hi3798mx/ddr_training_impl.o (.text)
		drivers/ddr/hi3798mx/ddr_training_ctl.o (.text)
		drivers/ddr/hi3798mx/ddr_training_boot.o (.text)
		drivers/ddr/hi3798mx/ddr_training_custom.o (.text)
		__init_end = .;
		ASSERT(((__init_end - __text_start) < 0x4f00), "init sections too big!");
		*(.text)
	}
	__text_end = .;
	. = . + 0x84;
	. = ALIGN(4);
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }

	. = ALIGN(4);
	.data : { *(.data) }

	. = ALIGN(4);
	.got : { *(.got) }

	__u_boot_cmd_start = .;
	.u_boot_cmd : { *(.u_boot_cmd) }
	__u_boot_cmd_end = .;

	. = ALIGN(4);
	.image : { *(.image) }

	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) }
	_end = .;
}
