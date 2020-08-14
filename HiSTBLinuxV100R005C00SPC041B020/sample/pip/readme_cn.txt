功能描述: 展示画中画功能
命令格式1:  sample_dvb_pip freq [srate] [qamtype] [audmix]
参数说明1：
    freq : 频率，单位为MHz
    srate : 符号率，单位为：Kbits/s
    qamtype : 调制方式，支持16、32、64、128、256、512，默认使用64
    audmix : 0 - 不混音；1 - 混音

命令格式2:  sample_ts_pip tsfile [audmix]
参数说明2：
    tsfile : TS流文件
    audmix : 0 - 不混音；1 - 混音

