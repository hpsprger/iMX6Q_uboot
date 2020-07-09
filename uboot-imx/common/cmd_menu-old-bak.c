/*
 * tq_668@126.com, www.embedsky.net
 *
 */



#include <common.h>
#include <command.h>
#include <nand.h>

#ifdef CONFIG_CMD_MENU

#ifdef CONFIG_EMBEDSKY_FAT
//#include <def.h>
//#include <vfat.h>
#endif

//extern char console_buffer[];
//int TQ_readline (const char *const prompt);	
//extern char awaitkey(unsigned long delay, int* error_p);
//extern void download_nkbin_to_flash(void);
//extern int boot_zImage(ulong from, size_t size);
//extern char bLARGEBLOCK;
static int do_tftp_para_setting(char *pbuf);
static int set_args(char *pmenu, char *pargs);
#define SERIAL_PORT_NUM 0
enum {
	DIS_LCD = 0,
	DIS_HDMI,
	DIS_LDB1,
	DIS_LDB2,
	MAX_DIS
};
char* dis_ref[4] = {"lcd","hdmi","ldb","ldb"};
int dis_dev[4] = {MAX_DIS, MAX_DIS, MAX_DIS, MAX_DIS};
char* mode_str[MAX_DIS];

static int parse_char(char* parse, char* opt, int num)
{
	
	if(strlen(parse) < num) {
		return 0;
	}
	
	while(*(parse+num-1) != '\0') {
		if(!strncmp(parse, opt, num)) {
			return 1;
		}
		parse ++;
	}
	return 0;
}

static int init_disp_dev(void)
{
	char* ptmp;
	char* env_buf;
	int i, flag;
	flag = 0;
	
	for(i = 0; i < MAX_DIS; i++) {
		sprintf(env_buf,"mxcfb%d",i);
		ptmp = getenv(env_buf);
		if(parse_char(ptmp, "lcd", 3) && !(flag & 0x01)) {
			dis_dev[i] = DIS_LCD;
			flag |= 0x01;
		}
		if(parse_char(ptmp, "hdmi", 4) && !(flag & 0x02)) {
			dis_dev[i] = DIS_HDMI;
			flag |= 0x02;
		}
		if(parse_char(ptmp, "ldb", 3) && !(flag & 0x08)) {
			
			if(flag & 0x04) {
				// second
				dis_dev[i] = DIS_LDB2;
				flag |= 0x08;
			} else {
				// first
				dis_dev[i] = DIS_LDB1;
				flag |= 0x04;
			}
		}
	}
	
	if(!(flag & 0x01)) {
		for(i = 0; i < MAX_DIS; i++)
			if(dis_dev[i] == MAX_DIS) {
				dis_dev[i] = DIS_LCD;
				flag |= 0x01;
				break;
			}
	}
	if(!(flag & 0x02)) {
		for(i = 0; i < MAX_DIS; i++)
			if(dis_dev[i] == MAX_DIS) {
				dis_dev[i] = DIS_HDMI;
				flag |= 0x02;
				break;
			}
	}
	if(!(flag & 0x04)) {
		for(i = 0; i < MAX_DIS; i++)
			if(dis_dev[i] == MAX_DIS) {
				dis_dev[i] = DIS_LDB1;
				flag |= 0x04;
				break;
			}
	}
	if(!(flag & 0x08)) {
		for(i = 0; i < MAX_DIS; i++)
			if(dis_dev[i] == MAX_DIS) {
				dis_dev[i] = DIS_LDB2;
				flag |= 0x08;
				break;
			}
	}
	
}

static int set_disp_dev(int num,int dev, char* mode)
{
	int i;
	
	if(num >= MAX_DIS) {
		return -1;
	}
	
	for(i = 0; i < MAX_DIS; i++) {
		if(dis_dev[i] == dev) {
			dis_dev[i] = dis_dev[num];
		}
	}

	if((dev == DIS_LCD) || (dev == DIS_LDB1)) {
		setenv("panel",mode);
	}
	dis_dev[num] = dev;
	
	mode_str[num] = mode;
}

