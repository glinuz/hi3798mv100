Function£º
Implement the displaying of mp3 spectrum distribution diagram.


Command: 
./sample_mp3_spectrum inputfile.mp3 [band_number] [vo_format]

In the preceding command, band_number indicate the number of rectangles to be drawn. The number can be 20, 80, or 512. 
vo_format indicate the format of video output, the format can be 1080P_60|1080P_50|1080i_60|1080i_50|720P_60|720P_50

for example:
./sample_mp3_spectrum ./bigsea.mp3 80 1080i_50
