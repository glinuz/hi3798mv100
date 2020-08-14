#ifndef __FV_CONFIG_H__
#define __FV_CONFIG_H__

#define VIDEO_FILE_NAME "video.ts"
#define DDR_DETECT_DRV	"/root/ddr.ko"
#define CONFIG_FILE_NAME "/root/config.ini"
#ifndef FV_FONT_PATH
#define FV_FONT_PATH_720   "/root/res/DroidSansFallbackLegacy.ttf"
#endif

#define UDISK_MOUNT_POINT "/mnt"

/* language config */
#define FV_CONF_LANG_ZH 
#ifdef FV_CONF_LANG_ZH 
#include <fv_lables_zh.h>
#else
#include <fv_lables_en.h>
#endif

#ifndef FACTORY_RESET_STRING
#define FACTORY_RESET_STRING "Press power to reset factory setting"
#endif

#ifndef TUNER_DISCON_STRING
#define TUNER_DISCON_STRING "Not connect"
#endif

#ifndef TUNER_CONNECT_STRING
#define TUNER_CONNECT_STRING "Connected"
#endif

#ifndef FV_TOOL_VERSION
#define FV_TOOL_VERSION "V0.1"
#endif

#define  FV_ITEM_NULL_NAME       "N/A"

#define MAX_USB_NR 2
#define MAX_SD_CARD_NR 1

/* ir key value definition */
#define IR_VOL_PLUS_KEY	              (0x3ec1ff00)	
#define IR_VOL_DEC_KEY	              (0x6699ff00)
#define IR_RESET_FACTORY_KEY	(0x639cff00)

enum fv_item_list
{
	/* fv name and version */
	FV_ITEM_FV_NAME_TXT,
	FV_ITEM_FV_VER_TXT,

	/* product version */
	FV_ITEM_SDK_VER_TXT,
	FV_ITEM_APP_VER_TXT,
	FV_ITEM_PLAT_VER_TXT,

	/* product serial number*/
	FV_ITEM_PRODUCT_SN_LABLE,
	FV_ITEM_PRODUCT_SN_TXT,

	/* flash */
	FV_ITEM_FLASH_LABLE,
	FV_ITEM_FLASH_TXT,

	/* ddr */
	FV_ITEM_DDR_LABLE,
	FV_ITEM_DDR_TXT,
	
	/* usb */
	FV_ITEM_USB1_LABLE,
	FV_ITEM_USB1_TXT,
	FV_ITEM_USB2_LABLE,
	FV_ITEM_USB2_TXT,
		
	/* sd card */
	FV_ITEM_SD_LABLE,
	FV_ITEM_SD_TXT,
	
	/* net */
	FV_ITEM_WIRE_NET_LABLE,
	
	FV_ITEM_ETH0_NAME_TXT,
	FV_ITEM_ETH0_IP_LABLE,
	FV_ITEM_ETH0_IP_TXT,
	FV_ITEM_ETH0_MAC_LABLE,
	FV_ITEM_ETH0_MAC_TXT,
	FV_ITEM_ETH0_LINK_TXT,

	FV_ITEM_ETH1_NAME_TXT,
	FV_ITEM_ETH1_IP_LABLE,
	FV_ITEM_ETH1_IP_TXT,
	FV_ITEM_ETH1_MAC_LABLE,
	FV_ITEM_ETH1_MAC_TXT,
	FV_ITEM_ETH1_LINK_TXT,

    /*WIRELESS NET*/
    FV_ITEM_WIRELESS_NET_LABLE,
          
	FV_ITEM_WIRELESS_MODEL_LABLE,
	FV_ITEM_WIRELESS_MODEL_TXT,
	FV_ITEM_WIRELESS_MAC_LABLE,
	FV_ITEM_WIRELESS_MAC_TXT,
	FV_ITEM_WIRELESS_IP_LABLE,
	FV_ITEM_WIRELESS_IP_TXT,	

	/* ir, key, led */
	FV_ITEM_IR_LABEL,
	FV_ITEM_IR_TXT,
	
	FV_ITEM_KEY_LABLE,
	FV_ITEM_KEY_TXT,


	FV_ITEM_LED_TXT,

	/* tuner */
	FV_ITEM_TUNER_LABLE,
	FV_ITEM_TUNER_TXT,

	/* volume  */
	FV_ITEM_VOLUME_LABLE,
	FV_ITEM_VOLUME_TXT,
	FV_ITEM_VOLUME_VAL,
	
	/*reset config*/
	FV_ITEM_FACTORY_RESET_TXT,
	
	/* video */
	FV_ITEM_VIDEO_TXT,

	/* customer defines at here. */
	FV_ITEM_BUTT
};

typedef struct
{
    int x, y;

    int w, h;
} fv_rect;

typedef enum 
{
    zero =0,
   one = 1
} fv_bool;

typedef   struct item_para
{	
	enum fv_item_list ITEM_TYPE;	
	fv_rect rc;
	char *str;	
	fv_bool isLable;
}HI_ITEM_PARA;
#define FV_DEV_NA FV_ITEM_NULL_NAME
#undef FV_ETH_NETMASK

#define MAX_WIRED_NET_NR 2
#define MAX_WIRELESS_NET_NR 1

#ifdef HI_BUILD_WITH_KEYLED

#if defined(HI_KEYLED_74HC164)
#define KEYLED_TYPE (0)
#elif defined(HI_KEYLED_PT6961)
#define KEYLED_TYPE (1)
#elif defined(HI_KEYLED_CT1642)
#define KEYLED_TYPE (2)
#elif defined(HI_KEYLED_PT6964)
#define KEYLED_TYPE (3)
#elif defined(HI_KEYLED_FD650)
#define KEYLED_TYPE (4)
#elif defined(HI_KEYLED_GPIOKEY)
#define KEYLED_TYPE (5)
#else
#error "invalid keyled type."
#endif

#endif

struct fv_config_tuner
{
#define MAX_TUNER_NR	1
#define TUNER_SECTION	"tuner"

#define	FREQ_KEY	"freq" 
#define SYMBOL_RATE_BAND_KEY	"symbol_rate_or_band"
#define MODE_TYPE_POLAR_KEY	"mode_type_or_polar"
#define REVERSE_KEY	"reverse"

	/* connect param refer to HI_UNF_TUNER_CONNECT_PARA_S */
	int freq;
	int symbol_rate_or_band;
	int mode_type_or_polar;
	int reverse;
};
struct fv_config_item
{
	struct fv_config_tuner tuner;
};
int fv_get_configs(const char* ini_file, struct fv_config_item *config);
#endif
