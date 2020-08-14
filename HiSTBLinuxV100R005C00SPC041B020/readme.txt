Term:
	$sdk_path: SDK code path
	$chip_type: chip type
	$out_dir£º the path of output files after building the SDK

1. For  SDK usage and development environment building,please refer <Linux Development Environment User Guide> for detail. 

2. Compile SDK
	2.1 "source $sdk_path/env.sh" must be executed before compiling the SDK. 
	2.2 "cp configs/$chip_type/xx_cfg.mak cfg.mak" can use to config the SDK before compile it. customer can customize the SDK configuration by "make menuconfig".
	2.3 Customer can get usage information of compiling command by execute "make help" .
	 
3. Burn the board
	3.1 Use the "HiTool" to burn the board is recommended.
	
	3.2 Command in boot console also be supported to burn the board. below gives the demonstration .
		NAND flash:
	        For example(the value should be modified according to chipset and board): 
	        mw.b 1000000 ff 80000                       
	        tftp 1000000 fastboot-burn.bin              
	        nand erase 0 100000                         
	        nand write 1000000 0 80000                  
	        
	        SPI flash:
	        sf probe 0                         	
	        mw.b 1000000 ff 80000             	
	        tftp 1000000 fastboot-burn.bin    	
	        nand erase 0 100000               	
	        sf write 1000000 0 80000          	
	        
		eMMC flash:
		The commands for the eMMC flash are as follows:
		Help command: help mmc
		Read command : mmc read 0 ddroffset startblock blockcount
		Write command: mmc write 0 ddroffset startblock blockcount
		[Note]
		---The eMMC flash is read or written by block. A block has 512 bytes.
		---The eMMC flash uses the ext4.
		---Burning images by using the command line on the eMMC flash is too complex. The HiTool is recommended.
	        
4. Customize bootargs

	4.1 The default bootargs.bin will be generate in $out_dir/image/ according to the bootargs_xx.txt .
	    bootargs_xx.txt is in $sdk_path/configs/$chip_type/prebuilts (this path may change in future ,customer can execute "find . -name bootargs* " in $sdk_path/configs to search it) 
	    
	4.2 For bootargs customize, customer can directly modify the bootargs_xx.txt ,then use the command "mkbootargs" to generate the bootargs.bin. below gives a demonstration:
	    mkbootargs  -s 128k -r bootargs.txt  -o bootargs.bin
	    
	4.3 Customer also can set bootargs directly in boot console. 
	    such as(the parameter should be modified by customer):
	    setenv bootcmd 'nand read 1000000 c00000 800000;bootm 1000000'
      	    setenv bootargs 'mem=1G console=ttyAMA0,115200 root=/dev/mtdblock5 rootfstype=yaffs2 mtdparts=hi_sfc:1M(boot);hinand:4M(baseparam),4M(pqparam),4M(logo),8M(kernel),96M(rootfs),-(others) mmz=ddr,0,0,300M'
      	    saveenv
      	4.4 For more detail ,please refer to <Linux Development Environment User Guide>.
      	    
5. Mounting the NFS and Debugging Programs
	5.1 Config the board network use command:
		udhcpc  
		or 
		ifconfig eth0 hw ether 00:xx:xx:xx:xx:xx; ifconfig eth0 xxx.xxx.xxx.xxx netmask 255.255.xxx.0;route add default gw xxx.xxx.xxx.x;     
		
	5.2  Mount the NFS.  
		mount -t nfs -o nolock -o tcp xxx.xxx.xxx.xxx:/xxx/$sdk_dir /mnt
		
	5.3  Add the path for searching dynamic library files: /mnt/$out_dir/lib/share/.
   		export LD_LIBRARY_PATH="/mnt/$out_dir/lib/share/:$LD_LIBRARY_PATH"
   		
   		
   	5.4. Replace the .ko file.
   		cd /mnt/$out_dir/kmod
   		rmmod xxx.ko
   		insmod xxx.ko

	5.5 Run the sample.
   		cd /mnt/$out_dir/obj/sample/
   		./sample_xxx
  
6. Mounting a USB device
	6.1 Load the .ko files.
	   insmod ehci-hcd.ko
	   insmod ohci-hcd.ko
	   If the NTFS is to be supported, load ufsd.ko.
	   insmod ufsd.ko
	6.2 Mount the USB device.
	   If the FAT32 is used, run the following command:
	   mount -t vfat /dev/sda /usb
	   If the NTFS is used, run the following command:
	   mount -t ufsd /dev/sda /usb

7. SDK developing and debug, please refer to <HMS Development Guide> and <HMS Debugging Guide>	  
8. HiLoader: 	please refer to  <HiLoader Development Guide>
9. CAS : 	please refer to <Advanced CA Development Guide>