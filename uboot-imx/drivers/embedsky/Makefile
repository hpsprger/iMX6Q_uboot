
ifdef TOPDIR
include $(TOPDIR)/config.mk

LIB     := $(obj)libembedsky.module

ifneq ($(wildcard board_init.c),)
obj-${CONFIG_TQ335X} += board/tq3358/tq335x_init.o
obj-${CONFIG_TQ335X} += board/tq3358/modify_335x.o
obj-${CONFIG_TQ335X} += board/tq3358/update_tq335x.o
#obj-${CONFIG_TQ335X} += board/tq3358/cmd_menu.o
obj-${CONFIG_TQ335X} += board/tq3358/cmd_bmp.o

obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/tq335x_init.o
obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/modify_335x.o
obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/update_tq335x.o
#obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/cmd_menu.o
obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/cmd_bmp.o

obj-${CONFIG_TQ335X_L419} +=board/tq335x_4_19/update.o
obj-${CONFIG_TQ335X_L419} +=board/tq335x_4_19/modify.o
obj-${CONFIG_TQ335X_L419} +=board/tq335x_4_19/interface.o

obj-${CONFIG_TQIMX6UL} += board/imx6ul/modify_imx6ul.o
obj-${CONFIG_TQIMX6UL} += board/imx6ul/update_imx6ul.o
obj-${CONFIG_TQIMX6UL} += board/imx6ul/fb_init.o

obj-${CONFIG_TQIMX6Q_3} += board/imx6q_3/update_imx6q_3.o
obj-${CONFIG_TQIMX6Q_3} += board/imx6q_3/modify_imx6q_3.c

obj-${CONFIG_TQIMX6Q_4} +=board/imx6q_4/update_imx6q_4.o
obj-${CONFIG_TQIMX6Q_4} +=board/imx6q_4/modify_imx6q_4.o
obj-${CONFIG_TQIMX6Q_4} +=board/imx6q_4/interface.o

obj-${CONFIG_TEST_BOARD} +=board/test/update.o
obj-${CONFIG_TEST_BOARD} +=board/test/modify.o
obj-${CONFIG_TEST_BOARD} +=board/test/interface.o

obj-${CONFIG_POWER_TEST} += power_test.o
obj-y += encrpty.o
obj-${CONFIG_FLASG_NAND} += lib/mtd/nand_common.o
obj-y += interface.o
obj-y += modify.o
obj-y += common/cmd_fdisk.o
obj-y += common/cmd_fat.o
obj-y += common/cmd_dd.o
obj-y += common/cmd_simg2img.o
obj-y += common/cmd_mmc.o
obj-y += common/cmd_part.o
obj-y += common/cmd_hexdump.o
obj-y += common/cmd_ini.o
obj-y += common/cmd_usb_ini.o
obj-y += common/cmd_set.o
obj-y += common/cmd_update.o
obj-y += common/cmd_net_update.o
obj-y += common/cmd_usb_update.o
obj-y += common/cmd_ext4.o
obj-y += common/cmd_fb.o
obj-y += common/cmd_date.o
#obj-y += common/cmd_httpd.o
obj-y += board_init.o
obj-y += lib/sparse.o
obj-y += lib/backed_block.o
obj-y += lib/io_file.o
obj-y += lib/section_sparse_read.o
obj-y += lib/emmc_partition.o
obj-y += lib/sparse_crc32.o
obj-y += lib/sparse_read.o
obj-y += lib/ini_parser.o
obj-y += lib/out_file.o
obj-y += lib/emmc_common.o
obj-y += lib/common.o
obj-y += lib/part.o
obj-y += lib/part_iso.o
obj-y += lib/part_dos.o
#obj-y += lib/part_amiga.o
obj-y += lib/part_efi.o
obj-y += lib/uuid.o
obj-y += lib/sscanf.o
obj-y += lib/mac.o
obj-y += lib/panel/panel.o
obj-y += lib/panel/fb_common.o
obj-y += lib/panel/show_bmp.o
obj-y += fs/fs.o
obj-y += fs/fat/fat.o
obj-y += fs/fat/file.o
obj-y += fs/ext4/ext4fs.o
obj-y += fs/ext4/ext4_common.o
obj-y += fs/ext4/ext4_journal.o
obj-y += fs/ext4/dev.o
obj-y += fs/ext4/ext4_write.o
obj-y += fs/ext4/crc16.o
obj-y += fs/ext4/ext4_format.o

#obj-y += uip0_9/uip.o
#obj-y += uip0_9/uip_arch.o 
#obj-y += uip0_9/uip_arp.o 
#obj-y += uip0_9/httpd.o 
#obj-y += uip0_9/fs.o
#obj-y += uip0_9/uip_interface.o

COBJS   := $(obj-y)
SRCS    := $(COBJS:.o=.c)
OBJS    := $(addprefix $(obj),$(COBJS))
endif

all:    $(LIB)

#$(LIB): $(obj).depend $(OBJS)
#	$(AR) $(ARFLAGS) $@ $(OBJS)