static void set_disp_env(void)
{
	char cmd_buf[512];
	int i;
	
	for(i = 0; i < MAX_DIS; i++) {
		if(dis_dev[i] == DIS_LCD) {
			if(!mode_str[i]) {
				mode_str[i] = "CLAA-WVGA";
			}
			sprintf(cmd_buf, "setenv mxcfb%d 'video=mxcfb%d:dev=%s,%s,if=RGB24'",i,i,dis_ref[dis_dev[i]],mode_str[i]);
			run_command(cmd_buf,0);
		} else if(dis_dev[i] == DIS_HDMI) {
			if(!mode_str[i]) {
				mode_str[i] = "1920x1080@60";
			}
			sprintf(cmd_buf, "setenv mxcfb%d 'video=mxcfb%d:dev=%s,%s,if=RGB24'",i,i,dis_ref[dis_dev[i]],mode_str[i]);
			run_command(cmd_buf,0);
		} else if(dis_dev[i] == DIS_LDB1) {
			if(!mode_str[i]) {
				mode_str[i] = "LDB-XGA";
			}
			sprintf(cmd_buf, "setenv mxcfb%d 'video=mxcfb%d:dev=%s,%s,if=RGB666 ldb=dul0'",i,i,dis_ref[dis_dev[i]],mode_str[i]);
			run_command(cmd_buf,0);
		} else if(dis_dev[i] == DIS_LDB2) {
			if(!mode_str[i]) {
				mode_str[i] = "LDB-XGA";
			}
			sprintf(cmd_buf, "setenv mxcfb%d 'video=mxcfb%d:dev=%s,%s,if=RGB666 ldb=dul1'",i,i,dis_ref[dis_dev[i]],mode_str[i]);
			run_command(cmd_buf,0);
		}
	}
	
}

static int sdcard_update(char* type)
{
	int start_addr;
	char cmd_buf[512];
	if(!type) {
		printf("type is null\n");
		return -1;
	}
	
	if(strcmp(type,"uboot") == 0) {
		start_addr = 0x02;
		
		sprintf(cmd_buf,"if mmc dev ${sddev};then fatload mmc ${sddev}:${sdpart} ${loadaddr} ${uboot_file};fi");
		if(run_command(cmd_buf,0) < 0) {
			printf("run command fail\n");
			return -1;
		}
		sprintf(cmd_buf,"if mmc dev ${mmcdev}; then setexpr fw_sz ${uboot_size} / 0x200;setexpr fw_sz ${fw_sz} + 1;mmc write ${loadaddr} %x ${fw_sz};fi",start_addr);
		run_command(cmd_buf,0);
	} else if(strcmp(type,"kernel") == 0) {
	
	} else if(strcmp(type,"system") == 0) {
		
	}
}

static void net_menu_shell(void)
{
	char c;
	int pri = 1;
	char *ptmp;
	
	do {
		if(pri) {
			pri--;
			printf("\r\n##### Parameter Menu #####\r\n");
			printf("[1]tftp update u-boot\n");
			printf("[2]tftp update kernel\n");
			printf("[3]tftp update dtb\n");
			printf("[4]tftp update logo\n");
			printf("[5]tftp update system\n");
			printf("[6]OneKey update\n");
			printf("[0]set tftp env\n");
			printf("[q] Return main Menu \r\n");
			printf("Enter your selection: ");
		}
		
		c = getc();
		switch(c) {
			case '1':
				printf("%c\n", c);
				//sdcard_update("uboot");
				pri = 1;
				break;
			case '2':
				printf("%c\n", c);
				ptmp = getenv("bootcmd");
				run_command("run tftp_loadimage", 0);

				pri = 1;
				break;
			case '3':
				printf("%c\n", c);
				run_command("run tftp_loadfdt", 0);
				pri = 1;
				break;
			case '4':
				printf("%c\n", c);
				run_command("run tftp_loadlogo", 0);
				pri = 1;
				break;
			case '5':
				printf("%c\n", c);
				//run_command("fatcp mmc 0:1 rootfs.tgz mmc 2:1 rootfs.tgz",0);
				pri = 1;
				break;
			case '6':
				printf("%c\n", c);
				pri = 1;
				break;
			case '0':
				printf("%c\n", c);
				do_tftp_para_setting(NULL);
				pri = 1;
				break;
			default:
				break;
		}
		
	}while(c != 'q');
}

