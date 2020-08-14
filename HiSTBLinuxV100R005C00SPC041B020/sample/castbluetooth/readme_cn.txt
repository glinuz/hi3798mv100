功能：
     1、演示Cast捞数据后使用蓝牙耳机播放音频数据基本功能。
	 2、使用蓝牙遥控器，蓝牙鼠标配对，自动连接基本功能。
     
依赖：
     使用sample前，需要确保bluetooth相关服务已经启动。

用法：
     1)在 /etc/init.d/S90modules 目录下添加：. bluez_init
	    #!/bin/sh
	    telnetd&
	    cd /kmod
	    ./load
	    . bluez_init
		
     2)在LD_LIBRARY_PATH定义中增加ALSA及蓝牙库路径/usr/share/bluetooth/lib/
     	 export LD_LIBRARY_PATH="/usr/share/bluetooth/lib/:$LD_LIBRARY_PATH"
      3)使用sample_esplay播放歌曲，用于Cast捞取音频数据。

     4)执行./sample_bluetooth配置蓝牙耳机。
       示例：./sample_bluetooth 
	  Please select the bluetooth function:
      0 -- Exit.
      1 -- Pair.
      2 -- Paired devices.
      3 -- Connected headset.
      4 -- Adapter control.
     其中：选择1进行蓝牙耳机的配对。选择2查看已经配对成功的蓝牙耳机设备的状态。选择3查看已经处于连接状态的蓝牙耳机设备。
	 选择 1，列出开始配对蓝牙耳机设备以及蓝牙适配器的状态。输入s开始扫描设备。此时蓝牙耳机设备应设置于配对状态。扫描完毕后会列出扫描到的蓝牙设备。
	 Sample_BT_Pairing(), 154: Pair bluetooth device start.
	 Sample_BT_Pairing(), 164: Get bluetooth adapter success, hci0  00:18:E4:0A:4A:E2
	 Input char 's' to scan the bluetooth device.
	 s
	 Sample_BT_ScanDevice(), 85: Scan bluetooth device start.
	 Scan bluetooth device success, 3 devices are available, please select:
	 0  BC:14:EF:02:49:2B  [unknown]  not paired
	 1  00:0A:94:80:21:FC  LB-08  paired
     2  00:0C:8A:B3:AC:EC  Bose Mini SoundLink  paired
	 选择需要配对的蓝牙设备。选择1，表示配对00:0A:94:80:21:FC设备。输入密码0000.提示配对成功后并返回第一级目录。
	 1
     Sample_BT_Pairing(), 178: Find bluetooth device , 00:0A:94:80:21:FC  LB-08
	 please input the bluetooth device password:
     0000
     Sample_BT_Pairing(), 190: Pair the bluetooth device success.

     Please select the bluetooth function:
      0 -- Exit.
      1 -- Pair.
      2 -- Paired devices.
      3 -- Connected headset.
      4 -- Adapter control.
      选择2列出已经配对成功的设备，选择0表示要和00:0A:94:80:21:FC这个设备进行连接。然后选择2进行连接。
	  2
      Sample_BT_PairedDevices(), 245: Get bluetooth adapter success, hci0  00:18:E4:0A:4A:E2
      Get paired bluetooth device success, 2 devices are available, please select:
      0: addr [00:0A:94:80:21:FC], name [LB-08], class [3], status [0]
      1: addr [00:0C:8A:B3:AC:EC], name [Bose Mini SoundLink], class [3], status [0]
	  0
	  Please select the operation :
      0 -- Exit.
      1 -- Status.
      2 -- Connect.
      3 -- Disconnect.
      4 -- Remove.
      5 -- SetTrust。
	  
	  2
	  input: 00:0A:94:80:21:FC as /devices/virtual/input/input0
	  arrd 00:0A:94:80:21:FC
	  Sample_BT_PairedDevices(), 334: Connect bluetooth device success.
	  Please select the operation :
      0 -- Exit.
      1 -- Status.
      2 -- Connect.
      3 -- Disconnect.
      4 -- Remove.
	  5 -- SetTrust。
	  
	  连接成功后，输入0返回第一级目录。选择2查看已经连接的设备。此时也可以选择将连接的哪个设备断开。
	  0
	  
	  如果是蓝牙鼠标和遥控器，可输入5, 设置设备为受信任设备，
	  当遥控器或鼠标待机后唤醒，关闭后开启，机顶盒重启等，遥控器或鼠标会自动连接，无需软件操作。
	  
     Please select the bluetooth function:
      0 -- Exit.
      1 -- Pair.
      2 -- Paired devices.
      3 -- Connected headset.
      4 -- Adapter control.

	  2
	  Sample_BT_PairedDevices(), 245: Get bluetooth adapter success, hci0  00:18:E4:0A:4A:E2
      Get paired bluetooth device success, 2 devices are available, please select:
      0: addr [00:0A:94:80:21:FC], name [LB-08], status [0]
      1: addr [00:0C:8A:B3:AC:EC], name [Bose Mini SoundLink], status [1]
	  蓝牙协议支持同时连接多个设备。可以输入3查看已经连接设备。
	  3
	  adapter : 00:18:E4:0A:4A:E2 hci0
      headset : 00:0C:8A:B3:AC:EC Bose Mini SoundLink

      Please select the bluetooth function:
      0 -- Exit.
      1 -- Pair.
      2 -- Paired devices.
      3 -- Connected headset.
      4 -- Adapter control.
	  
  
    5)执行./sample_castbluetooth
	  使用蓝牙耳机播放歌曲。此sample支持两个蓝牙设备切换发声，执行此sample前，先将两个蓝牙耳机连接好。
	  示例：./sample_castbluetooth 
	   输入"K"开始播放。输入"S"停止播放。再输入"c"切换蓝牙设备。再输入"K"开始播放，此时另一个蓝牙设备出声。
	        # ./sample_castbluetooth 
		    please input c to change config
		    please input the k to start cast!, s to stop and deinit
			k
			please input c to change config
			please input the k to start cast!, s to stop and deinit
			s
			please input c to change config
			please input the k to start cast!, s to stop and deinit
			c
			pcm.bluetooth{
				type bluetooth
				device 00:0A:94:80:21:FC
				}
		   please input c to change config
		   please input the k to start cast!, s to stop and deinit
		   k
		   please input c to change config
		   please input the k to start cast!, s to stop and deinit
    

注意：
	1、执行用例前应保证蓝牙服务已经开启。使用ps -ef查看是否有以下进程。
	   1000      1346     1  0 00:00 ?        00:00:00 dbus-daemon --config-file=/usr/share/bluetooth/etc/dbus-1/system.conf
       root      1348     1  0 00:00 ?        00:00:00 bluetoothd --udev
    2、sample_castbluetooth仅支持两个蓝牙设备的切换。
	3、sample_castbluetooth通过更新ALSA的配置文件来达到切换蓝牙设备的目的。
	   第一次连接成功的蓝牙设备MAC地址会更新在/etc/bluetooth/FirstConf/asound.conf。
	   第二次连接成功的蓝牙设备MAC地址会更新在/etc/bluetooth/SecondConf/asound.conf。
	   第三次连接成功的蓝牙设备MAC地址也会更新在/etc/bluetooth/FirstConf/asound.conf。从而覆盖第一次连接成功的Mac地址。
	   



	

    
    