功能描述：展示DVB播放功能
命令格式：sample_dvbplay freq srate [qamtype or polarization] [vo_format]
参数说明：freq：锁频频点(MHz)
          srate: 符号率(kbps)
          [qamtype or polarization]: 可选项，DVB-C类型时为qamtype，表示QAM调制方式；DVB-S类型时为polarization，表示极化方向
          [vo_format]：输出制式，取值为1080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50，缺省值为1080i_50
命令参考：./sample_dvbplay 618 6875 64 720p_50  