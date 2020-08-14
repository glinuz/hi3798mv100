Purpose: to demonstrate how to use the interface related to standby.
Command: sample_pmoc TimerCount [Display] [Running Count] [Scene] [IR Type] [Keyled Type] [Reset] [GPIO No.]
Parameter:
		TimerCount: 0: Disable timer wake up; other: Wake up time(in Second).
		Display: 0: no display; 1: display number; 2: display time.Default value is 2.
		Running Count: the number of running.Default value is 1.
		Scene: 0: standard; 1: eth forward.Default value is 0. 
		IR Type: 0:nec simple; 1:tc9012; 2:nec full; 3:sony_12bit; 4:raw.Default vaule is 0.
		Keyled Type: 0: 74hc164; 1: pt6961; 2: ct1642; 3: pt6964; 4: fd650.Default vaule is 0.
		Eth & USB Type: 0: Disable; 1: USB Wakeup; 2: Single packet; 3: Magic Packet; 4: Frame Filter.Default vaule is 0.
		Reset: 0: resume to ddr; 1: resume to reset system.
		GPIO No.: 0: disable gpio wake up ; other: gpio wake up number.
				 
Notice: 1.In ethernet fowrward scene, make sure power up two ethernet ports before enter standby status.
        2.The packet data in ethernet wake-up mode of sample is in the file named "eth_package"(Please set the MAC address of the test board to 00:71:83:05:08:13).

