功能描述: 展示音频多路track混音功能（track不与avplay绑定，直接往track送PCM帧）
命令格式: ./sample_mixengine file0 TrackWeight0 [SampleRate0] [Channel0] file1 TrackWeight1 [SampleRate1] [Channel1] ......
参数说明：file0 第0个输入文件名称
          TrackWeight0 第0个track的权重 取值范围0~100
          SampleRate0  第0个输入pcm码流的采样率 单位：Hz 取值范围 80000|11025|12000|16000|22050|24000|320000|44100|48000|88200|96000|176400|192000
          channel0     第0个输入pcm码流的声道数 取值范围 1|2
          file0 第1个输入文件名称
          TrackWeight1 第1个track的权重 取值范围0~100
          SampleRate1  第1个输入pcm码流的采样率 单位：Hz 取值范围 80000|11025|12000|16000|22050|24000|320000|44100|48000|88200|96000|176400|192000
          channel1     第1个输入pcm码流的声道数 取值范围 1|2          
例    子: ./sample/mixengine/sample_mixengine a.wav 100 48000 1 b.wav 100 48000 1


功能描述: 展示音频多路track混音功能（track绑定avplay）
命令格式: ./sample_audiomix freq srate inputfile0 type0 inputfile1 type1 ...
参数说明：freq 频点频率  单位 KHz
	  srate 符号率
          inputfile0   第0个输入文件名称
          type0        第0个输入文件的编码类型
          inputfile1   第1个输入文件名称
          type1        第1个输入文件的编码类型       
例    子: ./sample_audiomix 618 6187 a.mp3 mp3 b.aac aac
