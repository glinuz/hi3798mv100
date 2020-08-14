
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(main)
SECTIONS
{
	. = 0x00000000;

	. = ALIGN(4);
	.text	:
	{
		start.o
		main.o
		*(.text)
	}

	. = ALIGN(4);
	.rodata :
	{
		*(.rodata)
	}

	. = ALIGN(4);
	.data :
	{
	  *(.data)
	}

}
