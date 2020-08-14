
             本地播放sample使用说明

1、播放命令：
可执行程序名称sample_localplay，播放一个文件，如xxx.mp3，执行命令如下：
  ./sample_localplay /filepath/xxx.mp3

可选择命令如下：
  -s 指定外挂字幕文件，操作如下，
  ./sample_localplay /filepath/xxx.mp3 -s /filepath/xxx1.lrc,/filepath/xxx2.lrc
  
播放一个蓝光码流(蓝光目录）：
  ./sample_localplay bluray:/蓝光目录路径/?playlist=-1
  
播放一个蓝光码流（ISO格式）,分两步进行：
->挂载ISO
  mkdir /tmp/avatar/;mount -o loop -t udf /mnt/usb/3DBLURAYISO.COM_AVATAR_3D.iso  /tmp/avatar/
->播放挂载后的蓝光目录
  ./sample_localplay bluray:/tmp/avatar/?playlist=-1

2、播放过程中常用操作命令：
    h: 输出帮助信息
    play: 播放
    pause: 暂停
    seek: seek操作，seek time_offset(离文件开头的时间，单位：毫秒，milliseconds）
    stop: 停止
    resume: 恢复
    bw: 快退，bw 2 2倍速度快退
    ff: 快进，ff 2 2倍速度快进
    info: 输出当前播放器信息
	set sync 3000 0 0:设置视频流同步属性滞后3000ms(视频pts额外增加3000ms)
    set sync 0 3000 0:设置音频流同步属性滞后3000ms(音频pts额外增加3000ms)
	set sync 0 0 3000:设置字幕流同步属性滞后3000ms(字幕pts额外增加3000ms)
	set mute:设置静音
	set vol  20：设置音量等级20(0~100)
	set vmode 0: 设置HDMI视频模式（0-2D模式，1-3D FramePacking,2-3D SideBySide, 3-3D TopAndBottom,具体值请参考相关头文件）
	set id 0 0 1 0:多码流切换，4个数字依次表示节目ID，视频流ID，音频流ID,字幕流ID。
	
3、HiPlayer（非高安版本）支持playready，在使用时需要注意以下几点
   1> 使用playready组件之前，用户必须向微软申请取得PlayReady商用设备证书。
      没有取得PlayReady设备证书是无法使用PlayReady组件的。
	  
   2> 默认的playready证书放置路径为/system/bin/prpd/，将证书放到该目录下时，系统会自动读取证书，
      否则需要像下面这样主动指定证书路径：
	  ./sample_localplay ./xxxx.pyv --drm trans=1 mimetype=application/vnd.ms-playready DrmPath=./prpd/,LocalStoreFile=data.localstore
	  注意：./prpd最后一定要有符号'/'。
	        ./xxxx.pyv是drm加密的视频文件。
						
   3> 需要注册libHiplayreadyEngine.so库。将libHiplayreadyEngine.so放到/system/lib/drm/目录下时，HiPlayer会自动注册该库文件，
      否则需要像下边这样主动指明该库文件的存放路径：
	  ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so
	 
   4> 测试使用的各种playready音视频文件及cms文件可从微软的官方网站获取。
	  http://playready.directtaps.net/pr/public/rightstests.aspx?detail=thO/caiU10mTrzhLjSR06g==
	  
   5> 常用的测试：
      普通测试：
	     ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so
		 
	  domain测试（加入domain)：需要在网站上下载具有domain证书的码流
	      ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=JoinDomain.cms
	  domain测试（离开domain)：需要在网站上下载具有domain证书的码流
	      ./sample_localplay ./xxxx.pyv --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=LeaveDomain.cms
		  
	  envelope测试：
	      ./sample_localplay ./envelope_test.asf --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=102 mimetype=application/vnd.ms-playready EnvelopFile=envelope_test.pye,\
		  EnvelopOutputFile=envelope_test.asf 
		 其中，envelope_test.asf是envelope_test.pye经过解密后生成的文件
	   
	  metering测试：需要在网站上下载具有metering证书的码流
	      metering功能用于限制播放次数。
		  ./sample_localplay xxxx.asf --drm InstallDrmEngine=/usr/lib/libHiplayreadyEngine.so --drm trans=100 mimetype=application/vnd.ms-playready InitiatorFile=Metering.cms
		  在解密该码流后再次运行上述命令来向服务器发送Metering报告
	  


	  
	
	
