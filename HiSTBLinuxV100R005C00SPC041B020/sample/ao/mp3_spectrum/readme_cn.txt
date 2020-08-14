功能描述: 展示MP3播放时的频谱图
命令格式: ./sample_mp3_spectrum inputfile.mp3 [band_number] [vo_format]
参数说明：inputfile.mp3 mp3输入文件名称
          band_number 频谱图中矩形个数，取值范围 20|80|512
          vo_format 视频输出格式，取值范围 1080P_60|1080P_50|1080i_60|1080i_50|720P_60|720P_50      
例    子: ./sample_mp3_spectrum ./bigsea.mp3 80 1080i_50
