功能描述：展示ES播放与TS播放相互切换的功能
命令格式：sample_es_ts_switch esvidfile vidtype tsfile [vo_format]
参数说明：esvidfile：es视频文件
					vidtype：es视频协议类型
					tsfile：ts视频文件
					[vo_format]：输出制式，取值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，缺省值为1080i_50
命令参考: ./sample_es_ts_switch ./vid.es h264 ./test.ts 720p_50			