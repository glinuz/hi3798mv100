sample_omxvdec使用方法：

一、目的 
     测试验证OMX解码通道，可以测试OMX、vfmw、vpss整个通道是否能够正常工作，作用类似于sample_esplay。  

	 
二、使用方法

Linux环境：

1、linux环境下，系统默认不加载OMX模块的KO,所以需要手动加载：insmod /kmod/hi_omxvdec.ko, 然后在 Code/sample/omxvdec
   路径下，直接 make， 可以得到sample_omxvdec的测试程序。

2、在串口输入：
	./sample_omxvdec  filename1 filename2 ...
	
	回车确定后，需要选择测试码流的协议类型，如果上面输入的测试文件有多个，则需要多次选择对应测试码流的协议类型。
	
Android环境：	
	android环境的omx驱动默认编译进kernel，且sample_omxvdec命令已经放在 /usr/bin 路径下，所以可以直接运行：
	
	sample_omxvdec  filename1 filename2 ...
	 
	 
三、验证标准
  
    由于sample_omxvdec无法播放画面，所以不能通过看播放画面来判断是否正确解码。
如果测试结束后，有打印：
	
	**********...TEST SUCCESSFULL...*********
	
则表明基本通道正常，但是不能保证每一帧是否解码都正确。可以通过存yuv的方式来查看解码情况。
	
OMX存yuv方法：
	
	测试前：  echo save_yuv start >/proc/msp/omxvdec	
	测试后：  echo save_yuv stop >/proc/msp/omxvdec
 	默认存储在/mnt路径下。
	  