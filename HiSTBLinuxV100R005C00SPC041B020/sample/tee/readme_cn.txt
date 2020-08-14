功能描述：展示DVB安全播放功能
命令格式：sample_tee_dvbplay freq srate [qamtype or polarization] [vo_format]
参数说明：freq：锁频频点(MHz)
          srate: 符号率(kbps)
          [qamtype or polarization]: 可选项，DVB-C类型时为qamtype，表示QAM调制方式；DVB-S类型时为polarization，表示极化方向
          [vo_format]：输出制式，取值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，缺省值为1080i_50
命令参考：./sample_tee_dvbplay 618 6875 64 720p_50

功能描述：展示本地TS流安全播放功能
命令格式：sample_tee_tsplay file [vo_format]
参数说明：file：ts文件路径
          [vo_format]：输出制式，取值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，缺省值为1080i_50
命令参考：sample_tee_tsplay ./test.ts 720p_50

功能描述: 展示画中画安全播放功能（1路安全播放+1路非安全播放）
命令格式: sample_tee_tsplay_pip tsfile_0 tsfile_1 [vo_format] 
参数说明：tsfile_0：第一个ts文件路径
          tsfile_1: 第二个ts文件路径
          [vo_format]：输出制式，取值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，缺省值为1080i_50
命令参考：sample_tee_tsplay_pip ./test0.ts ./test1.ts 720p_50