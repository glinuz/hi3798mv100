#!/bin/sh
if [ $# -ne 1 ]
then
    echo -e "this script is for checking whether unsafe function exist in code"
    echo -e "usage: ./advca_unsafe_function_check.sh  sdk_dir"
    echo -e "        for example: ./advca_unsafe_function_check.sh /home/v1r2/Code"
    exit 1
fi
echo "==============check unsafe function for advca begin==================="
echo "check dir is " $1
exist=0

sdk_dir=$1
component_dir=$1/source/component
msp_dir=$1/source/msp
rootfs_dir=$1/source/rootfs
product_dir=$sdk_dir/source/boot/product

grep_path=" $sdk_dir/source/common "
grep_path+=" $component_dir"
grep_path+=" $msp_dir "
grep_path+=" $product_dir "

grep_flag=" -rnw  --include=*.c --exclude=Makefile --exclude=*.h  --exclude=*.o --exclude=*.a --exclude=*.so --exclude-dir=.svn "
#grep_contents=" 'gets|strcpy|strcmp|strcat|sprintf|scanf|sscanf|fscanf|vfscanf|vsprintf|vscanf|vsscanf|realpath|syslog|getopt|getopt_long|getpass|popen' "
#ICSL system checking realloc alloca passwd url 
grep_contents=" 'gets|strcpy|strcmp|strcat|sprintf|realpath|syslog|getopt|getopt_long|getpass|popen|realloc\(|alloca\(|system\(|passwd|url' "

exclude=" --exclude-dir=$component_dir/3g "
exclude+=" --exclude-dir=$component_dir/alsa "
exclude+=" --exclude-dir=$component_dir/bluetooth "
exclude+=" --exclude-dir=$component_dir/ciplus "
exclude+=" --exclude-dir=$component_dir/curl "
exclude+=" --exclude-dir=$component_dir/directfb "
exclude+=" --exclude-dir=$component_dir/exfat "
exclude+=" --exclude-dir=$component_dir/karaoke "
exclude+=" --exclude-dir=$component_dir/freetype "
exclude+=" --exclude-dir=$component_dir/usbphone "
exclude+=" --exclude-dir=$component_dir/wifi "
exclude+=" --exclude-dir=$component_dir/zlib "
exclude+=" --exclude-dir=$component_dir/voip "
exclude+=" --exclude-dir=$component_dir/advca/vmx "
exclude+=" --exclude-dir=$component_dir/playready/playready2.0/src/private "
exclude+=" --exclude-dir=$component_dir/playready/playready3.0/src/PK30 "

exclude+=" --exclude-dir=$component_dir/widevine/third_party "
exclude+=" --exclude-dir=$component_dir/widevine/cdm "
exclude+=" --exclude-dir=$component_dir/widevine/core "
exclude+=" --exclude-dir=$component_dir/widevine/platforms "

exclude+=" --exclude-dir=$msp_dir/api/png/libpng_for_5.0 "
exclude+=" --exclude-dir=$msp_dir/api/png/libpng "
exclude+=" --exclude-dir=$msp_dir/api/jpeg/src/src_turbo "
exclude+=" --exclude-dir=$msp_dir/api/higo/src/higo/adp/thirdpart "
exclude+=" --exclude-dir=$msp_dir/api/gpu "
exclude+=" --exclude-dir=$msp_dir/drv/gpu "
exclude+=" --exclude-dir=$msp_dir/drv/cimaxplus "
exclude+=" --exclude-dir=$msp_dir/drv/frontend "
exclude+=" --exclude-dir=$msp_dir/drv/adac "
exclude+=" --exclude-dir=$msp_dir/drv/vfmw/softlib/hwmedia_v1.1/src/hwdec/hwutil "

exclude+=" --exclude-dir=$product_dir/driver/frontend/tuner "
exclude+=" --exclude-dir=$product_dir/android "

if [  -e $sdk_dir/unsafe_function.log ];then
	rm $sdk_dir/unsafe_function.log
fi
touch $sdk_dir/unsafe_function.log
out=$sdk_dir/unsafe_function.log
 
for ff in $grep_path ; do 
    echo "grep" $ff "begin"
    grep ${grep_flag} ${exclude} -E ${grep_contents} $ff >>  $out 
    if [ 0 -eq $? ] ;then
	exist=1
    fi 
    if [ 1 -eq $exist ] ;then
	echo "warning: unsafe function be found!"
    else
    	echo "unsafe function not found"
    fi
done
echo "==============check unsafe function for advca end==================="

exit $exist
