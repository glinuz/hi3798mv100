
ENTRY(main)
SECTIONS
{
	. = 0x00000000;

	. = ALIGN(4);
	.text	:
	{
	  boot.o
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