static void sd_menu_shell(void)
{
	char c;
	int pri = 1;
	char *ptmp;
	
	do {
		if(pri) {
			pri--;
			printf("\r\n##### Parameter Menu #####\r\n");
			printf("[1]sd update u-boot\n");
			printf("[2]sd update kernel\n");
			printf("[3]sd update dtb\n");
			printf("[4]sd update logo\n");
			printf("[5]sd update system\n");
			printf("[6]OneKey update\n");
			printf("[q] Return main Menu \r\n");
			printf("Enter your selection: ");
		}
		
		c = getc();
		switch(c) {
			case '1':
				printf("%c\n", c);
				sdcard_update("uboot");
				pri = 1;
				break;
			case '2':
				printf("%c\n", c);
				ptmp = getenv("bootcmd");
				if(strcmp(ptmp,"run boot_ubuntu") == 0)
					run_command("fatcp mmc 0:1 zImage mmc 2:1 zImage", 0);
				else if (strcmp(ptmp,"run boot_android") == 0)
					run_command("fatcp mmc 0:1 boot.img mmc 2:1 boot.img", 0);
				else
					run_command("fatcp mmc 0:1 zImage mmc 2:1 zImage", 0);

				pri = 1;
				break;
			case '3':
				printf("%c\n", c);
				run_command("fatcp mmc 0:1 imx6q-sabresd.dtb mmc 2:1 imx6q-sabresd.dtb",0);
				pri = 1;
				break;
			case '4':
				printf("%c\n", c);
				run_command("fatcp mmc 0:1 logo.bmp mmc 2:1 logo.bmp",0);
				pri = 1;
				break;
			case '5':
				printf("%c\n", c);
				run_command("fatcp mmc 0:1 rootfs.tgz mmc 2:1 rootfs.tgz",0);
				pri = 1;
				break;
			case '6':
				printf("%c\n", c);
				run_command("fatcp mmc 0:1 zImage mmc 2:1 zImage",0);
				pri = 1;
				break;
			default:
				break;
		}
		
	}while(c != 'q');
}

static int determine_to_save()
{
	char c;
	printf("%s\n", "save setting ? (y/n) :");
	c = getc();
	printf("%c\n", c);
	if (c == 'y')
	{
		run_command("saveenv", 0);
		return 1;
	}
	printf("%s\n", "setting have not been saved");
	return 0;
}

