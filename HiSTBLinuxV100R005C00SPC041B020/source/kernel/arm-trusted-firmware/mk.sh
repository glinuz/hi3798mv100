 export ARCH=arm64 CROSS_COMPILE=aarch64-histbv100-linux-

 #make all fip PLAT=hi3798cv200 BL33=/home/lyjun/build/v1r2/kernel/linux-3.18.y/arch/arm64/boot/uImage BL32=/home/lyjun/build/v1r2/Code3/pub/image/hi3798cv200/trustedcore.img DEBUG=0

make clean PLAT=hi3798cv200 DEBUG=1
make distclean PLAT=hi3798cv200 DEBUG=1
make PLAT=hi3798cv200 SPD=none BL33=/home/lyjun/build/v1r2/Code2/source/kernel/linux-3.18.y/arch/arm64/boot/uImage DEBUG=1 DISABLE_TEE=1 fip 
#make PLAT=hi3798cv200 SPD=none BL33=/home/lyjun/tftp/uImage_nonsec DEBUG=1 DISABLE_TEE=1 fip

./tools/fip_create/fip_create --dump "build/hi3798cv200/debug/fip.bin"
cp "build/hi3798cv200/debug/fip.bin" ~/tftp
