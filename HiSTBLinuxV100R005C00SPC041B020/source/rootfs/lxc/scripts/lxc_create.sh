#!/bin/sh  

# create fs directory tree
install_fs_dir()
{
	tree="\
$PREFIX/$lxc_rootfs_dir/dev \
$PREFIX/$lxc_rootfs_dir/home \
$PREFIX/$lxc_rootfs_dir/home/stb \
$PREFIX/$lxc_rootfs_dir/root \
$PREFIX/$lxc_rootfs_dir/etc \
$PREFIX/$lxc_rootfs_dir/etc/init.d \
$PREFIX/$lxc_rootfs_dir/bin \
$PREFIX/$lxc_rootfs_dir/sbin \
$PREFIX/$lxc_rootfs_dir/proc \
$PREFIX/$lxc_rootfs_dir/sys \
$PREFIX/$lxc_rootfs_dir/mnt \
$PREFIX/$lxc_rootfs_dir/tmp \
$PREFIX/$lxc_rootfs_dir/dev/pts \
$PREFIX/$lxc_rootfs_dir/lib \
$PREFIX/$lxc_rootfs_dir/usr/lib"

	mkdir -p $tree
        chmod 755 $tree

	mkdir -p $PREFIX/`dirname $lxc_config_file` 
}

# create container init script for CA debug
install_dbg_fs_scripts()
{
	cat <<EOF > $PREFIX/$lxc_rootfs_dir/etc/inittab
::sysinit:/etc/init.d/rcS
tty1::respawn:/sbin/getty -L tty1 115200 vt100
EOF
	chmod 644 $PREFIX/$lxc_rootfs_dir/etc/inittab

	cat <<EOF > $PREFIX/$lxc_rootfs_dir/etc/init.d/rcS
#!/bin/sh
EOF
	chmod 744 $PREFIX/$lxc_rootfs_dir//etc/init.d/rcS

	cat <<EOF > $PREFIX/$lxc_rootfs_dir/etc/passwd
root:x:0:0:root:/root:/bin/sh
stb:x:1000:1000:Linux User,,,:/home/stb:/bin/sh
EOF
	chmod 644 $PREFIX/$lxc_rootfs_dir/etc/passwd

	cat <<EOF > $PREFIX/$lxc_rootfs_dir/etc/group
root::0:
stb:x:1000:
EOF
	chmod 644 $PREFIX/$lxc_rootfs_dir/etc/group

	cat <<EOF > $PREFIX/$lxc_rootfs_dir/etc/shadow
root:\$1\$eF8I65UL\$7FfdLFZEBw46GPpAOYcOQ/:0:0:99999:7:::
stb:\$1\$hxuNo7w4\$t7zxgVjDAYEul3BRH9oxc/:0:0:99999:7:::
EOF
	chmod 640 $PREFIX/$lxc_rootfs_dir/etc/shadow

	# udhcpc scripts
	mkdir -p $PREFIX/$lxc_rootfs_dir/usr/share/udhcpc
	cp -r $PREFIX/usr/share/udhcpc/* $PREFIX/$lxc_rootfs_dir/usr/share/udhcpc

}


# create container init script for CA release
install_rel_fs_scripts()
{
	cat <<EOF > $PREFIX/$lxc_rootfs_dir/etc/inittab
::sysinit:/etc/init.d/rcS
EOF
	chmod 644 $PREFIX/$lxc_rootfs_dir/etc/inittab

	cat <<EOF > $PREFIX/$lxc_rootfs_dir/etc/init.d/rcS
#!/bin/sh
EOF
	chmod 744 $PREFIX/$lxc_rootfs_dir/etc/init.d/rcS

}

# install busybox 
install_busybox()
{
	# refer to  SDK cfg.mak (arm-hisiv200-linux_lxc_user.config) Busybox Config File For User 
	echo "refer to CFG_HI_BUSYBOX_CFG_USER_NAME"
}


# create and deploy lxc config file for CA debug
install_lxc_dbg_config()
{
	cat <<EOF > $PREFIX/$lxc_config_file
lxc.utsname = $name
lxc.tty = 1
lxc.pts = 1
lxc.rootfs = $lxc_rootfs_dir
lxc.mount = $lxc_config_dir/fstab
lxc.pivotdir = /tmp/lxc_putold

lxc.network.type = veth
lxc.network.flags = up 
lxc.network.link = br0
lxc.network.hwaddr = 4a:49:43:49:79:bf

lxc.cgroup.devices.deny = a
# /dev/null and zero
lxc.cgroup.devices.allow = c 1:3 rwm
lxc.cgroup.devices.allow = c 1:5 rwm
# consoles
lxc.cgroup.devices.allow = c 5:1 rwm
lxc.cgroup.devices.allow = c 5:0 rwm
lxc.cgroup.devices.allow = c 4:0 rwm
lxc.cgroup.devices.allow = c 4:1 rwm
# /dev/{,u}random
lxc.cgroup.devices.allow = c 1:9 rwm
lxc.cgroup.devices.allow = c 1:8 rwm
lxc.cgroup.devices.allow = c 136:* rwm
lxc.cgroup.devices.allow = c 5:2 rwm
# hi_* 
lxc.cgroup.devices.allow = c 218:* rwm
# fb8
lxc.cgroup.devices.allow = c 29:* rwm
# others
lxc.cgroup.devices.allow = c 10:* rwm
EOF
	
	mkdir -p $PREFIX/usr/var/lib/lxc/$name
	cp $PREFIX/$lxc_config_file $PREFIX/usr/var/lib/lxc/$name
	chmod 644 $PREFIX/usr/var/lib/lxc/$name/config

	cat <<EOF > $PREFIX/$lxc_config_dir/fstab
proc            /home/stb/proc           proc    defaults        0       0
sysfs           /home/stb/sys            sysfs   defaults        0       0
tmp             /home/stb/tmp            tmpfs   nosuid,noexec,nodev,mode=1777
/dev            /home/stb/dev            none    bind            0       0
/lib            /home/stb/lib            none    bind            0       0
EOF
	chmod 644 $PREFIX/$lxc_config_dir/fstab

}

# create and deploy lxc config file for CA release 
install_lxc_rel_config()
{
	cat <<EOF > $PREFIX/$lxc_config_file
lxc.utsname = $name
lxc.console = none
lxc.rootfs = $lxc_rootfs_dir
lxc.mount = $lxc_config_dir/fstab
lxc.pivotdir = /tmp/lxc_putold

lxc.cgroup.devices.deny = a
# /dev/null and zero
lxc.cgroup.devices.allow = c 1:3 rwm
lxc.cgroup.devices.allow = c 1:5 rwm
# consoles
lxc.cgroup.devices.allow = c 5:1 rwm
lxc.cgroup.devices.allow = c 5:0 rwm
lxc.cgroup.devices.allow = c 4:0 rwm
lxc.cgroup.devices.allow = c 4:1 rwm
# /dev/{,u}random
lxc.cgroup.devices.allow = c 1:9 rwm
lxc.cgroup.devices.allow = c 1:8 rwm
lxc.cgroup.devices.allow = c 136:* rwm
lxc.cgroup.devices.allow = c 5:2 rwm
# hi_* 
lxc.cgroup.devices.allow = c 218:* rwm
# fb8
lxc.cgroup.devices.allow = c 29:* rwm
# others
lxc.cgroup.devices.allow = c 10:* rwm
EOF
	
	mkdir -p $PREFIX/usr/var/lib/lxc/$name
	cp $PREFIX/$lxc_config_file $PREFIX/usr/var/lib/lxc/$name
	chmod 644 $PREFIX/usr/var/lib/lxc/$name/config

	cat <<EOF > $PREFIX/$lxc_config_dir/fstab
proc            /home/stb/proc           proc    defaults        0       0
sysfs           /home/stb/sys            sysfs   defaults        0       0
tmp             /home/stb/tmp            tmpfs   nosuid,noexec,nodev,mode=1777
/dev            /home/stb/dev            none    bind            0       0
EOF
	chmod 644 $PREFIX/$lxc_config_dir/fstab

}

##################################################################################
while true
do
    case "$1" in
	-n|--name)      name=$2; shift 2;;
	-p|--prefix)    PREFIX=$2; shift 2;;
        *)              break ;;
    esac
done

if [ -z "$name" ]; then
    echo "'name' parameter is required"
    exit 1
fi

lxc_config_dir=/opt/$name
lxc_config_file=$lxc_config_dir/config
lxc_rootfs_dir=/home/$name

install_fs_dir
install_busybox
if [ "$CFG_FUNCTION_DEBUG" = "y" ]; then
	install_dbg_fs_scripts
	install_lxc_dbg_config
else
	install_rel_fs_scripts
	install_lxc_rel_config
fi

