/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* #define	DEBUG	*/

#include <common.h>
#include <autoboot.h>
#include <cli.h>
#include <version.h>
#include <tq_core.h>

#ifdef is_boot_from_usb
#include <environment.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
__weak void show_boot_progress(int val) {}

static void modem_init(void)
{
#ifdef CONFIG_MODEM_SUPPORT
	debug("DEBUG: main_loop:   gd->do_mdm_init=%lu\n", gd->do_mdm_init);
	if (gd->do_mdm_init) {
		char *str = getenv("mdm_cmd");

		setenv("preboot", str);  /* set or delete definition */
		mdm_init(); /* wait for modem connection */
	}
#endif  /* CONFIG_MODEM_SUPPORT */
}

static void run_preboot_environment_command(void)
{
#ifdef CONFIG_PREBOOT
	char *p;

	p = getenv("preboot");
	if (p != NULL) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

		run_command_list(p, -1, 0);

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}
#endif /* CONFIG_PREBOOT */
}

extern int tqboot_mode;
/* We come here after U-Boot is initialised and ready to process commands */
void main_loop(void)
{
	const char *s;

	bootstage_mark_name(BOOTSTAGE_ID_MAIN_LOOP, "main_loop");

#ifndef CONFIG_SYS_GENERIC_BOARD
	puts("Warning: Your board does not use generic board. Please read\n");
	puts("doc/README.generic-board and take action. Boards not\n");
	puts("upgraded by the late 2014 may break or be removed.\n");
#endif

	modem_init();
#ifdef CONFIG_VERSION_VARIABLE
	setenv("ver", version_string);  /* set version variable */
#endif /* CONFIG_VERSION_VARIABLE */

	cli_init();

	run_preboot_environment_command();

#if defined(CONFIG_UPDATE_TFTP)
	update_tftp(0UL);
#endif /* CONFIG_UPDATE_TFTP */

#ifdef	CONFIG_DISPLAY_LOGO
	printf("Fn:%s Ln:%d \n",__FUNCTION__,__LINE__);
	run_command("fatload mmc 2:1 0x15000000 logo.bmp",0);
	run_command("bmp display 0x15000000",0);
#endif
	printf("Fn:%s Ln:%d \n",__FUNCTION__,__LINE__);
	s = bootdelay_process();
	if (cli_process_fdt(&s))
		cli_secure_boot_cmd(s);

	embedsky_init();
	if(tqboot_mode == 0){
	        printf("Fn:%s Ln:%d \n",__FUNCTION__,__LINE__);
		autoboot_command("tq_ini");
		run_command("run bootcmd",0);
		/*s_env = getenv("download_root");
		if(s_env != NULL) {
			run_command("tq_boot sd",0);
		}*/
	} else {
	        printf("Fn:%s Ln:%d ==>s:%s \n",__FUNCTION__,__LINE__,s);
		autoboot_command(s);
	}
	printf("Fn:%s Ln:%d \n",__FUNCTION__,__LINE__);
	cli_loop();
}
