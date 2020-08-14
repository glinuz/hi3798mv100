#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mtd/mtd-abi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

#include "hi_unf_ecs.h"

#include "fv_dbg.h"
#include "fv_config.h"
#include "fv_adapter.h"

#include "fv.h"
#include "fv_face.h"
#include "fv_threads.h"

pthread_t ir_thread_handle;

pthread_t nonrt_thread_handle;

int nonrt_should_stop = 0;

int main_thread_stop = 0;
int should_main_thread_wait = 0;

struct fv_config_item fv_configs;

static int fv_read_configs(void)
{
	fv_get_configs(CONFIG_FILE_NAME, &fv_configs);
	return 0;
}

static void nonrt_thread_process()
{
	/* usb & sd card */	
	fv_storage_detect(MAX_USB_NR, MAX_SD_CARD_NR);

	/* net */ 
	fv_eth_detect();
    
	/* tuner */
	fv_tuner_detect(MAX_TUNER_NR, &fv_configs.tuner);

} 
void *nonrt_thread(void *arg)
{
	while (!nonrt_should_stop) {
		nonrt_thread_process();
		sleep(1);
	}
	return (void *)NULL;
}

static int fv_create_threads(void)
{
	fv_show_name();

	fv_show_versions();

	fv_show_factory_reset();

	fv_detect_flash();

	fv_detect_ddr();

	fv_clear_first_enter();

	fv_tsplay_enter(UDISK_MOUNT_POINT "/" VIDEO_FILE_NAME);	
	
	if (pthread_create(&nonrt_thread_handle, NULL, nonrt_thread, NULL))
		goto err_nonrt;

	if (pthread_create(&ir_thread_handle, NULL, ir_thread, NULL))
		goto err_ir;

	if (create_keyled_threads())
		goto err_keyled;
	
	return 0;
	
err_keyled:
	stop_ir_thread();
	pthread_join(ir_thread_handle,NULL);
err_ir:
	nonrt_should_stop = 1;
	pthread_join(nonrt_thread_handle, NULL);
err_nonrt:
	return -1;
}

static void fv_destroy_threads(void)
{
	if (!should_main_thread_wait)
		return;
	pthread_join(nonrt_thread_handle, NULL);
	pthread_join(ir_thread_handle, NULL);
	destory_keyled_threads();

	fv_tsplay_exit();
	return ;
}

static void fv_wait_exit(void)
{
	while(!main_thread_stop)
		sleep(3);
	nonrt_should_stop = 1;
	stop_key_thread();
	stop_ir_thread();
	stop_led_thread();
	/* TODO notify avplayer to stop */
}
static int fv_detect_entry(int force)
{
	int ret;
	int first = 0;
	main_thread_stop = 1;
	should_main_thread_wait = 0;

	ret = fv_is_first_enter(&first);
	if (ret)
		return 0;	

	fv_debug("> first:%d, force:%d .\n", first, force);
	if (!first && !force)
		return 0;

	main_thread_stop = 0;

	ret = fv_system_init();
	if (ret) {
		fv_error("Fail to init!\n");
		goto err_out;
	}

	fv_read_configs();

	ret = fv_face_on();
	if (ret) {
		fv_error("Fail to create main face!\n");
		goto err_face;
	}

	ret = fv_create_threads();
	if (ret) {
		fv_error("Fail to create threads!\n");
		goto err_threads;
	}
	should_main_thread_wait = 1;
	
	return 0;

err_threads:
	fv_face_off();
err_face:
	fv_system_exit();
err_out:
	return ret;
}
static void fv_detect_destroy(void)
{
	fv_destroy_threads();
	fv_face_off();
	fv_system_exit();
}

static void usage(char *base)
{
	printf("%s 0|1. 0 means not force to enter factory detect."\
		" 1 means force to enter.\n",base);
}
int main(int argc, char* argv[])
{
	int force = 0;

	if (argc > 2) {
		usage(argv[0]);
		return -1;
	}
	if (argc == 2)
		force = atoi(argv[1]);
	if (force < 0 || force > 1) {
		usage(argv[0]);
		return -1;
	}
	(void)fv_detect_entry(force);

	fv_wait_exit();

	fv_detect_destroy();

	return 0;
}


