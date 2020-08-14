Function:
     To demonstrate the basic functions of the Bluetooth headset play audio data which fishing by cast
     To demonstrate the basic functions of the Bluetooth mouse/remote controller
Depend:  
     Before run sample, should the Bluetooth service is started.

Instruction:

     1)Add alsa and bluetooth libraries path(/usr/share/bluetooth/lib/) into LD_LIBRARY_PATH
     	 export LD_LIBRARY_PATH="/usr/share/bluetooth/lib/:$LD_LIBRARY_PATH"

     2) Run ./sample_voip para. 
         para: function select. 0--normal; 1--play announcement
                 Normal: Tests basic voice function when two boards are connected.
                 Play announcement: Outputs audio to the local audio device to check whether the audio device works properly.

       Sample: ./sample_voip 1. This sample is used to test the function of listening to the audio played on the local audio device.

     3) Enter the source IP address (local end) and destination IP address (remote end) in sequence, for example, 192.168.1.1.
     
     4) Enter the input and output device number.
        
     5) Enter the following characters according to instructions:
        Enter q to end the VoIP sample.
        Enter r to conduct the next cyclic test of creating and destroying a VoIP channel.
        Enter s to select the audio device.

    1)Modify the file of /etc/init.d/S90modules,Add '. bluez_init'
	 Sample:
	    #!/bin/sh
	    telnetd&
	    cd /kmod
	    ./load
	    . bluez_init
		
    2)Add alsa and bluetooth libraries path(/usr/share/bluetooth/lib/) into LD_LIBRARY_PATH
     	 export LD_LIBRARY_PATH="/usr/share/bluetooth/lib/:$LD_LIBRARY_PATH"
		 
    3)Use sample_esplay to play songs.Cast can fish audio data.


    4)Run./sample_bluetooth configuing bluetooth headset
       Sample:./sample_bluetooth 
	  Please select the bluetooth function:
      0 -- Exit.
      1 -- Pair.
      2 -- Paired devices.
      3 -- Connected headset.
      4 -- Adapter control.
      Enter 1 to pair Headset Bluetooth. Enter 2 list already paired bluetooth devices. Enter 3 list already connected the Bluetooth Headset.
	  Enter 1 and then enter the s began scanning equipment. The Bluetooth Headset shall be in the paired state. It will list Bluetooth devices after scaning finish.
	  
	  Sample_BT_Pairing(), 154: Pair bluetooth device start.
	  Sample_BT_Pairing(), 164: Get bluetooth adapter success, hci0  00:18:E4:0A:4A:E2
	  Input char 's' to scan the bluetooth device.
	  s
	  Sample_BT_ScanDevice(), 85: Scan bluetooth device start.
	  Scan bluetooth device success, 3 devices are available, please select:
	  0  BC:14:EF:02:49:2B  [unknown]  not paired
	  1  00:0A:94:80:21:FC  LB-08  paired
      2  00:0C:8A:B3:AC:EC  Bose Mini SoundLink  paired
	  
	  After list bluetooth headset we can select paired bluetooth devices. Choose 1 paired 00:0A:94:80:21:FC equipment. 
	  It will indicates the pairing is successful after enter the password 0000.And then returns the first level directory.

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
	  5 -- SetTrust¡£
	 
	  Enter 2 lists have been paired equipment and Choose 0 will connect 00:0A:94:80:21:FC equipment. Then select 2 connect the 00:0A:94:80:21:FC equipment	  
      
	  2
      Sample_BT_PairedDevices(), 245: Get bluetooth adapter success, hci0  00:18:E4:0A:4A:E2
      Get paired bluetooth device success, 2 devices are available, please select:
      0: addr [00:0A:94:80:21:FC], name [LB-08], class [3],  status [0]
      1: addr [00:0C:8A:B3:AC:EC], name [Bose Mini SoundLink], class [3], status [0]
	  0
	  Please select the operation :
      0 -- Exit.
      1 -- Status.
      2 -- Connect.
      3 -- Disconnect.
      4 -- Remove.
	  5 -- SetTrust¡£
	  
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
	  After successful connection, Enter 0 returns to the first level directory. And then enter 2 to list connected devices. At this time we also can disconnected the equipment.

	  Here, when the device is mouse or remote controller, we can enter 5 (SetTrust), automatic connect will be enabled.
	  
	  0

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
	  
	  The Bluetooth protocol can connect multiple devices at the same time. You can enter 3 to view connected equipment.

	  3
	  adapter : 00:18:E4:0A:4A:E2 hci0
      headset : 00:0C:8A:B3:AC:EC Bose Mini SoundLink

      Please select the bluetooth function:
      0 -- Exit.
      1 -- Pair.
      2 -- Paired devices.
      3 -- Connected headset.
      4 -- Adapter control.
	  
  
    5)Run ./sample_castbluetooth
	  Firstly two Bluetooth Headsets has connected successful,Secondly,run the sample to play the songs using Bluetooth Headset. And also can switch between the rwo device.
	 
	  Sample:./sample_castbluetooth 
      Enter "K" to start playing. And the enter "S" to stop playing. Enter "C" switching Bluetooth devices. Enter "K" to start playing, while another Bluetooth device.
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
    

Note£º
	1¡¢Confirmation the Bluetooth services have been opened. Use PS -ef to check the information if there is the following process.
	   1000      1346     1  0 00:00 ?        00:00:00 dbus-daemon --config-file=/usr/share/bluetooth/etc/dbus-1/system.conf
       root      1348     1  0 00:00 ?        00:00:00 bluetoothd --udev
    2¡¢sample_castbluetooth support only two Bluetooth devices switch.
	3¡¢sample_castbluetooth To achieve switching Bluetooth devices through change the ALSA configuration file
       The MAC address of first connected Bluetooth device updated in ¡®etc/bluetooth/FirstConf/asound.conf¡¯
	   The MAC address of second connected Bluetooth device updated in ¡®/etc/bluetooth/SecondConf/asound.conf¡¯
	   The MAC address of Third connected Bluetooth device will cover the Mac of the first bluetooth device in ¡®etc/bluetooth/FirstConf/asound.conf¡¯
     

