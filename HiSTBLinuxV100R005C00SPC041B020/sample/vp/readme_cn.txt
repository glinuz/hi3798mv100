1. sample_vp_usbcam
功能描述：展示USB摄像头作输入源场景下，可视电话视频功能
命令格式：sample_vp_usbcam cam_format af_inet local_IP remote_IP [vo_format]
参数说明：cam_format 摄像头输出格式 取值范围 0|1 1表示YUV格式，0表示MJPEG格式
          af_inet    网络类型，取值范围 0|1 1表示IPv4， 0表示IPv6
          local_IP   本地IP地址
          remote_IP  远端IP地址
          vo_format  视频输出制式 取值范围 1080P_60|1080P_50|1080i_60|1080i_50|720P_60 可选 默认值720P_50
例    子：sample_vp_usbcam 1 1 192.168.1.2 192,168.1.3 720P_50
          sample_vp_usbcam 1 0 fe80::228:1ff:fe19:777 fe80::228:2ff:fe19:777 720p_50
注意事项：确认USB摄像头已正确连接

2. sample_vp_vi
功能描述：展示VI作输入源场景下，可视电话视频功能
命令格式：sample_vp_vi af_inet local_IP remote_IP [vo_format]
参数说明：af_inet    网络类型，取值范围 0|1 1表示IPv4， 0表示IPv6  
          local_IP   本地IP地址
          remote_IP  远端IP地址
          vo_format  视频输出制式 取值范围 1080P_60|1080P_50|1080i_60|1080i_50|720P_60 可选 默认值720P_50
例    子：sample_vp_vi 1 192.168.1.1 192,168.1.2 720P_50
          sample_vp_usbcam 0 fe80::228:1ff:fe19:777 fe80::228:2ff:fe19:777 720p_50
注意事项：参见sample/vi/readme_cn.txt注意事项