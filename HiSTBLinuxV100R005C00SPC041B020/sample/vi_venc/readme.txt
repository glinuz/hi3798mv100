Purpose:  The sample demonstrates the VI->VENC->VDEC->VO function. 

Usage:
sample_vivenc vencFmt [viPort] [viMode] [bEnViVo] [voFormat]
               vencFmt: An encoding type. The encoding type can be set to 0:H264, 1:H263, or 2:MPEG4. This parameter has no default value and must be specified by you. Currently, only the H264 is supported.
               viPort: VI port number. The value is 0 or 1, depending on the hardware design. If you create real vi, The VI port number must be set to 0.
               viMode: VI port mode. The port mode can be set to 0 (indicating the BT1120_576P), 1 (indicating the USBCAM YUV).
               bEnViVo: Whether to enable the VIVO loopback. 0: disabled. 1: enabled
               voFormat: VO high-definition output format: can be set to 080P_60, 1080P_50, 1080i_60, 1080i_50, 720P_60, or 720P_50. The default value is 1080i_50.
               
After you execute the sample, the encoding information is saved as a file in the current folder with the file name as venc_viXXX.h264, venc_viXXX.h263, or venc_viXXX.mp4.
If a file with the same name exists in the folder, the file replaces the existing file.

Attention: 
Refer to the attention part of sample/vi/readme.txt.