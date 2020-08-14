1. sample_vp_usbcam
Purpose:   The sample demonstrates the CAM->VENC->->RTP----Network----RTP->VDEC->VO function.
Commmand:  sample_vp_usbcam cam_format af_inet local_IP remote_IP [vo_format]
Parameter: cam_format: output format of camera, range: 0|1 1-YUV format, 0-MJPEG format
           af_inet:    Type of network, range: 0|1 1-IPv4, 0-IPv6
           local_IP:   local IP address
           remote_IP:  remote IP address
           vo_format:  video display format, range: 1080P_60|1080P_50|1080i_60|1080i_50|720P_60, optional, default as 720P_50
Example:   sample_vp_usbcam 1 1 192.168.1.2 192,168.1.3 720P_50
           sample_vp_usbcam 1 0 fe80::228:1ff:fe19:777 fe80::228:2ff:fe19:777 720p_50
Attention: Make sure the USB camera is connected correctly.

2. sample_vp_vi
Purpose:   The sample demonstrates the VI->VENC->->RTP----Network----RTP->VDEC->VO function.
Commmand:  sample_vp_vi af_inet local_IP remote_IP [vo_format]
Parameter: af_inet:    Type of network, range: 0|1 1-IPv4, 0-IPv6
           local_IP:   local IP address
           remote_IP:  remote IP address
           vo_format:  video display format, range: 1080P_60|1080P_50|1080i_60|1080i_50|720P_60, optional, default as 720P_50
Example:   sample_vp_vi 1 192.168.1.2 192,168.1.3 720P_50
           sample_vp_vi 0 fe80::228:1ff:fe19:777 fe80::228:2ff:fe19:777 720p_50
Attention: Refer to the attention part of sample/vi/readme.txt.