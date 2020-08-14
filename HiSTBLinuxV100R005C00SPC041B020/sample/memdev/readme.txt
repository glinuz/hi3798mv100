Purpose: to demonstrate the usage of device /dev/hi_mem and APIs: HI_SYS_ReadRegister/HI_SYS_WriteRegister/HI_SYS_MapRegister/HI_SYS_UnmapRegister.
Command: ./sample_memdev ADDRESS_IN_HEX VALUE_IN_HEX [METHOD]
eg:	
	./sample_memdev 0xf8001000 0x1 1
	./sample_memdev 0xf8001000 0x1 2
	./sample_memdev 0xf8001000 0x1 3
	./sample_memdev 0xf8001000 0x1