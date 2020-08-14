功能描述：展示本地TS流播放功能
命令格式：sample_tsplay file [vo_format]
参数说明：file：ts文件路径
          [vo_format]：输出制式，取值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，缺省值为1080i_50
命令参考：sample_tsplay ./test.ts 720p_50

功能描述：展示本地TS流多音轨播放功能
命令格式：sample_tsplay_multiaud file [vo_format]
参数说明：file：ts文件路径
          [vo_format]：输出制式，取值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，缺省值为1080i_50          
命令参考：sample_tsplay_multiaud ./test.ts 720p_50
音轨切换：播放成功后输入命令"c"，展示音轨切换功能

功能描述：展示指定音视频PID节目，本地TS流播放功能
命令格式：sample_tsplay_pid file vo_format vpid vtype apid atype
参数说明：file：ts文件路径
          vo_format：输出制式，取值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，缺省值为1080i_50
          vpid: 视频pid
          vtype: 视频协议类型，取值为mpeg2，mpeg4，h263，sor，vp6，vp6f，vp6a，h264，avs，real8，real9，vc1
          apid: 音频pid
          atype: 音频协议类型，取值为aac，mp3，dts，dra，mlp，pcm，ddp(ac3/eac3)          
命令参考：sample_tsplay_pid ./test.ts 720p_50 100 mpeg2 101 mp3      