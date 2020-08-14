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
#include <linux/fs.h>

#include "hi_unf_ecs.h"
#ifdef HI_BUILD_WITH_KEYLED
#include "hi_unf_keyled.h"
#endif

#include "fv.h"
#include "fv_face.h"
#include "fv_threads.h"
#include "fv_adapter.h"
#include "fv_dbg.h"

pthread_t key_thread_handle;

pthread_t led_thread_handle;


int ir_should_stop; 
int key_should_stop; 
int led_should_stop; 

void fv_show_name(void)
{
	//(void)HI_FV_DrawText(FV_ITEM_FV_NAME_TXT, FV_TOOL_NAME, strlen(FV_TOOL_NAME));
}

void fv_show_versions(void)
{
	int len = MAX_VERSION_LEN;
	char buf[MAX_VERSION_LEN];

	(void)HI_FV_DrawText(FV_ITEM_FV_VER_TXT, FV_TOOL_VERSION, strlen(FV_TOOL_VERSION));

	fv_get_sdk_version(buf, &len);
	(void)HI_FV_DrawText(FV_ITEM_SDK_VER_TXT, buf, len);

	len = MAX_VERSION_LEN;
	fv_get_app_version(buf, &len);
	(void)HI_FV_DrawText(FV_ITEM_APP_VER_TXT, buf, len);

	len = MAX_VERSION_LEN;
	fv_get_plat_version(buf, &len);
	(void)HI_FV_DrawText(FV_ITEM_PLAT_VER_TXT, buf, len);
	
	len = MAX_VERSION_LEN;
	fv_get_sn(buf, &len);
	(void)HI_FV_DrawText(FV_ITEM_PRODUCT_SN_TXT, buf, len);
}

void fv_show_factory_reset(void)
{
	(void)HI_FV_DrawText(FV_ITEM_FACTORY_RESET_TXT, FACTORY_RESET_STRING, strlen(FACTORY_RESET_STRING));
}

void fv_detect_flash(void)
{
	FILE *f;
	int fd;
	char buf[256], name[32], size[128], string[256];
	struct mtd_info_user info;
	unsigned long long nand_size = 0 , norspi_size = 0, emmc_size = 0;

       /* probe nand or spi device size */
	f = fopen("/proc/mtd", "r");
	if (!f) {
		fv_error("Fail to open /proc/mtd\n");
		return;
	}
	fgets(buf, sizeof(buf), f);

	while(fgets(buf, sizeof(buf), f)) {
		sscanf(buf, "%s: %s\n",name, size);
		snprintf(buf, sizeof(buf), "/dev/%s", name);
		buf[strlen(buf) - 1] = '\0';
		fv_debug("dev:%s.\n", buf);
		fd = open(buf, O_RDONLY);
		if (fd < 0) {
			fv_error("Fail to open %s\n", buf);
			continue;
		}
		if (ioctl(fd, MEMGETINFO, &info)) {
			fv_error("Fail to get info of %s\n", buf);
			close(fd);
			continue;
		}
		switch (info.type) {
			case MTD_NANDFLASH:
				nand_size += info.size;
				break;
			case MTD_NORFLASH:
				norspi_size += info.size;
				break;
			default	:
				fv_error("Unkown flash type:%d",info.type);
				break;
		}
		close(fd);
	}
	fclose(f);

       /* probe emmc0 device size*/
       if (!access("/dev/mmcblk0", F_OK))
       {
           fd = open("/dev/mmcblk0", O_RDONLY);
           if (-1 != fd)
           {
                ioctl(fd, BLKGETSIZE64, &emmc_size);
                close(fd);
           }   
       }

       /* show statistics */
	memset(string, 0, sizeof(string));

	fv_debug("nand_size:%llx(%dMB), spi_size:%llx(%dMB), mmcblk0_size:%llx(%dMB)\n",
		nand_size, (int)(nand_size >> 20), norspi_size, (int)(norspi_size >> 20), emmc_size, (int)(emmc_size >> 20));

       if (nand_size)
       {
            snprintf(buf, sizeof(buf), "nand:%dMB;", (int)(nand_size >> 20));
            strncat(string, buf, sizeof(string)); 
       }

       if (norspi_size)
       {
            snprintf(buf, sizeof(buf), "spi:%dMB;", (int)(norspi_size >> 20));
            strncat(string, buf, sizeof(string)); 
       }

        if (emmc_size)
        {
            snprintf(buf, sizeof(buf), "mmcblk0:%dMB;", (int)(emmc_size >> 20));
            strncat(string, buf, sizeof(string)); 
        }

	HI_FV_DrawText(FV_ITEM_FLASH_TXT, string, sizeof(string));
	return;
}

