#include <common.h>
#include <command.h>
#include <nand.h>
#include <linux/ctype.h>

typedef struct menu_info{
	const char lead;
	const char *help;
	const char *cmd;
	void (*func) (void *buf);
	const struct menu_info *next_menu_item;
	const int menu_arraysize;
	const int item_para;
}menu_info_st;

static void gen_eth_addr(void)
{
	char cmd[256];
	char addr[6];

	eth_random_addr((uint8_t *)addr);
	sprintf(cmd,"setenv ethaddr %02x:%02x:%02x:%02x:%02x:%02x\n",
	addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
	printf("%s\n",cmd);
	
	run_command(cmd, 0);
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
static void do_set_off_para(void *para)
{
	int num ;
	num = (int)(void*)para;
    char cmd[50];
    sprintf(cmd,"setenv mxcfb%d 'video=mxcfb%d:off'",num,num);
    run_command(cmd,0);
}

static void do_set_custom_eth_mac(void *para)
{
	set_args("custom eth mac:", "ethaddr");
}
static void do_set_gen_eth_addr(void *para)
{
	gen_eth_addr();
}
static void do_set_net_mask(void *para)
{
	set_args("set netmask:", "netmask");
}
static void do_set_gatewayip(void *para)
{
	set_args("set gatewayip:", "gatewayip");
}

static void do_set_kernel_ip_use_uboot(void *para)
{
	char cmd_buf[256];
	sprintf(cmd_buf, "setenv dh '%s:%s:%s:%s'",getenv("ipaddr"), getenv("serverip"), getenv("gatewayip"), getenv("netmask"));
	run_command(cmd_buf,0);
}
static void do_set_custom_kernel_ip(void *para)
{
	char cmd_buf[256];
	set_args("Enter the kernel IMX6 IP address","dhip");
	set_args("Enter the kernel bootserver IP address","dhserip");
	set_args("Enter the kernel gatewayip IP address","dhgaip");
	set_args("Enter the kernel netmask IP address","dhmaskip");
	sprintf(cmd_buf, "setenv dh '%s:%s:%s:%s'",getenv("dhip"), getenv("dhserip"), getenv("dhgaip"), getenv("dhmaskip"));
	run_command(cmd_buf,0);
}

static void set_display_args(int num,char* dev, char* mode, char* rgb, int bpp)
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
static void do_set_lvds_para_1360768(void *para)
{
	set_display_args((int)(void *)para,"ldb","1360x768@60","RGB666",32);
}
static void do_set_lvds_para_1366768(void *para)
{
        set_display_args((int)(void *)para,"ldb","1366x768@60","RGB666",32);
}
static void do_set_lvds_para_1280800(void *para)
{
        set_display_args((int)(void *)para,"ldb","1280x800@60","RGB24",32);
}

static void do_set_hdmi_para_480(void *para)
{
	set_display_args((int)(void *)para,"hdmi","640x480@60","RGB24",32); 
}
static void do_set_hdmi_para_720(void *para)
{
	set_display_args((int)(void *)para,"hdmi","1280x720@60","RGB24",32); 
}
static void do_set_hdmi_para_1080(void *para)
{
	set_display_args((int)(void *)para,"hdmi","1920x1080@60","RGB24",32); 
}

static void do_set_lcd_para_CLAA_WVGA(void *para)
{
	set_display_args((int)(void *)para,"lcd","CLAA-WVGA","RGB24",24);
}
static void do_set_lcd_para_TQ_TFT_1024600(void *para)
{
	set_display_args((int)(void *)para,"lcd","TQ-TFT_1024600","RGB24",24);
}
static void do_set_lcd_para_TQ_VGA_640480(void *para)
{
        set_display_args((int)(void *)para,"lcd","TQ-VGA_640480","RGB24",32);
}
static void do_set_lcd_para_TQ_VGA_1024768(void *para)
{
	set_display_args((int)(void *)para,"lcd","TQ-VGA_1024768","RGB24",32);
}
static void do_set_lcd_para_TQ_VGA_1280720(void *para)
{
        set_display_args((int)(void *)para,"lcd","TQ-VGA_1280720","RGB24",32);
}
static void do_set_lcd_para_TQ_VGA_1280768(void *para)
{
	set_display_args((int)(void *)para,"lcd","TQ-VGA_1280768","RGB24",32);
}
static void do_set_lcd_para_TQ_VGA_1360768(void *para)
{
	set_display_args((int)(void *)para,"lcd","TQ-VGA_1360768","RGB24",32);
}
static void do_set_lcd_para_TQ_VGA_19201080(void *para)
{
	set_display_args((int)(void *)para,"lcd","TQ-VGA_19201080","RGB24",32);
}
static void do_set_lcd_para_TQ_VGA_19201200(void *para)
{
	set_display_args((int)(void *)para,"lcd","TQ-VGA_19201200","RGB24",32);
}

static int determine_to_save(void)
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

static void do_nfs_para_setting(void *para)
{
	printf("--------setting %s args--------\n", "nfs");
	set_args("Enter the PC IP address:", "nfsserverip");
	set_args("Enter the IMX6 IP address:", "ipaddr");
	set_args("Enter NFS directory:", "nfsroot");
	determine_to_save();
}

static void tftp_para_setting(void *para)
{
	printf("--------setting %s args--------\n", "tftp");
	set_args("Enter the TFTP Server(PC) IP address:", "serverip");
	set_args("Enter the IMX6 IP address:", "ipaddr");
	set_args("Enter the GateWay IP:", "gatewayip");	
	determine_to_save();
}
static void tftp_update_kernel(void *para)
{
	char *ptmp;
	ptmp = getenv("bootcmd");
	(void *)ptmp;
	run_command("run tftp_loadimage", 0);
}

static void tftp_update_onekey(void *para)
{
	tftp_update_kernel((void*)0);
	run_command("run tftp_loadfdt", 0);
	run_command("run tftp_loadlogo", 0);
}

static int _sdcard_update(char* type)
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
	return 0;
}

static void sdcard_update(void *para)
{
	_sdcard_update("uboot");
}

static void sd_update_kernel(void *para)
{
	char *ptmp;
	ptmp = getenv("bootcmd");
	if(strcmp(ptmp,"run boot_ubuntu") == 0)
		run_command("fatcp mmc 0:1 zImage mmc 2:1 zImage", 0);
	else if (strcmp(ptmp,"run boot_android") == 0)
		run_command("fatcp mmc 0:1 boot.img mmc 2:1 boot.img", 0);
	else
		run_command("fatcp mmc 0:1 zImage mmc 2:1 zImage", 0);	
}

static void set_other_screen(void *para)
{
	set_args("Enter the screen type:", "p_fb0");
}


//++++++++++++++++++++++++++++++++++++          默认启动参数           ++++++++++++++++++++++++++++++++//
/************* default boot info *************/
static menu_info_st default_boot_menu_info[]={
		{0 , "default boot setting", 		NULL, NULL, NULL,0, 0},
		{'1',"android used emmc",		"tq_set os android",NULL,NULL,0,0},
		{'2',"linux used emmc",			"tq_set os linux",NULL,NULL,0,0},
		{'3',"rootfs used nfs",			"tq_set os nfs",NULL,NULL,0,0}, 	
		{'s',"save setting","saveenv",NULL,NULL,0,0},
		{'q',"quit",NULL,NULL,NULL,0,0},		
};
//++++++++++++++++++++++++++++++++++++          内核参数           ++++++++++++++++++++++++++++++++//
/*************  kernel_ip info *************/
static menu_info_st kernel_ip_menu_info[]={
		{0 , "kernel_ip setting", 		NULL, NULL, NULL,0, 0},
		{'1',"kernel ip use dhcp",	"setenv dh 'dhcp'",NULL,NULL,0,0},		
		{'2',"kernel ip use uboot",	NULL,&do_set_kernel_ip_use_uboot,NULL,0,0}, 	
		{'3',"custom kernel ip",	NULL,&do_set_custom_kernel_ip,NULL,0,0},	
		{'s',"save setting",		"saveenv",NULL,NULL,0,0},		
		{'q',"quit",				NULL,NULL,NULL,0,0},		
};
//++++++++++++++++++++++++++++++++++++          网络参数           ++++++++++++++++++++++++++++++++//
/*************	net env info *************/
static menu_info_st net_env_menu_info[]={
		{0 , "net env setting", 		NULL, NULL, NULL,0, 0},
		{'1',"custom eth mac",NULL,&do_set_custom_eth_mac,NULL,0,0},		
		{'2',"gen eth mac",NULL,&do_set_gen_eth_addr,NULL,0,0}, 	
		{'3',"set netmask",NULL,&do_set_net_mask,NULL,0,0},	
		{'4',"set gatewayip",NULL,&do_set_gatewayip,NULL,0,0},	
		{'s',"save setting","saveenv",NULL,NULL,0,0},		
		{'q',"quit",NULL,NULL,NULL,0,0},		
};


/************* display menu info *************/
static menu_info_st display_menu_item[]={
	{0 , "EmbedSky display type", 	NULL, NULL, NULL,0, 0},
	{'1',"LCD T43\" screen",			"setenv p_fb0 lcd_4_3pd",NULL,NULL,0,0}, 
	{'2',"LCD A70\" screen",			"setenv p_fb0 lcd_7pd",NULL,NULL,0,0},
	{'3',"LCD A104\" screen",			"setenv p_fb0 lcd_10_4pd",NULL,NULL,0,0},		
	{'4',"LCD H50\" screen",			"setenv p_fb0 lcd_5hd",NULL,NULL,0,0},
	{'5',"LCD H70\" screen",			"setenv p_fb0 lcd_7hd",NULL,NULL,0,0},
	{'6',"VGA 1280*768\" screen",		"setenv p_fb0 vga_720p",NULL,NULL,0,0},
	{'7',"VGA 1920*1080\" screen",		"setenv p_fb0 vga_1080p",NULL,NULL,0,0},
	{'a',"LVDS 1280*800\" screen",		"setenv p_fb0 lvds_1280_800",NULL,NULL,0,0},
	{'b',"LVDS 1366*768\" screen",		"setenv p_fb0 lvds_1366_768",NULL,NULL,0,0},
	{'c',"LVDS 1360*768\" screen",		"setenv p_fb0 lvds_1360_768",NULL,NULL,0,0},
	{'d',"HDMI 1280*720\" screen",		"setenv p_fb0 hdmi_720p",NULL,NULL,0,0},
	{'e',"HDMI 1920*1080\" screen",		"setenv p_fb0 hdmi_1080p",NULL,NULL,0,0},
	{'f',"off",							"setenv p_fb0 off",NULL,NULL,0,0},
	{'0',"Other\" screen",			NULL,&set_other_screen,NULL,0,0},
	{'s',"Save the parameters to Flash",		"saveenv",NULL,NULL,0,0},
	{'q',"Return main Menu",		NULL,NULL,NULL,0,0},
};


/************* net menu info *************/
static menu_info_st setting_menu_item[]={
	{0 , "EmbedSky Setting Args", 		NULL, NULL, NULL,0, 0},
	{'1',"setting NFS args",			NULL,&do_nfs_para_setting,NULL,0,1},
	{'2',"setting default boot",		NULL,NULL,default_boot_menu_info,ARRAY_SIZE(default_boot_menu_info),0},
	{'5',"kernel ip dhcp",				NULL,NULL,kernel_ip_menu_info,ARRAY_SIZE(kernel_ip_menu_info),0},
	{'0',"setting network env",			NULL,NULL,net_env_menu_info,ARRAY_SIZE(net_env_menu_info),0},
	{'c',"Choice lcd type",				NULL,NULL,display_menu_item,ARRAY_SIZE(display_menu_item),0},
	{'s',"save setting",				"saveenv",NULL,NULL,0},
	{'q',"Return main Menu",			NULL,NULL,NULL,0,0},
};

/************* net menu info *************/
static menu_info_st net_menu_info[]={
		{0 , "tftp update images", NULL, NULL, NULL, 0, 0},
		{'1',"tftp update u-boot(u-boot.imx)",		"tq_tftp_update bootloader u-boot.imx",NULL,NULL,0,0},
		{'2',"tftp update linux kernel(zImage)",	"tq_tftp_update kernel zImage",&tftp_update_kernel,NULL,0,0},
		{'3',"tftp update dtb(imx6q-sabresd.dtb)",	"tq_tftp_update dtb imx6q-sabresd.dtb",NULL,NULL,0,0},
		{'4',"tftp update android kernel(boot.img)","tq_tftp_update kernel boot.img",&tftp_update_kernel,NULL,0,0},
		{'5',"tftp update logo(logo.bmp)",			"tq_update logo logo.bmp",NULL,NULL,0,0},
		/*{'6',"tftp update linux system(rootfs.img)",NULL,NULL,NULL,0,0},
		{'7',"tftp update android system(system.img)",NULL,NULL,NULL,0,0},
		{'8',"OneKey update",NULL,&tftp_update_onekey,NULL,0,0},*/
		{'0',"set tftp env",						NULL,&tftp_para_setting,NULL,0,0},
		{'s',"save setting",						"saveenv",NULL,NULL,0,0},
		{'q',"Return main Menu",NULL,NULL,NULL,0,0},
};

/************* sd menu info *************/
static menu_info_st sd_menu_info[]={
		{0 , "sd update images", NULL, NULL, NULL, 0, 0},
		{'1',"sd update u-boot(u-boot.imx)",		"tq_update bootloader u-boot.imx",&sdcard_update,NULL,0,0},
		{'2',"sd update linux kernel(zImage)",		"tq_update kernel zImage",&sd_update_kernel,NULL,0,0},
		{'3',"sd update android kernel(boot.img)",	"tq_update kernel boot.img",&sd_update_kernel,NULL,0,0},
		{'4',"sd update dtb(imx6q-sabresd.dtb)",	"tq_update dtb imx6q-sabresd.dtb",NULL,NULL,0,0},
		{'5',"sd update logo(logo.bmp)",			"tq_update logo logo.bmp",NULL,NULL,0,0},	
		{'6',"sd update linux system(rootfs.img)",	"tq_update system rootfs.img",NULL,NULL,0,0},
		{'7',"sd update android system(system.img)","tq_update system system.img",NULL,NULL,0,0},
		/*{'8',"sd Onekey update",NULL,NULL,NULL,0},*/
		{'q',"Return main menu:",NULL,NULL,NULL,0},
};

/************* main menu info *************/
static menu_info_st main_menu_info[]={
	{0 , "Boot for Main Menu", NULL, NULL, NULL, 0, 0},
	{'1',"SD update images",				NULL,NULL,sd_menu_info,ARRAY_SIZE(sd_menu_info),0},
	{'2',"TFTP update images",				NULL,NULL,net_menu_info,ARRAY_SIZE(net_menu_info),0},
	{'3',"USB update images",				NULL,NULL,NULL,0,0},
	{'8',"Boot the system",					"run bootcmd",NULL,NULL,0,0},
	{'9',"erase uboot env",					"env default -a;saveenv",NULL,NULL,0,0},
	{'0',"Set the boot parameters",			NULL,NULL,setting_menu_item,ARRAY_SIZE(setting_menu_item),0},
	{'s',"save setting",					"saveenv",NULL,NULL,0,0},
	{'q',"exit to command mode",			NULL,NULL,NULL,0,0},
};





static int parse_menu(const menu_info_st menu[], int array_size)
{
	int i;
	char key;
	//menu_info_st *menu = x->menu;
	while(1) {
		if(menu[0].help)
		{
			printf("\n#####     U-boot MENU         #####\n");
			printf(  "          [%s]\n", menu[0].help);
			printf(  "##########################################\n\n");
		}
		for(i = 1; i < array_size; i++)
		{
			printf("[%c] %s\n", menu[i].lead, menu[i].help);
		}
		printf("Please press a key to continue :");
		key = getc();
		key = tolower(key);
		printf("%c\n",key);
		if(key == 'q')
		{
				return  0;
		}
		for(i = 1; i < array_size; i++)
		{
			if(key == menu[i].lead)
			{				
				if(menu[i].next_menu_item !=NULL)
				{
					parse_menu(menu[i].next_menu_item, menu[i].menu_arraysize);
				}
				else if (menu[i].cmd !=NULL) {
					run_command(menu[i].cmd,0);
				}
				else if (menu[i].func !=NULL)
				{
					menu[i].func((void*)menu[i].item_para);
				}
				else
					printf("Nothing to do.\n");
				break;
			}
		}
	}
}

int do_menu(cmd_tbl_t *cmdtp,int flag,int argc,char *const argv[])
{
	parse_menu(main_menu_info, ARRAY_SIZE(main_menu_info));
	return 0;
}

U_BOOT_CMD(
	menu, 1, 1, do_menu,
	"Terminal command menu",
	"show terminal menu"
);
