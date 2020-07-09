
#ifndef	_TQ_CORE_H_
#define	_TQ_CORE_H_

#include <common.h>
/*#include <asm/setup.h>*/

#define	TQ_PRODUCT_VERSION	"V8.10"

#define CONFIG_TQIMX6Q_4 1
#define	CONFIG_TQIMX6UL 0
#define	CONFIG_TQ335X 0
#define	CONFIG_TQIMX6Q_3 0
#define	CONFIG_TEST_BOARD 0

#define	CUR_VERSION 2015

#define	CONFIG_SWITCH_DISPLAY	0	/* 开启烧录进度条 */
#define	CONFIG_SWITCH_BACKLIGHT	1	/* 开启背光，1为开启，0为关闭 */
#define	CONFIG_FB_ARGS	1	/* 解析显示参数，传递给内核 */
#define	CONFIG_SWITCH_DATE		1	/* 启动时检测rtc时间 */
#define	CONFIG_SWITCH_ENCRPTY	0	/* 检测加密信息 */
#define	CONFIG_SWITCH_NAND	0		/* 存储芯片为nand */
#define	CONFIG_SWITCH_EMMC	1		/* 存储芯片为emmc */


#define	DEFAULT_DATE_INIT		"0101000010"	/* 2010-01-01 00:00 */

#define ATAG_EMBEDSKY	0x54410010
struct fb_common {
	const char* name;
	int width;
	int height;
	int hfp;
	int hbp;
	int hsw;
	int vfp;
	int vbp;
	int vsw;
	int pxl_clk;
	int bpp;
	char* rgb;
	int type;
	char* ldb_mode;
	void* fb_base;
	unsigned int fb_size;
};

/*#pragma pack(1)*/
struct display {
	int xres;
	int yres;
	int pix_clk;
	int hbp;
	int hfp;
	int hsw;
	int vbp;
	int vfp;
	int vsw;
	int rgb;
	int bpp;
	char is_type[20];
	char mode_str[20];
	char is_pass[10];
	char diffdisplay;
};

struct USB{
		char touch_pid[7];
		char touch_vid[7];
		char serial_pid[7];
		char serial_vid[7];
};
struct linux_args {
	struct display fb[4];
	struct USB usb_id;
	int nand_page;
};
/*#pragma pack()*/

extern struct linux_args largs;
extern struct fb_common* cur_fb;

extern void set_kernel_args(void);
extern void fdt_embedsky_setup(void *blob, bd_t *bd);

extern void embedsky_init(void);
extern struct fb_common* get_ini_fb(void);

#define BOARD_NAME      "TQIMX6Q_4" /*tanya*/
#define PRODUCT_VERSION "1.9"

#define TQ_UBOOT_VERSION(a)     (a)
#if (CUR_VERSION < TQ_UBOOT_VERSION(2017))
extern int sscanf(const char *buf, const char *fmt, ...);
#endif
#if	(CONFIG_SWITCH_NAND == 1) && (CONFIG_TQ335X == 1)
#include <nand.h>
extern int check_last_block(nand_info_t *nand, struct erase_info erase);
#endif

#endif
