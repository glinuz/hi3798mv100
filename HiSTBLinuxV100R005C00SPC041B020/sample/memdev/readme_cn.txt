功能描述：
	演示如何/dev/hi_mem设备以及HI_SYS_ReadRegister/HI_SYS_WriteRegister/HI_SYS_MapRegister/HI_SYS_UnmapRegister等API的使用方法。
命令格式：
	./sample_memdev ADDRESS_IN_HEX VALUE_IN_HEX [METHOD]
使用说明：
	示例：
	./sample_memdev 0xf8001000 0x1 1
	./sample_memdev 0xf8001000 0x1 2
	./sample_memdev 0xf8001000 0x1 3
	./sample_memdev 0xf8001000 0x1