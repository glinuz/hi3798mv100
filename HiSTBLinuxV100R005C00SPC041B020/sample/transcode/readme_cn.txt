功能描述: 展示音频和视频的转码功能
命令格式1: sample_dvb_transcode freq srate [qamtype or polarization] [vo_format]
参数说明1:
    freq: 频点，单位为：MHz
    srate: 符号率，单位为：Kbits/s
    qamtype or polarization:
        Cable时表示QAM调制方式：支持16、32、64、128、256、512，默认使用64
        卫星时表示极化方式：0 - 水平极化；1 - 垂直极化
    vo_format: 支持1080P_60、1080P_50、1080i_60、1080i_50、720P_60、720P_50，默认使用1080i_50。

命令格式2: sample_ts_transcode file [vo_format]
参数说明2:
    file: TS流文件。
    vo_format: 支持1080P_60、1080P_50、1080i_60、1080i_50、720P_60、720P_50，默认使用1080i_50。
