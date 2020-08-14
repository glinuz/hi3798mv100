This software is used to calculate the DDR hash value when standby.

1.Compilation
	make clean;make		Compile the software. ddr_wakeup_check.bin is generated.
	
2.Usage
	- if compile 64bit ddr_wakeup_check.bin,please modify the Makefile,
	delete CFLAGS += -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16 
	- Use the tool hex2char.exe to convert the ddr_wakeup_check.bin to output.txt
	