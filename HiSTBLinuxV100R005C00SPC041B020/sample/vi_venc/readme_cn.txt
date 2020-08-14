功能描述：展示VI->VENC->VDEC->VO功能
命令格式：sample_vivenc vencFmt [viPort] [viMode] [bEnViVo] [voFormat]
参数说明：
vencFmt: 编码类型。编码类型可设置为0:H264, 1:H263, or 2:MPEG4。该参数没有默认值，需要用户设置。当前只支持H264编码。
viPort: VI端口号。端口号可以为0或者1，具体取决于硬件设计。如果用户创建实体VI，那么端口号必须设置为0。
viMode: VI端口模式。端口模式可设置为0表示  BT1120_576P，1 表示USBCAM YUV。
bEnViVo: 是否使能VIVO环回。其中0表示不使能，1表示使能。
vo_format 视频输出制式 取值范围 1080P_60|1080P_50|1080i_60|1080i_50|720P_60 可选 默认值720P_50。
               
如果支持保存编码的文件，那么在执行sample后，编码信息被保存在当前文件夹，命名方式为venc_viXXX.h264, venc_viXXX.h263, or venc_viXXX.mp4。
同名文件将被覆盖。

注意事项：
参见sample/vi/readme_cn.txt注意事项