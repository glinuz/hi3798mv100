功能描述: 展示基本录音功能
命令格式: sample_ai file_size(MB)   file_name
参数说明：file_size 录音数据保存最大容量
          file_name 录音数据保存文件名称      
例    子: sample_ai  100   /mnt/ai.pcm
注意事项：确认外置codec连接及设置正确


功能描述: 展示录音通道跟编码绑定功能
命令格式: sample_ai_aenc aencfile aenctype
参数说明：aencfile 编码后数据保存文件名称
          aenctype 编码器类型      
例    子: sample_ai_aenc  /mnt/ai.aac  aac
注意事项：确认外置codec连接及设置正确


功能描述: 展示录音直通播放功能
命令格式: sample_ai_ao      
例    子: sample_ai_ao
注意事项：确认外置codec连接及设置正确

功能描述: 展示SLIC录音直通播放且同时捞获录音数据功能【需要打开SLIC选项编译开关与SPI驱动选项编译开关】
命令格式: sample_ai_ao_slic file_size(MB)   file_name
参数说明：file_size 录音数据保存最大容量
          file_name 录音数据保存文件名称      
例    子: sample_ai_ao_slic  100   /mnt/slic.pcm
注意事项：确认外置slic codec连接及设置正确