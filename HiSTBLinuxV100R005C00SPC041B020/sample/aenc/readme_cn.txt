1、sample_aenc
功能描述: 展示编码功能(上层应用提供音频流输入)
命令格式: ./sample_aenc infileName inChannel insamplerate outfileName outfileType
参数说明：infileName    输入的音源文件名称 (pcm/wav格式)
          inChannel     输入声道数 (仅支持1、2) 
          insamplerate  输入采样率 (输入采样率范围：8000-192000) 
          outfileName   编码后数据保存的文件名称
          outfileType   输出文件编码类型 (支持aac/g711/amrnb/amrwb)
例    子: ./sample_aenc ./boys_48kHz_stereo_16.wav 2 48000 ./boys.aac aac
注意事项：需要指定输入的音源通道数和采样率，采样位宽仅支持16bit

2、sample_aenc_track
功能描述: 展示aac编码功能(aenc与track的绑定)
命令格式: ./sample_aenc_track infile intype outfile outtype
参数说明：infile    输入的音源文件名称
          intype    输入的音源类型
          outfile   编码后数据保存的文件名称
          outtype   编码后保存的文件类型 
例    子: ./sample_aenc_track ./qgw.mp3 mp3 ./qgw.aac aac
注意事项：仅支持输入的音源类型aac/mp3/dts/dra/mlp/pcm/ddp(ac3/eac3)