void fv_detect_ddr(void)
{
	char cmd_buf[128];
	int fd, sizeM;
	struct stat stt;
	int retry = 5;

	snprintf(cmd_buf, sizeof(cmd_buf), "insmod %s", DDR_DETECT_DRV);
	system(cmd_buf);
	
	/* We should wait for /dev/ddr_detect show up.
	 */
	while (stat("/dev/ddr_detect", &stt) && retry --)
		usleep(10000);
	if (!retry) {
		fv_error("Cannot stat /dev/ddr_detect after load module ddr.ko\n");
		snprintf(cmd_buf, sizeof(cmd_buf), "rmmod %s", DDR_DETECT_DRV);
		system(cmd_buf);	
		return;
	}

	fd = open("/dev/ddr_detect", O_RDONLY);
	if (fd < 0) {
		perror("Fail to open /dev/ddr_detect: ");
		return;
	}
	if (sizeof(int) != read(fd, &sizeM, sizeof(int))) {
		fv_error("Fail to read ddr size!\n");
		close(fd);
		goto out;
	}
	close(fd);
	memset(cmd_buf, 0, sizeof(cmd_buf));
	snprintf(cmd_buf, sizeof(cmd_buf),"%dMB", sizeM);
	HI_FV_DrawText(FV_ITEM_DDR_TXT, cmd_buf, sizeof(cmd_buf));
	snprintf(cmd_buf, sizeof(cmd_buf), "rmmod %s", DDR_DETECT_DRV);
	system(cmd_buf);	
out:
	return;
}

void *ir_thread(void *arg)
{
	int ret;
	HI_U64 ir_val;
	HI_U64 ir_val_last = 0;
	HI_UNF_KEY_STATUS_E ir_press;
	char buf[64];
	unsigned int vol = 100;

	ret = HI_UNF_IR_Open();;
	if (ret < 0) {
		printf("%s->%d fail to open ir device!\n",__func__,__LINE__);
		goto err_out;
	}

	HI_UNF_IR_EnableKeyUp(0);

	fv_tsplay_setVolume(vol);
	fv_draw_vol(vol);

	while (!ir_should_stop) {
		ret = HI_UNF_IR_GetValue(&ir_press, &ir_val, 200);
		if (ret)
			continue;

		fv_debug("ir_val:%llu\n", ir_val);
		
		if (ir_val == IR_RESET_FACTORY_KEY)
			fv_factory_reset();

		if (ir_val == IR_VOL_DEC_KEY) {
			if (vol != 0xff && vol > 0) {
				vol -= 4;
				if (vol <= 0)
					vol = 0;
				if (!fv_tsplay_setVolume(vol)) 
					fv_draw_vol(vol);
			}
		}
		else if (ir_val == IR_VOL_PLUS_KEY) {
			if (vol != 0xff && vol < 100) {
				vol += 4;
				if (vol >= 100)
					vol = 100;
				if (!fv_tsplay_setVolume(vol))
					fv_draw_vol(vol);
			}
		}

		if (ir_val_last != ir_val) {
			snprintf(buf, sizeof(buf), "0x%.8llx", ir_val);
			HI_FV_DrawText(FV_ITEM_IR_TXT, buf, sizeof(buf));
			ir_val_last = ir_val;
		}
	}
	HI_UNF_IR_Close();	
err_out:
	return (void *)0;
}

#ifdef HI_BUILD_WITH_KEYLED

HI_UNF_KEYLED_TYPE_E config_keyled_type = KEYLED_TYPE;

