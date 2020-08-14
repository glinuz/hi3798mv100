demo description:
   	Examples:
	1. Blend 
        Function:
                 This test case shows how to operate alpha blending ForeGround with BackGround, which result output to Dst.
	 Command:
                 ./sample_blend

	2. Colorfill
        Function:
                 This test case shows the function of quickly filling fixed value to target bitmap. 
        Command:
	         ./sample_colorfill 

	3. Colorkey        
	 Function:
	 	 In the test case,the key color range pixel is not involved in TDE operations.
        Command:	
		 ./sample_colorkey

	4. Deflicker
	 Function:
		This test case shows how to deflicker source bitmap and output to target bitmap.        
	 Command:	 
		 ./sample_deflicker

	5. Quickcopy
	 Function:
		 This test case shows how to quickly blit source to target bitmap.
        Command:	
		 ./sample_quickcopy

	6. Resize
	 Function:
		 This test case shows how to zoom the size of source bitmap to the size of target bitmap.
        Command:	
		 ./sample_resize
	
	7.Convert YUV to RGB
         Function:
		 This test case shows how to convert format YUV SemiPlanar420 to ARGB888.
        Command:	
		 ./sample_YUVtoRGB
		 

The resources list of "sample_*" for running in advanced CA environment.

1. Capabilities list
   1) sys_rawio
      memmap() will open device ¡°/dev/mem¡±, need capability ¡°sys_rawio¡±.

2. Devices list
   /dev/mem
   /dev/hifb
   /dev/tde
   /dev/mmz

3. System resource
   1)NA

4. Commands list by system call
   1) NA

5. Dynamic libraries list
   NA
   Please run the command "/lib/ld-linux.so.3 --list /home/stb/bin/sample_*" on the board to check the dyanmic libraries needed by the application "/home/stb/bin/sample_wm", the list does not include audio libraries .

