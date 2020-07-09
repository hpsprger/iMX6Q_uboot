/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/errno.h>
#include <asm/imx-common/video.h>
#include <tq_core.h>

#define	TYPE_LCD	0x100
#define	TYPE_LVDS	0x101
#define	TYPE_HDMI	0x102
#define	TYPE_OFF	0x103
#define	TYPE_VGA	0x104

static int fb_from_ini(struct display_info_t* dis)
{
	struct fb_common* panel_fb;
	int ret;
	
	struct display_info_t* dis_info;
	
	panel_fb = get_ini_fb();
	if(panel_fb == NULL) {
		return 0;
	}
	
	if(panel_fb->type == TYPE_LVDS) {
		dis_info = &dis[display_count - 2];
		dis_info->mode.sync = FB_SYNC_EXT;
		ret = display_count - 2;
	} else if(panel_fb->type == TYPE_HDMI) {
		dis_info = &dis[display_count - 1];
		dis_info->mode.sync = 0;
		ret = display_count - 1;
	} else if((panel_fb->type == TYPE_LCD) || (panel_fb->type == TYPE_VGA)) {
		dis_info = &dis[display_count - 3];
		dis_info->mode.sync = 0;
		ret = display_count - 3;
	} else if(panel_fb->type == TYPE_OFF) {
		return display_count;
	} else {
		return 0;
	}
	
	if(!strcmp(panel_fb->rgb,"RGB565")) {
		dis_info->pixfmt = IPU_PIX_FMT_RGB565;
	} else if(!strcmp(panel_fb->rgb,"RGB666")) {
		dis_info->pixfmt = IPU_PIX_FMT_RGB666;
	} else if(!strcmp(panel_fb->rgb,"RGB24")) {
		dis_info->pixfmt = IPU_PIX_FMT_RGB24;
	} else {
		dis_info->pixfmt = IPU_PIX_FMT_RGB24;
	}
	dis_info->mode.xres = panel_fb->width;
	dis_info->mode.yres = panel_fb->height;
	dis_info->mode.left_margin = panel_fb->hbp;
	dis_info->mode.right_margin = panel_fb->hfp;
	dis_info->mode.upper_margin = panel_fb->vbp;
	dis_info->mode.lower_margin = panel_fb->vfp;
	dis_info->mode.hsync_len = panel_fb->hsw;
	dis_info->mode.vsync_len = panel_fb->vsw;
	dis_info->mode.pixclock = (1000000000/(panel_fb->pxl_clk/1000));
	//printf("dis_info->mode.pixclock = %d\n",dis_info->mode.pixclock);
	return ret;
}

int board_video_skip(void)
{
	int i;
	int ret;
	char const *panel = getenv("panel");

	if (!panel) {
		for (i = 0; i < display_count; i++) {
			struct display_info_t const *dev = displays+i;
			if (dev->detect && dev->detect(dev)) {
				panel = dev->mode.name;
				printf("auto-detected panel %s\n", panel);
				break;
			}
		}
		if (!panel) {
			panel = displays[0].mode.name;
			printf("No panel detected: default to %s\n", panel);
			i = 0;
		}
	} else {
		for (i = 0; i < display_count; i++) {
			if (!strcmp(panel, displays[i].mode.name))
				break;
		}
	}
	ret = fb_from_ini(displays);
	if(ret) {
		i = ret;
	}
	if (i < display_count) {
		ret = ipuv3_fb_init(&displays[i].mode, 0,
				    displays[i].pixfmt);
		if (!ret) {
			if (displays[i].enable)
				displays[i].enable(displays + i);

			printf("Display: %s (%ux%u)\n",
			       displays[i].mode.name,
			       displays[i].mode.xres,
			       displays[i].mode.yres);
		} else
			printf("LCD %s cannot be configured: %d\n",
			       displays[i].mode.name, ret);
	} else {
		printf("unsupported panel %s\n", panel);
		return -EINVAL;
	}
	
	return 0;
}

#ifdef CONFIG_IMX_HDMI
#include <asm/arch/mxc_hdmi.h>
#include <asm/io.h>
int detect_hdmi(struct display_info_t const *dev)
{
	struct hdmi_regs *hdmi	= (struct hdmi_regs *)HDMI_ARB_BASE_ADDR;
	return readb(&hdmi->phy_stat0) & HDMI_DVI_STAT;
}
#endif