int  create_keyled_threads()
{
	if (HI_UNF_KEYLED_Init())
		goto err_out;

	if (HI_UNF_KEYLED_SelectType(config_keyled_type))
		goto err_select_type;

	if (HI_UNF_KEY_Open())
		goto err_key_open;

	if (HI_UNF_LED_Open())
		goto err_led_open;
		
	if (pthread_create(&key_thread_handle, NULL, key_thread, NULL)) 
		goto err_key;  
	
	if (pthread_create(&led_thread_handle, NULL, led_thread, NULL)) 
		goto err_led;

	return 0;

err_led:
	stop_key_thread();
	pthread_join(key_thread_handle, NULL);
err_key:
	HI_UNF_LED_Close();
err_led_open:
	HI_UNF_KEY_Close();
err_key_open:	
err_select_type:
	HI_UNF_KEYLED_DeInit();
err_out:
	return -1;
}

void destory_keyled_threads()
{	
	pthread_join(led_thread_handle, NULL);
	pthread_join(key_thread_handle, NULL);
	HI_UNF_LED_Close();
	HI_UNF_KEY_Close();
	HI_UNF_KEYLED_DeInit();
}

void *key_thread(void *arg)
{
	HI_U32 key_val, key_press, key_val_last = 0;
	char buf[64];
	HI_S32 s32Ret;
	
	while (!key_should_stop) {
		s32Ret = HI_UNF_KEY_GetValue(&key_press, &key_val);
		if ( s32Ret == HI_SUCCESS){
			if (key_val != key_val_last) {
				snprintf(buf, sizeof(buf),"%d", key_val);
				HI_FV_DrawText(FV_ITEM_KEY_TXT, buf, sizeof(buf));
				key_val_last = key_val;
			}
		}

		usleep(50000);
	}

	return (void *)0;
}

void *led_thread(void *arg)
{
	int i;
	int done = 0;
#define LED_TEST_DONE	"led tested"
	HI_U8 DigDisCode_std[10]    = {0x03, 0x9F, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09};
	HI_U8 DigDisCode_pt6961[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	HI_U8 DigDisCode_ct1642[10] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};
	HI_U8 DigDisCode_pt6964[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	HI_U8 DigDisCode[10] = {0};
	HI_UNF_KEYLED_TIME_S time;
	time.u32Hour = 12;
	time.u32Minute = 55;
	switch(config_keyled_type) {
		case 0:
			memcpy(DigDisCode, DigDisCode_std, sizeof(DigDisCode_std));
			break;
		case 1:
			memcpy(DigDisCode, DigDisCode_pt6961, sizeof(DigDisCode_pt6961));
			break;
		case 2:
			memcpy(DigDisCode, DigDisCode_ct1642, sizeof(DigDisCode_ct1642));
			break;
		case 3:
			memcpy(DigDisCode, DigDisCode_pt6964, sizeof(DigDisCode_pt6964));
			break;
		default :
			fv_error("Unkown keyled type!\n");
			return (void *)0;
	}

	HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_1);
	HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_ALL);
	
	while (!led_should_stop) {
		for (i = 0; i < sizeof(DigDisCode) && !led_should_stop; i++) {
			if (HI_UNF_LED_Display(DigDisCode[i] << 24 | DigDisCode[i] << 16
                                        | DigDisCode[i] << 8 | DigDisCode[i])) {
				fv_error("Fail to display led!\n");
				break;
			}
			/* 1000 ms */
			usleep(1000000);
		}
		
		if (HI_UNF_LED_DisplayTime(time)) {
			fv_error("Fail to display time !\n");
			break;
		}
		
		sleep(3);
		
		if (!done) {
			HI_FV_DrawText(FV_ITEM_LED_TXT, LED_TEST_DONE, strlen(LED_TEST_DONE));
			done = 1;
		}
	}

	return (void *)0;
}
#endif

void stop_key_thread(void)
{
	key_should_stop = 1;
}
void stop_ir_thread(void)
{
	ir_should_stop = 1;
}
void stop_led_thread(void)
{
	led_should_stop = 1;
}
