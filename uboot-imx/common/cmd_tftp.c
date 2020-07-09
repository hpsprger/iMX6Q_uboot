/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <s_record.h>
#include <ata.h>
#include <asm/io.h>
#include <part.h>
#include <fat.h>
#include <fs.h>
#include <flash.h>
#include <net.h>
//#include <net/tftp.h>
#include <malloc.h>

static int update_load(char *filename, ulong msec_max, int cnt_max, ulong addr)
{
	int size, rv;
	ulong saved_timeout_msecs;
	int saved_timeout_count;
	char *saved_bootfile;

	rv = 0;
	saved_bootfile = strdup(BootFile);

	/* we don't want to retry the connection if errors occur */
	setenv("netretry", "no");

	/* download the update file */
	load_addr = addr;
	copy_filename(BootFile, filename, sizeof(BootFile));
	size = NetLoop(TFTPGET);

	if (size < 0)
		rv = 1;
	else if (size > 0)
		flush_cache(addr, size);

	if (saved_bootfile != NULL) {
		copy_filename(BootFile, saved_bootfile, sizeof(BootFile));
		free(saved_bootfile);
	}

	return size;
}

static int do_tftpfat(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char* env_addr;
	int size;
	int part;
	ulong addr;
	void *buf;
	int ret;
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	loff_t count;
	
	if(argc != 5) {
		printf("argc != 5\n");
		return 1;
	}
	
	part = get_device_and_partition(argv[2], argv[3], &dev_desc, &info, 1);
	if (part < 0) {
		printf("mmc part error\n");
		return 1;
	}
	if (fat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use %s %d:%d for fatwrite **\n",
			argv[2], dev_desc->dev, part);
		return 1;
	}
	
	if ((env_addr = getenv("loadaddr")) == NULL) {
		printf("env loadaddr no set\n");
		return 1;
	}
	addr = simple_strtoul(env_addr, NULL, 16);
	size = update_load(argv[1], 1000, 2, addr);
	if(size == 0xffffffff)
	{
		printf("size = 0x%x\n",size);
		printf("The file size is not correct");
		return 1;
	}
	printf("size = 0x%x\n",size);
	buf = map_sysmem(addr, size);
	ret = file_fat_write(argv[4], buf, 0, size, &count);
	unmap_sysmem(buf);
	
	printf("%llu bytes written\n", count);
	
	return 0;
}

U_BOOT_CMD(
	tftpfat,	5,	5,	do_tftpfat,
	"tftp update fat filesystem",
	"<filename> <interface> <dev[:part]> <filename>"
);
