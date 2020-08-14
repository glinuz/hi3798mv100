The sample demonstrates the USER->VENC function.

Usage:
 1) sample_chns  ./filename.yuv  Picture_Width  Picture_Height  Encode_Width  Encode_Height 
 2) sample_chns  user-defined 
 
If you use the first usage, you will creat just one venc instance.You should make sure the resolving power of the yuv file you set match with the [Picture_Width] and [Picture_Height] you set.
The parameter [Encode_Width] and [Encode_Height] can be different from the real resolving power of the yuv file. If it is different from the yuv file's resolving power, 
the Encoder will zoom out or in to encode according to the resolving power [Encode_Width][Encode_Height] whice you set.
 
If you use the second usage, you can put the  parameter based on the tips so that you can creat one more instances.
And you can alse choose encode type : h264,mjpeg and jpeg.
The output H264 stream file will store in the directory of input YUV file.
