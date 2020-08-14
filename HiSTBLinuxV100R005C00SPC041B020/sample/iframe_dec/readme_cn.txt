功能描述：展示I帧解码显示功能
命令格式：iframe_dec file protocol type [freq] [srate] [qamtype or polarization]
参数说明：file：I帧码流文件
          protocol: 协议类型，0-mpeg2 1-mpeg4 2-avs 3-h264
          type: 显示类型，0-显示到vo，1-显示到osd
					[freq]：可选项，锁频频点(MHz)
					[srate]：可选项，符号率(kbps)
					[qamtype or polarization]：可选项，DVB-C类型时为qamtype，表示QAM调制方式；DVB-S类型时为polarization，表示极化方向
命令参考：./sample_iframe_dec ./iframe.es 0 0 618 6875 64 