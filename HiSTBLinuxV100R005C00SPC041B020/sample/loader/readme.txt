Description
	sample_loader used to analog trigger upgrade, support OTA, USB, IP mode.
	
Command format
	> usage: sample_loader [-t trigger] [-s set] [-g get] [[command] arg].
	> command as follows:
	> sample_loader -t                  -- configure loader upgrade parameter and trigger it run.
	> sample_loader -s deviceinfo       -- configure deviceinfo.
	> sample_loader -g deviceinfo       -- get and display deviceinfo info.
	> sample_loader -s sw               -- configure software.
	> sample_loader -g sw               -- get and display software version info.

Guide
	1. Set deviceinfo
	# ./sample_loader -s deviceinfo
	> ================= Modify Device Info ===================
	> Please input new Manufacturer Identification[0x00000000] ? 0x00000003
	> Please input new Hardware Version[0x00000000] ? 0x00000001
	> Please input new Serial Number[0x00000000] ? 0x1d000021
	>
	> Device Information modify success! 
	> New Manufacturer Identification: 0x00000003
	> New Area Code:                   0x00000000
	> New Hardware Version:            0x00000001
	> New Serial Number                0x1d000021
	> ========================================================

	2. Get deviceinfo
	# ./sample_loader -g deviceinfo
	> ================== Get Device Info =====================
	> Manufacturer Identification: 0x00000003
	> Area Code:                   0x00000000
	> Hardware Version:            0x00000001
	> Serial Number                0x1d000021
	> ========================================================

	3. Set software version
	# ./sample_loader -s sw
	> ============== modify software version =================
	> Please input new software version[0x00000000] ? 0x00000001
	>
	> Software version modify success! 
	> The current version number is 0x00000001 
	> ========================================================
	
	4. Get current software version
	# ./sample_loader -g sw
	> ================ Get software version ==================
	> The current version number is 0x01010101 
	> ========================================================
	
	5. Trigger OTA Cable upgrade
	# ./sample_loader -t
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 0
	> Select upgrade type(0-OTA, 1-IP, 2-USB, 3-FS, 4-Reset) ? 0
	> Select signal type(0-CAB, 1-SAT, 2-DVB-T, 3-DVB-T2) ? 0
	> Input PID(32~8190)[0] ? 7000
	> Input Frequency(MHz)(100~900)[0] ? 302
	> Input Symbol rate(KS/s)(100~9000)[0] ? 6875
	> Select Modulation(0-16Qam,1-32Qam,2-64Qam,3-128Qam,4-256Qam)[0] ? 4
	>
	> Upgrade Parameter configuration success! 
	> Download type       : OTA
	> Signal type         : Cable
	> PID                 : 7000
	> Frequency           : 302(MHZ)
	> Symrate             : 6875(Kbps)
	> Reverse             : 0(0:False 1:True)
	> Modulation          : 256QAM
	> ========================================================
	
	6. Trigger OTA Terrestrial upgrade
	# ./sample_loader -t
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 0
	> Select upgrade type(0-OTA, 1-IP, 2-USB, 3-FS, 4-Reset) ? 0
	> Select signal type(0-CAB, 1-SAT, 2-DVB-T, 3-DVB-T2) ? 2
	> Input PID(32~8190)[0] ? 7000
	> Input Frequency(MHz)(100~900)[0] ? 302
	> Input BandWidth(KHz)(6000~9000)[0] ? 8000
	> Select Modulation(0-16Qam,1-32Qam,2-64Qam,3-128Qam,4-256Qam)[0] ? 4
	> Select Priority type, 0:no priority, 1:high priority,2:low priority[0] ? 0
	>
	> Upgrade Parameter configuration success! 
	> Download type       : OTA
	> Signal type         : Terrestrial DVB-T/T2
	> PID                 : 7000
	> Frequency           : 302(MHZ)
	> BandWidth           : 8000(KHZ)
	> Reverse             : 0(0:False 1:True)
	> Modulation          : 256QAM
	> Channel Mode        : Base channel
	> TS Priority         : No priority
	> PLPId               : 0
	> ========================================================
	
	7. Trigger OTA Satellite upgrade
	# ./sample_loader -t
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 0
	> Select upgrade type(0-OTA, 1-IP, 2-USB, 3-FS, 4-Reset) ? 0
	> Select signal type(0-CAB, 1-SAT, 2-DVB-T, 3-DVB-T2) ? 1
	> Input PID(32~8190)[0] ? 7000
	> Input Frequency(MHz)[0] ? 302
	> Input Symbol Rate(KS/s)[0] ? 6875
	> Select Polarization type(0-Horizontal,1-Vertical, 2-Left hand, 3-Right hand)[0] ? 0
	> Select LNB Power type(0-LNB Power Off, 1-LNB Power On)[0] ? 0
	> Input LNB Low Frequency(MHZ)[0] ? 5105
	> Input LNB High Frequency(MHZ)[0] ? 5705
	> Select 22K Switch(0-none, 1-0, 2-22)[0] ? 0
	> Select DiSEqC1.1 Port(0-Disable, 1-port1, 2-port2,etc)[0] ? 0
	> Select DiSEqC1.1 device level(0-1.x one way, 1-2.x two way)[0] ? 0
	> Select DiSEqC1.0 Port(0-Disable, 1-port1, 2-port2,etc)[0] ? 0
	> Select DiSEqC1.0 device level(0-1.x one way, 1-2.x two way)[0] ? 0
	>
	> Upgrade Parameter configuration success! 
	> Download type       : OTA
	> Signal type         : Satellite
	> PID                 : 7000
	> Frequency           : 302(MHZ)
	> SymbolRate          : 6875(Kbps)
	> Polarization        : Horizontal
	> ScrambleValue       : 0
	> LNB Power           : Power Off
	> LNB type            : Single LO frequency 
	> LNB Low Frequency   : 5105(MHZ)
	> LNB High Frequency  : 5705(MHZ)
	> LNB Bandtype        : C 
	> SCR number          : 0(0~7)
	> SCR IF frequency    : 0(MHZ)
	> Unicable switch port: A 
	> 22K Switch          : none
	> DiSEqC1.1           : SWITCH PORT Disable
	> DiSEqC1.1           : Level 1.x 
	> DiSEqC1.0           : SWITCH PORT Disable
	> DiSEqC1.0           : Polarization Horizontal
	> DiSEqC1.0           : 22K Switch none
	> Motortype           : none
	> USALS LocalLongitude: 0
	> USALS LocalLatitude : 0
	> USALS SatLongitude  : 0
	> USALS Angular       : 0
	> Position Level      : Level 1.x 
	> Position->Pos       : 0
	> ========================================================
	
	8. Trigger IP upgrade
	# ./sample_loader -t
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 0
	> Select upgrade type(0-OTA, 1-IP, 2-USB, 3-FS, 4-Reset) ? 1
	> Select protocol type(0-TFTP, 1-FTP, 2-HTTP) ? 2
	> Select IP type(0-static, 1-dynamic) ? 1
	> Input server IP address[0.0.0.0] ? 192.168.1.8
	> Input server port(default:80) ? 80
	> Input upgrade file name[] ? ip_update.bin
	>
	> Upgrade Parameter configuration success! 
	> Download type       : IP
	> Protocol type       : HTTP
	> IP type             : dynamic
	> server ip           : 192.168.1.8
	> server port         : 80
	> Upgrade file name   : ip_update.bin
	> ========================================================
	
	9. Trigger USB upgrade
	# ./sample_loader -t
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 0
	> Select upgrade type(0-OTA, 1-IP, 2-USB, 3-FS, 4-Reset) ? 2
	> specify upgrade file name[] ? usb_update.bin
	>
	> Upgrade Parameter configuration success! 
	> Download type       : USB
	> Upgrade Filename    : usb_update.bin
	> ========================================================
	
	10. Trigger FS upgrade
	# ./sample_loader -t
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 0
	> Select upgrade type(0-OTA, 1-IP, 2-USB, 3-FS, 4-Reset) ? 3
	> specify upgrade file name[] ? fs_update.bin
	> specify upgrade partition name[] ? rootfs
	> 
	> Upgrade Parameter configuration success! 
	> Download Type       : FS
	> Upgrade Filename    : fs_update.bin
	> Upgrade PartitonName: rootfs
	> ========================================================
		
	11. Trigger factory reset
	# ./sample_loader -t	
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 0
	> Select upgrade type(0-OTA, 1-IP, 2-USB, 3-FS, 4-Reset) ? 4
	> 
	> Upgrade Parameter configuration success! 
	> ========================================================
	
	12. Trigger Manual USB upgrade
	# ./sample_loader -t
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 2
	>
	> Manual USB upgrade has been triggered,
	> Please make sure the upgrade file named usb_update.bin 
	> has been put into the USB device, and the USB device
	> is connected properly. 
	> ========================================================

	13. Trigger EmptyFlashBurn upgrade
	# ./sample_loader -t
	> =========== Upgrade Parameter Configuration ============
	> Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn) ? 3
	>
	> Empty Flash Burn upgrade has been triggered,
	> Please make sure the upgrade file named usb_update.bin 
	> has been put into the USB device, and the USB device
	> is connected properly. 
	> ========================================================
	
	
	
