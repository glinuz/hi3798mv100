Purpose: to play subtitle from dvb stream.
    The streams need to support subtitle info.

1.Command: sample_subtitle -f file
      file: path of the subtitle stream file. 

example:
    ./sample_subtitle -f Winter_Hill_Ch-66_1a_sub.ts

2.Command: sample_subtitle -t frequency [symbol rate] [qam]
      frequency: the frequency of tuner. 
      symbol rate: optional field.
      qam:optional field

example:
    ./sample_subtitle -t 610
    ./sample_subtitle -t 610 6875
    ./sample_subtitle -t 610 6875 64

In playing, if you want to choose another program, please input 'p' and then input the program number. After that, you should input 's' and then input the subtitle number for playing.


Notes:
    About the source code, if you want to use it as SD output, please change USING_HD_RESOLUTION to 0 in sample_subtitle.c. By default, the subtitle output in HD. 

    In HD mode, driver framebuffer parameter should be: hifb.ko video="vram2_size=8100"
