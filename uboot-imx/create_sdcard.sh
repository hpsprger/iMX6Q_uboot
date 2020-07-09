#!/bin/bash

help() {

bn=`basename $0`
cat << EOF
usage $bn <option> device_node

options:
  -h        displays this help message
  -s        only get partition size
  -np         not partition.
  -f        flash android image.
EOF

}

# parse command line
moreoptions=1
node="na"
cal_only=0
flash_images=0
not_partition=0
cp_images=0
not_format_fs=0
while [ "$moreoptions" = 1 -a $# -gt 0 ]; do
  case $1 in
      -h) help; exit ;;
      -s) cal_only=1 ;;
      -f) flash_images=1 ;;
      -c) cp_images=1 ;;
      -np) not_partition=1 ;;
      -nf) not_format_fs=1 ;;
      *)  moreoptions=0; node=$1 ;;
  esac
  [ "$moreoptions" = 0 ] && [ $# -gt 1 ] && help && exit
  [ "$moreoptions" = 1 ] && shift
done

if [ ! -e ${node} ]; then
  help
  exit
fi


umount ${node}*
#擦除SD卡
dd if=/dev/zero of=${node} bs=1M count=10
sync

#烧写U-boot
dd if=u-boot.imx of=${node} bs=512 seek=2
sync

fdisk ${node} << EOF
n
p
1
20480

w
EOF
#创建分区（Linux系统）
#sfdisk --unit M ${node} <<-__EOF__
#4,,L,*
#__EOF__
sleep 1
#格式化分区（格式化为ext4）
mkfs.vfat ${node}1
#mkfs.fat -L rootfs ${node}1
## Make sure posted writes are cleaned up
sync
echo "Formatting done."

mkdir -p mount_tmp
mount ${node}1 mount_tmp
cp -v u-boot.imx mount_tmp
cp -v embedsky.ini mount_tmp
cp -v uEnv.txt mount_tmp
sync
umount mount_tmp
rm -rf mount_tmp
