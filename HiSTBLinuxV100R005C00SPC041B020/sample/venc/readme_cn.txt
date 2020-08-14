功能描述：展示用户态送帧给编码器编码的基本流程.

命令格式:
 1)./sample_chns yuvfile_path  Picture_Width  Picture_Height  Encode_Width  Encode_Height 
 2)./sample_chns  user-defined 

参数说明：
    yuvfile_path  yuv文件路径 
    Picture_Width YUV源图像宽度     Picture_Height  YUV源图像高度
    Encode_Width  编码器输出分辨率（宽度）            
    Encode_Height 编码器输出分辨率（高度）
 
注意事项：
1、如采用第一种命令格式，只能创建一个通道。请确保输入的YUV源分辨率与实际采用的YUV文件的分辨率匹配。
2、输入参数中，编码宽度/高度 与 YUV源宽度/高度 可以不一致。当不一致时，VENC会自动进行缩放编码。   
3、命令格式2）中，user-defined是固定字符串，输入命令 ./sample_chns  user-defined 后按提示输入相关的参数：如通道个数，文件路径，源宽高，编码宽高等。
4、采用第二种命令格式，可以选择不同协议方式来编码，目前支持的协议是：h264,mjpeg,jpeg，。若选择mjpeg模式，保存文件后缀名为.mjpeg，若选择jpeg模式，则保存成一幅幅图片。
5、生成文件放在与YUV文件同级目录下。
    



