deps_config := \
	test/dm/Kconfig \
	test/Kconfig \
	lib/rsa/Kconfig \
	lib/Kconfig \
	fs/cramfs/Kconfig \
	fs/ubifs/Kconfig \
	fs/jffs2/Kconfig \
	fs/fat/Kconfig \
	fs/reiserfs/Kconfig \
	fs/ext4/Kconfig \
	fs/Kconfig \
	drivers/thermal/Kconfig \
	drivers/crypto/fsl/Kconfig \
	drivers/crypto/Kconfig \
	drivers/dma/Kconfig \
	drivers/rtc/Kconfig \
	drivers/mmc/Kconfig \
	drivers/dfu/Kconfig \
	drivers/usb/host/Kconfig \
	drivers/usb/Kconfig \
	drivers/sound/Kconfig \
	drivers/video/Kconfig \
	drivers/watchdog/Kconfig \
	drivers/hwmon/Kconfig \
	drivers/power/Kconfig \
	drivers/gpio/Kconfig \
	drivers/spi/Kconfig \
	drivers/i2c/Kconfig \
	drivers/tpm/Kconfig \
	drivers/serial/Kconfig \
	drivers/input/Kconfig \
	drivers/net/Kconfig \
	drivers/misc/Kconfig \
	drivers/block/Kconfig \
	drivers/mtd/spi/Kconfig \
	drivers/mtd/nand/Kconfig \
	drivers/mtd/Kconfig \
	drivers/pcmcia/Kconfig \
	drivers/pci/Kconfig \
	drivers/demo/Kconfig \
	drivers/core/Kconfig \
	drivers/Kconfig \
	net/Kconfig \
	dts/Kconfig \
	common/Kconfig \
	arch/arm/Kconfig.debug \
	board/freescale/mx6sabresd/Kconfig \
	arch/arm/imx-common/Kconfig \
	arch/arm/cpu/armv7/Kconfig \
	arch/arm/cpu/armv7/mx6/Kconfig \
	arch/arm/Kconfig \
	arch/Kconfig \
	Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(UBOOTVERSION)" "2015.04"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