ifneq ($(wildcard board_init.c),)
$(LIB): $(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))
endif

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend
else

ifeq ($(wildcard board_init.c),)
obj-${CONFIG_TQ335X} += board/tq3358/tq335x_init.o
obj-${CONFIG_TQ335X} += board/tq3358/modify_335x.o
obj-${CONFIG_TQ335X} += board/tq3358/update_tq335x.o
#obj-${CONFIG_TQ335X} += board/tq3358/cmd_menu.o
obj-${CONFIG_TQ335X} += board/tq3358/cmd_bmp.o

obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/tq335x_init.o
obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/modify_335x.o
obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/update_tq335x.o
#obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/cmd_menu.o
obj-${CONFIG_TQ335X_V4} += board/tq3358_v4/cmd_bmp.o

obj-${CONFIG_TQ335X_L419} +=board/tq335x_4_19/update.o
obj-${CONFIG_TQ335X_L419} +=board/tq335x_4_19/modify.o
obj-${CONFIG_TQ335X_L419} +=board/tq335x_4_19/interface.o

obj-${CONFIG_TQIMX6UL} += board/imx6ul/modify_imx6ul.o
obj-${CONFIG_TQIMX6UL} += board/imx6ul/update_imx6ul.o
obj-${CONFIG_TQIMX6UL} += board/imx6ul/fb_init.o

obj-${CONFIG_TQIMX6Q_3} += board/imx6q_3/update_imx6q_3.o
obj-${CONFIG_TQIMX6Q_3} += board/imx6q_3/modify_imx6q_3.c

obj-${CONFIG_TQIMX6Q_4} +=board/imx6q_4/update_imx6q_4.o
obj-${CONFIG_TQIMX6Q_4} +=board/imx6q_4/modify_imx6q_4.o
obj-${CONFIG_TQIMX6Q_4} +=board/imx6q_4/interface.o

obj-${CONFIG_ROCKCHIP_RK3399} +=board/rk3399/update_rk3399.o
obj-${CONFIG_ROCKCHIP_RK3399} +=board/rk3399/modify_rk3399.o
obj-${CONFIG_ROCKCHIP_RK3399} +=board/rk3399/interface.o

obj-${CONFIG_TEST_BOARD} +=board/test/update.o
obj-${CONFIG_TEST_BOARD} +=board/test/modify.o
obj-${CONFIG_TEST_BOARD} +=board/test/interface.o

obj-${CONFIG_POWER_TEST} += power_test.o
obj-y += encrpty.o
obj-${CONFIG_FLASG_NAND} += lib/mtd/nand_common.o
obj-y += interface.o
obj-y += modify.o
obj-y += common/cmd_fdisk.o
obj-y += common/cmd_fat.o
obj-y += common/cmd_dd.o
obj-y += common/cmd_simg2img.o
obj-y += common/cmd_mmc.o
obj-y += common/cmd_part.o
obj-y += common/cmd_hexdump.o
obj-y += common/cmd_ini.o
obj-y += common/cmd_usb_ini.o
obj-y += common/cmd_set.o
obj-y += common/cmd_update.o
obj-y += common/cmd_net_update.o
obj-y += common/cmd_usb_update.o
obj-y += common/cmd_ext4.o
obj-y += common/cmd_fb.o
obj-y += common/cmd_date.o
#obj-y += common/cmd_httpd.o
obj-y += board_init.o
obj-y += lib/sparse.o
obj-y += lib/backed_block.o
obj-y += lib/io_file.o
obj-y += lib/section_sparse_read.o
obj-y += lib/emmc_partition.o
obj-y += lib/sparse_crc32.o
obj-y += lib/sparse_read.o
obj-y += lib/ini_parser.o
obj-y += lib/out_file.o
obj-y += lib/emmc_common.o
obj-y += lib/common.o
obj-y += lib/part.o
obj-y += lib/part_iso.o
obj-y += lib/part_dos.o
#obj-y += lib/part_amiga.o
obj-y += lib/part_efi.o
obj-y += lib/uuid.o
obj-y += lib/sscanf.o
obj-y += lib/mac.o
obj-y += lib/panel/panel.o
obj-y += lib/panel/fb_common.o
obj-y += lib/panel/show_bmp.o
obj-y += fs/fs.o
obj-y += fs/fat/fat.o
obj-y += fs/fat/file.o
obj-y += fs/ext4/ext4fs.o
obj-y += fs/ext4/ext4_common.o
obj-y += fs/ext4/ext4_journal.o
obj-y += fs/ext4/dev.o
obj-y += fs/ext4/ext4_write.o
obj-y += fs/ext4/crc16.o
obj-y += fs/ext4/ext4_format.o

#obj-y += uip0_9/uip.o
#obj-y += uip0_9/uip_arch.o 
#obj-y += uip0_9/uip_arp.o 
#obj-y += uip0_9/httpd.o 
#obj-y += uip0_9/fs.o
#obj-y += uip0_9/uip_interface.o
endif

endif
