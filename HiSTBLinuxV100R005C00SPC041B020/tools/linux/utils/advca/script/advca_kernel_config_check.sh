#!/bin/sh
if [ $# -ne 1 ]
then
    echo -e "this script is for checking whether kernel configuration meet advca's requirements"
    echo -e "usage: ./advca_kernel_config_check.sh  kernel_cfgfile"
    echo -e "for example: "
    echo "./advca_kernel_config_check.sh source/kernel/linux-3.18.y/arch/arm/configs/hi3716mv410-ca-final_defconfig"
    exit 1
fi

config_error=0
file=$1
echo kernel config file is "$file"
echo check whether the configuration is right
echo begin
echo ""


#==============enabled configuration================#
enable=" CONFIG_CC_STACKPROTECTOR              "
enable+=" CONFIG_CC_STACKPROTECTOR_REGULAR     "
enable+=" CONFIG_STRICT_DEVMEM                 "
enable+=" CONFIG_STRIP_ASM_SYMS                 "
enable+=" CONFIG_DEBUG_MEMORY_INIT              "
enable+=" CONFIG_DEBUG_SET_MODULE_RONX           "
enable+=" CONFIG_USB_OTG_BLACKLIST_HUB   "


disable=" CONFIG_BT "
disable+=" CONFIG_FIREWIRE "
disable+=" CONFIG_IRDA "
disable+=" CONFIG_SH_IRDA "
disable+=" CONFIG_USB_IRDA "
disable+=" CONFIG_PROFILING "
disable+=" CONFIG_KALLSYMS "
disable+=" CONFIG_ASHMEM "
disable+=" CONFIG_SWAP "
disable+=" CONFIG_CRASH_DUMP "

#Kernel hacking
#*DEBUG*
disable+=" DEBUG    	 "
disable+=" CONFIG_READABLE_ASM	 "
disable+=" CONFIG_FTRACE     	 "
disable+=" CONFIG_UNUSED_SYMBOLS "
disable+=" STATUS    	 "

#Debugging
disable+=" CONFIG_KGDB    	 "
disable+=" CONFIG_KPROBE   	 "
disable+=" CONFIG_IKCONFIG 	 "
disable+=" PRINTK	 "

#Networking feature
disable+=" CONFIG_WAN    	 "
disable+=" CONFIG_ATM   	 "
disable+=" CONFIG_DECNET 	 "
disable+=" CONFIG_IPX	 	"
disable+=" CONFIG_DCB    	 "
disable+=" CONFIG_BATMAN   	 "
disable+=" CONFIG_BATMAN_ADV   	 "
disable+=" CONFIG_NET_PKTGEN 	 "

disable+=" CONFIG_ATALK    	 "
disable+=" CONFIG_PHONET   	 "
disable+=" CONFIG_BRIDGE 	 "
disable+=" CONFIG_INET_DIAG	 "
disable+=" CONFIG_USB_NET    	 "
disable+=" CONFIG_USB_SERIAL   	 "
disable+=" CONFIG_USB_WUSB   	 "
disable+=" CONFIG_USB_WDM	 "
disable+=" CONFIG_NOP_USB_XCEIV   	 "
disable+=" CONFIG_ARCNET	 "

#Security options
disable+="  CONFIG_KEYS_DEBUG_PROC_KEYS  "

#Kernel features
disable+=" CONFIG_VT    	 "
disable+=" CONFIG_UNIX98_PTYS   	 "
disable+=" CONFIG_SYSFS_DEPRECATED 	 "
disable+=" CONSOLE	 "



disable+=" CONFIG_STACKTRACE_SUPPORT "
disable+=" CONFIG_EXPERIMENTAL       "
disable+=" CONFIG_DEBUG_KERNEL       "
disable+=" CONFIG_NFS_FS             "
disable+=" CONFIG_NFS_V3             "
disable+=" CONFIG_NFS_COMMON         "
disable+=" CONFIG_HIBERNATION_NVS    "
disable+=" CONFIG_SERIAL_CORE            "
disable+=" CONFIG_SERIAL_CORE_CONSOLE    "
disable+=" CONFIG_SERIAL_STM_ASC         "
disable+=" CONFIG_SERIAL_STM_ASC_CONSOLE "
disable+=" CONFIG_DUMMY_CONSOLE          "
disable+=" CONFIG_ENABLE_WARN_DEPRECATED "
disable+=" CONFIG_SYSVIPC_SYSCTL         "
disable+=" CONFIG_SYSCTL_SYSCALL         "
disable+=" CONFIG_PROC_KCORE         "    
disable+=" CONFIG_PROC_SYSCTL            "
disable+=" CONFIG_PROC_PAGE_MONITOR     "
disable+=" CONFIG_HAVE_KPTRACE           "
disable+=" CONFIG_USB_MON                "
disable+=" CONFIG_ISO9660_FS             "
disable+=" CONFIG_JOLIET                 "
disable+=" CONFIG_UDF_FS                 "
disable+=" CONFIG_UDF_NLS                "
disable+=" CONFIG_CHR_DEV_SG             "

disable+=" CONFIG_SCSI           "
disable+=" CONFIG_XZ_DEC         "
disable+=" MODULE_FORCE_LOAD     "
disable+=" ELF_CORE              "

#Device drivers
disable+=" CONFIG_USB_ANNOUNCE "
disable+=" CONFIG_USB_OTG      "
disable+=" CONFIG_USB_GADGET  "
disable+=" CONFIG_USB_MON     "
disable+=" CONFIG_USB_TMC      "
disable+=" CONFIG_USB_ACM     "
disable+=" CONFIG_USB_PRINTER "
disable+=" CONFIG_ANDRIOD_LOGGER "

#Kernel debug
disable+=" CONFIG_HAVE_KPROBES "
disable+=" CONFIG_PERF_USE_VMALLOC "

disable+=" CONFIG_OABI_COMPAT           "  
disable+=" CONFIG_SQUASHFS_LZO          "  
disable+=" CONFIG_HAVE_MACH_CLKDEV      "  
disable+=" CONFIG_XZ_DEC_X86            "  
disable+=" CONFIG_XZ_DEC_POWERPC        "  
disable+=" CONFIG_XZ_DEC_IA64           "  
disable+=" CONFIG_XZ_DEC_ARMTHUMB       "  
disable+=" CONFIG_XZ_DEC_SPARC          "   
disable+=" CONFIG_DECOMPRESS_LZO          "
disable+=" CONFIG_LZO_COMPRESS            "
disable+=" CONFIG_LZO_DECOMPRESS         "
disable+=" CONFIG_CRYPTO_LZO              "
disable+=" CONFIG_RD_LZO                  "
disable+=" CONFIG_RD_GZIP                 "
disable+=" CONFIG_RD_BZIP2                "
disable+=" CONFIG_RD_XZ                   " 
disable+=" CONFIG_DECOMPRESS_GZIP         "
disable+=" CONFIG_DECOMPRESS_BZIP2        "
disable+=" CONFIG_DECOMPRESS_XZ          "

#the following 3 configuration should be disabled in MV310 SES ,however, it be selected by arm=y, it is difficult to disabled it ,
#if customer want disabled anyway ,can deleted it manul.

#disable+=" CONFIG_HAVE_KERNEL_GZIP        "
#disable+=" CONFIG_HAVE_KERNEL_XZ          "
#disable+=" CONFIG_HAVE_KERNEL_LZO         "

disable+=" CONFIG_HINFC610_DBG_NAND_DEBUG "
disable+=" CONFIG_CMDLINE_FROM_BOOTLOADER "      
disable+=" CONFIG_ALLOW_DEV_COREDUMP         "
disable+=" CONFIG_APP_CAPABLE                "
disable+=" CONFIG_USB_OHCI_LITTLE_ENDIAN    "
disable+=" CONFIG_LBDAF                      "
disable+=" CONFIG_FW_LOADER                  "
disable+=" CONFIG_FIRMWARE_IN_KERNEL         "
disable+=" CONFIG_EXTRA_FIRMWARE             "
disable+=" CONFIG_SUPPORT_SRAM_MANAGER       " 
disable+=" CONFIG_SRAM_PHYS_START 		"
disable+=" CONFIG_SRAM_PHYS_SIZE         	"
disable+=" CONFIG_CC_STACKPROTECTOR_NONE     "
disable+=" CONFIG_EFI_PARTITION              "
disable+=" CONFIG_FS_POSIX_ACL               "
disable+=" CONFIG_TMPFS_XATTR                "
disable+=" CONFIG_HINFC610_DBG              "
disable+=" CONFIG_BLK_CMDLINE_PARSER         "
disable+=" CONFIG_CMDLINE_PARTITION         "

disable+=" CONFIG_GENERIC_BUG "
disable+=" CONFIG_IRQ_WORK "
                 
function check_disabled()
{
	if grep $1 $file > /dev/null; then 
		if grep  $1 $file | grep -rnw -E '#|is not set' >/dev/null;then
			echo $1 be disabled 
		else
			echo warning ! $1 is not be disabled
			config_error=1 
    		fi 
	fi	
}

function check_enabled()
{

	if grep  $1 $file | grep "=y" >/dev/null;then
		echo $1 be enabled 
	else
		echo warning ! $1 is not be enabled
		config_error=1 
	fi 
	
}

for ff in $disable ; do 
	check_disabled $ff
done

for ff in $enable ; do 
	check_enabled $ff
done

echo ""
echo check kernel configuration done
if [ 1 -eq $config_error ] ;then
	echo error ! configuration not meet advca requirement!
else
	echo right! configuration  meet advca requirement!
fi
    		
exit $config_error