static int set_kernel_ip_args(char *pbuf)
{
	int i;
	char c;
	int pri = 1;
	char* env;
	char cmd_buf[256];
	
	do {
		if(pri) {
			pri--;
			printf("\r\n#####kernel ip args #####\r\n");		
			printf("[1]kernel ip use dhcp\n");
			printf("[2]kernel ip use uboot\n");	
			printf("[3]custom kernel ip\n");
			printf("[s]save setting\n");	
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		switch (c)
		{
		case '1':
			printf("%c\n", c);
			run_command("setenv dh 'dhcp'",0);
			pri = 1;
			break;
		case '2':
			printf("%c\n", c);
			sprintf(cmd_buf, "setenv dh '%s:%s:%s:%s'",getenv("ipaddr"), getenv("serverip"), getenv("gatewayip"), getenv("netmask"));
			run_command(cmd_buf,0);
			pri = 1;
			break;
		case '3':
			printf("%c\n", c);
			set_args("Enter the kernel IMX6 IP address","dhip");
			set_args("Enter the kernel bootserver IP address","dhserip");
			set_args("Enter the kernel gatewayip IP address","dhgaip");
			set_args("Enter the kernel netmask IP address","dhmaskip");
			sprintf(cmd_buf, "setenv dh '%s:%s:%s:%s'",getenv("dhip"), getenv("dhserip"), getenv("dhgaip"), getenv("dhmaskip"));
			run_command(cmd_buf,0);
			pri = 1;
			break;
		case 's':
			printf("%c\n", c);
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	}while ((c != 'q') && (c != 'Q'));
	return 0;
}

static int set_args(char *pmenu, char *pargs)
{
	char param_buf[256], cmd_buf[256];
	char* pdesc = NULL;

	printf("%s\n", pmenu);
	pdesc = getenv(pargs);
	TQ_readline(pdesc);
	strcpy(param_buf, console_buffer);
	sprintf(cmd_buf, "setenv %s '%s'", pargs, param_buf);
	run_command(cmd_buf, 0);
	return 0;
}

static int do_nfs_para_setting(char *pbuf)
{
	int rtn = 0;

	printf("--------setting %s args--------\n", "nfs");
	set_args("Enter the PC IP address:", "nfsserverip");
	set_args("Enter the IMX6 IP address:", "ipaddr");
	set_args("Enter NFS directory:", "nfsroot");
	determine_to_save();
	return rtn;
}

static int do_tftp_para_setting(char *pbuf)
{
	printf("--------setting %s args--------\n", "tftp");
	set_args("Enter the TFTP Server(PC) IP address:", "serverip");
	set_args("Enter the IMX6 IP address:", "ipaddr");
	set_args("Enter the GateWay IP:", "gatewayip");	
	set_args("Enter the bootloader (u-boot or bootimage) image name:", "uboot");
	set_args("Enter the LOGO image name:", "logoimgname");
	set_args("Enter the kernel image name:", "tftpimage");
	set_args("Enter the dtb image name:", "fdt_file");
	set_args("Enter the root image name:", "rootfs");
	determine_to_save();
}

static set_display_args(int num,char* dev, char* mode, char* rgb, int bpp)
{
	char cmd_buf[256];
	if(!strcmp(dev,"off")) {
		sprintf(cmd_buf,"setenv mxcfb%d 'video=mxcfb%d:%s'",num,num,dev);
		run_command(cmd_buf,0);
	} else if(!strcmp(dev,"dtb")) {
		sprintf(cmd_buf,"setenv mxcfb%d ' '",num);
		run_command(cmd_buf,0);
	} else {
		if(num == 0) {
			setenv("panel",mode);
		}
		sprintf(cmd_buf,"setenv mxcfb%d 'video=mxcfb%d:dev=%s,%s,if=%s,bpp=%d'",num,num,dev,mode,rgb,bpp);
		run_command(cmd_buf,0);
	}
}

static int set_lcd_para(int num)
{
	int i;
	char c;
	int pri = 1;
	
	do {
		if(pri) {
			pri--;
			printf("\r\n##### mxcfb%d lcd args #####\r\n",num);		
			printf("[1]800*480 lcd\n");
			printf("[2]1024*600 lcd\n");
			printf("[s]save setting\n");	
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		switch (c)
		{
		case '1':
			printf("%c\n", c);
			set_display_args(num,"lcd","CLAA-WVGA","RGB24",24);
			//set_disp_dev(num,DIS_LCD,"CLAA-WVGA");
			pri = 1;
			break;
		case '2':
            printf("%c\n", c);
            set_display_args(num,"lcd","TQ-TFT_1024600","RGB24",24);
            //set_disp_dev(num,DIS_LCD,"TQ-TFT_1024600");
            pri = 1;
            break;
		case 's':
			printf("%c\n", c);
			//set_disp_env();
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	}while ((c != 'q') && (c != 'Q'));
	return 0;
}
static int set_lvds_para(int num, int channl)
{
	int i;
	char c;
	int pri = 1;
	int dev;
	
	if(channl) {
		dev = DIS_LDB2;
	} else {
		dev = DIS_LDB1;
	}
	do {
		if(pri) {
			pri--;
			printf("\r\n##### mxcfb%d lvds%d args #####\r\n",num,channl);		
			printf("[1]1366*768\n");
			printf("[s]save setting\n");	
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		switch (c)
		{
		case '1':
			printf("%c\n", c);
			set_display_args(num,"ldb","1366x768@60","RGB666",32);
			//set_disp_dev(num,dev,"1366x768@60,bpp=32");
			pri = 1;
			break;
		case 's':
			printf("%c\n", c);
			//set_disp_env();
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	}while ((c != 'q') && (c != 'Q'));
	return 0;
}
static int set_hdmi_para(int num)
{
	int i;
	char c;
	int pri = 1;
	
	do {
		if(pri) {
			pri--;
			printf("\r\n##### mxcfb%d hdmi args #####\r\n",num);		
			printf("[1]480p\n");
			printf("[2]720p\n");
			printf("[3]1080p\n");
			printf("[s]save setting\n");	
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		switch (c)
		{
		case '1':
			printf("%c\n", c);
			set_display_args(num,"hdmi","640x480@60","RGB24",24);
			//set_disp_dev(num,DIS_HDMI,"640x480@60");
			pri = 1;
			break;
		case '2':
			printf("%c\n", c);
			set_display_args(num,"hdmi","1280x720@60","RGB24",24);
			//set_disp_dev(num,DIS_HDMI,"1280x720@60");
			pri = 1;
			break;
		case '3':
			printf("%c\n", c);
			set_display_args(num,"hdmi","1920x1080@60","RGB24",24);
			//set_disp_dev(num,DIS_HDMI,"1920x1080@60");
			pri = 1;
			break;
		case 's':
			printf("%c\n", c);
			//set_disp_env();
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	}while ((c != 'q') && (c != 'Q'));
	return 0;
}

static int set_off_para(int num)
{
    char cmd[50];
    sprintf(cmd,"setenv mxcfb%d 'video=mxcfb%d:off'",num,num);
    run_command(cmd,0);
}

static int set_display_para(int num)
{
	int i;
	char c;
	int pri = 1;
	
	do {
		if(pri) {
			pri--;
			printf("\r\n##### mxcfb%d display args #####\r\n",num);		
			printf("[1]lcd or VGA\n");
			printf("[2]hdmi\n");
			printf("[3]lvds\n");
			//printf("[4]lvds1\n");
			printf("[0]off\n");
			printf("[s]save setting\n");		
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		switch (c)
		{
		case '1':
			printf("%c\n", c);
			set_lcd_para(num);
			pri = 1;
			break;
		case '2':
			printf("%c\n", c);
			set_hdmi_para(num);
			pri = 1;
			break;
		case '3':
			printf("%c\n", c);
			set_lvds_para(num,0);
			pri = 1;
			break;
		/*case '4':
			printf("%c\n", c);
			set_lvds_para(num,1);
			pri = 1;
			break;*/
		case '0':
			printf("%c\n", c);
			set_off_para(num);
			pri = 1;
			break;
		case 's':
			printf("%c\n", c);
			//set_disp_env();
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	}while ((c != 'q') && (c != 'Q'));
	return 0;
}

static int set_display_type()
{
	int i;
	char c;
	int pri = 1;
	
	do {
		if(pri) {
			pri--;
			printf("\r\n##### display type #####\r\n");		
			printf("[1]display use uboot args\n");
			printf("[2]display use dtb\n");
			printf("[s]save setting\n");		
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		switch (c)
		{
		case '1':
			printf("%c\n", c);
			set_display_args(0,"lcd","CLAA-WVGA","RGB24",24);
			set_display_args(1,"off",NULL,NULL,0);
			set_display_args(2,"off",NULL,NULL,0);
			set_display_args(3,"off",NULL,NULL,0);
			pri = 1;
			break;
		case '2':
			printf("%c\n", c);
			set_display_args(0,"dtb",NULL,NULL,0);
			set_display_args(1,"dtb",NULL,NULL,0);
			set_display_args(2,"dtb",NULL,NULL,0);
			set_display_args(3,"dtb",NULL,NULL,0);
			pri = 1;
			break;
		case 's':
			printf("%c\n", c);
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	}while ((c != 'q') && (c != 'Q'));
	return 0;
}

static int do_disp_para_setting(char *pbuf)
{
	int i;
	char c;
	int pri = 1;
	init_disp_dev();
	do {
		if(pri) {
			pri--;
			printf("\r\n##### setting display args #####\r\n");		
			printf("[1]mxfb0 display args\n");
			printf("[2]mxfb1 display args\n");
			//printf("[3]mxfb2 display args (%s)\n",dis_ref[dis_dev[2]]);
			//printf("[4]mxfb3 display args (%s)\n",dis_ref[dis_dev[3]]);
		//	printf("[0]display use type\n");
			printf("[s]save setting\n");		
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		switch (c)
		{
		case '1':
			printf("%c\n", c);
			set_display_para(0);
			pri = 1;
			break;
		case '2':
			printf("%c\n", c);
			set_display_para(1);
			pri = 1;
			break;
		case '3':
			printf("%c\n", c);
			//set_display_para(2);
			pri = 1;
			break;
		case '4':
			printf("%c\n", c);
			//set_display_para(3);
			pri = 1;
			break;
		case '0':
			printf("%c\n", c);
		//	set_display_type();
			pri = 1;
			break;
		case 's':
			printf("%c\n", c);
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	}while ((c != 'q') && (c != 'Q'));
	//lcd_menu_shell();
	return 0;
}


static void do_media_para_setting(char * pbuf)
{
	int i;
	char c;
	int pri = 1;

	do {
		if(pri) {
			pri--;
			printf("\r\n##### param setting #####\r\n");		
			//printf("[1]ubuntu used emmc \n");
			printf("[1]android used emmc \n");
			printf("[2]linux used emmc \n");
			printf("[3]andorid used tftp & nfs \n");
			printf("[s]save setting\n");		
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		printf("%c", c);
		switch (c)
		{
		/*case '1':
			run_command("setenv bootcmd 'run bootcmd_mmc_ubuntu'",0);
			pri = 1;
			break;*/
		case '1':
			run_command("setenv bootcmd 'run boot_android'",0);
			run_command("setenv image 'boot.img'",0);
			//setenv("image","boot.img");
			pri = 1;
			break;
		case '2':
			run_command("setenv bootcmd 'run boot_linux'",0);
			run_command("setenv image 'zImage'",0);
			//setenv("image","zImage");
			pri = 1;
			break;
		case '3':
			run_command("run nfs_root", 0);
			//run_command("setenv bootcmd run 'netboot'",0);
			pri = 1;
			break;
		case 's':
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	} while ((c != 'q') && (c != 'Q'));

}

static int gen_eth_addr(void)
{
	char cmd[256];
	char addr[6];

	eth_random_addr(addr);
	sprintf(cmd,"setenv ethaddr %02x:%02x:%02x:%02x:%02x:%02x\n",
	addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
	printf("%s\n",cmd);
	
	run_command(cmd, 0);
}

static int set_net_env(void)
{
	int i;
	char c;
	int pri = 1;

	do {
		if(pri) {
			pri--;
			printf("\r\n##### setting network env #####\r\n");		
			printf("[1]custom eth mac \n");
			printf("[2]gen eth mac \n");
			printf("[3]set netmask \n");
			printf("[4]set gatewayip \n");
			printf("[s]save setting\n");		
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		printf("%c", c);
		switch (c)
		{
		case '1':
			set_args("custom eth mac:", "ethaddr");
			pri = 1;
			break;
		case '2':
			gen_eth_addr();
			pri = 1;
			break;
		case '3':
			set_args("set netmask:", "netmask");
			pri = 1;
			break;
		case '4':
			set_args("set gatewayip:", "gatewayip");
			pri = 1;
			break;
		case 's':
			run_command("saveenv", 0);
			pri = 1;
			break;
		default:
			break;
		}
	} while ((c != 'q') && (c != 'Q'));
}

static int do_boot_para_setting(char *pbuf)
{
	int i;
	char c;
	int pri = 1;

	do {
		if(pri) {
			pri--;
			printf("\r\n##### param setting #####\r\n");		
			printf("[1]setting nfs args\n");
			//printf("[2]setting tftp args\n");
			printf("[3]setting display args\n");
			printf("[4]setting default boot\n");
			printf("[5]kernel ip dhcp\n");
			printf("[0]setting network env\n");
			printf("[s]save setting\n");
			printf("[q]quit\n");
			printf("Enter your selection: ");
		}
		c = getc();
		switch (c)
		{
		case '1':
			printf("%c\n", c);
			do_nfs_para_setting(NULL);
			pri = 1;
			break;
		/*case '2':
			printf("%c\n", c);
			do_tftp_para_setting(NULL);
			pri = 1;
			break;*/
		case '3':
			printf("%c\n", c);
			do_disp_para_setting(NULL);
			pri = 1;
			break;
		case '4':
			printf("%c\n", c);
			do_media_para_setting(NULL);
			pri = 1;
			break;
		case '5':
			printf("%c\n", c);
			set_kernel_ip_args(NULL);
			pri = 1;
			break;
		case '0':
			printf("%c\n", c);
			set_net_env();
			pri = 1;
			break;
		case 's':
			printf("%c\n", c);
			run_command("saveenv", 0);
			pri = 1;	
			break;
		default:
			break;
		}
	} while ((c != 'q') && (c != 'Q'));
}

#define USE_USB_DOWN        1
#define USE_TFTP_DOWN       2
#define USE_SD_DOWN     3

void menu_shell(void)
{
	char keyselect;
	char cmd_buf[512];
	char *ptmp;
	int pri = 1;
	
	do {
		if(pri) {
			pri--;
			printf("\r\n#####	 Boot for IMX6 Main Menu	#####\r\n");
			printf("[1]boot from emmc\n");
			printf("[2]boot from sdcard\n");
			printf("[3]boot from nfs\n");
			printf("[4]sdcard update system\n");
			printf("[5]net update system\n");
		//	printf("[5]download from sdcard\n");
			printf("[0]setting boot args\n");
			printf("[q]exit to command mode\n");
			printf("Enter your selection: ");
		}
		keyselect = getc();
		switch (keyselect)
		{
			case '1':
				printf("%c", keyselect);
				run_command("run bootcmd", 0);
				pri = 1;
				break;
			case '2':
				printf("%c", keyselect);
				pri = 1;
			break;
			case '3':
				printf("%c", keyselect);
				run_command("run nfs_root", 0);
				run_command("run bootcmd", 0);
				pri = 1;
			break;
			case '4':
				printf("%c", keyselect);
				sd_menu_shell();
				pri = 1;
				break;
			case '5':
				printf("%c", keyselect);
				net_menu_shell();
				pri = 1;
				break;
			case '0':
				printf("%c", keyselect);
				do_boot_para_setting(NULL);
				pri = 1;
				break;
			default:
				break;
		}

	} while ((keyselect != 'q') && (keyselect != 'Q'));
	printf("%c\n", keyselect);
}

int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	menu_shell();
	return 0;
}

U_BOOT_CMD(
    menu,   3,  0,  do_menu,
    "display a menu, to select the items to do something",
    "\n"
    "\tdisplay a menu, to select the items to do something"
);
#endif
